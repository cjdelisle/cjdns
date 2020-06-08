constant sigma:
  const32 1634760805
  const32 857760878
  const32 2036477234
  const32 1797285236

int128 abab
int128 diag0
int128 diag1
int128 diag2
int128 diag3
int128 a0
int128 a1
int128 a2
int128 a3
int128 b0
int128 b1
int128 b2
int128 b3
int128 next_diag0
int128 next_diag1
int128 next_diag2
int128 next_diag3
int128 next_a0
int128 next_a1
int128 next_a2
int128 next_a3
int128 next_b0
int128 next_b1
int128 next_b2
int128 next_b3
int128 x0x5x10x15
int128 x12x1x6x11
int128 x8x13x2x7
int128 x4x9x14x3
int128 x0x1x10x11
int128 x12x13x6x7
int128 x8x9x2x3
int128 x4x5x14x15
int128 x0x1x2x3
int128 x4x5x6x7
int128 x8x9x10x11
int128 x12x13x14x15
int128 m0m1m2m3
int128 m4m5m6m7
int128 m8m9m10m11
int128 m12m13m14m15
int128 start0
int128 start1
int128 start2
int128 start3
stack128 stack_start3
stack128 next_start2
stack128 next_start3
int128 k0k1k2k3
int128 k4k5k6k7
int128 k1n1k7k2
int128 n2n3n3n2
int128 k2k3k6k7
int128 nextblock

stack128 stack_q4
stack128 stack_q5
stack128 stack_q6
stack128 stack_q7
stack32 stack_r4

stack128 k2k3k6k7_stack
stack128 k1n1k7k2_stack

stack512 tmp
stack32 savec
int32 i
int32 ci
int32 mi

enter crypto_stream_salsa20_armneon3_xor

new stack_q4
new stack_q5
new stack_q6
new stack_q7
stack_q4 bot = caller_q4 bot
stack_q4 top = caller_q4 top
stack_q5 bot = caller_q5 bot
stack_q5 top = caller_q5 top
stack_q6 bot = caller_q6 bot
stack_q6 top = caller_q6 top
stack_q7 bot = caller_q7 bot
stack_q7 top = caller_q7 top

new stack_r4
stack_r4 = caller_r4

int32 c
c = input_0

int32 m
m = input_1

int32 mlenlow
mlenlow = input_2

int32 mlenhigh
mlenhigh = input_3

int32 n
n = input_4

int32 k
k = input_5

k0k1k2k3 = mem128[k]
k += 16
k4k5k6k7 = mem128[k]

i = sigma
start0 = mem128[i]

2x start1 = 0
start1 bot = mem64[n]                            # n0 n1 0 0
start1 = start1[1] start1[0] start1[2,3]         # n1 n0 0 0
start1 = start1[0,1] start1[1] k4k5k6k7[0]       # n1 n0 n0 k4

new k1n1k7k2
k1n1k7k2 = k0k1k2k3[1] start1[0] k1n1k7k2[2,3]   # k1 n1 X X
k1n1k7k2 = k1n1k7k2[0,1] k4k5k6k7[3] k0k1k2k3[2] # k1 n1 k7 k2

k2k3k6k7 = k4k5k6k7
k2k3k6k7 = k0k1k2k3[2,3] k2k3k6k7[2,3]

start1 = k4k5k6k7[1] k0k1k2k3[0] start1[2,3]     # k5 k0 n0 k4

new k2k3k6k7_stack
k2k3k6k7_stack bot = k2k3k6k7 bot
k2k3k6k7_stack top = k2k3k6k7 top

new k1n1k7k2_stack
k1n1k7k2_stack bot = k1n1k7k2 bot
k1n1k7k2_stack top = k1n1k7k2 top

2x n2n3n3n2 = 0


                        unsigned<? mlenlow - 128
goto mlenlowbelow128 if unsigned<

mlenatleast128:

  new k2k3k6k7
  k2k3k6k7 bot = k2k3k6k7_stack bot
  k2k3k6k7 top = k2k3k6k7_stack top

  new k1n1k7k2
  k1n1k7k2 bot = k1n1k7k2_stack bot
  k1n1k7k2 top = k1n1k7k2_stack top

  n2n3n3n2 = n2n3n3n2[0,1] n2n3n3n2[1] n2n3n3n2[0]

  new diag2
  diag2 = diag2[0,1] k1n1k7k2[0,1]             # X X k1 n1
  diag2 = n2n3n3n2[3] k2k3k6k7[2] diag2[2,3]   # n2 k6 k1 n1

  new diag3
  diag3 = diag3[0,1] k1n1k7k2[2,3]             # X X k7 k2
  diag3 = k2k3k6k7[1] n2n3n3n2[2] diag3[2,3]   # k3 n3 k7 k2

  diag0 = start0
  diag1 = start1
  start2 = diag2
  new stack_start3
  stack_start3 bot = diag3 bot
  stack_start3 top = diag3 top

  2x nextblock = 0xff
  4x nextblock unsigned>>= 7

  2x n2n3n3n2 += nextblock
  n2n3n3n2 = n2n3n3n2[0,1] n2n3n3n2[1] n2n3n3n2[0]

  new next_diag2
  next_diag2 = next_diag2[0,1] k1n1k7k2[0,1]
  next_diag2 = n2n3n3n2[3] k2k3k6k7[2] next_diag2[2,3]

  new next_diag3
  next_diag3 = next_diag3[0,1] k1n1k7k2[2,3]
  next_diag3 = k2k3k6k7[1] n2n3n3n2[2] next_diag3[2,3]

  2x n2n3n3n2 += nextblock

  next_diag0 = diag0
  next_diag1 = diag1
  next_start2 bot = next_diag2 bot
  next_start2 top = next_diag2 top
  next_start3 bot = next_diag3 bot
  next_start3 top = next_diag3 top

  i = 20

  mainloop2:

    4x a0 = diag1 + diag0
   					4x next_a0 = next_diag1 + next_diag0
    4x b0 = a0 << 7
    					4x next_b0 = next_a0 << 7
    4x b0 insert= a0 >> 25
    					4x next_b0 insert= next_a0 >> 25
       diag3 ^= b0
       					next_diag3 ^= next_b0
    4x a1 = diag0 + diag3
    					4x next_a1 = next_diag0 + next_diag3
    4x b1 = a1 << 9
    					4x next_b1 = next_a1 << 9
    4x b1 insert= a1 >> 23
    					4x next_b1 insert= next_a1 >> 23
       diag2 ^= b1
       					next_diag2 ^= next_b1
    4x a2 = diag3 + diag2
            diag3 = diag3[3] diag3[0,1,2]
    					4x next_a2 = next_diag3 + next_diag2
    4x b2 = a2 << 13
            					next_diag3 = next_diag3[3] next_diag3[0,1,2]
    					4x next_b2 = next_a2 << 13
    4x b2 insert= a2 >> 19
    					4x next_b2 insert= next_a2 >> 19
       diag1 ^= b2
       					next_diag1 ^= next_b2
    4x a3 = diag2 + diag1
            diag2 = diag2[2,3] diag2[0,1]
    					4x next_a3 = next_diag2 + next_diag1
    4x b3 = a3 << 18
            					next_diag2 = next_diag2[2,3] next_diag2[0,1]
    					4x next_b3 = next_a3 << 18
    4x b3 insert= a3 >> 14
            diag1 = diag1[1,2,3] diag1[0]
    					4x next_b3 insert= next_a3 >> 14
       diag0 ^= b3
            					next_diag1 = next_diag1[1,2,3] next_diag1[0]
       					next_diag0 ^= next_b3

    4x a0 = diag3 + diag0
    					4x next_a0 = next_diag3 + next_diag0
    4x b0 = a0 << 7
    					4x next_b0 = next_a0 << 7
    4x b0 insert= a0 >> 25
    					4x next_b0 insert= next_a0 >> 25
       diag1 ^= b0
       					next_diag1 ^= next_b0
    4x a1 = diag0 + diag1
    					4x next_a1 = next_diag0 + next_diag1
    4x b1 = a1 << 9
    					4x next_b1 = next_a1 << 9
    4x b1 insert= a1 >> 23
                  						unsigned>? i -= 2
    					4x next_b1 insert= next_a1 >> 23
       diag2 ^= b1
       					next_diag2 ^= next_b1
    4x a2 = diag1 + diag2
            diag1 = diag1[3] diag1[0,1,2]
    					4x next_a2 = next_diag1 + next_diag2
    4x b2 = a2 << 13
            					next_diag1 = next_diag1[3] next_diag1[0,1,2]
    					4x next_b2 = next_a2 << 13
    4x b2 insert= a2 >> 19
    					4x next_b2 insert= next_a2 >> 19
       diag3 ^= b2
       					next_diag3 ^= next_b2
    4x a3 = diag2 + diag3
            diag2 = diag2[2,3] diag2[0,1]
    					4x next_a3 = next_diag2 + next_diag3
    4x b3 = a3 << 18
            					next_diag2 = next_diag2[2,3] next_diag2[0,1]
    					4x next_b3 = next_a3 << 18
    4x b3 insert= a3 >> 14
            diag3 = diag3[1,2,3] diag3[0]
    					4x next_b3 insert= next_a3 >> 14
       diag0 ^= b3
            					next_diag3 = next_diag3[1,2,3] next_diag3[0]
       					next_diag0 ^= next_b3

  goto mainloop2 if unsigned>

  2x abab = 0xffffffff
  
  new x4x9x14x3
  x4x9x14x3 bot = stack_start3 bot
  x4x9x14x3 top = stack_start3 top
  4x x0x5x10x15 = diag0 + start0
  4x x12x1x6x11 = diag1 + start1
  4x x8x13x2x7 = diag2 + start2
  4x x4x9x14x3 += diag3
  
  x0x1x10x11 = x0x5x10x15
  x12x13x6x7 = x12x1x6x11
  x8x9x2x3 = x8x13x2x7
  x4x5x14x15 = x4x9x14x3
  
  x0x1x10x11 = (abab & x0x1x10x11) | (~abab & x12x1x6x11)
  x12x13x6x7 = (abab & x12x13x6x7) | (~abab & x8x13x2x7)
  x8x9x2x3 = (abab & x8x9x2x3) | (~abab & x4x9x14x3)
  x4x5x14x15 = (abab & x4x5x14x15) | (~abab & x0x5x10x15)
  
  x0x1x2x3 = x0x1x10x11
  x4x5x6x7 = x4x5x14x15
  x8x9x10x11 = x8x9x2x3
  x12x13x14x15 = x12x13x6x7
  
  x0x1x2x3 = x0x1x2x3[0,1] x8x9x2x3[2,3]
  x4x5x6x7 = x4x5x6x7[0,1] x12x13x6x7[2,3]
  x8x9x10x11 = x8x9x10x11[0,1] x0x1x10x11[2,3]
  x12x13x14x15 = x12x13x14x15[0,1] x4x5x14x15[2,3]

                     =? m - 0
  goto nomessage2 if =
    m0m1m2m3 = mem128[m]
    m += 16
    m4m5m6m7 = mem128[m]
    m += 16
    m8m9m10m11 = mem128[m]
    m += 16
    m12m13m14m15 = mem128[m]
    m += 16
    x0x1x2x3 ^= m0m1m2m3
    x4x5x6x7 ^= m4m5m6m7
    x8x9x10x11 ^= m8m9m10m11
    x12x13x14x15 ^= m12m13m14m15
  nomessage2:

  mem128[c] = x0x1x2x3
  c += 16
  mem128[c] = x4x5x6x7
  c += 16
  mem128[c] = x8x9x10x11
  c += 16
  mem128[c] = x12x13x14x15
  c += 16

  new x8x13x2x7
  x8x13x2x7 bot = next_start2 bot
  x8x13x2x7 top = next_start2 top
  new x4x9x14x3
  x4x9x14x3 bot = next_start3 bot
  x4x9x14x3 top = next_start3 top
  4x x0x5x10x15 = next_diag0 + start0
  4x x12x1x6x11 = next_diag1 + start1
  4x x8x13x2x7 += next_diag2
  4x x4x9x14x3 += next_diag3
  
  x0x1x10x11 = x0x5x10x15
  x12x13x6x7 = x12x1x6x11
  x8x9x2x3 = x8x13x2x7
  x4x5x14x15 = x4x9x14x3
  
  x0x1x10x11 = (abab & x0x1x10x11) | (~abab & x12x1x6x11)
  x12x13x6x7 = (abab & x12x13x6x7) | (~abab & x8x13x2x7)
  x8x9x2x3 = (abab & x8x9x2x3) | (~abab & x4x9x14x3)
  x4x5x14x15 = (abab & x4x5x14x15) | (~abab & x0x5x10x15)
  
  x0x1x2x3 = x0x1x10x11
  x4x5x6x7 = x4x5x14x15
  x8x9x10x11 = x8x9x2x3
  x12x13x14x15 = x12x13x6x7
  
  x0x1x2x3 = x0x1x2x3[0,1] x8x9x2x3[2,3]
  x4x5x6x7 = x4x5x6x7[0,1] x12x13x6x7[2,3]
  x8x9x10x11 = x8x9x10x11[0,1] x0x1x10x11[2,3]
  x12x13x14x15 = x12x13x14x15[0,1] x4x5x14x15[2,3]

                     =? m - 0
  goto nomessage2next if =
    m0m1m2m3 = mem128[m]
    m += 16
    m4m5m6m7 = mem128[m]
    m += 16
    m8m9m10m11 = mem128[m]
    m += 16
    m12m13m14m15 = mem128[m]
    m += 16
    x0x1x2x3 ^= m0m1m2m3
    x4x5x6x7 ^= m4m5m6m7
    x8x9x10x11 ^= m8m9m10m11
    x12x13x14x15 ^= m12m13m14m15
  nomessage2next:

  mem128[c] = x0x1x2x3
  c += 16
  mem128[c] = x4x5x6x7
  c += 16
  mem128[c] = x8x9x10x11
  c += 16
  mem128[c] = x12x13x14x15
  c += 16

  carry? mlenlow -= 128
  mlenhigh -= 0 - carry

                        unsigned<? mlenlow - 128
goto mlenatleast128 if !unsigned<

mlenlowbelow128:

                       unsigned>? mlenhigh - 0
goto mlenatleast128 if unsigned>

             =? mlenlow - 0
goto done if =

mlenatleast1:

                         unsigned<? mlenlow - 64
  goto mlenatleast64 if !unsigned<
    savec = c
    c = &tmp
                          =? m - 0
    goto mlenatleast64 if =
    i = 0
    mcopy:
      mi = mem8[m + 0]
      mem8[c + 0] = mi
      m += 1
      c += 1
      i += 1
                  unsigned<? i - mlenlow
    goto mcopy if unsigned<
    mi = 0
    pad:
      mem8[c + 0] = mi
      c += 1
      i += 1
                unsigned<? i - 64
    goto pad if unsigned<
    c -= 64
    m = &tmp
  mlenatleast64:

  new k2k3k6k7
  k2k3k6k7 bot = k2k3k6k7_stack bot
  k2k3k6k7 top = k2k3k6k7_stack top

  new k1n1k7k2
  k1n1k7k2 bot = k1n1k7k2_stack bot
  k1n1k7k2 top = k1n1k7k2_stack top

  n2n3n3n2 = n2n3n3n2[0,1] n2n3n3n2[1] n2n3n3n2[0]

  new start2
  start2 = start2[0,1] k1n1k7k2[0,1]             # X X k1 n1
  start2 = n2n3n3n2[3] k2k3k6k7[2] start2[2,3]   # n2 k6 k1 n1

  new start3
  start3 = start3[0,1] k1n1k7k2[2,3]             # X X k7 k2
  start3 = k2k3k6k7[1] n2n3n3n2[2] start3[2,3]   # k3 n3 k7 k2

  diag0 = start0
  diag1 = start1
  diag2 = start2
  diag3 = start3

  2x nextblock = 0xff
  4x nextblock unsigned>>= 7

  2x n2n3n3n2 += nextblock

  i = 20

  mainloop1:

    4x a0 = diag1 + diag0
    4x b0 = a0 << 7
    4x b0 insert= a0 >> 25
       diag3 ^= b0
    4x a1 = diag0 + diag3
    4x b1 = a1 << 9
    4x b1 insert= a1 >> 23
       diag2 ^= b1
    4x a2 = diag3 + diag2
            diag3 = diag3[3] diag3[0,1,2]
    4x b2 = a2 << 13
    4x b2 insert= a2 >> 19
       diag1 ^= b2
    4x a3 = diag2 + diag1
            diag2 = diag2[2,3] diag2[0,1]
    4x b3 = a3 << 18
    4x b3 insert= a3 >> 14
            diag1 = diag1[1,2,3] diag1[0]
       diag0 ^= b3
    4x a0 = diag3 + diag0
    4x b0 = a0 << 7
    4x b0 insert= a0 >> 25
       diag1 ^= b0
    4x a1 = diag0 + diag1
    4x b1 = a1 << 9
    4x b1 insert= a1 >> 23
                  						unsigned>? i -= 2
       diag2 ^= b1
    4x a2 = diag1 + diag2
            diag1 = diag1[3] diag1[0,1,2]
    4x b2 = a2 << 13
    4x b2 insert= a2 >> 19
       diag3 ^= b2
    4x a3 = diag2 + diag3
            diag2 = diag2[2,3] diag2[0,1]
    4x b3 = a3 << 18
    4x b3 insert= a3 >> 14
            diag3 = diag3[1,2,3] diag3[0]
       diag0 ^= b3

  goto mainloop1 if unsigned>

  2x abab = 0xffffffff
  
  4x x0x5x10x15 = diag0 + start0
  4x x12x1x6x11 = diag1 + start1
  4x x8x13x2x7 = diag2 + start2
  4x x4x9x14x3 = diag3 + start3

  x0x1x10x11 = x0x5x10x15
  x12x13x6x7 = x12x1x6x11
  x8x9x2x3 = x8x13x2x7
  x4x5x14x15 = x4x9x14x3
  
  x0x1x10x11 = (abab & x0x1x10x11) | (~abab & x12x1x6x11)
  x12x13x6x7 = (abab & x12x13x6x7) | (~abab & x8x13x2x7)
  x8x9x2x3 = (abab & x8x9x2x3) | (~abab & x4x9x14x3)
  x4x5x14x15 = (abab & x4x5x14x15) | (~abab & x0x5x10x15)
  
  x0x1x2x3 = x0x1x10x11
  x4x5x6x7 = x4x5x14x15
  x8x9x10x11 = x8x9x2x3
  x12x13x14x15 = x12x13x6x7
  
  x0x1x2x3 = x0x1x2x3[0,1] x8x9x2x3[2,3]
  x4x5x6x7 = x4x5x6x7[0,1] x12x13x6x7[2,3]
  x8x9x10x11 = x8x9x10x11[0,1] x0x1x10x11[2,3]
  x12x13x14x15 = x12x13x14x15[0,1] x4x5x14x15[2,3]

                     =? m - 0
  goto nomessage1 if =
    m0m1m2m3 = mem128[m]
    m += 16
    m4m5m6m7 = mem128[m]
    m += 16
    m8m9m10m11 = mem128[m]
    m += 16
    m12m13m14m15 = mem128[m]
    m += 16
    x0x1x2x3 ^= m0m1m2m3
    x4x5x6x7 ^= m4m5m6m7
    x8x9x10x11 ^= m8m9m10m11
    x12x13x14x15 ^= m12m13m14m15
  nomessage1:

  mem128[c] = x0x1x2x3
  c += 16
  mem128[c] = x4x5x6x7
  c += 16
  mem128[c] = x8x9x10x11
  c += 16
  mem128[c] = x12x13x14x15
  c += 16

                          unsigned<? mlenlow - 64
  goto xmlenatleast64 if !unsigned<
    i = 0
    m = c - 64
    c = savec
    ccopy:
      ci = mem8[m + 0]
      mem8[c + 0] = ci
      m += 1
      c += 1
      i += 1
                  unsigned<? i - mlenlow
    goto ccopy if unsigned<
  xmlenatleast64:

                     unsigned>? mlenlow -= 64
goto mlenatleast1 if unsigned>

done:

new caller_r4
caller_r4 = stack_r4
new caller_q4
new caller_q5
new caller_q6
new caller_q7
caller_q4 bot = stack_q4 bot
caller_q4 top = stack_q4 top
caller_q5 bot = stack_q5 bot
caller_q5 top = stack_q5 top
caller_q6 bot = stack_q6 bot
caller_q6 top = stack_q6 top
caller_q7 bot = stack_q7 bot
caller_q7 top = stack_q7 top

int32 result
result = 0
return result
