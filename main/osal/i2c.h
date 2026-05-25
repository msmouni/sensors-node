#ifndef ESP_OSAL_I2C_H
#define ESP_OSAL_I2C_H

#include "driver/i2c_master.h"
#include "osal_i2c.h"

typedef struct
{
    i2c_master_dev_handle_t dev;
    i2c_osal_t osal;
} esp_i2c_t;

int esp_i2c_write(
    void *ctx,
    uint8_t addr,
    const uint8_t *data,
    size_t len);

int esp_i2c_read(
    void *ctx,
    uint8_t addr,
    uint8_t *data,
    size_t len);

int esp_i2c_write_read(
    void *ctx,
    uint8_t addr,
    uint8_t *tx_data,
    size_t tx_len,
    uint8_t *rx_data,
    size_t rx_len);

void esp_i2c_delay_ms(
    void *ctx,
    uint32_t ms);

#endif /* ESP_OSAL_I2C_H */
