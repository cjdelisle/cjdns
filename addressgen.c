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
    struct Address addr;
    randombytes(key, 32);
    uint32_t smallest = UINT32_MAX;
    for (uint32_t i = 0;; i++) {
        crypto_scalarmult_curve25519_base(key, key);
        AddressCalc_addressForPublicKey(addr.ip6.bytes, key);
        if ((addr.ip6.ints.three & 0xFF) == 0xFC) {
            uint32_t bits = Endian_bigEndianToHost32(addr.ip6.ints.four);
            if (bits < smallest) {
                smallest = bits;
                for (int it = 0; it < 32; it++) {
                    printf("%02x", key[it]);
                }
                uint8_t ipv6[40];
                Address_printIp(ipv6, &addr);
                printf("    %s\n", ipv6);
            }
        }
    }
}
