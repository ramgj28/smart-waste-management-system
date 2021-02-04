#include <ESP8266WiFi.h>  
#include <Servo.h>
const int IR       = D3;   
const int trigPin1 = D5;   
const int echoPin1 = D6;  
const int trigPin2 = D7;   
const int echoPin2 = D8;  
Servo myservo; 
long duration1,duration2; 
int distance1,distance2;  

WiFiClient client;
WiFiServer server(80);  
const char* MY_SSID = "RGJ2000"; 
const char* MY_PWD = "ramg1245";
const char* TS_SERVER = "api.thingspeak.com"; 
String TS_API_KEY ="1DU031WUVU2DH79X";

void connectWifi() 
{ 
  Serial.print("Connecting to "+ *MY_SSID);  
  WiFi.begin(MY_SSID, MY_PWD);  
  while (WiFi.status() != WL_CONNECTED) 
  {     
    delay(1000);    
    Serial.print("."); 
  }
  Serial.println("");  
  Serial.println("WiFi Connected"); 
  Serial.println("");
  server.begin();   
  Serial.println("Server started"); 
  Serial.print("Use this URL to connect: ");
  Serial.print("http://"); 
  Serial.print(WiFi.localIP());
  Serial.println("/");   
}

void sendDataTS(void)
{   
  int water_value = analogRead(A0);
  Serial.print("Water_Value:  ");   
  Serial.println(water_value); 
  if(digitalRead(IR)==LOW && water_value>=1024)//moisture cutoff to be added
  {
    myservo.write(180);
    Serial.println(" dry ");
    delay(2000);
    myservo.write(90);
    delay(2000);   
  }
  else if(digitalRead(IR)==LOW && water_value<1024)//moisture cutoff to be added
  {
   myservo.write(0);
   Serial.println(" wet ");
   delay(2000);
   myservo.write(90);
   delay(2000);   
  } 
  digitalWrite(trigPin1, LOW);  
  delayMicroseconds(2);  
  digitalWrite(trigPin1, HIGH); 
  delayMicroseconds(10);  
  digitalWrite(trigPin1, LOW);   
  duration1 = pulseIn(echoPin1, HIGH);
  distance1 = duration1*0.034/2;  
  Serial.print("Distance_1: ");    
  Serial.println(distance1);    
  delay(1000); 
  digitalWrite(trigPin2, LOW);  
  delayMicroseconds(2); 
  digitalWrite(trigPin2, HIGH);  
  delayMicroseconds(10);  
  digitalWrite(trigPin2, LOW);   
  duration2 = pulseIn(echoPin2, HIGH);  
  distance2 = duration2*0.034/2; 
  Serial.print("Distance_2: "); 
  Serial.println(distance2);    
  Serial.print("Water_Value:  ");   
  Serial.println(water_value); 

 if (client.connect(TS_SERVER, 80))    
 {
          String postStr = TS_API_KEY;    
          postStr += "&field1="; 
          postStr += String(distance1);    
          postStr += "&field2=";  
          postStr += String(distance2);
          postStr += "\r\n\r\n";
          client.print("POST /update HTTP/1.1\n"); 
          client.print("Host: api.thingspeak.com\n");   
          client.print("Connection: close\n");  
          client.print("X-THINGSPEAKAPIKEY: " + TS_API_KEY + "\n");  
          client.print("Content-Type: application/x-www-form-urlencoded\n");  
          client.print("Content-Length: ");    
          client.print(postStr.length());  
          client.print("\n\n");   
          client.print(postStr);   
          delay(100); 
}
client.stop(); 
}


void setup() 
{
  Serial.begin(9600); 
  myservo.attach(D1);
  myservo.write(90);
  delay(100);   
  connectWifi(); 
  myservo.attach(D1);
  pinMode(trigPin1, OUTPUT); // Sets the trigPin as an Output   
  pinMode(echoPin1, INPUT); // Sets the echoPin as an Input  
  pinMode(trigPin2, OUTPUT); // Sets the trigPin as an Output   
  pinMode(echoPin2, INPUT); // Sets the echoPin as an Input  
  pinMode(IR,INPUT); 
}  
 
void loop()  
{  
 sendDataTS();
 delay(3000);
}
