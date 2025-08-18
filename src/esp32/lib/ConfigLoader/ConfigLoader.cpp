#include "ConfigLoader.h"

Config ConfigLoader::config;

bool ConfigLoader::loadConfig(const char *filename)
{
    // remount LittleFS to ensure we can read the config file
    LittleFS.end();
    if (!LittleFS.begin(false))
    { // false = do not format
        Serial.println("❌ Failed to mount LittleFS");
        return false;
    }

    File file = LittleFS.open(filename, "r");
    if (!file)
    {
        Serial.printf("❌ Failed to open config file: %s\n", filename);
        return false;
    }

    if (file.size() == 0)
    {
        Serial.println("❌ Config file is empty!");
        file.close();
        return false;
    }

    DynamicJsonDocument doc(4096);
    DeserializationError error = deserializeJson(doc, file);
    file.close();

    if (error)
    {
        Serial.print("❌ Failed to parse config file: ");
        Serial.println(error.c_str());
        return false;
    }

    // ====== WiFi ======
    config.wifi_ssid = doc["wifi"]["ssid"] | "";
    config.wifi_password = doc["wifi"]["password"] | "";

    // ====== EMQX ======
    config.mqtt_broker = doc["emqx"]["broker"] | "";
    config.mqtt_username = doc["emqx"]["auth"]["username"] | "";
    config.mqtt_password = doc["emqx"]["auth"]["password"] | "";
    config.mqtt_port = doc["emqx"]["port"]["tls/ssl"] | 8883;
    config.ca_cert = doc["emqx"]["ca_cert"] | "";

    // ====== LED ======
    config.led_pin = doc["esp32"]["led"]["pin"] | -1;
    config.led_control_topic = doc["emqx"]["topic"]["led_control"] | "esp32-mcp/control/led";

    // ====== Buzzer ======
    config.buzzer_pin = doc["esp32"]["buzzer"]["pin"] | -1;
    config.buzzer_control_topic = doc["emqx"]["topic"]["buzzer_control"] | "esp32-mcp/control/buzzer";

    // ====== Validation ======
    bool configValid = true;

    if (config.wifi_ssid.isEmpty())
    {
        Serial.println("❌ WiFi SSID is required");
        configValid = false;
    }

    if (config.mqtt_broker.isEmpty())
    {
        Serial.println("❌ MQTT broker is required");
        configValid = false;
    }

    if (config.led_pin < 0)
    {
        Serial.println("⚠️ LED pin not configured");
    }

    if (config.buzzer_pin < 0)
    {
        Serial.println("⚠️ Buzzer pin not configured");
    }

    if (!configValid)
    {
        return false;
    }

    // ====== Print ======
    Serial.println("✅ Config loaded:");
    Serial.printf("  WiFi: %s / %s\n", config.wifi_ssid.c_str(), config.wifi_password.c_str());
    Serial.printf("  MQTT: %s:%d (user=%s)\n", config.mqtt_broker.c_str(), config.mqtt_port, config.mqtt_username.c_str());
    Serial.printf("  LED Pin: %d, Topic: %s\n", config.led_pin, config.led_control_topic.c_str());
    Serial.printf("  Buzzer Pin: %d, Topic: %s\n", config.buzzer_pin, config.buzzer_control_topic.c_str());

    Serial.println("Configuration loaded successfully");
    return true;
}

Config &ConfigLoader::getConfig()
{
    return config;
}