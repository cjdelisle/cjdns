
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

# qhasm: int32 caller_r14

# qhasm: reg128 caller_q4

# qhasm: reg128 caller_q5

# qhasm: reg128 caller_q6

# qhasm: reg128 caller_q7

# qhasm: startcode
.text

# qhasm: int32 t0

# qhasm: int32 t1

# qhasm: int32 t2

# qhasm: int32 t3

# qhasm: int32 t4

# qhasm: int32 t5

# qhasm: int32 t6

# qhasm: int32 t7

# qhasm: int32 s0

# qhasm: int32 s1

# qhasm: int32 s2

# qhasm: int32 s3

# qhasm: int32 s4

# qhasm: int32 s5

# qhasm: int32 s6

# qhasm: int32 s7

# qhasm: int32 r0

# qhasm: int32 r1

# qhasm: int32 r2

# qhasm: int32 r3

# qhasm: int32 r4

# qhasm: int32 r5

# qhasm: int32 r6

# qhasm: int32 r7

# qhasm: int32 w0

# qhasm: int32 w1

# qhasm: int32 w2

# qhasm: int32 w3

# qhasm: int32 w4

# qhasm: int32 w5

# qhasm: int32 w6

# qhasm: int32 w7

# qhasm: int32 w8

# qhasm: int32 w9

# qhasm: int32 w10

# qhasm: int32 w11

# qhasm: int32 w12

# qhasm: int32 w13

# qhasm: int32 w14

# qhasm: int32 w15

# qhasm: stack32 s0_stack

# qhasm: stack32 s1_stack

# qhasm: stack32 s2_stack

# qhasm: stack32 s3_stack

# qhasm: stack32 s4_stack

# qhasm: stack32 s5_stack

# qhasm: stack32 s6_stack

# qhasm: stack32 s7_stack

# qhasm: stack32 r0_stack

# qhasm: stack32 r1_stack

# qhasm: stack32 r2_stack

# qhasm: stack32 r3_stack

# qhasm: stack32 r4_stack

# qhasm: stack32 r5_stack

# qhasm: stack32 r6_stack

# qhasm: stack32 r7_stack

# qhasm: stack32 w0_stack

# qhasm: stack32 w1_stack

# qhasm: stack32 w2_stack

# qhasm: stack32 w3_stack

# qhasm: stack32 w4_stack

# qhasm: stack32 w5_stack

# qhasm: stack32 w6_stack

# qhasm: stack32 w7_stack

# qhasm: stack32 w8_stack

# qhasm: stack32 w9_stack

# qhasm: stack32 w10_stack

# qhasm: stack32 w11_stack

# qhasm: stack32 w12_stack

# qhasm: stack32 w13_stack

# qhasm: stack32 w14_stack

# qhasm: stack32 w15_stack

# qhasm: int32 len

# qhasm: stack32 state_stack

# qhasm: stack32 len_stack

# qhasm: stack32 caller_r4_stack

# qhasm: stack32 caller_r5_stack

# qhasm: stack32 caller_r6_stack

# qhasm: stack32 caller_r7_stack

# qhasm: stack32 caller_r8_stack

# qhasm: stack32 caller_r9_stack

# qhasm: stack32 caller_r10_stack

# qhasm: stack32 caller_r11_stack

# qhasm: stack32 caller_r14_stack

# qhasm: enter crypto_hashblocks_sha256_arm11_core
.align 2
.global _crypto_hashblocks_sha256_arm11_core
.global crypto_hashblocks_sha256_arm11_core
.type _crypto_hashblocks_sha256_arm11_core STT_FUNC
.type crypto_hashblocks_sha256_arm11_core STT_FUNC
_crypto_hashblocks_sha256_arm11_core:
crypto_hashblocks_sha256_arm11_core:
sub sp,sp,#192

# qhasm: caller_r4_stack = caller_r4
# asm 1: str <caller_r4=int32#5,>caller_r4_stack=stack32#1
# asm 2: str <caller_r4=r4,>caller_r4_stack=[sp,#0]
str r4,[sp,#0]

# qhasm: caller_r5_stack = caller_r5
# asm 1: str <caller_r5=int32#6,>caller_r5_stack=stack32#2
# asm 2: str <caller_r5=r5,>caller_r5_stack=[sp,#4]
str r5,[sp,#4]

# qhasm: caller_r6_stack = caller_r6
# asm 1: str <caller_r6=int32#7,>caller_r6_stack=stack32#3
# asm 2: str <caller_r6=r6,>caller_r6_stack=[sp,#8]
str r6,[sp,#8]

# qhasm: caller_r7_stack = caller_r7
# asm 1: str <caller_r7=int32#8,>caller_r7_stack=stack32#4
# asm 2: str <caller_r7=r7,>caller_r7_stack=[sp,#12]
str r7,[sp,#12]

# qhasm: caller_r8_stack = caller_r8
# asm 1: str <caller_r8=int32#9,>caller_r8_stack=stack32#5
# asm 2: str <caller_r8=r8,>caller_r8_stack=[sp,#16]
str r8,[sp,#16]

# qhasm: caller_r9_stack = caller_r9
# asm 1: str <caller_r9=int32#10,>caller_r9_stack=stack32#6
# asm 2: str <caller_r9=r9,>caller_r9_stack=[sp,#20]
str r9,[sp,#20]

# qhasm: caller_r10_stack = caller_r10
# asm 1: str <caller_r10=int32#11,>caller_r10_stack=stack32#7
# asm 2: str <caller_r10=r10,>caller_r10_stack=[sp,#24]
str r10,[sp,#24]

# qhasm: caller_r11_stack = caller_r11
# asm 1: str <caller_r11=int32#12,>caller_r11_stack=stack32#8
# asm 2: str <caller_r11=r11,>caller_r11_stack=[sp,#28]
str r11,[sp,#28]

# qhasm: caller_r14_stack = caller_r14
# asm 1: str <caller_r14=int32#14,>caller_r14_stack=stack32#9
# asm 2: str <caller_r14=r14,>caller_r14_stack=[sp,#32]
str r14,[sp,#32]

# qhasm: bigendian
setend be

# qhasm: r0 = mem32[input_0 + 0]
# asm 1: ldr >r0=int32#5,[<input_0=int32#1,#0]
# asm 2: ldr >r0=r4,[<input_0=r0,#0]
ldr r4,[r0,#0]

# qhasm: r1 = mem32[input_0 + 4]
# asm 1: ldr >r1=int32#6,[<input_0=int32#1,#4]
# asm 2: ldr >r1=r5,[<input_0=r0,#4]
ldr r5,[r0,#4]

# qhasm: r2 = mem32[input_0 + 8]
# asm 1: ldr >r2=int32#7,[<input_0=int32#1,#8]
# asm 2: ldr >r2=r6,[<input_0=r0,#8]
ldr r6,[r0,#8]

# qhasm: r3 = mem32[input_0 + 12]
# asm 1: ldr >r3=int32#8,[<input_0=int32#1,#12]
# asm 2: ldr >r3=r7,[<input_0=r0,#12]
ldr r7,[r0,#12]

# qhasm: r4 = mem32[input_0 + 16]
# asm 1: ldr >r4=int32#9,[<input_0=int32#1,#16]
# asm 2: ldr >r4=r8,[<input_0=r0,#16]
ldr r8,[r0,#16]

# qhasm: r5 = mem32[input_0 + 20]
# asm 1: ldr >r5=int32#10,[<input_0=int32#1,#20]
# asm 2: ldr >r5=r9,[<input_0=r0,#20]
ldr r9,[r0,#20]

# qhasm: r6 = mem32[input_0 + 24]
# asm 1: ldr >r6=int32#11,[<input_0=int32#1,#24]
# asm 2: ldr >r6=r10,[<input_0=r0,#24]
ldr r10,[r0,#24]

# qhasm: r7 = mem32[input_0 + 28]
# asm 1: ldr >r7=int32#12,[<input_0=int32#1,#28]
# asm 2: ldr >r7=r11,[<input_0=r0,#28]
ldr r11,[r0,#28]

# qhasm: littleendian
setend le

# qhasm: s0_stack = r0
# asm 1: str <r0=int32#5,>s0_stack=stack32#10
# asm 2: str <r0=r4,>s0_stack=[sp,#36]
str r4,[sp,#36]

# qhasm: s1_stack = r1
# asm 1: str <r1=int32#6,>s1_stack=stack32#11
# asm 2: str <r1=r5,>s1_stack=[sp,#40]
str r5,[sp,#40]

# qhasm: s2_stack = r2
# asm 1: str <r2=int32#7,>s2_stack=stack32#12
# asm 2: str <r2=r6,>s2_stack=[sp,#44]
str r6,[sp,#44]

# qhasm: s3_stack = r3
# asm 1: str <r3=int32#8,>s3_stack=stack32#13
# asm 2: str <r3=r7,>s3_stack=[sp,#48]
str r7,[sp,#48]

# qhasm: s4_stack = r4
# asm 1: str <r4=int32#9,>s4_stack=stack32#14
# asm 2: str <r4=r8,>s4_stack=[sp,#52]
str r8,[sp,#52]

# qhasm: s5_stack = r5
# asm 1: str <r5=int32#10,>s5_stack=stack32#15
# asm 2: str <r5=r9,>s5_stack=[sp,#56]
str r9,[sp,#56]

# qhasm: s6_stack = r6
# asm 1: str <r6=int32#11,>s6_stack=stack32#16
# asm 2: str <r6=r10,>s6_stack=[sp,#60]
str r10,[sp,#60]

# qhasm: s7_stack = r7
# asm 1: str <r7=int32#12,>s7_stack=stack32#17
# asm 2: str <r7=r11,>s7_stack=[sp,#64]
str r11,[sp,#64]

# qhasm: state_stack = input_0
# asm 1: str <input_0=int32#1,>state_stack=stack32#18
# asm 2: str <input_0=r0,>state_stack=[sp,#68]
str r0,[sp,#68]

# qhasm: len_stack = input_2
# asm 1: str <input_2=int32#3,>len_stack=stack32#19
# asm 2: str <input_2=r2,>len_stack=[sp,#72]
str r2,[sp,#72]

# qhasm: mainloop:
._mainloop:

# qhasm: len = len_stack
# asm 1: ldr >len=int32#1,<len_stack=stack32#19
# asm 2: ldr >len=r0,<len_stack=[sp,#72]
ldr r0,[sp,#72]

# qhasm: unsigned<? len -= 64
# asm 1: subs >len=int32#1,<len=int32#1,#64
# asm 2: subs >len=r0,<len=r0,#64
subs r0,r0,#64

# qhasm: len_stack = len
# asm 1: str <len=int32#1,>len_stack=stack32#19
# asm 2: str <len=r0,>len_stack=[sp,#72]
str r0,[sp,#72]

# qhasm: goto end if unsigned<
blo ._end

# qhasm: bigendian
setend be

# qhasm: w0 = mem32[input_1 + 0]
# asm 1: ldr >w0=int32#1,[<input_1=int32#2,#0]
# asm 2: ldr >w0=r0,[<input_1=r1,#0]
ldr r0,[r1,#0]

# qhasm: w1 = mem32[input_1 + 4]
# asm 1: ldr >w1=int32#3,[<input_1=int32#2,#4]
# asm 2: ldr >w1=r2,[<input_1=r1,#4]
ldr r2,[r1,#4]

# qhasm: w2 = mem32[input_1 + 8]
# asm 1: ldr >w2=int32#13,[<input_1=int32#2,#8]
# asm 2: ldr >w2=r12,[<input_1=r1,#8]
ldr r12,[r1,#8]

# qhasm: w3 = mem32[input_1 + 12]
# asm 1: ldr >w3=int32#14,[<input_1=int32#2,#12]
# asm 2: ldr >w3=r14,[<input_1=r1,#12]
ldr r14,[r1,#12]

# qhasm: littleendian
setend le

# qhasm: w0_stack = w0
# asm 1: str <w0=int32#1,>w0_stack=stack32#20
# asm 2: str <w0=r0,>w0_stack=[sp,#76]
str r0,[sp,#76]

# qhasm: w1_stack = w1
# asm 1: str <w1=int32#3,>w1_stack=stack32#21
# asm 2: str <w1=r2,>w1_stack=[sp,#80]
str r2,[sp,#80]

# qhasm: w2_stack = w2
# asm 1: str <w2=int32#13,>w2_stack=stack32#22
# asm 2: str <w2=r12,>w2_stack=[sp,#84]
str r12,[sp,#84]

# qhasm: w3_stack = w3
# asm 1: str <w3=int32#14,>w3_stack=stack32#23
# asm 2: str <w3=r14,>w3_stack=[sp,#88]
str r14,[sp,#88]

# qhasm: bigendian
setend be

# qhasm: w4 = mem32[input_1 + 16]
# asm 1: ldr >w4=int32#1,[<input_1=int32#2,#16]
# asm 2: ldr >w4=r0,[<input_1=r1,#16]
ldr r0,[r1,#16]

# qhasm: w5 = mem32[input_1 + 20]
# asm 1: ldr >w5=int32#3,[<input_1=int32#2,#20]
# asm 2: ldr >w5=r2,[<input_1=r1,#20]
ldr r2,[r1,#20]

# qhasm: w6 = mem32[input_1 + 24]
# asm 1: ldr >w6=int32#13,[<input_1=int32#2,#24]
# asm 2: ldr >w6=r12,[<input_1=r1,#24]
ldr r12,[r1,#24]

# qhasm: w7 = mem32[input_1 + 28]
# asm 1: ldr >w7=int32#14,[<input_1=int32#2,#28]
# asm 2: ldr >w7=r14,[<input_1=r1,#28]
ldr r14,[r1,#28]

# qhasm: littleendian
setend le

# qhasm: w4_stack = w4
# asm 1: str <w4=int32#1,>w4_stack=stack32#24
# asm 2: str <w4=r0,>w4_stack=[sp,#92]
str r0,[sp,#92]

# qhasm: w5_stack = w5
# asm 1: str <w5=int32#3,>w5_stack=stack32#25
# asm 2: str <w5=r2,>w5_stack=[sp,#96]
str r2,[sp,#96]

# qhasm: w6_stack = w6
# asm 1: str <w6=int32#13,>w6_stack=stack32#26
# asm 2: str <w6=r12,>w6_stack=[sp,#100]
str r12,[sp,#100]

# qhasm: w7_stack = w7
# asm 1: str <w7=int32#14,>w7_stack=stack32#27
# asm 2: str <w7=r14,>w7_stack=[sp,#104]
str r14,[sp,#104]

# qhasm: bigendian
setend be

# qhasm: w8 = mem32[input_1 + 32]
# asm 1: ldr >w8=int32#1,[<input_1=int32#2,#32]
# asm 2: ldr >w8=r0,[<input_1=r1,#32]
ldr r0,[r1,#32]

# qhasm: w9 = mem32[input_1 + 36]
# asm 1: ldr >w9=int32#3,[<input_1=int32#2,#36]
# asm 2: ldr >w9=r2,[<input_1=r1,#36]
ldr r2,[r1,#36]

# qhasm: w10 = mem32[input_1 + 40]
# asm 1: ldr >w10=int32#13,[<input_1=int32#2,#40]
# asm 2: ldr >w10=r12,[<input_1=r1,#40]
ldr r12,[r1,#40]

# qhasm: w11 = mem32[input_1 + 44]
# asm 1: ldr >w11=int32#14,[<input_1=int32#2,#44]
# asm 2: ldr >w11=r14,[<input_1=r1,#44]
ldr r14,[r1,#44]

# qhasm: littleendian
setend le

# qhasm: w8_stack = w8
# asm 1: str <w8=int32#1,>w8_stack=stack32#28
# asm 2: str <w8=r0,>w8_stack=[sp,#108]
str r0,[sp,#108]

# qhasm: w9_stack = w9
# asm 1: str <w9=int32#3,>w9_stack=stack32#29
# asm 2: str <w9=r2,>w9_stack=[sp,#112]
str r2,[sp,#112]

# qhasm: w10_stack = w10
# asm 1: str <w10=int32#13,>w10_stack=stack32#30
# asm 2: str <w10=r12,>w10_stack=[sp,#116]
str r12,[sp,#116]

# qhasm: w11_stack = w11
# asm 1: str <w11=int32#14,>w11_stack=stack32#31
# asm 2: str <w11=r14,>w11_stack=[sp,#120]
str r14,[sp,#120]

# qhasm: bigendian
setend be

# qhasm: w12 = mem32[input_1 + 48]
# asm 1: ldr >w12=int32#1,[<input_1=int32#2,#48]
# asm 2: ldr >w12=r0,[<input_1=r1,#48]
ldr r0,[r1,#48]

# qhasm: w13 = mem32[input_1 + 52]
# asm 1: ldr >w13=int32#3,[<input_1=int32#2,#52]
# asm 2: ldr >w13=r2,[<input_1=r1,#52]
ldr r2,[r1,#52]

# qhasm: w14 = mem32[input_1 + 56]
# asm 1: ldr >w14=int32#13,[<input_1=int32#2,#56]
# asm 2: ldr >w14=r12,[<input_1=r1,#56]
ldr r12,[r1,#56]

# qhasm: w15 = mem32[input_1 + 60]
# asm 1: ldr >w15=int32#14,[<input_1=int32#2,#60]
# asm 2: ldr >w15=r14,[<input_1=r1,#60]
ldr r14,[r1,#60]

# qhasm: littleendian
setend le

# qhasm: w12_stack = w12
# asm 1: str <w12=int32#1,>w12_stack=stack32#32
# asm 2: str <w12=r0,>w12_stack=[sp,#124]
str r0,[sp,#124]

# qhasm: w13_stack = w13
# asm 1: str <w13=int32#3,>w13_stack=stack32#33
# asm 2: str <w13=r2,>w13_stack=[sp,#128]
str r2,[sp,#128]

# qhasm: w14_stack = w14
# asm 1: str <w14=int32#13,>w14_stack=stack32#34
# asm 2: str <w14=r12,>w14_stack=[sp,#132]
str r12,[sp,#132]

# qhasm: w15_stack = w15
# asm 1: str <w15=int32#14,>w15_stack=stack32#35
# asm 2: str <w15=r14,>w15_stack=[sp,#136]
str r14,[sp,#136]

# qhasm: w0 = w0_stack
# asm 1: ldr >w0=int32#1,<w0_stack=stack32#20
# asm 2: ldr >w0=r0,<w0_stack=[sp,#76]
ldr r0,[sp,#76]

# qhasm: t0 = r4 & r5
# asm 1: and >t0=int32#3,<r4=int32#9,<r5=int32#10
# asm 2: and >t0=r2,<r4=r8,<r5=r9
and r2,r8,r9

# qhasm: t1 = r6 & ~r4
# asm 1: bic >t1=int32#13,<r6=int32#11,<r4=int32#9
# asm 2: bic >t1=r12,<r6=r10,<r4=r8
bic r12,r10,r8

# qhasm: t0 ^= t1
# asm 1: eor >t0=int32#3,<t0=int32#3,<t1=int32#13
# asm 2: eor >t0=r2,<t0=r2,<t1=r12
eor r2,r2,r12

# qhasm: t1 = r4 ^ (r4 >>> 5)
# asm 1: eor >t1=int32#13,<r4=int32#9,<r4=int32#9,ROR #5
# asm 2: eor >t1=r12,<r4=r8,<r4=r8,ROR #5
eor r12,r8,r8,ROR #5

# qhasm: t1 ^= (r4 >>> 19)
# asm 1: eor >t1=int32#13,<t1=int32#13,<r4=int32#9,ROR #19
# asm 2: eor >t1=r12,<t1=r12,<r4=r8,ROR #19
eor r12,r12,r8,ROR #19

# qhasm: t2 = mem32[input_3 + 0]
# asm 1: ldr >t2=int32#14,[<input_3=int32#4,#0]
# asm 2: ldr >t2=r14,[<input_3=r3,#0]
ldr r14,[r3,#0]

# qhasm: t0 += w0
# asm 1: add >t0=int32#1,<t0=int32#3,<w0=int32#1
# asm 2: add >t0=r0,<t0=r2,<w0=r0
add r0,r2,r0

# qhasm: t0 += (t1 >>> 6)
# asm 1: add >t0=int32#1,<t0=int32#1,<t1=int32#13,ROR #6
# asm 2: add >t0=r0,<t0=r0,<t1=r12,ROR #6
add r0,r0,r12,ROR #6

# qhasm: t0 += t2
# asm 1: add >t0=int32#1,<t0=int32#1,<t2=int32#14
# asm 2: add >t0=r0,<t0=r0,<t2=r14
add r0,r0,r14

# qhasm: r7 += t0
# asm 1: add >r7=int32#1,<r7=int32#12,<t0=int32#1
# asm 2: add >r7=r0,<r7=r11,<t0=r0
add r0,r11,r0

# qhasm: r3 += r7
# asm 1: add >r3=int32#3,<r3=int32#8,<r7=int32#1
# asm 2: add >r3=r2,<r3=r7,<r7=r0
add r2,r7,r0

# qhasm: t0 = r0 & r1
# asm 1: and >t0=int32#8,<r0=int32#5,<r1=int32#6
# asm 2: and >t0=r7,<r0=r4,<r1=r5
and r7,r4,r5

# qhasm: t2 = r0 & r2
# asm 1: and >t2=int32#12,<r0=int32#5,<r2=int32#7
# asm 2: and >t2=r11,<r0=r4,<r2=r6
and r11,r4,r6

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#8,<t0=int32#8,<t2=int32#12
# asm 2: eor >t0=r7,<t0=r7,<t2=r11
eor r7,r7,r11

# qhasm: t2 = r1 & r2
# asm 1: and >t2=int32#12,<r1=int32#6,<r2=int32#7
# asm 2: and >t2=r11,<r1=r5,<r2=r6
and r11,r5,r6

# qhasm: t3 = r0 ^ (r0 >>> 11)
# asm 1: eor >t3=int32#13,<r0=int32#5,<r0=int32#5,ROR #11
# asm 2: eor >t3=r12,<r0=r4,<r0=r4,ROR #11
eor r12,r4,r4,ROR #11

# qhasm: t3 ^= (r0 >>> 20)
# asm 1: eor >t3=int32#13,<t3=int32#13,<r0=int32#5,ROR #20
# asm 2: eor >t3=r12,<t3=r12,<r0=r4,ROR #20
eor r12,r12,r4,ROR #20

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#8,<t0=int32#8,<t2=int32#12
# asm 2: eor >t0=r7,<t0=r7,<t2=r11
eor r7,r7,r11

# qhasm: t0 += (t3 >>> 2)
# asm 1: add >t0=int32#8,<t0=int32#8,<t3=int32#13,ROR #2
# asm 2: add >t0=r7,<t0=r7,<t3=r12,ROR #2
add r7,r7,r12,ROR #2

# qhasm: r7 += t0
# asm 1: add >r7=int32#1,<r7=int32#1,<t0=int32#8
# asm 2: add >r7=r0,<r7=r0,<t0=r7
add r0,r0,r7

# qhasm: w1 = w1_stack
# asm 1: ldr >w1=int32#8,<w1_stack=stack32#21
# asm 2: ldr >w1=r7,<w1_stack=[sp,#80]
ldr r7,[sp,#80]

# qhasm: t0 = r3 & r4
# asm 1: and >t0=int32#12,<r3=int32#3,<r4=int32#9
# asm 2: and >t0=r11,<r3=r2,<r4=r8
and r11,r2,r8

# qhasm: t1 = r5 & ~r3
# asm 1: bic >t1=int32#13,<r5=int32#10,<r3=int32#3
# asm 2: bic >t1=r12,<r5=r9,<r3=r2
bic r12,r9,r2

# qhasm: t0 ^= t1
# asm 1: eor >t0=int32#12,<t0=int32#12,<t1=int32#13
# asm 2: eor >t0=r11,<t0=r11,<t1=r12
eor r11,r11,r12

# qhasm: t1 = r3 ^ (r3 >>> 5)
# asm 1: eor >t1=int32#13,<r3=int32#3,<r3=int32#3,ROR #5
# asm 2: eor >t1=r12,<r3=r2,<r3=r2,ROR #5
eor r12,r2,r2,ROR #5

# qhasm: t1 ^= (r3 >>> 19)
# asm 1: eor >t1=int32#13,<t1=int32#13,<r3=int32#3,ROR #19
# asm 2: eor >t1=r12,<t1=r12,<r3=r2,ROR #19
eor r12,r12,r2,ROR #19

# qhasm: t2 = mem32[input_3 + 4]
# asm 1: ldr >t2=int32#14,[<input_3=int32#4,#4]
# asm 2: ldr >t2=r14,[<input_3=r3,#4]
ldr r14,[r3,#4]

# qhasm: t0 += w1
# asm 1: add >t0=int32#8,<t0=int32#12,<w1=int32#8
# asm 2: add >t0=r7,<t0=r11,<w1=r7
add r7,r11,r7

# qhasm: t0 += (t1 >>> 6)
# asm 1: add >t0=int32#8,<t0=int32#8,<t1=int32#13,ROR #6
# asm 2: add >t0=r7,<t0=r7,<t1=r12,ROR #6
add r7,r7,r12,ROR #6

# qhasm: t0 += t2
# asm 1: add >t0=int32#8,<t0=int32#8,<t2=int32#14
# asm 2: add >t0=r7,<t0=r7,<t2=r14
add r7,r7,r14

# qhasm: r6 += t0
# asm 1: add >r6=int32#8,<r6=int32#11,<t0=int32#8
# asm 2: add >r6=r7,<r6=r10,<t0=r7
add r7,r10,r7

# qhasm: r2 += r6
# asm 1: add >r2=int32#7,<r2=int32#7,<r6=int32#8
# asm 2: add >r2=r6,<r2=r6,<r6=r7
add r6,r6,r7

# qhasm: t0 = r7 & r0
# asm 1: and >t0=int32#11,<r7=int32#1,<r0=int32#5
# asm 2: and >t0=r10,<r7=r0,<r0=r4
and r10,r0,r4

# qhasm: t2 = r7 & r1
# asm 1: and >t2=int32#12,<r7=int32#1,<r1=int32#6
# asm 2: and >t2=r11,<r7=r0,<r1=r5
and r11,r0,r5

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t2 = r0 & r1
# asm 1: and >t2=int32#12,<r0=int32#5,<r1=int32#6
# asm 2: and >t2=r11,<r0=r4,<r1=r5
and r11,r4,r5

# qhasm: t3 = r7 ^ (r7 >>> 11)
# asm 1: eor >t3=int32#13,<r7=int32#1,<r7=int32#1,ROR #11
# asm 2: eor >t3=r12,<r7=r0,<r7=r0,ROR #11
eor r12,r0,r0,ROR #11

# qhasm: t3 ^= (r7 >>> 20)
# asm 1: eor >t3=int32#13,<t3=int32#13,<r7=int32#1,ROR #20
# asm 2: eor >t3=r12,<t3=r12,<r7=r0,ROR #20
eor r12,r12,r0,ROR #20

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t0 += (t3 >>> 2)
# asm 1: add >t0=int32#11,<t0=int32#11,<t3=int32#13,ROR #2
# asm 2: add >t0=r10,<t0=r10,<t3=r12,ROR #2
add r10,r10,r12,ROR #2

# qhasm: r6 += t0
# asm 1: add >r6=int32#8,<r6=int32#8,<t0=int32#11
# asm 2: add >r6=r7,<r6=r7,<t0=r10
add r7,r7,r10

# qhasm: w2 = w2_stack
# asm 1: ldr >w2=int32#11,<w2_stack=stack32#22
# asm 2: ldr >w2=r10,<w2_stack=[sp,#84]
ldr r10,[sp,#84]

# qhasm: t0 = r2 & r3
# asm 1: and >t0=int32#12,<r2=int32#7,<r3=int32#3
# asm 2: and >t0=r11,<r2=r6,<r3=r2
and r11,r6,r2

# qhasm: t1 = r4 & ~r2
# asm 1: bic >t1=int32#13,<r4=int32#9,<r2=int32#7
# asm 2: bic >t1=r12,<r4=r8,<r2=r6
bic r12,r8,r6

# qhasm: t0 ^= t1
# asm 1: eor >t0=int32#12,<t0=int32#12,<t1=int32#13
# asm 2: eor >t0=r11,<t0=r11,<t1=r12
eor r11,r11,r12

# qhasm: t1 = r2 ^ (r2 >>> 5)
# asm 1: eor >t1=int32#13,<r2=int32#7,<r2=int32#7,ROR #5
# asm 2: eor >t1=r12,<r2=r6,<r2=r6,ROR #5
eor r12,r6,r6,ROR #5

# qhasm: t1 ^= (r2 >>> 19)
# asm 1: eor >t1=int32#13,<t1=int32#13,<r2=int32#7,ROR #19
# asm 2: eor >t1=r12,<t1=r12,<r2=r6,ROR #19
eor r12,r12,r6,ROR #19

# qhasm: t2 = mem32[input_3 + 8]
# asm 1: ldr >t2=int32#14,[<input_3=int32#4,#8]
# asm 2: ldr >t2=r14,[<input_3=r3,#8]
ldr r14,[r3,#8]

# qhasm: t0 += w2
# asm 1: add >t0=int32#11,<t0=int32#12,<w2=int32#11
# asm 2: add >t0=r10,<t0=r11,<w2=r10
add r10,r11,r10

# qhasm: t0 += (t1 >>> 6)
# asm 1: add >t0=int32#11,<t0=int32#11,<t1=int32#13,ROR #6
# asm 2: add >t0=r10,<t0=r10,<t1=r12,ROR #6
add r10,r10,r12,ROR #6

# qhasm: t0 += t2
# asm 1: add >t0=int32#11,<t0=int32#11,<t2=int32#14
# asm 2: add >t0=r10,<t0=r10,<t2=r14
add r10,r10,r14

# qhasm: r5 += t0
# asm 1: add >r5=int32#10,<r5=int32#10,<t0=int32#11
# asm 2: add >r5=r9,<r5=r9,<t0=r10
add r9,r9,r10

# qhasm: r1 += r5
# asm 1: add >r1=int32#6,<r1=int32#6,<r5=int32#10
# asm 2: add >r1=r5,<r1=r5,<r5=r9
add r5,r5,r9

# qhasm: t0 = r6 & r7
# asm 1: and >t0=int32#11,<r6=int32#8,<r7=int32#1
# asm 2: and >t0=r10,<r6=r7,<r7=r0
and r10,r7,r0

# qhasm: t2 = r6 & r0
# asm 1: and >t2=int32#12,<r6=int32#8,<r0=int32#5
# asm 2: and >t2=r11,<r6=r7,<r0=r4
and r11,r7,r4

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t2 = r7 & r0
# asm 1: and >t2=int32#12,<r7=int32#1,<r0=int32#5
# asm 2: and >t2=r11,<r7=r0,<r0=r4
and r11,r0,r4

# qhasm: t3 = r6 ^ (r6 >>> 11)
# asm 1: eor >t3=int32#13,<r6=int32#8,<r6=int32#8,ROR #11
# asm 2: eor >t3=r12,<r6=r7,<r6=r7,ROR #11
eor r12,r7,r7,ROR #11

# qhasm: t3 ^= (r6 >>> 20)
# asm 1: eor >t3=int32#13,<t3=int32#13,<r6=int32#8,ROR #20
# asm 2: eor >t3=r12,<t3=r12,<r6=r7,ROR #20
eor r12,r12,r7,ROR #20

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t0 += (t3 >>> 2)
# asm 1: add >t0=int32#11,<t0=int32#11,<t3=int32#13,ROR #2
# asm 2: add >t0=r10,<t0=r10,<t3=r12,ROR #2
add r10,r10,r12,ROR #2

# qhasm: r5 += t0
# asm 1: add >r5=int32#10,<r5=int32#10,<t0=int32#11
# asm 2: add >r5=r9,<r5=r9,<t0=r10
add r9,r9,r10

# qhasm: w3 = w3_stack
# asm 1: ldr >w3=int32#11,<w3_stack=stack32#23
# asm 2: ldr >w3=r10,<w3_stack=[sp,#88]
ldr r10,[sp,#88]

# qhasm: t0 = r1 & r2
# asm 1: and >t0=int32#12,<r1=int32#6,<r2=int32#7
# asm 2: and >t0=r11,<r1=r5,<r2=r6
and r11,r5,r6

# qhasm: t1 = r3 & ~r1
# asm 1: bic >t1=int32#13,<r3=int32#3,<r1=int32#6
# asm 2: bic >t1=r12,<r3=r2,<r1=r5
bic r12,r2,r5

# qhasm: t0 ^= t1
# asm 1: eor >t0=int32#12,<t0=int32#12,<t1=int32#13
# asm 2: eor >t0=r11,<t0=r11,<t1=r12
eor r11,r11,r12

# qhasm: t1 = r1 ^ (r1 >>> 5)
# asm 1: eor >t1=int32#13,<r1=int32#6,<r1=int32#6,ROR #5
# asm 2: eor >t1=r12,<r1=r5,<r1=r5,ROR #5
eor r12,r5,r5,ROR #5

# qhasm: t1 ^= (r1 >>> 19)
# asm 1: eor >t1=int32#13,<t1=int32#13,<r1=int32#6,ROR #19
# asm 2: eor >t1=r12,<t1=r12,<r1=r5,ROR #19
eor r12,r12,r5,ROR #19

# qhasm: t2 = mem32[input_3 + 12]
# asm 1: ldr >t2=int32#14,[<input_3=int32#4,#12]
# asm 2: ldr >t2=r14,[<input_3=r3,#12]
ldr r14,[r3,#12]

# qhasm: t0 += w3
# asm 1: add >t0=int32#11,<t0=int32#12,<w3=int32#11
# asm 2: add >t0=r10,<t0=r11,<w3=r10
add r10,r11,r10

# qhasm: t0 += (t1 >>> 6)
# asm 1: add >t0=int32#11,<t0=int32#11,<t1=int32#13,ROR #6
# asm 2: add >t0=r10,<t0=r10,<t1=r12,ROR #6
add r10,r10,r12,ROR #6

# qhasm: t0 += t2
# asm 1: add >t0=int32#11,<t0=int32#11,<t2=int32#14
# asm 2: add >t0=r10,<t0=r10,<t2=r14
add r10,r10,r14

# qhasm: r4 += t0
# asm 1: add >r4=int32#9,<r4=int32#9,<t0=int32#11
# asm 2: add >r4=r8,<r4=r8,<t0=r10
add r8,r8,r10

# qhasm: r0 += r4
# asm 1: add >r0=int32#5,<r0=int32#5,<r4=int32#9
# asm 2: add >r0=r4,<r0=r4,<r4=r8
add r4,r4,r8

# qhasm: t0 = r5 & r6
# asm 1: and >t0=int32#11,<r5=int32#10,<r6=int32#8
# asm 2: and >t0=r10,<r5=r9,<r6=r7
and r10,r9,r7

# qhasm: t2 = r5 & r7
# asm 1: and >t2=int32#12,<r5=int32#10,<r7=int32#1
# asm 2: and >t2=r11,<r5=r9,<r7=r0
and r11,r9,r0

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t2 = r6 & r7
# asm 1: and >t2=int32#12,<r6=int32#8,<r7=int32#1
# asm 2: and >t2=r11,<r6=r7,<r7=r0
and r11,r7,r0

# qhasm: t3 = r5 ^ (r5 >>> 11)
# asm 1: eor >t3=int32#13,<r5=int32#10,<r5=int32#10,ROR #11
# asm 2: eor >t3=r12,<r5=r9,<r5=r9,ROR #11
eor r12,r9,r9,ROR #11

# qhasm: t3 ^= (r5 >>> 20)
# asm 1: eor >t3=int32#13,<t3=int32#13,<r5=int32#10,ROR #20
# asm 2: eor >t3=r12,<t3=r12,<r5=r9,ROR #20
eor r12,r12,r9,ROR #20

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t0 += (t3 >>> 2)
# asm 1: add >t0=int32#11,<t0=int32#11,<t3=int32#13,ROR #2
# asm 2: add >t0=r10,<t0=r10,<t3=r12,ROR #2
add r10,r10,r12,ROR #2

# qhasm: r4 += t0
# asm 1: add >r4=int32#9,<r4=int32#9,<t0=int32#11
# asm 2: add >r4=r8,<r4=r8,<t0=r10
add r8,r8,r10

# qhasm: w4 = w4_stack
# asm 1: ldr >w4=int32#11,<w4_stack=stack32#24
# asm 2: ldr >w4=r10,<w4_stack=[sp,#92]
ldr r10,[sp,#92]

# qhasm: t0 = r0 & r1
# asm 1: and >t0=int32#12,<r0=int32#5,<r1=int32#6
# asm 2: and >t0=r11,<r0=r4,<r1=r5
and r11,r4,r5

# qhasm: t1 = r2 & ~r0
# asm 1: bic >t1=int32#13,<r2=int32#7,<r0=int32#5
# asm 2: bic >t1=r12,<r2=r6,<r0=r4
bic r12,r6,r4

# qhasm: t0 ^= t1
# asm 1: eor >t0=int32#12,<t0=int32#12,<t1=int32#13
# asm 2: eor >t0=r11,<t0=r11,<t1=r12
eor r11,r11,r12

# qhasm: t1 = r0 ^ (r0 >>> 5)
# asm 1: eor >t1=int32#13,<r0=int32#5,<r0=int32#5,ROR #5
# asm 2: eor >t1=r12,<r0=r4,<r0=r4,ROR #5
eor r12,r4,r4,ROR #5

# qhasm: t1 ^= (r0 >>> 19)
# asm 1: eor >t1=int32#13,<t1=int32#13,<r0=int32#5,ROR #19
# asm 2: eor >t1=r12,<t1=r12,<r0=r4,ROR #19
eor r12,r12,r4,ROR #19

# qhasm: t2 = mem32[input_3 + 16]
# asm 1: ldr >t2=int32#14,[<input_3=int32#4,#16]
# asm 2: ldr >t2=r14,[<input_3=r3,#16]
ldr r14,[r3,#16]

# qhasm: t0 += w4
# asm 1: add >t0=int32#11,<t0=int32#12,<w4=int32#11
# asm 2: add >t0=r10,<t0=r11,<w4=r10
add r10,r11,r10

# qhasm: t0 += (t1 >>> 6)
# asm 1: add >t0=int32#11,<t0=int32#11,<t1=int32#13,ROR #6
# asm 2: add >t0=r10,<t0=r10,<t1=r12,ROR #6
add r10,r10,r12,ROR #6

# qhasm: t0 += t2
# asm 1: add >t0=int32#11,<t0=int32#11,<t2=int32#14
# asm 2: add >t0=r10,<t0=r10,<t2=r14
add r10,r10,r14

# qhasm: r3 += t0
# asm 1: add >r3=int32#3,<r3=int32#3,<t0=int32#11
# asm 2: add >r3=r2,<r3=r2,<t0=r10
add r2,r2,r10

# qhasm: r7 += r3
# asm 1: add >r7=int32#1,<r7=int32#1,<r3=int32#3
# asm 2: add >r7=r0,<r7=r0,<r3=r2
add r0,r0,r2

# qhasm: t0 = r4 & r5
# asm 1: and >t0=int32#11,<r4=int32#9,<r5=int32#10
# asm 2: and >t0=r10,<r4=r8,<r5=r9
and r10,r8,r9

# qhasm: t2 = r4 & r6
# asm 1: and >t2=int32#12,<r4=int32#9,<r6=int32#8
# asm 2: and >t2=r11,<r4=r8,<r6=r7
and r11,r8,r7

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t2 = r5 & r6
# asm 1: and >t2=int32#12,<r5=int32#10,<r6=int32#8
# asm 2: and >t2=r11,<r5=r9,<r6=r7
and r11,r9,r7

# qhasm: t3 = r4 ^ (r4 >>> 11)
# asm 1: eor >t3=int32#13,<r4=int32#9,<r4=int32#9,ROR #11
# asm 2: eor >t3=r12,<r4=r8,<r4=r8,ROR #11
eor r12,r8,r8,ROR #11

# qhasm: t3 ^= (r4 >>> 20)
# asm 1: eor >t3=int32#13,<t3=int32#13,<r4=int32#9,ROR #20
# asm 2: eor >t3=r12,<t3=r12,<r4=r8,ROR #20
eor r12,r12,r8,ROR #20

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t0 += (t3 >>> 2)
# asm 1: add >t0=int32#11,<t0=int32#11,<t3=int32#13,ROR #2
# asm 2: add >t0=r10,<t0=r10,<t3=r12,ROR #2
add r10,r10,r12,ROR #2

# qhasm: r3 += t0
# asm 1: add >r3=int32#3,<r3=int32#3,<t0=int32#11
# asm 2: add >r3=r2,<r3=r2,<t0=r10
add r2,r2,r10

# qhasm: w5 = w5_stack
# asm 1: ldr >w5=int32#11,<w5_stack=stack32#25
# asm 2: ldr >w5=r10,<w5_stack=[sp,#96]
ldr r10,[sp,#96]

# qhasm: t0 = r7 & r0
# asm 1: and >t0=int32#12,<r7=int32#1,<r0=int32#5
# asm 2: and >t0=r11,<r7=r0,<r0=r4
and r11,r0,r4

# qhasm: t1 = r1 & ~r7
# asm 1: bic >t1=int32#13,<r1=int32#6,<r7=int32#1
# asm 2: bic >t1=r12,<r1=r5,<r7=r0
bic r12,r5,r0

# qhasm: t0 ^= t1
# asm 1: eor >t0=int32#12,<t0=int32#12,<t1=int32#13
# asm 2: eor >t0=r11,<t0=r11,<t1=r12
eor r11,r11,r12

# qhasm: t1 = r7 ^ (r7 >>> 5)
# asm 1: eor >t1=int32#13,<r7=int32#1,<r7=int32#1,ROR #5
# asm 2: eor >t1=r12,<r7=r0,<r7=r0,ROR #5
eor r12,r0,r0,ROR #5

# qhasm: t1 ^= (r7 >>> 19)
# asm 1: eor >t1=int32#13,<t1=int32#13,<r7=int32#1,ROR #19
# asm 2: eor >t1=r12,<t1=r12,<r7=r0,ROR #19
eor r12,r12,r0,ROR #19

# qhasm: t2 = mem32[input_3 + 20]
# asm 1: ldr >t2=int32#14,[<input_3=int32#4,#20]
# asm 2: ldr >t2=r14,[<input_3=r3,#20]
ldr r14,[r3,#20]

# qhasm: t0 += w5
# asm 1: add >t0=int32#11,<t0=int32#12,<w5=int32#11
# asm 2: add >t0=r10,<t0=r11,<w5=r10
add r10,r11,r10

# qhasm: t0 += (t1 >>> 6)
# asm 1: add >t0=int32#11,<t0=int32#11,<t1=int32#13,ROR #6
# asm 2: add >t0=r10,<t0=r10,<t1=r12,ROR #6
add r10,r10,r12,ROR #6

# qhasm: t0 += t2
# asm 1: add >t0=int32#11,<t0=int32#11,<t2=int32#14
# asm 2: add >t0=r10,<t0=r10,<t2=r14
add r10,r10,r14

# qhasm: r2 += t0
# asm 1: add >r2=int32#7,<r2=int32#7,<t0=int32#11
# asm 2: add >r2=r6,<r2=r6,<t0=r10
add r6,r6,r10

# qhasm: r6 += r2
# asm 1: add >r6=int32#8,<r6=int32#8,<r2=int32#7
# asm 2: add >r6=r7,<r6=r7,<r2=r6
add r7,r7,r6

# qhasm: t0 = r3 & r4
# asm 1: and >t0=int32#11,<r3=int32#3,<r4=int32#9
# asm 2: and >t0=r10,<r3=r2,<r4=r8
and r10,r2,r8

# qhasm: t2 = r3 & r5
# asm 1: and >t2=int32#12,<r3=int32#3,<r5=int32#10
# asm 2: and >t2=r11,<r3=r2,<r5=r9
and r11,r2,r9

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t2 = r4 & r5
# asm 1: and >t2=int32#12,<r4=int32#9,<r5=int32#10
# asm 2: and >t2=r11,<r4=r8,<r5=r9
and r11,r8,r9

# qhasm: t3 = r3 ^ (r3 >>> 11)
# asm 1: eor >t3=int32#13,<r3=int32#3,<r3=int32#3,ROR #11
# asm 2: eor >t3=r12,<r3=r2,<r3=r2,ROR #11
eor r12,r2,r2,ROR #11

# qhasm: t3 ^= (r3 >>> 20)
# asm 1: eor >t3=int32#13,<t3=int32#13,<r3=int32#3,ROR #20
# asm 2: eor >t3=r12,<t3=r12,<r3=r2,ROR #20
eor r12,r12,r2,ROR #20

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t0 += (t3 >>> 2)
# asm 1: add >t0=int32#11,<t0=int32#11,<t3=int32#13,ROR #2
# asm 2: add >t0=r10,<t0=r10,<t3=r12,ROR #2
add r10,r10,r12,ROR #2

# qhasm: r2 += t0
# asm 1: add >r2=int32#7,<r2=int32#7,<t0=int32#11
# asm 2: add >r2=r6,<r2=r6,<t0=r10
add r6,r6,r10

# qhasm: w6 = w6_stack
# asm 1: ldr >w6=int32#11,<w6_stack=stack32#26
# asm 2: ldr >w6=r10,<w6_stack=[sp,#100]
ldr r10,[sp,#100]

# qhasm: t0 = r6 & r7
# asm 1: and >t0=int32#12,<r6=int32#8,<r7=int32#1
# asm 2: and >t0=r11,<r6=r7,<r7=r0
and r11,r7,r0

# qhasm: t1 = r0 & ~r6
# asm 1: bic >t1=int32#13,<r0=int32#5,<r6=int32#8
# asm 2: bic >t1=r12,<r0=r4,<r6=r7
bic r12,r4,r7

# qhasm: t0 ^= t1
# asm 1: eor >t0=int32#12,<t0=int32#12,<t1=int32#13
# asm 2: eor >t0=r11,<t0=r11,<t1=r12
eor r11,r11,r12

# qhasm: t1 = r6 ^ (r6 >>> 5)
# asm 1: eor >t1=int32#13,<r6=int32#8,<r6=int32#8,ROR #5
# asm 2: eor >t1=r12,<r6=r7,<r6=r7,ROR #5
eor r12,r7,r7,ROR #5

# qhasm: t1 ^= (r6 >>> 19)
# asm 1: eor >t1=int32#13,<t1=int32#13,<r6=int32#8,ROR #19
# asm 2: eor >t1=r12,<t1=r12,<r6=r7,ROR #19
eor r12,r12,r7,ROR #19

# qhasm: t2 = mem32[input_3 + 24]
# asm 1: ldr >t2=int32#14,[<input_3=int32#4,#24]
# asm 2: ldr >t2=r14,[<input_3=r3,#24]
ldr r14,[r3,#24]

# qhasm: t0 += w6
# asm 1: add >t0=int32#11,<t0=int32#12,<w6=int32#11
# asm 2: add >t0=r10,<t0=r11,<w6=r10
add r10,r11,r10

# qhasm: t0 += (t1 >>> 6)
# asm 1: add >t0=int32#11,<t0=int32#11,<t1=int32#13,ROR #6
# asm 2: add >t0=r10,<t0=r10,<t1=r12,ROR #6
add r10,r10,r12,ROR #6

# qhasm: t0 += t2
# asm 1: add >t0=int32#11,<t0=int32#11,<t2=int32#14
# asm 2: add >t0=r10,<t0=r10,<t2=r14
add r10,r10,r14

# qhasm: r1 += t0
# asm 1: add >r1=int32#6,<r1=int32#6,<t0=int32#11
# asm 2: add >r1=r5,<r1=r5,<t0=r10
add r5,r5,r10

# qhasm: r5 += r1
# asm 1: add >r5=int32#10,<r5=int32#10,<r1=int32#6
# asm 2: add >r5=r9,<r5=r9,<r1=r5
add r9,r9,r5

# qhasm: t0 = r2 & r3
# asm 1: and >t0=int32#11,<r2=int32#7,<r3=int32#3
# asm 2: and >t0=r10,<r2=r6,<r3=r2
and r10,r6,r2

# qhasm: t2 = r2 & r4
# asm 1: and >t2=int32#12,<r2=int32#7,<r4=int32#9
# asm 2: and >t2=r11,<r2=r6,<r4=r8
and r11,r6,r8

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t2 = r3 & r4
# asm 1: and >t2=int32#12,<r3=int32#3,<r4=int32#9
# asm 2: and >t2=r11,<r3=r2,<r4=r8
and r11,r2,r8

# qhasm: t3 = r2 ^ (r2 >>> 11)
# asm 1: eor >t3=int32#13,<r2=int32#7,<r2=int32#7,ROR #11
# asm 2: eor >t3=r12,<r2=r6,<r2=r6,ROR #11
eor r12,r6,r6,ROR #11

# qhasm: t3 ^= (r2 >>> 20)
# asm 1: eor >t3=int32#13,<t3=int32#13,<r2=int32#7,ROR #20
# asm 2: eor >t3=r12,<t3=r12,<r2=r6,ROR #20
eor r12,r12,r6,ROR #20

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t0 += (t3 >>> 2)
# asm 1: add >t0=int32#11,<t0=int32#11,<t3=int32#13,ROR #2
# asm 2: add >t0=r10,<t0=r10,<t3=r12,ROR #2
add r10,r10,r12,ROR #2

# qhasm: r1 += t0
# asm 1: add >r1=int32#6,<r1=int32#6,<t0=int32#11
# asm 2: add >r1=r5,<r1=r5,<t0=r10
add r5,r5,r10

# qhasm: w7 = w7_stack
# asm 1: ldr >w7=int32#11,<w7_stack=stack32#27
# asm 2: ldr >w7=r10,<w7_stack=[sp,#104]
ldr r10,[sp,#104]

# qhasm: t0 = r5 & r6
# asm 1: and >t0=int32#12,<r5=int32#10,<r6=int32#8
# asm 2: and >t0=r11,<r5=r9,<r6=r7
and r11,r9,r7

# qhasm: t1 = r7 & ~r5
# asm 1: bic >t1=int32#13,<r7=int32#1,<r5=int32#10
# asm 2: bic >t1=r12,<r7=r0,<r5=r9
bic r12,r0,r9

# qhasm: t0 ^= t1
# asm 1: eor >t0=int32#12,<t0=int32#12,<t1=int32#13
# asm 2: eor >t0=r11,<t0=r11,<t1=r12
eor r11,r11,r12

# qhasm: t1 = r5 ^ (r5 >>> 5)
# asm 1: eor >t1=int32#13,<r5=int32#10,<r5=int32#10,ROR #5
# asm 2: eor >t1=r12,<r5=r9,<r5=r9,ROR #5
eor r12,r9,r9,ROR #5

# qhasm: t1 ^= (r5 >>> 19)
# asm 1: eor >t1=int32#13,<t1=int32#13,<r5=int32#10,ROR #19
# asm 2: eor >t1=r12,<t1=r12,<r5=r9,ROR #19
eor r12,r12,r9,ROR #19

# qhasm: t2 = mem32[input_3 + 28]
# asm 1: ldr >t2=int32#14,[<input_3=int32#4,#28]
# asm 2: ldr >t2=r14,[<input_3=r3,#28]
ldr r14,[r3,#28]

# qhasm: t0 += w7
# asm 1: add >t0=int32#11,<t0=int32#12,<w7=int32#11
# asm 2: add >t0=r10,<t0=r11,<w7=r10
add r10,r11,r10

# qhasm: t0 += (t1 >>> 6)
# asm 1: add >t0=int32#11,<t0=int32#11,<t1=int32#13,ROR #6
# asm 2: add >t0=r10,<t0=r10,<t1=r12,ROR #6
add r10,r10,r12,ROR #6

# qhasm: t0 += t2
# asm 1: add >t0=int32#11,<t0=int32#11,<t2=int32#14
# asm 2: add >t0=r10,<t0=r10,<t2=r14
add r10,r10,r14

# qhasm: r0 += t0
# asm 1: add >r0=int32#5,<r0=int32#5,<t0=int32#11
# asm 2: add >r0=r4,<r0=r4,<t0=r10
add r4,r4,r10

# qhasm: r4 += r0
# asm 1: add >r4=int32#9,<r4=int32#9,<r0=int32#5
# asm 2: add >r4=r8,<r4=r8,<r0=r4
add r8,r8,r4

# qhasm: t0 = r1 & r2
# asm 1: and >t0=int32#11,<r1=int32#6,<r2=int32#7
# asm 2: and >t0=r10,<r1=r5,<r2=r6
and r10,r5,r6

# qhasm: t2 = r1 & r3
# asm 1: and >t2=int32#12,<r1=int32#6,<r3=int32#3
# asm 2: and >t2=r11,<r1=r5,<r3=r2
and r11,r5,r2

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t2 = r2 & r3
# asm 1: and >t2=int32#12,<r2=int32#7,<r3=int32#3
# asm 2: and >t2=r11,<r2=r6,<r3=r2
and r11,r6,r2

# qhasm: t3 = r1 ^ (r1 >>> 11)
# asm 1: eor >t3=int32#13,<r1=int32#6,<r1=int32#6,ROR #11
# asm 2: eor >t3=r12,<r1=r5,<r1=r5,ROR #11
eor r12,r5,r5,ROR #11

# qhasm: t3 ^= (r1 >>> 20)
# asm 1: eor >t3=int32#13,<t3=int32#13,<r1=int32#6,ROR #20
# asm 2: eor >t3=r12,<t3=r12,<r1=r5,ROR #20
eor r12,r12,r5,ROR #20

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t0 += (t3 >>> 2)
# asm 1: add >t0=int32#11,<t0=int32#11,<t3=int32#13,ROR #2
# asm 2: add >t0=r10,<t0=r10,<t3=r12,ROR #2
add r10,r10,r12,ROR #2

# qhasm: r0 += t0
# asm 1: add >r0=int32#5,<r0=int32#5,<t0=int32#11
# asm 2: add >r0=r4,<r0=r4,<t0=r10
add r4,r4,r10

# qhasm: w8 = w8_stack
# asm 1: ldr >w8=int32#11,<w8_stack=stack32#28
# asm 2: ldr >w8=r10,<w8_stack=[sp,#108]
ldr r10,[sp,#108]

# qhasm: t0 = r4 & r5
# asm 1: and >t0=int32#12,<r4=int32#9,<r5=int32#10
# asm 2: and >t0=r11,<r4=r8,<r5=r9
and r11,r8,r9

# qhasm: t1 = r6 & ~r4
# asm 1: bic >t1=int32#13,<r6=int32#8,<r4=int32#9
# asm 2: bic >t1=r12,<r6=r7,<r4=r8
bic r12,r7,r8

# qhasm: t0 ^= t1
# asm 1: eor >t0=int32#12,<t0=int32#12,<t1=int32#13
# asm 2: eor >t0=r11,<t0=r11,<t1=r12
eor r11,r11,r12

# qhasm: t1 = r4 ^ (r4 >>> 5)
# asm 1: eor >t1=int32#13,<r4=int32#9,<r4=int32#9,ROR #5
# asm 2: eor >t1=r12,<r4=r8,<r4=r8,ROR #5
eor r12,r8,r8,ROR #5

# qhasm: t1 ^= (r4 >>> 19)
# asm 1: eor >t1=int32#13,<t1=int32#13,<r4=int32#9,ROR #19
# asm 2: eor >t1=r12,<t1=r12,<r4=r8,ROR #19
eor r12,r12,r8,ROR #19

# qhasm: t2 = mem32[input_3 + 32]
# asm 1: ldr >t2=int32#14,[<input_3=int32#4,#32]
# asm 2: ldr >t2=r14,[<input_3=r3,#32]
ldr r14,[r3,#32]

# qhasm: t0 += w8
# asm 1: add >t0=int32#11,<t0=int32#12,<w8=int32#11
# asm 2: add >t0=r10,<t0=r11,<w8=r10
add r10,r11,r10

# qhasm: t0 += (t1 >>> 6)
# asm 1: add >t0=int32#11,<t0=int32#11,<t1=int32#13,ROR #6
# asm 2: add >t0=r10,<t0=r10,<t1=r12,ROR #6
add r10,r10,r12,ROR #6

# qhasm: t0 += t2
# asm 1: add >t0=int32#11,<t0=int32#11,<t2=int32#14
# asm 2: add >t0=r10,<t0=r10,<t2=r14
add r10,r10,r14

# qhasm: r7 += t0
# asm 1: add >r7=int32#1,<r7=int32#1,<t0=int32#11
# asm 2: add >r7=r0,<r7=r0,<t0=r10
add r0,r0,r10

# qhasm: r3 += r7
# asm 1: add >r3=int32#3,<r3=int32#3,<r7=int32#1
# asm 2: add >r3=r2,<r3=r2,<r7=r0
add r2,r2,r0

# qhasm: t0 = r0 & r1
# asm 1: and >t0=int32#11,<r0=int32#5,<r1=int32#6
# asm 2: and >t0=r10,<r0=r4,<r1=r5
and r10,r4,r5

# qhasm: t2 = r0 & r2
# asm 1: and >t2=int32#12,<r0=int32#5,<r2=int32#7
# asm 2: and >t2=r11,<r0=r4,<r2=r6
and r11,r4,r6

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t2 = r1 & r2
# asm 1: and >t2=int32#12,<r1=int32#6,<r2=int32#7
# asm 2: and >t2=r11,<r1=r5,<r2=r6
and r11,r5,r6

# qhasm: t3 = r0 ^ (r0 >>> 11)
# asm 1: eor >t3=int32#13,<r0=int32#5,<r0=int32#5,ROR #11
# asm 2: eor >t3=r12,<r0=r4,<r0=r4,ROR #11
eor r12,r4,r4,ROR #11

# qhasm: t3 ^= (r0 >>> 20)
# asm 1: eor >t3=int32#13,<t3=int32#13,<r0=int32#5,ROR #20
# asm 2: eor >t3=r12,<t3=r12,<r0=r4,ROR #20
eor r12,r12,r4,ROR #20

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t0 += (t3 >>> 2)
# asm 1: add >t0=int32#11,<t0=int32#11,<t3=int32#13,ROR #2
# asm 2: add >t0=r10,<t0=r10,<t3=r12,ROR #2
add r10,r10,r12,ROR #2

# qhasm: r7 += t0
# asm 1: add >r7=int32#1,<r7=int32#1,<t0=int32#11
# asm 2: add >r7=r0,<r7=r0,<t0=r10
add r0,r0,r10

# qhasm: w9 = w9_stack
# asm 1: ldr >w9=int32#11,<w9_stack=stack32#29
# asm 2: ldr >w9=r10,<w9_stack=[sp,#112]
ldr r10,[sp,#112]

# qhasm: t0 = r3 & r4
# asm 1: and >t0=int32#12,<r3=int32#3,<r4=int32#9
# asm 2: and >t0=r11,<r3=r2,<r4=r8
and r11,r2,r8

# qhasm: t1 = r5 & ~r3
# asm 1: bic >t1=int32#13,<r5=int32#10,<r3=int32#3
# asm 2: bic >t1=r12,<r5=r9,<r3=r2
bic r12,r9,r2

# qhasm: t0 ^= t1
# asm 1: eor >t0=int32#12,<t0=int32#12,<t1=int32#13
# asm 2: eor >t0=r11,<t0=r11,<t1=r12
eor r11,r11,r12

# qhasm: t1 = r3 ^ (r3 >>> 5)
# asm 1: eor >t1=int32#13,<r3=int32#3,<r3=int32#3,ROR #5
# asm 2: eor >t1=r12,<r3=r2,<r3=r2,ROR #5
eor r12,r2,r2,ROR #5

# qhasm: t1 ^= (r3 >>> 19)
# asm 1: eor >t1=int32#13,<t1=int32#13,<r3=int32#3,ROR #19
# asm 2: eor >t1=r12,<t1=r12,<r3=r2,ROR #19
eor r12,r12,r2,ROR #19

# qhasm: t2 = mem32[input_3 + 36]
# asm 1: ldr >t2=int32#14,[<input_3=int32#4,#36]
# asm 2: ldr >t2=r14,[<input_3=r3,#36]
ldr r14,[r3,#36]

# qhasm: t0 += w9
# asm 1: add >t0=int32#11,<t0=int32#12,<w9=int32#11
# asm 2: add >t0=r10,<t0=r11,<w9=r10
add r10,r11,r10

# qhasm: t0 += (t1 >>> 6)
# asm 1: add >t0=int32#11,<t0=int32#11,<t1=int32#13,ROR #6
# asm 2: add >t0=r10,<t0=r10,<t1=r12,ROR #6
add r10,r10,r12,ROR #6

# qhasm: t0 += t2
# asm 1: add >t0=int32#11,<t0=int32#11,<t2=int32#14
# asm 2: add >t0=r10,<t0=r10,<t2=r14
add r10,r10,r14

# qhasm: r6 += t0
# asm 1: add >r6=int32#8,<r6=int32#8,<t0=int32#11
# asm 2: add >r6=r7,<r6=r7,<t0=r10
add r7,r7,r10

# qhasm: r2 += r6
# asm 1: add >r2=int32#7,<r2=int32#7,<r6=int32#8
# asm 2: add >r2=r6,<r2=r6,<r6=r7
add r6,r6,r7

# qhasm: t0 = r7 & r0
# asm 1: and >t0=int32#11,<r7=int32#1,<r0=int32#5
# asm 2: and >t0=r10,<r7=r0,<r0=r4
and r10,r0,r4

# qhasm: t2 = r7 & r1
# asm 1: and >t2=int32#12,<r7=int32#1,<r1=int32#6
# asm 2: and >t2=r11,<r7=r0,<r1=r5
and r11,r0,r5

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t2 = r0 & r1
# asm 1: and >t2=int32#12,<r0=int32#5,<r1=int32#6
# asm 2: and >t2=r11,<r0=r4,<r1=r5
and r11,r4,r5

# qhasm: t3 = r7 ^ (r7 >>> 11)
# asm 1: eor >t3=int32#13,<r7=int32#1,<r7=int32#1,ROR #11
# asm 2: eor >t3=r12,<r7=r0,<r7=r0,ROR #11
eor r12,r0,r0,ROR #11

# qhasm: t3 ^= (r7 >>> 20)
# asm 1: eor >t3=int32#13,<t3=int32#13,<r7=int32#1,ROR #20
# asm 2: eor >t3=r12,<t3=r12,<r7=r0,ROR #20
eor r12,r12,r0,ROR #20

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t0 += (t3 >>> 2)
# asm 1: add >t0=int32#11,<t0=int32#11,<t3=int32#13,ROR #2
# asm 2: add >t0=r10,<t0=r10,<t3=r12,ROR #2
add r10,r10,r12,ROR #2

# qhasm: r6 += t0
# asm 1: add >r6=int32#8,<r6=int32#8,<t0=int32#11
# asm 2: add >r6=r7,<r6=r7,<t0=r10
add r7,r7,r10

# qhasm: w10 = w10_stack
# asm 1: ldr >w10=int32#11,<w10_stack=stack32#30
# asm 2: ldr >w10=r10,<w10_stack=[sp,#116]
ldr r10,[sp,#116]

# qhasm: t0 = r2 & r3
# asm 1: and >t0=int32#12,<r2=int32#7,<r3=int32#3
# asm 2: and >t0=r11,<r2=r6,<r3=r2
and r11,r6,r2

# qhasm: t1 = r4 & ~r2
# asm 1: bic >t1=int32#13,<r4=int32#9,<r2=int32#7
# asm 2: bic >t1=r12,<r4=r8,<r2=r6
bic r12,r8,r6

# qhasm: t0 ^= t1
# asm 1: eor >t0=int32#12,<t0=int32#12,<t1=int32#13
# asm 2: eor >t0=r11,<t0=r11,<t1=r12
eor r11,r11,r12

# qhasm: t1 = r2 ^ (r2 >>> 5)
# asm 1: eor >t1=int32#13,<r2=int32#7,<r2=int32#7,ROR #5
# asm 2: eor >t1=r12,<r2=r6,<r2=r6,ROR #5
eor r12,r6,r6,ROR #5

# qhasm: t1 ^= (r2 >>> 19)
# asm 1: eor >t1=int32#13,<t1=int32#13,<r2=int32#7,ROR #19
# asm 2: eor >t1=r12,<t1=r12,<r2=r6,ROR #19
eor r12,r12,r6,ROR #19

# qhasm: t2 = mem32[input_3 + 40]
# asm 1: ldr >t2=int32#14,[<input_3=int32#4,#40]
# asm 2: ldr >t2=r14,[<input_3=r3,#40]
ldr r14,[r3,#40]

# qhasm: t0 += w10
# asm 1: add >t0=int32#11,<t0=int32#12,<w10=int32#11
# asm 2: add >t0=r10,<t0=r11,<w10=r10
add r10,r11,r10

# qhasm: t0 += (t1 >>> 6)
# asm 1: add >t0=int32#11,<t0=int32#11,<t1=int32#13,ROR #6
# asm 2: add >t0=r10,<t0=r10,<t1=r12,ROR #6
add r10,r10,r12,ROR #6

# qhasm: t0 += t2
# asm 1: add >t0=int32#11,<t0=int32#11,<t2=int32#14
# asm 2: add >t0=r10,<t0=r10,<t2=r14
add r10,r10,r14

# qhasm: r5 += t0
# asm 1: add >r5=int32#10,<r5=int32#10,<t0=int32#11
# asm 2: add >r5=r9,<r5=r9,<t0=r10
add r9,r9,r10

# qhasm: r1 += r5
# asm 1: add >r1=int32#6,<r1=int32#6,<r5=int32#10
# asm 2: add >r1=r5,<r1=r5,<r5=r9
add r5,r5,r9

# qhasm: t0 = r6 & r7
# asm 1: and >t0=int32#11,<r6=int32#8,<r7=int32#1
# asm 2: and >t0=r10,<r6=r7,<r7=r0
and r10,r7,r0

# qhasm: t2 = r6 & r0
# asm 1: and >t2=int32#12,<r6=int32#8,<r0=int32#5
# asm 2: and >t2=r11,<r6=r7,<r0=r4
and r11,r7,r4

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t2 = r7 & r0
# asm 1: and >t2=int32#12,<r7=int32#1,<r0=int32#5
# asm 2: and >t2=r11,<r7=r0,<r0=r4
and r11,r0,r4

# qhasm: t3 = r6 ^ (r6 >>> 11)
# asm 1: eor >t3=int32#13,<r6=int32#8,<r6=int32#8,ROR #11
# asm 2: eor >t3=r12,<r6=r7,<r6=r7,ROR #11
eor r12,r7,r7,ROR #11

# qhasm: t3 ^= (r6 >>> 20)
# asm 1: eor >t3=int32#13,<t3=int32#13,<r6=int32#8,ROR #20
# asm 2: eor >t3=r12,<t3=r12,<r6=r7,ROR #20
eor r12,r12,r7,ROR #20

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t0 += (t3 >>> 2)
# asm 1: add >t0=int32#11,<t0=int32#11,<t3=int32#13,ROR #2
# asm 2: add >t0=r10,<t0=r10,<t3=r12,ROR #2
add r10,r10,r12,ROR #2

# qhasm: r5 += t0
# asm 1: add >r5=int32#10,<r5=int32#10,<t0=int32#11
# asm 2: add >r5=r9,<r5=r9,<t0=r10
add r9,r9,r10

# qhasm: w11 = w11_stack
# asm 1: ldr >w11=int32#11,<w11_stack=stack32#31
# asm 2: ldr >w11=r10,<w11_stack=[sp,#120]
ldr r10,[sp,#120]

# qhasm: t0 = r1 & r2
# asm 1: and >t0=int32#12,<r1=int32#6,<r2=int32#7
# asm 2: and >t0=r11,<r1=r5,<r2=r6
and r11,r5,r6

# qhasm: t1 = r3 & ~r1
# asm 1: bic >t1=int32#13,<r3=int32#3,<r1=int32#6
# asm 2: bic >t1=r12,<r3=r2,<r1=r5
bic r12,r2,r5

# qhasm: t0 ^= t1
# asm 1: eor >t0=int32#12,<t0=int32#12,<t1=int32#13
# asm 2: eor >t0=r11,<t0=r11,<t1=r12
eor r11,r11,r12

# qhasm: t1 = r1 ^ (r1 >>> 5)
# asm 1: eor >t1=int32#13,<r1=int32#6,<r1=int32#6,ROR #5
# asm 2: eor >t1=r12,<r1=r5,<r1=r5,ROR #5
eor r12,r5,r5,ROR #5

# qhasm: t1 ^= (r1 >>> 19)
# asm 1: eor >t1=int32#13,<t1=int32#13,<r1=int32#6,ROR #19
# asm 2: eor >t1=r12,<t1=r12,<r1=r5,ROR #19
eor r12,r12,r5,ROR #19

# qhasm: t2 = mem32[input_3 + 44]
# asm 1: ldr >t2=int32#14,[<input_3=int32#4,#44]
# asm 2: ldr >t2=r14,[<input_3=r3,#44]
ldr r14,[r3,#44]

# qhasm: t0 += w11
# asm 1: add >t0=int32#11,<t0=int32#12,<w11=int32#11
# asm 2: add >t0=r10,<t0=r11,<w11=r10
add r10,r11,r10

# qhasm: t0 += (t1 >>> 6)
# asm 1: add >t0=int32#11,<t0=int32#11,<t1=int32#13,ROR #6
# asm 2: add >t0=r10,<t0=r10,<t1=r12,ROR #6
add r10,r10,r12,ROR #6

# qhasm: t0 += t2
# asm 1: add >t0=int32#11,<t0=int32#11,<t2=int32#14
# asm 2: add >t0=r10,<t0=r10,<t2=r14
add r10,r10,r14

# qhasm: r4 += t0
# asm 1: add >r4=int32#9,<r4=int32#9,<t0=int32#11
# asm 2: add >r4=r8,<r4=r8,<t0=r10
add r8,r8,r10

# qhasm: r0 += r4
# asm 1: add >r0=int32#5,<r0=int32#5,<r4=int32#9
# asm 2: add >r0=r4,<r0=r4,<r4=r8
add r4,r4,r8

# qhasm: t0 = r5 & r6
# asm 1: and >t0=int32#11,<r5=int32#10,<r6=int32#8
# asm 2: and >t0=r10,<r5=r9,<r6=r7
and r10,r9,r7

# qhasm: t2 = r5 & r7
# asm 1: and >t2=int32#12,<r5=int32#10,<r7=int32#1
# asm 2: and >t2=r11,<r5=r9,<r7=r0
and r11,r9,r0

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t2 = r6 & r7
# asm 1: and >t2=int32#12,<r6=int32#8,<r7=int32#1
# asm 2: and >t2=r11,<r6=r7,<r7=r0
and r11,r7,r0

# qhasm: t3 = r5 ^ (r5 >>> 11)
# asm 1: eor >t3=int32#13,<r5=int32#10,<r5=int32#10,ROR #11
# asm 2: eor >t3=r12,<r5=r9,<r5=r9,ROR #11
eor r12,r9,r9,ROR #11

# qhasm: t3 ^= (r5 >>> 20)
# asm 1: eor >t3=int32#13,<t3=int32#13,<r5=int32#10,ROR #20
# asm 2: eor >t3=r12,<t3=r12,<r5=r9,ROR #20
eor r12,r12,r9,ROR #20

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t0 += (t3 >>> 2)
# asm 1: add >t0=int32#11,<t0=int32#11,<t3=int32#13,ROR #2
# asm 2: add >t0=r10,<t0=r10,<t3=r12,ROR #2
add r10,r10,r12,ROR #2

# qhasm: r4 += t0
# asm 1: add >r4=int32#9,<r4=int32#9,<t0=int32#11
# asm 2: add >r4=r8,<r4=r8,<t0=r10
add r8,r8,r10

# qhasm: w12 = w12_stack
# asm 1: ldr >w12=int32#11,<w12_stack=stack32#32
# asm 2: ldr >w12=r10,<w12_stack=[sp,#124]
ldr r10,[sp,#124]

# qhasm: t0 = r0 & r1
# asm 1: and >t0=int32#12,<r0=int32#5,<r1=int32#6
# asm 2: and >t0=r11,<r0=r4,<r1=r5
and r11,r4,r5

# qhasm: t1 = r2 & ~r0
# asm 1: bic >t1=int32#13,<r2=int32#7,<r0=int32#5
# asm 2: bic >t1=r12,<r2=r6,<r0=r4
bic r12,r6,r4

# qhasm: t0 ^= t1
# asm 1: eor >t0=int32#12,<t0=int32#12,<t1=int32#13
# asm 2: eor >t0=r11,<t0=r11,<t1=r12
eor r11,r11,r12

# qhasm: t1 = r0 ^ (r0 >>> 5)
# asm 1: eor >t1=int32#13,<r0=int32#5,<r0=int32#5,ROR #5
# asm 2: eor >t1=r12,<r0=r4,<r0=r4,ROR #5
eor r12,r4,r4,ROR #5

# qhasm: t1 ^= (r0 >>> 19)
# asm 1: eor >t1=int32#13,<t1=int32#13,<r0=int32#5,ROR #19
# asm 2: eor >t1=r12,<t1=r12,<r0=r4,ROR #19
eor r12,r12,r4,ROR #19

# qhasm: t2 = mem32[input_3 + 48]
# asm 1: ldr >t2=int32#14,[<input_3=int32#4,#48]
# asm 2: ldr >t2=r14,[<input_3=r3,#48]
ldr r14,[r3,#48]

# qhasm: t0 += w12
# asm 1: add >t0=int32#11,<t0=int32#12,<w12=int32#11
# asm 2: add >t0=r10,<t0=r11,<w12=r10
add r10,r11,r10

# qhasm: t0 += (t1 >>> 6)
# asm 1: add >t0=int32#11,<t0=int32#11,<t1=int32#13,ROR #6
# asm 2: add >t0=r10,<t0=r10,<t1=r12,ROR #6
add r10,r10,r12,ROR #6

# qhasm: t0 += t2
# asm 1: add >t0=int32#11,<t0=int32#11,<t2=int32#14
# asm 2: add >t0=r10,<t0=r10,<t2=r14
add r10,r10,r14

# qhasm: r3 += t0
# asm 1: add >r3=int32#3,<r3=int32#3,<t0=int32#11
# asm 2: add >r3=r2,<r3=r2,<t0=r10
add r2,r2,r10

# qhasm: r7 += r3
# asm 1: add >r7=int32#1,<r7=int32#1,<r3=int32#3
# asm 2: add >r7=r0,<r7=r0,<r3=r2
add r0,r0,r2

# qhasm: t0 = r4 & r5
# asm 1: and >t0=int32#11,<r4=int32#9,<r5=int32#10
# asm 2: and >t0=r10,<r4=r8,<r5=r9
and r10,r8,r9

# qhasm: t2 = r4 & r6
# asm 1: and >t2=int32#12,<r4=int32#9,<r6=int32#8
# asm 2: and >t2=r11,<r4=r8,<r6=r7
and r11,r8,r7

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t2 = r5 & r6
# asm 1: and >t2=int32#12,<r5=int32#10,<r6=int32#8
# asm 2: and >t2=r11,<r5=r9,<r6=r7
and r11,r9,r7

# qhasm: t3 = r4 ^ (r4 >>> 11)
# asm 1: eor >t3=int32#13,<r4=int32#9,<r4=int32#9,ROR #11
# asm 2: eor >t3=r12,<r4=r8,<r4=r8,ROR #11
eor r12,r8,r8,ROR #11

# qhasm: t3 ^= (r4 >>> 20)
# asm 1: eor >t3=int32#13,<t3=int32#13,<r4=int32#9,ROR #20
# asm 2: eor >t3=r12,<t3=r12,<r4=r8,ROR #20
eor r12,r12,r8,ROR #20

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t0 += (t3 >>> 2)
# asm 1: add >t0=int32#11,<t0=int32#11,<t3=int32#13,ROR #2
# asm 2: add >t0=r10,<t0=r10,<t3=r12,ROR #2
add r10,r10,r12,ROR #2

# qhasm: r3 += t0
# asm 1: add >r3=int32#3,<r3=int32#3,<t0=int32#11
# asm 2: add >r3=r2,<r3=r2,<t0=r10
add r2,r2,r10

# qhasm: w13 = w13_stack
# asm 1: ldr >w13=int32#11,<w13_stack=stack32#33
# asm 2: ldr >w13=r10,<w13_stack=[sp,#128]
ldr r10,[sp,#128]

# qhasm: t0 = r7 & r0
# asm 1: and >t0=int32#12,<r7=int32#1,<r0=int32#5
# asm 2: and >t0=r11,<r7=r0,<r0=r4
and r11,r0,r4

# qhasm: t1 = r1 & ~r7
# asm 1: bic >t1=int32#13,<r1=int32#6,<r7=int32#1
# asm 2: bic >t1=r12,<r1=r5,<r7=r0
bic r12,r5,r0

# qhasm: t0 ^= t1
# asm 1: eor >t0=int32#12,<t0=int32#12,<t1=int32#13
# asm 2: eor >t0=r11,<t0=r11,<t1=r12
eor r11,r11,r12

# qhasm: t1 = r7 ^ (r7 >>> 5)
# asm 1: eor >t1=int32#13,<r7=int32#1,<r7=int32#1,ROR #5
# asm 2: eor >t1=r12,<r7=r0,<r7=r0,ROR #5
eor r12,r0,r0,ROR #5

# qhasm: t1 ^= (r7 >>> 19)
# asm 1: eor >t1=int32#13,<t1=int32#13,<r7=int32#1,ROR #19
# asm 2: eor >t1=r12,<t1=r12,<r7=r0,ROR #19
eor r12,r12,r0,ROR #19

# qhasm: t2 = mem32[input_3 + 52]
# asm 1: ldr >t2=int32#14,[<input_3=int32#4,#52]
# asm 2: ldr >t2=r14,[<input_3=r3,#52]
ldr r14,[r3,#52]

# qhasm: t0 += w13
# asm 1: add >t0=int32#11,<t0=int32#12,<w13=int32#11
# asm 2: add >t0=r10,<t0=r11,<w13=r10
add r10,r11,r10

# qhasm: t0 += (t1 >>> 6)
# asm 1: add >t0=int32#11,<t0=int32#11,<t1=int32#13,ROR #6
# asm 2: add >t0=r10,<t0=r10,<t1=r12,ROR #6
add r10,r10,r12,ROR #6

# qhasm: t0 += t2
# asm 1: add >t0=int32#11,<t0=int32#11,<t2=int32#14
# asm 2: add >t0=r10,<t0=r10,<t2=r14
add r10,r10,r14

# qhasm: r2 += t0
# asm 1: add >r2=int32#7,<r2=int32#7,<t0=int32#11
# asm 2: add >r2=r6,<r2=r6,<t0=r10
add r6,r6,r10

# qhasm: r6 += r2
# asm 1: add >r6=int32#8,<r6=int32#8,<r2=int32#7
# asm 2: add >r6=r7,<r6=r7,<r2=r6
add r7,r7,r6

# qhasm: t0 = r3 & r4
# asm 1: and >t0=int32#11,<r3=int32#3,<r4=int32#9
# asm 2: and >t0=r10,<r3=r2,<r4=r8
and r10,r2,r8

# qhasm: t2 = r3 & r5
# asm 1: and >t2=int32#12,<r3=int32#3,<r5=int32#10
# asm 2: and >t2=r11,<r3=r2,<r5=r9
and r11,r2,r9

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t2 = r4 & r5
# asm 1: and >t2=int32#12,<r4=int32#9,<r5=int32#10
# asm 2: and >t2=r11,<r4=r8,<r5=r9
and r11,r8,r9

# qhasm: t3 = r3 ^ (r3 >>> 11)
# asm 1: eor >t3=int32#13,<r3=int32#3,<r3=int32#3,ROR #11
# asm 2: eor >t3=r12,<r3=r2,<r3=r2,ROR #11
eor r12,r2,r2,ROR #11

# qhasm: t3 ^= (r3 >>> 20)
# asm 1: eor >t3=int32#13,<t3=int32#13,<r3=int32#3,ROR #20
# asm 2: eor >t3=r12,<t3=r12,<r3=r2,ROR #20
eor r12,r12,r2,ROR #20

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t0 += (t3 >>> 2)
# asm 1: add >t0=int32#11,<t0=int32#11,<t3=int32#13,ROR #2
# asm 2: add >t0=r10,<t0=r10,<t3=r12,ROR #2
add r10,r10,r12,ROR #2

# qhasm: r2 += t0
# asm 1: add >r2=int32#7,<r2=int32#7,<t0=int32#11
# asm 2: add >r2=r6,<r2=r6,<t0=r10
add r6,r6,r10

# qhasm: w14 = w14_stack
# asm 1: ldr >w14=int32#11,<w14_stack=stack32#34
# asm 2: ldr >w14=r10,<w14_stack=[sp,#132]
ldr r10,[sp,#132]

# qhasm: t0 = r6 & r7
# asm 1: and >t0=int32#12,<r6=int32#8,<r7=int32#1
# asm 2: and >t0=r11,<r6=r7,<r7=r0
and r11,r7,r0

# qhasm: t1 = r0 & ~r6
# asm 1: bic >t1=int32#13,<r0=int32#5,<r6=int32#8
# asm 2: bic >t1=r12,<r0=r4,<r6=r7
bic r12,r4,r7

# qhasm: t0 ^= t1
# asm 1: eor >t0=int32#12,<t0=int32#12,<t1=int32#13
# asm 2: eor >t0=r11,<t0=r11,<t1=r12
eor r11,r11,r12

# qhasm: t1 = r6 ^ (r6 >>> 5)
# asm 1: eor >t1=int32#13,<r6=int32#8,<r6=int32#8,ROR #5
# asm 2: eor >t1=r12,<r6=r7,<r6=r7,ROR #5
eor r12,r7,r7,ROR #5

# qhasm: t1 ^= (r6 >>> 19)
# asm 1: eor >t1=int32#13,<t1=int32#13,<r6=int32#8,ROR #19
# asm 2: eor >t1=r12,<t1=r12,<r6=r7,ROR #19
eor r12,r12,r7,ROR #19

# qhasm: t2 = mem32[input_3 + 56]
# asm 1: ldr >t2=int32#14,[<input_3=int32#4,#56]
# asm 2: ldr >t2=r14,[<input_3=r3,#56]
ldr r14,[r3,#56]

# qhasm: t0 += w14
# asm 1: add >t0=int32#11,<t0=int32#12,<w14=int32#11
# asm 2: add >t0=r10,<t0=r11,<w14=r10
add r10,r11,r10

# qhasm: t0 += (t1 >>> 6)
# asm 1: add >t0=int32#11,<t0=int32#11,<t1=int32#13,ROR #6
# asm 2: add >t0=r10,<t0=r10,<t1=r12,ROR #6
add r10,r10,r12,ROR #6

# qhasm: t0 += t2
# asm 1: add >t0=int32#11,<t0=int32#11,<t2=int32#14
# asm 2: add >t0=r10,<t0=r10,<t2=r14
add r10,r10,r14

# qhasm: r1 += t0
# asm 1: add >r1=int32#6,<r1=int32#6,<t0=int32#11
# asm 2: add >r1=r5,<r1=r5,<t0=r10
add r5,r5,r10

# qhasm: r5 += r1
# asm 1: add >r5=int32#10,<r5=int32#10,<r1=int32#6
# asm 2: add >r5=r9,<r5=r9,<r1=r5
add r9,r9,r5

# qhasm: t0 = r2 & r3
# asm 1: and >t0=int32#11,<r2=int32#7,<r3=int32#3
# asm 2: and >t0=r10,<r2=r6,<r3=r2
and r10,r6,r2

# qhasm: t2 = r2 & r4
# asm 1: and >t2=int32#12,<r2=int32#7,<r4=int32#9
# asm 2: and >t2=r11,<r2=r6,<r4=r8
and r11,r6,r8

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t2 = r3 & r4
# asm 1: and >t2=int32#12,<r3=int32#3,<r4=int32#9
# asm 2: and >t2=r11,<r3=r2,<r4=r8
and r11,r2,r8

# qhasm: t3 = r2 ^ (r2 >>> 11)
# asm 1: eor >t3=int32#13,<r2=int32#7,<r2=int32#7,ROR #11
# asm 2: eor >t3=r12,<r2=r6,<r2=r6,ROR #11
eor r12,r6,r6,ROR #11

# qhasm: t3 ^= (r2 >>> 20)
# asm 1: eor >t3=int32#13,<t3=int32#13,<r2=int32#7,ROR #20
# asm 2: eor >t3=r12,<t3=r12,<r2=r6,ROR #20
eor r12,r12,r6,ROR #20

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t0 += (t3 >>> 2)
# asm 1: add >t0=int32#11,<t0=int32#11,<t3=int32#13,ROR #2
# asm 2: add >t0=r10,<t0=r10,<t3=r12,ROR #2
add r10,r10,r12,ROR #2

# qhasm: r1 += t0
# asm 1: add >r1=int32#6,<r1=int32#6,<t0=int32#11
# asm 2: add >r1=r5,<r1=r5,<t0=r10
add r5,r5,r10

# qhasm: w15 = w15_stack
# asm 1: ldr >w15=int32#11,<w15_stack=stack32#35
# asm 2: ldr >w15=r10,<w15_stack=[sp,#136]
ldr r10,[sp,#136]

# qhasm: t0 = r5 & r6
# asm 1: and >t0=int32#12,<r5=int32#10,<r6=int32#8
# asm 2: and >t0=r11,<r5=r9,<r6=r7
and r11,r9,r7

# qhasm: t1 = r7 & ~r5
# asm 1: bic >t1=int32#13,<r7=int32#1,<r5=int32#10
# asm 2: bic >t1=r12,<r7=r0,<r5=r9
bic r12,r0,r9

# qhasm: t0 ^= t1
# asm 1: eor >t0=int32#12,<t0=int32#12,<t1=int32#13
# asm 2: eor >t0=r11,<t0=r11,<t1=r12
eor r11,r11,r12

# qhasm: t1 = r5 ^ (r5 >>> 5)
# asm 1: eor >t1=int32#13,<r5=int32#10,<r5=int32#10,ROR #5
# asm 2: eor >t1=r12,<r5=r9,<r5=r9,ROR #5
eor r12,r9,r9,ROR #5

# qhasm: t1 ^= (r5 >>> 19)
# asm 1: eor >t1=int32#13,<t1=int32#13,<r5=int32#10,ROR #19
# asm 2: eor >t1=r12,<t1=r12,<r5=r9,ROR #19
eor r12,r12,r9,ROR #19

# qhasm: t2 = mem32[input_3 + 60]
# asm 1: ldr >t2=int32#14,[<input_3=int32#4,#60]
# asm 2: ldr >t2=r14,[<input_3=r3,#60]
ldr r14,[r3,#60]

# qhasm: t0 += w15
# asm 1: add >t0=int32#11,<t0=int32#12,<w15=int32#11
# asm 2: add >t0=r10,<t0=r11,<w15=r10
add r10,r11,r10

# qhasm: t0 += (t1 >>> 6)
# asm 1: add >t0=int32#11,<t0=int32#11,<t1=int32#13,ROR #6
# asm 2: add >t0=r10,<t0=r10,<t1=r12,ROR #6
add r10,r10,r12,ROR #6

# qhasm: t0 += t2
# asm 1: add >t0=int32#11,<t0=int32#11,<t2=int32#14
# asm 2: add >t0=r10,<t0=r10,<t2=r14
add r10,r10,r14

# qhasm: r0 += t0
# asm 1: add >r0=int32#5,<r0=int32#5,<t0=int32#11
# asm 2: add >r0=r4,<r0=r4,<t0=r10
add r4,r4,r10

# qhasm: r4 += r0
# asm 1: add >r4=int32#9,<r4=int32#9,<r0=int32#5
# asm 2: add >r4=r8,<r4=r8,<r0=r4
add r8,r8,r4

# qhasm: t0 = r1 & r2
# asm 1: and >t0=int32#11,<r1=int32#6,<r2=int32#7
# asm 2: and >t0=r10,<r1=r5,<r2=r6
and r10,r5,r6

# qhasm: t2 = r1 & r3
# asm 1: and >t2=int32#12,<r1=int32#6,<r3=int32#3
# asm 2: and >t2=r11,<r1=r5,<r3=r2
and r11,r5,r2

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t2 = r2 & r3
# asm 1: and >t2=int32#12,<r2=int32#7,<r3=int32#3
# asm 2: and >t2=r11,<r2=r6,<r3=r2
and r11,r6,r2

# qhasm: t3 = r1 ^ (r1 >>> 11)
# asm 1: eor >t3=int32#13,<r1=int32#6,<r1=int32#6,ROR #11
# asm 2: eor >t3=r12,<r1=r5,<r1=r5,ROR #11
eor r12,r5,r5,ROR #11

# qhasm: t3 ^= (r1 >>> 20)
# asm 1: eor >t3=int32#13,<t3=int32#13,<r1=int32#6,ROR #20
# asm 2: eor >t3=r12,<t3=r12,<r1=r5,ROR #20
eor r12,r12,r5,ROR #20

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t0 += (t3 >>> 2)
# asm 1: add >t0=int32#11,<t0=int32#11,<t3=int32#13,ROR #2
# asm 2: add >t0=r10,<t0=r10,<t3=r12,ROR #2
add r10,r10,r12,ROR #2

# qhasm: r0 += t0
# asm 1: add >r0=int32#5,<r0=int32#5,<t0=int32#11
# asm 2: add >r0=r4,<r0=r4,<t0=r10
add r4,r4,r10

# qhasm: r0_stack = r0
# asm 1: str <r0=int32#5,>r0_stack=stack32#36
# asm 2: str <r0=r4,>r0_stack=[sp,#140]
str r4,[sp,#140]

# qhasm: r1_stack = r1
# asm 1: str <r1=int32#6,>r1_stack=stack32#37
# asm 2: str <r1=r5,>r1_stack=[sp,#144]
str r5,[sp,#144]

# qhasm: r2_stack = r2
# asm 1: str <r2=int32#7,>r2_stack=stack32#38
# asm 2: str <r2=r6,>r2_stack=[sp,#148]
str r6,[sp,#148]

# qhasm: r3_stack = r3
# asm 1: str <r3=int32#3,>r3_stack=stack32#39
# asm 2: str <r3=r2,>r3_stack=[sp,#152]
str r2,[sp,#152]

# qhasm: r4_stack = r4
# asm 1: str <r4=int32#9,>r4_stack=stack32#40
# asm 2: str <r4=r8,>r4_stack=[sp,#156]
str r8,[sp,#156]

# qhasm: r5_stack = r5
# asm 1: str <r5=int32#10,>r5_stack=stack32#41
# asm 2: str <r5=r9,>r5_stack=[sp,#160]
str r9,[sp,#160]

# qhasm: r6_stack = r6
# asm 1: str <r6=int32#8,>r6_stack=stack32#42
# asm 2: str <r6=r7,>r6_stack=[sp,#164]
str r7,[sp,#164]

# qhasm: r7_stack = r7
# asm 1: str <r7=int32#1,>r7_stack=stack32#43
# asm 2: str <r7=r0,>r7_stack=[sp,#168]
str r0,[sp,#168]

# qhasm: w0 = w0_stack
# asm 1: ldr >w0=int32#1,<w0_stack=stack32#20
# asm 2: ldr >w0=r0,<w0_stack=[sp,#76]
ldr r0,[sp,#76]

# qhasm: w1 = w1_stack
# asm 1: ldr >w1=int32#3,<w1_stack=stack32#21
# asm 2: ldr >w1=r2,<w1_stack=[sp,#80]
ldr r2,[sp,#80]

# qhasm: w2 = w2_stack
# asm 1: ldr >w2=int32#5,<w2_stack=stack32#22
# asm 2: ldr >w2=r4,<w2_stack=[sp,#84]
ldr r4,[sp,#84]

# qhasm: w3 = w3_stack
# asm 1: ldr >w3=int32#6,<w3_stack=stack32#23
# asm 2: ldr >w3=r5,<w3_stack=[sp,#88]
ldr r5,[sp,#88]

# qhasm: w4 = w4_stack
# asm 1: ldr >w4=int32#7,<w4_stack=stack32#24
# asm 2: ldr >w4=r6,<w4_stack=[sp,#92]
ldr r6,[sp,#92]

# qhasm: w9 = w9_stack
# asm 1: ldr >w9=int32#8,<w9_stack=stack32#29
# asm 2: ldr >w9=r7,<w9_stack=[sp,#112]
ldr r7,[sp,#112]

# qhasm: w10 = w10_stack
# asm 1: ldr >w10=int32#9,<w10_stack=stack32#30
# asm 2: ldr >w10=r8,<w10_stack=[sp,#116]
ldr r8,[sp,#116]

# qhasm: w11 = w11_stack
# asm 1: ldr >w11=int32#10,<w11_stack=stack32#31
# asm 2: ldr >w11=r9,<w11_stack=[sp,#120]
ldr r9,[sp,#120]

# qhasm: w12 = w12_stack
# asm 1: ldr >w12=int32#11,<w12_stack=stack32#32
# asm 2: ldr >w12=r10,<w12_stack=[sp,#124]
ldr r10,[sp,#124]

# qhasm: w14 = w14_stack
# asm 1: ldr >w14=int32#12,<w14_stack=stack32#34
# asm 2: ldr >w14=r11,<w14_stack=[sp,#132]
ldr r11,[sp,#132]

# qhasm: w15 = w15_stack
# asm 1: ldr >w15=int32#13,<w15_stack=stack32#35
# asm 2: ldr >w15=r12,<w15_stack=[sp,#136]
ldr r12,[sp,#136]

# qhasm: w0 += w9
# asm 1: add >w0=int32#1,<w0=int32#1,<w9=int32#8
# asm 2: add >w0=r0,<w0=r0,<w9=r7
add r0,r0,r7

# qhasm: t0 = (w14 >>> 17)
# asm 1: mov >t0=int32#14,<w14=int32#12,ROR #17
# asm 2: mov >t0=r14,<w14=r11,ROR #17
mov r14,r11,ROR #17

# qhasm: t0 ^= (w14 >>> 19)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w14=int32#12,ROR #19
# asm 2: eor >t0=r14,<t0=r14,<w14=r11,ROR #19
eor r14,r14,r11,ROR #19

# qhasm: t0 ^= (w14 unsigned>> 10)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w14=int32#12,LSR #10
# asm 2: eor >t0=r14,<t0=r14,<w14=r11,LSR #10
eor r14,r14,r11,LSR #10

# qhasm: w0 += t0
# asm 1: add >w0=int32#1,<w0=int32#1,<t0=int32#14
# asm 2: add >w0=r0,<w0=r0,<t0=r14
add r0,r0,r14

# qhasm: t0 = (w1 >>> 7)
# asm 1: mov >t0=int32#14,<w1=int32#3,ROR #7
# asm 2: mov >t0=r14,<w1=r2,ROR #7
mov r14,r2,ROR #7

# qhasm: t0 ^= (w1 >>> 18)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w1=int32#3,ROR #18
# asm 2: eor >t0=r14,<t0=r14,<w1=r2,ROR #18
eor r14,r14,r2,ROR #18

# qhasm: t0 ^= (w1 unsigned>> 3)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w1=int32#3,LSR #3
# asm 2: eor >t0=r14,<t0=r14,<w1=r2,LSR #3
eor r14,r14,r2,LSR #3

# qhasm: w0 += t0
# asm 1: add >w0=int32#1,<w0=int32#1,<t0=int32#14
# asm 2: add >w0=r0,<w0=r0,<t0=r14
add r0,r0,r14

# qhasm: w1 += w10
# asm 1: add >w1=int32#3,<w1=int32#3,<w10=int32#9
# asm 2: add >w1=r2,<w1=r2,<w10=r8
add r2,r2,r8

# qhasm: t0 = (w15 >>> 17)
# asm 1: mov >t0=int32#14,<w15=int32#13,ROR #17
# asm 2: mov >t0=r14,<w15=r12,ROR #17
mov r14,r12,ROR #17

# qhasm: t0 ^= (w15 >>> 19)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w15=int32#13,ROR #19
# asm 2: eor >t0=r14,<t0=r14,<w15=r12,ROR #19
eor r14,r14,r12,ROR #19

# qhasm: t0 ^= (w15 unsigned>> 10)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w15=int32#13,LSR #10
# asm 2: eor >t0=r14,<t0=r14,<w15=r12,LSR #10
eor r14,r14,r12,LSR #10

# qhasm: w1 += t0
# asm 1: add >w1=int32#3,<w1=int32#3,<t0=int32#14
# asm 2: add >w1=r2,<w1=r2,<t0=r14
add r2,r2,r14

# qhasm: t0 = (w2 >>> 7)
# asm 1: mov >t0=int32#14,<w2=int32#5,ROR #7
# asm 2: mov >t0=r14,<w2=r4,ROR #7
mov r14,r4,ROR #7

# qhasm: t0 ^= (w2 >>> 18)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w2=int32#5,ROR #18
# asm 2: eor >t0=r14,<t0=r14,<w2=r4,ROR #18
eor r14,r14,r4,ROR #18

# qhasm: t0 ^= (w2 unsigned>> 3)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w2=int32#5,LSR #3
# asm 2: eor >t0=r14,<t0=r14,<w2=r4,LSR #3
eor r14,r14,r4,LSR #3

# qhasm: w1 += t0
# asm 1: add >w1=int32#3,<w1=int32#3,<t0=int32#14
# asm 2: add >w1=r2,<w1=r2,<t0=r14
add r2,r2,r14

# qhasm: w2 += w11
# asm 1: add >w2=int32#5,<w2=int32#5,<w11=int32#10
# asm 2: add >w2=r4,<w2=r4,<w11=r9
add r4,r4,r9

# qhasm: t0 = (w0 >>> 17)
# asm 1: mov >t0=int32#14,<w0=int32#1,ROR #17
# asm 2: mov >t0=r14,<w0=r0,ROR #17
mov r14,r0,ROR #17

# qhasm: t0 ^= (w0 >>> 19)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w0=int32#1,ROR #19
# asm 2: eor >t0=r14,<t0=r14,<w0=r0,ROR #19
eor r14,r14,r0,ROR #19

# qhasm: t0 ^= (w0 unsigned>> 10)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w0=int32#1,LSR #10
# asm 2: eor >t0=r14,<t0=r14,<w0=r0,LSR #10
eor r14,r14,r0,LSR #10

# qhasm: w2 += t0
# asm 1: add >w2=int32#5,<w2=int32#5,<t0=int32#14
# asm 2: add >w2=r4,<w2=r4,<t0=r14
add r4,r4,r14

# qhasm: t0 = (w3 >>> 7)
# asm 1: mov >t0=int32#14,<w3=int32#6,ROR #7
# asm 2: mov >t0=r14,<w3=r5,ROR #7
mov r14,r5,ROR #7

# qhasm: t0 ^= (w3 >>> 18)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w3=int32#6,ROR #18
# asm 2: eor >t0=r14,<t0=r14,<w3=r5,ROR #18
eor r14,r14,r5,ROR #18

# qhasm: t0 ^= (w3 unsigned>> 3)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w3=int32#6,LSR #3
# asm 2: eor >t0=r14,<t0=r14,<w3=r5,LSR #3
eor r14,r14,r5,LSR #3

# qhasm: w2 += t0
# asm 1: add >w2=int32#5,<w2=int32#5,<t0=int32#14
# asm 2: add >w2=r4,<w2=r4,<t0=r14
add r4,r4,r14

# qhasm: w3 += w12
# asm 1: add >w3=int32#6,<w3=int32#6,<w12=int32#11
# asm 2: add >w3=r5,<w3=r5,<w12=r10
add r5,r5,r10

# qhasm: t0 = (w1 >>> 17)
# asm 1: mov >t0=int32#14,<w1=int32#3,ROR #17
# asm 2: mov >t0=r14,<w1=r2,ROR #17
mov r14,r2,ROR #17

# qhasm: t0 ^= (w1 >>> 19)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w1=int32#3,ROR #19
# asm 2: eor >t0=r14,<t0=r14,<w1=r2,ROR #19
eor r14,r14,r2,ROR #19

# qhasm: t0 ^= (w1 unsigned>> 10)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w1=int32#3,LSR #10
# asm 2: eor >t0=r14,<t0=r14,<w1=r2,LSR #10
eor r14,r14,r2,LSR #10

# qhasm: w3 += t0
# asm 1: add >w3=int32#6,<w3=int32#6,<t0=int32#14
# asm 2: add >w3=r5,<w3=r5,<t0=r14
add r5,r5,r14

# qhasm: t0 = (w4 >>> 7)
# asm 1: mov >t0=int32#14,<w4=int32#7,ROR #7
# asm 2: mov >t0=r14,<w4=r6,ROR #7
mov r14,r6,ROR #7

# qhasm: t0 ^= (w4 >>> 18)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w4=int32#7,ROR #18
# asm 2: eor >t0=r14,<t0=r14,<w4=r6,ROR #18
eor r14,r14,r6,ROR #18

# qhasm: t0 ^= (w4 unsigned>> 3)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w4=int32#7,LSR #3
# asm 2: eor >t0=r14,<t0=r14,<w4=r6,LSR #3
eor r14,r14,r6,LSR #3

# qhasm: w3 += t0
# asm 1: add >w3=int32#6,<w3=int32#6,<t0=int32#14
# asm 2: add >w3=r5,<w3=r5,<t0=r14
add r5,r5,r14

# qhasm: w1_stack = w1
# asm 1: str <w1=int32#3,>w1_stack=stack32#20
# asm 2: str <w1=r2,>w1_stack=[sp,#76]
str r2,[sp,#76]

# qhasm: w9_stack = w9
# asm 1: str <w9=int32#8,>w9_stack=stack32#21
# asm 2: str <w9=r7,>w9_stack=[sp,#80]
str r7,[sp,#80]

# qhasm: w10_stack = w10
# asm 1: str <w10=int32#9,>w10_stack=stack32#22
# asm 2: str <w10=r8,>w10_stack=[sp,#84]
str r8,[sp,#84]

# qhasm: w11_stack = w11
# asm 1: str <w11=int32#10,>w11_stack=stack32#23
# asm 2: str <w11=r9,>w11_stack=[sp,#88]
str r9,[sp,#88]

# qhasm: w12_stack = w12
# asm 1: str <w12=int32#11,>w12_stack=stack32#24
# asm 2: str <w12=r10,>w12_stack=[sp,#92]
str r10,[sp,#92]

# qhasm: w13 = w13_stack
# asm 1: ldr >w13=int32#3,<w13_stack=stack32#33
# asm 2: ldr >w13=r2,<w13_stack=[sp,#128]
ldr r2,[sp,#128]

# qhasm: w5 = w5_stack
# asm 1: ldr >w5=int32#8,<w5_stack=stack32#25
# asm 2: ldr >w5=r7,<w5_stack=[sp,#96]
ldr r7,[sp,#96]

# qhasm: w6 = w6_stack
# asm 1: ldr >w6=int32#9,<w6_stack=stack32#26
# asm 2: ldr >w6=r8,<w6_stack=[sp,#100]
ldr r8,[sp,#100]

# qhasm: w7 = w7_stack
# asm 1: ldr >w7=int32#10,<w7_stack=stack32#27
# asm 2: ldr >w7=r9,<w7_stack=[sp,#104]
ldr r9,[sp,#104]

# qhasm: w8 = w8_stack
# asm 1: ldr >w8=int32#11,<w8_stack=stack32#28
# asm 2: ldr >w8=r10,<w8_stack=[sp,#108]
ldr r10,[sp,#108]

# qhasm: w4 += w13
# asm 1: add >w4=int32#7,<w4=int32#7,<w13=int32#3
# asm 2: add >w4=r6,<w4=r6,<w13=r2
add r6,r6,r2

# qhasm: t0 = (w2 >>> 17)
# asm 1: mov >t0=int32#14,<w2=int32#5,ROR #17
# asm 2: mov >t0=r14,<w2=r4,ROR #17
mov r14,r4,ROR #17

# qhasm: t0 ^= (w2 >>> 19)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w2=int32#5,ROR #19
# asm 2: eor >t0=r14,<t0=r14,<w2=r4,ROR #19
eor r14,r14,r4,ROR #19

# qhasm: t0 ^= (w2 unsigned>> 10)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w2=int32#5,LSR #10
# asm 2: eor >t0=r14,<t0=r14,<w2=r4,LSR #10
eor r14,r14,r4,LSR #10

# qhasm: w4 += t0
# asm 1: add >w4=int32#7,<w4=int32#7,<t0=int32#14
# asm 2: add >w4=r6,<w4=r6,<t0=r14
add r6,r6,r14

# qhasm: t0 = (w5 >>> 7)
# asm 1: mov >t0=int32#14,<w5=int32#8,ROR #7
# asm 2: mov >t0=r14,<w5=r7,ROR #7
mov r14,r7,ROR #7

# qhasm: t0 ^= (w5 >>> 18)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w5=int32#8,ROR #18
# asm 2: eor >t0=r14,<t0=r14,<w5=r7,ROR #18
eor r14,r14,r7,ROR #18

# qhasm: t0 ^= (w5 unsigned>> 3)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w5=int32#8,LSR #3
# asm 2: eor >t0=r14,<t0=r14,<w5=r7,LSR #3
eor r14,r14,r7,LSR #3

# qhasm: w4 += t0
# asm 1: add >w4=int32#7,<w4=int32#7,<t0=int32#14
# asm 2: add >w4=r6,<w4=r6,<t0=r14
add r6,r6,r14

# qhasm: w5 += w14
# asm 1: add >w5=int32#8,<w5=int32#8,<w14=int32#12
# asm 2: add >w5=r7,<w5=r7,<w14=r11
add r7,r7,r11

# qhasm: t0 = (w3 >>> 17)
# asm 1: mov >t0=int32#14,<w3=int32#6,ROR #17
# asm 2: mov >t0=r14,<w3=r5,ROR #17
mov r14,r5,ROR #17

# qhasm: t0 ^= (w3 >>> 19)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w3=int32#6,ROR #19
# asm 2: eor >t0=r14,<t0=r14,<w3=r5,ROR #19
eor r14,r14,r5,ROR #19

# qhasm: t0 ^= (w3 unsigned>> 10)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w3=int32#6,LSR #10
# asm 2: eor >t0=r14,<t0=r14,<w3=r5,LSR #10
eor r14,r14,r5,LSR #10

# qhasm: w5 += t0
# asm 1: add >w5=int32#8,<w5=int32#8,<t0=int32#14
# asm 2: add >w5=r7,<w5=r7,<t0=r14
add r7,r7,r14

# qhasm: t0 = (w6 >>> 7)
# asm 1: mov >t0=int32#14,<w6=int32#9,ROR #7
# asm 2: mov >t0=r14,<w6=r8,ROR #7
mov r14,r8,ROR #7

# qhasm: t0 ^= (w6 >>> 18)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w6=int32#9,ROR #18
# asm 2: eor >t0=r14,<t0=r14,<w6=r8,ROR #18
eor r14,r14,r8,ROR #18

# qhasm: t0 ^= (w6 unsigned>> 3)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w6=int32#9,LSR #3
# asm 2: eor >t0=r14,<t0=r14,<w6=r8,LSR #3
eor r14,r14,r8,LSR #3

# qhasm: w5 += t0
# asm 1: add >w5=int32#8,<w5=int32#8,<t0=int32#14
# asm 2: add >w5=r7,<w5=r7,<t0=r14
add r7,r7,r14

# qhasm: w6 += w15
# asm 1: add >w6=int32#9,<w6=int32#9,<w15=int32#13
# asm 2: add >w6=r8,<w6=r8,<w15=r12
add r8,r8,r12

# qhasm: t0 = (w4 >>> 17)
# asm 1: mov >t0=int32#14,<w4=int32#7,ROR #17
# asm 2: mov >t0=r14,<w4=r6,ROR #17
mov r14,r6,ROR #17

# qhasm: t0 ^= (w4 >>> 19)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w4=int32#7,ROR #19
# asm 2: eor >t0=r14,<t0=r14,<w4=r6,ROR #19
eor r14,r14,r6,ROR #19

# qhasm: t0 ^= (w4 unsigned>> 10)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w4=int32#7,LSR #10
# asm 2: eor >t0=r14,<t0=r14,<w4=r6,LSR #10
eor r14,r14,r6,LSR #10

# qhasm: w6 += t0
# asm 1: add >w6=int32#9,<w6=int32#9,<t0=int32#14
# asm 2: add >w6=r8,<w6=r8,<t0=r14
add r8,r8,r14

# qhasm: t0 = (w7 >>> 7)
# asm 1: mov >t0=int32#14,<w7=int32#10,ROR #7
# asm 2: mov >t0=r14,<w7=r9,ROR #7
mov r14,r9,ROR #7

# qhasm: t0 ^= (w7 >>> 18)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w7=int32#10,ROR #18
# asm 2: eor >t0=r14,<t0=r14,<w7=r9,ROR #18
eor r14,r14,r9,ROR #18

# qhasm: t0 ^= (w7 unsigned>> 3)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w7=int32#10,LSR #3
# asm 2: eor >t0=r14,<t0=r14,<w7=r9,LSR #3
eor r14,r14,r9,LSR #3

# qhasm: w6 += t0
# asm 1: add >w6=int32#9,<w6=int32#9,<t0=int32#14
# asm 2: add >w6=r8,<w6=r8,<t0=r14
add r8,r8,r14

# qhasm: w7 += w0
# asm 1: add >w7=int32#10,<w7=int32#10,<w0=int32#1
# asm 2: add >w7=r9,<w7=r9,<w0=r0
add r9,r9,r0

# qhasm: t0 = (w5 >>> 17)
# asm 1: mov >t0=int32#14,<w5=int32#8,ROR #17
# asm 2: mov >t0=r14,<w5=r7,ROR #17
mov r14,r7,ROR #17

# qhasm: t0 ^= (w5 >>> 19)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w5=int32#8,ROR #19
# asm 2: eor >t0=r14,<t0=r14,<w5=r7,ROR #19
eor r14,r14,r7,ROR #19

# qhasm: t0 ^= (w5 unsigned>> 10)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w5=int32#8,LSR #10
# asm 2: eor >t0=r14,<t0=r14,<w5=r7,LSR #10
eor r14,r14,r7,LSR #10

# qhasm: w7 += t0
# asm 1: add >w7=int32#10,<w7=int32#10,<t0=int32#14
# asm 2: add >w7=r9,<w7=r9,<t0=r14
add r9,r9,r14

# qhasm: t0 = (w8 >>> 7)
# asm 1: mov >t0=int32#14,<w8=int32#11,ROR #7
# asm 2: mov >t0=r14,<w8=r10,ROR #7
mov r14,r10,ROR #7

# qhasm: t0 ^= (w8 >>> 18)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w8=int32#11,ROR #18
# asm 2: eor >t0=r14,<t0=r14,<w8=r10,ROR #18
eor r14,r14,r10,ROR #18

# qhasm: t0 ^= (w8 unsigned>> 3)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w8=int32#11,LSR #3
# asm 2: eor >t0=r14,<t0=r14,<w8=r10,LSR #3
eor r14,r14,r10,LSR #3

# qhasm: w7 += t0
# asm 1: add >w7=int32#10,<w7=int32#10,<t0=int32#14
# asm 2: add >w7=r9,<w7=r9,<t0=r14
add r9,r9,r14

# qhasm: w5_stack = w5
# asm 1: str <w5=int32#8,>w5_stack=stack32#25
# asm 2: str <w5=r7,>w5_stack=[sp,#96]
str r7,[sp,#96]

# qhasm: w13_stack = w13
# asm 1: str <w13=int32#3,>w13_stack=stack32#26
# asm 2: str <w13=r2,>w13_stack=[sp,#100]
str r2,[sp,#100]

# qhasm: w14_stack = w14
# asm 1: str <w14=int32#12,>w14_stack=stack32#27
# asm 2: str <w14=r11,>w14_stack=[sp,#104]
str r11,[sp,#104]

# qhasm: w15_stack = w15
# asm 1: str <w15=int32#13,>w15_stack=stack32#28
# asm 2: str <w15=r12,>w15_stack=[sp,#108]
str r12,[sp,#108]

# qhasm: w0_stack = w0
# asm 1: str <w0=int32#1,>w0_stack=stack32#29
# asm 2: str <w0=r0,>w0_stack=[sp,#112]
str r0,[sp,#112]

# qhasm: w1 = w1_stack
# asm 1: ldr >w1=int32#1,<w1_stack=stack32#20
# asm 2: ldr >w1=r0,<w1_stack=[sp,#76]
ldr r0,[sp,#76]

# qhasm: w9 = w9_stack
# asm 1: ldr >w9=int32#3,<w9_stack=stack32#21
# asm 2: ldr >w9=r2,<w9_stack=[sp,#80]
ldr r2,[sp,#80]

# qhasm: w10 = w10_stack
# asm 1: ldr >w10=int32#8,<w10_stack=stack32#22
# asm 2: ldr >w10=r7,<w10_stack=[sp,#84]
ldr r7,[sp,#84]

# qhasm: w11 = w11_stack
# asm 1: ldr >w11=int32#12,<w11_stack=stack32#23
# asm 2: ldr >w11=r11,<w11_stack=[sp,#88]
ldr r11,[sp,#88]

# qhasm: w12 = w12_stack
# asm 1: ldr >w12=int32#13,<w12_stack=stack32#24
# asm 2: ldr >w12=r12,<w12_stack=[sp,#92]
ldr r12,[sp,#92]

# qhasm: w8 += w1
# asm 1: add >w8=int32#11,<w8=int32#11,<w1=int32#1
# asm 2: add >w8=r10,<w8=r10,<w1=r0
add r10,r10,r0

# qhasm: t0 = (w6 >>> 17)
# asm 1: mov >t0=int32#14,<w6=int32#9,ROR #17
# asm 2: mov >t0=r14,<w6=r8,ROR #17
mov r14,r8,ROR #17

# qhasm: t0 ^= (w6 >>> 19)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w6=int32#9,ROR #19
# asm 2: eor >t0=r14,<t0=r14,<w6=r8,ROR #19
eor r14,r14,r8,ROR #19

# qhasm: t0 ^= (w6 unsigned>> 10)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w6=int32#9,LSR #10
# asm 2: eor >t0=r14,<t0=r14,<w6=r8,LSR #10
eor r14,r14,r8,LSR #10

# qhasm: w8 += t0
# asm 1: add >w8=int32#11,<w8=int32#11,<t0=int32#14
# asm 2: add >w8=r10,<w8=r10,<t0=r14
add r10,r10,r14

# qhasm: t0 = (w9 >>> 7)
# asm 1: mov >t0=int32#14,<w9=int32#3,ROR #7
# asm 2: mov >t0=r14,<w9=r2,ROR #7
mov r14,r2,ROR #7

# qhasm: t0 ^= (w9 >>> 18)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w9=int32#3,ROR #18
# asm 2: eor >t0=r14,<t0=r14,<w9=r2,ROR #18
eor r14,r14,r2,ROR #18

# qhasm: t0 ^= (w9 unsigned>> 3)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w9=int32#3,LSR #3
# asm 2: eor >t0=r14,<t0=r14,<w9=r2,LSR #3
eor r14,r14,r2,LSR #3

# qhasm: w8 += t0
# asm 1: add >w8=int32#11,<w8=int32#11,<t0=int32#14
# asm 2: add >w8=r10,<w8=r10,<t0=r14
add r10,r10,r14

# qhasm: w9 += w2
# asm 1: add >w9=int32#3,<w9=int32#3,<w2=int32#5
# asm 2: add >w9=r2,<w9=r2,<w2=r4
add r2,r2,r4

# qhasm: t0 = (w7 >>> 17)
# asm 1: mov >t0=int32#14,<w7=int32#10,ROR #17
# asm 2: mov >t0=r14,<w7=r9,ROR #17
mov r14,r9,ROR #17

# qhasm: t0 ^= (w7 >>> 19)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w7=int32#10,ROR #19
# asm 2: eor >t0=r14,<t0=r14,<w7=r9,ROR #19
eor r14,r14,r9,ROR #19

# qhasm: t0 ^= (w7 unsigned>> 10)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w7=int32#10,LSR #10
# asm 2: eor >t0=r14,<t0=r14,<w7=r9,LSR #10
eor r14,r14,r9,LSR #10

# qhasm: w9 += t0
# asm 1: add >w9=int32#3,<w9=int32#3,<t0=int32#14
# asm 2: add >w9=r2,<w9=r2,<t0=r14
add r2,r2,r14

# qhasm: t0 = (w10 >>> 7)
# asm 1: mov >t0=int32#14,<w10=int32#8,ROR #7
# asm 2: mov >t0=r14,<w10=r7,ROR #7
mov r14,r7,ROR #7

# qhasm: t0 ^= (w10 >>> 18)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w10=int32#8,ROR #18
# asm 2: eor >t0=r14,<t0=r14,<w10=r7,ROR #18
eor r14,r14,r7,ROR #18

# qhasm: t0 ^= (w10 unsigned>> 3)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w10=int32#8,LSR #3
# asm 2: eor >t0=r14,<t0=r14,<w10=r7,LSR #3
eor r14,r14,r7,LSR #3

# qhasm: w9 += t0
# asm 1: add >w9=int32#3,<w9=int32#3,<t0=int32#14
# asm 2: add >w9=r2,<w9=r2,<t0=r14
add r2,r2,r14

# qhasm: w10 += w3
# asm 1: add >w10=int32#8,<w10=int32#8,<w3=int32#6
# asm 2: add >w10=r7,<w10=r7,<w3=r5
add r7,r7,r5

# qhasm: t0 = (w8 >>> 17)
# asm 1: mov >t0=int32#14,<w8=int32#11,ROR #17
# asm 2: mov >t0=r14,<w8=r10,ROR #17
mov r14,r10,ROR #17

# qhasm: t0 ^= (w8 >>> 19)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w8=int32#11,ROR #19
# asm 2: eor >t0=r14,<t0=r14,<w8=r10,ROR #19
eor r14,r14,r10,ROR #19

# qhasm: t0 ^= (w8 unsigned>> 10)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w8=int32#11,LSR #10
# asm 2: eor >t0=r14,<t0=r14,<w8=r10,LSR #10
eor r14,r14,r10,LSR #10

# qhasm: w10 += t0
# asm 1: add >w10=int32#8,<w10=int32#8,<t0=int32#14
# asm 2: add >w10=r7,<w10=r7,<t0=r14
add r7,r7,r14

# qhasm: t0 = (w11 >>> 7)
# asm 1: mov >t0=int32#14,<w11=int32#12,ROR #7
# asm 2: mov >t0=r14,<w11=r11,ROR #7
mov r14,r11,ROR #7

# qhasm: t0 ^= (w11 >>> 18)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w11=int32#12,ROR #18
# asm 2: eor >t0=r14,<t0=r14,<w11=r11,ROR #18
eor r14,r14,r11,ROR #18

# qhasm: t0 ^= (w11 unsigned>> 3)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w11=int32#12,LSR #3
# asm 2: eor >t0=r14,<t0=r14,<w11=r11,LSR #3
eor r14,r14,r11,LSR #3

# qhasm: w10 += t0
# asm 1: add >w10=int32#8,<w10=int32#8,<t0=int32#14
# asm 2: add >w10=r7,<w10=r7,<t0=r14
add r7,r7,r14

# qhasm: w11 += w4
# asm 1: add >w11=int32#12,<w11=int32#12,<w4=int32#7
# asm 2: add >w11=r11,<w11=r11,<w4=r6
add r11,r11,r6

# qhasm: t0 = (w9 >>> 17)
# asm 1: mov >t0=int32#14,<w9=int32#3,ROR #17
# asm 2: mov >t0=r14,<w9=r2,ROR #17
mov r14,r2,ROR #17

# qhasm: t0 ^= (w9 >>> 19)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w9=int32#3,ROR #19
# asm 2: eor >t0=r14,<t0=r14,<w9=r2,ROR #19
eor r14,r14,r2,ROR #19

# qhasm: t0 ^= (w9 unsigned>> 10)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w9=int32#3,LSR #10
# asm 2: eor >t0=r14,<t0=r14,<w9=r2,LSR #10
eor r14,r14,r2,LSR #10

# qhasm: w11 += t0
# asm 1: add >w11=int32#12,<w11=int32#12,<t0=int32#14
# asm 2: add >w11=r11,<w11=r11,<t0=r14
add r11,r11,r14

# qhasm: t0 = (w12 >>> 7)
# asm 1: mov >t0=int32#14,<w12=int32#13,ROR #7
# asm 2: mov >t0=r14,<w12=r12,ROR #7
mov r14,r12,ROR #7

# qhasm: t0 ^= (w12 >>> 18)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w12=int32#13,ROR #18
# asm 2: eor >t0=r14,<t0=r14,<w12=r12,ROR #18
eor r14,r14,r12,ROR #18

# qhasm: t0 ^= (w12 unsigned>> 3)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w12=int32#13,LSR #3
# asm 2: eor >t0=r14,<t0=r14,<w12=r12,LSR #3
eor r14,r14,r12,LSR #3

# qhasm: w11 += t0
# asm 1: add >w11=int32#12,<w11=int32#12,<t0=int32#14
# asm 2: add >w11=r11,<w11=r11,<t0=r14
add r11,r11,r14

# qhasm: w9_stack = w9
# asm 1: str <w9=int32#3,>w9_stack=stack32#20
# asm 2: str <w9=r2,>w9_stack=[sp,#76]
str r2,[sp,#76]

# qhasm: w1_stack = w1
# asm 1: str <w1=int32#1,>w1_stack=stack32#21
# asm 2: str <w1=r0,>w1_stack=[sp,#80]
str r0,[sp,#80]

# qhasm: w2_stack = w2
# asm 1: str <w2=int32#5,>w2_stack=stack32#22
# asm 2: str <w2=r4,>w2_stack=[sp,#84]
str r4,[sp,#84]

# qhasm: w3_stack = w3
# asm 1: str <w3=int32#6,>w3_stack=stack32#23
# asm 2: str <w3=r5,>w3_stack=[sp,#88]
str r5,[sp,#88]

# qhasm: w4_stack = w4
# asm 1: str <w4=int32#7,>w4_stack=stack32#24
# asm 2: str <w4=r6,>w4_stack=[sp,#92]
str r6,[sp,#92]

# qhasm: w5 = w5_stack
# asm 1: ldr >w5=int32#1,<w5_stack=stack32#25
# asm 2: ldr >w5=r0,<w5_stack=[sp,#96]
ldr r0,[sp,#96]

# qhasm: w13 = w13_stack
# asm 1: ldr >w13=int32#3,<w13_stack=stack32#26
# asm 2: ldr >w13=r2,<w13_stack=[sp,#100]
ldr r2,[sp,#100]

# qhasm: w14 = w14_stack
# asm 1: ldr >w14=int32#5,<w14_stack=stack32#27
# asm 2: ldr >w14=r4,<w14_stack=[sp,#104]
ldr r4,[sp,#104]

# qhasm: w15 = w15_stack
# asm 1: ldr >w15=int32#6,<w15_stack=stack32#28
# asm 2: ldr >w15=r5,<w15_stack=[sp,#108]
ldr r5,[sp,#108]

# qhasm: w0 = w0_stack
# asm 1: ldr >w0=int32#7,<w0_stack=stack32#29
# asm 2: ldr >w0=r6,<w0_stack=[sp,#112]
ldr r6,[sp,#112]

# qhasm: w12 += w5
# asm 1: add >w12=int32#13,<w12=int32#13,<w5=int32#1
# asm 2: add >w12=r12,<w12=r12,<w5=r0
add r12,r12,r0

# qhasm: t0 = (w10 >>> 17)
# asm 1: mov >t0=int32#14,<w10=int32#8,ROR #17
# asm 2: mov >t0=r14,<w10=r7,ROR #17
mov r14,r7,ROR #17

# qhasm: t0 ^= (w10 >>> 19)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w10=int32#8,ROR #19
# asm 2: eor >t0=r14,<t0=r14,<w10=r7,ROR #19
eor r14,r14,r7,ROR #19

# qhasm: t0 ^= (w10 unsigned>> 10)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w10=int32#8,LSR #10
# asm 2: eor >t0=r14,<t0=r14,<w10=r7,LSR #10
eor r14,r14,r7,LSR #10

# qhasm: w12 += t0
# asm 1: add >w12=int32#13,<w12=int32#13,<t0=int32#14
# asm 2: add >w12=r12,<w12=r12,<t0=r14
add r12,r12,r14

# qhasm: t0 = (w13 >>> 7)
# asm 1: mov >t0=int32#14,<w13=int32#3,ROR #7
# asm 2: mov >t0=r14,<w13=r2,ROR #7
mov r14,r2,ROR #7

# qhasm: t0 ^= (w13 >>> 18)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w13=int32#3,ROR #18
# asm 2: eor >t0=r14,<t0=r14,<w13=r2,ROR #18
eor r14,r14,r2,ROR #18

# qhasm: t0 ^= (w13 unsigned>> 3)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w13=int32#3,LSR #3
# asm 2: eor >t0=r14,<t0=r14,<w13=r2,LSR #3
eor r14,r14,r2,LSR #3

# qhasm: w12 += t0
# asm 1: add >w12=int32#13,<w12=int32#13,<t0=int32#14
# asm 2: add >w12=r12,<w12=r12,<t0=r14
add r12,r12,r14

# qhasm: w13 += w6
# asm 1: add >w13=int32#3,<w13=int32#3,<w6=int32#9
# asm 2: add >w13=r2,<w13=r2,<w6=r8
add r2,r2,r8

# qhasm: t0 = (w11 >>> 17)
# asm 1: mov >t0=int32#14,<w11=int32#12,ROR #17
# asm 2: mov >t0=r14,<w11=r11,ROR #17
mov r14,r11,ROR #17

# qhasm: t0 ^= (w11 >>> 19)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w11=int32#12,ROR #19
# asm 2: eor >t0=r14,<t0=r14,<w11=r11,ROR #19
eor r14,r14,r11,ROR #19

# qhasm: t0 ^= (w11 unsigned>> 10)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w11=int32#12,LSR #10
# asm 2: eor >t0=r14,<t0=r14,<w11=r11,LSR #10
eor r14,r14,r11,LSR #10

# qhasm: w13 += t0
# asm 1: add >w13=int32#3,<w13=int32#3,<t0=int32#14
# asm 2: add >w13=r2,<w13=r2,<t0=r14
add r2,r2,r14

# qhasm: t0 = (w14 >>> 7)
# asm 1: mov >t0=int32#14,<w14=int32#5,ROR #7
# asm 2: mov >t0=r14,<w14=r4,ROR #7
mov r14,r4,ROR #7

# qhasm: t0 ^= (w14 >>> 18)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w14=int32#5,ROR #18
# asm 2: eor >t0=r14,<t0=r14,<w14=r4,ROR #18
eor r14,r14,r4,ROR #18

# qhasm: t0 ^= (w14 unsigned>> 3)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w14=int32#5,LSR #3
# asm 2: eor >t0=r14,<t0=r14,<w14=r4,LSR #3
eor r14,r14,r4,LSR #3

# qhasm: w13 += t0
# asm 1: add >w13=int32#3,<w13=int32#3,<t0=int32#14
# asm 2: add >w13=r2,<w13=r2,<t0=r14
add r2,r2,r14

# qhasm: w14 += w7
# asm 1: add >w14=int32#5,<w14=int32#5,<w7=int32#10
# asm 2: add >w14=r4,<w14=r4,<w7=r9
add r4,r4,r9

# qhasm: t0 = (w12 >>> 17)
# asm 1: mov >t0=int32#14,<w12=int32#13,ROR #17
# asm 2: mov >t0=r14,<w12=r12,ROR #17
mov r14,r12,ROR #17

# qhasm: t0 ^= (w12 >>> 19)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w12=int32#13,ROR #19
# asm 2: eor >t0=r14,<t0=r14,<w12=r12,ROR #19
eor r14,r14,r12,ROR #19

# qhasm: t0 ^= (w12 unsigned>> 10)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w12=int32#13,LSR #10
# asm 2: eor >t0=r14,<t0=r14,<w12=r12,LSR #10
eor r14,r14,r12,LSR #10

# qhasm: w14 += t0
# asm 1: add >w14=int32#5,<w14=int32#5,<t0=int32#14
# asm 2: add >w14=r4,<w14=r4,<t0=r14
add r4,r4,r14

# qhasm: t0 = (w15 >>> 7)
# asm 1: mov >t0=int32#14,<w15=int32#6,ROR #7
# asm 2: mov >t0=r14,<w15=r5,ROR #7
mov r14,r5,ROR #7

# qhasm: t0 ^= (w15 >>> 18)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w15=int32#6,ROR #18
# asm 2: eor >t0=r14,<t0=r14,<w15=r5,ROR #18
eor r14,r14,r5,ROR #18

# qhasm: t0 ^= (w15 unsigned>> 3)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w15=int32#6,LSR #3
# asm 2: eor >t0=r14,<t0=r14,<w15=r5,LSR #3
eor r14,r14,r5,LSR #3

# qhasm: w14 += t0
# asm 1: add >w14=int32#5,<w14=int32#5,<t0=int32#14
# asm 2: add >w14=r4,<w14=r4,<t0=r14
add r4,r4,r14

# qhasm: w15 += w8
# asm 1: add >w15=int32#6,<w15=int32#6,<w8=int32#11
# asm 2: add >w15=r5,<w15=r5,<w8=r10
add r5,r5,r10

# qhasm: t0 = (w13 >>> 17)
# asm 1: mov >t0=int32#14,<w13=int32#3,ROR #17
# asm 2: mov >t0=r14,<w13=r2,ROR #17
mov r14,r2,ROR #17

# qhasm: t0 ^= (w13 >>> 19)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w13=int32#3,ROR #19
# asm 2: eor >t0=r14,<t0=r14,<w13=r2,ROR #19
eor r14,r14,r2,ROR #19

# qhasm: t0 ^= (w13 unsigned>> 10)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w13=int32#3,LSR #10
# asm 2: eor >t0=r14,<t0=r14,<w13=r2,LSR #10
eor r14,r14,r2,LSR #10

# qhasm: w15 += t0
# asm 1: add >w15=int32#6,<w15=int32#6,<t0=int32#14
# asm 2: add >w15=r5,<w15=r5,<t0=r14
add r5,r5,r14

# qhasm: t0 = (w0 >>> 7)
# asm 1: mov >t0=int32#14,<w0=int32#7,ROR #7
# asm 2: mov >t0=r14,<w0=r6,ROR #7
mov r14,r6,ROR #7

# qhasm: t0 ^= (w0 >>> 18)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w0=int32#7,ROR #18
# asm 2: eor >t0=r14,<t0=r14,<w0=r6,ROR #18
eor r14,r14,r6,ROR #18

# qhasm: t0 ^= (w0 unsigned>> 3)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w0=int32#7,LSR #3
# asm 2: eor >t0=r14,<t0=r14,<w0=r6,LSR #3
eor r14,r14,r6,LSR #3

# qhasm: w15 += t0
# asm 1: add >w15=int32#6,<w15=int32#6,<t0=int32#14
# asm 2: add >w15=r5,<w15=r5,<t0=r14
add r5,r5,r14

# qhasm: w0_stack = w0
# asm 1: str <w0=int32#7,>w0_stack=stack32#25
# asm 2: str <w0=r6,>w0_stack=[sp,#96]
str r6,[sp,#96]

# qhasm: w5_stack = w5
# asm 1: str <w5=int32#1,>w5_stack=stack32#26
# asm 2: str <w5=r0,>w5_stack=[sp,#100]
str r0,[sp,#100]

# qhasm: w6_stack = w6
# asm 1: str <w6=int32#9,>w6_stack=stack32#27
# asm 2: str <w6=r8,>w6_stack=[sp,#104]
str r8,[sp,#104]

# qhasm: w7_stack = w7
# asm 1: str <w7=int32#10,>w7_stack=stack32#28
# asm 2: str <w7=r9,>w7_stack=[sp,#108]
str r9,[sp,#108]

# qhasm: w8_stack = w8
# asm 1: str <w8=int32#11,>w8_stack=stack32#29
# asm 2: str <w8=r10,>w8_stack=[sp,#112]
str r10,[sp,#112]

# qhasm: w10_stack = w10
# asm 1: str <w10=int32#8,>w10_stack=stack32#30
# asm 2: str <w10=r7,>w10_stack=[sp,#116]
str r7,[sp,#116]

# qhasm: w11_stack = w11
# asm 1: str <w11=int32#12,>w11_stack=stack32#31
# asm 2: str <w11=r11,>w11_stack=[sp,#120]
str r11,[sp,#120]

# qhasm: w12_stack = w12
# asm 1: str <w12=int32#13,>w12_stack=stack32#32
# asm 2: str <w12=r12,>w12_stack=[sp,#124]
str r12,[sp,#124]

# qhasm: w13_stack = w13
# asm 1: str <w13=int32#3,>w13_stack=stack32#33
# asm 2: str <w13=r2,>w13_stack=[sp,#128]
str r2,[sp,#128]

# qhasm: w14_stack = w14
# asm 1: str <w14=int32#5,>w14_stack=stack32#34
# asm 2: str <w14=r4,>w14_stack=[sp,#132]
str r4,[sp,#132]

# qhasm: w15_stack = w15
# asm 1: str <w15=int32#6,>w15_stack=stack32#35
# asm 2: str <w15=r5,>w15_stack=[sp,#136]
str r5,[sp,#136]

# qhasm: r0 = r0_stack
# asm 1: ldr >r0=int32#1,<r0_stack=stack32#36
# asm 2: ldr >r0=r0,<r0_stack=[sp,#140]
ldr r0,[sp,#140]

# qhasm: r1 = r1_stack
# asm 1: ldr >r1=int32#3,<r1_stack=stack32#37
# asm 2: ldr >r1=r2,<r1_stack=[sp,#144]
ldr r2,[sp,#144]

# qhasm: r2 = r2_stack
# asm 1: ldr >r2=int32#5,<r2_stack=stack32#38
# asm 2: ldr >r2=r4,<r2_stack=[sp,#148]
ldr r4,[sp,#148]

# qhasm: r3 = r3_stack
# asm 1: ldr >r3=int32#6,<r3_stack=stack32#39
# asm 2: ldr >r3=r5,<r3_stack=[sp,#152]
ldr r5,[sp,#152]

# qhasm: r4 = r4_stack
# asm 1: ldr >r4=int32#7,<r4_stack=stack32#40
# asm 2: ldr >r4=r6,<r4_stack=[sp,#156]
ldr r6,[sp,#156]

# qhasm: r5 = r5_stack
# asm 1: ldr >r5=int32#8,<r5_stack=stack32#41
# asm 2: ldr >r5=r7,<r5_stack=[sp,#160]
ldr r7,[sp,#160]

# qhasm: r6 = r6_stack
# asm 1: ldr >r6=int32#9,<r6_stack=stack32#42
# asm 2: ldr >r6=r8,<r6_stack=[sp,#164]
ldr r8,[sp,#164]

# qhasm: r7 = r7_stack
# asm 1: ldr >r7=int32#10,<r7_stack=stack32#43
# asm 2: ldr >r7=r9,<r7_stack=[sp,#168]
ldr r9,[sp,#168]

# qhasm: w0 = w0_stack
# asm 1: ldr >w0=int32#11,<w0_stack=stack32#25
# asm 2: ldr >w0=r10,<w0_stack=[sp,#96]
ldr r10,[sp,#96]

# qhasm: t0 = r4 & r5
# asm 1: and >t0=int32#12,<r4=int32#7,<r5=int32#8
# asm 2: and >t0=r11,<r4=r6,<r5=r7
and r11,r6,r7

# qhasm: t1 = r6 & ~r4
# asm 1: bic >t1=int32#13,<r6=int32#9,<r4=int32#7
# asm 2: bic >t1=r12,<r6=r8,<r4=r6
bic r12,r8,r6

# qhasm: t0 ^= t1
# asm 1: eor >t0=int32#12,<t0=int32#12,<t1=int32#13
# asm 2: eor >t0=r11,<t0=r11,<t1=r12
eor r11,r11,r12

# qhasm: t1 = r4 ^ (r4 >>> 5)
# asm 1: eor >t1=int32#13,<r4=int32#7,<r4=int32#7,ROR #5
# asm 2: eor >t1=r12,<r4=r6,<r4=r6,ROR #5
eor r12,r6,r6,ROR #5

# qhasm: t1 ^= (r4 >>> 19)
# asm 1: eor >t1=int32#13,<t1=int32#13,<r4=int32#7,ROR #19
# asm 2: eor >t1=r12,<t1=r12,<r4=r6,ROR #19
eor r12,r12,r6,ROR #19

# qhasm: t2 = mem32[input_3 + 64]
# asm 1: ldr >t2=int32#14,[<input_3=int32#4,#64]
# asm 2: ldr >t2=r14,[<input_3=r3,#64]
ldr r14,[r3,#64]

# qhasm: t0 += w0
# asm 1: add >t0=int32#11,<t0=int32#12,<w0=int32#11
# asm 2: add >t0=r10,<t0=r11,<w0=r10
add r10,r11,r10

# qhasm: t0 += (t1 >>> 6)
# asm 1: add >t0=int32#11,<t0=int32#11,<t1=int32#13,ROR #6
# asm 2: add >t0=r10,<t0=r10,<t1=r12,ROR #6
add r10,r10,r12,ROR #6

# qhasm: t0 += t2
# asm 1: add >t0=int32#11,<t0=int32#11,<t2=int32#14
# asm 2: add >t0=r10,<t0=r10,<t2=r14
add r10,r10,r14

# qhasm: r7 += t0
# asm 1: add >r7=int32#10,<r7=int32#10,<t0=int32#11
# asm 2: add >r7=r9,<r7=r9,<t0=r10
add r9,r9,r10

# qhasm: r3 += r7
# asm 1: add >r3=int32#6,<r3=int32#6,<r7=int32#10
# asm 2: add >r3=r5,<r3=r5,<r7=r9
add r5,r5,r9

# qhasm: t0 = r0 & r1
# asm 1: and >t0=int32#11,<r0=int32#1,<r1=int32#3
# asm 2: and >t0=r10,<r0=r0,<r1=r2
and r10,r0,r2

# qhasm: t2 = r0 & r2
# asm 1: and >t2=int32#12,<r0=int32#1,<r2=int32#5
# asm 2: and >t2=r11,<r0=r0,<r2=r4
and r11,r0,r4

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t2 = r1 & r2
# asm 1: and >t2=int32#12,<r1=int32#3,<r2=int32#5
# asm 2: and >t2=r11,<r1=r2,<r2=r4
and r11,r2,r4

# qhasm: t3 = r0 ^ (r0 >>> 11)
# asm 1: eor >t3=int32#13,<r0=int32#1,<r0=int32#1,ROR #11
# asm 2: eor >t3=r12,<r0=r0,<r0=r0,ROR #11
eor r12,r0,r0,ROR #11

# qhasm: t3 ^= (r0 >>> 20)
# asm 1: eor >t3=int32#13,<t3=int32#13,<r0=int32#1,ROR #20
# asm 2: eor >t3=r12,<t3=r12,<r0=r0,ROR #20
eor r12,r12,r0,ROR #20

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t0 += (t3 >>> 2)
# asm 1: add >t0=int32#11,<t0=int32#11,<t3=int32#13,ROR #2
# asm 2: add >t0=r10,<t0=r10,<t3=r12,ROR #2
add r10,r10,r12,ROR #2

# qhasm: r7 += t0
# asm 1: add >r7=int32#10,<r7=int32#10,<t0=int32#11
# asm 2: add >r7=r9,<r7=r9,<t0=r10
add r9,r9,r10

# qhasm: w1 = w1_stack
# asm 1: ldr >w1=int32#11,<w1_stack=stack32#21
# asm 2: ldr >w1=r10,<w1_stack=[sp,#80]
ldr r10,[sp,#80]

# qhasm: t0 = r3 & r4
# asm 1: and >t0=int32#12,<r3=int32#6,<r4=int32#7
# asm 2: and >t0=r11,<r3=r5,<r4=r6
and r11,r5,r6

# qhasm: t1 = r5 & ~r3
# asm 1: bic >t1=int32#13,<r5=int32#8,<r3=int32#6
# asm 2: bic >t1=r12,<r5=r7,<r3=r5
bic r12,r7,r5

# qhasm: t0 ^= t1
# asm 1: eor >t0=int32#12,<t0=int32#12,<t1=int32#13
# asm 2: eor >t0=r11,<t0=r11,<t1=r12
eor r11,r11,r12

# qhasm: t1 = r3 ^ (r3 >>> 5)
# asm 1: eor >t1=int32#13,<r3=int32#6,<r3=int32#6,ROR #5
# asm 2: eor >t1=r12,<r3=r5,<r3=r5,ROR #5
eor r12,r5,r5,ROR #5

# qhasm: t1 ^= (r3 >>> 19)
# asm 1: eor >t1=int32#13,<t1=int32#13,<r3=int32#6,ROR #19
# asm 2: eor >t1=r12,<t1=r12,<r3=r5,ROR #19
eor r12,r12,r5,ROR #19

# qhasm: t2 = mem32[input_3 + 68]
# asm 1: ldr >t2=int32#14,[<input_3=int32#4,#68]
# asm 2: ldr >t2=r14,[<input_3=r3,#68]
ldr r14,[r3,#68]

# qhasm: t0 += w1
# asm 1: add >t0=int32#11,<t0=int32#12,<w1=int32#11
# asm 2: add >t0=r10,<t0=r11,<w1=r10
add r10,r11,r10

# qhasm: t0 += (t1 >>> 6)
# asm 1: add >t0=int32#11,<t0=int32#11,<t1=int32#13,ROR #6
# asm 2: add >t0=r10,<t0=r10,<t1=r12,ROR #6
add r10,r10,r12,ROR #6

# qhasm: t0 += t2
# asm 1: add >t0=int32#11,<t0=int32#11,<t2=int32#14
# asm 2: add >t0=r10,<t0=r10,<t2=r14
add r10,r10,r14

# qhasm: r6 += t0
# asm 1: add >r6=int32#9,<r6=int32#9,<t0=int32#11
# asm 2: add >r6=r8,<r6=r8,<t0=r10
add r8,r8,r10

# qhasm: r2 += r6
# asm 1: add >r2=int32#5,<r2=int32#5,<r6=int32#9
# asm 2: add >r2=r4,<r2=r4,<r6=r8
add r4,r4,r8

# qhasm: t0 = r7 & r0
# asm 1: and >t0=int32#11,<r7=int32#10,<r0=int32#1
# asm 2: and >t0=r10,<r7=r9,<r0=r0
and r10,r9,r0

# qhasm: t2 = r7 & r1
# asm 1: and >t2=int32#12,<r7=int32#10,<r1=int32#3
# asm 2: and >t2=r11,<r7=r9,<r1=r2
and r11,r9,r2

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t2 = r0 & r1
# asm 1: and >t2=int32#12,<r0=int32#1,<r1=int32#3
# asm 2: and >t2=r11,<r0=r0,<r1=r2
and r11,r0,r2

# qhasm: t3 = r7 ^ (r7 >>> 11)
# asm 1: eor >t3=int32#13,<r7=int32#10,<r7=int32#10,ROR #11
# asm 2: eor >t3=r12,<r7=r9,<r7=r9,ROR #11
eor r12,r9,r9,ROR #11

# qhasm: t3 ^= (r7 >>> 20)
# asm 1: eor >t3=int32#13,<t3=int32#13,<r7=int32#10,ROR #20
# asm 2: eor >t3=r12,<t3=r12,<r7=r9,ROR #20
eor r12,r12,r9,ROR #20

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t0 += (t3 >>> 2)
# asm 1: add >t0=int32#11,<t0=int32#11,<t3=int32#13,ROR #2
# asm 2: add >t0=r10,<t0=r10,<t3=r12,ROR #2
add r10,r10,r12,ROR #2

# qhasm: r6 += t0
# asm 1: add >r6=int32#9,<r6=int32#9,<t0=int32#11
# asm 2: add >r6=r8,<r6=r8,<t0=r10
add r8,r8,r10

# qhasm: w2 = w2_stack
# asm 1: ldr >w2=int32#11,<w2_stack=stack32#22
# asm 2: ldr >w2=r10,<w2_stack=[sp,#84]
ldr r10,[sp,#84]

# qhasm: t0 = r2 & r3
# asm 1: and >t0=int32#12,<r2=int32#5,<r3=int32#6
# asm 2: and >t0=r11,<r2=r4,<r3=r5
and r11,r4,r5

# qhasm: t1 = r4 & ~r2
# asm 1: bic >t1=int32#13,<r4=int32#7,<r2=int32#5
# asm 2: bic >t1=r12,<r4=r6,<r2=r4
bic r12,r6,r4

# qhasm: t0 ^= t1
# asm 1: eor >t0=int32#12,<t0=int32#12,<t1=int32#13
# asm 2: eor >t0=r11,<t0=r11,<t1=r12
eor r11,r11,r12

# qhasm: t1 = r2 ^ (r2 >>> 5)
# asm 1: eor >t1=int32#13,<r2=int32#5,<r2=int32#5,ROR #5
# asm 2: eor >t1=r12,<r2=r4,<r2=r4,ROR #5
eor r12,r4,r4,ROR #5

# qhasm: t1 ^= (r2 >>> 19)
# asm 1: eor >t1=int32#13,<t1=int32#13,<r2=int32#5,ROR #19
# asm 2: eor >t1=r12,<t1=r12,<r2=r4,ROR #19
eor r12,r12,r4,ROR #19

# qhasm: t2 = mem32[input_3 + 72]
# asm 1: ldr >t2=int32#14,[<input_3=int32#4,#72]
# asm 2: ldr >t2=r14,[<input_3=r3,#72]
ldr r14,[r3,#72]

# qhasm: t0 += w2
# asm 1: add >t0=int32#11,<t0=int32#12,<w2=int32#11
# asm 2: add >t0=r10,<t0=r11,<w2=r10
add r10,r11,r10

# qhasm: t0 += (t1 >>> 6)
# asm 1: add >t0=int32#11,<t0=int32#11,<t1=int32#13,ROR #6
# asm 2: add >t0=r10,<t0=r10,<t1=r12,ROR #6
add r10,r10,r12,ROR #6

# qhasm: t0 += t2
# asm 1: add >t0=int32#11,<t0=int32#11,<t2=int32#14
# asm 2: add >t0=r10,<t0=r10,<t2=r14
add r10,r10,r14

# qhasm: r5 += t0
# asm 1: add >r5=int32#8,<r5=int32#8,<t0=int32#11
# asm 2: add >r5=r7,<r5=r7,<t0=r10
add r7,r7,r10

# qhasm: r1 += r5
# asm 1: add >r1=int32#3,<r1=int32#3,<r5=int32#8
# asm 2: add >r1=r2,<r1=r2,<r5=r7
add r2,r2,r7

# qhasm: t0 = r6 & r7
# asm 1: and >t0=int32#11,<r6=int32#9,<r7=int32#10
# asm 2: and >t0=r10,<r6=r8,<r7=r9
and r10,r8,r9

# qhasm: t2 = r6 & r0
# asm 1: and >t2=int32#12,<r6=int32#9,<r0=int32#1
# asm 2: and >t2=r11,<r6=r8,<r0=r0
and r11,r8,r0

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t2 = r7 & r0
# asm 1: and >t2=int32#12,<r7=int32#10,<r0=int32#1
# asm 2: and >t2=r11,<r7=r9,<r0=r0
and r11,r9,r0

# qhasm: t3 = r6 ^ (r6 >>> 11)
# asm 1: eor >t3=int32#13,<r6=int32#9,<r6=int32#9,ROR #11
# asm 2: eor >t3=r12,<r6=r8,<r6=r8,ROR #11
eor r12,r8,r8,ROR #11

# qhasm: t3 ^= (r6 >>> 20)
# asm 1: eor >t3=int32#13,<t3=int32#13,<r6=int32#9,ROR #20
# asm 2: eor >t3=r12,<t3=r12,<r6=r8,ROR #20
eor r12,r12,r8,ROR #20

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t0 += (t3 >>> 2)
# asm 1: add >t0=int32#11,<t0=int32#11,<t3=int32#13,ROR #2
# asm 2: add >t0=r10,<t0=r10,<t3=r12,ROR #2
add r10,r10,r12,ROR #2

# qhasm: r5 += t0
# asm 1: add >r5=int32#8,<r5=int32#8,<t0=int32#11
# asm 2: add >r5=r7,<r5=r7,<t0=r10
add r7,r7,r10

# qhasm: w3 = w3_stack
# asm 1: ldr >w3=int32#11,<w3_stack=stack32#23
# asm 2: ldr >w3=r10,<w3_stack=[sp,#88]
ldr r10,[sp,#88]

# qhasm: t0 = r1 & r2
# asm 1: and >t0=int32#12,<r1=int32#3,<r2=int32#5
# asm 2: and >t0=r11,<r1=r2,<r2=r4
and r11,r2,r4

# qhasm: t1 = r3 & ~r1
# asm 1: bic >t1=int32#13,<r3=int32#6,<r1=int32#3
# asm 2: bic >t1=r12,<r3=r5,<r1=r2
bic r12,r5,r2

# qhasm: t0 ^= t1
# asm 1: eor >t0=int32#12,<t0=int32#12,<t1=int32#13
# asm 2: eor >t0=r11,<t0=r11,<t1=r12
eor r11,r11,r12

# qhasm: t1 = r1 ^ (r1 >>> 5)
# asm 1: eor >t1=int32#13,<r1=int32#3,<r1=int32#3,ROR #5
# asm 2: eor >t1=r12,<r1=r2,<r1=r2,ROR #5
eor r12,r2,r2,ROR #5

# qhasm: t1 ^= (r1 >>> 19)
# asm 1: eor >t1=int32#13,<t1=int32#13,<r1=int32#3,ROR #19
# asm 2: eor >t1=r12,<t1=r12,<r1=r2,ROR #19
eor r12,r12,r2,ROR #19

# qhasm: t2 = mem32[input_3 + 76]
# asm 1: ldr >t2=int32#14,[<input_3=int32#4,#76]
# asm 2: ldr >t2=r14,[<input_3=r3,#76]
ldr r14,[r3,#76]

# qhasm: t0 += w3
# asm 1: add >t0=int32#11,<t0=int32#12,<w3=int32#11
# asm 2: add >t0=r10,<t0=r11,<w3=r10
add r10,r11,r10

# qhasm: t0 += (t1 >>> 6)
# asm 1: add >t0=int32#11,<t0=int32#11,<t1=int32#13,ROR #6
# asm 2: add >t0=r10,<t0=r10,<t1=r12,ROR #6
add r10,r10,r12,ROR #6

# qhasm: t0 += t2
# asm 1: add >t0=int32#11,<t0=int32#11,<t2=int32#14
# asm 2: add >t0=r10,<t0=r10,<t2=r14
add r10,r10,r14

# qhasm: r4 += t0
# asm 1: add >r4=int32#7,<r4=int32#7,<t0=int32#11
# asm 2: add >r4=r6,<r4=r6,<t0=r10
add r6,r6,r10

# qhasm: r0 += r4
# asm 1: add >r0=int32#1,<r0=int32#1,<r4=int32#7
# asm 2: add >r0=r0,<r0=r0,<r4=r6
add r0,r0,r6

# qhasm: t0 = r5 & r6
# asm 1: and >t0=int32#11,<r5=int32#8,<r6=int32#9
# asm 2: and >t0=r10,<r5=r7,<r6=r8
and r10,r7,r8

# qhasm: t2 = r5 & r7
# asm 1: and >t2=int32#12,<r5=int32#8,<r7=int32#10
# asm 2: and >t2=r11,<r5=r7,<r7=r9
and r11,r7,r9

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t2 = r6 & r7
# asm 1: and >t2=int32#12,<r6=int32#9,<r7=int32#10
# asm 2: and >t2=r11,<r6=r8,<r7=r9
and r11,r8,r9

# qhasm: t3 = r5 ^ (r5 >>> 11)
# asm 1: eor >t3=int32#13,<r5=int32#8,<r5=int32#8,ROR #11
# asm 2: eor >t3=r12,<r5=r7,<r5=r7,ROR #11
eor r12,r7,r7,ROR #11

# qhasm: t3 ^= (r5 >>> 20)
# asm 1: eor >t3=int32#13,<t3=int32#13,<r5=int32#8,ROR #20
# asm 2: eor >t3=r12,<t3=r12,<r5=r7,ROR #20
eor r12,r12,r7,ROR #20

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t0 += (t3 >>> 2)
# asm 1: add >t0=int32#11,<t0=int32#11,<t3=int32#13,ROR #2
# asm 2: add >t0=r10,<t0=r10,<t3=r12,ROR #2
add r10,r10,r12,ROR #2

# qhasm: r4 += t0
# asm 1: add >r4=int32#7,<r4=int32#7,<t0=int32#11
# asm 2: add >r4=r6,<r4=r6,<t0=r10
add r6,r6,r10

# qhasm: w4 = w4_stack
# asm 1: ldr >w4=int32#11,<w4_stack=stack32#24
# asm 2: ldr >w4=r10,<w4_stack=[sp,#92]
ldr r10,[sp,#92]

# qhasm: t0 = r0 & r1
# asm 1: and >t0=int32#12,<r0=int32#1,<r1=int32#3
# asm 2: and >t0=r11,<r0=r0,<r1=r2
and r11,r0,r2

# qhasm: t1 = r2 & ~r0
# asm 1: bic >t1=int32#13,<r2=int32#5,<r0=int32#1
# asm 2: bic >t1=r12,<r2=r4,<r0=r0
bic r12,r4,r0

# qhasm: t0 ^= t1
# asm 1: eor >t0=int32#12,<t0=int32#12,<t1=int32#13
# asm 2: eor >t0=r11,<t0=r11,<t1=r12
eor r11,r11,r12

# qhasm: t1 = r0 ^ (r0 >>> 5)
# asm 1: eor >t1=int32#13,<r0=int32#1,<r0=int32#1,ROR #5
# asm 2: eor >t1=r12,<r0=r0,<r0=r0,ROR #5
eor r12,r0,r0,ROR #5

# qhasm: t1 ^= (r0 >>> 19)
# asm 1: eor >t1=int32#13,<t1=int32#13,<r0=int32#1,ROR #19
# asm 2: eor >t1=r12,<t1=r12,<r0=r0,ROR #19
eor r12,r12,r0,ROR #19

# qhasm: t2 = mem32[input_3 + 80]
# asm 1: ldr >t2=int32#14,[<input_3=int32#4,#80]
# asm 2: ldr >t2=r14,[<input_3=r3,#80]
ldr r14,[r3,#80]

# qhasm: t0 += w4
# asm 1: add >t0=int32#11,<t0=int32#12,<w4=int32#11
# asm 2: add >t0=r10,<t0=r11,<w4=r10
add r10,r11,r10

# qhasm: t0 += (t1 >>> 6)
# asm 1: add >t0=int32#11,<t0=int32#11,<t1=int32#13,ROR #6
# asm 2: add >t0=r10,<t0=r10,<t1=r12,ROR #6
add r10,r10,r12,ROR #6

# qhasm: t0 += t2
# asm 1: add >t0=int32#11,<t0=int32#11,<t2=int32#14
# asm 2: add >t0=r10,<t0=r10,<t2=r14
add r10,r10,r14

# qhasm: r3 += t0
# asm 1: add >r3=int32#6,<r3=int32#6,<t0=int32#11
# asm 2: add >r3=r5,<r3=r5,<t0=r10
add r5,r5,r10

# qhasm: r7 += r3
# asm 1: add >r7=int32#10,<r7=int32#10,<r3=int32#6
# asm 2: add >r7=r9,<r7=r9,<r3=r5
add r9,r9,r5

# qhasm: t0 = r4 & r5
# asm 1: and >t0=int32#11,<r4=int32#7,<r5=int32#8
# asm 2: and >t0=r10,<r4=r6,<r5=r7
and r10,r6,r7

# qhasm: t2 = r4 & r6
# asm 1: and >t2=int32#12,<r4=int32#7,<r6=int32#9
# asm 2: and >t2=r11,<r4=r6,<r6=r8
and r11,r6,r8

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t2 = r5 & r6
# asm 1: and >t2=int32#12,<r5=int32#8,<r6=int32#9
# asm 2: and >t2=r11,<r5=r7,<r6=r8
and r11,r7,r8

# qhasm: t3 = r4 ^ (r4 >>> 11)
# asm 1: eor >t3=int32#13,<r4=int32#7,<r4=int32#7,ROR #11
# asm 2: eor >t3=r12,<r4=r6,<r4=r6,ROR #11
eor r12,r6,r6,ROR #11

# qhasm: t3 ^= (r4 >>> 20)
# asm 1: eor >t3=int32#13,<t3=int32#13,<r4=int32#7,ROR #20
# asm 2: eor >t3=r12,<t3=r12,<r4=r6,ROR #20
eor r12,r12,r6,ROR #20

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t0 += (t3 >>> 2)
# asm 1: add >t0=int32#11,<t0=int32#11,<t3=int32#13,ROR #2
# asm 2: add >t0=r10,<t0=r10,<t3=r12,ROR #2
add r10,r10,r12,ROR #2

# qhasm: r3 += t0
# asm 1: add >r3=int32#6,<r3=int32#6,<t0=int32#11
# asm 2: add >r3=r5,<r3=r5,<t0=r10
add r5,r5,r10

# qhasm: w5 = w5_stack
# asm 1: ldr >w5=int32#11,<w5_stack=stack32#26
# asm 2: ldr >w5=r10,<w5_stack=[sp,#100]
ldr r10,[sp,#100]

# qhasm: t0 = r7 & r0
# asm 1: and >t0=int32#12,<r7=int32#10,<r0=int32#1
# asm 2: and >t0=r11,<r7=r9,<r0=r0
and r11,r9,r0

# qhasm: t1 = r1 & ~r7
# asm 1: bic >t1=int32#13,<r1=int32#3,<r7=int32#10
# asm 2: bic >t1=r12,<r1=r2,<r7=r9
bic r12,r2,r9

# qhasm: t0 ^= t1
# asm 1: eor >t0=int32#12,<t0=int32#12,<t1=int32#13
# asm 2: eor >t0=r11,<t0=r11,<t1=r12
eor r11,r11,r12

# qhasm: t1 = r7 ^ (r7 >>> 5)
# asm 1: eor >t1=int32#13,<r7=int32#10,<r7=int32#10,ROR #5
# asm 2: eor >t1=r12,<r7=r9,<r7=r9,ROR #5
eor r12,r9,r9,ROR #5

# qhasm: t1 ^= (r7 >>> 19)
# asm 1: eor >t1=int32#13,<t1=int32#13,<r7=int32#10,ROR #19
# asm 2: eor >t1=r12,<t1=r12,<r7=r9,ROR #19
eor r12,r12,r9,ROR #19

# qhasm: t2 = mem32[input_3 + 84]
# asm 1: ldr >t2=int32#14,[<input_3=int32#4,#84]
# asm 2: ldr >t2=r14,[<input_3=r3,#84]
ldr r14,[r3,#84]

# qhasm: t0 += w5
# asm 1: add >t0=int32#11,<t0=int32#12,<w5=int32#11
# asm 2: add >t0=r10,<t0=r11,<w5=r10
add r10,r11,r10

# qhasm: t0 += (t1 >>> 6)
# asm 1: add >t0=int32#11,<t0=int32#11,<t1=int32#13,ROR #6
# asm 2: add >t0=r10,<t0=r10,<t1=r12,ROR #6
add r10,r10,r12,ROR #6

# qhasm: t0 += t2
# asm 1: add >t0=int32#11,<t0=int32#11,<t2=int32#14
# asm 2: add >t0=r10,<t0=r10,<t2=r14
add r10,r10,r14

# qhasm: r2 += t0
# asm 1: add >r2=int32#5,<r2=int32#5,<t0=int32#11
# asm 2: add >r2=r4,<r2=r4,<t0=r10
add r4,r4,r10

# qhasm: r6 += r2
# asm 1: add >r6=int32#9,<r6=int32#9,<r2=int32#5
# asm 2: add >r6=r8,<r6=r8,<r2=r4
add r8,r8,r4

# qhasm: t0 = r3 & r4
# asm 1: and >t0=int32#11,<r3=int32#6,<r4=int32#7
# asm 2: and >t0=r10,<r3=r5,<r4=r6
and r10,r5,r6

# qhasm: t2 = r3 & r5
# asm 1: and >t2=int32#12,<r3=int32#6,<r5=int32#8
# asm 2: and >t2=r11,<r3=r5,<r5=r7
and r11,r5,r7

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t2 = r4 & r5
# asm 1: and >t2=int32#12,<r4=int32#7,<r5=int32#8
# asm 2: and >t2=r11,<r4=r6,<r5=r7
and r11,r6,r7

# qhasm: t3 = r3 ^ (r3 >>> 11)
# asm 1: eor >t3=int32#13,<r3=int32#6,<r3=int32#6,ROR #11
# asm 2: eor >t3=r12,<r3=r5,<r3=r5,ROR #11
eor r12,r5,r5,ROR #11

# qhasm: t3 ^= (r3 >>> 20)
# asm 1: eor >t3=int32#13,<t3=int32#13,<r3=int32#6,ROR #20
# asm 2: eor >t3=r12,<t3=r12,<r3=r5,ROR #20
eor r12,r12,r5,ROR #20

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t0 += (t3 >>> 2)
# asm 1: add >t0=int32#11,<t0=int32#11,<t3=int32#13,ROR #2
# asm 2: add >t0=r10,<t0=r10,<t3=r12,ROR #2
add r10,r10,r12,ROR #2

# qhasm: r2 += t0
# asm 1: add >r2=int32#5,<r2=int32#5,<t0=int32#11
# asm 2: add >r2=r4,<r2=r4,<t0=r10
add r4,r4,r10

# qhasm: w6 = w6_stack
# asm 1: ldr >w6=int32#11,<w6_stack=stack32#27
# asm 2: ldr >w6=r10,<w6_stack=[sp,#104]
ldr r10,[sp,#104]

# qhasm: t0 = r6 & r7
# asm 1: and >t0=int32#12,<r6=int32#9,<r7=int32#10
# asm 2: and >t0=r11,<r6=r8,<r7=r9
and r11,r8,r9

# qhasm: t1 = r0 & ~r6
# asm 1: bic >t1=int32#13,<r0=int32#1,<r6=int32#9
# asm 2: bic >t1=r12,<r0=r0,<r6=r8
bic r12,r0,r8

# qhasm: t0 ^= t1
# asm 1: eor >t0=int32#12,<t0=int32#12,<t1=int32#13
# asm 2: eor >t0=r11,<t0=r11,<t1=r12
eor r11,r11,r12

# qhasm: t1 = r6 ^ (r6 >>> 5)
# asm 1: eor >t1=int32#13,<r6=int32#9,<r6=int32#9,ROR #5
# asm 2: eor >t1=r12,<r6=r8,<r6=r8,ROR #5
eor r12,r8,r8,ROR #5

# qhasm: t1 ^= (r6 >>> 19)
# asm 1: eor >t1=int32#13,<t1=int32#13,<r6=int32#9,ROR #19
# asm 2: eor >t1=r12,<t1=r12,<r6=r8,ROR #19
eor r12,r12,r8,ROR #19

# qhasm: t2 = mem32[input_3 + 88]
# asm 1: ldr >t2=int32#14,[<input_3=int32#4,#88]
# asm 2: ldr >t2=r14,[<input_3=r3,#88]
ldr r14,[r3,#88]

# qhasm: t0 += w6
# asm 1: add >t0=int32#11,<t0=int32#12,<w6=int32#11
# asm 2: add >t0=r10,<t0=r11,<w6=r10
add r10,r11,r10

# qhasm: t0 += (t1 >>> 6)
# asm 1: add >t0=int32#11,<t0=int32#11,<t1=int32#13,ROR #6
# asm 2: add >t0=r10,<t0=r10,<t1=r12,ROR #6
add r10,r10,r12,ROR #6

# qhasm: t0 += t2
# asm 1: add >t0=int32#11,<t0=int32#11,<t2=int32#14
# asm 2: add >t0=r10,<t0=r10,<t2=r14
add r10,r10,r14

# qhasm: r1 += t0
# asm 1: add >r1=int32#3,<r1=int32#3,<t0=int32#11
# asm 2: add >r1=r2,<r1=r2,<t0=r10
add r2,r2,r10

# qhasm: r5 += r1
# asm 1: add >r5=int32#8,<r5=int32#8,<r1=int32#3
# asm 2: add >r5=r7,<r5=r7,<r1=r2
add r7,r7,r2

# qhasm: t0 = r2 & r3
# asm 1: and >t0=int32#11,<r2=int32#5,<r3=int32#6
# asm 2: and >t0=r10,<r2=r4,<r3=r5
and r10,r4,r5

# qhasm: t2 = r2 & r4
# asm 1: and >t2=int32#12,<r2=int32#5,<r4=int32#7
# asm 2: and >t2=r11,<r2=r4,<r4=r6
and r11,r4,r6

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t2 = r3 & r4
# asm 1: and >t2=int32#12,<r3=int32#6,<r4=int32#7
# asm 2: and >t2=r11,<r3=r5,<r4=r6
and r11,r5,r6

# qhasm: t3 = r2 ^ (r2 >>> 11)
# asm 1: eor >t3=int32#13,<r2=int32#5,<r2=int32#5,ROR #11
# asm 2: eor >t3=r12,<r2=r4,<r2=r4,ROR #11
eor r12,r4,r4,ROR #11

# qhasm: t3 ^= (r2 >>> 20)
# asm 1: eor >t3=int32#13,<t3=int32#13,<r2=int32#5,ROR #20
# asm 2: eor >t3=r12,<t3=r12,<r2=r4,ROR #20
eor r12,r12,r4,ROR #20

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t0 += (t3 >>> 2)
# asm 1: add >t0=int32#11,<t0=int32#11,<t3=int32#13,ROR #2
# asm 2: add >t0=r10,<t0=r10,<t3=r12,ROR #2
add r10,r10,r12,ROR #2

# qhasm: r1 += t0
# asm 1: add >r1=int32#3,<r1=int32#3,<t0=int32#11
# asm 2: add >r1=r2,<r1=r2,<t0=r10
add r2,r2,r10

# qhasm: w7 = w7_stack
# asm 1: ldr >w7=int32#11,<w7_stack=stack32#28
# asm 2: ldr >w7=r10,<w7_stack=[sp,#108]
ldr r10,[sp,#108]

# qhasm: t0 = r5 & r6
# asm 1: and >t0=int32#12,<r5=int32#8,<r6=int32#9
# asm 2: and >t0=r11,<r5=r7,<r6=r8
and r11,r7,r8

# qhasm: t1 = r7 & ~r5
# asm 1: bic >t1=int32#13,<r7=int32#10,<r5=int32#8
# asm 2: bic >t1=r12,<r7=r9,<r5=r7
bic r12,r9,r7

# qhasm: t0 ^= t1
# asm 1: eor >t0=int32#12,<t0=int32#12,<t1=int32#13
# asm 2: eor >t0=r11,<t0=r11,<t1=r12
eor r11,r11,r12

# qhasm: t1 = r5 ^ (r5 >>> 5)
# asm 1: eor >t1=int32#13,<r5=int32#8,<r5=int32#8,ROR #5
# asm 2: eor >t1=r12,<r5=r7,<r5=r7,ROR #5
eor r12,r7,r7,ROR #5

# qhasm: t1 ^= (r5 >>> 19)
# asm 1: eor >t1=int32#13,<t1=int32#13,<r5=int32#8,ROR #19
# asm 2: eor >t1=r12,<t1=r12,<r5=r7,ROR #19
eor r12,r12,r7,ROR #19

# qhasm: t2 = mem32[input_3 + 92]
# asm 1: ldr >t2=int32#14,[<input_3=int32#4,#92]
# asm 2: ldr >t2=r14,[<input_3=r3,#92]
ldr r14,[r3,#92]

# qhasm: t0 += w7
# asm 1: add >t0=int32#11,<t0=int32#12,<w7=int32#11
# asm 2: add >t0=r10,<t0=r11,<w7=r10
add r10,r11,r10

# qhasm: t0 += (t1 >>> 6)
# asm 1: add >t0=int32#11,<t0=int32#11,<t1=int32#13,ROR #6
# asm 2: add >t0=r10,<t0=r10,<t1=r12,ROR #6
add r10,r10,r12,ROR #6

# qhasm: t0 += t2
# asm 1: add >t0=int32#11,<t0=int32#11,<t2=int32#14
# asm 2: add >t0=r10,<t0=r10,<t2=r14
add r10,r10,r14

# qhasm: r0 += t0
# asm 1: add >r0=int32#1,<r0=int32#1,<t0=int32#11
# asm 2: add >r0=r0,<r0=r0,<t0=r10
add r0,r0,r10

# qhasm: r4 += r0
# asm 1: add >r4=int32#7,<r4=int32#7,<r0=int32#1
# asm 2: add >r4=r6,<r4=r6,<r0=r0
add r6,r6,r0

# qhasm: t0 = r1 & r2
# asm 1: and >t0=int32#11,<r1=int32#3,<r2=int32#5
# asm 2: and >t0=r10,<r1=r2,<r2=r4
and r10,r2,r4

# qhasm: t2 = r1 & r3
# asm 1: and >t2=int32#12,<r1=int32#3,<r3=int32#6
# asm 2: and >t2=r11,<r1=r2,<r3=r5
and r11,r2,r5

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t2 = r2 & r3
# asm 1: and >t2=int32#12,<r2=int32#5,<r3=int32#6
# asm 2: and >t2=r11,<r2=r4,<r3=r5
and r11,r4,r5

# qhasm: t3 = r1 ^ (r1 >>> 11)
# asm 1: eor >t3=int32#13,<r1=int32#3,<r1=int32#3,ROR #11
# asm 2: eor >t3=r12,<r1=r2,<r1=r2,ROR #11
eor r12,r2,r2,ROR #11

# qhasm: t3 ^= (r1 >>> 20)
# asm 1: eor >t3=int32#13,<t3=int32#13,<r1=int32#3,ROR #20
# asm 2: eor >t3=r12,<t3=r12,<r1=r2,ROR #20
eor r12,r12,r2,ROR #20

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t0 += (t3 >>> 2)
# asm 1: add >t0=int32#11,<t0=int32#11,<t3=int32#13,ROR #2
# asm 2: add >t0=r10,<t0=r10,<t3=r12,ROR #2
add r10,r10,r12,ROR #2

# qhasm: r0 += t0
# asm 1: add >r0=int32#1,<r0=int32#1,<t0=int32#11
# asm 2: add >r0=r0,<r0=r0,<t0=r10
add r0,r0,r10

# qhasm: w8 = w8_stack
# asm 1: ldr >w8=int32#11,<w8_stack=stack32#29
# asm 2: ldr >w8=r10,<w8_stack=[sp,#112]
ldr r10,[sp,#112]

# qhasm: t0 = r4 & r5
# asm 1: and >t0=int32#12,<r4=int32#7,<r5=int32#8
# asm 2: and >t0=r11,<r4=r6,<r5=r7
and r11,r6,r7

# qhasm: t1 = r6 & ~r4
# asm 1: bic >t1=int32#13,<r6=int32#9,<r4=int32#7
# asm 2: bic >t1=r12,<r6=r8,<r4=r6
bic r12,r8,r6

# qhasm: t0 ^= t1
# asm 1: eor >t0=int32#12,<t0=int32#12,<t1=int32#13
# asm 2: eor >t0=r11,<t0=r11,<t1=r12
eor r11,r11,r12

# qhasm: t1 = r4 ^ (r4 >>> 5)
# asm 1: eor >t1=int32#13,<r4=int32#7,<r4=int32#7,ROR #5
# asm 2: eor >t1=r12,<r4=r6,<r4=r6,ROR #5
eor r12,r6,r6,ROR #5

# qhasm: t1 ^= (r4 >>> 19)
# asm 1: eor >t1=int32#13,<t1=int32#13,<r4=int32#7,ROR #19
# asm 2: eor >t1=r12,<t1=r12,<r4=r6,ROR #19
eor r12,r12,r6,ROR #19

# qhasm: t2 = mem32[input_3 + 96]
# asm 1: ldr >t2=int32#14,[<input_3=int32#4,#96]
# asm 2: ldr >t2=r14,[<input_3=r3,#96]
ldr r14,[r3,#96]

# qhasm: t0 += w8
# asm 1: add >t0=int32#11,<t0=int32#12,<w8=int32#11
# asm 2: add >t0=r10,<t0=r11,<w8=r10
add r10,r11,r10

# qhasm: t0 += (t1 >>> 6)
# asm 1: add >t0=int32#11,<t0=int32#11,<t1=int32#13,ROR #6
# asm 2: add >t0=r10,<t0=r10,<t1=r12,ROR #6
add r10,r10,r12,ROR #6

# qhasm: t0 += t2
# asm 1: add >t0=int32#11,<t0=int32#11,<t2=int32#14
# asm 2: add >t0=r10,<t0=r10,<t2=r14
add r10,r10,r14

# qhasm: r7 += t0
# asm 1: add >r7=int32#10,<r7=int32#10,<t0=int32#11
# asm 2: add >r7=r9,<r7=r9,<t0=r10
add r9,r9,r10

# qhasm: r3 += r7
# asm 1: add >r3=int32#6,<r3=int32#6,<r7=int32#10
# asm 2: add >r3=r5,<r3=r5,<r7=r9
add r5,r5,r9

# qhasm: t0 = r0 & r1
# asm 1: and >t0=int32#11,<r0=int32#1,<r1=int32#3
# asm 2: and >t0=r10,<r0=r0,<r1=r2
and r10,r0,r2

# qhasm: t2 = r0 & r2
# asm 1: and >t2=int32#12,<r0=int32#1,<r2=int32#5
# asm 2: and >t2=r11,<r0=r0,<r2=r4
and r11,r0,r4

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t2 = r1 & r2
# asm 1: and >t2=int32#12,<r1=int32#3,<r2=int32#5
# asm 2: and >t2=r11,<r1=r2,<r2=r4
and r11,r2,r4

# qhasm: t3 = r0 ^ (r0 >>> 11)
# asm 1: eor >t3=int32#13,<r0=int32#1,<r0=int32#1,ROR #11
# asm 2: eor >t3=r12,<r0=r0,<r0=r0,ROR #11
eor r12,r0,r0,ROR #11

# qhasm: t3 ^= (r0 >>> 20)
# asm 1: eor >t3=int32#13,<t3=int32#13,<r0=int32#1,ROR #20
# asm 2: eor >t3=r12,<t3=r12,<r0=r0,ROR #20
eor r12,r12,r0,ROR #20

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t0 += (t3 >>> 2)
# asm 1: add >t0=int32#11,<t0=int32#11,<t3=int32#13,ROR #2
# asm 2: add >t0=r10,<t0=r10,<t3=r12,ROR #2
add r10,r10,r12,ROR #2

# qhasm: r7 += t0
# asm 1: add >r7=int32#10,<r7=int32#10,<t0=int32#11
# asm 2: add >r7=r9,<r7=r9,<t0=r10
add r9,r9,r10

# qhasm: w9 = w9_stack
# asm 1: ldr >w9=int32#11,<w9_stack=stack32#20
# asm 2: ldr >w9=r10,<w9_stack=[sp,#76]
ldr r10,[sp,#76]

# qhasm: t0 = r3 & r4
# asm 1: and >t0=int32#12,<r3=int32#6,<r4=int32#7
# asm 2: and >t0=r11,<r3=r5,<r4=r6
and r11,r5,r6

# qhasm: t1 = r5 & ~r3
# asm 1: bic >t1=int32#13,<r5=int32#8,<r3=int32#6
# asm 2: bic >t1=r12,<r5=r7,<r3=r5
bic r12,r7,r5

# qhasm: t0 ^= t1
# asm 1: eor >t0=int32#12,<t0=int32#12,<t1=int32#13
# asm 2: eor >t0=r11,<t0=r11,<t1=r12
eor r11,r11,r12

# qhasm: t1 = r3 ^ (r3 >>> 5)
# asm 1: eor >t1=int32#13,<r3=int32#6,<r3=int32#6,ROR #5
# asm 2: eor >t1=r12,<r3=r5,<r3=r5,ROR #5
eor r12,r5,r5,ROR #5

# qhasm: t1 ^= (r3 >>> 19)
# asm 1: eor >t1=int32#13,<t1=int32#13,<r3=int32#6,ROR #19
# asm 2: eor >t1=r12,<t1=r12,<r3=r5,ROR #19
eor r12,r12,r5,ROR #19

# qhasm: t2 = mem32[input_3 + 100]
# asm 1: ldr >t2=int32#14,[<input_3=int32#4,#100]
# asm 2: ldr >t2=r14,[<input_3=r3,#100]
ldr r14,[r3,#100]

# qhasm: t0 += w9
# asm 1: add >t0=int32#11,<t0=int32#12,<w9=int32#11
# asm 2: add >t0=r10,<t0=r11,<w9=r10
add r10,r11,r10

# qhasm: t0 += (t1 >>> 6)
# asm 1: add >t0=int32#11,<t0=int32#11,<t1=int32#13,ROR #6
# asm 2: add >t0=r10,<t0=r10,<t1=r12,ROR #6
add r10,r10,r12,ROR #6

# qhasm: t0 += t2
# asm 1: add >t0=int32#11,<t0=int32#11,<t2=int32#14
# asm 2: add >t0=r10,<t0=r10,<t2=r14
add r10,r10,r14

# qhasm: r6 += t0
# asm 1: add >r6=int32#9,<r6=int32#9,<t0=int32#11
# asm 2: add >r6=r8,<r6=r8,<t0=r10
add r8,r8,r10

# qhasm: r2 += r6
# asm 1: add >r2=int32#5,<r2=int32#5,<r6=int32#9
# asm 2: add >r2=r4,<r2=r4,<r6=r8
add r4,r4,r8

# qhasm: t0 = r7 & r0
# asm 1: and >t0=int32#11,<r7=int32#10,<r0=int32#1
# asm 2: and >t0=r10,<r7=r9,<r0=r0
and r10,r9,r0

# qhasm: t2 = r7 & r1
# asm 1: and >t2=int32#12,<r7=int32#10,<r1=int32#3
# asm 2: and >t2=r11,<r7=r9,<r1=r2
and r11,r9,r2

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t2 = r0 & r1
# asm 1: and >t2=int32#12,<r0=int32#1,<r1=int32#3
# asm 2: and >t2=r11,<r0=r0,<r1=r2
and r11,r0,r2

# qhasm: t3 = r7 ^ (r7 >>> 11)
# asm 1: eor >t3=int32#13,<r7=int32#10,<r7=int32#10,ROR #11
# asm 2: eor >t3=r12,<r7=r9,<r7=r9,ROR #11
eor r12,r9,r9,ROR #11

# qhasm: t3 ^= (r7 >>> 20)
# asm 1: eor >t3=int32#13,<t3=int32#13,<r7=int32#10,ROR #20
# asm 2: eor >t3=r12,<t3=r12,<r7=r9,ROR #20
eor r12,r12,r9,ROR #20

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t0 += (t3 >>> 2)
# asm 1: add >t0=int32#11,<t0=int32#11,<t3=int32#13,ROR #2
# asm 2: add >t0=r10,<t0=r10,<t3=r12,ROR #2
add r10,r10,r12,ROR #2

# qhasm: r6 += t0
# asm 1: add >r6=int32#9,<r6=int32#9,<t0=int32#11
# asm 2: add >r6=r8,<r6=r8,<t0=r10
add r8,r8,r10

# qhasm: w10 = w10_stack
# asm 1: ldr >w10=int32#11,<w10_stack=stack32#30
# asm 2: ldr >w10=r10,<w10_stack=[sp,#116]
ldr r10,[sp,#116]

# qhasm: t0 = r2 & r3
# asm 1: and >t0=int32#12,<r2=int32#5,<r3=int32#6
# asm 2: and >t0=r11,<r2=r4,<r3=r5
and r11,r4,r5

# qhasm: t1 = r4 & ~r2
# asm 1: bic >t1=int32#13,<r4=int32#7,<r2=int32#5
# asm 2: bic >t1=r12,<r4=r6,<r2=r4
bic r12,r6,r4

# qhasm: t0 ^= t1
# asm 1: eor >t0=int32#12,<t0=int32#12,<t1=int32#13
# asm 2: eor >t0=r11,<t0=r11,<t1=r12
eor r11,r11,r12

# qhasm: t1 = r2 ^ (r2 >>> 5)
# asm 1: eor >t1=int32#13,<r2=int32#5,<r2=int32#5,ROR #5
# asm 2: eor >t1=r12,<r2=r4,<r2=r4,ROR #5
eor r12,r4,r4,ROR #5

# qhasm: t1 ^= (r2 >>> 19)
# asm 1: eor >t1=int32#13,<t1=int32#13,<r2=int32#5,ROR #19
# asm 2: eor >t1=r12,<t1=r12,<r2=r4,ROR #19
eor r12,r12,r4,ROR #19

# qhasm: t2 = mem32[input_3 + 104]
# asm 1: ldr >t2=int32#14,[<input_3=int32#4,#104]
# asm 2: ldr >t2=r14,[<input_3=r3,#104]
ldr r14,[r3,#104]

# qhasm: t0 += w10
# asm 1: add >t0=int32#11,<t0=int32#12,<w10=int32#11
# asm 2: add >t0=r10,<t0=r11,<w10=r10
add r10,r11,r10

# qhasm: t0 += (t1 >>> 6)
# asm 1: add >t0=int32#11,<t0=int32#11,<t1=int32#13,ROR #6
# asm 2: add >t0=r10,<t0=r10,<t1=r12,ROR #6
add r10,r10,r12,ROR #6

# qhasm: t0 += t2
# asm 1: add >t0=int32#11,<t0=int32#11,<t2=int32#14
# asm 2: add >t0=r10,<t0=r10,<t2=r14
add r10,r10,r14

# qhasm: r5 += t0
# asm 1: add >r5=int32#8,<r5=int32#8,<t0=int32#11
# asm 2: add >r5=r7,<r5=r7,<t0=r10
add r7,r7,r10

# qhasm: r1 += r5
# asm 1: add >r1=int32#3,<r1=int32#3,<r5=int32#8
# asm 2: add >r1=r2,<r1=r2,<r5=r7
add r2,r2,r7

# qhasm: t0 = r6 & r7
# asm 1: and >t0=int32#11,<r6=int32#9,<r7=int32#10
# asm 2: and >t0=r10,<r6=r8,<r7=r9
and r10,r8,r9

# qhasm: t2 = r6 & r0
# asm 1: and >t2=int32#12,<r6=int32#9,<r0=int32#1
# asm 2: and >t2=r11,<r6=r8,<r0=r0
and r11,r8,r0

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t2 = r7 & r0
# asm 1: and >t2=int32#12,<r7=int32#10,<r0=int32#1
# asm 2: and >t2=r11,<r7=r9,<r0=r0
and r11,r9,r0

# qhasm: t3 = r6 ^ (r6 >>> 11)
# asm 1: eor >t3=int32#13,<r6=int32#9,<r6=int32#9,ROR #11
# asm 2: eor >t3=r12,<r6=r8,<r6=r8,ROR #11
eor r12,r8,r8,ROR #11

# qhasm: t3 ^= (r6 >>> 20)
# asm 1: eor >t3=int32#13,<t3=int32#13,<r6=int32#9,ROR #20
# asm 2: eor >t3=r12,<t3=r12,<r6=r8,ROR #20
eor r12,r12,r8,ROR #20

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t0 += (t3 >>> 2)
# asm 1: add >t0=int32#11,<t0=int32#11,<t3=int32#13,ROR #2
# asm 2: add >t0=r10,<t0=r10,<t3=r12,ROR #2
add r10,r10,r12,ROR #2

# qhasm: r5 += t0
# asm 1: add >r5=int32#8,<r5=int32#8,<t0=int32#11
# asm 2: add >r5=r7,<r5=r7,<t0=r10
add r7,r7,r10

# qhasm: w11 = w11_stack
# asm 1: ldr >w11=int32#11,<w11_stack=stack32#31
# asm 2: ldr >w11=r10,<w11_stack=[sp,#120]
ldr r10,[sp,#120]

# qhasm: t0 = r1 & r2
# asm 1: and >t0=int32#12,<r1=int32#3,<r2=int32#5
# asm 2: and >t0=r11,<r1=r2,<r2=r4
and r11,r2,r4

# qhasm: t1 = r3 & ~r1
# asm 1: bic >t1=int32#13,<r3=int32#6,<r1=int32#3
# asm 2: bic >t1=r12,<r3=r5,<r1=r2
bic r12,r5,r2

# qhasm: t0 ^= t1
# asm 1: eor >t0=int32#12,<t0=int32#12,<t1=int32#13
# asm 2: eor >t0=r11,<t0=r11,<t1=r12
eor r11,r11,r12

# qhasm: t1 = r1 ^ (r1 >>> 5)
# asm 1: eor >t1=int32#13,<r1=int32#3,<r1=int32#3,ROR #5
# asm 2: eor >t1=r12,<r1=r2,<r1=r2,ROR #5
eor r12,r2,r2,ROR #5

# qhasm: t1 ^= (r1 >>> 19)
# asm 1: eor >t1=int32#13,<t1=int32#13,<r1=int32#3,ROR #19
# asm 2: eor >t1=r12,<t1=r12,<r1=r2,ROR #19
eor r12,r12,r2,ROR #19

# qhasm: t2 = mem32[input_3 + 108]
# asm 1: ldr >t2=int32#14,[<input_3=int32#4,#108]
# asm 2: ldr >t2=r14,[<input_3=r3,#108]
ldr r14,[r3,#108]

# qhasm: t0 += w11
# asm 1: add >t0=int32#11,<t0=int32#12,<w11=int32#11
# asm 2: add >t0=r10,<t0=r11,<w11=r10
add r10,r11,r10

# qhasm: t0 += (t1 >>> 6)
# asm 1: add >t0=int32#11,<t0=int32#11,<t1=int32#13,ROR #6
# asm 2: add >t0=r10,<t0=r10,<t1=r12,ROR #6
add r10,r10,r12,ROR #6

# qhasm: t0 += t2
# asm 1: add >t0=int32#11,<t0=int32#11,<t2=int32#14
# asm 2: add >t0=r10,<t0=r10,<t2=r14
add r10,r10,r14

# qhasm: r4 += t0
# asm 1: add >r4=int32#7,<r4=int32#7,<t0=int32#11
# asm 2: add >r4=r6,<r4=r6,<t0=r10
add r6,r6,r10

# qhasm: r0 += r4
# asm 1: add >r0=int32#1,<r0=int32#1,<r4=int32#7
# asm 2: add >r0=r0,<r0=r0,<r4=r6
add r0,r0,r6

# qhasm: t0 = r5 & r6
# asm 1: and >t0=int32#11,<r5=int32#8,<r6=int32#9
# asm 2: and >t0=r10,<r5=r7,<r6=r8
and r10,r7,r8

# qhasm: t2 = r5 & r7
# asm 1: and >t2=int32#12,<r5=int32#8,<r7=int32#10
# asm 2: and >t2=r11,<r5=r7,<r7=r9
and r11,r7,r9

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t2 = r6 & r7
# asm 1: and >t2=int32#12,<r6=int32#9,<r7=int32#10
# asm 2: and >t2=r11,<r6=r8,<r7=r9
and r11,r8,r9

# qhasm: t3 = r5 ^ (r5 >>> 11)
# asm 1: eor >t3=int32#13,<r5=int32#8,<r5=int32#8,ROR #11
# asm 2: eor >t3=r12,<r5=r7,<r5=r7,ROR #11
eor r12,r7,r7,ROR #11

# qhasm: t3 ^= (r5 >>> 20)
# asm 1: eor >t3=int32#13,<t3=int32#13,<r5=int32#8,ROR #20
# asm 2: eor >t3=r12,<t3=r12,<r5=r7,ROR #20
eor r12,r12,r7,ROR #20

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t0 += (t3 >>> 2)
# asm 1: add >t0=int32#11,<t0=int32#11,<t3=int32#13,ROR #2
# asm 2: add >t0=r10,<t0=r10,<t3=r12,ROR #2
add r10,r10,r12,ROR #2

# qhasm: r4 += t0
# asm 1: add >r4=int32#7,<r4=int32#7,<t0=int32#11
# asm 2: add >r4=r6,<r4=r6,<t0=r10
add r6,r6,r10

# qhasm: w12 = w12_stack
# asm 1: ldr >w12=int32#11,<w12_stack=stack32#32
# asm 2: ldr >w12=r10,<w12_stack=[sp,#124]
ldr r10,[sp,#124]

# qhasm: t0 = r0 & r1
# asm 1: and >t0=int32#12,<r0=int32#1,<r1=int32#3
# asm 2: and >t0=r11,<r0=r0,<r1=r2
and r11,r0,r2

# qhasm: t1 = r2 & ~r0
# asm 1: bic >t1=int32#13,<r2=int32#5,<r0=int32#1
# asm 2: bic >t1=r12,<r2=r4,<r0=r0
bic r12,r4,r0

# qhasm: t0 ^= t1
# asm 1: eor >t0=int32#12,<t0=int32#12,<t1=int32#13
# asm 2: eor >t0=r11,<t0=r11,<t1=r12
eor r11,r11,r12

# qhasm: t1 = r0 ^ (r0 >>> 5)
# asm 1: eor >t1=int32#13,<r0=int32#1,<r0=int32#1,ROR #5
# asm 2: eor >t1=r12,<r0=r0,<r0=r0,ROR #5
eor r12,r0,r0,ROR #5

# qhasm: t1 ^= (r0 >>> 19)
# asm 1: eor >t1=int32#13,<t1=int32#13,<r0=int32#1,ROR #19
# asm 2: eor >t1=r12,<t1=r12,<r0=r0,ROR #19
eor r12,r12,r0,ROR #19

# qhasm: t2 = mem32[input_3 + 112]
# asm 1: ldr >t2=int32#14,[<input_3=int32#4,#112]
# asm 2: ldr >t2=r14,[<input_3=r3,#112]
ldr r14,[r3,#112]

# qhasm: t0 += w12
# asm 1: add >t0=int32#11,<t0=int32#12,<w12=int32#11
# asm 2: add >t0=r10,<t0=r11,<w12=r10
add r10,r11,r10

# qhasm: t0 += (t1 >>> 6)
# asm 1: add >t0=int32#11,<t0=int32#11,<t1=int32#13,ROR #6
# asm 2: add >t0=r10,<t0=r10,<t1=r12,ROR #6
add r10,r10,r12,ROR #6

# qhasm: t0 += t2
# asm 1: add >t0=int32#11,<t0=int32#11,<t2=int32#14
# asm 2: add >t0=r10,<t0=r10,<t2=r14
add r10,r10,r14

# qhasm: r3 += t0
# asm 1: add >r3=int32#6,<r3=int32#6,<t0=int32#11
# asm 2: add >r3=r5,<r3=r5,<t0=r10
add r5,r5,r10

# qhasm: r7 += r3
# asm 1: add >r7=int32#10,<r7=int32#10,<r3=int32#6
# asm 2: add >r7=r9,<r7=r9,<r3=r5
add r9,r9,r5

# qhasm: t0 = r4 & r5
# asm 1: and >t0=int32#11,<r4=int32#7,<r5=int32#8
# asm 2: and >t0=r10,<r4=r6,<r5=r7
and r10,r6,r7

# qhasm: t2 = r4 & r6
# asm 1: and >t2=int32#12,<r4=int32#7,<r6=int32#9
# asm 2: and >t2=r11,<r4=r6,<r6=r8
and r11,r6,r8

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t2 = r5 & r6
# asm 1: and >t2=int32#12,<r5=int32#8,<r6=int32#9
# asm 2: and >t2=r11,<r5=r7,<r6=r8
and r11,r7,r8

# qhasm: t3 = r4 ^ (r4 >>> 11)
# asm 1: eor >t3=int32#13,<r4=int32#7,<r4=int32#7,ROR #11
# asm 2: eor >t3=r12,<r4=r6,<r4=r6,ROR #11
eor r12,r6,r6,ROR #11

# qhasm: t3 ^= (r4 >>> 20)
# asm 1: eor >t3=int32#13,<t3=int32#13,<r4=int32#7,ROR #20
# asm 2: eor >t3=r12,<t3=r12,<r4=r6,ROR #20
eor r12,r12,r6,ROR #20

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t0 += (t3 >>> 2)
# asm 1: add >t0=int32#11,<t0=int32#11,<t3=int32#13,ROR #2
# asm 2: add >t0=r10,<t0=r10,<t3=r12,ROR #2
add r10,r10,r12,ROR #2

# qhasm: r3 += t0
# asm 1: add >r3=int32#6,<r3=int32#6,<t0=int32#11
# asm 2: add >r3=r5,<r3=r5,<t0=r10
add r5,r5,r10

# qhasm: w13 = w13_stack
# asm 1: ldr >w13=int32#11,<w13_stack=stack32#33
# asm 2: ldr >w13=r10,<w13_stack=[sp,#128]
ldr r10,[sp,#128]

# qhasm: t0 = r7 & r0
# asm 1: and >t0=int32#12,<r7=int32#10,<r0=int32#1
# asm 2: and >t0=r11,<r7=r9,<r0=r0
and r11,r9,r0

# qhasm: t1 = r1 & ~r7
# asm 1: bic >t1=int32#13,<r1=int32#3,<r7=int32#10
# asm 2: bic >t1=r12,<r1=r2,<r7=r9
bic r12,r2,r9

# qhasm: t0 ^= t1
# asm 1: eor >t0=int32#12,<t0=int32#12,<t1=int32#13
# asm 2: eor >t0=r11,<t0=r11,<t1=r12
eor r11,r11,r12

# qhasm: t1 = r7 ^ (r7 >>> 5)
# asm 1: eor >t1=int32#13,<r7=int32#10,<r7=int32#10,ROR #5
# asm 2: eor >t1=r12,<r7=r9,<r7=r9,ROR #5
eor r12,r9,r9,ROR #5

# qhasm: t1 ^= (r7 >>> 19)
# asm 1: eor >t1=int32#13,<t1=int32#13,<r7=int32#10,ROR #19
# asm 2: eor >t1=r12,<t1=r12,<r7=r9,ROR #19
eor r12,r12,r9,ROR #19

# qhasm: t2 = mem32[input_3 + 116]
# asm 1: ldr >t2=int32#14,[<input_3=int32#4,#116]
# asm 2: ldr >t2=r14,[<input_3=r3,#116]
ldr r14,[r3,#116]

# qhasm: t0 += w13
# asm 1: add >t0=int32#11,<t0=int32#12,<w13=int32#11
# asm 2: add >t0=r10,<t0=r11,<w13=r10
add r10,r11,r10

# qhasm: t0 += (t1 >>> 6)
# asm 1: add >t0=int32#11,<t0=int32#11,<t1=int32#13,ROR #6
# asm 2: add >t0=r10,<t0=r10,<t1=r12,ROR #6
add r10,r10,r12,ROR #6

# qhasm: t0 += t2
# asm 1: add >t0=int32#11,<t0=int32#11,<t2=int32#14
# asm 2: add >t0=r10,<t0=r10,<t2=r14
add r10,r10,r14

# qhasm: r2 += t0
# asm 1: add >r2=int32#5,<r2=int32#5,<t0=int32#11
# asm 2: add >r2=r4,<r2=r4,<t0=r10
add r4,r4,r10

# qhasm: r6 += r2
# asm 1: add >r6=int32#9,<r6=int32#9,<r2=int32#5
# asm 2: add >r6=r8,<r6=r8,<r2=r4
add r8,r8,r4

# qhasm: t0 = r3 & r4
# asm 1: and >t0=int32#11,<r3=int32#6,<r4=int32#7
# asm 2: and >t0=r10,<r3=r5,<r4=r6
and r10,r5,r6

# qhasm: t2 = r3 & r5
# asm 1: and >t2=int32#12,<r3=int32#6,<r5=int32#8
# asm 2: and >t2=r11,<r3=r5,<r5=r7
and r11,r5,r7

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t2 = r4 & r5
# asm 1: and >t2=int32#12,<r4=int32#7,<r5=int32#8
# asm 2: and >t2=r11,<r4=r6,<r5=r7
and r11,r6,r7

# qhasm: t3 = r3 ^ (r3 >>> 11)
# asm 1: eor >t3=int32#13,<r3=int32#6,<r3=int32#6,ROR #11
# asm 2: eor >t3=r12,<r3=r5,<r3=r5,ROR #11
eor r12,r5,r5,ROR #11

# qhasm: t3 ^= (r3 >>> 20)
# asm 1: eor >t3=int32#13,<t3=int32#13,<r3=int32#6,ROR #20
# asm 2: eor >t3=r12,<t3=r12,<r3=r5,ROR #20
eor r12,r12,r5,ROR #20

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t0 += (t3 >>> 2)
# asm 1: add >t0=int32#11,<t0=int32#11,<t3=int32#13,ROR #2
# asm 2: add >t0=r10,<t0=r10,<t3=r12,ROR #2
add r10,r10,r12,ROR #2

# qhasm: r2 += t0
# asm 1: add >r2=int32#5,<r2=int32#5,<t0=int32#11
# asm 2: add >r2=r4,<r2=r4,<t0=r10
add r4,r4,r10

# qhasm: w14 = w14_stack
# asm 1: ldr >w14=int32#11,<w14_stack=stack32#34
# asm 2: ldr >w14=r10,<w14_stack=[sp,#132]
ldr r10,[sp,#132]

# qhasm: t0 = r6 & r7
# asm 1: and >t0=int32#12,<r6=int32#9,<r7=int32#10
# asm 2: and >t0=r11,<r6=r8,<r7=r9
and r11,r8,r9

# qhasm: t1 = r0 & ~r6
# asm 1: bic >t1=int32#13,<r0=int32#1,<r6=int32#9
# asm 2: bic >t1=r12,<r0=r0,<r6=r8
bic r12,r0,r8

# qhasm: t0 ^= t1
# asm 1: eor >t0=int32#12,<t0=int32#12,<t1=int32#13
# asm 2: eor >t0=r11,<t0=r11,<t1=r12
eor r11,r11,r12

# qhasm: t1 = r6 ^ (r6 >>> 5)
# asm 1: eor >t1=int32#13,<r6=int32#9,<r6=int32#9,ROR #5
# asm 2: eor >t1=r12,<r6=r8,<r6=r8,ROR #5
eor r12,r8,r8,ROR #5

# qhasm: t1 ^= (r6 >>> 19)
# asm 1: eor >t1=int32#13,<t1=int32#13,<r6=int32#9,ROR #19
# asm 2: eor >t1=r12,<t1=r12,<r6=r8,ROR #19
eor r12,r12,r8,ROR #19

# qhasm: t2 = mem32[input_3 + 120]
# asm 1: ldr >t2=int32#14,[<input_3=int32#4,#120]
# asm 2: ldr >t2=r14,[<input_3=r3,#120]
ldr r14,[r3,#120]

# qhasm: t0 += w14
# asm 1: add >t0=int32#11,<t0=int32#12,<w14=int32#11
# asm 2: add >t0=r10,<t0=r11,<w14=r10
add r10,r11,r10

# qhasm: t0 += (t1 >>> 6)
# asm 1: add >t0=int32#11,<t0=int32#11,<t1=int32#13,ROR #6
# asm 2: add >t0=r10,<t0=r10,<t1=r12,ROR #6
add r10,r10,r12,ROR #6

# qhasm: t0 += t2
# asm 1: add >t0=int32#11,<t0=int32#11,<t2=int32#14
# asm 2: add >t0=r10,<t0=r10,<t2=r14
add r10,r10,r14

# qhasm: r1 += t0
# asm 1: add >r1=int32#3,<r1=int32#3,<t0=int32#11
# asm 2: add >r1=r2,<r1=r2,<t0=r10
add r2,r2,r10

# qhasm: r5 += r1
# asm 1: add >r5=int32#8,<r5=int32#8,<r1=int32#3
# asm 2: add >r5=r7,<r5=r7,<r1=r2
add r7,r7,r2

# qhasm: t0 = r2 & r3
# asm 1: and >t0=int32#11,<r2=int32#5,<r3=int32#6
# asm 2: and >t0=r10,<r2=r4,<r3=r5
and r10,r4,r5

# qhasm: t2 = r2 & r4
# asm 1: and >t2=int32#12,<r2=int32#5,<r4=int32#7
# asm 2: and >t2=r11,<r2=r4,<r4=r6
and r11,r4,r6

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t2 = r3 & r4
# asm 1: and >t2=int32#12,<r3=int32#6,<r4=int32#7
# asm 2: and >t2=r11,<r3=r5,<r4=r6
and r11,r5,r6

# qhasm: t3 = r2 ^ (r2 >>> 11)
# asm 1: eor >t3=int32#13,<r2=int32#5,<r2=int32#5,ROR #11
# asm 2: eor >t3=r12,<r2=r4,<r2=r4,ROR #11
eor r12,r4,r4,ROR #11

# qhasm: t3 ^= (r2 >>> 20)
# asm 1: eor >t3=int32#13,<t3=int32#13,<r2=int32#5,ROR #20
# asm 2: eor >t3=r12,<t3=r12,<r2=r4,ROR #20
eor r12,r12,r4,ROR #20

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t0 += (t3 >>> 2)
# asm 1: add >t0=int32#11,<t0=int32#11,<t3=int32#13,ROR #2
# asm 2: add >t0=r10,<t0=r10,<t3=r12,ROR #2
add r10,r10,r12,ROR #2

# qhasm: r1 += t0
# asm 1: add >r1=int32#3,<r1=int32#3,<t0=int32#11
# asm 2: add >r1=r2,<r1=r2,<t0=r10
add r2,r2,r10

# qhasm: w15 = w15_stack
# asm 1: ldr >w15=int32#11,<w15_stack=stack32#35
# asm 2: ldr >w15=r10,<w15_stack=[sp,#136]
ldr r10,[sp,#136]

# qhasm: t0 = r5 & r6
# asm 1: and >t0=int32#12,<r5=int32#8,<r6=int32#9
# asm 2: and >t0=r11,<r5=r7,<r6=r8
and r11,r7,r8

# qhasm: t1 = r7 & ~r5
# asm 1: bic >t1=int32#13,<r7=int32#10,<r5=int32#8
# asm 2: bic >t1=r12,<r7=r9,<r5=r7
bic r12,r9,r7

# qhasm: t0 ^= t1
# asm 1: eor >t0=int32#12,<t0=int32#12,<t1=int32#13
# asm 2: eor >t0=r11,<t0=r11,<t1=r12
eor r11,r11,r12

# qhasm: t1 = r5 ^ (r5 >>> 5)
# asm 1: eor >t1=int32#13,<r5=int32#8,<r5=int32#8,ROR #5
# asm 2: eor >t1=r12,<r5=r7,<r5=r7,ROR #5
eor r12,r7,r7,ROR #5

# qhasm: t1 ^= (r5 >>> 19)
# asm 1: eor >t1=int32#13,<t1=int32#13,<r5=int32#8,ROR #19
# asm 2: eor >t1=r12,<t1=r12,<r5=r7,ROR #19
eor r12,r12,r7,ROR #19

# qhasm: t2 = mem32[input_3 + 124]
# asm 1: ldr >t2=int32#14,[<input_3=int32#4,#124]
# asm 2: ldr >t2=r14,[<input_3=r3,#124]
ldr r14,[r3,#124]

# qhasm: t0 += w15
# asm 1: add >t0=int32#11,<t0=int32#12,<w15=int32#11
# asm 2: add >t0=r10,<t0=r11,<w15=r10
add r10,r11,r10

# qhasm: t0 += (t1 >>> 6)
# asm 1: add >t0=int32#11,<t0=int32#11,<t1=int32#13,ROR #6
# asm 2: add >t0=r10,<t0=r10,<t1=r12,ROR #6
add r10,r10,r12,ROR #6

# qhasm: t0 += t2
# asm 1: add >t0=int32#11,<t0=int32#11,<t2=int32#14
# asm 2: add >t0=r10,<t0=r10,<t2=r14
add r10,r10,r14

# qhasm: r0 += t0
# asm 1: add >r0=int32#1,<r0=int32#1,<t0=int32#11
# asm 2: add >r0=r0,<r0=r0,<t0=r10
add r0,r0,r10

# qhasm: r4 += r0
# asm 1: add >r4=int32#7,<r4=int32#7,<r0=int32#1
# asm 2: add >r4=r6,<r4=r6,<r0=r0
add r6,r6,r0

# qhasm: t0 = r1 & r2
# asm 1: and >t0=int32#11,<r1=int32#3,<r2=int32#5
# asm 2: and >t0=r10,<r1=r2,<r2=r4
and r10,r2,r4

# qhasm: t2 = r1 & r3
# asm 1: and >t2=int32#12,<r1=int32#3,<r3=int32#6
# asm 2: and >t2=r11,<r1=r2,<r3=r5
and r11,r2,r5

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t2 = r2 & r3
# asm 1: and >t2=int32#12,<r2=int32#5,<r3=int32#6
# asm 2: and >t2=r11,<r2=r4,<r3=r5
and r11,r4,r5

# qhasm: t3 = r1 ^ (r1 >>> 11)
# asm 1: eor >t3=int32#13,<r1=int32#3,<r1=int32#3,ROR #11
# asm 2: eor >t3=r12,<r1=r2,<r1=r2,ROR #11
eor r12,r2,r2,ROR #11

# qhasm: t3 ^= (r1 >>> 20)
# asm 1: eor >t3=int32#13,<t3=int32#13,<r1=int32#3,ROR #20
# asm 2: eor >t3=r12,<t3=r12,<r1=r2,ROR #20
eor r12,r12,r2,ROR #20

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t0 += (t3 >>> 2)
# asm 1: add >t0=int32#11,<t0=int32#11,<t3=int32#13,ROR #2
# asm 2: add >t0=r10,<t0=r10,<t3=r12,ROR #2
add r10,r10,r12,ROR #2

# qhasm: r0 += t0
# asm 1: add >r0=int32#1,<r0=int32#1,<t0=int32#11
# asm 2: add >r0=r0,<r0=r0,<t0=r10
add r0,r0,r10

# qhasm: r0_stack = r0
# asm 1: str <r0=int32#1,>r0_stack=stack32#36
# asm 2: str <r0=r0,>r0_stack=[sp,#140]
str r0,[sp,#140]

# qhasm: r1_stack = r1
# asm 1: str <r1=int32#3,>r1_stack=stack32#37
# asm 2: str <r1=r2,>r1_stack=[sp,#144]
str r2,[sp,#144]

# qhasm: r2_stack = r2
# asm 1: str <r2=int32#5,>r2_stack=stack32#38
# asm 2: str <r2=r4,>r2_stack=[sp,#148]
str r4,[sp,#148]

# qhasm: r3_stack = r3
# asm 1: str <r3=int32#6,>r3_stack=stack32#39
# asm 2: str <r3=r5,>r3_stack=[sp,#152]
str r5,[sp,#152]

# qhasm: r4_stack = r4
# asm 1: str <r4=int32#7,>r4_stack=stack32#40
# asm 2: str <r4=r6,>r4_stack=[sp,#156]
str r6,[sp,#156]

# qhasm: r5_stack = r5
# asm 1: str <r5=int32#8,>r5_stack=stack32#41
# asm 2: str <r5=r7,>r5_stack=[sp,#160]
str r7,[sp,#160]

# qhasm: r6_stack = r6
# asm 1: str <r6=int32#9,>r6_stack=stack32#42
# asm 2: str <r6=r8,>r6_stack=[sp,#164]
str r8,[sp,#164]

# qhasm: r7_stack = r7
# asm 1: str <r7=int32#10,>r7_stack=stack32#43
# asm 2: str <r7=r9,>r7_stack=[sp,#168]
str r9,[sp,#168]

# qhasm: w0 = w0_stack
# asm 1: ldr >w0=int32#1,<w0_stack=stack32#25
# asm 2: ldr >w0=r0,<w0_stack=[sp,#96]
ldr r0,[sp,#96]

# qhasm: w1 = w1_stack
# asm 1: ldr >w1=int32#3,<w1_stack=stack32#21
# asm 2: ldr >w1=r2,<w1_stack=[sp,#80]
ldr r2,[sp,#80]

# qhasm: w2 = w2_stack
# asm 1: ldr >w2=int32#5,<w2_stack=stack32#22
# asm 2: ldr >w2=r4,<w2_stack=[sp,#84]
ldr r4,[sp,#84]

# qhasm: w3 = w3_stack
# asm 1: ldr >w3=int32#6,<w3_stack=stack32#23
# asm 2: ldr >w3=r5,<w3_stack=[sp,#88]
ldr r5,[sp,#88]

# qhasm: w4 = w4_stack
# asm 1: ldr >w4=int32#7,<w4_stack=stack32#24
# asm 2: ldr >w4=r6,<w4_stack=[sp,#92]
ldr r6,[sp,#92]

# qhasm: w9 = w9_stack
# asm 1: ldr >w9=int32#8,<w9_stack=stack32#20
# asm 2: ldr >w9=r7,<w9_stack=[sp,#76]
ldr r7,[sp,#76]

# qhasm: w10 = w10_stack
# asm 1: ldr >w10=int32#9,<w10_stack=stack32#30
# asm 2: ldr >w10=r8,<w10_stack=[sp,#116]
ldr r8,[sp,#116]

# qhasm: w11 = w11_stack
# asm 1: ldr >w11=int32#10,<w11_stack=stack32#31
# asm 2: ldr >w11=r9,<w11_stack=[sp,#120]
ldr r9,[sp,#120]

# qhasm: w12 = w12_stack
# asm 1: ldr >w12=int32#11,<w12_stack=stack32#32
# asm 2: ldr >w12=r10,<w12_stack=[sp,#124]
ldr r10,[sp,#124]

# qhasm: w14 = w14_stack
# asm 1: ldr >w14=int32#12,<w14_stack=stack32#34
# asm 2: ldr >w14=r11,<w14_stack=[sp,#132]
ldr r11,[sp,#132]

# qhasm: w15 = w15_stack
# asm 1: ldr >w15=int32#13,<w15_stack=stack32#35
# asm 2: ldr >w15=r12,<w15_stack=[sp,#136]
ldr r12,[sp,#136]

# qhasm: w0 += w9
# asm 1: add >w0=int32#1,<w0=int32#1,<w9=int32#8
# asm 2: add >w0=r0,<w0=r0,<w9=r7
add r0,r0,r7

# qhasm: t0 = (w14 >>> 17)
# asm 1: mov >t0=int32#14,<w14=int32#12,ROR #17
# asm 2: mov >t0=r14,<w14=r11,ROR #17
mov r14,r11,ROR #17

# qhasm: t0 ^= (w14 >>> 19)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w14=int32#12,ROR #19
# asm 2: eor >t0=r14,<t0=r14,<w14=r11,ROR #19
eor r14,r14,r11,ROR #19

# qhasm: t0 ^= (w14 unsigned>> 10)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w14=int32#12,LSR #10
# asm 2: eor >t0=r14,<t0=r14,<w14=r11,LSR #10
eor r14,r14,r11,LSR #10

# qhasm: w0 += t0
# asm 1: add >w0=int32#1,<w0=int32#1,<t0=int32#14
# asm 2: add >w0=r0,<w0=r0,<t0=r14
add r0,r0,r14

# qhasm: t0 = (w1 >>> 7)
# asm 1: mov >t0=int32#14,<w1=int32#3,ROR #7
# asm 2: mov >t0=r14,<w1=r2,ROR #7
mov r14,r2,ROR #7

# qhasm: t0 ^= (w1 >>> 18)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w1=int32#3,ROR #18
# asm 2: eor >t0=r14,<t0=r14,<w1=r2,ROR #18
eor r14,r14,r2,ROR #18

# qhasm: t0 ^= (w1 unsigned>> 3)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w1=int32#3,LSR #3
# asm 2: eor >t0=r14,<t0=r14,<w1=r2,LSR #3
eor r14,r14,r2,LSR #3

# qhasm: w0 += t0
# asm 1: add >w0=int32#1,<w0=int32#1,<t0=int32#14
# asm 2: add >w0=r0,<w0=r0,<t0=r14
add r0,r0,r14

# qhasm: w1 += w10
# asm 1: add >w1=int32#3,<w1=int32#3,<w10=int32#9
# asm 2: add >w1=r2,<w1=r2,<w10=r8
add r2,r2,r8

# qhasm: t0 = (w15 >>> 17)
# asm 1: mov >t0=int32#14,<w15=int32#13,ROR #17
# asm 2: mov >t0=r14,<w15=r12,ROR #17
mov r14,r12,ROR #17

# qhasm: t0 ^= (w15 >>> 19)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w15=int32#13,ROR #19
# asm 2: eor >t0=r14,<t0=r14,<w15=r12,ROR #19
eor r14,r14,r12,ROR #19

# qhasm: t0 ^= (w15 unsigned>> 10)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w15=int32#13,LSR #10
# asm 2: eor >t0=r14,<t0=r14,<w15=r12,LSR #10
eor r14,r14,r12,LSR #10

# qhasm: w1 += t0
# asm 1: add >w1=int32#3,<w1=int32#3,<t0=int32#14
# asm 2: add >w1=r2,<w1=r2,<t0=r14
add r2,r2,r14

# qhasm: t0 = (w2 >>> 7)
# asm 1: mov >t0=int32#14,<w2=int32#5,ROR #7
# asm 2: mov >t0=r14,<w2=r4,ROR #7
mov r14,r4,ROR #7

# qhasm: t0 ^= (w2 >>> 18)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w2=int32#5,ROR #18
# asm 2: eor >t0=r14,<t0=r14,<w2=r4,ROR #18
eor r14,r14,r4,ROR #18

# qhasm: t0 ^= (w2 unsigned>> 3)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w2=int32#5,LSR #3
# asm 2: eor >t0=r14,<t0=r14,<w2=r4,LSR #3
eor r14,r14,r4,LSR #3

# qhasm: w1 += t0
# asm 1: add >w1=int32#3,<w1=int32#3,<t0=int32#14
# asm 2: add >w1=r2,<w1=r2,<t0=r14
add r2,r2,r14

# qhasm: w2 += w11
# asm 1: add >w2=int32#5,<w2=int32#5,<w11=int32#10
# asm 2: add >w2=r4,<w2=r4,<w11=r9
add r4,r4,r9

# qhasm: t0 = (w0 >>> 17)
# asm 1: mov >t0=int32#14,<w0=int32#1,ROR #17
# asm 2: mov >t0=r14,<w0=r0,ROR #17
mov r14,r0,ROR #17

# qhasm: t0 ^= (w0 >>> 19)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w0=int32#1,ROR #19
# asm 2: eor >t0=r14,<t0=r14,<w0=r0,ROR #19
eor r14,r14,r0,ROR #19

# qhasm: t0 ^= (w0 unsigned>> 10)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w0=int32#1,LSR #10
# asm 2: eor >t0=r14,<t0=r14,<w0=r0,LSR #10
eor r14,r14,r0,LSR #10

# qhasm: w2 += t0
# asm 1: add >w2=int32#5,<w2=int32#5,<t0=int32#14
# asm 2: add >w2=r4,<w2=r4,<t0=r14
add r4,r4,r14

# qhasm: t0 = (w3 >>> 7)
# asm 1: mov >t0=int32#14,<w3=int32#6,ROR #7
# asm 2: mov >t0=r14,<w3=r5,ROR #7
mov r14,r5,ROR #7

# qhasm: t0 ^= (w3 >>> 18)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w3=int32#6,ROR #18
# asm 2: eor >t0=r14,<t0=r14,<w3=r5,ROR #18
eor r14,r14,r5,ROR #18

# qhasm: t0 ^= (w3 unsigned>> 3)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w3=int32#6,LSR #3
# asm 2: eor >t0=r14,<t0=r14,<w3=r5,LSR #3
eor r14,r14,r5,LSR #3

# qhasm: w2 += t0
# asm 1: add >w2=int32#5,<w2=int32#5,<t0=int32#14
# asm 2: add >w2=r4,<w2=r4,<t0=r14
add r4,r4,r14

# qhasm: w3 += w12
# asm 1: add >w3=int32#6,<w3=int32#6,<w12=int32#11
# asm 2: add >w3=r5,<w3=r5,<w12=r10
add r5,r5,r10

# qhasm: t0 = (w1 >>> 17)
# asm 1: mov >t0=int32#14,<w1=int32#3,ROR #17
# asm 2: mov >t0=r14,<w1=r2,ROR #17
mov r14,r2,ROR #17

# qhasm: t0 ^= (w1 >>> 19)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w1=int32#3,ROR #19
# asm 2: eor >t0=r14,<t0=r14,<w1=r2,ROR #19
eor r14,r14,r2,ROR #19

# qhasm: t0 ^= (w1 unsigned>> 10)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w1=int32#3,LSR #10
# asm 2: eor >t0=r14,<t0=r14,<w1=r2,LSR #10
eor r14,r14,r2,LSR #10

# qhasm: w3 += t0
# asm 1: add >w3=int32#6,<w3=int32#6,<t0=int32#14
# asm 2: add >w3=r5,<w3=r5,<t0=r14
add r5,r5,r14

# qhasm: t0 = (w4 >>> 7)
# asm 1: mov >t0=int32#14,<w4=int32#7,ROR #7
# asm 2: mov >t0=r14,<w4=r6,ROR #7
mov r14,r6,ROR #7

# qhasm: t0 ^= (w4 >>> 18)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w4=int32#7,ROR #18
# asm 2: eor >t0=r14,<t0=r14,<w4=r6,ROR #18
eor r14,r14,r6,ROR #18

# qhasm: t0 ^= (w4 unsigned>> 3)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w4=int32#7,LSR #3
# asm 2: eor >t0=r14,<t0=r14,<w4=r6,LSR #3
eor r14,r14,r6,LSR #3

# qhasm: w3 += t0
# asm 1: add >w3=int32#6,<w3=int32#6,<t0=int32#14
# asm 2: add >w3=r5,<w3=r5,<t0=r14
add r5,r5,r14

# qhasm: w1_stack = w1
# asm 1: str <w1=int32#3,>w1_stack=stack32#20
# asm 2: str <w1=r2,>w1_stack=[sp,#76]
str r2,[sp,#76]

# qhasm: w9_stack = w9
# asm 1: str <w9=int32#8,>w9_stack=stack32#21
# asm 2: str <w9=r7,>w9_stack=[sp,#80]
str r7,[sp,#80]

# qhasm: w10_stack = w10
# asm 1: str <w10=int32#9,>w10_stack=stack32#22
# asm 2: str <w10=r8,>w10_stack=[sp,#84]
str r8,[sp,#84]

# qhasm: w11_stack = w11
# asm 1: str <w11=int32#10,>w11_stack=stack32#23
# asm 2: str <w11=r9,>w11_stack=[sp,#88]
str r9,[sp,#88]

# qhasm: w12_stack = w12
# asm 1: str <w12=int32#11,>w12_stack=stack32#24
# asm 2: str <w12=r10,>w12_stack=[sp,#92]
str r10,[sp,#92]

# qhasm: w13 = w13_stack
# asm 1: ldr >w13=int32#3,<w13_stack=stack32#33
# asm 2: ldr >w13=r2,<w13_stack=[sp,#128]
ldr r2,[sp,#128]

# qhasm: w5 = w5_stack
# asm 1: ldr >w5=int32#8,<w5_stack=stack32#26
# asm 2: ldr >w5=r7,<w5_stack=[sp,#100]
ldr r7,[sp,#100]

# qhasm: w6 = w6_stack
# asm 1: ldr >w6=int32#9,<w6_stack=stack32#27
# asm 2: ldr >w6=r8,<w6_stack=[sp,#104]
ldr r8,[sp,#104]

# qhasm: w7 = w7_stack
# asm 1: ldr >w7=int32#10,<w7_stack=stack32#28
# asm 2: ldr >w7=r9,<w7_stack=[sp,#108]
ldr r9,[sp,#108]

# qhasm: w8 = w8_stack
# asm 1: ldr >w8=int32#11,<w8_stack=stack32#29
# asm 2: ldr >w8=r10,<w8_stack=[sp,#112]
ldr r10,[sp,#112]

# qhasm: w4 += w13
# asm 1: add >w4=int32#7,<w4=int32#7,<w13=int32#3
# asm 2: add >w4=r6,<w4=r6,<w13=r2
add r6,r6,r2

# qhasm: t0 = (w2 >>> 17)
# asm 1: mov >t0=int32#14,<w2=int32#5,ROR #17
# asm 2: mov >t0=r14,<w2=r4,ROR #17
mov r14,r4,ROR #17

# qhasm: t0 ^= (w2 >>> 19)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w2=int32#5,ROR #19
# asm 2: eor >t0=r14,<t0=r14,<w2=r4,ROR #19
eor r14,r14,r4,ROR #19

# qhasm: t0 ^= (w2 unsigned>> 10)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w2=int32#5,LSR #10
# asm 2: eor >t0=r14,<t0=r14,<w2=r4,LSR #10
eor r14,r14,r4,LSR #10

# qhasm: w4 += t0
# asm 1: add >w4=int32#7,<w4=int32#7,<t0=int32#14
# asm 2: add >w4=r6,<w4=r6,<t0=r14
add r6,r6,r14

# qhasm: t0 = (w5 >>> 7)
# asm 1: mov >t0=int32#14,<w5=int32#8,ROR #7
# asm 2: mov >t0=r14,<w5=r7,ROR #7
mov r14,r7,ROR #7

# qhasm: t0 ^= (w5 >>> 18)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w5=int32#8,ROR #18
# asm 2: eor >t0=r14,<t0=r14,<w5=r7,ROR #18
eor r14,r14,r7,ROR #18

# qhasm: t0 ^= (w5 unsigned>> 3)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w5=int32#8,LSR #3
# asm 2: eor >t0=r14,<t0=r14,<w5=r7,LSR #3
eor r14,r14,r7,LSR #3

# qhasm: w4 += t0
# asm 1: add >w4=int32#7,<w4=int32#7,<t0=int32#14
# asm 2: add >w4=r6,<w4=r6,<t0=r14
add r6,r6,r14

# qhasm: w5 += w14
# asm 1: add >w5=int32#8,<w5=int32#8,<w14=int32#12
# asm 2: add >w5=r7,<w5=r7,<w14=r11
add r7,r7,r11

# qhasm: t0 = (w3 >>> 17)
# asm 1: mov >t0=int32#14,<w3=int32#6,ROR #17
# asm 2: mov >t0=r14,<w3=r5,ROR #17
mov r14,r5,ROR #17

# qhasm: t0 ^= (w3 >>> 19)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w3=int32#6,ROR #19
# asm 2: eor >t0=r14,<t0=r14,<w3=r5,ROR #19
eor r14,r14,r5,ROR #19

# qhasm: t0 ^= (w3 unsigned>> 10)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w3=int32#6,LSR #10
# asm 2: eor >t0=r14,<t0=r14,<w3=r5,LSR #10
eor r14,r14,r5,LSR #10

# qhasm: w5 += t0
# asm 1: add >w5=int32#8,<w5=int32#8,<t0=int32#14
# asm 2: add >w5=r7,<w5=r7,<t0=r14
add r7,r7,r14

# qhasm: t0 = (w6 >>> 7)
# asm 1: mov >t0=int32#14,<w6=int32#9,ROR #7
# asm 2: mov >t0=r14,<w6=r8,ROR #7
mov r14,r8,ROR #7

# qhasm: t0 ^= (w6 >>> 18)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w6=int32#9,ROR #18
# asm 2: eor >t0=r14,<t0=r14,<w6=r8,ROR #18
eor r14,r14,r8,ROR #18

# qhasm: t0 ^= (w6 unsigned>> 3)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w6=int32#9,LSR #3
# asm 2: eor >t0=r14,<t0=r14,<w6=r8,LSR #3
eor r14,r14,r8,LSR #3

# qhasm: w5 += t0
# asm 1: add >w5=int32#8,<w5=int32#8,<t0=int32#14
# asm 2: add >w5=r7,<w5=r7,<t0=r14
add r7,r7,r14

# qhasm: w6 += w15
# asm 1: add >w6=int32#9,<w6=int32#9,<w15=int32#13
# asm 2: add >w6=r8,<w6=r8,<w15=r12
add r8,r8,r12

# qhasm: t0 = (w4 >>> 17)
# asm 1: mov >t0=int32#14,<w4=int32#7,ROR #17
# asm 2: mov >t0=r14,<w4=r6,ROR #17
mov r14,r6,ROR #17

# qhasm: t0 ^= (w4 >>> 19)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w4=int32#7,ROR #19
# asm 2: eor >t0=r14,<t0=r14,<w4=r6,ROR #19
eor r14,r14,r6,ROR #19

# qhasm: t0 ^= (w4 unsigned>> 10)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w4=int32#7,LSR #10
# asm 2: eor >t0=r14,<t0=r14,<w4=r6,LSR #10
eor r14,r14,r6,LSR #10

# qhasm: w6 += t0
# asm 1: add >w6=int32#9,<w6=int32#9,<t0=int32#14
# asm 2: add >w6=r8,<w6=r8,<t0=r14
add r8,r8,r14

# qhasm: t0 = (w7 >>> 7)
# asm 1: mov >t0=int32#14,<w7=int32#10,ROR #7
# asm 2: mov >t0=r14,<w7=r9,ROR #7
mov r14,r9,ROR #7

# qhasm: t0 ^= (w7 >>> 18)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w7=int32#10,ROR #18
# asm 2: eor >t0=r14,<t0=r14,<w7=r9,ROR #18
eor r14,r14,r9,ROR #18

# qhasm: t0 ^= (w7 unsigned>> 3)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w7=int32#10,LSR #3
# asm 2: eor >t0=r14,<t0=r14,<w7=r9,LSR #3
eor r14,r14,r9,LSR #3

# qhasm: w6 += t0
# asm 1: add >w6=int32#9,<w6=int32#9,<t0=int32#14
# asm 2: add >w6=r8,<w6=r8,<t0=r14
add r8,r8,r14

# qhasm: w7 += w0
# asm 1: add >w7=int32#10,<w7=int32#10,<w0=int32#1
# asm 2: add >w7=r9,<w7=r9,<w0=r0
add r9,r9,r0

# qhasm: t0 = (w5 >>> 17)
# asm 1: mov >t0=int32#14,<w5=int32#8,ROR #17
# asm 2: mov >t0=r14,<w5=r7,ROR #17
mov r14,r7,ROR #17

# qhasm: t0 ^= (w5 >>> 19)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w5=int32#8,ROR #19
# asm 2: eor >t0=r14,<t0=r14,<w5=r7,ROR #19
eor r14,r14,r7,ROR #19

# qhasm: t0 ^= (w5 unsigned>> 10)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w5=int32#8,LSR #10
# asm 2: eor >t0=r14,<t0=r14,<w5=r7,LSR #10
eor r14,r14,r7,LSR #10

# qhasm: w7 += t0
# asm 1: add >w7=int32#10,<w7=int32#10,<t0=int32#14
# asm 2: add >w7=r9,<w7=r9,<t0=r14
add r9,r9,r14

# qhasm: t0 = (w8 >>> 7)
# asm 1: mov >t0=int32#14,<w8=int32#11,ROR #7
# asm 2: mov >t0=r14,<w8=r10,ROR #7
mov r14,r10,ROR #7

# qhasm: t0 ^= (w8 >>> 18)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w8=int32#11,ROR #18
# asm 2: eor >t0=r14,<t0=r14,<w8=r10,ROR #18
eor r14,r14,r10,ROR #18

# qhasm: t0 ^= (w8 unsigned>> 3)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w8=int32#11,LSR #3
# asm 2: eor >t0=r14,<t0=r14,<w8=r10,LSR #3
eor r14,r14,r10,LSR #3

# qhasm: w7 += t0
# asm 1: add >w7=int32#10,<w7=int32#10,<t0=int32#14
# asm 2: add >w7=r9,<w7=r9,<t0=r14
add r9,r9,r14

# qhasm: w5_stack = w5
# asm 1: str <w5=int32#8,>w5_stack=stack32#25
# asm 2: str <w5=r7,>w5_stack=[sp,#96]
str r7,[sp,#96]

# qhasm: w13_stack = w13
# asm 1: str <w13=int32#3,>w13_stack=stack32#26
# asm 2: str <w13=r2,>w13_stack=[sp,#100]
str r2,[sp,#100]

# qhasm: w14_stack = w14
# asm 1: str <w14=int32#12,>w14_stack=stack32#27
# asm 2: str <w14=r11,>w14_stack=[sp,#104]
str r11,[sp,#104]

# qhasm: w15_stack = w15
# asm 1: str <w15=int32#13,>w15_stack=stack32#28
# asm 2: str <w15=r12,>w15_stack=[sp,#108]
str r12,[sp,#108]

# qhasm: w0_stack = w0
# asm 1: str <w0=int32#1,>w0_stack=stack32#29
# asm 2: str <w0=r0,>w0_stack=[sp,#112]
str r0,[sp,#112]

# qhasm: w1 = w1_stack
# asm 1: ldr >w1=int32#1,<w1_stack=stack32#20
# asm 2: ldr >w1=r0,<w1_stack=[sp,#76]
ldr r0,[sp,#76]

# qhasm: w9 = w9_stack
# asm 1: ldr >w9=int32#3,<w9_stack=stack32#21
# asm 2: ldr >w9=r2,<w9_stack=[sp,#80]
ldr r2,[sp,#80]

# qhasm: w10 = w10_stack
# asm 1: ldr >w10=int32#8,<w10_stack=stack32#22
# asm 2: ldr >w10=r7,<w10_stack=[sp,#84]
ldr r7,[sp,#84]

# qhasm: w11 = w11_stack
# asm 1: ldr >w11=int32#12,<w11_stack=stack32#23
# asm 2: ldr >w11=r11,<w11_stack=[sp,#88]
ldr r11,[sp,#88]

# qhasm: w12 = w12_stack
# asm 1: ldr >w12=int32#13,<w12_stack=stack32#24
# asm 2: ldr >w12=r12,<w12_stack=[sp,#92]
ldr r12,[sp,#92]

# qhasm: w8 += w1
# asm 1: add >w8=int32#11,<w8=int32#11,<w1=int32#1
# asm 2: add >w8=r10,<w8=r10,<w1=r0
add r10,r10,r0

# qhasm: t0 = (w6 >>> 17)
# asm 1: mov >t0=int32#14,<w6=int32#9,ROR #17
# asm 2: mov >t0=r14,<w6=r8,ROR #17
mov r14,r8,ROR #17

# qhasm: t0 ^= (w6 >>> 19)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w6=int32#9,ROR #19
# asm 2: eor >t0=r14,<t0=r14,<w6=r8,ROR #19
eor r14,r14,r8,ROR #19

# qhasm: t0 ^= (w6 unsigned>> 10)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w6=int32#9,LSR #10
# asm 2: eor >t0=r14,<t0=r14,<w6=r8,LSR #10
eor r14,r14,r8,LSR #10

# qhasm: w8 += t0
# asm 1: add >w8=int32#11,<w8=int32#11,<t0=int32#14
# asm 2: add >w8=r10,<w8=r10,<t0=r14
add r10,r10,r14

# qhasm: t0 = (w9 >>> 7)
# asm 1: mov >t0=int32#14,<w9=int32#3,ROR #7
# asm 2: mov >t0=r14,<w9=r2,ROR #7
mov r14,r2,ROR #7

# qhasm: t0 ^= (w9 >>> 18)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w9=int32#3,ROR #18
# asm 2: eor >t0=r14,<t0=r14,<w9=r2,ROR #18
eor r14,r14,r2,ROR #18

# qhasm: t0 ^= (w9 unsigned>> 3)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w9=int32#3,LSR #3
# asm 2: eor >t0=r14,<t0=r14,<w9=r2,LSR #3
eor r14,r14,r2,LSR #3

# qhasm: w8 += t0
# asm 1: add >w8=int32#11,<w8=int32#11,<t0=int32#14
# asm 2: add >w8=r10,<w8=r10,<t0=r14
add r10,r10,r14

# qhasm: w9 += w2
# asm 1: add >w9=int32#3,<w9=int32#3,<w2=int32#5
# asm 2: add >w9=r2,<w9=r2,<w2=r4
add r2,r2,r4

# qhasm: t0 = (w7 >>> 17)
# asm 1: mov >t0=int32#14,<w7=int32#10,ROR #17
# asm 2: mov >t0=r14,<w7=r9,ROR #17
mov r14,r9,ROR #17

# qhasm: t0 ^= (w7 >>> 19)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w7=int32#10,ROR #19
# asm 2: eor >t0=r14,<t0=r14,<w7=r9,ROR #19
eor r14,r14,r9,ROR #19

# qhasm: t0 ^= (w7 unsigned>> 10)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w7=int32#10,LSR #10
# asm 2: eor >t0=r14,<t0=r14,<w7=r9,LSR #10
eor r14,r14,r9,LSR #10

# qhasm: w9 += t0
# asm 1: add >w9=int32#3,<w9=int32#3,<t0=int32#14
# asm 2: add >w9=r2,<w9=r2,<t0=r14
add r2,r2,r14

# qhasm: t0 = (w10 >>> 7)
# asm 1: mov >t0=int32#14,<w10=int32#8,ROR #7
# asm 2: mov >t0=r14,<w10=r7,ROR #7
mov r14,r7,ROR #7

# qhasm: t0 ^= (w10 >>> 18)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w10=int32#8,ROR #18
# asm 2: eor >t0=r14,<t0=r14,<w10=r7,ROR #18
eor r14,r14,r7,ROR #18

# qhasm: t0 ^= (w10 unsigned>> 3)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w10=int32#8,LSR #3
# asm 2: eor >t0=r14,<t0=r14,<w10=r7,LSR #3
eor r14,r14,r7,LSR #3

# qhasm: w9 += t0
# asm 1: add >w9=int32#3,<w9=int32#3,<t0=int32#14
# asm 2: add >w9=r2,<w9=r2,<t0=r14
add r2,r2,r14

# qhasm: w10 += w3
# asm 1: add >w10=int32#8,<w10=int32#8,<w3=int32#6
# asm 2: add >w10=r7,<w10=r7,<w3=r5
add r7,r7,r5

# qhasm: t0 = (w8 >>> 17)
# asm 1: mov >t0=int32#14,<w8=int32#11,ROR #17
# asm 2: mov >t0=r14,<w8=r10,ROR #17
mov r14,r10,ROR #17

# qhasm: t0 ^= (w8 >>> 19)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w8=int32#11,ROR #19
# asm 2: eor >t0=r14,<t0=r14,<w8=r10,ROR #19
eor r14,r14,r10,ROR #19

# qhasm: t0 ^= (w8 unsigned>> 10)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w8=int32#11,LSR #10
# asm 2: eor >t0=r14,<t0=r14,<w8=r10,LSR #10
eor r14,r14,r10,LSR #10

# qhasm: w10 += t0
# asm 1: add >w10=int32#8,<w10=int32#8,<t0=int32#14
# asm 2: add >w10=r7,<w10=r7,<t0=r14
add r7,r7,r14

# qhasm: t0 = (w11 >>> 7)
# asm 1: mov >t0=int32#14,<w11=int32#12,ROR #7
# asm 2: mov >t0=r14,<w11=r11,ROR #7
mov r14,r11,ROR #7

# qhasm: t0 ^= (w11 >>> 18)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w11=int32#12,ROR #18
# asm 2: eor >t0=r14,<t0=r14,<w11=r11,ROR #18
eor r14,r14,r11,ROR #18

# qhasm: t0 ^= (w11 unsigned>> 3)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w11=int32#12,LSR #3
# asm 2: eor >t0=r14,<t0=r14,<w11=r11,LSR #3
eor r14,r14,r11,LSR #3

# qhasm: w10 += t0
# asm 1: add >w10=int32#8,<w10=int32#8,<t0=int32#14
# asm 2: add >w10=r7,<w10=r7,<t0=r14
add r7,r7,r14

# qhasm: w11 += w4
# asm 1: add >w11=int32#12,<w11=int32#12,<w4=int32#7
# asm 2: add >w11=r11,<w11=r11,<w4=r6
add r11,r11,r6

# qhasm: t0 = (w9 >>> 17)
# asm 1: mov >t0=int32#14,<w9=int32#3,ROR #17
# asm 2: mov >t0=r14,<w9=r2,ROR #17
mov r14,r2,ROR #17

# qhasm: t0 ^= (w9 >>> 19)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w9=int32#3,ROR #19
# asm 2: eor >t0=r14,<t0=r14,<w9=r2,ROR #19
eor r14,r14,r2,ROR #19

# qhasm: t0 ^= (w9 unsigned>> 10)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w9=int32#3,LSR #10
# asm 2: eor >t0=r14,<t0=r14,<w9=r2,LSR #10
eor r14,r14,r2,LSR #10

# qhasm: w11 += t0
# asm 1: add >w11=int32#12,<w11=int32#12,<t0=int32#14
# asm 2: add >w11=r11,<w11=r11,<t0=r14
add r11,r11,r14

# qhasm: t0 = (w12 >>> 7)
# asm 1: mov >t0=int32#14,<w12=int32#13,ROR #7
# asm 2: mov >t0=r14,<w12=r12,ROR #7
mov r14,r12,ROR #7

# qhasm: t0 ^= (w12 >>> 18)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w12=int32#13,ROR #18
# asm 2: eor >t0=r14,<t0=r14,<w12=r12,ROR #18
eor r14,r14,r12,ROR #18

# qhasm: t0 ^= (w12 unsigned>> 3)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w12=int32#13,LSR #3
# asm 2: eor >t0=r14,<t0=r14,<w12=r12,LSR #3
eor r14,r14,r12,LSR #3

# qhasm: w11 += t0
# asm 1: add >w11=int32#12,<w11=int32#12,<t0=int32#14
# asm 2: add >w11=r11,<w11=r11,<t0=r14
add r11,r11,r14

# qhasm: w9_stack = w9
# asm 1: str <w9=int32#3,>w9_stack=stack32#20
# asm 2: str <w9=r2,>w9_stack=[sp,#76]
str r2,[sp,#76]

# qhasm: w1_stack = w1
# asm 1: str <w1=int32#1,>w1_stack=stack32#21
# asm 2: str <w1=r0,>w1_stack=[sp,#80]
str r0,[sp,#80]

# qhasm: w2_stack = w2
# asm 1: str <w2=int32#5,>w2_stack=stack32#22
# asm 2: str <w2=r4,>w2_stack=[sp,#84]
str r4,[sp,#84]

# qhasm: w3_stack = w3
# asm 1: str <w3=int32#6,>w3_stack=stack32#23
# asm 2: str <w3=r5,>w3_stack=[sp,#88]
str r5,[sp,#88]

# qhasm: w4_stack = w4
# asm 1: str <w4=int32#7,>w4_stack=stack32#24
# asm 2: str <w4=r6,>w4_stack=[sp,#92]
str r6,[sp,#92]

# qhasm: w5 = w5_stack
# asm 1: ldr >w5=int32#1,<w5_stack=stack32#25
# asm 2: ldr >w5=r0,<w5_stack=[sp,#96]
ldr r0,[sp,#96]

# qhasm: w13 = w13_stack
# asm 1: ldr >w13=int32#3,<w13_stack=stack32#26
# asm 2: ldr >w13=r2,<w13_stack=[sp,#100]
ldr r2,[sp,#100]

# qhasm: w14 = w14_stack
# asm 1: ldr >w14=int32#5,<w14_stack=stack32#27
# asm 2: ldr >w14=r4,<w14_stack=[sp,#104]
ldr r4,[sp,#104]

# qhasm: w15 = w15_stack
# asm 1: ldr >w15=int32#6,<w15_stack=stack32#28
# asm 2: ldr >w15=r5,<w15_stack=[sp,#108]
ldr r5,[sp,#108]

# qhasm: w0 = w0_stack
# asm 1: ldr >w0=int32#7,<w0_stack=stack32#29
# asm 2: ldr >w0=r6,<w0_stack=[sp,#112]
ldr r6,[sp,#112]

# qhasm: w12 += w5
# asm 1: add >w12=int32#13,<w12=int32#13,<w5=int32#1
# asm 2: add >w12=r12,<w12=r12,<w5=r0
add r12,r12,r0

# qhasm: t0 = (w10 >>> 17)
# asm 1: mov >t0=int32#14,<w10=int32#8,ROR #17
# asm 2: mov >t0=r14,<w10=r7,ROR #17
mov r14,r7,ROR #17

# qhasm: t0 ^= (w10 >>> 19)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w10=int32#8,ROR #19
# asm 2: eor >t0=r14,<t0=r14,<w10=r7,ROR #19
eor r14,r14,r7,ROR #19

# qhasm: t0 ^= (w10 unsigned>> 10)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w10=int32#8,LSR #10
# asm 2: eor >t0=r14,<t0=r14,<w10=r7,LSR #10
eor r14,r14,r7,LSR #10

# qhasm: w12 += t0
# asm 1: add >w12=int32#13,<w12=int32#13,<t0=int32#14
# asm 2: add >w12=r12,<w12=r12,<t0=r14
add r12,r12,r14

# qhasm: t0 = (w13 >>> 7)
# asm 1: mov >t0=int32#14,<w13=int32#3,ROR #7
# asm 2: mov >t0=r14,<w13=r2,ROR #7
mov r14,r2,ROR #7

# qhasm: t0 ^= (w13 >>> 18)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w13=int32#3,ROR #18
# asm 2: eor >t0=r14,<t0=r14,<w13=r2,ROR #18
eor r14,r14,r2,ROR #18

# qhasm: t0 ^= (w13 unsigned>> 3)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w13=int32#3,LSR #3
# asm 2: eor >t0=r14,<t0=r14,<w13=r2,LSR #3
eor r14,r14,r2,LSR #3

# qhasm: w12 += t0
# asm 1: add >w12=int32#13,<w12=int32#13,<t0=int32#14
# asm 2: add >w12=r12,<w12=r12,<t0=r14
add r12,r12,r14

# qhasm: w13 += w6
# asm 1: add >w13=int32#3,<w13=int32#3,<w6=int32#9
# asm 2: add >w13=r2,<w13=r2,<w6=r8
add r2,r2,r8

# qhasm: t0 = (w11 >>> 17)
# asm 1: mov >t0=int32#14,<w11=int32#12,ROR #17
# asm 2: mov >t0=r14,<w11=r11,ROR #17
mov r14,r11,ROR #17

# qhasm: t0 ^= (w11 >>> 19)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w11=int32#12,ROR #19
# asm 2: eor >t0=r14,<t0=r14,<w11=r11,ROR #19
eor r14,r14,r11,ROR #19

# qhasm: t0 ^= (w11 unsigned>> 10)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w11=int32#12,LSR #10
# asm 2: eor >t0=r14,<t0=r14,<w11=r11,LSR #10
eor r14,r14,r11,LSR #10

# qhasm: w13 += t0
# asm 1: add >w13=int32#3,<w13=int32#3,<t0=int32#14
# asm 2: add >w13=r2,<w13=r2,<t0=r14
add r2,r2,r14

# qhasm: t0 = (w14 >>> 7)
# asm 1: mov >t0=int32#14,<w14=int32#5,ROR #7
# asm 2: mov >t0=r14,<w14=r4,ROR #7
mov r14,r4,ROR #7

# qhasm: t0 ^= (w14 >>> 18)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w14=int32#5,ROR #18
# asm 2: eor >t0=r14,<t0=r14,<w14=r4,ROR #18
eor r14,r14,r4,ROR #18

# qhasm: t0 ^= (w14 unsigned>> 3)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w14=int32#5,LSR #3
# asm 2: eor >t0=r14,<t0=r14,<w14=r4,LSR #3
eor r14,r14,r4,LSR #3

# qhasm: w13 += t0
# asm 1: add >w13=int32#3,<w13=int32#3,<t0=int32#14
# asm 2: add >w13=r2,<w13=r2,<t0=r14
add r2,r2,r14

# qhasm: w14 += w7
# asm 1: add >w14=int32#5,<w14=int32#5,<w7=int32#10
# asm 2: add >w14=r4,<w14=r4,<w7=r9
add r4,r4,r9

# qhasm: t0 = (w12 >>> 17)
# asm 1: mov >t0=int32#14,<w12=int32#13,ROR #17
# asm 2: mov >t0=r14,<w12=r12,ROR #17
mov r14,r12,ROR #17

# qhasm: t0 ^= (w12 >>> 19)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w12=int32#13,ROR #19
# asm 2: eor >t0=r14,<t0=r14,<w12=r12,ROR #19
eor r14,r14,r12,ROR #19

# qhasm: t0 ^= (w12 unsigned>> 10)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w12=int32#13,LSR #10
# asm 2: eor >t0=r14,<t0=r14,<w12=r12,LSR #10
eor r14,r14,r12,LSR #10

# qhasm: w14 += t0
# asm 1: add >w14=int32#5,<w14=int32#5,<t0=int32#14
# asm 2: add >w14=r4,<w14=r4,<t0=r14
add r4,r4,r14

# qhasm: t0 = (w15 >>> 7)
# asm 1: mov >t0=int32#14,<w15=int32#6,ROR #7
# asm 2: mov >t0=r14,<w15=r5,ROR #7
mov r14,r5,ROR #7

# qhasm: t0 ^= (w15 >>> 18)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w15=int32#6,ROR #18
# asm 2: eor >t0=r14,<t0=r14,<w15=r5,ROR #18
eor r14,r14,r5,ROR #18

# qhasm: t0 ^= (w15 unsigned>> 3)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w15=int32#6,LSR #3
# asm 2: eor >t0=r14,<t0=r14,<w15=r5,LSR #3
eor r14,r14,r5,LSR #3

# qhasm: w14 += t0
# asm 1: add >w14=int32#5,<w14=int32#5,<t0=int32#14
# asm 2: add >w14=r4,<w14=r4,<t0=r14
add r4,r4,r14

# qhasm: w15 += w8
# asm 1: add >w15=int32#6,<w15=int32#6,<w8=int32#11
# asm 2: add >w15=r5,<w15=r5,<w8=r10
add r5,r5,r10

# qhasm: t0 = (w13 >>> 17)
# asm 1: mov >t0=int32#14,<w13=int32#3,ROR #17
# asm 2: mov >t0=r14,<w13=r2,ROR #17
mov r14,r2,ROR #17

# qhasm: t0 ^= (w13 >>> 19)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w13=int32#3,ROR #19
# asm 2: eor >t0=r14,<t0=r14,<w13=r2,ROR #19
eor r14,r14,r2,ROR #19

# qhasm: t0 ^= (w13 unsigned>> 10)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w13=int32#3,LSR #10
# asm 2: eor >t0=r14,<t0=r14,<w13=r2,LSR #10
eor r14,r14,r2,LSR #10

# qhasm: w15 += t0
# asm 1: add >w15=int32#6,<w15=int32#6,<t0=int32#14
# asm 2: add >w15=r5,<w15=r5,<t0=r14
add r5,r5,r14

# qhasm: t0 = (w0 >>> 7)
# asm 1: mov >t0=int32#14,<w0=int32#7,ROR #7
# asm 2: mov >t0=r14,<w0=r6,ROR #7
mov r14,r6,ROR #7

# qhasm: t0 ^= (w0 >>> 18)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w0=int32#7,ROR #18
# asm 2: eor >t0=r14,<t0=r14,<w0=r6,ROR #18
eor r14,r14,r6,ROR #18

# qhasm: t0 ^= (w0 unsigned>> 3)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w0=int32#7,LSR #3
# asm 2: eor >t0=r14,<t0=r14,<w0=r6,LSR #3
eor r14,r14,r6,LSR #3

# qhasm: w15 += t0
# asm 1: add >w15=int32#6,<w15=int32#6,<t0=int32#14
# asm 2: add >w15=r5,<w15=r5,<t0=r14
add r5,r5,r14

# qhasm: w0_stack = w0
# asm 1: str <w0=int32#7,>w0_stack=stack32#25
# asm 2: str <w0=r6,>w0_stack=[sp,#96]
str r6,[sp,#96]

# qhasm: w5_stack = w5
# asm 1: str <w5=int32#1,>w5_stack=stack32#26
# asm 2: str <w5=r0,>w5_stack=[sp,#100]
str r0,[sp,#100]

# qhasm: w6_stack = w6
# asm 1: str <w6=int32#9,>w6_stack=stack32#27
# asm 2: str <w6=r8,>w6_stack=[sp,#104]
str r8,[sp,#104]

# qhasm: w7_stack = w7
# asm 1: str <w7=int32#10,>w7_stack=stack32#28
# asm 2: str <w7=r9,>w7_stack=[sp,#108]
str r9,[sp,#108]

# qhasm: w8_stack = w8
# asm 1: str <w8=int32#11,>w8_stack=stack32#29
# asm 2: str <w8=r10,>w8_stack=[sp,#112]
str r10,[sp,#112]

# qhasm: w10_stack = w10
# asm 1: str <w10=int32#8,>w10_stack=stack32#30
# asm 2: str <w10=r7,>w10_stack=[sp,#116]
str r7,[sp,#116]

# qhasm: w11_stack = w11
# asm 1: str <w11=int32#12,>w11_stack=stack32#31
# asm 2: str <w11=r11,>w11_stack=[sp,#120]
str r11,[sp,#120]

# qhasm: w12_stack = w12
# asm 1: str <w12=int32#13,>w12_stack=stack32#32
# asm 2: str <w12=r12,>w12_stack=[sp,#124]
str r12,[sp,#124]

# qhasm: w13_stack = w13
# asm 1: str <w13=int32#3,>w13_stack=stack32#33
# asm 2: str <w13=r2,>w13_stack=[sp,#128]
str r2,[sp,#128]

# qhasm: w14_stack = w14
# asm 1: str <w14=int32#5,>w14_stack=stack32#34
# asm 2: str <w14=r4,>w14_stack=[sp,#132]
str r4,[sp,#132]

# qhasm: w15_stack = w15
# asm 1: str <w15=int32#6,>w15_stack=stack32#35
# asm 2: str <w15=r5,>w15_stack=[sp,#136]
str r5,[sp,#136]

# qhasm: r0 = r0_stack
# asm 1: ldr >r0=int32#1,<r0_stack=stack32#36
# asm 2: ldr >r0=r0,<r0_stack=[sp,#140]
ldr r0,[sp,#140]

# qhasm: r1 = r1_stack
# asm 1: ldr >r1=int32#3,<r1_stack=stack32#37
# asm 2: ldr >r1=r2,<r1_stack=[sp,#144]
ldr r2,[sp,#144]

# qhasm: r2 = r2_stack
# asm 1: ldr >r2=int32#5,<r2_stack=stack32#38
# asm 2: ldr >r2=r4,<r2_stack=[sp,#148]
ldr r4,[sp,#148]

# qhasm: r3 = r3_stack
# asm 1: ldr >r3=int32#6,<r3_stack=stack32#39
# asm 2: ldr >r3=r5,<r3_stack=[sp,#152]
ldr r5,[sp,#152]

# qhasm: r4 = r4_stack
# asm 1: ldr >r4=int32#7,<r4_stack=stack32#40
# asm 2: ldr >r4=r6,<r4_stack=[sp,#156]
ldr r6,[sp,#156]

# qhasm: r5 = r5_stack
# asm 1: ldr >r5=int32#8,<r5_stack=stack32#41
# asm 2: ldr >r5=r7,<r5_stack=[sp,#160]
ldr r7,[sp,#160]

# qhasm: r6 = r6_stack
# asm 1: ldr >r6=int32#9,<r6_stack=stack32#42
# asm 2: ldr >r6=r8,<r6_stack=[sp,#164]
ldr r8,[sp,#164]

# qhasm: r7 = r7_stack
# asm 1: ldr >r7=int32#10,<r7_stack=stack32#43
# asm 2: ldr >r7=r9,<r7_stack=[sp,#168]
ldr r9,[sp,#168]

# qhasm: w0 = w0_stack
# asm 1: ldr >w0=int32#11,<w0_stack=stack32#25
# asm 2: ldr >w0=r10,<w0_stack=[sp,#96]
ldr r10,[sp,#96]

# qhasm: t0 = r4 & r5
# asm 1: and >t0=int32#12,<r4=int32#7,<r5=int32#8
# asm 2: and >t0=r11,<r4=r6,<r5=r7
and r11,r6,r7

# qhasm: t1 = r6 & ~r4
# asm 1: bic >t1=int32#13,<r6=int32#9,<r4=int32#7
# asm 2: bic >t1=r12,<r6=r8,<r4=r6
bic r12,r8,r6

# qhasm: t0 ^= t1
# asm 1: eor >t0=int32#12,<t0=int32#12,<t1=int32#13
# asm 2: eor >t0=r11,<t0=r11,<t1=r12
eor r11,r11,r12

# qhasm: t1 = r4 ^ (r4 >>> 5)
# asm 1: eor >t1=int32#13,<r4=int32#7,<r4=int32#7,ROR #5
# asm 2: eor >t1=r12,<r4=r6,<r4=r6,ROR #5
eor r12,r6,r6,ROR #5

# qhasm: t1 ^= (r4 >>> 19)
# asm 1: eor >t1=int32#13,<t1=int32#13,<r4=int32#7,ROR #19
# asm 2: eor >t1=r12,<t1=r12,<r4=r6,ROR #19
eor r12,r12,r6,ROR #19

# qhasm: t2 = mem32[input_3 + 128]
# asm 1: ldr >t2=int32#14,[<input_3=int32#4,#128]
# asm 2: ldr >t2=r14,[<input_3=r3,#128]
ldr r14,[r3,#128]

# qhasm: t0 += w0
# asm 1: add >t0=int32#11,<t0=int32#12,<w0=int32#11
# asm 2: add >t0=r10,<t0=r11,<w0=r10
add r10,r11,r10

# qhasm: t0 += (t1 >>> 6)
# asm 1: add >t0=int32#11,<t0=int32#11,<t1=int32#13,ROR #6
# asm 2: add >t0=r10,<t0=r10,<t1=r12,ROR #6
add r10,r10,r12,ROR #6

# qhasm: t0 += t2
# asm 1: add >t0=int32#11,<t0=int32#11,<t2=int32#14
# asm 2: add >t0=r10,<t0=r10,<t2=r14
add r10,r10,r14

# qhasm: r7 += t0
# asm 1: add >r7=int32#10,<r7=int32#10,<t0=int32#11
# asm 2: add >r7=r9,<r7=r9,<t0=r10
add r9,r9,r10

# qhasm: r3 += r7
# asm 1: add >r3=int32#6,<r3=int32#6,<r7=int32#10
# asm 2: add >r3=r5,<r3=r5,<r7=r9
add r5,r5,r9

# qhasm: t0 = r0 & r1
# asm 1: and >t0=int32#11,<r0=int32#1,<r1=int32#3
# asm 2: and >t0=r10,<r0=r0,<r1=r2
and r10,r0,r2

# qhasm: t2 = r0 & r2
# asm 1: and >t2=int32#12,<r0=int32#1,<r2=int32#5
# asm 2: and >t2=r11,<r0=r0,<r2=r4
and r11,r0,r4

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t2 = r1 & r2
# asm 1: and >t2=int32#12,<r1=int32#3,<r2=int32#5
# asm 2: and >t2=r11,<r1=r2,<r2=r4
and r11,r2,r4

# qhasm: t3 = r0 ^ (r0 >>> 11)
# asm 1: eor >t3=int32#13,<r0=int32#1,<r0=int32#1,ROR #11
# asm 2: eor >t3=r12,<r0=r0,<r0=r0,ROR #11
eor r12,r0,r0,ROR #11

# qhasm: t3 ^= (r0 >>> 20)
# asm 1: eor >t3=int32#13,<t3=int32#13,<r0=int32#1,ROR #20
# asm 2: eor >t3=r12,<t3=r12,<r0=r0,ROR #20
eor r12,r12,r0,ROR #20

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t0 += (t3 >>> 2)
# asm 1: add >t0=int32#11,<t0=int32#11,<t3=int32#13,ROR #2
# asm 2: add >t0=r10,<t0=r10,<t3=r12,ROR #2
add r10,r10,r12,ROR #2

# qhasm: r7 += t0
# asm 1: add >r7=int32#10,<r7=int32#10,<t0=int32#11
# asm 2: add >r7=r9,<r7=r9,<t0=r10
add r9,r9,r10

# qhasm: w1 = w1_stack
# asm 1: ldr >w1=int32#11,<w1_stack=stack32#21
# asm 2: ldr >w1=r10,<w1_stack=[sp,#80]
ldr r10,[sp,#80]

# qhasm: t0 = r3 & r4
# asm 1: and >t0=int32#12,<r3=int32#6,<r4=int32#7
# asm 2: and >t0=r11,<r3=r5,<r4=r6
and r11,r5,r6

# qhasm: t1 = r5 & ~r3
# asm 1: bic >t1=int32#13,<r5=int32#8,<r3=int32#6
# asm 2: bic >t1=r12,<r5=r7,<r3=r5
bic r12,r7,r5

# qhasm: t0 ^= t1
# asm 1: eor >t0=int32#12,<t0=int32#12,<t1=int32#13
# asm 2: eor >t0=r11,<t0=r11,<t1=r12
eor r11,r11,r12

# qhasm: t1 = r3 ^ (r3 >>> 5)
# asm 1: eor >t1=int32#13,<r3=int32#6,<r3=int32#6,ROR #5
# asm 2: eor >t1=r12,<r3=r5,<r3=r5,ROR #5
eor r12,r5,r5,ROR #5

# qhasm: t1 ^= (r3 >>> 19)
# asm 1: eor >t1=int32#13,<t1=int32#13,<r3=int32#6,ROR #19
# asm 2: eor >t1=r12,<t1=r12,<r3=r5,ROR #19
eor r12,r12,r5,ROR #19

# qhasm: t2 = mem32[input_3 + 132]
# asm 1: ldr >t2=int32#14,[<input_3=int32#4,#132]
# asm 2: ldr >t2=r14,[<input_3=r3,#132]
ldr r14,[r3,#132]

# qhasm: t0 += w1
# asm 1: add >t0=int32#11,<t0=int32#12,<w1=int32#11
# asm 2: add >t0=r10,<t0=r11,<w1=r10
add r10,r11,r10

# qhasm: t0 += (t1 >>> 6)
# asm 1: add >t0=int32#11,<t0=int32#11,<t1=int32#13,ROR #6
# asm 2: add >t0=r10,<t0=r10,<t1=r12,ROR #6
add r10,r10,r12,ROR #6

# qhasm: t0 += t2
# asm 1: add >t0=int32#11,<t0=int32#11,<t2=int32#14
# asm 2: add >t0=r10,<t0=r10,<t2=r14
add r10,r10,r14

# qhasm: r6 += t0
# asm 1: add >r6=int32#9,<r6=int32#9,<t0=int32#11
# asm 2: add >r6=r8,<r6=r8,<t0=r10
add r8,r8,r10

# qhasm: r2 += r6
# asm 1: add >r2=int32#5,<r2=int32#5,<r6=int32#9
# asm 2: add >r2=r4,<r2=r4,<r6=r8
add r4,r4,r8

# qhasm: t0 = r7 & r0
# asm 1: and >t0=int32#11,<r7=int32#10,<r0=int32#1
# asm 2: and >t0=r10,<r7=r9,<r0=r0
and r10,r9,r0

# qhasm: t2 = r7 & r1
# asm 1: and >t2=int32#12,<r7=int32#10,<r1=int32#3
# asm 2: and >t2=r11,<r7=r9,<r1=r2
and r11,r9,r2

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t2 = r0 & r1
# asm 1: and >t2=int32#12,<r0=int32#1,<r1=int32#3
# asm 2: and >t2=r11,<r0=r0,<r1=r2
and r11,r0,r2

# qhasm: t3 = r7 ^ (r7 >>> 11)
# asm 1: eor >t3=int32#13,<r7=int32#10,<r7=int32#10,ROR #11
# asm 2: eor >t3=r12,<r7=r9,<r7=r9,ROR #11
eor r12,r9,r9,ROR #11

# qhasm: t3 ^= (r7 >>> 20)
# asm 1: eor >t3=int32#13,<t3=int32#13,<r7=int32#10,ROR #20
# asm 2: eor >t3=r12,<t3=r12,<r7=r9,ROR #20
eor r12,r12,r9,ROR #20

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t0 += (t3 >>> 2)
# asm 1: add >t0=int32#11,<t0=int32#11,<t3=int32#13,ROR #2
# asm 2: add >t0=r10,<t0=r10,<t3=r12,ROR #2
add r10,r10,r12,ROR #2

# qhasm: r6 += t0
# asm 1: add >r6=int32#9,<r6=int32#9,<t0=int32#11
# asm 2: add >r6=r8,<r6=r8,<t0=r10
add r8,r8,r10

# qhasm: w2 = w2_stack
# asm 1: ldr >w2=int32#11,<w2_stack=stack32#22
# asm 2: ldr >w2=r10,<w2_stack=[sp,#84]
ldr r10,[sp,#84]

# qhasm: t0 = r2 & r3
# asm 1: and >t0=int32#12,<r2=int32#5,<r3=int32#6
# asm 2: and >t0=r11,<r2=r4,<r3=r5
and r11,r4,r5

# qhasm: t1 = r4 & ~r2
# asm 1: bic >t1=int32#13,<r4=int32#7,<r2=int32#5
# asm 2: bic >t1=r12,<r4=r6,<r2=r4
bic r12,r6,r4

# qhasm: t0 ^= t1
# asm 1: eor >t0=int32#12,<t0=int32#12,<t1=int32#13
# asm 2: eor >t0=r11,<t0=r11,<t1=r12
eor r11,r11,r12

# qhasm: t1 = r2 ^ (r2 >>> 5)
# asm 1: eor >t1=int32#13,<r2=int32#5,<r2=int32#5,ROR #5
# asm 2: eor >t1=r12,<r2=r4,<r2=r4,ROR #5
eor r12,r4,r4,ROR #5

# qhasm: t1 ^= (r2 >>> 19)
# asm 1: eor >t1=int32#13,<t1=int32#13,<r2=int32#5,ROR #19
# asm 2: eor >t1=r12,<t1=r12,<r2=r4,ROR #19
eor r12,r12,r4,ROR #19

# qhasm: t2 = mem32[input_3 + 136]
# asm 1: ldr >t2=int32#14,[<input_3=int32#4,#136]
# asm 2: ldr >t2=r14,[<input_3=r3,#136]
ldr r14,[r3,#136]

# qhasm: t0 += w2
# asm 1: add >t0=int32#11,<t0=int32#12,<w2=int32#11
# asm 2: add >t0=r10,<t0=r11,<w2=r10
add r10,r11,r10

# qhasm: t0 += (t1 >>> 6)
# asm 1: add >t0=int32#11,<t0=int32#11,<t1=int32#13,ROR #6
# asm 2: add >t0=r10,<t0=r10,<t1=r12,ROR #6
add r10,r10,r12,ROR #6

# qhasm: t0 += t2
# asm 1: add >t0=int32#11,<t0=int32#11,<t2=int32#14
# asm 2: add >t0=r10,<t0=r10,<t2=r14
add r10,r10,r14

# qhasm: r5 += t0
# asm 1: add >r5=int32#8,<r5=int32#8,<t0=int32#11
# asm 2: add >r5=r7,<r5=r7,<t0=r10
add r7,r7,r10

# qhasm: r1 += r5
# asm 1: add >r1=int32#3,<r1=int32#3,<r5=int32#8
# asm 2: add >r1=r2,<r1=r2,<r5=r7
add r2,r2,r7

# qhasm: t0 = r6 & r7
# asm 1: and >t0=int32#11,<r6=int32#9,<r7=int32#10
# asm 2: and >t0=r10,<r6=r8,<r7=r9
and r10,r8,r9

# qhasm: t2 = r6 & r0
# asm 1: and >t2=int32#12,<r6=int32#9,<r0=int32#1
# asm 2: and >t2=r11,<r6=r8,<r0=r0
and r11,r8,r0

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t2 = r7 & r0
# asm 1: and >t2=int32#12,<r7=int32#10,<r0=int32#1
# asm 2: and >t2=r11,<r7=r9,<r0=r0
and r11,r9,r0

# qhasm: t3 = r6 ^ (r6 >>> 11)
# asm 1: eor >t3=int32#13,<r6=int32#9,<r6=int32#9,ROR #11
# asm 2: eor >t3=r12,<r6=r8,<r6=r8,ROR #11
eor r12,r8,r8,ROR #11

# qhasm: t3 ^= (r6 >>> 20)
# asm 1: eor >t3=int32#13,<t3=int32#13,<r6=int32#9,ROR #20
# asm 2: eor >t3=r12,<t3=r12,<r6=r8,ROR #20
eor r12,r12,r8,ROR #20

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t0 += (t3 >>> 2)
# asm 1: add >t0=int32#11,<t0=int32#11,<t3=int32#13,ROR #2
# asm 2: add >t0=r10,<t0=r10,<t3=r12,ROR #2
add r10,r10,r12,ROR #2

# qhasm: r5 += t0
# asm 1: add >r5=int32#8,<r5=int32#8,<t0=int32#11
# asm 2: add >r5=r7,<r5=r7,<t0=r10
add r7,r7,r10

# qhasm: w3 = w3_stack
# asm 1: ldr >w3=int32#11,<w3_stack=stack32#23
# asm 2: ldr >w3=r10,<w3_stack=[sp,#88]
ldr r10,[sp,#88]

# qhasm: t0 = r1 & r2
# asm 1: and >t0=int32#12,<r1=int32#3,<r2=int32#5
# asm 2: and >t0=r11,<r1=r2,<r2=r4
and r11,r2,r4

# qhasm: t1 = r3 & ~r1
# asm 1: bic >t1=int32#13,<r3=int32#6,<r1=int32#3
# asm 2: bic >t1=r12,<r3=r5,<r1=r2
bic r12,r5,r2

# qhasm: t0 ^= t1
# asm 1: eor >t0=int32#12,<t0=int32#12,<t1=int32#13
# asm 2: eor >t0=r11,<t0=r11,<t1=r12
eor r11,r11,r12

# qhasm: t1 = r1 ^ (r1 >>> 5)
# asm 1: eor >t1=int32#13,<r1=int32#3,<r1=int32#3,ROR #5
# asm 2: eor >t1=r12,<r1=r2,<r1=r2,ROR #5
eor r12,r2,r2,ROR #5

# qhasm: t1 ^= (r1 >>> 19)
# asm 1: eor >t1=int32#13,<t1=int32#13,<r1=int32#3,ROR #19
# asm 2: eor >t1=r12,<t1=r12,<r1=r2,ROR #19
eor r12,r12,r2,ROR #19

# qhasm: t2 = mem32[input_3 + 140]
# asm 1: ldr >t2=int32#14,[<input_3=int32#4,#140]
# asm 2: ldr >t2=r14,[<input_3=r3,#140]
ldr r14,[r3,#140]

# qhasm: t0 += w3
# asm 1: add >t0=int32#11,<t0=int32#12,<w3=int32#11
# asm 2: add >t0=r10,<t0=r11,<w3=r10
add r10,r11,r10

# qhasm: t0 += (t1 >>> 6)
# asm 1: add >t0=int32#11,<t0=int32#11,<t1=int32#13,ROR #6
# asm 2: add >t0=r10,<t0=r10,<t1=r12,ROR #6
add r10,r10,r12,ROR #6

# qhasm: t0 += t2
# asm 1: add >t0=int32#11,<t0=int32#11,<t2=int32#14
# asm 2: add >t0=r10,<t0=r10,<t2=r14
add r10,r10,r14

# qhasm: r4 += t0
# asm 1: add >r4=int32#7,<r4=int32#7,<t0=int32#11
# asm 2: add >r4=r6,<r4=r6,<t0=r10
add r6,r6,r10

# qhasm: r0 += r4
# asm 1: add >r0=int32#1,<r0=int32#1,<r4=int32#7
# asm 2: add >r0=r0,<r0=r0,<r4=r6
add r0,r0,r6

# qhasm: t0 = r5 & r6
# asm 1: and >t0=int32#11,<r5=int32#8,<r6=int32#9
# asm 2: and >t0=r10,<r5=r7,<r6=r8
and r10,r7,r8

# qhasm: t2 = r5 & r7
# asm 1: and >t2=int32#12,<r5=int32#8,<r7=int32#10
# asm 2: and >t2=r11,<r5=r7,<r7=r9
and r11,r7,r9

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t2 = r6 & r7
# asm 1: and >t2=int32#12,<r6=int32#9,<r7=int32#10
# asm 2: and >t2=r11,<r6=r8,<r7=r9
and r11,r8,r9

# qhasm: t3 = r5 ^ (r5 >>> 11)
# asm 1: eor >t3=int32#13,<r5=int32#8,<r5=int32#8,ROR #11
# asm 2: eor >t3=r12,<r5=r7,<r5=r7,ROR #11
eor r12,r7,r7,ROR #11

# qhasm: t3 ^= (r5 >>> 20)
# asm 1: eor >t3=int32#13,<t3=int32#13,<r5=int32#8,ROR #20
# asm 2: eor >t3=r12,<t3=r12,<r5=r7,ROR #20
eor r12,r12,r7,ROR #20

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t0 += (t3 >>> 2)
# asm 1: add >t0=int32#11,<t0=int32#11,<t3=int32#13,ROR #2
# asm 2: add >t0=r10,<t0=r10,<t3=r12,ROR #2
add r10,r10,r12,ROR #2

# qhasm: r4 += t0
# asm 1: add >r4=int32#7,<r4=int32#7,<t0=int32#11
# asm 2: add >r4=r6,<r4=r6,<t0=r10
add r6,r6,r10

# qhasm: w4 = w4_stack
# asm 1: ldr >w4=int32#11,<w4_stack=stack32#24
# asm 2: ldr >w4=r10,<w4_stack=[sp,#92]
ldr r10,[sp,#92]

# qhasm: t0 = r0 & r1
# asm 1: and >t0=int32#12,<r0=int32#1,<r1=int32#3
# asm 2: and >t0=r11,<r0=r0,<r1=r2
and r11,r0,r2

# qhasm: t1 = r2 & ~r0
# asm 1: bic >t1=int32#13,<r2=int32#5,<r0=int32#1
# asm 2: bic >t1=r12,<r2=r4,<r0=r0
bic r12,r4,r0

# qhasm: t0 ^= t1
# asm 1: eor >t0=int32#12,<t0=int32#12,<t1=int32#13
# asm 2: eor >t0=r11,<t0=r11,<t1=r12
eor r11,r11,r12

# qhasm: t1 = r0 ^ (r0 >>> 5)
# asm 1: eor >t1=int32#13,<r0=int32#1,<r0=int32#1,ROR #5
# asm 2: eor >t1=r12,<r0=r0,<r0=r0,ROR #5
eor r12,r0,r0,ROR #5

# qhasm: t1 ^= (r0 >>> 19)
# asm 1: eor >t1=int32#13,<t1=int32#13,<r0=int32#1,ROR #19
# asm 2: eor >t1=r12,<t1=r12,<r0=r0,ROR #19
eor r12,r12,r0,ROR #19

# qhasm: t2 = mem32[input_3 + 144]
# asm 1: ldr >t2=int32#14,[<input_3=int32#4,#144]
# asm 2: ldr >t2=r14,[<input_3=r3,#144]
ldr r14,[r3,#144]

# qhasm: t0 += w4
# asm 1: add >t0=int32#11,<t0=int32#12,<w4=int32#11
# asm 2: add >t0=r10,<t0=r11,<w4=r10
add r10,r11,r10

# qhasm: t0 += (t1 >>> 6)
# asm 1: add >t0=int32#11,<t0=int32#11,<t1=int32#13,ROR #6
# asm 2: add >t0=r10,<t0=r10,<t1=r12,ROR #6
add r10,r10,r12,ROR #6

# qhasm: t0 += t2
# asm 1: add >t0=int32#11,<t0=int32#11,<t2=int32#14
# asm 2: add >t0=r10,<t0=r10,<t2=r14
add r10,r10,r14

# qhasm: r3 += t0
# asm 1: add >r3=int32#6,<r3=int32#6,<t0=int32#11
# asm 2: add >r3=r5,<r3=r5,<t0=r10
add r5,r5,r10

# qhasm: r7 += r3
# asm 1: add >r7=int32#10,<r7=int32#10,<r3=int32#6
# asm 2: add >r7=r9,<r7=r9,<r3=r5
add r9,r9,r5

# qhasm: t0 = r4 & r5
# asm 1: and >t0=int32#11,<r4=int32#7,<r5=int32#8
# asm 2: and >t0=r10,<r4=r6,<r5=r7
and r10,r6,r7

# qhasm: t2 = r4 & r6
# asm 1: and >t2=int32#12,<r4=int32#7,<r6=int32#9
# asm 2: and >t2=r11,<r4=r6,<r6=r8
and r11,r6,r8

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t2 = r5 & r6
# asm 1: and >t2=int32#12,<r5=int32#8,<r6=int32#9
# asm 2: and >t2=r11,<r5=r7,<r6=r8
and r11,r7,r8

# qhasm: t3 = r4 ^ (r4 >>> 11)
# asm 1: eor >t3=int32#13,<r4=int32#7,<r4=int32#7,ROR #11
# asm 2: eor >t3=r12,<r4=r6,<r4=r6,ROR #11
eor r12,r6,r6,ROR #11

# qhasm: t3 ^= (r4 >>> 20)
# asm 1: eor >t3=int32#13,<t3=int32#13,<r4=int32#7,ROR #20
# asm 2: eor >t3=r12,<t3=r12,<r4=r6,ROR #20
eor r12,r12,r6,ROR #20

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t0 += (t3 >>> 2)
# asm 1: add >t0=int32#11,<t0=int32#11,<t3=int32#13,ROR #2
# asm 2: add >t0=r10,<t0=r10,<t3=r12,ROR #2
add r10,r10,r12,ROR #2

# qhasm: r3 += t0
# asm 1: add >r3=int32#6,<r3=int32#6,<t0=int32#11
# asm 2: add >r3=r5,<r3=r5,<t0=r10
add r5,r5,r10

# qhasm: w5 = w5_stack
# asm 1: ldr >w5=int32#11,<w5_stack=stack32#26
# asm 2: ldr >w5=r10,<w5_stack=[sp,#100]
ldr r10,[sp,#100]

# qhasm: t0 = r7 & r0
# asm 1: and >t0=int32#12,<r7=int32#10,<r0=int32#1
# asm 2: and >t0=r11,<r7=r9,<r0=r0
and r11,r9,r0

# qhasm: t1 = r1 & ~r7
# asm 1: bic >t1=int32#13,<r1=int32#3,<r7=int32#10
# asm 2: bic >t1=r12,<r1=r2,<r7=r9
bic r12,r2,r9

# qhasm: t0 ^= t1
# asm 1: eor >t0=int32#12,<t0=int32#12,<t1=int32#13
# asm 2: eor >t0=r11,<t0=r11,<t1=r12
eor r11,r11,r12

# qhasm: t1 = r7 ^ (r7 >>> 5)
# asm 1: eor >t1=int32#13,<r7=int32#10,<r7=int32#10,ROR #5
# asm 2: eor >t1=r12,<r7=r9,<r7=r9,ROR #5
eor r12,r9,r9,ROR #5

# qhasm: t1 ^= (r7 >>> 19)
# asm 1: eor >t1=int32#13,<t1=int32#13,<r7=int32#10,ROR #19
# asm 2: eor >t1=r12,<t1=r12,<r7=r9,ROR #19
eor r12,r12,r9,ROR #19

# qhasm: t2 = mem32[input_3 + 148]
# asm 1: ldr >t2=int32#14,[<input_3=int32#4,#148]
# asm 2: ldr >t2=r14,[<input_3=r3,#148]
ldr r14,[r3,#148]

# qhasm: t0 += w5
# asm 1: add >t0=int32#11,<t0=int32#12,<w5=int32#11
# asm 2: add >t0=r10,<t0=r11,<w5=r10
add r10,r11,r10

# qhasm: t0 += (t1 >>> 6)
# asm 1: add >t0=int32#11,<t0=int32#11,<t1=int32#13,ROR #6
# asm 2: add >t0=r10,<t0=r10,<t1=r12,ROR #6
add r10,r10,r12,ROR #6

# qhasm: t0 += t2
# asm 1: add >t0=int32#11,<t0=int32#11,<t2=int32#14
# asm 2: add >t0=r10,<t0=r10,<t2=r14
add r10,r10,r14

# qhasm: r2 += t0
# asm 1: add >r2=int32#5,<r2=int32#5,<t0=int32#11
# asm 2: add >r2=r4,<r2=r4,<t0=r10
add r4,r4,r10

# qhasm: r6 += r2
# asm 1: add >r6=int32#9,<r6=int32#9,<r2=int32#5
# asm 2: add >r6=r8,<r6=r8,<r2=r4
add r8,r8,r4

# qhasm: t0 = r3 & r4
# asm 1: and >t0=int32#11,<r3=int32#6,<r4=int32#7
# asm 2: and >t0=r10,<r3=r5,<r4=r6
and r10,r5,r6

# qhasm: t2 = r3 & r5
# asm 1: and >t2=int32#12,<r3=int32#6,<r5=int32#8
# asm 2: and >t2=r11,<r3=r5,<r5=r7
and r11,r5,r7

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t2 = r4 & r5
# asm 1: and >t2=int32#12,<r4=int32#7,<r5=int32#8
# asm 2: and >t2=r11,<r4=r6,<r5=r7
and r11,r6,r7

# qhasm: t3 = r3 ^ (r3 >>> 11)
# asm 1: eor >t3=int32#13,<r3=int32#6,<r3=int32#6,ROR #11
# asm 2: eor >t3=r12,<r3=r5,<r3=r5,ROR #11
eor r12,r5,r5,ROR #11

# qhasm: t3 ^= (r3 >>> 20)
# asm 1: eor >t3=int32#13,<t3=int32#13,<r3=int32#6,ROR #20
# asm 2: eor >t3=r12,<t3=r12,<r3=r5,ROR #20
eor r12,r12,r5,ROR #20

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t0 += (t3 >>> 2)
# asm 1: add >t0=int32#11,<t0=int32#11,<t3=int32#13,ROR #2
# asm 2: add >t0=r10,<t0=r10,<t3=r12,ROR #2
add r10,r10,r12,ROR #2

# qhasm: r2 += t0
# asm 1: add >r2=int32#5,<r2=int32#5,<t0=int32#11
# asm 2: add >r2=r4,<r2=r4,<t0=r10
add r4,r4,r10

# qhasm: w6 = w6_stack
# asm 1: ldr >w6=int32#11,<w6_stack=stack32#27
# asm 2: ldr >w6=r10,<w6_stack=[sp,#104]
ldr r10,[sp,#104]

# qhasm: t0 = r6 & r7
# asm 1: and >t0=int32#12,<r6=int32#9,<r7=int32#10
# asm 2: and >t0=r11,<r6=r8,<r7=r9
and r11,r8,r9

# qhasm: t1 = r0 & ~r6
# asm 1: bic >t1=int32#13,<r0=int32#1,<r6=int32#9
# asm 2: bic >t1=r12,<r0=r0,<r6=r8
bic r12,r0,r8

# qhasm: t0 ^= t1
# asm 1: eor >t0=int32#12,<t0=int32#12,<t1=int32#13
# asm 2: eor >t0=r11,<t0=r11,<t1=r12
eor r11,r11,r12

# qhasm: t1 = r6 ^ (r6 >>> 5)
# asm 1: eor >t1=int32#13,<r6=int32#9,<r6=int32#9,ROR #5
# asm 2: eor >t1=r12,<r6=r8,<r6=r8,ROR #5
eor r12,r8,r8,ROR #5

# qhasm: t1 ^= (r6 >>> 19)
# asm 1: eor >t1=int32#13,<t1=int32#13,<r6=int32#9,ROR #19
# asm 2: eor >t1=r12,<t1=r12,<r6=r8,ROR #19
eor r12,r12,r8,ROR #19

# qhasm: t2 = mem32[input_3 + 152]
# asm 1: ldr >t2=int32#14,[<input_3=int32#4,#152]
# asm 2: ldr >t2=r14,[<input_3=r3,#152]
ldr r14,[r3,#152]

# qhasm: t0 += w6
# asm 1: add >t0=int32#11,<t0=int32#12,<w6=int32#11
# asm 2: add >t0=r10,<t0=r11,<w6=r10
add r10,r11,r10

# qhasm: t0 += (t1 >>> 6)
# asm 1: add >t0=int32#11,<t0=int32#11,<t1=int32#13,ROR #6
# asm 2: add >t0=r10,<t0=r10,<t1=r12,ROR #6
add r10,r10,r12,ROR #6

# qhasm: t0 += t2
# asm 1: add >t0=int32#11,<t0=int32#11,<t2=int32#14
# asm 2: add >t0=r10,<t0=r10,<t2=r14
add r10,r10,r14

# qhasm: r1 += t0
# asm 1: add >r1=int32#3,<r1=int32#3,<t0=int32#11
# asm 2: add >r1=r2,<r1=r2,<t0=r10
add r2,r2,r10

# qhasm: r5 += r1
# asm 1: add >r5=int32#8,<r5=int32#8,<r1=int32#3
# asm 2: add >r5=r7,<r5=r7,<r1=r2
add r7,r7,r2

# qhasm: t0 = r2 & r3
# asm 1: and >t0=int32#11,<r2=int32#5,<r3=int32#6
# asm 2: and >t0=r10,<r2=r4,<r3=r5
and r10,r4,r5

# qhasm: t2 = r2 & r4
# asm 1: and >t2=int32#12,<r2=int32#5,<r4=int32#7
# asm 2: and >t2=r11,<r2=r4,<r4=r6
and r11,r4,r6

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t2 = r3 & r4
# asm 1: and >t2=int32#12,<r3=int32#6,<r4=int32#7
# asm 2: and >t2=r11,<r3=r5,<r4=r6
and r11,r5,r6

# qhasm: t3 = r2 ^ (r2 >>> 11)
# asm 1: eor >t3=int32#13,<r2=int32#5,<r2=int32#5,ROR #11
# asm 2: eor >t3=r12,<r2=r4,<r2=r4,ROR #11
eor r12,r4,r4,ROR #11

# qhasm: t3 ^= (r2 >>> 20)
# asm 1: eor >t3=int32#13,<t3=int32#13,<r2=int32#5,ROR #20
# asm 2: eor >t3=r12,<t3=r12,<r2=r4,ROR #20
eor r12,r12,r4,ROR #20

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t0 += (t3 >>> 2)
# asm 1: add >t0=int32#11,<t0=int32#11,<t3=int32#13,ROR #2
# asm 2: add >t0=r10,<t0=r10,<t3=r12,ROR #2
add r10,r10,r12,ROR #2

# qhasm: r1 += t0
# asm 1: add >r1=int32#3,<r1=int32#3,<t0=int32#11
# asm 2: add >r1=r2,<r1=r2,<t0=r10
add r2,r2,r10

# qhasm: w7 = w7_stack
# asm 1: ldr >w7=int32#11,<w7_stack=stack32#28
# asm 2: ldr >w7=r10,<w7_stack=[sp,#108]
ldr r10,[sp,#108]

# qhasm: t0 = r5 & r6
# asm 1: and >t0=int32#12,<r5=int32#8,<r6=int32#9
# asm 2: and >t0=r11,<r5=r7,<r6=r8
and r11,r7,r8

# qhasm: t1 = r7 & ~r5
# asm 1: bic >t1=int32#13,<r7=int32#10,<r5=int32#8
# asm 2: bic >t1=r12,<r7=r9,<r5=r7
bic r12,r9,r7

# qhasm: t0 ^= t1
# asm 1: eor >t0=int32#12,<t0=int32#12,<t1=int32#13
# asm 2: eor >t0=r11,<t0=r11,<t1=r12
eor r11,r11,r12

# qhasm: t1 = r5 ^ (r5 >>> 5)
# asm 1: eor >t1=int32#13,<r5=int32#8,<r5=int32#8,ROR #5
# asm 2: eor >t1=r12,<r5=r7,<r5=r7,ROR #5
eor r12,r7,r7,ROR #5

# qhasm: t1 ^= (r5 >>> 19)
# asm 1: eor >t1=int32#13,<t1=int32#13,<r5=int32#8,ROR #19
# asm 2: eor >t1=r12,<t1=r12,<r5=r7,ROR #19
eor r12,r12,r7,ROR #19

# qhasm: t2 = mem32[input_3 + 156]
# asm 1: ldr >t2=int32#14,[<input_3=int32#4,#156]
# asm 2: ldr >t2=r14,[<input_3=r3,#156]
ldr r14,[r3,#156]

# qhasm: t0 += w7
# asm 1: add >t0=int32#11,<t0=int32#12,<w7=int32#11
# asm 2: add >t0=r10,<t0=r11,<w7=r10
add r10,r11,r10

# qhasm: t0 += (t1 >>> 6)
# asm 1: add >t0=int32#11,<t0=int32#11,<t1=int32#13,ROR #6
# asm 2: add >t0=r10,<t0=r10,<t1=r12,ROR #6
add r10,r10,r12,ROR #6

# qhasm: t0 += t2
# asm 1: add >t0=int32#11,<t0=int32#11,<t2=int32#14
# asm 2: add >t0=r10,<t0=r10,<t2=r14
add r10,r10,r14

# qhasm: r0 += t0
# asm 1: add >r0=int32#1,<r0=int32#1,<t0=int32#11
# asm 2: add >r0=r0,<r0=r0,<t0=r10
add r0,r0,r10

# qhasm: r4 += r0
# asm 1: add >r4=int32#7,<r4=int32#7,<r0=int32#1
# asm 2: add >r4=r6,<r4=r6,<r0=r0
add r6,r6,r0

# qhasm: t0 = r1 & r2
# asm 1: and >t0=int32#11,<r1=int32#3,<r2=int32#5
# asm 2: and >t0=r10,<r1=r2,<r2=r4
and r10,r2,r4

# qhasm: t2 = r1 & r3
# asm 1: and >t2=int32#12,<r1=int32#3,<r3=int32#6
# asm 2: and >t2=r11,<r1=r2,<r3=r5
and r11,r2,r5

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t2 = r2 & r3
# asm 1: and >t2=int32#12,<r2=int32#5,<r3=int32#6
# asm 2: and >t2=r11,<r2=r4,<r3=r5
and r11,r4,r5

# qhasm: t3 = r1 ^ (r1 >>> 11)
# asm 1: eor >t3=int32#13,<r1=int32#3,<r1=int32#3,ROR #11
# asm 2: eor >t3=r12,<r1=r2,<r1=r2,ROR #11
eor r12,r2,r2,ROR #11

# qhasm: t3 ^= (r1 >>> 20)
# asm 1: eor >t3=int32#13,<t3=int32#13,<r1=int32#3,ROR #20
# asm 2: eor >t3=r12,<t3=r12,<r1=r2,ROR #20
eor r12,r12,r2,ROR #20

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t0 += (t3 >>> 2)
# asm 1: add >t0=int32#11,<t0=int32#11,<t3=int32#13,ROR #2
# asm 2: add >t0=r10,<t0=r10,<t3=r12,ROR #2
add r10,r10,r12,ROR #2

# qhasm: r0 += t0
# asm 1: add >r0=int32#1,<r0=int32#1,<t0=int32#11
# asm 2: add >r0=r0,<r0=r0,<t0=r10
add r0,r0,r10

# qhasm: w8 = w8_stack
# asm 1: ldr >w8=int32#11,<w8_stack=stack32#29
# asm 2: ldr >w8=r10,<w8_stack=[sp,#112]
ldr r10,[sp,#112]

# qhasm: t0 = r4 & r5
# asm 1: and >t0=int32#12,<r4=int32#7,<r5=int32#8
# asm 2: and >t0=r11,<r4=r6,<r5=r7
and r11,r6,r7

# qhasm: t1 = r6 & ~r4
# asm 1: bic >t1=int32#13,<r6=int32#9,<r4=int32#7
# asm 2: bic >t1=r12,<r6=r8,<r4=r6
bic r12,r8,r6

# qhasm: t0 ^= t1
# asm 1: eor >t0=int32#12,<t0=int32#12,<t1=int32#13
# asm 2: eor >t0=r11,<t0=r11,<t1=r12
eor r11,r11,r12

# qhasm: t1 = r4 ^ (r4 >>> 5)
# asm 1: eor >t1=int32#13,<r4=int32#7,<r4=int32#7,ROR #5
# asm 2: eor >t1=r12,<r4=r6,<r4=r6,ROR #5
eor r12,r6,r6,ROR #5

# qhasm: t1 ^= (r4 >>> 19)
# asm 1: eor >t1=int32#13,<t1=int32#13,<r4=int32#7,ROR #19
# asm 2: eor >t1=r12,<t1=r12,<r4=r6,ROR #19
eor r12,r12,r6,ROR #19

# qhasm: t2 = mem32[input_3 + 160]
# asm 1: ldr >t2=int32#14,[<input_3=int32#4,#160]
# asm 2: ldr >t2=r14,[<input_3=r3,#160]
ldr r14,[r3,#160]

# qhasm: t0 += w8
# asm 1: add >t0=int32#11,<t0=int32#12,<w8=int32#11
# asm 2: add >t0=r10,<t0=r11,<w8=r10
add r10,r11,r10

# qhasm: t0 += (t1 >>> 6)
# asm 1: add >t0=int32#11,<t0=int32#11,<t1=int32#13,ROR #6
# asm 2: add >t0=r10,<t0=r10,<t1=r12,ROR #6
add r10,r10,r12,ROR #6

# qhasm: t0 += t2
# asm 1: add >t0=int32#11,<t0=int32#11,<t2=int32#14
# asm 2: add >t0=r10,<t0=r10,<t2=r14
add r10,r10,r14

# qhasm: r7 += t0
# asm 1: add >r7=int32#10,<r7=int32#10,<t0=int32#11
# asm 2: add >r7=r9,<r7=r9,<t0=r10
add r9,r9,r10

# qhasm: r3 += r7
# asm 1: add >r3=int32#6,<r3=int32#6,<r7=int32#10
# asm 2: add >r3=r5,<r3=r5,<r7=r9
add r5,r5,r9

# qhasm: t0 = r0 & r1
# asm 1: and >t0=int32#11,<r0=int32#1,<r1=int32#3
# asm 2: and >t0=r10,<r0=r0,<r1=r2
and r10,r0,r2

# qhasm: t2 = r0 & r2
# asm 1: and >t2=int32#12,<r0=int32#1,<r2=int32#5
# asm 2: and >t2=r11,<r0=r0,<r2=r4
and r11,r0,r4

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t2 = r1 & r2
# asm 1: and >t2=int32#12,<r1=int32#3,<r2=int32#5
# asm 2: and >t2=r11,<r1=r2,<r2=r4
and r11,r2,r4

# qhasm: t3 = r0 ^ (r0 >>> 11)
# asm 1: eor >t3=int32#13,<r0=int32#1,<r0=int32#1,ROR #11
# asm 2: eor >t3=r12,<r0=r0,<r0=r0,ROR #11
eor r12,r0,r0,ROR #11

# qhasm: t3 ^= (r0 >>> 20)
# asm 1: eor >t3=int32#13,<t3=int32#13,<r0=int32#1,ROR #20
# asm 2: eor >t3=r12,<t3=r12,<r0=r0,ROR #20
eor r12,r12,r0,ROR #20

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t0 += (t3 >>> 2)
# asm 1: add >t0=int32#11,<t0=int32#11,<t3=int32#13,ROR #2
# asm 2: add >t0=r10,<t0=r10,<t3=r12,ROR #2
add r10,r10,r12,ROR #2

# qhasm: r7 += t0
# asm 1: add >r7=int32#10,<r7=int32#10,<t0=int32#11
# asm 2: add >r7=r9,<r7=r9,<t0=r10
add r9,r9,r10

# qhasm: w9 = w9_stack
# asm 1: ldr >w9=int32#11,<w9_stack=stack32#20
# asm 2: ldr >w9=r10,<w9_stack=[sp,#76]
ldr r10,[sp,#76]

# qhasm: t0 = r3 & r4
# asm 1: and >t0=int32#12,<r3=int32#6,<r4=int32#7
# asm 2: and >t0=r11,<r3=r5,<r4=r6
and r11,r5,r6

# qhasm: t1 = r5 & ~r3
# asm 1: bic >t1=int32#13,<r5=int32#8,<r3=int32#6
# asm 2: bic >t1=r12,<r5=r7,<r3=r5
bic r12,r7,r5

# qhasm: t0 ^= t1
# asm 1: eor >t0=int32#12,<t0=int32#12,<t1=int32#13
# asm 2: eor >t0=r11,<t0=r11,<t1=r12
eor r11,r11,r12

# qhasm: t1 = r3 ^ (r3 >>> 5)
# asm 1: eor >t1=int32#13,<r3=int32#6,<r3=int32#6,ROR #5
# asm 2: eor >t1=r12,<r3=r5,<r3=r5,ROR #5
eor r12,r5,r5,ROR #5

# qhasm: t1 ^= (r3 >>> 19)
# asm 1: eor >t1=int32#13,<t1=int32#13,<r3=int32#6,ROR #19
# asm 2: eor >t1=r12,<t1=r12,<r3=r5,ROR #19
eor r12,r12,r5,ROR #19

# qhasm: t2 = mem32[input_3 + 164]
# asm 1: ldr >t2=int32#14,[<input_3=int32#4,#164]
# asm 2: ldr >t2=r14,[<input_3=r3,#164]
ldr r14,[r3,#164]

# qhasm: t0 += w9
# asm 1: add >t0=int32#11,<t0=int32#12,<w9=int32#11
# asm 2: add >t0=r10,<t0=r11,<w9=r10
add r10,r11,r10

# qhasm: t0 += (t1 >>> 6)
# asm 1: add >t0=int32#11,<t0=int32#11,<t1=int32#13,ROR #6
# asm 2: add >t0=r10,<t0=r10,<t1=r12,ROR #6
add r10,r10,r12,ROR #6

# qhasm: t0 += t2
# asm 1: add >t0=int32#11,<t0=int32#11,<t2=int32#14
# asm 2: add >t0=r10,<t0=r10,<t2=r14
add r10,r10,r14

# qhasm: r6 += t0
# asm 1: add >r6=int32#9,<r6=int32#9,<t0=int32#11
# asm 2: add >r6=r8,<r6=r8,<t0=r10
add r8,r8,r10

# qhasm: r2 += r6
# asm 1: add >r2=int32#5,<r2=int32#5,<r6=int32#9
# asm 2: add >r2=r4,<r2=r4,<r6=r8
add r4,r4,r8

# qhasm: t0 = r7 & r0
# asm 1: and >t0=int32#11,<r7=int32#10,<r0=int32#1
# asm 2: and >t0=r10,<r7=r9,<r0=r0
and r10,r9,r0

# qhasm: t2 = r7 & r1
# asm 1: and >t2=int32#12,<r7=int32#10,<r1=int32#3
# asm 2: and >t2=r11,<r7=r9,<r1=r2
and r11,r9,r2

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t2 = r0 & r1
# asm 1: and >t2=int32#12,<r0=int32#1,<r1=int32#3
# asm 2: and >t2=r11,<r0=r0,<r1=r2
and r11,r0,r2

# qhasm: t3 = r7 ^ (r7 >>> 11)
# asm 1: eor >t3=int32#13,<r7=int32#10,<r7=int32#10,ROR #11
# asm 2: eor >t3=r12,<r7=r9,<r7=r9,ROR #11
eor r12,r9,r9,ROR #11

# qhasm: t3 ^= (r7 >>> 20)
# asm 1: eor >t3=int32#13,<t3=int32#13,<r7=int32#10,ROR #20
# asm 2: eor >t3=r12,<t3=r12,<r7=r9,ROR #20
eor r12,r12,r9,ROR #20

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t0 += (t3 >>> 2)
# asm 1: add >t0=int32#11,<t0=int32#11,<t3=int32#13,ROR #2
# asm 2: add >t0=r10,<t0=r10,<t3=r12,ROR #2
add r10,r10,r12,ROR #2

# qhasm: r6 += t0
# asm 1: add >r6=int32#9,<r6=int32#9,<t0=int32#11
# asm 2: add >r6=r8,<r6=r8,<t0=r10
add r8,r8,r10

# qhasm: w10 = w10_stack
# asm 1: ldr >w10=int32#11,<w10_stack=stack32#30
# asm 2: ldr >w10=r10,<w10_stack=[sp,#116]
ldr r10,[sp,#116]

# qhasm: t0 = r2 & r3
# asm 1: and >t0=int32#12,<r2=int32#5,<r3=int32#6
# asm 2: and >t0=r11,<r2=r4,<r3=r5
and r11,r4,r5

# qhasm: t1 = r4 & ~r2
# asm 1: bic >t1=int32#13,<r4=int32#7,<r2=int32#5
# asm 2: bic >t1=r12,<r4=r6,<r2=r4
bic r12,r6,r4

# qhasm: t0 ^= t1
# asm 1: eor >t0=int32#12,<t0=int32#12,<t1=int32#13
# asm 2: eor >t0=r11,<t0=r11,<t1=r12
eor r11,r11,r12

# qhasm: t1 = r2 ^ (r2 >>> 5)
# asm 1: eor >t1=int32#13,<r2=int32#5,<r2=int32#5,ROR #5
# asm 2: eor >t1=r12,<r2=r4,<r2=r4,ROR #5
eor r12,r4,r4,ROR #5

# qhasm: t1 ^= (r2 >>> 19)
# asm 1: eor >t1=int32#13,<t1=int32#13,<r2=int32#5,ROR #19
# asm 2: eor >t1=r12,<t1=r12,<r2=r4,ROR #19
eor r12,r12,r4,ROR #19

# qhasm: t2 = mem32[input_3 + 168]
# asm 1: ldr >t2=int32#14,[<input_3=int32#4,#168]
# asm 2: ldr >t2=r14,[<input_3=r3,#168]
ldr r14,[r3,#168]

# qhasm: t0 += w10
# asm 1: add >t0=int32#11,<t0=int32#12,<w10=int32#11
# asm 2: add >t0=r10,<t0=r11,<w10=r10
add r10,r11,r10

# qhasm: t0 += (t1 >>> 6)
# asm 1: add >t0=int32#11,<t0=int32#11,<t1=int32#13,ROR #6
# asm 2: add >t0=r10,<t0=r10,<t1=r12,ROR #6
add r10,r10,r12,ROR #6

# qhasm: t0 += t2
# asm 1: add >t0=int32#11,<t0=int32#11,<t2=int32#14
# asm 2: add >t0=r10,<t0=r10,<t2=r14
add r10,r10,r14

# qhasm: r5 += t0
# asm 1: add >r5=int32#8,<r5=int32#8,<t0=int32#11
# asm 2: add >r5=r7,<r5=r7,<t0=r10
add r7,r7,r10

# qhasm: r1 += r5
# asm 1: add >r1=int32#3,<r1=int32#3,<r5=int32#8
# asm 2: add >r1=r2,<r1=r2,<r5=r7
add r2,r2,r7

# qhasm: t0 = r6 & r7
# asm 1: and >t0=int32#11,<r6=int32#9,<r7=int32#10
# asm 2: and >t0=r10,<r6=r8,<r7=r9
and r10,r8,r9

# qhasm: t2 = r6 & r0
# asm 1: and >t2=int32#12,<r6=int32#9,<r0=int32#1
# asm 2: and >t2=r11,<r6=r8,<r0=r0
and r11,r8,r0

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t2 = r7 & r0
# asm 1: and >t2=int32#12,<r7=int32#10,<r0=int32#1
# asm 2: and >t2=r11,<r7=r9,<r0=r0
and r11,r9,r0

# qhasm: t3 = r6 ^ (r6 >>> 11)
# asm 1: eor >t3=int32#13,<r6=int32#9,<r6=int32#9,ROR #11
# asm 2: eor >t3=r12,<r6=r8,<r6=r8,ROR #11
eor r12,r8,r8,ROR #11

# qhasm: t3 ^= (r6 >>> 20)
# asm 1: eor >t3=int32#13,<t3=int32#13,<r6=int32#9,ROR #20
# asm 2: eor >t3=r12,<t3=r12,<r6=r8,ROR #20
eor r12,r12,r8,ROR #20

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t0 += (t3 >>> 2)
# asm 1: add >t0=int32#11,<t0=int32#11,<t3=int32#13,ROR #2
# asm 2: add >t0=r10,<t0=r10,<t3=r12,ROR #2
add r10,r10,r12,ROR #2

# qhasm: r5 += t0
# asm 1: add >r5=int32#8,<r5=int32#8,<t0=int32#11
# asm 2: add >r5=r7,<r5=r7,<t0=r10
add r7,r7,r10

# qhasm: w11 = w11_stack
# asm 1: ldr >w11=int32#11,<w11_stack=stack32#31
# asm 2: ldr >w11=r10,<w11_stack=[sp,#120]
ldr r10,[sp,#120]

# qhasm: t0 = r1 & r2
# asm 1: and >t0=int32#12,<r1=int32#3,<r2=int32#5
# asm 2: and >t0=r11,<r1=r2,<r2=r4
and r11,r2,r4

# qhasm: t1 = r3 & ~r1
# asm 1: bic >t1=int32#13,<r3=int32#6,<r1=int32#3
# asm 2: bic >t1=r12,<r3=r5,<r1=r2
bic r12,r5,r2

# qhasm: t0 ^= t1
# asm 1: eor >t0=int32#12,<t0=int32#12,<t1=int32#13
# asm 2: eor >t0=r11,<t0=r11,<t1=r12
eor r11,r11,r12

# qhasm: t1 = r1 ^ (r1 >>> 5)
# asm 1: eor >t1=int32#13,<r1=int32#3,<r1=int32#3,ROR #5
# asm 2: eor >t1=r12,<r1=r2,<r1=r2,ROR #5
eor r12,r2,r2,ROR #5

# qhasm: t1 ^= (r1 >>> 19)
# asm 1: eor >t1=int32#13,<t1=int32#13,<r1=int32#3,ROR #19
# asm 2: eor >t1=r12,<t1=r12,<r1=r2,ROR #19
eor r12,r12,r2,ROR #19

# qhasm: t2 = mem32[input_3 + 172]
# asm 1: ldr >t2=int32#14,[<input_3=int32#4,#172]
# asm 2: ldr >t2=r14,[<input_3=r3,#172]
ldr r14,[r3,#172]

# qhasm: t0 += w11
# asm 1: add >t0=int32#11,<t0=int32#12,<w11=int32#11
# asm 2: add >t0=r10,<t0=r11,<w11=r10
add r10,r11,r10

# qhasm: t0 += (t1 >>> 6)
# asm 1: add >t0=int32#11,<t0=int32#11,<t1=int32#13,ROR #6
# asm 2: add >t0=r10,<t0=r10,<t1=r12,ROR #6
add r10,r10,r12,ROR #6

# qhasm: t0 += t2
# asm 1: add >t0=int32#11,<t0=int32#11,<t2=int32#14
# asm 2: add >t0=r10,<t0=r10,<t2=r14
add r10,r10,r14

# qhasm: r4 += t0
# asm 1: add >r4=int32#7,<r4=int32#7,<t0=int32#11
# asm 2: add >r4=r6,<r4=r6,<t0=r10
add r6,r6,r10

# qhasm: r0 += r4
# asm 1: add >r0=int32#1,<r0=int32#1,<r4=int32#7
# asm 2: add >r0=r0,<r0=r0,<r4=r6
add r0,r0,r6

# qhasm: t0 = r5 & r6
# asm 1: and >t0=int32#11,<r5=int32#8,<r6=int32#9
# asm 2: and >t0=r10,<r5=r7,<r6=r8
and r10,r7,r8

# qhasm: t2 = r5 & r7
# asm 1: and >t2=int32#12,<r5=int32#8,<r7=int32#10
# asm 2: and >t2=r11,<r5=r7,<r7=r9
and r11,r7,r9

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t2 = r6 & r7
# asm 1: and >t2=int32#12,<r6=int32#9,<r7=int32#10
# asm 2: and >t2=r11,<r6=r8,<r7=r9
and r11,r8,r9

# qhasm: t3 = r5 ^ (r5 >>> 11)
# asm 1: eor >t3=int32#13,<r5=int32#8,<r5=int32#8,ROR #11
# asm 2: eor >t3=r12,<r5=r7,<r5=r7,ROR #11
eor r12,r7,r7,ROR #11

# qhasm: t3 ^= (r5 >>> 20)
# asm 1: eor >t3=int32#13,<t3=int32#13,<r5=int32#8,ROR #20
# asm 2: eor >t3=r12,<t3=r12,<r5=r7,ROR #20
eor r12,r12,r7,ROR #20

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t0 += (t3 >>> 2)
# asm 1: add >t0=int32#11,<t0=int32#11,<t3=int32#13,ROR #2
# asm 2: add >t0=r10,<t0=r10,<t3=r12,ROR #2
add r10,r10,r12,ROR #2

# qhasm: r4 += t0
# asm 1: add >r4=int32#7,<r4=int32#7,<t0=int32#11
# asm 2: add >r4=r6,<r4=r6,<t0=r10
add r6,r6,r10

# qhasm: w12 = w12_stack
# asm 1: ldr >w12=int32#11,<w12_stack=stack32#32
# asm 2: ldr >w12=r10,<w12_stack=[sp,#124]
ldr r10,[sp,#124]

# qhasm: t0 = r0 & r1
# asm 1: and >t0=int32#12,<r0=int32#1,<r1=int32#3
# asm 2: and >t0=r11,<r0=r0,<r1=r2
and r11,r0,r2

# qhasm: t1 = r2 & ~r0
# asm 1: bic >t1=int32#13,<r2=int32#5,<r0=int32#1
# asm 2: bic >t1=r12,<r2=r4,<r0=r0
bic r12,r4,r0

# qhasm: t0 ^= t1
# asm 1: eor >t0=int32#12,<t0=int32#12,<t1=int32#13
# asm 2: eor >t0=r11,<t0=r11,<t1=r12
eor r11,r11,r12

# qhasm: t1 = r0 ^ (r0 >>> 5)
# asm 1: eor >t1=int32#13,<r0=int32#1,<r0=int32#1,ROR #5
# asm 2: eor >t1=r12,<r0=r0,<r0=r0,ROR #5
eor r12,r0,r0,ROR #5

# qhasm: t1 ^= (r0 >>> 19)
# asm 1: eor >t1=int32#13,<t1=int32#13,<r0=int32#1,ROR #19
# asm 2: eor >t1=r12,<t1=r12,<r0=r0,ROR #19
eor r12,r12,r0,ROR #19

# qhasm: t2 = mem32[input_3 + 176]
# asm 1: ldr >t2=int32#14,[<input_3=int32#4,#176]
# asm 2: ldr >t2=r14,[<input_3=r3,#176]
ldr r14,[r3,#176]

# qhasm: t0 += w12
# asm 1: add >t0=int32#11,<t0=int32#12,<w12=int32#11
# asm 2: add >t0=r10,<t0=r11,<w12=r10
add r10,r11,r10

# qhasm: t0 += (t1 >>> 6)
# asm 1: add >t0=int32#11,<t0=int32#11,<t1=int32#13,ROR #6
# asm 2: add >t0=r10,<t0=r10,<t1=r12,ROR #6
add r10,r10,r12,ROR #6

# qhasm: t0 += t2
# asm 1: add >t0=int32#11,<t0=int32#11,<t2=int32#14
# asm 2: add >t0=r10,<t0=r10,<t2=r14
add r10,r10,r14

# qhasm: r3 += t0
# asm 1: add >r3=int32#6,<r3=int32#6,<t0=int32#11
# asm 2: add >r3=r5,<r3=r5,<t0=r10
add r5,r5,r10

# qhasm: r7 += r3
# asm 1: add >r7=int32#10,<r7=int32#10,<r3=int32#6
# asm 2: add >r7=r9,<r7=r9,<r3=r5
add r9,r9,r5

# qhasm: t0 = r4 & r5
# asm 1: and >t0=int32#11,<r4=int32#7,<r5=int32#8
# asm 2: and >t0=r10,<r4=r6,<r5=r7
and r10,r6,r7

# qhasm: t2 = r4 & r6
# asm 1: and >t2=int32#12,<r4=int32#7,<r6=int32#9
# asm 2: and >t2=r11,<r4=r6,<r6=r8
and r11,r6,r8

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t2 = r5 & r6
# asm 1: and >t2=int32#12,<r5=int32#8,<r6=int32#9
# asm 2: and >t2=r11,<r5=r7,<r6=r8
and r11,r7,r8

# qhasm: t3 = r4 ^ (r4 >>> 11)
# asm 1: eor >t3=int32#13,<r4=int32#7,<r4=int32#7,ROR #11
# asm 2: eor >t3=r12,<r4=r6,<r4=r6,ROR #11
eor r12,r6,r6,ROR #11

# qhasm: t3 ^= (r4 >>> 20)
# asm 1: eor >t3=int32#13,<t3=int32#13,<r4=int32#7,ROR #20
# asm 2: eor >t3=r12,<t3=r12,<r4=r6,ROR #20
eor r12,r12,r6,ROR #20

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t0 += (t3 >>> 2)
# asm 1: add >t0=int32#11,<t0=int32#11,<t3=int32#13,ROR #2
# asm 2: add >t0=r10,<t0=r10,<t3=r12,ROR #2
add r10,r10,r12,ROR #2

# qhasm: r3 += t0
# asm 1: add >r3=int32#6,<r3=int32#6,<t0=int32#11
# asm 2: add >r3=r5,<r3=r5,<t0=r10
add r5,r5,r10

# qhasm: w13 = w13_stack
# asm 1: ldr >w13=int32#11,<w13_stack=stack32#33
# asm 2: ldr >w13=r10,<w13_stack=[sp,#128]
ldr r10,[sp,#128]

# qhasm: t0 = r7 & r0
# asm 1: and >t0=int32#12,<r7=int32#10,<r0=int32#1
# asm 2: and >t0=r11,<r7=r9,<r0=r0
and r11,r9,r0

# qhasm: t1 = r1 & ~r7
# asm 1: bic >t1=int32#13,<r1=int32#3,<r7=int32#10
# asm 2: bic >t1=r12,<r1=r2,<r7=r9
bic r12,r2,r9

# qhasm: t0 ^= t1
# asm 1: eor >t0=int32#12,<t0=int32#12,<t1=int32#13
# asm 2: eor >t0=r11,<t0=r11,<t1=r12
eor r11,r11,r12

# qhasm: t1 = r7 ^ (r7 >>> 5)
# asm 1: eor >t1=int32#13,<r7=int32#10,<r7=int32#10,ROR #5
# asm 2: eor >t1=r12,<r7=r9,<r7=r9,ROR #5
eor r12,r9,r9,ROR #5

# qhasm: t1 ^= (r7 >>> 19)
# asm 1: eor >t1=int32#13,<t1=int32#13,<r7=int32#10,ROR #19
# asm 2: eor >t1=r12,<t1=r12,<r7=r9,ROR #19
eor r12,r12,r9,ROR #19

# qhasm: t2 = mem32[input_3 + 180]
# asm 1: ldr >t2=int32#14,[<input_3=int32#4,#180]
# asm 2: ldr >t2=r14,[<input_3=r3,#180]
ldr r14,[r3,#180]

# qhasm: t0 += w13
# asm 1: add >t0=int32#11,<t0=int32#12,<w13=int32#11
# asm 2: add >t0=r10,<t0=r11,<w13=r10
add r10,r11,r10

# qhasm: t0 += (t1 >>> 6)
# asm 1: add >t0=int32#11,<t0=int32#11,<t1=int32#13,ROR #6
# asm 2: add >t0=r10,<t0=r10,<t1=r12,ROR #6
add r10,r10,r12,ROR #6

# qhasm: t0 += t2
# asm 1: add >t0=int32#11,<t0=int32#11,<t2=int32#14
# asm 2: add >t0=r10,<t0=r10,<t2=r14
add r10,r10,r14

# qhasm: r2 += t0
# asm 1: add >r2=int32#5,<r2=int32#5,<t0=int32#11
# asm 2: add >r2=r4,<r2=r4,<t0=r10
add r4,r4,r10

# qhasm: r6 += r2
# asm 1: add >r6=int32#9,<r6=int32#9,<r2=int32#5
# asm 2: add >r6=r8,<r6=r8,<r2=r4
add r8,r8,r4

# qhasm: t0 = r3 & r4
# asm 1: and >t0=int32#11,<r3=int32#6,<r4=int32#7
# asm 2: and >t0=r10,<r3=r5,<r4=r6
and r10,r5,r6

# qhasm: t2 = r3 & r5
# asm 1: and >t2=int32#12,<r3=int32#6,<r5=int32#8
# asm 2: and >t2=r11,<r3=r5,<r5=r7
and r11,r5,r7

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t2 = r4 & r5
# asm 1: and >t2=int32#12,<r4=int32#7,<r5=int32#8
# asm 2: and >t2=r11,<r4=r6,<r5=r7
and r11,r6,r7

# qhasm: t3 = r3 ^ (r3 >>> 11)
# asm 1: eor >t3=int32#13,<r3=int32#6,<r3=int32#6,ROR #11
# asm 2: eor >t3=r12,<r3=r5,<r3=r5,ROR #11
eor r12,r5,r5,ROR #11

# qhasm: t3 ^= (r3 >>> 20)
# asm 1: eor >t3=int32#13,<t3=int32#13,<r3=int32#6,ROR #20
# asm 2: eor >t3=r12,<t3=r12,<r3=r5,ROR #20
eor r12,r12,r5,ROR #20

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t0 += (t3 >>> 2)
# asm 1: add >t0=int32#11,<t0=int32#11,<t3=int32#13,ROR #2
# asm 2: add >t0=r10,<t0=r10,<t3=r12,ROR #2
add r10,r10,r12,ROR #2

# qhasm: r2 += t0
# asm 1: add >r2=int32#5,<r2=int32#5,<t0=int32#11
# asm 2: add >r2=r4,<r2=r4,<t0=r10
add r4,r4,r10

# qhasm: w14 = w14_stack
# asm 1: ldr >w14=int32#11,<w14_stack=stack32#34
# asm 2: ldr >w14=r10,<w14_stack=[sp,#132]
ldr r10,[sp,#132]

# qhasm: t0 = r6 & r7
# asm 1: and >t0=int32#12,<r6=int32#9,<r7=int32#10
# asm 2: and >t0=r11,<r6=r8,<r7=r9
and r11,r8,r9

# qhasm: t1 = r0 & ~r6
# asm 1: bic >t1=int32#13,<r0=int32#1,<r6=int32#9
# asm 2: bic >t1=r12,<r0=r0,<r6=r8
bic r12,r0,r8

# qhasm: t0 ^= t1
# asm 1: eor >t0=int32#12,<t0=int32#12,<t1=int32#13
# asm 2: eor >t0=r11,<t0=r11,<t1=r12
eor r11,r11,r12

# qhasm: t1 = r6 ^ (r6 >>> 5)
# asm 1: eor >t1=int32#13,<r6=int32#9,<r6=int32#9,ROR #5
# asm 2: eor >t1=r12,<r6=r8,<r6=r8,ROR #5
eor r12,r8,r8,ROR #5

# qhasm: t1 ^= (r6 >>> 19)
# asm 1: eor >t1=int32#13,<t1=int32#13,<r6=int32#9,ROR #19
# asm 2: eor >t1=r12,<t1=r12,<r6=r8,ROR #19
eor r12,r12,r8,ROR #19

# qhasm: t2 = mem32[input_3 + 184]
# asm 1: ldr >t2=int32#14,[<input_3=int32#4,#184]
# asm 2: ldr >t2=r14,[<input_3=r3,#184]
ldr r14,[r3,#184]

# qhasm: t0 += w14
# asm 1: add >t0=int32#11,<t0=int32#12,<w14=int32#11
# asm 2: add >t0=r10,<t0=r11,<w14=r10
add r10,r11,r10

# qhasm: t0 += (t1 >>> 6)
# asm 1: add >t0=int32#11,<t0=int32#11,<t1=int32#13,ROR #6
# asm 2: add >t0=r10,<t0=r10,<t1=r12,ROR #6
add r10,r10,r12,ROR #6

# qhasm: t0 += t2
# asm 1: add >t0=int32#11,<t0=int32#11,<t2=int32#14
# asm 2: add >t0=r10,<t0=r10,<t2=r14
add r10,r10,r14

# qhasm: r1 += t0
# asm 1: add >r1=int32#3,<r1=int32#3,<t0=int32#11
# asm 2: add >r1=r2,<r1=r2,<t0=r10
add r2,r2,r10

# qhasm: r5 += r1
# asm 1: add >r5=int32#8,<r5=int32#8,<r1=int32#3
# asm 2: add >r5=r7,<r5=r7,<r1=r2
add r7,r7,r2

# qhasm: t0 = r2 & r3
# asm 1: and >t0=int32#11,<r2=int32#5,<r3=int32#6
# asm 2: and >t0=r10,<r2=r4,<r3=r5
and r10,r4,r5

# qhasm: t2 = r2 & r4
# asm 1: and >t2=int32#12,<r2=int32#5,<r4=int32#7
# asm 2: and >t2=r11,<r2=r4,<r4=r6
and r11,r4,r6

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t2 = r3 & r4
# asm 1: and >t2=int32#12,<r3=int32#6,<r4=int32#7
# asm 2: and >t2=r11,<r3=r5,<r4=r6
and r11,r5,r6

# qhasm: t3 = r2 ^ (r2 >>> 11)
# asm 1: eor >t3=int32#13,<r2=int32#5,<r2=int32#5,ROR #11
# asm 2: eor >t3=r12,<r2=r4,<r2=r4,ROR #11
eor r12,r4,r4,ROR #11

# qhasm: t3 ^= (r2 >>> 20)
# asm 1: eor >t3=int32#13,<t3=int32#13,<r2=int32#5,ROR #20
# asm 2: eor >t3=r12,<t3=r12,<r2=r4,ROR #20
eor r12,r12,r4,ROR #20

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t0 += (t3 >>> 2)
# asm 1: add >t0=int32#11,<t0=int32#11,<t3=int32#13,ROR #2
# asm 2: add >t0=r10,<t0=r10,<t3=r12,ROR #2
add r10,r10,r12,ROR #2

# qhasm: r1 += t0
# asm 1: add >r1=int32#3,<r1=int32#3,<t0=int32#11
# asm 2: add >r1=r2,<r1=r2,<t0=r10
add r2,r2,r10

# qhasm: w15 = w15_stack
# asm 1: ldr >w15=int32#11,<w15_stack=stack32#35
# asm 2: ldr >w15=r10,<w15_stack=[sp,#136]
ldr r10,[sp,#136]

# qhasm: t0 = r5 & r6
# asm 1: and >t0=int32#12,<r5=int32#8,<r6=int32#9
# asm 2: and >t0=r11,<r5=r7,<r6=r8
and r11,r7,r8

# qhasm: t1 = r7 & ~r5
# asm 1: bic >t1=int32#13,<r7=int32#10,<r5=int32#8
# asm 2: bic >t1=r12,<r7=r9,<r5=r7
bic r12,r9,r7

# qhasm: t0 ^= t1
# asm 1: eor >t0=int32#12,<t0=int32#12,<t1=int32#13
# asm 2: eor >t0=r11,<t0=r11,<t1=r12
eor r11,r11,r12

# qhasm: t1 = r5 ^ (r5 >>> 5)
# asm 1: eor >t1=int32#13,<r5=int32#8,<r5=int32#8,ROR #5
# asm 2: eor >t1=r12,<r5=r7,<r5=r7,ROR #5
eor r12,r7,r7,ROR #5

# qhasm: t1 ^= (r5 >>> 19)
# asm 1: eor >t1=int32#13,<t1=int32#13,<r5=int32#8,ROR #19
# asm 2: eor >t1=r12,<t1=r12,<r5=r7,ROR #19
eor r12,r12,r7,ROR #19

# qhasm: t2 = mem32[input_3 + 188]
# asm 1: ldr >t2=int32#14,[<input_3=int32#4,#188]
# asm 2: ldr >t2=r14,[<input_3=r3,#188]
ldr r14,[r3,#188]

# qhasm: t0 += w15
# asm 1: add >t0=int32#11,<t0=int32#12,<w15=int32#11
# asm 2: add >t0=r10,<t0=r11,<w15=r10
add r10,r11,r10

# qhasm: t0 += (t1 >>> 6)
# asm 1: add >t0=int32#11,<t0=int32#11,<t1=int32#13,ROR #6
# asm 2: add >t0=r10,<t0=r10,<t1=r12,ROR #6
add r10,r10,r12,ROR #6

# qhasm: t0 += t2
# asm 1: add >t0=int32#11,<t0=int32#11,<t2=int32#14
# asm 2: add >t0=r10,<t0=r10,<t2=r14
add r10,r10,r14

# qhasm: r0 += t0
# asm 1: add >r0=int32#1,<r0=int32#1,<t0=int32#11
# asm 2: add >r0=r0,<r0=r0,<t0=r10
add r0,r0,r10

# qhasm: r4 += r0
# asm 1: add >r4=int32#7,<r4=int32#7,<r0=int32#1
# asm 2: add >r4=r6,<r4=r6,<r0=r0
add r6,r6,r0

# qhasm: t0 = r1 & r2
# asm 1: and >t0=int32#11,<r1=int32#3,<r2=int32#5
# asm 2: and >t0=r10,<r1=r2,<r2=r4
and r10,r2,r4

# qhasm: t2 = r1 & r3
# asm 1: and >t2=int32#12,<r1=int32#3,<r3=int32#6
# asm 2: and >t2=r11,<r1=r2,<r3=r5
and r11,r2,r5

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t2 = r2 & r3
# asm 1: and >t2=int32#12,<r2=int32#5,<r3=int32#6
# asm 2: and >t2=r11,<r2=r4,<r3=r5
and r11,r4,r5

# qhasm: t3 = r1 ^ (r1 >>> 11)
# asm 1: eor >t3=int32#13,<r1=int32#3,<r1=int32#3,ROR #11
# asm 2: eor >t3=r12,<r1=r2,<r1=r2,ROR #11
eor r12,r2,r2,ROR #11

# qhasm: t3 ^= (r1 >>> 20)
# asm 1: eor >t3=int32#13,<t3=int32#13,<r1=int32#3,ROR #20
# asm 2: eor >t3=r12,<t3=r12,<r1=r2,ROR #20
eor r12,r12,r2,ROR #20

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t0 += (t3 >>> 2)
# asm 1: add >t0=int32#11,<t0=int32#11,<t3=int32#13,ROR #2
# asm 2: add >t0=r10,<t0=r10,<t3=r12,ROR #2
add r10,r10,r12,ROR #2

# qhasm: r0 += t0
# asm 1: add >r0=int32#1,<r0=int32#1,<t0=int32#11
# asm 2: add >r0=r0,<r0=r0,<t0=r10
add r0,r0,r10

# qhasm: r0_stack = r0
# asm 1: str <r0=int32#1,>r0_stack=stack32#36
# asm 2: str <r0=r0,>r0_stack=[sp,#140]
str r0,[sp,#140]

# qhasm: r1_stack = r1
# asm 1: str <r1=int32#3,>r1_stack=stack32#37
# asm 2: str <r1=r2,>r1_stack=[sp,#144]
str r2,[sp,#144]

# qhasm: r2_stack = r2
# asm 1: str <r2=int32#5,>r2_stack=stack32#38
# asm 2: str <r2=r4,>r2_stack=[sp,#148]
str r4,[sp,#148]

# qhasm: r3_stack = r3
# asm 1: str <r3=int32#6,>r3_stack=stack32#39
# asm 2: str <r3=r5,>r3_stack=[sp,#152]
str r5,[sp,#152]

# qhasm: r4_stack = r4
# asm 1: str <r4=int32#7,>r4_stack=stack32#40
# asm 2: str <r4=r6,>r4_stack=[sp,#156]
str r6,[sp,#156]

# qhasm: r5_stack = r5
# asm 1: str <r5=int32#8,>r5_stack=stack32#41
# asm 2: str <r5=r7,>r5_stack=[sp,#160]
str r7,[sp,#160]

# qhasm: r6_stack = r6
# asm 1: str <r6=int32#9,>r6_stack=stack32#42
# asm 2: str <r6=r8,>r6_stack=[sp,#164]
str r8,[sp,#164]

# qhasm: r7_stack = r7
# asm 1: str <r7=int32#10,>r7_stack=stack32#43
# asm 2: str <r7=r9,>r7_stack=[sp,#168]
str r9,[sp,#168]

# qhasm: w0 = w0_stack
# asm 1: ldr >w0=int32#1,<w0_stack=stack32#25
# asm 2: ldr >w0=r0,<w0_stack=[sp,#96]
ldr r0,[sp,#96]

# qhasm: w1 = w1_stack
# asm 1: ldr >w1=int32#3,<w1_stack=stack32#21
# asm 2: ldr >w1=r2,<w1_stack=[sp,#80]
ldr r2,[sp,#80]

# qhasm: w2 = w2_stack
# asm 1: ldr >w2=int32#5,<w2_stack=stack32#22
# asm 2: ldr >w2=r4,<w2_stack=[sp,#84]
ldr r4,[sp,#84]

# qhasm: w3 = w3_stack
# asm 1: ldr >w3=int32#6,<w3_stack=stack32#23
# asm 2: ldr >w3=r5,<w3_stack=[sp,#88]
ldr r5,[sp,#88]

# qhasm: w4 = w4_stack
# asm 1: ldr >w4=int32#7,<w4_stack=stack32#24
# asm 2: ldr >w4=r6,<w4_stack=[sp,#92]
ldr r6,[sp,#92]

# qhasm: w9 = w9_stack
# asm 1: ldr >w9=int32#8,<w9_stack=stack32#20
# asm 2: ldr >w9=r7,<w9_stack=[sp,#76]
ldr r7,[sp,#76]

# qhasm: w10 = w10_stack
# asm 1: ldr >w10=int32#9,<w10_stack=stack32#30
# asm 2: ldr >w10=r8,<w10_stack=[sp,#116]
ldr r8,[sp,#116]

# qhasm: w11 = w11_stack
# asm 1: ldr >w11=int32#10,<w11_stack=stack32#31
# asm 2: ldr >w11=r9,<w11_stack=[sp,#120]
ldr r9,[sp,#120]

# qhasm: w12 = w12_stack
# asm 1: ldr >w12=int32#11,<w12_stack=stack32#32
# asm 2: ldr >w12=r10,<w12_stack=[sp,#124]
ldr r10,[sp,#124]

# qhasm: w14 = w14_stack
# asm 1: ldr >w14=int32#12,<w14_stack=stack32#34
# asm 2: ldr >w14=r11,<w14_stack=[sp,#132]
ldr r11,[sp,#132]

# qhasm: w15 = w15_stack
# asm 1: ldr >w15=int32#13,<w15_stack=stack32#35
# asm 2: ldr >w15=r12,<w15_stack=[sp,#136]
ldr r12,[sp,#136]

# qhasm: w0 += w9
# asm 1: add >w0=int32#1,<w0=int32#1,<w9=int32#8
# asm 2: add >w0=r0,<w0=r0,<w9=r7
add r0,r0,r7

# qhasm: t0 = (w14 >>> 17)
# asm 1: mov >t0=int32#14,<w14=int32#12,ROR #17
# asm 2: mov >t0=r14,<w14=r11,ROR #17
mov r14,r11,ROR #17

# qhasm: t0 ^= (w14 >>> 19)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w14=int32#12,ROR #19
# asm 2: eor >t0=r14,<t0=r14,<w14=r11,ROR #19
eor r14,r14,r11,ROR #19

# qhasm: t0 ^= (w14 unsigned>> 10)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w14=int32#12,LSR #10
# asm 2: eor >t0=r14,<t0=r14,<w14=r11,LSR #10
eor r14,r14,r11,LSR #10

# qhasm: w0 += t0
# asm 1: add >w0=int32#1,<w0=int32#1,<t0=int32#14
# asm 2: add >w0=r0,<w0=r0,<t0=r14
add r0,r0,r14

# qhasm: t0 = (w1 >>> 7)
# asm 1: mov >t0=int32#14,<w1=int32#3,ROR #7
# asm 2: mov >t0=r14,<w1=r2,ROR #7
mov r14,r2,ROR #7

# qhasm: t0 ^= (w1 >>> 18)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w1=int32#3,ROR #18
# asm 2: eor >t0=r14,<t0=r14,<w1=r2,ROR #18
eor r14,r14,r2,ROR #18

# qhasm: t0 ^= (w1 unsigned>> 3)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w1=int32#3,LSR #3
# asm 2: eor >t0=r14,<t0=r14,<w1=r2,LSR #3
eor r14,r14,r2,LSR #3

# qhasm: w0 += t0
# asm 1: add >w0=int32#1,<w0=int32#1,<t0=int32#14
# asm 2: add >w0=r0,<w0=r0,<t0=r14
add r0,r0,r14

# qhasm: w1 += w10
# asm 1: add >w1=int32#3,<w1=int32#3,<w10=int32#9
# asm 2: add >w1=r2,<w1=r2,<w10=r8
add r2,r2,r8

# qhasm: t0 = (w15 >>> 17)
# asm 1: mov >t0=int32#14,<w15=int32#13,ROR #17
# asm 2: mov >t0=r14,<w15=r12,ROR #17
mov r14,r12,ROR #17

# qhasm: t0 ^= (w15 >>> 19)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w15=int32#13,ROR #19
# asm 2: eor >t0=r14,<t0=r14,<w15=r12,ROR #19
eor r14,r14,r12,ROR #19

# qhasm: t0 ^= (w15 unsigned>> 10)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w15=int32#13,LSR #10
# asm 2: eor >t0=r14,<t0=r14,<w15=r12,LSR #10
eor r14,r14,r12,LSR #10

# qhasm: w1 += t0
# asm 1: add >w1=int32#3,<w1=int32#3,<t0=int32#14
# asm 2: add >w1=r2,<w1=r2,<t0=r14
add r2,r2,r14

# qhasm: t0 = (w2 >>> 7)
# asm 1: mov >t0=int32#14,<w2=int32#5,ROR #7
# asm 2: mov >t0=r14,<w2=r4,ROR #7
mov r14,r4,ROR #7

# qhasm: t0 ^= (w2 >>> 18)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w2=int32#5,ROR #18
# asm 2: eor >t0=r14,<t0=r14,<w2=r4,ROR #18
eor r14,r14,r4,ROR #18

# qhasm: t0 ^= (w2 unsigned>> 3)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w2=int32#5,LSR #3
# asm 2: eor >t0=r14,<t0=r14,<w2=r4,LSR #3
eor r14,r14,r4,LSR #3

# qhasm: w1 += t0
# asm 1: add >w1=int32#3,<w1=int32#3,<t0=int32#14
# asm 2: add >w1=r2,<w1=r2,<t0=r14
add r2,r2,r14

# qhasm: w2 += w11
# asm 1: add >w2=int32#5,<w2=int32#5,<w11=int32#10
# asm 2: add >w2=r4,<w2=r4,<w11=r9
add r4,r4,r9

# qhasm: t0 = (w0 >>> 17)
# asm 1: mov >t0=int32#14,<w0=int32#1,ROR #17
# asm 2: mov >t0=r14,<w0=r0,ROR #17
mov r14,r0,ROR #17

# qhasm: t0 ^= (w0 >>> 19)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w0=int32#1,ROR #19
# asm 2: eor >t0=r14,<t0=r14,<w0=r0,ROR #19
eor r14,r14,r0,ROR #19

# qhasm: t0 ^= (w0 unsigned>> 10)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w0=int32#1,LSR #10
# asm 2: eor >t0=r14,<t0=r14,<w0=r0,LSR #10
eor r14,r14,r0,LSR #10

# qhasm: w2 += t0
# asm 1: add >w2=int32#5,<w2=int32#5,<t0=int32#14
# asm 2: add >w2=r4,<w2=r4,<t0=r14
add r4,r4,r14

# qhasm: t0 = (w3 >>> 7)
# asm 1: mov >t0=int32#14,<w3=int32#6,ROR #7
# asm 2: mov >t0=r14,<w3=r5,ROR #7
mov r14,r5,ROR #7

# qhasm: t0 ^= (w3 >>> 18)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w3=int32#6,ROR #18
# asm 2: eor >t0=r14,<t0=r14,<w3=r5,ROR #18
eor r14,r14,r5,ROR #18

# qhasm: t0 ^= (w3 unsigned>> 3)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w3=int32#6,LSR #3
# asm 2: eor >t0=r14,<t0=r14,<w3=r5,LSR #3
eor r14,r14,r5,LSR #3

# qhasm: w2 += t0
# asm 1: add >w2=int32#5,<w2=int32#5,<t0=int32#14
# asm 2: add >w2=r4,<w2=r4,<t0=r14
add r4,r4,r14

# qhasm: w3 += w12
# asm 1: add >w3=int32#6,<w3=int32#6,<w12=int32#11
# asm 2: add >w3=r5,<w3=r5,<w12=r10
add r5,r5,r10

# qhasm: t0 = (w1 >>> 17)
# asm 1: mov >t0=int32#14,<w1=int32#3,ROR #17
# asm 2: mov >t0=r14,<w1=r2,ROR #17
mov r14,r2,ROR #17

# qhasm: t0 ^= (w1 >>> 19)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w1=int32#3,ROR #19
# asm 2: eor >t0=r14,<t0=r14,<w1=r2,ROR #19
eor r14,r14,r2,ROR #19

# qhasm: t0 ^= (w1 unsigned>> 10)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w1=int32#3,LSR #10
# asm 2: eor >t0=r14,<t0=r14,<w1=r2,LSR #10
eor r14,r14,r2,LSR #10

# qhasm: w3 += t0
# asm 1: add >w3=int32#6,<w3=int32#6,<t0=int32#14
# asm 2: add >w3=r5,<w3=r5,<t0=r14
add r5,r5,r14

# qhasm: t0 = (w4 >>> 7)
# asm 1: mov >t0=int32#14,<w4=int32#7,ROR #7
# asm 2: mov >t0=r14,<w4=r6,ROR #7
mov r14,r6,ROR #7

# qhasm: t0 ^= (w4 >>> 18)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w4=int32#7,ROR #18
# asm 2: eor >t0=r14,<t0=r14,<w4=r6,ROR #18
eor r14,r14,r6,ROR #18

# qhasm: t0 ^= (w4 unsigned>> 3)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w4=int32#7,LSR #3
# asm 2: eor >t0=r14,<t0=r14,<w4=r6,LSR #3
eor r14,r14,r6,LSR #3

# qhasm: w3 += t0
# asm 1: add >w3=int32#6,<w3=int32#6,<t0=int32#14
# asm 2: add >w3=r5,<w3=r5,<t0=r14
add r5,r5,r14

# qhasm: w1_stack = w1
# asm 1: str <w1=int32#3,>w1_stack=stack32#20
# asm 2: str <w1=r2,>w1_stack=[sp,#76]
str r2,[sp,#76]

# qhasm: w9_stack = w9
# asm 1: str <w9=int32#8,>w9_stack=stack32#21
# asm 2: str <w9=r7,>w9_stack=[sp,#80]
str r7,[sp,#80]

# qhasm: w10_stack = w10
# asm 1: str <w10=int32#9,>w10_stack=stack32#22
# asm 2: str <w10=r8,>w10_stack=[sp,#84]
str r8,[sp,#84]

# qhasm: w11_stack = w11
# asm 1: str <w11=int32#10,>w11_stack=stack32#23
# asm 2: str <w11=r9,>w11_stack=[sp,#88]
str r9,[sp,#88]

# qhasm: w12_stack = w12
# asm 1: str <w12=int32#11,>w12_stack=stack32#24
# asm 2: str <w12=r10,>w12_stack=[sp,#92]
str r10,[sp,#92]

# qhasm: w13 = w13_stack
# asm 1: ldr >w13=int32#3,<w13_stack=stack32#33
# asm 2: ldr >w13=r2,<w13_stack=[sp,#128]
ldr r2,[sp,#128]

# qhasm: w5 = w5_stack
# asm 1: ldr >w5=int32#8,<w5_stack=stack32#26
# asm 2: ldr >w5=r7,<w5_stack=[sp,#100]
ldr r7,[sp,#100]

# qhasm: w6 = w6_stack
# asm 1: ldr >w6=int32#9,<w6_stack=stack32#27
# asm 2: ldr >w6=r8,<w6_stack=[sp,#104]
ldr r8,[sp,#104]

# qhasm: w7 = w7_stack
# asm 1: ldr >w7=int32#10,<w7_stack=stack32#28
# asm 2: ldr >w7=r9,<w7_stack=[sp,#108]
ldr r9,[sp,#108]

# qhasm: w8 = w8_stack
# asm 1: ldr >w8=int32#11,<w8_stack=stack32#29
# asm 2: ldr >w8=r10,<w8_stack=[sp,#112]
ldr r10,[sp,#112]

# qhasm: w4 += w13
# asm 1: add >w4=int32#7,<w4=int32#7,<w13=int32#3
# asm 2: add >w4=r6,<w4=r6,<w13=r2
add r6,r6,r2

# qhasm: t0 = (w2 >>> 17)
# asm 1: mov >t0=int32#14,<w2=int32#5,ROR #17
# asm 2: mov >t0=r14,<w2=r4,ROR #17
mov r14,r4,ROR #17

# qhasm: t0 ^= (w2 >>> 19)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w2=int32#5,ROR #19
# asm 2: eor >t0=r14,<t0=r14,<w2=r4,ROR #19
eor r14,r14,r4,ROR #19

# qhasm: t0 ^= (w2 unsigned>> 10)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w2=int32#5,LSR #10
# asm 2: eor >t0=r14,<t0=r14,<w2=r4,LSR #10
eor r14,r14,r4,LSR #10

# qhasm: w4 += t0
# asm 1: add >w4=int32#7,<w4=int32#7,<t0=int32#14
# asm 2: add >w4=r6,<w4=r6,<t0=r14
add r6,r6,r14

# qhasm: t0 = (w5 >>> 7)
# asm 1: mov >t0=int32#14,<w5=int32#8,ROR #7
# asm 2: mov >t0=r14,<w5=r7,ROR #7
mov r14,r7,ROR #7

# qhasm: t0 ^= (w5 >>> 18)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w5=int32#8,ROR #18
# asm 2: eor >t0=r14,<t0=r14,<w5=r7,ROR #18
eor r14,r14,r7,ROR #18

# qhasm: t0 ^= (w5 unsigned>> 3)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w5=int32#8,LSR #3
# asm 2: eor >t0=r14,<t0=r14,<w5=r7,LSR #3
eor r14,r14,r7,LSR #3

# qhasm: w4 += t0
# asm 1: add >w4=int32#7,<w4=int32#7,<t0=int32#14
# asm 2: add >w4=r6,<w4=r6,<t0=r14
add r6,r6,r14

# qhasm: w5 += w14
# asm 1: add >w5=int32#8,<w5=int32#8,<w14=int32#12
# asm 2: add >w5=r7,<w5=r7,<w14=r11
add r7,r7,r11

# qhasm: t0 = (w3 >>> 17)
# asm 1: mov >t0=int32#14,<w3=int32#6,ROR #17
# asm 2: mov >t0=r14,<w3=r5,ROR #17
mov r14,r5,ROR #17

# qhasm: t0 ^= (w3 >>> 19)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w3=int32#6,ROR #19
# asm 2: eor >t0=r14,<t0=r14,<w3=r5,ROR #19
eor r14,r14,r5,ROR #19

# qhasm: t0 ^= (w3 unsigned>> 10)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w3=int32#6,LSR #10
# asm 2: eor >t0=r14,<t0=r14,<w3=r5,LSR #10
eor r14,r14,r5,LSR #10

# qhasm: w5 += t0
# asm 1: add >w5=int32#8,<w5=int32#8,<t0=int32#14
# asm 2: add >w5=r7,<w5=r7,<t0=r14
add r7,r7,r14

# qhasm: t0 = (w6 >>> 7)
# asm 1: mov >t0=int32#14,<w6=int32#9,ROR #7
# asm 2: mov >t0=r14,<w6=r8,ROR #7
mov r14,r8,ROR #7

# qhasm: t0 ^= (w6 >>> 18)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w6=int32#9,ROR #18
# asm 2: eor >t0=r14,<t0=r14,<w6=r8,ROR #18
eor r14,r14,r8,ROR #18

# qhasm: t0 ^= (w6 unsigned>> 3)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w6=int32#9,LSR #3
# asm 2: eor >t0=r14,<t0=r14,<w6=r8,LSR #3
eor r14,r14,r8,LSR #3

# qhasm: w5 += t0
# asm 1: add >w5=int32#8,<w5=int32#8,<t0=int32#14
# asm 2: add >w5=r7,<w5=r7,<t0=r14
add r7,r7,r14

# qhasm: w6 += w15
# asm 1: add >w6=int32#9,<w6=int32#9,<w15=int32#13
# asm 2: add >w6=r8,<w6=r8,<w15=r12
add r8,r8,r12

# qhasm: t0 = (w4 >>> 17)
# asm 1: mov >t0=int32#14,<w4=int32#7,ROR #17
# asm 2: mov >t0=r14,<w4=r6,ROR #17
mov r14,r6,ROR #17

# qhasm: t0 ^= (w4 >>> 19)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w4=int32#7,ROR #19
# asm 2: eor >t0=r14,<t0=r14,<w4=r6,ROR #19
eor r14,r14,r6,ROR #19

# qhasm: t0 ^= (w4 unsigned>> 10)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w4=int32#7,LSR #10
# asm 2: eor >t0=r14,<t0=r14,<w4=r6,LSR #10
eor r14,r14,r6,LSR #10

# qhasm: w6 += t0
# asm 1: add >w6=int32#9,<w6=int32#9,<t0=int32#14
# asm 2: add >w6=r8,<w6=r8,<t0=r14
add r8,r8,r14

# qhasm: t0 = (w7 >>> 7)
# asm 1: mov >t0=int32#14,<w7=int32#10,ROR #7
# asm 2: mov >t0=r14,<w7=r9,ROR #7
mov r14,r9,ROR #7

# qhasm: t0 ^= (w7 >>> 18)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w7=int32#10,ROR #18
# asm 2: eor >t0=r14,<t0=r14,<w7=r9,ROR #18
eor r14,r14,r9,ROR #18

# qhasm: t0 ^= (w7 unsigned>> 3)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w7=int32#10,LSR #3
# asm 2: eor >t0=r14,<t0=r14,<w7=r9,LSR #3
eor r14,r14,r9,LSR #3

# qhasm: w6 += t0
# asm 1: add >w6=int32#9,<w6=int32#9,<t0=int32#14
# asm 2: add >w6=r8,<w6=r8,<t0=r14
add r8,r8,r14

# qhasm: w7 += w0
# asm 1: add >w7=int32#10,<w7=int32#10,<w0=int32#1
# asm 2: add >w7=r9,<w7=r9,<w0=r0
add r9,r9,r0

# qhasm: t0 = (w5 >>> 17)
# asm 1: mov >t0=int32#14,<w5=int32#8,ROR #17
# asm 2: mov >t0=r14,<w5=r7,ROR #17
mov r14,r7,ROR #17

# qhasm: t0 ^= (w5 >>> 19)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w5=int32#8,ROR #19
# asm 2: eor >t0=r14,<t0=r14,<w5=r7,ROR #19
eor r14,r14,r7,ROR #19

# qhasm: t0 ^= (w5 unsigned>> 10)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w5=int32#8,LSR #10
# asm 2: eor >t0=r14,<t0=r14,<w5=r7,LSR #10
eor r14,r14,r7,LSR #10

# qhasm: w7 += t0
# asm 1: add >w7=int32#10,<w7=int32#10,<t0=int32#14
# asm 2: add >w7=r9,<w7=r9,<t0=r14
add r9,r9,r14

# qhasm: t0 = (w8 >>> 7)
# asm 1: mov >t0=int32#14,<w8=int32#11,ROR #7
# asm 2: mov >t0=r14,<w8=r10,ROR #7
mov r14,r10,ROR #7

# qhasm: t0 ^= (w8 >>> 18)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w8=int32#11,ROR #18
# asm 2: eor >t0=r14,<t0=r14,<w8=r10,ROR #18
eor r14,r14,r10,ROR #18

# qhasm: t0 ^= (w8 unsigned>> 3)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w8=int32#11,LSR #3
# asm 2: eor >t0=r14,<t0=r14,<w8=r10,LSR #3
eor r14,r14,r10,LSR #3

# qhasm: w7 += t0
# asm 1: add >w7=int32#10,<w7=int32#10,<t0=int32#14
# asm 2: add >w7=r9,<w7=r9,<t0=r14
add r9,r9,r14

# qhasm: w5_stack = w5
# asm 1: str <w5=int32#8,>w5_stack=stack32#25
# asm 2: str <w5=r7,>w5_stack=[sp,#96]
str r7,[sp,#96]

# qhasm: w13_stack = w13
# asm 1: str <w13=int32#3,>w13_stack=stack32#26
# asm 2: str <w13=r2,>w13_stack=[sp,#100]
str r2,[sp,#100]

# qhasm: w14_stack = w14
# asm 1: str <w14=int32#12,>w14_stack=stack32#27
# asm 2: str <w14=r11,>w14_stack=[sp,#104]
str r11,[sp,#104]

# qhasm: w15_stack = w15
# asm 1: str <w15=int32#13,>w15_stack=stack32#28
# asm 2: str <w15=r12,>w15_stack=[sp,#108]
str r12,[sp,#108]

# qhasm: w0_stack = w0
# asm 1: str <w0=int32#1,>w0_stack=stack32#29
# asm 2: str <w0=r0,>w0_stack=[sp,#112]
str r0,[sp,#112]

# qhasm: w1 = w1_stack
# asm 1: ldr >w1=int32#1,<w1_stack=stack32#20
# asm 2: ldr >w1=r0,<w1_stack=[sp,#76]
ldr r0,[sp,#76]

# qhasm: w9 = w9_stack
# asm 1: ldr >w9=int32#3,<w9_stack=stack32#21
# asm 2: ldr >w9=r2,<w9_stack=[sp,#80]
ldr r2,[sp,#80]

# qhasm: w10 = w10_stack
# asm 1: ldr >w10=int32#8,<w10_stack=stack32#22
# asm 2: ldr >w10=r7,<w10_stack=[sp,#84]
ldr r7,[sp,#84]

# qhasm: w11 = w11_stack
# asm 1: ldr >w11=int32#12,<w11_stack=stack32#23
# asm 2: ldr >w11=r11,<w11_stack=[sp,#88]
ldr r11,[sp,#88]

# qhasm: w12 = w12_stack
# asm 1: ldr >w12=int32#13,<w12_stack=stack32#24
# asm 2: ldr >w12=r12,<w12_stack=[sp,#92]
ldr r12,[sp,#92]

# qhasm: w8 += w1
# asm 1: add >w8=int32#11,<w8=int32#11,<w1=int32#1
# asm 2: add >w8=r10,<w8=r10,<w1=r0
add r10,r10,r0

# qhasm: t0 = (w6 >>> 17)
# asm 1: mov >t0=int32#14,<w6=int32#9,ROR #17
# asm 2: mov >t0=r14,<w6=r8,ROR #17
mov r14,r8,ROR #17

# qhasm: t0 ^= (w6 >>> 19)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w6=int32#9,ROR #19
# asm 2: eor >t0=r14,<t0=r14,<w6=r8,ROR #19
eor r14,r14,r8,ROR #19

# qhasm: t0 ^= (w6 unsigned>> 10)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w6=int32#9,LSR #10
# asm 2: eor >t0=r14,<t0=r14,<w6=r8,LSR #10
eor r14,r14,r8,LSR #10

# qhasm: w8 += t0
# asm 1: add >w8=int32#11,<w8=int32#11,<t0=int32#14
# asm 2: add >w8=r10,<w8=r10,<t0=r14
add r10,r10,r14

# qhasm: t0 = (w9 >>> 7)
# asm 1: mov >t0=int32#14,<w9=int32#3,ROR #7
# asm 2: mov >t0=r14,<w9=r2,ROR #7
mov r14,r2,ROR #7

# qhasm: t0 ^= (w9 >>> 18)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w9=int32#3,ROR #18
# asm 2: eor >t0=r14,<t0=r14,<w9=r2,ROR #18
eor r14,r14,r2,ROR #18

# qhasm: t0 ^= (w9 unsigned>> 3)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w9=int32#3,LSR #3
# asm 2: eor >t0=r14,<t0=r14,<w9=r2,LSR #3
eor r14,r14,r2,LSR #3

# qhasm: w8 += t0
# asm 1: add >w8=int32#11,<w8=int32#11,<t0=int32#14
# asm 2: add >w8=r10,<w8=r10,<t0=r14
add r10,r10,r14

# qhasm: w9 += w2
# asm 1: add >w9=int32#3,<w9=int32#3,<w2=int32#5
# asm 2: add >w9=r2,<w9=r2,<w2=r4
add r2,r2,r4

# qhasm: t0 = (w7 >>> 17)
# asm 1: mov >t0=int32#14,<w7=int32#10,ROR #17
# asm 2: mov >t0=r14,<w7=r9,ROR #17
mov r14,r9,ROR #17

# qhasm: t0 ^= (w7 >>> 19)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w7=int32#10,ROR #19
# asm 2: eor >t0=r14,<t0=r14,<w7=r9,ROR #19
eor r14,r14,r9,ROR #19

# qhasm: t0 ^= (w7 unsigned>> 10)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w7=int32#10,LSR #10
# asm 2: eor >t0=r14,<t0=r14,<w7=r9,LSR #10
eor r14,r14,r9,LSR #10

# qhasm: w9 += t0
# asm 1: add >w9=int32#3,<w9=int32#3,<t0=int32#14
# asm 2: add >w9=r2,<w9=r2,<t0=r14
add r2,r2,r14

# qhasm: t0 = (w10 >>> 7)
# asm 1: mov >t0=int32#14,<w10=int32#8,ROR #7
# asm 2: mov >t0=r14,<w10=r7,ROR #7
mov r14,r7,ROR #7

# qhasm: t0 ^= (w10 >>> 18)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w10=int32#8,ROR #18
# asm 2: eor >t0=r14,<t0=r14,<w10=r7,ROR #18
eor r14,r14,r7,ROR #18

# qhasm: t0 ^= (w10 unsigned>> 3)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w10=int32#8,LSR #3
# asm 2: eor >t0=r14,<t0=r14,<w10=r7,LSR #3
eor r14,r14,r7,LSR #3

# qhasm: w9 += t0
# asm 1: add >w9=int32#3,<w9=int32#3,<t0=int32#14
# asm 2: add >w9=r2,<w9=r2,<t0=r14
add r2,r2,r14

# qhasm: w10 += w3
# asm 1: add >w10=int32#8,<w10=int32#8,<w3=int32#6
# asm 2: add >w10=r7,<w10=r7,<w3=r5
add r7,r7,r5

# qhasm: t0 = (w8 >>> 17)
# asm 1: mov >t0=int32#14,<w8=int32#11,ROR #17
# asm 2: mov >t0=r14,<w8=r10,ROR #17
mov r14,r10,ROR #17

# qhasm: t0 ^= (w8 >>> 19)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w8=int32#11,ROR #19
# asm 2: eor >t0=r14,<t0=r14,<w8=r10,ROR #19
eor r14,r14,r10,ROR #19

# qhasm: t0 ^= (w8 unsigned>> 10)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w8=int32#11,LSR #10
# asm 2: eor >t0=r14,<t0=r14,<w8=r10,LSR #10
eor r14,r14,r10,LSR #10

# qhasm: w10 += t0
# asm 1: add >w10=int32#8,<w10=int32#8,<t0=int32#14
# asm 2: add >w10=r7,<w10=r7,<t0=r14
add r7,r7,r14

# qhasm: t0 = (w11 >>> 7)
# asm 1: mov >t0=int32#14,<w11=int32#12,ROR #7
# asm 2: mov >t0=r14,<w11=r11,ROR #7
mov r14,r11,ROR #7

# qhasm: t0 ^= (w11 >>> 18)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w11=int32#12,ROR #18
# asm 2: eor >t0=r14,<t0=r14,<w11=r11,ROR #18
eor r14,r14,r11,ROR #18

# qhasm: t0 ^= (w11 unsigned>> 3)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w11=int32#12,LSR #3
# asm 2: eor >t0=r14,<t0=r14,<w11=r11,LSR #3
eor r14,r14,r11,LSR #3

# qhasm: w10 += t0
# asm 1: add >w10=int32#8,<w10=int32#8,<t0=int32#14
# asm 2: add >w10=r7,<w10=r7,<t0=r14
add r7,r7,r14

# qhasm: w11 += w4
# asm 1: add >w11=int32#12,<w11=int32#12,<w4=int32#7
# asm 2: add >w11=r11,<w11=r11,<w4=r6
add r11,r11,r6

# qhasm: t0 = (w9 >>> 17)
# asm 1: mov >t0=int32#14,<w9=int32#3,ROR #17
# asm 2: mov >t0=r14,<w9=r2,ROR #17
mov r14,r2,ROR #17

# qhasm: t0 ^= (w9 >>> 19)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w9=int32#3,ROR #19
# asm 2: eor >t0=r14,<t0=r14,<w9=r2,ROR #19
eor r14,r14,r2,ROR #19

# qhasm: t0 ^= (w9 unsigned>> 10)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w9=int32#3,LSR #10
# asm 2: eor >t0=r14,<t0=r14,<w9=r2,LSR #10
eor r14,r14,r2,LSR #10

# qhasm: w11 += t0
# asm 1: add >w11=int32#12,<w11=int32#12,<t0=int32#14
# asm 2: add >w11=r11,<w11=r11,<t0=r14
add r11,r11,r14

# qhasm: t0 = (w12 >>> 7)
# asm 1: mov >t0=int32#14,<w12=int32#13,ROR #7
# asm 2: mov >t0=r14,<w12=r12,ROR #7
mov r14,r12,ROR #7

# qhasm: t0 ^= (w12 >>> 18)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w12=int32#13,ROR #18
# asm 2: eor >t0=r14,<t0=r14,<w12=r12,ROR #18
eor r14,r14,r12,ROR #18

# qhasm: t0 ^= (w12 unsigned>> 3)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w12=int32#13,LSR #3
# asm 2: eor >t0=r14,<t0=r14,<w12=r12,LSR #3
eor r14,r14,r12,LSR #3

# qhasm: w11 += t0
# asm 1: add >w11=int32#12,<w11=int32#12,<t0=int32#14
# asm 2: add >w11=r11,<w11=r11,<t0=r14
add r11,r11,r14

# qhasm: w9_stack = w9
# asm 1: str <w9=int32#3,>w9_stack=stack32#20
# asm 2: str <w9=r2,>w9_stack=[sp,#76]
str r2,[sp,#76]

# qhasm: w1_stack = w1
# asm 1: str <w1=int32#1,>w1_stack=stack32#21
# asm 2: str <w1=r0,>w1_stack=[sp,#80]
str r0,[sp,#80]

# qhasm: w2_stack = w2
# asm 1: str <w2=int32#5,>w2_stack=stack32#22
# asm 2: str <w2=r4,>w2_stack=[sp,#84]
str r4,[sp,#84]

# qhasm: w3_stack = w3
# asm 1: str <w3=int32#6,>w3_stack=stack32#23
# asm 2: str <w3=r5,>w3_stack=[sp,#88]
str r5,[sp,#88]

# qhasm: w4_stack = w4
# asm 1: str <w4=int32#7,>w4_stack=stack32#24
# asm 2: str <w4=r6,>w4_stack=[sp,#92]
str r6,[sp,#92]

# qhasm: w5 = w5_stack
# asm 1: ldr >w5=int32#1,<w5_stack=stack32#25
# asm 2: ldr >w5=r0,<w5_stack=[sp,#96]
ldr r0,[sp,#96]

# qhasm: w13 = w13_stack
# asm 1: ldr >w13=int32#3,<w13_stack=stack32#26
# asm 2: ldr >w13=r2,<w13_stack=[sp,#100]
ldr r2,[sp,#100]

# qhasm: w14 = w14_stack
# asm 1: ldr >w14=int32#5,<w14_stack=stack32#27
# asm 2: ldr >w14=r4,<w14_stack=[sp,#104]
ldr r4,[sp,#104]

# qhasm: w15 = w15_stack
# asm 1: ldr >w15=int32#6,<w15_stack=stack32#28
# asm 2: ldr >w15=r5,<w15_stack=[sp,#108]
ldr r5,[sp,#108]

# qhasm: w0 = w0_stack
# asm 1: ldr >w0=int32#7,<w0_stack=stack32#29
# asm 2: ldr >w0=r6,<w0_stack=[sp,#112]
ldr r6,[sp,#112]

# qhasm: w12 += w5
# asm 1: add >w12=int32#13,<w12=int32#13,<w5=int32#1
# asm 2: add >w12=r12,<w12=r12,<w5=r0
add r12,r12,r0

# qhasm: t0 = (w10 >>> 17)
# asm 1: mov >t0=int32#14,<w10=int32#8,ROR #17
# asm 2: mov >t0=r14,<w10=r7,ROR #17
mov r14,r7,ROR #17

# qhasm: t0 ^= (w10 >>> 19)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w10=int32#8,ROR #19
# asm 2: eor >t0=r14,<t0=r14,<w10=r7,ROR #19
eor r14,r14,r7,ROR #19

# qhasm: t0 ^= (w10 unsigned>> 10)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w10=int32#8,LSR #10
# asm 2: eor >t0=r14,<t0=r14,<w10=r7,LSR #10
eor r14,r14,r7,LSR #10

# qhasm: w12 += t0
# asm 1: add >w12=int32#13,<w12=int32#13,<t0=int32#14
# asm 2: add >w12=r12,<w12=r12,<t0=r14
add r12,r12,r14

# qhasm: t0 = (w13 >>> 7)
# asm 1: mov >t0=int32#14,<w13=int32#3,ROR #7
# asm 2: mov >t0=r14,<w13=r2,ROR #7
mov r14,r2,ROR #7

# qhasm: t0 ^= (w13 >>> 18)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w13=int32#3,ROR #18
# asm 2: eor >t0=r14,<t0=r14,<w13=r2,ROR #18
eor r14,r14,r2,ROR #18

# qhasm: t0 ^= (w13 unsigned>> 3)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w13=int32#3,LSR #3
# asm 2: eor >t0=r14,<t0=r14,<w13=r2,LSR #3
eor r14,r14,r2,LSR #3

# qhasm: w12 += t0
# asm 1: add >w12=int32#13,<w12=int32#13,<t0=int32#14
# asm 2: add >w12=r12,<w12=r12,<t0=r14
add r12,r12,r14

# qhasm: w13 += w6
# asm 1: add >w13=int32#3,<w13=int32#3,<w6=int32#9
# asm 2: add >w13=r2,<w13=r2,<w6=r8
add r2,r2,r8

# qhasm: t0 = (w11 >>> 17)
# asm 1: mov >t0=int32#14,<w11=int32#12,ROR #17
# asm 2: mov >t0=r14,<w11=r11,ROR #17
mov r14,r11,ROR #17

# qhasm: t0 ^= (w11 >>> 19)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w11=int32#12,ROR #19
# asm 2: eor >t0=r14,<t0=r14,<w11=r11,ROR #19
eor r14,r14,r11,ROR #19

# qhasm: t0 ^= (w11 unsigned>> 10)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w11=int32#12,LSR #10
# asm 2: eor >t0=r14,<t0=r14,<w11=r11,LSR #10
eor r14,r14,r11,LSR #10

# qhasm: w13 += t0
# asm 1: add >w13=int32#3,<w13=int32#3,<t0=int32#14
# asm 2: add >w13=r2,<w13=r2,<t0=r14
add r2,r2,r14

# qhasm: t0 = (w14 >>> 7)
# asm 1: mov >t0=int32#14,<w14=int32#5,ROR #7
# asm 2: mov >t0=r14,<w14=r4,ROR #7
mov r14,r4,ROR #7

# qhasm: t0 ^= (w14 >>> 18)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w14=int32#5,ROR #18
# asm 2: eor >t0=r14,<t0=r14,<w14=r4,ROR #18
eor r14,r14,r4,ROR #18

# qhasm: t0 ^= (w14 unsigned>> 3)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w14=int32#5,LSR #3
# asm 2: eor >t0=r14,<t0=r14,<w14=r4,LSR #3
eor r14,r14,r4,LSR #3

# qhasm: w13 += t0
# asm 1: add >w13=int32#3,<w13=int32#3,<t0=int32#14
# asm 2: add >w13=r2,<w13=r2,<t0=r14
add r2,r2,r14

# qhasm: w14 += w7
# asm 1: add >w14=int32#5,<w14=int32#5,<w7=int32#10
# asm 2: add >w14=r4,<w14=r4,<w7=r9
add r4,r4,r9

# qhasm: t0 = (w12 >>> 17)
# asm 1: mov >t0=int32#14,<w12=int32#13,ROR #17
# asm 2: mov >t0=r14,<w12=r12,ROR #17
mov r14,r12,ROR #17

# qhasm: t0 ^= (w12 >>> 19)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w12=int32#13,ROR #19
# asm 2: eor >t0=r14,<t0=r14,<w12=r12,ROR #19
eor r14,r14,r12,ROR #19

# qhasm: t0 ^= (w12 unsigned>> 10)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w12=int32#13,LSR #10
# asm 2: eor >t0=r14,<t0=r14,<w12=r12,LSR #10
eor r14,r14,r12,LSR #10

# qhasm: w14 += t0
# asm 1: add >w14=int32#5,<w14=int32#5,<t0=int32#14
# asm 2: add >w14=r4,<w14=r4,<t0=r14
add r4,r4,r14

# qhasm: t0 = (w15 >>> 7)
# asm 1: mov >t0=int32#14,<w15=int32#6,ROR #7
# asm 2: mov >t0=r14,<w15=r5,ROR #7
mov r14,r5,ROR #7

# qhasm: t0 ^= (w15 >>> 18)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w15=int32#6,ROR #18
# asm 2: eor >t0=r14,<t0=r14,<w15=r5,ROR #18
eor r14,r14,r5,ROR #18

# qhasm: t0 ^= (w15 unsigned>> 3)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w15=int32#6,LSR #3
# asm 2: eor >t0=r14,<t0=r14,<w15=r5,LSR #3
eor r14,r14,r5,LSR #3

# qhasm: w14 += t0
# asm 1: add >w14=int32#5,<w14=int32#5,<t0=int32#14
# asm 2: add >w14=r4,<w14=r4,<t0=r14
add r4,r4,r14

# qhasm: w15 += w8
# asm 1: add >w15=int32#6,<w15=int32#6,<w8=int32#11
# asm 2: add >w15=r5,<w15=r5,<w8=r10
add r5,r5,r10

# qhasm: t0 = (w13 >>> 17)
# asm 1: mov >t0=int32#14,<w13=int32#3,ROR #17
# asm 2: mov >t0=r14,<w13=r2,ROR #17
mov r14,r2,ROR #17

# qhasm: t0 ^= (w13 >>> 19)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w13=int32#3,ROR #19
# asm 2: eor >t0=r14,<t0=r14,<w13=r2,ROR #19
eor r14,r14,r2,ROR #19

# qhasm: t0 ^= (w13 unsigned>> 10)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w13=int32#3,LSR #10
# asm 2: eor >t0=r14,<t0=r14,<w13=r2,LSR #10
eor r14,r14,r2,LSR #10

# qhasm: w15 += t0
# asm 1: add >w15=int32#6,<w15=int32#6,<t0=int32#14
# asm 2: add >w15=r5,<w15=r5,<t0=r14
add r5,r5,r14

# qhasm: t0 = (w0 >>> 7)
# asm 1: mov >t0=int32#14,<w0=int32#7,ROR #7
# asm 2: mov >t0=r14,<w0=r6,ROR #7
mov r14,r6,ROR #7

# qhasm: t0 ^= (w0 >>> 18)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w0=int32#7,ROR #18
# asm 2: eor >t0=r14,<t0=r14,<w0=r6,ROR #18
eor r14,r14,r6,ROR #18

# qhasm: t0 ^= (w0 unsigned>> 3)
# asm 1: eor >t0=int32#14,<t0=int32#14,<w0=int32#7,LSR #3
# asm 2: eor >t0=r14,<t0=r14,<w0=r6,LSR #3
eor r14,r14,r6,LSR #3

# qhasm: w15 += t0
# asm 1: add >w15=int32#6,<w15=int32#6,<t0=int32#14
# asm 2: add >w15=r5,<w15=r5,<t0=r14
add r5,r5,r14

# qhasm: w0_stack = w0
# asm 1: str <w0=int32#7,>w0_stack=stack32#25
# asm 2: str <w0=r6,>w0_stack=[sp,#96]
str r6,[sp,#96]

# qhasm: w5_stack = w5
# asm 1: str <w5=int32#1,>w5_stack=stack32#26
# asm 2: str <w5=r0,>w5_stack=[sp,#100]
str r0,[sp,#100]

# qhasm: w6_stack = w6
# asm 1: str <w6=int32#9,>w6_stack=stack32#27
# asm 2: str <w6=r8,>w6_stack=[sp,#104]
str r8,[sp,#104]

# qhasm: w7_stack = w7
# asm 1: str <w7=int32#10,>w7_stack=stack32#28
# asm 2: str <w7=r9,>w7_stack=[sp,#108]
str r9,[sp,#108]

# qhasm: w8_stack = w8
# asm 1: str <w8=int32#11,>w8_stack=stack32#29
# asm 2: str <w8=r10,>w8_stack=[sp,#112]
str r10,[sp,#112]

# qhasm: w10_stack = w10
# asm 1: str <w10=int32#8,>w10_stack=stack32#30
# asm 2: str <w10=r7,>w10_stack=[sp,#116]
str r7,[sp,#116]

# qhasm: w11_stack = w11
# asm 1: str <w11=int32#12,>w11_stack=stack32#31
# asm 2: str <w11=r11,>w11_stack=[sp,#120]
str r11,[sp,#120]

# qhasm: w12_stack = w12
# asm 1: str <w12=int32#13,>w12_stack=stack32#32
# asm 2: str <w12=r12,>w12_stack=[sp,#124]
str r12,[sp,#124]

# qhasm: w13_stack = w13
# asm 1: str <w13=int32#3,>w13_stack=stack32#33
# asm 2: str <w13=r2,>w13_stack=[sp,#128]
str r2,[sp,#128]

# qhasm: w14_stack = w14
# asm 1: str <w14=int32#5,>w14_stack=stack32#34
# asm 2: str <w14=r4,>w14_stack=[sp,#132]
str r4,[sp,#132]

# qhasm: w15_stack = w15
# asm 1: str <w15=int32#6,>w15_stack=stack32#35
# asm 2: str <w15=r5,>w15_stack=[sp,#136]
str r5,[sp,#136]

# qhasm: r0 = r0_stack
# asm 1: ldr >r0=int32#1,<r0_stack=stack32#36
# asm 2: ldr >r0=r0,<r0_stack=[sp,#140]
ldr r0,[sp,#140]

# qhasm: r1 = r1_stack
# asm 1: ldr >r1=int32#3,<r1_stack=stack32#37
# asm 2: ldr >r1=r2,<r1_stack=[sp,#144]
ldr r2,[sp,#144]

# qhasm: r2 = r2_stack
# asm 1: ldr >r2=int32#5,<r2_stack=stack32#38
# asm 2: ldr >r2=r4,<r2_stack=[sp,#148]
ldr r4,[sp,#148]

# qhasm: r3 = r3_stack
# asm 1: ldr >r3=int32#6,<r3_stack=stack32#39
# asm 2: ldr >r3=r5,<r3_stack=[sp,#152]
ldr r5,[sp,#152]

# qhasm: r4 = r4_stack
# asm 1: ldr >r4=int32#7,<r4_stack=stack32#40
# asm 2: ldr >r4=r6,<r4_stack=[sp,#156]
ldr r6,[sp,#156]

# qhasm: r5 = r5_stack
# asm 1: ldr >r5=int32#8,<r5_stack=stack32#41
# asm 2: ldr >r5=r7,<r5_stack=[sp,#160]
ldr r7,[sp,#160]

# qhasm: r6 = r6_stack
# asm 1: ldr >r6=int32#9,<r6_stack=stack32#42
# asm 2: ldr >r6=r8,<r6_stack=[sp,#164]
ldr r8,[sp,#164]

# qhasm: r7 = r7_stack
# asm 1: ldr >r7=int32#10,<r7_stack=stack32#43
# asm 2: ldr >r7=r9,<r7_stack=[sp,#168]
ldr r9,[sp,#168]

# qhasm: w0 = w0_stack
# asm 1: ldr >w0=int32#11,<w0_stack=stack32#25
# asm 2: ldr >w0=r10,<w0_stack=[sp,#96]
ldr r10,[sp,#96]

# qhasm: t0 = r4 & r5
# asm 1: and >t0=int32#12,<r4=int32#7,<r5=int32#8
# asm 2: and >t0=r11,<r4=r6,<r5=r7
and r11,r6,r7

# qhasm: t1 = r6 & ~r4
# asm 1: bic >t1=int32#13,<r6=int32#9,<r4=int32#7
# asm 2: bic >t1=r12,<r6=r8,<r4=r6
bic r12,r8,r6

# qhasm: t0 ^= t1
# asm 1: eor >t0=int32#12,<t0=int32#12,<t1=int32#13
# asm 2: eor >t0=r11,<t0=r11,<t1=r12
eor r11,r11,r12

# qhasm: t1 = r4 ^ (r4 >>> 5)
# asm 1: eor >t1=int32#13,<r4=int32#7,<r4=int32#7,ROR #5
# asm 2: eor >t1=r12,<r4=r6,<r4=r6,ROR #5
eor r12,r6,r6,ROR #5

# qhasm: t1 ^= (r4 >>> 19)
# asm 1: eor >t1=int32#13,<t1=int32#13,<r4=int32#7,ROR #19
# asm 2: eor >t1=r12,<t1=r12,<r4=r6,ROR #19
eor r12,r12,r6,ROR #19

# qhasm: t2 = mem32[input_3 + 192]
# asm 1: ldr >t2=int32#14,[<input_3=int32#4,#192]
# asm 2: ldr >t2=r14,[<input_3=r3,#192]
ldr r14,[r3,#192]

# qhasm: t0 += w0
# asm 1: add >t0=int32#11,<t0=int32#12,<w0=int32#11
# asm 2: add >t0=r10,<t0=r11,<w0=r10
add r10,r11,r10

# qhasm: t0 += (t1 >>> 6)
# asm 1: add >t0=int32#11,<t0=int32#11,<t1=int32#13,ROR #6
# asm 2: add >t0=r10,<t0=r10,<t1=r12,ROR #6
add r10,r10,r12,ROR #6

# qhasm: t0 += t2
# asm 1: add >t0=int32#11,<t0=int32#11,<t2=int32#14
# asm 2: add >t0=r10,<t0=r10,<t2=r14
add r10,r10,r14

# qhasm: r7 += t0
# asm 1: add >r7=int32#10,<r7=int32#10,<t0=int32#11
# asm 2: add >r7=r9,<r7=r9,<t0=r10
add r9,r9,r10

# qhasm: r3 += r7
# asm 1: add >r3=int32#6,<r3=int32#6,<r7=int32#10
# asm 2: add >r3=r5,<r3=r5,<r7=r9
add r5,r5,r9

# qhasm: t0 = r0 & r1
# asm 1: and >t0=int32#11,<r0=int32#1,<r1=int32#3
# asm 2: and >t0=r10,<r0=r0,<r1=r2
and r10,r0,r2

# qhasm: t2 = r0 & r2
# asm 1: and >t2=int32#12,<r0=int32#1,<r2=int32#5
# asm 2: and >t2=r11,<r0=r0,<r2=r4
and r11,r0,r4

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t2 = r1 & r2
# asm 1: and >t2=int32#12,<r1=int32#3,<r2=int32#5
# asm 2: and >t2=r11,<r1=r2,<r2=r4
and r11,r2,r4

# qhasm: t3 = r0 ^ (r0 >>> 11)
# asm 1: eor >t3=int32#13,<r0=int32#1,<r0=int32#1,ROR #11
# asm 2: eor >t3=r12,<r0=r0,<r0=r0,ROR #11
eor r12,r0,r0,ROR #11

# qhasm: t3 ^= (r0 >>> 20)
# asm 1: eor >t3=int32#13,<t3=int32#13,<r0=int32#1,ROR #20
# asm 2: eor >t3=r12,<t3=r12,<r0=r0,ROR #20
eor r12,r12,r0,ROR #20

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t0 += (t3 >>> 2)
# asm 1: add >t0=int32#11,<t0=int32#11,<t3=int32#13,ROR #2
# asm 2: add >t0=r10,<t0=r10,<t3=r12,ROR #2
add r10,r10,r12,ROR #2

# qhasm: r7 += t0
# asm 1: add >r7=int32#10,<r7=int32#10,<t0=int32#11
# asm 2: add >r7=r9,<r7=r9,<t0=r10
add r9,r9,r10

# qhasm: w1 = w1_stack
# asm 1: ldr >w1=int32#11,<w1_stack=stack32#21
# asm 2: ldr >w1=r10,<w1_stack=[sp,#80]
ldr r10,[sp,#80]

# qhasm: t0 = r3 & r4
# asm 1: and >t0=int32#12,<r3=int32#6,<r4=int32#7
# asm 2: and >t0=r11,<r3=r5,<r4=r6
and r11,r5,r6

# qhasm: t1 = r5 & ~r3
# asm 1: bic >t1=int32#13,<r5=int32#8,<r3=int32#6
# asm 2: bic >t1=r12,<r5=r7,<r3=r5
bic r12,r7,r5

# qhasm: t0 ^= t1
# asm 1: eor >t0=int32#12,<t0=int32#12,<t1=int32#13
# asm 2: eor >t0=r11,<t0=r11,<t1=r12
eor r11,r11,r12

# qhasm: t1 = r3 ^ (r3 >>> 5)
# asm 1: eor >t1=int32#13,<r3=int32#6,<r3=int32#6,ROR #5
# asm 2: eor >t1=r12,<r3=r5,<r3=r5,ROR #5
eor r12,r5,r5,ROR #5

# qhasm: t1 ^= (r3 >>> 19)
# asm 1: eor >t1=int32#13,<t1=int32#13,<r3=int32#6,ROR #19
# asm 2: eor >t1=r12,<t1=r12,<r3=r5,ROR #19
eor r12,r12,r5,ROR #19

# qhasm: t2 = mem32[input_3 + 196]
# asm 1: ldr >t2=int32#14,[<input_3=int32#4,#196]
# asm 2: ldr >t2=r14,[<input_3=r3,#196]
ldr r14,[r3,#196]

# qhasm: t0 += w1
# asm 1: add >t0=int32#11,<t0=int32#12,<w1=int32#11
# asm 2: add >t0=r10,<t0=r11,<w1=r10
add r10,r11,r10

# qhasm: t0 += (t1 >>> 6)
# asm 1: add >t0=int32#11,<t0=int32#11,<t1=int32#13,ROR #6
# asm 2: add >t0=r10,<t0=r10,<t1=r12,ROR #6
add r10,r10,r12,ROR #6

# qhasm: t0 += t2
# asm 1: add >t0=int32#11,<t0=int32#11,<t2=int32#14
# asm 2: add >t0=r10,<t0=r10,<t2=r14
add r10,r10,r14

# qhasm: r6 += t0
# asm 1: add >r6=int32#9,<r6=int32#9,<t0=int32#11
# asm 2: add >r6=r8,<r6=r8,<t0=r10
add r8,r8,r10

# qhasm: r2 += r6
# asm 1: add >r2=int32#5,<r2=int32#5,<r6=int32#9
# asm 2: add >r2=r4,<r2=r4,<r6=r8
add r4,r4,r8

# qhasm: t0 = r7 & r0
# asm 1: and >t0=int32#11,<r7=int32#10,<r0=int32#1
# asm 2: and >t0=r10,<r7=r9,<r0=r0
and r10,r9,r0

# qhasm: t2 = r7 & r1
# asm 1: and >t2=int32#12,<r7=int32#10,<r1=int32#3
# asm 2: and >t2=r11,<r7=r9,<r1=r2
and r11,r9,r2

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t2 = r0 & r1
# asm 1: and >t2=int32#12,<r0=int32#1,<r1=int32#3
# asm 2: and >t2=r11,<r0=r0,<r1=r2
and r11,r0,r2

# qhasm: t3 = r7 ^ (r7 >>> 11)
# asm 1: eor >t3=int32#13,<r7=int32#10,<r7=int32#10,ROR #11
# asm 2: eor >t3=r12,<r7=r9,<r7=r9,ROR #11
eor r12,r9,r9,ROR #11

# qhasm: t3 ^= (r7 >>> 20)
# asm 1: eor >t3=int32#13,<t3=int32#13,<r7=int32#10,ROR #20
# asm 2: eor >t3=r12,<t3=r12,<r7=r9,ROR #20
eor r12,r12,r9,ROR #20

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t0 += (t3 >>> 2)
# asm 1: add >t0=int32#11,<t0=int32#11,<t3=int32#13,ROR #2
# asm 2: add >t0=r10,<t0=r10,<t3=r12,ROR #2
add r10,r10,r12,ROR #2

# qhasm: r6 += t0
# asm 1: add >r6=int32#9,<r6=int32#9,<t0=int32#11
# asm 2: add >r6=r8,<r6=r8,<t0=r10
add r8,r8,r10

# qhasm: w2 = w2_stack
# asm 1: ldr >w2=int32#11,<w2_stack=stack32#22
# asm 2: ldr >w2=r10,<w2_stack=[sp,#84]
ldr r10,[sp,#84]

# qhasm: t0 = r2 & r3
# asm 1: and >t0=int32#12,<r2=int32#5,<r3=int32#6
# asm 2: and >t0=r11,<r2=r4,<r3=r5
and r11,r4,r5

# qhasm: t1 = r4 & ~r2
# asm 1: bic >t1=int32#13,<r4=int32#7,<r2=int32#5
# asm 2: bic >t1=r12,<r4=r6,<r2=r4
bic r12,r6,r4

# qhasm: t0 ^= t1
# asm 1: eor >t0=int32#12,<t0=int32#12,<t1=int32#13
# asm 2: eor >t0=r11,<t0=r11,<t1=r12
eor r11,r11,r12

# qhasm: t1 = r2 ^ (r2 >>> 5)
# asm 1: eor >t1=int32#13,<r2=int32#5,<r2=int32#5,ROR #5
# asm 2: eor >t1=r12,<r2=r4,<r2=r4,ROR #5
eor r12,r4,r4,ROR #5

# qhasm: t1 ^= (r2 >>> 19)
# asm 1: eor >t1=int32#13,<t1=int32#13,<r2=int32#5,ROR #19
# asm 2: eor >t1=r12,<t1=r12,<r2=r4,ROR #19
eor r12,r12,r4,ROR #19

# qhasm: t2 = mem32[input_3 + 200]
# asm 1: ldr >t2=int32#14,[<input_3=int32#4,#200]
# asm 2: ldr >t2=r14,[<input_3=r3,#200]
ldr r14,[r3,#200]

# qhasm: t0 += w2
# asm 1: add >t0=int32#11,<t0=int32#12,<w2=int32#11
# asm 2: add >t0=r10,<t0=r11,<w2=r10
add r10,r11,r10

# qhasm: t0 += (t1 >>> 6)
# asm 1: add >t0=int32#11,<t0=int32#11,<t1=int32#13,ROR #6
# asm 2: add >t0=r10,<t0=r10,<t1=r12,ROR #6
add r10,r10,r12,ROR #6

# qhasm: t0 += t2
# asm 1: add >t0=int32#11,<t0=int32#11,<t2=int32#14
# asm 2: add >t0=r10,<t0=r10,<t2=r14
add r10,r10,r14

# qhasm: r5 += t0
# asm 1: add >r5=int32#8,<r5=int32#8,<t0=int32#11
# asm 2: add >r5=r7,<r5=r7,<t0=r10
add r7,r7,r10

# qhasm: r1 += r5
# asm 1: add >r1=int32#3,<r1=int32#3,<r5=int32#8
# asm 2: add >r1=r2,<r1=r2,<r5=r7
add r2,r2,r7

# qhasm: t0 = r6 & r7
# asm 1: and >t0=int32#11,<r6=int32#9,<r7=int32#10
# asm 2: and >t0=r10,<r6=r8,<r7=r9
and r10,r8,r9

# qhasm: t2 = r6 & r0
# asm 1: and >t2=int32#12,<r6=int32#9,<r0=int32#1
# asm 2: and >t2=r11,<r6=r8,<r0=r0
and r11,r8,r0

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t2 = r7 & r0
# asm 1: and >t2=int32#12,<r7=int32#10,<r0=int32#1
# asm 2: and >t2=r11,<r7=r9,<r0=r0
and r11,r9,r0

# qhasm: t3 = r6 ^ (r6 >>> 11)
# asm 1: eor >t3=int32#13,<r6=int32#9,<r6=int32#9,ROR #11
# asm 2: eor >t3=r12,<r6=r8,<r6=r8,ROR #11
eor r12,r8,r8,ROR #11

# qhasm: t3 ^= (r6 >>> 20)
# asm 1: eor >t3=int32#13,<t3=int32#13,<r6=int32#9,ROR #20
# asm 2: eor >t3=r12,<t3=r12,<r6=r8,ROR #20
eor r12,r12,r8,ROR #20

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t0 += (t3 >>> 2)
# asm 1: add >t0=int32#11,<t0=int32#11,<t3=int32#13,ROR #2
# asm 2: add >t0=r10,<t0=r10,<t3=r12,ROR #2
add r10,r10,r12,ROR #2

# qhasm: r5 += t0
# asm 1: add >r5=int32#8,<r5=int32#8,<t0=int32#11
# asm 2: add >r5=r7,<r5=r7,<t0=r10
add r7,r7,r10

# qhasm: w3 = w3_stack
# asm 1: ldr >w3=int32#11,<w3_stack=stack32#23
# asm 2: ldr >w3=r10,<w3_stack=[sp,#88]
ldr r10,[sp,#88]

# qhasm: t0 = r1 & r2
# asm 1: and >t0=int32#12,<r1=int32#3,<r2=int32#5
# asm 2: and >t0=r11,<r1=r2,<r2=r4
and r11,r2,r4

# qhasm: t1 = r3 & ~r1
# asm 1: bic >t1=int32#13,<r3=int32#6,<r1=int32#3
# asm 2: bic >t1=r12,<r3=r5,<r1=r2
bic r12,r5,r2

# qhasm: t0 ^= t1
# asm 1: eor >t0=int32#12,<t0=int32#12,<t1=int32#13
# asm 2: eor >t0=r11,<t0=r11,<t1=r12
eor r11,r11,r12

# qhasm: t1 = r1 ^ (r1 >>> 5)
# asm 1: eor >t1=int32#13,<r1=int32#3,<r1=int32#3,ROR #5
# asm 2: eor >t1=r12,<r1=r2,<r1=r2,ROR #5
eor r12,r2,r2,ROR #5

# qhasm: t1 ^= (r1 >>> 19)
# asm 1: eor >t1=int32#13,<t1=int32#13,<r1=int32#3,ROR #19
# asm 2: eor >t1=r12,<t1=r12,<r1=r2,ROR #19
eor r12,r12,r2,ROR #19

# qhasm: t2 = mem32[input_3 + 204]
# asm 1: ldr >t2=int32#14,[<input_3=int32#4,#204]
# asm 2: ldr >t2=r14,[<input_3=r3,#204]
ldr r14,[r3,#204]

# qhasm: t0 += w3
# asm 1: add >t0=int32#11,<t0=int32#12,<w3=int32#11
# asm 2: add >t0=r10,<t0=r11,<w3=r10
add r10,r11,r10

# qhasm: t0 += (t1 >>> 6)
# asm 1: add >t0=int32#11,<t0=int32#11,<t1=int32#13,ROR #6
# asm 2: add >t0=r10,<t0=r10,<t1=r12,ROR #6
add r10,r10,r12,ROR #6

# qhasm: t0 += t2
# asm 1: add >t0=int32#11,<t0=int32#11,<t2=int32#14
# asm 2: add >t0=r10,<t0=r10,<t2=r14
add r10,r10,r14

# qhasm: r4 += t0
# asm 1: add >r4=int32#7,<r4=int32#7,<t0=int32#11
# asm 2: add >r4=r6,<r4=r6,<t0=r10
add r6,r6,r10

# qhasm: r0 += r4
# asm 1: add >r0=int32#1,<r0=int32#1,<r4=int32#7
# asm 2: add >r0=r0,<r0=r0,<r4=r6
add r0,r0,r6

# qhasm: t0 = r5 & r6
# asm 1: and >t0=int32#11,<r5=int32#8,<r6=int32#9
# asm 2: and >t0=r10,<r5=r7,<r6=r8
and r10,r7,r8

# qhasm: t2 = r5 & r7
# asm 1: and >t2=int32#12,<r5=int32#8,<r7=int32#10
# asm 2: and >t2=r11,<r5=r7,<r7=r9
and r11,r7,r9

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t2 = r6 & r7
# asm 1: and >t2=int32#12,<r6=int32#9,<r7=int32#10
# asm 2: and >t2=r11,<r6=r8,<r7=r9
and r11,r8,r9

# qhasm: t3 = r5 ^ (r5 >>> 11)
# asm 1: eor >t3=int32#13,<r5=int32#8,<r5=int32#8,ROR #11
# asm 2: eor >t3=r12,<r5=r7,<r5=r7,ROR #11
eor r12,r7,r7,ROR #11

# qhasm: t3 ^= (r5 >>> 20)
# asm 1: eor >t3=int32#13,<t3=int32#13,<r5=int32#8,ROR #20
# asm 2: eor >t3=r12,<t3=r12,<r5=r7,ROR #20
eor r12,r12,r7,ROR #20

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t0 += (t3 >>> 2)
# asm 1: add >t0=int32#11,<t0=int32#11,<t3=int32#13,ROR #2
# asm 2: add >t0=r10,<t0=r10,<t3=r12,ROR #2
add r10,r10,r12,ROR #2

# qhasm: r4 += t0
# asm 1: add >r4=int32#7,<r4=int32#7,<t0=int32#11
# asm 2: add >r4=r6,<r4=r6,<t0=r10
add r6,r6,r10

# qhasm: w4 = w4_stack
# asm 1: ldr >w4=int32#11,<w4_stack=stack32#24
# asm 2: ldr >w4=r10,<w4_stack=[sp,#92]
ldr r10,[sp,#92]

# qhasm: t0 = r0 & r1
# asm 1: and >t0=int32#12,<r0=int32#1,<r1=int32#3
# asm 2: and >t0=r11,<r0=r0,<r1=r2
and r11,r0,r2

# qhasm: t1 = r2 & ~r0
# asm 1: bic >t1=int32#13,<r2=int32#5,<r0=int32#1
# asm 2: bic >t1=r12,<r2=r4,<r0=r0
bic r12,r4,r0

# qhasm: t0 ^= t1
# asm 1: eor >t0=int32#12,<t0=int32#12,<t1=int32#13
# asm 2: eor >t0=r11,<t0=r11,<t1=r12
eor r11,r11,r12

# qhasm: t1 = r0 ^ (r0 >>> 5)
# asm 1: eor >t1=int32#13,<r0=int32#1,<r0=int32#1,ROR #5
# asm 2: eor >t1=r12,<r0=r0,<r0=r0,ROR #5
eor r12,r0,r0,ROR #5

# qhasm: t1 ^= (r0 >>> 19)
# asm 1: eor >t1=int32#13,<t1=int32#13,<r0=int32#1,ROR #19
# asm 2: eor >t1=r12,<t1=r12,<r0=r0,ROR #19
eor r12,r12,r0,ROR #19

# qhasm: t2 = mem32[input_3 + 208]
# asm 1: ldr >t2=int32#14,[<input_3=int32#4,#208]
# asm 2: ldr >t2=r14,[<input_3=r3,#208]
ldr r14,[r3,#208]

# qhasm: t0 += w4
# asm 1: add >t0=int32#11,<t0=int32#12,<w4=int32#11
# asm 2: add >t0=r10,<t0=r11,<w4=r10
add r10,r11,r10

# qhasm: t0 += (t1 >>> 6)
# asm 1: add >t0=int32#11,<t0=int32#11,<t1=int32#13,ROR #6
# asm 2: add >t0=r10,<t0=r10,<t1=r12,ROR #6
add r10,r10,r12,ROR #6

# qhasm: t0 += t2
# asm 1: add >t0=int32#11,<t0=int32#11,<t2=int32#14
# asm 2: add >t0=r10,<t0=r10,<t2=r14
add r10,r10,r14

# qhasm: r3 += t0
# asm 1: add >r3=int32#6,<r3=int32#6,<t0=int32#11
# asm 2: add >r3=r5,<r3=r5,<t0=r10
add r5,r5,r10

# qhasm: r7 += r3
# asm 1: add >r7=int32#10,<r7=int32#10,<r3=int32#6
# asm 2: add >r7=r9,<r7=r9,<r3=r5
add r9,r9,r5

# qhasm: t0 = r4 & r5
# asm 1: and >t0=int32#11,<r4=int32#7,<r5=int32#8
# asm 2: and >t0=r10,<r4=r6,<r5=r7
and r10,r6,r7

# qhasm: t2 = r4 & r6
# asm 1: and >t2=int32#12,<r4=int32#7,<r6=int32#9
# asm 2: and >t2=r11,<r4=r6,<r6=r8
and r11,r6,r8

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t2 = r5 & r6
# asm 1: and >t2=int32#12,<r5=int32#8,<r6=int32#9
# asm 2: and >t2=r11,<r5=r7,<r6=r8
and r11,r7,r8

# qhasm: t3 = r4 ^ (r4 >>> 11)
# asm 1: eor >t3=int32#13,<r4=int32#7,<r4=int32#7,ROR #11
# asm 2: eor >t3=r12,<r4=r6,<r4=r6,ROR #11
eor r12,r6,r6,ROR #11

# qhasm: t3 ^= (r4 >>> 20)
# asm 1: eor >t3=int32#13,<t3=int32#13,<r4=int32#7,ROR #20
# asm 2: eor >t3=r12,<t3=r12,<r4=r6,ROR #20
eor r12,r12,r6,ROR #20

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t0 += (t3 >>> 2)
# asm 1: add >t0=int32#11,<t0=int32#11,<t3=int32#13,ROR #2
# asm 2: add >t0=r10,<t0=r10,<t3=r12,ROR #2
add r10,r10,r12,ROR #2

# qhasm: r3 += t0
# asm 1: add >r3=int32#6,<r3=int32#6,<t0=int32#11
# asm 2: add >r3=r5,<r3=r5,<t0=r10
add r5,r5,r10

# qhasm: w5 = w5_stack
# asm 1: ldr >w5=int32#11,<w5_stack=stack32#26
# asm 2: ldr >w5=r10,<w5_stack=[sp,#100]
ldr r10,[sp,#100]

# qhasm: t0 = r7 & r0
# asm 1: and >t0=int32#12,<r7=int32#10,<r0=int32#1
# asm 2: and >t0=r11,<r7=r9,<r0=r0
and r11,r9,r0

# qhasm: t1 = r1 & ~r7
# asm 1: bic >t1=int32#13,<r1=int32#3,<r7=int32#10
# asm 2: bic >t1=r12,<r1=r2,<r7=r9
bic r12,r2,r9

# qhasm: t0 ^= t1
# asm 1: eor >t0=int32#12,<t0=int32#12,<t1=int32#13
# asm 2: eor >t0=r11,<t0=r11,<t1=r12
eor r11,r11,r12

# qhasm: t1 = r7 ^ (r7 >>> 5)
# asm 1: eor >t1=int32#13,<r7=int32#10,<r7=int32#10,ROR #5
# asm 2: eor >t1=r12,<r7=r9,<r7=r9,ROR #5
eor r12,r9,r9,ROR #5

# qhasm: t1 ^= (r7 >>> 19)
# asm 1: eor >t1=int32#13,<t1=int32#13,<r7=int32#10,ROR #19
# asm 2: eor >t1=r12,<t1=r12,<r7=r9,ROR #19
eor r12,r12,r9,ROR #19

# qhasm: t2 = mem32[input_3 + 212]
# asm 1: ldr >t2=int32#14,[<input_3=int32#4,#212]
# asm 2: ldr >t2=r14,[<input_3=r3,#212]
ldr r14,[r3,#212]

# qhasm: t0 += w5
# asm 1: add >t0=int32#11,<t0=int32#12,<w5=int32#11
# asm 2: add >t0=r10,<t0=r11,<w5=r10
add r10,r11,r10

# qhasm: t0 += (t1 >>> 6)
# asm 1: add >t0=int32#11,<t0=int32#11,<t1=int32#13,ROR #6
# asm 2: add >t0=r10,<t0=r10,<t1=r12,ROR #6
add r10,r10,r12,ROR #6

# qhasm: t0 += t2
# asm 1: add >t0=int32#11,<t0=int32#11,<t2=int32#14
# asm 2: add >t0=r10,<t0=r10,<t2=r14
add r10,r10,r14

# qhasm: r2 += t0
# asm 1: add >r2=int32#5,<r2=int32#5,<t0=int32#11
# asm 2: add >r2=r4,<r2=r4,<t0=r10
add r4,r4,r10

# qhasm: r6 += r2
# asm 1: add >r6=int32#9,<r6=int32#9,<r2=int32#5
# asm 2: add >r6=r8,<r6=r8,<r2=r4
add r8,r8,r4

# qhasm: t0 = r3 & r4
# asm 1: and >t0=int32#11,<r3=int32#6,<r4=int32#7
# asm 2: and >t0=r10,<r3=r5,<r4=r6
and r10,r5,r6

# qhasm: t2 = r3 & r5
# asm 1: and >t2=int32#12,<r3=int32#6,<r5=int32#8
# asm 2: and >t2=r11,<r3=r5,<r5=r7
and r11,r5,r7

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t2 = r4 & r5
# asm 1: and >t2=int32#12,<r4=int32#7,<r5=int32#8
# asm 2: and >t2=r11,<r4=r6,<r5=r7
and r11,r6,r7

# qhasm: t3 = r3 ^ (r3 >>> 11)
# asm 1: eor >t3=int32#13,<r3=int32#6,<r3=int32#6,ROR #11
# asm 2: eor >t3=r12,<r3=r5,<r3=r5,ROR #11
eor r12,r5,r5,ROR #11

# qhasm: t3 ^= (r3 >>> 20)
# asm 1: eor >t3=int32#13,<t3=int32#13,<r3=int32#6,ROR #20
# asm 2: eor >t3=r12,<t3=r12,<r3=r5,ROR #20
eor r12,r12,r5,ROR #20

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t0 += (t3 >>> 2)
# asm 1: add >t0=int32#11,<t0=int32#11,<t3=int32#13,ROR #2
# asm 2: add >t0=r10,<t0=r10,<t3=r12,ROR #2
add r10,r10,r12,ROR #2

# qhasm: r2 += t0
# asm 1: add >r2=int32#5,<r2=int32#5,<t0=int32#11
# asm 2: add >r2=r4,<r2=r4,<t0=r10
add r4,r4,r10

# qhasm: w6 = w6_stack
# asm 1: ldr >w6=int32#11,<w6_stack=stack32#27
# asm 2: ldr >w6=r10,<w6_stack=[sp,#104]
ldr r10,[sp,#104]

# qhasm: t0 = r6 & r7
# asm 1: and >t0=int32#12,<r6=int32#9,<r7=int32#10
# asm 2: and >t0=r11,<r6=r8,<r7=r9
and r11,r8,r9

# qhasm: t1 = r0 & ~r6
# asm 1: bic >t1=int32#13,<r0=int32#1,<r6=int32#9
# asm 2: bic >t1=r12,<r0=r0,<r6=r8
bic r12,r0,r8

# qhasm: t0 ^= t1
# asm 1: eor >t0=int32#12,<t0=int32#12,<t1=int32#13
# asm 2: eor >t0=r11,<t0=r11,<t1=r12
eor r11,r11,r12

# qhasm: t1 = r6 ^ (r6 >>> 5)
# asm 1: eor >t1=int32#13,<r6=int32#9,<r6=int32#9,ROR #5
# asm 2: eor >t1=r12,<r6=r8,<r6=r8,ROR #5
eor r12,r8,r8,ROR #5

# qhasm: t1 ^= (r6 >>> 19)
# asm 1: eor >t1=int32#13,<t1=int32#13,<r6=int32#9,ROR #19
# asm 2: eor >t1=r12,<t1=r12,<r6=r8,ROR #19
eor r12,r12,r8,ROR #19

# qhasm: t2 = mem32[input_3 + 216]
# asm 1: ldr >t2=int32#14,[<input_3=int32#4,#216]
# asm 2: ldr >t2=r14,[<input_3=r3,#216]
ldr r14,[r3,#216]

# qhasm: t0 += w6
# asm 1: add >t0=int32#11,<t0=int32#12,<w6=int32#11
# asm 2: add >t0=r10,<t0=r11,<w6=r10
add r10,r11,r10

# qhasm: t0 += (t1 >>> 6)
# asm 1: add >t0=int32#11,<t0=int32#11,<t1=int32#13,ROR #6
# asm 2: add >t0=r10,<t0=r10,<t1=r12,ROR #6
add r10,r10,r12,ROR #6

# qhasm: t0 += t2
# asm 1: add >t0=int32#11,<t0=int32#11,<t2=int32#14
# asm 2: add >t0=r10,<t0=r10,<t2=r14
add r10,r10,r14

# qhasm: r1 += t0
# asm 1: add >r1=int32#3,<r1=int32#3,<t0=int32#11
# asm 2: add >r1=r2,<r1=r2,<t0=r10
add r2,r2,r10

# qhasm: r5 += r1
# asm 1: add >r5=int32#8,<r5=int32#8,<r1=int32#3
# asm 2: add >r5=r7,<r5=r7,<r1=r2
add r7,r7,r2

# qhasm: t0 = r2 & r3
# asm 1: and >t0=int32#11,<r2=int32#5,<r3=int32#6
# asm 2: and >t0=r10,<r2=r4,<r3=r5
and r10,r4,r5

# qhasm: t2 = r2 & r4
# asm 1: and >t2=int32#12,<r2=int32#5,<r4=int32#7
# asm 2: and >t2=r11,<r2=r4,<r4=r6
and r11,r4,r6

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t2 = r3 & r4
# asm 1: and >t2=int32#12,<r3=int32#6,<r4=int32#7
# asm 2: and >t2=r11,<r3=r5,<r4=r6
and r11,r5,r6

# qhasm: t3 = r2 ^ (r2 >>> 11)
# asm 1: eor >t3=int32#13,<r2=int32#5,<r2=int32#5,ROR #11
# asm 2: eor >t3=r12,<r2=r4,<r2=r4,ROR #11
eor r12,r4,r4,ROR #11

# qhasm: t3 ^= (r2 >>> 20)
# asm 1: eor >t3=int32#13,<t3=int32#13,<r2=int32#5,ROR #20
# asm 2: eor >t3=r12,<t3=r12,<r2=r4,ROR #20
eor r12,r12,r4,ROR #20

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t0 += (t3 >>> 2)
# asm 1: add >t0=int32#11,<t0=int32#11,<t3=int32#13,ROR #2
# asm 2: add >t0=r10,<t0=r10,<t3=r12,ROR #2
add r10,r10,r12,ROR #2

# qhasm: r1 += t0
# asm 1: add >r1=int32#3,<r1=int32#3,<t0=int32#11
# asm 2: add >r1=r2,<r1=r2,<t0=r10
add r2,r2,r10

# qhasm: w7 = w7_stack
# asm 1: ldr >w7=int32#11,<w7_stack=stack32#28
# asm 2: ldr >w7=r10,<w7_stack=[sp,#108]
ldr r10,[sp,#108]

# qhasm: t0 = r5 & r6
# asm 1: and >t0=int32#12,<r5=int32#8,<r6=int32#9
# asm 2: and >t0=r11,<r5=r7,<r6=r8
and r11,r7,r8

# qhasm: t1 = r7 & ~r5
# asm 1: bic >t1=int32#13,<r7=int32#10,<r5=int32#8
# asm 2: bic >t1=r12,<r7=r9,<r5=r7
bic r12,r9,r7

# qhasm: t0 ^= t1
# asm 1: eor >t0=int32#12,<t0=int32#12,<t1=int32#13
# asm 2: eor >t0=r11,<t0=r11,<t1=r12
eor r11,r11,r12

# qhasm: t1 = r5 ^ (r5 >>> 5)
# asm 1: eor >t1=int32#13,<r5=int32#8,<r5=int32#8,ROR #5
# asm 2: eor >t1=r12,<r5=r7,<r5=r7,ROR #5
eor r12,r7,r7,ROR #5

# qhasm: t1 ^= (r5 >>> 19)
# asm 1: eor >t1=int32#13,<t1=int32#13,<r5=int32#8,ROR #19
# asm 2: eor >t1=r12,<t1=r12,<r5=r7,ROR #19
eor r12,r12,r7,ROR #19

# qhasm: t2 = mem32[input_3 + 220]
# asm 1: ldr >t2=int32#14,[<input_3=int32#4,#220]
# asm 2: ldr >t2=r14,[<input_3=r3,#220]
ldr r14,[r3,#220]

# qhasm: t0 += w7
# asm 1: add >t0=int32#11,<t0=int32#12,<w7=int32#11
# asm 2: add >t0=r10,<t0=r11,<w7=r10
add r10,r11,r10

# qhasm: t0 += (t1 >>> 6)
# asm 1: add >t0=int32#11,<t0=int32#11,<t1=int32#13,ROR #6
# asm 2: add >t0=r10,<t0=r10,<t1=r12,ROR #6
add r10,r10,r12,ROR #6

# qhasm: t0 += t2
# asm 1: add >t0=int32#11,<t0=int32#11,<t2=int32#14
# asm 2: add >t0=r10,<t0=r10,<t2=r14
add r10,r10,r14

# qhasm: r0 += t0
# asm 1: add >r0=int32#1,<r0=int32#1,<t0=int32#11
# asm 2: add >r0=r0,<r0=r0,<t0=r10
add r0,r0,r10

# qhasm: r4 += r0
# asm 1: add >r4=int32#7,<r4=int32#7,<r0=int32#1
# asm 2: add >r4=r6,<r4=r6,<r0=r0
add r6,r6,r0

# qhasm: t0 = r1 & r2
# asm 1: and >t0=int32#11,<r1=int32#3,<r2=int32#5
# asm 2: and >t0=r10,<r1=r2,<r2=r4
and r10,r2,r4

# qhasm: t2 = r1 & r3
# asm 1: and >t2=int32#12,<r1=int32#3,<r3=int32#6
# asm 2: and >t2=r11,<r1=r2,<r3=r5
and r11,r2,r5

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t2 = r2 & r3
# asm 1: and >t2=int32#12,<r2=int32#5,<r3=int32#6
# asm 2: and >t2=r11,<r2=r4,<r3=r5
and r11,r4,r5

# qhasm: t3 = r1 ^ (r1 >>> 11)
# asm 1: eor >t3=int32#13,<r1=int32#3,<r1=int32#3,ROR #11
# asm 2: eor >t3=r12,<r1=r2,<r1=r2,ROR #11
eor r12,r2,r2,ROR #11

# qhasm: t3 ^= (r1 >>> 20)
# asm 1: eor >t3=int32#13,<t3=int32#13,<r1=int32#3,ROR #20
# asm 2: eor >t3=r12,<t3=r12,<r1=r2,ROR #20
eor r12,r12,r2,ROR #20

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t0 += (t3 >>> 2)
# asm 1: add >t0=int32#11,<t0=int32#11,<t3=int32#13,ROR #2
# asm 2: add >t0=r10,<t0=r10,<t3=r12,ROR #2
add r10,r10,r12,ROR #2

# qhasm: r0 += t0
# asm 1: add >r0=int32#1,<r0=int32#1,<t0=int32#11
# asm 2: add >r0=r0,<r0=r0,<t0=r10
add r0,r0,r10

# qhasm: w8 = w8_stack
# asm 1: ldr >w8=int32#11,<w8_stack=stack32#29
# asm 2: ldr >w8=r10,<w8_stack=[sp,#112]
ldr r10,[sp,#112]

# qhasm: t0 = r4 & r5
# asm 1: and >t0=int32#12,<r4=int32#7,<r5=int32#8
# asm 2: and >t0=r11,<r4=r6,<r5=r7
and r11,r6,r7

# qhasm: t1 = r6 & ~r4
# asm 1: bic >t1=int32#13,<r6=int32#9,<r4=int32#7
# asm 2: bic >t1=r12,<r6=r8,<r4=r6
bic r12,r8,r6

# qhasm: t0 ^= t1
# asm 1: eor >t0=int32#12,<t0=int32#12,<t1=int32#13
# asm 2: eor >t0=r11,<t0=r11,<t1=r12
eor r11,r11,r12

# qhasm: t1 = r4 ^ (r4 >>> 5)
# asm 1: eor >t1=int32#13,<r4=int32#7,<r4=int32#7,ROR #5
# asm 2: eor >t1=r12,<r4=r6,<r4=r6,ROR #5
eor r12,r6,r6,ROR #5

# qhasm: t1 ^= (r4 >>> 19)
# asm 1: eor >t1=int32#13,<t1=int32#13,<r4=int32#7,ROR #19
# asm 2: eor >t1=r12,<t1=r12,<r4=r6,ROR #19
eor r12,r12,r6,ROR #19

# qhasm: t2 = mem32[input_3 + 224]
# asm 1: ldr >t2=int32#14,[<input_3=int32#4,#224]
# asm 2: ldr >t2=r14,[<input_3=r3,#224]
ldr r14,[r3,#224]

# qhasm: t0 += w8
# asm 1: add >t0=int32#11,<t0=int32#12,<w8=int32#11
# asm 2: add >t0=r10,<t0=r11,<w8=r10
add r10,r11,r10

# qhasm: t0 += (t1 >>> 6)
# asm 1: add >t0=int32#11,<t0=int32#11,<t1=int32#13,ROR #6
# asm 2: add >t0=r10,<t0=r10,<t1=r12,ROR #6
add r10,r10,r12,ROR #6

# qhasm: t0 += t2
# asm 1: add >t0=int32#11,<t0=int32#11,<t2=int32#14
# asm 2: add >t0=r10,<t0=r10,<t2=r14
add r10,r10,r14

# qhasm: r7 += t0
# asm 1: add >r7=int32#10,<r7=int32#10,<t0=int32#11
# asm 2: add >r7=r9,<r7=r9,<t0=r10
add r9,r9,r10

# qhasm: r3 += r7
# asm 1: add >r3=int32#6,<r3=int32#6,<r7=int32#10
# asm 2: add >r3=r5,<r3=r5,<r7=r9
add r5,r5,r9

# qhasm: t0 = r0 & r1
# asm 1: and >t0=int32#11,<r0=int32#1,<r1=int32#3
# asm 2: and >t0=r10,<r0=r0,<r1=r2
and r10,r0,r2

# qhasm: t2 = r0 & r2
# asm 1: and >t2=int32#12,<r0=int32#1,<r2=int32#5
# asm 2: and >t2=r11,<r0=r0,<r2=r4
and r11,r0,r4

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t2 = r1 & r2
# asm 1: and >t2=int32#12,<r1=int32#3,<r2=int32#5
# asm 2: and >t2=r11,<r1=r2,<r2=r4
and r11,r2,r4

# qhasm: t3 = r0 ^ (r0 >>> 11)
# asm 1: eor >t3=int32#13,<r0=int32#1,<r0=int32#1,ROR #11
# asm 2: eor >t3=r12,<r0=r0,<r0=r0,ROR #11
eor r12,r0,r0,ROR #11

# qhasm: t3 ^= (r0 >>> 20)
# asm 1: eor >t3=int32#13,<t3=int32#13,<r0=int32#1,ROR #20
# asm 2: eor >t3=r12,<t3=r12,<r0=r0,ROR #20
eor r12,r12,r0,ROR #20

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t0 += (t3 >>> 2)
# asm 1: add >t0=int32#11,<t0=int32#11,<t3=int32#13,ROR #2
# asm 2: add >t0=r10,<t0=r10,<t3=r12,ROR #2
add r10,r10,r12,ROR #2

# qhasm: r7 += t0
# asm 1: add >r7=int32#10,<r7=int32#10,<t0=int32#11
# asm 2: add >r7=r9,<r7=r9,<t0=r10
add r9,r9,r10

# qhasm: w9 = w9_stack
# asm 1: ldr >w9=int32#11,<w9_stack=stack32#20
# asm 2: ldr >w9=r10,<w9_stack=[sp,#76]
ldr r10,[sp,#76]

# qhasm: t0 = r3 & r4
# asm 1: and >t0=int32#12,<r3=int32#6,<r4=int32#7
# asm 2: and >t0=r11,<r3=r5,<r4=r6
and r11,r5,r6

# qhasm: t1 = r5 & ~r3
# asm 1: bic >t1=int32#13,<r5=int32#8,<r3=int32#6
# asm 2: bic >t1=r12,<r5=r7,<r3=r5
bic r12,r7,r5

# qhasm: t0 ^= t1
# asm 1: eor >t0=int32#12,<t0=int32#12,<t1=int32#13
# asm 2: eor >t0=r11,<t0=r11,<t1=r12
eor r11,r11,r12

# qhasm: t1 = r3 ^ (r3 >>> 5)
# asm 1: eor >t1=int32#13,<r3=int32#6,<r3=int32#6,ROR #5
# asm 2: eor >t1=r12,<r3=r5,<r3=r5,ROR #5
eor r12,r5,r5,ROR #5

# qhasm: t1 ^= (r3 >>> 19)
# asm 1: eor >t1=int32#13,<t1=int32#13,<r3=int32#6,ROR #19
# asm 2: eor >t1=r12,<t1=r12,<r3=r5,ROR #19
eor r12,r12,r5,ROR #19

# qhasm: t2 = mem32[input_3 + 228]
# asm 1: ldr >t2=int32#14,[<input_3=int32#4,#228]
# asm 2: ldr >t2=r14,[<input_3=r3,#228]
ldr r14,[r3,#228]

# qhasm: t0 += w9
# asm 1: add >t0=int32#11,<t0=int32#12,<w9=int32#11
# asm 2: add >t0=r10,<t0=r11,<w9=r10
add r10,r11,r10

# qhasm: t0 += (t1 >>> 6)
# asm 1: add >t0=int32#11,<t0=int32#11,<t1=int32#13,ROR #6
# asm 2: add >t0=r10,<t0=r10,<t1=r12,ROR #6
add r10,r10,r12,ROR #6

# qhasm: t0 += t2
# asm 1: add >t0=int32#11,<t0=int32#11,<t2=int32#14
# asm 2: add >t0=r10,<t0=r10,<t2=r14
add r10,r10,r14

# qhasm: r6 += t0
# asm 1: add >r6=int32#9,<r6=int32#9,<t0=int32#11
# asm 2: add >r6=r8,<r6=r8,<t0=r10
add r8,r8,r10

# qhasm: r2 += r6
# asm 1: add >r2=int32#5,<r2=int32#5,<r6=int32#9
# asm 2: add >r2=r4,<r2=r4,<r6=r8
add r4,r4,r8

# qhasm: t0 = r7 & r0
# asm 1: and >t0=int32#11,<r7=int32#10,<r0=int32#1
# asm 2: and >t0=r10,<r7=r9,<r0=r0
and r10,r9,r0

# qhasm: t2 = r7 & r1
# asm 1: and >t2=int32#12,<r7=int32#10,<r1=int32#3
# asm 2: and >t2=r11,<r7=r9,<r1=r2
and r11,r9,r2

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t2 = r0 & r1
# asm 1: and >t2=int32#12,<r0=int32#1,<r1=int32#3
# asm 2: and >t2=r11,<r0=r0,<r1=r2
and r11,r0,r2

# qhasm: t3 = r7 ^ (r7 >>> 11)
# asm 1: eor >t3=int32#13,<r7=int32#10,<r7=int32#10,ROR #11
# asm 2: eor >t3=r12,<r7=r9,<r7=r9,ROR #11
eor r12,r9,r9,ROR #11

# qhasm: t3 ^= (r7 >>> 20)
# asm 1: eor >t3=int32#13,<t3=int32#13,<r7=int32#10,ROR #20
# asm 2: eor >t3=r12,<t3=r12,<r7=r9,ROR #20
eor r12,r12,r9,ROR #20

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t0 += (t3 >>> 2)
# asm 1: add >t0=int32#11,<t0=int32#11,<t3=int32#13,ROR #2
# asm 2: add >t0=r10,<t0=r10,<t3=r12,ROR #2
add r10,r10,r12,ROR #2

# qhasm: r6 += t0
# asm 1: add >r6=int32#9,<r6=int32#9,<t0=int32#11
# asm 2: add >r6=r8,<r6=r8,<t0=r10
add r8,r8,r10

# qhasm: w10 = w10_stack
# asm 1: ldr >w10=int32#11,<w10_stack=stack32#30
# asm 2: ldr >w10=r10,<w10_stack=[sp,#116]
ldr r10,[sp,#116]

# qhasm: t0 = r2 & r3
# asm 1: and >t0=int32#12,<r2=int32#5,<r3=int32#6
# asm 2: and >t0=r11,<r2=r4,<r3=r5
and r11,r4,r5

# qhasm: t1 = r4 & ~r2
# asm 1: bic >t1=int32#13,<r4=int32#7,<r2=int32#5
# asm 2: bic >t1=r12,<r4=r6,<r2=r4
bic r12,r6,r4

# qhasm: t0 ^= t1
# asm 1: eor >t0=int32#12,<t0=int32#12,<t1=int32#13
# asm 2: eor >t0=r11,<t0=r11,<t1=r12
eor r11,r11,r12

# qhasm: t1 = r2 ^ (r2 >>> 5)
# asm 1: eor >t1=int32#13,<r2=int32#5,<r2=int32#5,ROR #5
# asm 2: eor >t1=r12,<r2=r4,<r2=r4,ROR #5
eor r12,r4,r4,ROR #5

# qhasm: t1 ^= (r2 >>> 19)
# asm 1: eor >t1=int32#13,<t1=int32#13,<r2=int32#5,ROR #19
# asm 2: eor >t1=r12,<t1=r12,<r2=r4,ROR #19
eor r12,r12,r4,ROR #19

# qhasm: t2 = mem32[input_3 + 232]
# asm 1: ldr >t2=int32#14,[<input_3=int32#4,#232]
# asm 2: ldr >t2=r14,[<input_3=r3,#232]
ldr r14,[r3,#232]

# qhasm: t0 += w10
# asm 1: add >t0=int32#11,<t0=int32#12,<w10=int32#11
# asm 2: add >t0=r10,<t0=r11,<w10=r10
add r10,r11,r10

# qhasm: t0 += (t1 >>> 6)
# asm 1: add >t0=int32#11,<t0=int32#11,<t1=int32#13,ROR #6
# asm 2: add >t0=r10,<t0=r10,<t1=r12,ROR #6
add r10,r10,r12,ROR #6

# qhasm: t0 += t2
# asm 1: add >t0=int32#11,<t0=int32#11,<t2=int32#14
# asm 2: add >t0=r10,<t0=r10,<t2=r14
add r10,r10,r14

# qhasm: r5 += t0
# asm 1: add >r5=int32#8,<r5=int32#8,<t0=int32#11
# asm 2: add >r5=r7,<r5=r7,<t0=r10
add r7,r7,r10

# qhasm: r1 += r5
# asm 1: add >r1=int32#3,<r1=int32#3,<r5=int32#8
# asm 2: add >r1=r2,<r1=r2,<r5=r7
add r2,r2,r7

# qhasm: t0 = r6 & r7
# asm 1: and >t0=int32#11,<r6=int32#9,<r7=int32#10
# asm 2: and >t0=r10,<r6=r8,<r7=r9
and r10,r8,r9

# qhasm: t2 = r6 & r0
# asm 1: and >t2=int32#12,<r6=int32#9,<r0=int32#1
# asm 2: and >t2=r11,<r6=r8,<r0=r0
and r11,r8,r0

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t2 = r7 & r0
# asm 1: and >t2=int32#12,<r7=int32#10,<r0=int32#1
# asm 2: and >t2=r11,<r7=r9,<r0=r0
and r11,r9,r0

# qhasm: t3 = r6 ^ (r6 >>> 11)
# asm 1: eor >t3=int32#13,<r6=int32#9,<r6=int32#9,ROR #11
# asm 2: eor >t3=r12,<r6=r8,<r6=r8,ROR #11
eor r12,r8,r8,ROR #11

# qhasm: t3 ^= (r6 >>> 20)
# asm 1: eor >t3=int32#13,<t3=int32#13,<r6=int32#9,ROR #20
# asm 2: eor >t3=r12,<t3=r12,<r6=r8,ROR #20
eor r12,r12,r8,ROR #20

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t0 += (t3 >>> 2)
# asm 1: add >t0=int32#11,<t0=int32#11,<t3=int32#13,ROR #2
# asm 2: add >t0=r10,<t0=r10,<t3=r12,ROR #2
add r10,r10,r12,ROR #2

# qhasm: r5 += t0
# asm 1: add >r5=int32#8,<r5=int32#8,<t0=int32#11
# asm 2: add >r5=r7,<r5=r7,<t0=r10
add r7,r7,r10

# qhasm: w11 = w11_stack
# asm 1: ldr >w11=int32#11,<w11_stack=stack32#31
# asm 2: ldr >w11=r10,<w11_stack=[sp,#120]
ldr r10,[sp,#120]

# qhasm: t0 = r1 & r2
# asm 1: and >t0=int32#12,<r1=int32#3,<r2=int32#5
# asm 2: and >t0=r11,<r1=r2,<r2=r4
and r11,r2,r4

# qhasm: t1 = r3 & ~r1
# asm 1: bic >t1=int32#13,<r3=int32#6,<r1=int32#3
# asm 2: bic >t1=r12,<r3=r5,<r1=r2
bic r12,r5,r2

# qhasm: t0 ^= t1
# asm 1: eor >t0=int32#12,<t0=int32#12,<t1=int32#13
# asm 2: eor >t0=r11,<t0=r11,<t1=r12
eor r11,r11,r12

# qhasm: t1 = r1 ^ (r1 >>> 5)
# asm 1: eor >t1=int32#13,<r1=int32#3,<r1=int32#3,ROR #5
# asm 2: eor >t1=r12,<r1=r2,<r1=r2,ROR #5
eor r12,r2,r2,ROR #5

# qhasm: t1 ^= (r1 >>> 19)
# asm 1: eor >t1=int32#13,<t1=int32#13,<r1=int32#3,ROR #19
# asm 2: eor >t1=r12,<t1=r12,<r1=r2,ROR #19
eor r12,r12,r2,ROR #19

# qhasm: t2 = mem32[input_3 + 236]
# asm 1: ldr >t2=int32#14,[<input_3=int32#4,#236]
# asm 2: ldr >t2=r14,[<input_3=r3,#236]
ldr r14,[r3,#236]

# qhasm: t0 += w11
# asm 1: add >t0=int32#11,<t0=int32#12,<w11=int32#11
# asm 2: add >t0=r10,<t0=r11,<w11=r10
add r10,r11,r10

# qhasm: t0 += (t1 >>> 6)
# asm 1: add >t0=int32#11,<t0=int32#11,<t1=int32#13,ROR #6
# asm 2: add >t0=r10,<t0=r10,<t1=r12,ROR #6
add r10,r10,r12,ROR #6

# qhasm: t0 += t2
# asm 1: add >t0=int32#11,<t0=int32#11,<t2=int32#14
# asm 2: add >t0=r10,<t0=r10,<t2=r14
add r10,r10,r14

# qhasm: r4 += t0
# asm 1: add >r4=int32#7,<r4=int32#7,<t0=int32#11
# asm 2: add >r4=r6,<r4=r6,<t0=r10
add r6,r6,r10

# qhasm: r0 += r4
# asm 1: add >r0=int32#1,<r0=int32#1,<r4=int32#7
# asm 2: add >r0=r0,<r0=r0,<r4=r6
add r0,r0,r6

# qhasm: t0 = r5 & r6
# asm 1: and >t0=int32#11,<r5=int32#8,<r6=int32#9
# asm 2: and >t0=r10,<r5=r7,<r6=r8
and r10,r7,r8

# qhasm: t2 = r5 & r7
# asm 1: and >t2=int32#12,<r5=int32#8,<r7=int32#10
# asm 2: and >t2=r11,<r5=r7,<r7=r9
and r11,r7,r9

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t2 = r6 & r7
# asm 1: and >t2=int32#12,<r6=int32#9,<r7=int32#10
# asm 2: and >t2=r11,<r6=r8,<r7=r9
and r11,r8,r9

# qhasm: t3 = r5 ^ (r5 >>> 11)
# asm 1: eor >t3=int32#13,<r5=int32#8,<r5=int32#8,ROR #11
# asm 2: eor >t3=r12,<r5=r7,<r5=r7,ROR #11
eor r12,r7,r7,ROR #11

# qhasm: t3 ^= (r5 >>> 20)
# asm 1: eor >t3=int32#13,<t3=int32#13,<r5=int32#8,ROR #20
# asm 2: eor >t3=r12,<t3=r12,<r5=r7,ROR #20
eor r12,r12,r7,ROR #20

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#11,<t0=int32#11,<t2=int32#12
# asm 2: eor >t0=r10,<t0=r10,<t2=r11
eor r10,r10,r11

# qhasm: t0 += (t3 >>> 2)
# asm 1: add >t0=int32#11,<t0=int32#11,<t3=int32#13,ROR #2
# asm 2: add >t0=r10,<t0=r10,<t3=r12,ROR #2
add r10,r10,r12,ROR #2

# qhasm: r4 += t0
# asm 1: add >r4=int32#7,<r4=int32#7,<t0=int32#11
# asm 2: add >r4=r6,<r4=r6,<t0=r10
add r6,r6,r10

# qhasm: w12 = w12_stack
# asm 1: ldr >w12=int32#11,<w12_stack=stack32#32
# asm 2: ldr >w12=r10,<w12_stack=[sp,#124]
ldr r10,[sp,#124]

# qhasm: t0 = r0 & r1
# asm 1: and >t0=int32#12,<r0=int32#1,<r1=int32#3
# asm 2: and >t0=r11,<r0=r0,<r1=r2
and r11,r0,r2

# qhasm: t1 = r2 & ~r0
# asm 1: bic >t1=int32#13,<r2=int32#5,<r0=int32#1
# asm 2: bic >t1=r12,<r2=r4,<r0=r0
bic r12,r4,r0

# qhasm: t0 ^= t1
# asm 1: eor >t0=int32#12,<t0=int32#12,<t1=int32#13
# asm 2: eor >t0=r11,<t0=r11,<t1=r12
eor r11,r11,r12

# qhasm: t1 = r0 ^ (r0 >>> 5)
# asm 1: eor >t1=int32#13,<r0=int32#1,<r0=int32#1,ROR #5
# asm 2: eor >t1=r12,<r0=r0,<r0=r0,ROR #5
eor r12,r0,r0,ROR #5

# qhasm: t1 ^= (r0 >>> 19)
# asm 1: eor >t1=int32#13,<t1=int32#13,<r0=int32#1,ROR #19
# asm 2: eor >t1=r12,<t1=r12,<r0=r0,ROR #19
eor r12,r12,r0,ROR #19

# qhasm: t2 = mem32[input_3 + 240]
# asm 1: ldr >t2=int32#14,[<input_3=int32#4,#240]
# asm 2: ldr >t2=r14,[<input_3=r3,#240]
ldr r14,[r3,#240]

# qhasm: t0 += w12
# asm 1: add >t0=int32#11,<t0=int32#12,<w12=int32#11
# asm 2: add >t0=r10,<t0=r11,<w12=r10
add r10,r11,r10

# qhasm: t0 += (t1 >>> 6)
# asm 1: add >t0=int32#11,<t0=int32#11,<t1=int32#13,ROR #6
# asm 2: add >t0=r10,<t0=r10,<t1=r12,ROR #6
add r10,r10,r12,ROR #6

# qhasm: t0 += t2
# asm 1: add >t0=int32#11,<t0=int32#11,<t2=int32#14
# asm 2: add >t0=r10,<t0=r10,<t2=r14
add r10,r10,r14

# qhasm: r3 += t0
# asm 1: add >r3=int32#6,<r3=int32#6,<t0=int32#11
# asm 2: add >r3=r5,<r3=r5,<t0=r10
add r5,r5,r10

# qhasm: r7 += r3
# asm 1: add >r7=int32#12,<r7=int32#10,<r3=int32#6
# asm 2: add >r7=r11,<r7=r9,<r3=r5
add r11,r9,r5

# qhasm: t0 = r4 & r5
# asm 1: and >t0=int32#10,<r4=int32#7,<r5=int32#8
# asm 2: and >t0=r9,<r4=r6,<r5=r7
and r9,r6,r7

# qhasm: t2 = r4 & r6
# asm 1: and >t2=int32#11,<r4=int32#7,<r6=int32#9
# asm 2: and >t2=r10,<r4=r6,<r6=r8
and r10,r6,r8

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#10,<t0=int32#10,<t2=int32#11
# asm 2: eor >t0=r9,<t0=r9,<t2=r10
eor r9,r9,r10

# qhasm: t2 = r5 & r6
# asm 1: and >t2=int32#11,<r5=int32#8,<r6=int32#9
# asm 2: and >t2=r10,<r5=r7,<r6=r8
and r10,r7,r8

# qhasm: t3 = r4 ^ (r4 >>> 11)
# asm 1: eor >t3=int32#13,<r4=int32#7,<r4=int32#7,ROR #11
# asm 2: eor >t3=r12,<r4=r6,<r4=r6,ROR #11
eor r12,r6,r6,ROR #11

# qhasm: t3 ^= (r4 >>> 20)
# asm 1: eor >t3=int32#13,<t3=int32#13,<r4=int32#7,ROR #20
# asm 2: eor >t3=r12,<t3=r12,<r4=r6,ROR #20
eor r12,r12,r6,ROR #20

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#10,<t0=int32#10,<t2=int32#11
# asm 2: eor >t0=r9,<t0=r9,<t2=r10
eor r9,r9,r10

# qhasm: t0 += (t3 >>> 2)
# asm 1: add >t0=int32#10,<t0=int32#10,<t3=int32#13,ROR #2
# asm 2: add >t0=r9,<t0=r9,<t3=r12,ROR #2
add r9,r9,r12,ROR #2

# qhasm: r3 += t0
# asm 1: add >r3=int32#10,<r3=int32#6,<t0=int32#10
# asm 2: add >r3=r9,<r3=r5,<t0=r9
add r9,r5,r9

# qhasm: w13 = w13_stack
# asm 1: ldr >w13=int32#6,<w13_stack=stack32#33
# asm 2: ldr >w13=r5,<w13_stack=[sp,#128]
ldr r5,[sp,#128]

# qhasm: t0 = r7 & r0
# asm 1: and >t0=int32#11,<r7=int32#12,<r0=int32#1
# asm 2: and >t0=r10,<r7=r11,<r0=r0
and r10,r11,r0

# qhasm: t1 = r1 & ~r7
# asm 1: bic >t1=int32#13,<r1=int32#3,<r7=int32#12
# asm 2: bic >t1=r12,<r1=r2,<r7=r11
bic r12,r2,r11

# qhasm: t0 ^= t1
# asm 1: eor >t0=int32#11,<t0=int32#11,<t1=int32#13
# asm 2: eor >t0=r10,<t0=r10,<t1=r12
eor r10,r10,r12

# qhasm: t1 = r7 ^ (r7 >>> 5)
# asm 1: eor >t1=int32#13,<r7=int32#12,<r7=int32#12,ROR #5
# asm 2: eor >t1=r12,<r7=r11,<r7=r11,ROR #5
eor r12,r11,r11,ROR #5

# qhasm: t1 ^= (r7 >>> 19)
# asm 1: eor >t1=int32#13,<t1=int32#13,<r7=int32#12,ROR #19
# asm 2: eor >t1=r12,<t1=r12,<r7=r11,ROR #19
eor r12,r12,r11,ROR #19

# qhasm: t2 = mem32[input_3 + 244]
# asm 1: ldr >t2=int32#14,[<input_3=int32#4,#244]
# asm 2: ldr >t2=r14,[<input_3=r3,#244]
ldr r14,[r3,#244]

# qhasm: t0 += w13
# asm 1: add >t0=int32#6,<t0=int32#11,<w13=int32#6
# asm 2: add >t0=r5,<t0=r10,<w13=r5
add r5,r10,r5

# qhasm: t0 += (t1 >>> 6)
# asm 1: add >t0=int32#6,<t0=int32#6,<t1=int32#13,ROR #6
# asm 2: add >t0=r5,<t0=r5,<t1=r12,ROR #6
add r5,r5,r12,ROR #6

# qhasm: t0 += t2
# asm 1: add >t0=int32#6,<t0=int32#6,<t2=int32#14
# asm 2: add >t0=r5,<t0=r5,<t2=r14
add r5,r5,r14

# qhasm: r2 += t0
# asm 1: add >r2=int32#5,<r2=int32#5,<t0=int32#6
# asm 2: add >r2=r4,<r2=r4,<t0=r5
add r4,r4,r5

# qhasm: r6 += r2
# asm 1: add >r6=int32#11,<r6=int32#9,<r2=int32#5
# asm 2: add >r6=r10,<r6=r8,<r2=r4
add r10,r8,r4

# qhasm: t0 = r3 & r4
# asm 1: and >t0=int32#6,<r3=int32#10,<r4=int32#7
# asm 2: and >t0=r5,<r3=r9,<r4=r6
and r5,r9,r6

# qhasm: t2 = r3 & r5
# asm 1: and >t2=int32#9,<r3=int32#10,<r5=int32#8
# asm 2: and >t2=r8,<r3=r9,<r5=r7
and r8,r9,r7

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#6,<t0=int32#6,<t2=int32#9
# asm 2: eor >t0=r5,<t0=r5,<t2=r8
eor r5,r5,r8

# qhasm: t2 = r4 & r5
# asm 1: and >t2=int32#9,<r4=int32#7,<r5=int32#8
# asm 2: and >t2=r8,<r4=r6,<r5=r7
and r8,r6,r7

# qhasm: t3 = r3 ^ (r3 >>> 11)
# asm 1: eor >t3=int32#13,<r3=int32#10,<r3=int32#10,ROR #11
# asm 2: eor >t3=r12,<r3=r9,<r3=r9,ROR #11
eor r12,r9,r9,ROR #11

# qhasm: t3 ^= (r3 >>> 20)
# asm 1: eor >t3=int32#13,<t3=int32#13,<r3=int32#10,ROR #20
# asm 2: eor >t3=r12,<t3=r12,<r3=r9,ROR #20
eor r12,r12,r9,ROR #20

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#6,<t0=int32#6,<t2=int32#9
# asm 2: eor >t0=r5,<t0=r5,<t2=r8
eor r5,r5,r8

# qhasm: t0 += (t3 >>> 2)
# asm 1: add >t0=int32#6,<t0=int32#6,<t3=int32#13,ROR #2
# asm 2: add >t0=r5,<t0=r5,<t3=r12,ROR #2
add r5,r5,r12,ROR #2

# qhasm: r2 += t0
# asm 1: add >r2=int32#9,<r2=int32#5,<t0=int32#6
# asm 2: add >r2=r8,<r2=r4,<t0=r5
add r8,r4,r5

# qhasm: w14 = w14_stack
# asm 1: ldr >w14=int32#5,<w14_stack=stack32#34
# asm 2: ldr >w14=r4,<w14_stack=[sp,#132]
ldr r4,[sp,#132]

# qhasm: t0 = r6 & r7
# asm 1: and >t0=int32#6,<r6=int32#11,<r7=int32#12
# asm 2: and >t0=r5,<r6=r10,<r7=r11
and r5,r10,r11

# qhasm: t1 = r0 & ~r6
# asm 1: bic >t1=int32#13,<r0=int32#1,<r6=int32#11
# asm 2: bic >t1=r12,<r0=r0,<r6=r10
bic r12,r0,r10

# qhasm: t0 ^= t1
# asm 1: eor >t0=int32#6,<t0=int32#6,<t1=int32#13
# asm 2: eor >t0=r5,<t0=r5,<t1=r12
eor r5,r5,r12

# qhasm: t1 = r6 ^ (r6 >>> 5)
# asm 1: eor >t1=int32#13,<r6=int32#11,<r6=int32#11,ROR #5
# asm 2: eor >t1=r12,<r6=r10,<r6=r10,ROR #5
eor r12,r10,r10,ROR #5

# qhasm: t1 ^= (r6 >>> 19)
# asm 1: eor >t1=int32#13,<t1=int32#13,<r6=int32#11,ROR #19
# asm 2: eor >t1=r12,<t1=r12,<r6=r10,ROR #19
eor r12,r12,r10,ROR #19

# qhasm: t2 = mem32[input_3 + 248]
# asm 1: ldr >t2=int32#14,[<input_3=int32#4,#248]
# asm 2: ldr >t2=r14,[<input_3=r3,#248]
ldr r14,[r3,#248]

# qhasm: t0 += w14
# asm 1: add >t0=int32#5,<t0=int32#6,<w14=int32#5
# asm 2: add >t0=r4,<t0=r5,<w14=r4
add r4,r5,r4

# qhasm: t0 += (t1 >>> 6)
# asm 1: add >t0=int32#5,<t0=int32#5,<t1=int32#13,ROR #6
# asm 2: add >t0=r4,<t0=r4,<t1=r12,ROR #6
add r4,r4,r12,ROR #6

# qhasm: t0 += t2
# asm 1: add >t0=int32#5,<t0=int32#5,<t2=int32#14
# asm 2: add >t0=r4,<t0=r4,<t2=r14
add r4,r4,r14

# qhasm: r1 += t0
# asm 1: add >r1=int32#3,<r1=int32#3,<t0=int32#5
# asm 2: add >r1=r2,<r1=r2,<t0=r4
add r2,r2,r4

# qhasm: r5 += r1
# asm 1: add >r5=int32#13,<r5=int32#8,<r1=int32#3
# asm 2: add >r5=r12,<r5=r7,<r1=r2
add r12,r7,r2

# qhasm: t0 = r2 & r3
# asm 1: and >t0=int32#5,<r2=int32#9,<r3=int32#10
# asm 2: and >t0=r4,<r2=r8,<r3=r9
and r4,r8,r9

# qhasm: t2 = r2 & r4
# asm 1: and >t2=int32#6,<r2=int32#9,<r4=int32#7
# asm 2: and >t2=r5,<r2=r8,<r4=r6
and r5,r8,r6

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#5,<t0=int32#5,<t2=int32#6
# asm 2: eor >t0=r4,<t0=r4,<t2=r5
eor r4,r4,r5

# qhasm: t2 = r3 & r4
# asm 1: and >t2=int32#6,<r3=int32#10,<r4=int32#7
# asm 2: and >t2=r5,<r3=r9,<r4=r6
and r5,r9,r6

# qhasm: t3 = r2 ^ (r2 >>> 11)
# asm 1: eor >t3=int32#8,<r2=int32#9,<r2=int32#9,ROR #11
# asm 2: eor >t3=r7,<r2=r8,<r2=r8,ROR #11
eor r7,r8,r8,ROR #11

# qhasm: t3 ^= (r2 >>> 20)
# asm 1: eor >t3=int32#8,<t3=int32#8,<r2=int32#9,ROR #20
# asm 2: eor >t3=r7,<t3=r7,<r2=r8,ROR #20
eor r7,r7,r8,ROR #20

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#5,<t0=int32#5,<t2=int32#6
# asm 2: eor >t0=r4,<t0=r4,<t2=r5
eor r4,r4,r5

# qhasm: t0 += (t3 >>> 2)
# asm 1: add >t0=int32#5,<t0=int32#5,<t3=int32#8,ROR #2
# asm 2: add >t0=r4,<t0=r4,<t3=r7,ROR #2
add r4,r4,r7,ROR #2

# qhasm: r1 += t0
# asm 1: add >r1=int32#3,<r1=int32#3,<t0=int32#5
# asm 2: add >r1=r2,<r1=r2,<t0=r4
add r2,r2,r4

# qhasm: w15 = w15_stack
# asm 1: ldr >w15=int32#5,<w15_stack=stack32#35
# asm 2: ldr >w15=r4,<w15_stack=[sp,#136]
ldr r4,[sp,#136]

# qhasm: t0 = r5 & r6
# asm 1: and >t0=int32#6,<r5=int32#13,<r6=int32#11
# asm 2: and >t0=r5,<r5=r12,<r6=r10
and r5,r12,r10

# qhasm: t1 = r7 & ~r5
# asm 1: bic >t1=int32#8,<r7=int32#12,<r5=int32#13
# asm 2: bic >t1=r7,<r7=r11,<r5=r12
bic r7,r11,r12

# qhasm: t0 ^= t1
# asm 1: eor >t0=int32#6,<t0=int32#6,<t1=int32#8
# asm 2: eor >t0=r5,<t0=r5,<t1=r7
eor r5,r5,r7

# qhasm: t1 = r5 ^ (r5 >>> 5)
# asm 1: eor >t1=int32#8,<r5=int32#13,<r5=int32#13,ROR #5
# asm 2: eor >t1=r7,<r5=r12,<r5=r12,ROR #5
eor r7,r12,r12,ROR #5

# qhasm: t1 ^= (r5 >>> 19)
# asm 1: eor >t1=int32#8,<t1=int32#8,<r5=int32#13,ROR #19
# asm 2: eor >t1=r7,<t1=r7,<r5=r12,ROR #19
eor r7,r7,r12,ROR #19

# qhasm: t2 = mem32[input_3 + 252]
# asm 1: ldr >t2=int32#14,[<input_3=int32#4,#252]
# asm 2: ldr >t2=r14,[<input_3=r3,#252]
ldr r14,[r3,#252]

# qhasm: t0 += w15
# asm 1: add >t0=int32#5,<t0=int32#6,<w15=int32#5
# asm 2: add >t0=r4,<t0=r5,<w15=r4
add r4,r5,r4

# qhasm: t0 += (t1 >>> 6)
# asm 1: add >t0=int32#5,<t0=int32#5,<t1=int32#8,ROR #6
# asm 2: add >t0=r4,<t0=r4,<t1=r7,ROR #6
add r4,r4,r7,ROR #6

# qhasm: t0 += t2
# asm 1: add >t0=int32#5,<t0=int32#5,<t2=int32#14
# asm 2: add >t0=r4,<t0=r4,<t2=r14
add r4,r4,r14

# qhasm: r0 += t0
# asm 1: add >r0=int32#1,<r0=int32#1,<t0=int32#5
# asm 2: add >r0=r0,<r0=r0,<t0=r4
add r0,r0,r4

# qhasm: r4 += r0
# asm 1: add >r4=int32#14,<r4=int32#7,<r0=int32#1
# asm 2: add >r4=r14,<r4=r6,<r0=r0
add r14,r6,r0

# qhasm: t0 = r1 & r2
# asm 1: and >t0=int32#5,<r1=int32#3,<r2=int32#9
# asm 2: and >t0=r4,<r1=r2,<r2=r8
and r4,r2,r8

# qhasm: t2 = r1 & r3
# asm 1: and >t2=int32#6,<r1=int32#3,<r3=int32#10
# asm 2: and >t2=r5,<r1=r2,<r3=r9
and r5,r2,r9

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#5,<t0=int32#5,<t2=int32#6
# asm 2: eor >t0=r4,<t0=r4,<t2=r5
eor r4,r4,r5

# qhasm: t2 = r2 & r3
# asm 1: and >t2=int32#6,<r2=int32#9,<r3=int32#10
# asm 2: and >t2=r5,<r2=r8,<r3=r9
and r5,r8,r9

# qhasm: t3 = r1 ^ (r1 >>> 11)
# asm 1: eor >t3=int32#7,<r1=int32#3,<r1=int32#3,ROR #11
# asm 2: eor >t3=r6,<r1=r2,<r1=r2,ROR #11
eor r6,r2,r2,ROR #11

# qhasm: t3 ^= (r1 >>> 20)
# asm 1: eor >t3=int32#7,<t3=int32#7,<r1=int32#3,ROR #20
# asm 2: eor >t3=r6,<t3=r6,<r1=r2,ROR #20
eor r6,r6,r2,ROR #20

# qhasm: t0 ^= t2
# asm 1: eor >t0=int32#5,<t0=int32#5,<t2=int32#6
# asm 2: eor >t0=r4,<t0=r4,<t2=r5
eor r4,r4,r5

# qhasm: t0 += (t3 >>> 2)
# asm 1: add >t0=int32#5,<t0=int32#5,<t3=int32#7,ROR #2
# asm 2: add >t0=r4,<t0=r4,<t3=r6,ROR #2
add r4,r4,r6,ROR #2

# qhasm: r0 += t0
# asm 1: add >r0=int32#1,<r0=int32#1,<t0=int32#5
# asm 2: add >r0=r0,<r0=r0,<t0=r4
add r0,r0,r4

# qhasm: s0 = s0_stack
# asm 1: ldr >s0=int32#5,<s0_stack=stack32#10
# asm 2: ldr >s0=r4,<s0_stack=[sp,#36]
ldr r4,[sp,#36]

# qhasm: s1 = s1_stack
# asm 1: ldr >s1=int32#6,<s1_stack=stack32#11
# asm 2: ldr >s1=r5,<s1_stack=[sp,#40]
ldr r5,[sp,#40]

# qhasm: r0 += s0
# asm 1: add >r0=int32#5,<r0=int32#1,<s0=int32#5
# asm 2: add >r0=r4,<r0=r0,<s0=r4
add r4,r0,r4

# qhasm: r1 += s1
# asm 1: add >r1=int32#6,<r1=int32#3,<s1=int32#6
# asm 2: add >r1=r5,<r1=r2,<s1=r5
add r5,r2,r5

# qhasm: s0_stack = r0
# asm 1: str <r0=int32#5,>s0_stack=stack32#10
# asm 2: str <r0=r4,>s0_stack=[sp,#36]
str r4,[sp,#36]

# qhasm: s1_stack = r1
# asm 1: str <r1=int32#6,>s1_stack=stack32#11
# asm 2: str <r1=r5,>s1_stack=[sp,#40]
str r5,[sp,#40]

# qhasm: s2 = s2_stack
# asm 1: ldr >s2=int32#1,<s2_stack=stack32#12
# asm 2: ldr >s2=r0,<s2_stack=[sp,#44]
ldr r0,[sp,#44]

# qhasm: s3 = s3_stack
# asm 1: ldr >s3=int32#3,<s3_stack=stack32#13
# asm 2: ldr >s3=r2,<s3_stack=[sp,#48]
ldr r2,[sp,#48]

# qhasm: r2 += s2
# asm 1: add >r2=int32#7,<r2=int32#9,<s2=int32#1
# asm 2: add >r2=r6,<r2=r8,<s2=r0
add r6,r8,r0

# qhasm: r3 += s3
# asm 1: add >r3=int32#8,<r3=int32#10,<s3=int32#3
# asm 2: add >r3=r7,<r3=r9,<s3=r2
add r7,r9,r2

# qhasm: s2_stack = r2
# asm 1: str <r2=int32#7,>s2_stack=stack32#12
# asm 2: str <r2=r6,>s2_stack=[sp,#44]
str r6,[sp,#44]

# qhasm: s3_stack = r3
# asm 1: str <r3=int32#8,>s3_stack=stack32#13
# asm 2: str <r3=r7,>s3_stack=[sp,#48]
str r7,[sp,#48]

# qhasm: s4 = s4_stack
# asm 1: ldr >s4=int32#1,<s4_stack=stack32#14
# asm 2: ldr >s4=r0,<s4_stack=[sp,#52]
ldr r0,[sp,#52]

# qhasm: s5 = s5_stack
# asm 1: ldr >s5=int32#3,<s5_stack=stack32#15
# asm 2: ldr >s5=r2,<s5_stack=[sp,#56]
ldr r2,[sp,#56]

# qhasm: r4 += s4
# asm 1: add >r4=int32#9,<r4=int32#14,<s4=int32#1
# asm 2: add >r4=r8,<r4=r14,<s4=r0
add r8,r14,r0

# qhasm: r5 += s5
# asm 1: add >r5=int32#10,<r5=int32#13,<s5=int32#3
# asm 2: add >r5=r9,<r5=r12,<s5=r2
add r9,r12,r2

# qhasm: s4_stack = r4
# asm 1: str <r4=int32#9,>s4_stack=stack32#14
# asm 2: str <r4=r8,>s4_stack=[sp,#52]
str r8,[sp,#52]

# qhasm: s5_stack = r5
# asm 1: str <r5=int32#10,>s5_stack=stack32#15
# asm 2: str <r5=r9,>s5_stack=[sp,#56]
str r9,[sp,#56]

# qhasm: s6 = s6_stack
# asm 1: ldr >s6=int32#1,<s6_stack=stack32#16
# asm 2: ldr >s6=r0,<s6_stack=[sp,#60]
ldr r0,[sp,#60]

# qhasm: s7 = s7_stack
# asm 1: ldr >s7=int32#3,<s7_stack=stack32#17
# asm 2: ldr >s7=r2,<s7_stack=[sp,#64]
ldr r2,[sp,#64]

# qhasm: input_1 += 64
# asm 1: add >input_1=int32#2,<input_1=int32#2,#64
# asm 2: add >input_1=r1,<input_1=r1,#64
add r1,r1,#64

# qhasm: r6 += s6
# asm 1: add >r6=int32#11,<r6=int32#11,<s6=int32#1
# asm 2: add >r6=r10,<r6=r10,<s6=r0
add r10,r10,r0

# qhasm: r7 += s7
# asm 1: add >r7=int32#12,<r7=int32#12,<s7=int32#3
# asm 2: add >r7=r11,<r7=r11,<s7=r2
add r11,r11,r2

# qhasm: s6_stack = r6
# asm 1: str <r6=int32#11,>s6_stack=stack32#16
# asm 2: str <r6=r10,>s6_stack=[sp,#60]
str r10,[sp,#60]

# qhasm: s7_stack = r7
# asm 1: str <r7=int32#12,>s7_stack=stack32#17
# asm 2: str <r7=r11,>s7_stack=[sp,#64]
str r11,[sp,#64]

# qhasm: goto mainloop
b ._mainloop

# qhasm: end:
._end:

# qhasm: t0 = state_stack
# asm 1: ldr >t0=int32#1,<state_stack=stack32#18
# asm 2: ldr >t0=r0,<state_stack=[sp,#68]
ldr r0,[sp,#68]

# qhasm: bigendian
setend be

# qhasm: mem32[t0 + 0] = r0
# asm 1: str <r0=int32#5,[<t0=int32#1,#0]
# asm 2: str <r0=r4,[<t0=r0,#0]
str r4,[r0,#0]

# qhasm: mem32[t0 + 4] = r1
# asm 1: str <r1=int32#6,[<t0=int32#1,#4]
# asm 2: str <r1=r5,[<t0=r0,#4]
str r5,[r0,#4]

# qhasm: mem32[t0 + 8] = r2
# asm 1: str <r2=int32#7,[<t0=int32#1,#8]
# asm 2: str <r2=r6,[<t0=r0,#8]
str r6,[r0,#8]

# qhasm: mem32[t0 + 12] = r3
# asm 1: str <r3=int32#8,[<t0=int32#1,#12]
# asm 2: str <r3=r7,[<t0=r0,#12]
str r7,[r0,#12]

# qhasm: mem32[t0 + 16] = r4
# asm 1: str <r4=int32#9,[<t0=int32#1,#16]
# asm 2: str <r4=r8,[<t0=r0,#16]
str r8,[r0,#16]

# qhasm: mem32[t0 + 20] = r5
# asm 1: str <r5=int32#10,[<t0=int32#1,#20]
# asm 2: str <r5=r9,[<t0=r0,#20]
str r9,[r0,#20]

# qhasm: mem32[t0 + 24] = r6
# asm 1: str <r6=int32#11,[<t0=int32#1,#24]
# asm 2: str <r6=r10,[<t0=r0,#24]
str r10,[r0,#24]

# qhasm: mem32[t0 + 28] = r7
# asm 1: str <r7=int32#12,[<t0=int32#1,#28]
# asm 2: str <r7=r11,[<t0=r0,#28]
str r11,[r0,#28]

# qhasm: littleendian
setend le

# qhasm: caller_r4 = caller_r4_stack
# asm 1: ldr >caller_r4=int32#5,<caller_r4_stack=stack32#1
# asm 2: ldr >caller_r4=r4,<caller_r4_stack=[sp,#0]
ldr r4,[sp,#0]

# qhasm: caller_r5 = caller_r5_stack
# asm 1: ldr >caller_r5=int32#6,<caller_r5_stack=stack32#2
# asm 2: ldr >caller_r5=r5,<caller_r5_stack=[sp,#4]
ldr r5,[sp,#4]

# qhasm: caller_r6 = caller_r6_stack
# asm 1: ldr >caller_r6=int32#7,<caller_r6_stack=stack32#3
# asm 2: ldr >caller_r6=r6,<caller_r6_stack=[sp,#8]
ldr r6,[sp,#8]

# qhasm: caller_r7 = caller_r7_stack
# asm 1: ldr >caller_r7=int32#8,<caller_r7_stack=stack32#4
# asm 2: ldr >caller_r7=r7,<caller_r7_stack=[sp,#12]
ldr r7,[sp,#12]

# qhasm: caller_r8 = caller_r8_stack
# asm 1: ldr >caller_r8=int32#9,<caller_r8_stack=stack32#5
# asm 2: ldr >caller_r8=r8,<caller_r8_stack=[sp,#16]
ldr r8,[sp,#16]

# qhasm: caller_r9 = caller_r9_stack
# asm 1: ldr >caller_r9=int32#10,<caller_r9_stack=stack32#6
# asm 2: ldr >caller_r9=r9,<caller_r9_stack=[sp,#20]
ldr r9,[sp,#20]

# qhasm: caller_r10 = caller_r10_stack
# asm 1: ldr >caller_r10=int32#11,<caller_r10_stack=stack32#7
# asm 2: ldr >caller_r10=r10,<caller_r10_stack=[sp,#24]
ldr r10,[sp,#24]

# qhasm: caller_r11 = caller_r11_stack
# asm 1: ldr >caller_r11=int32#12,<caller_r11_stack=stack32#8
# asm 2: ldr >caller_r11=r11,<caller_r11_stack=[sp,#28]
ldr r11,[sp,#28]

# qhasm: caller_r14 = caller_r14_stack
# asm 1: ldr >caller_r14=int32#14,<caller_r14_stack=stack32#9
# asm 2: ldr >caller_r14=r14,<caller_r14_stack=[sp,#32]
ldr r14,[sp,#32]

# qhasm: return
add sp,sp,#192
bx lr
