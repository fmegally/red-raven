#ifndef PID_H_
#	define PID_H_

#include <stdint.h>

enum PID_parameter
{
	KP,
	KI,
	KD,
	DEAD_BAND,
	ANTI_WINDUP
};

struct PID
{
	uint8_t kp;
	uint8_t ki;
	uint8_t kd;

	int16_t err;
	int16_t err_;
	int16_t err__;

	int16_t i_err;
	int16_t d_err;

	int8_t deadband;
	int16_t contvar;
	int16_t setpnt;
	int16_t procvar;
};


int16_t PID_step(struct PID *pid, int16_t cv);
void PID_set_sp(struct PID *pid, int16_t sp);
void PID_init(struct PID *pid);
void PID_set_parameter(struct PID *pid, enum PID_parameter, int16_t val);

#endif
