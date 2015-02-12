/*
u16.h version $Date: 2014/09/24 12:07:11 $
D. J. Bernstein
Romain Dolbeau
Public domain.
*/

if (!bytes) return;
if (bytes >= 1024)
{
  u32 in0;
  u32 in1;
  u32 in2;
  u32 in3;
  u32 in4;
  u32 in5;
  u32 in6;
  u32 in7;
  u32 in8;
  u32 in9;
  u32 in10;
  u32 in11;
  u32 in12;
  u32 in13;
  u32 in14;
  u32 in15;
  __m512i y0;
  __m512i y1;
  __m512i y2;
  __m512i y3;
  __m512i y4;
  __m512i y5;
  __m512i y6;
  __m512i y7;
  __m512i y8;
  __m512i y9;
  __m512i y10;
  __m512i y11;
  __m512i y12;
  __m512i y13;
  __m512i y14;
  __m512i y15;

  __m512i z0  = _mm512_loadu_si512((__m512i*)(x +  0)); /* load the entire block */
  /* permutexvar -> not subtle */
  __m512i z5  = _mm512_permutexvar_epi32(_mm512_set1_epi32(1), z0);
  __m512i z10 = _mm512_permutexvar_epi32(_mm512_set1_epi32(2), z0);
  __m512i z15 = _mm512_permutexvar_epi32(_mm512_set1_epi32(3), z0);
  __m512i z12 = _mm512_permutexvar_epi32(_mm512_set1_epi32(4), z0);
  __m512i z1  = _mm512_permutexvar_epi32(_mm512_set1_epi32(5), z0);
  __m512i z6  = _mm512_permutexvar_epi32(_mm512_set1_epi32(6), z0);
  __m512i z11 = _mm512_permutexvar_epi32(_mm512_set1_epi32(7), z0);
  __m512i z8;//  = _mm512_permutexvar_epi32(_mm512_set1_epi32(8), z0);
  __m512i z13 = _mm512_permutexvar_epi32(_mm512_set1_epi32(9), z0);
  __m512i z2  = _mm512_permutexvar_epi32(_mm512_set1_epi32(10), z0);
  __m512i z7  = _mm512_permutexvar_epi32(_mm512_set1_epi32(11), z0);
  __m512i z4  = _mm512_permutexvar_epi32(_mm512_set1_epi32(12), z0);
  __m512i z9;//  = _mm512_permutexvar_epi32(_mm512_set1_epi32(13), z0);
  __m512i z14 = _mm512_permutexvar_epi32(_mm512_set1_epi32(14), z0);
  __m512i z3  = _mm512_permutexvar_epi32(_mm512_set1_epi32(15), z0);
  z0  = _mm512_permutexvar_epi32(_mm512_setzero_si512(), z0);

  __m512i orig0 = z0;
  __m512i orig1 = z1;
  __m512i orig2 = z2;
  __m512i orig3 = z3;
  __m512i orig4 = z4;
  __m512i orig5 = z5;
  __m512i orig6 = z6;
  __m512i orig7 = z7;
  __m512i orig8;
  __m512i orig9;
  __m512i orig10 = z10;
  __m512i orig11 = z11;
  __m512i orig12 = z12;
  __m512i orig13 = z13;
  __m512i orig14 = z14;
  __m512i orig15 = z15;
  while (bytes >= 1024) {
    const __m512i permute = _mm512_set_epi64(7,5,3,1,6,4,2,0);
    const __m512i addv8 = _mm512_set_epi64(7,6,5,4,3,2,1,0);
    const __m512i addv9 = _mm512_set_epi64(15,14,13,12,11,10,9,8);
    __m512i t8, t9;
    in8 = x[8];
    in9 = x[13]; // see arrays above for the address translation
    u64 in89 = ((u64)in8) | (((u64)in9) << 32);

    z8 = _mm512_broadcastq_epi64(_mm_cvtsi64_si128(in89));
    z9 = _mm512_broadcastq_epi64(_mm_cvtsi64_si128(in89));
    t8 = _mm512_add_epi64(addv8, z8);
    t9 = _mm512_add_epi64(addv9, z9);
    z8 = _mm512_unpacklo_epi32(t8, t9);
    z9 = _mm512_unpackhi_epi32(t8, t9);
    t8 = _mm512_unpacklo_epi32(z8, z9);
    t9 = _mm512_unpackhi_epi32(z8, z9);
    z8 = _mm512_permutexvar_epi64(permute, t8);
    z9 = _mm512_permutexvar_epi64(permute, t9);

    orig8 = z8;
    orig9 = z9;

    in89 += 16;
    
    x[8] = in89 & 0xFFFFFFFF;
    x[13] = (in89>>32)&0xFFFFFFFF;

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
      __m512i r0;
      __m512i r1;
      __m512i r2;
      __m512i r3;
      __m512i r4;
      __m512i r5;
      __m512i r6;
      __m512i r7;
      __m512i r8;
      __m512i r9;
      __m512i r10;
      __m512i r11;
      __m512i r12;
      __m512i r13;
      __m512i r14;
      __m512i r15;
        
      y4 = z12;
      y4 = _mm512_add_epi32(y4, z0);
      r4 = _mm512_rol_epi32(y4, 7);
      z4 = _mm512_xor_si512(z4, r4);

      y9 = z1;
      y9 = _mm512_add_epi32(y9, z5);
      r9 = _mm512_rol_epi32(y9, 7);
      z9 = _mm512_xor_si512(z9, r9);

      y8 = z0;
      y8 = _mm512_add_epi32(y8, z4);
      r8 = _mm512_rol_epi32(y8, 9);
      z8 = _mm512_xor_si512(z8, r8);

      y13 = z5;
      y13 = _mm512_add_epi32(y13, z9);
      r13 = _mm512_rol_epi32(y13, 9);
      z13 = _mm512_xor_si512(z13, r13);

      y12 = z4;
      y12 = _mm512_add_epi32(y12, z8);
      r12 = _mm512_rol_epi32(y12, 13);
      z12 = _mm512_xor_si512(z12, r12);

      y1 = z9;
      y1 = _mm512_add_epi32(y1, z13);
      r1 = _mm512_rol_epi32(y1, 13);
      z1 = _mm512_xor_si512(z1, r1);

      y0 = z8;
      y0 = _mm512_add_epi32(y0, z12);
      r0 = _mm512_rol_epi32(y0, 18);
      z0 = _mm512_xor_si512(z0, r0);


      y5 = z13;
      y5 = _mm512_add_epi32(y5, z1);
      r5 = _mm512_rol_epi32(y5, 18);
      z5 = _mm512_xor_si512(z5, r5);

      y14 = z6;
      y14 = _mm512_add_epi32(y14, z10);
      r14 = _mm512_rol_epi32(y14, 7);
      z14 = _mm512_xor_si512(z14, r14);


      y3 = z11;
      y3 = _mm512_add_epi32(y3, z15);
      r3 = _mm512_rol_epi32(y3, 7);
      z3 = _mm512_xor_si512(z3, r3);

      y2 = z10;
      y2 = _mm512_add_epi32(y2, z14);
      r2 = _mm512_rol_epi32(y2, 9);
      z2 = _mm512_xor_si512(z2, r2);

      y7 = z15;
      y7 = _mm512_add_epi32(y7, z3);
      r7 = _mm512_rol_epi32(y7, 9);
      z7 = _mm512_xor_si512(z7, r7);

      y6 = z14;
      y6 = _mm512_add_epi32(y6, z2);
      r6 = _mm512_rol_epi32(y6, 13);
      z6 = _mm512_xor_si512(z6, r6);

      y11 = z3;
      y11 = _mm512_add_epi32(y11, z7);
      r11 = _mm512_rol_epi32(y11, 13);
      z11 = _mm512_xor_si512(z11, r11);

      y10 = z2;
      y10 = _mm512_add_epi32(y10, z6);
      r10 = _mm512_rol_epi32(y10, 18);
      z10 = _mm512_xor_si512(z10, r10);


      y1 = z3;
      y1 = _mm512_add_epi32(y1, z0);
      r1 = _mm512_rol_epi32(y1, 7);
      z1 = _mm512_xor_si512(z1, r1);

      y15 = z7;
      y15 = _mm512_add_epi32(y15, z11);
      r15 = _mm512_rol_epi32(y15, 18);
      z15 = _mm512_xor_si512(z15, r15);


      y6 = z4;
      y6 = _mm512_add_epi32(y6, z5);
      r6 = _mm512_rol_epi32(y6, 7);
      z6 = _mm512_xor_si512(z6, r6);

      y2 = z0;
      y2 = _mm512_add_epi32(y2, z1);
      r2 = _mm512_rol_epi32(y2, 9);
      z2 = _mm512_xor_si512(z2, r2);

      y7 = z5;
      y7 = _mm512_add_epi32(y7, z6);
      r7 = _mm512_rol_epi32(y7, 9);
      z7 = _mm512_xor_si512(z7, r7);

      y3 = z1;
      y3 = _mm512_add_epi32(y3, z2);
      r3 = _mm512_rol_epi32(y3, 13);
      z3 = _mm512_xor_si512(z3, r3);

      y4 = z6;
      y4 = _mm512_add_epi32(y4, z7);
      r4 = _mm512_rol_epi32(y4, 13);
      z4 = _mm512_xor_si512(z4, r4);

      y0 = z2;
      y0 = _mm512_add_epi32(y0, z3);
      r0 = _mm512_rol_epi32(y0, 18);
      z0 = _mm512_xor_si512(z0, r0);


      y5 = z7;
      y5 = _mm512_add_epi32(y5, z4);
      r5 = _mm512_rol_epi32(y5, 18);
      z5 = _mm512_xor_si512(z5, r5);

      y11 = z9;
      y11 = _mm512_add_epi32(y11, z10);
      r11 = _mm512_rol_epi32(y11, 7);
      z11 = _mm512_xor_si512(z11, r11);



      y12 = z14;
      y12 = _mm512_add_epi32(y12, z15);
      r12 = _mm512_rol_epi32(y12, 7);
      z12 = _mm512_xor_si512(z12, r12);

      y8 = z10;
      y8 = _mm512_add_epi32(y8, z11);
      r8 = _mm512_rol_epi32(y8, 9);
      z8 = _mm512_xor_si512(z8, r8);

      y13 = z15;
      y13 = _mm512_add_epi32(y13, z12);
      r13 = _mm512_rol_epi32(y13, 9);
      z13 = _mm512_xor_si512(z13, r13);

      y9 = z11;
      y9 = _mm512_add_epi32(y9, z8);
      r9 = _mm512_rol_epi32(y9, 13);
      z9 = _mm512_xor_si512(z9, r9);

      y14 = z12;
      y14 = _mm512_add_epi32(y14, z13);
      r14 = _mm512_rol_epi32(y14, 13);
      z14 = _mm512_xor_si512(z14, r14);

      y10 = z8;
      y10 = _mm512_add_epi32(y10, z9);
      r10 = _mm512_rol_epi32(y10, 18);
      z10 = _mm512_xor_si512(z10, r10);

      y15 = z13;
      y15 = _mm512_add_epi32(y15, z14);
      r15 = _mm512_rol_epi32(y15, 18);
      z15 = _mm512_xor_si512(z15, r15);

    }

    /* store data ; this macro first transpose data in-registers, and then store them in memory. */
#define ONEQUAD_TRANSPOSE(a,b,c,d)                                      \
    {                                                                   \
      __m128i t0, t1, t2, t3;                                           \
      z##a = _mm512_add_epi32(z##a, orig##a);                           \
      z##b = _mm512_add_epi32(z##b, orig##b);                           \
      z##c = _mm512_add_epi32(z##c, orig##c);                           \
      z##d = _mm512_add_epi32(z##d, orig##d);                           \
      y##a = _mm512_unpacklo_epi32(z##a, z##b);                         \
      y##b = _mm512_unpacklo_epi32(z##c, z##d);                         \
      y##c = _mm512_unpackhi_epi32(z##a, z##b);                         \
      y##d = _mm512_unpackhi_epi32(z##c, z##d);                         \
      z##a = _mm512_unpacklo_epi64(y##a, y##b);                         \
      z##b = _mm512_unpackhi_epi64(y##a, y##b);                         \
      z##c = _mm512_unpacklo_epi64(y##c, y##d);                         \
      z##d = _mm512_unpackhi_epi64(y##c, y##d);                         \
      t0 = _mm_xor_si128(_mm512_extracti32x4_epi32(z##a,0), _mm_loadu_si128((__m128i*)(m+0))); \
      _mm_storeu_si128((__m128i*)(out+0),t0);                            \
      t1 = _mm_xor_si128(_mm512_extracti32x4_epi32(z##b,0), _mm_loadu_si128((__m128i*)(m+64))); \
      _mm_storeu_si128((__m128i*)(out+64),t1);                           \
      t2 = _mm_xor_si128(_mm512_extracti32x4_epi32(z##c,0), _mm_loadu_si128((__m128i*)(m+128))); \
      _mm_storeu_si128((__m128i*)(out+128),t2);                          \
      t3 = _mm_xor_si128(_mm512_extracti32x4_epi32(z##d,0), _mm_loadu_si128((__m128i*)(m+192))); \
      _mm_storeu_si128((__m128i*)(out+192),t3);                          \
      t0 = _mm_xor_si128(_mm512_extracti32x4_epi32(z##a,1), _mm_loadu_si128((__m128i*)(m+256))); \
      _mm_storeu_si128((__m128i*)(out+256),t0);                          \
      t1 = _mm_xor_si128(_mm512_extracti32x4_epi32(z##b,1), _mm_loadu_si128((__m128i*)(m+320))); \
      _mm_storeu_si128((__m128i*)(out+320),t1);                          \
      t2 = _mm_xor_si128(_mm512_extracti32x4_epi32(z##c,1), _mm_loadu_si128((__m128i*)(m+384))); \
      _mm_storeu_si128((__m128i*)(out+384),t2);                          \
      t3 = _mm_xor_si128(_mm512_extracti32x4_epi32(z##d,1), _mm_loadu_si128((__m128i*)(m+448))); \
      _mm_storeu_si128((__m128i*)(out+448),t3);                          \
      t0 = _mm_xor_si128(_mm512_extracti32x4_epi32(z##a,2), _mm_loadu_si128((__m128i*)(m+512))); \
      _mm_storeu_si128((__m128i*)(out+512),t0);                            \
      t1 = _mm_xor_si128(_mm512_extracti32x4_epi32(z##b,2), _mm_loadu_si128((__m128i*)(m+576))); \
      _mm_storeu_si128((__m128i*)(out+576),t1);                           \
      t2 = _mm_xor_si128(_mm512_extracti32x4_epi32(z##c,2), _mm_loadu_si128((__m128i*)(m+640))); \
      _mm_storeu_si128((__m128i*)(out+640),t2);                          \
      t3 = _mm_xor_si128(_mm512_extracti32x4_epi32(z##d,2), _mm_loadu_si128((__m128i*)(m+704))); \
      _mm_storeu_si128((__m128i*)(out+704),t3);                          \
      t0 = _mm_xor_si128(_mm512_extracti32x4_epi32(z##a,3), _mm_loadu_si128((__m128i*)(m+768))); \
      _mm_storeu_si128((__m128i*)(out+768),t0);                          \
      t1 = _mm_xor_si128(_mm512_extracti32x4_epi32(z##b,3), _mm_loadu_si128((__m128i*)(m+832))); \
      _mm_storeu_si128((__m128i*)(out+832),t1);                          \
      t2 = _mm_xor_si128(_mm512_extracti32x4_epi32(z##c,3), _mm_loadu_si128((__m128i*)(m+896))); \
      _mm_storeu_si128((__m128i*)(out+896),t2);                          \
      t3 = _mm_xor_si128(_mm512_extracti32x4_epi32(z##d,3), _mm_loadu_si128((__m128i*)(m+960))); \
      _mm_storeu_si128((__m128i*)(out+960),t3);                          \
    }
    
    __m512i gvv = _mm512_set_epi64(776,768,520,512,264,256,8,0);
    /* same principle, but doesn't break 512-bits block in 128-bits chunks.
       Uses gather & scatter operation instead */
#define ONEQUAD_TRANSPOSE_SCATTERGATHER(a,b,c,d)                        \
    {                                                                   \
      __m512i t0, t1, t2, t3;                                           \
      z##a = _mm512_add_epi32(z##a, orig##a);                           \
      z##b = _mm512_add_epi32(z##b, orig##b);                           \
      z##c = _mm512_add_epi32(z##c, orig##c);                           \
      z##d = _mm512_add_epi32(z##d, orig##d);                           \
      y##a = _mm512_unpacklo_epi32(z##a, z##b);                         \
      y##b = _mm512_unpacklo_epi32(z##c, z##d);                         \
      y##c = _mm512_unpackhi_epi32(z##a, z##b);                         \
      y##d = _mm512_unpackhi_epi32(z##c, z##d);                         \
      z##a = _mm512_unpacklo_epi64(y##a, y##b);                         \
      z##b = _mm512_unpackhi_epi64(y##a, y##b);                         \
      z##c = _mm512_unpacklo_epi64(y##c, y##d);                         \
      z##d = _mm512_unpackhi_epi64(y##c, y##d);                         \
      t0 = _mm512_xor_si512(z##a, _mm512_i64gather_epi64(gvv, m+  0, 1)); \
      t1 = _mm512_xor_si512(z##b, _mm512_i64gather_epi64(gvv, m+ 64, 1)); \
      t2 = _mm512_xor_si512(z##c, _mm512_i64gather_epi64(gvv, m+128, 1)); \
      t3 = _mm512_xor_si512(z##d, _mm512_i64gather_epi64(gvv, m+192, 1)); \
      _mm512_i64scatter_epi64(out+  0, gvv, t0, 1);                     \
      _mm512_i64scatter_epi64(out+ 64, gvv, t1, 1);                     \
      _mm512_i64scatter_epi64(out+128, gvv, t2, 1);                     \
      _mm512_i64scatter_epi64(out+192, gvv, t3, 1);                     \
    }
#define ONEQUAD(a,b,c,d) ONEQUAD_TRANSPOSE_SCATTERGATHER(a,b,c,d)

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
#undef ONEQUAD_TRANSPOSE_SCATTERGATHER

    bytes -= 1024;
    out += 1024;
    m += 1024;
  }
}
