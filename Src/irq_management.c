#include "button.h"
#include "stm32f411xe.h"
#include "th_sensor.h"

void TIM1_TRG_COM_TIM11_IRQHandler(void)
{
    button_irq_handler();
}

void TIM1_UP_TIM10_IRQHandler(void)
{
    
}