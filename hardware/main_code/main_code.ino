#include <SPI.h>
#include <Ethernet.h>
#include <dht11.h>
#include "Wire.h"
#include "LiquidCrystal.h"

//DHT11 - temperature & humidity ambient
dht11 DHT11;
#define DHT11PIN 8
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
float last_bpm;
float sugar_level = 80;
float temp_body = 30;
float panic_btn = 0;

void setup(){
  Serial.begin(9600);
  //Ethernet
  Ethernet.begin(mac);
  //LCD
  lcd.begin(16, 2);
  lcd.setBacklight(HIGH);
  lcd.print("  HomeCare HUB");
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
    apiClient.stop();
  }

  // if you're not connected, and ten seconds have passed since
  // your last connection, then connect again and send data:
  if(!apiClient.connected() && (millis() - lastConnectionTime > postingInterval)) {
    lcd.setCursor(0, 1);
    lcd.print("Up devicehub.net");
    sendData();
    lcd.setCursor(0, 1);
    lcd.print("Up done        ");
  }
  // store the state of the connection for next time through
  // the loop:
  lastConnected = apiClient.connected();
  
  
  sendDataToProcessing('S', Signal);     // send Processing the raw Pulse Sensor data
  if (QS == true){                       // Quantified Self flag is true when arduino finds a heartbeat
   fadeRate = 255;                  // Set 'fadeRate' Variable to 255 to fade LED with pulse
   sendDataToProcessing('B',BPM);   // send heart rate with a 'B' prefix
    lcd.setCursor(0, 1);
   last_bpm = BPM;
    lcd.print("Pulse (BPM): ");
    lcd.print(BPM);
    lcd.print(" ");
   //sendDataToProcessing('Q',IBI);   // send time between beats with a 'Q' prefix
   QS = false;                      // reset the Quantified Self flag for next time    
  }
  ledFadeToBeat();
  
}

// this method makes a HTTP connection to the devicehub server:
void sendData() {
   int chk = DHT11.read(DHT11PIN);
   float sensor1 = (int)DHT11.temperature;
   float sensor2 = (int)DHT11.humidity;
   float sensor3 = last_bpm;
   float sensor4 = analogRead(A2); //alcool
   float sensor5 = sugar_level;
   float sensor6 = temp_body;
   float sensor7 = panic_btn;
   
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
