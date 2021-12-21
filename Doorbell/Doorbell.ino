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
//const char *mqtt_topic = "doorbell/status";
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
unsigned long minDingTime = 1000;
DoorbellState state = Dong;

void loop() {
  bool isButtonPressed = readButtonState();
  setBellStatus(isButtonPressed);

  if(isButtonPressed)
  {
    if(whenDingStarted == 0)
    {
      whenDingStarted = millis();
    }
  }
  else
  {
    if(millis() - whenDingStarted < minDingTime) {
      isButtonPressed = true;
    }
    else {
      whenDingStarted = 0;
    }
  }

  updateStatus(isButtonPressed?Ding:Dong);
}

bool readButtonState()
{
  bool isPressed = digitalRead(doorbellSwitchInput) == LOW;
  if(isPressed)
  {
    // Check again that the button is pressed - avoid interference causing false Dings
    delay(5);
    if(digitalRead(doorbellSwitchInput) != LOW)
    {
      Serial.println("Interference");
      return false;
    }
  }
  return isPressed;
}

void setBellStatus(bool isPressed)
{
  digitalWrite(doorbellRingOutput, isPressed?LOW:HIGH);
}

void updateStatus(DoorbellState newState)
{
  if(state != newState)
  {
    char* payload = getMqttMessagePayload(newState);
    Serial.print("Sending MQTT message: ");
    Serial.println(payload);
    //if(broadcast(payload))
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
