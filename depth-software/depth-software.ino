#include <OneWire.h>

//Used for Air Temp/Humidity
#include <dht.h>
// OneWire DS18S20, DS18B20, DS1822 Temperature Example
//
// http://www.pjrc.com/teensy/td_libs_OneWire.html
//
// The DallasTemperature library can do all this work for you!
// http://milesburton.com/Dallas_Temperature_Control_Library

OneWire  ds(8);  // on pin 10 (a 4.7K resistor is necessary)
int ph_pin = A2; //This is the pin number connected to Po for PH 
float offset = 0.0; //Calculated from measuring with chem thing
dht DHT;

#define DHT11_PIN 2 //Air sensor pin

void setup(void) {
  Serial.begin(9600);
}

struct air_result { 
  float airTemp; 
  float humid;
  float waterTemp;
  float pH;
  } values;

void pH(){
  int measure;
  int n = 15;
  float voltage; //classic digital to voltage conversion

  float Po = 0;
  for (int i = 0; i < n; i++){ //Loop to make sum
    measure = analogRead(ph_pin);
    voltage = (5 / 1024.0 * measure) - 0.037;
    Po += (7 + ((2.5 - voltage) / 0.18)) + offset;
//    Serial.println(voltage, 5);
    delay(100);
  }
  // PH_step = (voltage@PH7 - voltage@PH4) / (PH7 - PH4)
  // PH_probe = PH7 - ((voltage@PH7 - voltage@probe) / PH_step)
  Po /= n;
  
  values.pH = Po;
}
int air(){
  // READ DATA
//  Serial.print("DHT11, \t");
  DHT.read11(DHT11_PIN);
  
  // DISPLAY DATA
  float temp = DHT.temperature;
  float humid = DHT.humidity;
//  Serial.print(humid);
//  Serial.print(",\t");
//  Serial.println(temp);
  
//  delay(2000);
  
  values.airTemp = temp;
  values.humid = humid;
  return 1;
}

void waterTemp(){
  byte i;
  byte present = 0;
  byte type_s;
  byte data[12];
  byte addr[8];
  float celsius, fahrenheit;
  
  if ( !ds.search(addr)) {
//    Serial.println("No more addresses.");
    Serial.println();
    ds.reset_search();
    delay(250);
    return;
  }
 
  ds.reset();
  ds.select(addr);
  ds.write(0x44, 1);        // start conversion, with parasite power on at the end
  
  delay(1200);     // maybe 750ms is enough, maybe not
  // we might do a ds.depower() here, but the reset will take care of it.
  
  present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE);         // Read Scratchpad

  //Serial.print("  Data = ");
  //Serial.print(present, HEX);
  //Serial.print(" ");
  for ( i = 0; i < 9; i++) {           // we need 9 bytes
    data[i] = ds.read();
//    Serial.print(data[i], HEX);
//    Serial.print(" ");
  }
//  Serial.print(" CRC=");
//  Serial.print(OneWire::crc8(data, 8), HEX);
  Serial.println();

  // Convert the data to actual temperature
  // because the result is a 16 bit signed integer, it should
  // be stored to an "int16_t" type, which is always 16 bits
  // even when compiled on a 32 bit processor.
  int16_t raw = (data[1] << 8) | data[0];
  if (type_s) {
    raw = raw << 3; // 9 bit resolution default
    if (data[7] == 0x10) {
      // "count remain" gives full 12 bit resolution
      raw = (raw & 0xFFF0) + 12 - data[6];
    }
  } else {
    byte cfg = (data[4] & 0x60);
    // at lower res, the low bits are undefined, so let's zero them
    if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
    else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
    else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
    //// default is 12 bit resolution, 750 ms conversion time
  }
  celsius = (float)raw / 16.0;
  fahrenheit = celsius * 1.8 + 32.0;
//  Serial.print("  Temperature = ");
//  Serial.print(celsius);
//  Serial.print(" Celsius, ");
//  Serial.print(fahrenheit);
//  Serial.println(" Fahrenheit");
  values.waterTemp = celsius;
  return 1;
}

void loop(void) {
  waterTemp();
  
  air();
   
  pH();
   
  Serial.print("Water Temperature = ");
  Serial.print(values.waterTemp);
  Serial.print(" Celsius");
  Serial.println();
  
  Serial.print("Air Temperature = ");
  Serial.print(values.airTemp);
  Serial.print(" Celsius");
  Serial.println();
  
  Serial.print("Air Humidity = ");
  Serial.print(values.humid);
  Serial.println();

  Serial.print("pH = ");
  Serial.print(values.pH);
  Serial.println();

  //Already delayed in the ph calculations
  //delay(2000);

}
