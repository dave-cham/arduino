
bool broadcast(char* payload) 
{
  Serial.print("Publishing message on MQTT:");
  Serial.print(mqtt_client_name);
  Serial.print(" - ");
  Serial.println(payload);

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

  if(!client.publish(mqtt_client_name, payload))
  {
    Serial.println("Failed to send MQTT message");
    return false;
  }

  return true;
}
