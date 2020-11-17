#include "ringbuffer.h"

struct ringbuffer {
	unsigned char size;
	unsigned char full;
	unsigned int head;
	unsigned int tail;
	unsigned char* data;
};

ringbuffer*
rb_create(unsigned char buff_size){
	ringbuffer* buff = (ringbuffer*) malloc(sizeof(ringbuffer));		
	buff->is_full = 0;
	buff->data = (unsigned char *) malloc(buff_size * sizeof(unsigned char));
	buff->size = buff_size;
	buff->head = 0;
	buff->tail = 0;
	return buff;
}

int
rb_putchar(const char* c, ringbuffer* buff){
	if (! buff->is_full){
		buff->data[buff->head] = *c;
		buff->head = (buff->head + 1) % buff->length;
		if (buff->head == buff->tail) buff->is_full = 1;
		return 0;
	} else {
		return -1;
	}
}

int
rb_getchar(char* c, ringbuffer* buff){
	if ((buff->tail) != (buff->head)){
		*c = buff->data[buff->tail];
		buff->tail = (buff->tail + 1) % buff->length;
		return 0;
	} else {
		if (buff->is_full){
			*c = buff->data[buff->tail];
			buff->tail = (buff->tail + 1) % buff->length;
			buff->is_full = 0;
			return 0;
		} else {
			return -1;
		}
	}
}

int
rb_getblock(ringbuffer *buff, const char *block, unsigned int size)
{
	int s = ringbuffer_getsize(buff);
	if (s > size){
		block = &(buff->data[buff->tail]);
		buff->tail = (buff->tail + size) % buff->length;
		return 0;
	} else {
		return -1;
	}
}

int
rb_isempty(ringbuffer *buff){
	if (buff->head	== buff->tail){
		return 1;
	} else {
		return 0;
	}
}

int
rb_isfull(ringbuffer *buff){
	if((buff->head + 1) % buff->length != buff->tail){
		return 0;
	} else {
		return 1;
	}
}
