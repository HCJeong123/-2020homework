//192.168.4.1
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char WiFiAPPSK[] = "00000000"; //연결 시에 패스워드로 입력

const int ledPin = D4;            //기본 내장 LED 사용
bool LEDstatus = LOW;



ESP8266WebServer server(80);       //80로 서버 사용
void setup() 
{
  initHardware(); //Output 등 하드웨어 세팅의 변수를 셋업
  setupWiFi();

  server.on("/", handle_OnConnect);
  server.on("/ledon", handle_ledon); //192.168.4.1/on 
  server.on("/ledoff", handle_ledoff); //192.168.4.1/off
  server.onNotFound(handle_NotFound);
    
  server.begin();  
}

void loop() 
{

  server.handleClient();
  if(LEDstatus){
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
  }
}  

void handle_OnConnect() {
  LEDstatus = LOW;
  Serial.println("LED Status: OFF");
  server.send(200, "text/html", SendHTML(LEDstatus)); 
}

void handle_ledon() {                      //192.168.4.1/on 에 대한 정의
  LEDstatus = HIGH;
  Serial.println("LED Status: ON");
  server.send(200, "text/html", SendHTML(true)); 
}

void handle_ledoff() {                      //192.168.4.1/off에 대한 정의
  LEDstatus = LOW;
  Serial.println("LED Status: OFF");
  server.send(200, "text/html", SendHTML(false)); 
}

void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}

String SendHTML(uint8_t ledstat){                     //홈페이지 디자인에 대한 설정
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>LED Control</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr +=".button {display: block;width: 80px;background-color: #1abc9c;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr +=".button-on {background-color: #1abc9c;}\n";
  ptr +=".button-on:active {background-color: #16a085;}\n";
  ptr +=".button-off {background-color: #34495e;}\n";
  ptr +=".button-off:active {background-color: #2c3e50;}\n";
  ptr +="p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<h1>41615081 Jeong Hae Chang</h1>\n"; //상단 제목
  ptr +="<h3>2020-11-06 LED CONTROL WITH WIFI AP MODE</h3>\n"; //부제목
  
   if(ledstat)
  {ptr +="<p>LED Status: ON</p><a class=\"button button-off\" href=\"/ledoff\">OFF</a>\n";}
  else
  {ptr +="<p>LED Status: OFF</p><a class=\"button button-on\" href=\"/ledon\">ON</a>\n";}

  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}

void setupWiFi()         //SoftAP 연결에 대한 설정
{
  WiFi.mode(WIFI_AP);

  uint8_t mac[WL_MAC_ADDR_LENGTH];
  WiFi.softAPmacAddress(mac);
  String macID = String(mac[WL_MAC_ADDR_LENGTH - 2], HEX) +
                 String(mac[WL_MAC_ADDR_LENGTH - 1], HEX);
  macID.toUpperCase();
  String AP_NameString = "ESP8266 Thing " + macID;

  char AP_NameChar[AP_NameString.length() + 1];
  memset(AP_NameChar, 0, AP_NameString.length() + 1);

  for (int i=0; i<AP_NameString.length(); i++)
    AP_NameChar[i] = AP_NameString.charAt(i);

  WiFi.softAP(AP_NameChar, WiFiAPPSK);  
}

void initHardware()          //연결된 하드웨어에 대한 정의
{
  Serial.begin(115200);      //시리얼 통신 속도
  pinMode(ledPin, OUTPUT);   //D4 Output 설정
  digitalWrite(ledPin, LOW); 
}
