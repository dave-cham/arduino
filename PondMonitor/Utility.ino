
bool broadcast(char* message, char* payload) 
{
  Serial.println("Publishing message on MQTT");

  if(!client.connected())
  {
    if(!client.connect(mqtt_client_name, mqtt_user, mqtt_pass))
    {
      int status = client.state();
      Serial.print("MQTT Error: ");
      Serial.println(status);
      return false;
    }
  }

  char topic[100];
  sprintf(topic, "pond/%s/status", message);
  if(!client.publish(topic, payload))
  {
    Serial.println("Failed to send MQTT message");
    return false;
  }

  return true;
}
