#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

#include "esp_system.h"
#include "esp_event.h"
#include "esp_event_loop.h"
#include "esp_log.h"
#include "esp_https_ota.h"

#include "ota.h"
#include "settings.h"

static const char *TAG = "OTA";

#define OTA_URL_SIZE 256

static esp_err_t do_firmware_upgrade()
{
    esp_http_client_config_t config = {
        .host = CONFIG_OTA_BROKER_IP,
        .port =  CONFIG_OTA_BROKER_PORT,
        .path = "/api/v1/latest_firmware"
    };

    esp_err_t ret = esp_https_ota(&config);
    if (ret == ESP_OK) {
        ESP_LOGI(TAG, "OTA inited successfuly");
        esp_restart();
    } else {
        ESP_LOGI(TAG, "OTA init failed");
        return ESP_FAIL;
    }
    return ESP_OK;
}

void start_ota_updater()
{
    do_firmware_upgrade();
    
}
