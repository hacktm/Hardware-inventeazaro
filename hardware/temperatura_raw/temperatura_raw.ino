void setup(){
  Serial.begin(9600);
}

void loop(){
  Serial.print("Temperatura: ");
  float temperatura = readTempInCelsius(10,0);
  Serial.print(temperatura);
  Serial.print("  ");
  Serial.write(176);
  Serial.println("C");
  delay(200);
}

float readTempInCelsius(int count, int pin) {
  float temperaturaMediata = 0;
  float sumaTemperatura;
  for (int i =0; i < count; i++) {
    int reading = analogRead(pin);
    float voltage = reading * 5.0;
    voltage /= 1024.0;
    float temperatureCelsius = (voltage - 0.5) * 100 ;
    sumaTemperatura = sumaTemperatura + temperatureCelsius;
  }
  return sumaTemperatura / (float)count;
}

