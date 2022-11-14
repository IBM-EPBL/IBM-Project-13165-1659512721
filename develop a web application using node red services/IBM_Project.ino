#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"
#define DHTPIN D2
#define DHTTYPE DHT11 
DHT dht (DHTPIN, DHTTYPE);
float t;
int h;
String command;
String data="";
void callback (char* topic, byte* payload, unsigned int payloadLength);
const char* ssid = "      ";
const char* password = "       ";
#define ORG "     "
#define DEVICE_TYPE "       "
#define  DEVICE_ID "     "
#define led1 D0
#define led2 D1
const char publishTopic [] = "iot-2/evt/Data?fmt/json";
char server[] = ORG ".messaging.internetofthings.ibmcloud.com";
char topic[] = "iot-2/cmd/home/fmt/string";
char authMethod[] = "use-token-auth";
char token[] = TOKEN;
char clientId[] ="d:" ORG ":" DEVICE_TYPE ":" DEVICE_ID;
WiFiClient wifiClient;
PubSubClient client(server, 1883, callback, wifiClient);
int publishInterval = 5000:
long lastPublishMillis;
void publishData();
void setup () {
  Serial.begin(115200);
  Serial.println();
  pinMode(led1,OUTPUT);
  pinMode(led2,OUTPUT);
  dth.begin();
  wifiConnect();
  mqttConnect();
  }
void loop() {
  if (millis() - lastPublishMillis > publishInterval)
  {
    publishData();
    lastPublishMillis = millis();
  }
  if (!client.loop()) {
    mqttConnect();
  }
}
void wifiConnect(){
  Serial.print("Connecting to "); Serial.print(ssid);
  WiFi.begin(ssid,password);
  while (WiFi .status() != WL_CONNECTED)  {
    delay(500);
    Serial.print(".");
  }
  Serial.print("Wifi connected, IP address: ");
  Serial.println(WiFi.localIP());
}

void mqttConnect()  {
  if (!client.connect())
  {
    Serial.print("Reconnecting MQTT client to "); Serial.println(server);
    while (!client.connect(clientId,authMethod, token)) {
      Serial.print(".");
      delay(500);
    }
    initManagedDevice();
    Serial.println();
  }
}
void initmanagedDevice()   {
  if (client.subscribe(topic))  {
    Serial.println("subscribe to cmd OK");
  }else  {
    Serial.println("subscribe to cmd FAILED");
  }
}
void callback(char* topic, byte* payload, unsigned int payloadLength)    {
  Serial.print("callback invoked for topic: ");
  Serial.println(topic);
  for (int i = 0; i < payloadLength; i++)   {
    command+= (char)payload[i];
  }
  Serial.print("data: "+ command);
  control_func();
  command= "";
}
void control_func()
{
  if(command== "lightoff")
  {
    digitalWrite(led1,LOW);
    digitalWrite(led2,LOW);
    Serial.println(".........lights are off.....");
    else if(command== "lighton");
    {
      digitalWrite(led1,HIGH);
      digitalWrite(led2,HIGH);
      Serial.println("........lights are on......");
      
    }
    else
    {
      Serial.println("......no commands have beed subscribed.....");
    }
  }
}

void publishData ()
{
  h= dht.readHumidity();
  t= dht.readTemperature();
  if (isnan(h)  || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  String payload = "(\"d\":{ \"temperature\":");
  payload += t;
  payload += ",""\"humidity\":";
  payload += h;
  payload += "}}";
  Serial.print("\n");
  Serial.print("Sending payload: ");
  Serial.println(payload);
  if (client.publish(publishTopic, (char*) payload.c_str()))  {
    Serial.println("Publish OK");
  }  else  {
    Serial.println("Publish FAILED"); 
  }
}
