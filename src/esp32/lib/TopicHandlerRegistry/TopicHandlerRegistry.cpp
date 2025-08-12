#include "TopicHandlerRegistry.h"

std::vector<std::unique_ptr<TopicHandler>> TopicHandlerRegistry::handlers;

void TopicHandlerRegistry::registerHandler(std::unique_ptr<TopicHandler> handler) {
    handlers.push_back(std::move(handler));
}

TopicHandler* TopicHandlerRegistry::getHandlerForTopic(const char* topic) {
    for (auto& handler : handlers) {
        if (topic == handler->getTopic()) {
            return handler.get();
        }
    }
    return nullptr;
}

std::vector<TopicHandler*> TopicHandlerRegistry::getAllHandlers() {
    std::vector<TopicHandler*> rawHandlers;
    for (auto& h : handlers) {
        rawHandlers.push_back(h.get());
    }
    return rawHandlers;
}

void TopicHandlerRegistry::initAllHandlers() {
    for (auto& h : handlers) {
        Serial.printf("[Init] handler：%s\n", h->getTopic());
        h->init();
    }
}