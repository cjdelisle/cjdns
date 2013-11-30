.fpu neon
.text
.p2align 4
sigma:
.word 1634760805
.word 857760878
.word 2036477234
.word 1797285236
.align 2
.global _crypto_stream_salsa20_armneon6_xor
.global crypto_stream_salsa20_armneon6_xor
_crypto_stream_salsa20_armneon6_xor:
crypto_stream_salsa20_armneon6_xor:
vpush {q4,q5,q6,q7}
mov r12,sp
sub sp,sp,#352
and sp,sp,#0xffffffe0
strd r4,[sp,#0]
strd r6,[sp,#8]
strd r8,[sp,#16]
strd r10,[sp,#24]
str r14,[sp,#224]
ldr r14,[r12,#68]
ldr r4,[r12,#64]
str r0,[sp,#228]
str r1,[sp,#232]
str r2,[sp,#236]
str r3,[sp,#240]
str r14,[sp,#244]
str r4,[sp,#248]
str r12,[sp,#252]
ldr r6,[r4,#0]
ldr r7,[r4,#4]
ldr r0,=0
ldr r1,=0
strd r6,[sp,#32]
strd r0,[sp,#40]
ldr r8,[r14,#4]
ldr r9,[r14,#8]
ldr r4,[r14,#20]
ldr r5,[r14,#24]
strd r8,[sp,#48]
strd r4,[sp,#56]
ldr r2,=sigma
ldr r0,[r2,#0]
ldr r1,[r14,#0]
ldr r4,[r14,#12]
ldr r5,[r2,#4]
strd r0,[sp,#64]
strd r4,[sp,#72]
ldr r10,[r2,#8]
ldr r11,[r14,#16]
ldr r8,[r14,#28]
ldr r9,[r2,#12]
strd r10,[sp,#80]
strd r8,[sp,#88]
ldr r0,[sp,#236]
ldr r1,[sp,#240]
vld1.8 {d0-d1},[r14]!
vld1.8 {d2-d3},[r2]
vld1.8 {d4-d5},[r14]
ldr r2,[sp,#248]
vmov.i64 q3,#0
vld1.8 {d6},[r2]
vrev64.i32 d6,d6
vext.32 d7,d6,d4,#1
vext.32 d8,d0,d6,#1
vext.32 d9,d5,d1,#1
vmov d11,d5
vext.32 d10,d1,d0,#0
vext.32 d6,d4,d0,#1
add r2,sp,#256
add r3,sp,#272
vst1.8 {d10-d11},[r2,: 128]
vst1.8 {d8-d9},[r3,: 128]
vmov.i64 q0,#0
cmp r0,#192
blo ._mlenlowbelow192
._mlenatleast192:
add r2,sp,#272
add r3,sp,#256
vld1.8 {d4-d5},[r2,: 128]
vmov.i64 q4,#0xff
vld1.8 {d10-d11},[r3,: 128]
vext.32 d13,d4,d4,#0
vshr.u32 q4,q4,#7
vext.32 d15,d5,d5,#0
vext.32 d17,d4,d4,#0
vadd.i64 q0,q0,q4
vext.32 d19,d5,d5,#0
add r2,sp,#288
vst1.8 {d12-d13},[r2,: 128]
vext.32 d1,d0,d0,#1
add r2,sp,#304
vst1.8 {d14-d15},[r2,: 128]
vext.32 d12,d1,d11,#1
vext.32 d14,d10,d1,#1
vadd.i64 q0,q0,q4
vmov q2,q1
vext.32 d1,d0,d0,#1
vmov q10,q3
vext.32 d16,d1,d11,#1
vmov q11,q2
vext.32 d18,d10,d1,#1
vmov q5,q10
add r2,sp,#320
vst1.8 {d16-d17},[r2,: 128]
vadd.i64 q0,q0,q4
add r2,sp,#336
vst1.8 {d18-d19},[r2,: 128]
str r0,[sp,#236]
str r1,[sp,#240]
ldr r12,=20
ldrd r2,[sp,#48]
ldrd r6,[sp,#32]
ldrd r0,[sp,#64]
ldrd r4,[sp,#72]
ldrd r8,[sp,#88]
ldrd r10,[sp,#80]
strd r2,[sp,#96]
strd r6,[sp,#104]
ldrd r2,[sp,#56]
ldrd r6,[sp,#40]
._mainloop2:
str r12,[sp,#244]
vadd.i32 q4,q10,q2
vadd.i32 q12,q5,q11
add r12,r0,r2
add r14,r5,r1
vshl.i32 q13,q4,#7
vshl.i32 q14,q12,#7
vshr.u32 q4,q4,#25
vshr.u32 q12,q12,#25
eor r4,r4,r12,ROR #25
eor r7,r7,r14,ROR #25
add r12,r4,r0
add r14,r7,r5
veor q7,q7,q13
veor q9,q9,q14
veor q4,q7,q4
veor q7,q9,q12
eor r6,r6,r12,ROR #23
eor r3,r3,r14,ROR #23
add r12,r6,r4
str r7,[sp,#116]
add r7,r3,r7
ldr r14,[sp,#108]
vadd.i32 q9,q2,q4
vadd.i32 q12,q11,q7
vshl.i32 q13,q9,#9
vshl.i32 q14,q12,#9
vshr.u32 q9,q9,#23
vshr.u32 q12,q12,#23
veor q6,q6,q13
veor q8,q8,q14
veor q6,q6,q9
veor q8,q8,q12
eor r2,r2,r12,ROR #19
str r2,[sp,#120]
eor r1,r1,r7,ROR #19
ldr r7,[sp,#96]
add r2,r2,r6
str r6,[sp,#112]
add r6,r1,r3
ldr r12,[sp,#104]
vadd.i32 q9,q4,q6
vext.32 q4,q4,q4,#3
vadd.i32 q12,q7,q8
vshl.i32 q13,q9,#13
vext.32 q7,q7,q7,#3
vshl.i32 q14,q12,#13
eor r0,r0,r2,ROR #14
eor r2,r5,r6,ROR #14
str r3,[sp,#124]
add r3,r10,r12
ldr r5,[sp,#100]
add r6,r9,r11
vshr.u32 q9,q9,#19
vshr.u32 q12,q12,#19
veor q10,q10,q13
veor q5,q5,q14
eor r8,r8,r3,ROR #25
eor r3,r5,r6,ROR #25
add r5,r8,r10
add r6,r3,r9
veor q9,q10,q9
veor q5,q5,q12
eor r5,r7,r5,ROR #23
eor r6,r14,r6,ROR #23
add r7,r5,r8
add r14,r6,r3
vadd.i32 q10,q6,q9
vswp d12,d13
vadd.i32 q12,q8,q5
vshl.i32 q13,q10,#18
vswp d16,d17
vshl.i32 q14,q12,#18
eor r7,r12,r7,ROR #19
eor r11,r11,r14,ROR #19
add r12,r7,r5
add r14,r11,r6
vshr.u32 q10,q10,#14
vext.32 q9,q9,q9,#1
vshr.u32 q12,q12,#14
veor q2,q2,q13
vext.32 q5,q5,q5,#1
veor q11,q11,q14
eor r10,r10,r12,ROR #14
eor r9,r9,r14,ROR #14
add r12,r0,r3
add r14,r2,r4
veor q2,q2,q10
veor q10,q11,q12
eor r1,r1,r12,ROR #25
eor r7,r7,r14,ROR #25
add r12,r1,r0
add r14,r7,r2
vadd.i32 q11,q4,q2
vadd.i32 q12,q7,q10
vshl.i32 q13,q11,#7
vshl.i32 q14,q12,#7
eor r5,r5,r12,ROR #23
eor r6,r6,r14,ROR #23
vshr.u32 q11,q11,#25
vshr.u32 q12,q12,#25
add r12,r5,r1
add r14,r6,r7
veor q9,q9,q13
veor q5,q5,q14
veor q9,q9,q11
veor q5,q5,q12
vadd.i32 q11,q2,q9
vadd.i32 q12,q10,q5
vshl.i32 q13,q11,#9
vshl.i32 q14,q12,#9
eor r3,r3,r12,ROR #19
str r7,[sp,#104]
eor r4,r4,r14,ROR #19
ldr r7,[sp,#112]
add r12,r3,r5
str r6,[sp,#108]
add r6,r4,r6
ldr r14,[sp,#116]
eor r0,r0,r12,ROR #14
str r5,[sp,#96]
eor r5,r2,r6,ROR #14
ldr r2,[sp,#120]
vshr.u32 q11,q11,#23
vshr.u32 q12,q12,#23
veor q6,q6,q13
veor q8,q8,q14
veor q6,q6,q11
veor q8,q8,q12
add r6,r10,r14
add r12,r9,r8
vadd.i32 q11,q9,q6
vext.32 q9,q9,q9,#3
vadd.i32 q12,q5,q8
vshl.i32 q13,q11,#13
vext.32 q5,q5,q5,#3
vshl.i32 q14,q12,#13
vshr.u32 q11,q11,#19
vshr.u32 q12,q12,#19
eor r11,r11,r6,ROR #25
eor r2,r2,r12,ROR #25
add r6,r11,r10
str r3,[sp,#100]
add r3,r2,r9
ldr r12,[sp,#124]
veor q4,q4,q13
veor q7,q7,q14
veor q4,q4,q11
veor q7,q7,q12
eor r6,r7,r6,ROR #23
eor r3,r12,r3,ROR #23
add r7,r6,r11
add r12,r3,r2
vadd.i32 q11,q6,q4
vswp d12,d13
vadd.i32 q12,q8,q7
vshl.i32 q13,q11,#18
vswp d16,d17
vshl.i32 q14,q12,#18
eor r7,r14,r7,ROR #19
eor r8,r8,r12,ROR #19
add r12,r7,r6
add r14,r8,r3
vshr.u32 q11,q11,#14
vext.32 q4,q4,q4,#1
vshr.u32 q12,q12,#14
veor q2,q2,q13
vext.32 q7,q7,q7,#1
veor q10,q10,q14
eor r10,r10,r12,ROR #14
veor q2,q2,q11
eor r9,r9,r14,ROR #14
veor q10,q10,q12
vadd.i32 q11,q9,q2
vadd.i32 q12,q5,q10
add r12,r0,r2
add r14,r5,r1
vshl.i32 q13,q11,#7
vshl.i32 q14,q12,#7
vshr.u32 q11,q11,#25
vshr.u32 q12,q12,#25
eor r4,r4,r12,ROR #25
eor r7,r7,r14,ROR #25
add r12,r4,r0
add r14,r7,r5
veor q4,q4,q13
veor q7,q7,q14
veor q4,q4,q11
veor q7,q7,q12
eor r6,r6,r12,ROR #23
eor r3,r3,r14,ROR #23
add r12,r6,r4
str r7,[sp,#116]
add r7,r3,r7
ldr r14,[sp,#108]
vadd.i32 q11,q2,q4
vadd.i32 q12,q10,q7
vshl.i32 q13,q11,#9
vshl.i32 q14,q12,#9
vshr.u32 q11,q11,#23
vshr.u32 q12,q12,#23
veor q6,q6,q13
veor q8,q8,q14
veor q6,q6,q11
veor q8,q8,q12
eor r2,r2,r12,ROR #19
str r2,[sp,#120]
eor r1,r1,r7,ROR #19
ldr r7,[sp,#96]
add r2,r2,r6
str r6,[sp,#112]
add r6,r1,r3
ldr r12,[sp,#104]
vadd.i32 q11,q4,q6
vext.32 q4,q4,q4,#3
vadd.i32 q12,q7,q8
vshl.i32 q13,q11,#13
vext.32 q7,q7,q7,#3
vshl.i32 q14,q12,#13
eor r0,r0,r2,ROR #14
eor r2,r5,r6,ROR #14
str r3,[sp,#124]
add r3,r10,r12
ldr r5,[sp,#100]
add r6,r9,r11
vshr.u32 q11,q11,#19
vshr.u32 q12,q12,#19
veor q9,q9,q13
veor q5,q5,q14
eor r8,r8,r3,ROR #25
eor r3,r5,r6,ROR #25
add r5,r8,r10
add r6,r3,r9
veor q9,q9,q11
veor q5,q5,q12
eor r5,r7,r5,ROR #23
eor r6,r14,r6,ROR #23
add r7,r5,r8
add r14,r6,r3
vadd.i32 q11,q6,q9
vswp d12,d13
vadd.i32 q12,q8,q5
vshl.i32 q13,q11,#18
vswp d16,d17
vshl.i32 q14,q12,#18
eor r7,r12,r7,ROR #19
eor r11,r11,r14,ROR #19
add r12,r7,r5
add r14,r11,r6
vshr.u32 q11,q11,#14
vext.32 q9,q9,q9,#1
vshr.u32 q12,q12,#14
veor q2,q2,q13
vext.32 q5,q5,q5,#1
veor q10,q10,q14
eor r10,r10,r12,ROR #14
eor r9,r9,r14,ROR #14
add r12,r0,r3
add r14,r2,r4
veor q2,q2,q11
veor q11,q10,q12
eor r1,r1,r12,ROR #25
eor r7,r7,r14,ROR #25
add r12,r1,r0
add r14,r7,r2
vadd.i32 q10,q4,q2
vadd.i32 q12,q7,q11
vshl.i32 q13,q10,#7
vshl.i32 q14,q12,#7
eor r5,r5,r12,ROR #23
eor r6,r6,r14,ROR #23
vshr.u32 q10,q10,#25
vshr.u32 q12,q12,#25
add r12,r5,r1
add r14,r6,r7
veor q9,q9,q13
veor q5,q5,q14
veor q9,q9,q10
veor q5,q5,q12
vadd.i32 q10,q2,q9
vadd.i32 q12,q11,q5
vshl.i32 q13,q10,#9
vshl.i32 q14,q12,#9
eor r3,r3,r12,ROR #19
str r7,[sp,#104]
eor r4,r4,r14,ROR #19
ldr r7,[sp,#112]
add r12,r3,r5
str r6,[sp,#108]
add r6,r4,r6
ldr r14,[sp,#116]
eor r0,r0,r12,ROR #14
str r5,[sp,#96]
eor r5,r2,r6,ROR #14
ldr r2,[sp,#120]
vshr.u32 q10,q10,#23
vshr.u32 q12,q12,#23
veor q6,q6,q13
veor q8,q8,q14
veor q6,q6,q10
veor q8,q8,q12
add r6,r10,r14
add r12,r9,r8
vadd.i32 q12,q9,q6
vext.32 q10,q9,q9,#3
vadd.i32 q9,q5,q8
vshl.i32 q13,q12,#13
vext.32 q5,q5,q5,#3
vshl.i32 q14,q9,#13
vshr.u32 q12,q12,#19
vshr.u32 q9,q9,#19
eor r11,r11,r6,ROR #25
eor r2,r2,r12,ROR #25
add r6,r11,r10
str r3,[sp,#100]
add r3,r2,r9
ldr r12,[sp,#124]
veor q4,q4,q13
veor q7,q7,q14
veor q4,q4,q12
veor q9,q7,q9
eor r6,r7,r6,ROR #23
eor r3,r12,r3,ROR #23
add r7,r6,r11
add r12,r3,r2
vadd.i32 q7,q6,q4
vswp d12,d13
vadd.i32 q12,q8,q9
vshl.i32 q13,q7,#18
vswp d16,d17
vshl.i32 q14,q12,#18
eor r7,r14,r7,ROR #19
eor r8,r8,r12,ROR #19
add r12,r7,r6
add r14,r8,r3
vshr.u32 q15,q7,#14
vext.32 q7,q4,q4,#1
vshr.u32 q4,q12,#14
veor q2,q2,q13
vext.32 q9,q9,q9,#1
veor q11,q11,q14
eor r10,r10,r12,ROR #14
ldr r12,[sp,#244]
veor q2,q2,q15
eor r9,r9,r14,ROR #14
veor q11,q11,q4
subs r12,r12,#4
bhi ._mainloop2
strd r2,[sp,#112]
strd r6,[sp,#120]
strd r10,[sp,#128]
strd r8,[sp,#136]
ldrd r2,[sp,#96]
ldrd r6,[sp,#104]
ldrd r8,[sp,#64]
ldrd r10,[sp,#48]
add r0,r0,r8
add r1,r1,r9
add r2,r2,r10
add r3,r3,r11
ldrd r8,[sp,#72]
ldrd r10,[sp,#32]
add r4,r4,r8
add r5,r5,r9
add r6,r6,r10
add r7,r7,r11
ldr r12,[sp,#232]
cmp r12,#0
beq ._nomessage1
ldr r8,[r12,#0]
ldr r9,[r12,#4]
ldr r10,[r12,#8]
ldr r11,[r12,#12]
eor r0,r0,r8
eor r1,r1,r9
eor r2,r2,r10
eor r3,r3,r11
ldr r8,[r12,#16]
ldr r9,[r12,#20]
ldr r10,[r12,#24]
ldr r11,[r12,#28]
eor r4,r4,r8
eor r5,r5,r9
eor r6,r6,r10
eor r7,r7,r11
._nomessage1:
ldr r14,[sp,#228]
str r0,[r14,#0]
str r1,[r14,#4]
str r2,[r14,#8]
str r3,[r14,#12]
str r4,[r14,#16]
str r5,[r14,#20]
str r6,[r14,#24]
str r7,[r14,#28]
ldrd r6,[sp,#120]
ldrd r10,[sp,#128]
ldrd r0,[sp,#40]
ldrd r4,[sp,#80]
add r6,r6,r0
add r7,r7,r1
add r10,r10,r4
add r11,r11,r5
adds r0,r0,#3
adc r1,r1,#0
strd r0,[sp,#40]
ldrd r2,[sp,#112]
ldrd r8,[sp,#136]
ldrd r4,[sp,#56]
ldrd r0,[sp,#88]
add r2,r2,r4
add r3,r3,r5
add r0,r8,r0
add r1,r9,r1
cmp r12,#0
beq ._nomessage2
ldr r4,[r12,#32]
ldr r5,[r12,#36]
ldr r8,[r12,#40]
ldr r9,[r12,#44]
eor r6,r6,r4
eor r7,r7,r5
eor r10,r10,r8
eor r11,r11,r9
ldr r4,[r12,#48]
ldr r5,[r12,#52]
ldr r8,[r12,#56]
ldr r9,[r12,#60]
eor r2,r2,r4
eor r3,r3,r5
eor r0,r0,r8
eor r1,r1,r9
add r12,r12,#64
._nomessage2:
str r6,[r14,#32]
str r7,[r14,#36]
str r10,[r14,#40]
str r11,[r14,#44]
str r2,[r14,#48]
str r3,[r14,#52]
str r0,[r14,#56]
str r1,[r14,#60]
add r2,r14,#64
ldr r0,[sp,#236]
ldr r1,[sp,#240]
add r3,sp,#304
add r4,sp,#288
vadd.i32 q2,q2,q1
vld1.8 {d8-d9},[r3,: 128]
vadd.i32 q4,q4,q7
vld1.8 {d14-d15},[r4,: 128]
vadd.i32 q10,q10,q3
vadd.i32 q6,q7,q6
vmov.i64 q7,#0xffffffff
vext.32 d24,d4,d8,#1
vext.32 d27,d5,d9,#1
vbif q2,q10,q7
vext.32 d25,d13,d21,#1
vext.32 d26,d12,d20,#1
vbif q6,q4,q7
vrev64.i32 q4,q12
vrev64.i32 q10,q13
vswp d13,d5
cmp r12,#0
beq ._nomessage20
vld1.8 {d24-d25},[r12]!
vld1.8 {d26-d27},[r12]!
vld1.8 {d28-d29},[r12]!
vld1.8 {d30-d31},[r12]!
veor q2,q2,q12
veor q4,q4,q13
veor q6,q6,q14
veor q10,q10,q15
._nomessage20:
vst1.8 {d4-d5},[r2]!
vst1.8 {d8-d9},[r2]!
vst1.8 {d12-d13},[r2]!
vst1.8 {d20-d21},[r2]!
add r3,sp,#336
add r4,sp,#320
vadd.i32 q2,q11,q1
vld1.8 {d8-d9},[r3,: 128]
vadd.i32 q4,q4,q9
vld1.8 {d12-d13},[r4,: 128]
vadd.i32 q5,q5,q3
vadd.i32 q6,q6,q8
vext.32 d16,d4,d8,#1
vext.32 d19,d5,d9,#1
vbif q2,q5,q7
vext.32 d17,d13,d11,#1
vext.32 d18,d12,d10,#1
vbif q6,q4,q7
vrev64.i32 q4,q8
vrev64.i32 q5,q9
vswp d13,d5
subs r0,r0,#192
sbc r1,r1,#0
cmp r12,#0
beq ._nomessage21
vld1.8 {d14-d15},[r12]!
vld1.8 {d16-d17},[r12]!
vld1.8 {d18-d19},[r12]!
vld1.8 {d20-d21},[r12]!
veor q2,q2,q7
veor q4,q4,q8
veor q6,q6,q9
veor q5,q5,q10
._nomessage21:
vst1.8 {d4-d5},[r2]!
vst1.8 {d8-d9},[r2]!
vst1.8 {d12-d13},[r2]!
vst1.8 {d10-d11},[r2]!
str r12,[sp,#232]
str r2,[sp,#228]
cmp r0,#192
bhs ._mlenatleast192
._mlenlowbelow192:
cmp r1,#0
bhi ._mlenatleast192
cmp r0,#0
beq ._done
b ._mlenatleast1
._nextblock:
subs r0,r0,#64
sbc r1,r1,#0
._mlenatleast1:
cmp r1,#0
bne ._handleblock
cmp r0,#64
bhs ._handleblock
ldr r2,[sp,#228]
ldr r3,[sp,#232]
str r2,[sp,#236]
add r2,sp,#160
str r2,[sp,#228]
cmp r3,#0
beq ._handleblock
str r2,[sp,#232]
ldr r4,=0
._mcopy:
ldrb r5,[r3,#0]
add r3,r3,#1
add r4,r4,#1
strb r5,[r2,#0]
add r2,r2,#1
cmp r4,r0
blo ._mcopy
._handleblock:
str r0,[sp,#240]
str r1,[sp,#244]
ldrd r2,[sp,#48]
ldrd r6,[sp,#32]
ldrd r0,[sp,#64]
ldrd r4,[sp,#72]
ldrd r10,[sp,#80]
ldrd r8,[sp,#88]
strd r2,[sp,#96]
strd r6,[sp,#104]
ldrd r2,[sp,#56]
ldrd r6,[sp,#40]
ldr r12,=20
._mainloop1:
str r12,[sp,#248]
add r12,r0,r2
add r14,r5,r1
eor r4,r4,r12,ROR #25
eor r7,r7,r14,ROR #25
add r12,r4,r0
add r14,r7,r5
eor r6,r6,r12,ROR #23
eor r3,r3,r14,ROR #23
add r12,r6,r4
str r7,[sp,#124]
add r7,r3,r7
ldr r14,[sp,#104]
eor r2,r2,r12,ROR #19
str r6,[sp,#120]
eor r1,r1,r7,ROR #19
ldr r7,[sp,#100]
add r6,r2,r6
str r2,[sp,#112]
add r2,r1,r3
ldr r12,[sp,#96]
eor r0,r0,r6,ROR #14
str r3,[sp,#116]
eor r2,r5,r2,ROR #14
ldr r3,[sp,#108]
add r5,r10,r14
add r6,r9,r11
eor r8,r8,r5,ROR #25
eor r5,r7,r6,ROR #25
add r6,r8,r10
add r7,r5,r9
eor r6,r12,r6,ROR #23
eor r3,r3,r7,ROR #23
add r7,r6,r8
add r12,r3,r5
eor r7,r14,r7,ROR #19
eor r11,r11,r12,ROR #19
add r12,r7,r6
add r14,r11,r3
eor r10,r10,r12,ROR #14
eor r9,r9,r14,ROR #14
add r12,r0,r5
add r14,r2,r4
eor r1,r1,r12,ROR #25
eor r7,r7,r14,ROR #25
add r12,r1,r0
add r14,r7,r2
eor r6,r6,r12,ROR #23
eor r3,r3,r14,ROR #23
add r12,r6,r1
str r7,[sp,#104]
add r7,r3,r7
ldr r14,[sp,#120]
eor r5,r5,r12,ROR #19
str r3,[sp,#108]
eor r4,r4,r7,ROR #19
ldr r7,[sp,#124]
add r12,r5,r6
str r6,[sp,#96]
add r3,r4,r3
ldr r6,[sp,#112]
eor r0,r0,r12,ROR #14
str r5,[sp,#100]
eor r5,r2,r3,ROR #14
ldr r3,[sp,#116]
add r2,r10,r7
add r12,r9,r8
eor r11,r11,r2,ROR #25
eor r2,r6,r12,ROR #25
add r6,r11,r10
add r12,r2,r9
eor r6,r14,r6,ROR #23
eor r3,r3,r12,ROR #23
add r12,r6,r11
add r14,r3,r2
eor r7,r7,r12,ROR #19
eor r8,r8,r14,ROR #19
add r12,r7,r6
add r14,r8,r3
eor r10,r10,r12,ROR #14
eor r9,r9,r14,ROR #14
ldr r12,[sp,#248]
subs r12,r12,#2
bhi ._mainloop1
strd r6,[sp,#120]
strd r2,[sp,#112]
strd r10,[sp,#128]
strd r8,[sp,#136]
ldrd r2,[sp,#96]
ldrd r6,[sp,#104]
ldrd r8,[sp,#64]
ldrd r10,[sp,#48]
add r0,r0,r8
add r1,r1,r9
add r2,r2,r10
add r3,r3,r11
ldrd r8,[sp,#72]
ldrd r10,[sp,#32]
add r4,r4,r8
add r5,r5,r9
add r6,r6,r10
add r7,r7,r11
ldr r12,[sp,#232]
cmp r12,#0
beq ._nomessage10
ldr r8,[r12,#0]
ldr r9,[r12,#4]
ldr r10,[r12,#8]
ldr r11,[r12,#12]
eor r0,r0,r8
eor r1,r1,r9
eor r2,r2,r10
eor r3,r3,r11
ldr r8,[r12,#16]
ldr r9,[r12,#20]
ldr r10,[r12,#24]
ldr r11,[r12,#28]
eor r4,r4,r8
eor r5,r5,r9
eor r6,r6,r10
eor r7,r7,r11
._nomessage10:
ldr r14,[sp,#228]
str r0,[r14,#0]
str r1,[r14,#4]
str r2,[r14,#8]
str r3,[r14,#12]
str r4,[r14,#16]
str r5,[r14,#20]
str r6,[r14,#24]
str r7,[r14,#28]
ldrd r6,[sp,#120]
ldrd r10,[sp,#128]
ldrd r0,[sp,#40]
ldrd r4,[sp,#80]
add r6,r6,r0
add r7,r7,r1
add r10,r10,r4
add r11,r11,r5
adds r0,r0,#1
adc r1,r1,#0
strd r0,[sp,#40]
ldrd r2,[sp,#112]
ldrd r8,[sp,#136]
ldrd r4,[sp,#56]
ldrd r0,[sp,#88]
add r2,r2,r4
add r3,r3,r5
add r0,r8,r0
add r1,r9,r1
cmp r12,#0
beq ._nomessage11
ldr r4,[r12,#32]
ldr r5,[r12,#36]
ldr r8,[r12,#40]
ldr r9,[r12,#44]
eor r6,r6,r4
eor r7,r7,r5
eor r10,r10,r8
eor r11,r11,r9
ldr r4,[r12,#48]
ldr r5,[r12,#52]
ldr r8,[r12,#56]
ldr r9,[r12,#60]
eor r2,r2,r4
eor r3,r3,r5
eor r0,r0,r8
eor r1,r1,r9
add r4,r12,#64
str r4,[sp,#232]
._nomessage11:
str r6,[r14,#32]
str r7,[r14,#36]
str r10,[r14,#40]
str r11,[r14,#44]
str r2,[r14,#48]
str r3,[r14,#52]
str r0,[r14,#56]
str r1,[r14,#60]
add r0,r14,#64
str r0,[sp,#228]
ldr r0,[sp,#240]
ldr r1,[sp,#244]
cmp r0,#64
bhi ._nextblock
cmp r1,#0
bne ._nextblock
cmp r0,#64
beq ._done
add r1,sp,#160
ldr r2,[sp,#236]
ldr r3,=0
._ccopy:
ldrb r4,[r1,#0]
add r1,r1,#1
add r3,r3,#1
strb r4,[r2,#0]
add r2,r2,#1
cmp r3,r0
blo ._ccopy
._done:
ldrd r4,[sp,#0]
ldrd r6,[sp,#8]
ldrd r8,[sp,#16]
ldrd r10,[sp,#24]
ldr r12,[sp,#252]
ldr r14,[sp,#224]
ldr r0,=0
mov sp,r12
vpop {q4,q5,q6,q7}
bx lr
