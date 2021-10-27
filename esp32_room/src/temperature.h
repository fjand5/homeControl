#include "env.h"
#include <Arduino.h>
#include "store.h"
#include "mqtt.h"
void loopTemperature(void *pvParameters) {
    while (1)
    {
        vTaskDelay(1000/ portTICK_PERIOD_MS);
        setValue("temp1", String(random(20,50)), true);
        mqtt_publish("temp1", getValue("temp1"));
        vTaskDelay(1000/ portTICK_PERIOD_MS);
        setValue("temp2", String(random(20,50)), true);
        mqtt_publish("temp2", getValue("temp2"));

    }
    
}


void setupTemperature() {
  // put your setup code here, to run once:
  xTaskCreate(loopTemperature, "loopTemperature", 2048, NULL, 1, NULL);
}
uint32_t timerTemperature = millis();