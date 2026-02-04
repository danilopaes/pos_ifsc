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

// ================= TEXTO APAIXONADO =================
const char* mensagem =
  "Voce e o motivo do meu sorriso todos os dias.\n"
  "Meu coracao encontrou paz em voce.\n"
  "Te amar e a melhor parte de mim.";   // <-- PONTO E VÍRGULA AQUI ✅

int yScroll = SCREEN_HEIGHT;

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

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, yScroll);

  display.print(mensagem);

  display.display();

  yScroll--;

  // Reinicia quando o texto sair da tela
  if (yScroll < -40) {
    yScroll = SCREEN_HEIGHT;
  }

  delay(40);
}




