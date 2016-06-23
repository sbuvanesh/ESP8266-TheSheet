#include <ESP8266WiFi.h>

const char* host = "ec2-50-112-211-235.us-west-2.compute.amazonaws.com";
int iLoop = 0;
const int GREENLED=5;
const int ANALOG_PIN = A0;

void BlinkLED(int pinNumber, int nooftimes, int interval);
float GetADCVolt(int pinNumber, int noofObs);

void setup() {
  pinMode(GREENLED,OUTPUT);
  pinMode(ANALOG_PIN,INPUT);
  Serial.begin(115200);
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  WiFi.begin("","");
  delay(1000);
  if (WiFi.status() == WL_CONNECTED){
    digitalWrite(GREENLED, HIGH);
  }
  else{
    WiFi.disconnect();
    WiFi.mode(WIFI_STA);
    WiFi.begin("","");
    delay(1000);
    if (WiFi.status() == WL_CONNECTED){
      digitalWrite(GREENLED, HIGH);
    }  
  }
  delay(500);
}

void loop() {

  if (WiFi.status()==WL_CONNECTED){
    digitalWrite(GREENLED, HIGH);
    WiFiClient client;
    delay(500);
    
    //Write data to server
    if (!client.connect(host, 80)){
      //Serial.println("Connection failed");
      return;
    }
    
    client.print(String("GET /api.php?method=postData&data=" + String(analogRead(ANALOG_PIN) + 10) + "HTTP/1.1 \r\nHost: ") + host + "\r\nConnection: close \r\n\r\n");
    delay(500);

    unsigned long timeout = millis();
    while (client.available()==0){
     if (millis() - timeout > 5000) {
      //Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
   }
   while (client.available()){
    //String line = client.readStringUntil('\r');
    //Serial.println(line);
    BlinkLED(GREENLED,20,400);
    digitalWrite(GREENLED, HIGH);
   }
  }
  //Serial.println(GetADCVolt(10,3));
  delay(1000);
}

void BlinkLED(int pinNumber, int nooftimes, int interval=500){
  int iLoop=0;
  //Set pinmode to output
  pinMode(pinNumber,OUTPUT);
  
  while (iLoop < nooftimes){
    digitalWrite(pinNumber,HIGH);
    delay(interval);
    digitalWrite(pinNumber,LOW);
    delay(interval);
    iLoop++;
  }
}

