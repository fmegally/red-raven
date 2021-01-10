#include "timer.h"
#include "bits.h"

void timer16_set_mode (tcnt_16b_t* timer, enum tcnt_modes mode)
{
    uint8_t msk;

    msk = (1<<WGM10) | (1<<WGM11);
    timer->TCCRnA = (timer->TCCRnA & ~msk) | (mode & msk);

    msk = (1<<WGM12) | (1<<WGM13);
    timer->TCCRnB = (timer->TCCRnB & ~msk) | (mode & msk);
	return;
}

void timer16_start (tcnt_16b_t* timer, enum tcnt_clk_src)
{
	msk = (1<<CS12) | (1<<CS11) | (1<<CS10);
    	timer->TCCRnB = (timer->TCCRnA & ~msk) | (tcnt_clk_src & msk);
	return;
}

void timer16_stop (tcnt_16b_t* timer)
{
	msk = (1<<CS12) | (1<<CS11) | (1<<CS10);
    	timer->TCCRnB &= (timer->TCCRnA & ~msk) 
	return;
}

void timer16_get_input_capture (tcnt_16b_t* timer, uint16_t *dst)
{
	dst = time->ICRn;	
	return;
}

void timer16_set_ocr (tcnt_16b_t* timer, uint16_t ocr, enum tcnt_occ channel)
{
	switch(channel){
		case Chnl_A:
			timer->OCRnA = ocr;
			break;
		case Chnl_B:
			timer->OCRnB = ocr;
			break;
		case Chnl_C:
			timer->OCRnC = ocr;
			break;
		default:
			break;
				
	return;
}

void timer16_set_pwm_freq (tcnt_16b_t* timer, uint16_t freq)
{
	return;
}

void timer16_udelay (tcnt_16b_t * timer, uint16_t usec)
{
	return;
}

