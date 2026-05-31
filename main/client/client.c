#include "data.h"
#include "esp_log.h"
#include "mqtt.h"
#include <arpa/inet.h>
#include <sys/socket.h>

static const char *TAG = "sensors client";

void tcp_client_task(void *arg)
{
    while (1) {
        ESP_LOGI(TAG, "Connecting to TCP server...");

        int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);

        if (sock < 0) {
            vTaskDelay(pdMS_TO_TICKS(5000));
            continue;
        }

        struct sockaddr_in dest = {
            .sin_addr.s_addr = inet_addr(CONFIG_HOST_IP_ADDR),
            .sin_family = AF_INET,
            .sin_port = htons(CONFIG_HOST_PORT),
        };

        if (connect(sock, (struct sockaddr *)&dest, sizeof(dest)) != 0) {
            ESP_LOGE(TAG, "Failed to connect to server");

            close(sock);
            vTaskDelay(pdMS_TO_TICKS(5000));
            continue;
        }

        ESP_LOGI(TAG, "Connected to server");

        while (1) {
            sensor_data_t sensor_data;
            sensors_data_get(&sensor_data);

            char msg[128];

            snprintf(msg, sizeof(msg),
                     "htu_temp=%.2f htu_hum=%.2f bmp_temp=%.2f bmp_pressure=%.2f\n",
                     sensor_data.htu_temperature.value, sensor_data.htu_humidity.value,
                     sensor_data.bmp_temperature, sensor_data.bmp_pressure);

            int ret = send(sock, msg, strlen(msg), 0);

            if (ret < 0) {
                ESP_LOGE(TAG, "Failed to send message");
                break;
            }

            vTaskDelay(pdMS_TO_TICKS(CONFIG_SENSOR_PUBLISH_PERIOD_MS));
        }
    }
}

void mqtt_client_task(void *arg)
{
    if (mqtt_app_start() != 0) {
        while (1) {
            ESP_LOGE(TAG, "Failed to start MQTT client");
            vTaskDelay(pdMS_TO_TICKS(5000));
        }
    }

    while (1) {
        sensor_data_t sensor_data;
        sensors_data_get(&sensor_data);

        mqtt_publish_sensor(sensor_data.htu_temperature.value, sensor_data.htu_humidity.value,
                            sensor_data.bmp_temperature, sensor_data.bmp_pressure);

        vTaskDelay(pdMS_TO_TICKS(CONFIG_SENSOR_PUBLISH_PERIOD_MS));
    }
}
