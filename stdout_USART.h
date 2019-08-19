#ifndef __USE_STDOUT_USART_H__
#define __USE_STDOUT_USART_H__

#include <string.h>
#include "stdint.h"
#include "stdbool.h"

extern bool uart_init (void);
extern bool serial_out(uint8_t chByte);
extern bool serial_in(uint8_t *pchByte);


#endif

