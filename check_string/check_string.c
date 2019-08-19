#include "./app_cfg.h"

declare_simple_fsm(check_string);

def_simple_fsm(check_string,
    uint8_t * pchString;
    uint8_t chStatus;
    uint8_t chCount;
    uint8_t pchByte;        
)

fsm_initialiser(check_string,
    args(
        uint8_t * pchString
    ))
    init_body(
        if(NULL == pchString){
            abort_init();
        }
        this.chCount = 0;
        this.pchString = pchString;
    )


fsm_implementation(check_string)
    def_states(CHECK_END,GET_BYTE,CHECK_BYTE)
    body(
        state(CHECK_END,
            if(this.pchString[this.chCount] == '\0'){
                fsm_cpl(); 
            }
            transfer_to(GET_BYTE)
        )

        state(GET_BYTE,
            if(serial_in(&this.pchByte)){
                transfer_to(CHECK_BYTE);
            }
        )

        state(CHECK_BYTE,
            if(this.pchByte == this.pchString[this.chCount]){
                this.chCount ++;
                update_state_to(CHECK_END);
            }
            reset_fsm();
        )
    )
