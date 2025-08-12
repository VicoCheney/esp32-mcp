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
    _isPlaying = false;
}

// handleMessage
void BuzzerController::handleMessage(DynamicJsonDocument &doc)
{
    const char *command = doc["command"];

    if (strcmp(command, "ON") == 0)
    {
        _turnOn();
    }
    else if (strcmp(command, "OFF") == 0)
    {
        _turnOff();
    }
    else if (strcmp(command, "BEEP") == 0)
    {
        int duration = doc["duration"] | 100; // Default 100ms
        _beep(duration);
    }
    else if (strcmp(command, "PLAY_SONG") == 0)
    {
        JsonArray score = doc["score"].as<JsonArray>();
        int tempo = doc["tempo"] | 120;
        if (!score.isNull())
            _playSong(score, tempo);
    }
}

// --- buzzer control methods ---

void BuzzerController::_turnOn()
{
    Serial.println("Buzzer ON");
    tone(ConfigLoader::getConfig().buzzer_pin, 1000); // Play a 1kHz tone
}

void BuzzerController::_turnOff()
{
    Serial.println("Buzzer OFF");
    noTone(ConfigLoader::getConfig().buzzer_pin);
}

void BuzzerController::_beep(int duration)
{
    Serial.printf("Buzzer BEEP for %dms\n", duration);
    tone(ConfigLoader::getConfig().buzzer_pin, 1000, duration); // Beep for specified duration
}

/**
 * @brief 根据提供的 JsonArray 曲谱播放一首歌曲。
 *
 * 此函数会遍历定义在 JSON 数组中的一系列音符。
 * 每个音符都是一个对象，指定了其频率和持续时间。
 * 这是一个阻塞式函数，在歌曲播放完毕前会暂停程序的其他部分。
 *
 * @param score 一个包含歌曲音符的 JsonArray。
 *
 * MQTT 消息应采用以下 JSON 格式：
 * {
 *   "command": "PLAY_SONG",
 *   "score": [
 *     { "freq": 262, "duration": 250 },  // C4 音符，持续 250ms
 *     { "freq": 294, "duration": 250 },  // D4 音符，持续 250ms
 *     { "freq": 330, "duration": 500 },  // E4 音符，持续 500ms
 *     { "freq": 0,   "duration": 100 },  // 一个 100ms 的休止符
 *     { "freq": 349, "duration": 500 }   // F4 音符，持续 500ms
 *   ]
 * }
 *
 * - "freq": 音符的频率，单位是赫兹 (Hz)。频率为 0 代表休止符（静音）。
 * - "duration": 音符的持续时间，单位是毫秒 (ms)。
 */
// ------------------- 高级非阻塞播放 -------------------
void BuzzerController::_playSong(JsonArray score, int tempo) {
    if (_isPlaying) {
        _isPlaying = false;  // 停止正在播放的歌曲
        delay(50);           // 给任务一点时间退出
    }

    _currentScore = score;
    _currentNoteIndex = 0;
    _tempo = tempo;
    _isPlaying = true;

    // 创建 FreeRTOS 任务
    xTaskCreate(_songTask, "SongTask", 4096, this, 1, NULL);
}

void BuzzerController::_songTask(void* param) {
    BuzzerController* self = (BuzzerController*)param;

    while(self->_isPlaying && self->_currentNoteIndex < self->_currentScore.size()) {
        JsonObject note = self->_currentScore[self->_currentNoteIndex].as<JsonObject>();

        int freq_start = note["freq_start"] | note["freq"] | 0;
        int freq_end = note["freq_end"] | freq_start;
        float beats = note["beats"] | 1.0;
        int amp = note["amp"] | 200;      // 默认 PWM 200/255
        bool tie = note["tie"] | false;

        int duration = (60000 / self->_tempo) * beats; // 毫秒计算

        if(freq_start > 0) {
            if(freq_start != freq_end) {
                // 滑音处理
                uint32_t startMillis = millis();
                while(millis() - startMillis < (uint32_t)duration) {
                    float t = float(millis() - startMillis) / duration;
                    int currentFreq = freq_start + t * (freq_end - freq_start);
                    ledcWriteTone(0, currentFreq); // LEDC 通道0
                    vTaskDelay(1);
                }
            } else {
                ledcWriteTone(0, freq_start);
                uint32_t startMillis = millis();
                while(millis() - startMillis < (uint32_t)duration) vTaskDelay(1);
            }
        } else {
            noTone(ConfigLoader::getConfig().buzzer_pin);
            vTaskDelay(duration / portTICK_PERIOD_MS);
        }

        // 如果没有 tie，短暂停顿
        if(!tie) vTaskDelay(20 / portTICK_PERIOD_MS);

        self->_currentNoteIndex++;
    }

    // 歌曲结束
    self->_turnOff();
    self->_isPlaying = false;
    vTaskDelete(NULL);
}

// Automatically register the BuzzerController
AUTO_REGISTER_HANDLER(BuzzerController);