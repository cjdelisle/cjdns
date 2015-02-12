
# qhasm: int64 a

# qhasm: int64 b

# qhasm: int64 c

# qhasm: int64 d

# qhasm: int64 zero

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

# qhasm: stack64 i_backup

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

# qhasm: int64 x0

# qhasm: int64 x1

# qhasm: int64 x2

# qhasm: int64 x3

# qhasm: int64 x4

# qhasm: int64 x5

# qhasm: int64 x6

# qhasm: int64 x7

# qhasm: int64 x8

# qhasm: int64 x9

# qhasm: int64 x10

# qhasm: int64 x11

# qhasm: int64 x12

# qhasm: int64 x13

# qhasm: int64 x14

# qhasm: int64 x15

# qhasm: stack64 x5_stack

# qhasm: stack64 x10_stack

# qhasm: stack64 x15_stack

# qhasm: stack64 j0

# qhasm: stack64 j2

# qhasm: stack64 j4

# qhasm: stack64 j6

# qhasm: stack64 j8

# qhasm: stack64 j10

# qhasm: stack64 j12

# qhasm: stack64 j14

# qhasm: stack512 tmp

# qhasm: stack64 ctarget

# qhasm: enter ECRYPT_keystream_bytes
.text
.p2align 5
.globl _ECRYPT_keystream_bytes
.globl ECRYPT_keystream_bytes
_ECRYPT_keystream_bytes:
ECRYPT_keystream_bytes:
mov %rsp,%r11
and $31,%r11
add $256,%r11
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

# qhasm: zero = 0
# asm 1: mov  $0,>zero=int64#7
# asm 2: mov  $0,>zero=%rax
mov  $0,%rax

# qhasm: i = bytes
# asm 1: mov  <bytes=int64#3,>i=int64#4
# asm 2: mov  <bytes=%rdx,>i=%rcx
mov  %rdx,%rcx

# qhasm: while (i) { *out++ = zero; --i }
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
add $256,%r11
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
add $256,%r11
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
# asm 1: movq <out=int64#1,>ctarget=stack64#17
# asm 2: movq <out=%rdi,>ctarget=128(%rsp)
movq %rdi,128(%rsp)

# qhasm:     out = &tmp
# asm 1: leaq <tmp=stack512#1,>out=int64#1
# asm 2: leaq <tmp=192(%rsp),>out=%rdi
leaq 192(%rsp),%rdi

# qhasm:     i = bytes
# asm 1: mov  <bytes=int64#3,>i=int64#4
# asm 2: mov  <bytes=%rdx,>i=%rcx
mov  %rdx,%rcx

# qhasm:     while (i) { *out++ = *m++; --i }
rep movsb

# qhasm:     out = &tmp
# asm 1: leaq <tmp=stack512#1,>out=int64#1
# asm 2: leaq <tmp=192(%rsp),>out=%rdi
leaq 192(%rsp),%rdi

# qhasm:     m = &tmp
# asm 1: leaq <tmp=stack512#1,>m=int64#2
# asm 2: leaq <tmp=192(%rsp),>m=%rsi
leaq 192(%rsp),%rsi
# comment:fp stack unchanged by fallthrough

# qhasm:   nocopy:
._nocopy:

# qhasm:   out_backup = out
# asm 1: movq <out=int64#1,>out_backup=stack64#18
# asm 2: movq <out=%rdi,>out_backup=136(%rsp)
movq %rdi,136(%rsp)

# qhasm:   m_backup = m
# asm 1: movq <m=int64#2,>m_backup=stack64#19
# asm 2: movq <m=%rsi,>m_backup=144(%rsp)
movq %rsi,144(%rsp)

# qhasm:   bytes_backup = bytes
# asm 1: movq <bytes=int64#3,>bytes_backup=stack64#20
# asm 2: movq <bytes=%rdx,>bytes_backup=152(%rsp)
movq %rdx,152(%rsp)

# qhasm:   x1 = j0
# asm 1: movq <j0=stack64#8,>x1=int64#1
# asm 2: movq <j0=56(%rsp),>x1=%rdi
movq 56(%rsp),%rdi

# qhasm:   x0 = x1
# asm 1: mov  <x1=int64#1,>x0=int64#3
# asm 2: mov  <x1=%rdi,>x0=%rdx
mov  %rdi,%rdx

# qhasm:   (uint64) x1 >>= 32
# asm 1: shr  $32,<x1=int64#1
# asm 2: shr  $32,<x1=%rdi
shr  $32,%rdi

# qhasm:   		x3 = j2
# asm 1: movq <j2=stack64#9,>x3=int64#2
# asm 2: movq <j2=64(%rsp),>x3=%rsi
movq 64(%rsp),%rsi

# qhasm:   		x2 = x3
# asm 1: mov  <x3=int64#2,>x2=int64#4
# asm 2: mov  <x3=%rsi,>x2=%rcx
mov  %rsi,%rcx

# qhasm:   		(uint64) x3 >>= 32
# asm 1: shr  $32,<x3=int64#2
# asm 2: shr  $32,<x3=%rsi
shr  $32,%rsi

# qhasm:   x5 = j4
# asm 1: movq <j4=stack64#10,>x5=int64#5
# asm 2: movq <j4=72(%rsp),>x5=%r8
movq 72(%rsp),%r8

# qhasm:   x4 = x5
# asm 1: mov  <x5=int64#5,>x4=int64#6
# asm 2: mov  <x5=%r8,>x4=%r9
mov  %r8,%r9

# qhasm:   (uint64) x5 >>= 32
# asm 1: shr  $32,<x5=int64#5
# asm 2: shr  $32,<x5=%r8
shr  $32,%r8

# qhasm:   x5_stack = x5
# asm 1: movq <x5=int64#5,>x5_stack=stack64#21
# asm 2: movq <x5=%r8,>x5_stack=160(%rsp)
movq %r8,160(%rsp)

# qhasm:   		x7 = j6
# asm 1: movq <j6=stack64#11,>x7=int64#5
# asm 2: movq <j6=80(%rsp),>x7=%r8
movq 80(%rsp),%r8

# qhasm:   		x6 = x7
# asm 1: mov  <x7=int64#5,>x6=int64#7
# asm 2: mov  <x7=%r8,>x6=%rax
mov  %r8,%rax

# qhasm:   		(uint64) x7 >>= 32
# asm 1: shr  $32,<x7=int64#5
# asm 2: shr  $32,<x7=%r8
shr  $32,%r8

# qhasm:   x9 = j8
# asm 1: movq <j8=stack64#12,>x9=int64#8
# asm 2: movq <j8=88(%rsp),>x9=%r10
movq 88(%rsp),%r10

# qhasm:   x8 = x9
# asm 1: mov  <x9=int64#8,>x8=int64#9
# asm 2: mov  <x9=%r10,>x8=%r11
mov  %r10,%r11

# qhasm:   (uint64) x9 >>= 32
# asm 1: shr  $32,<x9=int64#8
# asm 2: shr  $32,<x9=%r10
shr  $32,%r10

# qhasm:   		x11 = j10
# asm 1: movq <j10=stack64#13,>x11=int64#10
# asm 2: movq <j10=96(%rsp),>x11=%r12
movq 96(%rsp),%r12

# qhasm:   		x10 = x11
# asm 1: mov  <x11=int64#10,>x10=int64#11
# asm 2: mov  <x11=%r12,>x10=%r13
mov  %r12,%r13

# qhasm:   		x10_stack = x10
# asm 1: movq <x10=int64#11,>x10_stack=stack64#22
# asm 2: movq <x10=%r13,>x10_stack=168(%rsp)
movq %r13,168(%rsp)

# qhasm:   		(uint64) x11 >>= 32
# asm 1: shr  $32,<x11=int64#10
# asm 2: shr  $32,<x11=%r12
shr  $32,%r12

# qhasm:   x13 = j12
# asm 1: movq <j12=stack64#14,>x13=int64#11
# asm 2: movq <j12=104(%rsp),>x13=%r13
movq 104(%rsp),%r13

# qhasm:   x12 = x13
# asm 1: mov  <x13=int64#11,>x12=int64#12
# asm 2: mov  <x13=%r13,>x12=%r14
mov  %r13,%r14

# qhasm:   (uint64) x13 >>= 32
# asm 1: shr  $32,<x13=int64#11
# asm 2: shr  $32,<x13=%r13
shr  $32,%r13

# qhasm:   		x15 = j14
# asm 1: movq <j14=stack64#15,>x15=int64#13
# asm 2: movq <j14=112(%rsp),>x15=%r15
movq 112(%rsp),%r15

# qhasm:   		x14 = x15
# asm 1: mov  <x15=int64#13,>x14=int64#14
# asm 2: mov  <x15=%r15,>x14=%rbx
mov  %r15,%rbx

# qhasm:   		(uint64) x15 >>= 32
# asm 1: shr  $32,<x15=int64#13
# asm 2: shr  $32,<x15=%r15
shr  $32,%r15

# qhasm:   		x15_stack = x15
# asm 1: movq <x15=int64#13,>x15_stack=stack64#23
# asm 2: movq <x15=%r15,>x15_stack=176(%rsp)
movq %r15,176(%rsp)

# qhasm:   i = 20
# asm 1: mov  $20,>i=int64#13
# asm 2: mov  $20,>i=%r15
mov  $20,%r15

# qhasm:   mainloop:
._mainloop:

# qhasm:   i_backup = i
# asm 1: movq <i=int64#13,>i_backup=stack64#24
# asm 2: movq <i=%r15,>i_backup=184(%rsp)
movq %r15,184(%rsp)

# qhasm: 		x5 = x5_stack
# asm 1: movq <x5_stack=stack64#21,>x5=int64#13
# asm 2: movq <x5_stack=160(%rsp),>x5=%r15
movq 160(%rsp),%r15

# qhasm: a = x12 + x0
# asm 1: lea  (<x12=int64#12,<x0=int64#3),>a=int64#15
# asm 2: lea  (<x12=%r14,<x0=%rdx),>a=%rbp
lea  (%r14,%rdx),%rbp

# qhasm: (uint32) a <<<= 7
# asm 1: rol  $7,<a=int64#15d
# asm 2: rol  $7,<a=%ebp
rol  $7,%ebp

# qhasm: x4 ^= a
# asm 1: xor  <a=int64#15,<x4=int64#6
# asm 2: xor  <a=%rbp,<x4=%r9
xor  %rbp,%r9

# qhasm: 		b = x1 + x5
# asm 1: lea  (<x1=int64#1,<x5=int64#13),>b=int64#15
# asm 2: lea  (<x1=%rdi,<x5=%r15),>b=%rbp
lea  (%rdi,%r15),%rbp

# qhasm: 		(uint32) b <<<= 7
# asm 1: rol  $7,<b=int64#15d
# asm 2: rol  $7,<b=%ebp
rol  $7,%ebp

# qhasm: 		x9 ^= b
# asm 1: xor  <b=int64#15,<x9=int64#8
# asm 2: xor  <b=%rbp,<x9=%r10
xor  %rbp,%r10

# qhasm: a = x0 + x4
# asm 1: lea  (<x0=int64#3,<x4=int64#6),>a=int64#15
# asm 2: lea  (<x0=%rdx,<x4=%r9),>a=%rbp
lea  (%rdx,%r9),%rbp

# qhasm: (uint32) a <<<= 9
# asm 1: rol  $9,<a=int64#15d
# asm 2: rol  $9,<a=%ebp
rol  $9,%ebp

# qhasm: x8 ^= a
# asm 1: xor  <a=int64#15,<x8=int64#9
# asm 2: xor  <a=%rbp,<x8=%r11
xor  %rbp,%r11

# qhasm: 		b = x5 + x9
# asm 1: lea  (<x5=int64#13,<x9=int64#8),>b=int64#15
# asm 2: lea  (<x5=%r15,<x9=%r10),>b=%rbp
lea  (%r15,%r10),%rbp

# qhasm: 		(uint32) b <<<= 9
# asm 1: rol  $9,<b=int64#15d
# asm 2: rol  $9,<b=%ebp
rol  $9,%ebp

# qhasm: 		x13 ^= b
# asm 1: xor  <b=int64#15,<x13=int64#11
# asm 2: xor  <b=%rbp,<x13=%r13
xor  %rbp,%r13

# qhasm: a = x4 + x8
# asm 1: lea  (<x4=int64#6,<x8=int64#9),>a=int64#15
# asm 2: lea  (<x4=%r9,<x8=%r11),>a=%rbp
lea  (%r9,%r11),%rbp

# qhasm: (uint32) a <<<= 13
# asm 1: rol  $13,<a=int64#15d
# asm 2: rol  $13,<a=%ebp
rol  $13,%ebp

# qhasm: x12 ^= a
# asm 1: xor  <a=int64#15,<x12=int64#12
# asm 2: xor  <a=%rbp,<x12=%r14
xor  %rbp,%r14

# qhasm: 		b = x9 + x13
# asm 1: lea  (<x9=int64#8,<x13=int64#11),>b=int64#15
# asm 2: lea  (<x9=%r10,<x13=%r13),>b=%rbp
lea  (%r10,%r13),%rbp

# qhasm: 		(uint32) b <<<= 13
# asm 1: rol  $13,<b=int64#15d
# asm 2: rol  $13,<b=%ebp
rol  $13,%ebp

# qhasm: 		x1 ^= b
# asm 1: xor  <b=int64#15,<x1=int64#1
# asm 2: xor  <b=%rbp,<x1=%rdi
xor  %rbp,%rdi

# qhasm: a = x8 + x12
# asm 1: lea  (<x8=int64#9,<x12=int64#12),>a=int64#15
# asm 2: lea  (<x8=%r11,<x12=%r14),>a=%rbp
lea  (%r11,%r14),%rbp

# qhasm: (uint32) a <<<= 18
# asm 1: rol  $18,<a=int64#15d
# asm 2: rol  $18,<a=%ebp
rol  $18,%ebp

# qhasm: x0 ^= a
# asm 1: xor  <a=int64#15,<x0=int64#3
# asm 2: xor  <a=%rbp,<x0=%rdx
xor  %rbp,%rdx

# qhasm: 		b = x13 + x1
# asm 1: lea  (<x13=int64#11,<x1=int64#1),>b=int64#15
# asm 2: lea  (<x13=%r13,<x1=%rdi),>b=%rbp
lea  (%r13,%rdi),%rbp

# qhasm: 		(uint32) b <<<= 18
# asm 1: rol  $18,<b=int64#15d
# asm 2: rol  $18,<b=%ebp
rol  $18,%ebp

# qhasm: 		x5 ^= b
# asm 1: xor  <b=int64#15,<x5=int64#13
# asm 2: xor  <b=%rbp,<x5=%r15
xor  %rbp,%r15

# qhasm: 				x10 = x10_stack
# asm 1: movq <x10_stack=stack64#22,>x10=int64#15
# asm 2: movq <x10_stack=168(%rsp),>x10=%rbp
movq 168(%rsp),%rbp

# qhasm: 		x5_stack = x5
# asm 1: movq <x5=int64#13,>x5_stack=stack64#21
# asm 2: movq <x5=%r15,>x5_stack=160(%rsp)
movq %r15,160(%rsp)

# qhasm: 				c = x6 + x10
# asm 1: lea  (<x6=int64#7,<x10=int64#15),>c=int64#13
# asm 2: lea  (<x6=%rax,<x10=%rbp),>c=%r15
lea  (%rax,%rbp),%r15

# qhasm: 				(uint32) c <<<= 7
# asm 1: rol  $7,<c=int64#13d
# asm 2: rol  $7,<c=%r15d
rol  $7,%r15d

# qhasm: 				x14 ^= c
# asm 1: xor  <c=int64#13,<x14=int64#14
# asm 2: xor  <c=%r15,<x14=%rbx
xor  %r15,%rbx

# qhasm: 				c = x10 + x14
# asm 1: lea  (<x10=int64#15,<x14=int64#14),>c=int64#13
# asm 2: lea  (<x10=%rbp,<x14=%rbx),>c=%r15
lea  (%rbp,%rbx),%r15

# qhasm: 				(uint32) c <<<= 9
# asm 1: rol  $9,<c=int64#13d
# asm 2: rol  $9,<c=%r15d
rol  $9,%r15d

# qhasm: 				x2 ^= c
# asm 1: xor  <c=int64#13,<x2=int64#4
# asm 2: xor  <c=%r15,<x2=%rcx
xor  %r15,%rcx

# qhasm: 				c = x14 + x2
# asm 1: lea  (<x14=int64#14,<x2=int64#4),>c=int64#13
# asm 2: lea  (<x14=%rbx,<x2=%rcx),>c=%r15
lea  (%rbx,%rcx),%r15

# qhasm: 				(uint32) c <<<= 13
# asm 1: rol  $13,<c=int64#13d
# asm 2: rol  $13,<c=%r15d
rol  $13,%r15d

# qhasm: 				x6 ^= c
# asm 1: xor  <c=int64#13,<x6=int64#7
# asm 2: xor  <c=%r15,<x6=%rax
xor  %r15,%rax

# qhasm: 				c = x2 + x6
# asm 1: lea  (<x2=int64#4,<x6=int64#7),>c=int64#13
# asm 2: lea  (<x2=%rcx,<x6=%rax),>c=%r15
lea  (%rcx,%rax),%r15

# qhasm: 				(uint32) c <<<= 18
# asm 1: rol  $18,<c=int64#13d
# asm 2: rol  $18,<c=%r15d
rol  $18,%r15d

# qhasm: 				x10 ^= c
# asm 1: xor  <c=int64#13,<x10=int64#15
# asm 2: xor  <c=%r15,<x10=%rbp
xor  %r15,%rbp

# qhasm: 						x15 = x15_stack
# asm 1: movq <x15_stack=stack64#23,>x15=int64#13
# asm 2: movq <x15_stack=176(%rsp),>x15=%r15
movq 176(%rsp),%r15

# qhasm: 				x10_stack = x10
# asm 1: movq <x10=int64#15,>x10_stack=stack64#22
# asm 2: movq <x10=%rbp,>x10_stack=168(%rsp)
movq %rbp,168(%rsp)

# qhasm: 						d = x11 + x15
# asm 1: lea  (<x11=int64#10,<x15=int64#13),>d=int64#15
# asm 2: lea  (<x11=%r12,<x15=%r15),>d=%rbp
lea  (%r12,%r15),%rbp

# qhasm: 						(uint32) d <<<= 7
# asm 1: rol  $7,<d=int64#15d
# asm 2: rol  $7,<d=%ebp
rol  $7,%ebp

# qhasm: 						x3 ^= d
# asm 1: xor  <d=int64#15,<x3=int64#2
# asm 2: xor  <d=%rbp,<x3=%rsi
xor  %rbp,%rsi

# qhasm: 						d = x15 + x3
# asm 1: lea  (<x15=int64#13,<x3=int64#2),>d=int64#15
# asm 2: lea  (<x15=%r15,<x3=%rsi),>d=%rbp
lea  (%r15,%rsi),%rbp

# qhasm: 						(uint32) d <<<= 9
# asm 1: rol  $9,<d=int64#15d
# asm 2: rol  $9,<d=%ebp
rol  $9,%ebp

# qhasm: 						x7 ^= d
# asm 1: xor  <d=int64#15,<x7=int64#5
# asm 2: xor  <d=%rbp,<x7=%r8
xor  %rbp,%r8

# qhasm: 						d = x3 + x7
# asm 1: lea  (<x3=int64#2,<x7=int64#5),>d=int64#15
# asm 2: lea  (<x3=%rsi,<x7=%r8),>d=%rbp
lea  (%rsi,%r8),%rbp

# qhasm: 						(uint32) d <<<= 13
# asm 1: rol  $13,<d=int64#15d
# asm 2: rol  $13,<d=%ebp
rol  $13,%ebp

# qhasm: 						x11 ^= d
# asm 1: xor  <d=int64#15,<x11=int64#10
# asm 2: xor  <d=%rbp,<x11=%r12
xor  %rbp,%r12

# qhasm: 						d = x7 + x11
# asm 1: lea  (<x7=int64#5,<x11=int64#10),>d=int64#15
# asm 2: lea  (<x7=%r8,<x11=%r12),>d=%rbp
lea  (%r8,%r12),%rbp

# qhasm: 						(uint32) d <<<= 18
# asm 1: rol  $18,<d=int64#15d
# asm 2: rol  $18,<d=%ebp
rol  $18,%ebp

# qhasm: 						x15 ^= d
# asm 1: xor  <d=int64#15,<x15=int64#13
# asm 2: xor  <d=%rbp,<x15=%r15
xor  %rbp,%r15

# qhasm: 						x15_stack = x15
# asm 1: movq <x15=int64#13,>x15_stack=stack64#23
# asm 2: movq <x15=%r15,>x15_stack=176(%rsp)
movq %r15,176(%rsp)

# qhasm: 		x5 = x5_stack
# asm 1: movq <x5_stack=stack64#21,>x5=int64#13
# asm 2: movq <x5_stack=160(%rsp),>x5=%r15
movq 160(%rsp),%r15

# qhasm: a = x3 + x0
# asm 1: lea  (<x3=int64#2,<x0=int64#3),>a=int64#15
# asm 2: lea  (<x3=%rsi,<x0=%rdx),>a=%rbp
lea  (%rsi,%rdx),%rbp

# qhasm: (uint32) a <<<= 7
# asm 1: rol  $7,<a=int64#15d
# asm 2: rol  $7,<a=%ebp
rol  $7,%ebp

# qhasm: x1 ^= a
# asm 1: xor  <a=int64#15,<x1=int64#1
# asm 2: xor  <a=%rbp,<x1=%rdi
xor  %rbp,%rdi

# qhasm: 		b = x4 + x5
# asm 1: lea  (<x4=int64#6,<x5=int64#13),>b=int64#15
# asm 2: lea  (<x4=%r9,<x5=%r15),>b=%rbp
lea  (%r9,%r15),%rbp

# qhasm: 		(uint32) b <<<= 7
# asm 1: rol  $7,<b=int64#15d
# asm 2: rol  $7,<b=%ebp
rol  $7,%ebp

# qhasm: 		x6 ^= b
# asm 1: xor  <b=int64#15,<x6=int64#7
# asm 2: xor  <b=%rbp,<x6=%rax
xor  %rbp,%rax

# qhasm: a = x0 + x1
# asm 1: lea  (<x0=int64#3,<x1=int64#1),>a=int64#15
# asm 2: lea  (<x0=%rdx,<x1=%rdi),>a=%rbp
lea  (%rdx,%rdi),%rbp

# qhasm: (uint32) a <<<= 9
# asm 1: rol  $9,<a=int64#15d
# asm 2: rol  $9,<a=%ebp
rol  $9,%ebp

# qhasm: x2 ^= a
# asm 1: xor  <a=int64#15,<x2=int64#4
# asm 2: xor  <a=%rbp,<x2=%rcx
xor  %rbp,%rcx

# qhasm: 		b = x5 + x6
# asm 1: lea  (<x5=int64#13,<x6=int64#7),>b=int64#15
# asm 2: lea  (<x5=%r15,<x6=%rax),>b=%rbp
lea  (%r15,%rax),%rbp

# qhasm: 		(uint32) b <<<= 9
# asm 1: rol  $9,<b=int64#15d
# asm 2: rol  $9,<b=%ebp
rol  $9,%ebp

# qhasm: 		x7 ^= b
# asm 1: xor  <b=int64#15,<x7=int64#5
# asm 2: xor  <b=%rbp,<x7=%r8
xor  %rbp,%r8

# qhasm: a = x1 + x2
# asm 1: lea  (<x1=int64#1,<x2=int64#4),>a=int64#15
# asm 2: lea  (<x1=%rdi,<x2=%rcx),>a=%rbp
lea  (%rdi,%rcx),%rbp

# qhasm: (uint32) a <<<= 13
# asm 1: rol  $13,<a=int64#15d
# asm 2: rol  $13,<a=%ebp
rol  $13,%ebp

# qhasm: x3 ^= a
# asm 1: xor  <a=int64#15,<x3=int64#2
# asm 2: xor  <a=%rbp,<x3=%rsi
xor  %rbp,%rsi

# qhasm: 		b = x6 + x7
# asm 1: lea  (<x6=int64#7,<x7=int64#5),>b=int64#15
# asm 2: lea  (<x6=%rax,<x7=%r8),>b=%rbp
lea  (%rax,%r8),%rbp

# qhasm: 		(uint32) b <<<= 13
# asm 1: rol  $13,<b=int64#15d
# asm 2: rol  $13,<b=%ebp
rol  $13,%ebp

# qhasm: 		x4 ^= b
# asm 1: xor  <b=int64#15,<x4=int64#6
# asm 2: xor  <b=%rbp,<x4=%r9
xor  %rbp,%r9

# qhasm: a = x2 + x3
# asm 1: lea  (<x2=int64#4,<x3=int64#2),>a=int64#15
# asm 2: lea  (<x2=%rcx,<x3=%rsi),>a=%rbp
lea  (%rcx,%rsi),%rbp

# qhasm: (uint32) a <<<= 18
# asm 1: rol  $18,<a=int64#15d
# asm 2: rol  $18,<a=%ebp
rol  $18,%ebp

# qhasm: x0 ^= a
# asm 1: xor  <a=int64#15,<x0=int64#3
# asm 2: xor  <a=%rbp,<x0=%rdx
xor  %rbp,%rdx

# qhasm: 		b = x7 + x4
# asm 1: lea  (<x7=int64#5,<x4=int64#6),>b=int64#15
# asm 2: lea  (<x7=%r8,<x4=%r9),>b=%rbp
lea  (%r8,%r9),%rbp

# qhasm: 		(uint32) b <<<= 18
# asm 1: rol  $18,<b=int64#15d
# asm 2: rol  $18,<b=%ebp
rol  $18,%ebp

# qhasm: 		x5 ^= b
# asm 1: xor  <b=int64#15,<x5=int64#13
# asm 2: xor  <b=%rbp,<x5=%r15
xor  %rbp,%r15

# qhasm: 				x10 = x10_stack
# asm 1: movq <x10_stack=stack64#22,>x10=int64#15
# asm 2: movq <x10_stack=168(%rsp),>x10=%rbp
movq 168(%rsp),%rbp

# qhasm: 		x5_stack = x5
# asm 1: movq <x5=int64#13,>x5_stack=stack64#21
# asm 2: movq <x5=%r15,>x5_stack=160(%rsp)
movq %r15,160(%rsp)

# qhasm: 				c = x9 + x10
# asm 1: lea  (<x9=int64#8,<x10=int64#15),>c=int64#13
# asm 2: lea  (<x9=%r10,<x10=%rbp),>c=%r15
lea  (%r10,%rbp),%r15

# qhasm: 				(uint32) c <<<= 7
# asm 1: rol  $7,<c=int64#13d
# asm 2: rol  $7,<c=%r15d
rol  $7,%r15d

# qhasm: 				x11 ^= c
# asm 1: xor  <c=int64#13,<x11=int64#10
# asm 2: xor  <c=%r15,<x11=%r12
xor  %r15,%r12

# qhasm: 				c = x10 + x11
# asm 1: lea  (<x10=int64#15,<x11=int64#10),>c=int64#13
# asm 2: lea  (<x10=%rbp,<x11=%r12),>c=%r15
lea  (%rbp,%r12),%r15

# qhasm: 				(uint32) c <<<= 9
# asm 1: rol  $9,<c=int64#13d
# asm 2: rol  $9,<c=%r15d
rol  $9,%r15d

# qhasm: 				x8 ^= c
# asm 1: xor  <c=int64#13,<x8=int64#9
# asm 2: xor  <c=%r15,<x8=%r11
xor  %r15,%r11

# qhasm: 				c = x11 + x8
# asm 1: lea  (<x11=int64#10,<x8=int64#9),>c=int64#13
# asm 2: lea  (<x11=%r12,<x8=%r11),>c=%r15
lea  (%r12,%r11),%r15

# qhasm: 				(uint32) c <<<= 13
# asm 1: rol  $13,<c=int64#13d
# asm 2: rol  $13,<c=%r15d
rol  $13,%r15d

# qhasm: 				x9 ^= c
# asm 1: xor  <c=int64#13,<x9=int64#8
# asm 2: xor  <c=%r15,<x9=%r10
xor  %r15,%r10

# qhasm: 				c = x8 + x9
# asm 1: lea  (<x8=int64#9,<x9=int64#8),>c=int64#13
# asm 2: lea  (<x8=%r11,<x9=%r10),>c=%r15
lea  (%r11,%r10),%r15

# qhasm: 				(uint32) c <<<= 18
# asm 1: rol  $18,<c=int64#13d
# asm 2: rol  $18,<c=%r15d
rol  $18,%r15d

# qhasm: 				x10 ^= c
# asm 1: xor  <c=int64#13,<x10=int64#15
# asm 2: xor  <c=%r15,<x10=%rbp
xor  %r15,%rbp

# qhasm: 						x15 = x15_stack
# asm 1: movq <x15_stack=stack64#23,>x15=int64#13
# asm 2: movq <x15_stack=176(%rsp),>x15=%r15
movq 176(%rsp),%r15

# qhasm: 				x10_stack = x10
# asm 1: movq <x10=int64#15,>x10_stack=stack64#22
# asm 2: movq <x10=%rbp,>x10_stack=168(%rsp)
movq %rbp,168(%rsp)

# qhasm: 						d = x14 + x15
# asm 1: lea  (<x14=int64#14,<x15=int64#13),>d=int64#15
# asm 2: lea  (<x14=%rbx,<x15=%r15),>d=%rbp
lea  (%rbx,%r15),%rbp

# qhasm: 						(uint32) d <<<= 7
# asm 1: rol  $7,<d=int64#15d
# asm 2: rol  $7,<d=%ebp
rol  $7,%ebp

# qhasm: 						x12 ^= d
# asm 1: xor  <d=int64#15,<x12=int64#12
# asm 2: xor  <d=%rbp,<x12=%r14
xor  %rbp,%r14

# qhasm: 						d = x15 + x12
# asm 1: lea  (<x15=int64#13,<x12=int64#12),>d=int64#15
# asm 2: lea  (<x15=%r15,<x12=%r14),>d=%rbp
lea  (%r15,%r14),%rbp

# qhasm: 						(uint32) d <<<= 9
# asm 1: rol  $9,<d=int64#15d
# asm 2: rol  $9,<d=%ebp
rol  $9,%ebp

# qhasm: 						x13 ^= d
# asm 1: xor  <d=int64#15,<x13=int64#11
# asm 2: xor  <d=%rbp,<x13=%r13
xor  %rbp,%r13

# qhasm: 						d = x12 + x13
# asm 1: lea  (<x12=int64#12,<x13=int64#11),>d=int64#15
# asm 2: lea  (<x12=%r14,<x13=%r13),>d=%rbp
lea  (%r14,%r13),%rbp

# qhasm: 						(uint32) d <<<= 13
# asm 1: rol  $13,<d=int64#15d
# asm 2: rol  $13,<d=%ebp
rol  $13,%ebp

# qhasm: 						x14 ^= d
# asm 1: xor  <d=int64#15,<x14=int64#14
# asm 2: xor  <d=%rbp,<x14=%rbx
xor  %rbp,%rbx

# qhasm: 						d = x13 + x14
# asm 1: lea  (<x13=int64#11,<x14=int64#14),>d=int64#15
# asm 2: lea  (<x13=%r13,<x14=%rbx),>d=%rbp
lea  (%r13,%rbx),%rbp

# qhasm: 						(uint32) d <<<= 18
# asm 1: rol  $18,<d=int64#15d
# asm 2: rol  $18,<d=%ebp
rol  $18,%ebp

# qhasm: 						x15 ^= d
# asm 1: xor  <d=int64#15,<x15=int64#13
# asm 2: xor  <d=%rbp,<x15=%r15
xor  %rbp,%r15

# qhasm: 						x15_stack = x15
# asm 1: movq <x15=int64#13,>x15_stack=stack64#23
# asm 2: movq <x15=%r15,>x15_stack=176(%rsp)
movq %r15,176(%rsp)

# qhasm: 		x5 = x5_stack
# asm 1: movq <x5_stack=stack64#21,>x5=int64#13
# asm 2: movq <x5_stack=160(%rsp),>x5=%r15
movq 160(%rsp),%r15

# qhasm: a = x12 + x0
# asm 1: lea  (<x12=int64#12,<x0=int64#3),>a=int64#15
# asm 2: lea  (<x12=%r14,<x0=%rdx),>a=%rbp
lea  (%r14,%rdx),%rbp

# qhasm: (uint32) a <<<= 7
# asm 1: rol  $7,<a=int64#15d
# asm 2: rol  $7,<a=%ebp
rol  $7,%ebp

# qhasm: x4 ^= a
# asm 1: xor  <a=int64#15,<x4=int64#6
# asm 2: xor  <a=%rbp,<x4=%r9
xor  %rbp,%r9

# qhasm: 		b = x1 + x5
# asm 1: lea  (<x1=int64#1,<x5=int64#13),>b=int64#15
# asm 2: lea  (<x1=%rdi,<x5=%r15),>b=%rbp
lea  (%rdi,%r15),%rbp

# qhasm: 		(uint32) b <<<= 7
# asm 1: rol  $7,<b=int64#15d
# asm 2: rol  $7,<b=%ebp
rol  $7,%ebp

# qhasm: 		x9 ^= b
# asm 1: xor  <b=int64#15,<x9=int64#8
# asm 2: xor  <b=%rbp,<x9=%r10
xor  %rbp,%r10

# qhasm: a = x0 + x4
# asm 1: lea  (<x0=int64#3,<x4=int64#6),>a=int64#15
# asm 2: lea  (<x0=%rdx,<x4=%r9),>a=%rbp
lea  (%rdx,%r9),%rbp

# qhasm: (uint32) a <<<= 9
# asm 1: rol  $9,<a=int64#15d
# asm 2: rol  $9,<a=%ebp
rol  $9,%ebp

# qhasm: x8 ^= a
# asm 1: xor  <a=int64#15,<x8=int64#9
# asm 2: xor  <a=%rbp,<x8=%r11
xor  %rbp,%r11

# qhasm: 		b = x5 + x9
# asm 1: lea  (<x5=int64#13,<x9=int64#8),>b=int64#15
# asm 2: lea  (<x5=%r15,<x9=%r10),>b=%rbp
lea  (%r15,%r10),%rbp

# qhasm: 		(uint32) b <<<= 9
# asm 1: rol  $9,<b=int64#15d
# asm 2: rol  $9,<b=%ebp
rol  $9,%ebp

# qhasm: 		x13 ^= b
# asm 1: xor  <b=int64#15,<x13=int64#11
# asm 2: xor  <b=%rbp,<x13=%r13
xor  %rbp,%r13

# qhasm: a = x4 + x8
# asm 1: lea  (<x4=int64#6,<x8=int64#9),>a=int64#15
# asm 2: lea  (<x4=%r9,<x8=%r11),>a=%rbp
lea  (%r9,%r11),%rbp

# qhasm: (uint32) a <<<= 13
# asm 1: rol  $13,<a=int64#15d
# asm 2: rol  $13,<a=%ebp
rol  $13,%ebp

# qhasm: x12 ^= a
# asm 1: xor  <a=int64#15,<x12=int64#12
# asm 2: xor  <a=%rbp,<x12=%r14
xor  %rbp,%r14

# qhasm: 		b = x9 + x13
# asm 1: lea  (<x9=int64#8,<x13=int64#11),>b=int64#15
# asm 2: lea  (<x9=%r10,<x13=%r13),>b=%rbp
lea  (%r10,%r13),%rbp

# qhasm: 		(uint32) b <<<= 13
# asm 1: rol  $13,<b=int64#15d
# asm 2: rol  $13,<b=%ebp
rol  $13,%ebp

# qhasm: 		x1 ^= b
# asm 1: xor  <b=int64#15,<x1=int64#1
# asm 2: xor  <b=%rbp,<x1=%rdi
xor  %rbp,%rdi

# qhasm: a = x8 + x12
# asm 1: lea  (<x8=int64#9,<x12=int64#12),>a=int64#15
# asm 2: lea  (<x8=%r11,<x12=%r14),>a=%rbp
lea  (%r11,%r14),%rbp

# qhasm: (uint32) a <<<= 18
# asm 1: rol  $18,<a=int64#15d
# asm 2: rol  $18,<a=%ebp
rol  $18,%ebp

# qhasm: x0 ^= a
# asm 1: xor  <a=int64#15,<x0=int64#3
# asm 2: xor  <a=%rbp,<x0=%rdx
xor  %rbp,%rdx

# qhasm: 		b = x13 + x1
# asm 1: lea  (<x13=int64#11,<x1=int64#1),>b=int64#15
# asm 2: lea  (<x13=%r13,<x1=%rdi),>b=%rbp
lea  (%r13,%rdi),%rbp

# qhasm: 		(uint32) b <<<= 18
# asm 1: rol  $18,<b=int64#15d
# asm 2: rol  $18,<b=%ebp
rol  $18,%ebp

# qhasm: 		x5 ^= b
# asm 1: xor  <b=int64#15,<x5=int64#13
# asm 2: xor  <b=%rbp,<x5=%r15
xor  %rbp,%r15

# qhasm: 				x10 = x10_stack
# asm 1: movq <x10_stack=stack64#22,>x10=int64#15
# asm 2: movq <x10_stack=168(%rsp),>x10=%rbp
movq 168(%rsp),%rbp

# qhasm: 		x5_stack = x5
# asm 1: movq <x5=int64#13,>x5_stack=stack64#21
# asm 2: movq <x5=%r15,>x5_stack=160(%rsp)
movq %r15,160(%rsp)

# qhasm: 				c = x6 + x10
# asm 1: lea  (<x6=int64#7,<x10=int64#15),>c=int64#13
# asm 2: lea  (<x6=%rax,<x10=%rbp),>c=%r15
lea  (%rax,%rbp),%r15

# qhasm: 				(uint32) c <<<= 7
# asm 1: rol  $7,<c=int64#13d
# asm 2: rol  $7,<c=%r15d
rol  $7,%r15d

# qhasm: 				x14 ^= c
# asm 1: xor  <c=int64#13,<x14=int64#14
# asm 2: xor  <c=%r15,<x14=%rbx
xor  %r15,%rbx

# qhasm: 				c = x10 + x14
# asm 1: lea  (<x10=int64#15,<x14=int64#14),>c=int64#13
# asm 2: lea  (<x10=%rbp,<x14=%rbx),>c=%r15
lea  (%rbp,%rbx),%r15

# qhasm: 				(uint32) c <<<= 9
# asm 1: rol  $9,<c=int64#13d
# asm 2: rol  $9,<c=%r15d
rol  $9,%r15d

# qhasm: 				x2 ^= c
# asm 1: xor  <c=int64#13,<x2=int64#4
# asm 2: xor  <c=%r15,<x2=%rcx
xor  %r15,%rcx

# qhasm: 				c = x14 + x2
# asm 1: lea  (<x14=int64#14,<x2=int64#4),>c=int64#13
# asm 2: lea  (<x14=%rbx,<x2=%rcx),>c=%r15
lea  (%rbx,%rcx),%r15

# qhasm: 				(uint32) c <<<= 13
# asm 1: rol  $13,<c=int64#13d
# asm 2: rol  $13,<c=%r15d
rol  $13,%r15d

# qhasm: 				x6 ^= c
# asm 1: xor  <c=int64#13,<x6=int64#7
# asm 2: xor  <c=%r15,<x6=%rax
xor  %r15,%rax

# qhasm: 				c = x2 + x6
# asm 1: lea  (<x2=int64#4,<x6=int64#7),>c=int64#13
# asm 2: lea  (<x2=%rcx,<x6=%rax),>c=%r15
lea  (%rcx,%rax),%r15

# qhasm: 				(uint32) c <<<= 18
# asm 1: rol  $18,<c=int64#13d
# asm 2: rol  $18,<c=%r15d
rol  $18,%r15d

# qhasm: 				x10 ^= c
# asm 1: xor  <c=int64#13,<x10=int64#15
# asm 2: xor  <c=%r15,<x10=%rbp
xor  %r15,%rbp

# qhasm: 						x15 = x15_stack
# asm 1: movq <x15_stack=stack64#23,>x15=int64#13
# asm 2: movq <x15_stack=176(%rsp),>x15=%r15
movq 176(%rsp),%r15

# qhasm: 				x10_stack = x10
# asm 1: movq <x10=int64#15,>x10_stack=stack64#22
# asm 2: movq <x10=%rbp,>x10_stack=168(%rsp)
movq %rbp,168(%rsp)

# qhasm: 						d = x11 + x15
# asm 1: lea  (<x11=int64#10,<x15=int64#13),>d=int64#15
# asm 2: lea  (<x11=%r12,<x15=%r15),>d=%rbp
lea  (%r12,%r15),%rbp

# qhasm: 						(uint32) d <<<= 7
# asm 1: rol  $7,<d=int64#15d
# asm 2: rol  $7,<d=%ebp
rol  $7,%ebp

# qhasm: 						x3 ^= d
# asm 1: xor  <d=int64#15,<x3=int64#2
# asm 2: xor  <d=%rbp,<x3=%rsi
xor  %rbp,%rsi

# qhasm: 						d = x15 + x3
# asm 1: lea  (<x15=int64#13,<x3=int64#2),>d=int64#15
# asm 2: lea  (<x15=%r15,<x3=%rsi),>d=%rbp
lea  (%r15,%rsi),%rbp

# qhasm: 						(uint32) d <<<= 9
# asm 1: rol  $9,<d=int64#15d
# asm 2: rol  $9,<d=%ebp
rol  $9,%ebp

# qhasm: 						x7 ^= d
# asm 1: xor  <d=int64#15,<x7=int64#5
# asm 2: xor  <d=%rbp,<x7=%r8
xor  %rbp,%r8

# qhasm: 						d = x3 + x7
# asm 1: lea  (<x3=int64#2,<x7=int64#5),>d=int64#15
# asm 2: lea  (<x3=%rsi,<x7=%r8),>d=%rbp
lea  (%rsi,%r8),%rbp

# qhasm: 						(uint32) d <<<= 13
# asm 1: rol  $13,<d=int64#15d
# asm 2: rol  $13,<d=%ebp
rol  $13,%ebp

# qhasm: 						x11 ^= d
# asm 1: xor  <d=int64#15,<x11=int64#10
# asm 2: xor  <d=%rbp,<x11=%r12
xor  %rbp,%r12

# qhasm: 						d = x7 + x11
# asm 1: lea  (<x7=int64#5,<x11=int64#10),>d=int64#15
# asm 2: lea  (<x7=%r8,<x11=%r12),>d=%rbp
lea  (%r8,%r12),%rbp

# qhasm: 						(uint32) d <<<= 18
# asm 1: rol  $18,<d=int64#15d
# asm 2: rol  $18,<d=%ebp
rol  $18,%ebp

# qhasm: 						x15 ^= d
# asm 1: xor  <d=int64#15,<x15=int64#13
# asm 2: xor  <d=%rbp,<x15=%r15
xor  %rbp,%r15

# qhasm: 						x15_stack = x15
# asm 1: movq <x15=int64#13,>x15_stack=stack64#23
# asm 2: movq <x15=%r15,>x15_stack=176(%rsp)
movq %r15,176(%rsp)

# qhasm: 		x5 = x5_stack
# asm 1: movq <x5_stack=stack64#21,>x5=int64#13
# asm 2: movq <x5_stack=160(%rsp),>x5=%r15
movq 160(%rsp),%r15

# qhasm: a = x3 + x0
# asm 1: lea  (<x3=int64#2,<x0=int64#3),>a=int64#15
# asm 2: lea  (<x3=%rsi,<x0=%rdx),>a=%rbp
lea  (%rsi,%rdx),%rbp

# qhasm: (uint32) a <<<= 7
# asm 1: rol  $7,<a=int64#15d
# asm 2: rol  $7,<a=%ebp
rol  $7,%ebp

# qhasm: x1 ^= a
# asm 1: xor  <a=int64#15,<x1=int64#1
# asm 2: xor  <a=%rbp,<x1=%rdi
xor  %rbp,%rdi

# qhasm: 		b = x4 + x5
# asm 1: lea  (<x4=int64#6,<x5=int64#13),>b=int64#15
# asm 2: lea  (<x4=%r9,<x5=%r15),>b=%rbp
lea  (%r9,%r15),%rbp

# qhasm: 		(uint32) b <<<= 7
# asm 1: rol  $7,<b=int64#15d
# asm 2: rol  $7,<b=%ebp
rol  $7,%ebp

# qhasm: 		x6 ^= b
# asm 1: xor  <b=int64#15,<x6=int64#7
# asm 2: xor  <b=%rbp,<x6=%rax
xor  %rbp,%rax

# qhasm: a = x0 + x1
# asm 1: lea  (<x0=int64#3,<x1=int64#1),>a=int64#15
# asm 2: lea  (<x0=%rdx,<x1=%rdi),>a=%rbp
lea  (%rdx,%rdi),%rbp

# qhasm: (uint32) a <<<= 9
# asm 1: rol  $9,<a=int64#15d
# asm 2: rol  $9,<a=%ebp
rol  $9,%ebp

# qhasm: x2 ^= a
# asm 1: xor  <a=int64#15,<x2=int64#4
# asm 2: xor  <a=%rbp,<x2=%rcx
xor  %rbp,%rcx

# qhasm: 		b = x5 + x6
# asm 1: lea  (<x5=int64#13,<x6=int64#7),>b=int64#15
# asm 2: lea  (<x5=%r15,<x6=%rax),>b=%rbp
lea  (%r15,%rax),%rbp

# qhasm: 		(uint32) b <<<= 9
# asm 1: rol  $9,<b=int64#15d
# asm 2: rol  $9,<b=%ebp
rol  $9,%ebp

# qhasm: 		x7 ^= b
# asm 1: xor  <b=int64#15,<x7=int64#5
# asm 2: xor  <b=%rbp,<x7=%r8
xor  %rbp,%r8

# qhasm: a = x1 + x2
# asm 1: lea  (<x1=int64#1,<x2=int64#4),>a=int64#15
# asm 2: lea  (<x1=%rdi,<x2=%rcx),>a=%rbp
lea  (%rdi,%rcx),%rbp

# qhasm: (uint32) a <<<= 13
# asm 1: rol  $13,<a=int64#15d
# asm 2: rol  $13,<a=%ebp
rol  $13,%ebp

# qhasm: x3 ^= a
# asm 1: xor  <a=int64#15,<x3=int64#2
# asm 2: xor  <a=%rbp,<x3=%rsi
xor  %rbp,%rsi

# qhasm: 		b = x6 + x7
# asm 1: lea  (<x6=int64#7,<x7=int64#5),>b=int64#15
# asm 2: lea  (<x6=%rax,<x7=%r8),>b=%rbp
lea  (%rax,%r8),%rbp

# qhasm: 		(uint32) b <<<= 13
# asm 1: rol  $13,<b=int64#15d
# asm 2: rol  $13,<b=%ebp
rol  $13,%ebp

# qhasm: 		x4 ^= b
# asm 1: xor  <b=int64#15,<x4=int64#6
# asm 2: xor  <b=%rbp,<x4=%r9
xor  %rbp,%r9

# qhasm: a = x2 + x3
# asm 1: lea  (<x2=int64#4,<x3=int64#2),>a=int64#15
# asm 2: lea  (<x2=%rcx,<x3=%rsi),>a=%rbp
lea  (%rcx,%rsi),%rbp

# qhasm: (uint32) a <<<= 18
# asm 1: rol  $18,<a=int64#15d
# asm 2: rol  $18,<a=%ebp
rol  $18,%ebp

# qhasm: x0 ^= a
# asm 1: xor  <a=int64#15,<x0=int64#3
# asm 2: xor  <a=%rbp,<x0=%rdx
xor  %rbp,%rdx

# qhasm: 		b = x7 + x4
# asm 1: lea  (<x7=int64#5,<x4=int64#6),>b=int64#15
# asm 2: lea  (<x7=%r8,<x4=%r9),>b=%rbp
lea  (%r8,%r9),%rbp

# qhasm: 		(uint32) b <<<= 18
# asm 1: rol  $18,<b=int64#15d
# asm 2: rol  $18,<b=%ebp
rol  $18,%ebp

# qhasm: 		x5 ^= b
# asm 1: xor  <b=int64#15,<x5=int64#13
# asm 2: xor  <b=%rbp,<x5=%r15
xor  %rbp,%r15

# qhasm: 				x10 = x10_stack
# asm 1: movq <x10_stack=stack64#22,>x10=int64#15
# asm 2: movq <x10_stack=168(%rsp),>x10=%rbp
movq 168(%rsp),%rbp

# qhasm: 		x5_stack = x5
# asm 1: movq <x5=int64#13,>x5_stack=stack64#21
# asm 2: movq <x5=%r15,>x5_stack=160(%rsp)
movq %r15,160(%rsp)

# qhasm: 				c = x9 + x10
# asm 1: lea  (<x9=int64#8,<x10=int64#15),>c=int64#13
# asm 2: lea  (<x9=%r10,<x10=%rbp),>c=%r15
lea  (%r10,%rbp),%r15

# qhasm: 				(uint32) c <<<= 7
# asm 1: rol  $7,<c=int64#13d
# asm 2: rol  $7,<c=%r15d
rol  $7,%r15d

# qhasm: 				x11 ^= c
# asm 1: xor  <c=int64#13,<x11=int64#10
# asm 2: xor  <c=%r15,<x11=%r12
xor  %r15,%r12

# qhasm: 				c = x10 + x11
# asm 1: lea  (<x10=int64#15,<x11=int64#10),>c=int64#13
# asm 2: lea  (<x10=%rbp,<x11=%r12),>c=%r15
lea  (%rbp,%r12),%r15

# qhasm: 				(uint32) c <<<= 9
# asm 1: rol  $9,<c=int64#13d
# asm 2: rol  $9,<c=%r15d
rol  $9,%r15d

# qhasm: 				x8 ^= c
# asm 1: xor  <c=int64#13,<x8=int64#9
# asm 2: xor  <c=%r15,<x8=%r11
xor  %r15,%r11

# qhasm: 				c = x11 + x8
# asm 1: lea  (<x11=int64#10,<x8=int64#9),>c=int64#13
# asm 2: lea  (<x11=%r12,<x8=%r11),>c=%r15
lea  (%r12,%r11),%r15

# qhasm: 				(uint32) c <<<= 13
# asm 1: rol  $13,<c=int64#13d
# asm 2: rol  $13,<c=%r15d
rol  $13,%r15d

# qhasm: 				x9 ^= c
# asm 1: xor  <c=int64#13,<x9=int64#8
# asm 2: xor  <c=%r15,<x9=%r10
xor  %r15,%r10

# qhasm: 				c = x8 + x9
# asm 1: lea  (<x8=int64#9,<x9=int64#8),>c=int64#13
# asm 2: lea  (<x8=%r11,<x9=%r10),>c=%r15
lea  (%r11,%r10),%r15

# qhasm: 				(uint32) c <<<= 18
# asm 1: rol  $18,<c=int64#13d
# asm 2: rol  $18,<c=%r15d
rol  $18,%r15d

# qhasm: 				x10 ^= c
# asm 1: xor  <c=int64#13,<x10=int64#15
# asm 2: xor  <c=%r15,<x10=%rbp
xor  %r15,%rbp

# qhasm: 						x15 = x15_stack
# asm 1: movq <x15_stack=stack64#23,>x15=int64#13
# asm 2: movq <x15_stack=176(%rsp),>x15=%r15
movq 176(%rsp),%r15

# qhasm: 				x10_stack = x10
# asm 1: movq <x10=int64#15,>x10_stack=stack64#22
# asm 2: movq <x10=%rbp,>x10_stack=168(%rsp)
movq %rbp,168(%rsp)

# qhasm: 						d = x14 + x15
# asm 1: lea  (<x14=int64#14,<x15=int64#13),>d=int64#15
# asm 2: lea  (<x14=%rbx,<x15=%r15),>d=%rbp
lea  (%rbx,%r15),%rbp

# qhasm: 						(uint32) d <<<= 7
# asm 1: rol  $7,<d=int64#15d
# asm 2: rol  $7,<d=%ebp
rol  $7,%ebp

# qhasm: 						x12 ^= d
# asm 1: xor  <d=int64#15,<x12=int64#12
# asm 2: xor  <d=%rbp,<x12=%r14
xor  %rbp,%r14

# qhasm: 						d = x15 + x12
# asm 1: lea  (<x15=int64#13,<x12=int64#12),>d=int64#15
# asm 2: lea  (<x15=%r15,<x12=%r14),>d=%rbp
lea  (%r15,%r14),%rbp

# qhasm: 						(uint32) d <<<= 9
# asm 1: rol  $9,<d=int64#15d
# asm 2: rol  $9,<d=%ebp
rol  $9,%ebp

# qhasm: 						x13 ^= d
# asm 1: xor  <d=int64#15,<x13=int64#11
# asm 2: xor  <d=%rbp,<x13=%r13
xor  %rbp,%r13

# qhasm: 						d = x12 + x13
# asm 1: lea  (<x12=int64#12,<x13=int64#11),>d=int64#15
# asm 2: lea  (<x12=%r14,<x13=%r13),>d=%rbp
lea  (%r14,%r13),%rbp

# qhasm: 						(uint32) d <<<= 13
# asm 1: rol  $13,<d=int64#15d
# asm 2: rol  $13,<d=%ebp
rol  $13,%ebp

# qhasm: 						x14 ^= d
# asm 1: xor  <d=int64#15,<x14=int64#14
# asm 2: xor  <d=%rbp,<x14=%rbx
xor  %rbp,%rbx

# qhasm: 						d = x13 + x14
# asm 1: lea  (<x13=int64#11,<x14=int64#14),>d=int64#15
# asm 2: lea  (<x13=%r13,<x14=%rbx),>d=%rbp
lea  (%r13,%rbx),%rbp

# qhasm: 						(uint32) d <<<= 18
# asm 1: rol  $18,<d=int64#15d
# asm 2: rol  $18,<d=%ebp
rol  $18,%ebp

# qhasm: 						x15 ^= d
# asm 1: xor  <d=int64#15,<x15=int64#13
# asm 2: xor  <d=%rbp,<x15=%r15
xor  %rbp,%r15

# qhasm: 						x15_stack = x15
# asm 1: movq <x15=int64#13,>x15_stack=stack64#23
# asm 2: movq <x15=%r15,>x15_stack=176(%rsp)
movq %r15,176(%rsp)

# qhasm:   i = i_backup
# asm 1: movq <i_backup=stack64#24,>i=int64#13
# asm 2: movq <i_backup=184(%rsp),>i=%r15
movq 184(%rsp),%r15

# qhasm:                  unsigned>? i -= 4
# asm 1: sub  $4,<i=int64#13
# asm 2: sub  $4,<i=%r15
sub  $4,%r15
# comment:fp stack unchanged by jump

# qhasm: goto mainloop if unsigned>
ja ._mainloop

# qhasm:   (uint32) x2 += j2
# asm 1: addl <j2=stack64#9,<x2=int64#4d
# asm 2: addl <j2=64(%rsp),<x2=%ecx
addl 64(%rsp),%ecx

# qhasm:   x3 <<= 32
# asm 1: shl  $32,<x3=int64#2
# asm 2: shl  $32,<x3=%rsi
shl  $32,%rsi

# qhasm:   x3 += j2
# asm 1: addq <j2=stack64#9,<x3=int64#2
# asm 2: addq <j2=64(%rsp),<x3=%rsi
addq 64(%rsp),%rsi

# qhasm:   (uint64) x3 >>= 32
# asm 1: shr  $32,<x3=int64#2
# asm 2: shr  $32,<x3=%rsi
shr  $32,%rsi

# qhasm:   x3 <<= 32
# asm 1: shl  $32,<x3=int64#2
# asm 2: shl  $32,<x3=%rsi
shl  $32,%rsi

# qhasm:   x2 += x3
# asm 1: add  <x3=int64#2,<x2=int64#4
# asm 2: add  <x3=%rsi,<x2=%rcx
add  %rsi,%rcx

# qhasm:   (uint32) x6 += j6
# asm 1: addl <j6=stack64#11,<x6=int64#7d
# asm 2: addl <j6=80(%rsp),<x6=%eax
addl 80(%rsp),%eax

# qhasm:   x7 <<= 32
# asm 1: shl  $32,<x7=int64#5
# asm 2: shl  $32,<x7=%r8
shl  $32,%r8

# qhasm:   x7 += j6
# asm 1: addq <j6=stack64#11,<x7=int64#5
# asm 2: addq <j6=80(%rsp),<x7=%r8
addq 80(%rsp),%r8

# qhasm:   (uint64) x7 >>= 32
# asm 1: shr  $32,<x7=int64#5
# asm 2: shr  $32,<x7=%r8
shr  $32,%r8

# qhasm:   x7 <<= 32
# asm 1: shl  $32,<x7=int64#5
# asm 2: shl  $32,<x7=%r8
shl  $32,%r8

# qhasm:   x6 += x7
# asm 1: add  <x7=int64#5,<x6=int64#7
# asm 2: add  <x7=%r8,<x6=%rax
add  %r8,%rax

# qhasm:   (uint32) x8 += j8
# asm 1: addl <j8=stack64#12,<x8=int64#9d
# asm 2: addl <j8=88(%rsp),<x8=%r11d
addl 88(%rsp),%r11d

# qhasm:   x9 <<= 32
# asm 1: shl  $32,<x9=int64#8
# asm 2: shl  $32,<x9=%r10
shl  $32,%r10

# qhasm:   x9 += j8
# asm 1: addq <j8=stack64#12,<x9=int64#8
# asm 2: addq <j8=88(%rsp),<x9=%r10
addq 88(%rsp),%r10

# qhasm:   (uint64) x9 >>= 32
# asm 1: shr  $32,<x9=int64#8
# asm 2: shr  $32,<x9=%r10
shr  $32,%r10

# qhasm:   x9 <<= 32
# asm 1: shl  $32,<x9=int64#8
# asm 2: shl  $32,<x9=%r10
shl  $32,%r10

# qhasm:   x8 += x9
# asm 1: add  <x9=int64#8,<x8=int64#9
# asm 2: add  <x9=%r10,<x8=%r11
add  %r10,%r11

# qhasm:   (uint32) x12 += j12
# asm 1: addl <j12=stack64#14,<x12=int64#12d
# asm 2: addl <j12=104(%rsp),<x12=%r14d
addl 104(%rsp),%r14d

# qhasm:   x13 <<= 32
# asm 1: shl  $32,<x13=int64#11
# asm 2: shl  $32,<x13=%r13
shl  $32,%r13

# qhasm:   x13 += j12
# asm 1: addq <j12=stack64#14,<x13=int64#11
# asm 2: addq <j12=104(%rsp),<x13=%r13
addq 104(%rsp),%r13

# qhasm:   (uint64) x13 >>= 32
# asm 1: shr  $32,<x13=int64#11
# asm 2: shr  $32,<x13=%r13
shr  $32,%r13

# qhasm:   x13 <<= 32
# asm 1: shl  $32,<x13=int64#11
# asm 2: shl  $32,<x13=%r13
shl  $32,%r13

# qhasm:   x12 += x13
# asm 1: add  <x13=int64#11,<x12=int64#12
# asm 2: add  <x13=%r13,<x12=%r14
add  %r13,%r14

# qhasm:   (uint32) x0 += j0
# asm 1: addl <j0=stack64#8,<x0=int64#3d
# asm 2: addl <j0=56(%rsp),<x0=%edx
addl 56(%rsp),%edx

# qhasm:   x1 <<= 32
# asm 1: shl  $32,<x1=int64#1
# asm 2: shl  $32,<x1=%rdi
shl  $32,%rdi

# qhasm:   x1 += j0
# asm 1: addq <j0=stack64#8,<x1=int64#1
# asm 2: addq <j0=56(%rsp),<x1=%rdi
addq 56(%rsp),%rdi

# qhasm:   (uint64) x1 >>= 32
# asm 1: shr  $32,<x1=int64#1
# asm 2: shr  $32,<x1=%rdi
shr  $32,%rdi

# qhasm:   x1 <<= 32
# asm 1: shl  $32,<x1=int64#1
# asm 2: shl  $32,<x1=%rdi
shl  $32,%rdi

# qhasm:   x0 += x1
# asm 1: add  <x1=int64#1,<x0=int64#3
# asm 2: add  <x1=%rdi,<x0=%rdx
add  %rdi,%rdx

# qhasm:   x5 = x5_stack
# asm 1: movq <x5_stack=stack64#21,>x5=int64#1
# asm 2: movq <x5_stack=160(%rsp),>x5=%rdi
movq 160(%rsp),%rdi

# qhasm:   (uint32) x4 += j4
# asm 1: addl <j4=stack64#10,<x4=int64#6d
# asm 2: addl <j4=72(%rsp),<x4=%r9d
addl 72(%rsp),%r9d

# qhasm:   x5 <<= 32
# asm 1: shl  $32,<x5=int64#1
# asm 2: shl  $32,<x5=%rdi
shl  $32,%rdi

# qhasm:   x5 += j4
# asm 1: addq <j4=stack64#10,<x5=int64#1
# asm 2: addq <j4=72(%rsp),<x5=%rdi
addq 72(%rsp),%rdi

# qhasm:   (uint64) x5 >>= 32
# asm 1: shr  $32,<x5=int64#1
# asm 2: shr  $32,<x5=%rdi
shr  $32,%rdi

# qhasm:   x5 <<= 32
# asm 1: shl  $32,<x5=int64#1
# asm 2: shl  $32,<x5=%rdi
shl  $32,%rdi

# qhasm:   x4 += x5
# asm 1: add  <x5=int64#1,<x4=int64#6
# asm 2: add  <x5=%rdi,<x4=%r9
add  %rdi,%r9

# qhasm:   x10 = x10_stack
# asm 1: movq <x10_stack=stack64#22,>x10=int64#5
# asm 2: movq <x10_stack=168(%rsp),>x10=%r8
movq 168(%rsp),%r8

# qhasm:   (uint32) x10 += j10
# asm 1: addl <j10=stack64#13,<x10=int64#5d
# asm 2: addl <j10=96(%rsp),<x10=%r8d
addl 96(%rsp),%r8d

# qhasm:   x11 <<= 32
# asm 1: shl  $32,<x11=int64#10
# asm 2: shl  $32,<x11=%r12
shl  $32,%r12

# qhasm:   x11 += j10
# asm 1: addq <j10=stack64#13,<x11=int64#10
# asm 2: addq <j10=96(%rsp),<x11=%r12
addq 96(%rsp),%r12

# qhasm:   (uint64) x11 >>= 32
# asm 1: shr  $32,<x11=int64#10
# asm 2: shr  $32,<x11=%r12
shr  $32,%r12

# qhasm:   x11 <<= 32
# asm 1: shl  $32,<x11=int64#10
# asm 2: shl  $32,<x11=%r12
shl  $32,%r12

# qhasm:   x10 += x11
# asm 1: add  <x11=int64#10,<x10=int64#5
# asm 2: add  <x11=%r12,<x10=%r8
add  %r12,%r8

# qhasm:   x15 = x15_stack
# asm 1: movq <x15_stack=stack64#23,>x15=int64#1
# asm 2: movq <x15_stack=176(%rsp),>x15=%rdi
movq 176(%rsp),%rdi

# qhasm:   (uint32) x14 += j14
# asm 1: addl <j14=stack64#15,<x14=int64#14d
# asm 2: addl <j14=112(%rsp),<x14=%ebx
addl 112(%rsp),%ebx

# qhasm:   x15 <<= 32
# asm 1: shl  $32,<x15=int64#1
# asm 2: shl  $32,<x15=%rdi
shl  $32,%rdi

# qhasm:   x15 += j14
# asm 1: addq <j14=stack64#15,<x15=int64#1
# asm 2: addq <j14=112(%rsp),<x15=%rdi
addq 112(%rsp),%rdi

# qhasm:   (uint64) x15 >>= 32
# asm 1: shr  $32,<x15=int64#1
# asm 2: shr  $32,<x15=%rdi
shr  $32,%rdi

# qhasm:   x15 <<= 32
# asm 1: shl  $32,<x15=int64#1
# asm 2: shl  $32,<x15=%rdi
shl  $32,%rdi

# qhasm:   x14 += x15
# asm 1: add  <x15=int64#1,<x14=int64#14
# asm 2: add  <x15=%rdi,<x14=%rbx
add  %rdi,%rbx

# qhasm:   out = out_backup
# asm 1: movq <out_backup=stack64#18,>out=int64#1
# asm 2: movq <out_backup=136(%rsp),>out=%rdi
movq 136(%rsp),%rdi

# qhasm:   m = m_backup
# asm 1: movq <m_backup=stack64#19,>m=int64#2
# asm 2: movq <m_backup=144(%rsp),>m=%rsi
movq 144(%rsp),%rsi

# qhasm:   x0 ^= *(uint64 *) (m + 0)
# asm 1: xorq 0(<m=int64#2),<x0=int64#3
# asm 2: xorq 0(<m=%rsi),<x0=%rdx
xorq 0(%rsi),%rdx

# qhasm:   *(uint64 *) (out + 0) = x0
# asm 1: movq   <x0=int64#3,0(<out=int64#1)
# asm 2: movq   <x0=%rdx,0(<out=%rdi)
movq   %rdx,0(%rdi)

# qhasm:   x2 ^= *(uint64 *) (m + 8)
# asm 1: xorq 8(<m=int64#2),<x2=int64#4
# asm 2: xorq 8(<m=%rsi),<x2=%rcx
xorq 8(%rsi),%rcx

# qhasm:   *(uint64 *) (out + 8) = x2
# asm 1: movq   <x2=int64#4,8(<out=int64#1)
# asm 2: movq   <x2=%rcx,8(<out=%rdi)
movq   %rcx,8(%rdi)

# qhasm:   x4 ^= *(uint64 *) (m + 16)
# asm 1: xorq 16(<m=int64#2),<x4=int64#6
# asm 2: xorq 16(<m=%rsi),<x4=%r9
xorq 16(%rsi),%r9

# qhasm:   *(uint64 *) (out + 16) = x4
# asm 1: movq   <x4=int64#6,16(<out=int64#1)
# asm 2: movq   <x4=%r9,16(<out=%rdi)
movq   %r9,16(%rdi)

# qhasm:   x6 ^= *(uint64 *) (m + 24)
# asm 1: xorq 24(<m=int64#2),<x6=int64#7
# asm 2: xorq 24(<m=%rsi),<x6=%rax
xorq 24(%rsi),%rax

# qhasm:   *(uint64 *) (out + 24) = x6
# asm 1: movq   <x6=int64#7,24(<out=int64#1)
# asm 2: movq   <x6=%rax,24(<out=%rdi)
movq   %rax,24(%rdi)

# qhasm:   x8 ^= *(uint64 *) (m + 32)
# asm 1: xorq 32(<m=int64#2),<x8=int64#9
# asm 2: xorq 32(<m=%rsi),<x8=%r11
xorq 32(%rsi),%r11

# qhasm:   *(uint64 *) (out + 32) = x8
# asm 1: movq   <x8=int64#9,32(<out=int64#1)
# asm 2: movq   <x8=%r11,32(<out=%rdi)
movq   %r11,32(%rdi)

# qhasm:   x10 ^= *(uint64 *) (m + 40)
# asm 1: xorq 40(<m=int64#2),<x10=int64#5
# asm 2: xorq 40(<m=%rsi),<x10=%r8
xorq 40(%rsi),%r8

# qhasm:   *(uint64 *) (out + 40) = x10
# asm 1: movq   <x10=int64#5,40(<out=int64#1)
# asm 2: movq   <x10=%r8,40(<out=%rdi)
movq   %r8,40(%rdi)

# qhasm:   x12 ^= *(uint64 *) (m + 48)
# asm 1: xorq 48(<m=int64#2),<x12=int64#12
# asm 2: xorq 48(<m=%rsi),<x12=%r14
xorq 48(%rsi),%r14

# qhasm:   *(uint64 *) (out + 48) = x12
# asm 1: movq   <x12=int64#12,48(<out=int64#1)
# asm 2: movq   <x12=%r14,48(<out=%rdi)
movq   %r14,48(%rdi)

# qhasm:   x14 ^= *(uint64 *) (m + 56)
# asm 1: xorq 56(<m=int64#2),<x14=int64#14
# asm 2: xorq 56(<m=%rsi),<x14=%rbx
xorq 56(%rsi),%rbx

# qhasm:   *(uint64 *) (out + 56) = x14
# asm 1: movq   <x14=int64#14,56(<out=int64#1)
# asm 2: movq   <x14=%rbx,56(<out=%rdi)
movq   %rbx,56(%rdi)

# qhasm:   bytes = bytes_backup
# asm 1: movq <bytes_backup=stack64#20,>bytes=int64#3
# asm 2: movq <bytes_backup=152(%rsp),>bytes=%rdx
movq 152(%rsp),%rdx

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
# asm 1: movq <ctarget=stack64#17,>out=int64#1
# asm 2: movq <ctarget=128(%rsp),>out=%rdi
movq 128(%rsp),%rdi

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
add $256,%r11
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
add $256,%r11
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
add $256,%r11
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
