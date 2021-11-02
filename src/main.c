#include <stdio.h>
#include "uart.h"
#include "gpio.h"
#include "telegram.h"
#include "ringbuffer.h"
#include <avr/io.h>
#include <avr/interrupt.h>


static struct fifo uart_rx_buffer;
static struct protocol uart_protocol;

#if defined (__AVR_ATmega328P__)
ISR(USART_RX_vect)
{
	uint8_t c = UDR0;
	rb_putc(&c , &uart_rx_buffer);
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
		fifo_init(&uart_rx_buffer, 48);
                protocol_init(&uart_protocol);
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

        struct telegram tg;
        int8_t tlgrm_error_code;

	while(1){
                tlgrm_error_code = fetch_tlgrm(&uart_rx_buffer,&tg);
                if(tlgrm_error_code == 0){
                        send_ack_tlgrm(UART0);
                        dispatch(&tg, tlgrm_handler_table);
                } else {
                        send_nak_tlgrm(UART0);
                }
	}

	return 0;
}

