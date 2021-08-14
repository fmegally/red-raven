#ifndef _ADC_H
#define _ADC_H

#include <avr/io.h>
#include <stdint.h>

enum adc_ref
{
        AREF=0x00,
        AVCC=0x01,
        INTERNAL11=0x03
}

/*

Value already defined in avr header
keeping enum just for reference

enum adc_chan
{
        ADCH0,
        ADCH1,
        ADCH2,
        ADCH3,
        ADCH4,
        ADCH5,
        ADCH6,
        ADCH7
}
*/

typedef struct adc adc_t;

void adc_enable(adc_t * const a);
void adc_diable(adc_t * const a);
void adc_select_chan(adc_t * const a, uint8_t ch);
void adc_select_refv(adc_t * const a, enum adc_ref r);
uint16_t  adc_read_ch_blocking(adc_t * const a);

#endif
