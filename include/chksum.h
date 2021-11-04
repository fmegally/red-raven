#ifndef CHKSUM_H_
#define CHKSUM_H_
#include <stdint.h>

uint8_t chksum8 (uint8_t data[], uint8_t cnt);
uint8_t verify_chksum8 (uint8_t data[], uint8_t cnt, uint8_t chksum);
#endif /*CHKSUM_H_*/

