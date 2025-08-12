#include "OTAHandler.h"

void OTAHandler::init()
{
    // Set a hostname for easier identification on the network
    ArduinoOTA.setHostname("esp32");

    // Set up OTA event handlers for progress and error feedback
    ArduinoOTA
        .onStart([]()
                 {
            String type;
            if (ArduinoOTA.getCommand() == U_FLASH) {
                type = "sketch";
            } else { // U_FS
                type = "filesystem";
            }
            Serial.println("Start updating " + type); })
        .onEnd([]()
               { Serial.println("\nEnd"); })
        .onProgress([](unsigned int progress, unsigned int total)
                    { Serial.printf("Progress: %u%%\r", (progress / (total / 100))); })
        .onError([](ota_error_t error)
                 {
            Serial.printf("Error[%u]: ", error);
            if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
            else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
            else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
            else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
            else if (error == OTA_END_ERROR) Serial.println("End Failed"); });

    ArduinoOTA.begin();
    Serial.println("✅ OTA Ready");
}

void OTAHandler::handle()
{
    ArduinoOTA.handle();
}
