#include <ESP8266WiFi.h>
 
const char* ssid = "KT_WLAN_A4BE";  //와이파이에 대한 설정
const char* password = "000000cbe5";

int Step = 0; //D3
int Dir  = 2; //D4

WiFiServer server(80);
 
void setup() {
  Serial.begin(115200);
  delay(10);
 pinMode(Step, OUTPUT); 
 pinMode(Dir,  OUTPUT);  
 digitalWrite(Dir, LOW);  
 
  
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
 
  WiFi.begin(ssid, password);  // 와이파이 네트워크에 연결
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
 
  // 서버를 시작함
  server.begin();
  Serial.println("Server started");
 
  // 시리얼 모니터에 IP주소를 표시함
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");    //IP주소
  Serial.print(WiFi.localIP());
  Serial.println("/");
 
}
 
void loop() {
  
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
 
  
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
 
  
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();
 
  
  int i=0;
  int value = LOW;
  
  if (request.indexOf("/Command=feed") != -1)  { 
    digitalWrite(Dir, HIGH);//시계 방향으로 모터를 회전
          for( i=1;i<=200;i++){ //200스텝, 한바퀴 회전
          digitalWrite(Step, HIGH);
          delay(10);
          digitalWrite(Step, LOW);
          delay(10);}
    value = HIGH;
  }

  // 리턴
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); 
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<h1 align=center>Stepper motor controlled over WiFi</h1><br><br>");
  client.print("Stepper motor moving= ");
 
  if(value == HIGH) {
    client.print("Forward");
  }
  client.println("<br><br>");
  client.println("<a href=\"/Command=feed\"\"><button>FEED </button></a>"); 
  client.println("</html>");
  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");
 //홈페이지 버튼 디자인
}
