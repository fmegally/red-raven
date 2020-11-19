#include "ringbuffer.h"
#include <stdlib.h>

int
rb_init(struct ringbuffer* buff, unsigned char buff_size){
	buff->data = (unsigned char *) malloc(buff_size * sizeof(unsigned char));
	if (buff->data == NULL) return -1;
	buff->size = buff_size;
	buff->cnt = 0;
	buff->head = 0;
	buff->tail = 0;
	return 0;
}

int
rb_putc(const char* c, struct ringbuffer* buff){
	if (buff->cnt < buff->size){
		buff->data[buff->head] = *c;
		buff->head = (buff->head + 1) % buff->size;
		++(buff->cnt);
		return 0;
	} else {
		return -1;
	}
}

int
rb_getc(char* c, struct ringbuffer* buff){
	if (buff->cnt > 0){
		*c = buff->data[buff->tail];
		buff->tail = (buff->tail + 1) % buff->size;
		--(buff->cnt);
		return 0;
	} else {
		return -1;
	}
}

int
rb_getblock(struct ringbuffer *buff, char *block, unsigned int size)
{
	if (size <= buff->cnt){
		unsigned int i;
		for (i=0; i<size; i++){
			block[i] = buff->data[buff->tail];
			buff->tail = (buff->tail + 1) % buff->size;
		}
		buff->tail = (buff->tail + size) % buff->size;
		buff->cnt = buff->cnt - size;
		return 0;
	} else {
		return -1;
	}
}

int
rb_isempty(const struct ringbuffer *buff){
	return buff->cnt;
}

int
rb_isfull(const struct ringbuffer *buff){
	return buff->cnt == buff->size;
}

//================================================================================
/*
rb_init(struct ringbuffer* buff, unsigned char buff_size){
	buff->size = buff_size;
	buff->data = (unsigned char *) malloc(buff_size * sizeof(unsigned char));
	if (buff->data == NULL) return -1;
	buff->head = 0;
	buff->tail = 0;
	return 0;
}

int
rb_putc(const char* c, struct ringbuffer* buff){
	unsigned int n = (buff->head + 1) % buff->size;
	if (n != buff->tail){
		buff->data[buff->head] = *c;
		buff->head = n;
		return 0;
	} else {
		return -1;
	}
}

int
rb_getc(char* c, struct ringbuffer* buff){
	if (!(buff->tail == buff->head)){
		*c = buff->data[buff->tail];
		buff->tail = (buff->tail + 1) % buff->size;
		return 0;
	} else {
		return -1;
	}
}

int
rb_isempty(const struct ringbuffer *buff){
	return buff->cnt;
}

int
rb_isfull(const struct ringbuffer *buff){
	return buff->cnt == buff->size;
}
*/

//============================================================

/*
rb_init(struct ringbuffer* buff, unsigned char buff_size){
	buff->size = buff_size;
	buff->full = FALSE;
	buff->data = (unsigned char *) malloc(buff_size * sizeof(unsigned char));
	buff->head = 0;
	buff->tail = 0;
	return buff;
}

int
rb_putc(const char* c, ringbuffer* buff){
	if (! buff->full){
		buff->data[buff->head] = *c;
		buff->head = (buff->head + 1) % buff->size;
		if (buff->head == buff->tail) buff->full = TRUE;
		return 0;
	} else {
		return -1;
	}
}

int
rb_getc(char* c, ringbuffer* buff){
	if ((buff->tail) != (buff->head)){
		*c = buff->data[buff->tail];
		buff->tail = (buff->tail + 1) % buff->size;
		return 0;
	} else {
		if (buff->is_full){
			*c = buff->data[buff->tail];
			buff->tail = (buff->tail + 1) % buff->size;
			buff->full = FALSE;
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
		buff->tail = (buff->tail + size) % buff->size;
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
	if((buff->head + 1) % buff->size != buff->tail){
		return 0;
	} else {
		return 1;
	}
}
*/
