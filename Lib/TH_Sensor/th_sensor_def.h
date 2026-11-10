#ifndef TH_SENSOR_DEF_H
#define TH_SENSOR_DEF_H

#include <stdint.h>

#define LOW_STATE                   0
#define HIGH_STATE                  1

#define PSC_VALUE                   (16 - 1)            // 1us
#define ARR_VALUE                   (20000 - 1)         // 20ms

#define T_WAIT_SIGNAL_HIGH          ((uint32_t) 1000)   // 1ms
#define T_WAIT_START_SIGNAL_LOW     ((uint32_t) 1000)   // 1ms  
#define T_WAIT_START_SIGNAL_HIGH    ((uint32_t) 30)     // 30us

#endif