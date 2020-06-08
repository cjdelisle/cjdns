
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

# qhasm: reg128 r0

# qhasm: reg128 r1

# qhasm: reg128 r2

# qhasm: reg128 r3

# qhasm: reg128 r4

# qhasm: reg128 x01

# qhasm: reg128 x23

# qhasm: reg128 x4

# qhasm: reg128 y01

# qhasm: reg128 y23

# qhasm: reg128 y4

# qhasm: reg128 _5y01

# qhasm: reg128 _5y23

# qhasm: reg128 _5y4

# qhasm: reg128 c01

# qhasm: reg128 c23

# qhasm: reg128 c4

# qhasm: reg128 t0

# qhasm: reg128 t1

# qhasm: reg128 t2

# qhasm: reg128 t3

# qhasm: reg128 t4

# qhasm: reg128 mask

# qhasm: enter crypto_onetimeauth_poly1305_neon2_addmulmod
.align 2
.global _crypto_onetimeauth_poly1305_neon2_addmulmod
.global crypto_onetimeauth_poly1305_neon2_addmulmod
.type _crypto_onetimeauth_poly1305_neon2_addmulmod STT_FUNC
.type crypto_onetimeauth_poly1305_neon2_addmulmod STT_FUNC
_crypto_onetimeauth_poly1305_neon2_addmulmod:
crypto_onetimeauth_poly1305_neon2_addmulmod:
sub sp,sp,#0

# qhasm: 				2x mask = 0xffffffff
# asm 1: vmov.i64 >mask=reg128#1,#0xffffffff
# asm 2: vmov.i64 >mask=q0,#0xffffffff
vmov.i64 q0,#0xffffffff

# qhasm:   y01 aligned= mem128[input_2];input_2+=16
# asm 1: vld1.8 {>y01=reg128#2%bot->y01=reg128#2%top},[<input_2=int32#3,: 128]!
# asm 2: vld1.8 {>y01=d2->y01=d3},[<input_2=r2,: 128]!
vld1.8 {d2-d3},[r2,: 128]!

# qhasm: 4x _5y01 = y01 << 2
# asm 1: vshl.i32 >_5y01=reg128#3,<y01=reg128#2,#2
# asm 2: vshl.i32 >_5y01=q2,<y01=q1,#2
vshl.i32 q2,q1,#2

# qhasm:   y23 aligned= mem128[input_2];input_2+=16
# asm 1: vld1.8 {>y23=reg128#4%bot->y23=reg128#4%top},[<input_2=int32#3,: 128]!
# asm 2: vld1.8 {>y23=d6->y23=d7},[<input_2=r2,: 128]!
vld1.8 {d6-d7},[r2,: 128]!

# qhasm: 4x _5y23 = y23 << 2
# asm 1: vshl.i32 >_5y23=reg128#9,<y23=reg128#4,#2
# asm 2: vshl.i32 >_5y23=q8,<y23=q3,#2
vshl.i32 q8,q3,#2

# qhasm:   y4  aligned= mem64[input_2]y4[1]
# asm 1: vld1.8 {<y4=reg128#10%bot},[<input_2=int32#3,: 64]
# asm 2: vld1.8 {<y4=d18},[<input_2=r2,: 64]
vld1.8 {d18},[r2,: 64]

# qhasm: 4x _5y4 = y4 << 2
# asm 1: vshl.i32 >_5y4=reg128#11,<y4=reg128#10,#2
# asm 2: vshl.i32 >_5y4=q10,<y4=q9,#2
vshl.i32 q10,q9,#2

# qhasm:   x01 aligned= mem128[input_1];input_1+=16
# asm 1: vld1.8 {>x01=reg128#12%bot->x01=reg128#12%top},[<input_1=int32#2,: 128]!
# asm 2: vld1.8 {>x01=d22->x01=d23},[<input_1=r1,: 128]!
vld1.8 {d22-d23},[r1,: 128]!

# qhasm: 4x _5y01 += y01
# asm 1: vadd.i32 >_5y01=reg128#3,<_5y01=reg128#3,<y01=reg128#2
# asm 2: vadd.i32 >_5y01=q2,<_5y01=q2,<y01=q1
vadd.i32 q2,q2,q1

# qhasm:   x23 aligned= mem128[input_1];input_1+=16
# asm 1: vld1.8 {>x23=reg128#13%bot->x23=reg128#13%top},[<input_1=int32#2,: 128]!
# asm 2: vld1.8 {>x23=d24->x23=d25},[<input_1=r1,: 128]!
vld1.8 {d24-d25},[r1,: 128]!

# qhasm: 4x _5y23 += y23
# asm 1: vadd.i32 >_5y23=reg128#9,<_5y23=reg128#9,<y23=reg128#4
# asm 2: vadd.i32 >_5y23=q8,<_5y23=q8,<y23=q3
vadd.i32 q8,q8,q3

# qhasm: 4x _5y4 += y4
# asm 1: vadd.i32 >_5y4=reg128#11,<_5y4=reg128#11,<y4=reg128#10
# asm 2: vadd.i32 >_5y4=q10,<_5y4=q10,<y4=q9
vadd.i32 q10,q10,q9

# qhasm:   c01 aligned= mem128[input_3];input_3+=16
# asm 1: vld1.8 {>c01=reg128#14%bot->c01=reg128#14%top},[<input_3=int32#4,: 128]!
# asm 2: vld1.8 {>c01=d26->c01=d27},[<input_3=r3,: 128]!
vld1.8 {d26-d27},[r3,: 128]!

# qhasm: 4x x01 += c01
# asm 1: vadd.i32 >x01=reg128#12,<x01=reg128#12,<c01=reg128#14
# asm 2: vadd.i32 >x01=q11,<x01=q11,<c01=q13
vadd.i32 q11,q11,q13

# qhasm:   c23 aligned= mem128[input_3];input_3+=16
# asm 1: vld1.8 {>c23=reg128#14%bot->c23=reg128#14%top},[<input_3=int32#4,: 128]!
# asm 2: vld1.8 {>c23=d26->c23=d27},[<input_3=r3,: 128]!
vld1.8 {d26-d27},[r3,: 128]!

# qhasm: 4x x23 += c23
# asm 1: vadd.i32 >x23=reg128#13,<x23=reg128#13,<c23=reg128#14
# asm 2: vadd.i32 >x23=q12,<x23=q12,<c23=q13
vadd.i32 q12,q12,q13

# qhasm:   x4  aligned= mem64[input_1]x4[1]
# asm 1: vld1.8 {<x4=reg128#14%bot},[<input_1=int32#2,: 64]
# asm 2: vld1.8 {<x4=d26},[<input_1=r1,: 64]
vld1.8 {d26},[r1,: 64]

# qhasm: 				2x mask unsigned>>=6
# asm 1: vshr.u64 >mask=reg128#1,<mask=reg128#1,#6
# asm 2: vshr.u64 >mask=q0,<mask=q0,#6
vshr.u64 q0,q0,#6

# qhasm:   c4  aligned= mem64[input_3]c4[1]
# asm 1: vld1.8 {<c4=reg128#15%bot},[<input_3=int32#4,: 64]
# asm 2: vld1.8 {<c4=d28},[<input_3=r3,: 64]
vld1.8 {d28},[r3,: 64]

# qhasm: 4x x4 += c4
# asm 1: vadd.i32 >x4=reg128#14,<x4=reg128#14,<c4=reg128#15
# asm 2: vadd.i32 >x4=q13,<x4=q13,<c4=q14
vadd.i32 q13,q13,q14

# qhasm: r0[0,1]  = x01[0] unsigned* y01[0];   r0[2,3]  = x01[1] unsigned* y01[1]
# asm 1: vmull.u32 >r0=reg128#15,<x01=reg128#12%bot,<y01=reg128#2%bot
# asm 2: vmull.u32 >r0=q14,<x01=d22,<y01=d2
vmull.u32 q14,d22,d2

# qhasm: r0[0,1] += x01[2] unsigned*  _5y4[0]; r0[2,3] += x01[3] unsigned*  _5y4[1]
# asm 1: vmlal.u32 <r0=reg128#15,<x01=reg128#12%top,<_5y4=reg128#11%bot
# asm 2: vmlal.u32 <r0=q14,<x01=d23,<_5y4=d20
vmlal.u32 q14,d23,d20

# qhasm: r0[0,1] += x23[0] unsigned* _5y23[2]; r0[2,3] += x23[1] unsigned* _5y23[3]
# asm 1: vmlal.u32 <r0=reg128#15,<x23=reg128#13%bot,<_5y23=reg128#9%top
# asm 2: vmlal.u32 <r0=q14,<x23=d24,<_5y23=d17
vmlal.u32 q14,d24,d17

# qhasm: r0[0,1] += x23[2] unsigned* _5y23[0]; r0[2,3] += x23[3] unsigned* _5y23[1]
# asm 1: vmlal.u32 <r0=reg128#15,<x23=reg128#13%top,<_5y23=reg128#9%bot
# asm 2: vmlal.u32 <r0=q14,<x23=d25,<_5y23=d16
vmlal.u32 q14,d25,d16

# qhasm: r0[0,1] +=  x4[0] unsigned* _5y01[2]; r0[2,3] +=  x4[1] unsigned* _5y01[3]
# asm 1: vmlal.u32 <r0=reg128#15,<x4=reg128#14%bot,<_5y01=reg128#3%top
# asm 2: vmlal.u32 <r0=q14,<x4=d26,<_5y01=d5
vmlal.u32 q14,d26,d5

# qhasm: r1[0,1]  = x01[0] unsigned* y01[2];   r1[2,3]  = x01[1] unsigned* y01[3]
# asm 1: vmull.u32 >r1=reg128#3,<x01=reg128#12%bot,<y01=reg128#2%top
# asm 2: vmull.u32 >r1=q2,<x01=d22,<y01=d3
vmull.u32 q2,d22,d3

# qhasm: r1[0,1] += x01[2] unsigned* y01[0];   r1[2,3] += x01[3] unsigned* y01[1]
# asm 1: vmlal.u32 <r1=reg128#3,<x01=reg128#12%top,<y01=reg128#2%bot
# asm 2: vmlal.u32 <r1=q2,<x01=d23,<y01=d2
vmlal.u32 q2,d23,d2

# qhasm: r1[0,1] += x23[0] unsigned*  _5y4[0]; r1[2,3] += x23[1] unsigned*  _5y4[1]
# asm 1: vmlal.u32 <r1=reg128#3,<x23=reg128#13%bot,<_5y4=reg128#11%bot
# asm 2: vmlal.u32 <r1=q2,<x23=d24,<_5y4=d20
vmlal.u32 q2,d24,d20

# qhasm: r1[0,1] += x23[2] unsigned* _5y23[2]; r1[2,3] += x23[3] unsigned* _5y23[3]
# asm 1: vmlal.u32 <r1=reg128#3,<x23=reg128#13%top,<_5y23=reg128#9%top
# asm 2: vmlal.u32 <r1=q2,<x23=d25,<_5y23=d17
vmlal.u32 q2,d25,d17

# qhasm: r1[0,1] +=  x4[0] unsigned* _5y23[0]; r1[2,3] +=  x4[1] unsigned* _5y23[1]
# asm 1: vmlal.u32 <r1=reg128#3,<x4=reg128#14%bot,<_5y23=reg128#9%bot
# asm 2: vmlal.u32 <r1=q2,<x4=d26,<_5y23=d16
vmlal.u32 q2,d26,d16

# qhasm: r2[0,1]  = x01[0] unsigned* y23[0];   r2[2,3]  = x01[1] unsigned* y23[1]
# asm 1: vmull.u32 >r2=reg128#16,<x01=reg128#12%bot,<y23=reg128#4%bot
# asm 2: vmull.u32 >r2=q15,<x01=d22,<y23=d6
vmull.u32 q15,d22,d6

# qhasm: r2[0,1] += x01[2] unsigned* y01[2];   r2[2,3] += x01[3] unsigned* y01[3]
# asm 1: vmlal.u32 <r2=reg128#16,<x01=reg128#12%top,<y01=reg128#2%top
# asm 2: vmlal.u32 <r2=q15,<x01=d23,<y01=d3
vmlal.u32 q15,d23,d3

# qhasm: r2[0,1] += x23[0] unsigned* y01[0];   r2[2,3] += x23[1] unsigned* y01[1]
# asm 1: vmlal.u32 <r2=reg128#16,<x23=reg128#13%bot,<y01=reg128#2%bot
# asm 2: vmlal.u32 <r2=q15,<x23=d24,<y01=d2
vmlal.u32 q15,d24,d2

# qhasm: r2[0,1] += x23[2] unsigned*  _5y4[0]; r2[2,3] += x23[3] unsigned*  _5y4[1]
# asm 1: vmlal.u32 <r2=reg128#16,<x23=reg128#13%top,<_5y4=reg128#11%bot
# asm 2: vmlal.u32 <r2=q15,<x23=d25,<_5y4=d20
vmlal.u32 q15,d25,d20

# qhasm: r2[0,1] +=  x4[0] unsigned* _5y23[2]; r2[2,3] +=  x4[1] unsigned* _5y23[3]
# asm 1: vmlal.u32 <r2=reg128#16,<x4=reg128#14%bot,<_5y23=reg128#9%top
# asm 2: vmlal.u32 <r2=q15,<x4=d26,<_5y23=d17
vmlal.u32 q15,d26,d17

# qhasm: r3[0,1]  = x01[0] unsigned* y23[2];   r3[2,3]  = x01[1] unsigned* y23[3]
# asm 1: vmull.u32 >r3=reg128#9,<x01=reg128#12%bot,<y23=reg128#4%top
# asm 2: vmull.u32 >r3=q8,<x01=d22,<y23=d7
vmull.u32 q8,d22,d7

# qhasm: r3[0,1] += x01[2] unsigned* y23[0];   r3[2,3] += x01[3] unsigned* y23[1]
# asm 1: vmlal.u32 <r3=reg128#9,<x01=reg128#12%top,<y23=reg128#4%bot
# asm 2: vmlal.u32 <r3=q8,<x01=d23,<y23=d6
vmlal.u32 q8,d23,d6

# qhasm: r3[0,1] += x23[0] unsigned* y01[2];   r3[2,3] += x23[1] unsigned* y01[3]
# asm 1: vmlal.u32 <r3=reg128#9,<x23=reg128#13%bot,<y01=reg128#2%top
# asm 2: vmlal.u32 <r3=q8,<x23=d24,<y01=d3
vmlal.u32 q8,d24,d3

# qhasm: r3[0,1] += x23[2] unsigned* y01[0];   r3[2,3] += x23[3] unsigned* y01[1]
# asm 1: vmlal.u32 <r3=reg128#9,<x23=reg128#13%top,<y01=reg128#2%bot
# asm 2: vmlal.u32 <r3=q8,<x23=d25,<y01=d2
vmlal.u32 q8,d25,d2

# qhasm: r3[0,1] +=  x4[0] unsigned*  _5y4[0]; r3[2,3] +=  x4[1] unsigned*  _5y4[1]
# asm 1: vmlal.u32 <r3=reg128#9,<x4=reg128#14%bot,<_5y4=reg128#11%bot
# asm 2: vmlal.u32 <r3=q8,<x4=d26,<_5y4=d20
vmlal.u32 q8,d26,d20

# qhasm: r4[0,1]  = x01[0] unsigned*  y4[0];  r4[2,3]  = x01[1] unsigned*  y4[1]
# asm 1: vmull.u32 >r4=reg128#10,<x01=reg128#12%bot,<y4=reg128#10%bot
# asm 2: vmull.u32 >r4=q9,<x01=d22,<y4=d18
vmull.u32 q9,d22,d18

# qhasm: r4[0,1] += x01[2] unsigned* y23[2];  r4[2,3] += x01[3] unsigned* y23[3]
# asm 1: vmlal.u32 <r4=reg128#10,<x01=reg128#12%top,<y23=reg128#4%top
# asm 2: vmlal.u32 <r4=q9,<x01=d23,<y23=d7
vmlal.u32 q9,d23,d7

# qhasm: r4[0,1] += x23[0] unsigned* y23[0];  r4[2,3] += x23[1] unsigned* y23[1]
# asm 1: vmlal.u32 <r4=reg128#10,<x23=reg128#13%bot,<y23=reg128#4%bot
# asm 2: vmlal.u32 <r4=q9,<x23=d24,<y23=d6
vmlal.u32 q9,d24,d6

# qhasm: r4[0,1] += x23[2] unsigned* y01[2];  r4[2,3] += x23[3] unsigned* y01[3]
# asm 1: vmlal.u32 <r4=reg128#10,<x23=reg128#13%top,<y01=reg128#2%top
# asm 2: vmlal.u32 <r4=q9,<x23=d25,<y01=d3
vmlal.u32 q9,d25,d3

# qhasm: r4[0,1] +=  x4[0] unsigned* y01[0];  r4[2,3] +=  x4[1] unsigned* y01[1]
# asm 1: vmlal.u32 <r4=reg128#10,<x4=reg128#14%bot,<y01=reg128#2%bot
# asm 2: vmlal.u32 <r4=q9,<x4=d26,<y01=d2
vmlal.u32 q9,d26,d2

# qhasm: 2x t1 = r0 unsigned>> 26
# asm 1: vshr.u64 >t1=reg128#2,<r0=reg128#15,#26
# asm 2: vshr.u64 >t1=q1,<r0=q14,#26
vshr.u64 q1,q14,#26

# qhasm:    r0 &= mask
# asm 1: vand >r0=reg128#4,<r0=reg128#15,<mask=reg128#1
# asm 2: vand >r0=q3,<r0=q14,<mask=q0
vand q3,q14,q0

# qhasm: 2x r1 += t1
# asm 1: vadd.i64 >r1=reg128#2,<r1=reg128#3,<t1=reg128#2
# asm 2: vadd.i64 >r1=q1,<r1=q2,<t1=q1
vadd.i64 q1,q2,q1

# qhasm:                 2x t4 = r3 unsigned>> 26
# asm 1: vshr.u64 >t4=reg128#3,<r3=reg128#9,#26
# asm 2: vshr.u64 >t4=q2,<r3=q8,#26
vshr.u64 q2,q8,#26

# qhasm:                    r3 &= mask
# asm 1: vand >r3=reg128#9,<r3=reg128#9,<mask=reg128#1
# asm 2: vand >r3=q8,<r3=q8,<mask=q0
vand q8,q8,q0

# qhasm:                 2x r4 += t4
# asm 1: vadd.i64 >r4=reg128#3,<r4=reg128#10,<t4=reg128#3
# asm 2: vadd.i64 >r4=q2,<r4=q9,<t4=q2
vadd.i64 q2,q9,q2

# qhasm: 2x t2 = r1 unsigned>> 26
# asm 1: vshr.u64 >t2=reg128#10,<r1=reg128#2,#26
# asm 2: vshr.u64 >t2=q9,<r1=q1,#26
vshr.u64 q9,q1,#26

# qhasm:    r1 &= mask
# asm 1: vand >r1=reg128#2,<r1=reg128#2,<mask=reg128#1
# asm 2: vand >r1=q1,<r1=q1,<mask=q0
vand q1,q1,q0

# qhasm:                 2x t0 = r4 unsigned>> 26
# asm 1: vshr.u64 >t0=reg128#11,<r4=reg128#3,#26
# asm 2: vshr.u64 >t0=q10,<r4=q2,#26
vshr.u64 q10,q2,#26

# qhasm: 2x r2 += t2
# asm 1: vadd.i64 >r2=reg128#10,<r2=reg128#16,<t2=reg128#10
# asm 2: vadd.i64 >r2=q9,<r2=q15,<t2=q9
vadd.i64 q9,q15,q9

# qhasm:                    r4 &= mask
# asm 1: vand >r4=reg128#3,<r4=reg128#3,<mask=reg128#1
# asm 2: vand >r4=q2,<r4=q2,<mask=q0
vand q2,q2,q0

# qhasm:                 2x r0 += t0
# asm 1: vadd.i64 >r0=reg128#4,<r0=reg128#4,<t0=reg128#11
# asm 2: vadd.i64 >r0=q3,<r0=q3,<t0=q10
vadd.i64 q3,q3,q10

# qhasm:                 2x t0 <<= 2
# asm 1: vshl.i64 >t0=reg128#11,<t0=reg128#11,#2
# asm 2: vshl.i64 >t0=q10,<t0=q10,#2
vshl.i64 q10,q10,#2

# qhasm: 2x t3 = r2 unsigned>> 26
# asm 1: vshr.u64 >t3=reg128#12,<r2=reg128#10,#26
# asm 2: vshr.u64 >t3=q11,<r2=q9,#26
vshr.u64 q11,q9,#26

# qhasm:                 2x r0 += t0
# asm 1: vadd.i64 >r0=reg128#4,<r0=reg128#4,<t0=reg128#11
# asm 2: vadd.i64 >r0=q3,<r0=q3,<t0=q10
vadd.i64 q3,q3,q10

# qhasm:    x23 = r2 & mask
# asm 1: vand >x23=reg128#10,<r2=reg128#10,<mask=reg128#1
# asm 2: vand >x23=q9,<r2=q9,<mask=q0
vand q9,q9,q0

# qhasm: 2x r3 += t3
# asm 1: vadd.i64 >r3=reg128#9,<r3=reg128#9,<t3=reg128#12
# asm 2: vadd.i64 >r3=q8,<r3=q8,<t3=q11
vadd.i64 q8,q8,q11

# qhasm:                 2x t1 = r0 unsigned>> 26
# asm 1: vshr.u64 >t1=reg128#11,<r0=reg128#4,#26
# asm 2: vshr.u64 >t1=q10,<r0=q3,#26
vshr.u64 q10,q3,#26

# qhasm: 				x23 = x23[0,2,1,3]
# asm 1: vtrn.32 <x23=reg128#10%bot,<x23=reg128#10%top
# asm 2: vtrn.32 <x23=d18,<x23=d19
vtrn.32 d18,d19

# qhasm:                    x01 = r0 & mask
# asm 1: vand >x01=reg128#4,<r0=reg128#4,<mask=reg128#1
# asm 2: vand >x01=q3,<r0=q3,<mask=q0
vand q3,q3,q0

# qhasm:                 2x r1 += t1
# asm 1: vadd.i64 >r1=reg128#2,<r1=reg128#2,<t1=reg128#11
# asm 2: vadd.i64 >r1=q1,<r1=q1,<t1=q10
vadd.i64 q1,q1,q10

# qhasm: 2x t4 = r3 unsigned>> 26
# asm 1: vshr.u64 >t4=reg128#11,<r3=reg128#9,#26
# asm 2: vshr.u64 >t4=q10,<r3=q8,#26
vshr.u64 q10,q8,#26

# qhasm: 				x01 = x01[0,2,1,3]
# asm 1: vtrn.32 <x01=reg128#4%bot,<x01=reg128#4%top
# asm 2: vtrn.32 <x01=d6,<x01=d7
vtrn.32 d6,d7

# qhasm:    r3 &= mask
# asm 1: vand >r3=reg128#1,<r3=reg128#9,<mask=reg128#1
# asm 2: vand >r3=q0,<r3=q8,<mask=q0
vand q0,q8,q0

# qhasm: 				r1 = r1[0,2,1,3]
# asm 1: vtrn.32 <r1=reg128#2%bot,<r1=reg128#2%top
# asm 2: vtrn.32 <r1=d2,<r1=d3
vtrn.32 d2,d3

# qhasm: 2x x4 = r4 + t4
# asm 1: vadd.i64 >x4=reg128#3,<r4=reg128#3,<t4=reg128#11
# asm 2: vadd.i64 >x4=q2,<r4=q2,<t4=q10
vadd.i64 q2,q2,q10

# qhasm: 				r3 = r3[0,2,1,3]
# asm 1: vtrn.32 <r3=reg128#1%bot,<r3=reg128#1%top
# asm 2: vtrn.32 <r3=d0,<r3=d1
vtrn.32 d0,d1

# qhasm: 				x01 = x01[0,1] r1[0,1]
# asm 1: vext.32 <x01=reg128#4%top,<r1=reg128#2%bot,<r1=reg128#2%bot,#0
# asm 2: vext.32 <x01=d7,<r1=d2,<r1=d2,#0
vext.32 d7,d2,d2,#0

# qhasm: 				x23 = x23[0,1] r3[0,1]
# asm 1: vext.32 <x23=reg128#10%top,<r3=reg128#1%bot,<r3=reg128#1%bot,#0
# asm 2: vext.32 <x23=d19,<r3=d0,<r3=d0,#0
vext.32 d19,d0,d0,#0

# qhasm: 				x4 = x4[0,2,1,3]
# asm 1: vtrn.32 <x4=reg128#3%bot,<x4=reg128#3%top
# asm 2: vtrn.32 <x4=d4,<x4=d5
vtrn.32 d4,d5

# qhasm: mem128[input_0] aligned= x01;input_0+=16
# asm 1: vst1.8 {<x01=reg128#4%bot-<x01=reg128#4%top},[<input_0=int32#1,: 128]!
# asm 2: vst1.8 {<x01=d6-<x01=d7},[<input_0=r0,: 128]!
vst1.8 {d6-d7},[r0,: 128]!

# qhasm: mem128[input_0] aligned= x23;input_0+=16
# asm 1: vst1.8 {<x23=reg128#10%bot-<x23=reg128#10%top},[<input_0=int32#1,: 128]!
# asm 2: vst1.8 {<x23=d18-<x23=d19},[<input_0=r0,: 128]!
vst1.8 {d18-d19},[r0,: 128]!

# qhasm: mem64[input_0] aligned= x4[0]
# asm 1: vst1.8 <x4=reg128#3%bot,[<input_0=int32#1,: 64]
# asm 2: vst1.8 <x4=d4,[<input_0=r0,: 64]
vst1.8 d4,[r0,: 64]

# qhasm: return
add sp,sp,#0
bx lr
