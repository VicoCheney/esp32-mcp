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
    String command = doc["command"].as<String>();

    if (command == "on")
    {
        turnOnLED();
    }
    else if (command == "off")
    {
        turnOffLED();
    }
    else if (command == "set_brightness")
    {
        int brightness = doc["brightness"].as<int>();
        setBrightness(brightness);
    }
}

// LED control methods
void LEDController::turnOnLED()
{
    digitalWrite(ConfigLoader::getConfig().led_pin, HIGH);
    Serial.println("turn on LED");
}

void LEDController::turnOffLED()
{
    digitalWrite(ConfigLoader::getConfig().led_pin, LOW);
    Serial.println("turn off LED");
}

void LEDController::setBrightness(int brightness)
{
    analogWrite(ConfigLoader::getConfig().led_pin, brightness);
    Serial.print("set LED brightness to: ");
    Serial.println(brightness);
}

// automatically register the LEDController
AUTO_REGISTER_HANDLER(LEDController);