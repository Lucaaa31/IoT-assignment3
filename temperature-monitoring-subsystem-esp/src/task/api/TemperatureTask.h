#ifndef TEMPERATURETASK_H
#define TEMPERATURETASK_H

#include "./data/api/SharedState.h"
#include "./config/config.h"

#include <Arduino.h>
#include <DHT.h>

class TemperatureTask {
public:
    TemperatureTask(int dhtPin, 
                    SharedState& state, 
                    SemaphoreHandle_t& sharedStateMutex);
    void update();

private:
    TaskState state;
    DHT dht;
    SharedState& sharedState;
    SemaphoreHandle_t& sharedStateMutex;
};

#endif
