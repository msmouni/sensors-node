#ifndef SENSORS_DATA_H
#define SENSORS_DATA_H

#include <htu21d.h>
#include <stdbool.h>

typedef struct {
    htu21d_measurement_t htu_temperature;
    htu21d_measurement_t htu_humidity;

    float bmp_temperature;
    float bmp_pressure;
} sensor_data_t;

void sensors_data_init(void);

void sensors_data_set(sensor_data_t *new_data);

void sensors_data_get(sensor_data_t *out);

#endif /* SENSORS_DATA_H */