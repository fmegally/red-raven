#include "message.h"

int8_t scan(struct ringbuffer *buff, struct message *msg)
{
    static uint8_t n = 0;
    static enum sc_state state = IDLE;
    uint8_t c;
     
    while(true)
    {
        if(rb_getc(&c, buff))
        {
            switch(state)
            {
                case IDLE:
                    if(c == PREAMBLE){
                        state = FETCHING;
                        continue;
                    } else {
                        return ERROR_INVALID_PREAMBLE;
                    }

                case FETCHING:
                    ((uint8_t*)msg)[n++] = c;
                    if (n == sizeof(struct message)) state = TERM;
                    continue;

                case TERM:
                    if (c == TERMINATOR && chksum((uint8_t *)msg, sizeof(*msg)))
            }
        } else {
            continue;
        }
    }
}

void dispatch(struct message *msg)
{
    return;
}