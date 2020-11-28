#include <stdio.h>
#include "ringbuffer.h"

int main (int argc, char *argv[])
{
	unsigned char c[]="Hello world";
	unsigned char t[256];

	struct ringbuffer new_buffer;
	rb_init(&new_buffer,8);
	printf("\tcnt\thead\ttail\treturn\n");
	printf("\t%d,\t%d,\t%d,\t%d\n",new_buffer.cnt,new_buffer.head,new_buffer.tail,0);
	int i;
	int r=0;

	for (i=0; i<8;i++)
	{
		r = rb_putc(&c[i],&new_buffer);
		printf("\t%d,\t%d,\t%d,\t%d\n",new_buffer.cnt,new_buffer.head,new_buffer.tail,r);
	}
	
	rb_getblock(&new_buffer, t, 4);
	
	for (i=0; i<4; i++) {
		printf("%c",t[i]);
	}
	putchar('\n');

	return 0;
}
