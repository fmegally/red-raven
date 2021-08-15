#include "adc.h"

void adc_enable (adc_t * const a)
{
        a->ADCSRnA = a->ADCSRnA | ((1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0)); // set prescaller to 128 to get 125k @ 16MHz clock freq
        BITSET(a->ADCSRnA,ADEN);
        return;
}

void adc_disable (adc_t * const a)
{
        BITCLR(a->ADCSRnA,ADEN);
        return;
}

int8_t adc_select_ch (adc_t * const a, uint8_t ch)
{
        if (ch >=0 && ch < 8){
                a->ADMUXn = ((a->ADMUXn) & ~0x0F) | ch;
                return 0;
        } else {
                return -1;
        }
}

int8_t adc_select_refv (adc_t * const a, enum adc_ref r)
{
        a->ADMUXn = (a->ADMUXn & ~(1<<REFS1 | 1<<REFS0)) | r;
        return 0;
}

uint16_t adc_read_ch_blocking(adc_t * const a)
{
        BITSET(a->ADCSRnA,ADSC);
        while (BITGET(a->ADCSRnA,ADSC)); //wait till conversion is complete
        return a->ADCn;
};

