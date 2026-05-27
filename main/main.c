#include "bmp280.h"
#include "client.h"
#include "data.h"
#include "driver/i2c_master.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "htu21d.h"
#include "i2c/i2c.h"
#include "nvs_flash.h"
#include "osal_i2c.h"
#include "sensors.h"
#include "wifi.h"
#include <stdio.h>

static const char *TAG = "sensors-node";

void app_main(void)
{
    ESP_LOGI(TAG, "Starting sensors node...");

    // Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    i2c_master_bus_handle_t bus_handle;
    i2c_master_init(&bus_handle);
    i2c_master_scan(bus_handle);

    sensors_data_init();

    sensors_init(bus_handle);

    wifi_init_sta();

    xTaskCreate(sensor_task, "sensor_task", 4096, NULL, 5, NULL);

    xTaskCreate(tcp_client_task, "tcp_task", 4096, NULL, 5, NULL);
}
