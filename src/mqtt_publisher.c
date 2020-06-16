#include <string.h>
#include <stdio.h>
#include <stdlib.h>
//#include <syslog.h>

#include "mqtt_client.h"

#include "mqtt_publisher.h"

typedef struct private_mqtt_publisher_t private_mqtt_publisher_t;

struct private_mqtt_publisher_t
{
    /* Public publisher Interface */
    mqtt_publisher_t public;

    /* MQTT Config */
    esp_mqtt_client_config_t mqtt_cfg;

    /* MQTT Client Handle */
    esp_mqtt_client_handle_t client;
};

#define MAX_TOPIC_LEN 128

static char *api_string = "/api/v1/";

static char *topics[] = {"", "user/temperature", "user/humidity", "user/pressure", "user/info", "user/status", "user/status", "", "", "", "", "", "", "", "", "",
                         "", "admin/battery", "admin/work_time", "admin/model", "admin/info"};

static char *topic_to_string(topic_t topic)
{
    const char *topic_str = topics[topic];
    char str[MAX_TOPIC_LEN];
    snprintf(str, 100, "%s%s\n", api_string, topic_str);

    uint8_t str_len = strlen(str);
    char *res = malloc(str_len + 1);
    snprintf(res, str_len, "%.100s", str);
    res[str_len] = 0x00;
    return res;
}

static esp_err_t mqtt_event_handler_cb(esp_mqtt_event_handle_t event)
{
    esp_mqtt_client_handle_t client = event->client;
    int msg_id;
    
    switch (event->event_id) {
        case MQTT_EVENT_CONNECTED:
            // ESP_LOGI(MQTT_TAG, "MQTT_EVENT_CONNECTED");
            // msg_id = esp_mqtt_client_publish(client, "/topic/qos1", "data_3", 0, 1, 0);
            // ESP_LOGI(MQTT_TAG, "sent publish successful, msg_id=%d", msg_id);

            // msg_id = esp_mqtt_client_subscribe(client, "/topic/qos0", 0);
            // ESP_LOGI(MQTT_TAG, "sent subscribe successful, msg_id=%d", msg_id);

            // msg_id = esp_mqtt_client_subscribe(client, "/topic/qos1", 1);
            // ESP_LOGI(MQTT_TAG, "sent subscribe successful, msg_id=%d", msg_id);

            // msg_id = esp_mqtt_client_unsubscribe(client, "/topic/qos1");
            // ESP_LOGI(MQTT_TAG, "sent unsubscribe successful, msg_id=%d", msg_id);
            break;
        case MQTT_EVENT_DISCONNECTED:
            ESP_LOGI(MQTT_TAG, "MQTT_EVENT_DISCONNECTED");
            break;

        case MQTT_EVENT_SUBSCRIBED:
            ESP_LOGI(MQTT_TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
            msg_id = esp_mqtt_client_publish(client, "/topic/qos0", "data", 0, 0, 0);
            ESP_LOGI(MQTT_TAG, "sent publish successful, msg_id=%d", msg_id);
            break;
        case MQTT_EVENT_UNSUBSCRIBED:
            ESP_LOGI(MQTT_TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_PUBLISHED:
            ESP_LOGI(MQTT_TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_DATA:
            ESP_LOGI(MQTT_TAG, "MQTT_EVENT_DATA");
            printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
            printf("DATA=%.*s\r\n", event->data_len, event->data);
            break;
        case MQTT_EVENT_ERROR:
            ESP_LOGI(MQTT_TAG, "MQTT_EVENT_ERROR");
            break;
        default:
            ESP_LOGI(MQTT_TAG, "Other event id:%d", event->event_id);
            break;
    }
    return ESP_OK;
}

static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data) {
    ESP_LOGD(MQTT_TAG, "Event dispatched from event loop base=%s, event_id=%d", base, event_id);
    mqtt_event_handler_cb(event_data);
}

METHOD(mqtt_publisher_t, publish_string, status_t, private_mqtt_publisher_t *this, char *string, topic_t topic)
{
    char *topic_str;
    int status = 0;

    topic_str = topic_to_string(topic);
    status = esp_mqtt_client_publish(this->client, topic_str, string, strlen(string), 0, false);
    free(topic_str);
    
    if (status != 0)
    {
        ESP_LOGI(MQTT_TAG, "failed to publish message!\n");
        return STATUS_ERROR;
    }
    return STATUS_OK;
}

METHOD(mqtt_publisher_t, destroy, void, private_mqtt_publisher_t *this)
{
    esp_mqtt_client_stop(this->client);
    esp_mqtt_client_destroy(this->client);
    free(this);
}

/**
 * See header
 */
mqtt_publisher_t *create_mqtt_publisher_anonymous(const char *host, uint16_t port)
{
    private_mqtt_publisher_t *this;
    this = malloc(sizeof(private_mqtt_publisher_t));

    this->mqtt_cfg = (esp_mqtt_client_config_t){
        .host = host,
        .port = port,
    };

    this->client = esp_mqtt_client_init(&this->mqtt_cfg);
    if (this->client == NULL)
    {
        ESP_LOGI(MQTT_TAG, "cannot create esp_mqtt object!\n");
        free(this);
        return NULL;
    }

    if (esp_mqtt_client_register_event(this->client, ESP_EVENT_ANY_ID, mqtt_event_handler, this->client) == ESP_ERR_NO_MEM)
    {
        ESP_LOGI(MQTT_TAG, "cannot allocate memmory to register event!\n");
        esp_mqtt_client_destroy(this->client);
        free(this);
        return NULL;
    }

    if (esp_mqtt_client_start(this->client) != ESP_OK)
    {
        ESP_LOGI(MQTT_TAG, "cannot start client!\n");
        esp_mqtt_client_destroy(this->client);
        free(this);
        return NULL;
    }

    //this->public.publish_string = _publish_string;
    this->public.destroy = _destroy;

    return &this->public;
};

/**
 * See header
 */
mqtt_publisher_t *create_mqtt_publisher_authenticated(const char *host, uint16_t port, const char *username, const char *password)
{
    private_mqtt_publisher_t *this;
    this = malloc(sizeof(private_mqtt_publisher_t));

    this->mqtt_cfg = (esp_mqtt_client_config_t){
        .host = host,
        .port = port,

        .username = username,
        .password = password,
    };

    this->client = esp_mqtt_client_init(&this->mqtt_cfg);
    if (this->client == NULL)
    {
        ESP_LOGI(MQTT_TAG, "cannot create esp_mqtt object!\n");
        free(this);
        return NULL;
    }

    if (esp_mqtt_client_register_event(this->client, ESP_EVENT_ANY_ID, mqtt_event_handler, this->client) == ESP_ERR_NO_MEM)
    {
        ESP_LOGI(MQTT_TAG, "cannot allocate memmory to register event!\n");
        esp_mqtt_client_destroy(this->client);
        free(this);
        return NULL;
    }

    if (esp_mqtt_client_start(this->client) != ESP_OK)
    {
        ESP_LOGI(MQTT_TAG, "cannot start client!\n");
        esp_mqtt_client_destroy(this->client);
        free(this);
        return NULL;
    }

    //this->public.publish_string = _publish_string;
    this->public.destroy = _destroy;

    return &this->public;
}