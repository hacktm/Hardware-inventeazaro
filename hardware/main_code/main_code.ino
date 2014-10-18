#include <SPI.h>
#include <Ethernet.h>
#include <dht11.h>
#include "Wire.h"
#include "LiquidCrystal.h"




dht11 DHT11;
#define DHT11PIN 8


byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

char apiKey[] = "afb7baff-25c4-42cd-9795-56ccdfff4c35";
char apiserver[] = "devicehub.net";


#define temp_amb    "Temp_amb"
#define humidity_amb    "Humidity_amb"

EthernetClient apiClient;

unsigned long lastConnectionTime = 0;             // last time you connected to the server, in milliseconds
boolean lastConnected = false;
const unsigned long postingInterval = 30L*1000L;  // delay between updates to devicehub.net

// Connect via i2c, default address #0 (A0-A2 not jumpered)
LiquidCrystal lcd(0);

void setup(){
  lcd.begin(16, 2);

  lcd.setBacklight(HIGH);
  lcd.print("Home Care Hub");
  
 Serial.begin(9600);
 Ethernet.begin(mac);
 
  lcd.setCursor(0, 1);
  lcd.print("IP: ");
  lcd.print(Ethernet.localIP());
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
    lcd.print("upload done    ");
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

}

// this method makes a HTTP connection to the devicehub server:
void sendData() {

  
   int chk = DHT11.read(DHT11PIN);
    
   float sensor1 = (int)DHT11.temperature;
   float sensor2 = (int)DHT11.humidity;
   
  //lcd.setCursor(0, 1);
  //lcd.print(sensor1);

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
