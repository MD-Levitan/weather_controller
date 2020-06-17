#ifndef JOBS_H_
#define JOBS_H_

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

#include "mqtt_publisher.h"
#include "macros.h"

typedef void (*job_t)(void *);
typedef struct arg_t arg_t;

static const char *JOBS_TAG = "JOB";
static const char *JOBS_INFO_TAG = "JOB_INFO";

struct arg_t
{
    /* Mutex for Publisher */
    SemaphoreHandle_t mutex;

    /* MQTT Publisher */
    mqtt_publisher_t *publisher;

    // /* Config of publisher */
    // config_t *config;

    // /* Device */
    // device_t *device;
};

void update_temperature(void *arg_void);
void update_humidity(void *arg_void);
void update_pressure(void *arg_void);
void update_admin_info(void *arg_void);
void update_info(void *arg_void);
void update_work_time(void *arg_void);
void update_status(void *arg_void);
void update_version(void *arg_void);
void update_battery(void *arg_void);
void get_info_from_device(void *arg_void);

static job_t jobs[] = {//get_info_from_device,
                       update_humidity, update_pressure,
                       update_temperature, update_info, update_admin_info,
                       update_battery, update_work_time, update_status, update_version};

#endif //JOBS_H_