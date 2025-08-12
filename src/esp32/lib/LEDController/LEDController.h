#pragma once

#include <Arduino.h>
#include "TopicHandler.h"
#include "TopicHandlerRegistry.h"
#include "ConfigLoader.h"

class LEDController : public TopicHandler
{
public:
    void init() override;
    void handleMessage(DynamicJsonDocument &doc) override;
    const char *getTopic() const override;

private:
    void turnOnLED();
    void turnOffLED();
    void setBrightness(int brightness);
};
