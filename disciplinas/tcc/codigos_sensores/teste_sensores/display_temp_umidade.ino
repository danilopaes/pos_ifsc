#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

// ---------------- OLED ----------------
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define OLED_ADDR 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// ---------------- DHT ----------------
#define DHTPIN 4
#define DHTTYPE DHT11   // ou DHT22
DHT dht(DHTPIN, DHTTYPE);

// ---------------- Ícones 8x8 ----------------

// ❄️ Frio
const unsigned char coldIcon[] PROGMEM = {
  0b00101000,
  0b00010000,
  0b11101110,
  0b00010000,
  0b00101000,
  0b00000000,
  0b00000000,
  0b00000000
};

// 🌤️ Normal
const unsigned char normalIcon[] PROGMEM = {
  0b00111000,
  0b01111100,
  0b11111110,
  0b00111000,
  0b00111000,
  0b00000000,
  0b00000000,
  0b00000000
};

// 🔥 Quente
const unsigned char hotIcon[] PROGMEM = {
  0b00110000,
  0b00110000,
  0b00110000,
  0b01111000,
  0b11111100,
  0b01111000,
  0b00110000,
  0b00000000
};

// ---------------- Variáveis ----------------
float tempAtual = 0;
float humAtual  = 0;

// ---------------- Função ícone ----------------
const unsigned char* getTempIcon(float temp) {
  if (temp < 18.0) return coldIcon;
  if (temp <= 26.0) return normalIcon;
  return hotIcon;
}

// ---------------- SETUP ----------------
void setup() {
  Wire.begin(21, 22);

  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.clearDisplay();

  dht.begin();

  display.setTextColor(SSD1306_WHITE);
}

// ---------------- LOOP ----------------
void loop() {
  float tempNova = dht.readTemperature();
  float humNova  = dht.readHumidity();

  if (isnan(tempNova) || isnan(humNova)) return;

  // Animação suave
  for (int i = 0; i <= 20; i++) {
    float tempAnim = tempAtual + (tempNova - tempAtual) * i / 20.0;
    float humAnim  = humAtual  + (humNova  - humAtual)  * i / 20.0;

    display.clearDisplay();

    display.setTextSize(1);
    display.setCursor(0, 0);
    display.println("Clima Ambiente");

    // Ícone temperatura dinâmico
    display.drawBitmap(
      0,
      20,
      getTempIcon(tempAnim),
      8,
      8,
      SSD1306_WHITE
    );

    // Texto temperatura
    display.setTextSize(2);
    display.setCursor(12, 18);
    display.print(tempAnim, 1);
    display.print("C");

    // Umidade
    display.setTextSize(1);
    display.setCursor(0, 45);
    display.print("U ");

    display.setTextSize(2);
    display.setCursor(12, 43);
    display.print(humAnim, 0);
    display.print("%");

    display.display();
    delay(30);
  }

  tempAtual = tempNova;
  humAtual  = humNova;

  delay(2000);
}

