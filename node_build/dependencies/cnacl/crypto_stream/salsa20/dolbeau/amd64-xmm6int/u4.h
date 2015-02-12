/*
u4.h version $Date: 2014/09/24 12:11:29 $
D. J. Bernstein
Romain Dolbeau
Public domain.
*/

if (!bytes) return;
if (bytes >= 256)
{
  u32 in8;
  u32 in9;
  __m128i y0;
  __m128i y1;
  __m128i y2;
  __m128i y3;
  __m128i y4;
  __m128i y5;
  __m128i y6;
  __m128i y7;
  __m128i y8;
  __m128i y9;
  __m128i y10;
  __m128i y11;
  __m128i y12;
  __m128i y13;
  __m128i y14;
  __m128i y15;


  /* element broadcast immediate for _mm_shuffle_epi32 are in order:
     0x00, 0x55, 0xaa, 0xff */
  __m128i z0  = _mm_loadu_si128((__m128i*)(x +  0));
  __m128i z5  = _mm_shuffle_epi32(z0, 0x55);
  __m128i z10 = _mm_shuffle_epi32(z0, 0xaa);
  __m128i z15 = _mm_shuffle_epi32(z0, 0xff);
  z0  = _mm_shuffle_epi32(z0, 0x00);
  __m128i z1  = _mm_loadu_si128((__m128i*)(x +  4));
  __m128i z6  = _mm_shuffle_epi32(z1, 0xaa);
  __m128i z11 = _mm_shuffle_epi32(z1, 0xff);
  __m128i z12 = _mm_shuffle_epi32(z1, 0x00);
  z1  = _mm_shuffle_epi32(z1, 0x55);
  __m128i z2  = _mm_loadu_si128((__m128i*)(x +  8));
  __m128i z7  = _mm_shuffle_epi32(z2, 0xff);
  __m128i z13 = _mm_shuffle_epi32(z2, 0x55);
  z2 = _mm_shuffle_epi32(z2, 0xaa);
  /* no z8 -> first half of the nonce, will fill later */
  __m128i z3  = _mm_loadu_si128((__m128i*)(x +  12));
  __m128i z4  = _mm_shuffle_epi32(z3, 0x00);
  __m128i z14 = _mm_shuffle_epi32(z3, 0xaa);
  z3 = _mm_shuffle_epi32(z3, 0xff);
  /* no z9 -> second half of the nonce, will fill later */
  __m128i z8;
  __m128i z9;
  __m128i orig0 = z0;
  __m128i orig1 = z1;
  __m128i orig2 = z2;
  __m128i orig3 = z3;
  __m128i orig4 = z4;
  __m128i orig5 = z5;
  __m128i orig6 = z6;
  __m128i orig7 = z7;
  __m128i orig8;
  __m128i orig9;
  __m128i orig10 = z10;
  __m128i orig11 = z11;
  __m128i orig12 = z12;
  __m128i orig13 = z13;
  __m128i orig14 = z14;
  __m128i orig15 = z15;
  while (bytes >= 256) {
#if 1
    /* vector implementation for z8 and z9 */
    /* not sure if it helps for only 4 blocks */
    const __m128i addv8 = _mm_set_epi64x(1,0);
    const __m128i addv9 = _mm_set_epi64x(3,2);
    __m128i t8, t9;
    in8 = x[8];
    in9 = x[13]; // see arrays above for the address translation
    u64 in89 = ((u64)in8) | (((u64)in9) << 32);
    t8 = _mm_set1_epi64x(in89);
    t9 = _mm_set1_epi64x(in89);

    z8 = _mm_add_epi64(addv8, t8);
    z9 = _mm_add_epi64(addv9, t9);

    t8 = _mm_unpacklo_epi32(z8, z9);
    t9 = _mm_unpackhi_epi32(z8, z9);

    z8 = _mm_unpacklo_epi32(t8, t9);
    z9 = _mm_unpackhi_epi32(t8, t9);

    orig8 = z8;
    orig9 = z9;

    in89 += 4;
    
    x[8] = in89 & 0xFFFFFFFF;
    x[13] = (in89>>32)&0xFFFFFFFF;
#else
    in8 = x[8];
    in9 = x[13]; // see arrays above for the address translation
    /*     z8 = _mm_insert_epi32(z8, in8, 0); */
    /*     z9 = _mm_insert_epi32(z9, in9, 0); */
    /* might be more efficient to use  _mm_cvtsi32_si128 ? */
    z8 = _mm_cvtsi32_si128(in8);
    z9 = _mm_cvtsi32_si128(in9);
    in8++;
    if (in8 == 0)
      in9++;
    z8 = _mm_insert_epi32(z8, in8, 1);
    z9 = _mm_insert_epi32(z9, in9, 1);
    in8++;
    if (in8 == 0)
      in9++;
    z8 = _mm_insert_epi32(z8, in8, 2);
    z9 = _mm_insert_epi32(z9, in9, 2);
    in8++;
    if (in8 == 0)
      in9++;
    z8 = _mm_insert_epi32(z8, in8, 3);
    z9 = _mm_insert_epi32(z9, in9, 3);
    orig8 = z8;
    orig9 = z9;
    in8++;
    if (in8 == 0)
      in9++;
    x[8] = in8;
    x[13] = in9;
#endif

    z5 = orig5;
    z10 = orig10;
    z15 = orig15;
    z14 = orig14;
    z3 = orig3;
    z6 = orig6;
    z11 = orig11;
    z1 = orig1;

    z7 = orig7;
    z13 = orig13;
    z2 = orig2;
    z9 = orig9;
    z0 = orig0;
    z12 = orig12;
    z4 = orig4;
    z8 = orig8;

    for (i = 0 ; i < 20 ; i+=2) {
      /* the inner loop is a direct translation (regexp search/replace) from the amd64-xmm6 ASM */
      __m128i r0;
      __m128i r1;
      __m128i r2;
      __m128i r3;
      __m128i r4;
      __m128i r5;
      __m128i r6;
      __m128i r7;
      __m128i r8;
      __m128i r9;
      __m128i r10;
      __m128i r11;
      __m128i r12;
      __m128i r13;
      __m128i r14;
      __m128i r15;
        
      y4 = z12;
      y4 = _mm_add_epi32(y4, z0);
      r4 = y4;
      y4 = _mm_slli_epi32(y4, 7);
      z4 = _mm_xor_si128(z4, y4);
      r4 = _mm_srli_epi32(r4, 25);
      z4 = _mm_xor_si128(z4, r4);

      y9 = z1;
      y9 = _mm_add_epi32(y9, z5);
      r9 = y9;
      y9 = _mm_slli_epi32(y9, 7);
      z9 = _mm_xor_si128(z9, y9);
      r9 = _mm_srli_epi32(r9, 25);
      z9 = _mm_xor_si128(z9, r9);

      y8 = z0;
      y8 = _mm_add_epi32(y8, z4);
      r8 = y8;
      y8 = _mm_slli_epi32(y8, 9);
      z8 = _mm_xor_si128(z8, y8);
      r8 = _mm_srli_epi32(r8, 23);
      z8 = _mm_xor_si128(z8, r8);

      y13 = z5;
      y13 = _mm_add_epi32(y13, z9);
      r13 = y13;
      y13 = _mm_slli_epi32(y13, 9);
      z13 = _mm_xor_si128(z13, y13);
      r13 = _mm_srli_epi32(r13, 23);
      z13 = _mm_xor_si128(z13, r13);

      y12 = z4;
      y12 = _mm_add_epi32(y12, z8);
      r12 = y12;
      y12 = _mm_slli_epi32(y12, 13);
      z12 = _mm_xor_si128(z12, y12);
      r12 = _mm_srli_epi32(r12, 19);
      z12 = _mm_xor_si128(z12, r12);

      y1 = z9;
      y1 = _mm_add_epi32(y1, z13);
      r1 = y1;
      y1 = _mm_slli_epi32(y1, 13);
      z1 = _mm_xor_si128(z1, y1);
      r1 = _mm_srli_epi32(r1, 19);
      z1 = _mm_xor_si128(z1, r1);

      y0 = z8;
      y0 = _mm_add_epi32(y0, z12);
      r0 = y0;
      y0 = _mm_slli_epi32(y0, 18);
      z0 = _mm_xor_si128(z0, y0);
      r0 = _mm_srli_epi32(r0, 14);
      z0 = _mm_xor_si128(z0, r0);


      y5 = z13;
      y5 = _mm_add_epi32(y5, z1);
      r5 = y5;
      y5 = _mm_slli_epi32(y5, 18);
      z5 = _mm_xor_si128(z5, y5);
      r5 = _mm_srli_epi32(r5, 14);
      z5 = _mm_xor_si128(z5, r5);

      y14 = z6;
      y14 = _mm_add_epi32(y14, z10);
      r14 = y14;
      y14 = _mm_slli_epi32(y14, 7);
      z14 = _mm_xor_si128(z14, y14);
      r14 = _mm_srli_epi32(r14, 25);
      z14 = _mm_xor_si128(z14, r14);


      y3 = z11;
      y3 = _mm_add_epi32(y3, z15);
      r3 = y3;
      y3 = _mm_slli_epi32(y3, 7);
      z3 = _mm_xor_si128(z3, y3);
      r3 = _mm_srli_epi32(r3, 25);
      z3 = _mm_xor_si128(z3, r3);

      y2 = z10;
      y2 = _mm_add_epi32(y2, z14);
      r2 = y2;
      y2 = _mm_slli_epi32(y2, 9);
      z2 = _mm_xor_si128(z2, y2);
      r2 = _mm_srli_epi32(r2, 23);
      z2 = _mm_xor_si128(z2, r2);

      y7 = z15;
      y7 = _mm_add_epi32(y7, z3);
      r7 = y7;
      y7 = _mm_slli_epi32(y7, 9);
      z7 = _mm_xor_si128(z7, y7);
      r7 = _mm_srli_epi32(r7, 23);
      z7 = _mm_xor_si128(z7, r7);

      y6 = z14;
      y6 = _mm_add_epi32(y6, z2);
      r6 = y6;
      y6 = _mm_slli_epi32(y6, 13);
      z6 = _mm_xor_si128(z6, y6);
      r6 = _mm_srli_epi32(r6, 19);
      z6 = _mm_xor_si128(z6, r6);

      y11 = z3;
      y11 = _mm_add_epi32(y11, z7);
      r11 = y11;
      y11 = _mm_slli_epi32(y11, 13);
      z11 = _mm_xor_si128(z11, y11);
      r11 = _mm_srli_epi32(r11, 19);
      z11 = _mm_xor_si128(z11, r11);

      y10 = z2;
      y10 = _mm_add_epi32(y10, z6);
      r10 = y10;
      y10 = _mm_slli_epi32(y10, 18);
      z10 = _mm_xor_si128(z10, y10);
      r10 = _mm_srli_epi32(r10, 14);
      z10 = _mm_xor_si128(z10, r10);


      y1 = z3;
      y1 = _mm_add_epi32(y1, z0);
      r1 = y1;
      y1 = _mm_slli_epi32(y1, 7);
      z1 = _mm_xor_si128(z1, y1);
      r1 = _mm_srli_epi32(r1, 25);
      z1 = _mm_xor_si128(z1, r1);

      y15 = z7;
      y15 = _mm_add_epi32(y15, z11);
      r15 = y15;
      y15 = _mm_slli_epi32(y15, 18);
      z15 = _mm_xor_si128(z15, y15);
      r15 = _mm_srli_epi32(r15, 14);
      z15 = _mm_xor_si128(z15, r15);


      y6 = z4;
      y6 = _mm_add_epi32(y6, z5);
      r6 = y6;
      y6 = _mm_slli_epi32(y6, 7);
      z6 = _mm_xor_si128(z6, y6);
      r6 = _mm_srli_epi32(r6, 25);
      z6 = _mm_xor_si128(z6, r6);

      y2 = z0;
      y2 = _mm_add_epi32(y2, z1);
      r2 = y2;
      y2 = _mm_slli_epi32(y2, 9);
      z2 = _mm_xor_si128(z2, y2);
      r2 = _mm_srli_epi32(r2, 23);
      z2 = _mm_xor_si128(z2, r2);

      y7 = z5;
      y7 = _mm_add_epi32(y7, z6);
      r7 = y7;
      y7 = _mm_slli_epi32(y7, 9);
      z7 = _mm_xor_si128(z7, y7);
      r7 = _mm_srli_epi32(r7, 23);
      z7 = _mm_xor_si128(z7, r7);

      y3 = z1;
      y3 = _mm_add_epi32(y3, z2);
      r3 = y3;
      y3 = _mm_slli_epi32(y3, 13);
      z3 = _mm_xor_si128(z3, y3);
      r3 = _mm_srli_epi32(r3, 19);
      z3 = _mm_xor_si128(z3, r3);

      y4 = z6;
      y4 = _mm_add_epi32(y4, z7);
      r4 = y4;
      y4 = _mm_slli_epi32(y4, 13);
      z4 = _mm_xor_si128(z4, y4);
      r4 = _mm_srli_epi32(r4, 19);
      z4 = _mm_xor_si128(z4, r4);

      y0 = z2;
      y0 = _mm_add_epi32(y0, z3);
      r0 = y0;
      y0 = _mm_slli_epi32(y0, 18);
      z0 = _mm_xor_si128(z0, y0);
      r0 = _mm_srli_epi32(r0, 14);
      z0 = _mm_xor_si128(z0, r0);


      y5 = z7;
      y5 = _mm_add_epi32(y5, z4);
      r5 = y5;
      y5 = _mm_slli_epi32(y5, 18);
      z5 = _mm_xor_si128(z5, y5);
      r5 = _mm_srli_epi32(r5, 14);
      z5 = _mm_xor_si128(z5, r5);

      y11 = z9;
      y11 = _mm_add_epi32(y11, z10);
      r11 = y11;
      y11 = _mm_slli_epi32(y11, 7);
      z11 = _mm_xor_si128(z11, y11);
      r11 = _mm_srli_epi32(r11, 25);
      z11 = _mm_xor_si128(z11, r11);



      y12 = z14;
      y12 = _mm_add_epi32(y12, z15);
      r12 = y12;
      y12 = _mm_slli_epi32(y12, 7);
      z12 = _mm_xor_si128(z12, y12);
      r12 = _mm_srli_epi32(r12, 25);
      z12 = _mm_xor_si128(z12, r12);

      y8 = z10;
      y8 = _mm_add_epi32(y8, z11);
      r8 = y8;
      y8 = _mm_slli_epi32(y8, 9);
      z8 = _mm_xor_si128(z8, y8);
      r8 = _mm_srli_epi32(r8, 23);
      z8 = _mm_xor_si128(z8, r8);

      y13 = z15;
      y13 = _mm_add_epi32(y13, z12);
      r13 = y13;
      y13 = _mm_slli_epi32(y13, 9);
      z13 = _mm_xor_si128(z13, y13);
      r13 = _mm_srli_epi32(r13, 23);
      z13 = _mm_xor_si128(z13, r13);

      y9 = z11;
      y9 = _mm_add_epi32(y9, z8);
      r9 = y9;
      y9 = _mm_slli_epi32(y9, 13);
      z9 = _mm_xor_si128(z9, y9);
      r9 = _mm_srli_epi32(r9, 19);
      z9 = _mm_xor_si128(z9, r9);

      y14 = z12;
      y14 = _mm_add_epi32(y14, z13);
      r14 = y14;
      y14 = _mm_slli_epi32(y14, 13);
      z14 = _mm_xor_si128(z14, y14);
      r14 = _mm_srli_epi32(r14, 19);
      z14 = _mm_xor_si128(z14, r14);

      y10 = z8;
      y10 = _mm_add_epi32(y10, z9);
      r10 = y10;
      y10 = _mm_slli_epi32(y10, 18);
      z10 = _mm_xor_si128(z10, y10);
      r10 = _mm_srli_epi32(r10, 14);
      z10 = _mm_xor_si128(z10, r10);

      y15 = z13;
      y15 = _mm_add_epi32(y15, z14);
      r15 = y15;
      y15 = _mm_slli_epi32(y15, 18);
      z15 = _mm_xor_si128(z15, y15);
      r15 = _mm_srli_epi32(r15, 14);
      z15 = _mm_xor_si128(z15, r15);

    }

    /* store data ; this macro replicates the original amd64-xmm6 code */
#define ONEQUAD_SHUFFLE(a,b,c,d)                \
    z##a = _mm_add_epi32(z##a, orig##a);        \
    z##b = _mm_add_epi32(z##b, orig##b);        \
    z##c = _mm_add_epi32(z##c, orig##c);        \
    z##d = _mm_add_epi32(z##d, orig##d);        \
    in##a = _mm_cvtsi128_si32(z##a);            \
    in##b = _mm_cvtsi128_si32(z##b);            \
    in##c = _mm_cvtsi128_si32(z##c);            \
    in##d = _mm_cvtsi128_si32(z##d);            \
    z##a = _mm_shuffle_epi32(z##a, 0x39);       \
    z##b = _mm_shuffle_epi32(z##b, 0x39);       \
    z##c = _mm_shuffle_epi32(z##c, 0x39);       \
    z##d = _mm_shuffle_epi32(z##d, 0x39);       \
    in##a ^= *(u32 *) (m + 0);                  \
    in##b ^= *(u32 *) (m + 4);                  \
    in##c ^= *(u32 *) (m + 8);                  \
    in##d ^= *(u32 *) (m + 12);                 \
    *(u32 *) (out + 0) = in##a;                 \
    *(u32 *) (out + 4) = in##b;                 \
    *(u32 *) (out + 8) = in##c;                 \
    *(u32 *) (out + 12) = in##d;                \
    in##a = _mm_cvtsi128_si32(z##a);            \
    in##b = _mm_cvtsi128_si32(z##b);            \
    in##c = _mm_cvtsi128_si32(z##c);            \
    in##d = _mm_cvtsi128_si32(z##d);            \
    z##a = _mm_shuffle_epi32(z##a, 0x39);       \
    z##b = _mm_shuffle_epi32(z##b, 0x39);       \
    z##c = _mm_shuffle_epi32(z##c, 0x39);       \
    z##d = _mm_shuffle_epi32(z##d, 0x39);       \
    in##a ^= *(u32 *) (m + 64);                 \
    in##b ^= *(u32 *) (m + 68);                 \
    in##c ^= *(u32 *) (m + 72);                 \
    in##d ^= *(u32 *) (m + 76);                 \
    *(u32 *) (out + 64) = in##a;                \
    *(u32 *) (out + 68) = in##b;                \
    *(u32 *) (out + 72) = in##c;                \
    *(u32 *) (out + 76) = in##d;                \
    in##a = _mm_cvtsi128_si32(z##a);            \
    in##b = _mm_cvtsi128_si32(z##b);            \
    in##c = _mm_cvtsi128_si32(z##c);            \
    in##d = _mm_cvtsi128_si32(z##d);            \
    z##a = _mm_shuffle_epi32(z##a, 0x39);       \
    z##b = _mm_shuffle_epi32(z##b, 0x39);       \
    z##c = _mm_shuffle_epi32(z##c, 0x39);       \
    z##d = _mm_shuffle_epi32(z##d, 0x39);       \
    in##a ^= *(u32 *) (m + 128);                \
    in##b ^= *(u32 *) (m + 132);                \
    in##c ^= *(u32 *) (m + 136);                \
    in##d ^= *(u32 *) (m + 140);                \
    *(u32 *) (out + 128) = in##a;               \
    *(u32 *) (out + 132) = in##b;               \
    *(u32 *) (out + 136) = in##c;               \
    *(u32 *) (out + 140) = in##d;               \
    in##a = _mm_cvtsi128_si32(z##a);            \
    in##b = _mm_cvtsi128_si32(z##b);            \
    in##c = _mm_cvtsi128_si32(z##c);            \
    in##d = _mm_cvtsi128_si32(z##d);            \
    in##a ^= *(u32 *) (m + 192);                \
    in##b ^= *(u32 *) (m + 196);                \
    in##c ^= *(u32 *) (m + 200);                \
    in##d ^= *(u32 *) (m + 204);                \
    *(u32 *) (out + 192) = in##a;               \
    *(u32 *) (out + 196) = in##b;               \
    *(u32 *) (out + 200) = in##c;               \
    *(u32 *) (out + 204) = in##d
    
    
    /* store data ; this macro replaces shuffle+mov by a direct extract; not much difference */
#define ONEQUAD_EXTRACT(a,b,c,d)                \
    z##a = _mm_add_epi32(z##a, orig##a);        \
    z##b = _mm_add_epi32(z##b, orig##b);        \
    z##c = _mm_add_epi32(z##c, orig##c);        \
    z##d = _mm_add_epi32(z##d, orig##d);        \
    in##a = _mm_cvtsi128_si32(z##a);            \
    in##b = _mm_cvtsi128_si32(z##b);            \
    in##c = _mm_cvtsi128_si32(z##c);            \
    in##d = _mm_cvtsi128_si32(z##d);            \
    in##a ^= *(u32 *) (m + 0);                  \
    in##b ^= *(u32 *) (m + 4);                  \
    in##c ^= *(u32 *) (m + 8);                  \
    in##d ^= *(u32 *) (m + 12);                 \
    *(u32 *) (out + 0) = in##a;                 \
    *(u32 *) (out + 4) = in##b;                 \
    *(u32 *) (out + 8) = in##c;                 \
    *(u32 *) (out + 12) = in##d;                \
    in##a = _mm_extract_epi32(z##a,1);          \
    in##b = _mm_extract_epi32(z##b,1);          \
    in##c = _mm_extract_epi32(z##c,1);          \
    in##d = _mm_extract_epi32(z##d,1);          \
    in##a ^= *(u32 *) (m + 64);                 \
    in##b ^= *(u32 *) (m + 68);                 \
    in##c ^= *(u32 *) (m + 72);                 \
    in##d ^= *(u32 *) (m + 76);                 \
    *(u32 *) (out + 64) = in##a;                \
    *(u32 *) (out + 68) = in##b;                \
    *(u32 *) (out + 72) = in##c;                \
    *(u32 *) (out + 76) = in##d;                \
    in##a = _mm_extract_epi32(z##a,2);          \
    in##b = _mm_extract_epi32(z##b,2);          \
    in##c = _mm_extract_epi32(z##c,2);          \
    in##d = _mm_extract_epi32(z##d,2);          \
    in##a ^= *(u32 *) (m + 128);                \
    in##b ^= *(u32 *) (m + 132);                \
    in##c ^= *(u32 *) (m + 136);                \
    in##d ^= *(u32 *) (m + 140);                \
    *(u32 *) (out + 128) = in##a;               \
    *(u32 *) (out + 132) = in##b;               \
    *(u32 *) (out + 136) = in##c;               \
    *(u32 *) (out + 140) = in##d;               \
    in##a = _mm_extract_epi32(z##a,3);          \
    in##b = _mm_extract_epi32(z##b,3);          \
    in##c = _mm_extract_epi32(z##c,3);          \
    in##d = _mm_extract_epi32(z##d,3);          \
    in##a ^= *(u32 *) (m + 192);                \
    in##b ^= *(u32 *) (m + 196);                \
    in##c ^= *(u32 *) (m + 200);                \
    in##d ^= *(u32 *) (m + 204);                \
    *(u32 *) (out + 192) = in##a;               \
    *(u32 *) (out + 196) = in##b;               \
    *(u32 *) (out + 200) = in##c;               \
    *(u32 *) (out + 204) = in##d

    /* store data ; this macro first transpose data in-registers, and then store them in memory. much faster with icc. */
#define ONEQUAD_TRANSPOSE(a,b,c,d)                                      \
    z##a = _mm_add_epi32(z##a, orig##a);                                \
    z##b = _mm_add_epi32(z##b, orig##b);                                \
    z##c = _mm_add_epi32(z##c, orig##c);                                \
    z##d = _mm_add_epi32(z##d, orig##d);                                \
    y##a = _mm_unpacklo_epi32(z##a, z##b);                              \
    y##b = _mm_unpacklo_epi32(z##c, z##d);                              \
    y##c = _mm_unpackhi_epi32(z##a, z##b);                              \
    y##d = _mm_unpackhi_epi32(z##c, z##d);                              \
    z##a = _mm_unpacklo_epi64(y##a, y##b);                              \
    z##b = _mm_unpackhi_epi64(y##a, y##b);                              \
    z##c = _mm_unpacklo_epi64(y##c, y##d);                              \
    z##d = _mm_unpackhi_epi64(y##c, y##d);                              \
    y##a = _mm_xor_si128(z##a, _mm_loadu_si128((__m128i*)(m+0)));        \
    _mm_storeu_si128((__m128i*)(out+0),y##a);                            \
    y##b = _mm_xor_si128(z##b, _mm_loadu_si128((__m128i*)(m+64)));       \
    _mm_storeu_si128((__m128i*)(out+64),y##b);                           \
    y##c = _mm_xor_si128(z##c, _mm_loadu_si128((__m128i*)(m+128)));      \
    _mm_storeu_si128((__m128i*)(out+128),y##c);                          \
    y##d = _mm_xor_si128(z##d, _mm_loadu_si128((__m128i*)(m+192)));      \
    _mm_storeu_si128((__m128i*)(out+192),y##d)

#define ONEQUAD(a,b,c,d) ONEQUAD_TRANSPOSE(a,b,c,d)

    ONEQUAD(0,1,2,3);
    m+=16;
    out+=16;
    ONEQUAD(4,5,6,7);
    m+=16;
    out+=16;
    ONEQUAD(8,9,10,11);
    m+=16;
    out+=16;
    ONEQUAD(12,13,14,15);
    m-=48;
    out-=48;

#undef ONEQUAD
#undef ONEQUAD_TRANSPOSE
#undef ONEQUAD_EXTRACT
#undef ONEQUAD_SHUFFLE

    bytes -= 256;
    out += 256;
    m += 256;
  }
}
