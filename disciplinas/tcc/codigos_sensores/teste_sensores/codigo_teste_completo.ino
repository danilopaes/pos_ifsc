
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

// ===== OLED =====
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// ===== DHT22 (CONFIG FIXA) =====
#define DHTPIN 15
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// ===== Ultrassônico (CONFIG FIXA) =====
#define TRIG_PIN 26
#define ECHO_PIN 25

// ===== MQ-2 =====
#define MQ2_PIN 34  // pino analógico

// ===== LIMITES NIVEL CHORUME (cm) =====
#define NIVEL_MEDIO 25
#define NIVEL_CRITICO 15

// ===== VARIAVEIS =====
float temp, umid;
float dist;
int gas;

// ===== FUNCAO ULTRASONICO =====
float medirDistancia() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duracao = pulseIn(ECHO_PIN, HIGH, 30000);
  if (duracao == 0) return -1;

  return (duracao * 0.0343) / 2;
}

void setup() {
  Serial.begin(115200);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  dht.begin();

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("Falha no OLED");
    while (true);
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
}

void loop() {
  temp = dht.readTemperature();
  umid = dht.readHumidity();
  dist = medirDistancia();
  gas = analogRead(MQ2_PIN);

  display.clearDisplay();

  // ===== DADOS GERAIS =====
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("Temp: ");
  display.print(isnan(temp) ? 0 : temp);
  display.println(" C");

  display.setCursor(0, 10);
  display.print("Umid: ");
  display.print(isnan(umid) ? 0 : umid);
  display.println(" %");

  display.setCursor(0, 20);
  display.print("Gas: ");
  display.println(gas);

  // ===== NIVEL CHORUME (SEMPRE VISIVEL) =====
  display.setCursor(0, 36);

  if (dist > NIVEL_MEDIO) {
    display.println("Nivel: BAIXO");
  }
  else if (dist > NIVEL_CRITICO && dist <= NIVEL_MEDIO) {
    display.println("Nivel: MEDIO");
  }
  else if (dist > 0 && dist <= NIVEL_CRITICO) {
    display.println("Nivel: ALTO");
  }
  else {
    display.println("Nivel: ERRO");
  }

  // ===== ALERTAS =====
  if (dist > 0 && dist <= NIVEL_CRITICO) {
    // CRITICO
    display.setCursor(28, 52);
    display.println("NIVEL ALTO");
  }
  else if (dist > NIVEL_CRITICO && dist <= NIVEL_MEDIO) {
    // ATENCAO (pisca)
    if ((millis() / 500) % 2 == 0) {
      display.setCursor(30, 52);
      display.println("ATENCAO");
    }
  }

  display.display();

  // ===== DEBUG SERIAL =====
  Serial.print("Temp: "); Serial.print(temp);
  Serial.print(" | Umid: "); Serial.print(umid);
  Serial.print(" | Dist: "); Serial.print(dist);
  Serial.print(" | Gas: "); Serial.println(gas);

  delay(500);
}
