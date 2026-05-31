#include "mqtt.h"

#include "esp_event.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "esp_system.h"
#include "mqtt_client.h"
#include "sdkconfig.h"

static const char *TAG = "mqtt";

extern const uint8_t ca_cert_start[] asm("_binary_ca_crt_start");
extern const uint8_t ca_cert_end[] asm("_binary_ca_crt_end");

static esp_mqtt_client_handle_t client;
static bool mqtt_connected = false;

static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id,
                               void *event_data)
{
    ESP_LOGD(TAG, "Event dispatched from event loop base=%s, event_id=%" PRIi32, base, event_id);
    esp_mqtt_event_handle_t event = event_data;

    switch ((esp_mqtt_event_id_t)event_id) {
    case MQTT_EVENT_CONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
        mqtt_connected = true;
        break;
    case MQTT_EVENT_DISCONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
        mqtt_connected = false;
        break;
    case MQTT_EVENT_PUBLISHED:
        ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_ERROR:
        ESP_LOGE(TAG, "MQTT_EVENT_ERROR");
        mqtt_connected = false;

        if (event->error_handle->error_type == MQTT_ERROR_TYPE_TCP_TRANSPORT) {
            ESP_LOGE(TAG, "Last error code reported from esp-tls: 0x%x",
                     event->error_handle->esp_tls_last_esp_err);
            ESP_LOGE(TAG, "Last tls stack error number: 0x%x",
                     event->error_handle->esp_tls_stack_err);
            ESP_LOGE(TAG, "Last captured errno : %d (%s)",
                     event->error_handle->esp_transport_sock_errno,
                     strerror(event->error_handle->esp_transport_sock_errno));
        } else if (event->error_handle->error_type == MQTT_ERROR_TYPE_CONNECTION_REFUSED) {
            ESP_LOGE(TAG, "Connection refused error: 0x%x",
                     event->error_handle->connect_return_code);
        } else {
            ESP_LOGE(TAG, "Unknown error type: 0x%x", event->error_handle->error_type);
        }
        break;
    default:
        ESP_LOGI(TAG, "Other event id:%d", event->event_id);
        break;
    }
}

int mqtt_app_start(void)
{
    esp_mqtt_client_config_t cfg = {
        .broker.address.uri = "mqtts://192.168.1.200:8883",
        .broker.verification.certificate = (const char *)ca_cert_start,
    };

    ESP_LOGI(TAG, "[APP] Free memory: %" PRIu32 " bytes", esp_get_free_heap_size());
    client = esp_mqtt_client_init(&cfg);

    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);

    esp_mqtt_client_start(client);

    /* Wait for connection */
    int retries = 20;
    while (!mqtt_connected && retries-- > 0) {
        vTaskDelay(pdMS_TO_TICKS(500));
    }

    if (!mqtt_connected) {
        ESP_LOGW(TAG, "MQTT broker connection timeout");
        return -1;
    }

    return 0;
}

int mqtt_publish_sensor(float htu_t, float htu_h, float bmp_t, float bmp_p)
{
    if (!mqtt_connected || client == NULL)
        return -1;

    char payload[128];

    snprintf(payload, sizeof(payload),
             "{\"htu_temp\":%.2f,\"htu_hum\":%.2f,"
             "\"bmp_temp\":%.2f,\"bmp_press\":%.2f}",
             htu_t, htu_h, bmp_t, bmp_p);

    /* (topic, payload, len, qos, retain) */
    int msg_id = esp_mqtt_client_publish(client, "sensors/room1", payload, 0, 1, 1);

    if (msg_id < 0) {
        ESP_LOGW(TAG, "Publish failed");
        return -1;
    }

    return msg_id;
}
