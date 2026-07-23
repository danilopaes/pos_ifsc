# SPEC-004 — Wi-Fi

## Objetivo
Conectar automaticamente ao Wi-Fi e manter a conectividade sem `while` bloqueante.

## Critérios
- Tentar conexão por no máximo 30 s em cada ciclo.
- Reconectar automaticamente usando intervalo configurável.
- Registrar falha na Serial e manter o firmware executando offline.
- Atualizar `dados.wifiConectado`.
- Credenciais devem ser configuradas em `config.h`, sem hardcode em lógica de negócio.