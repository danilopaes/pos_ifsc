#include "config.h"
#include "dados.h"
#include "sensores.h"
#include "display.h"
#include "wifi_manager.h"
#include "mqtt_client.h"
#include "webserver.h"
#include "storage.h"

DadosSistema dados;
unsigned long ultimaLeitura = 0;
unsigned long ultimoDisplay = 0;
unsigned long ultimoStorage = 0;

void setup() {
  Serial.begin(115200);
  delay(100);
  Serial.println(F("[SISTEMA] iniciando ComposteiraESP32"));
  sensoresIniciar();
  displayIniciar();
  storageIniciar(dados);
  wifiIniciar(dados);
  mqttIniciar(dados);
  webserverIniciar(dados);
}

void loop() {
  const unsigned long agora = millis();
  wifiAtualizar(dados);
  webserverAtualizar();
  mqttAtualizar(dados);

  if (agora - ultimaLeitura >= INTERVALO_SENSORES_MS) {
    ultimaLeitura = agora;
    sensoresAtualizar(dados);
    dados.estado = avaliarEstado(dados.sensores);
    registrarTransicaoEstado(dados);
  }
  if (agora - ultimoDisplay >= INTERVALO_DISPLAY_MS) {
    ultimoDisplay = agora;
    displayAtualizar(dados);
  }
  if (agora - ultimoStorage >= INTERVALO_STORAGE_MS) {
    ultimoStorage = agora;
    storageRegistrar(dados);
  }
}
