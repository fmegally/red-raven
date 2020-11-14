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

enum tctn_occ {A,B,C};

enum tcnt_clk_src {None,
	      CPU_Freq,
	      Prescaler_8,
	      Prescaler_64,
	      Prescaler_256,
	      Prescaler_1024,
	      Ext_Falling,
	      Ext_Rising};

struct tcnt_8b {
	uint8_t TCCRnA;     // Timer/Counter n Control Register A
	uint8_t TCCRnB;     // Timer/Counter n Control Register B
	uint8_t TCNTn;     // Timer/Counter
	uint8_t OCRnA;     // Output Compare A
	uint8_t OCRnB;     // Output Compare B
	uint8_t _padding_0;
};

typedef struct tcnt_8b tcnt_8b_t;

#define TCNT0_8B_BASE_ADD   (0x46)
#define TCNT0_8B            ((tcnt_8b_t*) TCNT0_8B_BASE_ADD)

struct tcnt_16b {
	uint8_t TCCRnA;     // Timer/Counter n Control Register A
	uint8_t TCCRnB;     // Timer/Counter n Control Register B
	uint8_t TCCRnC;     // Timer/Counter n Control Register C
	uint8_t _padding_0;
	uint16_t TCNTn;     // Timer/Counter
	uint16_t ICRn;      // Input Capture
	uint16_t OCRnA;     // Output Compare A
	uint16_t OCRnB;     // Output Compare B
	uint16_t OCRnC;     // Output Compare C
	uint8_t _padding_1;
	uint8_t _padding_2;
};

typedef struct tcnt_16b tcnt_16b_t;

#define TCNT1_16B_BASE_ADD   (0x80)
#define TCNT1_16B            ((tcnt_16b_t*) TCNT1_8B_BASE_ADD)

#define TCNT3_16B_BASE_ADD   (0x90)
#define TCNT3_16B            ((tcnt_16b_t*) TCNT3_16B_BASE_ADD)

#define TCNT4_16B_BASE_ADD   (0xA0)
#define TCNT4_16B            ((tcnt_16b_t*) TCNT3_16B_BASE_ADD)

#define TCNT5_16B_BASE_ADD   (0x120)
#define TCNT5_16B            ((tcnt_16b_t*) TCNT3_16B_BASE_ADD)

void timer_set_mode (tcnt_16b_t* timer, enum tcnt_modes mode);
void timer_start (tcnt_16b_t* timer, enum tcnt_clk_src);
void timer_stop (tcnt_16b_t* timer);
void timer_get_input_capture (tcnt_16b_t* timer, uint16_t *dst);
void timer_set_pwm_dc (tcnt_16b_t* timer, uint16_t duty_cycle);
void timer_set_pwm_freq (tcnt_16b_t* timer, uint16_t freq);
void timer_udelay (tcnt_16b_t * timer, uint16_t usec);

