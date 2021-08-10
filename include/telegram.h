#ifndef MESSAGE_H_
#define MESSAGE_H_

#include <stdint.h>
#include <stddef.h>
#include "ringbuffer.h"
#include "gpio.h"
#include "uart.h"
#include "chksum.h"

#define TELEGRAM_SZ         8
#define SDB                 0xAAU  //Start Delimiter for fixed length telegram
#define EDB                 0x55U  //End Delimiter

#define ERROR_INVALID_SD        -1
#define ERROR_INVALID_ID        -2
#define ERROR_FAILED_CHKSUM     -4
#define ERROR_INVALID_CHKSUM    -8
#define ERROR_INVALID_ED       -16

struct telegram {
	uint8_t id;
	uint8_t data[TELEGRAM_SZ];
	uint8_t chksum;
};

enum telegram_id {
	NULL_MSG,
	ECHO_MSG,
	ACK=0x06,
	NAK=0x15,
	ID_TABLE_SIZE
};

typedef int8_t (*handler_func_t)(void *data);
extern handler_func_t handler_table[ID_TABLE_SIZE];

enum sc_state {
	SD,
	ID,
	PDU,
	FCS,
	ED,
	HALT	
};

int8_t process_telegram(struct telegram *tg);
int8_t dispatch(struct telegram *tg, handler_func_t table[]);
void print_telegram(struct telegram *tg);

//void wrap_message(uint8_t id, const uint8_t *data);
//int8_t send_message(struct message *msg);

    
#endif
