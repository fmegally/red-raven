#include <avr/io.h>
#include <stdint.h>
#include "gpio.h"

int main (void)
{
	uint32_t d;
	gpio_setmode(GPIO_F, 0xFF);
	while(1){
		gpio_pwrite(GPIO_F,0b11111110);
		for(d = 0; d<100;d++);
		gpio_pwrite(GPIO_F,0b11111111);
		for(d = 0; d<1600;d++);
	}
	return 0;
}

