#include "i2c.h"

#include "esp_log.h"
#include <stdio.h>

static const char *TAG = "sensors-i2c";

#define I2C_MASTER_SCL_IO 9         /*!< GPIO number used for I2C master clock */
#define I2C_MASTER_SDA_IO 8         /*!< GPIO number used for I2C master data */
#define I2C_MASTER_NUM I2C_NUM_0    /*!< I2C port number for master dev */
#define I2C_MASTER_FREQ_HZ 100000   /*!< I2C master clock frequency */
#define I2C_MASTER_TX_BUF_DISABLE 0 /*!< I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_DISABLE 0 /*!< I2C master doesn't need buffer */
#define I2C_MASTER_TIMEOUT_MS 1000

void i2c_master_init(i2c_master_bus_handle_t *bus_handle)
{
    i2c_master_bus_config_t bus_config = {
        .i2c_port = I2C_MASTER_NUM,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup = true,
    };

    ESP_ERROR_CHECK(i2c_new_master_bus(&bus_config, bus_handle));
}

void i2c_master_scan(i2c_master_bus_handle_t bus)
{
    ESP_LOGI(TAG, "Scanning I2C bus...");

    for (uint8_t addr = 1; addr < 127; addr++) {
        i2c_device_config_t cfg = {
            .dev_addr_length = I2C_ADDR_BIT_LEN_7,
            .device_address = addr,
            .scl_speed_hz = 100000,
        };

        i2c_master_dev_handle_t dev;

        if (i2c_master_bus_add_device(bus, &cfg, &dev) == ESP_OK) {
            esp_err_t err = i2c_master_probe(bus, addr, 100);

            if (err == ESP_OK) {
                ESP_LOGI(TAG, "Found device at 0x%02X", addr);
            }

            i2c_master_bus_rm_device(dev);
        }
    }
}
