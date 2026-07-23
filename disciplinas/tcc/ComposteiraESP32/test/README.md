# Testes da ComposteiraESP32

Os testes são executados em bancada e documentados por sprint. Cada registro deve conter data, versão do firmware, hardware usado, procedimento, resultado esperado, resultado observado e evidência.

## Matriz inicial

- **T-001:** compilar o projeto para `esp32dev`.
- **T-002:** executar sem sensores e confirmar flags inválidas sem travamento.
- **T-003:** medir atualização do DHT/nível em aproximadamente 2 s.
- **T-004:** desligar o roteador e verificar reconexão sem bloqueio.
- **T-005:** validar `GET /api/status`.
- **T-006:** observar `IFSCTub/ETCC001/status` no broker.
- **T-007:** remover o SD e confirmar operação offline.
- **T-008:** testar as quatro classificações do algoritmo.
- **T-009:** manter o firmware ativo por 30 minutos e registrar contadores.
