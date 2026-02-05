#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

// ===== OLED =====
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define OLED_ADDR 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// ===== DHT22 =====
#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// ===== Ultrassônico =====
#define TRIG_PIN 18
#define ECHO_PIN 19

// ===== MQ-2 =====
#define MQ2_PIN 34

// ===== Limites de gás =====
#define GAS_ATENCAO 1200
#define GAS_CRITICO 2000

void setup() {
  Serial.begin(115200);

  Wire.begin(21, 22);
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.clearDisplay();

  dht.begin();

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  display.setTextColor(SSD1306_WHITE);
}

float medirDistancia() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duracao = pulseIn(ECHO_PIN, HIGH, 25000);
  if (duracao == 0) return -1;

  return duracao * 0.034 / 2;
}

void loop() {
  float temp = dht.readTemperature();
  float hum  = dht.readHumidity();
  float dist = medirDistancia();
  int gas = analogRead(MQ2_PIN);

  display.clearDisplay();

  // ===== DADOS NORMAIS =====
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("Temp: ");
  display.print(temp, 1);
  display.println(" C");

  display.print("Umid: ");
  display.print(hum, 1);
  display.println(" %");

  display.print("Dist: ");
  if (dist < 0) display.println("Erro");
  else {
    display.print(dist, 1);
    display.println(" cm");
  }

  display.print("Gas: ");
  display.println(gas);

  // ===== ALERTA DE GAS =====
  if (gas >= GAS_CRITICO) {
    // ALERTA CRITICO
    display.setTextSize(2);
    display.setCursor(10, 45);
    display.println("!!! GAS !!!");
  }
  else if (gas >= GAS_ATENCAO) {
    // ATENCAO (pisca)
    if (millis() % 1000 < 500) {
      display.setTextSize(1);
      display.setCursor(30, 45);
      display.println("ATENCAO");
    }
  }

  display.display();
  delay(500);
}

