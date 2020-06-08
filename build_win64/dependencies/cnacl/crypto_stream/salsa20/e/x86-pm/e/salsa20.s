# salsa20_pm.s version 20051229
# D. J. Bernstein
# Public domain.

# int32 a

# int32 p

# int32 s

# int32 t

# int32 w

# int32 r

# int32 v

# stack32 arg1

# stack32 arg2

# stack32 arg3

# stack32 arg4

# input arg1

# input arg2

# input arg3

# input arg4

# int32 eax

# int32 ebx

# int32 esi

# int32 edi

# int32 ebp

# caller eax

# caller ebx

# caller esi

# caller edi

# caller ebp

# int32 k

# int32 kbits

# int32 iv

# int32 i

# stack32 x_backup

# int32 x

# stack32 m_backup

# int32 m

# stack32 out_backup

# int32 out

# stack32 bytes_backup

# int32 bytes

# stack32 eax_stack

# stack32 ebx_stack

# stack32 esi_stack

# stack32 edi_stack

# stack32 ebp_stack

# int32 in0

# int32 in1

# int32 in2

# int32 in3

# int32 in4

# int32 in5

# int32 in6

# int32 in7

# int32 in8

# int32 in9

# int32 in10

# int32 in11

# int32 in12

# int32 in13

# int32 in14

# int32 in15

# stack32 x0

# stack32 x1

# stack32 x2

# stack32 x3

# stack32 x4

# stack32 x5

# stack32 x6

# stack32 x7

# stack32 x8

# stack32 x9

# stack32 x10

# stack32 x11

# stack32 x12

# stack32 x13

# stack32 x14

# stack32 x15

# stack32 j0

# stack32 j1

# stack32 j2

# stack32 j3

# stack32 j4

# stack32 j5

# stack32 j6

# stack32 j7

# stack32 j8

# stack32 j9

# stack32 j10

# stack32 j11

# stack32 j12

# stack32 j13

# stack32 j14

# stack32 j15

# stack512 tmp

# stack32 ctarget

# enter ECRYPT_keystream_bytes
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

# eax_stack = eax
# movl <eax=int32#1,>eax_stack=stack32#5
# movl <eax=%eax,>eax_stack=80(%esp)
movl %eax,80(%esp)

# ebx_stack = ebx
# movl <ebx=int32#4,>ebx_stack=stack32#6
# movl <ebx=%ebx,>ebx_stack=84(%esp)
movl %ebx,84(%esp)

# esi_stack = esi
# movl <esi=int32#5,>esi_stack=stack32#7
# movl <esi=%esi,>esi_stack=88(%esp)
movl %esi,88(%esp)

# edi_stack = edi
# movl <edi=int32#6,>edi_stack=stack32#8
# movl <edi=%edi,>edi_stack=92(%esp)
movl %edi,92(%esp)

# ebp_stack = ebp
# movl <ebp=int32#7,>ebp_stack=stack32#9
# movl <ebp=%ebp,>ebp_stack=96(%esp)
movl %ebp,96(%esp)

# x = arg1
# movl <arg1=stack32#-1,>x=int32#3
# movl <arg1=4(%esp,%eax),>x=%edx
movl 4(%esp,%eax),%edx

# m = arg2
# movl <arg2=stack32#-2,>m=int32#5
# movl <arg2=8(%esp,%eax),>m=%esi
movl 8(%esp,%eax),%esi

# out = m
# mov  <m=int32#5,>out=int32#6
# mov  <m=%esi,>out=%edi
mov  %esi,%edi

# bytes = arg3
# movl <arg3=stack32#-3,>bytes=int32#4
# movl <arg3=12(%esp,%eax),>bytes=%ebx
movl 12(%esp,%eax),%ebx

# bytes -= 0
# sub  $0,<bytes=int32#4
# sub  $0,<bytes=%ebx
sub  $0,%ebx

# goto done if unsigned<=
jbe ._done

# a = 0
# mov  $0,>a=int32#1
# mov  $0,>a=%eax
mov  $0,%eax

# i = bytes
# mov  <bytes=int32#4,>i=int32#2
# mov  <bytes=%ebx,>i=%ecx
mov  %ebx,%ecx

# while (i) { *out++ = a; --i }
rep stosb

# out -= bytes
# sub  <bytes=int32#4,<out=int32#6
# sub  <bytes=%ebx,<out=%edi
sub  %ebx,%edi

# goto start
jmp ._start

# enter ECRYPT_decrypt_bytes
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

# eax_stack = eax
# movl <eax=int32#1,>eax_stack=stack32#5
# movl <eax=%eax,>eax_stack=80(%esp)
movl %eax,80(%esp)

# ebx_stack = ebx
# movl <ebx=int32#4,>ebx_stack=stack32#6
# movl <ebx=%ebx,>ebx_stack=84(%esp)
movl %ebx,84(%esp)

# esi_stack = esi
# movl <esi=int32#5,>esi_stack=stack32#7
# movl <esi=%esi,>esi_stack=88(%esp)
movl %esi,88(%esp)

# edi_stack = edi
# movl <edi=int32#6,>edi_stack=stack32#8
# movl <edi=%edi,>edi_stack=92(%esp)
movl %edi,92(%esp)

# ebp_stack = ebp
# movl <ebp=int32#7,>ebp_stack=stack32#9
# movl <ebp=%ebp,>ebp_stack=96(%esp)
movl %ebp,96(%esp)

# x = arg1
# movl <arg1=stack32#-1,>x=int32#3
# movl <arg1=4(%esp,%eax),>x=%edx
movl 4(%esp,%eax),%edx

# m = arg2
# movl <arg2=stack32#-2,>m=int32#5
# movl <arg2=8(%esp,%eax),>m=%esi
movl 8(%esp,%eax),%esi

# out = arg3
# movl <arg3=stack32#-3,>out=int32#6
# movl <arg3=12(%esp,%eax),>out=%edi
movl 12(%esp,%eax),%edi

# bytes = arg4
# movl <arg4=stack32#-4,>bytes=int32#4
# movl <arg4=16(%esp,%eax),>bytes=%ebx
movl 16(%esp,%eax),%ebx

# bytes -= 0
# sub  $0,<bytes=int32#4
# sub  $0,<bytes=%ebx
sub  $0,%ebx

# goto done if unsigned<=
jbe ._done

# goto start
jmp ._start

# enter ECRYPT_encrypt_bytes
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

# eax_stack = eax
# movl <eax=int32#1,>eax_stack=stack32#5
# movl <eax=%eax,>eax_stack=80(%esp)
movl %eax,80(%esp)

# ebx_stack = ebx
# movl <ebx=int32#4,>ebx_stack=stack32#6
# movl <ebx=%ebx,>ebx_stack=84(%esp)
movl %ebx,84(%esp)

# esi_stack = esi
# movl <esi=int32#5,>esi_stack=stack32#7
# movl <esi=%esi,>esi_stack=88(%esp)
movl %esi,88(%esp)

# edi_stack = edi
# movl <edi=int32#6,>edi_stack=stack32#8
# movl <edi=%edi,>edi_stack=92(%esp)
movl %edi,92(%esp)

# ebp_stack = ebp
# movl <ebp=int32#7,>ebp_stack=stack32#9
# movl <ebp=%ebp,>ebp_stack=96(%esp)
movl %ebp,96(%esp)

# x = arg1
# movl <arg1=stack32#-1,>x=int32#3
# movl <arg1=4(%esp,%eax),>x=%edx
movl 4(%esp,%eax),%edx

# m = arg2
# movl <arg2=stack32#-2,>m=int32#5
# movl <arg2=8(%esp,%eax),>m=%esi
movl 8(%esp,%eax),%esi

# out = arg3
# movl <arg3=stack32#-3,>out=int32#6
# movl <arg3=12(%esp,%eax),>out=%edi
movl 12(%esp,%eax),%edi

# bytes = arg4
# movl <arg4=stack32#-4,>bytes=int32#4
# movl <arg4=16(%esp,%eax),>bytes=%ebx
movl 16(%esp,%eax),%ebx

# bytes -= 0
# sub  $0,<bytes=int32#4
# sub  $0,<bytes=%ebx
sub  $0,%ebx

# goto done if unsigned<=
jbe ._done

# start#
._start:

# in0 = *(uint32 *) (x + 0)
# movl 0(<x=int32#3),>in0=int32#1
# movl 0(<x=%edx),>in0=%eax
movl 0(%edx),%eax

# in1 = *(uint32 *) (x + 4)
# movl 4(<x=int32#3),>in1=int32#2
# movl 4(<x=%edx),>in1=%ecx
movl 4(%edx),%ecx

# in2 = *(uint32 *) (x + 8)
# movl 8(<x=int32#3),>in2=int32#7
# movl 8(<x=%edx),>in2=%ebp
movl 8(%edx),%ebp

# j0 = in0
# movl <in0=int32#1,>j0=stack32#26
# movl <in0=%eax,>j0=164(%esp)
movl %eax,164(%esp)

# in3 = *(uint32 *) (x + 12)
# movl 12(<x=int32#3),>in3=int32#1
# movl 12(<x=%edx),>in3=%eax
movl 12(%edx),%eax

# j1 = in1
# movl <in1=int32#2,>j1=stack32#27
# movl <in1=%ecx,>j1=168(%esp)
movl %ecx,168(%esp)

# in4 = *(uint32 *) (x + 16)
# movl 16(<x=int32#3),>in4=int32#2
# movl 16(<x=%edx),>in4=%ecx
movl 16(%edx),%ecx

# j2 = in2
# movl <in2=int32#7,>j2=stack32#28
# movl <in2=%ebp,>j2=172(%esp)
movl %ebp,172(%esp)

# in5 = *(uint32 *) (x + 20)
# movl 20(<x=int32#3),>in5=int32#7
# movl 20(<x=%edx),>in5=%ebp
movl 20(%edx),%ebp

# j3 = in3
# movl <in3=int32#1,>j3=stack32#29
# movl <in3=%eax,>j3=176(%esp)
movl %eax,176(%esp)

# in6 = *(uint32 *) (x + 24)
# movl 24(<x=int32#3),>in6=int32#1
# movl 24(<x=%edx),>in6=%eax
movl 24(%edx),%eax

# j4 = in4
# movl <in4=int32#2,>j4=stack32#30
# movl <in4=%ecx,>j4=180(%esp)
movl %ecx,180(%esp)

# in7 = *(uint32 *) (x + 28)
# movl 28(<x=int32#3),>in7=int32#2
# movl 28(<x=%edx),>in7=%ecx
movl 28(%edx),%ecx

# j5 = in5
# movl <in5=int32#7,>j5=stack32#31
# movl <in5=%ebp,>j5=184(%esp)
movl %ebp,184(%esp)

# in8 = *(uint32 *) (x + 32)
# movl 32(<x=int32#3),>in8=int32#7
# movl 32(<x=%edx),>in8=%ebp
movl 32(%edx),%ebp

# j6 = in6
# movl <in6=int32#1,>j6=stack32#32
# movl <in6=%eax,>j6=188(%esp)
movl %eax,188(%esp)

# in9 = *(uint32 *) (x + 36)
# movl 36(<x=int32#3),>in9=int32#1
# movl 36(<x=%edx),>in9=%eax
movl 36(%edx),%eax

# j7 = in7
# movl <in7=int32#2,>j7=stack32#33
# movl <in7=%ecx,>j7=192(%esp)
movl %ecx,192(%esp)

# in10 = *(uint32 *) (x + 40)
# movl 40(<x=int32#3),>in10=int32#2
# movl 40(<x=%edx),>in10=%ecx
movl 40(%edx),%ecx

# j8 = in8
# movl <in8=int32#7,>j8=stack32#34
# movl <in8=%ebp,>j8=196(%esp)
movl %ebp,196(%esp)

# in11 = *(uint32 *) (x + 44)
# movl 44(<x=int32#3),>in11=int32#7
# movl 44(<x=%edx),>in11=%ebp
movl 44(%edx),%ebp

# j9 = in9
# movl <in9=int32#1,>j9=stack32#35
# movl <in9=%eax,>j9=200(%esp)
movl %eax,200(%esp)

# in12 = *(uint32 *) (x + 48)
# movl 48(<x=int32#3),>in12=int32#1
# movl 48(<x=%edx),>in12=%eax
movl 48(%edx),%eax

# j10 = in10
# movl <in10=int32#2,>j10=stack32#36
# movl <in10=%ecx,>j10=204(%esp)
movl %ecx,204(%esp)

# in13 = *(uint32 *) (x + 52)
# movl 52(<x=int32#3),>in13=int32#2
# movl 52(<x=%edx),>in13=%ecx
movl 52(%edx),%ecx

# j11 = in11
# movl <in11=int32#7,>j11=stack32#37
# movl <in11=%ebp,>j11=208(%esp)
movl %ebp,208(%esp)

# in14 = *(uint32 *) (x + 56)
# movl 56(<x=int32#3),>in14=int32#7
# movl 56(<x=%edx),>in14=%ebp
movl 56(%edx),%ebp

# j12 = in12
# movl <in12=int32#1,>j12=stack32#38
# movl <in12=%eax,>j12=212(%esp)
movl %eax,212(%esp)

# in15 = *(uint32 *) (x + 60)
# movl 60(<x=int32#3),>in15=int32#1
# movl 60(<x=%edx),>in15=%eax
movl 60(%edx),%eax

# j13 = in13
# movl <in13=int32#2,>j13=stack32#39
# movl <in13=%ecx,>j13=216(%esp)
movl %ecx,216(%esp)

# j14 = in14
# movl <in14=int32#7,>j14=stack32#40
# movl <in14=%ebp,>j14=220(%esp)
movl %ebp,220(%esp)

# j15 = in15
# movl <in15=int32#1,>j15=stack32#41
# movl <in15=%eax,>j15=224(%esp)
movl %eax,224(%esp)

# x_backup = x
# movl <x=int32#3,>x_backup=stack32#1
# movl <x=%edx,>x_backup=64(%esp)
movl %edx,64(%esp)

# bytesatleast1#
._bytesatleast1:

#   bytes - 64
# cmp  $64,<bytes=int32#4
# cmp  $64,<bytes=%ebx
cmp  $64,%ebx

#   goto nocopy if unsigned>=
jae ._nocopy

#     ctarget = out
# movl <out=int32#6,>ctarget=stack32#42
# movl <out=%edi,>ctarget=228(%esp)
movl %edi,228(%esp)

#     out = &tmp
# leal >tmp=stack512#1,>out=int32#6
# leal >tmp=0(%esp),>out=%edi
leal 0(%esp),%edi

#     i = bytes
# mov  <bytes=int32#4,>i=int32#2
# mov  <bytes=%ebx,>i=%ecx
mov  %ebx,%ecx

#     while (i) { *out++ = *m++; --i }
rep movsb

#     out = &tmp
# leal >tmp=stack512#1,>out=int32#6
# leal >tmp=0(%esp),>out=%edi
leal 0(%esp),%edi

#     m = &tmp
# leal >tmp=stack512#1,>m=int32#5
# leal >tmp=0(%esp),>m=%esi
leal 0(%esp),%esi

#   nocopy#
._nocopy:

#   out_backup = out
# movl <out=int32#6,>out_backup=stack32#3
# movl <out=%edi,>out_backup=72(%esp)
movl %edi,72(%esp)

#   m_backup = m
# movl <m=int32#5,>m_backup=stack32#2
# movl <m=%esi,>m_backup=68(%esp)
movl %esi,68(%esp)

#   bytes_backup = bytes
# movl <bytes=int32#4,>bytes_backup=stack32#4
# movl <bytes=%ebx,>bytes_backup=76(%esp)
movl %ebx,76(%esp)

#   in0 = j0
# movl <j0=stack32#26,>in0=int32#1
# movl <j0=164(%esp),>in0=%eax
movl 164(%esp),%eax

#   in1 = j1
# movl <j1=stack32#27,>in1=int32#2
# movl <j1=168(%esp),>in1=%ecx
movl 168(%esp),%ecx

#   in2 = j2
# movl <j2=stack32#28,>in2=int32#3
# movl <j2=172(%esp),>in2=%edx
movl 172(%esp),%edx

#   in3 = j3
# movl <j3=stack32#29,>in3=int32#4
# movl <j3=176(%esp),>in3=%ebx
movl 176(%esp),%ebx

#   x0 = in0
# movl <in0=int32#1,>x0=stack32#10
# movl <in0=%eax,>x0=100(%esp)
movl %eax,100(%esp)

#   x1 = in1
# movl <in1=int32#2,>x1=stack32#11
# movl <in1=%ecx,>x1=104(%esp)
movl %ecx,104(%esp)

#   x2 = in2
# movl <in2=int32#3,>x2=stack32#12
# movl <in2=%edx,>x2=108(%esp)
movl %edx,108(%esp)

#   x3 = in3
# movl <in3=int32#4,>x3=stack32#13
# movl <in3=%ebx,>x3=112(%esp)
movl %ebx,112(%esp)

#   in4 = j4
# movl <j4=stack32#30,>in4=int32#1
# movl <j4=180(%esp),>in4=%eax
movl 180(%esp),%eax

#   in5 = j5
# movl <j5=stack32#31,>in5=int32#2
# movl <j5=184(%esp),>in5=%ecx
movl 184(%esp),%ecx

#   in6 = j6
# movl <j6=stack32#32,>in6=int32#3
# movl <j6=188(%esp),>in6=%edx
movl 188(%esp),%edx

#   in7 = j7
# movl <j7=stack32#33,>in7=int32#4
# movl <j7=192(%esp),>in7=%ebx
movl 192(%esp),%ebx

#   x4 = in4
# movl <in4=int32#1,>x4=stack32#14
# movl <in4=%eax,>x4=116(%esp)
movl %eax,116(%esp)

#   x5 = in5
# movl <in5=int32#2,>x5=stack32#15
# movl <in5=%ecx,>x5=120(%esp)
movl %ecx,120(%esp)

#   x6 = in6
# movl <in6=int32#3,>x6=stack32#16
# movl <in6=%edx,>x6=124(%esp)
movl %edx,124(%esp)

#   x7 = in7
# movl <in7=int32#4,>x7=stack32#17
# movl <in7=%ebx,>x7=128(%esp)
movl %ebx,128(%esp)

#   in8 = j8
# movl <j8=stack32#34,>in8=int32#1
# movl <j8=196(%esp),>in8=%eax
movl 196(%esp),%eax

#   in9 = j9
# movl <j9=stack32#35,>in9=int32#2
# movl <j9=200(%esp),>in9=%ecx
movl 200(%esp),%ecx

#   in10 = j10
# movl <j10=stack32#36,>in10=int32#3
# movl <j10=204(%esp),>in10=%edx
movl 204(%esp),%edx

#   in11 = j11
# movl <j11=stack32#37,>in11=int32#4
# movl <j11=208(%esp),>in11=%ebx
movl 208(%esp),%ebx

#   x8 = in8
# movl <in8=int32#1,>x8=stack32#18
# movl <in8=%eax,>x8=132(%esp)
movl %eax,132(%esp)

#   x9 = in9
# movl <in9=int32#2,>x9=stack32#19
# movl <in9=%ecx,>x9=136(%esp)
movl %ecx,136(%esp)

#   x10 = in10
# movl <in10=int32#3,>x10=stack32#20
# movl <in10=%edx,>x10=140(%esp)
movl %edx,140(%esp)

#   x11 = in11
# movl <in11=int32#4,>x11=stack32#21
# movl <in11=%ebx,>x11=144(%esp)
movl %ebx,144(%esp)

#   in12 = j12
# movl <j12=stack32#38,>in12=int32#1
# movl <j12=212(%esp),>in12=%eax
movl 212(%esp),%eax

#   in13 = j13
# movl <j13=stack32#39,>in13=int32#2
# movl <j13=216(%esp),>in13=%ecx
movl 216(%esp),%ecx

#   in14 = j14
# movl <j14=stack32#40,>in14=int32#3
# movl <j14=220(%esp),>in14=%edx
movl 220(%esp),%edx

#   in15 = j15
# movl <j15=stack32#41,>in15=int32#4
# movl <j15=224(%esp),>in15=%ebx
movl 224(%esp),%ebx

#   x12 = in12
# movl <in12=int32#1,>x12=stack32#22
# movl <in12=%eax,>x12=148(%esp)
movl %eax,148(%esp)

#   x13 = in13
# movl <in13=int32#2,>x13=stack32#23
# movl <in13=%ecx,>x13=152(%esp)
movl %ecx,152(%esp)

#   x14 = in14
# movl <in14=int32#3,>x14=stack32#24
# movl <in14=%edx,>x14=156(%esp)
movl %edx,156(%esp)

#   x15 = in15
# movl <in15=int32#4,>x15=stack32#25
# movl <in15=%ebx,>x15=160(%esp)
movl %ebx,160(%esp)

#   i = 20
# mov  $20,>i=int32#7
# mov  $20,>i=%ebp
mov  $20,%ebp

# p = x0
# movl <x0=stack32#10,>p=int32#1
# movl <x0=100(%esp),>p=%eax
movl 100(%esp),%eax

# s = x5
# movl <x5=stack32#15,>s=int32#2
# movl <x5=120(%esp),>s=%ecx
movl 120(%esp),%ecx

# t = x10
# movl <x10=stack32#20,>t=int32#3
# movl <x10=140(%esp),>t=%edx
movl 140(%esp),%edx

# w = x15
# movl <x15=stack32#25,>w=int32#4
# movl <x15=160(%esp),>w=%ebx
movl 160(%esp),%ebx

# mainloop#
._mainloop:

# x0 = p
# movl <p=int32#1,>x0=stack32#10
# movl <p=%eax,>x0=100(%esp)
movl %eax,100(%esp)

# 				x10 = t
# movl <t=int32#3,>x10=stack32#20
# movl <t=%edx,>x10=140(%esp)
movl %edx,140(%esp)

# p += x12
# addl <x12=stack32#22,<p=int32#1
# addl <x12=148(%esp),<p=%eax
addl 148(%esp),%eax

# 		x5 = s
# movl <s=int32#2,>x5=stack32#15
# movl <s=%ecx,>x5=120(%esp)
movl %ecx,120(%esp)

# 				t += x6
# addl <x6=stack32#16,<t=int32#3
# addl <x6=124(%esp),<t=%edx
addl 124(%esp),%edx

# 						x15 = w
# movl <w=int32#4,>x15=stack32#25
# movl <w=%ebx,>x15=160(%esp)
movl %ebx,160(%esp)

# 		r = x1
# movl <x1=stack32#11,>r=int32#5
# movl <x1=104(%esp),>r=%esi
movl 104(%esp),%esi

# 		r += s
# add  <s=int32#2,<r=int32#5
# add  <s=%ecx,<r=%esi
add  %ecx,%esi

# 						v = x11
# movl <x11=stack32#21,>v=int32#6
# movl <x11=144(%esp),>v=%edi
movl 144(%esp),%edi

# 						v += w
# add  <w=int32#4,<v=int32#6
# add  <w=%ebx,<v=%edi
add  %ebx,%edi

# p <<<= 7
# rol  $7,<p=int32#1
# rol  $7,<p=%eax
rol  $7,%eax

# p ^= x4
# xorl <x4=stack32#14,<p=int32#1
# xorl <x4=116(%esp),<p=%eax
xorl 116(%esp),%eax

# 				t <<<= 7
# rol  $7,<t=int32#3
# rol  $7,<t=%edx
rol  $7,%edx

# 				t ^= x14
# xorl <x14=stack32#24,<t=int32#3
# xorl <x14=156(%esp),<t=%edx
xorl 156(%esp),%edx

# 		r <<<= 7
# rol  $7,<r=int32#5
# rol  $7,<r=%esi
rol  $7,%esi

# 		r ^= x9
# xorl <x9=stack32#19,<r=int32#5
# xorl <x9=136(%esp),<r=%esi
xorl 136(%esp),%esi

# 						v <<<= 7
# rol  $7,<v=int32#6
# rol  $7,<v=%edi
rol  $7,%edi

# 						v ^= x3
# xorl <x3=stack32#13,<v=int32#6
# xorl <x3=112(%esp),<v=%edi
xorl 112(%esp),%edi

# x4 = p
# movl <p=int32#1,>x4=stack32#14
# movl <p=%eax,>x4=116(%esp)
movl %eax,116(%esp)

# 				x14 = t
# movl <t=int32#3,>x14=stack32#24
# movl <t=%edx,>x14=156(%esp)
movl %edx,156(%esp)

# p += x0
# addl <x0=stack32#10,<p=int32#1
# addl <x0=100(%esp),<p=%eax
addl 100(%esp),%eax

# 		x9 = r
# movl <r=int32#5,>x9=stack32#19
# movl <r=%esi,>x9=136(%esp)
movl %esi,136(%esp)

# 				t += x10
# addl <x10=stack32#20,<t=int32#3
# addl <x10=140(%esp),<t=%edx
addl 140(%esp),%edx

# 						x3 = v
# movl <v=int32#6,>x3=stack32#13
# movl <v=%edi,>x3=112(%esp)
movl %edi,112(%esp)

# p <<<= 9
# rol  $9,<p=int32#1
# rol  $9,<p=%eax
rol  $9,%eax

# p ^= x8
# xorl <x8=stack32#18,<p=int32#1
# xorl <x8=132(%esp),<p=%eax
xorl 132(%esp),%eax

# 				t <<<= 9
# rol  $9,<t=int32#3
# rol  $9,<t=%edx
rol  $9,%edx

# 				t ^= x2
# xorl <x2=stack32#12,<t=int32#3
# xorl <x2=108(%esp),<t=%edx
xorl 108(%esp),%edx

# 		s += r
# add  <r=int32#5,<s=int32#2
# add  <r=%esi,<s=%ecx
add  %esi,%ecx

# 		s <<<= 9
# rol  $9,<s=int32#2
# rol  $9,<s=%ecx
rol  $9,%ecx

# 		s ^= x13
# xorl <x13=stack32#23,<s=int32#2
# xorl <x13=152(%esp),<s=%ecx
xorl 152(%esp),%ecx

# 						w += v
# add  <v=int32#6,<w=int32#4
# add  <v=%edi,<w=%ebx
add  %edi,%ebx

# 						w <<<= 9
# rol  $9,<w=int32#4
# rol  $9,<w=%ebx
rol  $9,%ebx

# 						w ^= x7
# xorl <x7=stack32#17,<w=int32#4
# xorl <x7=128(%esp),<w=%ebx
xorl 128(%esp),%ebx

# x8 = p
# movl <p=int32#1,>x8=stack32#18
# movl <p=%eax,>x8=132(%esp)
movl %eax,132(%esp)

# 				x2 = t
# movl <t=int32#3,>x2=stack32#12
# movl <t=%edx,>x2=108(%esp)
movl %edx,108(%esp)

# p += x4
# addl <x4=stack32#14,<p=int32#1
# addl <x4=116(%esp),<p=%eax
addl 116(%esp),%eax

# 		x13 = s
# movl <s=int32#2,>x13=stack32#23
# movl <s=%ecx,>x13=152(%esp)
movl %ecx,152(%esp)

# 				t += x14
# addl <x14=stack32#24,<t=int32#3
# addl <x14=156(%esp),<t=%edx
addl 156(%esp),%edx

# 						x7 = w
# movl <w=int32#4,>x7=stack32#17
# movl <w=%ebx,>x7=128(%esp)
movl %ebx,128(%esp)

# p <<<= 13
# rol  $13,<p=int32#1
# rol  $13,<p=%eax
rol  $13,%eax

# p ^= x12
# xorl <x12=stack32#22,<p=int32#1
# xorl <x12=148(%esp),<p=%eax
xorl 148(%esp),%eax

# 				t <<<= 13
# rol  $13,<t=int32#3
# rol  $13,<t=%edx
rol  $13,%edx

# 				t ^= x6
# xorl <x6=stack32#16,<t=int32#3
# xorl <x6=124(%esp),<t=%edx
xorl 124(%esp),%edx

# 		r += s
# add  <s=int32#2,<r=int32#5
# add  <s=%ecx,<r=%esi
add  %ecx,%esi

# 		r <<<= 13
# rol  $13,<r=int32#5
# rol  $13,<r=%esi
rol  $13,%esi

# 		r ^= x1
# xorl <x1=stack32#11,<r=int32#5
# xorl <x1=104(%esp),<r=%esi
xorl 104(%esp),%esi

# 						v += w
# add  <w=int32#4,<v=int32#6
# add  <w=%ebx,<v=%edi
add  %ebx,%edi

# 						v <<<= 13
# rol  $13,<v=int32#6
# rol  $13,<v=%edi
rol  $13,%edi

# 						v ^= x11
# xorl <x11=stack32#21,<v=int32#6
# xorl <x11=144(%esp),<v=%edi
xorl 144(%esp),%edi

# x12 = p
# movl <p=int32#1,>x12=stack32#22
# movl <p=%eax,>x12=148(%esp)
movl %eax,148(%esp)

# 				x6 = t
# movl <t=int32#3,>x6=stack32#16
# movl <t=%edx,>x6=124(%esp)
movl %edx,124(%esp)

# p += x8
# addl <x8=stack32#18,<p=int32#1
# addl <x8=132(%esp),<p=%eax
addl 132(%esp),%eax

# 		x1 = r
# movl <r=int32#5,>x1=stack32#11
# movl <r=%esi,>x1=104(%esp)
movl %esi,104(%esp)

# 				t += x2
# addl <x2=stack32#12,<t=int32#3
# addl <x2=108(%esp),<t=%edx
addl 108(%esp),%edx

# 						x11 = v
# movl <v=int32#6,>x11=stack32#21
# movl <v=%edi,>x11=144(%esp)
movl %edi,144(%esp)

# p <<<= 18
# rol  $18,<p=int32#1
# rol  $18,<p=%eax
rol  $18,%eax

# p ^= x0
# xorl <x0=stack32#10,<p=int32#1
# xorl <x0=100(%esp),<p=%eax
xorl 100(%esp),%eax

# 				t <<<= 18
# rol  $18,<t=int32#3
# rol  $18,<t=%edx
rol  $18,%edx

# 				t ^= x10
# xorl <x10=stack32#20,<t=int32#3
# xorl <x10=140(%esp),<t=%edx
xorl 140(%esp),%edx

# 		s += r
# add  <r=int32#5,<s=int32#2
# add  <r=%esi,<s=%ecx
add  %esi,%ecx

# 		s <<<= 18
# rol  $18,<s=int32#2
# rol  $18,<s=%ecx
rol  $18,%ecx

# 		s ^= x5
# xorl <x5=stack32#15,<s=int32#2
# xorl <x5=120(%esp),<s=%ecx
xorl 120(%esp),%ecx

# 						w += v
# add  <v=int32#6,<w=int32#4
# add  <v=%edi,<w=%ebx
add  %edi,%ebx

# 						w <<<= 18
# rol  $18,<w=int32#4
# rol  $18,<w=%ebx
rol  $18,%ebx

# 						w ^= x15
# xorl <x15=stack32#25,<w=int32#4
# xorl <x15=160(%esp),<w=%ebx
xorl 160(%esp),%ebx

# x0 = p
# movl <p=int32#1,>x0=stack32#10
# movl <p=%eax,>x0=100(%esp)
movl %eax,100(%esp)

# 				x10 = t
# movl <t=int32#3,>x10=stack32#20
# movl <t=%edx,>x10=140(%esp)
movl %edx,140(%esp)

# p += x3
# addl <x3=stack32#13,<p=int32#1
# addl <x3=112(%esp),<p=%eax
addl 112(%esp),%eax

# p <<<= 7
# rol  $7,<p=int32#1
# rol  $7,<p=%eax
rol  $7,%eax

# 		x5 = s
# movl <s=int32#2,>x5=stack32#15
# movl <s=%ecx,>x5=120(%esp)
movl %ecx,120(%esp)

# 				t += x9
# addl <x9=stack32#19,<t=int32#3
# addl <x9=136(%esp),<t=%edx
addl 136(%esp),%edx

# 						x15 = w
# movl <w=int32#4,>x15=stack32#25
# movl <w=%ebx,>x15=160(%esp)
movl %ebx,160(%esp)

# 		r = x4
# movl <x4=stack32#14,>r=int32#5
# movl <x4=116(%esp),>r=%esi
movl 116(%esp),%esi

# 		r += s
# add  <s=int32#2,<r=int32#5
# add  <s=%ecx,<r=%esi
add  %ecx,%esi

# 						v = x14
# movl <x14=stack32#24,>v=int32#6
# movl <x14=156(%esp),>v=%edi
movl 156(%esp),%edi

# 						v += w
# add  <w=int32#4,<v=int32#6
# add  <w=%ebx,<v=%edi
add  %ebx,%edi

# p ^= x1
# xorl <x1=stack32#11,<p=int32#1
# xorl <x1=104(%esp),<p=%eax
xorl 104(%esp),%eax

# 				t <<<= 7
# rol  $7,<t=int32#3
# rol  $7,<t=%edx
rol  $7,%edx

# 				t ^= x11
# xorl <x11=stack32#21,<t=int32#3
# xorl <x11=144(%esp),<t=%edx
xorl 144(%esp),%edx

# 		r <<<= 7
# rol  $7,<r=int32#5
# rol  $7,<r=%esi
rol  $7,%esi

# 		r ^= x6
# xorl <x6=stack32#16,<r=int32#5
# xorl <x6=124(%esp),<r=%esi
xorl 124(%esp),%esi

# 						v <<<= 7
# rol  $7,<v=int32#6
# rol  $7,<v=%edi
rol  $7,%edi

# 						v ^= x12
# xorl <x12=stack32#22,<v=int32#6
# xorl <x12=148(%esp),<v=%edi
xorl 148(%esp),%edi

# x1 = p
# movl <p=int32#1,>x1=stack32#11
# movl <p=%eax,>x1=104(%esp)
movl %eax,104(%esp)

# 				x11 = t
# movl <t=int32#3,>x11=stack32#21
# movl <t=%edx,>x11=144(%esp)
movl %edx,144(%esp)

# p += x0
# addl <x0=stack32#10,<p=int32#1
# addl <x0=100(%esp),<p=%eax
addl 100(%esp),%eax

# 		x6 = r
# movl <r=int32#5,>x6=stack32#16
# movl <r=%esi,>x6=124(%esp)
movl %esi,124(%esp)

# 				t += x10
# addl <x10=stack32#20,<t=int32#3
# addl <x10=140(%esp),<t=%edx
addl 140(%esp),%edx

# 						x12 = v
# movl <v=int32#6,>x12=stack32#22
# movl <v=%edi,>x12=148(%esp)
movl %edi,148(%esp)

# p <<<= 9
# rol  $9,<p=int32#1
# rol  $9,<p=%eax
rol  $9,%eax

# p ^= x2
# xorl <x2=stack32#12,<p=int32#1
# xorl <x2=108(%esp),<p=%eax
xorl 108(%esp),%eax

# 				t <<<= 9
# rol  $9,<t=int32#3
# rol  $9,<t=%edx
rol  $9,%edx

# 				t ^= x8
# xorl <x8=stack32#18,<t=int32#3
# xorl <x8=132(%esp),<t=%edx
xorl 132(%esp),%edx

# 		s += r
# add  <r=int32#5,<s=int32#2
# add  <r=%esi,<s=%ecx
add  %esi,%ecx

# 		s <<<= 9
# rol  $9,<s=int32#2
# rol  $9,<s=%ecx
rol  $9,%ecx

# 		s ^= x7
# xorl <x7=stack32#17,<s=int32#2
# xorl <x7=128(%esp),<s=%ecx
xorl 128(%esp),%ecx

# 						w += v
# add  <v=int32#6,<w=int32#4
# add  <v=%edi,<w=%ebx
add  %edi,%ebx

# 						w <<<= 9
# rol  $9,<w=int32#4
# rol  $9,<w=%ebx
rol  $9,%ebx

# 						w ^= x13
# xorl <x13=stack32#23,<w=int32#4
# xorl <x13=152(%esp),<w=%ebx
xorl 152(%esp),%ebx

# x2 = p
# movl <p=int32#1,>x2=stack32#12
# movl <p=%eax,>x2=108(%esp)
movl %eax,108(%esp)

# 				x8 = t
# movl <t=int32#3,>x8=stack32#18
# movl <t=%edx,>x8=132(%esp)
movl %edx,132(%esp)

# p += x1
# addl <x1=stack32#11,<p=int32#1
# addl <x1=104(%esp),<p=%eax
addl 104(%esp),%eax

# 		x7 = s
# movl <s=int32#2,>x7=stack32#17
# movl <s=%ecx,>x7=128(%esp)
movl %ecx,128(%esp)

# 				t += x11
# addl <x11=stack32#21,<t=int32#3
# addl <x11=144(%esp),<t=%edx
addl 144(%esp),%edx

# 						x13 = w
# movl <w=int32#4,>x13=stack32#23
# movl <w=%ebx,>x13=152(%esp)
movl %ebx,152(%esp)

# p <<<= 13
# rol  $13,<p=int32#1
# rol  $13,<p=%eax
rol  $13,%eax

# p ^= x3
# xorl <x3=stack32#13,<p=int32#1
# xorl <x3=112(%esp),<p=%eax
xorl 112(%esp),%eax

# 				t <<<= 13
# rol  $13,<t=int32#3
# rol  $13,<t=%edx
rol  $13,%edx

# 				t ^= x9
# xorl <x9=stack32#19,<t=int32#3
# xorl <x9=136(%esp),<t=%edx
xorl 136(%esp),%edx

# 		r += s
# add  <s=int32#2,<r=int32#5
# add  <s=%ecx,<r=%esi
add  %ecx,%esi

# 		r <<<= 13
# rol  $13,<r=int32#5
# rol  $13,<r=%esi
rol  $13,%esi

# 		r ^= x4
# xorl <x4=stack32#14,<r=int32#5
# xorl <x4=116(%esp),<r=%esi
xorl 116(%esp),%esi

# 						v += w
# add  <w=int32#4,<v=int32#6
# add  <w=%ebx,<v=%edi
add  %ebx,%edi

# 						v <<<= 13
# rol  $13,<v=int32#6
# rol  $13,<v=%edi
rol  $13,%edi

# 						v ^= x14
# xorl <x14=stack32#24,<v=int32#6
# xorl <x14=156(%esp),<v=%edi
xorl 156(%esp),%edi

# x3 = p
# movl <p=int32#1,>x3=stack32#13
# movl <p=%eax,>x3=112(%esp)
movl %eax,112(%esp)

# 				x9 = t
# movl <t=int32#3,>x9=stack32#19
# movl <t=%edx,>x9=136(%esp)
movl %edx,136(%esp)

# p += x2
# addl <x2=stack32#12,<p=int32#1
# addl <x2=108(%esp),<p=%eax
addl 108(%esp),%eax

# 		x4 = r
# movl <r=int32#5,>x4=stack32#14
# movl <r=%esi,>x4=116(%esp)
movl %esi,116(%esp)

# 				t += x8
# addl <x8=stack32#18,<t=int32#3
# addl <x8=132(%esp),<t=%edx
addl 132(%esp),%edx

# 						x14 = v
# movl <v=int32#6,>x14=stack32#24
# movl <v=%edi,>x14=156(%esp)
movl %edi,156(%esp)

# p <<<= 18
# rol  $18,<p=int32#1
# rol  $18,<p=%eax
rol  $18,%eax

# p ^= x0
# xorl <x0=stack32#10,<p=int32#1
# xorl <x0=100(%esp),<p=%eax
xorl 100(%esp),%eax

# 				t <<<= 18
# rol  $18,<t=int32#3
# rol  $18,<t=%edx
rol  $18,%edx

# 				t ^= x10
# xorl <x10=stack32#20,<t=int32#3
# xorl <x10=140(%esp),<t=%edx
xorl 140(%esp),%edx

# 		s += r
# add  <r=int32#5,<s=int32#2
# add  <r=%esi,<s=%ecx
add  %esi,%ecx

# 		s <<<= 18
# rol  $18,<s=int32#2
# rol  $18,<s=%ecx
rol  $18,%ecx

# 		s ^= x5
# xorl <x5=stack32#15,<s=int32#2
# xorl <x5=120(%esp),<s=%ecx
xorl 120(%esp),%ecx

# 						w += v
# add  <v=int32#6,<w=int32#4
# add  <v=%edi,<w=%ebx
add  %edi,%ebx

# 						w <<<= 18
# rol  $18,<w=int32#4
# rol  $18,<w=%ebx
rol  $18,%ebx

# 						w ^= x15
# xorl <x15=stack32#25,<w=int32#4
# xorl <x15=160(%esp),<w=%ebx
xorl 160(%esp),%ebx

# x0 = p
# movl <p=int32#1,>x0=stack32#10
# movl <p=%eax,>x0=100(%esp)
movl %eax,100(%esp)

# 				x10 = t
# movl <t=int32#3,>x10=stack32#20
# movl <t=%edx,>x10=140(%esp)
movl %edx,140(%esp)

# p += x12
# addl <x12=stack32#22,<p=int32#1
# addl <x12=148(%esp),<p=%eax
addl 148(%esp),%eax

# 		x5 = s
# movl <s=int32#2,>x5=stack32#15
# movl <s=%ecx,>x5=120(%esp)
movl %ecx,120(%esp)

# 				t += x6
# addl <x6=stack32#16,<t=int32#3
# addl <x6=124(%esp),<t=%edx
addl 124(%esp),%edx

# 						x15 = w
# movl <w=int32#4,>x15=stack32#25
# movl <w=%ebx,>x15=160(%esp)
movl %ebx,160(%esp)

# 		r = x1
# movl <x1=stack32#11,>r=int32#5
# movl <x1=104(%esp),>r=%esi
movl 104(%esp),%esi

# 		r += s
# add  <s=int32#2,<r=int32#5
# add  <s=%ecx,<r=%esi
add  %ecx,%esi

# 						v = x11
# movl <x11=stack32#21,>v=int32#6
# movl <x11=144(%esp),>v=%edi
movl 144(%esp),%edi

# 						v += w
# add  <w=int32#4,<v=int32#6
# add  <w=%ebx,<v=%edi
add  %ebx,%edi

# p <<<= 7
# rol  $7,<p=int32#1
# rol  $7,<p=%eax
rol  $7,%eax

# p ^= x4
# xorl <x4=stack32#14,<p=int32#1
# xorl <x4=116(%esp),<p=%eax
xorl 116(%esp),%eax

# 				t <<<= 7
# rol  $7,<t=int32#3
# rol  $7,<t=%edx
rol  $7,%edx

# 				t ^= x14
# xorl <x14=stack32#24,<t=int32#3
# xorl <x14=156(%esp),<t=%edx
xorl 156(%esp),%edx

# 		r <<<= 7
# rol  $7,<r=int32#5
# rol  $7,<r=%esi
rol  $7,%esi

# 		r ^= x9
# xorl <x9=stack32#19,<r=int32#5
# xorl <x9=136(%esp),<r=%esi
xorl 136(%esp),%esi

# 						v <<<= 7
# rol  $7,<v=int32#6
# rol  $7,<v=%edi
rol  $7,%edi

# 						v ^= x3
# xorl <x3=stack32#13,<v=int32#6
# xorl <x3=112(%esp),<v=%edi
xorl 112(%esp),%edi

# x4 = p
# movl <p=int32#1,>x4=stack32#14
# movl <p=%eax,>x4=116(%esp)
movl %eax,116(%esp)

# 				x14 = t
# movl <t=int32#3,>x14=stack32#24
# movl <t=%edx,>x14=156(%esp)
movl %edx,156(%esp)

# p += x0
# addl <x0=stack32#10,<p=int32#1
# addl <x0=100(%esp),<p=%eax
addl 100(%esp),%eax

# 		x9 = r
# movl <r=int32#5,>x9=stack32#19
# movl <r=%esi,>x9=136(%esp)
movl %esi,136(%esp)

# 				t += x10
# addl <x10=stack32#20,<t=int32#3
# addl <x10=140(%esp),<t=%edx
addl 140(%esp),%edx

# 						x3 = v
# movl <v=int32#6,>x3=stack32#13
# movl <v=%edi,>x3=112(%esp)
movl %edi,112(%esp)

# p <<<= 9
# rol  $9,<p=int32#1
# rol  $9,<p=%eax
rol  $9,%eax

# p ^= x8
# xorl <x8=stack32#18,<p=int32#1
# xorl <x8=132(%esp),<p=%eax
xorl 132(%esp),%eax

# 				t <<<= 9
# rol  $9,<t=int32#3
# rol  $9,<t=%edx
rol  $9,%edx

# 				t ^= x2
# xorl <x2=stack32#12,<t=int32#3
# xorl <x2=108(%esp),<t=%edx
xorl 108(%esp),%edx

# 		s += r
# add  <r=int32#5,<s=int32#2
# add  <r=%esi,<s=%ecx
add  %esi,%ecx

# 		s <<<= 9
# rol  $9,<s=int32#2
# rol  $9,<s=%ecx
rol  $9,%ecx

# 		s ^= x13
# xorl <x13=stack32#23,<s=int32#2
# xorl <x13=152(%esp),<s=%ecx
xorl 152(%esp),%ecx

# 						w += v
# add  <v=int32#6,<w=int32#4
# add  <v=%edi,<w=%ebx
add  %edi,%ebx

# 						w <<<= 9
# rol  $9,<w=int32#4
# rol  $9,<w=%ebx
rol  $9,%ebx

# 						w ^= x7
# xorl <x7=stack32#17,<w=int32#4
# xorl <x7=128(%esp),<w=%ebx
xorl 128(%esp),%ebx

# x8 = p
# movl <p=int32#1,>x8=stack32#18
# movl <p=%eax,>x8=132(%esp)
movl %eax,132(%esp)

# 				x2 = t
# movl <t=int32#3,>x2=stack32#12
# movl <t=%edx,>x2=108(%esp)
movl %edx,108(%esp)

# p += x4
# addl <x4=stack32#14,<p=int32#1
# addl <x4=116(%esp),<p=%eax
addl 116(%esp),%eax

# 		x13 = s
# movl <s=int32#2,>x13=stack32#23
# movl <s=%ecx,>x13=152(%esp)
movl %ecx,152(%esp)

# 				t += x14
# addl <x14=stack32#24,<t=int32#3
# addl <x14=156(%esp),<t=%edx
addl 156(%esp),%edx

# 						x7 = w
# movl <w=int32#4,>x7=stack32#17
# movl <w=%ebx,>x7=128(%esp)
movl %ebx,128(%esp)

# p <<<= 13
# rol  $13,<p=int32#1
# rol  $13,<p=%eax
rol  $13,%eax

# p ^= x12
# xorl <x12=stack32#22,<p=int32#1
# xorl <x12=148(%esp),<p=%eax
xorl 148(%esp),%eax

# 				t <<<= 13
# rol  $13,<t=int32#3
# rol  $13,<t=%edx
rol  $13,%edx

# 				t ^= x6
# xorl <x6=stack32#16,<t=int32#3
# xorl <x6=124(%esp),<t=%edx
xorl 124(%esp),%edx

# 		r += s
# add  <s=int32#2,<r=int32#5
# add  <s=%ecx,<r=%esi
add  %ecx,%esi

# 		r <<<= 13
# rol  $13,<r=int32#5
# rol  $13,<r=%esi
rol  $13,%esi

# 		r ^= x1
# xorl <x1=stack32#11,<r=int32#5
# xorl <x1=104(%esp),<r=%esi
xorl 104(%esp),%esi

# 						v += w
# add  <w=int32#4,<v=int32#6
# add  <w=%ebx,<v=%edi
add  %ebx,%edi

# 						v <<<= 13
# rol  $13,<v=int32#6
# rol  $13,<v=%edi
rol  $13,%edi

# 						v ^= x11
# xorl <x11=stack32#21,<v=int32#6
# xorl <x11=144(%esp),<v=%edi
xorl 144(%esp),%edi

# x12 = p
# movl <p=int32#1,>x12=stack32#22
# movl <p=%eax,>x12=148(%esp)
movl %eax,148(%esp)

# 				x6 = t
# movl <t=int32#3,>x6=stack32#16
# movl <t=%edx,>x6=124(%esp)
movl %edx,124(%esp)

# p += x8
# addl <x8=stack32#18,<p=int32#1
# addl <x8=132(%esp),<p=%eax
addl 132(%esp),%eax

# 		x1 = r
# movl <r=int32#5,>x1=stack32#11
# movl <r=%esi,>x1=104(%esp)
movl %esi,104(%esp)

# 				t += x2
# addl <x2=stack32#12,<t=int32#3
# addl <x2=108(%esp),<t=%edx
addl 108(%esp),%edx

# 						x11 = v
# movl <v=int32#6,>x11=stack32#21
# movl <v=%edi,>x11=144(%esp)
movl %edi,144(%esp)

# p <<<= 18
# rol  $18,<p=int32#1
# rol  $18,<p=%eax
rol  $18,%eax

# p ^= x0
# xorl <x0=stack32#10,<p=int32#1
# xorl <x0=100(%esp),<p=%eax
xorl 100(%esp),%eax

# 				t <<<= 18
# rol  $18,<t=int32#3
# rol  $18,<t=%edx
rol  $18,%edx

# 				t ^= x10
# xorl <x10=stack32#20,<t=int32#3
# xorl <x10=140(%esp),<t=%edx
xorl 140(%esp),%edx

# 		s += r
# add  <r=int32#5,<s=int32#2
# add  <r=%esi,<s=%ecx
add  %esi,%ecx

# 		s <<<= 18
# rol  $18,<s=int32#2
# rol  $18,<s=%ecx
rol  $18,%ecx

# 		s ^= x5
# xorl <x5=stack32#15,<s=int32#2
# xorl <x5=120(%esp),<s=%ecx
xorl 120(%esp),%ecx

# 						w += v
# add  <v=int32#6,<w=int32#4
# add  <v=%edi,<w=%ebx
add  %edi,%ebx

# 						w <<<= 18
# rol  $18,<w=int32#4
# rol  $18,<w=%ebx
rol  $18,%ebx

# 						w ^= x15
# xorl <x15=stack32#25,<w=int32#4
# xorl <x15=160(%esp),<w=%ebx
xorl 160(%esp),%ebx

# x0 = p
# movl <p=int32#1,>x0=stack32#10
# movl <p=%eax,>x0=100(%esp)
movl %eax,100(%esp)

# 				x10 = t
# movl <t=int32#3,>x10=stack32#20
# movl <t=%edx,>x10=140(%esp)
movl %edx,140(%esp)

# p += x3
# addl <x3=stack32#13,<p=int32#1
# addl <x3=112(%esp),<p=%eax
addl 112(%esp),%eax

# p <<<= 7
# rol  $7,<p=int32#1
# rol  $7,<p=%eax
rol  $7,%eax

# 		x5 = s
# movl <s=int32#2,>x5=stack32#15
# movl <s=%ecx,>x5=120(%esp)
movl %ecx,120(%esp)

# 				t += x9
# addl <x9=stack32#19,<t=int32#3
# addl <x9=136(%esp),<t=%edx
addl 136(%esp),%edx

# 						x15 = w
# movl <w=int32#4,>x15=stack32#25
# movl <w=%ebx,>x15=160(%esp)
movl %ebx,160(%esp)

# 		r = x4
# movl <x4=stack32#14,>r=int32#5
# movl <x4=116(%esp),>r=%esi
movl 116(%esp),%esi

# 		r += s
# add  <s=int32#2,<r=int32#5
# add  <s=%ecx,<r=%esi
add  %ecx,%esi

# 						v = x14
# movl <x14=stack32#24,>v=int32#6
# movl <x14=156(%esp),>v=%edi
movl 156(%esp),%edi

# 						v += w
# add  <w=int32#4,<v=int32#6
# add  <w=%ebx,<v=%edi
add  %ebx,%edi

# p ^= x1
# xorl <x1=stack32#11,<p=int32#1
# xorl <x1=104(%esp),<p=%eax
xorl 104(%esp),%eax

# 				t <<<= 7
# rol  $7,<t=int32#3
# rol  $7,<t=%edx
rol  $7,%edx

# 				t ^= x11
# xorl <x11=stack32#21,<t=int32#3
# xorl <x11=144(%esp),<t=%edx
xorl 144(%esp),%edx

# 		r <<<= 7
# rol  $7,<r=int32#5
# rol  $7,<r=%esi
rol  $7,%esi

# 		r ^= x6
# xorl <x6=stack32#16,<r=int32#5
# xorl <x6=124(%esp),<r=%esi
xorl 124(%esp),%esi

# 						v <<<= 7
# rol  $7,<v=int32#6
# rol  $7,<v=%edi
rol  $7,%edi

# 						v ^= x12
# xorl <x12=stack32#22,<v=int32#6
# xorl <x12=148(%esp),<v=%edi
xorl 148(%esp),%edi

# x1 = p
# movl <p=int32#1,>x1=stack32#11
# movl <p=%eax,>x1=104(%esp)
movl %eax,104(%esp)

# 				x11 = t
# movl <t=int32#3,>x11=stack32#21
# movl <t=%edx,>x11=144(%esp)
movl %edx,144(%esp)

# p += x0
# addl <x0=stack32#10,<p=int32#1
# addl <x0=100(%esp),<p=%eax
addl 100(%esp),%eax

# 		x6 = r
# movl <r=int32#5,>x6=stack32#16
# movl <r=%esi,>x6=124(%esp)
movl %esi,124(%esp)

# 				t += x10
# addl <x10=stack32#20,<t=int32#3
# addl <x10=140(%esp),<t=%edx
addl 140(%esp),%edx

# 						x12 = v
# movl <v=int32#6,>x12=stack32#22
# movl <v=%edi,>x12=148(%esp)
movl %edi,148(%esp)

# p <<<= 9
# rol  $9,<p=int32#1
# rol  $9,<p=%eax
rol  $9,%eax

# p ^= x2
# xorl <x2=stack32#12,<p=int32#1
# xorl <x2=108(%esp),<p=%eax
xorl 108(%esp),%eax

# 				t <<<= 9
# rol  $9,<t=int32#3
# rol  $9,<t=%edx
rol  $9,%edx

# 				t ^= x8
# xorl <x8=stack32#18,<t=int32#3
# xorl <x8=132(%esp),<t=%edx
xorl 132(%esp),%edx

# 		s += r
# add  <r=int32#5,<s=int32#2
# add  <r=%esi,<s=%ecx
add  %esi,%ecx

# 		s <<<= 9
# rol  $9,<s=int32#2
# rol  $9,<s=%ecx
rol  $9,%ecx

# 		s ^= x7
# xorl <x7=stack32#17,<s=int32#2
# xorl <x7=128(%esp),<s=%ecx
xorl 128(%esp),%ecx

# 						w += v
# add  <v=int32#6,<w=int32#4
# add  <v=%edi,<w=%ebx
add  %edi,%ebx

# 						w <<<= 9
# rol  $9,<w=int32#4
# rol  $9,<w=%ebx
rol  $9,%ebx

# 						w ^= x13
# xorl <x13=stack32#23,<w=int32#4
# xorl <x13=152(%esp),<w=%ebx
xorl 152(%esp),%ebx

# x2 = p
# movl <p=int32#1,>x2=stack32#12
# movl <p=%eax,>x2=108(%esp)
movl %eax,108(%esp)

# 				x8 = t
# movl <t=int32#3,>x8=stack32#18
# movl <t=%edx,>x8=132(%esp)
movl %edx,132(%esp)

# p += x1
# addl <x1=stack32#11,<p=int32#1
# addl <x1=104(%esp),<p=%eax
addl 104(%esp),%eax

# 		x7 = s
# movl <s=int32#2,>x7=stack32#17
# movl <s=%ecx,>x7=128(%esp)
movl %ecx,128(%esp)

# 				t += x11
# addl <x11=stack32#21,<t=int32#3
# addl <x11=144(%esp),<t=%edx
addl 144(%esp),%edx

# 						x13 = w
# movl <w=int32#4,>x13=stack32#23
# movl <w=%ebx,>x13=152(%esp)
movl %ebx,152(%esp)

# p <<<= 13
# rol  $13,<p=int32#1
# rol  $13,<p=%eax
rol  $13,%eax

# p ^= x3
# xorl <x3=stack32#13,<p=int32#1
# xorl <x3=112(%esp),<p=%eax
xorl 112(%esp),%eax

# 				t <<<= 13
# rol  $13,<t=int32#3
# rol  $13,<t=%edx
rol  $13,%edx

# 				t ^= x9
# xorl <x9=stack32#19,<t=int32#3
# xorl <x9=136(%esp),<t=%edx
xorl 136(%esp),%edx

# 		r += s
# add  <s=int32#2,<r=int32#5
# add  <s=%ecx,<r=%esi
add  %ecx,%esi

# 		r <<<= 13
# rol  $13,<r=int32#5
# rol  $13,<r=%esi
rol  $13,%esi

# 		r ^= x4
# xorl <x4=stack32#14,<r=int32#5
# xorl <x4=116(%esp),<r=%esi
xorl 116(%esp),%esi

# 						v += w
# add  <w=int32#4,<v=int32#6
# add  <w=%ebx,<v=%edi
add  %ebx,%edi

# 						v <<<= 13
# rol  $13,<v=int32#6
# rol  $13,<v=%edi
rol  $13,%edi

# 						v ^= x14
# xorl <x14=stack32#24,<v=int32#6
# xorl <x14=156(%esp),<v=%edi
xorl 156(%esp),%edi

# x3 = p
# movl <p=int32#1,>x3=stack32#13
# movl <p=%eax,>x3=112(%esp)
movl %eax,112(%esp)

# 				x9 = t
# movl <t=int32#3,>x9=stack32#19
# movl <t=%edx,>x9=136(%esp)
movl %edx,136(%esp)

# p += x2
# addl <x2=stack32#12,<p=int32#1
# addl <x2=108(%esp),<p=%eax
addl 108(%esp),%eax

# 		x4 = r
# movl <r=int32#5,>x4=stack32#14
# movl <r=%esi,>x4=116(%esp)
movl %esi,116(%esp)

# 				t += x8
# addl <x8=stack32#18,<t=int32#3
# addl <x8=132(%esp),<t=%edx
addl 132(%esp),%edx

# 						x14 = v
# movl <v=int32#6,>x14=stack32#24
# movl <v=%edi,>x14=156(%esp)
movl %edi,156(%esp)

# p <<<= 18
# rol  $18,<p=int32#1
# rol  $18,<p=%eax
rol  $18,%eax

# p ^= x0
# xorl <x0=stack32#10,<p=int32#1
# xorl <x0=100(%esp),<p=%eax
xorl 100(%esp),%eax

# 				t <<<= 18
# rol  $18,<t=int32#3
# rol  $18,<t=%edx
rol  $18,%edx

# 				t ^= x10
# xorl <x10=stack32#20,<t=int32#3
# xorl <x10=140(%esp),<t=%edx
xorl 140(%esp),%edx

# 		s += r
# add  <r=int32#5,<s=int32#2
# add  <r=%esi,<s=%ecx
add  %esi,%ecx

# 		s <<<= 18
# rol  $18,<s=int32#2
# rol  $18,<s=%ecx
rol  $18,%ecx

# 		s ^= x5
# xorl <x5=stack32#15,<s=int32#2
# xorl <x5=120(%esp),<s=%ecx
xorl 120(%esp),%ecx

# 						w += v
# add  <v=int32#6,<w=int32#4
# add  <v=%edi,<w=%ebx
add  %edi,%ebx

# 						w <<<= 18
# rol  $18,<w=int32#4
# rol  $18,<w=%ebx
rol  $18,%ebx

# 						w ^= x15
# xorl <x15=stack32#25,<w=int32#4
# xorl <x15=160(%esp),<w=%ebx
xorl 160(%esp),%ebx

# i -= 4
# sub  $4,<i=int32#7
# sub  $4,<i=%ebp
sub  $4,%ebp

# goto mainloop if unsigned >
ja ._mainloop

# x0 = p
# movl <p=int32#1,>x0=stack32#10
# movl <p=%eax,>x0=100(%esp)
movl %eax,100(%esp)

# x5 = s
# movl <s=int32#2,>x5=stack32#15
# movl <s=%ecx,>x5=120(%esp)
movl %ecx,120(%esp)

# x10 = t
# movl <t=int32#3,>x10=stack32#20
# movl <t=%edx,>x10=140(%esp)
movl %edx,140(%esp)

# x15 = w
# movl <w=int32#4,>x15=stack32#25
# movl <w=%ebx,>x15=160(%esp)
movl %ebx,160(%esp)

#   out = out_backup
# movl <out_backup=stack32#3,>out=int32#6
# movl <out_backup=72(%esp),>out=%edi
movl 72(%esp),%edi

#   m = m_backup
# movl <m_backup=stack32#2,>m=int32#5
# movl <m_backup=68(%esp),>m=%esi
movl 68(%esp),%esi

#   in0 = x0
# movl <x0=stack32#10,>in0=int32#1
# movl <x0=100(%esp),>in0=%eax
movl 100(%esp),%eax

#   in1 = x1
# movl <x1=stack32#11,>in1=int32#2
# movl <x1=104(%esp),>in1=%ecx
movl 104(%esp),%ecx

#   in0 += j0
# addl <j0=stack32#26,<in0=int32#1
# addl <j0=164(%esp),<in0=%eax
addl 164(%esp),%eax

#   in1 += j1
# addl <j1=stack32#27,<in1=int32#2
# addl <j1=168(%esp),<in1=%ecx
addl 168(%esp),%ecx

#   in0 ^= *(uint32 *) (m + 0)
# xorl 0(<m=int32#5),<in0=int32#1
# xorl 0(<m=%esi),<in0=%eax
xorl 0(%esi),%eax

#   in1 ^= *(uint32 *) (m + 4)
# xorl 4(<m=int32#5),<in1=int32#2
# xorl 4(<m=%esi),<in1=%ecx
xorl 4(%esi),%ecx

#   *(uint32 *) (out + 0) = in0
# movl <in0=int32#1,0(<out=int32#6)
# movl <in0=%eax,0(<out=%edi)
movl %eax,0(%edi)

#   *(uint32 *) (out + 4) = in1
# movl <in1=int32#2,4(<out=int32#6)
# movl <in1=%ecx,4(<out=%edi)
movl %ecx,4(%edi)

#   in2 = x2
# movl <x2=stack32#12,>in2=int32#1
# movl <x2=108(%esp),>in2=%eax
movl 108(%esp),%eax

#   in3 = x3
# movl <x3=stack32#13,>in3=int32#2
# movl <x3=112(%esp),>in3=%ecx
movl 112(%esp),%ecx

#   in2 += j2
# addl <j2=stack32#28,<in2=int32#1
# addl <j2=172(%esp),<in2=%eax
addl 172(%esp),%eax

#   in3 += j3
# addl <j3=stack32#29,<in3=int32#2
# addl <j3=176(%esp),<in3=%ecx
addl 176(%esp),%ecx

#   in2 ^= *(uint32 *) (m + 8)
# xorl 8(<m=int32#5),<in2=int32#1
# xorl 8(<m=%esi),<in2=%eax
xorl 8(%esi),%eax

#   in3 ^= *(uint32 *) (m + 12)
# xorl 12(<m=int32#5),<in3=int32#2
# xorl 12(<m=%esi),<in3=%ecx
xorl 12(%esi),%ecx

#   *(uint32 *) (out + 8) = in2
# movl <in2=int32#1,8(<out=int32#6)
# movl <in2=%eax,8(<out=%edi)
movl %eax,8(%edi)

#   *(uint32 *) (out + 12) = in3
# movl <in3=int32#2,12(<out=int32#6)
# movl <in3=%ecx,12(<out=%edi)
movl %ecx,12(%edi)

#   in4 = x4
# movl <x4=stack32#14,>in4=int32#1
# movl <x4=116(%esp),>in4=%eax
movl 116(%esp),%eax

#   in5 = x5
# movl <x5=stack32#15,>in5=int32#2
# movl <x5=120(%esp),>in5=%ecx
movl 120(%esp),%ecx

#   in4 += j4
# addl <j4=stack32#30,<in4=int32#1
# addl <j4=180(%esp),<in4=%eax
addl 180(%esp),%eax

#   in5 += j5
# addl <j5=stack32#31,<in5=int32#2
# addl <j5=184(%esp),<in5=%ecx
addl 184(%esp),%ecx

#   in4 ^= *(uint32 *) (m + 16)
# xorl 16(<m=int32#5),<in4=int32#1
# xorl 16(<m=%esi),<in4=%eax
xorl 16(%esi),%eax

#   in5 ^= *(uint32 *) (m + 20)
# xorl 20(<m=int32#5),<in5=int32#2
# xorl 20(<m=%esi),<in5=%ecx
xorl 20(%esi),%ecx

#   *(uint32 *) (out + 16) = in4
# movl <in4=int32#1,16(<out=int32#6)
# movl <in4=%eax,16(<out=%edi)
movl %eax,16(%edi)

#   *(uint32 *) (out + 20) = in5
# movl <in5=int32#2,20(<out=int32#6)
# movl <in5=%ecx,20(<out=%edi)
movl %ecx,20(%edi)

#   in6 = x6
# movl <x6=stack32#16,>in6=int32#1
# movl <x6=124(%esp),>in6=%eax
movl 124(%esp),%eax

#   in7 = x7
# movl <x7=stack32#17,>in7=int32#2
# movl <x7=128(%esp),>in7=%ecx
movl 128(%esp),%ecx

#   in6 += j6
# addl <j6=stack32#32,<in6=int32#1
# addl <j6=188(%esp),<in6=%eax
addl 188(%esp),%eax

#   in7 += j7
# addl <j7=stack32#33,<in7=int32#2
# addl <j7=192(%esp),<in7=%ecx
addl 192(%esp),%ecx

#   in6 ^= *(uint32 *) (m + 24)
# xorl 24(<m=int32#5),<in6=int32#1
# xorl 24(<m=%esi),<in6=%eax
xorl 24(%esi),%eax

#   in7 ^= *(uint32 *) (m + 28)
# xorl 28(<m=int32#5),<in7=int32#2
# xorl 28(<m=%esi),<in7=%ecx
xorl 28(%esi),%ecx

#   *(uint32 *) (out + 24) = in6
# movl <in6=int32#1,24(<out=int32#6)
# movl <in6=%eax,24(<out=%edi)
movl %eax,24(%edi)

#   *(uint32 *) (out + 28) = in7
# movl <in7=int32#2,28(<out=int32#6)
# movl <in7=%ecx,28(<out=%edi)
movl %ecx,28(%edi)

#   in8 = x8
# movl <x8=stack32#18,>in8=int32#1
# movl <x8=132(%esp),>in8=%eax
movl 132(%esp),%eax

#   in9 = x9
# movl <x9=stack32#19,>in9=int32#2
# movl <x9=136(%esp),>in9=%ecx
movl 136(%esp),%ecx

#   in8 += j8
# addl <j8=stack32#34,<in8=int32#1
# addl <j8=196(%esp),<in8=%eax
addl 196(%esp),%eax

#   in9 += j9
# addl <j9=stack32#35,<in9=int32#2
# addl <j9=200(%esp),<in9=%ecx
addl 200(%esp),%ecx

#   in8 ^= *(uint32 *) (m + 32)
# xorl 32(<m=int32#5),<in8=int32#1
# xorl 32(<m=%esi),<in8=%eax
xorl 32(%esi),%eax

#   in9 ^= *(uint32 *) (m + 36)
# xorl 36(<m=int32#5),<in9=int32#2
# xorl 36(<m=%esi),<in9=%ecx
xorl 36(%esi),%ecx

#   *(uint32 *) (out + 32) = in8
# movl <in8=int32#1,32(<out=int32#6)
# movl <in8=%eax,32(<out=%edi)
movl %eax,32(%edi)

#   *(uint32 *) (out + 36) = in9
# movl <in9=int32#2,36(<out=int32#6)
# movl <in9=%ecx,36(<out=%edi)
movl %ecx,36(%edi)

#   in10 = x10
# movl <x10=stack32#20,>in10=int32#1
# movl <x10=140(%esp),>in10=%eax
movl 140(%esp),%eax

#   in11 = x11
# movl <x11=stack32#21,>in11=int32#2
# movl <x11=144(%esp),>in11=%ecx
movl 144(%esp),%ecx

#   in10 += j10
# addl <j10=stack32#36,<in10=int32#1
# addl <j10=204(%esp),<in10=%eax
addl 204(%esp),%eax

#   in11 += j11
# addl <j11=stack32#37,<in11=int32#2
# addl <j11=208(%esp),<in11=%ecx
addl 208(%esp),%ecx

#   in10 ^= *(uint32 *) (m + 40)
# xorl 40(<m=int32#5),<in10=int32#1
# xorl 40(<m=%esi),<in10=%eax
xorl 40(%esi),%eax

#   in11 ^= *(uint32 *) (m + 44)
# xorl 44(<m=int32#5),<in11=int32#2
# xorl 44(<m=%esi),<in11=%ecx
xorl 44(%esi),%ecx

#   *(uint32 *) (out + 40) = in10
# movl <in10=int32#1,40(<out=int32#6)
# movl <in10=%eax,40(<out=%edi)
movl %eax,40(%edi)

#   *(uint32 *) (out + 44) = in11
# movl <in11=int32#2,44(<out=int32#6)
# movl <in11=%ecx,44(<out=%edi)
movl %ecx,44(%edi)

#   in12 = x12
# movl <x12=stack32#22,>in12=int32#1
# movl <x12=148(%esp),>in12=%eax
movl 148(%esp),%eax

#   in13 = x13
# movl <x13=stack32#23,>in13=int32#2
# movl <x13=152(%esp),>in13=%ecx
movl 152(%esp),%ecx

#   in12 += j12
# addl <j12=stack32#38,<in12=int32#1
# addl <j12=212(%esp),<in12=%eax
addl 212(%esp),%eax

#   in13 += j13
# addl <j13=stack32#39,<in13=int32#2
# addl <j13=216(%esp),<in13=%ecx
addl 216(%esp),%ecx

#   in12 ^= *(uint32 *) (m + 48)
# xorl 48(<m=int32#5),<in12=int32#1
# xorl 48(<m=%esi),<in12=%eax
xorl 48(%esi),%eax

#   in13 ^= *(uint32 *) (m + 52)
# xorl 52(<m=int32#5),<in13=int32#2
# xorl 52(<m=%esi),<in13=%ecx
xorl 52(%esi),%ecx

#   *(uint32 *) (out + 48) = in12
# movl <in12=int32#1,48(<out=int32#6)
# movl <in12=%eax,48(<out=%edi)
movl %eax,48(%edi)

#   *(uint32 *) (out + 52) = in13
# movl <in13=int32#2,52(<out=int32#6)
# movl <in13=%ecx,52(<out=%edi)
movl %ecx,52(%edi)

#   in14 = x14
# movl <x14=stack32#24,>in14=int32#1
# movl <x14=156(%esp),>in14=%eax
movl 156(%esp),%eax

#   in15 = x15
# movl <x15=stack32#25,>in15=int32#2
# movl <x15=160(%esp),>in15=%ecx
movl 160(%esp),%ecx

#   in14 += j14
# addl <j14=stack32#40,<in14=int32#1
# addl <j14=220(%esp),<in14=%eax
addl 220(%esp),%eax

#   in15 += j15
# addl <j15=stack32#41,<in15=int32#2
# addl <j15=224(%esp),<in15=%ecx
addl 224(%esp),%ecx

#   in14 ^= *(uint32 *) (m + 56)
# xorl 56(<m=int32#5),<in14=int32#1
# xorl 56(<m=%esi),<in14=%eax
xorl 56(%esi),%eax

#   in15 ^= *(uint32 *) (m + 60)
# xorl 60(<m=int32#5),<in15=int32#2
# xorl 60(<m=%esi),<in15=%ecx
xorl 60(%esi),%ecx

#   *(uint32 *) (out + 56) = in14
# movl <in14=int32#1,56(<out=int32#6)
# movl <in14=%eax,56(<out=%edi)
movl %eax,56(%edi)

#   *(uint32 *) (out + 60) = in15
# movl <in15=int32#2,60(<out=int32#6)
# movl <in15=%ecx,60(<out=%edi)
movl %ecx,60(%edi)

#   bytes = bytes_backup
# movl <bytes_backup=stack32#4,>bytes=int32#4
# movl <bytes_backup=76(%esp),>bytes=%ebx
movl 76(%esp),%ebx

#   in8 = j8
# movl <j8=stack32#34,>in8=int32#1
# movl <j8=196(%esp),>in8=%eax
movl 196(%esp),%eax

#   in9 = j9
# movl <j9=stack32#35,>in9=int32#2
# movl <j9=200(%esp),>in9=%ecx
movl 200(%esp),%ecx

#   in8 += 1
# add  $1,<in8=int32#1
# add  $1,<in8=%eax
add  $1,%eax

#   in9 += 0 + carry
# adc  $0,<in9=int32#2
# adc  $0,<in9=%ecx
adc  $0,%ecx

#   j8 = in8
# movl <in8=int32#1,>j8=stack32#34
# movl <in8=%eax,>j8=196(%esp)
movl %eax,196(%esp)

#   j9 = in9
# movl <in9=int32#2,>j9=stack32#35
# movl <in9=%ecx,>j9=200(%esp)
movl %ecx,200(%esp)

#   bytes - 64
# cmp  $64,<bytes=int32#4
# cmp  $64,<bytes=%ebx
cmp  $64,%ebx

#   goto bytesatleast65 if unsigned>
ja ._bytesatleast65

#     goto bytesatleast64 if unsigned>=
jae ._bytesatleast64

#       m = out
# mov  <out=int32#6,>m=int32#5
# mov  <out=%edi,>m=%esi
mov  %edi,%esi

#       out = ctarget
# movl <ctarget=stack32#42,>out=int32#6
# movl <ctarget=228(%esp),>out=%edi
movl 228(%esp),%edi

#       i = bytes
# mov  <bytes=int32#4,>i=int32#2
# mov  <bytes=%ebx,>i=%ecx
mov  %ebx,%ecx

#       while (i) { *out++ = *m++; --i }
rep movsb

#     bytesatleast64#
._bytesatleast64:

#     x = x_backup
# movl <x_backup=stack32#1,>x=int32#1
# movl <x_backup=64(%esp),>x=%eax
movl 64(%esp),%eax

#     in8 = j8
# movl <j8=stack32#34,>in8=int32#2
# movl <j8=196(%esp),>in8=%ecx
movl 196(%esp),%ecx

#     in9 = j9
# movl <j9=stack32#35,>in9=int32#3
# movl <j9=200(%esp),>in9=%edx
movl 200(%esp),%edx

#     *(uint32 *) (x + 32) = in8
# movl <in8=int32#2,32(<x=int32#1)
# movl <in8=%ecx,32(<x=%eax)
movl %ecx,32(%eax)

#     *(uint32 *) (x + 36) = in9
# movl <in9=int32#3,36(<x=int32#1)
# movl <in9=%edx,36(<x=%eax)
movl %edx,36(%eax)

#     done#
._done:

#     eax = eax_stack
# movl <eax_stack=stack32#5,>eax=int32#1
# movl <eax_stack=80(%esp),>eax=%eax
movl 80(%esp),%eax

#     ebx = ebx_stack
# movl <ebx_stack=stack32#6,>ebx=int32#4
# movl <ebx_stack=84(%esp),>ebx=%ebx
movl 84(%esp),%ebx

#     esi = esi_stack
# movl <esi_stack=stack32#7,>esi=int32#5
# movl <esi_stack=88(%esp),>esi=%esi
movl 88(%esp),%esi

#     edi = edi_stack
# movl <edi_stack=stack32#8,>edi=int32#6
# movl <edi_stack=92(%esp),>edi=%edi
movl 92(%esp),%edi

#     ebp = ebp_stack
# movl <ebp_stack=stack32#9,>ebp=int32#7
# movl <ebp_stack=96(%esp),>ebp=%ebp
movl 96(%esp),%ebp

#     leave
add %eax,%esp
ret

#   bytesatleast65#
._bytesatleast65:

#   bytes -= 64
# sub  $64,<bytes=int32#4
# sub  $64,<bytes=%ebx
sub  $64,%ebx

#   out += 64
# add  $64,<out=int32#6
# add  $64,<out=%edi
add  $64,%edi

#   m += 64
# add  $64,<m=int32#5
# add  $64,<m=%esi
add  $64,%esi

# goto bytesatleast1
jmp ._bytesatleast1

# enter ECRYPT_init
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

# leave
add %eax,%esp
ret

# enter ECRYPT_keysetup
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

#   eax_stack = eax
# movl <eax=int32#1,>eax_stack=stack32#1
# movl <eax=%eax,>eax_stack=64(%esp)
movl %eax,64(%esp)

#   ebx_stack = ebx
# movl <ebx=int32#4,>ebx_stack=stack32#2
# movl <ebx=%ebx,>ebx_stack=68(%esp)
movl %ebx,68(%esp)

#   esi_stack = esi
# movl <esi=int32#5,>esi_stack=stack32#3
# movl <esi=%esi,>esi_stack=72(%esp)
movl %esi,72(%esp)

#   edi_stack = edi
# movl <edi=int32#6,>edi_stack=stack32#4
# movl <edi=%edi,>edi_stack=76(%esp)
movl %edi,76(%esp)

#   ebp_stack = ebp
# movl <ebp=int32#7,>ebp_stack=stack32#5
# movl <ebp=%ebp,>ebp_stack=80(%esp)
movl %ebp,80(%esp)

#   k = arg2
# movl <arg2=stack32#-2,>k=int32#2
# movl <arg2=8(%esp,%eax),>k=%ecx
movl 8(%esp,%eax),%ecx

#   kbits = arg3
# movl <arg3=stack32#-3,>kbits=int32#3
# movl <arg3=12(%esp,%eax),>kbits=%edx
movl 12(%esp,%eax),%edx

#   x = arg1
# movl <arg1=stack32#-1,>x=int32#1
# movl <arg1=4(%esp,%eax),>x=%eax
movl 4(%esp,%eax),%eax

#   in1 = *(uint32 *) (k + 0)
# movl 0(<k=int32#2),>in1=int32#4
# movl 0(<k=%ecx),>in1=%ebx
movl 0(%ecx),%ebx

#   in2 = *(uint32 *) (k + 4)
# movl 4(<k=int32#2),>in2=int32#5
# movl 4(<k=%ecx),>in2=%esi
movl 4(%ecx),%esi

#   in3 = *(uint32 *) (k + 8)
# movl 8(<k=int32#2),>in3=int32#6
# movl 8(<k=%ecx),>in3=%edi
movl 8(%ecx),%edi

#   in4 = *(uint32 *) (k + 12)
# movl 12(<k=int32#2),>in4=int32#7
# movl 12(<k=%ecx),>in4=%ebp
movl 12(%ecx),%ebp

#   *(uint32 *) (x + 4) = in1
# movl <in1=int32#4,4(<x=int32#1)
# movl <in1=%ebx,4(<x=%eax)
movl %ebx,4(%eax)

#   *(uint32 *) (x + 8) = in2
# movl <in2=int32#5,8(<x=int32#1)
# movl <in2=%esi,8(<x=%eax)
movl %esi,8(%eax)

#   *(uint32 *) (x + 12) = in3
# movl <in3=int32#6,12(<x=int32#1)
# movl <in3=%edi,12(<x=%eax)
movl %edi,12(%eax)

#   *(uint32 *) (x + 16) = in4
# movl <in4=int32#7,16(<x=int32#1)
# movl <in4=%ebp,16(<x=%eax)
movl %ebp,16(%eax)

#   kbits - 256
# cmp  $256,<kbits=int32#3
# cmp  $256,<kbits=%edx
cmp  $256,%edx

#   goto kbits128 if unsigned<
jb ._kbits128

#   kbits256#
._kbits256:

#     in11 = *(uint32 *) (k + 16)
# movl 16(<k=int32#2),>in11=int32#3
# movl 16(<k=%ecx),>in11=%edx
movl 16(%ecx),%edx

#     in12 = *(uint32 *) (k + 20)
# movl 20(<k=int32#2),>in12=int32#4
# movl 20(<k=%ecx),>in12=%ebx
movl 20(%ecx),%ebx

#     in13 = *(uint32 *) (k + 24)
# movl 24(<k=int32#2),>in13=int32#5
# movl 24(<k=%ecx),>in13=%esi
movl 24(%ecx),%esi

#     in14 = *(uint32 *) (k + 28)
# movl 28(<k=int32#2),>in14=int32#2
# movl 28(<k=%ecx),>in14=%ecx
movl 28(%ecx),%ecx

#     *(uint32 *) (x + 44) = in11
# movl <in11=int32#3,44(<x=int32#1)
# movl <in11=%edx,44(<x=%eax)
movl %edx,44(%eax)

#     *(uint32 *) (x + 48) = in12
# movl <in12=int32#4,48(<x=int32#1)
# movl <in12=%ebx,48(<x=%eax)
movl %ebx,48(%eax)

#     *(uint32 *) (x + 52) = in13
# movl <in13=int32#5,52(<x=int32#1)
# movl <in13=%esi,52(<x=%eax)
movl %esi,52(%eax)

#     *(uint32 *) (x + 56) = in14
# movl <in14=int32#2,56(<x=int32#1)
# movl <in14=%ecx,56(<x=%eax)
movl %ecx,56(%eax)

#     in0 = 1634760805
# mov  $1634760805,>in0=int32#2
# mov  $1634760805,>in0=%ecx
mov  $1634760805,%ecx

#     in5 = 857760878
# mov  $857760878,>in5=int32#3
# mov  $857760878,>in5=%edx
mov  $857760878,%edx

#     in10 = 2036477234
# mov  $2036477234,>in10=int32#4
# mov  $2036477234,>in10=%ebx
mov  $2036477234,%ebx

#     in15 = 1797285236
# mov  $1797285236,>in15=int32#5
# mov  $1797285236,>in15=%esi
mov  $1797285236,%esi

#     *(uint32 *) (x + 0) = in0
# movl <in0=int32#2,0(<x=int32#1)
# movl <in0=%ecx,0(<x=%eax)
movl %ecx,0(%eax)

#     *(uint32 *) (x + 20) = in5
# movl <in5=int32#3,20(<x=int32#1)
# movl <in5=%edx,20(<x=%eax)
movl %edx,20(%eax)

#     *(uint32 *) (x + 40) = in10
# movl <in10=int32#4,40(<x=int32#1)
# movl <in10=%ebx,40(<x=%eax)
movl %ebx,40(%eax)

#     *(uint32 *) (x + 60) = in15
# movl <in15=int32#5,60(<x=int32#1)
# movl <in15=%esi,60(<x=%eax)
movl %esi,60(%eax)

#   goto keysetupdone
jmp ._keysetupdone

#   kbits128#
._kbits128:

#     in11 = *(uint32 *) (k + 0)
# movl 0(<k=int32#2),>in11=int32#3
# movl 0(<k=%ecx),>in11=%edx
movl 0(%ecx),%edx

#     in12 = *(uint32 *) (k + 4)
# movl 4(<k=int32#2),>in12=int32#4
# movl 4(<k=%ecx),>in12=%ebx
movl 4(%ecx),%ebx

#     in13 = *(uint32 *) (k + 8)
# movl 8(<k=int32#2),>in13=int32#5
# movl 8(<k=%ecx),>in13=%esi
movl 8(%ecx),%esi

#     in14 = *(uint32 *) (k + 12)
# movl 12(<k=int32#2),>in14=int32#2
# movl 12(<k=%ecx),>in14=%ecx
movl 12(%ecx),%ecx

#     *(uint32 *) (x + 44) = in11
# movl <in11=int32#3,44(<x=int32#1)
# movl <in11=%edx,44(<x=%eax)
movl %edx,44(%eax)

#     *(uint32 *) (x + 48) = in12
# movl <in12=int32#4,48(<x=int32#1)
# movl <in12=%ebx,48(<x=%eax)
movl %ebx,48(%eax)

#     *(uint32 *) (x + 52) = in13
# movl <in13=int32#5,52(<x=int32#1)
# movl <in13=%esi,52(<x=%eax)
movl %esi,52(%eax)

#     *(uint32 *) (x + 56) = in14
# movl <in14=int32#2,56(<x=int32#1)
# movl <in14=%ecx,56(<x=%eax)
movl %ecx,56(%eax)

#     in0 = 1634760805
# mov  $1634760805,>in0=int32#2
# mov  $1634760805,>in0=%ecx
mov  $1634760805,%ecx

#     in5 = 824206446
# mov  $824206446,>in5=int32#3
# mov  $824206446,>in5=%edx
mov  $824206446,%edx

#     in10 = 2036477238
# mov  $2036477238,>in10=int32#4
# mov  $2036477238,>in10=%ebx
mov  $2036477238,%ebx

#     in15 = 1797285236
# mov  $1797285236,>in15=int32#5
# mov  $1797285236,>in15=%esi
mov  $1797285236,%esi

#     *(uint32 *) (x + 0) = in0
# movl <in0=int32#2,0(<x=int32#1)
# movl <in0=%ecx,0(<x=%eax)
movl %ecx,0(%eax)

#     *(uint32 *) (x + 20) = in5
# movl <in5=int32#3,20(<x=int32#1)
# movl <in5=%edx,20(<x=%eax)
movl %edx,20(%eax)

#     *(uint32 *) (x + 40) = in10
# movl <in10=int32#4,40(<x=int32#1)
# movl <in10=%ebx,40(<x=%eax)
movl %ebx,40(%eax)

#     *(uint32 *) (x + 60) = in15
# movl <in15=int32#5,60(<x=int32#1)
# movl <in15=%esi,60(<x=%eax)
movl %esi,60(%eax)

#   keysetupdone#
._keysetupdone:

#   eax = eax_stack
# movl <eax_stack=stack32#1,>eax=int32#1
# movl <eax_stack=64(%esp),>eax=%eax
movl 64(%esp),%eax

#   ebx = ebx_stack
# movl <ebx_stack=stack32#2,>ebx=int32#4
# movl <ebx_stack=68(%esp),>ebx=%ebx
movl 68(%esp),%ebx

#   esi = esi_stack
# movl <esi_stack=stack32#3,>esi=int32#5
# movl <esi_stack=72(%esp),>esi=%esi
movl 72(%esp),%esi

#   edi = edi_stack
# movl <edi_stack=stack32#4,>edi=int32#6
# movl <edi_stack=76(%esp),>edi=%edi
movl 76(%esp),%edi

#   ebp = ebp_stack
# movl <ebp_stack=stack32#5,>ebp=int32#7
# movl <ebp_stack=80(%esp),>ebp=%ebp
movl 80(%esp),%ebp

# leave
add %eax,%esp
ret

# enter ECRYPT_ivsetup
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

#   eax_stack = eax
# movl <eax=int32#1,>eax_stack=stack32#1
# movl <eax=%eax,>eax_stack=64(%esp)
movl %eax,64(%esp)

#   ebx_stack = ebx
# movl <ebx=int32#4,>ebx_stack=stack32#2
# movl <ebx=%ebx,>ebx_stack=68(%esp)
movl %ebx,68(%esp)

#   esi_stack = esi
# movl <esi=int32#5,>esi_stack=stack32#3
# movl <esi=%esi,>esi_stack=72(%esp)
movl %esi,72(%esp)

#   edi_stack = edi
# movl <edi=int32#6,>edi_stack=stack32#4
# movl <edi=%edi,>edi_stack=76(%esp)
movl %edi,76(%esp)

#   ebp_stack = ebp
# movl <ebp=int32#7,>ebp_stack=stack32#5
# movl <ebp=%ebp,>ebp_stack=80(%esp)
movl %ebp,80(%esp)

#   iv = arg2
# movl <arg2=stack32#-2,>iv=int32#2
# movl <arg2=8(%esp,%eax),>iv=%ecx
movl 8(%esp,%eax),%ecx

#   x = arg1
# movl <arg1=stack32#-1,>x=int32#1
# movl <arg1=4(%esp,%eax),>x=%eax
movl 4(%esp,%eax),%eax

#   in6 = *(uint32 *) (iv + 0)
# movl 0(<iv=int32#2),>in6=int32#3
# movl 0(<iv=%ecx),>in6=%edx
movl 0(%ecx),%edx

#   in7 = *(uint32 *) (iv + 4)
# movl 4(<iv=int32#2),>in7=int32#2
# movl 4(<iv=%ecx),>in7=%ecx
movl 4(%ecx),%ecx

#   in8 = 0
# mov  $0,>in8=int32#4
# mov  $0,>in8=%ebx
mov  $0,%ebx

#   in9 = 0
# mov  $0,>in9=int32#5
# mov  $0,>in9=%esi
mov  $0,%esi

#   *(uint32 *) (x + 24) = in6
# movl <in6=int32#3,24(<x=int32#1)
# movl <in6=%edx,24(<x=%eax)
movl %edx,24(%eax)

#   *(uint32 *) (x + 28) = in7
# movl <in7=int32#2,28(<x=int32#1)
# movl <in7=%ecx,28(<x=%eax)
movl %ecx,28(%eax)

#   *(uint32 *) (x + 32) = in8
# movl <in8=int32#4,32(<x=int32#1)
# movl <in8=%ebx,32(<x=%eax)
movl %ebx,32(%eax)

#   *(uint32 *) (x + 36) = in9
# movl <in9=int32#5,36(<x=int32#1)
# movl <in9=%esi,36(<x=%eax)
movl %esi,36(%eax)

#   eax = eax_stack
# movl <eax_stack=stack32#1,>eax=int32#1
# movl <eax_stack=64(%esp),>eax=%eax
movl 64(%esp),%eax

#   ebx = ebx_stack
# movl <ebx_stack=stack32#2,>ebx=int32#4
# movl <ebx_stack=68(%esp),>ebx=%ebx
movl 68(%esp),%ebx

#   esi = esi_stack
# movl <esi_stack=stack32#3,>esi=int32#5
# movl <esi_stack=72(%esp),>esi=%esi
movl 72(%esp),%esi

#   edi = edi_stack
# movl <edi_stack=stack32#4,>edi=int32#6
# movl <edi_stack=76(%esp),>edi=%edi
movl 76(%esp),%edi

#   ebp = ebp_stack
# movl <ebp_stack=stack32#5,>ebp=int32#7
# movl <ebp_stack=80(%esp),>ebp=%ebp
movl 80(%esp),%ebp

# leave
add %eax,%esp
ret
