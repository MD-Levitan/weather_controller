#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include "esp_wifi.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "tcpip_adapter.h"

#include "settings.h"
#include "common.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"

#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"

#include "esp_log.h"

#include "mqtt_publisher.h"
#include "jobs.h"

#include "device_complex.h"
#include "device_simulator.h"

#include "ota.h"

static void mqtt_app_start(void)
{
    mqtt_publisher_t *mqtt;
    arg_t *arg;

    mqtt = create_mqtt_publisher_authenticated(CONFIG_MQTT_BROKER_IP, CONFIG_MQTT_BROKER_PORT, CONFIG_PUBLISHER_USERNAME, CONFIG_PUBLISHER_PASSWORD);
    arg = malloc(sizeof(arg_t));
    arg->publisher = mqtt;
    arg->mutex = xSemaphoreCreateMutex();
    arg->device = create_device_simulator();

    for (uint8_t job = 0; job < sizeof(jobs) / sizeof(jobs[0]); ++job)
    {
        ESP_LOGI(MQTT_TAG, "Create task");
        xTaskCreate(jobs[job], "", 2048, arg, 4, NULL);
    }
}

void app_main()
{
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    ESP_LOGI(MQTT_TAG, "[APP] Startup..");
    ESP_LOGI(MQTT_TAG, "[APP] Free memory: %d bytes", esp_get_free_heap_size());
    ESP_LOGI(MQTT_TAG, "[APP] IDF version: %s", esp_get_idf_version());

    esp_log_level_set("*", ESP_LOG_INFO);
    esp_log_level_set(MQTT_TAG, ESP_LOG_VERBOSE);
    esp_log_level_set(JOBS_TAG, ESP_LOG_VERBOSE);
    esp_log_level_set(JOBS_INFO_TAG, ESP_LOG_DEBUG);

    tcpip_adapter_init();
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    ESP_ERROR_CHECK(example_connect());

    start_ota_updater();
    //mqtt_app_start();
}