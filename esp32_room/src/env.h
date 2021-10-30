#pragma once
#define APP_CORE 0
#define PRO_CORE 1

//wifiSetup.h

#define EAP_IDENTITY "Vong Cat-Hide" //if connecting from another corporation, use identity@organisation.domain in Eduroam
#define EAP_PASSWORD "78787878"      //your Eduroam password


// buttons.h
// nút nhấn cần treo trở ngoài.
#define BUTTON_1 34
#define BUTTON_2 35
#define BUTTON_3 32
#define BUTTON_4 33

// mqtt.h
#define MQTT_SERVER "ngoinhaiot.com"
#define MQTT_USER "lastwillesp8266"
#define MQTT_PWK "123123123"
#define MQTT_PORT 1111

// output.h
#define LIGHT_1 23
#define LIGHT_2 19
#define LIGHT_3 18

// temprerature.h
#define PIN_HEAT1 26
#define PIN_HEAT2 27

#define ON_HEAT true
#define OFF_HEAT false