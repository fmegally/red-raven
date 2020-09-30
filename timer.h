#include <avr/io.h>
#include <stdint.h>

enum tcnt_modes {Normal,
		 PWM_PhaseCorrect_8bit,
		 PWM_PhaseCorrect_9bit,
		 PWM_PhaseCorrect_10bit,
		 CTC_OCRnA,
		 PWM_Fast_8bit,
		 PWM_Fast_9bit,
		 PWM_Fast_10bit,
		 PWM_PhaseAndFreqCorrect,
		 PWM_PhaseCorrect_ICRn,
		 PWM_PhaseCorrect_OCRnA,
		 CTC_ICRn,
		 reserved_mode,
		 PWM_Fast_ICRn,
		 PWM_Fast_OCRnA};

enum tcnt_clk_src {None,
	      CPU_Freq,
	      Prescaler_8,
	      Prescaler_64,
	      Prescaler_256,
	      Prescaler_1024,
	      Ext_Falling,
	      Ext_Rising};

struct tcnt {
	uint8_t TCCRnA; // Timer/Counter n Control Register A
	uint8_t TCCRnB; // Timer/Counter n Control Register B
	uint8_t TCCRnC; // Timer/Counter n Control Register C
	uint8_t _padding_0;
	uint16_t TCNTn; // Timer/Counter
	uint16_t OCRnA; // Output Compare A
	uint16_t OCRnB; // Output Compare B
	uint16_t OCRnC; // Output Compare C
	uint16_t ICRn;  // Input Capture
	uint8_t TIMSKn; // Interrupt Mask
	uint8_t TIFRn;  // Interrupt Flags
	uint8_t _padding_1;
	uint8_t _padding_2;
};

int timer_init (struct tcnt * timer, enum tcnt_modes mode);
void timer_start(struct tcnt * timer, enum tcnt_clk_src);
void timer_stop (struct tcnt * timer);
void timer_get_input_capture(struct tcnt * timer, uint16_t *dst);
void timer_set_pwm_ds(struct tcnt * timer, uint8_t duty_cycle);
void timer_set_pwm_frq(struct tcnt * timer, uint16_t frq);



