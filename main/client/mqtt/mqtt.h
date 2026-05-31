#ifndef SENORS_MQTT_H
#define SENORS_MQTT_H

int mqtt_app_start(void);

int mqtt_publish_sensor(float htu_t, float htu_h, float bmp_t, float bmp_p);

#endif /* SENORS_MQTT_H */
