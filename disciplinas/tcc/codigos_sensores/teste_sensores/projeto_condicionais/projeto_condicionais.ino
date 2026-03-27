#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <MQ135.h>

// ===== OLED =====
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDR 0x3C
#define OLED_SDA 21
#define OLED_SCL 22
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// ===== DHT22 =====
#define DHTPIN 15
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// ===== DS18B20 =====
#define ONE_WIRE_BUS 4
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// ===== Ultrassônico =====
#define TRIG_PIN 26
#define ECHO_PIN 25

// ===== MQ-2 =====
#define MQ2_PIN 34

// ===== MQ-135 =====
#define MQ135_PIN 35
MQ135 mq135(MQ135_PIN);

// ===== Umidade Solo =====
#define SOIL_MOISTURE_PIN 36

// ===== LIMITES =====
#define TA_MIN 15
#define TA_MAX 35
#define UA_MIN 40
#define UA_MAX 80

#define CH4_MAX_BAIXO 2000
#define CH4_MAX_CRITICO 3000
#define NH3_MAX 800
#define CO2_MIN 300

#define UC_MIN 1500
#define UC_MAX 3000

#define TC_MIN 10
#define TC_MAX 60

#define NI_MAX 10

// ===== CONTROLE =====
unsigned long contador_ciclo = 0;
unsigned long dias = 0;

// ===== WiFi =====
const char* ssid = "RIBEIRO";
const char* password = "PaesRibeiro";

// ===== MQTT =====
const char* mqtt_server = "192.168.16.111";
const int mqtt_port = 1883;

const char* topic_temp_air  = "IFSCTub/ETCC001/temperatura_ar";
const char* topic_hum_air   = "IFSCTub/ETCC001/umidade_ar";
const char* topic_gas_mq2   = "IFSCTub/ETCC001/gas_mq2";
const char* topic_gas_mq135 = "IFSCTub/ETCC001/gas_mq135";
const char* topic_dist      = "IFSCTub/ETCC001/distancia";
const char* topic_soil_hum  = "IFSCTub/ETCC001/umidade_solo_hd38";
const char* topic_temp_soil = "IFSCTub/ETCC001/temp_solo_ds18b20";

WiFiClient espClient;
PubSubClient mqttClient(espClient);

// ===== FUNÇÕES =====
void setup_wifi() {

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

}

void verify_wifi() {

  if (WiFi.status() != WL_CONNECTED) {
    setup_wifi();
  }

}

void reconnect_mqtt() {

  while (!mqttClient.connected()) {

    mqttClient.connect("ESP32_ETCC001");
    delay(2000);

  }

}

float medirDistancia() {

  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);

  digitalWrite(TRIG_PIN, LOW);

  long duracao = pulseIn(ECHO_PIN, HIGH, 25000);

  if (duracao == 0) return -1;

  return duracao * 0.034 / 2;

}

// ===== SETUP =====
void setup() {

  Serial.begin(115200);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  Wire.begin(OLED_SDA, OLED_SCL);

  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  dht.begin();
  sensors.begin();

  setup_wifi();

  mqttClient.setServer(mqtt_server, mqtt_port);

}

// ===== LOOP =====
void loop() {

  verify_wifi();

  if (!mqttClient.connected()) reconnect_mqtt();

  mqttClient.loop();

  contador_ciclo++;

  // ===== LEITURA SENSORES =====

  float temp = dht.readTemperature();
  float hum  = dht.readHumidity();
  float dist = medirDistancia();

  int gas_mq2 = analogRead(MQ2_PIN);
  int soil_hum = analogRead(SOIL_MOISTURE_PIN);

  sensors.requestTemperatures();
  float temp_soil = sensors.getTempCByIndex(0);

  float gas_mq135 = mq135.getPPM();

  // ===== PUBLICAÇÃO MQTT =====

  char msg[50];

  snprintf(msg, sizeof(msg), "%.2f", temp);
  mqttClient.publish(topic_temp_air, msg);

  snprintf(msg, sizeof(msg), "%.2f", hum);
  mqttClient.publish(topic_hum_air, msg);

  snprintf(msg, sizeof(msg), "%d", gas_mq2);
  mqttClient.publish(topic_gas_mq2, msg);

  snprintf(msg, sizeof(msg), "%.2f", gas_mq135);
  mqttClient.publish(topic_gas_mq135, msg);

  snprintf(msg, sizeof(msg), "%.2f", dist);
  mqttClient.publish(topic_dist, msg);

  snprintf(msg, sizeof(msg), "%d", soil_hum);
  mqttClient.publish(topic_soil_hum, msg);

  snprintf(msg, sizeof(msg), "%.2f", temp_soil);
  mqttClient.publish(topic_temp_soil, msg);

  // ===== ALGORITMO BOKASHI =====

  int CH4 = gas_mq2;
  float NH3 = gas_mq135;
  float CO2 = gas_mq135;

  int UC = soil_hum;
  float TC = temp_soil;

  float TA = temp;
  float UA = hum;

  float NI = dist;

  float pH_estimado = (NH3 * 0.0001 + UC * 0.00005 - TC * 0.02) + 6.0;
  float relacao_CN = (CO2 * 1.0 / (NH3 + 1));

  bool status_ok = true;
  String problemas = "";

  if (CH4 > CH4_MAX_CRITICO) { problemas += "CH4 CRITICO "; status_ok = false; }
  else if (CH4 > CH4_MAX_BAIXO) { problemas += "CH4 Alto "; status_ok = false; }

  if (NH3 > NH3_MAX) { problemas += "NH3 Alto "; status_ok = false; }
  if (CO2 < CO2_MIN) { problemas += "CO2 Baixo "; status_ok = false; }

  if (relacao_CN < 20) { problemas += "Falta C "; status_ok = false; }
  if (relacao_CN > 45) { problemas += "Excesso C "; status_ok = false; }

  if (TC < TC_MIN || TC > TC_MAX) { problemas += "Temp Solo "; status_ok = false; }
  if (UC < UC_MIN || UC > UC_MAX) { problemas += "Umid Solo "; status_ok = false; }
  if (NI > NI_MAX && NI > 0) { problemas += "Chorume "; status_ok = false; }

  if (TA < TA_MIN || TA > TA_MAX || UA < UA_MIN || UA > UA_MAX) {
    problemas += "Ambiente ";
    status_ok = false;
  }

  String fase;

  if (dias <= 4) fase = "Inicial";
  else if (dias <= 23) fase = "Termofilica";
  else fase = "Maturacao";

  if (contador_ciclo % 288 == 0) dias++;

  // ===== OLED =====

  display.clearDisplay();
  display.setCursor(0,0);
  display.setTextSize(1);

  display.println("Monitoramento");
  display.print("Fase: "); display.println(fase);
  display.print("Dia: "); display.println(dias);

  display.print("TA: "); display.print(TA); display.print(" UA: "); display.println(UA);
  display.print("TC: "); display.print(TC); display.print(" UC: "); display.println(UC);
  display.print("pH: "); display.println(pH_estimado);

  if (!status_ok) {
    display.println("ALERTA:");
    display.println(problemas);
  }

  display.display();

  delay(300000); // 5 minutos

}