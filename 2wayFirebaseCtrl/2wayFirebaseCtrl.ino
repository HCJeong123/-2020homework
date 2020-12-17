#include <FirebaseESP8266.h>
#include  <ESP8266WiFi.h>
#define WIFI_SSID "olleh_WiFi_E900"//"<Your Wifi SSID>"
#define WIFI_PASS "0000007046"//"<Your WiFi Password>"

#define FIREBASE_HOST "waybuttonctrl-default-rtdb.firebaseio.com" //Firebase Project URL:Remove "https:", "/"
#define FIREBASE_AUTH "AymgDLtiqgkRTzpzsJjo8zKCpLd53tTIvROy9bKQ"      //Firebase Auth Token

FirebaseData fbData;

//If using Relay Module
int LED = D4; //initialize 2 Pin
int SW1 = D2;
String state = "0";
String swState = "0";
bool X = LOW;

void setup() {
  
 Serial.begin(9600);
 
   WiFi.begin (WIFI_SSID, WIFI_PASS);
   while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println ("");
  Serial.println ("WiFi Connected!");
  
  Firebase.begin(FIREBASE_HOST,FIREBASE_AUTH);
    pinMode(LED,OUTPUT);// 
    digitalWrite(LED,HIGH); 
    pinMode(SW1,INPUT_PULLUP);// 

}

void loop() {

  if (Firebase.getString(fbData,"/Voice/LED")) {
    state = fbData.stringData();
    if (state=="1"){
      digitalWrite(LED,LOW); //Device1 is ON
    } else if (state=="0"){
      digitalWrite(LED,HIGH);//Device1 if OFF
    }
  }

  if(digitalRead(SW1) == HIGH){
    while(digitalRead(SW1) != LOW){
      delay(5);
    }
    swState = (state=="1")? "0" : "1";
    fbWrite(swState);
  }
}

void fbWrite(String s){
  if (Firebase.setString(fbData,"/Voice/LED", s))
  {
    Serial.println("PASSED");
    Serial.println("PATH: " + fbData.dataPath());
    Serial.println("TYPE: " + fbData.dataType());
    Serial.println("ETag: " + fbData.ETag());
    Serial.println("------------------------------------");
    Serial.println();
  } else {
    Serial.println("FAILED");
    Serial.println("REASON: " + fbData.errorReason());
    Serial.println("------------------------------------");
    Serial.println();
  } 
}
