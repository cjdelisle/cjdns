#include "crypto_onetimeauth.h"
#include "crypto_uint32.h"

#define addmulmod crypto_onetimeauth_poly1305_neon2_addmulmod
#define blocks crypto_onetimeauth_poly1305_neon2_blocks

typedef struct {
  crypto_uint32 v[12]; /* for alignment; only using 10 */
} fe1305x2;

extern void addmulmod(fe1305x2 *r, const fe1305x2 *x, const fe1305x2 *y, const fe1305x2 *c);
  
extern int blocks(fe1305x2 *h, const fe1305x2 *precomp, const unsigned char *in, unsigned int inlen);

static void freeze(fe1305x2 *r)
{
  int i;
  crypto_uint32 x0 = r->v[0];
  crypto_uint32 x1 = r->v[2];
  crypto_uint32 x2 = r->v[4];
  crypto_uint32 x3 = r->v[6];
  crypto_uint32 x4 = r->v[8];
  crypto_uint32 y0;
  crypto_uint32 y1;
  crypto_uint32 y2;
  crypto_uint32 y3;
  crypto_uint32 y4;
  crypto_uint32 swap;

  for (i = 0;i < 3;++i) {
    x1 += x0 >> 26; x0 &= 0x3ffffff;
    x2 += x1 >> 26; x1 &= 0x3ffffff;
    x3 += x2 >> 26; x2 &= 0x3ffffff;
    x4 += x3 >> 26; x3 &= 0x3ffffff;
    x0 += 5*(x4 >> 26); x4 &= 0x3ffffff;
  }

  y0 = x0 + 5;
  y1 = x1 + (y0 >> 26); y0 &= 0x3ffffff;
  y2 = x2 + (y1 >> 26); y1 &= 0x3ffffff;
  y3 = x3 + (y2 >> 26); y2 &= 0x3ffffff;
  y4 = x4 + (y3 >> 26); y3 &= 0x3ffffff;
  swap = -(y4 >> 26); y4 &= 0x3ffffff;

  y0 ^= x0;
  y1 ^= x1;
  y2 ^= x2;
  y3 ^= x3;
  y4 ^= x4;

  y0 &= swap;
  y1 &= swap;
  y2 &= swap;
  y3 &= swap;
  y4 &= swap;

  y0 ^= x0;
  y1 ^= x1;
  y2 ^= x2;
  y3 ^= x3;
  y4 ^= x4;

  r->v[0] = y0;
  r->v[2] = y1;
  r->v[4] = y2;
  r->v[6] = y3;
  r->v[8] = y4;
}
  
static void fe1305x2_tobytearray(unsigned char *r, fe1305x2 *x)
{
  crypto_uint32 x0 = x->v[0];
  crypto_uint32 x1 = x->v[2];
  crypto_uint32 x2 = x->v[4];
  crypto_uint32 x3 = x->v[6];
  crypto_uint32 x4 = x->v[8];

  x1 += x0 >> 26;
  x0 &= 0x3ffffff;
  x2 += x1 >> 26;
  x1 &= 0x3ffffff;
  x3 += x2 >> 26;
  x2 &= 0x3ffffff;
  x4 += x3 >> 26;
  x3 &= 0x3ffffff;

  *(crypto_uint32 *) r = x0 + (x1 << 26);
  *(crypto_uint32 *) (r + 4) = (x1 >> 6) + (x2 << 20);
  *(crypto_uint32 *) (r + 8) = (x2 >> 12) + (x3 << 14);
  *(crypto_uint32 *) (r + 12) = (x3 >> 18) + (x4 << 8);
}

static void fe1305x2_frombytearray(fe1305x2 *r, const unsigned char *x, unsigned long long xlen)
{
  int i;
  unsigned char t[17];

  for (i = 0;(i < 16) && (i < xlen);i++) t[i] = x[i];
  xlen -= i;
  x += i;
  t[i++] = 1;
  for (;i<17;i++) t[i] = 0;

  r->v[0] = 0x3ffffff & *(crypto_uint32 *) t;
  r->v[2] = 0x3ffffff & ((*(crypto_uint32 *) (t + 3)) >> 2);
  r->v[4] = 0x3ffffff & ((*(crypto_uint32 *) (t + 6)) >> 4);
  r->v[6] = 0x3ffffff & ((*(crypto_uint32 *) (t + 9)) >> 6);
  r->v[8] = *(crypto_uint32 *) (t + 13);

  if (xlen) {
    for (i = 0;(i < 16) && (i < xlen);i++) t[i] = x[i];
    t[i++] = 1;
    for (;i<17;i++) t[i] = 0;

    r->v[1] = 0x3ffffff & *(crypto_uint32 *) t;
    r->v[3] = 0x3ffffff & ((*(crypto_uint32 *) (t + 3)) >> 2);
    r->v[5] = 0x3ffffff & ((*(crypto_uint32 *) (t + 6)) >> 4);
    r->v[7] = 0x3ffffff & ((*(crypto_uint32 *) (t + 9)) >> 6);
    r->v[9] = *(crypto_uint32 *) (t + 13);
  }
  else
    r->v[1] = r->v[3] = r->v[5] = r->v[7] = r->v[9] = 0;
}

static const fe1305x2 zero __attribute__ ((aligned (16)));

int crypto_onetimeauth(unsigned char *out,const unsigned char *in,unsigned long long inlen,const unsigned char *k)
{
  unsigned int j;
  unsigned int tlen;
  unsigned char data[sizeof(fe1305x2[5]) + 128];
  fe1305x2 *const r = (fe1305x2 *) (data + (15 & (-(int) data)));
  fe1305x2 *const h = r + 1;
  fe1305x2 *const c = h + 1;
  fe1305x2 *const precomp = c + 1;

  r->v[1] = r->v[0] = 0x3ffffff & *(crypto_uint32 *) k;
  r->v[3] = r->v[2] = 0x3ffff03 & ((*(crypto_uint32 *) (k + 3)) >> 2);
  r->v[5] = r->v[4] = 0x3ffc0ff & ((*(crypto_uint32 *) (k + 6)) >> 4);
  r->v[7] = r->v[6] = 0x3f03fff & ((*(crypto_uint32 *) (k + 9)) >> 6);
  r->v[9] = r->v[8] = 0x00fffff & ((*(crypto_uint32 *) (k + 12)) >> 8);

  for (j = 0;j < 10;j++) h->v[j] = 0; /* XXX: should fast-forward a bit */

  addmulmod(precomp,r,r,&zero);                   /* precompute r^2 */
  if (inlen >= 64)
    addmulmod(precomp + 1,precomp,precomp,&zero); /* precompute r^4 */

  while (inlen > 32) {
    tlen = 1048576;
    if (inlen < 1048576) tlen = inlen;
    tlen -= blocks(h, precomp, in, tlen);
    inlen -= tlen;
    in += tlen;
  }

  addmulmod(h,h,precomp,&zero); 

  if (inlen > 16) {
    fe1305x2_frombytearray(c,in,inlen);
    precomp->v[1] = r->v[1];
    precomp->v[3] = r->v[3];
    precomp->v[5] = r->v[5];
    precomp->v[7] = r->v[7];
    precomp->v[9] = r->v[9];
    addmulmod(h,h,precomp,c); 
  } else if (inlen > 0) {
    fe1305x2_frombytearray(c,in,inlen);
    r->v[1] = 1;
    r->v[3] = 0;
    r->v[5] = 0;
    r->v[7] = 0;
    r->v[9] = 0;
    addmulmod(h,h,r,c); 
  }

  h->v[0] += h->v[1];
  h->v[2] += h->v[3];
  h->v[4] += h->v[5];
  h->v[6] += h->v[7];
  h->v[8] += h->v[9];
  freeze(h);

  fe1305x2_frombytearray(c,k+16,16);
  c->v[8] ^= (1 << 24);

  h->v[0] += c->v[0];
  h->v[2] += c->v[2];
  h->v[4] += c->v[4];
  h->v[6] += c->v[6];
  h->v[8] += c->v[8];
  fe1305x2_tobytearray(out,h);

  return 0;
}
