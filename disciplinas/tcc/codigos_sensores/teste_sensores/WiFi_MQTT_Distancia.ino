//#include <WiFi.h>
#include <ESP8266WiFi.h> 
#include <PubSubClient.h>

//const char* ssid = "ProjetoTCC"; // colocar nome da minha rede
//const char* password = "danilo123"; // colocar senha da minha rede

const char* ssid = "RIBEIRO"; // colocar nome da minha rede
const char* password = "PaesRibeiro"; // colocar senha da minha rede
const int trigPin = 12;
const int echoPin = 14;

// Broker MQTT
const char* mqtt_server = "192.168.16.108";
const int mqtt_port = 1883;
const char* mqtt_topic = "IFSCTub/ETCC001/distancia";

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

void setup() {
  Serial.begin(115200);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  setup_wifi();
  mqttClient.setServer(mqtt_server, mqtt_port);
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

//proxima função

void loop() {
  char msg[200];
  verify_wifi();
  
  if (!mqttClient.connected()) {
    reconnect_mqtt();
  }
  mqttClient.loop();
  
  // Publicar no MQTT: Distancia
  snprintf(msg, sizeof(msg), "{\"distance\":%.2f}", distance_measure());
  mqttClient.publish(mqtt_topic, msg);
  Serial.print("MQTT Send: ");
  Serial.println(msg);
  delay(1000);

  // Publicar no MQTT: Proximo Sensor
  snprintf(msg, sizeof(msg), "{\"distance\":%.2f}", distance_measure());
  mqttClient.publish(mqtt_topic, msg);
  Serial.print("MQTT Send: ");
  Serial.println(msg);
  delay(1000);

  delay(5000);
}
