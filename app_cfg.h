#ifndef __APP_CFG_H__
#define __APP_CFG_H__

#include "stdint.h"
#include "string.h"
#include "stdbool.h"

#include "simple_fsm.h"
#include "ooc.h"


#include "stdout_USART.h"

#define ABS(__N)    ((__N) < 0 ? -(__N) : (__N))
#define TOP         (0x00FF)
#define _BV(__N)	((uint32_t)1<<(__N))

#endif
