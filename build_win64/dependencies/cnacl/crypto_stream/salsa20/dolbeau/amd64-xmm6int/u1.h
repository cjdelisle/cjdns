/*
u1.h version $Date: 2014/09/24 12:07:11 $
D. J. Bernstein
Romain Dolbeau
Public domain.
*/

if (!bytes) return;
{
  while (bytes >= 64) {
    __m128i diag0 = _mm_loadu_si128((__m128i*)(x +  0));
    __m128i diag1 = _mm_loadu_si128((__m128i*)(x +  4));
    __m128i diag2 = _mm_loadu_si128((__m128i*)(x +  8));
    __m128i diag3 = _mm_loadu_si128((__m128i*)(x + 12));
    __m128i a0;
    __m128i a1;
    __m128i a2;
    __m128i a3;
    __m128i a4;
    __m128i a5;
    __m128i a6;
    __m128i a7;
    __m128i b0;
    __m128i b1;
    __m128i b2;
    __m128i b3;
    __m128i b4;
    __m128i b5;
    __m128i b6;
    __m128i b7;

    a0 = diag1;
    for (i = 0 ; i < 20 ; i+=4) {
      a0 = _mm_add_epi32(a0, diag0);
      a1 = diag0;
      b0 = a0;
      a0 = _mm_slli_epi32(a0, 7);
      b0 = _mm_srli_epi32(b0, 25);
      diag3 = _mm_xor_si128(diag3, a0);

      diag3 = _mm_xor_si128(diag3, b0);

      a1 = _mm_add_epi32(a1, diag3);
      a2 = diag3;
      b1 = a1;
      a1 = _mm_slli_epi32(a1, 9);
      b1 = _mm_srli_epi32(b1, 23);
      diag2 = _mm_xor_si128(diag2, a1);
      diag3 = _mm_shuffle_epi32(diag3, 0x93);
      diag2 = _mm_xor_si128(diag2, b1);

      a2 = _mm_add_epi32(a2, diag2);
      a3 = diag2;
      b2 = a2;
      a2 = _mm_slli_epi32(a2, 13);
      b2 = _mm_srli_epi32(b2, 19);
      diag1 = _mm_xor_si128(diag1, a2);
      diag2 = _mm_shuffle_epi32(diag2, 0x4e);
      diag1 = _mm_xor_si128(diag1, b2);

      a3 = _mm_add_epi32(a3, diag1);
      a4 = diag3;
      b3 = a3;
      a3 = _mm_slli_epi32(a3, 18);
      b3 = _mm_srli_epi32(b3, 14);
      diag0 = _mm_xor_si128(diag0, a3);
      diag1 = _mm_shuffle_epi32(diag1, 0x39);
      diag0 = _mm_xor_si128(diag0, b3);

      a4 = _mm_add_epi32(a4, diag0);
      a5 = diag0;
      b4 = a4;
      a4 = _mm_slli_epi32(a4, 7);
      b4 = _mm_srli_epi32(b4, 25);
      diag1 = _mm_xor_si128(diag1, a4);

      diag1 = _mm_xor_si128(diag1, b4);

      a5 = _mm_add_epi32(a5, diag1);
      a6 = diag1;
      b5 = a5;
      a5 = _mm_slli_epi32(a5, 9);
      b5 = _mm_srli_epi32(b5, 23);
      diag2 = _mm_xor_si128(diag2, a5);
      diag1 = _mm_shuffle_epi32(diag1, 0x93);
      diag2 = _mm_xor_si128(diag2, b5);

      a6 = _mm_add_epi32(a6, diag2);
      a7 = diag2;
      b6 = a6;
      a6 = _mm_slli_epi32(a6, 13);
      b6 = _mm_srli_epi32(b6, 19);
      diag3 = _mm_xor_si128(diag3, a6);
      diag2 = _mm_shuffle_epi32(diag2, 0x4e);
      diag3 = _mm_xor_si128(diag3, b6);

      a7 = _mm_add_epi32(a7, diag3);
      a0 = diag1;
      b7 = a7;
      a7 = _mm_slli_epi32(a7, 18);
      b7 = _mm_srli_epi32(b7, 14);
      diag0 = _mm_xor_si128(diag0, a7);
      diag3 = _mm_shuffle_epi32(diag3, 0x39);
      diag0 = _mm_xor_si128(diag0, b7);


      a0 = _mm_add_epi32(a0, diag0);
      a1 = diag0;
      b0 = a0;
      a0 = _mm_slli_epi32(a0, 7);
      b0 = _mm_srli_epi32(b0, 25);
      diag3 = _mm_xor_si128(diag3, a0);

      diag3 = _mm_xor_si128(diag3, b0);

      a1 = _mm_add_epi32(a1, diag3);
      a2 = diag3;
      b1 = a1;
      a1 = _mm_slli_epi32(a1, 9);
      b1 = _mm_srli_epi32(b1, 23);
      diag2 = _mm_xor_si128(diag2, a1);
      diag3 = _mm_shuffle_epi32(diag3, 0x93);
      diag2 = _mm_xor_si128(diag2, b1);

      a2 = _mm_add_epi32(a2, diag2);
      a3 = diag2;
      b2 = a2;
      a2 = _mm_slli_epi32(a2, 13);
      b2 = _mm_srli_epi32(b2, 19);
      diag1 = _mm_xor_si128(diag1, a2);
      diag2 = _mm_shuffle_epi32(diag2, 0x4e);
      diag1 = _mm_xor_si128(diag1, b2);

      a3 = _mm_add_epi32(a3, diag1);
      a4 = diag3;
      b3 = a3;
      a3 = _mm_slli_epi32(a3, 18);
      b3 = _mm_srli_epi32(b3, 14);
      diag0 = _mm_xor_si128(diag0, a3);
      diag1 = _mm_shuffle_epi32(diag1, 0x39);
      diag0 = _mm_xor_si128(diag0, b3);

      a4 = _mm_add_epi32(a4, diag0);
      a5 = diag0;
      b4 = a4;
      a4 = _mm_slli_epi32(a4, 7);
      b4 = _mm_srli_epi32(b4, 25);
      diag1 = _mm_xor_si128(diag1, a4);

      diag1 = _mm_xor_si128(diag1, b4);

      a5 = _mm_add_epi32(a5, diag1);
      a6 = diag1;
      b5 = a5;
      a5 = _mm_slli_epi32(a5, 9);
      b5 = _mm_srli_epi32(b5, 23);
      diag2 = _mm_xor_si128(diag2, a5);
      diag1 = _mm_shuffle_epi32(diag1, 0x93);
      diag2 = _mm_xor_si128(diag2, b5);

      a6 = _mm_add_epi32(a6, diag2);
      a7 = diag2;
      b6 = a6;
      a6 = _mm_slli_epi32(a6, 13);
      b6 = _mm_srli_epi32(b6, 19);
      diag3 = _mm_xor_si128(diag3, a6);
      diag2 = _mm_shuffle_epi32(diag2, 0x4e);
      diag3 = _mm_xor_si128(diag3, b6);

      a7 = _mm_add_epi32(a7, diag3);
      a0 = diag1;
      b7 = a7;
      a7 = _mm_slli_epi32(a7, 18);
      b7 = _mm_srli_epi32(b7, 14);
      diag0 = _mm_xor_si128(diag0, a7);
      diag3 = _mm_shuffle_epi32(diag3, 0x39);
      diag0 = _mm_xor_si128(diag0, b7);
    }

    diag0 = _mm_add_epi32(diag0, _mm_loadu_si128((__m128i*)(x +  0)));
    diag1 = _mm_add_epi32(diag1, _mm_loadu_si128((__m128i*)(x +  4)));
    diag2 = _mm_add_epi32(diag2, _mm_loadu_si128((__m128i*)(x +  8)));
    diag3 = _mm_add_epi32(diag3, _mm_loadu_si128((__m128i*)(x + 12)));
    
#define ONEQUAD_SHUFFLE(a,b,c,d)                    \
    u32 in##a =_mm_cvtsi128_si32(diag0);            \
    u32 in##b =_mm_cvtsi128_si32(diag1);            \
    u32 in##c =_mm_cvtsi128_si32(diag2);            \
    u32 in##d =_mm_cvtsi128_si32(diag3);            \
    diag0 = _mm_shuffle_epi32(diag0, 0x39);         \
    diag1 = _mm_shuffle_epi32(diag1, 0x39);         \
    diag2 = _mm_shuffle_epi32(diag2, 0x39);         \
    diag3 = _mm_shuffle_epi32(diag3, 0x39);         \
    in##a ^= *(u32 *) (m + (a*4));                  \
    in##b ^= *(u32 *) (m + (b*4));                  \
    in##c ^= *(u32 *) (m + (c*4));                  \
    in##d ^= *(u32 *) (m + (d*4));                  \
    *(u32 *) (out + (a*4)) = in##a;                 \
    *(u32 *) (out + (b*4)) = in##b;                 \
    *(u32 *) (out + (c*4)) = in##c;                 \
    *(u32 *) (out + (d*4)) = in##d

#define ONEQUAD(a,b,c,d) ONEQUAD_SHUFFLE(a,b,c,d)    

    ONEQUAD(0,12,8,4);
    ONEQUAD(5,1,13,9);
    ONEQUAD(10,6,2,14);
    ONEQUAD(15,11,7,3);

#undef ONEQUAD
#undef ONEQUAD_SHUFFLE

#if 0
    u32 in0 =_mm_cvtsi128_si32(diag0);
    u32 in12 =_mm_cvtsi128_si32(diag1);
    u32 in8 =_mm_cvtsi128_si32(diag2);
    u32 in4 =_mm_cvtsi128_si32(diag3);
    diag0 = _mm_shuffle_epi32(diag0, 0x39);
    diag1 = _mm_shuffle_epi32(diag1, 0x39);
    diag2 = _mm_shuffle_epi32(diag2, 0x39);
    diag3 = _mm_shuffle_epi32(diag3, 0x39);
    in0 ^= *(u32 *) (m + 0);
    in12 ^= *(u32 *) (m + 48);
    in8 ^= *(u32 *) (m + 32);
    in4 ^= *(u32 *) (m + 16);
    *(u32 *) (out + 0) = in0;
    *(u32 *) (out + 48) = in12;
    *(u32 *) (out + 32) = in8;
    *(u32 *) (out + 16) = in4;

    u32 in5 =_mm_cvtsi128_si32(diag0);
    u32 in1 =_mm_cvtsi128_si32(diag1);
    u32 in13 =_mm_cvtsi128_si32(diag2);
    u32 in9 =_mm_cvtsi128_si32(diag3);
    diag0 = _mm_shuffle_epi32(diag0, 0x39);
    diag1 = _mm_shuffle_epi32(diag1, 0x39);
    diag2 = _mm_shuffle_epi32(diag2, 0x39);
    diag3 = _mm_shuffle_epi32(diag3, 0x39);
    in5 ^= *(u32 *) (m + 20);
    in1 ^= *(u32 *) (m + 4);
    in13 ^= *(u32 *) (m + 52);
    in9 ^= *(u32 *) (m + 36);
    *(u32 *) (out + 20) = in5;
    *(u32 *) (out + 4) = in1;
    *(u32 *) (out + 52) = in13;
    *(u32 *) (out + 36) = in9;

    u32 in10 =_mm_cvtsi128_si32(diag0);
    u32 in6 =_mm_cvtsi128_si32(diag1);
    u32 in2 =_mm_cvtsi128_si32(diag2);
    u32 in14 =_mm_cvtsi128_si32(diag3);
    diag0 = _mm_shuffle_epi32(diag0, 0x39);
    diag1 = _mm_shuffle_epi32(diag1, 0x39);
    diag2 = _mm_shuffle_epi32(diag2, 0x39);
    diag3 = _mm_shuffle_epi32(diag3, 0x39);
    in10 ^= *(u32 *) (m + 40);
    in6 ^= *(u32 *) (m + 24);
    in2 ^= *(u32 *) (m + 8);
    in14 ^= *(u32 *) (m + 56);
    *(u32 *) (out + 40) = in10;
    *(u32 *) (out + 24) = in6;
    *(u32 *) (out + 8) = in2;
    *(u32 *) (out + 56) = in14;

    u32 in15 =_mm_cvtsi128_si32(diag0);
    u32 in11 =_mm_cvtsi128_si32(diag1);
    u32 in7 =_mm_cvtsi128_si32(diag2);
    u32 in3 =_mm_cvtsi128_si32(diag3);
    in15 ^= *(u32 *) (m + 60);
    in11 ^= *(u32 *) (m + 44);
    in7 ^= *(u32 *) (m + 28);
    in3 ^= *(u32 *) (m + 12);
    *(u32 *) (out + 60) = in15;
    *(u32 *) (out + 44) = in11;
    *(u32 *) (out + 28) = in7;
    *(u32 *) (out + 12) = in3;
#endif

    in8 = x[8];
    in9 = x[13];
    in8 ++;
    if (in8 == 0)
      in9 ++;
    x[8] = in8;
    x[13] = in9;

    out += 64;
    m += 64;
    bytes -=64;
  }
}
