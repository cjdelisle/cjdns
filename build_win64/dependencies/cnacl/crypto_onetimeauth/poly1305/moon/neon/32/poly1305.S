#include "arm.inc"

SECTION_TEXT

.arm
.fpu neon

GLOBAL_HIDDEN_FN poly1305_block_size_neon
mov r0, #32
bx lr
FN_END poly1305_block_size_neon

.p2align 2
poly1305_init_constants_neon:
.long 0x3ffff03
.long 0x3ffc0ff
.long 0x3f03fff
.long 0x00fffff

GLOBAL_HIDDEN_FN poly1305_init_ext_neon
poly1305_init_ext_neon_local:
stmfd sp!, {r4-r11, lr}
sub sp, sp, #32
mov r14, r2
and r2, r2, r2
moveq r14, #-1
ldmia r1!, {r2-r5}
ldr r7, =poly1305_init_constants_neon
mov r6, r2
mov r8, r2, lsr #26
mov r9, r3, lsr #20
mov r10, r4, lsr #14
mov r11, r5, lsr #8
orr r8, r8, r3, lsl #6
orr r9, r9, r4, lsl #12
orr r10, r10, r5, lsl #18
ldmia r7, {r2-r5}
and r2, r2, r8
and r3, r3, r9
and r4, r4, r10
and r5, r5, r11
and r6, r6, 0x3ffffff
stmia r0!, {r2-r6}
eor r8, r8, r8
str r8, [sp, #24]
poly1305_init_ext_neon_squareloop:
ldr r8, [sp, #24]
mov r12, #16
cmp r8, #2
beq poly1305_init_ext_neon_donesquaring
cmp r8, #1
moveq r12, #64
cmp r14, r12
bls poly1305_init_ext_neon_donesquaring
add r8, #1
str r8, [sp, #24]
mov r6, r6, lsl #1
mov r2, r2, lsl #1
umull r7, r8, r3, r3
umull r9, r10, r6, r4
umlal r7, r8, r6, r5
umlal r9, r10, r2, r3
add r11, r5, r5, lsl #2
umlal r7, r8, r2, r4
umlal r9, r10, r5, r11
str r7, [sp, #16]
str r8, [sp, #20]
mov r2, r2, lsr #1
mov r5, r5, lsl #1
str r9, [sp, #8]
str r10, [sp, #12]
umull r7, r8, r2, r2
umull r9, r10, r6, r2
add r11, r3, r3, lsl #2
add r12, r4, r4, lsl #2
umlal r7, r8, r6, r3
umlal r9, r10, r5, r11
umlal r7, r8, r5, r12
umlal r9, r10, r4, r12
mov r6, r6, lsr #1
mov r3, r3, lsl #1
add r11, r2, r2, lsl #2
str r7, [sp, #0]
str r8, [sp, #4]
umull r7, r8, r6, r6
umlal r7, r8, r3, r12
umlal r7, r8, r5, r11
and r6, r7, 0x3ffffff
mov r11, r7, lsr #26
orr r11, r11, r8, lsl #6
ldr r7, [sp, #0]
ldr r8, [sp, #4]
adds r9, r9, r11
adc r10, r10, #0
and r2, r9, 0x3ffffff
mov r11, r9, lsr #26
orr r11, r11, r10, lsl #6
ldr r9, [sp, #8]
ldr r10, [sp, #12]
adds r7, r7, r11
adc r8, r8, #0
and r3, r7, 0x3ffffff
mov r11, r7, lsr #26
orr r11, r11, r8, lsl #6
ldr r7, [sp, #16]
ldr r8, [sp, #20]
adds r9, r9, r11
adc r10, r10, #0
and r4, r9, 0x3ffffff
mov r11, r9, lsr #26
orr r11, r11, r10, lsl #6
adds r7, r7, r11
adc r8, r8, #0
and r5, r7, 0x3ffffff
mov r11, r7, lsr #26
orr r11, r11, r8, lsl #6
add r11, r11, r11, lsl #2
add r6, r6, r11
mov r11, r6, lsr #26
and r6, r6, 0x3ffffff
add r2, r2, r11
stmia r0!, {r2-r6}
b poly1305_init_ext_neon_squareloop
poly1305_init_ext_neon_donesquaring:
mov r2, #2
ldr r14, [sp, #24]
sub r14, r2, r14
mov r3, r14, lsl #4
add r3, r3, r14, lsl #2
add r0, r0, r3
eor r2, r2, r2
eor r3, r3, r3
eor r4, r4, r4
eor r5, r5, r5
eor r6, r6, r6
stmia r0!, {r2-r6}
stmia r0!, {r2-r6}
ldmia r1!, {r2-r5}
stmia r0, {r2-r6}
add sp, sp, #32
ldmfd sp!, {r4-r11, lr}
bx lr
FN_END poly1305_init_ext_neon

.ltorg

GLOBAL_HIDDEN_FN poly1305_blocks_neon
poly1305_blocks_neon_local:
vmov.i32 q0, #0xffffffff
vmov.i32 d4, #1
vsubw.u32 q0, q0, d4
vstmdb sp!, {q4,q5,q6,q7}
stmfd sp!, {r4-r11, lr}
mov r8, sp
and sp, sp, #~63
sub sp, sp, #192
str r0, [sp, #108]
str r1, [sp, #112]
str r2, [sp, #116]
str r8, [sp, #120]
mov r3, r0
mov r0, r1
mov r1, r2
mov r2, r3
ldr r8, [r2, #116]
veor d15, d15, d15
vorr.i32 d15, #(1 << 24)
tst r8, #2
beq poly1305_blocks_neon_skip_shift8
vshr.u64 d15, #32
poly1305_blocks_neon_skip_shift8:
tst r8, #4
beq poly1305_blocks_neon_skip_shift16
veor d15, d15, d15
poly1305_blocks_neon_skip_shift16:
vst1.64 d15, [sp, :64]
tst r8, #1
bne poly1305_blocks_neon_started
vld1.64 {q0-q1}, [r0]!
vswp d1, d2
vmovn.i64 d21, q0
vshrn.i64 d22, q0, #26
vshrn.u64 d24, q1, #14
vext.8 d0, d0, d2, #4
vext.8 d1, d1, d3, #4
vshr.u64 q1, q1, #32
vshrn.i64 d23, q0, #20
vshrn.u64 d25, q1, #8
vand.i32 d21, #0x03ffffff
vand.i32 q11, #0x03ffffff
vand.i32 q12, #0x03ffffff
orr r8, r8, #1
sub r1, r1, #32
str r8, [r2, #116]
vorr d25, d25, d15
b poly1305_blocks_neon_setupr20
poly1305_blocks_neon_started:
add r9, r2, #60
vldm r9, {d21-d25}
poly1305_blocks_neon_setupr20:
vmov.i32 d0, #5
tst r8, #(8|16)
beq poly1305_blocks_neon_setupr20_simple
tst r8, #(8)
beq poly1305_blocks_neon_setupr20_r_1
mov r9, r2
add r10, r2, #20
vld1.64 {q9}, [r9]!
vld1.64 {q8}, [r10]!
vld1.64 {d2}, [r9]
vld1.64 {d20}, [r10]
b poly1305_blocks_neon_setupr20_hard
poly1305_blocks_neon_setupr20_r_1:
mov r9, r2
vmov.i32 d2, #1
vld1.64 {q8}, [r9]!
veor q9, q9, q9
vshr.u64 d2, d2, #32
vld1.64 {d20}, [r9]
poly1305_blocks_neon_setupr20_hard:
vzip.i32 q8, q9
vzip.i32 d20, d2
b poly1305_blocks_neon_setups20
poly1305_blocks_neon_setupr20_simple:
add r9, r2, #20
vld1.64 {d2-d4}, [r9]
vdup.32 d16, d2[0]
vdup.32 d17, d2[1]
vdup.32 d18, d3[0]
vdup.32 d19, d3[1]
vdup.32 d20, d4[0]
poly1305_blocks_neon_setups20:
vmul.i32 q13, q8, d0[0]
vmov.i64 q15, 0x00000000ffffffff
vmul.i32 q14, q9, d0[0]
vshr.u64 q15, q15, #6
cmp r1, #64
blo poly1305_blocks_neon_try32
add r9, sp, #16
add r10, r2, #40
add r11, sp, #64
str r1, [sp, #116]
vld1.64 {d10-d12}, [r10]
vmov d14, d12
vmul.i32 q6, q5, d0[0]
poly1305_blocks_neon_mainloop:
ldmia r0!, {r2-r5}
vmull.u32 q0, d25, d12[0]
mov r7, r2, lsr #26
vmlal.u32 q0, d24, d12[1]
mov r8, r3, lsr #20
ldr r6, [sp, #0]
vmlal.u32 q0, d23, d13[0]
mov r9, r4, lsr #14
vmlal.u32 q0, d22, d13[1]
orr r6, r6, r5, lsr #8
vmlal.u32 q0, d21, d14[0]
orr r3, r7, r3, lsl #6
vmull.u32 q1, d25, d12[1]
orr r4, r8, r4, lsl #12
orr r5, r9, r5, lsl #18
vmlal.u32 q1, d24, d13[0]
ldmia r0!, {r7-r10}
vmlal.u32 q1, d23, d13[1]
mov r1, r7, lsr #26
vmlal.u32 q1, d22, d14[0]
ldr r11, [sp, #4]
mov r12, r8, lsr #20
vmlal.u32 q1, d21, d10[0]
mov r14, r9, lsr #14
vmull.u32 q2, d25, d13[0]
orr r11, r11, r10, lsr #8
orr r8, r1, r8, lsl #6
vmlal.u32 q2, d24, d13[1]
orr r9, r12, r9, lsl #12
vmlal.u32 q2, d23, d14[0]
orr r10, r14, r10, lsl #18
vmlal.u32 q2, d22, d10[0]
mov r12, r3
and r2, r2, #0x3ffffff
vmlal.u32 q2, d21, d10[1]
mov r14, r5
vmull.u32 q3, d25, d13[1]
and r3, r7, #0x3ffffff
vmlal.u32 q3, d24, d14[0]
and r5, r8, #0x3ffffff
vmlal.u32 q3, d23, d10[0]
and r7, r9, #0x3ffffff
vmlal.u32 q3, d22, d10[1]
and r8, r14, #0x3ffffff
vmlal.u32 q3, d21, d11[0]
and r9, r10, #0x3ffffff
add r14, sp, #128
vmull.u32 q4, d25, d14[0]
mov r10, r6
vmlal.u32 q4, d24, d10[0]
and r6, r4, #0x3ffffff
vmlal.u32 q4, d23, d10[1]
and r4, r12, #0x3ffffff
vmlal.u32 q4, d22, d11[0]
stm r14, {r2-r11}
vmlal.u32 q4, d21, d11[1]
vld1.64 {d21-d24}, [r14, :256]!
vld1.64 {d25}, [r14, :64]
ldmia r0!, {r2-r5}
vmlal.u32 q0, d25, d26
mov r7, r2, lsr #26
vmlal.u32 q0, d24, d27
ldr r6, [sp, #0]
mov r8, r3, lsr #20
vmlal.u32 q0, d23, d28
mov r9, r4, lsr #14
vmlal.u32 q0, d22, d29
orr r6, r6, r5, lsr #8
vmlal.u32 q0, d21, d20
orr r3, r7, r3, lsl #6
vmlal.u32 q1, d25, d27
orr r4, r8, r4, lsl #12
orr r5, r9, r5, lsl #18
vmlal.u32 q1, d24, d28
ldmia r0!, {r7-r10}
vmlal.u32 q1, d23, d29
mov r1, r7, lsr #26
vmlal.u32 q1, d22, d20
ldr r11, [sp, #4]
mov r12, r8, lsr #20
vmlal.u32 q1, d21, d16
mov r14, r9, lsr #14
vmlal.u32 q2, d25, d28
orr r11, r11, r10, lsr #8
orr r8, r1, r8, lsl #6
orr r9, r12, r9, lsl #12
vmlal.u32 q2, d24, d29
orr r10, r14, r10, lsl #18
and r2, r2, #0x3ffffff
mov r12, r3
vmlal.u32 q2, d23, d20
mov r14, r5
vmlal.u32 q2, d22, d16
and r3, r7, #0x3ffffff
vmlal.u32 q2, d21, d17
and r5, r8, #0x3ffffff
vmlal.u32 q3, d25, d29
and r7, r9, #0x3ffffff
vmlal.u32 q3, d24, d20
and r8, r14, #0x3ffffff
vmlal.u32 q3, d23, d16
and r9, r10, #0x3ffffff
vmlal.u32 q3, d22, d17
add r14, sp, #128
vmlal.u32 q3, d21, d18
mov r10, r6
vmlal.u32 q4, d25, d20
vmlal.u32 q4, d24, d16
and r6, r4, #0x3ffffff
vmlal.u32 q4, d23, d17
and r4, r12, #0x3ffffff
vmlal.u32 q4, d22, d18
stm r14, {r2-r11}
vmlal.u32 q4, d21, d19
vld1.64 {d21-d24}, [r14, :256]!
vld1.64 {d25}, [r14, :64]
vaddw.u32 q0, q0, d21
vaddw.u32 q1, q1, d22
vaddw.u32 q2, q2, d23
vaddw.u32 q3, q3, d24
vaddw.u32 q4, q4, d25
vshr.u64 q11, q0, #26
vand q0, q0, q15
vadd.i64 q1, q1, q11
vshr.u64 q12, q3, #26
vand q3, q3, q15
vadd.i64 q4, q4, q12
vshr.u64 q11, q1, #26
vand q1, q1, q15
vadd.i64 q2, q2, q11
vshr.u64 q12, q4, #26
vand q4, q4, q15
vadd.i64 q0, q0, q12
vshl.i64 q12, q12, #2
ldr r1, [sp, #116]
vadd.i64 q0, q0, q12
vshr.u64 q11, q2, #26
vand q2, q2, q15
vadd.i64 q3, q3, q11
sub r1, #64
vshr.u64 q12, q0, #26
vand q0, q0, q15
vadd.i64 q1, q1, q12
cmp r1, #64
vshr.u64 q11, q3, #26
vand q3, q3, q15
vadd.i64 q4, q4, q11
vmovn.i64 d21, q0
str r1, [sp, #116]
vmovn.i64 d22, q1
vmovn.i64 d23, q2
vmovn.i64 d24, q3
vmovn.i64 d25, q4
bhs poly1305_blocks_neon_mainloop
poly1305_blocks_neon_try32:
cmp r1, #32
blo poly1305_blocks_neon_done
tst r0, r0
bne poly1305_blocks_loadm32
veor q0, q0, q0
veor q1, q1, q1
veor q2, q2, q2
veor q3, q3, q3
veor q4, q4, q4
b poly1305_blocks_continue32
poly1305_blocks_loadm32:
vld1.64 {q0-q1}, [r0]!
veor q4, q4, q4
vswp d1, d2
veor q3, q3, q3
vtrn.32 q0, q4
vtrn.32 q1, q3
vshl.i64 q2, q1, #12
vshl.i64 q3, q3, #18
vshl.i64 q1, q4, #6
vmovl.u32 q4, d15
poly1305_blocks_continue32:
vmlal.u32 q0, d25, d26
vmlal.u32 q0, d24, d27
vmlal.u32 q0, d23, d28
vmlal.u32 q0, d22, d29
vmlal.u32 q0, d21, d20
vmlal.u32 q1, d25, d27
vmlal.u32 q1, d24, d28
vmlal.u32 q1, d23, d29
vmlal.u32 q1, d22, d20
vmlal.u32 q1, d21, d16
vmlal.u32 q2, d25, d28
vmlal.u32 q2, d24, d29
vmlal.u32 q2, d23, d20
vmlal.u32 q2, d22, d16
vmlal.u32 q2, d21, d17
vmlal.u32 q3, d25, d29
vmlal.u32 q3, d24, d20
vmlal.u32 q3, d23, d16
vmlal.u32 q3, d22, d17
vmlal.u32 q3, d21, d18
vmlal.u32 q4, d25, d20
vmlal.u32 q4, d24, d16
vmlal.u32 q4, d23, d17
vmlal.u32 q4, d22, d18
vmlal.u32 q4, d21, d19
vshr.u64 q11, q0, #26
vand q0, q0, q15
vadd.i64 q1, q1, q11
vshr.u64 q12, q3, #26
vand q3, q3, q15
vadd.i64 q4, q4, q12
vshr.u64 q11, q1, #26
vand q1, q1, q15
vadd.i64 q2, q2, q11
vshr.u64 q12, q4, #26
vand q4, q4, q15
vadd.i64 q0, q0, q12
vshl.i64 q12, q12, #2
vadd.i64 q0, q0, q12
vshr.u64 q11, q2, #26
vand q2, q2, q15
vadd.i64 q3, q3, q11
vshr.u64 q12, q0, #26
vand q0, q0, q15
vadd.i64 q1, q1, q12
vshr.u64 q11, q3, #26
vand q3, q3, q15
vadd.i64 q4, q4, q11
vmovn.i64 d21, q0
vmovn.i64 d22, q1
vmovn.i64 d23, q2
vmovn.i64 d24, q3
vmovn.i64 d25, q4
poly1305_blocks_neon_done:
tst r0, r0
beq poly1305_blocks_neon_final
ldr r2, [sp, #108]
add r2, r2, #60
vst1.64 {d21}, [r2]!
vst1.64 {d22-d25}, [r2]
b poly1305_blocks_neon_leave
poly1305_blocks_neon_final:
vadd.u32 d10, d0, d1
vadd.u32 d13, d2, d3
vadd.u32 d11, d4, d5
ldr r5, [sp, #108]
vadd.u32 d14, d6, d7
vadd.u32 d12, d8, d9
vtrn.32 d10, d13
vtrn.32 d11, d14
vst1.64 {d10-d12}, [sp]
ldm sp, {r0-r4}
mov r12, r0, lsr #26
and r0, r0, #0x3ffffff
add r1, r1, r12
mov r12, r1, lsr #26
and r1, r1, #0x3ffffff
add r2, r2, r12
mov r12, r2, lsr #26
and r2, r2, #0x3ffffff
add r3, r3, r12
mov r12, r3, lsr #26
and r3, r3, #0x3ffffff
add r4, r4, r12
mov r12, r4, lsr #26
and r4, r4, #0x3ffffff
add r12, r12, r12, lsl #2
add r0, r0, r12
mov r12, r0, lsr #26
and r0, r0, #0x3ffffff
add r1, r1, r12
mov r12, r1, lsr #26
and r1, r1, #0x3ffffff
add r2, r2, r12
mov r12, r2, lsr #26
and r2, r2, #0x3ffffff
add r3, r3, r12
mov r12, r3, lsr #26
and r3, r3, #0x3ffffff
add r4, r4, r12
mov r12, r4, lsr #26
and r4, r4, #0x3ffffff
add r12, r12, r12, lsl #2
add r0, r0, r12
mov r12, r0, lsr #26
and r0, r0, #0x3ffffff
add r1, r1, r12
add r6, r0, #5
mov r12, r6, lsr #26
and r6, r6, #0x3ffffff
add r7, r1, r12
mov r12, r7, lsr #26
and r7, r7, #0x3ffffff
add r10, r2, r12
mov r12, r10, lsr #26
and r10, r10, #0x3ffffff
add r11, r3, r12
mov r12, #-(1 << 26)
add r12, r12, r11, lsr #26
and r11, r11, #0x3ffffff
add r14, r4, r12
mov r12, r14, lsr #31
sub r12, #1
and r6, r6, r12
and r7, r7, r12
and r10, r10, r12
and r11, r11, r12
and r14, r14, r12
mvn r12, r12
and r0, r0, r12
and r1, r1, r12
and r2, r2, r12
and r3, r3, r12
and r4, r4, r12
orr r0, r0, r6
orr r1, r1, r7
orr r2, r2, r10
orr r3, r3, r11
orr r4, r4, r14
orr r0, r0, r1, lsl #26
lsr r1, r1, #6
orr r1, r1, r2, lsl #20
lsr r2, r2, #12
orr r2, r2, r3, lsl #14
lsr r3, r3, #18
orr r3, r3, r4, lsl #8
add r5, r5, #60
stm r5, {r0-r3}
poly1305_blocks_neon_leave:
ldr sp, [sp, #120]
ldmfd sp!, {r4-r11, lr}
vldm sp!, {q4-q7}
bx lr
FN_END poly1305_init_ext_neon

GLOBAL_HIDDEN_FN poly1305_finish_ext_neon
poly1305_finish_ext_neon_local:
stmfd sp!, {r4-r11, lr}
sub sp, sp, #32
mov r5, r0
mov r6, r1
mov r7, r2
mov r8, r3
ands r7, r7, r7
beq poly1305_finish_ext_neon_noremaining
mov r9, sp
veor q0, q0, q0
veor q1, q1, q1
vst1.64 {q0-q1}, [sp]
tst r7, #16
beq poly1305_finish_ext_neon_skip16
vld1.u64 {q0}, [r1]!
vst1.64 {q0}, [r9]!
poly1305_finish_ext_neon_skip16:
tst r7, #8
beq poly1305_finish_ext_neon_skip8
ldmia r1!, {r10-r11}
stmia r9!, {r10-r11}
poly1305_finish_ext_neon_skip8:
tst r7, #4
beq poly1305_finish_ext_neon_skip4
ldr r10, [r1], #4
str r10, [r9], #4
poly1305_finish_ext_neon_skip4:
tst r7, #2
beq poly1305_finish_ext_neon_skip2
ldrh r10, [r1], #2
strh r10, [r9], #2
poly1305_finish_ext_neon_skip2:
tst r7, #1
beq poly1305_finish_ext_neon_skip1
ldrb r10, [r1], #1
strb r10, [r9], #1
poly1305_finish_ext_neon_skip1:
cmp r7, #16
beq poly1305_finish_ext_neon_skipfinalbit
mov r10, #1
strb r10, [r9]
poly1305_finish_ext_neon_skipfinalbit:
ldr r10, [r5, #116]
orrhs r10, #2
orrlo r10, #4
str r10, [r5, #116]
mov r0, r5
mov r1, sp
mov r2, #32
bl poly1305_blocks_neon_local
poly1305_finish_ext_neon_noremaining:
ldr r10, [r5, #116]
tst r10, #1
beq poly1305_finish_ext_neon_notstarted
cmp r7, #0
beq poly1305_finish_ext_neon_user2r
cmp r7, #16
bls poly1305_finish_ext_neon_user1
poly1305_finish_ext_neon_user2r:
orr r10, r10, #8
b poly1305_finish_ext_neon_finalblock
poly1305_finish_ext_neon_user1:
orr r10, r10, #16
poly1305_finish_ext_neon_finalblock:
str r10, [r5, #116]
mov r0, r5
eor r1, r1, r1
mov r2, #32
bl poly1305_blocks_neon_local
poly1305_finish_ext_neon_notstarted:
add r0, r5, #60
add r9, r5, #100
ldm r0, {r0-r3}
ldm r9, {r9-r12}
adds r0, r0, r9
adcs r1, r1, r10
adcs r2, r2, r11
adcs r3, r3, r12
stm r8, {r0-r3}
veor q0, q0, q0
veor q1, q1, q1
veor q2, q2, q2
veor q3, q3, q3
vstmia r5!, {q0-q3}
vstm r5, {q0-q3}
add sp, sp, #32
ldmfd sp!, {r4-r11, lr}
bx lr
FN_END poly1305_finish_ext_neon

GLOBAL_HIDDEN_FN poly1305_auth_neon
stmfd sp!, {r4-r8, lr}
mov r8, sp
and sp, sp, #(~63)
sub sp, sp, #128
mov r4, r0
mov r5, r1
mov r6, r2
mov r7, r3
mov r0, sp
mov r1, r7
bl poly1305_init_ext_neon_local
ands r2, r6, #(~31)
beq poly1305_auth_neon_noblocks
mov r0, sp
mov r1, r5
add r5, r5, r2
sub r6, r6, r2
bl poly1305_blocks_neon_local
poly1305_auth_neon_noblocks:
mov r0, sp
mov r1, r5
mov r2, r6
mov r3, r4
bl poly1305_finish_ext_neon_local
mov sp, r8
ldmfd sp!, {r4-r8, lr}
bx lr
FN_END poly1305_auth_neon
