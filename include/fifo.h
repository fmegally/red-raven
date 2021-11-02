#ifndef FIFO_H_
#define FIFO_H_

#define ERROR -1

/* this implementation uses a boolean full flag instead of count*/

/*
struct fifo {
	unsigned char size;
	unsigned int head;
	unsigned int tail;
	unsigned char* data;
};
*/

/* this implementation uses a boolean full flag instead of count*/

/*
struct fifo {
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

struct fifo {
	unsigned char size;
	unsigned char cnt;
	unsigned int head;
	unsigned int tail;
	unsigned char* data;
};

typedef struct fifo fifo_t;

int fifo_init(fifo_t *buff, unsigned char buff_size);
int fifo_putc(const unsigned char *c, fifo_t *buff);
int fifo_getc(unsigned char *c, fifo_t *buff);
int fifo_write(fifo_t *buff, unsigned char *data, unsigned int size);
int fifo_read(fifo_t *buff, unsigned char *data, unsigned int size);
int fifo_isempty(const fifo_t *buff);
int fifo_isfull(const fifo_t *buff);
int fifo_flush(fifo_t *buff);
#endif
