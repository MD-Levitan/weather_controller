#ifndef MQTT_publisher_H_
#define MQTT_publisher_H_

#include <stdint.h>
#include <inttypes.h>

#include "macros.h"
#include "common.h"

typedef struct mqtt_publisher_t mqtt_publisher_t;
typedef enum status_t status_t;
typedef enum topic_t topic_t;

static const char *MQTT_TAG = "MQTT";

enum status_t
{
    STATUS_OK = 0x01,
    STATUS_ERROR = 0x02,
};

enum topic_t
{
    /* User Data Topics */
    TOPIC_USER_TEMPERATURE = 0x01,
    TOPIC_USER_HUMIDITY = 0x02,
    TOPIC_USER_PRESSURE = 0x03,
    TOPIC_USER_INFO = 0x04,
    TOPIC_USER_STATUS = 0x05,

    /* Admin Data Topics*/
    TOPIC_ADMIN_BATTERY = 0x11,
    TOPIC_ADMIN_WORK_TIME = 0x12,
    TOPIC_ADMIN_MODEL = 0x13,
    TOPIC_ADMIN_INFO = 0x14,
    TOPIC_ADMIN_STATUS = 0x15,
};

struct mqtt_publisher_t
{
    /**
     * @brief  Send payload with topic to broker
     * 
     * @param payload       string with payload
     * @param topic         topic of payload
     * 
     * @return              status of sending
     */
    status_t (*publish_string)(mqtt_publisher_t *publisher, const char *payload, topic_t topic);

    /**
     * @brief Destroy MQTT publisher
     * 
     */
    void (*destroy)(mqtt_publisher_t *publisher);
};

mqtt_publisher_t *create_mqtt_publisher_anonymous(const char *host, uint16_t port);

mqtt_publisher_t *create_mqtt_publisher_authenticated(const char *host, uint16_t port, const char *username, const char *password);

#endif //MQTT_publisher_H_