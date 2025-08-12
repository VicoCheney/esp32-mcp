#include <Arduino.h>
#include <LittleFS.h>
#include "ConfigLoader.h"
#include "TopicHandlerRegistry.h"
#include "MQTTClient.h"
#include "OTAHandler.h"
#include "WiFiManager.h"

void setup()
{
    Serial.begin(115200);

    // Initialize LittleFS
    if (!LittleFS.begin()) {
        Serial.println("⚠️ Failed to initialize LittleFS");
    }

    // Wait until config.json is uploaded and loaded
    while (!ConfigLoader::loadConfig())
    {
        Serial.println("⚠️ No valid config.json, please upload via LittleFS...");
        delay(3000);
    }
    Serial.println("✅ Config loaded!");

    // Initialize all topic handlers (LED, Buzzer, etc.)
    TopicHandlerRegistry::initAllHandlers();

    // Connect to WiFi
    WiFiManager::begin();

    // Initialize MQTT client
    MQTTClient::init();
    MQTTClient::connectToMQTT();

    // Initialize OTA update handling
    OTAHandler::init();
}

void loop()
{
    // Handle OTA update requests
    OTAHandler::handle();

    // Ensure MQTT connection
    if (!MQTTClient::getMQTTClient().connected())
    {
        MQTTClient::connectToMQTT();
    }

    // Keep MQTT client running
    MQTTClient::loop();

    // Optional: handle WiFi auto-reconnect
    WiFiManager::reconnectIfNeeded();
}
