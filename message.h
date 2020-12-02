#ifndef MESSAGE_H_
#define MESSAGE_H_

#include <stdint.h>
#include <stddef.h>
#include "ringbuffer.h"
#ifndef TESTING
#include "gpio.h"
#include "uart.h"
#endif // TESTING
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
    GPIO_SET_PIN,
    GPIO_GET_PIN,
    SET_PWM_DUTY,
    ECHO_MSG,
    PID_SET_KP,
    PID_SET_KI,
    PID_SET_KD
};


typedef void (*handler_func_t)(void *data);
extern handler_func_t handler_table[HANDLERS_TABLE_SZ];

enum sc_state {
    IDLE,
    FETCHING,
    TERM,
    STOP
};

int8_t process_message(struct ringbuffer *buff);
void dispatch(struct message *msg, handler_func_t table[]);
void print_message(struct message *msg);
    
#endif
