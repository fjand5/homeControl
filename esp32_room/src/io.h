#include <Arduino.h>
#include <EasyButton.h>
#include "mqtt.h"

#define BUTTON_1 34
#define BUTTON_2 35
#define BUTTON_3 32
#define BUTTON_4 33

#define LIGHT_1 23
#define LIGHT_2 22
#define LIGHT_3 21
EasyButton button1(BUTTON_1);
EasyButton button2(BUTTON_2);
EasyButton button3(BUTTON_3);
EasyButton button4(BUTTON_4);

TimerHandle_t xOneMinuteTimer;
void onPressed4()
{
    Serial.println("Button pressed");
}
void onPressed1()
{
    digitalWrite(LIGHT_1, !digitalRead(LIGHT_1));
    String msg;
    msg = digitalRead(LIGHT_1) ? "on" : "off";
    mqtt_publish("esp32/light1", msg, true);
}
void onPressed2()
{
    digitalWrite(LIGHT_2, !digitalRead(LIGHT_2));
    String msg;
    msg = digitalRead(LIGHT_2) ? "on" : "off";
    mqtt_publish("esp32/light2", msg, true);
}
void onPressed3()
{
    digitalWrite(LIGHT_3, !digitalRead(LIGHT_3));
    String msg;
    msg = digitalRead(LIGHT_3) ? "on" : "off";
    mqtt_publish("esp32/light3", msg, true);
}
void ioHandle(void *pvParameters)
{
    button1.onPressed(onPressed1);
    button2.onPressed(onPressed2);
    button3.onPressed(onPressed3);
    button4.onPressed(onPressed4);
    while (1)
    {
        vTaskDelay(100 / portTICK_PERIOD_MS);
        // button1.read();
        // button2.read();
        // button3.read();
        // button4.read();
    }
}
void vTimerOneMinuteCallback( TimerHandle_t xTimer )
 {
     Serial.print(millis());
    Serial.println("Button pressed");

 }

void setupIO()
{
    pinMode(BUTTON_1, INPUT);
    pinMode(BUTTON_2, INPUT);
    pinMode(BUTTON_3, INPUT);
    pinMode(BUTTON_4, INPUT);

    pinMode(LIGHT_1, OUTPUT);
    pinMode(LIGHT_2, OUTPUT);
    pinMode(LIGHT_3, OUTPUT);

    button1.begin();
    button2.begin();
    button3.begin();
    button4.begin();
    xOneMinuteTimer = xTimerCreate(
        "xOneMinuteTimer",
        pdMS_TO_TICKS(60000),
        pdTRUE,
        0,
        vTimerOneMinuteCallback
        );
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
    xTaskCreate(ioHandle, "ioHandle", 2048, NULL, 0, NULL);
}