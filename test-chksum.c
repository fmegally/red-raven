#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "chksum.h"

int main (int argc, char* argv[])
{
        uint8_t data[9];
        uint8_t c;

        srand(time(0));

        for (c = 0; c < 9; c++)
        {
            uint8_t tmp = rand();
            data[c] = tmp;
            printf("%d\t",tmp);
        } 
        printf("\n");
        

        uint8_t sum = chksum(data,sizeof(data));
        uint8_t data_w_chksum[10];
        memcpy (data_w_chksum,data,9);
        data_w_chksum[9] = sum;
        for (c = 0; c < 10; c++)
        {    
            printf("%d\t",data_w_chksum[c]);
        } 
        printf("\n");
        uint8_t check = chksum(data_w_chksum,sizeof(data_w_chksum));
        printf("%d\n",check);
        return 0;
}