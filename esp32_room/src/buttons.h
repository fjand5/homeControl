#include <Arduino.h>
#include <EasyButton.h>

#define BUTTON_1 34
#define BUTTON_2 35
#define BUTTON_3 32
#define BUTTON_4 33

EasyButton button1(BUTTON_1);
EasyButton button2(BUTTON_2);
EasyButton button3(BUTTON_3);
EasyButton button4(BUTTON_4);

void (*onButton1Click)(void);
void (*onButton2Click)(void);
void (*onButton3Click)(void);

void setOnButton1Click(void (*func)(void)){
    onButton1Click=func;
}
void setOnButton2Click(void (*func)(void)){
    onButton2Click=func;
}
void setOnButton3Click(void (*func)(void)){
    onButton3Click=func;
}
void onPressed4()
{
    Serial.println("Button pressed");
}
void onPressed1()
{
    Serial.println("onPressed1");

    if(onButton1Click != NULL)
        onButton1Click();
}
void onPressed2()
{   
    if(onButton2Click != NULL)
        onButton2Click();
}
void onPressed3()
{
    if(onButton3Click != NULL)
        onButton3Click();
}
void buttonsHandle(void *pvParameters)
{
    button1.onPressed(onPressed1);
    button2.onPressed(onPressed2);
    button3.onPressed(onPressed3);
    button4.onPressed(onPressed4);
    while (1)
    {
        vTaskDelay(100 / portTICK_PERIOD_MS);
        button1.read();
        button2.read();
        button3.read();
        button4.read();
    }
}

void setupButtons()
{
    pinMode(BUTTON_1, INPUT);
    pinMode(BUTTON_2, INPUT);
    pinMode(BUTTON_3, INPUT);
    pinMode(BUTTON_4, INPUT);

    button1.begin();
    button2.begin();
    button3.begin();
    button4.begin();

    xTaskCreate(buttonsHandle, "buttonsHandle", 2048, NULL, 0, NULL);
}