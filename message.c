#include "message.h"
#include <stdio.h>

#define SCAN_MAX_TRIALS 1000


/* this section is for the handler functions that correspond to each message type (ID)*/
/* should probably me moved to its own handler files                                  */

static
void handler_gpio_set_mode(void* data)
{
    struct frame
    {
        uint8_t gpio_port;
        uint8_t gpio_ddr;
    };
    
    struct frame *gpio_args = (struct frame *)data;
    #ifdef TESTING
    printf("\nfunction call :: handler_gpio_set_mode()\n");
    #endif

    switch (gpio_args->gpio_port)
    {
        case('A'):
            #ifdef TESTING
            printf("\nPort:\t%c\nDDR:\t\02X\n");
            #endif
            #ifndef TESTING
            gpio_setmode(GPIO_A, gpio_args->gpio_ddr);
            #endif
            break;

        case('B'):
            #ifdef TESTING
            printf("\nPort:\t%c\nDDR:\t\02X\n");
            #endif
            #ifndef TESTING
            gpio_setmode(GPIO_B, gpio_args->gpio_ddr);
            #endif
            break;

        case('C'):
            #ifdef TESTING
            printf("\nPort:\t%c\nDDR:\t\02X\n");
            #endif
            #ifndef TESTING
            gpio_setmode(GPIO_C, gpio_args->gpio_ddr);
            #endif
            break;

        case('D'):
            #ifdef TESTING
            printf("\nPort:\t%c\nDDR:\t\02X\n");
            #endif
            #ifndef TESTING
            gpio_setmode(GPIO_D, gpio_args->gpio_ddr);
            #endif
            break;

        case('E'):
            #ifdef TESTING
            printf("\nPort:\t%c\nDDR:\t\02X\n");
            #endif
            #ifndef TESTING
            gpio_setmode(GPIO_E, gpio_args->gpio_ddr);
            #endif
            break;

        case('F'):
            #ifdef TESTING
            printf("\nPort:\t%c\nDDR:\t\02X\n");
            #endif
            #ifndef TESTING
            gpio_setmode(GPIO_F, gpio_args->gpio_ddr);
            #endif
            break;

        case('G'):
            #ifdef TESTING
            printf("\nPort:\t%c\nDDR:\t\02X\n");
            #endif
            #ifndef TESTING
            gpio_setmode(GPIO_G, gpio_args->gpio_ddr);
            #endif
            break;

        case('H'):
            #ifdef TESTING
            printf("\nPort:\t%c\nDDR:\t\02X\n");
            #endif
            #ifndef TESTING
            gpio_setmode(GPIO_H, gpio_args->gpio_ddr);
            #endif
            break;

        case('J'):
            #ifdef TESTING
            printf("\nPort:\t%c\nDDR:\t\02X\n");
            #endif
            #ifndef TESTING
            gpio_setmode(GPIO_J, gpio_args->gpio_ddr);
            #endif
            break;

        case('K'):
            #ifdef TESTING
            printf("\nPort:\t%c\nDDR:\t\02X\n");
            #endif
            #ifndef TESTING
            gpio_setmode(GPIO_K, gpio_args->gpio_ddr);
            #endif
            break;

        case('L'):
            #ifdef TESTING
            printf("\nPort:\t%c\nDDR:\t\02X\n");
            #endif
            #ifndef TESTING
            gpio_setmode(GPIO_L, gpio_args->gpio_ddr);
            #endif
            break;
        default:
            break;
    }
        
//:CLEANUP   
    printf("\n");
    printf("GPIO port:\t%x\n", gpio_args->gpio_port);
    printf("GPIO DDR:\t%x\n", gpio_args->gpio_ddr);
//CLEANUP
    return;
}

static
void handler_gpio_set_pin(void* data)
{
        struct frame
    {
        uint8_t gpio_port;
        uint8_t gpio_pin;
        uint8_t gpio_value;
    };
    
    struct frame *gpio_args = (struct frame *)data;

    printf("\n");
    printf("function call :: handler_gpio_set_pin()\n");
    printf("GPIO port:\t%x\n", gpio_args->gpio_port);
    printf("GPIO pin:\t%x\n", gpio_args->gpio_pin);
    printf("GPIO value:\t%x\n", gpio_args->gpio_value);
    return;
}

static
void handler_gpio_get_pin(void* data)
{
    struct frame
    {
        uint8_t gpio_port;
        uint8_t gpio_pin;
    };
    
    struct frame *gpio_args = (struct frame *)data;

    printf("\n");
    printf("function call :: handler_gpio_get_pin()\n");
    printf("GPIO port:\t%x\n", gpio_args->gpio_port);
    printf("GPIO pin:\t%x\n", gpio_args->gpio_pin);
    return;
}

static
void handler_set_pwm_duty(void* data)
{
    printf("\n");
    printf("function call :: handler_set_pwm_duty()\n");
    return;
}

static
void handler_echo_msg(void* data)
{
    printf("\n");
    printf("function call :: handler_echo_msg()\n");
    return;
}

static
void handler_set_kp(void* data)
{
    printf("\n");
    printf("function call :: handler_set_kp()\n");
    return;
}

static
void handler_set_ki(void* data)
{
    printf("\n");
    printf("function call :: handler_set_ki()\n");
    return;
}

static
void handler_set_kd(void* data)
{
    printf("\n");
    printf("function call :: handler_set_kd()\n");
    return;
}

handler_func_t handler_table[HANDLERS_TABLE_SZ] = 
{
    [1] = handler_gpio_set_mode,
    handler_gpio_set_pin,
    handler_gpio_get_pin,
    handler_set_pwm_duty,
    handler_echo_msg,
    handler_set_kp,
    handler_set_ki,
    handler_set_kd
};

static
int8_t scan(struct ringbuffer *buff, struct message *msg)
{
    static uint8_t n = 0;
    static enum sc_state state = IDLE;
    unsigned char c;

    int trials = 0;
     
    while(state != STOP)
    {
        if(rb_getc(&c, buff))
        {
            switch(state)
            {
                case IDLE:
                    if(c == PREAMBLE) state = FETCHING;
                    #ifdef TESTING
                        printf("IDLE state\n");
                    #endif
                    continue;

                case FETCHING:
                    ((uint8_t*)msg)[n++] = c;
                    if (n == sizeof(struct message)) state = TERM;
                    #ifdef TESTING
                        printf("FETCHING state\n");
                        printf("%d\n",n);
                    #endif
                    continue;

                case TERM:
                    if (c == TERMINATOR && !chksum((uint8_t *)msg, sizeof(*msg)))
                    {
                        state = STOP;
                        #ifdef TESTING
                            printf("Message TERM received and chksum passed.\n");
                        #endif
                        #ifndef TESTING
                            UART_putc(UART0, ACK);
                        #endif
                    } else {
                        state = IDLE;
                        #ifdef TESTING
                            printf("Message TERM missed or chksum passed.\n");
                        #endif
                        #ifndef TESTING
                        UART_putc(UART0, NAK);
                        #endif
                    }
                
                case STOP:
                    #ifdef TESTING
                        printf("STOP state reached.\n");
                    #endif
                    break;
                default:
                    #ifdef TESTING
                        printf("default exit. no match\n");
                    #endif
                    break;
            }
        } else {
            if(++trials > SCAN_MAX_TRIALS) state = STOP;
            #ifdef TESTING
                printf("no char returned from buffer.\n");
            #endif
            continue;
        }
    }
    return 0;
}

void dispatch(struct message *msg, handler_func_t table[])
{
    (*(table[msg->id]))(msg->data);
    return;
}

int8_t process_message(struct ringbuffer *buff)
{
    struct message msg;
    scan(buff, &msg);
    dispatch(&msg,handler_table);
    return 0;
}

#ifdef TESTING
    void print_message(struct message *msg)
    {
        printf("Message ID :\t\t%02X\n",msg->id);
        printf("Message Data :\t\t");
        for (int i = 0; i < MSG_SZ; i++) printf("%02X ",msg->data[i]);
        printf("\n");
        printf("Message Checksum :\t%02X\n",msg->chksum);
    }
#endif
