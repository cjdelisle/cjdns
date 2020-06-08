# version 20090331
# Peter Schwabe & Neil Costigan
# Public domain.

vec128 retp
vec128 inp

input retp
input inp

vec128 in_03
vec128 in_47
vec128 in_811
vec128 in_1215
vec128 in_1619

vec128 out0
vec128 out0s
vec128 out1
vec128 out1s
vec128 dummy
vec128 c

vec128 p_lower
vec128 p_upper
vec128 zero
vec128 nmsbit

vec128 b0
vec128 b1

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

enter fromred

swapendian = extern(_swapendian)
p_lower = extern(_p_lower)
p_upper = extern(_p_upper)
nmsbit = extern(_nmsbit)
int32323232 zero = 0

#DEBUG
int32323232 out1 = 0

in_03 = *(vec128 *) ((inp + 0) & ~15)
in_47 = *(vec128 *) ((inp + 16) & ~15)
in_811 = *(vec128 *) ((inp + 32) & ~15)
in_1215 = *(vec128 *) ((inp + 48) & ~15)
in_1619 = *(vec128 *) ((inp + 64) & ~15)

# Bits 0-12 in word0
out0 = in_03 >> (8 * 12)

# Bits 13-25 in word0
in_03 <<= (8 * 4)
dummy = in_03 >> (8 * 10)
dummy >>= (3 % 8)
int32323232 out0 += dummy

# Bits 26-31 in word0 (+carry in word1, TODO: check if necessary...)
in_03 <<= (8 * 4)
dummy = in_03 << (8 * 3)
dummy <<= (2 % 8)
dummy >>= (8 * 15)
dummy <<= (8 * 3)
int32323232 c = carry(out0 + dummy)
int32323232 out0 += dummy
c <<= (8 * 4)
int32323232 out0 += c

# Bits 0-6 in word1
dummy = in_03 >> (8 * 12)
dummy >>= (6 % 8)
dummy <<= (8 * 4)
int32323232 out0 += dummy

# Bits 7-19 in word1 (7-18 in pure reduced)
in_03 <<= (8 * 4)
dummy = in_03 >> (8 * 7)
dummy  >>= (1 % 8)
int32323232 out0 += dummy

# Bits 19-31 in word1 (+carry in word2)
dummy = in_47 >> (8 * 12)
dummy <<= (8 * 6)
dummy <<= (3 % 8)
int32323232 c = carry(out0 + dummy)
int32323232 out0 += dummy
c <<= (8 * 4)
int32323232 out0 += c

# Bits 0-12 in word2
in_47 <<= (8 * 4)
dummy = in_47 >> (8 * 12)
dummy <<= (8 * 8)
int32323232 out0 += dummy

# Bits 13-25 in word2
in_47 <<= (8 * 4)
dummy = in_47 >> (8 * 12)
dummy <<= (8 * 9)
dummy <<= (5 % 8)
int32323232 out0 += dummy

# Bits 26-31 in word2 (+carry in word3)
in_47 <<= (8 * 4)
dummy = in_47 << (8 * 3)
dummy <<= (2 % 8)
dummy >>= (8 * 4)
int32323232 c = carry(out0 + dummy)
int32323232 out0 += dummy
c <<= (8 * 4)
int32323232 out0 += c

# Bits 0-6 in word3 (0-5 in pure reduced)
dummy = in_47 >> (8 * 12)
dummy >>= (6 % 8)
dummy <<= (8 * 12)
int32323232 out0 += dummy

# Bits 6-18 in word3
dummy = in_811 >> (8 * 12)
dummy <<= (8 * 12)
dummy <<= (6 % 8)
int32323232 out0 += dummy

# Bits 19-31 in word3 (+ carry in word4)
in_811 <<= (8 * 4)
dummy = in_811 >> (8 * 12)
dummy <<= (8 * 14)
dummy <<= (3 % 8)
int32323232 c = carry(out0 + dummy)
int32323232 out0 += dummy

# Bits 0-12 in word4
out1 = c >> (8 * 12)
in_811 <<= (8 * 4)
dummy = in_811 >> (8 * 12)
int32323232 out1 += dummy

# Bits 13-25 in word4 (13-24 in pure reduced)
in_811 <<= (8 * 4)
dummy = in_811 >> (8 * 10)
dummy >>= (3 % 8)
int32323232 out1 += dummy

# Bits 25-31 in word4 (+ carry in word5)
dummy = in_1215 >> (8*12)
dummy <<= (8 * 15)
dummy <<= (1 % 8)
dummy >>= (8 * 12)
int32323232 c = carry(dummy + out1)
int32323232 out1 += dummy
c <<= (8 * 4)
int32323232 out1 += c

# Bits 0-5 in word 5
dummy = in_1215 >> (8 * 12)
dummy >>= (7 % 8) 
dummy <<= (8 * 4)
int32323232 out1 +=  dummy

# Bits 6-18 in word5
in_1215 <<= (8 * 4)
dummy = in_1215 >> (8 * 12)
dummy <<= (8 * 4)
dummy <<= (6 % 8)
int32323232 out1 += dummy

# Bits 19-31 in word5 (+carry in word6)
in_1215 <<= (8 * 4)
dummy = in_1215 >> (8 * 12)
dummy <<= (8 * 6)
dummy <<= (3 % 8)
int32323232 c = carry(out1 + dummy)
int32323232 out1 += dummy
c <<= (8 * 4)
int32323232 out1 += c

# Bits 0-12 in word6 (0-11 in pure reduced)
in_1215 <<= (8 * 4)
dummy = in_1215 >> (8 * 4)
int32323232 out1 += dummy

# Bits 12-24 in word6
dummy = in_1619 >> (8 * 12)
dummy <<= (8 * 9)
dummy <<= (4 % 8)
int32323232 out1 += dummy

# Bits 25-31 in word6 (+carry in word7)
in_1619 <<= (8 * 4)
dummy = in_1619 >> (8 * 12)
dummy <<= (8 * 15)
dummy <<= (1 % 8)
dummy >>= (8 * 4)
int32323232 c = carry(dummy + out1)
int32323232 out1 += dummy
c <<= (8 * 4)
int32323232 out1 += c

# Bits 0-5 in word7 
dummy = in_1619 >> (8 * 12)
dummy >>= (7 % 8) 
dummy <<= (8 * 12)
int32323232 out1 +=  dummy

# Bits 6-18 in word7
in_1619 <<= (8 * 4)
dummy = in_1619 >> (8 * 12)
dummy <<= (8 * 12)
dummy <<= (6 % 8)
int32323232 out1 += dummy

# Bits 19-31 in word7 (+carry)
in_1619 <<= (8 * 4)
dummy = in_1619 >> (8 * 12)
dummy <<= (8 * 14)
dummy <<= (3 % 8)
int32323232 c = carry(out1 + dummy)
int32323232 out1 += dummy
c >>= (8 * 12)

# Reduce mod 2^255-19

# Handle carry from previous addition (2^256)
int32323232 dummy = (c & 0xffff) * 38
int32323232 c = carry(out0 + dummy)
int32323232 out0 += dummy
dummy = c << (8 * 4)
int32323232 out0 += dummy

# Handle highest bit (must be 0)
c = out1 >> (8 * 15)
c = c >> (7 % 8)
int32323232 dummy = (c & 0xffff) * 19
int32323232 c = carry(out0 + dummy)
int32323232 out0 += dummy
dummy = c << (8 * 4)
int32323232 out0 += dummy
out1 &= nmsbit

# Now we're smaller than 2^255, but possibly still larger than 2^255-19
int32323232 out0s = out0 - p_upper
int32323232 out1s = out1 - p_lower

uint32323232 b0 = borrow(p_upper - out0)
uint32323232 b1 = borrow(p_lower - out0)
#
uint32323232 b0 = b0[0] | b0[1] | b0[2] | b0[3]
uint32323232 b1 = b0[0] | b0[1] | b0[2] | b0[3]

b0 = b0 | b1
int32323232 b0 = zero - b0
dummy = b0 <<< (8 * 4)
b0 ^= dummy
dummy = b0 <<< (8 * 8)
b0 ^= dummy
b1 = ~(zero | b0)
out0 &= b1
out1 &= b1
out0s &= b0
out1s &= b0
#
out0 = out0 ^ out0s
out1 = out1 ^ out1s
#
out0 = select bytes from out0 by swapendian
out1 = select bytes from out1 by swapendian

*(vec128 *) ((retp + 0) & ~15) = out0
*(vec128 *) ((retp + 16) & ~15) = out1

leave
