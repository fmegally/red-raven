#include <stdio.h>
#include "uart.h"
#include "gpio.h"
#include "protocol.h"
#include "fifo.h"
#include <avr/io.h>
#include <avr/interrupt.h>


static fifo_t uart_rx_buffer;
static protocol_t* uart_protocol;

#if defined (__AVR_ATmega328P__)
ISR(USART_RX_vect)
{
	uint8_t c = UDR0;
	fifo_putc(&c , &uart_rx_buffer);
	return;
}
#endif

#if defined (__AVR_ATmega2560__)
ISR(USART0_RX_vect)
{
	uint8_t c = UDR0;
	rb_putc(&c , &uart_rx_buffer);
	return;
}
#endif

void init_system()
{
		cli();
                fifo_init(&uart_rx_buffer, 32);
                uart_protocol = create_protocol(UART0);
                protocol_attach_buffers(uart_protocol, &uart_rx_buffer, NULL);
		UART_init(UART0, 115200, UART_PARITY_NONE, UART_CHAR_SIZE_8BIT);
		sei();
		return;
}

int main(int argc, char *argv[])
{
	init_system();
	UART_print(UART0,"System initialization complete.\n\r");
	UART_print(UART0,"===============================\n\r");
	UART_print(UART0,"THIS IS FOR DEBUGGING ONLY     \n\r");
	UART_print(UART0,"CHECK TO SEE IF ALL CHARACTERS \n\r");
	UART_print(UART0,"SHOW UP CORRECTLY              \n\r");
	UART_print(UART0,"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz\n\r");

	return 0;
}

