#pragma once
#include <PubSubClient.h>
#include <WiFiClient.h>
#define mqtt_server "ngoinhaiot.com"
#define mqtt_user "lastwillesp8266"
#define mqtt_pwk "123123123"
const uint16_t mqtt_port = 1111;
WiFiClient espClient;
PubSubClient client(espClient);
#define MSG_BUFFER_SIZE (2048)
SemaphoreHandle_t mqtt_pub_sem;
void mqtt_publish(String subTopic, String data)
{
    String topic = String(mqtt_user)+"/"+subTopic;
    if (mqtt_pub_sem != NULL)
    {
        if (xSemaphoreTake(mqtt_pub_sem, (TickType_t)10) == pdTRUE)
        {
     
            client.publish(topic.c_str(), data.c_str());
            xSemaphoreGive(mqtt_pub_sem);
        }
    }
}
void callback(char *topic, byte *payload, unsigned int length)
{
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    for (int i = 0; i < length; i++)
    {
        Serial.print((char)payload[i]);
    }
    Serial.println();
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
        if (client.connect(clientId.c_str(), mqtt_user, mqtt_pwk,
                           mqtt_user "/isOnline/", 2, true, "false"))
        {
            Serial.println("connected");
            // Once connected, publish an announcement...
            // ... and resubscribe
            client.publish(mqtt_user "/isOnline/", (uint8_t *)"true", 4, true);
            client.subscribe(mqtt_user "/#");
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
    xSemaphoreGive(mqtt_pub_sem);
    while (1)
    {
        if (!client.connected())
        {
            reconnect();
        }
        client.loop();
    }
}
void setupMQTT()
{
    client.setServer(mqtt_server, mqtt_port);
    client.setCallback(callback);
    xTaskCreate(mqttHandle, "mqttHandle", 10000, NULL, 0, NULL);
}