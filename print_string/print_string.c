#include "./app_cfg.h"

declare_simple_fsm(print_string);

def_simple_fsm(print_string,
    uint8_t * pchString;
    uint8_t chStatus;       
)

fsm_initialiser(print_string,
    args(
        uint8_t * pchString
    ))
    init_body(
        if(NULL == pchString){
            abort_init();
        }
        this.pchString = pchString;
    )

fsm_implementation(print_string)
    def_states(CHECK_END,SEND_CHAR)
    body(
        state(CHECK_END,
            if(*(this.pchString) != '\0'){
                transfer_to(SEND_CHAR);
            }else{
                fsm_cpl();
            }
        )

        state(SEND_CHAR,
            if(serial_out(*this.pchString)){
                this.pchString++;
                transfer_to(CHECK_END)
            }
        )
    )





