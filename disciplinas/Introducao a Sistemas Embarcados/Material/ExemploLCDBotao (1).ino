#include <Wire.h>
#include "DHT.h"
#include <LiquidCrystal_I2C.h>
#include "Ultrasonic.h"

#define col 16
#define lines 2
#define address 0x27

#define BTN1 4
#define BTN2 5

#define LED1 2
#define LED2 3

#define DHT_PIN 8
#define DHT_TYPE DHT22

Ultrasonic utrasom(7,6); //Ultrassom Primeiro pino de Trigger, Segundo pino de Echo
LiquidCrystal_I2C lcd(address, col, lines);
DHT dht(DHT_PIN, DHT_TYPE);

void setup(){
  dht.begin();

  pinMode(BTN1, INPUT);
  pinMode(BTN2, INPUT);

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);

  lcd.init();
  lcd.backlight();
  lcd.clear();

  lcd.setCursor(0,0);
  lcd.print("Hello");
  delay(2000);
}
 
void loop(){
  int click1 = digitalRead(BTN1);
  int click2 = digitalRead(BTN2);
  int exit = 0;
  int exit1 = 0;
  int contador = 8;

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Escolha uma");
  lcd.setCursor(0,1);
  lcd.print("opcao!");

  if(click1){
    digitalWrite(LED1, HIGH);
    while(exit1 == 0){
      if(contador == 8){
        float temp = dht.readTemperature();
        int humi = dht.readHumidity();
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Temp: ");
        lcd.print(temp);
        lcd.print("C");
        lcd.setCursor(0,1);
        lcd.print("Umi: ");
        lcd.print(humi);
        lcd.print("%");
        contador == 0;
      }else{
        contador++;
      }
      exit1 = digitalRead(BTN1);
      if(exit1){
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Saiu do 1");
        delay(500);
      }
      delay(250);
    }
    digitalWrite(LED1, LOW);
  }

  if(click2){
    digitalWrite(LED2, HIGH);
    while(exit == 0){
      int valor = utrasom.Ranging(CM);

      lcd.clear();
      lcd.setCursor(0,0);
      if(valor > 30)
        lcd.print("OK!");
      if(valor <= 30 && valor > 10 )
        lcd.print("CUIDADO!");
      if(valor <= 10 )
        lcd.print("PARE!");

      lcd.setCursor(0,1);
      lcd.print("Valor: ");
      lcd.print(valor);
      lcd.print(" cm");
      
      delay(250);
      exit = digitalRead(BTN2);
      if(exit == 1){
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Saiu do 2");
        delay(500); 
      }
    }
    digitalWrite(LED2, LOW);
    
  }

  /*
  
   */
   
   delay(250);
}
