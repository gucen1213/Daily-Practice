#include "./app_cfg.h"
#include "../print_string/print_string.h"

#define PRINT_STR_POOL_ITEM_SIZE         sizeof(fsm(print_string)) 

typedef struct print_str_pool_item_t     print_str_pool_item_t;
struct print_str_pool_item_t{
    bool     bIsFree;
    uint8_t  chBuffer[PRINT_STR_POOL_ITEM_SIZE];
};

#define PRINT_STR_POOL_ITEM_COUNT 2

static print_str_pool_item_t s_tExamplePool[PRINT_STR_POOL_ITEM_COUNT];

void print_str_pool_free(print_str_pool_item_t *ptItem)
{
    if(NULL == ptItem){
        return;
    }
    ptItem->bIsFree = true;
}

static uint8_t s_chItem;

print_str_pool_item_t * print_str_pool_allocate (void)
{
    uint8_t chCount = s_chItem + 1;
    uint8_t chItemCount = 0;
    do{
        if(chCount >= PRINT_STR_POOL_ITEM_COUNT){
            chCount = 0;
        }
        if(s_tExamplePool[chCount].bIsFree){
            s_chItem = chCount;
            s_tExamplePool[chCount].bIsFree = false;
            return &s_tExamplePool[chCount];
        }
        chCount ++ ;
        chItemCount ++;
    }while(chItemCount <= PRINT_STR_POOL_ITEM_COUNT);

    return NULL;
}


bool print_str_pool_init(void)
{
    uint8_t chCount = PRINT_STR_POOL_ITEM_COUNT;
    s_chItem = 0;
    do{
        print_str_pool_free(&s_tExamplePool[chCount]);
        chCount --;
    }while(chCount);
    
    return true;
}

