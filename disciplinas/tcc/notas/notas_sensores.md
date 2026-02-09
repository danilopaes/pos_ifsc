
# 📟 Sistema de Monitoramento com ESP32, OLED, DHT22, Ultrassônico e MQ-2

Este projeto implementa um sistema de monitoramento utilizando **ESP32**, capaz de exibir em um **display OLED SSD1306 (128x64 via I2C)**
informações de **temperatura**, **umidade**, **distância** e **nível de gás** em tempo real.

---

### 🧠 Componentes Utilizados

- ESP32 Dev Module
- Display OLED 0.96" SSD1306 (128x64 – I2C)
- Sensor de temperatura e umidade DHT22
- Sensor ultrassônico HC-SR04
- Sensor de gás MQ-2
- Resistores para divisor de tensão (ECHO do HC-SR04)
- Jumpers
- Protoboard

---

### 🔌 Ligações dos Componentes

### 📟 Display OLED SSD1306 (I2C)

| OLED | ESP32 |
|----|----|
| VCC | 3.3V |
| GND | GND |
| SDA | GPIO 21 |
| SCL | GPIO 22 |

> Endereço I2C utilizado: `0x3C`

---

### 🌡️ Sensor DHT22

| DHT22 | ESP32 |
|----|----|
| VCC | 3.3V |
| DATA | GPIO 15 |
| GND | GND |

> Utiliza a biblioteca `DHT`.

---

### 📏 Sensor Ultrassônico HC-SR04

| HC-SR04 | ESP32 |
|----|----|
| VCC | 5V |
| TRIG | GPIO 26 |
| ECHO | GPIO 25 (com divisor de tensão) |
| GND | GND |

⚠️ **Importante:**  
O pino **ECHO** do HC-SR04 trabalha em **5V**, enquanto o ESP32 aceita no máximo **3.3V**.  
Utilize um **divisor de tensão** (ex: 1kΩ + 2kΩ) para proteger o microcontrolador.

---

### 🔥 Sensor de Gás MQ-2

| MQ-2 | ESP32 |
|----|----|
| VCC | 5V |
| AO | GPIO 34 |
| GND | GND |

> O GPIO 34 é **somente entrada analógica**, ideal para leitura do MQ-2.

---

### 📚 Bibliotecas Necessárias

Instale via **Gerenciador de Bibliotecas da Arduino IDE**:

- `Adafruit SSD1306`
- `Adafruit GFX`
- `DHT sensor library`
- `Adafruit Unified Sensor`

---

### ⚙️ Funcionamento do Sistema

O sistema realiza as seguintes leituras:

- **Temperatura (°C)** – via DHT22
- **Umidade relativa (%)** – via DHT22
- **Distância (cm)** – via HC-SR04
- **Nível de gás (valor analógico)** – via MQ-2

Todas as informações são atualizadas a cada **2 segundos** e exibidas no display OLED.

---

### 🖥️ Exibição no Display OLED

Formato exibido:

Temp: 25.3 C
Umid: 60.2 %
Dist: 45.8 cm
Gas: 1234


Caso algum sensor falhe, o sistema exibe mensagens de erro específicas, como:
- `Falha DHT22`
- `Dist: Erro`

---

### 🧪 Testes Realizados

- ✔️ Inicialização do display OLED
- ✔️ Leitura correta de temperatura e umidade
- ✔️ Medição de distância com timeout de segurança
- ✔️ Leitura analógica estável do MQ-2
- ✔️ Atualização contínua no display

---

### 🛠️ Observações Técnicas

- O barramento I2C do ESP32 foi inicializado manualmente nos pinos:
  - SDA → GPIO 21
  - SCL → GPIO 22
- O tempo máximo do `pulseIn()` foi limitado para evitar travamentos.
- O sistema foi estruturado para fácil expansão (ícones, alertas, MQTT, etc.).

---

### 🚀 Possíveis Melhorias Futuras

- Implementação de ícones gráficos no OLED
- Alertas visuais para níveis críticos de gás
- Integração com MQTT / Wi-Fi
- Registro de dados em nuvem
- Alternância automática de telas

---

### 📄 Autor

Projeto desenvolvido por **Danilo**  
Plataforma: ESP32  
Linguagem: C++ (Arduino Framework)

---

### Estrutura FINAL do sistema (conceitual)

setup()
 ├─ Serial
 ├─ OLED
 ├─ DHT22
 ├─ Ultrassônico
 ├─ WiFi
 └─ MQTT

loop()
 ├─ Verifica WiFi
 ├─ Verifica MQTT
 ├─ Le sensores
 │   ├─ Temperatura
 │   ├─ Umidade
 │   ├─ Gás
 │   └─ Distância
 ├─ Publica MQTT
 ├─ Atualiza OLED
 │   ├─ Dados
 │   ├─ Status
 │   └─ Alertas
 └─ Delay controlado

---

### Repositorio dos Dados e Códigos

link: https://github.com/danilopaes/pos_ifsc/tree/main/disciplinas/tcc
