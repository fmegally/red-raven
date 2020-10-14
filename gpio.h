#include <avr/io.h>
#include <stdint.h>


struct gpio {
	uint8_t PIN;
	uint8_t DDR;
	uint8_t PORT;
};

extern volatile struct gpio* const GPIO_A;
extern volatile struct gpio* const GPIO_B;
extern volatile struct gpio* const GPIO_C;
extern volatile struct gpio* const GPIO_D;
extern volatile struct gpio* const GPIO_E;
extern volatile struct gpio* const GPIO_F;
extern volatile struct gpio* const GPIO_G;
extern volatile struct gpio* const GPIO_H;
extern volatile struct gpio* const GPIO_J;
extern volatile struct gpio* const GPIO_K;
extern volatile struct gpio* const GPIO_L;

void gpio_setmode(volatile struct gpio* const port, uint8_t mode);
void gpio_pwrite(volatile struct gpio* const port, uint8_t value);
uint8_t gpio_pread(volatile struct gpio* const port);

