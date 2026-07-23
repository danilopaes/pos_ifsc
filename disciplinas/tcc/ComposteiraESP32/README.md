# ComposteiraESP32

Firmware modular para monitoramento de uma composteira Bokashi com ESP32, desenvolvido com **Specification-Driven Development**. Cada sprint começa por uma especificação em `specs/` e termina com implementação e procedimento de teste.

## Estrutura

- `specs/`: contratos funcionais e critérios de aceitação dos nove sprints.
- `src/`: firmware modular (`.h` e `.cpp`) e orquestração no `.ino`.
- `test/`: testes de bancada, contratos e evidências de validação.

## Sprints

| Sprint | Especificação | Entrega |
|---|---|---|
| 1 | SPEC-001 | Estrutura modular |
| 2 | SPEC-002 | Leitura dos sensores |
| 3 | SPEC-003 | Display OLED |
| 4 | SPEC-004 | Wi-Fi não bloqueante |
| 5 | SPEC-005 | WebServer e JSON |
| 6 | SPEC-006 | MQTT em `IFSCTub/ETCC001/` |
| 7 | SPEC-007 | Log JSON Lines no SD |
| 8 | SPEC-008 | Algoritmo Bokashi |
| 9 | SPEC-009 | Integração com `millis()` |

## Dependências

Bibliotecas Arduino: **DHT sensor library**, **OneWire**, **DallasTemperature**, **Adafruit GFX**, **Adafruit SSD1306**, **PubSubClient** e **ArduinoJson**. `WiFi`, `WebServer`, `SPI` e `SD` são fornecidas pelo core ESP32.

## Configuração

Edite `src/config.h` para informar SSID, senha, pinos, limites e broker. Não versione credenciais reais. O broker padrão é `192.168.16.109` e o prefixo MQTT é `IFSCTub/ETCC001/`.

## Compilação

1. Instale o pacote de placas ESP32 e as bibliotecas listadas.
2. Abra a pasta `src` como sketch no Arduino IDE, ou use o `platformio.ini` com PlatformIO.
3. Selecione uma placa ESP32 compatível.
4. Compile e faça upload.
5. Abra o monitor serial em 115200 baud.

## Procedimento de validação

1. Execute primeiro sem sensores, SD e rede para confirmar degradação segura.
2. Conecte o DHT22, DS18B20, MQ2, MQ135, sensor de solo e HC-SR04.
3. Confirme uma atualização de sensores a cada 2 s sem `delay()` longo.
4. Teste `/api/status` no IP exibido pelo ESP32.
5. Verifique a publicação em `IFSCTub/ETCC001/status`.
6. Remova o SD durante a operação e confirme que o loop continua.
7. Registre evidências em `test/` para o TCC.

## Nota de arquitetura

O `loop()` usa timers independentes baseados em `millis()`. Uma falha de display, Wi-Fi, MQTT ou SD é isolada e registrada na Serial. O mesmo snapshot `DadosSistema` alimenta OLED, WebServer, MQTT e armazenamento.
