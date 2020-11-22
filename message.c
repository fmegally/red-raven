#include "message.h"

void scan(struct ringbuffer *buff, struct message *msg)
{
    static enum sc_state state = IDLE;
    switch(state){
        case IDLE:
            break;
        case FETCHING:
            break;
        case CHECKSUM:
            break;
        default:
            return;
    }
}
void dispatch(struct message *msg)
{
    return;
}
