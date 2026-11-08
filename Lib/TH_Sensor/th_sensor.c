#include "th_sensor.h"
#include "th_sensor_def.h"

#include "stm32f411xe.h"
#include "stm32f4xx.h"

#include <stdbool.h>

typedef enum
{
    INPUT_MODE = 0x0U,
    OUTPUT_MODE = 0x1U
} gpio_mode_t;

static GPIO_TypeDef *const gpio_ports[] = {
    [PORT_A] = GPIOA,
    [PORT_B] = GPIOB,
    [PORT_C] = GPIOC,
    [PORT_D] = GPIOD,
    [PORT_E] = GPIOE,
    [PORT_H] = GPIOH,
};

static void set_pin_mode(const sensor_data_t *sensor_data, gpio_mode_t mode)
{
    GPIO_TypeDef *gpio = gpio_ports[sensor_data->sensor_pin.port];
    uint32_t shift = sensor_data->sensor_pin.num * 2U;

    gpio->MODER &= ~(0x3U << shift);
    gpio->MODER |= ((uint32_t)mode << shift);
}

void th_sensor_init(sensor_data_t *sensor_data)
{
    switch (sensor_data->sensor_pin.port)
    {
        case PORT_A:
            RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
            GPIOA->MODER |= (0x1U << (sensor_data->sensor_pin.num * 2));
            break;

        case PORT_B:
            RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
            GPIOB->MODER |= (0x1U << (sensor_data->sensor_pin.num * 2));
            break;

        case PORT_C:
            RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
            GPIOC->MODER |= (0x1U << (sensor_data->sensor_pin.num * 2));
            break;

        case PORT_D:
            RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
            GPIOD->MODER |= (0x1U << (sensor_data->sensor_pin.num * 2));
            break;

        case PORT_E:
            RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
            GPIOE->MODER |= (0x1U << (sensor_data->sensor_pin.num * 2));
            break;

        case PORT_H:
            RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
            GPIOH->MODER |= (0x1U << (sensor_data->sensor_pin.num * 2));
            break;
    }
}