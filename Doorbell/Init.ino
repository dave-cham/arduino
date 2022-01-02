void setup_wifi() 
{
  //Serial.print("Connecting WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(100);
    //Serial.print(".");
  }
  //Serial.println();
  //Serial.println("WiFi Connected");
}

void setup_ota() {
  ArduinoOTA.setHostname("doorbell");
  
  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_FS
      type = "filesystem";
    }

    // NOTE: if updating FS this would be the place to unmount FS using FS.end()
    //Serial.println("Start updating " + type);
  });
  
  ArduinoOTA.onEnd([]() {
    //Serial.println("\nEnd");
  });
  
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    //Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  
  ArduinoOTA.onError([](ota_error_t error) {
    //Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      //Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      //Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      //Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      //Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      //Serial.println("End Failed");
    }
  });
  
  ArduinoOTA.begin();
  //Serial.println("Ready");
  //Serial.print("IP address: ");
  //Serial.println(WiFi.localIP());
}

void setup() {
  //Serial.begin(115200);
  //Serial.println();
  //Serial.println("Startup...");

  pinMode(doorbellSwitchInput, INPUT_PULLUP); // GPIO0 - Yellow wire - doorbell switch
  pinMode(doorbellRingOutput, OUTPUT); // GPIO2 - Brown wire - doorbell ringer
  pinMode(LED_BUILTIN, OUTPUT);

  digitalWrite(doorbellRingOutput, LOW);
  
  setup_wifi();
  setup_ota();
  client.setServer(mqtt_server, mqtt_port);
}
