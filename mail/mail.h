#ifndef     __MAIL_H__
#define     __MAIL_H__

#include "stdbool.h"
#include ".\app_cfg.h"

typedef struct {
    event_t tSealed;
    void * pTarget;
} mailbox_t;

extern bool init_mailbox(mailbox_t *);
extern bool send_mail(mailbox_t *, void * );
extern void * open_mail(mailbox_t *);

#   define INIT_MAILBOX(__MAIL)        init_mailbox(__MAIL)
#   define POST_MAIL(__MAIL,__TARGET)  send_mail(__MAIL,__TARGET)
#   define OPEN_MAILBOX(__MAIL)        open_mail(__MAIL)

#endif

