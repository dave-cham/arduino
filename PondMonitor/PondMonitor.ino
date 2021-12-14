// ESP-01 Project to monitor the pond:
// Inputs from float switches
// Publish status to MQTT
// Output to refill valve



#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>

//USER CONFIGURED SECTION START//
const char* ssid = "Barney & Family";
const char* password = "Rice Krispies";
//const char* mqtt_server = "192.168.1.189";
const char* mqtt_server = "homeassistant";
const int mqtt_port = 1883;
const char *mqtt_user = "mqtt";
const char *mqtt_pass = "Rhu8ar8";
const char *mqtt_client_name = "pond-level-monitor"; 
const char *mqtt_topic = "pond/level/status";
//USER CONFIGURED SECTION END//

enum WaterLevel
{
  Error,
  Ok,
  Low,
  VeryLow
};

WiFiClient espClient;
PubSubClient client(espClient);
int upperFloatSwitch = 0;
int lowerFloatSwitch = 2;
WaterLevel waterLevel = Ok;
int watchdogCounter = 0;


void setup_wifi() 
{
  Serial.print("Connecting WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(100);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi Connected");
}

bool broadcast(char* payload) 
{
  Serial.println("Publishing message on MQTT");
  if(!client.connect(mqtt_client_name, mqtt_user, mqtt_pass))
  {
    int status = client.state();
    Serial.print("MQTT Error: ");
    Serial.println(status);
    return false;
  }
    
  if(!client.publish(mqtt_topic, payload))
  {
    Serial.println("Failed to send MQTT message");
    return false;
  }

  return true;
}

void updateStatus(WaterLevel detectedLevel)
{
  if(waterLevel != detectedLevel)
  {
    char* payload = getPayload(detectedLevel);
    Serial.print("Sending MQTT message: ");
    Serial.println(payload);
    if(broadcast(payload))
    {
      waterLevel = detectedLevel;
    }
  }
}

char* getPayload(WaterLevel level)
{
  switch(level)
  {
    case Ok: return "Ok";
    case Low: return "Low";
    case VeryLow: return "VeryLow";
    default: return "Error";
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println("Startup...");
  pinMode(upperFloatSwitch, INPUT_PULLUP); // GPIO0 - Yellow wire - upper float switch
  pinMode(lowerFloatSwitch, INPUT_PULLUP); // GPIO2 - Brown wire - lower float switch
  
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
}

void loop() {
  delay(1000);
  Serial.print("-");

  if(watchdogCounter++ > 300) // 5 minutes
  {
    broadcast("Running");
    watchdogCounter = 0;
  }

  WaterLevel detectedLevel;
  bool upperFloatSwitchOn = digitalRead(upperFloatSwitch) == LOW;
  bool lowerFloatSwitchOn = digitalRead(lowerFloatSwitch) == LOW;

  if(upperFloatSwitchOn)
  {
    if(lowerFloatSwitchOn)
    {
      detectedLevel = VeryLow;
    }
    else
    {
      detectedLevel = Low;
    }
  }
  else
  {
    if(lowerFloatSwitchOn)
    {
      detectedLevel = Error;
    }
    else
    {
      detectedLevel = Ok;
    }
  }

  updateStatus(detectedLevel);
}
