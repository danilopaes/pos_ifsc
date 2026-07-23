# SPEC-009 — Integração

## Objetivo
Integrar os módulos usando timers baseados em `millis()` e garantir operação contínua.

## Agenda
- Sensores: 2 s.
- Display: 1 s.
- MQTT: 10 s.
- SD: 10 s.
- Wi-Fi, WebServer e MQTT: atendimento contínuo e não bloqueante.

## Critérios de aceitação
- Não usar `delay(600000)` nem loops de espera.
- Uma falha isolada deve ser degradada e registrada, não encerrar o programa.
- O estado apresentado no display, web, MQTT e SD deve ser o mesmo snapshot.
- Validar em bancada com sensores ausentes, Wi-Fi indisponível e SD removido.