#ifndef _RINGBUFFER_H
#define _RINGBUFFER_H

#define ERROR -1

/* this implementation uses a boolean full flag instead of count*/

/*
struct ringbuffer {
	unsigned char size;
	unsigned int head;
	unsigned int tail;
	unsigned char* data;
};
*/

/* this implementation uses a boolean full flag instead of count*/

/*
struct ringbuffer {
	unsigned char size;
	unsigned char full;
	unsigned int head;
	unsigned int tail;
	unsigned char* data;
};
*/

/*
	this implementation relies on count to determine if the buffer
	is full or not.
*/

struct ringbuffer {
	unsigned char size;
	unsigned char cnt;
	unsigned int head;
	unsigned int tail;
	unsigned char* data;
};

int rb_init( struct ringbuffer* buff, unsigned char buff_size);
int rb_putc(const char *c, struct ringbuffer *buff);
int rb_getc(char *c, struct ringbuffer *buff);
int rb_isempty(const struct ringbuffer *buff);
int rb_isfull(const struct ringbuffer *buff);
int rb_getblock(struct ringbuffer *buff, char *block, unsigned int size);
int rb_flush(struct ringbuffer *buff);
#endif
