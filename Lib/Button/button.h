#ifndef BUTTON_H
#define BUTTON_H

#include <stdbool.h>

void button_init(void);
bool button_state(bool button_inverted);
void button_irq_handler(void);

#endif