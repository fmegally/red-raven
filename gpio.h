#include <avr/io.h>
#include <stdint.h>

struct gpio {
	uint8_t PORT;
	uint8_t DDR;
	uint8_t PIN;
};

void gpio_setmode(struct gpio* const port, uint8_t mode);
void gpio_pwrite(struct gpio* const port, uint8_t value);
uint8_t gpio_pread(struct gpio* const port);

