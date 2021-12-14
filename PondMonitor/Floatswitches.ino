
void checkWaterLevel()
{
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

void updateStatus(WaterLevel detectedLevel)
{
  if(waterLevel != detectedLevel)
  {
    char* payload = getMqttMessagePayload(detectedLevel);
    Serial.print("Sending MQTT message: ");
    Serial.println(payload);
    if(broadcast("level", payload))
    {
      waterLevel = detectedLevel;
    }
  }
}

char* getMqttMessagePayload(WaterLevel level)
{
  switch(level)
  {
    case Ok: return "Ok";
    case Low: return "Low";
    case VeryLow: return "VeryLow";
    default: return "Error";
  }
}
