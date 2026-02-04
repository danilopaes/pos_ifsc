#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <math.h>

// ================= DISPLAY =================
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define OLED_ADDR 0x3C

#define SDA_PIN 21
#define SCL_PIN 22

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// ================= ANIMAÇÃO =================
#define CENTER_X 64
#define CENTER_Y 32
#define SCALE 22        // tamanho do infinito
#define STEPS 120       // suavidade
#define SPEED 2         // velocidade da animação

float t = 0;

void setup() {
  Wire.begin(SDA_PIN, SCL_PIN);

  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    while (true);
  }

  display.clearDisplay();
  display.display();
}

void loop() {
  display.clearDisplay();

  float lastX = 0;
  float lastY = 0;
  bool firstPoint = true;

  // Desenha o traço animado
  for (int i = 0; i < STEPS; i++) {
    float angle = (t + i) * 0.05;

    // Equação da lemniscata (∞)
    float x = SCALE * sin(angle);
    float y = SCALE * sin(angle) * cos(angle);

    int screenX = CENTER_X + x;
    int screenY = CENTER_Y + y;

    if (!firstPoint) {
      display.drawLine(lastX, lastY, screenX, screenY, SSD1306_WHITE);
    }

    lastX = screenX;
    lastY = screenY;
    firstPoint = false;
  }

  display.display();

  t += SPEED;
  delay(30);
}

