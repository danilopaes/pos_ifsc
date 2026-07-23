# SPEC-003 — Display OLED

## Objetivo
Exibir no OLED o estado resumido da composteira sem interromper as leituras.

## Critérios
- Atualizar no máximo a cada 1 s.
- Exibir temperatura, umidade, nível e estado do processo.
- Exibir `--` para valores inválidos.
- A falha do display não pode impedir Wi-Fi, MQTT ou armazenamento.
- Toda comunicação deve usar I2C e endereço configurável.