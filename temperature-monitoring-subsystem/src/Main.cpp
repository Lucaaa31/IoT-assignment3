#include <Arduino.h>
#include "SharedState.h"
#include "LedTask.h"
#include "TemperatureTask.h"
#include "MqttTask.h"

SharedState sharedState;

SemaphoreHandle_t sharedStateMutex;

TaskHandle_t LedTaskHandle;
TaskHandle_t TemperatureTaskHandle;
TaskHandle_t MqttTaskHandle;

void LedTaskFunction(void* parameter);
void TemperatureTaskFunction(void* parameter);
void MqttTaskFunction(void* parameter);


void LedTaskFunction(void* parameter) {
    LedTask ledTask(LED_GREEN_PIN, LED_RED_PIN, sharedState, sharedStateMutex);
    for (;;) {
        ledTask.update();
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}

void TemperatureTaskFunction(void* parameter) {
    TemperatureTask temperatureTask(DHT_PIN, sharedState, sharedStateMutex);
    for (;;) {
        temperatureTask.update();
        vTaskDelay(sharedState.getFrequency() / portTICK_PERIOD_MS);
    }
}

void MqttTaskFunction(void* parameter) {
    MqttTask mqttTask(MQTT_SERVER, MQTT_PORT, MQTT_CLIENT_ID, sharedState, sharedStateMutex);
    for (;;) {
        mqttTask.update();
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

void setup() {
    Serial.begin(115200);
    Serial.println("start");


    sharedStateMutex = xSemaphoreCreateMutex();

    xTaskCreatePinnedToCore(LedTaskFunction, "LedTask", 10000, NULL, 1, &LedTaskHandle, 1); 
    xTaskCreatePinnedToCore(TemperatureTaskFunction, "TemperatureTask", 10000, NULL, 1, &TemperatureTaskHandle, 1);
    xTaskCreatePinnedToCore(MqttTaskFunction, "MqttTask", 10000, NULL, 1, &MqttTaskHandle, 0);
}

void loop() {
}
