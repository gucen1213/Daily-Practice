#ifndef     __PRINT_STRING_H__
#   define  __PRINT_STRING_H__

#include "./app_cfg.h"

extern_simple_fsm(print_string,
    def_params(
        uint8_t * pchString;
        uint8_t chStatus;      
    )
)

extern_fsm_implementation(print_string);

extern_fsm_initialiser(print_string,
    args(
        uint8_t *pchString
    )
);


#endif