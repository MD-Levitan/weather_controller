#include "device_simulator.h"

#include <time.h>

typedef struct private_device_simulator_t private_device_simulator_t;

struct private_device_simulator_t
{
    /* Public */
    device_simulator_t public;
    /* Start time */
    time_t start_time;
    /* Version of device == 0x00 */
    uint8_t version;
    /* Status of device, always CONNECTED */
    device_status_t status;
};

METHOD(device_interface_t, get_version, uint8_t, private_device_simulator_t *this)
{
    uint8_t version = this->version;
    return version;
}

METHOD(device_interface_t, get_work_time, uint32_t, private_device_simulator_t *this)
{
    time_t value = time(NULL) - this->start_time;
    return value;
}


METHOD(device_interface_t, get_status, device_status_t, private_device_simulator_t *this)
{
     return this->status;
}

METHOD(device_interface_t, get_indicators, void, private_device_simulator_t *this, indicators_t *ind)
{
    ind->temperature = (double)(rand() % 2000 + 2000) / 100;
    ind->humidity =  rand() % 100;
    ind->pressure = rand() % 100 + 700;
}

METHOD(device_interface_t, destroy, void, private_device_simulator_t *this)
{
    free(this);
}

device_simulator_t *create_device_simulator()
{
    private_device_simulator_t *this;
    this = malloc(sizeof(private_device_simulator_t));
    
    this->public.device_inteface.get_indicators = _get_indicators;
    this->public.device_inteface.get_work_time = _get_work_time;
    this->public.device_inteface.get_status = _get_status;
    this->public.device_inteface.get_version = _get_version;
    this->public.device_inteface.destroy = _destroy;

    this->start_time = time(NULL);
    /* Simulator version 0x00 */
    this->version = 0x00;
    this->status = DEVICE_CONNECTED;

    return &this->public;
}
