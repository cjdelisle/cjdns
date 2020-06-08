#include "x86.inc"

SECTION_TEXT

GLOBAL_HIDDEN_FN poly1305_block_size_avx
movl $32, %eax
ret
FN_END poly1305_block_size_avx


GLOBAL_HIDDEN_FN poly1305_auth_avx
pushl %ebp
movl %esp, %ebp
andl $-64, %esp
pushl %esi
pushl %edi
pushl %ebx
subl $232, %esp
movl 16(%ebp), %edi
lea 52(%esp), %eax
pushl %edi
pushl 20(%ebp)
pushl %eax
movl 12(%ebp), %esi
call poly1305_init_ext_avx_local
poly1305_auth_avx_2:
movl %edi, %edx
andl $-32, %edx
je poly1305_auth_avx_5
poly1305_auth_avx_3:
addl $12, %esp
pushl %edx
pushl %esi
lea 60(%esp), %eax
pushl %eax
movl %edx, 192(%esp)
call poly1305_blocks_avx_local
poly1305_auth_avx_4:
movl 192(%esp), %edx
addl %edx, %esi
subl %edx, %edi
poly1305_auth_avx_5:
pushl 8(%ebp)
pushl %edi
pushl %esi
lea 76(%esp), %eax
pushl %eax
call poly1305_finish_ext_avx_local
poly1305_auth_avx_6:
addl $260, %esp
popl %ebx
popl %edi
popl %esi
movl %ebp, %esp
popl %ebp
ret
FN_END poly1305_auth_avx

GLOBAL_HIDDEN_FN poly1305_finish_ext_avx
poly1305_finish_ext_avx_local:
pushl %esi
pushl %edi
pushl %ebx
pushl %ebp
subl $60, %esp
movl 88(%esp), %ebp
movl 80(%esp), %esi
testl %ebp, %ebp
je poly1305_finish_ext_avx_18
poly1305_finish_ext_avx_2:
vpxor %xmm0, %xmm0, %xmm0
vmovups %xmm0, 32(%esp)
vmovups %xmm0, 16(%esp)
poly1305_finish_ext_avx_3:
movl 84(%esp), %eax
lea 16(%esp), %edx
subl %edx, %eax
testl $16, %ebp
je poly1305_finish_ext_avx_5
poly1305_finish_ext_avx_4:
lea 32(%esp), %edx
vmovdqu 16(%esp,%eax), %xmm0
vmovdqu %xmm0, 16(%esp)
poly1305_finish_ext_avx_5:
testl $8, %ebp
je poly1305_finish_ext_avx_7
poly1305_finish_ext_avx_6:
movl (%edx,%eax), %ecx
movl 4(%edx,%eax), %edi
movl %ecx, (%edx)
movl %edi, 4(%edx)
addl $8, %edx
poly1305_finish_ext_avx_7:
testl $4, %ebp
je poly1305_finish_ext_avx_9
poly1305_finish_ext_avx_8:
movl (%edx,%eax), %ecx
movl %ecx, (%edx)
addl $4, %edx
poly1305_finish_ext_avx_9:
testl $2, %ebp
je poly1305_finish_ext_avx_11
poly1305_finish_ext_avx_10:
movzwl (%edx,%eax), %ecx
movw %cx, (%edx)
addl $2, %edx
poly1305_finish_ext_avx_11:
testl $1, %ebp
je poly1305_finish_ext_avx_13
poly1305_finish_ext_avx_12:
movzbl (%edx,%eax), %eax
movb %al, (%edx)
poly1305_finish_ext_avx_13:
cmpl $16, %ebp
je poly1305_finish_ext_avx_16
poly1305_finish_ext_avx_14:
movb $1, 16(%esp,%ebp)
jae poly1305_finish_ext_avx_16
poly1305_finish_ext_avx_15:
movl $8, %eax
jmp poly1305_finish_ext_avx_17
poly1305_finish_ext_avx_16:
movl $4, %eax
poly1305_finish_ext_avx_17:
addl $12, %esp
pushl $32
orl %eax, 116(%esi)
lea 8(%esp), %eax
pushl %eax
pushl %esi
call poly1305_blocks_avx_local
poly1305_finish_ext_avx_18:
movl 116(%esi), %eax
testb $1, %al
je poly1305_finish_ext_avx_24
poly1305_finish_ext_avx_19:
testl %ebp, %ebp
je poly1305_finish_ext_avx_21
poly1305_finish_ext_avx_20:
cmpl $16, %ebp
jbe poly1305_finish_ext_avx_22
poly1305_finish_ext_avx_21:
orl $16, %eax
movl %eax, 116(%esi)
jmp poly1305_finish_ext_avx_23
poly1305_finish_ext_avx_22:
orl $32, %eax
movl %eax, 116(%esi)
poly1305_finish_ext_avx_23:
addl $12, %esp
pushl $32
pushl $0
pushl %esi
call poly1305_blocks_avx_local
poly1305_finish_ext_avx_24:
movl 8(%esi), %eax
movl %eax, %edi
movl 4(%esi), %edx
movl %edx, %ebx
shrl $6, %edx
shll $20, %edi
vpxor %xmm0, %xmm0, %xmm0
movl 12(%esi), %ecx
orl %edi, %edx
movl %ecx, %edi
shrl $12, %eax
shll $14, %edi
orl %edi, %eax
movl 16(%esi), %edi
shll $26, %ebx
shrl $18, %ecx
shll $8, %edi
movl 92(%esp), %ebp
orl %edi, %ecx
orl (%esi), %ebx
addl 100(%esi), %ebx
adcl 104(%esi), %edx
adcl 108(%esi), %eax
adcl 112(%esi), %ecx
vmovdqu %xmm0, (%esi)
vmovdqu %xmm0, 16(%esi)
vmovdqu %xmm0, 32(%esi)
vmovdqu %xmm0, 48(%esi)
vmovdqu %xmm0, 64(%esi)
vmovdqu %xmm0, 80(%esi)
vmovdqu %xmm0, 96(%esi)
vmovdqu %xmm0, 112(%esi)
movl %ebx, (%ebp)
movl %edx, 4(%ebp)
movl %eax, 8(%ebp)
movl %ecx, 12(%ebp)
addl $60, %esp
popl %ebp
popl %ebx
popl %edi
popl %esi
ret
FN_END poly1305_finish_ext_avx

GLOBAL_HIDDEN_FN poly1305_blocks_avx
poly1305_blocks_avx_local:
pushl %esi
pushl %edi
pushl %ebx
subl $560, %esp
movl $16777216, %ebx
movl $67108863, %esi
movl $5, %edi
movl 576(%esp), %ecx
movl 580(%esp), %eax
vmovd %ebx, %xmm0
vmovd %esi, %xmm2
vmovd %edi, %xmm4
movl 116(%ecx), %esi
vpshufd $68, %xmm0, %xmm1
vpshufd $68, %xmm2, %xmm3
vpshufd $68, %xmm4, %xmm5
movl 584(%esp), %edx
vmovdqu %xmm1, 96(%esp)
vmovdqu %xmm3, 80(%esp)
vmovdqu %xmm5, 48(%esp)
testl $4, %esi
je poly1305_blocks_avx_3
poly1305_blocks_avx_2:
vmovdqu 96(%esp), %xmm0
vpsrldq $8, %xmm0, %xmm1
vmovdqu %xmm1, 96(%esp)
poly1305_blocks_avx_3:
testl $8, %esi
je poly1305_blocks_avx_5
poly1305_blocks_avx_4:
vpxor %xmm0, %xmm0, %xmm0
vmovdqu %xmm0, 96(%esp)
poly1305_blocks_avx_5:
movl %esi, %ebx
btsl $0, %ebx
jc poly1305_blocks_avx_7
poly1305_blocks_avx_6:
vmovq (%eax), %xmm0
movl %ebx, %esi
vmovq 8(%eax), %xmm1
addl $-32, %edx
vmovhpd 16(%eax), %xmm0, %xmm3
vmovhpd 24(%eax), %xmm1, %xmm2
vpsrlq $52, %xmm3, %xmm4
vmovdqu 80(%esp), %xmm5
vpsllq $12, %xmm2, %xmm7
vpand %xmm3, %xmm5, %xmm0
vpsrlq $26, %xmm3, %xmm6
vpor %xmm7, %xmm4, %xmm3
vpsrlq $40, %xmm2, %xmm2
vpsrlq $26, %xmm3, %xmm4
vpand %xmm6, %xmm5, %xmm1
movl %esi, 116(%ecx)
addl $32, %eax
vpand %xmm3, %xmm5, %xmm6
vpand %xmm4, %xmm5, %xmm5
vpor 96(%esp), %xmm2, %xmm3
jmp poly1305_blocks_avx_8
poly1305_blocks_avx_7:
vmovdqu (%ecx), %xmm1
vmovdqu 16(%ecx), %xmm2
vpshufd $80, 32(%ecx), %xmm3
vpshufd $80, %xmm1, %xmm0
vpshufd $250, %xmm1, %xmm1
vpshufd $80, %xmm2, %xmm6
vpshufd $250, %xmm2, %xmm5
poly1305_blocks_avx_8:
testl $48, %esi
je poly1305_blocks_avx_13
poly1305_blocks_avx_9:
vmovdqu 40(%ecx), %xmm2
movl 56(%ecx), %edi
testl $16, %esi
je poly1305_blocks_avx_11
poly1305_blocks_avx_10:
vmovdqu 60(%ecx), %xmm7
vpunpckldq %xmm2, %xmm7, %xmm4
vpunpckhdq %xmm2, %xmm7, %xmm2
vmovd %edi, %xmm7
vmovdqu %xmm2, (%esp)
vmovd 76(%ecx), %xmm2
vpunpcklqdq %xmm7, %xmm2, %xmm2
vmovdqu %xmm2, 64(%esp)
vmovdqu (%esp), %xmm2
jmp poly1305_blocks_avx_12
poly1305_blocks_avx_11:
movl $1, %ebx
vmovd %ebx, %xmm7
vpunpckldq %xmm7, %xmm2, %xmm4
vpunpckhdq %xmm7, %xmm2, %xmm2
vmovd %edi, %xmm7
vmovdqu %xmm7, 64(%esp)
poly1305_blocks_avx_12:
vpshufd $80, %xmm4, %xmm7
vpshufd $250, %xmm4, %xmm4
vmovdqu %xmm4, 176(%esp)
vpshufd $80, %xmm2, %xmm4
vpshufd $250, %xmm2, %xmm2
vmovdqu %xmm7, 112(%esp)
vmovdqu %xmm4, 144(%esp)
vmovdqu %xmm2, 128(%esp)
jmp poly1305_blocks_avx_14
poly1305_blocks_avx_13:
vpshufd $0, 60(%ecx), %xmm4
vpshufd $85, 60(%ecx), %xmm7
vmovd 76(%ecx), %xmm2
vmovdqu %xmm4, 112(%esp)
vmovdqu %xmm7, 176(%esp)
vpshufd $170, 60(%ecx), %xmm4
vpshufd $255, 60(%ecx), %xmm7
vpshufd $0, %xmm2, %xmm2
vmovdqu %xmm4, 144(%esp)
vmovdqu %xmm7, 128(%esp)
vmovdqu %xmm2, 64(%esp)
poly1305_blocks_avx_14:
vmovdqu 48(%esp), %xmm4
vpmuludq 176(%esp), %xmm4, %xmm2
vpmuludq 128(%esp), %xmm4, %xmm7
vmovdqu %xmm2, 208(%esp)
vpmuludq 144(%esp), %xmm4, %xmm2
vpmuludq 64(%esp), %xmm4, %xmm4
vmovdqu %xmm7, 192(%esp)
vmovdqu %xmm4, 160(%esp)
cmpl $64, %edx
jb poly1305_blocks_avx_18
poly1305_blocks_avx_15:
vmovdqu %xmm6, 240(%esp)
vpshufd $0, 80(%ecx), %xmm6
vmovd 96(%ecx), %xmm7
vmovdqu %xmm5, (%esp)
vmovdqu %xmm3, 400(%esp)
vmovdqu %xmm1, 224(%esp)
vmovdqu %xmm6, 304(%esp)
vpshufd $170, 80(%ecx), %xmm5
vpshufd $255, 80(%ecx), %xmm4
vpshufd $0, %xmm7, %xmm1
vmovdqu 48(%esp), %xmm3
vpshufd $85, 80(%ecx), %xmm6
vmovdqu %xmm5, 352(%esp)
vmovdqu %xmm4, 336(%esp)
vmovdqu %xmm1, 320(%esp)
vpmuludq %xmm3, %xmm5, %xmm5
vpmuludq %xmm3, %xmm4, %xmm4
vpmuludq %xmm3, %xmm1, %xmm1
vpmuludq %xmm3, %xmm6, %xmm7
vmovdqu %xmm6, 384(%esp)
vmovdqu %xmm5, 272(%esp)
vmovdqu %xmm4, 32(%esp)
vmovdqu %xmm1, 16(%esp)
vmovdqu %xmm7, 288(%esp)
vmovdqu 400(%esp), %xmm3
vmovdqu (%esp), %xmm5
vmovdqu 240(%esp), %xmm6
vmovdqu 224(%esp), %xmm1
vmovdqu %xmm2, 368(%esp)
poly1305_blocks_avx_16:
vmovq (%eax), %xmm2
addl $-64, %edx
vmovdqu %xmm0, 256(%esp)
vmovhpd 16(%eax), %xmm2, %xmm0
vmovq 8(%eax), %xmm4
vmovdqu 80(%esp), %xmm2
vmovhpd 24(%eax), %xmm4, %xmm7
vpand %xmm0, %xmm2, %xmm4
vmovdqu %xmm4, (%esp)
vpsrlq $26, %xmm0, %xmm4
vpand %xmm4, %xmm2, %xmm4
vpsrlq $52, %xmm0, %xmm0
vmovdqu %xmm4, 416(%esp)
vpsrlq $14, %xmm7, %xmm4
vpand %xmm4, %xmm2, %xmm4
vmovdqu %xmm4, 432(%esp)
vpsllq $12, %xmm7, %xmm4
vpor %xmm4, %xmm0, %xmm0
vpsrlq $40, %xmm7, %xmm7
vpand %xmm0, %xmm2, %xmm2
vpor 96(%esp), %xmm7, %xmm0
vmovdqu 32(%eax), %xmm7
vpunpckldq 48(%eax), %xmm7, %xmm4
vpunpckhdq 48(%eax), %xmm7, %xmm7
addl $64, %eax
vmovdqu %xmm3, 400(%esp)
vmovdqu %xmm7, 496(%esp)
vpmuludq 288(%esp), %xmm3, %xmm3
vpmuludq 272(%esp), %xmm5, %xmm7
vmovdqu %xmm6, 240(%esp)
vpmuludq 32(%esp), %xmm6, %xmm6
vpaddq %xmm7, %xmm3, %xmm3
vpaddq %xmm6, %xmm3, %xmm3
vmovdqu 16(%esp), %xmm6
vmovdqu %xmm1, 224(%esp)
vpmuludq %xmm6, %xmm1, %xmm1
vmovdqu 304(%esp), %xmm7
vpaddq %xmm1, %xmm3, %xmm1
vpmuludq 256(%esp), %xmm7, %xmm3
vpmuludq %xmm7, %xmm5, %xmm7
vpaddq %xmm3, %xmm1, %xmm1
vmovdqu %xmm0, 464(%esp)
vpmuludq 208(%esp), %xmm0, %xmm0
vpaddq %xmm0, %xmm1, %xmm1
vmovdqu 432(%esp), %xmm0
vpmuludq 368(%esp), %xmm0, %xmm3
vmovdqu %xmm2, 448(%esp)
vpmuludq 192(%esp), %xmm2, %xmm2
vpaddq %xmm3, %xmm1, %xmm0
vmovdqu 160(%esp), %xmm3
vpmuludq 416(%esp), %xmm3, %xmm1
vpaddq %xmm2, %xmm0, %xmm0
vpaddq %xmm1, %xmm0, %xmm2
vmovdqu 112(%esp), %xmm1
vpmuludq (%esp), %xmm1, %xmm0
vpmuludq 432(%esp), %xmm1, %xmm1
vpaddq %xmm0, %xmm2, %xmm0
vpxor %xmm2, %xmm2, %xmm2
vmovdqu %xmm4, 480(%esp)
vpunpckldq %xmm2, %xmm4, %xmm4
vpaddq %xmm4, %xmm0, %xmm0
vmovdqu %xmm0, 512(%esp)
vmovdqu 400(%esp), %xmm0
vpmuludq %xmm6, %xmm0, %xmm6
vpmuludq 272(%esp), %xmm0, %xmm0
vpaddq %xmm7, %xmm6, %xmm4
vmovdqu 240(%esp), %xmm6
vpmuludq 384(%esp), %xmm6, %xmm7
vmovdqu 224(%esp), %xmm6
vpaddq %xmm7, %xmm4, %xmm4
vpmuludq 352(%esp), %xmm6, %xmm7
vmovdqu 256(%esp), %xmm6
vpaddq %xmm7, %xmm4, %xmm4
vpmuludq 336(%esp), %xmm6, %xmm7
vpaddq %xmm7, %xmm4, %xmm4
vmovdqu 464(%esp), %xmm7
vpmuludq %xmm3, %xmm7, %xmm3
vpmuludq 368(%esp), %xmm7, %xmm7
vpaddq %xmm3, %xmm4, %xmm3
vpaddq %xmm1, %xmm3, %xmm3
vmovdqu 448(%esp), %xmm1
vpmuludq 176(%esp), %xmm1, %xmm4
vmovdqu 416(%esp), %xmm1
vpaddq %xmm4, %xmm3, %xmm3
vpmuludq 144(%esp), %xmm1, %xmm4
vmovdqu (%esp), %xmm1
vpaddq %xmm4, %xmm3, %xmm3
vpmuludq 128(%esp), %xmm1, %xmm4
vpaddq %xmm4, %xmm3, %xmm4
vmovdqu 496(%esp), %xmm3
vpunpckhdq %xmm2, %xmm3, %xmm3
vpsllq $18, %xmm3, %xmm3
vpaddq %xmm3, %xmm4, %xmm4
vpmuludq 32(%esp), %xmm5, %xmm3
vmovdqu %xmm4, 528(%esp)
vpaddq %xmm3, %xmm0, %xmm4
vmovdqu 240(%esp), %xmm0
vpmuludq 16(%esp), %xmm0, %xmm0
vpaddq %xmm0, %xmm4, %xmm3
vmovdqu 304(%esp), %xmm4
vpmuludq 224(%esp), %xmm4, %xmm0
vpaddq %xmm0, %xmm3, %xmm0
vmovdqu 384(%esp), %xmm3
vpmuludq %xmm3, %xmm6, %xmm6
vpmuludq %xmm3, %xmm5, %xmm3
vpmuludq 16(%esp), %xmm5, %xmm5
vpaddq %xmm6, %xmm0, %xmm6
vpaddq %xmm7, %xmm6, %xmm0
vmovdqu 432(%esp), %xmm6
vpmuludq 192(%esp), %xmm6, %xmm7
vmovdqu 448(%esp), %xmm6
vpaddq %xmm7, %xmm0, %xmm0
vpmuludq 160(%esp), %xmm6, %xmm7
vpaddq %xmm7, %xmm0, %xmm6
vmovdqu 112(%esp), %xmm7
vpmuludq 416(%esp), %xmm7, %xmm0
vpaddq %xmm0, %xmm6, %xmm0
vmovdqu 176(%esp), %xmm6
vpmuludq %xmm6, %xmm1, %xmm1
vpmuludq 432(%esp), %xmm6, %xmm6
vpaddq %xmm1, %xmm0, %xmm0
vmovdqu 480(%esp), %xmm1
vpunpckhdq %xmm2, %xmm1, %xmm2
vpsllq $6, %xmm2, %xmm1
vpaddq %xmm1, %xmm0, %xmm2
vmovdqu 512(%esp), %xmm0
vpsrlq $26, %xmm0, %xmm0
vpaddq %xmm0, %xmm2, %xmm1
vmovdqu 400(%esp), %xmm0
vpmuludq %xmm4, %xmm0, %xmm2
vpmuludq 32(%esp), %xmm0, %xmm0
vpaddq %xmm3, %xmm2, %xmm2
vpaddq %xmm5, %xmm0, %xmm0
vmovdqu %xmm1, 544(%esp)
vmovdqu 240(%esp), %xmm1
vpmuludq 352(%esp), %xmm1, %xmm3
vpmuludq %xmm4, %xmm1, %xmm5
vpaddq %xmm3, %xmm2, %xmm2
vpaddq %xmm5, %xmm0, %xmm4
vmovdqu 224(%esp), %xmm3
vpmuludq 336(%esp), %xmm3, %xmm3
vpaddq %xmm3, %xmm2, %xmm2
vmovdqu 256(%esp), %xmm3
vpmuludq 320(%esp), %xmm3, %xmm3
vpaddq %xmm3, %xmm2, %xmm3
vmovdqu 464(%esp), %xmm2
vpmuludq %xmm7, %xmm2, %xmm7
vpmuludq 192(%esp), %xmm2, %xmm2
vpaddq %xmm7, %xmm3, %xmm3
vpaddq %xmm6, %xmm3, %xmm7
vmovdqu 448(%esp), %xmm6
vpmuludq 144(%esp), %xmm6, %xmm3
vpmuludq 112(%esp), %xmm6, %xmm6
vpaddq %xmm3, %xmm7, %xmm3
vmovdqu 416(%esp), %xmm7
vpmuludq 128(%esp), %xmm7, %xmm7
vpaddq %xmm7, %xmm3, %xmm7
vmovdqu (%esp), %xmm3
vpmuludq 64(%esp), %xmm3, %xmm3
vpaddq %xmm3, %xmm7, %xmm7
vpaddq 96(%esp), %xmm7, %xmm3
vmovdqu 528(%esp), %xmm7
vmovdqu 224(%esp), %xmm1
vpsrlq $26, %xmm7, %xmm7
vpaddq %xmm7, %xmm3, %xmm3
vpmuludq 384(%esp), %xmm1, %xmm7
vmovdqu 256(%esp), %xmm0
vpmuludq 352(%esp), %xmm0, %xmm1
vpaddq %xmm7, %xmm4, %xmm5
vmovdqu 432(%esp), %xmm0
vpaddq %xmm1, %xmm5, %xmm4
vpmuludq 160(%esp), %xmm0, %xmm1
vpaddq %xmm2, %xmm4, %xmm5
vpaddq %xmm1, %xmm5, %xmm2
vpaddq %xmm6, %xmm2, %xmm0
vmovdqu 416(%esp), %xmm6
vpmuludq 176(%esp), %xmm6, %xmm5
vmovdqu (%esp), %xmm1
vpxor %xmm6, %xmm6, %xmm6
vpmuludq 144(%esp), %xmm1, %xmm4
vpaddq %xmm5, %xmm0, %xmm2
vmovdqu 496(%esp), %xmm7
vpunpckldq %xmm6, %xmm7, %xmm0
vpsrlq $26, %xmm3, %xmm7
vpaddq %xmm4, %xmm2, %xmm5
vpsllq $12, %xmm0, %xmm1
vpaddq %xmm1, %xmm5, %xmm2
vpmuludq 48(%esp), %xmm7, %xmm5
vmovdqu 544(%esp), %xmm1
vpsrlq $26, %xmm1, %xmm4
vpaddq %xmm4, %xmm2, %xmm6
vmovdqu 80(%esp), %xmm4
vpsrlq $26, %xmm6, %xmm7
vpand 512(%esp), %xmm4, %xmm0
vpand %xmm4, %xmm1, %xmm1
vpand 528(%esp), %xmm4, %xmm2
vpand %xmm4, %xmm3, %xmm3
vpaddq %xmm5, %xmm0, %xmm5
vpaddq %xmm7, %xmm2, %xmm2
vpand %xmm4, %xmm5, %xmm0
vpsrlq $26, %xmm5, %xmm5
vpaddq %xmm5, %xmm1, %xmm1
vpand %xmm4, %xmm2, %xmm5
vpsrlq $26, %xmm2, %xmm2
vpaddq %xmm2, %xmm3, %xmm3
vpand %xmm4, %xmm6, %xmm6
cmpl $64, %edx
jae poly1305_blocks_avx_16
poly1305_blocks_avx_17:
vmovdqu 368(%esp), %xmm2
poly1305_blocks_avx_18:
cmpl $32, %edx
jb poly1305_blocks_avx_22
poly1305_blocks_avx_19:
vpmuludq 208(%esp), %xmm3, %xmm4
vpmuludq %xmm2, %xmm5, %xmm7
vpaddq %xmm7, %xmm4, %xmm7
vmovdqu 192(%esp), %xmm4
vmovdqu %xmm6, 240(%esp)
vpmuludq %xmm4, %xmm6, %xmm6
vpaddq %xmm6, %xmm7, %xmm7
vmovdqu 160(%esp), %xmm6
vmovdqu %xmm1, 224(%esp)
vpmuludq %xmm6, %xmm1, %xmm1
vpaddq %xmm1, %xmm7, %xmm7
vmovdqu 112(%esp), %xmm1
vmovdqu %xmm0, 256(%esp)
vpmuludq %xmm1, %xmm0, %xmm0
vpaddq %xmm0, %xmm7, %xmm0
vpmuludq %xmm2, %xmm3, %xmm7
vpmuludq %xmm4, %xmm5, %xmm2
vpmuludq %xmm4, %xmm3, %xmm4
vmovdqu %xmm0, 32(%esp)
vpaddq %xmm2, %xmm7, %xmm0
vpmuludq 240(%esp), %xmm6, %xmm7
vpaddq %xmm7, %xmm0, %xmm2
vpmuludq 224(%esp), %xmm1, %xmm0
vpaddq %xmm0, %xmm2, %xmm7
vmovdqu 176(%esp), %xmm2
vpmuludq 256(%esp), %xmm2, %xmm0
vpaddq %xmm0, %xmm7, %xmm7
vpmuludq %xmm6, %xmm5, %xmm0
vpmuludq %xmm6, %xmm3, %xmm6
vpmuludq %xmm1, %xmm3, %xmm3
vmovdqu %xmm7, (%esp)
vpaddq %xmm0, %xmm4, %xmm7
vpmuludq 240(%esp), %xmm1, %xmm4
vpaddq %xmm4, %xmm7, %xmm0
vpmuludq 224(%esp), %xmm2, %xmm7
vmovdqu 144(%esp), %xmm4
vpaddq %xmm7, %xmm0, %xmm0
vpmuludq 256(%esp), %xmm4, %xmm7
vpaddq %xmm7, %xmm0, %xmm0
vmovdqu %xmm0, 16(%esp)
vpmuludq %xmm1, %xmm5, %xmm0
vpmuludq %xmm2, %xmm5, %xmm5
vpaddq %xmm0, %xmm6, %xmm7
vpaddq %xmm5, %xmm3, %xmm1
vpmuludq 240(%esp), %xmm2, %xmm6
vpmuludq 240(%esp), %xmm4, %xmm2
vpaddq %xmm6, %xmm7, %xmm0
vpaddq %xmm2, %xmm1, %xmm3
vpmuludq 224(%esp), %xmm4, %xmm7
vpaddq %xmm7, %xmm0, %xmm6
vmovdqu 128(%esp), %xmm7
vpmuludq 256(%esp), %xmm7, %xmm0
vpmuludq 224(%esp), %xmm7, %xmm4
vpaddq %xmm0, %xmm6, %xmm0
vpaddq %xmm4, %xmm3, %xmm6
vmovdqu 256(%esp), %xmm5
vpmuludq 64(%esp), %xmm5, %xmm7
vmovdqu 32(%esp), %xmm1
vpaddq %xmm7, %xmm6, %xmm4
testl %eax, %eax
je poly1305_blocks_avx_21
poly1305_blocks_avx_20:
vmovdqu (%eax), %xmm5
vpaddq 96(%esp), %xmm4, %xmm4
vpunpckldq 16(%eax), %xmm5, %xmm7
vpxor %xmm3, %xmm3, %xmm3
vpunpckhdq 16(%eax), %xmm5, %xmm2
vpunpckldq %xmm3, %xmm7, %xmm6
vpunpckhdq %xmm3, %xmm7, %xmm5
vpaddq %xmm6, %xmm1, %xmm1
vpsllq $6, %xmm5, %xmm6
vpunpckldq %xmm3, %xmm2, %xmm5
vpaddq (%esp), %xmm6, %xmm7
vpunpckhdq %xmm3, %xmm2, %xmm2
vpsllq $12, %xmm5, %xmm6
vmovdqu %xmm7, (%esp)
vpsllq $18, %xmm2, %xmm3
vpaddq 16(%esp), %xmm6, %xmm7
vpaddq %xmm3, %xmm0, %xmm0
vmovdqu %xmm7, 16(%esp)
poly1305_blocks_avx_21:
vpsrlq $26, %xmm0, %xmm5
vpsrlq $26, %xmm1, %xmm7
vpaddq %xmm5, %xmm4, %xmm2
vpaddq (%esp), %xmm7, %xmm6
vmovdqu 80(%esp), %xmm3
vpsrlq $26, %xmm6, %xmm4
vpand %xmm3, %xmm1, %xmm7
vpsrlq $26, %xmm2, %xmm1
vpmuludq 48(%esp), %xmm1, %xmm1
vpand %xmm3, %xmm0, %xmm0
vpaddq 16(%esp), %xmm4, %xmm5
vpaddq %xmm1, %xmm7, %xmm1
vpsrlq $26, %xmm5, %xmm4
vpand %xmm3, %xmm6, %xmm6
vpaddq %xmm4, %xmm0, %xmm4
vpand %xmm3, %xmm1, %xmm0
vpsrlq $26, %xmm1, %xmm1
vpaddq %xmm1, %xmm6, %xmm1
vpand %xmm3, %xmm5, %xmm6
vpand %xmm3, %xmm4, %xmm5
vpand %xmm3, %xmm2, %xmm2
vpsrlq $26, %xmm4, %xmm3
vpaddq %xmm3, %xmm2, %xmm3
poly1305_blocks_avx_22:
testl %eax, %eax
je poly1305_blocks_avx_24
poly1305_blocks_avx_23:
vpshufd $8, %xmm0, %xmm0
vpshufd $8, %xmm1, %xmm1
vpshufd $8, %xmm6, %xmm6
vpshufd $8, %xmm5, %xmm5
vpunpcklqdq %xmm1, %xmm0, %xmm2
vpunpcklqdq %xmm5, %xmm6, %xmm4
vpshufd $8, %xmm3, %xmm3
vmovdqu %xmm2, (%ecx)
vmovdqu %xmm4, 16(%ecx)
vmovq %xmm3, 32(%ecx)
addl $560, %esp
popl %ebx
popl %edi
popl %esi
ret
poly1305_blocks_avx_24:
vpsrldq $8, %xmm0, %xmm2
vpaddq %xmm2, %xmm0, %xmm0
vpsrldq $8, %xmm1, %xmm4
vmovd %xmm0, %ecx
vpaddq %xmm4, %xmm1, %xmm1
vmovd %xmm1, %ebx
movl %ecx, %esi
vpsrldq $8, %xmm6, %xmm7
vpaddq %xmm7, %xmm6, %xmm6
shrl $26, %esi
andl $67108863, %ecx
addl %esi, %ebx
vmovd %xmm6, %edi
movl %ebx, %eax
vpsrldq $8, %xmm5, %xmm0
vpaddq %xmm0, %xmm5, %xmm5
shrl $26, %eax
andl $67108863, %ebx
addl %eax, %edi
vmovd %xmm5, %eax
movl %edi, %edx
vpsrldq $8, %xmm3, %xmm0
vpaddq %xmm0, %xmm3, %xmm3
shrl $26, %edx
andl $67108863, %edi
addl %edx, %eax
vmovd %xmm3, %edx
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
movl %eax, 4(%esp)
movl %edi, (%esp)
lea (%edx,%edx,4), %edx
addl %edx, %ecx
movl %ecx, %edx
andl $67108863, %edx
shrl $26, %ecx
addl %ecx, %ebx
lea 5(%edx), %ecx
movl %ecx, 8(%esp)
shrl $26, %ecx
addl %ebx, %ecx
movl %ecx, 12(%esp)
shrl $26, %ecx
addl %esi, %ecx
movl %ecx, 16(%esp)
shrl $26, %ecx
addl %edi, %ecx
movl %ecx, 20(%esp)
shrl $26, %ecx
movl 8(%esp), %edi
andl $67108863, %edi
lea -67108864(%ecx,%eax), %eax
movl %eax, 24(%esp)
shrl $31, %eax
decl %eax
movl %eax, %ecx
andl %eax, %edi
notl %ecx
andl %ecx, %edx
andl %ecx, %ebx
orl %edi, %edx
andl %ecx, %esi
movl 576(%esp), %edi
movl %edx, (%edi)
movl 12(%esp), %edx
andl $67108863, %edx
andl %eax, %edx
orl %edx, %ebx
movl %ebx, 4(%edi)
movl 16(%esp), %ebx
andl $67108863, %ebx
andl %eax, %ebx
movl 20(%esp), %edx
orl %ebx, %esi
andl $67108863, %edx
movl %esi, 8(%edi)
andl %eax, %edx
movl (%esp), %esi
andl %ecx, %esi
orl %edx, %esi
movl 24(%esp), %edx
andl 4(%esp), %ecx
andl %eax, %edx
orl %edx, %ecx
movl %esi, 12(%edi)
movl %ecx, 16(%edi)
poly1305_blocks_avx_25:
addl $560, %esp
popl %ebx
popl %edi
popl %esi
ret
FN_END poly1305_blocks_avx


GLOBAL_HIDDEN_FN poly1305_init_ext_avx
poly1305_init_ext_avx_local:
pushl %esi
pushl %edi
pushl %ebx
pushl %ebp
subl $76, %esp
movl $-1, %ecx
movl 96(%esp), %eax
vpxor %xmm0, %xmm0, %xmm0
movl 100(%esp), %ebp
movl 104(%esp), %edx
testl %edx, %edx
vmovdqu %xmm0, (%eax)
vmovdqu %xmm0, 16(%eax)
vmovdqu %xmm0, 32(%eax)
cmove %ecx, %edx
movl 4(%ebp), %ecx
movl %ecx, %esi
movl %edx, 12(%esp)
movl (%ebp), %edx
movl %edx, %edi
shrl $26, %edx
andl $67108863, %edi
shll $6, %esi
movl 8(%ebp), %ebx
orl %esi, %edx
movl %ebx, %esi
andl $67108611, %edx
shrl $20, %ecx
shll $12, %esi
movl 12(%ebp), %ebp
orl %esi, %ecx
movl %ebp, %esi
andl $67092735, %ecx
shrl $14, %ebx
shll $18, %esi
orl %esi, %ebx
movl 100(%esp), %esi
andl $66076671, %ebx
shrl $8, %ebp
andl $1048575, %ebp
movl %edi, 40(%eax)
movl %edx, 44(%eax)
movl %ecx, 48(%eax)
movl %ebx, 52(%eax)
movl %ebp, 56(%eax)
movl %edi, 24(%esp)
movl 16(%esi), %edi
movl %edi, 100(%eax)
movl 20(%esi), %edi
movl %edi, 104(%eax)
movl 24(%esi), %edi
movl %edi, 108(%eax)
movl 28(%esi), %esi
movl %esi, 112(%eax)
lea 60(%eax), %esi
movl $0, 36(%esp)
lea 80(%eax), %eax
movl %esi, 8(%esp)
cmpl $16, 12(%esp)
jbe poly1305_init_ext_avx_9
poly1305_init_ext_avx_2:
movl %esi, 4(%esp)
movl %eax, (%esp)
movl %ebp, 16(%esp)
movl %ebx, 32(%esp)
movl %ecx, 20(%esp)
movl %edx, 28(%esp)
poly1305_init_ext_avx_3:
movl 32(%esp), %ebp
movl 24(%esp), %eax
mull %eax
movl 16(%esp), %ebx
lea (%ebp,%ebp,4), %esi
movl 28(%esp), %ebp
movl %eax, %ecx
movl %esi, 44(%esp)
lea (%ebx,%ebx), %edi
movl %edx, %ebx
movl %edi, 40(%esp)
lea (%ebp,%ebp,4), %eax
mull %edi
movl 20(%esp), %edi
addl %eax, %ecx
movl %edi, 56(%esp)
adcl %edx, %ebx
lea (%edi,%edi), %eax
mull %esi
addl %eax, %ecx
lea (%ebp,%ebp), %esi
movl 24(%esp), %eax
adcl %edx, %ebx
movl 32(%esp), %ebp
movl %esi, 52(%esp)
lea (%eax,%eax), %edx
movl %edx, 64(%esp)
mull %esi
movl %edx, %esi
addl %ebp, %ebp
movl %ebp, 60(%esp)
movl %eax, %ebp
movl %ecx, 48(%esp)
lea (%edi,%edi,4), %eax
mull 40(%esp)
addl %eax, %ebp
movl 32(%esp), %eax
movl 44(%esp), %edi
adcl %edx, %esi
mull %edi
shll $6, %ebx
shrl $26, %ecx
orl %ecx, %ebx
addl %ebx, %eax
adcl $0, %edx
addl %eax, %ebp
movl 28(%esp), %eax
adcl %edx, %esi
mull %eax
movl %eax, %ecx
movl %edx, %ebx
movl 56(%esp), %eax
mull 64(%esp)
addl %eax, %ecx
movl 40(%esp), %eax
adcl %edx, %ebx
mull %edi
movl %ebp, 68(%esp)
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
movl 52(%esp), %eax
orl %ecx, %ebx
movl 56(%esp), %ecx
mull %ecx
movl %edi, 20(%esp)
movl %eax, %esi
movl 24(%esp), %eax
movl %edx, %ebp
movl 60(%esp), %edi
mull %edi
addl %eax, %esi
movl 16(%esp), %eax
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
mull 28(%esp)
addl %eax, %ecx
movl 64(%esp), %eax
adcl %edx, %ebx
mull 16(%esp)
addl %eax, %ecx
movl 48(%esp), %edi
adcl %edx, %ebx
addl %ecx, %ebp
movl %ebp, %ecx
adcl $0, %ebx
andl $67108863, %edi
shll $6, %ebx
andl $67108863, %ecx
shrl $26, %ebp
orl %ebp, %ebx
movl 68(%esp), %eax
andl $67108863, %eax
movl %ecx, 16(%esp)
movl 36(%esp), %ecx
lea (%ebx,%ebx,4), %ebx
addl %ebx, %edi
incl %ecx
movl %edi, %ebp
shrl $26, %edi
andl $67108863, %ebp
movl %esi, 32(%esp)
movl %ebp, 24(%esp)
movl %ecx, 36(%esp)
lea (%eax,%edi), %edx
movl %edx, 28(%esp)
cmpl $2, %ecx
jae poly1305_init_ext_avx_8
poly1305_init_ext_avx_4:
cmpl $0, 36(%esp)
jne poly1305_init_ext_avx_6
poly1305_init_ext_avx_5:
movl 4(%esp), %esi
movl 16(%esp), %eax
movl 32(%esp), %edx
movl 20(%esp), %ecx
movl 28(%esp), %ebx
movl 8(%esp), %edi
movl %eax, 16(%esi)
movl %edx, 12(%esi)
movl %ecx, 8(%esi)
movl %ebx, 4(%esi)
movl %ebp, (%esi)
movl %edi, 4(%esp)
jmp poly1305_init_ext_avx_3
poly1305_init_ext_avx_6:
cmpl $1, 36(%esp)
jne poly1305_init_ext_avx_3
poly1305_init_ext_avx_7:
movl 4(%esp), %esi
movl 16(%esp), %eax
movl 32(%esp), %edx
movl 20(%esp), %ecx
movl 28(%esp), %ebx
movl (%esp), %edi
movl %eax, 16(%esi)
movl %edx, 12(%esi)
movl %ecx, 8(%esi)
movl %ebx, 4(%esi)
movl %ebp, (%esi)
movl %edi, 4(%esp)
cmpl $96, 12(%esp)
jae poly1305_init_ext_avx_3
jmp poly1305_init_ext_avx_9
poly1305_init_ext_avx_8:
movl 4(%esp), %esi
movl 16(%esp), %ebp
movl 32(%esp), %ebx
movl 20(%esp), %ecx
movl 24(%esp), %eax
movl %ebp, 16(%esi)
movl %ebx, 12(%esi)
movl %ecx, 8(%esi)
movl %edx, 4(%esi)
movl %eax, (%esi)
poly1305_init_ext_avx_9:
movl 96(%esp), %eax
movl $0, 116(%eax)
addl $76, %esp
popl %ebp
popl %ebx
popl %edi
popl %esi
ret
FN_END poly1305_init_ext_avx

