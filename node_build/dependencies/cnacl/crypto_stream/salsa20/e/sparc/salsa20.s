
# qhasm: int64 x

# qhasm: int64 arg2

# qhasm: int64 arg3

# qhasm: int64 arg4

# qhasm: input x

# qhasm: input arg2

# qhasm: input arg3

# qhasm: input arg4

# qhasm: int64 i

# qhasm: int64 a

# qhasm: int64 m

# qhasm: int64 out

# qhasm: int64 bytes

# qhasm: stack64 ctarget

# qhasm: stack512 tmp

# qhasm: stack64 bytes_stack

# qhasm: stack64 out_stack

# qhasm: stack64 m_stack

# qhasm: stack64 x_stack

# qhasm: int64 z0

# qhasm: int64 z1

# qhasm: int64 z2

# qhasm: int64 z3

# qhasm: int64 z4

# qhasm: int64 z5

# qhasm: int64 z6

# qhasm: int64 z7

# qhasm: int64 z8

# qhasm: int64 z9

# qhasm: int64 z10

# qhasm: int64 z11

# qhasm: int64 z12

# qhasm: int64 z13

# qhasm: int64 z14

# qhasm: int64 z15

# qhasm: int64 u0

# qhasm: int64 u1

# qhasm: int64 u2

# qhasm: int64 u3

# qhasm: int64 u4

# qhasm: int64 u5

# qhasm: int64 u6

# qhasm: int64 u7

# qhasm: int64 u8

# qhasm: int64 u9

# qhasm: int64 u10

# qhasm: int64 u11

# qhasm: int64 u12

# qhasm: int64 u13

# qhasm: int64 u14

# qhasm: int64 u15

# qhasm: int64 y0

# qhasm: int64 y1

# qhasm: int64 y2

# qhasm: int64 y3

# qhasm: int64 y4

# qhasm: int64 y5

# qhasm: int64 y6

# qhasm: int64 y7

# qhasm: int64 y8

# qhasm: int64 y9

# qhasm: int64 y10

# qhasm: int64 y11

# qhasm: int64 y12

# qhasm: int64 y13

# qhasm: int64 y14

# qhasm: int64 y15

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

# qhasm: int64 q0

# qhasm: int64 q1

# qhasm: int64 q2

# qhasm: int64 q3

# qhasm: int64 q4

# qhasm: int64 q5

# qhasm: int64 q6

# qhasm: int64 q7

# qhasm: int64 q8

# qhasm: int64 q9

# qhasm: int64 q10

# qhasm: int64 q11

# qhasm: int64 q12

# qhasm: int64 q13

# qhasm: int64 q14

# qhasm: int64 q15

# qhasm: int64 m0

# qhasm: int64 m1

# qhasm: int64 m2

# qhasm: int64 m3

# qhasm: int64 m4

# qhasm: int64 m5

# qhasm: int64 m6

# qhasm: int64 m7

# qhasm: int64 m8

# qhasm: int64 m9

# qhasm: int64 m10

# qhasm: int64 m11

# qhasm: int64 m12

# qhasm: int64 m13

# qhasm: int64 m14

# qhasm: int64 m15

# qhasm: enter ECRYPT_init
.section ".text"
.align 32
.global ECRYPT_init
ECRYPT_init:
save %sp,-288,%sp

# qhasm: leave
ret
restore

# qhasm: enter ECRYPT_ivsetup
.section ".text"
.align 32
.global ECRYPT_ivsetup
ECRYPT_ivsetup:
save %sp,-288,%sp

# qhasm:   x6 = *(uint32 *) (arg2 + 0)
# asm 1: lduw [<arg2=int64#2+0],>x6=int64#5
# asm 2: lduw [<arg2=%i1+0],>x6=%i4
lduw [%i1+0],%i4

# qhasm:   x8 = 0
# asm 1: add %g0,0,>x8=int64#6
# asm 2: add %g0,0,>x8=%i5
add %g0,0,%i5

# qhasm:   x7 = *(uint32 *) (arg2 + 4)
# asm 1: lduw [<arg2=int64#2+4],>x7=int64#2
# asm 2: lduw [<arg2=%i1+4],>x7=%i1
lduw [%i1+4],%i1

# qhasm:   x9 = 0
# asm 1: add %g0,0,>x9=int64#7
# asm 2: add %g0,0,>x9=%g1
add %g0,0,%g1

# qhasm:   x += 24
# asm 1: add <x=int64#1,24,>x=int64#1
# asm 2: add <x=%i0,24,>x=%i0
add %i0,24,%i0

# qhasm:   *(swapendian int32 *) x = x6
# asm 1: stwa <x6=int64#5,[<x=int64#1] 0x88
# asm 2: stwa <x6=%i4,[<x=%i0] 0x88
stwa %i4,[%i0] 0x88

# qhasm:   x += 4
# asm 1: add <x=int64#1,4,>x=int64#1
# asm 2: add <x=%i0,4,>x=%i0
add %i0,4,%i0

# qhasm:   *(swapendian int32 *) x = x7
# asm 1: stwa <x7=int64#2,[<x=int64#1] 0x88
# asm 2: stwa <x7=%i1,[<x=%i0] 0x88
stwa %i1,[%i0] 0x88

# qhasm:   x += 4
# asm 1: add <x=int64#1,4,>x=int64#1
# asm 2: add <x=%i0,4,>x=%i0
add %i0,4,%i0

# qhasm:   *(int32 *) (x + 0) = x8
# asm 1: stw <x8=int64#6,[<x=int64#1+0]
# asm 2: stw <x8=%i5,[<x=%i0+0]
stw %i5,[%i0+0]

# qhasm:   x += 4
# asm 1: add <x=int64#1,4,>x=int64#1
# asm 2: add <x=%i0,4,>x=%i0
add %i0,4,%i0

# qhasm:   *(int32 *) (x + 0) = x9
# asm 1: stw <x9=int64#7,[<x=int64#1+0]
# asm 2: stw <x9=%g1,[<x=%i0+0]
stw %g1,[%i0+0]

# qhasm: leave
ret
restore

# qhasm: enter ECRYPT_keysetup
.section ".text"
.align 32
.global ECRYPT_keysetup
ECRYPT_keysetup:
save %sp,-288,%sp

# qhasm:                  unsigned>? arg3 - 128
# asm 1: subcc <arg3=int64#3,128,%g0
# asm 2: subcc <arg3=%i2,128,%g0
subcc %i2,128,%g0

# qhasm: goto kbits256 if unsigned>
bgu,pt %xcc,._kbits256
nop

# qhasm: kbits128:
._kbits128:

# qhasm:   x1 = *(uint32 *) (arg2 + 0)
# asm 1: lduw [<arg2=int64#2+0],>x1=int64#3
# asm 2: lduw [<arg2=%i1+0],>x1=%i2
lduw [%i1+0],%i2

# qhasm:   x0 = 1634760805 & 0xfffffc00
# asm 1: sethi %lm(1634760805),>x0=int64#4
# asm 2: sethi %lm(1634760805),>x0=%i3
sethi %lm(1634760805),%i3

# qhasm:   x2 = *(uint32 *) (arg2 + 4)
# asm 1: lduw [<arg2=int64#2+4],>x2=int64#5
# asm 2: lduw [<arg2=%i1+4],>x2=%i4
lduw [%i1+4],%i4

# qhasm:   x5 = 824206446 & 0xfffffc00
# asm 1: sethi %lm(824206446),>x5=int64#6
# asm 2: sethi %lm(824206446),>x5=%i5
sethi %lm(824206446),%i5

# qhasm:   x3 = *(uint32 *) (arg2 + 8)
# asm 1: lduw [<arg2=int64#2+8],>x3=int64#7
# asm 2: lduw [<arg2=%i1+8],>x3=%g1
lduw [%i1+8],%g1

# qhasm:   x10 = 2036477238 & 0xfffffc00
# asm 1: sethi %lm(2036477238),>x10=int64#8
# asm 2: sethi %lm(2036477238),>x10=%g4
sethi %lm(2036477238),%g4

# qhasm:   x4 = *(uint32 *) (arg2 + 12)
# asm 1: lduw [<arg2=int64#2+12],>x4=int64#9
# asm 2: lduw [<arg2=%i1+12],>x4=%g5
lduw [%i1+12],%g5

# qhasm:   x15 = 1797285236 & 0xfffffc00
# asm 1: sethi %lm(1797285236),>x15=int64#10
# asm 2: sethi %lm(1797285236),>x15=%o0
sethi %lm(1797285236),%o0

# qhasm:   x11 = *(uint32 *) (arg2 + 0)
# asm 1: lduw [<arg2=int64#2+0],>x11=int64#11
# asm 2: lduw [<arg2=%i1+0],>x11=%o1
lduw [%i1+0],%o1

# qhasm:   x0 |= 1634760805 & 0x3ff
# asm 1: or <x0=int64#4,%lo(1634760805),>x0=int64#4
# asm 2: or <x0=%i3,%lo(1634760805),>x0=%i3
or %i3,%lo(1634760805),%i3

# qhasm:   x12 = *(uint32 *) (arg2 + 4)
# asm 1: lduw [<arg2=int64#2+4],>x12=int64#12
# asm 2: lduw [<arg2=%i1+4],>x12=%o2
lduw [%i1+4],%o2

# qhasm:   x5 |= 824206446 & 0x3ff
# asm 1: or <x5=int64#6,%lo(824206446),>x5=int64#6
# asm 2: or <x5=%i5,%lo(824206446),>x5=%i5
or %i5,%lo(824206446),%i5

# qhasm:   x13 = *(uint32 *) (arg2 + 8)
# asm 1: lduw [<arg2=int64#2+8],>x13=int64#13
# asm 2: lduw [<arg2=%i1+8],>x13=%o3
lduw [%i1+8],%o3

# qhasm:   x10 |= 2036477238 & 0x3ff
# asm 1: or <x10=int64#8,%lo(2036477238),>x10=int64#8
# asm 2: or <x10=%g4,%lo(2036477238),>x10=%g4
or %g4,%lo(2036477238),%g4

# qhasm:   x14 = *(uint32 *) (arg2 + 12)
# asm 1: lduw [<arg2=int64#2+12],>x14=int64#2
# asm 2: lduw [<arg2=%i1+12],>x14=%i1
lduw [%i1+12],%i1

# qhasm:   x15 |= 1797285236 & 0x3ff
# asm 1: or <x15=int64#10,%lo(1797285236),>x15=int64#10
# asm 2: or <x15=%o0,%lo(1797285236),>x15=%o0
or %o0,%lo(1797285236),%o0

# qhasm: goto storekey
b ._storekey
nop

# qhasm: kbits256:
._kbits256:

# qhasm:   x1 = *(uint32 *) (arg2 + 0)
# asm 1: lduw [<arg2=int64#2+0],>x1=int64#3
# asm 2: lduw [<arg2=%i1+0],>x1=%i2
lduw [%i1+0],%i2

# qhasm:   x0 = 1634760805 & 0xfffffc00
# asm 1: sethi %lm(1634760805),>x0=int64#4
# asm 2: sethi %lm(1634760805),>x0=%i3
sethi %lm(1634760805),%i3

# qhasm:   x2 = *(uint32 *) (arg2 + 4)
# asm 1: lduw [<arg2=int64#2+4],>x2=int64#5
# asm 2: lduw [<arg2=%i1+4],>x2=%i4
lduw [%i1+4],%i4

# qhasm:   x5 = 857760878 & 0xfffffc00
# asm 1: sethi %lm(857760878),>x5=int64#6
# asm 2: sethi %lm(857760878),>x5=%i5
sethi %lm(857760878),%i5

# qhasm:   x3 = *(uint32 *) (arg2 + 8)
# asm 1: lduw [<arg2=int64#2+8],>x3=int64#7
# asm 2: lduw [<arg2=%i1+8],>x3=%g1
lduw [%i1+8],%g1

# qhasm:   x10 = 2036477234 & 0xfffffc00
# asm 1: sethi %lm(2036477234),>x10=int64#8
# asm 2: sethi %lm(2036477234),>x10=%g4
sethi %lm(2036477234),%g4

# qhasm:   x4 = *(uint32 *) (arg2 + 12)
# asm 1: lduw [<arg2=int64#2+12],>x4=int64#9
# asm 2: lduw [<arg2=%i1+12],>x4=%g5
lduw [%i1+12],%g5

# qhasm:   x15 = 1797285236 & 0xfffffc00
# asm 1: sethi %lm(1797285236),>x15=int64#10
# asm 2: sethi %lm(1797285236),>x15=%o0
sethi %lm(1797285236),%o0

# qhasm:   x11 = *(uint32 *) (arg2 + 16)
# asm 1: lduw [<arg2=int64#2+16],>x11=int64#11
# asm 2: lduw [<arg2=%i1+16],>x11=%o1
lduw [%i1+16],%o1

# qhasm:   x0 |= 1634760805 & 0x3ff
# asm 1: or <x0=int64#4,%lo(1634760805),>x0=int64#4
# asm 2: or <x0=%i3,%lo(1634760805),>x0=%i3
or %i3,%lo(1634760805),%i3

# qhasm:   x12 = *(uint32 *) (arg2 + 20)
# asm 1: lduw [<arg2=int64#2+20],>x12=int64#12
# asm 2: lduw [<arg2=%i1+20],>x12=%o2
lduw [%i1+20],%o2

# qhasm:   x5 |= 857760878 & 0x3ff
# asm 1: or <x5=int64#6,%lo(857760878),>x5=int64#6
# asm 2: or <x5=%i5,%lo(857760878),>x5=%i5
or %i5,%lo(857760878),%i5

# qhasm:   x13 = *(uint32 *) (arg2 + 24)
# asm 1: lduw [<arg2=int64#2+24],>x13=int64#13
# asm 2: lduw [<arg2=%i1+24],>x13=%o3
lduw [%i1+24],%o3

# qhasm:   x10 |= 2036477234 & 0x3ff
# asm 1: or <x10=int64#8,%lo(2036477234),>x10=int64#8
# asm 2: or <x10=%g4,%lo(2036477234),>x10=%g4
or %g4,%lo(2036477234),%g4

# qhasm:   x14 = *(uint32 *) (arg2 + 28)
# asm 1: lduw [<arg2=int64#2+28],>x14=int64#2
# asm 2: lduw [<arg2=%i1+28],>x14=%i1
lduw [%i1+28],%i1

# qhasm:   x15 |= 1797285236 & 0x3ff
# asm 1: or <x15=int64#10,%lo(1797285236),>x15=int64#10
# asm 2: or <x15=%o0,%lo(1797285236),>x15=%o0
or %o0,%lo(1797285236),%o0

# qhasm: storekey:
._storekey:

# qhasm:   *(int32 *) (x + 0) = x0
# asm 1: stw <x0=int64#4,[<x=int64#1+0]
# asm 2: stw <x0=%i3,[<x=%i0+0]
stw %i3,[%i0+0]

# qhasm:   x += 4
# asm 1: add <x=int64#1,4,>x=int64#1
# asm 2: add <x=%i0,4,>x=%i0
add %i0,4,%i0

# qhasm:   *(swapendian int32 *) x = x1
# asm 1: stwa <x1=int64#3,[<x=int64#1] 0x88
# asm 2: stwa <x1=%i2,[<x=%i0] 0x88
stwa %i2,[%i0] 0x88

# qhasm:   x += 4
# asm 1: add <x=int64#1,4,>x=int64#1
# asm 2: add <x=%i0,4,>x=%i0
add %i0,4,%i0

# qhasm:   *(swapendian int32 *) x = x2
# asm 1: stwa <x2=int64#5,[<x=int64#1] 0x88
# asm 2: stwa <x2=%i4,[<x=%i0] 0x88
stwa %i4,[%i0] 0x88

# qhasm:   x += 4
# asm 1: add <x=int64#1,4,>x=int64#1
# asm 2: add <x=%i0,4,>x=%i0
add %i0,4,%i0

# qhasm:   *(swapendian int32 *) x = x3
# asm 1: stwa <x3=int64#7,[<x=int64#1] 0x88
# asm 2: stwa <x3=%g1,[<x=%i0] 0x88
stwa %g1,[%i0] 0x88

# qhasm:   x += 4
# asm 1: add <x=int64#1,4,>x=int64#1
# asm 2: add <x=%i0,4,>x=%i0
add %i0,4,%i0

# qhasm:   *(swapendian int32 *) x = x4
# asm 1: stwa <x4=int64#9,[<x=int64#1] 0x88
# asm 2: stwa <x4=%g5,[<x=%i0] 0x88
stwa %g5,[%i0] 0x88

# qhasm:   x += 4
# asm 1: add <x=int64#1,4,>x=int64#1
# asm 2: add <x=%i0,4,>x=%i0
add %i0,4,%i0

# qhasm:   *(int32 *) (x + 0) = x5
# asm 1: stw <x5=int64#6,[<x=int64#1+0]
# asm 2: stw <x5=%i5,[<x=%i0+0]
stw %i5,[%i0+0]

# qhasm:   x += 20
# asm 1: add <x=int64#1,20,>x=int64#1
# asm 2: add <x=%i0,20,>x=%i0
add %i0,20,%i0

# qhasm:   *(int32 *) (x + 0) = x10
# asm 1: stw <x10=int64#8,[<x=int64#1+0]
# asm 2: stw <x10=%g4,[<x=%i0+0]
stw %g4,[%i0+0]

# qhasm:   x += 4
# asm 1: add <x=int64#1,4,>x=int64#1
# asm 2: add <x=%i0,4,>x=%i0
add %i0,4,%i0

# qhasm:   *(swapendian int32 *) x = x11
# asm 1: stwa <x11=int64#11,[<x=int64#1] 0x88
# asm 2: stwa <x11=%o1,[<x=%i0] 0x88
stwa %o1,[%i0] 0x88

# qhasm:   x += 4
# asm 1: add <x=int64#1,4,>x=int64#1
# asm 2: add <x=%i0,4,>x=%i0
add %i0,4,%i0

# qhasm:   *(swapendian int32 *) x = x12
# asm 1: stwa <x12=int64#12,[<x=int64#1] 0x88
# asm 2: stwa <x12=%o2,[<x=%i0] 0x88
stwa %o2,[%i0] 0x88

# qhasm:   x += 4
# asm 1: add <x=int64#1,4,>x=int64#1
# asm 2: add <x=%i0,4,>x=%i0
add %i0,4,%i0

# qhasm:   *(swapendian int32 *) x = x13
# asm 1: stwa <x13=int64#13,[<x=int64#1] 0x88
# asm 2: stwa <x13=%o3,[<x=%i0] 0x88
stwa %o3,[%i0] 0x88

# qhasm:   x += 4
# asm 1: add <x=int64#1,4,>x=int64#1
# asm 2: add <x=%i0,4,>x=%i0
add %i0,4,%i0

# qhasm:   *(swapendian int32 *) x = x14
# asm 1: stwa <x14=int64#2,[<x=int64#1] 0x88
# asm 2: stwa <x14=%i1,[<x=%i0] 0x88
stwa %i1,[%i0] 0x88

# qhasm:   x += 4
# asm 1: add <x=int64#1,4,>x=int64#1
# asm 2: add <x=%i0,4,>x=%i0
add %i0,4,%i0

# qhasm:   *(int32 *) (x + 0) = x15
# asm 1: stw <x15=int64#10,[<x=int64#1+0]
# asm 2: stw <x15=%o0,[<x=%i0+0]
stw %o0,[%i0+0]

# qhasm: leave
ret
restore

# qhasm: enter ECRYPT_keystream_bytes
.section ".text"
.align 32
.global ECRYPT_keystream_bytes
ECRYPT_keystream_bytes:
save %sp,-288,%sp

# qhasm: bytes = arg3
# asm 1: add %g0,<arg3=int64#3,>bytes=int64#5
# asm 2: add %g0,<arg3=%i2,>bytes=%i4
add %g0,%i2,%i4

# qhasm: m = arg2
# asm 1: add %g0,<arg2=int64#2,>m=int64#6
# asm 2: add %g0,<arg2=%i1,>m=%i5
add %g0,%i1,%i5

# qhasm: out = arg2
# asm 1: add %g0,<arg2=int64#2,>out=int64#2
# asm 2: add %g0,<arg2=%i1,>out=%i1
add %g0,%i1,%i1

# qhasm:               unsigned>? bytes - 0
# asm 1: subcc <bytes=int64#5,0,%g0
# asm 2: subcc <bytes=%i4,0,%g0
subcc %i4,0,%g0

# qhasm: goto done if !unsigned>
bleu,pt %xcc,._done
nop

# qhasm:   a = 0
# asm 1: add %g0,0,>a=int64#3
# asm 2: add %g0,0,>a=%i2
add %g0,0,%i2

# qhasm:   i = bytes
# asm 1: add %g0,<bytes=int64#5,>i=int64#4
# asm 2: add %g0,<bytes=%i4,>i=%i3
add %g0,%i4,%i3

# qhasm:   zeroloop:
._zeroloop:

# qhasm:     *(int8 *) (out + 0) = a
# asm 1: stb <a=int64#3,[<out=int64#2+0]
# asm 2: stb <a=%i2,[<out=%i1+0]
stb %i2,[%i1+0]

# qhasm:     out += 1
# asm 1: add <out=int64#2,1,>out=int64#2
# asm 2: add <out=%i1,1,>out=%i1
add %i1,1,%i1

# qhasm:                    unsigned>? i -= 1
# asm 1: subcc <i=int64#4,1,>i=int64#4
# asm 2: subcc <i=%i3,1,>i=%i3
subcc %i3,1,%i3

# qhasm:   goto zeroloop if unsigned>
bgu,pt %xcc,._zeroloop
nop

# qhasm:   out -= bytes
# asm 1: sub <out=int64#2,<bytes=int64#5,>out=int64#2
# asm 2: sub <out=%i1,<bytes=%i4,>out=%i1
sub %i1,%i4,%i1

# qhasm: goto bytesatleast1
b ._bytesatleast1
nop

# qhasm: enter ECRYPT_decrypt_bytes
.section ".text"
.align 32
.global ECRYPT_decrypt_bytes
ECRYPT_decrypt_bytes:
save %sp,-288,%sp

# qhasm: bytes = arg4
# asm 1: add %g0,<arg4=int64#4,>bytes=int64#5
# asm 2: add %g0,<arg4=%i3,>bytes=%i4
add %g0,%i3,%i4

# qhasm: m = arg2
# asm 1: add %g0,<arg2=int64#2,>m=int64#6
# asm 2: add %g0,<arg2=%i1,>m=%i5
add %g0,%i1,%i5

# qhasm: out = arg3
# asm 1: add %g0,<arg3=int64#3,>out=int64#2
# asm 2: add %g0,<arg3=%i2,>out=%i1
add %g0,%i2,%i1

# qhasm:               unsigned>? bytes - 0
# asm 1: subcc <bytes=int64#5,0,%g0
# asm 2: subcc <bytes=%i4,0,%g0
subcc %i4,0,%g0

# qhasm: goto done if !unsigned>
bleu,pt %xcc,._done
nop

# qhasm: goto bytesatleast1
b ._bytesatleast1
nop

# qhasm: enter ECRYPT_encrypt_bytes
.section ".text"
.align 32
.global ECRYPT_encrypt_bytes
ECRYPT_encrypt_bytes:
save %sp,-288,%sp

# qhasm: bytes = arg4
# asm 1: add %g0,<arg4=int64#4,>bytes=int64#5
# asm 2: add %g0,<arg4=%i3,>bytes=%i4
add %g0,%i3,%i4

# qhasm: m = arg2
# asm 1: add %g0,<arg2=int64#2,>m=int64#6
# asm 2: add %g0,<arg2=%i1,>m=%i5
add %g0,%i1,%i5

# qhasm: out = arg3
# asm 1: add %g0,<arg3=int64#3,>out=int64#2
# asm 2: add %g0,<arg3=%i2,>out=%i1
add %g0,%i2,%i1

# qhasm:               unsigned>? bytes - 0
# asm 1: subcc <bytes=int64#5,0,%g0
# asm 2: subcc <bytes=%i4,0,%g0
subcc %i4,0,%g0

# qhasm: goto done if !unsigned>
bleu,pt %xcc,._done
nop

# qhasm: bytesatleast1:
._bytesatleast1:

# qhasm:                           unsigned<? bytes - 64
# asm 1: subcc <bytes=int64#5,64,%g0
# asm 2: subcc <bytes=%i4,64,%g0
subcc %i4,64,%g0

# qhasm:   goto bytesatleast64 if !unsigned<
bgeu,pt %xcc,._bytesatleast64
nop

# qhasm:     ctarget = out
# asm 1: stx <out=int64#2,[%fp+2023->ctarget=stack64#1]
# asm 2: stx <out=%i1,[%fp+2023->ctarget=0]
stx %i1,[%fp+2023-0]

# qhasm:     out = &tmp
# asm 1: add %fp,1967->tmp=stack512#1,>out=int64#2
# asm 2: add %fp,1967->tmp=48,>out=%i1
add %fp,1967-48,%i1

# qhasm:     i = 0
# asm 1: add %g0,0,>i=int64#3
# asm 2: add %g0,0,>i=%i2
add %g0,0,%i2

# qhasm:     mcopyloop:
._mcopyloop:

# qhasm:       a = *(int8 *) (m + i)
# asm 1: ldsb [<m=int64#6+<i=int64#3],>a=int64#4
# asm 2: ldsb [<m=%i5+<i=%i2],>a=%i3
ldsb [%i5+%i2],%i3

# qhasm:       *(int8 *) (out + i) = a
# asm 1: stb <a=int64#4,[<out=int64#2+<i=int64#3]
# asm 2: stb <a=%i3,[<out=%i1+<i=%i2]
stb %i3,[%i1+%i2]

# qhasm:       i += 1
# asm 1: add <i=int64#3,1,>i=int64#3
# asm 2: add <i=%i2,1,>i=%i2
add %i2,1,%i2

# qhasm:                       unsigned<? i - bytes
# asm 1: subcc <i=int64#3,<bytes=int64#5,%g0
# asm 2: subcc <i=%i2,<bytes=%i4,%g0
subcc %i2,%i4,%g0

# qhasm:     goto mcopyloop if unsigned<
blu,pt %xcc,._mcopyloop
nop

# qhasm:     m = &tmp
# asm 1: add %fp,1967->tmp=stack512#1,>m=int64#6
# asm 2: add %fp,1967->tmp=48,>m=%i5
add %fp,1967-48,%i5

# qhasm:   bytesatleast64:
._bytesatleast64:

# qhasm:     x0 = *(uint32 *) (x + 0)
# asm 1: lduw [<x=int64#1+0],>x0=int64#3
# asm 2: lduw [<x=%i0+0],>x0=%i2
lduw [%i0+0],%i2

# qhasm:     x1 = *(uint32 *) (x + 4)
# asm 1: lduw [<x=int64#1+4],>x1=int64#4
# asm 2: lduw [<x=%i0+4],>x1=%i3
lduw [%i0+4],%i3

# qhasm:     x2 = *(uint32 *) (x + 8)
# asm 1: lduw [<x=int64#1+8],>x2=int64#7
# asm 2: lduw [<x=%i0+8],>x2=%g1
lduw [%i0+8],%g1

# qhasm:     x3 = *(uint32 *) (x + 12)
# asm 1: lduw [<x=int64#1+12],>x3=int64#8
# asm 2: lduw [<x=%i0+12],>x3=%g4
lduw [%i0+12],%g4

# qhasm:     x4 = *(uint32 *) (x + 16)
# asm 1: lduw [<x=int64#1+16],>x4=int64#9
# asm 2: lduw [<x=%i0+16],>x4=%g5
lduw [%i0+16],%g5

# qhasm:     x5 = *(uint32 *) (x + 20)
# asm 1: lduw [<x=int64#1+20],>x5=int64#10
# asm 2: lduw [<x=%i0+20],>x5=%o0
lduw [%i0+20],%o0

# qhasm:     x6 = *(uint32 *) (x + 24)
# asm 1: lduw [<x=int64#1+24],>x6=int64#11
# asm 2: lduw [<x=%i0+24],>x6=%o1
lduw [%i0+24],%o1

# qhasm:     x7 = *(uint32 *) (x + 28)
# asm 1: lduw [<x=int64#1+28],>x7=int64#12
# asm 2: lduw [<x=%i0+28],>x7=%o2
lduw [%i0+28],%o2

# qhasm:     x8 = *(uint32 *) (x + 32)
# asm 1: lduw [<x=int64#1+32],>x8=int64#13
# asm 2: lduw [<x=%i0+32],>x8=%o3
lduw [%i0+32],%o3

# qhasm:     x9 = *(uint32 *) (x + 36)
# asm 1: lduw [<x=int64#1+36],>x9=int64#14
# asm 2: lduw [<x=%i0+36],>x9=%o4
lduw [%i0+36],%o4

# qhasm:     x10 = *(uint32 *) (x + 40)
# asm 1: lduw [<x=int64#1+40],>x10=int64#15
# asm 2: lduw [<x=%i0+40],>x10=%o5
lduw [%i0+40],%o5

# qhasm:     x11 = *(uint32 *) (x + 44)
# asm 1: lduw [<x=int64#1+44],>x11=int64#16
# asm 2: lduw [<x=%i0+44],>x11=%o7
lduw [%i0+44],%o7

# qhasm:     x12 = *(uint32 *) (x + 48)
# asm 1: lduw [<x=int64#1+48],>x12=int64#17
# asm 2: lduw [<x=%i0+48],>x12=%l0
lduw [%i0+48],%l0

# qhasm:     x13 = *(uint32 *) (x + 52)
# asm 1: lduw [<x=int64#1+52],>x13=int64#18
# asm 2: lduw [<x=%i0+52],>x13=%l1
lduw [%i0+52],%l1

# qhasm:     x14 = *(uint32 *) (x + 56)
# asm 1: lduw [<x=int64#1+56],>x14=int64#19
# asm 2: lduw [<x=%i0+56],>x14=%l2
lduw [%i0+56],%l2

# qhasm:     x15 = *(uint32 *) (x + 60)
# asm 1: lduw [<x=int64#1+60],>x15=int64#20
# asm 2: lduw [<x=%i0+60],>x15=%l3
lduw [%i0+60],%l3

# qhasm:     i = 20
# asm 1: add %g0,20,>i=int64#21
# asm 2: add %g0,20,>i=%l4
add %g0,20,%l4

# qhasm:     bytes_stack = bytes
# asm 1: stx <bytes=int64#5,[%fp+2023->bytes_stack=stack64#2]
# asm 2: stx <bytes=%i4,[%fp+2023->bytes_stack=8]
stx %i4,[%fp+2023-8]

# qhasm:     out_stack = out
# asm 1: stx <out=int64#2,[%fp+2023->out_stack=stack64#3]
# asm 2: stx <out=%i1,[%fp+2023->out_stack=16]
stx %i1,[%fp+2023-16]

# qhasm:     m_stack = m
# asm 1: stx <m=int64#6,[%fp+2023->m_stack=stack64#4]
# asm 2: stx <m=%i5,[%fp+2023->m_stack=24]
stx %i5,[%fp+2023-24]

# qhasm:     x_stack = x
# asm 1: stx <x=int64#1,[%fp+2023->x_stack=stack64#5]
# asm 2: stx <x=%i0,[%fp+2023->x_stack=32]
stx %i0,[%fp+2023-32]

# qhasm:     mainloop:
._mainloop:

# qhasm: y4 = x0 + x12
# asm 1: add <x0=int64#3,<x12=int64#17,>y4=int64#1
# asm 2: add <x0=%i2,<x12=%l0,>y4=%i0
add %i2,%l0,%i0

# qhasm: 		y9 = x5 + x1
# asm 1: add <x5=int64#10,<x1=int64#4,>y9=int64#2
# asm 2: add <x5=%o0,<x1=%i3,>y9=%i1
add %o0,%i3,%i1

# qhasm: z4 = (uint32) y4 << 7
# asm 1: sll <y4=int64#1,7,>z4=int64#5
# asm 2: sll <y4=%i0,7,>z4=%i4
sll %i0,7,%i4

# qhasm: 				y14 = x10 + x6
# asm 1: add <x10=int64#15,<x6=int64#11,>y14=int64#6
# asm 2: add <x10=%o5,<x6=%o1,>y14=%i5
add %o5,%o1,%i5

# qhasm: y4 = (uint32) y4 >> 25
# asm 1: srl <y4=int64#1,25,>y4=int64#1
# asm 2: srl <y4=%i0,25,>y4=%i0
srl %i0,25,%i0

# qhasm: 						y3 = x15 + x11
# asm 1: add <x15=int64#20,<x11=int64#16,>y3=int64#22
# asm 2: add <x15=%l3,<x11=%o7,>y3=%l5
add %l3,%o7,%l5

# qhasm: 		z9 = (uint32) y9 << 7
# asm 1: sll <y9=int64#2,7,>z9=int64#23
# asm 2: sll <y9=%i1,7,>z9=%l6
sll %i1,7,%l6

# qhasm: y4 |= z4
# asm 1: or  <y4=int64#1,<z4=int64#5,>y4=int64#1
# asm 2: or  <y4=%i0,<z4=%i4,>y4=%i0
or  %i0,%i4,%i0

# qhasm: 		y9 = (uint32) y9 >> 25
# asm 1: srl <y9=int64#2,25,>y9=int64#2
# asm 2: srl <y9=%i1,25,>y9=%i1
srl %i1,25,%i1

# qhasm: x4 ^= y4
# asm 1: xor <x4=int64#9,<y4=int64#1,>x4=int64#1
# asm 2: xor <x4=%g5,<y4=%i0,>x4=%i0
xor %g5,%i0,%i0

# qhasm: 				z14 = (uint32) y14 << 7
# asm 1: sll <y14=int64#6,7,>z14=int64#5
# asm 2: sll <y14=%i5,7,>z14=%i4
sll %i5,7,%i4

# qhasm: 		y9 |= z9
# asm 1: or  <y9=int64#2,<z9=int64#23,>y9=int64#2
# asm 2: or  <y9=%i1,<z9=%l6,>y9=%i1
or  %i1,%l6,%i1

# qhasm: 				y14 = (uint32) y14 >> 25
# asm 1: srl <y14=int64#6,25,>y14=int64#6
# asm 2: srl <y14=%i5,25,>y14=%i5
srl %i5,25,%i5

# qhasm: y8 = x4 + x0
# asm 1: add <x4=int64#1,<x0=int64#3,>y8=int64#9
# asm 2: add <x4=%i0,<x0=%i2,>y8=%g5
add %i0,%i2,%g5

# qhasm: 						z3 = (uint32) y3 << 7
# asm 1: sll <y3=int64#22,7,>z3=int64#23
# asm 2: sll <y3=%l5,7,>z3=%l6
sll %l5,7,%l6

# qhasm: 		x9 ^= y9
# asm 1: xor <x9=int64#14,<y9=int64#2,>x9=int64#2
# asm 2: xor <x9=%o4,<y9=%i1,>x9=%i1
xor %o4,%i1,%i1

# qhasm: 						y3 = (uint32) y3 >> 25
# asm 1: srl <y3=int64#22,25,>y3=int64#14
# asm 2: srl <y3=%l5,25,>y3=%o4
srl %l5,25,%o4

# qhasm: 				y14 |= z14
# asm 1: or  <y14=int64#6,<z14=int64#5,>y14=int64#5
# asm 2: or  <y14=%i5,<z14=%i4,>y14=%i4
or  %i5,%i4,%i4

# qhasm: z8 = (uint32) y8 << 9
# asm 1: sll <y8=int64#9,9,>z8=int64#6
# asm 2: sll <y8=%g5,9,>z8=%i5
sll %g5,9,%i5

# qhasm: 		y13 = x9 + x5
# asm 1: add <x9=int64#2,<x5=int64#10,>y13=int64#22
# asm 2: add <x9=%i1,<x5=%o0,>y13=%l5
add %i1,%o0,%l5

# qhasm: y8 = (uint32) y8 >> 23
# asm 1: srl <y8=int64#9,23,>y8=int64#9
# asm 2: srl <y8=%g5,23,>y8=%g5
srl %g5,23,%g5

# qhasm: 				x14 ^= y14
# asm 1: xor <x14=int64#19,<y14=int64#5,>x14=int64#5
# asm 2: xor <x14=%l2,<y14=%i4,>x14=%i4
xor %l2,%i4,%i4

# qhasm: 						y3 |= z3
# asm 1: or  <y3=int64#14,<z3=int64#23,>y3=int64#14
# asm 2: or  <y3=%o4,<z3=%l6,>y3=%o4
or  %o4,%l6,%o4

# qhasm: 				y2 = x14 + x10
# asm 1: add <x14=int64#5,<x10=int64#15,>y2=int64#19
# asm 2: add <x14=%i4,<x10=%o5,>y2=%l2
add %i4,%o5,%l2

# qhasm: 		z13 = (uint32) y13 << 9
# asm 1: sll <y13=int64#22,9,>z13=int64#23
# asm 2: sll <y13=%l5,9,>z13=%l6
sll %l5,9,%l6

# qhasm: 						x3 ^= y3
# asm 1: xor <x3=int64#8,<y3=int64#14,>x3=int64#8
# asm 2: xor <x3=%g4,<y3=%o4,>x3=%g4
xor %g4,%o4,%g4

# qhasm: 		y13 = (uint32) y13 >> 23
# asm 1: srl <y13=int64#22,23,>y13=int64#14
# asm 2: srl <y13=%l5,23,>y13=%o4
srl %l5,23,%o4

# qhasm: y8 |= z8
# asm 1: or  <y8=int64#9,<z8=int64#6,>y8=int64#6
# asm 2: or  <y8=%g5,<z8=%i5,>y8=%i5
or  %g5,%i5,%i5

# qhasm: 				z2 = (uint32) y2 << 9
# asm 1: sll <y2=int64#19,9,>z2=int64#9
# asm 2: sll <y2=%l2,9,>z2=%g5
sll %l2,9,%g5

# qhasm: 						y7 = x3 + x15
# asm 1: add <x3=int64#8,<x15=int64#20,>y7=int64#22
# asm 2: add <x3=%g4,<x15=%l3,>y7=%l5
add %g4,%l3,%l5

# qhasm: 				y2 = (uint32) y2 >> 23
# asm 1: srl <y2=int64#19,23,>y2=int64#19
# asm 2: srl <y2=%l2,23,>y2=%l2
srl %l2,23,%l2

# qhasm: x8 ^= y8
# asm 1: xor <x8=int64#13,<y8=int64#6,>x8=int64#6
# asm 2: xor <x8=%o3,<y8=%i5,>x8=%i5
xor %o3,%i5,%i5

# qhasm: 		y13 |= z13
# asm 1: or  <y13=int64#14,<z13=int64#23,>y13=int64#13
# asm 2: or  <y13=%o4,<z13=%l6,>y13=%o3
or  %o4,%l6,%o3

# qhasm: y12 = x8 + x4
# asm 1: add <x8=int64#6,<x4=int64#1,>y12=int64#14
# asm 2: add <x8=%i5,<x4=%i0,>y12=%o4
add %i5,%i0,%o4

# qhasm: 						z7 = (uint32) y7 << 9
# asm 1: sll <y7=int64#22,9,>z7=int64#23
# asm 2: sll <y7=%l5,9,>z7=%l6
sll %l5,9,%l6

# qhasm: 		x13 ^= y13
# asm 1: xor <x13=int64#18,<y13=int64#13,>x13=int64#18
# asm 2: xor <x13=%l1,<y13=%o3,>x13=%l1
xor %l1,%o3,%l1

# qhasm: 						y7 = (uint32) y7 >> 23
# asm 1: srl <y7=int64#22,23,>y7=int64#13
# asm 2: srl <y7=%l5,23,>y7=%o3
srl %l5,23,%o3

# qhasm: 				y2 |= z2
# asm 1: or  <y2=int64#19,<z2=int64#9,>y2=int64#9
# asm 2: or  <y2=%l2,<z2=%g5,>y2=%g5
or  %l2,%g5,%g5

# qhasm: z12 = (uint32) y12 << 13
# asm 1: sll <y12=int64#14,13,>z12=int64#19
# asm 2: sll <y12=%o4,13,>z12=%l2
sll %o4,13,%l2

# qhasm: 		y1 = x13 + x9
# asm 1: add <x13=int64#18,<x9=int64#2,>y1=int64#22
# asm 2: add <x13=%l1,<x9=%i1,>y1=%l5
add %l1,%i1,%l5

# qhasm: y12 = (uint32) y12 >> 19
# asm 1: srl <y12=int64#14,19,>y12=int64#14
# asm 2: srl <y12=%o4,19,>y12=%o4
srl %o4,19,%o4

# qhasm: 				x2 ^= y2
# asm 1: xor <x2=int64#7,<y2=int64#9,>x2=int64#7
# asm 2: xor <x2=%g1,<y2=%g5,>x2=%g1
xor %g1,%g5,%g1

# qhasm: 						y7 |= z7
# asm 1: or  <y7=int64#13,<z7=int64#23,>y7=int64#9
# asm 2: or  <y7=%o3,<z7=%l6,>y7=%g5
or  %o3,%l6,%g5

# qhasm: 				y6 = x2 + x14
# asm 1: add <x2=int64#7,<x14=int64#5,>y6=int64#13
# asm 2: add <x2=%g1,<x14=%i4,>y6=%o3
add %g1,%i4,%o3

# qhasm: 		z1 = (uint32) y1 << 13
# asm 1: sll <y1=int64#22,13,>z1=int64#23
# asm 2: sll <y1=%l5,13,>z1=%l6
sll %l5,13,%l6

# qhasm: 						x7 ^= y7
# asm 1: xor <x7=int64#12,<y7=int64#9,>x7=int64#9
# asm 2: xor <x7=%o2,<y7=%g5,>x7=%g5
xor %o2,%g5,%g5

# qhasm: 		y1 = (uint32) y1 >> 19
# asm 1: srl <y1=int64#22,19,>y1=int64#12
# asm 2: srl <y1=%l5,19,>y1=%o2
srl %l5,19,%o2

# qhasm: y12 |= z12
# asm 1: or  <y12=int64#14,<z12=int64#19,>y12=int64#14
# asm 2: or  <y12=%o4,<z12=%l2,>y12=%o4
or  %o4,%l2,%o4

# qhasm: 				z6 = (uint32) y6 << 13
# asm 1: sll <y6=int64#13,13,>z6=int64#19
# asm 2: sll <y6=%o3,13,>z6=%l2
sll %o3,13,%l2

# qhasm: 						y11 = x7 + x3
# asm 1: add <x7=int64#9,<x3=int64#8,>y11=int64#22
# asm 2: add <x7=%g5,<x3=%g4,>y11=%l5
add %g5,%g4,%l5

# qhasm: 				y6 = (uint32) y6 >> 19
# asm 1: srl <y6=int64#13,19,>y6=int64#13
# asm 2: srl <y6=%o3,19,>y6=%o3
srl %o3,19,%o3

# qhasm: x12 ^= y12
# asm 1: xor <x12=int64#17,<y12=int64#14,>x12=int64#14
# asm 2: xor <x12=%l0,<y12=%o4,>x12=%o4
xor %l0,%o4,%o4

# qhasm: 		y1 |= z1
# asm 1: or  <y1=int64#12,<z1=int64#23,>y1=int64#12
# asm 2: or  <y1=%o2,<z1=%l6,>y1=%o2
or  %o2,%l6,%o2

# qhasm: y0 = x12 + x8
# asm 1: add <x12=int64#14,<x8=int64#6,>y0=int64#17
# asm 2: add <x12=%o4,<x8=%i5,>y0=%l0
add %o4,%i5,%l0

# qhasm: 						z11 = (uint32) y11 << 13
# asm 1: sll <y11=int64#22,13,>z11=int64#23
# asm 2: sll <y11=%l5,13,>z11=%l6
sll %l5,13,%l6

# qhasm: 		x1 ^= y1
# asm 1: xor <x1=int64#4,<y1=int64#12,>x1=int64#4
# asm 2: xor <x1=%i3,<y1=%o2,>x1=%i3
xor %i3,%o2,%i3

# qhasm: 						y11 = (uint32) y11 >> 19
# asm 1: srl <y11=int64#22,19,>y11=int64#12
# asm 2: srl <y11=%l5,19,>y11=%o2
srl %l5,19,%o2

# qhasm: 				y6 |= z6
# asm 1: or  <y6=int64#13,<z6=int64#19,>y6=int64#13
# asm 2: or  <y6=%o3,<z6=%l2,>y6=%o3
or  %o3,%l2,%o3

# qhasm: z0 = (uint32) y0 << 18
# asm 1: sll <y0=int64#17,18,>z0=int64#19
# asm 2: sll <y0=%l0,18,>z0=%l2
sll %l0,18,%l2

# qhasm: 		y5 = x1 + x13
# asm 1: add <x1=int64#4,<x13=int64#18,>y5=int64#22
# asm 2: add <x1=%i3,<x13=%l1,>y5=%l5
add %i3,%l1,%l5

# qhasm: y0 = (uint32) y0 >> 14
# asm 1: srl <y0=int64#17,14,>y0=int64#17
# asm 2: srl <y0=%l0,14,>y0=%l0
srl %l0,14,%l0

# qhasm: 				x6 ^= y6
# asm 1: xor <x6=int64#11,<y6=int64#13,>x6=int64#11
# asm 2: xor <x6=%o1,<y6=%o3,>x6=%o1
xor %o1,%o3,%o1

# qhasm: 						y11 |= z11
# asm 1: or  <y11=int64#12,<z11=int64#23,>y11=int64#12
# asm 2: or  <y11=%o2,<z11=%l6,>y11=%o2
or  %o2,%l6,%o2

# qhasm: 				y10 = x6 + x2
# asm 1: add <x6=int64#11,<x2=int64#7,>y10=int64#13
# asm 2: add <x6=%o1,<x2=%g1,>y10=%o3
add %o1,%g1,%o3

# qhasm: 		z5 = (uint32) y5 << 18
# asm 1: sll <y5=int64#22,18,>z5=int64#23
# asm 2: sll <y5=%l5,18,>z5=%l6
sll %l5,18,%l6

# qhasm: 						x11 ^= y11
# asm 1: xor <x11=int64#16,<y11=int64#12,>x11=int64#12
# asm 2: xor <x11=%o7,<y11=%o2,>x11=%o2
xor %o7,%o2,%o2

# qhasm: 		y5 = (uint32) y5 >> 14
# asm 1: srl <y5=int64#22,14,>y5=int64#16
# asm 2: srl <y5=%l5,14,>y5=%o7
srl %l5,14,%o7

# qhasm: y0 |= z0
# asm 1: or  <y0=int64#17,<z0=int64#19,>y0=int64#17
# asm 2: or  <y0=%l0,<z0=%l2,>y0=%l0
or  %l0,%l2,%l0

# qhasm: 				z10 = (uint32) y10 << 18
# asm 1: sll <y10=int64#13,18,>z10=int64#19
# asm 2: sll <y10=%o3,18,>z10=%l2
sll %o3,18,%l2

# qhasm: 						y15 = x11 + x7
# asm 1: add <x11=int64#12,<x7=int64#9,>y15=int64#22
# asm 2: add <x11=%o2,<x7=%g5,>y15=%l5
add %o2,%g5,%l5

# qhasm: 				y10 = (uint32) y10 >> 14
# asm 1: srl <y10=int64#13,14,>y10=int64#13
# asm 2: srl <y10=%o3,14,>y10=%o3
srl %o3,14,%o3

# qhasm: x0 ^= y0
# asm 1: xor <x0=int64#3,<y0=int64#17,>x0=int64#3
# asm 2: xor <x0=%i2,<y0=%l0,>x0=%i2
xor %i2,%l0,%i2

# qhasm: 		y5 |= z5
# asm 1: or  <y5=int64#16,<z5=int64#23,>y5=int64#16
# asm 2: or  <y5=%o7,<z5=%l6,>y5=%o7
or  %o7,%l6,%o7

# qhasm: u1 = x0 + x3
# asm 1: add <x0=int64#3,<x3=int64#8,>u1=int64#17
# asm 2: add <x0=%i2,<x3=%g4,>u1=%l0
add %i2,%g4,%l0

# qhasm: 						z15 = (uint32) y15 << 18
# asm 1: sll <y15=int64#22,18,>z15=int64#23
# asm 2: sll <y15=%l5,18,>z15=%l6
sll %l5,18,%l6

# qhasm: 		x5 ^= y5
# asm 1: xor <x5=int64#10,<y5=int64#16,>x5=int64#10
# asm 2: xor <x5=%o0,<y5=%o7,>x5=%o0
xor %o0,%o7,%o0

# qhasm: 						y15 = (uint32) y15 >> 14
# asm 1: srl <y15=int64#22,14,>y15=int64#16
# asm 2: srl <y15=%l5,14,>y15=%o7
srl %l5,14,%o7

# qhasm: 				y10 |= z10
# asm 1: or  <y10=int64#13,<z10=int64#19,>y10=int64#13
# asm 2: or  <y10=%o3,<z10=%l2,>y10=%o3
or  %o3,%l2,%o3

# qhasm: z1 = (uint32) u1 << 7
# asm 1: sll <u1=int64#17,7,>z1=int64#19
# asm 2: sll <u1=%l0,7,>z1=%l2
sll %l0,7,%l2

# qhasm: 		u6 = x5 + x4
# asm 1: add <x5=int64#10,<x4=int64#1,>u6=int64#22
# asm 2: add <x5=%o0,<x4=%i0,>u6=%l5
add %o0,%i0,%l5

# qhasm: u1 = (uint32) u1 >> 25
# asm 1: srl <u1=int64#17,25,>u1=int64#17
# asm 2: srl <u1=%l0,25,>u1=%l0
srl %l0,25,%l0

# qhasm: 				x10 ^= y10
# asm 1: xor <x10=int64#15,<y10=int64#13,>x10=int64#15
# asm 2: xor <x10=%o5,<y10=%o3,>x10=%o5
xor %o5,%o3,%o5

# qhasm: 						y15 |= z15
# asm 1: or  <y15=int64#16,<z15=int64#23,>y15=int64#13
# asm 2: or  <y15=%o7,<z15=%l6,>y15=%o3
or  %o7,%l6,%o3

# qhasm: 				u11 = x10 + x9
# asm 1: add <x10=int64#15,<x9=int64#2,>u11=int64#16
# asm 2: add <x10=%o5,<x9=%i1,>u11=%o7
add %o5,%i1,%o7

# qhasm: 		z6 = (uint32) u6 << 7
# asm 1: sll <u6=int64#22,7,>z6=int64#23
# asm 2: sll <u6=%l5,7,>z6=%l6
sll %l5,7,%l6

# qhasm: 						x15 ^= y15
# asm 1: xor <x15=int64#20,<y15=int64#13,>x15=int64#20
# asm 2: xor <x15=%l3,<y15=%o3,>x15=%l3
xor %l3,%o3,%l3

# qhasm: 		u6 = (uint32) u6 >> 25
# asm 1: srl <u6=int64#22,25,>u6=int64#13
# asm 2: srl <u6=%l5,25,>u6=%o3
srl %l5,25,%o3

# qhasm: u1 |= z1
# asm 1: or  <u1=int64#17,<z1=int64#19,>u1=int64#17
# asm 2: or  <u1=%l0,<z1=%l2,>u1=%l0
or  %l0,%l2,%l0

# qhasm: 				z11 = (uint32) u11 << 7
# asm 1: sll <u11=int64#16,7,>z11=int64#19
# asm 2: sll <u11=%o7,7,>z11=%l2
sll %o7,7,%l2

# qhasm: 						u12 = x15 + x14
# asm 1: add <x15=int64#20,<x14=int64#5,>u12=int64#22
# asm 2: add <x15=%l3,<x14=%i4,>u12=%l5
add %l3,%i4,%l5

# qhasm: 				u11 = (uint32) u11 >> 25
# asm 1: srl <u11=int64#16,25,>u11=int64#16
# asm 2: srl <u11=%o7,25,>u11=%o7
srl %o7,25,%o7

# qhasm: 		u6 |= z6
# asm 1: or  <u6=int64#13,<z6=int64#23,>u6=int64#13
# asm 2: or  <u6=%o3,<z6=%l6,>u6=%o3
or  %o3,%l6,%o3

# qhasm: x1 ^= u1
# asm 1: xor <x1=int64#4,<u1=int64#17,>x1=int64#4
# asm 2: xor <x1=%i3,<u1=%l0,>x1=%i3
xor %i3,%l0,%i3

# qhasm: 						z12 = (uint32) u12 << 7
# asm 1: sll <u12=int64#22,7,>z12=int64#17
# asm 2: sll <u12=%l5,7,>z12=%l0
sll %l5,7,%l0

# qhasm: 		x6 ^= u6
# asm 1: xor <x6=int64#11,<u6=int64#13,>x6=int64#11
# asm 2: xor <x6=%o1,<u6=%o3,>x6=%o1
xor %o1,%o3,%o1

# qhasm: 						u12 = (uint32) u12 >> 25
# asm 1: srl <u12=int64#22,25,>u12=int64#13
# asm 2: srl <u12=%l5,25,>u12=%o3
srl %l5,25,%o3

# qhasm: 				u11 |= z11
# asm 1: or  <u11=int64#16,<z11=int64#19,>u11=int64#16
# asm 2: or  <u11=%o7,<z11=%l2,>u11=%o7
or  %o7,%l2,%o7

# qhasm: u2 = x1 + x0
# asm 1: add <x1=int64#4,<x0=int64#3,>u2=int64#19
# asm 2: add <x1=%i3,<x0=%i2,>u2=%l2
add %i3,%i2,%l2

# qhasm: 		u7 = x6 + x5
# asm 1: add <x6=int64#11,<x5=int64#10,>u7=int64#22
# asm 2: add <x6=%o1,<x5=%o0,>u7=%l5
add %o1,%o0,%l5

# qhasm: z2 = (uint32) u2 << 9
# asm 1: sll <u2=int64#19,9,>z2=int64#23
# asm 2: sll <u2=%l2,9,>z2=%l6
sll %l2,9,%l6

# qhasm: 				x11 ^= u11
# asm 1: xor <x11=int64#12,<u11=int64#16,>x11=int64#16
# asm 2: xor <x11=%o2,<u11=%o7,>x11=%o7
xor %o2,%o7,%o7

# qhasm: u2 = (uint32) u2 >> 23
# asm 1: srl <u2=int64#19,23,>u2=int64#12
# asm 2: srl <u2=%l2,23,>u2=%o2
srl %l2,23,%o2

# qhasm: 						u12 |= z12
# asm 1: or  <u12=int64#13,<z12=int64#17,>u12=int64#13
# asm 2: or  <u12=%o3,<z12=%l0,>u12=%o3
or  %o3,%l0,%o3

# qhasm: 		z7 = (uint32) u7 << 9
# asm 1: sll <u7=int64#22,9,>z7=int64#19
# asm 2: sll <u7=%l5,9,>z7=%l2
sll %l5,9,%l2

# qhasm: 				u8 = x11 + x10
# asm 1: add <x11=int64#16,<x10=int64#15,>u8=int64#24
# asm 2: add <x11=%o7,<x10=%o5,>u8=%l7
add %o7,%o5,%l7

# qhasm: 		u7 = (uint32) u7 >> 23
# asm 1: srl <u7=int64#22,23,>u7=int64#22
# asm 2: srl <u7=%l5,23,>u7=%l5
srl %l5,23,%l5

# qhasm: 						x12 ^= u12
# asm 1: xor <x12=int64#14,<u12=int64#13,>x12=int64#17
# asm 2: xor <x12=%o4,<u12=%o3,>x12=%l0
xor %o4,%o3,%l0

# qhasm: 				z8 = (uint32) u8 << 9
# asm 1: sll <u8=int64#24,9,>z8=int64#13
# asm 2: sll <u8=%l7,9,>z8=%o3
sll %l7,9,%o3

# qhasm: u2 |= z2
# asm 1: or  <u2=int64#12,<z2=int64#23,>u2=int64#12
# asm 2: or  <u2=%o2,<z2=%l6,>u2=%o2
or  %o2,%l6,%o2

# qhasm: 				u8 = (uint32) u8 >> 23
# asm 1: srl <u8=int64#24,23,>u8=int64#14
# asm 2: srl <u8=%l7,23,>u8=%o4
srl %l7,23,%o4

# qhasm: 						u13 = x12 + x15
# asm 1: add <x12=int64#17,<x15=int64#20,>u13=int64#23
# asm 2: add <x12=%l0,<x15=%l3,>u13=%l6
add %l0,%l3,%l6

# qhasm: 		u7 |= z7
# asm 1: or  <u7=int64#22,<z7=int64#19,>u7=int64#19
# asm 2: or  <u7=%l5,<z7=%l2,>u7=%l2
or  %l5,%l2,%l2

# qhasm: x2 ^= u2
# asm 1: xor <x2=int64#7,<u2=int64#12,>x2=int64#7
# asm 2: xor <x2=%g1,<u2=%o2,>x2=%g1
xor %g1,%o2,%g1

# qhasm: 						z13 = (uint32) u13 << 9
# asm 1: sll <u13=int64#23,9,>z13=int64#22
# asm 2: sll <u13=%l6,9,>z13=%l5
sll %l6,9,%l5

# qhasm: 		x7 ^= u7
# asm 1: xor <x7=int64#9,<u7=int64#19,>x7=int64#12
# asm 2: xor <x7=%g5,<u7=%l2,>x7=%o2
xor %g5,%l2,%o2

# qhasm: 						u13 = (uint32) u13 >> 23
# asm 1: srl <u13=int64#23,23,>u13=int64#9
# asm 2: srl <u13=%l6,23,>u13=%g5
srl %l6,23,%g5

# qhasm: 				u8 |= z8
# asm 1: or  <u8=int64#14,<z8=int64#13,>u8=int64#13
# asm 2: or  <u8=%o4,<z8=%o3,>u8=%o3
or  %o4,%o3,%o3

# qhasm: u3 = x2 + x1
# asm 1: add <x2=int64#7,<x1=int64#4,>u3=int64#14
# asm 2: add <x2=%g1,<x1=%i3,>u3=%o4
add %g1,%i3,%o4

# qhasm: 		u4 = x7 + x6
# asm 1: add <x7=int64#12,<x6=int64#11,>u4=int64#19
# asm 2: add <x7=%o2,<x6=%o1,>u4=%l2
add %o2,%o1,%l2

# qhasm: z3 = (uint32) u3 << 13
# asm 1: sll <u3=int64#14,13,>z3=int64#23
# asm 2: sll <u3=%o4,13,>z3=%l6
sll %o4,13,%l6

# qhasm: 				x8 ^= u8
# asm 1: xor <x8=int64#6,<u8=int64#13,>x8=int64#13
# asm 2: xor <x8=%i5,<u8=%o3,>x8=%o3
xor %i5,%o3,%o3

# qhasm: 		z4 = (uint32) u4 << 13
# asm 1: sll <u4=int64#19,13,>z4=int64#6
# asm 2: sll <u4=%l2,13,>z4=%i5
sll %l2,13,%i5

# qhasm: 						u13 |= z13
# asm 1: or  <u13=int64#9,<z13=int64#22,>u13=int64#9
# asm 2: or  <u13=%g5,<z13=%l5,>u13=%g5
or  %g5,%l5,%g5

# qhasm: u3 = (uint32) u3 >> 19
# asm 1: srl <u3=int64#14,19,>u3=int64#14
# asm 2: srl <u3=%o4,19,>u3=%o4
srl %o4,19,%o4

# qhasm: 				u9 = x8 + x11
# asm 1: add <x8=int64#13,<x11=int64#16,>u9=int64#22
# asm 2: add <x8=%o3,<x11=%o7,>u9=%l5
add %o3,%o7,%l5

# qhasm: 		u4 = (uint32) u4 >> 19
# asm 1: srl <u4=int64#19,19,>u4=int64#19
# asm 2: srl <u4=%l2,19,>u4=%l2
srl %l2,19,%l2

# qhasm: 						x13 ^= u13
# asm 1: xor <x13=int64#18,<u13=int64#9,>x13=int64#18
# asm 2: xor <x13=%l1,<u13=%g5,>x13=%l1
xor %l1,%g5,%l1

# qhasm: 				z9 = (uint32) u9 << 13
# asm 1: sll <u9=int64#22,13,>z9=int64#24
# asm 2: sll <u9=%l5,13,>z9=%l7
sll %l5,13,%l7

# qhasm: u3 |= z3
# asm 1: or  <u3=int64#14,<z3=int64#23,>u3=int64#9
# asm 2: or  <u3=%o4,<z3=%l6,>u3=%g5
or  %o4,%l6,%g5

# qhasm: 				u9 = (uint32) u9 >> 19
# asm 1: srl <u9=int64#22,19,>u9=int64#14
# asm 2: srl <u9=%l5,19,>u9=%o4
srl %l5,19,%o4

# qhasm: 						u14 = x13 + x12
# asm 1: add <x13=int64#18,<x12=int64#17,>u14=int64#22
# asm 2: add <x13=%l1,<x12=%l0,>u14=%l5
add %l1,%l0,%l5

# qhasm: 		u4 |= z4
# asm 1: or  <u4=int64#19,<z4=int64#6,>u4=int64#6
# asm 2: or  <u4=%l2,<z4=%i5,>u4=%i5
or  %l2,%i5,%i5

# qhasm: x3 ^= u3
# asm 1: xor <x3=int64#8,<u3=int64#9,>x3=int64#8
# asm 2: xor <x3=%g4,<u3=%g5,>x3=%g4
xor %g4,%g5,%g4

# qhasm: 						z14 = (uint32) u14 << 13
# asm 1: sll <u14=int64#22,13,>z14=int64#19
# asm 2: sll <u14=%l5,13,>z14=%l2
sll %l5,13,%l2

# qhasm: 		x4 ^= u4
# asm 1: xor <x4=int64#1,<u4=int64#6,>x4=int64#9
# asm 2: xor <x4=%i0,<u4=%i5,>x4=%g5
xor %i0,%i5,%g5

# qhasm: 						u14 = (uint32) u14 >> 19
# asm 1: srl <u14=int64#22,19,>u14=int64#1
# asm 2: srl <u14=%l5,19,>u14=%i0
srl %l5,19,%i0

# qhasm: 				u9 |= z9
# asm 1: or  <u9=int64#14,<z9=int64#24,>u9=int64#6
# asm 2: or  <u9=%o4,<z9=%l7,>u9=%i5
or  %o4,%l7,%i5

# qhasm: u0 = x3 + x2
# asm 1: add <x3=int64#8,<x2=int64#7,>u0=int64#22
# asm 2: add <x3=%g4,<x2=%g1,>u0=%l5
add %g4,%g1,%l5

# qhasm: 		u5 = x4 + x7
# asm 1: add <x4=int64#9,<x7=int64#12,>u5=int64#23
# asm 2: add <x4=%g5,<x7=%o2,>u5=%l6
add %g5,%o2,%l6

# qhasm: z0 = (uint32) u0 << 18
# asm 1: sll <u0=int64#22,18,>z0=int64#24
# asm 2: sll <u0=%l5,18,>z0=%l7
sll %l5,18,%l7

# qhasm: 				x9 ^= u9
# asm 1: xor <x9=int64#2,<u9=int64#6,>x9=int64#14
# asm 2: xor <x9=%i1,<u9=%i5,>x9=%o4
xor %i1,%i5,%o4

# qhasm: 		z5 = (uint32) u5 << 18
# asm 1: sll <u5=int64#23,18,>z5=int64#2
# asm 2: sll <u5=%l6,18,>z5=%i1
sll %l6,18,%i1

# qhasm: 						u14 |= z14
# asm 1: or  <u14=int64#1,<z14=int64#19,>u14=int64#1
# asm 2: or  <u14=%i0,<z14=%l2,>u14=%i0
or  %i0,%l2,%i0

# qhasm: u0 = (uint32) u0 >> 14
# asm 1: srl <u0=int64#22,14,>u0=int64#6
# asm 2: srl <u0=%l5,14,>u0=%i5
srl %l5,14,%i5

# qhasm: 				u10 = x9 + x8
# asm 1: add <x9=int64#14,<x8=int64#13,>u10=int64#22
# asm 2: add <x9=%o4,<x8=%o3,>u10=%l5
add %o4,%o3,%l5

# qhasm: 		u5 = (uint32) u5 >> 14
# asm 1: srl <u5=int64#23,14,>u5=int64#23
# asm 2: srl <u5=%l6,14,>u5=%l6
srl %l6,14,%l6

# qhasm: 						x14 ^= u14
# asm 1: xor <x14=int64#5,<u14=int64#1,>x14=int64#19
# asm 2: xor <x14=%i4,<u14=%i0,>x14=%l2
xor %i4,%i0,%l2

# qhasm: 				z10 = (uint32) u10 << 18
# asm 1: sll <u10=int64#22,18,>z10=int64#1
# asm 2: sll <u10=%l5,18,>z10=%i0
sll %l5,18,%i0

# qhasm: u0 |= z0
# asm 1: or  <u0=int64#6,<z0=int64#24,>u0=int64#5
# asm 2: or  <u0=%i5,<z0=%l7,>u0=%i4
or  %i5,%l7,%i4

# qhasm: 				u10 = (uint32) u10 >> 14
# asm 1: srl <u10=int64#22,14,>u10=int64#6
# asm 2: srl <u10=%l5,14,>u10=%i5
srl %l5,14,%i5

# qhasm: 						u15 = x14 + x13
# asm 1: add <x14=int64#19,<x13=int64#18,>u15=int64#22
# asm 2: add <x14=%l2,<x13=%l1,>u15=%l5
add %l2,%l1,%l5

# qhasm: 		u5 |= z5
# asm 1: or  <u5=int64#23,<z5=int64#2,>u5=int64#2
# asm 2: or  <u5=%l6,<z5=%i1,>u5=%i1
or  %l6,%i1,%i1

# qhasm: x0 ^= u0
# asm 1: xor <x0=int64#3,<u0=int64#5,>x0=int64#3
# asm 2: xor <x0=%i2,<u0=%i4,>x0=%i2
xor %i2,%i4,%i2

# qhasm: 						z15 = (uint32) u15 << 18
# asm 1: sll <u15=int64#22,18,>z15=int64#5
# asm 2: sll <u15=%l5,18,>z15=%i4
sll %l5,18,%i4

# qhasm: 		x5 ^= u5
# asm 1: xor <x5=int64#10,<u5=int64#2,>x5=int64#10
# asm 2: xor <x5=%o0,<u5=%i1,>x5=%o0
xor %o0,%i1,%o0

# qhasm: 						u15 = (uint32) u15 >> 14
# asm 1: srl <u15=int64#22,14,>u15=int64#2
# asm 2: srl <u15=%l5,14,>u15=%i1
srl %l5,14,%i1

# qhasm:                  unsigned>? i -= 2
# asm 1: subcc <i=int64#21,2,>i=int64#21
# asm 2: subcc <i=%l4,2,>i=%l4
subcc %l4,2,%l4

# qhasm: 				u10 |= z10
# asm 1: or  <u10=int64#6,<z10=int64#1,>u10=int64#1
# asm 2: or  <u10=%i5,<z10=%i0,>u10=%i0
or  %i5,%i0,%i0

# qhasm: 						u15 |= z15
# asm 1: or  <u15=int64#2,<z15=int64#5,>u15=int64#2
# asm 2: or  <u15=%i1,<z15=%i4,>u15=%i1
or  %i1,%i4,%i1

# qhasm: 				x10 ^= u10
# asm 1: xor <x10=int64#15,<u10=int64#1,>x10=int64#15
# asm 2: xor <x10=%o5,<u10=%i0,>x10=%o5
xor %o5,%i0,%o5

# qhasm: 						x15 ^= u15
# asm 1: xor <x15=int64#20,<u15=int64#2,>x15=int64#20
# asm 2: xor <x15=%l3,<u15=%i1,>x15=%l3
xor %l3,%i1,%l3

# qhasm: goto mainloop if unsigned>
bgu,pt %xcc,._mainloop
nop

# qhasm:   x = x_stack
# asm 1: ldx [%fp+2023-<x_stack=stack64#5],>x=int64#1
# asm 2: ldx [%fp+2023-<x_stack=32],>x=%i0
ldx [%fp+2023-32],%i0

# qhasm:   q0 = *(uint32 *) (x + 0)
# asm 1: lduw [<x=int64#1+0],>q0=int64#2
# asm 2: lduw [<x=%i0+0],>q0=%i1
lduw [%i0+0],%i1

# qhasm:   q1 = *(uint32 *) (x + 4)
# asm 1: lduw [<x=int64#1+4],>q1=int64#5
# asm 2: lduw [<x=%i0+4],>q1=%i4
lduw [%i0+4],%i4

# qhasm:   q2 = *(uint32 *) (x + 8)
# asm 1: lduw [<x=int64#1+8],>q2=int64#6
# asm 2: lduw [<x=%i0+8],>q2=%i5
lduw [%i0+8],%i5

# qhasm:   q3 = *(uint32 *) (x + 12)
# asm 1: lduw [<x=int64#1+12],>q3=int64#21
# asm 2: lduw [<x=%i0+12],>q3=%l4
lduw [%i0+12],%l4

# qhasm:   x0 += q0
# asm 1: add <x0=int64#3,<q0=int64#2,>x0=int64#2
# asm 2: add <x0=%i2,<q0=%i1,>x0=%i1
add %i2,%i1,%i1

# qhasm:   q4 = *(uint32 *) (x + 16)
# asm 1: lduw [<x=int64#1+16],>q4=int64#3
# asm 2: lduw [<x=%i0+16],>q4=%i2
lduw [%i0+16],%i2

# qhasm:   x1 += q1
# asm 1: add <x1=int64#4,<q1=int64#5,>x1=int64#4
# asm 2: add <x1=%i3,<q1=%i4,>x1=%i3
add %i3,%i4,%i3

# qhasm:   q5 = *(uint32 *) (x + 20)
# asm 1: lduw [<x=int64#1+20],>q5=int64#5
# asm 2: lduw [<x=%i0+20],>q5=%i4
lduw [%i0+20],%i4

# qhasm:   x2 += q2
# asm 1: add <x2=int64#7,<q2=int64#6,>x2=int64#6
# asm 2: add <x2=%g1,<q2=%i5,>x2=%i5
add %g1,%i5,%i5

# qhasm:   q6 = *(uint32 *) (x + 24)
# asm 1: lduw [<x=int64#1+24],>q6=int64#7
# asm 2: lduw [<x=%i0+24],>q6=%g1
lduw [%i0+24],%g1

# qhasm:   x3 += q3
# asm 1: add <x3=int64#8,<q3=int64#21,>x3=int64#8
# asm 2: add <x3=%g4,<q3=%l4,>x3=%g4
add %g4,%l4,%g4

# qhasm:   q7 = *(uint32 *) (x + 28)
# asm 1: lduw [<x=int64#1+28],>q7=int64#21
# asm 2: lduw [<x=%i0+28],>q7=%l4
lduw [%i0+28],%l4

# qhasm:   x4 += q4
# asm 1: add <x4=int64#9,<q4=int64#3,>x4=int64#3
# asm 2: add <x4=%g5,<q4=%i2,>x4=%i2
add %g5,%i2,%i2

# qhasm:   q8 = *(uint32 *) (x + 32)
# asm 1: lduw [<x=int64#1+32],>q8=int64#9
# asm 2: lduw [<x=%i0+32],>q8=%g5
lduw [%i0+32],%g5

# qhasm:   x5 += q5
# asm 1: add <x5=int64#10,<q5=int64#5,>x5=int64#5
# asm 2: add <x5=%o0,<q5=%i4,>x5=%i4
add %o0,%i4,%i4

# qhasm:   q9 = *(uint32 *) (x + 36)
# asm 1: lduw [<x=int64#1+36],>q9=int64#10
# asm 2: lduw [<x=%i0+36],>q9=%o0
lduw [%i0+36],%o0

# qhasm:   x6 += q6
# asm 1: add <x6=int64#11,<q6=int64#7,>x6=int64#7
# asm 2: add <x6=%o1,<q6=%g1,>x6=%g1
add %o1,%g1,%g1

# qhasm:   q10 = *(uint32 *) (x + 40)
# asm 1: lduw [<x=int64#1+40],>q10=int64#11
# asm 2: lduw [<x=%i0+40],>q10=%o1
lduw [%i0+40],%o1

# qhasm:   x7 += q7
# asm 1: add <x7=int64#12,<q7=int64#21,>x7=int64#12
# asm 2: add <x7=%o2,<q7=%l4,>x7=%o2
add %o2,%l4,%o2

# qhasm:   q11 = *(uint32 *) (x + 44)
# asm 1: lduw [<x=int64#1+44],>q11=int64#21
# asm 2: lduw [<x=%i0+44],>q11=%l4
lduw [%i0+44],%l4

# qhasm:   x8 += q8
# asm 1: add <x8=int64#13,<q8=int64#9,>x8=int64#13
# asm 2: add <x8=%o3,<q8=%g5,>x8=%o3
add %o3,%g5,%o3

# qhasm:   q8 += 1
# asm 1: add <q8=int64#9,1,>q8=int64#9
# asm 2: add <q8=%g5,1,>q8=%g5
add %g5,1,%g5

# qhasm:   *(uint32 *) (x + 32) = q8
# asm 1: stw <q8=int64#9,[<x=int64#1+32]
# asm 2: stw <q8=%g5,[<x=%i0+32]
stw %g5,[%i0+32]

# qhasm:   q8 = (uint64) q8 >> 32
# asm 1: srlx <q8=int64#9,32,>q8=int64#9
# asm 2: srlx <q8=%g5,32,>q8=%g5
srlx %g5,32,%g5

# qhasm:   q12 = *(uint32 *) (x + 48)
# asm 1: lduw [<x=int64#1+48],>q12=int64#22
# asm 2: lduw [<x=%i0+48],>q12=%l5
lduw [%i0+48],%l5

# qhasm:   x9 += q9
# asm 1: add <x9=int64#14,<q9=int64#10,>x9=int64#14
# asm 2: add <x9=%o4,<q9=%o0,>x9=%o4
add %o4,%o0,%o4

# qhasm:   q9 += q8
# asm 1: add <q9=int64#10,<q8=int64#9,>q9=int64#9
# asm 2: add <q9=%o0,<q8=%g5,>q9=%g5
add %o0,%g5,%g5

# qhasm:   *(uint32 *) (x + 36) = q9
# asm 1: stw <q9=int64#9,[<x=int64#1+36]
# asm 2: stw <q9=%g5,[<x=%i0+36]
stw %g5,[%i0+36]

# qhasm:   q13 = *(uint32 *) (x + 52)
# asm 1: lduw [<x=int64#1+52],>q13=int64#9
# asm 2: lduw [<x=%i0+52],>q13=%g5
lduw [%i0+52],%g5

# qhasm:   x10 += q10
# asm 1: add <x10=int64#15,<q10=int64#11,>x10=int64#10
# asm 2: add <x10=%o5,<q10=%o1,>x10=%o0
add %o5,%o1,%o0

# qhasm:   q14 = *(uint32 *) (x + 56)
# asm 1: lduw [<x=int64#1+56],>q14=int64#11
# asm 2: lduw [<x=%i0+56],>q14=%o1
lduw [%i0+56],%o1

# qhasm:   x11 += q11
# asm 1: add <x11=int64#16,<q11=int64#21,>x11=int64#15
# asm 2: add <x11=%o7,<q11=%l4,>x11=%o5
add %o7,%l4,%o5

# qhasm:   q15 = *(uint32 *) (x + 60)
# asm 1: lduw [<x=int64#1+60],>q15=int64#16
# asm 2: lduw [<x=%i0+60],>q15=%o7
lduw [%i0+60],%o7

# qhasm:   x12 += q12
# asm 1: add <x12=int64#17,<q12=int64#22,>x12=int64#17
# asm 2: add <x12=%l0,<q12=%l5,>x12=%l0
add %l0,%l5,%l0

# qhasm:   x13 += q13
# asm 1: add <x13=int64#18,<q13=int64#9,>x13=int64#9
# asm 2: add <x13=%l1,<q13=%g5,>x13=%g5
add %l1,%g5,%g5

# qhasm:   x14 += q14
# asm 1: add <x14=int64#19,<q14=int64#11,>x14=int64#11
# asm 2: add <x14=%l2,<q14=%o1,>x14=%o1
add %l2,%o1,%o1

# qhasm:   x15 += q15
# asm 1: add <x15=int64#20,<q15=int64#16,>x15=int64#16
# asm 2: add <x15=%l3,<q15=%o7,>x15=%o7
add %l3,%o7,%o7

# qhasm:   m = m_stack
# asm 1: ldx [%fp+2023-<m_stack=stack64#4],>m=int64#18
# asm 2: ldx [%fp+2023-<m_stack=24],>m=%l1
ldx [%fp+2023-24],%l1

# qhasm:   m0 = *(swapendian uint32 *) m
# asm 1: lduwa [<m=int64#18] 0x88,>m0=int64#19
# asm 2: lduwa [<m=%l1] 0x88,>m0=%l2
lduwa [%l1] 0x88,%l2

# qhasm:   m += 4
# asm 1: add <m=int64#18,4,>m=int64#18
# asm 2: add <m=%l1,4,>m=%l1
add %l1,4,%l1

# qhasm:   m1 = *(swapendian uint32 *) m
# asm 1: lduwa [<m=int64#18] 0x88,>m1=int64#20
# asm 2: lduwa [<m=%l1] 0x88,>m1=%l3
lduwa [%l1] 0x88,%l3

# qhasm:   m += 4
# asm 1: add <m=int64#18,4,>m=int64#18
# asm 2: add <m=%l1,4,>m=%l1
add %l1,4,%l1

# qhasm:   m2 = *(swapendian uint32 *) m
# asm 1: lduwa [<m=int64#18] 0x88,>m2=int64#21
# asm 2: lduwa [<m=%l1] 0x88,>m2=%l4
lduwa [%l1] 0x88,%l4

# qhasm:   m += 4
# asm 1: add <m=int64#18,4,>m=int64#18
# asm 2: add <m=%l1,4,>m=%l1
add %l1,4,%l1

# qhasm:   m3 = *(swapendian uint32 *) m
# asm 1: lduwa [<m=int64#18] 0x88,>m3=int64#22
# asm 2: lduwa [<m=%l1] 0x88,>m3=%l5
lduwa [%l1] 0x88,%l5

# qhasm:   m += 4
# asm 1: add <m=int64#18,4,>m=int64#18
# asm 2: add <m=%l1,4,>m=%l1
add %l1,4,%l1

# qhasm:   x0 ^= m0
# asm 1: xor <x0=int64#2,<m0=int64#19,>x0=int64#2
# asm 2: xor <x0=%i1,<m0=%l2,>x0=%i1
xor %i1,%l2,%i1

# qhasm:   m4 = *(swapendian uint32 *) m
# asm 1: lduwa [<m=int64#18] 0x88,>m4=int64#19
# asm 2: lduwa [<m=%l1] 0x88,>m4=%l2
lduwa [%l1] 0x88,%l2

# qhasm:   m += 4
# asm 1: add <m=int64#18,4,>m=int64#18
# asm 2: add <m=%l1,4,>m=%l1
add %l1,4,%l1

# qhasm:   x1 ^= m1
# asm 1: xor <x1=int64#4,<m1=int64#20,>x1=int64#4
# asm 2: xor <x1=%i3,<m1=%l3,>x1=%i3
xor %i3,%l3,%i3

# qhasm:   m5 = *(swapendian uint32 *) m
# asm 1: lduwa [<m=int64#18] 0x88,>m5=int64#20
# asm 2: lduwa [<m=%l1] 0x88,>m5=%l3
lduwa [%l1] 0x88,%l3

# qhasm:   m += 4
# asm 1: add <m=int64#18,4,>m=int64#18
# asm 2: add <m=%l1,4,>m=%l1
add %l1,4,%l1

# qhasm:   x2 ^= m2
# asm 1: xor <x2=int64#6,<m2=int64#21,>x2=int64#21
# asm 2: xor <x2=%i5,<m2=%l4,>x2=%l4
xor %i5,%l4,%l4

# qhasm:   m6 = *(swapendian uint32 *) m
# asm 1: lduwa [<m=int64#18] 0x88,>m6=int64#6
# asm 2: lduwa [<m=%l1] 0x88,>m6=%i5
lduwa [%l1] 0x88,%i5

# qhasm:   m += 4
# asm 1: add <m=int64#18,4,>m=int64#18
# asm 2: add <m=%l1,4,>m=%l1
add %l1,4,%l1

# qhasm:   x3 ^= m3
# asm 1: xor <x3=int64#8,<m3=int64#22,>x3=int64#8
# asm 2: xor <x3=%g4,<m3=%l5,>x3=%g4
xor %g4,%l5,%g4

# qhasm:   m7 = *(swapendian uint32 *) m
# asm 1: lduwa [<m=int64#18] 0x88,>m7=int64#22
# asm 2: lduwa [<m=%l1] 0x88,>m7=%l5
lduwa [%l1] 0x88,%l5

# qhasm:   m += 4
# asm 1: add <m=int64#18,4,>m=int64#18
# asm 2: add <m=%l1,4,>m=%l1
add %l1,4,%l1

# qhasm:   x4 ^= m4
# asm 1: xor <x4=int64#3,<m4=int64#19,>x4=int64#3
# asm 2: xor <x4=%i2,<m4=%l2,>x4=%i2
xor %i2,%l2,%i2

# qhasm:   m8 = *(swapendian uint32 *) m
# asm 1: lduwa [<m=int64#18] 0x88,>m8=int64#19
# asm 2: lduwa [<m=%l1] 0x88,>m8=%l2
lduwa [%l1] 0x88,%l2

# qhasm:   m += 4
# asm 1: add <m=int64#18,4,>m=int64#18
# asm 2: add <m=%l1,4,>m=%l1
add %l1,4,%l1

# qhasm:   x5 ^= m5
# asm 1: xor <x5=int64#5,<m5=int64#20,>x5=int64#5
# asm 2: xor <x5=%i4,<m5=%l3,>x5=%i4
xor %i4,%l3,%i4

# qhasm:   m9 = *(swapendian uint32 *) m
# asm 1: lduwa [<m=int64#18] 0x88,>m9=int64#20
# asm 2: lduwa [<m=%l1] 0x88,>m9=%l3
lduwa [%l1] 0x88,%l3

# qhasm:   m += 4
# asm 1: add <m=int64#18,4,>m=int64#18
# asm 2: add <m=%l1,4,>m=%l1
add %l1,4,%l1

# qhasm:   x6 ^= m6
# asm 1: xor <x6=int64#7,<m6=int64#6,>x6=int64#7
# asm 2: xor <x6=%g1,<m6=%i5,>x6=%g1
xor %g1,%i5,%g1

# qhasm:   m10 = *(swapendian uint32 *) m
# asm 1: lduwa [<m=int64#18] 0x88,>m10=int64#6
# asm 2: lduwa [<m=%l1] 0x88,>m10=%i5
lduwa [%l1] 0x88,%i5

# qhasm:   m += 4
# asm 1: add <m=int64#18,4,>m=int64#18
# asm 2: add <m=%l1,4,>m=%l1
add %l1,4,%l1

# qhasm:   x7 ^= m7
# asm 1: xor <x7=int64#12,<m7=int64#22,>x7=int64#12
# asm 2: xor <x7=%o2,<m7=%l5,>x7=%o2
xor %o2,%l5,%o2

# qhasm:   m11 = *(swapendian uint32 *) m
# asm 1: lduwa [<m=int64#18] 0x88,>m11=int64#22
# asm 2: lduwa [<m=%l1] 0x88,>m11=%l5
lduwa [%l1] 0x88,%l5

# qhasm:   m += 4
# asm 1: add <m=int64#18,4,>m=int64#18
# asm 2: add <m=%l1,4,>m=%l1
add %l1,4,%l1

# qhasm:   x8 ^= m8
# asm 1: xor <x8=int64#13,<m8=int64#19,>x8=int64#13
# asm 2: xor <x8=%o3,<m8=%l2,>x8=%o3
xor %o3,%l2,%o3

# qhasm:   m12 = *(swapendian uint32 *) m
# asm 1: lduwa [<m=int64#18] 0x88,>m12=int64#19
# asm 2: lduwa [<m=%l1] 0x88,>m12=%l2
lduwa [%l1] 0x88,%l2

# qhasm:   m += 4
# asm 1: add <m=int64#18,4,>m=int64#18
# asm 2: add <m=%l1,4,>m=%l1
add %l1,4,%l1

# qhasm:   x9 ^= m9
# asm 1: xor <x9=int64#14,<m9=int64#20,>x9=int64#14
# asm 2: xor <x9=%o4,<m9=%l3,>x9=%o4
xor %o4,%l3,%o4

# qhasm:   m13 = *(swapendian uint32 *) m
# asm 1: lduwa [<m=int64#18] 0x88,>m13=int64#20
# asm 2: lduwa [<m=%l1] 0x88,>m13=%l3
lduwa [%l1] 0x88,%l3

# qhasm:   m += 4
# asm 1: add <m=int64#18,4,>m=int64#18
# asm 2: add <m=%l1,4,>m=%l1
add %l1,4,%l1

# qhasm:   x10 ^= m10
# asm 1: xor <x10=int64#10,<m10=int64#6,>x10=int64#10
# asm 2: xor <x10=%o0,<m10=%i5,>x10=%o0
xor %o0,%i5,%o0

# qhasm:   m14 = *(swapendian uint32 *) m
# asm 1: lduwa [<m=int64#18] 0x88,>m14=int64#23
# asm 2: lduwa [<m=%l1] 0x88,>m14=%l6
lduwa [%l1] 0x88,%l6

# qhasm:   m += 4
# asm 1: add <m=int64#18,4,>m=int64#6
# asm 2: add <m=%l1,4,>m=%i5
add %l1,4,%i5

# qhasm:   x11 ^= m11
# asm 1: xor <x11=int64#15,<m11=int64#22,>x11=int64#15
# asm 2: xor <x11=%o5,<m11=%l5,>x11=%o5
xor %o5,%l5,%o5

# qhasm:   m15 = *(swapendian uint32 *) m
# asm 1: lduwa [<m=int64#6] 0x88,>m15=int64#18
# asm 2: lduwa [<m=%i5] 0x88,>m15=%l1
lduwa [%i5] 0x88,%l1

# qhasm:   m += 4
# asm 1: add <m=int64#6,4,>m=int64#6
# asm 2: add <m=%i5,4,>m=%i5
add %i5,4,%i5

# qhasm:   x12 ^= m12
# asm 1: xor <x12=int64#17,<m12=int64#19,>x12=int64#17
# asm 2: xor <x12=%l0,<m12=%l2,>x12=%l0
xor %l0,%l2,%l0

# qhasm:   x13 ^= m13
# asm 1: xor <x13=int64#9,<m13=int64#20,>x13=int64#9
# asm 2: xor <x13=%g5,<m13=%l3,>x13=%g5
xor %g5,%l3,%g5

# qhasm:   x14 ^= m14
# asm 1: xor <x14=int64#11,<m14=int64#23,>x14=int64#11
# asm 2: xor <x14=%o1,<m14=%l6,>x14=%o1
xor %o1,%l6,%o1

# qhasm:   x15 ^= m15
# asm 1: xor <x15=int64#16,<m15=int64#18,>x15=int64#16
# asm 2: xor <x15=%o7,<m15=%l1,>x15=%o7
xor %o7,%l1,%o7

# qhasm:   out = out_stack
# asm 1: ldx [%fp+2023-<out_stack=stack64#3],>out=int64#18
# asm 2: ldx [%fp+2023-<out_stack=16],>out=%l1
ldx [%fp+2023-16],%l1

# qhasm:   *(swapendian uint32 *) out = x0
# asm 1: stwa <x0=int64#2,[<out=int64#18] 0x88
# asm 2: stwa <x0=%i1,[<out=%l1] 0x88
stwa %i1,[%l1] 0x88

# qhasm:   out += 4
# asm 1: add <out=int64#18,4,>out=int64#2
# asm 2: add <out=%l1,4,>out=%i1
add %l1,4,%i1

# qhasm:   *(swapendian uint32 *) out = x1
# asm 1: stwa <x1=int64#4,[<out=int64#2] 0x88
# asm 2: stwa <x1=%i3,[<out=%i1] 0x88
stwa %i3,[%i1] 0x88

# qhasm:   out += 4
# asm 1: add <out=int64#2,4,>out=int64#2
# asm 2: add <out=%i1,4,>out=%i1
add %i1,4,%i1

# qhasm:   *(swapendian uint32 *) out = x2
# asm 1: stwa <x2=int64#21,[<out=int64#2] 0x88
# asm 2: stwa <x2=%l4,[<out=%i1] 0x88
stwa %l4,[%i1] 0x88

# qhasm:   out += 4
# asm 1: add <out=int64#2,4,>out=int64#2
# asm 2: add <out=%i1,4,>out=%i1
add %i1,4,%i1

# qhasm:   *(swapendian uint32 *) out = x3
# asm 1: stwa <x3=int64#8,[<out=int64#2] 0x88
# asm 2: stwa <x3=%g4,[<out=%i1] 0x88
stwa %g4,[%i1] 0x88

# qhasm:   out += 4
# asm 1: add <out=int64#2,4,>out=int64#2
# asm 2: add <out=%i1,4,>out=%i1
add %i1,4,%i1

# qhasm:   *(swapendian uint32 *) out = x4
# asm 1: stwa <x4=int64#3,[<out=int64#2] 0x88
# asm 2: stwa <x4=%i2,[<out=%i1] 0x88
stwa %i2,[%i1] 0x88

# qhasm:   out += 4
# asm 1: add <out=int64#2,4,>out=int64#2
# asm 2: add <out=%i1,4,>out=%i1
add %i1,4,%i1

# qhasm:   *(swapendian uint32 *) out = x5
# asm 1: stwa <x5=int64#5,[<out=int64#2] 0x88
# asm 2: stwa <x5=%i4,[<out=%i1] 0x88
stwa %i4,[%i1] 0x88

# qhasm:   out += 4
# asm 1: add <out=int64#2,4,>out=int64#2
# asm 2: add <out=%i1,4,>out=%i1
add %i1,4,%i1

# qhasm:   *(swapendian uint32 *) out = x6
# asm 1: stwa <x6=int64#7,[<out=int64#2] 0x88
# asm 2: stwa <x6=%g1,[<out=%i1] 0x88
stwa %g1,[%i1] 0x88

# qhasm:   out += 4
# asm 1: add <out=int64#2,4,>out=int64#2
# asm 2: add <out=%i1,4,>out=%i1
add %i1,4,%i1

# qhasm:   *(swapendian uint32 *) out = x7
# asm 1: stwa <x7=int64#12,[<out=int64#2] 0x88
# asm 2: stwa <x7=%o2,[<out=%i1] 0x88
stwa %o2,[%i1] 0x88

# qhasm:   out += 4
# asm 1: add <out=int64#2,4,>out=int64#2
# asm 2: add <out=%i1,4,>out=%i1
add %i1,4,%i1

# qhasm:   *(swapendian uint32 *) out = x8
# asm 1: stwa <x8=int64#13,[<out=int64#2] 0x88
# asm 2: stwa <x8=%o3,[<out=%i1] 0x88
stwa %o3,[%i1] 0x88

# qhasm:   out += 4
# asm 1: add <out=int64#2,4,>out=int64#2
# asm 2: add <out=%i1,4,>out=%i1
add %i1,4,%i1

# qhasm:   *(swapendian uint32 *) out = x9
# asm 1: stwa <x9=int64#14,[<out=int64#2] 0x88
# asm 2: stwa <x9=%o4,[<out=%i1] 0x88
stwa %o4,[%i1] 0x88

# qhasm:   out += 4
# asm 1: add <out=int64#2,4,>out=int64#2
# asm 2: add <out=%i1,4,>out=%i1
add %i1,4,%i1

# qhasm:   *(swapendian uint32 *) out = x10
# asm 1: stwa <x10=int64#10,[<out=int64#2] 0x88
# asm 2: stwa <x10=%o0,[<out=%i1] 0x88
stwa %o0,[%i1] 0x88

# qhasm:   out += 4
# asm 1: add <out=int64#2,4,>out=int64#2
# asm 2: add <out=%i1,4,>out=%i1
add %i1,4,%i1

# qhasm:   *(swapendian uint32 *) out = x11
# asm 1: stwa <x11=int64#15,[<out=int64#2] 0x88
# asm 2: stwa <x11=%o5,[<out=%i1] 0x88
stwa %o5,[%i1] 0x88

# qhasm:   out += 4
# asm 1: add <out=int64#2,4,>out=int64#2
# asm 2: add <out=%i1,4,>out=%i1
add %i1,4,%i1

# qhasm:   *(swapendian uint32 *) out = x12
# asm 1: stwa <x12=int64#17,[<out=int64#2] 0x88
# asm 2: stwa <x12=%l0,[<out=%i1] 0x88
stwa %l0,[%i1] 0x88

# qhasm:   out += 4
# asm 1: add <out=int64#2,4,>out=int64#2
# asm 2: add <out=%i1,4,>out=%i1
add %i1,4,%i1

# qhasm:   *(swapendian uint32 *) out = x13
# asm 1: stwa <x13=int64#9,[<out=int64#2] 0x88
# asm 2: stwa <x13=%g5,[<out=%i1] 0x88
stwa %g5,[%i1] 0x88

# qhasm:   out += 4
# asm 1: add <out=int64#2,4,>out=int64#2
# asm 2: add <out=%i1,4,>out=%i1
add %i1,4,%i1

# qhasm:   *(swapendian uint32 *) out = x14
# asm 1: stwa <x14=int64#11,[<out=int64#2] 0x88
# asm 2: stwa <x14=%o1,[<out=%i1] 0x88
stwa %o1,[%i1] 0x88

# qhasm:   out += 4
# asm 1: add <out=int64#2,4,>out=int64#2
# asm 2: add <out=%i1,4,>out=%i1
add %i1,4,%i1

# qhasm:   *(swapendian uint32 *) out = x15
# asm 1: stwa <x15=int64#16,[<out=int64#2] 0x88
# asm 2: stwa <x15=%o7,[<out=%i1] 0x88
stwa %o7,[%i1] 0x88

# qhasm:   out += 4
# asm 1: add <out=int64#2,4,>out=int64#2
# asm 2: add <out=%i1,4,>out=%i1
add %i1,4,%i1

# qhasm:   bytes = bytes_stack
# asm 1: ldx [%fp+2023-<bytes_stack=stack64#2],>bytes=int64#3
# asm 2: ldx [%fp+2023-<bytes_stack=8],>bytes=%i2
ldx [%fp+2023-8],%i2

# qhasm:                         unsigned>? bytes -= 64
# asm 1: subcc <bytes=int64#3,64,>bytes=int64#5
# asm 2: subcc <bytes=%i2,64,>bytes=%i4
subcc %i2,64,%i4

# qhasm:   goto bytesatleast1 if unsigned>
bgu,pt %xcc,._bytesatleast1
nop

# qhasm:   goto done if !unsigned<
bgeu,pt %xcc,._done
nop

# qhasm:     m = ctarget
# asm 1: ldx [%fp+2023-<ctarget=stack64#1],>m=int64#1
# asm 2: ldx [%fp+2023-<ctarget=0],>m=%i0
ldx [%fp+2023-0],%i0

# qhasm:     bytes += 64
# asm 1: add <bytes=int64#5,64,>bytes=int64#3
# asm 2: add <bytes=%i4,64,>bytes=%i2
add %i4,64,%i2

# qhasm:     out -= 64
# asm 1: sub <out=int64#2,64,>out=int64#2
# asm 2: sub <out=%i1,64,>out=%i1
sub %i1,64,%i1

# qhasm:     i = 0
# asm 1: add %g0,0,>i=int64#4
# asm 2: add %g0,0,>i=%i3
add %g0,0,%i3

# qhasm:     ccopyloop:
._ccopyloop:

# qhasm:       a = *(int8 *) (out + i)
# asm 1: ldsb [<out=int64#2+<i=int64#4],>a=int64#5
# asm 2: ldsb [<out=%i1+<i=%i3],>a=%i4
ldsb [%i1+%i3],%i4

# qhasm:       *(int8 *) (m + i) = a
# asm 1: stb <a=int64#5,[<m=int64#1+<i=int64#4]
# asm 2: stb <a=%i4,[<m=%i0+<i=%i3]
stb %i4,[%i0+%i3]

# qhasm:       i += 1
# asm 1: add <i=int64#4,1,>i=int64#4
# asm 2: add <i=%i3,1,>i=%i3
add %i3,1,%i3

# qhasm:                       unsigned<? i - bytes
# asm 1: subcc <i=int64#4,<bytes=int64#3,%g0
# asm 2: subcc <i=%i3,<bytes=%i2,%g0
subcc %i3,%i2,%g0

# qhasm:     goto ccopyloop if unsigned<
blu,pt %xcc,._ccopyloop
nop

# qhasm: done:
._done:

# qhasm: leave
ret
restore
