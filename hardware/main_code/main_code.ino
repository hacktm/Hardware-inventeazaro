#include <SPI.h>
#include <Ethernet.h>
#include <DHT.h>
#include "Wire.h"
#include "LiquidCrystal.h"

#define DHTPIN 8

// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11 
//#define DHTTYPE DHT22   // DHT 22  (AM2302)
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

//DHT11 - temperature & humidity ambient


DHT dht(DHTPIN, DHTTYPE);

#define temp_amb    "Temp_amb"
#define humidity_amb    "Humidity_amb"
#define pulse_user    "Pulse"
#define alcool_user    "Air_qual"
#define blood_user  "Blood_sugar"
#define temp_user  "Temp_body"
#define panic_user  "Panic_btn"

//Ethernet
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
char apiKey[] = "afb7baff-25c4-42cd-9795-56ccdfff4c35";
char apiserver[] = "devicehub.net";
EthernetClient apiClient;
unsigned long lastConnectionTime = 0;             // last time you connected to the server, in milliseconds
boolean lastConnected = false;
const unsigned long postingInterval = 30L*1000L;  // delay between updates to devicehub.net

// LCD - Connect via i2c, default address #0 (A0-A2 not jumpered)
LiquidCrystal lcd(0);

//  Pulse
int pulsePin = 0;                 // Pulse Sensor purple wire connected to analog pin 0
int blinkPin = 13;                // pin to blink led at each beat
int fadePin = 5;                  // pin to do fancy classy fading blink at each beat
int fadeRate = 0;                 // used to fade LED on with PWM on fadePin
// these variables are volatile because they are used during the interrupt service routine!
volatile int BPM;                   // used to hold the pulse rate
volatile int Signal;                // holds the incoming raw data
volatile int IBI = 600;             // holds the time between beats, must be seeded! 
volatile boolean Pulse = false;     // true when pulse wave is high, false when it's low
volatile boolean QS = false;        // becomes true when Arduoino finds a beat.
float last_bpm = 70;
float sugar_level = 80;
float temp_body = 33;
float panic_btn = 0;

int panicpPin = 3;
bool panicState = false;
bool personState = false;


int inputPin = 2;               // choose the input pin (for PIR sensor)
int pirState = LOW;             // we start, assuming no motion detected
int val = 0;                    // variable for reading the pin status

const int analogInPin = A1;  // Analog input pin that the potentiometer is attached to


int sensorValue = 0;        // value read from the pot
int outputValue = 0;        // value output to the PWM (analog out)

void setup(){
  //Serial.begin(9600);
  //BUzzer
  pinMode(7, OUTPUT);
  //Ethernet
  Ethernet.begin(mac);
  //LCD
  lcd.begin(16, 2);
  lcd.setBacklight(HIGH);
  lcd.print("  HomeCare HUB  ");
  lcd.setCursor(0, 1);
  lcd.print("IP: ");
  lcd.print(Ethernet.localIP());
  //Pulse
  pinMode(blinkPin,OUTPUT);         // pin that will blink to your heartbeat!
  pinMode(fadePin,OUTPUT);          // pin that will fade to your heartbeat!
  Serial.begin(115200);             // we agree to talk fast!
  interruptSetup();                 // sets up to read Pulse Sensor signal every 2mS 
  // UN-COMMENT THE NEXT LINE IF YOU ARE POWERING The Pulse Sensor AT LOW VOLTAGE, 
  // AND APPLY THAT VOLTAGE TO THE A-REF PIN
  //analogReference(EXTERNAL);
 
  dht.begin();
  pinMode(3,INPUT_PULLUP);
  pinMode(inputPin, INPUT);     // declare sensor as input
  pinMode(A1,INPUT_PULLUP);
}

void loop() {

 if (apiClient.available()) {
    char c = apiClient.read();
    Serial.print(c);
  }

  // if there's no net connection, but there was one last time
  // through the loop, then stop the client:
  if (!apiClient.connected() && lastConnected) {
    lcd.setCursor(0, 1);
    lcd.print("   upload done   ");
    analogWrite(6, 130);
    delay(500);
    analogWrite(6, 0);
    apiClient.stop();
  }

  // if you're not connected, and ten seconds have passed since
  // your last connection, then connect again and send data:
  if(!apiClient.connected() && (millis() - lastConnectionTime > postingInterval)) {
    sendData();
  }
  // store the state of the connection for next time through
  // the loop:
  lastConnected = apiClient.connected();
  
  
  sendDataToProcessing('S', Signal);     // send Processing the raw Pulse Sensor data
  if (QS == true){                       // Quantified Self flag is true when arduino finds a heartbeat
   fadeRate = 255;                  // Set 'fadeRate' Variable to 255 to fade LED with pulse
   sendDataToProcessing('B',BPM);   // send heart rate with a 'B' prefix
    lcd.setCursor(0, 1);
    lcd.print("Pulse (BPM): ");
    if(BPM <=150 && BPM>=50){ 
      last_bpm = BPM;
      lcd.print(BPM);
      lcd.print(" ");
    }else{
      lcd.print("---");
    }
   //sendDataToProcessing('Q',IBI);   // send time between beats with a 'Q' prefix
   QS = false;                      // reset the Quantified Self flag for next time    
  }
  ledFadeToBeat();
  if(!digitalRead(3)){
    //Panic
    analogWrite(6, 250);
    analogWrite(6, 0);
    lcd.setCursor(0, 1);
    lcd.print("Calling Clinic    ");
    delay(1000);
    panic_btn = 1;
  }
  val = digitalRead(inputPin);  // read input value
  if (val == HIGH) {            // check if the input is HIGH
    if (pirState == LOW) {
      // we have just turned on
      lcd.setCursor(0, 1);
      lcd.print("Person detected!");
      delay(200);
      // We only want to print on the output change, not state
      pirState = HIGH;
    }
  } else {
    if (pirState == HIGH){
      // we have just turned of
      //lcd.setCursor(0, 1);
      //lcd.print("Person not here!");
      delay(200);
      // We only want to print on the output change, not state
      pirState = LOW;
    }
  }

  temp_body = 219 - analogRead(analogInPin);
  
}

// this method makes a HTTP connection to the devicehub server:
void sendData() {
   //int chk = DHT11.read(DHT11PIN);
   float sensor1 = dht.readTemperature();
   float sensor2 = dht.readHumidity();
   float sensor3 = last_bpm;
   float sensor4 = analogRead(A2); //alcool
   float sensor5 = sugar_level;
   float sensor6 = temp_body;
   float sensor7 = panic_btn;
   if(panic_btn == 1){
    panic_btn = 0;
   }
   
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius
  float t = dht.readTemperature();  
   
  // Read temperature as Fahrenheit
  float f = dht.readTemperature(true);
  
  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Compute heat index
  // Must send in temp in Fahrenheit!
  float hi = dht.computeHeatIndex(f, h);
   
  //lcd.setCursor(0, 1);
  //lcd.print(sensor1);

  // if there's a successful connection:
  if (apiClient.connect(apiserver, 80)) {
    // send the HTTP PUT request:
    apiClient.print(F("GET /io/453/?apiKey="));
    apiClient.print(apiKey);
    //apiClient.print(F("&" pulse_user "="));
    //apiClient.print((int)sensor3, DEC);
    apiClient.print(F("&" temp_amb "="));
    apiClient.print((int)sensor1, DEC);
    apiClient.print(F("&" humidity_amb "="));
    apiClient.print((int)sensor2, DEC);
    apiClient.print(F("&" pulse_user "="));
    apiClient.print((int)sensor3, DEC);
    apiClient.print(F("&" alcool_user "="));
    apiClient.print((int)sensor4, DEC);
    apiClient.print(F("&" blood_user "="));
    apiClient.print((int)sensor5, DEC);
    apiClient.print(F("&" temp_user "="));
    apiClient.print((int)sensor6, DEC);
    apiClient.print(F("&" panic_user "="));
    apiClient.print((int)sensor7, DEC);
  
  
    apiClient.println(F(" HTTP/1.1"));
    apiClient.println(F("Host: www.devicehub.net"));
    apiClient.print(F("User-Agent: "));
    apiClient.println(F("devicehub"));
    apiClient.println(F("Connection: close"));
    apiClient.println();

  }
  else {
    // if you couldn't make a connection:
    Serial.println(F("connection failed"));
    Serial.println();
    Serial.println(F("disconnecting."));
    apiClient.stop();
  }
  // note the time that the connection was made or attempted:
  lastConnectionTime = millis();
}


//Celsius to Fahrenheit conversion
double Fahrenheit(double celsius)
{
  return 1.8 * celsius + 32;
}

//Celsius to Kelvin conversion
double Kelvin(double celsius)
{
  return celsius + 273.15;
}

// dewPoint function NOAA
// reference: http://wahiduddin.net/calc/density_algorithms.htm
double dewPoint(double celsius, double humidity)
{
  double A0= 373.15/(273.15 + celsius);
  double SUM = -7.90298 * (A0-1);
  SUM += 5.02808 * log10(A0);
  SUM += -1.3816e-7 * (pow(10, (11.344*(1-1/A0)))-1) ;
  SUM += 8.1328e-3 * (pow(10,(-3.49149*(A0-1)))-1) ;
  SUM += log10(1013.246);
  double VP = pow(10, SUM-3) * humidity;
  double T = log(VP/0.61078);   // temp var
  return (241.88 * T) / (17.558-T);
}

// delta max = 0.6544 wrt dewPoint()
// 5x faster than dewPoint()
// reference: http://en.wikipedia.org/wiki/Dew_point
double dewPointFast(double celsius, double humidity)
{
  double a = 17.271;
  double b = 237.7;
  double temp = (a * celsius) / (b + celsius) + log(humidity/100);
  double Td = (b * temp) / (a - temp);
  return Td;
}

void ledFadeToBeat(){
  fadeRate -= 15;                         //  set LED fade value
  fadeRate = constrain(fadeRate,0,255);   //  keep LED fade value from going into negative numbers!
  analogWrite(fadePin,fadeRate);          //  fade LED
}


void sendDataToProcessing(char symbol, int data ){
  Serial.print(symbol);                // symbol prefix tells Processing what type of data is coming
  Serial.println(data);                // the data to send culminating in a carriage return
}
