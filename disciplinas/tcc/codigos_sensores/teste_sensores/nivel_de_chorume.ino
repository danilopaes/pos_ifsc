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

// ===== Limites Chorume (cm) =====
#define NIVEL_MEDIO 25
#define NIVEL_CRITICO 15

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

void setup() {
  Wire.begin(21, 22);
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.clearDisplay();

  dht.begin();

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  display.setTextColor(SSD1306_WHITE);
}

void loop() {
  float temp = dht.readTemperature();
  float hum  = dht.readHumidity();
  float dist = medirDistancia();
  int gas = analogRead(MQ2_PIN);

  display.clearDisplay();
  display.setTextSize(1);

  display.setCursor(0, 0);
  display.print("Temp: ");
  display.print(temp, 1);
  display.println(" C");

  display.print("Umid: ");
  display.print(hum, 1);
  display.println(" %");

  display.print("Gas: ");
  display.println(gas);

  display.print("Dist: ");
  if (dist < 0) display.println("Erro");
  else {
    display.print(dist, 1);
    display.println(" cm");
  }

  // ===== ALERTA NIVEL CHORUME =====
  if (dist > 0 && dist <= NIVEL_CRITICO) {
    // CRITICO
    display.setTextSize(2);
    display.setCursor(5, 45);
    display.println("NIVEL");
    display.setCursor(10, 60);
    display.println("ALTO");
  }
  else if (dist > NIVEL_CRITICO && dist <= NIVEL_MEDIO) {
    // ATENCAO (pisca)
    if (millis() % 1000 < 500) {
      display.setCursor(30, 45);
      display.println("ATENCAO");
    }
  }

  display.display();
  delay(500);
}

