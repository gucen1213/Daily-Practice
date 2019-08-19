#include <stdio.h>

#include "LED.h"
#include "Board_BUTTONS.h"
#include "stdout_USART.h"

void system_init(void)
{
	led_init();
	Buttons_Initialize();
}
