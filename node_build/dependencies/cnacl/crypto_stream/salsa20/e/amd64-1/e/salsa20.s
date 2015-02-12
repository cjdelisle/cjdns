
# qhasm: int64 a

# qhasm: int64 b

# qhasm: int64 c

# qhasm: int64 d

# qhasm: int64 e

# qhasm: int64 A

# qhasm: int64 B

# qhasm: int64 C

# qhasm: int64 D

# qhasm: int64 E

# qhasm: int64 r

# qhasm: int64 s

# qhasm: int64 t

# qhasm: int64 u

# qhasm: int64 v

# qhasm: int64 R

# qhasm: int64 S

# qhasm: int64 T

# qhasm: int64 U

# qhasm: int64 V

# qhasm: int64 arg1

# qhasm: int64 arg2

# qhasm: int64 arg3

# qhasm: int64 arg4

# qhasm: input arg1

# qhasm: input arg2

# qhasm: input arg3

# qhasm: input arg4

# qhasm: int64 r11

# qhasm: int64 r12

# qhasm: int64 r13

# qhasm: int64 r14

# qhasm: int64 r15

# qhasm: int64 rbx

# qhasm: int64 rbp

# qhasm: caller r11

# qhasm: caller r12

# qhasm: caller r13

# qhasm: caller r14

# qhasm: caller r15

# qhasm: caller rbx

# qhasm: caller rbp

# qhasm: stack64 r11_stack

# qhasm: stack64 r12_stack

# qhasm: stack64 r13_stack

# qhasm: stack64 r14_stack

# qhasm: stack64 r15_stack

# qhasm: stack64 rbx_stack

# qhasm: stack64 rbp_stack

# qhasm: int64 k

# qhasm: int64 kbits

# qhasm: int64 iv

# qhasm: int64 i

# qhasm: stack64 x_backup

# qhasm: int64 x

# qhasm: stack64 m_backup

# qhasm: int64 m

# qhasm: stack64 out_backup

# qhasm: int64 out

# qhasm: stack64 bytes_backup

# qhasm: int64 bytes

# qhasm: int64 in0

# qhasm: int64 in2

# qhasm: int64 in4

# qhasm: int64 in6

# qhasm: int64 in8

# qhasm: int64 in10

# qhasm: int64 in12

# qhasm: int64 in14

# qhasm: int64 out0

# qhasm: int64 out2

# qhasm: int64 out4

# qhasm: int64 out6

# qhasm: int64 out8

# qhasm: int64 out10

# qhasm: int64 out12

# qhasm: int64 out14

# qhasm: stack64 x0

# qhasm: stack64 x1

# qhasm: stack64 x2

# qhasm: stack64 x3

# qhasm: stack64 x4

# qhasm: stack64 x5

# qhasm: stack64 x6

# qhasm: stack64 x7

# qhasm: stack64 x8

# qhasm: stack64 x9

# qhasm: stack64 x10

# qhasm: stack64 x11

# qhasm: stack64 x12

# qhasm: stack64 x13

# qhasm: stack64 x14

# qhasm: stack64 x15

# qhasm: stack64 j0

# qhasm: stack64 j2

# qhasm: stack64 j4

# qhasm: stack64 j6

# qhasm: stack64 j8

# qhasm: stack64 j10

# qhasm: stack64 j12

# qhasm: stack64 j14

# qhasm: stack512 tmp

# qhasm: int64 ctarget

# qhasm: enter ECRYPT_keystream_bytes
.text
.p2align 5
.globl _ECRYPT_keystream_bytes
.globl ECRYPT_keystream_bytes
_ECRYPT_keystream_bytes:
ECRYPT_keystream_bytes:
mov %rsp,%r11
and $31,%r11
add $352,%r11
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
add $352,%r11
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
add $352,%r11
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

# qhasm: r11_stack = r11
# asm 1: movq <r11=int64#9,>r11_stack=stack64#1
# asm 2: movq <r11=%r11,>r11_stack=0(%rsp)
movq %r11,0(%rsp)

# qhasm: r12_stack = r12
# asm 1: movq <r12=int64#10,>r12_stack=stack64#2
# asm 2: movq <r12=%r12,>r12_stack=8(%rsp)
movq %r12,8(%rsp)

# qhasm: r13_stack = r13
# asm 1: movq <r13=int64#11,>r13_stack=stack64#3
# asm 2: movq <r13=%r13,>r13_stack=16(%rsp)
movq %r13,16(%rsp)

# qhasm: r14_stack = r14
# asm 1: movq <r14=int64#12,>r14_stack=stack64#4
# asm 2: movq <r14=%r14,>r14_stack=24(%rsp)
movq %r14,24(%rsp)

# qhasm: r15_stack = r15
# asm 1: movq <r15=int64#13,>r15_stack=stack64#5
# asm 2: movq <r15=%r15,>r15_stack=32(%rsp)
movq %r15,32(%rsp)

# qhasm: rbx_stack = rbx
# asm 1: movq <rbx=int64#14,>rbx_stack=stack64#6
# asm 2: movq <rbx=%rbx,>rbx_stack=40(%rsp)
movq %rbx,40(%rsp)

# qhasm: rbp_stack = rbp
# asm 1: movq <rbp=int64#15,>rbp_stack=stack64#7
# asm 2: movq <rbp=%rbp,>rbp_stack=48(%rsp)
movq %rbp,48(%rsp)

# qhasm: in0 = *(uint64 *) (x + 0)
# asm 1: movq   0(<x=int64#5),>in0=int64#4
# asm 2: movq   0(<x=%r8),>in0=%rcx
movq   0(%r8),%rcx

# qhasm: in2 = *(uint64 *) (x + 8)
# asm 1: movq   8(<x=int64#5),>in2=int64#6
# asm 2: movq   8(<x=%r8),>in2=%r9
movq   8(%r8),%r9

# qhasm: in4 = *(uint64 *) (x + 16)
# asm 1: movq   16(<x=int64#5),>in4=int64#7
# asm 2: movq   16(<x=%r8),>in4=%rax
movq   16(%r8),%rax

# qhasm: in6 = *(uint64 *) (x + 24)
# asm 1: movq   24(<x=int64#5),>in6=int64#8
# asm 2: movq   24(<x=%r8),>in6=%r10
movq   24(%r8),%r10

# qhasm: in8 = *(uint64 *) (x + 32)
# asm 1: movq   32(<x=int64#5),>in8=int64#9
# asm 2: movq   32(<x=%r8),>in8=%r11
movq   32(%r8),%r11

# qhasm: in10 = *(uint64 *) (x + 40)
# asm 1: movq   40(<x=int64#5),>in10=int64#10
# asm 2: movq   40(<x=%r8),>in10=%r12
movq   40(%r8),%r12

# qhasm: in12 = *(uint64 *) (x + 48)
# asm 1: movq   48(<x=int64#5),>in12=int64#11
# asm 2: movq   48(<x=%r8),>in12=%r13
movq   48(%r8),%r13

# qhasm: in14 = *(uint64 *) (x + 56)
# asm 1: movq   56(<x=int64#5),>in14=int64#12
# asm 2: movq   56(<x=%r8),>in14=%r14
movq   56(%r8),%r14

# qhasm: j0 = in0
# asm 1: movq <in0=int64#4,>j0=stack64#8
# asm 2: movq <in0=%rcx,>j0=56(%rsp)
movq %rcx,56(%rsp)

# qhasm: j2 = in2
# asm 1: movq <in2=int64#6,>j2=stack64#9
# asm 2: movq <in2=%r9,>j2=64(%rsp)
movq %r9,64(%rsp)

# qhasm: j4 = in4
# asm 1: movq <in4=int64#7,>j4=stack64#10
# asm 2: movq <in4=%rax,>j4=72(%rsp)
movq %rax,72(%rsp)

# qhasm: j6 = in6
# asm 1: movq <in6=int64#8,>j6=stack64#11
# asm 2: movq <in6=%r10,>j6=80(%rsp)
movq %r10,80(%rsp)

# qhasm: j8 = in8
# asm 1: movq <in8=int64#9,>j8=stack64#12
# asm 2: movq <in8=%r11,>j8=88(%rsp)
movq %r11,88(%rsp)

# qhasm: j10 = in10
# asm 1: movq <in10=int64#10,>j10=stack64#13
# asm 2: movq <in10=%r12,>j10=96(%rsp)
movq %r12,96(%rsp)

# qhasm: j12 = in12
# asm 1: movq <in12=int64#11,>j12=stack64#14
# asm 2: movq <in12=%r13,>j12=104(%rsp)
movq %r13,104(%rsp)

# qhasm: j14 = in14
# asm 1: movq <in14=int64#12,>j14=stack64#15
# asm 2: movq <in14=%r14,>j14=112(%rsp)
movq %r14,112(%rsp)

# qhasm: x_backup = x
# asm 1: movq <x=int64#5,>x_backup=stack64#16
# asm 2: movq <x=%r8,>x_backup=120(%rsp)
movq %r8,120(%rsp)

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
# asm 1: mov  <out=int64#1,>ctarget=int64#5
# asm 2: mov  <out=%rdi,>ctarget=%r8
mov  %rdi,%r8

# qhasm:     out = &tmp
# asm 1: leaq <tmp=stack512#1,>out=int64#1
# asm 2: leaq <tmp=288(%rsp),>out=%rdi
leaq 288(%rsp),%rdi

# qhasm:     i = bytes
# asm 1: mov  <bytes=int64#3,>i=int64#4
# asm 2: mov  <bytes=%rdx,>i=%rcx
mov  %rdx,%rcx

# qhasm:     while (i) { *out++ = *m++; --i }
rep movsb

# qhasm:     out = &tmp
# asm 1: leaq <tmp=stack512#1,>out=int64#1
# asm 2: leaq <tmp=288(%rsp),>out=%rdi
leaq 288(%rsp),%rdi

# qhasm:     m = &tmp
# asm 1: leaq <tmp=stack512#1,>m=int64#2
# asm 2: leaq <tmp=288(%rsp),>m=%rsi
leaq 288(%rsp),%rsi
# comment:fp stack unchanged by fallthrough

# qhasm:   nocopy:
._nocopy:

# qhasm:   out_backup = out
# asm 1: movq <out=int64#1,>out_backup=stack64#17
# asm 2: movq <out=%rdi,>out_backup=128(%rsp)
movq %rdi,128(%rsp)

# qhasm:   m_backup = m
# asm 1: movq <m=int64#2,>m_backup=stack64#18
# asm 2: movq <m=%rsi,>m_backup=136(%rsp)
movq %rsi,136(%rsp)

# qhasm:   bytes_backup = bytes
# asm 1: movq <bytes=int64#3,>bytes_backup=stack64#19
# asm 2: movq <bytes=%rdx,>bytes_backup=144(%rsp)
movq %rdx,144(%rsp)

# qhasm:   in0 = j0
# asm 1: movq <j0=stack64#8,>in0=int64#1
# asm 2: movq <j0=56(%rsp),>in0=%rdi
movq 56(%rsp),%rdi

# qhasm:   in2 = j2
# asm 1: movq <j2=stack64#9,>in2=int64#2
# asm 2: movq <j2=64(%rsp),>in2=%rsi
movq 64(%rsp),%rsi

# qhasm:   in4 = j4
# asm 1: movq <j4=stack64#10,>in4=int64#3
# asm 2: movq <j4=72(%rsp),>in4=%rdx
movq 72(%rsp),%rdx

# qhasm:   in6 = j6
# asm 1: movq <j6=stack64#11,>in6=int64#4
# asm 2: movq <j6=80(%rsp),>in6=%rcx
movq 80(%rsp),%rcx

# qhasm:   in8 = j8
# asm 1: movq <j8=stack64#12,>in8=int64#6
# asm 2: movq <j8=88(%rsp),>in8=%r9
movq 88(%rsp),%r9

# qhasm:   in10 = j10
# asm 1: movq <j10=stack64#13,>in10=int64#7
# asm 2: movq <j10=96(%rsp),>in10=%rax
movq 96(%rsp),%rax

# qhasm:   in12 = j12
# asm 1: movq <j12=stack64#14,>in12=int64#8
# asm 2: movq <j12=104(%rsp),>in12=%r10
movq 104(%rsp),%r10

# qhasm:   in14 = j14
# asm 1: movq <j14=stack64#15,>in14=int64#9
# asm 2: movq <j14=112(%rsp),>in14=%r11
movq 112(%rsp),%r11

# qhasm:   x0 = in0
# asm 1: movq <in0=int64#1,>x0=stack64#20
# asm 2: movq <in0=%rdi,>x0=152(%rsp)
movq %rdi,152(%rsp)

# qhasm:   x2 = in2
# asm 1: movq <in2=int64#2,>x2=stack64#21
# asm 2: movq <in2=%rsi,>x2=160(%rsp)
movq %rsi,160(%rsp)

# qhasm:   x4 = in4
# asm 1: movq <in4=int64#3,>x4=stack64#22
# asm 2: movq <in4=%rdx,>x4=168(%rsp)
movq %rdx,168(%rsp)

# qhasm:   x6 = in6
# asm 1: movq <in6=int64#4,>x6=stack64#23
# asm 2: movq <in6=%rcx,>x6=176(%rsp)
movq %rcx,176(%rsp)

# qhasm:   x8 = in8
# asm 1: movq <in8=int64#6,>x8=stack64#24
# asm 2: movq <in8=%r9,>x8=184(%rsp)
movq %r9,184(%rsp)

# qhasm:   x10 = in10
# asm 1: movq <in10=int64#7,>x10=stack64#25
# asm 2: movq <in10=%rax,>x10=192(%rsp)
movq %rax,192(%rsp)

# qhasm:   x12 = in12
# asm 1: movq <in12=int64#8,>x12=stack64#26
# asm 2: movq <in12=%r10,>x12=200(%rsp)
movq %r10,200(%rsp)

# qhasm:   x14 = in14
# asm 1: movq <in14=int64#9,>x14=stack64#27
# asm 2: movq <in14=%r11,>x14=208(%rsp)
movq %r11,208(%rsp)

# qhasm:   (uint64) in0 >>= 32
# asm 1: shr  $32,<in0=int64#1
# asm 2: shr  $32,<in0=%rdi
shr  $32,%rdi

# qhasm:   (uint64) in2 >>= 32
# asm 1: shr  $32,<in2=int64#2
# asm 2: shr  $32,<in2=%rsi
shr  $32,%rsi

# qhasm:   (uint64) in4 >>= 32
# asm 1: shr  $32,<in4=int64#3
# asm 2: shr  $32,<in4=%rdx
shr  $32,%rdx

# qhasm:   (uint64) in6 >>= 32
# asm 1: shr  $32,<in6=int64#4
# asm 2: shr  $32,<in6=%rcx
shr  $32,%rcx

# qhasm:   (uint64) in8 >>= 32
# asm 1: shr  $32,<in8=int64#6
# asm 2: shr  $32,<in8=%r9
shr  $32,%r9

# qhasm:   (uint64) in10 >>= 32
# asm 1: shr  $32,<in10=int64#7
# asm 2: shr  $32,<in10=%rax
shr  $32,%rax

# qhasm:   (uint64) in12 >>= 32
# asm 1: shr  $32,<in12=int64#8
# asm 2: shr  $32,<in12=%r10
shr  $32,%r10

# qhasm:   (uint64) in14 >>= 32
# asm 1: shr  $32,<in14=int64#9
# asm 2: shr  $32,<in14=%r11
shr  $32,%r11

# qhasm:   x1 = in0
# asm 1: movq <in0=int64#1,>x1=stack64#28
# asm 2: movq <in0=%rdi,>x1=216(%rsp)
movq %rdi,216(%rsp)

# qhasm:   x3 = in2
# asm 1: movq <in2=int64#2,>x3=stack64#29
# asm 2: movq <in2=%rsi,>x3=224(%rsp)
movq %rsi,224(%rsp)

# qhasm:   x5 = in4
# asm 1: movq <in4=int64#3,>x5=stack64#30
# asm 2: movq <in4=%rdx,>x5=232(%rsp)
movq %rdx,232(%rsp)

# qhasm:   x7 = in6
# asm 1: movq <in6=int64#4,>x7=stack64#31
# asm 2: movq <in6=%rcx,>x7=240(%rsp)
movq %rcx,240(%rsp)

# qhasm:   x9 = in8
# asm 1: movq <in8=int64#6,>x9=stack64#32
# asm 2: movq <in8=%r9,>x9=248(%rsp)
movq %r9,248(%rsp)

# qhasm:   x11 = in10
# asm 1: movq <in10=int64#7,>x11=stack64#33
# asm 2: movq <in10=%rax,>x11=256(%rsp)
movq %rax,256(%rsp)

# qhasm:   x13 = in12
# asm 1: movq <in12=int64#8,>x13=stack64#34
# asm 2: movq <in12=%r10,>x13=264(%rsp)
movq %r10,264(%rsp)

# qhasm:   x15 = in14
# asm 1: movq <in14=int64#9,>x15=stack64#35
# asm 2: movq <in14=%r11,>x15=272(%rsp)
movq %r11,272(%rsp)

# qhasm:   i = 20
# asm 1: mov  $20,>i=int64#1
# asm 2: mov  $20,>i=%rdi
mov  $20,%rdi

# qhasm:   mainloop:
._mainloop:

# qhasm: a = x12
# asm 1: movq <x12=stack64#26,>a=int64#2
# asm 2: movq <x12=200(%rsp),>a=%rsi
movq 200(%rsp),%rsi

# qhasm: b = x0
# asm 1: movq <x0=stack64#20,>b=int64#3
# asm 2: movq <x0=152(%rsp),>b=%rdx
movq 152(%rsp),%rdx

# qhasm: c = x4
# asm 1: movq <x4=stack64#22,>c=int64#4
# asm 2: movq <x4=168(%rsp),>c=%rcx
movq 168(%rsp),%rcx

# qhasm: e = a + b
# asm 1: lea  (<a=int64#2,<b=int64#3),>e=int64#6
# asm 2: lea  (<a=%rsi,<b=%rdx),>e=%r9
lea  (%rsi,%rdx),%r9

# qhasm: (uint32) e <<<= 7
# asm 1: rol  $7,<e=int64#6d
# asm 2: rol  $7,<e=%r9d
rol  $7,%r9d

# qhasm: c ^= e
# asm 1: xor  <e=int64#6,<c=int64#4
# asm 2: xor  <e=%r9,<c=%rcx
xor  %r9,%rcx

# qhasm: x4 = c
# asm 1: movq <c=int64#4,>x4=stack64#22
# asm 2: movq <c=%rcx,>x4=168(%rsp)
movq %rcx,168(%rsp)

# qhasm: d = x8
# asm 1: movq <x8=stack64#24,>d=int64#6
# asm 2: movq <x8=184(%rsp),>d=%r9
movq 184(%rsp),%r9

# qhasm: e = b + c
# asm 1: lea  (<b=int64#3,<c=int64#4),>e=int64#7
# asm 2: lea  (<b=%rdx,<c=%rcx),>e=%rax
lea  (%rdx,%rcx),%rax

# qhasm: (uint32) e <<<= 9
# asm 1: rol  $9,<e=int64#7d
# asm 2: rol  $9,<e=%eax
rol  $9,%eax

# qhasm: d ^= e
# asm 1: xor  <e=int64#7,<d=int64#6
# asm 2: xor  <e=%rax,<d=%r9
xor  %rax,%r9

# qhasm: x8 = d
# asm 1: movq <d=int64#6,>x8=stack64#24
# asm 2: movq <d=%r9,>x8=184(%rsp)
movq %r9,184(%rsp)

# qhasm: c += d
# asm 1: add  <d=int64#6,<c=int64#4
# asm 2: add  <d=%r9,<c=%rcx
add  %r9,%rcx

# qhasm: (uint32) c <<<= 13
# asm 1: rol  $13,<c=int64#4d
# asm 2: rol  $13,<c=%ecx
rol  $13,%ecx

# qhasm: a ^= c
# asm 1: xor  <c=int64#4,<a=int64#2
# asm 2: xor  <c=%rcx,<a=%rsi
xor  %rcx,%rsi

# qhasm: x12 = a
# asm 1: movq <a=int64#2,>x12=stack64#26
# asm 2: movq <a=%rsi,>x12=200(%rsp)
movq %rsi,200(%rsp)

# qhasm: a += d
# asm 1: add  <d=int64#6,<a=int64#2
# asm 2: add  <d=%r9,<a=%rsi
add  %r9,%rsi

# qhasm: (uint32) a <<<= 18
# asm 1: rol  $18,<a=int64#2d
# asm 2: rol  $18,<a=%esi
rol  $18,%esi

# qhasm: b ^= a
# asm 1: xor  <a=int64#2,<b=int64#3
# asm 2: xor  <a=%rsi,<b=%rdx
xor  %rsi,%rdx

# qhasm: x0 = b
# asm 1: movq <b=int64#3,>x0=stack64#20
# asm 2: movq <b=%rdx,>x0=152(%rsp)
movq %rdx,152(%rsp)

# qhasm: 		r = x1
# asm 1: movq <x1=stack64#28,>r=int64#2
# asm 2: movq <x1=216(%rsp),>r=%rsi
movq 216(%rsp),%rsi

# qhasm: 		s = x5
# asm 1: movq <x5=stack64#30,>s=int64#3
# asm 2: movq <x5=232(%rsp),>s=%rdx
movq 232(%rsp),%rdx

# qhasm: 		t = x9
# asm 1: movq <x9=stack64#32,>t=int64#4
# asm 2: movq <x9=248(%rsp),>t=%rcx
movq 248(%rsp),%rcx

# qhasm: 		v = r + s
# asm 1: lea  (<r=int64#2,<s=int64#3),>v=int64#6
# asm 2: lea  (<r=%rsi,<s=%rdx),>v=%r9
lea  (%rsi,%rdx),%r9

# qhasm: 		(uint32) v <<<= 7
# asm 1: rol  $7,<v=int64#6d
# asm 2: rol  $7,<v=%r9d
rol  $7,%r9d

# qhasm: 		t ^= v
# asm 1: xor  <v=int64#6,<t=int64#4
# asm 2: xor  <v=%r9,<t=%rcx
xor  %r9,%rcx

# qhasm: 		x9 = t
# asm 1: movq <t=int64#4,>x9=stack64#32
# asm 2: movq <t=%rcx,>x9=248(%rsp)
movq %rcx,248(%rsp)

# qhasm: 		u = x13
# asm 1: movq <x13=stack64#34,>u=int64#6
# asm 2: movq <x13=264(%rsp),>u=%r9
movq 264(%rsp),%r9

# qhasm: 		v = s + t
# asm 1: lea  (<s=int64#3,<t=int64#4),>v=int64#7
# asm 2: lea  (<s=%rdx,<t=%rcx),>v=%rax
lea  (%rdx,%rcx),%rax

# qhasm: 		(uint32) v <<<= 9
# asm 1: rol  $9,<v=int64#7d
# asm 2: rol  $9,<v=%eax
rol  $9,%eax

# qhasm: 		u ^= v
# asm 1: xor  <v=int64#7,<u=int64#6
# asm 2: xor  <v=%rax,<u=%r9
xor  %rax,%r9

# qhasm: 		x13 = u
# asm 1: movq <u=int64#6,>x13=stack64#34
# asm 2: movq <u=%r9,>x13=264(%rsp)
movq %r9,264(%rsp)

# qhasm: 		t += u
# asm 1: add  <u=int64#6,<t=int64#4
# asm 2: add  <u=%r9,<t=%rcx
add  %r9,%rcx

# qhasm: 		(uint32) t <<<= 13
# asm 1: rol  $13,<t=int64#4d
# asm 2: rol  $13,<t=%ecx
rol  $13,%ecx

# qhasm: 		r ^= t
# asm 1: xor  <t=int64#4,<r=int64#2
# asm 2: xor  <t=%rcx,<r=%rsi
xor  %rcx,%rsi

# qhasm: 		x1 = r
# asm 1: movq <r=int64#2,>x1=stack64#28
# asm 2: movq <r=%rsi,>x1=216(%rsp)
movq %rsi,216(%rsp)

# qhasm: 		r += u
# asm 1: add  <u=int64#6,<r=int64#2
# asm 2: add  <u=%r9,<r=%rsi
add  %r9,%rsi

# qhasm: 		(uint32) r <<<= 18
# asm 1: rol  $18,<r=int64#2d
# asm 2: rol  $18,<r=%esi
rol  $18,%esi

# qhasm: 		s ^= r
# asm 1: xor  <r=int64#2,<s=int64#3
# asm 2: xor  <r=%rsi,<s=%rdx
xor  %rsi,%rdx

# qhasm: 		x5 = s
# asm 1: movq <s=int64#3,>x5=stack64#30
# asm 2: movq <s=%rdx,>x5=232(%rsp)
movq %rdx,232(%rsp)

# qhasm: 				A = x6
# asm 1: movq <x6=stack64#23,>A=int64#2
# asm 2: movq <x6=176(%rsp),>A=%rsi
movq 176(%rsp),%rsi

# qhasm: 				B = x10
# asm 1: movq <x10=stack64#25,>B=int64#3
# asm 2: movq <x10=192(%rsp),>B=%rdx
movq 192(%rsp),%rdx

# qhasm: 				C = x14
# asm 1: movq <x14=stack64#27,>C=int64#4
# asm 2: movq <x14=208(%rsp),>C=%rcx
movq 208(%rsp),%rcx

# qhasm: 				E = A + B
# asm 1: lea  (<A=int64#2,<B=int64#3),>E=int64#6
# asm 2: lea  (<A=%rsi,<B=%rdx),>E=%r9
lea  (%rsi,%rdx),%r9

# qhasm: 				(uint32) E <<<= 7
# asm 1: rol  $7,<E=int64#6d
# asm 2: rol  $7,<E=%r9d
rol  $7,%r9d

# qhasm: 				C ^= E
# asm 1: xor  <E=int64#6,<C=int64#4
# asm 2: xor  <E=%r9,<C=%rcx
xor  %r9,%rcx

# qhasm: 				x14 = C
# asm 1: movq <C=int64#4,>x14=stack64#27
# asm 2: movq <C=%rcx,>x14=208(%rsp)
movq %rcx,208(%rsp)

# qhasm: 				D = x2
# asm 1: movq <x2=stack64#21,>D=int64#6
# asm 2: movq <x2=160(%rsp),>D=%r9
movq 160(%rsp),%r9

# qhasm: 				E = B + C
# asm 1: lea  (<B=int64#3,<C=int64#4),>E=int64#7
# asm 2: lea  (<B=%rdx,<C=%rcx),>E=%rax
lea  (%rdx,%rcx),%rax

# qhasm: 				(uint32) E <<<= 9
# asm 1: rol  $9,<E=int64#7d
# asm 2: rol  $9,<E=%eax
rol  $9,%eax

# qhasm: 				D ^= E
# asm 1: xor  <E=int64#7,<D=int64#6
# asm 2: xor  <E=%rax,<D=%r9
xor  %rax,%r9

# qhasm: 				x2 = D
# asm 1: movq <D=int64#6,>x2=stack64#21
# asm 2: movq <D=%r9,>x2=160(%rsp)
movq %r9,160(%rsp)

# qhasm: 				C += D
# asm 1: add  <D=int64#6,<C=int64#4
# asm 2: add  <D=%r9,<C=%rcx
add  %r9,%rcx

# qhasm: 				(uint32) C <<<= 13
# asm 1: rol  $13,<C=int64#4d
# asm 2: rol  $13,<C=%ecx
rol  $13,%ecx

# qhasm: 				A ^= C
# asm 1: xor  <C=int64#4,<A=int64#2
# asm 2: xor  <C=%rcx,<A=%rsi
xor  %rcx,%rsi

# qhasm: 				x6 = A
# asm 1: movq <A=int64#2,>x6=stack64#23
# asm 2: movq <A=%rsi,>x6=176(%rsp)
movq %rsi,176(%rsp)

# qhasm: 				A += D
# asm 1: add  <D=int64#6,<A=int64#2
# asm 2: add  <D=%r9,<A=%rsi
add  %r9,%rsi

# qhasm: 				(uint32) A <<<= 18
# asm 1: rol  $18,<A=int64#2d
# asm 2: rol  $18,<A=%esi
rol  $18,%esi

# qhasm: 				B ^= A
# asm 1: xor  <A=int64#2,<B=int64#3
# asm 2: xor  <A=%rsi,<B=%rdx
xor  %rsi,%rdx

# qhasm: 				x10 = B
# asm 1: movq <B=int64#3,>x10=stack64#25
# asm 2: movq <B=%rdx,>x10=192(%rsp)
movq %rdx,192(%rsp)

# qhasm: 						R = x11
# asm 1: movq <x11=stack64#33,>R=int64#2
# asm 2: movq <x11=256(%rsp),>R=%rsi
movq 256(%rsp),%rsi

# qhasm: 						S = x15
# asm 1: movq <x15=stack64#35,>S=int64#3
# asm 2: movq <x15=272(%rsp),>S=%rdx
movq 272(%rsp),%rdx

# qhasm: 						T = x3
# asm 1: movq <x3=stack64#29,>T=int64#4
# asm 2: movq <x3=224(%rsp),>T=%rcx
movq 224(%rsp),%rcx

# qhasm: 						V = R + S
# asm 1: lea  (<R=int64#2,<S=int64#3),>V=int64#6
# asm 2: lea  (<R=%rsi,<S=%rdx),>V=%r9
lea  (%rsi,%rdx),%r9

# qhasm: 						(uint32) V <<<= 7
# asm 1: rol  $7,<V=int64#6d
# asm 2: rol  $7,<V=%r9d
rol  $7,%r9d

# qhasm: 						T ^= V
# asm 1: xor  <V=int64#6,<T=int64#4
# asm 2: xor  <V=%r9,<T=%rcx
xor  %r9,%rcx

# qhasm: 						x3 = T
# asm 1: movq <T=int64#4,>x3=stack64#29
# asm 2: movq <T=%rcx,>x3=224(%rsp)
movq %rcx,224(%rsp)

# qhasm: 						U = x7
# asm 1: movq <x7=stack64#31,>U=int64#6
# asm 2: movq <x7=240(%rsp),>U=%r9
movq 240(%rsp),%r9

# qhasm: 						V = S + T
# asm 1: lea  (<S=int64#3,<T=int64#4),>V=int64#7
# asm 2: lea  (<S=%rdx,<T=%rcx),>V=%rax
lea  (%rdx,%rcx),%rax

# qhasm: 						(uint32) V <<<= 9
# asm 1: rol  $9,<V=int64#7d
# asm 2: rol  $9,<V=%eax
rol  $9,%eax

# qhasm: 						U ^= V
# asm 1: xor  <V=int64#7,<U=int64#6
# asm 2: xor  <V=%rax,<U=%r9
xor  %rax,%r9

# qhasm: 						x7 = U
# asm 1: movq <U=int64#6,>x7=stack64#31
# asm 2: movq <U=%r9,>x7=240(%rsp)
movq %r9,240(%rsp)

# qhasm: 						T += U
# asm 1: add  <U=int64#6,<T=int64#4
# asm 2: add  <U=%r9,<T=%rcx
add  %r9,%rcx

# qhasm: 						(uint32) T <<<= 13
# asm 1: rol  $13,<T=int64#4d
# asm 2: rol  $13,<T=%ecx
rol  $13,%ecx

# qhasm: 						R ^= T
# asm 1: xor  <T=int64#4,<R=int64#2
# asm 2: xor  <T=%rcx,<R=%rsi
xor  %rcx,%rsi

# qhasm: 						x11 = R
# asm 1: movq <R=int64#2,>x11=stack64#33
# asm 2: movq <R=%rsi,>x11=256(%rsp)
movq %rsi,256(%rsp)

# qhasm: 						R += U
# asm 1: add  <U=int64#6,<R=int64#2
# asm 2: add  <U=%r9,<R=%rsi
add  %r9,%rsi

# qhasm: 						(uint32) R <<<= 18
# asm 1: rol  $18,<R=int64#2d
# asm 2: rol  $18,<R=%esi
rol  $18,%esi

# qhasm: 						S ^= R
# asm 1: xor  <R=int64#2,<S=int64#3
# asm 2: xor  <R=%rsi,<S=%rdx
xor  %rsi,%rdx

# qhasm: 						x15 = S
# asm 1: movq <S=int64#3,>x15=stack64#35
# asm 2: movq <S=%rdx,>x15=272(%rsp)
movq %rdx,272(%rsp)

# qhasm: a = x3
# asm 1: movq <x3=stack64#29,>a=int64#2
# asm 2: movq <x3=224(%rsp),>a=%rsi
movq 224(%rsp),%rsi

# qhasm: b = x0
# asm 1: movq <x0=stack64#20,>b=int64#3
# asm 2: movq <x0=152(%rsp),>b=%rdx
movq 152(%rsp),%rdx

# qhasm: c = x1
# asm 1: movq <x1=stack64#28,>c=int64#4
# asm 2: movq <x1=216(%rsp),>c=%rcx
movq 216(%rsp),%rcx

# qhasm: e = a + b
# asm 1: lea  (<a=int64#2,<b=int64#3),>e=int64#6
# asm 2: lea  (<a=%rsi,<b=%rdx),>e=%r9
lea  (%rsi,%rdx),%r9

# qhasm: (uint32) e <<<= 7
# asm 1: rol  $7,<e=int64#6d
# asm 2: rol  $7,<e=%r9d
rol  $7,%r9d

# qhasm: c ^= e
# asm 1: xor  <e=int64#6,<c=int64#4
# asm 2: xor  <e=%r9,<c=%rcx
xor  %r9,%rcx

# qhasm: x1 = c
# asm 1: movq <c=int64#4,>x1=stack64#28
# asm 2: movq <c=%rcx,>x1=216(%rsp)
movq %rcx,216(%rsp)

# qhasm: d = x2
# asm 1: movq <x2=stack64#21,>d=int64#6
# asm 2: movq <x2=160(%rsp),>d=%r9
movq 160(%rsp),%r9

# qhasm: e = b + c
# asm 1: lea  (<b=int64#3,<c=int64#4),>e=int64#7
# asm 2: lea  (<b=%rdx,<c=%rcx),>e=%rax
lea  (%rdx,%rcx),%rax

# qhasm: (uint32) e <<<= 9
# asm 1: rol  $9,<e=int64#7d
# asm 2: rol  $9,<e=%eax
rol  $9,%eax

# qhasm: d ^= e
# asm 1: xor  <e=int64#7,<d=int64#6
# asm 2: xor  <e=%rax,<d=%r9
xor  %rax,%r9

# qhasm: x2 = d
# asm 1: movq <d=int64#6,>x2=stack64#21
# asm 2: movq <d=%r9,>x2=160(%rsp)
movq %r9,160(%rsp)

# qhasm: c += d
# asm 1: add  <d=int64#6,<c=int64#4
# asm 2: add  <d=%r9,<c=%rcx
add  %r9,%rcx

# qhasm: (uint32) c <<<= 13
# asm 1: rol  $13,<c=int64#4d
# asm 2: rol  $13,<c=%ecx
rol  $13,%ecx

# qhasm: a ^= c
# asm 1: xor  <c=int64#4,<a=int64#2
# asm 2: xor  <c=%rcx,<a=%rsi
xor  %rcx,%rsi

# qhasm: x3 = a
# asm 1: movq <a=int64#2,>x3=stack64#29
# asm 2: movq <a=%rsi,>x3=224(%rsp)
movq %rsi,224(%rsp)

# qhasm: a += d
# asm 1: add  <d=int64#6,<a=int64#2
# asm 2: add  <d=%r9,<a=%rsi
add  %r9,%rsi

# qhasm: (uint32) a <<<= 18
# asm 1: rol  $18,<a=int64#2d
# asm 2: rol  $18,<a=%esi
rol  $18,%esi

# qhasm: b ^= a
# asm 1: xor  <a=int64#2,<b=int64#3
# asm 2: xor  <a=%rsi,<b=%rdx
xor  %rsi,%rdx

# qhasm: x0 = b
# asm 1: movq <b=int64#3,>x0=stack64#20
# asm 2: movq <b=%rdx,>x0=152(%rsp)
movq %rdx,152(%rsp)

# qhasm: 		r = x4
# asm 1: movq <x4=stack64#22,>r=int64#2
# asm 2: movq <x4=168(%rsp),>r=%rsi
movq 168(%rsp),%rsi

# qhasm: 		s = x5
# asm 1: movq <x5=stack64#30,>s=int64#3
# asm 2: movq <x5=232(%rsp),>s=%rdx
movq 232(%rsp),%rdx

# qhasm: 		t = x6
# asm 1: movq <x6=stack64#23,>t=int64#4
# asm 2: movq <x6=176(%rsp),>t=%rcx
movq 176(%rsp),%rcx

# qhasm: 		v = r + s
# asm 1: lea  (<r=int64#2,<s=int64#3),>v=int64#6
# asm 2: lea  (<r=%rsi,<s=%rdx),>v=%r9
lea  (%rsi,%rdx),%r9

# qhasm: 		(uint32) v <<<= 7
# asm 1: rol  $7,<v=int64#6d
# asm 2: rol  $7,<v=%r9d
rol  $7,%r9d

# qhasm: 		t ^= v
# asm 1: xor  <v=int64#6,<t=int64#4
# asm 2: xor  <v=%r9,<t=%rcx
xor  %r9,%rcx

# qhasm: 		x6 = t
# asm 1: movq <t=int64#4,>x6=stack64#23
# asm 2: movq <t=%rcx,>x6=176(%rsp)
movq %rcx,176(%rsp)

# qhasm: 		u = x7
# asm 1: movq <x7=stack64#31,>u=int64#6
# asm 2: movq <x7=240(%rsp),>u=%r9
movq 240(%rsp),%r9

# qhasm: 		v = s + t
# asm 1: lea  (<s=int64#3,<t=int64#4),>v=int64#7
# asm 2: lea  (<s=%rdx,<t=%rcx),>v=%rax
lea  (%rdx,%rcx),%rax

# qhasm: 		(uint32) v <<<= 9
# asm 1: rol  $9,<v=int64#7d
# asm 2: rol  $9,<v=%eax
rol  $9,%eax

# qhasm: 		u ^= v
# asm 1: xor  <v=int64#7,<u=int64#6
# asm 2: xor  <v=%rax,<u=%r9
xor  %rax,%r9

# qhasm: 		x7 = u
# asm 1: movq <u=int64#6,>x7=stack64#31
# asm 2: movq <u=%r9,>x7=240(%rsp)
movq %r9,240(%rsp)

# qhasm: 		t += u
# asm 1: add  <u=int64#6,<t=int64#4
# asm 2: add  <u=%r9,<t=%rcx
add  %r9,%rcx

# qhasm: 		(uint32) t <<<= 13
# asm 1: rol  $13,<t=int64#4d
# asm 2: rol  $13,<t=%ecx
rol  $13,%ecx

# qhasm: 		r ^= t
# asm 1: xor  <t=int64#4,<r=int64#2
# asm 2: xor  <t=%rcx,<r=%rsi
xor  %rcx,%rsi

# qhasm: 		x4 = r
# asm 1: movq <r=int64#2,>x4=stack64#22
# asm 2: movq <r=%rsi,>x4=168(%rsp)
movq %rsi,168(%rsp)

# qhasm: 		r += u
# asm 1: add  <u=int64#6,<r=int64#2
# asm 2: add  <u=%r9,<r=%rsi
add  %r9,%rsi

# qhasm: 		(uint32) r <<<= 18
# asm 1: rol  $18,<r=int64#2d
# asm 2: rol  $18,<r=%esi
rol  $18,%esi

# qhasm: 		s ^= r
# asm 1: xor  <r=int64#2,<s=int64#3
# asm 2: xor  <r=%rsi,<s=%rdx
xor  %rsi,%rdx

# qhasm: 		x5 = s
# asm 1: movq <s=int64#3,>x5=stack64#30
# asm 2: movq <s=%rdx,>x5=232(%rsp)
movq %rdx,232(%rsp)

# qhasm: 				A = x9
# asm 1: movq <x9=stack64#32,>A=int64#2
# asm 2: movq <x9=248(%rsp),>A=%rsi
movq 248(%rsp),%rsi

# qhasm: 				B = x10
# asm 1: movq <x10=stack64#25,>B=int64#3
# asm 2: movq <x10=192(%rsp),>B=%rdx
movq 192(%rsp),%rdx

# qhasm: 				C = x11
# asm 1: movq <x11=stack64#33,>C=int64#4
# asm 2: movq <x11=256(%rsp),>C=%rcx
movq 256(%rsp),%rcx

# qhasm: 				E = A + B
# asm 1: lea  (<A=int64#2,<B=int64#3),>E=int64#6
# asm 2: lea  (<A=%rsi,<B=%rdx),>E=%r9
lea  (%rsi,%rdx),%r9

# qhasm: 				(uint32) E <<<= 7
# asm 1: rol  $7,<E=int64#6d
# asm 2: rol  $7,<E=%r9d
rol  $7,%r9d

# qhasm: 				C ^= E
# asm 1: xor  <E=int64#6,<C=int64#4
# asm 2: xor  <E=%r9,<C=%rcx
xor  %r9,%rcx

# qhasm: 				x11 = C
# asm 1: movq <C=int64#4,>x11=stack64#33
# asm 2: movq <C=%rcx,>x11=256(%rsp)
movq %rcx,256(%rsp)

# qhasm: 				D = x8
# asm 1: movq <x8=stack64#24,>D=int64#6
# asm 2: movq <x8=184(%rsp),>D=%r9
movq 184(%rsp),%r9

# qhasm: 				E = B + C
# asm 1: lea  (<B=int64#3,<C=int64#4),>E=int64#7
# asm 2: lea  (<B=%rdx,<C=%rcx),>E=%rax
lea  (%rdx,%rcx),%rax

# qhasm: 				(uint32) E <<<= 9
# asm 1: rol  $9,<E=int64#7d
# asm 2: rol  $9,<E=%eax
rol  $9,%eax

# qhasm: 				D ^= E
# asm 1: xor  <E=int64#7,<D=int64#6
# asm 2: xor  <E=%rax,<D=%r9
xor  %rax,%r9

# qhasm: 				x8 = D
# asm 1: movq <D=int64#6,>x8=stack64#24
# asm 2: movq <D=%r9,>x8=184(%rsp)
movq %r9,184(%rsp)

# qhasm: 				C += D
# asm 1: add  <D=int64#6,<C=int64#4
# asm 2: add  <D=%r9,<C=%rcx
add  %r9,%rcx

# qhasm: 				(uint32) C <<<= 13
# asm 1: rol  $13,<C=int64#4d
# asm 2: rol  $13,<C=%ecx
rol  $13,%ecx

# qhasm: 				A ^= C
# asm 1: xor  <C=int64#4,<A=int64#2
# asm 2: xor  <C=%rcx,<A=%rsi
xor  %rcx,%rsi

# qhasm: 				x9 = A
# asm 1: movq <A=int64#2,>x9=stack64#32
# asm 2: movq <A=%rsi,>x9=248(%rsp)
movq %rsi,248(%rsp)

# qhasm: 				A += D
# asm 1: add  <D=int64#6,<A=int64#2
# asm 2: add  <D=%r9,<A=%rsi
add  %r9,%rsi

# qhasm: 				(uint32) A <<<= 18
# asm 1: rol  $18,<A=int64#2d
# asm 2: rol  $18,<A=%esi
rol  $18,%esi

# qhasm: 				B ^= A
# asm 1: xor  <A=int64#2,<B=int64#3
# asm 2: xor  <A=%rsi,<B=%rdx
xor  %rsi,%rdx

# qhasm: 				x10 = B
# asm 1: movq <B=int64#3,>x10=stack64#25
# asm 2: movq <B=%rdx,>x10=192(%rsp)
movq %rdx,192(%rsp)

# qhasm: 						R = x14
# asm 1: movq <x14=stack64#27,>R=int64#2
# asm 2: movq <x14=208(%rsp),>R=%rsi
movq 208(%rsp),%rsi

# qhasm: 						S = x15
# asm 1: movq <x15=stack64#35,>S=int64#3
# asm 2: movq <x15=272(%rsp),>S=%rdx
movq 272(%rsp),%rdx

# qhasm: 						T = x12
# asm 1: movq <x12=stack64#26,>T=int64#4
# asm 2: movq <x12=200(%rsp),>T=%rcx
movq 200(%rsp),%rcx

# qhasm: 						V = R + S
# asm 1: lea  (<R=int64#2,<S=int64#3),>V=int64#6
# asm 2: lea  (<R=%rsi,<S=%rdx),>V=%r9
lea  (%rsi,%rdx),%r9

# qhasm: 						(uint32) V <<<= 7
# asm 1: rol  $7,<V=int64#6d
# asm 2: rol  $7,<V=%r9d
rol  $7,%r9d

# qhasm: 						T ^= V
# asm 1: xor  <V=int64#6,<T=int64#4
# asm 2: xor  <V=%r9,<T=%rcx
xor  %r9,%rcx

# qhasm: 						x12 = T
# asm 1: movq <T=int64#4,>x12=stack64#26
# asm 2: movq <T=%rcx,>x12=200(%rsp)
movq %rcx,200(%rsp)

# qhasm: 						U = x13
# asm 1: movq <x13=stack64#34,>U=int64#6
# asm 2: movq <x13=264(%rsp),>U=%r9
movq 264(%rsp),%r9

# qhasm: 						V = S + T
# asm 1: lea  (<S=int64#3,<T=int64#4),>V=int64#7
# asm 2: lea  (<S=%rdx,<T=%rcx),>V=%rax
lea  (%rdx,%rcx),%rax

# qhasm: 						(uint32) V <<<= 9
# asm 1: rol  $9,<V=int64#7d
# asm 2: rol  $9,<V=%eax
rol  $9,%eax

# qhasm: 						U ^= V
# asm 1: xor  <V=int64#7,<U=int64#6
# asm 2: xor  <V=%rax,<U=%r9
xor  %rax,%r9

# qhasm: 						x13 = U
# asm 1: movq <U=int64#6,>x13=stack64#34
# asm 2: movq <U=%r9,>x13=264(%rsp)
movq %r9,264(%rsp)

# qhasm: 						T += U
# asm 1: add  <U=int64#6,<T=int64#4
# asm 2: add  <U=%r9,<T=%rcx
add  %r9,%rcx

# qhasm: 						(uint32) T <<<= 13
# asm 1: rol  $13,<T=int64#4d
# asm 2: rol  $13,<T=%ecx
rol  $13,%ecx

# qhasm: 						R ^= T
# asm 1: xor  <T=int64#4,<R=int64#2
# asm 2: xor  <T=%rcx,<R=%rsi
xor  %rcx,%rsi

# qhasm: 						x14 = R
# asm 1: movq <R=int64#2,>x14=stack64#27
# asm 2: movq <R=%rsi,>x14=208(%rsp)
movq %rsi,208(%rsp)

# qhasm: 						R += U
# asm 1: add  <U=int64#6,<R=int64#2
# asm 2: add  <U=%r9,<R=%rsi
add  %r9,%rsi

# qhasm: 						(uint32) R <<<= 18
# asm 1: rol  $18,<R=int64#2d
# asm 2: rol  $18,<R=%esi
rol  $18,%esi

# qhasm: 						S ^= R
# asm 1: xor  <R=int64#2,<S=int64#3
# asm 2: xor  <R=%rsi,<S=%rdx
xor  %rsi,%rdx

# qhasm: 						x15 = S
# asm 1: movq <S=int64#3,>x15=stack64#35
# asm 2: movq <S=%rdx,>x15=272(%rsp)
movq %rdx,272(%rsp)

# qhasm:                  unsigned>? i -= 2
# asm 1: sub  $2,<i=int64#1
# asm 2: sub  $2,<i=%rdi
sub  $2,%rdi
# comment:fp stack unchanged by jump

# qhasm: goto mainloop if unsigned>
ja ._mainloop

# qhasm:   out = out_backup
# asm 1: movq <out_backup=stack64#17,>out=int64#1
# asm 2: movq <out_backup=128(%rsp),>out=%rdi
movq 128(%rsp),%rdi

# qhasm:   m = m_backup
# asm 1: movq <m_backup=stack64#18,>m=int64#2
# asm 2: movq <m_backup=136(%rsp),>m=%rsi
movq 136(%rsp),%rsi

# qhasm:   in0 = j0
# asm 1: movq <j0=stack64#8,>in0=int64#3
# asm 2: movq <j0=56(%rsp),>in0=%rdx
movq 56(%rsp),%rdx

# qhasm:   out0 = in0
# asm 1: mov  <in0=int64#3,>out0=int64#4
# asm 2: mov  <in0=%rdx,>out0=%rcx
mov  %rdx,%rcx

# qhasm:   (uint64) out0 >>= 32
# asm 1: shr  $32,<out0=int64#4
# asm 2: shr  $32,<out0=%rcx
shr  $32,%rcx

# qhasm:   (uint32) in0 += x0
# asm 1: addl <x0=stack64#20,<in0=int64#3d
# asm 2: addl <x0=152(%rsp),<in0=%edx
addl 152(%rsp),%edx

# qhasm:   (uint32) out0 += x1
# asm 1: addl <x1=stack64#28,<out0=int64#4d
# asm 2: addl <x1=216(%rsp),<out0=%ecx
addl 216(%rsp),%ecx

# qhasm:   out0 <<= 32
# asm 1: shl  $32,<out0=int64#4
# asm 2: shl  $32,<out0=%rcx
shl  $32,%rcx

# qhasm:   out0 ^= in0
# asm 1: xor  <in0=int64#3,<out0=int64#4
# asm 2: xor  <in0=%rdx,<out0=%rcx
xor  %rdx,%rcx

# qhasm:   out0 ^= *(uint64 *) (m + 0)
# asm 1: xorq 0(<m=int64#2),<out0=int64#4
# asm 2: xorq 0(<m=%rsi),<out0=%rcx
xorq 0(%rsi),%rcx

# qhasm:   *(uint64 *) (out + 0) = out0
# asm 1: movq   <out0=int64#4,0(<out=int64#1)
# asm 2: movq   <out0=%rcx,0(<out=%rdi)
movq   %rcx,0(%rdi)

# qhasm:   in2 = j2
# asm 1: movq <j2=stack64#9,>in2=int64#3
# asm 2: movq <j2=64(%rsp),>in2=%rdx
movq 64(%rsp),%rdx

# qhasm:   out2 = in2
# asm 1: mov  <in2=int64#3,>out2=int64#4
# asm 2: mov  <in2=%rdx,>out2=%rcx
mov  %rdx,%rcx

# qhasm:   (uint64) out2 >>= 32
# asm 1: shr  $32,<out2=int64#4
# asm 2: shr  $32,<out2=%rcx
shr  $32,%rcx

# qhasm:   (uint32) in2 += x2
# asm 1: addl <x2=stack64#21,<in2=int64#3d
# asm 2: addl <x2=160(%rsp),<in2=%edx
addl 160(%rsp),%edx

# qhasm:   (uint32) out2 += x3
# asm 1: addl <x3=stack64#29,<out2=int64#4d
# asm 2: addl <x3=224(%rsp),<out2=%ecx
addl 224(%rsp),%ecx

# qhasm:   out2 <<= 32
# asm 1: shl  $32,<out2=int64#4
# asm 2: shl  $32,<out2=%rcx
shl  $32,%rcx

# qhasm:   out2 ^= in2
# asm 1: xor  <in2=int64#3,<out2=int64#4
# asm 2: xor  <in2=%rdx,<out2=%rcx
xor  %rdx,%rcx

# qhasm:   out2 ^= *(uint64 *) (m + 8)
# asm 1: xorq 8(<m=int64#2),<out2=int64#4
# asm 2: xorq 8(<m=%rsi),<out2=%rcx
xorq 8(%rsi),%rcx

# qhasm:   *(uint64 *) (out + 8) = out2
# asm 1: movq   <out2=int64#4,8(<out=int64#1)
# asm 2: movq   <out2=%rcx,8(<out=%rdi)
movq   %rcx,8(%rdi)

# qhasm:   in4 = j4
# asm 1: movq <j4=stack64#10,>in4=int64#3
# asm 2: movq <j4=72(%rsp),>in4=%rdx
movq 72(%rsp),%rdx

# qhasm:   out4 = in4
# asm 1: mov  <in4=int64#3,>out4=int64#4
# asm 2: mov  <in4=%rdx,>out4=%rcx
mov  %rdx,%rcx

# qhasm:   (uint64) out4 >>= 32
# asm 1: shr  $32,<out4=int64#4
# asm 2: shr  $32,<out4=%rcx
shr  $32,%rcx

# qhasm:   (uint32) in4 += x4
# asm 1: addl <x4=stack64#22,<in4=int64#3d
# asm 2: addl <x4=168(%rsp),<in4=%edx
addl 168(%rsp),%edx

# qhasm:   (uint32) out4 += x5
# asm 1: addl <x5=stack64#30,<out4=int64#4d
# asm 2: addl <x5=232(%rsp),<out4=%ecx
addl 232(%rsp),%ecx

# qhasm:   out4 <<= 32
# asm 1: shl  $32,<out4=int64#4
# asm 2: shl  $32,<out4=%rcx
shl  $32,%rcx

# qhasm:   out4 ^= in4
# asm 1: xor  <in4=int64#3,<out4=int64#4
# asm 2: xor  <in4=%rdx,<out4=%rcx
xor  %rdx,%rcx

# qhasm:   out4 ^= *(uint64 *) (m + 16)
# asm 1: xorq 16(<m=int64#2),<out4=int64#4
# asm 2: xorq 16(<m=%rsi),<out4=%rcx
xorq 16(%rsi),%rcx

# qhasm:   *(uint64 *) (out + 16) = out4
# asm 1: movq   <out4=int64#4,16(<out=int64#1)
# asm 2: movq   <out4=%rcx,16(<out=%rdi)
movq   %rcx,16(%rdi)

# qhasm:   in6 = j6
# asm 1: movq <j6=stack64#11,>in6=int64#3
# asm 2: movq <j6=80(%rsp),>in6=%rdx
movq 80(%rsp),%rdx

# qhasm:   out6 = in6
# asm 1: mov  <in6=int64#3,>out6=int64#4
# asm 2: mov  <in6=%rdx,>out6=%rcx
mov  %rdx,%rcx

# qhasm:   (uint64) out6 >>= 32
# asm 1: shr  $32,<out6=int64#4
# asm 2: shr  $32,<out6=%rcx
shr  $32,%rcx

# qhasm:   (uint32) in6 += x6
# asm 1: addl <x6=stack64#23,<in6=int64#3d
# asm 2: addl <x6=176(%rsp),<in6=%edx
addl 176(%rsp),%edx

# qhasm:   (uint32) out6 += x7
# asm 1: addl <x7=stack64#31,<out6=int64#4d
# asm 2: addl <x7=240(%rsp),<out6=%ecx
addl 240(%rsp),%ecx

# qhasm:   out6 <<= 32
# asm 1: shl  $32,<out6=int64#4
# asm 2: shl  $32,<out6=%rcx
shl  $32,%rcx

# qhasm:   out6 ^= in6
# asm 1: xor  <in6=int64#3,<out6=int64#4
# asm 2: xor  <in6=%rdx,<out6=%rcx
xor  %rdx,%rcx

# qhasm:   out6 ^= *(uint64 *) (m + 24)
# asm 1: xorq 24(<m=int64#2),<out6=int64#4
# asm 2: xorq 24(<m=%rsi),<out6=%rcx
xorq 24(%rsi),%rcx

# qhasm:   *(uint64 *) (out + 24) = out6
# asm 1: movq   <out6=int64#4,24(<out=int64#1)
# asm 2: movq   <out6=%rcx,24(<out=%rdi)
movq   %rcx,24(%rdi)

# qhasm:   in8 = j8
# asm 1: movq <j8=stack64#12,>in8=int64#3
# asm 2: movq <j8=88(%rsp),>in8=%rdx
movq 88(%rsp),%rdx

# qhasm:   out8 = in8
# asm 1: mov  <in8=int64#3,>out8=int64#4
# asm 2: mov  <in8=%rdx,>out8=%rcx
mov  %rdx,%rcx

# qhasm:   (uint64) out8 >>= 32
# asm 1: shr  $32,<out8=int64#4
# asm 2: shr  $32,<out8=%rcx
shr  $32,%rcx

# qhasm:   (uint32) in8 += x8
# asm 1: addl <x8=stack64#24,<in8=int64#3d
# asm 2: addl <x8=184(%rsp),<in8=%edx
addl 184(%rsp),%edx

# qhasm:   (uint32) out8 += x9
# asm 1: addl <x9=stack64#32,<out8=int64#4d
# asm 2: addl <x9=248(%rsp),<out8=%ecx
addl 248(%rsp),%ecx

# qhasm:   out8 <<= 32
# asm 1: shl  $32,<out8=int64#4
# asm 2: shl  $32,<out8=%rcx
shl  $32,%rcx

# qhasm:   out8 ^= in8
# asm 1: xor  <in8=int64#3,<out8=int64#4
# asm 2: xor  <in8=%rdx,<out8=%rcx
xor  %rdx,%rcx

# qhasm:   out8 ^= *(uint64 *) (m + 32)
# asm 1: xorq 32(<m=int64#2),<out8=int64#4
# asm 2: xorq 32(<m=%rsi),<out8=%rcx
xorq 32(%rsi),%rcx

# qhasm:   *(uint64 *) (out + 32) = out8
# asm 1: movq   <out8=int64#4,32(<out=int64#1)
# asm 2: movq   <out8=%rcx,32(<out=%rdi)
movq   %rcx,32(%rdi)

# qhasm:   in10 = j10
# asm 1: movq <j10=stack64#13,>in10=int64#3
# asm 2: movq <j10=96(%rsp),>in10=%rdx
movq 96(%rsp),%rdx

# qhasm:   out10 = in10
# asm 1: mov  <in10=int64#3,>out10=int64#4
# asm 2: mov  <in10=%rdx,>out10=%rcx
mov  %rdx,%rcx

# qhasm:   (uint64) out10 >>= 32
# asm 1: shr  $32,<out10=int64#4
# asm 2: shr  $32,<out10=%rcx
shr  $32,%rcx

# qhasm:   (uint32) in10 += x10
# asm 1: addl <x10=stack64#25,<in10=int64#3d
# asm 2: addl <x10=192(%rsp),<in10=%edx
addl 192(%rsp),%edx

# qhasm:   (uint32) out10 += x11
# asm 1: addl <x11=stack64#33,<out10=int64#4d
# asm 2: addl <x11=256(%rsp),<out10=%ecx
addl 256(%rsp),%ecx

# qhasm:   out10 <<= 32
# asm 1: shl  $32,<out10=int64#4
# asm 2: shl  $32,<out10=%rcx
shl  $32,%rcx

# qhasm:   out10 ^= in10
# asm 1: xor  <in10=int64#3,<out10=int64#4
# asm 2: xor  <in10=%rdx,<out10=%rcx
xor  %rdx,%rcx

# qhasm:   out10 ^= *(uint64 *) (m + 40)
# asm 1: xorq 40(<m=int64#2),<out10=int64#4
# asm 2: xorq 40(<m=%rsi),<out10=%rcx
xorq 40(%rsi),%rcx

# qhasm:   *(uint64 *) (out + 40) = out10
# asm 1: movq   <out10=int64#4,40(<out=int64#1)
# asm 2: movq   <out10=%rcx,40(<out=%rdi)
movq   %rcx,40(%rdi)

# qhasm:   in12 = j12
# asm 1: movq <j12=stack64#14,>in12=int64#3
# asm 2: movq <j12=104(%rsp),>in12=%rdx
movq 104(%rsp),%rdx

# qhasm:   out12 = in12
# asm 1: mov  <in12=int64#3,>out12=int64#4
# asm 2: mov  <in12=%rdx,>out12=%rcx
mov  %rdx,%rcx

# qhasm:   (uint64) out12 >>= 32
# asm 1: shr  $32,<out12=int64#4
# asm 2: shr  $32,<out12=%rcx
shr  $32,%rcx

# qhasm:   (uint32) in12 += x12
# asm 1: addl <x12=stack64#26,<in12=int64#3d
# asm 2: addl <x12=200(%rsp),<in12=%edx
addl 200(%rsp),%edx

# qhasm:   (uint32) out12 += x13
# asm 1: addl <x13=stack64#34,<out12=int64#4d
# asm 2: addl <x13=264(%rsp),<out12=%ecx
addl 264(%rsp),%ecx

# qhasm:   out12 <<= 32
# asm 1: shl  $32,<out12=int64#4
# asm 2: shl  $32,<out12=%rcx
shl  $32,%rcx

# qhasm:   out12 ^= in12
# asm 1: xor  <in12=int64#3,<out12=int64#4
# asm 2: xor  <in12=%rdx,<out12=%rcx
xor  %rdx,%rcx

# qhasm:   out12 ^= *(uint64 *) (m + 48)
# asm 1: xorq 48(<m=int64#2),<out12=int64#4
# asm 2: xorq 48(<m=%rsi),<out12=%rcx
xorq 48(%rsi),%rcx

# qhasm:   *(uint64 *) (out + 48) = out12
# asm 1: movq   <out12=int64#4,48(<out=int64#1)
# asm 2: movq   <out12=%rcx,48(<out=%rdi)
movq   %rcx,48(%rdi)

# qhasm:   in14 = j14
# asm 1: movq <j14=stack64#15,>in14=int64#3
# asm 2: movq <j14=112(%rsp),>in14=%rdx
movq 112(%rsp),%rdx

# qhasm:   out14 = in14
# asm 1: mov  <in14=int64#3,>out14=int64#4
# asm 2: mov  <in14=%rdx,>out14=%rcx
mov  %rdx,%rcx

# qhasm:   (uint64) out14 >>= 32
# asm 1: shr  $32,<out14=int64#4
# asm 2: shr  $32,<out14=%rcx
shr  $32,%rcx

# qhasm:   (uint32) in14 += x14
# asm 1: addl <x14=stack64#27,<in14=int64#3d
# asm 2: addl <x14=208(%rsp),<in14=%edx
addl 208(%rsp),%edx

# qhasm:   (uint32) out14 += x15
# asm 1: addl <x15=stack64#35,<out14=int64#4d
# asm 2: addl <x15=272(%rsp),<out14=%ecx
addl 272(%rsp),%ecx

# qhasm:   out14 <<= 32
# asm 1: shl  $32,<out14=int64#4
# asm 2: shl  $32,<out14=%rcx
shl  $32,%rcx

# qhasm:   out14 ^= in14
# asm 1: xor  <in14=int64#3,<out14=int64#4
# asm 2: xor  <in14=%rdx,<out14=%rcx
xor  %rdx,%rcx

# qhasm:   out14 ^= *(uint64 *) (m + 56)
# asm 1: xorq 56(<m=int64#2),<out14=int64#4
# asm 2: xorq 56(<m=%rsi),<out14=%rcx
xorq 56(%rsi),%rcx

# qhasm:   *(uint64 *) (out + 56) = out14
# asm 1: movq   <out14=int64#4,56(<out=int64#1)
# asm 2: movq   <out14=%rcx,56(<out=%rdi)
movq   %rcx,56(%rdi)

# qhasm:   bytes = bytes_backup
# asm 1: movq <bytes_backup=stack64#19,>bytes=int64#3
# asm 2: movq <bytes_backup=144(%rsp),>bytes=%rdx
movq 144(%rsp),%rdx

# qhasm:   in8 = j8
# asm 1: movq <j8=stack64#12,>in8=int64#4
# asm 2: movq <j8=88(%rsp),>in8=%rcx
movq 88(%rsp),%rcx

# qhasm:   in8 += 1
# asm 1: add  $1,<in8=int64#4
# asm 2: add  $1,<in8=%rcx
add  $1,%rcx

# qhasm:   j8 = in8
# asm 1: movq <in8=int64#4,>j8=stack64#12
# asm 2: movq <in8=%rcx,>j8=88(%rsp)
movq %rcx,88(%rsp)

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
# asm 1: mov  <ctarget=int64#5,>out=int64#1
# asm 2: mov  <ctarget=%r8,>out=%rdi
mov  %r8,%rdi

# qhasm:       i = bytes
# asm 1: mov  <bytes=int64#3,>i=int64#4
# asm 2: mov  <bytes=%rdx,>i=%rcx
mov  %rdx,%rcx

# qhasm:       while (i) { *out++ = *m++; --i }
rep movsb
# comment:fp stack unchanged by fallthrough

# qhasm:     bytesatleast64:
._bytesatleast64:

# qhasm:     x = x_backup
# asm 1: movq <x_backup=stack64#16,>x=int64#1
# asm 2: movq <x_backup=120(%rsp),>x=%rdi
movq 120(%rsp),%rdi

# qhasm:     in8 = j8
# asm 1: movq <j8=stack64#12,>in8=int64#2
# asm 2: movq <j8=88(%rsp),>in8=%rsi
movq 88(%rsp),%rsi

# qhasm:     *(uint64 *) (x + 32) = in8
# asm 1: movq   <in8=int64#2,32(<x=int64#1)
# asm 2: movq   <in8=%rsi,32(<x=%rdi)
movq   %rsi,32(%rdi)

# qhasm:     r11 = r11_stack
# asm 1: movq <r11_stack=stack64#1,>r11=int64#9
# asm 2: movq <r11_stack=0(%rsp),>r11=%r11
movq 0(%rsp),%r11

# qhasm:     r12 = r12_stack
# asm 1: movq <r12_stack=stack64#2,>r12=int64#10
# asm 2: movq <r12_stack=8(%rsp),>r12=%r12
movq 8(%rsp),%r12

# qhasm:     r13 = r13_stack
# asm 1: movq <r13_stack=stack64#3,>r13=int64#11
# asm 2: movq <r13_stack=16(%rsp),>r13=%r13
movq 16(%rsp),%r13

# qhasm:     r14 = r14_stack
# asm 1: movq <r14_stack=stack64#4,>r14=int64#12
# asm 2: movq <r14_stack=24(%rsp),>r14=%r14
movq 24(%rsp),%r14

# qhasm:     r15 = r15_stack
# asm 1: movq <r15_stack=stack64#5,>r15=int64#13
# asm 2: movq <r15_stack=32(%rsp),>r15=%r15
movq 32(%rsp),%r15

# qhasm:     rbx = rbx_stack
# asm 1: movq <rbx_stack=stack64#6,>rbx=int64#14
# asm 2: movq <rbx_stack=40(%rsp),>rbx=%rbx
movq 40(%rsp),%rbx

# qhasm:     rbp = rbp_stack
# asm 1: movq <rbp_stack=stack64#7,>rbp=int64#15
# asm 2: movq <rbp_stack=48(%rsp),>rbp=%rbp
movq 48(%rsp),%rbp
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
add $352,%r11
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
add $352,%r11
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

# qhasm:   in0 = *(uint64 *) (k + 0)
# asm 1: movq   0(<k=int64#2),>in0=int64#5
# asm 2: movq   0(<k=%rsi),>in0=%r8
movq   0(%rsi),%r8

# qhasm:   in2 = *(uint64 *) (k + 8)
# asm 1: movq   8(<k=int64#2),>in2=int64#6
# asm 2: movq   8(<k=%rsi),>in2=%r9
movq   8(%rsi),%r9

# qhasm:   *(uint64 *) (x + 4) = in0
# asm 1: movq   <in0=int64#5,4(<x=int64#1)
# asm 2: movq   <in0=%r8,4(<x=%rdi)
movq   %r8,4(%rdi)

# qhasm:   *(uint64 *) (x + 12) = in2
# asm 1: movq   <in2=int64#6,12(<x=int64#1)
# asm 2: movq   <in2=%r9,12(<x=%rdi)
movq   %r9,12(%rdi)

# qhasm:                    unsigned<? kbits - 256
# asm 1: cmp  $256,<kbits=int64#3
# asm 2: cmp  $256,<kbits=%rdx
cmp  $256,%rdx
# comment:fp stack unchanged by jump

# qhasm:   goto kbits128 if unsigned<
jb ._kbits128

# qhasm:   kbits256:
._kbits256:

# qhasm:     in10 = *(uint64 *) (k + 16)
# asm 1: movq   16(<k=int64#2),>in10=int64#3
# asm 2: movq   16(<k=%rsi),>in10=%rdx
movq   16(%rsi),%rdx

# qhasm:     in12 = *(uint64 *) (k + 24)
# asm 1: movq   24(<k=int64#2),>in12=int64#2
# asm 2: movq   24(<k=%rsi),>in12=%rsi
movq   24(%rsi),%rsi

# qhasm:     *(uint64 *) (x + 44) = in10
# asm 1: movq   <in10=int64#3,44(<x=int64#1)
# asm 2: movq   <in10=%rdx,44(<x=%rdi)
movq   %rdx,44(%rdi)

# qhasm:     *(uint64 *) (x + 52) = in12
# asm 1: movq   <in12=int64#2,52(<x=int64#1)
# asm 2: movq   <in12=%rsi,52(<x=%rdi)
movq   %rsi,52(%rdi)

# qhasm:     in0 = 1634760805
# asm 1: mov  $1634760805,>in0=int64#2
# asm 2: mov  $1634760805,>in0=%rsi
mov  $1634760805,%rsi

# qhasm:     in4 = 857760878
# asm 1: mov  $857760878,>in4=int64#3
# asm 2: mov  $857760878,>in4=%rdx
mov  $857760878,%rdx

# qhasm:     in10 = 2036477234
# asm 1: mov  $2036477234,>in10=int64#4
# asm 2: mov  $2036477234,>in10=%rcx
mov  $2036477234,%rcx

# qhasm:     in14 = 1797285236
# asm 1: mov  $1797285236,>in14=int64#5
# asm 2: mov  $1797285236,>in14=%r8
mov  $1797285236,%r8

# qhasm:     *(uint32 *) (x + 0) = in0
# asm 1: movl   <in0=int64#2d,0(<x=int64#1)
# asm 2: movl   <in0=%esi,0(<x=%rdi)
movl   %esi,0(%rdi)

# qhasm:     *(uint32 *) (x + 20) = in4
# asm 1: movl   <in4=int64#3d,20(<x=int64#1)
# asm 2: movl   <in4=%edx,20(<x=%rdi)
movl   %edx,20(%rdi)

# qhasm:     *(uint32 *) (x + 40) = in10
# asm 1: movl   <in10=int64#4d,40(<x=int64#1)
# asm 2: movl   <in10=%ecx,40(<x=%rdi)
movl   %ecx,40(%rdi)

# qhasm:     *(uint32 *) (x + 60) = in14
# asm 1: movl   <in14=int64#5d,60(<x=int64#1)
# asm 2: movl   <in14=%r8d,60(<x=%rdi)
movl   %r8d,60(%rdi)
# comment:fp stack unchanged by jump

# qhasm:   goto keysetupdone
jmp ._keysetupdone

# qhasm:   kbits128:
._kbits128:

# qhasm:     in10 = *(uint64 *) (k + 0)
# asm 1: movq   0(<k=int64#2),>in10=int64#3
# asm 2: movq   0(<k=%rsi),>in10=%rdx
movq   0(%rsi),%rdx

# qhasm:     in12 = *(uint64 *) (k + 8)
# asm 1: movq   8(<k=int64#2),>in12=int64#2
# asm 2: movq   8(<k=%rsi),>in12=%rsi
movq   8(%rsi),%rsi

# qhasm:     *(uint64 *) (x + 44) = in10
# asm 1: movq   <in10=int64#3,44(<x=int64#1)
# asm 2: movq   <in10=%rdx,44(<x=%rdi)
movq   %rdx,44(%rdi)

# qhasm:     *(uint64 *) (x + 52) = in12
# asm 1: movq   <in12=int64#2,52(<x=int64#1)
# asm 2: movq   <in12=%rsi,52(<x=%rdi)
movq   %rsi,52(%rdi)

# qhasm:     in0 = 1634760805
# asm 1: mov  $1634760805,>in0=int64#2
# asm 2: mov  $1634760805,>in0=%rsi
mov  $1634760805,%rsi

# qhasm:     in4 = 824206446
# asm 1: mov  $824206446,>in4=int64#3
# asm 2: mov  $824206446,>in4=%rdx
mov  $824206446,%rdx

# qhasm:     in10 = 2036477238
# asm 1: mov  $2036477238,>in10=int64#4
# asm 2: mov  $2036477238,>in10=%rcx
mov  $2036477238,%rcx

# qhasm:     in14 = 1797285236
# asm 1: mov  $1797285236,>in14=int64#5
# asm 2: mov  $1797285236,>in14=%r8
mov  $1797285236,%r8

# qhasm:     *(uint32 *) (x + 0) = in0
# asm 1: movl   <in0=int64#2d,0(<x=int64#1)
# asm 2: movl   <in0=%esi,0(<x=%rdi)
movl   %esi,0(%rdi)

# qhasm:     *(uint32 *) (x + 20) = in4
# asm 1: movl   <in4=int64#3d,20(<x=int64#1)
# asm 2: movl   <in4=%edx,20(<x=%rdi)
movl   %edx,20(%rdi)

# qhasm:     *(uint32 *) (x + 40) = in10
# asm 1: movl   <in10=int64#4d,40(<x=int64#1)
# asm 2: movl   <in10=%ecx,40(<x=%rdi)
movl   %ecx,40(%rdi)

# qhasm:     *(uint32 *) (x + 60) = in14
# asm 1: movl   <in14=int64#5d,60(<x=int64#1)
# asm 2: movl   <in14=%r8d,60(<x=%rdi)
movl   %r8d,60(%rdi)

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
add $352,%r11
sub %r11,%rsp

# qhasm:   iv = arg2
# asm 1: mov  <arg2=int64#2,>iv=int64#2
# asm 2: mov  <arg2=%rsi,>iv=%rsi
mov  %rsi,%rsi

# qhasm:   x = arg1
# asm 1: mov  <arg1=int64#1,>x=int64#1
# asm 2: mov  <arg1=%rdi,>x=%rdi
mov  %rdi,%rdi

# qhasm:   in6 = *(uint64 *) (iv + 0)
# asm 1: movq   0(<iv=int64#2),>in6=int64#2
# asm 2: movq   0(<iv=%rsi),>in6=%rsi
movq   0(%rsi),%rsi

# qhasm:   in8 = 0
# asm 1: mov  $0,>in8=int64#5
# asm 2: mov  $0,>in8=%r8
mov  $0,%r8

# qhasm:   *(uint64 *) (x + 24) = in6
# asm 1: movq   <in6=int64#2,24(<x=int64#1)
# asm 2: movq   <in6=%rsi,24(<x=%rdi)
movq   %rsi,24(%rdi)

# qhasm:   *(uint64 *) (x + 32) = in8
# asm 1: movq   <in8=int64#5,32(<x=int64#1)
# asm 2: movq   <in8=%r8,32(<x=%rdi)
movq   %r8,32(%rdi)

# qhasm: leave
add %r11,%rsp
mov %rdi,%rax
mov %rsi,%rdx
ret
