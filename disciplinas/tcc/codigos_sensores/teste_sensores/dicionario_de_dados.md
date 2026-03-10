# Dicionário de Dados
## Sistema de Monitoramento Bokashi com ESP32

Este documento descreve as variáveis, sensores, parâmetros e estruturas utilizadas no sistema de monitoramento do processo de compostagem Bokashi baseado em ESP32.

---

# 1. Sensores do Sistema

| Variável | Sensor | Tipo de Dado | Unidade | Descrição |
|---|---|---|---|---|
| temp | DHT22 | float | °C | Temperatura do ar do ambiente |
| hum | DHT22 | float | % | Umidade relativa do ar |
| dist | Ultrassônico HC-SR04 | float | cm | Distância medida para detectar nível de chorume |
| gas_mq2 | MQ-2 | int | ADC | Concentração de gases inflamáveis (principalmente CH4) |
| gas_mq135 | MQ-135 | int | ppm | Concentração de gases como NH3 e CO2 |
| soil_hum | Sensor de umidade do solo | int | ADC | Nível de umidade do composto |
| temp_soil | DS18B20 | float | °C | Temperatura interna do composto |

---

# 2. Variáveis Derivadas do Sistema

| Variável | Tipo | Unidade | Descrição |
|---|---|---|---|
| CH4 | int | ppm estimado | Estimativa de metano baseada no sensor MQ2 |
| NH3 | int | ppm | Concentração de amônia |
| CO2 | int | ppm | Concentração estimada de CO2 |
| UC | int | ADC | Umidade do composto |
| TC | float | °C | Temperatura do composto |
| TA | float | °C | Temperatura ambiente |
| UA | float | % | Umidade do ar |
| NI | float | cm | Nível de líquido/chorume |
| pH_estimado | float | pH | Estimativa de pH do composto |
| relacao_CN | float | razão | Relação carbono/nitrogênio estimada |

---

# 3. Variáveis de Controle do Sistema

| Variável | Tipo | Descrição |
|---|---|---|
| contador_ciclo | unsigned long | Contador de ciclos de leitura |
| dias | unsigned long | Número de dias de compostagem |
| status_ok | bool | Indica se o sistema está dentro dos parâmetros ideais |
| problemas | String | Lista de alertas detectados |
| fase | String | Fase atual da compostagem |

---

# 4. Limites Operacionais do Sistema

| Constante | Valor | Unidade | Descrição |
|---|---|---|---|
| TA_MIN | 15 | °C | Temperatura mínima do ambiente |
| TA_MAX | 35 | °C | Temperatura máxima do ambiente |
| UA_MIN | 40 | % | Umidade mínima do ar |
| UA_MAX | 80 | % | Umidade máxima do ar |
| CH4_MAX_BAIXO | 2000 | ppm | Limite de alerta para metano |
| CH4_MAX_CRITICO | 3000 | ppm | Limite crítico de metano |
| NH3_MAX | 800 | ppm | Limite máximo de amônia |
| CO2_MIN | 300 | ppm | Limite mínimo de CO2 |
| UC_MIN | 1500 | ADC | Umidade mínima do composto |
| UC_MAX | 3000 | ADC | Umidade máxima do composto |
| TC_MIN | 10 | °C | Temperatura mínima do composto |
| TC_MAX | 60 | °C | Temperatura máxima do composto |
| NI_MAX | 10 | cm | Nível máximo de chorume |

---

# 5. Pinos do Hardware

| Componente | Pino ESP32 | Descrição |
|---|---|---|
| DHT22 | GPIO 15 | Sensor de temperatura e umidade do ar |
| DS18B20 | GPIO 4 | Sensor de temperatura do solo |
| Ultrassônico TRIG | GPIO 26 | Pulso de disparo |
| Ultrassônico ECHO | GPIO 25 | Retorno do sinal |
| MQ-2 | GPIO 34 | Sensor de metano |
| MQ-135 | GPIO 35 | Sensor de gases |
| Umidade do solo | GPIO 36 | Sensor de umidade do composto |
| OLED SDA | GPIO 21 | Comunicação I2C |
| OLED SCL | GPIO 22 | Comunicação I2C |
| LED Verde | GPIO 2 | Indicação de sistema normal |
| LED Vermelho | GPIO 3 | Indicação de alerta |

---

# 6. Comunicação MQTT

| Variável | Tipo | Descrição |
|---|---|---|
| mqtt_server | string | Endereço do broker MQTT |
| mqtt_port | int | Porta de comunicação MQTT |
| topic_temp_air | string | Tópico da temperatura do ar |
| topic_hum_air | string | Tópico da umidade do ar |
| topic_gas_mq2 | string | Tópico do sensor MQ2 |
| topic_gas_mq135 | string | Tópico do sensor MQ135 |
| topic_dist | string | Tópico do nível de chorume |
| topic_soil_hum | string | Tópico da umidade do composto |
| topic_temp_soil | string | Tópico da temperatura do composto |

---

# 7. Fases da Compostagem

| Fase | Dias | Descrição |
|---|---|---|
| Inicial | 0–4 | Início da decomposição |
| Termofílica | 5–23 | Alta atividade microbiana |
| Maturação | 24+ | Estabilização do composto |

---

# Observações

- O sistema realiza leituras dos sensores a cada **5 minutos**.
- Os dados são enviados para um **broker MQTT** para monitoramento remoto.
- O display **OLED** apresenta informações locais sobre o processo de compostagem.
- LEDs indicam o **estado do sistema**:
  - Verde → funcionamento normal
  - Vermelho → alerta de parâmetros fora do ideal.
