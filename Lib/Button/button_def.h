#ifndef BUTTON_DEF_H
#define BUTTON_DEF_H

#define FREQ_KHZ_CLK_TIM             16000U
#define MS_TICK                     1U
#define PRESCALER_VALUE             ((FREQ_KHZ_CLK_TIM * MS_TICK) - 1)

#define TICK_RESET_VALUE            200U
#define TIMER_ARR_VALUE             (TICK_RESET_VALUE - 1)

#endif