// ===== BIBLIOTECAS =====
#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <MQ135.h>
#include <SD.h>
#include <SPI.h>
#include <time.h>

// ===== HARDWARE =====
#define OLED_SDA 21
#define OLED_SCL 22
#define OLED_ADDR 0x3C

#define DHTPIN 15
#define ONE_WIRE_BUS 4
#define TRIG_PIN 26
#define ECHO_PIN 25

#define MQ2_PIN 34
#define MQ135_PIN 35
#define SOIL_PIN 36

#define SD_CS 5

// ===== LIMITES =====
#define TEMP_AR_MAX 30
#define UMID_AR_MIN 10
#define TEMP_SOLO_MAX 30
#define UMID_SOLO_MIN 10
#define CHORUME_ALTO 15
#define CHORUME_CRITICO 20

// ===== OBJETOS =====
Adafruit_SSD1306 display(128, 64, &Wire, -1);
DHT dht(DHTPIN, DHT22);
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
MQ135 mq135(MQ135_PIN);

WiFiClient espClient;
PubSubClient mqttClient(espClient);

// ===== WIFI =====
const char* ssid = "RIBEIRO";
const char* password = "PaesRibeiro";

// ===== MQTT =====
const char* mqtt_server = "192.168.16.111";

// ===== TÓPICOS =====
const char* topic_temp_ar      = "IFSCTub/ETCC001/temperatura_ar";
const char* topic_umidade_ar   = "IFSCTub/ETCC001/umidade_ar";
const char* topic_temp_solo    = "IFSCTub/ETCC001/temperatura_solo";
const char* topic_umidade_solo = "IFSCTub/ETCC001/umidade_solo";
const char* topic_mq2          = "IFSCTub/ETCC001/gas_mq2";
const char* topic_mq135        = "IFSCTub/ETCC001/gas_mq135";
const char* topic_nivel        = "IFSCTub/ETCC001/nivel_chorume";
const char* topic_alerta       = "IFSCTub/ETCC001/alertas";
const char* topic_json         = "IFSCTub/ETCC001/json";

// ===== WIFI =====
void setup_wifi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) delay(500);
}

// ===== TEMPO =====
String getTimestamp() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) return "erro";

  char buffer[25];
  strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timeinfo);
  return String(buffer);
}

String getDate() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) return "erro";

  char buffer[15];
  strftime(buffer, sizeof(buffer), "%Y-%m-%d", &timeinfo);
  return String(buffer);
}

// ===== SD =====
void garantirPasta() {
  if (!SD.exists("/logs")) SD.mkdir("/logs");
}

void salvarLog(String json) {
  garantirPasta();
  String file = "/logs/" + getDate() + ".json";

  File f = SD.open(file, FILE_APPEND);
  if (!f) return;

  f.println(json);
  f.close();
}

// ===== REPLAY SD =====
void reenviarLogsSD() {

  String fileName = "/logs/" + getDate() + ".json";

  File file = SD.open(fileName);
  if (!file) {
    Serial.println("Sem log para reenviar");
    return;
  }

  Serial.println("Reenviando logs do SD...");

  while (file.available()) {
    String linha = file.readStringUntil('\n');
    linha.trim();

    if (linha.length() > 10) {
      mqttClient.publish(topic_json, linha.c_str());
      mqttClient.loop();
      delay(10);
    }
  }

  file.close();
  Serial.println("Reenvio concluido");
}

// ===== MQTT =====
void reconnect_mqtt() {

  while (!mqttClient.connected()) {

    Serial.println("Conectando MQTT...");

    if (mqttClient.connect("ESP32_TCC_MAX")) {

      Serial.println("MQTT conectado");

      // 🔥 REPLAY AUTOMÁTICO
      reenviarLogsSD();

    } else {
      Serial.println("Falha MQTT");
      delay(2000);
    }
  }
}

// ===== DISTANCIA =====
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

// ===== ALERTAS =====
String gerarAlertas(float TA, float UA, float TS, int US, float nivel) {

  String alerta = "";

  if (TA > TEMP_AR_MAX) alerta += "Temp AR ALTA\n";
  if (UA < UMID_AR_MIN) alerta += "Umid AR BAIXA\n";

  if (TS > TEMP_SOLO_MAX) alerta += "Temp SOLO ALTA\n";
  if (US < UMID_SOLO_MIN) alerta += "Umid SOLO BAIXA\n";

  if (nivel > CHORUME_CRITICO) alerta += "CHORUME CRITICO\n";
  else if (nivel > CHORUME_ALTO) alerta += "CHORUME ALTO\n";

  return alerta;
}

// ===== DISPLAY =====
void atualizarDisplay(float TA, float UA, float TS, int US, int MQ2, float MQ135v, float nivel, String alerta) {

  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0,0);

  display.println("AR:");
  display.print("T: "); display.println(TA);
  display.print("U: "); display.println(UA);

  display.println("SOLO:");
  display.print("T: "); display.println(TS);
  display.print("U: "); display.println(US);

  display.println("GASES:");
  display.print("MQ2: "); display.println(MQ2);
  display.print("MQ135: "); display.println(MQ135v);

  display.print("Nivel: "); display.println(nivel);

  if (alerta != "") {
    display.println("ALERTA:");
    display.println(alerta);
  }

  display.display();
}

// ===== SETUP =====
void setup() {

  Serial.begin(115200);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  Wire.begin(OLED_SDA, OLED_SCL);
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);

  dht.begin();
  sensors.begin();

  setup_wifi();

  mqttClient.setServer(mqtt_server, 1883);

  configTime(-3 * 3600, 0, "pool.ntp.org");

  SD.begin(SD_CS);
}

// ===== LOOP =====
void loop() {

  if (WiFi.status() != WL_CONNECTED) setup_wifi();
  if (!mqttClient.connected()) reconnect_mqtt();

  mqttClient.loop();

  float TA = dht.readTemperature();
  float UA = dht.readHumidity();

  sensors.requestTemperatures();
  float TS = sensors.getTempCByIndex(0);

  int US = analogRead(SOIL_PIN);
  int MQ2 = analogRead(MQ2_PIN);
  float MQ135v = mq135.getPPM();

  float nivel = medirDistancia();

  String alerta = gerarAlertas(TA, UA, TS, US, nivel);

  char msg[50];

  snprintf(msg, sizeof(msg), "%.2f", TA);
  mqttClient.publish(topic_temp_ar, msg);

  snprintf(msg, sizeof(msg), "%.2f", UA);
  mqttClient.publish(topic_umidade_ar, msg);

  snprintf(msg, sizeof(msg), "%.2f", TS);
  mqttClient.publish(topic_temp_solo, msg);

  snprintf(msg, sizeof(msg), "%d", US);
  mqttClient.publish(topic_umidade_solo, msg);

  snprintf(msg, sizeof(msg), "%d", MQ2);
  mqttClient.publish(topic_mq2, msg);

  snprintf(msg, sizeof(msg), "%.2f", MQ135v);
  mqttClient.publish(topic_mq135, msg);

  snprintf(msg, sizeof(msg), "%.2f", nivel);
  mqttClient.publish(topic_nivel, msg);

  mqttClient.publish(topic_alerta, alerta.c_str());

  String json = "{";
  json += "\"timestamp\":\"" + getTimestamp() + "\",";
  json += "\"temp_ar\":" + String(TA) + ",";
  json += "\"umidade_ar\":" + String(UA) + ",";
  json += "\"temp_solo\":" + String(TS) + ",";
  json += "\"umidade_solo\":" + String(US) + ",";
  json += "\"mq2\":" + String(MQ2) + ",";
  json += "\"mq135\":" + String(MQ135v) + ",";
  json += "\"nivel\":" + String(nivel);
  json += "}";

  salvarLog(json);
  mqttClient.publish(topic_json, json.c_str());

  atualizarDisplay(TA, UA, TS, US, MQ2, MQ135v, nivel, alerta);

  delay(300000);
}
