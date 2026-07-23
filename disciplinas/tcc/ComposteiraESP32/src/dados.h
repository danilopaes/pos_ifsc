#pragma once
#include <Arduino.h>

struct DadosSensores {
  float temperatura = NAN;
  float umidade = NAN;
  float temperaturaSolo = NAN;
  int mq2 = 0;
  int mq135 = 0;
  int solo = 0;
  float distanciaCm = NAN;
  float nivelPercentual = NAN;
  bool temperaturaValida = false;
  bool umidadeValida = false;
  bool nivelValido = false;
  unsigned long atualizadoEm = 0;
};

struct DadosSistema {
  DadosSensores sensores;
  bool wifiConectado = false;
  bool mqttConectado = false;
  bool storageDisponivel = false;
  const char* estado = "INATIVO";
  unsigned long leituras = 0;
  unsigned long erros = 0;
};

const char* avaliarEstado(const DadosSensores& sensores);
void registrarTransicaoEstado(DadosSistema& dados);
