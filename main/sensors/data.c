#include "data.h"

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

static sensor_data_t g_sensor_data;

static SemaphoreHandle_t sensor_mutex;

void sensors_data_init(void) { sensor_mutex = xSemaphoreCreateMutex(); }

void sensors_data_set(sensor_data_t *new_data)
{
    xSemaphoreTake(sensor_mutex, portMAX_DELAY);

    g_sensor_data = *new_data;

    xSemaphoreGive(sensor_mutex);
}

void sensors_data_get(sensor_data_t *out)
{
    xSemaphoreTake(sensor_mutex, portMAX_DELAY);

    *out = g_sensor_data;

    xSemaphoreGive(sensor_mutex);
}