#ifndef DEVICE_H_
#define DEVICE_H_

#include <stdio.h>
#include <stdlib.h>

#include <inttypes.h>

#include "macros.h"


typedef struct device_interface_t device_interface_t;
typedef struct indicators_t indicators_t;
typedef enum device_status_t device_status_t;

/* Standart values of devices */
struct indicators_t
{
    uint8_t humidity;

    uint32_t pressure;

    double temperature;
};

enum device_status_t
{
    /* Device started successfuly */
    DEVICE_CONNECTED = 0x01,
    /* Device disconnected */
    DEVICE_DISCONNECTED = 0x00,
    /* Device try to reload */
    DEVICE_RECONNECTING = 0x02,
};

struct device_interface_t
{
    /**
     * @brief Get indicators from device.
     * 
     * @param indicators    pointer to store indicators from device
     */
    void (*get_indicators)(device_interface_t *device, indicators_t *indicators);

    /**
     * @brief Get temperatire from device.
     * 
     * @return      tempearature in C
     */
    double (*get_temperature)(device_interface_t *device);

    /**
     * @brief Get pressure from device.
     * 
     * @return      pressure in Pa
     */
    uint32_t (*get_pressure)(device_interface_t *device);

    /**
     * @brief Get humidity from device.
     * 
     * @return      humidity
     */
    uint8_t (*get_humidity)(device_interface_t *device);

    /**
     * @brief Get status from sensor.
     * 
     * @return      status of device
     */
    device_status_t (*get_status)(device_interface_t *device);

    /**
     * @brief Get version of device. 
     * 
     * @return      version of device
     */
    uint8_t (*get_version)(device_interface_t *device_t);

    /**
     * @brief Get wotk time of device in seconds.
     * Start time during crating of device. 
     * 
     * @return      time of wotk in sec
     */
    uint32_t (*get_work_time)(device_interface_t *device);

    /**
     * @brief Destroy device object.
     */
    void (*destroy)(device_interface_t *this);
};

#endif /* DEVICE_H_ */