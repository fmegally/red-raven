#include <avr/io.h>
#include <stdint.h>

struct gpio {
	uint8_t PORT;
	uint8_t DDR;
	uint8_t PIN;
};


