# SPEC-001 — Estrutura do projeto

## Objetivo
Organizar o firmware da composteira em módulos independentes, documentados e testáveis.

## Escopo
- `src/` contém o firmware Arduino.
- `test/` contém testes de bancada e contratos de integração.
- Cada módulo expõe uma API mínima por `.h` e implementa seu comportamento em `.cpp`.
- O arquivo `.ino` coordena os módulos e não contém regras de negócio extensas.

## Critérios de aceitação
- O projeto deve compilar para ESP32 com Arduino IDE ou PlatformIO.
- O `loop()` não pode conter `delay()` bloqueante.
- Estado compartilhado deve estar em `DadosSistema`.
- Dependências entre módulos devem ser explícitas nos headers.

## Entregáveis
Código-base, README, mapa de dependências e procedimento de compilação.