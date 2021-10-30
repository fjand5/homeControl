#include <Arduino.h>
#include "temperature.h"
#include "store.h"
#include "mqtt.h"
#include "wifiSetup.h"
#include "output.h"

void setup() {
  Serial.begin(115200);
  Serial.println("Booted...");
  setupConfig();
  setupButtons();
  setupOutput();
  setupWifi();
  setupMQTT();
  setupTemperature();

  vTaskDelete(NULL);
}

void loop() {
}