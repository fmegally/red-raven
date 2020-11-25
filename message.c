#include "message.h"

void (*callback_func[CALLBACK_TABLE_SZ])(void *data) = 
{
    [1] = callback_gpio_set_mode,
    callback_gpio_set_pin,
    callback_gpio_get_pin,
    callback_set_pwm_duty,
    callback_echo_msg,
    callback_set_kp,
    callback_set_ki,
    callback_set_kd
};

int8_t scan(struct ringbuffer *buff, struct message *msg)
{
    static uint8_t n = 0;
    static enum sc_state state = IDLE;
    uint8_t c;
     
    while(state != STOP)
    {
        if(rb_getc(&c, buff))
        {
            switch(state)
            {
                case IDLE:
                    if(c == PREAMBLE) state = FETCHING;
                    continue;

                case FETCHING:
                    ((uint8_t*)msg)[n++] = c;
                    if (n == sizeof(struct message)) state = TERM;
                    continue;

                case TERM:
                    if (c == TERMINATOR && !chksum((uint8_t *)msg, sizeof(*msg)))
                    {
                        state = STOP;
                        UART_putc(UART0, ACK);
                    } else {
                        state = IDLE;
                        UART_putc(UART0, NAK);
                    }
            }
        } else {
            continue;
        }
    }
}

void dispatch(struct message * msg )
{

    return;
}