#include "gpio.h"
#include "uart.h"
#include "telegram.h"
#include <stdio.h>

#define SCAN_MAX_TRIALS 20

enum gpio_cmds
{
	SET_DDR,
	SET_PORT,
	GET_PORT,
	SET_PIN,
	CLR_PIN
};

unsigned char atord(char c)
{
        if (c >= 'A' && c <= 'Z'){
                return (c - 'A') + 1;
        } else if (c >= 'a' && c <= 'z') {
                return (c - 'a') + 1;
        } else {
                return 0;
        }
}

/* this section is for the handler functions that correspond to each telegram type (ID)*/
/* should probably me moved to its own handler files                                  */

static
int is_valid_id(int id)
{
        switch(id){
                case NULL_MSG:
                case ECHO_MSG:
                case ACK:
                case NAK:
                        return 1;
                default:
                        return 0;
                }
}

static
int8_t handler_gpio_cmd(void* data)
{
    struct gpio_cmd
    {
	uint8_t cmd;
        uint8_t port;
	uint8_t pin;
        uint8_t value;
    };

    #ifdef TESTING  
    UART_print(UART0,"function call:handler_gpio_cmd");
    #endif
    struct gpio_cmd *args = (struct gpio_cmd *)data;
    gpio_t* const port = gpio_ports_list[args->port];
    switch(args->cmd){
	case SET_DDR:
		gpio_setmode(port,args->value);
		break;

	case SET_PORT:
		gpio_pwrite(port,args->value);
		break;

	case GET_PORT:
		uint8_t port_value = gpio_pread(port);
		struct telegram	reply = {GPIO_REPLY,{port_value}};
		reply.chksum = chksum(&reply,sizeof(struct telegram) - 1);
		UART_putc(UART0, SD);
		UART_write((uint8_t*)&reply,sizeof(struct telegram));
		UART_putc(UART0, ED);
		break;

	case SET_PIN:
		if( args->pin < 8){
			gpio_setbit(port,args->pin);
			break;
		} else {
			break;
		}

	case CLR_PIN:
		if( args->pin < 8){
			gpio_clrbit(port,args->pin);
			break;
		} else {
			break;
		}

	case default:
		break;


    return 0;
}


static
int8_t handler_echo_msg(void* data)
{
        return 0;
}

/* end of handlers implementation */

handler_func_t tlgrm_handler_table[ID_TABLE_SIZE] = 
    {
        [ECHO_MSG] = handler_echo_msg,
	[GPIO_CMD] = handler_gpio_cmd
    };

int8_t fetch_tlgrm(struct ringbuffer *src, struct telegram *tg)
{
        enum sc_state state = FSM_SD;
        uint8_t n = 0;
        unsigned char c;
     
        while(state != FSM_HALT) {
                if(rb_getc(&c, src)) {
                        switch(state) {

                        case FSM_SD:
                        if(c == SD) {
                                state = FSM_ID;
                                break;
                        } else {
                                return -1;
                        }

                        case FSM_ID:
                        if(is_valid_id(c)){
                                tg->id = c;
                                state = FSM_PDU;
                                break;
                        } else {
                                return -2;
                        }

                        case FSM_PDU:
                                tg->data[n++] = c;
                                if (n == TELEGRAM_SZ) state = FSM_FCS;
                                break;

                        case FSM_FCS:
                                tg->chksum = c;
                                if (chksum((uint8_t*)tg, sizeof(struct telegram)) == 0){
                                        state = ED;
                                        break;
                                } else {
                                        return -4;
                                }

                        case FSM_ED:
                                if(c == ED) {
                                        state = FSM_HALT;
                                        break;
                                } else {
                                        return -8;
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
void calc_telegram_chksum(struct telegram* tg)
{
        tg->chksum = chksum((uint8_t*)tg, sizeof(struct telegram)-1);
        return;
}

int8_t dispatch(struct telegram *tg, handler_func_t table[])
{
        int8_t r = (*(table[tg->id]))(tg->data);
        return r;
}

void send_ack_tlgrm(uart_t *u)
{
        uint8_t seq[2+TELEGRAM_SZ+2] = {[0]=SD,ACK,[1+TELEGRAM_SZ]=~ACK,ED};
        UART_write(u, seq, 2+TELEGRAM_SZ+2);
        return;
}
    
void send_nak_tlgrm(uart_t *u)
{
        uint8_t seq[2+TELEGRAM_SZ+2] = {[0]=SD,NAK,[1+TELEGRAM_SZ]=~NAK,ED};
        UART_write(u, seq, 2+TELEGRAM_SZ+2);
        return;
}

void print_tlgrm(struct telegram *src, uart_t *dst)
{
        char buffer[32];
        sprintf(buffer, "Message ID :\t\t%02X\n",src->id);
        UART_print(dst, buffer);
        UART_print(dst, "Message Data :\t\t");

        for (int i = 0; i < TELEGRAM_SZ; i++) 
        {
                sprintf(buffer,"%02X ",src->data[i]);
                UART_print(dst, buffer);
                UART_print(dst, "\n");
        }

        sprintf(buffer, "Message Checksum :\t%02X\n",src->chksum);
        UART_print(dst, buffer);
        return;
}
