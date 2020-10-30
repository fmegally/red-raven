/*
 * uart.c
 *
 * Created: 3/25/2020 11:48:09 PM
 *  Author: fmega
 */ 
#include "uart.h"

void UART_Init(uint32_t baud, uint8_t parity, uint8_t byte_size )
{
	UBRR0  = (F_OSC/(16 * baud)) - 1;                                       //set baud rate countdown register. formula for UBRR register is in atmel data sheet
	UCSR0B = (~(0x01 << UCSZ02) & UCSR0B) | (byte_size & 0x04);             //byte size settings are split over 2 registers so it has to be done in 2 steps
	UCSR0C = (~(0x03 << UCSZ00) & UCSR0C) | ((byte_size & 0x03) << UCSZ00);
	UCSR0C = (~(0x03 << UPM00) & UCSR0C) | (parity << UPM00);               //select parity
	UCSR0B |= (1 << RXEN0) | (1 << TXEN0);                                  //enable serial TX and RX
	UCSR0B |= (1 << RXCIE0);                                                //enable RX complete interrupt
	return;
}

static
void UART_wait_TX(void)
{
	while(!(UCSR0A & (1 << UDRE0)));
	return;
}

void UART_putc(char c)
{
    UART_wait_TX();
    UDR0 = c;
    return;
}

void UART_write(const uint8_t* buff, uint8_t len)
{
	uint8_t i;
	for (i = 0; i < len; i++) {
		UART_wait_TX();
		UDR0 = *(buff + i);
	}
	return;
}

void UART_print(const char* str){
	uint16_t i = 0;
	while (*(str + i) != '\0'){
		UART_wait_TX();
		UDR0 = *(str + i++);
	}
	return;
}


