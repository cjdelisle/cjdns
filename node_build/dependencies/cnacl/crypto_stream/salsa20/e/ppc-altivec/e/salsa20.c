/*
salsa20-altivec.c version 20051118
D. J. Bernstein, with big contributions from Matthijs van Duin
Public domain.
*/

#include <altivec.h>
#include "ecrypt-sync.h"

void ECRYPT_init(void)
{
  return;
}

static const char sigma[16] = "expand 32-byte k";
static const char tau[16] = "expand 16-byte k";

void ECRYPT_keysetup(ECRYPT_ctx *x,const u8 *k,u32 kbits,u32 ivbits)
{
  const char *constants;

  memcpy(x->myaligned.input + 4,k,16);
  if (kbits == 256) { /* recommended */
    k += 16;
    constants = sigma;
  } else { /* kbits == 128 */
    constants = tau;
  }
  memcpy(x->myaligned.input + 44,k,16);
  memcpy(x->myaligned.input + 0,constants,4);
  memcpy(x->myaligned.input + 20,constants + 4,4);
  memcpy(x->myaligned.input + 40,constants + 8,4);
  memcpy(x->myaligned.input + 60,constants + 12,4);
}

void ECRYPT_ivsetup(ECRYPT_ctx *x,const u8 *iv)
{
  memcpy(x->myaligned.input + 24,iv,8);
  memset(x->myaligned.input + 32,0,8);
}

typedef vector unsigned int vu32;
typedef vector unsigned char vu8;

static const u32 salsa20_vconst[] __attribute__((aligned (16))) = {
  0x03020100, 0x17161514, 0x0B0A0908, 0x1F1E1D1C,
  0x00000000, 0x00000000, 0xFFFFFFFF, 0xFFFFFFFF,
  0x04050607, 0x08090A0B, 0x0C0D0E0F, 0x00010203
} ;

void ECRYPT_encrypt_bytes(ECRYPT_ctx *x,const u8 *m,u8 *c,u32 bytes)
{
  const vu8  viop = (vu8) vec_ld( 0, salsa20_vconst);
  const vu32 vios =       vec_ld(16, salsa20_vconst);
  const vu8  vrp1 = (vu8) vec_ld(32, salsa20_vconst);
  const vu8  vrp2 = vec_perm(vrp1, vrp1, vrp1);
  const vu8  vrp3 = vec_perm(vrp2, vrp2, vrp1);
  const vu32 vrr07 = vec_splat_u32( 7);
  const vu32 vrr09 = vec_splat_u32( 9);
  const vu32 vrr13 = vec_splat_u32(13);
  const vu32 vrr18 = vrr09 + vrr09;
  u8 *ctarget;
  vu32 tmp[4];
  vu32 x0;
  vu32 x1;
  vu32 x2;
  vu32 x3;
  vu32 y0;
  vu32 y1;
  vu32 y2;
  vu32 y3;
  vu32 z1;
  vu32 z2;
  vu32 z3;
  int i;

  if (!bytes) return;

  for (;;) {
    if (bytes < 64) {
      for (i = 0;i < bytes;++i) ((char *) tmp)[i] = m[i];
      m = (char *) tmp;
      ctarget = c;
      c = (char *) tmp;
    }

    y0 = vec_ld( 0, (const u32 *) &x->myaligned.input);
    y1 = vec_ld(16, (const u32 *) &x->myaligned.input);
    y2 = vec_ld(32, (const u32 *) &x->myaligned.input);
    y3 = vec_ld(48, (const u32 *) &x->myaligned.input);
    x0 = vec_perm(y0, y1, viop);
    x1 = vec_perm(y1, y2, viop);
    x2 = vec_perm(y2, y3, viop);
    x3 = vec_perm(y3, y0, viop);
    y0 = vec_sel(x0, x2, vios);
    y1 = vec_sel(x1, x3, vios);
    y2 = vec_sel(x2, x0, vios);
    y3 = vec_sel(x3, x1, vios);

    if (!++x->myaligned.input[32])
    if (!++x->myaligned.input[33])
    if (!++x->myaligned.input[34])
    if (!++x->myaligned.input[35])
    if (!++x->myaligned.input[36])
    if (!++x->myaligned.input[37])
    if (!++x->myaligned.input[38])
    if (!++x->myaligned.input[39])
      ; /* stopping at 2^70 bytes per nonce is user's responsibility */

    for (i = 0;i < 20;++i) {
      z1 = y1 ^ vec_rl(y0 + y3, vrr07);
      z2 = y2 ^ vec_rl(z1 + y0, vrr09);
      z3 = y3 ^ vec_rl(z2 + z1, vrr13);
      y0 = y0 ^ vec_rl(z3 + z2, vrr18);
      y1 = vec_perm(z3, z3, vrp1);
      y2 = vec_perm(z2, z2, vrp2);
      y3 = vec_perm(z1, z1, vrp3);
    }
    x0 += vec_sel(y0, y2, vios);
    x1 += vec_sel(y1, y3, vios);
    x2 += vec_sel(y2, y0, vios);
    x3 += vec_sel(y3, y1, vios);
    y0 = vec_ld( 0, (u32 *) m) ^ vec_perm(x0, x3, viop);
    y1 = vec_ld(16, (u32 *) m) ^ vec_perm(x1, x0, viop);
    y2 = vec_ld(32, (u32 *) m) ^ vec_perm(x2, x1, viop);
    y3 = vec_ld(48, (u32 *) m) ^ vec_perm(x3, x2, viop);
    vec_st(y0,  0, (u32 *) c);
    vec_st(y1, 16, (u32 *) c);
    vec_st(y2, 32, (u32 *) c);
    vec_st(y3, 48, (u32 *) c);

    if (bytes <= 64) {
      if (bytes < 64) {
        for (i = 0;i < bytes;++i) ctarget[i] = c[i];
      }
      return;
    }
    bytes -= 64;
    c += 64;
    m += 64;
  }
}

void ECRYPT_decrypt_bytes(ECRYPT_ctx *x,const u8 *c,u8 *m,u32 bytes)
{
  ECRYPT_encrypt_bytes(x,c,m,bytes);
}

void ECRYPT_keystream_bytes(ECRYPT_ctx *x,u8 *stream,u32 bytes)
{
  u32 i;
  for (i = 0;i < bytes;++i) stream[i] = 0;
  ECRYPT_encrypt_bytes(x,stream,stream,bytes);
}
