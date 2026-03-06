# Sistema IoT para Monitoramento de Compostagem Bokashi

## Descrição

Este projeto implementa um sistema de monitoramento ambiental baseado em **ESP32** para acompanhamento do processo de compostagem Bokashi. O sistema coleta dados de múltiplos sensores, processa informações localmente e envia os dados para uma infraestrutura IoT utilizando o protocolo **MQTT**.

Os dados coletados podem ser armazenados e visualizados em ferramentas como **InfluxDB** e **Grafana**, permitindo análise histórica e monitoramento em tempo real.

---

# Arquitetura do Sistema

Sensores → ESP32 → MQTT Broker → Telegraf → InfluxDB → Grafana

O ESP32 atua como unidade central de aquisição e processamento, sendo responsável por:

* Coletar dados dos sensores
* Executar o algoritmo de análise do processo de compostagem
* Publicar dados via MQTT
* Exibir informações localmente em um display OLED
* Ativar alertas através de LEDs

---

# Hardware Utilizado

## Microcontrolador

* ESP32

## Sensores

| Sensor                    | Função                                    |
| ------------------------- | ----------------------------------------- |
| DHT22                     | Temperatura e umidade do ar               |
| DS18B20                   | Temperatura do solo                       |
| MQ-2                      | Detecção de metano (CH4)                  |
| MQ-135                    | Detecção de gases como amônia (NH3) e CO2 |
| Sensor ultrassônico       | Medição do nível de material ou chorume   |
| Sensor de umidade do solo | Umidade do composto                       |

## Atuadores

| Dispositivo  | Função                      |
| ------------ | --------------------------- |
| OLED SSD1306 | Exibição local de dados     |
| LED Verde    | Indicação de sistema normal |
| LED Vermelho | Indicação de alerta         |

---

# Pinagem do ESP32

| Componente        | GPIO |
| ----------------- | ---- |
| DHT22             | 15   |
| DS18B20           | 4    |
| Ultrassônico TRIG | 26   |
| Ultrassônico ECHO | 25   |
| MQ-2              | 34   |
| MQ-135            | 35   |
| Umidade do solo   | 36   |
| OLED SDA          | 21   |
| OLED SCL          | 22   |
| LED Verde         | 2    |
| LED Vermelho      | 3    |

---

# Bibliotecas Utilizadas

* WiFi
* PubSubClient
* Wire
* Adafruit_GFX
* Adafruit_SSD1306
* DHT
* OneWire
* DallasTemperature
* MQ135

Instalação via **Arduino IDE Library Manager**.

---

# Comunicação MQTT

## Broker

Porta padrão utilizada:

1883

## Estrutura de tópicos

IFSCTub/ETCC001/

| Tópico            | Descrição                      |
| ----------------- | ------------------------------ |
| temperatura_ar    | Temperatura ambiente           |
| umidade_ar        | Umidade ambiente               |
| gas_mq2           | Concentração de metano         |
| gas_mq135         | Concentração de amônia/CO2     |
| distancia         | Nível medido pelo ultrassônico |
| umidade_solo_hd38 | Umidade do composto            |
| temp_solo_ds18b20 | Temperatura do composto        |

---

# Algoritmo de Monitoramento

O sistema executa um algoritmo de análise baseado em parâmetros ambientais do processo de compostagem.

## Variáveis Monitoradas

* Temperatura ambiente
* Umidade ambiente
* Temperatura do composto
* Umidade do composto
* Metano
* Amônia
* CO2
* Nível de líquido

## Parâmetros Calculados

### Estimativa de pH

Uma estimativa empírica do pH é calculada com base em variáveis ambientais.

### Relação Carbono/Nitrogênio

Utilizada para avaliar o equilíbrio do processo de decomposição.

Valores ideais:

20 – 45

---

# Fases da Compostagem

| Fase        | Dias   |
| ----------- | ------ |
| Inicial     | 0 – 4  |
| Termofílica | 5 – 23 |
| Maturação   | 24+    |

---

# Sistema de Alertas

O sistema detecta condições fora dos limites ideais.

## Exemplos

* Excesso de metano
* Excesso de amônia
* Umidade inadequada
* Temperatura inadequada
* Relação C/N desequilibrada

Quando um problema é detectado:

* LED vermelho pisca
* OLED exibe alerta

---

# Intervalo de Coleta

O sistema realiza leituras a cada:

5 minutos

Total de leituras por dia:

288

---

# Display OLED

O display exibe:

* Fase da compostagem
* Dia do processo
* Temperatura do ar
* Umidade do ar
* Temperatura do composto
* Umidade do composto
* pH estimado
* Alertas do sistema

---

# Possíveis Extensões

Melhorias futuras incluem:

* Integração com sensores de pH reais
* Controle automático de irrigação
* Ventilação automática
* Dashboard avançado no Grafana
* Análise preditiva utilizando Machine Learning

---

# Aplicação

Este sistema foi desenvolvido como parte de um projeto acadêmico voltado à **Agricultura 4.0**, com foco em monitoramento ambiental e sustentabilidade no processo de compostagem.

---

# Licença

Projeto desenvolvido para fins acadêmicos.

