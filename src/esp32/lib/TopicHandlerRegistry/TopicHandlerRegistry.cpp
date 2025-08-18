#include "TopicHandlerRegistry.h"
#include <cstring>

std::vector<std::unique_ptr<TopicHandler>> TopicHandlerRegistry::handlers;

void TopicHandlerRegistry::registerHandler(std::unique_ptr<TopicHandler> handler)
{
    handlers.push_back(std::move(handler));
}

TopicHandler *TopicHandlerRegistry::getHandlerForTopic(const char *topic)
{
    if (!topic)
    {
        return nullptr;
    }

    for (auto &handler : handlers)
    {
        if (strcmp(topic, handler->getTopic()) == 0)
        {
            return handler.get();
        }
    }
    return nullptr;
}

std::vector<TopicHandler *> TopicHandlerRegistry::getAllHandlers()
{
    std::vector<TopicHandler *> rawHandlers;
    for (auto &h : handlers)
    {
        rawHandlers.push_back(h.get());
    }
    return rawHandlers;
}

void TopicHandlerRegistry::initAllHandlers()
{
    Serial.printf("📊 Number of registered handlers: %d\n", handlers.size());
    for (auto &h : handlers)
    {
        Serial.printf("  - Handler for topic: %s\n", h->getTopic());
        h->init();
    }
}