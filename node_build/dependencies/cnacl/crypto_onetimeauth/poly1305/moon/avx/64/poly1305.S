#include "x86.inc"

SECTION_TEXT

GLOBAL_HIDDEN_FN_EXT poly1305_block_size_avx,0,0
movl $32, %eax
ret
FN_END poly1305_block_size_avx

GLOBAL_HIDDEN_FN_EXT poly1305_init_ext_avx,4,1
poly1305_init_ext_avx_local:
pushq %r15
pushq %r14
pushq %r13
pushq %r12
pushq %rbp
pushq %rbx
movq %rdi, %rbp
testq %rdx, %rdx
movq $-1, %rax
cmovne %rdx, %rax
movq %rax, -16(%rsp)
vpxor %xmm0, %xmm0, %xmm0
vmovdqu %xmm0, (%rdi)
vmovdqu %xmm0, 16(%rdi)
vmovdqu %xmm0, 32(%rdi)
movq (%rsi), %r9
movq 8(%rsi), %r8
movabsq $17575274610687, %r10
andq %r9, %r10
shrq $44, %r9
movq %r8, %rax
salq $20, %rax
orq %rax, %r9
movabsq $17592181915647, %rax
andq %rax, %r9
shrq $24, %r8
movabsq $68719475727, %rax
andq %rax, %r8
leaq 40(%rdi), %r15
movl %r10d, %eax
andl $67108863, %eax
movl %eax, 40(%rdi)
movl %r9d, %edx
sall $18, %edx
movq %r10, %rax
shrq $26, %rax
orl %edx, %eax
andl $67108863, %eax
movl %eax, 44(%rdi)
movq %r9, %rax
shrq $8, %rax
andl $67108863, %eax
movl %eax, 48(%rdi)
movq %r9, %rax
shrq $34, %rax
movl %r8d, %edx
sall $10, %edx
orl %edx, %eax
andl $67108863, %eax
movl %eax, 52(%rdi)
movq %r8, %rax
shrq $16, %rax
movl %eax, 56(%rdi)
movq 16(%rsi), %rax
movq %rax, 104(%rdi)
movq 24(%rsi), %rax
movq %rax, 112(%rdi)
movl $0, %ebx
.L7:
testq %rbx, %rbx
jne .L4
leaq 60(%rbp), %r15
cmpq $16, -16(%rsp)
ja .L6
jmp .L5
.L4:
cmpq $1, %rbx
jne .L6
leaq 80(%rbp), %r15
cmpq $95, -16(%rsp)
jbe .L5
.L6:
leaq (%r8,%r8,4), %rsi
salq $2, %rsi
leaq (%r9,%r9), %rdi
movq %rdi, %rax
mulq %rsi
movq %rax, %r13
movq %rdx, %r14
movq %r10, %rax
mulq %r10
addq %r13, %rax
adcq %r14, %rdx
movabsq $17592186044415, %rcx
movq %rax, -72(%rsp)
movq %rdx, -64(%rsp)
andq -72(%rsp), %rcx
leaq (%r10,%r10), %r11
movq %r11, %rax
mulq %r9
movq %rax, %r11
movq %rdx, %r12
movq %rsi, %rax
mulq %r8
movq %rax, %r13
movq %rdx, %r14
addq %r11, %r13
adcq %r12, %r14
movq -72(%rsp), %rax
movq -64(%rsp), %rdx
shrdq $44, %rdx, %rax
movq %rax, -56(%rsp)
movq $0, -48(%rsp)
addq -56(%rsp), %r13
adcq -48(%rsp), %r14
movabsq $17592186044415, %rsi
andq %r13, %rsi
leaq (%r8,%r8), %rdi
movq %rdi, %rax
mulq %r10
movq %rax, %r11
movq %rdx, %r12
movq %r9, %rax
mulq %r9
addq %r11, %rax
adcq %r12, %rdx
shrdq $44, %r14, %r13
movq %r13, -40(%rsp)
movq $0, -32(%rsp)
addq -40(%rsp), %rax
adcq -32(%rsp), %rdx
movabsq $4398046511103, %rdi
andq %rax, %rdi
shrdq $42, %rdx, %rax
leaq (%rax,%rax,4), %r8
addq %rcx, %r8
movabsq $17592186044415, %r10
andq %r8, %r10
shrq $44, %r8
addq %rsi, %r8
movabsq $17592186044415, %r9
andq %r8, %r9
shrq $44, %r8
addq %rdi, %r8
movl %r10d, %eax
andl $67108863, %eax
movl %eax, (%r15)
movl %r9d, %edx
sall $18, %edx
movq %r10, %rax
shrq $26, %rax
orl %edx, %eax
andl $67108863, %eax
movl %eax, 4(%r15)
movq %r9, %rax
shrq $8, %rax
andl $67108863, %eax
movl %eax, 8(%r15)
movl %r8d, %edx
sall $10, %edx
movq %r9, %rax
shrq $34, %rax
orl %edx, %eax
andl $67108863, %eax
movl %eax, 12(%r15)
movq %r8, %rax
shrq $16, %rax
movl %eax, 16(%r15)
addq $1, %rbx
cmpq $2, %rbx
jne .L7
.L5:
movq $0, 120(%rbp)
popq %rbx
popq %rbp
popq %r12
popq %r13
popq %r14
popq %r15
ret
FN_END poly1305_init_ext_avx



GLOBAL_HIDDEN_FN poly1305_blocks_avx
poly1305_blocks_avx_local:
pushq %rbp
movq %rsp, %rbp
pushq %rbx
andq $-64, %rsp
subq $200, %rsp
movl $(1 << 24), %eax
movl $((1 << 26) - 1), %r8d
movl $(5), %r9d
vmovd %eax, %xmm1
vmovd %r8d, %xmm0
vmovd %r9d, %xmm2
vpshufd $68, %xmm1, %xmm1
vpshufd $68, %xmm0, %xmm0
vpshufd $68, %xmm2, %xmm2
vmovdqa %xmm1, 152(%rsp)
vmovdqa %xmm2, 184(%rsp)
movq 120(%rdi), %rax
testb $4, %al
je .L12
vpsrldq $8, %xmm1, %xmm1
vmovdqa %xmm1, 152(%rsp)
.L12:
testb $8, %al
je .L13
vpxor %xmm1, %xmm1, %xmm1
vmovdqa %xmm1, 152(%rsp)
.L13:
testb $1, %al
jne .L14
vmovq (%rsi), %xmm1
vpinsrq $1, 16(%rsi), %xmm1, %xmm1
vmovq 8(%rsi), %xmm3
vpinsrq $1, 24(%rsi), %xmm3, %xmm2
vpand %xmm0, %xmm1, %xmm7
vpsrlq $26, %xmm1, %xmm12
vpand %xmm0, %xmm12, %xmm12
vpsllq $12, %xmm2, %xmm3
vpsrlq $52, %xmm1, %xmm1
vpor %xmm3, %xmm1, %xmm6
vpand %xmm0, %xmm6, %xmm3
vpsrlq $26, %xmm6, %xmm6
vpand %xmm0, %xmm6, %xmm6
vpsrlq $40, %xmm2, %xmm2
vpor 152(%rsp), %xmm2, %xmm2
addq $32, %rsi
subq $32, %rdx
orq $1, %rax
movq %rax, 120(%rdi)
jmp .L15
.L14:
vmovdqu (%rdi), %xmm12
vmovdqu 16(%rdi), %xmm6
vmovdqu 32(%rdi), %xmm2
vpshufd $80, %xmm12, %xmm7
vpshufd $250, %xmm12, %xmm12
vpshufd $80, %xmm6, %xmm3
vpshufd $250, %xmm6, %xmm6
vpshufd $80, %xmm2, %xmm2
.L15:
movq 120(%rdi), %rax
testb $48, %al
je .L16
testb $16, %al
je .L17
vmovdqu 40(%rdi), %xmm1
vmovd 56(%rdi), %xmm4
vmovdqu 60(%rdi), %xmm5
vpunpckldq %xmm1, %xmm5, %xmm11
vpunpckhdq %xmm1, %xmm5, %xmm5
vmovd 76(%rdi), %xmm1
vpunpcklqdq %xmm4, %xmm1, %xmm4
jmp .L18
.L17:
movl $(1), %r8d
vmovdqu 40(%rdi), %xmm5
vmovd 56(%rdi), %xmm4
vmovd %r8d, %xmm1
vpunpckldq %xmm1, %xmm5, %xmm11
vpunpckhdq %xmm1, %xmm5, %xmm5
.L18:
vpshufd $80, %xmm11, %xmm1
vpshufd $250, %xmm11, %xmm11
vpshufd $80, %xmm5, %xmm10
vpshufd $250, %xmm5, %xmm5
jmp .L19
.L16:
vmovdqu 60(%rdi), %xmm5
vpshufd $0, %xmm5, %xmm1
vpshufd $85, %xmm5, %xmm11
vpshufd $170, %xmm5, %xmm10
vpshufd $255, %xmm5, %xmm5
vmovd 76(%rdi), %xmm4
vpshufd $0, %xmm4, %xmm4
.L19:
vmovdqa %xmm11, 136(%rsp)
vpmuludq 184(%rsp), %xmm11, %xmm13
vmovdqa %xmm13, 120(%rsp)
vmovdqa %xmm10, 104(%rsp)
vpmuludq 184(%rsp), %xmm10, %xmm13
vmovdqa %xmm13, 88(%rsp)
vmovdqa %xmm5, 72(%rsp)
vpmuludq 184(%rsp), %xmm5, %xmm5
vmovdqa %xmm5, 56(%rsp)
vmovdqa %xmm4, 40(%rsp)
vpmuludq 184(%rsp), %xmm4, %xmm4
vmovdqa %xmm4, 24(%rsp)
cmpq $63, %rdx
jbe .L20
vmovdqu 80(%rdi), %xmm4
vpshufd $0, %xmm4, %xmm5
vmovdqa %xmm5, 8(%rsp)
vpshufd $85, %xmm4, %xmm5
vmovdqa %xmm5, -8(%rsp)
vpshufd $170, %xmm4, %xmm13
vmovdqa %xmm13, -24(%rsp)
vpshufd $255, %xmm4, %xmm4
vmovdqa %xmm4, %xmm10
vmovdqa %xmm4, -40(%rsp)
vmovd 96(%rdi), %xmm4
vpshufd $0, %xmm4, %xmm4
vmovdqa %xmm4, %xmm8
vmovdqa %xmm4, -56(%rsp)
vpmuludq 184(%rsp), %xmm5, %xmm4
vmovdqa %xmm4, -72(%rsp)
vpmuludq 184(%rsp), %xmm13, %xmm4
vmovdqa %xmm4, -88(%rsp)
vpmuludq 184(%rsp), %xmm10, %xmm4
vmovdqa %xmm4, -104(%rsp)
vpmuludq 184(%rsp), %xmm8, %xmm4
vmovdqa %xmm4, -120(%rsp)
leaq 32(%rsi), %rax
movq %rdx, %rcx
vmovdqa %xmm1, 168(%rsp)
jmp .L22
.p2align 6
nop
nop
nop
nop
.L22:
vpmuludq -72(%rsp), %xmm2, %xmm13
vmovdqa -88(%rsp), %xmm5
vpmuludq %xmm5, %xmm6, %xmm4
vpmuludq %xmm5, %xmm2, %xmm11
vmovdqa -104(%rsp), %xmm9
vpmuludq %xmm9, %xmm6, %xmm5
vpmuludq %xmm9, %xmm2, %xmm10
vpaddq %xmm4, %xmm13, %xmm13
vpmuludq %xmm9, %xmm3, %xmm4
vmovdqa -120(%rsp), %xmm8
vpmuludq %xmm8, %xmm2, %xmm9
vpaddq %xmm5, %xmm11, %xmm11
vmovdqa %xmm8, %xmm5
vpmuludq %xmm8, %xmm12, %xmm8
vpmuludq %xmm5, %xmm3, %xmm14
vpaddq %xmm4, %xmm13, %xmm13
vpmuludq %xmm5, %xmm6, %xmm4
vmovdqa 8(%rsp), %xmm15
vpmuludq %xmm15, %xmm6, %xmm5
vpaddq %xmm8, %xmm13, %xmm13
vpmuludq %xmm15, %xmm2, %xmm8
vpaddq %xmm14, %xmm11, %xmm11
vpmuludq %xmm15, %xmm7, %xmm14
vpaddq %xmm4, %xmm10, %xmm10
vpmuludq %xmm15, %xmm12, %xmm4
vpaddq %xmm5, %xmm9, %xmm9
vpmuludq %xmm15, %xmm3, %xmm5
vmovdqa -8(%rsp), %xmm15
vpmuludq %xmm15, %xmm3, %xmm2
vpaddq %xmm14, %xmm13, %xmm13
vpmuludq %xmm15, %xmm6, %xmm6
vpaddq %xmm4, %xmm11, %xmm11
vpmuludq %xmm15, %xmm7, %xmm4
vpaddq %xmm5, %xmm10, %xmm10
vmovq -32(%rax), %xmm5
vpinsrq $1, -16(%rax), %xmm5, %xmm5
vpmuludq %xmm15, %xmm12, %xmm14
vpaddq %xmm2, %xmm9, %xmm9
vmovdqa -24(%rsp), %xmm2
vpmuludq %xmm2, %xmm12, %xmm15
vpaddq %xmm6, %xmm8, %xmm8
vpmuludq %xmm2, %xmm3, %xmm3
vpaddq %xmm4, %xmm11, %xmm11
vmovq -24(%rax), %xmm4
vpinsrq $1, -8(%rax), %xmm4, %xmm6
vpmuludq %xmm2, %xmm7, %xmm4
vpaddq %xmm14, %xmm10, %xmm10
vmovdqa -40(%rsp), %xmm1
vpmuludq %xmm1, %xmm7, %xmm14
vpaddq %xmm15, %xmm9, %xmm9
vpand %xmm5, %xmm0, %xmm2
vpmuludq %xmm1, %xmm12, %xmm12
vpaddq %xmm3, %xmm8, %xmm8
vpsrlq $26, %xmm5, %xmm3
vpand %xmm3, %xmm0, %xmm3
vpmuludq -56(%rsp), %xmm7, %xmm7
vpaddq %xmm4, %xmm10, %xmm10
vpsllq $12, %xmm6, %xmm15
vpsrlq $52, %xmm5, %xmm4
vpor %xmm15, %xmm4, %xmm4
vpaddq %xmm14, %xmm9, %xmm9
vpsrlq $14, %xmm6, %xmm5
vpand %xmm5, %xmm0, %xmm5
vpaddq %xmm12, %xmm8, %xmm8
vpand %xmm4, %xmm0, %xmm4
vpaddq %xmm7, %xmm8, %xmm8
vpsrlq $40, %xmm6, %xmm6
vpor 152(%rsp), %xmm6, %xmm6
vmovdqu (%rax), %xmm12
vmovdqu 16(%rax), %xmm7
vpunpckldq %xmm7, %xmm12, %xmm15
vpunpckhdq %xmm7, %xmm12, %xmm7
vpxor %xmm14, %xmm14, %xmm14
vpunpckldq %xmm14, %xmm15, %xmm12
vpunpckhdq %xmm14, %xmm15, %xmm15
vpunpckldq %xmm14, %xmm7, %xmm14
vpxor %xmm1, %xmm1, %xmm1
vpunpckhdq %xmm1, %xmm7, %xmm7
vpsllq $6, %xmm15, %xmm15
vpsllq $12, %xmm14, %xmm14
vpsllq $18, %xmm7, %xmm7
vpaddq %xmm12, %xmm13, %xmm12
vpaddq %xmm15, %xmm11, %xmm15
vpaddq %xmm14, %xmm10, %xmm14
vpaddq %xmm7, %xmm9, %xmm7
vpaddq 152(%rsp), %xmm8, %xmm8
vpmuludq 120(%rsp), %xmm6, %xmm13
vmovdqa 88(%rsp), %xmm10
vpmuludq %xmm10, %xmm5, %xmm9
vpmuludq %xmm10, %xmm6, %xmm11
vmovdqa 56(%rsp), %xmm1
vpmuludq %xmm1, %xmm5, %xmm10
vpaddq %xmm13, %xmm12, %xmm12
vpmuludq %xmm1, %xmm6, %xmm13
vpaddq %xmm9, %xmm12, %xmm12
vpmuludq %xmm1, %xmm4, %xmm9
vpaddq %xmm11, %xmm15, %xmm15
vmovdqa 24(%rsp), %xmm1
vpmuludq %xmm1, %xmm6, %xmm11
vpaddq %xmm10, %xmm15, %xmm10
vpmuludq %xmm1, %xmm3, %xmm15
vpaddq %xmm13, %xmm14, %xmm14
vpmuludq %xmm1, %xmm4, %xmm13
vpaddq %xmm9, %xmm12, %xmm9
vpmuludq %xmm1, %xmm5, %xmm12
vpaddq %xmm11, %xmm7, %xmm7
vpmuludq 168(%rsp), %xmm5, %xmm11
vpaddq %xmm15, %xmm9, %xmm9
vpmuludq 168(%rsp), %xmm6, %xmm6
vpaddq %xmm13, %xmm10, %xmm10
vpmuludq 168(%rsp), %xmm2, %xmm15
vpaddq %xmm12, %xmm14, %xmm14
vpmuludq 168(%rsp), %xmm3, %xmm13
vpaddq %xmm11, %xmm7, %xmm11
vpmuludq 168(%rsp), %xmm4, %xmm12
vpaddq %xmm6, %xmm8, %xmm6
vmovdqa 136(%rsp), %xmm8
vpmuludq %xmm8, %xmm4, %xmm7
vpaddq %xmm15, %xmm9, %xmm9
vpmuludq %xmm8, %xmm5, %xmm5
vpaddq %xmm13, %xmm10, %xmm10
vpmuludq %xmm8, %xmm2, %xmm15
vpaddq %xmm12, %xmm14, %xmm14
vpmuludq %xmm8, %xmm3, %xmm8
vpaddq %xmm7, %xmm11, %xmm11
vmovdqa 104(%rsp), %xmm7
vpmuludq %xmm7, %xmm3, %xmm13
vpaddq %xmm5, %xmm6, %xmm6
vpmuludq %xmm7, %xmm4, %xmm4
vpaddq %xmm15, %xmm10, %xmm10
vpmuludq %xmm7, %xmm2, %xmm15
vpaddq %xmm8, %xmm14, %xmm14
vmovdqa 72(%rsp), %xmm5
vpmuludq %xmm5, %xmm2, %xmm7
vpaddq %xmm13, %xmm11, %xmm11
vpmuludq %xmm5, %xmm3, %xmm3
vpaddq %xmm4, %xmm6, %xmm6
vpmuludq 40(%rsp), %xmm2, %xmm2
vpaddq %xmm15, %xmm14, %xmm14
vpaddq %xmm7, %xmm11, %xmm11
vpaddq %xmm3, %xmm6, %xmm6
vpaddq %xmm2, %xmm6, %xmm2
vpsrlq $26, %xmm9, %xmm12
vpsrlq $26, %xmm11, %xmm5
vpand %xmm0, %xmm9, %xmm9
vpand %xmm0, %xmm11, %xmm11
vpaddq %xmm12, %xmm10, %xmm10
vpaddq %xmm5, %xmm2, %xmm2
vpsrlq $26, %xmm10, %xmm3
vpsrlq $26, %xmm2, %xmm7
vpand %xmm0, %xmm10, %xmm10
vpand %xmm0, %xmm2, %xmm2
vpaddq %xmm3, %xmm14, %xmm3
vpmuludq 184(%rsp), %xmm7, %xmm7
vpaddq %xmm7, %xmm9, %xmm9
vpsrlq $26, %xmm3, %xmm6
vpsrlq $26, %xmm9, %xmm12
vpand %xmm0, %xmm3, %xmm3
vpand %xmm0, %xmm9, %xmm7
vpaddq %xmm6, %xmm11, %xmm6
vpaddq %xmm12, %xmm10, %xmm12
vpsrlq $26, %xmm6, %xmm8
vpand %xmm0, %xmm6, %xmm6
vpaddq %xmm8, %xmm2, %xmm2
subq $64, %rcx
addq $64, %rax
cmpq $63, %rcx
ja .L22
vmovdqa 168(%rsp), %xmm1
leaq -64(%rdx), %rax
andq $-64, %rax
leaq 64(%rsi,%rax), %rsi
andl $63, %edx
.L20:
cmpq $31, %rdx
jbe .L23
vpmuludq 120(%rsp), %xmm2, %xmm11
vmovdqa 88(%rsp), %xmm4
vpmuludq %xmm4, %xmm6, %xmm0
vpmuludq %xmm4, %xmm2, %xmm10
vmovdqa 56(%rsp), %xmm4
vpmuludq %xmm4, %xmm6, %xmm8
vpmuludq %xmm4, %xmm2, %xmm5
vpaddq %xmm0, %xmm11, %xmm11
vpmuludq %xmm4, %xmm3, %xmm0
vmovdqa 24(%rsp), %xmm13
vpmuludq %xmm13, %xmm2, %xmm4
vpaddq %xmm8, %xmm10, %xmm10
vpmuludq %xmm13, %xmm12, %xmm8
vpmuludq %xmm13, %xmm3, %xmm9
vpaddq %xmm0, %xmm11, %xmm11
vpmuludq %xmm13, %xmm6, %xmm13
vpmuludq %xmm1, %xmm6, %xmm0
vpaddq %xmm8, %xmm11, %xmm8
vpmuludq %xmm1, %xmm2, %xmm2
vpaddq %xmm9, %xmm10, %xmm9
vpmuludq %xmm1, %xmm7, %xmm11
vpaddq %xmm13, %xmm5, %xmm5
vpmuludq %xmm1, %xmm12, %xmm10
vpaddq %xmm0, %xmm4, %xmm0
vpmuludq %xmm1, %xmm3, %xmm1
vmovdqa 136(%rsp), %xmm4
vpmuludq %xmm4, %xmm3, %xmm14
vpaddq %xmm11, %xmm8, %xmm11
vpmuludq %xmm4, %xmm6, %xmm6
vpaddq %xmm10, %xmm9, %xmm9
vpmuludq %xmm4, %xmm7, %xmm15
vpaddq %xmm1, %xmm5, %xmm5
vpmuludq %xmm4, %xmm12, %xmm1
vpaddq %xmm14, %xmm0, %xmm0
vmovdqa 104(%rsp), %xmm4
vpmuludq %xmm4, %xmm12, %xmm8
vpaddq %xmm6, %xmm2, %xmm2
vpmuludq %xmm4, %xmm3, %xmm3
vpaddq %xmm15, %xmm9, %xmm9
vpmuludq %xmm4, %xmm7, %xmm10
vpaddq %xmm1, %xmm5, %xmm1
vmovdqa 72(%rsp), %xmm4
vpmuludq %xmm4, %xmm7, %xmm15
vpaddq %xmm8, %xmm0, %xmm0
vpmuludq %xmm4, %xmm12, %xmm12
vpaddq %xmm3, %xmm2, %xmm2
vpmuludq 40(%rsp), %xmm7, %xmm7
vpaddq %xmm10, %xmm1, %xmm1
vpaddq %xmm15, %xmm0, %xmm0
vpaddq %xmm12, %xmm2, %xmm2
vpaddq %xmm7, %xmm2, %xmm2
movl $((1 << 26) - 1), %r8d
testq %rsi, %rsi
vmovd %r8d, %xmm15
je .L24
vmovdqu (%rsi), %xmm4
vmovdqu 16(%rsi), %xmm3
vpunpckldq %xmm3, %xmm4, %xmm5
vpunpckhdq %xmm3, %xmm4, %xmm3
vpxor %xmm4, %xmm4, %xmm4
vpunpckldq %xmm4, %xmm5, %xmm7
vpunpckhdq %xmm4, %xmm5, %xmm5
vpunpckldq %xmm4, %xmm3, %xmm6
vpunpckhdq %xmm4, %xmm3, %xmm3
vpsllq $6, %xmm5, %xmm5
vpsllq $12, %xmm6, %xmm6
vpsllq $18, %xmm3, %xmm3
vpaddq %xmm7, %xmm11, %xmm11
vpaddq %xmm5, %xmm9, %xmm9
vpaddq %xmm6, %xmm1, %xmm1
vpaddq %xmm3, %xmm0, %xmm0
vpaddq 152(%rsp), %xmm2, %xmm2
.L24:
vpshufd $68, %xmm15, %xmm15
vpsrlq $26, %xmm11, %xmm12
vpsrlq $26, %xmm0, %xmm3
vpand %xmm15, %xmm11, %xmm11
vpand %xmm15, %xmm0, %xmm6
vpaddq %xmm12, %xmm9, %xmm9
vpaddq %xmm3, %xmm2, %xmm2
vpsrlq $26, %xmm9, %xmm3
vpsrlq $26, %xmm2, %xmm7
vpand %xmm15, %xmm9, %xmm9
vpand %xmm15, %xmm2, %xmm2
vpaddq %xmm3, %xmm1, %xmm3
vpmuludq 184(%rsp), %xmm7, %xmm7
vpaddq %xmm7, %xmm11, %xmm7
vpsrlq $26, %xmm3, %xmm4
vpsrlq $26, %xmm7, %xmm1
vpand %xmm15, %xmm3, %xmm3
vpand %xmm15, %xmm7, %xmm7
vpaddq %xmm4, %xmm6, %xmm6
vpaddq %xmm1, %xmm9, %xmm12
vpsrlq $26, %xmm6, %xmm0
vpand %xmm15, %xmm6, %xmm6
vpaddq %xmm0, %xmm2, %xmm2
.L23:
testq %rsi, %rsi
je .L25
vpshufd $8, %xmm7, %xmm7
vpshufd $8, %xmm12, %xmm12
vpshufd $8, %xmm3, %xmm3
vpshufd $8, %xmm6, %xmm6
vpshufd $8, %xmm2, %xmm2
vpunpcklqdq %xmm12, %xmm7, %xmm7
vpunpcklqdq %xmm6, %xmm3, %xmm3
vmovdqu %xmm7, (%rdi)
vmovdqu %xmm3, 16(%rdi)
vmovq %xmm2, 32(%rdi)
jmp .L11
.L25:
vpsrldq $8, %xmm7, %xmm0
vpaddq %xmm0, %xmm7, %xmm7
vpsrldq $8, %xmm12, %xmm0
vpaddq %xmm0, %xmm12, %xmm12
vpsrldq $8, %xmm3, %xmm0
vpaddq %xmm0, %xmm3, %xmm3
vpsrldq $8, %xmm6, %xmm0
vpaddq %xmm0, %xmm6, %xmm6
vpsrldq $8, %xmm2, %xmm0
vpaddq %xmm0, %xmm2, %xmm2
vmovd %xmm7, %eax
vmovd %xmm12, %edx
movl %eax, %r9d
shrl $26, %r9d
addl %edx, %r9d
movl %r9d, %r8d
andl $67108863, %r8d
vmovd %xmm3, %edx
shrl $26, %r9d
addl %edx, %r9d
vmovd %xmm6, %edx
movl %r9d, %ecx
shrl $26, %ecx
addl %edx, %ecx
movl %ecx, %esi
andl $67108863, %esi
vmovd %xmm2, %r10d
movl %r8d, %r11d
salq $26, %r11
andl $67108863, %eax
orq %rax, %r11
movabsq $17592186044415, %rax
andq %rax, %r11
andl $67108863, %r9d
salq $8, %r9
shrl $18, %r8d
movl %r8d, %r8d
orq %r8, %r9
movq %rsi, %rdx
salq $34, %rdx
orq %rdx, %r9
andq %rax, %r9
shrl $26, %ecx
addl %r10d, %ecx
salq $16, %rcx
shrl $10, %esi
movl %esi, %esi
orq %rsi, %rcx
movabsq $4398046511103, %r10
movq %rcx, %r8
andq %r10, %r8
shrq $42, %rcx
leaq (%rcx,%rcx,4), %rdx
addq %r11, %rdx
movq %rdx, %rsi
andq %rax, %rsi
shrq $44, %rdx
addq %r9, %rdx
movq %rdx, %rcx
andq %rax, %rcx
shrq $44, %rdx
addq %r8, %rdx
andq %rdx, %r10
shrq $42, %rdx
leaq (%rsi,%rdx,4), %rsi
leaq (%rsi,%rdx), %r11
movq %r11, %rbx
andq %rax, %rbx
shrq $44, %r11
addq %rcx, %r11
leaq 5(%rbx), %r9
movq %r9, %r8
shrq $44, %r8
addq %r11, %r8
movabsq $-4398046511104, %rsi
addq %r10, %rsi
movq %r8, %rdx
shrq $44, %rdx
addq %rdx, %rsi
movq %rsi, %rdx
shrq $63, %rdx
subq $1, %rdx
movq %rdx, %rcx
notq %rcx
andq %rcx, %rbx
andq %rcx, %r11
andq %r10, %rcx
andq %rax, %r9
andq %rdx, %r9
orq %r9, %rbx
movq %rbx, (%rdi)
andq %r8, %rax
andq %rdx, %rax
orq %rax, %r11
movq %r11, 8(%rdi)
andq %rsi, %rdx
orq %rcx, %rdx
movq %rdx, 16(%rdi)
.L11:
movq -8(%rbp), %rbx
leave
ret
FN_END poly1305_blocks_avx

GLOBAL_HIDDEN_FN poly1305_finish_ext_avx
poly1305_finish_ext_avx_local:
pushq %r12
pushq %rbp
pushq %rbx
subq $32, %rsp
movq %rdi, %rbx
movq %rdx, %rbp
movq %rcx, %r12
testq %rdx, %rdx
je .L30
movq $0, (%rsp)
movq $0, 8(%rsp)
movq $0, 16(%rsp)
movq $0, 24(%rsp)
movq %rsp, %rax
subq %rsp, %rsi
testb $16, %dl
je .L31
vmovdqu (%rsp,%rsi), %xmm0
vmovdqa %xmm0, (%rsp)
addq $16, %rax
.L31:
testb $8, %bpl
je .L32
movq (%rax,%rsi), %rdx
movq %rdx, (%rax)
addq $8, %rax
.L32:
testb $4, %bpl
je .L33
movl (%rax,%rsi), %edx
movl %edx, (%rax)
addq $4, %rax
.L33:
testb $2, %bpl
je .L34
movzwl (%rax,%rsi), %edx
movw %dx, (%rax)
addq $2, %rax
.L34:
testb $1, %bpl
je .L35
movzbl (%rax,%rsi), %edx
movb %dl, (%rax)
.L35:
cmpq $16, %rbp
je .L36
movb $1, (%rsp,%rbp)
movq 120(%rbx), %rdx
cmpq $16, %rbp
sbbq %rax, %rax
andl $4, %eax
addq $4, %rax
.L37:
orq %rdx, %rax
movq %rax, 120(%rbx)
movq %rsp, %rsi
movl $32, %edx
movq %rbx, %rdi
call poly1305_blocks_avx_local
.L30:
movq 120(%rbx), %rax
testb $1, %al
je .L38
subq $1, %rbp
cmpq $15, %rbp
jbe .L39
orq $16, %rax
movq %rax, 120(%rbx)
jmp .L40
.L39:
orq $32, %rax
movq %rax, 120(%rbx)
.L40:
movl $32, %edx
movl $0, %esi
movq %rbx, %rdi
call poly1305_blocks_avx_local
.L38:
movq 8(%rbx), %rax
movq %rax, %rdx
salq $44, %rdx
orq (%rbx), %rdx
shrq $20, %rax
movq 16(%rbx), %rcx
salq $24, %rcx
orq %rcx, %rax
movq 104(%rbx), %rcx
movq 112(%rbx), %rsi
addq %rcx, %rdx
adcq %rsi, %rax
vpxor %xmm0, %xmm0, %xmm0
vmovdqu %xmm0, (%rbx)
vmovdqu %xmm0, 16(%rbx)
vmovdqu %xmm0, 32(%rbx)
vmovdqu %xmm0, 48(%rbx)
vmovdqu %xmm0, 64(%rbx)
vmovdqu %xmm0, 80(%rbx)
vmovdqu %xmm0, 96(%rbx)
vmovdqu %xmm0, 112(%rbx)
movq %rdx, (%r12)
movq %rax, 8(%r12)
jmp .L43
.L36:
movq 120(%rbx), %rdx
movl $4, %eax
jmp .L37
.L43:
addq $32, %rsp
popq %rbx
popq %rbp
popq %r12
ret
FN_END poly1305_finish_ext_avx

GLOBAL_HIDDEN_FN poly1305_auth_avx
pushq %rbp
movq %rsp, %rbp
pushq %r14
pushq %r13
pushq %r12
pushq %rbx
andq $-64, %rsp
addq $-128, %rsp
movq %rdi, %r14
movq %rsi, %r12
movq %rdx, %rbx
movq %rsp, %rdi
movq %rcx, %rsi
call poly1305_init_ext_avx_local
movq %rbx, %r13
andq $-32, %r13
je .L46
movq %rsp, %rdi
movq %r13, %rdx
movq %r12, %rsi
call poly1305_blocks_avx_local
addq %r13, %r12
subq %r13, %rbx
.L46:
movq %rsp, %rdi
movq %r14, %rcx
movq %rbx, %rdx
movq %r12, %rsi
call poly1305_finish_ext_avx_local
leaq -32(%rbp), %rsp
popq %rbx
popq %r12
popq %r13
popq %r14
popq %rbp
ret
FN_END poly1305_auth_avx
