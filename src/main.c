#include <stdio.h>
#include "uart.h"
#include "gpio.h"
#include "telegram.h"
#include "ringbuffer.h"
#include <avr/io.h>
#include <avr/interrupt.h>


static struct ringbuffer uart_rx_buffer;

ISR(USART0_RX_vect)
{
	uint8_t c = UDR0;
	rb_putc(&c , &uart_rx_buffer);
	return;
}

void init_system()
{
		cli();
		rb_init(&uart_rx_buffer, 48);
		UART_init(UART0, 9600, UART_PARITY_NONE, UART_CHAR_SIZE_8BIT);
		sei();
		return;
}

int main(int argc, char *argv[])
{
	init_system();
	UART_print(UART0,"Init complete");

	while(1){
	}

	return 0;
}

