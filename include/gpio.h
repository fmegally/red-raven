#ifndef GPIO_H_
#define GPIO_H_

#include <stddef.h>

#include <avr/io.h>
#include <stdint.h>
#include "bits.h"

struct gpio {
	volatile uint8_t PIN;
	volatile uint8_t DDR;
	volatile uint8_t PORT;
};

typedef struct gpio gpio_t;

extern gpio_t * const gpio_ports_list [];

#if defined (__AVR_ATmega2560__)
#define GPIO_PORTA_ADD (0x20)
#define GPIO_PORTA     ((gpio_t *) GPIO_PORTA_ADD)
#endif

#if defined (__AVR_ATmega2560__) || (__AVR_ATmega328__) || (__AVR_ATmega328P__)
#define GPIO_PORTB_ADD (0x23)
#define GPIO_PORTB     ((gpio_t *) GPIO_PORTB_ADD)
#endif

#if defined (__AVR_ATmega2560__) || (__AVR_ATmega328__) || (__AVR_ATmega328P__)
#define GPIO_PORTC_ADD (0x26)
#define GPIO_PORTC     ((gpio_t *) GPIO_PORTC_ADD)
#endif

#if defined (__AVR_ATmega2560__) || (__AVR_ATmega328__) || (__AVR_ATmega328P__)
#define GPIO_PORTD_ADD (0x29)
#define GPIO_PORTD     ((gpio_t *) GPIO_PORTD_ADD)
#endif

#if defined (__AVR_ATmega2560__)
#define GPIO_PORTE_ADD (0x2C)
#define GPIO_PORTE     ((gpio_t *) GPIO_PORTE_ADD)
#endif

#if defined (__AVR_ATmega2560__)
#define GPIO_PORTF_ADD (0x2F)
#define GPIO_PORTF     ((gpio_t *) GPIO_PORTF_ADD)
#endif

#if defined (__AVR_ATmega2560__)
#define GPIO_PORTG_ADD (0x32)
#define GPIO_PORTG     ((gpio_t *) GPIO_PORTG_ADD)
#endif

#if defined (__AVR_ATmega2560__)
#define GPIO_PORTH_ADD (0x100)
#define GPIO_PORTH     ((gpio_t *) GPIO_PORTH_ADD)
#endif

#if defined (__AVR_ATmega2560__)
#define GPIO_PORTJ_ADD (0x103)
#define GPIO_PORTJ     ((gpio_t *) GPIO_PORTJ_ADD)
#endif

#if defined (__AVR_ATmega2560__)
#define GPIO_PORTK_ADD (0x106)
#define GPIO_PORTK     ((gpio_t *) GPIO_PORTK_ADD)
#endif

#if defined (__AVR_ATmega2560__)
#define GPIO_PORTL_ADD (0x109)
#define GPIO_PORTL     ((gpio_t *) GPIO_PORTL_ADD)
#endif

void gpio_setmode(gpio_t* const port, uint8_t mode);
void gpio_pwrite(gpio_t* const port, uint8_t value);
uint8_t gpio_pread(gpio_t* const port);

void gpio_setbit(gpio_t* const port, uint8_t pin);
void gpio_clrbit(gpio_t* const port, uint8_t pin);
uint8_t gpio_getbit(gpio_t* const port, uint8_t pin);
void gpio_flipbit(gpio_t* const port, uint8_t pin);

#endif
