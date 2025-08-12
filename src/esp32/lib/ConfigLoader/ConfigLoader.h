#pragma once

#include <Arduino.h>
#include <FS.h>
#include <LittleFS.h>
#include <ArduinoJson.h>

// Configuration structure
struct Config {
    String wifi_ssid;
    String wifi_password;
    String mqtt_broker;
    String mqtt_username;
    String mqtt_password;
    int mqtt_port;
    String ca_cert;
    int led_pin;
    String led_control_topic;
    int buzzer_pin;
    String buzzer_control_topic;
};

class ConfigLoader {
public:
    static bool loadConfig(const char* filename = "/config.json");
    static Config& getConfig();

private:
    static Config config;
};
