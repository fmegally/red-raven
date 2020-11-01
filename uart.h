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

struct uart
{
	uint8_t  UCSRnA;
	uint8_t  UCSRnB;
	uint8_t  UCSRnC;
	uint8_t  _padding_0;
	uint16_t UBRRn;
	uint8_t  UDRn;
	uint8_t  _padding_1;
};

typedef struct uart uart_t;

#define UART0_BASE	 (0xC0)
#define UART0 		 ((uart *) UART0_BASE)

#define UART1_BASE	 (0xC8)
#define UART1 		 ((uart *) UART0_BASE)

#define UART2_BASE	 (0xD0)
#define UART2 		 ((uart *) UART0_BASE)

#define UART3_BASE	 (0x130)
#define UART3 		 ((uart *) UART0_BASE)

#define RXCn    7
#define TXCn    6
#define UDREn   5
#define FEn     4
#define DORn    3
#define UPEn    2
#define U2Xn    1
#define MPCMn   0

#define RXCIEn  7
#define TXCIEn  6
#define UDRIEn  5
#define RXENn   4
#define TXENn   3
#define UCSZn2  2
#define RXB8n   1
#define TXB8n   0

#define UMSELn1 7
#define UMSELn0 6
#define UPMn1   5
#define UPMn0   4
#define USBSn   3
#define UCSZn1  2
#define UCSZn0  1
#define UCPOLn  0

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


void UART_init(const uart* base, uint32_t baud, uint8_t parity, uint8_t byte_size );
void UART_write(const uart* base, const uint8_t* buff, uint8_t len);
void UART_print(const uart* base, const char* str);
void UART_putc(const uart* base, char c);
#endif /* UART_H_ */
