#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "RWSS2F";
const char* password = "Doi39x-Wa!";
const int trigPin = 18;
const int echoPin = 19;

// Broker MQTT
const char* mqtt_server = "192.168.1.101";
const int mqtt_port = 1883;
const char* mqtt_topic = "tpiot/distancia";

WiFiClient espClient;
PubSubClient mqttClient(espClient);

void setup_wifi() {
  delay(10);
  Serial.print("Conectando ao WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500); Serial.print(".");
  }
  Serial.println("\nWiFi conectado com IP: " + WiFi.localIP().toString());
}

void verify_wifi() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Wi-Fi desconectado! Tentando reconectar...");
    setup_wifi();  // reconecta usando mesma função do setup
    return; // espera conectar antes de seguir
  }
}

void reconnect_mqtt() {
  while (!mqttClient.connected()) {
    Serial.print("Conectando ao broker MQTT...");
    if (mqttClient.connect("ESP32Client")) {
      Serial.println("Conectado");
    } else {
      Serial.print("Falha, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" tentando novamente em 5s...");
      delay(5000);
    }
  }
}

float distance_measure() {
  long duration;
  float distance;

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;
  
  Serial.print("Distância: ");
  Serial.print(distance);
  Serial.println(" cm");

  return distance;
}

void setup() {
  Serial.begin(115200);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  setup_wifi();
  mqttClient.setServer(mqtt_server, mqtt_port);
}

void loop() {
  verify_wifi();
  
  if (!mqttClient.connected()) {
    reconnect_mqtt();
  }
  mqttClient.loop();
  
  // Publicar no MQTT
  char msg[50];
  snprintf(msg, sizeof(msg), "%.2f", distance_measure());
  mqttClient.publish(mqtt_topic, msg);
  Serial.print("MQTT Send: ");
  Serial.println(msg);
  
  delay(5000);
}
