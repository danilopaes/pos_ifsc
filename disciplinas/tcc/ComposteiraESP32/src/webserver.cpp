#include "webserver.h"
#include <WebServer.h>
#include <ArduinoJson.h>

namespace {
WebServer servidor(80);
const DadosSistema* estado = nullptr;
}

void webserverIniciar(const DadosSistema& dados) {
  estado = &dados;
  servidor.on("/", HTTP_GET, []() {
    servidor.send(200, "text/html; charset=utf-8", "<h1>Composteira ESP32</h1><p>Acesse /api/status</p>");
  });
  servidor.on("/api/status", HTTP_GET, []() {
    if (estado == nullptr) { servidor.send(503, "application/json", "{\"erro\":\"indisponivel\"}"); return; }
    StaticJsonDocument<512> doc;
    const DadosSensores& s = estado->sensores;
    doc["temperatura"] = s.temperatura;
    doc["umidade"] = s.umidade;
    doc["temperaturaSolo"] = s.temperaturaSolo;
    doc["mq2"] = s.mq2;
    doc["mq135"] = s.mq135;
    doc["solo"] = s.solo;
    doc["nivel"] = s.nivelPercentual;
    doc["estado"] = estado->estado;
    doc["wifi"] = estado->wifiConectado;
    doc["mqtt"] = estado->mqttConectado;
    String resposta;
    serializeJson(doc, resposta);
    servidor.send(200, "application/json", resposta);
  });
  servidor.begin();
}

void webserverAtualizar() { servidor.handleClient(); }
