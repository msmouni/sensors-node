#ifndef ESP_SENSORS_H
#define ESP_SENSORS_H

#include "driver/i2c_master.h"

void sensors_init(i2c_master_bus_handle_t bus);
void sensor_task(void *arg);

#endif /* ESP_SENSORS_H */
