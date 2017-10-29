#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"

//=========================================
//PinNumber
//=========================================
#define DHTPin D4
#define DHTType DHT22
#define soilSens A0
#define R_LED D3
#define G_LED D2
#define relay D1
#define port 1883
#define SN "11112"
//-----------------------------------------

//=========================================
//Public Variables
//=========================================
IPAddress WingServer(5,160,71,117);

WiFiClient nodeMcu;
PubSubClient client(nodeMcu);
DHT dht(DHTPin,DHTType);
char msg[50];
const char* ssid = "hrf";
const char* pass = "12345678";
const char* mqttServer = "192.168.43.117";
//-----------------------------------------

//=========================================
//setup
//=========================================
void setup() {
  // put your setup code here, to run once:
  pinMode(R_LED,OUTPUT);
  pinMode(G_LED,OUTPUT);
  pinMode(relay,OUTPUT);
  digitalWrite(R_LED,1);
  digitalWrite(G_LED,1);
  setup_wifi();
  client.setServer(mqttServer,port);
  client.setCallback(callback);
  dht.begin();
  Serial.begin(9600);
}
//-----------------------------------------

//=========================================
//Loop
//=========================================
void loop() {
  // put your main code here, to run repeatedly:
  if(!client.connected()){
    reconnect();
  }
  client.loop();
  
}
//-----------------------------------------

//=========================================
//WIFI Setting
//=========================================
void setup_wifi(){
  delay(20);
  Serial.println();
  Serial.print("Connect to ");
  Serial.println("ssid");
  WiFi.begin(ssid,pass);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Succesfully CoNNected!!!");
  Serial.print("IP IPAddress :");
  Serial.println(WiFi.localIP());
}
//-----------------------------------------

//=========================================
//callback
//=========================================
void callback(char* topic,byte* payload, unsigned int length){
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  String ServerMesssage="";
  for(int i = 0 ; i< length;i++){
    ServerMesssage += (char)payload[i];
    Serial.print((char)payload[i]);
  }
  Serial.println();
  if(ServerMesssage == "hi"){
    Serial.println("Meesage Recive");
    client.publish("recive","thanks");
    lightControl('r');
  }
  else{
    Serial.println(ServerMesssage);
    lightControl('g');
  }
}
//-----------------------------------------

//=========================================
//Reconnect 
//=========================================
void reconnect(){
  while(!client.connected()){
    Serial.println("MQTT connection ...");
    if(client.connect("Abyary")){
      Serial.println("connected To MQTT Server!");
      client.publish("all","i'm login");
      client.subscribe(SN);
    }
    else{
      Serial.print("failed !");
      Serial.println(client.state());
      delay(5000);
    }
  }
}
//-----------------------------------------

//=========================================
//Control LED Color
//=========================================
void lightControl(char light){
  if(light == 'g'){
    digitalWrite(R_LED,1);
    digitalWrite(G_LED,0);
  }
  else if(light == 'r'){
    digitalWrite(R_LED,0);
    digitalWrite(G_LED,1);
  }
};
//------------------------------------------