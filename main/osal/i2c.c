#include "i2c.h"
#include "driver/i2c_master.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define ESP_I2C_TIMEOUT_MS 1000

int esp_i2c_write(
    void *ctx,
    uint8_t addr,
    const uint8_t *data,
    size_t len)
{
    if (!ctx || !data || len == 0)
        return -1;

    esp_i2c_t *esp_i2c = (esp_i2c_t *)ctx;

    if (esp_i2c->dev == NULL)
        return -1;

    esp_err_t err = i2c_master_transmit(esp_i2c->dev, data, len, ESP_I2C_TIMEOUT_MS);
    if (err != ESP_OK)
        return -1;

    return 0;
}

int esp_i2c_read(
    void *ctx,
    uint8_t addr,
    uint8_t *data,
    size_t len)

{
    if (!ctx || !data || len == 0)
        return -1;

    esp_i2c_t *esp_i2c = (esp_i2c_t *)ctx;

    if (esp_i2c->dev == NULL)
        return -1;

    esp_err_t err = i2c_master_receive(esp_i2c->dev, data, len, ESP_I2C_TIMEOUT_MS);
    if (err != ESP_OK)
        return -1;

    return 0;
}

int esp_i2c_write_read(
    void *ctx,
    uint8_t addr,
    uint8_t *tx_data,
    size_t tx_len,
    uint8_t *rx_data,
    size_t rx_len)
{
    if (!ctx || (!tx_data && !rx_data) || (tx_len == 0 && rx_len == 0))
        return -1;

    esp_i2c_t *esp_i2c = (esp_i2c_t *)ctx;

    if (esp_i2c->dev == NULL)
        return -1;

    esp_err_t err = i2c_master_transmit_receive(esp_i2c->dev, tx_data, tx_len, rx_data, rx_len, ESP_I2C_TIMEOUT_MS);
    if (err != ESP_OK)
        return -1;

    return 0;
}

void esp_i2c_delay_ms(
    void *ctx,
    uint32_t ms)
{
    vTaskDelay(pdMS_TO_TICKS(ms));
}
