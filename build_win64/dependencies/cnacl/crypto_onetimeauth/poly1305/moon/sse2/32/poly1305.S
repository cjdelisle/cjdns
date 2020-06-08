#include "x86.inc"

SECTION_TEXT

GLOBAL_HIDDEN_FN poly1305_block_size_sse2
movl $32, %eax
ret
FN_END poly1305_block_size_sse2

GLOBAL_HIDDEN_FN poly1305_auth_sse2
poly1305_auth_sse2_local:
pushl %ebp
movl %esp, %ebp
andl $-64, %esp
pushl %esi
pushl %edi
pushl %ebx
subl $244, %esp
movl 16(%ebp), %esi
lea 64(%esp), %eax
movl %esi, %ecx
movl 20(%ebp), %edx
movl 12(%ebp), %edi
call poly1305_init_ext_sse2_local
poly1305_auth_sse2_2:
movl %esi, %ebx
andl $-32, %ebx
je poly1305_auth_sse2_5
poly1305_auth_sse2_3:
movl %edi, %edx
lea 64(%esp), %eax
movl %ebx, %ecx
call poly1305_blocks_sse2_local
poly1305_auth_sse2_4:
addl %ebx, %edi
subl %ebx, %esi
poly1305_auth_sse2_5:
pushl 8(%ebp)
pushl %esi
pushl %edi
lea 76(%esp), %eax
pushl %eax
call poly1305_finish_ext_sse2_local
poly1305_auth_sse2_6:
addl $260, %esp
popl %ebx
popl %edi
popl %esi
movl %ebp, %esp
popl %ebp
ret
FN_END poly1305_auth_sse2

GLOBAL_HIDDEN_FN poly1305_finish_ext_sse2
poly1305_finish_ext_sse2_local:
pushl %esi
pushl %edi
pushl %ebx
pushl %ebp
subl $60, %esp
movl 88(%esp), %ebp
testl %ebp, %ebp
movl 80(%esp), %ebx
je poly1305_finish_ext_sse2_18
poly1305_finish_ext_sse2_2:
pxor %xmm0, %xmm0
movaps %xmm0, 16(%esp)
movaps %xmm0, 32(%esp)
poly1305_finish_ext_sse2_3:
movl 84(%esp), %ecx
lea 16(%esp), %edx
subl %edx, %ecx
testl $16, %ebp
je poly1305_finish_ext_sse2_5
poly1305_finish_ext_sse2_4:
lea 32(%esp), %edx
movdqu 16(%esp,%ecx), %xmm0
movdqa %xmm0, 16(%esp)
poly1305_finish_ext_sse2_5:
testl $8, %ebp
je poly1305_finish_ext_sse2_7
poly1305_finish_ext_sse2_6:
movl (%edx,%ecx), %esi
movl 4(%edx,%ecx), %edi
movl %esi, (%edx)
movl %edi, 4(%edx)
addl $8, %edx
poly1305_finish_ext_sse2_7:
testl $4, %ebp
je poly1305_finish_ext_sse2_9
poly1305_finish_ext_sse2_8:
movl (%edx,%ecx), %esi
movl %esi, (%edx)
addl $4, %edx
poly1305_finish_ext_sse2_9:
testl $2, %ebp
je poly1305_finish_ext_sse2_11
poly1305_finish_ext_sse2_10:
movzwl (%edx,%ecx), %esi
movw %si, (%edx)
addl $2, %edx
poly1305_finish_ext_sse2_11:
testl $1, %ebp
je poly1305_finish_ext_sse2_13
poly1305_finish_ext_sse2_12:
movzbl (%edx,%ecx), %ecx
movb %cl, (%edx)
poly1305_finish_ext_sse2_13:
cmpl $16, %ebp
je poly1305_finish_ext_sse2_16
poly1305_finish_ext_sse2_14:
movb $1, 16(%esp,%ebp)
jae poly1305_finish_ext_sse2_16
poly1305_finish_ext_sse2_15:
movl $8, %edx
jmp poly1305_finish_ext_sse2_17
poly1305_finish_ext_sse2_16:
movl $4, %edx
poly1305_finish_ext_sse2_17:
orl %edx, 116(%ebx)
movl %ebx, %eax
movl $32, %ecx
lea 16(%esp), %edx
call poly1305_blocks_sse2_local
poly1305_finish_ext_sse2_18:
movl 116(%ebx), %edx
testb $1, %dl
je poly1305_finish_ext_sse2_24
poly1305_finish_ext_sse2_19:
testl %ebp, %ebp
je poly1305_finish_ext_sse2_21
poly1305_finish_ext_sse2_20:
cmpl $16, %ebp
jbe poly1305_finish_ext_sse2_22
poly1305_finish_ext_sse2_21:
orl $16, %edx
movl %edx, 116(%ebx)
jmp poly1305_finish_ext_sse2_23
poly1305_finish_ext_sse2_22:
orl $32, %edx
movl %edx, 116(%ebx)
poly1305_finish_ext_sse2_23:
movl %ebx, %eax
xorl %edx, %edx
movl $32, %ecx
call poly1305_blocks_sse2_local
poly1305_finish_ext_sse2_24:
movl 8(%ebx), %edx
movl %edx, %eax
movl 4(%ebx), %ecx
movl %ecx, %esi
shrl $6, %ecx
shll $20, %eax
pxor %xmm0, %xmm0
movl 12(%ebx), %ebp
orl %eax, %ecx
movl %ebp, %eax
shrl $12, %edx
shll $14, %eax
orl %eax, %edx
movl 16(%ebx), %eax
shll $26, %esi
shrl $18, %ebp
shll $8, %eax
movl 92(%esp), %edi
orl %eax, %ebp
orl (%ebx), %esi
addl 100(%ebx), %esi
adcl 104(%ebx), %ecx
adcl 108(%ebx), %edx
adcl 112(%ebx), %ebp
movdqu %xmm0, (%ebx)
movdqu %xmm0, 16(%ebx)
movdqu %xmm0, 32(%ebx)
movdqu %xmm0, 48(%ebx)
movdqu %xmm0, 64(%ebx)
movdqu %xmm0, 80(%ebx)
movdqu %xmm0, 96(%ebx)
movdqu %xmm0, 112(%ebx)
movl %esi, (%edi)
movl %ecx, 4(%edi)
movl %edx, 8(%edi)
movl %ebp, 12(%edi)
addl $60, %esp
popl %ebp
popl %ebx
popl %edi
popl %esi
ret
FN_END poly1305_finish_ext_sse2


GLOBAL_HIDDEN_FN poly1305_blocks_sse2
movl 4(%esp), %eax
movl 8(%esp), %edx
movl 12(%esp), %ecx
poly1305_blocks_sse2_local:
pushl %esi
pushl %edi
pushl %ebx
subl $544, %esp
movl $16777216, %ebx
movl $67108863, %esi
movl $5, %edi
movd %ebx, %xmm0
movd %esi, %xmm2
movd %edi, %xmm4
movl 116(%eax), %ebx
testb $4, %bl
pshufd $68, %xmm0, %xmm1
pshufd $68, %xmm2, %xmm3
pshufd $68, %xmm4, %xmm5
movdqa %xmm1, 272(%esp)
movdqa %xmm3, 256(%esp)
movdqa %xmm5, 160(%esp)
je poly1305_blocks_sse2_3
poly1305_blocks_sse2_2:
movdqa 272(%esp), %xmm0
psrldq $8, %xmm0
movdqa %xmm0, 272(%esp)
poly1305_blocks_sse2_3:
testb $8, %bl
je poly1305_blocks_sse2_5
poly1305_blocks_sse2_4:
pxor %xmm0, %xmm0
movdqa %xmm0, 272(%esp)
poly1305_blocks_sse2_5:
testb $1, %bl
jne poly1305_blocks_sse2_7
poly1305_blocks_sse2_6:
movq 8(%edx), %xmm0
orl $1, %ebx
movq (%edx), %xmm1
addl $-32, %ecx
movhpd 24(%edx), %xmm0
movdqa 256(%esp), %xmm4
movaps %xmm0, %xmm2
movhpd 16(%edx), %xmm1
movdqa %xmm4, %xmm7
pand %xmm1, %xmm7
movaps %xmm1, %xmm6
psrlq $52, %xmm1
psllq $12, %xmm2
por %xmm2, %xmm1
movdqa %xmm4, %xmm3
psrlq $26, %xmm6
pand %xmm1, %xmm3
psrlq $26, %xmm1
psrlq $40, %xmm0
movdqa %xmm3, 32(%esp)
pand %xmm4, %xmm6
por 272(%esp), %xmm0
pand %xmm4, %xmm1
movl %ebx, 116(%eax)
addl $32, %edx
jmp poly1305_blocks_sse2_8
poly1305_blocks_sse2_7:
movdqu 16(%eax), %xmm0
movdqu (%eax), %xmm6
movdqu 32(%eax), %xmm2
pshufd $80, %xmm0, %xmm1
pshufd $80, %xmm6, %xmm7
pshufd $250, %xmm6, %xmm6
movdqa %xmm1, 32(%esp)
pshufd $250, %xmm0, %xmm1
pshufd $80, %xmm2, %xmm0
poly1305_blocks_sse2_8:
testb $48, %bl
je poly1305_blocks_sse2_13
poly1305_blocks_sse2_9:
movdqu 40(%eax), %xmm2
movl 56(%eax), %esi
testb $16, %bl
je poly1305_blocks_sse2_11
poly1305_blocks_sse2_10:
movdqu 60(%eax), %xmm4
movdqa %xmm4, %xmm5
movd 76(%eax), %xmm3
punpckldq %xmm2, %xmm5
punpckhdq %xmm2, %xmm4
movd %esi, %xmm2
punpcklqdq %xmm2, %xmm3
movdqa %xmm3, 240(%esp)
jmp poly1305_blocks_sse2_12
poly1305_blocks_sse2_11:
movl $1, %ebx
movdqa %xmm2, %xmm5
movdqa %xmm2, %xmm4
movd %esi, %xmm2
movdqa %xmm2, 240(%esp)
movd %ebx, %xmm3
punpckldq %xmm3, %xmm5
punpckhdq %xmm3, %xmm4
poly1305_blocks_sse2_12:
pshufd $80, %xmm5, %xmm2
pshufd $250, %xmm5, %xmm3
pshufd $80, %xmm4, %xmm5
pshufd $250, %xmm4, %xmm4
movdqa %xmm2, 176(%esp)
movdqa %xmm3, 224(%esp)
movdqa %xmm5, 208(%esp)
movdqa %xmm4, 192(%esp)
jmp poly1305_blocks_sse2_14
poly1305_blocks_sse2_13:
movdqu 60(%eax), %xmm3
movd 76(%eax), %xmm2
pshufd $0, %xmm3, %xmm4
movdqa %xmm4, 176(%esp)
pshufd $85, %xmm3, %xmm5
pshufd $170, %xmm3, %xmm4
pshufd $255, %xmm3, %xmm3
pshufd $0, %xmm2, %xmm2
movdqa %xmm5, 224(%esp)
movdqa %xmm4, 208(%esp)
movdqa %xmm3, 192(%esp)
movdqa %xmm2, 240(%esp)
poly1305_blocks_sse2_14:
movdqa 160(%esp), %xmm2
cmpl $64, %ecx
movdqa 192(%esp), %xmm5
pmuludq %xmm2, %xmm5
movdqa %xmm5, 320(%esp)
movdqa 224(%esp), %xmm4
movdqa 208(%esp), %xmm3
movdqa 240(%esp), %xmm5
pmuludq %xmm2, %xmm4
pmuludq %xmm2, %xmm3
pmuludq %xmm2, %xmm5
movdqa 320(%esp), %xmm2
jb poly1305_blocks_sse2_18
poly1305_blocks_sse2_15:
movdqa %xmm3, 304(%esp)
movdqu 80(%eax), %xmm3
movaps %xmm0, 400(%esp)
movd 96(%eax), %xmm0
movdqa %xmm2, 320(%esp)
pshufd $85, %xmm3, %xmm2
movdqa %xmm2, 128(%esp)
pshufd $0, %xmm0, %xmm2
movdqa 160(%esp), %xmm0
movdqa %xmm5, 336(%esp)
pshufd $0, %xmm3, %xmm5
movdqa %xmm5, (%esp)
movdqa %xmm0, %xmm5
pmuludq 128(%esp), %xmm5
movdqa %xmm4, 288(%esp)
pshufd $170, %xmm3, %xmm4
movdqa %xmm5, 80(%esp)
movdqa %xmm0, %xmm5
movdqa %xmm4, 112(%esp)
pshufd $255, %xmm3, %xmm3
pmuludq %xmm4, %xmm5
movdqa %xmm0, %xmm4
pmuludq %xmm3, %xmm4
pmuludq %xmm2, %xmm0
movdqa %xmm3, 144(%esp)
movdqa %xmm2, 96(%esp)
movdqa %xmm5, 64(%esp)
movdqa %xmm4, 48(%esp)
movdqa %xmm0, 16(%esp)
movaps 400(%esp), %xmm0
movaps %xmm1, 384(%esp)
movdqa %xmm6, 368(%esp)
movdqa %xmm7, 352(%esp)
poly1305_blocks_sse2_16:
movq 8(%edx), %xmm6
addl $-64, %ecx
movq (%edx), %xmm2
movhpd 24(%edx), %xmm6
movdqa 256(%esp), %xmm1
movaps %xmm6, %xmm7
movhpd 16(%edx), %xmm2
movdqa %xmm1, %xmm4
pand %xmm2, %xmm4
movaps %xmm2, %xmm3
movaps %xmm6, %xmm5
psrlq $52, %xmm2
psllq $12, %xmm7
psrlq $26, %xmm3
psrlq $14, %xmm5
por %xmm7, %xmm2
pand %xmm1, %xmm3
pand %xmm1, %xmm5
pand %xmm1, %xmm2
psrlq $40, %xmm6
movdqu 32(%edx), %xmm1
movdqu 48(%edx), %xmm7
movdqa %xmm4, 416(%esp)
movdqa %xmm1, %xmm4
punpckldq %xmm7, %xmm4
addl $64, %edx
punpckhdq %xmm7, %xmm1
cmpl $64, %ecx
movaps 384(%esp), %xmm7
movaps %xmm0, 400(%esp)
pmuludq 80(%esp), %xmm0
pmuludq 64(%esp), %xmm7
movdqa %xmm1, 480(%esp)
movdqa 32(%esp), %xmm1
pmuludq 48(%esp), %xmm1
paddq %xmm7, %xmm0
movdqa 368(%esp), %xmm7
pmuludq 16(%esp), %xmm7
paddq %xmm1, %xmm0
movdqa 352(%esp), %xmm1
paddq %xmm7, %xmm0
movdqa (%esp), %xmm7
pmuludq %xmm7, %xmm1
por 272(%esp), %xmm6
paddq %xmm1, %xmm0
movdqa 288(%esp), %xmm1
pmuludq %xmm6, %xmm1
paddq %xmm1, %xmm0
movdqa 304(%esp), %xmm1
pmuludq %xmm5, %xmm1
paddq %xmm1, %xmm0
movdqa 320(%esp), %xmm1
pmuludq %xmm2, %xmm1
movaps %xmm2, 448(%esp)
movdqa 336(%esp), %xmm2
paddq %xmm1, %xmm0
movdqa %xmm2, %xmm1
pmuludq %xmm3, %xmm1
paddq %xmm1, %xmm0
movdqa 176(%esp), %xmm1
movdqa %xmm3, 432(%esp)
movdqa %xmm1, %xmm3
pmuludq 416(%esp), %xmm3
pmuludq %xmm5, %xmm1
paddq %xmm3, %xmm0
pxor %xmm3, %xmm3
movdqa %xmm4, 464(%esp)
punpckldq %xmm3, %xmm4
paddq %xmm4, %xmm0
movdqa %xmm0, 496(%esp)
movaps 400(%esp), %xmm4
movaps 384(%esp), %xmm0
pmuludq 16(%esp), %xmm4
pmuludq %xmm7, %xmm0
movdqa 32(%esp), %xmm7
pmuludq 128(%esp), %xmm7
paddq %xmm0, %xmm4
movdqa 368(%esp), %xmm0
pmuludq 112(%esp), %xmm0
paddq %xmm7, %xmm4
movdqa 352(%esp), %xmm7
pmuludq 144(%esp), %xmm7
paddq %xmm0, %xmm4
movdqa %xmm2, %xmm0
pmuludq %xmm6, %xmm0
paddq %xmm7, %xmm4
movdqa 224(%esp), %xmm7
paddq %xmm0, %xmm4
movaps 448(%esp), %xmm0
pmuludq %xmm0, %xmm7
pmuludq %xmm0, %xmm2
paddq %xmm1, %xmm4
movdqa 208(%esp), %xmm1
pmuludq 432(%esp), %xmm1
paddq %xmm7, %xmm4
movdqa 192(%esp), %xmm7
paddq %xmm1, %xmm4
movdqa 416(%esp), %xmm1
pmuludq %xmm1, %xmm7
paddq %xmm7, %xmm4
movdqa 480(%esp), %xmm7
punpckhdq %xmm3, %xmm7
psllq $18, %xmm7
paddq %xmm7, %xmm4
movdqa %xmm4, 512(%esp)
movaps 400(%esp), %xmm4
movaps 384(%esp), %xmm3
pmuludq 64(%esp), %xmm4
pmuludq 48(%esp), %xmm3
movdqa 32(%esp), %xmm7
pmuludq 16(%esp), %xmm7
paddq %xmm3, %xmm4
movdqa 368(%esp), %xmm3
pmuludq (%esp), %xmm3
paddq %xmm7, %xmm4
movdqa 352(%esp), %xmm7
paddq %xmm3, %xmm4
movdqa 128(%esp), %xmm3
pmuludq %xmm3, %xmm7
paddq %xmm7, %xmm4
movdqa 304(%esp), %xmm7
pmuludq %xmm6, %xmm7
paddq %xmm7, %xmm4
movdqa 320(%esp), %xmm7
pmuludq %xmm5, %xmm7
paddq %xmm7, %xmm4
movdqa 176(%esp), %xmm7
movdqa %xmm7, %xmm0
pmuludq 432(%esp), %xmm0
paddq %xmm2, %xmm4
movdqa 224(%esp), %xmm2
paddq %xmm0, %xmm4
movdqa %xmm2, %xmm0
pmuludq %xmm1, %xmm0
pmuludq %xmm5, %xmm2
pmuludq 336(%esp), %xmm5
paddq %xmm0, %xmm4
movdqa 464(%esp), %xmm0
pxor %xmm1, %xmm1
punpckhdq %xmm1, %xmm0
psllq $6, %xmm0
movdqa 496(%esp), %xmm1
paddq %xmm0, %xmm4
psrlq $26, %xmm1
paddq %xmm1, %xmm4
movdqa %xmm4, 528(%esp)
movaps 400(%esp), %xmm4
movaps 384(%esp), %xmm1
movaps %xmm4, %xmm0
pmuludq (%esp), %xmm0
pmuludq %xmm3, %xmm1
pmuludq 48(%esp), %xmm4
paddq %xmm1, %xmm0
movdqa 32(%esp), %xmm3
pmuludq 112(%esp), %xmm3
paddq %xmm3, %xmm0
movdqa 368(%esp), %xmm3
movdqa %xmm3, %xmm1
pmuludq 144(%esp), %xmm1
pmuludq 128(%esp), %xmm3
paddq %xmm1, %xmm0
movdqa 352(%esp), %xmm1
pmuludq 96(%esp), %xmm1
paddq %xmm1, %xmm0
movdqa %xmm7, %xmm1
pmuludq %xmm6, %xmm1
pmuludq 320(%esp), %xmm6
paddq %xmm1, %xmm0
movdqa 208(%esp), %xmm1
pmuludq 448(%esp), %xmm1
paddq %xmm2, %xmm0
movdqa 192(%esp), %xmm2
pmuludq 432(%esp), %xmm2
paddq %xmm1, %xmm0
movdqa 416(%esp), %xmm1
paddq %xmm2, %xmm0
movdqa 240(%esp), %xmm2
pmuludq %xmm1, %xmm2
pmuludq 208(%esp), %xmm1
paddq %xmm2, %xmm0
movdqa 512(%esp), %xmm2
paddq 272(%esp), %xmm0
psrlq $26, %xmm2
paddq %xmm2, %xmm0
movaps 384(%esp), %xmm2
pmuludq 16(%esp), %xmm2
paddq %xmm2, %xmm4
movdqa 32(%esp), %xmm2
pmuludq (%esp), %xmm2
paddq %xmm2, %xmm4
paddq %xmm3, %xmm4
movdqa 352(%esp), %xmm3
pmuludq 112(%esp), %xmm3
paddq %xmm3, %xmm4
paddq %xmm6, %xmm4
movaps 448(%esp), %xmm6
movdqa %xmm0, %xmm3
pmuludq %xmm7, %xmm6
psrlq $26, %xmm3
pmuludq 160(%esp), %xmm3
paddq %xmm5, %xmm4
movdqa 432(%esp), %xmm5
pmuludq 224(%esp), %xmm5
paddq %xmm6, %xmm4
paddq %xmm5, %xmm4
movdqa 480(%esp), %xmm7
pxor %xmm5, %xmm5
punpckldq %xmm5, %xmm7
paddq %xmm1, %xmm4
movdqa 528(%esp), %xmm2
psllq $12, %xmm7
movdqa %xmm2, %xmm1
paddq %xmm7, %xmm4
psrlq $26, %xmm1
paddq %xmm1, %xmm4
movdqa 256(%esp), %xmm5
movaps %xmm4, %xmm1
movdqa 496(%esp), %xmm6
psrlq $26, %xmm1
movdqa 512(%esp), %xmm7
pand %xmm5, %xmm6
pand %xmm5, %xmm7
pand %xmm5, %xmm2
paddq %xmm3, %xmm6
paddq %xmm1, %xmm7
movdqa %xmm5, %xmm3
movdqa %xmm5, %xmm1
pand %xmm6, %xmm3
psrlq $26, %xmm6
pand %xmm4, %xmm1
movdqa %xmm5, %xmm4
paddq %xmm6, %xmm2
pand %xmm7, %xmm4
pand %xmm5, %xmm0
psrlq $26, %xmm7
movdqa %xmm3, 352(%esp)
movdqa %xmm2, 368(%esp)
movdqa %xmm1, 32(%esp)
movaps %xmm4, 384(%esp)
paddq %xmm7, %xmm0
jae poly1305_blocks_sse2_16
poly1305_blocks_sse2_17:
movdqa 336(%esp), %xmm5
movdqa 320(%esp), %xmm2
movdqa 304(%esp), %xmm3
movdqa 288(%esp), %xmm4
movaps 384(%esp), %xmm1
movdqa 368(%esp), %xmm6
movdqa 352(%esp), %xmm7
poly1305_blocks_sse2_18:
cmpl $32, %ecx
jb poly1305_blocks_sse2_22
poly1305_blocks_sse2_19:
movaps %xmm1, 384(%esp)
testl %edx, %edx
pmuludq %xmm0, %xmm4
pmuludq %xmm3, %xmm1
pmuludq %xmm0, %xmm3
paddq %xmm1, %xmm4
movdqa 32(%esp), %xmm1
pmuludq %xmm2, %xmm1
paddq %xmm1, %xmm4
movdqa %xmm6, %xmm1
pmuludq %xmm5, %xmm1
paddq %xmm1, %xmm4
movdqa 176(%esp), %xmm1
movdqa %xmm7, 352(%esp)
pmuludq %xmm1, %xmm7
paddq %xmm7, %xmm4
movdqa %xmm4, 288(%esp)
movaps 384(%esp), %xmm4
movaps %xmm4, %xmm7
pmuludq %xmm2, %xmm7
pmuludq %xmm0, %xmm2
paddq %xmm7, %xmm3
movdqa 32(%esp), %xmm7
pmuludq %xmm5, %xmm7
movdqa %xmm6, 368(%esp)
pmuludq %xmm1, %xmm6
paddq %xmm7, %xmm3
paddq %xmm6, %xmm3
movdqa 352(%esp), %xmm7
movdqa 224(%esp), %xmm6
pmuludq %xmm6, %xmm7
paddq %xmm7, %xmm3
movdqa %xmm3, 304(%esp)
movaps %xmm4, %xmm3
pmuludq %xmm5, %xmm3
pmuludq %xmm0, %xmm5
pmuludq %xmm1, %xmm0
paddq %xmm3, %xmm2
movdqa 32(%esp), %xmm3
movdqa %xmm3, %xmm7
pmuludq %xmm1, %xmm7
paddq %xmm7, %xmm2
movdqa 368(%esp), %xmm7
pmuludq %xmm6, %xmm7
movdqa 352(%esp), %xmm6
paddq %xmm7, %xmm2
movdqa 208(%esp), %xmm7
pmuludq %xmm7, %xmm6
paddq %xmm6, %xmm2
movdqa %xmm2, 320(%esp)
movaps %xmm4, %xmm2
pmuludq %xmm1, %xmm2
paddq %xmm2, %xmm5
movdqa 224(%esp), %xmm6
movdqa %xmm3, %xmm2
pmuludq %xmm6, %xmm2
pmuludq %xmm6, %xmm4
pmuludq 208(%esp), %xmm3
paddq %xmm2, %xmm5
paddq %xmm4, %xmm0
movdqa 368(%esp), %xmm2
pmuludq %xmm7, %xmm2
paddq %xmm3, %xmm0
paddq %xmm2, %xmm5
movdqa 192(%esp), %xmm2
movdqa 368(%esp), %xmm4
pmuludq %xmm2, %xmm4
movdqa 352(%esp), %xmm7
movdqa 352(%esp), %xmm1
pmuludq 240(%esp), %xmm1
pmuludq %xmm2, %xmm7
paddq %xmm4, %xmm0
paddq %xmm7, %xmm5
paddq %xmm1, %xmm0
movdqa 288(%esp), %xmm4
movdqa 304(%esp), %xmm3
movdqa 320(%esp), %xmm2
je poly1305_blocks_sse2_21
poly1305_blocks_sse2_20:
movdqu (%edx), %xmm1
movdqu 16(%edx), %xmm7
movdqa %xmm1, %xmm6
movaps %xmm0, 400(%esp)
punpckldq %xmm7, %xmm6
pxor %xmm0, %xmm0
punpckhdq %xmm7, %xmm1
movdqa %xmm6, %xmm7
punpckhdq %xmm0, %xmm6
psllq $6, %xmm6
paddq %xmm6, %xmm3
movdqa %xmm1, %xmm6
punpckldq %xmm0, %xmm6
punpckhdq %xmm0, %xmm1
psllq $12, %xmm6
punpckldq %xmm0, %xmm7
psllq $18, %xmm1
movaps 400(%esp), %xmm0
paddq 272(%esp), %xmm0
paddq %xmm7, %xmm4
paddq %xmm6, %xmm2
paddq %xmm1, %xmm5
poly1305_blocks_sse2_21:
movdqa %xmm5, %xmm6
movdqa %xmm4, %xmm7
psrlq $26, %xmm6
psrlq $26, %xmm7
paddq %xmm6, %xmm0
paddq %xmm7, %xmm3
movaps %xmm0, %xmm7
movdqa %xmm3, %xmm1
psrlq $26, %xmm7
psrlq $26, %xmm1
pmuludq 160(%esp), %xmm7
paddq %xmm1, %xmm2
movdqa 256(%esp), %xmm1
movdqa %xmm2, %xmm6
pand 256(%esp), %xmm5
psrlq $26, %xmm6
pand %xmm1, %xmm3
pand 256(%esp), %xmm4
paddq %xmm6, %xmm5
paddq %xmm7, %xmm4
movdqa %xmm3, %xmm6
movdqa %xmm1, %xmm3
movdqa %xmm1, %xmm7
pand %xmm2, %xmm3
movdqa %xmm1, %xmm2
pand %xmm4, %xmm7
psrlq $26, %xmm4
pand %xmm5, %xmm1
pand %xmm2, %xmm0
psrlq $26, %xmm5
paddq %xmm4, %xmm6
paddq %xmm5, %xmm0
movdqa %xmm3, 32(%esp)
poly1305_blocks_sse2_22:
testl %edx, %edx
je poly1305_blocks_sse2_24
poly1305_blocks_sse2_23:
pshufd $8, %xmm7, %xmm2
pshufd $8, %xmm6, %xmm6
pshufd $8, 32(%esp), %xmm3
pshufd $8, %xmm1, %xmm1
punpcklqdq %xmm6, %xmm2
punpcklqdq %xmm1, %xmm3
pshufd $8, %xmm0, %xmm0
movdqu %xmm2, (%eax)
movdqu %xmm3, 16(%eax)
movq %xmm0, 32(%eax)
addl $544, %esp
popl %ebx
popl %edi
popl %esi
ret
poly1305_blocks_sse2_24:
movdqa %xmm7, %xmm2
movdqa %xmm6, %xmm3
psrldq $8, %xmm2
paddq %xmm2, %xmm7
psrldq $8, %xmm3
movd %xmm7, %ecx
paddq %xmm3, %xmm6
movdqa 32(%esp), %xmm5
movl %ecx, %esi
movdqa %xmm5, %xmm4
andl $67108863, %ecx
movd %xmm6, %ebx
movaps %xmm1, %xmm6
psrldq $8, %xmm4
paddq %xmm4, %xmm5
shrl $26, %esi
addl %esi, %ebx
psrldq $8, %xmm6
movd %xmm5, %edi
paddq %xmm6, %xmm1
movl %eax, (%esp)
movl %ebx, %eax
shrl $26, %eax
andl $67108863, %ebx
addl %eax, %edi
movd %xmm1, %eax
movaps %xmm0, %xmm1
psrldq $8, %xmm1
paddq %xmm1, %xmm0
movl %edi, %edx
andl $67108863, %edi
shrl $26, %edx
addl %edx, %eax
movd %xmm0, %edx
movl %eax, %esi
shrl $26, %esi
andl $67108863, %eax
addl %esi, %edx
movl %edx, %esi
andl $67108863, %edx
shrl $26, %esi
lea (%esi,%esi,4), %esi
addl %esi, %ecx
movl %ecx, %esi
andl $67108863, %ecx
shrl $26, %esi
addl %esi, %ebx
movl %ebx, %esi
andl $67108863, %ebx
shrl $26, %esi
addl %esi, %edi
movl %edi, %esi
shrl $26, %edi
andl $67108863, %esi
addl %edi, %eax
movl %eax, %edi
shrl $26, %eax
andl $67108863, %edi
addl %eax, %edx
movl %edx, %eax
shrl $26, %edx
andl $67108863, %eax
movl %eax, 8(%esp)
movl %edi, 4(%esp)
lea (%edx,%edx,4), %edx
addl %edx, %ecx
movl %ecx, %edx
andl $67108863, %edx
shrl $26, %ecx
addl %ecx, %ebx
lea 5(%edx), %ecx
movl %ecx, 12(%esp)
shrl $26, %ecx
addl %ebx, %ecx
movl %ecx, 16(%esp)
shrl $26, %ecx
addl %esi, %ecx
movl %ecx, 20(%esp)
shrl $26, %ecx
addl %edi, %ecx
movl %ecx, 24(%esp)
shrl $26, %ecx
movl 12(%esp), %edi
andl $67108863, %edi
lea -67108864(%ecx,%eax), %eax
movl %eax, 28(%esp)
shrl $31, %eax
decl %eax
movl %eax, %ecx
andl %eax, %edi
notl %ecx
andl %ecx, %edx
andl %ecx, %ebx
orl %edi, %edx
andl %ecx, %esi
movl (%esp), %edi
movl %edx, (%edi)
movl 16(%esp), %edx
andl $67108863, %edx
andl %eax, %edx
orl %edx, %ebx
movl %ebx, 4(%edi)
movl 20(%esp), %ebx
andl $67108863, %ebx
andl %eax, %ebx
movl 24(%esp), %edx
orl %ebx, %esi
andl $67108863, %edx
movl %esi, 8(%edi)
andl %eax, %edx
movl 4(%esp), %esi
andl %ecx, %esi
orl %edx, %esi
movl 28(%esp), %edx
andl 8(%esp), %ecx
andl %eax, %edx
orl %edx, %ecx
movl %esi, 12(%edi)
movl %ecx, 16(%edi)
poly1305_blocks_sse2_25:
addl $544, %esp
popl %ebx
popl %edi
popl %esi
ret
FN_END poly1305_blocks_sse2

GLOBAL_HIDDEN_FN poly1305_init_ext_sse2
movl 4(%esp), %eax
movl 8(%esp), %edx
movl 12(%esp), %ecx
poly1305_init_ext_sse2_local:
pushl %esi
pushl %edi
pushl %ebx
pushl %ebp
subl $76, %esp
movl %edx, %ebx
movl $-1, %edx
testl %ecx, %ecx
pxor %xmm0, %xmm0
movdqu %xmm0, (%eax)
movdqu %xmm0, 16(%eax)
movdqu %xmm0, 32(%eax)
cmove %edx, %ecx
movl 4(%ebx), %edx
movl %edx, %ebp
movl (%ebx), %edi
movl %edi, %esi
shrl $26, %edi
andl $67108863, %esi
shll $6, %ebp
movl %ecx, 12(%esp)
orl %ebp, %edi
movl 8(%ebx), %ecx
movl %ecx, %ebp
shrl $20, %edx
andl $67108611, %edi
shll $12, %ebp
movl %ebx, (%esp)
orl %ebp, %edx
movl 12(%ebx), %ebx
movl %ebx, %ebp
shrl $14, %ecx
andl $67092735, %edx
shll $18, %ebp
orl %ebp, %ecx
movl (%esp), %ebp
andl $66076671, %ecx
shrl $8, %ebx
andl $1048575, %ebx
movl %esi, 40(%eax)
movl %edi, 44(%eax)
movl %edx, 48(%eax)
movl %ecx, 52(%eax)
movl %ebx, 56(%eax)
movl %esi, 20(%esp)
movl 16(%ebp), %esi
movl %esi, 100(%eax)
movl %edi, 24(%esp)
movl 20(%ebp), %edi
movl %edi, 104(%eax)
movl 24(%ebp), %esi
movl %esi, 108(%eax)
lea 80(%eax), %esi
movl 28(%ebp), %ebp
movl 12(%esp), %edi
cmpl $16, %edi
movl %ebp, 112(%eax)
lea 60(%eax), %ebp
movl $0, 28(%esp)
movl %ebp, 16(%esp)
jbe poly1305_init_ext_sse2_9
poly1305_init_ext_sse2_2:
movl %ebp, 8(%esp)
movl %esi, (%esp)
movl %ebx, 32(%esp)
movl %ecx, 40(%esp)
movl %edx, 36(%esp)
movl %edi, 12(%esp)
movl %eax, 4(%esp)
poly1305_init_ext_sse2_3:
movl 40(%esp), %ebp
movl 20(%esp), %eax
mull %eax
movl 32(%esp), %ebx
lea (%ebp,%ebp,4), %esi
movl 24(%esp), %ebp
movl %eax, %ecx
movl %esi, 48(%esp)
lea (%ebx,%ebx), %edi
movl %edx, %ebx
movl %edi, 44(%esp)
lea (%ebp,%ebp,4), %eax
mull %edi
movl 36(%esp), %edi
addl %eax, %ecx
movl %edi, 60(%esp)
adcl %edx, %ebx
lea (%edi,%edi), %eax
mull %esi
addl %eax, %ecx
lea (%ebp,%ebp), %esi
movl 20(%esp), %eax
adcl %edx, %ebx
movl 40(%esp), %ebp
movl %esi, 56(%esp)
lea (%eax,%eax), %edx
movl %edx, 68(%esp)
mull %esi
movl %edx, %esi
addl %ebp, %ebp
movl %ebp, 64(%esp)
movl %eax, %ebp
movl %ecx, 52(%esp)
lea (%edi,%edi,4), %eax
mull 44(%esp)
addl %eax, %ebp
movl 40(%esp), %eax
movl 48(%esp), %edi
adcl %edx, %esi
mull %edi
shll $6, %ebx
shrl $26, %ecx
orl %ecx, %ebx
addl %ebx, %eax
adcl $0, %edx
addl %eax, %ebp
movl 24(%esp), %eax
adcl %edx, %esi
mull %eax
movl %eax, %ecx
movl %edx, %ebx
movl 60(%esp), %eax
mull 68(%esp)
addl %eax, %ecx
movl 44(%esp), %eax
adcl %edx, %ebx
mull %edi
movl %ebp, 72(%esp)
shll $6, %esi
shrl $26, %ebp
orl %ebp, %esi
addl %esi, %eax
adcl $0, %edx
addl %eax, %ecx
movl %ecx, %edi
adcl %edx, %ebx
andl $67108863, %edi
shll $6, %ebx
shrl $26, %ecx
movl 56(%esp), %eax
orl %ecx, %ebx
movl 60(%esp), %ecx
mull %ecx
movl %edi, 36(%esp)
movl %eax, %esi
movl 20(%esp), %eax
movl %edx, %ebp
movl 64(%esp), %edi
mull %edi
addl %eax, %esi
movl 32(%esp), %eax
adcl %edx, %ebp
lea (%eax,%eax,4), %edx
mull %edx
addl %eax, %esi
movl %ecx, %eax
adcl %edx, %ebp
addl %esi, %ebx
movl %ebx, %esi
adcl $0, %ebp
andl $67108863, %esi
mull %ecx
shll $6, %ebp
movl %eax, %ecx
shrl $26, %ebx
movl %edi, %eax
orl %ebx, %ebp
movl %edx, %ebx
mull 24(%esp)
addl %eax, %ecx
movl 68(%esp), %eax
adcl %edx, %ebx
mull 32(%esp)
addl %eax, %ecx
movl 52(%esp), %edi
adcl %edx, %ebx
addl %ecx, %ebp
movl %ebp, %ecx
adcl $0, %ebx
andl $67108863, %edi
shll $6, %ebx
andl $67108863, %ecx
shrl $26, %ebp
orl %ebp, %ebx
movl 72(%esp), %eax
andl $67108863, %eax
movl %ecx, 32(%esp)
movl 28(%esp), %ecx
lea (%ebx,%ebx,4), %ebx
addl %ebx, %edi
incl %ecx
movl %edi, %ebp
shrl $26, %edi
andl $67108863, %ebp
movl %esi, 40(%esp)
cmpl $2, %ecx
movl %ebp, 20(%esp)
movl %ecx, 28(%esp)
lea (%eax,%edi), %edx
movl %edx, 24(%esp)
jae poly1305_init_ext_sse2_8
poly1305_init_ext_sse2_4:
cmpl $0, 28(%esp)
jne poly1305_init_ext_sse2_6
poly1305_init_ext_sse2_5:
movl 8(%esp), %esi
movl 32(%esp), %eax
movl 40(%esp), %edx
movl 36(%esp), %ecx
movl 24(%esp), %ebx
movl 16(%esp), %edi
movl %eax, 16(%esi)
movl %edx, 12(%esi)
movl %ecx, 8(%esi)
movl %ebx, 4(%esi)
movl %ebp, (%esi)
movl %edi, 8(%esp)
jmp poly1305_init_ext_sse2_3
poly1305_init_ext_sse2_6:
cmpl $1, 28(%esp)
jne poly1305_init_ext_sse2_3
poly1305_init_ext_sse2_7:
movl 8(%esp), %esi
movl 32(%esp), %eax
movl 40(%esp), %edx
movl 36(%esp), %ecx
movl 24(%esp), %ebx
movl (%esp), %edi
movl %eax, 16(%esi)
movl %edx, 12(%esi)
movl %ecx, 8(%esi)
movl %ebx, 4(%esi)
movl %ebp, (%esi)
movl %edi, 8(%esp)
cmpl $96, 12(%esp)
jae poly1305_init_ext_sse2_3
jmp poly1305_init_ext_sse2_10
poly1305_init_ext_sse2_8:
movl 8(%esp), %ebp
movl %esi, %ecx
movl 36(%esp), %edx
movl %ecx, 12(%ebp)
movl %edx, 8(%ebp)
movl 32(%esp), %ebx
movl 24(%esp), %edx
movl 20(%esp), %ecx
movl 4(%esp), %eax
movl %ebx, 16(%ebp)
movl %edx, 4(%ebp)
movl %ecx, (%ebp)
poly1305_init_ext_sse2_9:
movl $0, 116(%eax)
addl $76, %esp
popl %ebp
popl %ebx
popl %edi
popl %esi
ret
poly1305_init_ext_sse2_10:
movl 4(%esp), %eax
jmp poly1305_init_ext_sse2_9
FN_END poly1305_init_ext_sse2

