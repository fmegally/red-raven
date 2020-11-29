#include <stdio.h>
#include <stdlib.h>
#include "message.h"
#define PRINTLN printf("\n")

struct ringbuffer rx_buffer;


int main (int argc, char* argv[])
{
    rb_init(&rx_buffer,64);
    unsigned char c = PREAMBLE;
    rb_putc(&c, &rx_buffer);

    uint8_t fake_message[10] = {GPIO_SET_PIN,0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80,0xFD};
    
    int i;
    for(i = 0; i < sizeof(fake_message);i++)
    {
        printf("%02X ",fake_message[i]);
        rb_putc(&fake_message[i],&rx_buffer);
    }
    PRINTLN;

    c = TERMINATOR;
    rb_putc(&c, &rx_buffer);
    /*
    unsigned char t;
    while (rb_getc(&t,&rx_buffer))
    {
        printf("%02X ",t);
    } */
    PRINTLN;


//    uint8_t temp_chksum = chksum(fake_message,9);
//    struct message dummy_message = {GPIO_SET_PIN,{0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80},temp_chksum};
    struct message rx_message;
    process_message(&rx_buffer);
    //print_message(&rx_message);
    

    return 0;
}
