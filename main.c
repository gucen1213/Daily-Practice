#include "stdint.h"
#include "stdout_USART.h"
#include "LED.h"
#include "event.h"
#include "mail.h"
#include "app_cfg.h"
#include "print_string.h"
#include "./check_string/check_string.h"
#include "ooc.h"
#include "simple_fsm.h"
#include "print_pool.h"

static event_t s_tPrint;

#define Log(__STR)  \
    do{       \
        uint8_t *pchSTR = (__STR);  \
        do{          \
            while(!serial_out(*pchSTR)); \
            pchSTR++;         \
            if('\0' == *pchSTR){    \
                break;    \
            }   \
        }while(1); \
    }while(0)  

static void set_led_gradation(uint16_t hwLevel)
{
    static uint16_t s_hwCounter = 0;
    
    if (hwLevel >= s_hwCounter) {
        LED1_ON();
    } else {
        LED1_OFF();
    }
    s_hwCounter++;
    s_hwCounter &= TOP;
}

static void breath_led(void)   //???
{
    static uint16_t s_hwCounter = 0;
    static int16_t s_nGray = (TOP >> 1);
    s_hwCounter++;
    if (!(s_hwCounter & (_BV(10)-1))) {
        s_nGray++; 
        if (s_nGray == TOP) {
            s_nGray = 0;
        }
    }
    set_led_gradation(ABS(s_nGray - (TOP >> 1)));
}


def_simple_fsm(print_task,
    print_str_pool_item_t  *pfsmPrintHello;
)

def_simple_fsm(check_world,
    def_params(
        fsm(check_string) fsmCheckString;
    )
)

def_simple_fsm(task_check)

declare_simple_fsm(task_check);
declare_simple_fsm(print_hello);
declare_simple_fsm(print_task);
declare_simple_fsm(check_world);

def_simple_fsm(print_hello,
    def_params(
        fsm(print_string) fsmPrintString;
    )
)

fsm_implementation(print_hello)
    def_states(PRINT_INIT,PRINT)
    body(
        state(PRINT_INIT,
            if(init_fsm(print_string,&this.fsmPrintString,
                args((uint8_t*)"hello\r\n"))
            ){
                transfer_to(PRINT);
            }
        )

        state(PRINT,
            if(fsm_rt_cpl == call_fsm(print_string,&this.fsmPrintString)){
                fsm_cpl();
            }
        )
    )

fsm_implementation(print_task)
    def_states(WAIT_EVENT,APPLY,PRINT)
    body(
        state(WAIT_EVENT){
            if(WAIT_EVENT(&s_tPrint)){
                transfer_to(APPLY);
            }
        }
        state(APPLY){
            this.pfsmPrintHello = print_str_pool_allocate();
            if(NULL != this.pfsmPrintHello){
                transfer_to(PRINT);
            }
        }
        state(PRINT){
            if(fsm_rt_cpl == call_fsm(print_hello,(fsm(print_hello)*)this.pfsmPrintHello)){
                print_str_pool_free(this.pfsmPrintHello);
                fsm_cpl();
            }
        }
    )


fsm_implementation(check_world)
    def_states(CHECK_INIT,CHECK)
    body(
        state(CHECK_INIT,
            if(init_fsm(check_string,&this.fsmCheckString,
                args((uint8_t*)"world"))
            ){
                transfer_to(CHECK);
            }
        )

        state(CHECK,
            if(fsm_rt_cpl == call_fsm(check_string,&this.fsmCheckString)){
                fsm_cpl();
            }
        )
    )

static fsm(check_world) s_fsmCheckWorld;
fsm_implementation(task_check)
    def_states(CHECK_STRING)

    body(
        state(CHECK_STRING,
            if(fsm_rt_cpl == call_fsm(check_world,&s_fsmCheckWorld)){
                SET_EVENT(&s_tPrint);
                fsm_cpl(); 
            }
        )
    )

static fsm(task_check) s_fsmTaskCheck;
static fsm(print_task) s_fsmPrintTask;

int main(void)
{
    led_init();
    uart_init();
    print_str_pool_init();
    INIT_EVENT(&s_tPrint,false,false);
    while(1){
        breath_led();
        call_fsm(task_check,&s_fsmTaskCheck);
        call_fsm(print_task,&s_fsmPrintTask);
    }
}

