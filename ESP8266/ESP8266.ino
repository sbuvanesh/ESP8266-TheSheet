#include <ESP8266WiFi.h>

const char* server = "ec2-50-112-211-235.us-west-2.compute.amazonaws.com";
int iLoop = 0;
const int LEDPIN=14;

void BlinkLED(int pinNumber, int nooftimes, int interval);
int ConnectWifi(char* ssid, char* password);
float GetADCVolt(int pinNumber, int noofObs);


void setup() {
}

void loop() {
  BlinkLED(LEDPIN,1,1000);
  
  iLoop ++;
  if (iLoop == 20000){
    iLoop = 0;
  }

  //Serial.println(GetADCVolt(10,3));
  
  //Connect to wifi
  if (!WiFi.status() == WL_CONNECTED){
   if (!ConnectWifi("eBayGuest","#WeAreeBay")){
    if (!ConnectWifi("Peace","NonViolence")){
      //Blink LED
      BlinkLED(LEDPIN,1,2000);
     }
   }
   else{
    BlinkLED(LEDPIN,5,500);
   }
  }
  
  if (WiFi.status() == WL_CONNECTED){
    WiFiClient client;
    
    //Write data to server
    if (client.connect(server, 80)){
      client.print("GET /api.php?method=postData&data=" + String(iLoop) + " HTTP/1.1 \r\n Host: " + server + "\r\n Connection: close\r\n\r\n");
      while(client.available()){
        String line = client.readStringUntil('\r');
        Serial.print(line);
      }
    }
  }
  else{
    Serial.println("Server not reachable");  
  }  
  //Deep sleep command ESP.deepSleep(10000);
}

int ConnectWifi(char* ssid, char* password){
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  return WiFi.status();
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

float GetADCVolt(int pinNumber, int noofObs){
  pinMode(pinNumber,INPUT);
  float reading[noofObs];
  int iObs=0;
  int obsMean = 0;
  float obsSum=0;

  while (iObs < noofObs){
    reading[iObs] = (1.0/1024.0) * float(analogRead(pinNumber));
    Serial.println(reading[iObs]);
    obsSum += reading[iObs];
    iObs++;
  }
  
  //Compute return only the closest readings
  obsMean = obsSum/noofObs;
   
  return obsMean;
}

