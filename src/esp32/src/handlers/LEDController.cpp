#include "LEDController.h"

// LED controller topic
const char *LEDController::getTopic() const
{
    return ConfigLoader::getConfig().led_control_topic.c_str();
}

// LED initialization
void LEDController::init()
{
    pinMode(ConfigLoader::getConfig().led_pin, OUTPUT);
    digitalWrite(ConfigLoader::getConfig().led_pin, LOW);
}

// handleMessage
void LEDController::handleMessage(DynamicJsonDocument &doc)
{
    if (!doc.containsKey("command"))
    {
        Serial.println(F("LEDController: missing 'command'"));
        return;
    }
    String command = doc["command"].as<String>();
    command.toLowerCase();

    if (command == "on")
    {
        _turnOnLED();
    }
    else if (command == "off")
    {
        _turnOffLED();
    }
    else if (command == "set_brightness")
    {
        if (!doc.containsKey("brightness") || !doc["brightness"].is<int>())
        {
            Serial.println(F("LEDController: invalid or missing 'brightness'"));
            return;
        }
        int brightness = doc["brightness"].as<int>();
        brightness = constrain(brightness, 0, 255);
        _setBrightness(brightness);
    }
    else
    {
        Serial.print(F("LEDController: unknown command: "));
        Serial.println(command);
    }
}

// LED control methods
void LEDController::_turnOnLED()
{
    digitalWrite(ConfigLoader::getConfig().led_pin, HIGH);
    Serial.println("turn on LED");
}

void LEDController::_turnOffLED()
{
    digitalWrite(ConfigLoader::getConfig().led_pin, LOW);
    Serial.println("turn off LED");
}

void LEDController::_setBrightness(int brightness)
{
    analogWrite(ConfigLoader::getConfig().led_pin, brightness);
    Serial.print(F("set LED brightness to: "));
    Serial.println(brightness);
}

// automatically register the LEDController
AUTO_REGISTER_HANDLER(LEDController);