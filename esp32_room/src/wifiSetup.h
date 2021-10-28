#pragma once
#include <Arduino.h>
#include <WiFi.h>
#define EAP_IDENTITY "Vong Cat 3" //if connecting from another corporation, use identity@organisation.domain in Eduroam
#define EAP_PASSWORD "78787878"      //your Eduroam password
void setupWifi()
{
    WiFi.mode(WIFI_AP);
    Serial.print("Connecting to network: ");
    Serial.println(EAP_IDENTITY);
    WiFi.disconnect(true);                  //disconnect form wifi to set new wifi connection
    WiFi.mode(WIFI_STA);                    //init wifi mode
    WiFi.begin(EAP_IDENTITY, EAP_PASSWORD); //connect to wifi
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address set: ");
    Serial.println(WiFi.localIP()); //print LAN IP
}