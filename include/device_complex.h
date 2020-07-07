#ifndef DEVICE_COMPLEX_H_
#define DEVICE_COMPLEX_H_

#include "device.h"

#include "driver/gpio.h"

typedef struct device_complex_t device_complex_t;

/**
 *  Complex of sensors which collected data.
 *  Sensors: DHT11, BMP180.
 */
struct device_complex_t
{
    /* Implement device interface */
    device_interface_t device_inteface;
};

/**
 * @brief Create a device complex object
 * 
 * @param dht11_gpio            gpio of DHT11 sensor
 * @param sda_gpio              gpio of SDA
 * @param slc_gpio              gpio of SLC
 * @return device_complex_t* 
 */
device_complex_t *create_device_complex(gpio_num_t dht11_gpio, gpio_num_t sda_gpio, gpio_num_t slc_gpio);

#endif /* DEVICE_COMPLEX_H_ */