#include "../api/TemperatureTask.h"

TemperatureTask::TemperatureTask(int dhtPin, 
                                SharedState& state, 
                                SemaphoreHandle_t& sharedStateMutex)
    : state(NOT_CONNECT),
      dht(dhtPin, DHTTYPE),
      sharedState(state),
      sharedStateMutex(sharedStateMutex) {
    dht.begin(); 
}

void TemperatureTask::update() {
    unsigned long currentTime = millis();
    int period = 0;

    while (!xSemaphoreTake(sharedStateMutex, pdMS_TO_TICKS(5000))) {
        vTaskDelay(pdMS_TO_TICKS(100));
    }

    if (sharedState.isMqttNetworkConnected()) {
        state = CONNECT;
    } else {
        state = NOT_CONNECT;
    }

    xSemaphoreGive(sharedStateMutex);

    switch (state) {
        case CONNECT:
            while (!xSemaphoreTake(sharedStateMutex, pdMS_TO_TICKS(5000))) {
                vTaskDelay(pdMS_TO_TICKS(100));
            }
            period = ONE_SECOND / sharedState.getFrequency();
            if (currentTime - sharedState.getLastReadTime() >= period) {
                float temperature = dht.readTemperature();
                if (!isnan(temperature)) {
                    sharedState.setTemperature((int)temperature);
                    currentTime = millis();
                    sharedState.setLastReadTime((unsigned long)currentTime);
                }
            }
            xSemaphoreGive(sharedStateMutex);
            break;
        default:
            break;
    }
}