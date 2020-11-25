#ifndef H_MESSAGE
#define H_MESSAGE

#include <stdint.h>
#include "ringbuffer.h"
#include "uart.h"
#include "chksum.h"

#define MSG_SZ              8
#define CALLBACK_TABLE_SZ   64
#define PREAMBLE            0xAA
#define TERMINATOR          0x55
#define ACK                 0x06
#define NAK                 0x15


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

void callback_gpio_set_mode(void* data);
void callback_gpio_set_pin(void* data);
void callback_gpio_get_pin(void* data);
void callback_set_pwm_duty(void* data);
void callback_echo_msg(void* data);
void callback_set_kp(void* data);
void callback_set_ki(void* data);
void callback_set_kd(void* data);

extern void (*callback_func[CALLBACK_TABLE_SZ])(void *data);

enum sc_state {
    IDLE,
    FETCHING,
    TERM,
    STOP
};

int8_t scan(struct ringbuffer *buff, struct message *msg);
void dispatch(struct message *msg);
    
#endif
