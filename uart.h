/*
 * uart.h
 *
 * Created: 3/25/2020 11:48:22 PM
 *  Author: fmega
 */ 


#ifndef UART_H_
#define UART_H_


#include <avr/io.h>
#include "common.h"
 
#define UART_PARITY_NONE 0
#define UART_PARITY_EVEN 2
#define UART_PARITY_ODD  3

#define UART_SB_1BIT 0
#define UART_SB_2BIT 1

#define UART_CHAR_SIZE_5BIT 0
#define UART_CHAR_SIZE_6BIT 1
#define UART_CHAR_SIZE_7BIT 2
#define UART_CHAR_SIZE_8BIT 3
#define UART_CHAR_SIZE_9BIT 7

struct uart
{
	uint8_t UCSRnA;
	uint8_t UCSRnB;
	uint8_t UCSRnC;
	uint8_t padding_0;
	uint16_t UBRRn;
	uint8_t UDR;
	uint8_t padding_1;
};

void UART_init(uint32_t baud, uint8_t parity, uint8_t byte_size );
void UART_write(const uint8_t* buff, uint8_t len);
void UART_print(const char* str);
void UART_putc(char c);
#endif /* UART_H_ */
