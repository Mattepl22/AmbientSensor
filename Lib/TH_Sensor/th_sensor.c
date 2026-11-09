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

static void timer_init(void)
{
    // 1. Abilitazione del clock del bus
    RCC->APB2ENR |= RCC_APB2ENR_TIM11EN;

    // 2. Configurazione del prescaler
    // Voglio 1 tick ogni 1us => (16000000 HZ * (1e-6)s) - 1 = 15
    TIM10->PSC = PSC_VALUE;

    // 3. Configuro autoreload di 10000 -> 10 ms
    TIM10->ARR = ARR_VALUE;

    // 4. Forzo aggiornamento registri
    TIM10->EGR |= TIM_EGR_UG;

    // 5. Pulizia flag di update nel registro di stato
    TIM10->SR &= ~TIM_SR_UIF;

    // 6. Abilito interrupt update del timer
    TIM10->DIER |= TIM_DIER_UIE;

    // 7. Abilito interrupt a livello di processore
    NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);
    NVIC_SetPriority(TIM1_UP_TIM10_IRQn, 2);
}

void th_sensor_init(sensor_data_t *sensor_data)
{
    GPIO_TypeDef *gpio = gpio_ports[sensor_data->sensor_pin.port];
    
    // Se è la port H la posizione da scrivere è la 7
    uint32_t shift = ((sensor_data->sensor_pin.port != PORT_H) ? sensor_data->sensor_pin.num : 7);

    RCC->AHB1ENR |= (1 << shift);
    set_pin_mode(&sensor_data, OUTPUT_MODE);
}