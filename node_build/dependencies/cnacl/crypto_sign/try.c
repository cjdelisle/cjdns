/*
 * crypto_sign/try.c version 20111119
 * D. J. Bernstein
 * Public domain.
 */

#include <stdlib.h>
#include "randombytes.h"
#include "crypto_sign.h"

#define MAXTEST_BYTES 10000
#define TUNE_BYTES 1536

extern unsigned char *alignedcalloc(unsigned long long);

const char *primitiveimplementation = crypto_sign_IMPLEMENTATION;

static unsigned char *pk; unsigned long long pklen; static unsigned char *pk2;
static unsigned char *sk; unsigned long long sklen; static unsigned char *sk2;
static unsigned char *m; unsigned long long mlen; static unsigned char *m2;
static unsigned char *sm; unsigned long long smlen; static unsigned char *sm2;
static unsigned char *t; unsigned long long tlen; static unsigned char *t2;

void preallocate(void)
{
#ifdef RAND_R_PRNG_NOT_SEEDED
  RAND_status();
#endif
}

void allocate(void)
{
  pk = alignedcalloc(pklen = crypto_sign_PUBLICKEYBYTES);
  sk = alignedcalloc(sklen = crypto_sign_SECRETKEYBYTES);
  m = alignedcalloc(MAXTEST_BYTES + crypto_sign_BYTES);
  sm = alignedcalloc(MAXTEST_BYTES + crypto_sign_BYTES);
  t = alignedcalloc(MAXTEST_BYTES + crypto_sign_BYTES);
  pk2 = alignedcalloc(pklen);
  sk2 = alignedcalloc(sklen);
  m2 = alignedcalloc(MAXTEST_BYTES + crypto_sign_BYTES);
  sm2 = alignedcalloc(MAXTEST_BYTES + crypto_sign_BYTES);
  t2 = alignedcalloc(MAXTEST_BYTES + crypto_sign_BYTES);
}

void predoit(void)
{
  crypto_sign_keypair(pk,sk);
  mlen = TUNE_BYTES;
  smlen = 0;
}

void doit(void)
{
  randombytes(m,mlen);
  crypto_sign(sm,&smlen,m,mlen,sk);
  crypto_sign_open(t,&tlen,sm,smlen,pk);
}

static unsigned char chain[37]; long long chainlen = 37;
char checksum[37 * 2 + 1];

const char *checksum_compute(void)
{
  long long mlen;
  long long i;
  long long j;
  long long loops;

  for (loops = 0;loops < 10;++loops) {
    for (j = -16;j < 0;++j) sk2[j] = sk[j] = random();
    for (j = 0;j < sklen + 16;++j) sk2[j] = sk[j] = random();
    for (j = -16;j < 0;++j) pk2[j] = pk[j] = random();
    for (j = 0;j < pklen + 16;++j) pk2[j] = pk[j] = random();
    if (crypto_sign_keypair(pk,sk) != 0) return "crypto_sign_keypair returns nonzero";
    for (j = -16;j < 0;++j) if (pk[j] != pk2[j]) return "crypto_sign_keypair writes before pk";
    for (j = pklen;j < pklen + 16;++j) if (pk[j] != pk2[j]) return "crypto_sign_keypair writes after pk";
    for (j = -16;j < 0;++j) if (sk[j] != sk2[j]) return "crypto_sign_keypair writes before sk";
    for (j = sklen;j < sklen + 16;++j) if (sk[j] != sk2[j]) return "crypto_sign_keypair writes after sk";
  
    for (mlen = 0;mlen < MAXTEST_BYTES;mlen += 1 + (mlen / 16)) {
      for (j = -16;j < 0;++j) m2[j] = m[j] = random();
      for (j = mlen;j < mlen + 16;++j) m2[j] = m[j] = random();
      randombytes(m,mlen);
      if (mlen > 0)
        for (j = 0;j < chainlen;++j) m[j % mlen] ^= chain[j];
      for (j = 0;j < mlen;++j) m2[j] = m[j];
      for (j = -16;j < 0;++j) pk2[j] = pk[j];
      for (j = 0;j < pklen + 16;++j) pk2[j] = pk[j];
      for (j = -16;j < 0;++j) sk2[j] = sk[j];
      for (j = 0;j < sklen + 16;++j) sk2[j] = sk[j];
      for (j = -16;j < 0;++j) sm2[j] = sm[j] = random();
      for (j = 0;j < mlen + crypto_sign_BYTES + 16;++j) sm2[j] = sm[j] = random();
  
      if (crypto_sign(sm,&smlen,m,mlen,sk) != 0) return "crypto_sign returns nonzero";
      if (smlen > mlen + crypto_sign_BYTES) return "crypto_sign returns more than crypto_sign_BYTES extra bytes";
      if (smlen == 0) return "crypto_sign returns empty message";
      for (j = -16;j < 0;++j) if (pk[j] != pk2[j]) return "crypto_sign overwrites pk";
      for (j = 0;j < pklen + 16;++j) if (pk[j] != pk2[j]) return "crypto_sign overwrites pk";
      for (j = -16;j < 0;++j) if (sk[j] != sk2[j]) return "crypto_sign overwrites sk";
      for (j = 0;j < sklen + 16;++j) if (sk[j] != sk2[j]) return "crypto_sign overwrites sk";
      for (j = -16;j < 0;++j) if (m[j] != m2[j]) return "crypto_sign overwrites m";
      for (j = 0;j < mlen + 16;++j) if (m[j] != m2[j]) return "crypto_sign overwrites m";
      for (j = -16;j < 0;++j) if (sm[j] != sm2[j]) return "crypto_sign writes before sm";
      for (j = smlen;j < smlen + 16;++j) if (sm[j] != sm2[j]) return "crypto_sign writes after sm";

      for (j = 0;j < smlen;++j) chain[j % chainlen] ^= sm[j];
  
      for (j = -16;j < 0;++j) sm2[j] = sm[j];
      for (j = 0;j < smlen + 16;++j) sm2[j] = sm[j];
      for (j = -16;j < 0;++j) t2[j] = t[j] = random();
      for (j = 0;j < smlen + 16;++j) t2[j] = t[j] = random();
  
      if (crypto_sign_open(t,&tlen,sm,smlen,pk) != 0) return "crypto_sign_open returns nonzero";
      if (tlen != mlen) return "crypto_sign_open does not match length";
      for (i = 0;i < tlen;++i)
        if (t[i] != m[i])
          return "crypto_sign_open does not match contents";
      for (j = -16;j < 0;++j) if (pk[j] != pk2[j]) return "crypto_sign_open overwrites pk";
      for (j = 0;j < pklen + 16;++j) if (pk[j] != pk2[j]) return "crypto_sign_open overwrites pk";
      for (j = -16;j < 0;++j) if (sk[j] != sk2[j]) return "crypto_sign_open overwrites sk";
      for (j = 0;j < sklen + 16;++j) if (sk[j] != sk2[j]) return "crypto_sign_open overwrites sk";
      for (j = -16;j < 0;++j) if (sm[j] != sm2[j]) return "crypto_sign_open overwrites sm";
      for (j = 0;j < smlen + 16;++j) if (sm[j] != sm2[j]) return "crypto_sign_open overwrites sm";
      for (j = -16;j < 0;++j) if (t[j] != t2[j]) return "crypto_sign_open writes before t";
      for (j = smlen;j < smlen + 16;++j) if (t[j] != t2[j]) return "crypto_sign_open writes after t";
  
      j = random() % smlen;
      sm[j] ^= 1;
      for (j = -16;j < 0;++j) sm2[j] = sm[j];
      for (j = 0;j < smlen + 16;++j) sm2[j] = sm[j];
      for (j = -16;j < 0;++j) t2[j] = t[j] = random();
      for (j = 0;j < smlen + 16;++j) t2[j] = t[j] = random();
      if (crypto_sign_open(t,&tlen,sm,smlen,pk) == 0) {
        if (tlen != mlen) return "crypto_sign_open allows trivial forgery of length";
        for (i = 0;i < tlen;++i)
          if (t[i] != m[i])
            return "crypto_sign_open allows trivial forgery of contents";
      }
      for (j = -16;j < 0;++j) if (pk[j] != pk2[j]) return "crypto_sign_open overwrites pk";
      for (j = 0;j < pklen + 16;++j) if (pk[j] != pk2[j]) return "crypto_sign_open overwrites pk";
      for (j = -16;j < 0;++j) if (sk[j] != sk2[j]) return "crypto_sign_open overwrites sk";
      for (j = 0;j < sklen + 16;++j) if (sk[j] != sk2[j]) return "crypto_sign_open overwrites sk";
      for (j = -16;j < 0;++j) if (sm[j] != sm2[j]) return "crypto_sign_open overwrites sm";
      for (j = 0;j < smlen + 16;++j) if (sm[j] != sm2[j]) return "crypto_sign_open overwrites sm";
      for (j = -16;j < 0;++j) if (t[j] != t2[j]) return "crypto_sign_open writes before t";
      for (j = smlen;j < smlen + 16;++j) if (t[j] != t2[j]) return "crypto_sign_open writes after t";
      sm[j] ^= 1;
    }
  }

  for (i = 0;i < chainlen;++i) {
    checksum[2 * i] = "0123456789abcdef"[15 & (chain[i] >> 4)];
    checksum[2 * i + 1] = "0123456789abcdef"[15 & chain[i]];
  }
  checksum[2 * i] = 0;
  return 0;
}
