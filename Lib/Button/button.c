#include "button.h"

#include "button_def.h"
#include "stm32f411xe.h"
#include "stm32f4xx.h"

static volatile bool button_active = false;

static void timer_init(void)
{
    // 1. Abilitazione del clock del bus
    RCC->APB2ENR |= RCC_APB2ENR_TIM11EN;

    // 2. Configurazione del prescaler
    // Voglio 1 tick ogni 1ms => (16000 HZ * 1ms) - 1 = 15999
    TIM11->PSC = PRESCALER_VALUE;

    // 3. Configuro autoreload
    TIM11->ARR = TIMER_ARR_VALUE;

    // 4. Forzo aggiornamento registri
    TIM11->EGR |= TIM_EGR_UG;

    // 5. Pulizia flag di update nel registro di stato
    TIM11->SR &= ~TIM_SR_UIF;

    // 6. Abilito interrupt update del timer
    TIM11->DIER |= TIM_DIER_UIE;

    // 7. Abilito interrupt a livello di processore
    NVIC_EnableIRQ(TIM1_TRG_COM_TIM11_IRQn);
    NVIC_SetPriority(TIM1_TRG_COM_TIM11_IRQn, 2);
}

void button_irq_handler(void)
{
    if(TIM11->SR & TIM_SR_UIF)
    {
        TIM11->SR &= ~TIM_SR_UIF;
        TIM11->CR1 &= ~TIM_CR1_CEN;
        button_active = true;
    }
}

void button_init(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; // Abilito il clock per GPIOA
    GPIOA->MODER &= ~(0x00 << (0 * 2));  // Imposto il pin come input
    GPIOA->PUPDR &= ~(0x00 << (0 * 2));  // Resetto il registro del pullup
    GPIOA->PUPDR |= (0x01 << (0 * 2));   // Setto la resistenza di pullup interna

    button_active = true;

    timer_init();
}

bool button_state(bool button_inverted)
{
    bool button_state = (GPIOA->IDR & (0x1 << 0));

    button_state ^= button_inverted;

    if(button_active && button_state)
    {
        button_active = false;
        TIM11->CR1 |= TIM_CR1_CEN;
        return true;
    }

    return false;
}