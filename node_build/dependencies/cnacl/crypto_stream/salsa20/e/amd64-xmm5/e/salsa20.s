
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

# qhasm: stack32 eax_stack

# qhasm: stack32 ebx_stack

# qhasm: stack32 esi_stack

# qhasm: stack32 edi_stack

# qhasm: stack32 ebp_stack

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

# qhasm: int6464 z0

# qhasm: int6464 z1

# qhasm: int6464 z2

# qhasm: int6464 z3

# qhasm: int6464 z4

# qhasm: int6464 z5

# qhasm: int6464 z6

# qhasm: int6464 z7

# qhasm: int6464 z8

# qhasm: int6464 z9

# qhasm: int6464 z10

# qhasm: int6464 z11

# qhasm: int6464 z12

# qhasm: int6464 z13

# qhasm: int6464 z14

# qhasm: int6464 z15

# qhasm: stack128 z0_stack

# qhasm: stack128 z1_stack

# qhasm: stack128 z2_stack

# qhasm: stack128 z3_stack

# qhasm: stack128 z4_stack

# qhasm: stack128 z5_stack

# qhasm: stack128 z6_stack

# qhasm: stack128 z7_stack

# qhasm: stack128 z8_stack

# qhasm: stack128 z9_stack

# qhasm: stack128 z10_stack

# qhasm: stack128 z11_stack

# qhasm: stack128 z12_stack

# qhasm: stack128 z13_stack

# qhasm: stack128 z14_stack

# qhasm: stack128 z15_stack

# qhasm: stack128 orig0

# qhasm: stack128 orig1

# qhasm: stack128 orig2

# qhasm: stack128 orig3

# qhasm: stack128 orig4

# qhasm: stack128 orig5

# qhasm: stack128 orig6

# qhasm: stack128 orig7

# qhasm: stack128 orig8

# qhasm: stack128 orig9

# qhasm: stack128 orig10

# qhasm: stack128 orig11

# qhasm: stack128 orig12

# qhasm: stack128 orig13

# qhasm: stack128 orig14

# qhasm: stack128 orig15

# qhasm: int6464 p

# qhasm: int6464 q

# qhasm: int6464 r

# qhasm: int6464 s

# qhasm: int6464 t

# qhasm: int6464 u

# qhasm: int6464 v

# qhasm: int6464 w

# qhasm: int6464 mp

# qhasm: int6464 mq

# qhasm: int6464 mr

# qhasm: int6464 ms

# qhasm: int6464 mt

# qhasm: int6464 mu

# qhasm: int6464 mv

# qhasm: int6464 mw

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
add $640,%r11
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
add $640,%r11
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
add $640,%r11
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

# qhasm:                               unsigned<? bytes - 256
# asm 1: cmp  $256,<bytes=int64#3
# asm 2: cmp  $256,<bytes=%rdx
cmp  $256,%rdx
# comment:fp stack unchanged by jump

# qhasm:   goto bytesbetween1and255 if unsigned<
jb ._bytesbetween1and255

# qhasm:   z0 = *(int128 *) (x + 0)
# asm 1: movdqa 0(<x=int64#5),>z0=int6464#1
# asm 2: movdqa 0(<x=%r8),>z0=%xmm0
movdqa 0(%r8),%xmm0

# qhasm:   z5 = z0[1,1,1,1]
# asm 1: pshufd $0x55,<z0=int6464#1,>z5=int6464#2
# asm 2: pshufd $0x55,<z0=%xmm0,>z5=%xmm1
pshufd $0x55,%xmm0,%xmm1

# qhasm:   z10 = z0[2,2,2,2]
# asm 1: pshufd $0xaa,<z0=int6464#1,>z10=int6464#3
# asm 2: pshufd $0xaa,<z0=%xmm0,>z10=%xmm2
pshufd $0xaa,%xmm0,%xmm2

# qhasm:   z15 = z0[3,3,3,3]
# asm 1: pshufd $0xff,<z0=int6464#1,>z15=int6464#4
# asm 2: pshufd $0xff,<z0=%xmm0,>z15=%xmm3
pshufd $0xff,%xmm0,%xmm3

# qhasm:   z0 = z0[0,0,0,0]
# asm 1: pshufd $0x00,<z0=int6464#1,>z0=int6464#1
# asm 2: pshufd $0x00,<z0=%xmm0,>z0=%xmm0
pshufd $0x00,%xmm0,%xmm0

# qhasm:   orig5 = z5
# asm 1: movdqa <z5=int6464#2,>orig5=stack128#1
# asm 2: movdqa <z5=%xmm1,>orig5=0(%rsp)
movdqa %xmm1,0(%rsp)

# qhasm:   orig10 = z10
# asm 1: movdqa <z10=int6464#3,>orig10=stack128#2
# asm 2: movdqa <z10=%xmm2,>orig10=16(%rsp)
movdqa %xmm2,16(%rsp)

# qhasm:   orig15 = z15
# asm 1: movdqa <z15=int6464#4,>orig15=stack128#3
# asm 2: movdqa <z15=%xmm3,>orig15=32(%rsp)
movdqa %xmm3,32(%rsp)

# qhasm:   orig0 = z0
# asm 1: movdqa <z0=int6464#1,>orig0=stack128#4
# asm 2: movdqa <z0=%xmm0,>orig0=48(%rsp)
movdqa %xmm0,48(%rsp)

# qhasm:   z1 = *(int128 *) (x + 16)
# asm 1: movdqa 16(<x=int64#5),>z1=int6464#1
# asm 2: movdqa 16(<x=%r8),>z1=%xmm0
movdqa 16(%r8),%xmm0

# qhasm:   z6 = z1[2,2,2,2]
# asm 1: pshufd $0xaa,<z1=int6464#1,>z6=int6464#2
# asm 2: pshufd $0xaa,<z1=%xmm0,>z6=%xmm1
pshufd $0xaa,%xmm0,%xmm1

# qhasm:   z11 = z1[3,3,3,3]
# asm 1: pshufd $0xff,<z1=int6464#1,>z11=int6464#3
# asm 2: pshufd $0xff,<z1=%xmm0,>z11=%xmm2
pshufd $0xff,%xmm0,%xmm2

# qhasm:   z12 = z1[0,0,0,0]
# asm 1: pshufd $0x00,<z1=int6464#1,>z12=int6464#4
# asm 2: pshufd $0x00,<z1=%xmm0,>z12=%xmm3
pshufd $0x00,%xmm0,%xmm3

# qhasm:   z1 = z1[1,1,1,1]
# asm 1: pshufd $0x55,<z1=int6464#1,>z1=int6464#1
# asm 2: pshufd $0x55,<z1=%xmm0,>z1=%xmm0
pshufd $0x55,%xmm0,%xmm0

# qhasm:   orig6 = z6
# asm 1: movdqa <z6=int6464#2,>orig6=stack128#5
# asm 2: movdqa <z6=%xmm1,>orig6=64(%rsp)
movdqa %xmm1,64(%rsp)

# qhasm:   orig11 = z11
# asm 1: movdqa <z11=int6464#3,>orig11=stack128#6
# asm 2: movdqa <z11=%xmm2,>orig11=80(%rsp)
movdqa %xmm2,80(%rsp)

# qhasm:   orig12 = z12
# asm 1: movdqa <z12=int6464#4,>orig12=stack128#7
# asm 2: movdqa <z12=%xmm3,>orig12=96(%rsp)
movdqa %xmm3,96(%rsp)

# qhasm:   orig1 = z1
# asm 1: movdqa <z1=int6464#1,>orig1=stack128#8
# asm 2: movdqa <z1=%xmm0,>orig1=112(%rsp)
movdqa %xmm0,112(%rsp)

# qhasm:   z2 = *(int128 *) (x + 32)
# asm 1: movdqa 32(<x=int64#5),>z2=int6464#1
# asm 2: movdqa 32(<x=%r8),>z2=%xmm0
movdqa 32(%r8),%xmm0

# qhasm:   z7 = z2[3,3,3,3]
# asm 1: pshufd $0xff,<z2=int6464#1,>z7=int6464#2
# asm 2: pshufd $0xff,<z2=%xmm0,>z7=%xmm1
pshufd $0xff,%xmm0,%xmm1

# qhasm:   z13 = z2[1,1,1,1]
# asm 1: pshufd $0x55,<z2=int6464#1,>z13=int6464#3
# asm 2: pshufd $0x55,<z2=%xmm0,>z13=%xmm2
pshufd $0x55,%xmm0,%xmm2

# qhasm:   z2 = z2[2,2,2,2]
# asm 1: pshufd $0xaa,<z2=int6464#1,>z2=int6464#1
# asm 2: pshufd $0xaa,<z2=%xmm0,>z2=%xmm0
pshufd $0xaa,%xmm0,%xmm0

# qhasm:   orig7 = z7
# asm 1: movdqa <z7=int6464#2,>orig7=stack128#9
# asm 2: movdqa <z7=%xmm1,>orig7=128(%rsp)
movdqa %xmm1,128(%rsp)

# qhasm:   orig13 = z13
# asm 1: movdqa <z13=int6464#3,>orig13=stack128#10
# asm 2: movdqa <z13=%xmm2,>orig13=144(%rsp)
movdqa %xmm2,144(%rsp)

# qhasm:   orig2 = z2
# asm 1: movdqa <z2=int6464#1,>orig2=stack128#11
# asm 2: movdqa <z2=%xmm0,>orig2=160(%rsp)
movdqa %xmm0,160(%rsp)

# qhasm:   z3 = *(int128 *) (x + 48)
# asm 1: movdqa 48(<x=int64#5),>z3=int6464#1
# asm 2: movdqa 48(<x=%r8),>z3=%xmm0
movdqa 48(%r8),%xmm0

# qhasm:   z4 = z3[0,0,0,0]
# asm 1: pshufd $0x00,<z3=int6464#1,>z4=int6464#2
# asm 2: pshufd $0x00,<z3=%xmm0,>z4=%xmm1
pshufd $0x00,%xmm0,%xmm1

# qhasm:   z14 = z3[2,2,2,2]
# asm 1: pshufd $0xaa,<z3=int6464#1,>z14=int6464#3
# asm 2: pshufd $0xaa,<z3=%xmm0,>z14=%xmm2
pshufd $0xaa,%xmm0,%xmm2

# qhasm:   z3 = z3[3,3,3,3]
# asm 1: pshufd $0xff,<z3=int6464#1,>z3=int6464#1
# asm 2: pshufd $0xff,<z3=%xmm0,>z3=%xmm0
pshufd $0xff,%xmm0,%xmm0

# qhasm:   orig4 = z4
# asm 1: movdqa <z4=int6464#2,>orig4=stack128#12
# asm 2: movdqa <z4=%xmm1,>orig4=176(%rsp)
movdqa %xmm1,176(%rsp)

# qhasm:   orig14 = z14
# asm 1: movdqa <z14=int6464#3,>orig14=stack128#13
# asm 2: movdqa <z14=%xmm2,>orig14=192(%rsp)
movdqa %xmm2,192(%rsp)

# qhasm:   orig3 = z3
# asm 1: movdqa <z3=int6464#1,>orig3=stack128#14
# asm 2: movdqa <z3=%xmm0,>orig3=208(%rsp)
movdqa %xmm0,208(%rsp)

# qhasm: bytesatleast256:
._bytesatleast256:

# qhasm:   in8 = *(uint32 *) (x + 32)
# asm 1: movl   32(<x=int64#5),>in8=int64#4d
# asm 2: movl   32(<x=%r8),>in8=%ecx
movl   32(%r8),%ecx

# qhasm:   in9 = *(uint32 *) (x + 52)
# asm 1: movl   52(<x=int64#5),>in9=int64#6d
# asm 2: movl   52(<x=%r8),>in9=%r9d
movl   52(%r8),%r9d

# qhasm:   ((uint32 *) &orig8)[0] = in8
# asm 1: movl <in8=int64#4d,>orig8=stack128#15
# asm 2: movl <in8=%ecx,>orig8=224(%rsp)
movl %ecx,224(%rsp)

# qhasm:   ((uint32 *) &orig9)[0] = in9
# asm 1: movl <in9=int64#6d,>orig9=stack128#16
# asm 2: movl <in9=%r9d,>orig9=240(%rsp)
movl %r9d,240(%rsp)

# qhasm:   in8 += 1
# asm 1: add  $1,<in8=int64#4
# asm 2: add  $1,<in8=%rcx
add  $1,%rcx

# qhasm:   in9 <<= 32
# asm 1: shl  $32,<in9=int64#6
# asm 2: shl  $32,<in9=%r9
shl  $32,%r9

# qhasm:   in8 += in9
# asm 1: add  <in9=int64#6,<in8=int64#4
# asm 2: add  <in9=%r9,<in8=%rcx
add  %r9,%rcx

# qhasm:   in9 = in8
# asm 1: mov  <in8=int64#4,>in9=int64#6
# asm 2: mov  <in8=%rcx,>in9=%r9
mov  %rcx,%r9

# qhasm:   (uint64) in9 >>= 32
# asm 1: shr  $32,<in9=int64#6
# asm 2: shr  $32,<in9=%r9
shr  $32,%r9

# qhasm:   ((uint32 *) &orig8)[1] = in8
# asm 1: movl <in8=int64#4d,4+<orig8=stack128#15
# asm 2: movl <in8=%ecx,4+<orig8=224(%rsp)
movl %ecx,4+224(%rsp)

# qhasm:   ((uint32 *) &orig9)[1] = in9
# asm 1: movl <in9=int64#6d,4+<orig9=stack128#16
# asm 2: movl <in9=%r9d,4+<orig9=240(%rsp)
movl %r9d,4+240(%rsp)

# qhasm:   in8 += 1
# asm 1: add  $1,<in8=int64#4
# asm 2: add  $1,<in8=%rcx
add  $1,%rcx

# qhasm:   in9 <<= 32
# asm 1: shl  $32,<in9=int64#6
# asm 2: shl  $32,<in9=%r9
shl  $32,%r9

# qhasm:   in8 += in9
# asm 1: add  <in9=int64#6,<in8=int64#4
# asm 2: add  <in9=%r9,<in8=%rcx
add  %r9,%rcx

# qhasm:   in9 = in8
# asm 1: mov  <in8=int64#4,>in9=int64#6
# asm 2: mov  <in8=%rcx,>in9=%r9
mov  %rcx,%r9

# qhasm:   (uint64) in9 >>= 32
# asm 1: shr  $32,<in9=int64#6
# asm 2: shr  $32,<in9=%r9
shr  $32,%r9

# qhasm:   ((uint32 *) &orig8)[2] = in8
# asm 1: movl <in8=int64#4d,8+<orig8=stack128#15
# asm 2: movl <in8=%ecx,8+<orig8=224(%rsp)
movl %ecx,8+224(%rsp)

# qhasm:   ((uint32 *) &orig9)[2] = in9
# asm 1: movl <in9=int64#6d,8+<orig9=stack128#16
# asm 2: movl <in9=%r9d,8+<orig9=240(%rsp)
movl %r9d,8+240(%rsp)

# qhasm:   in8 += 1
# asm 1: add  $1,<in8=int64#4
# asm 2: add  $1,<in8=%rcx
add  $1,%rcx

# qhasm:   in9 <<= 32
# asm 1: shl  $32,<in9=int64#6
# asm 2: shl  $32,<in9=%r9
shl  $32,%r9

# qhasm:   in8 += in9
# asm 1: add  <in9=int64#6,<in8=int64#4
# asm 2: add  <in9=%r9,<in8=%rcx
add  %r9,%rcx

# qhasm:   in9 = in8
# asm 1: mov  <in8=int64#4,>in9=int64#6
# asm 2: mov  <in8=%rcx,>in9=%r9
mov  %rcx,%r9

# qhasm:   (uint64) in9 >>= 32
# asm 1: shr  $32,<in9=int64#6
# asm 2: shr  $32,<in9=%r9
shr  $32,%r9

# qhasm:   ((uint32 *) &orig8)[3] = in8
# asm 1: movl <in8=int64#4d,12+<orig8=stack128#15
# asm 2: movl <in8=%ecx,12+<orig8=224(%rsp)
movl %ecx,12+224(%rsp)

# qhasm:   ((uint32 *) &orig9)[3] = in9
# asm 1: movl <in9=int64#6d,12+<orig9=stack128#16
# asm 2: movl <in9=%r9d,12+<orig9=240(%rsp)
movl %r9d,12+240(%rsp)

# qhasm:   in8 += 1
# asm 1: add  $1,<in8=int64#4
# asm 2: add  $1,<in8=%rcx
add  $1,%rcx

# qhasm:   in9 <<= 32
# asm 1: shl  $32,<in9=int64#6
# asm 2: shl  $32,<in9=%r9
shl  $32,%r9

# qhasm:   in8 += in9
# asm 1: add  <in9=int64#6,<in8=int64#4
# asm 2: add  <in9=%r9,<in8=%rcx
add  %r9,%rcx

# qhasm:   in9 = in8
# asm 1: mov  <in8=int64#4,>in9=int64#6
# asm 2: mov  <in8=%rcx,>in9=%r9
mov  %rcx,%r9

# qhasm:   (uint64) in9 >>= 32
# asm 1: shr  $32,<in9=int64#6
# asm 2: shr  $32,<in9=%r9
shr  $32,%r9

# qhasm:   *(uint32 *) (x + 32) = in8
# asm 1: movl   <in8=int64#4d,32(<x=int64#5)
# asm 2: movl   <in8=%ecx,32(<x=%r8)
movl   %ecx,32(%r8)

# qhasm:   *(uint32 *) (x + 52) = in9
# asm 1: movl   <in9=int64#6d,52(<x=int64#5)
# asm 2: movl   <in9=%r9d,52(<x=%r8)
movl   %r9d,52(%r8)

# qhasm:   bytes_backup = bytes
# asm 1: movq <bytes=int64#3,>bytes_backup=stack64#1
# asm 2: movq <bytes=%rdx,>bytes_backup=544(%rsp)
movq %rdx,544(%rsp)

# qhasm: i = 20
# asm 1: mov  $20,>i=int64#3
# asm 2: mov  $20,>i=%rdx
mov  $20,%rdx

# qhasm:   z5 = orig5
# asm 1: movdqa <orig5=stack128#1,>z5=int6464#1
# asm 2: movdqa <orig5=0(%rsp),>z5=%xmm0
movdqa 0(%rsp),%xmm0

# qhasm:   z10 = orig10
# asm 1: movdqa <orig10=stack128#2,>z10=int6464#2
# asm 2: movdqa <orig10=16(%rsp),>z10=%xmm1
movdqa 16(%rsp),%xmm1

# qhasm:   z15 = orig15
# asm 1: movdqa <orig15=stack128#3,>z15=int6464#3
# asm 2: movdqa <orig15=32(%rsp),>z15=%xmm2
movdqa 32(%rsp),%xmm2

# qhasm:   z14 = orig14
# asm 1: movdqa <orig14=stack128#13,>z14=int6464#4
# asm 2: movdqa <orig14=192(%rsp),>z14=%xmm3
movdqa 192(%rsp),%xmm3

# qhasm:   z3 = orig3
# asm 1: movdqa <orig3=stack128#14,>z3=int6464#5
# asm 2: movdqa <orig3=208(%rsp),>z3=%xmm4
movdqa 208(%rsp),%xmm4

# qhasm:   z6 = orig6
# asm 1: movdqa <orig6=stack128#5,>z6=int6464#6
# asm 2: movdqa <orig6=64(%rsp),>z6=%xmm5
movdqa 64(%rsp),%xmm5

# qhasm:   z11 = orig11
# asm 1: movdqa <orig11=stack128#6,>z11=int6464#7
# asm 2: movdqa <orig11=80(%rsp),>z11=%xmm6
movdqa 80(%rsp),%xmm6

# qhasm:   z1 = orig1
# asm 1: movdqa <orig1=stack128#8,>z1=int6464#8
# asm 2: movdqa <orig1=112(%rsp),>z1=%xmm7
movdqa 112(%rsp),%xmm7

# qhasm:   z5_stack = z5
# asm 1: movdqa <z5=int6464#1,>z5_stack=stack128#17
# asm 2: movdqa <z5=%xmm0,>z5_stack=256(%rsp)
movdqa %xmm0,256(%rsp)

# qhasm:   z10_stack = z10
# asm 1: movdqa <z10=int6464#2,>z10_stack=stack128#18
# asm 2: movdqa <z10=%xmm1,>z10_stack=272(%rsp)
movdqa %xmm1,272(%rsp)

# qhasm:   z15_stack = z15
# asm 1: movdqa <z15=int6464#3,>z15_stack=stack128#19
# asm 2: movdqa <z15=%xmm2,>z15_stack=288(%rsp)
movdqa %xmm2,288(%rsp)

# qhasm:   z14_stack = z14
# asm 1: movdqa <z14=int6464#4,>z14_stack=stack128#20
# asm 2: movdqa <z14=%xmm3,>z14_stack=304(%rsp)
movdqa %xmm3,304(%rsp)

# qhasm:   z3_stack = z3
# asm 1: movdqa <z3=int6464#5,>z3_stack=stack128#21
# asm 2: movdqa <z3=%xmm4,>z3_stack=320(%rsp)
movdqa %xmm4,320(%rsp)

# qhasm:   z6_stack = z6
# asm 1: movdqa <z6=int6464#6,>z6_stack=stack128#22
# asm 2: movdqa <z6=%xmm5,>z6_stack=336(%rsp)
movdqa %xmm5,336(%rsp)

# qhasm:   z11_stack = z11
# asm 1: movdqa <z11=int6464#7,>z11_stack=stack128#23
# asm 2: movdqa <z11=%xmm6,>z11_stack=352(%rsp)
movdqa %xmm6,352(%rsp)

# qhasm:   z1_stack = z1
# asm 1: movdqa <z1=int6464#8,>z1_stack=stack128#24
# asm 2: movdqa <z1=%xmm7,>z1_stack=368(%rsp)
movdqa %xmm7,368(%rsp)

# qhasm:   z7 = orig7
# asm 1: movdqa <orig7=stack128#9,>z7=int6464#5
# asm 2: movdqa <orig7=128(%rsp),>z7=%xmm4
movdqa 128(%rsp),%xmm4

# qhasm:   z13 = orig13
# asm 1: movdqa <orig13=stack128#10,>z13=int6464#6
# asm 2: movdqa <orig13=144(%rsp),>z13=%xmm5
movdqa 144(%rsp),%xmm5

# qhasm:   z2 = orig2
# asm 1: movdqa <orig2=stack128#11,>z2=int6464#7
# asm 2: movdqa <orig2=160(%rsp),>z2=%xmm6
movdqa 160(%rsp),%xmm6

# qhasm:   z9 = orig9
# asm 1: movdqa <orig9=stack128#16,>z9=int6464#8
# asm 2: movdqa <orig9=240(%rsp),>z9=%xmm7
movdqa 240(%rsp),%xmm7

# qhasm:                   p = orig0
# asm 1: movdqa <orig0=stack128#4,>p=int6464#1
# asm 2: movdqa <orig0=48(%rsp),>p=%xmm0
movdqa 48(%rsp),%xmm0

# qhasm:                   t = orig12
# asm 1: movdqa <orig12=stack128#7,>t=int6464#3
# asm 2: movdqa <orig12=96(%rsp),>t=%xmm2
movdqa 96(%rsp),%xmm2

# qhasm:                   q = orig4
# asm 1: movdqa <orig4=stack128#12,>q=int6464#4
# asm 2: movdqa <orig4=176(%rsp),>q=%xmm3
movdqa 176(%rsp),%xmm3

# qhasm:                   r = orig8
# asm 1: movdqa <orig8=stack128#15,>r=int6464#2
# asm 2: movdqa <orig8=224(%rsp),>r=%xmm1
movdqa 224(%rsp),%xmm1

# qhasm:   z7_stack = z7
# asm 1: movdqa <z7=int6464#5,>z7_stack=stack128#25
# asm 2: movdqa <z7=%xmm4,>z7_stack=384(%rsp)
movdqa %xmm4,384(%rsp)

# qhasm:   z13_stack = z13
# asm 1: movdqa <z13=int6464#6,>z13_stack=stack128#26
# asm 2: movdqa <z13=%xmm5,>z13_stack=400(%rsp)
movdqa %xmm5,400(%rsp)

# qhasm:   z2_stack = z2
# asm 1: movdqa <z2=int6464#7,>z2_stack=stack128#27
# asm 2: movdqa <z2=%xmm6,>z2_stack=416(%rsp)
movdqa %xmm6,416(%rsp)

# qhasm:   z9_stack = z9
# asm 1: movdqa <z9=int6464#8,>z9_stack=stack128#28
# asm 2: movdqa <z9=%xmm7,>z9_stack=432(%rsp)
movdqa %xmm7,432(%rsp)

# qhasm:   z0_stack = p
# asm 1: movdqa <p=int6464#1,>z0_stack=stack128#29
# asm 2: movdqa <p=%xmm0,>z0_stack=448(%rsp)
movdqa %xmm0,448(%rsp)

# qhasm:   z12_stack = t
# asm 1: movdqa <t=int6464#3,>z12_stack=stack128#30
# asm 2: movdqa <t=%xmm2,>z12_stack=464(%rsp)
movdqa %xmm2,464(%rsp)

# qhasm:   z4_stack = q
# asm 1: movdqa <q=int6464#4,>z4_stack=stack128#31
# asm 2: movdqa <q=%xmm3,>z4_stack=480(%rsp)
movdqa %xmm3,480(%rsp)

# qhasm:   z8_stack = r
# asm 1: movdqa <r=int6464#2,>z8_stack=stack128#32
# asm 2: movdqa <r=%xmm1,>z8_stack=496(%rsp)
movdqa %xmm1,496(%rsp)

# qhasm: mainloop1:
._mainloop1:

# qhasm: 		  assign xmm0 to p

# qhasm: 		  assign xmm1 to r

# qhasm: 		  assign xmm2 to t

# qhasm: 		  assign xmm3 to q

# qhasm:                   s = t
# asm 1: movdqa <t=int6464#3,>s=int6464#7
# asm 2: movdqa <t=%xmm2,>s=%xmm6
movdqa %xmm2,%xmm6

# qhasm: uint32323232      t += p
# asm 1: paddd <p=int6464#1,<t=int6464#3
# asm 2: paddd <p=%xmm0,<t=%xmm2
paddd %xmm0,%xmm2

# qhasm:                   u = t
# asm 1: movdqa <t=int6464#3,>u=int6464#5
# asm 2: movdqa <t=%xmm2,>u=%xmm4
movdqa %xmm2,%xmm4

# qhasm: uint32323232      t >>= 25
# asm 1: psrld $25,<t=int6464#3
# asm 2: psrld $25,<t=%xmm2
psrld $25,%xmm2

# qhasm:                   q ^= t
# asm 1: pxor  <t=int6464#3,<q=int6464#4
# asm 2: pxor  <t=%xmm2,<q=%xmm3
pxor  %xmm2,%xmm3

# qhasm: uint32323232      u <<= 7
# asm 1: pslld $7,<u=int6464#5
# asm 2: pslld $7,<u=%xmm4
pslld $7,%xmm4

# qhasm:                   q ^= u
# asm 1: pxor  <u=int6464#5,<q=int6464#4
# asm 2: pxor  <u=%xmm4,<q=%xmm3
pxor  %xmm4,%xmm3

# qhasm:                   z4_stack = q
# asm 1: movdqa <q=int6464#4,>z4_stack=stack128#29
# asm 2: movdqa <q=%xmm3,>z4_stack=448(%rsp)
movdqa %xmm3,448(%rsp)

# qhasm:                   t = p
# asm 1: movdqa <p=int6464#1,>t=int6464#3
# asm 2: movdqa <p=%xmm0,>t=%xmm2
movdqa %xmm0,%xmm2

# qhasm: uint32323232      t += q
# asm 1: paddd <q=int6464#4,<t=int6464#3
# asm 2: paddd <q=%xmm3,<t=%xmm2
paddd %xmm3,%xmm2

# qhasm:                   u = t
# asm 1: movdqa <t=int6464#3,>u=int6464#5
# asm 2: movdqa <t=%xmm2,>u=%xmm4
movdqa %xmm2,%xmm4

# qhasm: uint32323232      t >>= 23
# asm 1: psrld $23,<t=int6464#3
# asm 2: psrld $23,<t=%xmm2
psrld $23,%xmm2

# qhasm:                   r ^= t
# asm 1: pxor  <t=int6464#3,<r=int6464#2
# asm 2: pxor  <t=%xmm2,<r=%xmm1
pxor  %xmm2,%xmm1

# qhasm: uint32323232      u <<= 9
# asm 1: pslld $9,<u=int6464#5
# asm 2: pslld $9,<u=%xmm4
pslld $9,%xmm4

# qhasm:                   r ^= u
# asm 1: pxor  <u=int6464#5,<r=int6464#2
# asm 2: pxor  <u=%xmm4,<r=%xmm1
pxor  %xmm4,%xmm1

# qhasm:                   z8_stack = r
# asm 1: movdqa <r=int6464#2,>z8_stack=stack128#30
# asm 2: movdqa <r=%xmm1,>z8_stack=464(%rsp)
movdqa %xmm1,464(%rsp)

# qhasm: uint32323232      q += r
# asm 1: paddd <r=int6464#2,<q=int6464#4
# asm 2: paddd <r=%xmm1,<q=%xmm3
paddd %xmm1,%xmm3

# qhasm:                   u = q
# asm 1: movdqa <q=int6464#4,>u=int6464#3
# asm 2: movdqa <q=%xmm3,>u=%xmm2
movdqa %xmm3,%xmm2

# qhasm: uint32323232      q >>= 19
# asm 1: psrld $19,<q=int6464#4
# asm 2: psrld $19,<q=%xmm3
psrld $19,%xmm3

# qhasm:                   s ^= q
# asm 1: pxor  <q=int6464#4,<s=int6464#7
# asm 2: pxor  <q=%xmm3,<s=%xmm6
pxor  %xmm3,%xmm6

# qhasm: uint32323232      u <<= 13
# asm 1: pslld $13,<u=int6464#3
# asm 2: pslld $13,<u=%xmm2
pslld $13,%xmm2

# qhasm:                   s ^= u
# asm 1: pxor  <u=int6464#3,<s=int6464#7
# asm 2: pxor  <u=%xmm2,<s=%xmm6
pxor  %xmm2,%xmm6

# qhasm:                                   mt = z1_stack
# asm 1: movdqa <z1_stack=stack128#24,>mt=int6464#3
# asm 2: movdqa <z1_stack=368(%rsp),>mt=%xmm2
movdqa 368(%rsp),%xmm2

# qhasm:                                   mp = z5_stack
# asm 1: movdqa <z5_stack=stack128#17,>mp=int6464#5
# asm 2: movdqa <z5_stack=256(%rsp),>mp=%xmm4
movdqa 256(%rsp),%xmm4

# qhasm:                                   mq = z9_stack
# asm 1: movdqa <z9_stack=stack128#28,>mq=int6464#4
# asm 2: movdqa <z9_stack=432(%rsp),>mq=%xmm3
movdqa 432(%rsp),%xmm3

# qhasm:                                   mr = z13_stack
# asm 1: movdqa <z13_stack=stack128#26,>mr=int6464#6
# asm 2: movdqa <z13_stack=400(%rsp),>mr=%xmm5
movdqa 400(%rsp),%xmm5

# qhasm:                   z12_stack = s
# asm 1: movdqa <s=int6464#7,>z12_stack=stack128#26
# asm 2: movdqa <s=%xmm6,>z12_stack=400(%rsp)
movdqa %xmm6,400(%rsp)

# qhasm: uint32323232      r += s
# asm 1: paddd <s=int6464#7,<r=int6464#2
# asm 2: paddd <s=%xmm6,<r=%xmm1
paddd %xmm6,%xmm1

# qhasm:                   u = r
# asm 1: movdqa <r=int6464#2,>u=int6464#7
# asm 2: movdqa <r=%xmm1,>u=%xmm6
movdqa %xmm1,%xmm6

# qhasm: uint32323232      r >>= 14
# asm 1: psrld $14,<r=int6464#2
# asm 2: psrld $14,<r=%xmm1
psrld $14,%xmm1

# qhasm:                   p ^= r
# asm 1: pxor  <r=int6464#2,<p=int6464#1
# asm 2: pxor  <r=%xmm1,<p=%xmm0
pxor  %xmm1,%xmm0

# qhasm: uint32323232      u <<= 18
# asm 1: pslld $18,<u=int6464#7
# asm 2: pslld $18,<u=%xmm6
pslld $18,%xmm6

# qhasm:                   p ^= u
# asm 1: pxor  <u=int6464#7,<p=int6464#1
# asm 2: pxor  <u=%xmm6,<p=%xmm0
pxor  %xmm6,%xmm0

# qhasm:                   z0_stack = p
# asm 1: movdqa <p=int6464#1,>z0_stack=stack128#17
# asm 2: movdqa <p=%xmm0,>z0_stack=256(%rsp)
movdqa %xmm0,256(%rsp)

# qhasm: 		  		  assign xmm2 to mt

# qhasm: 		  		  assign xmm3 to mq

# qhasm: 		  		  assign xmm4 to mp

# qhasm: 		  		  assign xmm5 to mr

# qhasm:                                   ms = mt
# asm 1: movdqa <mt=int6464#3,>ms=int6464#7
# asm 2: movdqa <mt=%xmm2,>ms=%xmm6
movdqa %xmm2,%xmm6

# qhasm: uint32323232                      mt += mp
# asm 1: paddd <mp=int6464#5,<mt=int6464#3
# asm 2: paddd <mp=%xmm4,<mt=%xmm2
paddd %xmm4,%xmm2

# qhasm:                                   mu = mt
# asm 1: movdqa <mt=int6464#3,>mu=int6464#1
# asm 2: movdqa <mt=%xmm2,>mu=%xmm0
movdqa %xmm2,%xmm0

# qhasm: uint32323232                      mt >>= 25
# asm 1: psrld $25,<mt=int6464#3
# asm 2: psrld $25,<mt=%xmm2
psrld $25,%xmm2

# qhasm:                                   mq ^= mt
# asm 1: pxor  <mt=int6464#3,<mq=int6464#4
# asm 2: pxor  <mt=%xmm2,<mq=%xmm3
pxor  %xmm2,%xmm3

# qhasm: uint32323232                      mu <<= 7
# asm 1: pslld $7,<mu=int6464#1
# asm 2: pslld $7,<mu=%xmm0
pslld $7,%xmm0

# qhasm:                                   mq ^= mu
# asm 1: pxor  <mu=int6464#1,<mq=int6464#4
# asm 2: pxor  <mu=%xmm0,<mq=%xmm3
pxor  %xmm0,%xmm3

# qhasm:                                   z9_stack = mq
# asm 1: movdqa <mq=int6464#4,>z9_stack=stack128#28
# asm 2: movdqa <mq=%xmm3,>z9_stack=432(%rsp)
movdqa %xmm3,432(%rsp)

# qhasm:                                   mt = mp
# asm 1: movdqa <mp=int6464#5,>mt=int6464#1
# asm 2: movdqa <mp=%xmm4,>mt=%xmm0
movdqa %xmm4,%xmm0

# qhasm: uint32323232                      mt += mq
# asm 1: paddd <mq=int6464#4,<mt=int6464#1
# asm 2: paddd <mq=%xmm3,<mt=%xmm0
paddd %xmm3,%xmm0

# qhasm:                                   mu = mt
# asm 1: movdqa <mt=int6464#1,>mu=int6464#2
# asm 2: movdqa <mt=%xmm0,>mu=%xmm1
movdqa %xmm0,%xmm1

# qhasm: uint32323232                      mt >>= 23
# asm 1: psrld $23,<mt=int6464#1
# asm 2: psrld $23,<mt=%xmm0
psrld $23,%xmm0

# qhasm:                                   mr ^= mt
# asm 1: pxor  <mt=int6464#1,<mr=int6464#6
# asm 2: pxor  <mt=%xmm0,<mr=%xmm5
pxor  %xmm0,%xmm5

# qhasm: uint32323232                      mu <<= 9
# asm 1: pslld $9,<mu=int6464#2
# asm 2: pslld $9,<mu=%xmm1
pslld $9,%xmm1

# qhasm:                                   mr ^= mu
# asm 1: pxor  <mu=int6464#2,<mr=int6464#6
# asm 2: pxor  <mu=%xmm1,<mr=%xmm5
pxor  %xmm1,%xmm5

# qhasm:                                   z13_stack = mr
# asm 1: movdqa <mr=int6464#6,>z13_stack=stack128#31
# asm 2: movdqa <mr=%xmm5,>z13_stack=480(%rsp)
movdqa %xmm5,480(%rsp)

# qhasm: uint32323232                      mq += mr
# asm 1: paddd <mr=int6464#6,<mq=int6464#4
# asm 2: paddd <mr=%xmm5,<mq=%xmm3
paddd %xmm5,%xmm3

# qhasm:                                   mu = mq
# asm 1: movdqa <mq=int6464#4,>mu=int6464#1
# asm 2: movdqa <mq=%xmm3,>mu=%xmm0
movdqa %xmm3,%xmm0

# qhasm: uint32323232                      mq >>= 19
# asm 1: psrld $19,<mq=int6464#4
# asm 2: psrld $19,<mq=%xmm3
psrld $19,%xmm3

# qhasm:                                   ms ^= mq
# asm 1: pxor  <mq=int6464#4,<ms=int6464#7
# asm 2: pxor  <mq=%xmm3,<ms=%xmm6
pxor  %xmm3,%xmm6

# qhasm: uint32323232                      mu <<= 13
# asm 1: pslld $13,<mu=int6464#1
# asm 2: pslld $13,<mu=%xmm0
pslld $13,%xmm0

# qhasm:                                   ms ^= mu
# asm 1: pxor  <mu=int6464#1,<ms=int6464#7
# asm 2: pxor  <mu=%xmm0,<ms=%xmm6
pxor  %xmm0,%xmm6

# qhasm:                                                   t = z6_stack
# asm 1: movdqa <z6_stack=stack128#22,>t=int6464#3
# asm 2: movdqa <z6_stack=336(%rsp),>t=%xmm2
movdqa 336(%rsp),%xmm2

# qhasm:                                                   p = z10_stack
# asm 1: movdqa <z10_stack=stack128#18,>p=int6464#1
# asm 2: movdqa <z10_stack=272(%rsp),>p=%xmm0
movdqa 272(%rsp),%xmm0

# qhasm:                                                   q = z14_stack
# asm 1: movdqa <z14_stack=stack128#20,>q=int6464#4
# asm 2: movdqa <z14_stack=304(%rsp),>q=%xmm3
movdqa 304(%rsp),%xmm3

# qhasm:                                                   r = z2_stack
# asm 1: movdqa <z2_stack=stack128#27,>r=int6464#2
# asm 2: movdqa <z2_stack=416(%rsp),>r=%xmm1
movdqa 416(%rsp),%xmm1

# qhasm:                                   z1_stack = ms
# asm 1: movdqa <ms=int6464#7,>z1_stack=stack128#18
# asm 2: movdqa <ms=%xmm6,>z1_stack=272(%rsp)
movdqa %xmm6,272(%rsp)

# qhasm: uint32323232                      mr += ms
# asm 1: paddd <ms=int6464#7,<mr=int6464#6
# asm 2: paddd <ms=%xmm6,<mr=%xmm5
paddd %xmm6,%xmm5

# qhasm:                                   mu = mr
# asm 1: movdqa <mr=int6464#6,>mu=int6464#7
# asm 2: movdqa <mr=%xmm5,>mu=%xmm6
movdqa %xmm5,%xmm6

# qhasm: uint32323232                      mr >>= 14
# asm 1: psrld $14,<mr=int6464#6
# asm 2: psrld $14,<mr=%xmm5
psrld $14,%xmm5

# qhasm:                                   mp ^= mr
# asm 1: pxor  <mr=int6464#6,<mp=int6464#5
# asm 2: pxor  <mr=%xmm5,<mp=%xmm4
pxor  %xmm5,%xmm4

# qhasm: uint32323232                      mu <<= 18
# asm 1: pslld $18,<mu=int6464#7
# asm 2: pslld $18,<mu=%xmm6
pslld $18,%xmm6

# qhasm:                                   mp ^= mu
# asm 1: pxor  <mu=int6464#7,<mp=int6464#5
# asm 2: pxor  <mu=%xmm6,<mp=%xmm4
pxor  %xmm6,%xmm4

# qhasm:                                   z5_stack = mp
# asm 1: movdqa <mp=int6464#5,>z5_stack=stack128#20
# asm 2: movdqa <mp=%xmm4,>z5_stack=304(%rsp)
movdqa %xmm4,304(%rsp)

# qhasm: 		  				  assign xmm0 to p

# qhasm: 						  assign xmm1 to r

# qhasm: 						  assign xmm2 to t

# qhasm: 						  assign xmm3 to q

# qhasm:                                                   s = t
# asm 1: movdqa <t=int6464#3,>s=int6464#7
# asm 2: movdqa <t=%xmm2,>s=%xmm6
movdqa %xmm2,%xmm6

# qhasm: uint32323232                                      t += p
# asm 1: paddd <p=int6464#1,<t=int6464#3
# asm 2: paddd <p=%xmm0,<t=%xmm2
paddd %xmm0,%xmm2

# qhasm:                                                   u = t
# asm 1: movdqa <t=int6464#3,>u=int6464#5
# asm 2: movdqa <t=%xmm2,>u=%xmm4
movdqa %xmm2,%xmm4

# qhasm: uint32323232                                      t >>= 25
# asm 1: psrld $25,<t=int6464#3
# asm 2: psrld $25,<t=%xmm2
psrld $25,%xmm2

# qhasm:                                                   q ^= t
# asm 1: pxor  <t=int6464#3,<q=int6464#4
# asm 2: pxor  <t=%xmm2,<q=%xmm3
pxor  %xmm2,%xmm3

# qhasm: uint32323232                                      u <<= 7
# asm 1: pslld $7,<u=int6464#5
# asm 2: pslld $7,<u=%xmm4
pslld $7,%xmm4

# qhasm:                                                   q ^= u
# asm 1: pxor  <u=int6464#5,<q=int6464#4
# asm 2: pxor  <u=%xmm4,<q=%xmm3
pxor  %xmm4,%xmm3

# qhasm:                                                   z14_stack = q
# asm 1: movdqa <q=int6464#4,>z14_stack=stack128#32
# asm 2: movdqa <q=%xmm3,>z14_stack=496(%rsp)
movdqa %xmm3,496(%rsp)

# qhasm:                                                   t = p
# asm 1: movdqa <p=int6464#1,>t=int6464#3
# asm 2: movdqa <p=%xmm0,>t=%xmm2
movdqa %xmm0,%xmm2

# qhasm: uint32323232                                      t += q
# asm 1: paddd <q=int6464#4,<t=int6464#3
# asm 2: paddd <q=%xmm3,<t=%xmm2
paddd %xmm3,%xmm2

# qhasm:                                                   u = t
# asm 1: movdqa <t=int6464#3,>u=int6464#5
# asm 2: movdqa <t=%xmm2,>u=%xmm4
movdqa %xmm2,%xmm4

# qhasm: uint32323232                                      t >>= 23
# asm 1: psrld $23,<t=int6464#3
# asm 2: psrld $23,<t=%xmm2
psrld $23,%xmm2

# qhasm:                                                   r ^= t
# asm 1: pxor  <t=int6464#3,<r=int6464#2
# asm 2: pxor  <t=%xmm2,<r=%xmm1
pxor  %xmm2,%xmm1

# qhasm: uint32323232                                      u <<= 9
# asm 1: pslld $9,<u=int6464#5
# asm 2: pslld $9,<u=%xmm4
pslld $9,%xmm4

# qhasm:                                                   r ^= u
# asm 1: pxor  <u=int6464#5,<r=int6464#2
# asm 2: pxor  <u=%xmm4,<r=%xmm1
pxor  %xmm4,%xmm1

# qhasm:                                                   z2_stack = r
# asm 1: movdqa <r=int6464#2,>z2_stack=stack128#22
# asm 2: movdqa <r=%xmm1,>z2_stack=336(%rsp)
movdqa %xmm1,336(%rsp)

# qhasm: uint32323232                                      q += r
# asm 1: paddd <r=int6464#2,<q=int6464#4
# asm 2: paddd <r=%xmm1,<q=%xmm3
paddd %xmm1,%xmm3

# qhasm:                                                   u = q
# asm 1: movdqa <q=int6464#4,>u=int6464#3
# asm 2: movdqa <q=%xmm3,>u=%xmm2
movdqa %xmm3,%xmm2

# qhasm: uint32323232                                      q >>= 19
# asm 1: psrld $19,<q=int6464#4
# asm 2: psrld $19,<q=%xmm3
psrld $19,%xmm3

# qhasm:                                                   s ^= q
# asm 1: pxor  <q=int6464#4,<s=int6464#7
# asm 2: pxor  <q=%xmm3,<s=%xmm6
pxor  %xmm3,%xmm6

# qhasm: uint32323232                                      u <<= 13
# asm 1: pslld $13,<u=int6464#3
# asm 2: pslld $13,<u=%xmm2
pslld $13,%xmm2

# qhasm:                                                   s ^= u
# asm 1: pxor  <u=int6464#3,<s=int6464#7
# asm 2: pxor  <u=%xmm2,<s=%xmm6
pxor  %xmm2,%xmm6

# qhasm:                                                                   mt = z11_stack
# asm 1: movdqa <z11_stack=stack128#23,>mt=int6464#3
# asm 2: movdqa <z11_stack=352(%rsp),>mt=%xmm2
movdqa 352(%rsp),%xmm2

# qhasm:                                                                   mp = z15_stack
# asm 1: movdqa <z15_stack=stack128#19,>mp=int6464#5
# asm 2: movdqa <z15_stack=288(%rsp),>mp=%xmm4
movdqa 288(%rsp),%xmm4

# qhasm:                                                                   mq = z3_stack
# asm 1: movdqa <z3_stack=stack128#21,>mq=int6464#4
# asm 2: movdqa <z3_stack=320(%rsp),>mq=%xmm3
movdqa 320(%rsp),%xmm3

# qhasm:                                                                   mr = z7_stack
# asm 1: movdqa <z7_stack=stack128#25,>mr=int6464#6
# asm 2: movdqa <z7_stack=384(%rsp),>mr=%xmm5
movdqa 384(%rsp),%xmm5

# qhasm:                                                   z6_stack = s
# asm 1: movdqa <s=int6464#7,>z6_stack=stack128#19
# asm 2: movdqa <s=%xmm6,>z6_stack=288(%rsp)
movdqa %xmm6,288(%rsp)

# qhasm: uint32323232                                      r += s
# asm 1: paddd <s=int6464#7,<r=int6464#2
# asm 2: paddd <s=%xmm6,<r=%xmm1
paddd %xmm6,%xmm1

# qhasm:                                                   u = r
# asm 1: movdqa <r=int6464#2,>u=int6464#7
# asm 2: movdqa <r=%xmm1,>u=%xmm6
movdqa %xmm1,%xmm6

# qhasm: uint32323232                                      r >>= 14
# asm 1: psrld $14,<r=int6464#2
# asm 2: psrld $14,<r=%xmm1
psrld $14,%xmm1

# qhasm:                                                   p ^= r
# asm 1: pxor  <r=int6464#2,<p=int6464#1
# asm 2: pxor  <r=%xmm1,<p=%xmm0
pxor  %xmm1,%xmm0

# qhasm: uint32323232                                      u <<= 18
# asm 1: pslld $18,<u=int6464#7
# asm 2: pslld $18,<u=%xmm6
pslld $18,%xmm6

# qhasm:                                                   p ^= u
# asm 1: pxor  <u=int6464#7,<p=int6464#1
# asm 2: pxor  <u=%xmm6,<p=%xmm0
pxor  %xmm6,%xmm0

# qhasm:                                                   z10_stack = p
# asm 1: movdqa <p=int6464#1,>z10_stack=stack128#23
# asm 2: movdqa <p=%xmm0,>z10_stack=352(%rsp)
movdqa %xmm0,352(%rsp)

# qhasm: 		  						  assign xmm2 to mt

# qhasm: 						  		  assign xmm3 to mq

# qhasm: 						  		  assign xmm4 to mp

# qhasm: 						  		  assign xmm5 to mr

# qhasm:                                                                   ms = mt
# asm 1: movdqa <mt=int6464#3,>ms=int6464#7
# asm 2: movdqa <mt=%xmm2,>ms=%xmm6
movdqa %xmm2,%xmm6

# qhasm: uint32323232                                                      mt += mp
# asm 1: paddd <mp=int6464#5,<mt=int6464#3
# asm 2: paddd <mp=%xmm4,<mt=%xmm2
paddd %xmm4,%xmm2

# qhasm:                                                                   mu = mt
# asm 1: movdqa <mt=int6464#3,>mu=int6464#1
# asm 2: movdqa <mt=%xmm2,>mu=%xmm0
movdqa %xmm2,%xmm0

# qhasm: uint32323232                                                      mt >>= 25
# asm 1: psrld $25,<mt=int6464#3
# asm 2: psrld $25,<mt=%xmm2
psrld $25,%xmm2

# qhasm:                                                                   mq ^= mt
# asm 1: pxor  <mt=int6464#3,<mq=int6464#4
# asm 2: pxor  <mt=%xmm2,<mq=%xmm3
pxor  %xmm2,%xmm3

# qhasm: uint32323232                                                      mu <<= 7
# asm 1: pslld $7,<mu=int6464#1
# asm 2: pslld $7,<mu=%xmm0
pslld $7,%xmm0

# qhasm:                                                                   mq ^= mu
# asm 1: pxor  <mu=int6464#1,<mq=int6464#4
# asm 2: pxor  <mu=%xmm0,<mq=%xmm3
pxor  %xmm0,%xmm3

# qhasm:                                                                   z3_stack = mq
# asm 1: movdqa <mq=int6464#4,>z3_stack=stack128#21
# asm 2: movdqa <mq=%xmm3,>z3_stack=320(%rsp)
movdqa %xmm3,320(%rsp)

# qhasm:                                                                   mt = mp
# asm 1: movdqa <mp=int6464#5,>mt=int6464#1
# asm 2: movdqa <mp=%xmm4,>mt=%xmm0
movdqa %xmm4,%xmm0

# qhasm: uint32323232                                                      mt += mq
# asm 1: paddd <mq=int6464#4,<mt=int6464#1
# asm 2: paddd <mq=%xmm3,<mt=%xmm0
paddd %xmm3,%xmm0

# qhasm:                                                                   mu = mt
# asm 1: movdqa <mt=int6464#1,>mu=int6464#2
# asm 2: movdqa <mt=%xmm0,>mu=%xmm1
movdqa %xmm0,%xmm1

# qhasm: uint32323232                                                      mt >>= 23
# asm 1: psrld $23,<mt=int6464#1
# asm 2: psrld $23,<mt=%xmm0
psrld $23,%xmm0

# qhasm:                                                                   mr ^= mt
# asm 1: pxor  <mt=int6464#1,<mr=int6464#6
# asm 2: pxor  <mt=%xmm0,<mr=%xmm5
pxor  %xmm0,%xmm5

# qhasm: uint32323232                                                      mu <<= 9
# asm 1: pslld $9,<mu=int6464#2
# asm 2: pslld $9,<mu=%xmm1
pslld $9,%xmm1

# qhasm:                                                                   mr ^= mu
# asm 1: pxor  <mu=int6464#2,<mr=int6464#6
# asm 2: pxor  <mu=%xmm1,<mr=%xmm5
pxor  %xmm1,%xmm5

# qhasm:                                                                   z7_stack = mr
# asm 1: movdqa <mr=int6464#6,>z7_stack=stack128#25
# asm 2: movdqa <mr=%xmm5,>z7_stack=384(%rsp)
movdqa %xmm5,384(%rsp)

# qhasm: uint32323232                                                      mq += mr
# asm 1: paddd <mr=int6464#6,<mq=int6464#4
# asm 2: paddd <mr=%xmm5,<mq=%xmm3
paddd %xmm5,%xmm3

# qhasm:                                                                   mu = mq
# asm 1: movdqa <mq=int6464#4,>mu=int6464#1
# asm 2: movdqa <mq=%xmm3,>mu=%xmm0
movdqa %xmm3,%xmm0

# qhasm: uint32323232                                                      mq >>= 19
# asm 1: psrld $19,<mq=int6464#4
# asm 2: psrld $19,<mq=%xmm3
psrld $19,%xmm3

# qhasm:                                                                   ms ^= mq
# asm 1: pxor  <mq=int6464#4,<ms=int6464#7
# asm 2: pxor  <mq=%xmm3,<ms=%xmm6
pxor  %xmm3,%xmm6

# qhasm: uint32323232                                                      mu <<= 13
# asm 1: pslld $13,<mu=int6464#1
# asm 2: pslld $13,<mu=%xmm0
pslld $13,%xmm0

# qhasm:                                                                   ms ^= mu
# asm 1: pxor  <mu=int6464#1,<ms=int6464#7
# asm 2: pxor  <mu=%xmm0,<ms=%xmm6
pxor  %xmm0,%xmm6

# qhasm:                   t = z3_stack
# asm 1: movdqa <z3_stack=stack128#21,>t=int6464#3
# asm 2: movdqa <z3_stack=320(%rsp),>t=%xmm2
movdqa 320(%rsp),%xmm2

# qhasm:                   p = z0_stack
# asm 1: movdqa <z0_stack=stack128#17,>p=int6464#1
# asm 2: movdqa <z0_stack=256(%rsp),>p=%xmm0
movdqa 256(%rsp),%xmm0

# qhasm:                   q = z1_stack
# asm 1: movdqa <z1_stack=stack128#18,>q=int6464#4
# asm 2: movdqa <z1_stack=272(%rsp),>q=%xmm3
movdqa 272(%rsp),%xmm3

# qhasm:                   r = z2_stack
# asm 1: movdqa <z2_stack=stack128#22,>r=int6464#2
# asm 2: movdqa <z2_stack=336(%rsp),>r=%xmm1
movdqa 336(%rsp),%xmm1

# qhasm:                                                                   z11_stack = ms
# asm 1: movdqa <ms=int6464#7,>z11_stack=stack128#17
# asm 2: movdqa <ms=%xmm6,>z11_stack=256(%rsp)
movdqa %xmm6,256(%rsp)

# qhasm: uint32323232                                                      mr += ms
# asm 1: paddd <ms=int6464#7,<mr=int6464#6
# asm 2: paddd <ms=%xmm6,<mr=%xmm5
paddd %xmm6,%xmm5

# qhasm:                                                                   mu = mr
# asm 1: movdqa <mr=int6464#6,>mu=int6464#7
# asm 2: movdqa <mr=%xmm5,>mu=%xmm6
movdqa %xmm5,%xmm6

# qhasm: uint32323232                                                      mr >>= 14
# asm 1: psrld $14,<mr=int6464#6
# asm 2: psrld $14,<mr=%xmm5
psrld $14,%xmm5

# qhasm:                                                                   mp ^= mr
# asm 1: pxor  <mr=int6464#6,<mp=int6464#5
# asm 2: pxor  <mr=%xmm5,<mp=%xmm4
pxor  %xmm5,%xmm4

# qhasm: uint32323232                                                      mu <<= 18
# asm 1: pslld $18,<mu=int6464#7
# asm 2: pslld $18,<mu=%xmm6
pslld $18,%xmm6

# qhasm:                                                                   mp ^= mu
# asm 1: pxor  <mu=int6464#7,<mp=int6464#5
# asm 2: pxor  <mu=%xmm6,<mp=%xmm4
pxor  %xmm6,%xmm4

# qhasm:                                                                   z15_stack = mp
# asm 1: movdqa <mp=int6464#5,>z15_stack=stack128#18
# asm 2: movdqa <mp=%xmm4,>z15_stack=272(%rsp)
movdqa %xmm4,272(%rsp)

# qhasm: 		  assign xmm0 to p

# qhasm: 		  assign xmm1 to r

# qhasm: 		  assign xmm2 to t

# qhasm: 		  assign xmm3 to q

# qhasm:                   s = t
# asm 1: movdqa <t=int6464#3,>s=int6464#7
# asm 2: movdqa <t=%xmm2,>s=%xmm6
movdqa %xmm2,%xmm6

# qhasm: uint32323232      t += p
# asm 1: paddd <p=int6464#1,<t=int6464#3
# asm 2: paddd <p=%xmm0,<t=%xmm2
paddd %xmm0,%xmm2

# qhasm:                   u = t
# asm 1: movdqa <t=int6464#3,>u=int6464#5
# asm 2: movdqa <t=%xmm2,>u=%xmm4
movdqa %xmm2,%xmm4

# qhasm: uint32323232      t >>= 25
# asm 1: psrld $25,<t=int6464#3
# asm 2: psrld $25,<t=%xmm2
psrld $25,%xmm2

# qhasm:                   q ^= t
# asm 1: pxor  <t=int6464#3,<q=int6464#4
# asm 2: pxor  <t=%xmm2,<q=%xmm3
pxor  %xmm2,%xmm3

# qhasm: uint32323232      u <<= 7
# asm 1: pslld $7,<u=int6464#5
# asm 2: pslld $7,<u=%xmm4
pslld $7,%xmm4

# qhasm:                   q ^= u
# asm 1: pxor  <u=int6464#5,<q=int6464#4
# asm 2: pxor  <u=%xmm4,<q=%xmm3
pxor  %xmm4,%xmm3

# qhasm:                   z1_stack = q
# asm 1: movdqa <q=int6464#4,>z1_stack=stack128#24
# asm 2: movdqa <q=%xmm3,>z1_stack=368(%rsp)
movdqa %xmm3,368(%rsp)

# qhasm:                   t = p
# asm 1: movdqa <p=int6464#1,>t=int6464#3
# asm 2: movdqa <p=%xmm0,>t=%xmm2
movdqa %xmm0,%xmm2

# qhasm: uint32323232      t += q
# asm 1: paddd <q=int6464#4,<t=int6464#3
# asm 2: paddd <q=%xmm3,<t=%xmm2
paddd %xmm3,%xmm2

# qhasm:                   u = t
# asm 1: movdqa <t=int6464#3,>u=int6464#5
# asm 2: movdqa <t=%xmm2,>u=%xmm4
movdqa %xmm2,%xmm4

# qhasm: uint32323232      t >>= 23
# asm 1: psrld $23,<t=int6464#3
# asm 2: psrld $23,<t=%xmm2
psrld $23,%xmm2

# qhasm:                   r ^= t
# asm 1: pxor  <t=int6464#3,<r=int6464#2
# asm 2: pxor  <t=%xmm2,<r=%xmm1
pxor  %xmm2,%xmm1

# qhasm: uint32323232      u <<= 9
# asm 1: pslld $9,<u=int6464#5
# asm 2: pslld $9,<u=%xmm4
pslld $9,%xmm4

# qhasm:                   r ^= u
# asm 1: pxor  <u=int6464#5,<r=int6464#2
# asm 2: pxor  <u=%xmm4,<r=%xmm1
pxor  %xmm4,%xmm1

# qhasm:                   z2_stack = r
# asm 1: movdqa <r=int6464#2,>z2_stack=stack128#27
# asm 2: movdqa <r=%xmm1,>z2_stack=416(%rsp)
movdqa %xmm1,416(%rsp)

# qhasm: uint32323232      q += r
# asm 1: paddd <r=int6464#2,<q=int6464#4
# asm 2: paddd <r=%xmm1,<q=%xmm3
paddd %xmm1,%xmm3

# qhasm:                   u = q
# asm 1: movdqa <q=int6464#4,>u=int6464#3
# asm 2: movdqa <q=%xmm3,>u=%xmm2
movdqa %xmm3,%xmm2

# qhasm: uint32323232      q >>= 19
# asm 1: psrld $19,<q=int6464#4
# asm 2: psrld $19,<q=%xmm3
psrld $19,%xmm3

# qhasm:                   s ^= q
# asm 1: pxor  <q=int6464#4,<s=int6464#7
# asm 2: pxor  <q=%xmm3,<s=%xmm6
pxor  %xmm3,%xmm6

# qhasm: uint32323232      u <<= 13
# asm 1: pslld $13,<u=int6464#3
# asm 2: pslld $13,<u=%xmm2
pslld $13,%xmm2

# qhasm:                   s ^= u
# asm 1: pxor  <u=int6464#3,<s=int6464#7
# asm 2: pxor  <u=%xmm2,<s=%xmm6
pxor  %xmm2,%xmm6

# qhasm:                                   mt = z4_stack
# asm 1: movdqa <z4_stack=stack128#29,>mt=int6464#3
# asm 2: movdqa <z4_stack=448(%rsp),>mt=%xmm2
movdqa 448(%rsp),%xmm2

# qhasm:                                   mp = z5_stack
# asm 1: movdqa <z5_stack=stack128#20,>mp=int6464#5
# asm 2: movdqa <z5_stack=304(%rsp),>mp=%xmm4
movdqa 304(%rsp),%xmm4

# qhasm:                                   mq = z6_stack
# asm 1: movdqa <z6_stack=stack128#19,>mq=int6464#4
# asm 2: movdqa <z6_stack=288(%rsp),>mq=%xmm3
movdqa 288(%rsp),%xmm3

# qhasm:                                   mr = z7_stack
# asm 1: movdqa <z7_stack=stack128#25,>mr=int6464#6
# asm 2: movdqa <z7_stack=384(%rsp),>mr=%xmm5
movdqa 384(%rsp),%xmm5

# qhasm:                   z3_stack = s
# asm 1: movdqa <s=int6464#7,>z3_stack=stack128#21
# asm 2: movdqa <s=%xmm6,>z3_stack=320(%rsp)
movdqa %xmm6,320(%rsp)

# qhasm: uint32323232      r += s
# asm 1: paddd <s=int6464#7,<r=int6464#2
# asm 2: paddd <s=%xmm6,<r=%xmm1
paddd %xmm6,%xmm1

# qhasm:                   u = r
# asm 1: movdqa <r=int6464#2,>u=int6464#7
# asm 2: movdqa <r=%xmm1,>u=%xmm6
movdqa %xmm1,%xmm6

# qhasm: uint32323232      r >>= 14
# asm 1: psrld $14,<r=int6464#2
# asm 2: psrld $14,<r=%xmm1
psrld $14,%xmm1

# qhasm:                   p ^= r
# asm 1: pxor  <r=int6464#2,<p=int6464#1
# asm 2: pxor  <r=%xmm1,<p=%xmm0
pxor  %xmm1,%xmm0

# qhasm: uint32323232      u <<= 18
# asm 1: pslld $18,<u=int6464#7
# asm 2: pslld $18,<u=%xmm6
pslld $18,%xmm6

# qhasm:                   p ^= u
# asm 1: pxor  <u=int6464#7,<p=int6464#1
# asm 2: pxor  <u=%xmm6,<p=%xmm0
pxor  %xmm6,%xmm0

# qhasm:                   z0_stack = p
# asm 1: movdqa <p=int6464#1,>z0_stack=stack128#29
# asm 2: movdqa <p=%xmm0,>z0_stack=448(%rsp)
movdqa %xmm0,448(%rsp)

# qhasm: 		  		  assign xmm2 to mt

# qhasm: 		  		  assign xmm3 to mq

# qhasm: 		  		  assign xmm4 to mp

# qhasm: 		  		  assign xmm5 to mr

# qhasm:                                   ms = mt
# asm 1: movdqa <mt=int6464#3,>ms=int6464#7
# asm 2: movdqa <mt=%xmm2,>ms=%xmm6
movdqa %xmm2,%xmm6

# qhasm: uint32323232                      mt += mp
# asm 1: paddd <mp=int6464#5,<mt=int6464#3
# asm 2: paddd <mp=%xmm4,<mt=%xmm2
paddd %xmm4,%xmm2

# qhasm:                                   mu = mt
# asm 1: movdqa <mt=int6464#3,>mu=int6464#1
# asm 2: movdqa <mt=%xmm2,>mu=%xmm0
movdqa %xmm2,%xmm0

# qhasm: uint32323232                      mt >>= 25
# asm 1: psrld $25,<mt=int6464#3
# asm 2: psrld $25,<mt=%xmm2
psrld $25,%xmm2

# qhasm:                                   mq ^= mt
# asm 1: pxor  <mt=int6464#3,<mq=int6464#4
# asm 2: pxor  <mt=%xmm2,<mq=%xmm3
pxor  %xmm2,%xmm3

# qhasm: uint32323232                      mu <<= 7
# asm 1: pslld $7,<mu=int6464#1
# asm 2: pslld $7,<mu=%xmm0
pslld $7,%xmm0

# qhasm:                                   mq ^= mu
# asm 1: pxor  <mu=int6464#1,<mq=int6464#4
# asm 2: pxor  <mu=%xmm0,<mq=%xmm3
pxor  %xmm0,%xmm3

# qhasm:                                   z6_stack = mq
# asm 1: movdqa <mq=int6464#4,>z6_stack=stack128#22
# asm 2: movdqa <mq=%xmm3,>z6_stack=336(%rsp)
movdqa %xmm3,336(%rsp)

# qhasm:                                   mt = mp
# asm 1: movdqa <mp=int6464#5,>mt=int6464#1
# asm 2: movdqa <mp=%xmm4,>mt=%xmm0
movdqa %xmm4,%xmm0

# qhasm: uint32323232                      mt += mq
# asm 1: paddd <mq=int6464#4,<mt=int6464#1
# asm 2: paddd <mq=%xmm3,<mt=%xmm0
paddd %xmm3,%xmm0

# qhasm:                                   mu = mt
# asm 1: movdqa <mt=int6464#1,>mu=int6464#2
# asm 2: movdqa <mt=%xmm0,>mu=%xmm1
movdqa %xmm0,%xmm1

# qhasm: uint32323232                      mt >>= 23
# asm 1: psrld $23,<mt=int6464#1
# asm 2: psrld $23,<mt=%xmm0
psrld $23,%xmm0

# qhasm:                                   mr ^= mt
# asm 1: pxor  <mt=int6464#1,<mr=int6464#6
# asm 2: pxor  <mt=%xmm0,<mr=%xmm5
pxor  %xmm0,%xmm5

# qhasm: uint32323232                      mu <<= 9
# asm 1: pslld $9,<mu=int6464#2
# asm 2: pslld $9,<mu=%xmm1
pslld $9,%xmm1

# qhasm:                                   mr ^= mu
# asm 1: pxor  <mu=int6464#2,<mr=int6464#6
# asm 2: pxor  <mu=%xmm1,<mr=%xmm5
pxor  %xmm1,%xmm5

# qhasm:                                   z7_stack = mr
# asm 1: movdqa <mr=int6464#6,>z7_stack=stack128#25
# asm 2: movdqa <mr=%xmm5,>z7_stack=384(%rsp)
movdqa %xmm5,384(%rsp)

# qhasm: uint32323232                      mq += mr
# asm 1: paddd <mr=int6464#6,<mq=int6464#4
# asm 2: paddd <mr=%xmm5,<mq=%xmm3
paddd %xmm5,%xmm3

# qhasm:                                   mu = mq
# asm 1: movdqa <mq=int6464#4,>mu=int6464#1
# asm 2: movdqa <mq=%xmm3,>mu=%xmm0
movdqa %xmm3,%xmm0

# qhasm: uint32323232                      mq >>= 19
# asm 1: psrld $19,<mq=int6464#4
# asm 2: psrld $19,<mq=%xmm3
psrld $19,%xmm3

# qhasm:                                   ms ^= mq
# asm 1: pxor  <mq=int6464#4,<ms=int6464#7
# asm 2: pxor  <mq=%xmm3,<ms=%xmm6
pxor  %xmm3,%xmm6

# qhasm: uint32323232                      mu <<= 13
# asm 1: pslld $13,<mu=int6464#1
# asm 2: pslld $13,<mu=%xmm0
pslld $13,%xmm0

# qhasm:                                   ms ^= mu
# asm 1: pxor  <mu=int6464#1,<ms=int6464#7
# asm 2: pxor  <mu=%xmm0,<ms=%xmm6
pxor  %xmm0,%xmm6

# qhasm:                                                   t = z9_stack
# asm 1: movdqa <z9_stack=stack128#28,>t=int6464#3
# asm 2: movdqa <z9_stack=432(%rsp),>t=%xmm2
movdqa 432(%rsp),%xmm2

# qhasm:                                                   p = z10_stack
# asm 1: movdqa <z10_stack=stack128#23,>p=int6464#1
# asm 2: movdqa <z10_stack=352(%rsp),>p=%xmm0
movdqa 352(%rsp),%xmm0

# qhasm:                                                   q = z11_stack
# asm 1: movdqa <z11_stack=stack128#17,>q=int6464#4
# asm 2: movdqa <z11_stack=256(%rsp),>q=%xmm3
movdqa 256(%rsp),%xmm3

# qhasm:                                                   r = z8_stack
# asm 1: movdqa <z8_stack=stack128#30,>r=int6464#2
# asm 2: movdqa <z8_stack=464(%rsp),>r=%xmm1
movdqa 464(%rsp),%xmm1

# qhasm:                                   z4_stack = ms
# asm 1: movdqa <ms=int6464#7,>z4_stack=stack128#30
# asm 2: movdqa <ms=%xmm6,>z4_stack=464(%rsp)
movdqa %xmm6,464(%rsp)

# qhasm: uint32323232                      mr += ms
# asm 1: paddd <ms=int6464#7,<mr=int6464#6
# asm 2: paddd <ms=%xmm6,<mr=%xmm5
paddd %xmm6,%xmm5

# qhasm:                                   mu = mr
# asm 1: movdqa <mr=int6464#6,>mu=int6464#7
# asm 2: movdqa <mr=%xmm5,>mu=%xmm6
movdqa %xmm5,%xmm6

# qhasm: uint32323232                      mr >>= 14
# asm 1: psrld $14,<mr=int6464#6
# asm 2: psrld $14,<mr=%xmm5
psrld $14,%xmm5

# qhasm:                                   mp ^= mr
# asm 1: pxor  <mr=int6464#6,<mp=int6464#5
# asm 2: pxor  <mr=%xmm5,<mp=%xmm4
pxor  %xmm5,%xmm4

# qhasm: uint32323232                      mu <<= 18
# asm 1: pslld $18,<mu=int6464#7
# asm 2: pslld $18,<mu=%xmm6
pslld $18,%xmm6

# qhasm:                                   mp ^= mu
# asm 1: pxor  <mu=int6464#7,<mp=int6464#5
# asm 2: pxor  <mu=%xmm6,<mp=%xmm4
pxor  %xmm6,%xmm4

# qhasm:                                   z5_stack = mp
# asm 1: movdqa <mp=int6464#5,>z5_stack=stack128#17
# asm 2: movdqa <mp=%xmm4,>z5_stack=256(%rsp)
movdqa %xmm4,256(%rsp)

# qhasm: 		  				  assign xmm0 to p

# qhasm: 						  assign xmm1 to r

# qhasm: 						  assign xmm2 to t

# qhasm: 						  assign xmm3 to q

# qhasm:                                                   s = t
# asm 1: movdqa <t=int6464#3,>s=int6464#7
# asm 2: movdqa <t=%xmm2,>s=%xmm6
movdqa %xmm2,%xmm6

# qhasm: uint32323232                                      t += p
# asm 1: paddd <p=int6464#1,<t=int6464#3
# asm 2: paddd <p=%xmm0,<t=%xmm2
paddd %xmm0,%xmm2

# qhasm:                                                   u = t
# asm 1: movdqa <t=int6464#3,>u=int6464#5
# asm 2: movdqa <t=%xmm2,>u=%xmm4
movdqa %xmm2,%xmm4

# qhasm: uint32323232                                      t >>= 25
# asm 1: psrld $25,<t=int6464#3
# asm 2: psrld $25,<t=%xmm2
psrld $25,%xmm2

# qhasm:                                                   q ^= t
# asm 1: pxor  <t=int6464#3,<q=int6464#4
# asm 2: pxor  <t=%xmm2,<q=%xmm3
pxor  %xmm2,%xmm3

# qhasm: uint32323232                                      u <<= 7
# asm 1: pslld $7,<u=int6464#5
# asm 2: pslld $7,<u=%xmm4
pslld $7,%xmm4

# qhasm:                                                   q ^= u
# asm 1: pxor  <u=int6464#5,<q=int6464#4
# asm 2: pxor  <u=%xmm4,<q=%xmm3
pxor  %xmm4,%xmm3

# qhasm:                                                   z11_stack = q
# asm 1: movdqa <q=int6464#4,>z11_stack=stack128#23
# asm 2: movdqa <q=%xmm3,>z11_stack=352(%rsp)
movdqa %xmm3,352(%rsp)

# qhasm:                                                   t = p
# asm 1: movdqa <p=int6464#1,>t=int6464#3
# asm 2: movdqa <p=%xmm0,>t=%xmm2
movdqa %xmm0,%xmm2

# qhasm: uint32323232                                      t += q
# asm 1: paddd <q=int6464#4,<t=int6464#3
# asm 2: paddd <q=%xmm3,<t=%xmm2
paddd %xmm3,%xmm2

# qhasm:                                                   u = t
# asm 1: movdqa <t=int6464#3,>u=int6464#5
# asm 2: movdqa <t=%xmm2,>u=%xmm4
movdqa %xmm2,%xmm4

# qhasm: uint32323232                                      t >>= 23
# asm 1: psrld $23,<t=int6464#3
# asm 2: psrld $23,<t=%xmm2
psrld $23,%xmm2

# qhasm:                                                   r ^= t
# asm 1: pxor  <t=int6464#3,<r=int6464#2
# asm 2: pxor  <t=%xmm2,<r=%xmm1
pxor  %xmm2,%xmm1

# qhasm: uint32323232                                      u <<= 9
# asm 1: pslld $9,<u=int6464#5
# asm 2: pslld $9,<u=%xmm4
pslld $9,%xmm4

# qhasm:                                                   r ^= u
# asm 1: pxor  <u=int6464#5,<r=int6464#2
# asm 2: pxor  <u=%xmm4,<r=%xmm1
pxor  %xmm4,%xmm1

# qhasm:                                                   z8_stack = r
# asm 1: movdqa <r=int6464#2,>z8_stack=stack128#33
# asm 2: movdqa <r=%xmm1,>z8_stack=512(%rsp)
movdqa %xmm1,512(%rsp)

# qhasm: uint32323232                                      q += r
# asm 1: paddd <r=int6464#2,<q=int6464#4
# asm 2: paddd <r=%xmm1,<q=%xmm3
paddd %xmm1,%xmm3

# qhasm:                                                   u = q
# asm 1: movdqa <q=int6464#4,>u=int6464#3
# asm 2: movdqa <q=%xmm3,>u=%xmm2
movdqa %xmm3,%xmm2

# qhasm: uint32323232                                      q >>= 19
# asm 1: psrld $19,<q=int6464#4
# asm 2: psrld $19,<q=%xmm3
psrld $19,%xmm3

# qhasm:                                                   s ^= q
# asm 1: pxor  <q=int6464#4,<s=int6464#7
# asm 2: pxor  <q=%xmm3,<s=%xmm6
pxor  %xmm3,%xmm6

# qhasm: uint32323232                                      u <<= 13
# asm 1: pslld $13,<u=int6464#3
# asm 2: pslld $13,<u=%xmm2
pslld $13,%xmm2

# qhasm:                                                   s ^= u
# asm 1: pxor  <u=int6464#3,<s=int6464#7
# asm 2: pxor  <u=%xmm2,<s=%xmm6
pxor  %xmm2,%xmm6

# qhasm:                                                                   mt = z14_stack
# asm 1: movdqa <z14_stack=stack128#32,>mt=int6464#3
# asm 2: movdqa <z14_stack=496(%rsp),>mt=%xmm2
movdqa 496(%rsp),%xmm2

# qhasm:                                                                   mp = z15_stack
# asm 1: movdqa <z15_stack=stack128#18,>mp=int6464#5
# asm 2: movdqa <z15_stack=272(%rsp),>mp=%xmm4
movdqa 272(%rsp),%xmm4

# qhasm:                                                                   mq = z12_stack
# asm 1: movdqa <z12_stack=stack128#26,>mq=int6464#4
# asm 2: movdqa <z12_stack=400(%rsp),>mq=%xmm3
movdqa 400(%rsp),%xmm3

# qhasm:                                                                   mr = z13_stack
# asm 1: movdqa <z13_stack=stack128#31,>mr=int6464#6
# asm 2: movdqa <z13_stack=480(%rsp),>mr=%xmm5
movdqa 480(%rsp),%xmm5

# qhasm:                                                   z9_stack = s
# asm 1: movdqa <s=int6464#7,>z9_stack=stack128#28
# asm 2: movdqa <s=%xmm6,>z9_stack=432(%rsp)
movdqa %xmm6,432(%rsp)

# qhasm: uint32323232                                      r += s
# asm 1: paddd <s=int6464#7,<r=int6464#2
# asm 2: paddd <s=%xmm6,<r=%xmm1
paddd %xmm6,%xmm1

# qhasm:                                                   u = r
# asm 1: movdqa <r=int6464#2,>u=int6464#7
# asm 2: movdqa <r=%xmm1,>u=%xmm6
movdqa %xmm1,%xmm6

# qhasm: uint32323232                                      r >>= 14
# asm 1: psrld $14,<r=int6464#2
# asm 2: psrld $14,<r=%xmm1
psrld $14,%xmm1

# qhasm:                                                   p ^= r
# asm 1: pxor  <r=int6464#2,<p=int6464#1
# asm 2: pxor  <r=%xmm1,<p=%xmm0
pxor  %xmm1,%xmm0

# qhasm: uint32323232                                      u <<= 18
# asm 1: pslld $18,<u=int6464#7
# asm 2: pslld $18,<u=%xmm6
pslld $18,%xmm6

# qhasm:                                                   p ^= u
# asm 1: pxor  <u=int6464#7,<p=int6464#1
# asm 2: pxor  <u=%xmm6,<p=%xmm0
pxor  %xmm6,%xmm0

# qhasm:                                                   z10_stack = p
# asm 1: movdqa <p=int6464#1,>z10_stack=stack128#18
# asm 2: movdqa <p=%xmm0,>z10_stack=272(%rsp)
movdqa %xmm0,272(%rsp)

# qhasm: 		  						  assign xmm2 to mt

# qhasm: 						  		  assign xmm3 to mq

# qhasm: 						  		  assign xmm4 to mp

# qhasm: 						  		  assign xmm5 to mr

# qhasm:                                                                   ms = mt
# asm 1: movdqa <mt=int6464#3,>ms=int6464#7
# asm 2: movdqa <mt=%xmm2,>ms=%xmm6
movdqa %xmm2,%xmm6

# qhasm: uint32323232                                                      mt += mp
# asm 1: paddd <mp=int6464#5,<mt=int6464#3
# asm 2: paddd <mp=%xmm4,<mt=%xmm2
paddd %xmm4,%xmm2

# qhasm:                                                                   mu = mt
# asm 1: movdqa <mt=int6464#3,>mu=int6464#1
# asm 2: movdqa <mt=%xmm2,>mu=%xmm0
movdqa %xmm2,%xmm0

# qhasm: uint32323232                                                      mt >>= 25
# asm 1: psrld $25,<mt=int6464#3
# asm 2: psrld $25,<mt=%xmm2
psrld $25,%xmm2

# qhasm:                                                                   mq ^= mt
# asm 1: pxor  <mt=int6464#3,<mq=int6464#4
# asm 2: pxor  <mt=%xmm2,<mq=%xmm3
pxor  %xmm2,%xmm3

# qhasm: uint32323232                                                      mu <<= 7
# asm 1: pslld $7,<mu=int6464#1
# asm 2: pslld $7,<mu=%xmm0
pslld $7,%xmm0

# qhasm:                                                                   mq ^= mu
# asm 1: pxor  <mu=int6464#1,<mq=int6464#4
# asm 2: pxor  <mu=%xmm0,<mq=%xmm3
pxor  %xmm0,%xmm3

# qhasm:                                                                   z12_stack = mq
# asm 1: movdqa <mq=int6464#4,>z12_stack=stack128#31
# asm 2: movdqa <mq=%xmm3,>z12_stack=480(%rsp)
movdqa %xmm3,480(%rsp)

# qhasm:                                                                   mt = mp
# asm 1: movdqa <mp=int6464#5,>mt=int6464#1
# asm 2: movdqa <mp=%xmm4,>mt=%xmm0
movdqa %xmm4,%xmm0

# qhasm: uint32323232                                                      mt += mq
# asm 1: paddd <mq=int6464#4,<mt=int6464#1
# asm 2: paddd <mq=%xmm3,<mt=%xmm0
paddd %xmm3,%xmm0

# qhasm:                                                                   mu = mt
# asm 1: movdqa <mt=int6464#1,>mu=int6464#2
# asm 2: movdqa <mt=%xmm0,>mu=%xmm1
movdqa %xmm0,%xmm1

# qhasm: uint32323232                                                      mt >>= 23
# asm 1: psrld $23,<mt=int6464#1
# asm 2: psrld $23,<mt=%xmm0
psrld $23,%xmm0

# qhasm:                                                                   mr ^= mt
# asm 1: pxor  <mt=int6464#1,<mr=int6464#6
# asm 2: pxor  <mt=%xmm0,<mr=%xmm5
pxor  %xmm0,%xmm5

# qhasm: uint32323232                                                      mu <<= 9
# asm 1: pslld $9,<mu=int6464#2
# asm 2: pslld $9,<mu=%xmm1
pslld $9,%xmm1

# qhasm:                                                                   mr ^= mu
# asm 1: pxor  <mu=int6464#2,<mr=int6464#6
# asm 2: pxor  <mu=%xmm1,<mr=%xmm5
pxor  %xmm1,%xmm5

# qhasm:                                                                   z13_stack = mr
# asm 1: movdqa <mr=int6464#6,>z13_stack=stack128#26
# asm 2: movdqa <mr=%xmm5,>z13_stack=400(%rsp)
movdqa %xmm5,400(%rsp)

# qhasm: uint32323232                                                      mq += mr
# asm 1: paddd <mr=int6464#6,<mq=int6464#4
# asm 2: paddd <mr=%xmm5,<mq=%xmm3
paddd %xmm5,%xmm3

# qhasm:                                                                   mu = mq
# asm 1: movdqa <mq=int6464#4,>mu=int6464#1
# asm 2: movdqa <mq=%xmm3,>mu=%xmm0
movdqa %xmm3,%xmm0

# qhasm: uint32323232                                                      mq >>= 19
# asm 1: psrld $19,<mq=int6464#4
# asm 2: psrld $19,<mq=%xmm3
psrld $19,%xmm3

# qhasm:                                                                   ms ^= mq
# asm 1: pxor  <mq=int6464#4,<ms=int6464#7
# asm 2: pxor  <mq=%xmm3,<ms=%xmm6
pxor  %xmm3,%xmm6

# qhasm: uint32323232                                                      mu <<= 13
# asm 1: pslld $13,<mu=int6464#1
# asm 2: pslld $13,<mu=%xmm0
pslld $13,%xmm0

# qhasm:                                                                   ms ^= mu
# asm 1: pxor  <mu=int6464#1,<ms=int6464#7
# asm 2: pxor  <mu=%xmm0,<ms=%xmm6
pxor  %xmm0,%xmm6

# qhasm:                   t = z12_stack
# asm 1: movdqa <z12_stack=stack128#31,>t=int6464#3
# asm 2: movdqa <z12_stack=480(%rsp),>t=%xmm2
movdqa 480(%rsp),%xmm2

# qhasm:                   p = z0_stack
# asm 1: movdqa <z0_stack=stack128#29,>p=int6464#1
# asm 2: movdqa <z0_stack=448(%rsp),>p=%xmm0
movdqa 448(%rsp),%xmm0

# qhasm:                   q = z4_stack
# asm 1: movdqa <z4_stack=stack128#30,>q=int6464#4
# asm 2: movdqa <z4_stack=464(%rsp),>q=%xmm3
movdqa 464(%rsp),%xmm3

# qhasm:                   r = z8_stack
# asm 1: movdqa <z8_stack=stack128#33,>r=int6464#2
# asm 2: movdqa <z8_stack=512(%rsp),>r=%xmm1
movdqa 512(%rsp),%xmm1

# qhasm:                                                                   z14_stack = ms
# asm 1: movdqa <ms=int6464#7,>z14_stack=stack128#20
# asm 2: movdqa <ms=%xmm6,>z14_stack=304(%rsp)
movdqa %xmm6,304(%rsp)

# qhasm: uint32323232                                                      mr += ms
# asm 1: paddd <ms=int6464#7,<mr=int6464#6
# asm 2: paddd <ms=%xmm6,<mr=%xmm5
paddd %xmm6,%xmm5

# qhasm:                                                                   mu = mr
# asm 1: movdqa <mr=int6464#6,>mu=int6464#7
# asm 2: movdqa <mr=%xmm5,>mu=%xmm6
movdqa %xmm5,%xmm6

# qhasm: uint32323232                                                      mr >>= 14
# asm 1: psrld $14,<mr=int6464#6
# asm 2: psrld $14,<mr=%xmm5
psrld $14,%xmm5

# qhasm:                                                                   mp ^= mr
# asm 1: pxor  <mr=int6464#6,<mp=int6464#5
# asm 2: pxor  <mr=%xmm5,<mp=%xmm4
pxor  %xmm5,%xmm4

# qhasm: uint32323232                                                      mu <<= 18
# asm 1: pslld $18,<mu=int6464#7
# asm 2: pslld $18,<mu=%xmm6
pslld $18,%xmm6

# qhasm:                                                                   mp ^= mu
# asm 1: pxor  <mu=int6464#7,<mp=int6464#5
# asm 2: pxor  <mu=%xmm6,<mp=%xmm4
pxor  %xmm6,%xmm4

# qhasm:                                                                   z15_stack = mp
# asm 1: movdqa <mp=int6464#5,>z15_stack=stack128#19
# asm 2: movdqa <mp=%xmm4,>z15_stack=288(%rsp)
movdqa %xmm4,288(%rsp)

# qhasm:                   unsigned>? i -= 2
# asm 1: sub  $2,<i=int64#3
# asm 2: sub  $2,<i=%rdx
sub  $2,%rdx
# comment:fp stack unchanged by jump

# qhasm: goto mainloop1 if unsigned>
ja ._mainloop1

# qhasm:   z0 = z0_stack
# asm 1: movdqa <z0_stack=stack128#29,>z0=int6464#1
# asm 2: movdqa <z0_stack=448(%rsp),>z0=%xmm0
movdqa 448(%rsp),%xmm0

# qhasm:   z1 = z1_stack
# asm 1: movdqa <z1_stack=stack128#24,>z1=int6464#2
# asm 2: movdqa <z1_stack=368(%rsp),>z1=%xmm1
movdqa 368(%rsp),%xmm1

# qhasm:   z2 = z2_stack
# asm 1: movdqa <z2_stack=stack128#27,>z2=int6464#3
# asm 2: movdqa <z2_stack=416(%rsp),>z2=%xmm2
movdqa 416(%rsp),%xmm2

# qhasm:   z3 = z3_stack
# asm 1: movdqa <z3_stack=stack128#21,>z3=int6464#4
# asm 2: movdqa <z3_stack=320(%rsp),>z3=%xmm3
movdqa 320(%rsp),%xmm3

# qhasm:   uint32323232 z0 += orig0
# asm 1: paddd <orig0=stack128#4,<z0=int6464#1
# asm 2: paddd <orig0=48(%rsp),<z0=%xmm0
paddd 48(%rsp),%xmm0

# qhasm:   uint32323232 z1 += orig1
# asm 1: paddd <orig1=stack128#8,<z1=int6464#2
# asm 2: paddd <orig1=112(%rsp),<z1=%xmm1
paddd 112(%rsp),%xmm1

# qhasm:   uint32323232 z2 += orig2
# asm 1: paddd <orig2=stack128#11,<z2=int6464#3
# asm 2: paddd <orig2=160(%rsp),<z2=%xmm2
paddd 160(%rsp),%xmm2

# qhasm:   uint32323232 z3 += orig3
# asm 1: paddd <orig3=stack128#14,<z3=int6464#4
# asm 2: paddd <orig3=208(%rsp),<z3=%xmm3
paddd 208(%rsp),%xmm3

# qhasm:   in0 = z0
# asm 1: movd   <z0=int6464#1,>in0=int64#3
# asm 2: movd   <z0=%xmm0,>in0=%rdx
movd   %xmm0,%rdx

# qhasm:   in1 = z1
# asm 1: movd   <z1=int6464#2,>in1=int64#4
# asm 2: movd   <z1=%xmm1,>in1=%rcx
movd   %xmm1,%rcx

# qhasm:   in2 = z2
# asm 1: movd   <z2=int6464#3,>in2=int64#6
# asm 2: movd   <z2=%xmm2,>in2=%r9
movd   %xmm2,%r9

# qhasm:   in3 = z3
# asm 1: movd   <z3=int6464#4,>in3=int64#7
# asm 2: movd   <z3=%xmm3,>in3=%rax
movd   %xmm3,%rax

# qhasm:   z0 <<<= 96
# asm 1: pshufd $0x39,<z0=int6464#1,<z0=int6464#1
# asm 2: pshufd $0x39,<z0=%xmm0,<z0=%xmm0
pshufd $0x39,%xmm0,%xmm0

# qhasm:   z1 <<<= 96
# asm 1: pshufd $0x39,<z1=int6464#2,<z1=int6464#2
# asm 2: pshufd $0x39,<z1=%xmm1,<z1=%xmm1
pshufd $0x39,%xmm1,%xmm1

# qhasm:   z2 <<<= 96
# asm 1: pshufd $0x39,<z2=int6464#3,<z2=int6464#3
# asm 2: pshufd $0x39,<z2=%xmm2,<z2=%xmm2
pshufd $0x39,%xmm2,%xmm2

# qhasm:   z3 <<<= 96
# asm 1: pshufd $0x39,<z3=int6464#4,<z3=int6464#4
# asm 2: pshufd $0x39,<z3=%xmm3,<z3=%xmm3
pshufd $0x39,%xmm3,%xmm3

# qhasm:   (uint32) in0 ^= *(uint32 *) (m + 0)
# asm 1: xorl 0(<m=int64#2),<in0=int64#3d
# asm 2: xorl 0(<m=%rsi),<in0=%edx
xorl 0(%rsi),%edx

# qhasm:   (uint32) in1 ^= *(uint32 *) (m + 4)
# asm 1: xorl 4(<m=int64#2),<in1=int64#4d
# asm 2: xorl 4(<m=%rsi),<in1=%ecx
xorl 4(%rsi),%ecx

# qhasm:   (uint32) in2 ^= *(uint32 *) (m + 8)
# asm 1: xorl 8(<m=int64#2),<in2=int64#6d
# asm 2: xorl 8(<m=%rsi),<in2=%r9d
xorl 8(%rsi),%r9d

# qhasm:   (uint32) in3 ^= *(uint32 *) (m + 12)
# asm 1: xorl 12(<m=int64#2),<in3=int64#7d
# asm 2: xorl 12(<m=%rsi),<in3=%eax
xorl 12(%rsi),%eax

# qhasm:   *(uint32 *) (out + 0) = in0
# asm 1: movl   <in0=int64#3d,0(<out=int64#1)
# asm 2: movl   <in0=%edx,0(<out=%rdi)
movl   %edx,0(%rdi)

# qhasm:   *(uint32 *) (out + 4) = in1
# asm 1: movl   <in1=int64#4d,4(<out=int64#1)
# asm 2: movl   <in1=%ecx,4(<out=%rdi)
movl   %ecx,4(%rdi)

# qhasm:   *(uint32 *) (out + 8) = in2
# asm 1: movl   <in2=int64#6d,8(<out=int64#1)
# asm 2: movl   <in2=%r9d,8(<out=%rdi)
movl   %r9d,8(%rdi)

# qhasm:   *(uint32 *) (out + 12) = in3
# asm 1: movl   <in3=int64#7d,12(<out=int64#1)
# asm 2: movl   <in3=%eax,12(<out=%rdi)
movl   %eax,12(%rdi)

# qhasm:   in0 = z0
# asm 1: movd   <z0=int6464#1,>in0=int64#3
# asm 2: movd   <z0=%xmm0,>in0=%rdx
movd   %xmm0,%rdx

# qhasm:   in1 = z1
# asm 1: movd   <z1=int6464#2,>in1=int64#4
# asm 2: movd   <z1=%xmm1,>in1=%rcx
movd   %xmm1,%rcx

# qhasm:   in2 = z2
# asm 1: movd   <z2=int6464#3,>in2=int64#6
# asm 2: movd   <z2=%xmm2,>in2=%r9
movd   %xmm2,%r9

# qhasm:   in3 = z3
# asm 1: movd   <z3=int6464#4,>in3=int64#7
# asm 2: movd   <z3=%xmm3,>in3=%rax
movd   %xmm3,%rax

# qhasm:   z0 <<<= 96
# asm 1: pshufd $0x39,<z0=int6464#1,<z0=int6464#1
# asm 2: pshufd $0x39,<z0=%xmm0,<z0=%xmm0
pshufd $0x39,%xmm0,%xmm0

# qhasm:   z1 <<<= 96
# asm 1: pshufd $0x39,<z1=int6464#2,<z1=int6464#2
# asm 2: pshufd $0x39,<z1=%xmm1,<z1=%xmm1
pshufd $0x39,%xmm1,%xmm1

# qhasm:   z2 <<<= 96
# asm 1: pshufd $0x39,<z2=int6464#3,<z2=int6464#3
# asm 2: pshufd $0x39,<z2=%xmm2,<z2=%xmm2
pshufd $0x39,%xmm2,%xmm2

# qhasm:   z3 <<<= 96
# asm 1: pshufd $0x39,<z3=int6464#4,<z3=int6464#4
# asm 2: pshufd $0x39,<z3=%xmm3,<z3=%xmm3
pshufd $0x39,%xmm3,%xmm3

# qhasm:   (uint32) in0 ^= *(uint32 *) (m + 64)
# asm 1: xorl 64(<m=int64#2),<in0=int64#3d
# asm 2: xorl 64(<m=%rsi),<in0=%edx
xorl 64(%rsi),%edx

# qhasm:   (uint32) in1 ^= *(uint32 *) (m + 68)
# asm 1: xorl 68(<m=int64#2),<in1=int64#4d
# asm 2: xorl 68(<m=%rsi),<in1=%ecx
xorl 68(%rsi),%ecx

# qhasm:   (uint32) in2 ^= *(uint32 *) (m + 72)
# asm 1: xorl 72(<m=int64#2),<in2=int64#6d
# asm 2: xorl 72(<m=%rsi),<in2=%r9d
xorl 72(%rsi),%r9d

# qhasm:   (uint32) in3 ^= *(uint32 *) (m + 76)
# asm 1: xorl 76(<m=int64#2),<in3=int64#7d
# asm 2: xorl 76(<m=%rsi),<in3=%eax
xorl 76(%rsi),%eax

# qhasm:   *(uint32 *) (out + 64) = in0
# asm 1: movl   <in0=int64#3d,64(<out=int64#1)
# asm 2: movl   <in0=%edx,64(<out=%rdi)
movl   %edx,64(%rdi)

# qhasm:   *(uint32 *) (out + 68) = in1
# asm 1: movl   <in1=int64#4d,68(<out=int64#1)
# asm 2: movl   <in1=%ecx,68(<out=%rdi)
movl   %ecx,68(%rdi)

# qhasm:   *(uint32 *) (out + 72) = in2
# asm 1: movl   <in2=int64#6d,72(<out=int64#1)
# asm 2: movl   <in2=%r9d,72(<out=%rdi)
movl   %r9d,72(%rdi)

# qhasm:   *(uint32 *) (out + 76) = in3
# asm 1: movl   <in3=int64#7d,76(<out=int64#1)
# asm 2: movl   <in3=%eax,76(<out=%rdi)
movl   %eax,76(%rdi)

# qhasm:   in0 = z0
# asm 1: movd   <z0=int6464#1,>in0=int64#3
# asm 2: movd   <z0=%xmm0,>in0=%rdx
movd   %xmm0,%rdx

# qhasm:   in1 = z1
# asm 1: movd   <z1=int6464#2,>in1=int64#4
# asm 2: movd   <z1=%xmm1,>in1=%rcx
movd   %xmm1,%rcx

# qhasm:   in2 = z2
# asm 1: movd   <z2=int6464#3,>in2=int64#6
# asm 2: movd   <z2=%xmm2,>in2=%r9
movd   %xmm2,%r9

# qhasm:   in3 = z3
# asm 1: movd   <z3=int6464#4,>in3=int64#7
# asm 2: movd   <z3=%xmm3,>in3=%rax
movd   %xmm3,%rax

# qhasm:   z0 <<<= 96
# asm 1: pshufd $0x39,<z0=int6464#1,<z0=int6464#1
# asm 2: pshufd $0x39,<z0=%xmm0,<z0=%xmm0
pshufd $0x39,%xmm0,%xmm0

# qhasm:   z1 <<<= 96
# asm 1: pshufd $0x39,<z1=int6464#2,<z1=int6464#2
# asm 2: pshufd $0x39,<z1=%xmm1,<z1=%xmm1
pshufd $0x39,%xmm1,%xmm1

# qhasm:   z2 <<<= 96
# asm 1: pshufd $0x39,<z2=int6464#3,<z2=int6464#3
# asm 2: pshufd $0x39,<z2=%xmm2,<z2=%xmm2
pshufd $0x39,%xmm2,%xmm2

# qhasm:   z3 <<<= 96
# asm 1: pshufd $0x39,<z3=int6464#4,<z3=int6464#4
# asm 2: pshufd $0x39,<z3=%xmm3,<z3=%xmm3
pshufd $0x39,%xmm3,%xmm3

# qhasm:   (uint32) in0 ^= *(uint32 *) (m + 128)
# asm 1: xorl 128(<m=int64#2),<in0=int64#3d
# asm 2: xorl 128(<m=%rsi),<in0=%edx
xorl 128(%rsi),%edx

# qhasm:   (uint32) in1 ^= *(uint32 *) (m + 132)
# asm 1: xorl 132(<m=int64#2),<in1=int64#4d
# asm 2: xorl 132(<m=%rsi),<in1=%ecx
xorl 132(%rsi),%ecx

# qhasm:   (uint32) in2 ^= *(uint32 *) (m + 136)
# asm 1: xorl 136(<m=int64#2),<in2=int64#6d
# asm 2: xorl 136(<m=%rsi),<in2=%r9d
xorl 136(%rsi),%r9d

# qhasm:   (uint32) in3 ^= *(uint32 *) (m + 140)
# asm 1: xorl 140(<m=int64#2),<in3=int64#7d
# asm 2: xorl 140(<m=%rsi),<in3=%eax
xorl 140(%rsi),%eax

# qhasm:   *(uint32 *) (out + 128) = in0
# asm 1: movl   <in0=int64#3d,128(<out=int64#1)
# asm 2: movl   <in0=%edx,128(<out=%rdi)
movl   %edx,128(%rdi)

# qhasm:   *(uint32 *) (out + 132) = in1
# asm 1: movl   <in1=int64#4d,132(<out=int64#1)
# asm 2: movl   <in1=%ecx,132(<out=%rdi)
movl   %ecx,132(%rdi)

# qhasm:   *(uint32 *) (out + 136) = in2
# asm 1: movl   <in2=int64#6d,136(<out=int64#1)
# asm 2: movl   <in2=%r9d,136(<out=%rdi)
movl   %r9d,136(%rdi)

# qhasm:   *(uint32 *) (out + 140) = in3
# asm 1: movl   <in3=int64#7d,140(<out=int64#1)
# asm 2: movl   <in3=%eax,140(<out=%rdi)
movl   %eax,140(%rdi)

# qhasm:   in0 = z0
# asm 1: movd   <z0=int6464#1,>in0=int64#3
# asm 2: movd   <z0=%xmm0,>in0=%rdx
movd   %xmm0,%rdx

# qhasm:   in1 = z1
# asm 1: movd   <z1=int6464#2,>in1=int64#4
# asm 2: movd   <z1=%xmm1,>in1=%rcx
movd   %xmm1,%rcx

# qhasm:   in2 = z2
# asm 1: movd   <z2=int6464#3,>in2=int64#6
# asm 2: movd   <z2=%xmm2,>in2=%r9
movd   %xmm2,%r9

# qhasm:   in3 = z3
# asm 1: movd   <z3=int6464#4,>in3=int64#7
# asm 2: movd   <z3=%xmm3,>in3=%rax
movd   %xmm3,%rax

# qhasm:   (uint32) in0 ^= *(uint32 *) (m + 192)
# asm 1: xorl 192(<m=int64#2),<in0=int64#3d
# asm 2: xorl 192(<m=%rsi),<in0=%edx
xorl 192(%rsi),%edx

# qhasm:   (uint32) in1 ^= *(uint32 *) (m + 196)
# asm 1: xorl 196(<m=int64#2),<in1=int64#4d
# asm 2: xorl 196(<m=%rsi),<in1=%ecx
xorl 196(%rsi),%ecx

# qhasm:   (uint32) in2 ^= *(uint32 *) (m + 200)
# asm 1: xorl 200(<m=int64#2),<in2=int64#6d
# asm 2: xorl 200(<m=%rsi),<in2=%r9d
xorl 200(%rsi),%r9d

# qhasm:   (uint32) in3 ^= *(uint32 *) (m + 204)
# asm 1: xorl 204(<m=int64#2),<in3=int64#7d
# asm 2: xorl 204(<m=%rsi),<in3=%eax
xorl 204(%rsi),%eax

# qhasm:   *(uint32 *) (out + 192) = in0
# asm 1: movl   <in0=int64#3d,192(<out=int64#1)
# asm 2: movl   <in0=%edx,192(<out=%rdi)
movl   %edx,192(%rdi)

# qhasm:   *(uint32 *) (out + 196) = in1
# asm 1: movl   <in1=int64#4d,196(<out=int64#1)
# asm 2: movl   <in1=%ecx,196(<out=%rdi)
movl   %ecx,196(%rdi)

# qhasm:   *(uint32 *) (out + 200) = in2
# asm 1: movl   <in2=int64#6d,200(<out=int64#1)
# asm 2: movl   <in2=%r9d,200(<out=%rdi)
movl   %r9d,200(%rdi)

# qhasm:   *(uint32 *) (out + 204) = in3
# asm 1: movl   <in3=int64#7d,204(<out=int64#1)
# asm 2: movl   <in3=%eax,204(<out=%rdi)
movl   %eax,204(%rdi)

# qhasm:   z4 = z4_stack
# asm 1: movdqa <z4_stack=stack128#30,>z4=int6464#1
# asm 2: movdqa <z4_stack=464(%rsp),>z4=%xmm0
movdqa 464(%rsp),%xmm0

# qhasm:   z5 = z5_stack
# asm 1: movdqa <z5_stack=stack128#17,>z5=int6464#2
# asm 2: movdqa <z5_stack=256(%rsp),>z5=%xmm1
movdqa 256(%rsp),%xmm1

# qhasm:   z6 = z6_stack
# asm 1: movdqa <z6_stack=stack128#22,>z6=int6464#3
# asm 2: movdqa <z6_stack=336(%rsp),>z6=%xmm2
movdqa 336(%rsp),%xmm2

# qhasm:   z7 = z7_stack
# asm 1: movdqa <z7_stack=stack128#25,>z7=int6464#4
# asm 2: movdqa <z7_stack=384(%rsp),>z7=%xmm3
movdqa 384(%rsp),%xmm3

# qhasm:   uint32323232 z4 += orig4
# asm 1: paddd <orig4=stack128#12,<z4=int6464#1
# asm 2: paddd <orig4=176(%rsp),<z4=%xmm0
paddd 176(%rsp),%xmm0

# qhasm:   uint32323232 z5 += orig5
# asm 1: paddd <orig5=stack128#1,<z5=int6464#2
# asm 2: paddd <orig5=0(%rsp),<z5=%xmm1
paddd 0(%rsp),%xmm1

# qhasm:   uint32323232 z6 += orig6
# asm 1: paddd <orig6=stack128#5,<z6=int6464#3
# asm 2: paddd <orig6=64(%rsp),<z6=%xmm2
paddd 64(%rsp),%xmm2

# qhasm:   uint32323232 z7 += orig7
# asm 1: paddd <orig7=stack128#9,<z7=int6464#4
# asm 2: paddd <orig7=128(%rsp),<z7=%xmm3
paddd 128(%rsp),%xmm3

# qhasm:   in4 = z4
# asm 1: movd   <z4=int6464#1,>in4=int64#3
# asm 2: movd   <z4=%xmm0,>in4=%rdx
movd   %xmm0,%rdx

# qhasm:   in5 = z5
# asm 1: movd   <z5=int6464#2,>in5=int64#4
# asm 2: movd   <z5=%xmm1,>in5=%rcx
movd   %xmm1,%rcx

# qhasm:   in6 = z6
# asm 1: movd   <z6=int6464#3,>in6=int64#6
# asm 2: movd   <z6=%xmm2,>in6=%r9
movd   %xmm2,%r9

# qhasm:   in7 = z7
# asm 1: movd   <z7=int6464#4,>in7=int64#7
# asm 2: movd   <z7=%xmm3,>in7=%rax
movd   %xmm3,%rax

# qhasm:   z4 <<<= 96
# asm 1: pshufd $0x39,<z4=int6464#1,<z4=int6464#1
# asm 2: pshufd $0x39,<z4=%xmm0,<z4=%xmm0
pshufd $0x39,%xmm0,%xmm0

# qhasm:   z5 <<<= 96
# asm 1: pshufd $0x39,<z5=int6464#2,<z5=int6464#2
# asm 2: pshufd $0x39,<z5=%xmm1,<z5=%xmm1
pshufd $0x39,%xmm1,%xmm1

# qhasm:   z6 <<<= 96
# asm 1: pshufd $0x39,<z6=int6464#3,<z6=int6464#3
# asm 2: pshufd $0x39,<z6=%xmm2,<z6=%xmm2
pshufd $0x39,%xmm2,%xmm2

# qhasm:   z7 <<<= 96
# asm 1: pshufd $0x39,<z7=int6464#4,<z7=int6464#4
# asm 2: pshufd $0x39,<z7=%xmm3,<z7=%xmm3
pshufd $0x39,%xmm3,%xmm3

# qhasm:   (uint32) in4 ^= *(uint32 *) (m + 16)
# asm 1: xorl 16(<m=int64#2),<in4=int64#3d
# asm 2: xorl 16(<m=%rsi),<in4=%edx
xorl 16(%rsi),%edx

# qhasm:   (uint32) in5 ^= *(uint32 *) (m + 20)
# asm 1: xorl 20(<m=int64#2),<in5=int64#4d
# asm 2: xorl 20(<m=%rsi),<in5=%ecx
xorl 20(%rsi),%ecx

# qhasm:   (uint32) in6 ^= *(uint32 *) (m + 24)
# asm 1: xorl 24(<m=int64#2),<in6=int64#6d
# asm 2: xorl 24(<m=%rsi),<in6=%r9d
xorl 24(%rsi),%r9d

# qhasm:   (uint32) in7 ^= *(uint32 *) (m + 28)
# asm 1: xorl 28(<m=int64#2),<in7=int64#7d
# asm 2: xorl 28(<m=%rsi),<in7=%eax
xorl 28(%rsi),%eax

# qhasm:   *(uint32 *) (out + 16) = in4
# asm 1: movl   <in4=int64#3d,16(<out=int64#1)
# asm 2: movl   <in4=%edx,16(<out=%rdi)
movl   %edx,16(%rdi)

# qhasm:   *(uint32 *) (out + 20) = in5
# asm 1: movl   <in5=int64#4d,20(<out=int64#1)
# asm 2: movl   <in5=%ecx,20(<out=%rdi)
movl   %ecx,20(%rdi)

# qhasm:   *(uint32 *) (out + 24) = in6
# asm 1: movl   <in6=int64#6d,24(<out=int64#1)
# asm 2: movl   <in6=%r9d,24(<out=%rdi)
movl   %r9d,24(%rdi)

# qhasm:   *(uint32 *) (out + 28) = in7
# asm 1: movl   <in7=int64#7d,28(<out=int64#1)
# asm 2: movl   <in7=%eax,28(<out=%rdi)
movl   %eax,28(%rdi)

# qhasm:   in4 = z4
# asm 1: movd   <z4=int6464#1,>in4=int64#3
# asm 2: movd   <z4=%xmm0,>in4=%rdx
movd   %xmm0,%rdx

# qhasm:   in5 = z5
# asm 1: movd   <z5=int6464#2,>in5=int64#4
# asm 2: movd   <z5=%xmm1,>in5=%rcx
movd   %xmm1,%rcx

# qhasm:   in6 = z6
# asm 1: movd   <z6=int6464#3,>in6=int64#6
# asm 2: movd   <z6=%xmm2,>in6=%r9
movd   %xmm2,%r9

# qhasm:   in7 = z7
# asm 1: movd   <z7=int6464#4,>in7=int64#7
# asm 2: movd   <z7=%xmm3,>in7=%rax
movd   %xmm3,%rax

# qhasm:   z4 <<<= 96
# asm 1: pshufd $0x39,<z4=int6464#1,<z4=int6464#1
# asm 2: pshufd $0x39,<z4=%xmm0,<z4=%xmm0
pshufd $0x39,%xmm0,%xmm0

# qhasm:   z5 <<<= 96
# asm 1: pshufd $0x39,<z5=int6464#2,<z5=int6464#2
# asm 2: pshufd $0x39,<z5=%xmm1,<z5=%xmm1
pshufd $0x39,%xmm1,%xmm1

# qhasm:   z6 <<<= 96
# asm 1: pshufd $0x39,<z6=int6464#3,<z6=int6464#3
# asm 2: pshufd $0x39,<z6=%xmm2,<z6=%xmm2
pshufd $0x39,%xmm2,%xmm2

# qhasm:   z7 <<<= 96
# asm 1: pshufd $0x39,<z7=int6464#4,<z7=int6464#4
# asm 2: pshufd $0x39,<z7=%xmm3,<z7=%xmm3
pshufd $0x39,%xmm3,%xmm3

# qhasm:   (uint32) in4 ^= *(uint32 *) (m + 80)
# asm 1: xorl 80(<m=int64#2),<in4=int64#3d
# asm 2: xorl 80(<m=%rsi),<in4=%edx
xorl 80(%rsi),%edx

# qhasm:   (uint32) in5 ^= *(uint32 *) (m + 84)
# asm 1: xorl 84(<m=int64#2),<in5=int64#4d
# asm 2: xorl 84(<m=%rsi),<in5=%ecx
xorl 84(%rsi),%ecx

# qhasm:   (uint32) in6 ^= *(uint32 *) (m + 88)
# asm 1: xorl 88(<m=int64#2),<in6=int64#6d
# asm 2: xorl 88(<m=%rsi),<in6=%r9d
xorl 88(%rsi),%r9d

# qhasm:   (uint32) in7 ^= *(uint32 *) (m + 92)
# asm 1: xorl 92(<m=int64#2),<in7=int64#7d
# asm 2: xorl 92(<m=%rsi),<in7=%eax
xorl 92(%rsi),%eax

# qhasm:   *(uint32 *) (out + 80) = in4
# asm 1: movl   <in4=int64#3d,80(<out=int64#1)
# asm 2: movl   <in4=%edx,80(<out=%rdi)
movl   %edx,80(%rdi)

# qhasm:   *(uint32 *) (out + 84) = in5
# asm 1: movl   <in5=int64#4d,84(<out=int64#1)
# asm 2: movl   <in5=%ecx,84(<out=%rdi)
movl   %ecx,84(%rdi)

# qhasm:   *(uint32 *) (out + 88) = in6
# asm 1: movl   <in6=int64#6d,88(<out=int64#1)
# asm 2: movl   <in6=%r9d,88(<out=%rdi)
movl   %r9d,88(%rdi)

# qhasm:   *(uint32 *) (out + 92) = in7
# asm 1: movl   <in7=int64#7d,92(<out=int64#1)
# asm 2: movl   <in7=%eax,92(<out=%rdi)
movl   %eax,92(%rdi)

# qhasm:   in4 = z4
# asm 1: movd   <z4=int6464#1,>in4=int64#3
# asm 2: movd   <z4=%xmm0,>in4=%rdx
movd   %xmm0,%rdx

# qhasm:   in5 = z5
# asm 1: movd   <z5=int6464#2,>in5=int64#4
# asm 2: movd   <z5=%xmm1,>in5=%rcx
movd   %xmm1,%rcx

# qhasm:   in6 = z6
# asm 1: movd   <z6=int6464#3,>in6=int64#6
# asm 2: movd   <z6=%xmm2,>in6=%r9
movd   %xmm2,%r9

# qhasm:   in7 = z7
# asm 1: movd   <z7=int6464#4,>in7=int64#7
# asm 2: movd   <z7=%xmm3,>in7=%rax
movd   %xmm3,%rax

# qhasm:   z4 <<<= 96
# asm 1: pshufd $0x39,<z4=int6464#1,<z4=int6464#1
# asm 2: pshufd $0x39,<z4=%xmm0,<z4=%xmm0
pshufd $0x39,%xmm0,%xmm0

# qhasm:   z5 <<<= 96
# asm 1: pshufd $0x39,<z5=int6464#2,<z5=int6464#2
# asm 2: pshufd $0x39,<z5=%xmm1,<z5=%xmm1
pshufd $0x39,%xmm1,%xmm1

# qhasm:   z6 <<<= 96
# asm 1: pshufd $0x39,<z6=int6464#3,<z6=int6464#3
# asm 2: pshufd $0x39,<z6=%xmm2,<z6=%xmm2
pshufd $0x39,%xmm2,%xmm2

# qhasm:   z7 <<<= 96
# asm 1: pshufd $0x39,<z7=int6464#4,<z7=int6464#4
# asm 2: pshufd $0x39,<z7=%xmm3,<z7=%xmm3
pshufd $0x39,%xmm3,%xmm3

# qhasm:   (uint32) in4 ^= *(uint32 *) (m + 144)
# asm 1: xorl 144(<m=int64#2),<in4=int64#3d
# asm 2: xorl 144(<m=%rsi),<in4=%edx
xorl 144(%rsi),%edx

# qhasm:   (uint32) in5 ^= *(uint32 *) (m + 148)
# asm 1: xorl 148(<m=int64#2),<in5=int64#4d
# asm 2: xorl 148(<m=%rsi),<in5=%ecx
xorl 148(%rsi),%ecx

# qhasm:   (uint32) in6 ^= *(uint32 *) (m + 152)
# asm 1: xorl 152(<m=int64#2),<in6=int64#6d
# asm 2: xorl 152(<m=%rsi),<in6=%r9d
xorl 152(%rsi),%r9d

# qhasm:   (uint32) in7 ^= *(uint32 *) (m + 156)
# asm 1: xorl 156(<m=int64#2),<in7=int64#7d
# asm 2: xorl 156(<m=%rsi),<in7=%eax
xorl 156(%rsi),%eax

# qhasm:   *(uint32 *) (out + 144) = in4
# asm 1: movl   <in4=int64#3d,144(<out=int64#1)
# asm 2: movl   <in4=%edx,144(<out=%rdi)
movl   %edx,144(%rdi)

# qhasm:   *(uint32 *) (out + 148) = in5
# asm 1: movl   <in5=int64#4d,148(<out=int64#1)
# asm 2: movl   <in5=%ecx,148(<out=%rdi)
movl   %ecx,148(%rdi)

# qhasm:   *(uint32 *) (out + 152) = in6
# asm 1: movl   <in6=int64#6d,152(<out=int64#1)
# asm 2: movl   <in6=%r9d,152(<out=%rdi)
movl   %r9d,152(%rdi)

# qhasm:   *(uint32 *) (out + 156) = in7
# asm 1: movl   <in7=int64#7d,156(<out=int64#1)
# asm 2: movl   <in7=%eax,156(<out=%rdi)
movl   %eax,156(%rdi)

# qhasm:   in4 = z4
# asm 1: movd   <z4=int6464#1,>in4=int64#3
# asm 2: movd   <z4=%xmm0,>in4=%rdx
movd   %xmm0,%rdx

# qhasm:   in5 = z5
# asm 1: movd   <z5=int6464#2,>in5=int64#4
# asm 2: movd   <z5=%xmm1,>in5=%rcx
movd   %xmm1,%rcx

# qhasm:   in6 = z6
# asm 1: movd   <z6=int6464#3,>in6=int64#6
# asm 2: movd   <z6=%xmm2,>in6=%r9
movd   %xmm2,%r9

# qhasm:   in7 = z7
# asm 1: movd   <z7=int6464#4,>in7=int64#7
# asm 2: movd   <z7=%xmm3,>in7=%rax
movd   %xmm3,%rax

# qhasm:   (uint32) in4 ^= *(uint32 *) (m + 208)
# asm 1: xorl 208(<m=int64#2),<in4=int64#3d
# asm 2: xorl 208(<m=%rsi),<in4=%edx
xorl 208(%rsi),%edx

# qhasm:   (uint32) in5 ^= *(uint32 *) (m + 212)
# asm 1: xorl 212(<m=int64#2),<in5=int64#4d
# asm 2: xorl 212(<m=%rsi),<in5=%ecx
xorl 212(%rsi),%ecx

# qhasm:   (uint32) in6 ^= *(uint32 *) (m + 216)
# asm 1: xorl 216(<m=int64#2),<in6=int64#6d
# asm 2: xorl 216(<m=%rsi),<in6=%r9d
xorl 216(%rsi),%r9d

# qhasm:   (uint32) in7 ^= *(uint32 *) (m + 220)
# asm 1: xorl 220(<m=int64#2),<in7=int64#7d
# asm 2: xorl 220(<m=%rsi),<in7=%eax
xorl 220(%rsi),%eax

# qhasm:   *(uint32 *) (out + 208) = in4
# asm 1: movl   <in4=int64#3d,208(<out=int64#1)
# asm 2: movl   <in4=%edx,208(<out=%rdi)
movl   %edx,208(%rdi)

# qhasm:   *(uint32 *) (out + 212) = in5
# asm 1: movl   <in5=int64#4d,212(<out=int64#1)
# asm 2: movl   <in5=%ecx,212(<out=%rdi)
movl   %ecx,212(%rdi)

# qhasm:   *(uint32 *) (out + 216) = in6
# asm 1: movl   <in6=int64#6d,216(<out=int64#1)
# asm 2: movl   <in6=%r9d,216(<out=%rdi)
movl   %r9d,216(%rdi)

# qhasm:   *(uint32 *) (out + 220) = in7
# asm 1: movl   <in7=int64#7d,220(<out=int64#1)
# asm 2: movl   <in7=%eax,220(<out=%rdi)
movl   %eax,220(%rdi)

# qhasm:   z8 = z8_stack
# asm 1: movdqa <z8_stack=stack128#33,>z8=int6464#1
# asm 2: movdqa <z8_stack=512(%rsp),>z8=%xmm0
movdqa 512(%rsp),%xmm0

# qhasm:   z9 = z9_stack
# asm 1: movdqa <z9_stack=stack128#28,>z9=int6464#2
# asm 2: movdqa <z9_stack=432(%rsp),>z9=%xmm1
movdqa 432(%rsp),%xmm1

# qhasm:   z10 = z10_stack
# asm 1: movdqa <z10_stack=stack128#18,>z10=int6464#3
# asm 2: movdqa <z10_stack=272(%rsp),>z10=%xmm2
movdqa 272(%rsp),%xmm2

# qhasm:   z11 = z11_stack
# asm 1: movdqa <z11_stack=stack128#23,>z11=int6464#4
# asm 2: movdqa <z11_stack=352(%rsp),>z11=%xmm3
movdqa 352(%rsp),%xmm3

# qhasm:   uint32323232 z8 += orig8
# asm 1: paddd <orig8=stack128#15,<z8=int6464#1
# asm 2: paddd <orig8=224(%rsp),<z8=%xmm0
paddd 224(%rsp),%xmm0

# qhasm:   uint32323232 z9 += orig9
# asm 1: paddd <orig9=stack128#16,<z9=int6464#2
# asm 2: paddd <orig9=240(%rsp),<z9=%xmm1
paddd 240(%rsp),%xmm1

# qhasm:   uint32323232 z10 += orig10
# asm 1: paddd <orig10=stack128#2,<z10=int6464#3
# asm 2: paddd <orig10=16(%rsp),<z10=%xmm2
paddd 16(%rsp),%xmm2

# qhasm:   uint32323232 z11 += orig11
# asm 1: paddd <orig11=stack128#6,<z11=int6464#4
# asm 2: paddd <orig11=80(%rsp),<z11=%xmm3
paddd 80(%rsp),%xmm3

# qhasm:   in8 = z8
# asm 1: movd   <z8=int6464#1,>in8=int64#3
# asm 2: movd   <z8=%xmm0,>in8=%rdx
movd   %xmm0,%rdx

# qhasm:   in9 = z9
# asm 1: movd   <z9=int6464#2,>in9=int64#4
# asm 2: movd   <z9=%xmm1,>in9=%rcx
movd   %xmm1,%rcx

# qhasm:   in10 = z10
# asm 1: movd   <z10=int6464#3,>in10=int64#6
# asm 2: movd   <z10=%xmm2,>in10=%r9
movd   %xmm2,%r9

# qhasm:   in11 = z11
# asm 1: movd   <z11=int6464#4,>in11=int64#7
# asm 2: movd   <z11=%xmm3,>in11=%rax
movd   %xmm3,%rax

# qhasm:   z8 <<<= 96
# asm 1: pshufd $0x39,<z8=int6464#1,<z8=int6464#1
# asm 2: pshufd $0x39,<z8=%xmm0,<z8=%xmm0
pshufd $0x39,%xmm0,%xmm0

# qhasm:   z9 <<<= 96
# asm 1: pshufd $0x39,<z9=int6464#2,<z9=int6464#2
# asm 2: pshufd $0x39,<z9=%xmm1,<z9=%xmm1
pshufd $0x39,%xmm1,%xmm1

# qhasm:   z10 <<<= 96
# asm 1: pshufd $0x39,<z10=int6464#3,<z10=int6464#3
# asm 2: pshufd $0x39,<z10=%xmm2,<z10=%xmm2
pshufd $0x39,%xmm2,%xmm2

# qhasm:   z11 <<<= 96
# asm 1: pshufd $0x39,<z11=int6464#4,<z11=int6464#4
# asm 2: pshufd $0x39,<z11=%xmm3,<z11=%xmm3
pshufd $0x39,%xmm3,%xmm3

# qhasm:   (uint32) in8 ^= *(uint32 *) (m + 32)
# asm 1: xorl 32(<m=int64#2),<in8=int64#3d
# asm 2: xorl 32(<m=%rsi),<in8=%edx
xorl 32(%rsi),%edx

# qhasm:   (uint32) in9 ^= *(uint32 *) (m + 36)
# asm 1: xorl 36(<m=int64#2),<in9=int64#4d
# asm 2: xorl 36(<m=%rsi),<in9=%ecx
xorl 36(%rsi),%ecx

# qhasm:   (uint32) in10 ^= *(uint32 *) (m + 40)
# asm 1: xorl 40(<m=int64#2),<in10=int64#6d
# asm 2: xorl 40(<m=%rsi),<in10=%r9d
xorl 40(%rsi),%r9d

# qhasm:   (uint32) in11 ^= *(uint32 *) (m + 44)
# asm 1: xorl 44(<m=int64#2),<in11=int64#7d
# asm 2: xorl 44(<m=%rsi),<in11=%eax
xorl 44(%rsi),%eax

# qhasm:   *(uint32 *) (out + 32) = in8
# asm 1: movl   <in8=int64#3d,32(<out=int64#1)
# asm 2: movl   <in8=%edx,32(<out=%rdi)
movl   %edx,32(%rdi)

# qhasm:   *(uint32 *) (out + 36) = in9
# asm 1: movl   <in9=int64#4d,36(<out=int64#1)
# asm 2: movl   <in9=%ecx,36(<out=%rdi)
movl   %ecx,36(%rdi)

# qhasm:   *(uint32 *) (out + 40) = in10
# asm 1: movl   <in10=int64#6d,40(<out=int64#1)
# asm 2: movl   <in10=%r9d,40(<out=%rdi)
movl   %r9d,40(%rdi)

# qhasm:   *(uint32 *) (out + 44) = in11
# asm 1: movl   <in11=int64#7d,44(<out=int64#1)
# asm 2: movl   <in11=%eax,44(<out=%rdi)
movl   %eax,44(%rdi)

# qhasm:   in8 = z8
# asm 1: movd   <z8=int6464#1,>in8=int64#3
# asm 2: movd   <z8=%xmm0,>in8=%rdx
movd   %xmm0,%rdx

# qhasm:   in9 = z9
# asm 1: movd   <z9=int6464#2,>in9=int64#4
# asm 2: movd   <z9=%xmm1,>in9=%rcx
movd   %xmm1,%rcx

# qhasm:   in10 = z10
# asm 1: movd   <z10=int6464#3,>in10=int64#6
# asm 2: movd   <z10=%xmm2,>in10=%r9
movd   %xmm2,%r9

# qhasm:   in11 = z11
# asm 1: movd   <z11=int6464#4,>in11=int64#7
# asm 2: movd   <z11=%xmm3,>in11=%rax
movd   %xmm3,%rax

# qhasm:   z8 <<<= 96
# asm 1: pshufd $0x39,<z8=int6464#1,<z8=int6464#1
# asm 2: pshufd $0x39,<z8=%xmm0,<z8=%xmm0
pshufd $0x39,%xmm0,%xmm0

# qhasm:   z9 <<<= 96
# asm 1: pshufd $0x39,<z9=int6464#2,<z9=int6464#2
# asm 2: pshufd $0x39,<z9=%xmm1,<z9=%xmm1
pshufd $0x39,%xmm1,%xmm1

# qhasm:   z10 <<<= 96
# asm 1: pshufd $0x39,<z10=int6464#3,<z10=int6464#3
# asm 2: pshufd $0x39,<z10=%xmm2,<z10=%xmm2
pshufd $0x39,%xmm2,%xmm2

# qhasm:   z11 <<<= 96
# asm 1: pshufd $0x39,<z11=int6464#4,<z11=int6464#4
# asm 2: pshufd $0x39,<z11=%xmm3,<z11=%xmm3
pshufd $0x39,%xmm3,%xmm3

# qhasm:   (uint32) in8 ^= *(uint32 *) (m + 96)
# asm 1: xorl 96(<m=int64#2),<in8=int64#3d
# asm 2: xorl 96(<m=%rsi),<in8=%edx
xorl 96(%rsi),%edx

# qhasm:   (uint32) in9 ^= *(uint32 *) (m + 100)
# asm 1: xorl 100(<m=int64#2),<in9=int64#4d
# asm 2: xorl 100(<m=%rsi),<in9=%ecx
xorl 100(%rsi),%ecx

# qhasm:   (uint32) in10 ^= *(uint32 *) (m + 104)
# asm 1: xorl 104(<m=int64#2),<in10=int64#6d
# asm 2: xorl 104(<m=%rsi),<in10=%r9d
xorl 104(%rsi),%r9d

# qhasm:   (uint32) in11 ^= *(uint32 *) (m + 108)
# asm 1: xorl 108(<m=int64#2),<in11=int64#7d
# asm 2: xorl 108(<m=%rsi),<in11=%eax
xorl 108(%rsi),%eax

# qhasm:   *(uint32 *) (out + 96) = in8
# asm 1: movl   <in8=int64#3d,96(<out=int64#1)
# asm 2: movl   <in8=%edx,96(<out=%rdi)
movl   %edx,96(%rdi)

# qhasm:   *(uint32 *) (out + 100) = in9
# asm 1: movl   <in9=int64#4d,100(<out=int64#1)
# asm 2: movl   <in9=%ecx,100(<out=%rdi)
movl   %ecx,100(%rdi)

# qhasm:   *(uint32 *) (out + 104) = in10
# asm 1: movl   <in10=int64#6d,104(<out=int64#1)
# asm 2: movl   <in10=%r9d,104(<out=%rdi)
movl   %r9d,104(%rdi)

# qhasm:   *(uint32 *) (out + 108) = in11
# asm 1: movl   <in11=int64#7d,108(<out=int64#1)
# asm 2: movl   <in11=%eax,108(<out=%rdi)
movl   %eax,108(%rdi)

# qhasm:   in8 = z8
# asm 1: movd   <z8=int6464#1,>in8=int64#3
# asm 2: movd   <z8=%xmm0,>in8=%rdx
movd   %xmm0,%rdx

# qhasm:   in9 = z9
# asm 1: movd   <z9=int6464#2,>in9=int64#4
# asm 2: movd   <z9=%xmm1,>in9=%rcx
movd   %xmm1,%rcx

# qhasm:   in10 = z10
# asm 1: movd   <z10=int6464#3,>in10=int64#6
# asm 2: movd   <z10=%xmm2,>in10=%r9
movd   %xmm2,%r9

# qhasm:   in11 = z11
# asm 1: movd   <z11=int6464#4,>in11=int64#7
# asm 2: movd   <z11=%xmm3,>in11=%rax
movd   %xmm3,%rax

# qhasm:   z8 <<<= 96
# asm 1: pshufd $0x39,<z8=int6464#1,<z8=int6464#1
# asm 2: pshufd $0x39,<z8=%xmm0,<z8=%xmm0
pshufd $0x39,%xmm0,%xmm0

# qhasm:   z9 <<<= 96
# asm 1: pshufd $0x39,<z9=int6464#2,<z9=int6464#2
# asm 2: pshufd $0x39,<z9=%xmm1,<z9=%xmm1
pshufd $0x39,%xmm1,%xmm1

# qhasm:   z10 <<<= 96
# asm 1: pshufd $0x39,<z10=int6464#3,<z10=int6464#3
# asm 2: pshufd $0x39,<z10=%xmm2,<z10=%xmm2
pshufd $0x39,%xmm2,%xmm2

# qhasm:   z11 <<<= 96
# asm 1: pshufd $0x39,<z11=int6464#4,<z11=int6464#4
# asm 2: pshufd $0x39,<z11=%xmm3,<z11=%xmm3
pshufd $0x39,%xmm3,%xmm3

# qhasm:   (uint32) in8 ^= *(uint32 *) (m + 160)
# asm 1: xorl 160(<m=int64#2),<in8=int64#3d
# asm 2: xorl 160(<m=%rsi),<in8=%edx
xorl 160(%rsi),%edx

# qhasm:   (uint32) in9 ^= *(uint32 *) (m + 164)
# asm 1: xorl 164(<m=int64#2),<in9=int64#4d
# asm 2: xorl 164(<m=%rsi),<in9=%ecx
xorl 164(%rsi),%ecx

# qhasm:   (uint32) in10 ^= *(uint32 *) (m + 168)
# asm 1: xorl 168(<m=int64#2),<in10=int64#6d
# asm 2: xorl 168(<m=%rsi),<in10=%r9d
xorl 168(%rsi),%r9d

# qhasm:   (uint32) in11 ^= *(uint32 *) (m + 172)
# asm 1: xorl 172(<m=int64#2),<in11=int64#7d
# asm 2: xorl 172(<m=%rsi),<in11=%eax
xorl 172(%rsi),%eax

# qhasm:   *(uint32 *) (out + 160) = in8
# asm 1: movl   <in8=int64#3d,160(<out=int64#1)
# asm 2: movl   <in8=%edx,160(<out=%rdi)
movl   %edx,160(%rdi)

# qhasm:   *(uint32 *) (out + 164) = in9
# asm 1: movl   <in9=int64#4d,164(<out=int64#1)
# asm 2: movl   <in9=%ecx,164(<out=%rdi)
movl   %ecx,164(%rdi)

# qhasm:   *(uint32 *) (out + 168) = in10
# asm 1: movl   <in10=int64#6d,168(<out=int64#1)
# asm 2: movl   <in10=%r9d,168(<out=%rdi)
movl   %r9d,168(%rdi)

# qhasm:   *(uint32 *) (out + 172) = in11
# asm 1: movl   <in11=int64#7d,172(<out=int64#1)
# asm 2: movl   <in11=%eax,172(<out=%rdi)
movl   %eax,172(%rdi)

# qhasm:   in8 = z8
# asm 1: movd   <z8=int6464#1,>in8=int64#3
# asm 2: movd   <z8=%xmm0,>in8=%rdx
movd   %xmm0,%rdx

# qhasm:   in9 = z9
# asm 1: movd   <z9=int6464#2,>in9=int64#4
# asm 2: movd   <z9=%xmm1,>in9=%rcx
movd   %xmm1,%rcx

# qhasm:   in10 = z10
# asm 1: movd   <z10=int6464#3,>in10=int64#6
# asm 2: movd   <z10=%xmm2,>in10=%r9
movd   %xmm2,%r9

# qhasm:   in11 = z11
# asm 1: movd   <z11=int6464#4,>in11=int64#7
# asm 2: movd   <z11=%xmm3,>in11=%rax
movd   %xmm3,%rax

# qhasm:   (uint32) in8 ^= *(uint32 *) (m + 224)
# asm 1: xorl 224(<m=int64#2),<in8=int64#3d
# asm 2: xorl 224(<m=%rsi),<in8=%edx
xorl 224(%rsi),%edx

# qhasm:   (uint32) in9 ^= *(uint32 *) (m + 228)
# asm 1: xorl 228(<m=int64#2),<in9=int64#4d
# asm 2: xorl 228(<m=%rsi),<in9=%ecx
xorl 228(%rsi),%ecx

# qhasm:   (uint32) in10 ^= *(uint32 *) (m + 232)
# asm 1: xorl 232(<m=int64#2),<in10=int64#6d
# asm 2: xorl 232(<m=%rsi),<in10=%r9d
xorl 232(%rsi),%r9d

# qhasm:   (uint32) in11 ^= *(uint32 *) (m + 236)
# asm 1: xorl 236(<m=int64#2),<in11=int64#7d
# asm 2: xorl 236(<m=%rsi),<in11=%eax
xorl 236(%rsi),%eax

# qhasm:   *(uint32 *) (out + 224) = in8
# asm 1: movl   <in8=int64#3d,224(<out=int64#1)
# asm 2: movl   <in8=%edx,224(<out=%rdi)
movl   %edx,224(%rdi)

# qhasm:   *(uint32 *) (out + 228) = in9
# asm 1: movl   <in9=int64#4d,228(<out=int64#1)
# asm 2: movl   <in9=%ecx,228(<out=%rdi)
movl   %ecx,228(%rdi)

# qhasm:   *(uint32 *) (out + 232) = in10
# asm 1: movl   <in10=int64#6d,232(<out=int64#1)
# asm 2: movl   <in10=%r9d,232(<out=%rdi)
movl   %r9d,232(%rdi)

# qhasm:   *(uint32 *) (out + 236) = in11
# asm 1: movl   <in11=int64#7d,236(<out=int64#1)
# asm 2: movl   <in11=%eax,236(<out=%rdi)
movl   %eax,236(%rdi)

# qhasm:   z12 = z12_stack
# asm 1: movdqa <z12_stack=stack128#31,>z12=int6464#1
# asm 2: movdqa <z12_stack=480(%rsp),>z12=%xmm0
movdqa 480(%rsp),%xmm0

# qhasm:   z13 = z13_stack
# asm 1: movdqa <z13_stack=stack128#26,>z13=int6464#2
# asm 2: movdqa <z13_stack=400(%rsp),>z13=%xmm1
movdqa 400(%rsp),%xmm1

# qhasm:   z14 = z14_stack
# asm 1: movdqa <z14_stack=stack128#20,>z14=int6464#3
# asm 2: movdqa <z14_stack=304(%rsp),>z14=%xmm2
movdqa 304(%rsp),%xmm2

# qhasm:   z15 = z15_stack
# asm 1: movdqa <z15_stack=stack128#19,>z15=int6464#4
# asm 2: movdqa <z15_stack=288(%rsp),>z15=%xmm3
movdqa 288(%rsp),%xmm3

# qhasm:   uint32323232 z12 += orig12
# asm 1: paddd <orig12=stack128#7,<z12=int6464#1
# asm 2: paddd <orig12=96(%rsp),<z12=%xmm0
paddd 96(%rsp),%xmm0

# qhasm:   uint32323232 z13 += orig13
# asm 1: paddd <orig13=stack128#10,<z13=int6464#2
# asm 2: paddd <orig13=144(%rsp),<z13=%xmm1
paddd 144(%rsp),%xmm1

# qhasm:   uint32323232 z14 += orig14
# asm 1: paddd <orig14=stack128#13,<z14=int6464#3
# asm 2: paddd <orig14=192(%rsp),<z14=%xmm2
paddd 192(%rsp),%xmm2

# qhasm:   uint32323232 z15 += orig15
# asm 1: paddd <orig15=stack128#3,<z15=int6464#4
# asm 2: paddd <orig15=32(%rsp),<z15=%xmm3
paddd 32(%rsp),%xmm3

# qhasm:   in12 = z12
# asm 1: movd   <z12=int6464#1,>in12=int64#3
# asm 2: movd   <z12=%xmm0,>in12=%rdx
movd   %xmm0,%rdx

# qhasm:   in13 = z13
# asm 1: movd   <z13=int6464#2,>in13=int64#4
# asm 2: movd   <z13=%xmm1,>in13=%rcx
movd   %xmm1,%rcx

# qhasm:   in14 = z14
# asm 1: movd   <z14=int6464#3,>in14=int64#6
# asm 2: movd   <z14=%xmm2,>in14=%r9
movd   %xmm2,%r9

# qhasm:   in15 = z15
# asm 1: movd   <z15=int6464#4,>in15=int64#7
# asm 2: movd   <z15=%xmm3,>in15=%rax
movd   %xmm3,%rax

# qhasm:   z12 <<<= 96
# asm 1: pshufd $0x39,<z12=int6464#1,<z12=int6464#1
# asm 2: pshufd $0x39,<z12=%xmm0,<z12=%xmm0
pshufd $0x39,%xmm0,%xmm0

# qhasm:   z13 <<<= 96
# asm 1: pshufd $0x39,<z13=int6464#2,<z13=int6464#2
# asm 2: pshufd $0x39,<z13=%xmm1,<z13=%xmm1
pshufd $0x39,%xmm1,%xmm1

# qhasm:   z14 <<<= 96
# asm 1: pshufd $0x39,<z14=int6464#3,<z14=int6464#3
# asm 2: pshufd $0x39,<z14=%xmm2,<z14=%xmm2
pshufd $0x39,%xmm2,%xmm2

# qhasm:   z15 <<<= 96
# asm 1: pshufd $0x39,<z15=int6464#4,<z15=int6464#4
# asm 2: pshufd $0x39,<z15=%xmm3,<z15=%xmm3
pshufd $0x39,%xmm3,%xmm3

# qhasm:   (uint32) in12 ^= *(uint32 *) (m + 48)
# asm 1: xorl 48(<m=int64#2),<in12=int64#3d
# asm 2: xorl 48(<m=%rsi),<in12=%edx
xorl 48(%rsi),%edx

# qhasm:   (uint32) in13 ^= *(uint32 *) (m + 52)
# asm 1: xorl 52(<m=int64#2),<in13=int64#4d
# asm 2: xorl 52(<m=%rsi),<in13=%ecx
xorl 52(%rsi),%ecx

# qhasm:   (uint32) in14 ^= *(uint32 *) (m + 56)
# asm 1: xorl 56(<m=int64#2),<in14=int64#6d
# asm 2: xorl 56(<m=%rsi),<in14=%r9d
xorl 56(%rsi),%r9d

# qhasm:   (uint32) in15 ^= *(uint32 *) (m + 60)
# asm 1: xorl 60(<m=int64#2),<in15=int64#7d
# asm 2: xorl 60(<m=%rsi),<in15=%eax
xorl 60(%rsi),%eax

# qhasm:   *(uint32 *) (out + 48) = in12
# asm 1: movl   <in12=int64#3d,48(<out=int64#1)
# asm 2: movl   <in12=%edx,48(<out=%rdi)
movl   %edx,48(%rdi)

# qhasm:   *(uint32 *) (out + 52) = in13
# asm 1: movl   <in13=int64#4d,52(<out=int64#1)
# asm 2: movl   <in13=%ecx,52(<out=%rdi)
movl   %ecx,52(%rdi)

# qhasm:   *(uint32 *) (out + 56) = in14
# asm 1: movl   <in14=int64#6d,56(<out=int64#1)
# asm 2: movl   <in14=%r9d,56(<out=%rdi)
movl   %r9d,56(%rdi)

# qhasm:   *(uint32 *) (out + 60) = in15
# asm 1: movl   <in15=int64#7d,60(<out=int64#1)
# asm 2: movl   <in15=%eax,60(<out=%rdi)
movl   %eax,60(%rdi)

# qhasm:   in12 = z12
# asm 1: movd   <z12=int6464#1,>in12=int64#3
# asm 2: movd   <z12=%xmm0,>in12=%rdx
movd   %xmm0,%rdx

# qhasm:   in13 = z13
# asm 1: movd   <z13=int6464#2,>in13=int64#4
# asm 2: movd   <z13=%xmm1,>in13=%rcx
movd   %xmm1,%rcx

# qhasm:   in14 = z14
# asm 1: movd   <z14=int6464#3,>in14=int64#6
# asm 2: movd   <z14=%xmm2,>in14=%r9
movd   %xmm2,%r9

# qhasm:   in15 = z15
# asm 1: movd   <z15=int6464#4,>in15=int64#7
# asm 2: movd   <z15=%xmm3,>in15=%rax
movd   %xmm3,%rax

# qhasm:   z12 <<<= 96
# asm 1: pshufd $0x39,<z12=int6464#1,<z12=int6464#1
# asm 2: pshufd $0x39,<z12=%xmm0,<z12=%xmm0
pshufd $0x39,%xmm0,%xmm0

# qhasm:   z13 <<<= 96
# asm 1: pshufd $0x39,<z13=int6464#2,<z13=int6464#2
# asm 2: pshufd $0x39,<z13=%xmm1,<z13=%xmm1
pshufd $0x39,%xmm1,%xmm1

# qhasm:   z14 <<<= 96
# asm 1: pshufd $0x39,<z14=int6464#3,<z14=int6464#3
# asm 2: pshufd $0x39,<z14=%xmm2,<z14=%xmm2
pshufd $0x39,%xmm2,%xmm2

# qhasm:   z15 <<<= 96
# asm 1: pshufd $0x39,<z15=int6464#4,<z15=int6464#4
# asm 2: pshufd $0x39,<z15=%xmm3,<z15=%xmm3
pshufd $0x39,%xmm3,%xmm3

# qhasm:   (uint32) in12 ^= *(uint32 *) (m + 112)
# asm 1: xorl 112(<m=int64#2),<in12=int64#3d
# asm 2: xorl 112(<m=%rsi),<in12=%edx
xorl 112(%rsi),%edx

# qhasm:   (uint32) in13 ^= *(uint32 *) (m + 116)
# asm 1: xorl 116(<m=int64#2),<in13=int64#4d
# asm 2: xorl 116(<m=%rsi),<in13=%ecx
xorl 116(%rsi),%ecx

# qhasm:   (uint32) in14 ^= *(uint32 *) (m + 120)
# asm 1: xorl 120(<m=int64#2),<in14=int64#6d
# asm 2: xorl 120(<m=%rsi),<in14=%r9d
xorl 120(%rsi),%r9d

# qhasm:   (uint32) in15 ^= *(uint32 *) (m + 124)
# asm 1: xorl 124(<m=int64#2),<in15=int64#7d
# asm 2: xorl 124(<m=%rsi),<in15=%eax
xorl 124(%rsi),%eax

# qhasm:   *(uint32 *) (out + 112) = in12
# asm 1: movl   <in12=int64#3d,112(<out=int64#1)
# asm 2: movl   <in12=%edx,112(<out=%rdi)
movl   %edx,112(%rdi)

# qhasm:   *(uint32 *) (out + 116) = in13
# asm 1: movl   <in13=int64#4d,116(<out=int64#1)
# asm 2: movl   <in13=%ecx,116(<out=%rdi)
movl   %ecx,116(%rdi)

# qhasm:   *(uint32 *) (out + 120) = in14
# asm 1: movl   <in14=int64#6d,120(<out=int64#1)
# asm 2: movl   <in14=%r9d,120(<out=%rdi)
movl   %r9d,120(%rdi)

# qhasm:   *(uint32 *) (out + 124) = in15
# asm 1: movl   <in15=int64#7d,124(<out=int64#1)
# asm 2: movl   <in15=%eax,124(<out=%rdi)
movl   %eax,124(%rdi)

# qhasm:   in12 = z12
# asm 1: movd   <z12=int6464#1,>in12=int64#3
# asm 2: movd   <z12=%xmm0,>in12=%rdx
movd   %xmm0,%rdx

# qhasm:   in13 = z13
# asm 1: movd   <z13=int6464#2,>in13=int64#4
# asm 2: movd   <z13=%xmm1,>in13=%rcx
movd   %xmm1,%rcx

# qhasm:   in14 = z14
# asm 1: movd   <z14=int6464#3,>in14=int64#6
# asm 2: movd   <z14=%xmm2,>in14=%r9
movd   %xmm2,%r9

# qhasm:   in15 = z15
# asm 1: movd   <z15=int6464#4,>in15=int64#7
# asm 2: movd   <z15=%xmm3,>in15=%rax
movd   %xmm3,%rax

# qhasm:   z12 <<<= 96
# asm 1: pshufd $0x39,<z12=int6464#1,<z12=int6464#1
# asm 2: pshufd $0x39,<z12=%xmm0,<z12=%xmm0
pshufd $0x39,%xmm0,%xmm0

# qhasm:   z13 <<<= 96
# asm 1: pshufd $0x39,<z13=int6464#2,<z13=int6464#2
# asm 2: pshufd $0x39,<z13=%xmm1,<z13=%xmm1
pshufd $0x39,%xmm1,%xmm1

# qhasm:   z14 <<<= 96
# asm 1: pshufd $0x39,<z14=int6464#3,<z14=int6464#3
# asm 2: pshufd $0x39,<z14=%xmm2,<z14=%xmm2
pshufd $0x39,%xmm2,%xmm2

# qhasm:   z15 <<<= 96
# asm 1: pshufd $0x39,<z15=int6464#4,<z15=int6464#4
# asm 2: pshufd $0x39,<z15=%xmm3,<z15=%xmm3
pshufd $0x39,%xmm3,%xmm3

# qhasm:   (uint32) in12 ^= *(uint32 *) (m + 176)
# asm 1: xorl 176(<m=int64#2),<in12=int64#3d
# asm 2: xorl 176(<m=%rsi),<in12=%edx
xorl 176(%rsi),%edx

# qhasm:   (uint32) in13 ^= *(uint32 *) (m + 180)
# asm 1: xorl 180(<m=int64#2),<in13=int64#4d
# asm 2: xorl 180(<m=%rsi),<in13=%ecx
xorl 180(%rsi),%ecx

# qhasm:   (uint32) in14 ^= *(uint32 *) (m + 184)
# asm 1: xorl 184(<m=int64#2),<in14=int64#6d
# asm 2: xorl 184(<m=%rsi),<in14=%r9d
xorl 184(%rsi),%r9d

# qhasm:   (uint32) in15 ^= *(uint32 *) (m + 188)
# asm 1: xorl 188(<m=int64#2),<in15=int64#7d
# asm 2: xorl 188(<m=%rsi),<in15=%eax
xorl 188(%rsi),%eax

# qhasm:   *(uint32 *) (out + 176) = in12
# asm 1: movl   <in12=int64#3d,176(<out=int64#1)
# asm 2: movl   <in12=%edx,176(<out=%rdi)
movl   %edx,176(%rdi)

# qhasm:   *(uint32 *) (out + 180) = in13
# asm 1: movl   <in13=int64#4d,180(<out=int64#1)
# asm 2: movl   <in13=%ecx,180(<out=%rdi)
movl   %ecx,180(%rdi)

# qhasm:   *(uint32 *) (out + 184) = in14
# asm 1: movl   <in14=int64#6d,184(<out=int64#1)
# asm 2: movl   <in14=%r9d,184(<out=%rdi)
movl   %r9d,184(%rdi)

# qhasm:   *(uint32 *) (out + 188) = in15
# asm 1: movl   <in15=int64#7d,188(<out=int64#1)
# asm 2: movl   <in15=%eax,188(<out=%rdi)
movl   %eax,188(%rdi)

# qhasm:   in12 = z12
# asm 1: movd   <z12=int6464#1,>in12=int64#3
# asm 2: movd   <z12=%xmm0,>in12=%rdx
movd   %xmm0,%rdx

# qhasm:   in13 = z13
# asm 1: movd   <z13=int6464#2,>in13=int64#4
# asm 2: movd   <z13=%xmm1,>in13=%rcx
movd   %xmm1,%rcx

# qhasm:   in14 = z14
# asm 1: movd   <z14=int6464#3,>in14=int64#6
# asm 2: movd   <z14=%xmm2,>in14=%r9
movd   %xmm2,%r9

# qhasm:   in15 = z15
# asm 1: movd   <z15=int6464#4,>in15=int64#7
# asm 2: movd   <z15=%xmm3,>in15=%rax
movd   %xmm3,%rax

# qhasm:   (uint32) in12 ^= *(uint32 *) (m + 240)
# asm 1: xorl 240(<m=int64#2),<in12=int64#3d
# asm 2: xorl 240(<m=%rsi),<in12=%edx
xorl 240(%rsi),%edx

# qhasm:   (uint32) in13 ^= *(uint32 *) (m + 244)
# asm 1: xorl 244(<m=int64#2),<in13=int64#4d
# asm 2: xorl 244(<m=%rsi),<in13=%ecx
xorl 244(%rsi),%ecx

# qhasm:   (uint32) in14 ^= *(uint32 *) (m + 248)
# asm 1: xorl 248(<m=int64#2),<in14=int64#6d
# asm 2: xorl 248(<m=%rsi),<in14=%r9d
xorl 248(%rsi),%r9d

# qhasm:   (uint32) in15 ^= *(uint32 *) (m + 252)
# asm 1: xorl 252(<m=int64#2),<in15=int64#7d
# asm 2: xorl 252(<m=%rsi),<in15=%eax
xorl 252(%rsi),%eax

# qhasm:   *(uint32 *) (out + 240) = in12
# asm 1: movl   <in12=int64#3d,240(<out=int64#1)
# asm 2: movl   <in12=%edx,240(<out=%rdi)
movl   %edx,240(%rdi)

# qhasm:   *(uint32 *) (out + 244) = in13
# asm 1: movl   <in13=int64#4d,244(<out=int64#1)
# asm 2: movl   <in13=%ecx,244(<out=%rdi)
movl   %ecx,244(%rdi)

# qhasm:   *(uint32 *) (out + 248) = in14
# asm 1: movl   <in14=int64#6d,248(<out=int64#1)
# asm 2: movl   <in14=%r9d,248(<out=%rdi)
movl   %r9d,248(%rdi)

# qhasm:   *(uint32 *) (out + 252) = in15
# asm 1: movl   <in15=int64#7d,252(<out=int64#1)
# asm 2: movl   <in15=%eax,252(<out=%rdi)
movl   %eax,252(%rdi)

# qhasm:   bytes = bytes_backup
# asm 1: movq <bytes_backup=stack64#1,>bytes=int64#3
# asm 2: movq <bytes_backup=544(%rsp),>bytes=%rdx
movq 544(%rsp),%rdx

# qhasm:   bytes -= 256
# asm 1: sub  $256,<bytes=int64#3
# asm 2: sub  $256,<bytes=%rdx
sub  $256,%rdx

# qhasm:   m += 256
# asm 1: add  $256,<m=int64#2
# asm 2: add  $256,<m=%rsi
add  $256,%rsi

# qhasm:   out += 256
# asm 1: add  $256,<out=int64#1
# asm 2: add  $256,<out=%rdi
add  $256,%rdi

# qhasm:                            unsigned<? bytes - 256
# asm 1: cmp  $256,<bytes=int64#3
# asm 2: cmp  $256,<bytes=%rdx
cmp  $256,%rdx
# comment:fp stack unchanged by jump

# qhasm:   goto bytesatleast256 if !unsigned<
jae ._bytesatleast256

# qhasm:                 unsigned>? bytes - 0
# asm 1: cmp  $0,<bytes=int64#3
# asm 2: cmp  $0,<bytes=%rdx
cmp  $0,%rdx
# comment:fp stack unchanged by jump

# qhasm:   goto done if !unsigned>
jbe ._done
# comment:fp stack unchanged by fallthrough

# qhasm: bytesbetween1and255:
._bytesbetween1and255:

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
# asm 2: leaq <tmp=576(%rsp),>out=%rdi
leaq 576(%rsp),%rdi

# qhasm:     i = bytes
# asm 1: mov  <bytes=int64#3,>i=int64#4
# asm 2: mov  <bytes=%rdx,>i=%rcx
mov  %rdx,%rcx

# qhasm:     while (i) { *out++ = *m++; --i }
rep movsb

# qhasm:     out = &tmp
# asm 1: leaq <tmp=stack512#1,>out=int64#1
# asm 2: leaq <tmp=576(%rsp),>out=%rdi
leaq 576(%rsp),%rdi

# qhasm:     m = &tmp
# asm 1: leaq <tmp=stack512#1,>m=int64#2
# asm 2: leaq <tmp=576(%rsp),>m=%rsi
leaq 576(%rsp),%rsi
# comment:fp stack unchanged by fallthrough

# qhasm:   nocopy:
._nocopy:

# qhasm:   bytes_backup = bytes
# asm 1: movq <bytes=int64#3,>bytes_backup=stack64#1
# asm 2: movq <bytes=%rdx,>bytes_backup=544(%rsp)
movq %rdx,544(%rsp)

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

# qhasm:                     a0 = diag1
# asm 1: movdqa <diag1=int6464#2,>a0=int6464#5
# asm 2: movdqa <diag1=%xmm1,>a0=%xmm4
movdqa %xmm1,%xmm4

# qhasm: i = 20
# asm 1: mov  $20,>i=int64#3
# asm 2: mov  $20,>i=%rdx
mov  $20,%rdx

# qhasm: mainloop2:
._mainloop2:

# qhasm: uint32323232        a0 += diag0
# asm 1: paddd <diag0=int6464#1,<a0=int6464#5
# asm 2: paddd <diag0=%xmm0,<a0=%xmm4
paddd %xmm0,%xmm4

# qhasm:                                 a1 = diag0
# asm 1: movdqa <diag0=int6464#1,>a1=int6464#6
# asm 2: movdqa <diag0=%xmm0,>a1=%xmm5
movdqa %xmm0,%xmm5

# qhasm:                     b0 = a0
# asm 1: movdqa <a0=int6464#5,>b0=int6464#7
# asm 2: movdqa <a0=%xmm4,>b0=%xmm6
movdqa %xmm4,%xmm6

# qhasm: uint32323232        a0 <<= 7
# asm 1: pslld $7,<a0=int6464#5
# asm 2: pslld $7,<a0=%xmm4
pslld $7,%xmm4

# qhasm: uint32323232        b0 >>= 25
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

# qhasm: uint32323232                        a1 += diag3
# asm 1: paddd <diag3=int6464#4,<a1=int6464#6
# asm 2: paddd <diag3=%xmm3,<a1=%xmm5
paddd %xmm3,%xmm5

# qhasm:                                                 a2 = diag3
# asm 1: movdqa <diag3=int6464#4,>a2=int6464#5
# asm 2: movdqa <diag3=%xmm3,>a2=%xmm4
movdqa %xmm3,%xmm4

# qhasm:                                     b1 = a1
# asm 1: movdqa <a1=int6464#6,>b1=int6464#7
# asm 2: movdqa <a1=%xmm5,>b1=%xmm6
movdqa %xmm5,%xmm6

# qhasm: uint32323232                        a1 <<= 9
# asm 1: pslld $9,<a1=int6464#6
# asm 2: pslld $9,<a1=%xmm5
pslld $9,%xmm5

# qhasm: uint32323232                        b1 >>= 23
# asm 1: psrld $23,<b1=int6464#7
# asm 2: psrld $23,<b1=%xmm6
psrld $23,%xmm6

# qhasm:                                 diag2 ^= a1
# asm 1: pxor  <a1=int6464#6,<diag2=int6464#3
# asm 2: pxor  <a1=%xmm5,<diag2=%xmm2
pxor  %xmm5,%xmm2

# qhasm:                 diag3 <<<= 32
# asm 1: pshufd $0x93,<diag3=int6464#4,<diag3=int6464#4
# asm 2: pshufd $0x93,<diag3=%xmm3,<diag3=%xmm3
pshufd $0x93,%xmm3,%xmm3

# qhasm:                                 diag2 ^= b1
# asm 1: pxor  <b1=int6464#7,<diag2=int6464#3
# asm 2: pxor  <b1=%xmm6,<diag2=%xmm2
pxor  %xmm6,%xmm2

# qhasm: uint32323232                                        a2 += diag2
# asm 1: paddd <diag2=int6464#3,<a2=int6464#5
# asm 2: paddd <diag2=%xmm2,<a2=%xmm4
paddd %xmm2,%xmm4

# qhasm:                                                                 a3 = diag2
# asm 1: movdqa <diag2=int6464#3,>a3=int6464#6
# asm 2: movdqa <diag2=%xmm2,>a3=%xmm5
movdqa %xmm2,%xmm5

# qhasm:                                                     b2 = a2
# asm 1: movdqa <a2=int6464#5,>b2=int6464#7
# asm 2: movdqa <a2=%xmm4,>b2=%xmm6
movdqa %xmm4,%xmm6

# qhasm: uint32323232                                        a2 <<= 13
# asm 1: pslld $13,<a2=int6464#5
# asm 2: pslld $13,<a2=%xmm4
pslld $13,%xmm4

# qhasm: uint32323232                                        b2 >>= 19
# asm 1: psrld $19,<b2=int6464#7
# asm 2: psrld $19,<b2=%xmm6
psrld $19,%xmm6

# qhasm:                                                 diag1 ^= a2
# asm 1: pxor  <a2=int6464#5,<diag1=int6464#2
# asm 2: pxor  <a2=%xmm4,<diag1=%xmm1
pxor  %xmm4,%xmm1

# qhasm:                                 diag2 <<<= 64
# asm 1: pshufd $0x4e,<diag2=int6464#3,<diag2=int6464#3
# asm 2: pshufd $0x4e,<diag2=%xmm2,<diag2=%xmm2
pshufd $0x4e,%xmm2,%xmm2

# qhasm:                                                 diag1 ^= b2
# asm 1: pxor  <b2=int6464#7,<diag1=int6464#2
# asm 2: pxor  <b2=%xmm6,<diag1=%xmm1
pxor  %xmm6,%xmm1

# qhasm: uint32323232                                                        a3 += diag1
# asm 1: paddd <diag1=int6464#2,<a3=int6464#6
# asm 2: paddd <diag1=%xmm1,<a3=%xmm5
paddd %xmm1,%xmm5

# qhasm:                 a4 = diag3
# asm 1: movdqa <diag3=int6464#4,>a4=int6464#5
# asm 2: movdqa <diag3=%xmm3,>a4=%xmm4
movdqa %xmm3,%xmm4

# qhasm:                                                                     b3 = a3
# asm 1: movdqa <a3=int6464#6,>b3=int6464#7
# asm 2: movdqa <a3=%xmm5,>b3=%xmm6
movdqa %xmm5,%xmm6

# qhasm: uint32323232                                                        a3 <<= 18
# asm 1: pslld $18,<a3=int6464#6
# asm 2: pslld $18,<a3=%xmm5
pslld $18,%xmm5

# qhasm: uint32323232                                                        b3 >>= 14
# asm 1: psrld $14,<b3=int6464#7
# asm 2: psrld $14,<b3=%xmm6
psrld $14,%xmm6

# qhasm:                                                                 diag0 ^= a3
# asm 1: pxor  <a3=int6464#6,<diag0=int6464#1
# asm 2: pxor  <a3=%xmm5,<diag0=%xmm0
pxor  %xmm5,%xmm0

# qhasm:                                                 diag1 <<<= 96
# asm 1: pshufd $0x39,<diag1=int6464#2,<diag1=int6464#2
# asm 2: pshufd $0x39,<diag1=%xmm1,<diag1=%xmm1
pshufd $0x39,%xmm1,%xmm1

# qhasm:                                                                 diag0 ^= b3
# asm 1: pxor  <b3=int6464#7,<diag0=int6464#1
# asm 2: pxor  <b3=%xmm6,<diag0=%xmm0
pxor  %xmm6,%xmm0

# qhasm: uint32323232        a4 += diag0
# asm 1: paddd <diag0=int6464#1,<a4=int6464#5
# asm 2: paddd <diag0=%xmm0,<a4=%xmm4
paddd %xmm0,%xmm4

# qhasm:                                 a5 = diag0
# asm 1: movdqa <diag0=int6464#1,>a5=int6464#6
# asm 2: movdqa <diag0=%xmm0,>a5=%xmm5
movdqa %xmm0,%xmm5

# qhasm:                     b4 = a4
# asm 1: movdqa <a4=int6464#5,>b4=int6464#7
# asm 2: movdqa <a4=%xmm4,>b4=%xmm6
movdqa %xmm4,%xmm6

# qhasm: uint32323232        a4 <<= 7
# asm 1: pslld $7,<a4=int6464#5
# asm 2: pslld $7,<a4=%xmm4
pslld $7,%xmm4

# qhasm: uint32323232        b4 >>= 25
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

# qhasm: uint32323232                        a5 += diag1
# asm 1: paddd <diag1=int6464#2,<a5=int6464#6
# asm 2: paddd <diag1=%xmm1,<a5=%xmm5
paddd %xmm1,%xmm5

# qhasm:                                                 a6 = diag1
# asm 1: movdqa <diag1=int6464#2,>a6=int6464#5
# asm 2: movdqa <diag1=%xmm1,>a6=%xmm4
movdqa %xmm1,%xmm4

# qhasm:                                     b5 = a5
# asm 1: movdqa <a5=int6464#6,>b5=int6464#7
# asm 2: movdqa <a5=%xmm5,>b5=%xmm6
movdqa %xmm5,%xmm6

# qhasm: uint32323232                        a5 <<= 9
# asm 1: pslld $9,<a5=int6464#6
# asm 2: pslld $9,<a5=%xmm5
pslld $9,%xmm5

# qhasm: uint32323232                        b5 >>= 23
# asm 1: psrld $23,<b5=int6464#7
# asm 2: psrld $23,<b5=%xmm6
psrld $23,%xmm6

# qhasm:                                 diag2 ^= a5
# asm 1: pxor  <a5=int6464#6,<diag2=int6464#3
# asm 2: pxor  <a5=%xmm5,<diag2=%xmm2
pxor  %xmm5,%xmm2

# qhasm:                 diag1 <<<= 32
# asm 1: pshufd $0x93,<diag1=int6464#2,<diag1=int6464#2
# asm 2: pshufd $0x93,<diag1=%xmm1,<diag1=%xmm1
pshufd $0x93,%xmm1,%xmm1

# qhasm:                                 diag2 ^= b5
# asm 1: pxor  <b5=int6464#7,<diag2=int6464#3
# asm 2: pxor  <b5=%xmm6,<diag2=%xmm2
pxor  %xmm6,%xmm2

# qhasm: uint32323232                                        a6 += diag2
# asm 1: paddd <diag2=int6464#3,<a6=int6464#5
# asm 2: paddd <diag2=%xmm2,<a6=%xmm4
paddd %xmm2,%xmm4

# qhasm:                                                                 a7 = diag2
# asm 1: movdqa <diag2=int6464#3,>a7=int6464#6
# asm 2: movdqa <diag2=%xmm2,>a7=%xmm5
movdqa %xmm2,%xmm5

# qhasm:                                                     b6 = a6
# asm 1: movdqa <a6=int6464#5,>b6=int6464#7
# asm 2: movdqa <a6=%xmm4,>b6=%xmm6
movdqa %xmm4,%xmm6

# qhasm: uint32323232                                        a6 <<= 13
# asm 1: pslld $13,<a6=int6464#5
# asm 2: pslld $13,<a6=%xmm4
pslld $13,%xmm4

# qhasm: uint32323232                                        b6 >>= 19
# asm 1: psrld $19,<b6=int6464#7
# asm 2: psrld $19,<b6=%xmm6
psrld $19,%xmm6

# qhasm:                                                 diag3 ^= a6
# asm 1: pxor  <a6=int6464#5,<diag3=int6464#4
# asm 2: pxor  <a6=%xmm4,<diag3=%xmm3
pxor  %xmm4,%xmm3

# qhasm:                                 diag2 <<<= 64
# asm 1: pshufd $0x4e,<diag2=int6464#3,<diag2=int6464#3
# asm 2: pshufd $0x4e,<diag2=%xmm2,<diag2=%xmm2
pshufd $0x4e,%xmm2,%xmm2

# qhasm:                                                 diag3 ^= b6
# asm 1: pxor  <b6=int6464#7,<diag3=int6464#4
# asm 2: pxor  <b6=%xmm6,<diag3=%xmm3
pxor  %xmm6,%xmm3

# qhasm: uint32323232                                                        a7 += diag3
# asm 1: paddd <diag3=int6464#4,<a7=int6464#6
# asm 2: paddd <diag3=%xmm3,<a7=%xmm5
paddd %xmm3,%xmm5

# qhasm:                 a0 = diag1
# asm 1: movdqa <diag1=int6464#2,>a0=int6464#5
# asm 2: movdqa <diag1=%xmm1,>a0=%xmm4
movdqa %xmm1,%xmm4

# qhasm:                                                                     b7 = a7
# asm 1: movdqa <a7=int6464#6,>b7=int6464#7
# asm 2: movdqa <a7=%xmm5,>b7=%xmm6
movdqa %xmm5,%xmm6

# qhasm: uint32323232                                                        a7 <<= 18
# asm 1: pslld $18,<a7=int6464#6
# asm 2: pslld $18,<a7=%xmm5
pslld $18,%xmm5

# qhasm: uint32323232                                                        b7 >>= 14
# asm 1: psrld $14,<b7=int6464#7
# asm 2: psrld $14,<b7=%xmm6
psrld $14,%xmm6

# qhasm:                                                                 diag0 ^= a7
# asm 1: pxor  <a7=int6464#6,<diag0=int6464#1
# asm 2: pxor  <a7=%xmm5,<diag0=%xmm0
pxor  %xmm5,%xmm0

# qhasm:                                                 diag3 <<<= 96
# asm 1: pshufd $0x39,<diag3=int6464#4,<diag3=int6464#4
# asm 2: pshufd $0x39,<diag3=%xmm3,<diag3=%xmm3
pshufd $0x39,%xmm3,%xmm3

# qhasm:                                                                 diag0 ^= b7
# asm 1: pxor  <b7=int6464#7,<diag0=int6464#1
# asm 2: pxor  <b7=%xmm6,<diag0=%xmm0
pxor  %xmm6,%xmm0

# qhasm: uint32323232        a0 += diag0
# asm 1: paddd <diag0=int6464#1,<a0=int6464#5
# asm 2: paddd <diag0=%xmm0,<a0=%xmm4
paddd %xmm0,%xmm4

# qhasm:                                 a1 = diag0
# asm 1: movdqa <diag0=int6464#1,>a1=int6464#6
# asm 2: movdqa <diag0=%xmm0,>a1=%xmm5
movdqa %xmm0,%xmm5

# qhasm:                     b0 = a0
# asm 1: movdqa <a0=int6464#5,>b0=int6464#7
# asm 2: movdqa <a0=%xmm4,>b0=%xmm6
movdqa %xmm4,%xmm6

# qhasm: uint32323232        a0 <<= 7
# asm 1: pslld $7,<a0=int6464#5
# asm 2: pslld $7,<a0=%xmm4
pslld $7,%xmm4

# qhasm: uint32323232        b0 >>= 25
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

# qhasm: uint32323232                        a1 += diag3
# asm 1: paddd <diag3=int6464#4,<a1=int6464#6
# asm 2: paddd <diag3=%xmm3,<a1=%xmm5
paddd %xmm3,%xmm5

# qhasm:                                                 a2 = diag3
# asm 1: movdqa <diag3=int6464#4,>a2=int6464#5
# asm 2: movdqa <diag3=%xmm3,>a2=%xmm4
movdqa %xmm3,%xmm4

# qhasm:                                     b1 = a1
# asm 1: movdqa <a1=int6464#6,>b1=int6464#7
# asm 2: movdqa <a1=%xmm5,>b1=%xmm6
movdqa %xmm5,%xmm6

# qhasm: uint32323232                        a1 <<= 9
# asm 1: pslld $9,<a1=int6464#6
# asm 2: pslld $9,<a1=%xmm5
pslld $9,%xmm5

# qhasm: uint32323232                        b1 >>= 23
# asm 1: psrld $23,<b1=int6464#7
# asm 2: psrld $23,<b1=%xmm6
psrld $23,%xmm6

# qhasm:                                 diag2 ^= a1
# asm 1: pxor  <a1=int6464#6,<diag2=int6464#3
# asm 2: pxor  <a1=%xmm5,<diag2=%xmm2
pxor  %xmm5,%xmm2

# qhasm:                 diag3 <<<= 32
# asm 1: pshufd $0x93,<diag3=int6464#4,<diag3=int6464#4
# asm 2: pshufd $0x93,<diag3=%xmm3,<diag3=%xmm3
pshufd $0x93,%xmm3,%xmm3

# qhasm:                                 diag2 ^= b1
# asm 1: pxor  <b1=int6464#7,<diag2=int6464#3
# asm 2: pxor  <b1=%xmm6,<diag2=%xmm2
pxor  %xmm6,%xmm2

# qhasm: uint32323232                                        a2 += diag2
# asm 1: paddd <diag2=int6464#3,<a2=int6464#5
# asm 2: paddd <diag2=%xmm2,<a2=%xmm4
paddd %xmm2,%xmm4

# qhasm:                                                                 a3 = diag2
# asm 1: movdqa <diag2=int6464#3,>a3=int6464#6
# asm 2: movdqa <diag2=%xmm2,>a3=%xmm5
movdqa %xmm2,%xmm5

# qhasm:                                                     b2 = a2
# asm 1: movdqa <a2=int6464#5,>b2=int6464#7
# asm 2: movdqa <a2=%xmm4,>b2=%xmm6
movdqa %xmm4,%xmm6

# qhasm: uint32323232                                        a2 <<= 13
# asm 1: pslld $13,<a2=int6464#5
# asm 2: pslld $13,<a2=%xmm4
pslld $13,%xmm4

# qhasm: uint32323232                                        b2 >>= 19
# asm 1: psrld $19,<b2=int6464#7
# asm 2: psrld $19,<b2=%xmm6
psrld $19,%xmm6

# qhasm:                                                 diag1 ^= a2
# asm 1: pxor  <a2=int6464#5,<diag1=int6464#2
# asm 2: pxor  <a2=%xmm4,<diag1=%xmm1
pxor  %xmm4,%xmm1

# qhasm:                                 diag2 <<<= 64
# asm 1: pshufd $0x4e,<diag2=int6464#3,<diag2=int6464#3
# asm 2: pshufd $0x4e,<diag2=%xmm2,<diag2=%xmm2
pshufd $0x4e,%xmm2,%xmm2

# qhasm:                                                 diag1 ^= b2
# asm 1: pxor  <b2=int6464#7,<diag1=int6464#2
# asm 2: pxor  <b2=%xmm6,<diag1=%xmm1
pxor  %xmm6,%xmm1

# qhasm: uint32323232                                                        a3 += diag1
# asm 1: paddd <diag1=int6464#2,<a3=int6464#6
# asm 2: paddd <diag1=%xmm1,<a3=%xmm5
paddd %xmm1,%xmm5

# qhasm:                 a4 = diag3
# asm 1: movdqa <diag3=int6464#4,>a4=int6464#5
# asm 2: movdqa <diag3=%xmm3,>a4=%xmm4
movdqa %xmm3,%xmm4

# qhasm:                                                                     b3 = a3
# asm 1: movdqa <a3=int6464#6,>b3=int6464#7
# asm 2: movdqa <a3=%xmm5,>b3=%xmm6
movdqa %xmm5,%xmm6

# qhasm: uint32323232                                                        a3 <<= 18
# asm 1: pslld $18,<a3=int6464#6
# asm 2: pslld $18,<a3=%xmm5
pslld $18,%xmm5

# qhasm: uint32323232                                                        b3 >>= 14
# asm 1: psrld $14,<b3=int6464#7
# asm 2: psrld $14,<b3=%xmm6
psrld $14,%xmm6

# qhasm:                                                                 diag0 ^= a3
# asm 1: pxor  <a3=int6464#6,<diag0=int6464#1
# asm 2: pxor  <a3=%xmm5,<diag0=%xmm0
pxor  %xmm5,%xmm0

# qhasm:                                                 diag1 <<<= 96
# asm 1: pshufd $0x39,<diag1=int6464#2,<diag1=int6464#2
# asm 2: pshufd $0x39,<diag1=%xmm1,<diag1=%xmm1
pshufd $0x39,%xmm1,%xmm1

# qhasm:                                                                 diag0 ^= b3
# asm 1: pxor  <b3=int6464#7,<diag0=int6464#1
# asm 2: pxor  <b3=%xmm6,<diag0=%xmm0
pxor  %xmm6,%xmm0

# qhasm: uint32323232        a4 += diag0
# asm 1: paddd <diag0=int6464#1,<a4=int6464#5
# asm 2: paddd <diag0=%xmm0,<a4=%xmm4
paddd %xmm0,%xmm4

# qhasm:                                 a5 = diag0
# asm 1: movdqa <diag0=int6464#1,>a5=int6464#6
# asm 2: movdqa <diag0=%xmm0,>a5=%xmm5
movdqa %xmm0,%xmm5

# qhasm:                     b4 = a4
# asm 1: movdqa <a4=int6464#5,>b4=int6464#7
# asm 2: movdqa <a4=%xmm4,>b4=%xmm6
movdqa %xmm4,%xmm6

# qhasm: uint32323232        a4 <<= 7
# asm 1: pslld $7,<a4=int6464#5
# asm 2: pslld $7,<a4=%xmm4
pslld $7,%xmm4

# qhasm: uint32323232        b4 >>= 25
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

# qhasm: uint32323232                        a5 += diag1
# asm 1: paddd <diag1=int6464#2,<a5=int6464#6
# asm 2: paddd <diag1=%xmm1,<a5=%xmm5
paddd %xmm1,%xmm5

# qhasm:                                                 a6 = diag1
# asm 1: movdqa <diag1=int6464#2,>a6=int6464#5
# asm 2: movdqa <diag1=%xmm1,>a6=%xmm4
movdqa %xmm1,%xmm4

# qhasm:                                     b5 = a5
# asm 1: movdqa <a5=int6464#6,>b5=int6464#7
# asm 2: movdqa <a5=%xmm5,>b5=%xmm6
movdqa %xmm5,%xmm6

# qhasm: uint32323232                        a5 <<= 9
# asm 1: pslld $9,<a5=int6464#6
# asm 2: pslld $9,<a5=%xmm5
pslld $9,%xmm5

# qhasm: uint32323232                        b5 >>= 23
# asm 1: psrld $23,<b5=int6464#7
# asm 2: psrld $23,<b5=%xmm6
psrld $23,%xmm6

# qhasm:                                 diag2 ^= a5
# asm 1: pxor  <a5=int6464#6,<diag2=int6464#3
# asm 2: pxor  <a5=%xmm5,<diag2=%xmm2
pxor  %xmm5,%xmm2

# qhasm:                 diag1 <<<= 32
# asm 1: pshufd $0x93,<diag1=int6464#2,<diag1=int6464#2
# asm 2: pshufd $0x93,<diag1=%xmm1,<diag1=%xmm1
pshufd $0x93,%xmm1,%xmm1

# qhasm:                                 diag2 ^= b5
# asm 1: pxor  <b5=int6464#7,<diag2=int6464#3
# asm 2: pxor  <b5=%xmm6,<diag2=%xmm2
pxor  %xmm6,%xmm2

# qhasm: uint32323232                                        a6 += diag2
# asm 1: paddd <diag2=int6464#3,<a6=int6464#5
# asm 2: paddd <diag2=%xmm2,<a6=%xmm4
paddd %xmm2,%xmm4

# qhasm:                                                                 a7 = diag2
# asm 1: movdqa <diag2=int6464#3,>a7=int6464#6
# asm 2: movdqa <diag2=%xmm2,>a7=%xmm5
movdqa %xmm2,%xmm5

# qhasm:                                                     b6 = a6
# asm 1: movdqa <a6=int6464#5,>b6=int6464#7
# asm 2: movdqa <a6=%xmm4,>b6=%xmm6
movdqa %xmm4,%xmm6

# qhasm: uint32323232                                        a6 <<= 13
# asm 1: pslld $13,<a6=int6464#5
# asm 2: pslld $13,<a6=%xmm4
pslld $13,%xmm4

# qhasm: uint32323232                                        b6 >>= 19
# asm 1: psrld $19,<b6=int6464#7
# asm 2: psrld $19,<b6=%xmm6
psrld $19,%xmm6

# qhasm:                                                 diag3 ^= a6
# asm 1: pxor  <a6=int6464#5,<diag3=int6464#4
# asm 2: pxor  <a6=%xmm4,<diag3=%xmm3
pxor  %xmm4,%xmm3

# qhasm:                                 diag2 <<<= 64
# asm 1: pshufd $0x4e,<diag2=int6464#3,<diag2=int6464#3
# asm 2: pshufd $0x4e,<diag2=%xmm2,<diag2=%xmm2
pshufd $0x4e,%xmm2,%xmm2

# qhasm:                                                 diag3 ^= b6
# asm 1: pxor  <b6=int6464#7,<diag3=int6464#4
# asm 2: pxor  <b6=%xmm6,<diag3=%xmm3
pxor  %xmm6,%xmm3

# qhasm:                  unsigned>? i -= 4
# asm 1: sub  $4,<i=int64#3
# asm 2: sub  $4,<i=%rdx
sub  $4,%rdx

# qhasm: uint32323232                                                        a7 += diag3
# asm 1: paddd <diag3=int6464#4,<a7=int6464#6
# asm 2: paddd <diag3=%xmm3,<a7=%xmm5
paddd %xmm3,%xmm5

# qhasm:                 a0 = diag1
# asm 1: movdqa <diag1=int6464#2,>a0=int6464#5
# asm 2: movdqa <diag1=%xmm1,>a0=%xmm4
movdqa %xmm1,%xmm4

# qhasm:                                                                     b7 = a7
# asm 1: movdqa <a7=int6464#6,>b7=int6464#7
# asm 2: movdqa <a7=%xmm5,>b7=%xmm6
movdqa %xmm5,%xmm6

# qhasm: uint32323232                                                        a7 <<= 18
# asm 1: pslld $18,<a7=int6464#6
# asm 2: pslld $18,<a7=%xmm5
pslld $18,%xmm5

# qhasm:                 b0 = 0
# asm 1: pxor   >b0=int6464#8,>b0=int6464#8
# asm 2: pxor   >b0=%xmm7,>b0=%xmm7
pxor   %xmm7,%xmm7

# qhasm: uint32323232                                                        b7 >>= 14
# asm 1: psrld $14,<b7=int6464#7
# asm 2: psrld $14,<b7=%xmm6
psrld $14,%xmm6

# qhasm:                                                                 diag0 ^= a7
# asm 1: pxor  <a7=int6464#6,<diag0=int6464#1
# asm 2: pxor  <a7=%xmm5,<diag0=%xmm0
pxor  %xmm5,%xmm0

# qhasm:                                                 diag3 <<<= 96
# asm 1: pshufd $0x39,<diag3=int6464#4,<diag3=int6464#4
# asm 2: pshufd $0x39,<diag3=%xmm3,<diag3=%xmm3
pshufd $0x39,%xmm3,%xmm3

# qhasm:                                                                 diag0 ^= b7
# asm 1: pxor  <b7=int6464#7,<diag0=int6464#1
# asm 2: pxor  <b7=%xmm6,<diag0=%xmm0
pxor  %xmm6,%xmm0
# comment:fp stack unchanged by jump

# qhasm: goto mainloop2 if unsigned>
ja ._mainloop2

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

# qhasm:   bytes = bytes_backup
# asm 1: movq <bytes_backup=stack64#1,>bytes=int64#3
# asm 2: movq <bytes_backup=544(%rsp),>bytes=%rdx
movq 544(%rsp),%rdx

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

# qhasm:   in9 = in8
# asm 1: mov  <in8=int64#4,>in9=int64#7
# asm 2: mov  <in8=%rcx,>in9=%rax
mov  %rcx,%rax

# qhasm:   (uint64) in9 >>= 32
# asm 1: shr  $32,<in9=int64#7
# asm 2: shr  $32,<in9=%rax
shr  $32,%rax

# qhasm:   *(uint32 *) (x + 32) = in8
# asm 1: movl   <in8=int64#4d,32(<x=int64#5)
# asm 2: movl   <in8=%ecx,32(<x=%r8)
movl   %ecx,32(%r8)

# qhasm:   *(uint32 *) (x + 52) = in9
# asm 1: movl   <in9=int64#7d,52(<x=int64#5)
# asm 2: movl   <in9=%eax,52(<x=%r8)
movl   %eax,52(%r8)

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

# qhasm: goto bytesbetween1and255
jmp ._bytesbetween1and255

# qhasm: enter ECRYPT_init
.text
.p2align 5
.globl _ECRYPT_init
.globl ECRYPT_init
_ECRYPT_init:
ECRYPT_init:
mov %rsp,%r11
and $31,%r11
add $640,%r11
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
add $640,%r11
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
add $640,%r11
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
