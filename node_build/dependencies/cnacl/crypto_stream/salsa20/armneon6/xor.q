constant sigma:
  const32 1634760805
  const32 857760878
  const32 2036477234
  const32 1797285236

stack64 j01_stack
stack64 j23_stack
stack64 j45_stack
stack64 j67_stack
stack64 j89_stack
stack64 j1011_stack
stack64 j1213_stack
stack64 j1415_stack
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
int128 m0m1m2m3
int128 m4m5m6m7
int128 m8m9m10m11
int128 m12m13m14m15
int128 start0
int128 start1
int128 start2
int128 start3
stack128 stack_start2
stack128 stack_start3
stack128 next_start2
stack128 next_start3
int128 k0k1k2k3
int128 k4k5k6k7
int128 k1n1k7k2
int128 n2n3n3n2
int128 k2k3k6k7
int128 blockshift
int128 v0
int128 v4
int128 v8
int128 v12
int128 t4
int128 t12
int32 x0
int32 x1
int32 x2
int32 x3
int32 x4
int32 x5
int32 x6
int32 x7
int32 x8
int32 x9
int32 x10
int32 x11
int32 x12
int32 x13
int32 x14
int32 x15
stack64 x23_stack
stack64 x67_stack
stack64 x89_stack
stack64 x1011_stack
stack64 x1213_stack
stack64 x1415_stack
int32 m0
int32 m1
int32 m2
int32 m3
int32 m4
int32 m5
int32 m6
int32 m7
int32 m8
int32 m9
int32 m10
int32 m11
int32 m12
int32 m13
int32 m14
int32 m15
int32 j0
int32 j1
int32 j2
int32 j3
int32 j4
int32 j5
int32 j6
int32 j7
int32 j8
int32 j9
int32 j10
int32 j11
int32 j12
int32 j13
int32 j14
int32 j15
int32 s0
int32 s1
int32 s2
int32 s3
int32 s4
int32 s5
int32 s6
int32 s7
int32 s8
int32 s9
int32 s10
int32 s11
int32 s12
int32 s13
int32 s14
int32 s15
stack32 i_stack

stack128 k2k3k6k7_stack
stack128 k1n1k7k2_stack

stack512 tmp
int32 i
int32 ci
int32 mi

qpushenter crypto_stream_salsa20_armneon6_xor

stack64 stack_r45
stack64 stack_r67
stack64 stack_r89
stack64 stack_r1011
stack32 stack_r12
stack32 stack_r14
assign r4 r5 to caller_r4 caller_r5; stack_r45 = caller_r4 caller_r5
assign r6 r7 to caller_r6 caller_r7; stack_r67 = caller_r6 caller_r7
assign r8 r9 to caller_r8 caller_r9; stack_r89 = caller_r8 caller_r9
assign r10 r11 to caller_r10 caller_r11; stack_r1011 = caller_r10 caller_r11
stack_r14 = caller_r14
stack_r12 = caller_r12

int32 k
int32 n
int32 c
int32 m
int32 mlenlow
int32 mlenhigh

stack32 mlenlow_stack
stack32 mlenhigh_stack
stack32 m_stack
stack32 c_stack
stack32 c_original

c_stack = input_0
m_stack = input_1
mlenlow_stack = input_2
mlenhigh_stack = input_3

k = input_5
n = input_4

new start1
new k1n1k7k2
new k2k3k6k7
new k2k3k6k7_stack
new k1n1k7k2_stack

						i = sigma
2x n2n3n3n2 = 0xff
						assign r8 r9 to j2 j3 = mem64[k + 4]
  start1 = mem64[n] start1[1] # n0 n1 ? ?
						assign r4 r5 to j12 j13 = mem64[k + 20]
  k0k1k2k3 = mem128[k]; k += 16
						assign r6 r7 to j6 j7 = mem64[n + 0]
k2k3k6k7 = k0k1k2k3[1] k2k3k6k7[1]
						j8 = 0
  start1 = start1[1] start1[0] start1[2,3] # n1 n0 ? ?
						j9 = 0
  k4k5k6k7 = mem128[k]
						  assign r6 r7 to j6 j7; j67_stack = j6 j7
  k -= 16
						  assign r0 r1 to j8 j9; j89_stack = j8 j9
k2k3k6k7 = k2k3k6k7[0] k4k5k6k7[1]
						  assign r8 r9 to j2 j3; j23_stack = j2 j3
  start1 = start1[0,1] start1[1] k4k5k6k7[0] # n1 n0 n0 k4
						  assign r4 r5 to j12 j13; j1213_stack = j12 j13
						
						j1 = mem32[k + 0]
4x n2n3n3n2 unsigned>>= 7
						j4 = mem32[k + 12]
  k1n1k7k2 = k1n1k7k2[0,1] k4k5k6k7[3] k0k1k2k3[2] # ? ? k7 k2
						j11 = mem32[k + 16]
  k1n1k7k2 = k0k1k2k3[1] start1[0] k1n1k7k2[2,3]   # k1 n1 k7 k2
						j14 = mem32[k + 28]
  start1 = k4k5k6k7[1] k0k1k2k3[0] start1[2,3]     # k5 k0 n0 k4
						j0 = mem32[i + 0]
blockshift = n2n3n3n2
						j5 = mem32[i + 4]
  int32 ptr_k2k3k6k7
  ptr_k2k3k6k7 = &k2k3k6k7_stack
  mem128[ptr_k2k3k6k7] aligned= k2k3k6k7
						j10 = mem32[i + 8]
  int32 ptr_k1n1k7k2
  ptr_k1n1k7k2 = &k1n1k7k2_stack
  mem128[ptr_k1n1k7k2] aligned= k1n1k7k2
						j15 = mem32[i + 12]
  start0 = mem128[i]
						  assign r0 r1 to j0 j1; j01_stack = j0 j1
						mlenlow = mlenlow_stack
						  assign r4 r5 to j4 j5; j45_stack = j4 j5
						mlenhigh = mlenhigh_stack
						  assign r10 r11 to j10 j11; j1011_stack = j10 j11
						  assign r8 r9 to j14 j15; j1415_stack = j14 j15
nop

# j is sigma0 k0 k1 k2 k3 sigma1 n0 n1 b0 b1 sigma2 k4 k5 k6 k7 sigma3
# start0 is sigma
# start1 is k5 k0 n0 k4
# k1n1k7k2_stack is k1 n1 k7 k2
# k2k3k6k7_stack is k2 k3 k6 k7

                        unsigned<? mlenlow - 192
goto mlenlowbelow192 if unsigned<

mlenatleast192:

  new diag2
  new diag3
  new next_diag2
  new next_diag3
    new stack_start2
    new stack_start3
    new next_start2
    new next_start3
    int32 ptr_start2
    int32 ptr_start3

  						assign r2 r3 to x2 x3 = j23_stack
    n2n3n3n2 = n2n3n3n2[0,1] n2n3n3n2[1] n2n3n3n2[0]
  diag0 = start0

  						assign r6 r7 to x6 x7 = j67_stack
    k2k3k6k7 aligned= mem128[ptr_k2k3k6k7]
  diag1 = start1

  								mlenlow_stack = mlenlow
    diag2 = n2n3n3n2[3] k2k3k6k7[2] diag2[2,3]
  next_diag0 = diag0

  								mlenhigh_stack = mlenhigh
    diag3 = k2k3k6k7[1] n2n3n3n2[2] diag3[2,3]
  2x n2n3n3n2 += blockshift

  						assign r0 r1 to x0 x1 = j01_stack
    k1n1k7k2 aligned= mem128[ptr_k1n1k7k2]
  diag2 = diag2[0] k1n1k7k2[0]
    ptr_start2 = &stack_start2

  						assign r4 r5 to x4 x5 = j45_stack
  diag3 = diag3[0] k1n1k7k2[1]
    ptr_start3 = &stack_start3
  						assign r10 r11 to x10 x11 = j1011_stack
    mem128[ptr_start2] aligned= diag2  # n2 k6 k1 n1

  						assign r2 r3 to x2 x3; x23_stack = x2 x3
    n2n3n3n2 = n2n3n3n2[0,1] n2n3n3n2[1] n2n3n3n2[0]
  next_diag1 = diag1

  						assign r6 r7 to x6 x7; x67_stack = x6 x7
  next_diag2 = next_diag2[0] k1n1k7k2[0]
    mem128[ptr_start3] aligned= diag3  # k3 n3 k7 k2
    ptr_start2 = &next_start2

    next_diag2 = n2n3n3n2[3] k2k3k6k7[2] next_diag2[2,3]
  next_diag3 = next_diag3[0] k1n1k7k2[1]
    ptr_start3 = &next_start3

  								i = 20
    next_diag3 = k2k3k6k7[1] n2n3n3n2[2] next_diag3[2,3]
  2x n2n3n3n2 += blockshift

  						assign r8 r9 to x14 x15 = j1415_stack
    mem128[ptr_start2] aligned= next_diag2
  						assign r2 r3 to x12 x13 = j1213_stack
    mem128[ptr_start3] aligned= next_diag3
  						assign r6 r7 to x8 x9 = j89_stack

  						new x89_stack
  						new x1213_stack

  mainloop2:
							    	i_stack = i
    4x a0 = diag1 + diag0
   			4x next_a0 = next_diag1 + next_diag0
					       s4 = x0 + x12
					         s9 = x5 + x1
    4x b0 = a0 << 7
    			4x next_b0 = next_a0 << 7
    4x a0 unsigned>>= 25
    			4x next_a0 unsigned>>= 25
					       x4 ^= (s4 >>> 25)
					         x9 ^= (s9 >>> 25)
					       s8 = x4 + x0
					         s13 = x9 + x5
       diag3 ^= b0
       			next_diag3 ^= next_b0
       diag3 ^= a0
       			next_diag3 ^= next_a0
					       x8 ^= (s8 >>> 23)
					        x13 ^= (s13 >>> 23)
					      s12 = x8 + x4
					mem32[&x89_stack+4] = x9
					         s1 = x13 + x9
					x7 = mem32[&x67_stack+4]
    4x a1 = diag0 + diag3
    			4x next_a1 = next_diag0 + next_diag3
    4x b1 = a1 << 9
    			4x next_b1 = next_a1 << 9
    4x a1 unsigned>>= 23
    			4x next_a1 unsigned>>= 23
       diag2 ^= b1
       			next_diag2 ^= next_b1
       diag2 ^= a1
       			next_diag2 ^= next_a1
					      x12 ^= (s12 >>> 19)
					mem32[&x1213_stack] = x12
					         x1 ^= (s1 >>> 19)
					x2 = mem32[&x23_stack]
					       s0 = x12 + x8
					mem32[&x89_stack] = x8
					         s5 = x1 + x13
					x6 = mem32[&x67_stack]
    4x a2 = diag3 + diag2
            diag3 = diag3[3] diag3[0,1,2]
    			4x next_a2 = next_diag3 + next_diag2
    4x b2 = a2 << 13
       				next_diag3 = next_diag3[3] next_diag3[0,1,2]
    			4x next_b2 = next_a2 << 13
					       x0 ^= (s0 >>> 14)
					         x5 ^= (s5 >>> 14)
					mem32[&x1213_stack+4] = x13
					          s14 = x10 + x6
					x3 = mem32[&x23_stack+4]
					             s3 = x15 + x11
    4x a2 unsigned>>= 19
    			4x next_a2 unsigned>>= 19
       diag1 ^= b2
       			next_diag1 ^= next_b2
					          x14 ^= (s14 >>> 25)
					             x3 ^= (s3 >>> 25)
					           s2 = x14 + x10
					             s7 = x3 + x15
       diag1 ^= a2
       			next_diag1 ^= next_a2
					           x2 ^= (s2 >>> 23)
					             x7 ^= (s7 >>> 23)
					           s6 = x2 + x14
					            s11 = x7 + x3
    4x a3 = diag2 + diag1
            diag2 = diag2[2,3] diag2[0,1]
    			4x next_a3 = next_diag2 + next_diag1
    4x b3 = a3 << 18
       				next_diag2 = next_diag2[2,3] next_diag2[0,1]
    			4x next_b3 = next_a3 << 18
					           x6 ^= (s6 >>> 19)
					            x11 ^= (s11 >>> 19)
					          s10 = x6 + x2
					            s15 = x11 + x7
    4x a3 unsigned>>= 14
            diag1 = diag1[1,2,3] diag1[0]
    			4x next_a3 unsigned>>= 14
       diag0 ^= b3
       				next_diag1 = next_diag1[1,2,3] next_diag1[0]
       			next_diag0 ^= next_b3
					          x10 ^= (s10 >>> 14)
					            x15 ^= (s15 >>> 14)
					       s1 = x0 + x3
					         s6 = x5 + x4
       diag0 ^= a3
       			next_diag0 ^= next_a3
					       x1 ^= (s1 >>> 25)
					         x6 ^= (s6 >>> 25)
					       s2 = x1 + x0
					         s7 = x6 + x5
    4x a0 = diag3 + diag0
    			4x next_a0 = next_diag3 + next_diag0
    4x b0 = a0 << 7
    			4x next_b0 = next_a0 << 7
					       x2 ^= (s2 >>> 23)
					         x7 ^= (s7 >>> 23)
    4x a0 unsigned>>= 25
    			4x next_a0 unsigned>>= 25
					       s3 = x2 + x1
					         s4 = x7 + x6
       diag1 ^= b0
       			next_diag1 ^= next_b0
       diag1 ^= a0
       			next_diag1 ^= next_a0
    4x a1 = diag0 + diag1
    			4x next_a1 = next_diag0 + next_diag1
    4x b1 = a1 << 9
    			4x next_b1 = next_a1 << 9
					       x3 ^= (s3 >>> 19)
					mem32[&x67_stack] = x6
					         x4 ^= (s4 >>> 19)
					x8 = mem32[&x89_stack]
					       s0 = x3 + x2
					mem32[&x67_stack+4] = x7
					         s5 = x4 + x7
					x9 = mem32[&x89_stack+4]
					       x0 ^= (s0 >>> 14)
					mem32[&x23_stack] = x2
					         x5 ^= (s5 >>> 14)
					x12 = mem32[&x1213_stack]
    4x a1 unsigned>>= 23
    			4x next_a1 unsigned>>= 23
       diag2 ^= b1
       			next_diag2 ^= next_b1
       diag2 ^= a1
       			next_diag2 ^= next_a1
					          s11 = x10 + x9
					            s12 = x15 + x14
    4x a2 = diag1 + diag2
            diag1 = diag1[3] diag1[0,1,2]
    			4x next_a2 = next_diag1 + next_diag2
    4x b2 = a2 << 13
       				next_diag1 = next_diag1[3] next_diag1[0,1,2]
    			4x next_b2 = next_a2 << 13
    4x a2 unsigned>>= 19
    			4x next_a2 unsigned>>= 19
					          x11 ^= (s11 >>> 25)
					            x12 ^= (s12 >>> 25)
					           s8 = x11 + x10
					mem32[&x23_stack+4] = x3
					            s13 = x12 + x15
					x13 = mem32[&x1213_stack+4]
       diag3 ^= b2
       			next_diag3 ^= next_b2
       diag3 ^= a2
       			next_diag3 ^= next_a2
					           x8 ^= (s8 >>> 23)
					            x13 ^= (s13 >>> 23)
					           s9 = x8 + x11
					            s14 = x13 + x12
    4x a3 = diag2 + diag3
            diag2 = diag2[2,3] diag2[0,1]
    			4x next_a3 = next_diag2 + next_diag3
    4x b3 = a3 << 18
       				next_diag2 = next_diag2[2,3] next_diag2[0,1]
    			4x next_b3 = next_a3 << 18
					           x9 ^= (s9 >>> 19)
					            x14 ^= (s14 >>> 19)
					          s10 = x9 + x8
					            s15 = x14 + x13
    4x a3 unsigned>>= 14
            diag3 = diag3[1,2,3] diag3[0]
    			4x next_a3 unsigned>>= 14
       diag0 ^= b3
       				next_diag3 = next_diag3[1,2,3] next_diag3[0]
       			next_diag0 ^= next_b3
					          x10 ^= (s10 >>> 14)
       diag0 ^= a3
					            x15 ^= (s15 >>> 14)
       			next_diag0 ^= next_a3


assign r0 to x0
assign r1 to x1
assign r2 to x12
assign r3 to x13
assign r4 to x4
assign r5 to x5
assign r6 to x8
assign r7 to x9
assign r8 to x14
assign r9 to x15
assign r10 to x10
assign r11 to x11


    4x a0 = diag1 + diag0
   			4x next_a0 = next_diag1 + next_diag0
					       s4 = x0 + x12
					         s9 = x5 + x1
    4x b0 = a0 << 7
    			4x next_b0 = next_a0 << 7
    4x a0 unsigned>>= 25
    			4x next_a0 unsigned>>= 25
					       x4 ^= (s4 >>> 25)
					         x9 ^= (s9 >>> 25)
					       s8 = x4 + x0
					         s13 = x9 + x5
       diag3 ^= b0
       			next_diag3 ^= next_b0
       diag3 ^= a0
       			next_diag3 ^= next_a0
					       x8 ^= (s8 >>> 23)
					        x13 ^= (s13 >>> 23)
					      s12 = x8 + x4
					mem32[&x89_stack+4] = x9
					         s1 = x13 + x9
					x7 = mem32[&x67_stack+4]
    4x a1 = diag0 + diag3
    			4x next_a1 = next_diag0 + next_diag3
    4x b1 = a1 << 9
    			4x next_b1 = next_a1 << 9
    4x a1 unsigned>>= 23
    			4x next_a1 unsigned>>= 23
       diag2 ^= b1
       			next_diag2 ^= next_b1
       diag2 ^= a1
       			next_diag2 ^= next_a1
					      x12 ^= (s12 >>> 19)
					mem32[&x1213_stack] = x12
					         x1 ^= (s1 >>> 19)
					x2 = mem32[&x23_stack]
					       s0 = x12 + x8
					mem32[&x89_stack] = x8
					         s5 = x1 + x13
					x6 = mem32[&x67_stack]
    4x a2 = diag3 + diag2
            diag3 = diag3[3] diag3[0,1,2]
    			4x next_a2 = next_diag3 + next_diag2
    4x b2 = a2 << 13
       				next_diag3 = next_diag3[3] next_diag3[0,1,2]
    			4x next_b2 = next_a2 << 13
					       x0 ^= (s0 >>> 14)
					         x5 ^= (s5 >>> 14)
					mem32[&x1213_stack+4] = x13
					          s14 = x10 + x6
					x3 = mem32[&x23_stack+4]
					             s3 = x15 + x11
    4x a2 unsigned>>= 19
    			4x next_a2 unsigned>>= 19
       diag1 ^= b2
       			next_diag1 ^= next_b2
					          x14 ^= (s14 >>> 25)
					             x3 ^= (s3 >>> 25)
					           s2 = x14 + x10
					             s7 = x3 + x15
       diag1 ^= a2
       			next_diag1 ^= next_a2
					           x2 ^= (s2 >>> 23)
					             x7 ^= (s7 >>> 23)
					           s6 = x2 + x14
					            s11 = x7 + x3
    4x a3 = diag2 + diag1
            diag2 = diag2[2,3] diag2[0,1]
    			4x next_a3 = next_diag2 + next_diag1
    4x b3 = a3 << 18
       				next_diag2 = next_diag2[2,3] next_diag2[0,1]
    			4x next_b3 = next_a3 << 18
					           x6 ^= (s6 >>> 19)
					            x11 ^= (s11 >>> 19)
					          s10 = x6 + x2
					            s15 = x11 + x7
    4x a3 unsigned>>= 14
            diag1 = diag1[1,2,3] diag1[0]
    			4x next_a3 unsigned>>= 14
       diag0 ^= b3
       				next_diag1 = next_diag1[1,2,3] next_diag1[0]
       			next_diag0 ^= next_b3
					          x10 ^= (s10 >>> 14)
					            x15 ^= (s15 >>> 14)
					       s1 = x0 + x3
					         s6 = x5 + x4
       diag0 ^= a3
       			next_diag0 ^= next_a3
					       x1 ^= (s1 >>> 25)
					         x6 ^= (s6 >>> 25)
					       s2 = x1 + x0
					         s7 = x6 + x5
    4x a0 = diag3 + diag0
    			4x next_a0 = next_diag3 + next_diag0
    4x b0 = a0 << 7
    			4x next_b0 = next_a0 << 7
					       x2 ^= (s2 >>> 23)
					         x7 ^= (s7 >>> 23)
    4x a0 unsigned>>= 25
    			4x next_a0 unsigned>>= 25
					       s3 = x2 + x1
					         s4 = x7 + x6
       diag1 ^= b0
       			next_diag1 ^= next_b0
       diag1 ^= a0
       			next_diag1 ^= next_a0
    4x a1 = diag0 + diag1
    			4x next_a1 = next_diag0 + next_diag1
    4x b1 = a1 << 9
    			4x next_b1 = next_a1 << 9
					       x3 ^= (s3 >>> 19)
					mem32[&x67_stack] = x6
					         x4 ^= (s4 >>> 19)
					x8 = mem32[&x89_stack]
					       s0 = x3 + x2
					mem32[&x67_stack+4] = x7
					         s5 = x4 + x7
					x9 = mem32[&x89_stack+4]
					       x0 ^= (s0 >>> 14)
					mem32[&x23_stack] = x2
					         x5 ^= (s5 >>> 14)
					x12 = mem32[&x1213_stack]
    4x a1 unsigned>>= 23
    			4x next_a1 unsigned>>= 23
       diag2 ^= b1
       			next_diag2 ^= next_b1
       diag2 ^= a1
       			next_diag2 ^= next_a1
					          s11 = x10 + x9
					            s12 = x15 + x14
    4x a2 = diag1 + diag2
            diag1 = diag1[3] diag1[0,1,2]
    			4x next_a2 = next_diag1 + next_diag2
    4x b2 = a2 << 13
       				next_diag1 = next_diag1[3] next_diag1[0,1,2]
    			4x next_b2 = next_a2 << 13
    4x a2 unsigned>>= 19
    			4x next_a2 unsigned>>= 19
					          x11 ^= (s11 >>> 25)
					            x12 ^= (s12 >>> 25)
					           s8 = x11 + x10
					mem32[&x23_stack+4] = x3
					            s13 = x12 + x15
					x13 = mem32[&x1213_stack+4]
       diag3 ^= b2
       			next_diag3 ^= next_b2
       diag3 ^= a2
       			next_diag3 ^= next_a2
					           x8 ^= (s8 >>> 23)
					            x13 ^= (s13 >>> 23)
					           s9 = x8 + x11
					            s14 = x13 + x12
    4x a3 = diag2 + diag3
            diag2 = diag2[2,3] diag2[0,1]
    			4x next_a3 = next_diag2 + next_diag3
    4x b3 = a3 << 18
       				next_diag2 = next_diag2[2,3] next_diag2[0,1]
    			4x next_b3 = next_a3 << 18
					           x9 ^= (s9 >>> 19)
					            x14 ^= (s14 >>> 19)
					          s10 = x9 + x8
					            s15 = x14 + x13
    4x a3 unsigned>>= 14
            diag3 = diag3[1,2,3] diag3[0]
    			4x next_a3 unsigned>>= 14
       diag0 ^= b3
       				next_diag3 = next_diag3[1,2,3] next_diag3[0]
       			next_diag0 ^= next_b3
					          x10 ^= (s10 >>> 14)
    								i = i_stack
       diag0 ^= a3
					            x15 ^= (s15 >>> 14)
       			next_diag0 ^= next_a3
                  						unsigned>? i -= 4
  goto mainloop2 if unsigned>

    assign r8 r9 to x14 x15; x1415_stack = x14 x15
    assign r8 r9 to j0 j1 = j01_stack
    assign r2 r3 to x12 x13; x1213_stack = x12 x13
    assign r2 r3 to x2 x3 = x23_stack
  x0 += j0
    assign r10 r11 to x10 x11; x1011_stack = x10 x11
  x1 += j1
    assign r10 r11 to j2 j3 = j23_stack
    assign r8 r9 to j4 j5 = j45_stack
  x2 += j2
    assign r6 r7 to x8 x9; x89_stack = x8 x9
  x3 += j3
    assign r6 r7 to x6 x7 = x67_stack
    assign r10 r11 to j6 j7 = j67_stack

    m = m_stack

  x4 += j4
  x5 += j5

  x6 += j6
  x7 += j7

                     =? m - 0
  goto nomessage1 if =
      m0 = mem32[m + 0]
      m1 = mem32[m + 4]
      m2 = mem32[m + 8]
      m3 = mem32[m + 12]
    x0 ^= m0
      m4 = mem32[m + 16]
    x1 ^= m1
      m5 = mem32[m + 20]
    x2 ^= m2
      m6 = mem32[m + 24]
    x3 ^= m3
      m7 = mem32[m + 28]
    x4 ^= m4
    x5 ^= m5
    x6 ^= m6
    x7 ^= m7
  nomessage1:

  c = c_stack
				  4x v0 = diag0 + start0           # x0 x5 x10 x15
  mem32[c + 0] = x0
				  ptr_start3 = &stack_start3
  mem32[c + 4] = x1
				  v4 aligned= mem128[ptr_start3]
  mem32[c + 8] = x2
				  4x v4 += diag3                   # x4 x9 x14 x3
  mem32[c + 12] = x3
				  ptr_start2 = &stack_start2
  mem32[c + 16] = x4
				  v8 aligned= mem128[ptr_start2]
  mem32[c + 20] = x5
				  4x v12 = diag1 + start1          # x12 x1 x6 x11
  mem32[c + 24] = x6
				  4x v8 += diag2                   # x8 x13 x2 x7
  mem32[c + 28] = x7
				  2x abab = 0xffffffff
  assign r6 r7 to x8 x9 = x89_stack
				  new t4
				  t4 = v0[1] v4[0] t4[2,3]         # x5 x4 - -
  assign r0 r1 to j8 j9 = j89_stack
				  new t12
				  t12 = t12[0,1] v0[3] v4[2]       # - - x15 x14
  assign r2 r3 to x12 x13 = x1213_stack
				  v0 = (abab & v0) | (~abab & v12) # x0 x1 x10 x11
  assign r4 r5 to j12 j13 = j1213_stack
				  t4 = t4[0,1] v8[3] v12[2]        # x5 x4 x7 x6
  x8 += j8
				  t12 = v8[1] v12[0] t12[2,3]      # x13 x12 x15 x14
  x9 += j9
				  v8 = (abab & v8) | (~abab & v4)  # x8 x9 x2 x3
  x12 += j12
				  v4 = t4[1]t4[0]t4[3]t4[2]        # x4 x5 x6 x7
  x13 += j13
				  v12 = t12[1]t12[0]t12[3]t12[2]   # x12 x13 x14 x15

  carry? j8 += 3
				  v0 v8 = v0[0] v8[1] v8[0] v0[1]  # x0 x1 x2 x3 x8 x9 x10 x11
  j9 += 0 + carry

  assign r0 r1 to j8 j9; j89_stack = j8 j9

  assign r8 r9 to x14 x15 = x1415_stack
  assign r0 r1 to j14 j15 = j1415_stack

  assign r10 r11 to x10 x11 = x1011_stack
  assign r4 r5 to j10 j11 = j1011_stack

  x14 += j14
  x15 += j15
  x10 += j10
  x11 += j11
				  c += 64

                     =? m - 0
  goto nomessage2 if =
      m8 = mem32[m + 32]
      m9 = mem32[m + 36]
      m10 = mem32[m + 40]
      m11 = mem32[m + 44]
    x8 ^= m8
      m12 = mem32[m + 48]
    x9 ^= m9
      m13 = mem32[m + 52]
    x10 ^= m10
      m14 = mem32[m + 56]
    x11 ^= m11
      m15 = mem32[m + 60]
    m += 64
    x12 ^= m12
				    m0m1m2m3 = mem128[m]; m += 16
				    v0 ^= m0m1m2m3
    x13 ^= m13
				    m4m5m6m7 = mem128[m]; m += 16
				    v4 ^= m4m5m6m7
    x14 ^= m14
				    m8m9m10m11 = mem128[m]; m += 16
				    v8 ^= m8m9m10m11
    x15 ^= m15
				    m12m13m14m15 = mem128[m]; m += 16
				    v12 ^= m12m13m14m15
  nomessage2:

				  mem128[c] = v0; c += 16
				  mem128[c] = v4; c += 16
				    2x blockshift = 0xff
				  mem128[c] = v8; c += 16
				  mem128[c] = v12; c += 16
				
  mem32[c - 96] = x8
				  ptr_start3 = &next_start3
  mem32[c - 92] = x9
				  ptr_start2 = &next_start2
  mem32[c - 88] = x10
				  4x v0 = next_diag0 + start0      # x0 x5 x10 x15
				  v4 aligned= mem128[ptr_start3]
				  4x v4 += next_diag3              # x4 x9 x14 x3
				  v8 aligned= mem128[ptr_start2]
				  4x v12 = next_diag1 + start1     # x12 x1 x6 x11
				  4x v8 += next_diag2              # x8 x13 x2 x7
  mem32[c - 84] = x11
				  new t4
				  t4 = v0[1] v4[0] t4[2,3]         # x5 x4 - -
  mem32[c - 80] = x12
				  new t12
				  t12 = t12[0,1] v0[3] v4[2]       # - - x15 x14
  mem32[c - 76] = x13
				  v0 = (abab & v0) | (~abab & v12) # x0 x1 x10 x11
  mem32[c - 72] = x14
				  t4 = t4[0,1] v8[3] v12[2]        # x5 x4 x7 x6
				    4x blockshift unsigned>>= 7
  mem32[c - 68] = x15
				  t12 = v8[1] v12[0] t12[2,3]      # x13 x12 x15 x14
				  v8 = (abab & v8) | (~abab & v4)  # x8 x9 x2 x3
  mlenlow = mlenlow_stack
				  v4 = t4[1]t4[0]t4[3]t4[2]        # x4 x5 x6 x7
  mlenhigh = mlenhigh_stack
				  v12 = t12[1]t12[0]t12[3]t12[2]   # x12 x13 x14 x15
				  v0 v8 = v0[0] v8[1] v8[0] v0[1]  # x0 x1 x2 x3 x8 x9 x10 x11
				    2x n2n3n3n2 += blockshift
				
				  carry? mlenlow -= 192
				  mlenhigh -= 0 - carry
				
				                     =? m - 0
				  goto nomessage21 if =
				    m0m1m2m3 = mem128[m]; m += 16
				    v0 ^= m0m1m2m3
				    m4m5m6m7 = mem128[m]; m += 16
				    v4 ^= m4m5m6m7
				    m8m9m10m11 = mem128[m]; m += 16
				    v8 ^= m8m9m10m11
				    m12m13m14m15 = mem128[m]; m += 16
				    v12 ^= m12m13m14m15
				  nomessage21:
							
				  mem128[c] = v0; c += 16
				  mem128[c] = v4; c += 16
				  mem128[c] = v8; c += 16
				  mem128[c] = v12; c += 16
				
  m_stack = m
				    ptr_k1n1k7k2 = &k1n1k7k2_stack
				    ptr_k2k3k6k7 = &k2k3k6k7_stack
  c_stack = c

                        unsigned<? mlenlow - 192
goto mlenatleast192 if !unsigned<

mlenlowbelow192:
                       unsigned>? mlenhigh - 0
goto mlenatleast192 if unsigned>

             =? mlenlow - 0
goto done if =

goto mlenatleast1

nextblock:

  carry? mlenlow -= 64
  mlenhigh -= 0 - carry

mlenatleast1:

                       =? mlenhigh - 0
  goto handleblock if !=
                       unsigned<? mlenlow - 64
  goto handleblock if !unsigned<

    c = c_stack
    m = m_stack
    c_original = c

    c = &tmp
    c_stack = c
                        =? m - 0
    goto handleblock if =

    m_stack = c
    i = 0
    mcopy:
      mi = mem8[m]; m += 1
      i += 1
      mem8[c] = mi; c += 1
                  unsigned<? i - mlenlow
    goto mcopy if unsigned<

  handleblock:

  mlenlow_stack = mlenlow
  mlenhigh_stack = mlenhigh

  assign r2 r3 to x2 x3 = j23_stack
  assign r6 r7 to x6 x7 = j67_stack
  assign r0 r1 to x0 x1 = j01_stack
  assign r4 r5 to x4 x5 = j45_stack
  assign r10 r11 to x10 x11 = j1011_stack
  assign r8 r9 to x14 x15 = j1415_stack

  assign r2 r3 to x2 x3; x23_stack = x2 x3
  assign r6 r7 to x6 x7; x67_stack = x6 x7

  assign r2 r3 to x12 x13 = j1213_stack
  assign r6 r7 to x8 x9 = j89_stack

  i = 20

  mainloop1:
    i_stack = i

       s4 = x0 + x12
         s9 = x5 + x1


       x4 ^= (s4 >>> 25)
         x9 ^= (s9 >>> 25)
       s8 = x4 + x0
         s13 = x9 + x5

       x8 ^= (s8 >>> 23)
        x13 ^= (s13 >>> 23)
      s12 = x8 + x4
mem32[&x89_stack+4] = x9
         s1 = x13 + x9
x6 = mem32[&x67_stack]
      x12 ^= (s12 >>> 19)
mem32[&x89_stack] = x8
         x1 ^= (s1 >>> 19)
x3 = mem32[&x23_stack+4]
       s0 = x12 + x8
mem32[&x1213_stack] = x12
         s5 = x1 + x13
x2 = mem32[&x23_stack]
       x0 ^= (s0 >>> 14)
mem32[&x1213_stack+4] = x13
         x5 ^= (s5 >>> 14)
x7 = mem32[&x67_stack+4]
          s14 = x10 + x6
             s3 = x15 + x11

          x14 ^= (s14 >>> 25)
             x3 ^= (s3 >>> 25)
           s2 = x14 + x10
             s7 = x3 + x15

           x2 ^= (s2 >>> 23)
             x7 ^= (s7 >>> 23)
           s6 = x2 + x14
            s11 = x7 + x3

           x6 ^= (s6 >>> 19)
            x11 ^= (s11 >>> 19)
          s10 = x6 + x2
            s15 = x11 + x7
          x10 ^= (s10 >>> 14)
            x15 ^= (s15 >>> 14)
       s1 = x0 + x3
         s6 = x5 + x4
       x1 ^= (s1 >>> 25)
         x6 ^= (s6 >>> 25)
       s2 = x1 + x0
         s7 = x6 + x5
       x2 ^= (s2 >>> 23)
         x7 ^= (s7 >>> 23)
       s3 = x2 + x1
mem32[&x67_stack] = x6
         s4 = x7 + x6
x8 = mem32[&x89_stack]
       x3 ^= (s3 >>> 19)
mem32[&x67_stack+4] = x7
         x4 ^= (s4 >>> 19)
x9 = mem32[&x89_stack+4]
       s0 = x3 + x2
mem32[&x23_stack] = x2
         s5 = x4 + x7
x12 = mem32[&x1213_stack]
       x0 ^= (s0 >>> 14)
mem32[&x23_stack+4] = x3
         x5 ^= (s5 >>> 14)
x13 = mem32[&x1213_stack+4]
          s11 = x10 + x9
            s12 = x15 + x14
          x11 ^= (s11 >>> 25)
            x12 ^= (s12 >>> 25)
           s8 = x11 + x10
            s13 = x12 + x15
           x8 ^= (s8 >>> 23)
            x13 ^= (s13 >>> 23)
           s9 = x8 + x11
            s14 = x13 + x12
           x9 ^= (s9 >>> 19)
            x14 ^= (s14 >>> 19)
          s10 = x9 + x8
            s15 = x14 + x13
          x10 ^= (s10 >>> 14)
            x15 ^= (s15 >>> 14)

    i = i_stack
                    unsigned>? i -= 2
  goto mainloop1 if unsigned>

  assign r6 r7 to x8 x9; x89_stack = x8 x9
  assign r2 r3 to x12 x13; x1213_stack = x12 x13
  assign r10 r11 to x10 x11; x1011_stack = x10 x11
  assign r8 r9 to x14 x15; x1415_stack = x14 x15

  assign r2 r3 to x2 x3 = x23_stack
  assign r6 r7 to x6 x7 = x67_stack

  assign r8 r9 to j0 j1 = j01_stack
  assign r10 r11 to j2 j3 = j23_stack

  x0 += j0
  x1 += j1
  x2 += j2
  x3 += j3

  assign r8 r9 to j4 j5 = j45_stack
  assign r10 r11 to j6 j7 = j67_stack

  x4 += j4
  x5 += j5
  x6 += j6
  x7 += j7
  m = m_stack
                     =? m - 0
  goto nomessage10 if =
      m0 = mem32[m + 0]
      m1 = mem32[m + 4]
      m2 = mem32[m + 8]
      m3 = mem32[m + 12]
    x0 ^= m0
      m4 = mem32[m + 16]
    x1 ^= m1
      m5 = mem32[m + 20]
    x2 ^= m2
      m6 = mem32[m + 24]
    x3 ^= m3
      m7 = mem32[m + 28]
    x4 ^= m4
    x5 ^= m5
    x6 ^= m6
    x7 ^= m7
  nomessage10:

  c = c_stack
  mem32[c + 0] = x0
  mem32[c + 4] = x1
  mem32[c + 8] = x2
  mem32[c + 12] = x3
  mem32[c + 16] = x4
  mem32[c + 20] = x5
  mem32[c + 24] = x6
  mem32[c + 28] = x7

  assign r6 r7 to x8 x9 = x89_stack
  assign r10 r11 to x10 x11 = x1011_stack

  assign r0 r1 to j8 j9 = j89_stack
  assign r4 r5 to j10 j11 = j1011_stack

  x8 += j8
  x9 += j9
  x10 += j10
  x11 += j11

  carry? j8 += 1
  j9 += 0 + carry

  assign r0 r1 to j8 j9; j89_stack = j8 j9

  assign r2 r3 to x12 x13 = x1213_stack
  assign r8 r9 to x14 x15 = x1415_stack

  assign r4 r5 to j12 j13 = j1213_stack
  assign r0 r1 to j14 j15 = j1415_stack

  x12 += j12
  x13 += j13
  x14 += j14
  x15 += j15

                     =? m - 0
  goto nomessage11 if =
      m8 = mem32[m + 32]
      m9 = mem32[m + 36]
      m10 = mem32[m + 40]
      m11 = mem32[m + 44]
    x8 ^= m8
      m12 = mem32[m + 48]
    x9 ^= m9
      m13 = mem32[m + 52]
    x10 ^= m10
      m14 = mem32[m + 56]
    x11 ^= m11
      m15 = mem32[m + 60]
    x12 ^= m12
    x13 ^= m13
    x14 ^= m14
    x15 ^= m15
    m += 64
    m_stack = m
  nomessage11:

  mem32[c + 32] = x8
  mem32[c + 36] = x9
  mem32[c + 40] = x10
  mem32[c + 44] = x11
  mem32[c + 48] = x12
  mem32[c + 52] = x13
  mem32[c + 56] = x14
  mem32[c + 60] = x15
  c += 64
  c_stack = c

  mlenlow = mlenlow_stack
  mlenhigh = mlenhigh_stack

                  unsigned>? mlenlow - 64
goto nextblock if unsigned>
                   =? mlenhigh - 0
goto nextblock if !=
             =? mlenlow - 64
goto done if =

  m = &tmp
  c = c_original
  i = 0

  ccopy:
    ci = mem8[m]; m += 1
    i += 1
    mem8[c] = ci; c += 1
                unsigned<? i - mlenlow
  goto ccopy if unsigned<

done:

assign r4 r5 to caller_r4 caller_r5 = stack_r45
assign r6 r7 to caller_r6 caller_r7 = stack_r67
assign r8 r9 to caller_r8 caller_r9 = stack_r89
assign r10 r11 to caller_r10 caller_r11 = stack_r1011
caller_r12 = stack_r12
caller_r14 = stack_r14

int32 result
result = 0
qpopreturn result
