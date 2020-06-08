# version 20090331
# Peter Schwabe & Neil Costigan
# Public domain.

# qhasm: vec128 retp

# qhasm: vec128 inp

# qhasm: input retp

# qhasm: input inp

# qhasm: vec128 in0

# qhasm: vec128 in1

# qhasm: vec128 out_03

# qhasm: vec128 out_47

# qhasm: vec128 out_811

# qhasm: vec128 out_1215

# qhasm: vec128 out_1619

# qhasm: vec128 dummy

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

# qhasm: enter tored
.text
.align 3
.globl _tored
.global tored
.type tored, @function
tored:
stqd $lr, 16($sp)
stqd $sp,-32($sp)
ai $sp,$sp,-32

# qhasm: swapendian = extern(_swapendian)
# asm 1: lqr <swapendian=vec128#3,_swapendian
# asm 2: lqr <swapendian=$5,_swapendian
lqr $5,_swapendian

# qhasm: in1 = *(vec128 *) ((inp + 0) & ~15)
# asm 1: lqd >in1=vec128#4,0(<inp=vec128#2)
# asm 2: lqd >in1=$6,0(<inp=$4)
lqd $6,0($4)

# qhasm: in0 = *(vec128 *) ((inp + 16) & ~15)
# asm 1: lqd >in0=vec128#2,16(<inp=vec128#2)
# asm 2: lqd >in0=$4,16(<inp=$4)
lqd $4,16($4)

# qhasm: in0 = select bytes from in0 by swapendian
# asm 1: shufb >in0=vec128#2,<in0=vec128#2,<in0=vec128#2,<swapendian=vec128#3
# asm 2: shufb >in0=$4,<in0=$4,<in0=$4,<swapendian=$5
shufb $4,$4,$4,$5

# qhasm: in1 = select bytes from in1 by swapendian
# asm 1: shufb >in1=vec128#3,<in1=vec128#4,<in1=vec128#4,<swapendian=vec128#3
# asm 2: shufb >in1=$5,<in1=$6,<in1=$6,<swapendian=$5
shufb $5,$6,$6,$5

# qhasm: dummy = in1 << (8 * 14)
# asm 1: shlqbyi >dummy=vec128#4,<in1=vec128#3,14
# asm 2: shlqbyi >dummy=$6,<in1=$5,14
shlqbyi $6,$5,14

# qhasm: dummy <<= (3 % 8)
# asm 1: shlqbii <dummy=vec128#4,<dummy=vec128#4,3
# asm 2: shlqbii <dummy=$6,<dummy=$6,3
shlqbii $6,$6,3

# qhasm: dummy >>= (3 % 8)
# asm 1: rotqmbii <dummy=vec128#4,<dummy=vec128#4,-3
# asm 2: rotqmbii <dummy=$6,<dummy=$6,-3
rotqmbii $6,$6,-3

# qhasm: out_03 = dummy >> (8 * 2)
# asm 1: rotqmbyi >out_03=vec128#4,<dummy=vec128#4,-2
# asm 2: rotqmbyi >out_03=$6,<dummy=$6,-2
rotqmbyi $6,$6,-2

# qhasm: in1 >>= (8 * 1)
# asm 1: rotqmbyi <in1=vec128#3,<in1=vec128#3,-1
# asm 2: rotqmbyi <in1=$5,<in1=$5,-1
rotqmbyi $5,$5,-1

# qhasm: in1 >>= (5 % 8)
# asm 1: rotqmbii <in1=vec128#3,<in1=vec128#3,-5
# asm 2: rotqmbii <in1=$5,<in1=$5,-5
rotqmbii $5,$5,-5

# qhasm: dummy = in1 << (8 * 14)
# asm 1: shlqbyi >dummy=vec128#5,<in1=vec128#3,14
# asm 2: shlqbyi >dummy=$7,<in1=$5,14
shlqbyi $7,$5,14

# qhasm: dummy <<= (3 % 8)
# asm 1: shlqbii <dummy=vec128#5,<dummy=vec128#5,3
# asm 2: shlqbii <dummy=$7,<dummy=$7,3
shlqbii $7,$7,3

# qhasm: dummy >>= (3 % 8)
# asm 1: rotqmbii <dummy=vec128#5,<dummy=vec128#5,-3
# asm 2: rotqmbii <dummy=$7,<dummy=$7,-3
rotqmbii $7,$7,-3

# qhasm: dummy >>= (8 * 6)
# asm 1: rotqmbyi <dummy=vec128#5,<dummy=vec128#5,-6
# asm 2: rotqmbyi <dummy=$7,<dummy=$7,-6
rotqmbyi $7,$7,-6

# qhasm: out_03 ^= dummy
# asm 1: xor <out_03=vec128#4,<out_03=vec128#4,<dummy=vec128#5
# asm 2: xor <out_03=$6,<out_03=$6,<dummy=$7
xor $6,$6,$7

# qhasm: in1 >>= (8 * 1)
# asm 1: rotqmbyi <in1=vec128#3,<in1=vec128#3,-1
# asm 2: rotqmbyi <in1=$5,<in1=$5,-1
rotqmbyi $5,$5,-1

# qhasm: in1 >>= (5 % 8)
# asm 1: rotqmbii <in1=vec128#3,<in1=vec128#3,-5
# asm 2: rotqmbii <in1=$5,<in1=$5,-5
rotqmbii $5,$5,-5

# qhasm: dummy = in1 << (8 * 14)
# asm 1: shlqbyi >dummy=vec128#5,<in1=vec128#3,14
# asm 2: shlqbyi >dummy=$7,<in1=$5,14
shlqbyi $7,$5,14

# qhasm: dummy <<= (3 % 8)
# asm 1: shlqbii <dummy=vec128#5,<dummy=vec128#5,3
# asm 2: shlqbii <dummy=$7,<dummy=$7,3
shlqbii $7,$7,3

# qhasm: dummy >>= (3 % 8)
# asm 1: rotqmbii <dummy=vec128#5,<dummy=vec128#5,-3
# asm 2: rotqmbii <dummy=$7,<dummy=$7,-3
rotqmbii $7,$7,-3

# qhasm: dummy >>= (8 * 10)
# asm 1: rotqmbyi <dummy=vec128#5,<dummy=vec128#5,-10
# asm 2: rotqmbyi <dummy=$7,<dummy=$7,-10
rotqmbyi $7,$7,-10

# qhasm: out_03 ^= dummy
# asm 1: xor <out_03=vec128#4,<out_03=vec128#4,<dummy=vec128#5
# asm 2: xor <out_03=$6,<out_03=$6,<dummy=$7
xor $6,$6,$7

# qhasm: in1 >>= (8 * 1)
# asm 1: rotqmbyi <in1=vec128#3,<in1=vec128#3,-1
# asm 2: rotqmbyi <in1=$5,<in1=$5,-1
rotqmbyi $5,$5,-1

# qhasm: in1 >>= (5 % 8)
# asm 1: rotqmbii <in1=vec128#3,<in1=vec128#3,-5
# asm 2: rotqmbii <in1=$5,<in1=$5,-5
rotqmbii $5,$5,-5

# qhasm: dummy = in1 << (8 * 14)
# asm 1: shlqbyi >dummy=vec128#5,<in1=vec128#3,14
# asm 2: shlqbyi >dummy=$7,<in1=$5,14
shlqbyi $7,$5,14

# qhasm: dummy <<= (4 % 8)
# asm 1: shlqbii <dummy=vec128#5,<dummy=vec128#5,4
# asm 2: shlqbii <dummy=$7,<dummy=$7,4
shlqbii $7,$7,4

# qhasm: dummy >>= (4 % 8)
# asm 1: rotqmbii <dummy=vec128#5,<dummy=vec128#5,-4
# asm 2: rotqmbii <dummy=$7,<dummy=$7,-4
rotqmbii $7,$7,-4

# qhasm: dummy >>= (8 * 14)
# asm 1: rotqmbyi <dummy=vec128#5,<dummy=vec128#5,-14
# asm 2: rotqmbyi <dummy=$7,<dummy=$7,-14
rotqmbyi $7,$7,-14

# qhasm: out_03 ^= dummy
# asm 1: xor <out_03=vec128#4,<out_03=vec128#4,<dummy=vec128#5
# asm 2: xor <out_03=$6,<out_03=$6,<dummy=$7
xor $6,$6,$7

# qhasm: in1 >>= (8 * 1)
# asm 1: rotqmbyi <in1=vec128#3,<in1=vec128#3,-1
# asm 2: rotqmbyi <in1=$5,<in1=$5,-1
rotqmbyi $5,$5,-1

# qhasm: in1 >>= (4 % 8)
# asm 1: rotqmbii <in1=vec128#3,<in1=vec128#3,-4
# asm 2: rotqmbii <in1=$5,<in1=$5,-4
rotqmbii $5,$5,-4

# qhasm: dummy = in1 << (8 * 14)
# asm 1: shlqbyi >dummy=vec128#5,<in1=vec128#3,14
# asm 2: shlqbyi >dummy=$7,<in1=$5,14
shlqbyi $7,$5,14

# qhasm: dummy <<= (3 % 8)
# asm 1: shlqbii <dummy=vec128#5,<dummy=vec128#5,3
# asm 2: shlqbii <dummy=$7,<dummy=$7,3
shlqbii $7,$7,3

# qhasm: dummy >>= (3 % 8)
# asm 1: rotqmbii <dummy=vec128#5,<dummy=vec128#5,-3
# asm 2: rotqmbii <dummy=$7,<dummy=$7,-3
rotqmbii $7,$7,-3

# qhasm: out_47 = dummy >> (8 * 2)
# asm 1: rotqmbyi >out_47=vec128#5,<dummy=vec128#5,-2
# asm 2: rotqmbyi >out_47=$7,<dummy=$7,-2
rotqmbyi $7,$7,-2

# qhasm: in1 >>= (8 * 1)
# asm 1: rotqmbyi <in1=vec128#3,<in1=vec128#3,-1
# asm 2: rotqmbyi <in1=$5,<in1=$5,-1
rotqmbyi $5,$5,-1

# qhasm: in1 >>= (5 % 8)
# asm 1: rotqmbii <in1=vec128#3,<in1=vec128#3,-5
# asm 2: rotqmbii <in1=$5,<in1=$5,-5
rotqmbii $5,$5,-5

# qhasm: dummy = in1 << (8 * 14)
# asm 1: shlqbyi >dummy=vec128#6,<in1=vec128#3,14
# asm 2: shlqbyi >dummy=$8,<in1=$5,14
shlqbyi $8,$5,14

# qhasm: dummy <<= (3 % 8)
# asm 1: shlqbii <dummy=vec128#6,<dummy=vec128#6,3
# asm 2: shlqbii <dummy=$8,<dummy=$8,3
shlqbii $8,$8,3

# qhasm: dummy >>= (3 % 8)
# asm 1: rotqmbii <dummy=vec128#6,<dummy=vec128#6,-3
# asm 2: rotqmbii <dummy=$8,<dummy=$8,-3
rotqmbii $8,$8,-3

# qhasm: dummy >>= (8 * 6)
# asm 1: rotqmbyi <dummy=vec128#6,<dummy=vec128#6,-6
# asm 2: rotqmbyi <dummy=$8,<dummy=$8,-6
rotqmbyi $8,$8,-6

# qhasm: out_47 ^= dummy
# asm 1: xor <out_47=vec128#5,<out_47=vec128#5,<dummy=vec128#6
# asm 2: xor <out_47=$7,<out_47=$7,<dummy=$8
xor $7,$7,$8

# qhasm: in1 >>= (8 * 1)
# asm 1: rotqmbyi <in1=vec128#3,<in1=vec128#3,-1
# asm 2: rotqmbyi <in1=$5,<in1=$5,-1
rotqmbyi $5,$5,-1

# qhasm: in1 >>= (5 % 8)
# asm 1: rotqmbii <in1=vec128#3,<in1=vec128#3,-5
# asm 2: rotqmbii <in1=$5,<in1=$5,-5
rotqmbii $5,$5,-5

# qhasm: dummy = in1 << (8 * 14)
# asm 1: shlqbyi >dummy=vec128#6,<in1=vec128#3,14
# asm 2: shlqbyi >dummy=$8,<in1=$5,14
shlqbyi $8,$5,14

# qhasm: dummy <<= (3 % 8)
# asm 1: shlqbii <dummy=vec128#6,<dummy=vec128#6,3
# asm 2: shlqbii <dummy=$8,<dummy=$8,3
shlqbii $8,$8,3

# qhasm: dummy >>= (3 % 8)
# asm 1: rotqmbii <dummy=vec128#6,<dummy=vec128#6,-3
# asm 2: rotqmbii <dummy=$8,<dummy=$8,-3
rotqmbii $8,$8,-3

# qhasm: dummy >>= (8 * 10)
# asm 1: rotqmbyi <dummy=vec128#6,<dummy=vec128#6,-10
# asm 2: rotqmbyi <dummy=$8,<dummy=$8,-10
rotqmbyi $8,$8,-10

# qhasm: out_47 ^= dummy
# asm 1: xor <out_47=vec128#5,<out_47=vec128#5,<dummy=vec128#6
# asm 2: xor <out_47=$7,<out_47=$7,<dummy=$8
xor $7,$7,$8

# qhasm: in1 >>= (8 * 1)
# asm 1: rotqmbyi <in1=vec128#3,<in1=vec128#3,-1
# asm 2: rotqmbyi <in1=$5,<in1=$5,-1
rotqmbyi $5,$5,-1

# qhasm: in1 >>= (5 % 8)
# asm 1: rotqmbii <in1=vec128#3,<in1=vec128#3,-5
# asm 2: rotqmbii <in1=$5,<in1=$5,-5
rotqmbii $5,$5,-5

# qhasm: dummy = in1 << (8 * 14)
# asm 1: shlqbyi >dummy=vec128#6,<in1=vec128#3,14
# asm 2: shlqbyi >dummy=$8,<in1=$5,14
shlqbyi $8,$5,14

# qhasm: dummy <<= (4 % 8)
# asm 1: shlqbii <dummy=vec128#6,<dummy=vec128#6,4
# asm 2: shlqbii <dummy=$8,<dummy=$8,4
shlqbii $8,$8,4

# qhasm: dummy >>= (4 % 8)
# asm 1: rotqmbii <dummy=vec128#6,<dummy=vec128#6,-4
# asm 2: rotqmbii <dummy=$8,<dummy=$8,-4
rotqmbii $8,$8,-4

# qhasm: dummy >>= (8 * 14)
# asm 1: rotqmbyi <dummy=vec128#6,<dummy=vec128#6,-14
# asm 2: rotqmbyi <dummy=$8,<dummy=$8,-14
rotqmbyi $8,$8,-14

# qhasm: out_47 ^= dummy
# asm 1: xor <out_47=vec128#5,<out_47=vec128#5,<dummy=vec128#6
# asm 2: xor <out_47=$7,<out_47=$7,<dummy=$8
xor $7,$7,$8

# qhasm: in1 >>= (8 * 1)
# asm 1: rotqmbyi <in1=vec128#3,<in1=vec128#3,-1
# asm 2: rotqmbyi <in1=$5,<in1=$5,-1
rotqmbyi $5,$5,-1

# qhasm: in1 >>= (4 % 8)
# asm 1: rotqmbii <in1=vec128#3,<in1=vec128#3,-4
# asm 2: rotqmbii <in1=$5,<in1=$5,-4
rotqmbii $5,$5,-4

# qhasm: dummy = in1 << (8 * 14)
# asm 1: shlqbyi >dummy=vec128#6,<in1=vec128#3,14
# asm 2: shlqbyi >dummy=$8,<in1=$5,14
shlqbyi $8,$5,14

# qhasm: dummy <<= (3 % 8)
# asm 1: shlqbii <dummy=vec128#6,<dummy=vec128#6,3
# asm 2: shlqbii <dummy=$8,<dummy=$8,3
shlqbii $8,$8,3

# qhasm: dummy >>= (3 % 8)
# asm 1: rotqmbii <dummy=vec128#6,<dummy=vec128#6,-3
# asm 2: rotqmbii <dummy=$8,<dummy=$8,-3
rotqmbii $8,$8,-3

# qhasm: out_811 = dummy >> (8 * 2)
# asm 1: rotqmbyi >out_811=vec128#6,<dummy=vec128#6,-2
# asm 2: rotqmbyi >out_811=$8,<dummy=$8,-2
rotqmbyi $8,$8,-2

# qhasm: in1 >>= (8 * 1)
# asm 1: rotqmbyi <in1=vec128#3,<in1=vec128#3,-1
# asm 2: rotqmbyi <in1=$5,<in1=$5,-1
rotqmbyi $5,$5,-1

# qhasm: in1 >>= (5 % 8)
# asm 1: rotqmbii <in1=vec128#3,<in1=vec128#3,-5
# asm 2: rotqmbii <in1=$5,<in1=$5,-5
rotqmbii $5,$5,-5

# qhasm: dummy = in1 << (8 * 14)
# asm 1: shlqbyi >dummy=vec128#7,<in1=vec128#3,14
# asm 2: shlqbyi >dummy=$9,<in1=$5,14
shlqbyi $9,$5,14

# qhasm: dummy <<= (3 % 8)
# asm 1: shlqbii <dummy=vec128#7,<dummy=vec128#7,3
# asm 2: shlqbii <dummy=$9,<dummy=$9,3
shlqbii $9,$9,3

# qhasm: dummy >>= (3 % 8)
# asm 1: rotqmbii <dummy=vec128#7,<dummy=vec128#7,-3
# asm 2: rotqmbii <dummy=$9,<dummy=$9,-3
rotqmbii $9,$9,-3

# qhasm: dummy >>= (8 * 6)
# asm 1: rotqmbyi <dummy=vec128#7,<dummy=vec128#7,-6
# asm 2: rotqmbyi <dummy=$9,<dummy=$9,-6
rotqmbyi $9,$9,-6

# qhasm: out_811 ^= dummy
# asm 1: xor <out_811=vec128#6,<out_811=vec128#6,<dummy=vec128#7
# asm 2: xor <out_811=$8,<out_811=$8,<dummy=$9
xor $8,$8,$9

# qhasm: in1 >>= (8 * 1)
# asm 1: rotqmbyi <in1=vec128#3,<in1=vec128#3,-1
# asm 2: rotqmbyi <in1=$5,<in1=$5,-1
rotqmbyi $5,$5,-1

# qhasm: in1 >>= (5 % 8)
# asm 1: rotqmbii <in1=vec128#3,<in1=vec128#3,-5
# asm 2: rotqmbii <in1=$5,<in1=$5,-5
rotqmbii $5,$5,-5

# qhasm: dummy = in0 << (8 * 14)
# asm 1: shlqbyi >dummy=vec128#3,<in0=vec128#2,14
# asm 2: shlqbyi >dummy=$5,<in0=$4,14
shlqbyi $5,$4,14

# qhasm: dummy <<= (3 % 8)
# asm 1: shlqbii <dummy=vec128#3,<dummy=vec128#3,3
# asm 2: shlqbii <dummy=$5,<dummy=$5,3
shlqbii $5,$5,3

# qhasm: dummy >>= (3 % 8)
# asm 1: rotqmbii <dummy=vec128#3,<dummy=vec128#3,-3
# asm 2: rotqmbii <dummy=$5,<dummy=$5,-3
rotqmbii $5,$5,-3

# qhasm: dummy >>= (8 * 10)
# asm 1: rotqmbyi <dummy=vec128#3,<dummy=vec128#3,-10
# asm 2: rotqmbyi <dummy=$5,<dummy=$5,-10
rotqmbyi $5,$5,-10

# qhasm: out_811 ^= dummy
# asm 1: xor <out_811=vec128#6,<out_811=vec128#6,<dummy=vec128#3
# asm 2: xor <out_811=$8,<out_811=$8,<dummy=$5
xor $8,$8,$5

# qhasm: in0 >>= (8 * 1)
# asm 1: rotqmbyi <in0=vec128#2,<in0=vec128#2,-1
# asm 2: rotqmbyi <in0=$4,<in0=$4,-1
rotqmbyi $4,$4,-1

# qhasm: in0 >>= (5 % 8)
# asm 1: rotqmbii <in0=vec128#2,<in0=vec128#2,-5
# asm 2: rotqmbii <in0=$4,<in0=$4,-5
rotqmbii $4,$4,-5

# qhasm: dummy = in0 << (8 * 14)
# asm 1: shlqbyi >dummy=vec128#3,<in0=vec128#2,14
# asm 2: shlqbyi >dummy=$5,<in0=$4,14
shlqbyi $5,$4,14

# qhasm: dummy <<= (4 % 8)
# asm 1: shlqbii <dummy=vec128#3,<dummy=vec128#3,4
# asm 2: shlqbii <dummy=$5,<dummy=$5,4
shlqbii $5,$5,4

# qhasm: dummy >>= (4 % 8)
# asm 1: rotqmbii <dummy=vec128#3,<dummy=vec128#3,-4
# asm 2: rotqmbii <dummy=$5,<dummy=$5,-4
rotqmbii $5,$5,-4

# qhasm: dummy >>= (8 * 14)
# asm 1: rotqmbyi <dummy=vec128#3,<dummy=vec128#3,-14
# asm 2: rotqmbyi <dummy=$5,<dummy=$5,-14
rotqmbyi $5,$5,-14

# qhasm: out_811 ^= dummy
# asm 1: xor <out_811=vec128#6,<out_811=vec128#6,<dummy=vec128#3
# asm 2: xor <out_811=$8,<out_811=$8,<dummy=$5
xor $8,$8,$5

# qhasm: in0 >>= (8 * 1)
# asm 1: rotqmbyi <in0=vec128#2,<in0=vec128#2,-1
# asm 2: rotqmbyi <in0=$4,<in0=$4,-1
rotqmbyi $4,$4,-1

# qhasm: in0 >>= (4 % 8)
# asm 1: rotqmbii <in0=vec128#2,<in0=vec128#2,-4
# asm 2: rotqmbii <in0=$4,<in0=$4,-4
rotqmbii $4,$4,-4

# qhasm: dummy = in0 << (8 * 14)
# asm 1: shlqbyi >dummy=vec128#3,<in0=vec128#2,14
# asm 2: shlqbyi >dummy=$5,<in0=$4,14
shlqbyi $5,$4,14

# qhasm: dummy <<= (3 % 8)
# asm 1: shlqbii <dummy=vec128#3,<dummy=vec128#3,3
# asm 2: shlqbii <dummy=$5,<dummy=$5,3
shlqbii $5,$5,3

# qhasm: dummy >>= (3 % 8)
# asm 1: rotqmbii <dummy=vec128#3,<dummy=vec128#3,-3
# asm 2: rotqmbii <dummy=$5,<dummy=$5,-3
rotqmbii $5,$5,-3

# qhasm: out_1215 = dummy >> (8 * 2)
# asm 1: rotqmbyi >out_1215=vec128#3,<dummy=vec128#3,-2
# asm 2: rotqmbyi >out_1215=$5,<dummy=$5,-2
rotqmbyi $5,$5,-2

# qhasm: in0 >>= (8 * 1)
# asm 1: rotqmbyi <in0=vec128#2,<in0=vec128#2,-1
# asm 2: rotqmbyi <in0=$4,<in0=$4,-1
rotqmbyi $4,$4,-1

# qhasm: in0 >>= (5 % 8)
# asm 1: rotqmbii <in0=vec128#2,<in0=vec128#2,-5
# asm 2: rotqmbii <in0=$4,<in0=$4,-5
rotqmbii $4,$4,-5

# qhasm: dummy = in0 << (8 * 14)
# asm 1: shlqbyi >dummy=vec128#7,<in0=vec128#2,14
# asm 2: shlqbyi >dummy=$9,<in0=$4,14
shlqbyi $9,$4,14

# qhasm: dummy <<= (3 % 8)
# asm 1: shlqbii <dummy=vec128#7,<dummy=vec128#7,3
# asm 2: shlqbii <dummy=$9,<dummy=$9,3
shlqbii $9,$9,3

# qhasm: dummy >>= (3 % 8)
# asm 1: rotqmbii <dummy=vec128#7,<dummy=vec128#7,-3
# asm 2: rotqmbii <dummy=$9,<dummy=$9,-3
rotqmbii $9,$9,-3

# qhasm: dummy >>= (8 * 6)
# asm 1: rotqmbyi <dummy=vec128#7,<dummy=vec128#7,-6
# asm 2: rotqmbyi <dummy=$9,<dummy=$9,-6
rotqmbyi $9,$9,-6

# qhasm: out_1215 ^= dummy
# asm 1: xor <out_1215=vec128#3,<out_1215=vec128#3,<dummy=vec128#7
# asm 2: xor <out_1215=$5,<out_1215=$5,<dummy=$9
xor $5,$5,$9

# qhasm: in0 >>= (8 * 1)
# asm 1: rotqmbyi <in0=vec128#2,<in0=vec128#2,-1
# asm 2: rotqmbyi <in0=$4,<in0=$4,-1
rotqmbyi $4,$4,-1

# qhasm: in0 >>= (5 % 8)
# asm 1: rotqmbii <in0=vec128#2,<in0=vec128#2,-5
# asm 2: rotqmbii <in0=$4,<in0=$4,-5
rotqmbii $4,$4,-5

# qhasm: dummy = in0 << (8 * 14)
# asm 1: shlqbyi >dummy=vec128#7,<in0=vec128#2,14
# asm 2: shlqbyi >dummy=$9,<in0=$4,14
shlqbyi $9,$4,14

# qhasm: dummy <<= (3 % 8)
# asm 1: shlqbii <dummy=vec128#7,<dummy=vec128#7,3
# asm 2: shlqbii <dummy=$9,<dummy=$9,3
shlqbii $9,$9,3

# qhasm: dummy >>= (3 % 8)
# asm 1: rotqmbii <dummy=vec128#7,<dummy=vec128#7,-3
# asm 2: rotqmbii <dummy=$9,<dummy=$9,-3
rotqmbii $9,$9,-3

# qhasm: dummy >>= (8 * 10)
# asm 1: rotqmbyi <dummy=vec128#7,<dummy=vec128#7,-10
# asm 2: rotqmbyi <dummy=$9,<dummy=$9,-10
rotqmbyi $9,$9,-10

# qhasm: out_1215 ^= dummy
# asm 1: xor <out_1215=vec128#3,<out_1215=vec128#3,<dummy=vec128#7
# asm 2: xor <out_1215=$5,<out_1215=$5,<dummy=$9
xor $5,$5,$9

# qhasm: in0 >>= (8 * 1)
# asm 1: rotqmbyi <in0=vec128#2,<in0=vec128#2,-1
# asm 2: rotqmbyi <in0=$4,<in0=$4,-1
rotqmbyi $4,$4,-1

# qhasm: in0 >>= (5 % 8)
# asm 1: rotqmbii <in0=vec128#2,<in0=vec128#2,-5
# asm 2: rotqmbii <in0=$4,<in0=$4,-5
rotqmbii $4,$4,-5

# qhasm: dummy = in0 << (8 * 14)
# asm 1: shlqbyi >dummy=vec128#7,<in0=vec128#2,14
# asm 2: shlqbyi >dummy=$9,<in0=$4,14
shlqbyi $9,$4,14

# qhasm: dummy <<= (4 % 8)
# asm 1: shlqbii <dummy=vec128#7,<dummy=vec128#7,4
# asm 2: shlqbii <dummy=$9,<dummy=$9,4
shlqbii $9,$9,4

# qhasm: dummy >>= (4 % 8)
# asm 1: rotqmbii <dummy=vec128#7,<dummy=vec128#7,-4
# asm 2: rotqmbii <dummy=$9,<dummy=$9,-4
rotqmbii $9,$9,-4

# qhasm: dummy >>= (8 * 14)
# asm 1: rotqmbyi <dummy=vec128#7,<dummy=vec128#7,-14
# asm 2: rotqmbyi <dummy=$9,<dummy=$9,-14
rotqmbyi $9,$9,-14

# qhasm: out_1215 ^= dummy
# asm 1: xor <out_1215=vec128#3,<out_1215=vec128#3,<dummy=vec128#7
# asm 2: xor <out_1215=$5,<out_1215=$5,<dummy=$9
xor $5,$5,$9

# qhasm: in0 >>= (8 * 1)
# asm 1: rotqmbyi <in0=vec128#2,<in0=vec128#2,-1
# asm 2: rotqmbyi <in0=$4,<in0=$4,-1
rotqmbyi $4,$4,-1

# qhasm: in0 >>= (4 % 8)
# asm 1: rotqmbii <in0=vec128#2,<in0=vec128#2,-4
# asm 2: rotqmbii <in0=$4,<in0=$4,-4
rotqmbii $4,$4,-4

# qhasm: dummy = in0 << (8 * 14)
# asm 1: shlqbyi >dummy=vec128#7,<in0=vec128#2,14
# asm 2: shlqbyi >dummy=$9,<in0=$4,14
shlqbyi $9,$4,14

# qhasm: dummy <<= (3 % 8)
# asm 1: shlqbii <dummy=vec128#7,<dummy=vec128#7,3
# asm 2: shlqbii <dummy=$9,<dummy=$9,3
shlqbii $9,$9,3

# qhasm: dummy >>= (3 % 8)
# asm 1: rotqmbii <dummy=vec128#7,<dummy=vec128#7,-3
# asm 2: rotqmbii <dummy=$9,<dummy=$9,-3
rotqmbii $9,$9,-3

# qhasm: out_1619 = dummy >> (8 * 2)
# asm 1: rotqmbyi >out_1619=vec128#7,<dummy=vec128#7,-2
# asm 2: rotqmbyi >out_1619=$9,<dummy=$9,-2
rotqmbyi $9,$9,-2

# qhasm: in0 >>= (8 * 1)
# asm 1: rotqmbyi <in0=vec128#2,<in0=vec128#2,-1
# asm 2: rotqmbyi <in0=$4,<in0=$4,-1
rotqmbyi $4,$4,-1

# qhasm: in0 >>= (5 % 8)
# asm 1: rotqmbii <in0=vec128#2,<in0=vec128#2,-5
# asm 2: rotqmbii <in0=$4,<in0=$4,-5
rotqmbii $4,$4,-5

# qhasm: dummy = in0 << (8 * 14)
# asm 1: shlqbyi >dummy=vec128#8,<in0=vec128#2,14
# asm 2: shlqbyi >dummy=$10,<in0=$4,14
shlqbyi $10,$4,14

# qhasm: dummy <<= (3 % 8)
# asm 1: shlqbii <dummy=vec128#8,<dummy=vec128#8,3
# asm 2: shlqbii <dummy=$10,<dummy=$10,3
shlqbii $10,$10,3

# qhasm: dummy >>= (3 % 8)
# asm 1: rotqmbii <dummy=vec128#8,<dummy=vec128#8,-3
# asm 2: rotqmbii <dummy=$10,<dummy=$10,-3
rotqmbii $10,$10,-3

# qhasm: dummy >>= (8 * 6)
# asm 1: rotqmbyi <dummy=vec128#8,<dummy=vec128#8,-6
# asm 2: rotqmbyi <dummy=$10,<dummy=$10,-6
rotqmbyi $10,$10,-6

# qhasm: out_1619 ^= dummy
# asm 1: xor <out_1619=vec128#7,<out_1619=vec128#7,<dummy=vec128#8
# asm 2: xor <out_1619=$9,<out_1619=$9,<dummy=$10
xor $9,$9,$10

# qhasm: in0 >>= (8 * 1)
# asm 1: rotqmbyi <in0=vec128#2,<in0=vec128#2,-1
# asm 2: rotqmbyi <in0=$4,<in0=$4,-1
rotqmbyi $4,$4,-1

# qhasm: in0 >>= (5 % 8)
# asm 1: rotqmbii <in0=vec128#2,<in0=vec128#2,-5
# asm 2: rotqmbii <in0=$4,<in0=$4,-5
rotqmbii $4,$4,-5

# qhasm: dummy = in0 << (8 * 14)
# asm 1: shlqbyi >dummy=vec128#8,<in0=vec128#2,14
# asm 2: shlqbyi >dummy=$10,<in0=$4,14
shlqbyi $10,$4,14

# qhasm: dummy <<= (3 % 8)
# asm 1: shlqbii <dummy=vec128#8,<dummy=vec128#8,3
# asm 2: shlqbii <dummy=$10,<dummy=$10,3
shlqbii $10,$10,3

# qhasm: dummy >>= (3 % 8)
# asm 1: rotqmbii <dummy=vec128#8,<dummy=vec128#8,-3
# asm 2: rotqmbii <dummy=$10,<dummy=$10,-3
rotqmbii $10,$10,-3

# qhasm: dummy >>= (8 * 10)
# asm 1: rotqmbyi <dummy=vec128#8,<dummy=vec128#8,-10
# asm 2: rotqmbyi <dummy=$10,<dummy=$10,-10
rotqmbyi $10,$10,-10

# qhasm: out_1619 ^= dummy
# asm 1: xor <out_1619=vec128#7,<out_1619=vec128#7,<dummy=vec128#8
# asm 2: xor <out_1619=$9,<out_1619=$9,<dummy=$10
xor $9,$9,$10

# qhasm: in0 >>= (8 * 1)
# asm 1: rotqmbyi <in0=vec128#2,<in0=vec128#2,-1
# asm 2: rotqmbyi <in0=$4,<in0=$4,-1
rotqmbyi $4,$4,-1

# qhasm: in0 >>= (5 % 8)
# asm 1: rotqmbii <in0=vec128#2,<in0=vec128#2,-5
# asm 2: rotqmbii <in0=$4,<in0=$4,-5
rotqmbii $4,$4,-5

# qhasm: dummy = in0 << (8 * 14)
# asm 1: shlqbyi >dummy=vec128#2,<in0=vec128#2,14
# asm 2: shlqbyi >dummy=$4,<in0=$4,14
shlqbyi $4,$4,14

# qhasm: dummy <<= (3 % 8)
# asm 1: shlqbii <dummy=vec128#2,<dummy=vec128#2,3
# asm 2: shlqbii <dummy=$4,<dummy=$4,3
shlqbii $4,$4,3

# qhasm: dummy >>= (3 % 8)
# asm 1: rotqmbii <dummy=vec128#2,<dummy=vec128#2,-3
# asm 2: rotqmbii <dummy=$4,<dummy=$4,-3
rotqmbii $4,$4,-3

# qhasm: dummy >>= (8 * 14)
# asm 1: rotqmbyi <dummy=vec128#2,<dummy=vec128#2,-14
# asm 2: rotqmbyi <dummy=$4,<dummy=$4,-14
rotqmbyi $4,$4,-14

# qhasm: out_1619  ^= dummy
# asm 1: xor <out_1619=vec128#7,<out_1619=vec128#7,<dummy=vec128#2
# asm 2: xor <out_1619=$9,<out_1619=$9,<dummy=$4
xor $9,$9,$4

# qhasm: *(vec128 *) ((retp + 0) & ~15) = out_03
# asm 1: stqd <out_03=vec128#4,0(<retp=vec128#1)
# asm 2: stqd <out_03=$6,0(<retp=$3)
stqd $6,0($3)

# qhasm: *(vec128 *) ((retp + 16) & ~15) = out_47
# asm 1: stqd <out_47=vec128#5,16(<retp=vec128#1)
# asm 2: stqd <out_47=$7,16(<retp=$3)
stqd $7,16($3)

# qhasm: *(vec128 *) ((retp + 32) & ~15)= out_811
# asm 1: stqd <out_811=vec128#6,32(<retp=vec128#1)
# asm 2: stqd <out_811=$8,32(<retp=$3)
stqd $8,32($3)

# qhasm: *(vec128 *) ((retp + 48) & ~15) = out_1215
# asm 1: stqd <out_1215=vec128#3,48(<retp=vec128#1)
# asm 2: stqd <out_1215=$5,48(<retp=$3)
stqd $5,48($3)

# qhasm: *(vec128 *) ((retp + 64) & ~15) = out_1619
# asm 1: stqd <out_1619=vec128#7,64(<retp=vec128#1)
# asm 2: stqd <out_1619=$9,64(<retp=$3)
stqd $9,64($3)

# qhasm: leave
ai $sp,$sp,32
bi $lr
