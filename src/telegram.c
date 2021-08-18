#include "gpio.h"
#include "uart.h"
#include "telegram.h"
#include <stdio.h>

#define SCAN_MAX_TRIALS 20

enum gpio_cmd_list
{
        SET_DDR=0x01,
        SET_PORT,
        GET_PORT,
        SET_PIN,
        CLR_PIN
};

enum adc_cmd_list
{
        ENABLE_ADC=0x01,
        SET_CHANNEL,
        GET_CONV_VALUE
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
                case CONFIRM:
                case GPIO_CMD:
                case GPIO_REPLY:
                case ADC_CMD:
                case EXCEPTION:
                case NAK:
                        return 1;
                default:
                        return 0;
                }
}

static
int8_t handler_adc_cmd(void* data)
{
        struct adc_cmd
        {
                uint8_t cmd; 
                uint8_t value;
        };

        struct adc_cmd *args = (struct adc_cmd *) data;

        switch(args->cmd){
                case ENABLE_ADC:
                        adc_enable (ADC0);
                        adc_select_refv (ADC0, args->value);
                        break; 
                
                case SET_CHANNEL:
                        adc_select_ch (ADC0, args->value);
                        break;

                case GET_CONV_VALUE:
                        uint16_t tmp = adc_read_ch_blocking (ADC0);

                        struct telegram reply = {ADC_REPLY,{(tmp>>8) & 0xFF,tmp & 0xFF}};

                        reply.chksum = chksum((uint8_t*)&reply,sizeof(struct telegram) - 1);

                        UART_putc(UART0, SD);
                        UART_write(UART0, (uint8_t*)&reply,sizeof(struct telegram));
                        UART_putc(UART0, ED);

                        break;

                default:
                        break;
        }

        return 0;
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
                        struct telegram reply = {GPIO_REPLY,{port_value}};
                        reply.chksum = chksum((uint8_t*)&reply,sizeof(struct telegram) - 1);
                        UART_putc(UART0, SD);
                        UART_write(UART0, (uint8_t*)&reply,sizeof(struct telegram));
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

                default:
                        break;

        }
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
        [GPIO_CMD] = handler_gpio_cmd,
        [ADC_CMD]  = handler_adc_cmd
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
                                if (n == TELEGRAM_SZ){
                                        state = FSM_FCS;
                                }
                                break;

                        case FSM_FCS:
                                tg->chksum = c;
                                if (chksum((uint8_t*)tg, sizeof(struct telegram)) == 0){
                                        state = FSM_ED;
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
