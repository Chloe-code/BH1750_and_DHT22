#include <Wire.h>
#include <BH1750.h>
#include <ESP8266WiFi.h>
#include <DHT.h>
#include <ESP8266HTTPClient.h>

#ifndef STASSID
#define STASSID "********"   //Wifi SSID
#define STAPSK  "********"   //Wifi Password
#endif

#define DHTPIN 0
DHT dht(DHTPIN, DHT22);
BH1750 lightMeter;
 
void setup(){
  Serial.begin(9600);
  WiFi.begin(STASSID, STAPSK); 
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Wire.begin();
  lightMeter.begin();
  dht.begin();
}
 
 
void loop() {
  if (WiFi.status() == WL_CONNECTED)
  {
    uint16_t lux = lightMeter.readLightLevel();
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    Serial.print("Temperature: ");
    Serial.print(t);
    Serial.print(" degrees Celcius, Humidity: ");
    Serial.print(h);
    Serial.print("Light: ");
    Serial.print(lux);
    Serial.println(" lx");          
    HTTPClient http;
    http.begin("http://***.***.**.**/WebService/WebService.asmx/insert_CDs2");  //Webservice URL
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");                
    String a1=String("CDs=");          
    int httpCode = http.POST(a1+lux);   
    String payload = http.getString();   
    Serial.println(httpCode);            
    Serial.println(payload);             
    http.end();
    delay(60000);
  }    
}
