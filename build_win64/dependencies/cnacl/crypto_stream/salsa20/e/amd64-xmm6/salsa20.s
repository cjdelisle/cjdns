
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

# qhasm: int6464 y0

# qhasm: int6464 y1

# qhasm: int6464 y2

# qhasm: int6464 y3

# qhasm: int6464 y4

# qhasm: int6464 y5

# qhasm: int6464 y6

# qhasm: int6464 y7

# qhasm: int6464 y8

# qhasm: int6464 y9

# qhasm: int6464 y10

# qhasm: int6464 y11

# qhasm: int6464 y12

# qhasm: int6464 y13

# qhasm: int6464 y14

# qhasm: int6464 y15

# qhasm: int6464 r0

# qhasm: int6464 r1

# qhasm: int6464 r2

# qhasm: int6464 r3

# qhasm: int6464 r4

# qhasm: int6464 r5

# qhasm: int6464 r6

# qhasm: int6464 r7

# qhasm: int6464 r8

# qhasm: int6464 r9

# qhasm: int6464 r10

# qhasm: int6464 r11

# qhasm: int6464 r12

# qhasm: int6464 r13

# qhasm: int6464 r14

# qhasm: int6464 r15

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
add $384,%r11
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
add $384,%r11
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
add $384,%r11
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
# asm 2: movq <bytes=%rdx,>bytes_backup=288(%rsp)
movq %rdx,288(%rsp)

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

# qhasm:   z7 = orig7
# asm 1: movdqa <orig7=stack128#9,>z7=int6464#9
# asm 2: movdqa <orig7=128(%rsp),>z7=%xmm8
movdqa 128(%rsp),%xmm8

# qhasm:   z13 = orig13
# asm 1: movdqa <orig13=stack128#10,>z13=int6464#10
# asm 2: movdqa <orig13=144(%rsp),>z13=%xmm9
movdqa 144(%rsp),%xmm9

# qhasm:   z2 = orig2
# asm 1: movdqa <orig2=stack128#11,>z2=int6464#11
# asm 2: movdqa <orig2=160(%rsp),>z2=%xmm10
movdqa 160(%rsp),%xmm10

# qhasm:   z9 = orig9
# asm 1: movdqa <orig9=stack128#16,>z9=int6464#12
# asm 2: movdqa <orig9=240(%rsp),>z9=%xmm11
movdqa 240(%rsp),%xmm11

# qhasm:   z0 = orig0
# asm 1: movdqa <orig0=stack128#4,>z0=int6464#13
# asm 2: movdqa <orig0=48(%rsp),>z0=%xmm12
movdqa 48(%rsp),%xmm12

# qhasm:   z12 = orig12
# asm 1: movdqa <orig12=stack128#7,>z12=int6464#14
# asm 2: movdqa <orig12=96(%rsp),>z12=%xmm13
movdqa 96(%rsp),%xmm13

# qhasm:   z4 = orig4
# asm 1: movdqa <orig4=stack128#12,>z4=int6464#15
# asm 2: movdqa <orig4=176(%rsp),>z4=%xmm14
movdqa 176(%rsp),%xmm14

# qhasm:   z8 = orig8
# asm 1: movdqa <orig8=stack128#15,>z8=int6464#16
# asm 2: movdqa <orig8=224(%rsp),>z8=%xmm15
movdqa 224(%rsp),%xmm15

# qhasm: mainloop1:
._mainloop1:

# qhasm: 						z10_stack = z10
# asm 1: movdqa <z10=int6464#2,>z10_stack=stack128#17
# asm 2: movdqa <z10=%xmm1,>z10_stack=256(%rsp)
movdqa %xmm1,256(%rsp)

# qhasm: 								z15_stack = z15
# asm 1: movdqa <z15=int6464#3,>z15_stack=stack128#18
# asm 2: movdqa <z15=%xmm2,>z15_stack=272(%rsp)
movdqa %xmm2,272(%rsp)

# qhasm: 		y4 = z12
# asm 1: movdqa <z12=int6464#14,>y4=int6464#2
# asm 2: movdqa <z12=%xmm13,>y4=%xmm1
movdqa %xmm13,%xmm1

# qhasm: uint32323232	y4 += z0
# asm 1: paddd <z0=int6464#13,<y4=int6464#2
# asm 2: paddd <z0=%xmm12,<y4=%xmm1
paddd %xmm12,%xmm1

# qhasm: 		r4 = y4
# asm 1: movdqa <y4=int6464#2,>r4=int6464#3
# asm 2: movdqa <y4=%xmm1,>r4=%xmm2
movdqa %xmm1,%xmm2

# qhasm: uint32323232	y4 <<= 7
# asm 1: pslld $7,<y4=int6464#2
# asm 2: pslld $7,<y4=%xmm1
pslld $7,%xmm1

# qhasm: 		z4 ^= y4
# asm 1: pxor  <y4=int6464#2,<z4=int6464#15
# asm 2: pxor  <y4=%xmm1,<z4=%xmm14
pxor  %xmm1,%xmm14

# qhasm: uint32323232	r4 >>= 25
# asm 1: psrld $25,<r4=int6464#3
# asm 2: psrld $25,<r4=%xmm2
psrld $25,%xmm2

# qhasm: 		z4 ^= r4
# asm 1: pxor  <r4=int6464#3,<z4=int6464#15
# asm 2: pxor  <r4=%xmm2,<z4=%xmm14
pxor  %xmm2,%xmm14

# qhasm: 				y9 = z1
# asm 1: movdqa <z1=int6464#8,>y9=int6464#2
# asm 2: movdqa <z1=%xmm7,>y9=%xmm1
movdqa %xmm7,%xmm1

# qhasm: uint32323232			y9 += z5
# asm 1: paddd <z5=int6464#1,<y9=int6464#2
# asm 2: paddd <z5=%xmm0,<y9=%xmm1
paddd %xmm0,%xmm1

# qhasm: 				r9 = y9
# asm 1: movdqa <y9=int6464#2,>r9=int6464#3
# asm 2: movdqa <y9=%xmm1,>r9=%xmm2
movdqa %xmm1,%xmm2

# qhasm: uint32323232			y9 <<= 7
# asm 1: pslld $7,<y9=int6464#2
# asm 2: pslld $7,<y9=%xmm1
pslld $7,%xmm1

# qhasm: 				z9 ^= y9
# asm 1: pxor  <y9=int6464#2,<z9=int6464#12
# asm 2: pxor  <y9=%xmm1,<z9=%xmm11
pxor  %xmm1,%xmm11

# qhasm: uint32323232			r9 >>= 25
# asm 1: psrld $25,<r9=int6464#3
# asm 2: psrld $25,<r9=%xmm2
psrld $25,%xmm2

# qhasm: 				z9 ^= r9
# asm 1: pxor  <r9=int6464#3,<z9=int6464#12
# asm 2: pxor  <r9=%xmm2,<z9=%xmm11
pxor  %xmm2,%xmm11

# qhasm: 		y8 = z0
# asm 1: movdqa <z0=int6464#13,>y8=int6464#2
# asm 2: movdqa <z0=%xmm12,>y8=%xmm1
movdqa %xmm12,%xmm1

# qhasm: uint32323232	y8 += z4
# asm 1: paddd <z4=int6464#15,<y8=int6464#2
# asm 2: paddd <z4=%xmm14,<y8=%xmm1
paddd %xmm14,%xmm1

# qhasm: 		r8 = y8
# asm 1: movdqa <y8=int6464#2,>r8=int6464#3
# asm 2: movdqa <y8=%xmm1,>r8=%xmm2
movdqa %xmm1,%xmm2

# qhasm: uint32323232	y8 <<= 9
# asm 1: pslld $9,<y8=int6464#2
# asm 2: pslld $9,<y8=%xmm1
pslld $9,%xmm1

# qhasm: 		z8 ^= y8
# asm 1: pxor  <y8=int6464#2,<z8=int6464#16
# asm 2: pxor  <y8=%xmm1,<z8=%xmm15
pxor  %xmm1,%xmm15

# qhasm: uint32323232	r8 >>= 23
# asm 1: psrld $23,<r8=int6464#3
# asm 2: psrld $23,<r8=%xmm2
psrld $23,%xmm2

# qhasm: 		z8 ^= r8
# asm 1: pxor  <r8=int6464#3,<z8=int6464#16
# asm 2: pxor  <r8=%xmm2,<z8=%xmm15
pxor  %xmm2,%xmm15

# qhasm: 				y13 = z5
# asm 1: movdqa <z5=int6464#1,>y13=int6464#2
# asm 2: movdqa <z5=%xmm0,>y13=%xmm1
movdqa %xmm0,%xmm1

# qhasm: uint32323232			y13 += z9
# asm 1: paddd <z9=int6464#12,<y13=int6464#2
# asm 2: paddd <z9=%xmm11,<y13=%xmm1
paddd %xmm11,%xmm1

# qhasm: 				r13 = y13
# asm 1: movdqa <y13=int6464#2,>r13=int6464#3
# asm 2: movdqa <y13=%xmm1,>r13=%xmm2
movdqa %xmm1,%xmm2

# qhasm: uint32323232			y13 <<= 9
# asm 1: pslld $9,<y13=int6464#2
# asm 2: pslld $9,<y13=%xmm1
pslld $9,%xmm1

# qhasm: 				z13 ^= y13
# asm 1: pxor  <y13=int6464#2,<z13=int6464#10
# asm 2: pxor  <y13=%xmm1,<z13=%xmm9
pxor  %xmm1,%xmm9

# qhasm: uint32323232			r13 >>= 23
# asm 1: psrld $23,<r13=int6464#3
# asm 2: psrld $23,<r13=%xmm2
psrld $23,%xmm2

# qhasm: 				z13 ^= r13
# asm 1: pxor  <r13=int6464#3,<z13=int6464#10
# asm 2: pxor  <r13=%xmm2,<z13=%xmm9
pxor  %xmm2,%xmm9

# qhasm: 		y12 = z4
# asm 1: movdqa <z4=int6464#15,>y12=int6464#2
# asm 2: movdqa <z4=%xmm14,>y12=%xmm1
movdqa %xmm14,%xmm1

# qhasm: uint32323232	y12 += z8
# asm 1: paddd <z8=int6464#16,<y12=int6464#2
# asm 2: paddd <z8=%xmm15,<y12=%xmm1
paddd %xmm15,%xmm1

# qhasm: 		r12 = y12
# asm 1: movdqa <y12=int6464#2,>r12=int6464#3
# asm 2: movdqa <y12=%xmm1,>r12=%xmm2
movdqa %xmm1,%xmm2

# qhasm: uint32323232	y12 <<= 13
# asm 1: pslld $13,<y12=int6464#2
# asm 2: pslld $13,<y12=%xmm1
pslld $13,%xmm1

# qhasm: 		z12 ^= y12
# asm 1: pxor  <y12=int6464#2,<z12=int6464#14
# asm 2: pxor  <y12=%xmm1,<z12=%xmm13
pxor  %xmm1,%xmm13

# qhasm: uint32323232	r12 >>= 19
# asm 1: psrld $19,<r12=int6464#3
# asm 2: psrld $19,<r12=%xmm2
psrld $19,%xmm2

# qhasm: 		z12 ^= r12
# asm 1: pxor  <r12=int6464#3,<z12=int6464#14
# asm 2: pxor  <r12=%xmm2,<z12=%xmm13
pxor  %xmm2,%xmm13

# qhasm: 				y1 = z9
# asm 1: movdqa <z9=int6464#12,>y1=int6464#2
# asm 2: movdqa <z9=%xmm11,>y1=%xmm1
movdqa %xmm11,%xmm1

# qhasm: uint32323232			y1 += z13
# asm 1: paddd <z13=int6464#10,<y1=int6464#2
# asm 2: paddd <z13=%xmm9,<y1=%xmm1
paddd %xmm9,%xmm1

# qhasm: 				r1 = y1
# asm 1: movdqa <y1=int6464#2,>r1=int6464#3
# asm 2: movdqa <y1=%xmm1,>r1=%xmm2
movdqa %xmm1,%xmm2

# qhasm: uint32323232			y1 <<= 13
# asm 1: pslld $13,<y1=int6464#2
# asm 2: pslld $13,<y1=%xmm1
pslld $13,%xmm1

# qhasm: 				z1 ^= y1
# asm 1: pxor  <y1=int6464#2,<z1=int6464#8
# asm 2: pxor  <y1=%xmm1,<z1=%xmm7
pxor  %xmm1,%xmm7

# qhasm: uint32323232			r1 >>= 19
# asm 1: psrld $19,<r1=int6464#3
# asm 2: psrld $19,<r1=%xmm2
psrld $19,%xmm2

# qhasm: 				z1 ^= r1
# asm 1: pxor  <r1=int6464#3,<z1=int6464#8
# asm 2: pxor  <r1=%xmm2,<z1=%xmm7
pxor  %xmm2,%xmm7

# qhasm: 		y0 = z8
# asm 1: movdqa <z8=int6464#16,>y0=int6464#2
# asm 2: movdqa <z8=%xmm15,>y0=%xmm1
movdqa %xmm15,%xmm1

# qhasm: uint32323232	y0 += z12
# asm 1: paddd <z12=int6464#14,<y0=int6464#2
# asm 2: paddd <z12=%xmm13,<y0=%xmm1
paddd %xmm13,%xmm1

# qhasm: 		r0 = y0
# asm 1: movdqa <y0=int6464#2,>r0=int6464#3
# asm 2: movdqa <y0=%xmm1,>r0=%xmm2
movdqa %xmm1,%xmm2

# qhasm: uint32323232	y0 <<= 18
# asm 1: pslld $18,<y0=int6464#2
# asm 2: pslld $18,<y0=%xmm1
pslld $18,%xmm1

# qhasm: 		z0 ^= y0
# asm 1: pxor  <y0=int6464#2,<z0=int6464#13
# asm 2: pxor  <y0=%xmm1,<z0=%xmm12
pxor  %xmm1,%xmm12

# qhasm: uint32323232	r0 >>= 14
# asm 1: psrld $14,<r0=int6464#3
# asm 2: psrld $14,<r0=%xmm2
psrld $14,%xmm2

# qhasm: 		z0 ^= r0
# asm 1: pxor  <r0=int6464#3,<z0=int6464#13
# asm 2: pxor  <r0=%xmm2,<z0=%xmm12
pxor  %xmm2,%xmm12

# qhasm: 						z10 = z10_stack
# asm 1: movdqa <z10_stack=stack128#17,>z10=int6464#2
# asm 2: movdqa <z10_stack=256(%rsp),>z10=%xmm1
movdqa 256(%rsp),%xmm1

# qhasm: 		z0_stack = z0
# asm 1: movdqa <z0=int6464#13,>z0_stack=stack128#17
# asm 2: movdqa <z0=%xmm12,>z0_stack=256(%rsp)
movdqa %xmm12,256(%rsp)

# qhasm: 				y5 = z13
# asm 1: movdqa <z13=int6464#10,>y5=int6464#3
# asm 2: movdqa <z13=%xmm9,>y5=%xmm2
movdqa %xmm9,%xmm2

# qhasm: uint32323232			y5 += z1
# asm 1: paddd <z1=int6464#8,<y5=int6464#3
# asm 2: paddd <z1=%xmm7,<y5=%xmm2
paddd %xmm7,%xmm2

# qhasm: 				r5 = y5
# asm 1: movdqa <y5=int6464#3,>r5=int6464#13
# asm 2: movdqa <y5=%xmm2,>r5=%xmm12
movdqa %xmm2,%xmm12

# qhasm: uint32323232			y5 <<= 18
# asm 1: pslld $18,<y5=int6464#3
# asm 2: pslld $18,<y5=%xmm2
pslld $18,%xmm2

# qhasm: 				z5 ^= y5
# asm 1: pxor  <y5=int6464#3,<z5=int6464#1
# asm 2: pxor  <y5=%xmm2,<z5=%xmm0
pxor  %xmm2,%xmm0

# qhasm: uint32323232			r5 >>= 14
# asm 1: psrld $14,<r5=int6464#13
# asm 2: psrld $14,<r5=%xmm12
psrld $14,%xmm12

# qhasm: 				z5 ^= r5
# asm 1: pxor  <r5=int6464#13,<z5=int6464#1
# asm 2: pxor  <r5=%xmm12,<z5=%xmm0
pxor  %xmm12,%xmm0

# qhasm: 						y14 = z6
# asm 1: movdqa <z6=int6464#6,>y14=int6464#3
# asm 2: movdqa <z6=%xmm5,>y14=%xmm2
movdqa %xmm5,%xmm2

# qhasm: uint32323232					y14 += z10
# asm 1: paddd <z10=int6464#2,<y14=int6464#3
# asm 2: paddd <z10=%xmm1,<y14=%xmm2
paddd %xmm1,%xmm2

# qhasm: 						r14 = y14
# asm 1: movdqa <y14=int6464#3,>r14=int6464#13
# asm 2: movdqa <y14=%xmm2,>r14=%xmm12
movdqa %xmm2,%xmm12

# qhasm: uint32323232					y14 <<= 7
# asm 1: pslld $7,<y14=int6464#3
# asm 2: pslld $7,<y14=%xmm2
pslld $7,%xmm2

# qhasm: 						z14 ^= y14
# asm 1: pxor  <y14=int6464#3,<z14=int6464#4
# asm 2: pxor  <y14=%xmm2,<z14=%xmm3
pxor  %xmm2,%xmm3

# qhasm: uint32323232					r14 >>= 25
# asm 1: psrld $25,<r14=int6464#13
# asm 2: psrld $25,<r14=%xmm12
psrld $25,%xmm12

# qhasm: 						z14 ^= r14
# asm 1: pxor  <r14=int6464#13,<z14=int6464#4
# asm 2: pxor  <r14=%xmm12,<z14=%xmm3
pxor  %xmm12,%xmm3

# qhasm: 								z15 = z15_stack
# asm 1: movdqa <z15_stack=stack128#18,>z15=int6464#3
# asm 2: movdqa <z15_stack=272(%rsp),>z15=%xmm2
movdqa 272(%rsp),%xmm2

# qhasm: 				z5_stack = z5
# asm 1: movdqa <z5=int6464#1,>z5_stack=stack128#18
# asm 2: movdqa <z5=%xmm0,>z5_stack=272(%rsp)
movdqa %xmm0,272(%rsp)

# qhasm: 								y3 = z11
# asm 1: movdqa <z11=int6464#7,>y3=int6464#1
# asm 2: movdqa <z11=%xmm6,>y3=%xmm0
movdqa %xmm6,%xmm0

# qhasm: uint32323232							y3 += z15
# asm 1: paddd <z15=int6464#3,<y3=int6464#1
# asm 2: paddd <z15=%xmm2,<y3=%xmm0
paddd %xmm2,%xmm0

# qhasm: 								r3 = y3
# asm 1: movdqa <y3=int6464#1,>r3=int6464#13
# asm 2: movdqa <y3=%xmm0,>r3=%xmm12
movdqa %xmm0,%xmm12

# qhasm: uint32323232							y3 <<= 7
# asm 1: pslld $7,<y3=int6464#1
# asm 2: pslld $7,<y3=%xmm0
pslld $7,%xmm0

# qhasm: 								z3 ^= y3
# asm 1: pxor  <y3=int6464#1,<z3=int6464#5
# asm 2: pxor  <y3=%xmm0,<z3=%xmm4
pxor  %xmm0,%xmm4

# qhasm: uint32323232							r3 >>= 25
# asm 1: psrld $25,<r3=int6464#13
# asm 2: psrld $25,<r3=%xmm12
psrld $25,%xmm12

# qhasm: 								z3 ^= r3
# asm 1: pxor  <r3=int6464#13,<z3=int6464#5
# asm 2: pxor  <r3=%xmm12,<z3=%xmm4
pxor  %xmm12,%xmm4

# qhasm: 						y2 = z10
# asm 1: movdqa <z10=int6464#2,>y2=int6464#1
# asm 2: movdqa <z10=%xmm1,>y2=%xmm0
movdqa %xmm1,%xmm0

# qhasm: uint32323232					y2 += z14
# asm 1: paddd <z14=int6464#4,<y2=int6464#1
# asm 2: paddd <z14=%xmm3,<y2=%xmm0
paddd %xmm3,%xmm0

# qhasm: 						r2 = y2
# asm 1: movdqa <y2=int6464#1,>r2=int6464#13
# asm 2: movdqa <y2=%xmm0,>r2=%xmm12
movdqa %xmm0,%xmm12

# qhasm: uint32323232					y2 <<= 9
# asm 1: pslld $9,<y2=int6464#1
# asm 2: pslld $9,<y2=%xmm0
pslld $9,%xmm0

# qhasm: 						z2 ^= y2
# asm 1: pxor  <y2=int6464#1,<z2=int6464#11
# asm 2: pxor  <y2=%xmm0,<z2=%xmm10
pxor  %xmm0,%xmm10

# qhasm: uint32323232					r2 >>= 23
# asm 1: psrld $23,<r2=int6464#13
# asm 2: psrld $23,<r2=%xmm12
psrld $23,%xmm12

# qhasm: 						z2 ^= r2
# asm 1: pxor  <r2=int6464#13,<z2=int6464#11
# asm 2: pxor  <r2=%xmm12,<z2=%xmm10
pxor  %xmm12,%xmm10

# qhasm: 								y7 = z15
# asm 1: movdqa <z15=int6464#3,>y7=int6464#1
# asm 2: movdqa <z15=%xmm2,>y7=%xmm0
movdqa %xmm2,%xmm0

# qhasm: uint32323232							y7 += z3
# asm 1: paddd <z3=int6464#5,<y7=int6464#1
# asm 2: paddd <z3=%xmm4,<y7=%xmm0
paddd %xmm4,%xmm0

# qhasm: 								r7 = y7
# asm 1: movdqa <y7=int6464#1,>r7=int6464#13
# asm 2: movdqa <y7=%xmm0,>r7=%xmm12
movdqa %xmm0,%xmm12

# qhasm: uint32323232							y7 <<= 9
# asm 1: pslld $9,<y7=int6464#1
# asm 2: pslld $9,<y7=%xmm0
pslld $9,%xmm0

# qhasm: 								z7 ^= y7
# asm 1: pxor  <y7=int6464#1,<z7=int6464#9
# asm 2: pxor  <y7=%xmm0,<z7=%xmm8
pxor  %xmm0,%xmm8

# qhasm: uint32323232							r7 >>= 23
# asm 1: psrld $23,<r7=int6464#13
# asm 2: psrld $23,<r7=%xmm12
psrld $23,%xmm12

# qhasm: 								z7 ^= r7
# asm 1: pxor  <r7=int6464#13,<z7=int6464#9
# asm 2: pxor  <r7=%xmm12,<z7=%xmm8
pxor  %xmm12,%xmm8

# qhasm: 						y6 = z14
# asm 1: movdqa <z14=int6464#4,>y6=int6464#1
# asm 2: movdqa <z14=%xmm3,>y6=%xmm0
movdqa %xmm3,%xmm0

# qhasm: uint32323232					y6 += z2
# asm 1: paddd <z2=int6464#11,<y6=int6464#1
# asm 2: paddd <z2=%xmm10,<y6=%xmm0
paddd %xmm10,%xmm0

# qhasm: 						r6 = y6
# asm 1: movdqa <y6=int6464#1,>r6=int6464#13
# asm 2: movdqa <y6=%xmm0,>r6=%xmm12
movdqa %xmm0,%xmm12

# qhasm: uint32323232					y6 <<= 13
# asm 1: pslld $13,<y6=int6464#1
# asm 2: pslld $13,<y6=%xmm0
pslld $13,%xmm0

# qhasm: 						z6 ^= y6
# asm 1: pxor  <y6=int6464#1,<z6=int6464#6
# asm 2: pxor  <y6=%xmm0,<z6=%xmm5
pxor  %xmm0,%xmm5

# qhasm: uint32323232					r6 >>= 19
# asm 1: psrld $19,<r6=int6464#13
# asm 2: psrld $19,<r6=%xmm12
psrld $19,%xmm12

# qhasm: 						z6 ^= r6
# asm 1: pxor  <r6=int6464#13,<z6=int6464#6
# asm 2: pxor  <r6=%xmm12,<z6=%xmm5
pxor  %xmm12,%xmm5

# qhasm: 								y11 = z3
# asm 1: movdqa <z3=int6464#5,>y11=int6464#1
# asm 2: movdqa <z3=%xmm4,>y11=%xmm0
movdqa %xmm4,%xmm0

# qhasm: uint32323232							y11 += z7
# asm 1: paddd <z7=int6464#9,<y11=int6464#1
# asm 2: paddd <z7=%xmm8,<y11=%xmm0
paddd %xmm8,%xmm0

# qhasm: 								r11 = y11
# asm 1: movdqa <y11=int6464#1,>r11=int6464#13
# asm 2: movdqa <y11=%xmm0,>r11=%xmm12
movdqa %xmm0,%xmm12

# qhasm: uint32323232							y11 <<= 13
# asm 1: pslld $13,<y11=int6464#1
# asm 2: pslld $13,<y11=%xmm0
pslld $13,%xmm0

# qhasm: 								z11 ^= y11
# asm 1: pxor  <y11=int6464#1,<z11=int6464#7
# asm 2: pxor  <y11=%xmm0,<z11=%xmm6
pxor  %xmm0,%xmm6

# qhasm: uint32323232							r11 >>= 19
# asm 1: psrld $19,<r11=int6464#13
# asm 2: psrld $19,<r11=%xmm12
psrld $19,%xmm12

# qhasm: 								z11 ^= r11
# asm 1: pxor  <r11=int6464#13,<z11=int6464#7
# asm 2: pxor  <r11=%xmm12,<z11=%xmm6
pxor  %xmm12,%xmm6

# qhasm: 						y10 = z2
# asm 1: movdqa <z2=int6464#11,>y10=int6464#1
# asm 2: movdqa <z2=%xmm10,>y10=%xmm0
movdqa %xmm10,%xmm0

# qhasm: uint32323232					y10 += z6
# asm 1: paddd <z6=int6464#6,<y10=int6464#1
# asm 2: paddd <z6=%xmm5,<y10=%xmm0
paddd %xmm5,%xmm0

# qhasm: 						r10 = y10
# asm 1: movdqa <y10=int6464#1,>r10=int6464#13
# asm 2: movdqa <y10=%xmm0,>r10=%xmm12
movdqa %xmm0,%xmm12

# qhasm: uint32323232					y10 <<= 18
# asm 1: pslld $18,<y10=int6464#1
# asm 2: pslld $18,<y10=%xmm0
pslld $18,%xmm0

# qhasm: 						z10 ^= y10
# asm 1: pxor  <y10=int6464#1,<z10=int6464#2
# asm 2: pxor  <y10=%xmm0,<z10=%xmm1
pxor  %xmm0,%xmm1

# qhasm: uint32323232					r10 >>= 14
# asm 1: psrld $14,<r10=int6464#13
# asm 2: psrld $14,<r10=%xmm12
psrld $14,%xmm12

# qhasm: 						z10 ^= r10
# asm 1: pxor  <r10=int6464#13,<z10=int6464#2
# asm 2: pxor  <r10=%xmm12,<z10=%xmm1
pxor  %xmm12,%xmm1

# qhasm: 		z0 = z0_stack
# asm 1: movdqa <z0_stack=stack128#17,>z0=int6464#1
# asm 2: movdqa <z0_stack=256(%rsp),>z0=%xmm0
movdqa 256(%rsp),%xmm0

# qhasm: 						z10_stack = z10
# asm 1: movdqa <z10=int6464#2,>z10_stack=stack128#17
# asm 2: movdqa <z10=%xmm1,>z10_stack=256(%rsp)
movdqa %xmm1,256(%rsp)

# qhasm: 		y1 = z3
# asm 1: movdqa <z3=int6464#5,>y1=int6464#2
# asm 2: movdqa <z3=%xmm4,>y1=%xmm1
movdqa %xmm4,%xmm1

# qhasm: uint32323232	y1 += z0
# asm 1: paddd <z0=int6464#1,<y1=int6464#2
# asm 2: paddd <z0=%xmm0,<y1=%xmm1
paddd %xmm0,%xmm1

# qhasm: 		r1 = y1
# asm 1: movdqa <y1=int6464#2,>r1=int6464#13
# asm 2: movdqa <y1=%xmm1,>r1=%xmm12
movdqa %xmm1,%xmm12

# qhasm: uint32323232	y1 <<= 7
# asm 1: pslld $7,<y1=int6464#2
# asm 2: pslld $7,<y1=%xmm1
pslld $7,%xmm1

# qhasm: 		z1 ^= y1
# asm 1: pxor  <y1=int6464#2,<z1=int6464#8
# asm 2: pxor  <y1=%xmm1,<z1=%xmm7
pxor  %xmm1,%xmm7

# qhasm: uint32323232	r1 >>= 25
# asm 1: psrld $25,<r1=int6464#13
# asm 2: psrld $25,<r1=%xmm12
psrld $25,%xmm12

# qhasm: 		z1 ^= r1
# asm 1: pxor  <r1=int6464#13,<z1=int6464#8
# asm 2: pxor  <r1=%xmm12,<z1=%xmm7
pxor  %xmm12,%xmm7

# qhasm: 								y15 = z7
# asm 1: movdqa <z7=int6464#9,>y15=int6464#2
# asm 2: movdqa <z7=%xmm8,>y15=%xmm1
movdqa %xmm8,%xmm1

# qhasm: uint32323232							y15 += z11
# asm 1: paddd <z11=int6464#7,<y15=int6464#2
# asm 2: paddd <z11=%xmm6,<y15=%xmm1
paddd %xmm6,%xmm1

# qhasm: 								r15 = y15
# asm 1: movdqa <y15=int6464#2,>r15=int6464#13
# asm 2: movdqa <y15=%xmm1,>r15=%xmm12
movdqa %xmm1,%xmm12

# qhasm: uint32323232							y15 <<= 18
# asm 1: pslld $18,<y15=int6464#2
# asm 2: pslld $18,<y15=%xmm1
pslld $18,%xmm1

# qhasm: 								z15 ^= y15
# asm 1: pxor  <y15=int6464#2,<z15=int6464#3
# asm 2: pxor  <y15=%xmm1,<z15=%xmm2
pxor  %xmm1,%xmm2

# qhasm: uint32323232							r15 >>= 14
# asm 1: psrld $14,<r15=int6464#13
# asm 2: psrld $14,<r15=%xmm12
psrld $14,%xmm12

# qhasm: 								z15 ^= r15
# asm 1: pxor  <r15=int6464#13,<z15=int6464#3
# asm 2: pxor  <r15=%xmm12,<z15=%xmm2
pxor  %xmm12,%xmm2

# qhasm: 				z5 = z5_stack
# asm 1: movdqa <z5_stack=stack128#18,>z5=int6464#13
# asm 2: movdqa <z5_stack=272(%rsp),>z5=%xmm12
movdqa 272(%rsp),%xmm12

# qhasm: 								z15_stack = z15
# asm 1: movdqa <z15=int6464#3,>z15_stack=stack128#18
# asm 2: movdqa <z15=%xmm2,>z15_stack=272(%rsp)
movdqa %xmm2,272(%rsp)

# qhasm: 				y6 = z4
# asm 1: movdqa <z4=int6464#15,>y6=int6464#2
# asm 2: movdqa <z4=%xmm14,>y6=%xmm1
movdqa %xmm14,%xmm1

# qhasm: uint32323232			y6 += z5
# asm 1: paddd <z5=int6464#13,<y6=int6464#2
# asm 2: paddd <z5=%xmm12,<y6=%xmm1
paddd %xmm12,%xmm1

# qhasm: 				r6 = y6
# asm 1: movdqa <y6=int6464#2,>r6=int6464#3
# asm 2: movdqa <y6=%xmm1,>r6=%xmm2
movdqa %xmm1,%xmm2

# qhasm: uint32323232			y6 <<= 7
# asm 1: pslld $7,<y6=int6464#2
# asm 2: pslld $7,<y6=%xmm1
pslld $7,%xmm1

# qhasm: 				z6 ^= y6
# asm 1: pxor  <y6=int6464#2,<z6=int6464#6
# asm 2: pxor  <y6=%xmm1,<z6=%xmm5
pxor  %xmm1,%xmm5

# qhasm: uint32323232			r6 >>= 25
# asm 1: psrld $25,<r6=int6464#3
# asm 2: psrld $25,<r6=%xmm2
psrld $25,%xmm2

# qhasm: 				z6 ^= r6
# asm 1: pxor  <r6=int6464#3,<z6=int6464#6
# asm 2: pxor  <r6=%xmm2,<z6=%xmm5
pxor  %xmm2,%xmm5

# qhasm: 		y2 = z0
# asm 1: movdqa <z0=int6464#1,>y2=int6464#2
# asm 2: movdqa <z0=%xmm0,>y2=%xmm1
movdqa %xmm0,%xmm1

# qhasm: uint32323232	y2 += z1
# asm 1: paddd <z1=int6464#8,<y2=int6464#2
# asm 2: paddd <z1=%xmm7,<y2=%xmm1
paddd %xmm7,%xmm1

# qhasm: 		r2 = y2
# asm 1: movdqa <y2=int6464#2,>r2=int6464#3
# asm 2: movdqa <y2=%xmm1,>r2=%xmm2
movdqa %xmm1,%xmm2

# qhasm: uint32323232	y2 <<= 9
# asm 1: pslld $9,<y2=int6464#2
# asm 2: pslld $9,<y2=%xmm1
pslld $9,%xmm1

# qhasm: 		z2 ^= y2
# asm 1: pxor  <y2=int6464#2,<z2=int6464#11
# asm 2: pxor  <y2=%xmm1,<z2=%xmm10
pxor  %xmm1,%xmm10

# qhasm: uint32323232	r2 >>= 23
# asm 1: psrld $23,<r2=int6464#3
# asm 2: psrld $23,<r2=%xmm2
psrld $23,%xmm2

# qhasm: 		z2 ^= r2
# asm 1: pxor  <r2=int6464#3,<z2=int6464#11
# asm 2: pxor  <r2=%xmm2,<z2=%xmm10
pxor  %xmm2,%xmm10

# qhasm: 				y7 = z5
# asm 1: movdqa <z5=int6464#13,>y7=int6464#2
# asm 2: movdqa <z5=%xmm12,>y7=%xmm1
movdqa %xmm12,%xmm1

# qhasm: uint32323232			y7 += z6
# asm 1: paddd <z6=int6464#6,<y7=int6464#2
# asm 2: paddd <z6=%xmm5,<y7=%xmm1
paddd %xmm5,%xmm1

# qhasm: 				r7 = y7
# asm 1: movdqa <y7=int6464#2,>r7=int6464#3
# asm 2: movdqa <y7=%xmm1,>r7=%xmm2
movdqa %xmm1,%xmm2

# qhasm: uint32323232			y7 <<= 9
# asm 1: pslld $9,<y7=int6464#2
# asm 2: pslld $9,<y7=%xmm1
pslld $9,%xmm1

# qhasm: 				z7 ^= y7
# asm 1: pxor  <y7=int6464#2,<z7=int6464#9
# asm 2: pxor  <y7=%xmm1,<z7=%xmm8
pxor  %xmm1,%xmm8

# qhasm: uint32323232			r7 >>= 23
# asm 1: psrld $23,<r7=int6464#3
# asm 2: psrld $23,<r7=%xmm2
psrld $23,%xmm2

# qhasm: 				z7 ^= r7
# asm 1: pxor  <r7=int6464#3,<z7=int6464#9
# asm 2: pxor  <r7=%xmm2,<z7=%xmm8
pxor  %xmm2,%xmm8

# qhasm: 		y3 = z1
# asm 1: movdqa <z1=int6464#8,>y3=int6464#2
# asm 2: movdqa <z1=%xmm7,>y3=%xmm1
movdqa %xmm7,%xmm1

# qhasm: uint32323232	y3 += z2
# asm 1: paddd <z2=int6464#11,<y3=int6464#2
# asm 2: paddd <z2=%xmm10,<y3=%xmm1
paddd %xmm10,%xmm1

# qhasm: 		r3 = y3
# asm 1: movdqa <y3=int6464#2,>r3=int6464#3
# asm 2: movdqa <y3=%xmm1,>r3=%xmm2
movdqa %xmm1,%xmm2

# qhasm: uint32323232	y3 <<= 13
# asm 1: pslld $13,<y3=int6464#2
# asm 2: pslld $13,<y3=%xmm1
pslld $13,%xmm1

# qhasm: 		z3 ^= y3
# asm 1: pxor  <y3=int6464#2,<z3=int6464#5
# asm 2: pxor  <y3=%xmm1,<z3=%xmm4
pxor  %xmm1,%xmm4

# qhasm: uint32323232	r3 >>= 19
# asm 1: psrld $19,<r3=int6464#3
# asm 2: psrld $19,<r3=%xmm2
psrld $19,%xmm2

# qhasm: 		z3 ^= r3
# asm 1: pxor  <r3=int6464#3,<z3=int6464#5
# asm 2: pxor  <r3=%xmm2,<z3=%xmm4
pxor  %xmm2,%xmm4

# qhasm: 				y4 = z6
# asm 1: movdqa <z6=int6464#6,>y4=int6464#2
# asm 2: movdqa <z6=%xmm5,>y4=%xmm1
movdqa %xmm5,%xmm1

# qhasm: uint32323232			y4 += z7
# asm 1: paddd <z7=int6464#9,<y4=int6464#2
# asm 2: paddd <z7=%xmm8,<y4=%xmm1
paddd %xmm8,%xmm1

# qhasm: 				r4 = y4
# asm 1: movdqa <y4=int6464#2,>r4=int6464#3
# asm 2: movdqa <y4=%xmm1,>r4=%xmm2
movdqa %xmm1,%xmm2

# qhasm: uint32323232			y4 <<= 13
# asm 1: pslld $13,<y4=int6464#2
# asm 2: pslld $13,<y4=%xmm1
pslld $13,%xmm1

# qhasm: 				z4 ^= y4
# asm 1: pxor  <y4=int6464#2,<z4=int6464#15
# asm 2: pxor  <y4=%xmm1,<z4=%xmm14
pxor  %xmm1,%xmm14

# qhasm: uint32323232			r4 >>= 19
# asm 1: psrld $19,<r4=int6464#3
# asm 2: psrld $19,<r4=%xmm2
psrld $19,%xmm2

# qhasm: 				z4 ^= r4
# asm 1: pxor  <r4=int6464#3,<z4=int6464#15
# asm 2: pxor  <r4=%xmm2,<z4=%xmm14
pxor  %xmm2,%xmm14

# qhasm: 		y0 = z2
# asm 1: movdqa <z2=int6464#11,>y0=int6464#2
# asm 2: movdqa <z2=%xmm10,>y0=%xmm1
movdqa %xmm10,%xmm1

# qhasm: uint32323232	y0 += z3
# asm 1: paddd <z3=int6464#5,<y0=int6464#2
# asm 2: paddd <z3=%xmm4,<y0=%xmm1
paddd %xmm4,%xmm1

# qhasm: 		r0 = y0
# asm 1: movdqa <y0=int6464#2,>r0=int6464#3
# asm 2: movdqa <y0=%xmm1,>r0=%xmm2
movdqa %xmm1,%xmm2

# qhasm: uint32323232	y0 <<= 18
# asm 1: pslld $18,<y0=int6464#2
# asm 2: pslld $18,<y0=%xmm1
pslld $18,%xmm1

# qhasm: 		z0 ^= y0
# asm 1: pxor  <y0=int6464#2,<z0=int6464#1
# asm 2: pxor  <y0=%xmm1,<z0=%xmm0
pxor  %xmm1,%xmm0

# qhasm: uint32323232	r0 >>= 14
# asm 1: psrld $14,<r0=int6464#3
# asm 2: psrld $14,<r0=%xmm2
psrld $14,%xmm2

# qhasm: 		z0 ^= r0
# asm 1: pxor  <r0=int6464#3,<z0=int6464#1
# asm 2: pxor  <r0=%xmm2,<z0=%xmm0
pxor  %xmm2,%xmm0

# qhasm: 						z10 = z10_stack
# asm 1: movdqa <z10_stack=stack128#17,>z10=int6464#2
# asm 2: movdqa <z10_stack=256(%rsp),>z10=%xmm1
movdqa 256(%rsp),%xmm1

# qhasm: 		z0_stack = z0
# asm 1: movdqa <z0=int6464#1,>z0_stack=stack128#17
# asm 2: movdqa <z0=%xmm0,>z0_stack=256(%rsp)
movdqa %xmm0,256(%rsp)

# qhasm: 				y5 = z7
# asm 1: movdqa <z7=int6464#9,>y5=int6464#1
# asm 2: movdqa <z7=%xmm8,>y5=%xmm0
movdqa %xmm8,%xmm0

# qhasm: uint32323232			y5 += z4
# asm 1: paddd <z4=int6464#15,<y5=int6464#1
# asm 2: paddd <z4=%xmm14,<y5=%xmm0
paddd %xmm14,%xmm0

# qhasm: 				r5 = y5
# asm 1: movdqa <y5=int6464#1,>r5=int6464#3
# asm 2: movdqa <y5=%xmm0,>r5=%xmm2
movdqa %xmm0,%xmm2

# qhasm: uint32323232			y5 <<= 18
# asm 1: pslld $18,<y5=int6464#1
# asm 2: pslld $18,<y5=%xmm0
pslld $18,%xmm0

# qhasm: 				z5 ^= y5
# asm 1: pxor  <y5=int6464#1,<z5=int6464#13
# asm 2: pxor  <y5=%xmm0,<z5=%xmm12
pxor  %xmm0,%xmm12

# qhasm: uint32323232			r5 >>= 14
# asm 1: psrld $14,<r5=int6464#3
# asm 2: psrld $14,<r5=%xmm2
psrld $14,%xmm2

# qhasm: 				z5 ^= r5
# asm 1: pxor  <r5=int6464#3,<z5=int6464#13
# asm 2: pxor  <r5=%xmm2,<z5=%xmm12
pxor  %xmm2,%xmm12

# qhasm: 						y11 = z9
# asm 1: movdqa <z9=int6464#12,>y11=int6464#1
# asm 2: movdqa <z9=%xmm11,>y11=%xmm0
movdqa %xmm11,%xmm0

# qhasm: uint32323232					y11 += z10
# asm 1: paddd <z10=int6464#2,<y11=int6464#1
# asm 2: paddd <z10=%xmm1,<y11=%xmm0
paddd %xmm1,%xmm0

# qhasm: 						r11 = y11
# asm 1: movdqa <y11=int6464#1,>r11=int6464#3
# asm 2: movdqa <y11=%xmm0,>r11=%xmm2
movdqa %xmm0,%xmm2

# qhasm: uint32323232					y11 <<= 7
# asm 1: pslld $7,<y11=int6464#1
# asm 2: pslld $7,<y11=%xmm0
pslld $7,%xmm0

# qhasm: 						z11 ^= y11
# asm 1: pxor  <y11=int6464#1,<z11=int6464#7
# asm 2: pxor  <y11=%xmm0,<z11=%xmm6
pxor  %xmm0,%xmm6

# qhasm: uint32323232					r11 >>= 25
# asm 1: psrld $25,<r11=int6464#3
# asm 2: psrld $25,<r11=%xmm2
psrld $25,%xmm2

# qhasm: 						z11 ^= r11
# asm 1: pxor  <r11=int6464#3,<z11=int6464#7
# asm 2: pxor  <r11=%xmm2,<z11=%xmm6
pxor  %xmm2,%xmm6

# qhasm: 								z15 = z15_stack
# asm 1: movdqa <z15_stack=stack128#18,>z15=int6464#3
# asm 2: movdqa <z15_stack=272(%rsp),>z15=%xmm2
movdqa 272(%rsp),%xmm2

# qhasm: 				z5_stack = z5
# asm 1: movdqa <z5=int6464#13,>z5_stack=stack128#18
# asm 2: movdqa <z5=%xmm12,>z5_stack=272(%rsp)
movdqa %xmm12,272(%rsp)

# qhasm: 								y12 = z14
# asm 1: movdqa <z14=int6464#4,>y12=int6464#1
# asm 2: movdqa <z14=%xmm3,>y12=%xmm0
movdqa %xmm3,%xmm0

# qhasm: uint32323232							y12 += z15
# asm 1: paddd <z15=int6464#3,<y12=int6464#1
# asm 2: paddd <z15=%xmm2,<y12=%xmm0
paddd %xmm2,%xmm0

# qhasm: 								r12 = y12
# asm 1: movdqa <y12=int6464#1,>r12=int6464#13
# asm 2: movdqa <y12=%xmm0,>r12=%xmm12
movdqa %xmm0,%xmm12

# qhasm: uint32323232							y12 <<= 7
# asm 1: pslld $7,<y12=int6464#1
# asm 2: pslld $7,<y12=%xmm0
pslld $7,%xmm0

# qhasm: 								z12 ^= y12
# asm 1: pxor  <y12=int6464#1,<z12=int6464#14
# asm 2: pxor  <y12=%xmm0,<z12=%xmm13
pxor  %xmm0,%xmm13

# qhasm: uint32323232							r12 >>= 25
# asm 1: psrld $25,<r12=int6464#13
# asm 2: psrld $25,<r12=%xmm12
psrld $25,%xmm12

# qhasm: 								z12 ^= r12
# asm 1: pxor  <r12=int6464#13,<z12=int6464#14
# asm 2: pxor  <r12=%xmm12,<z12=%xmm13
pxor  %xmm12,%xmm13

# qhasm: 						y8 = z10
# asm 1: movdqa <z10=int6464#2,>y8=int6464#1
# asm 2: movdqa <z10=%xmm1,>y8=%xmm0
movdqa %xmm1,%xmm0

# qhasm: uint32323232					y8 += z11
# asm 1: paddd <z11=int6464#7,<y8=int6464#1
# asm 2: paddd <z11=%xmm6,<y8=%xmm0
paddd %xmm6,%xmm0

# qhasm: 						r8 = y8
# asm 1: movdqa <y8=int6464#1,>r8=int6464#13
# asm 2: movdqa <y8=%xmm0,>r8=%xmm12
movdqa %xmm0,%xmm12

# qhasm: uint32323232					y8 <<= 9
# asm 1: pslld $9,<y8=int6464#1
# asm 2: pslld $9,<y8=%xmm0
pslld $9,%xmm0

# qhasm: 						z8 ^= y8
# asm 1: pxor  <y8=int6464#1,<z8=int6464#16
# asm 2: pxor  <y8=%xmm0,<z8=%xmm15
pxor  %xmm0,%xmm15

# qhasm: uint32323232					r8 >>= 23
# asm 1: psrld $23,<r8=int6464#13
# asm 2: psrld $23,<r8=%xmm12
psrld $23,%xmm12

# qhasm: 						z8 ^= r8
# asm 1: pxor  <r8=int6464#13,<z8=int6464#16
# asm 2: pxor  <r8=%xmm12,<z8=%xmm15
pxor  %xmm12,%xmm15

# qhasm: 								y13 = z15
# asm 1: movdqa <z15=int6464#3,>y13=int6464#1
# asm 2: movdqa <z15=%xmm2,>y13=%xmm0
movdqa %xmm2,%xmm0

# qhasm: uint32323232							y13 += z12
# asm 1: paddd <z12=int6464#14,<y13=int6464#1
# asm 2: paddd <z12=%xmm13,<y13=%xmm0
paddd %xmm13,%xmm0

# qhasm: 								r13 = y13
# asm 1: movdqa <y13=int6464#1,>r13=int6464#13
# asm 2: movdqa <y13=%xmm0,>r13=%xmm12
movdqa %xmm0,%xmm12

# qhasm: uint32323232							y13 <<= 9
# asm 1: pslld $9,<y13=int6464#1
# asm 2: pslld $9,<y13=%xmm0
pslld $9,%xmm0

# qhasm: 								z13 ^= y13
# asm 1: pxor  <y13=int6464#1,<z13=int6464#10
# asm 2: pxor  <y13=%xmm0,<z13=%xmm9
pxor  %xmm0,%xmm9

# qhasm: uint32323232							r13 >>= 23
# asm 1: psrld $23,<r13=int6464#13
# asm 2: psrld $23,<r13=%xmm12
psrld $23,%xmm12

# qhasm: 								z13 ^= r13
# asm 1: pxor  <r13=int6464#13,<z13=int6464#10
# asm 2: pxor  <r13=%xmm12,<z13=%xmm9
pxor  %xmm12,%xmm9

# qhasm: 						y9 = z11
# asm 1: movdqa <z11=int6464#7,>y9=int6464#1
# asm 2: movdqa <z11=%xmm6,>y9=%xmm0
movdqa %xmm6,%xmm0

# qhasm: uint32323232					y9 += z8
# asm 1: paddd <z8=int6464#16,<y9=int6464#1
# asm 2: paddd <z8=%xmm15,<y9=%xmm0
paddd %xmm15,%xmm0

# qhasm: 						r9 = y9
# asm 1: movdqa <y9=int6464#1,>r9=int6464#13
# asm 2: movdqa <y9=%xmm0,>r9=%xmm12
movdqa %xmm0,%xmm12

# qhasm: uint32323232					y9 <<= 13
# asm 1: pslld $13,<y9=int6464#1
# asm 2: pslld $13,<y9=%xmm0
pslld $13,%xmm0

# qhasm: 						z9 ^= y9
# asm 1: pxor  <y9=int6464#1,<z9=int6464#12
# asm 2: pxor  <y9=%xmm0,<z9=%xmm11
pxor  %xmm0,%xmm11

# qhasm: uint32323232					r9 >>= 19
# asm 1: psrld $19,<r9=int6464#13
# asm 2: psrld $19,<r9=%xmm12
psrld $19,%xmm12

# qhasm: 						z9 ^= r9
# asm 1: pxor  <r9=int6464#13,<z9=int6464#12
# asm 2: pxor  <r9=%xmm12,<z9=%xmm11
pxor  %xmm12,%xmm11

# qhasm: 								y14 = z12
# asm 1: movdqa <z12=int6464#14,>y14=int6464#1
# asm 2: movdqa <z12=%xmm13,>y14=%xmm0
movdqa %xmm13,%xmm0

# qhasm: uint32323232							y14 += z13
# asm 1: paddd <z13=int6464#10,<y14=int6464#1
# asm 2: paddd <z13=%xmm9,<y14=%xmm0
paddd %xmm9,%xmm0

# qhasm: 								r14 = y14
# asm 1: movdqa <y14=int6464#1,>r14=int6464#13
# asm 2: movdqa <y14=%xmm0,>r14=%xmm12
movdqa %xmm0,%xmm12

# qhasm: uint32323232							y14 <<= 13
# asm 1: pslld $13,<y14=int6464#1
# asm 2: pslld $13,<y14=%xmm0
pslld $13,%xmm0

# qhasm: 								z14 ^= y14
# asm 1: pxor  <y14=int6464#1,<z14=int6464#4
# asm 2: pxor  <y14=%xmm0,<z14=%xmm3
pxor  %xmm0,%xmm3

# qhasm: uint32323232							r14 >>= 19
# asm 1: psrld $19,<r14=int6464#13
# asm 2: psrld $19,<r14=%xmm12
psrld $19,%xmm12

# qhasm: 								z14 ^= r14
# asm 1: pxor  <r14=int6464#13,<z14=int6464#4
# asm 2: pxor  <r14=%xmm12,<z14=%xmm3
pxor  %xmm12,%xmm3

# qhasm: 						y10 = z8
# asm 1: movdqa <z8=int6464#16,>y10=int6464#1
# asm 2: movdqa <z8=%xmm15,>y10=%xmm0
movdqa %xmm15,%xmm0

# qhasm: uint32323232					y10 += z9
# asm 1: paddd <z9=int6464#12,<y10=int6464#1
# asm 2: paddd <z9=%xmm11,<y10=%xmm0
paddd %xmm11,%xmm0

# qhasm: 						r10 = y10
# asm 1: movdqa <y10=int6464#1,>r10=int6464#13
# asm 2: movdqa <y10=%xmm0,>r10=%xmm12
movdqa %xmm0,%xmm12

# qhasm: uint32323232					y10 <<= 18
# asm 1: pslld $18,<y10=int6464#1
# asm 2: pslld $18,<y10=%xmm0
pslld $18,%xmm0

# qhasm: 						z10 ^= y10
# asm 1: pxor  <y10=int6464#1,<z10=int6464#2
# asm 2: pxor  <y10=%xmm0,<z10=%xmm1
pxor  %xmm0,%xmm1

# qhasm: uint32323232					r10 >>= 14
# asm 1: psrld $14,<r10=int6464#13
# asm 2: psrld $14,<r10=%xmm12
psrld $14,%xmm12

# qhasm: 						z10 ^= r10
# asm 1: pxor  <r10=int6464#13,<z10=int6464#2
# asm 2: pxor  <r10=%xmm12,<z10=%xmm1
pxor  %xmm12,%xmm1

# qhasm: 								y15 = z13
# asm 1: movdqa <z13=int6464#10,>y15=int6464#1
# asm 2: movdqa <z13=%xmm9,>y15=%xmm0
movdqa %xmm9,%xmm0

# qhasm: uint32323232							y15 += z14
# asm 1: paddd <z14=int6464#4,<y15=int6464#1
# asm 2: paddd <z14=%xmm3,<y15=%xmm0
paddd %xmm3,%xmm0

# qhasm: 								r15 = y15
# asm 1: movdqa <y15=int6464#1,>r15=int6464#13
# asm 2: movdqa <y15=%xmm0,>r15=%xmm12
movdqa %xmm0,%xmm12

# qhasm: uint32323232							y15 <<= 18
# asm 1: pslld $18,<y15=int6464#1
# asm 2: pslld $18,<y15=%xmm0
pslld $18,%xmm0

# qhasm: 								z15 ^= y15
# asm 1: pxor  <y15=int6464#1,<z15=int6464#3
# asm 2: pxor  <y15=%xmm0,<z15=%xmm2
pxor  %xmm0,%xmm2

# qhasm: uint32323232							r15 >>= 14
# asm 1: psrld $14,<r15=int6464#13
# asm 2: psrld $14,<r15=%xmm12
psrld $14,%xmm12

# qhasm: 								z15 ^= r15
# asm 1: pxor  <r15=int6464#13,<z15=int6464#3
# asm 2: pxor  <r15=%xmm12,<z15=%xmm2
pxor  %xmm12,%xmm2

# qhasm: 		z0 = z0_stack
# asm 1: movdqa <z0_stack=stack128#17,>z0=int6464#13
# asm 2: movdqa <z0_stack=256(%rsp),>z0=%xmm12
movdqa 256(%rsp),%xmm12

# qhasm: 				z5 = z5_stack
# asm 1: movdqa <z5_stack=stack128#18,>z5=int6464#1
# asm 2: movdqa <z5_stack=272(%rsp),>z5=%xmm0
movdqa 272(%rsp),%xmm0

# qhasm:                   unsigned>? i -= 2
# asm 1: sub  $2,<i=int64#3
# asm 2: sub  $2,<i=%rdx
sub  $2,%rdx
# comment:fp stack unchanged by jump

# qhasm: goto mainloop1 if unsigned>
ja ._mainloop1

# qhasm:   uint32323232 z0 += orig0
# asm 1: paddd <orig0=stack128#4,<z0=int6464#13
# asm 2: paddd <orig0=48(%rsp),<z0=%xmm12
paddd 48(%rsp),%xmm12

# qhasm:   uint32323232 z1 += orig1
# asm 1: paddd <orig1=stack128#8,<z1=int6464#8
# asm 2: paddd <orig1=112(%rsp),<z1=%xmm7
paddd 112(%rsp),%xmm7

# qhasm:   uint32323232 z2 += orig2
# asm 1: paddd <orig2=stack128#11,<z2=int6464#11
# asm 2: paddd <orig2=160(%rsp),<z2=%xmm10
paddd 160(%rsp),%xmm10

# qhasm:   uint32323232 z3 += orig3
# asm 1: paddd <orig3=stack128#14,<z3=int6464#5
# asm 2: paddd <orig3=208(%rsp),<z3=%xmm4
paddd 208(%rsp),%xmm4

# qhasm:   in0 = z0
# asm 1: movd   <z0=int6464#13,>in0=int64#3
# asm 2: movd   <z0=%xmm12,>in0=%rdx
movd   %xmm12,%rdx

# qhasm:   in1 = z1
# asm 1: movd   <z1=int6464#8,>in1=int64#4
# asm 2: movd   <z1=%xmm7,>in1=%rcx
movd   %xmm7,%rcx

# qhasm:   in2 = z2
# asm 1: movd   <z2=int6464#11,>in2=int64#6
# asm 2: movd   <z2=%xmm10,>in2=%r9
movd   %xmm10,%r9

# qhasm:   in3 = z3
# asm 1: movd   <z3=int6464#5,>in3=int64#7
# asm 2: movd   <z3=%xmm4,>in3=%rax
movd   %xmm4,%rax

# qhasm:   z0 <<<= 96
# asm 1: pshufd $0x39,<z0=int6464#13,<z0=int6464#13
# asm 2: pshufd $0x39,<z0=%xmm12,<z0=%xmm12
pshufd $0x39,%xmm12,%xmm12

# qhasm:   z1 <<<= 96
# asm 1: pshufd $0x39,<z1=int6464#8,<z1=int6464#8
# asm 2: pshufd $0x39,<z1=%xmm7,<z1=%xmm7
pshufd $0x39,%xmm7,%xmm7

# qhasm:   z2 <<<= 96
# asm 1: pshufd $0x39,<z2=int6464#11,<z2=int6464#11
# asm 2: pshufd $0x39,<z2=%xmm10,<z2=%xmm10
pshufd $0x39,%xmm10,%xmm10

# qhasm:   z3 <<<= 96
# asm 1: pshufd $0x39,<z3=int6464#5,<z3=int6464#5
# asm 2: pshufd $0x39,<z3=%xmm4,<z3=%xmm4
pshufd $0x39,%xmm4,%xmm4

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
# asm 1: movd   <z0=int6464#13,>in0=int64#3
# asm 2: movd   <z0=%xmm12,>in0=%rdx
movd   %xmm12,%rdx

# qhasm:   in1 = z1
# asm 1: movd   <z1=int6464#8,>in1=int64#4
# asm 2: movd   <z1=%xmm7,>in1=%rcx
movd   %xmm7,%rcx

# qhasm:   in2 = z2
# asm 1: movd   <z2=int6464#11,>in2=int64#6
# asm 2: movd   <z2=%xmm10,>in2=%r9
movd   %xmm10,%r9

# qhasm:   in3 = z3
# asm 1: movd   <z3=int6464#5,>in3=int64#7
# asm 2: movd   <z3=%xmm4,>in3=%rax
movd   %xmm4,%rax

# qhasm:   z0 <<<= 96
# asm 1: pshufd $0x39,<z0=int6464#13,<z0=int6464#13
# asm 2: pshufd $0x39,<z0=%xmm12,<z0=%xmm12
pshufd $0x39,%xmm12,%xmm12

# qhasm:   z1 <<<= 96
# asm 1: pshufd $0x39,<z1=int6464#8,<z1=int6464#8
# asm 2: pshufd $0x39,<z1=%xmm7,<z1=%xmm7
pshufd $0x39,%xmm7,%xmm7

# qhasm:   z2 <<<= 96
# asm 1: pshufd $0x39,<z2=int6464#11,<z2=int6464#11
# asm 2: pshufd $0x39,<z2=%xmm10,<z2=%xmm10
pshufd $0x39,%xmm10,%xmm10

# qhasm:   z3 <<<= 96
# asm 1: pshufd $0x39,<z3=int6464#5,<z3=int6464#5
# asm 2: pshufd $0x39,<z3=%xmm4,<z3=%xmm4
pshufd $0x39,%xmm4,%xmm4

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
# asm 1: movd   <z0=int6464#13,>in0=int64#3
# asm 2: movd   <z0=%xmm12,>in0=%rdx
movd   %xmm12,%rdx

# qhasm:   in1 = z1
# asm 1: movd   <z1=int6464#8,>in1=int64#4
# asm 2: movd   <z1=%xmm7,>in1=%rcx
movd   %xmm7,%rcx

# qhasm:   in2 = z2
# asm 1: movd   <z2=int6464#11,>in2=int64#6
# asm 2: movd   <z2=%xmm10,>in2=%r9
movd   %xmm10,%r9

# qhasm:   in3 = z3
# asm 1: movd   <z3=int6464#5,>in3=int64#7
# asm 2: movd   <z3=%xmm4,>in3=%rax
movd   %xmm4,%rax

# qhasm:   z0 <<<= 96
# asm 1: pshufd $0x39,<z0=int6464#13,<z0=int6464#13
# asm 2: pshufd $0x39,<z0=%xmm12,<z0=%xmm12
pshufd $0x39,%xmm12,%xmm12

# qhasm:   z1 <<<= 96
# asm 1: pshufd $0x39,<z1=int6464#8,<z1=int6464#8
# asm 2: pshufd $0x39,<z1=%xmm7,<z1=%xmm7
pshufd $0x39,%xmm7,%xmm7

# qhasm:   z2 <<<= 96
# asm 1: pshufd $0x39,<z2=int6464#11,<z2=int6464#11
# asm 2: pshufd $0x39,<z2=%xmm10,<z2=%xmm10
pshufd $0x39,%xmm10,%xmm10

# qhasm:   z3 <<<= 96
# asm 1: pshufd $0x39,<z3=int6464#5,<z3=int6464#5
# asm 2: pshufd $0x39,<z3=%xmm4,<z3=%xmm4
pshufd $0x39,%xmm4,%xmm4

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
# asm 1: movd   <z0=int6464#13,>in0=int64#3
# asm 2: movd   <z0=%xmm12,>in0=%rdx
movd   %xmm12,%rdx

# qhasm:   in1 = z1
# asm 1: movd   <z1=int6464#8,>in1=int64#4
# asm 2: movd   <z1=%xmm7,>in1=%rcx
movd   %xmm7,%rcx

# qhasm:   in2 = z2
# asm 1: movd   <z2=int6464#11,>in2=int64#6
# asm 2: movd   <z2=%xmm10,>in2=%r9
movd   %xmm10,%r9

# qhasm:   in3 = z3
# asm 1: movd   <z3=int6464#5,>in3=int64#7
# asm 2: movd   <z3=%xmm4,>in3=%rax
movd   %xmm4,%rax

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

# qhasm:   uint32323232 z4 += orig4
# asm 1: paddd <orig4=stack128#12,<z4=int6464#15
# asm 2: paddd <orig4=176(%rsp),<z4=%xmm14
paddd 176(%rsp),%xmm14

# qhasm:   uint32323232 z5 += orig5
# asm 1: paddd <orig5=stack128#1,<z5=int6464#1
# asm 2: paddd <orig5=0(%rsp),<z5=%xmm0
paddd 0(%rsp),%xmm0

# qhasm:   uint32323232 z6 += orig6
# asm 1: paddd <orig6=stack128#5,<z6=int6464#6
# asm 2: paddd <orig6=64(%rsp),<z6=%xmm5
paddd 64(%rsp),%xmm5

# qhasm:   uint32323232 z7 += orig7
# asm 1: paddd <orig7=stack128#9,<z7=int6464#9
# asm 2: paddd <orig7=128(%rsp),<z7=%xmm8
paddd 128(%rsp),%xmm8

# qhasm:   in4 = z4
# asm 1: movd   <z4=int6464#15,>in4=int64#3
# asm 2: movd   <z4=%xmm14,>in4=%rdx
movd   %xmm14,%rdx

# qhasm:   in5 = z5
# asm 1: movd   <z5=int6464#1,>in5=int64#4
# asm 2: movd   <z5=%xmm0,>in5=%rcx
movd   %xmm0,%rcx

# qhasm:   in6 = z6
# asm 1: movd   <z6=int6464#6,>in6=int64#6
# asm 2: movd   <z6=%xmm5,>in6=%r9
movd   %xmm5,%r9

# qhasm:   in7 = z7
# asm 1: movd   <z7=int6464#9,>in7=int64#7
# asm 2: movd   <z7=%xmm8,>in7=%rax
movd   %xmm8,%rax

# qhasm:   z4 <<<= 96
# asm 1: pshufd $0x39,<z4=int6464#15,<z4=int6464#15
# asm 2: pshufd $0x39,<z4=%xmm14,<z4=%xmm14
pshufd $0x39,%xmm14,%xmm14

# qhasm:   z5 <<<= 96
# asm 1: pshufd $0x39,<z5=int6464#1,<z5=int6464#1
# asm 2: pshufd $0x39,<z5=%xmm0,<z5=%xmm0
pshufd $0x39,%xmm0,%xmm0

# qhasm:   z6 <<<= 96
# asm 1: pshufd $0x39,<z6=int6464#6,<z6=int6464#6
# asm 2: pshufd $0x39,<z6=%xmm5,<z6=%xmm5
pshufd $0x39,%xmm5,%xmm5

# qhasm:   z7 <<<= 96
# asm 1: pshufd $0x39,<z7=int6464#9,<z7=int6464#9
# asm 2: pshufd $0x39,<z7=%xmm8,<z7=%xmm8
pshufd $0x39,%xmm8,%xmm8

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
# asm 1: movd   <z4=int6464#15,>in4=int64#3
# asm 2: movd   <z4=%xmm14,>in4=%rdx
movd   %xmm14,%rdx

# qhasm:   in5 = z5
# asm 1: movd   <z5=int6464#1,>in5=int64#4
# asm 2: movd   <z5=%xmm0,>in5=%rcx
movd   %xmm0,%rcx

# qhasm:   in6 = z6
# asm 1: movd   <z6=int6464#6,>in6=int64#6
# asm 2: movd   <z6=%xmm5,>in6=%r9
movd   %xmm5,%r9

# qhasm:   in7 = z7
# asm 1: movd   <z7=int6464#9,>in7=int64#7
# asm 2: movd   <z7=%xmm8,>in7=%rax
movd   %xmm8,%rax

# qhasm:   z4 <<<= 96
# asm 1: pshufd $0x39,<z4=int6464#15,<z4=int6464#15
# asm 2: pshufd $0x39,<z4=%xmm14,<z4=%xmm14
pshufd $0x39,%xmm14,%xmm14

# qhasm:   z5 <<<= 96
# asm 1: pshufd $0x39,<z5=int6464#1,<z5=int6464#1
# asm 2: pshufd $0x39,<z5=%xmm0,<z5=%xmm0
pshufd $0x39,%xmm0,%xmm0

# qhasm:   z6 <<<= 96
# asm 1: pshufd $0x39,<z6=int6464#6,<z6=int6464#6
# asm 2: pshufd $0x39,<z6=%xmm5,<z6=%xmm5
pshufd $0x39,%xmm5,%xmm5

# qhasm:   z7 <<<= 96
# asm 1: pshufd $0x39,<z7=int6464#9,<z7=int6464#9
# asm 2: pshufd $0x39,<z7=%xmm8,<z7=%xmm8
pshufd $0x39,%xmm8,%xmm8

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
# asm 1: movd   <z4=int6464#15,>in4=int64#3
# asm 2: movd   <z4=%xmm14,>in4=%rdx
movd   %xmm14,%rdx

# qhasm:   in5 = z5
# asm 1: movd   <z5=int6464#1,>in5=int64#4
# asm 2: movd   <z5=%xmm0,>in5=%rcx
movd   %xmm0,%rcx

# qhasm:   in6 = z6
# asm 1: movd   <z6=int6464#6,>in6=int64#6
# asm 2: movd   <z6=%xmm5,>in6=%r9
movd   %xmm5,%r9

# qhasm:   in7 = z7
# asm 1: movd   <z7=int6464#9,>in7=int64#7
# asm 2: movd   <z7=%xmm8,>in7=%rax
movd   %xmm8,%rax

# qhasm:   z4 <<<= 96
# asm 1: pshufd $0x39,<z4=int6464#15,<z4=int6464#15
# asm 2: pshufd $0x39,<z4=%xmm14,<z4=%xmm14
pshufd $0x39,%xmm14,%xmm14

# qhasm:   z5 <<<= 96
# asm 1: pshufd $0x39,<z5=int6464#1,<z5=int6464#1
# asm 2: pshufd $0x39,<z5=%xmm0,<z5=%xmm0
pshufd $0x39,%xmm0,%xmm0

# qhasm:   z6 <<<= 96
# asm 1: pshufd $0x39,<z6=int6464#6,<z6=int6464#6
# asm 2: pshufd $0x39,<z6=%xmm5,<z6=%xmm5
pshufd $0x39,%xmm5,%xmm5

# qhasm:   z7 <<<= 96
# asm 1: pshufd $0x39,<z7=int6464#9,<z7=int6464#9
# asm 2: pshufd $0x39,<z7=%xmm8,<z7=%xmm8
pshufd $0x39,%xmm8,%xmm8

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
# asm 1: movd   <z4=int6464#15,>in4=int64#3
# asm 2: movd   <z4=%xmm14,>in4=%rdx
movd   %xmm14,%rdx

# qhasm:   in5 = z5
# asm 1: movd   <z5=int6464#1,>in5=int64#4
# asm 2: movd   <z5=%xmm0,>in5=%rcx
movd   %xmm0,%rcx

# qhasm:   in6 = z6
# asm 1: movd   <z6=int6464#6,>in6=int64#6
# asm 2: movd   <z6=%xmm5,>in6=%r9
movd   %xmm5,%r9

# qhasm:   in7 = z7
# asm 1: movd   <z7=int6464#9,>in7=int64#7
# asm 2: movd   <z7=%xmm8,>in7=%rax
movd   %xmm8,%rax

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

# qhasm:   uint32323232 z8 += orig8
# asm 1: paddd <orig8=stack128#15,<z8=int6464#16
# asm 2: paddd <orig8=224(%rsp),<z8=%xmm15
paddd 224(%rsp),%xmm15

# qhasm:   uint32323232 z9 += orig9
# asm 1: paddd <orig9=stack128#16,<z9=int6464#12
# asm 2: paddd <orig9=240(%rsp),<z9=%xmm11
paddd 240(%rsp),%xmm11

# qhasm:   uint32323232 z10 += orig10
# asm 1: paddd <orig10=stack128#2,<z10=int6464#2
# asm 2: paddd <orig10=16(%rsp),<z10=%xmm1
paddd 16(%rsp),%xmm1

# qhasm:   uint32323232 z11 += orig11
# asm 1: paddd <orig11=stack128#6,<z11=int6464#7
# asm 2: paddd <orig11=80(%rsp),<z11=%xmm6
paddd 80(%rsp),%xmm6

# qhasm:   in8 = z8
# asm 1: movd   <z8=int6464#16,>in8=int64#3
# asm 2: movd   <z8=%xmm15,>in8=%rdx
movd   %xmm15,%rdx

# qhasm:   in9 = z9
# asm 1: movd   <z9=int6464#12,>in9=int64#4
# asm 2: movd   <z9=%xmm11,>in9=%rcx
movd   %xmm11,%rcx

# qhasm:   in10 = z10
# asm 1: movd   <z10=int6464#2,>in10=int64#6
# asm 2: movd   <z10=%xmm1,>in10=%r9
movd   %xmm1,%r9

# qhasm:   in11 = z11
# asm 1: movd   <z11=int6464#7,>in11=int64#7
# asm 2: movd   <z11=%xmm6,>in11=%rax
movd   %xmm6,%rax

# qhasm:   z8 <<<= 96
# asm 1: pshufd $0x39,<z8=int6464#16,<z8=int6464#16
# asm 2: pshufd $0x39,<z8=%xmm15,<z8=%xmm15
pshufd $0x39,%xmm15,%xmm15

# qhasm:   z9 <<<= 96
# asm 1: pshufd $0x39,<z9=int6464#12,<z9=int6464#12
# asm 2: pshufd $0x39,<z9=%xmm11,<z9=%xmm11
pshufd $0x39,%xmm11,%xmm11

# qhasm:   z10 <<<= 96
# asm 1: pshufd $0x39,<z10=int6464#2,<z10=int6464#2
# asm 2: pshufd $0x39,<z10=%xmm1,<z10=%xmm1
pshufd $0x39,%xmm1,%xmm1

# qhasm:   z11 <<<= 96
# asm 1: pshufd $0x39,<z11=int6464#7,<z11=int6464#7
# asm 2: pshufd $0x39,<z11=%xmm6,<z11=%xmm6
pshufd $0x39,%xmm6,%xmm6

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
# asm 1: movd   <z8=int6464#16,>in8=int64#3
# asm 2: movd   <z8=%xmm15,>in8=%rdx
movd   %xmm15,%rdx

# qhasm:   in9 = z9
# asm 1: movd   <z9=int6464#12,>in9=int64#4
# asm 2: movd   <z9=%xmm11,>in9=%rcx
movd   %xmm11,%rcx

# qhasm:   in10 = z10
# asm 1: movd   <z10=int6464#2,>in10=int64#6
# asm 2: movd   <z10=%xmm1,>in10=%r9
movd   %xmm1,%r9

# qhasm:   in11 = z11
# asm 1: movd   <z11=int6464#7,>in11=int64#7
# asm 2: movd   <z11=%xmm6,>in11=%rax
movd   %xmm6,%rax

# qhasm:   z8 <<<= 96
# asm 1: pshufd $0x39,<z8=int6464#16,<z8=int6464#16
# asm 2: pshufd $0x39,<z8=%xmm15,<z8=%xmm15
pshufd $0x39,%xmm15,%xmm15

# qhasm:   z9 <<<= 96
# asm 1: pshufd $0x39,<z9=int6464#12,<z9=int6464#12
# asm 2: pshufd $0x39,<z9=%xmm11,<z9=%xmm11
pshufd $0x39,%xmm11,%xmm11

# qhasm:   z10 <<<= 96
# asm 1: pshufd $0x39,<z10=int6464#2,<z10=int6464#2
# asm 2: pshufd $0x39,<z10=%xmm1,<z10=%xmm1
pshufd $0x39,%xmm1,%xmm1

# qhasm:   z11 <<<= 96
# asm 1: pshufd $0x39,<z11=int6464#7,<z11=int6464#7
# asm 2: pshufd $0x39,<z11=%xmm6,<z11=%xmm6
pshufd $0x39,%xmm6,%xmm6

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
# asm 1: movd   <z8=int6464#16,>in8=int64#3
# asm 2: movd   <z8=%xmm15,>in8=%rdx
movd   %xmm15,%rdx

# qhasm:   in9 = z9
# asm 1: movd   <z9=int6464#12,>in9=int64#4
# asm 2: movd   <z9=%xmm11,>in9=%rcx
movd   %xmm11,%rcx

# qhasm:   in10 = z10
# asm 1: movd   <z10=int6464#2,>in10=int64#6
# asm 2: movd   <z10=%xmm1,>in10=%r9
movd   %xmm1,%r9

# qhasm:   in11 = z11
# asm 1: movd   <z11=int6464#7,>in11=int64#7
# asm 2: movd   <z11=%xmm6,>in11=%rax
movd   %xmm6,%rax

# qhasm:   z8 <<<= 96
# asm 1: pshufd $0x39,<z8=int6464#16,<z8=int6464#16
# asm 2: pshufd $0x39,<z8=%xmm15,<z8=%xmm15
pshufd $0x39,%xmm15,%xmm15

# qhasm:   z9 <<<= 96
# asm 1: pshufd $0x39,<z9=int6464#12,<z9=int6464#12
# asm 2: pshufd $0x39,<z9=%xmm11,<z9=%xmm11
pshufd $0x39,%xmm11,%xmm11

# qhasm:   z10 <<<= 96
# asm 1: pshufd $0x39,<z10=int6464#2,<z10=int6464#2
# asm 2: pshufd $0x39,<z10=%xmm1,<z10=%xmm1
pshufd $0x39,%xmm1,%xmm1

# qhasm:   z11 <<<= 96
# asm 1: pshufd $0x39,<z11=int6464#7,<z11=int6464#7
# asm 2: pshufd $0x39,<z11=%xmm6,<z11=%xmm6
pshufd $0x39,%xmm6,%xmm6

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
# asm 1: movd   <z8=int6464#16,>in8=int64#3
# asm 2: movd   <z8=%xmm15,>in8=%rdx
movd   %xmm15,%rdx

# qhasm:   in9 = z9
# asm 1: movd   <z9=int6464#12,>in9=int64#4
# asm 2: movd   <z9=%xmm11,>in9=%rcx
movd   %xmm11,%rcx

# qhasm:   in10 = z10
# asm 1: movd   <z10=int6464#2,>in10=int64#6
# asm 2: movd   <z10=%xmm1,>in10=%r9
movd   %xmm1,%r9

# qhasm:   in11 = z11
# asm 1: movd   <z11=int6464#7,>in11=int64#7
# asm 2: movd   <z11=%xmm6,>in11=%rax
movd   %xmm6,%rax

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

# qhasm:   uint32323232 z12 += orig12
# asm 1: paddd <orig12=stack128#7,<z12=int6464#14
# asm 2: paddd <orig12=96(%rsp),<z12=%xmm13
paddd 96(%rsp),%xmm13

# qhasm:   uint32323232 z13 += orig13
# asm 1: paddd <orig13=stack128#10,<z13=int6464#10
# asm 2: paddd <orig13=144(%rsp),<z13=%xmm9
paddd 144(%rsp),%xmm9

# qhasm:   uint32323232 z14 += orig14
# asm 1: paddd <orig14=stack128#13,<z14=int6464#4
# asm 2: paddd <orig14=192(%rsp),<z14=%xmm3
paddd 192(%rsp),%xmm3

# qhasm:   uint32323232 z15 += orig15
# asm 1: paddd <orig15=stack128#3,<z15=int6464#3
# asm 2: paddd <orig15=32(%rsp),<z15=%xmm2
paddd 32(%rsp),%xmm2

# qhasm:   in12 = z12
# asm 1: movd   <z12=int6464#14,>in12=int64#3
# asm 2: movd   <z12=%xmm13,>in12=%rdx
movd   %xmm13,%rdx

# qhasm:   in13 = z13
# asm 1: movd   <z13=int6464#10,>in13=int64#4
# asm 2: movd   <z13=%xmm9,>in13=%rcx
movd   %xmm9,%rcx

# qhasm:   in14 = z14
# asm 1: movd   <z14=int6464#4,>in14=int64#6
# asm 2: movd   <z14=%xmm3,>in14=%r9
movd   %xmm3,%r9

# qhasm:   in15 = z15
# asm 1: movd   <z15=int6464#3,>in15=int64#7
# asm 2: movd   <z15=%xmm2,>in15=%rax
movd   %xmm2,%rax

# qhasm:   z12 <<<= 96
# asm 1: pshufd $0x39,<z12=int6464#14,<z12=int6464#14
# asm 2: pshufd $0x39,<z12=%xmm13,<z12=%xmm13
pshufd $0x39,%xmm13,%xmm13

# qhasm:   z13 <<<= 96
# asm 1: pshufd $0x39,<z13=int6464#10,<z13=int6464#10
# asm 2: pshufd $0x39,<z13=%xmm9,<z13=%xmm9
pshufd $0x39,%xmm9,%xmm9

# qhasm:   z14 <<<= 96
# asm 1: pshufd $0x39,<z14=int6464#4,<z14=int6464#4
# asm 2: pshufd $0x39,<z14=%xmm3,<z14=%xmm3
pshufd $0x39,%xmm3,%xmm3

# qhasm:   z15 <<<= 96
# asm 1: pshufd $0x39,<z15=int6464#3,<z15=int6464#3
# asm 2: pshufd $0x39,<z15=%xmm2,<z15=%xmm2
pshufd $0x39,%xmm2,%xmm2

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
# asm 1: movd   <z12=int6464#14,>in12=int64#3
# asm 2: movd   <z12=%xmm13,>in12=%rdx
movd   %xmm13,%rdx

# qhasm:   in13 = z13
# asm 1: movd   <z13=int6464#10,>in13=int64#4
# asm 2: movd   <z13=%xmm9,>in13=%rcx
movd   %xmm9,%rcx

# qhasm:   in14 = z14
# asm 1: movd   <z14=int6464#4,>in14=int64#6
# asm 2: movd   <z14=%xmm3,>in14=%r9
movd   %xmm3,%r9

# qhasm:   in15 = z15
# asm 1: movd   <z15=int6464#3,>in15=int64#7
# asm 2: movd   <z15=%xmm2,>in15=%rax
movd   %xmm2,%rax

# qhasm:   z12 <<<= 96
# asm 1: pshufd $0x39,<z12=int6464#14,<z12=int6464#14
# asm 2: pshufd $0x39,<z12=%xmm13,<z12=%xmm13
pshufd $0x39,%xmm13,%xmm13

# qhasm:   z13 <<<= 96
# asm 1: pshufd $0x39,<z13=int6464#10,<z13=int6464#10
# asm 2: pshufd $0x39,<z13=%xmm9,<z13=%xmm9
pshufd $0x39,%xmm9,%xmm9

# qhasm:   z14 <<<= 96
# asm 1: pshufd $0x39,<z14=int6464#4,<z14=int6464#4
# asm 2: pshufd $0x39,<z14=%xmm3,<z14=%xmm3
pshufd $0x39,%xmm3,%xmm3

# qhasm:   z15 <<<= 96
# asm 1: pshufd $0x39,<z15=int6464#3,<z15=int6464#3
# asm 2: pshufd $0x39,<z15=%xmm2,<z15=%xmm2
pshufd $0x39,%xmm2,%xmm2

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
# asm 1: movd   <z12=int6464#14,>in12=int64#3
# asm 2: movd   <z12=%xmm13,>in12=%rdx
movd   %xmm13,%rdx

# qhasm:   in13 = z13
# asm 1: movd   <z13=int6464#10,>in13=int64#4
# asm 2: movd   <z13=%xmm9,>in13=%rcx
movd   %xmm9,%rcx

# qhasm:   in14 = z14
# asm 1: movd   <z14=int6464#4,>in14=int64#6
# asm 2: movd   <z14=%xmm3,>in14=%r9
movd   %xmm3,%r9

# qhasm:   in15 = z15
# asm 1: movd   <z15=int6464#3,>in15=int64#7
# asm 2: movd   <z15=%xmm2,>in15=%rax
movd   %xmm2,%rax

# qhasm:   z12 <<<= 96
# asm 1: pshufd $0x39,<z12=int6464#14,<z12=int6464#14
# asm 2: pshufd $0x39,<z12=%xmm13,<z12=%xmm13
pshufd $0x39,%xmm13,%xmm13

# qhasm:   z13 <<<= 96
# asm 1: pshufd $0x39,<z13=int6464#10,<z13=int6464#10
# asm 2: pshufd $0x39,<z13=%xmm9,<z13=%xmm9
pshufd $0x39,%xmm9,%xmm9

# qhasm:   z14 <<<= 96
# asm 1: pshufd $0x39,<z14=int6464#4,<z14=int6464#4
# asm 2: pshufd $0x39,<z14=%xmm3,<z14=%xmm3
pshufd $0x39,%xmm3,%xmm3

# qhasm:   z15 <<<= 96
# asm 1: pshufd $0x39,<z15=int6464#3,<z15=int6464#3
# asm 2: pshufd $0x39,<z15=%xmm2,<z15=%xmm2
pshufd $0x39,%xmm2,%xmm2

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
# asm 1: movd   <z12=int6464#14,>in12=int64#3
# asm 2: movd   <z12=%xmm13,>in12=%rdx
movd   %xmm13,%rdx

# qhasm:   in13 = z13
# asm 1: movd   <z13=int6464#10,>in13=int64#4
# asm 2: movd   <z13=%xmm9,>in13=%rcx
movd   %xmm9,%rcx

# qhasm:   in14 = z14
# asm 1: movd   <z14=int6464#4,>in14=int64#6
# asm 2: movd   <z14=%xmm3,>in14=%r9
movd   %xmm3,%r9

# qhasm:   in15 = z15
# asm 1: movd   <z15=int6464#3,>in15=int64#7
# asm 2: movd   <z15=%xmm2,>in15=%rax
movd   %xmm2,%rax

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
# asm 2: movq <bytes_backup=288(%rsp),>bytes=%rdx
movq 288(%rsp),%rdx

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
# asm 2: leaq <tmp=320(%rsp),>out=%rdi
leaq 320(%rsp),%rdi

# qhasm:     i = bytes
# asm 1: mov  <bytes=int64#3,>i=int64#4
# asm 2: mov  <bytes=%rdx,>i=%rcx
mov  %rdx,%rcx

# qhasm:     while (i) { *out++ = *m++; --i }
rep movsb

# qhasm:     out = &tmp
# asm 1: leaq <tmp=stack512#1,>out=int64#1
# asm 2: leaq <tmp=320(%rsp),>out=%rdi
leaq 320(%rsp),%rdi

# qhasm:     m = &tmp
# asm 1: leaq <tmp=stack512#1,>m=int64#2
# asm 2: leaq <tmp=320(%rsp),>m=%rsi
leaq 320(%rsp),%rsi
# comment:fp stack unchanged by fallthrough

# qhasm:   nocopy:
._nocopy:

# qhasm:   bytes_backup = bytes
# asm 1: movq <bytes=int64#3,>bytes_backup=stack64#1
# asm 2: movq <bytes=%rdx,>bytes_backup=288(%rsp)
movq %rdx,288(%rsp)

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
# asm 2: movq <bytes_backup=288(%rsp),>bytes=%rdx
movq 288(%rsp),%rdx

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
add $384,%r11
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
add $384,%r11
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
add $384,%r11
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
