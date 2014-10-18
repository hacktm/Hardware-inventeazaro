void setup() {                
    Serial.begin(9600);
}
void loop() {
    int valoareAlcool = analogRead(A2);
    Serial.print("Nivel alcool: ");
    Serial.println(valoareAlcool, DEC); 
    delay(100);
}
