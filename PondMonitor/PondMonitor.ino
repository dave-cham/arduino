// ESP-01 Project to monitor the pond:
// Inputs from float switches
// Publish status to MQTT
// Output to refill valve

#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <OneWire.h>
#include <DallasTemperature.h>

//USER CONFIGURED SECTION START//
//const char* ssid = "";
//const char* password = "";
//const char* mqtt_server = "";
//const int mqtt_port = 1883;
//const char *mqtt_user = "";
//const char *mqtt_pass = "";
//const char *mqtt_client_name = "pond-level-monitor"; 
//const char *mqtt_topic = "pond/level/status";
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
OneWire oneWire(2);
DallasTemperature sensors(&oneWire);

int upperFloatSwitch = 0;
int lowerFloatSwitch = 2;
WaterLevel waterLevel = Ok;
unsigned long lastTempCheck = 0;

void loop() {
  delay(1000);
  Serial.print("-");

  checkWaterLevel();

  unsigned long millisecs = millis();
  if(millisecs - lastTempCheck > 30000) // 30 seconds
  {
    checkTemperature();
    lastTempCheck = millisecs;
  }
}
