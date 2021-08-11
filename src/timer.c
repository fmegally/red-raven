#include "timer.h"
#include "bits.h"

void timer8_set_mode (tcnt_8b_t* timer, enum tcnt_modes mode)
{
	uint8_t msk;
	uint8_t t_mode;	
	switch(mode)
	{
		case CTC_OCRnA:
			t_mode = 0x02;
			break;
		case PWM_Fast_8bit:
			t_mode = 0x03;
			break;
		case PWM_PhaseCorrect_OCRnA:
			t_mode = 0x05;
			break;
		case PWM_Fast_OCRnA:
			t_mode = 0x07;
			break;
		default:
			t_mode = mode;
			break;
	}

	msk = (1<<WGM00) | (1<<WGM01);
	timer->TCCRnA = (timer->TCCRnA & ~msk) | (t_mode & msk);

	msk = (1<<WGM02) ;
	timer->TCCRnB = (timer->TCCRnB & ~msk) | (t_mode & msk);

	return;
}

void timer8_start (tcnt_8b_t* timer, enum tcnt_clk_src src)
{
	uint8_t msk = (1<<CS02) | (1<<CS01) | (1<<CS00);
    	timer->TCCRnB = (timer->TCCRnA & ~msk) | (src & msk);
	return;
}

void timer8_stop (tcnt_8b_t* timer)
{
	uint8_t msk = (1<<CS02) | (1<<CS01) | (1<<CS00);
    	timer->TCCRnB &= (timer->TCCRnA & ~msk);
	return;
}

void timer8_set_ocr (tcnt_8b_t* timer, uint8_t ocr, enum tcnt_occ channel)
{
	switch(channel){
		case Chnl_A:
			timer->OCRnA = ocr;
			break;
		case Chnl_B:
			timer->OCRnB = ocr;
			break;
		default:
			break;
	}
	return;
}

void timer8_udelay (tcnt_8b_t * timer, uint8_t usec)
{
	return;
}


void timer16_set_mode (tcnt_16b_t* timer, enum tcnt_modes mode)
{
	uint8_t msk;

	msk = (1<<WGM10) | (1<<WGM11);
	timer->TCCRnA = (timer->TCCRnA & ~msk) | (mode & msk);

	msk = (1<<WGM12) | (1<<WGM13);
	timer->TCCRnB = (timer->TCCRnB & ~msk) | (mode & msk);

	return;
}

void timer16_start (tcnt_16b_t* timer, enum tcnt_clk_src src)
{
	uint8_t msk = (1<<CS12) | (1<<CS11) | (1<<CS10);
    	timer->TCCRnB = (timer->TCCRnA & ~msk) | (src & msk);
	return;
}

void timer16_stop (tcnt_16b_t* timer)
{
	uint8_t msk = (1<<CS12) | (1<<CS11) | (1<<CS10);
    	timer->TCCRnB &= (timer->TCCRnA & ~msk);
	return;
}

uint16_t timer16_get_input_capture (tcnt_16b_t* timer)
{
	return timer->ICRn;	
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
	}
	return;
}

void timer16_udelay (tcnt_16b_t *timer, uint16_t usec)
{
	return;
}
