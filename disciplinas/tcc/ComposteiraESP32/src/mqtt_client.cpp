#include "mqtt_client.h"
#include "config.h"
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

namespace {
WiFiClient rede;
PubSubClient mqtt(rede);
unsigned long ultimaPublicacao = 0;
String idCliente;
}

void mqttIniciar(DadosSistema& dados) {
  mqtt.setServer(MQTT_HOST, MQTT_PORT);
  idCliente = String("composteira-") + String((uint32_t)ESP.getEfuseMac(), HEX);
  dados.mqttConectado = false;
}

void mqttAtualizar(DadosSistema& dados) {
  if (!dados.wifiConectado) { dados.mqttConectado = false; return; }
  if (!mqtt.connected()) {
    dados.mqttConectado = mqtt.connect(idCliente.c_str());
    if (!dados.mqttConectado) Serial.println(F("[MQTT] falha ao conectar"));
  }
  if (dados.mqttConectado) mqtt.loop();
  if (dados.mqttConectado && millis() - ultimaPublicacao >= INTERVALO_MQTT_MS) {
    mqttPublicar(dados);
    ultimaPublicacao = millis();
  }
}

void mqttPublicar(const DadosSistema& dados) {
  if (!mqtt.connected()) return;
  StaticJsonDocument<512> doc;
  const DadosSensores& s = dados.sensores;
  doc["temperatura"] = s.temperatura;
  doc["umidade"] = s.umidade;
  doc["mq2"] = s.mq2;
  doc["mq135"] = s.mq135;
  doc["solo"] = s.solo;
  doc["nivel"] = s.nivelPercentual;
  doc["estado"] = dados.estado;
  char payload[512];
  serializeJson(doc, payload, sizeof(payload));
  String topico = String(MQTT_TOPIC_PREFIX) + "status";
  if (!mqtt.publish(topico.c_str(), payload)) Serial.println(F("[MQTT] falha ao publicar"));
}
