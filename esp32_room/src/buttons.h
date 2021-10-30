#pragma once
#include "env.h"
#include <Arduino.h>
#include <EasyButton.h>
#define CTX_TOGLE 1
#define CTX_ADJ 2

EasyButton button1(BUTTON_1);
EasyButton button2(BUTTON_2);
EasyButton button3(BUTTON_3);
EasyButton button4(BUTTON_4);

void (*onButton1Click)(void);
void (*onButton2Click)(void);
void (*onButton3Click)(void);
void (*onButton4Click)(void);


int context_button = CTX_TOGLE;
void setOnButton1Click(void (*func)(void)){
    onButton1Click=func;
}
void setOnButton2Click(void (*func)(void)){
    onButton2Click=func;
}
void setOnButton3Click(void (*func)(void)){
    onButton3Click=func;
}
void setOnButton4Click(void (*func)(void)){
    onButton4Click=func;
}
void onPressed4()
{
    if(onButton4Click != NULL)
        onButton4Click();
}
void changeContext(){
    if(context_button == CTX_ADJ)
        context_button = CTX_TOGLE;
    if(context_button == CTX_TOGLE)
        context_button = CTX_ADJ;
}
void onPressed1()
{

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
    button4.onPressedFor(2000, changeContext);
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