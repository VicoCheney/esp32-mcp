#ifndef TOPIC_HANDLER_H
#define TOPIC_HANDLER_H

#include <ArduinoJson.h>

class TopicHandler {
public:
    // Virtual destructor to ensure proper cleanup of derived class objects
    virtual ~TopicHandler() {}

    // Virtual init method that can be overridden in derived classes, or can have a default implementation
    virtual void init() {}

    // Pure virtual method to handle incoming messages; each derived class must implement this method
    // The message is passed as a JSON document (DynamicJsonDocument type)
    virtual void handleMessage(DynamicJsonDocument& doc) = 0;

    // Pure virtual method to return the topic this handler processes
    // Each derived class must specify the topic it handles
    virtual const char* getTopic() const = 0;

};

#endif