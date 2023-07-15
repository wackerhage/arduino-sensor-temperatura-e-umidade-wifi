#include "DHT.h"
#include <LiquidCrystal_I2C.h>

#ifdef __cplusplus
extern "C" {
#endif
uint8_t temprature_sens_read();
#ifdef __cplusplus
}
#endif
uint8_t temprature_sens_read();

#include <WiFi.h>
String apiKey = "B8DC21I404URWWKL";                 
const char *ssid =  "Wifi";   
const char *pass =  "12345678";
const char* server = "api.thingspeak.com";
WiFiClient client;

#define DHTPIN 2     
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

DHT dht(DHTPIN, DHTTYPE);

LiquidCrystal_I2C display(0x27, 16, 2);

int led = 3;
int led2 = 4;

void setup() {
  Serial.begin(115200);
  Serial.println(F("Teste Medição"));

  pinMode(led, OUTPUT);
  pinMode(led2, OUTPUT);

  dht.begin();
  display.init();

  Serial.begin(115200);
  delay(10);
  Serial.println("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
}

void loop() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (client.connect(server,80))                                 //   "184.106.153.149" ou api.thingspeak.com
    {  
                            
    String postStr = apiKey;
    postStr +="&field1=";
    postStr += String(h);
    postStr += "&field2=";
    postStr += String(t);
    postStr += "\r\n\r\n";

    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);
 
    Serial.print("Umidade: ");
    Serial.println(humidity);
    Serial.print("Temperatura:");
    Serial.print(temperature);
    Serial.println(" C");
                             
    Serial.println("%. Send to Thingspeak.");
  }
  // Erro
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println(F("Falha no leitor"));
    return;
  }

  //temperatura acima de 35 vai acionar o led vermelho

  if (temperature >= 35.00) {
    digitalWrite(led, HIGH);
  }

  //umidade acima de 70 vai acionar o led azul
  
  if (humidity >= 70.00) {
    digitalWrite(led2, HIGH);
  }
  
  Serial.print(F("Umidade: "));
  Serial.print(humidity);
  Serial.print(F("%  Temp: "));
  Serial.print(temperature);
  Serial.println(F("C"));

// display

  display.setCursor(0,0);
  display.println(F("Umidade: "));
  display.println(humidity);

  display.setCursor(0,1);
  display.println(F("Clima: "));
  display.println(temperature);


  client.stop();
  delay(5000);
  
}