#ifndef ADDRESS_CALC_H
#define ADDRESS_CALC_H

#include <stdint.h>

void AddressCalc_addressForPublicKey(uint8_t addressOut[16], const uint8_t key[32]);

#endif
