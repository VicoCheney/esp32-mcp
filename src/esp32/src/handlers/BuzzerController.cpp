#include "BuzzerController.h"

// BuzzerController topic
const char *BuzzerController::getTopic() const
{
    return ConfigLoader::getConfig().buzzer_control_topic.c_str();
}

// buzzer initialization
void BuzzerController::init()
{
    pinMode(ConfigLoader::getConfig().buzzer_pin, OUTPUT);
}

// handleMessage
void BuzzerController::handleMessage(DynamicJsonDocument &doc)
{
    if (!doc.containsKey("command") || !doc["command"].is<const char *>())
    {
        Serial.println("BuzzerController: missing 'command' field");
        return;
    }
    const char *command = doc["command"].as<const char *>();
    if (!command)
    {
        Serial.println("BuzzerController: 'command' is null");
        return;
    }

    if (strcasecmp(command, "ON") == 0)
    {
        _turnOnBuzzer();
    }
    else if (strcasecmp(command, "OFF") == 0)
    {
        _turnOffBuzzer();
    }
    else if (strcasecmp(command, "BEEP") == 0)
    {
        int duration = doc["duration"].is<int>() ? doc["duration"].as<int>() : 100;
        duration = constrain(duration, 10, 10000); // 10ms - 10s
        _beep(duration);
    }
    else
    {
        Serial.printf("BuzzerController: unknown command '%s'\n", command);
    }
}

// --- buzzer control methods ---

void BuzzerController::_turnOnBuzzer()
{
    Serial.println("Buzzer ON");
    tone(ConfigLoader::getConfig().buzzer_pin, 1000); // Play a 1kHz tone
}

void BuzzerController::_turnOffBuzzer()
{
    Serial.println("Buzzer OFF");
    noTone(ConfigLoader::getConfig().buzzer_pin);
}

void BuzzerController::_beep(int duration)
{
    Serial.printf("Buzzer BEEP for %dms\n", duration);
    tone(ConfigLoader::getConfig().buzzer_pin, 1000, duration); // Beep for specified duration
}

// Automatically register the BuzzerController
AUTO_REGISTER_HANDLER(BuzzerController);