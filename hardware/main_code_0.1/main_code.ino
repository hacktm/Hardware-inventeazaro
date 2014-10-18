//#include "Wire.h"
//#include "LiquidCrystal.h"
#include <SPI.h>
#include <Ethernet.h>
#include <dht11.h>
dht11 DHT11;

#define PanicButton 3
#define PowerButton 4
#define PowerLed    5
#define StatusLed   6
#define Buzzer      7
#define DHT11PIN 8



// ETH
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

char apiKey[] = "afb7baff-25c4-42cd-9795-56ccdfff4c35";
char apiserver[] = "devicehub.net";


#define temp_amb    "Temp_amb"
#define humidity_amb    "Humidity_amb"

EthernetClient apiClient;

unsigned long lastConnectionTime = 0;             // last time you connected to the server, in milliseconds
boolean lastConnected = false;
const unsigned long postingInterval = 30L*1000L;  // delay between updates to devicehub.net

// LCD
//LiquidCrystal lcd(0);

//Pir
int inputPin = 2;               // choose the input pin for PIR
int pirState = LOW;             // we start, assuming no motion
int val = 0;                    // variable for reading the pin

//  Pulse VARIABLES
int pulsePin = A0;                 // Pulse Sensor purple wire connected to analog pin 0

volatile int BPM;                   // used to hold the pulse rate
volatile int Signal;                // holds the incoming raw data
volatile int IBI = 600;             // holds the time between beats, must be seeded! 
volatile boolean Pulse = false;     // true when pulse wave is high, false when it's low
volatile boolean QS = false;        // becomes true when Arduoino finds a beat.


void setup(){
  pinMode(inputPin, INPUT);     // declare sensor as input
  
  Ethernet.begin(mac);
  
  Serial.begin(9600);             
  
  Serial.print("arduino is at ");
  Serial.println(Ethernet.localIP());
  
  //lcd.begin(16, 2);
  //lcd.setBacklight(HIGH);
  
  interruptSetup();                 // sets up to read Pulse Sensor signal every 2mS 
}



void loop(){
  //sendDataToProcessing('S', Signal);     // send Processing the raw Pulse Sensor data
  if (QS == true){                       // Quantified Self flag is true when arduino finds a heartbeat
   //     fadeRate = 255;                  // Set 'fadeRate' Variable to 255 to fade LED with pulse
        //sendDataToProcessing('B',BPM);   // send heart rate with a 'B' prefix
   //     sendDataToProcessing('Q',IBI);   // send time between beats with a 'Q' prefix
        QS = false;                      // reset the Quantified Self flag for next time    
     }
    
  //delay(20);  

  //pir(); 
  //alcool(); 
  //tempOm(); 
  ethConnection();
  
}

void sendDataToProcessing(char symbol, int data ){
    Serial.print(symbol);                // symbol prefix tells Processing what type of data is coming
    Serial.println(data);                // the data to send culminating in a carriage return
  }



void pir(){
  val = digitalRead(inputPin);  // read input value
  if (val == HIGH) {            // check if the input is HIGH
    if (pirState == LOW) {
      // we have just turned on
      //Serial.println("Motion detected!");
      // We only want to print on the output change, not state
      pirState = HIGH;
    }
  } else {
    if (pirState == HIGH){
      // we have just turned of
      //Serial.println("Motion ended!");
      // We only want to print on the output change, not state
      pirState = LOW;
    }
  }
}


void alcool(){
  int valoareAlcool = analogRead(2);
  //Serial.print("Nivel alcool: ");
  //Serial.println(valoareAlcool, DEC); 
  delay(100);
}


void tempOm(){
  //Serial.print("Temperatura: ");
  float temperatura = readTempInCelsius(10,0);
  //Serial.print(temperatura);
  //Serial.print("  ");
  //Serial.write(176);
  //Serial.println("C");
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

void ethConnection(){
  
 if (apiClient.available()) {
    char c = apiClient.read();
    Serial.print(c);
  }

  // if there's no net connection, but there was one last time
  // through the loop, then stop the client:
  if (!apiClient.connected() && lastConnected) {
    Serial.println(F("disconnecting."));
    apiClient.stop();
  }

  // if you're not connected, and ten seconds have passed since
  // your last connection, then connect again and send data:
  if(!apiClient.connected() && (millis() - lastConnectionTime > postingInterval)) {
    Serial.println("Uploading it to devicehub.net");
    sendData();
    Serial.print("devicehub.net put returned ");
  }
  // store the state of the connection for next time through
  // the loop:
  lastConnected = apiClient.connected();

}

// this method makes a HTTP connection to the devicehub server:
void sendData() {

  
   int chk = DHT11.read(DHT11PIN);
    
   float sensor1 = (int)DHT11.temperature;
   float sensor2 = (int)DHT11.humidity;

  // if there's a successful connection:
  if (apiClient.connect(apiserver, 80)) {
    // send the HTTP PUT request:
    apiClient.print(F("GET /io/453/?apiKey="));
    apiClient.print(apiKey);
    apiClient.print(F("&" temp_amb "="));
    apiClient.print((int)sensor1, DEC);
    apiClient.print(F("&" humidity_amb "="));
    apiClient.print((int)sensor2, DEC);
    
    
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
