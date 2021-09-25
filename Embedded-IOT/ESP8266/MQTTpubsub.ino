#include<ESP8266WiFi.h>
#include<PubSubClient.h>
#include<ArduinoJson.h>
#include<NTPClient.h>
#include<DHT.h>
#include<DHT_U.h>

#define DHTPIN 2
#define DHTTYPE DHT11

DHT dht(DHTPIN,DHTTYPE);

const char* ssid=""; //your wifi name
const char* pass=""; //your wifi password
const char* mqttServer=""; //ip address of raspberry pi
const int mqttPort= 1883;
const char* mqttUser=""; //username of mosquitto setup on raspberry pi
const char* mqttPass=""; //password of mosquitto setup on raspberry pi
const char* ntpServer = ""; //ip address of raspberry pi on which NTP server is setup
const long  gmtOffset_sec = 19800;   //Replace with your GMT offset (seconds)
const int   daylightOffset_sec = 0; //Replace with your daylight offset (seconds)


WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
 Serial.begin(9600);
 WiFi.begin(ssid,pass);
 Serial.println("Welcome");
 while(WiFi.status()!= WL_CONNECTED){
  delay(500);
  Serial.println("Connecting to WiFi...");
 }
 Serial.println("Connected");

 client.setServer(mqttServer,mqttPort);
 client.setCallback(callback);

 while(!client.connected()){
  Serial.println("Connecting to MQTT server..");
  String clientId = "ESP8266Client";
  if(client.connect(clientId.c_str(),mqttUser, mqttPass)){
    client.subscribe("Fan");
    Serial.println("Connected");
  }
  else{
    Serial.print("Failed");
    Serial.println(client.state());
    delay(1000);
  }
 }
 configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}

void callback(char* topic, byte* payload, unsigned int length){
  Serial.print("Message arrived in topic ");
  Serial.println(topic);

  Serial.print("Message:");
  String message="";
  for(int i=0; i<length;i++){
    Serial.println((char)payload[i]);
    message+=(char)payload[i];
  }
  if (String(topic)="Fan"){
    if(message="Fan on"){
      Serial.println("Fan is switched on");
    }
    else{
      Serial.println("Fan is turned off");
    }
  }
  Serial.println();
  Serial.println("**************************");
}

void loop() {
  client.loop();
  float hum= dht.readHumidity();
  //Serial.println(hum);
  float temp= dht.readTemperature();
  //Serial.println(temp);
  int sensorValue = analogRead(A0);
  time_t rawtime;
  struct tm* timeinfo;
  char buffer1[80];
  // Used to store the time
  // returned by localetime() function
  //char buffer[80];
  delay(400);
  time(&rawtime);
  timeinfo = localtime(&rawtime);
  strftime(buffer1, 80,
           "%A,%D,%I:%M:%S%p",
           timeinfo);
  // strftime() function stores the
    // current time as Hours : Minutes
    //%I %M and %p-> format specifier
    // of Hours minutes and am/pm respectively
    // %A is for day and %D is for date*/
    
  StaticJsonDocument<200> doc;
  doc["Temp_cel"] = temp;
  doc["Humidity_percent"] = hum;
  doc["CO2"]=sensorValue;
  doc["SensorID"]="sensor01";
  doc["SensorTime"]=buffer1;
  

  serializeJson(doc, Serial);
  char buffer[256];
  serializeJson(doc, buffer);
  client.publish("Temp", buffer);
  Serial.println();
  Serial.println("Data sent");
  delay(2000);
}
