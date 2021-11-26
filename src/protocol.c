#include <stdio.h>
#include <stdlib.h>
#include "gpio.h"
#include "uart.h"
#include "protocol.h"
#include "fifo.h"

enum protocol_state_e {
	ACCEPT_SD,
	ACCEPT_FLAGS,
	ACCEPT_PDU,
	ACCEPT_FCS,
	ACCEPT_ED,
	HALT,
        PROTOCOL_STATE_TBL_SZ
};

struct protocol
{
        enum protocol_state_e state;
        uint8_t n_bytes;
        uint8_t flags;
        uint8_t errors;
        uint8_t *dst;
        uart_t *dev;
        fifo_t *rx_buff;
        fifo_t *tx_buff;
};

typedef int8_t (*handler_func_t)(protocol_t *p, void *data);

static int8_t handler_SD (protocol_t *p, void* input);
static int8_t handler_FLAGS (protocol_t *p, void* input);
static int8_t handler_PDU (protocol_t *p, void* input);
static int8_t handler_FCS (protocol_t *p, void* input);
static int8_t handler_ED (protocol_t *p, void* input);

protocol_t* create_protocol(uart_t *u)
{
        protocol_t *p = (protocol_t *) malloc(sizeof(protocol_t));
        p->n_bytes = 0;
        p->state = ACCEPT_SD;
        p->dst = NULL;
        p->errors = 0;
        p->dev = u;
        p->rx_buff = NULL;
//        p->rx_buff = (fifo_t *) malloc(sizeof(fifo_t));
//        fifo_init(p->rx_buff, 4 * (FRAME_SZ + SD_SZ + ED_SZ)):
        p->tx_buff = NULL;
        return p;
}

void protocol_attach_buffers(protocol_t *p, fifo_t *rx, fifo_t *tx)
{
        p->rx_buff = rx;
        p->tx_buff = NULL;
        return;
}

handler_func_t handlers[PROTOCOL_STATE_TBL_SZ] = { [ACCEPT_SD] = handler_SD,
                                                   [ACCEPT_FLAGS] = handler_FLAGS,
                                                   [ACCEPT_PDU]  = handler_PDU,
                                                   [ACCEPT_FCS] = handler_FCS,
                                                   [ACCEPT_ED] = handler_ED
                                                 };

inline
static void send_ack(uart_t *u)
{
        uint8_t seq[4] = {SD,ACK,~ACK,ED};
        UART_write_blocking(u, seq, 4);
        return;
}
    
inline
static void send_nak(uart_t *u)
{
        uint8_t seq[4] = {SD,NAK,~NAK,ED};
        UART_write_blocking(u, seq, 4);
        return;
}



static int8_t handler_SD (protocol_t *p, void* input)
{
        if(*((uint8_t*) input) == SD) {
                p->state =  ACCEPT_FLAGS;
                return 0;
        } else {
                p->state =  ACCEPT_SD;
                return 1;
        }
}

static int8_t handler_FLAGS (protocol_t *p, void* input)
{
        if (*((uint8_t*) input) & (1 << 0)) {
                p->state = ACCEPT_FCS;
                p->flags = *((uint8_t*) input;
                return 0;
        } else {
                p->state =  ACCEPT_PDU;
                return 0;
        }
}

static int8_t handler_PDU (protocol_t *p, void* input)
{
        p->dst[p->n_bytes] = *((uint8_t*) input);
        p->n_bytes++;

        if (p->n_bytes == MESSAGE_SZ){
                p->state =  ACCEPT_FCS;
        }
        return 0;
}

static int8_t handler_FCS (protocol_t *p, void* input)
{
        uint8_t c = *((uint8_t *)input);

        if (verify_chksum8(p->dst, sizeof(message_t), c) == 0){
                p->state =  ACCEPT_ED;
                return 0;
        } else {
                p->state =  HALT;
                return 2;
        }
}

static int8_t handler_ED(protocol_t *p,  void* input)
{
        if(*((uint8_t*) input) == ED) {
                p->state =  HALT;
                return 0;
        } else {
                p->n_bytes=0;
                p->state =  HALT;
                return  4;
        }
}

int8_t fetch_message(protocol_t *p, message_t *dst, uint8_t *flags)
{
        p->dst = (uint8_t*) dst;
        unsigned char c;

        while(1) {
                p->state = ACCEPT_SD;
                p->flags = 0;
                p->errors = 0;
                p->n_bytes = 0;
        
                while(p->state != HALT) {
                        if(fifo_getc(&c, p->rx_buff)) {
                                p->errors |= handlers[p->state](p, &c);
                        } else {
                                continue;
                        }
                }

                if (!p->errors){
                        send_ack(p->dev);
                        if (flags != NULL) *flags = p->flags;
                        return 0;
                } else {
                        send_nak(p->dev);
                        continue;
                }
        }
}


void send_message(protocol_t *p, message_t *src, uint8_t flags)
{
        uint8_t c = chksum8((uint8_t*)src, sizeof(message_t));
        uint8_t 
        do{
                UART_putc(p->dev, SD);
                UART_putc(p->dev, flags);
                UART_write_blocking(p->dev, (uint8_t*) src, sizeof(message_t));
                UART_putc(p->dev, c);
                UART_putc(p->dev, ED);
                fetch_message
        return;
}

void print_message(message_t *src, uart_t *dst)
{
        char buffer[32];
        sprintf(buffer, "Message ID :\t\t%02X\n",src->id);
        UART_prints(dst, buffer);
        UART_prints(dst, "Message Data :\t\t");

        for (int i = 0; i < MESSAGE_SZ; i++) 
        {
                sprintf(buffer,"%02X ",src->data[i]);
                UART_prints(dst, buffer);
                UART_prints(dst, "\n");
        }

        return;
}
