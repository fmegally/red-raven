#include "gpio.h"
#include "uart.h"
#include "telegram.h"
#include <stdio.h>

#define SCAN_MAX_TRIALS 20


/* this section is for the handler functions that correspond to each telegram type (ID)*/
/* should probably me moved to its own handler files                                  */

unsigned char atord(char c)
{
	if (c >= 'A' && c <= 'Z')
	{
		return (c - 'A') + 1;
	} else if (c >= 'a' && c <= 'z')
	{
		return (c - 'a') + 1;
	} else {
		return 0;
	}
}

static
int8_t handler_gpio_set_mode(void* data)
{
    struct frame
    {
        uint8_t gpio_port;
        uint8_t gpio_ddr;
    };
	#ifdef TESTING	
	UART_print(UART0,"function call:handler_gpio_set_mode");
	#endif
	struct frame *gpio_args = (struct frame *)data;
	gpio_t* const port = gpio_ports_list[gpio_args->gpio_port];
	gpio_setmode(port, gpio_args->gpio_ddr);
        
    return 0;
}

static
int8_t handler_gpio_clr_pin(void* data)
{
	struct frame
	{
		uint8_t gpio_port_ix;
		uint8_t gpio_pin;
	};
    
	struct frame *gpio_args = (struct frame *)data;

	#ifdef TESTING	
	UART_print(UART0,"function call:handler_gpio_clr_pin");
	#endif
	gpio_t* const port = gpio_ports_list[gpio_args->gpio_port_ix];
	gpio_clrbit(port, gpio_args->gpio_pin);

	return 0;
}

static
int8_t handler_gpio_set_pin(void* data)
{
	struct frame
	{
		uint8_t gpio_port_ix;
		uint8_t gpio_pin;
	};
    
	struct frame *gpio_args = (struct frame *)data;

	#ifdef TESTING	
	UART_print(UART0,"function call:handler_gpio_set_pin");
	#endif
	gpio_t* const port = gpio_ports_list[gpio_args->gpio_port_ix];
	gpio_setbit(port, gpio_args->gpio_pin);

	return 0;
}

static
int8_t handler_gpio_get_pin(void* data)
{
	struct frame
	{
		uint8_t gpio_port_ix;
		uint8_t gpio_pin;
	};
    
	struct frame *gpio_args = (struct frame *)data;
	gpio_t* const  port = gpio_ports_list[gpio_args->gpio_port_ix];
	uint8_t res = gpio_getbit(port, gpio_args->gpio_pin);

	return res;
}

static
int8_t handler_set_pwm_duty(void* data)
{
	return 0;
}

static
int8_t handler_echo_msg(void* data)
{
	return 0;
}

static
int8_t handler_set_kp(void* data)
{
	return 0;
}

static
int8_t handler_set_ki(void* data)
{
	return 0;
}

static
int8_t handler_set_kd(void* data)
{
	return 0;
}

/* end of handlers implementation */

handler_func_t handler_table[HANDLERS_TABLE_SZ] = 
{
	[1] = handler_gpio_set_mode,
	handler_gpio_set_pin,
	handler_gpio_clr_pin,
	handler_gpio_get_pin,
	handler_set_pwm_duty,
	handler_echo_msg,
	handler_set_kp,
	handler_set_ki,
	handler_set_kd
};

static
int8_t scan(struct ringbuffer *buff, struct telegram *tg)
{
    uint8_t n = 0;
    enum sc_state state = IDLE;
    unsigned char c;
     
    while(state != HALT)
    {
        if(rb_getc(&c, buff))
        {
            switch(state)
            {
                case IDLE:
                    if(c == SD) state = FETCHING;
                    continue;

                case FETCHING:
                    ((uint8_t*)msg)[n++] = c;
                    if (n == sizeof(struct telegram)) state = TERM;
                    continue;

                case TERM:
                    if (c == ED && !chksum((uint8_t *)tg, sizeof(*msg)))
                    {
                        state = STOP;
			n = 0;
			continue;
                    } else {
                        state = IDLE;
			n = 0;
			continue;
                    }
                
                default:
			return -1;
            }
        } else {
		continue;
        }
    }
    return 0;
}

static
void telegram_append_chksum(struct telegram* tg)
{
	tg->chksum = chksum((uint8_t*)tg, sizeof(struct message)-1);
	return;
}

int8_t dispatch(struct telegram *tg, handler_func_t table[])
{
	int8_t r = (*(table[tg->id]))(tg->data);
	return r;
}

int8_t process_telegram(struct ringbuffer *buff)
{
    struct telegram ttg;
    int8_t r;
    if (scan(buff, &ttg) == 0){
    	r = dispatch(&ttg,handler_table);
	if( r == 0){
		ttg.id = RESPONSE_SUCCESS;
		int8_t i;
		for(i = 0;i < MSG_SZ; i++) ttg.data[i] = 0;
		msg_append_chksum(&ttg);
		UART_putc(UART0,SD);
		UART_write(UART0, (uint8_t*)&ttg,10);
		UART_putc(UART0,ED);
	} else {
		ttg.id = RESPONSE_ERROR;
		int8_t i;
		for(i = 0;i < MSG_SZ; i++) ttg.data[i] = 0;
		msg_append_chksum(&ttg);
		UART_putc(UART0,SD);
		UART_write(UART0, (uint8_t*)&ttg, sizeof(ttg));
		UART_putc(UART0,ED);
	}
    	return 0;
    } else {
	return -1;
    }
}

void print_telegram(struct telegram *ttg)
{
    char buffer[32];
    sprintf(buffer, "Message ID :\t\t%02X\n",ttg->id);
    UART_print(UART0,buffer);
    UART_print(UART0, "Message Data :\t\t");

    for (int i = 0; i < MSG_SZ; i++) 
    {
    	sprintf(buffer,"%02X ",ttg->data[i]);
    	UART_print(UART0, buffer);
    	UART_print(UART0, "\n");
    }

    sprintf(buffer, "Message Checksum :\t%02X\n",ttg->chksum);
    UART_print(UART0,buffer);
    return;
}
