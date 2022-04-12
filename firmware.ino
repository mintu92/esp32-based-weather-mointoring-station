
  #include <WiFi.h>
  #include <ESPAsyncWebServer.h>
  #include <SPIFFS.h>
  #include "DHT.h"
  #include <Wire.h>
  #include <Adafruit_BMP280.h>

  #define DHTPIN 2
  #define DHTTYPE DHT11
  Adafruit_BMP280 bmp;

const int windpin = 34;
const int pressurepin = 39;
const int led = 5;

//const int temppin = 36;
//const int humiditypin = 35;

const char* ssid = "ARKENSTONE";
const char* password = "Mintu1992";
AsyncWebServer server(80); 
DHT dht(DHTPIN, DHTTYPE);


String readW() 
{
 int a = analogRead(windpin);
 float v = (a* 0.000805);
 Serial.println(v);
 return String(v);
}
String readP() 
{
 float u = bmp.readPressure();
// int b = analogRead(pressurepin);
 //float u = (b* 0.000805);
 Serial.println(u);
 return String(u);
}
String readT() 
{
 float t = bmp.readTemperature();
 delay(100);
 //int c = analogRead(temppin);
 //float d = (c* 0.000805);
 Serial.println(t);
 return String(t); 
}
String readH() 
{
 float h = dht.readHumidity();
 delay(100);
 //int e = analogRead(humiditypin);
 //float f = (e* 0.000805);
 Serial.println(h);
 return String(h);
}
String readA() 
{
 float x = bmp.readAltitude(1013.25);
 delay(100);
 Serial.println(x);
 return String(x); 
}

void setup()
{                                      // Serial port for debugging purposes
  Serial.begin(115200);
  dht.begin();
  
   bmp.begin(0x76);
   bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("connected");
  Serial.println(WiFi.localIP());

  // Initialize SPIFFS
  if(!SPIFFS.begin()){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(SPIFFS, "/index.html");
  });
  server.on("/windspeed", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send_P(200, "text/plain", readW().c_str());
  });
    server.on("/presure", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send_P(200, "text/plain", readP().c_str());
  });
    server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send_P(200, "text/plain", readT().c_str());
  });
    server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send_P(200, "text/plain", readH().c_str());
  });
  server.on("/logo", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/logo.jpg", "image/jpg");
  });
 
  server.begin();
  pinMode(led,OUTPUT);
}
 
void loop()
{ digitalWrite(led,HIGH);
  delay(1000);
  digitalWrite(led,LOW);
  delay(1000);
}
