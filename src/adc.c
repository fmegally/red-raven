#include "adc.h"

void adc_enable (adc_t * const a)
{
        BITSET(a->ADCSRnA,ADEN);
        return;
}

void adc_disable (adc_t * const a)
{
        BITCLR(a->ADCSRnA,ADEN);
        return;
}

int8_t adc_select_chan (adc_t * const a, uint8_t ch)
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


