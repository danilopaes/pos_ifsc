#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

// ====== WiFi Config ======
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

// ====== MQTT Config ======
const char* mqtt_server = "192.168.1.100"; // IP or hostname of Mosquitto broker
const int mqtt_port = 1883;
const char* mqtt_topic = "sensors/dht22";
const char* mqtt_client_id = "esp32-sensor-1";

// ====== DHT Config ======
#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// ====== WiFi & MQTT Clients ======
WiFiClient espClient;
PubSubClient client(espClient);

// ====== WiFi Connection ======
void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

// ====== MQTT Reconnection ======
void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect(mqtt_client_id)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  dht.begin();
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Read sensor data
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    delay(2000);
    return;
  }

  // Create JSON payload
  char payload[100];
  snprintf(payload, sizeof(payload), "{\"temperature\": %.2f, \"humidity\": %.2f}", t, h);

  // Publish to MQTT
  Serial.print("Publishing message: ");
  Serial.println(payload);
  client.publish(mqtt_topic, payload);

  delay(5000); // wait 5 seconds before sending again
}

