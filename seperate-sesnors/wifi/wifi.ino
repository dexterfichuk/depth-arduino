//For Wifi
#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager

/********************************************************************/
// First we include the libraries
#include <OneWire.h> 
#include <DallasTemperature.h>
/********************************************************************/
// Data wire is plugged into pin 2 on the Arduino 

//Used for Air Temp/Humidity
#include <dht.h>
dht DHT;

//Declare Variables 

float offset = 0.0; //Calculated from measuring with chem thing

int ph_pin = A0;        // pH Pin        : A0
#define DHT11_PIN 16    // Humidity Pin  : D2
#define ONE_WIRE_BUS 12 // Water Temp    : D8

OneWire oneWire(ONE_WIRE_BUS); 

DallasTemperature sensors(&oneWire);
 
void setup() {
    // put your setup code here, to run once:
    Serial.begin(115200);
 
    //WiFiManager
    //Local intialization. Once its business is done, there is no need to keep it around
    WiFiManager wifiManager;
    //reset saved settings
    wifiManager.resetSettings();
 
    //set custom ip for portal
    wifiManager.setAPStaticIPConfig(IPAddress(10,0,1,1), IPAddress(10,0,1,1), IPAddress(255,255,255,0));
 
    //fetches ssid and pass from eeprom and tries to connect
    //if it does not connect it starts an access point with the specified name
    //here  "AutoConnectAP"
    //and goes into a blocking loop awaiting configuration
    wifiManager.autoConnect("Depth_5005");
    //or use this for auto generated name ESP + ChipID
    //wifiManager.autoConnect();
 
    //if you get here you have connected to the WiFi
    Serial.println("connected... :)");
}

struct air_result { 
  float airTemp; 
  float humid;
  float waterTemp;
  float pH;
  float turbidity;
  } values;
 
void loop() {
    // put your main code here, to run repeatedly:
}
