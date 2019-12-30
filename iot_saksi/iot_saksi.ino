#include <Firebase.h>
#include <FirebaseArduino.h>
#include <ArduinoJson.h>
#include <ThingSpeak.h>
#include <ESP8266WiFi.h>

#define FIREBASE_HOST "projeiot-5aef8.firebaseio.com"
#define FIREBASE_AUTH "HxftFmztH024XZYIbWJWjuIuWRMTFg61omaETiwB"
       
unsigned long channelId = 942134; 
const char * WriteApiKey = "U5HJKO9PK3XWIL91"; 
const char* server = "api.thingspeak.com";

const char* ssid = "AliG";
const char* pass = "ali435123";

const int sensorPin = A0;
const int ledPin = D1;
const int motorPin = D2;

String pompaDurum = "0";

WiFiClient client;

unsigned long dataZamani = 10000;
unsigned long olcumZamani = 1000;
unsigned long oncekiMilisaniye = 0;
unsigned long oncekiMilisaniye1 = 0;
int nemYuzdesi;
int sensorDegeri;

void setup()
{
  pinMode(sensorPin,INPUT);
  pinMode(motorPin,OUTPUT);
  pinMode(ledPin,OUTPUT);
  
  digitalWrite(motorPin,LOW);
  digitalWrite(ledPin,LOW);
  
  Serial.begin(115200);
  delay(10);
  WiFi.begin(ssid,pass);
  
  while(WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi Baglandi.");
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  ThingSpeak.begin(client);
}

void loop()
{

  pompaDurum = Firebase.getString("pompaDurum");
  unsigned long suankiMilisaniye = millis();

   sensorDegeri= analogRead(sensorPin);
   nemYuzdesi = map(sensorDegeri,1023,200,0,100);

   if(nemYuzdesi < 70){
    sulama();
  }

 if((unsigned long)(suankiMilisaniye - oncekiMilisaniye)>= dataZamani)
 {
  ThingSpeak.writeField(channelId, 2,nemYuzdesi, WriteApiKey);
  oncekiMilisaniye = millis();
  client.stop();
 }

 if(pompaDurum == "1"){
  digitalWrite(motorPin,HIGH);
  digitalWrite(ledPin,HIGH);
 }
 else{
  digitalWrite(motorPin,LOW);
  digitalWrite(ledPin,LOW);
 }
}

void sulama(){
    digitalWrite(motorPin,HIGH);
    digitalWrite(ledPin,HIGH);
    delay(5000);
    digitalWrite(motorPin,LOW);
    digitalWrite(ledPin,LOW);
}
