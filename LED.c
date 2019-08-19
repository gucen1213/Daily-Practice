#include <string.h>
#include <stdbool.h>
#include "Board_LED.h"
#include "Board_BUTTONS.h"

void led_init(void)
{
    while(LED_Initialize());
    Buttons_Initialize();
}

