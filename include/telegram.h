#ifndef MESSAGE_H_
#define MESSAGE_H_

#include <stdint.h>
#include <stddef.h>
#include "ringbuffer.h"
#include "gpio.h"
#include "uart.h"
#include "chksum.h"

#define TELEGRAM_SZ         8
#define HANDLERS_TABLE_SZ   32
#define SD1                 0xAAU  //Start Delimiter
#define ED                  0x55U  //End Delimiter
#define ACK                 0x06U
#define NAK                 0x15U

#define ERROR_INVALID_SD        -1
#define ERROR_FSM_FAULT         -2

struct telegram
{
	uint8_t id;
	uint8_t data[MSG_SZ];
	uint8_t chksum;
};

enum telegram_id {
	NULL_MSG,
	GPIO_SET_MODE,
	GPIO_WRITE_PORT,
	GPIO_READ_PORT,
	GPIO_SET_PIN,
	GPIO_GET_PIN,
	GPIO_FLIP_PIN,
	SET_PWM_DUTY,
	ECHO_MSG,
	PID_SET_KP,
 	PID_SET_KI,
	PID_SET_KD,
	RESPONSE_SUCCESS,
	RESPONSE_ERROR
};

typedef int8_t (*handler_func_t)(void *data);
extern handler_func_t handler_table[HANDLERS_TABLE_SZ];

enum sc_state {
	SD,
	ID,
	PDU,
	FCS,
	ED,
	HALT	
};

int8_t process_telegram(struct ringbuffer *buff);
int8_t dispatch(struct telegram *tg, handler_func_t table[]);
void print_telegram(struct telegram *tg);

//void wrap_message(uint8_t id, const uint8_t *data);
//int8_t send_message(struct message *msg);

    
#endif
