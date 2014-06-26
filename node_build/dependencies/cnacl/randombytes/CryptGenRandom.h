#ifndef randombytes_CryptGenRandom_H
#define randombytes_CryptGenRandom_H

#ifdef __cplusplus
extern "C" {
#endif

extern void randombytes(unsigned char *,unsigned long long);

#ifdef __cplusplus
}
#endif

#ifndef randombytes_implementation
#define randombytes_implementation "CryptGenRandom"
#endif

#endif
