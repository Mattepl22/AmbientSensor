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

static bool send_start_signal(const sensor_data_t *sensor_data)
{
    GPIO_TypeDef *gpio = gpio_ports[sensor_data->sensor_pin.port];
    // Se è la port H la posizione da scrivere è la 7
    uint32_t shift = ((sensor_data->sensor_pin.port != PORT_H) ? sensor_data->sensor_pin.num : 7);

    static uint8_t machine_state = 0;   // Fase della macchina a stati
    static uint32_t start_time = 0;    // CNT di inizio della fase
    static bool waiting = false;        // Variabile per segnare l'attesa del tempo

    // Imposto il pin come output
    set_pin_mode(&sensor_data, OUTPUT_MODE);


    switch (machine_state)
    {
        case 0: // Alzo l'uscita e attendo per essere sicuro che il sensore lo rilevi
            if (!waiting)
            {
                gpio->ODR |= (1 << shift);
                start_time = TIM10->CNT;
                waiting = true;
            }

            if (waiting && ((TIM10->CNT - start_time) >= T_WAIT_SIGNAL_HIGH))
            {
                machine_state = 1;
                waiting = false;
            }
            else
            {
                // Il break lo metto qua così quando scatta la condizione ok salto diretto
                // allo stato successivo
                break; 
            }

        case 1: // Abbasso l'uscita e attendo 1ms
            if (!waiting)
            {
                gpio->ODR &= ~(1 << shift);
                start_time = TIM10->CNT;
                waiting = true;                
            }

            if (waiting && ((TIM10->CNT - start_time) >= T_WAIT_START_SIGNAL_LOW))
            {
                machine_state = 2;
                waiting = false;
            }
            else
            {
                break;
            }

        case 2: // Alzo l'uscita e attendo 20-40us
            if (!waiting)
            {
                gpio->ODR |= (1 << shift);
                start_time = TIM10->CNT;
                waiting = true;                
            }

            if (waiting && ((TIM10->CNT - start_time) >= T_WAIT_START_SIGNAL_HIGH))
            {
                machine_state = 3;
                waiting = false;
            }
            else
            {
                break;
            }

        case 3: // Azzero la macchina a stati e ritorno true in uscita per segnalare op completata
            machine_state = 0;
            return true;
            break;

        default:
            machine_state = 0;
            break;
    }

    return false;
}

static uint32_t count_signal_time(const sensor_data_t *sensor_data, bool state_to_read)
{
    GPIO_TypeDef *gpio = gpio_ports[sensor_data->sensor_pin.port];
    // Se è la port H la posizione da scrivere è la 7
    uint32_t shift = ((sensor_data->sensor_pin.port != PORT_H) ? sensor_data->sensor_pin.num : 7);

    static uint8_t machine_state = 0;   // Fase della macchina a stati
    static uint32_t start_time = 0;     // CNT di inizio della fase e del tempo di fine
    static uint32_t stop_time = 0;
    static bool waiting = false;        // Variabile per segnare l'attesa del tempo

    bool actual_pin_state;

    switch (machine_state)
    {
        case 0: // Attendo cambio stato segnale
            actual_pin_state = (gpio->IDR & (1 << shift));

            if (actual_pin_state == state_to_read)
            {
                machine_state = 1;
            }
            else
            {
                break;
            }

        case 1: // Comincio la misura del tempo
            if (!waiting)
            {
                start_time = TIM10->CNT;
                actual_pin_state = (gpio->IDR & (1 << shift));
                waiting = true;
            }
            
            if (waiting && (actual_pin_state != state_to_read))
            {
                stop_time = TIM10->CNT;
                waiting = false;
                machine_state = 2:
            }
            else
            {
                break;
            }
        
        case 2: // Calcolo il tempo 

            machine_state = 0;
            return stop_time - start_time;
        
        default:
            machine_state = 0;
            break;
    }

    return 0;
}

/*
 *  Definizione funzioni per utente
 */

void th_sensor_init(const sensor_data_t *sensor_data)
{
    GPIO_TypeDef *gpio = gpio_ports[sensor_data->sensor_pin.port];
    
    // Se è la port H la posizione da scrivere è la 7
    uint32_t shift = ((sensor_data->sensor_pin.port != PORT_H) ? sensor_data->sensor_pin.num : 7);

    RCC->AHB1ENR |= (1 << shift);

    timer_init();
}

bool th_sensor_read(sensor_data_t *sensor_data)
{
    // Azzero il timer per sicurezza
    TIM10->CNT = 0;
    // Abilito il timer
    TIM10->CR1 |= TIM_CR1_CEN;

    static uint8_t machine_state = 0; 

    // Macchina a stati per la gestione della lettura
    switch (machine_state)
    {
        case 0: // Segnale di start
            if (send_start_signal(&sensor_data))
            {
                machine_state = 1;
            }

            break;

        case 1: // Attendo risposta del sensore

        case 2:

        default:
            machine_state = 0;
    }

    return false;

}