# SPEC-005 — WebServer

## Objetivo
Disponibilizar uma página local e um endpoint JSON para diagnóstico e acompanhamento.

## Critérios
- `GET /` retorna uma página simples de status.
- `GET /api/status` retorna leituras, flags e estados de conectividade.
- Os dados são acessados por referência constante, sem cópia desnecessária.
- O servidor deve ser atendido periodicamente e não pode bloquear o `loop()`.
- A indisponibilidade da rede não deve corromper o estado do sistema.