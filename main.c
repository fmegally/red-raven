#include <stdio.h>
#include "uart.h"

int main(int argc, char *argv[])
{
	char buffer[32];
	UART_init(UART0, 9600, UART_PARITY_NONE, UART_CHAR_SIZE_8BIT);
	UART_print(UART0, "This software is property of FOXDYNE LLC.\n");

	for(int x = 0; x<100; x++){
		sprintf(buffer,"%d\n",x);
		UART_print(UART0, buffer);
	}
	
	UART_print(UART0, "=========================================\n");
	while(1);
	return 0;
}

