#include "gpio.h"
#include "uart.h"
#include "message.h"
#include <stdio.h>

#define SCAN_MAX_TRIALS 20


/* this section is for the handler functions that correspond to each message type (ID)*/
/* should probably me moved to its own handler files                                  */

unsigned char atord(char c)
{
	if (c >= 'a' && c <= 'z')
	{
		return (c - 'a') + 1;
	}
	else if (c >= 'A' && c <= 'Z')
	{
		return (c - 'A') + 1;
	}
	else
	{
		return 0;
	}
}

static
void handler_gpio_set_mode(void* data)
{
    struct frame
    {
        uint8_t gpio_port;
        uint8_t gpio_ddr;
    };
    
    struct frame *gpio_args = (struct frame *)data;
    UART_print(UART0,"\nfunction call :: handler_gpio_set_mode()\n");

    switch (gpio_args->gpio_port)
    {
        case('A'):
            gpio_setmode(GPIO_PORTA, gpio_args->gpio_ddr);
            break;

        case('B'):
            gpio_setmode(GPIO_PORTB, gpio_args->gpio_ddr);
            break;

        case('C'):
            gpio_setmode(GPIO_PORTC, gpio_args->gpio_ddr);
            break;

        case('D'):
            gpio_setmode(GPIO_PORTD, gpio_args->gpio_ddr);
            break;

        case('E'):
            gpio_setmode(GPIO_PORTE, gpio_args->gpio_ddr);
            break;

        case('F'):
            gpio_setmode(GPIO_PORTF, gpio_args->gpio_ddr);
            break;

        case('G'):
            gpio_setmode(GPIO_PORTG, gpio_args->gpio_ddr);
            break;

        case('H'):
            gpio_setmode(GPIO_PORTH, gpio_args->gpio_ddr);
            break;

        case('J'):
            gpio_setmode(GPIO_PORTJ, gpio_args->gpio_ddr);
            break;

        case('K'):
            gpio_setmode(GPIO_PORTK, gpio_args->gpio_ddr);
            break;

        case('L'):
            gpio_setmode(GPIO_PORTL, gpio_args->gpio_ddr);
            break;
        default:
            break;
    }
        
    return;
}

static
void handler_gpio_set_pin(void* data)
{
        struct frame
    {
        uint8_t gpio_port;
        uint8_t gpio_pin;
    };
    
    struct frame *gpio_args = (struct frame *)data;

    UART_print(UART0,"function call :: handler_gpio_set_pin()\n");

    switch (gpio_args->gpio_port)
    {
        case('A'):
            gpio_setbit(GPIO_PORTA, gpio_args->gpio_pin);
            break;

        case('B'):
            gpio_setbit(GPIO_PORTB, gpio_args->gpio_pin);
            break;

        case('C'):
            gpio_setbit(GPIO_PORTC, gpio_args->gpio_pin);
            break;

        case('D'):
            gpio_setbit(GPIO_PORTD, gpio_args->gpio_pin);
            break;

        case('E'):
            gpio_setbit(GPIO_PORTE, gpio_args->gpio_pin);
            break;

        case('F'):
            gpio_setbit(GPIO_PORTF, gpio_args->gpio_pin);
            break;

        case('G'):
            gpio_setbit(GPIO_PORTG, gpio_args->gpio_pin);
            break;

        case('H'):
            gpio_setbit(GPIO_PORTH, gpio_args->gpio_pin);
            break;

        case('J'):
            gpio_setbit(GPIO_PORTJ, gpio_args->gpio_pin);
            break;

        case('K'):
            gpio_setbit(GPIO_PORTK, gpio_args->gpio_pin);
            break;

        case('L'):
            gpio_setbit(GPIO_PORTL, gpio_args->gpio_pin);
            break;
        default:
            break;
    }

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

    UART_print(UART0, "function call :: handler_gpio_get_pin()\n");
    switch (gpio_args->gpio_port)
    {
        case('A'):
            gpio_getbit(GPIO_PORTA, gpio_args->gpio_pin);
            break;

        case('B'):
            gpio_getbit(GPIO_PORTB, gpio_args->gpio_pin);
            break;

        case('C'):
            gpio_getbit(GPIO_PORTC, gpio_args->gpio_pin);
            break;

        case('D'):
            gpio_getbit(GPIO_PORTD, gpio_args->gpio_pin);
            break;

        case('E'):
            gpio_getbit(GPIO_PORTE, gpio_args->gpio_pin);
            break;

        case('F'):
            gpio_getbit(GPIO_PORTF, gpio_args->gpio_pin);
            break;

        case('G'):
            gpio_getbit(GPIO_PORTG, gpio_args->gpio_pin);
            break;

        case('H'):
            gpio_getbit(GPIO_PORTH, gpio_args->gpio_pin);
            break;

        case('J'):
            gpio_getbit(GPIO_PORTJ, gpio_args->gpio_pin);
            break;

        case('K'):
            gpio_getbit(GPIO_PORTK, gpio_args->gpio_pin);
            break;

        case('L'):
            gpio_getbit(GPIO_PORTL, gpio_args->gpio_pin);
            break;
        default:
            break;
    }

    return;
}

static
void handler_set_pwm_duty(void* data)
{
    UART_print(UART0,"function call :: handler_set_pwm_duty()\n");
    return;
}

static
void handler_echo_msg(void* data)
{
    UART_print(UART0,"function call :: handler_echo_msg()\n");
    return;
}

static
void handler_set_kp(void* data)
{
    UART_print(UART0,"function call :: handler_set_kp()\n");
    return;
}

static
void handler_set_ki(void* data)
{
    UART_print(UART0,"function call :: handler_set_ki()\n");
    return;
}

static
void handler_set_kd(void* data)
{
    UART_print(UART0,"function call :: handler_set_kd()\n");
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
                        UART_print(UART0, "IDLE state\n");
                    #endif
                    continue;

                case FETCHING:
                    ((uint8_t*)msg)[n++] = c;
                    if (n == sizeof(struct message)) state = TERM;
                    #ifdef TESTING
                        UART_print(UART0, "FETCHING state\n");
                    #endif
                    continue;

                case TERM:
                    if (c == TERMINATOR && !chksum((uint8_t *)msg, sizeof(*msg)))
                    {
                        state = STOP;
			n = 0;
                        #ifdef TESTING
                            UART_print(UART0, "Message TERM received and chksum passed.\n");
                        #endif
                            UART_putc(UART0, ACK);
			break;
                    } else {
                        state = IDLE;
                        #ifdef TESTING
                            UART_print(UART0, "Message TERM missed or chksum failed.\n");
                        #endif
                        UART_putc(UART0, NAK);
			break;
                    }
                
                case STOP:
                    #ifdef TESTING
                        UART_print(UART0, "STOP state reached.\n");
			return 0;
                    #endif
                    break;
                default:
                    #ifdef TESTING
                        UART_print(UART0, "exception: error scanning incoming characters\n");
			return -1;
                    #endif
                    break;
            }
        } else {
            if(++trials > SCAN_MAX_TRIALS) state = STOP;
            #ifdef TESTING
                UART_print(UART0, "no char returned from buffer.\n");
            #endif
            continue;
        }
    }
    return -1;
}

void dispatch(struct message *msg, handler_func_t table[])
{
    (*(table[msg->id]))(msg->data);
    return;
}

int8_t process_message(struct ringbuffer *buff)
{

    struct message msg;
    if (scan(buff, &msg) == 0){
    	dispatch(&msg,handler_table);
    	return 0;
    } else {
	return -1;
    }
}

void print_message(struct message *msg)
{
    char buffer[32];
    sprintf(buffer, "Message ID :\t\t%02X\n",msg->id);
    UART_print(UART0,buffer);
    UART_print(UART0, "Message Data :\t\t");
    for (int i = 0; i < MSG_SZ; i++) 
    {
    	sprintf(buffer,"%02X ",msg->data[i]);
    	UART_print(UART0, buffer);
    	UART_print(UART0, "\n");
    }

    sprintf(buffer, "Message Checksum :\t%02X\n",msg->chksum);
    UART_print(UART0,buffer);
    return;
}
