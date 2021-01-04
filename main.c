#include <stdio.h>
#include "uart.h"
#include "gpio.h"
#include "message.h"
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


int main(int argc, char *argv[])
{
	rb_init(&uart_rx_buffer, 48);
	UART_init(UART0, 9600, UART_PARITY_NONE, UART_CHAR_SIZE_8BIT);
	sei();

	while(1){
		process_message(&uart_rx_buffer);
	}

	return 0;
}

