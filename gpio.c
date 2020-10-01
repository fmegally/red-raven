#include "gpio.h"

volatile struct gpio * const __attribute__ ((packed)) GPIO_A= (volatile struct gpio *) &PORTA;
volatile struct gpio * const __attribute__ ((packed)) GPIO_B= (volatile struct gpio *) &PORTB;
volatile struct gpio * const __attribute__ ((packed)) GPIO_C= (volatile struct gpio *) &PORTC;
volatile struct gpio * const __attribute__ ((packed)) GPIO_D= (volatile struct gpio *) &PORTD;
volatile struct gpio * const __attribute__ ((packed)) GPIO_E= (volatile struct gpio *) &PORTE;
volatile struct gpio * const __attribute__ ((packed)) GPIO_F= (volatile struct gpio *) &PORTF;
volatile struct gpio * const __attribute__ ((packed)) GPIO_G= (volatile struct gpio *) &PORTG;
volatile struct gpio * const __attribute__ ((packed)) GPIO_H= (volatile struct gpio *) &PORTH;
volatile struct gpio * const __attribute__ ((packed)) GPIO_J= (volatile struct gpio *) &PORTJ;
volatile struct gpio * const __attribute__ ((packed)) GPIO_K= (volatile struct gpio *) &PORTK;
volatile struct gpio * const __attribute__ ((packed)) GPIO_L= (volatile struct gpio *) &PORTL;

void gpio_setmode(struct gpio* const port, uint8_t mode)
{
	port->DDR = mode;
	return;
}

void gpio_pwrite(struct gpio* const port, uint8_t value)
{
	port->PORT = value;
	return;
}

uint8_t gpio_pread(struct gpio* const port)
{
	return port->PIN;
}



