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

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println("Startup...");

  pinMode(doorbellSwitchInput, INPUT_PULLUP); // GPIO0 - Yellow wire - doorbell switch
  pinMode(doorbellRingOutput, OUTPUT); // GPIO2 - Brown wire - doorbell ringer

  digitalWrite(doorbellRingOutput, LOW);
  
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
}
