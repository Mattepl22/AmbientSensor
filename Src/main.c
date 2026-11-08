#include <stdint.h>
#include <stdio.h>

#include "button.h"
#include "th_sensor.h"

extern void initialise_monitor_handles(void);

int main(void)
{
    uint8_t index = 0;

    /* Loop forever */
    initialise_monitor_handles();
    button_init();

    while(1)
    {
        if (button_state(true))
        {
            printf("Pulsante premuto %u volte\n", ++index);
        }
    }

}

