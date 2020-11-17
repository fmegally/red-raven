#ifndef _RINGBUFFER_H
#define _RINGBUFFER_H
#define FALSE 0
#define TRUE  1
#define ERROR -1

typedef struct ringbuffer_t ringbuffer;

int rb_create(unsigned char buff_size, ringbuffer* buff);
int rb_putchar(const char *c, ringbuffer *buff);
int rb_getchar(char *c, ringbuffer *buff);
int rb_isempty(ringbuffer *buff);
int rb_isfull(ringbuffer *buff);

#endif
