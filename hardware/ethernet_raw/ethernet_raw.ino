#include <SPI.h>
#include <WiFly.h>
#include <SPI.h>
#include <Ethernet.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte ip[] = { 192, 168, 20, 77 };   // static ip of Arduino
byte gateway[] = { 192, 168, 20, 1};  // gateway address
byte subnet[] = { 255, 255, 248, 0 };  //subnet mask
byte dnsip[] = { 8, 8, 8, 8 };  //subnet mask

char apiKey[] = "paste_api_key_here";
char apiserver[] = "www.devicehub.net";

#define sensorName    "paste_sensor_name_here"

EthernetClient apiClient;

unsigned long lastConnectionTime = 0;             // last time you connected to the server, in milliseconds
boolean lastConnected = false;
const unsigned long postingInterval = 30L*1000L;  // delay between updates to devicehub.net


void setup(){
 Serial.begin(9600);
 Ethernet.begin(mac, ip, dnsip, gateway, subnet);
 Serial.print("arduino is at ");
 Serial.println(Ethernet.localIP());
}

void loop() {

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

   float sensor1 = 32;

  // if there's a successful connection:
  if (apiClient.connect(apiserver, 80)) {
    // send the HTTP PUT request:
    apiClient.print(F("GET /io/paste_project_id_here/?apiKey="));
    apiClient.print(apiKey);
    apiClient.print(F("&" sensorName "="));
    apiClient.print((int)sensor1, DEC);
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
   
