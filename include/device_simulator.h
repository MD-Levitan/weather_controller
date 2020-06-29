#ifndef DEVICE_SIMULATOR_H_
#define DEVICE_SIMULATOR_H_

#include "device.h"

typedef struct device_simulator_t device_simulator_t;

struct device_simulator_t
{
    /* Implement  device interface */
    device_interface_t device_inteface;
};

device_simulator_t *create_device_simulator();

#endif /* DEVICE_SIMULATOR_H_ */