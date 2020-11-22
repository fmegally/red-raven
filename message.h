#ifndef H_MESSAGE
#define H_MESSAGE

#include <stdint.h>
#include "ringbuffer.h"

#define MSG_SZ 8;
#define CALLBACK_TABLE_SZ 64;

struct message
{
	uint8_t id;
	uint8_t data[MSG_SZ];
	uint8_t chksum;
};

enum messgage_id {
    NULL_MSG,
    GPIO_SET_PIN_MODE,
    GPIO_SET_PIN_OUT,
    GPIO_READ_PIN,
    SET_PWM_DUTY_T0_A,
    SET_PWM_DUTY_T0_B,
    SET_PWM_DUTY_T1_A,
    SET_PWM_DUTY_T1_B,
    SET_PWM_DUTY_T1_C,
    ECHO_MSG,
    PID_SET_KP,
    PID_SET_KI,
    PID_SET_KD
};

void (*callback_func[CALLBACK_TABLE_SZ])(uint8_t *data);

enum sc_state {
    IDLE,
    FETCHING,
    CHECKSUM
};

void scan(struct ringbuffer *buff, struct message *msg);
void dispatch(struct message *msg);
    
#endif
