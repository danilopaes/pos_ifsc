#include "display.h"
#include "config.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

namespace {
Adafruit_SSD1306 oled(128, 64, &Wire, -1);
bool disponivel = false;
}

void displayIniciar() {
  disponivel = oled.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS);
  if (!disponivel) {
    Serial.println(F("[DISPLAY] OLED indisponivel"));
    return;
  }
  oled.clearDisplay();
  oled.setTextColor(SSD1306_WHITE);
  oled.setTextSize(1);
  oled.display();
}

void displayAtualizar(const DadosSistema& dados) {
  if (!disponivel) return;
  const DadosSensores& s = dados.sensores;
  oled.clearDisplay();
  oled.setCursor(0, 0);
  oled.print(F("Composteira ESP32"));
  oled.setCursor(0, 14);
  oled.print(F("T: ")); s.temperaturaValida ? oled.print(s.temperatura, 1) : oled.print(F("--")); oled.print(F(" C"));
  oled.setCursor(0, 26);
  oled.print(F("U: ")); s.umidadeValida ? oled.print(s.umidade, 1) : oled.print(F("--")); oled.print(F(" %"));
  oled.setCursor(0, 38);
  oled.print(F("Nivel: ")); s.nivelValido ? oled.print(s.nivelPercentual, 0) : oled.print(F("--")); oled.print(F(" %"));
  oled.setCursor(0, 50);
  oled.print(dados.estado);
  oled.print(F(" WiFi:")); oled.print(dados.wifiConectado ? F("OK") : F("--"));
  oled.display();
}
