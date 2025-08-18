#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>
#include <memory>
#include <vector>
#include "TopicHandler.h"

class TopicHandlerRegistry
{
public:
    // Registers a handler to the registry, allowing it to be used later for topic matching
    static void registerHandler(std::unique_ptr<TopicHandler> handler);

    // Retrieves the handler associated with a given topic
    // If no handler is found for the topic, it returns nullptr
    static TopicHandler *getHandlerForTopic(const char *topic);

    // Returns all the registered handlers
    static std::vector<TopicHandler *> getAllHandlers();

    // Initializes all registered handlers
    // Typically called during startup to set up the handlers
    static void initAllHandlers();

private:
    // List of all registered topic handlers
    static std::vector<std::unique_ptr<TopicHandler>> handlers;
};

// Automatically registers a handler class using this macro
// The macro creates an instance of the handler and registers it with the TopicHandlerRegistry
#define AUTO_REGISTER_HANDLER(cls)                         \
    struct cls##AutoRegister                               \
    {                                                      \
        cls##AutoRegister()                                \
        {                                                  \
            TopicHandlerRegistry::registerHandler(         \
                std::unique_ptr<TopicHandler>(new cls())); \
        }                                                  \
    };                                                     \
    static cls##AutoRegister global_##cls##AutoRegister;
