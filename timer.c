#include "timer.h"
#include "bits.h"

void timer_set_mode (tcnt_16b_t* timer, enum tcnt_modes mode)
{
    uint8_t msk;

    msk = (1<<WGM10) | (1<<WGM11);
    timer->TCCRnA = (timer->TCCRnA & ~msk) | (mode & msk);

    msk = (1<<WGM12) | (1<<WGM13);
    timer->TCCRnB = (timer->TCCRnB & ~msk) | (mode & msk);
	return;
}

void timer_start (tcnt_16b_t* timer, enum tcnt_clk_src)
{
    msk = ( 
	return;
}

void timer_stop (tcnt_16b_t* timer)
{
	return;
}

void timer_get_input_capture (tcnt_16b_t* timer, uint16_t *dst)
{
	return;
}

void timer_set_pwm_dc (tcnt_16b_t* timer, uint16_t duty_cycle)
{
	return;
}

void timer_set_pwm_freq (tcnt_16b_t* timer, uint16_t freq)
{
	return;
}

void timer_udelay (tcnt_16b_t * timer, uint16_t usec)
{
	return;
}

