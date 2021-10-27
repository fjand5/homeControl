#include <Arduino.h>
#include "temperature.h"
#include "store.h"
#include "mqtt.h"
#include "wifiSetup.h"

void setup() {
  Serial.begin(115200);
  Serial.println("Booted...");
  setupWifi();
  setupConfig();
  setupTemperature();
  setupMQTT();
  vTaskDelete(NULL);
}

void loop() {
}