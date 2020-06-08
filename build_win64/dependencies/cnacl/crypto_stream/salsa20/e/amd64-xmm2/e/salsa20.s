
# qhasm: int64 a

# qhasm: int64 arg1

# qhasm: int64 arg2

# qhasm: int64 arg3

# qhasm: int64 arg4

# qhasm: input arg1

# qhasm: input arg2

# qhasm: input arg3

# qhasm: input arg4

# qhasm: int64 k

# qhasm: int64 kbits

# qhasm: int64 iv

# qhasm: int64 i

# qhasm: int64 x

# qhasm: int64 m

# qhasm: int64 out

# qhasm: int64 bytes

# qhasm: int6464 diag0

# qhasm: int6464 diag1

# qhasm: int6464 diag2

# qhasm: int6464 diag3

# qhasm: int6464 a0

# qhasm: int6464 a1

# qhasm: int6464 a2

# qhasm: int6464 a3

# qhasm: int6464 a4

# qhasm: int6464 a5

# qhasm: int6464 a6

# qhasm: int6464 a7

# qhasm: int6464 b0

# qhasm: int6464 b1

# qhasm: int6464 b2

# qhasm: int6464 b3

# qhasm: int6464 b4

# qhasm: int6464 b5

# qhasm: int6464 b6

# qhasm: int6464 b7

# qhasm: int64 in0

# qhasm: int64 in1

# qhasm: int64 in2

# qhasm: int64 in3

# qhasm: int64 in4

# qhasm: int64 in5

# qhasm: int64 in6

# qhasm: int64 in7

# qhasm: int64 in8

# qhasm: int64 in9

# qhasm: int64 in10

# qhasm: int64 in11

# qhasm: int64 in12

# qhasm: int64 in13

# qhasm: int64 in14

# qhasm: int64 in15

# qhasm: stack512 tmp

# qhasm: int64 ctarget

# qhasm: stack64 bytes_backup

# qhasm: enter ECRYPT_keystream_bytes
.text
.p2align 5
.globl _ECRYPT_keystream_bytes
.globl ECRYPT_keystream_bytes
_ECRYPT_keystream_bytes:
ECRYPT_keystream_bytes:
mov %rsp,%r11
and $31,%r11
add $96,%r11
sub %r11,%rsp

# qhasm: x = arg1
# asm 1: mov  <arg1=int64#1,>x=int64#5
# asm 2: mov  <arg1=%rdi,>x=%r8
mov  %rdi,%r8

# qhasm: m = arg2
# asm 1: mov  <arg2=int64#2,>m=int64#2
# asm 2: mov  <arg2=%rsi,>m=%rsi
mov  %rsi,%rsi

# qhasm: out = m
# asm 1: mov  <m=int64#2,>out=int64#1
# asm 2: mov  <m=%rsi,>out=%rdi
mov  %rsi,%rdi

# qhasm: bytes = arg3
# asm 1: mov  <arg3=int64#3,>bytes=int64#3
# asm 2: mov  <arg3=%rdx,>bytes=%rdx
mov  %rdx,%rdx

# qhasm:               unsigned>? bytes - 0
# asm 1: cmp  $0,<bytes=int64#3
# asm 2: cmp  $0,<bytes=%rdx
cmp  $0,%rdx
# comment:fp stack unchanged by jump

# qhasm: goto done if !unsigned>
jbe ._done

# qhasm: a = 0
# asm 1: mov  $0,>a=int64#7
# asm 2: mov  $0,>a=%rax
mov  $0,%rax

# qhasm: i = bytes
# asm 1: mov  <bytes=int64#3,>i=int64#4
# asm 2: mov  <bytes=%rdx,>i=%rcx
mov  %rdx,%rcx

# qhasm: while (i) { *out++ = a; --i }
rep stosb

# qhasm: out -= bytes
# asm 1: sub  <bytes=int64#3,<out=int64#1
# asm 2: sub  <bytes=%rdx,<out=%rdi
sub  %rdx,%rdi
# comment:fp stack unchanged by jump

# qhasm: goto start
jmp ._start

# qhasm: enter ECRYPT_decrypt_bytes
.text
.p2align 5
.globl _ECRYPT_decrypt_bytes
.globl ECRYPT_decrypt_bytes
_ECRYPT_decrypt_bytes:
ECRYPT_decrypt_bytes:
mov %rsp,%r11
and $31,%r11
add $96,%r11
sub %r11,%rsp

# qhasm: x = arg1
# asm 1: mov  <arg1=int64#1,>x=int64#5
# asm 2: mov  <arg1=%rdi,>x=%r8
mov  %rdi,%r8

# qhasm: m = arg2
# asm 1: mov  <arg2=int64#2,>m=int64#2
# asm 2: mov  <arg2=%rsi,>m=%rsi
mov  %rsi,%rsi

# qhasm: out = arg3
# asm 1: mov  <arg3=int64#3,>out=int64#1
# asm 2: mov  <arg3=%rdx,>out=%rdi
mov  %rdx,%rdi

# qhasm: bytes = arg4
# asm 1: mov  <arg4=int64#4,>bytes=int64#3
# asm 2: mov  <arg4=%rcx,>bytes=%rdx
mov  %rcx,%rdx

# qhasm:               unsigned>? bytes - 0
# asm 1: cmp  $0,<bytes=int64#3
# asm 2: cmp  $0,<bytes=%rdx
cmp  $0,%rdx
# comment:fp stack unchanged by jump

# qhasm: goto done if !unsigned>
jbe ._done
# comment:fp stack unchanged by jump

# qhasm: goto start
jmp ._start

# qhasm: enter ECRYPT_encrypt_bytes
.text
.p2align 5
.globl _ECRYPT_encrypt_bytes
.globl ECRYPT_encrypt_bytes
_ECRYPT_encrypt_bytes:
ECRYPT_encrypt_bytes:
mov %rsp,%r11
and $31,%r11
add $96,%r11
sub %r11,%rsp

# qhasm: x = arg1
# asm 1: mov  <arg1=int64#1,>x=int64#5
# asm 2: mov  <arg1=%rdi,>x=%r8
mov  %rdi,%r8

# qhasm: m = arg2
# asm 1: mov  <arg2=int64#2,>m=int64#2
# asm 2: mov  <arg2=%rsi,>m=%rsi
mov  %rsi,%rsi

# qhasm: out = arg3
# asm 1: mov  <arg3=int64#3,>out=int64#1
# asm 2: mov  <arg3=%rdx,>out=%rdi
mov  %rdx,%rdi

# qhasm: bytes = arg4
# asm 1: mov  <arg4=int64#4,>bytes=int64#3
# asm 2: mov  <arg4=%rcx,>bytes=%rdx
mov  %rcx,%rdx

# qhasm:               unsigned>? bytes - 0
# asm 1: cmp  $0,<bytes=int64#3
# asm 2: cmp  $0,<bytes=%rdx
cmp  $0,%rdx
# comment:fp stack unchanged by jump

# qhasm: goto done if !unsigned>
jbe ._done
# comment:fp stack unchanged by fallthrough

# qhasm: start:
._start:

# qhasm: bytesatleast1:
._bytesatleast1:

# qhasm:                   unsigned<? bytes - 64
# asm 1: cmp  $64,<bytes=int64#3
# asm 2: cmp  $64,<bytes=%rdx
cmp  $64,%rdx
# comment:fp stack unchanged by jump

# qhasm:   goto nocopy if !unsigned<
jae ._nocopy

# qhasm:     ctarget = out
# asm 1: mov  <out=int64#1,>ctarget=int64#6
# asm 2: mov  <out=%rdi,>ctarget=%r9
mov  %rdi,%r9

# qhasm:     out = &tmp
# asm 1: leaq <tmp=stack512#1,>out=int64#1
# asm 2: leaq <tmp=32(%rsp),>out=%rdi
leaq 32(%rsp),%rdi

# qhasm:     i = bytes
# asm 1: mov  <bytes=int64#3,>i=int64#4
# asm 2: mov  <bytes=%rdx,>i=%rcx
mov  %rdx,%rcx

# qhasm:     while (i) { *out++ = *m++; --i }
rep movsb

# qhasm:     out = &tmp
# asm 1: leaq <tmp=stack512#1,>out=int64#1
# asm 2: leaq <tmp=32(%rsp),>out=%rdi
leaq 32(%rsp),%rdi

# qhasm:     m = &tmp
# asm 1: leaq <tmp=stack512#1,>m=int64#2
# asm 2: leaq <tmp=32(%rsp),>m=%rsi
leaq 32(%rsp),%rsi
# comment:fp stack unchanged by fallthrough

# qhasm:   nocopy:
._nocopy:

# qhasm: bytes_backup = bytes
# asm 1: movq <bytes=int64#3,>bytes_backup=stack64#1
# asm 2: movq <bytes=%rdx,>bytes_backup=0(%rsp)
movq %rdx,0(%rsp)

# qhasm: diag0 = *(int128 *) (x + 0)
# asm 1: movdqa 0(<x=int64#5),>diag0=int6464#1
# asm 2: movdqa 0(<x=%r8),>diag0=%xmm0
movdqa 0(%r8),%xmm0

# qhasm: diag1 = *(int128 *) (x + 16)
# asm 1: movdqa 16(<x=int64#5),>diag1=int6464#2
# asm 2: movdqa 16(<x=%r8),>diag1=%xmm1
movdqa 16(%r8),%xmm1

# qhasm: diag2 = *(int128 *) (x + 32)
# asm 1: movdqa 32(<x=int64#5),>diag2=int6464#3
# asm 2: movdqa 32(<x=%r8),>diag2=%xmm2
movdqa 32(%r8),%xmm2

# qhasm: diag3 = *(int128 *) (x + 48)
# asm 1: movdqa 48(<x=int64#5),>diag3=int6464#4
# asm 2: movdqa 48(<x=%r8),>diag3=%xmm3
movdqa 48(%r8),%xmm3

# qhasm:             	a0 = diag1
# asm 1: movdqa <diag1=int6464#2,>a0=int6464#5
# asm 2: movdqa <diag1=%xmm1,>a0=%xmm4
movdqa %xmm1,%xmm4

# qhasm: i = 20
# asm 1: mov  $20,>i=int64#3
# asm 2: mov  $20,>i=%rdx
mov  $20,%rdx

# qhasm: mainloop:
._mainloop:

# qhasm: uint32323232	a0 += diag0
# asm 1: paddd <diag0=int6464#1,<a0=int6464#5
# asm 2: paddd <diag0=%xmm0,<a0=%xmm4
paddd %xmm0,%xmm4

# qhasm: 				a1 = diag0
# asm 1: movdqa <diag0=int6464#1,>a1=int6464#6
# asm 2: movdqa <diag0=%xmm0,>a1=%xmm5
movdqa %xmm0,%xmm5

# qhasm:             	b0 = a0
# asm 1: movdqa <a0=int6464#5,>b0=int6464#7
# asm 2: movdqa <a0=%xmm4,>b0=%xmm6
movdqa %xmm4,%xmm6

# qhasm: uint32323232	a0 <<= 7
# asm 1: pslld $7,<a0=int6464#5
# asm 2: pslld $7,<a0=%xmm4
pslld $7,%xmm4

# qhasm: uint32323232	b0 >>= 25
# asm 1: psrld $25,<b0=int6464#7
# asm 2: psrld $25,<b0=%xmm6
psrld $25,%xmm6

# qhasm:                 diag3 ^= a0
# asm 1: pxor  <a0=int6464#5,<diag3=int6464#4
# asm 2: pxor  <a0=%xmm4,<diag3=%xmm3
pxor  %xmm4,%xmm3

# qhasm:                 diag3 ^= b0
# asm 1: pxor  <b0=int6464#7,<diag3=int6464#4
# asm 2: pxor  <b0=%xmm6,<diag3=%xmm3
pxor  %xmm6,%xmm3

# qhasm: uint32323232			a1 += diag3
# asm 1: paddd <diag3=int6464#4,<a1=int6464#6
# asm 2: paddd <diag3=%xmm3,<a1=%xmm5
paddd %xmm3,%xmm5

# qhasm: 						a2 = diag3
# asm 1: movdqa <diag3=int6464#4,>a2=int6464#5
# asm 2: movdqa <diag3=%xmm3,>a2=%xmm4
movdqa %xmm3,%xmm4

# qhasm:             			b1 = a1
# asm 1: movdqa <a1=int6464#6,>b1=int6464#7
# asm 2: movdqa <a1=%xmm5,>b1=%xmm6
movdqa %xmm5,%xmm6

# qhasm: uint32323232			a1 <<= 9
# asm 1: pslld $9,<a1=int6464#6
# asm 2: pslld $9,<a1=%xmm5
pslld $9,%xmm5

# qhasm: uint32323232			b1 >>= 23
# asm 1: psrld $23,<b1=int6464#7
# asm 2: psrld $23,<b1=%xmm6
psrld $23,%xmm6

# qhasm: 				diag2 ^= a1
# asm 1: pxor  <a1=int6464#6,<diag2=int6464#3
# asm 2: pxor  <a1=%xmm5,<diag2=%xmm2
pxor  %xmm5,%xmm2

# qhasm: 		diag3 <<<= 32
# asm 1: pshufd $0x93,<diag3=int6464#4,<diag3=int6464#4
# asm 2: pshufd $0x93,<diag3=%xmm3,<diag3=%xmm3
pshufd $0x93,%xmm3,%xmm3

# qhasm: 				diag2 ^= b1
# asm 1: pxor  <b1=int6464#7,<diag2=int6464#3
# asm 2: pxor  <b1=%xmm6,<diag2=%xmm2
pxor  %xmm6,%xmm2

# qhasm: uint32323232					a2 += diag2
# asm 1: paddd <diag2=int6464#3,<a2=int6464#5
# asm 2: paddd <diag2=%xmm2,<a2=%xmm4
paddd %xmm2,%xmm4

# qhasm: 								a3 = diag2
# asm 1: movdqa <diag2=int6464#3,>a3=int6464#6
# asm 2: movdqa <diag2=%xmm2,>a3=%xmm5
movdqa %xmm2,%xmm5

# qhasm:             					b2 = a2
# asm 1: movdqa <a2=int6464#5,>b2=int6464#7
# asm 2: movdqa <a2=%xmm4,>b2=%xmm6
movdqa %xmm4,%xmm6

# qhasm: uint32323232					a2 <<= 13
# asm 1: pslld $13,<a2=int6464#5
# asm 2: pslld $13,<a2=%xmm4
pslld $13,%xmm4

# qhasm: uint32323232					b2 >>= 19
# asm 1: psrld $19,<b2=int6464#7
# asm 2: psrld $19,<b2=%xmm6
psrld $19,%xmm6

# qhasm: 						diag1 ^= a2
# asm 1: pxor  <a2=int6464#5,<diag1=int6464#2
# asm 2: pxor  <a2=%xmm4,<diag1=%xmm1
pxor  %xmm4,%xmm1

# qhasm: 				diag2 <<<= 64
# asm 1: pshufd $0x4e,<diag2=int6464#3,<diag2=int6464#3
# asm 2: pshufd $0x4e,<diag2=%xmm2,<diag2=%xmm2
pshufd $0x4e,%xmm2,%xmm2

# qhasm: 						diag1 ^= b2
# asm 1: pxor  <b2=int6464#7,<diag1=int6464#2
# asm 2: pxor  <b2=%xmm6,<diag1=%xmm1
pxor  %xmm6,%xmm1

# qhasm: uint32323232							a3 += diag1
# asm 1: paddd <diag1=int6464#2,<a3=int6464#6
# asm 2: paddd <diag1=%xmm1,<a3=%xmm5
paddd %xmm1,%xmm5

# qhasm: 		a4 = diag3
# asm 1: movdqa <diag3=int6464#4,>a4=int6464#5
# asm 2: movdqa <diag3=%xmm3,>a4=%xmm4
movdqa %xmm3,%xmm4

# qhasm:             							b3 = a3
# asm 1: movdqa <a3=int6464#6,>b3=int6464#7
# asm 2: movdqa <a3=%xmm5,>b3=%xmm6
movdqa %xmm5,%xmm6

# qhasm: uint32323232							a3 <<= 18
# asm 1: pslld $18,<a3=int6464#6
# asm 2: pslld $18,<a3=%xmm5
pslld $18,%xmm5

# qhasm: uint32323232							b3 >>= 14
# asm 1: psrld $14,<b3=int6464#7
# asm 2: psrld $14,<b3=%xmm6
psrld $14,%xmm6

# qhasm: 								diag0 ^= a3
# asm 1: pxor  <a3=int6464#6,<diag0=int6464#1
# asm 2: pxor  <a3=%xmm5,<diag0=%xmm0
pxor  %xmm5,%xmm0

# qhasm: 						diag1 <<<= 96
# asm 1: pshufd $0x39,<diag1=int6464#2,<diag1=int6464#2
# asm 2: pshufd $0x39,<diag1=%xmm1,<diag1=%xmm1
pshufd $0x39,%xmm1,%xmm1

# qhasm: 								diag0 ^= b3
# asm 1: pxor  <b3=int6464#7,<diag0=int6464#1
# asm 2: pxor  <b3=%xmm6,<diag0=%xmm0
pxor  %xmm6,%xmm0

# qhasm: uint32323232	a4 += diag0
# asm 1: paddd <diag0=int6464#1,<a4=int6464#5
# asm 2: paddd <diag0=%xmm0,<a4=%xmm4
paddd %xmm0,%xmm4

# qhasm: 				a5 = diag0
# asm 1: movdqa <diag0=int6464#1,>a5=int6464#6
# asm 2: movdqa <diag0=%xmm0,>a5=%xmm5
movdqa %xmm0,%xmm5

# qhasm:             	b4 = a4
# asm 1: movdqa <a4=int6464#5,>b4=int6464#7
# asm 2: movdqa <a4=%xmm4,>b4=%xmm6
movdqa %xmm4,%xmm6

# qhasm: uint32323232	a4 <<= 7
# asm 1: pslld $7,<a4=int6464#5
# asm 2: pslld $7,<a4=%xmm4
pslld $7,%xmm4

# qhasm: uint32323232	b4 >>= 25
# asm 1: psrld $25,<b4=int6464#7
# asm 2: psrld $25,<b4=%xmm6
psrld $25,%xmm6

# qhasm:                 diag1 ^= a4
# asm 1: pxor  <a4=int6464#5,<diag1=int6464#2
# asm 2: pxor  <a4=%xmm4,<diag1=%xmm1
pxor  %xmm4,%xmm1

# qhasm:                 diag1 ^= b4
# asm 1: pxor  <b4=int6464#7,<diag1=int6464#2
# asm 2: pxor  <b4=%xmm6,<diag1=%xmm1
pxor  %xmm6,%xmm1

# qhasm: uint32323232			a5 += diag1
# asm 1: paddd <diag1=int6464#2,<a5=int6464#6
# asm 2: paddd <diag1=%xmm1,<a5=%xmm5
paddd %xmm1,%xmm5

# qhasm: 						a6 = diag1
# asm 1: movdqa <diag1=int6464#2,>a6=int6464#5
# asm 2: movdqa <diag1=%xmm1,>a6=%xmm4
movdqa %xmm1,%xmm4

# qhasm:             			b5 = a5
# asm 1: movdqa <a5=int6464#6,>b5=int6464#7
# asm 2: movdqa <a5=%xmm5,>b5=%xmm6
movdqa %xmm5,%xmm6

# qhasm: uint32323232			a5 <<= 9
# asm 1: pslld $9,<a5=int6464#6
# asm 2: pslld $9,<a5=%xmm5
pslld $9,%xmm5

# qhasm: uint32323232			b5 >>= 23
# asm 1: psrld $23,<b5=int6464#7
# asm 2: psrld $23,<b5=%xmm6
psrld $23,%xmm6

# qhasm: 				diag2 ^= a5
# asm 1: pxor  <a5=int6464#6,<diag2=int6464#3
# asm 2: pxor  <a5=%xmm5,<diag2=%xmm2
pxor  %xmm5,%xmm2

# qhasm: 		diag1 <<<= 32
# asm 1: pshufd $0x93,<diag1=int6464#2,<diag1=int6464#2
# asm 2: pshufd $0x93,<diag1=%xmm1,<diag1=%xmm1
pshufd $0x93,%xmm1,%xmm1

# qhasm: 				diag2 ^= b5
# asm 1: pxor  <b5=int6464#7,<diag2=int6464#3
# asm 2: pxor  <b5=%xmm6,<diag2=%xmm2
pxor  %xmm6,%xmm2

# qhasm: uint32323232					a6 += diag2
# asm 1: paddd <diag2=int6464#3,<a6=int6464#5
# asm 2: paddd <diag2=%xmm2,<a6=%xmm4
paddd %xmm2,%xmm4

# qhasm: 								a7 = diag2
# asm 1: movdqa <diag2=int6464#3,>a7=int6464#6
# asm 2: movdqa <diag2=%xmm2,>a7=%xmm5
movdqa %xmm2,%xmm5

# qhasm:             					b6 = a6
# asm 1: movdqa <a6=int6464#5,>b6=int6464#7
# asm 2: movdqa <a6=%xmm4,>b6=%xmm6
movdqa %xmm4,%xmm6

# qhasm: uint32323232					a6 <<= 13
# asm 1: pslld $13,<a6=int6464#5
# asm 2: pslld $13,<a6=%xmm4
pslld $13,%xmm4

# qhasm: uint32323232					b6 >>= 19
# asm 1: psrld $19,<b6=int6464#7
# asm 2: psrld $19,<b6=%xmm6
psrld $19,%xmm6

# qhasm: 						diag3 ^= a6
# asm 1: pxor  <a6=int6464#5,<diag3=int6464#4
# asm 2: pxor  <a6=%xmm4,<diag3=%xmm3
pxor  %xmm4,%xmm3

# qhasm: 				diag2 <<<= 64
# asm 1: pshufd $0x4e,<diag2=int6464#3,<diag2=int6464#3
# asm 2: pshufd $0x4e,<diag2=%xmm2,<diag2=%xmm2
pshufd $0x4e,%xmm2,%xmm2

# qhasm: 						diag3 ^= b6
# asm 1: pxor  <b6=int6464#7,<diag3=int6464#4
# asm 2: pxor  <b6=%xmm6,<diag3=%xmm3
pxor  %xmm6,%xmm3

# qhasm: uint32323232							a7 += diag3
# asm 1: paddd <diag3=int6464#4,<a7=int6464#6
# asm 2: paddd <diag3=%xmm3,<a7=%xmm5
paddd %xmm3,%xmm5

# qhasm: 		a0 = diag1
# asm 1: movdqa <diag1=int6464#2,>a0=int6464#5
# asm 2: movdqa <diag1=%xmm1,>a0=%xmm4
movdqa %xmm1,%xmm4

# qhasm:             							b7 = a7
# asm 1: movdqa <a7=int6464#6,>b7=int6464#7
# asm 2: movdqa <a7=%xmm5,>b7=%xmm6
movdqa %xmm5,%xmm6

# qhasm: uint32323232							a7 <<= 18
# asm 1: pslld $18,<a7=int6464#6
# asm 2: pslld $18,<a7=%xmm5
pslld $18,%xmm5

# qhasm: uint32323232							b7 >>= 14
# asm 1: psrld $14,<b7=int6464#7
# asm 2: psrld $14,<b7=%xmm6
psrld $14,%xmm6

# qhasm: 								diag0 ^= a7
# asm 1: pxor  <a7=int6464#6,<diag0=int6464#1
# asm 2: pxor  <a7=%xmm5,<diag0=%xmm0
pxor  %xmm5,%xmm0

# qhasm: 						diag3 <<<= 96
# asm 1: pshufd $0x39,<diag3=int6464#4,<diag3=int6464#4
# asm 2: pshufd $0x39,<diag3=%xmm3,<diag3=%xmm3
pshufd $0x39,%xmm3,%xmm3

# qhasm: 								diag0 ^= b7
# asm 1: pxor  <b7=int6464#7,<diag0=int6464#1
# asm 2: pxor  <b7=%xmm6,<diag0=%xmm0
pxor  %xmm6,%xmm0

# qhasm: uint32323232	a0 += diag0
# asm 1: paddd <diag0=int6464#1,<a0=int6464#5
# asm 2: paddd <diag0=%xmm0,<a0=%xmm4
paddd %xmm0,%xmm4

# qhasm: 				a1 = diag0
# asm 1: movdqa <diag0=int6464#1,>a1=int6464#6
# asm 2: movdqa <diag0=%xmm0,>a1=%xmm5
movdqa %xmm0,%xmm5

# qhasm:             	b0 = a0
# asm 1: movdqa <a0=int6464#5,>b0=int6464#7
# asm 2: movdqa <a0=%xmm4,>b0=%xmm6
movdqa %xmm4,%xmm6

# qhasm: uint32323232	a0 <<= 7
# asm 1: pslld $7,<a0=int6464#5
# asm 2: pslld $7,<a0=%xmm4
pslld $7,%xmm4

# qhasm: uint32323232	b0 >>= 25
# asm 1: psrld $25,<b0=int6464#7
# asm 2: psrld $25,<b0=%xmm6
psrld $25,%xmm6

# qhasm:                 diag3 ^= a0
# asm 1: pxor  <a0=int6464#5,<diag3=int6464#4
# asm 2: pxor  <a0=%xmm4,<diag3=%xmm3
pxor  %xmm4,%xmm3

# qhasm:                 diag3 ^= b0
# asm 1: pxor  <b0=int6464#7,<diag3=int6464#4
# asm 2: pxor  <b0=%xmm6,<diag3=%xmm3
pxor  %xmm6,%xmm3

# qhasm: uint32323232			a1 += diag3
# asm 1: paddd <diag3=int6464#4,<a1=int6464#6
# asm 2: paddd <diag3=%xmm3,<a1=%xmm5
paddd %xmm3,%xmm5

# qhasm: 						a2 = diag3
# asm 1: movdqa <diag3=int6464#4,>a2=int6464#5
# asm 2: movdqa <diag3=%xmm3,>a2=%xmm4
movdqa %xmm3,%xmm4

# qhasm:             			b1 = a1
# asm 1: movdqa <a1=int6464#6,>b1=int6464#7
# asm 2: movdqa <a1=%xmm5,>b1=%xmm6
movdqa %xmm5,%xmm6

# qhasm: uint32323232			a1 <<= 9
# asm 1: pslld $9,<a1=int6464#6
# asm 2: pslld $9,<a1=%xmm5
pslld $9,%xmm5

# qhasm: uint32323232			b1 >>= 23
# asm 1: psrld $23,<b1=int6464#7
# asm 2: psrld $23,<b1=%xmm6
psrld $23,%xmm6

# qhasm: 				diag2 ^= a1
# asm 1: pxor  <a1=int6464#6,<diag2=int6464#3
# asm 2: pxor  <a1=%xmm5,<diag2=%xmm2
pxor  %xmm5,%xmm2

# qhasm: 		diag3 <<<= 32
# asm 1: pshufd $0x93,<diag3=int6464#4,<diag3=int6464#4
# asm 2: pshufd $0x93,<diag3=%xmm3,<diag3=%xmm3
pshufd $0x93,%xmm3,%xmm3

# qhasm: 				diag2 ^= b1
# asm 1: pxor  <b1=int6464#7,<diag2=int6464#3
# asm 2: pxor  <b1=%xmm6,<diag2=%xmm2
pxor  %xmm6,%xmm2

# qhasm: uint32323232					a2 += diag2
# asm 1: paddd <diag2=int6464#3,<a2=int6464#5
# asm 2: paddd <diag2=%xmm2,<a2=%xmm4
paddd %xmm2,%xmm4

# qhasm: 								a3 = diag2
# asm 1: movdqa <diag2=int6464#3,>a3=int6464#6
# asm 2: movdqa <diag2=%xmm2,>a3=%xmm5
movdqa %xmm2,%xmm5

# qhasm:             					b2 = a2
# asm 1: movdqa <a2=int6464#5,>b2=int6464#7
# asm 2: movdqa <a2=%xmm4,>b2=%xmm6
movdqa %xmm4,%xmm6

# qhasm: uint32323232					a2 <<= 13
# asm 1: pslld $13,<a2=int6464#5
# asm 2: pslld $13,<a2=%xmm4
pslld $13,%xmm4

# qhasm: uint32323232					b2 >>= 19
# asm 1: psrld $19,<b2=int6464#7
# asm 2: psrld $19,<b2=%xmm6
psrld $19,%xmm6

# qhasm: 						diag1 ^= a2
# asm 1: pxor  <a2=int6464#5,<diag1=int6464#2
# asm 2: pxor  <a2=%xmm4,<diag1=%xmm1
pxor  %xmm4,%xmm1

# qhasm: 				diag2 <<<= 64
# asm 1: pshufd $0x4e,<diag2=int6464#3,<diag2=int6464#3
# asm 2: pshufd $0x4e,<diag2=%xmm2,<diag2=%xmm2
pshufd $0x4e,%xmm2,%xmm2

# qhasm: 						diag1 ^= b2
# asm 1: pxor  <b2=int6464#7,<diag1=int6464#2
# asm 2: pxor  <b2=%xmm6,<diag1=%xmm1
pxor  %xmm6,%xmm1

# qhasm: uint32323232							a3 += diag1
# asm 1: paddd <diag1=int6464#2,<a3=int6464#6
# asm 2: paddd <diag1=%xmm1,<a3=%xmm5
paddd %xmm1,%xmm5

# qhasm: 		a4 = diag3
# asm 1: movdqa <diag3=int6464#4,>a4=int6464#5
# asm 2: movdqa <diag3=%xmm3,>a4=%xmm4
movdqa %xmm3,%xmm4

# qhasm:             							b3 = a3
# asm 1: movdqa <a3=int6464#6,>b3=int6464#7
# asm 2: movdqa <a3=%xmm5,>b3=%xmm6
movdqa %xmm5,%xmm6

# qhasm: uint32323232							a3 <<= 18
# asm 1: pslld $18,<a3=int6464#6
# asm 2: pslld $18,<a3=%xmm5
pslld $18,%xmm5

# qhasm: uint32323232							b3 >>= 14
# asm 1: psrld $14,<b3=int6464#7
# asm 2: psrld $14,<b3=%xmm6
psrld $14,%xmm6

# qhasm: 								diag0 ^= a3
# asm 1: pxor  <a3=int6464#6,<diag0=int6464#1
# asm 2: pxor  <a3=%xmm5,<diag0=%xmm0
pxor  %xmm5,%xmm0

# qhasm: 						diag1 <<<= 96
# asm 1: pshufd $0x39,<diag1=int6464#2,<diag1=int6464#2
# asm 2: pshufd $0x39,<diag1=%xmm1,<diag1=%xmm1
pshufd $0x39,%xmm1,%xmm1

# qhasm: 								diag0 ^= b3
# asm 1: pxor  <b3=int6464#7,<diag0=int6464#1
# asm 2: pxor  <b3=%xmm6,<diag0=%xmm0
pxor  %xmm6,%xmm0

# qhasm: uint32323232	a4 += diag0
# asm 1: paddd <diag0=int6464#1,<a4=int6464#5
# asm 2: paddd <diag0=%xmm0,<a4=%xmm4
paddd %xmm0,%xmm4

# qhasm: 				a5 = diag0
# asm 1: movdqa <diag0=int6464#1,>a5=int6464#6
# asm 2: movdqa <diag0=%xmm0,>a5=%xmm5
movdqa %xmm0,%xmm5

# qhasm:             	b4 = a4
# asm 1: movdqa <a4=int6464#5,>b4=int6464#7
# asm 2: movdqa <a4=%xmm4,>b4=%xmm6
movdqa %xmm4,%xmm6

# qhasm: uint32323232	a4 <<= 7
# asm 1: pslld $7,<a4=int6464#5
# asm 2: pslld $7,<a4=%xmm4
pslld $7,%xmm4

# qhasm: uint32323232	b4 >>= 25
# asm 1: psrld $25,<b4=int6464#7
# asm 2: psrld $25,<b4=%xmm6
psrld $25,%xmm6

# qhasm:                 diag1 ^= a4
# asm 1: pxor  <a4=int6464#5,<diag1=int6464#2
# asm 2: pxor  <a4=%xmm4,<diag1=%xmm1
pxor  %xmm4,%xmm1

# qhasm:                 diag1 ^= b4
# asm 1: pxor  <b4=int6464#7,<diag1=int6464#2
# asm 2: pxor  <b4=%xmm6,<diag1=%xmm1
pxor  %xmm6,%xmm1

# qhasm: uint32323232			a5 += diag1
# asm 1: paddd <diag1=int6464#2,<a5=int6464#6
# asm 2: paddd <diag1=%xmm1,<a5=%xmm5
paddd %xmm1,%xmm5

# qhasm: 						a6 = diag1
# asm 1: movdqa <diag1=int6464#2,>a6=int6464#5
# asm 2: movdqa <diag1=%xmm1,>a6=%xmm4
movdqa %xmm1,%xmm4

# qhasm:             			b5 = a5
# asm 1: movdqa <a5=int6464#6,>b5=int6464#7
# asm 2: movdqa <a5=%xmm5,>b5=%xmm6
movdqa %xmm5,%xmm6

# qhasm: uint32323232			a5 <<= 9
# asm 1: pslld $9,<a5=int6464#6
# asm 2: pslld $9,<a5=%xmm5
pslld $9,%xmm5

# qhasm: uint32323232			b5 >>= 23
# asm 1: psrld $23,<b5=int6464#7
# asm 2: psrld $23,<b5=%xmm6
psrld $23,%xmm6

# qhasm: 				diag2 ^= a5
# asm 1: pxor  <a5=int6464#6,<diag2=int6464#3
# asm 2: pxor  <a5=%xmm5,<diag2=%xmm2
pxor  %xmm5,%xmm2

# qhasm: 		diag1 <<<= 32
# asm 1: pshufd $0x93,<diag1=int6464#2,<diag1=int6464#2
# asm 2: pshufd $0x93,<diag1=%xmm1,<diag1=%xmm1
pshufd $0x93,%xmm1,%xmm1

# qhasm: 				diag2 ^= b5
# asm 1: pxor  <b5=int6464#7,<diag2=int6464#3
# asm 2: pxor  <b5=%xmm6,<diag2=%xmm2
pxor  %xmm6,%xmm2

# qhasm: uint32323232					a6 += diag2
# asm 1: paddd <diag2=int6464#3,<a6=int6464#5
# asm 2: paddd <diag2=%xmm2,<a6=%xmm4
paddd %xmm2,%xmm4

# qhasm: 								a7 = diag2
# asm 1: movdqa <diag2=int6464#3,>a7=int6464#6
# asm 2: movdqa <diag2=%xmm2,>a7=%xmm5
movdqa %xmm2,%xmm5

# qhasm:             					b6 = a6
# asm 1: movdqa <a6=int6464#5,>b6=int6464#7
# asm 2: movdqa <a6=%xmm4,>b6=%xmm6
movdqa %xmm4,%xmm6

# qhasm: uint32323232					a6 <<= 13
# asm 1: pslld $13,<a6=int6464#5
# asm 2: pslld $13,<a6=%xmm4
pslld $13,%xmm4

# qhasm: uint32323232					b6 >>= 19
# asm 1: psrld $19,<b6=int6464#7
# asm 2: psrld $19,<b6=%xmm6
psrld $19,%xmm6

# qhasm: 						diag3 ^= a6
# asm 1: pxor  <a6=int6464#5,<diag3=int6464#4
# asm 2: pxor  <a6=%xmm4,<diag3=%xmm3
pxor  %xmm4,%xmm3

# qhasm: 				diag2 <<<= 64
# asm 1: pshufd $0x4e,<diag2=int6464#3,<diag2=int6464#3
# asm 2: pshufd $0x4e,<diag2=%xmm2,<diag2=%xmm2
pshufd $0x4e,%xmm2,%xmm2

# qhasm: 						diag3 ^= b6
# asm 1: pxor  <b6=int6464#7,<diag3=int6464#4
# asm 2: pxor  <b6=%xmm6,<diag3=%xmm3
pxor  %xmm6,%xmm3

# qhasm:                  unsigned>? i -= 4
# asm 1: sub  $4,<i=int64#3
# asm 2: sub  $4,<i=%rdx
sub  $4,%rdx

# qhasm: uint32323232							a7 += diag3
# asm 1: paddd <diag3=int6464#4,<a7=int6464#6
# asm 2: paddd <diag3=%xmm3,<a7=%xmm5
paddd %xmm3,%xmm5

# qhasm: 		a0 = diag1
# asm 1: movdqa <diag1=int6464#2,>a0=int6464#5
# asm 2: movdqa <diag1=%xmm1,>a0=%xmm4
movdqa %xmm1,%xmm4

# qhasm:             							b7 = a7
# asm 1: movdqa <a7=int6464#6,>b7=int6464#7
# asm 2: movdqa <a7=%xmm5,>b7=%xmm6
movdqa %xmm5,%xmm6

# qhasm: uint32323232							a7 <<= 18
# asm 1: pslld $18,<a7=int6464#6
# asm 2: pslld $18,<a7=%xmm5
pslld $18,%xmm5

# qhasm: uint32323232							b7 >>= 14
# asm 1: psrld $14,<b7=int6464#7
# asm 2: psrld $14,<b7=%xmm6
psrld $14,%xmm6

# qhasm: 								diag0 ^= a7
# asm 1: pxor  <a7=int6464#6,<diag0=int6464#1
# asm 2: pxor  <a7=%xmm5,<diag0=%xmm0
pxor  %xmm5,%xmm0

# qhasm: 						diag3 <<<= 96
# asm 1: pshufd $0x39,<diag3=int6464#4,<diag3=int6464#4
# asm 2: pshufd $0x39,<diag3=%xmm3,<diag3=%xmm3
pshufd $0x39,%xmm3,%xmm3

# qhasm: 								diag0 ^= b7
# asm 1: pxor  <b7=int6464#7,<diag0=int6464#1
# asm 2: pxor  <b7=%xmm6,<diag0=%xmm0
pxor  %xmm6,%xmm0
# comment:fp stack unchanged by jump

# qhasm: goto mainloop if unsigned>
ja ._mainloop

# qhasm: uint32323232 diag0 += *(int128 *) (x + 0)
# asm 1: paddd 0(<x=int64#5),<diag0=int6464#1
# asm 2: paddd 0(<x=%r8),<diag0=%xmm0
paddd 0(%r8),%xmm0

# qhasm: uint32323232 diag1 += *(int128 *) (x + 16)
# asm 1: paddd 16(<x=int64#5),<diag1=int6464#2
# asm 2: paddd 16(<x=%r8),<diag1=%xmm1
paddd 16(%r8),%xmm1

# qhasm: uint32323232 diag2 += *(int128 *) (x + 32)
# asm 1: paddd 32(<x=int64#5),<diag2=int6464#3
# asm 2: paddd 32(<x=%r8),<diag2=%xmm2
paddd 32(%r8),%xmm2

# qhasm: uint32323232 diag3 += *(int128 *) (x + 48)
# asm 1: paddd 48(<x=int64#5),<diag3=int6464#4
# asm 2: paddd 48(<x=%r8),<diag3=%xmm3
paddd 48(%r8),%xmm3

# qhasm: in0 = diag0
# asm 1: movd   <diag0=int6464#1,>in0=int64#3
# asm 2: movd   <diag0=%xmm0,>in0=%rdx
movd   %xmm0,%rdx

# qhasm: in12 = diag1
# asm 1: movd   <diag1=int6464#2,>in12=int64#4
# asm 2: movd   <diag1=%xmm1,>in12=%rcx
movd   %xmm1,%rcx

# qhasm: in8 = diag2
# asm 1: movd   <diag2=int6464#3,>in8=int64#7
# asm 2: movd   <diag2=%xmm2,>in8=%rax
movd   %xmm2,%rax

# qhasm: in4 = diag3
# asm 1: movd   <diag3=int6464#4,>in4=int64#8
# asm 2: movd   <diag3=%xmm3,>in4=%r10
movd   %xmm3,%r10

# qhasm: diag0 <<<= 96
# asm 1: pshufd $0x39,<diag0=int6464#1,<diag0=int6464#1
# asm 2: pshufd $0x39,<diag0=%xmm0,<diag0=%xmm0
pshufd $0x39,%xmm0,%xmm0

# qhasm: diag1 <<<= 96
# asm 1: pshufd $0x39,<diag1=int6464#2,<diag1=int6464#2
# asm 2: pshufd $0x39,<diag1=%xmm1,<diag1=%xmm1
pshufd $0x39,%xmm1,%xmm1

# qhasm: diag2 <<<= 96
# asm 1: pshufd $0x39,<diag2=int6464#3,<diag2=int6464#3
# asm 2: pshufd $0x39,<diag2=%xmm2,<diag2=%xmm2
pshufd $0x39,%xmm2,%xmm2

# qhasm: diag3 <<<= 96
# asm 1: pshufd $0x39,<diag3=int6464#4,<diag3=int6464#4
# asm 2: pshufd $0x39,<diag3=%xmm3,<diag3=%xmm3
pshufd $0x39,%xmm3,%xmm3

# qhasm: (uint32) in0 ^= *(uint32 *) (m + 0)
# asm 1: xorl 0(<m=int64#2),<in0=int64#3d
# asm 2: xorl 0(<m=%rsi),<in0=%edx
xorl 0(%rsi),%edx

# qhasm: (uint32) in12 ^= *(uint32 *) (m + 48)
# asm 1: xorl 48(<m=int64#2),<in12=int64#4d
# asm 2: xorl 48(<m=%rsi),<in12=%ecx
xorl 48(%rsi),%ecx

# qhasm: (uint32) in8 ^= *(uint32 *) (m + 32)
# asm 1: xorl 32(<m=int64#2),<in8=int64#7d
# asm 2: xorl 32(<m=%rsi),<in8=%eax
xorl 32(%rsi),%eax

# qhasm: (uint32) in4 ^= *(uint32 *) (m + 16)
# asm 1: xorl 16(<m=int64#2),<in4=int64#8d
# asm 2: xorl 16(<m=%rsi),<in4=%r10d
xorl 16(%rsi),%r10d

# qhasm: *(uint32 *) (out + 0) = in0
# asm 1: movl   <in0=int64#3d,0(<out=int64#1)
# asm 2: movl   <in0=%edx,0(<out=%rdi)
movl   %edx,0(%rdi)

# qhasm: *(uint32 *) (out + 48) = in12
# asm 1: movl   <in12=int64#4d,48(<out=int64#1)
# asm 2: movl   <in12=%ecx,48(<out=%rdi)
movl   %ecx,48(%rdi)

# qhasm: *(uint32 *) (out + 32) = in8
# asm 1: movl   <in8=int64#7d,32(<out=int64#1)
# asm 2: movl   <in8=%eax,32(<out=%rdi)
movl   %eax,32(%rdi)

# qhasm: *(uint32 *) (out + 16) = in4
# asm 1: movl   <in4=int64#8d,16(<out=int64#1)
# asm 2: movl   <in4=%r10d,16(<out=%rdi)
movl   %r10d,16(%rdi)

# qhasm: in5 = diag0
# asm 1: movd   <diag0=int6464#1,>in5=int64#3
# asm 2: movd   <diag0=%xmm0,>in5=%rdx
movd   %xmm0,%rdx

# qhasm: in1 = diag1
# asm 1: movd   <diag1=int6464#2,>in1=int64#4
# asm 2: movd   <diag1=%xmm1,>in1=%rcx
movd   %xmm1,%rcx

# qhasm: in13 = diag2
# asm 1: movd   <diag2=int6464#3,>in13=int64#7
# asm 2: movd   <diag2=%xmm2,>in13=%rax
movd   %xmm2,%rax

# qhasm: in9 = diag3
# asm 1: movd   <diag3=int6464#4,>in9=int64#8
# asm 2: movd   <diag3=%xmm3,>in9=%r10
movd   %xmm3,%r10

# qhasm: diag0 <<<= 96
# asm 1: pshufd $0x39,<diag0=int6464#1,<diag0=int6464#1
# asm 2: pshufd $0x39,<diag0=%xmm0,<diag0=%xmm0
pshufd $0x39,%xmm0,%xmm0

# qhasm: diag1 <<<= 96
# asm 1: pshufd $0x39,<diag1=int6464#2,<diag1=int6464#2
# asm 2: pshufd $0x39,<diag1=%xmm1,<diag1=%xmm1
pshufd $0x39,%xmm1,%xmm1

# qhasm: diag2 <<<= 96
# asm 1: pshufd $0x39,<diag2=int6464#3,<diag2=int6464#3
# asm 2: pshufd $0x39,<diag2=%xmm2,<diag2=%xmm2
pshufd $0x39,%xmm2,%xmm2

# qhasm: diag3 <<<= 96
# asm 1: pshufd $0x39,<diag3=int6464#4,<diag3=int6464#4
# asm 2: pshufd $0x39,<diag3=%xmm3,<diag3=%xmm3
pshufd $0x39,%xmm3,%xmm3

# qhasm: (uint32) in5 ^= *(uint32 *) (m + 20)
# asm 1: xorl 20(<m=int64#2),<in5=int64#3d
# asm 2: xorl 20(<m=%rsi),<in5=%edx
xorl 20(%rsi),%edx

# qhasm: (uint32) in1 ^= *(uint32 *) (m + 4)
# asm 1: xorl 4(<m=int64#2),<in1=int64#4d
# asm 2: xorl 4(<m=%rsi),<in1=%ecx
xorl 4(%rsi),%ecx

# qhasm: (uint32) in13 ^= *(uint32 *) (m + 52)
# asm 1: xorl 52(<m=int64#2),<in13=int64#7d
# asm 2: xorl 52(<m=%rsi),<in13=%eax
xorl 52(%rsi),%eax

# qhasm: (uint32) in9 ^= *(uint32 *) (m + 36)
# asm 1: xorl 36(<m=int64#2),<in9=int64#8d
# asm 2: xorl 36(<m=%rsi),<in9=%r10d
xorl 36(%rsi),%r10d

# qhasm: *(uint32 *) (out + 20) = in5
# asm 1: movl   <in5=int64#3d,20(<out=int64#1)
# asm 2: movl   <in5=%edx,20(<out=%rdi)
movl   %edx,20(%rdi)

# qhasm: *(uint32 *) (out + 4) = in1
# asm 1: movl   <in1=int64#4d,4(<out=int64#1)
# asm 2: movl   <in1=%ecx,4(<out=%rdi)
movl   %ecx,4(%rdi)

# qhasm: *(uint32 *) (out + 52) = in13
# asm 1: movl   <in13=int64#7d,52(<out=int64#1)
# asm 2: movl   <in13=%eax,52(<out=%rdi)
movl   %eax,52(%rdi)

# qhasm: *(uint32 *) (out + 36) = in9
# asm 1: movl   <in9=int64#8d,36(<out=int64#1)
# asm 2: movl   <in9=%r10d,36(<out=%rdi)
movl   %r10d,36(%rdi)

# qhasm: in10 = diag0
# asm 1: movd   <diag0=int6464#1,>in10=int64#3
# asm 2: movd   <diag0=%xmm0,>in10=%rdx
movd   %xmm0,%rdx

# qhasm: in6 = diag1
# asm 1: movd   <diag1=int6464#2,>in6=int64#4
# asm 2: movd   <diag1=%xmm1,>in6=%rcx
movd   %xmm1,%rcx

# qhasm: in2 = diag2
# asm 1: movd   <diag2=int6464#3,>in2=int64#7
# asm 2: movd   <diag2=%xmm2,>in2=%rax
movd   %xmm2,%rax

# qhasm: in14 = diag3
# asm 1: movd   <diag3=int6464#4,>in14=int64#8
# asm 2: movd   <diag3=%xmm3,>in14=%r10
movd   %xmm3,%r10

# qhasm: diag0 <<<= 96
# asm 1: pshufd $0x39,<diag0=int6464#1,<diag0=int6464#1
# asm 2: pshufd $0x39,<diag0=%xmm0,<diag0=%xmm0
pshufd $0x39,%xmm0,%xmm0

# qhasm: diag1 <<<= 96
# asm 1: pshufd $0x39,<diag1=int6464#2,<diag1=int6464#2
# asm 2: pshufd $0x39,<diag1=%xmm1,<diag1=%xmm1
pshufd $0x39,%xmm1,%xmm1

# qhasm: diag2 <<<= 96
# asm 1: pshufd $0x39,<diag2=int6464#3,<diag2=int6464#3
# asm 2: pshufd $0x39,<diag2=%xmm2,<diag2=%xmm2
pshufd $0x39,%xmm2,%xmm2

# qhasm: diag3 <<<= 96
# asm 1: pshufd $0x39,<diag3=int6464#4,<diag3=int6464#4
# asm 2: pshufd $0x39,<diag3=%xmm3,<diag3=%xmm3
pshufd $0x39,%xmm3,%xmm3

# qhasm: (uint32) in10 ^= *(uint32 *) (m + 40)
# asm 1: xorl 40(<m=int64#2),<in10=int64#3d
# asm 2: xorl 40(<m=%rsi),<in10=%edx
xorl 40(%rsi),%edx

# qhasm: (uint32) in6 ^= *(uint32 *) (m + 24)
# asm 1: xorl 24(<m=int64#2),<in6=int64#4d
# asm 2: xorl 24(<m=%rsi),<in6=%ecx
xorl 24(%rsi),%ecx

# qhasm: (uint32) in2 ^= *(uint32 *) (m + 8)
# asm 1: xorl 8(<m=int64#2),<in2=int64#7d
# asm 2: xorl 8(<m=%rsi),<in2=%eax
xorl 8(%rsi),%eax

# qhasm: (uint32) in14 ^= *(uint32 *) (m + 56)
# asm 1: xorl 56(<m=int64#2),<in14=int64#8d
# asm 2: xorl 56(<m=%rsi),<in14=%r10d
xorl 56(%rsi),%r10d

# qhasm: *(uint32 *) (out + 40) = in10
# asm 1: movl   <in10=int64#3d,40(<out=int64#1)
# asm 2: movl   <in10=%edx,40(<out=%rdi)
movl   %edx,40(%rdi)

# qhasm: *(uint32 *) (out + 24) = in6
# asm 1: movl   <in6=int64#4d,24(<out=int64#1)
# asm 2: movl   <in6=%ecx,24(<out=%rdi)
movl   %ecx,24(%rdi)

# qhasm: *(uint32 *) (out + 8) = in2
# asm 1: movl   <in2=int64#7d,8(<out=int64#1)
# asm 2: movl   <in2=%eax,8(<out=%rdi)
movl   %eax,8(%rdi)

# qhasm: *(uint32 *) (out + 56) = in14
# asm 1: movl   <in14=int64#8d,56(<out=int64#1)
# asm 2: movl   <in14=%r10d,56(<out=%rdi)
movl   %r10d,56(%rdi)

# qhasm: in15 = diag0
# asm 1: movd   <diag0=int6464#1,>in15=int64#3
# asm 2: movd   <diag0=%xmm0,>in15=%rdx
movd   %xmm0,%rdx

# qhasm: in11 = diag1
# asm 1: movd   <diag1=int6464#2,>in11=int64#4
# asm 2: movd   <diag1=%xmm1,>in11=%rcx
movd   %xmm1,%rcx

# qhasm: in7 = diag2
# asm 1: movd   <diag2=int6464#3,>in7=int64#7
# asm 2: movd   <diag2=%xmm2,>in7=%rax
movd   %xmm2,%rax

# qhasm: in3 = diag3
# asm 1: movd   <diag3=int6464#4,>in3=int64#8
# asm 2: movd   <diag3=%xmm3,>in3=%r10
movd   %xmm3,%r10

# qhasm: (uint32) in15 ^= *(uint32 *) (m + 60)
# asm 1: xorl 60(<m=int64#2),<in15=int64#3d
# asm 2: xorl 60(<m=%rsi),<in15=%edx
xorl 60(%rsi),%edx

# qhasm: (uint32) in11 ^= *(uint32 *) (m + 44)
# asm 1: xorl 44(<m=int64#2),<in11=int64#4d
# asm 2: xorl 44(<m=%rsi),<in11=%ecx
xorl 44(%rsi),%ecx

# qhasm: (uint32) in7 ^= *(uint32 *) (m + 28)
# asm 1: xorl 28(<m=int64#2),<in7=int64#7d
# asm 2: xorl 28(<m=%rsi),<in7=%eax
xorl 28(%rsi),%eax

# qhasm: (uint32) in3 ^= *(uint32 *) (m + 12)
# asm 1: xorl 12(<m=int64#2),<in3=int64#8d
# asm 2: xorl 12(<m=%rsi),<in3=%r10d
xorl 12(%rsi),%r10d

# qhasm: *(uint32 *) (out + 60) = in15
# asm 1: movl   <in15=int64#3d,60(<out=int64#1)
# asm 2: movl   <in15=%edx,60(<out=%rdi)
movl   %edx,60(%rdi)

# qhasm: *(uint32 *) (out + 44) = in11
# asm 1: movl   <in11=int64#4d,44(<out=int64#1)
# asm 2: movl   <in11=%ecx,44(<out=%rdi)
movl   %ecx,44(%rdi)

# qhasm: *(uint32 *) (out + 28) = in7
# asm 1: movl   <in7=int64#7d,28(<out=int64#1)
# asm 2: movl   <in7=%eax,28(<out=%rdi)
movl   %eax,28(%rdi)

# qhasm: *(uint32 *) (out + 12) = in3
# asm 1: movl   <in3=int64#8d,12(<out=int64#1)
# asm 2: movl   <in3=%r10d,12(<out=%rdi)
movl   %r10d,12(%rdi)

# qhasm: bytes = bytes_backup
# asm 1: movq <bytes_backup=stack64#1,>bytes=int64#3
# asm 2: movq <bytes_backup=0(%rsp),>bytes=%rdx
movq 0(%rsp),%rdx

# qhasm:   in8 = *(uint32 *) (x + 32)
# asm 1: movl   32(<x=int64#5),>in8=int64#4d
# asm 2: movl   32(<x=%r8),>in8=%ecx
movl   32(%r8),%ecx

# qhasm:   in9 = *(uint32 *) (x + 52)
# asm 1: movl   52(<x=int64#5),>in9=int64#7d
# asm 2: movl   52(<x=%r8),>in9=%eax
movl   52(%r8),%eax

# qhasm:   in8 += 1
# asm 1: add  $1,<in8=int64#4
# asm 2: add  $1,<in8=%rcx
add  $1,%rcx

# qhasm:   in9 <<= 32
# asm 1: shl  $32,<in9=int64#7
# asm 2: shl  $32,<in9=%rax
shl  $32,%rax

# qhasm:   in8 += in9
# asm 1: add  <in9=int64#7,<in8=int64#4
# asm 2: add  <in9=%rax,<in8=%rcx
add  %rax,%rcx

# qhasm:   *(uint32 *) (x + 32) = in8
# asm 1: movl   <in8=int64#4d,32(<x=int64#5)
# asm 2: movl   <in8=%ecx,32(<x=%r8)
movl   %ecx,32(%r8)

# qhasm:   (uint64) in8 >>= 32
# asm 1: shr  $32,<in8=int64#4
# asm 2: shr  $32,<in8=%rcx
shr  $32,%rcx

# qhasm:   *(uint32 *) (x + 52) = in8
# asm 1: movl   <in8=int64#4d,52(<x=int64#5)
# asm 2: movl   <in8=%ecx,52(<x=%r8)
movl   %ecx,52(%r8)

# qhasm:                          unsigned>? unsigned<? bytes - 64
# asm 1: cmp  $64,<bytes=int64#3
# asm 2: cmp  $64,<bytes=%rdx
cmp  $64,%rdx
# comment:fp stack unchanged by jump

# qhasm:   goto bytesatleast65 if unsigned>
ja ._bytesatleast65
# comment:fp stack unchanged by jump

# qhasm:     goto bytesatleast64 if !unsigned<
jae ._bytesatleast64

# qhasm:       m = out
# asm 1: mov  <out=int64#1,>m=int64#2
# asm 2: mov  <out=%rdi,>m=%rsi
mov  %rdi,%rsi

# qhasm:       out = ctarget
# asm 1: mov  <ctarget=int64#6,>out=int64#1
# asm 2: mov  <ctarget=%r9,>out=%rdi
mov  %r9,%rdi

# qhasm:       i = bytes
# asm 1: mov  <bytes=int64#3,>i=int64#4
# asm 2: mov  <bytes=%rdx,>i=%rcx
mov  %rdx,%rcx

# qhasm:       while (i) { *out++ = *m++; --i }
rep movsb
# comment:fp stack unchanged by fallthrough

# qhasm:     bytesatleast64:
._bytesatleast64:
# comment:fp stack unchanged by fallthrough

# qhasm:     done:
._done:

# qhasm:     leave
add %r11,%rsp
mov %rdi,%rax
mov %rsi,%rdx
ret

# qhasm:   bytesatleast65:
._bytesatleast65:

# qhasm:   bytes -= 64
# asm 1: sub  $64,<bytes=int64#3
# asm 2: sub  $64,<bytes=%rdx
sub  $64,%rdx

# qhasm:   out += 64
# asm 1: add  $64,<out=int64#1
# asm 2: add  $64,<out=%rdi
add  $64,%rdi

# qhasm:   m += 64
# asm 1: add  $64,<m=int64#2
# asm 2: add  $64,<m=%rsi
add  $64,%rsi
# comment:fp stack unchanged by jump

# qhasm: goto bytesatleast1
jmp ._bytesatleast1

# qhasm: enter ECRYPT_init
.text
.p2align 5
.globl _ECRYPT_init
.globl ECRYPT_init
_ECRYPT_init:
ECRYPT_init:
mov %rsp,%r11
and $31,%r11
add $96,%r11
sub %r11,%rsp

# qhasm: leave
add %r11,%rsp
mov %rdi,%rax
mov %rsi,%rdx
ret

# qhasm: enter ECRYPT_keysetup
.text
.p2align 5
.globl _ECRYPT_keysetup
.globl ECRYPT_keysetup
_ECRYPT_keysetup:
ECRYPT_keysetup:
mov %rsp,%r11
and $31,%r11
add $96,%r11
sub %r11,%rsp

# qhasm:   k = arg2
# asm 1: mov  <arg2=int64#2,>k=int64#2
# asm 2: mov  <arg2=%rsi,>k=%rsi
mov  %rsi,%rsi

# qhasm:   kbits = arg3
# asm 1: mov  <arg3=int64#3,>kbits=int64#3
# asm 2: mov  <arg3=%rdx,>kbits=%rdx
mov  %rdx,%rdx

# qhasm:   x = arg1
# asm 1: mov  <arg1=int64#1,>x=int64#1
# asm 2: mov  <arg1=%rdi,>x=%rdi
mov  %rdi,%rdi

# qhasm:   in1 = *(uint32 *) (k + 0)
# asm 1: movl   0(<k=int64#2),>in1=int64#5d
# asm 2: movl   0(<k=%rsi),>in1=%r8d
movl   0(%rsi),%r8d

# qhasm:   in2 = *(uint32 *) (k + 4)
# asm 1: movl   4(<k=int64#2),>in2=int64#6d
# asm 2: movl   4(<k=%rsi),>in2=%r9d
movl   4(%rsi),%r9d

# qhasm:   in3 = *(uint32 *) (k + 8)
# asm 1: movl   8(<k=int64#2),>in3=int64#7d
# asm 2: movl   8(<k=%rsi),>in3=%eax
movl   8(%rsi),%eax

# qhasm:   in4 = *(uint32 *) (k + 12)
# asm 1: movl   12(<k=int64#2),>in4=int64#8d
# asm 2: movl   12(<k=%rsi),>in4=%r10d
movl   12(%rsi),%r10d

# qhasm:   *(uint32 *) (x + 20) = in1
# asm 1: movl   <in1=int64#5d,20(<x=int64#1)
# asm 2: movl   <in1=%r8d,20(<x=%rdi)
movl   %r8d,20(%rdi)

# qhasm:   *(uint32 *) (x + 40) = in2
# asm 1: movl   <in2=int64#6d,40(<x=int64#1)
# asm 2: movl   <in2=%r9d,40(<x=%rdi)
movl   %r9d,40(%rdi)

# qhasm:   *(uint32 *) (x + 60) = in3
# asm 1: movl   <in3=int64#7d,60(<x=int64#1)
# asm 2: movl   <in3=%eax,60(<x=%rdi)
movl   %eax,60(%rdi)

# qhasm:   *(uint32 *) (x + 48) = in4
# asm 1: movl   <in4=int64#8d,48(<x=int64#1)
# asm 2: movl   <in4=%r10d,48(<x=%rdi)
movl   %r10d,48(%rdi)

# qhasm:                    unsigned<? kbits - 256
# asm 1: cmp  $256,<kbits=int64#3
# asm 2: cmp  $256,<kbits=%rdx
cmp  $256,%rdx
# comment:fp stack unchanged by jump

# qhasm:   goto kbits128 if unsigned<
jb ._kbits128

# qhasm:   kbits256:
._kbits256:

# qhasm:     in11 = *(uint32 *) (k + 16)
# asm 1: movl   16(<k=int64#2),>in11=int64#3d
# asm 2: movl   16(<k=%rsi),>in11=%edx
movl   16(%rsi),%edx

# qhasm:     in12 = *(uint32 *) (k + 20)
# asm 1: movl   20(<k=int64#2),>in12=int64#4d
# asm 2: movl   20(<k=%rsi),>in12=%ecx
movl   20(%rsi),%ecx

# qhasm:     in13 = *(uint32 *) (k + 24)
# asm 1: movl   24(<k=int64#2),>in13=int64#5d
# asm 2: movl   24(<k=%rsi),>in13=%r8d
movl   24(%rsi),%r8d

# qhasm:     in14 = *(uint32 *) (k + 28)
# asm 1: movl   28(<k=int64#2),>in14=int64#2d
# asm 2: movl   28(<k=%rsi),>in14=%esi
movl   28(%rsi),%esi

# qhasm:     *(uint32 *) (x + 28) = in11
# asm 1: movl   <in11=int64#3d,28(<x=int64#1)
# asm 2: movl   <in11=%edx,28(<x=%rdi)
movl   %edx,28(%rdi)

# qhasm:     *(uint32 *) (x + 16) = in12
# asm 1: movl   <in12=int64#4d,16(<x=int64#1)
# asm 2: movl   <in12=%ecx,16(<x=%rdi)
movl   %ecx,16(%rdi)

# qhasm:     *(uint32 *) (x + 36) = in13
# asm 1: movl   <in13=int64#5d,36(<x=int64#1)
# asm 2: movl   <in13=%r8d,36(<x=%rdi)
movl   %r8d,36(%rdi)

# qhasm:     *(uint32 *) (x + 56) = in14
# asm 1: movl   <in14=int64#2d,56(<x=int64#1)
# asm 2: movl   <in14=%esi,56(<x=%rdi)
movl   %esi,56(%rdi)

# qhasm:     in0 = 1634760805
# asm 1: mov  $1634760805,>in0=int64#2
# asm 2: mov  $1634760805,>in0=%rsi
mov  $1634760805,%rsi

# qhasm:     in5 = 857760878
# asm 1: mov  $857760878,>in5=int64#3
# asm 2: mov  $857760878,>in5=%rdx
mov  $857760878,%rdx

# qhasm:     in10 = 2036477234
# asm 1: mov  $2036477234,>in10=int64#4
# asm 2: mov  $2036477234,>in10=%rcx
mov  $2036477234,%rcx

# qhasm:     in15 = 1797285236
# asm 1: mov  $1797285236,>in15=int64#5
# asm 2: mov  $1797285236,>in15=%r8
mov  $1797285236,%r8

# qhasm:     *(uint32 *) (x + 0) = in0
# asm 1: movl   <in0=int64#2d,0(<x=int64#1)
# asm 2: movl   <in0=%esi,0(<x=%rdi)
movl   %esi,0(%rdi)

# qhasm:     *(uint32 *) (x + 4) = in5
# asm 1: movl   <in5=int64#3d,4(<x=int64#1)
# asm 2: movl   <in5=%edx,4(<x=%rdi)
movl   %edx,4(%rdi)

# qhasm:     *(uint32 *) (x + 8) = in10
# asm 1: movl   <in10=int64#4d,8(<x=int64#1)
# asm 2: movl   <in10=%ecx,8(<x=%rdi)
movl   %ecx,8(%rdi)

# qhasm:     *(uint32 *) (x + 12) = in15
# asm 1: movl   <in15=int64#5d,12(<x=int64#1)
# asm 2: movl   <in15=%r8d,12(<x=%rdi)
movl   %r8d,12(%rdi)
# comment:fp stack unchanged by jump

# qhasm:   goto keysetupdone
jmp ._keysetupdone

# qhasm:   kbits128:
._kbits128:

# qhasm:     in11 = *(uint32 *) (k + 0)
# asm 1: movl   0(<k=int64#2),>in11=int64#3d
# asm 2: movl   0(<k=%rsi),>in11=%edx
movl   0(%rsi),%edx

# qhasm:     in12 = *(uint32 *) (k + 4)
# asm 1: movl   4(<k=int64#2),>in12=int64#4d
# asm 2: movl   4(<k=%rsi),>in12=%ecx
movl   4(%rsi),%ecx

# qhasm:     in13 = *(uint32 *) (k + 8)
# asm 1: movl   8(<k=int64#2),>in13=int64#5d
# asm 2: movl   8(<k=%rsi),>in13=%r8d
movl   8(%rsi),%r8d

# qhasm:     in14 = *(uint32 *) (k + 12)
# asm 1: movl   12(<k=int64#2),>in14=int64#2d
# asm 2: movl   12(<k=%rsi),>in14=%esi
movl   12(%rsi),%esi

# qhasm:     *(uint32 *) (x + 28) = in11
# asm 1: movl   <in11=int64#3d,28(<x=int64#1)
# asm 2: movl   <in11=%edx,28(<x=%rdi)
movl   %edx,28(%rdi)

# qhasm:     *(uint32 *) (x + 16) = in12
# asm 1: movl   <in12=int64#4d,16(<x=int64#1)
# asm 2: movl   <in12=%ecx,16(<x=%rdi)
movl   %ecx,16(%rdi)

# qhasm:     *(uint32 *) (x + 36) = in13
# asm 1: movl   <in13=int64#5d,36(<x=int64#1)
# asm 2: movl   <in13=%r8d,36(<x=%rdi)
movl   %r8d,36(%rdi)

# qhasm:     *(uint32 *) (x + 56) = in14
# asm 1: movl   <in14=int64#2d,56(<x=int64#1)
# asm 2: movl   <in14=%esi,56(<x=%rdi)
movl   %esi,56(%rdi)

# qhasm:     in0 = 1634760805
# asm 1: mov  $1634760805,>in0=int64#2
# asm 2: mov  $1634760805,>in0=%rsi
mov  $1634760805,%rsi

# qhasm:     in5 = 824206446
# asm 1: mov  $824206446,>in5=int64#3
# asm 2: mov  $824206446,>in5=%rdx
mov  $824206446,%rdx

# qhasm:     in10 = 2036477238
# asm 1: mov  $2036477238,>in10=int64#4
# asm 2: mov  $2036477238,>in10=%rcx
mov  $2036477238,%rcx

# qhasm:     in15 = 1797285236
# asm 1: mov  $1797285236,>in15=int64#5
# asm 2: mov  $1797285236,>in15=%r8
mov  $1797285236,%r8

# qhasm:     *(uint32 *) (x + 0) = in0
# asm 1: movl   <in0=int64#2d,0(<x=int64#1)
# asm 2: movl   <in0=%esi,0(<x=%rdi)
movl   %esi,0(%rdi)

# qhasm:     *(uint32 *) (x + 4) = in5
# asm 1: movl   <in5=int64#3d,4(<x=int64#1)
# asm 2: movl   <in5=%edx,4(<x=%rdi)
movl   %edx,4(%rdi)

# qhasm:     *(uint32 *) (x + 8) = in10
# asm 1: movl   <in10=int64#4d,8(<x=int64#1)
# asm 2: movl   <in10=%ecx,8(<x=%rdi)
movl   %ecx,8(%rdi)

# qhasm:     *(uint32 *) (x + 12) = in15
# asm 1: movl   <in15=int64#5d,12(<x=int64#1)
# asm 2: movl   <in15=%r8d,12(<x=%rdi)
movl   %r8d,12(%rdi)

# qhasm:   keysetupdone:
._keysetupdone:

# qhasm: leave
add %r11,%rsp
mov %rdi,%rax
mov %rsi,%rdx
ret

# qhasm: enter ECRYPT_ivsetup
.text
.p2align 5
.globl _ECRYPT_ivsetup
.globl ECRYPT_ivsetup
_ECRYPT_ivsetup:
ECRYPT_ivsetup:
mov %rsp,%r11
and $31,%r11
add $96,%r11
sub %r11,%rsp

# qhasm:   iv = arg2
# asm 1: mov  <arg2=int64#2,>iv=int64#2
# asm 2: mov  <arg2=%rsi,>iv=%rsi
mov  %rsi,%rsi

# qhasm:   x = arg1
# asm 1: mov  <arg1=int64#1,>x=int64#1
# asm 2: mov  <arg1=%rdi,>x=%rdi
mov  %rdi,%rdi

# qhasm:   in6 = *(uint32 *) (iv + 0)
# asm 1: movl   0(<iv=int64#2),>in6=int64#5d
# asm 2: movl   0(<iv=%rsi),>in6=%r8d
movl   0(%rsi),%r8d

# qhasm:   in7 = *(uint32 *) (iv + 4)
# asm 1: movl   4(<iv=int64#2),>in7=int64#2d
# asm 2: movl   4(<iv=%rsi),>in7=%esi
movl   4(%rsi),%esi

# qhasm:   in8 = 0
# asm 1: mov  $0,>in8=int64#6
# asm 2: mov  $0,>in8=%r9
mov  $0,%r9

# qhasm:   in9 = 0
# asm 1: mov  $0,>in9=int64#7
# asm 2: mov  $0,>in9=%rax
mov  $0,%rax

# qhasm:   *(uint32 *) (x + 24) = in6
# asm 1: movl   <in6=int64#5d,24(<x=int64#1)
# asm 2: movl   <in6=%r8d,24(<x=%rdi)
movl   %r8d,24(%rdi)

# qhasm:   *(uint32 *) (x + 44) = in7
# asm 1: movl   <in7=int64#2d,44(<x=int64#1)
# asm 2: movl   <in7=%esi,44(<x=%rdi)
movl   %esi,44(%rdi)

# qhasm:   *(uint32 *) (x + 32) = in8
# asm 1: movl   <in8=int64#6d,32(<x=int64#1)
# asm 2: movl   <in8=%r9d,32(<x=%rdi)
movl   %r9d,32(%rdi)

# qhasm:   *(uint32 *) (x + 52) = in9
# asm 1: movl   <in9=int64#7d,52(<x=int64#1)
# asm 2: movl   <in9=%eax,52(<x=%rdi)
movl   %eax,52(%rdi)

# qhasm: leave
add %r11,%rsp
mov %rdi,%rax
mov %rsi,%rdx
ret
