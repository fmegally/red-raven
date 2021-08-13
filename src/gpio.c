#include "gpio.h"

#if defined (__AVR_ATmega328P__) || defined (__AVR_ATmega328__)
gpio_t * const gpio_ports_list [] = {	[1]=GPIO_PORTB,
					[2]=GPIO_PORTC,
			     		[3]=GPIO_PORTD };
#endif

#if defined (__AVR_ATmega2560__)
gpio_t * const gpio_ports_list [] = {	GPIO_PORTA,
				     	GPIO_PORTB,
					GPIO_PORTC,
			     		GPIO_PORTD,
			     		GPIO_PORTE,
			     		GPIO_PORTF,
			     		GPIO_PORTG,
			     		GPIO_PORTH,
			     		NULL,
			     		GPIO_PORTJ,
		  	     		GPIO_PORTK,
	             			GPIO_PORTL };
#endif

void gpio_setmode(gpio_t* const port, uint8_t mode)
{
	port->DDR = mode;
	return;
}

void gpio_pwrite(gpio_t* const port, uint8_t value)
{
	port->PORT = value;
	return;
}

uint8_t gpio_pread(gpio_t* const port)
{
	return port->PIN;
}

void gpio_setbit(gpio_t* const port, uint8_t pin)
{
	BITSET(port->PORT, pin);
	return;
}

void gpio_clrbit(gpio_t* const port, uint8_t pin)
{
	BITCLR(port->PORT, pin);
	return;
}

uint8_t gpio_getbit(gpio_t* const port, uint8_t pin)
{
	return BITGET(port->PORT, pin);
}

void gpio_flipbit(gpio_t* const port, uint8_t pin)
{
	BITFLIP(port->PORT, pin);
	return;
}
