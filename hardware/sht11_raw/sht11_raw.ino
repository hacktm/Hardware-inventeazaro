
#include "SHT1x.h"

#define dataPin  10
#define clockPin 11
SHT1x sht1x(dataPin, clockPin);

float temp_c;
float temp_f;
float humidity;
int indice;

void setup()
{
  Serial.begin(9600); 
  
  temp_c = sht1x.readTemperatureC();
  temp_f = sht1x.readTemperatureF();
  humidity = sht1x.readHumidity();
}

void loop()
{
  temp_c = sht1x.readTemperatureC();
  temp_f = sht1x.readTemperatureF();
  humidity = sht1x.readHumidity();
  indice = (temp_c * 1.8 + 32) - (0.55 - 0.0055 * humidity) * ((temp_c * 1.8 + 32) - 58);

  Serial.print("Temperatura: ");
  Serial.print(temp_c, 2);
  Serial.print("C / ");
  Serial.print(temp_f, 2);
  Serial.print("F. Umiditate: ");
  Serial.print(humidity);
  Serial.print(" %");
  Serial.print(" Indice de confort termic: ");
  Serial.println(indice);
  delay(2000);
}
