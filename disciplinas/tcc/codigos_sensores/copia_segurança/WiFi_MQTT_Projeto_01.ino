#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// ===== OLED =====
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ----- Definições OLED -----
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDRESS 0x3C   // Endereço I2C padrão

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// ----- WiFi -----
const char* ssid = "RIBEIRO";
const char* password = "PaesRibeiro";

// ----- Ultrassônico -----
const int trigPin = 12;
const int echoPin = 14;

// ----- Sensor de Gás MQ-2 -----
const int gasPin = A0;  // Entrada analógica do ESP8266

// ---------- SENSOR DE GÁS-MQ4 ----------

// ---------- SENSOR DE SOLO ----------

// ---------- SENSOR DE TEMPERATURA E UMIDADE DO SOLO ----------  

// ---------- SENSOR DE TEMPERATURA E UMIDADE DO AR ----------

// ----- MQTT -----
const char* mqtt_server = "192.168.16.108";
const int mqtt_port = 1883;
const char* mqtt_topic_distance = "IFSCTub/ETCC001/distancia";
const char* mqtt_topic_gas = "IFSCTub/ETCC001/gas";

WiFiClient espClient;
PubSubClient mqttClient(espClient);

// ===== FUNÇÕES =====

void setup_wifi() {
  Serial.print("Conectando ao WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi conectado! IP: " + WiFi.localIP().toString());
}

void verify_wifi() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi caiu! Reconectando...");
    setup_wifi();
  }
}

void reconnect_mqtt() {
  while (!mqttClient.connected()) {
    Serial.print("Conectando ao MQTT...");
    if (mqttClient.connect("ESP8266Client")) {
      Serial.println("Conectado!");
    } else {
      Serial.print("Falha, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" tentando em 5s...");
      delay(5000);
    }
  }
}

// ---- Sensor Ultrassônico ----
float distance_measure() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  float distance = duration * 0.034 / 2;

  Serial.print("Distância: ");
  Serial.print(distance);
  Serial.println(" cm");

  return distance;
}

// ---- Sensor de Gás MQ-2 ----
int read_gas() {
  int sensorValue = analogRead(gasPin);

  Serial.print("Leitura do sensor de gás: ");
  Serial.println(sensorValue);

  return sensorValue;
}

// ===== SETUP =====
void setup() {
  Serial.begin(115200);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // ----- Inicialização OLED -----
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
    Serial.println(F("Falha ao iniciar o OLED"));
    while (true);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Sistema iniciando...");
  display.display();
  delay(1500);

  setup_wifi();
  mqttClient.setServer(mqtt_server, mqtt_port);
}

// ===== LOOP =====
void loop() {
  verify_wifi();
  if (!mqttClient.connected()) reconnect_mqtt();
  mqttClient.loop();

  char msg[200];

  // ---------- DISTÂNCIA ----------
  float distance = distance_measure();
  snprintf(msg, sizeof(msg), "{\"distance\":%.2f}", distance);
  mqttClient.publish(mqtt_topic_distance, msg);
  Serial.println(String("MQTT Send (Distância): ") + msg);

  // ---------- SENSOR DE GÁS MQ-2 ----------
  int gas_value = read_gas();
  snprintf(msg, sizeof(msg), "{\"gas\":%d}", gas_value);
  mqttClient.publish(mqtt_topic_gas, msg);
  Serial.println(String("MQTT Send (Gás): ") + msg);

  // ---------- SENSOR DE GÁS-MQ4 ----------

  // ---------- SENSOR DE SOLO ----------

  // ---------- SENSOR DE TEMPERATURA E UMIDADE DO SOLO ----------  

  // ---------- SENSOR DE TEMPERATURA E UMIDADE DO AR ----------

  // ===== Atualização do OLED =====
  display.clearDisplay();
  display.setCursor(0, 0);

  display.println("Monitoramento");
  display.println("----------------");

  display.print("Dist: ");
  display.print(distance, 1);
  display.println(" cm");

  display.print("Gas MQ2: ");
  display.println(gas_value);

  display.println();

  display.print("WiFi: ");
  display.println(WiFi.status() == WL_CONNECTED ? "OK" : "OFF");

  display.print("MQTT: ");
  display.println(mqttClient.connected() ? "OK" : "OFF");

  display.display();  // Atualiza tudo de uma vez (sem flicker)

  delay(2000); // Ajuste conforme necessário
}
