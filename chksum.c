#include "chksum.h"
/*
uint8_t chksum (uint8_t data[], uint8_t cnt) 
{
	uint16_t acc = 0;
	uint8_t i;
	for(i=0; i < cnt; i++){
		acc = acc + data[i];
		while ((acc >> 8) > 0){
			acc = (acc & 0xFF) + (acc >> 8);
		}
	}
	return ~acc;
}
*/
uint8_t chksum (uint8_t data[], uint8_t cnt) 
{
	uint16_t acc = 0;
	uint8_t i;
	for(i=0; i < cnt; i++){
		acc = acc + data[i];
	}
	acc = (acc & 0xFF) + (acc >> 8);
	return ~acc;
}