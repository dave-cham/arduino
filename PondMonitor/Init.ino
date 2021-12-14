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
  
  sensors.begin();
  pinMode(upperFloatSwitch, INPUT_PULLUP); // GPIO0 - Yellow wire - upper float switch
  pinMode(lowerFloatSwitch, INPUT_PULLUP); // GPIO2 - Brown wire - lower float switch
  
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
}
