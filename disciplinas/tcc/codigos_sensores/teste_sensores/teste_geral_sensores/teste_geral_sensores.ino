#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <MQ135.h>

// ===== OLED =====
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDR 0x3C
#define OLED_SDA 21
#define OLED_SCL 22

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// ===== DHT22 =====
#define DHTPIN 15
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// ===== DS18B20 =====
#define ONE_WIRE_BUS 4
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// ===== Ultrassônico =====
#define TRIG_PIN 26
#define ECHO_PIN 25

// ===== MQ2 =====
#define MQ2_PIN 34

// ===== MQ135 =====
#define MQ135_PIN 35
MQ135 mq135(MQ135_PIN);

// ===== Umidade do Solo =====
#define SOIL_MOISTURE_PIN 33

// ===== LEDs =====
#define LED_VERDE 2
#define LED_VERMELHO 3


// ===== FUNÇÃO DISTANCIA =====
float medirDistancia() {

  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);

  digitalWrite(TRIG_PIN, LOW);

  long duracao = pulseIn(ECHO_PIN, HIGH, 30000);

  if (duracao == 0) return -1;

  return duracao * 0.034 / 2;
}


void setup() {

  Serial.begin(115200);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_VERMELHO, OUTPUT);

  // OLED
  Wire.begin(OLED_SDA, OLED_SCL);

  if(!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)){
    Serial.println("Erro OLED");
    while(true);
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  dht.begin();
  sensors.begin();

  Serial.println("TESTE DE SENSORES INICIADO");
}


void loop() {

  // ===== LEITURAS =====

  float temp_ar = dht.readTemperature();
  float hum_ar = dht.readHumidity();

  sensors.requestTemperatures();
  float temp_solo = sensors.getTempCByIndex(0);

  float distancia = medirDistancia();

  int gas_mq2 = analogRead(MQ2_PIN);
  int gas_mq135 = mq135.getPPM();

  int umidade_solo = analogRead(SOIL_MOISTURE_PIN);


  // ===== SERIAL =====

  Serial.println("------ LEITURAS ------");

  Serial.print("Temp Ar: ");
  Serial.println(temp_ar);

  Serial.print("Umid Ar: ");
  Serial.println(hum_ar);

  Serial.print("Temp Solo: ");
  Serial.println(temp_solo);

  Serial.print("Distancia: ");
  Serial.println(distancia);

  Serial.print("MQ2: ");
  Serial.println(gas_mq2);

  Serial.print("MQ135: ");
  Serial.println(gas_mq135);

  Serial.print("Umidade Solo: ");
  Serial.println(umidade_solo);

  Serial.println("----------------------");


  // ===== OLED =====

  display.clearDisplay();

  display.setTextSize(1);
  display.setCursor(0,0);

  display.println("TESTE SENSORES");

  display.print("TA:");
  display.println(temp_ar);

  display.print("UA:");
  display.println(hum_ar);

  display.print("TS:");
  display.println(temp_solo);

  display.print("Dist:");
  display.println(distancia);

  display.print("MQ2:");
  display.println(gas_mq2);

  display.print("Solo:");
  display.println(umidade_solo);

  display.display();


  // ===== LED TESTE =====

  digitalWrite(LED_VERDE, HIGH);
  digitalWrite(LED_VERMELHO, LOW);

  delay(2000);
}