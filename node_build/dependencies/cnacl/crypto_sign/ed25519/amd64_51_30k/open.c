#include "crypto_sign.h"
#include "crypto_verify_32.h"
#include "crypto_hash_sha512.h"
#include "ge25519.h"
#include "hram.h"

int crypto_sign_open(
    unsigned char *m,unsigned long long *mlen,
    const unsigned char *sm,unsigned long long smlen,
    const unsigned char *pk
    )
{
  int i;
  unsigned char t2[32];
  ge25519 get1, get2;
  sc25519 schram, scs;
  unsigned char hram[crypto_hash_sha512_BYTES];

  *mlen = (unsigned long long) -1;

  if (smlen < 64) goto badsig;
  if (ge25519_unpackneg_vartime(&get1, pk)) goto badsig;

  get_hram(hram,sm,pk,m,smlen);

  sc25519_from64bytes(&schram, hram);

  sc25519_from32bytes(&scs, sm+32);

  ge25519_double_scalarmult_vartime(&get2, &get1, &schram, &scs);
  ge25519_pack(t2, &get2);

  if (!crypto_verify_32(sm, t2))
  {
    for(i=0;i<smlen-64;i++)
      m[i] = sm[i + 64];
    *mlen = smlen-64;
    return 0;
  }

  badsig:
  for(i=0;i<smlen-64;i++)
    m[i] = 0;
  return -1;
}
