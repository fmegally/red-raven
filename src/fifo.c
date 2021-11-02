#include "fifo.h"
#include <stdlib.h>

int fifo_init(struct fifo* buff, unsigned char buff_size)
{
	buff->data = (unsigned char *) malloc(buff_size * sizeof(unsigned char));
	if (buff->data == NULL) return -1;
	buff->size = buff_size;
	buff->cnt = 0;
	buff->head = 0;
	buff->tail = 0;
	return 0;
}

int fifo_putc(const unsigned char* c, struct fifo* buff)
{
	if (buff->cnt < buff->size){
		buff->data[buff->head] = *c;
		buff->head = (buff->head + 1) % buff->size;
		++(buff->cnt);
		return 0;
	} else {
		return -1;
	}
}

int fifo_getc(unsigned char* c, struct fifo* buff)
{
	if (buff->cnt > 0){
		*c = buff->data[buff->tail];
		buff->tail = (buff->tail + 1) % buff->size;
		--(buff->cnt);
		return 1;
	} else {
		return 0;
	}
}

int fifo_write(struct fifo *buff, unsigned char *data, unsigned int size)
{
	if (size <= (buff->size - buff->cnt)){
		unsigned int i;
		for (i=0; i<size; i++){
			buff->data[buff->head] = block[i] ;
                        buff->head = (buff->head + 1) % buff->size;
		}
		buff->cnt = buff->cnt + size;
		return 0;
	} else {
		return -1;
	}
}

int fifo_read(struct fifo *buff, unsigned char *data, unsigned int size)
{
	if (size <= buff->cnt){
		unsigned int i;
		for (i=0; i<size; i++){
			block[i] = buff->data[buff->tail];
		        buff->tail = (buff->tail + 1) % buff->size;
		}
		buff->cnt = buff->cnt - size;
		return 0;
	} else {
		return -1;
	}
}

int fifo_isempty(const struct fifo *buff)
{
	return buff->cnt;
}

int fifo_isfull(const struct fifo *buff)
{
	return buff->cnt == buff->size;
}

int fifo_flush(struct fifo *buff)
{
	buff->cnt = 0;
	buff->head = 0;
	buff->tail = 0;
	return 0;
}

//================================================================================
/*
fifo_init(struct fifo* buff, unsigned char buff_size){
	buff->size = buff_size;
	buff->data = (unsigned char *) malloc(buff_size * sizeof(unsigned char));
	if (buff->data == NULL) return -1;
	buff->head = 0;
	buff->tail = 0;
	return 0;
}

int
fifo_putc(const char* c, struct fifo* buff){
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
fifo_getc(char* c, struct fifo* buff){
	if (!(buff->tail == buff->head)){
		*c = buff->data[buff->tail];
		buff->tail = (buff->tail + 1) % buff->size;
		return 0;
	} else {
		return -1;
	}
}

int
fifo_isempty(const struct fifo *buff){
	return buff->cnt;
}

int
fifo_isfull(const struct fifo *buff){
	return buff->cnt == buff->size;
}
*/

//============================================================

/*
fifo_init(struct fifo* buff, unsigned char buff_size){
	buff->size = buff_size;
	buff->full = FALSE;
	buff->data = (unsigned char *) malloc(buff_size * sizeof(unsigned char));
	buff->head = 0;
	buff->tail = 0;
	return buff;
}

int
fifo_putc(const char* c, fifo* buff){
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
fifo_getc(char* c, fifo* buff){
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
fifo_getblock(fifo *buff, const char *block, unsigned int size)
{
	int s = fifo_getsize(buff);
	if (s > size){
		block = &(buff->data[buff->tail]);
		buff->tail = (buff->tail + size) % buff->size;
		return 0;
	} else {
		return -1;
	}
}

int
fifo_isempty(fifo *buff){
	if (buff->head	== buff->tail){
		return 1;
	} else {
		return 0;
	}
}

int
fifo_isfull(fifo *buff){
	if((buff->head + 1) % buff->size != buff->tail){
		return 0;
	} else {
		return 1;
	}
}
*/
