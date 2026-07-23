#include "wifi_manager.h"
#include "config.h"
#include <WiFi.h>

namespace {
unsigned long iniciouTentativa = 0;
unsigned long proximaTentativa = 0;
bool tentando = false;
}

void wifiIniciar(DadosSistema& dados) {
  WiFi.mode(WIFI_STA);
  WiFi.setAutoReconnect(true);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  iniciouTentativa = millis();
  tentando = true;
  dados.wifiConectado = false;
}

void wifiAtualizar(DadosSistema& dados) {
  if (WiFi.status() == WL_CONNECTED) {
    if (!dados.wifiConectado) Serial.println(F("[WIFI] conectado"));
    dados.wifiConectado = true;
    tentando = false;
    return;
  }
  dados.wifiConectado = false;
  const unsigned long agora = millis();
  if (tentando && agora - iniciouTentativa < WIFI_TIMEOUT_MS) return;
  if (agora < proximaTentativa) return;
  WiFi.disconnect();
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  iniciouTentativa = agora;
  proximaTentativa = agora + WIFI_RETRY_MS;
  tentando = true;
  Serial.println(F("[WIFI] tentando reconectar"));
}
