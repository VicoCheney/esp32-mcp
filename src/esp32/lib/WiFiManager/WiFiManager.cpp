#include "WiFiManager.h"

bool WiFiManager::connected = false;

void WiFiManager::begin() {
    auto &cfg = ConfigLoader::getConfig();
    WiFi.begin(cfg.wifi_ssid.c_str(), cfg.wifi_password.c_str());
    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    connected = true;
    Serial.println("WiFi connected!");
}

bool WiFiManager::isConnected() {
    return connected;
}

void WiFiManager::reconnectIfNeeded() {
    if (WiFi.status() != WL_CONNECTED) {
        WiFi.disconnect();
        begin();
    }
}