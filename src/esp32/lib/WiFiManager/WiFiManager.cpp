#include "WiFiManager.h"

bool WiFiManager::connected = false;

void WiFiManager::begin()
{
    auto &cfg = ConfigLoader::getConfig();

    if (cfg.wifi_ssid.isEmpty())
    {
        Serial.println("❌ WiFi SSID is empty!");
        return;
    }

    WiFi.begin(cfg.wifi_ssid.c_str(), cfg.wifi_password.c_str());
    Serial.print("Connecting to WiFi");

    int attempts = 0;
    const int maxAttempts = 30; // timeout

    while (WiFi.status() != WL_CONNECTED && attempts < maxAttempts)
    {
        delay(500);
        Serial.print(".");
        attempts++;
    }

    if (WiFi.status() == WL_CONNECTED)
    {
        connected = true;
        Serial.println("\n✅ WiFi connected!");
        Serial.printf("IP address: %s\n", WiFi.localIP().toString().c_str());
    }
    else
    {
        Serial.println("\n❌ WiFi connection failed!");
        connected = false;
    }
}

bool WiFiManager::isConnected()
{
    return WiFi.status() == WL_CONNECTED;
}

void WiFiManager::reconnectIfNeeded()
{
    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.println("🔄 WiFi disconnected, attempting to reconnect...");
        connected = false;
        WiFi.disconnect();
        delay(1000);
        begin();
    }
}