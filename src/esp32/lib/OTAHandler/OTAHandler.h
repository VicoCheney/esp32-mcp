#ifndef OTA_HANDLER_H
#define OTA_HANDLER_H

#include <Arduino.h>
#include <ArduinoOTA.h>

class OTAHandler
{
public:
    /**
     * @brief Initializes the OTA (Over-The-Air) update service.
     * Sets up the hostname, event handlers, and starts the OTA listener.
     */
    static void init();

    /**
     * @brief Handles incoming OTA requests.
     * This should be called in the main loop to process update events.
     */
    static void handle();
};

#endif // OTA_HANDLER_H
