/*
 * uart.c
 *
 * Created: 3/25/2020 11:48:09 PM
 *  Author: fmega
 */ 

#include "uart.h"

void UART_init(uart_t* base, uint32_t baud, uint8_t parity, uint8_t byte_size )
{
	base->UBRRn  = (F_OSC/(16 * baud)) - 1;                                       //set baud rate countdown register. formula for UBRR register is in atmel data sheet
	base->UCSRnB = (~(0x01 << UCSZn2) & base->UCSRnB) | (byte_size & 0x04);             //byte size settings are split over 2 registers so it has to be done in 2 steps
	base->UCSRnC = (~(0x03 << UCSZn0) & base->UCSRnC) | ((byte_size & 0x03) << UCSZn0);
	base->UCSRnC = (~(0x03 << UPMn0) & base->UCSRnC) | (parity << UPMn0);               //select parity
	base->UCSRnB |= (1 << RXENn) | (1 << TXENn);                                  //enable serial TX and RX
	base->UCSRnB |= (1 << RXCIEn);                                                //enable RX complete interrupt
	return;
}

static inline
void UART_wait_TX(const uart_t* base)
{
	while(!(base->UCSRnA & (1 << UDREn)));
	return;
}

void UART_putc(uart_t* base, const char c)
{
    UART_wait_TX(base);
    base->UDRn = c;
    return;
}

void UART_write(uart_t* base, const uint8_t* buff, uint8_t len)
{
	uint8_t i;
	for (i = 0; i < len; i++) {
		UART_wait_TX(base);
		base->UDRn = *(buff + i);
	}
	return;
}

void UART_print(uart_t* base, const char* str){
	uint16_t i = 0;
	while (*(str + i) != '\0'){
		UART_wait_TX(base);
		base->UDRn = *(str + i++);
	}
	return;
}


