#pragma once
#include "env.h"
#include <PubSubClient.h>
#include <WiFiClient.h>
#include <list>

typedef void (*mqttReciveCallback)(String, String);
std::list<mqttReciveCallback> mqttReciveCallbacks;
void setOnMqttReciveCallbacks(void (*func)(String key, String value))
{
    mqttReciveCallbacks.push_front(func);
}

WiFiClient espClient;
PubSubClient client(espClient);
SemaphoreHandle_t mqtt_pub_sem;
bool requirePublishOnline = false;
void mqtt_publish(String subTopic, String data, bool retain = false)
{
    String topic = String(MQTT_USER) + "/" + subTopic;

    if (mqtt_pub_sem != NULL)
    {
        if (xSemaphoreTake(mqtt_pub_sem, portMAX_DELAY) == pdTRUE)
        {
            if (client.connected())
            {
                client.publish(topic.c_str(), data.c_str(), retain);
            }

            xSemaphoreGive(mqtt_pub_sem);
        }
    }
}
void callback(char *topic, byte *payload, unsigned int length)
{
    String msg;

    String subTopic = String(topic).substring(String(MQTT_USER).length() + 1);

    for (int i = 0; i < length; i++)
    {
        msg += (char)payload[i];
    }
    Serial.println(String("mqtt cb: ") + topic + " : " + msg);
    if(subTopic == "esp32/isOnline"
    && msg != "true"
    ){
        requirePublishOnline = true;
        
    }
    for (auto mqttReciveCallback = mqttReciveCallbacks.begin();
         mqttReciveCallback != mqttReciveCallbacks.end();
         ++mqttReciveCallback)
    {
        if ((*mqttReciveCallback) != NULL)
        {
            (*mqttReciveCallback)(subTopic, msg);
        }
    }
}

void reconnect()
{
    // Loop until we're reconnected
    while (!client.connected())
    {
        Serial.print("Attempting MQTT connection...");
        // Create a random client ID
        String clientId = "ESP8266Client-";
        clientId += String(random(0xffff), HEX);
        // Attempt to connect
        if (client.connect(clientId.c_str(), MQTT_USER, MQTT_PWK,
                           MQTT_USER "/esp32/isOnline", 1, true, "false"))
        {
            client.subscribe(MQTT_USER "/#");
            Serial.println("MQTT connected");
        }
        else
        {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            // Wait 5 seconds before retrying
            delay(5000);
        }
    }
}
void mqttHandle(void *arg)
{
    mqtt_pub_sem = xSemaphoreCreateBinary();
    reconnect();
    xSemaphoreGive(mqtt_pub_sem);

    while (1)
    {
        if (!client.connected())
        {
            reconnect();
        }
        client.loop();
        if(requirePublishOnline){
            mqtt_publish("esp32/isOnline", "true", true);
            requirePublishOnline = false;
        }
        vTaskDelay(100/portTICK_PERIOD_MS);
    }
}
void setupMQTT()
{
    client.setServer(MQTT_SERVER, MQTT_PORT);
    client.setCallback(callback);
    xTaskCreate(mqttHandle, "mqttHandle", 20000, NULL, 0, NULL);
}