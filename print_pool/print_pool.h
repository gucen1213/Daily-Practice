#ifndef  _PRINT_POOL_APP_H_
#define  _PRINT_POOL_APP_H_

#include "./app_cfg.h"

#define PRINT_STR_POOL_ITEM_SIZE         sizeof(fsm(print_string)) 

typedef struct print_str_pool_item_t     print_str_pool_item_t;
struct print_str_pool_item_t{
    bool     bIsFree;
    uint8_t  chBuffer[PRINT_STR_POOL_ITEM_SIZE];
};

extern void print_str_pool_free(print_str_pool_item_t *ptItem);

extern print_str_pool_item_t * print_str_pool_allocate (void);

extern bool print_str_pool_init(void);



#endif