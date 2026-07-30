#include "button.h"
#include "platform.h"

void button_init(void) { platform_button_init(); }
bool button_is_pressed(void) { return platform_button_is_pressed(); }
