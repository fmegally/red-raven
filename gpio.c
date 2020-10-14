#include "gpio.h"

volatile struct gpio * const __attribute__ ((packed)) GPIO_A= (volatile struct gpio *) &PINA;
volatile struct gpio * const __attribute__ ((packed)) GPIO_B= (volatile struct gpio *) &PINB;
volatile struct gpio * const __attribute__ ((packed)) GPIO_C= (volatile struct gpio *) &PINC;
volatile struct gpio * const __attribute__ ((packed)) GPIO_D= (volatile struct gpio *) &PIND;
volatile struct gpio * const __attribute__ ((packed)) GPIO_E= (volatile struct gpio *) &PINE;
volatile struct gpio * const __attribute__ ((packed)) GPIO_F= (volatile struct gpio *) &PINF;
volatile struct gpio * const __attribute__ ((packed)) GPIO_G= (volatile struct gpio *) &PING;
volatile struct gpio * const __attribute__ ((packed)) GPIO_H= (volatile struct gpio *) &PINH;
volatile struct gpio * const __attribute__ ((packed)) GPIO_J= (volatile struct gpio *) &PINJ;
volatile struct gpio * const __attribute__ ((packed)) GPIO_K= (volatile struct gpio *) &PINK;
volatile struct gpio * const __attribute__ ((packed)) GPIO_L= (volatile struct gpio *) &PINL;

void gpio_setmode(volatile struct gpio* const port, uint8_t mode)
{
	port->DDR = mode;
	return;
}

void gpio_pwrite(volatile struct gpio* const port, uint8_t value)
{
	port->PORT = value;
	return;
}

uint8_t gpio_pread(volatile struct gpio* const port)
{
	return port->PIN;
}



