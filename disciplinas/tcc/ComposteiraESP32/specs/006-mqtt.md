# SPEC-006 — MQTT

## Objetivo
Publicar o estado da composteira em um broker MQTT.

## Configuração
- Broker padrão: `192.168.16.109`.
- Prefixo dos tópicos: `IFSCTub/ETCC001/`.
- Publicações: `temperatura`, `umidade`, `mq2`, `mq135`, `solo`, `nivel` e `status`.

## Critérios
- Reconectar automaticamente sem travar o firmware.
- Publicar JSON consistente em intervalo configurável.
- Confirmar a tentativa de publicação e registrar erros.
- Corrigir o mapeamento dos tópicos para não trocar leituras.
- MQTT indisponível não pode impedir display, sensores ou SD.