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
#define ONE_WIRE_BUS 4 // Pino para o DS18B20
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// ===== Ultrassônico =====
#define TRIG_PIN 26
#define ECHO_PIN 25

// ===== MQ-2 =====
#define MQ2_PIN 34
MQ135 mq135(MQ2_PIN);

// ===== MQ-135 =====
#define MQ135_PIN 35 // Pino para o MQ-135

// ===== Sensor de Umidade do Solo HD-38 =====
#define SOIL_MOISTURE_PIN A0 // Pino para o sensor de umidade do solo HD-38

// ===== Limites Chorume (cm) =====
#define NIVEL_MEDIO 25
#define NIVEL_CRITICO 15

// ===== WiFi =====
const char* ssid = "RIBEIRO";
const char* password = "PaesRibeiro";

// ===== MQTT =====
const char* mqtt_server = "192.168.16.108";
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
  Serial.print("Conectando ao WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi conectado!");
  Serial.println(WiFi.localIP());
}

void verify_wifi() {
  if (WiFi.status() != WL_CONNECTED) {
    setup_wifi();
  }
}

void reconnect_mqtt() {
  while (!mqttClient.connected()) {
    Serial.print("Conectando ao MQTT...");
    if (mqttClient.connect("ESP32_ETCC001")) {
      Serial.println("Conectado!");
    } else {
      Serial.print("Erro: ");
      Serial.println(mqttClient.state());
      delay(5000);
    }
  }
}

// ---- Ultrassônico (com timeout) ----
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
  pinMode(MQ135_PIN, INPUT);
  pinMode(SOIL_MOISTURE_PIN, INPUT);

  Wire.begin(OLED_SDA, OLED_SCL);
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Iniciando sistema...");
  display.display();

  dht.begin();
  sensors.begin();
  setup_wifi();
  mqttClient.setServer(mqtt_server, mqtt_port);

  delay(1500);
}

// ===== LOOP =====
void loop() {
  verify_wifi();

  // Leitura dos sensores
  float temp = dht.readTemperature();
  float hum  = dht.readHumidity();
  float dist = medirDistancia();
  int gas_mq2    = analogRead(MQ2_PIN);
  int soil_hum   = analogRead(SOIL_MOISTURE_PIN);
  
  sensors.requestTemperatures();
  float temp_soil = sensors.getTempCByIndex(0);
  int gas_mq135 = mq135.getPPM();

  // Publica os dados no MQTT
  char msg[100];

  snprintf(msg, sizeof(msg), "%.2f", temp);
  mqttClient.publish(topic_temp_air, msg);

  snprintf(msg, sizeof(msg), "%.2f", hum);
  mqttClient.publish(topic_hum_air, msg);

  snprintf(msg, sizeof(msg), "%d", gas_mq2);
  mqttClient.publish(topic_gas_mq2, msg);

  snprintf(msg, sizeof(msg), "%d", gas_mq135);
  mqttClient.publish(topic_gas_mq135, msg);

  snprintf(msg, sizeof(msg), "%.2f", dist);
  mqttClient.publish(topic_dist, msg);

  snprintf(msg, sizeof(msg), "%d", soil_hum);
  mqttClient.publish(topic_soil_hum, msg);
  
  snprintf(msg, sizeof(msg), "%.2f", temp_soil);
  mqttClient.publish(topic_temp_soil, msg);

  // ===== OLED =====
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Monitoramento");
  display.println("----------------");

  display.print("Temp Ar: ");
  display.print(temp, 1);
  display.println(" C");

  display.print("Umid Ar: ");
  display.print(hum, 1);
  display.println(" %");

  display.print("Gas MQ2: ");
  display.println(gas_mq2);

  display.print("Gas MQ135: ");
  display.println(gas_mq135);

  display.print("Dist: ");
  if (dist < 0) display.println("Erro");
  else {
    display.print(dist, 1);
    display.println(" cm");
  }

  display.print("Umid Solo HD-38: ");
  display.println(soil_hum);

  display.print("Temp Solo DS18B20: ");
  display.println(temp_soil, 1);
  display.println(" C");

  // ===== ALERTA =====
  if (dist > 0 && dist <= NIVEL_CRITICO) {
    display.setTextSize(2);
    display.setCursor(5, 45);
    display.println("NIVEL");
    display.setCursor(10, 60);
    display.println("ALTO");
    display.setTextSize(1);
  }
  else if (dist > NIVEL_CRITICO && dist <= NIVEL_MEDIO) {
    if (millis() % 1000 < 500) {
      display.setCursor(30, 45);
      display.println("ATENCAO");
    }
  }

  display.display();
  delay(2000);

  if (!mqttClient.connected()) reconnect_mqtt();
  mqttClient.loop();
}
