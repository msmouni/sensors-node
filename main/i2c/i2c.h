#ifndef SENSORS_I2C_H
#define SENSORS_I2C_H

#include "driver/i2c_master.h"

void i2c_master_init(i2c_master_bus_handle_t *bus_handle);
void i2c_master_scan(i2c_master_bus_handle_t bus);

#endif /* SENSORS_I2C_H */
