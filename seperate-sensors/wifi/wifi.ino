#include <ESP8266HTTPClient.h>

#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
 
//needed for library
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager

const char* host = "159.203.4.227/depth";

 
void setup() {
    // put your setup code here, to run once:
    Serial.begin(115200);
 
    //WiFiManager
    //Local intialization. Once its business is done, there is no need to keep it around
    WiFiManager wifiManager;
    //reset saved settings
    //wifiManager.resetSettings();
 
    //set custom ip for portal
    wifiManager.setAPStaticIPConfig(IPAddress(10,0,1,1), IPAddress(10,0,1,1), IPAddress(255,255,255,0));
 
    //fetches ssid and pass from eeprom and tries to connect
    //if it does not connect it starts an access point with the specified name
    //here  "AutoConnectAP"
    //and goes into a blocking loop awaiting configuration
    wifiManager.autoConnect("Depth_Setup");
    //or use this for auto generated name ESP + ChipID
    //wifiManager.autoConnect();
 
    //if you get here you have connected to the WiFi
    Serial.println("connected... :)");

}

int value = 0;
String depthUrl = "";

void loop() {
    if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
 
    HTTPClient http;  //Declare an object of class HTTPClient

    depthUrl = "http://159.203.4.227/depth/add.php?key=Saucy&nodeid=5005&";
    depthUrl += "waterTemp=";
    depthUrl += 23;
    depthUrl += "&airTemp=";
    depthUrl += 26;
    depthUrl += "&airHumidity=";
    depthUrl += 18;
    depthUrl += "&pH=";
    depthUrl += "6.5";
    depthUrl += "&turbidity=";
    depthUrl += 0;

    
    http.begin(depthUrl);  //Specify request destination
    int httpCode = http.GET();                                                                  //Send the request
 
    if (httpCode > 0) { //Check the returning code
 
      String payload = http.getString();   //Get the request response payload
      Serial.println(payload);                     //Print the response payload
 
    }
 
    http.end();   //Close connection
 
  }
  Serial.println("NextRequest!");
  delay(30000);    //Send a request every 30 seconds
}
