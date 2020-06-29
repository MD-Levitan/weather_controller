#include "device_complex.h"

#include <time.h>
#include <dht11.h>

typedef struct private_device_complex_t private_device_complex_t;

struct private_device_complex_t
{
    /* Public */
    device_complex_t public;
    /* Start time */
    time_t start_time;
    /* Version of device == 0x01 */
    uint8_t version;
    /* Status of device */
    device_status_t status;
};

static bool read_dht11(indicators_t *ind)
{
    struct dht11_reading data;
    data = DHT11_read();
    if (data.status != DHT11_OK)
    {
        return false;
    }
    ind->temperature = data.temperature;
    ind->humidity = data.humidity;
    return true;
}

METHOD(device_interface_t, get_version, uint8_t, private_device_complex_t *this)
{
    uint8_t version = this->version;
    return version;
}

METHOD(device_interface_t, get_work_time, uint32_t, private_device_complex_t *this)
{
    time_t value = time(NULL) - this->start_time;
    return value;
}


METHOD(device_interface_t, get_status, device_status_t, private_device_complex_t *this)
{
     return this->status;
}

METHOD(device_interface_t, get_indicators, void, private_device_complex_t *this, indicators_t *ind)
{
   if (!read_dht11(ind))
   {
       /* TODO */
       this->status = DEVICE_DISCONNECTED;
   }
   /* Add BMP180 */
}

METHOD(device_interface_t, destroy, void, private_device_complex_t *this)
{
    free(this);
}

device_complex_t *create_device_complex(gpio_num_t dht11_gpio)
{
    private_device_complex_t *this;
    this = malloc(sizeof(private_device_complex_t));
    
    this->public.device_inteface.get_indicators = _get_indicators;
    this->public.device_inteface.get_work_time = _get_work_time;
    this->public.device_inteface.get_status = _get_status;
    this->public.device_inteface.get_version = _get_version;
    this->public.device_inteface.destroy = _destroy;

    DHT11_init(dht11_gpio);

    this->start_time = time(NULL);
    /* complex version 0x00 */
    this->version = 0x01;
    this->status = DEVICE_CONNECTED;

    return &this->public;
}
