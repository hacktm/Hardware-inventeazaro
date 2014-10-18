void setup() {                
    Serial.begin(9600);
}
void loop() {
    int valoareAlcool = analogRead(0);
    Serial.print("Nivel alcool: ");
    Serial.println(valoareAlcool, DEC); 
    delay(100);
}
