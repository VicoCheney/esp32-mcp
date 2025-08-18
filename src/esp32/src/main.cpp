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
    if (!LittleFS.begin())
    {
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

    // Check WiFi connection first
    if (WiFi.status() != WL_CONNECTED)
    {
        WiFiManager::reconnectIfNeeded();
        delay(1000);
        return;
    }

    // Ensure MQTT connection
    if (!MQTTClient::getMQTTClient().connected())
    {
        MQTTClient::connectToMQTT();
        delay(5000); // Wait before retry
        return;
    }

    // Keep MQTT client running
    MQTTClient::loop();

    // Small delay to prevent watchdog reset
    delay(10);
}
