#include "gpio.h"
#include "uart.h"
#include "telegram.h"
#include <stdio.h>

#define SCAN_MAX_TRIALS 20

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

/*
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
*/


static
int8_t handler_echo_msg(void* data)
{
        return 0;
}

/* end of handlers implementation */

handler_func_t tlgrm_handler_table[ID_TABLE_SIZE] = 
    {
        [ECHO_MSG] = handler_echo_msg
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
