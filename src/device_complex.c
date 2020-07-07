#include "device_complex.h"

#include <time.h>

/* Include libs for BMP180 and DHT11 */
#include <dht11.h>
#include <bmp180.h>

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
    /* BMP180 sensor */
    bmp_sensor_t *sensor;
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

static uint8_t read_dht11_humidity()
{
    struct dht11_reading data;
    data = DHT11_read();
    if (data.status != DHT11_OK)
    {
        return 0;
    }
    return data.humidity;
}

static bool read_bmp180(bmp_sensor_t *sensor, indicators_t *ind)
{
    tuple_t *pair;
    pair = sensor->get_tuple(sensor);
    if (pair == NULL)
    {
        return false;
    }
    ind->temperature = pair->temperature;
    ind->pressure = pair->pressure;
    free(pair);
    return true;
}

static double read_bmp180_temp(bmp_sensor_t *sensor)
{
    return sensor->get_temperature(sensor);
}

static uint32_t read_bmp180_pressure(bmp_sensor_t *sensor)
{
    return sensor->get_pressure(sensor);
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
    /* As BMP180 temperature senosr is more precisely than DHT11 that's way we get temp from it */
    if (!read_dht11(ind) || !read_bmp180(this->sensor, ind))
    {
        /* TODO */
        this->status = DEVICE_DISCONNECTED;
    }
}

METHOD(device_interface_t, get_temperature, double, private_device_complex_t *this)
{
    return read_bmp180_temp(this->sensor);
}

METHOD(device_interface_t, get_pressure, uint32_t, private_device_complex_t *this)
{
    return read_bmp180_pressure(this->sensor);
}

METHOD(device_interface_t, get_humidity, uint8_t, private_device_complex_t *this)
{
    return read_dht11_humidity();
}

METHOD(device_interface_t, destroy, void, private_device_complex_t *this)
{
    this->sensor->destroy(this->sensor);
    free(this);
}

device_complex_t *create_device_complex(gpio_num_t dht11_gpio, gpio_num_t sda_gpio, gpio_num_t slc_gpio)
{
    private_device_complex_t *this;
    this = malloc(sizeof(private_device_complex_t));

    this->public.device_inteface.get_indicators = _get_indicators;
    this->public.device_inteface.get_humidity = _get_humidity;
    this->public.device_inteface.get_temperature = _get_temperature;
    this->public.device_inteface.get_pressure = _get_pressure;
    this->public.device_inteface.get_work_time = _get_work_time;
    this->public.device_inteface.get_status = _get_status;
    this->public.device_inteface.get_version = _get_version;
    this->public.device_inteface.destroy = _destroy;

    DHT11_init(dht11_gpio);
    this->sensor = create_bmp_sensor(sda_gpio, slc_gpio, BMP085_MODE_STANDARD);

    if (this->sensor == NULL ||
        this->sensor->begin(this->sensor) != STATUS_OK)
    {
        free(this);
        return NULL;
    }

    this->start_time = time(NULL);
    /* complex version 0x00 */
    this->version = 0x01;
    this->status = DEVICE_CONNECTED;

    return &this->public;
}
