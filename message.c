#include "message.h"
#include <stdio.h>

void callback_gpio_set_mode(void* data)
{
    struct frame
    {
        uint8_t gpio_port;
        uint8_t gpio_ddr;
    };
    
    struct frame *gpio_settings = (struct frame *)data;

    printf("function call :: callback_gpio_set_mode()\n");
    printf("GPIO port:\t%x\n", gpio_settings->gpio_port);
    printf("GPIO DDR:\t%x\n", gpio_settings->gpio_ddr);
    return;
}

void callback_gpio_set_pin(void* data)
{
        struct frame
    {
        uint8_t gpio_port;
        uint8_t gpio_pin;
        uint8_t gpio_value;
    };
    
    struct frame *gpio_settings = (struct frame *)data;

    printf("function call :: callback_gpio_set_pin()");
    printf("GPIO port:\t%x\n", gpio_settings->gpio_port);
    printf("GPIO pin:\t%x\n", gpio_settings->gpio_pin);
    printf("GPIO value:\t%x\n", gpio_settings->gpio_value);
    return;
}

void callback_gpio_get_pin(void* data)
{
    printf("function call :: callback_gpio_get_pin()");
    return;
}

void callback_set_pwm_duty(void* data)
{
    printf("function call :: callback_set_pwm_duty()");
    return;
}

void callback_echo_msg(void* data)
{
    printf("function call :: callback_echo_msg()");
    return;
}

void callback_set_kp(void* data)
{
    printf("function call :: callback_set_kp()");
    return;
}

void callback_set_ki(void* data)
{
    printf("function call :: callback_set_ki()");
    return;
}

void callback_set_kd(void* data)
{
    printf("function call :: callback_set_kd()");
    return;
}

callback_func_t callback_table[CALLBACK_TABLE_SZ] = 
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

static
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

static
void dispatch(struct message *msg, callback_func_t table[])
{
    (*(table[msg->id]))(msg->data);
    return;
}

int8_t process_message(struct ringbuffer *buff)
{
    struct message msg;
    scan(buff, &msg);
    dispatch(&msg,callback_table);
    return 0;
}