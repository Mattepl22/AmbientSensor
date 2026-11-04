#include <stdint.h>
#include <stdio.h>

#include "button.h"

extern void initialise_monitor_handles(void);

int main(void)
{
    /* Loop forever */
    initialise_monitor_handles();
    button_init();

	while(1)
    {
        if (!button_state())
        {
            printf("Bottone premuto!\n");
        }
        else
        {
            printf("Bottone NON premuto!\n");
        }
    }
}
