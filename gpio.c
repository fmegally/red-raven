#include "gpio.h"

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
