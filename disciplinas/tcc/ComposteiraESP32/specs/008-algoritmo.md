# SPEC-008 — Algoritmo Bokashi

## Objetivo
Classificar o estado da composteira a partir das leituras e limites reais do sistema.

## Critérios
- Calcular estado `INATIVO`, `ATENCAO`, `IDEAL` ou `ALERTA`.
- Usar limites centralizados em `config.h`.
- Valores inválidos devem produzir `ATENCAO`, nunca decisão silenciosa.
- A função deve ser determinística e independente de Wi-Fi/MQTT.
- Registrar transições de estado na Serial.