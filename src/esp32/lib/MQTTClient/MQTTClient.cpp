#include "MQTTClient.h"

// Define static members
WiFiClientSecure MQTTClient::wifiClient;
PubSubClient MQTTClient::mqttClient(MQTTClient::wifiClient);

/**
 * MQTT message callback
 * Called automatically when a message arrives on a subscribed topic
 */
void MQTTClient::mqttCallback(char *topic, byte *payload, unsigned int length)
{
    Serial.printf("Message arrived [%s]: ", topic);
    for (unsigned int i = 0; i < length; i++)
    {
        Serial.print((char)payload[i]);
    }
    Serial.println();

    // Parse JSON message
    DynamicJsonDocument doc(4096);
    DeserializationError error = deserializeJson(doc, payload, length);
    if (error)
    {
        Serial.print("deserializeJson() failed: ");
        Serial.println(error.c_str());
        return;
    }

    // Dispatch to the corresponding handler
    TopicHandler *handler = TopicHandlerRegistry::getHandlerForTopic(topic);
    if (handler)
    {
        handler->handleMessage(doc);
    }
    else
    {
        Serial.println("No handler found for topic");
    }
}

/**
 * Initialize the MQTT client (set server, callback, keep-alive, etc.)
 * This does not connect yet; call connectToMQTT() to actually connect
 */
void MQTTClient::init()
{
    const auto &cfg = ConfigLoader::getConfig();
    wifiClient.setCACert(cfg.ca_cert.c_str());
    mqttClient.setServer(cfg.mqtt_broker.c_str(), cfg.mqtt_port);
    mqttClient.setKeepAlive(60);
    mqttClient.setCallback(mqttCallback);
}

/**
 * Connect to the MQTT broker and subscribe to all registered topics
 */
void MQTTClient::connectToMQTT()
{
    if (mqttClient.connected())
        return;

    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.println("❌ WiFi not connected, cannot connect to MQTT");
        return;
    }

    Serial.print("🔗 Attempting MQTT connection...");
    String client_id = "esp32-client-" + String(WiFi.macAddress());

    const auto &cfg = ConfigLoader::getConfig();
    if (mqttClient.connect(client_id.c_str(), cfg.mqtt_username.c_str(), cfg.mqtt_password.c_str()))
    {
        Serial.println(" ✅ connected");

        // Subscribe to all registered topic handlers
        const auto &handlers = TopicHandlerRegistry::getAllHandlers();
        Serial.printf("📡 Subscribing to %d topics:\n", handlers.size());
        for (auto h : handlers)
        {
            if (mqttClient.subscribe(h->getTopic()))
            {
                Serial.printf("  ✅ %s\n", h->getTopic());
            }
            else
            {
                Serial.printf("  ❌ Failed to subscribe to %s\n", h->getTopic());
            }
        }
    }
    else
    {
        Serial.print(" ❌ failed, rc=");
        Serial.print(mqttClient.state());

        // explain the error code
        switch (mqttClient.state())
        {
        case -4:
            Serial.println(" (Connection timeout)");
            break;
        case -3:
            Serial.println(" (Connection lost)");
            break;
        case -2:
            Serial.println(" (Connect failed)");
            break;
        case -1:
            Serial.println(" (Disconnected)");
            break;
        case 1:
            Serial.println(" (Bad protocol)");
            break;
        case 2:
            Serial.println(" (Bad client ID)");
            break;
        case 3:
            Serial.println(" (Unavailable)");
            break;
        case 4:
            Serial.println(" (Bad credentials)");
            break;
        case 5:
            Serial.println(" (Unauthorized)");
            break;
        default:
            Serial.println(" (Unknown error)");
            break;
        }
    }
}
