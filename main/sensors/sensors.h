#ifndef ESP_SENSORS_H
#define ESP_SENSORS_H

#include "driver/i2c_master.h"

void sensor_app_init(i2c_master_bus_handle_t bus);
void sensor_app_update(void);

#endif /* ESP_SENSORS_H */
