#include "Config.h"
#include "WIFI.h"
#include "Server.h"
#include "MQTT.h"

bool serviceRunning = false;
String commandTopic = "";

void setup() {
  Serial.begin(9600);
  pinMode(led, OUTPUT);
}

void loop() {
  if (commandTopic.isEmpty()) {
    requestTopic();
  } else if (serviceRunning) {
    mqtt_cli.loop();
  } else {
    initializeService();
  }
}

void initializeService() {
  WIFI_init(false);
  Serial.println("Starting MQTT service...");
  MQTT_init();
  mqtt_cli.subscribe(commandTopic.c_str(), 1);
  serviceRunning = true;
}

void requestTopic() {
  if (commandTopic.isEmpty()) {
    Serial.println("Please enter the MQTT topic for commands:");
  }

  if (Serial.available() > 0) {
    String receivedTopic = Serial.readString();
    Serial.println(receivedTopic);
    if (commandTopic.isEmpty()) {
      commandTopic = receivedTopic;
    }
  }
}
