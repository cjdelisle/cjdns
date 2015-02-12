
# qhasm: int32 a

# qhasm: int32 p

# qhasm: int32 s

# qhasm: int32 t

# qhasm: int32 w

# qhasm: int32 r

# qhasm: int32 v

# qhasm: stack32 arg1

# qhasm: stack32 arg2

# qhasm: stack32 arg3

# qhasm: stack32 arg4

# qhasm: input arg1

# qhasm: input arg2

# qhasm: input arg3

# qhasm: input arg4

# qhasm: int32 eax

# qhasm: int32 ebx

# qhasm: int32 esi

# qhasm: int32 edi

# qhasm: int32 ebp

# qhasm: caller eax

# qhasm: caller ebx

# qhasm: caller esi

# qhasm: caller edi

# qhasm: caller ebp

# qhasm: int32 k

# qhasm: int32 kbits

# qhasm: int32 iv

# qhasm: int32 i

# qhasm: stack32 x_backup

# qhasm: int32 x

# qhasm: stack32 m_backup

# qhasm: int32 m

# qhasm: stack32 out_backup

# qhasm: int32 out

# qhasm: stack32 bytes_backup

# qhasm: int32 bytes

# qhasm: stack32 eax_stack

# qhasm: stack32 ebx_stack

# qhasm: stack32 esi_stack

# qhasm: stack32 edi_stack

# qhasm: stack32 ebp_stack

# qhasm: int32 in0

# qhasm: int32 in1

# qhasm: int32 in2

# qhasm: int32 in3

# qhasm: int32 in4

# qhasm: int32 in5

# qhasm: int32 in6

# qhasm: int32 in7

# qhasm: int32 in8

# qhasm: int32 in9

# qhasm: int32 in10

# qhasm: int32 in11

# qhasm: int32 in12

# qhasm: int32 in13

# qhasm: int32 in14

# qhasm: int32 in15

# qhasm: stack32 x0

# qhasm: stack32 x1

# qhasm: stack32 x2

# qhasm: stack32 x3

# qhasm: stack32 x4

# qhasm: stack32 x5

# qhasm: stack32 x6

# qhasm: stack32 x7

# qhasm: stack32 x8

# qhasm: stack32 x9

# qhasm: stack32 x10

# qhasm: stack32 x11

# qhasm: stack32 x12

# qhasm: stack32 x13

# qhasm: stack32 x14

# qhasm: stack32 x15

# qhasm: stack32 j0

# qhasm: stack32 j1

# qhasm: stack32 j2

# qhasm: stack32 j3

# qhasm: stack32 j4

# qhasm: stack32 j5

# qhasm: stack32 j6

# qhasm: stack32 j7

# qhasm: stack32 j8

# qhasm: stack32 j9

# qhasm: stack32 j10

# qhasm: stack32 j11

# qhasm: stack32 j12

# qhasm: stack32 j13

# qhasm: stack32 j14

# qhasm: stack32 j15

# qhasm: stack512 tmp

# qhasm: stack32 ctarget

# qhasm: enter ECRYPT_keystream_bytes
.text
.p2align 5
.globl _ECRYPT_keystream_bytes
.globl ECRYPT_keystream_bytes
_ECRYPT_keystream_bytes:
ECRYPT_keystream_bytes:
mov %esp,%eax
and $31,%eax
add $256,%eax
sub %eax,%esp

# qhasm: eax_stack = eax
# asm 1: movl <eax=int32#1,>eax_stack=stack32#1
# asm 2: movl <eax=%eax,>eax_stack=0(%esp)
movl %eax,0(%esp)

# qhasm: ebx_stack = ebx
# asm 1: movl <ebx=int32#4,>ebx_stack=stack32#2
# asm 2: movl <ebx=%ebx,>ebx_stack=4(%esp)
movl %ebx,4(%esp)

# qhasm: esi_stack = esi
# asm 1: movl <esi=int32#5,>esi_stack=stack32#3
# asm 2: movl <esi=%esi,>esi_stack=8(%esp)
movl %esi,8(%esp)

# qhasm: edi_stack = edi
# asm 1: movl <edi=int32#6,>edi_stack=stack32#4
# asm 2: movl <edi=%edi,>edi_stack=12(%esp)
movl %edi,12(%esp)

# qhasm: ebp_stack = ebp
# asm 1: movl <ebp=int32#7,>ebp_stack=stack32#5
# asm 2: movl <ebp=%ebp,>ebp_stack=16(%esp)
movl %ebp,16(%esp)

# qhasm: x = arg1
# asm 1: movl <arg1=stack32#-1,>x=int32#3
# asm 2: movl <arg1=4(%esp,%eax),>x=%edx
movl 4(%esp,%eax),%edx

# qhasm: m = arg2
# asm 1: movl <arg2=stack32#-2,>m=int32#5
# asm 2: movl <arg2=8(%esp,%eax),>m=%esi
movl 8(%esp,%eax),%esi

# qhasm: out = m
# asm 1: mov  <m=int32#5,>out=int32#6
# asm 2: mov  <m=%esi,>out=%edi
mov  %esi,%edi

# qhasm: bytes = arg3
# asm 1: movl <arg3=stack32#-3,>bytes=int32#4
# asm 2: movl <arg3=12(%esp,%eax),>bytes=%ebx
movl 12(%esp,%eax),%ebx

# qhasm:               unsigned>? bytes - 0
# asm 1: cmp  $0,<bytes=int32#4
# asm 2: cmp  $0,<bytes=%ebx
cmp  $0,%ebx
# comment:fp stack unchanged by jump

# qhasm: goto done if !unsigned>
jbe ._done

# qhasm: a = 0
# asm 1: mov  $0,>a=int32#1
# asm 2: mov  $0,>a=%eax
mov  $0,%eax

# qhasm: i = bytes
# asm 1: mov  <bytes=int32#4,>i=int32#2
# asm 2: mov  <bytes=%ebx,>i=%ecx
mov  %ebx,%ecx

# qhasm: while (i) { *out++ = a; --i }
rep stosb

# qhasm: out -= bytes
# asm 1: sub  <bytes=int32#4,<out=int32#6
# asm 2: sub  <bytes=%ebx,<out=%edi
sub  %ebx,%edi

# qhasm: x0 = a
# asm 1: movl <a=int32#1,>x0=stack32#6
# asm 2: movl <a=%eax,>x0=20(%esp)
movl %eax,20(%esp)

# qhasm: x1 = a
# asm 1: movl <a=int32#1,>x1=stack32#7
# asm 2: movl <a=%eax,>x1=24(%esp)
movl %eax,24(%esp)

# qhasm: x2 = a
# asm 1: movl <a=int32#1,>x2=stack32#8
# asm 2: movl <a=%eax,>x2=28(%esp)
movl %eax,28(%esp)

# qhasm: x3 = a
# asm 1: movl <a=int32#1,>x3=stack32#9
# asm 2: movl <a=%eax,>x3=32(%esp)
movl %eax,32(%esp)

# qhasm: x4 = a
# asm 1: movl <a=int32#1,>x4=stack32#10
# asm 2: movl <a=%eax,>x4=36(%esp)
movl %eax,36(%esp)

# qhasm: x5 = a
# asm 1: movl <a=int32#1,>x5=stack32#11
# asm 2: movl <a=%eax,>x5=40(%esp)
movl %eax,40(%esp)

# qhasm: x6 = a
# asm 1: movl <a=int32#1,>x6=stack32#12
# asm 2: movl <a=%eax,>x6=44(%esp)
movl %eax,44(%esp)

# qhasm: x7 = a
# asm 1: movl <a=int32#1,>x7=stack32#13
# asm 2: movl <a=%eax,>x7=48(%esp)
movl %eax,48(%esp)

# qhasm: x8 = a
# asm 1: movl <a=int32#1,>x8=stack32#14
# asm 2: movl <a=%eax,>x8=52(%esp)
movl %eax,52(%esp)

# qhasm: x9 = a
# asm 1: movl <a=int32#1,>x9=stack32#15
# asm 2: movl <a=%eax,>x9=56(%esp)
movl %eax,56(%esp)

# qhasm: x10 = a
# asm 1: movl <a=int32#1,>x10=stack32#16
# asm 2: movl <a=%eax,>x10=60(%esp)
movl %eax,60(%esp)

# qhasm: x11 = a
# asm 1: movl <a=int32#1,>x11=stack32#17
# asm 2: movl <a=%eax,>x11=64(%esp)
movl %eax,64(%esp)

# qhasm: x12 = a
# asm 1: movl <a=int32#1,>x12=stack32#18
# asm 2: movl <a=%eax,>x12=68(%esp)
movl %eax,68(%esp)

# qhasm: x13 = a
# asm 1: movl <a=int32#1,>x13=stack32#19
# asm 2: movl <a=%eax,>x13=72(%esp)
movl %eax,72(%esp)

# qhasm: x14 = a
# asm 1: movl <a=int32#1,>x14=stack32#20
# asm 2: movl <a=%eax,>x14=76(%esp)
movl %eax,76(%esp)

# qhasm: x15 = a
# asm 1: movl <a=int32#1,>x15=stack32#21
# asm 2: movl <a=%eax,>x15=80(%esp)
movl %eax,80(%esp)
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
mov %esp,%eax
and $31,%eax
add $256,%eax
sub %eax,%esp

# qhasm: eax_stack = eax
# asm 1: movl <eax=int32#1,>eax_stack=stack32#1
# asm 2: movl <eax=%eax,>eax_stack=0(%esp)
movl %eax,0(%esp)

# qhasm: ebx_stack = ebx
# asm 1: movl <ebx=int32#4,>ebx_stack=stack32#2
# asm 2: movl <ebx=%ebx,>ebx_stack=4(%esp)
movl %ebx,4(%esp)

# qhasm: esi_stack = esi
# asm 1: movl <esi=int32#5,>esi_stack=stack32#3
# asm 2: movl <esi=%esi,>esi_stack=8(%esp)
movl %esi,8(%esp)

# qhasm: edi_stack = edi
# asm 1: movl <edi=int32#6,>edi_stack=stack32#4
# asm 2: movl <edi=%edi,>edi_stack=12(%esp)
movl %edi,12(%esp)

# qhasm: ebp_stack = ebp
# asm 1: movl <ebp=int32#7,>ebp_stack=stack32#5
# asm 2: movl <ebp=%ebp,>ebp_stack=16(%esp)
movl %ebp,16(%esp)

# qhasm: x = arg1
# asm 1: movl <arg1=stack32#-1,>x=int32#3
# asm 2: movl <arg1=4(%esp,%eax),>x=%edx
movl 4(%esp,%eax),%edx

# qhasm: m = arg2
# asm 1: movl <arg2=stack32#-2,>m=int32#5
# asm 2: movl <arg2=8(%esp,%eax),>m=%esi
movl 8(%esp,%eax),%esi

# qhasm: out = arg3
# asm 1: movl <arg3=stack32#-3,>out=int32#6
# asm 2: movl <arg3=12(%esp,%eax),>out=%edi
movl 12(%esp,%eax),%edi

# qhasm: bytes = arg4
# asm 1: movl <arg4=stack32#-4,>bytes=int32#4
# asm 2: movl <arg4=16(%esp,%eax),>bytes=%ebx
movl 16(%esp,%eax),%ebx

# qhasm:               unsigned>? bytes - 0
# asm 1: cmp  $0,<bytes=int32#4
# asm 2: cmp  $0,<bytes=%ebx
cmp  $0,%ebx
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
mov %esp,%eax
and $31,%eax
add $256,%eax
sub %eax,%esp

# qhasm: eax_stack = eax
# asm 1: movl <eax=int32#1,>eax_stack=stack32#1
# asm 2: movl <eax=%eax,>eax_stack=0(%esp)
movl %eax,0(%esp)

# qhasm: ebx_stack = ebx
# asm 1: movl <ebx=int32#4,>ebx_stack=stack32#2
# asm 2: movl <ebx=%ebx,>ebx_stack=4(%esp)
movl %ebx,4(%esp)

# qhasm: esi_stack = esi
# asm 1: movl <esi=int32#5,>esi_stack=stack32#3
# asm 2: movl <esi=%esi,>esi_stack=8(%esp)
movl %esi,8(%esp)

# qhasm: edi_stack = edi
# asm 1: movl <edi=int32#6,>edi_stack=stack32#4
# asm 2: movl <edi=%edi,>edi_stack=12(%esp)
movl %edi,12(%esp)

# qhasm: ebp_stack = ebp
# asm 1: movl <ebp=int32#7,>ebp_stack=stack32#5
# asm 2: movl <ebp=%ebp,>ebp_stack=16(%esp)
movl %ebp,16(%esp)

# qhasm: x = arg1
# asm 1: movl <arg1=stack32#-1,>x=int32#3
# asm 2: movl <arg1=4(%esp,%eax),>x=%edx
movl 4(%esp,%eax),%edx

# qhasm: m = arg2
# asm 1: movl <arg2=stack32#-2,>m=int32#5
# asm 2: movl <arg2=8(%esp,%eax),>m=%esi
movl 8(%esp,%eax),%esi

# qhasm: out = arg3
# asm 1: movl <arg3=stack32#-3,>out=int32#6
# asm 2: movl <arg3=12(%esp,%eax),>out=%edi
movl 12(%esp,%eax),%edi

# qhasm: bytes = arg4
# asm 1: movl <arg4=stack32#-4,>bytes=int32#4
# asm 2: movl <arg4=16(%esp,%eax),>bytes=%ebx
movl 16(%esp,%eax),%ebx

# qhasm:               unsigned>? bytes - 0
# asm 1: cmp  $0,<bytes=int32#4
# asm 2: cmp  $0,<bytes=%ebx
cmp  $0,%ebx
# comment:fp stack unchanged by jump

# qhasm: goto done if !unsigned>
jbe ._done
# comment:fp stack unchanged by fallthrough

# qhasm: start:
._start:

# qhasm: in0 = *(uint32 *) (x + 0)
# asm 1: movl 0(<x=int32#3),>in0=int32#1
# asm 2: movl 0(<x=%edx),>in0=%eax
movl 0(%edx),%eax

# qhasm: in1 = *(uint32 *) (x + 4)
# asm 1: movl 4(<x=int32#3),>in1=int32#2
# asm 2: movl 4(<x=%edx),>in1=%ecx
movl 4(%edx),%ecx

# qhasm: in2 = *(uint32 *) (x + 8)
# asm 1: movl 8(<x=int32#3),>in2=int32#7
# asm 2: movl 8(<x=%edx),>in2=%ebp
movl 8(%edx),%ebp

# qhasm: j0 = in0
# asm 1: movl <in0=int32#1,>j0=stack32#22
# asm 2: movl <in0=%eax,>j0=84(%esp)
movl %eax,84(%esp)

# qhasm: in3 = *(uint32 *) (x + 12)
# asm 1: movl 12(<x=int32#3),>in3=int32#1
# asm 2: movl 12(<x=%edx),>in3=%eax
movl 12(%edx),%eax

# qhasm: j1 = in1
# asm 1: movl <in1=int32#2,>j1=stack32#23
# asm 2: movl <in1=%ecx,>j1=88(%esp)
movl %ecx,88(%esp)

# qhasm: in4 = *(uint32 *) (x + 16)
# asm 1: movl 16(<x=int32#3),>in4=int32#2
# asm 2: movl 16(<x=%edx),>in4=%ecx
movl 16(%edx),%ecx

# qhasm: j2 = in2
# asm 1: movl <in2=int32#7,>j2=stack32#24
# asm 2: movl <in2=%ebp,>j2=92(%esp)
movl %ebp,92(%esp)

# qhasm: in5 = *(uint32 *) (x + 20)
# asm 1: movl 20(<x=int32#3),>in5=int32#7
# asm 2: movl 20(<x=%edx),>in5=%ebp
movl 20(%edx),%ebp

# qhasm: j3 = in3
# asm 1: movl <in3=int32#1,>j3=stack32#25
# asm 2: movl <in3=%eax,>j3=96(%esp)
movl %eax,96(%esp)

# qhasm: in6 = *(uint32 *) (x + 24)
# asm 1: movl 24(<x=int32#3),>in6=int32#1
# asm 2: movl 24(<x=%edx),>in6=%eax
movl 24(%edx),%eax

# qhasm: j4 = in4
# asm 1: movl <in4=int32#2,>j4=stack32#26
# asm 2: movl <in4=%ecx,>j4=100(%esp)
movl %ecx,100(%esp)

# qhasm: in7 = *(uint32 *) (x + 28)
# asm 1: movl 28(<x=int32#3),>in7=int32#2
# asm 2: movl 28(<x=%edx),>in7=%ecx
movl 28(%edx),%ecx

# qhasm: j5 = in5
# asm 1: movl <in5=int32#7,>j5=stack32#27
# asm 2: movl <in5=%ebp,>j5=104(%esp)
movl %ebp,104(%esp)

# qhasm: in8 = *(uint32 *) (x + 32)
# asm 1: movl 32(<x=int32#3),>in8=int32#7
# asm 2: movl 32(<x=%edx),>in8=%ebp
movl 32(%edx),%ebp

# qhasm: j6 = in6
# asm 1: movl <in6=int32#1,>j6=stack32#28
# asm 2: movl <in6=%eax,>j6=108(%esp)
movl %eax,108(%esp)

# qhasm: in9 = *(uint32 *) (x + 36)
# asm 1: movl 36(<x=int32#3),>in9=int32#1
# asm 2: movl 36(<x=%edx),>in9=%eax
movl 36(%edx),%eax

# qhasm: j7 = in7
# asm 1: movl <in7=int32#2,>j7=stack32#29
# asm 2: movl <in7=%ecx,>j7=112(%esp)
movl %ecx,112(%esp)

# qhasm: in10 = *(uint32 *) (x + 40)
# asm 1: movl 40(<x=int32#3),>in10=int32#2
# asm 2: movl 40(<x=%edx),>in10=%ecx
movl 40(%edx),%ecx

# qhasm: j8 = in8
# asm 1: movl <in8=int32#7,>j8=stack32#30
# asm 2: movl <in8=%ebp,>j8=116(%esp)
movl %ebp,116(%esp)

# qhasm: in11 = *(uint32 *) (x + 44)
# asm 1: movl 44(<x=int32#3),>in11=int32#7
# asm 2: movl 44(<x=%edx),>in11=%ebp
movl 44(%edx),%ebp

# qhasm: j9 = in9
# asm 1: movl <in9=int32#1,>j9=stack32#31
# asm 2: movl <in9=%eax,>j9=120(%esp)
movl %eax,120(%esp)

# qhasm: in12 = *(uint32 *) (x + 48)
# asm 1: movl 48(<x=int32#3),>in12=int32#1
# asm 2: movl 48(<x=%edx),>in12=%eax
movl 48(%edx),%eax

# qhasm: j10 = in10
# asm 1: movl <in10=int32#2,>j10=stack32#32
# asm 2: movl <in10=%ecx,>j10=124(%esp)
movl %ecx,124(%esp)

# qhasm: in13 = *(uint32 *) (x + 52)
# asm 1: movl 52(<x=int32#3),>in13=int32#2
# asm 2: movl 52(<x=%edx),>in13=%ecx
movl 52(%edx),%ecx

# qhasm: j11 = in11
# asm 1: movl <in11=int32#7,>j11=stack32#33
# asm 2: movl <in11=%ebp,>j11=128(%esp)
movl %ebp,128(%esp)

# qhasm: in14 = *(uint32 *) (x + 56)
# asm 1: movl 56(<x=int32#3),>in14=int32#7
# asm 2: movl 56(<x=%edx),>in14=%ebp
movl 56(%edx),%ebp

# qhasm: j12 = in12
# asm 1: movl <in12=int32#1,>j12=stack32#34
# asm 2: movl <in12=%eax,>j12=132(%esp)
movl %eax,132(%esp)

# qhasm: in15 = *(uint32 *) (x + 60)
# asm 1: movl 60(<x=int32#3),>in15=int32#1
# asm 2: movl 60(<x=%edx),>in15=%eax
movl 60(%edx),%eax

# qhasm: j13 = in13
# asm 1: movl <in13=int32#2,>j13=stack32#35
# asm 2: movl <in13=%ecx,>j13=136(%esp)
movl %ecx,136(%esp)

# qhasm: j14 = in14
# asm 1: movl <in14=int32#7,>j14=stack32#36
# asm 2: movl <in14=%ebp,>j14=140(%esp)
movl %ebp,140(%esp)

# qhasm: j15 = in15
# asm 1: movl <in15=int32#1,>j15=stack32#37
# asm 2: movl <in15=%eax,>j15=144(%esp)
movl %eax,144(%esp)

# qhasm: x_backup = x
# asm 1: movl <x=int32#3,>x_backup=stack32#38
# asm 2: movl <x=%edx,>x_backup=148(%esp)
movl %edx,148(%esp)

# qhasm: bytesatleast1:
._bytesatleast1:

# qhasm:                   unsigned<? bytes - 64
# asm 1: cmp  $64,<bytes=int32#4
# asm 2: cmp  $64,<bytes=%ebx
cmp  $64,%ebx
# comment:fp stack unchanged by jump

# qhasm:   goto nocopy if !unsigned<
jae ._nocopy

# qhasm:     ctarget = out
# asm 1: movl <out=int32#6,>ctarget=stack32#39
# asm 2: movl <out=%edi,>ctarget=152(%esp)
movl %edi,152(%esp)

# qhasm:     out = &tmp
# asm 1: leal <tmp=stack512#1,>out=int32#6
# asm 2: leal <tmp=192(%esp),>out=%edi
leal 192(%esp),%edi

# qhasm:     i = bytes
# asm 1: mov  <bytes=int32#4,>i=int32#2
# asm 2: mov  <bytes=%ebx,>i=%ecx
mov  %ebx,%ecx

# qhasm:     while (i) { *out++ = *m++; --i }
rep movsb

# qhasm:     out = &tmp
# asm 1: leal <tmp=stack512#1,>out=int32#6
# asm 2: leal <tmp=192(%esp),>out=%edi
leal 192(%esp),%edi

# qhasm:     m = &tmp
# asm 1: leal <tmp=stack512#1,>m=int32#5
# asm 2: leal <tmp=192(%esp),>m=%esi
leal 192(%esp),%esi
# comment:fp stack unchanged by fallthrough

# qhasm:   nocopy:
._nocopy:

# qhasm:   out_backup = out
# asm 1: movl <out=int32#6,>out_backup=stack32#40
# asm 2: movl <out=%edi,>out_backup=156(%esp)
movl %edi,156(%esp)

# qhasm:   m_backup = m
# asm 1: movl <m=int32#5,>m_backup=stack32#41
# asm 2: movl <m=%esi,>m_backup=160(%esp)
movl %esi,160(%esp)

# qhasm:   bytes_backup = bytes
# asm 1: movl <bytes=int32#4,>bytes_backup=stack32#42
# asm 2: movl <bytes=%ebx,>bytes_backup=164(%esp)
movl %ebx,164(%esp)

# qhasm:   in0 = j0
# asm 1: movl <j0=stack32#22,>in0=int32#1
# asm 2: movl <j0=84(%esp),>in0=%eax
movl 84(%esp),%eax

# qhasm:   in1 = j1
# asm 1: movl <j1=stack32#23,>in1=int32#2
# asm 2: movl <j1=88(%esp),>in1=%ecx
movl 88(%esp),%ecx

# qhasm:   in2 = j2
# asm 1: movl <j2=stack32#24,>in2=int32#3
# asm 2: movl <j2=92(%esp),>in2=%edx
movl 92(%esp),%edx

# qhasm:   in3 = j3
# asm 1: movl <j3=stack32#25,>in3=int32#4
# asm 2: movl <j3=96(%esp),>in3=%ebx
movl 96(%esp),%ebx

# qhasm:   x0 = in0
# asm 1: movl <in0=int32#1,>x0=stack32#6
# asm 2: movl <in0=%eax,>x0=20(%esp)
movl %eax,20(%esp)

# qhasm:   x1 = in1
# asm 1: movl <in1=int32#2,>x1=stack32#7
# asm 2: movl <in1=%ecx,>x1=24(%esp)
movl %ecx,24(%esp)

# qhasm:   x2 = in2
# asm 1: movl <in2=int32#3,>x2=stack32#8
# asm 2: movl <in2=%edx,>x2=28(%esp)
movl %edx,28(%esp)

# qhasm:   x3 = in3
# asm 1: movl <in3=int32#4,>x3=stack32#9
# asm 2: movl <in3=%ebx,>x3=32(%esp)
movl %ebx,32(%esp)

# qhasm:   in4 = j4
# asm 1: movl <j4=stack32#26,>in4=int32#1
# asm 2: movl <j4=100(%esp),>in4=%eax
movl 100(%esp),%eax

# qhasm:   in5 = j5
# asm 1: movl <j5=stack32#27,>in5=int32#2
# asm 2: movl <j5=104(%esp),>in5=%ecx
movl 104(%esp),%ecx

# qhasm:   in6 = j6
# asm 1: movl <j6=stack32#28,>in6=int32#3
# asm 2: movl <j6=108(%esp),>in6=%edx
movl 108(%esp),%edx

# qhasm:   in7 = j7
# asm 1: movl <j7=stack32#29,>in7=int32#4
# asm 2: movl <j7=112(%esp),>in7=%ebx
movl 112(%esp),%ebx

# qhasm:   x4 = in4
# asm 1: movl <in4=int32#1,>x4=stack32#10
# asm 2: movl <in4=%eax,>x4=36(%esp)
movl %eax,36(%esp)

# qhasm:   x5 = in5
# asm 1: movl <in5=int32#2,>x5=stack32#11
# asm 2: movl <in5=%ecx,>x5=40(%esp)
movl %ecx,40(%esp)

# qhasm:   x6 = in6
# asm 1: movl <in6=int32#3,>x6=stack32#12
# asm 2: movl <in6=%edx,>x6=44(%esp)
movl %edx,44(%esp)

# qhasm:   x7 = in7
# asm 1: movl <in7=int32#4,>x7=stack32#13
# asm 2: movl <in7=%ebx,>x7=48(%esp)
movl %ebx,48(%esp)

# qhasm:   in8 = j8
# asm 1: movl <j8=stack32#30,>in8=int32#1
# asm 2: movl <j8=116(%esp),>in8=%eax
movl 116(%esp),%eax

# qhasm:   in9 = j9
# asm 1: movl <j9=stack32#31,>in9=int32#2
# asm 2: movl <j9=120(%esp),>in9=%ecx
movl 120(%esp),%ecx

# qhasm:   in10 = j10
# asm 1: movl <j10=stack32#32,>in10=int32#3
# asm 2: movl <j10=124(%esp),>in10=%edx
movl 124(%esp),%edx

# qhasm:   in11 = j11
# asm 1: movl <j11=stack32#33,>in11=int32#4
# asm 2: movl <j11=128(%esp),>in11=%ebx
movl 128(%esp),%ebx

# qhasm:   x8 = in8
# asm 1: movl <in8=int32#1,>x8=stack32#14
# asm 2: movl <in8=%eax,>x8=52(%esp)
movl %eax,52(%esp)

# qhasm:   x9 = in9
# asm 1: movl <in9=int32#2,>x9=stack32#15
# asm 2: movl <in9=%ecx,>x9=56(%esp)
movl %ecx,56(%esp)

# qhasm:   x10 = in10
# asm 1: movl <in10=int32#3,>x10=stack32#16
# asm 2: movl <in10=%edx,>x10=60(%esp)
movl %edx,60(%esp)

# qhasm:   x11 = in11
# asm 1: movl <in11=int32#4,>x11=stack32#17
# asm 2: movl <in11=%ebx,>x11=64(%esp)
movl %ebx,64(%esp)

# qhasm:   in12 = j12
# asm 1: movl <j12=stack32#34,>in12=int32#1
# asm 2: movl <j12=132(%esp),>in12=%eax
movl 132(%esp),%eax

# qhasm:   in13 = j13
# asm 1: movl <j13=stack32#35,>in13=int32#2
# asm 2: movl <j13=136(%esp),>in13=%ecx
movl 136(%esp),%ecx

# qhasm:   in14 = j14
# asm 1: movl <j14=stack32#36,>in14=int32#3
# asm 2: movl <j14=140(%esp),>in14=%edx
movl 140(%esp),%edx

# qhasm:   in15 = j15
# asm 1: movl <j15=stack32#37,>in15=int32#4
# asm 2: movl <j15=144(%esp),>in15=%ebx
movl 144(%esp),%ebx

# qhasm:   x12 = in12
# asm 1: movl <in12=int32#1,>x12=stack32#18
# asm 2: movl <in12=%eax,>x12=68(%esp)
movl %eax,68(%esp)

# qhasm:   x13 = in13
# asm 1: movl <in13=int32#2,>x13=stack32#19
# asm 2: movl <in13=%ecx,>x13=72(%esp)
movl %ecx,72(%esp)

# qhasm:   x14 = in14
# asm 1: movl <in14=int32#3,>x14=stack32#20
# asm 2: movl <in14=%edx,>x14=76(%esp)
movl %edx,76(%esp)

# qhasm:   x15 = in15
# asm 1: movl <in15=int32#4,>x15=stack32#21
# asm 2: movl <in15=%ebx,>x15=80(%esp)
movl %ebx,80(%esp)

# qhasm:   i = 20
# asm 1: mov  $20,>i=int32#2
# asm 2: mov  $20,>i=%ecx
mov  $20,%ecx

# qhasm: p = x0
# asm 1: movl <x0=stack32#6,>p=int32#1
# asm 2: movl <x0=20(%esp),>p=%eax
movl 20(%esp),%eax

# qhasm: s = x5
# asm 1: movl <x5=stack32#11,>s=int32#3
# asm 2: movl <x5=40(%esp),>s=%edx
.byte 0xf3
movl 40(%esp),%edx

# qhasm: t = x10
# asm 1: movl <x10=stack32#16,>t=int32#4
# asm 2: movl <x10=60(%esp),>t=%ebx
.byte 0xf3
movl 60(%esp),%ebx

# qhasm: w = x15
# asm 1: movl <x15=stack32#21,>w=int32#5
# asm 2: movl <x15=80(%esp),>w=%esi
.byte 0xf3
movl 80(%esp),%esi

# qhasm: mainloop:
._mainloop:

# qhasm: x0 = p
# asm 1: movl <p=int32#1,>x0=stack32#6
# asm 2: movl <p=%eax,>x0=20(%esp)
movl %eax,20(%esp)

# qhasm: 				x10 = t
# asm 1: movl <t=int32#4,>x10=stack32#11
# asm 2: movl <t=%ebx,>x10=40(%esp)
movl %ebx,40(%esp)

# qhasm: p += x12
# asm 1: addl <x12=stack32#18,<p=int32#1
# asm 2: addl <x12=68(%esp),<p=%eax
addl 68(%esp),%eax

# qhasm: 		x5 = s
# asm 1: movl <s=int32#3,>x5=stack32#16
# asm 2: movl <s=%edx,>x5=60(%esp)
movl %edx,60(%esp)

# qhasm: 				t += x6
# asm 1: addl <x6=stack32#12,<t=int32#4
# asm 2: addl <x6=44(%esp),<t=%ebx
addl 44(%esp),%ebx

# qhasm: 						x15 = w
# asm 1: movl <w=int32#5,>x15=stack32#21
# asm 2: movl <w=%esi,>x15=80(%esp)
movl %esi,80(%esp)

# qhasm: 		r = x1
# asm 1: movl <x1=stack32#7,>r=int32#6
# asm 2: movl <x1=24(%esp),>r=%edi
.byte 0xf3
movl 24(%esp),%edi

# qhasm: 		r += s
# asm 1: add  <s=int32#3,<r=int32#6
# asm 2: add  <s=%edx,<r=%edi
.byte 0xf3
add  %edx,%edi

# qhasm: 						v = x11
# asm 1: movl <x11=stack32#17,>v=int32#7
# asm 2: movl <x11=64(%esp),>v=%ebp
movl 64(%esp),%ebp

# qhasm: 						v += w
# asm 1: add  <w=int32#5,<v=int32#7
# asm 2: add  <w=%esi,<v=%ebp
add  %esi,%ebp

# qhasm: p <<<= 7
# asm 1: rol  $7,<p=int32#1
# asm 2: rol  $7,<p=%eax
rol  $7,%eax

# qhasm: p ^= x4
# asm 1: xorl <x4=stack32#10,<p=int32#1
# asm 2: xorl <x4=36(%esp),<p=%eax
xorl 36(%esp),%eax

# qhasm: 				t <<<= 7
# asm 1: rol  $7,<t=int32#4
# asm 2: rol  $7,<t=%ebx
rol  $7,%ebx

# qhasm: 				t ^= x14
# asm 1: xorl <x14=stack32#20,<t=int32#4
# asm 2: xorl <x14=76(%esp),<t=%ebx
xorl 76(%esp),%ebx

# qhasm: 		r <<<= 7
# asm 1: rol  $7,<r=int32#6
# asm 2: rol  $7,<r=%edi
.byte 0xf3
rol  $7,%edi

# qhasm: 		r ^= x9
# asm 1: xorl <x9=stack32#15,<r=int32#6
# asm 2: xorl <x9=56(%esp),<r=%edi
xorl 56(%esp),%edi

# qhasm: 						v <<<= 7
# asm 1: rol  $7,<v=int32#7
# asm 2: rol  $7,<v=%ebp
.byte 0xf3
rol  $7,%ebp

# qhasm: 						v ^= x3
# asm 1: xorl <x3=stack32#9,<v=int32#7
# asm 2: xorl <x3=32(%esp),<v=%ebp
xorl 32(%esp),%ebp

# qhasm: x4 = p
# asm 1: movl <p=int32#1,>x4=stack32#9
# asm 2: movl <p=%eax,>x4=32(%esp)
movl %eax,32(%esp)

# qhasm: 				x14 = t
# asm 1: movl <t=int32#4,>x14=stack32#10
# asm 2: movl <t=%ebx,>x14=36(%esp)
movl %ebx,36(%esp)

# qhasm: p += x0
# asm 1: addl <x0=stack32#6,<p=int32#1
# asm 2: addl <x0=20(%esp),<p=%eax
addl 20(%esp),%eax

# qhasm: 		x9 = r
# asm 1: movl <r=int32#6,>x9=stack32#15
# asm 2: movl <r=%edi,>x9=56(%esp)
movl %edi,56(%esp)

# qhasm: 				t += x10
# asm 1: addl <x10=stack32#11,<t=int32#4
# asm 2: addl <x10=40(%esp),<t=%ebx
addl 40(%esp),%ebx

# qhasm: 						x3 = v
# asm 1: movl <v=int32#7,>x3=stack32#20
# asm 2: movl <v=%ebp,>x3=76(%esp)
movl %ebp,76(%esp)

# qhasm: p <<<= 9
# asm 1: rol  $9,<p=int32#1
# asm 2: rol  $9,<p=%eax
.byte 0xf3
rol  $9,%eax

# qhasm: p ^= x8
# asm 1: xorl <x8=stack32#14,<p=int32#1
# asm 2: xorl <x8=52(%esp),<p=%eax
xorl 52(%esp),%eax

# qhasm: 				t <<<= 9
# asm 1: rol  $9,<t=int32#4
# asm 2: rol  $9,<t=%ebx
rol  $9,%ebx

# qhasm: 				t ^= x2
# asm 1: xorl <x2=stack32#8,<t=int32#4
# asm 2: xorl <x2=28(%esp),<t=%ebx
xorl 28(%esp),%ebx

# qhasm: 		s += r
# asm 1: add  <r=int32#6,<s=int32#3
# asm 2: add  <r=%edi,<s=%edx
add  %edi,%edx

# qhasm: 		s <<<= 9
# asm 1: rol  $9,<s=int32#3
# asm 2: rol  $9,<s=%edx
rol  $9,%edx

# qhasm: 		s ^= x13
# asm 1: xorl <x13=stack32#19,<s=int32#3
# asm 2: xorl <x13=72(%esp),<s=%edx
xorl 72(%esp),%edx

# qhasm: 						w += v
# asm 1: add  <v=int32#7,<w=int32#5
# asm 2: add  <v=%ebp,<w=%esi
.byte 0xf3
add  %ebp,%esi

# qhasm: 						w <<<= 9
# asm 1: rol  $9,<w=int32#5
# asm 2: rol  $9,<w=%esi
.byte 0xf3
rol  $9,%esi

# qhasm: 						w ^= x7
# asm 1: xorl <x7=stack32#13,<w=int32#5
# asm 2: xorl <x7=48(%esp),<w=%esi
.byte 0xf3
xorl 48(%esp),%esi

# qhasm: x8 = p
# asm 1: movl <p=int32#1,>x8=stack32#8
# asm 2: movl <p=%eax,>x8=28(%esp)
movl %eax,28(%esp)

# qhasm: 				x2 = t
# asm 1: movl <t=int32#4,>x2=stack32#13
# asm 2: movl <t=%ebx,>x2=48(%esp)
movl %ebx,48(%esp)

# qhasm: p += x4
# asm 1: addl <x4=stack32#9,<p=int32#1
# asm 2: addl <x4=32(%esp),<p=%eax
addl 32(%esp),%eax

# qhasm: 		x13 = s
# asm 1: movl <s=int32#3,>x13=stack32#14
# asm 2: movl <s=%edx,>x13=52(%esp)
movl %edx,52(%esp)

# qhasm: 				t += x14
# asm 1: addl <x14=stack32#10,<t=int32#4
# asm 2: addl <x14=36(%esp),<t=%ebx
addl 36(%esp),%ebx

# qhasm: 						x7 = w
# asm 1: movl <w=int32#5,>x7=stack32#19
# asm 2: movl <w=%esi,>x7=72(%esp)
movl %esi,72(%esp)

# qhasm: p <<<= 13
# asm 1: rol  $13,<p=int32#1
# asm 2: rol  $13,<p=%eax
.byte 0xf3
rol  $13,%eax

# qhasm: p ^= x12
# asm 1: xorl <x12=stack32#18,<p=int32#1
# asm 2: xorl <x12=68(%esp),<p=%eax
xorl 68(%esp),%eax

# qhasm: 				t <<<= 13
# asm 1: rol  $13,<t=int32#4
# asm 2: rol  $13,<t=%ebx
rol  $13,%ebx

# qhasm: 				t ^= x6
# asm 1: xorl <x6=stack32#12,<t=int32#4
# asm 2: xorl <x6=44(%esp),<t=%ebx
xorl 44(%esp),%ebx

# qhasm: 		r += s
# asm 1: add  <s=int32#3,<r=int32#6
# asm 2: add  <s=%edx,<r=%edi
add  %edx,%edi

# qhasm: 		r <<<= 13
# asm 1: rol  $13,<r=int32#6
# asm 2: rol  $13,<r=%edi
rol  $13,%edi

# qhasm: 		r ^= x1
# asm 1: xorl <x1=stack32#7,<r=int32#6
# asm 2: xorl <x1=24(%esp),<r=%edi
xorl 24(%esp),%edi

# qhasm: 						v += w
# asm 1: add  <w=int32#5,<v=int32#7
# asm 2: add  <w=%esi,<v=%ebp
.byte 0xf3
add  %esi,%ebp

# qhasm: 						v <<<= 13
# asm 1: rol  $13,<v=int32#7
# asm 2: rol  $13,<v=%ebp
.byte 0xf3
rol  $13,%ebp

# qhasm: 						v ^= x11
# asm 1: xorl <x11=stack32#17,<v=int32#7
# asm 2: xorl <x11=64(%esp),<v=%ebp
.byte 0xf3
xorl 64(%esp),%ebp

# qhasm: x12 = p
# asm 1: movl <p=int32#1,>x12=stack32#7
# asm 2: movl <p=%eax,>x12=24(%esp)
movl %eax,24(%esp)

# qhasm: 				x6 = t
# asm 1: movl <t=int32#4,>x6=stack32#12
# asm 2: movl <t=%ebx,>x6=44(%esp)
movl %ebx,44(%esp)

# qhasm: p += x8
# asm 1: addl <x8=stack32#8,<p=int32#1
# asm 2: addl <x8=28(%esp),<p=%eax
addl 28(%esp),%eax

# qhasm: 		x1 = r
# asm 1: movl <r=int32#6,>x1=stack32#17
# asm 2: movl <r=%edi,>x1=64(%esp)
movl %edi,64(%esp)

# qhasm: 				t += x2
# asm 1: addl <x2=stack32#13,<t=int32#4
# asm 2: addl <x2=48(%esp),<t=%ebx
addl 48(%esp),%ebx

# qhasm: 						x11 = v
# asm 1: movl <v=int32#7,>x11=stack32#18
# asm 2: movl <v=%ebp,>x11=68(%esp)
movl %ebp,68(%esp)

# qhasm: p <<<= 18
# asm 1: rol  $18,<p=int32#1
# asm 2: rol  $18,<p=%eax
.byte 0xf3
rol  $18,%eax

# qhasm: p ^= x0
# asm 1: xorl <x0=stack32#6,<p=int32#1
# asm 2: xorl <x0=20(%esp),<p=%eax
xorl 20(%esp),%eax

# qhasm: 				t <<<= 18
# asm 1: rol  $18,<t=int32#4
# asm 2: rol  $18,<t=%ebx
.byte 0xf3
rol  $18,%ebx

# qhasm: 				t ^= x10
# asm 1: xorl <x10=stack32#11,<t=int32#4
# asm 2: xorl <x10=40(%esp),<t=%ebx
xorl 40(%esp),%ebx

# qhasm: 		s += r
# asm 1: add  <r=int32#6,<s=int32#3
# asm 2: add  <r=%edi,<s=%edx
add  %edi,%edx

# qhasm: 		s <<<= 18
# asm 1: rol  $18,<s=int32#3
# asm 2: rol  $18,<s=%edx
.byte 0xf3
rol  $18,%edx

# qhasm: 		s ^= x5
# asm 1: xorl <x5=stack32#16,<s=int32#3
# asm 2: xorl <x5=60(%esp),<s=%edx
xorl 60(%esp),%edx

# qhasm: 						w += v
# asm 1: add  <v=int32#7,<w=int32#5
# asm 2: add  <v=%ebp,<w=%esi
add  %ebp,%esi

# qhasm: 						w <<<= 18
# asm 1: rol  $18,<w=int32#5
# asm 2: rol  $18,<w=%esi
.byte 0xf3
rol  $18,%esi

# qhasm: 						w ^= x15
# asm 1: xorl <x15=stack32#21,<w=int32#5
# asm 2: xorl <x15=80(%esp),<w=%esi
xorl 80(%esp),%esi

# qhasm: x0 = p
# asm 1: movl <p=int32#1,>x0=stack32#6
# asm 2: movl <p=%eax,>x0=20(%esp)
movl %eax,20(%esp)

# qhasm: 				x10 = t
# asm 1: movl <t=int32#4,>x10=stack32#11
# asm 2: movl <t=%ebx,>x10=40(%esp)
movl %ebx,40(%esp)

# qhasm: p += x3
# asm 1: addl <x3=stack32#20,<p=int32#1
# asm 2: addl <x3=76(%esp),<p=%eax
addl 76(%esp),%eax

# qhasm: p <<<= 7
# asm 1: rol  $7,<p=int32#1
# asm 2: rol  $7,<p=%eax
.byte 0xf3
rol  $7,%eax

# qhasm: 		x5 = s
# asm 1: movl <s=int32#3,>x5=stack32#16
# asm 2: movl <s=%edx,>x5=60(%esp)
movl %edx,60(%esp)

# qhasm: 				t += x9
# asm 1: addl <x9=stack32#15,<t=int32#4
# asm 2: addl <x9=56(%esp),<t=%ebx
addl 56(%esp),%ebx

# qhasm: 						x15 = w
# asm 1: movl <w=int32#5,>x15=stack32#21
# asm 2: movl <w=%esi,>x15=80(%esp)
movl %esi,80(%esp)

# qhasm: 		r = x4
# asm 1: movl <x4=stack32#9,>r=int32#6
# asm 2: movl <x4=32(%esp),>r=%edi
movl 32(%esp),%edi

# qhasm: 		r += s
# asm 1: add  <s=int32#3,<r=int32#6
# asm 2: add  <s=%edx,<r=%edi
add  %edx,%edi

# qhasm: 						v = x14
# asm 1: movl <x14=stack32#10,>v=int32#7
# asm 2: movl <x14=36(%esp),>v=%ebp
movl 36(%esp),%ebp

# qhasm: 						v += w
# asm 1: add  <w=int32#5,<v=int32#7
# asm 2: add  <w=%esi,<v=%ebp
add  %esi,%ebp

# qhasm: p ^= x1
# asm 1: xorl <x1=stack32#17,<p=int32#1
# asm 2: xorl <x1=64(%esp),<p=%eax
xorl 64(%esp),%eax

# qhasm: 				t <<<= 7
# asm 1: rol  $7,<t=int32#4
# asm 2: rol  $7,<t=%ebx
.byte 0xf3
rol  $7,%ebx

# qhasm: 				t ^= x11
# asm 1: xorl <x11=stack32#18,<t=int32#4
# asm 2: xorl <x11=68(%esp),<t=%ebx
xorl 68(%esp),%ebx

# qhasm: 		r <<<= 7
# asm 1: rol  $7,<r=int32#6
# asm 2: rol  $7,<r=%edi
.byte 0xf3
rol  $7,%edi

# qhasm: 		r ^= x6
# asm 1: xorl <x6=stack32#12,<r=int32#6
# asm 2: xorl <x6=44(%esp),<r=%edi
xorl 44(%esp),%edi

# qhasm: 						v <<<= 7
# asm 1: rol  $7,<v=int32#7
# asm 2: rol  $7,<v=%ebp
.byte 0xf3
rol  $7,%ebp

# qhasm: 						v ^= x12
# asm 1: xorl <x12=stack32#7,<v=int32#7
# asm 2: xorl <x12=24(%esp),<v=%ebp
xorl 24(%esp),%ebp

# qhasm: x1 = p
# asm 1: movl <p=int32#1,>x1=stack32#7
# asm 2: movl <p=%eax,>x1=24(%esp)
movl %eax,24(%esp)

# qhasm: 				x11 = t
# asm 1: movl <t=int32#4,>x11=stack32#12
# asm 2: movl <t=%ebx,>x11=44(%esp)
movl %ebx,44(%esp)

# qhasm: p += x0
# asm 1: addl <x0=stack32#6,<p=int32#1
# asm 2: addl <x0=20(%esp),<p=%eax
addl 20(%esp),%eax

# qhasm: 		x6 = r
# asm 1: movl <r=int32#6,>x6=stack32#17
# asm 2: movl <r=%edi,>x6=64(%esp)
movl %edi,64(%esp)

# qhasm: 				t += x10
# asm 1: addl <x10=stack32#11,<t=int32#4
# asm 2: addl <x10=40(%esp),<t=%ebx
addl 40(%esp),%ebx

# qhasm: 						x12 = v
# asm 1: movl <v=int32#7,>x12=stack32#18
# asm 2: movl <v=%ebp,>x12=68(%esp)
movl %ebp,68(%esp)

# qhasm: p <<<= 9
# asm 1: rol  $9,<p=int32#1
# asm 2: rol  $9,<p=%eax
.byte 0xf3
rol  $9,%eax

# qhasm: p ^= x2
# asm 1: xorl <x2=stack32#13,<p=int32#1
# asm 2: xorl <x2=48(%esp),<p=%eax
xorl 48(%esp),%eax

# qhasm: 				t <<<= 9
# asm 1: rol  $9,<t=int32#4
# asm 2: rol  $9,<t=%ebx
.byte 0xf3
rol  $9,%ebx

# qhasm: 				t ^= x8
# asm 1: xorl <x8=stack32#8,<t=int32#4
# asm 2: xorl <x8=28(%esp),<t=%ebx
xorl 28(%esp),%ebx

# qhasm: 		s += r
# asm 1: add  <r=int32#6,<s=int32#3
# asm 2: add  <r=%edi,<s=%edx
add  %edi,%edx

# qhasm: 		s <<<= 9
# asm 1: rol  $9,<s=int32#3
# asm 2: rol  $9,<s=%edx
.byte 0xf3
rol  $9,%edx

# qhasm: 		s ^= x7
# asm 1: xorl <x7=stack32#19,<s=int32#3
# asm 2: xorl <x7=72(%esp),<s=%edx
xorl 72(%esp),%edx

# qhasm: 						w += v
# asm 1: add  <v=int32#7,<w=int32#5
# asm 2: add  <v=%ebp,<w=%esi
add  %ebp,%esi

# qhasm: 						w <<<= 9
# asm 1: rol  $9,<w=int32#5
# asm 2: rol  $9,<w=%esi
.byte 0xf3
rol  $9,%esi

# qhasm: 						w ^= x13
# asm 1: xorl <x13=stack32#14,<w=int32#5
# asm 2: xorl <x13=52(%esp),<w=%esi
xorl 52(%esp),%esi

# qhasm: x2 = p
# asm 1: movl <p=int32#1,>x2=stack32#8
# asm 2: movl <p=%eax,>x2=28(%esp)
movl %eax,28(%esp)

# qhasm: 				x8 = t
# asm 1: movl <t=int32#4,>x8=stack32#13
# asm 2: movl <t=%ebx,>x8=48(%esp)
movl %ebx,48(%esp)

# qhasm: p += x1
# asm 1: addl <x1=stack32#7,<p=int32#1
# asm 2: addl <x1=24(%esp),<p=%eax
addl 24(%esp),%eax

# qhasm: 		x7 = s
# asm 1: movl <s=int32#3,>x7=stack32#14
# asm 2: movl <s=%edx,>x7=52(%esp)
movl %edx,52(%esp)

# qhasm: 				t += x11
# asm 1: addl <x11=stack32#12,<t=int32#4
# asm 2: addl <x11=44(%esp),<t=%ebx
addl 44(%esp),%ebx

# qhasm: 						x13 = w
# asm 1: movl <w=int32#5,>x13=stack32#19
# asm 2: movl <w=%esi,>x13=72(%esp)
movl %esi,72(%esp)

# qhasm: p <<<= 13
# asm 1: rol  $13,<p=int32#1
# asm 2: rol  $13,<p=%eax
rol  $13,%eax

# qhasm: p ^= x3
# asm 1: xorl <x3=stack32#20,<p=int32#1
# asm 2: xorl <x3=76(%esp),<p=%eax
xorl 76(%esp),%eax

# qhasm: 				t <<<= 13
# asm 1: rol  $13,<t=int32#4
# asm 2: rol  $13,<t=%ebx
rol  $13,%ebx

# qhasm: 				t ^= x9
# asm 1: xorl <x9=stack32#15,<t=int32#4
# asm 2: xorl <x9=56(%esp),<t=%ebx
xorl 56(%esp),%ebx

# qhasm: 		r += s
# asm 1: add  <s=int32#3,<r=int32#6
# asm 2: add  <s=%edx,<r=%edi
add  %edx,%edi

# qhasm: 		r <<<= 13
# asm 1: rol  $13,<r=int32#6
# asm 2: rol  $13,<r=%edi
rol  $13,%edi

# qhasm: 		r ^= x4
# asm 1: xorl <x4=stack32#9,<r=int32#6
# asm 2: xorl <x4=32(%esp),<r=%edi
xorl 32(%esp),%edi

# qhasm: 						v += w
# asm 1: add  <w=int32#5,<v=int32#7
# asm 2: add  <w=%esi,<v=%ebp
add  %esi,%ebp

# qhasm: 						v <<<= 13
# asm 1: rol  $13,<v=int32#7
# asm 2: rol  $13,<v=%ebp
rol  $13,%ebp

# qhasm: 						v ^= x14
# asm 1: xorl <x14=stack32#10,<v=int32#7
# asm 2: xorl <x14=36(%esp),<v=%ebp
xorl 36(%esp),%ebp

# qhasm: x3 = p
# asm 1: movl <p=int32#1,>x3=stack32#9
# asm 2: movl <p=%eax,>x3=32(%esp)
movl %eax,32(%esp)

# qhasm: 				x9 = t
# asm 1: movl <t=int32#4,>x9=stack32#10
# asm 2: movl <t=%ebx,>x9=36(%esp)
movl %ebx,36(%esp)

# qhasm: p += x2
# asm 1: addl <x2=stack32#8,<p=int32#1
# asm 2: addl <x2=28(%esp),<p=%eax
addl 28(%esp),%eax

# qhasm: 		x4 = r
# asm 1: movl <r=int32#6,>x4=stack32#15
# asm 2: movl <r=%edi,>x4=56(%esp)
movl %edi,56(%esp)

# qhasm: 				t += x8
# asm 1: addl <x8=stack32#13,<t=int32#4
# asm 2: addl <x8=48(%esp),<t=%ebx
addl 48(%esp),%ebx

# qhasm: 						x14 = v
# asm 1: movl <v=int32#7,>x14=stack32#20
# asm 2: movl <v=%ebp,>x14=76(%esp)
movl %ebp,76(%esp)

# qhasm: p <<<= 18
# asm 1: rol  $18,<p=int32#1
# asm 2: rol  $18,<p=%eax
.byte 0xf3
rol  $18,%eax

# qhasm: p ^= x0
# asm 1: xorl <x0=stack32#6,<p=int32#1
# asm 2: xorl <x0=20(%esp),<p=%eax
xorl 20(%esp),%eax

# qhasm: 				t <<<= 18
# asm 1: rol  $18,<t=int32#4
# asm 2: rol  $18,<t=%ebx
rol  $18,%ebx

# qhasm: 				t ^= x10
# asm 1: xorl <x10=stack32#11,<t=int32#4
# asm 2: xorl <x10=40(%esp),<t=%ebx
xorl 40(%esp),%ebx

# qhasm: 		s += r
# asm 1: add  <r=int32#6,<s=int32#3
# asm 2: add  <r=%edi,<s=%edx
add  %edi,%edx

# qhasm: 		s <<<= 18
# asm 1: rol  $18,<s=int32#3
# asm 2: rol  $18,<s=%edx
rol  $18,%edx

# qhasm: 		s ^= x5
# asm 1: xorl <x5=stack32#16,<s=int32#3
# asm 2: xorl <x5=60(%esp),<s=%edx
xorl 60(%esp),%edx

# qhasm: 						w += v
# asm 1: add  <v=int32#7,<w=int32#5
# asm 2: add  <v=%ebp,<w=%esi
.byte 0xf3
add  %ebp,%esi

# qhasm: 						w <<<= 18
# asm 1: rol  $18,<w=int32#5
# asm 2: rol  $18,<w=%esi
.byte 0xf3
rol  $18,%esi

# qhasm: 						w ^= x15
# asm 1: xorl <x15=stack32#21,<w=int32#5
# asm 2: xorl <x15=80(%esp),<w=%esi
.byte 0xf3
xorl 80(%esp),%esi

# qhasm: x0 = p
# asm 1: movl <p=int32#1,>x0=stack32#6
# asm 2: movl <p=%eax,>x0=20(%esp)
movl %eax,20(%esp)

# qhasm: 				x10 = t
# asm 1: movl <t=int32#4,>x10=stack32#11
# asm 2: movl <t=%ebx,>x10=40(%esp)
movl %ebx,40(%esp)

# qhasm: p += x12
# asm 1: addl <x12=stack32#18,<p=int32#1
# asm 2: addl <x12=68(%esp),<p=%eax
addl 68(%esp),%eax

# qhasm: 		x5 = s
# asm 1: movl <s=int32#3,>x5=stack32#16
# asm 2: movl <s=%edx,>x5=60(%esp)
movl %edx,60(%esp)

# qhasm: 				t += x6
# asm 1: addl <x6=stack32#17,<t=int32#4
# asm 2: addl <x6=64(%esp),<t=%ebx
addl 64(%esp),%ebx

# qhasm: 						x15 = w
# asm 1: movl <w=int32#5,>x15=stack32#21
# asm 2: movl <w=%esi,>x15=80(%esp)
movl %esi,80(%esp)

# qhasm: 		r = x1
# asm 1: movl <x1=stack32#7,>r=int32#6
# asm 2: movl <x1=24(%esp),>r=%edi
movl 24(%esp),%edi

# qhasm: 		r += s
# asm 1: add  <s=int32#3,<r=int32#6
# asm 2: add  <s=%edx,<r=%edi
add  %edx,%edi

# qhasm: 						v = x11
# asm 1: movl <x11=stack32#12,>v=int32#7
# asm 2: movl <x11=44(%esp),>v=%ebp
movl 44(%esp),%ebp

# qhasm: 						v += w
# asm 1: add  <w=int32#5,<v=int32#7
# asm 2: add  <w=%esi,<v=%ebp
add  %esi,%ebp

# qhasm: p <<<= 7
# asm 1: rol  $7,<p=int32#1
# asm 2: rol  $7,<p=%eax
.byte 0xf3
rol  $7,%eax

# qhasm: p ^= x4
# asm 1: xorl <x4=stack32#15,<p=int32#1
# asm 2: xorl <x4=56(%esp),<p=%eax
xorl 56(%esp),%eax

# qhasm: 				t <<<= 7
# asm 1: rol  $7,<t=int32#4
# asm 2: rol  $7,<t=%ebx
.byte 0xf3
rol  $7,%ebx

# qhasm: 				t ^= x14
# asm 1: xorl <x14=stack32#20,<t=int32#4
# asm 2: xorl <x14=76(%esp),<t=%ebx
xorl 76(%esp),%ebx

# qhasm: 		r <<<= 7
# asm 1: rol  $7,<r=int32#6
# asm 2: rol  $7,<r=%edi
.byte 0xf3
rol  $7,%edi

# qhasm: 		r ^= x9
# asm 1: xorl <x9=stack32#10,<r=int32#6
# asm 2: xorl <x9=36(%esp),<r=%edi
xorl 36(%esp),%edi

# qhasm: 						v <<<= 7
# asm 1: rol  $7,<v=int32#7
# asm 2: rol  $7,<v=%ebp
.byte 0xf3
rol  $7,%ebp

# qhasm: 						v ^= x3
# asm 1: xorl <x3=stack32#9,<v=int32#7
# asm 2: xorl <x3=32(%esp),<v=%ebp
xorl 32(%esp),%ebp

# qhasm: x4 = p
# asm 1: movl <p=int32#1,>x4=stack32#9
# asm 2: movl <p=%eax,>x4=32(%esp)
movl %eax,32(%esp)

# qhasm: 				x14 = t
# asm 1: movl <t=int32#4,>x14=stack32#10
# asm 2: movl <t=%ebx,>x14=36(%esp)
movl %ebx,36(%esp)

# qhasm: p += x0
# asm 1: addl <x0=stack32#6,<p=int32#1
# asm 2: addl <x0=20(%esp),<p=%eax
addl 20(%esp),%eax

# qhasm: 		x9 = r
# asm 1: movl <r=int32#6,>x9=stack32#15
# asm 2: movl <r=%edi,>x9=56(%esp)
movl %edi,56(%esp)

# qhasm: 				t += x10
# asm 1: addl <x10=stack32#11,<t=int32#4
# asm 2: addl <x10=40(%esp),<t=%ebx
addl 40(%esp),%ebx

# qhasm: 						x3 = v
# asm 1: movl <v=int32#7,>x3=stack32#20
# asm 2: movl <v=%ebp,>x3=76(%esp)
movl %ebp,76(%esp)

# qhasm: p <<<= 9
# asm 1: rol  $9,<p=int32#1
# asm 2: rol  $9,<p=%eax
.byte 0xf3
rol  $9,%eax

# qhasm: p ^= x8
# asm 1: xorl <x8=stack32#13,<p=int32#1
# asm 2: xorl <x8=48(%esp),<p=%eax
xorl 48(%esp),%eax

# qhasm: 				t <<<= 9
# asm 1: rol  $9,<t=int32#4
# asm 2: rol  $9,<t=%ebx
rol  $9,%ebx

# qhasm: 				t ^= x2
# asm 1: xorl <x2=stack32#8,<t=int32#4
# asm 2: xorl <x2=28(%esp),<t=%ebx
xorl 28(%esp),%ebx

# qhasm: 		s += r
# asm 1: add  <r=int32#6,<s=int32#3
# asm 2: add  <r=%edi,<s=%edx
add  %edi,%edx

# qhasm: 		s <<<= 9
# asm 1: rol  $9,<s=int32#3
# asm 2: rol  $9,<s=%edx
rol  $9,%edx

# qhasm: 		s ^= x13
# asm 1: xorl <x13=stack32#19,<s=int32#3
# asm 2: xorl <x13=72(%esp),<s=%edx
xorl 72(%esp),%edx

# qhasm: 						w += v
# asm 1: add  <v=int32#7,<w=int32#5
# asm 2: add  <v=%ebp,<w=%esi
.byte 0xf3
add  %ebp,%esi

# qhasm: 						w <<<= 9
# asm 1: rol  $9,<w=int32#5
# asm 2: rol  $9,<w=%esi
.byte 0xf3
rol  $9,%esi

# qhasm: 						w ^= x7
# asm 1: xorl <x7=stack32#14,<w=int32#5
# asm 2: xorl <x7=52(%esp),<w=%esi
.byte 0xf3
xorl 52(%esp),%esi

# qhasm: x8 = p
# asm 1: movl <p=int32#1,>x8=stack32#8
# asm 2: movl <p=%eax,>x8=28(%esp)
movl %eax,28(%esp)

# qhasm: 				x2 = t
# asm 1: movl <t=int32#4,>x2=stack32#13
# asm 2: movl <t=%ebx,>x2=48(%esp)
movl %ebx,48(%esp)

# qhasm: p += x4
# asm 1: addl <x4=stack32#9,<p=int32#1
# asm 2: addl <x4=32(%esp),<p=%eax
addl 32(%esp),%eax

# qhasm: 		x13 = s
# asm 1: movl <s=int32#3,>x13=stack32#14
# asm 2: movl <s=%edx,>x13=52(%esp)
movl %edx,52(%esp)

# qhasm: 				t += x14
# asm 1: addl <x14=stack32#10,<t=int32#4
# asm 2: addl <x14=36(%esp),<t=%ebx
addl 36(%esp),%ebx

# qhasm: 						x7 = w
# asm 1: movl <w=int32#5,>x7=stack32#19
# asm 2: movl <w=%esi,>x7=72(%esp)
movl %esi,72(%esp)

# qhasm: p <<<= 13
# asm 1: rol  $13,<p=int32#1
# asm 2: rol  $13,<p=%eax
.byte 0xf3
rol  $13,%eax

# qhasm: p ^= x12
# asm 1: xorl <x12=stack32#18,<p=int32#1
# asm 2: xorl <x12=68(%esp),<p=%eax
xorl 68(%esp),%eax

# qhasm: 				t <<<= 13
# asm 1: rol  $13,<t=int32#4
# asm 2: rol  $13,<t=%ebx
.byte 0xf3
rol  $13,%ebx

# qhasm: 				t ^= x6
# asm 1: xorl <x6=stack32#17,<t=int32#4
# asm 2: xorl <x6=64(%esp),<t=%ebx
xorl 64(%esp),%ebx

# qhasm: 		r += s
# asm 1: add  <s=int32#3,<r=int32#6
# asm 2: add  <s=%edx,<r=%edi
add  %edx,%edi

# qhasm: 		r <<<= 13
# asm 1: rol  $13,<r=int32#6
# asm 2: rol  $13,<r=%edi
.byte 0xf3
rol  $13,%edi

# qhasm: 		r ^= x1
# asm 1: xorl <x1=stack32#7,<r=int32#6
# asm 2: xorl <x1=24(%esp),<r=%edi
xorl 24(%esp),%edi

# qhasm: 						v += w
# asm 1: add  <w=int32#5,<v=int32#7
# asm 2: add  <w=%esi,<v=%ebp
add  %esi,%ebp

# qhasm: 						v <<<= 13
# asm 1: rol  $13,<v=int32#7
# asm 2: rol  $13,<v=%ebp
.byte 0xf3
rol  $13,%ebp

# qhasm: 						v ^= x11
# asm 1: xorl <x11=stack32#12,<v=int32#7
# asm 2: xorl <x11=44(%esp),<v=%ebp
xorl 44(%esp),%ebp

# qhasm: x12 = p
# asm 1: movl <p=int32#1,>x12=stack32#7
# asm 2: movl <p=%eax,>x12=24(%esp)
movl %eax,24(%esp)

# qhasm: 				x6 = t
# asm 1: movl <t=int32#4,>x6=stack32#12
# asm 2: movl <t=%ebx,>x6=44(%esp)
movl %ebx,44(%esp)

# qhasm: p += x8
# asm 1: addl <x8=stack32#8,<p=int32#1
# asm 2: addl <x8=28(%esp),<p=%eax
addl 28(%esp),%eax

# qhasm: 		x1 = r
# asm 1: movl <r=int32#6,>x1=stack32#17
# asm 2: movl <r=%edi,>x1=64(%esp)
movl %edi,64(%esp)

# qhasm: 				t += x2
# asm 1: addl <x2=stack32#13,<t=int32#4
# asm 2: addl <x2=48(%esp),<t=%ebx
addl 48(%esp),%ebx

# qhasm: 						x11 = v
# asm 1: movl <v=int32#7,>x11=stack32#18
# asm 2: movl <v=%ebp,>x11=68(%esp)
movl %ebp,68(%esp)

# qhasm: p <<<= 18
# asm 1: rol  $18,<p=int32#1
# asm 2: rol  $18,<p=%eax
rol  $18,%eax

# qhasm: p ^= x0
# asm 1: xorl <x0=stack32#6,<p=int32#1
# asm 2: xorl <x0=20(%esp),<p=%eax
xorl 20(%esp),%eax

# qhasm: 				t <<<= 18
# asm 1: rol  $18,<t=int32#4
# asm 2: rol  $18,<t=%ebx
rol  $18,%ebx

# qhasm: 				t ^= x10
# asm 1: xorl <x10=stack32#11,<t=int32#4
# asm 2: xorl <x10=40(%esp),<t=%ebx
xorl 40(%esp),%ebx

# qhasm: 		s += r
# asm 1: add  <r=int32#6,<s=int32#3
# asm 2: add  <r=%edi,<s=%edx
add  %edi,%edx

# qhasm: 		s <<<= 18
# asm 1: rol  $18,<s=int32#3
# asm 2: rol  $18,<s=%edx
rol  $18,%edx

# qhasm: 		s ^= x5
# asm 1: xorl <x5=stack32#16,<s=int32#3
# asm 2: xorl <x5=60(%esp),<s=%edx
xorl 60(%esp),%edx

# qhasm: 						w += v
# asm 1: add  <v=int32#7,<w=int32#5
# asm 2: add  <v=%ebp,<w=%esi
add  %ebp,%esi

# qhasm: 						w <<<= 18
# asm 1: rol  $18,<w=int32#5
# asm 2: rol  $18,<w=%esi
rol  $18,%esi

# qhasm: 						w ^= x15
# asm 1: xorl <x15=stack32#21,<w=int32#5
# asm 2: xorl <x15=80(%esp),<w=%esi
xorl 80(%esp),%esi

# qhasm: x0 = p
# asm 1: movl <p=int32#1,>x0=stack32#6
# asm 2: movl <p=%eax,>x0=20(%esp)
movl %eax,20(%esp)

# qhasm: 				x10 = t
# asm 1: movl <t=int32#4,>x10=stack32#16
# asm 2: movl <t=%ebx,>x10=60(%esp)
movl %ebx,60(%esp)

# qhasm: p += x3
# asm 1: addl <x3=stack32#20,<p=int32#1
# asm 2: addl <x3=76(%esp),<p=%eax
addl 76(%esp),%eax

# qhasm: p <<<= 7
# asm 1: rol  $7,<p=int32#1
# asm 2: rol  $7,<p=%eax
.byte 0xf3
rol  $7,%eax

# qhasm: 		x5 = s
# asm 1: movl <s=int32#3,>x5=stack32#11
# asm 2: movl <s=%edx,>x5=40(%esp)
movl %edx,40(%esp)

# qhasm: 				t += x9
# asm 1: addl <x9=stack32#15,<t=int32#4
# asm 2: addl <x9=56(%esp),<t=%ebx
addl 56(%esp),%ebx

# qhasm: 						x15 = w
# asm 1: movl <w=int32#5,>x15=stack32#21
# asm 2: movl <w=%esi,>x15=80(%esp)
movl %esi,80(%esp)

# qhasm: 		r = x4
# asm 1: movl <x4=stack32#9,>r=int32#6
# asm 2: movl <x4=32(%esp),>r=%edi
movl 32(%esp),%edi

# qhasm: 		r += s
# asm 1: add  <s=int32#3,<r=int32#6
# asm 2: add  <s=%edx,<r=%edi
add  %edx,%edi

# qhasm: 						v = x14
# asm 1: movl <x14=stack32#10,>v=int32#7
# asm 2: movl <x14=36(%esp),>v=%ebp
movl 36(%esp),%ebp

# qhasm: 						v += w
# asm 1: add  <w=int32#5,<v=int32#7
# asm 2: add  <w=%esi,<v=%ebp
add  %esi,%ebp

# qhasm: p ^= x1
# asm 1: xorl <x1=stack32#17,<p=int32#1
# asm 2: xorl <x1=64(%esp),<p=%eax
xorl 64(%esp),%eax

# qhasm: 				t <<<= 7
# asm 1: rol  $7,<t=int32#4
# asm 2: rol  $7,<t=%ebx
.byte 0xf3
rol  $7,%ebx

# qhasm: 				t ^= x11
# asm 1: xorl <x11=stack32#18,<t=int32#4
# asm 2: xorl <x11=68(%esp),<t=%ebx
xorl 68(%esp),%ebx

# qhasm: 		r <<<= 7
# asm 1: rol  $7,<r=int32#6
# asm 2: rol  $7,<r=%edi
.byte 0xf3
rol  $7,%edi

# qhasm: 		r ^= x6
# asm 1: xorl <x6=stack32#12,<r=int32#6
# asm 2: xorl <x6=44(%esp),<r=%edi
xorl 44(%esp),%edi

# qhasm: 						v <<<= 7
# asm 1: rol  $7,<v=int32#7
# asm 2: rol  $7,<v=%ebp
.byte 0xf3
rol  $7,%ebp

# qhasm: 						v ^= x12
# asm 1: xorl <x12=stack32#7,<v=int32#7
# asm 2: xorl <x12=24(%esp),<v=%ebp
xorl 24(%esp),%ebp

# qhasm: x1 = p
# asm 1: movl <p=int32#1,>x1=stack32#7
# asm 2: movl <p=%eax,>x1=24(%esp)
movl %eax,24(%esp)

# qhasm: 				x11 = t
# asm 1: movl <t=int32#4,>x11=stack32#17
# asm 2: movl <t=%ebx,>x11=64(%esp)
movl %ebx,64(%esp)

# qhasm: p += x0
# asm 1: addl <x0=stack32#6,<p=int32#1
# asm 2: addl <x0=20(%esp),<p=%eax
addl 20(%esp),%eax

# qhasm: 		x6 = r
# asm 1: movl <r=int32#6,>x6=stack32#12
# asm 2: movl <r=%edi,>x6=44(%esp)
movl %edi,44(%esp)

# qhasm: 				t += x10
# asm 1: addl <x10=stack32#16,<t=int32#4
# asm 2: addl <x10=60(%esp),<t=%ebx
addl 60(%esp),%ebx

# qhasm: 						x12 = v
# asm 1: movl <v=int32#7,>x12=stack32#18
# asm 2: movl <v=%ebp,>x12=68(%esp)
movl %ebp,68(%esp)

# qhasm: p <<<= 9
# asm 1: rol  $9,<p=int32#1
# asm 2: rol  $9,<p=%eax
.byte 0xf3
rol  $9,%eax

# qhasm: p ^= x2
# asm 1: xorl <x2=stack32#13,<p=int32#1
# asm 2: xorl <x2=48(%esp),<p=%eax
xorl 48(%esp),%eax

# qhasm: 				t <<<= 9
# asm 1: rol  $9,<t=int32#4
# asm 2: rol  $9,<t=%ebx
rol  $9,%ebx

# qhasm: 				t ^= x8
# asm 1: xorl <x8=stack32#8,<t=int32#4
# asm 2: xorl <x8=28(%esp),<t=%ebx
xorl 28(%esp),%ebx

# qhasm: 		s += r
# asm 1: add  <r=int32#6,<s=int32#3
# asm 2: add  <r=%edi,<s=%edx
add  %edi,%edx

# qhasm: 		s <<<= 9
# asm 1: rol  $9,<s=int32#3
# asm 2: rol  $9,<s=%edx
rol  $9,%edx

# qhasm: 		s ^= x7
# asm 1: xorl <x7=stack32#19,<s=int32#3
# asm 2: xorl <x7=72(%esp),<s=%edx
.byte 0xf3
xorl 72(%esp),%edx

# qhasm: 						w += v
# asm 1: add  <v=int32#7,<w=int32#5
# asm 2: add  <v=%ebp,<w=%esi
.byte 0xf3
add  %ebp,%esi

# qhasm: 						w <<<= 9
# asm 1: rol  $9,<w=int32#5
# asm 2: rol  $9,<w=%esi
.byte 0xf3
rol  $9,%esi

# qhasm: 						w ^= x13
# asm 1: xorl <x13=stack32#14,<w=int32#5
# asm 2: xorl <x13=52(%esp),<w=%esi
xorl 52(%esp),%esi

# qhasm: x2 = p
# asm 1: movl <p=int32#1,>x2=stack32#8
# asm 2: movl <p=%eax,>x2=28(%esp)
movl %eax,28(%esp)

# qhasm: 				x8 = t
# asm 1: movl <t=int32#4,>x8=stack32#14
# asm 2: movl <t=%ebx,>x8=52(%esp)
movl %ebx,52(%esp)

# qhasm: p += x1
# asm 1: addl <x1=stack32#7,<p=int32#1
# asm 2: addl <x1=24(%esp),<p=%eax
addl 24(%esp),%eax

# qhasm: 		x7 = s
# asm 1: movl <s=int32#3,>x7=stack32#13
# asm 2: movl <s=%edx,>x7=48(%esp)
movl %edx,48(%esp)

# qhasm: 				t += x11
# asm 1: addl <x11=stack32#17,<t=int32#4
# asm 2: addl <x11=64(%esp),<t=%ebx
addl 64(%esp),%ebx

# qhasm: 						x13 = w
# asm 1: movl <w=int32#5,>x13=stack32#19
# asm 2: movl <w=%esi,>x13=72(%esp)
movl %esi,72(%esp)

# qhasm: p <<<= 13
# asm 1: rol  $13,<p=int32#1
# asm 2: rol  $13,<p=%eax
.byte 0xf3
rol  $13,%eax

# qhasm: p ^= x3
# asm 1: xorl <x3=stack32#20,<p=int32#1
# asm 2: xorl <x3=76(%esp),<p=%eax
xorl 76(%esp),%eax

# qhasm: 				t <<<= 13
# asm 1: rol  $13,<t=int32#4
# asm 2: rol  $13,<t=%ebx
rol  $13,%ebx

# qhasm: 				t ^= x9
# asm 1: xorl <x9=stack32#15,<t=int32#4
# asm 2: xorl <x9=56(%esp),<t=%ebx
xorl 56(%esp),%ebx

# qhasm: 		r += s
# asm 1: add  <s=int32#3,<r=int32#6
# asm 2: add  <s=%edx,<r=%edi
add  %edx,%edi

# qhasm: 		r <<<= 13
# asm 1: rol  $13,<r=int32#6
# asm 2: rol  $13,<r=%edi
rol  $13,%edi

# qhasm: 		r ^= x4
# asm 1: xorl <x4=stack32#9,<r=int32#6
# asm 2: xorl <x4=32(%esp),<r=%edi
xorl 32(%esp),%edi

# qhasm: 						v += w
# asm 1: add  <w=int32#5,<v=int32#7
# asm 2: add  <w=%esi,<v=%ebp
.byte 0xf3
add  %esi,%ebp

# qhasm: 						v <<<= 13
# asm 1: rol  $13,<v=int32#7
# asm 2: rol  $13,<v=%ebp
.byte 0xf3
rol  $13,%ebp

# qhasm: 						v ^= x14
# asm 1: xorl <x14=stack32#10,<v=int32#7
# asm 2: xorl <x14=36(%esp),<v=%ebp
.byte 0xf3
xorl 36(%esp),%ebp

# qhasm: x3 = p
# asm 1: movl <p=int32#1,>x3=stack32#9
# asm 2: movl <p=%eax,>x3=32(%esp)
movl %eax,32(%esp)

# qhasm: 				x9 = t
# asm 1: movl <t=int32#4,>x9=stack32#15
# asm 2: movl <t=%ebx,>x9=56(%esp)
movl %ebx,56(%esp)

# qhasm: p += x2
# asm 1: addl <x2=stack32#8,<p=int32#1
# asm 2: addl <x2=28(%esp),<p=%eax
addl 28(%esp),%eax

# qhasm: 		x4 = r
# asm 1: movl <r=int32#6,>x4=stack32#10
# asm 2: movl <r=%edi,>x4=36(%esp)
movl %edi,36(%esp)

# qhasm: 				t += x8
# asm 1: addl <x8=stack32#14,<t=int32#4
# asm 2: addl <x8=52(%esp),<t=%ebx
addl 52(%esp),%ebx

# qhasm: 						x14 = v
# asm 1: movl <v=int32#7,>x14=stack32#20
# asm 2: movl <v=%ebp,>x14=76(%esp)
movl %ebp,76(%esp)

# qhasm: p <<<= 18
# asm 1: rol  $18,<p=int32#1
# asm 2: rol  $18,<p=%eax
.byte 0xf3
rol  $18,%eax

# qhasm: p ^= x0
# asm 1: xorl <x0=stack32#6,<p=int32#1
# asm 2: xorl <x0=20(%esp),<p=%eax
xorl 20(%esp),%eax

# qhasm: 				t <<<= 18
# asm 1: rol  $18,<t=int32#4
# asm 2: rol  $18,<t=%ebx
.byte 0xf3
rol  $18,%ebx

# qhasm: 				t ^= x10
# asm 1: xorl <x10=stack32#16,<t=int32#4
# asm 2: xorl <x10=60(%esp),<t=%ebx
xorl 60(%esp),%ebx

# qhasm: 		s += r
# asm 1: add  <r=int32#6,<s=int32#3
# asm 2: add  <r=%edi,<s=%edx
add  %edi,%edx

# qhasm: 		s <<<= 18
# asm 1: rol  $18,<s=int32#3
# asm 2: rol  $18,<s=%edx
.byte 0xf3
rol  $18,%edx

# qhasm: 		s ^= x5
# asm 1: xorl <x5=stack32#11,<s=int32#3
# asm 2: xorl <x5=40(%esp),<s=%edx
xorl 40(%esp),%edx

# qhasm: 						w += v
# asm 1: add  <v=int32#7,<w=int32#5
# asm 2: add  <v=%ebp,<w=%esi
add  %ebp,%esi

# qhasm: 						w <<<= 18
# asm 1: rol  $18,<w=int32#5
# asm 2: rol  $18,<w=%esi
rol  $18,%esi

# qhasm: 						w ^= x15
# asm 1: xorl <x15=stack32#21,<w=int32#5
# asm 2: xorl <x15=80(%esp),<w=%esi
xorl 80(%esp),%esi

# qhasm:                  unsigned>? i -= 4
# asm 1: sub  $4,<i=int32#2
# asm 2: sub  $4,<i=%ecx
sub  $4,%ecx
# comment:fp stack unchanged by jump

# qhasm: goto mainloop if unsigned>
ja ._mainloop

# qhasm: x0 = p
# asm 1: movl <p=int32#1,>x0=stack32#6
# asm 2: movl <p=%eax,>x0=20(%esp)
movl %eax,20(%esp)

# qhasm: x5 = s
# asm 1: movl <s=int32#3,>x5=stack32#11
# asm 2: movl <s=%edx,>x5=40(%esp)
movl %edx,40(%esp)

# qhasm: x10 = t
# asm 1: movl <t=int32#4,>x10=stack32#16
# asm 2: movl <t=%ebx,>x10=60(%esp)
movl %ebx,60(%esp)

# qhasm: x15 = w
# asm 1: movl <w=int32#5,>x15=stack32#21
# asm 2: movl <w=%esi,>x15=80(%esp)
movl %esi,80(%esp)

# qhasm:   out = out_backup
# asm 1: movl <out_backup=stack32#40,>out=int32#6
# asm 2: movl <out_backup=156(%esp),>out=%edi
movl 156(%esp),%edi

# qhasm:   m = m_backup
# asm 1: movl <m_backup=stack32#41,>m=int32#5
# asm 2: movl <m_backup=160(%esp),>m=%esi
movl 160(%esp),%esi

# qhasm:   in0 = x0
# asm 1: movl <x0=stack32#6,>in0=int32#1
# asm 2: movl <x0=20(%esp),>in0=%eax
movl 20(%esp),%eax

# qhasm:   in1 = x1
# asm 1: movl <x1=stack32#7,>in1=int32#2
# asm 2: movl <x1=24(%esp),>in1=%ecx
movl 24(%esp),%ecx

# qhasm:   in0 += j0
# asm 1: addl <j0=stack32#22,<in0=int32#1
# asm 2: addl <j0=84(%esp),<in0=%eax
addl 84(%esp),%eax

# qhasm:   in1 += j1
# asm 1: addl <j1=stack32#23,<in1=int32#2
# asm 2: addl <j1=88(%esp),<in1=%ecx
addl 88(%esp),%ecx

# qhasm:   in0 ^= *(uint32 *) (m + 0)
# asm 1: xorl 0(<m=int32#5),<in0=int32#1
# asm 2: xorl 0(<m=%esi),<in0=%eax
xorl 0(%esi),%eax

# qhasm:   in1 ^= *(uint32 *) (m + 4)
# asm 1: xorl 4(<m=int32#5),<in1=int32#2
# asm 2: xorl 4(<m=%esi),<in1=%ecx
xorl 4(%esi),%ecx

# qhasm:   *(uint32 *) (out + 0) = in0
# asm 1: movl <in0=int32#1,0(<out=int32#6)
# asm 2: movl <in0=%eax,0(<out=%edi)
movl %eax,0(%edi)

# qhasm:   *(uint32 *) (out + 4) = in1
# asm 1: movl <in1=int32#2,4(<out=int32#6)
# asm 2: movl <in1=%ecx,4(<out=%edi)
movl %ecx,4(%edi)

# qhasm:   in2 = x2
# asm 1: movl <x2=stack32#8,>in2=int32#7
# asm 2: movl <x2=28(%esp),>in2=%ebp
movl 28(%esp),%ebp

# qhasm:   in3 = x3
# asm 1: movl <x3=stack32#9,>in3=int32#1
# asm 2: movl <x3=32(%esp),>in3=%eax
movl 32(%esp),%eax

# qhasm:   in2 += j2
# asm 1: addl <j2=stack32#24,<in2=int32#7
# asm 2: addl <j2=92(%esp),<in2=%ebp
addl 92(%esp),%ebp

# qhasm:   in3 += j3
# asm 1: addl <j3=stack32#25,<in3=int32#1
# asm 2: addl <j3=96(%esp),<in3=%eax
addl 96(%esp),%eax

# qhasm:   in2 ^= *(uint32 *) (m + 8)
# asm 1: xorl 8(<m=int32#5),<in2=int32#7
# asm 2: xorl 8(<m=%esi),<in2=%ebp
xorl 8(%esi),%ebp

# qhasm:   in3 ^= *(uint32 *) (m + 12)
# asm 1: xorl 12(<m=int32#5),<in3=int32#1
# asm 2: xorl 12(<m=%esi),<in3=%eax
xorl 12(%esi),%eax

# qhasm:   *(uint32 *) (out + 8) = in2
# asm 1: movl <in2=int32#7,8(<out=int32#6)
# asm 2: movl <in2=%ebp,8(<out=%edi)
movl %ebp,8(%edi)

# qhasm:   *(uint32 *) (out + 12) = in3
# asm 1: movl <in3=int32#1,12(<out=int32#6)
# asm 2: movl <in3=%eax,12(<out=%edi)
movl %eax,12(%edi)

# qhasm:   in4 = x4
# asm 1: movl <x4=stack32#10,>in4=int32#2
# asm 2: movl <x4=36(%esp),>in4=%ecx
movl 36(%esp),%ecx

# qhasm:   in5 = x5
# asm 1: movl <x5=stack32#11,>in5=int32#7
# asm 2: movl <x5=40(%esp),>in5=%ebp
movl 40(%esp),%ebp

# qhasm:   in4 += j4
# asm 1: addl <j4=stack32#26,<in4=int32#2
# asm 2: addl <j4=100(%esp),<in4=%ecx
addl 100(%esp),%ecx

# qhasm:   in5 += j5
# asm 1: addl <j5=stack32#27,<in5=int32#7
# asm 2: addl <j5=104(%esp),<in5=%ebp
addl 104(%esp),%ebp

# qhasm:   in4 ^= *(uint32 *) (m + 16)
# asm 1: xorl 16(<m=int32#5),<in4=int32#2
# asm 2: xorl 16(<m=%esi),<in4=%ecx
xorl 16(%esi),%ecx

# qhasm:   in5 ^= *(uint32 *) (m + 20)
# asm 1: xorl 20(<m=int32#5),<in5=int32#7
# asm 2: xorl 20(<m=%esi),<in5=%ebp
xorl 20(%esi),%ebp

# qhasm:   *(uint32 *) (out + 16) = in4
# asm 1: movl <in4=int32#2,16(<out=int32#6)
# asm 2: movl <in4=%ecx,16(<out=%edi)
movl %ecx,16(%edi)

# qhasm:   *(uint32 *) (out + 20) = in5
# asm 1: movl <in5=int32#7,20(<out=int32#6)
# asm 2: movl <in5=%ebp,20(<out=%edi)
movl %ebp,20(%edi)

# qhasm:   in6 = x6
# asm 1: movl <x6=stack32#12,>in6=int32#1
# asm 2: movl <x6=44(%esp),>in6=%eax
movl 44(%esp),%eax

# qhasm:   in7 = x7
# asm 1: movl <x7=stack32#13,>in7=int32#2
# asm 2: movl <x7=48(%esp),>in7=%ecx
movl 48(%esp),%ecx

# qhasm:   in6 += j6
# asm 1: addl <j6=stack32#28,<in6=int32#1
# asm 2: addl <j6=108(%esp),<in6=%eax
addl 108(%esp),%eax

# qhasm:   in7 += j7
# asm 1: addl <j7=stack32#29,<in7=int32#2
# asm 2: addl <j7=112(%esp),<in7=%ecx
addl 112(%esp),%ecx

# qhasm:   in6 ^= *(uint32 *) (m + 24)
# asm 1: xorl 24(<m=int32#5),<in6=int32#1
# asm 2: xorl 24(<m=%esi),<in6=%eax
xorl 24(%esi),%eax

# qhasm:   in7 ^= *(uint32 *) (m + 28)
# asm 1: xorl 28(<m=int32#5),<in7=int32#2
# asm 2: xorl 28(<m=%esi),<in7=%ecx
xorl 28(%esi),%ecx

# qhasm:   *(uint32 *) (out + 24) = in6
# asm 1: movl <in6=int32#1,24(<out=int32#6)
# asm 2: movl <in6=%eax,24(<out=%edi)
movl %eax,24(%edi)

# qhasm:   *(uint32 *) (out + 28) = in7
# asm 1: movl <in7=int32#2,28(<out=int32#6)
# asm 2: movl <in7=%ecx,28(<out=%edi)
movl %ecx,28(%edi)

# qhasm:   in8 = x8
# asm 1: movl <x8=stack32#14,>in8=int32#1
# asm 2: movl <x8=52(%esp),>in8=%eax
movl 52(%esp),%eax

# qhasm:   in9 = x9
# asm 1: movl <x9=stack32#15,>in9=int32#2
# asm 2: movl <x9=56(%esp),>in9=%ecx
movl 56(%esp),%ecx

# qhasm:   in8 += j8
# asm 1: addl <j8=stack32#30,<in8=int32#1
# asm 2: addl <j8=116(%esp),<in8=%eax
addl 116(%esp),%eax

# qhasm:   in9 += j9
# asm 1: addl <j9=stack32#31,<in9=int32#2
# asm 2: addl <j9=120(%esp),<in9=%ecx
addl 120(%esp),%ecx

# qhasm:   in8 ^= *(uint32 *) (m + 32)
# asm 1: xorl 32(<m=int32#5),<in8=int32#1
# asm 2: xorl 32(<m=%esi),<in8=%eax
xorl 32(%esi),%eax

# qhasm:   in9 ^= *(uint32 *) (m + 36)
# asm 1: xorl 36(<m=int32#5),<in9=int32#2
# asm 2: xorl 36(<m=%esi),<in9=%ecx
xorl 36(%esi),%ecx

# qhasm:   *(uint32 *) (out + 32) = in8
# asm 1: movl <in8=int32#1,32(<out=int32#6)
# asm 2: movl <in8=%eax,32(<out=%edi)
movl %eax,32(%edi)

# qhasm:   *(uint32 *) (out + 36) = in9
# asm 1: movl <in9=int32#2,36(<out=int32#6)
# asm 2: movl <in9=%ecx,36(<out=%edi)
movl %ecx,36(%edi)

# qhasm:   in10 = x10
# asm 1: movl <x10=stack32#16,>in10=int32#2
# asm 2: movl <x10=60(%esp),>in10=%ecx
movl 60(%esp),%ecx

# qhasm:   in11 = x11
# asm 1: movl <x11=stack32#17,>in11=int32#7
# asm 2: movl <x11=64(%esp),>in11=%ebp
movl 64(%esp),%ebp

# qhasm:   in10 += j10
# asm 1: addl <j10=stack32#32,<in10=int32#2
# asm 2: addl <j10=124(%esp),<in10=%ecx
addl 124(%esp),%ecx

# qhasm:   in11 += j11
# asm 1: addl <j11=stack32#33,<in11=int32#7
# asm 2: addl <j11=128(%esp),<in11=%ebp
addl 128(%esp),%ebp

# qhasm:   in10 ^= *(uint32 *) (m + 40)
# asm 1: xorl 40(<m=int32#5),<in10=int32#2
# asm 2: xorl 40(<m=%esi),<in10=%ecx
xorl 40(%esi),%ecx

# qhasm:   in11 ^= *(uint32 *) (m + 44)
# asm 1: xorl 44(<m=int32#5),<in11=int32#7
# asm 2: xorl 44(<m=%esi),<in11=%ebp
xorl 44(%esi),%ebp

# qhasm:   *(uint32 *) (out + 40) = in10
# asm 1: movl <in10=int32#2,40(<out=int32#6)
# asm 2: movl <in10=%ecx,40(<out=%edi)
movl %ecx,40(%edi)

# qhasm:   *(uint32 *) (out + 44) = in11
# asm 1: movl <in11=int32#7,44(<out=int32#6)
# asm 2: movl <in11=%ebp,44(<out=%edi)
movl %ebp,44(%edi)

# qhasm:   in12 = x12
# asm 1: movl <x12=stack32#18,>in12=int32#1
# asm 2: movl <x12=68(%esp),>in12=%eax
movl 68(%esp),%eax

# qhasm:   in13 = x13
# asm 1: movl <x13=stack32#19,>in13=int32#2
# asm 2: movl <x13=72(%esp),>in13=%ecx
movl 72(%esp),%ecx

# qhasm:   in12 += j12
# asm 1: addl <j12=stack32#34,<in12=int32#1
# asm 2: addl <j12=132(%esp),<in12=%eax
addl 132(%esp),%eax

# qhasm:   in13 += j13
# asm 1: addl <j13=stack32#35,<in13=int32#2
# asm 2: addl <j13=136(%esp),<in13=%ecx
addl 136(%esp),%ecx

# qhasm:   in12 ^= *(uint32 *) (m + 48)
# asm 1: xorl 48(<m=int32#5),<in12=int32#1
# asm 2: xorl 48(<m=%esi),<in12=%eax
xorl 48(%esi),%eax

# qhasm:   in13 ^= *(uint32 *) (m + 52)
# asm 1: xorl 52(<m=int32#5),<in13=int32#2
# asm 2: xorl 52(<m=%esi),<in13=%ecx
xorl 52(%esi),%ecx

# qhasm:   *(uint32 *) (out + 48) = in12
# asm 1: movl <in12=int32#1,48(<out=int32#6)
# asm 2: movl <in12=%eax,48(<out=%edi)
movl %eax,48(%edi)

# qhasm:   *(uint32 *) (out + 52) = in13
# asm 1: movl <in13=int32#2,52(<out=int32#6)
# asm 2: movl <in13=%ecx,52(<out=%edi)
movl %ecx,52(%edi)

# qhasm:   in14 = x14
# asm 1: movl <x14=stack32#20,>in14=int32#7
# asm 2: movl <x14=76(%esp),>in14=%ebp
movl 76(%esp),%ebp

# qhasm:   in15 = x15
# asm 1: movl <x15=stack32#21,>in15=int32#1
# asm 2: movl <x15=80(%esp),>in15=%eax
movl 80(%esp),%eax

# qhasm:   in14 += j14
# asm 1: addl <j14=stack32#36,<in14=int32#7
# asm 2: addl <j14=140(%esp),<in14=%ebp
addl 140(%esp),%ebp

# qhasm:   in15 += j15
# asm 1: addl <j15=stack32#37,<in15=int32#1
# asm 2: addl <j15=144(%esp),<in15=%eax
addl 144(%esp),%eax

# qhasm:   in14 ^= *(uint32 *) (m + 56)
# asm 1: xorl 56(<m=int32#5),<in14=int32#7
# asm 2: xorl 56(<m=%esi),<in14=%ebp
xorl 56(%esi),%ebp

# qhasm:   in15 ^= *(uint32 *) (m + 60)
# asm 1: xorl 60(<m=int32#5),<in15=int32#1
# asm 2: xorl 60(<m=%esi),<in15=%eax
xorl 60(%esi),%eax

# qhasm:   *(uint32 *) (out + 56) = in14
# asm 1: movl <in14=int32#7,56(<out=int32#6)
# asm 2: movl <in14=%ebp,56(<out=%edi)
movl %ebp,56(%edi)

# qhasm:   *(uint32 *) (out + 60) = in15
# asm 1: movl <in15=int32#1,60(<out=int32#6)
# asm 2: movl <in15=%eax,60(<out=%edi)
movl %eax,60(%edi)

# qhasm:   bytes = bytes_backup
# asm 1: movl <bytes_backup=stack32#42,>bytes=int32#4
# asm 2: movl <bytes_backup=164(%esp),>bytes=%ebx
movl 164(%esp),%ebx

# qhasm:   in8 = j8
# asm 1: movl <j8=stack32#30,>in8=int32#7
# asm 2: movl <j8=116(%esp),>in8=%ebp
movl 116(%esp),%ebp

# qhasm:   in9 = j9
# asm 1: movl <j9=stack32#31,>in9=int32#1
# asm 2: movl <j9=120(%esp),>in9=%eax
movl 120(%esp),%eax

# qhasm:   in8 += 1
# asm 1: add  $1,<in8=int32#7
# asm 2: add  $1,<in8=%ebp
add  $1,%ebp

# qhasm:   in9 += 0 + carry
# asm 1: adc  $0,<in9=int32#1
# asm 2: adc  $0,<in9=%eax
adc  $0,%eax

# qhasm:   j8 = in8
# asm 1: movl <in8=int32#7,>j8=stack32#30
# asm 2: movl <in8=%ebp,>j8=116(%esp)
movl %ebp,116(%esp)

# qhasm:   j9 = in9
# asm 1: movl <in9=int32#1,>j9=stack32#31
# asm 2: movl <in9=%eax,>j9=120(%esp)
movl %eax,120(%esp)

# qhasm:                          unsigned>? unsigned<? bytes - 64
# asm 1: cmp  $64,<bytes=int32#4
# asm 2: cmp  $64,<bytes=%ebx
cmp  $64,%ebx
# comment:fp stack unchanged by jump

# qhasm:   goto bytesatleast65 if unsigned>
ja ._bytesatleast65
# comment:fp stack unchanged by jump

# qhasm:     goto bytesatleast64 if !unsigned<
jae ._bytesatleast64

# qhasm:       m = out
# asm 1: mov  <out=int32#6,>m=int32#5
# asm 2: mov  <out=%edi,>m=%esi
mov  %edi,%esi

# qhasm:       out = ctarget
# asm 1: movl <ctarget=stack32#39,>out=int32#6
# asm 2: movl <ctarget=152(%esp),>out=%edi
movl 152(%esp),%edi

# qhasm:       i = bytes
# asm 1: mov  <bytes=int32#4,>i=int32#2
# asm 2: mov  <bytes=%ebx,>i=%ecx
mov  %ebx,%ecx

# qhasm:       while (i) { *out++ = *m++; --i }
rep movsb
# comment:fp stack unchanged by fallthrough

# qhasm:     bytesatleast64:
._bytesatleast64:

# qhasm:     x = x_backup
# asm 1: movl <x_backup=stack32#38,>x=int32#3
# asm 2: movl <x_backup=148(%esp),>x=%edx
movl 148(%esp),%edx

# qhasm:     in8 = j8
# asm 1: movl <j8=stack32#30,>in8=int32#1
# asm 2: movl <j8=116(%esp),>in8=%eax
movl 116(%esp),%eax

# qhasm:     in9 = j9
# asm 1: movl <j9=stack32#31,>in9=int32#2
# asm 2: movl <j9=120(%esp),>in9=%ecx
movl 120(%esp),%ecx

# qhasm:     *(uint32 *) (x + 32) = in8
# asm 1: movl <in8=int32#1,32(<x=int32#3)
# asm 2: movl <in8=%eax,32(<x=%edx)
movl %eax,32(%edx)

# qhasm:     *(uint32 *) (x + 36) = in9
# asm 1: movl <in9=int32#2,36(<x=int32#3)
# asm 2: movl <in9=%ecx,36(<x=%edx)
movl %ecx,36(%edx)
# comment:fp stack unchanged by fallthrough

# qhasm:     done:
._done:

# qhasm:     eax = eax_stack
# asm 1: movl <eax_stack=stack32#1,>eax=int32#1
# asm 2: movl <eax_stack=0(%esp),>eax=%eax
movl 0(%esp),%eax

# qhasm:     ebx = ebx_stack
# asm 1: movl <ebx_stack=stack32#2,>ebx=int32#4
# asm 2: movl <ebx_stack=4(%esp),>ebx=%ebx
movl 4(%esp),%ebx

# qhasm:     esi = esi_stack
# asm 1: movl <esi_stack=stack32#3,>esi=int32#5
# asm 2: movl <esi_stack=8(%esp),>esi=%esi
movl 8(%esp),%esi

# qhasm:     edi = edi_stack
# asm 1: movl <edi_stack=stack32#4,>edi=int32#6
# asm 2: movl <edi_stack=12(%esp),>edi=%edi
movl 12(%esp),%edi

# qhasm:     ebp = ebp_stack
# asm 1: movl <ebp_stack=stack32#5,>ebp=int32#7
# asm 2: movl <ebp_stack=16(%esp),>ebp=%ebp
movl 16(%esp),%ebp

# qhasm:     leave
add %eax,%esp
ret

# qhasm:   bytesatleast65:
._bytesatleast65:

# qhasm:   bytes -= 64
# asm 1: sub  $64,<bytes=int32#4
# asm 2: sub  $64,<bytes=%ebx
sub  $64,%ebx

# qhasm:   out += 64
# asm 1: add  $64,<out=int32#6
# asm 2: add  $64,<out=%edi
add  $64,%edi

# qhasm:   m += 64
# asm 1: add  $64,<m=int32#5
# asm 2: add  $64,<m=%esi
add  $64,%esi
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
mov %esp,%eax
and $31,%eax
add $256,%eax
sub %eax,%esp

# qhasm: leave
add %eax,%esp
ret

# qhasm: enter ECRYPT_keysetup
.text
.p2align 5
.globl _ECRYPT_keysetup
.globl ECRYPT_keysetup
_ECRYPT_keysetup:
ECRYPT_keysetup:
mov %esp,%eax
and $31,%eax
add $256,%eax
sub %eax,%esp

# qhasm:   eax_stack = eax
# asm 1: movl <eax=int32#1,>eax_stack=stack32#1
# asm 2: movl <eax=%eax,>eax_stack=0(%esp)
movl %eax,0(%esp)

# qhasm:   ebx_stack = ebx
# asm 1: movl <ebx=int32#4,>ebx_stack=stack32#2
# asm 2: movl <ebx=%ebx,>ebx_stack=4(%esp)
movl %ebx,4(%esp)

# qhasm:   esi_stack = esi
# asm 1: movl <esi=int32#5,>esi_stack=stack32#3
# asm 2: movl <esi=%esi,>esi_stack=8(%esp)
movl %esi,8(%esp)

# qhasm:   edi_stack = edi
# asm 1: movl <edi=int32#6,>edi_stack=stack32#4
# asm 2: movl <edi=%edi,>edi_stack=12(%esp)
movl %edi,12(%esp)

# qhasm:   ebp_stack = ebp
# asm 1: movl <ebp=int32#7,>ebp_stack=stack32#5
# asm 2: movl <ebp=%ebp,>ebp_stack=16(%esp)
movl %ebp,16(%esp)

# qhasm:   k = arg2
# asm 1: movl <arg2=stack32#-2,>k=int32#2
# asm 2: movl <arg2=8(%esp,%eax),>k=%ecx
movl 8(%esp,%eax),%ecx

# qhasm:   kbits = arg3
# asm 1: movl <arg3=stack32#-3,>kbits=int32#3
# asm 2: movl <arg3=12(%esp,%eax),>kbits=%edx
movl 12(%esp,%eax),%edx

# qhasm:   x = arg1
# asm 1: movl <arg1=stack32#-1,>x=int32#1
# asm 2: movl <arg1=4(%esp,%eax),>x=%eax
movl 4(%esp,%eax),%eax

# qhasm:   in1 = *(uint32 *) (k + 0)
# asm 1: movl 0(<k=int32#2),>in1=int32#4
# asm 2: movl 0(<k=%ecx),>in1=%ebx
movl 0(%ecx),%ebx

# qhasm:   in2 = *(uint32 *) (k + 4)
# asm 1: movl 4(<k=int32#2),>in2=int32#5
# asm 2: movl 4(<k=%ecx),>in2=%esi
movl 4(%ecx),%esi

# qhasm:   in3 = *(uint32 *) (k + 8)
# asm 1: movl 8(<k=int32#2),>in3=int32#6
# asm 2: movl 8(<k=%ecx),>in3=%edi
movl 8(%ecx),%edi

# qhasm:   in4 = *(uint32 *) (k + 12)
# asm 1: movl 12(<k=int32#2),>in4=int32#7
# asm 2: movl 12(<k=%ecx),>in4=%ebp
movl 12(%ecx),%ebp

# qhasm:   *(uint32 *) (x + 4) = in1
# asm 1: movl <in1=int32#4,4(<x=int32#1)
# asm 2: movl <in1=%ebx,4(<x=%eax)
movl %ebx,4(%eax)

# qhasm:   *(uint32 *) (x + 8) = in2
# asm 1: movl <in2=int32#5,8(<x=int32#1)
# asm 2: movl <in2=%esi,8(<x=%eax)
movl %esi,8(%eax)

# qhasm:   *(uint32 *) (x + 12) = in3
# asm 1: movl <in3=int32#6,12(<x=int32#1)
# asm 2: movl <in3=%edi,12(<x=%eax)
movl %edi,12(%eax)

# qhasm:   *(uint32 *) (x + 16) = in4
# asm 1: movl <in4=int32#7,16(<x=int32#1)
# asm 2: movl <in4=%ebp,16(<x=%eax)
movl %ebp,16(%eax)

# qhasm:                    unsigned<? kbits - 256
# asm 1: cmp  $256,<kbits=int32#3
# asm 2: cmp  $256,<kbits=%edx
cmp  $256,%edx
# comment:fp stack unchanged by jump

# qhasm:   goto kbits128 if unsigned<
jb ._kbits128

# qhasm:   kbits256:
._kbits256:

# qhasm:     in11 = *(uint32 *) (k + 16)
# asm 1: movl 16(<k=int32#2),>in11=int32#3
# asm 2: movl 16(<k=%ecx),>in11=%edx
movl 16(%ecx),%edx

# qhasm:     in12 = *(uint32 *) (k + 20)
# asm 1: movl 20(<k=int32#2),>in12=int32#4
# asm 2: movl 20(<k=%ecx),>in12=%ebx
movl 20(%ecx),%ebx

# qhasm:     in13 = *(uint32 *) (k + 24)
# asm 1: movl 24(<k=int32#2),>in13=int32#5
# asm 2: movl 24(<k=%ecx),>in13=%esi
movl 24(%ecx),%esi

# qhasm:     in14 = *(uint32 *) (k + 28)
# asm 1: movl 28(<k=int32#2),>in14=int32#2
# asm 2: movl 28(<k=%ecx),>in14=%ecx
movl 28(%ecx),%ecx

# qhasm:     *(uint32 *) (x + 44) = in11
# asm 1: movl <in11=int32#3,44(<x=int32#1)
# asm 2: movl <in11=%edx,44(<x=%eax)
movl %edx,44(%eax)

# qhasm:     *(uint32 *) (x + 48) = in12
# asm 1: movl <in12=int32#4,48(<x=int32#1)
# asm 2: movl <in12=%ebx,48(<x=%eax)
movl %ebx,48(%eax)

# qhasm:     *(uint32 *) (x + 52) = in13
# asm 1: movl <in13=int32#5,52(<x=int32#1)
# asm 2: movl <in13=%esi,52(<x=%eax)
movl %esi,52(%eax)

# qhasm:     *(uint32 *) (x + 56) = in14
# asm 1: movl <in14=int32#2,56(<x=int32#1)
# asm 2: movl <in14=%ecx,56(<x=%eax)
movl %ecx,56(%eax)

# qhasm:     in0 = 1634760805
# asm 1: mov  $1634760805,>in0=int32#2
# asm 2: mov  $1634760805,>in0=%ecx
mov  $1634760805,%ecx

# qhasm:     in5 = 857760878
# asm 1: mov  $857760878,>in5=int32#3
# asm 2: mov  $857760878,>in5=%edx
mov  $857760878,%edx

# qhasm:     in10 = 2036477234
# asm 1: mov  $2036477234,>in10=int32#4
# asm 2: mov  $2036477234,>in10=%ebx
mov  $2036477234,%ebx

# qhasm:     in15 = 1797285236
# asm 1: mov  $1797285236,>in15=int32#5
# asm 2: mov  $1797285236,>in15=%esi
mov  $1797285236,%esi

# qhasm:     *(uint32 *) (x + 0) = in0
# asm 1: movl <in0=int32#2,0(<x=int32#1)
# asm 2: movl <in0=%ecx,0(<x=%eax)
movl %ecx,0(%eax)

# qhasm:     *(uint32 *) (x + 20) = in5
# asm 1: movl <in5=int32#3,20(<x=int32#1)
# asm 2: movl <in5=%edx,20(<x=%eax)
movl %edx,20(%eax)

# qhasm:     *(uint32 *) (x + 40) = in10
# asm 1: movl <in10=int32#4,40(<x=int32#1)
# asm 2: movl <in10=%ebx,40(<x=%eax)
movl %ebx,40(%eax)

# qhasm:     *(uint32 *) (x + 60) = in15
# asm 1: movl <in15=int32#5,60(<x=int32#1)
# asm 2: movl <in15=%esi,60(<x=%eax)
movl %esi,60(%eax)
# comment:fp stack unchanged by jump

# qhasm:   goto keysetupdone
jmp ._keysetupdone

# qhasm:   kbits128:
._kbits128:

# qhasm:     in11 = *(uint32 *) (k + 0)
# asm 1: movl 0(<k=int32#2),>in11=int32#3
# asm 2: movl 0(<k=%ecx),>in11=%edx
movl 0(%ecx),%edx

# qhasm:     in12 = *(uint32 *) (k + 4)
# asm 1: movl 4(<k=int32#2),>in12=int32#4
# asm 2: movl 4(<k=%ecx),>in12=%ebx
movl 4(%ecx),%ebx

# qhasm:     in13 = *(uint32 *) (k + 8)
# asm 1: movl 8(<k=int32#2),>in13=int32#5
# asm 2: movl 8(<k=%ecx),>in13=%esi
movl 8(%ecx),%esi

# qhasm:     in14 = *(uint32 *) (k + 12)
# asm 1: movl 12(<k=int32#2),>in14=int32#2
# asm 2: movl 12(<k=%ecx),>in14=%ecx
movl 12(%ecx),%ecx

# qhasm:     *(uint32 *) (x + 44) = in11
# asm 1: movl <in11=int32#3,44(<x=int32#1)
# asm 2: movl <in11=%edx,44(<x=%eax)
movl %edx,44(%eax)

# qhasm:     *(uint32 *) (x + 48) = in12
# asm 1: movl <in12=int32#4,48(<x=int32#1)
# asm 2: movl <in12=%ebx,48(<x=%eax)
movl %ebx,48(%eax)

# qhasm:     *(uint32 *) (x + 52) = in13
# asm 1: movl <in13=int32#5,52(<x=int32#1)
# asm 2: movl <in13=%esi,52(<x=%eax)
movl %esi,52(%eax)

# qhasm:     *(uint32 *) (x + 56) = in14
# asm 1: movl <in14=int32#2,56(<x=int32#1)
# asm 2: movl <in14=%ecx,56(<x=%eax)
movl %ecx,56(%eax)

# qhasm:     in0 = 1634760805
# asm 1: mov  $1634760805,>in0=int32#2
# asm 2: mov  $1634760805,>in0=%ecx
mov  $1634760805,%ecx

# qhasm:     in5 = 824206446
# asm 1: mov  $824206446,>in5=int32#3
# asm 2: mov  $824206446,>in5=%edx
mov  $824206446,%edx

# qhasm:     in10 = 2036477238
# asm 1: mov  $2036477238,>in10=int32#4
# asm 2: mov  $2036477238,>in10=%ebx
mov  $2036477238,%ebx

# qhasm:     in15 = 1797285236
# asm 1: mov  $1797285236,>in15=int32#5
# asm 2: mov  $1797285236,>in15=%esi
mov  $1797285236,%esi

# qhasm:     *(uint32 *) (x + 0) = in0
# asm 1: movl <in0=int32#2,0(<x=int32#1)
# asm 2: movl <in0=%ecx,0(<x=%eax)
movl %ecx,0(%eax)

# qhasm:     *(uint32 *) (x + 20) = in5
# asm 1: movl <in5=int32#3,20(<x=int32#1)
# asm 2: movl <in5=%edx,20(<x=%eax)
movl %edx,20(%eax)

# qhasm:     *(uint32 *) (x + 40) = in10
# asm 1: movl <in10=int32#4,40(<x=int32#1)
# asm 2: movl <in10=%ebx,40(<x=%eax)
movl %ebx,40(%eax)

# qhasm:     *(uint32 *) (x + 60) = in15
# asm 1: movl <in15=int32#5,60(<x=int32#1)
# asm 2: movl <in15=%esi,60(<x=%eax)
movl %esi,60(%eax)

# qhasm:   keysetupdone:
._keysetupdone:

# qhasm:   eax = eax_stack
# asm 1: movl <eax_stack=stack32#1,>eax=int32#1
# asm 2: movl <eax_stack=0(%esp),>eax=%eax
movl 0(%esp),%eax

# qhasm:   ebx = ebx_stack
# asm 1: movl <ebx_stack=stack32#2,>ebx=int32#4
# asm 2: movl <ebx_stack=4(%esp),>ebx=%ebx
movl 4(%esp),%ebx

# qhasm:   esi = esi_stack
# asm 1: movl <esi_stack=stack32#3,>esi=int32#5
# asm 2: movl <esi_stack=8(%esp),>esi=%esi
movl 8(%esp),%esi

# qhasm:   edi = edi_stack
# asm 1: movl <edi_stack=stack32#4,>edi=int32#6
# asm 2: movl <edi_stack=12(%esp),>edi=%edi
movl 12(%esp),%edi

# qhasm:   ebp = ebp_stack
# asm 1: movl <ebp_stack=stack32#5,>ebp=int32#7
# asm 2: movl <ebp_stack=16(%esp),>ebp=%ebp
movl 16(%esp),%ebp

# qhasm: leave
add %eax,%esp
ret

# qhasm: enter ECRYPT_ivsetup
.text
.p2align 5
.globl _ECRYPT_ivsetup
.globl ECRYPT_ivsetup
_ECRYPT_ivsetup:
ECRYPT_ivsetup:
mov %esp,%eax
and $31,%eax
add $256,%eax
sub %eax,%esp

# qhasm:   eax_stack = eax
# asm 1: movl <eax=int32#1,>eax_stack=stack32#1
# asm 2: movl <eax=%eax,>eax_stack=0(%esp)
movl %eax,0(%esp)

# qhasm:   ebx_stack = ebx
# asm 1: movl <ebx=int32#4,>ebx_stack=stack32#2
# asm 2: movl <ebx=%ebx,>ebx_stack=4(%esp)
movl %ebx,4(%esp)

# qhasm:   esi_stack = esi
# asm 1: movl <esi=int32#5,>esi_stack=stack32#3
# asm 2: movl <esi=%esi,>esi_stack=8(%esp)
movl %esi,8(%esp)

# qhasm:   edi_stack = edi
# asm 1: movl <edi=int32#6,>edi_stack=stack32#4
# asm 2: movl <edi=%edi,>edi_stack=12(%esp)
movl %edi,12(%esp)

# qhasm:   ebp_stack = ebp
# asm 1: movl <ebp=int32#7,>ebp_stack=stack32#5
# asm 2: movl <ebp=%ebp,>ebp_stack=16(%esp)
movl %ebp,16(%esp)

# qhasm:   iv = arg2
# asm 1: movl <arg2=stack32#-2,>iv=int32#2
# asm 2: movl <arg2=8(%esp,%eax),>iv=%ecx
movl 8(%esp,%eax),%ecx

# qhasm:   x = arg1
# asm 1: movl <arg1=stack32#-1,>x=int32#1
# asm 2: movl <arg1=4(%esp,%eax),>x=%eax
movl 4(%esp,%eax),%eax

# qhasm:   in6 = *(uint32 *) (iv + 0)
# asm 1: movl 0(<iv=int32#2),>in6=int32#3
# asm 2: movl 0(<iv=%ecx),>in6=%edx
movl 0(%ecx),%edx

# qhasm:   in7 = *(uint32 *) (iv + 4)
# asm 1: movl 4(<iv=int32#2),>in7=int32#2
# asm 2: movl 4(<iv=%ecx),>in7=%ecx
movl 4(%ecx),%ecx

# qhasm:   in8 = 0
# asm 1: mov  $0,>in8=int32#4
# asm 2: mov  $0,>in8=%ebx
mov  $0,%ebx

# qhasm:   in9 = 0
# asm 1: mov  $0,>in9=int32#5
# asm 2: mov  $0,>in9=%esi
mov  $0,%esi

# qhasm:   *(uint32 *) (x + 24) = in6
# asm 1: movl <in6=int32#3,24(<x=int32#1)
# asm 2: movl <in6=%edx,24(<x=%eax)
movl %edx,24(%eax)

# qhasm:   *(uint32 *) (x + 28) = in7
# asm 1: movl <in7=int32#2,28(<x=int32#1)
# asm 2: movl <in7=%ecx,28(<x=%eax)
movl %ecx,28(%eax)

# qhasm:   *(uint32 *) (x + 32) = in8
# asm 1: movl <in8=int32#4,32(<x=int32#1)
# asm 2: movl <in8=%ebx,32(<x=%eax)
movl %ebx,32(%eax)

# qhasm:   *(uint32 *) (x + 36) = in9
# asm 1: movl <in9=int32#5,36(<x=int32#1)
# asm 2: movl <in9=%esi,36(<x=%eax)
movl %esi,36(%eax)

# qhasm:   eax = eax_stack
# asm 1: movl <eax_stack=stack32#1,>eax=int32#1
# asm 2: movl <eax_stack=0(%esp),>eax=%eax
movl 0(%esp),%eax

# qhasm:   ebx = ebx_stack
# asm 1: movl <ebx_stack=stack32#2,>ebx=int32#4
# asm 2: movl <ebx_stack=4(%esp),>ebx=%ebx
movl 4(%esp),%ebx

# qhasm:   esi = esi_stack
# asm 1: movl <esi_stack=stack32#3,>esi=int32#5
# asm 2: movl <esi_stack=8(%esp),>esi=%esi
movl 8(%esp),%esi

# qhasm:   edi = edi_stack
# asm 1: movl <edi_stack=stack32#4,>edi=int32#6
# asm 2: movl <edi_stack=12(%esp),>edi=%edi
movl 12(%esp),%edi

# qhasm:   ebp = ebp_stack
# asm 1: movl <ebp_stack=stack32#5,>ebp=int32#7
# asm 2: movl <ebp_stack=16(%esp),>ebp=%ebp
movl 16(%esp),%ebp

# qhasm: leave
add %eax,%esp
ret
