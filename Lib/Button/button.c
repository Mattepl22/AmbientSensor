#include "button.h"

#include "stm32f411xe.h"
#include "stm32f4xx.h"

void button_init(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; // Abilito il clock per GPIOA
    GPIOA->MODER &= ~(0x00 << (0 * 2));  // Imposto il pin come input
    GPIOA->PUPDR &= ~(0x00 << (0 * 2));  // Resetto il registro del pullup
    GPIOA->PUPDR |= (0x01 << (0 * 2));   // Setto la resistenza di pullup interna
}

bool button_state(void)
{
    return (GPIOA->IDR & (0x1 << 0));
}