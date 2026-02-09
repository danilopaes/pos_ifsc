#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

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

// ===== Ultrassônico =====
#define TRIG_PIN 26
#define ECHO_PIN 25

// ===== MQ-2 =====
#define MQ2_PIN 34

// ===== Limites Chorume (cm) =====
#define NIVEL_MEDIO 25
#define NIVEL_CRITICO 15

// ===== WiFi =====
const char* ssid = "RIBEIRO";
const char* password = "PaesRibeiro";

// ===== MQTT =====
const char* mqtt_server = "192.168.16.108";
const int mqtt_port = 1883;

const char* topic_temp  = "IFSCTub/ETCC001/temperatura";
const char* topic_hum   = "IFSCTub/ETCC001/umidade";
const char* topic_gas   = "IFSCTub/ETCC001/gas";
const char* topic_dist  = "IFSCTub/ETCC001/distancia";

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

  Wire.begin(OLED_SDA, OLED_SCL);
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Iniciando sistema...");
  display.display();

  dht.begin();

  setup_wifi();
  mqttClient.setServer(mqtt_server, mqtt_port);

  delay(1500);
}

// ===== LOOP =====
void loop() {
  verify_wifi();
  if (!mqttClient.connected()) reconnect_mqtt();
  mqttClient.loop();

  float temp = dht.readTemperature();
  float hum  = dht.readHumidity();
  float dist = medirDistancia();
  int gas    = analogRead(MQ2_PIN);

  char msg[100];

  snprintf(msg, sizeof(msg), "%.2f", temp);
  mqttClient.publish(topic_temp, msg);

  snprintf(msg, sizeof(msg), "%.2f", hum);
  mqttClient.publish(topic_hum, msg);

  snprintf(msg, sizeof(msg), "%d", gas);
  mqttClient.publish(topic_gas, msg);

  snprintf(msg, sizeof(msg), "%.2f", dist);
  mqttClient.publish(topic_dist, msg);

  // ===== OLED =====
  display.clearDisplay();
  display.setCursor(0, 0);

  display.println("Monitoramento");
  display.println("----------------");

  display.print("Temp: ");
  display.print(temp, 1);
  display.println(" C");

  display.print("Umid: ");
  display.print(hum, 1);
  display.println(" %");

  display.print("Gas: ");
  display.println(gas);

  display.print("Dist: ");
  if (dist < 0) display.println("Erro");
  else {
    display.print(dist, 1);
    display.println(" cm");
  }

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
}

