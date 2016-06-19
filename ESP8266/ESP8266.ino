#include <ESP8266WiFi.h>

const char* ssid     = "";
const char* password = "";
const int LED_PIN = 14; //GPIO08

void setup() {
  // put your setup code here, to run once:
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  pinMode(LED_PIN,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (WiFi.status() == WL_CONNECTED){
    digitalWrite(LED_PIN,HIGH);
    delay(500);
    digitalWrite(LED_PIN,LOW);
  }
  else{
    digitalWrite(LED_PIN,HIGH);
  }
}
