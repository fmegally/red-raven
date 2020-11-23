#include "message.h"

void scan(struct ringbuffer *buff, struct message *msg)
{
    static uint8_t n = 0;
    static enum sc_state state = IDLE;
    uint8_t c;
    
    rb_getc(&c, buff);

    
    switch(state){
        case IDLE:
            if(c == PREAMBLE){
                state = FETCHING;
            } else {
                
            }
            
            break;
        case FETCHING:
            state = CHECKSUM;
            break;
        case CHECKSUM:
            state = IDLE;
            break;
        default:
            return;
    }
}
void dispatch(struct message *msg)
{
    return;
}
