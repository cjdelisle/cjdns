#include "x86.inc"

SECTION_TEXT

GLOBAL_HIDDEN_FN_EXT poly1305_block_size_avx2, 0, 0
movl $64, %eax
ret
FN_END poly1305_block_size_avx2

GLOBAL_HIDDEN_FN poly1305_auth_avx2
pushq %rbp
movq %rsp, %rbp
andq $-64, %rsp
pushq %r12
pushq %r14
pushq %r15
pushq %rbx
subq $224, %rsp
movq %rsi, %r14
movq %rdi, %rbx
lea (%rsp), %rdi
movq %rcx, %rsi
movq %rdx, %r12
call poly1305_init_ext_avx2_local
poly1305_auth_avx2_2:
movq %r12, %r15
andq $-64, %r15
je poly1305_auth_avx2_5
poly1305_auth_avx2_3:
movq %r14, %rsi
lea (%rsp), %rdi
movq %r15, %rdx
call poly1305_blocks_avx2_local
poly1305_auth_avx2_4:
addq %r15, %r14
subq %r15, %r12
poly1305_auth_avx2_5:
movq %r14, %rsi
lea (%rsp), %rdi
movq %r12, %rdx
movq %rbx, %rcx
call poly1305_finish_ext_avx2_local
poly1305_auth_avx2_6:
addq $224, %rsp
popq %rbx
popq %r15
popq %r14
popq %r12
movq %rbp, %rsp
popq %rbp
ret
FN_END poly1305_auth_avx2


GLOBAL_HIDDEN_FN poly1305_finish_ext_avx2
poly1305_finish_ext_avx2_local:
pushq %rbp
movq %rsp, %rbp
andq $-64, %rsp
pushq %r12
pushq %r13
pushq %r14
subq $104, %rsp
movq %rdx, %r13
movq %rcx, %r14
movq %rdi, %r12
testq %r13, %r13
je poly1305_finish_ext_avx2_29
poly1305_finish_ext_avx2_2:
lea (%rsp), %rax
vpxor %ymm0, %ymm0, %ymm0
subq %rax, %rsi
vmovdqu %ymm0, (%rsp)
vmovdqu %ymm0, 32(%rsp)
testq $32, %r13
je poly1305_finish_ext_avx2_4
poly1305_finish_ext_avx2_3:
vmovdqu (%rsp,%rsi), %ymm0
lea 32(%rsp), %rax
vmovdqu %ymm0, (%rsp)
poly1305_finish_ext_avx2_4:
testq $16, %r13
je poly1305_finish_ext_avx2_6
poly1305_finish_ext_avx2_5:
vmovdqu (%rax,%rsi), %xmm0
vmovdqu %xmm0, (%rax)
addq $16, %rax
poly1305_finish_ext_avx2_6:
testq $8, %r13
je poly1305_finish_ext_avx2_8
poly1305_finish_ext_avx2_7:
movq (%rax,%rsi), %rdx
movq %rdx, (%rax)
addq $8, %rax
poly1305_finish_ext_avx2_8:
testq $4, %r13
je poly1305_finish_ext_avx2_10
poly1305_finish_ext_avx2_9:
movl (%rax,%rsi), %edx
movl %edx, (%rax)
addq $4, %rax
poly1305_finish_ext_avx2_10:
testq $2, %r13
je poly1305_finish_ext_avx2_12
poly1305_finish_ext_avx2_11:
movzwl (%rax,%rsi), %edx
movw %dx, (%rax)
addq $2, %rax
poly1305_finish_ext_avx2_12:
testq $1, %r13
je poly1305_finish_ext_avx2_14
poly1305_finish_ext_avx2_13:
movb (%rax,%rsi), %dl
movb %dl, (%rax)
poly1305_finish_ext_avx2_14:
testq $15, %r13
je poly1305_finish_ext_avx2_16
poly1305_finish_ext_avx2_15:
movb $1, (%rsp,%r13)
poly1305_finish_ext_avx2_16:
movq 176(%r12), %rdx
andq $-8125, %rdx
cmpq $48, %r13
jb poly1305_finish_ext_avx2_18
poly1305_finish_ext_avx2_17:
orq $4, %rdx
jmp poly1305_finish_ext_avx2_21
poly1305_finish_ext_avx2_18:
cmpq $32, %r13
jb poly1305_finish_ext_avx2_20
poly1305_finish_ext_avx2_19:
orq $8, %rdx
jmp poly1305_finish_ext_avx2_21
poly1305_finish_ext_avx2_20:
movq %rdx, %rax
orq $32, %rdx
orq $16, %rax
cmpq $16, %r13
cmovae %rax, %rdx
poly1305_finish_ext_avx2_21:
testq $1, %rdx
je poly1305_finish_ext_avx2_27
poly1305_finish_ext_avx2_22:
cmpq $16, %r13
ja poly1305_finish_ext_avx2_24
poly1305_finish_ext_avx2_23:
orq $256, %rdx
movq %rdx, 176(%r12)
jmp poly1305_finish_ext_avx2_28
poly1305_finish_ext_avx2_24:
cmpq $32, %r13
ja poly1305_finish_ext_avx2_27
poly1305_finish_ext_avx2_25:
orq $128, %rdx
movq %rdx, 176(%r12)
jmp poly1305_finish_ext_avx2_28
poly1305_finish_ext_avx2_27:
movq %rdx, 176(%r12)
poly1305_finish_ext_avx2_28:
movq %r12, %rdi
lea (%rsp), %rsi
movl $64, %edx
vzeroupper
call poly1305_blocks_avx2_local
poly1305_finish_ext_avx2_29:
movq 176(%r12), %rdx
testq $1, %rdx
je poly1305_finish_ext_avx2_37
poly1305_finish_ext_avx2_30:
andq $-8125, %rdx
testq %r13, %r13
je poly1305_finish_ext_avx2_32
poly1305_finish_ext_avx2_31:
cmpq $48, %r13
jbe poly1305_finish_ext_avx2_33
poly1305_finish_ext_avx2_32:
orq $512, %rdx
jmp poly1305_finish_ext_avx2_36
poly1305_finish_ext_avx2_33:
cmpq $32, %r13
jbe poly1305_finish_ext_avx2_35
poly1305_finish_ext_avx2_34:
orq $1024, %rdx
jmp poly1305_finish_ext_avx2_36
poly1305_finish_ext_avx2_35:
movq %rdx, %rax
orq $4096, %rdx
orq $2048, %rax
cmpq $16, %r13
cmova %rax, %rdx
poly1305_finish_ext_avx2_36:
orq $96, %rdx
movq %r12, %rdi
vpxor %ymm0, %ymm0, %ymm0
lea (%rsp), %rsi
movq %rdx, 176(%r12)
movl $64, %edx
vmovdqu %ymm0, (%rsp)
vmovdqu %ymm0, 32(%rsp)
vzeroupper
call poly1305_blocks_avx2_local
poly1305_finish_ext_avx2_37:
movq 8(%r12), %r8
movq %r8, %rsi
movq 16(%r12), %rax
vpxor %ymm0, %ymm0, %ymm0
shlq $44, %rsi
shrq $20, %r8
shlq $24, %rax
orq (%r12), %rsi
orq %rax, %r8
movq 160(%r12), %rdx
movq 168(%r12), %rcx
addq %rdx, %rsi
adcq %rcx, %r8
vmovdqu %ymm0, (%r12)
vmovdqu %ymm0, 32(%r12)
vmovdqu %ymm0, 64(%r12)
vmovdqu %ymm0, 96(%r12)
vmovdqu %ymm0, 128(%r12)
vmovdqu %ymm0, 160(%r12)
movq %rsi, (%r14)
movq %r8, 8(%r14)
vzeroupper
addq $104, %rsp
popq %r14
popq %r13
popq %r12
movq %rbp, %rsp
popq %rbp
ret
FN_END poly1305_finish_ext_avx2

GLOBAL_HIDDEN_FN poly1305_blocks_avx2
poly1305_blocks_avx2_local:
pushq %rbp
movq %rsp, %rbp
andq $-64, %rsp
subq $384, %rsp
movl $16777216, %eax
movl $67108863, %ecx
movl $5, %r8d
vmovd %eax, %xmm1
vmovd %ecx, %xmm10
vmovd %r8d, %xmm0
movq 176(%rdi), %rax
vpbroadcastq %xmm1, %ymm1
vpbroadcastq %xmm10, %ymm10
vpbroadcastq %xmm0, %ymm11
testq $60, %rax
je poly1305_blocks_avx2_11
poly1305_blocks_avx2_2:
vpsrldq $8, %ymm1, %ymm15
testq $4, %rax
je poly1305_blocks_avx2_4
poly1305_blocks_avx2_3:
vpermq $192, %ymm15, %ymm15
poly1305_blocks_avx2_4:
testq $8, %rax
je poly1305_blocks_avx2_6
poly1305_blocks_avx2_5:
vpermq $240, %ymm15, %ymm15
poly1305_blocks_avx2_6:
testq $16, %rax
je poly1305_blocks_avx2_8
poly1305_blocks_avx2_7:
vpermq $252, %ymm15, %ymm15
poly1305_blocks_avx2_8:
testq $32, %rax
je poly1305_blocks_avx2_10
poly1305_blocks_avx2_9:
vpxor %ymm15, %ymm15, %ymm15
poly1305_blocks_avx2_10:
vmovdqa %ymm15, %ymm1
poly1305_blocks_avx2_11:
movq %rax, %rcx
btsq $0, %rcx
jc poly1305_blocks_avx2_13
poly1305_blocks_avx2_12:
vmovdqu (%rsi), %ymm3
movq %rcx, %rax
vmovdqu 32(%rsi), %ymm5
vpunpcklqdq %ymm5, %ymm3, %ymm4
addq $64, %rsi
vpunpckhqdq %ymm5, %ymm3, %ymm7
vpermq $216, %ymm4, %ymm6
addq $-64, %rdx
vpermq $216, %ymm7, %ymm0
vpsrlq $52, %ymm6, %ymm8
vpsllq $12, %ymm0, %ymm9
vpsrlq $26, %ymm6, %ymm2
vpsrlq $40, %ymm0, %ymm0
vpand %ymm6, %ymm10, %ymm4
vpor %ymm9, %ymm8, %ymm7
vpand %ymm2, %ymm10, %ymm3
vpor %ymm1, %ymm0, %ymm9
vpsrlq $26, %ymm7, %ymm2
vpand %ymm7, %ymm10, %ymm5
vpand %ymm2, %ymm10, %ymm7
movq %rax, 176(%rdi)
jmp poly1305_blocks_avx2_14
poly1305_blocks_avx2_13:
vpermq $216, (%rdi), %ymm15
vpxor %ymm0, %ymm0, %ymm0
vpermq $216, 32(%rdi), %ymm14
vpermq $216, 64(%rdi), %ymm13
vpunpckldq %ymm0, %ymm15, %ymm4
vpunpckhdq %ymm0, %ymm15, %ymm3
vpunpckldq %ymm0, %ymm14, %ymm5
vpunpckhdq %ymm0, %ymm14, %ymm7
vpunpckldq %ymm0, %ymm13, %ymm9
poly1305_blocks_avx2_14:
cmpq $64, %rdx
jb poly1305_blocks_avx2_34
poly1305_blocks_avx2_15:
vmovdqu 140(%rdi), %ymm0
testq $8064, %rax
je poly1305_blocks_avx2_29
poly1305_blocks_avx2_16:
vpermq $216, 80(%rdi), %ymm6
vpermq $216, 100(%rdi), %ymm2
vpermq $216, 120(%rdi), %ymm8
vpermq $216, %ymm0, %ymm0
testq $128, %rax
je poly1305_blocks_avx2_18
poly1305_blocks_avx2_17:
vmovdqa %ymm0, %ymm15
vmovdqa %ymm0, %ymm14
vmovdqa %ymm0, %ymm13
vmovdqa %ymm8, %ymm12
jmp poly1305_blocks_avx2_28
poly1305_blocks_avx2_18:
testq $256, %rax
je poly1305_blocks_avx2_20
poly1305_blocks_avx2_19:
vmovdqa %ymm0, %ymm15
vmovdqa %ymm0, %ymm14
vmovdqa %ymm8, %ymm13
vmovdqa %ymm2, %ymm12
jmp poly1305_blocks_avx2_28
poly1305_blocks_avx2_20:
testq $512, %rax
je poly1305_blocks_avx2_22
poly1305_blocks_avx2_21:
vmovdqa %ymm0, %ymm15
vmovdqa %ymm8, %ymm14
vmovdqa %ymm2, %ymm13
vmovdqa %ymm6, %ymm12
jmp poly1305_blocks_avx2_28
poly1305_blocks_avx2_22:
testq $1024, %rax
je poly1305_blocks_avx2_24
poly1305_blocks_avx2_23:
vpxor %ymm12, %ymm12, %ymm12
movl $1, %r8d
vmovdqa %ymm8, %ymm15
vmovdqa %ymm2, %ymm14
vmovdqa %ymm6, %ymm13
vmovd %r8d, %xmm12
jmp poly1305_blocks_avx2_28
poly1305_blocks_avx2_24:
testq $2048, %rax
je poly1305_blocks_avx2_26
poly1305_blocks_avx2_25:
vpxor %ymm12, %ymm12, %ymm12
movl $1, %r8d
vmovd %r8d, %xmm13
vmovdqa %ymm2, %ymm15
vmovdqa %ymm6, %ymm14
vmovdqa %ymm13, %ymm12
jmp poly1305_blocks_avx2_28
poly1305_blocks_avx2_26:
testq $4096, %rax
je poly1305_blocks_avx2_28
poly1305_blocks_avx2_27:
movl $1, %r8d
vmovd %r8d, %xmm14
vmovdqa %ymm6, %ymm15
vmovdqa %ymm14, %ymm13
vmovdqa %ymm14, %ymm12
poly1305_blocks_avx2_28:
vpunpcklqdq %ymm14, %ymm15, %ymm6
vpunpcklqdq %ymm12, %ymm13, %ymm8
vpunpckhqdq %ymm14, %ymm15, %ymm14
vpunpckhqdq %ymm12, %ymm13, %ymm12
vperm2i128 $32, %ymm8, %ymm6, %ymm2
vperm2i128 $49, %ymm8, %ymm6, %ymm6
vpsrlq $32, %ymm6, %ymm0
vpsrlq $32, %ymm2, %ymm8
vmovdqu %ymm0, 352(%rsp)
vperm2i128 $32, %ymm12, %ymm14, %ymm13
vmovdqu %ymm13, 320(%rsp)
jmp poly1305_blocks_avx2_30
poly1305_blocks_avx2_29:
vpsrlq $32, %ymm0, %ymm12
vpermq $0, %ymm0, %ymm2
vpermq $85, %ymm0, %ymm6
vpermq $85, %ymm12, %ymm13
vpermq $170, %ymm0, %ymm0
vpermq $0, %ymm12, %ymm8
vmovdqu %ymm13, 352(%rsp)
vmovdqu %ymm0, 320(%rsp)
poly1305_blocks_avx2_30:
vmovdqu (%rsi), %ymm12
movq %rdx, %r9
vmovdqu 352(%rsp), %ymm15
vmovdqu %ymm1, 160(%rsp)
vmovdqu %ymm10, 192(%rsp)
vmovdqu %ymm11, 128(%rsp)
vperm2i128 $32, 32(%rsi), %ymm12, %ymm13
xorl %r8d, %r8d
vperm2i128 $49, 32(%rsi), %ymm12, %ymm12
xorl %ecx, %ecx
vpmuludq %ymm11, %ymm8, %ymm0
vpmuludq %ymm11, %ymm6, %ymm1
vmovdqu %ymm0, 224(%rsp)
vmovdqu %ymm1, 256(%rsp)
vpunpckldq %ymm12, %ymm13, %ymm14
vpunpckhdq %ymm12, %ymm13, %ymm12
vmovdqu %ymm14, 32(%rsp)
vpmuludq %ymm0, %ymm9, %ymm0
vpmuludq %ymm1, %ymm7, %ymm13
vpaddq %ymm13, %ymm0, %ymm0
vpmuludq %ymm11, %ymm15, %ymm10
vpmuludq %ymm10, %ymm5, %ymm13
vpaddq %ymm13, %ymm0, %ymm0
vmovdqu %ymm10, 288(%rsp)
vpmuludq 320(%rsp), %ymm11, %ymm11
vpmuludq %ymm11, %ymm3, %ymm13
vpaddq %ymm13, %ymm0, %ymm0
vmovdqu %ymm11, (%rsp)
vpmuludq %ymm2, %ymm4, %ymm13
vpaddq %ymm13, %ymm0, %ymm0
vpxor %ymm13, %ymm13, %ymm13
vpunpckldq %ymm13, %ymm14, %ymm14
vpaddq %ymm14, %ymm0, %ymm0
vmovdqu %ymm0, 64(%rsp)
vpmuludq %ymm11, %ymm9, %ymm14
vpmuludq %ymm2, %ymm7, %ymm0
vpaddq %ymm0, %ymm14, %ymm14
vpmuludq %ymm8, %ymm5, %ymm0
vpaddq %ymm0, %ymm14, %ymm14
vpmuludq %ymm6, %ymm3, %ymm0
vpaddq %ymm0, %ymm14, %ymm14
vpmuludq %ymm15, %ymm4, %ymm0
vpaddq %ymm0, %ymm14, %ymm0
vpunpckhdq %ymm13, %ymm12, %ymm14
vpsllq $18, %ymm14, %ymm14
vpaddq %ymm14, %ymm0, %ymm14
vpmuludq %ymm1, %ymm9, %ymm1
vpmuludq %ymm10, %ymm7, %ymm0
vpaddq %ymm0, %ymm1, %ymm1
vpmuludq %ymm11, %ymm5, %ymm0
vpaddq %ymm0, %ymm1, %ymm1
vpmuludq %ymm2, %ymm3, %ymm0
vpaddq %ymm0, %ymm1, %ymm1
vpmuludq %ymm8, %ymm4, %ymm0
vpaddq %ymm0, %ymm1, %ymm1
vmovdqu 32(%rsp), %ymm0
vpunpckhdq %ymm13, %ymm0, %ymm0
vpsllq $6, %ymm0, %ymm0
vpaddq %ymm0, %ymm1, %ymm1
vmovdqu 64(%rsp), %ymm0
vpsrlq $26, %ymm0, %ymm0
vpaddq %ymm0, %ymm1, %ymm1
vmovdqu %ymm1, 96(%rsp)
vpmuludq %ymm2, %ymm9, %ymm1
vpmuludq %ymm8, %ymm7, %ymm0
vpaddq %ymm0, %ymm1, %ymm1
vpmuludq %ymm10, %ymm9, %ymm10
vpmuludq %ymm11, %ymm7, %ymm11
vpaddq %ymm11, %ymm10, %ymm7
vpmuludq %ymm6, %ymm5, %ymm0
vpaddq %ymm0, %ymm1, %ymm1
vpmuludq %ymm2, %ymm5, %ymm5
vpaddq %ymm5, %ymm7, %ymm10
vpmuludq %ymm15, %ymm3, %ymm15
vpaddq %ymm15, %ymm1, %ymm1
vpmuludq %ymm8, %ymm3, %ymm11
vpaddq %ymm11, %ymm10, %ymm5
vpunpckldq %ymm13, %ymm12, %ymm10
vmovdqu 96(%rsp), %ymm12
vpmuludq 320(%rsp), %ymm4, %ymm0
vpaddq %ymm0, %ymm1, %ymm15
vpsrlq $26, %ymm12, %ymm3
vmovdqu 160(%rsp), %ymm1
vpmuludq %ymm6, %ymm4, %ymm4
vpaddq %ymm1, %ymm15, %ymm0
vpsrlq $26, %ymm14, %ymm15
vpaddq %ymm4, %ymm5, %ymm11
vpsllq $12, %ymm10, %ymm4
vmovdqu 192(%rsp), %ymm10
vpaddq %ymm15, %ymm0, %ymm0
vpaddq %ymm4, %ymm11, %ymm5
vmovdqu 128(%rsp), %ymm11
vpsrlq $26, %ymm0, %ymm9
vpaddq %ymm3, %ymm5, %ymm7
vpand 64(%rsp), %ymm10, %ymm13
vpand %ymm10, %ymm12, %ymm12
vpand %ymm10, %ymm7, %ymm5
vpsrlq $26, %ymm7, %ymm7
vpmuludq %ymm11, %ymm9, %ymm15
vpand %ymm10, %ymm14, %ymm9
vpaddq %ymm15, %ymm13, %ymm3
vpand %ymm10, %ymm0, %ymm14
vpaddq %ymm7, %ymm9, %ymm9
vpand %ymm10, %ymm3, %ymm4
vpsrlq $26, %ymm3, %ymm3
vpsrlq $26, %ymm9, %ymm0
vpand %ymm10, %ymm9, %ymm7
vpaddq %ymm3, %ymm12, %ymm3
vpaddq %ymm0, %ymm14, %ymm9
sarq $5, %r9
shrq $58, %r9
addq %rdx, %r9
sarq $6, %r9
cmpq $2, %r9
jl poly1305_blocks_avx2_34
poly1305_blocks_avx2_31:
vmovdqu %ymm6, 32(%rsp)
lea -64(%rdx), %r9
vmovdqu %ymm8, 64(%rsp)
vmovdqu %ymm11, 128(%rsp)
vmovdqu %ymm10, 192(%rsp)
vmovdqu %ymm1, 160(%rsp)
vmovdqu (%rsp), %ymm12
sarq $5, %r9
shrq $58, %r9
lea -64(%rdx,%r9), %rdx
sarq $6, %rdx
poly1305_blocks_avx2_32:
vmovdqu 256(%rsp), %ymm15
incq %r8
vmovdqu 64(%rcx,%rsi), %ymm11
vpmuludq 224(%rsp), %ymm9, %ymm8
vpmuludq %ymm15, %ymm7, %ymm14
vpaddq %ymm14, %ymm8, %ymm1
vmovdqu 288(%rsp), %ymm8
vperm2i128 $32, 96(%rcx,%rsi), %ymm11, %ymm10
vperm2i128 $49, 96(%rcx,%rsi), %ymm11, %ymm6
addq $64, %rcx
vpmuludq %ymm8, %ymm5, %ymm13
vpunpckldq %ymm6, %ymm10, %ymm0
vpunpckhdq %ymm6, %ymm10, %ymm11
vpaddq %ymm13, %ymm1, %ymm10
vpmuludq %ymm12, %ymm3, %ymm6
vpaddq %ymm6, %ymm10, %ymm14
vpxor %ymm10, %ymm10, %ymm10
vpunpckldq %ymm10, %ymm0, %ymm6
vpunpckhdq %ymm10, %ymm0, %ymm0
vpmuludq %ymm2, %ymm4, %ymm1
vpaddq %ymm1, %ymm14, %ymm13
vpaddq %ymm6, %ymm13, %ymm1
vmovdqu 64(%rsp), %ymm6
vmovdqu %ymm1, (%rsp)
vpsrlq $26, %ymm1, %ymm1
vpmuludq %ymm12, %ymm9, %ymm14
vpmuludq %ymm2, %ymm7, %ymm13
vpaddq %ymm13, %ymm14, %ymm14
vpmuludq %ymm6, %ymm5, %ymm13
vpaddq %ymm13, %ymm14, %ymm14
vpmuludq 32(%rsp), %ymm3, %ymm13
vpaddq %ymm13, %ymm14, %ymm14
vpmuludq 352(%rsp), %ymm4, %ymm13
vpaddq %ymm13, %ymm14, %ymm13
vpunpckhdq %ymm10, %ymm11, %ymm14
vpsllq $18, %ymm14, %ymm14
vpaddq %ymm14, %ymm13, %ymm13
vpmuludq %ymm15, %ymm9, %ymm15
vpmuludq %ymm8, %ymm7, %ymm14
vpaddq %ymm14, %ymm15, %ymm15
vpmuludq %ymm12, %ymm5, %ymm14
vpaddq %ymm14, %ymm15, %ymm15
vpmuludq %ymm2, %ymm3, %ymm14
vpaddq %ymm14, %ymm15, %ymm15
vpmuludq %ymm6, %ymm4, %ymm14
vpaddq %ymm14, %ymm15, %ymm14
vpsllq $6, %ymm0, %ymm15
vpaddq %ymm15, %ymm14, %ymm14
vmovdqu 32(%rsp), %ymm15
vpaddq %ymm1, %ymm14, %ymm1
vpmuludq %ymm2, %ymm9, %ymm0
vpmuludq %ymm6, %ymm7, %ymm14
vpmuludq %ymm8, %ymm9, %ymm9
vpmuludq %ymm12, %ymm7, %ymm7
vpaddq %ymm7, %ymm9, %ymm7
vpaddq %ymm14, %ymm0, %ymm0
vpsrlq $26, %ymm1, %ymm9
vpmuludq %ymm15, %ymm5, %ymm14
vpmuludq %ymm2, %ymm5, %ymm5
vpaddq %ymm5, %ymm7, %ymm5
vpaddq %ymm14, %ymm0, %ymm0
vpmuludq 352(%rsp), %ymm3, %ymm14
vpmuludq %ymm6, %ymm3, %ymm3
vpaddq %ymm3, %ymm5, %ymm5
vpaddq %ymm14, %ymm0, %ymm0
vpmuludq 320(%rsp), %ymm4, %ymm14
vpmuludq %ymm15, %ymm4, %ymm4
vpaddq %ymm4, %ymm5, %ymm5
vpaddq %ymm14, %ymm0, %ymm0
vpunpckldq %ymm10, %ymm11, %ymm4
vpaddq 160(%rsp), %ymm0, %ymm14
vpsrlq $26, %ymm13, %ymm0
vpsllq $12, %ymm4, %ymm3
vpaddq %ymm0, %ymm14, %ymm14
vpaddq %ymm3, %ymm5, %ymm7
vpsrlq $26, %ymm14, %ymm0
vpaddq %ymm9, %ymm7, %ymm10
vmovdqu 192(%rsp), %ymm9
vpsrlq $26, %ymm10, %ymm11
vpand (%rsp), %ymm9, %ymm6
vpand %ymm9, %ymm13, %ymm13
vpand %ymm9, %ymm1, %ymm1
vpand %ymm9, %ymm14, %ymm14
vpand %ymm9, %ymm10, %ymm5
vpmuludq 128(%rsp), %ymm0, %ymm8
vpaddq %ymm8, %ymm6, %ymm15
vpaddq %ymm11, %ymm13, %ymm0
vpsrlq $26, %ymm15, %ymm3
vpand %ymm9, %ymm0, %ymm7
vpsrlq $26, %ymm0, %ymm0
vpand %ymm9, %ymm15, %ymm4
vpaddq %ymm3, %ymm1, %ymm3
vpaddq %ymm0, %ymm14, %ymm9
cmpq %rdx, %r8
jb poly1305_blocks_avx2_32
poly1305_blocks_avx2_34:
testq $64, %rax
jne poly1305_blocks_avx2_36
poly1305_blocks_avx2_35:
vpshufd $8, %ymm4, %ymm0
vpshufd $8, %ymm3, %ymm3
vpshufd $8, %ymm5, %ymm5
vpshufd $8, %ymm7, %ymm7
vpshufd $8, %ymm9, %ymm9
vpermq $8, %ymm0, %ymm1
vpermq $8, %ymm3, %ymm2
vpermq $8, %ymm5, %ymm4
vpermq $8, %ymm7, %ymm6
vpermq $8, %ymm9, %ymm11
vperm2i128 $32, %ymm2, %ymm1, %ymm8
vperm2i128 $32, %ymm6, %ymm4, %ymm10
vmovdqu %ymm8, (%rdi)
vmovdqu %ymm10, 32(%rdi)
vmovdqu %xmm11, 64(%rdi)
jmp poly1305_blocks_avx2_37
poly1305_blocks_avx2_36:
vpermq $245, %ymm4, %ymm0
vpaddq %ymm0, %ymm4, %ymm4
vpermq $245, %ymm3, %ymm1
vpaddq %ymm1, %ymm3, %ymm10
vpermq $245, %ymm5, %ymm3
vpermq $170, %ymm4, %ymm6
vpaddq %ymm3, %ymm5, %ymm13
vpaddq %ymm6, %ymm4, %ymm8
vpermq $170, %ymm10, %ymm11
vpermq $245, %ymm7, %ymm5
vpaddq %ymm11, %ymm10, %ymm12
vpaddq %ymm5, %ymm7, %ymm7
vpermq $170, %ymm13, %ymm14
vpermq $245, %ymm9, %ymm2
vpaddq %ymm14, %ymm13, %ymm15
vpaddq %ymm2, %ymm9, %ymm9
vpermq $170, %ymm7, %ymm0
vpaddq %ymm0, %ymm7, %ymm1
vpermq $170, %ymm9, %ymm2
vpaddq %ymm2, %ymm9, %ymm3
vmovd %xmm8, %r9d
movl %r9d, %r8d
shrl $26, %r8d
andq $67108863, %r9
vmovd %xmm12, %esi
addl %r8d, %esi
movl %esi, %r11d
shrl $26, %esi
andq $67108863, %r11
vmovd %xmm15, %ecx
addl %esi, %ecx
movl %ecx, %eax
shrl $26, %eax
andq $67108863, %rcx
shlq $8, %rcx
vmovd %xmm1, %r8d
addl %eax, %r8d
movl %r8d, %r10d
shrl $26, %r8d
andq $67108863, %r10
movq %r10, %rax
shrq $10, %rax
shlq $34, %r10
vmovd %xmm3, %edx
addl %r8d, %edx
shlq $16, %rdx
orq %rdx, %rax
movq %rax, %r8
shrq $42, %r8
lea (%r8,%r8,4), %rdx
movq %r11, %r8
shlq $26, %r8
orq %r8, %r9
movq $0xfffffffffff, %r8
shrq $18, %r11
andq %r8, %r9
addq %r9, %rdx
orq %rcx, %r11
movq %rdx, %rsi
orq %r10, %r11
shrq $44, %rsi
andq %r8, %r11
addq %r11, %rsi
movq $0x3ffffffffff, %r9
movq %rsi, %r10
andq %r9, %rax
shrq $44, %r10
andq %r8, %rdx
addq %r10, %rax
movq %r8, %rcx
andq %rax, %r9
andq %r8, %rsi
shrq $42, %rax
movq $0xfffffc0000000000, %r10
lea (%rax,%rax,4), %r11
addq %r11, %rdx
andq %rdx, %rcx
shrq $44, %rdx
addq %rdx, %rsi
lea 5(%rcx), %rdx
movq %rdx, %r11
andq %r8, %rdx
shrq $44, %r11
addq %rsi, %r11
movq %r11, %rax
andq %r11, %r8
shrq $44, %rax
addq %r9, %rax
addq %r10, %rax
movq %rax, %r10
shrq $63, %r10
decq %r10
andn %rcx, %r10, %rcx
andq %r10, %rdx
orq %rdx, %rcx
andq %r10, %r8
andn %rsi, %r10, %rdx
andq %r10, %rax
andn %r9, %r10, %rsi
orq %r8, %rdx
orq %rax, %rsi
movq %rcx, (%rdi)
movq %rdx, 8(%rdi)
movq %rsi, 16(%rdi)
poly1305_blocks_avx2_37:
vzeroupper
movq %rbp, %rsp
popq %rbp
ret
FN_END poly1305_blocks_avx2

GLOBAL_HIDDEN_FN poly1305_init_ext_avx2
poly1305_init_ext_avx2_local:
pushq %r12
pushq %r13
pushq %r14
pushq %r15
pushq %rbx
movq %rdi, %r10
vpxor %ymm0, %ymm0, %ymm0
movq %rdx, %r12
vpxor %xmm1, %xmm1, %xmm1
vmovdqu %xmm1, 64(%r10)
vmovdqu %ymm0, (%r10)
vmovdqu %ymm0, 32(%r10)
movq $-1, %r8
testq %r12, %r12
movq 8(%rsi), %rdi
movq $0xffc0fffffff, %r9
movq %rdi, %rcx
cmove %r8, %r12
movq (%rsi), %r8
andq %r8, %r9
shrq $44, %r8
movq $0xfffffc0ffff, %r11
shlq $20, %rcx
shrq $24, %rdi
orq %rcx, %r8
movq $0xffffffc0f, %rcx
andq %r11, %r8
andq %rcx, %rdi
movq 16(%rsi), %rcx
movq %rcx, 160(%r10)
movq %r9, %rcx
movq 24(%rsi), %rdx
movq %rdx, 168(%r10)
movl %r9d, %edx
andl $67108863, %edx
movl %edx, 80(%r10)
movq %r8, %rdx
shrq $26, %rcx
shlq $18, %rdx
orq %rdx, %rcx
movq %r8, %rdx
shrq $8, %rdx
andl $67108863, %ecx
andl $67108863, %edx
movl %ecx, 84(%r10)
movq %r8, %rcx
movl %edx, 88(%r10)
movq %rdi, %rdx
shrq $34, %rcx
shlq $10, %rdx
orq %rdx, %rcx
movq %rdi, %rdx
shrq $16, %rdx
andl $67108863, %ecx
movl %ecx, 92(%r10)
movl %edx, 96(%r10)
cmpq $16, %r12
jbe poly1305_init_ext_avx2_7
poly1305_init_ext_avx2_2:
movq %r9, %rax
lea (%rdi,%rdi,4), %r14
mulq %r9
shlq $2, %r14
movq %rax, %r11
movq %rdx, %r15
lea (%r8,%r8), %rax
mulq %r14
addq %rax, %r11
lea (%r9,%r9), %rax
movq %r11, %rsi
adcq %rdx, %r15
mulq %r8
movq %rax, %rbx
movq %r14, %rax
movq %rdx, %rcx
lea (%rdi,%rdi), %r14
mulq %rdi
addq %rax, %rbx
movq %r8, %rax
adcq %rdx, %rcx
mulq %r8
shlq $20, %r15
movq %rax, %r13
shrq $44, %rsi
movq %r9, %rax
orq %rsi, %r15
movq %rdx, %rsi
mulq %r14
addq %r15, %rbx
movq %rbx, %r15
adcq $0, %rcx
addq %rax, %r13
adcq %rdx, %rsi
shlq $20, %rcx
shrq $44, %r15
orq %r15, %rcx
addq %rcx, %r13
movq $0xfffffffffff, %rcx
movq %r13, %rdx
adcq $0, %rsi
andq %rcx, %r11
shlq $22, %rsi
andq %rcx, %rbx
shrq $42, %rdx
orq %rdx, %rsi
lea (%rsi,%rsi,4), %rsi
addq %rsi, %r11
movq %rcx, %rsi
andq %r11, %rsi
shrq $44, %r11
addq %r11, %rbx
movq $0x3ffffffffff, %r11
andq %rbx, %rcx
andq %r11, %r13
shrq $44, %rbx
movq %rsi, %r11
movq %rcx, %rdx
addq %r13, %rbx
shrq $26, %r11
movq %rbx, %r15
shlq $18, %rdx
movq %rcx, %r14
orq %rdx, %r11
movq %rcx, %rdx
shrq $34, %rdx
movl %esi, %r13d
shlq $10, %r15
andl $67108863, %r13d
orq %r15, %rdx
andl $67108863, %r11d
shrq $8, %r14
andl $67108863, %edx
movl %edx, 112(%r10)
movq %rbx, %rdx
shrq $16, %rdx
andl $67108863, %r14d
movl %r13d, 100(%r10)
movl %r11d, 104(%r10)
movl %r14d, 108(%r10)
movl %edx, 116(%r10)
cmpq $48, %r12
jbe poly1305_init_ext_avx2_4
poly1305_init_ext_avx2_3:
movq %rsi, %rax
lea (%rbx,%rbx,4), %r15
mulq %rsi
shlq $2, %r15
movq %rax, %r13
movq %rdx, %r12
lea (%rcx,%rcx), %rax
mulq %r15
addq %rax, %r13
lea (%rsi,%rsi), %rax
movq %r15, -16(%rsp)
adcq %rdx, %r12
mulq %rcx
movq %rax, %r14
movq %rbx, %rax
movq %rdx, %r11
mulq %r15
addq %rax, %r14
movq %rcx, %rax
movq %r13, %r15
adcq %rdx, %r11
mulq %rcx
shlq $20, %r12
shrq $44, %r15
orq %r15, %r12
movq %rax, %r15
addq %r12, %r14
movq %rdx, %r12
movq %rsi, %rax
lea (%rbx,%rbx), %rdx
adcq $0, %r11
mulq %rdx
addq %rax, %r15
adcq %rdx, %r12
movq %r14, %rdx
shlq $20, %r11
shrq $44, %rdx
orq %rdx, %r11
addq %r11, %r15
movq $0xfffffffffff, %r11
movq %r15, %rdx
adcq $0, %r12
andq %r11, %r13
shlq $22, %r12
andq %r11, %r14
shrq $42, %rdx
orq %rdx, %r12
lea (%r12,%r12,4), %r12
addq %r12, %r13
movq %r11, %r12
andq %r13, %r12
shrq $44, %r13
addq %r13, %r14
movq $0x3ffffffffff, %r13
andq %r14, %r11
andq %r13, %r15
shrq $44, %r14
movq %r11, %rdx
shlq $18, %rdx
addq %r14, %r15
movl %r12d, %r14d
movq %r11, %r13
shrq $26, %r12
andl $67108863, %r14d
orq %rdx, %r12
movq %r15, %rdx
shrq $34, %r11
shlq $10, %rdx
andl $67108863, %r12d
orq %rdx, %r11
shrq $8, %r13
andl $67108863, %r11d
movl %r11d, 152(%r10)
andl $67108863, %r13d
shrq $16, %r15
movl %r14d, 140(%r10)
movl %r12d, 144(%r10)
movl %r13d, 148(%r10)
movl %r15d, 156(%r10)
movq -16(%rsp), %r11
jmp poly1305_init_ext_avx2_6
poly1305_init_ext_avx2_4:
cmpq $32, %r12
jbe poly1305_init_ext_avx2_7
poly1305_init_ext_avx2_5:
lea (%rbx,%rbx,4), %r11
shlq $2, %r11
poly1305_init_ext_avx2_6:
movq %r9, %rax
lea (%rcx,%rcx,4), %r13
mulq %rsi
shlq $2, %r13
movq %rax, %r14
movq %rdi, %rax
movq %rdx, %r12
mulq %r13
addq %rax, %r14
movq %r8, %rax
adcq %rdx, %r12
mulq %r11
addq %rax, %r14
movq %r8, %rax
adcq %rdx, %r12
mulq %rsi
movq %rax, %r15
movq %r9, %rax
movq %rdx, %r13
mulq %rcx
addq %rax, %r15
movq %r11, %rax
movq %r14, %r11
adcq %rdx, %r13
mulq %rdi
addq %rax, %r15
movq %rdi, %rax
adcq %rdx, %r13
mulq %rsi
shlq $20, %r12
movq %rax, %rsi
shrq $44, %r11
movq %r8, %rax
orq %r11, %r12
movq %rdx, %rdi
mulq %rcx
addq %r12, %r15
movq %r15, %rcx
adcq $0, %r13
addq %rax, %rsi
movq %r9, %rax
movq $0xfffffffffff, %r9
adcq %rdx, %rdi
andq %r9, %r14
mulq %rbx
addq %rax, %rsi
adcq %rdx, %rdi
movq %r9, %rdx
shlq $20, %r13
andq %r9, %r15
shrq $44, %rcx
orq %rcx, %r13
addq %r13, %rsi
movq %rsi, %rbx
adcq $0, %rdi
shlq $22, %rdi
shrq $42, %rbx
orq %rbx, %rdi
lea (%rdi,%rdi,4), %r8
addq %r8, %r14
andq %r14, %rdx
shrq $44, %r14
addq %r14, %r15
movq $0x3ffffffffff, %r14
andq %r15, %r9
andq %r14, %rsi
shrq $44, %r15
movq %r9, %rax
addq %r15, %rsi
movl %edx, %r15d
movq %rsi, %rbx
movq %r9, %rcx
shrq $26, %rdx
andl $67108863, %r15d
shlq $18, %rax
shrq $34, %r9
orq %rax, %rdx
shlq $10, %rbx
shrq $8, %rcx
orq %rbx, %r9
shrq $16, %rsi
andl $67108863, %edx
andl $67108863, %ecx
andl $67108863, %r9d
movl %r15d, 120(%r10)
movl %edx, 124(%r10)
movl %ecx, 128(%r10)
movl %r9d, 132(%r10)
movl %esi, 136(%r10)
poly1305_init_ext_avx2_7:
movq $0, 176(%r10)
vzeroupper
popq %rbx
popq %r15
popq %r14
popq %r13
popq %r12
ret
FN_END poly1305_init_ext_avx2

