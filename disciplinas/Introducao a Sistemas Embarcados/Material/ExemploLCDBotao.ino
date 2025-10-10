#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define col 16
#define lines 2
#define address 0x27
#define POT A0
#define BTN1 2
#define BTN2 3

LiquidCrystal_I2C lcd(address, col, lines);

void setup(){
  pinMode(POT, INPUT);
  pinMode(BTN1, INPUT);
  pinMode(BTN2, INPUT);

  lcd.init();
  lcd.backlight();
  lcd.clear();

  lcd.setCursor(0,0);
  lcd.print("Hello");
  delay(1000);
}
 
void loop(){
  int click1 = digitalRead(BTN1);
  int click2 = digitalRead(BTN2);
  int exit = 0;

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Escolha uma");
  lcd.setCursor(0,1);
  lcd.print("opcao!");

  if(click1){
    for(int i = 10; i>0; i--){
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Contagem:");
        lcd.setCursor(0,1);
        lcd.print(i);
        delay(600);  
    }

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Hoje e 04/07"); 
    delay(1000);
  }

  if(click2){
    while(exit == 0){
      int valor = analogRead(POT);

      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Potenciometro");
      lcd.setCursor(0,1);
      lcd.print("Valor: ");
      lcd.print(valor);
      
      delay(250);
      exit = digitalRead(BTN2);
      if(exit == 1){
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Saiu do 2"); 
      }
    }
    
  }

  /*
  
   */
   
   delay(250);
}
