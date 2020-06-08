/*
salsa20-regs.c version 20051118
D. J. Bernstein
Public domain.
*/

#include "ecrypt-sync.h"

#define ROTATE(v,c) (ROTL32(v,c))
#define XOR(v,w) ((v) ^ (w))
#define PLUS(v,w) (U32V((v) + (w)))
#define PLUSONE(v) (PLUS((v),1))

static void salsa20_wordtobyte(u8 output[64],const u32 input[16])
{
  u32 x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15;
  int i;

  x0 = input[0];
  x1 = input[1];
  x2 = input[2];
  x3 = input[3];
  x4 = input[4];
  x5 = input[5];
  x6 = input[6];
  x7 = input[7];
  x8 = input[8];
  x9 = input[9];
  x10 = input[10];
  x11 = input[11];
  x12 = input[12];
  x13 = input[13];
  x14 = input[14];
  x15 = input[15];
  for (i = 20;i > 0;i -= 2) {
     x4 = XOR( x4,ROTATE(PLUS( x0,x12), 7));
     x8 = XOR( x8,ROTATE(PLUS( x4, x0), 9));
    x12 = XOR(x12,ROTATE(PLUS( x8, x4),13));
     x0 = XOR( x0,ROTATE(PLUS(x12, x8),18));
     x9 = XOR( x9,ROTATE(PLUS( x5, x1), 7));
    x13 = XOR(x13,ROTATE(PLUS( x9, x5), 9));
     x1 = XOR( x1,ROTATE(PLUS(x13, x9),13));
     x5 = XOR( x5,ROTATE(PLUS( x1,x13),18));
    x14 = XOR(x14,ROTATE(PLUS(x10, x6), 7));
     x2 = XOR( x2,ROTATE(PLUS(x14,x10), 9));
     x6 = XOR( x6,ROTATE(PLUS( x2,x14),13));
    x10 = XOR(x10,ROTATE(PLUS( x6, x2),18));
     x3 = XOR( x3,ROTATE(PLUS(x15,x11), 7));
     x7 = XOR( x7,ROTATE(PLUS( x3,x15), 9));
    x11 = XOR(x11,ROTATE(PLUS( x7, x3),13));
    x15 = XOR(x15,ROTATE(PLUS(x11, x7),18));
     x1 = XOR( x1,ROTATE(PLUS( x0, x3), 7));
     x2 = XOR( x2,ROTATE(PLUS( x1, x0), 9));
     x3 = XOR( x3,ROTATE(PLUS( x2, x1),13));
     x0 = XOR( x0,ROTATE(PLUS( x3, x2),18));
     x6 = XOR( x6,ROTATE(PLUS( x5, x4), 7));
     x7 = XOR( x7,ROTATE(PLUS( x6, x5), 9));
     x4 = XOR( x4,ROTATE(PLUS( x7, x6),13));
     x5 = XOR( x5,ROTATE(PLUS( x4, x7),18));
    x11 = XOR(x11,ROTATE(PLUS(x10, x9), 7));
     x8 = XOR( x8,ROTATE(PLUS(x11,x10), 9));
     x9 = XOR( x9,ROTATE(PLUS( x8,x11),13));
    x10 = XOR(x10,ROTATE(PLUS( x9, x8),18));
    x12 = XOR(x12,ROTATE(PLUS(x15,x14), 7));
    x13 = XOR(x13,ROTATE(PLUS(x12,x15), 9));
    x14 = XOR(x14,ROTATE(PLUS(x13,x12),13));
    x15 = XOR(x15,ROTATE(PLUS(x14,x13),18));
  }
  x0 = PLUS(x0,input[0]);
  x1 = PLUS(x1,input[1]);
  x2 = PLUS(x2,input[2]);
  x3 = PLUS(x3,input[3]);
  x4 = PLUS(x4,input[4]);
  x5 = PLUS(x5,input[5]);
  x6 = PLUS(x6,input[6]);
  x7 = PLUS(x7,input[7]);
  x8 = PLUS(x8,input[8]);
  x9 = PLUS(x9,input[9]);
  x10 = PLUS(x10,input[10]);
  x11 = PLUS(x11,input[11]);
  x12 = PLUS(x12,input[12]);
  x13 = PLUS(x13,input[13]);
  x14 = PLUS(x14,input[14]);
  x15 = PLUS(x15,input[15]);
  U32TO8_LITTLE(output + 0,x0);
  U32TO8_LITTLE(output + 4,x1);
  U32TO8_LITTLE(output + 8,x2);
  U32TO8_LITTLE(output + 12,x3);
  U32TO8_LITTLE(output + 16,x4);
  U32TO8_LITTLE(output + 20,x5);
  U32TO8_LITTLE(output + 24,x6);
  U32TO8_LITTLE(output + 28,x7);
  U32TO8_LITTLE(output + 32,x8);
  U32TO8_LITTLE(output + 36,x9);
  U32TO8_LITTLE(output + 40,x10);
  U32TO8_LITTLE(output + 44,x11);
  U32TO8_LITTLE(output + 48,x12);
  U32TO8_LITTLE(output + 52,x13);
  U32TO8_LITTLE(output + 56,x14);
  U32TO8_LITTLE(output + 60,x15);
}

void ECRYPT_init(void)
{
  return;
}

static const char sigma[16] = "expand 32-byte k";
static const char tau[16] = "expand 16-byte k";

void ECRYPT_keysetup(ECRYPT_ctx *x,const u8 *k,u32 kbits,u32 ivbits)
{
  const char *constants;

  x->input[1] = U8TO32_LITTLE(k + 0);
  x->input[2] = U8TO32_LITTLE(k + 4);
  x->input[3] = U8TO32_LITTLE(k + 8);
  x->input[4] = U8TO32_LITTLE(k + 12);
  if (kbits == 256) { /* recommended */
    k += 16;
    constants = sigma;
  } else { /* kbits == 128 */
    constants = tau;
  }
  x->input[11] = U8TO32_LITTLE(k + 0);
  x->input[12] = U8TO32_LITTLE(k + 4);
  x->input[13] = U8TO32_LITTLE(k + 8);
  x->input[14] = U8TO32_LITTLE(k + 12);
  x->input[0] = U8TO32_LITTLE(constants + 0);
  x->input[5] = U8TO32_LITTLE(constants + 4);
  x->input[10] = U8TO32_LITTLE(constants + 8);
  x->input[15] = U8TO32_LITTLE(constants + 12);
}

void ECRYPT_ivsetup(ECRYPT_ctx *x,const u8 *iv)
{
  x->input[6] = U8TO32_LITTLE(iv + 0);
  x->input[7] = U8TO32_LITTLE(iv + 4);
  x->input[8] = 0;
  x->input[9] = 0;
}

void ECRYPT_encrypt_bytes(ECRYPT_ctx *x,const u8 *m,u8 *c,u32 bytes)
{
  u8 output[64];
  int i;

  if (!bytes) return;
  for (;;) {
    salsa20_wordtobyte(output,x->input);
    x->input[8] = PLUSONE(x->input[8]);
    if (!x->input[8]) {
      x->input[9] = PLUSONE(x->input[9]);
      /* stopping at 2^70 bytes per nonce is user's responsibility */
    }
    if (bytes <= 64) {
      for (i = 0;i < bytes;++i) c[i] = m[i] ^ output[i];
      return;
    }
    for (i = 0;i < 64;++i) c[i] = m[i] ^ output[i];
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
