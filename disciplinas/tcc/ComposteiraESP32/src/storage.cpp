#include "storage.h"
#include "config.h"
#include <SPI.h>
#include <SD.h>
#include <ArduinoJson.h>

namespace { const char* arquivoLog = "/leituras.jsonl"; }

void storageIniciar(DadosSistema& dados) {
  dados.storageDisponivel = SD.begin(PIN_SD_CS);
  if (!dados.storageDisponivel) Serial.println(F("[STORAGE] SD indisponivel"));
  else Serial.println(F("[STORAGE] SD pronto"));
}

void appendJsonLog(const DadosSistema& dados) {
  if (!dados.storageDisponivel) return;
  File arquivo = SD.open(arquivoLog, FILE_APPEND);
  if (!arquivo) {
    Serial.println(F("[STORAGE] falha ao abrir log"));
    return;
  }
  StaticJsonDocument<384> doc;
  const DadosSensores& s = dados.sensores;
  doc["ts"] = s.atualizadoEm;
  doc["temperatura"] = s.temperatura;
  doc["umidade"] = s.umidade;
  doc["mq2"] = s.mq2;
  doc["mq135"] = s.mq135;
  doc["solo"] = s.solo;
  doc["nivel"] = s.nivelPercentual;
  doc["estado"] = dados.estado;
  serializeJson(doc, arquivo);
  arquivo.println();
  arquivo.close();
}

void storageRegistrar(const DadosSistema& dados) { appendJsonLog(dados); }
