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
    // 基础控制方法
    void _turnOn();
    void _turnOff();
    void _beep(int duration);

    // 高级非阻塞播放
    void _playSong(JsonArray score, int tempo = 120);
    static void _songTask(void* param);

    // 播放状态
    bool _isPlaying = false;
    JsonArray _currentScore;
    int _currentNoteIndex = 0;
    int _tempo = 120;
};
