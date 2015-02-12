# version 20090331
# Peter Schwabe & Neil Costigan
# Public domain.

vec128 retp
vec128 inp

input retp
input inp

vec128 in0
vec128 in1

vec128 out_03
vec128 out_47
vec128 out_811
vec128 out_1215
vec128 out_1619

vec128 dummy

vec128 swapendian

vec128 call0
vec128 call1
vec128 call2
vec128 call3
vec128 call4
vec128 call5
vec128 call6
vec128 call7
vec128 call8
vec128 call9
vec128 call10
vec128 call11
vec128 call12
vec128 call13
vec128 call14
vec128 call15
vec128 call16
vec128 call17
vec128 call18
vec128 call19
vec128 call20
vec128 call21
vec128 call22
vec128 call23
vec128 call24
vec128 call25
vec128 call26
vec128 call27
vec128 call28
vec128 call29
vec128 call30
vec128 call31
vec128 call32
vec128 call33
vec128 call34
vec128 call35
vec128 call36
vec128 call37
vec128 call38
vec128 call39
vec128 call40
vec128 call41
vec128 call42
vec128 call43
vec128 call44
vec128 call45
vec128 call46
vec128 call47

caller call0
caller call1
caller call2
caller call3
caller call4
caller call5
caller call6
caller call7
caller call8
caller call9
caller call10
caller call11
caller call12
caller call13
caller call14
caller call15
caller call16
caller call17
caller call18
caller call19
caller call20
caller call21
caller call22
caller call23
caller call24
caller call25
caller call26
caller call27
caller call28
caller call29
caller call30
caller call31
caller call32
caller call33
caller call34
caller call35
caller call36
caller call37
caller call38
caller call39
caller call40
caller call41
caller call42
caller call43
caller call44
caller call45
caller call46
caller call47


enter tored

swapendian = extern(_swapendian)

in1 = *(vec128 *) ((inp + 0) & ~15)
in0 = *(vec128 *) ((inp + 16) & ~15)

in0 = select bytes from in0 by swapendian
in1 = select bytes from in1 by swapendian

dummy = in1 << (8 * 14)
dummy <<= (3 % 8)
dummy >>= (3 % 8)
out_03 = dummy >> (8 * 2)
in1 >>= (8 * 1)
in1 >>= (5 % 8)

dummy = in1 << (8 * 14)
dummy <<= (3 % 8)
dummy >>= (3 % 8)
dummy >>= (8 * 6)
out_03 ^= dummy
in1 >>= (8 * 1)
in1 >>= (5 % 8)

dummy = in1 << (8 * 14)
dummy <<= (3 % 8)
dummy >>= (3 % 8)
dummy >>= (8 * 10)
out_03 ^= dummy
in1 >>= (8 * 1)
in1 >>= (5 % 8)

dummy = in1 << (8 * 14)
dummy <<= (4 % 8)
dummy >>= (4 % 8)
dummy >>= (8 * 14)
out_03 ^= dummy
in1 >>= (8 * 1)
in1 >>= (4 % 8)


dummy = in1 << (8 * 14)
dummy <<= (3 % 8)
dummy >>= (3 % 8)
out_47 = dummy >> (8 * 2)
in1 >>= (8 * 1)
in1 >>= (5 % 8)

dummy = in1 << (8 * 14)
dummy <<= (3 % 8)
dummy >>= (3 % 8)
dummy >>= (8 * 6)
out_47 ^= dummy
in1 >>= (8 * 1)
in1 >>= (5 % 8)

dummy = in1 << (8 * 14)
dummy <<= (3 % 8)
dummy >>= (3 % 8)
dummy >>= (8 * 10)
out_47 ^= dummy
in1 >>= (8 * 1)
in1 >>= (5 % 8)

dummy = in1 << (8 * 14)
dummy <<= (4 % 8)
dummy >>= (4 % 8)
dummy >>= (8 * 14)
out_47 ^= dummy
in1 >>= (8 * 1)
in1 >>= (4 % 8)


dummy = in1 << (8 * 14)
dummy <<= (3 % 8)
dummy >>= (3 % 8)
out_811 = dummy >> (8 * 2)
in1 >>= (8 * 1)
in1 >>= (5 % 8)

dummy = in1 << (8 * 14)
dummy <<= (3 % 8)
dummy >>= (3 % 8)
dummy >>= (8 * 6)
out_811 ^= dummy
in1 >>= (8 * 1)
in1 >>= (5 % 8)

dummy = in0 << (8 * 14)
dummy <<= (3 % 8)
dummy >>= (3 % 8)
dummy >>= (8 * 10)
out_811 ^= dummy
in0 >>= (8 * 1)
in0 >>= (5 % 8)

dummy = in0 << (8 * 14)
dummy <<= (4 % 8)
dummy >>= (4 % 8)
dummy >>= (8 * 14)
out_811 ^= dummy
in0 >>= (8 * 1)
in0 >>= (4 % 8)


dummy = in0 << (8 * 14)
dummy <<= (3 % 8)
dummy >>= (3 % 8)
out_1215 = dummy >> (8 * 2)
in0 >>= (8 * 1)
in0 >>= (5 % 8)

dummy = in0 << (8 * 14)
dummy <<= (3 % 8)
dummy >>= (3 % 8)
dummy >>= (8 * 6)
out_1215 ^= dummy
in0 >>= (8 * 1)
in0 >>= (5 % 8)

dummy = in0 << (8 * 14)
dummy <<= (3 % 8)
dummy >>= (3 % 8)
dummy >>= (8 * 10)
out_1215 ^= dummy
in0 >>= (8 * 1)
in0 >>= (5 % 8)

dummy = in0 << (8 * 14)
dummy <<= (4 % 8)
dummy >>= (4 % 8)
dummy >>= (8 * 14)
out_1215 ^= dummy
in0 >>= (8 * 1)
in0 >>= (4 % 8)


dummy = in0 << (8 * 14)
dummy <<= (3 % 8)
dummy >>= (3 % 8)
out_1619 = dummy >> (8 * 2)
in0 >>= (8 * 1)
in0 >>= (5 % 8)

dummy = in0 << (8 * 14)
dummy <<= (3 % 8)
dummy >>= (3 % 8)
dummy >>= (8 * 6)
out_1619 ^= dummy
in0 >>= (8 * 1)
in0 >>= (5 % 8)

dummy = in0 << (8 * 14)
dummy <<= (3 % 8)
dummy >>= (3 % 8)
dummy >>= (8 * 10)
out_1619 ^= dummy
in0 >>= (8 * 1)
in0 >>= (5 % 8)

dummy = in0 << (8 * 14)
dummy <<= (3 % 8)
dummy >>= (3 % 8)
dummy >>= (8 * 14)
out_1619  ^= dummy


*(vec128 *) ((retp + 0) & ~15) = out_03
*(vec128 *) ((retp + 16) & ~15) = out_47
*(vec128 *) ((retp + 32) & ~15)= out_811
*(vec128 *) ((retp + 48) & ~15) = out_1215
*(vec128 *) ((retp + 64) & ~15) = out_1619

leave
