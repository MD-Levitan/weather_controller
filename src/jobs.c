#include "jobs.h"

#define MAX_STRING 100
static char *message_templates[] = {"", "{\"temperature\":%f}", "{\"humidity\":%d}", "{\"pressure\":%ld}", "{\"info\":\"%s\"}", "{\"status\":%d}", "", "", "", "", "", "", "", "", "", "",
                                    "", "{\"battery\":%f}", "{\"work_time\":%lld}", "{\"version\":%d}", "{\"info\":\"%s\"}"};

void update_temperature(void *arg_void)
{
    arg_t *arg = (arg_t *)(arg_void);
    char *message;
    int status;

    ESP_LOGI(JOBS_TAG, "init temperature job");
    while (true)
    {
        /* Sleep and update temprature */
        vTaskDelay(5000 / portTICK_RATE_MS);

        if (xSemaphoreTake(arg->mutex, (TickType_t)50) != pdTRUE)
        {
            continue;
        }

        // /* Check if device is connected */
        // if (arg->device->get_status(arg->device) != DEVICE_CONNECTED)
        // {
        //     xSemaphoreGive(arg->mutex);
        //     continue;
        // }

        message = malloc(MAX_STRING);
        // arg->device->get_indicators(arg->device, &ind);
        snprintf(message, MAX_STRING - 1, message_templates[TOPIC_USER_TEMPERATURE], 15.2);
        status = arg->publisher->publish_string(arg->publisher, message, TOPIC_USER_TEMPERATURE);
        ESP_LOGI(JOBS_INFO_TAG, "update Temperature status");
        free(message);
        xSemaphoreGive(arg->mutex);
    }
}

void update_humidity(void *arg_void)
{
    arg_t *arg = (arg_t *)(arg_void);
    char *message;
    int status;

    ESP_LOGI(JOBS_TAG, "init temperature job");
    while (true)
    {
        /* Sleep 1 minute and update humidity */
        vTaskDelay(5000 / portTICK_RATE_MS);

        if (xSemaphoreTake(arg->mutex, (TickType_t)50) != pdTRUE)
        {
            continue;
        }

        // /* Check if device is connected */
        // if (arg->device->get_status(arg->device) != DEVICE_CONNECTED)
        // {
        //     xSemaphoreGive(arg->mutex);
        //     continue;
        // }

        message = malloc(MAX_STRING);
        // arg->device->get_indicators(arg->device, &ind);
        snprintf(message, MAX_STRING - 1, message_templates[TOPIC_USER_HUMIDITY], 777);
        status = arg->publisher->publish_string(arg->publisher, message, TOPIC_USER_HUMIDITY);
        ESP_LOGI(JOBS_INFO_TAG, "update Humidity status");
        free(message);
        xSemaphoreGive(arg->mutex);
    }
}

void update_pressure(void *arg_void)
{
    arg_t *arg = (arg_t *)(arg_void);
    char *message;
    int status;

    ESP_LOGI(JOBS_TAG, "init pressure job");
    while (true)
    {
        /* Sleep and update pressure */
        vTaskDelay(5000 / portTICK_RATE_MS);
        if (xSemaphoreTake(arg->mutex, (TickType_t)50) != pdTRUE)
        {
            continue;
        }

        // /* Check if device is connected */
        // if (arg->device->get_status(arg->device) != DEVICE_CONNECTED)
        // {
        //     xSemaphoreGive(arg->mutex);
        //     continue;
        // }

        message = malloc(MAX_STRING);
        //arg->device->get_indicators(arg->device, &ind);
        snprintf(message, MAX_STRING - 1, message_templates[TOPIC_USER_PRESSURE], 0.88);
        status = arg->publisher->publish_string(arg->publisher, message, TOPIC_USER_PRESSURE);
        ESP_LOGI(JOBS_INFO_TAG, "update Pressure status");
        free(message);
        xSemaphoreGive(arg->mutex);
    }
}

void update_status(void *arg_void)
{
    arg_t *arg = (arg_t *)(arg_void);
    char *message;
    int status;

    ESP_LOGI(JOBS_TAG, "init status job");
    while (true)
    {
        /* Sleep and update pressure */
        vTaskDelay(5000 / portTICK_RATE_MS);
        if (xSemaphoreTake(arg->mutex, (TickType_t)50) != pdTRUE)
        {
            continue;
        }

        message = malloc(MAX_STRING);
        snprintf(message, MAX_STRING - 1, message_templates[TOPIC_USER_STATUS], 1); //arg->device->get_status(arg->device));
        status = arg->publisher->publish_string(arg->publisher, message, TOPIC_USER_STATUS);
        ESP_LOGI(JOBS_INFO_TAG, "update device status");
        free(message);
        xSemaphoreGive(arg->mutex);
    }
}

void update_work_time(void *arg_void)
{
    arg_t *arg = (arg_t *)(arg_void);
    char *message;
    int status;

    ESP_LOGI(JOBS_TAG, "init work time job");
    while (true)
    {
        vTaskDelay(5000 / portTICK_RATE_MS);
        if (xSemaphoreTake(arg->mutex, (TickType_t)50) != pdTRUE)
        {
            continue;
        }

        // /* Check if device is connected */
        // if (arg->device->get_status(arg->device) != DEVICE_CONNECTED)
        // {
        //     xSemaphoreGive(arg->mutex);
        //     continue;
        // }

        message = malloc(MAX_STRING);
        snprintf(message, MAX_STRING - 1, message_templates[TOPIC_ADMIN_WORK_TIME], 100); //arg->device->get_work_time(arg->device));
        status = arg->publisher->publish_string(arg->publisher, message, TOPIC_ADMIN_WORK_TIME);
        ESP_LOGI(JOBS_INFO_TAG, "update work time");
        free(message);
        xSemaphoreGive(arg->mutex);
    }
}

void update_battery(void *arg_void)
{
    arg_t *arg = (arg_t *)(arg_void);
    char *message;
    int status;

    ESP_LOGI(JOBS_TAG, "init battery job");
    while (true)
    {
        vTaskDelay(5000 / portTICK_RATE_MS);
        if (xSemaphoreTake(arg->mutex, (TickType_t)50) != pdTRUE)
        {
            continue;
        }

        // /* Check if device is connected */
        // if (arg->device->get_status(arg->device) != DEVICE_CONNECTED)
        // {
        //     xSemaphoreGive(arg->mutex);
        //     continue;
        // }

        message = malloc(MAX_STRING);
        snprintf(message, MAX_STRING - 1, message_templates[TOPIC_ADMIN_BATTERY], 0.90); //arg->device->get_battery(arg->device));
        status = arg->publisher->publish_string(arg->publisher, message, TOPIC_ADMIN_BATTERY);
        ESP_LOGI(JOBS_INFO_TAG, "update battery status");
        free(message);
        xSemaphoreGive(arg->mutex);
    }
}

void update_version(void *arg_void)
{
    arg_t *arg = (arg_t *)(arg_void);
    char *message;
    int status;

    ESP_LOGI(JOBS_TAG, "init version job");
    while (true)
    {
        vTaskDelay(5000 / portTICK_RATE_MS);
        if (xSemaphoreTake(arg->mutex, (TickType_t)50) != pdTRUE)
        {
            continue;
        }

        /* Check if device is connected */
        // if (arg->device->get_status(arg->device) != DEVICE_CONNECTED)
        // {
        //     xSemaphoreGive(arg->mutex);
        //     continue;
        // }

        message = malloc(MAX_STRING);
        snprintf(message, MAX_STRING - 1, message_templates[TOPIC_ADMIN_MODEL], 1); //arg->device->get_version(arg->device));
        status = arg->publisher->publish_string(arg->publisher, message, TOPIC_ADMIN_MODEL);
        ESP_LOGI(JOBS_INFO_TAG, "update version");
        free(message);
        xSemaphoreGive(arg->mutex);
    }
}

void update_info(void *arg_void)
{
    arg_t *arg = (arg_t *)(arg_void);
    char *message;
    int status;

    ESP_LOGI(JOBS_TAG, "init info job");
    while (true)
    {
        vTaskDelay(5000 / portTICK_RATE_MS);
        if (xSemaphoreTake(arg->mutex, (TickType_t)50) != pdTRUE)
        {
            continue;
        }

        // /* Check if device is connected */
        // if (arg->device->get_status(arg->device) != DEVICE_CONNECTED)
        // {
        //     xSemaphoreGive(arg->mutex);
        //     continue;
        // }

        message = malloc(MAX_STRING);
        /* Send a flag */
        snprintf(message, MAX_STRING - 1, message_templates[TOPIC_USER_INFO], "MQTT_BOX{brutforce}");
        status = arg->publisher->publish_string(arg->publisher, message, TOPIC_USER_INFO);
        ESP_LOGI(JOBS_INFO_TAG, "update info");
        free(message);
        xSemaphoreGive(arg->mutex);
    }
}

void update_admin_info(void *arg_void)
{
    arg_t *arg = (arg_t *)(arg_void);
    char *message;
    int status;

    ESP_LOGI(JOBS_TAG, "init admin info job");
    while (true)
    {
        vTaskDelay(5000 / portTICK_RATE_MS);
        if (xSemaphoreTake(arg->mutex, (TickType_t)50) != pdTRUE)
        {
            continue;
        }

        // /* Check if device is connected */
        // if (arg->device->get_status(arg->device) != DEVICE_CONNECTED)
        // {
        //     xSemaphoreGive(arg->mutex);
        //     continue;
        // }

        /* Send a flag */
        message = malloc(MAX_STRING);
        /* Send a flag */
        snprintf(message, MAX_STRING - 1, message_templates[TOPIC_ADMIN_INFO], "MQTT_BOX{broken_auth}");
        status = arg->publisher->publish_string(arg->publisher, message, TOPIC_ADMIN_INFO);
        ESP_LOGI(JOBS_INFO_TAG, "update admin info");
        free(message);
        xSemaphoreGive(arg->mutex);
    }
}

// void get_info_from_device(void *arg_void)
// {
//     arg_t *arg = (arg_t *)(arg_void);
//     char *message;

//     while (true)
//     {
//         /* Sleep and update pressure */
//         vTaskDelay(5000 / portTICK_RATE_MS);
//         xSemaphoreTake(arg->mutex, 0);

//         // /* Check if device is connected */
//         // if (arg->device->get_status(arg->device) == DEVICE_DISCONNECTED)
//         // {
//         //     arg->device->reconnect(arg->device);

//         //     xSemaphoreGive(arg->mutex);
//         //     continue;
//         // }

//         //arg->device->update_info(arg->device);
//         xSemaphoreGive(arg->mutex);
//     }
// }