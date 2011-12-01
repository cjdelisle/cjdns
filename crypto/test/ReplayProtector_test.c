#include "crypto/ReplayProtector.h"
#include "crypto/Crypto.h"

#include <assert.h>
#include <stdint.h>

int main()
{
    uint16_t randomShorts[8192];
    uint16_t out[8192];
    struct ReplayProtector rp = {0,0};

    randombytes((uint8_t*)randomShorts, sizeof(randomShorts));

    uint32_t outIdx = 0;
    for (uint32_t i = 0; i < 1024; i++) {
        if (ReplayProtector_checkNonce((randomShorts[i] % (i + 20)), &rp)) {
            out[outIdx] = (randomShorts[i] % (i + 20));
            outIdx++;
        }
    }

    for (uint32_t i = 0; i < outIdx; i++) {
        for (uint32_t j = i + 1; j < outIdx; j++) {
            assert(out[i] != out[j]);
        }
    }
}
