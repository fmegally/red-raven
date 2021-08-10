#include "gpio.h"
#include "uart.h"
#include "telegram.h"
#include <stdio.h>

#define SCAN_MAX_TRIALS 20

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

/* this section is for the handler functions that correspond to each telegram type (ID)*/
/* should probably me moved to its own handler files                                  */


static
int is_valid_id(int id)
{
    switch(id){
        case NULL_MSG:
        case GPIO_SET_MODE:
        case GPIO_WRITE_PORT:
        case GPIO_READ_PORT:
        case GPIO_SET_PIN:
        case GPIO_GET_PIN:
        case GPIO_FLIP_PIN:
        case SET_PWM_DUTY:
        case ECHO_MSG:
        case ACK:
        case NAK:
            return 1;
        default:
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
int8_t handler_echo_msg(void* data)
{
    return 0;
}

/* end of handlers implementation */

handler_func_t handler_table[ID_TABLE_SIZE] = 
    {
        [ECHO_MSG] = handler_gpio_set_mode,
        [GPIO_SET_PIN]  = handler_gpio_set_pin,
    };

static
int8_t scan_telegram(struct ringbuffer *src, struct telegram *tg)
{
    enum sc_state state = SD;
    uint8_t type; 
    uint8_t n = 0;
    unsigned char c;
     
    while(state != HALT) {
        if(rb_getc(&c, src)) {
            switch(state) {
            case SD:
                if(c == SDB) {
                    state = ID;
                    break;
                } else {
                    return -1;
                }
            case ID:
                if(is_valid_id(c)){
                    tg->id = c;
                    state = PDU;
                    break;
                } else {
                    return -2;
                }
            case PDU:
                tg->data[n++] = c;
                if (n == TELEGRAM_SZ) state = FCS;
                break;
            case FCS:
                tg->chksum = c;
                if (chksum((uint8_t*)tg, sizeof(struct telegram)) == 0){
                    state = ED;
                    break;
                } else {
                    return -4;
                }
            case ED:
                if(c == ED) {
                    state = HALT;
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
void telegram_append_chksum(struct telegram* tg)
{
    tg->chksum = chksum((uint8_t*)tg, sizeof(struct telegram)-1);
    return;
}

int8_t dispatch(struct telegram *tg, handler_func_t table[])
{
    int8_t r = (*(table[tg->id]))(tg->data);
    return r;
}

static
void send_ack_tlgrm(uart_t *u)
{
    uint8_t seq[2+TELEGRAM_SZ+2] = {[0]=SDB,ACKB,[1+TELEGRAM_SZ]=~ACKB,EDB};
    UART_write(u, seq, 2+TELEGRAM_SZ+2);
    return;
}
    
static
void send_nak_tlgrm(uart_t *u)
{
    uint8_t seq[2+TELEGRAM_SZ+2] = {[0]=SDB,NAKB,[1+TELEGRAM_SZ]=~NAKB,EDB};
    UART_write(u, seq, 2+TELEGRAM_SZ+2);
    return;
}

int8_t fetch_telegram(struct rinbuffer *src,struct telegram *tg)
{
    int8_t r = scan(src,tg);
    if( r == 0){
        send_ack_tlgrm(UART0);
        return 0;
    } else {
        send_nak_tlgrm(UART0);
        return -1;
    }
}

void print_telegram(struct telegram *ttg)
{
    char buffer[32];
    sprintf(buffer, "Message ID :\t\t%02X\n",ttg->id);
    UART_print(UART0,buffer);
    UART_print(UART0, "Message Data :\t\t");

    for (int i = 0; i < TELEGRAM_SZ; i++) 
    {
        sprintf(buffer,"%02X ",ttg->data[i]);
        UART_print(UART0, buffer);
        UART_print(UART0, "\n");
    }

    sprintf(buffer, "Message Checksum :\t%02X\n",ttg->chksum);
    UART_print(UART0,buffer);
    return;
}
