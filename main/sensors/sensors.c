#include "sensors.h"
#include "bmp280.h"
#include "data.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "htu21d.h"
#include "osal/i2c.h"

static const char *TAG = "sensor_app";

static htu21d_t htu;
static esp_i2c_t htu_i2c;

static bmp280_t bmp;
static esp_i2c_t bmp_i2c;

static sensor_data_t g_sensor_data;

int sensor_htu_init(i2c_master_bus_handle_t bus)
{
    i2c_device_config_t htu_cfg = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = HTU21D_I2C_ADDR,
        .scl_speed_hz = 100000,
    };

    ESP_ERROR_CHECK(i2c_master_bus_add_device(bus, &htu_cfg, &htu_i2c.dev));

    htu_i2c.osal.ctx = &htu_i2c;
    htu_i2c.osal.write = esp_i2c_write;
    htu_i2c.osal.read = esp_i2c_read;
    htu_i2c.osal.write_read = esp_i2c_write_read;
    htu_i2c.osal.delay_ms = esp_i2c_delay_ms;

    return htu21d_init(&htu, &htu_i2c.osal);
}

static int sensor_bmp_init(i2c_master_bus_handle_t bus)
{
    i2c_device_config_t bmp_cfg = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = BMP280_I2C_ADDR,
        .scl_speed_hz = 100000,
    };

    ESP_ERROR_CHECK(i2c_master_bus_add_device(bus, &bmp_cfg, &bmp_i2c.dev));

    bmp_i2c.osal.ctx = &bmp_i2c;
    bmp_i2c.osal.write = esp_i2c_write;
    bmp_i2c.osal.read = esp_i2c_read;
    bmp_i2c.osal.write_read = esp_i2c_write_read;
    bmp_i2c.osal.delay_ms = esp_i2c_delay_ms;

    return bmp280_init(&bmp, &bmp_i2c.osal);
}

void sensors_init(i2c_master_bus_handle_t bus)
{
    // HTU21D
    if (sensor_htu_init(bus))
        ESP_LOGE(TAG, "Failed to initialize HTU21D sensor");

    // BMP280
    if (sensor_bmp_init(bus))
        ESP_LOGE(TAG, "Failed to initialize BMP280 sensor");
}

static void sensor_app_update(void)
{
    g_sensor_data.htu_temperature = htu21d_read_temperature_no_hold(&htu);
    g_sensor_data.htu_humidity = htu21d_read_humidity_no_hold(&htu);

    bmp280_get_measurement(&bmp, &g_sensor_data.bmp_temperature, &g_sensor_data.bmp_pressure);

    sensors_data_set(&g_sensor_data);

    ESP_LOGD(TAG, "HTU21D: T=%.2fC H=%.2f%%", g_sensor_data.htu_temperature.value,
             g_sensor_data.htu_humidity.value);

    ESP_LOGD(TAG, "BMP280: T=%.2fC P=%.2fhPa\n", g_sensor_data.bmp_temperature,
             g_sensor_data.bmp_pressure);
}

void sensor_task(void *arg)
{
    while (1) {
        sensor_app_update();

        vTaskDelay(pdMS_TO_TICKS(CONFIG_SENSOR_READ_PERIOD_MS));
    }
}