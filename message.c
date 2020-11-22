#ifndef H_MESSAGE
#define H_MESSAGE
#include <stdint.h>

#define MESSAGE_DATA_SZ 8;

struct message
{
	uint8_t id;
	uint8_t data[8];
	uint8_t chksum;
};


#endif
