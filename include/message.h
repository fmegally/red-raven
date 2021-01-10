#ifndef MESSAGE_H_
#define MESSAGE_H_

#include <stdint.h>
#include <stddef.h>
#include "ringbuffer.h"
#include "gpio.h"
#include "uart.h"
#include "chksum.h"

#define MSG_SZ              8
#define HANDLERS_TABLE_SZ   32
#define PREAMBLE            0xAAU
#define TERMINATOR          0x55U
#define ACK                 0x06U
#define NAK                 0x15U

#define ERROR_INVALID_PREAMBLE  -1
#define ERROR_FSM_FAULT         -2

struct message
{
	uint8_t id;
	uint8_t data[MSG_SZ];
	uint8_t chksum;
};

enum messgage_id {
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




typedef int16_t (*handler_func_t)(void *data);
extern handler_func_t handler_table[HANDLERS_TABLE_SZ];

enum sc_state {
	IDLE,
	FETCHING,
	TERM,
	STOP
};

int16_t process_message(struct ringbuffer *buff);
int16_t dispatch(struct message *msg, handler_func_t table[]);
void print_message(struct message *msg);

//void wrap_message(uint8_t id, const uint8_t *data);
//int8_t send_message(struct message *msg);

    
#endif
