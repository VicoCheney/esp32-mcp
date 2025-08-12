#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include "ConfigLoader.h"
#include "TopicHandlerRegistry.h"

class MQTTClient
{
public:
    /** 
     * MQTT callback function called when a message arrives
     * @param topic MQTT topic of the message
     * @param payload Pointer to the message payload
     * @param length Length of the payload
     */
    static void mqttCallback(char *topic, byte *payload, unsigned int length);

    /**
     * Initialize the MQTT client (set server, callback, etc.)
     * This does not connect yet
     */
    static void init();

    /**
     * Connect to the MQTT broker and subscribe to all registered topics
     */
    static void connectToMQTT();

    /**
     * Access the internal PubSubClient instance if needed
     * @return Reference to the PubSubClient object
     */
    static PubSubClient &getMQTTClient() { return mqttClient; }

    /**
     * Call this in the main loop to keep MQTT client running
     */
    static void loop() { mqttClient.loop(); }

private:
    static WiFiClientSecure wifiClient;  // Secure WiFi client for TLS/SSL
    static PubSubClient mqttClient;      // MQTT client instance
};
