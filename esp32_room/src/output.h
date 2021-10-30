#include <Arduino.h>
#include "store.h"
#include "mqtt.h"
#include "buttons.h"
#define LIGHT_1 23
#define LIGHT_2 22
#define LIGHT_3 21

TimerHandle_t xOneMinuteTimer;
void vTimerOneMinuteCallback(TimerHandle_t xTimer)
{
    Serial.print(millis());
    Serial.println("Button pressed");
}

void setupOutput()
{
    pinMode(LIGHT_1, OUTPUT);
    pinMode(LIGHT_2, OUTPUT);
    pinMode(LIGHT_3, OUTPUT);

    
    setOnButton1Click(
        [](){
            digitalWrite(LIGHT_1, !digitalRead(LIGHT_1));
            String msg;
            msg = digitalRead(LIGHT_1) ? "on" : "off";
            mqtt_publish("esp32/light1", msg, true);
        });
    setOnButton2Click(
        [](){
            digitalWrite(LIGHT_2, !digitalRead(LIGHT_2));
            String msg;
            msg = digitalRead(LIGHT_2) ? "on" : "off";
            mqtt_publish("esp32/light2", msg, true);
        });
    setOnButton3Click(
        [](){
            digitalWrite(LIGHT_3, !digitalRead(LIGHT_3));
            String msg;
            msg = digitalRead(LIGHT_3) ? "on" : "off";
            mqtt_publish("esp32/light3", msg, true);
        });
    xOneMinuteTimer = xTimerCreate(
        "xOneMinuteTimer",
        pdMS_TO_TICKS(60000),
        pdTRUE,
        0,
        vTimerOneMinuteCallback);
    //   vTaskStartScheduler();
    xTimerStart(xOneMinuteTimer, 0);

    setOnMqttReciveCallbacks(
        [](String subTopic, String msg)
        {
            if (subTopic == "esp32/light1")
            {
                bool state = msg == "on" ? true : false;
                digitalWrite(LIGHT_1, state);
            }
            else if (subTopic == "esp32/light2")
            {
                bool state = msg == "on" ? true : false;
                digitalWrite(LIGHT_2, state);
            }
            else if (subTopic == "esp32/light3")
            {
                bool state = msg == "on" ? true : false;
                digitalWrite(LIGHT_3, state);
            }
            else if (subTopic == "esp32/timer_on1")
            {
                setValue("timer_on1", msg, true);
            }
            else if (subTopic == "esp32/timer_off1")
            {
                setValue("timer_off1", msg, true);
            }
            else if (subTopic == "esp32/timer_on2")
            {
                setValue("timer_on2", msg, true);
            }
            else if (subTopic == "esp32/timer_off2")
            {
                setValue("timer_off2", msg, true);
            }
            else if (subTopic == "esp32/timer_on3")
            {
                setValue("timer_on3", msg, true);
            }
            else if (subTopic == "esp32/timer_off3")
            {
                setValue("timer_off3", msg, true);
            }
        });
}