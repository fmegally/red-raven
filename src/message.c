#include "gpio.h"
#include "uart.h"
#include "message.h"
#include <stdio.h>

#define SCAN_MAX_TRIALS 20


/* this section is for the handler functions that correspond to each message type (ID)*/
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

    //uint8_t buff[12] = {0xAA, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x55};
    //UART_write(UART0, buff, 12);

        
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

	//uint8_t buff[12] = {0xAA, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x55};
	//UART_write(UART0, buff, 12);
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

	//uint8_t buff[12] = {0xAA, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x55};
	//UART_write(UART0, buff, 12);
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
    
	#ifdef TESTING	
	UART_print(UART0,"function call:handler_gpio_get_pin");
	#endif
	struct frame *gpio_args = (struct frame *)data;
	gpio_t* const  port = gpio_ports_list[gpio_args->gpio_port_ix];
	uint8_t res = gpio_getbit(port, gpio_args->gpio_pin);

	//uint8_t buff[12] = {0xAA, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x55};
	//UART_write(UART0, buff, 12);
	return res;
}

static
int8_t handler_set_pwm_duty(void* data)
{
	#ifdef TESTING	
    UART_print(UART0,"function call :: handler_set_pwm_duty()\n");
	#endif
    return 0;
}

static
int8_t handler_echo_msg(void* data)
{
	#ifdef TESTING	
	UART_print(UART0,"function call :: handler_echo_msg()\n");
	#endif
	return 0;
}

static
int8_t handler_set_kp(void* data)
{
	#ifdef TESTING	
    UART_print(UART0,"function call :: handler_set_kp()\n");
	#endif
    return 0;
}

static
int8_t handler_set_ki(void* data)
{
	#ifdef TESTING	
	UART_print(UART0,"function call :: handler_set_ki()\n");
	#endif
	return 0;
}

static
int8_t handler_set_kd(void* data)
{
	#ifdef TESTING	
    UART_print(UART0,"function call :: handler_set_kd()\n");
	#endif
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
int8_t scan(struct ringbuffer *buff, struct message *msg)
{
    uint8_t n = 0;
    enum sc_state state = IDLE;
    unsigned char c;

    //int trials = 0;
     
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
                        //UART_putc(UART0, ACK);
						continue;
                    } else {
                        state = IDLE;
						n = 0;
                        #ifdef TESTING
                        UART_print(UART0, "Message TERM missed or chksum failed.\n");
                        #endif
                        //UART_putc(UART0, NAK);
						continue;
                    }
                
                //case STOP:
                //    #ifdef TESTING
                //        UART_print(UART0, "STOP state reached.\n");
                //    #endif
			    //    return 0;
                    
                default:
                    #ifdef TESTING
                        UART_print(UART0, "exception: error scanning incoming characters\n");
                    #endif
			        return -1;
            }
        } else {
            //if(++trials > SCAN_MAX_TRIALS) state = STOP;
            #ifdef TESTING
//                UART_print(UART0, "no char returned from buffer.\n");
            #endif
            continue;
        }
    }
    return 0;
}

static
void msg_append_chksum(struct message* msg)
{
	uint8_t t[sizeof(struct message) - sizeof(uint8_t)];
	msg->chksum = chksum(t,sizeof(t));
	return;
}

int8_t dispatch(struct message *msg, handler_func_t table[])
{
	int8_t r = (*(table[msg->id]))(msg->data);
	return r;
}

int8_t process_message(struct ringbuffer *buff)
{
    struct message tmsg;
    int8_t r;
    if (scan(buff, &tmsg) == 0){
    	r = dispatch(&tmsg,handler_table);
	if( r == 0){
		tmsg.id = RESPONSE_SUCCESS;
		int8_t i;
		for(i = 0;i < MSG_SZ; i++) tmsg.data[i] = 0;
		msg_append_chksum(&tmsg);
		UART_putc(UART0,PREAMBLE);
		UART_write(UART0, (uint8_t*)&tmsg,10);
		UART_putc(UART0,TERMINATOR);
	} else {
		tmsg.id = RESPONSE_ERROR;
		int8_t i;
		for(i = 0;i < MSG_SZ; i++) tmsg.data[i] = 0;
		msg_append_chksum(&tmsg);
		UART_putc(UART0,PREAMBLE);
		UART_write(UART0, (uint8_t*)&tmsg, sizeof(tmsg));
		UART_putc(UART0,TERMINATOR);
	}
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

