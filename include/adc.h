#ifndef _ADC_H
#define _ADC_H

#include <avr/io.h>
#include <stdint.h>
#include "bits.h"

enum adc_ref
{
        AREF=0x00,
        AVCC=0x01,
        INTERNAL11=0x03
};

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
struct adc
{
        uint16_t ADCn;
        uint8_t  ADCSRnA;
        uint8_t  ADCSRnB;
        uint8_t  ADMUXn;
        uint8_t  _padding_0;
        uint8_t  DID0n;
};

typedef struct adc adc_t;

#if defined (__AVR_ATmega328P__) || (__AVR_ATmega328__)
#define ADC0 ((adc_t *) (0x78))
#endif

void adc_enable (adc_t * const a);
void adc_diable (adc_t * const a);
int8_t adc_select_ch (adc_t * const a, uint8_t ch);
int8_t adc_select_refv (adc_t * const a, enum adc_ref r);
uint16_t  adc_read_ch_blocking (adc_t * const a);

#endif
