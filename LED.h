#ifndef __USE_LED_H__
#define __USE_LED_H__

#include "Board_LED.h"

#define LED1_ON()   LED_On(0)
#define LED1_OFF()  LED_Off(0)

#define LED2_ON()   LED_On(1)
#define LED2_OFF()  LED_Off(1)

extern void led_init(void);

#endif

