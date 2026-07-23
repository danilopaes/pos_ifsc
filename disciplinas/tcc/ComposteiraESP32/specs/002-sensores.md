# SPEC-002 — Sensores

## Objetivo
Realizar a leitura periódica dos sensores da composteira e atualizar `DadosSensores`.

## Entradas
DHT22 (temperatura/umidade), DS18B20 (temperatura do solo), MQ2, MQ135, sensor de umidade do solo e HC-SR04 (nível).

## Saídas
Valores válidos, timestamp da leitura e flags de validade em `DadosSensores`.

## Critérios
- Uma rodada de leitura deve ser iniciada no máximo a cada 2 s.
- Leituras inválidas devem resultar em `NAN` ou flag falsa.
- Erros devem ser registrados na Serial.
- A API não pode bloquear o `loop()`.
- A distância deve ser convertida para percentual de ocupação com limites configuráveis.