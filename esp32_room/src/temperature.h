#include "env.h"
#include <Arduino.h>
#include "store.h"
#include "mqtt.h"

#define PIN_HEAT1 18
#define PIN_HEAT2 19

#define ON_HEAT true
#define OFF_HEAT false
void loopTemperature(void *pvParameters)
{
  setOnMqttReciveCallbacks(
      [](String subTopic, String msg)
      {
        if (subTopic == "temp_upper1")
        {
          setValue("temp_upper1", msg, true);
        }
        else if (subTopic == "temp_upper2")
        {
          setValue("temp_upper2", msg, true);
        }
        else if (subTopic == "temp_lower1")
        {
          setValue("temp_lower1", msg, true);
        }
        else if (subTopic == "temp_lower2")
        {
          setValue("temp_lower2", msg, true);
        }
      });
  setOnConfigChange(
      [](String key, String val)
      {
        if (key == "temp1")
        {
          if (val.toInt() >= getValue("temp_upper1", "99").toInt())
          {
            digitalWrite(PIN_HEAT1, OFF_HEAT);
          }
          else if (val.toInt() <= getValue("temp_lower1", "0").toInt())
          {
            digitalWrite(PIN_HEAT1, ON_HEAT);
          }
        }
        else if (key == "temp2")
        {
          if (val.toInt() >= getValue("temp_upper2", "99").toInt())
          {
            digitalWrite(PIN_HEAT2, OFF_HEAT);
          }
          else if (val.toInt() <= getValue("temp_lower2", "0").toInt())
          {
            digitalWrite(PIN_HEAT2, ON_HEAT);
          }
        }
       
      });
  pinMode(PIN_HEAT1, OUTPUT);
  pinMode(PIN_HEAT2, OUTPUT);
  mqtt_publish("temp_upper1", getValue("temp_upper1"), true);
  mqtt_publish("temp_upper2", getValue("temp_upper2"), true);
  mqtt_publish("temp_lower1", getValue("temp_lower1"), true);
  mqtt_publish("temp_lower2", getValue("temp_lower2"), true);

  while (1)
  {
    vTaskDelay(3000 / portTICK_PERIOD_MS);
    setValue("temp1", String(random(20, 50)), false);
    mqtt_publish("temp1", getValue("temp1"));
    vTaskDelay(3000 / portTICK_PERIOD_MS);
    setValue("temp2", String(random(20, 50)), false);
    mqtt_publish("temp2", getValue("temp2"));
  }
}

void setupTemperature()
{
  // put your setup code here, to run once:
  xTaskCreate(loopTemperature, "loopTemperature", 10000, NULL, 1, NULL);
}