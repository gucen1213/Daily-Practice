#include "stdint.h"
#include "stdout_USART.h"
#include "LED.h"
#include "app_cfg.h"
#include "event.h"
#include "mail.h"

static event_t s_tEchoFree;
static event_t s_tEcho;
static event_t s_tApple;
static uint8_t s_chEchoByte;
static mailbox_t s_tMailBox;

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

#define PRINT_RESET_FSM()  \
    do{  \
        s_tState = PRINT_START;   \
    }while(0)		
static fsm_rt_t print_apple(void)   
{
    static enum {
        PRINT_START = 0,
        PRINT_H,
        PRINT_E,
        PRINT_L,
        PRINT_LL,
        PRINT_O
    }s_tState = PRINT_START;
    switch(s_tState){
        case PRINT_START:
            s_tState = PRINT_H;
//            break;
        case PRINT_H:
            if(serial_out('A')){
                s_tState = PRINT_E;
            }
            break;
        case PRINT_E:
            if(serial_out('P')){
                s_tState = PRINT_L;
            }            
            break;
        case PRINT_L:
            if(serial_out('P')){
                s_tState = PRINT_LL;
            }            
            break;
        case PRINT_LL:
            if(serial_out('L')){
                s_tState = PRINT_O;
            }            
            break;
        case PRINT_O:
            if(serial_out('E')){
                PRINT_RESET_FSM();
                return fsm_rt_cpl;
            }            
            break;
    }
    return fsm_rt_on_going;
} 

#define CHECK_WORLD_RESET_FSM()  \
    do{  \
        s_tState = CHECK_START;   \
    }while(0)	
static fsm_rt_t check_world(void)
{
    static enum{
        CHECK_START = 0,
        CHECK_W,
        CHECK_O,
        CHECK_R,
        CHECK_L,
        CHECK_D
    }s_tState = CHECK_START;
    static uint8_t s_chByte;
    uint8_t chByte;
    switch(s_tState){
        case CHECK_START:
            s_tState = CHECK_W;
//            break;
        case CHECK_W:
            if(serial_in(&chByte)){
                if(WAIT_EVENT(&s_tEchoFree)){
                    s_chByte = chByte;
                    POST_MAIL(&s_tMailBox,&s_chByte);
                }
                if(chByte == 'w'){
                    s_tState = CHECK_O;
                }else{
                    CHECK_WORLD_RESET_FSM();
                }
            }
            break;
        case CHECK_O:
            if(serial_in(&chByte)){
                if(WAIT_EVENT(&s_tEchoFree)){
                    s_chByte = chByte;
                    POST_MAIL(&s_tMailBox,&s_chByte);
                }
                if(chByte == 'o'){
                    s_tState = CHECK_R;
                }else{
                    CHECK_WORLD_RESET_FSM();
                }
            }            
            break;
        case CHECK_R:
            if(serial_in(&chByte)){
                if(WAIT_EVENT(&s_tEchoFree)){
                    s_chByte = chByte;
                    POST_MAIL(&s_tMailBox,&s_chByte);
                }
                if(chByte == 'r'){
                    s_tState = CHECK_L;
                }else{
                    CHECK_WORLD_RESET_FSM();
                }
            }
            break;
        case CHECK_L:
            if(serial_in(&chByte)){
                if(WAIT_EVENT(&s_tEchoFree)){
                    s_chByte = chByte;
                    POST_MAIL(&s_tMailBox,&s_chByte);
                }
                if(chByte == 'l'){
                    s_tState = CHECK_D;
                }else{
                    CHECK_WORLD_RESET_FSM();
                }
            }
            break;
        case CHECK_D:
            if(serial_in(&chByte)){
                if(WAIT_EVENT(&s_tEchoFree)){
                    s_chByte = chByte;
                    POST_MAIL(&s_tMailBox,&s_chByte);
                }
                if(chByte == 'd'){
                    CHECK_WORLD_RESET_FSM();
                    return fsm_rt_cpl;
                }else{
                    CHECK_WORLD_RESET_FSM();
                }
            }
            break;
    }
    return fsm_rt_on_going;
}


#define CHECK_TASK_RESET_FSM()  \
    do{  \
        s_tState = START;   \
    }while(0)
static fsm_rt_t check_task(void)
{
    static enum{
        START = 0,
        CHECK,
    }s_tState = START;  
    switch(s_tState){
        case START:
            s_tState = CHECK;  
//            break;
        case CHECK:
            if(fsm_rt_cpl == check_world()){
                SET_EVENT(&s_tApple);
                CHECK_TASK_RESET_FSM();
                return fsm_rt_cpl;
            }
            break;
    }
    return fsm_rt_on_going;
}


#define ECHO_TASK_RESET_FSM()  \
    do{  \
        s_tState = START;   \
    }while(0)
static fsm_rt_t echo_task(void)
{
    static enum{
        START = 0,
        WAIT_PRINT,
        WAIT_APPLE,
        SEND_CHAR,
    }s_tState = START; 
    uint8_t * ptTempObj;
    static uint8_t  s_chByte;
    switch(s_tState){
        case START:
            s_tState = WAIT_PRINT;
//            break;
        case WAIT_PRINT:
            ptTempObj = OPEN_MAILBOX(&s_tMailBox);
            if(NULL != ptTempObj){
                s_chByte = *ptTempObj;
                s_tState = WAIT_APPLE;
            }
            break;
        case WAIT_APPLE:
            if(WAIT_EVENT(&s_tApple)){
                s_tState = WAIT_APPLE;
            }else{
                s_tState = SEND_CHAR;
            }
            break;
        case SEND_CHAR:
            if(serial_out(s_chByte)){
                SET_EVENT(&s_tEchoFree);
                ECHO_TASK_RESET_FSM();
                return fsm_rt_cpl;
            }else{
                s_tState = WAIT_APPLE;
            }
            break;
    }
    return fsm_rt_on_going;
}

#define PRINT_TASK_RESET_FSM()  \
    do{  \
        s_tState = START;   \
    }while(0)
static fsm_rt_t print_task(void)
{
    static enum{
        START = 0,
        WAIT_EVENT,
        PRINT,
    }s_tState = START; 
    switch(s_tState){
        case START:
            s_tState = WAIT_EVENT; 
            break;
        case WAIT_EVENT:
            if(WAIT_EVENT(&s_tApple)){
                s_tState = PRINT; 
            }
            break;
        case PRINT:
            if(fsm_rt_cpl == print_apple()){
                RESET_EVENT(&s_tApple);
                PRINT_TASK_RESET_FSM();
                return fsm_rt_cpl;
            }
            break;
    }
    return fsm_rt_on_going;
}

int main(void)
{
    led_init();
    uart_init();
    INIT_EVENT(&s_tEchoFree,true,false);
    INIT_EVENT(&s_tEcho,false,false);
    INIT_EVENT(&s_tApple,false,true);
    INIT_MAILBOX(&s_tMailBox);
    while(1){
        breath_led();
        echo_task();
        print_task();
        check_task();
    }
}

