#include "crypto_hash_sha512.h"

#include <stdint.h>
#include <string.h>

void AddressCalc_addressForPublicKey(uint8_t addressOut[16], const uint8_t key[32])
{
    uint8_t hash[crypto_hash_sha512_BYTES];
    crypto_hash_sha512(hash, key, 32);
    crypto_hash_sha512(hash, hash, crypto_hash_sha512_BYTES);
    memcpy(addressOut, hash, 16);
}
