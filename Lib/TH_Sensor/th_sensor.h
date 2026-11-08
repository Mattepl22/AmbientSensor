#ifndef TH_SENSOR_H
#define TH_SENSOR_H

#include <stdint.h>

typedef enum
{
    PORT_A,
    PORT_B,
    PORT_C,
    PORT_D,
    PORT_E,
    PORT_H
} sensor_pin_port_t;

typedef struct
{
    struct
    {
        sensor_pin_port_t port;
        uint8_t num;
    } sensor_pin;

    float temperature;
    float humidity;
} sensor_data_t;

void th_sensor_init(sensor_data_t *sensor_data);

#endif