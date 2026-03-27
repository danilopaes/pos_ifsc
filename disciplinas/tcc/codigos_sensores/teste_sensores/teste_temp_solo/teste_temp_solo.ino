#include <OneWire.h>
#include <DallasTemperature.h>


#define ONE_WIRE_BUS 4
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
void setup(void)
{
sensors.begin();
Serial.begin(9600);
}
void loop(void)
{
sensors.requestTemperatures();
Serial.print("Temperature: ");
Serial.print(sensors.getTempCByIndex(0));
Serial.print((char)176);
Serial.print("C | ");

Serial.print((sensors.getTempCByIndex(0) * 9.0) / 5.0 + 32.0);
Serial.print((char)176);
Serial.println("F");

delay(500);
}