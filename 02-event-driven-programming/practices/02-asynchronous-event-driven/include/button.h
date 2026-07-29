#ifndef BUTTON_H
#define BUTTON_H

#include "event.h"

#include <stdbool.h>

void button_init(void);
void button_event_handler(const event_t *event);
bool button_is_stably_pressed(void);

#endif
