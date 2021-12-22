// ESP-01 Project send an MQTT message when the doorbell rings

#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>

//USER CONFIGURED SECTION START//
//const char* ssid = "";
//const char* password = "";
//const char* mqtt_server = "";
//const int mqtt_port = 1883;
//const char *mqtt_user = "";
//const char *mqtt_pass = "";
//const char *mqtt_client_name = "pond-level-monitor"; 
//USER CONFIGURED SECTION END//

WiFiClient espClient;
PubSubClient client(espClient);

enum DoorbellState {
  Ding = 1,
  Dong = 0
};

int doorbellSwitchInput = 2;
int doorbellRingOutput = 0;
unsigned long whenDingStarted = 0;
unsigned long whenMqttStarted = 0;
unsigned long minDingTime = 200;
unsigned long minMqttTime = 1000;
DoorbellState state = Dong;

void loop() {
  bool isButtonPressed = readButtonState();

  DoorbellState mqttStatus = Dong;
  DoorbellState dingStatus = Dong;
  if(isButtonPressed)
  {
    if(whenDingStarted == 0)
    {
      whenDingStarted = millis();
    }
    if(whenMqttStarted == 0)
    {
      whenMqttStarted = millis();
    }
    mqttStatus = Ding;
    dingStatus = Ding;
  }
  else
  {
    if(millis() - whenDingStarted < minDingTime) {
      dingStatus = Ding;
    }
    else {
      whenDingStarted = 0;
    }
    if(millis() - whenMqttStarted < minMqttTime) {
      mqttStatus = Ding;
    }
    else {
      whenMqttStarted = 0;
    }
  }

  setBellStatus(dingStatus);
  updateMqttStatus(mqttStatus);
}

bool readButtonState()
{
  bool isPressed = digitalRead(doorbellSwitchInput) == LOW;
  if(isPressed)
  {
    Serial.println("pressed");
    
    // Check again that the button is pressed - avoid interference causing false Dings
    delay(5);
    if(digitalRead(doorbellSwitchInput) != LOW)
    {
      Serial.println("Interference");
      return false;
    }
    else
    {
      Serial.println("deffo pressed");
    }
  }
  return isPressed;
}

DoorbellState lastStatus = Dong;
void setBellStatus(DoorbellState status)
{
  if(status != lastStatus)
  {
    Serial.println(status==Ding?"Ding":"Dong");
    lastStatus = status;
  }
  
  digitalWrite(doorbellRingOutput, status==Ding ? HIGH : LOW);
}

void updateMqttStatus(DoorbellState newState)
{
  if(state != newState)
  {
    char* payload = getMqttMessagePayload(newState);
    Serial.print("Sending MQTT message: ");
    Serial.println(payload);
    if(broadcast(payload))
    {
      state = newState;
    }
  }
}

char* getMqttMessagePayload(DoorbellState state)
{
  switch(state)
  {
    case Dong: return "Dong";
    case Ding: return "Ding";
    default: return "Error";
  }
}
