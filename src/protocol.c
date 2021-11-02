#include <stdio.h>

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


typedef int8_t (*handler_func_t)(void *data);

static int8_t handler_SD (protocol_t *p, void* input);
static int8_t handler_FLAGS (protocol_t *p, void* input):
static int8_t handler_PDU (protocol_t *p, void* input);
static int8_t handler_FCS (protocol_t *p, void* input);
static int8_t handler_ED (protocol_t *p, void* input);

void protocol_init(protocol_t *p, uart_t *u)
{
        p->nbytes = 0;
        p->sstate = ACCEPT_SD;
        p->dst = NULL;
        p->err_flags = 0;
        p->port = u;
        p->rx_buff = NULL;
        p->tx_buff = NULL;
        return;
}

handler_func_t handlers[SCANNER_STTBL_SZ] = {   [ACCEPT_SD] = handler_SD,
                                                [ACCEPT_FLAGS] = handler_FALGS,
                                                [ACCEPT_PDU]  = handler_PDU
                                                [ACCEPT_FCS] = handler_FCS,
                                                [ACCEPT_ED] = handler_ED
                                            };

static void send_ack_message(uart_t *u)
{
        uint8_t seq[4] = {SD,ACK,~ACK,ED};
        UART_write(u, seq, 4);
        return;
}
    
static void send_nak_message(uart_t *u)
{
        uint8_t seq[4] = {SD,NAK,~NAK,ED};
        UART_write(u, seq, 4);
        return;
}

static int8_t handler_SD (protocol_t *p, void* input)
{
        if(*((uint8_t*) input) == SD) {
                p->state =  ACCEPT_FLAGS;
                return 0;
        } else {
                p->state =  ACCEPT_SD;
                return -1;
        }
}

static int8_t handler_FLAGS (protocol_t *p, void* input)
{
        if (*((uint8_t*) input) & (1 << 0)) {
                p->state = ACCEPT_FCS;
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

        if (protocol.n_bytes == TELEGRAM_SZ){
                p->state =  ACCEPT_FCS;
        }
        return 0;
}

static int8_t handler_FCS (protocol_t *p, void* input)
{
        protocol.buffer[FRAME_SZ-1] = *((uint8_t *)input);
        if (chksum(, sizeof(message_t)) == 0){
                p->state =  ACCEPT_ED;
                return 0;
        } else {
                send_nak_tlgrm(p->port);
                p->state =  ACCEPT_SD;
                return -1;
        }
}

static int8_t handler_ED(protocol_t *p,  void* input)
{
        if(*((uint8_t*) input) == ED) {
                send_ack_tlgrm(p->port);
                p->state =  HALT;
                return 0;
        } else {
                protocol.n_bytes=0;
                send_nak_tlgrm(p->port);
                p->state =  ACCEPT_SD;
                return  -1;
        }
}

int8_t fetch_message(protocol_t *p, message_t *dst)
{
        p->state = ACCEPT_SD;
        p->n_bytes = 0;
        p->dst = dst;

        unsigned char c;
        while(p->state != HALT) {
                if(fifo_getc(&c, p->rx_buff)) {
                        p->state = handlers[p->state](p, &c);
                } else {
                        continue;
                }
        }
        return 0;
}

void print_message(message_t *src, uart_t *dst)
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
