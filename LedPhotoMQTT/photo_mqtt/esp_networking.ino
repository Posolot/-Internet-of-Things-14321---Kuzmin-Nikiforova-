#define PIN_PHOTO_SENSOR A0
#include "Config.h"
#include "WIFI.h"
#include "Server.h"
#include "MQTT.h"

int minval = 1000000;
int maxval = 0;

void setup(void){
  Serial.begin(9600);
  WIFI_init(false);
  MQTT_init();
  pinMode(led, OUTPUT);
}

void loop(void){
  int val = analogRead(PIN_PHOTO_SENSOR);
  String comled = "d";
  // Serial.println(val);
  if (minval > val){
    minval = val;
  }
  if (maxval < val){
    maxval = val;
  }
  if (val>((minval+maxval)/2)){
    comled="u";
  }
  mqtt_cli.publish("esp8266/dashaIlya/setCommand", comled.c_str());
  delay(500);                  

}