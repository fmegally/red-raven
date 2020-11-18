#ifndef _RINGBUFFER_H
#define _RINGBUFFER_H
#define FALSE 0
#define TRUE  1
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

int rb_init(unsigned char buff_size, struct ringbuffer* buff);
int rb_putc(const char *c, struct ringbuffer *buff);
int rb_getc(char *c, struct ringbuffer *buff);
int rb_isempty(struct ringbuffer *buff);
int rb_isfull(struct ringbuffer *buff);

#endif
