#pragma once

#include <Arduino.h>
#include "TopicHandler.h"
#include "ConfigLoader.h"
#include "TopicHandlerRegistry.h"

class BuzzerController : public TopicHandler
{
public:
    void init() override;
    void handleMessage(DynamicJsonDocument &doc) override;
    const char *getTopic() const override;

private:
    void _turnOnBuzzer();
    void _turnOffBuzzer();
    void _beep(int duration);
};
