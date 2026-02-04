#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ================= DISPLAY =================
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define OLED_ADDR 0x3C

#define SDA_PIN 21
#define SCL_PIN 22

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// ================= ANIMAÇÃO =================
int frame = 0;

void setup() {
  Wire.begin(SDA_PIN, SCL_PIN);

  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    while (true);
  }

  display.clearDisplay();
  display.display();
}

void drawStickman(int x, int y, bool step) {
  // Cabeça
  display.drawCircle(x, y - 18, 4, SSD1306_WHITE);

  // Corpo
  display.drawLine(x, y - 14, x, y, SSD1306_WHITE);

  // Braços
  if (step) {
    display.drawLine(x, y - 12, x - 8, y - 4, SSD1306_WHITE);
    display.drawLine(x, y - 12, x + 6, y - 10, SSD1306_WHITE);
  } else {
    display.drawLine(x, y - 12, x - 6, y - 10, SSD1306_WHITE);
    display.drawLine(x, y - 12, x + 8, y - 4, SSD1306_WHITE);
  }

  // Pernas
  if (step) {
    display.drawLine(x, y, x - 6, y + 10, SSD1306_WHITE);
    display.drawLine(x, y, x + 6, y + 6, SSD1306_WHITE);
  } else {
    display.drawLine(x, y, x - 6, y + 6, SSD1306_WHITE);
    display.drawLine(x, y, x + 6, y + 10, SSD1306_WHITE);
  }
}

void loop() {
  display.clearDisplay();

  int x = 20 + frame * 2;
  int y = 40;

  bool step = frame % 2;

  drawStickman(x, y, step);

  display.display();

  frame++;
  if (x > 110) frame = 0;

  delay(120);
}

