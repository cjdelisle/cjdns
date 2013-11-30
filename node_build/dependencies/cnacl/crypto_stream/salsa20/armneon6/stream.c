/*
version 20110506
D. J. Bernstein
Public domain.
*/

#include "crypto_stream.h"

int crypto_stream(
        unsigned char *c,unsigned long long clen,
  const unsigned char *n,
  const unsigned char *k
)
{
  return crypto_stream_xor(c,0,clen,n,k);
}
