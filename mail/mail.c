#include "..\event\event.h"
#include "string.h"

#define this   (*ptThis)

typedef struct {
    event_t tSealed;
    void * pTarget;
} mailbox_t;

bool init_mailbox(mailbox_t *ptThis)
{
    if (NULL == ptThis) {
        return false;
    }
    this.pTarget = NULL;
    INIT_EVENT(&this.tSealed,false,false);  
    return true;
}

bool send_mail(mailbox_t *ptThis,void * pObj)
{
    if ((ptThis != NULL)
        &&(pObj != NULL)) {
        SET_EVENT(&this.tSealed);
        this.pTarget = pObj;
        return true;
    }
    return false;
}

void * open_mail(mailbox_t *ptThis)
{
    if(ptThis == NULL){
        return NULL;
    }
    if(WAIT_EVENT(&this.tSealed)){
        return this.pTarget;
    }
    return NULL;
}

