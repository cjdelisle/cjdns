#include "x86.inc"

SECTION_TEXT


GLOBAL_HIDDEN_FN poly1305_block_size_avx2
movl $64, %eax
ret
FN_END poly1305_block_size_avx2


GLOBAL_HIDDEN_FN poly1305_auth_avx2
pushl %ebp
movl %esp, %ebp
andl $-64, %esp
pushl %esi
pushl %edi
pushl %ebx
subl $308, %esp
movl 16(%ebp), %esi
lea 64(%esp), %eax
movl %esi, %ecx
movl 20(%ebp), %edx
movl 12(%ebp), %edi
call poly1305_init_ext_avx2_local
movl %esi, %ebx
andl $-64, %ebx
je poly1305_auth_avx2_5
movl %edi, %edx
lea 64(%esp), %eax
movl %ebx, %ecx
call poly1305_blocks_avx2_local
addl %ebx, %edi
subl %ebx, %esi
poly1305_auth_avx2_5:
pushl 8(%ebp)
pushl %esi
pushl %edi
lea 76(%esp), %eax
pushl %eax
call poly1305_finish_ext_avx2_local
addl $324, %esp
popl %ebx
popl %edi
popl %esi
movl %ebp, %esp
popl %ebp
ret
FN_END poly1305_auth_avx2


GLOBAL_HIDDEN_FN poly1305_finish_ext_avx2
poly1305_finish_ext_avx2_local:
pushl %ebp
movl %esp, %ebp
andl $-64, %esp
pushl %esi
pushl %edi
pushl %ebx
subl $180, %esp
movl 16(%ebp), %ebx
testl %ebx, %ebx
je poly1305_finish_ext_avx2_29
poly1305_finish_ext_avx2_2:
movl 12(%ebp), %edx
lea 64(%esp), %eax
vpxor %ymm0, %ymm0, %ymm0
subl %eax, %edx
vmovdqu %ymm0, 64(%esp)
vmovdqu %ymm0, 96(%esp)
testb $32, %bl
je poly1305_finish_ext_avx2_4
poly1305_finish_ext_avx2_3:
vmovdqu 64(%esp,%edx), %ymm0
lea 96(%esp), %eax
vmovdqu %ymm0, 64(%esp)
poly1305_finish_ext_avx2_4:
testb $16, %bl
je poly1305_finish_ext_avx2_6
poly1305_finish_ext_avx2_5:
vmovdqu (%eax,%edx), %xmm0
vmovdqu %xmm0, (%eax)
addl $16, %eax
poly1305_finish_ext_avx2_6:
testb $8, %bl
je poly1305_finish_ext_avx2_8
poly1305_finish_ext_avx2_7:
movl (%eax,%edx), %ecx
movl 4(%eax,%edx), %esi
movl %ecx, (%eax)
movl %esi, 4(%eax)
addl $8, %eax
poly1305_finish_ext_avx2_8:
testb $4, %bl
je poly1305_finish_ext_avx2_10
poly1305_finish_ext_avx2_9:
movl (%eax,%edx), %ecx
movl %ecx, (%eax)
addl $4, %eax
poly1305_finish_ext_avx2_10:
testb $2, %bl
je poly1305_finish_ext_avx2_12
poly1305_finish_ext_avx2_11:
movzwl (%eax,%edx), %ecx
movw %cx, (%eax)
addl $2, %eax
poly1305_finish_ext_avx2_12:
testb $1, %bl
je poly1305_finish_ext_avx2_14
poly1305_finish_ext_avx2_13:
movzbl (%eax,%edx), %edx
movb %dl, (%eax)
poly1305_finish_ext_avx2_14:
testb $15, %bl
je poly1305_finish_ext_avx2_16
poly1305_finish_ext_avx2_15:
movb $1, 64(%esp,%ebx)
poly1305_finish_ext_avx2_16:
movl 8(%ebp), %eax
movl 176(%eax), %eax
andl $-8125, %eax
cmpl $48, %ebx
jb poly1305_finish_ext_avx2_18
poly1305_finish_ext_avx2_17:
orl $4, %eax
jmp poly1305_finish_ext_avx2_21
poly1305_finish_ext_avx2_18:
cmpl $32, %ebx
jb poly1305_finish_ext_avx2_20
poly1305_finish_ext_avx2_19:
orl $8, %eax
jmp poly1305_finish_ext_avx2_21
poly1305_finish_ext_avx2_20:
movl %eax, %edx
orl $32, %eax
orl $16, %edx
cmpl $16, %ebx
cmovae %edx, %eax
poly1305_finish_ext_avx2_21:
testb $1, %al
je poly1305_finish_ext_avx2_27
poly1305_finish_ext_avx2_22:
cmpl $16, %ebx
ja poly1305_finish_ext_avx2_24
poly1305_finish_ext_avx2_23:
movl 8(%ebp), %edx
orl $256, %eax
movl %eax, 176(%edx)
jmp poly1305_finish_ext_avx2_28
poly1305_finish_ext_avx2_24:
cmpl $32, %ebx
ja poly1305_finish_ext_avx2_27
poly1305_finish_ext_avx2_25:
movl 8(%ebp), %edx
orl $128, %eax
movl %eax, 176(%edx)
jmp poly1305_finish_ext_avx2_28
poly1305_finish_ext_avx2_27:
movl 8(%ebp), %edx
movl %eax, 176(%edx)
poly1305_finish_ext_avx2_28:
movl $64, %ecx
lea 64(%esp), %edx
movl 8(%ebp), %eax
vzeroupper
call poly1305_blocks_avx2_local
poly1305_finish_ext_avx2_29:
movl 8(%ebp), %eax
movl 176(%eax), %edx
testb $1, %dl
je poly1305_finish_ext_avx2_37
poly1305_finish_ext_avx2_30:
andl $-8125, %edx
testl %ebx, %ebx
je poly1305_finish_ext_avx2_32
poly1305_finish_ext_avx2_31:
cmpl $48, %ebx
jbe poly1305_finish_ext_avx2_33
poly1305_finish_ext_avx2_32:
orl $512, %edx
jmp poly1305_finish_ext_avx2_36
poly1305_finish_ext_avx2_33:
cmpl $32, %ebx
jbe poly1305_finish_ext_avx2_35
poly1305_finish_ext_avx2_34:
orl $1024, %edx
jmp poly1305_finish_ext_avx2_36
poly1305_finish_ext_avx2_35:
movl %edx, %eax
orl $4096, %edx
orl $2048, %eax
cmpl $16, %ebx
cmova %eax, %edx
poly1305_finish_ext_avx2_36:
movl 8(%ebp), %eax
orl $96, %edx
vpxor %ymm0, %ymm0, %ymm0
movl $64, %ecx
movl %edx, 176(%eax)
lea 64(%esp), %edx
vmovdqu %ymm0, 64(%esp)
vmovdqu %ymm0, 96(%esp)
vzeroupper
call poly1305_blocks_avx2_local
poly1305_finish_ext_avx2_37:
movl 8(%ebp), %esi
movl 20(%ebp), %edi
movl %edi, 128(%esp)
movl 8(%esi), %eax
movl %eax, %edi
movl 4(%esi), %edx
movl %edx, %ebx
shrl $6, %edx
shll $20, %edi
movl 12(%esi), %ecx
orl %edi, %edx
movl %ecx, %edi
shrl $12, %eax
shll $14, %edi
vpxor %ymm0, %ymm0, %ymm0
orl %edi, %eax
movl 16(%esi), %edi
shll $26, %ebx
shrl $18, %ecx
shll $8, %edi
orl (%esi), %ebx
orl %edi, %ecx
addl 160(%esi), %ebx
adcl 164(%esi), %edx
adcl 168(%esi), %eax
adcl 172(%esi), %ecx
vmovdqu %ymm0, (%esi)
vmovdqu %ymm0, 32(%esi)
vmovdqu %ymm0, 64(%esi)
vmovdqu %ymm0, 96(%esi)
vmovdqu %ymm0, 128(%esi)
vmovdqu %ymm0, 160(%esi)
movl 128(%esp), %esi
vzeroupper
movl %ebx, (%esi)
movl %edx, 4(%esi)
movl %eax, 8(%esi)
movl %ecx, 12(%esi)
addl $180, %esp
popl %ebx
popl %edi
popl %esi
movl %ebp, %esp
popl %ebp
ret
FN_END poly1305_finish_ext_avx2



GLOBAL_HIDDEN_FN poly1305_blocks_avx2
movl 4(%esp), %eax
movl 8(%esp), %edx
movl 12(%esp), %ecx
poly1305_blocks_avx2_local:
pushl %ebp
movl %esp, %ebp
andl $-64, %esp
pushl %esi
pushl %edi
pushl %ebx
subl $628, %esp
movl $16777216, %ebx
movl $67108863, %esi
movl $5, %edi
vmovd %ebx, %xmm0
vmovd %esi, %xmm2
vmovd %edi, %xmm4
vpbroadcastq %xmm0, %ymm1
vpbroadcastq %xmm2, %ymm3
vpbroadcastq %xmm4, %ymm5
movl 176(%eax), %ebx
vmovdqu %ymm1, 256(%esp)
vmovdqu %ymm3, 224(%esp)
vmovdqu %ymm5, 192(%esp)
testb $60, %bl
je poly1305_blocks_avx2_12
poly1305_blocks_avx2_2:
vmovdqu 256(%esp), %ymm0
vpsrldq $8, %ymm0, %ymm5
testb $4, %bl
je poly1305_blocks_avx2_4
poly1305_blocks_avx2_3:
vpermq $192, %ymm5, %ymm5
poly1305_blocks_avx2_4:
testb $8, %bl
je poly1305_blocks_avx2_6
poly1305_blocks_avx2_5:
vpermq $240, %ymm5, %ymm5
poly1305_blocks_avx2_6:
testb $16, %bl
je poly1305_blocks_avx2_8
poly1305_blocks_avx2_7:
vpermq $252, %ymm5, %ymm5
poly1305_blocks_avx2_8:
testb $32, %bl
je poly1305_blocks_avx2_10
poly1305_blocks_avx2_9:
vpxor %ymm5, %ymm5, %ymm5
poly1305_blocks_avx2_10:
vmovdqu %ymm5, 256(%esp)
poly1305_blocks_avx2_12:
movl %ebx, %esi
btsl $0, %esi
jc poly1305_blocks_avx2_14
poly1305_blocks_avx2_13:
vmovdqu (%edx), %ymm3
movl %esi, %ebx
vmovdqu 224(%esp), %ymm2
vpunpcklqdq 32(%edx), %ymm3, %ymm4
vpunpckhqdq 32(%edx), %ymm3, %ymm6
vpermq $216, %ymm4, %ymm1
addl $64, %edx
vpermq $216, %ymm6, %ymm0
addl $-64, %ecx
vpand %ymm1, %ymm2, %ymm4
vpsrlq $26, %ymm1, %ymm7
vpsrlq $52, %ymm1, %ymm1
vpsllq $12, %ymm0, %ymm6
vpsrlq $40, %ymm0, %ymm0
vpand %ymm7, %ymm2, %ymm3
vpor %ymm6, %ymm1, %ymm7
vpor 256(%esp), %ymm0, %ymm0
vpsrlq $26, %ymm7, %ymm6
vpand %ymm7, %ymm2, %ymm1
vmovdqu %ymm7, 128(%esp)
vpand %ymm6, %ymm2, %ymm2
vmovdqu %ymm6, 160(%esp)
movl %ebx, 176(%eax)
jmp poly1305_blocks_avx2_15
poly1305_blocks_avx2_14:
vpermq $216, (%eax), %ymm5
vpxor %ymm6, %ymm6, %ymm6
vpermq $216, 32(%eax), %ymm2
vpermq $216, 64(%eax), %ymm0
vmovdqu %ymm2, 128(%esp)
vmovdqu %ymm0, 160(%esp)
vpunpckldq %ymm6, %ymm5, %ymm4
vpunpckhdq %ymm6, %ymm5, %ymm3
vpunpckldq %ymm6, %ymm2, %ymm1
vpunpckhdq %ymm6, %ymm2, %ymm2
vpunpckldq %ymm6, %ymm0, %ymm0
poly1305_blocks_avx2_15:
cmpl $64, %ecx
jb poly1305_blocks_avx2_35
poly1305_blocks_avx2_16:
vmovdqu 140(%eax), %ymm6
testl $8064, %ebx
je poly1305_blocks_avx2_31
poly1305_blocks_avx2_17:
vpermq $216, 80(%eax), %ymm7
vmovdqu %ymm7, 32(%esp)
vpermq $216, 100(%eax), %ymm7
vmovdqu %ymm7, 64(%esp)
vpermq $216, 120(%eax), %ymm7
vmovdqu %ymm7, (%esp)
vpermq $216, %ymm6, %ymm6
testl $128, %ebx
je poly1305_blocks_avx2_19
poly1305_blocks_avx2_18:
vmovdqa %ymm6, %ymm5
vmovdqu %ymm6, 128(%esp)
vmovdqu %ymm6, 160(%esp)
jmp poly1305_blocks_avx2_30
poly1305_blocks_avx2_19:
testl $256, %ebx
je poly1305_blocks_avx2_21
poly1305_blocks_avx2_20:
vmovdqu %ymm6, 128(%esp)
vmovdqu 64(%esp), %ymm7
vmovdqa %ymm6, %ymm5
vmovdqu (%esp), %ymm6
vmovdqu %ymm7, (%esp)
vmovdqu %ymm6, 160(%esp)
jmp poly1305_blocks_avx2_30
poly1305_blocks_avx2_21:
testl $512, %ebx
je poly1305_blocks_avx2_23
poly1305_blocks_avx2_22:
vmovdqa %ymm6, %ymm5
vmovdqu (%esp), %ymm6
vmovdqu 64(%esp), %ymm7
vmovdqu %ymm6, 128(%esp)
vmovdqu 32(%esp), %ymm6
vmovdqu %ymm7, 160(%esp)
vmovdqu %ymm6, (%esp)
jmp poly1305_blocks_avx2_30
poly1305_blocks_avx2_23:
testl $1024, %ebx
je poly1305_blocks_avx2_25
poly1305_blocks_avx2_24:
movl $1, %edi
vmovdqu 64(%esp), %ymm6
vmovdqu 32(%esp), %ymm7
vmovdqu (%esp), %ymm5
vmovdqu %ymm6, 128(%esp)
vpxor %ymm6, %ymm6, %ymm6
vmovd %edi, %xmm6
vmovdqu %ymm7, 160(%esp)
vmovdqu %ymm6, (%esp)
jmp poly1305_blocks_avx2_30
poly1305_blocks_avx2_25:
testl $2048, %ebx
je poly1305_blocks_avx2_27
poly1305_blocks_avx2_26:
movl $1, %edi
vmovdqu 32(%esp), %ymm6
vpxor %ymm7, %ymm7, %ymm7
vmovd %edi, %xmm7
vmovdqu 64(%esp), %ymm5
vmovdqu %ymm6, 128(%esp)
vmovdqu %ymm7, 160(%esp)
vmovdqa %ymm7, %ymm6
vmovdqu %ymm6, (%esp)
jmp poly1305_blocks_avx2_30
poly1305_blocks_avx2_27:
testl $4096, %ebx
je poly1305_blocks_avx2_29
poly1305_blocks_avx2_28:
movl $1, %edi
vpxor %ymm6, %ymm6, %ymm6
vmovd %edi, %xmm6
vmovdqu 32(%esp), %ymm5
vmovdqu %ymm6, 128(%esp)
vmovdqu %ymm6, (%esp)
vmovdqa %ymm6, %ymm7
vmovdqu %ymm7, 160(%esp)
jmp poly1305_blocks_avx2_30
poly1305_blocks_avx2_29:
vmovdqu %ymm6, (%esp)
poly1305_blocks_avx2_30:
vmovdqu %ymm3, 416(%esp)
vmovdqu %ymm1, 32(%esp)
vmovdqu %ymm2, 64(%esp)
vmovdqu 128(%esp), %ymm1
vmovdqu (%esp), %ymm2
vmovdqu 160(%esp), %ymm3
vmovdqu %ymm0, 96(%esp)
vpunpcklqdq %ymm1, %ymm5, %ymm6
vpunpckhqdq %ymm1, %ymm5, %ymm5
vpunpcklqdq %ymm2, %ymm3, %ymm7
vmovdqu 32(%esp), %ymm1
vperm2i128 $32, %ymm7, %ymm6, %ymm0
vmovdqu %ymm0, 288(%esp)
vpsrlq $32, %ymm0, %ymm0
vmovdqu %ymm0, 320(%esp)
vpunpckhqdq %ymm2, %ymm3, %ymm0
vmovdqu 64(%esp), %ymm2
vmovdqu 416(%esp), %ymm3
vperm2i128 $49, %ymm7, %ymm6, %ymm6
vpsrlq $32, %ymm6, %ymm7
vmovdqu %ymm6, 352(%esp)
vmovdqu %ymm7, 384(%esp)
vperm2i128 $32, %ymm0, %ymm5, %ymm5
vmovdqu 96(%esp), %ymm0
jmp poly1305_blocks_avx2_32
poly1305_blocks_avx2_31:
vpsrlq $32, %ymm6, %ymm5
vpermq $0, %ymm6, %ymm7
vmovdqu %ymm7, 288(%esp)
vpermq $0, %ymm5, %ymm7
vmovdqu %ymm7, 320(%esp)
vpermq $85, %ymm6, %ymm7
vpermq $85, %ymm5, %ymm5
vmovdqu %ymm7, 352(%esp)
vmovdqu %ymm5, 384(%esp)
vpermq $170, %ymm6, %ymm5
poly1305_blocks_avx2_32:
vmovdqu 192(%esp), %ymm6
vmovdqu %ymm5, (%esp)
vpmuludq 320(%esp), %ymm6, %ymm7
vmovdqu %ymm7, 128(%esp)
vpmuludq 352(%esp), %ymm6, %ymm7
vmovdqu %ymm7, 96(%esp)
vpmuludq 384(%esp), %ymm6, %ymm7
vpmuludq %ymm6, %ymm5, %ymm6
vmovdqu %ymm7, 64(%esp)
vmovdqu %ymm6, 32(%esp)
jmp poly1305_blocks_avx2_33
.p2align 6
poly1305_blocks_avx2_33:
vmovdqu (%edx), %ymm6
addl $-64, %ecx
vmovdqu %ymm3, 416(%esp)
vmovdqu %ymm4, 160(%esp)
vperm2i128 $32, 32(%edx), %ymm6, %ymm7
vperm2i128 $49, 32(%edx), %ymm6, %ymm5
addl $64, %edx
vpunpckldq %ymm5, %ymm7, %ymm6
vpunpckhdq %ymm5, %ymm7, %ymm7
vmovdqu %ymm6, 448(%esp)
vmovdqu %ymm7, 480(%esp)
vpmuludq 128(%esp), %ymm0, %ymm7
vpmuludq 96(%esp), %ymm2, %ymm5
vpaddq %ymm5, %ymm7, %ymm7
vpmuludq 64(%esp), %ymm1, %ymm5
vpaddq %ymm5, %ymm7, %ymm5
vmovdqu 32(%esp), %ymm7
vpmuludq %ymm7, %ymm3, %ymm3
vpaddq %ymm3, %ymm5, %ymm5
vmovdqu 288(%esp), %ymm3
vpmuludq %ymm3, %ymm4, %ymm4
vpaddq %ymm4, %ymm5, %ymm4
vpxor %ymm5, %ymm5, %ymm5
vpunpckldq %ymm5, %ymm6, %ymm6
vpaddq %ymm6, %ymm4, %ymm4
vpmuludq %ymm7, %ymm0, %ymm6
vpmuludq %ymm3, %ymm2, %ymm3
vmovdqu %ymm4, 512(%esp)
vpaddq %ymm3, %ymm6, %ymm4
vmovdqu 416(%esp), %ymm6
vpmuludq 320(%esp), %ymm1, %ymm3
vpaddq %ymm3, %ymm4, %ymm4
vpmuludq 352(%esp), %ymm6, %ymm3
vmovdqu 160(%esp), %ymm6
vpaddq %ymm3, %ymm4, %ymm4
vpmuludq 384(%esp), %ymm6, %ymm3
vpaddq %ymm3, %ymm4, %ymm4
vmovdqu 480(%esp), %ymm3
vpunpckhdq %ymm5, %ymm3, %ymm3
vpsllq $18, %ymm3, %ymm3
vpaddq %ymm3, %ymm4, %ymm4
vmovdqu %ymm4, 544(%esp)
vpmuludq 96(%esp), %ymm0, %ymm4
vpmuludq 64(%esp), %ymm2, %ymm3
vpaddq %ymm3, %ymm4, %ymm4
vmovdqu 288(%esp), %ymm3
vpmuludq %ymm7, %ymm1, %ymm7
vpaddq %ymm7, %ymm4, %ymm4
vpmuludq 416(%esp), %ymm3, %ymm7
vpaddq %ymm7, %ymm4, %ymm7
vmovdqu 320(%esp), %ymm4
vpmuludq %ymm4, %ymm6, %ymm6
vpaddq %ymm6, %ymm7, %ymm7
vmovdqu 448(%esp), %ymm6
vpunpckhdq %ymm5, %ymm6, %ymm5
vpsllq $6, %ymm5, %ymm5
vpaddq %ymm5, %ymm7, %ymm6
vmovdqu 512(%esp), %ymm7
vpsrlq $26, %ymm7, %ymm7
vpaddq %ymm7, %ymm6, %ymm5
vpmuludq %ymm3, %ymm0, %ymm6
vpmuludq %ymm4, %ymm2, %ymm7
vmovdqu %ymm5, 576(%esp)
vpaddq %ymm7, %ymm6, %ymm5
vmovdqu 416(%esp), %ymm7
vpmuludq 352(%esp), %ymm1, %ymm6
vpaddq %ymm6, %ymm5, %ymm5
vpmuludq 384(%esp), %ymm7, %ymm6
vpaddq %ymm6, %ymm5, %ymm6
vmovdqu 160(%esp), %ymm5
vpmuludq (%esp), %ymm5, %ymm5
vpaddq %ymm5, %ymm6, %ymm6
vpaddq 256(%esp), %ymm6, %ymm5
vmovdqu 544(%esp), %ymm6
vpmuludq 64(%esp), %ymm0, %ymm0
vpmuludq 32(%esp), %ymm2, %ymm2
vpsrlq $26, %ymm6, %ymm6
vpaddq %ymm2, %ymm0, %ymm2
vmovdqu 480(%esp), %ymm0
vpaddq %ymm6, %ymm5, %ymm5
vpxor %ymm6, %ymm6, %ymm6
vpmuludq %ymm3, %ymm1, %ymm1
vpaddq %ymm1, %ymm2, %ymm2
vmovdqu 160(%esp), %ymm1
vpmuludq %ymm4, %ymm7, %ymm4
vpaddq %ymm4, %ymm2, %ymm3
vpunpckldq %ymm6, %ymm0, %ymm2
vpsrlq $26, %ymm5, %ymm6
vpmuludq 352(%esp), %ymm1, %ymm7
vpaddq %ymm7, %ymm3, %ymm4
vpsllq $12, %ymm2, %ymm1
vmovdqu 576(%esp), %ymm3
vpaddq %ymm1, %ymm4, %ymm7
vpsrlq $26, %ymm3, %ymm0
vpaddq %ymm0, %ymm7, %ymm2
vmovdqu 224(%esp), %ymm0
vpsrlq $26, %ymm2, %ymm7
vpand 512(%esp), %ymm0, %ymm4
vpand %ymm0, %ymm3, %ymm3
vpand %ymm0, %ymm5, %ymm5
vpmuludq 192(%esp), %ymm6, %ymm1
vpaddq %ymm1, %ymm4, %ymm1
vpand 544(%esp), %ymm0, %ymm4
vpaddq %ymm7, %ymm4, %ymm7
vpand %ymm0, %ymm1, %ymm4
vpsrlq $26, %ymm1, %ymm1
vpaddq %ymm1, %ymm3, %ymm3
vpand %ymm0, %ymm2, %ymm1
vpand %ymm0, %ymm7, %ymm2
vpsrlq $26, %ymm7, %ymm0
vpaddq %ymm0, %ymm5, %ymm0
cmpl $64, %ecx
jae poly1305_blocks_avx2_33
poly1305_blocks_avx2_35:
testb $64, %bl
jne poly1305_blocks_avx2_37
poly1305_blocks_avx2_36:
vpshufd $8, %ymm4, %ymm4
vpshufd $8, %ymm3, %ymm5
vpshufd $8, %ymm1, %ymm1
vpshufd $8, %ymm2, %ymm2
vpshufd $8, %ymm0, %ymm0
vpermq $8, %ymm4, %ymm6
vpermq $8, %ymm5, %ymm7
vpermq $8, %ymm1, %ymm1
vpermq $8, %ymm2, %ymm2
vpermq $8, %ymm0, %ymm0
vperm2i128 $32, %ymm7, %ymm6, %ymm3
vperm2i128 $32, %ymm2, %ymm1, %ymm4
vmovdqu %ymm3, (%eax)
vmovdqu %ymm4, 32(%eax)
vmovdqu %xmm0, 64(%eax)
jmp poly1305_blocks_avx2_38
poly1305_blocks_avx2_37:
vpermq $245, %ymm4, %ymm5
vpaddq %ymm5, %ymm4, %ymm4
vpermq $245, %ymm3, %ymm6
vpaddq %ymm6, %ymm3, %ymm3
vpermq $245, %ymm1, %ymm7
vpaddq %ymm7, %ymm1, %ymm1
vpermq $170, %ymm4, %ymm7
vpermq $245, %ymm2, %ymm5
vpaddq %ymm7, %ymm4, %ymm4
vpaddq %ymm5, %ymm2, %ymm2
vpermq $170, %ymm3, %ymm5
vpaddq %ymm5, %ymm3, %ymm3
movl %eax, (%esp)
vpermq $245, %ymm0, %ymm6
vpaddq %ymm6, %ymm0, %ymm0
vmovd %xmm4, %eax
vpermq $170, %ymm1, %ymm4
movl %eax, %edx
vpaddq %ymm4, %ymm1, %ymm1
andl $67108863, %eax
shrl $26, %edx
vmovd %xmm3, %ebx
vpermq $170, %ymm2, %ymm3
addl %edx, %ebx
vpaddq %ymm3, %ymm2, %ymm2
movl %ebx, %edi
shrl $26, %edi
andl $67108863, %ebx
vmovd %xmm1, %ecx
vpermq $170, %ymm0, %ymm1
addl %edi, %ecx
vpaddq %ymm1, %ymm0, %ymm0
movl %ecx, %edx
shrl $26, %edx
andl $67108863, %ecx
vmovd %xmm2, %esi
addl %edx, %esi
movl %esi, %edx
andl $67108863, %esi
shrl $26, %edx
vmovd %xmm0, %edi
addl %edx, %edi
movl %edi, %edx
andl $67108863, %edi
shrl $26, %edx
lea (%edx,%edx,4), %edx
addl %edx, %eax
movl %eax, %edx
andl $67108863, %eax
shrl $26, %edx
addl %edx, %ebx
movl %ebx, %edx
andl $67108863, %ebx
shrl $26, %edx
addl %edx, %ecx
movl %ecx, %edx
shrl $26, %ecx
andl $67108863, %edx
addl %ecx, %esi
movl %esi, %ecx
shrl $26, %esi
andl $67108863, %ecx
addl %esi, %edi
movl %edi, %esi
shrl $26, %edi
andl $67108863, %esi
movl %esi, 4(%esp)
lea (%edi,%edi,4), %edi
addl %edi, %eax
movl %eax, %edi
andl $67108863, %edi
shrl $26, %eax
addl %eax, %ebx
lea 5(%edi), %eax
movl %eax, 8(%esp)
shrl $26, %eax
addl %ebx, %eax
movl %eax, 12(%esp)
shrl $26, %eax
addl %edx, %eax
movl %eax, 16(%esp)
shrl $26, %eax
addl %ecx, %eax
movl %eax, 20(%esp)
shrl $26, %eax
lea -67108864(%eax,%esi), %eax
movl %eax, 24(%esp)
shrl $31, %eax
decl %eax
andn %edi, %eax, %esi
movl 8(%esp), %edi
andl $67108863, %edi
andl %eax, %edi
orl %edi, %esi
movl (%esp), %edi
movl %esi, (%edi)
andn %ebx, %eax, %esi
movl 12(%esp), %ebx
andl $67108863, %ebx
andl %eax, %ebx
orl %ebx, %esi
andn %edx, %eax, %ebx
movl 16(%esp), %edx
andl $67108863, %edx
andl %eax, %edx
orl %edx, %ebx
andn %ecx, %eax, %edx
movl 20(%esp), %ecx
andl $67108863, %ecx
movl %ebx, 8(%edi)
andl %eax, %ecx
movl %esi, 4(%edi)
orl %ecx, %edx
movl 24(%esp), %ebx
andn 4(%esp), %eax, %esi
andl %eax, %ebx
orl %ebx, %esi
movl %edx, 12(%edi)
movl %esi, 16(%edi)
poly1305_blocks_avx2_38:
vzeroupper
addl $628, %esp
popl %ebx
popl %edi
popl %esi
movl %ebp, %esp
popl %ebp
ret
FN_END poly1305_blocks_avx2


GLOBAL_HIDDEN_FN poly1305_init_ext_avx2
movl 4(%esp), %eax
movl 8(%esp), %edx
movl 12(%esp), %ecx
poly1305_init_ext_avx2_local:
pushl %esi
pushl %edi
pushl %ebx
subl $272, %esp
movl %edx, %edi
vpxor %ymm0, %ymm0, %ymm0
movl %eax, %edx
vmovdqu %ymm0, (%edx)
movl $-1, %eax
testl %ecx, %ecx
vmovdqu %ymm0, 32(%edx)
vpxor %xmm0, %xmm0, %xmm0
vmovdqu %xmm0, 64(%edx)
cmove %eax, %ecx
movl %ecx, 4(%esp)
movl (%edi), %esi
movl 4(%edi), %ecx
movl 8(%edi), %ebx
movl 12(%edi), %eax
movl %edi, (%esp)
movl %esi, %edi
andl $67108863, %edi
movl %edi, 80(%edx)
movl %ecx, %edi
shrl $26, %esi
shll $6, %edi
orl %edi, %esi
andl $67108611, %esi
movl %esi, 84(%edx)
movl %ebx, %esi
shrl $20, %ecx
shll $12, %esi
orl %esi, %ecx
andl $67092735, %ecx
movl %ecx, 88(%edx)
movl %eax, %ecx
shrl $14, %ebx
shll $18, %ecx
shrl $8, %eax
orl %ecx, %ebx
andl $66076671, %ebx
andl $1048575, %eax
movl %ebx, 92(%edx)
xorl %ecx, %ecx
movl %eax, 96(%edx)
movl 4(%esp), %edi
movl (%esp), %esi
poly1305_init_ext_avx2_2:
movl 16(%esi,%ecx,4), %ebx
movl %ebx, 160(%edx,%ecx,4)
incl %ecx
cmpl $4, %ecx
jl poly1305_init_ext_avx2_2
poly1305_init_ext_avx2_3:
cmpl $16, %edi
jbe poly1305_init_ext_avx2_6
poly1305_init_ext_avx2_4:
movl $5, %ecx
vmovd %eax, %xmm5
vmovdqu 80(%edx), %xmm4
vmovd %ecx, %xmm3
movl $67108863, %ecx
vmovdqu %xmm4, 224(%esp)
vpshufd $68, %xmm3, %xmm3
vpunpcklqdq %xmm0, %xmm5, %xmm1
vmovd %ecx, %xmm2
vpshufd $68, %xmm2, %xmm6
vpunpcklqdq %xmm0, %xmm4, %xmm2
vpunpckhqdq %xmm0, %xmm4, %xmm4
vpsrlq $32, %xmm2, %xmm7
vpsrlq $32, %xmm4, %xmm0
vmovdqu %xmm5, 208(%esp)
vmovdqu %xmm0, 32(%esp)
vpmuludq %xmm3, %xmm0, %xmm5
vpaddd %xmm0, %xmm0, %xmm0
vmovdqu %xmm0, 48(%esp)
vpaddd %xmm2, %xmm2, %xmm0
vmovdqu %xmm7, 16(%esp)
vmovdqu %xmm0, 64(%esp)
vpaddd %xmm7, %xmm7, %xmm0
vpmuludq %xmm3, %xmm7, %xmm7
vmovdqu %xmm6, 256(%esp)
vpaddd %xmm1, %xmm1, %xmm6
vmovdqu %xmm0, 80(%esp)
vpaddd %xmm4, %xmm4, %xmm0
vpmuludq %xmm5, %xmm0, %xmm0
vpmuludq %xmm7, %xmm6, %xmm7
vpaddq %xmm7, %xmm0, %xmm0
vpmuludq %xmm2, %xmm2, %xmm7
vpaddq %xmm7, %xmm0, %xmm0
vmovdqu %xmm0, 96(%esp)
vpmuludq %xmm3, %xmm1, %xmm0
vmovdqu %xmm1, (%esp)
vpmuludq %xmm0, %xmm1, %xmm1
vpmuludq 48(%esp), %xmm2, %xmm0
vmovdqu 80(%esp), %xmm7
vmovdqu %xmm3, 240(%esp)
vpaddq %xmm0, %xmm1, %xmm1
vpmuludq %xmm4, %xmm7, %xmm0
vpmuludq %xmm3, %xmm4, %xmm3
vpmuludq %xmm2, %xmm7, %xmm2
vpmuludq %xmm3, %xmm6, %xmm3
vpmuludq %xmm5, %xmm6, %xmm6
vpaddq %xmm0, %xmm1, %xmm1
vpmuludq 32(%esp), %xmm5, %xmm0
vpaddq %xmm3, %xmm0, %xmm0
vmovdqu 96(%esp), %xmm7
vpaddq %xmm2, %xmm0, %xmm2
vpsrlq $26, %xmm7, %xmm0
vpaddq %xmm0, %xmm2, %xmm7
vmovdqu %xmm7, 112(%esp)
vmovdqu 16(%esp), %xmm7
vmovdqu 64(%esp), %xmm2
vpmuludq 48(%esp), %xmm7, %xmm3
vpmuludq (%esp), %xmm2, %xmm0
vpaddq %xmm0, %xmm3, %xmm3
vpmuludq %xmm4, %xmm4, %xmm0
vpmuludq %xmm4, %xmm2, %xmm4
vpmuludq %xmm7, %xmm7, %xmm2
vpaddq %xmm0, %xmm3, %xmm3
vpaddq %xmm4, %xmm6, %xmm5
vpsrlq $26, %xmm1, %xmm0
vpaddq %xmm0, %xmm3, %xmm0
vpaddq %xmm2, %xmm5, %xmm6
vmovdqu 112(%esp), %xmm7
vpsrlq $26, %xmm0, %xmm5
vpsrlq $26, %xmm7, %xmm4
vpaddq %xmm4, %xmm6, %xmm4
vpmuludq 240(%esp), %xmm5, %xmm6
vmovdqu 256(%esp), %xmm2
vpand 96(%esp), %xmm2, %xmm3
vpand %xmm2, %xmm1, %xmm1
vpaddq %xmm6, %xmm3, %xmm5
vpsrlq $26, %xmm4, %xmm6
vpand %xmm2, %xmm0, %xmm0
vpaddq %xmm6, %xmm1, %xmm6
vpsrlq $26, %xmm6, %xmm1
vpand %xmm2, %xmm7, %xmm7
vpsrlq $26, %xmm5, %xmm3
vpand %xmm2, %xmm4, %xmm4
vpaddq %xmm1, %xmm0, %xmm0
vpand %xmm2, %xmm5, %xmm1
vpand %xmm2, %xmm6, %xmm6
vpaddq %xmm3, %xmm7, %xmm5
vpunpcklqdq %xmm5, %xmm1, %xmm1
vpunpcklqdq %xmm6, %xmm4, %xmm3
vshufps $136, %xmm3, %xmm1, %xmm4
vmovups %xmm4, 100(%edx)
vmovd %xmm0, 116(%edx)
cmpl $32, %edi
jbe poly1305_init_ext_avx2_6
poly1305_init_ext_avx2_5:
vmovdqu 224(%esp), %xmm3
vmovdqu %xmm2, 256(%esp)
vpunpcklqdq %xmm4, %xmm3, %xmm1
vpunpckhqdq %xmm4, %xmm3, %xmm2
vpsrlq $32, %xmm1, %xmm5
vmovdqu 208(%esp), %xmm4
vpsrlq $32, %xmm2, %xmm3
vpunpcklqdq %xmm0, %xmm4, %xmm4
vpshufd $238, %xmm1, %xmm7
vmovdqu %xmm7, 32(%esp)
vmovdqu %xmm1, (%esp)
vmovdqu %xmm5, 16(%esp)
vpshufd $238, %xmm5, %xmm0
vpshufd $238, %xmm2, %xmm5
vpshufd $238, %xmm3, %xmm6
vpshufd $238, %xmm4, %xmm1
vmovdqu 240(%esp), %xmm7
vmovdqu %xmm5, 64(%esp)
vmovdqu %xmm6, 80(%esp)
vmovdqu %xmm1, 96(%esp)
vpmuludq %xmm7, %xmm5, %xmm5
vpmuludq %xmm7, %xmm6, %xmm6
vpmuludq %xmm7, %xmm1, %xmm1
vpmuludq %xmm7, %xmm0, %xmm7
vmovdqu %xmm5, 112(%esp)
vpmuludq %xmm7, %xmm4, %xmm7
vpmuludq %xmm5, %xmm3, %xmm5
vmovdqu %xmm6, 128(%esp)
vpmuludq %xmm6, %xmm2, %xmm6
vpaddq %xmm5, %xmm7, %xmm7
vpmuludq 16(%esp), %xmm1, %xmm5
vpaddq %xmm6, %xmm7, %xmm7
vmovdqu 32(%esp), %xmm6
vpaddq %xmm5, %xmm7, %xmm7
vpmuludq (%esp), %xmm6, %xmm5
vpaddq %xmm5, %xmm7, %xmm7
vpmuludq %xmm1, %xmm4, %xmm5
vmovdqu %xmm7, 160(%esp)
vpmuludq %xmm6, %xmm3, %xmm7
vmovdqu %xmm0, 48(%esp)
vpmuludq %xmm0, %xmm2, %xmm0
vpaddq %xmm7, %xmm5, %xmm5
vmovdqu 16(%esp), %xmm7
vpaddq %xmm0, %xmm5, %xmm5
vpmuludq 64(%esp), %xmm7, %xmm0
vpaddq %xmm0, %xmm5, %xmm7
vmovdqu (%esp), %xmm0
vpmuludq 80(%esp), %xmm0, %xmm5
vpaddq %xmm5, %xmm7, %xmm7
vpmuludq 112(%esp), %xmm4, %xmm5
vmovdqu %xmm7, 176(%esp)
vpmuludq 128(%esp), %xmm3, %xmm7
vmovdqu %xmm1, 144(%esp)
vpmuludq %xmm1, %xmm2, %xmm1
vpaddq %xmm7, %xmm5, %xmm5
vpaddq %xmm1, %xmm5, %xmm7
vmovdqu 16(%esp), %xmm1
vpmuludq %xmm6, %xmm1, %xmm5
vpaddq %xmm5, %xmm7, %xmm5
vmovdqu 48(%esp), %xmm7
vpmuludq %xmm7, %xmm0, %xmm0
vpaddq %xmm0, %xmm5, %xmm5
vmovdqu 160(%esp), %xmm0
vpsrlq $26, %xmm0, %xmm0
vpaddq %xmm0, %xmm5, %xmm5
vpmuludq %xmm7, %xmm3, %xmm0
vpmuludq 144(%esp), %xmm3, %xmm3
vpmuludq %xmm7, %xmm1, %xmm7
vmovdqu %xmm5, 192(%esp)
vpmuludq %xmm6, %xmm4, %xmm5
vpmuludq 128(%esp), %xmm4, %xmm4
vpaddq %xmm0, %xmm5, %xmm5
vpaddq %xmm3, %xmm4, %xmm3
vpmuludq 64(%esp), %xmm2, %xmm0
vpmuludq %xmm6, %xmm2, %xmm2
vpaddq %xmm0, %xmm5, %xmm5
vpaddq %xmm2, %xmm3, %xmm3
vpmuludq 80(%esp), %xmm1, %xmm0
vpaddq %xmm7, %xmm3, %xmm2
vpaddq %xmm0, %xmm5, %xmm0
vmovdqu (%esp), %xmm5
vpmuludq 96(%esp), %xmm5, %xmm5
vmovdqu (%esp), %xmm4
vpmuludq 64(%esp), %xmm4, %xmm1
vpaddq %xmm5, %xmm0, %xmm5
vpaddq %xmm1, %xmm2, %xmm6
vmovdqu 176(%esp), %xmm0
vpsrlq $26, %xmm0, %xmm0
vpaddq %xmm0, %xmm5, %xmm5
vmovdqu 192(%esp), %xmm0
vpsrlq $26, %xmm5, %xmm4
vpsrlq $26, %xmm0, %xmm7
vpaddq %xmm7, %xmm6, %xmm2
vpmuludq 240(%esp), %xmm4, %xmm6
vmovdqu 256(%esp), %xmm3
vpsrlq $26, %xmm2, %xmm4
vpand 176(%esp), %xmm3, %xmm7
vpand %xmm3, %xmm5, %xmm5
vpand 160(%esp), %xmm3, %xmm1
vpand %xmm3, %xmm0, %xmm0
vpaddq %xmm4, %xmm7, %xmm4
vpaddq %xmm6, %xmm1, %xmm6
vpsrlq $26, %xmm4, %xmm7
vpaddq %xmm7, %xmm5, %xmm7
vpand %xmm3, %xmm6, %xmm5
vpshufd $8, %xmm5, %xmm1
vpsrlq $26, %xmm6, %xmm5
vpaddq %xmm5, %xmm0, %xmm0
vpshufd $8, %xmm0, %xmm6
vpunpckldq %xmm6, %xmm1, %xmm5
vpand %xmm3, %xmm2, %xmm1
vpand %xmm3, %xmm4, %xmm3
vpshufd $8, %xmm1, %xmm2
vpshufd $8, %xmm3, %xmm4
vpunpckldq %xmm4, %xmm2, %xmm0
vpunpcklqdq %xmm0, %xmm5, %xmm1
vpunpckhqdq %xmm0, %xmm5, %xmm3
vpshufd $8, %xmm7, %xmm2
vpshufd $2, %xmm7, %xmm7
vmovdqu %xmm1, 120(%edx)
vmovdqu %xmm3, 140(%edx)
vmovd %xmm2, 136(%edx)
vmovd %xmm7, 156(%edx)
poly1305_init_ext_avx2_6:
movl $0, 176(%edx)
vzeroupper
addl $272, %esp
popl %ebx
popl %edi
popl %esi
ret
FN_END poly1305_init_ext_avx2

