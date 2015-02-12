#include "arm.inc"

SECTION_TEXT

.arch armv5

GLOBAL_HIDDEN_FN poly1305_block_size_armv6
mov r0, #16
bx lr
FN_END poly1305_block_size_armv6

.p2align 2
poly1305_init_constants_armv6:
.long 0x3ffffff
.long 0x3ffff03
.long 0x3ffc0ff
.long 0x3f03fff
.long 0x00fffff

GLOBAL_HIDDEN_FN poly1305_init_ext_armv6
poly1305_init_ext_armv6_local:
stmfd sp!, {r4-r11, lr}
ldmia r1!, {r2-r5}
ldr r7, =poly1305_init_constants_armv6
mov r8, r2
mov r9, r2, lsr #26
mov r10, r3, lsr #20
mov r11, r4, lsr #14
mov r12, r5, lsr #8
orr r9, r9, r3, lsl #6
orr r10, r10, r4, lsl #12
orr r11, r11, r5, lsl #18
ldmia r7, {r2-r6}
and r2, r2, r8
and r3, r3, r9
and r4, r4, r10
and r5, r5, r11
and r6, r6, r12
stmia r0!, {r2-r6}
eor r2, r2, r2
eor r3, r3, r3
eor r4, r4, r4
eor r5, r5, r5
eor r6, r6, r6
stmia r0!, {r2-r6}
ldmia r1!, {r2-r5}
stmia r0, {r2-r6}
ldmfd sp!, {r4-r11, lr}
bx lr
FN_END poly1305_init_ext_armv6

.ltorg

GLOBAL_HIDDEN_FN poly1305_blocks_armv6
poly1305_blocks_armv6_local:
stmfd sp!, {r4-r11, lr}
sub sp, sp, #128
str r0, [sp, #36]
str r1, [sp, #40]
str r2, [sp, #44]
mov r14, r1
mov r12, r2
ldr r8, [r0, #56]
tst r8, r8
eor r6, r6, r6
moveq r6, #(1 << 24)
str r6, [sp, #32]
add r10, sp, #64
ldmia r0, {r0-r9}
stmia r10, {r0-r4}
cmp r12, #16
blo poly1305_blocks_armv6_done
poly1305_blocks_armv6_mainloop:
ldmia r14!, {r0-r3}
mov r10, r0, lsr #26
mov r11, r1, lsr #20
mov r12, r2, lsr #14
str r14, [sp, #40]
mov r4, r3, lsr #8
orr r10, r10, r1, lsl #6
orr r11, r11, r2, lsl #12
orr r12, r12, r3, lsl #18
and r0, r0, #0x3ffffff
and r10, r10, #0x3ffffff
ldr r3, [sp, #32]
and r11, r11, #0x3ffffff
and r12, r12, #0x3ffffff
add r5, r5, r0
add r6, r6, r10
orr r4, r4, r3
add r7, r7, r11
add r14, sp, #64
add r8, r8, r12
add r9, r9, r4
ldmia r14, {r0-r4}
umull r10,r11,r5,r4
umull r12,r14,r5,r3
umlal r10,r11,r6,r3
umlal r12,r14,r6,r2
umlal r10,r11,r7,r2
umlal r12,r14,r7,r1
add r4,r4,r4,lsl #2
add r3,r3,r3,lsl #2
umlal r10,r11,r8,r1
umlal r12,r14,r8,r0
umlal r10,r11,r9,r0
umlal r12,r14,r9,r4
str r10, [sp, #24]
str r11, [sp, #28]
str r12, [sp, #16]
str r14, [sp, #20]
umull r10,r11,r5,r2
umull r12,r14,r5,r1
umlal r10,r11,r6,r1
umlal r12,r14,r6,r0
umlal r10,r11,r7,r0
umlal r12,r14,r7,r4
add r2,r2,r2,lsl #2
add r1,r1,r1,lsl #2
umlal r10,r11,r8,r4
umlal r12,r14,r8,r3
umlal r10,r11,r9,r3
umlal r12,r14,r9,r2
str r10, [sp, #8]
str r11, [sp, #12]
str r12, [sp, #0]
str r14, [sp, #4]
umull r10,r11,r5,r0
umlal r10,r11,r6,r4
umlal r10,r11,r7,r3
umlal r10,r11,r8,r2
umlal r10,r11,r9,r1
ldmia sp, {r0-r7}
lsr r12, r10, #26
lsr r14, r4, #26
orr r12, r12, r11, lsl #6
orr r14, r14, r5, lsl #6
and r10, r10, #0x3ffffff
and r4, r4, #0x3ffffff
adds r0, r0, r12
adc r1, r1, #0
adds r6, r6, r14
adc r7, r7, #0
lsr r12, r0, #26
lsr r14, r6, #26
orr r12, r12, r1, lsl #6
orr r14, r14, r7, lsl #6
and r0, r0, #0x3ffffff
and r6, r6, #0x3ffffff
add r14, r14, r14, lsl #2
adds r2, r2, r12
adc r3, r3, #0
add r10, r10, r14
lsr r12, r2, #26
lsr r14, r10, #26
orr r12, r12, r3, lsl #6
and r5, r10, #0x3ffffff
and r7, r2, #0x3ffffff
add r4, r4, r12
add r0, r0, r14
lsr r12, r4, #26
and r8, r4, #0x3ffffff
add r9, r6, r12
ldr r12, [sp, #44]
ldr r14, [sp, #40]
mov r6, r0
cmp r12, #32
sub r12, r12, #16
str r12, [sp, #44]
bhs poly1305_blocks_armv6_mainloop
poly1305_blocks_armv6_done:
ldr r12, [sp, #36]
str r5, [r12, #20]
str r6, [r12, #24]
str r7, [r12, #28]
str r8, [r12, #32]
str r9, [r12, #36]
add sp, sp, #128
ldmfd sp!, {r4-r11, lr}
bx lr
FN_END poly1305_blocks_armv6

GLOBAL_HIDDEN_FN poly1305_finish_ext_armv6
poly1305_finish_ext_armv6_local:
stmfd sp!, {r4-r11, lr}
sub sp, sp, #16
mov r5, r0
mov r6, r1
mov r7, r2
mov r8, r3
ands r2, r2, r2
beq poly1305_finish_ext_armv6_noremaining
eor r0, r0
mov r9, sp
str r0, [sp, #0]
str r0, [sp, #4]
str r0, [sp, #8]
str r0, [sp, #12]
tst r2, #8
beq poly1305_finish_ext_armv6_skip8
ldmia r1!, {r10-r11}
stmia r9!, {r10-r11}
poly1305_finish_ext_armv6_skip8:
tst r2, #4
beq poly1305_finish_ext_armv6_skip4
ldr r10, [r1], #4
str r10, [r9], #4
poly1305_finish_ext_armv6_skip4:
tst r2, #2
beq poly1305_finish_ext_armv6_skip2
ldrh r10, [r1], #2
strh r10, [r9], #2
poly1305_finish_ext_armv6_skip2:
tst r2, #1
beq poly1305_finish_ext_armv6_skip1
ldrb r10, [r1], #1
strb r10, [r9], #1
poly1305_finish_ext_armv6_skip1:
mov r11, #1
strb r11, [r9]
str r11, [r5, #56]
mov r0, r5
mov r1, sp
mov r2, #16
bl poly1305_blocks_armv6_local
poly1305_finish_ext_armv6_noremaining:
ldr r0, [r5, #20]
ldr r1, [r5, #24]
ldr r2, [r5, #28]
ldr r3, [r5, #32]
ldr r4, [r5, #36]
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
ldr r6, [r5, #40]
ldr r7, [r5, #44]
ldr r10, [r5, #48]
ldr r11, [r5, #52]
adds r0, r0, r6
adcs r1, r1, r7
adcs r2, r2, r10
adcs r3, r3, r11
stmia r8, {r0-r3}
mov r12, r5
eor r0, r0, r0
eor r1, r1, r1
eor r2, r2, r2
eor r3, r3, r3
eor r4, r4, r4
eor r5, r5, r5
eor r6, r6, r6
eor r7, r7, r7
stmia r12!, {r0-r7}
stmia r12, {r0-r7}
add sp, sp, #16
ldmfd sp!, {r4-r11, lr}
bx lr
FN_END poly1305_finish_ext_armv6

GLOBAL_HIDDEN_FN poly1305_auth_armv6
poly1305_auth_armv6_local:
stmfd sp!, {r4-r8, lr}
mov r8, sp
and sp, sp, #(~63)
sub sp, sp, #64
mov r4, r0
mov r5, r1
mov r6, r2
mov r7, r3
mov r0, sp
mov r1, r7
bl poly1305_init_ext_armv6_local
ands r2, r6, #(~15)
beq poly1305_auth_armv6_noblocks
mov r0, sp
mov r1, r5
add r5, r5, r2
sub r6, r6, r2
bl poly1305_blocks_armv6_local
poly1305_auth_armv6_noblocks:
mov r0, sp
mov r1, r5
mov r2, r6
mov r3, r4
bl poly1305_finish_ext_armv6_local
mov sp, r8
ldmfd sp!, {r4-r8, lr}
bx lr
FN_END poly1305_auth_armv6

