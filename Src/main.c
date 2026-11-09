#include <stdint.h>
#include <stdio.h>

#include "button.h"
#include "th_sensor.h"

extern void initialise_monitor_handles(void);

int main(void)
{
    initialise_monitor_handles();

    /* Loop forever */
    while(1);
}

