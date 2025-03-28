#ifndef LEDTASK_H
#define LEDTASK_H

#include "SharedState.h"
#include "config.h"
#include "LedManager.h"

#include <Arduino.h>

class LedTask {
public:
    LedTask(int greenPin, 
            int redPin, 
            SharedState& state, 
            SemaphoreHandle_t& sharedStateMutex);
            
    void update();

private:
    TaskState state;

    LedManager greenLed, redLed;

    SharedState& sharedState;
    SemaphoreHandle_t& sharedStateMutex;

};

#endif
