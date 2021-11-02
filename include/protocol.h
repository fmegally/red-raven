#ifndef PROTOCOL_H_
#define PROTOCOL_H_

#include <stdint.h>
#include <stddef.h>
#include "fifo.h"
#include "uart.h"
#include "chksum.h"

#define ID_SZ              1 * sizeof(uint8_t)
#define MESSAGE_SZ        8 * sizeof(uint8_t)
#define PDU_SZ             ID_SZ + MESSAGE_SZ

#define FLAGS_SZ           1 * sizeof(uint8_t)
#define FCS_SZ             1 * sizeof(uint8_t)
#define SD_SZ             1 * sizeof(uint8_t)
#define ED_SZ             1 * sizeof(uint8_t)
#define FRAME_SZ        FLAGS_SZ + PDU_SZ + FCS_SZ

#define ERROR_INVALID_SD        -1
#define ERROR_INVALID_ID        -2
#define ERROR_FAILED_CHKSUM     -4
#define ERROR_INVALID_CHKSUM    -8
#define ERROR_INVALID_ED       -16

struct message {
	uint8_t id;
	uint8_t data[TELEGRAM_SZ];
};

typedef struct message message_t;

enum message_id_e {
	NULL_MSG,
	ECHO_MSG,
	GPIO_CMD=0x08,
	GPIO_REPLY=0x09,
        ADC_CMD=0x0A,
        ADC_REPLY=0x0B,
	//Insert new IDs here. IDs
	MSG_ID_TBL_SIZE
};

typedef struct protocol protocol_t ;

protocol_t* create_protocol(uart_t *dev);
int8_t fetch_message(struct protocol_t *p, struct message_t *dst);
int8_t send_message(struct protocol_t *p, struct message_t *src);
void print_message(struct message_t *src, uart_t *dst);
    
#endif
