
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

// -------- OLED --------
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define OLED_ADDR 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// -------- DHT22 --------
#define DHTPIN 15
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// -------- Ultrassônico --------
#define TRIG_PIN 26
#define ECHO_PIN 25

// -------- MQ-2 --------
#define MQ2_PIN 34

void setup() {
  Serial.begin(115200);

  // I2C padrão do ESP32
  Wire.begin(21, 22);

  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println("Falha no OLED");
    while (true);
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  dht.begin();

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Sistema iniciado");
  display.display();
  delay(2000);
}

float lerDistancia() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duracao = pulseIn(ECHO_PIN, HIGH, 30000);
  if (duracao == 0) return -1;

  return duracao * 0.034 / 2;
}

void loop() {
  float temp = dht.readTemperature();
  float umid = dht.readHumidity();
  float distancia = lerDistancia();
  int gas = analogRead(MQ2_PIN);

  display.clearDisplay();
  display.setCursor(0, 0);

  if (isnan(temp) || isnan(umid)) {
    display.println("Falha DHT22");
  } else {
    display.print("Temp: ");
    display.print(temp, 1);
    display.println(" C");

    display.print("Umid: ");
    display.print(umid, 1);
    display.println(" %");
  }

  display.print("Dist: ");
  if (distancia < 0) {
    display.println("Erro");
  } else {
    display.print(distancia, 1);
    display.println(" cm");
  }

  display.print("Gas: ");
  display.println(gas);

  display.display();
  delay(2000);
}
