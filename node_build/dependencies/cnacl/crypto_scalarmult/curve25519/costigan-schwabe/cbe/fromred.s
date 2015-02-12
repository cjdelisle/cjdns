# version 20090331
# Peter Schwabe & Neil Costigan
# Public domain.

# qhasm: vec128 retp

# qhasm: vec128 inp

# qhasm: input retp

# qhasm: input inp

# qhasm: vec128 in_03

# qhasm: vec128 in_47

# qhasm: vec128 in_811

# qhasm: vec128 in_1215

# qhasm: vec128 in_1619

# qhasm: vec128 out0

# qhasm: vec128 out0s

# qhasm: vec128 out1

# qhasm: vec128 out1s

# qhasm: vec128 dummy

# qhasm: vec128 c

# qhasm: vec128 p_lower

# qhasm: vec128 p_upper

# qhasm: vec128 zero

# qhasm: vec128 nmsbit

# qhasm: vec128 b0

# qhasm: vec128 b1

# qhasm: vec128 swapendian

# qhasm: vec128 call0

# qhasm: vec128 call1

# qhasm: vec128 call2

# qhasm: vec128 call3

# qhasm: vec128 call4

# qhasm: vec128 call5

# qhasm: vec128 call6

# qhasm: vec128 call7

# qhasm: vec128 call8

# qhasm: vec128 call9

# qhasm: vec128 call10

# qhasm: vec128 call11

# qhasm: vec128 call12

# qhasm: vec128 call13

# qhasm: vec128 call14

# qhasm: vec128 call15

# qhasm: vec128 call16

# qhasm: vec128 call17

# qhasm: vec128 call18

# qhasm: vec128 call19

# qhasm: vec128 call20

# qhasm: vec128 call21

# qhasm: vec128 call22

# qhasm: vec128 call23

# qhasm: vec128 call24

# qhasm: vec128 call25

# qhasm: vec128 call26

# qhasm: vec128 call27

# qhasm: vec128 call28

# qhasm: vec128 call29

# qhasm: vec128 call30

# qhasm: vec128 call31

# qhasm: vec128 call32

# qhasm: vec128 call33

# qhasm: vec128 call34

# qhasm: vec128 call35

# qhasm: vec128 call36

# qhasm: vec128 call37

# qhasm: vec128 call38

# qhasm: vec128 call39

# qhasm: vec128 call40

# qhasm: vec128 call41

# qhasm: vec128 call42

# qhasm: vec128 call43

# qhasm: vec128 call44

# qhasm: vec128 call45

# qhasm: vec128 call46

# qhasm: vec128 call47

# qhasm: caller call0

# qhasm: caller call1

# qhasm: caller call2

# qhasm: caller call3

# qhasm: caller call4

# qhasm: caller call5

# qhasm: caller call6

# qhasm: caller call7

# qhasm: caller call8

# qhasm: caller call9

# qhasm: caller call10

# qhasm: caller call11

# qhasm: caller call12

# qhasm: caller call13

# qhasm: caller call14

# qhasm: caller call15

# qhasm: caller call16

# qhasm: caller call17

# qhasm: caller call18

# qhasm: caller call19

# qhasm: caller call20

# qhasm: caller call21

# qhasm: caller call22

# qhasm: caller call23

# qhasm: caller call24

# qhasm: caller call25

# qhasm: caller call26

# qhasm: caller call27

# qhasm: caller call28

# qhasm: caller call29

# qhasm: caller call30

# qhasm: caller call31

# qhasm: caller call32

# qhasm: caller call33

# qhasm: caller call34

# qhasm: caller call35

# qhasm: caller call36

# qhasm: caller call37

# qhasm: caller call38

# qhasm: caller call39

# qhasm: caller call40

# qhasm: caller call41

# qhasm: caller call42

# qhasm: caller call43

# qhasm: caller call44

# qhasm: caller call45

# qhasm: caller call46

# qhasm: caller call47

# qhasm: enter fromred
.text
.align 3
.globl _fromred
.global fromred
.type fromred, @function
fromred:
stqd $lr, 16($sp)
stqd $sp,-32($sp)
ai $sp,$sp,-32

# qhasm: swapendian = extern(_swapendian)
# asm 1: lqr <swapendian=vec128#3,_swapendian
# asm 2: lqr <swapendian=$5,_swapendian
lqr $5,_swapendian

# qhasm: p_lower = extern(_p_lower)
# asm 1: lqr <p_lower=vec128#4,_p_lower
# asm 2: lqr <p_lower=$6,_p_lower
lqr $6,_p_lower

# qhasm: p_upper = extern(_p_upper)
# asm 1: lqr <p_upper=vec128#5,_p_upper
# asm 2: lqr <p_upper=$7,_p_upper
lqr $7,_p_upper

# qhasm: nmsbit = extern(_nmsbit)
# asm 1: lqr <nmsbit=vec128#6,_nmsbit
# asm 2: lqr <nmsbit=$8,_nmsbit
lqr $8,_nmsbit

# qhasm: int32323232 zero = 0
# asm 1: il >zero=vec128#7,0
# asm 2: il >zero=$9,0
il $9,0

# qhasm: int32323232 out1 = 0
# asm 1: il >out1=vec128#8,0
# asm 2: il >out1=$10,0
il $10,0

# qhasm: in_03 = *(vec128 *) ((inp + 0) & ~15)
# asm 1: lqd >in_03=vec128#9,0(<inp=vec128#2)
# asm 2: lqd >in_03=$11,0(<inp=$4)
lqd $11,0($4)

# qhasm: in_47 = *(vec128 *) ((inp + 16) & ~15)
# asm 1: lqd >in_47=vec128#10,16(<inp=vec128#2)
# asm 2: lqd >in_47=$12,16(<inp=$4)
lqd $12,16($4)

# qhasm: in_811 = *(vec128 *) ((inp + 32) & ~15)
# asm 1: lqd >in_811=vec128#11,32(<inp=vec128#2)
# asm 2: lqd >in_811=$13,32(<inp=$4)
lqd $13,32($4)

# qhasm: in_1215 = *(vec128 *) ((inp + 48) & ~15)
# asm 1: lqd >in_1215=vec128#12,48(<inp=vec128#2)
# asm 2: lqd >in_1215=$14,48(<inp=$4)
lqd $14,48($4)

# qhasm: in_1619 = *(vec128 *) ((inp + 64) & ~15)
# asm 1: lqd >in_1619=vec128#2,64(<inp=vec128#2)
# asm 2: lqd >in_1619=$4,64(<inp=$4)
lqd $4,64($4)

# qhasm: out0 = in_03 >> (8 * 12)
# asm 1: rotqmbyi >out0=vec128#13,<in_03=vec128#9,-12
# asm 2: rotqmbyi >out0=$15,<in_03=$11,-12
rotqmbyi $15,$11,-12

# qhasm: in_03 <<= (8 * 4)
# asm 1: shlqbyi >in_03=vec128#9,<in_03=vec128#9,4
# asm 2: shlqbyi >in_03=$11,<in_03=$11,4
shlqbyi $11,$11,4

# qhasm: dummy = in_03 >> (8 * 10)
# asm 1: rotqmbyi >dummy=vec128#14,<in_03=vec128#9,-10
# asm 2: rotqmbyi >dummy=$16,<in_03=$11,-10
rotqmbyi $16,$11,-10

# qhasm: dummy >>= (3 % 8)
# asm 1: rotqmbii >dummy=vec128#14,<dummy=vec128#14,-3
# asm 2: rotqmbii >dummy=$16,<dummy=$16,-3
rotqmbii $16,$16,-3

# qhasm: int32323232 out0 += dummy
# asm 1: a >out0=vec128#13,<out0=vec128#13,<dummy=vec128#14
# asm 2: a >out0=$15,<out0=$15,<dummy=$16
a $15,$15,$16

# qhasm: in_03 <<= (8 * 4)
# asm 1: shlqbyi >in_03=vec128#9,<in_03=vec128#9,4
# asm 2: shlqbyi >in_03=$11,<in_03=$11,4
shlqbyi $11,$11,4

# qhasm: dummy = in_03 << (8 * 3)
# asm 1: shlqbyi >dummy=vec128#14,<in_03=vec128#9,3
# asm 2: shlqbyi >dummy=$16,<in_03=$11,3
shlqbyi $16,$11,3

# qhasm: dummy <<= (2 % 8)
# asm 1: shlqbii >dummy=vec128#14,<dummy=vec128#14,2
# asm 2: shlqbii >dummy=$16,<dummy=$16,2
shlqbii $16,$16,2

# qhasm: dummy >>= (8 * 15)
# asm 1: rotqmbyi >dummy=vec128#14,<dummy=vec128#14,-15
# asm 2: rotqmbyi >dummy=$16,<dummy=$16,-15
rotqmbyi $16,$16,-15

# qhasm: dummy <<= (8 * 3)
# asm 1: shlqbyi >dummy=vec128#14,<dummy=vec128#14,3
# asm 2: shlqbyi >dummy=$16,<dummy=$16,3
shlqbyi $16,$16,3

# qhasm: int32323232 c = carry(out0 + dummy)
# asm 1: cg >c=vec128#15,<out0=vec128#13,<dummy=vec128#14
# asm 2: cg >c=$17,<out0=$15,<dummy=$16
cg $17,$15,$16

# qhasm: int32323232 out0 += dummy
# asm 1: a >out0=vec128#13,<out0=vec128#13,<dummy=vec128#14
# asm 2: a >out0=$15,<out0=$15,<dummy=$16
a $15,$15,$16

# qhasm: c <<= (8 * 4)
# asm 1: shlqbyi >c=vec128#14,<c=vec128#15,4
# asm 2: shlqbyi >c=$16,<c=$17,4
shlqbyi $16,$17,4

# qhasm: int32323232 out0 += c
# asm 1: a >out0=vec128#13,<out0=vec128#13,<c=vec128#14
# asm 2: a >out0=$15,<out0=$15,<c=$16
a $15,$15,$16

# qhasm: dummy = in_03 >> (8 * 12)
# asm 1: rotqmbyi >dummy=vec128#14,<in_03=vec128#9,-12
# asm 2: rotqmbyi >dummy=$16,<in_03=$11,-12
rotqmbyi $16,$11,-12

# qhasm: dummy >>= (6 % 8)
# asm 1: rotqmbii >dummy=vec128#14,<dummy=vec128#14,-6
# asm 2: rotqmbii >dummy=$16,<dummy=$16,-6
rotqmbii $16,$16,-6

# qhasm: dummy <<= (8 * 4)
# asm 1: shlqbyi >dummy=vec128#14,<dummy=vec128#14,4
# asm 2: shlqbyi >dummy=$16,<dummy=$16,4
shlqbyi $16,$16,4

# qhasm: int32323232 out0 += dummy
# asm 1: a >out0=vec128#13,<out0=vec128#13,<dummy=vec128#14
# asm 2: a >out0=$15,<out0=$15,<dummy=$16
a $15,$15,$16

# qhasm: in_03 <<= (8 * 4)
# asm 1: shlqbyi >in_03=vec128#9,<in_03=vec128#9,4
# asm 2: shlqbyi >in_03=$11,<in_03=$11,4
shlqbyi $11,$11,4

# qhasm: dummy = in_03 >> (8 * 7)
# asm 1: rotqmbyi >dummy=vec128#9,<in_03=vec128#9,-7
# asm 2: rotqmbyi >dummy=$11,<in_03=$11,-7
rotqmbyi $11,$11,-7

# qhasm: dummy  >>= (1 % 8)
# asm 1: rotqmbii >dummy=vec128#9,<dummy=vec128#9,-1
# asm 2: rotqmbii >dummy=$11,<dummy=$11,-1
rotqmbii $11,$11,-1

# qhasm: int32323232 out0 += dummy
# asm 1: a >out0=vec128#9,<out0=vec128#13,<dummy=vec128#9
# asm 2: a >out0=$11,<out0=$15,<dummy=$11
a $11,$15,$11

# qhasm: dummy = in_47 >> (8 * 12)
# asm 1: rotqmbyi >dummy=vec128#13,<in_47=vec128#10,-12
# asm 2: rotqmbyi >dummy=$15,<in_47=$12,-12
rotqmbyi $15,$12,-12

# qhasm: dummy <<= (8 * 6)
# asm 1: shlqbyi >dummy=vec128#13,<dummy=vec128#13,6
# asm 2: shlqbyi >dummy=$15,<dummy=$15,6
shlqbyi $15,$15,6

# qhasm: dummy <<= (3 % 8)
# asm 1: shlqbii >dummy=vec128#13,<dummy=vec128#13,3
# asm 2: shlqbii >dummy=$15,<dummy=$15,3
shlqbii $15,$15,3

# qhasm: int32323232 c = carry(out0 + dummy)
# asm 1: cg >c=vec128#14,<out0=vec128#9,<dummy=vec128#13
# asm 2: cg >c=$16,<out0=$11,<dummy=$15
cg $16,$11,$15

# qhasm: int32323232 out0 += dummy
# asm 1: a >out0=vec128#9,<out0=vec128#9,<dummy=vec128#13
# asm 2: a >out0=$11,<out0=$11,<dummy=$15
a $11,$11,$15

# qhasm: c <<= (8 * 4)
# asm 1: shlqbyi >c=vec128#13,<c=vec128#14,4
# asm 2: shlqbyi >c=$15,<c=$16,4
shlqbyi $15,$16,4

# qhasm: int32323232 out0 += c
# asm 1: a >out0=vec128#9,<out0=vec128#9,<c=vec128#13
# asm 2: a >out0=$11,<out0=$11,<c=$15
a $11,$11,$15

# qhasm: in_47 <<= (8 * 4)
# asm 1: shlqbyi >in_47=vec128#10,<in_47=vec128#10,4
# asm 2: shlqbyi >in_47=$12,<in_47=$12,4
shlqbyi $12,$12,4

# qhasm: dummy = in_47 >> (8 * 12)
# asm 1: rotqmbyi >dummy=vec128#13,<in_47=vec128#10,-12
# asm 2: rotqmbyi >dummy=$15,<in_47=$12,-12
rotqmbyi $15,$12,-12

# qhasm: dummy <<= (8 * 8)
# asm 1: shlqbyi >dummy=vec128#13,<dummy=vec128#13,8
# asm 2: shlqbyi >dummy=$15,<dummy=$15,8
shlqbyi $15,$15,8

# qhasm: int32323232 out0 += dummy
# asm 1: a >out0=vec128#9,<out0=vec128#9,<dummy=vec128#13
# asm 2: a >out0=$11,<out0=$11,<dummy=$15
a $11,$11,$15

# qhasm: in_47 <<= (8 * 4)
# asm 1: shlqbyi >in_47=vec128#10,<in_47=vec128#10,4
# asm 2: shlqbyi >in_47=$12,<in_47=$12,4
shlqbyi $12,$12,4

# qhasm: dummy = in_47 >> (8 * 12)
# asm 1: rotqmbyi >dummy=vec128#13,<in_47=vec128#10,-12
# asm 2: rotqmbyi >dummy=$15,<in_47=$12,-12
rotqmbyi $15,$12,-12

# qhasm: dummy <<= (8 * 9)
# asm 1: shlqbyi >dummy=vec128#13,<dummy=vec128#13,9
# asm 2: shlqbyi >dummy=$15,<dummy=$15,9
shlqbyi $15,$15,9

# qhasm: dummy <<= (5 % 8)
# asm 1: shlqbii >dummy=vec128#13,<dummy=vec128#13,5
# asm 2: shlqbii >dummy=$15,<dummy=$15,5
shlqbii $15,$15,5

# qhasm: int32323232 out0 += dummy
# asm 1: a >out0=vec128#9,<out0=vec128#9,<dummy=vec128#13
# asm 2: a >out0=$11,<out0=$11,<dummy=$15
a $11,$11,$15

# qhasm: in_47 <<= (8 * 4)
# asm 1: shlqbyi >in_47=vec128#10,<in_47=vec128#10,4
# asm 2: shlqbyi >in_47=$12,<in_47=$12,4
shlqbyi $12,$12,4

# qhasm: dummy = in_47 << (8 * 3)
# asm 1: shlqbyi >dummy=vec128#13,<in_47=vec128#10,3
# asm 2: shlqbyi >dummy=$15,<in_47=$12,3
shlqbyi $15,$12,3

# qhasm: dummy <<= (2 % 8)
# asm 1: shlqbii >dummy=vec128#13,<dummy=vec128#13,2
# asm 2: shlqbii >dummy=$15,<dummy=$15,2
shlqbii $15,$15,2

# qhasm: dummy >>= (8 * 4)
# asm 1: rotqmbyi >dummy=vec128#13,<dummy=vec128#13,-4
# asm 2: rotqmbyi >dummy=$15,<dummy=$15,-4
rotqmbyi $15,$15,-4

# qhasm: int32323232 c = carry(out0 + dummy)
# asm 1: cg >c=vec128#14,<out0=vec128#9,<dummy=vec128#13
# asm 2: cg >c=$16,<out0=$11,<dummy=$15
cg $16,$11,$15

# qhasm: int32323232 out0 += dummy
# asm 1: a >out0=vec128#9,<out0=vec128#9,<dummy=vec128#13
# asm 2: a >out0=$11,<out0=$11,<dummy=$15
a $11,$11,$15

# qhasm: c <<= (8 * 4)
# asm 1: shlqbyi >c=vec128#13,<c=vec128#14,4
# asm 2: shlqbyi >c=$15,<c=$16,4
shlqbyi $15,$16,4

# qhasm: int32323232 out0 += c
# asm 1: a >out0=vec128#9,<out0=vec128#9,<c=vec128#13
# asm 2: a >out0=$11,<out0=$11,<c=$15
a $11,$11,$15

# qhasm: dummy = in_47 >> (8 * 12)
# asm 1: rotqmbyi >dummy=vec128#10,<in_47=vec128#10,-12
# asm 2: rotqmbyi >dummy=$12,<in_47=$12,-12
rotqmbyi $12,$12,-12

# qhasm: dummy >>= (6 % 8)
# asm 1: rotqmbii >dummy=vec128#10,<dummy=vec128#10,-6
# asm 2: rotqmbii >dummy=$12,<dummy=$12,-6
rotqmbii $12,$12,-6

# qhasm: dummy <<= (8 * 12)
# asm 1: shlqbyi >dummy=vec128#10,<dummy=vec128#10,12
# asm 2: shlqbyi >dummy=$12,<dummy=$12,12
shlqbyi $12,$12,12

# qhasm: int32323232 out0 += dummy
# asm 1: a >out0=vec128#9,<out0=vec128#9,<dummy=vec128#10
# asm 2: a >out0=$11,<out0=$11,<dummy=$12
a $11,$11,$12

# qhasm: dummy = in_811 >> (8 * 12)
# asm 1: rotqmbyi >dummy=vec128#10,<in_811=vec128#11,-12
# asm 2: rotqmbyi >dummy=$12,<in_811=$13,-12
rotqmbyi $12,$13,-12

# qhasm: dummy <<= (8 * 12)
# asm 1: shlqbyi >dummy=vec128#10,<dummy=vec128#10,12
# asm 2: shlqbyi >dummy=$12,<dummy=$12,12
shlqbyi $12,$12,12

# qhasm: dummy <<= (6 % 8)
# asm 1: shlqbii >dummy=vec128#10,<dummy=vec128#10,6
# asm 2: shlqbii >dummy=$12,<dummy=$12,6
shlqbii $12,$12,6

# qhasm: int32323232 out0 += dummy
# asm 1: a >out0=vec128#9,<out0=vec128#9,<dummy=vec128#10
# asm 2: a >out0=$11,<out0=$11,<dummy=$12
a $11,$11,$12

# qhasm: in_811 <<= (8 * 4)
# asm 1: shlqbyi >in_811=vec128#10,<in_811=vec128#11,4
# asm 2: shlqbyi >in_811=$12,<in_811=$13,4
shlqbyi $12,$13,4

# qhasm: dummy = in_811 >> (8 * 12)
# asm 1: rotqmbyi >dummy=vec128#11,<in_811=vec128#10,-12
# asm 2: rotqmbyi >dummy=$13,<in_811=$12,-12
rotqmbyi $13,$12,-12

# qhasm: dummy <<= (8 * 14)
# asm 1: shlqbyi >dummy=vec128#11,<dummy=vec128#11,14
# asm 2: shlqbyi >dummy=$13,<dummy=$13,14
shlqbyi $13,$13,14

# qhasm: dummy <<= (3 % 8)
# asm 1: shlqbii >dummy=vec128#11,<dummy=vec128#11,3
# asm 2: shlqbii >dummy=$13,<dummy=$13,3
shlqbii $13,$13,3

# qhasm: int32323232 c = carry(out0 + dummy)
# asm 1: cg >c=vec128#13,<out0=vec128#9,<dummy=vec128#11
# asm 2: cg >c=$15,<out0=$11,<dummy=$13
cg $15,$11,$13

# qhasm: int32323232 out0 += dummy
# asm 1: a >out0=vec128#9,<out0=vec128#9,<dummy=vec128#11
# asm 2: a >out0=$11,<out0=$11,<dummy=$13
a $11,$11,$13

# qhasm: out1 = c >> (8 * 12)
# asm 1: rotqmbyi >out1=vec128#11,<c=vec128#13,-12
# asm 2: rotqmbyi >out1=$13,<c=$15,-12
rotqmbyi $13,$15,-12

# qhasm: in_811 <<= (8 * 4)
# asm 1: shlqbyi >in_811=vec128#10,<in_811=vec128#10,4
# asm 2: shlqbyi >in_811=$12,<in_811=$12,4
shlqbyi $12,$12,4

# qhasm: dummy = in_811 >> (8 * 12)
# asm 1: rotqmbyi >dummy=vec128#13,<in_811=vec128#10,-12
# asm 2: rotqmbyi >dummy=$15,<in_811=$12,-12
rotqmbyi $15,$12,-12

# qhasm: int32323232 out1 += dummy
# asm 1: a >out1=vec128#11,<out1=vec128#11,<dummy=vec128#13
# asm 2: a >out1=$13,<out1=$13,<dummy=$15
a $13,$13,$15

# qhasm: in_811 <<= (8 * 4)
# asm 1: shlqbyi >in_811=vec128#10,<in_811=vec128#10,4
# asm 2: shlqbyi >in_811=$12,<in_811=$12,4
shlqbyi $12,$12,4

# qhasm: dummy = in_811 >> (8 * 10)
# asm 1: rotqmbyi >dummy=vec128#10,<in_811=vec128#10,-10
# asm 2: rotqmbyi >dummy=$12,<in_811=$12,-10
rotqmbyi $12,$12,-10

# qhasm: dummy >>= (3 % 8)
# asm 1: rotqmbii >dummy=vec128#10,<dummy=vec128#10,-3
# asm 2: rotqmbii >dummy=$12,<dummy=$12,-3
rotqmbii $12,$12,-3

# qhasm: int32323232 out1 += dummy
# asm 1: a >out1=vec128#10,<out1=vec128#11,<dummy=vec128#10
# asm 2: a >out1=$12,<out1=$13,<dummy=$12
a $12,$13,$12

# qhasm: dummy = in_1215 >> (8*12)
# asm 1: rotqmbyi >dummy=vec128#11,<in_1215=vec128#12,-12
# asm 2: rotqmbyi >dummy=$13,<in_1215=$14,-12
rotqmbyi $13,$14,-12

# qhasm: dummy <<= (8 * 15)
# asm 1: shlqbyi >dummy=vec128#11,<dummy=vec128#11,15
# asm 2: shlqbyi >dummy=$13,<dummy=$13,15
shlqbyi $13,$13,15

# qhasm: dummy <<= (1 % 8)
# asm 1: shlqbii >dummy=vec128#11,<dummy=vec128#11,1
# asm 2: shlqbii >dummy=$13,<dummy=$13,1
shlqbii $13,$13,1

# qhasm: dummy >>= (8 * 12)
# asm 1: rotqmbyi >dummy=vec128#11,<dummy=vec128#11,-12
# asm 2: rotqmbyi >dummy=$13,<dummy=$13,-12
rotqmbyi $13,$13,-12

# qhasm: int32323232 c = carry(dummy + out1)
# asm 1: cg >c=vec128#13,<dummy=vec128#11,<out1=vec128#10
# asm 2: cg >c=$15,<dummy=$13,<out1=$12
cg $15,$13,$12

# qhasm: int32323232 out1 += dummy
# asm 1: a >out1=vec128#10,<out1=vec128#10,<dummy=vec128#11
# asm 2: a >out1=$12,<out1=$12,<dummy=$13
a $12,$12,$13

# qhasm: c <<= (8 * 4)
# asm 1: shlqbyi >c=vec128#11,<c=vec128#13,4
# asm 2: shlqbyi >c=$13,<c=$15,4
shlqbyi $13,$15,4

# qhasm: int32323232 out1 += c
# asm 1: a >out1=vec128#10,<out1=vec128#10,<c=vec128#11
# asm 2: a >out1=$12,<out1=$12,<c=$13
a $12,$12,$13

# qhasm: dummy = in_1215 >> (8 * 12)
# asm 1: rotqmbyi >dummy=vec128#11,<in_1215=vec128#12,-12
# asm 2: rotqmbyi >dummy=$13,<in_1215=$14,-12
rotqmbyi $13,$14,-12

# qhasm: dummy >>= (7 % 8) 
# asm 1: rotqmbii >dummy=vec128#11,<dummy=vec128#11,-7
# asm 2: rotqmbii >dummy=$13,<dummy=$13,-7
rotqmbii $13,$13,-7

# qhasm: dummy <<= (8 * 4)
# asm 1: shlqbyi >dummy=vec128#11,<dummy=vec128#11,4
# asm 2: shlqbyi >dummy=$13,<dummy=$13,4
shlqbyi $13,$13,4

# qhasm: int32323232 out1 +=  dummy
# asm 1: a >out1=vec128#10,<out1=vec128#10,<dummy=vec128#11
# asm 2: a >out1=$12,<out1=$12,<dummy=$13
a $12,$12,$13

# qhasm: in_1215 <<= (8 * 4)
# asm 1: shlqbyi >in_1215=vec128#11,<in_1215=vec128#12,4
# asm 2: shlqbyi >in_1215=$13,<in_1215=$14,4
shlqbyi $13,$14,4

# qhasm: dummy = in_1215 >> (8 * 12)
# asm 1: rotqmbyi >dummy=vec128#12,<in_1215=vec128#11,-12
# asm 2: rotqmbyi >dummy=$14,<in_1215=$13,-12
rotqmbyi $14,$13,-12

# qhasm: dummy <<= (8 * 4)
# asm 1: shlqbyi >dummy=vec128#12,<dummy=vec128#12,4
# asm 2: shlqbyi >dummy=$14,<dummy=$14,4
shlqbyi $14,$14,4

# qhasm: dummy <<= (6 % 8)
# asm 1: shlqbii >dummy=vec128#12,<dummy=vec128#12,6
# asm 2: shlqbii >dummy=$14,<dummy=$14,6
shlqbii $14,$14,6

# qhasm: int32323232 out1 += dummy
# asm 1: a >out1=vec128#10,<out1=vec128#10,<dummy=vec128#12
# asm 2: a >out1=$12,<out1=$12,<dummy=$14
a $12,$12,$14

# qhasm: in_1215 <<= (8 * 4)
# asm 1: shlqbyi >in_1215=vec128#11,<in_1215=vec128#11,4
# asm 2: shlqbyi >in_1215=$13,<in_1215=$13,4
shlqbyi $13,$13,4

# qhasm: dummy = in_1215 >> (8 * 12)
# asm 1: rotqmbyi >dummy=vec128#12,<in_1215=vec128#11,-12
# asm 2: rotqmbyi >dummy=$14,<in_1215=$13,-12
rotqmbyi $14,$13,-12

# qhasm: dummy <<= (8 * 6)
# asm 1: shlqbyi >dummy=vec128#12,<dummy=vec128#12,6
# asm 2: shlqbyi >dummy=$14,<dummy=$14,6
shlqbyi $14,$14,6

# qhasm: dummy <<= (3 % 8)
# asm 1: shlqbii >dummy=vec128#12,<dummy=vec128#12,3
# asm 2: shlqbii >dummy=$14,<dummy=$14,3
shlqbii $14,$14,3

# qhasm: int32323232 c = carry(out1 + dummy)
# asm 1: cg >c=vec128#13,<out1=vec128#10,<dummy=vec128#12
# asm 2: cg >c=$15,<out1=$12,<dummy=$14
cg $15,$12,$14

# qhasm: int32323232 out1 += dummy
# asm 1: a >out1=vec128#10,<out1=vec128#10,<dummy=vec128#12
# asm 2: a >out1=$12,<out1=$12,<dummy=$14
a $12,$12,$14

# qhasm: c <<= (8 * 4)
# asm 1: shlqbyi >c=vec128#12,<c=vec128#13,4
# asm 2: shlqbyi >c=$14,<c=$15,4
shlqbyi $14,$15,4

# qhasm: int32323232 out1 += c
# asm 1: a >out1=vec128#10,<out1=vec128#10,<c=vec128#12
# asm 2: a >out1=$12,<out1=$12,<c=$14
a $12,$12,$14

# qhasm: in_1215 <<= (8 * 4)
# asm 1: shlqbyi >in_1215=vec128#11,<in_1215=vec128#11,4
# asm 2: shlqbyi >in_1215=$13,<in_1215=$13,4
shlqbyi $13,$13,4

# qhasm: dummy = in_1215 >> (8 * 4)
# asm 1: rotqmbyi >dummy=vec128#11,<in_1215=vec128#11,-4
# asm 2: rotqmbyi >dummy=$13,<in_1215=$13,-4
rotqmbyi $13,$13,-4

# qhasm: int32323232 out1 += dummy
# asm 1: a >out1=vec128#10,<out1=vec128#10,<dummy=vec128#11
# asm 2: a >out1=$12,<out1=$12,<dummy=$13
a $12,$12,$13

# qhasm: dummy = in_1619 >> (8 * 12)
# asm 1: rotqmbyi >dummy=vec128#11,<in_1619=vec128#2,-12
# asm 2: rotqmbyi >dummy=$13,<in_1619=$4,-12
rotqmbyi $13,$4,-12

# qhasm: dummy <<= (8 * 9)
# asm 1: shlqbyi >dummy=vec128#11,<dummy=vec128#11,9
# asm 2: shlqbyi >dummy=$13,<dummy=$13,9
shlqbyi $13,$13,9

# qhasm: dummy <<= (4 % 8)
# asm 1: shlqbii >dummy=vec128#11,<dummy=vec128#11,4
# asm 2: shlqbii >dummy=$13,<dummy=$13,4
shlqbii $13,$13,4

# qhasm: int32323232 out1 += dummy
# asm 1: a >out1=vec128#10,<out1=vec128#10,<dummy=vec128#11
# asm 2: a >out1=$12,<out1=$12,<dummy=$13
a $12,$12,$13

# qhasm: in_1619 <<= (8 * 4)
# asm 1: shlqbyi >in_1619=vec128#2,<in_1619=vec128#2,4
# asm 2: shlqbyi >in_1619=$4,<in_1619=$4,4
shlqbyi $4,$4,4

# qhasm: dummy = in_1619 >> (8 * 12)
# asm 1: rotqmbyi >dummy=vec128#11,<in_1619=vec128#2,-12
# asm 2: rotqmbyi >dummy=$13,<in_1619=$4,-12
rotqmbyi $13,$4,-12

# qhasm: dummy <<= (8 * 15)
# asm 1: shlqbyi >dummy=vec128#11,<dummy=vec128#11,15
# asm 2: shlqbyi >dummy=$13,<dummy=$13,15
shlqbyi $13,$13,15

# qhasm: dummy <<= (1 % 8)
# asm 1: shlqbii >dummy=vec128#11,<dummy=vec128#11,1
# asm 2: shlqbii >dummy=$13,<dummy=$13,1
shlqbii $13,$13,1

# qhasm: dummy >>= (8 * 4)
# asm 1: rotqmbyi >dummy=vec128#11,<dummy=vec128#11,-4
# asm 2: rotqmbyi >dummy=$13,<dummy=$13,-4
rotqmbyi $13,$13,-4

# qhasm: int32323232 c = carry(dummy + out1)
# asm 1: cg >c=vec128#12,<dummy=vec128#11,<out1=vec128#10
# asm 2: cg >c=$14,<dummy=$13,<out1=$12
cg $14,$13,$12

# qhasm: int32323232 out1 += dummy
# asm 1: a >out1=vec128#10,<out1=vec128#10,<dummy=vec128#11
# asm 2: a >out1=$12,<out1=$12,<dummy=$13
a $12,$12,$13

# qhasm: c <<= (8 * 4)
# asm 1: shlqbyi >c=vec128#11,<c=vec128#12,4
# asm 2: shlqbyi >c=$13,<c=$14,4
shlqbyi $13,$14,4

# qhasm: int32323232 out1 += c
# asm 1: a >out1=vec128#10,<out1=vec128#10,<c=vec128#11
# asm 2: a >out1=$12,<out1=$12,<c=$13
a $12,$12,$13

# qhasm: dummy = in_1619 >> (8 * 12)
# asm 1: rotqmbyi >dummy=vec128#11,<in_1619=vec128#2,-12
# asm 2: rotqmbyi >dummy=$13,<in_1619=$4,-12
rotqmbyi $13,$4,-12

# qhasm: dummy >>= (7 % 8) 
# asm 1: rotqmbii >dummy=vec128#11,<dummy=vec128#11,-7
# asm 2: rotqmbii >dummy=$13,<dummy=$13,-7
rotqmbii $13,$13,-7

# qhasm: dummy <<= (8 * 12)
# asm 1: shlqbyi >dummy=vec128#11,<dummy=vec128#11,12
# asm 2: shlqbyi >dummy=$13,<dummy=$13,12
shlqbyi $13,$13,12

# qhasm: int32323232 out1 +=  dummy
# asm 1: a >out1=vec128#10,<out1=vec128#10,<dummy=vec128#11
# asm 2: a >out1=$12,<out1=$12,<dummy=$13
a $12,$12,$13

# qhasm: in_1619 <<= (8 * 4)
# asm 1: shlqbyi >in_1619=vec128#2,<in_1619=vec128#2,4
# asm 2: shlqbyi >in_1619=$4,<in_1619=$4,4
shlqbyi $4,$4,4

# qhasm: dummy = in_1619 >> (8 * 12)
# asm 1: rotqmbyi >dummy=vec128#11,<in_1619=vec128#2,-12
# asm 2: rotqmbyi >dummy=$13,<in_1619=$4,-12
rotqmbyi $13,$4,-12

# qhasm: dummy <<= (8 * 12)
# asm 1: shlqbyi >dummy=vec128#11,<dummy=vec128#11,12
# asm 2: shlqbyi >dummy=$13,<dummy=$13,12
shlqbyi $13,$13,12

# qhasm: dummy <<= (6 % 8)
# asm 1: shlqbii >dummy=vec128#11,<dummy=vec128#11,6
# asm 2: shlqbii >dummy=$13,<dummy=$13,6
shlqbii $13,$13,6

# qhasm: int32323232 out1 += dummy
# asm 1: a >out1=vec128#10,<out1=vec128#10,<dummy=vec128#11
# asm 2: a >out1=$12,<out1=$12,<dummy=$13
a $12,$12,$13

# qhasm: in_1619 <<= (8 * 4)
# asm 1: shlqbyi >in_1619=vec128#2,<in_1619=vec128#2,4
# asm 2: shlqbyi >in_1619=$4,<in_1619=$4,4
shlqbyi $4,$4,4

# qhasm: dummy = in_1619 >> (8 * 12)
# asm 1: rotqmbyi >dummy=vec128#2,<in_1619=vec128#2,-12
# asm 2: rotqmbyi >dummy=$4,<in_1619=$4,-12
rotqmbyi $4,$4,-12

# qhasm: dummy <<= (8 * 14)
# asm 1: shlqbyi >dummy=vec128#2,<dummy=vec128#2,14
# asm 2: shlqbyi >dummy=$4,<dummy=$4,14
shlqbyi $4,$4,14

# qhasm: dummy <<= (3 % 8)
# asm 1: shlqbii >dummy=vec128#2,<dummy=vec128#2,3
# asm 2: shlqbii >dummy=$4,<dummy=$4,3
shlqbii $4,$4,3

# qhasm: int32323232 c = carry(out1 + dummy)
# asm 1: cg >c=vec128#11,<out1=vec128#10,<dummy=vec128#2
# asm 2: cg >c=$13,<out1=$12,<dummy=$4
cg $13,$12,$4

# qhasm: int32323232 out1 += dummy
# asm 1: a >out1=vec128#2,<out1=vec128#10,<dummy=vec128#2
# asm 2: a >out1=$4,<out1=$12,<dummy=$4
a $4,$12,$4

# qhasm: c >>= (8 * 12)
# asm 1: rotqmbyi >c=vec128#10,<c=vec128#11,-12
# asm 2: rotqmbyi >c=$12,<c=$13,-12
rotqmbyi $12,$13,-12

# qhasm: int32323232 dummy = (c & 0xffff) * 38
# asm 1: mpyi >dummy=vec128#10,<c=vec128#10,38
# asm 2: mpyi >dummy=$12,<c=$12,38
mpyi $12,$12,38

# qhasm: int32323232 c = carry(out0 + dummy)
# asm 1: cg >c=vec128#11,<out0=vec128#9,<dummy=vec128#10
# asm 2: cg >c=$13,<out0=$11,<dummy=$12
cg $13,$11,$12

# qhasm: int32323232 out0 += dummy
# asm 1: a >out0=vec128#9,<out0=vec128#9,<dummy=vec128#10
# asm 2: a >out0=$11,<out0=$11,<dummy=$12
a $11,$11,$12

# qhasm: dummy = c << (8 * 4)
# asm 1: shlqbyi >dummy=vec128#10,<c=vec128#11,4
# asm 2: shlqbyi >dummy=$12,<c=$13,4
shlqbyi $12,$13,4

# qhasm: int32323232 out0 += dummy
# asm 1: a >out0=vec128#9,<out0=vec128#9,<dummy=vec128#10
# asm 2: a >out0=$11,<out0=$11,<dummy=$12
a $11,$11,$12

# qhasm: c = out1 >> (8 * 15)
# asm 1: rotqmbyi >c=vec128#10,<out1=vec128#2,-15
# asm 2: rotqmbyi >c=$12,<out1=$4,-15
rotqmbyi $12,$4,-15

# qhasm: c = c >> (7 % 8)
# asm 1: rotqmbii >c=vec128#10,<c=vec128#10,-7
# asm 2: rotqmbii >c=$12,<c=$12,-7
rotqmbii $12,$12,-7

# qhasm: int32323232 dummy = (c & 0xffff) * 19
# asm 1: mpyi >dummy=vec128#10,<c=vec128#10,19
# asm 2: mpyi >dummy=$12,<c=$12,19
mpyi $12,$12,19

# qhasm: int32323232 c = carry(out0 + dummy)
# asm 1: cg >c=vec128#11,<out0=vec128#9,<dummy=vec128#10
# asm 2: cg >c=$13,<out0=$11,<dummy=$12
cg $13,$11,$12

# qhasm: int32323232 out0 += dummy
# asm 1: a >out0=vec128#9,<out0=vec128#9,<dummy=vec128#10
# asm 2: a >out0=$11,<out0=$11,<dummy=$12
a $11,$11,$12

# qhasm: dummy = c << (8 * 4)
# asm 1: shlqbyi >dummy=vec128#10,<c=vec128#11,4
# asm 2: shlqbyi >dummy=$12,<c=$13,4
shlqbyi $12,$13,4

# qhasm: int32323232 out0 += dummy
# asm 1: a >out0=vec128#9,<out0=vec128#9,<dummy=vec128#10
# asm 2: a >out0=$11,<out0=$11,<dummy=$12
a $11,$11,$12

# qhasm: out1 &= nmsbit
# asm 1: and >out1=vec128#2,<out1=vec128#2,<nmsbit=vec128#6
# asm 2: and >out1=$4,<out1=$4,<nmsbit=$8
and $4,$4,$8

# qhasm: int32323232 out0s = out0 - p_upper
# asm 1: sf >out0s=vec128#6,<p_upper=vec128#5,<out0=vec128#9
# asm 2: sf >out0s=$8,<p_upper=$7,<out0=$11
sf $8,$7,$11

# qhasm: int32323232 out1s = out1 - p_lower
# asm 1: sf >out1s=vec128#10,<p_lower=vec128#4,<out1=vec128#2
# asm 2: sf >out1s=$12,<p_lower=$6,<out1=$4
sf $12,$6,$4

# qhasm: uint32323232 b0 = borrow(p_upper - out0)
# asm 1: bg >b0=vec128#5,<p_upper=vec128#5,<out0=vec128#9
# asm 2: bg >b0=$7,<p_upper=$7,<out0=$11
bg $7,$7,$11

# qhasm: uint32323232 b1 = borrow(p_lower - out0)
# asm 1: bg >b1=vec128#4,<p_lower=vec128#4,<out0=vec128#9
# asm 2: bg >b1=$6,<p_lower=$6,<out0=$11
bg $6,$6,$11

# qhasm: uint32323232 b0 = b0[0] | b0[1] | b0[2] | b0[3]
# asm 1: orx >b0=vec128#5,<b0=vec128#5
# asm 2: orx >b0=$7,<b0=$7
orx $7,$7

# qhasm: uint32323232 b1 = b0[0] | b0[1] | b0[2] | b0[3]
# asm 1: orx >b1=vec128#11,<b0=vec128#5
# asm 2: orx >b1=$13,<b0=$7
orx $13,$7

# qhasm: b0 = b0 | b1
# asm 1: or >b0=vec128#5,<b0=vec128#5,<b1=vec128#11
# asm 2: or >b0=$7,<b0=$7,<b1=$13
or $7,$7,$13

# qhasm: int32323232 b0 = zero - b0
# asm 1: sf >b0=vec128#5,<b0=vec128#5,<zero=vec128#7
# asm 2: sf >b0=$7,<b0=$7,<zero=$9
sf $7,$7,$9

# qhasm: dummy = b0 <<< (8 * 4)
# asm 1: rotqbyi >dummy=vec128#11,<b0=vec128#5,4
# asm 2: rotqbyi >dummy=$13,<b0=$7,4
rotqbyi $13,$7,4

# qhasm: b0 ^= dummy
# asm 1: xor >b0=vec128#5,<b0=vec128#5,<dummy=vec128#11
# asm 2: xor >b0=$7,<b0=$7,<dummy=$13
xor $7,$7,$13

# qhasm: dummy = b0 <<< (8 * 8)
# asm 1: rotqbyi >dummy=vec128#11,<b0=vec128#5,8
# asm 2: rotqbyi >dummy=$13,<b0=$7,8
rotqbyi $13,$7,8

# qhasm: b0 ^= dummy
# asm 1: xor >b0=vec128#5,<b0=vec128#5,<dummy=vec128#11
# asm 2: xor >b0=$7,<b0=$7,<dummy=$13
xor $7,$7,$13

# qhasm: b1 = ~(zero | b0)
# asm 1: nor >b1=vec128#7,<zero=vec128#7,<b0=vec128#5
# asm 2: nor >b1=$9,<zero=$9,<b0=$7
nor $9,$9,$7

# qhasm: out0 &= b1
# asm 1: and >out0=vec128#9,<out0=vec128#9,<b1=vec128#7
# asm 2: and >out0=$11,<out0=$11,<b1=$9
and $11,$11,$9

# qhasm: out1 &= b1
# asm 1: and >out1=vec128#2,<out1=vec128#2,<b1=vec128#7
# asm 2: and >out1=$4,<out1=$4,<b1=$9
and $4,$4,$9

# qhasm: out0s &= b0
# asm 1: and >out0s=vec128#6,<out0s=vec128#6,<b0=vec128#5
# asm 2: and >out0s=$8,<out0s=$8,<b0=$7
and $8,$8,$7

# qhasm: out1s &= b0
# asm 1: and >out1s=vec128#5,<out1s=vec128#10,<b0=vec128#5
# asm 2: and >out1s=$7,<out1s=$12,<b0=$7
and $7,$12,$7

# qhasm: out0 = out0 ^ out0s
# asm 1: xor >out0=vec128#6,<out0=vec128#9,<out0s=vec128#6
# asm 2: xor >out0=$8,<out0=$11,<out0s=$8
xor $8,$11,$8

# qhasm: out1 = out1 ^ out1s
# asm 1: xor >out1=vec128#2,<out1=vec128#2,<out1s=vec128#5
# asm 2: xor >out1=$4,<out1=$4,<out1s=$7
xor $4,$4,$7

# qhasm: out0 = select bytes from out0 by swapendian
# asm 1: shufb >out0=vec128#5,<out0=vec128#6,<out0=vec128#6,<swapendian=vec128#3
# asm 2: shufb >out0=$7,<out0=$8,<out0=$8,<swapendian=$5
shufb $7,$8,$8,$5

# qhasm: out1 = select bytes from out1 by swapendian
# asm 1: shufb >out1=vec128#2,<out1=vec128#2,<out1=vec128#2,<swapendian=vec128#3
# asm 2: shufb >out1=$4,<out1=$4,<out1=$4,<swapendian=$5
shufb $4,$4,$4,$5

# qhasm: *(vec128 *) ((retp + 0) & ~15) = out0
# asm 1: stqd <out0=vec128#5,0(<retp=vec128#1)
# asm 2: stqd <out0=$7,0(<retp=$3)
stqd $7,0($3)

# qhasm: *(vec128 *) ((retp + 16) & ~15) = out1
# asm 1: stqd <out1=vec128#2,16(<retp=vec128#1)
# asm 2: stqd <out1=$4,16(<retp=$3)
stqd $4,16($3)

# qhasm: leave
ai $sp,$sp,32
bi $lr
