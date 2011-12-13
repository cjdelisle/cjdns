#include "crypto/AddressCalc.h"
#include "crypto/Crypto.h"
#include "dht/Address.h"
#include "util/Base32.h"
#include "util/Time.h"
#include "crypto_scalarmult_curve25519.h"

#include <stdint.h>
#include <stdio.h>

int main()
{
    uint8_t key[32];
    //uint8_t addr[32];
    randombytes(key, 32);
    //uint32_t smallest = ~0;
    for (uint32_t i = 0; i < 100000; i++) {
        crypto_scalarmult_curve25519_base(key, key);
        /*AddressCalc_addressForPublicKey(addr, key);
        if (*((uint16_t*)(addr + 2)) == 0) {
            uint32_t bits = Endian_bigEndianToHost32(*((uint32_t*)(addr + 4)));
            if (bits < smallest) {
                smallest = bits;
                for (int it = 0; it < 32; it++) {
                    printf("%02x", key[it]);
                }
                printf("    %04x iterations = %u\n", bits, i);
            }
        }*/
    }
}
