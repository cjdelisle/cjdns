#include <windows.h>
#include <wincrypt.h>
#include <process.h>
#include <assert.h>

void randombytes(unsigned char *x,unsigned long long xlen)
{
    static int provider_set = 0;
    static HCRYPTPROV provider;

    if (!provider_set) {
        if (!CryptAcquireContext(&provider, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT)) {
            if (GetLastError() != (DWORD)NTE_BAD_KEYSET) {
                assert(0);
            }
        }
        provider_set = 1;
    }
    if (!CryptGenRandom(provider, xlen, x)) {
        assert(0);
    }
}

// Windows does not have random() by default.
int random()
{
    return rand();
}
