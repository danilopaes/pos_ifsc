#include "sensores.h"
#include "config.h"
#include <DHT.h>
#include <OneWire.h>
#include <DallasTemperature.h>

namespace {
DHT dht(PIN_DHT, PIN_DHT_TYPE);
OneWire oneWire(PIN_ONEWIRE);
DallasTemperature ds18b20(&oneWire);
}

void sensoresIniciar() {
  pinMode(PIN_MQ2, INPUT);
  pinMode(PIN_MQ135, INPUT);
  pinMode(PIN_SOIL, INPUT);
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
  dht.begin();
  ds18b20.begin();
}

float medirDistancia() {
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);
  const unsigned long duracao = pulseIn(PIN_ECHO, HIGH, 30000UL);
  if (duracao == 0) return NAN;
  return duracao * 0.0343f / 2.0f;
}

void sensoresAtualizar(DadosSistema& dados) {
  const float temperatura = dht.readTemperature();
  const float umidade = dht.readHumidity();
  ds18b20.requestTemperatures();
  const float temperaturaSolo = ds18b20.getTempCByIndex(0);
  const float distancia = medirDistancia();

  dados.sensores.temperatura = temperatura;
  dados.sensores.umidade = umidade;
  dados.sensores.temperaturaSolo = temperaturaSolo;
  dados.sensores.temperaturaValida = !isnan(temperatura);
  dados.sensores.umidadeValida = !isnan(umidade);
  dados.sensores.mq2 = analogRead(PIN_MQ2);
  dados.sensores.mq135 = analogRead(PIN_MQ135);
  dados.sensores.solo = analogRead(PIN_SOIL);
  dados.sensores.distanciaCm = distancia;
  dados.sensores.nivelValido = !isnan(distancia);
  if (dados.sensores.nivelValido) {
    dados.sensores.nivelPercentual = constrain(
      (DISTANCIA_VAZIO_CM - distancia) * 100.0f /
        (DISTANCIA_VAZIO_CM - DISTANCIA_CHEIO_CM), 0.0f, 100.0f);
  } else {
    dados.sensores.nivelPercentual = NAN;
  }
  dados.sensores.atualizadoEm = millis();
  dados.leituras++;
  if (!dados.sensores.temperaturaValida || !dados.sensores.umidadeValida) {
    dados.erros++;
    Serial.println(F("[SENSORES] DHT22 invalido"));
  }
}
