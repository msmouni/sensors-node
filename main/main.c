#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2c_master.h"
#include "esp_log.h"
#include "htu21d.h"
#include "bmp280.h"
#include "osal_i2c.h"
#include "sensors.h"
#include "i2c/i2c.h"

static const char *TAG = "sensors-node";

void app_main(void)
{
    ESP_LOGI(TAG, "Starting sensors node...");

    i2c_master_bus_handle_t bus_handle;

    i2c_master_init(&bus_handle);
    i2c_master_scan(bus_handle);

    sensor_app_init(bus_handle);

    while (1)
    {
        sensor_app_update();

        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}
