# SPEC-007 — Storage

## Objetivo
Registrar leituras no cartão SD para auditoria e operação offline.

## Critérios
- Inicializar o SD sem bloquear os demais módulos.
- Criar ou manter um arquivo de log em JSON Lines.
- `appendJsonLog()` deve acrescentar uma linha por rodada de registro.
- Erros de montagem e escrita devem ser informados na Serial.
- O sistema deve continuar funcionando quando o cartão estiver ausente.