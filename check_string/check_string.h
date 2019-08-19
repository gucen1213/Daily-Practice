#ifndef     __CHECK_STRING_H__
#   define  __CHECK_STRING_H__

#include "./app_cfg.h"

extern_simple_fsm(check_string,
    def_params(
        uint8_t * pchString;
        uint8_t chStatus;
        uint8_t chCount;
        uint8_t pchByte;
    )
)

extern_fsm_implementation(check_string);

extern_fsm_initialiser(check_string,
    args(
        uint8_t *pchString
    ));


#endif