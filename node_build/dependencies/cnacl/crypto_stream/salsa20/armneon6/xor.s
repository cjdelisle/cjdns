
# qhasm: int32 input_0

# qhasm: int32 input_1

# qhasm: int32 input_2

# qhasm: int32 input_3

# qhasm: stack32 input_4

# qhasm: stack32 input_5

# qhasm: stack32 input_6

# qhasm: stack32 input_7

# qhasm: int32 caller_r4

# qhasm: int32 caller_r5

# qhasm: int32 caller_r6

# qhasm: int32 caller_r7

# qhasm: int32 caller_r8

# qhasm: int32 caller_r9

# qhasm: int32 caller_r10

# qhasm: int32 caller_r11

# qhasm: int32 caller_r12

# qhasm: int32 caller_r14

# qhasm: reg128 caller_q4

# qhasm: reg128 caller_q5

# qhasm: reg128 caller_q6

# qhasm: reg128 caller_q7

# qhasm: startcode
.fpu neon
.text

# qhasm: constant sigma:
.p2align 4
sigma:

# qhasm:   const32 1634760805
.word 1634760805

# qhasm:   const32 857760878
.word 857760878

# qhasm:   const32 2036477234
.word 2036477234

# qhasm:   const32 1797285236
.word 1797285236

# qhasm: stack64 j01_stack

# qhasm: stack64 j23_stack

# qhasm: stack64 j45_stack

# qhasm: stack64 j67_stack

# qhasm: stack64 j89_stack

# qhasm: stack64 j1011_stack

# qhasm: stack64 j1213_stack

# qhasm: stack64 j1415_stack

# qhasm: int128 abab

# qhasm: int128 diag0

# qhasm: int128 diag1

# qhasm: int128 diag2

# qhasm: int128 diag3

# qhasm: int128 a0

# qhasm: int128 a1

# qhasm: int128 a2

# qhasm: int128 a3

# qhasm: int128 b0

# qhasm: int128 b1

# qhasm: int128 b2

# qhasm: int128 b3

# qhasm: int128 next_diag0

# qhasm: int128 next_diag1

# qhasm: int128 next_diag2

# qhasm: int128 next_diag3

# qhasm: int128 next_a0

# qhasm: int128 next_a1

# qhasm: int128 next_a2

# qhasm: int128 next_a3

# qhasm: int128 next_b0

# qhasm: int128 next_b1

# qhasm: int128 next_b2

# qhasm: int128 next_b3

# qhasm: int128 m0m1m2m3

# qhasm: int128 m4m5m6m7

# qhasm: int128 m8m9m10m11

# qhasm: int128 m12m13m14m15

# qhasm: int128 start0

# qhasm: int128 start1

# qhasm: int128 start2

# qhasm: int128 start3

# qhasm: stack128 stack_start2

# qhasm: stack128 stack_start3

# qhasm: stack128 next_start2

# qhasm: stack128 next_start3

# qhasm: int128 k0k1k2k3

# qhasm: int128 k4k5k6k7

# qhasm: int128 k1n1k7k2

# qhasm: int128 n2n3n3n2

# qhasm: int128 k2k3k6k7

# qhasm: int128 blockshift

# qhasm: int128 v0

# qhasm: int128 v4

# qhasm: int128 v8

# qhasm: int128 v12

# qhasm: int128 t4

# qhasm: int128 t12

# qhasm: int32 x0

# qhasm: int32 x1

# qhasm: int32 x2

# qhasm: int32 x3

# qhasm: int32 x4

# qhasm: int32 x5

# qhasm: int32 x6

# qhasm: int32 x7

# qhasm: int32 x8

# qhasm: int32 x9

# qhasm: int32 x10

# qhasm: int32 x11

# qhasm: int32 x12

# qhasm: int32 x13

# qhasm: int32 x14

# qhasm: int32 x15

# qhasm: stack64 x23_stack

# qhasm: stack64 x67_stack

# qhasm: stack64 x89_stack

# qhasm: stack64 x1011_stack

# qhasm: stack64 x1213_stack

# qhasm: stack64 x1415_stack

# qhasm: int32 m0

# qhasm: int32 m1

# qhasm: int32 m2

# qhasm: int32 m3

# qhasm: int32 m4

# qhasm: int32 m5

# qhasm: int32 m6

# qhasm: int32 m7

# qhasm: int32 m8

# qhasm: int32 m9

# qhasm: int32 m10

# qhasm: int32 m11

# qhasm: int32 m12

# qhasm: int32 m13

# qhasm: int32 m14

# qhasm: int32 m15

# qhasm: int32 j0

# qhasm: int32 j1

# qhasm: int32 j2

# qhasm: int32 j3

# qhasm: int32 j4

# qhasm: int32 j5

# qhasm: int32 j6

# qhasm: int32 j7

# qhasm: int32 j8

# qhasm: int32 j9

# qhasm: int32 j10

# qhasm: int32 j11

# qhasm: int32 j12

# qhasm: int32 j13

# qhasm: int32 j14

# qhasm: int32 j15

# qhasm: int32 s0

# qhasm: int32 s1

# qhasm: int32 s2

# qhasm: int32 s3

# qhasm: int32 s4

# qhasm: int32 s5

# qhasm: int32 s6

# qhasm: int32 s7

# qhasm: int32 s8

# qhasm: int32 s9

# qhasm: int32 s10

# qhasm: int32 s11

# qhasm: int32 s12

# qhasm: int32 s13

# qhasm: int32 s14

# qhasm: int32 s15

# qhasm: stack32 i_stack

# qhasm: stack128 k2k3k6k7_stack

# qhasm: stack128 k1n1k7k2_stack

# qhasm: stack512 tmp

# qhasm: int32 i

# qhasm: int32 ci

# qhasm: int32 mi

# qhasm: qpushenter crypto_stream_salsa20_armneon6_xor
.align 2
.global _crypto_stream_salsa20_armneon6_xor
.global crypto_stream_salsa20_armneon6_xor
.type _crypto_stream_salsa20_armneon6_xor STT_FUNC
.type crypto_stream_salsa20_armneon6_xor STT_FUNC
_crypto_stream_salsa20_armneon6_xor:
crypto_stream_salsa20_armneon6_xor:
vpush {q4,q5,q6,q7}
mov r12,sp
sub sp,sp,#352
and sp,sp,#0xffffffe0

# qhasm: stack64 stack_r45

# qhasm: stack64 stack_r67

# qhasm: stack64 stack_r89

# qhasm: stack64 stack_r1011

# qhasm: stack32 stack_r12

# qhasm: stack32 stack_r14

# qhasm: assign r4 r5 to caller_r4 caller_r5; stack_r45 = caller_r4 caller_r5
# asm 1: strd <caller_r4=int32#5,>stack_r45=stack64#1
# asm 2: strd <caller_r4=r4,>stack_r45=[sp,#0]
strd r4,[sp,#0]

# qhasm: assign r6 r7 to caller_r6 caller_r7; stack_r67 = caller_r6 caller_r7
# asm 1: strd <caller_r6=int32#7,>stack_r67=stack64#2
# asm 2: strd <caller_r6=r6,>stack_r67=[sp,#8]
strd r6,[sp,#8]

# qhasm: assign r8 r9 to caller_r8 caller_r9; stack_r89 = caller_r8 caller_r9
# asm 1: strd <caller_r8=int32#9,>stack_r89=stack64#3
# asm 2: strd <caller_r8=r8,>stack_r89=[sp,#16]
strd r8,[sp,#16]

# qhasm: assign r10 r11 to caller_r10 caller_r11; stack_r1011 = caller_r10 caller_r11
# asm 1: strd <caller_r10=int32#11,>stack_r1011=stack64#4
# asm 2: strd <caller_r10=r10,>stack_r1011=[sp,#24]
strd r10,[sp,#24]

# qhasm: stack_r14 = caller_r14
# asm 1: str <caller_r14=int32#14,>stack_r14=stack32#1
# asm 2: str <caller_r14=r14,>stack_r14=[sp,#224]
str r14,[sp,#224]

# qhasm: stack_r12 = caller_r12
# asm 1: str <caller_r12=int32#13,>stack_r12=stack32#2
# asm 2: str <caller_r12=r12,>stack_r12=[sp,#228]
str r12,[sp,#228]

# qhasm: int32 k

# qhasm: int32 n

# qhasm: int32 c

# qhasm: int32 m

# qhasm: int32 mlenlow

# qhasm: int32 mlenhigh

# qhasm: stack32 mlenlow_stack

# qhasm: stack32 mlenhigh_stack

# qhasm: stack32 m_stack

# qhasm: stack32 c_stack

# qhasm: stack32 c_original

# qhasm: c_stack = input_0
# asm 1: str <input_0=int32#1,>c_stack=stack32#3
# asm 2: str <input_0=r0,>c_stack=[sp,#232]
str r0,[sp,#232]

# qhasm: m_stack = input_1
# asm 1: str <input_1=int32#2,>m_stack=stack32#4
# asm 2: str <input_1=r1,>m_stack=[sp,#236]
str r1,[sp,#236]

# qhasm: mlenlow_stack = input_2
# asm 1: str <input_2=int32#3,>mlenlow_stack=stack32#5
# asm 2: str <input_2=r2,>mlenlow_stack=[sp,#240]
str r2,[sp,#240]

# qhasm: mlenhigh_stack = input_3
# asm 1: str <input_3=int32#4,>mlenhigh_stack=stack32#6
# asm 2: str <input_3=r3,>mlenhigh_stack=[sp,#244]
str r3,[sp,#244]

# qhasm: k = input_5
# asm 1: ldr >k=int32#3,<input_5=stack32#arg18
# asm 2: ldr >k=r2,<input_5=[r12,#68]
ldr r2,[r12,#68]

# qhasm: n = input_4
# asm 1: ldr >n=int32#1,<input_4=stack32#arg17
# asm 2: ldr >n=r0,<input_4=[r12,#64]
ldr r0,[r12,#64]

# qhasm: new start1

# qhasm: new k1n1k7k2

# qhasm: new k2k3k6k7

# qhasm: new k2k3k6k7_stack

# qhasm: new k1n1k7k2_stack

# qhasm: 						i = sigma
# asm 1: ldr >i=int32#4,=sigma
# asm 2: ldr >i=r3,=sigma
ldr r3,=sigma

# qhasm: 2x n2n3n3n2 = 0xff
# asm 1: vmov.i64 >n2n3n3n2=reg128#4,#0xff
# asm 2: vmov.i64 >n2n3n3n2=q3,#0xff
vmov.i64 q3,#0xff

# qhasm: 						assign r8 r9 to j2 j3 = mem64[k + 4]
# asm 1: ldrd >j2=int32#9,[<k=int32#3,#4]
# asm 2: ldrd >j2=r8,[<k=r2,#4]
ldrd r8,[r2,#4]

# qhasm:   start1 = mem64[n] start1[1] 
# asm 1: vld1.8 {<start1=reg128#1%bot},[<n=int32#1]
# asm 2: vld1.8 {<start1=d0},[<n=r0]
vld1.8 {d0},[r0]

# qhasm: 						assign r4 r5 to j12 j13 = mem64[k + 20]
# asm 1: ldrd >j12=int32#5,[<k=int32#3,#20]
# asm 2: ldrd >j12=r4,[<k=r2,#20]
ldrd r4,[r2,#20]

# qhasm:   k0k1k2k3 = mem128[k]; k += 16
# asm 1: vld1.8 {>k0k1k2k3=reg128#5%bot->k0k1k2k3=reg128#5%top},[<k=int32#3]!
# asm 2: vld1.8 {>k0k1k2k3=d8->k0k1k2k3=d9},[<k=r2]!
vld1.8 {d8-d9},[r2]!

# qhasm: 						assign r6 r7 to j6 j7 = mem64[n + 0]
# asm 1: ldrd >j6=int32#7,[<n=int32#1,#0]
# asm 2: ldrd >j6=r6,[<n=r0,#0]
ldrd r6,[r0,#0]

# qhasm: k2k3k6k7 = k0k1k2k3[1] k2k3k6k7[1]
# asm 1: vmov <k2k3k6k7=reg128#3%bot,<k0k1k2k3=reg128#5%top
# asm 2: vmov <k2k3k6k7=d4,<k0k1k2k3=d9
vmov d4,d9

# qhasm: 						j8 = 0
# asm 1: ldr >j8=int32#1,=0
# asm 2: ldr >j8=r0,=0
ldr r0,=0

# qhasm:   start1 = start1[1] start1[0] start1[2,3] 
# asm 1: vrev64.i32 <start1=reg128#1%bot,<start1=reg128#1%bot
# asm 2: vrev64.i32 <start1=d0,<start1=d0
vrev64.i32 d0,d0

# qhasm: 						j9 = 0
# asm 1: ldr >j9=int32#2,=0
# asm 2: ldr >j9=r1,=0
ldr r1,=0

# qhasm:   k4k5k6k7 = mem128[k]
# asm 1: vld1.8 {>k4k5k6k7=reg128#6%bot->k4k5k6k7=reg128#6%top},[<k=int32#3]
# asm 2: vld1.8 {>k4k5k6k7=d10->k4k5k6k7=d11},[<k=r2]
vld1.8 {d10-d11},[r2]

# qhasm: 						  assign r6 r7 to j6 j7; j67_stack = j6 j7
# asm 1: strd <j6=int32#7,>j67_stack=stack64#5
# asm 2: strd <j6=r6,>j67_stack=[sp,#32]
strd r6,[sp,#32]

# qhasm:   k -= 16
# asm 1: sub >k=int32#3,<k=int32#3,#16
# asm 2: sub >k=r2,<k=r2,#16
sub r2,r2,#16

# qhasm: 						  assign r0 r1 to j8 j9; j89_stack = j8 j9
# asm 1: strd <j8=int32#1,>j89_stack=stack64#6
# asm 2: strd <j8=r0,>j89_stack=[sp,#40]
strd r0,[sp,#40]

# qhasm: k2k3k6k7 = k2k3k6k7[0] k4k5k6k7[1]
# asm 1: vmov <k2k3k6k7=reg128#3%top,<k4k5k6k7=reg128#6%top
# asm 2: vmov <k2k3k6k7=d5,<k4k5k6k7=d11
vmov d5,d11

# qhasm: 						  assign r8 r9 to j2 j3; j23_stack = j2 j3
# asm 1: strd <j2=int32#9,>j23_stack=stack64#7
# asm 2: strd <j2=r8,>j23_stack=[sp,#48]
strd r8,[sp,#48]

# qhasm:   start1 = start1[0,1] start1[1] k4k5k6k7[0] 
# asm 1: vext.32 <start1=reg128#1%top,<start1=reg128#1%bot,<k4k5k6k7=reg128#6%bot,#1
# asm 2: vext.32 <start1=d1,<start1=d0,<k4k5k6k7=d10,#1
vext.32 d1,d0,d10,#1

# qhasm: 						  assign r4 r5 to j12 j13; j1213_stack = j12 j13
# asm 1: strd <j12=int32#5,>j1213_stack=stack64#8
# asm 2: strd <j12=r4,>j1213_stack=[sp,#56]
strd r4,[sp,#56]

# qhasm: 						j1 = mem32[k + 0]
# asm 1: ldr >j1=int32#2,[<k=int32#3,#0]
# asm 2: ldr >j1=r1,[<k=r2,#0]
ldr r1,[r2,#0]

# qhasm: 4x n2n3n3n2 unsigned>>= 7
# asm 1: vshr.u32 >n2n3n3n2=reg128#4,<n2n3n3n2=reg128#4,#7
# asm 2: vshr.u32 >n2n3n3n2=q3,<n2n3n3n2=q3,#7
vshr.u32 q3,q3,#7

# qhasm: 						j4 = mem32[k + 12]
# asm 1: ldr >j4=int32#5,[<k=int32#3,#12]
# asm 2: ldr >j4=r4,[<k=r2,#12]
ldr r4,[r2,#12]

# qhasm:   k1n1k7k2 = k1n1k7k2[0,1] k4k5k6k7[3] k0k1k2k3[2] 
# asm 1: vext.32 <k1n1k7k2=reg128#2%top,<k4k5k6k7=reg128#6%top,<k0k1k2k3=reg128#5%top,#1
# asm 2: vext.32 <k1n1k7k2=d3,<k4k5k6k7=d11,<k0k1k2k3=d9,#1
vext.32 d3,d11,d9,#1

# qhasm: 						j11 = mem32[k + 16]
# asm 1: ldr >j11=int32#12,[<k=int32#3,#16]
# asm 2: ldr >j11=r11,[<k=r2,#16]
ldr r11,[r2,#16]

# qhasm:   k1n1k7k2 = k0k1k2k3[1] start1[0] k1n1k7k2[2,3]   
# asm 1: vext.32 <k1n1k7k2=reg128#2%bot,<k0k1k2k3=reg128#5%bot,<start1=reg128#1%bot,#1
# asm 2: vext.32 <k1n1k7k2=d2,<k0k1k2k3=d8,<start1=d0,#1
vext.32 d2,d8,d0,#1

# qhasm: 						j14 = mem32[k + 28]
# asm 1: ldr >j14=int32#9,[<k=int32#3,#28]
# asm 2: ldr >j14=r8,[<k=r2,#28]
ldr r8,[r2,#28]

# qhasm:   start1 = k4k5k6k7[1] k0k1k2k3[0] start1[2,3]     
# asm 1: vext.32 <start1=reg128#1%bot,<k4k5k6k7=reg128#6%bot,<k0k1k2k3=reg128#5%bot,#1
# asm 2: vext.32 <start1=d0,<k4k5k6k7=d10,<k0k1k2k3=d8,#1
vext.32 d0,d10,d8,#1

# qhasm: 						j0 = mem32[i + 0]
# asm 1: ldr >j0=int32#1,[<i=int32#4,#0]
# asm 2: ldr >j0=r0,[<i=r3,#0]
ldr r0,[r3,#0]

# qhasm: blockshift = n2n3n3n2
# asm 1: vmov >blockshift=reg128#5,<n2n3n3n2=reg128#4
# asm 2: vmov >blockshift=q4,<n2n3n3n2=q3
vmov q4,q3

# qhasm: 						j5 = mem32[i + 4]
# asm 1: ldr >j5=int32#6,[<i=int32#4,#4]
# asm 2: ldr >j5=r5,[<i=r3,#4]
ldr r5,[r3,#4]

# qhasm:   int32 ptr_k2k3k6k7

# qhasm:   ptr_k2k3k6k7 = &k2k3k6k7_stack
# asm 1: lea >ptr_k2k3k6k7=int32#13,<k2k3k6k7_stack=stack128#1
# asm 2: lea >ptr_k2k3k6k7=r12,<k2k3k6k7_stack=[sp,#256]
add r12,sp,#256

# qhasm:   mem128[ptr_k2k3k6k7] aligned= k2k3k6k7
# asm 1: vst1.8 {<k2k3k6k7=reg128#3%bot-<k2k3k6k7=reg128#3%top},[<ptr_k2k3k6k7=int32#13,: 128]
# asm 2: vst1.8 {<k2k3k6k7=d4-<k2k3k6k7=d5},[<ptr_k2k3k6k7=r12,: 128]
vst1.8 {d4-d5},[r12,: 128]

# qhasm: 						j10 = mem32[i + 8]
# asm 1: ldr >j10=int32#11,[<i=int32#4,#8]
# asm 2: ldr >j10=r10,[<i=r3,#8]
ldr r10,[r3,#8]

# qhasm:   int32 ptr_k1n1k7k2

# qhasm:   ptr_k1n1k7k2 = &k1n1k7k2_stack
# asm 1: lea >ptr_k1n1k7k2=int32#14,<k1n1k7k2_stack=stack128#2
# asm 2: lea >ptr_k1n1k7k2=r14,<k1n1k7k2_stack=[sp,#272]
add r14,sp,#272

# qhasm:   mem128[ptr_k1n1k7k2] aligned= k1n1k7k2
# asm 1: vst1.8 {<k1n1k7k2=reg128#2%bot-<k1n1k7k2=reg128#2%top},[<ptr_k1n1k7k2=int32#14,: 128]
# asm 2: vst1.8 {<k1n1k7k2=d2-<k1n1k7k2=d3},[<ptr_k1n1k7k2=r14,: 128]
vst1.8 {d2-d3},[r14,: 128]

# qhasm: 						j15 = mem32[i + 12]
# asm 1: ldr >j15=int32#10,[<i=int32#4,#12]
# asm 2: ldr >j15=r9,[<i=r3,#12]
ldr r9,[r3,#12]

# qhasm:   start0 = mem128[i]
# asm 1: vld1.8 {>start0=reg128#2%bot->start0=reg128#2%top},[<i=int32#4]
# asm 2: vld1.8 {>start0=d2->start0=d3},[<i=r3]
vld1.8 {d2-d3},[r3]

# qhasm: 						  assign r0 r1 to j0 j1; j01_stack = j0 j1
# asm 1: strd <j0=int32#1,>j01_stack=stack64#9
# asm 2: strd <j0=r0,>j01_stack=[sp,#64]
strd r0,[sp,#64]

# qhasm: 						mlenlow = mlenlow_stack
# asm 1: ldr >mlenlow=int32#1,<mlenlow_stack=stack32#5
# asm 2: ldr >mlenlow=r0,<mlenlow_stack=[sp,#240]
ldr r0,[sp,#240]

# qhasm: 						  assign r4 r5 to j4 j5; j45_stack = j4 j5
# asm 1: strd <j4=int32#5,>j45_stack=stack64#10
# asm 2: strd <j4=r4,>j45_stack=[sp,#72]
strd r4,[sp,#72]

# qhasm: 						mlenhigh = mlenhigh_stack
# asm 1: ldr >mlenhigh=int32#2,<mlenhigh_stack=stack32#6
# asm 2: ldr >mlenhigh=r1,<mlenhigh_stack=[sp,#244]
ldr r1,[sp,#244]

# qhasm: 						  assign r10 r11 to j10 j11; j1011_stack = j10 j11
# asm 1: strd <j10=int32#11,>j1011_stack=stack64#11
# asm 2: strd <j10=r10,>j1011_stack=[sp,#80]
strd r10,[sp,#80]

# qhasm: 						  assign r8 r9 to j14 j15; j1415_stack = j14 j15
# asm 1: strd <j14=int32#9,>j1415_stack=stack64#12
# asm 2: strd <j14=r8,>j1415_stack=[sp,#88]
strd r8,[sp,#88]

# qhasm: nop
nop

# qhasm:                         unsigned<? mlenlow - 192
# asm 1: cmp <mlenlow=int32#1,#192
# asm 2: cmp <mlenlow=r0,#192
cmp r0,#192

# qhasm: goto mlenlowbelow192 if unsigned<
blo ._mlenlowbelow192

# qhasm: mlenatleast192:
._mlenatleast192:

# qhasm:   new diag2

# qhasm:   new diag3

# qhasm:   new next_diag2

# qhasm:   new next_diag3

# qhasm:     new stack_start2

# qhasm:     new stack_start3

# qhasm:     new next_start2

# qhasm:     new next_start3

# qhasm:     int32 ptr_start2

# qhasm:     int32 ptr_start3

# qhasm:   						assign r2 r3 to x2 x3 = j23_stack
# asm 1: ldrd >x2=int32#3,<j23_stack=stack64#7
# asm 2: ldrd >x2=r2,<j23_stack=[sp,#48]
ldrd r2,[sp,#48]

# qhasm:     n2n3n3n2 = n2n3n3n2[0,1] n2n3n3n2[1] n2n3n3n2[0]
# asm 1: vext.32 <n2n3n3n2=reg128#4%top,<n2n3n3n2=reg128#4%bot,<n2n3n3n2=reg128#4%bot,#1
# asm 2: vext.32 <n2n3n3n2=d7,<n2n3n3n2=d6,<n2n3n3n2=d6,#1
vext.32 d7,d6,d6,#1

# qhasm:   diag0 = start0
# asm 1: vmov >diag0=reg128#9,<start0=reg128#2
# asm 2: vmov >diag0=q8,<start0=q1
vmov q8,q1

# qhasm:   						assign r6 r7 to x6 x7 = j67_stack
# asm 1: ldrd >x6=int32#7,<j67_stack=stack64#5
# asm 2: ldrd >x6=r6,<j67_stack=[sp,#32]
ldrd r6,[sp,#32]

# qhasm:     k2k3k6k7 aligned= mem128[ptr_k2k3k6k7]
# asm 1: vld1.8 {>k2k3k6k7=reg128#10%bot->k2k3k6k7=reg128#10%top},[<ptr_k2k3k6k7=int32#13,: 128]
# asm 2: vld1.8 {>k2k3k6k7=d18->k2k3k6k7=d19},[<ptr_k2k3k6k7=r12,: 128]
vld1.8 {d18-d19},[r12,: 128]

# qhasm:   diag1 = start1
# asm 1: vmov >diag1=reg128#11,<start1=reg128#1
# asm 2: vmov >diag1=q10,<start1=q0
vmov q10,q0

# qhasm:   								mlenlow_stack = mlenlow
# asm 1: str <mlenlow=int32#1,>mlenlow_stack=stack32#5
# asm 2: str <mlenlow=r0,>mlenlow_stack=[sp,#240]
str r0,[sp,#240]

# qhasm:     diag2 = n2n3n3n2[3] k2k3k6k7[2] diag2[2,3]
# asm 1: vext.32 <diag2=reg128#3%bot,<n2n3n3n2=reg128#4%top,<k2k3k6k7=reg128#10%top,#1
# asm 2: vext.32 <diag2=d4,<n2n3n3n2=d7,<k2k3k6k7=d19,#1
vext.32 d4,d7,d19,#1

# qhasm:   next_diag0 = diag0
# asm 1: vmov >next_diag0=reg128#12,<diag0=reg128#9
# asm 2: vmov >next_diag0=q11,<diag0=q8
vmov q11,q8

# qhasm:   								mlenhigh_stack = mlenhigh
# asm 1: str <mlenhigh=int32#2,>mlenhigh_stack=stack32#6
# asm 2: str <mlenhigh=r1,>mlenhigh_stack=[sp,#244]
str r1,[sp,#244]

# qhasm:     diag3 = k2k3k6k7[1] n2n3n3n2[2] diag3[2,3]
# asm 1: vext.32 <diag3=reg128#6%bot,<k2k3k6k7=reg128#10%bot,<n2n3n3n2=reg128#4%top,#1
# asm 2: vext.32 <diag3=d10,<k2k3k6k7=d18,<n2n3n3n2=d7,#1
vext.32 d10,d18,d7,#1

# qhasm:   2x n2n3n3n2 += blockshift
# asm 1: vadd.i64 >n2n3n3n2=reg128#4,<n2n3n3n2=reg128#4,<blockshift=reg128#5
# asm 2: vadd.i64 >n2n3n3n2=q3,<n2n3n3n2=q3,<blockshift=q4
vadd.i64 q3,q3,q4

# qhasm:   						assign r0 r1 to x0 x1 = j01_stack
# asm 1: ldrd >x0=int32#1,<j01_stack=stack64#9
# asm 2: ldrd >x0=r0,<j01_stack=[sp,#64]
ldrd r0,[sp,#64]

# qhasm:     k1n1k7k2 aligned= mem128[ptr_k1n1k7k2]
# asm 1: vld1.8 {>k1n1k7k2=reg128#13%bot->k1n1k7k2=reg128#13%top},[<ptr_k1n1k7k2=int32#14,: 128]
# asm 2: vld1.8 {>k1n1k7k2=d24->k1n1k7k2=d25},[<ptr_k1n1k7k2=r14,: 128]
vld1.8 {d24-d25},[r14,: 128]

# qhasm:   diag2 = diag2[0] k1n1k7k2[0]
# asm 1: vmov <diag2=reg128#3%top,<k1n1k7k2=reg128#13%bot
# asm 2: vmov <diag2=d5,<k1n1k7k2=d24
vmov d5,d24

# qhasm:     ptr_start2 = &stack_start2
# asm 1: lea >ptr_start2=int32#9,<stack_start2=stack128#3
# asm 2: lea >ptr_start2=r8,<stack_start2=[sp,#288]
add r8,sp,#288

# qhasm:   						assign r4 r5 to x4 x5 = j45_stack
# asm 1: ldrd >x4=int32#5,<j45_stack=stack64#10
# asm 2: ldrd >x4=r4,<j45_stack=[sp,#72]
ldrd r4,[sp,#72]

# qhasm:   diag3 = diag3[0] k1n1k7k2[1]
# asm 1: vmov <diag3=reg128#6%top,<k1n1k7k2=reg128#13%top
# asm 2: vmov <diag3=d11,<k1n1k7k2=d25
vmov d11,d25

# qhasm:     ptr_start3 = &stack_start3
# asm 1: lea >ptr_start3=int32#10,<stack_start3=stack128#4
# asm 2: lea >ptr_start3=r9,<stack_start3=[sp,#304]
add r9,sp,#304

# qhasm:   						assign r10 r11 to x10 x11 = j1011_stack
# asm 1: ldrd >x10=int32#11,<j1011_stack=stack64#11
# asm 2: ldrd >x10=r10,<j1011_stack=[sp,#80]
ldrd r10,[sp,#80]

# qhasm:     mem128[ptr_start2] aligned= diag2  
# asm 1: vst1.8 {<diag2=reg128#3%bot-<diag2=reg128#3%top},[<ptr_start2=int32#9,: 128]
# asm 2: vst1.8 {<diag2=d4-<diag2=d5},[<ptr_start2=r8,: 128]
vst1.8 {d4-d5},[r8,: 128]

# qhasm:   						assign r2 r3 to x2 x3; x23_stack = x2 x3
# asm 1: strd <x2=int32#3,>x23_stack=stack64#13
# asm 2: strd <x2=r2,>x23_stack=[sp,#96]
strd r2,[sp,#96]

# qhasm:     n2n3n3n2 = n2n3n3n2[0,1] n2n3n3n2[1] n2n3n3n2[0]
# asm 1: vext.32 <n2n3n3n2=reg128#4%top,<n2n3n3n2=reg128#4%bot,<n2n3n3n2=reg128#4%bot,#1
# asm 2: vext.32 <n2n3n3n2=d7,<n2n3n3n2=d6,<n2n3n3n2=d6,#1
vext.32 d7,d6,d6,#1

# qhasm:   next_diag1 = diag1
# asm 1: vmov >next_diag1=reg128#14,<diag1=reg128#11
# asm 2: vmov >next_diag1=q13,<diag1=q10
vmov q13,q10

# qhasm:   						assign r6 r7 to x6 x7; x67_stack = x6 x7
# asm 1: strd <x6=int32#7,>x67_stack=stack64#14
# asm 2: strd <x6=r6,>x67_stack=[sp,#104]
strd r6,[sp,#104]

# qhasm:   next_diag2 = next_diag2[0] k1n1k7k2[0]
# asm 1: vmov <next_diag2=reg128#7%top,<k1n1k7k2=reg128#13%bot
# asm 2: vmov <next_diag2=d13,<k1n1k7k2=d24
vmov d13,d24

# qhasm:     mem128[ptr_start3] aligned= diag3  
# asm 1: vst1.8 {<diag3=reg128#6%bot-<diag3=reg128#6%top},[<ptr_start3=int32#10,: 128]
# asm 2: vst1.8 {<diag3=d10-<diag3=d11},[<ptr_start3=r9,: 128]
vst1.8 {d10-d11},[r9,: 128]

# qhasm:     ptr_start2 = &next_start2
# asm 1: lea >ptr_start2=int32#3,<next_start2=stack128#5
# asm 2: lea >ptr_start2=r2,<next_start2=[sp,#320]
add r2,sp,#320

# qhasm:     next_diag2 = n2n3n3n2[3] k2k3k6k7[2] next_diag2[2,3]
# asm 1: vext.32 <next_diag2=reg128#7%bot,<n2n3n3n2=reg128#4%top,<k2k3k6k7=reg128#10%top,#1
# asm 2: vext.32 <next_diag2=d12,<n2n3n3n2=d7,<k2k3k6k7=d19,#1
vext.32 d12,d7,d19,#1

# qhasm:   next_diag3 = next_diag3[0] k1n1k7k2[1]
# asm 1: vmov <next_diag3=reg128#8%top,<k1n1k7k2=reg128#13%top
# asm 2: vmov <next_diag3=d15,<k1n1k7k2=d25
vmov d15,d25

# qhasm:     ptr_start3 = &next_start3
# asm 1: lea >ptr_start3=int32#7,<next_start3=stack128#6
# asm 2: lea >ptr_start3=r6,<next_start3=[sp,#336]
add r6,sp,#336

# qhasm:   								i = 20
# asm 1: ldr >i=int32#13,=20
# asm 2: ldr >i=r12,=20
ldr r12,=20

# qhasm:     next_diag3 = k2k3k6k7[1] n2n3n3n2[2] next_diag3[2,3]
# asm 1: vext.32 <next_diag3=reg128#8%bot,<k2k3k6k7=reg128#10%bot,<n2n3n3n2=reg128#4%top,#1
# asm 2: vext.32 <next_diag3=d14,<k2k3k6k7=d18,<n2n3n3n2=d7,#1
vext.32 d14,d18,d7,#1

# qhasm:   2x n2n3n3n2 += blockshift
# asm 1: vadd.i64 >n2n3n3n2=reg128#4,<n2n3n3n2=reg128#4,<blockshift=reg128#5
# asm 2: vadd.i64 >n2n3n3n2=q3,<n2n3n3n2=q3,<blockshift=q4
vadd.i64 q3,q3,q4

# qhasm:   						assign r8 r9 to x14 x15 = j1415_stack
# asm 1: ldrd >x14=int32#9,<j1415_stack=stack64#12
# asm 2: ldrd >x14=r8,<j1415_stack=[sp,#88]
ldrd r8,[sp,#88]

# qhasm:     mem128[ptr_start2] aligned= next_diag2
# asm 1: vst1.8 {<next_diag2=reg128#7%bot-<next_diag2=reg128#7%top},[<ptr_start2=int32#3,: 128]
# asm 2: vst1.8 {<next_diag2=d12-<next_diag2=d13},[<ptr_start2=r2,: 128]
vst1.8 {d12-d13},[r2,: 128]

# qhasm:   						assign r2 r3 to x12 x13 = j1213_stack
# asm 1: ldrd >x12=int32#3,<j1213_stack=stack64#8
# asm 2: ldrd >x12=r2,<j1213_stack=[sp,#56]
ldrd r2,[sp,#56]

# qhasm:     mem128[ptr_start3] aligned= next_diag3
# asm 1: vst1.8 {<next_diag3=reg128#8%bot-<next_diag3=reg128#8%top},[<ptr_start3=int32#7,: 128]
# asm 2: vst1.8 {<next_diag3=d14-<next_diag3=d15},[<ptr_start3=r6,: 128]
vst1.8 {d14-d15},[r6,: 128]

# qhasm:   						assign r6 r7 to x8 x9 = j89_stack
# asm 1: ldrd >x8=int32#7,<j89_stack=stack64#6
# asm 2: ldrd >x8=r6,<j89_stack=[sp,#40]
ldrd r6,[sp,#40]

# qhasm:   						new x89_stack

# qhasm:   						new x1213_stack

# qhasm:   mainloop2:
._mainloop2:

# qhasm: 							    	i_stack = i
# asm 1: str <i=int32#13,>i_stack=stack32#7
# asm 2: str <i=r12,>i_stack=[sp,#248]
str r12,[sp,#248]

# qhasm:     4x a0 = diag1 + diag0
# asm 1: vadd.i32 >a0=reg128#5,<diag1=reg128#11,<diag0=reg128#9
# asm 2: vadd.i32 >a0=q4,<diag1=q10,<diag0=q8
vadd.i32 q4,q10,q8

# qhasm:    			4x next_a0 = next_diag1 + next_diag0
# asm 1: vadd.i32 >next_a0=reg128#10,<next_diag1=reg128#14,<next_diag0=reg128#12
# asm 2: vadd.i32 >next_a0=q9,<next_diag1=q13,<next_diag0=q11
vadd.i32 q9,q13,q11

# qhasm: 					       s4 = x0 + x12
# asm 1: add >s4=int32#13,<x0=int32#1,<x12=int32#3
# asm 2: add >s4=r12,<x0=r0,<x12=r2
add r12,r0,r2

# qhasm: 					         s9 = x5 + x1
# asm 1: add >s9=int32#14,<x5=int32#6,<x1=int32#2
# asm 2: add >s9=r14,<x5=r5,<x1=r1
add r14,r5,r1

# qhasm:     4x b0 = a0 << 7
# asm 1: vshl.i32 >b0=reg128#13,<a0=reg128#5,#7
# asm 2: vshl.i32 >b0=q12,<a0=q4,#7
vshl.i32 q12,q4,#7

# qhasm:     			4x next_b0 = next_a0 << 7
# asm 1: vshl.i32 >next_b0=reg128#15,<next_a0=reg128#10,#7
# asm 2: vshl.i32 >next_b0=q14,<next_a0=q9,#7
vshl.i32 q14,q9,#7

# qhasm:     4x a0 unsigned>>= 25
# asm 1: vshr.u32 >a0=reg128#5,<a0=reg128#5,#25
# asm 2: vshr.u32 >a0=q4,<a0=q4,#25
vshr.u32 q4,q4,#25

# qhasm:     			4x next_a0 unsigned>>= 25
# asm 1: vshr.u32 >next_a0=reg128#10,<next_a0=reg128#10,#25
# asm 2: vshr.u32 >next_a0=q9,<next_a0=q9,#25
vshr.u32 q9,q9,#25

# qhasm: 					       x4 ^= (s4 >>> 25)
# asm 1: eor >x4=int32#5,<x4=int32#5,<s4=int32#13,ROR #25
# asm 2: eor >x4=r4,<x4=r4,<s4=r12,ROR #25
eor r4,r4,r12,ROR #25

# qhasm: 					         x9 ^= (s9 >>> 25)
# asm 1: eor >x9=int32#8,<x9=int32#8,<s9=int32#14,ROR #25
# asm 2: eor >x9=r7,<x9=r7,<s9=r14,ROR #25
eor r7,r7,r14,ROR #25

# qhasm: 					       s8 = x4 + x0
# asm 1: add >s8=int32#13,<x4=int32#5,<x0=int32#1
# asm 2: add >s8=r12,<x4=r4,<x0=r0
add r12,r4,r0

# qhasm: 					         s13 = x9 + x5
# asm 1: add >s13=int32#14,<x9=int32#8,<x5=int32#6
# asm 2: add >s13=r14,<x9=r7,<x5=r5
add r14,r7,r5

# qhasm:        diag3 ^= b0
# asm 1: veor >diag3=reg128#6,<diag3=reg128#6,<b0=reg128#13
# asm 2: veor >diag3=q5,<diag3=q5,<b0=q12
veor q5,q5,q12

# qhasm:        			next_diag3 ^= next_b0
# asm 1: veor >next_diag3=reg128#8,<next_diag3=reg128#8,<next_b0=reg128#15
# asm 2: veor >next_diag3=q7,<next_diag3=q7,<next_b0=q14
veor q7,q7,q14

# qhasm:        diag3 ^= a0
# asm 1: veor >diag3=reg128#5,<diag3=reg128#6,<a0=reg128#5
# asm 2: veor >diag3=q4,<diag3=q5,<a0=q4
veor q4,q5,q4

# qhasm:        			next_diag3 ^= next_a0
# asm 1: veor >next_diag3=reg128#6,<next_diag3=reg128#8,<next_a0=reg128#10
# asm 2: veor >next_diag3=q5,<next_diag3=q7,<next_a0=q9
veor q5,q7,q9

# qhasm: 					       x8 ^= (s8 >>> 23)
# asm 1: eor >x8=int32#7,<x8=int32#7,<s8=int32#13,ROR #23
# asm 2: eor >x8=r6,<x8=r6,<s8=r12,ROR #23
eor r6,r6,r12,ROR #23

# qhasm: 					        x13 ^= (s13 >>> 23)
# asm 1: eor >x13=int32#4,<x13=int32#4,<s13=int32#14,ROR #23
# asm 2: eor >x13=r3,<x13=r3,<s13=r14,ROR #23
eor r3,r3,r14,ROR #23

# qhasm: 					      s12 = x8 + x4
# asm 1: add >s12=int32#13,<x8=int32#7,<x4=int32#5
# asm 2: add >s12=r12,<x8=r6,<x4=r4
add r12,r6,r4

# qhasm: 					mem32[&x89_stack+4] = x9
# asm 1: str <x9=int32#8,<x89_stack=stack64#15
# asm 2: str <x9=r7,<x89_stack=[sp,#116]
str r7,[sp,#116]

# qhasm: 					         s1 = x13 + x9
# asm 1: add >s1=int32#8,<x13=int32#4,<x9=int32#8
# asm 2: add >s1=r7,<x13=r3,<x9=r7
add r7,r3,r7

# qhasm: 					x7 = mem32[&x67_stack+4]
# asm 1: ldr >x7=int32#14,<x67_stack=stack64#14
# asm 2: ldr >x7=r14,<x67_stack=[sp,#108]
ldr r14,[sp,#108]

# qhasm:     4x a1 = diag0 + diag3
# asm 1: vadd.i32 >a1=reg128#8,<diag0=reg128#9,<diag3=reg128#5
# asm 2: vadd.i32 >a1=q7,<diag0=q8,<diag3=q4
vadd.i32 q7,q8,q4

# qhasm:     			4x next_a1 = next_diag0 + next_diag3
# asm 1: vadd.i32 >next_a1=reg128#10,<next_diag0=reg128#12,<next_diag3=reg128#6
# asm 2: vadd.i32 >next_a1=q9,<next_diag0=q11,<next_diag3=q5
vadd.i32 q9,q11,q5

# qhasm:     4x b1 = a1 << 9
# asm 1: vshl.i32 >b1=reg128#13,<a1=reg128#8,#9
# asm 2: vshl.i32 >b1=q12,<a1=q7,#9
vshl.i32 q12,q7,#9

# qhasm:     			4x next_b1 = next_a1 << 9
# asm 1: vshl.i32 >next_b1=reg128#15,<next_a1=reg128#10,#9
# asm 2: vshl.i32 >next_b1=q14,<next_a1=q9,#9
vshl.i32 q14,q9,#9

# qhasm:     4x a1 unsigned>>= 23
# asm 1: vshr.u32 >a1=reg128#8,<a1=reg128#8,#23
# asm 2: vshr.u32 >a1=q7,<a1=q7,#23
vshr.u32 q7,q7,#23

# qhasm:     			4x next_a1 unsigned>>= 23
# asm 1: vshr.u32 >next_a1=reg128#10,<next_a1=reg128#10,#23
# asm 2: vshr.u32 >next_a1=q9,<next_a1=q9,#23
vshr.u32 q9,q9,#23

# qhasm:        diag2 ^= b1
# asm 1: veor >diag2=reg128#3,<diag2=reg128#3,<b1=reg128#13
# asm 2: veor >diag2=q2,<diag2=q2,<b1=q12
veor q2,q2,q12

# qhasm:        			next_diag2 ^= next_b1
# asm 1: veor >next_diag2=reg128#7,<next_diag2=reg128#7,<next_b1=reg128#15
# asm 2: veor >next_diag2=q6,<next_diag2=q6,<next_b1=q14
veor q6,q6,q14

# qhasm:        diag2 ^= a1
# asm 1: veor >diag2=reg128#3,<diag2=reg128#3,<a1=reg128#8
# asm 2: veor >diag2=q2,<diag2=q2,<a1=q7
veor q2,q2,q7

# qhasm:        			next_diag2 ^= next_a1
# asm 1: veor >next_diag2=reg128#7,<next_diag2=reg128#7,<next_a1=reg128#10
# asm 2: veor >next_diag2=q6,<next_diag2=q6,<next_a1=q9
veor q6,q6,q9

# qhasm: 					      x12 ^= (s12 >>> 19)
# asm 1: eor >x12=int32#3,<x12=int32#3,<s12=int32#13,ROR #19
# asm 2: eor >x12=r2,<x12=r2,<s12=r12,ROR #19
eor r2,r2,r12,ROR #19

# qhasm: 					mem32[&x1213_stack] = x12
# asm 1: str <x12=int32#3,<x1213_stack=stack64#16
# asm 2: str <x12=r2,<x1213_stack=[sp,#120]
str r2,[sp,#120]

# qhasm: 					         x1 ^= (s1 >>> 19)
# asm 1: eor >x1=int32#2,<x1=int32#2,<s1=int32#8,ROR #19
# asm 2: eor >x1=r1,<x1=r1,<s1=r7,ROR #19
eor r1,r1,r7,ROR #19

# qhasm: 					x2 = mem32[&x23_stack]
# asm 1: ldr >x2=int32#8,<x23_stack=stack64#13
# asm 2: ldr >x2=r7,<x23_stack=[sp,#96]
ldr r7,[sp,#96]

# qhasm: 					       s0 = x12 + x8
# asm 1: add >s0=int32#3,<x12=int32#3,<x8=int32#7
# asm 2: add >s0=r2,<x12=r2,<x8=r6
add r2,r2,r6

# qhasm: 					mem32[&x89_stack] = x8
# asm 1: str <x8=int32#7,<x89_stack=stack64#15
# asm 2: str <x8=r6,<x89_stack=[sp,#112]
str r6,[sp,#112]

# qhasm: 					         s5 = x1 + x13
# asm 1: add >s5=int32#7,<x1=int32#2,<x13=int32#4
# asm 2: add >s5=r6,<x1=r1,<x13=r3
add r6,r1,r3

# qhasm: 					x6 = mem32[&x67_stack]
# asm 1: ldr >x6=int32#13,<x67_stack=stack64#14
# asm 2: ldr >x6=r12,<x67_stack=[sp,#104]
ldr r12,[sp,#104]

# qhasm:     4x a2 = diag3 + diag2
# asm 1: vadd.i32 >a2=reg128#8,<diag3=reg128#5,<diag2=reg128#3
# asm 2: vadd.i32 >a2=q7,<diag3=q4,<diag2=q2
vadd.i32 q7,q4,q2

# qhasm:             diag3 = diag3[3] diag3[0,1,2]
# asm 1: vext.32 >diag3=reg128#5,<diag3=reg128#5,<diag3=reg128#5,#3
# asm 2: vext.32 >diag3=q4,<diag3=q4,<diag3=q4,#3
vext.32 q4,q4,q4,#3

# qhasm:     			4x next_a2 = next_diag3 + next_diag2
# asm 1: vadd.i32 >next_a2=reg128#10,<next_diag3=reg128#6,<next_diag2=reg128#7
# asm 2: vadd.i32 >next_a2=q9,<next_diag3=q5,<next_diag2=q6
vadd.i32 q9,q5,q6

# qhasm:     4x b2 = a2 << 13
# asm 1: vshl.i32 >b2=reg128#13,<a2=reg128#8,#13
# asm 2: vshl.i32 >b2=q12,<a2=q7,#13
vshl.i32 q12,q7,#13

# qhasm:        				next_diag3 = next_diag3[3] next_diag3[0,1,2]
# asm 1: vext.32 >next_diag3=reg128#6,<next_diag3=reg128#6,<next_diag3=reg128#6,#3
# asm 2: vext.32 >next_diag3=q5,<next_diag3=q5,<next_diag3=q5,#3
vext.32 q5,q5,q5,#3

# qhasm:     			4x next_b2 = next_a2 << 13
# asm 1: vshl.i32 >next_b2=reg128#15,<next_a2=reg128#10,#13
# asm 2: vshl.i32 >next_b2=q14,<next_a2=q9,#13
vshl.i32 q14,q9,#13

# qhasm: 					       x0 ^= (s0 >>> 14)
# asm 1: eor >x0=int32#1,<x0=int32#1,<s0=int32#3,ROR #14
# asm 2: eor >x0=r0,<x0=r0,<s0=r2,ROR #14
eor r0,r0,r2,ROR #14

# qhasm: 					         x5 ^= (s5 >>> 14)
# asm 1: eor >x5=int32#3,<x5=int32#6,<s5=int32#7,ROR #14
# asm 2: eor >x5=r2,<x5=r5,<s5=r6,ROR #14
eor r2,r5,r6,ROR #14

# qhasm: 					mem32[&x1213_stack+4] = x13
# asm 1: str <x13=int32#4,<x1213_stack=stack64#16
# asm 2: str <x13=r3,<x1213_stack=[sp,#124]
str r3,[sp,#124]

# qhasm: 					          s14 = x10 + x6
# asm 1: add >s14=int32#4,<x10=int32#11,<x6=int32#13
# asm 2: add >s14=r3,<x10=r10,<x6=r12
add r3,r10,r12

# qhasm: 					x3 = mem32[&x23_stack+4]
# asm 1: ldr >x3=int32#6,<x23_stack=stack64#13
# asm 2: ldr >x3=r5,<x23_stack=[sp,#100]
ldr r5,[sp,#100]

# qhasm: 					             s3 = x15 + x11
# asm 1: add >s3=int32#7,<x15=int32#10,<x11=int32#12
# asm 2: add >s3=r6,<x15=r9,<x11=r11
add r6,r9,r11

# qhasm:     4x a2 unsigned>>= 19
# asm 1: vshr.u32 >a2=reg128#8,<a2=reg128#8,#19
# asm 2: vshr.u32 >a2=q7,<a2=q7,#19
vshr.u32 q7,q7,#19

# qhasm:     			4x next_a2 unsigned>>= 19
# asm 1: vshr.u32 >next_a2=reg128#10,<next_a2=reg128#10,#19
# asm 2: vshr.u32 >next_a2=q9,<next_a2=q9,#19
vshr.u32 q9,q9,#19

# qhasm:        diag1 ^= b2
# asm 1: veor >diag1=reg128#11,<diag1=reg128#11,<b2=reg128#13
# asm 2: veor >diag1=q10,<diag1=q10,<b2=q12
veor q10,q10,q12

# qhasm:        			next_diag1 ^= next_b2
# asm 1: veor >next_diag1=reg128#13,<next_diag1=reg128#14,<next_b2=reg128#15
# asm 2: veor >next_diag1=q12,<next_diag1=q13,<next_b2=q14
veor q12,q13,q14

# qhasm: 					          x14 ^= (s14 >>> 25)
# asm 1: eor >x14=int32#9,<x14=int32#9,<s14=int32#4,ROR #25
# asm 2: eor >x14=r8,<x14=r8,<s14=r3,ROR #25
eor r8,r8,r3,ROR #25

# qhasm: 					             x3 ^= (s3 >>> 25)
# asm 1: eor >x3=int32#4,<x3=int32#6,<s3=int32#7,ROR #25
# asm 2: eor >x3=r3,<x3=r5,<s3=r6,ROR #25
eor r3,r5,r6,ROR #25

# qhasm: 					           s2 = x14 + x10
# asm 1: add >s2=int32#6,<x14=int32#9,<x10=int32#11
# asm 2: add >s2=r5,<x14=r8,<x10=r10
add r5,r8,r10

# qhasm: 					             s7 = x3 + x15
# asm 1: add >s7=int32#7,<x3=int32#4,<x15=int32#10
# asm 2: add >s7=r6,<x3=r3,<x15=r9
add r6,r3,r9

# qhasm:        diag1 ^= a2
# asm 1: veor >diag1=reg128#8,<diag1=reg128#11,<a2=reg128#8
# asm 2: veor >diag1=q7,<diag1=q10,<a2=q7
veor q7,q10,q7

# qhasm:        			next_diag1 ^= next_a2
# asm 1: veor >next_diag1=reg128#10,<next_diag1=reg128#13,<next_a2=reg128#10
# asm 2: veor >next_diag1=q9,<next_diag1=q12,<next_a2=q9
veor q9,q12,q9

# qhasm: 					           x2 ^= (s2 >>> 23)
# asm 1: eor >x2=int32#6,<x2=int32#8,<s2=int32#6,ROR #23
# asm 2: eor >x2=r5,<x2=r7,<s2=r5,ROR #23
eor r5,r7,r5,ROR #23

# qhasm: 					             x7 ^= (s7 >>> 23)
# asm 1: eor >x7=int32#7,<x7=int32#14,<s7=int32#7,ROR #23
# asm 2: eor >x7=r6,<x7=r14,<s7=r6,ROR #23
eor r6,r14,r6,ROR #23

# qhasm: 					           s6 = x2 + x14
# asm 1: add >s6=int32#8,<x2=int32#6,<x14=int32#9
# asm 2: add >s6=r7,<x2=r5,<x14=r8
add r7,r5,r8

# qhasm: 					            s11 = x7 + x3
# asm 1: add >s11=int32#14,<x7=int32#7,<x3=int32#4
# asm 2: add >s11=r14,<x7=r6,<x3=r3
add r14,r6,r3

# qhasm:     4x a3 = diag2 + diag1
# asm 1: vadd.i32 >a3=reg128#11,<diag2=reg128#3,<diag1=reg128#8
# asm 2: vadd.i32 >a3=q10,<diag2=q2,<diag1=q7
vadd.i32 q10,q2,q7

# qhasm:             diag2 = diag2[2,3] diag2[0,1]
# asm 1: vswp <diag2=reg128#3%bot,<diag2=reg128#3%top
# asm 2: vswp <diag2=d4,<diag2=d5
vswp d4,d5

# qhasm:     			4x next_a3 = next_diag2 + next_diag1
# asm 1: vadd.i32 >next_a3=reg128#13,<next_diag2=reg128#7,<next_diag1=reg128#10
# asm 2: vadd.i32 >next_a3=q12,<next_diag2=q6,<next_diag1=q9
vadd.i32 q12,q6,q9

# qhasm:     4x b3 = a3 << 18
# asm 1: vshl.i32 >b3=reg128#14,<a3=reg128#11,#18
# asm 2: vshl.i32 >b3=q13,<a3=q10,#18
vshl.i32 q13,q10,#18

# qhasm:        				next_diag2 = next_diag2[2,3] next_diag2[0,1]
# asm 1: vswp <next_diag2=reg128#7%bot,<next_diag2=reg128#7%top
# asm 2: vswp <next_diag2=d12,<next_diag2=d13
vswp d12,d13

# qhasm:     			4x next_b3 = next_a3 << 18
# asm 1: vshl.i32 >next_b3=reg128#15,<next_a3=reg128#13,#18
# asm 2: vshl.i32 >next_b3=q14,<next_a3=q12,#18
vshl.i32 q14,q12,#18

# qhasm: 					           x6 ^= (s6 >>> 19)
# asm 1: eor >x6=int32#8,<x6=int32#13,<s6=int32#8,ROR #19
# asm 2: eor >x6=r7,<x6=r12,<s6=r7,ROR #19
eor r7,r12,r7,ROR #19

# qhasm: 					            x11 ^= (s11 >>> 19)
# asm 1: eor >x11=int32#12,<x11=int32#12,<s11=int32#14,ROR #19
# asm 2: eor >x11=r11,<x11=r11,<s11=r14,ROR #19
eor r11,r11,r14,ROR #19

# qhasm: 					          s10 = x6 + x2
# asm 1: add >s10=int32#13,<x6=int32#8,<x2=int32#6
# asm 2: add >s10=r12,<x6=r7,<x2=r5
add r12,r7,r5

# qhasm: 					            s15 = x11 + x7
# asm 1: add >s15=int32#14,<x11=int32#12,<x7=int32#7
# asm 2: add >s15=r14,<x11=r11,<x7=r6
add r14,r11,r6

# qhasm:     4x a3 unsigned>>= 14
# asm 1: vshr.u32 >a3=reg128#11,<a3=reg128#11,#14
# asm 2: vshr.u32 >a3=q10,<a3=q10,#14
vshr.u32 q10,q10,#14

# qhasm:             diag1 = diag1[1,2,3] diag1[0]
# asm 1: vext.32 >diag1=reg128#8,<diag1=reg128#8,<diag1=reg128#8,#1
# asm 2: vext.32 >diag1=q7,<diag1=q7,<diag1=q7,#1
vext.32 q7,q7,q7,#1

# qhasm:     			4x next_a3 unsigned>>= 14
# asm 1: vshr.u32 >next_a3=reg128#13,<next_a3=reg128#13,#14
# asm 2: vshr.u32 >next_a3=q12,<next_a3=q12,#14
vshr.u32 q12,q12,#14

# qhasm:        diag0 ^= b3
# asm 1: veor >diag0=reg128#9,<diag0=reg128#9,<b3=reg128#14
# asm 2: veor >diag0=q8,<diag0=q8,<b3=q13
veor q8,q8,q13

# qhasm:        				next_diag1 = next_diag1[1,2,3] next_diag1[0]
# asm 1: vext.32 >next_diag1=reg128#10,<next_diag1=reg128#10,<next_diag1=reg128#10,#1
# asm 2: vext.32 >next_diag1=q9,<next_diag1=q9,<next_diag1=q9,#1
vext.32 q9,q9,q9,#1

# qhasm:        			next_diag0 ^= next_b3
# asm 1: veor >next_diag0=reg128#12,<next_diag0=reg128#12,<next_b3=reg128#15
# asm 2: veor >next_diag0=q11,<next_diag0=q11,<next_b3=q14
veor q11,q11,q14

# qhasm: 					          x10 ^= (s10 >>> 14)
# asm 1: eor >x10=int32#11,<x10=int32#11,<s10=int32#13,ROR #14
# asm 2: eor >x10=r10,<x10=r10,<s10=r12,ROR #14
eor r10,r10,r12,ROR #14

# qhasm: 					            x15 ^= (s15 >>> 14)
# asm 1: eor >x15=int32#10,<x15=int32#10,<s15=int32#14,ROR #14
# asm 2: eor >x15=r9,<x15=r9,<s15=r14,ROR #14
eor r9,r9,r14,ROR #14

# qhasm: 					       s1 = x0 + x3
# asm 1: add >s1=int32#13,<x0=int32#1,<x3=int32#4
# asm 2: add >s1=r12,<x0=r0,<x3=r3
add r12,r0,r3

# qhasm: 					         s6 = x5 + x4
# asm 1: add >s6=int32#14,<x5=int32#3,<x4=int32#5
# asm 2: add >s6=r14,<x5=r2,<x4=r4
add r14,r2,r4

# qhasm:        diag0 ^= a3
# asm 1: veor >diag0=reg128#9,<diag0=reg128#9,<a3=reg128#11
# asm 2: veor >diag0=q8,<diag0=q8,<a3=q10
veor q8,q8,q10

# qhasm:        			next_diag0 ^= next_a3
# asm 1: veor >next_diag0=reg128#11,<next_diag0=reg128#12,<next_a3=reg128#13
# asm 2: veor >next_diag0=q10,<next_diag0=q11,<next_a3=q12
veor q10,q11,q12

# qhasm: 					       x1 ^= (s1 >>> 25)
# asm 1: eor >x1=int32#2,<x1=int32#2,<s1=int32#13,ROR #25
# asm 2: eor >x1=r1,<x1=r1,<s1=r12,ROR #25
eor r1,r1,r12,ROR #25

# qhasm: 					         x6 ^= (s6 >>> 25)
# asm 1: eor >x6=int32#8,<x6=int32#8,<s6=int32#14,ROR #25
# asm 2: eor >x6=r7,<x6=r7,<s6=r14,ROR #25
eor r7,r7,r14,ROR #25

# qhasm: 					       s2 = x1 + x0
# asm 1: add >s2=int32#13,<x1=int32#2,<x0=int32#1
# asm 2: add >s2=r12,<x1=r1,<x0=r0
add r12,r1,r0

# qhasm: 					         s7 = x6 + x5
# asm 1: add >s7=int32#14,<x6=int32#8,<x5=int32#3
# asm 2: add >s7=r14,<x6=r7,<x5=r2
add r14,r7,r2

# qhasm:     4x a0 = diag3 + diag0
# asm 1: vadd.i32 >a0=reg128#12,<diag3=reg128#5,<diag0=reg128#9
# asm 2: vadd.i32 >a0=q11,<diag3=q4,<diag0=q8
vadd.i32 q11,q4,q8

# qhasm:     			4x next_a0 = next_diag3 + next_diag0
# asm 1: vadd.i32 >next_a0=reg128#13,<next_diag3=reg128#6,<next_diag0=reg128#11
# asm 2: vadd.i32 >next_a0=q12,<next_diag3=q5,<next_diag0=q10
vadd.i32 q12,q5,q10

# qhasm:     4x b0 = a0 << 7
# asm 1: vshl.i32 >b0=reg128#14,<a0=reg128#12,#7
# asm 2: vshl.i32 >b0=q13,<a0=q11,#7
vshl.i32 q13,q11,#7

# qhasm:     			4x next_b0 = next_a0 << 7
# asm 1: vshl.i32 >next_b0=reg128#15,<next_a0=reg128#13,#7
# asm 2: vshl.i32 >next_b0=q14,<next_a0=q12,#7
vshl.i32 q14,q12,#7

# qhasm: 					       x2 ^= (s2 >>> 23)
# asm 1: eor >x2=int32#6,<x2=int32#6,<s2=int32#13,ROR #23
# asm 2: eor >x2=r5,<x2=r5,<s2=r12,ROR #23
eor r5,r5,r12,ROR #23

# qhasm: 					         x7 ^= (s7 >>> 23)
# asm 1: eor >x7=int32#7,<x7=int32#7,<s7=int32#14,ROR #23
# asm 2: eor >x7=r6,<x7=r6,<s7=r14,ROR #23
eor r6,r6,r14,ROR #23

# qhasm:     4x a0 unsigned>>= 25
# asm 1: vshr.u32 >a0=reg128#12,<a0=reg128#12,#25
# asm 2: vshr.u32 >a0=q11,<a0=q11,#25
vshr.u32 q11,q11,#25

# qhasm:     			4x next_a0 unsigned>>= 25
# asm 1: vshr.u32 >next_a0=reg128#13,<next_a0=reg128#13,#25
# asm 2: vshr.u32 >next_a0=q12,<next_a0=q12,#25
vshr.u32 q12,q12,#25

# qhasm: 					       s3 = x2 + x1
# asm 1: add >s3=int32#13,<x2=int32#6,<x1=int32#2
# asm 2: add >s3=r12,<x2=r5,<x1=r1
add r12,r5,r1

# qhasm: 					         s4 = x7 + x6
# asm 1: add >s4=int32#14,<x7=int32#7,<x6=int32#8
# asm 2: add >s4=r14,<x7=r6,<x6=r7
add r14,r6,r7

# qhasm:        diag1 ^= b0
# asm 1: veor >diag1=reg128#8,<diag1=reg128#8,<b0=reg128#14
# asm 2: veor >diag1=q7,<diag1=q7,<b0=q13
veor q7,q7,q13

# qhasm:        			next_diag1 ^= next_b0
# asm 1: veor >next_diag1=reg128#10,<next_diag1=reg128#10,<next_b0=reg128#15
# asm 2: veor >next_diag1=q9,<next_diag1=q9,<next_b0=q14
veor q9,q9,q14

# qhasm:        diag1 ^= a0
# asm 1: veor >diag1=reg128#8,<diag1=reg128#8,<a0=reg128#12
# asm 2: veor >diag1=q7,<diag1=q7,<a0=q11
veor q7,q7,q11

# qhasm:        			next_diag1 ^= next_a0
# asm 1: veor >next_diag1=reg128#10,<next_diag1=reg128#10,<next_a0=reg128#13
# asm 2: veor >next_diag1=q9,<next_diag1=q9,<next_a0=q12
veor q9,q9,q12

# qhasm:     4x a1 = diag0 + diag1
# asm 1: vadd.i32 >a1=reg128#12,<diag0=reg128#9,<diag1=reg128#8
# asm 2: vadd.i32 >a1=q11,<diag0=q8,<diag1=q7
vadd.i32 q11,q8,q7

# qhasm:     			4x next_a1 = next_diag0 + next_diag1
# asm 1: vadd.i32 >next_a1=reg128#13,<next_diag0=reg128#11,<next_diag1=reg128#10
# asm 2: vadd.i32 >next_a1=q12,<next_diag0=q10,<next_diag1=q9
vadd.i32 q12,q10,q9

# qhasm:     4x b1 = a1 << 9
# asm 1: vshl.i32 >b1=reg128#14,<a1=reg128#12,#9
# asm 2: vshl.i32 >b1=q13,<a1=q11,#9
vshl.i32 q13,q11,#9

# qhasm:     			4x next_b1 = next_a1 << 9
# asm 1: vshl.i32 >next_b1=reg128#15,<next_a1=reg128#13,#9
# asm 2: vshl.i32 >next_b1=q14,<next_a1=q12,#9
vshl.i32 q14,q12,#9

# qhasm: 					       x3 ^= (s3 >>> 19)
# asm 1: eor >x3=int32#4,<x3=int32#4,<s3=int32#13,ROR #19
# asm 2: eor >x3=r3,<x3=r3,<s3=r12,ROR #19
eor r3,r3,r12,ROR #19

# qhasm: 					mem32[&x67_stack] = x6
# asm 1: str <x6=int32#8,<x67_stack=stack64#14
# asm 2: str <x6=r7,<x67_stack=[sp,#104]
str r7,[sp,#104]

# qhasm: 					         x4 ^= (s4 >>> 19)
# asm 1: eor >x4=int32#5,<x4=int32#5,<s4=int32#14,ROR #19
# asm 2: eor >x4=r4,<x4=r4,<s4=r14,ROR #19
eor r4,r4,r14,ROR #19

# qhasm: 					x8 = mem32[&x89_stack]
# asm 1: ldr >x8=int32#8,<x89_stack=stack64#15
# asm 2: ldr >x8=r7,<x89_stack=[sp,#112]
ldr r7,[sp,#112]

# qhasm: 					       s0 = x3 + x2
# asm 1: add >s0=int32#13,<x3=int32#4,<x2=int32#6
# asm 2: add >s0=r12,<x3=r3,<x2=r5
add r12,r3,r5

# qhasm: 					mem32[&x67_stack+4] = x7
# asm 1: str <x7=int32#7,<x67_stack=stack64#14
# asm 2: str <x7=r6,<x67_stack=[sp,#108]
str r6,[sp,#108]

# qhasm: 					         s5 = x4 + x7
# asm 1: add >s5=int32#7,<x4=int32#5,<x7=int32#7
# asm 2: add >s5=r6,<x4=r4,<x7=r6
add r6,r4,r6

# qhasm: 					x9 = mem32[&x89_stack+4]
# asm 1: ldr >x9=int32#14,<x89_stack=stack64#15
# asm 2: ldr >x9=r14,<x89_stack=[sp,#116]
ldr r14,[sp,#116]

# qhasm: 					       x0 ^= (s0 >>> 14)
# asm 1: eor >x0=int32#1,<x0=int32#1,<s0=int32#13,ROR #14
# asm 2: eor >x0=r0,<x0=r0,<s0=r12,ROR #14
eor r0,r0,r12,ROR #14

# qhasm: 					mem32[&x23_stack] = x2
# asm 1: str <x2=int32#6,<x23_stack=stack64#13
# asm 2: str <x2=r5,<x23_stack=[sp,#96]
str r5,[sp,#96]

# qhasm: 					         x5 ^= (s5 >>> 14)
# asm 1: eor >x5=int32#6,<x5=int32#3,<s5=int32#7,ROR #14
# asm 2: eor >x5=r5,<x5=r2,<s5=r6,ROR #14
eor r5,r2,r6,ROR #14

# qhasm: 					x12 = mem32[&x1213_stack]
# asm 1: ldr >x12=int32#3,<x1213_stack=stack64#16
# asm 2: ldr >x12=r2,<x1213_stack=[sp,#120]
ldr r2,[sp,#120]

# qhasm:     4x a1 unsigned>>= 23
# asm 1: vshr.u32 >a1=reg128#12,<a1=reg128#12,#23
# asm 2: vshr.u32 >a1=q11,<a1=q11,#23
vshr.u32 q11,q11,#23

# qhasm:     			4x next_a1 unsigned>>= 23
# asm 1: vshr.u32 >next_a1=reg128#13,<next_a1=reg128#13,#23
# asm 2: vshr.u32 >next_a1=q12,<next_a1=q12,#23
vshr.u32 q12,q12,#23

# qhasm:        diag2 ^= b1
# asm 1: veor >diag2=reg128#3,<diag2=reg128#3,<b1=reg128#14
# asm 2: veor >diag2=q2,<diag2=q2,<b1=q13
veor q2,q2,q13

# qhasm:        			next_diag2 ^= next_b1
# asm 1: veor >next_diag2=reg128#7,<next_diag2=reg128#7,<next_b1=reg128#15
# asm 2: veor >next_diag2=q6,<next_diag2=q6,<next_b1=q14
veor q6,q6,q14

# qhasm:        diag2 ^= a1
# asm 1: veor >diag2=reg128#3,<diag2=reg128#3,<a1=reg128#12
# asm 2: veor >diag2=q2,<diag2=q2,<a1=q11
veor q2,q2,q11

# qhasm:        			next_diag2 ^= next_a1
# asm 1: veor >next_diag2=reg128#7,<next_diag2=reg128#7,<next_a1=reg128#13
# asm 2: veor >next_diag2=q6,<next_diag2=q6,<next_a1=q12
veor q6,q6,q12

# qhasm: 					          s11 = x10 + x9
# asm 1: add >s11=int32#7,<x10=int32#11,<x9=int32#14
# asm 2: add >s11=r6,<x10=r10,<x9=r14
add r6,r10,r14

# qhasm: 					            s12 = x15 + x14
# asm 1: add >s12=int32#13,<x15=int32#10,<x14=int32#9
# asm 2: add >s12=r12,<x15=r9,<x14=r8
add r12,r9,r8

# qhasm:     4x a2 = diag1 + diag2
# asm 1: vadd.i32 >a2=reg128#12,<diag1=reg128#8,<diag2=reg128#3
# asm 2: vadd.i32 >a2=q11,<diag1=q7,<diag2=q2
vadd.i32 q11,q7,q2

# qhasm:             diag1 = diag1[3] diag1[0,1,2]
# asm 1: vext.32 >diag1=reg128#8,<diag1=reg128#8,<diag1=reg128#8,#3
# asm 2: vext.32 >diag1=q7,<diag1=q7,<diag1=q7,#3
vext.32 q7,q7,q7,#3

# qhasm:     			4x next_a2 = next_diag1 + next_diag2
# asm 1: vadd.i32 >next_a2=reg128#13,<next_diag1=reg128#10,<next_diag2=reg128#7
# asm 2: vadd.i32 >next_a2=q12,<next_diag1=q9,<next_diag2=q6
vadd.i32 q12,q9,q6

# qhasm:     4x b2 = a2 << 13
# asm 1: vshl.i32 >b2=reg128#14,<a2=reg128#12,#13
# asm 2: vshl.i32 >b2=q13,<a2=q11,#13
vshl.i32 q13,q11,#13

# qhasm:        				next_diag1 = next_diag1[3] next_diag1[0,1,2]
# asm 1: vext.32 >next_diag1=reg128#10,<next_diag1=reg128#10,<next_diag1=reg128#10,#3
# asm 2: vext.32 >next_diag1=q9,<next_diag1=q9,<next_diag1=q9,#3
vext.32 q9,q9,q9,#3

# qhasm:     			4x next_b2 = next_a2 << 13
# asm 1: vshl.i32 >next_b2=reg128#15,<next_a2=reg128#13,#13
# asm 2: vshl.i32 >next_b2=q14,<next_a2=q12,#13
vshl.i32 q14,q12,#13

# qhasm:     4x a2 unsigned>>= 19
# asm 1: vshr.u32 >a2=reg128#12,<a2=reg128#12,#19
# asm 2: vshr.u32 >a2=q11,<a2=q11,#19
vshr.u32 q11,q11,#19

# qhasm:     			4x next_a2 unsigned>>= 19
# asm 1: vshr.u32 >next_a2=reg128#13,<next_a2=reg128#13,#19
# asm 2: vshr.u32 >next_a2=q12,<next_a2=q12,#19
vshr.u32 q12,q12,#19

# qhasm: 					          x11 ^= (s11 >>> 25)
# asm 1: eor >x11=int32#12,<x11=int32#12,<s11=int32#7,ROR #25
# asm 2: eor >x11=r11,<x11=r11,<s11=r6,ROR #25
eor r11,r11,r6,ROR #25

# qhasm: 					            x12 ^= (s12 >>> 25)
# asm 1: eor >x12=int32#3,<x12=int32#3,<s12=int32#13,ROR #25
# asm 2: eor >x12=r2,<x12=r2,<s12=r12,ROR #25
eor r2,r2,r12,ROR #25

# qhasm: 					           s8 = x11 + x10
# asm 1: add >s8=int32#7,<x11=int32#12,<x10=int32#11
# asm 2: add >s8=r6,<x11=r11,<x10=r10
add r6,r11,r10

# qhasm: 					mem32[&x23_stack+4] = x3
# asm 1: str <x3=int32#4,<x23_stack=stack64#13
# asm 2: str <x3=r3,<x23_stack=[sp,#100]
str r3,[sp,#100]

# qhasm: 					            s13 = x12 + x15
# asm 1: add >s13=int32#4,<x12=int32#3,<x15=int32#10
# asm 2: add >s13=r3,<x12=r2,<x15=r9
add r3,r2,r9

# qhasm: 					x13 = mem32[&x1213_stack+4]
# asm 1: ldr >x13=int32#13,<x1213_stack=stack64#16
# asm 2: ldr >x13=r12,<x1213_stack=[sp,#124]
ldr r12,[sp,#124]

# qhasm:        diag3 ^= b2
# asm 1: veor >diag3=reg128#5,<diag3=reg128#5,<b2=reg128#14
# asm 2: veor >diag3=q4,<diag3=q4,<b2=q13
veor q4,q4,q13

# qhasm:        			next_diag3 ^= next_b2
# asm 1: veor >next_diag3=reg128#6,<next_diag3=reg128#6,<next_b2=reg128#15
# asm 2: veor >next_diag3=q5,<next_diag3=q5,<next_b2=q14
veor q5,q5,q14

# qhasm:        diag3 ^= a2
# asm 1: veor >diag3=reg128#5,<diag3=reg128#5,<a2=reg128#12
# asm 2: veor >diag3=q4,<diag3=q4,<a2=q11
veor q4,q4,q11

# qhasm:        			next_diag3 ^= next_a2
# asm 1: veor >next_diag3=reg128#6,<next_diag3=reg128#6,<next_a2=reg128#13
# asm 2: veor >next_diag3=q5,<next_diag3=q5,<next_a2=q12
veor q5,q5,q12

# qhasm: 					           x8 ^= (s8 >>> 23)
# asm 1: eor >x8=int32#7,<x8=int32#8,<s8=int32#7,ROR #23
# asm 2: eor >x8=r6,<x8=r7,<s8=r6,ROR #23
eor r6,r7,r6,ROR #23

# qhasm: 					            x13 ^= (s13 >>> 23)
# asm 1: eor >x13=int32#4,<x13=int32#13,<s13=int32#4,ROR #23
# asm 2: eor >x13=r3,<x13=r12,<s13=r3,ROR #23
eor r3,r12,r3,ROR #23

# qhasm: 					           s9 = x8 + x11
# asm 1: add >s9=int32#8,<x8=int32#7,<x11=int32#12
# asm 2: add >s9=r7,<x8=r6,<x11=r11
add r7,r6,r11

# qhasm: 					            s14 = x13 + x12
# asm 1: add >s14=int32#13,<x13=int32#4,<x12=int32#3
# asm 2: add >s14=r12,<x13=r3,<x12=r2
add r12,r3,r2

# qhasm:     4x a3 = diag2 + diag3
# asm 1: vadd.i32 >a3=reg128#12,<diag2=reg128#3,<diag3=reg128#5
# asm 2: vadd.i32 >a3=q11,<diag2=q2,<diag3=q4
vadd.i32 q11,q2,q4

# qhasm:             diag2 = diag2[2,3] diag2[0,1]
# asm 1: vswp <diag2=reg128#3%bot,<diag2=reg128#3%top
# asm 2: vswp <diag2=d4,<diag2=d5
vswp d4,d5

# qhasm:     			4x next_a3 = next_diag2 + next_diag3
# asm 1: vadd.i32 >next_a3=reg128#13,<next_diag2=reg128#7,<next_diag3=reg128#6
# asm 2: vadd.i32 >next_a3=q12,<next_diag2=q6,<next_diag3=q5
vadd.i32 q12,q6,q5

# qhasm:     4x b3 = a3 << 18
# asm 1: vshl.i32 >b3=reg128#14,<a3=reg128#12,#18
# asm 2: vshl.i32 >b3=q13,<a3=q11,#18
vshl.i32 q13,q11,#18

# qhasm:        				next_diag2 = next_diag2[2,3] next_diag2[0,1]
# asm 1: vswp <next_diag2=reg128#7%bot,<next_diag2=reg128#7%top
# asm 2: vswp <next_diag2=d12,<next_diag2=d13
vswp d12,d13

# qhasm:     			4x next_b3 = next_a3 << 18
# asm 1: vshl.i32 >next_b3=reg128#15,<next_a3=reg128#13,#18
# asm 2: vshl.i32 >next_b3=q14,<next_a3=q12,#18
vshl.i32 q14,q12,#18

# qhasm: 					           x9 ^= (s9 >>> 19)
# asm 1: eor >x9=int32#8,<x9=int32#14,<s9=int32#8,ROR #19
# asm 2: eor >x9=r7,<x9=r14,<s9=r7,ROR #19
eor r7,r14,r7,ROR #19

# qhasm: 					            x14 ^= (s14 >>> 19)
# asm 1: eor >x14=int32#9,<x14=int32#9,<s14=int32#13,ROR #19
# asm 2: eor >x14=r8,<x14=r8,<s14=r12,ROR #19
eor r8,r8,r12,ROR #19

# qhasm: 					          s10 = x9 + x8
# asm 1: add >s10=int32#13,<x9=int32#8,<x8=int32#7
# asm 2: add >s10=r12,<x9=r7,<x8=r6
add r12,r7,r6

# qhasm: 					            s15 = x14 + x13
# asm 1: add >s15=int32#14,<x14=int32#9,<x13=int32#4
# asm 2: add >s15=r14,<x14=r8,<x13=r3
add r14,r8,r3

# qhasm:     4x a3 unsigned>>= 14
# asm 1: vshr.u32 >a3=reg128#12,<a3=reg128#12,#14
# asm 2: vshr.u32 >a3=q11,<a3=q11,#14
vshr.u32 q11,q11,#14

# qhasm:             diag3 = diag3[1,2,3] diag3[0]
# asm 1: vext.32 >diag3=reg128#5,<diag3=reg128#5,<diag3=reg128#5,#1
# asm 2: vext.32 >diag3=q4,<diag3=q4,<diag3=q4,#1
vext.32 q4,q4,q4,#1

# qhasm:     			4x next_a3 unsigned>>= 14
# asm 1: vshr.u32 >next_a3=reg128#13,<next_a3=reg128#13,#14
# asm 2: vshr.u32 >next_a3=q12,<next_a3=q12,#14
vshr.u32 q12,q12,#14

# qhasm:        diag0 ^= b3
# asm 1: veor >diag0=reg128#9,<diag0=reg128#9,<b3=reg128#14
# asm 2: veor >diag0=q8,<diag0=q8,<b3=q13
veor q8,q8,q13

# qhasm:        				next_diag3 = next_diag3[1,2,3] next_diag3[0]
# asm 1: vext.32 >next_diag3=reg128#6,<next_diag3=reg128#6,<next_diag3=reg128#6,#1
# asm 2: vext.32 >next_diag3=q5,<next_diag3=q5,<next_diag3=q5,#1
vext.32 q5,q5,q5,#1

# qhasm:        			next_diag0 ^= next_b3
# asm 1: veor >next_diag0=reg128#11,<next_diag0=reg128#11,<next_b3=reg128#15
# asm 2: veor >next_diag0=q10,<next_diag0=q10,<next_b3=q14
veor q10,q10,q14

# qhasm: 					          x10 ^= (s10 >>> 14)
# asm 1: eor >x10=int32#11,<x10=int32#11,<s10=int32#13,ROR #14
# asm 2: eor >x10=r10,<x10=r10,<s10=r12,ROR #14
eor r10,r10,r12,ROR #14

# qhasm:        diag0 ^= a3
# asm 1: veor >diag0=reg128#9,<diag0=reg128#9,<a3=reg128#12
# asm 2: veor >diag0=q8,<diag0=q8,<a3=q11
veor q8,q8,q11

# qhasm: 					            x15 ^= (s15 >>> 14)
# asm 1: eor >x15=int32#10,<x15=int32#10,<s15=int32#14,ROR #14
# asm 2: eor >x15=r9,<x15=r9,<s15=r14,ROR #14
eor r9,r9,r14,ROR #14

# qhasm:        			next_diag0 ^= next_a3
# asm 1: veor >next_diag0=reg128#11,<next_diag0=reg128#11,<next_a3=reg128#13
# asm 2: veor >next_diag0=q10,<next_diag0=q10,<next_a3=q12
veor q10,q10,q12

# qhasm: assign r0 to x0

# qhasm: assign r1 to x1

# qhasm: assign r2 to x12

# qhasm: assign r3 to x13

# qhasm: assign r4 to x4

# qhasm: assign r5 to x5

# qhasm: assign r6 to x8

# qhasm: assign r7 to x9

# qhasm: assign r8 to x14

# qhasm: assign r9 to x15

# qhasm: assign r10 to x10

# qhasm: assign r11 to x11

# qhasm:     4x a0 = diag1 + diag0
# asm 1: vadd.i32 >a0=reg128#12,<diag1=reg128#8,<diag0=reg128#9
# asm 2: vadd.i32 >a0=q11,<diag1=q7,<diag0=q8
vadd.i32 q11,q7,q8

# qhasm:    			4x next_a0 = next_diag1 + next_diag0
# asm 1: vadd.i32 >next_a0=reg128#13,<next_diag1=reg128#10,<next_diag0=reg128#11
# asm 2: vadd.i32 >next_a0=q12,<next_diag1=q9,<next_diag0=q10
vadd.i32 q12,q9,q10

# qhasm: 					       s4 = x0 + x12
# asm 1: add >s4=int32#13,<x0=int32#1,<x12=int32#3
# asm 2: add >s4=r12,<x0=r0,<x12=r2
add r12,r0,r2

# qhasm: 					         s9 = x5 + x1
# asm 1: add >s9=int32#14,<x5=int32#6,<x1=int32#2
# asm 2: add >s9=r14,<x5=r5,<x1=r1
add r14,r5,r1

# qhasm:     4x b0 = a0 << 7
# asm 1: vshl.i32 >b0=reg128#14,<a0=reg128#12,#7
# asm 2: vshl.i32 >b0=q13,<a0=q11,#7
vshl.i32 q13,q11,#7

# qhasm:     			4x next_b0 = next_a0 << 7
# asm 1: vshl.i32 >next_b0=reg128#15,<next_a0=reg128#13,#7
# asm 2: vshl.i32 >next_b0=q14,<next_a0=q12,#7
vshl.i32 q14,q12,#7

# qhasm:     4x a0 unsigned>>= 25
# asm 1: vshr.u32 >a0=reg128#12,<a0=reg128#12,#25
# asm 2: vshr.u32 >a0=q11,<a0=q11,#25
vshr.u32 q11,q11,#25

# qhasm:     			4x next_a0 unsigned>>= 25
# asm 1: vshr.u32 >next_a0=reg128#13,<next_a0=reg128#13,#25
# asm 2: vshr.u32 >next_a0=q12,<next_a0=q12,#25
vshr.u32 q12,q12,#25

# qhasm: 					       x4 ^= (s4 >>> 25)
# asm 1: eor >x4=int32#5,<x4=int32#5,<s4=int32#13,ROR #25
# asm 2: eor >x4=r4,<x4=r4,<s4=r12,ROR #25
eor r4,r4,r12,ROR #25

# qhasm: 					         x9 ^= (s9 >>> 25)
# asm 1: eor >x9=int32#8,<x9=int32#8,<s9=int32#14,ROR #25
# asm 2: eor >x9=r7,<x9=r7,<s9=r14,ROR #25
eor r7,r7,r14,ROR #25

# qhasm: 					       s8 = x4 + x0
# asm 1: add >s8=int32#13,<x4=int32#5,<x0=int32#1
# asm 2: add >s8=r12,<x4=r4,<x0=r0
add r12,r4,r0

# qhasm: 					         s13 = x9 + x5
# asm 1: add >s13=int32#14,<x9=int32#8,<x5=int32#6
# asm 2: add >s13=r14,<x9=r7,<x5=r5
add r14,r7,r5

# qhasm:        diag3 ^= b0
# asm 1: veor >diag3=reg128#5,<diag3=reg128#5,<b0=reg128#14
# asm 2: veor >diag3=q4,<diag3=q4,<b0=q13
veor q4,q4,q13

# qhasm:        			next_diag3 ^= next_b0
# asm 1: veor >next_diag3=reg128#6,<next_diag3=reg128#6,<next_b0=reg128#15
# asm 2: veor >next_diag3=q5,<next_diag3=q5,<next_b0=q14
veor q5,q5,q14

# qhasm:        diag3 ^= a0
# asm 1: veor >diag3=reg128#5,<diag3=reg128#5,<a0=reg128#12
# asm 2: veor >diag3=q4,<diag3=q4,<a0=q11
veor q4,q4,q11

# qhasm:        			next_diag3 ^= next_a0
# asm 1: veor >next_diag3=reg128#6,<next_diag3=reg128#6,<next_a0=reg128#13
# asm 2: veor >next_diag3=q5,<next_diag3=q5,<next_a0=q12
veor q5,q5,q12

# qhasm: 					       x8 ^= (s8 >>> 23)
# asm 1: eor >x8=int32#7,<x8=int32#7,<s8=int32#13,ROR #23
# asm 2: eor >x8=r6,<x8=r6,<s8=r12,ROR #23
eor r6,r6,r12,ROR #23

# qhasm: 					        x13 ^= (s13 >>> 23)
# asm 1: eor >x13=int32#4,<x13=int32#4,<s13=int32#14,ROR #23
# asm 2: eor >x13=r3,<x13=r3,<s13=r14,ROR #23
eor r3,r3,r14,ROR #23

# qhasm: 					      s12 = x8 + x4
# asm 1: add >s12=int32#13,<x8=int32#7,<x4=int32#5
# asm 2: add >s12=r12,<x8=r6,<x4=r4
add r12,r6,r4

# qhasm: 					mem32[&x89_stack+4] = x9
# asm 1: str <x9=int32#8,<x89_stack=stack64#15
# asm 2: str <x9=r7,<x89_stack=[sp,#116]
str r7,[sp,#116]

# qhasm: 					         s1 = x13 + x9
# asm 1: add >s1=int32#8,<x13=int32#4,<x9=int32#8
# asm 2: add >s1=r7,<x13=r3,<x9=r7
add r7,r3,r7

# qhasm: 					x7 = mem32[&x67_stack+4]
# asm 1: ldr >x7=int32#14,<x67_stack=stack64#14
# asm 2: ldr >x7=r14,<x67_stack=[sp,#108]
ldr r14,[sp,#108]

# qhasm:     4x a1 = diag0 + diag3
# asm 1: vadd.i32 >a1=reg128#12,<diag0=reg128#9,<diag3=reg128#5
# asm 2: vadd.i32 >a1=q11,<diag0=q8,<diag3=q4
vadd.i32 q11,q8,q4

# qhasm:     			4x next_a1 = next_diag0 + next_diag3
# asm 1: vadd.i32 >next_a1=reg128#13,<next_diag0=reg128#11,<next_diag3=reg128#6
# asm 2: vadd.i32 >next_a1=q12,<next_diag0=q10,<next_diag3=q5
vadd.i32 q12,q10,q5

# qhasm:     4x b1 = a1 << 9
# asm 1: vshl.i32 >b1=reg128#14,<a1=reg128#12,#9
# asm 2: vshl.i32 >b1=q13,<a1=q11,#9
vshl.i32 q13,q11,#9

# qhasm:     			4x next_b1 = next_a1 << 9
# asm 1: vshl.i32 >next_b1=reg128#15,<next_a1=reg128#13,#9
# asm 2: vshl.i32 >next_b1=q14,<next_a1=q12,#9
vshl.i32 q14,q12,#9

# qhasm:     4x a1 unsigned>>= 23
# asm 1: vshr.u32 >a1=reg128#12,<a1=reg128#12,#23
# asm 2: vshr.u32 >a1=q11,<a1=q11,#23
vshr.u32 q11,q11,#23

# qhasm:     			4x next_a1 unsigned>>= 23
# asm 1: vshr.u32 >next_a1=reg128#13,<next_a1=reg128#13,#23
# asm 2: vshr.u32 >next_a1=q12,<next_a1=q12,#23
vshr.u32 q12,q12,#23

# qhasm:        diag2 ^= b1
# asm 1: veor >diag2=reg128#3,<diag2=reg128#3,<b1=reg128#14
# asm 2: veor >diag2=q2,<diag2=q2,<b1=q13
veor q2,q2,q13

# qhasm:        			next_diag2 ^= next_b1
# asm 1: veor >next_diag2=reg128#7,<next_diag2=reg128#7,<next_b1=reg128#15
# asm 2: veor >next_diag2=q6,<next_diag2=q6,<next_b1=q14
veor q6,q6,q14

# qhasm:        diag2 ^= a1
# asm 1: veor >diag2=reg128#3,<diag2=reg128#3,<a1=reg128#12
# asm 2: veor >diag2=q2,<diag2=q2,<a1=q11
veor q2,q2,q11

# qhasm:        			next_diag2 ^= next_a1
# asm 1: veor >next_diag2=reg128#7,<next_diag2=reg128#7,<next_a1=reg128#13
# asm 2: veor >next_diag2=q6,<next_diag2=q6,<next_a1=q12
veor q6,q6,q12

# qhasm: 					      x12 ^= (s12 >>> 19)
# asm 1: eor >x12=int32#3,<x12=int32#3,<s12=int32#13,ROR #19
# asm 2: eor >x12=r2,<x12=r2,<s12=r12,ROR #19
eor r2,r2,r12,ROR #19

# qhasm: 					mem32[&x1213_stack] = x12
# asm 1: str <x12=int32#3,<x1213_stack=stack64#16
# asm 2: str <x12=r2,<x1213_stack=[sp,#120]
str r2,[sp,#120]

# qhasm: 					         x1 ^= (s1 >>> 19)
# asm 1: eor >x1=int32#2,<x1=int32#2,<s1=int32#8,ROR #19
# asm 2: eor >x1=r1,<x1=r1,<s1=r7,ROR #19
eor r1,r1,r7,ROR #19

# qhasm: 					x2 = mem32[&x23_stack]
# asm 1: ldr >x2=int32#8,<x23_stack=stack64#13
# asm 2: ldr >x2=r7,<x23_stack=[sp,#96]
ldr r7,[sp,#96]

# qhasm: 					       s0 = x12 + x8
# asm 1: add >s0=int32#3,<x12=int32#3,<x8=int32#7
# asm 2: add >s0=r2,<x12=r2,<x8=r6
add r2,r2,r6

# qhasm: 					mem32[&x89_stack] = x8
# asm 1: str <x8=int32#7,<x89_stack=stack64#15
# asm 2: str <x8=r6,<x89_stack=[sp,#112]
str r6,[sp,#112]

# qhasm: 					         s5 = x1 + x13
# asm 1: add >s5=int32#7,<x1=int32#2,<x13=int32#4
# asm 2: add >s5=r6,<x1=r1,<x13=r3
add r6,r1,r3

# qhasm: 					x6 = mem32[&x67_stack]
# asm 1: ldr >x6=int32#13,<x67_stack=stack64#14
# asm 2: ldr >x6=r12,<x67_stack=[sp,#104]
ldr r12,[sp,#104]

# qhasm:     4x a2 = diag3 + diag2
# asm 1: vadd.i32 >a2=reg128#12,<diag3=reg128#5,<diag2=reg128#3
# asm 2: vadd.i32 >a2=q11,<diag3=q4,<diag2=q2
vadd.i32 q11,q4,q2

# qhasm:             diag3 = diag3[3] diag3[0,1,2]
# asm 1: vext.32 >diag3=reg128#5,<diag3=reg128#5,<diag3=reg128#5,#3
# asm 2: vext.32 >diag3=q4,<diag3=q4,<diag3=q4,#3
vext.32 q4,q4,q4,#3

# qhasm:     			4x next_a2 = next_diag3 + next_diag2
# asm 1: vadd.i32 >next_a2=reg128#13,<next_diag3=reg128#6,<next_diag2=reg128#7
# asm 2: vadd.i32 >next_a2=q12,<next_diag3=q5,<next_diag2=q6
vadd.i32 q12,q5,q6

# qhasm:     4x b2 = a2 << 13
# asm 1: vshl.i32 >b2=reg128#14,<a2=reg128#12,#13
# asm 2: vshl.i32 >b2=q13,<a2=q11,#13
vshl.i32 q13,q11,#13

# qhasm:        				next_diag3 = next_diag3[3] next_diag3[0,1,2]
# asm 1: vext.32 >next_diag3=reg128#6,<next_diag3=reg128#6,<next_diag3=reg128#6,#3
# asm 2: vext.32 >next_diag3=q5,<next_diag3=q5,<next_diag3=q5,#3
vext.32 q5,q5,q5,#3

# qhasm:     			4x next_b2 = next_a2 << 13
# asm 1: vshl.i32 >next_b2=reg128#15,<next_a2=reg128#13,#13
# asm 2: vshl.i32 >next_b2=q14,<next_a2=q12,#13
vshl.i32 q14,q12,#13

# qhasm: 					       x0 ^= (s0 >>> 14)
# asm 1: eor >x0=int32#1,<x0=int32#1,<s0=int32#3,ROR #14
# asm 2: eor >x0=r0,<x0=r0,<s0=r2,ROR #14
eor r0,r0,r2,ROR #14

# qhasm: 					         x5 ^= (s5 >>> 14)
# asm 1: eor >x5=int32#3,<x5=int32#6,<s5=int32#7,ROR #14
# asm 2: eor >x5=r2,<x5=r5,<s5=r6,ROR #14
eor r2,r5,r6,ROR #14

# qhasm: 					mem32[&x1213_stack+4] = x13
# asm 1: str <x13=int32#4,<x1213_stack=stack64#16
# asm 2: str <x13=r3,<x1213_stack=[sp,#124]
str r3,[sp,#124]

# qhasm: 					          s14 = x10 + x6
# asm 1: add >s14=int32#4,<x10=int32#11,<x6=int32#13
# asm 2: add >s14=r3,<x10=r10,<x6=r12
add r3,r10,r12

# qhasm: 					x3 = mem32[&x23_stack+4]
# asm 1: ldr >x3=int32#6,<x23_stack=stack64#13
# asm 2: ldr >x3=r5,<x23_stack=[sp,#100]
ldr r5,[sp,#100]

# qhasm: 					             s3 = x15 + x11
# asm 1: add >s3=int32#7,<x15=int32#10,<x11=int32#12
# asm 2: add >s3=r6,<x15=r9,<x11=r11
add r6,r9,r11

# qhasm:     4x a2 unsigned>>= 19
# asm 1: vshr.u32 >a2=reg128#12,<a2=reg128#12,#19
# asm 2: vshr.u32 >a2=q11,<a2=q11,#19
vshr.u32 q11,q11,#19

# qhasm:     			4x next_a2 unsigned>>= 19
# asm 1: vshr.u32 >next_a2=reg128#13,<next_a2=reg128#13,#19
# asm 2: vshr.u32 >next_a2=q12,<next_a2=q12,#19
vshr.u32 q12,q12,#19

# qhasm:        diag1 ^= b2
# asm 1: veor >diag1=reg128#8,<diag1=reg128#8,<b2=reg128#14
# asm 2: veor >diag1=q7,<diag1=q7,<b2=q13
veor q7,q7,q13

# qhasm:        			next_diag1 ^= next_b2
# asm 1: veor >next_diag1=reg128#10,<next_diag1=reg128#10,<next_b2=reg128#15
# asm 2: veor >next_diag1=q9,<next_diag1=q9,<next_b2=q14
veor q9,q9,q14

# qhasm: 					          x14 ^= (s14 >>> 25)
# asm 1: eor >x14=int32#9,<x14=int32#9,<s14=int32#4,ROR #25
# asm 2: eor >x14=r8,<x14=r8,<s14=r3,ROR #25
eor r8,r8,r3,ROR #25

# qhasm: 					             x3 ^= (s3 >>> 25)
# asm 1: eor >x3=int32#4,<x3=int32#6,<s3=int32#7,ROR #25
# asm 2: eor >x3=r3,<x3=r5,<s3=r6,ROR #25
eor r3,r5,r6,ROR #25

# qhasm: 					           s2 = x14 + x10
# asm 1: add >s2=int32#6,<x14=int32#9,<x10=int32#11
# asm 2: add >s2=r5,<x14=r8,<x10=r10
add r5,r8,r10

# qhasm: 					             s7 = x3 + x15
# asm 1: add >s7=int32#7,<x3=int32#4,<x15=int32#10
# asm 2: add >s7=r6,<x3=r3,<x15=r9
add r6,r3,r9

# qhasm:        diag1 ^= a2
# asm 1: veor >diag1=reg128#8,<diag1=reg128#8,<a2=reg128#12
# asm 2: veor >diag1=q7,<diag1=q7,<a2=q11
veor q7,q7,q11

# qhasm:        			next_diag1 ^= next_a2
# asm 1: veor >next_diag1=reg128#10,<next_diag1=reg128#10,<next_a2=reg128#13
# asm 2: veor >next_diag1=q9,<next_diag1=q9,<next_a2=q12
veor q9,q9,q12

# qhasm: 					           x2 ^= (s2 >>> 23)
# asm 1: eor >x2=int32#6,<x2=int32#8,<s2=int32#6,ROR #23
# asm 2: eor >x2=r5,<x2=r7,<s2=r5,ROR #23
eor r5,r7,r5,ROR #23

# qhasm: 					             x7 ^= (s7 >>> 23)
# asm 1: eor >x7=int32#7,<x7=int32#14,<s7=int32#7,ROR #23
# asm 2: eor >x7=r6,<x7=r14,<s7=r6,ROR #23
eor r6,r14,r6,ROR #23

# qhasm: 					           s6 = x2 + x14
# asm 1: add >s6=int32#8,<x2=int32#6,<x14=int32#9
# asm 2: add >s6=r7,<x2=r5,<x14=r8
add r7,r5,r8

# qhasm: 					            s11 = x7 + x3
# asm 1: add >s11=int32#14,<x7=int32#7,<x3=int32#4
# asm 2: add >s11=r14,<x7=r6,<x3=r3
add r14,r6,r3

# qhasm:     4x a3 = diag2 + diag1
# asm 1: vadd.i32 >a3=reg128#12,<diag2=reg128#3,<diag1=reg128#8
# asm 2: vadd.i32 >a3=q11,<diag2=q2,<diag1=q7
vadd.i32 q11,q2,q7

# qhasm:             diag2 = diag2[2,3] diag2[0,1]
# asm 1: vswp <diag2=reg128#3%bot,<diag2=reg128#3%top
# asm 2: vswp <diag2=d4,<diag2=d5
vswp d4,d5

# qhasm:     			4x next_a3 = next_diag2 + next_diag1
# asm 1: vadd.i32 >next_a3=reg128#13,<next_diag2=reg128#7,<next_diag1=reg128#10
# asm 2: vadd.i32 >next_a3=q12,<next_diag2=q6,<next_diag1=q9
vadd.i32 q12,q6,q9

# qhasm:     4x b3 = a3 << 18
# asm 1: vshl.i32 >b3=reg128#14,<a3=reg128#12,#18
# asm 2: vshl.i32 >b3=q13,<a3=q11,#18
vshl.i32 q13,q11,#18

# qhasm:        				next_diag2 = next_diag2[2,3] next_diag2[0,1]
# asm 1: vswp <next_diag2=reg128#7%bot,<next_diag2=reg128#7%top
# asm 2: vswp <next_diag2=d12,<next_diag2=d13
vswp d12,d13

# qhasm:     			4x next_b3 = next_a3 << 18
# asm 1: vshl.i32 >next_b3=reg128#15,<next_a3=reg128#13,#18
# asm 2: vshl.i32 >next_b3=q14,<next_a3=q12,#18
vshl.i32 q14,q12,#18

# qhasm: 					           x6 ^= (s6 >>> 19)
# asm 1: eor >x6=int32#8,<x6=int32#13,<s6=int32#8,ROR #19
# asm 2: eor >x6=r7,<x6=r12,<s6=r7,ROR #19
eor r7,r12,r7,ROR #19

# qhasm: 					            x11 ^= (s11 >>> 19)
# asm 1: eor >x11=int32#12,<x11=int32#12,<s11=int32#14,ROR #19
# asm 2: eor >x11=r11,<x11=r11,<s11=r14,ROR #19
eor r11,r11,r14,ROR #19

# qhasm: 					          s10 = x6 + x2
# asm 1: add >s10=int32#13,<x6=int32#8,<x2=int32#6
# asm 2: add >s10=r12,<x6=r7,<x2=r5
add r12,r7,r5

# qhasm: 					            s15 = x11 + x7
# asm 1: add >s15=int32#14,<x11=int32#12,<x7=int32#7
# asm 2: add >s15=r14,<x11=r11,<x7=r6
add r14,r11,r6

# qhasm:     4x a3 unsigned>>= 14
# asm 1: vshr.u32 >a3=reg128#12,<a3=reg128#12,#14
# asm 2: vshr.u32 >a3=q11,<a3=q11,#14
vshr.u32 q11,q11,#14

# qhasm:             diag1 = diag1[1,2,3] diag1[0]
# asm 1: vext.32 >diag1=reg128#8,<diag1=reg128#8,<diag1=reg128#8,#1
# asm 2: vext.32 >diag1=q7,<diag1=q7,<diag1=q7,#1
vext.32 q7,q7,q7,#1

# qhasm:     			4x next_a3 unsigned>>= 14
# asm 1: vshr.u32 >next_a3=reg128#13,<next_a3=reg128#13,#14
# asm 2: vshr.u32 >next_a3=q12,<next_a3=q12,#14
vshr.u32 q12,q12,#14

# qhasm:        diag0 ^= b3
# asm 1: veor >diag0=reg128#9,<diag0=reg128#9,<b3=reg128#14
# asm 2: veor >diag0=q8,<diag0=q8,<b3=q13
veor q8,q8,q13

# qhasm:        				next_diag1 = next_diag1[1,2,3] next_diag1[0]
# asm 1: vext.32 >next_diag1=reg128#10,<next_diag1=reg128#10,<next_diag1=reg128#10,#1
# asm 2: vext.32 >next_diag1=q9,<next_diag1=q9,<next_diag1=q9,#1
vext.32 q9,q9,q9,#1

# qhasm:        			next_diag0 ^= next_b3
# asm 1: veor >next_diag0=reg128#11,<next_diag0=reg128#11,<next_b3=reg128#15
# asm 2: veor >next_diag0=q10,<next_diag0=q10,<next_b3=q14
veor q10,q10,q14

# qhasm: 					          x10 ^= (s10 >>> 14)
# asm 1: eor >x10=int32#11,<x10=int32#11,<s10=int32#13,ROR #14
# asm 2: eor >x10=r10,<x10=r10,<s10=r12,ROR #14
eor r10,r10,r12,ROR #14

# qhasm: 					            x15 ^= (s15 >>> 14)
# asm 1: eor >x15=int32#10,<x15=int32#10,<s15=int32#14,ROR #14
# asm 2: eor >x15=r9,<x15=r9,<s15=r14,ROR #14
eor r9,r9,r14,ROR #14

# qhasm: 					       s1 = x0 + x3
# asm 1: add >s1=int32#13,<x0=int32#1,<x3=int32#4
# asm 2: add >s1=r12,<x0=r0,<x3=r3
add r12,r0,r3

# qhasm: 					         s6 = x5 + x4
# asm 1: add >s6=int32#14,<x5=int32#3,<x4=int32#5
# asm 2: add >s6=r14,<x5=r2,<x4=r4
add r14,r2,r4

# qhasm:        diag0 ^= a3
# asm 1: veor >diag0=reg128#9,<diag0=reg128#9,<a3=reg128#12
# asm 2: veor >diag0=q8,<diag0=q8,<a3=q11
veor q8,q8,q11

# qhasm:        			next_diag0 ^= next_a3
# asm 1: veor >next_diag0=reg128#12,<next_diag0=reg128#11,<next_a3=reg128#13
# asm 2: veor >next_diag0=q11,<next_diag0=q10,<next_a3=q12
veor q11,q10,q12

# qhasm: 					       x1 ^= (s1 >>> 25)
# asm 1: eor >x1=int32#2,<x1=int32#2,<s1=int32#13,ROR #25
# asm 2: eor >x1=r1,<x1=r1,<s1=r12,ROR #25
eor r1,r1,r12,ROR #25

# qhasm: 					         x6 ^= (s6 >>> 25)
# asm 1: eor >x6=int32#8,<x6=int32#8,<s6=int32#14,ROR #25
# asm 2: eor >x6=r7,<x6=r7,<s6=r14,ROR #25
eor r7,r7,r14,ROR #25

# qhasm: 					       s2 = x1 + x0
# asm 1: add >s2=int32#13,<x1=int32#2,<x0=int32#1
# asm 2: add >s2=r12,<x1=r1,<x0=r0
add r12,r1,r0

# qhasm: 					         s7 = x6 + x5
# asm 1: add >s7=int32#14,<x6=int32#8,<x5=int32#3
# asm 2: add >s7=r14,<x6=r7,<x5=r2
add r14,r7,r2

# qhasm:     4x a0 = diag3 + diag0
# asm 1: vadd.i32 >a0=reg128#11,<diag3=reg128#5,<diag0=reg128#9
# asm 2: vadd.i32 >a0=q10,<diag3=q4,<diag0=q8
vadd.i32 q10,q4,q8

# qhasm:     			4x next_a0 = next_diag3 + next_diag0
# asm 1: vadd.i32 >next_a0=reg128#13,<next_diag3=reg128#6,<next_diag0=reg128#12
# asm 2: vadd.i32 >next_a0=q12,<next_diag3=q5,<next_diag0=q11
vadd.i32 q12,q5,q11

# qhasm:     4x b0 = a0 << 7
# asm 1: vshl.i32 >b0=reg128#14,<a0=reg128#11,#7
# asm 2: vshl.i32 >b0=q13,<a0=q10,#7
vshl.i32 q13,q10,#7

# qhasm:     			4x next_b0 = next_a0 << 7
# asm 1: vshl.i32 >next_b0=reg128#15,<next_a0=reg128#13,#7
# asm 2: vshl.i32 >next_b0=q14,<next_a0=q12,#7
vshl.i32 q14,q12,#7

# qhasm: 					       x2 ^= (s2 >>> 23)
# asm 1: eor >x2=int32#6,<x2=int32#6,<s2=int32#13,ROR #23
# asm 2: eor >x2=r5,<x2=r5,<s2=r12,ROR #23
eor r5,r5,r12,ROR #23

# qhasm: 					         x7 ^= (s7 >>> 23)
# asm 1: eor >x7=int32#7,<x7=int32#7,<s7=int32#14,ROR #23
# asm 2: eor >x7=r6,<x7=r6,<s7=r14,ROR #23
eor r6,r6,r14,ROR #23

# qhasm:     4x a0 unsigned>>= 25
# asm 1: vshr.u32 >a0=reg128#11,<a0=reg128#11,#25
# asm 2: vshr.u32 >a0=q10,<a0=q10,#25
vshr.u32 q10,q10,#25

# qhasm:     			4x next_a0 unsigned>>= 25
# asm 1: vshr.u32 >next_a0=reg128#13,<next_a0=reg128#13,#25
# asm 2: vshr.u32 >next_a0=q12,<next_a0=q12,#25
vshr.u32 q12,q12,#25

# qhasm: 					       s3 = x2 + x1
# asm 1: add >s3=int32#13,<x2=int32#6,<x1=int32#2
# asm 2: add >s3=r12,<x2=r5,<x1=r1
add r12,r5,r1

# qhasm: 					         s4 = x7 + x6
# asm 1: add >s4=int32#14,<x7=int32#7,<x6=int32#8
# asm 2: add >s4=r14,<x7=r6,<x6=r7
add r14,r6,r7

# qhasm:        diag1 ^= b0
# asm 1: veor >diag1=reg128#8,<diag1=reg128#8,<b0=reg128#14
# asm 2: veor >diag1=q7,<diag1=q7,<b0=q13
veor q7,q7,q13

# qhasm:        			next_diag1 ^= next_b0
# asm 1: veor >next_diag1=reg128#10,<next_diag1=reg128#10,<next_b0=reg128#15
# asm 2: veor >next_diag1=q9,<next_diag1=q9,<next_b0=q14
veor q9,q9,q14

# qhasm:        diag1 ^= a0
# asm 1: veor >diag1=reg128#8,<diag1=reg128#8,<a0=reg128#11
# asm 2: veor >diag1=q7,<diag1=q7,<a0=q10
veor q7,q7,q10

# qhasm:        			next_diag1 ^= next_a0
# asm 1: veor >next_diag1=reg128#10,<next_diag1=reg128#10,<next_a0=reg128#13
# asm 2: veor >next_diag1=q9,<next_diag1=q9,<next_a0=q12
veor q9,q9,q12

# qhasm:     4x a1 = diag0 + diag1
# asm 1: vadd.i32 >a1=reg128#11,<diag0=reg128#9,<diag1=reg128#8
# asm 2: vadd.i32 >a1=q10,<diag0=q8,<diag1=q7
vadd.i32 q10,q8,q7

# qhasm:     			4x next_a1 = next_diag0 + next_diag1
# asm 1: vadd.i32 >next_a1=reg128#13,<next_diag0=reg128#12,<next_diag1=reg128#10
# asm 2: vadd.i32 >next_a1=q12,<next_diag0=q11,<next_diag1=q9
vadd.i32 q12,q11,q9

# qhasm:     4x b1 = a1 << 9
# asm 1: vshl.i32 >b1=reg128#14,<a1=reg128#11,#9
# asm 2: vshl.i32 >b1=q13,<a1=q10,#9
vshl.i32 q13,q10,#9

# qhasm:     			4x next_b1 = next_a1 << 9
# asm 1: vshl.i32 >next_b1=reg128#15,<next_a1=reg128#13,#9
# asm 2: vshl.i32 >next_b1=q14,<next_a1=q12,#9
vshl.i32 q14,q12,#9

# qhasm: 					       x3 ^= (s3 >>> 19)
# asm 1: eor >x3=int32#4,<x3=int32#4,<s3=int32#13,ROR #19
# asm 2: eor >x3=r3,<x3=r3,<s3=r12,ROR #19
eor r3,r3,r12,ROR #19

# qhasm: 					mem32[&x67_stack] = x6
# asm 1: str <x6=int32#8,<x67_stack=stack64#14
# asm 2: str <x6=r7,<x67_stack=[sp,#104]
str r7,[sp,#104]

# qhasm: 					         x4 ^= (s4 >>> 19)
# asm 1: eor >x4=int32#5,<x4=int32#5,<s4=int32#14,ROR #19
# asm 2: eor >x4=r4,<x4=r4,<s4=r14,ROR #19
eor r4,r4,r14,ROR #19

# qhasm: 					x8 = mem32[&x89_stack]
# asm 1: ldr >x8=int32#8,<x89_stack=stack64#15
# asm 2: ldr >x8=r7,<x89_stack=[sp,#112]
ldr r7,[sp,#112]

# qhasm: 					       s0 = x3 + x2
# asm 1: add >s0=int32#13,<x3=int32#4,<x2=int32#6
# asm 2: add >s0=r12,<x3=r3,<x2=r5
add r12,r3,r5

# qhasm: 					mem32[&x67_stack+4] = x7
# asm 1: str <x7=int32#7,<x67_stack=stack64#14
# asm 2: str <x7=r6,<x67_stack=[sp,#108]
str r6,[sp,#108]

# qhasm: 					         s5 = x4 + x7
# asm 1: add >s5=int32#7,<x4=int32#5,<x7=int32#7
# asm 2: add >s5=r6,<x4=r4,<x7=r6
add r6,r4,r6

# qhasm: 					x9 = mem32[&x89_stack+4]
# asm 1: ldr >x9=int32#14,<x89_stack=stack64#15
# asm 2: ldr >x9=r14,<x89_stack=[sp,#116]
ldr r14,[sp,#116]

# qhasm: 					       x0 ^= (s0 >>> 14)
# asm 1: eor >x0=int32#1,<x0=int32#1,<s0=int32#13,ROR #14
# asm 2: eor >x0=r0,<x0=r0,<s0=r12,ROR #14
eor r0,r0,r12,ROR #14

# qhasm: 					mem32[&x23_stack] = x2
# asm 1: str <x2=int32#6,<x23_stack=stack64#13
# asm 2: str <x2=r5,<x23_stack=[sp,#96]
str r5,[sp,#96]

# qhasm: 					         x5 ^= (s5 >>> 14)
# asm 1: eor >x5=int32#6,<x5=int32#3,<s5=int32#7,ROR #14
# asm 2: eor >x5=r5,<x5=r2,<s5=r6,ROR #14
eor r5,r2,r6,ROR #14

# qhasm: 					x12 = mem32[&x1213_stack]
# asm 1: ldr >x12=int32#3,<x1213_stack=stack64#16
# asm 2: ldr >x12=r2,<x1213_stack=[sp,#120]
ldr r2,[sp,#120]

# qhasm:     4x a1 unsigned>>= 23
# asm 1: vshr.u32 >a1=reg128#11,<a1=reg128#11,#23
# asm 2: vshr.u32 >a1=q10,<a1=q10,#23
vshr.u32 q10,q10,#23

# qhasm:     			4x next_a1 unsigned>>= 23
# asm 1: vshr.u32 >next_a1=reg128#13,<next_a1=reg128#13,#23
# asm 2: vshr.u32 >next_a1=q12,<next_a1=q12,#23
vshr.u32 q12,q12,#23

# qhasm:        diag2 ^= b1
# asm 1: veor >diag2=reg128#3,<diag2=reg128#3,<b1=reg128#14
# asm 2: veor >diag2=q2,<diag2=q2,<b1=q13
veor q2,q2,q13

# qhasm:        			next_diag2 ^= next_b1
# asm 1: veor >next_diag2=reg128#7,<next_diag2=reg128#7,<next_b1=reg128#15
# asm 2: veor >next_diag2=q6,<next_diag2=q6,<next_b1=q14
veor q6,q6,q14

# qhasm:        diag2 ^= a1
# asm 1: veor >diag2=reg128#3,<diag2=reg128#3,<a1=reg128#11
# asm 2: veor >diag2=q2,<diag2=q2,<a1=q10
veor q2,q2,q10

# qhasm:        			next_diag2 ^= next_a1
# asm 1: veor >next_diag2=reg128#7,<next_diag2=reg128#7,<next_a1=reg128#13
# asm 2: veor >next_diag2=q6,<next_diag2=q6,<next_a1=q12
veor q6,q6,q12

# qhasm: 					          s11 = x10 + x9
# asm 1: add >s11=int32#7,<x10=int32#11,<x9=int32#14
# asm 2: add >s11=r6,<x10=r10,<x9=r14
add r6,r10,r14

# qhasm: 					            s12 = x15 + x14
# asm 1: add >s12=int32#13,<x15=int32#10,<x14=int32#9
# asm 2: add >s12=r12,<x15=r9,<x14=r8
add r12,r9,r8

# qhasm:     4x a2 = diag1 + diag2
# asm 1: vadd.i32 >a2=reg128#13,<diag1=reg128#8,<diag2=reg128#3
# asm 2: vadd.i32 >a2=q12,<diag1=q7,<diag2=q2
vadd.i32 q12,q7,q2

# qhasm:             diag1 = diag1[3] diag1[0,1,2]
# asm 1: vext.32 >diag1=reg128#11,<diag1=reg128#8,<diag1=reg128#8,#3
# asm 2: vext.32 >diag1=q10,<diag1=q7,<diag1=q7,#3
vext.32 q10,q7,q7,#3

# qhasm:     			4x next_a2 = next_diag1 + next_diag2
# asm 1: vadd.i32 >next_a2=reg128#8,<next_diag1=reg128#10,<next_diag2=reg128#7
# asm 2: vadd.i32 >next_a2=q7,<next_diag1=q9,<next_diag2=q6
vadd.i32 q7,q9,q6

# qhasm:     4x b2 = a2 << 13
# asm 1: vshl.i32 >b2=reg128#15,<a2=reg128#13,#13
# asm 2: vshl.i32 >b2=q14,<a2=q12,#13
vshl.i32 q14,q12,#13

# qhasm:        				next_diag1 = next_diag1[3] next_diag1[0,1,2]
# asm 1: vext.32 >next_diag1=reg128#14,<next_diag1=reg128#10,<next_diag1=reg128#10,#3
# asm 2: vext.32 >next_diag1=q13,<next_diag1=q9,<next_diag1=q9,#3
vext.32 q13,q9,q9,#3

# qhasm:     			4x next_b2 = next_a2 << 13
# asm 1: vshl.i32 >next_b2=reg128#10,<next_a2=reg128#8,#13
# asm 2: vshl.i32 >next_b2=q9,<next_a2=q7,#13
vshl.i32 q9,q7,#13

# qhasm:     4x a2 unsigned>>= 19
# asm 1: vshr.u32 >a2=reg128#13,<a2=reg128#13,#19
# asm 2: vshr.u32 >a2=q12,<a2=q12,#19
vshr.u32 q12,q12,#19

# qhasm:     			4x next_a2 unsigned>>= 19
# asm 1: vshr.u32 >next_a2=reg128#8,<next_a2=reg128#8,#19
# asm 2: vshr.u32 >next_a2=q7,<next_a2=q7,#19
vshr.u32 q7,q7,#19

# qhasm: 					          x11 ^= (s11 >>> 25)
# asm 1: eor >x11=int32#12,<x11=int32#12,<s11=int32#7,ROR #25
# asm 2: eor >x11=r11,<x11=r11,<s11=r6,ROR #25
eor r11,r11,r6,ROR #25

# qhasm: 					            x12 ^= (s12 >>> 25)
# asm 1: eor >x12=int32#3,<x12=int32#3,<s12=int32#13,ROR #25
# asm 2: eor >x12=r2,<x12=r2,<s12=r12,ROR #25
eor r2,r2,r12,ROR #25

# qhasm: 					           s8 = x11 + x10
# asm 1: add >s8=int32#7,<x11=int32#12,<x10=int32#11
# asm 2: add >s8=r6,<x11=r11,<x10=r10
add r6,r11,r10

# qhasm: 					mem32[&x23_stack+4] = x3
# asm 1: str <x3=int32#4,<x23_stack=stack64#13
# asm 2: str <x3=r3,<x23_stack=[sp,#100]
str r3,[sp,#100]

# qhasm: 					            s13 = x12 + x15
# asm 1: add >s13=int32#4,<x12=int32#3,<x15=int32#10
# asm 2: add >s13=r3,<x12=r2,<x15=r9
add r3,r2,r9

# qhasm: 					x13 = mem32[&x1213_stack+4]
# asm 1: ldr >x13=int32#13,<x1213_stack=stack64#16
# asm 2: ldr >x13=r12,<x1213_stack=[sp,#124]
ldr r12,[sp,#124]

# qhasm:        diag3 ^= b2
# asm 1: veor >diag3=reg128#5,<diag3=reg128#5,<b2=reg128#15
# asm 2: veor >diag3=q4,<diag3=q4,<b2=q14
veor q4,q4,q14

# qhasm:        			next_diag3 ^= next_b2
# asm 1: veor >next_diag3=reg128#6,<next_diag3=reg128#6,<next_b2=reg128#10
# asm 2: veor >next_diag3=q5,<next_diag3=q5,<next_b2=q9
veor q5,q5,q9

# qhasm:        diag3 ^= a2
# asm 1: veor >diag3=reg128#5,<diag3=reg128#5,<a2=reg128#13
# asm 2: veor >diag3=q4,<diag3=q4,<a2=q12
veor q4,q4,q12

# qhasm:        			next_diag3 ^= next_a2
# asm 1: veor >next_diag3=reg128#8,<next_diag3=reg128#6,<next_a2=reg128#8
# asm 2: veor >next_diag3=q7,<next_diag3=q5,<next_a2=q7
veor q7,q5,q7

# qhasm: 					           x8 ^= (s8 >>> 23)
# asm 1: eor >x8=int32#7,<x8=int32#8,<s8=int32#7,ROR #23
# asm 2: eor >x8=r6,<x8=r7,<s8=r6,ROR #23
eor r6,r7,r6,ROR #23

# qhasm: 					            x13 ^= (s13 >>> 23)
# asm 1: eor >x13=int32#4,<x13=int32#13,<s13=int32#4,ROR #23
# asm 2: eor >x13=r3,<x13=r12,<s13=r3,ROR #23
eor r3,r12,r3,ROR #23

# qhasm: 					           s9 = x8 + x11
# asm 1: add >s9=int32#8,<x8=int32#7,<x11=int32#12
# asm 2: add >s9=r7,<x8=r6,<x11=r11
add r7,r6,r11

# qhasm: 					            s14 = x13 + x12
# asm 1: add >s14=int32#13,<x13=int32#4,<x12=int32#3
# asm 2: add >s14=r12,<x13=r3,<x12=r2
add r12,r3,r2

# qhasm:     4x a3 = diag2 + diag3
# asm 1: vadd.i32 >a3=reg128#6,<diag2=reg128#3,<diag3=reg128#5
# asm 2: vadd.i32 >a3=q5,<diag2=q2,<diag3=q4
vadd.i32 q5,q2,q4

# qhasm:             diag2 = diag2[2,3] diag2[0,1]
# asm 1: vswp <diag2=reg128#3%bot,<diag2=reg128#3%top
# asm 2: vswp <diag2=d4,<diag2=d5
vswp d4,d5

# qhasm:     			4x next_a3 = next_diag2 + next_diag3
# asm 1: vadd.i32 >next_a3=reg128#10,<next_diag2=reg128#7,<next_diag3=reg128#8
# asm 2: vadd.i32 >next_a3=q9,<next_diag2=q6,<next_diag3=q7
vadd.i32 q9,q6,q7

# qhasm:     4x b3 = a3 << 18
# asm 1: vshl.i32 >b3=reg128#13,<a3=reg128#6,#18
# asm 2: vshl.i32 >b3=q12,<a3=q5,#18
vshl.i32 q12,q5,#18

# qhasm:        				next_diag2 = next_diag2[2,3] next_diag2[0,1]
# asm 1: vswp <next_diag2=reg128#7%bot,<next_diag2=reg128#7%top
# asm 2: vswp <next_diag2=d12,<next_diag2=d13
vswp d12,d13

# qhasm:     			4x next_b3 = next_a3 << 18
# asm 1: vshl.i32 >next_b3=reg128#15,<next_a3=reg128#10,#18
# asm 2: vshl.i32 >next_b3=q14,<next_a3=q9,#18
vshl.i32 q14,q9,#18

# qhasm: 					           x9 ^= (s9 >>> 19)
# asm 1: eor >x9=int32#8,<x9=int32#14,<s9=int32#8,ROR #19
# asm 2: eor >x9=r7,<x9=r14,<s9=r7,ROR #19
eor r7,r14,r7,ROR #19

# qhasm: 					            x14 ^= (s14 >>> 19)
# asm 1: eor >x14=int32#9,<x14=int32#9,<s14=int32#13,ROR #19
# asm 2: eor >x14=r8,<x14=r8,<s14=r12,ROR #19
eor r8,r8,r12,ROR #19

# qhasm: 					          s10 = x9 + x8
# asm 1: add >s10=int32#13,<x9=int32#8,<x8=int32#7
# asm 2: add >s10=r12,<x9=r7,<x8=r6
add r12,r7,r6

# qhasm: 					            s15 = x14 + x13
# asm 1: add >s15=int32#14,<x14=int32#9,<x13=int32#4
# asm 2: add >s15=r14,<x14=r8,<x13=r3
add r14,r8,r3

# qhasm:     4x a3 unsigned>>= 14
# asm 1: vshr.u32 >a3=reg128#16,<a3=reg128#6,#14
# asm 2: vshr.u32 >a3=q15,<a3=q5,#14
vshr.u32 q15,q5,#14

# qhasm:             diag3 = diag3[1,2,3] diag3[0]
# asm 1: vext.32 >diag3=reg128#6,<diag3=reg128#5,<diag3=reg128#5,#1
# asm 2: vext.32 >diag3=q5,<diag3=q4,<diag3=q4,#1
vext.32 q5,q4,q4,#1

# qhasm:     			4x next_a3 unsigned>>= 14
# asm 1: vshr.u32 >next_a3=reg128#5,<next_a3=reg128#10,#14
# asm 2: vshr.u32 >next_a3=q4,<next_a3=q9,#14
vshr.u32 q4,q9,#14

# qhasm:        diag0 ^= b3
# asm 1: veor >diag0=reg128#9,<diag0=reg128#9,<b3=reg128#13
# asm 2: veor >diag0=q8,<diag0=q8,<b3=q12
veor q8,q8,q12

# qhasm:        				next_diag3 = next_diag3[1,2,3] next_diag3[0]
# asm 1: vext.32 >next_diag3=reg128#8,<next_diag3=reg128#8,<next_diag3=reg128#8,#1
# asm 2: vext.32 >next_diag3=q7,<next_diag3=q7,<next_diag3=q7,#1
vext.32 q7,q7,q7,#1

# qhasm:        			next_diag0 ^= next_b3
# asm 1: veor >next_diag0=reg128#10,<next_diag0=reg128#12,<next_b3=reg128#15
# asm 2: veor >next_diag0=q9,<next_diag0=q11,<next_b3=q14
veor q9,q11,q14

# qhasm: 					          x10 ^= (s10 >>> 14)
# asm 1: eor >x10=int32#11,<x10=int32#11,<s10=int32#13,ROR #14
# asm 2: eor >x10=r10,<x10=r10,<s10=r12,ROR #14
eor r10,r10,r12,ROR #14

# qhasm:     								i = i_stack
# asm 1: ldr >i=int32#13,<i_stack=stack32#7
# asm 2: ldr >i=r12,<i_stack=[sp,#248]
ldr r12,[sp,#248]

# qhasm:        diag0 ^= a3
# asm 1: veor >diag0=reg128#9,<diag0=reg128#9,<a3=reg128#16
# asm 2: veor >diag0=q8,<diag0=q8,<a3=q15
veor q8,q8,q15

# qhasm: 					            x15 ^= (s15 >>> 14)
# asm 1: eor >x15=int32#10,<x15=int32#10,<s15=int32#14,ROR #14
# asm 2: eor >x15=r9,<x15=r9,<s15=r14,ROR #14
eor r9,r9,r14,ROR #14

# qhasm:        			next_diag0 ^= next_a3
# asm 1: veor >next_diag0=reg128#12,<next_diag0=reg128#10,<next_a3=reg128#5
# asm 2: veor >next_diag0=q11,<next_diag0=q9,<next_a3=q4
veor q11,q9,q4

# qhasm:                   						unsigned>? i -= 4
# asm 1: subs >i=int32#13,<i=int32#13,#4
# asm 2: subs >i=r12,<i=r12,#4
subs r12,r12,#4

# qhasm:   goto mainloop2 if unsigned>
bhi ._mainloop2

# qhasm:     assign r8 r9 to x14 x15; x1415_stack = x14 x15
# asm 1: strd <x14=int32#9,>x1415_stack=stack64#15
# asm 2: strd <x14=r8,>x1415_stack=[sp,#112]
strd r8,[sp,#112]

# qhasm:     assign r8 r9 to j0 j1 = j01_stack
# asm 1: ldrd >j0=int32#9,<j01_stack=stack64#9
# asm 2: ldrd >j0=r8,<j01_stack=[sp,#64]
ldrd r8,[sp,#64]

# qhasm:     assign r2 r3 to x12 x13; x1213_stack = x12 x13
# asm 1: strd <x12=int32#3,>x1213_stack=stack64#16
# asm 2: strd <x12=r2,>x1213_stack=[sp,#120]
strd r2,[sp,#120]

# qhasm:     assign r2 r3 to x2 x3 = x23_stack
# asm 1: ldrd >x2=int32#3,<x23_stack=stack64#13
# asm 2: ldrd >x2=r2,<x23_stack=[sp,#96]
ldrd r2,[sp,#96]

# qhasm:   x0 += j0
# asm 1: add >x0=int32#1,<x0=int32#1,<j0=int32#9
# asm 2: add >x0=r0,<x0=r0,<j0=r8
add r0,r0,r8

# qhasm:     assign r10 r11 to x10 x11; x1011_stack = x10 x11
# asm 1: strd <x10=int32#11,>x1011_stack=stack64#13
# asm 2: strd <x10=r10,>x1011_stack=[sp,#96]
strd r10,[sp,#96]

# qhasm:   x1 += j1
# asm 1: add >x1=int32#2,<x1=int32#2,<j1=int32#10
# asm 2: add >x1=r1,<x1=r1,<j1=r9
add r1,r1,r9

# qhasm:     assign r10 r11 to j2 j3 = j23_stack
# asm 1: ldrd >j2=int32#11,<j23_stack=stack64#7
# asm 2: ldrd >j2=r10,<j23_stack=[sp,#48]
ldrd r10,[sp,#48]

# qhasm:     assign r8 r9 to j4 j5 = j45_stack
# asm 1: ldrd >j4=int32#9,<j45_stack=stack64#10
# asm 2: ldrd >j4=r8,<j45_stack=[sp,#72]
ldrd r8,[sp,#72]

# qhasm:   x2 += j2
# asm 1: add >x2=int32#3,<x2=int32#3,<j2=int32#11
# asm 2: add >x2=r2,<x2=r2,<j2=r10
add r2,r2,r10

# qhasm:     assign r6 r7 to x8 x9; x89_stack = x8 x9
# asm 1: strd <x8=int32#7,>x89_stack=stack64#17
# asm 2: strd <x8=r6,>x89_stack=[sp,#128]
strd r6,[sp,#128]

# qhasm:   x3 += j3
# asm 1: add >x3=int32#4,<x3=int32#4,<j3=int32#12
# asm 2: add >x3=r3,<x3=r3,<j3=r11
add r3,r3,r11

# qhasm:     assign r6 r7 to x6 x7 = x67_stack
# asm 1: ldrd >x6=int32#7,<x67_stack=stack64#14
# asm 2: ldrd >x6=r6,<x67_stack=[sp,#104]
ldrd r6,[sp,#104]

# qhasm:     assign r10 r11 to j6 j7 = j67_stack
# asm 1: ldrd >j6=int32#11,<j67_stack=stack64#5
# asm 2: ldrd >j6=r10,<j67_stack=[sp,#32]
ldrd r10,[sp,#32]

# qhasm:     m = m_stack
# asm 1: ldr >m=int32#13,<m_stack=stack32#4
# asm 2: ldr >m=r12,<m_stack=[sp,#236]
ldr r12,[sp,#236]

# qhasm:   x4 += j4
# asm 1: add >x4=int32#5,<x4=int32#5,<j4=int32#9
# asm 2: add >x4=r4,<x4=r4,<j4=r8
add r4,r4,r8

# qhasm:   x5 += j5
# asm 1: add >x5=int32#6,<x5=int32#6,<j5=int32#10
# asm 2: add >x5=r5,<x5=r5,<j5=r9
add r5,r5,r9

# qhasm:   x6 += j6
# asm 1: add >x6=int32#7,<x6=int32#7,<j6=int32#11
# asm 2: add >x6=r6,<x6=r6,<j6=r10
add r6,r6,r10

# qhasm:   x7 += j7
# asm 1: add >x7=int32#8,<x7=int32#8,<j7=int32#12
# asm 2: add >x7=r7,<x7=r7,<j7=r11
add r7,r7,r11

# qhasm:                      =? m - 0
# asm 1: cmp <m=int32#13,#0
# asm 2: cmp <m=r12,#0
cmp r12,#0

# qhasm:   goto nomessage1 if =
beq ._nomessage1

# qhasm:       m0 = mem32[m + 0]
# asm 1: ldr >m0=int32#9,[<m=int32#13,#0]
# asm 2: ldr >m0=r8,[<m=r12,#0]
ldr r8,[r12,#0]

# qhasm:       m1 = mem32[m + 4]
# asm 1: ldr >m1=int32#10,[<m=int32#13,#4]
# asm 2: ldr >m1=r9,[<m=r12,#4]
ldr r9,[r12,#4]

# qhasm:       m2 = mem32[m + 8]
# asm 1: ldr >m2=int32#11,[<m=int32#13,#8]
# asm 2: ldr >m2=r10,[<m=r12,#8]
ldr r10,[r12,#8]

# qhasm:       m3 = mem32[m + 12]
# asm 1: ldr >m3=int32#12,[<m=int32#13,#12]
# asm 2: ldr >m3=r11,[<m=r12,#12]
ldr r11,[r12,#12]

# qhasm:     x0 ^= m0
# asm 1: eor >x0=int32#1,<x0=int32#1,<m0=int32#9
# asm 2: eor >x0=r0,<x0=r0,<m0=r8
eor r0,r0,r8

# qhasm:       m4 = mem32[m + 16]
# asm 1: ldr >m4=int32#9,[<m=int32#13,#16]
# asm 2: ldr >m4=r8,[<m=r12,#16]
ldr r8,[r12,#16]

# qhasm:     x1 ^= m1
# asm 1: eor >x1=int32#2,<x1=int32#2,<m1=int32#10
# asm 2: eor >x1=r1,<x1=r1,<m1=r9
eor r1,r1,r9

# qhasm:       m5 = mem32[m + 20]
# asm 1: ldr >m5=int32#10,[<m=int32#13,#20]
# asm 2: ldr >m5=r9,[<m=r12,#20]
ldr r9,[r12,#20]

# qhasm:     x2 ^= m2
# asm 1: eor >x2=int32#3,<x2=int32#3,<m2=int32#11
# asm 2: eor >x2=r2,<x2=r2,<m2=r10
eor r2,r2,r10

# qhasm:       m6 = mem32[m + 24]
# asm 1: ldr >m6=int32#11,[<m=int32#13,#24]
# asm 2: ldr >m6=r10,[<m=r12,#24]
ldr r10,[r12,#24]

# qhasm:     x3 ^= m3
# asm 1: eor >x3=int32#4,<x3=int32#4,<m3=int32#12
# asm 2: eor >x3=r3,<x3=r3,<m3=r11
eor r3,r3,r11

# qhasm:       m7 = mem32[m + 28]
# asm 1: ldr >m7=int32#12,[<m=int32#13,#28]
# asm 2: ldr >m7=r11,[<m=r12,#28]
ldr r11,[r12,#28]

# qhasm:     x4 ^= m4
# asm 1: eor >x4=int32#5,<x4=int32#5,<m4=int32#9
# asm 2: eor >x4=r4,<x4=r4,<m4=r8
eor r4,r4,r8

# qhasm:     x5 ^= m5
# asm 1: eor >x5=int32#6,<x5=int32#6,<m5=int32#10
# asm 2: eor >x5=r5,<x5=r5,<m5=r9
eor r5,r5,r9

# qhasm:     x6 ^= m6
# asm 1: eor >x6=int32#7,<x6=int32#7,<m6=int32#11
# asm 2: eor >x6=r6,<x6=r6,<m6=r10
eor r6,r6,r10

# qhasm:     x7 ^= m7
# asm 1: eor >x7=int32#8,<x7=int32#8,<m7=int32#12
# asm 2: eor >x7=r7,<x7=r7,<m7=r11
eor r7,r7,r11

# qhasm:   nomessage1:
._nomessage1:

# qhasm:   c = c_stack
# asm 1: ldr >c=int32#14,<c_stack=stack32#3
# asm 2: ldr >c=r14,<c_stack=[sp,#232]
ldr r14,[sp,#232]

# qhasm: 				  4x v0 = diag0 + start0           
# asm 1: vadd.i32 >v0=reg128#5,<diag0=reg128#9,<start0=reg128#2
# asm 2: vadd.i32 >v0=q4,<diag0=q8,<start0=q1
vadd.i32 q4,q8,q1

# qhasm:   mem32[c + 0] = x0
# asm 1: str <x0=int32#1,[<c=int32#14,#0]
# asm 2: str <x0=r0,[<c=r14,#0]
str r0,[r14,#0]

# qhasm: 				  ptr_start3 = &stack_start3
# asm 1: lea >ptr_start3=int32#1,<stack_start3=stack128#4
# asm 2: lea >ptr_start3=r0,<stack_start3=[sp,#304]
add r0,sp,#304

# qhasm:   mem32[c + 4] = x1
# asm 1: str <x1=int32#2,[<c=int32#14,#4]
# asm 2: str <x1=r1,[<c=r14,#4]
str r1,[r14,#4]

# qhasm: 				  v4 aligned= mem128[ptr_start3]
# asm 1: vld1.8 {>v4=reg128#9%bot->v4=reg128#9%top},[<ptr_start3=int32#1,: 128]
# asm 2: vld1.8 {>v4=d16->v4=d17},[<ptr_start3=r0,: 128]
vld1.8 {d16-d17},[r0,: 128]

# qhasm:   mem32[c + 8] = x2
# asm 1: str <x2=int32#3,[<c=int32#14,#8]
# asm 2: str <x2=r2,[<c=r14,#8]
str r2,[r14,#8]

# qhasm: 				  4x v4 += diag3                   
# asm 1: vadd.i32 >v4=reg128#6,<v4=reg128#9,<diag3=reg128#6
# asm 2: vadd.i32 >v4=q5,<v4=q8,<diag3=q5
vadd.i32 q5,q8,q5

# qhasm:   mem32[c + 12] = x3
# asm 1: str <x3=int32#4,[<c=int32#14,#12]
# asm 2: str <x3=r3,[<c=r14,#12]
str r3,[r14,#12]

# qhasm: 				  ptr_start2 = &stack_start2
# asm 1: lea >ptr_start2=int32#1,<stack_start2=stack128#3
# asm 2: lea >ptr_start2=r0,<stack_start2=[sp,#288]
add r0,sp,#288

# qhasm:   mem32[c + 16] = x4
# asm 1: str <x4=int32#5,[<c=int32#14,#16]
# asm 2: str <x4=r4,[<c=r14,#16]
str r4,[r14,#16]

# qhasm: 				  v8 aligned= mem128[ptr_start2]
# asm 1: vld1.8 {>v8=reg128#9%bot->v8=reg128#9%top},[<ptr_start2=int32#1,: 128]
# asm 2: vld1.8 {>v8=d16->v8=d17},[<ptr_start2=r0,: 128]
vld1.8 {d16-d17},[r0,: 128]

# qhasm:   mem32[c + 20] = x5
# asm 1: str <x5=int32#6,[<c=int32#14,#20]
# asm 2: str <x5=r5,[<c=r14,#20]
str r5,[r14,#20]

# qhasm: 				  4x v12 = diag1 + start1          
# asm 1: vadd.i32 >v12=reg128#10,<diag1=reg128#11,<start1=reg128#1
# asm 2: vadd.i32 >v12=q9,<diag1=q10,<start1=q0
vadd.i32 q9,q10,q0

# qhasm:   mem32[c + 24] = x6
# asm 1: str <x6=int32#7,[<c=int32#14,#24]
# asm 2: str <x6=r6,[<c=r14,#24]
str r6,[r14,#24]

# qhasm: 				  4x v8 += diag2                   
# asm 1: vadd.i32 >v8=reg128#3,<v8=reg128#9,<diag2=reg128#3
# asm 2: vadd.i32 >v8=q2,<v8=q8,<diag2=q2
vadd.i32 q2,q8,q2

# qhasm:   mem32[c + 28] = x7
# asm 1: str <x7=int32#8,[<c=int32#14,#28]
# asm 2: str <x7=r7,[<c=r14,#28]
str r7,[r14,#28]

# qhasm: 				  2x abab = 0xffffffff
# asm 1: vmov.i64 >abab=reg128#9,#0xffffffff
# asm 2: vmov.i64 >abab=q8,#0xffffffff
vmov.i64 q8,#0xffffffff

# qhasm:   assign r6 r7 to x8 x9 = x89_stack
# asm 1: ldrd >x8=int32#7,<x89_stack=stack64#17
# asm 2: ldrd >x8=r6,<x89_stack=[sp,#128]
ldrd r6,[sp,#128]

# qhasm: 				  new t4

# qhasm: 				  t4 = v0[1] v4[0] t4[2,3]         
# asm 1: vext.32 <t4=reg128#11%bot,<v0=reg128#5%bot,<v4=reg128#6%bot,#1
# asm 2: vext.32 <t4=d20,<v0=d8,<v4=d10,#1
vext.32 d20,d8,d10,#1

# qhasm:   assign r0 r1 to j8 j9 = j89_stack
# asm 1: ldrd >j8=int32#1,<j89_stack=stack64#6
# asm 2: ldrd >j8=r0,<j89_stack=[sp,#40]
ldrd r0,[sp,#40]

# qhasm: 				  new t12

# qhasm: 				  t12 = t12[0,1] v0[3] v4[2]       
# asm 1: vext.32 <t12=reg128#13%top,<v0=reg128#5%top,<v4=reg128#6%top,#1
# asm 2: vext.32 <t12=d25,<v0=d9,<v4=d11,#1
vext.32 d25,d9,d11,#1

# qhasm:   assign r2 r3 to x12 x13 = x1213_stack
# asm 1: ldrd >x12=int32#3,<x1213_stack=stack64#16
# asm 2: ldrd >x12=r2,<x1213_stack=[sp,#120]
ldrd r2,[sp,#120]

# qhasm: 				  v0 = (abab & v0) | (~abab & v12) 
# asm 1: vbif <v0=reg128#5,<v12=reg128#10,<abab=reg128#9
# asm 2: vbif <v0=q4,<v12=q9,<abab=q8
vbif q4,q9,q8

# qhasm:   assign r4 r5 to j12 j13 = j1213_stack
# asm 1: ldrd >j12=int32#5,<j1213_stack=stack64#8
# asm 2: ldrd >j12=r4,<j1213_stack=[sp,#56]
ldrd r4,[sp,#56]

# qhasm: 				  t4 = t4[0,1] v8[3] v12[2]        
# asm 1: vext.32 <t4=reg128#11%top,<v8=reg128#3%top,<v12=reg128#10%top,#1
# asm 2: vext.32 <t4=d21,<v8=d5,<v12=d19,#1
vext.32 d21,d5,d19,#1

# qhasm:   x8 += j8
# asm 1: add >x8=int32#7,<x8=int32#7,<j8=int32#1
# asm 2: add >x8=r6,<x8=r6,<j8=r0
add r6,r6,r0

# qhasm: 				  t12 = v8[1] v12[0] t12[2,3]      
# asm 1: vext.32 <t12=reg128#13%bot,<v8=reg128#3%bot,<v12=reg128#10%bot,#1
# asm 2: vext.32 <t12=d24,<v8=d4,<v12=d18,#1
vext.32 d24,d4,d18,#1

# qhasm:   x9 += j9
# asm 1: add >x9=int32#8,<x9=int32#8,<j9=int32#2
# asm 2: add >x9=r7,<x9=r7,<j9=r1
add r7,r7,r1

# qhasm: 				  v8 = (abab & v8) | (~abab & v4)  
# asm 1: vbif <v8=reg128#3,<v4=reg128#6,<abab=reg128#9
# asm 2: vbif <v8=q2,<v4=q5,<abab=q8
vbif q2,q5,q8

# qhasm:   x12 += j12
# asm 1: add >x12=int32#3,<x12=int32#3,<j12=int32#5
# asm 2: add >x12=r2,<x12=r2,<j12=r4
add r2,r2,r4

# qhasm: 				  v4 = t4[1]t4[0]t4[3]t4[2]        
# asm 1: vrev64.i32 >v4=reg128#6,<t4=reg128#11
# asm 2: vrev64.i32 >v4=q5,<t4=q10
vrev64.i32 q5,q10

# qhasm:   x13 += j13
# asm 1: add >x13=int32#4,<x13=int32#4,<j13=int32#6
# asm 2: add >x13=r3,<x13=r3,<j13=r5
add r3,r3,r5

# qhasm: 				  v12 = t12[1]t12[0]t12[3]t12[2]   
# asm 1: vrev64.i32 >v12=reg128#10,<t12=reg128#13
# asm 2: vrev64.i32 >v12=q9,<t12=q12
vrev64.i32 q9,q12

# qhasm:   carry? j8 += 3
# asm 1: adds >j8=int32#1,<j8=int32#1,#3
# asm 2: adds >j8=r0,<j8=r0,#3
adds r0,r0,#3

# qhasm: 				  v0 v8 = v0[0] v8[1] v8[0] v0[1]  
# asm 1: vswp <v8=reg128#3%top,<v0=reg128#5%top
# asm 2: vswp <v8=d5,<v0=d9
vswp d5,d9

# qhasm:   j9 += 0 + carry
# asm 1: adc >j9=int32#2,<j9=int32#2,#0
# asm 2: adc >j9=r1,<j9=r1,#0
adc r1,r1,#0

# qhasm:   assign r0 r1 to j8 j9; j89_stack = j8 j9
# asm 1: strd <j8=int32#1,>j89_stack=stack64#6
# asm 2: strd <j8=r0,>j89_stack=[sp,#40]
strd r0,[sp,#40]

# qhasm:   assign r8 r9 to x14 x15 = x1415_stack
# asm 1: ldrd >x14=int32#9,<x1415_stack=stack64#15
# asm 2: ldrd >x14=r8,<x1415_stack=[sp,#112]
ldrd r8,[sp,#112]

# qhasm:   assign r0 r1 to j14 j15 = j1415_stack
# asm 1: ldrd >j14=int32#1,<j1415_stack=stack64#12
# asm 2: ldrd >j14=r0,<j1415_stack=[sp,#88]
ldrd r0,[sp,#88]

# qhasm:   assign r10 r11 to x10 x11 = x1011_stack
# asm 1: ldrd >x10=int32#11,<x1011_stack=stack64#13
# asm 2: ldrd >x10=r10,<x1011_stack=[sp,#96]
ldrd r10,[sp,#96]

# qhasm:   assign r4 r5 to j10 j11 = j1011_stack
# asm 1: ldrd >j10=int32#5,<j1011_stack=stack64#11
# asm 2: ldrd >j10=r4,<j1011_stack=[sp,#80]
ldrd r4,[sp,#80]

# qhasm:   x14 += j14
# asm 1: add >x14=int32#1,<x14=int32#9,<j14=int32#1
# asm 2: add >x14=r0,<x14=r8,<j14=r0
add r0,r8,r0

# qhasm:   x15 += j15
# asm 1: add >x15=int32#2,<x15=int32#10,<j15=int32#2
# asm 2: add >x15=r1,<x15=r9,<j15=r1
add r1,r9,r1

# qhasm:   x10 += j10
# asm 1: add >x10=int32#5,<x10=int32#11,<j10=int32#5
# asm 2: add >x10=r4,<x10=r10,<j10=r4
add r4,r10,r4

# qhasm:   x11 += j11
# asm 1: add >x11=int32#6,<x11=int32#12,<j11=int32#6
# asm 2: add >x11=r5,<x11=r11,<j11=r5
add r5,r11,r5

# qhasm: 				  c += 64
# asm 1: add >c=int32#9,<c=int32#14,#64
# asm 2: add >c=r8,<c=r14,#64
add r8,r14,#64

# qhasm:                      =? m - 0
# asm 1: cmp <m=int32#13,#0
# asm 2: cmp <m=r12,#0
cmp r12,#0

# qhasm:   goto nomessage2 if =
beq ._nomessage2

# qhasm:       m8 = mem32[m + 32]
# asm 1: ldr >m8=int32#10,[<m=int32#13,#32]
# asm 2: ldr >m8=r9,[<m=r12,#32]
ldr r9,[r12,#32]

# qhasm:       m9 = mem32[m + 36]
# asm 1: ldr >m9=int32#11,[<m=int32#13,#36]
# asm 2: ldr >m9=r10,[<m=r12,#36]
ldr r10,[r12,#36]

# qhasm:       m10 = mem32[m + 40]
# asm 1: ldr >m10=int32#12,[<m=int32#13,#40]
# asm 2: ldr >m10=r11,[<m=r12,#40]
ldr r11,[r12,#40]

# qhasm:       m11 = mem32[m + 44]
# asm 1: ldr >m11=int32#14,[<m=int32#13,#44]
# asm 2: ldr >m11=r14,[<m=r12,#44]
ldr r14,[r12,#44]

# qhasm:     x8 ^= m8
# asm 1: eor >x8=int32#7,<x8=int32#7,<m8=int32#10
# asm 2: eor >x8=r6,<x8=r6,<m8=r9
eor r6,r6,r9

# qhasm:       m12 = mem32[m + 48]
# asm 1: ldr >m12=int32#10,[<m=int32#13,#48]
# asm 2: ldr >m12=r9,[<m=r12,#48]
ldr r9,[r12,#48]

# qhasm:     x9 ^= m9
# asm 1: eor >x9=int32#8,<x9=int32#8,<m9=int32#11
# asm 2: eor >x9=r7,<x9=r7,<m9=r10
eor r7,r7,r10

# qhasm:       m13 = mem32[m + 52]
# asm 1: ldr >m13=int32#11,[<m=int32#13,#52]
# asm 2: ldr >m13=r10,[<m=r12,#52]
ldr r10,[r12,#52]

# qhasm:     x10 ^= m10
# asm 1: eor >x10=int32#5,<x10=int32#5,<m10=int32#12
# asm 2: eor >x10=r4,<x10=r4,<m10=r11
eor r4,r4,r11

# qhasm:       m14 = mem32[m + 56]
# asm 1: ldr >m14=int32#12,[<m=int32#13,#56]
# asm 2: ldr >m14=r11,[<m=r12,#56]
ldr r11,[r12,#56]

# qhasm:     x11 ^= m11
# asm 1: eor >x11=int32#6,<x11=int32#6,<m11=int32#14
# asm 2: eor >x11=r5,<x11=r5,<m11=r14
eor r5,r5,r14

# qhasm:       m15 = mem32[m + 60]
# asm 1: ldr >m15=int32#14,[<m=int32#13,#60]
# asm 2: ldr >m15=r14,[<m=r12,#60]
ldr r14,[r12,#60]

# qhasm:     m += 64
# asm 1: add >m=int32#13,<m=int32#13,#64
# asm 2: add >m=r12,<m=r12,#64
add r12,r12,#64

# qhasm:     x12 ^= m12
# asm 1: eor >x12=int32#3,<x12=int32#3,<m12=int32#10
# asm 2: eor >x12=r2,<x12=r2,<m12=r9
eor r2,r2,r9

# qhasm: 				    m0m1m2m3 = mem128[m]; m += 16
# asm 1: vld1.8 {>m0m1m2m3=reg128#11%bot->m0m1m2m3=reg128#11%top},[<m=int32#13]!
# asm 2: vld1.8 {>m0m1m2m3=d20->m0m1m2m3=d21},[<m=r12]!
vld1.8 {d20-d21},[r12]!

# qhasm: 				    v0 ^= m0m1m2m3
# asm 1: veor >v0=reg128#5,<v0=reg128#5,<m0m1m2m3=reg128#11
# asm 2: veor >v0=q4,<v0=q4,<m0m1m2m3=q10
veor q4,q4,q10

# qhasm:     x13 ^= m13
# asm 1: eor >x13=int32#4,<x13=int32#4,<m13=int32#11
# asm 2: eor >x13=r3,<x13=r3,<m13=r10
eor r3,r3,r10

# qhasm: 				    m4m5m6m7 = mem128[m]; m += 16
# asm 1: vld1.8 {>m4m5m6m7=reg128#11%bot->m4m5m6m7=reg128#11%top},[<m=int32#13]!
# asm 2: vld1.8 {>m4m5m6m7=d20->m4m5m6m7=d21},[<m=r12]!
vld1.8 {d20-d21},[r12]!

# qhasm: 				    v4 ^= m4m5m6m7
# asm 1: veor >v4=reg128#6,<v4=reg128#6,<m4m5m6m7=reg128#11
# asm 2: veor >v4=q5,<v4=q5,<m4m5m6m7=q10
veor q5,q5,q10

# qhasm:     x14 ^= m14
# asm 1: eor >x14=int32#1,<x14=int32#1,<m14=int32#12
# asm 2: eor >x14=r0,<x14=r0,<m14=r11
eor r0,r0,r11

# qhasm: 				    m8m9m10m11 = mem128[m]; m += 16
# asm 1: vld1.8 {>m8m9m10m11=reg128#11%bot->m8m9m10m11=reg128#11%top},[<m=int32#13]!
# asm 2: vld1.8 {>m8m9m10m11=d20->m8m9m10m11=d21},[<m=r12]!
vld1.8 {d20-d21},[r12]!

# qhasm: 				    v8 ^= m8m9m10m11
# asm 1: veor >v8=reg128#3,<v8=reg128#3,<m8m9m10m11=reg128#11
# asm 2: veor >v8=q2,<v8=q2,<m8m9m10m11=q10
veor q2,q2,q10

# qhasm:     x15 ^= m15
# asm 1: eor >x15=int32#2,<x15=int32#2,<m15=int32#14
# asm 2: eor >x15=r1,<x15=r1,<m15=r14
eor r1,r1,r14

# qhasm: 				    m12m13m14m15 = mem128[m]; m += 16
# asm 1: vld1.8 {>m12m13m14m15=reg128#11%bot->m12m13m14m15=reg128#11%top},[<m=int32#13]!
# asm 2: vld1.8 {>m12m13m14m15=d20->m12m13m14m15=d21},[<m=r12]!
vld1.8 {d20-d21},[r12]!

# qhasm: 				    v12 ^= m12m13m14m15
# asm 1: veor >v12=reg128#10,<v12=reg128#10,<m12m13m14m15=reg128#11
# asm 2: veor >v12=q9,<v12=q9,<m12m13m14m15=q10
veor q9,q9,q10

# qhasm:   nomessage2:
._nomessage2:

# qhasm: 				  mem128[c] = v0; c += 16
# asm 1: vst1.8 {<v0=reg128#5%bot-<v0=reg128#5%top},[<c=int32#9]!
# asm 2: vst1.8 {<v0=d8-<v0=d9},[<c=r8]!
vst1.8 {d8-d9},[r8]!

# qhasm: 				  mem128[c] = v4; c += 16
# asm 1: vst1.8 {<v4=reg128#6%bot-<v4=reg128#6%top},[<c=int32#9]!
# asm 2: vst1.8 {<v4=d10-<v4=d11},[<c=r8]!
vst1.8 {d10-d11},[r8]!

# qhasm: 				    2x blockshift = 0xff
# asm 1: vmov.i64 >blockshift=reg128#5,#0xff
# asm 2: vmov.i64 >blockshift=q4,#0xff
vmov.i64 q4,#0xff

# qhasm: 				  mem128[c] = v8; c += 16
# asm 1: vst1.8 {<v8=reg128#3%bot-<v8=reg128#3%top},[<c=int32#9]!
# asm 2: vst1.8 {<v8=d4-<v8=d5},[<c=r8]!
vst1.8 {d4-d5},[r8]!

# qhasm: 				  mem128[c] = v12; c += 16
# asm 1: vst1.8 {<v12=reg128#10%bot-<v12=reg128#10%top},[<c=int32#9]!
# asm 2: vst1.8 {<v12=d18-<v12=d19},[<c=r8]!
vst1.8 {d18-d19},[r8]!

# qhasm:   mem32[c - 96] = x8
# asm 1: str <x8=int32#7,[<c=int32#9,#-96]
# asm 2: str <x8=r6,[<c=r8,#-96]
str r6,[r8,#-96]

# qhasm: 				  ptr_start3 = &next_start3
# asm 1: lea >ptr_start3=int32#7,<next_start3=stack128#6
# asm 2: lea >ptr_start3=r6,<next_start3=[sp,#336]
add r6,sp,#336

# qhasm:   mem32[c - 92] = x9
# asm 1: str <x9=int32#8,[<c=int32#9,#-92]
# asm 2: str <x9=r7,[<c=r8,#-92]
str r7,[r8,#-92]

# qhasm: 				  ptr_start2 = &next_start2
# asm 1: lea >ptr_start2=int32#8,<next_start2=stack128#5
# asm 2: lea >ptr_start2=r7,<next_start2=[sp,#320]
add r7,sp,#320

# qhasm:   mem32[c - 88] = x10
# asm 1: str <x10=int32#5,[<c=int32#9,#-88]
# asm 2: str <x10=r4,[<c=r8,#-88]
str r4,[r8,#-88]

# qhasm: 				  4x v0 = next_diag0 + start0      
# asm 1: vadd.i32 >v0=reg128#3,<next_diag0=reg128#12,<start0=reg128#2
# asm 2: vadd.i32 >v0=q2,<next_diag0=q11,<start0=q1
vadd.i32 q2,q11,q1

# qhasm: 				  v4 aligned= mem128[ptr_start3]
# asm 1: vld1.8 {>v4=reg128#6%bot->v4=reg128#6%top},[<ptr_start3=int32#7,: 128]
# asm 2: vld1.8 {>v4=d10->v4=d11},[<ptr_start3=r6,: 128]
vld1.8 {d10-d11},[r6,: 128]

# qhasm: 				  4x v4 += next_diag3              
# asm 1: vadd.i32 >v4=reg128#6,<v4=reg128#6,<next_diag3=reg128#8
# asm 2: vadd.i32 >v4=q5,<v4=q5,<next_diag3=q7
vadd.i32 q5,q5,q7

# qhasm: 				  v8 aligned= mem128[ptr_start2]
# asm 1: vld1.8 {>v8=reg128#8%bot->v8=reg128#8%top},[<ptr_start2=int32#8,: 128]
# asm 2: vld1.8 {>v8=d14->v8=d15},[<ptr_start2=r7,: 128]
vld1.8 {d14-d15},[r7,: 128]

# qhasm: 				  4x v12 = next_diag1 + start1     
# asm 1: vadd.i32 >v12=reg128#10,<next_diag1=reg128#14,<start1=reg128#1
# asm 2: vadd.i32 >v12=q9,<next_diag1=q13,<start1=q0
vadd.i32 q9,q13,q0

# qhasm: 				  4x v8 += next_diag2              
# asm 1: vadd.i32 >v8=reg128#7,<v8=reg128#8,<next_diag2=reg128#7
# asm 2: vadd.i32 >v8=q6,<v8=q7,<next_diag2=q6
vadd.i32 q6,q7,q6

# qhasm:   mem32[c - 84] = x11
# asm 1: str <x11=int32#6,[<c=int32#9,#-84]
# asm 2: str <x11=r5,[<c=r8,#-84]
str r5,[r8,#-84]

# qhasm: 				  new t4

# qhasm: 				  t4 = v0[1] v4[0] t4[2,3]         
# asm 1: vext.32 <t4=reg128#8%bot,<v0=reg128#3%bot,<v4=reg128#6%bot,#1
# asm 2: vext.32 <t4=d14,<v0=d4,<v4=d10,#1
vext.32 d14,d4,d10,#1

# qhasm:   mem32[c - 80] = x12
# asm 1: str <x12=int32#3,[<c=int32#9,#-80]
# asm 2: str <x12=r2,[<c=r8,#-80]
str r2,[r8,#-80]

# qhasm: 				  new t12

# qhasm: 				  t12 = t12[0,1] v0[3] v4[2]       
# asm 1: vext.32 <t12=reg128#11%top,<v0=reg128#3%top,<v4=reg128#6%top,#1
# asm 2: vext.32 <t12=d21,<v0=d5,<v4=d11,#1
vext.32 d21,d5,d11,#1

# qhasm:   mem32[c - 76] = x13
# asm 1: str <x13=int32#4,[<c=int32#9,#-76]
# asm 2: str <x13=r3,[<c=r8,#-76]
str r3,[r8,#-76]

# qhasm: 				  v0 = (abab & v0) | (~abab & v12) 
# asm 1: vbif <v0=reg128#3,<v12=reg128#10,<abab=reg128#9
# asm 2: vbif <v0=q2,<v12=q9,<abab=q8
vbif q2,q9,q8

# qhasm:   mem32[c - 72] = x14
# asm 1: str <x14=int32#1,[<c=int32#9,#-72]
# asm 2: str <x14=r0,[<c=r8,#-72]
str r0,[r8,#-72]

# qhasm: 				  t4 = t4[0,1] v8[3] v12[2]        
# asm 1: vext.32 <t4=reg128#8%top,<v8=reg128#7%top,<v12=reg128#10%top,#1
# asm 2: vext.32 <t4=d15,<v8=d13,<v12=d19,#1
vext.32 d15,d13,d19,#1

# qhasm: 				    4x blockshift unsigned>>= 7
# asm 1: vshr.u32 >blockshift=reg128#5,<blockshift=reg128#5,#7
# asm 2: vshr.u32 >blockshift=q4,<blockshift=q4,#7
vshr.u32 q4,q4,#7

# qhasm:   mem32[c - 68] = x15
# asm 1: str <x15=int32#2,[<c=int32#9,#-68]
# asm 2: str <x15=r1,[<c=r8,#-68]
str r1,[r8,#-68]

# qhasm: 				  t12 = v8[1] v12[0] t12[2,3]      
# asm 1: vext.32 <t12=reg128#11%bot,<v8=reg128#7%bot,<v12=reg128#10%bot,#1
# asm 2: vext.32 <t12=d20,<v8=d12,<v12=d18,#1
vext.32 d20,d12,d18,#1

# qhasm: 				  v8 = (abab & v8) | (~abab & v4)  
# asm 1: vbif <v8=reg128#7,<v4=reg128#6,<abab=reg128#9
# asm 2: vbif <v8=q6,<v4=q5,<abab=q8
vbif q6,q5,q8

# qhasm:   mlenlow = mlenlow_stack
# asm 1: ldr >mlenlow=int32#1,<mlenlow_stack=stack32#5
# asm 2: ldr >mlenlow=r0,<mlenlow_stack=[sp,#240]
ldr r0,[sp,#240]

# qhasm: 				  v4 = t4[1]t4[0]t4[3]t4[2]        
# asm 1: vrev64.i32 >v4=reg128#6,<t4=reg128#8
# asm 2: vrev64.i32 >v4=q5,<t4=q7
vrev64.i32 q5,q7

# qhasm:   mlenhigh = mlenhigh_stack
# asm 1: ldr >mlenhigh=int32#2,<mlenhigh_stack=stack32#6
# asm 2: ldr >mlenhigh=r1,<mlenhigh_stack=[sp,#244]
ldr r1,[sp,#244]

# qhasm: 				  v12 = t12[1]t12[0]t12[3]t12[2]   
# asm 1: vrev64.i32 >v12=reg128#8,<t12=reg128#11
# asm 2: vrev64.i32 >v12=q7,<t12=q10
vrev64.i32 q7,q10

# qhasm: 				  v0 v8 = v0[0] v8[1] v8[0] v0[1]  
# asm 1: vswp <v8=reg128#7%top,<v0=reg128#3%top
# asm 2: vswp <v8=d13,<v0=d5
vswp d13,d5

# qhasm: 				    2x n2n3n3n2 += blockshift
# asm 1: vadd.i64 >n2n3n3n2=reg128#4,<n2n3n3n2=reg128#4,<blockshift=reg128#5
# asm 2: vadd.i64 >n2n3n3n2=q3,<n2n3n3n2=q3,<blockshift=q4
vadd.i64 q3,q3,q4

# qhasm: 				  carry? mlenlow -= 192
# asm 1: subs >mlenlow=int32#1,<mlenlow=int32#1,#192
# asm 2: subs >mlenlow=r0,<mlenlow=r0,#192
subs r0,r0,#192

# qhasm: 				  mlenhigh -= 0 - carry
# asm 1: sbc >mlenhigh=int32#2,<mlenhigh=int32#2,#0
# asm 2: sbc >mlenhigh=r1,<mlenhigh=r1,#0
sbc r1,r1,#0

# qhasm: 				                     =? m - 0
# asm 1: cmp <m=int32#13,#0
# asm 2: cmp <m=r12,#0
cmp r12,#0

# qhasm: 				  goto nomessage21 if =
beq ._nomessage21

# qhasm: 				    m0m1m2m3 = mem128[m]; m += 16
# asm 1: vld1.8 {>m0m1m2m3=reg128#9%bot->m0m1m2m3=reg128#9%top},[<m=int32#13]!
# asm 2: vld1.8 {>m0m1m2m3=d16->m0m1m2m3=d17},[<m=r12]!
vld1.8 {d16-d17},[r12]!

# qhasm: 				    v0 ^= m0m1m2m3
# asm 1: veor >v0=reg128#3,<v0=reg128#3,<m0m1m2m3=reg128#9
# asm 2: veor >v0=q2,<v0=q2,<m0m1m2m3=q8
veor q2,q2,q8

# qhasm: 				    m4m5m6m7 = mem128[m]; m += 16
# asm 1: vld1.8 {>m4m5m6m7=reg128#9%bot->m4m5m6m7=reg128#9%top},[<m=int32#13]!
# asm 2: vld1.8 {>m4m5m6m7=d16->m4m5m6m7=d17},[<m=r12]!
vld1.8 {d16-d17},[r12]!

# qhasm: 				    v4 ^= m4m5m6m7
# asm 1: veor >v4=reg128#6,<v4=reg128#6,<m4m5m6m7=reg128#9
# asm 2: veor >v4=q5,<v4=q5,<m4m5m6m7=q8
veor q5,q5,q8

# qhasm: 				    m8m9m10m11 = mem128[m]; m += 16
# asm 1: vld1.8 {>m8m9m10m11=reg128#9%bot->m8m9m10m11=reg128#9%top},[<m=int32#13]!
# asm 2: vld1.8 {>m8m9m10m11=d16->m8m9m10m11=d17},[<m=r12]!
vld1.8 {d16-d17},[r12]!

# qhasm: 				    v8 ^= m8m9m10m11
# asm 1: veor >v8=reg128#7,<v8=reg128#7,<m8m9m10m11=reg128#9
# asm 2: veor >v8=q6,<v8=q6,<m8m9m10m11=q8
veor q6,q6,q8

# qhasm: 				    m12m13m14m15 = mem128[m]; m += 16
# asm 1: vld1.8 {>m12m13m14m15=reg128#9%bot->m12m13m14m15=reg128#9%top},[<m=int32#13]!
# asm 2: vld1.8 {>m12m13m14m15=d16->m12m13m14m15=d17},[<m=r12]!
vld1.8 {d16-d17},[r12]!

# qhasm: 				    v12 ^= m12m13m14m15
# asm 1: veor >v12=reg128#8,<v12=reg128#8,<m12m13m14m15=reg128#9
# asm 2: veor >v12=q7,<v12=q7,<m12m13m14m15=q8
veor q7,q7,q8

# qhasm: 				  nomessage21:
._nomessage21:

# qhasm: 				  mem128[c] = v0; c += 16
# asm 1: vst1.8 {<v0=reg128#3%bot-<v0=reg128#3%top},[<c=int32#9]!
# asm 2: vst1.8 {<v0=d4-<v0=d5},[<c=r8]!
vst1.8 {d4-d5},[r8]!

# qhasm: 				  mem128[c] = v4; c += 16
# asm 1: vst1.8 {<v4=reg128#6%bot-<v4=reg128#6%top},[<c=int32#9]!
# asm 2: vst1.8 {<v4=d10-<v4=d11},[<c=r8]!
vst1.8 {d10-d11},[r8]!

# qhasm: 				  mem128[c] = v8; c += 16
# asm 1: vst1.8 {<v8=reg128#7%bot-<v8=reg128#7%top},[<c=int32#9]!
# asm 2: vst1.8 {<v8=d12-<v8=d13},[<c=r8]!
vst1.8 {d12-d13},[r8]!

# qhasm: 				  mem128[c] = v12; c += 16
# asm 1: vst1.8 {<v12=reg128#8%bot-<v12=reg128#8%top},[<c=int32#9]!
# asm 2: vst1.8 {<v12=d14-<v12=d15},[<c=r8]!
vst1.8 {d14-d15},[r8]!

# qhasm:   m_stack = m
# asm 1: str <m=int32#13,>m_stack=stack32#4
# asm 2: str <m=r12,>m_stack=[sp,#236]
str r12,[sp,#236]

# qhasm: 				    ptr_k1n1k7k2 = &k1n1k7k2_stack
# asm 1: lea >ptr_k1n1k7k2=int32#14,<k1n1k7k2_stack=stack128#2
# asm 2: lea >ptr_k1n1k7k2=r14,<k1n1k7k2_stack=[sp,#272]
add r14,sp,#272

# qhasm: 				    ptr_k2k3k6k7 = &k2k3k6k7_stack
# asm 1: lea >ptr_k2k3k6k7=int32#13,<k2k3k6k7_stack=stack128#1
# asm 2: lea >ptr_k2k3k6k7=r12,<k2k3k6k7_stack=[sp,#256]
add r12,sp,#256

# qhasm:   c_stack = c
# asm 1: str <c=int32#9,>c_stack=stack32#3
# asm 2: str <c=r8,>c_stack=[sp,#232]
str r8,[sp,#232]

# qhasm:                         unsigned<? mlenlow - 192
# asm 1: cmp <mlenlow=int32#1,#192
# asm 2: cmp <mlenlow=r0,#192
cmp r0,#192

# qhasm: goto mlenatleast192 if !unsigned<
bhs ._mlenatleast192

# qhasm: mlenlowbelow192:
._mlenlowbelow192:

# qhasm:                        unsigned>? mlenhigh - 0
# asm 1: cmp <mlenhigh=int32#2,#0
# asm 2: cmp <mlenhigh=r1,#0
cmp r1,#0

# qhasm: goto mlenatleast192 if unsigned>
bhi ._mlenatleast192

# qhasm:              =? mlenlow - 0
# asm 1: cmp <mlenlow=int32#1,#0
# asm 2: cmp <mlenlow=r0,#0
cmp r0,#0

# qhasm: goto done if =
beq ._done

# qhasm: goto mlenatleast1
b ._mlenatleast1

# qhasm: nextblock:
._nextblock:

# qhasm:   carry? mlenlow -= 64
# asm 1: subs >mlenlow=int32#1,<mlenlow=int32#1,#64
# asm 2: subs >mlenlow=r0,<mlenlow=r0,#64
subs r0,r0,#64

# qhasm:   mlenhigh -= 0 - carry
# asm 1: sbc >mlenhigh=int32#2,<mlenhigh=int32#2,#0
# asm 2: sbc >mlenhigh=r1,<mlenhigh=r1,#0
sbc r1,r1,#0

# qhasm: mlenatleast1:
._mlenatleast1:

# qhasm:                        =? mlenhigh - 0
# asm 1: cmp <mlenhigh=int32#2,#0
# asm 2: cmp <mlenhigh=r1,#0
cmp r1,#0

# qhasm:   goto handleblock if !=
bne ._handleblock

# qhasm:                        unsigned<? mlenlow - 64
# asm 1: cmp <mlenlow=int32#1,#64
# asm 2: cmp <mlenlow=r0,#64
cmp r0,#64

# qhasm:   goto handleblock if !unsigned<
bhs ._handleblock

# qhasm:     c = c_stack
# asm 1: ldr >c=int32#3,<c_stack=stack32#3
# asm 2: ldr >c=r2,<c_stack=[sp,#232]
ldr r2,[sp,#232]

# qhasm:     m = m_stack
# asm 1: ldr >m=int32#4,<m_stack=stack32#4
# asm 2: ldr >m=r3,<m_stack=[sp,#236]
ldr r3,[sp,#236]

# qhasm:     c_original = c
# asm 1: str <c=int32#3,>c_original=stack32#5
# asm 2: str <c=r2,>c_original=[sp,#240]
str r2,[sp,#240]

# qhasm:     c = &tmp
# asm 1: lea >c=int32#3,<tmp=stack512#1
# asm 2: lea >c=r2,<tmp=[sp,#160]
add r2,sp,#160

# qhasm:     c_stack = c
# asm 1: str <c=int32#3,>c_stack=stack32#3
# asm 2: str <c=r2,>c_stack=[sp,#232]
str r2,[sp,#232]

# qhasm:                         =? m - 0
# asm 1: cmp <m=int32#4,#0
# asm 2: cmp <m=r3,#0
cmp r3,#0

# qhasm:     goto handleblock if =
beq ._handleblock

# qhasm:     m_stack = c
# asm 1: str <c=int32#3,>m_stack=stack32#4
# asm 2: str <c=r2,>m_stack=[sp,#236]
str r2,[sp,#236]

# qhasm:     i = 0
# asm 1: ldr >i=int32#5,=0
# asm 2: ldr >i=r4,=0
ldr r4,=0

# qhasm:     mcopy:
._mcopy:

# qhasm:       mi = mem8[m]; m += 1
# asm 1: ldrb >mi=int32#6,[<m=int32#4],#1
# asm 2: ldrb >mi=r5,[<m=r3],#1
ldrb r5,[r3],#1

# qhasm:       i += 1
# asm 1: add >i=int32#5,<i=int32#5,#1
# asm 2: add >i=r4,<i=r4,#1
add r4,r4,#1

# qhasm:       mem8[c] = mi; c += 1
# asm 1: strb <mi=int32#6,[<c=int32#3],#1
# asm 2: strb <mi=r5,[<c=r2],#1
strb r5,[r2],#1

# qhasm:                   unsigned<? i - mlenlow
# asm 1: cmp <i=int32#5,<mlenlow=int32#1
# asm 2: cmp <i=r4,<mlenlow=r0
cmp r4,r0

# qhasm:     goto mcopy if unsigned<
blo ._mcopy

# qhasm:   handleblock:
._handleblock:

# qhasm:   mlenlow_stack = mlenlow
# asm 1: str <mlenlow=int32#1,>mlenlow_stack=stack32#6
# asm 2: str <mlenlow=r0,>mlenlow_stack=[sp,#244]
str r0,[sp,#244]

# qhasm:   mlenhigh_stack = mlenhigh
# asm 1: str <mlenhigh=int32#2,>mlenhigh_stack=stack32#7
# asm 2: str <mlenhigh=r1,>mlenhigh_stack=[sp,#248]
str r1,[sp,#248]

# qhasm:   assign r2 r3 to x2 x3 = j23_stack
# asm 1: ldrd >x2=int32#3,<j23_stack=stack64#7
# asm 2: ldrd >x2=r2,<j23_stack=[sp,#48]
ldrd r2,[sp,#48]

# qhasm:   assign r6 r7 to x6 x7 = j67_stack
# asm 1: ldrd >x6=int32#7,<j67_stack=stack64#5
# asm 2: ldrd >x6=r6,<j67_stack=[sp,#32]
ldrd r6,[sp,#32]

# qhasm:   assign r0 r1 to x0 x1 = j01_stack
# asm 1: ldrd >x0=int32#1,<j01_stack=stack64#9
# asm 2: ldrd >x0=r0,<j01_stack=[sp,#64]
ldrd r0,[sp,#64]

# qhasm:   assign r4 r5 to x4 x5 = j45_stack
# asm 1: ldrd >x4=int32#5,<j45_stack=stack64#10
# asm 2: ldrd >x4=r4,<j45_stack=[sp,#72]
ldrd r4,[sp,#72]

# qhasm:   assign r10 r11 to x10 x11 = j1011_stack
# asm 1: ldrd >x10=int32#11,<j1011_stack=stack64#11
# asm 2: ldrd >x10=r10,<j1011_stack=[sp,#80]
ldrd r10,[sp,#80]

# qhasm:   assign r8 r9 to x14 x15 = j1415_stack
# asm 1: ldrd >x14=int32#9,<j1415_stack=stack64#12
# asm 2: ldrd >x14=r8,<j1415_stack=[sp,#88]
ldrd r8,[sp,#88]

# qhasm:   assign r2 r3 to x2 x3; x23_stack = x2 x3
# asm 1: strd <x2=int32#3,>x23_stack=stack64#13
# asm 2: strd <x2=r2,>x23_stack=[sp,#96]
strd r2,[sp,#96]

# qhasm:   assign r6 r7 to x6 x7; x67_stack = x6 x7
# asm 1: strd <x6=int32#7,>x67_stack=stack64#14
# asm 2: strd <x6=r6,>x67_stack=[sp,#104]
strd r6,[sp,#104]

# qhasm:   assign r2 r3 to x12 x13 = j1213_stack
# asm 1: ldrd >x12=int32#3,<j1213_stack=stack64#8
# asm 2: ldrd >x12=r2,<j1213_stack=[sp,#56]
ldrd r2,[sp,#56]

# qhasm:   assign r6 r7 to x8 x9 = j89_stack
# asm 1: ldrd >x8=int32#7,<j89_stack=stack64#6
# asm 2: ldrd >x8=r6,<j89_stack=[sp,#40]
ldrd r6,[sp,#40]

# qhasm:   i = 20
# asm 1: ldr >i=int32#13,=20
# asm 2: ldr >i=r12,=20
ldr r12,=20

# qhasm:   mainloop1:
._mainloop1:

# qhasm:     i_stack = i
# asm 1: str <i=int32#13,>i_stack=stack32#8
# asm 2: str <i=r12,>i_stack=[sp,#252]
str r12,[sp,#252]

# qhasm:        s4 = x0 + x12
# asm 1: add >s4=int32#13,<x0=int32#1,<x12=int32#3
# asm 2: add >s4=r12,<x0=r0,<x12=r2
add r12,r0,r2

# qhasm:          s9 = x5 + x1
# asm 1: add >s9=int32#14,<x5=int32#6,<x1=int32#2
# asm 2: add >s9=r14,<x5=r5,<x1=r1
add r14,r5,r1

# qhasm:        x4 ^= (s4 >>> 25)
# asm 1: eor >x4=int32#5,<x4=int32#5,<s4=int32#13,ROR #25
# asm 2: eor >x4=r4,<x4=r4,<s4=r12,ROR #25
eor r4,r4,r12,ROR #25

# qhasm:          x9 ^= (s9 >>> 25)
# asm 1: eor >x9=int32#8,<x9=int32#8,<s9=int32#14,ROR #25
# asm 2: eor >x9=r7,<x9=r7,<s9=r14,ROR #25
eor r7,r7,r14,ROR #25

# qhasm:        s8 = x4 + x0
# asm 1: add >s8=int32#13,<x4=int32#5,<x0=int32#1
# asm 2: add >s8=r12,<x4=r4,<x0=r0
add r12,r4,r0

# qhasm:          s13 = x9 + x5
# asm 1: add >s13=int32#14,<x9=int32#8,<x5=int32#6
# asm 2: add >s13=r14,<x9=r7,<x5=r5
add r14,r7,r5

# qhasm:        x8 ^= (s8 >>> 23)
# asm 1: eor >x8=int32#7,<x8=int32#7,<s8=int32#13,ROR #23
# asm 2: eor >x8=r6,<x8=r6,<s8=r12,ROR #23
eor r6,r6,r12,ROR #23

# qhasm:         x13 ^= (s13 >>> 23)
# asm 1: eor >x13=int32#4,<x13=int32#4,<s13=int32#14,ROR #23
# asm 2: eor >x13=r3,<x13=r3,<s13=r14,ROR #23
eor r3,r3,r14,ROR #23

# qhasm:       s12 = x8 + x4
# asm 1: add >s12=int32#13,<x8=int32#7,<x4=int32#5
# asm 2: add >s12=r12,<x8=r6,<x4=r4
add r12,r6,r4

# qhasm: mem32[&x89_stack+4] = x9
# asm 1: str <x9=int32#8,<x89_stack=stack64#17
# asm 2: str <x9=r7,<x89_stack=[sp,#132]
str r7,[sp,#132]

# qhasm:          s1 = x13 + x9
# asm 1: add >s1=int32#8,<x13=int32#4,<x9=int32#8
# asm 2: add >s1=r7,<x13=r3,<x9=r7
add r7,r3,r7

# qhasm: x6 = mem32[&x67_stack]
# asm 1: ldr >x6=int32#14,<x67_stack=stack64#14
# asm 2: ldr >x6=r14,<x67_stack=[sp,#104]
ldr r14,[sp,#104]

# qhasm:       x12 ^= (s12 >>> 19)
# asm 1: eor >x12=int32#3,<x12=int32#3,<s12=int32#13,ROR #19
# asm 2: eor >x12=r2,<x12=r2,<s12=r12,ROR #19
eor r2,r2,r12,ROR #19

# qhasm: mem32[&x89_stack] = x8
# asm 1: str <x8=int32#7,<x89_stack=stack64#17
# asm 2: str <x8=r6,<x89_stack=[sp,#128]
str r6,[sp,#128]

# qhasm:          x1 ^= (s1 >>> 19)
# asm 1: eor >x1=int32#2,<x1=int32#2,<s1=int32#8,ROR #19
# asm 2: eor >x1=r1,<x1=r1,<s1=r7,ROR #19
eor r1,r1,r7,ROR #19

# qhasm: x3 = mem32[&x23_stack+4]
# asm 1: ldr >x3=int32#8,<x23_stack=stack64#13
# asm 2: ldr >x3=r7,<x23_stack=[sp,#100]
ldr r7,[sp,#100]

# qhasm:        s0 = x12 + x8
# asm 1: add >s0=int32#7,<x12=int32#3,<x8=int32#7
# asm 2: add >s0=r6,<x12=r2,<x8=r6
add r6,r2,r6

# qhasm: mem32[&x1213_stack] = x12
# asm 1: str <x12=int32#3,<x1213_stack=stack64#16
# asm 2: str <x12=r2,<x1213_stack=[sp,#120]
str r2,[sp,#120]

# qhasm:          s5 = x1 + x13
# asm 1: add >s5=int32#3,<x1=int32#2,<x13=int32#4
# asm 2: add >s5=r2,<x1=r1,<x13=r3
add r2,r1,r3

# qhasm: x2 = mem32[&x23_stack]
# asm 1: ldr >x2=int32#13,<x23_stack=stack64#13
# asm 2: ldr >x2=r12,<x23_stack=[sp,#96]
ldr r12,[sp,#96]

# qhasm:        x0 ^= (s0 >>> 14)
# asm 1: eor >x0=int32#1,<x0=int32#1,<s0=int32#7,ROR #14
# asm 2: eor >x0=r0,<x0=r0,<s0=r6,ROR #14
eor r0,r0,r6,ROR #14

# qhasm: mem32[&x1213_stack+4] = x13
# asm 1: str <x13=int32#4,<x1213_stack=stack64#16
# asm 2: str <x13=r3,<x1213_stack=[sp,#124]
str r3,[sp,#124]

# qhasm:          x5 ^= (s5 >>> 14)
# asm 1: eor >x5=int32#3,<x5=int32#6,<s5=int32#3,ROR #14
# asm 2: eor >x5=r2,<x5=r5,<s5=r2,ROR #14
eor r2,r5,r2,ROR #14

# qhasm: x7 = mem32[&x67_stack+4]
# asm 1: ldr >x7=int32#4,<x67_stack=stack64#14
# asm 2: ldr >x7=r3,<x67_stack=[sp,#108]
ldr r3,[sp,#108]

# qhasm:           s14 = x10 + x6
# asm 1: add >s14=int32#6,<x10=int32#11,<x6=int32#14
# asm 2: add >s14=r5,<x10=r10,<x6=r14
add r5,r10,r14

# qhasm:              s3 = x15 + x11
# asm 1: add >s3=int32#7,<x15=int32#10,<x11=int32#12
# asm 2: add >s3=r6,<x15=r9,<x11=r11
add r6,r9,r11

# qhasm:           x14 ^= (s14 >>> 25)
# asm 1: eor >x14=int32#9,<x14=int32#9,<s14=int32#6,ROR #25
# asm 2: eor >x14=r8,<x14=r8,<s14=r5,ROR #25
eor r8,r8,r5,ROR #25

# qhasm:              x3 ^= (s3 >>> 25)
# asm 1: eor >x3=int32#6,<x3=int32#8,<s3=int32#7,ROR #25
# asm 2: eor >x3=r5,<x3=r7,<s3=r6,ROR #25
eor r5,r7,r6,ROR #25

# qhasm:            s2 = x14 + x10
# asm 1: add >s2=int32#7,<x14=int32#9,<x10=int32#11
# asm 2: add >s2=r6,<x14=r8,<x10=r10
add r6,r8,r10

# qhasm:              s7 = x3 + x15
# asm 1: add >s7=int32#8,<x3=int32#6,<x15=int32#10
# asm 2: add >s7=r7,<x3=r5,<x15=r9
add r7,r5,r9

# qhasm:            x2 ^= (s2 >>> 23)
# asm 1: eor >x2=int32#7,<x2=int32#13,<s2=int32#7,ROR #23
# asm 2: eor >x2=r6,<x2=r12,<s2=r6,ROR #23
eor r6,r12,r6,ROR #23

# qhasm:              x7 ^= (s7 >>> 23)
# asm 1: eor >x7=int32#4,<x7=int32#4,<s7=int32#8,ROR #23
# asm 2: eor >x7=r3,<x7=r3,<s7=r7,ROR #23
eor r3,r3,r7,ROR #23

# qhasm:            s6 = x2 + x14
# asm 1: add >s6=int32#8,<x2=int32#7,<x14=int32#9
# asm 2: add >s6=r7,<x2=r6,<x14=r8
add r7,r6,r8

# qhasm:             s11 = x7 + x3
# asm 1: add >s11=int32#13,<x7=int32#4,<x3=int32#6
# asm 2: add >s11=r12,<x7=r3,<x3=r5
add r12,r3,r5

# qhasm:            x6 ^= (s6 >>> 19)
# asm 1: eor >x6=int32#8,<x6=int32#14,<s6=int32#8,ROR #19
# asm 2: eor >x6=r7,<x6=r14,<s6=r7,ROR #19
eor r7,r14,r7,ROR #19

# qhasm:             x11 ^= (s11 >>> 19)
# asm 1: eor >x11=int32#12,<x11=int32#12,<s11=int32#13,ROR #19
# asm 2: eor >x11=r11,<x11=r11,<s11=r12,ROR #19
eor r11,r11,r12,ROR #19

# qhasm:           s10 = x6 + x2
# asm 1: add >s10=int32#13,<x6=int32#8,<x2=int32#7
# asm 2: add >s10=r12,<x6=r7,<x2=r6
add r12,r7,r6

# qhasm:             s15 = x11 + x7
# asm 1: add >s15=int32#14,<x11=int32#12,<x7=int32#4
# asm 2: add >s15=r14,<x11=r11,<x7=r3
add r14,r11,r3

# qhasm:           x10 ^= (s10 >>> 14)
# asm 1: eor >x10=int32#11,<x10=int32#11,<s10=int32#13,ROR #14
# asm 2: eor >x10=r10,<x10=r10,<s10=r12,ROR #14
eor r10,r10,r12,ROR #14

# qhasm:             x15 ^= (s15 >>> 14)
# asm 1: eor >x15=int32#10,<x15=int32#10,<s15=int32#14,ROR #14
# asm 2: eor >x15=r9,<x15=r9,<s15=r14,ROR #14
eor r9,r9,r14,ROR #14

# qhasm:        s1 = x0 + x3
# asm 1: add >s1=int32#13,<x0=int32#1,<x3=int32#6
# asm 2: add >s1=r12,<x0=r0,<x3=r5
add r12,r0,r5

# qhasm:          s6 = x5 + x4
# asm 1: add >s6=int32#14,<x5=int32#3,<x4=int32#5
# asm 2: add >s6=r14,<x5=r2,<x4=r4
add r14,r2,r4

# qhasm:        x1 ^= (s1 >>> 25)
# asm 1: eor >x1=int32#2,<x1=int32#2,<s1=int32#13,ROR #25
# asm 2: eor >x1=r1,<x1=r1,<s1=r12,ROR #25
eor r1,r1,r12,ROR #25

# qhasm:          x6 ^= (s6 >>> 25)
# asm 1: eor >x6=int32#8,<x6=int32#8,<s6=int32#14,ROR #25
# asm 2: eor >x6=r7,<x6=r7,<s6=r14,ROR #25
eor r7,r7,r14,ROR #25

# qhasm:        s2 = x1 + x0
# asm 1: add >s2=int32#13,<x1=int32#2,<x0=int32#1
# asm 2: add >s2=r12,<x1=r1,<x0=r0
add r12,r1,r0

# qhasm:          s7 = x6 + x5
# asm 1: add >s7=int32#14,<x6=int32#8,<x5=int32#3
# asm 2: add >s7=r14,<x6=r7,<x5=r2
add r14,r7,r2

# qhasm:        x2 ^= (s2 >>> 23)
# asm 1: eor >x2=int32#7,<x2=int32#7,<s2=int32#13,ROR #23
# asm 2: eor >x2=r6,<x2=r6,<s2=r12,ROR #23
eor r6,r6,r12,ROR #23

# qhasm:          x7 ^= (s7 >>> 23)
# asm 1: eor >x7=int32#4,<x7=int32#4,<s7=int32#14,ROR #23
# asm 2: eor >x7=r3,<x7=r3,<s7=r14,ROR #23
eor r3,r3,r14,ROR #23

# qhasm:        s3 = x2 + x1
# asm 1: add >s3=int32#13,<x2=int32#7,<x1=int32#2
# asm 2: add >s3=r12,<x2=r6,<x1=r1
add r12,r6,r1

# qhasm: mem32[&x67_stack] = x6
# asm 1: str <x6=int32#8,<x67_stack=stack64#14
# asm 2: str <x6=r7,<x67_stack=[sp,#104]
str r7,[sp,#104]

# qhasm:          s4 = x7 + x6
# asm 1: add >s4=int32#8,<x7=int32#4,<x6=int32#8
# asm 2: add >s4=r7,<x7=r3,<x6=r7
add r7,r3,r7

# qhasm: x8 = mem32[&x89_stack]
# asm 1: ldr >x8=int32#14,<x89_stack=stack64#17
# asm 2: ldr >x8=r14,<x89_stack=[sp,#128]
ldr r14,[sp,#128]

# qhasm:        x3 ^= (s3 >>> 19)
# asm 1: eor >x3=int32#6,<x3=int32#6,<s3=int32#13,ROR #19
# asm 2: eor >x3=r5,<x3=r5,<s3=r12,ROR #19
eor r5,r5,r12,ROR #19

# qhasm: mem32[&x67_stack+4] = x7
# asm 1: str <x7=int32#4,<x67_stack=stack64#14
# asm 2: str <x7=r3,<x67_stack=[sp,#108]
str r3,[sp,#108]

# qhasm:          x4 ^= (s4 >>> 19)
# asm 1: eor >x4=int32#5,<x4=int32#5,<s4=int32#8,ROR #19
# asm 2: eor >x4=r4,<x4=r4,<s4=r7,ROR #19
eor r4,r4,r7,ROR #19

# qhasm: x9 = mem32[&x89_stack+4]
# asm 1: ldr >x9=int32#8,<x89_stack=stack64#17
# asm 2: ldr >x9=r7,<x89_stack=[sp,#132]
ldr r7,[sp,#132]

# qhasm:        s0 = x3 + x2
# asm 1: add >s0=int32#13,<x3=int32#6,<x2=int32#7
# asm 2: add >s0=r12,<x3=r5,<x2=r6
add r12,r5,r6

# qhasm: mem32[&x23_stack] = x2
# asm 1: str <x2=int32#7,<x23_stack=stack64#13
# asm 2: str <x2=r6,<x23_stack=[sp,#96]
str r6,[sp,#96]

# qhasm:          s5 = x4 + x7
# asm 1: add >s5=int32#4,<x4=int32#5,<x7=int32#4
# asm 2: add >s5=r3,<x4=r4,<x7=r3
add r3,r4,r3

# qhasm: x12 = mem32[&x1213_stack]
# asm 1: ldr >x12=int32#7,<x1213_stack=stack64#16
# asm 2: ldr >x12=r6,<x1213_stack=[sp,#120]
ldr r6,[sp,#120]

# qhasm:        x0 ^= (s0 >>> 14)
# asm 1: eor >x0=int32#1,<x0=int32#1,<s0=int32#13,ROR #14
# asm 2: eor >x0=r0,<x0=r0,<s0=r12,ROR #14
eor r0,r0,r12,ROR #14

# qhasm: mem32[&x23_stack+4] = x3
# asm 1: str <x3=int32#6,<x23_stack=stack64#13
# asm 2: str <x3=r5,<x23_stack=[sp,#100]
str r5,[sp,#100]

# qhasm:          x5 ^= (s5 >>> 14)
# asm 1: eor >x5=int32#6,<x5=int32#3,<s5=int32#4,ROR #14
# asm 2: eor >x5=r5,<x5=r2,<s5=r3,ROR #14
eor r5,r2,r3,ROR #14

# qhasm: x13 = mem32[&x1213_stack+4]
# asm 1: ldr >x13=int32#4,<x1213_stack=stack64#16
# asm 2: ldr >x13=r3,<x1213_stack=[sp,#124]
ldr r3,[sp,#124]

# qhasm:           s11 = x10 + x9
# asm 1: add >s11=int32#3,<x10=int32#11,<x9=int32#8
# asm 2: add >s11=r2,<x10=r10,<x9=r7
add r2,r10,r7

# qhasm:             s12 = x15 + x14
# asm 1: add >s12=int32#13,<x15=int32#10,<x14=int32#9
# asm 2: add >s12=r12,<x15=r9,<x14=r8
add r12,r9,r8

# qhasm:           x11 ^= (s11 >>> 25)
# asm 1: eor >x11=int32#12,<x11=int32#12,<s11=int32#3,ROR #25
# asm 2: eor >x11=r11,<x11=r11,<s11=r2,ROR #25
eor r11,r11,r2,ROR #25

# qhasm:             x12 ^= (s12 >>> 25)
# asm 1: eor >x12=int32#3,<x12=int32#7,<s12=int32#13,ROR #25
# asm 2: eor >x12=r2,<x12=r6,<s12=r12,ROR #25
eor r2,r6,r12,ROR #25

# qhasm:            s8 = x11 + x10
# asm 1: add >s8=int32#7,<x11=int32#12,<x10=int32#11
# asm 2: add >s8=r6,<x11=r11,<x10=r10
add r6,r11,r10

# qhasm:             s13 = x12 + x15
# asm 1: add >s13=int32#13,<x12=int32#3,<x15=int32#10
# asm 2: add >s13=r12,<x12=r2,<x15=r9
add r12,r2,r9

# qhasm:            x8 ^= (s8 >>> 23)
# asm 1: eor >x8=int32#7,<x8=int32#14,<s8=int32#7,ROR #23
# asm 2: eor >x8=r6,<x8=r14,<s8=r6,ROR #23
eor r6,r14,r6,ROR #23

# qhasm:             x13 ^= (s13 >>> 23)
# asm 1: eor >x13=int32#4,<x13=int32#4,<s13=int32#13,ROR #23
# asm 2: eor >x13=r3,<x13=r3,<s13=r12,ROR #23
eor r3,r3,r12,ROR #23

# qhasm:            s9 = x8 + x11
# asm 1: add >s9=int32#13,<x8=int32#7,<x11=int32#12
# asm 2: add >s9=r12,<x8=r6,<x11=r11
add r12,r6,r11

# qhasm:             s14 = x13 + x12
# asm 1: add >s14=int32#14,<x13=int32#4,<x12=int32#3
# asm 2: add >s14=r14,<x13=r3,<x12=r2
add r14,r3,r2

# qhasm:            x9 ^= (s9 >>> 19)
# asm 1: eor >x9=int32#8,<x9=int32#8,<s9=int32#13,ROR #19
# asm 2: eor >x9=r7,<x9=r7,<s9=r12,ROR #19
eor r7,r7,r12,ROR #19

# qhasm:             x14 ^= (s14 >>> 19)
# asm 1: eor >x14=int32#9,<x14=int32#9,<s14=int32#14,ROR #19
# asm 2: eor >x14=r8,<x14=r8,<s14=r14,ROR #19
eor r8,r8,r14,ROR #19

# qhasm:           s10 = x9 + x8
# asm 1: add >s10=int32#13,<x9=int32#8,<x8=int32#7
# asm 2: add >s10=r12,<x9=r7,<x8=r6
add r12,r7,r6

# qhasm:             s15 = x14 + x13
# asm 1: add >s15=int32#14,<x14=int32#9,<x13=int32#4
# asm 2: add >s15=r14,<x14=r8,<x13=r3
add r14,r8,r3

# qhasm:           x10 ^= (s10 >>> 14)
# asm 1: eor >x10=int32#11,<x10=int32#11,<s10=int32#13,ROR #14
# asm 2: eor >x10=r10,<x10=r10,<s10=r12,ROR #14
eor r10,r10,r12,ROR #14

# qhasm:             x15 ^= (s15 >>> 14)
# asm 1: eor >x15=int32#10,<x15=int32#10,<s15=int32#14,ROR #14
# asm 2: eor >x15=r9,<x15=r9,<s15=r14,ROR #14
eor r9,r9,r14,ROR #14

# qhasm:     i = i_stack
# asm 1: ldr >i=int32#13,<i_stack=stack32#8
# asm 2: ldr >i=r12,<i_stack=[sp,#252]
ldr r12,[sp,#252]

# qhasm:                     unsigned>? i -= 2
# asm 1: subs >i=int32#13,<i=int32#13,#2
# asm 2: subs >i=r12,<i=r12,#2
subs r12,r12,#2

# qhasm:   goto mainloop1 if unsigned>
bhi ._mainloop1

# qhasm:   assign r6 r7 to x8 x9; x89_stack = x8 x9
# asm 1: strd <x8=int32#7,>x89_stack=stack64#17
# asm 2: strd <x8=r6,>x89_stack=[sp,#128]
strd r6,[sp,#128]

# qhasm:   assign r2 r3 to x12 x13; x1213_stack = x12 x13
# asm 1: strd <x12=int32#3,>x1213_stack=stack64#16
# asm 2: strd <x12=r2,>x1213_stack=[sp,#120]
strd r2,[sp,#120]

# qhasm:   assign r10 r11 to x10 x11; x1011_stack = x10 x11
# asm 1: strd <x10=int32#11,>x1011_stack=stack64#15
# asm 2: strd <x10=r10,>x1011_stack=[sp,#112]
strd r10,[sp,#112]

# qhasm:   assign r8 r9 to x14 x15; x1415_stack = x14 x15
# asm 1: strd <x14=int32#9,>x1415_stack=stack64#18
# asm 2: strd <x14=r8,>x1415_stack=[sp,#136]
strd r8,[sp,#136]

# qhasm:   assign r2 r3 to x2 x3 = x23_stack
# asm 1: ldrd >x2=int32#3,<x23_stack=stack64#13
# asm 2: ldrd >x2=r2,<x23_stack=[sp,#96]
ldrd r2,[sp,#96]

# qhasm:   assign r6 r7 to x6 x7 = x67_stack
# asm 1: ldrd >x6=int32#7,<x67_stack=stack64#14
# asm 2: ldrd >x6=r6,<x67_stack=[sp,#104]
ldrd r6,[sp,#104]

# qhasm:   assign r8 r9 to j0 j1 = j01_stack
# asm 1: ldrd >j0=int32#9,<j01_stack=stack64#9
# asm 2: ldrd >j0=r8,<j01_stack=[sp,#64]
ldrd r8,[sp,#64]

# qhasm:   assign r10 r11 to j2 j3 = j23_stack
# asm 1: ldrd >j2=int32#11,<j23_stack=stack64#7
# asm 2: ldrd >j2=r10,<j23_stack=[sp,#48]
ldrd r10,[sp,#48]

# qhasm:   x0 += j0
# asm 1: add >x0=int32#1,<x0=int32#1,<j0=int32#9
# asm 2: add >x0=r0,<x0=r0,<j0=r8
add r0,r0,r8

# qhasm:   x1 += j1
# asm 1: add >x1=int32#2,<x1=int32#2,<j1=int32#10
# asm 2: add >x1=r1,<x1=r1,<j1=r9
add r1,r1,r9

# qhasm:   x2 += j2
# asm 1: add >x2=int32#3,<x2=int32#3,<j2=int32#11
# asm 2: add >x2=r2,<x2=r2,<j2=r10
add r2,r2,r10

# qhasm:   x3 += j3
# asm 1: add >x3=int32#4,<x3=int32#4,<j3=int32#12
# asm 2: add >x3=r3,<x3=r3,<j3=r11
add r3,r3,r11

# qhasm:   assign r8 r9 to j4 j5 = j45_stack
# asm 1: ldrd >j4=int32#9,<j45_stack=stack64#10
# asm 2: ldrd >j4=r8,<j45_stack=[sp,#72]
ldrd r8,[sp,#72]

# qhasm:   assign r10 r11 to j6 j7 = j67_stack
# asm 1: ldrd >j6=int32#11,<j67_stack=stack64#5
# asm 2: ldrd >j6=r10,<j67_stack=[sp,#32]
ldrd r10,[sp,#32]

# qhasm:   x4 += j4
# asm 1: add >x4=int32#5,<x4=int32#5,<j4=int32#9
# asm 2: add >x4=r4,<x4=r4,<j4=r8
add r4,r4,r8

# qhasm:   x5 += j5
# asm 1: add >x5=int32#6,<x5=int32#6,<j5=int32#10
# asm 2: add >x5=r5,<x5=r5,<j5=r9
add r5,r5,r9

# qhasm:   x6 += j6
# asm 1: add >x6=int32#7,<x6=int32#7,<j6=int32#11
# asm 2: add >x6=r6,<x6=r6,<j6=r10
add r6,r6,r10

# qhasm:   x7 += j7
# asm 1: add >x7=int32#8,<x7=int32#8,<j7=int32#12
# asm 2: add >x7=r7,<x7=r7,<j7=r11
add r7,r7,r11

# qhasm:   m = m_stack
# asm 1: ldr >m=int32#13,<m_stack=stack32#4
# asm 2: ldr >m=r12,<m_stack=[sp,#236]
ldr r12,[sp,#236]

# qhasm:                      =? m - 0
# asm 1: cmp <m=int32#13,#0
# asm 2: cmp <m=r12,#0
cmp r12,#0

# qhasm:   goto nomessage10 if =
beq ._nomessage10

# qhasm:       m0 = mem32[m + 0]
# asm 1: ldr >m0=int32#9,[<m=int32#13,#0]
# asm 2: ldr >m0=r8,[<m=r12,#0]
ldr r8,[r12,#0]

# qhasm:       m1 = mem32[m + 4]
# asm 1: ldr >m1=int32#10,[<m=int32#13,#4]
# asm 2: ldr >m1=r9,[<m=r12,#4]
ldr r9,[r12,#4]

# qhasm:       m2 = mem32[m + 8]
# asm 1: ldr >m2=int32#11,[<m=int32#13,#8]
# asm 2: ldr >m2=r10,[<m=r12,#8]
ldr r10,[r12,#8]

# qhasm:       m3 = mem32[m + 12]
# asm 1: ldr >m3=int32#12,[<m=int32#13,#12]
# asm 2: ldr >m3=r11,[<m=r12,#12]
ldr r11,[r12,#12]

# qhasm:     x0 ^= m0
# asm 1: eor >x0=int32#1,<x0=int32#1,<m0=int32#9
# asm 2: eor >x0=r0,<x0=r0,<m0=r8
eor r0,r0,r8

# qhasm:       m4 = mem32[m + 16]
# asm 1: ldr >m4=int32#9,[<m=int32#13,#16]
# asm 2: ldr >m4=r8,[<m=r12,#16]
ldr r8,[r12,#16]

# qhasm:     x1 ^= m1
# asm 1: eor >x1=int32#2,<x1=int32#2,<m1=int32#10
# asm 2: eor >x1=r1,<x1=r1,<m1=r9
eor r1,r1,r9

# qhasm:       m5 = mem32[m + 20]
# asm 1: ldr >m5=int32#10,[<m=int32#13,#20]
# asm 2: ldr >m5=r9,[<m=r12,#20]
ldr r9,[r12,#20]

# qhasm:     x2 ^= m2
# asm 1: eor >x2=int32#3,<x2=int32#3,<m2=int32#11
# asm 2: eor >x2=r2,<x2=r2,<m2=r10
eor r2,r2,r10

# qhasm:       m6 = mem32[m + 24]
# asm 1: ldr >m6=int32#11,[<m=int32#13,#24]
# asm 2: ldr >m6=r10,[<m=r12,#24]
ldr r10,[r12,#24]

# qhasm:     x3 ^= m3
# asm 1: eor >x3=int32#4,<x3=int32#4,<m3=int32#12
# asm 2: eor >x3=r3,<x3=r3,<m3=r11
eor r3,r3,r11

# qhasm:       m7 = mem32[m + 28]
# asm 1: ldr >m7=int32#12,[<m=int32#13,#28]
# asm 2: ldr >m7=r11,[<m=r12,#28]
ldr r11,[r12,#28]

# qhasm:     x4 ^= m4
# asm 1: eor >x4=int32#5,<x4=int32#5,<m4=int32#9
# asm 2: eor >x4=r4,<x4=r4,<m4=r8
eor r4,r4,r8

# qhasm:     x5 ^= m5
# asm 1: eor >x5=int32#6,<x5=int32#6,<m5=int32#10
# asm 2: eor >x5=r5,<x5=r5,<m5=r9
eor r5,r5,r9

# qhasm:     x6 ^= m6
# asm 1: eor >x6=int32#7,<x6=int32#7,<m6=int32#11
# asm 2: eor >x6=r6,<x6=r6,<m6=r10
eor r6,r6,r10

# qhasm:     x7 ^= m7
# asm 1: eor >x7=int32#8,<x7=int32#8,<m7=int32#12
# asm 2: eor >x7=r7,<x7=r7,<m7=r11
eor r7,r7,r11

# qhasm:   nomessage10:
._nomessage10:

# qhasm:   c = c_stack
# asm 1: ldr >c=int32#14,<c_stack=stack32#3
# asm 2: ldr >c=r14,<c_stack=[sp,#232]
ldr r14,[sp,#232]

# qhasm:   mem32[c + 0] = x0
# asm 1: str <x0=int32#1,[<c=int32#14,#0]
# asm 2: str <x0=r0,[<c=r14,#0]
str r0,[r14,#0]

# qhasm:   mem32[c + 4] = x1
# asm 1: str <x1=int32#2,[<c=int32#14,#4]
# asm 2: str <x1=r1,[<c=r14,#4]
str r1,[r14,#4]

# qhasm:   mem32[c + 8] = x2
# asm 1: str <x2=int32#3,[<c=int32#14,#8]
# asm 2: str <x2=r2,[<c=r14,#8]
str r2,[r14,#8]

# qhasm:   mem32[c + 12] = x3
# asm 1: str <x3=int32#4,[<c=int32#14,#12]
# asm 2: str <x3=r3,[<c=r14,#12]
str r3,[r14,#12]

# qhasm:   mem32[c + 16] = x4
# asm 1: str <x4=int32#5,[<c=int32#14,#16]
# asm 2: str <x4=r4,[<c=r14,#16]
str r4,[r14,#16]

# qhasm:   mem32[c + 20] = x5
# asm 1: str <x5=int32#6,[<c=int32#14,#20]
# asm 2: str <x5=r5,[<c=r14,#20]
str r5,[r14,#20]

# qhasm:   mem32[c + 24] = x6
# asm 1: str <x6=int32#7,[<c=int32#14,#24]
# asm 2: str <x6=r6,[<c=r14,#24]
str r6,[r14,#24]

# qhasm:   mem32[c + 28] = x7
# asm 1: str <x7=int32#8,[<c=int32#14,#28]
# asm 2: str <x7=r7,[<c=r14,#28]
str r7,[r14,#28]

# qhasm:   assign r6 r7 to x8 x9 = x89_stack
# asm 1: ldrd >x8=int32#7,<x89_stack=stack64#17
# asm 2: ldrd >x8=r6,<x89_stack=[sp,#128]
ldrd r6,[sp,#128]

# qhasm:   assign r10 r11 to x10 x11 = x1011_stack
# asm 1: ldrd >x10=int32#11,<x1011_stack=stack64#15
# asm 2: ldrd >x10=r10,<x1011_stack=[sp,#112]
ldrd r10,[sp,#112]

# qhasm:   assign r0 r1 to j8 j9 = j89_stack
# asm 1: ldrd >j8=int32#1,<j89_stack=stack64#6
# asm 2: ldrd >j8=r0,<j89_stack=[sp,#40]
ldrd r0,[sp,#40]

# qhasm:   assign r4 r5 to j10 j11 = j1011_stack
# asm 1: ldrd >j10=int32#5,<j1011_stack=stack64#11
# asm 2: ldrd >j10=r4,<j1011_stack=[sp,#80]
ldrd r4,[sp,#80]

# qhasm:   x8 += j8
# asm 1: add >x8=int32#7,<x8=int32#7,<j8=int32#1
# asm 2: add >x8=r6,<x8=r6,<j8=r0
add r6,r6,r0

# qhasm:   x9 += j9
# asm 1: add >x9=int32#8,<x9=int32#8,<j9=int32#2
# asm 2: add >x9=r7,<x9=r7,<j9=r1
add r7,r7,r1

# qhasm:   x10 += j10
# asm 1: add >x10=int32#11,<x10=int32#11,<j10=int32#5
# asm 2: add >x10=r10,<x10=r10,<j10=r4
add r10,r10,r4

# qhasm:   x11 += j11
# asm 1: add >x11=int32#12,<x11=int32#12,<j11=int32#6
# asm 2: add >x11=r11,<x11=r11,<j11=r5
add r11,r11,r5

# qhasm:   carry? j8 += 1
# asm 1: adds >j8=int32#1,<j8=int32#1,#1
# asm 2: adds >j8=r0,<j8=r0,#1
adds r0,r0,#1

# qhasm:   j9 += 0 + carry
# asm 1: adc >j9=int32#2,<j9=int32#2,#0
# asm 2: adc >j9=r1,<j9=r1,#0
adc r1,r1,#0

# qhasm:   assign r0 r1 to j8 j9; j89_stack = j8 j9
# asm 1: strd <j8=int32#1,>j89_stack=stack64#6
# asm 2: strd <j8=r0,>j89_stack=[sp,#40]
strd r0,[sp,#40]

# qhasm:   assign r2 r3 to x12 x13 = x1213_stack
# asm 1: ldrd >x12=int32#3,<x1213_stack=stack64#16
# asm 2: ldrd >x12=r2,<x1213_stack=[sp,#120]
ldrd r2,[sp,#120]

# qhasm:   assign r8 r9 to x14 x15 = x1415_stack
# asm 1: ldrd >x14=int32#9,<x1415_stack=stack64#18
# asm 2: ldrd >x14=r8,<x1415_stack=[sp,#136]
ldrd r8,[sp,#136]

# qhasm:   assign r4 r5 to j12 j13 = j1213_stack
# asm 1: ldrd >j12=int32#5,<j1213_stack=stack64#8
# asm 2: ldrd >j12=r4,<j1213_stack=[sp,#56]
ldrd r4,[sp,#56]

# qhasm:   assign r0 r1 to j14 j15 = j1415_stack
# asm 1: ldrd >j14=int32#1,<j1415_stack=stack64#12
# asm 2: ldrd >j14=r0,<j1415_stack=[sp,#88]
ldrd r0,[sp,#88]

# qhasm:   x12 += j12
# asm 1: add >x12=int32#3,<x12=int32#3,<j12=int32#5
# asm 2: add >x12=r2,<x12=r2,<j12=r4
add r2,r2,r4

# qhasm:   x13 += j13
# asm 1: add >x13=int32#4,<x13=int32#4,<j13=int32#6
# asm 2: add >x13=r3,<x13=r3,<j13=r5
add r3,r3,r5

# qhasm:   x14 += j14
# asm 1: add >x14=int32#1,<x14=int32#9,<j14=int32#1
# asm 2: add >x14=r0,<x14=r8,<j14=r0
add r0,r8,r0

# qhasm:   x15 += j15
# asm 1: add >x15=int32#2,<x15=int32#10,<j15=int32#2
# asm 2: add >x15=r1,<x15=r9,<j15=r1
add r1,r9,r1

# qhasm:                      =? m - 0
# asm 1: cmp <m=int32#13,#0
# asm 2: cmp <m=r12,#0
cmp r12,#0

# qhasm:   goto nomessage11 if =
beq ._nomessage11

# qhasm:       m8 = mem32[m + 32]
# asm 1: ldr >m8=int32#5,[<m=int32#13,#32]
# asm 2: ldr >m8=r4,[<m=r12,#32]
ldr r4,[r12,#32]

# qhasm:       m9 = mem32[m + 36]
# asm 1: ldr >m9=int32#6,[<m=int32#13,#36]
# asm 2: ldr >m9=r5,[<m=r12,#36]
ldr r5,[r12,#36]

# qhasm:       m10 = mem32[m + 40]
# asm 1: ldr >m10=int32#9,[<m=int32#13,#40]
# asm 2: ldr >m10=r8,[<m=r12,#40]
ldr r8,[r12,#40]

# qhasm:       m11 = mem32[m + 44]
# asm 1: ldr >m11=int32#10,[<m=int32#13,#44]
# asm 2: ldr >m11=r9,[<m=r12,#44]
ldr r9,[r12,#44]

# qhasm:     x8 ^= m8
# asm 1: eor >x8=int32#7,<x8=int32#7,<m8=int32#5
# asm 2: eor >x8=r6,<x8=r6,<m8=r4
eor r6,r6,r4

# qhasm:       m12 = mem32[m + 48]
# asm 1: ldr >m12=int32#5,[<m=int32#13,#48]
# asm 2: ldr >m12=r4,[<m=r12,#48]
ldr r4,[r12,#48]

# qhasm:     x9 ^= m9
# asm 1: eor >x9=int32#8,<x9=int32#8,<m9=int32#6
# asm 2: eor >x9=r7,<x9=r7,<m9=r5
eor r7,r7,r5

# qhasm:       m13 = mem32[m + 52]
# asm 1: ldr >m13=int32#6,[<m=int32#13,#52]
# asm 2: ldr >m13=r5,[<m=r12,#52]
ldr r5,[r12,#52]

# qhasm:     x10 ^= m10
# asm 1: eor >x10=int32#11,<x10=int32#11,<m10=int32#9
# asm 2: eor >x10=r10,<x10=r10,<m10=r8
eor r10,r10,r8

# qhasm:       m14 = mem32[m + 56]
# asm 1: ldr >m14=int32#9,[<m=int32#13,#56]
# asm 2: ldr >m14=r8,[<m=r12,#56]
ldr r8,[r12,#56]

# qhasm:     x11 ^= m11
# asm 1: eor >x11=int32#12,<x11=int32#12,<m11=int32#10
# asm 2: eor >x11=r11,<x11=r11,<m11=r9
eor r11,r11,r9

# qhasm:       m15 = mem32[m + 60]
# asm 1: ldr >m15=int32#10,[<m=int32#13,#60]
# asm 2: ldr >m15=r9,[<m=r12,#60]
ldr r9,[r12,#60]

# qhasm:     x12 ^= m12
# asm 1: eor >x12=int32#3,<x12=int32#3,<m12=int32#5
# asm 2: eor >x12=r2,<x12=r2,<m12=r4
eor r2,r2,r4

# qhasm:     x13 ^= m13
# asm 1: eor >x13=int32#4,<x13=int32#4,<m13=int32#6
# asm 2: eor >x13=r3,<x13=r3,<m13=r5
eor r3,r3,r5

# qhasm:     x14 ^= m14
# asm 1: eor >x14=int32#1,<x14=int32#1,<m14=int32#9
# asm 2: eor >x14=r0,<x14=r0,<m14=r8
eor r0,r0,r8

# qhasm:     x15 ^= m15
# asm 1: eor >x15=int32#2,<x15=int32#2,<m15=int32#10
# asm 2: eor >x15=r1,<x15=r1,<m15=r9
eor r1,r1,r9

# qhasm:     m += 64
# asm 1: add >m=int32#5,<m=int32#13,#64
# asm 2: add >m=r4,<m=r12,#64
add r4,r12,#64

# qhasm:     m_stack = m
# asm 1: str <m=int32#5,>m_stack=stack32#4
# asm 2: str <m=r4,>m_stack=[sp,#236]
str r4,[sp,#236]

# qhasm:   nomessage11:
._nomessage11:

# qhasm:   mem32[c + 32] = x8
# asm 1: str <x8=int32#7,[<c=int32#14,#32]
# asm 2: str <x8=r6,[<c=r14,#32]
str r6,[r14,#32]

# qhasm:   mem32[c + 36] = x9
# asm 1: str <x9=int32#8,[<c=int32#14,#36]
# asm 2: str <x9=r7,[<c=r14,#36]
str r7,[r14,#36]

# qhasm:   mem32[c + 40] = x10
# asm 1: str <x10=int32#11,[<c=int32#14,#40]
# asm 2: str <x10=r10,[<c=r14,#40]
str r10,[r14,#40]

# qhasm:   mem32[c + 44] = x11
# asm 1: str <x11=int32#12,[<c=int32#14,#44]
# asm 2: str <x11=r11,[<c=r14,#44]
str r11,[r14,#44]

# qhasm:   mem32[c + 48] = x12
# asm 1: str <x12=int32#3,[<c=int32#14,#48]
# asm 2: str <x12=r2,[<c=r14,#48]
str r2,[r14,#48]

# qhasm:   mem32[c + 52] = x13
# asm 1: str <x13=int32#4,[<c=int32#14,#52]
# asm 2: str <x13=r3,[<c=r14,#52]
str r3,[r14,#52]

# qhasm:   mem32[c + 56] = x14
# asm 1: str <x14=int32#1,[<c=int32#14,#56]
# asm 2: str <x14=r0,[<c=r14,#56]
str r0,[r14,#56]

# qhasm:   mem32[c + 60] = x15
# asm 1: str <x15=int32#2,[<c=int32#14,#60]
# asm 2: str <x15=r1,[<c=r14,#60]
str r1,[r14,#60]

# qhasm:   c += 64
# asm 1: add >c=int32#1,<c=int32#14,#64
# asm 2: add >c=r0,<c=r14,#64
add r0,r14,#64

# qhasm:   c_stack = c
# asm 1: str <c=int32#1,>c_stack=stack32#3
# asm 2: str <c=r0,>c_stack=[sp,#232]
str r0,[sp,#232]

# qhasm:   mlenlow = mlenlow_stack
# asm 1: ldr >mlenlow=int32#1,<mlenlow_stack=stack32#6
# asm 2: ldr >mlenlow=r0,<mlenlow_stack=[sp,#244]
ldr r0,[sp,#244]

# qhasm:   mlenhigh = mlenhigh_stack
# asm 1: ldr >mlenhigh=int32#2,<mlenhigh_stack=stack32#7
# asm 2: ldr >mlenhigh=r1,<mlenhigh_stack=[sp,#248]
ldr r1,[sp,#248]

# qhasm:                   unsigned>? mlenlow - 64
# asm 1: cmp <mlenlow=int32#1,#64
# asm 2: cmp <mlenlow=r0,#64
cmp r0,#64

# qhasm: goto nextblock if unsigned>
bhi ._nextblock

# qhasm:                    =? mlenhigh - 0
# asm 1: cmp <mlenhigh=int32#2,#0
# asm 2: cmp <mlenhigh=r1,#0
cmp r1,#0

# qhasm: goto nextblock if !=
bne ._nextblock

# qhasm:              =? mlenlow - 64
# asm 1: cmp <mlenlow=int32#1,#64
# asm 2: cmp <mlenlow=r0,#64
cmp r0,#64

# qhasm: goto done if =
beq ._done

# qhasm:   m = &tmp
# asm 1: lea >m=int32#2,<tmp=stack512#1
# asm 2: lea >m=r1,<tmp=[sp,#160]
add r1,sp,#160

# qhasm:   c = c_original
# asm 1: ldr >c=int32#3,<c_original=stack32#5
# asm 2: ldr >c=r2,<c_original=[sp,#240]
ldr r2,[sp,#240]

# qhasm:   i = 0
# asm 1: ldr >i=int32#4,=0
# asm 2: ldr >i=r3,=0
ldr r3,=0

# qhasm:   ccopy:
._ccopy:

# qhasm:     ci = mem8[m]; m += 1
# asm 1: ldrb >ci=int32#5,[<m=int32#2],#1
# asm 2: ldrb >ci=r4,[<m=r1],#1
ldrb r4,[r1],#1

# qhasm:     i += 1
# asm 1: add >i=int32#4,<i=int32#4,#1
# asm 2: add >i=r3,<i=r3,#1
add r3,r3,#1

# qhasm:     mem8[c] = ci; c += 1
# asm 1: strb <ci=int32#5,[<c=int32#3],#1
# asm 2: strb <ci=r4,[<c=r2],#1
strb r4,[r2],#1

# qhasm:                 unsigned<? i - mlenlow
# asm 1: cmp <i=int32#4,<mlenlow=int32#1
# asm 2: cmp <i=r3,<mlenlow=r0
cmp r3,r0

# qhasm:   goto ccopy if unsigned<
blo ._ccopy

# qhasm: done:
._done:

# qhasm: assign r4 r5 to caller_r4 caller_r5 = stack_r45
# asm 1: ldrd >caller_r4=int32#5,<stack_r45=stack64#1
# asm 2: ldrd >caller_r4=r4,<stack_r45=[sp,#0]
ldrd r4,[sp,#0]

# qhasm: assign r6 r7 to caller_r6 caller_r7 = stack_r67
# asm 1: ldrd >caller_r6=int32#7,<stack_r67=stack64#2
# asm 2: ldrd >caller_r6=r6,<stack_r67=[sp,#8]
ldrd r6,[sp,#8]

# qhasm: assign r8 r9 to caller_r8 caller_r9 = stack_r89
# asm 1: ldrd >caller_r8=int32#9,<stack_r89=stack64#3
# asm 2: ldrd >caller_r8=r8,<stack_r89=[sp,#16]
ldrd r8,[sp,#16]

# qhasm: assign r10 r11 to caller_r10 caller_r11 = stack_r1011
# asm 1: ldrd >caller_r10=int32#11,<stack_r1011=stack64#4
# asm 2: ldrd >caller_r10=r10,<stack_r1011=[sp,#24]
ldrd r10,[sp,#24]

# qhasm: caller_r12 = stack_r12
# asm 1: ldr >caller_r12=int32#13,<stack_r12=stack32#2
# asm 2: ldr >caller_r12=r12,<stack_r12=[sp,#228]
ldr r12,[sp,#228]

# qhasm: caller_r14 = stack_r14
# asm 1: ldr >caller_r14=int32#14,<stack_r14=stack32#1
# asm 2: ldr >caller_r14=r14,<stack_r14=[sp,#224]
ldr r14,[sp,#224]

# qhasm: int32 result

# qhasm: result = 0
# asm 1: ldr >result=int32#1,=0
# asm 2: ldr >result=r0,=0
ldr r0,=0

# qhasm: qpopreturn result
mov sp,r12
vpop {q4,q5,q6,q7}
bx lr
