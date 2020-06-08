/*
salsa20-xmm6int.c version $Date: 2014/09/24 12:07:11 $
D. J. Bernstein
Romain Dolbeau
Public domain.
*/

#include "e/ecrypt-sync.h"
#include "api.h"

#include <immintrin.h>
#include <stdio.h>

#define ROTATE(v,c) (ROTL32(v,c))
#define XOR(v,w) ((v) ^ (w))
#define PLUS(v,w) (U32V((v) + (w)))
#define PLUSONE(v) (PLUS((v),1))

/* Compatibility layer. This array help translates
   to the storage format in the SIMD code. */
static const int tr[16] = {  0,  5, 10, 15,
                            12,  1,  6, 11,
                             8, 13,  2,  7,
                             4,  9, 14,  3 };

/* SIMD format-compatible scalar code. This should be replaced by
   more efficient code, but so far this is a proof of concept. */
static void salsa20_wordtobyte_tr(u8 output[64],const u32 input[16])
{
  u32 x[16];
  int i;

  for (i = 0;i < 16;++i) x[tr[i]] = input[tr[i]];
  for (i = 20;i > 0;i -= 2) {
    x[tr[ 4]] = XOR(x[tr[ 4]],ROTATE(PLUS(x[tr[ 0]],x[tr[12]]), 7));
    x[tr[ 8]] = XOR(x[tr[ 8]],ROTATE(PLUS(x[tr[ 4]],x[tr[ 0]]), 9));
    x[tr[12]] = XOR(x[tr[12]],ROTATE(PLUS(x[tr[ 8]],x[tr[ 4]]),13));
    x[tr[ 0]] = XOR(x[tr[ 0]],ROTATE(PLUS(x[tr[12]],x[tr[ 8]]),18));
    x[tr[ 9]] = XOR(x[tr[ 9]],ROTATE(PLUS(x[tr[ 5]],x[tr[ 1]]), 7));
    x[tr[13]] = XOR(x[tr[13]],ROTATE(PLUS(x[tr[ 9]],x[tr[ 5]]), 9));
    x[tr[ 1]] = XOR(x[tr[ 1]],ROTATE(PLUS(x[tr[13]],x[tr[ 9]]),13));
    x[tr[ 5]] = XOR(x[tr[ 5]],ROTATE(PLUS(x[tr[ 1]],x[tr[13]]),18));
    x[tr[14]] = XOR(x[tr[14]],ROTATE(PLUS(x[tr[10]],x[tr[ 6]]), 7));
    x[tr[ 2]] = XOR(x[tr[ 2]],ROTATE(PLUS(x[tr[14]],x[tr[10]]), 9));
    x[tr[ 6]] = XOR(x[tr[ 6]],ROTATE(PLUS(x[tr[ 2]],x[tr[14]]),13));
    x[tr[10]] = XOR(x[tr[10]],ROTATE(PLUS(x[tr[ 6]],x[tr[ 2]]),18));
    x[tr[ 3]] = XOR(x[tr[ 3]],ROTATE(PLUS(x[tr[15]],x[tr[11]]), 7));
    x[tr[ 7]] = XOR(x[tr[ 7]],ROTATE(PLUS(x[tr[ 3]],x[tr[15]]), 9));
    x[tr[11]] = XOR(x[tr[11]],ROTATE(PLUS(x[tr[ 7]],x[tr[ 3]]),13));
    x[tr[15]] = XOR(x[tr[15]],ROTATE(PLUS(x[tr[11]],x[tr[ 7]]),18));
    x[tr[ 1]] = XOR(x[tr[ 1]],ROTATE(PLUS(x[tr[ 0]],x[tr[ 3]]), 7));
    x[tr[ 2]] = XOR(x[tr[ 2]],ROTATE(PLUS(x[tr[ 1]],x[tr[ 0]]), 9));
    x[tr[ 3]] = XOR(x[tr[ 3]],ROTATE(PLUS(x[tr[ 2]],x[tr[ 1]]),13));
    x[tr[ 0]] = XOR(x[tr[ 0]],ROTATE(PLUS(x[tr[ 3]],x[tr[ 2]]),18));
    x[tr[ 6]] = XOR(x[tr[ 6]],ROTATE(PLUS(x[tr[ 5]],x[tr[ 4]]), 7));
    x[tr[ 7]] = XOR(x[tr[ 7]],ROTATE(PLUS(x[tr[ 6]],x[tr[ 5]]), 9));
    x[tr[ 4]] = XOR(x[tr[ 4]],ROTATE(PLUS(x[tr[ 7]],x[tr[ 6]]),13));
    x[tr[ 5]] = XOR(x[tr[ 5]],ROTATE(PLUS(x[tr[ 4]],x[tr[ 7]]),18));
    x[tr[11]] = XOR(x[tr[11]],ROTATE(PLUS(x[tr[10]],x[tr[ 9]]), 7));
    x[tr[ 8]] = XOR(x[tr[ 8]],ROTATE(PLUS(x[tr[11]],x[tr[10]]), 9));
    x[tr[ 9]] = XOR(x[tr[ 9]],ROTATE(PLUS(x[tr[ 8]],x[tr[11]]),13));
    x[tr[10]] = XOR(x[tr[10]],ROTATE(PLUS(x[tr[ 9]],x[tr[ 8]]),18));
    x[tr[12]] = XOR(x[tr[12]],ROTATE(PLUS(x[tr[15]],x[tr[14]]), 7));
    x[tr[13]] = XOR(x[tr[13]],ROTATE(PLUS(x[tr[12]],x[tr[15]]), 9));
    x[tr[14]] = XOR(x[tr[14]],ROTATE(PLUS(x[tr[13]],x[tr[12]]),13));
    x[tr[15]] = XOR(x[tr[15]],ROTATE(PLUS(x[tr[14]],x[tr[13]]),18));
  }
  for (i = 0;i < 16;++i) x[tr[i]] = PLUS(x[tr[i]],input[tr[i]]);
  for (i = 0;i < 16;++i) U32TO8_LITTLE(output + 4 * i,x[tr[i]]);
}

void ECRYPT_encrypt_bytes(ECRYPT_ctx *x_, const u8 *m_,u8 *c_,u32 bytes)
{
  int i;
  u32* x = (u32*)&x_->input;
  u8* out = c_;
  const u8* m = m_;

#if defined(__AVX512F__)
#include "u16.h"
#endif

#if defined(__AVX2__)
#include "u8.h"
#endif

#if 1
#include "u4.h"
#endif

#if 1
#include "u1.h"
#endif

  if (!bytes) return;
  u8 output[64];
  for (;;) {
    salsa20_wordtobyte_tr(output,x);
    x[tr[8]] = PLUSONE(x[tr[8]]);
    if (!x[tr[8]]) {
      x[tr[9]] = PLUSONE(x[tr[9]]);
      /* stopping at 2^70 bytes per nonce is user's responsibility */
    }
    if (bytes <= 64) {
      for (i = 0;i < bytes;++i) out[i] = m[i] ^ output[i];
      return;
    }
    for (i = 0;i < 64;++i) out[i] = m[i] ^ output[i];
    bytes -= 64;
    out += 64;
    m += 64;
  }

}

void ECRYPT_init(void)
{
  return;
}

static const char sigma[16] = "expand 32-byte k";
static const char tau[16] = "expand 16-byte k";

/* This initializes in SIMD format */
void ECRYPT_keysetup(ECRYPT_ctx *x,const u8 *k,u32 kbits,u32 ivbits)
{
  const char *constants;

  x->input[tr[1]] = U8TO32_LITTLE(k + 0);
  x->input[tr[2]] = U8TO32_LITTLE(k + 4);
  x->input[tr[3]] = U8TO32_LITTLE(k + 8);
  x->input[tr[4]] = U8TO32_LITTLE(k + 12);
  if (kbits == 256) { /* recommended */
    k += 16;
    constants = sigma;
  } else { /* kbits == 128 */
    constants = tau;
  }
  x->input[tr[11]] = U8TO32_LITTLE(k + 0);
  x->input[tr[12]] = U8TO32_LITTLE(k + 4);
  x->input[tr[13]] = U8TO32_LITTLE(k + 8);
  x->input[tr[14]] = U8TO32_LITTLE(k + 12);
  x->input[tr[0]] = U8TO32_LITTLE(constants + 0);
  x->input[tr[5]] = U8TO32_LITTLE(constants + 4);
  x->input[tr[10]] = U8TO32_LITTLE(constants + 8);
  x->input[tr[15]] = U8TO32_LITTLE(constants + 12);
}

/* This initializes in SIMD format */
void ECRYPT_ivsetup(ECRYPT_ctx *x,const u8 *iv)
{
  x->input[tr[6]] = U8TO32_LITTLE(iv + 0);
  x->input[tr[7]] = U8TO32_LITTLE(iv + 4);
  x->input[tr[8]] = 0;
  x->input[tr[9]] = 0;
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
