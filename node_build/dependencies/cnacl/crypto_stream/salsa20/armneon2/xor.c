/*
version 20110506
D. J. Bernstein
Public domain.
*/

#define ROUNDS 20
#include <arm_neon.h>
#include "crypto_stream.h"

static const unsigned char sigma[16] = "expand 32-byte k";

int crypto_stream_xor(
        unsigned char *c,
  const unsigned char *m,unsigned long long mlen,
  const unsigned char *n,
  const unsigned char *k
)
{
  const uint32x4_t abab = {-1,0,-1,0};
  const uint64x1_t nextblock = {1};
  uint32x4_t k0k1k2k3 = (uint32x4_t) vld1q_u8((uint8_t *) k);
  uint32x4_t k4k5k6k7 = (uint32x4_t) vld1q_u8((uint8_t *) (k + 16));
  uint32x4_t start0 = (uint32x4_t) vld1q_u8((uint8_t *) sigma);
  uint32x2_t n0n1 = (uint32x2_t) vld1_u8((uint8_t *) n);
  uint32x2_t n2n3 = {0,0};
  uint32x2_t k0k1 = vget_low_u32(k0k1k2k3);
  uint32x2_t k2k3 = vget_high_u32(k0k1k2k3);
  uint32x2_t k4k5 = vget_low_u32(k4k5k6k7);
  uint32x2_t k6k7 = vget_high_u32(k4k5k6k7);
  uint32x2_t n1n0 = vext_u32(n0n1,n0n1,1);
  uint32x2_t n3n2;
  uint32x2_t n0k4 = vext_u32(n1n0,k4k5,1);
  uint32x2_t k5k0 = vext_u32(k4k5,k0k1,1);
  uint32x2_t k1n1 = vext_u32(k0k1,n1n0,1);
  uint32x2_t n2k6;
  uint32x2_t k7k2 = vext_u32(k6k7,k2k3,1);
  uint32x2_t k3n3;
  uint32x4_t start1 = vcombine_u32(k5k0,n0k4);
  uint32x4_t start2;
  uint32x4_t start3;
  register uint32x4_t diag0;
  register uint32x4_t diag1;
  register uint32x4_t diag2;
  register uint32x4_t diag3;
  uint32x4_t next_start2;
  uint32x4_t next_start3;
  register uint32x4_t next_diag0;
  register uint32x4_t next_diag1;
  register uint32x4_t next_diag2;
  register uint32x4_t next_diag3;
  uint32x4_t x0x5x10x15;
  uint32x4_t x12x1x6x11;
  uint32x4_t x8x13x2x7;
  uint32x4_t x4x9x14x3;
  uint32x4_t x0x1x10x11;
  uint32x4_t x12x13x6x7;
  uint32x4_t x8x9x2x3;
  uint32x4_t x4x5x14x15;
  uint32x4_t x0x1x2x3;
  uint32x4_t x4x5x6x7;
  uint32x4_t x8x9x10x11;
  uint32x4_t x12x13x14x15;
  uint32x4_t m0m1m2m3;
  uint32x4_t m4m5m6m7;
  uint32x4_t m8m9m10m11;
  uint32x4_t m12m13m14m15;
  register uint32x4_t a0;
  register uint32x4_t a1;
  register uint32x4_t a2;
  register uint32x4_t a3;
  register uint32x4_t b0;
  register uint32x4_t b1;
  register uint32x4_t b2;
  register uint32x4_t b3;
  register uint32x4_t next_a0;
  register uint32x4_t next_a1;
  register uint32x4_t next_a2;
  register uint32x4_t next_a3;
  register uint32x4_t next_b0;
  register uint32x4_t next_b1;
  register uint32x4_t next_b2;
  register uint32x4_t next_b3;
  unsigned char block[64];
  unsigned char *savec;
  int i;
  int flagm = (m != 0);

  if (!mlen) return 0;
  if (mlen < 128) goto mlenatleast1;

  mlenatleast128:

  n3n2 = vext_u32(n2n3,n2n3,1);
  n2k6 = vext_u32(n3n2,k6k7,1);
  k3n3 = vext_u32(k2k3,n3n2,1);
  start2 = vcombine_u32(n2k6,k1n1);
  start3 = vcombine_u32(k3n3,k7k2);

  n2n3 = (uint32x2_t) vadd_u64(nextblock,(uint64x1_t) n2n3);

  diag0 = start0;
  diag1 = start1;
  diag2 = start2;
  diag3 = start3;

  n3n2 = vext_u32(n2n3,n2n3,1);
  n2k6 = vext_u32(n3n2,k6k7,1);
  k3n3 = vext_u32(k2k3,n3n2,1);
  next_start2 = vcombine_u32(n2k6,k1n1);
  next_start3 = vcombine_u32(k3n3,k7k2);

  n2n3 = (uint32x2_t) vadd_u64(nextblock,(uint64x1_t) n2n3);

  next_diag0 = start0;
  next_diag1 = start1;
  next_diag2 = next_start2;
  next_diag3 = next_start3;

  for (i = ROUNDS;i > 0;i -= 2) {
    a0 = diag1 + diag0;
    b0 = vshlq_n_u32(a0,7);
                                        next_a0 = next_diag1 + next_diag0;
    a0 = vsriq_n_u32(b0,a0,25);
                                        next_b0 = vshlq_n_u32(next_a0,7);
    diag3 ^= a0;
                                        next_a0 = vsriq_n_u32(next_b0,next_a0,25);
    a1 = diag0 + diag3;
                                        next_diag3 ^= next_a0;
    b1 = vshlq_n_u32(a1,9);
                                        next_a1 = next_diag0 + next_diag3;
    a1 = vsriq_n_u32(b1,a1,23);
                                        next_b1 = vshlq_n_u32(next_a1,9);
    diag2 ^= a1;
                                        next_a1 = vsriq_n_u32(next_b1,next_a1,23);
    a2 = diag3 + diag2;
      diag3 = vextq_u32(diag3,diag3,3);
                                        next_diag2 ^= next_a1;
    b2 = vshlq_n_u32(a2,13);
                                        next_a2 = next_diag3 + next_diag2;
                                          next_diag3 = vextq_u32(next_diag3,next_diag3,3);
    a2 = vsriq_n_u32(b2,a2,19);
                                        next_b2 = vshlq_n_u32(next_a2,13);
    diag1 ^= a2;
                                        next_a2 = vsriq_n_u32(next_b2,next_a2,19);
    a3 = diag2 + diag1;
      diag2 = vextq_u32(diag2,diag2,2);
                                        next_diag1 ^= next_a2;
    b3 = vshlq_n_u32(a3,18);
      diag1 = vextq_u32(diag1,diag1,1);
                                        next_a3 = next_diag2 + next_diag1;
                                          next_diag2 = vextq_u32(next_diag2,next_diag2,2);
    a3 = vsriq_n_u32(b3,a3,14);
                                        next_b3 = vshlq_n_u32(next_a3,18);
                                          next_diag1 = vextq_u32(next_diag1,next_diag1,1);
    diag0 ^= a3;
                                        next_a3 = vsriq_n_u32(next_b3,next_a3,14);
    a0 = diag3 + diag0;
                                        next_diag0 ^= next_a3;
    b0 = vshlq_n_u32(a0,7);
                                        next_a0 = next_diag3 + next_diag0;
    a0 = vsriq_n_u32(b0,a0,25);
                                        next_b0 = vshlq_n_u32(next_a0,7);
    diag1 ^= a0;
                                        next_a0 = vsriq_n_u32(next_b0,next_a0,25);
    a1 = diag0 + diag1;
                                        next_diag1 ^= next_a0;
    b1 = vshlq_n_u32(a1,9);
                                        next_a1 = next_diag0 + next_diag1;
    a1 = vsriq_n_u32(b1,a1,23);
                                        next_b1 = vshlq_n_u32(next_a1,9);
    diag2 ^= a1;
                                        next_a1 = vsriq_n_u32(next_b1,next_a1,23);
    a2 = diag1 + diag2;
      diag1 = vextq_u32(diag1,diag1,3);
                                        next_diag2 ^= next_a1;
    b2 = vshlq_n_u32(a2,13);
                                        next_a2 = next_diag1 + next_diag2;
                                          next_diag1 = vextq_u32(next_diag1,next_diag1,3);
    a2 = vsriq_n_u32(b2,a2,19);
                                        next_b2 = vshlq_n_u32(next_a2,13);
    diag3 ^= a2;
                                        next_a2 = vsriq_n_u32(next_b2,next_a2,19);
    a3 = diag2 + diag3;
      diag2 = vextq_u32(diag2,diag2,2);
                                        next_diag3 ^= next_a2;
    b3 = vshlq_n_u32(a3,18);
      diag3 = vextq_u32(diag3,diag3,1);
                                        next_a3 = next_diag2 + next_diag3;
                                          next_diag2 = vextq_u32(next_diag2,next_diag2,2);
    a3 = vsriq_n_u32(b3,a3,14);
                                        next_b3 = vshlq_n_u32(next_a3,18);
                                          next_diag3 = vextq_u32(next_diag3,next_diag3,1);
    diag0 ^= a3;
                                        next_a3 = vsriq_n_u32(next_b3,next_a3,14);
                                        next_diag0 ^= next_a3;
  }

  x0x5x10x15 = diag0 + start0;
  x12x1x6x11 = diag1 + start1;
  x8x13x2x7 = diag2 + start2;
  x4x9x14x3 = diag3 + start3;

  if (flagm) m0m1m2m3 = (uint32x4_t) vld1q_u8((uint8_t *) m);
  if (flagm) m4m5m6m7 = (uint32x4_t) vld1q_u8(16 + (uint8_t *) m);
  if (flagm) m8m9m10m11 = (uint32x4_t) vld1q_u8(32 + (uint8_t *) m);
  if (flagm) m12m13m14m15 = (uint32x4_t) vld1q_u8(48 + (uint8_t *) m);

  x0x1x10x11 = vbslq_u32(abab,x0x5x10x15,x12x1x6x11);
  x12x13x6x7 = vbslq_u32(abab,x12x1x6x11,x8x13x2x7);
  x8x9x2x3 = vbslq_u32(abab,x8x13x2x7,x4x9x14x3);
  x4x5x14x15 = vbslq_u32(abab,x4x9x14x3,x0x5x10x15);

  x0x1x2x3 = vcombine_u32(vget_low_u32(x0x1x10x11),vget_high_u32(x8x9x2x3));
  x4x5x6x7 = vcombine_u32(vget_low_u32(x4x5x14x15),vget_high_u32(x12x13x6x7));
  x8x9x10x11 = vcombine_u32(vget_low_u32(x8x9x2x3),vget_high_u32(x0x1x10x11));
  x12x13x14x15 = vcombine_u32(vget_low_u32(x12x13x6x7),vget_high_u32(x4x5x14x15));

  if (flagm) x0x1x2x3 ^= m0m1m2m3;
  if (flagm) x4x5x6x7 ^= m4m5m6m7;
  if (flagm) x8x9x10x11 ^= m8m9m10m11;
  if (flagm) x12x13x14x15 ^= m12m13m14m15;

  vst1q_u8((uint8_t *) c,(uint8x16_t) x0x1x2x3);
  vst1q_u8(16 + (uint8_t *) c,(uint8x16_t) x4x5x6x7);
  vst1q_u8(32 + (uint8_t *) c,(uint8x16_t) x8x9x10x11);
  vst1q_u8(48 + (uint8_t *) c,(uint8x16_t) x12x13x14x15);

  x0x5x10x15 = next_diag0 + start0;
  x12x1x6x11 = next_diag1 + start1;
  x8x13x2x7 = next_diag2 + next_start2;
  x4x9x14x3 = next_diag3 + next_start3;

  if (flagm) m0m1m2m3 = (uint32x4_t) vld1q_u8(64 + (uint8_t *) m);
  if (flagm) m4m5m6m7 = (uint32x4_t) vld1q_u8(80 + (uint8_t *) m);
  if (flagm) m8m9m10m11 = (uint32x4_t) vld1q_u8(96 + (uint8_t *) m);
  if (flagm) m12m13m14m15 = (uint32x4_t) vld1q_u8(112 + (uint8_t *) m);

  x0x1x10x11 = vbslq_u32(abab,x0x5x10x15,x12x1x6x11);
  x12x13x6x7 = vbslq_u32(abab,x12x1x6x11,x8x13x2x7);
  x8x9x2x3 = vbslq_u32(abab,x8x13x2x7,x4x9x14x3);
  x4x5x14x15 = vbslq_u32(abab,x4x9x14x3,x0x5x10x15);

  x0x1x2x3 = vcombine_u32(vget_low_u32(x0x1x10x11),vget_high_u32(x8x9x2x3));
  x4x5x6x7 = vcombine_u32(vget_low_u32(x4x5x14x15),vget_high_u32(x12x13x6x7));
  x8x9x10x11 = vcombine_u32(vget_low_u32(x8x9x2x3),vget_high_u32(x0x1x10x11));
  x12x13x14x15 = vcombine_u32(vget_low_u32(x12x13x6x7),vget_high_u32(x4x5x14x15));

  if (flagm) x0x1x2x3 ^= m0m1m2m3;
  if (flagm) x4x5x6x7 ^= m4m5m6m7;
  if (flagm) x8x9x10x11 ^= m8m9m10m11;
  if (flagm) x12x13x14x15 ^= m12m13m14m15;

  vst1q_u8(64 + (uint8_t *) c,(uint8x16_t) x0x1x2x3);
  vst1q_u8(80 + (uint8_t *) c,(uint8x16_t) x4x5x6x7);
  vst1q_u8(96 + (uint8_t *) c,(uint8x16_t) x8x9x10x11);
  vst1q_u8(112 + (uint8_t *) c,(uint8x16_t) x12x13x14x15);

  mlen -= 128;
  c += 128;
  if (flagm) m += 128;

  if (mlen >= 128) goto mlenatleast128;

  mlenatleast1:

  if (mlen < 64) {
    if (flagm) for (i = 0;i < 64;++i) block[i] = 0;
    if (flagm) for (i = 0;i < mlen;++i) block[i] = m[i];
    savec = c;
    c = block;
    if (flagm) m = block;
  }

  n3n2 = vext_u32(n2n3,n2n3,1);
  n2k6 = vext_u32(n3n2,k6k7,1);
  k3n3 = vext_u32(k2k3,n3n2,1);
  start2 = vcombine_u32(n2k6,k1n1);
  start3 = vcombine_u32(k3n3,k7k2);

  diag0 = start0;
  diag1 = start1;
  diag2 = start2;
  diag3 = start3;

  for (i = ROUNDS;i > 0;i -= 2) {
    a0 = diag1 + diag0;
    b0 = vshlq_n_u32(a0,7);
    a0 = vsriq_n_u32(b0,a0,25);
    diag3 ^= a0;
    a1 = diag0 + diag3;
    b1 = vshlq_n_u32(a1,9);
    a1 = vsriq_n_u32(b1,a1,23);
    diag2 ^= a1;
    a2 = diag3 + diag2;
      diag3 = vextq_u32(diag3,diag3,3);
    b2 = vshlq_n_u32(a2,13);
    a2 = vsriq_n_u32(b2,a2,19);
    diag1 ^= a2;
    a3 = diag2 + diag1;
      diag2 = vextq_u32(diag2,diag2,2);
    b3 = vshlq_n_u32(a3,18);
      diag1 = vextq_u32(diag1,diag1,1);
    a3 = vsriq_n_u32(b3,a3,14);
    diag0 ^= a3;

    a0 = diag3 + diag0;
    b0 = vshlq_n_u32(a0,7);
    a0 = vsriq_n_u32(b0,a0,25);
    diag1 ^= a0;
    a1 = diag0 + diag1;
    b1 = vshlq_n_u32(a1,9);
    a1 = vsriq_n_u32(b1,a1,23);
    diag2 ^= a1;
    a2 = diag1 + diag2;
      diag1 = vextq_u32(diag1,diag1,3);
    b2 = vshlq_n_u32(a2,13);
    a2 = vsriq_n_u32(b2,a2,19);
    diag3 ^= a2;
    a3 = diag2 + diag3;
      diag2 = vextq_u32(diag2,diag2,2);
    b3 = vshlq_n_u32(a3,18);
      diag3 = vextq_u32(diag3,diag3,1);
    a3 = vsriq_n_u32(b3,a3,14);
    diag0 ^= a3;
  }

  x0x5x10x15 = diag0 + start0;
  x12x1x6x11 = diag1 + start1;
  x8x13x2x7 = diag2 + start2;
  x4x9x14x3 = diag3 + start3;

  if (flagm) m0m1m2m3 = (uint32x4_t) vld1q_u8((uint8_t *) m);
  if (flagm) m4m5m6m7 = (uint32x4_t) vld1q_u8(16 + (uint8_t *) m);
  if (flagm) m8m9m10m11 = (uint32x4_t) vld1q_u8(32 + (uint8_t *) m);
  if (flagm) m12m13m14m15 = (uint32x4_t) vld1q_u8(48 + (uint8_t *) m);

  x0x1x10x11 = vbslq_u32(abab,x0x5x10x15,x12x1x6x11);
  x12x13x6x7 = vbslq_u32(abab,x12x1x6x11,x8x13x2x7);
  x8x9x2x3 = vbslq_u32(abab,x8x13x2x7,x4x9x14x3);
  x4x5x14x15 = vbslq_u32(abab,x4x9x14x3,x0x5x10x15);

  x0x1x2x3 = vcombine_u32(vget_low_u32(x0x1x10x11),vget_high_u32(x8x9x2x3));
  x4x5x6x7 = vcombine_u32(vget_low_u32(x4x5x14x15),vget_high_u32(x12x13x6x7));
  x8x9x10x11 = vcombine_u32(vget_low_u32(x8x9x2x3),vget_high_u32(x0x1x10x11));
  x12x13x14x15 = vcombine_u32(vget_low_u32(x12x13x6x7),vget_high_u32(x4x5x14x15));

  if (flagm) x0x1x2x3 ^= m0m1m2m3;
  if (flagm) x4x5x6x7 ^= m4m5m6m7;
  if (flagm) x8x9x10x11 ^= m8m9m10m11;
  if (flagm) x12x13x14x15 ^= m12m13m14m15;

  vst1q_u8((uint8_t *) c,(uint8x16_t) x0x1x2x3);
  vst1q_u8(16 + (uint8_t *) c,(uint8x16_t) x4x5x6x7);
  vst1q_u8(32 + (uint8_t *) c,(uint8x16_t) x8x9x10x11);
  vst1q_u8(48 + (uint8_t *) c,(uint8x16_t) x12x13x14x15);

  if (mlen < 64) {
    for (i = 0;i < mlen;++i) savec[i] = c[i];
  }
  if (mlen <= 64) return 0;

  n2n3 = (uint32x2_t) vadd_u64(nextblock,(uint64x1_t) n2n3);

  mlen -= 64;
  c += 64;
  if (flagm) m += 64;

  goto mlenatleast1;
}
