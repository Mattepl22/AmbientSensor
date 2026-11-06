#include "button.h"

void TIM1_TRG_COM_TIM11_IRQHandler(void)
{
    button_irq_handler();
}