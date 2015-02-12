# version 20090331
# Peter Schwabe & Neil Costigan
# Public domain.

# qhasm: vec128 retp

# qhasm: vec128 ainp

# qhasm: vec128 binp

# qhasm: input retp

# qhasm: input ainp

# qhasm: input binp

# qhasm: vec128 zero

# qhasm: vec128 a0

# qhasm: vec128 a1

# qhasm: vec128 a2

# qhasm: vec128 a3

# qhasm: vec128 a4

# qhasm: vec128 a5

# qhasm: vec128 a6

# qhasm: vec128 a7

# qhasm: vec128 a8

# qhasm: vec128 a9

# qhasm: vec128 a10

# qhasm: vec128 a11

# qhasm: vec128 a12

# qhasm: vec128 a13

# qhasm: vec128 a14

# qhasm: vec128 a15

# qhasm: vec128 a16

# qhasm: vec128 a17

# qhasm: vec128 a18

# qhasm: vec128 a19

# qhasm: vec128 a03

# qhasm: vec128 a47

# qhasm: vec128 a811

# qhasm: vec128 a1215

# qhasm: vec128 a1619

# qhasm: vec128 b03

# qhasm: vec128 b47

# qhasm: vec128 b811

# qhasm: vec128 b1215

# qhasm: vec128 b1619

# qhasm: vec128 b2023

# qhasm: vec128 b2427

# qhasm: vec128 b2831

# qhasm: vec128 b3235

# qhasm: vec128 b3638

# qhasm: vec128 b03s1

# qhasm: vec128 b47s1

# qhasm: vec128 b811s1

# qhasm: vec128 b1215s1

# qhasm: vec128 b1619s1

# qhasm: vec128 b03s2

# qhasm: vec128 b47s2

# qhasm: vec128 b811s2

# qhasm: vec128 b1215s2

# qhasm: vec128 b1619s2

# qhasm: vec128 b03s3

# qhasm: vec128 b47s3

# qhasm: vec128 b811s3

# qhasm: vec128 b1215s3

# qhasm: vec128 b1619s3

# qhasm: vec128 r03

# qhasm: vec128 r14

# qhasm: vec128 r25

# qhasm: vec128 r36

# qhasm: vec128 r47

# qhasm: vec128 r58

# qhasm: vec128 r69

# qhasm: vec128 r710

# qhasm: vec128 r811

# qhasm: vec128 r912

# qhasm: vec128 r1013

# qhasm: vec128 r1114

# qhasm: vec128 r1215

# qhasm: vec128 r1316

# qhasm: vec128 r1417

# qhasm: vec128 r1518

# qhasm: vec128 r1619

# qhasm: vec128 r1720

# qhasm: vec128 r1821

# qhasm: vec128 r1922

# qhasm: vec128 r2023

# qhasm: vec128 r2124

# qhasm: vec128 r2225

# qhasm: vec128 r2326

# qhasm: vec128 r2427

# qhasm: vec128 r2528

# qhasm: vec128 r2629

# qhasm: vec128 r2730

# qhasm: vec128 r2831

# qhasm: vec128 r2932

# qhasm: vec128 r3033

# qhasm: vec128 r3134

# qhasm: vec128 r3235

# qhasm: vec128 r3336

# qhasm: vec128 r3437

# qhasm: vec128 r3538

# qhasm: vec128 r3639

# qhasm: vec128 tmp0

# qhasm: vec128 tmp1

# qhasm: vec128 tmp10

# qhasm: vec128 tmp11

# qhasm: vec128 tmp12

# qhasm: vec128 tmp13

# qhasm: vec128 tmp14

# qhasm: vec128 tmp15

# qhasm: vec128 tmp16

# qhasm: vec128 tmp17

# qhasm: vec128 tmp18

# qhasm: vec128 tmp19

# qhasm: vec128 tmp20

# qhasm: vec128 tmp21

# qhasm: vec128 tmp22

# qhasm: vec128 tmp23

# qhasm: vec128 tmp24

# qhasm: vec128 tmp25

# qhasm: vec128 tmp26

# qhasm: vec128 tmp27

# qhasm: vec128 tmp28

# qhasm: vec128 tmp29

# qhasm: vec128 tmp30

# qhasm: vec128 tmp31

# qhasm: vec128 tmp32

# qhasm: vec128 tmp33

# qhasm: vec128 tmp34

# qhasm: vec128 tmp35

# qhasm: vec128 tmp36

# qhasm: vec128 tmp37

# qhasm: vec128 tmp38

# qhasm: vec128 tmp39

# qhasm: vec128 carry

# qhasm: vec128 carry0

# qhasm: vec128 carry1

# qhasm: vec128 carry2

# qhasm: vec128 carry3

# qhasm: vec128 carry4

# qhasm: vec128 red

# qhasm: vec128 red0

# qhasm: vec128 red1

# qhasm: vec128 red2

# qhasm: vec128 red3

# qhasm: vec128 red4

# qhasm: vec128 sel0

# qhasm: vec128 sel1

# qhasm: vec128 sel2

# qhasm: vec128 sel3

# qhasm: vec128 sel4

# qhasm: vec128 sel5

# qhasm: vec128 sel6

# qhasm: vec128 sel7

# qhasm: vec128 selL

# qhasm: vec128 selH

# qhasm: vec128 sel01

# qhasm: vec128 sel12

# qhasm: vec128 sel23

# qhasm: vec128 sel30

# qhasm: vec128 comb13

# qhasm: vec128 comb22

# qhasm: vec128 comb31

# qhasm: vec128 redcoeffmask

# qhasm: vec128 redcoeffmaskend

# qhasm: vec128 redcoeffmaskveryend

# qhasm: vec128 shlw0001

# qhasm: vec128 shlw0011

# qhasm: vec128 shlw0111

# qhasm: vec128 vec19

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

# qhasm: enter mul
.text
.align 3
.globl _mul
.global mul
.type mul, @function
mul:
stqd $lr, 16($sp)
stqd $sp,-32($sp)
ai $sp,$sp,-32

# qhasm: b03 = *(vec128 *) ((binp + 0) & ~15)
# asm 1: lqd >b03=vec128#4,0(<binp=vec128#3)
# asm 2: lqd >b03=$6,0(<binp=$5)
lqd $6,0($5)

# qhasm: int32323232 zero = 0
# asm 1: il >zero=vec128#5,0
# asm 2: il >zero=$7,0
il $7,0

# qhasm: shlw0001 = extern(_shlw0001)
# asm 1: lqr <shlw0001=vec128#6,_shlw0001
# asm 2: lqr <shlw0001=$8,_shlw0001
lqr $8,_shlw0001

# qhasm: shlw0011 = extern(_shlw0011)
# asm 1: lqr <shlw0011=vec128#7,_shlw0011
# asm 2: lqr <shlw0011=$9,_shlw0011
lqr $9,_shlw0011

# qhasm: shlw0111 = extern(_shlw0111)
# asm 1: lqr <shlw0111=vec128#8,_shlw0111
# asm 2: lqr <shlw0111=$10,_shlw0111
lqr $10,_shlw0111

# qhasm: b47 = *(vec128 *) ((binp + 16) & ~15)
# asm 1: lqd >b47=vec128#9,16(<binp=vec128#3)
# asm 2: lqd >b47=$11,16(<binp=$5)
lqd $11,16($5)

# qhasm: b811 = *(vec128 *) ((binp + 32) & ~15)
# asm 1: lqd >b811=vec128#10,32(<binp=vec128#3)
# asm 2: lqd >b811=$12,32(<binp=$5)
lqd $12,32($5)

# qhasm: nop
 nop

# qhasm: b1215 = *(vec128 *) ((binp + 48) & ~15)
# asm 1: lqd >b1215=vec128#11,48(<binp=vec128#3)
# asm 2: lqd >b1215=$13,48(<binp=$5)
lqd $13,48($5)

# qhasm: int32323232 b03s1 = b03 << shlw0001
# asm 1: shl >b03s1=vec128#12,<b03=vec128#4,<shlw0001=vec128#6
# asm 2: shl >b03s1=$14,<b03=$6,<shlw0001=$8
shl $14,$6,$8

# qhasm: b1619 = *(vec128 *) ((binp + 64) & ~15)
# asm 1: lqd >b1619=vec128#3,64(<binp=vec128#3)
# asm 2: lqd >b1619=$5,64(<binp=$5)
lqd $5,64($5)

# qhasm: int32323232 b03s2 = b03 << shlw0011
# asm 1: shl >b03s2=vec128#13,<b03=vec128#4,<shlw0011=vec128#7
# asm 2: shl >b03s2=$15,<b03=$6,<shlw0011=$9
shl $15,$6,$9

# qhasm: a03 = *(vec128 *) ((ainp + 0) & ~15)
# asm 1: lqd >a03=vec128#14,0(<ainp=vec128#2)
# asm 2: lqd >a03=$16,0(<ainp=$4)
lqd $16,0($4)

# qhasm: int32323232 b03s3 = b03 << shlw0111
# asm 1: shl >b03s3=vec128#15,<b03=vec128#4,<shlw0111=vec128#8
# asm 2: shl >b03s3=$17,<b03=$6,<shlw0111=$10
shl $17,$6,$10

# qhasm: a47 = *(vec128 *) ((ainp + 16) & ~15)
# asm 1: lqd >a47=vec128#16,16(<ainp=vec128#2)
# asm 2: lqd >a47=$18,16(<ainp=$4)
lqd $18,16($4)

# qhasm: int32323232 b47s1 = b47 << shlw0001
# asm 1: shl >b47s1=vec128#17,<b47=vec128#9,<shlw0001=vec128#6
# asm 2: shl >b47s1=$19,<b47=$11,<shlw0001=$8
shl $19,$11,$8

# qhasm: a811 = *(vec128 *) ((ainp + 32) & ~15)
# asm 1: lqd >a811=vec128#18,32(<ainp=vec128#2)
# asm 2: lqd >a811=$20,32(<ainp=$4)
lqd $20,32($4)

# qhasm: int32323232 b47s2 = b47 << shlw0011
# asm 1: shl >b47s2=vec128#19,<b47=vec128#9,<shlw0011=vec128#7
# asm 2: shl >b47s2=$21,<b47=$11,<shlw0011=$9
shl $21,$11,$9

# qhasm: a1215 = *(vec128 *) ((ainp + 48) & ~15)
# asm 1: lqd >a1215=vec128#20,48(<ainp=vec128#2)
# asm 2: lqd >a1215=$22,48(<ainp=$4)
lqd $22,48($4)

# qhasm: int32323232 b47s3 = b47 << shlw0111
# asm 1: shl >b47s3=vec128#21,<b47=vec128#9,<shlw0111=vec128#8
# asm 2: shl >b47s3=$23,<b47=$11,<shlw0111=$10
shl $23,$11,$10

# qhasm: a1619 = *(vec128 *) ((ainp + 64) & ~15)
# asm 1: lqd >a1619=vec128#2,64(<ainp=vec128#2)
# asm 2: lqd >a1619=$4,64(<ainp=$4)
lqd $4,64($4)

# qhasm: int32323232 b811s1 = b811 << shlw0001
# asm 1: shl >b811s1=vec128#22,<b811=vec128#10,<shlw0001=vec128#6
# asm 2: shl >b811s1=$24,<b811=$12,<shlw0001=$8
shl $24,$12,$8

# qhasm: sel1 = extern(select1)
# asm 1: lqr <sel1=vec128#23,select1
# asm 2: lqr <sel1=$25,select1
lqr $25,select1

# qhasm: int32323232 b811s2 = b811 << shlw0011
# asm 1: shl >b811s2=vec128#24,<b811=vec128#10,<shlw0011=vec128#7
# asm 2: shl >b811s2=$26,<b811=$12,<shlw0011=$9
shl $26,$12,$9

# qhasm: sel3 = extern(select3)
# asm 1: lqr <sel3=vec128#25,select3
# asm 2: lqr <sel3=$27,select3
lqr $27,select3

# qhasm: int32323232 b811s3 = b811 << shlw0111
# asm 1: shl >b811s3=vec128#26,<b811=vec128#10,<shlw0111=vec128#8
# asm 2: shl >b811s3=$28,<b811=$12,<shlw0111=$10
shl $28,$12,$10

# qhasm: sel5 = extern(select5)
# asm 1: lqr <sel5=vec128#27,select5
# asm 2: lqr <sel5=$29,select5
lqr $29,select5

# qhasm: int32323232 b1215s1 = b1215 << shlw0001
# asm 1: shl >b1215s1=vec128#28,<b1215=vec128#11,<shlw0001=vec128#6
# asm 2: shl >b1215s1=$30,<b1215=$13,<shlw0001=$8
shl $30,$13,$8

# qhasm: sel7 = extern(select7)
# asm 1: lqr <sel7=vec128#29,select7
# asm 2: lqr <sel7=$31,select7
lqr $31,select7

# qhasm: int32323232 b1215s2 = b1215 << shlw0011
# asm 1: shl >b1215s2=vec128#30,<b1215=vec128#11,<shlw0011=vec128#7
# asm 2: shl >b1215s2=$32,<b1215=$13,<shlw0011=$9
shl $32,$13,$9

# qhasm: a0 = select bytes from a03 by sel1
# asm 1: shufb >a0=vec128#31,<a03=vec128#14,<a03=vec128#14,<sel1=vec128#23
# asm 2: shufb >a0=$33,<a03=$16,<a03=$16,<sel1=$25
shufb $33,$16,$16,$25

# qhasm: int32323232 b1215s3 = b1215 << shlw0111
# asm 1: shl >b1215s3=vec128#32,<b1215=vec128#11,<shlw0111=vec128#8
# asm 2: shl >b1215s3=$34,<b1215=$13,<shlw0111=$10
shl $34,$13,$10

# qhasm: a1 = select bytes from a03 by sel3
# asm 1: shufb >a1=vec128#33,<a03=vec128#14,<a03=vec128#14,<sel3=vec128#25
# asm 2: shufb >a1=$35,<a03=$16,<a03=$16,<sel3=$27
shufb $35,$16,$16,$27

# qhasm: int32323232 b1619s1 = b1619 << shlw0001
# asm 1: shl >b1619s1=vec128#6,<b1619=vec128#3,<shlw0001=vec128#6
# asm 2: shl >b1619s1=$8,<b1619=$5,<shlw0001=$8
shl $8,$5,$8

# qhasm: int32323232 b1619s2 = b1619 << shlw0011
# asm 1: shl >b1619s2=vec128#7,<b1619=vec128#3,<shlw0011=vec128#7
# asm 2: shl >b1619s2=$9,<b1619=$5,<shlw0011=$9
shl $9,$5,$9

# qhasm: int32323232 b1619s3 = b1619 << shlw0111
# asm 1: shl >b1619s3=vec128#8,<b1619=vec128#3,<shlw0111=vec128#8
# asm 2: shl >b1619s3=$10,<b1619=$5,<shlw0111=$10
shl $10,$5,$10

# qhasm: a2 = select bytes from a03 by sel5
# asm 1: shufb >a2=vec128#34,<a03=vec128#14,<a03=vec128#14,<sel5=vec128#27
# asm 2: shufb >a2=$36,<a03=$16,<a03=$16,<sel5=$29
shufb $36,$16,$16,$29

# qhasm: int32323232 r03    = (a0  & 0xffff) * (b03   & 0xffff)
# asm 1: mpy >r03=vec128#35,<a0=vec128#31,<b03=vec128#4
# asm 2: mpy >r03=$37,<a0=$33,<b03=$6
mpy $37,$33,$6

# qhasm: a3 = select bytes from a03 by sel7
# asm 1: shufb >a3=vec128#14,<a03=vec128#14,<a03=vec128#14,<sel7=vec128#29
# asm 2: shufb >a3=$16,<a03=$16,<a03=$16,<sel7=$31
shufb $16,$16,$16,$31

# qhasm: int32323232 r47    = (a0  & 0xffff) * (b47   & 0xffff)
# asm 1: mpy >r47=vec128#36,<a0=vec128#31,<b47=vec128#9
# asm 2: mpy >r47=$38,<a0=$33,<b47=$11
mpy $38,$33,$11

# qhasm: a4 = select bytes from a47 by sel1
# asm 1: shufb >a4=vec128#37,<a47=vec128#16,<a47=vec128#16,<sel1=vec128#23
# asm 2: shufb >a4=$39,<a47=$18,<a47=$18,<sel1=$25
shufb $39,$18,$18,$25

# qhasm: int32323232 r811   = (a0  & 0xffff) * (b811  & 0xffff)
# asm 1: mpy >r811=vec128#38,<a0=vec128#31,<b811=vec128#10
# asm 2: mpy >r811=$40,<a0=$33,<b811=$12
mpy $40,$33,$12

# qhasm: a5 = select bytes from a47 by sel3
# asm 1: shufb >a5=vec128#39,<a47=vec128#16,<a47=vec128#16,<sel3=vec128#25
# asm 2: shufb >a5=$41,<a47=$18,<a47=$18,<sel3=$27
shufb $41,$18,$18,$27

# qhasm: int32323232 r1215  = (a0  & 0xffff) * (b1215 & 0xffff)
# asm 1: mpy >r1215=vec128#40,<a0=vec128#31,<b1215=vec128#11
# asm 2: mpy >r1215=$42,<a0=$33,<b1215=$13
mpy $42,$33,$13

# qhasm: a6 = select bytes from a47 by sel5
# asm 1: shufb >a6=vec128#41,<a47=vec128#16,<a47=vec128#16,<sel5=vec128#27
# asm 2: shufb >a6=$43,<a47=$18,<a47=$18,<sel5=$29
shufb $43,$18,$18,$29

# qhasm: int32323232 r1619  = (a0  & 0xffff) * (b1619 & 0xffff)
# asm 1: mpy >r1619=vec128#31,<a0=vec128#31,<b1619=vec128#3
# asm 2: mpy >r1619=$33,<a0=$33,<b1619=$5
mpy $33,$33,$5

# qhasm: a7 = select bytes from a47 by sel7
# asm 1: shufb >a7=vec128#16,<a47=vec128#16,<a47=vec128#16,<sel7=vec128#29
# asm 2: shufb >a7=$18,<a47=$18,<a47=$18,<sel7=$31
shufb $18,$18,$18,$31

# qhasm: int32323232 r14    = (a1  & 0xffff) * (b03s1   & 0xffff)
# asm 1: mpy >r14=vec128#42,<a1=vec128#33,<b03s1=vec128#12
# asm 2: mpy >r14=$44,<a1=$35,<b03s1=$14
mpy $44,$35,$14

# qhasm: a8 = select bytes from a811 by sel1
# asm 1: shufb >a8=vec128#43,<a811=vec128#18,<a811=vec128#18,<sel1=vec128#23
# asm 2: shufb >a8=$45,<a811=$20,<a811=$20,<sel1=$25
shufb $45,$20,$20,$25

# qhasm: int32323232 r58    = (a1  & 0xffff) * (b47s1   & 0xffff)
# asm 1: mpy >r58=vec128#44,<a1=vec128#33,<b47s1=vec128#17
# asm 2: mpy >r58=$46,<a1=$35,<b47s1=$19
mpy $46,$35,$19

# qhasm: a9 = select bytes from a811 by sel3
# asm 1: shufb >a9=vec128#45,<a811=vec128#18,<a811=vec128#18,<sel3=vec128#25
# asm 2: shufb >a9=$47,<a811=$20,<a811=$20,<sel3=$27
shufb $47,$20,$20,$27

# qhasm: int32323232 r912   = (a1  & 0xffff) * (b811s1  & 0xffff)
# asm 1: mpy >r912=vec128#46,<a1=vec128#33,<b811s1=vec128#22
# asm 2: mpy >r912=$48,<a1=$35,<b811s1=$24
mpy $48,$35,$24

# qhasm: a10 = select bytes from a811 by sel5
# asm 1: shufb >a10=vec128#47,<a811=vec128#18,<a811=vec128#18,<sel5=vec128#27
# asm 2: shufb >a10=$49,<a811=$20,<a811=$20,<sel5=$29
shufb $49,$20,$20,$29

# qhasm: int32323232 r1316  = (a1  & 0xffff) * (b1215s1 & 0xffff)
# asm 1: mpy >r1316=vec128#48,<a1=vec128#33,<b1215s1=vec128#28
# asm 2: mpy >r1316=$50,<a1=$35,<b1215s1=$30
mpy $50,$35,$30

# qhasm: a11 = select bytes from a811 by sel7
# asm 1: shufb >a11=vec128#18,<a811=vec128#18,<a811=vec128#18,<sel7=vec128#29
# asm 2: shufb >a11=$20,<a811=$20,<a811=$20,<sel7=$31
shufb $20,$20,$20,$31

# qhasm: int32323232 r1720  = (a1  & 0xffff) * (b1619s1 & 0xffff)
# asm 1: mpy >r1720=vec128#33,<a1=vec128#33,<b1619s1=vec128#6
# asm 2: mpy >r1720=$35,<a1=$35,<b1619s1=$8
mpy $35,$35,$8

# qhasm: a12 = select bytes from a1215 by sel1
# asm 1: shufb >a12=vec128#49,<a1215=vec128#20,<a1215=vec128#20,<sel1=vec128#23
# asm 2: shufb >a12=$51,<a1215=$22,<a1215=$22,<sel1=$25
shufb $51,$22,$22,$25

# qhasm: int32323232 r25    = (a2  & 0xffff) * (b03s2   & 0xffff)
# asm 1: mpy >r25=vec128#50,<a2=vec128#34,<b03s2=vec128#13
# asm 2: mpy >r25=$52,<a2=$36,<b03s2=$15
mpy $52,$36,$15

# qhasm: a13 = select bytes from a1215 by sel3
# asm 1: shufb >a13=vec128#51,<a1215=vec128#20,<a1215=vec128#20,<sel3=vec128#25
# asm 2: shufb >a13=$53,<a1215=$22,<a1215=$22,<sel3=$27
shufb $53,$22,$22,$27

# qhasm: int32323232 r69    = (a2  & 0xffff) * (b47s2   & 0xffff)
# asm 1: mpy >r69=vec128#52,<a2=vec128#34,<b47s2=vec128#19
# asm 2: mpy >r69=$54,<a2=$36,<b47s2=$21
mpy $54,$36,$21

# qhasm: a14 = select bytes from a1215 by sel5
# asm 1: shufb >a14=vec128#53,<a1215=vec128#20,<a1215=vec128#20,<sel5=vec128#27
# asm 2: shufb >a14=$55,<a1215=$22,<a1215=$22,<sel5=$29
shufb $55,$22,$22,$29

# qhasm: int32323232 r1013  = (a2  & 0xffff) * (b811s2  & 0xffff)
# asm 1: mpy >r1013=vec128#54,<a2=vec128#34,<b811s2=vec128#24
# asm 2: mpy >r1013=$56,<a2=$36,<b811s2=$26
mpy $56,$36,$26

# qhasm: a15 = select bytes from a1215 by sel7
# asm 1: shufb >a15=vec128#20,<a1215=vec128#20,<a1215=vec128#20,<sel7=vec128#29
# asm 2: shufb >a15=$22,<a1215=$22,<a1215=$22,<sel7=$31
shufb $22,$22,$22,$31

# qhasm: int32323232 r1417  = (a2  & 0xffff) * (b1215s2 & 0xffff)
# asm 1: mpy >r1417=vec128#55,<a2=vec128#34,<b1215s2=vec128#30
# asm 2: mpy >r1417=$57,<a2=$36,<b1215s2=$32
mpy $57,$36,$32

# qhasm: a16 = select bytes from a1619 by sel1
# asm 1: shufb >a16=vec128#23,<a1619=vec128#2,<a1619=vec128#2,<sel1=vec128#23
# asm 2: shufb >a16=$25,<a1619=$4,<a1619=$4,<sel1=$25
shufb $25,$4,$4,$25

# qhasm: int32323232 r1821  = (a2  & 0xffff) * (b1619s2 & 0xffff)
# asm 1: mpy >r1821=vec128#34,<a2=vec128#34,<b1619s2=vec128#7
# asm 2: mpy >r1821=$36,<a2=$36,<b1619s2=$9
mpy $36,$36,$9

# qhasm: a17 = select bytes from a1619 by sel3
# asm 1: shufb >a17=vec128#25,<a1619=vec128#2,<a1619=vec128#2,<sel3=vec128#25
# asm 2: shufb >a17=$27,<a1619=$4,<a1619=$4,<sel3=$27
shufb $27,$4,$4,$27

# qhasm: int32323232 r36    = (a3  & 0xffff) * (b03s3   & 0xffff)
# asm 1: mpy >r36=vec128#56,<a3=vec128#14,<b03s3=vec128#15
# asm 2: mpy >r36=$58,<a3=$16,<b03s3=$17
mpy $58,$16,$17

# qhasm: a18 = select bytes from a1619 by sel5
# asm 1: shufb >a18=vec128#27,<a1619=vec128#2,<a1619=vec128#2,<sel5=vec128#27
# asm 2: shufb >a18=$29,<a1619=$4,<a1619=$4,<sel5=$29
shufb $29,$4,$4,$29

# qhasm: int32323232 r710   = (a3  & 0xffff) * (b47s3   & 0xffff)
# asm 1: mpy >r710=vec128#57,<a3=vec128#14,<b47s3=vec128#21
# asm 2: mpy >r710=$59,<a3=$16,<b47s3=$23
mpy $59,$16,$23

# qhasm: a19 = select bytes from a1619 by sel7
# asm 1: shufb >a19=vec128#2,<a1619=vec128#2,<a1619=vec128#2,<sel7=vec128#29
# asm 2: shufb >a19=$4,<a1619=$4,<a1619=$4,<sel7=$31
shufb $4,$4,$4,$31

# qhasm: int32323232 r1114  = (a3  & 0xffff) * (b811s3  & 0xffff)
# asm 1: mpy >r1114=vec128#29,<a3=vec128#14,<b811s3=vec128#26
# asm 2: mpy >r1114=$31,<a3=$16,<b811s3=$28
mpy $31,$16,$28

# qhasm: sel01 = extern(select01)
# asm 1: lqr <sel01=vec128#58,select01
# asm 2: lqr <sel01=$60,select01
lqr $60,select01

# qhasm: int32323232 r1518  = (a3  & 0xffff) * (b1215s3 & 0xffff)
# asm 1: mpy >r1518=vec128#59,<a3=vec128#14,<b1215s3=vec128#32
# asm 2: mpy >r1518=$61,<a3=$16,<b1215s3=$34
mpy $61,$16,$34

# qhasm: sel12 = extern(select12)
# asm 1: lqr <sel12=vec128#60,select12
# asm 2: lqr <sel12=$62,select12
lqr $62,select12

# qhasm: int32323232 r1922  = (a3  & 0xffff) * (b1619s3 & 0xffff)
# asm 1: mpy >r1922=vec128#14,<a3=vec128#14,<b1619s3=vec128#8
# asm 2: mpy >r1922=$16,<a3=$16,<b1619s3=$10
mpy $16,$16,$10

# qhasm: sel23 = extern(select23)
# asm 1: lqr <sel23=vec128#61,select23
# asm 2: lqr <sel23=$63,select23
lqr $63,select23

# qhasm: int32323232 r47   += (a4  & 0xffff) * (b03   & 0xffff)
# asm 1: mpya >r47=vec128#36,<a4=vec128#37,<b03=vec128#4,<r47=vec128#36
# asm 2: mpya >r47=$38,<a4=$39,<b03=$6,<r47=$38
mpya $38,$39,$6,$38

# qhasm: sel30 = extern(select30)
# asm 1: lqr <sel30=vec128#62,select30
# asm 2: lqr <sel30=$64,select30
lqr $64,select30

# qhasm: int32323232 r811  += (a4  & 0xffff) * (b47   & 0xffff)
# asm 1: mpya >r811=vec128#38,<a4=vec128#37,<b47=vec128#9,<r811=vec128#38
# asm 2: mpya >r811=$40,<a4=$39,<b47=$11,<r811=$40
mpya $40,$39,$11,$40

# qhasm: redcoeffmask = extern(redCoeffMask)
# asm 1: lqr <redcoeffmask=vec128#63,redCoeffMask
# asm 2: lqr <redcoeffmask=$65,redCoeffMask
lqr $65,redCoeffMask

# qhasm: int32323232 r1215 += (a4  & 0xffff) * (b811  & 0xffff)
# asm 1: mpya >r1215=vec128#40,<a4=vec128#37,<b811=vec128#10,<r1215=vec128#40
# asm 2: mpya >r1215=$42,<a4=$39,<b811=$12,<r1215=$42
mpya $42,$39,$12,$42

# qhasm: redcoeffmaskend = extern(redCoeffMaskEnd)
# asm 1: lqr <redcoeffmaskend=vec128#64,redCoeffMaskEnd
# asm 2: lqr <redcoeffmaskend=$66,redCoeffMaskEnd
lqr $66,redCoeffMaskEnd

# qhasm: int32323232 r1619 += (a4  & 0xffff) * (b1215 & 0xffff)
# asm 1: mpya >r1619=vec128#31,<a4=vec128#37,<b1215=vec128#11,<r1619=vec128#31
# asm 2: mpya >r1619=$33,<a4=$39,<b1215=$13,<r1619=$33
mpya $33,$39,$13,$33

# qhasm: redcoeffmaskveryend = extern(redCoeffMaskVeryEnd)
# asm 1: lqr <redcoeffmaskveryend=vec128#65,redCoeffMaskVeryEnd
# asm 2: lqr <redcoeffmaskveryend=$67,redCoeffMaskVeryEnd
lqr $67,redCoeffMaskVeryEnd

# qhasm: int32323232 r2023  = (a4  & 0xffff) * (b1619 & 0xffff)
# asm 1: mpy >r2023=vec128#37,<a4=vec128#37,<b1619=vec128#3
# asm 2: mpy >r2023=$39,<a4=$39,<b1619=$5
mpy $39,$39,$5

# qhasm: comb13 = extern(combine13)
# asm 1: lqr <comb13=vec128#66,combine13
# asm 2: lqr <comb13=$68,combine13
lqr $68,combine13

# qhasm: int32323232 r58   += (a5  & 0xffff) * (b03s1   & 0xffff)
# asm 1: mpya >r58=vec128#44,<a5=vec128#39,<b03s1=vec128#12,<r58=vec128#44
# asm 2: mpya >r58=$46,<a5=$41,<b03s1=$14,<r58=$46
mpya $46,$41,$14,$46

# qhasm: comb22 = extern(combine22)
# asm 1: lqr <comb22=vec128#67,combine22
# asm 2: lqr <comb22=$69,combine22
lqr $69,combine22

# qhasm: int32323232 r912  += (a5  & 0xffff) * (b47s1   & 0xffff)
# asm 1: mpya >r912=vec128#46,<a5=vec128#39,<b47s1=vec128#17,<r912=vec128#46
# asm 2: mpya >r912=$48,<a5=$41,<b47s1=$19,<r912=$48
mpya $48,$41,$19,$48

# qhasm: comb31 = extern(combine31)
# asm 1: lqr <comb31=vec128#68,combine31
# asm 2: lqr <comb31=$70,combine31
lqr $70,combine31

# qhasm: int32323232 r1316 += (a5  & 0xffff) * (b811s1  & 0xffff)
# asm 1: mpya >r1316=vec128#48,<a5=vec128#39,<b811s1=vec128#22,<r1316=vec128#48
# asm 2: mpya >r1316=$50,<a5=$41,<b811s1=$24,<r1316=$50
mpya $50,$41,$24,$50

# qhasm: comb31 = extern(combine31)
# asm 1: lqr <comb31=vec128#68,combine31
# asm 2: lqr <comb31=$70,combine31
lqr $70,combine31

# qhasm: int32323232 r1720 += (a5  & 0xffff) * (b1215s1 & 0xffff)
# asm 1: mpya >r1720=vec128#33,<a5=vec128#39,<b1215s1=vec128#28,<r1720=vec128#33
# asm 2: mpya >r1720=$35,<a5=$41,<b1215s1=$30,<r1720=$35
mpya $35,$41,$30,$35

# qhasm: vec19 = extern(_vec19)
# asm 1: lqr <vec19=vec128#69,_vec19
# asm 2: lqr <vec19=$71,_vec19
lqr $71,_vec19

# qhasm: int32323232 r2124  = (a5  & 0xffff) * (b1619s1 & 0xffff)
# asm 1: mpy >r2124=vec128#39,<a5=vec128#39,<b1619s1=vec128#6
# asm 2: mpy >r2124=$41,<a5=$41,<b1619s1=$8
mpy $41,$41,$8

# qhasm: int32323232 r69   += (a6  & 0xffff) * (b03s2   & 0xffff)
# asm 1: mpya >r69=vec128#52,<a6=vec128#41,<b03s2=vec128#13,<r69=vec128#52
# asm 2: mpya >r69=$54,<a6=$43,<b03s2=$15,<r69=$54
mpya $54,$43,$15,$54

# qhasm: int32323232 r1013 += (a6  & 0xffff) * (b47s2   & 0xffff)
# asm 1: mpya >r1013=vec128#54,<a6=vec128#41,<b47s2=vec128#19,<r1013=vec128#54
# asm 2: mpya >r1013=$56,<a6=$43,<b47s2=$21,<r1013=$56
mpya $56,$43,$21,$56

# qhasm: int32323232 r1417 += (a6  & 0xffff) * (b811s2  & 0xffff)
# asm 1: mpya >r1417=vec128#55,<a6=vec128#41,<b811s2=vec128#24,<r1417=vec128#55
# asm 2: mpya >r1417=$57,<a6=$43,<b811s2=$26,<r1417=$57
mpya $57,$43,$26,$57

# qhasm: int32323232 r1821 += (a6  & 0xffff) * (b1215s2 & 0xffff)
# asm 1: mpya >r1821=vec128#34,<a6=vec128#41,<b1215s2=vec128#30,<r1821=vec128#34
# asm 2: mpya >r1821=$36,<a6=$43,<b1215s2=$32,<r1821=$36
mpya $36,$43,$32,$36

# qhasm: int32323232 r2225  = (a6  & 0xffff) * (b1619s2 & 0xffff)
# asm 1: mpy >r2225=vec128#41,<a6=vec128#41,<b1619s2=vec128#7
# asm 2: mpy >r2225=$43,<a6=$43,<b1619s2=$9
mpy $43,$43,$9

# qhasm: int32323232 r710  += (a7  & 0xffff) * (b03s3   & 0xffff)
# asm 1: mpya >r710=vec128#57,<a7=vec128#16,<b03s3=vec128#15,<r710=vec128#57
# asm 2: mpya >r710=$59,<a7=$18,<b03s3=$17,<r710=$59
mpya $59,$18,$17,$59

# qhasm: int32323232 r1114 += (a7  & 0xffff) * (b47s3   & 0xffff)
# asm 1: mpya >r1114=vec128#29,<a7=vec128#16,<b47s3=vec128#21,<r1114=vec128#29
# asm 2: mpya >r1114=$31,<a7=$18,<b47s3=$23,<r1114=$31
mpya $31,$18,$23,$31

# qhasm: int32323232 r1518 += (a7  & 0xffff) * (b811s3  & 0xffff)
# asm 1: mpya >r1518=vec128#59,<a7=vec128#16,<b811s3=vec128#26,<r1518=vec128#59
# asm 2: mpya >r1518=$61,<a7=$18,<b811s3=$28,<r1518=$61
mpya $61,$18,$28,$61

# qhasm: int32323232 r1922 += (a7  & 0xffff) * (b1215s3 & 0xffff)
# asm 1: mpya >r1922=vec128#14,<a7=vec128#16,<b1215s3=vec128#32,<r1922=vec128#14
# asm 2: mpya >r1922=$16,<a7=$18,<b1215s3=$34,<r1922=$16
mpya $16,$18,$34,$16

# qhasm: int32323232 r2326  = (a7  & 0xffff) * (b1619s3 & 0xffff)
# asm 1: mpy >r2326=vec128#16,<a7=vec128#16,<b1619s3=vec128#8
# asm 2: mpy >r2326=$18,<a7=$18,<b1619s3=$10
mpy $18,$18,$10

# qhasm: int32323232 r811  += (a8  & 0xffff) * (b03   & 0xffff)
# asm 1: mpya >r811=vec128#38,<a8=vec128#43,<b03=vec128#4,<r811=vec128#38
# asm 2: mpya >r811=$40,<a8=$45,<b03=$6,<r811=$40
mpya $40,$45,$6,$40

# qhasm: int32323232 r1215 += (a8  & 0xffff) * (b47   & 0xffff)
# asm 1: mpya >r1215=vec128#40,<a8=vec128#43,<b47=vec128#9,<r1215=vec128#40
# asm 2: mpya >r1215=$42,<a8=$45,<b47=$11,<r1215=$42
mpya $42,$45,$11,$42

# qhasm: int32323232 r1619 += (a8  & 0xffff) * (b811  & 0xffff)
# asm 1: mpya >r1619=vec128#31,<a8=vec128#43,<b811=vec128#10,<r1619=vec128#31
# asm 2: mpya >r1619=$33,<a8=$45,<b811=$12,<r1619=$33
mpya $33,$45,$12,$33

# qhasm: int32323232 r2023 += (a8  & 0xffff) * (b1215 & 0xffff)
# asm 1: mpya >r2023=vec128#37,<a8=vec128#43,<b1215=vec128#11,<r2023=vec128#37
# asm 2: mpya >r2023=$39,<a8=$45,<b1215=$13,<r2023=$39
mpya $39,$45,$13,$39

# qhasm: int32323232 r2427  = (a8  & 0xffff) * (b1619 & 0xffff)
# asm 1: mpy >r2427=vec128#43,<a8=vec128#43,<b1619=vec128#3
# asm 2: mpy >r2427=$45,<a8=$45,<b1619=$5
mpy $45,$45,$5

# qhasm: int32323232 r912  += (a9  & 0xffff) * (b03s1   & 0xffff)
# asm 1: mpya >r912=vec128#46,<a9=vec128#45,<b03s1=vec128#12,<r912=vec128#46
# asm 2: mpya >r912=$48,<a9=$47,<b03s1=$14,<r912=$48
mpya $48,$47,$14,$48

# qhasm: int32323232 r1316 += (a9  & 0xffff) * (b47s1   & 0xffff)
# asm 1: mpya >r1316=vec128#48,<a9=vec128#45,<b47s1=vec128#17,<r1316=vec128#48
# asm 2: mpya >r1316=$50,<a9=$47,<b47s1=$19,<r1316=$50
mpya $50,$47,$19,$50

# qhasm: int32323232 r1720 += (a9  & 0xffff) * (b811s1  & 0xffff)
# asm 1: mpya >r1720=vec128#33,<a9=vec128#45,<b811s1=vec128#22,<r1720=vec128#33
# asm 2: mpya >r1720=$35,<a9=$47,<b811s1=$24,<r1720=$35
mpya $35,$47,$24,$35

# qhasm: int32323232 r2124 += (a9  & 0xffff) * (b1215s1 & 0xffff)
# asm 1: mpya >r2124=vec128#39,<a9=vec128#45,<b1215s1=vec128#28,<r2124=vec128#39
# asm 2: mpya >r2124=$41,<a9=$47,<b1215s1=$30,<r2124=$41
mpya $41,$47,$30,$41

# qhasm: int32323232 r2528  = (a9  & 0xffff) * (b1619s1 & 0xffff)
# asm 1: mpy >r2528=vec128#45,<a9=vec128#45,<b1619s1=vec128#6
# asm 2: mpy >r2528=$47,<a9=$47,<b1619s1=$8
mpy $47,$47,$8

# qhasm: int32323232 r1013 += (a10 & 0xffff) * (b03s2   & 0xffff)
# asm 1: mpya >r1013=vec128#54,<a10=vec128#47,<b03s2=vec128#13,<r1013=vec128#54
# asm 2: mpya >r1013=$56,<a10=$49,<b03s2=$15,<r1013=$56
mpya $56,$49,$15,$56

# qhasm: int32323232 r1417 += (a10 & 0xffff) * (b47s2   & 0xffff)
# asm 1: mpya >r1417=vec128#55,<a10=vec128#47,<b47s2=vec128#19,<r1417=vec128#55
# asm 2: mpya >r1417=$57,<a10=$49,<b47s2=$21,<r1417=$57
mpya $57,$49,$21,$57

# qhasm: int32323232 r1821 += (a10 & 0xffff) * (b811s2  & 0xffff)
# asm 1: mpya >r1821=vec128#34,<a10=vec128#47,<b811s2=vec128#24,<r1821=vec128#34
# asm 2: mpya >r1821=$36,<a10=$49,<b811s2=$26,<r1821=$36
mpya $36,$49,$26,$36

# qhasm: int32323232 r2225 += (a10 & 0xffff) * (b1215s2 & 0xffff)
# asm 1: mpya >r2225=vec128#41,<a10=vec128#47,<b1215s2=vec128#30,<r2225=vec128#41
# asm 2: mpya >r2225=$43,<a10=$49,<b1215s2=$32,<r2225=$43
mpya $43,$49,$32,$43

# qhasm: int32323232 r2629  = (a10 & 0xffff) * (b1619s2 & 0xffff)
# asm 1: mpy >r2629=vec128#47,<a10=vec128#47,<b1619s2=vec128#7
# asm 2: mpy >r2629=$49,<a10=$49,<b1619s2=$9
mpy $49,$49,$9

# qhasm: int32323232 r1114 += (a11 & 0xffff) * (b03s3   & 0xffff)
# asm 1: mpya >r1114=vec128#29,<a11=vec128#18,<b03s3=vec128#15,<r1114=vec128#29
# asm 2: mpya >r1114=$31,<a11=$20,<b03s3=$17,<r1114=$31
mpya $31,$20,$17,$31

# qhasm: int32323232 r1518 += (a11 & 0xffff) * (b47s3   & 0xffff)
# asm 1: mpya >r1518=vec128#59,<a11=vec128#18,<b47s3=vec128#21,<r1518=vec128#59
# asm 2: mpya >r1518=$61,<a11=$20,<b47s3=$23,<r1518=$61
mpya $61,$20,$23,$61

# qhasm: int32323232 r1922 += (a11 & 0xffff) * (b811s3  & 0xffff)
# asm 1: mpya >r1922=vec128#14,<a11=vec128#18,<b811s3=vec128#26,<r1922=vec128#14
# asm 2: mpya >r1922=$16,<a11=$20,<b811s3=$28,<r1922=$16
mpya $16,$20,$28,$16

# qhasm: int32323232 r2326 += (a11 & 0xffff) * (b1215s3 & 0xffff)
# asm 1: mpya >r2326=vec128#16,<a11=vec128#18,<b1215s3=vec128#32,<r2326=vec128#16
# asm 2: mpya >r2326=$18,<a11=$20,<b1215s3=$34,<r2326=$18
mpya $18,$20,$34,$18

# qhasm: int32323232 r2730  = (a11 & 0xffff) * (b1619s3 & 0xffff)
# asm 1: mpy >r2730=vec128#18,<a11=vec128#18,<b1619s3=vec128#8
# asm 2: mpy >r2730=$20,<a11=$20,<b1619s3=$10
mpy $20,$20,$10

# qhasm: int32323232 r1215 += (a12 & 0xffff) * (b03   & 0xffff)
# asm 1: mpya >r1215=vec128#40,<a12=vec128#49,<b03=vec128#4,<r1215=vec128#40
# asm 2: mpya >r1215=$42,<a12=$51,<b03=$6,<r1215=$42
mpya $42,$51,$6,$42

# qhasm: int32323232 r1619 += (a12 & 0xffff) * (b47   & 0xffff)
# asm 1: mpya >r1619=vec128#31,<a12=vec128#49,<b47=vec128#9,<r1619=vec128#31
# asm 2: mpya >r1619=$33,<a12=$51,<b47=$11,<r1619=$33
mpya $33,$51,$11,$33

# qhasm: int32323232 r2023 += (a12 & 0xffff) * (b811  & 0xffff)
# asm 1: mpya >r2023=vec128#37,<a12=vec128#49,<b811=vec128#10,<r2023=vec128#37
# asm 2: mpya >r2023=$39,<a12=$51,<b811=$12,<r2023=$39
mpya $39,$51,$12,$39

# qhasm: int32323232 r2427 += (a12 & 0xffff) * (b1215 & 0xffff)
# asm 1: mpya >r2427=vec128#43,<a12=vec128#49,<b1215=vec128#11,<r2427=vec128#43
# asm 2: mpya >r2427=$45,<a12=$51,<b1215=$13,<r2427=$45
mpya $45,$51,$13,$45

# qhasm: int32323232 r2831  = (a12 & 0xffff) * (b1619 & 0xffff)
# asm 1: mpy >r2831=vec128#49,<a12=vec128#49,<b1619=vec128#3
# asm 2: mpy >r2831=$51,<a12=$51,<b1619=$5
mpy $51,$51,$5

# qhasm: int32323232 r1316 += (a13 & 0xffff) * (b03s1   & 0xffff)
# asm 1: mpya >r1316=vec128#48,<a13=vec128#51,<b03s1=vec128#12,<r1316=vec128#48
# asm 2: mpya >r1316=$50,<a13=$53,<b03s1=$14,<r1316=$50
mpya $50,$53,$14,$50

# qhasm: int32323232 r1720 += (a13 & 0xffff) * (b47s1   & 0xffff)
# asm 1: mpya >r1720=vec128#33,<a13=vec128#51,<b47s1=vec128#17,<r1720=vec128#33
# asm 2: mpya >r1720=$35,<a13=$53,<b47s1=$19,<r1720=$35
mpya $35,$53,$19,$35

# qhasm: int32323232 r2124 += (a13 & 0xffff) * (b811s1  & 0xffff)
# asm 1: mpya >r2124=vec128#39,<a13=vec128#51,<b811s1=vec128#22,<r2124=vec128#39
# asm 2: mpya >r2124=$41,<a13=$53,<b811s1=$24,<r2124=$41
mpya $41,$53,$24,$41

# qhasm: int32323232 r2528 += (a13 & 0xffff) * (b1215s1 & 0xffff)
# asm 1: mpya >r2528=vec128#45,<a13=vec128#51,<b1215s1=vec128#28,<r2528=vec128#45
# asm 2: mpya >r2528=$47,<a13=$53,<b1215s1=$30,<r2528=$47
mpya $47,$53,$30,$47

# qhasm: int32323232 r2932  = (a13 & 0xffff) * (b1619s1 & 0xffff)
# asm 1: mpy >r2932=vec128#51,<a13=vec128#51,<b1619s1=vec128#6
# asm 2: mpy >r2932=$53,<a13=$53,<b1619s1=$8
mpy $53,$53,$8

# qhasm: int32323232 r1417 += (a14 & 0xffff) * (b03s2   & 0xffff)
# asm 1: mpya >r1417=vec128#55,<a14=vec128#53,<b03s2=vec128#13,<r1417=vec128#55
# asm 2: mpya >r1417=$57,<a14=$55,<b03s2=$15,<r1417=$57
mpya $57,$55,$15,$57

# qhasm: int32323232 r1821 += (a14 & 0xffff) * (b47s2   & 0xffff)
# asm 1: mpya >r1821=vec128#34,<a14=vec128#53,<b47s2=vec128#19,<r1821=vec128#34
# asm 2: mpya >r1821=$36,<a14=$55,<b47s2=$21,<r1821=$36
mpya $36,$55,$21,$36

# qhasm: int32323232 r2225 += (a14 & 0xffff) * (b811s2  & 0xffff)
# asm 1: mpya >r2225=vec128#41,<a14=vec128#53,<b811s2=vec128#24,<r2225=vec128#41
# asm 2: mpya >r2225=$43,<a14=$55,<b811s2=$26,<r2225=$43
mpya $43,$55,$26,$43

# qhasm: int32323232 r2629 += (a14 & 0xffff) * (b1215s2 & 0xffff)
# asm 1: mpya >r2629=vec128#47,<a14=vec128#53,<b1215s2=vec128#30,<r2629=vec128#47
# asm 2: mpya >r2629=$49,<a14=$55,<b1215s2=$32,<r2629=$49
mpya $49,$55,$32,$49

# qhasm: int32323232 r3033  = (a14 & 0xffff) * (b1619s2 & 0xffff)
# asm 1: mpy >r3033=vec128#53,<a14=vec128#53,<b1619s2=vec128#7
# asm 2: mpy >r3033=$55,<a14=$55,<b1619s2=$9
mpy $55,$55,$9

# qhasm: int32323232 r1518 += (a15 & 0xffff) * (b03s3   & 0xffff)
# asm 1: mpya >r1518=vec128#59,<a15=vec128#20,<b03s3=vec128#15,<r1518=vec128#59
# asm 2: mpya >r1518=$61,<a15=$22,<b03s3=$17,<r1518=$61
mpya $61,$22,$17,$61

# qhasm: int32323232 r1922 += (a15 & 0xffff) * (b47s3   & 0xffff)
# asm 1: mpya >r1922=vec128#14,<a15=vec128#20,<b47s3=vec128#21,<r1922=vec128#14
# asm 2: mpya >r1922=$16,<a15=$22,<b47s3=$23,<r1922=$16
mpya $16,$22,$23,$16

# qhasm: int32323232 r2326 += (a15 & 0xffff) * (b811s3  & 0xffff)
# asm 1: mpya >r2326=vec128#16,<a15=vec128#20,<b811s3=vec128#26,<r2326=vec128#16
# asm 2: mpya >r2326=$18,<a15=$22,<b811s3=$28,<r2326=$18
mpya $18,$22,$28,$18

# qhasm: int32323232 r2730 += (a15 & 0xffff) * (b1215s3 & 0xffff)
# asm 1: mpya >r2730=vec128#18,<a15=vec128#20,<b1215s3=vec128#32,<r2730=vec128#18
# asm 2: mpya >r2730=$20,<a15=$22,<b1215s3=$34,<r2730=$20
mpya $20,$22,$34,$20

# qhasm: int32323232 r3134  = (a15 & 0xffff) * (b1619s3 & 0xffff)
# asm 1: mpy >r3134=vec128#20,<a15=vec128#20,<b1619s3=vec128#8
# asm 2: mpy >r3134=$22,<a15=$22,<b1619s3=$10
mpy $22,$22,$10

# qhasm: int32323232 r1619 += (a16 & 0xffff) * (b03   & 0xffff)
# asm 1: mpya >r1619=vec128#4,<a16=vec128#23,<b03=vec128#4,<r1619=vec128#31
# asm 2: mpya >r1619=$6,<a16=$25,<b03=$6,<r1619=$33
mpya $6,$25,$6,$33

# qhasm: int32323232 r2023 += (a16 & 0xffff) * (b47   & 0xffff)
# asm 1: mpya >r2023=vec128#9,<a16=vec128#23,<b47=vec128#9,<r2023=vec128#37
# asm 2: mpya >r2023=$11,<a16=$25,<b47=$11,<r2023=$39
mpya $11,$25,$11,$39

# qhasm: lnop
 lnop

# qhasm: int32323232 r2427 += (a16 & 0xffff) * (b811  & 0xffff)
# asm 1: mpya >r2427=vec128#10,<a16=vec128#23,<b811=vec128#10,<r2427=vec128#43
# asm 2: mpya >r2427=$12,<a16=$25,<b811=$12,<r2427=$45
mpya $12,$25,$12,$45

# qhasm: tmp10 = combine zero  and r14   by comb13
# asm 1: shufb >tmp10=vec128#31,<zero=vec128#5,<r14=vec128#42,<comb13=vec128#66
# asm 2: shufb >tmp10=$33,<zero=$7,<r14=$44,<comb13=$68
shufb $33,$7,$44,$68

# qhasm: int32323232 r2831 += (a16 & 0xffff) * (b1215 & 0xffff)
# asm 1: mpya >r2831=vec128#11,<a16=vec128#23,<b1215=vec128#11,<r2831=vec128#49
# asm 2: mpya >r2831=$13,<a16=$25,<b1215=$13,<r2831=$51
mpya $13,$25,$13,$51

# qhasm: tmp20 = combine zero  and r25   by comb22
# asm 1: shufb >tmp20=vec128#37,<zero=vec128#5,<r25=vec128#50,<comb22=vec128#67
# asm 2: shufb >tmp20=$39,<zero=$7,<r25=$52,<comb22=$69
shufb $39,$7,$52,$69

# qhasm: int32323232 r3235  = (a16 & 0xffff) * (b1619 & 0xffff)
# asm 1: mpy >r3235=vec128#3,<a16=vec128#23,<b1619=vec128#3
# asm 2: mpy >r3235=$5,<a16=$25,<b1619=$5
mpy $5,$25,$5

# qhasm: tmp30 = combine zero  and r36   by comb31
# asm 1: shufb >tmp30=vec128#23,<zero=vec128#5,<r36=vec128#56,<comb31=vec128#68
# asm 2: shufb >tmp30=$25,<zero=$7,<r36=$58,<comb31=$70
shufb $25,$7,$58,$70

# qhasm: int32323232 r1720 += (a17 & 0xffff) * (b03s1   & 0xffff)
# asm 1: mpya >r1720=vec128#12,<a17=vec128#25,<b03s1=vec128#12,<r1720=vec128#33
# asm 2: mpya >r1720=$14,<a17=$27,<b03s1=$14,<r1720=$35
mpya $14,$27,$14,$35

# qhasm: tmp11 = combine r14   and r58   by comb13
# asm 1: shufb >tmp11=vec128#33,<r14=vec128#42,<r58=vec128#44,<comb13=vec128#66
# asm 2: shufb >tmp11=$35,<r14=$44,<r58=$46,<comb13=$68
shufb $35,$44,$46,$68

# qhasm: int32323232 r2124 += (a17 & 0xffff) * (b47s1   & 0xffff)
# asm 1: mpya >r2124=vec128#17,<a17=vec128#25,<b47s1=vec128#17,<r2124=vec128#39
# asm 2: mpya >r2124=$19,<a17=$27,<b47s1=$19,<r2124=$41
mpya $19,$27,$19,$41

# qhasm: tmp21 = combine r25   and r69   by comb22
# asm 1: shufb >tmp21=vec128#39,<r25=vec128#50,<r69=vec128#52,<comb22=vec128#67
# asm 2: shufb >tmp21=$41,<r25=$52,<r69=$54,<comb22=$69
shufb $41,$52,$54,$69

# qhasm: int32323232 r2528 += (a17 & 0xffff) * (b811s1  & 0xffff)
# asm 1: mpya >r2528=vec128#22,<a17=vec128#25,<b811s1=vec128#22,<r2528=vec128#45
# asm 2: mpya >r2528=$24,<a17=$27,<b811s1=$24,<r2528=$47
mpya $24,$27,$24,$47

# qhasm: tmp31 = combine r36   and r710  by comb31
# asm 1: shufb >tmp31=vec128#42,<r36=vec128#56,<r710=vec128#57,<comb31=vec128#68
# asm 2: shufb >tmp31=$44,<r36=$58,<r710=$59,<comb31=$70
shufb $44,$58,$59,$70

# qhasm: int32323232 r2932 += (a17 & 0xffff) * (b1215s1 & 0xffff)
# asm 1: mpya >r2932=vec128#28,<a17=vec128#25,<b1215s1=vec128#28,<r2932=vec128#51
# asm 2: mpya >r2932=$30,<a17=$27,<b1215s1=$30,<r2932=$53
mpya $30,$27,$30,$53

# qhasm: tmp12 = combine r58   and r912  by comb13
# asm 1: shufb >tmp12=vec128#43,<r58=vec128#44,<r912=vec128#46,<comb13=vec128#66
# asm 2: shufb >tmp12=$45,<r58=$46,<r912=$48,<comb13=$68
shufb $45,$46,$48,$68

# qhasm: int32323232 r3336  = (a17 & 0xffff) * (b1619s1 & 0xffff)
# asm 1: mpy >r3336=vec128#6,<a17=vec128#25,<b1619s1=vec128#6
# asm 2: mpy >r3336=$8,<a17=$27,<b1619s1=$8
mpy $8,$27,$8

# qhasm: tmp22 = combine r69   and r1013 by comb22
# asm 1: shufb >tmp22=vec128#25,<r69=vec128#52,<r1013=vec128#54,<comb22=vec128#67
# asm 2: shufb >tmp22=$27,<r69=$54,<r1013=$56,<comb22=$69
shufb $27,$54,$56,$69

# qhasm: int32323232 r1821 += (a18 & 0xffff) * (b03s2   & 0xffff)
# asm 1: mpya >r1821=vec128#13,<a18=vec128#27,<b03s2=vec128#13,<r1821=vec128#34
# asm 2: mpya >r1821=$15,<a18=$29,<b03s2=$15,<r1821=$36
mpya $15,$29,$15,$36

# qhasm: tmp32 = combine r710  and r1114 by comb31
# asm 1: shufb >tmp32=vec128#34,<r710=vec128#57,<r1114=vec128#29,<comb31=vec128#68
# asm 2: shufb >tmp32=$36,<r710=$59,<r1114=$31,<comb31=$70
shufb $36,$59,$31,$70

# qhasm: int32323232 r2225 += (a18 & 0xffff) * (b47s2   & 0xffff)
# asm 1: mpya >r2225=vec128#19,<a18=vec128#27,<b47s2=vec128#19,<r2225=vec128#41
# asm 2: mpya >r2225=$21,<a18=$29,<b47s2=$21,<r2225=$43
mpya $21,$29,$21,$43

# qhasm: tmp13 = combine r912  and r1316 by comb13
# asm 1: shufb >tmp13=vec128#41,<r912=vec128#46,<r1316=vec128#48,<comb13=vec128#66
# asm 2: shufb >tmp13=$43,<r912=$48,<r1316=$50,<comb13=$68
shufb $43,$48,$50,$68

# qhasm: int32323232 r2629 += (a18 & 0xffff) * (b811s2  & 0xffff)
# asm 1: mpya >r2629=vec128#24,<a18=vec128#27,<b811s2=vec128#24,<r2629=vec128#47
# asm 2: mpya >r2629=$26,<a18=$29,<b811s2=$26,<r2629=$49
mpya $26,$29,$26,$49

# qhasm: tmp23 = combine r1013 and r1417 by comb22
# asm 1: shufb >tmp23=vec128#44,<r1013=vec128#54,<r1417=vec128#55,<comb22=vec128#67
# asm 2: shufb >tmp23=$46,<r1013=$56,<r1417=$57,<comb22=$69
shufb $46,$56,$57,$69

# qhasm: int32323232 r3033 += (a18 & 0xffff) * (b1215s2 & 0xffff)
# asm 1: mpya >r3033=vec128#30,<a18=vec128#27,<b1215s2=vec128#30,<r3033=vec128#53
# asm 2: mpya >r3033=$32,<a18=$29,<b1215s2=$32,<r3033=$55
mpya $32,$29,$32,$55

# qhasm: tmp33 = combine r1114 and r1518 by comb31
# asm 1: shufb >tmp33=vec128#29,<r1114=vec128#29,<r1518=vec128#59,<comb31=vec128#68
# asm 2: shufb >tmp33=$31,<r1114=$31,<r1518=$61,<comb31=$70
shufb $31,$31,$61,$70

# qhasm: int32323232 r3437  = (a18 & 0xffff) * (b1619s2 & 0xffff)
# asm 1: mpy >r3437=vec128#7,<a18=vec128#27,<b1619s2=vec128#7
# asm 2: mpy >r3437=$9,<a18=$29,<b1619s2=$9
mpy $9,$29,$9

# qhasm: tmp14 = combine r1316 and r1720 by comb13
# asm 1: shufb >tmp14=vec128#27,<r1316=vec128#48,<r1720=vec128#12,<comb13=vec128#66
# asm 2: shufb >tmp14=$29,<r1316=$50,<r1720=$14,<comb13=$68
shufb $29,$50,$14,$68

# qhasm: int32323232 r1922 += (a19 & 0xffff) * (b03s3   & 0xffff)
# asm 1: mpya >r1922=vec128#14,<a19=vec128#2,<b03s3=vec128#15,<r1922=vec128#14
# asm 2: mpya >r1922=$16,<a19=$4,<b03s3=$17,<r1922=$16
mpya $16,$4,$17,$16

# qhasm: tmp15 = combine r1720 and r2124 by comb13
# asm 1: shufb >tmp15=vec128#12,<r1720=vec128#12,<r2124=vec128#17,<comb13=vec128#66
# asm 2: shufb >tmp15=$14,<r1720=$14,<r2124=$19,<comb13=$68
shufb $14,$14,$19,$68

# qhasm: int32323232 r2326 += (a19 & 0xffff) * (b47s3   & 0xffff)
# asm 1: mpya >r2326=vec128#15,<a19=vec128#2,<b47s3=vec128#21,<r2326=vec128#16
# asm 2: mpya >r2326=$17,<a19=$4,<b47s3=$23,<r2326=$18
mpya $17,$4,$23,$18

# qhasm: tmp16 = combine r2124 and r2528 by comb13
# asm 1: shufb >tmp16=vec128#16,<r2124=vec128#17,<r2528=vec128#22,<comb13=vec128#66
# asm 2: shufb >tmp16=$18,<r2124=$19,<r2528=$24,<comb13=$68
shufb $18,$19,$24,$68

# qhasm: int32323232 r2730 += (a19 & 0xffff) * (b811s3  & 0xffff)
# asm 1: mpya >r2730=vec128#17,<a19=vec128#2,<b811s3=vec128#26,<r2730=vec128#18
# asm 2: mpya >r2730=$19,<a19=$4,<b811s3=$28,<r2730=$20
mpya $19,$4,$28,$20

# qhasm: tmp17 = combine r2528 and r2932 by comb13
# asm 1: shufb >tmp17=vec128#18,<r2528=vec128#22,<r2932=vec128#28,<comb13=vec128#66
# asm 2: shufb >tmp17=$20,<r2528=$24,<r2932=$30,<comb13=$68
shufb $20,$24,$30,$68

# qhasm: int32323232 r3134 += (a19 & 0xffff) * (b1215s3 & 0xffff)
# asm 1: mpya >r3134=vec128#20,<a19=vec128#2,<b1215s3=vec128#32,<r3134=vec128#20
# asm 2: mpya >r3134=$22,<a19=$4,<b1215s3=$34,<r3134=$22
mpya $22,$4,$34,$22

# qhasm: tmp18 = combine r2932 and r3336 by comb13
# asm 1: shufb >tmp18=vec128#21,<r2932=vec128#28,<r3336=vec128#6,<comb13=vec128#66
# asm 2: shufb >tmp18=$23,<r2932=$30,<r3336=$8,<comb13=$68
shufb $23,$30,$8,$68

# qhasm: int32323232 r3538  = (a19 & 0xffff) * (b1619s3 & 0xffff)
# asm 1: mpy >r3538=vec128#2,<a19=vec128#2,<b1619s3=vec128#8
# asm 2: mpy >r3538=$4,<a19=$4,<b1619s3=$10
mpy $4,$4,$10

# qhasm: r3639 = combine r3336 and zero  by comb13
# asm 1: shufb >r3639=vec128#6,<r3336=vec128#6,<zero=vec128#5,<comb13=vec128#66
# asm 2: shufb >r3639=$8,<r3336=$8,<zero=$7,<comb13=$68
shufb $8,$8,$7,$68

# qhasm: int32323232 r03   += tmp10
# asm 1: a <r03=vec128#35,<r03=vec128#35,<tmp10=vec128#31
# asm 2: a <r03=$37,<r03=$37,<tmp10=$33
a $37,$37,$33

# qhasm: tmp24 = combine r1417 and r1821 by comb22
# asm 1: shufb >tmp24=vec128#8,<r1417=vec128#55,<r1821=vec128#13,<comb22=vec128#67
# asm 2: shufb >tmp24=$10,<r1417=$57,<r1821=$15,<comb22=$69
shufb $10,$57,$15,$69

# qhasm: int32323232 r47   += tmp11
# asm 1: a <r47=vec128#36,<r47=vec128#36,<tmp11=vec128#33
# asm 2: a <r47=$38,<r47=$38,<tmp11=$35
a $38,$38,$35

# qhasm: tmp25 = combine r1821 and r2225 by comb22
# asm 1: shufb >tmp25=vec128#13,<r1821=vec128#13,<r2225=vec128#19,<comb22=vec128#67
# asm 2: shufb >tmp25=$15,<r1821=$15,<r2225=$21,<comb22=$69
shufb $15,$15,$21,$69

# qhasm: int32323232 r811  += tmp12
# asm 1: a <r811=vec128#38,<r811=vec128#38,<tmp12=vec128#43
# asm 2: a <r811=$40,<r811=$40,<tmp12=$45
a $40,$40,$45

# qhasm: tmp26 = combine r2225 and r2629 by comb22
# asm 1: shufb >tmp26=vec128#19,<r2225=vec128#19,<r2629=vec128#24,<comb22=vec128#67
# asm 2: shufb >tmp26=$21,<r2225=$21,<r2629=$26,<comb22=$69
shufb $21,$21,$26,$69

# qhasm: int32323232 r1215 += tmp13
# asm 1: a <r1215=vec128#40,<r1215=vec128#40,<tmp13=vec128#41
# asm 2: a <r1215=$42,<r1215=$42,<tmp13=$43
a $42,$42,$43

# qhasm: tmp27 = combine r2629 and r3033 by comb22
# asm 1: shufb >tmp27=vec128#22,<r2629=vec128#24,<r3033=vec128#30,<comb22=vec128#67
# asm 2: shufb >tmp27=$24,<r2629=$26,<r3033=$32,<comb22=$69
shufb $24,$26,$32,$69

# qhasm: int32323232 r1619 += tmp14
# asm 1: a <r1619=vec128#4,<r1619=vec128#4,<tmp14=vec128#27
# asm 2: a <r1619=$6,<r1619=$6,<tmp14=$29
a $6,$6,$29

# qhasm: tmp28 = combine r3033 and r3437 by comb22
# asm 1: shufb >tmp28=vec128#24,<r3033=vec128#30,<r3437=vec128#7,<comb22=vec128#67
# asm 2: shufb >tmp28=$26,<r3033=$32,<r3437=$9,<comb22=$69
shufb $26,$32,$9,$69

# qhasm: int32323232 r2023 += tmp15
# asm 1: a <r2023=vec128#9,<r2023=vec128#9,<tmp15=vec128#12
# asm 2: a <r2023=$11,<r2023=$11,<tmp15=$14
a $11,$11,$14

# qhasm: tmp29 = combine r3437 and zero  by comb22
# asm 1: shufb >tmp29=vec128#7,<r3437=vec128#7,<zero=vec128#5,<comb22=vec128#67
# asm 2: shufb >tmp29=$9,<r3437=$9,<zero=$7,<comb22=$69
shufb $9,$9,$7,$69

# qhasm: int32323232 r2427 += tmp16
# asm 1: a <r2427=vec128#10,<r2427=vec128#10,<tmp16=vec128#16
# asm 2: a <r2427=$12,<r2427=$12,<tmp16=$18
a $12,$12,$18

# qhasm: tmp34 = combine r1518 and r1922 by comb31
# asm 1: shufb >tmp34=vec128#12,<r1518=vec128#59,<r1922=vec128#14,<comb31=vec128#68
# asm 2: shufb >tmp34=$14,<r1518=$61,<r1922=$16,<comb31=$70
shufb $14,$61,$16,$70

# qhasm: int32323232 r2831 += tmp17
# asm 1: a <r2831=vec128#11,<r2831=vec128#11,<tmp17=vec128#18
# asm 2: a <r2831=$13,<r2831=$13,<tmp17=$20
a $13,$13,$20

# qhasm: tmp35 = combine r1922 and r2326 by comb31
# asm 1: shufb >tmp35=vec128#14,<r1922=vec128#14,<r2326=vec128#15,<comb31=vec128#68
# asm 2: shufb >tmp35=$16,<r1922=$16,<r2326=$17,<comb31=$70
shufb $16,$16,$17,$70

# qhasm: int32323232 r3235 += tmp18
# asm 1: a <r3235=vec128#3,<r3235=vec128#3,<tmp18=vec128#21
# asm 2: a <r3235=$5,<r3235=$5,<tmp18=$23
a $5,$5,$23

# qhasm: tmp36 = combine r2326 and r2730 by comb31
# asm 1: shufb >tmp36=vec128#15,<r2326=vec128#15,<r2730=vec128#17,<comb31=vec128#68
# asm 2: shufb >tmp36=$17,<r2326=$17,<r2730=$19,<comb31=$70
shufb $17,$17,$19,$70

# qhasm: int32323232 r03   += tmp20
# asm 1: a <r03=vec128#35,<r03=vec128#35,<tmp20=vec128#37
# asm 2: a <r03=$37,<r03=$37,<tmp20=$39
a $37,$37,$39

# qhasm: tmp37 = combine r2730 and r3134 by comb31
# asm 1: shufb >tmp37=vec128#16,<r2730=vec128#17,<r3134=vec128#20,<comb31=vec128#68
# asm 2: shufb >tmp37=$18,<r2730=$19,<r3134=$22,<comb31=$70
shufb $18,$19,$22,$70

# qhasm: int32323232 r47   += tmp21
# asm 1: a <r47=vec128#36,<r47=vec128#36,<tmp21=vec128#39
# asm 2: a <r47=$38,<r47=$38,<tmp21=$41
a $38,$38,$41

# qhasm: tmp38 = combine r3134 and r3538 by comb31
# asm 1: shufb >tmp38=vec128#17,<r3134=vec128#20,<r3538=vec128#2,<comb31=vec128#68
# asm 2: shufb >tmp38=$19,<r3134=$22,<r3538=$4,<comb31=$70
shufb $19,$22,$4,$70

# qhasm: int32323232 r811  += tmp22
# asm 1: a <r811=vec128#38,<r811=vec128#38,<tmp22=vec128#25
# asm 2: a <r811=$40,<r811=$40,<tmp22=$27
a $40,$40,$27

# qhasm: tmp39 = combine r3538 and zero  by comb31
# asm 1: shufb >tmp39=vec128#2,<r3538=vec128#2,<zero=vec128#5,<comb31=vec128#68
# asm 2: shufb >tmp39=$4,<r3538=$4,<zero=$7,<comb31=$70
shufb $4,$4,$7,$70

# qhasm: int32323232 r1215 += tmp23
# asm 1: a <r1215=vec128#40,<r1215=vec128#40,<tmp23=vec128#44
# asm 2: a <r1215=$42,<r1215=$42,<tmp23=$46
a $42,$42,$46

# qhasm: int32323232 r1619 += tmp24
# asm 1: a <r1619=vec128#4,<r1619=vec128#4,<tmp24=vec128#8
# asm 2: a <r1619=$6,<r1619=$6,<tmp24=$10
a $6,$6,$10

# qhasm: int32323232 r2023 += tmp25
# asm 1: a <r2023=vec128#9,<r2023=vec128#9,<tmp25=vec128#13
# asm 2: a <r2023=$11,<r2023=$11,<tmp25=$15
a $11,$11,$15

# qhasm: int32323232 r2427 += tmp26
# asm 1: a <r2427=vec128#10,<r2427=vec128#10,<tmp26=vec128#19
# asm 2: a <r2427=$12,<r2427=$12,<tmp26=$21
a $12,$12,$21

# qhasm: int32323232 r2831 += tmp27
# asm 1: a <r2831=vec128#11,<r2831=vec128#11,<tmp27=vec128#22
# asm 2: a <r2831=$13,<r2831=$13,<tmp27=$24
a $13,$13,$24

# qhasm: int32323232 r3235 += tmp28
# asm 1: a <r3235=vec128#3,<r3235=vec128#3,<tmp28=vec128#24
# asm 2: a <r3235=$5,<r3235=$5,<tmp28=$26
a $5,$5,$26

# qhasm: int32323232 r3639 += tmp29
# asm 1: a <r3639=vec128#6,<r3639=vec128#6,<tmp29=vec128#7
# asm 2: a <r3639=$8,<r3639=$8,<tmp29=$9
a $8,$8,$9

# qhasm: int32323232 r03   += tmp30
# asm 1: a <r03=vec128#35,<r03=vec128#35,<tmp30=vec128#23
# asm 2: a <r03=$37,<r03=$37,<tmp30=$25
a $37,$37,$25

# qhasm: int32323232 r47   += tmp31
# asm 1: a <r47=vec128#36,<r47=vec128#36,<tmp31=vec128#42
# asm 2: a <r47=$38,<r47=$38,<tmp31=$44
a $38,$38,$44

# qhasm: int32323232 r811  += tmp32
# asm 1: a <r811=vec128#38,<r811=vec128#38,<tmp32=vec128#34
# asm 2: a <r811=$40,<r811=$40,<tmp32=$36
a $40,$40,$36

# qhasm: int32323232 r1215 += tmp33
# asm 1: a <r1215=vec128#40,<r1215=vec128#40,<tmp33=vec128#29
# asm 2: a <r1215=$42,<r1215=$42,<tmp33=$31
a $42,$42,$31

# qhasm: int32323232 r1619 += tmp34
# asm 1: a <r1619=vec128#4,<r1619=vec128#4,<tmp34=vec128#12
# asm 2: a <r1619=$6,<r1619=$6,<tmp34=$14
a $6,$6,$14

# qhasm: int32323232 r2023 += tmp35
# asm 1: a <r2023=vec128#9,<r2023=vec128#9,<tmp35=vec128#14
# asm 2: a <r2023=$11,<r2023=$11,<tmp35=$16
a $11,$11,$16

# qhasm: int32323232 r2427 += tmp36
# asm 1: a <r2427=vec128#10,<r2427=vec128#10,<tmp36=vec128#15
# asm 2: a <r2427=$12,<r2427=$12,<tmp36=$17
a $12,$12,$17

# qhasm: int32323232 r2831 += tmp37
# asm 1: a <r2831=vec128#11,<r2831=vec128#11,<tmp37=vec128#16
# asm 2: a <r2831=$13,<r2831=$13,<tmp37=$18
a $13,$13,$18

# qhasm: int32323232 r3235 += tmp38
# asm 1: a <r3235=vec128#3,<r3235=vec128#3,<tmp38=vec128#17
# asm 2: a <r3235=$5,<r3235=$5,<tmp38=$19
a $5,$5,$19

# qhasm: int32323232 r3639 += tmp39
# asm 1: a <r3639=vec128#6,<r3639=vec128#6,<tmp39=vec128#2
# asm 2: a <r3639=$8,<r3639=$8,<tmp39=$4
a $8,$8,$4

# qhasm: carry0 = select bytes from r2023 by sel01
# asm 1: shufb >carry0=vec128#2,<r2023=vec128#9,<r2023=vec128#9,<sel01=vec128#58
# asm 2: shufb >carry0=$4,<r2023=$11,<r2023=$11,<sel01=$60
shufb $4,$11,$11,$60

# qhasm: carry1 = select bytes from r2427 by sel01
# asm 1: shufb >carry1=vec128#5,<r2427=vec128#10,<r2427=vec128#10,<sel01=vec128#58
# asm 2: shufb >carry1=$7,<r2427=$12,<r2427=$12,<sel01=$60
shufb $7,$12,$12,$60

# qhasm: carry2 = select bytes from r2831 by sel01
# asm 1: shufb >carry2=vec128#7,<r2831=vec128#11,<r2831=vec128#11,<sel01=vec128#58
# asm 2: shufb >carry2=$9,<r2831=$13,<r2831=$13,<sel01=$60
shufb $9,$13,$13,$60

# qhasm: carry3 = select bytes from r3235 by sel01
# asm 1: shufb >carry3=vec128#8,<r3235=vec128#3,<r3235=vec128#3,<sel01=vec128#58
# asm 2: shufb >carry3=$10,<r3235=$5,<r3235=$5,<sel01=$60
shufb $10,$5,$5,$60

# qhasm: uint32323232 carry0 >>= 13
# asm 1: rotmi <carry0=vec128#2,<carry0=vec128#2,-13
# asm 2: rotmi <carry0=$4,<carry0=$4,-13
rotmi $4,$4,-13

# qhasm: uint32323232 carry1 >>= 13
# asm 1: rotmi <carry1=vec128#5,<carry1=vec128#5,-13
# asm 2: rotmi <carry1=$7,<carry1=$7,-13
rotmi $7,$7,-13

# qhasm: uint32323232 carry2 >>= 13
# asm 1: rotmi <carry2=vec128#7,<carry2=vec128#7,-13
# asm 2: rotmi <carry2=$9,<carry2=$9,-13
rotmi $9,$9,-13

# qhasm: uint32323232 carry3 >>= 13
# asm 1: rotmi <carry3=vec128#8,<carry3=vec128#8,-13
# asm 2: rotmi <carry3=$10,<carry3=$10,-13
rotmi $10,$10,-13

# qhasm: int32323232 r2023 += carry0
# asm 1: a <r2023=vec128#9,<r2023=vec128#9,<carry0=vec128#2
# asm 2: a <r2023=$11,<r2023=$11,<carry0=$4
a $11,$11,$4

# qhasm: int32323232 r2427 += carry1
# asm 1: a <r2427=vec128#10,<r2427=vec128#10,<carry1=vec128#5
# asm 2: a <r2427=$12,<r2427=$12,<carry1=$7
a $12,$12,$7

# qhasm: int32323232 r2831 += carry2
# asm 1: a <r2831=vec128#11,<r2831=vec128#11,<carry2=vec128#7
# asm 2: a <r2831=$13,<r2831=$13,<carry2=$9
a $13,$13,$9

# qhasm: int32323232 r3235 += carry3
# asm 1: a <r3235=vec128#3,<r3235=vec128#3,<carry3=vec128#8
# asm 2: a <r3235=$5,<r3235=$5,<carry3=$10
a $5,$5,$10

# qhasm: carry0 = select bytes from r2023 by sel12
# asm 1: shufb >carry0=vec128#2,<r2023=vec128#9,<r2023=vec128#9,<sel12=vec128#60
# asm 2: shufb >carry0=$4,<r2023=$11,<r2023=$11,<sel12=$62
shufb $4,$11,$11,$62

# qhasm: carry1 = select bytes from r2427 by sel12
# asm 1: shufb >carry1=vec128#5,<r2427=vec128#10,<r2427=vec128#10,<sel12=vec128#60
# asm 2: shufb >carry1=$7,<r2427=$12,<r2427=$12,<sel12=$62
shufb $7,$12,$12,$62

# qhasm: carry2 = select bytes from r2831 by sel12
# asm 1: shufb >carry2=vec128#7,<r2831=vec128#11,<r2831=vec128#11,<sel12=vec128#60
# asm 2: shufb >carry2=$9,<r2831=$13,<r2831=$13,<sel12=$62
shufb $9,$13,$13,$62

# qhasm: carry3 = select bytes from r3235 by sel12
# asm 1: shufb >carry3=vec128#8,<r3235=vec128#3,<r3235=vec128#3,<sel12=vec128#60
# asm 2: shufb >carry3=$10,<r3235=$5,<r3235=$5,<sel12=$62
shufb $10,$5,$5,$62

# qhasm: uint32323232 carry0 >>= 13
# asm 1: rotmi <carry0=vec128#2,<carry0=vec128#2,-13
# asm 2: rotmi <carry0=$4,<carry0=$4,-13
rotmi $4,$4,-13

# qhasm: uint32323232 carry1 >>= 13
# asm 1: rotmi <carry1=vec128#5,<carry1=vec128#5,-13
# asm 2: rotmi <carry1=$7,<carry1=$7,-13
rotmi $7,$7,-13

# qhasm: uint32323232 carry2 >>= 13
# asm 1: rotmi <carry2=vec128#7,<carry2=vec128#7,-13
# asm 2: rotmi <carry2=$9,<carry2=$9,-13
rotmi $9,$9,-13

# qhasm: uint32323232 carry3 >>= 13
# asm 1: rotmi <carry3=vec128#8,<carry3=vec128#8,-13
# asm 2: rotmi <carry3=$10,<carry3=$10,-13
rotmi $10,$10,-13

# qhasm: int32323232 r2023 += carry0
# asm 1: a <r2023=vec128#9,<r2023=vec128#9,<carry0=vec128#2
# asm 2: a <r2023=$11,<r2023=$11,<carry0=$4
a $11,$11,$4

# qhasm: int32323232 r2427 += carry1
# asm 1: a <r2427=vec128#10,<r2427=vec128#10,<carry1=vec128#5
# asm 2: a <r2427=$12,<r2427=$12,<carry1=$7
a $12,$12,$7

# qhasm: int32323232 r2831 += carry2
# asm 1: a <r2831=vec128#11,<r2831=vec128#11,<carry2=vec128#7
# asm 2: a <r2831=$13,<r2831=$13,<carry2=$9
a $13,$13,$9

# qhasm: int32323232 r3235 += carry3
# asm 1: a <r3235=vec128#3,<r3235=vec128#3,<carry3=vec128#8
# asm 2: a <r3235=$5,<r3235=$5,<carry3=$10
a $5,$5,$10

# qhasm: carry0 = select bytes from r2023 by sel23
# asm 1: shufb >carry0=vec128#2,<r2023=vec128#9,<r2023=vec128#9,<sel23=vec128#61
# asm 2: shufb >carry0=$4,<r2023=$11,<r2023=$11,<sel23=$63
shufb $4,$11,$11,$63

# qhasm: carry1 = select bytes from r2427 by sel23
# asm 1: shufb >carry1=vec128#5,<r2427=vec128#10,<r2427=vec128#10,<sel23=vec128#61
# asm 2: shufb >carry1=$7,<r2427=$12,<r2427=$12,<sel23=$63
shufb $7,$12,$12,$63

# qhasm: carry2 = select bytes from r2831 by sel23
# asm 1: shufb >carry2=vec128#7,<r2831=vec128#11,<r2831=vec128#11,<sel23=vec128#61
# asm 2: shufb >carry2=$9,<r2831=$13,<r2831=$13,<sel23=$63
shufb $9,$13,$13,$63

# qhasm: carry3 = select bytes from r3235 by sel23
# asm 1: shufb >carry3=vec128#8,<r3235=vec128#3,<r3235=vec128#3,<sel23=vec128#61
# asm 2: shufb >carry3=$10,<r3235=$5,<r3235=$5,<sel23=$63
shufb $10,$5,$5,$63

# qhasm: uint32323232 carry0 >>= 13
# asm 1: rotmi <carry0=vec128#2,<carry0=vec128#2,-13
# asm 2: rotmi <carry0=$4,<carry0=$4,-13
rotmi $4,$4,-13

# qhasm: uint32323232 carry1 >>= 13
# asm 1: rotmi <carry1=vec128#5,<carry1=vec128#5,-13
# asm 2: rotmi <carry1=$7,<carry1=$7,-13
rotmi $7,$7,-13

# qhasm: uint32323232 carry2 >>= 13
# asm 1: rotmi <carry2=vec128#7,<carry2=vec128#7,-13
# asm 2: rotmi <carry2=$9,<carry2=$9,-13
rotmi $9,$9,-13

# qhasm: uint32323232 carry3 >>= 13
# asm 1: rotmi <carry3=vec128#8,<carry3=vec128#8,-13
# asm 2: rotmi <carry3=$10,<carry3=$10,-13
rotmi $10,$10,-13

# qhasm: int32323232 r2023 += carry0
# asm 1: a <r2023=vec128#9,<r2023=vec128#9,<carry0=vec128#2
# asm 2: a <r2023=$11,<r2023=$11,<carry0=$4
a $11,$11,$4

# qhasm: int32323232 r2427 += carry1
# asm 1: a <r2427=vec128#10,<r2427=vec128#10,<carry1=vec128#5
# asm 2: a <r2427=$12,<r2427=$12,<carry1=$7
a $12,$12,$7

# qhasm: int32323232 r2831 += carry2
# asm 1: a <r2831=vec128#11,<r2831=vec128#11,<carry2=vec128#7
# asm 2: a <r2831=$13,<r2831=$13,<carry2=$9
a $13,$13,$9

# qhasm: int32323232 r3235 += carry3
# asm 1: a <r3235=vec128#3,<r3235=vec128#3,<carry3=vec128#8
# asm 2: a <r3235=$5,<r3235=$5,<carry3=$10
a $5,$5,$10

# qhasm: carry0 = select bytes from r2023 by sel30
# asm 1: shufb >carry0=vec128#2,<r2023=vec128#9,<r2023=vec128#9,<sel30=vec128#62
# asm 2: shufb >carry0=$4,<r2023=$11,<r2023=$11,<sel30=$64
shufb $4,$11,$11,$64

# qhasm: carry1 = select bytes from r2427 by sel30
# asm 1: shufb >carry1=vec128#5,<r2427=vec128#10,<r2427=vec128#10,<sel30=vec128#62
# asm 2: shufb >carry1=$7,<r2427=$12,<r2427=$12,<sel30=$64
shufb $7,$12,$12,$64

# qhasm: carry2 = select bytes from r2831 by sel30
# asm 1: shufb >carry2=vec128#7,<r2831=vec128#11,<r2831=vec128#11,<sel30=vec128#62
# asm 2: shufb >carry2=$9,<r2831=$13,<r2831=$13,<sel30=$64
shufb $9,$13,$13,$64

# qhasm: carry3 = select bytes from r3235 by sel30
# asm 1: shufb >carry3=vec128#8,<r3235=vec128#3,<r3235=vec128#3,<sel30=vec128#62
# asm 2: shufb >carry3=$10,<r3235=$5,<r3235=$5,<sel30=$64
shufb $10,$5,$5,$64

# qhasm: uint32323232 carry0 >>= 12
# asm 1: rotmi <carry0=vec128#2,<carry0=vec128#2,-12
# asm 2: rotmi <carry0=$4,<carry0=$4,-12
rotmi $4,$4,-12

# qhasm: uint32323232 carry1 >>= 12
# asm 1: rotmi <carry1=vec128#5,<carry1=vec128#5,-12
# asm 2: rotmi <carry1=$7,<carry1=$7,-12
rotmi $7,$7,-12

# qhasm: uint32323232 carry2 >>= 12
# asm 1: rotmi <carry2=vec128#7,<carry2=vec128#7,-12
# asm 2: rotmi <carry2=$9,<carry2=$9,-12
rotmi $9,$9,-12

# qhasm: uint32323232 carry3 >>= 12
# asm 1: rotmi <carry3=vec128#8,<carry3=vec128#8,-12
# asm 2: rotmi <carry3=$10,<carry3=$10,-12
rotmi $10,$10,-12

# qhasm: r2023 &= redcoeffmask
# asm 1: and <r2023=vec128#9,<r2023=vec128#9,<redcoeffmask=vec128#63
# asm 2: and <r2023=$11,<r2023=$11,<redcoeffmask=$65
and $11,$11,$65

# qhasm: r2427 &= redcoeffmask
# asm 1: and <r2427=vec128#10,<r2427=vec128#10,<redcoeffmask=vec128#63
# asm 2: and <r2427=$12,<r2427=$12,<redcoeffmask=$65
and $12,$12,$65

# qhasm: r2831 &= redcoeffmask
# asm 1: and <r2831=vec128#11,<r2831=vec128#11,<redcoeffmask=vec128#63
# asm 2: and <r2831=$13,<r2831=$13,<redcoeffmask=$65
and $13,$13,$65

# qhasm: r3235 &= redcoeffmask
# asm 1: and <r3235=vec128#3,<r3235=vec128#3,<redcoeffmask=vec128#63
# asm 2: and <r3235=$5,<r3235=$5,<redcoeffmask=$65
and $5,$5,$65

# qhasm: int32323232 r2427 += carry0
# asm 1: a <r2427=vec128#10,<r2427=vec128#10,<carry0=vec128#2
# asm 2: a <r2427=$12,<r2427=$12,<carry0=$4
a $12,$12,$4

# qhasm: int32323232 r2831 += carry1
# asm 1: a <r2831=vec128#11,<r2831=vec128#11,<carry1=vec128#5
# asm 2: a <r2831=$13,<r2831=$13,<carry1=$7
a $13,$13,$7

# qhasm: int32323232 r3235 += carry2
# asm 1: a <r3235=vec128#3,<r3235=vec128#3,<carry2=vec128#7
# asm 2: a <r3235=$5,<r3235=$5,<carry2=$9
a $5,$5,$9

# qhasm: int32323232 r3639 += carry3
# asm 1: a <r3639=vec128#6,<r3639=vec128#6,<carry3=vec128#8
# asm 2: a <r3639=$8,<r3639=$8,<carry3=$10
a $8,$8,$10

# qhasm: carry1 = select bytes from r2427 by sel01
# asm 1: shufb >carry1=vec128#2,<r2427=vec128#10,<r2427=vec128#10,<sel01=vec128#58
# asm 2: shufb >carry1=$4,<r2427=$12,<r2427=$12,<sel01=$60
shufb $4,$12,$12,$60

# qhasm: carry2 = select bytes from r2831 by sel01
# asm 1: shufb >carry2=vec128#5,<r2831=vec128#11,<r2831=vec128#11,<sel01=vec128#58
# asm 2: shufb >carry2=$7,<r2831=$13,<r2831=$13,<sel01=$60
shufb $7,$13,$13,$60

# qhasm: carry3 = select bytes from r3235 by sel01
# asm 1: shufb >carry3=vec128#7,<r3235=vec128#3,<r3235=vec128#3,<sel01=vec128#58
# asm 2: shufb >carry3=$9,<r3235=$5,<r3235=$5,<sel01=$60
shufb $9,$5,$5,$60

# qhasm: carry4 = select bytes from r3639 by sel01
# asm 1: shufb >carry4=vec128#8,<r3639=vec128#6,<r3639=vec128#6,<sel01=vec128#58
# asm 2: shufb >carry4=$10,<r3639=$8,<r3639=$8,<sel01=$60
shufb $10,$8,$8,$60

# qhasm: uint32323232 carry1 >>= 13
# asm 1: rotmi <carry1=vec128#2,<carry1=vec128#2,-13
# asm 2: rotmi <carry1=$4,<carry1=$4,-13
rotmi $4,$4,-13

# qhasm: uint32323232 carry2 >>= 13
# asm 1: rotmi <carry2=vec128#5,<carry2=vec128#5,-13
# asm 2: rotmi <carry2=$7,<carry2=$7,-13
rotmi $7,$7,-13

# qhasm: uint32323232 carry3 >>= 13
# asm 1: rotmi <carry3=vec128#7,<carry3=vec128#7,-13
# asm 2: rotmi <carry3=$9,<carry3=$9,-13
rotmi $9,$9,-13

# qhasm: uint32323232 carry4 >>= 13
# asm 1: rotmi <carry4=vec128#8,<carry4=vec128#8,-13
# asm 2: rotmi <carry4=$10,<carry4=$10,-13
rotmi $10,$10,-13

# qhasm: int32323232 r2427 += carry1
# asm 1: a <r2427=vec128#10,<r2427=vec128#10,<carry1=vec128#2
# asm 2: a <r2427=$12,<r2427=$12,<carry1=$4
a $12,$12,$4

# qhasm: int32323232 r2831 += carry2
# asm 1: a <r2831=vec128#11,<r2831=vec128#11,<carry2=vec128#5
# asm 2: a <r2831=$13,<r2831=$13,<carry2=$7
a $13,$13,$7

# qhasm: int32323232 r3235 += carry3
# asm 1: a <r3235=vec128#3,<r3235=vec128#3,<carry3=vec128#7
# asm 2: a <r3235=$5,<r3235=$5,<carry3=$9
a $5,$5,$9

# qhasm: int32323232 r3639 += carry4
# asm 1: a <r3639=vec128#6,<r3639=vec128#6,<carry4=vec128#8
# asm 2: a <r3639=$8,<r3639=$8,<carry4=$10
a $8,$8,$10

# qhasm: carry1 = select bytes from r2427 by sel12
# asm 1: shufb >carry1=vec128#2,<r2427=vec128#10,<r2427=vec128#10,<sel12=vec128#60
# asm 2: shufb >carry1=$4,<r2427=$12,<r2427=$12,<sel12=$62
shufb $4,$12,$12,$62

# qhasm: carry2 = select bytes from r2831 by sel12
# asm 1: shufb >carry2=vec128#5,<r2831=vec128#11,<r2831=vec128#11,<sel12=vec128#60
# asm 2: shufb >carry2=$7,<r2831=$13,<r2831=$13,<sel12=$62
shufb $7,$13,$13,$62

# qhasm: carry3 = select bytes from r3235 by sel12
# asm 1: shufb >carry3=vec128#7,<r3235=vec128#3,<r3235=vec128#3,<sel12=vec128#60
# asm 2: shufb >carry3=$9,<r3235=$5,<r3235=$5,<sel12=$62
shufb $9,$5,$5,$62

# qhasm: carry4 = select bytes from r3639 by sel12
# asm 1: shufb >carry4=vec128#8,<r3639=vec128#6,<r3639=vec128#6,<sel12=vec128#60
# asm 2: shufb >carry4=$10,<r3639=$8,<r3639=$8,<sel12=$62
shufb $10,$8,$8,$62

# qhasm: uint32323232 carry1 >>= 13
# asm 1: rotmi <carry1=vec128#2,<carry1=vec128#2,-13
# asm 2: rotmi <carry1=$4,<carry1=$4,-13
rotmi $4,$4,-13

# qhasm: uint32323232 carry2 >>= 13
# asm 1: rotmi <carry2=vec128#5,<carry2=vec128#5,-13
# asm 2: rotmi <carry2=$7,<carry2=$7,-13
rotmi $7,$7,-13

# qhasm: uint32323232 carry3 >>= 13
# asm 1: rotmi <carry3=vec128#7,<carry3=vec128#7,-13
# asm 2: rotmi <carry3=$9,<carry3=$9,-13
rotmi $9,$9,-13

# qhasm: uint32323232 carry4 >>= 13
# asm 1: rotmi <carry4=vec128#8,<carry4=vec128#8,-13
# asm 2: rotmi <carry4=$10,<carry4=$10,-13
rotmi $10,$10,-13

# qhasm: int32323232 r2427 += carry1
# asm 1: a <r2427=vec128#10,<r2427=vec128#10,<carry1=vec128#2
# asm 2: a <r2427=$12,<r2427=$12,<carry1=$4
a $12,$12,$4

# qhasm: int32323232 r2831 += carry2
# asm 1: a <r2831=vec128#11,<r2831=vec128#11,<carry2=vec128#5
# asm 2: a <r2831=$13,<r2831=$13,<carry2=$7
a $13,$13,$7

# qhasm: int32323232 r3235 += carry3
# asm 1: a <r3235=vec128#3,<r3235=vec128#3,<carry3=vec128#7
# asm 2: a <r3235=$5,<r3235=$5,<carry3=$9
a $5,$5,$9

# qhasm: int32323232 r3639 += carry4
# asm 1: a <r3639=vec128#6,<r3639=vec128#6,<carry4=vec128#8
# asm 2: a <r3639=$8,<r3639=$8,<carry4=$10
a $8,$8,$10

# qhasm: carry1 = select bytes from r2427 by sel23
# asm 1: shufb >carry1=vec128#2,<r2427=vec128#10,<r2427=vec128#10,<sel23=vec128#61
# asm 2: shufb >carry1=$4,<r2427=$12,<r2427=$12,<sel23=$63
shufb $4,$12,$12,$63

# qhasm: carry2 = select bytes from r2831 by sel23
# asm 1: shufb >carry2=vec128#5,<r2831=vec128#11,<r2831=vec128#11,<sel23=vec128#61
# asm 2: shufb >carry2=$7,<r2831=$13,<r2831=$13,<sel23=$63
shufb $7,$13,$13,$63

# qhasm: carry3 = select bytes from r3235 by sel23
# asm 1: shufb >carry3=vec128#7,<r3235=vec128#3,<r3235=vec128#3,<sel23=vec128#61
# asm 2: shufb >carry3=$9,<r3235=$5,<r3235=$5,<sel23=$63
shufb $9,$5,$5,$63

# qhasm: carry4 = select bytes from r3639 by sel23
# asm 1: shufb >carry4=vec128#8,<r3639=vec128#6,<r3639=vec128#6,<sel23=vec128#61
# asm 2: shufb >carry4=$10,<r3639=$8,<r3639=$8,<sel23=$63
shufb $10,$8,$8,$63

# qhasm: uint32323232 carry1 >>= 13
# asm 1: rotmi <carry1=vec128#2,<carry1=vec128#2,-13
# asm 2: rotmi <carry1=$4,<carry1=$4,-13
rotmi $4,$4,-13

# qhasm: uint32323232 carry2 >>= 13
# asm 1: rotmi <carry2=vec128#5,<carry2=vec128#5,-13
# asm 2: rotmi <carry2=$7,<carry2=$7,-13
rotmi $7,$7,-13

# qhasm: uint32323232 carry3 >>= 13
# asm 1: rotmi <carry3=vec128#7,<carry3=vec128#7,-13
# asm 2: rotmi <carry3=$9,<carry3=$9,-13
rotmi $9,$9,-13

# qhasm: uint32323232 carry4 >>= 13
# asm 1: rotmi <carry4=vec128#8,<carry4=vec128#8,-13
# asm 2: rotmi <carry4=$10,<carry4=$10,-13
rotmi $10,$10,-13

# qhasm: int32323232 r2427 += carry1
# asm 1: a <r2427=vec128#10,<r2427=vec128#10,<carry1=vec128#2
# asm 2: a <r2427=$12,<r2427=$12,<carry1=$4
a $12,$12,$4

# qhasm: int32323232 r2831 += carry2
# asm 1: a <r2831=vec128#11,<r2831=vec128#11,<carry2=vec128#5
# asm 2: a <r2831=$13,<r2831=$13,<carry2=$7
a $13,$13,$7

# qhasm: int32323232 r3235 += carry3
# asm 1: a <r3235=vec128#3,<r3235=vec128#3,<carry3=vec128#7
# asm 2: a <r3235=$5,<r3235=$5,<carry3=$9
a $5,$5,$9

# qhasm: int32323232 r3639 += carry4
# asm 1: a <r3639=vec128#6,<r3639=vec128#6,<carry4=vec128#8
# asm 2: a <r3639=$8,<r3639=$8,<carry4=$10
a $8,$8,$10

# qhasm: carry1 = select bytes from r2427 by sel30
# asm 1: shufb >carry1=vec128#2,<r2427=vec128#10,<r2427=vec128#10,<sel30=vec128#62
# asm 2: shufb >carry1=$4,<r2427=$12,<r2427=$12,<sel30=$64
shufb $4,$12,$12,$64

# qhasm: carry2 = select bytes from r2831 by sel30
# asm 1: shufb >carry2=vec128#5,<r2831=vec128#11,<r2831=vec128#11,<sel30=vec128#62
# asm 2: shufb >carry2=$7,<r2831=$13,<r2831=$13,<sel30=$64
shufb $7,$13,$13,$64

# qhasm: carry3 = select bytes from r3235 by sel30
# asm 1: shufb >carry3=vec128#7,<r3235=vec128#3,<r3235=vec128#3,<sel30=vec128#62
# asm 2: shufb >carry3=$9,<r3235=$5,<r3235=$5,<sel30=$64
shufb $9,$5,$5,$64

# qhasm: uint32323232 carry1 >>= 12
# asm 1: rotmi <carry1=vec128#2,<carry1=vec128#2,-12
# asm 2: rotmi <carry1=$4,<carry1=$4,-12
rotmi $4,$4,-12

# qhasm: uint32323232 carry2 >>= 12
# asm 1: rotmi <carry2=vec128#5,<carry2=vec128#5,-12
# asm 2: rotmi <carry2=$7,<carry2=$7,-12
rotmi $7,$7,-12

# qhasm: uint32323232 carry3 >>= 12
# asm 1: rotmi <carry3=vec128#7,<carry3=vec128#7,-12
# asm 2: rotmi <carry3=$9,<carry3=$9,-12
rotmi $9,$9,-12

# qhasm: r2427 &= redcoeffmask
# asm 1: and <r2427=vec128#10,<r2427=vec128#10,<redcoeffmask=vec128#63
# asm 2: and <r2427=$12,<r2427=$12,<redcoeffmask=$65
and $12,$12,$65

# qhasm: r2831 &= redcoeffmask
# asm 1: and <r2831=vec128#11,<r2831=vec128#11,<redcoeffmask=vec128#63
# asm 2: and <r2831=$13,<r2831=$13,<redcoeffmask=$65
and $13,$13,$65

# qhasm: r3235 &= redcoeffmask
# asm 1: and <r3235=vec128#3,<r3235=vec128#3,<redcoeffmask=vec128#63
# asm 2: and <r3235=$5,<r3235=$5,<redcoeffmask=$65
and $5,$5,$65

# qhasm: r3639 &= redcoeffmaskend
# asm 1: and <r3639=vec128#6,<r3639=vec128#6,<redcoeffmaskend=vec128#64
# asm 2: and <r3639=$8,<r3639=$8,<redcoeffmaskend=$66
and $8,$8,$66

# qhasm: int32323232 r2831 += carry1
# asm 1: a <r2831=vec128#11,<r2831=vec128#11,<carry1=vec128#2
# asm 2: a <r2831=$13,<r2831=$13,<carry1=$4
a $13,$13,$4

# qhasm: int32323232 r3235 += carry2
# asm 1: a <r3235=vec128#3,<r3235=vec128#3,<carry2=vec128#5
# asm 2: a <r3235=$5,<r3235=$5,<carry2=$7
a $5,$5,$7

# qhasm: int32323232 r3639 += carry3
# asm 1: a <r3639=vec128#6,<r3639=vec128#6,<carry3=vec128#7
# asm 2: a <r3639=$8,<r3639=$8,<carry3=$9
a $8,$8,$9

# qhasm: int32323232 r03   += (r2023 & 0xffff) * (vec19 & 0xffff)
# asm 1: mpya >r03=vec128#2,<r2023=vec128#9,<vec19=vec128#69,<r03=vec128#35
# asm 2: mpya >r03=$4,<r2023=$11,<vec19=$71,<r03=$37
mpya $4,$11,$71,$37

# qhasm: int32323232 r1619 += (r3639 & 0xffff) * (vec19 & 0xffff)
# asm 1: mpya >r1619=vec128#4,<r3639=vec128#6,<vec19=vec128#69,<r1619=vec128#4
# asm 2: mpya >r1619=$6,<r3639=$8,<vec19=$71,<r1619=$6
mpya $6,$8,$71,$6

# qhasm: int32323232 r47   += (r2427 & 0xffff) * (vec19 & 0xffff)
# asm 1: mpya >r47=vec128#5,<r2427=vec128#10,<vec19=vec128#69,<r47=vec128#36
# asm 2: mpya >r47=$7,<r2427=$12,<vec19=$71,<r47=$38
mpya $7,$12,$71,$38

# qhasm: int32323232 r811  += (r2831 & 0xffff) * (vec19 & 0xffff)
# asm 1: mpya >r811=vec128#6,<r2831=vec128#11,<vec19=vec128#69,<r811=vec128#38
# asm 2: mpya >r811=$8,<r2831=$13,<vec19=$71,<r811=$40
mpya $8,$13,$71,$40

# qhasm: int32323232 r1215 += (r3235 & 0xffff) * (vec19 & 0xffff)
# asm 1: mpya >r1215=vec128#3,<r3235=vec128#3,<vec19=vec128#69,<r1215=vec128#40
# asm 2: mpya >r1215=$5,<r3235=$5,<vec19=$71,<r1215=$42
mpya $5,$5,$71,$42

# qhasm: lnop
 lnop

# qhasm: carry = select bytes from r1619 by sel01
# asm 1: shufb >carry=vec128#7,<r1619=vec128#4,<r1619=vec128#4,<sel01=vec128#58
# asm 2: shufb >carry=$9,<r1619=$6,<r1619=$6,<sel01=$60
shufb $9,$6,$6,$60

# qhasm: uint32323232 carry >>= 13
# asm 1: rotmi <carry=vec128#7,<carry=vec128#7,-13
# asm 2: rotmi <carry=$9,<carry=$9,-13
rotmi $9,$9,-13

# qhasm: int32323232 r1619 += carry
# asm 1: a <r1619=vec128#4,<r1619=vec128#4,<carry=vec128#7
# asm 2: a <r1619=$6,<r1619=$6,<carry=$9
a $6,$6,$9

# qhasm: carry = select bytes from r1619 by sel12
# asm 1: shufb >carry=vec128#7,<r1619=vec128#4,<r1619=vec128#4,<sel12=vec128#60
# asm 2: shufb >carry=$9,<r1619=$6,<r1619=$6,<sel12=$62
shufb $9,$6,$6,$62

# qhasm: uint32323232 carry >>= 13
# asm 1: rotmi <carry=vec128#7,<carry=vec128#7,-13
# asm 2: rotmi <carry=$9,<carry=$9,-13
rotmi $9,$9,-13

# qhasm: int32323232 r1619 += carry
# asm 1: a <r1619=vec128#4,<r1619=vec128#4,<carry=vec128#7
# asm 2: a <r1619=$6,<r1619=$6,<carry=$9
a $6,$6,$9

# qhasm: carry = select bytes from r1619 by sel23
# asm 1: shufb >carry=vec128#7,<r1619=vec128#4,<r1619=vec128#4,<sel23=vec128#61
# asm 2: shufb >carry=$9,<r1619=$6,<r1619=$6,<sel23=$63
shufb $9,$6,$6,$63

# qhasm: uint32323232 carry >>= 13
# asm 1: rotmi <carry=vec128#7,<carry=vec128#7,-13
# asm 2: rotmi <carry=$9,<carry=$9,-13
rotmi $9,$9,-13

# qhasm: int32323232 r1619 += carry
# asm 1: a <r1619=vec128#4,<r1619=vec128#4,<carry=vec128#7
# asm 2: a <r1619=$6,<r1619=$6,<carry=$9
a $6,$6,$9

# qhasm: carry = select bytes from r1619 by sel30
# asm 1: shufb >carry=vec128#7,<r1619=vec128#4,<r1619=vec128#4,<sel30=vec128#62
# asm 2: shufb >carry=$9,<r1619=$6,<r1619=$6,<sel30=$64
shufb $9,$6,$6,$64

# qhasm: uint32323232 carry >>= 12
# asm 1: rotmi <carry=vec128#7,<carry=vec128#7,-12
# asm 2: rotmi <carry=$9,<carry=$9,-12
rotmi $9,$9,-12

# qhasm: int32323232 red = carry << 4
# asm 1: shli >red=vec128#8,<carry=vec128#7,4
# asm 2: shli >red=$10,<carry=$9,4
shli $10,$9,4

# qhasm: int32323232 red = red + carry 
# asm 1: a >red=vec128#8,<red=vec128#8,<carry=vec128#7
# asm 2: a >red=$10,<red=$10,<carry=$9
a $10,$10,$9

# qhasm: int32323232 red = red + carry 
# asm 1: a >red=vec128#8,<red=vec128#8,<carry=vec128#7
# asm 2: a >red=$10,<red=$10,<carry=$9
a $10,$10,$9

# qhasm: int32323232 red = red + carry 
# asm 1: a >red=vec128#7,<red=vec128#8,<carry=vec128#7
# asm 2: a >red=$9,<red=$10,<carry=$9
a $9,$10,$9

# qhasm: int32323232 r03 += red
# asm 1: a <r03=vec128#2,<r03=vec128#2,<red=vec128#7
# asm 2: a <r03=$4,<r03=$4,<red=$9
a $4,$4,$9

# qhasm: r1619 &= redcoeffmask
# asm 1: and <r1619=vec128#4,<r1619=vec128#4,<redcoeffmask=vec128#63
# asm 2: and <r1619=$6,<r1619=$6,<redcoeffmask=$65
and $6,$6,$65

# qhasm: carry0 = select bytes from r03 by sel01
# asm 1: shufb >carry0=vec128#7,<r03=vec128#2,<r03=vec128#2,<sel01=vec128#58
# asm 2: shufb >carry0=$9,<r03=$4,<r03=$4,<sel01=$60
shufb $9,$4,$4,$60

# qhasm: carry1 = select bytes from r47 by sel01
# asm 1: shufb >carry1=vec128#8,<r47=vec128#5,<r47=vec128#5,<sel01=vec128#58
# asm 2: shufb >carry1=$10,<r47=$7,<r47=$7,<sel01=$60
shufb $10,$7,$7,$60

# qhasm: carry2 = select bytes from r811 by sel01
# asm 1: shufb >carry2=vec128#9,<r811=vec128#6,<r811=vec128#6,<sel01=vec128#58
# asm 2: shufb >carry2=$11,<r811=$8,<r811=$8,<sel01=$60
shufb $11,$8,$8,$60

# qhasm: carry3 = select bytes from r1215 by sel01
# asm 1: shufb >carry3=vec128#10,<r1215=vec128#3,<r1215=vec128#3,<sel01=vec128#58
# asm 2: shufb >carry3=$12,<r1215=$5,<r1215=$5,<sel01=$60
shufb $12,$5,$5,$60

# qhasm: uint32323232 carry0 >>= 13
# asm 1: rotmi <carry0=vec128#7,<carry0=vec128#7,-13
# asm 2: rotmi <carry0=$9,<carry0=$9,-13
rotmi $9,$9,-13

# qhasm: uint32323232 carry1 >>= 13
# asm 1: rotmi <carry1=vec128#8,<carry1=vec128#8,-13
# asm 2: rotmi <carry1=$10,<carry1=$10,-13
rotmi $10,$10,-13

# qhasm: uint32323232 carry2 >>= 13
# asm 1: rotmi <carry2=vec128#9,<carry2=vec128#9,-13
# asm 2: rotmi <carry2=$11,<carry2=$11,-13
rotmi $11,$11,-13

# qhasm: uint32323232 carry3 >>= 13
# asm 1: rotmi <carry3=vec128#10,<carry3=vec128#10,-13
# asm 2: rotmi <carry3=$12,<carry3=$12,-13
rotmi $12,$12,-13

# qhasm: int32323232 r03 += carry0
# asm 1: a <r03=vec128#2,<r03=vec128#2,<carry0=vec128#7
# asm 2: a <r03=$4,<r03=$4,<carry0=$9
a $4,$4,$9

# qhasm: int32323232 r47 += carry1
# asm 1: a <r47=vec128#5,<r47=vec128#5,<carry1=vec128#8
# asm 2: a <r47=$7,<r47=$7,<carry1=$10
a $7,$7,$10

# qhasm: int32323232 r811 += carry2
# asm 1: a <r811=vec128#6,<r811=vec128#6,<carry2=vec128#9
# asm 2: a <r811=$8,<r811=$8,<carry2=$11
a $8,$8,$11

# qhasm: int32323232 r1215 += carry3
# asm 1: a <r1215=vec128#3,<r1215=vec128#3,<carry3=vec128#10
# asm 2: a <r1215=$5,<r1215=$5,<carry3=$12
a $5,$5,$12

# qhasm: carry0 = select bytes from r03 by sel12
# asm 1: shufb >carry0=vec128#7,<r03=vec128#2,<r03=vec128#2,<sel12=vec128#60
# asm 2: shufb >carry0=$9,<r03=$4,<r03=$4,<sel12=$62
shufb $9,$4,$4,$62

# qhasm: carry1 = select bytes from r47 by sel12
# asm 1: shufb >carry1=vec128#8,<r47=vec128#5,<r47=vec128#5,<sel12=vec128#60
# asm 2: shufb >carry1=$10,<r47=$7,<r47=$7,<sel12=$62
shufb $10,$7,$7,$62

# qhasm: carry2 = select bytes from r811 by sel12
# asm 1: shufb >carry2=vec128#9,<r811=vec128#6,<r811=vec128#6,<sel12=vec128#60
# asm 2: shufb >carry2=$11,<r811=$8,<r811=$8,<sel12=$62
shufb $11,$8,$8,$62

# qhasm: carry3 = select bytes from r1215 by sel12
# asm 1: shufb >carry3=vec128#10,<r1215=vec128#3,<r1215=vec128#3,<sel12=vec128#60
# asm 2: shufb >carry3=$12,<r1215=$5,<r1215=$5,<sel12=$62
shufb $12,$5,$5,$62

# qhasm: uint32323232 carry0 >>= 13
# asm 1: rotmi <carry0=vec128#7,<carry0=vec128#7,-13
# asm 2: rotmi <carry0=$9,<carry0=$9,-13
rotmi $9,$9,-13

# qhasm: uint32323232 carry1 >>= 13
# asm 1: rotmi <carry1=vec128#8,<carry1=vec128#8,-13
# asm 2: rotmi <carry1=$10,<carry1=$10,-13
rotmi $10,$10,-13

# qhasm: uint32323232 carry2 >>= 13
# asm 1: rotmi <carry2=vec128#9,<carry2=vec128#9,-13
# asm 2: rotmi <carry2=$11,<carry2=$11,-13
rotmi $11,$11,-13

# qhasm: uint32323232 carry3 >>= 13
# asm 1: rotmi <carry3=vec128#10,<carry3=vec128#10,-13
# asm 2: rotmi <carry3=$12,<carry3=$12,-13
rotmi $12,$12,-13

# qhasm: int32323232 r03 += carry0
# asm 1: a <r03=vec128#2,<r03=vec128#2,<carry0=vec128#7
# asm 2: a <r03=$4,<r03=$4,<carry0=$9
a $4,$4,$9

# qhasm: int32323232 r47 += carry1
# asm 1: a <r47=vec128#5,<r47=vec128#5,<carry1=vec128#8
# asm 2: a <r47=$7,<r47=$7,<carry1=$10
a $7,$7,$10

# qhasm: int32323232 r811 += carry2
# asm 1: a <r811=vec128#6,<r811=vec128#6,<carry2=vec128#9
# asm 2: a <r811=$8,<r811=$8,<carry2=$11
a $8,$8,$11

# qhasm: int32323232 r1215 += carry3
# asm 1: a <r1215=vec128#3,<r1215=vec128#3,<carry3=vec128#10
# asm 2: a <r1215=$5,<r1215=$5,<carry3=$12
a $5,$5,$12

# qhasm: carry0 = select bytes from r03 by sel23
# asm 1: shufb >carry0=vec128#7,<r03=vec128#2,<r03=vec128#2,<sel23=vec128#61
# asm 2: shufb >carry0=$9,<r03=$4,<r03=$4,<sel23=$63
shufb $9,$4,$4,$63

# qhasm: carry1 = select bytes from r47 by sel23
# asm 1: shufb >carry1=vec128#8,<r47=vec128#5,<r47=vec128#5,<sel23=vec128#61
# asm 2: shufb >carry1=$10,<r47=$7,<r47=$7,<sel23=$63
shufb $10,$7,$7,$63

# qhasm: carry2 = select bytes from r811 by sel23
# asm 1: shufb >carry2=vec128#9,<r811=vec128#6,<r811=vec128#6,<sel23=vec128#61
# asm 2: shufb >carry2=$11,<r811=$8,<r811=$8,<sel23=$63
shufb $11,$8,$8,$63

# qhasm: carry3 = select bytes from r1215 by sel23
# asm 1: shufb >carry3=vec128#10,<r1215=vec128#3,<r1215=vec128#3,<sel23=vec128#61
# asm 2: shufb >carry3=$12,<r1215=$5,<r1215=$5,<sel23=$63
shufb $12,$5,$5,$63

# qhasm: uint32323232 carry0 >>= 13
# asm 1: rotmi <carry0=vec128#7,<carry0=vec128#7,-13
# asm 2: rotmi <carry0=$9,<carry0=$9,-13
rotmi $9,$9,-13

# qhasm: uint32323232 carry1 >>= 13
# asm 1: rotmi <carry1=vec128#8,<carry1=vec128#8,-13
# asm 2: rotmi <carry1=$10,<carry1=$10,-13
rotmi $10,$10,-13

# qhasm: uint32323232 carry2 >>= 13
# asm 1: rotmi <carry2=vec128#9,<carry2=vec128#9,-13
# asm 2: rotmi <carry2=$11,<carry2=$11,-13
rotmi $11,$11,-13

# qhasm: uint32323232 carry3 >>= 13
# asm 1: rotmi <carry3=vec128#10,<carry3=vec128#10,-13
# asm 2: rotmi <carry3=$12,<carry3=$12,-13
rotmi $12,$12,-13

# qhasm: int32323232 r03 += carry0
# asm 1: a <r03=vec128#2,<r03=vec128#2,<carry0=vec128#7
# asm 2: a <r03=$4,<r03=$4,<carry0=$9
a $4,$4,$9

# qhasm: int32323232 r47 += carry1
# asm 1: a <r47=vec128#5,<r47=vec128#5,<carry1=vec128#8
# asm 2: a <r47=$7,<r47=$7,<carry1=$10
a $7,$7,$10

# qhasm: int32323232 r811 += carry2
# asm 1: a <r811=vec128#6,<r811=vec128#6,<carry2=vec128#9
# asm 2: a <r811=$8,<r811=$8,<carry2=$11
a $8,$8,$11

# qhasm: int32323232 r1215 += carry3
# asm 1: a <r1215=vec128#3,<r1215=vec128#3,<carry3=vec128#10
# asm 2: a <r1215=$5,<r1215=$5,<carry3=$12
a $5,$5,$12

# qhasm: carry0 = select bytes from r03 by sel30
# asm 1: shufb >carry0=vec128#7,<r03=vec128#2,<r03=vec128#2,<sel30=vec128#62
# asm 2: shufb >carry0=$9,<r03=$4,<r03=$4,<sel30=$64
shufb $9,$4,$4,$64

# qhasm: carry1 = select bytes from r47 by sel30
# asm 1: shufb >carry1=vec128#8,<r47=vec128#5,<r47=vec128#5,<sel30=vec128#62
# asm 2: shufb >carry1=$10,<r47=$7,<r47=$7,<sel30=$64
shufb $10,$7,$7,$64

# qhasm: carry2 = select bytes from r811 by sel30
# asm 1: shufb >carry2=vec128#9,<r811=vec128#6,<r811=vec128#6,<sel30=vec128#62
# asm 2: shufb >carry2=$11,<r811=$8,<r811=$8,<sel30=$64
shufb $11,$8,$8,$64

# qhasm: carry3 = select bytes from r1215 by sel30
# asm 1: shufb >carry3=vec128#10,<r1215=vec128#3,<r1215=vec128#3,<sel30=vec128#62
# asm 2: shufb >carry3=$12,<r1215=$5,<r1215=$5,<sel30=$64
shufb $12,$5,$5,$64

# qhasm: uint32323232 carry0 >>= 12
# asm 1: rotmi <carry0=vec128#7,<carry0=vec128#7,-12
# asm 2: rotmi <carry0=$9,<carry0=$9,-12
rotmi $9,$9,-12

# qhasm: uint32323232 carry1 >>= 12
# asm 1: rotmi <carry1=vec128#8,<carry1=vec128#8,-12
# asm 2: rotmi <carry1=$10,<carry1=$10,-12
rotmi $10,$10,-12

# qhasm: uint32323232 carry2 >>= 12
# asm 1: rotmi <carry2=vec128#9,<carry2=vec128#9,-12
# asm 2: rotmi <carry2=$11,<carry2=$11,-12
rotmi $11,$11,-12

# qhasm: uint32323232 carry3 >>= 12
# asm 1: rotmi <carry3=vec128#10,<carry3=vec128#10,-12
# asm 2: rotmi <carry3=$12,<carry3=$12,-12
rotmi $12,$12,-12

# qhasm: r03 &= redcoeffmask
# asm 1: and <r03=vec128#2,<r03=vec128#2,<redcoeffmask=vec128#63
# asm 2: and <r03=$4,<r03=$4,<redcoeffmask=$65
and $4,$4,$65

# qhasm: r47 &= redcoeffmask
# asm 1: and <r47=vec128#5,<r47=vec128#5,<redcoeffmask=vec128#63
# asm 2: and <r47=$7,<r47=$7,<redcoeffmask=$65
and $7,$7,$65

# qhasm: r811 &= redcoeffmask
# asm 1: and <r811=vec128#6,<r811=vec128#6,<redcoeffmask=vec128#63
# asm 2: and <r811=$8,<r811=$8,<redcoeffmask=$65
and $8,$8,$65

# qhasm: r1215 &= redcoeffmask
# asm 1: and <r1215=vec128#3,<r1215=vec128#3,<redcoeffmask=vec128#63
# asm 2: and <r1215=$5,<r1215=$5,<redcoeffmask=$65
and $5,$5,$65

# qhasm: int32323232 r47 += carry0
# asm 1: a <r47=vec128#5,<r47=vec128#5,<carry0=vec128#7
# asm 2: a <r47=$7,<r47=$7,<carry0=$9
a $7,$7,$9

# qhasm: int32323232 r811 += carry1
# asm 1: a <r811=vec128#6,<r811=vec128#6,<carry1=vec128#8
# asm 2: a <r811=$8,<r811=$8,<carry1=$10
a $8,$8,$10

# qhasm: int32323232 r1215 += carry2
# asm 1: a <r1215=vec128#3,<r1215=vec128#3,<carry2=vec128#9
# asm 2: a <r1215=$5,<r1215=$5,<carry2=$11
a $5,$5,$11

# qhasm: int32323232 r1619 += carry3
# asm 1: a <r1619=vec128#4,<r1619=vec128#4,<carry3=vec128#10
# asm 2: a <r1619=$6,<r1619=$6,<carry3=$12
a $6,$6,$12

# qhasm: carry1 = select bytes from r47 by sel01
# asm 1: shufb >carry1=vec128#7,<r47=vec128#5,<r47=vec128#5,<sel01=vec128#58
# asm 2: shufb >carry1=$9,<r47=$7,<r47=$7,<sel01=$60
shufb $9,$7,$7,$60

# qhasm: carry2 = select bytes from r811 by sel01
# asm 1: shufb >carry2=vec128#8,<r811=vec128#6,<r811=vec128#6,<sel01=vec128#58
# asm 2: shufb >carry2=$10,<r811=$8,<r811=$8,<sel01=$60
shufb $10,$8,$8,$60

# qhasm: carry3 = select bytes from r1215 by sel01
# asm 1: shufb >carry3=vec128#9,<r1215=vec128#3,<r1215=vec128#3,<sel01=vec128#58
# asm 2: shufb >carry3=$11,<r1215=$5,<r1215=$5,<sel01=$60
shufb $11,$5,$5,$60

# qhasm: carry4 = select bytes from r1619 by sel01
# asm 1: shufb >carry4=vec128#10,<r1619=vec128#4,<r1619=vec128#4,<sel01=vec128#58
# asm 2: shufb >carry4=$12,<r1619=$6,<r1619=$6,<sel01=$60
shufb $12,$6,$6,$60

# qhasm: uint32323232 carry1 >>= 13
# asm 1: rotmi <carry1=vec128#7,<carry1=vec128#7,-13
# asm 2: rotmi <carry1=$9,<carry1=$9,-13
rotmi $9,$9,-13

# qhasm: uint32323232 carry2 >>= 13
# asm 1: rotmi <carry2=vec128#8,<carry2=vec128#8,-13
# asm 2: rotmi <carry2=$10,<carry2=$10,-13
rotmi $10,$10,-13

# qhasm: uint32323232 carry3 >>= 13
# asm 1: rotmi <carry3=vec128#9,<carry3=vec128#9,-13
# asm 2: rotmi <carry3=$11,<carry3=$11,-13
rotmi $11,$11,-13

# qhasm: uint32323232 carry4 >>= 13
# asm 1: rotmi <carry4=vec128#10,<carry4=vec128#10,-13
# asm 2: rotmi <carry4=$12,<carry4=$12,-13
rotmi $12,$12,-13

# qhasm: int32323232 r47 += carry1
# asm 1: a <r47=vec128#5,<r47=vec128#5,<carry1=vec128#7
# asm 2: a <r47=$7,<r47=$7,<carry1=$9
a $7,$7,$9

# qhasm: int32323232 r811 += carry2
# asm 1: a <r811=vec128#6,<r811=vec128#6,<carry2=vec128#8
# asm 2: a <r811=$8,<r811=$8,<carry2=$10
a $8,$8,$10

# qhasm: int32323232 r1215 += carry3
# asm 1: a <r1215=vec128#3,<r1215=vec128#3,<carry3=vec128#9
# asm 2: a <r1215=$5,<r1215=$5,<carry3=$11
a $5,$5,$11

# qhasm: int32323232 r1619 += carry4
# asm 1: a <r1619=vec128#4,<r1619=vec128#4,<carry4=vec128#10
# asm 2: a <r1619=$6,<r1619=$6,<carry4=$12
a $6,$6,$12

# qhasm: carry1 = select bytes from r47 by sel12
# asm 1: shufb >carry1=vec128#7,<r47=vec128#5,<r47=vec128#5,<sel12=vec128#60
# asm 2: shufb >carry1=$9,<r47=$7,<r47=$7,<sel12=$62
shufb $9,$7,$7,$62

# qhasm: carry2 = select bytes from r811 by sel12
# asm 1: shufb >carry2=vec128#8,<r811=vec128#6,<r811=vec128#6,<sel12=vec128#60
# asm 2: shufb >carry2=$10,<r811=$8,<r811=$8,<sel12=$62
shufb $10,$8,$8,$62

# qhasm: carry3 = select bytes from r1215 by sel12
# asm 1: shufb >carry3=vec128#9,<r1215=vec128#3,<r1215=vec128#3,<sel12=vec128#60
# asm 2: shufb >carry3=$11,<r1215=$5,<r1215=$5,<sel12=$62
shufb $11,$5,$5,$62

# qhasm: carry4 = select bytes from r1619 by sel12
# asm 1: shufb >carry4=vec128#10,<r1619=vec128#4,<r1619=vec128#4,<sel12=vec128#60
# asm 2: shufb >carry4=$12,<r1619=$6,<r1619=$6,<sel12=$62
shufb $12,$6,$6,$62

# qhasm: uint32323232 carry1 >>= 13
# asm 1: rotmi <carry1=vec128#7,<carry1=vec128#7,-13
# asm 2: rotmi <carry1=$9,<carry1=$9,-13
rotmi $9,$9,-13

# qhasm: uint32323232 carry2 >>= 13
# asm 1: rotmi <carry2=vec128#8,<carry2=vec128#8,-13
# asm 2: rotmi <carry2=$10,<carry2=$10,-13
rotmi $10,$10,-13

# qhasm: uint32323232 carry3 >>= 13
# asm 1: rotmi <carry3=vec128#9,<carry3=vec128#9,-13
# asm 2: rotmi <carry3=$11,<carry3=$11,-13
rotmi $11,$11,-13

# qhasm: uint32323232 carry4 >>= 13
# asm 1: rotmi <carry4=vec128#10,<carry4=vec128#10,-13
# asm 2: rotmi <carry4=$12,<carry4=$12,-13
rotmi $12,$12,-13

# qhasm: int32323232 r47 += carry1
# asm 1: a <r47=vec128#5,<r47=vec128#5,<carry1=vec128#7
# asm 2: a <r47=$7,<r47=$7,<carry1=$9
a $7,$7,$9

# qhasm: int32323232 r811 += carry2
# asm 1: a <r811=vec128#6,<r811=vec128#6,<carry2=vec128#8
# asm 2: a <r811=$8,<r811=$8,<carry2=$10
a $8,$8,$10

# qhasm: int32323232 r1215 += carry3
# asm 1: a <r1215=vec128#3,<r1215=vec128#3,<carry3=vec128#9
# asm 2: a <r1215=$5,<r1215=$5,<carry3=$11
a $5,$5,$11

# qhasm: int32323232 r1619 += carry4
# asm 1: a <r1619=vec128#4,<r1619=vec128#4,<carry4=vec128#10
# asm 2: a <r1619=$6,<r1619=$6,<carry4=$12
a $6,$6,$12

# qhasm: carry1 = select bytes from r47 by sel23
# asm 1: shufb >carry1=vec128#7,<r47=vec128#5,<r47=vec128#5,<sel23=vec128#61
# asm 2: shufb >carry1=$9,<r47=$7,<r47=$7,<sel23=$63
shufb $9,$7,$7,$63

# qhasm: carry2 = select bytes from r811 by sel23
# asm 1: shufb >carry2=vec128#8,<r811=vec128#6,<r811=vec128#6,<sel23=vec128#61
# asm 2: shufb >carry2=$10,<r811=$8,<r811=$8,<sel23=$63
shufb $10,$8,$8,$63

# qhasm: carry3 = select bytes from r1215 by sel23
# asm 1: shufb >carry3=vec128#9,<r1215=vec128#3,<r1215=vec128#3,<sel23=vec128#61
# asm 2: shufb >carry3=$11,<r1215=$5,<r1215=$5,<sel23=$63
shufb $11,$5,$5,$63

# qhasm: carry4 = select bytes from r1619 by sel23
# asm 1: shufb >carry4=vec128#10,<r1619=vec128#4,<r1619=vec128#4,<sel23=vec128#61
# asm 2: shufb >carry4=$12,<r1619=$6,<r1619=$6,<sel23=$63
shufb $12,$6,$6,$63

# qhasm: uint32323232 carry1 >>= 13
# asm 1: rotmi <carry1=vec128#7,<carry1=vec128#7,-13
# asm 2: rotmi <carry1=$9,<carry1=$9,-13
rotmi $9,$9,-13

# qhasm: uint32323232 carry2 >>= 13
# asm 1: rotmi <carry2=vec128#8,<carry2=vec128#8,-13
# asm 2: rotmi <carry2=$10,<carry2=$10,-13
rotmi $10,$10,-13

# qhasm: uint32323232 carry3 >>= 13
# asm 1: rotmi <carry3=vec128#9,<carry3=vec128#9,-13
# asm 2: rotmi <carry3=$11,<carry3=$11,-13
rotmi $11,$11,-13

# qhasm: uint32323232 carry4 >>= 13
# asm 1: rotmi <carry4=vec128#10,<carry4=vec128#10,-13
# asm 2: rotmi <carry4=$12,<carry4=$12,-13
rotmi $12,$12,-13

# qhasm: int32323232 r47 += carry1
# asm 1: a <r47=vec128#5,<r47=vec128#5,<carry1=vec128#7
# asm 2: a <r47=$7,<r47=$7,<carry1=$9
a $7,$7,$9

# qhasm: int32323232 r811 += carry2
# asm 1: a <r811=vec128#6,<r811=vec128#6,<carry2=vec128#8
# asm 2: a <r811=$8,<r811=$8,<carry2=$10
a $8,$8,$10

# qhasm: int32323232 r1215 += carry3
# asm 1: a <r1215=vec128#3,<r1215=vec128#3,<carry3=vec128#9
# asm 2: a <r1215=$5,<r1215=$5,<carry3=$11
a $5,$5,$11

# qhasm: int32323232 r1619 += carry4
# asm 1: a <r1619=vec128#4,<r1619=vec128#4,<carry4=vec128#10
# asm 2: a <r1619=$6,<r1619=$6,<carry4=$12
a $6,$6,$12

# qhasm: r47 &= redcoeffmaskveryend
# asm 1: and <r47=vec128#5,<r47=vec128#5,<redcoeffmaskveryend=vec128#65
# asm 2: and <r47=$7,<r47=$7,<redcoeffmaskveryend=$67
and $7,$7,$67

# qhasm: r811 &= redcoeffmaskveryend
# asm 1: and <r811=vec128#6,<r811=vec128#6,<redcoeffmaskveryend=vec128#65
# asm 2: and <r811=$8,<r811=$8,<redcoeffmaskveryend=$67
and $8,$8,$67

# qhasm: r1215 &= redcoeffmaskveryend
# asm 1: and <r1215=vec128#3,<r1215=vec128#3,<redcoeffmaskveryend=vec128#65
# asm 2: and <r1215=$5,<r1215=$5,<redcoeffmaskveryend=$67
and $5,$5,$67

# qhasm: r1619 &= redcoeffmaskveryend
# asm 1: and <r1619=vec128#4,<r1619=vec128#4,<redcoeffmaskveryend=vec128#65
# asm 2: and <r1619=$6,<r1619=$6,<redcoeffmaskveryend=$67
and $6,$6,$67

# qhasm: *(vec128 *) ((retp + 0) & ~15) = r03
# asm 1: stqd <r03=vec128#2,0(<retp=vec128#1)
# asm 2: stqd <r03=$4,0(<retp=$3)
stqd $4,0($3)

# qhasm: *(vec128 *) ((retp + 16) & ~15) = r47
# asm 1: stqd <r47=vec128#5,16(<retp=vec128#1)
# asm 2: stqd <r47=$7,16(<retp=$3)
stqd $7,16($3)

# qhasm: *(vec128 *) ((retp + 32) & ~15) = r811
# asm 1: stqd <r811=vec128#6,32(<retp=vec128#1)
# asm 2: stqd <r811=$8,32(<retp=$3)
stqd $8,32($3)

# qhasm: *(vec128 *) ((retp + 48) & ~15) = r1215
# asm 1: stqd <r1215=vec128#3,48(<retp=vec128#1)
# asm 2: stqd <r1215=$5,48(<retp=$3)
stqd $5,48($3)

# qhasm: *(vec128 *) ((retp + 64) & ~15) = r1619
# asm 1: stqd <r1619=vec128#4,64(<retp=vec128#1)
# asm 2: stqd <r1619=$6,64(<retp=$3)
stqd $6,64($3)

# qhasm: leave
ai $sp,$sp,32
bi $lr
