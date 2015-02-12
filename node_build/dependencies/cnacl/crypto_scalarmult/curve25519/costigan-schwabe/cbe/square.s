# version 20090331
# Peter Schwabe & Neil Costigan
# Public domain.

# qhasm: vec128 retp

# qhasm: vec128 ainp

# qhasm: input retp

# qhasm: input ainp

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

# qhasm: vec128 a2023

# qhasm: vec128 a2427

# qhasm: vec128 a2831

# qhasm: vec128 a3235

# qhasm: vec128 a3638

# qhasm: vec128 a03s11

# qhasm: vec128 a47s11

# qhasm: vec128 a811s11

# qhasm: vec128 a1215s11

# qhasm: vec128 a1619s11

# qhasm: vec128 a03s12

# qhasm: vec128 a47s12

# qhasm: vec128 a811s12

# qhasm: vec128 a1215s12

# qhasm: vec128 a1619s12

# qhasm: vec128 a03s13

# qhasm: vec128 a47s13

# qhasm: vec128 a811s13

# qhasm: vec128 a1215s13

# qhasm: vec128 a1619s13

# qhasm: vec128 a03s20

# qhasm: vec128 a47s20

# qhasm: vec128 a811s20

# qhasm: vec128 a1215s20

# qhasm: vec128 a1619s20

# qhasm: vec128 a03s21

# qhasm: vec128 a47s21

# qhasm: vec128 a811s21

# qhasm: vec128 a1215s21

# qhasm: vec128 a1619s21

# qhasm: vec128 a03s22

# qhasm: vec128 a47s22

# qhasm: vec128 a811s22

# qhasm: vec128 a1215s22

# qhasm: vec128 a1619s22

# qhasm: vec128 a03s23

# qhasm: vec128 a47s23

# qhasm: vec128 a811s23

# qhasm: vec128 a1215s23

# qhasm: vec128 a1619s23

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

# qhasm: vec128 vec19

# qhasm: vec128 shlw0001

# qhasm: vec128 shlw0011

# qhasm: vec128 shlw0111

# qhasm: vec128 shlw1111

# qhasm: vec128 shlw1112

# qhasm: vec128 shlw1122

# qhasm: vec128 shlw1222

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

# qhasm: stack128 call0_stack

# qhasm: stack128 call1_stack

# qhasm: stack128 call2_stack

# qhasm: stack128 call3_stack

# qhasm: stack128 call4_stack

# qhasm: stack128 call5_stack

# qhasm: stack128 call6_stack

# qhasm: stack128 call7_stack

# qhasm: stack128 call8_stack

# qhasm: stack128 call9_stack

# qhasm: enter square
.text
.align 3
.globl _square
.global square
.type square, @function
square:
stqd $lr, 16($sp)
stqd $sp,-96($sp)
ai $sp,$sp,-96

# qhasm: a03 = *(vec128 *) ((ainp + 0) & ~15)
# asm 1: lqd >a03=vec128#3,0(<ainp=vec128#2)
# asm 2: lqd >a03=$5,0(<ainp=$4)
lqd $5,0($4)

# qhasm: int32323232 zero = 0
# asm 1: il >zero=vec128#4,0
# asm 2: il >zero=$6,0
il $6,0

# qhasm: shlw0001 = extern(_shlw0001)
# asm 1: lqr <shlw0001=vec128#5,_shlw0001
# asm 2: lqr <shlw0001=$7,_shlw0001
lqr $7,_shlw0001

# qhasm: nop
 nop

# qhasm: shlw0011 = extern(_shlw0011)
# asm 1: lqr <shlw0011=vec128#6,_shlw0011
# asm 2: lqr <shlw0011=$8,_shlw0011
lqr $8,_shlw0011

# qhasm: shlw0111 = extern(_shlw0111)
# asm 1: lqr <shlw0111=vec128#7,_shlw0111
# asm 2: lqr <shlw0111=$9,_shlw0111
lqr $9,_shlw0111

# qhasm: a47 = *(vec128 *) ((ainp + 16) & ~15)
# asm 1: lqd >a47=vec128#8,16(<ainp=vec128#2)
# asm 2: lqd >a47=$10,16(<ainp=$4)
lqd $10,16($4)

# qhasm: a811 = *(vec128 *) ((ainp + 32) & ~15)
# asm 1: lqd >a811=vec128#9,32(<ainp=vec128#2)
# asm 2: lqd >a811=$11,32(<ainp=$4)
lqd $11,32($4)

# qhasm: a1215 = *(vec128 *) ((ainp + 48) & ~15)
# asm 1: lqd >a1215=vec128#10,48(<ainp=vec128#2)
# asm 2: lqd >a1215=$12,48(<ainp=$4)
lqd $12,48($4)

# qhasm: int32323232 a03s11 = a03 << shlw0001
# asm 1: shl >a03s11=vec128#11,<a03=vec128#3,<shlw0001=vec128#5
# asm 2: shl >a03s11=$13,<a03=$5,<shlw0001=$7
shl $13,$5,$7

# qhasm: a1619 = *(vec128 *) ((ainp + 64) & ~15)
# asm 1: lqd >a1619=vec128#2,64(<ainp=vec128#2)
# asm 2: lqd >a1619=$4,64(<ainp=$4)
lqd $4,64($4)

# qhasm: int32323232 a03s12 = a03 << shlw0011
# asm 1: shl >a03s12=vec128#12,<a03=vec128#3,<shlw0011=vec128#6
# asm 2: shl >a03s12=$14,<a03=$5,<shlw0011=$8
shl $14,$5,$8

# qhasm: shlw1111 = extern(_shlw1111)
# asm 1: lqr <shlw1111=vec128#13,_shlw1111
# asm 2: lqr <shlw1111=$15,_shlw1111
lqr $15,_shlw1111

# qhasm: int32323232 a03s13 = a03 << shlw0111
# asm 1: shl >a03s13=vec128#14,<a03=vec128#3,<shlw0111=vec128#7
# asm 2: shl >a03s13=$16,<a03=$5,<shlw0111=$9
shl $16,$5,$9

# qhasm: shlw1112 = extern(_shlw1112)
# asm 1: lqr <shlw1112=vec128#15,_shlw1112
# asm 2: lqr <shlw1112=$17,_shlw1112
lqr $17,_shlw1112

# qhasm: int32323232 a47s11 = a47 << shlw0001
# asm 1: shl >a47s11=vec128#16,<a47=vec128#8,<shlw0001=vec128#5
# asm 2: shl >a47s11=$18,<a47=$10,<shlw0001=$7
shl $18,$10,$7

# qhasm: shlw1122 = extern(_shlw1122)
# asm 1: lqr <shlw1122=vec128#17,_shlw1122
# asm 2: lqr <shlw1122=$19,_shlw1122
lqr $19,_shlw1122

# qhasm: int32323232 a47s12 = a47 << shlw0011
# asm 1: shl >a47s12=vec128#18,<a47=vec128#8,<shlw0011=vec128#6
# asm 2: shl >a47s12=$20,<a47=$10,<shlw0011=$8
shl $20,$10,$8

# qhasm: shlw1222 = extern(_shlw1222)
# asm 1: lqr <shlw1222=vec128#19,_shlw1222
# asm 2: lqr <shlw1222=$21,_shlw1222
lqr $21,_shlw1222

# qhasm: int32323232 a47s13 = a47 << shlw0111
# asm 1: shl >a47s13=vec128#20,<a47=vec128#8,<shlw0111=vec128#7
# asm 2: shl >a47s13=$22,<a47=$10,<shlw0111=$9
shl $22,$10,$9

# qhasm: sel1 = extern(select1)
# asm 1: lqr <sel1=vec128#21,select1
# asm 2: lqr <sel1=$23,select1
lqr $23,select1

# qhasm: int32323232 a811s11 = a811 << shlw0001
# asm 1: shl >a811s11=vec128#22,<a811=vec128#9,<shlw0001=vec128#5
# asm 2: shl >a811s11=$24,<a811=$11,<shlw0001=$7
shl $24,$11,$7

# qhasm: sel3 = extern(select3)
# asm 1: lqr <sel3=vec128#23,select3
# asm 2: lqr <sel3=$25,select3
lqr $25,select3

# qhasm: int32323232 a811s12 = a811 << shlw0011
# asm 1: shl >a811s12=vec128#24,<a811=vec128#9,<shlw0011=vec128#6
# asm 2: shl >a811s12=$26,<a811=$11,<shlw0011=$8
shl $26,$11,$8

# qhasm: sel5 = extern(select5)
# asm 1: lqr <sel5=vec128#25,select5
# asm 2: lqr <sel5=$27,select5
lqr $27,select5

# qhasm: int32323232 a811s13 = a811 << shlw0111
# asm 1: shl >a811s13=vec128#26,<a811=vec128#9,<shlw0111=vec128#7
# asm 2: shl >a811s13=$28,<a811=$11,<shlw0111=$9
shl $28,$11,$9

# qhasm: sel7 = extern(select7)
# asm 1: lqr <sel7=vec128#27,select7
# asm 2: lqr <sel7=$29,select7
lqr $29,select7

# qhasm: int32323232 a1215s11 = a1215 << shlw0001
# asm 1: shl >a1215s11=vec128#28,<a1215=vec128#10,<shlw0001=vec128#5
# asm 2: shl >a1215s11=$30,<a1215=$12,<shlw0001=$7
shl $30,$12,$7

# qhasm: sel01 = extern(select01)
# asm 1: lqr <sel01=vec128#29,select01
# asm 2: lqr <sel01=$31,select01
lqr $31,select01

# qhasm: int32323232 a1215s12 = a1215 << shlw0011
# asm 1: shl >a1215s12=vec128#30,<a1215=vec128#10,<shlw0011=vec128#6
# asm 2: shl >a1215s12=$32,<a1215=$12,<shlw0011=$8
shl $32,$12,$8

# qhasm: sel12 = extern(select12)
# asm 1: lqr <sel12=vec128#31,select12
# asm 2: lqr <sel12=$33,select12
lqr $33,select12

# qhasm: int32323232 a1215s13 = a1215 << shlw0111
# asm 1: shl >a1215s13=vec128#32,<a1215=vec128#10,<shlw0111=vec128#7
# asm 2: shl >a1215s13=$34,<a1215=$12,<shlw0111=$9
shl $34,$12,$9

# qhasm: sel23 = extern(select23)
# asm 1: lqr <sel23=vec128#33,select23
# asm 2: lqr <sel23=$35,select23
lqr $35,select23

# qhasm: int32323232 a1619s11 = a1619 << shlw0001
# asm 1: shl >a1619s11=vec128#5,<a1619=vec128#2,<shlw0001=vec128#5
# asm 2: shl >a1619s11=$7,<a1619=$4,<shlw0001=$7
shl $7,$4,$7

# qhasm: sel30 = extern(select30)
# asm 1: lqr <sel30=vec128#34,select30
# asm 2: lqr <sel30=$36,select30
lqr $36,select30

# qhasm: int32323232 a1619s12 = a1619 << shlw0011
# asm 1: shl >a1619s12=vec128#6,<a1619=vec128#2,<shlw0011=vec128#6
# asm 2: shl >a1619s12=$8,<a1619=$4,<shlw0011=$8
shl $8,$4,$8

# qhasm: redcoeffmask = extern(redCoeffMask)
# asm 1: lqr <redcoeffmask=vec128#35,redCoeffMask
# asm 2: lqr <redcoeffmask=$37,redCoeffMask
lqr $37,redCoeffMask

# qhasm: int32323232 a1619s13 = a1619 << shlw0111
# asm 1: shl >a1619s13=vec128#7,<a1619=vec128#2,<shlw0111=vec128#7
# asm 2: shl >a1619s13=$9,<a1619=$4,<shlw0111=$9
shl $9,$4,$9

# qhasm: a1 = select bytes from a03 by sel3
# asm 1: shufb >a1=vec128#36,<a03=vec128#3,<a03=vec128#3,<sel3=vec128#23
# asm 2: shufb >a1=$38,<a03=$5,<a03=$5,<sel3=$25
shufb $38,$5,$5,$25

# qhasm: int32323232 a03s20 = a03 << shlw1111
# asm 1: shl >a03s20=vec128#37,<a03=vec128#3,<shlw1111=vec128#13
# asm 2: shl >a03s20=$39,<a03=$5,<shlw1111=$15
shl $39,$5,$15

# qhasm: redcoeffmaskend = extern(redCoeffMaskEnd)
# asm 1: lqr <redcoeffmaskend=vec128#38,redCoeffMaskEnd
# asm 2: lqr <redcoeffmaskend=$40,redCoeffMaskEnd
lqr $40,redCoeffMaskEnd

# qhasm: int32323232 a03s21 = a03 << shlw1112
# asm 1: shl >a03s21=vec128#39,<a03=vec128#3,<shlw1112=vec128#15
# asm 2: shl >a03s21=$41,<a03=$5,<shlw1112=$17
shl $41,$5,$17

# qhasm: redcoeffmaskveryend = extern(redCoeffMaskVeryEnd)
# asm 1: lqr <redcoeffmaskveryend=vec128#40,redCoeffMaskVeryEnd
# asm 2: lqr <redcoeffmaskveryend=$42,redCoeffMaskVeryEnd
lqr $42,redCoeffMaskVeryEnd

# qhasm: int32323232 a03s22 = a03 << shlw1122
# asm 1: shl >a03s22=vec128#41,<a03=vec128#3,<shlw1122=vec128#17
# asm 2: shl >a03s22=$43,<a03=$5,<shlw1122=$19
shl $43,$5,$19

# qhasm: call0_stack = call0
# asm 1: stqd <call0=vec128#78,[32+>call0_stack=stack128#1]($sp)
# asm 2: stqd <call0=$80,[32+>call0_stack=0]($sp)
stqd $80,[32+0]($sp)

# qhasm: int32323232 a03s23 = a03 << shlw1222
# asm 1: shl >a03s23=vec128#42,<a03=vec128#3,<shlw1222=vec128#19
# asm 2: shl >a03s23=$44,<a03=$5,<shlw1222=$21
shl $44,$5,$21

# qhasm: call1_stack = call1
# asm 1: stqd <call1=vec128#79,[32+>call1_stack=stack128#2]($sp)
# asm 2: stqd <call1=$81,[32+>call1_stack=16]($sp)
stqd $81,[32+16]($sp)

# qhasm: int32323232 a47s20 = a47 << shlw1111
# asm 1: shl >a47s20=vec128#43,<a47=vec128#8,<shlw1111=vec128#13
# asm 2: shl >a47s20=$45,<a47=$10,<shlw1111=$15
shl $45,$10,$15

# qhasm: call2_stack = call2
# asm 1: stqd <call2=vec128#80,[32+>call2_stack=stack128#3]($sp)
# asm 2: stqd <call2=$82,[32+>call2_stack=32]($sp)
stqd $82,[32+32]($sp)

# qhasm: int32323232 a47s21 = a47 << shlw1112
# asm 1: shl >a47s21=vec128#44,<a47=vec128#8,<shlw1112=vec128#15
# asm 2: shl >a47s21=$46,<a47=$10,<shlw1112=$17
shl $46,$10,$17

# qhasm: a0 = select bytes from a03 by sel1
# asm 1: shufb >a0=vec128#45,<a03=vec128#3,<a03=vec128#3,<sel1=vec128#21
# asm 2: shufb >a0=$47,<a03=$5,<a03=$5,<sel1=$23
shufb $47,$5,$5,$23

# qhasm: int32323232 a47s22 = a47 << shlw1122
# asm 1: shl >a47s22=vec128#46,<a47=vec128#8,<shlw1122=vec128#17
# asm 2: shl >a47s22=$48,<a47=$10,<shlw1122=$19
shl $48,$10,$19

# qhasm: a2 = select bytes from a03 by sel5
# asm 1: shufb >a2=vec128#47,<a03=vec128#3,<a03=vec128#3,<sel5=vec128#25
# asm 2: shufb >a2=$49,<a03=$5,<a03=$5,<sel5=$27
shufb $49,$5,$5,$27

# qhasm: int32323232 a47s23 = a47 << shlw1222
# asm 1: shl >a47s23=vec128#48,<a47=vec128#8,<shlw1222=vec128#19
# asm 2: shl >a47s23=$50,<a47=$10,<shlw1222=$21
shl $50,$10,$21

# qhasm: a3 = select bytes from a03 by sel7
# asm 1: shufb >a3=vec128#49,<a03=vec128#3,<a03=vec128#3,<sel7=vec128#27
# asm 2: shufb >a3=$51,<a03=$5,<a03=$5,<sel7=$29
shufb $51,$5,$5,$29

# qhasm: int32323232 a811s20 = a811 << shlw1111
# asm 1: shl >a811s20=vec128#50,<a811=vec128#9,<shlw1111=vec128#13
# asm 2: shl >a811s20=$52,<a811=$11,<shlw1111=$15
shl $52,$11,$15

# qhasm: a4 = select bytes from a47 by sel1
# asm 1: shufb >a4=vec128#51,<a47=vec128#8,<a47=vec128#8,<sel1=vec128#21
# asm 2: shufb >a4=$53,<a47=$10,<a47=$10,<sel1=$23
shufb $53,$10,$10,$23

# qhasm: int32323232 a811s21 = a811 << shlw1112
# asm 1: shl >a811s21=vec128#52,<a811=vec128#9,<shlw1112=vec128#15
# asm 2: shl >a811s21=$54,<a811=$11,<shlw1112=$17
shl $54,$11,$17

# qhasm: a5 = select bytes from a47 by sel3
# asm 1: shufb >a5=vec128#53,<a47=vec128#8,<a47=vec128#8,<sel3=vec128#23
# asm 2: shufb >a5=$55,<a47=$10,<a47=$10,<sel3=$25
shufb $55,$10,$10,$25

# qhasm: int32323232 a811s22 = a811 << shlw1122
# asm 1: shl >a811s22=vec128#54,<a811=vec128#9,<shlw1122=vec128#17
# asm 2: shl >a811s22=$56,<a811=$11,<shlw1122=$19
shl $56,$11,$19

# qhasm: a6 = select bytes from a47 by sel5
# asm 1: shufb >a6=vec128#55,<a47=vec128#8,<a47=vec128#8,<sel5=vec128#25
# asm 2: shufb >a6=$57,<a47=$10,<a47=$10,<sel5=$27
shufb $57,$10,$10,$27

# qhasm: int32323232 a811s23 = a811 << shlw1222
# asm 1: shl >a811s23=vec128#56,<a811=vec128#9,<shlw1222=vec128#19
# asm 2: shl >a811s23=$58,<a811=$11,<shlw1222=$21
shl $58,$11,$21

# qhasm: a7 = select bytes from a47 by sel7
# asm 1: shufb >a7=vec128#57,<a47=vec128#8,<a47=vec128#8,<sel7=vec128#27
# asm 2: shufb >a7=$59,<a47=$10,<a47=$10,<sel7=$29
shufb $59,$10,$10,$29

# qhasm: int32323232 a1215s20 = a1215 << shlw1111
# asm 1: shl >a1215s20=vec128#58,<a1215=vec128#10,<shlw1111=vec128#13
# asm 2: shl >a1215s20=$60,<a1215=$12,<shlw1111=$15
shl $60,$12,$15

# qhasm: a8 = select bytes from a811 by sel1
# asm 1: shufb >a8=vec128#59,<a811=vec128#9,<a811=vec128#9,<sel1=vec128#21
# asm 2: shufb >a8=$61,<a811=$11,<a811=$11,<sel1=$23
shufb $61,$11,$11,$23

# qhasm: int32323232 a1215s21 = a1215 << shlw1112
# asm 1: shl >a1215s21=vec128#60,<a1215=vec128#10,<shlw1112=vec128#15
# asm 2: shl >a1215s21=$62,<a1215=$12,<shlw1112=$17
shl $62,$12,$17

# qhasm: a9 = select bytes from a811 by sel3
# asm 1: shufb >a9=vec128#61,<a811=vec128#9,<a811=vec128#9,<sel3=vec128#23
# asm 2: shufb >a9=$63,<a811=$11,<a811=$11,<sel3=$25
shufb $63,$11,$11,$25

# qhasm: int32323232 a1215s22 = a1215 << shlw1122
# asm 1: shl >a1215s22=vec128#62,<a1215=vec128#10,<shlw1122=vec128#17
# asm 2: shl >a1215s22=$64,<a1215=$12,<shlw1122=$19
shl $64,$12,$19

# qhasm: a10 = select bytes from a811 by sel5
# asm 1: shufb >a10=vec128#63,<a811=vec128#9,<a811=vec128#9,<sel5=vec128#25
# asm 2: shufb >a10=$65,<a811=$11,<a811=$11,<sel5=$27
shufb $65,$11,$11,$27

# qhasm: int32323232 a1215s23 = a1215 << shlw1222
# asm 1: shl >a1215s23=vec128#64,<a1215=vec128#10,<shlw1222=vec128#19
# asm 2: shl >a1215s23=$66,<a1215=$12,<shlw1222=$21
shl $66,$12,$21

# qhasm: a11 = select bytes from a811 by sel7
# asm 1: shufb >a11=vec128#65,<a811=vec128#9,<a811=vec128#9,<sel7=vec128#27
# asm 2: shufb >a11=$67,<a811=$11,<a811=$11,<sel7=$29
shufb $67,$11,$11,$29

# qhasm: int32323232 a1619s20 = a1619 << shlw1111
# asm 1: shl >a1619s20=vec128#13,<a1619=vec128#2,<shlw1111=vec128#13
# asm 2: shl >a1619s20=$15,<a1619=$4,<shlw1111=$15
shl $15,$4,$15

# qhasm: a12 = select bytes from a1215 by sel1
# asm 1: shufb >a12=vec128#66,<a1215=vec128#10,<a1215=vec128#10,<sel1=vec128#21
# asm 2: shufb >a12=$68,<a1215=$12,<a1215=$12,<sel1=$23
shufb $68,$12,$12,$23

# qhasm: int32323232 a1619s21 = a1619 << shlw1112
# asm 1: shl >a1619s21=vec128#15,<a1619=vec128#2,<shlw1112=vec128#15
# asm 2: shl >a1619s21=$17,<a1619=$4,<shlw1112=$17
shl $17,$4,$17

# qhasm: a13 = select bytes from a1215 by sel3
# asm 1: shufb >a13=vec128#67,<a1215=vec128#10,<a1215=vec128#10,<sel3=vec128#23
# asm 2: shufb >a13=$69,<a1215=$12,<a1215=$12,<sel3=$25
shufb $69,$12,$12,$25

# qhasm: int32323232 a1619s22 = a1619 << shlw1122
# asm 1: shl >a1619s22=vec128#17,<a1619=vec128#2,<shlw1122=vec128#17
# asm 2: shl >a1619s22=$19,<a1619=$4,<shlw1122=$19
shl $19,$4,$19

# qhasm: a14 = select bytes from a1215 by sel5
# asm 1: shufb >a14=vec128#68,<a1215=vec128#10,<a1215=vec128#10,<sel5=vec128#25
# asm 2: shufb >a14=$70,<a1215=$12,<a1215=$12,<sel5=$27
shufb $70,$12,$12,$27

# qhasm: int32323232 a1619s23 = a1619 << shlw1222
# asm 1: shl >a1619s23=vec128#19,<a1619=vec128#2,<shlw1222=vec128#19
# asm 2: shl >a1619s23=$21,<a1619=$4,<shlw1222=$21
shl $21,$4,$21

# qhasm: a15 = select bytes from a1215 by sel7
# asm 1: shufb >a15=vec128#69,<a1215=vec128#10,<a1215=vec128#10,<sel7=vec128#27
# asm 2: shufb >a15=$71,<a1215=$12,<a1215=$12,<sel7=$29
shufb $71,$12,$12,$29

# qhasm: int32323232 r03    = (a0  & 0xffff) * (a03   & 0xffff)
# asm 1: mpy >r03=vec128#3,<a0=vec128#45,<a03=vec128#3
# asm 2: mpy >r03=$5,<a0=$47,<a03=$5
mpy $5,$47,$5

# qhasm: a16 = select bytes from a1619 by sel1
# asm 1: shufb >a16=vec128#21,<a1619=vec128#2,<a1619=vec128#2,<sel1=vec128#21
# asm 2: shufb >a16=$23,<a1619=$4,<a1619=$4,<sel1=$23
shufb $23,$4,$4,$23

# qhasm: int32323232 r47    = (a0  & 0xffff) * (a47s20   & 0xffff)
# asm 1: mpy >r47=vec128#43,<a0=vec128#45,<a47s20=vec128#43
# asm 2: mpy >r47=$45,<a0=$47,<a47s20=$45
mpy $45,$47,$45

# qhasm: a17 = select bytes from a1619 by sel3
# asm 1: shufb >a17=vec128#23,<a1619=vec128#2,<a1619=vec128#2,<sel3=vec128#23
# asm 2: shufb >a17=$25,<a1619=$4,<a1619=$4,<sel3=$25
shufb $25,$4,$4,$25

# qhasm: int32323232 r811   = (a0  & 0xffff) * (a811s20  & 0xffff)
# asm 1: mpy >r811=vec128#70,<a0=vec128#45,<a811s20=vec128#50
# asm 2: mpy >r811=$72,<a0=$47,<a811s20=$52
mpy $72,$47,$52

# qhasm: a18 = select bytes from a1619 by sel5
# asm 1: shufb >a18=vec128#25,<a1619=vec128#2,<a1619=vec128#2,<sel5=vec128#25
# asm 2: shufb >a18=$27,<a1619=$4,<a1619=$4,<sel5=$27
shufb $27,$4,$4,$27

# qhasm: int32323232 r1215  = (a0  & 0xffff) * (a1215s20 & 0xffff)
# asm 1: mpy >r1215=vec128#71,<a0=vec128#45,<a1215s20=vec128#58
# asm 2: mpy >r1215=$73,<a0=$47,<a1215s20=$60
mpy $73,$47,$60

# qhasm: a19 = select bytes from a1619 by sel7
# asm 1: shufb >a19=vec128#27,<a1619=vec128#2,<a1619=vec128#2,<sel7=vec128#27
# asm 2: shufb >a19=$29,<a1619=$4,<a1619=$4,<sel7=$29
shufb $29,$4,$4,$29

# qhasm: int32323232 r1619  = (a0  & 0xffff) * (a1619s20 & 0xffff)
# asm 1: mpy >r1619=vec128#45,<a0=vec128#45,<a1619s20=vec128#13
# asm 2: mpy >r1619=$47,<a0=$47,<a1619s20=$15
mpy $47,$47,$15

# qhasm: comb13 = extern(combine13)
# asm 1: lqr <comb13=vec128#72,combine13
# asm 2: lqr <comb13=$74,combine13
lqr $74,combine13

# qhasm: int32323232 r14    = (a1  & 0xffff) * (a03s11   & 0xffff)
# asm 1: mpy >r14=vec128#11,<a1=vec128#36,<a03s11=vec128#11
# asm 2: mpy >r14=$13,<a1=$38,<a03s11=$13
mpy $13,$38,$13

# qhasm: comb22 = extern(combine22)
# asm 1: lqr <comb22=vec128#73,combine22
# asm 2: lqr <comb22=$75,combine22
lqr $75,combine22

# qhasm: int32323232 r58    = (a1  & 0xffff) * (a47s21   & 0xffff)
# asm 1: mpy >r58=vec128#44,<a1=vec128#36,<a47s21=vec128#44
# asm 2: mpy >r58=$46,<a1=$38,<a47s21=$46
mpy $46,$38,$46

# qhasm: comb31 = extern(combine31)
# asm 1: lqr <comb31=vec128#74,combine31
# asm 2: lqr <comb31=$76,combine31
lqr $76,combine31

# qhasm: int32323232 r912   = (a1  & 0xffff) * (a811s21  & 0xffff)
# asm 1: mpy >r912=vec128#75,<a1=vec128#36,<a811s21=vec128#52
# asm 2: mpy >r912=$77,<a1=$38,<a811s21=$54
mpy $77,$38,$54

# qhasm: comb31 = extern(combine31)
# asm 1: lqr <comb31=vec128#74,combine31
# asm 2: lqr <comb31=$76,combine31
lqr $76,combine31

# qhasm: int32323232 r1316  = (a1  & 0xffff) * (a1215s21 & 0xffff)
# asm 1: mpy >r1316=vec128#76,<a1=vec128#36,<a1215s21=vec128#60
# asm 2: mpy >r1316=$78,<a1=$38,<a1215s21=$62
mpy $78,$38,$62

# qhasm: vec19 = extern(_vec19)
# asm 1: lqr <vec19=vec128#77,_vec19
# asm 2: lqr <vec19=$79,_vec19
lqr $79,_vec19

# qhasm: int32323232 r1720  = (a1  & 0xffff) * (a1619s21 & 0xffff)
# asm 1: mpy >r1720=vec128#36,<a1=vec128#36,<a1619s21=vec128#15
# asm 2: mpy >r1720=$38,<a1=$38,<a1619s21=$17
mpy $38,$38,$17

# qhasm: call3_stack = call3
# asm 1: stqd <call3=vec128#81,[32+>call3_stack=stack128#4]($sp)
# asm 2: stqd <call3=$83,[32+>call3_stack=48]($sp)
stqd $83,[32+48]($sp)

# qhasm: int32323232 r25    = (a2  & 0xffff) * (a03s12   & 0xffff)
# asm 1: mpy >r25=vec128#12,<a2=vec128#47,<a03s12=vec128#12
# asm 2: mpy >r25=$14,<a2=$49,<a03s12=$14
mpy $14,$49,$14

# qhasm: int32323232 r69    = (a2  & 0xffff) * (a47s22   & 0xffff)
# asm 1: mpy >r69=vec128#46,<a2=vec128#47,<a47s22=vec128#46
# asm 2: mpy >r69=$48,<a2=$49,<a47s22=$48
mpy $48,$49,$48

# qhasm: int32323232 r1013  = (a2  & 0xffff) * (a811s22  & 0xffff)
# asm 1: mpy >r1013=vec128#78,<a2=vec128#47,<a811s22=vec128#54
# asm 2: mpy >r1013=$80,<a2=$49,<a811s22=$56
mpy $80,$49,$56

# qhasm: int32323232 r1417  = (a2  & 0xffff) * (a1215s22 & 0xffff)
# asm 1: mpy >r1417=vec128#79,<a2=vec128#47,<a1215s22=vec128#62
# asm 2: mpy >r1417=$81,<a2=$49,<a1215s22=$64
mpy $81,$49,$64

# qhasm: int32323232 r1821  = (a2  & 0xffff) * (a1619s22 & 0xffff)
# asm 1: mpy >r1821=vec128#47,<a2=vec128#47,<a1619s22=vec128#17
# asm 2: mpy >r1821=$49,<a2=$49,<a1619s22=$19
mpy $49,$49,$19

# qhasm: int32323232 r36    = (a3  & 0xffff) * (a03s13   & 0xffff)
# asm 1: mpy >r36=vec128#14,<a3=vec128#49,<a03s13=vec128#14
# asm 2: mpy >r36=$16,<a3=$51,<a03s13=$16
mpy $16,$51,$16

# qhasm: int32323232 r710   = (a3  & 0xffff) * (a47s23   & 0xffff)
# asm 1: mpy >r710=vec128#48,<a3=vec128#49,<a47s23=vec128#48
# asm 2: mpy >r710=$50,<a3=$51,<a47s23=$50
mpy $50,$51,$50

# qhasm: int32323232 r1114  = (a3  & 0xffff) * (a811s23  & 0xffff)
# asm 1: mpy >r1114=vec128#80,<a3=vec128#49,<a811s23=vec128#56
# asm 2: mpy >r1114=$82,<a3=$51,<a811s23=$58
mpy $82,$51,$58

# qhasm: int32323232 r1518  = (a3  & 0xffff) * (a1215s23 & 0xffff)
# asm 1: mpy >r1518=vec128#81,<a3=vec128#49,<a1215s23=vec128#64
# asm 2: mpy >r1518=$83,<a3=$51,<a1215s23=$66
mpy $83,$51,$66

# qhasm: int32323232 r1922  = (a3  & 0xffff) * (a1619s23 & 0xffff)
# asm 1: mpy >r1922=vec128#49,<a3=vec128#49,<a1619s23=vec128#19
# asm 2: mpy >r1922=$51,<a3=$51,<a1619s23=$21
mpy $51,$51,$21

# qhasm: int32323232 r811  += (a4  & 0xffff) * (a47   & 0xffff)
# asm 1: mpya >r811=vec128#8,<a4=vec128#51,<a47=vec128#8,<r811=vec128#70
# asm 2: mpya >r811=$10,<a4=$53,<a47=$10,<r811=$72
mpya $10,$53,$10,$72

# qhasm: int32323232 r1215 += (a4  & 0xffff) * (a811s20  & 0xffff)
# asm 1: mpya >r1215=vec128#50,<a4=vec128#51,<a811s20=vec128#50,<r1215=vec128#71
# asm 2: mpya >r1215=$52,<a4=$53,<a811s20=$52,<r1215=$73
mpya $52,$53,$52,$73

# qhasm: int32323232 r1619 += (a4  & 0xffff) * (a1215s20 & 0xffff)
# asm 1: mpya >r1619=vec128#45,<a4=vec128#51,<a1215s20=vec128#58,<r1619=vec128#45
# asm 2: mpya >r1619=$47,<a4=$53,<a1215s20=$60,<r1619=$47
mpya $47,$53,$60,$47

# qhasm: int32323232 r2023  = (a4  & 0xffff) * (a1619s20 & 0xffff)
# asm 1: mpy >r2023=vec128#51,<a4=vec128#51,<a1619s20=vec128#13
# asm 2: mpy >r2023=$53,<a4=$53,<a1619s20=$15
mpy $53,$53,$15

# qhasm: int32323232 r912  += (a5  & 0xffff) * (a47s11   & 0xffff)
# asm 1: mpya >r912=vec128#16,<a5=vec128#53,<a47s11=vec128#16,<r912=vec128#75
# asm 2: mpya >r912=$18,<a5=$55,<a47s11=$18,<r912=$77
mpya $18,$55,$18,$77

# qhasm: int32323232 r1316 += (a5  & 0xffff) * (a811s21  & 0xffff)
# asm 1: mpya >r1316=vec128#52,<a5=vec128#53,<a811s21=vec128#52,<r1316=vec128#76
# asm 2: mpya >r1316=$54,<a5=$55,<a811s21=$54,<r1316=$78
mpya $54,$55,$54,$78

# qhasm: int32323232 r1720 += (a5  & 0xffff) * (a1215s21 & 0xffff)
# asm 1: mpya >r1720=vec128#36,<a5=vec128#53,<a1215s21=vec128#60,<r1720=vec128#36
# asm 2: mpya >r1720=$38,<a5=$55,<a1215s21=$62,<r1720=$38
mpya $38,$55,$62,$38

# qhasm: int32323232 r2124  = (a5  & 0xffff) * (a1619s21 & 0xffff)
# asm 1: mpy >r2124=vec128#53,<a5=vec128#53,<a1619s21=vec128#15
# asm 2: mpy >r2124=$55,<a5=$55,<a1619s21=$17
mpy $55,$55,$17

# qhasm: int32323232 r1013 += (a6  & 0xffff) * (a47s12   & 0xffff)
# asm 1: mpya >r1013=vec128#18,<a6=vec128#55,<a47s12=vec128#18,<r1013=vec128#78
# asm 2: mpya >r1013=$20,<a6=$57,<a47s12=$20,<r1013=$80
mpya $20,$57,$20,$80

# qhasm: int32323232 r1417 += (a6  & 0xffff) * (a811s22  & 0xffff)
# asm 1: mpya >r1417=vec128#54,<a6=vec128#55,<a811s22=vec128#54,<r1417=vec128#79
# asm 2: mpya >r1417=$56,<a6=$57,<a811s22=$56,<r1417=$81
mpya $56,$57,$56,$81

# qhasm: int32323232 r1821 += (a6  & 0xffff) * (a1215s22 & 0xffff)
# asm 1: mpya >r1821=vec128#47,<a6=vec128#55,<a1215s22=vec128#62,<r1821=vec128#47
# asm 2: mpya >r1821=$49,<a6=$57,<a1215s22=$64,<r1821=$49
mpya $49,$57,$64,$49

# qhasm: int32323232 r2225  = (a6  & 0xffff) * (a1619s22 & 0xffff)
# asm 1: mpy >r2225=vec128#55,<a6=vec128#55,<a1619s22=vec128#17
# asm 2: mpy >r2225=$57,<a6=$57,<a1619s22=$19
mpy $57,$57,$19

# qhasm: int32323232 r1114 += (a7  & 0xffff) * (a47s13   & 0xffff)
# asm 1: mpya >r1114=vec128#20,<a7=vec128#57,<a47s13=vec128#20,<r1114=vec128#80
# asm 2: mpya >r1114=$22,<a7=$59,<a47s13=$22,<r1114=$82
mpya $22,$59,$22,$82

# qhasm: int32323232 r1518 += (a7  & 0xffff) * (a811s23  & 0xffff)
# asm 1: mpya >r1518=vec128#56,<a7=vec128#57,<a811s23=vec128#56,<r1518=vec128#81
# asm 2: mpya >r1518=$58,<a7=$59,<a811s23=$58,<r1518=$83
mpya $58,$59,$58,$83

# qhasm: int32323232 r1922 += (a7  & 0xffff) * (a1215s23 & 0xffff)
# asm 1: mpya >r1922=vec128#49,<a7=vec128#57,<a1215s23=vec128#64,<r1922=vec128#49
# asm 2: mpya >r1922=$51,<a7=$59,<a1215s23=$66,<r1922=$51
mpya $51,$59,$66,$51

# qhasm: int32323232 r2326  = (a7  & 0xffff) * (a1619s23 & 0xffff)
# asm 1: mpy >r2326=vec128#57,<a7=vec128#57,<a1619s23=vec128#19
# asm 2: mpy >r2326=$59,<a7=$59,<a1619s23=$21
mpy $59,$59,$21

# qhasm: int32323232 r1619 += (a8  & 0xffff) * (a811  & 0xffff)
# asm 1: mpya >r1619=vec128#9,<a8=vec128#59,<a811=vec128#9,<r1619=vec128#45
# asm 2: mpya >r1619=$11,<a8=$61,<a811=$11,<r1619=$47
mpya $11,$61,$11,$47

# qhasm: int32323232 r2023 += (a8  & 0xffff) * (a1215s20 & 0xffff)
# asm 1: mpya >r2023=vec128#45,<a8=vec128#59,<a1215s20=vec128#58,<r2023=vec128#51
# asm 2: mpya >r2023=$47,<a8=$61,<a1215s20=$60,<r2023=$53
mpya $47,$61,$60,$53

# qhasm: int32323232 r2427  = (a8  & 0xffff) * (a1619s20 & 0xffff)
# asm 1: mpy >r2427=vec128#51,<a8=vec128#59,<a1619s20=vec128#13
# asm 2: mpy >r2427=$53,<a8=$61,<a1619s20=$15
mpy $53,$61,$15

# qhasm: int32323232 r1720 += (a9  & 0xffff) * (a811s11  & 0xffff)
# asm 1: mpya >r1720=vec128#22,<a9=vec128#61,<a811s11=vec128#22,<r1720=vec128#36
# asm 2: mpya >r1720=$24,<a9=$63,<a811s11=$24,<r1720=$38
mpya $24,$63,$24,$38

# qhasm: int32323232 r2124 += (a9  & 0xffff) * (a1215s21 & 0xffff)
# asm 1: mpya >r2124=vec128#36,<a9=vec128#61,<a1215s21=vec128#60,<r2124=vec128#53
# asm 2: mpya >r2124=$38,<a9=$63,<a1215s21=$62,<r2124=$55
mpya $38,$63,$62,$55

# qhasm: int32323232 r2528  = (a9  & 0xffff) * (a1619s21 & 0xffff)
# asm 1: mpy >r2528=vec128#53,<a9=vec128#61,<a1619s21=vec128#15
# asm 2: mpy >r2528=$55,<a9=$63,<a1619s21=$17
mpy $55,$63,$17

# qhasm: int32323232 r1821 += (a10 & 0xffff) * (a811s12  & 0xffff)
# asm 1: mpya >r1821=vec128#24,<a10=vec128#63,<a811s12=vec128#24,<r1821=vec128#47
# asm 2: mpya >r1821=$26,<a10=$65,<a811s12=$26,<r1821=$49
mpya $26,$65,$26,$49

# qhasm: tmp10 = combine zero  and r14   by comb13
# asm 1: shufb >tmp10=vec128#47,<zero=vec128#4,<r14=vec128#11,<comb13=vec128#72
# asm 2: shufb >tmp10=$49,<zero=$6,<r14=$13,<comb13=$74
shufb $49,$6,$13,$74

# qhasm: int32323232 r2225 += (a10 & 0xffff) * (a1215s22 & 0xffff)
# asm 1: mpya >r2225=vec128#55,<a10=vec128#63,<a1215s22=vec128#62,<r2225=vec128#55
# asm 2: mpya >r2225=$57,<a10=$65,<a1215s22=$64,<r2225=$57
mpya $57,$65,$64,$57

# qhasm: tmp20 = combine zero  and r25   by comb22
# asm 1: shufb >tmp20=vec128#58,<zero=vec128#4,<r25=vec128#12,<comb22=vec128#73
# asm 2: shufb >tmp20=$60,<zero=$6,<r25=$14,<comb22=$75
shufb $60,$6,$14,$75

# qhasm: int32323232 r2629  = (a10 & 0xffff) * (a1619s22 & 0xffff)
# asm 1: mpy >r2629=vec128#59,<a10=vec128#63,<a1619s22=vec128#17
# asm 2: mpy >r2629=$61,<a10=$65,<a1619s22=$19
mpy $61,$65,$19

# qhasm: tmp30 = combine zero  and r36   by comb31
# asm 1: shufb >tmp30=vec128#60,<zero=vec128#4,<r36=vec128#14,<comb31=vec128#74
# asm 2: shufb >tmp30=$62,<zero=$6,<r36=$16,<comb31=$76
shufb $62,$6,$16,$76

# qhasm: int32323232 r1922 += (a11 & 0xffff) * (a811s13  & 0xffff)
# asm 1: mpya >r1922=vec128#26,<a11=vec128#65,<a811s13=vec128#26,<r1922=vec128#49
# asm 2: mpya >r1922=$28,<a11=$67,<a811s13=$28,<r1922=$51
mpya $28,$67,$28,$51

# qhasm: tmp11 = combine r14   and r58   by comb13
# asm 1: shufb >tmp11=vec128#11,<r14=vec128#11,<r58=vec128#44,<comb13=vec128#72
# asm 2: shufb >tmp11=$13,<r14=$13,<r58=$46,<comb13=$74
shufb $13,$13,$46,$74

# qhasm: int32323232 r2326 += (a11 & 0xffff) * (a1215s23 & 0xffff)
# asm 1: mpya >r2326=vec128#49,<a11=vec128#65,<a1215s23=vec128#64,<r2326=vec128#57
# asm 2: mpya >r2326=$51,<a11=$67,<a1215s23=$66,<r2326=$59
mpya $51,$67,$66,$59

# qhasm: tmp21 = combine r25   and r69   by comb22
# asm 1: shufb >tmp21=vec128#12,<r25=vec128#12,<r69=vec128#46,<comb22=vec128#73
# asm 2: shufb >tmp21=$14,<r25=$14,<r69=$48,<comb22=$75
shufb $14,$14,$48,$75

# qhasm: int32323232 r2730  = (a11 & 0xffff) * (a1619s23 & 0xffff)
# asm 1: mpy >r2730=vec128#57,<a11=vec128#65,<a1619s23=vec128#19
# asm 2: mpy >r2730=$59,<a11=$67,<a1619s23=$21
mpy $59,$67,$21

# qhasm: tmp31 = combine r36   and r710  by comb31
# asm 1: shufb >tmp31=vec128#14,<r36=vec128#14,<r710=vec128#48,<comb31=vec128#74
# asm 2: shufb >tmp31=$16,<r36=$16,<r710=$50,<comb31=$76
shufb $16,$16,$50,$76

# qhasm: int32323232 r2427 += (a12 & 0xffff) * (a1215 & 0xffff)
# asm 1: mpya >r2427=vec128#10,<a12=vec128#66,<a1215=vec128#10,<r2427=vec128#51
# asm 2: mpya >r2427=$12,<a12=$68,<a1215=$12,<r2427=$53
mpya $12,$68,$12,$53

# qhasm: tmp12 = combine r58   and r912  by comb13
# asm 1: shufb >tmp12=vec128#44,<r58=vec128#44,<r912=vec128#16,<comb13=vec128#72
# asm 2: shufb >tmp12=$46,<r58=$46,<r912=$18,<comb13=$74
shufb $46,$46,$18,$74

# qhasm: int32323232 r2831  = (a12 & 0xffff) * (a1619s20 & 0xffff)
# asm 1: mpy >r2831=vec128#13,<a12=vec128#66,<a1619s20=vec128#13
# asm 2: mpy >r2831=$15,<a12=$68,<a1619s20=$15
mpy $15,$68,$15

# qhasm: tmp22 = combine r69   and r1013 by comb22
# asm 1: shufb >tmp22=vec128#46,<r69=vec128#46,<r1013=vec128#18,<comb22=vec128#73
# asm 2: shufb >tmp22=$48,<r69=$48,<r1013=$20,<comb22=$75
shufb $48,$48,$20,$75

# qhasm: int32323232 r2528 += (a13 & 0xffff) * (a1215s11 & 0xffff)
# asm 1: mpya >r2528=vec128#28,<a13=vec128#67,<a1215s11=vec128#28,<r2528=vec128#53
# asm 2: mpya >r2528=$30,<a13=$69,<a1215s11=$30,<r2528=$55
mpya $30,$69,$30,$55

# qhasm: tmp32 = combine r710  and r1114 by comb31
# asm 1: shufb >tmp32=vec128#48,<r710=vec128#48,<r1114=vec128#20,<comb31=vec128#74
# asm 2: shufb >tmp32=$50,<r710=$50,<r1114=$22,<comb31=$76
shufb $50,$50,$22,$76

# qhasm: int32323232 r2932  = (a13 & 0xffff) * (a1619s21 & 0xffff)
# asm 1: mpy >r2932=vec128#15,<a13=vec128#67,<a1619s21=vec128#15
# asm 2: mpy >r2932=$17,<a13=$69,<a1619s21=$17
mpy $17,$69,$17

# qhasm: tmp13 = combine r912  and r1316 by comb13
# asm 1: shufb >tmp13=vec128#16,<r912=vec128#16,<r1316=vec128#52,<comb13=vec128#72
# asm 2: shufb >tmp13=$18,<r912=$18,<r1316=$54,<comb13=$74
shufb $18,$18,$54,$74

# qhasm: int32323232 r2629 += (a14 & 0xffff) * (a1215s12 & 0xffff)
# asm 1: mpya >r2629=vec128#30,<a14=vec128#68,<a1215s12=vec128#30,<r2629=vec128#59
# asm 2: mpya >r2629=$32,<a14=$70,<a1215s12=$32,<r2629=$61
mpya $32,$70,$32,$61

# qhasm: tmp23 = combine r1013 and r1417 by comb22
# asm 1: shufb >tmp23=vec128#18,<r1013=vec128#18,<r1417=vec128#54,<comb22=vec128#73
# asm 2: shufb >tmp23=$20,<r1013=$20,<r1417=$56,<comb22=$75
shufb $20,$20,$56,$75

# qhasm: int32323232 r3033  = (a14 & 0xffff) * (a1619s22 & 0xffff)
# asm 1: mpy >r3033=vec128#17,<a14=vec128#68,<a1619s22=vec128#17
# asm 2: mpy >r3033=$19,<a14=$70,<a1619s22=$19
mpy $19,$70,$19

# qhasm: tmp33 = combine r1114 and r1518 by comb31
# asm 1: shufb >tmp33=vec128#20,<r1114=vec128#20,<r1518=vec128#56,<comb31=vec128#74
# asm 2: shufb >tmp33=$22,<r1114=$22,<r1518=$58,<comb31=$76
shufb $22,$22,$58,$76

# qhasm: int32323232 r2730 += (a15 & 0xffff) * (a1215s13 & 0xffff)
# asm 1: mpya >r2730=vec128#32,<a15=vec128#69,<a1215s13=vec128#32,<r2730=vec128#57
# asm 2: mpya >r2730=$34,<a15=$71,<a1215s13=$34,<r2730=$59
mpya $34,$71,$34,$59

# qhasm: tmp14 = combine r1316 and r1720 by comb13
# asm 1: shufb >tmp14=vec128#51,<r1316=vec128#52,<r1720=vec128#22,<comb13=vec128#72
# asm 2: shufb >tmp14=$53,<r1316=$54,<r1720=$24,<comb13=$74
shufb $53,$54,$24,$74

# qhasm: int32323232 r3134  = (a15 & 0xffff) * (a1619s23 & 0xffff)
# asm 1: mpy >r3134=vec128#19,<a15=vec128#69,<a1619s23=vec128#19
# asm 2: mpy >r3134=$21,<a15=$71,<a1619s23=$21
mpy $21,$71,$21

# qhasm: tmp24 = combine r1417 and r1821 by comb22
# asm 1: shufb >tmp24=vec128#52,<r1417=vec128#54,<r1821=vec128#24,<comb22=vec128#73
# asm 2: shufb >tmp24=$54,<r1417=$56,<r1821=$26,<comb22=$75
shufb $54,$56,$26,$75

# qhasm: int32323232 r3235  = (a16 & 0xffff) * (a1619 & 0xffff)
# asm 1: mpy >r3235=vec128#2,<a16=vec128#21,<a1619=vec128#2
# asm 2: mpy >r3235=$4,<a16=$23,<a1619=$4
mpy $4,$23,$4

# qhasm: tmp34 = combine r1518 and r1922 by comb31
# asm 1: shufb >tmp34=vec128#21,<r1518=vec128#56,<r1922=vec128#26,<comb31=vec128#74
# asm 2: shufb >tmp34=$23,<r1518=$58,<r1922=$28,<comb31=$76
shufb $23,$58,$28,$76

# qhasm: int32323232 r3336  = (a17 & 0xffff) * (a1619s11 & 0xffff)
# asm 1: mpy >r3336=vec128#5,<a17=vec128#23,<a1619s11=vec128#5
# asm 2: mpy >r3336=$7,<a17=$25,<a1619s11=$7
mpy $7,$25,$7

# qhasm: tmp15 = combine r1720 and r2124 by comb13
# asm 1: shufb >tmp15=vec128#22,<r1720=vec128#22,<r2124=vec128#36,<comb13=vec128#72
# asm 2: shufb >tmp15=$24,<r1720=$24,<r2124=$38,<comb13=$74
shufb $24,$24,$38,$74

# qhasm: int32323232 r3437  = (a18 & 0xffff) * (a1619s12 & 0xffff)
# asm 1: mpy >r3437=vec128#6,<a18=vec128#25,<a1619s12=vec128#6
# asm 2: mpy >r3437=$8,<a18=$27,<a1619s12=$8
mpy $8,$27,$8

# qhasm: tmp25 = combine r1821 and r2225 by comb22
# asm 1: shufb >tmp25=vec128#23,<r1821=vec128#24,<r2225=vec128#55,<comb22=vec128#73
# asm 2: shufb >tmp25=$25,<r1821=$26,<r2225=$57,<comb22=$75
shufb $25,$26,$57,$75

# qhasm: int32323232 r3538  = (a19 & 0xffff) * (a1619s13 & 0xffff)
# asm 1: mpy >r3538=vec128#7,<a19=vec128#27,<a1619s13=vec128#7
# asm 2: mpy >r3538=$9,<a19=$29,<a1619s13=$9
mpy $9,$29,$9

# qhasm: tmp35 = combine r1922 and r2326 by comb31
# asm 1: shufb >tmp35=vec128#24,<r1922=vec128#26,<r2326=vec128#49,<comb31=vec128#74
# asm 2: shufb >tmp35=$26,<r1922=$28,<r2326=$51,<comb31=$76
shufb $26,$28,$51,$76

# qhasm: int32323232 r03   += tmp10
# asm 1: a <r03=vec128#3,<r03=vec128#3,<tmp10=vec128#47
# asm 2: a <r03=$5,<r03=$5,<tmp10=$49
a $5,$5,$49

# qhasm: tmp16 = combine r2124 and r2528 by comb13
# asm 1: shufb >tmp16=vec128#25,<r2124=vec128#36,<r2528=vec128#28,<comb13=vec128#72
# asm 2: shufb >tmp16=$27,<r2124=$38,<r2528=$30,<comb13=$74
shufb $27,$38,$30,$74

# qhasm: int32323232 r47   += tmp11
# asm 1: a <r47=vec128#43,<r47=vec128#43,<tmp11=vec128#11
# asm 2: a <r47=$45,<r47=$45,<tmp11=$13
a $45,$45,$13

# qhasm: tmp17 = combine r2528 and r2932 by comb13
# asm 1: shufb >tmp17=vec128#11,<r2528=vec128#28,<r2932=vec128#15,<comb13=vec128#72
# asm 2: shufb >tmp17=$13,<r2528=$30,<r2932=$17,<comb13=$74
shufb $13,$30,$17,$74

# qhasm: int32323232 r03   += tmp20
# asm 1: a <r03=vec128#3,<r03=vec128#3,<tmp20=vec128#58
# asm 2: a <r03=$5,<r03=$5,<tmp20=$60
a $5,$5,$60

# qhasm: tmp26 = combine r2225 and r2629 by comb22
# asm 1: shufb >tmp26=vec128#26,<r2225=vec128#55,<r2629=vec128#30,<comb22=vec128#73
# asm 2: shufb >tmp26=$28,<r2225=$57,<r2629=$32,<comb22=$75
shufb $28,$57,$32,$75

# qhasm: int32323232 r47   += tmp21
# asm 1: a <r47=vec128#43,<r47=vec128#43,<tmp21=vec128#12
# asm 2: a <r47=$45,<r47=$45,<tmp21=$14
a $45,$45,$14

# qhasm: tmp27 = combine r2629 and r3033 by comb22
# asm 1: shufb >tmp27=vec128#12,<r2629=vec128#30,<r3033=vec128#17,<comb22=vec128#73
# asm 2: shufb >tmp27=$14,<r2629=$32,<r3033=$19,<comb22=$75
shufb $14,$32,$19,$75

# qhasm: int32323232 r03   += tmp30
# asm 1: a <r03=vec128#3,<r03=vec128#3,<tmp30=vec128#60
# asm 2: a <r03=$5,<r03=$5,<tmp30=$62
a $5,$5,$62

# qhasm: tmp36 = combine r2326 and r2730 by comb31
# asm 1: shufb >tmp36=vec128#27,<r2326=vec128#49,<r2730=vec128#32,<comb31=vec128#74
# asm 2: shufb >tmp36=$29,<r2326=$51,<r2730=$34,<comb31=$76
shufb $29,$51,$34,$76

# qhasm: int32323232 r47   += tmp31
# asm 1: a <r47=vec128#43,<r47=vec128#43,<tmp31=vec128#14
# asm 2: a <r47=$45,<r47=$45,<tmp31=$16
a $45,$45,$16

# qhasm: tmp37 = combine r2730 and r3134 by comb31
# asm 1: shufb >tmp37=vec128#14,<r2730=vec128#32,<r3134=vec128#19,<comb31=vec128#74
# asm 2: shufb >tmp37=$16,<r2730=$34,<r3134=$21,<comb31=$76
shufb $16,$34,$21,$76

# qhasm: int32323232 r811  += tmp12
# asm 1: a <r811=vec128#8,<r811=vec128#8,<tmp12=vec128#44
# asm 2: a <r811=$10,<r811=$10,<tmp12=$46
a $10,$10,$46

# qhasm: tmp18 = combine r2932 and r3336 by comb13
# asm 1: shufb >tmp18=vec128#15,<r2932=vec128#15,<r3336=vec128#5,<comb13=vec128#72
# asm 2: shufb >tmp18=$17,<r2932=$17,<r3336=$7,<comb13=$74
shufb $17,$17,$7,$74

# qhasm: int32323232 r1215 += tmp13
# asm 1: a <r1215=vec128#50,<r1215=vec128#50,<tmp13=vec128#16
# asm 2: a <r1215=$52,<r1215=$52,<tmp13=$18
a $52,$52,$18

# qhasm: r3639 = combine r3336 and zero  by comb13
# asm 1: shufb >r3639=vec128#5,<r3336=vec128#5,<zero=vec128#4,<comb13=vec128#72
# asm 2: shufb >r3639=$7,<r3336=$7,<zero=$6,<comb13=$74
shufb $7,$7,$6,$74

# qhasm: int32323232 r811  += tmp22
# asm 1: a <r811=vec128#8,<r811=vec128#8,<tmp22=vec128#46
# asm 2: a <r811=$10,<r811=$10,<tmp22=$48
a $10,$10,$48

# qhasm: tmp28 = combine r3033 and r3437 by comb22
# asm 1: shufb >tmp28=vec128#16,<r3033=vec128#17,<r3437=vec128#6,<comb22=vec128#73
# asm 2: shufb >tmp28=$18,<r3033=$19,<r3437=$8,<comb22=$75
shufb $18,$19,$8,$75

# qhasm: int32323232 r1215 += tmp23
# asm 1: a <r1215=vec128#50,<r1215=vec128#50,<tmp23=vec128#18
# asm 2: a <r1215=$52,<r1215=$52,<tmp23=$20
a $52,$52,$20

# qhasm: tmp29 = combine r3437 and zero  by comb22
# asm 1: shufb >tmp29=vec128#6,<r3437=vec128#6,<zero=vec128#4,<comb22=vec128#73
# asm 2: shufb >tmp29=$8,<r3437=$8,<zero=$6,<comb22=$75
shufb $8,$8,$6,$75

# qhasm: int32323232 r811  += tmp32
# asm 1: a <r811=vec128#8,<r811=vec128#8,<tmp32=vec128#48
# asm 2: a <r811=$10,<r811=$10,<tmp32=$50
a $10,$10,$50

# qhasm: tmp38 = combine r3134 and r3538 by comb31
# asm 1: shufb >tmp38=vec128#17,<r3134=vec128#19,<r3538=vec128#7,<comb31=vec128#74
# asm 2: shufb >tmp38=$19,<r3134=$21,<r3538=$9,<comb31=$76
shufb $19,$21,$9,$76

# qhasm: int32323232 r1215 += tmp33
# asm 1: a <r1215=vec128#50,<r1215=vec128#50,<tmp33=vec128#20
# asm 2: a <r1215=$52,<r1215=$52,<tmp33=$22
a $52,$52,$22

# qhasm: tmp39 = combine r3538 and zero  by comb31
# asm 1: shufb >tmp39=vec128#4,<r3538=vec128#7,<zero=vec128#4,<comb31=vec128#74
# asm 2: shufb >tmp39=$6,<r3538=$9,<zero=$6,<comb31=$76
shufb $6,$9,$6,$76

# qhasm: int32323232 r1619 += tmp14
# asm 1: a <r1619=vec128#9,<r1619=vec128#9,<tmp14=vec128#51
# asm 2: a <r1619=$11,<r1619=$11,<tmp14=$53
a $11,$11,$53

# qhasm: int32323232 r2023 += tmp15
# asm 1: a <r2023=vec128#45,<r2023=vec128#45,<tmp15=vec128#22
# asm 2: a <r2023=$47,<r2023=$47,<tmp15=$24
a $47,$47,$24

# qhasm: int32323232 r1619 += tmp24
# asm 1: a <r1619=vec128#9,<r1619=vec128#9,<tmp24=vec128#52
# asm 2: a <r1619=$11,<r1619=$11,<tmp24=$54
a $11,$11,$54

# qhasm: int32323232 r2023 += tmp25
# asm 1: a <r2023=vec128#45,<r2023=vec128#45,<tmp25=vec128#23
# asm 2: a <r2023=$47,<r2023=$47,<tmp25=$25
a $47,$47,$25

# qhasm: int32323232 r1619 += tmp34
# asm 1: a <r1619=vec128#9,<r1619=vec128#9,<tmp34=vec128#21
# asm 2: a <r1619=$11,<r1619=$11,<tmp34=$23
a $11,$11,$23

# qhasm: int32323232 r2023 += tmp35
# asm 1: a <r2023=vec128#45,<r2023=vec128#45,<tmp35=vec128#24
# asm 2: a <r2023=$47,<r2023=$47,<tmp35=$26
a $47,$47,$26

# qhasm: int32323232 r2427 += tmp16
# asm 1: a <r2427=vec128#10,<r2427=vec128#10,<tmp16=vec128#25
# asm 2: a <r2427=$12,<r2427=$12,<tmp16=$27
a $12,$12,$27

# qhasm: int32323232 r2831 += tmp17
# asm 1: a <r2831=vec128#13,<r2831=vec128#13,<tmp17=vec128#11
# asm 2: a <r2831=$15,<r2831=$15,<tmp17=$13
a $15,$15,$13

# qhasm: int32323232 r2427 += tmp26
# asm 1: a <r2427=vec128#10,<r2427=vec128#10,<tmp26=vec128#26
# asm 2: a <r2427=$12,<r2427=$12,<tmp26=$28
a $12,$12,$28

# qhasm: int32323232 r2831 += tmp27
# asm 1: a <r2831=vec128#13,<r2831=vec128#13,<tmp27=vec128#12
# asm 2: a <r2831=$15,<r2831=$15,<tmp27=$14
a $15,$15,$14

# qhasm: int32323232 r2427 += tmp36
# asm 1: a <r2427=vec128#10,<r2427=vec128#10,<tmp36=vec128#27
# asm 2: a <r2427=$12,<r2427=$12,<tmp36=$29
a $12,$12,$29

# qhasm: int32323232 r2831 += tmp37
# asm 1: a <r2831=vec128#13,<r2831=vec128#13,<tmp37=vec128#14
# asm 2: a <r2831=$15,<r2831=$15,<tmp37=$16
a $15,$15,$16

# qhasm: int32323232 r3235 += tmp18
# asm 1: a <r3235=vec128#2,<r3235=vec128#2,<tmp18=vec128#15
# asm 2: a <r3235=$4,<r3235=$4,<tmp18=$17
a $4,$4,$17

# qhasm: int32323232 r3639 += tmp29
# asm 1: a <r3639=vec128#5,<r3639=vec128#5,<tmp29=vec128#6
# asm 2: a <r3639=$7,<r3639=$7,<tmp29=$8
a $7,$7,$8

# qhasm: int32323232 r3235 += tmp28
# asm 1: a <r3235=vec128#2,<r3235=vec128#2,<tmp28=vec128#16
# asm 2: a <r3235=$4,<r3235=$4,<tmp28=$18
a $4,$4,$18

# qhasm: int32323232 r3639 += tmp39
# asm 1: a <r3639=vec128#5,<r3639=vec128#5,<tmp39=vec128#4
# asm 2: a <r3639=$7,<r3639=$7,<tmp39=$6
a $7,$7,$6

# qhasm: int32323232 r3235 += tmp38
# asm 1: a <r3235=vec128#2,<r3235=vec128#2,<tmp38=vec128#17
# asm 2: a <r3235=$4,<r3235=$4,<tmp38=$19
a $4,$4,$19

# qhasm: carry0 = select bytes from r2023 by sel01
# asm 1: shufb >carry0=vec128#4,<r2023=vec128#45,<r2023=vec128#45,<sel01=vec128#29
# asm 2: shufb >carry0=$6,<r2023=$47,<r2023=$47,<sel01=$31
shufb $6,$47,$47,$31

# qhasm: carry1 = select bytes from r2427 by sel01
# asm 1: shufb >carry1=vec128#6,<r2427=vec128#10,<r2427=vec128#10,<sel01=vec128#29
# asm 2: shufb >carry1=$8,<r2427=$12,<r2427=$12,<sel01=$31
shufb $8,$12,$12,$31

# qhasm: carry2 = select bytes from r2831 by sel01
# asm 1: shufb >carry2=vec128#7,<r2831=vec128#13,<r2831=vec128#13,<sel01=vec128#29
# asm 2: shufb >carry2=$9,<r2831=$15,<r2831=$15,<sel01=$31
shufb $9,$15,$15,$31

# qhasm: carry3 = select bytes from r3235 by sel01
# asm 1: shufb >carry3=vec128#11,<r3235=vec128#2,<r3235=vec128#2,<sel01=vec128#29
# asm 2: shufb >carry3=$13,<r3235=$4,<r3235=$4,<sel01=$31
shufb $13,$4,$4,$31

# qhasm: uint32323232 carry0 >>= 13
# asm 1: rotmi <carry0=vec128#4,<carry0=vec128#4,-13
# asm 2: rotmi <carry0=$6,<carry0=$6,-13
rotmi $6,$6,-13

# qhasm: uint32323232 carry1 >>= 13
# asm 1: rotmi <carry1=vec128#6,<carry1=vec128#6,-13
# asm 2: rotmi <carry1=$8,<carry1=$8,-13
rotmi $8,$8,-13

# qhasm: uint32323232 carry2 >>= 13
# asm 1: rotmi <carry2=vec128#7,<carry2=vec128#7,-13
# asm 2: rotmi <carry2=$9,<carry2=$9,-13
rotmi $9,$9,-13

# qhasm: uint32323232 carry3 >>= 13
# asm 1: rotmi <carry3=vec128#11,<carry3=vec128#11,-13
# asm 2: rotmi <carry3=$13,<carry3=$13,-13
rotmi $13,$13,-13

# qhasm: int32323232 r2023 += carry0
# asm 1: a <r2023=vec128#45,<r2023=vec128#45,<carry0=vec128#4
# asm 2: a <r2023=$47,<r2023=$47,<carry0=$6
a $47,$47,$6

# qhasm: int32323232 r2427 += carry1
# asm 1: a <r2427=vec128#10,<r2427=vec128#10,<carry1=vec128#6
# asm 2: a <r2427=$12,<r2427=$12,<carry1=$8
a $12,$12,$8

# qhasm: int32323232 r2831 += carry2
# asm 1: a <r2831=vec128#13,<r2831=vec128#13,<carry2=vec128#7
# asm 2: a <r2831=$15,<r2831=$15,<carry2=$9
a $15,$15,$9

# qhasm: int32323232 r3235 += carry3
# asm 1: a <r3235=vec128#2,<r3235=vec128#2,<carry3=vec128#11
# asm 2: a <r3235=$4,<r3235=$4,<carry3=$13
a $4,$4,$13

# qhasm: carry0 = select bytes from r2023 by sel12
# asm 1: shufb >carry0=vec128#4,<r2023=vec128#45,<r2023=vec128#45,<sel12=vec128#31
# asm 2: shufb >carry0=$6,<r2023=$47,<r2023=$47,<sel12=$33
shufb $6,$47,$47,$33

# qhasm: carry1 = select bytes from r2427 by sel12
# asm 1: shufb >carry1=vec128#6,<r2427=vec128#10,<r2427=vec128#10,<sel12=vec128#31
# asm 2: shufb >carry1=$8,<r2427=$12,<r2427=$12,<sel12=$33
shufb $8,$12,$12,$33

# qhasm: carry2 = select bytes from r2831 by sel12
# asm 1: shufb >carry2=vec128#7,<r2831=vec128#13,<r2831=vec128#13,<sel12=vec128#31
# asm 2: shufb >carry2=$9,<r2831=$15,<r2831=$15,<sel12=$33
shufb $9,$15,$15,$33

# qhasm: carry3 = select bytes from r3235 by sel12
# asm 1: shufb >carry3=vec128#11,<r3235=vec128#2,<r3235=vec128#2,<sel12=vec128#31
# asm 2: shufb >carry3=$13,<r3235=$4,<r3235=$4,<sel12=$33
shufb $13,$4,$4,$33

# qhasm: uint32323232 carry0 >>= 13
# asm 1: rotmi <carry0=vec128#4,<carry0=vec128#4,-13
# asm 2: rotmi <carry0=$6,<carry0=$6,-13
rotmi $6,$6,-13

# qhasm: uint32323232 carry1 >>= 13
# asm 1: rotmi <carry1=vec128#6,<carry1=vec128#6,-13
# asm 2: rotmi <carry1=$8,<carry1=$8,-13
rotmi $8,$8,-13

# qhasm: uint32323232 carry2 >>= 13
# asm 1: rotmi <carry2=vec128#7,<carry2=vec128#7,-13
# asm 2: rotmi <carry2=$9,<carry2=$9,-13
rotmi $9,$9,-13

# qhasm: uint32323232 carry3 >>= 13
# asm 1: rotmi <carry3=vec128#11,<carry3=vec128#11,-13
# asm 2: rotmi <carry3=$13,<carry3=$13,-13
rotmi $13,$13,-13

# qhasm: int32323232 r2023 += carry0
# asm 1: a <r2023=vec128#45,<r2023=vec128#45,<carry0=vec128#4
# asm 2: a <r2023=$47,<r2023=$47,<carry0=$6
a $47,$47,$6

# qhasm: int32323232 r2427 += carry1
# asm 1: a <r2427=vec128#10,<r2427=vec128#10,<carry1=vec128#6
# asm 2: a <r2427=$12,<r2427=$12,<carry1=$8
a $12,$12,$8

# qhasm: int32323232 r2831 += carry2
# asm 1: a <r2831=vec128#13,<r2831=vec128#13,<carry2=vec128#7
# asm 2: a <r2831=$15,<r2831=$15,<carry2=$9
a $15,$15,$9

# qhasm: int32323232 r3235 += carry3
# asm 1: a <r3235=vec128#2,<r3235=vec128#2,<carry3=vec128#11
# asm 2: a <r3235=$4,<r3235=$4,<carry3=$13
a $4,$4,$13

# qhasm: carry0 = select bytes from r2023 by sel23
# asm 1: shufb >carry0=vec128#4,<r2023=vec128#45,<r2023=vec128#45,<sel23=vec128#33
# asm 2: shufb >carry0=$6,<r2023=$47,<r2023=$47,<sel23=$35
shufb $6,$47,$47,$35

# qhasm: carry1 = select bytes from r2427 by sel23
# asm 1: shufb >carry1=vec128#6,<r2427=vec128#10,<r2427=vec128#10,<sel23=vec128#33
# asm 2: shufb >carry1=$8,<r2427=$12,<r2427=$12,<sel23=$35
shufb $8,$12,$12,$35

# qhasm: carry2 = select bytes from r2831 by sel23
# asm 1: shufb >carry2=vec128#7,<r2831=vec128#13,<r2831=vec128#13,<sel23=vec128#33
# asm 2: shufb >carry2=$9,<r2831=$15,<r2831=$15,<sel23=$35
shufb $9,$15,$15,$35

# qhasm: carry3 = select bytes from r3235 by sel23
# asm 1: shufb >carry3=vec128#11,<r3235=vec128#2,<r3235=vec128#2,<sel23=vec128#33
# asm 2: shufb >carry3=$13,<r3235=$4,<r3235=$4,<sel23=$35
shufb $13,$4,$4,$35

# qhasm: uint32323232 carry0 >>= 13
# asm 1: rotmi <carry0=vec128#4,<carry0=vec128#4,-13
# asm 2: rotmi <carry0=$6,<carry0=$6,-13
rotmi $6,$6,-13

# qhasm: uint32323232 carry1 >>= 13
# asm 1: rotmi <carry1=vec128#6,<carry1=vec128#6,-13
# asm 2: rotmi <carry1=$8,<carry1=$8,-13
rotmi $8,$8,-13

# qhasm: uint32323232 carry2 >>= 13
# asm 1: rotmi <carry2=vec128#7,<carry2=vec128#7,-13
# asm 2: rotmi <carry2=$9,<carry2=$9,-13
rotmi $9,$9,-13

# qhasm: uint32323232 carry3 >>= 13
# asm 1: rotmi <carry3=vec128#11,<carry3=vec128#11,-13
# asm 2: rotmi <carry3=$13,<carry3=$13,-13
rotmi $13,$13,-13

# qhasm: int32323232 r2023 += carry0
# asm 1: a <r2023=vec128#45,<r2023=vec128#45,<carry0=vec128#4
# asm 2: a <r2023=$47,<r2023=$47,<carry0=$6
a $47,$47,$6

# qhasm: int32323232 r2427 += carry1
# asm 1: a <r2427=vec128#10,<r2427=vec128#10,<carry1=vec128#6
# asm 2: a <r2427=$12,<r2427=$12,<carry1=$8
a $12,$12,$8

# qhasm: int32323232 r2831 += carry2
# asm 1: a <r2831=vec128#13,<r2831=vec128#13,<carry2=vec128#7
# asm 2: a <r2831=$15,<r2831=$15,<carry2=$9
a $15,$15,$9

# qhasm: int32323232 r3235 += carry3
# asm 1: a <r3235=vec128#2,<r3235=vec128#2,<carry3=vec128#11
# asm 2: a <r3235=$4,<r3235=$4,<carry3=$13
a $4,$4,$13

# qhasm: carry0 = select bytes from r2023 by sel30
# asm 1: shufb >carry0=vec128#4,<r2023=vec128#45,<r2023=vec128#45,<sel30=vec128#34
# asm 2: shufb >carry0=$6,<r2023=$47,<r2023=$47,<sel30=$36
shufb $6,$47,$47,$36

# qhasm: carry1 = select bytes from r2427 by sel30
# asm 1: shufb >carry1=vec128#6,<r2427=vec128#10,<r2427=vec128#10,<sel30=vec128#34
# asm 2: shufb >carry1=$8,<r2427=$12,<r2427=$12,<sel30=$36
shufb $8,$12,$12,$36

# qhasm: carry2 = select bytes from r2831 by sel30
# asm 1: shufb >carry2=vec128#7,<r2831=vec128#13,<r2831=vec128#13,<sel30=vec128#34
# asm 2: shufb >carry2=$9,<r2831=$15,<r2831=$15,<sel30=$36
shufb $9,$15,$15,$36

# qhasm: carry3 = select bytes from r3235 by sel30
# asm 1: shufb >carry3=vec128#11,<r3235=vec128#2,<r3235=vec128#2,<sel30=vec128#34
# asm 2: shufb >carry3=$13,<r3235=$4,<r3235=$4,<sel30=$36
shufb $13,$4,$4,$36

# qhasm: uint32323232 carry0 >>= 12
# asm 1: rotmi <carry0=vec128#4,<carry0=vec128#4,-12
# asm 2: rotmi <carry0=$6,<carry0=$6,-12
rotmi $6,$6,-12

# qhasm: uint32323232 carry1 >>= 12
# asm 1: rotmi <carry1=vec128#6,<carry1=vec128#6,-12
# asm 2: rotmi <carry1=$8,<carry1=$8,-12
rotmi $8,$8,-12

# qhasm: uint32323232 carry2 >>= 12
# asm 1: rotmi <carry2=vec128#7,<carry2=vec128#7,-12
# asm 2: rotmi <carry2=$9,<carry2=$9,-12
rotmi $9,$9,-12

# qhasm: uint32323232 carry3 >>= 12
# asm 1: rotmi <carry3=vec128#11,<carry3=vec128#11,-12
# asm 2: rotmi <carry3=$13,<carry3=$13,-12
rotmi $13,$13,-12

# qhasm: r2023 &= redcoeffmask
# asm 1: and <r2023=vec128#45,<r2023=vec128#45,<redcoeffmask=vec128#35
# asm 2: and <r2023=$47,<r2023=$47,<redcoeffmask=$37
and $47,$47,$37

# qhasm: r2427 &= redcoeffmask
# asm 1: and <r2427=vec128#10,<r2427=vec128#10,<redcoeffmask=vec128#35
# asm 2: and <r2427=$12,<r2427=$12,<redcoeffmask=$37
and $12,$12,$37

# qhasm: r2831 &= redcoeffmask
# asm 1: and <r2831=vec128#13,<r2831=vec128#13,<redcoeffmask=vec128#35
# asm 2: and <r2831=$15,<r2831=$15,<redcoeffmask=$37
and $15,$15,$37

# qhasm: r3235 &= redcoeffmask
# asm 1: and <r3235=vec128#2,<r3235=vec128#2,<redcoeffmask=vec128#35
# asm 2: and <r3235=$4,<r3235=$4,<redcoeffmask=$37
and $4,$4,$37

# qhasm: int32323232 r2427 += carry0
# asm 1: a <r2427=vec128#10,<r2427=vec128#10,<carry0=vec128#4
# asm 2: a <r2427=$12,<r2427=$12,<carry0=$6
a $12,$12,$6

# qhasm: int32323232 r2831 += carry1
# asm 1: a <r2831=vec128#13,<r2831=vec128#13,<carry1=vec128#6
# asm 2: a <r2831=$15,<r2831=$15,<carry1=$8
a $15,$15,$8

# qhasm: int32323232 r3235 += carry2
# asm 1: a <r3235=vec128#2,<r3235=vec128#2,<carry2=vec128#7
# asm 2: a <r3235=$4,<r3235=$4,<carry2=$9
a $4,$4,$9

# qhasm: int32323232 r3639 += carry3
# asm 1: a <r3639=vec128#5,<r3639=vec128#5,<carry3=vec128#11
# asm 2: a <r3639=$7,<r3639=$7,<carry3=$13
a $7,$7,$13

# qhasm: carry1 = select bytes from r2427 by sel01
# asm 1: shufb >carry1=vec128#4,<r2427=vec128#10,<r2427=vec128#10,<sel01=vec128#29
# asm 2: shufb >carry1=$6,<r2427=$12,<r2427=$12,<sel01=$31
shufb $6,$12,$12,$31

# qhasm: carry2 = select bytes from r2831 by sel01
# asm 1: shufb >carry2=vec128#6,<r2831=vec128#13,<r2831=vec128#13,<sel01=vec128#29
# asm 2: shufb >carry2=$8,<r2831=$15,<r2831=$15,<sel01=$31
shufb $8,$15,$15,$31

# qhasm: carry3 = select bytes from r3235 by sel01
# asm 1: shufb >carry3=vec128#7,<r3235=vec128#2,<r3235=vec128#2,<sel01=vec128#29
# asm 2: shufb >carry3=$9,<r3235=$4,<r3235=$4,<sel01=$31
shufb $9,$4,$4,$31

# qhasm: carry4 = select bytes from r3639 by sel01
# asm 1: shufb >carry4=vec128#11,<r3639=vec128#5,<r3639=vec128#5,<sel01=vec128#29
# asm 2: shufb >carry4=$13,<r3639=$7,<r3639=$7,<sel01=$31
shufb $13,$7,$7,$31

# qhasm: uint32323232 carry1 >>= 13
# asm 1: rotmi <carry1=vec128#4,<carry1=vec128#4,-13
# asm 2: rotmi <carry1=$6,<carry1=$6,-13
rotmi $6,$6,-13

# qhasm: uint32323232 carry2 >>= 13
# asm 1: rotmi <carry2=vec128#6,<carry2=vec128#6,-13
# asm 2: rotmi <carry2=$8,<carry2=$8,-13
rotmi $8,$8,-13

# qhasm: uint32323232 carry3 >>= 13
# asm 1: rotmi <carry3=vec128#7,<carry3=vec128#7,-13
# asm 2: rotmi <carry3=$9,<carry3=$9,-13
rotmi $9,$9,-13

# qhasm: uint32323232 carry4 >>= 13
# asm 1: rotmi <carry4=vec128#11,<carry4=vec128#11,-13
# asm 2: rotmi <carry4=$13,<carry4=$13,-13
rotmi $13,$13,-13

# qhasm: int32323232 r2427 += carry1
# asm 1: a <r2427=vec128#10,<r2427=vec128#10,<carry1=vec128#4
# asm 2: a <r2427=$12,<r2427=$12,<carry1=$6
a $12,$12,$6

# qhasm: int32323232 r2831 += carry2
# asm 1: a <r2831=vec128#13,<r2831=vec128#13,<carry2=vec128#6
# asm 2: a <r2831=$15,<r2831=$15,<carry2=$8
a $15,$15,$8

# qhasm: int32323232 r3235 += carry3
# asm 1: a <r3235=vec128#2,<r3235=vec128#2,<carry3=vec128#7
# asm 2: a <r3235=$4,<r3235=$4,<carry3=$9
a $4,$4,$9

# qhasm: int32323232 r3639 += carry4
# asm 1: a <r3639=vec128#5,<r3639=vec128#5,<carry4=vec128#11
# asm 2: a <r3639=$7,<r3639=$7,<carry4=$13
a $7,$7,$13

# qhasm: carry1 = select bytes from r2427 by sel12
# asm 1: shufb >carry1=vec128#4,<r2427=vec128#10,<r2427=vec128#10,<sel12=vec128#31
# asm 2: shufb >carry1=$6,<r2427=$12,<r2427=$12,<sel12=$33
shufb $6,$12,$12,$33

# qhasm: carry2 = select bytes from r2831 by sel12
# asm 1: shufb >carry2=vec128#6,<r2831=vec128#13,<r2831=vec128#13,<sel12=vec128#31
# asm 2: shufb >carry2=$8,<r2831=$15,<r2831=$15,<sel12=$33
shufb $8,$15,$15,$33

# qhasm: carry3 = select bytes from r3235 by sel12
# asm 1: shufb >carry3=vec128#7,<r3235=vec128#2,<r3235=vec128#2,<sel12=vec128#31
# asm 2: shufb >carry3=$9,<r3235=$4,<r3235=$4,<sel12=$33
shufb $9,$4,$4,$33

# qhasm: carry4 = select bytes from r3639 by sel12
# asm 1: shufb >carry4=vec128#11,<r3639=vec128#5,<r3639=vec128#5,<sel12=vec128#31
# asm 2: shufb >carry4=$13,<r3639=$7,<r3639=$7,<sel12=$33
shufb $13,$7,$7,$33

# qhasm: uint32323232 carry1 >>= 13
# asm 1: rotmi <carry1=vec128#4,<carry1=vec128#4,-13
# asm 2: rotmi <carry1=$6,<carry1=$6,-13
rotmi $6,$6,-13

# qhasm: uint32323232 carry2 >>= 13
# asm 1: rotmi <carry2=vec128#6,<carry2=vec128#6,-13
# asm 2: rotmi <carry2=$8,<carry2=$8,-13
rotmi $8,$8,-13

# qhasm: uint32323232 carry3 >>= 13
# asm 1: rotmi <carry3=vec128#7,<carry3=vec128#7,-13
# asm 2: rotmi <carry3=$9,<carry3=$9,-13
rotmi $9,$9,-13

# qhasm: uint32323232 carry4 >>= 13
# asm 1: rotmi <carry4=vec128#11,<carry4=vec128#11,-13
# asm 2: rotmi <carry4=$13,<carry4=$13,-13
rotmi $13,$13,-13

# qhasm: int32323232 r2427 += carry1
# asm 1: a <r2427=vec128#10,<r2427=vec128#10,<carry1=vec128#4
# asm 2: a <r2427=$12,<r2427=$12,<carry1=$6
a $12,$12,$6

# qhasm: int32323232 r2831 += carry2
# asm 1: a <r2831=vec128#13,<r2831=vec128#13,<carry2=vec128#6
# asm 2: a <r2831=$15,<r2831=$15,<carry2=$8
a $15,$15,$8

# qhasm: int32323232 r3235 += carry3
# asm 1: a <r3235=vec128#2,<r3235=vec128#2,<carry3=vec128#7
# asm 2: a <r3235=$4,<r3235=$4,<carry3=$9
a $4,$4,$9

# qhasm: int32323232 r3639 += carry4
# asm 1: a <r3639=vec128#5,<r3639=vec128#5,<carry4=vec128#11
# asm 2: a <r3639=$7,<r3639=$7,<carry4=$13
a $7,$7,$13

# qhasm: carry1 = select bytes from r2427 by sel23
# asm 1: shufb >carry1=vec128#4,<r2427=vec128#10,<r2427=vec128#10,<sel23=vec128#33
# asm 2: shufb >carry1=$6,<r2427=$12,<r2427=$12,<sel23=$35
shufb $6,$12,$12,$35

# qhasm: carry2 = select bytes from r2831 by sel23
# asm 1: shufb >carry2=vec128#6,<r2831=vec128#13,<r2831=vec128#13,<sel23=vec128#33
# asm 2: shufb >carry2=$8,<r2831=$15,<r2831=$15,<sel23=$35
shufb $8,$15,$15,$35

# qhasm: carry3 = select bytes from r3235 by sel23
# asm 1: shufb >carry3=vec128#7,<r3235=vec128#2,<r3235=vec128#2,<sel23=vec128#33
# asm 2: shufb >carry3=$9,<r3235=$4,<r3235=$4,<sel23=$35
shufb $9,$4,$4,$35

# qhasm: carry4 = select bytes from r3639 by sel23
# asm 1: shufb >carry4=vec128#11,<r3639=vec128#5,<r3639=vec128#5,<sel23=vec128#33
# asm 2: shufb >carry4=$13,<r3639=$7,<r3639=$7,<sel23=$35
shufb $13,$7,$7,$35

# qhasm: uint32323232 carry1 >>= 13
# asm 1: rotmi <carry1=vec128#4,<carry1=vec128#4,-13
# asm 2: rotmi <carry1=$6,<carry1=$6,-13
rotmi $6,$6,-13

# qhasm: uint32323232 carry2 >>= 13
# asm 1: rotmi <carry2=vec128#6,<carry2=vec128#6,-13
# asm 2: rotmi <carry2=$8,<carry2=$8,-13
rotmi $8,$8,-13

# qhasm: uint32323232 carry3 >>= 13
# asm 1: rotmi <carry3=vec128#7,<carry3=vec128#7,-13
# asm 2: rotmi <carry3=$9,<carry3=$9,-13
rotmi $9,$9,-13

# qhasm: uint32323232 carry4 >>= 13
# asm 1: rotmi <carry4=vec128#11,<carry4=vec128#11,-13
# asm 2: rotmi <carry4=$13,<carry4=$13,-13
rotmi $13,$13,-13

# qhasm: int32323232 r2427 += carry1
# asm 1: a <r2427=vec128#10,<r2427=vec128#10,<carry1=vec128#4
# asm 2: a <r2427=$12,<r2427=$12,<carry1=$6
a $12,$12,$6

# qhasm: int32323232 r2831 += carry2
# asm 1: a <r2831=vec128#13,<r2831=vec128#13,<carry2=vec128#6
# asm 2: a <r2831=$15,<r2831=$15,<carry2=$8
a $15,$15,$8

# qhasm: int32323232 r3235 += carry3
# asm 1: a <r3235=vec128#2,<r3235=vec128#2,<carry3=vec128#7
# asm 2: a <r3235=$4,<r3235=$4,<carry3=$9
a $4,$4,$9

# qhasm: int32323232 r3639 += carry4
# asm 1: a <r3639=vec128#5,<r3639=vec128#5,<carry4=vec128#11
# asm 2: a <r3639=$7,<r3639=$7,<carry4=$13
a $7,$7,$13

# qhasm: carry1 = select bytes from r2427 by sel30
# asm 1: shufb >carry1=vec128#4,<r2427=vec128#10,<r2427=vec128#10,<sel30=vec128#34
# asm 2: shufb >carry1=$6,<r2427=$12,<r2427=$12,<sel30=$36
shufb $6,$12,$12,$36

# qhasm: carry2 = select bytes from r2831 by sel30
# asm 1: shufb >carry2=vec128#6,<r2831=vec128#13,<r2831=vec128#13,<sel30=vec128#34
# asm 2: shufb >carry2=$8,<r2831=$15,<r2831=$15,<sel30=$36
shufb $8,$15,$15,$36

# qhasm: carry3 = select bytes from r3235 by sel30
# asm 1: shufb >carry3=vec128#7,<r3235=vec128#2,<r3235=vec128#2,<sel30=vec128#34
# asm 2: shufb >carry3=$9,<r3235=$4,<r3235=$4,<sel30=$36
shufb $9,$4,$4,$36

# qhasm: uint32323232 carry1 >>= 12
# asm 1: rotmi <carry1=vec128#4,<carry1=vec128#4,-12
# asm 2: rotmi <carry1=$6,<carry1=$6,-12
rotmi $6,$6,-12

# qhasm: uint32323232 carry2 >>= 12
# asm 1: rotmi <carry2=vec128#6,<carry2=vec128#6,-12
# asm 2: rotmi <carry2=$8,<carry2=$8,-12
rotmi $8,$8,-12

# qhasm: uint32323232 carry3 >>= 12
# asm 1: rotmi <carry3=vec128#7,<carry3=vec128#7,-12
# asm 2: rotmi <carry3=$9,<carry3=$9,-12
rotmi $9,$9,-12

# qhasm: r2427 &= redcoeffmask
# asm 1: and <r2427=vec128#10,<r2427=vec128#10,<redcoeffmask=vec128#35
# asm 2: and <r2427=$12,<r2427=$12,<redcoeffmask=$37
and $12,$12,$37

# qhasm: r2831 &= redcoeffmask
# asm 1: and <r2831=vec128#13,<r2831=vec128#13,<redcoeffmask=vec128#35
# asm 2: and <r2831=$15,<r2831=$15,<redcoeffmask=$37
and $15,$15,$37

# qhasm: r3235 &= redcoeffmask
# asm 1: and <r3235=vec128#2,<r3235=vec128#2,<redcoeffmask=vec128#35
# asm 2: and <r3235=$4,<r3235=$4,<redcoeffmask=$37
and $4,$4,$37

# qhasm: r3639 &= redcoeffmaskend
# asm 1: and <r3639=vec128#5,<r3639=vec128#5,<redcoeffmaskend=vec128#38
# asm 2: and <r3639=$7,<r3639=$7,<redcoeffmaskend=$40
and $7,$7,$40

# qhasm: int32323232 r2831 += carry1
# asm 1: a <r2831=vec128#13,<r2831=vec128#13,<carry1=vec128#4
# asm 2: a <r2831=$15,<r2831=$15,<carry1=$6
a $15,$15,$6

# qhasm: int32323232 r3235 += carry2
# asm 1: a <r3235=vec128#2,<r3235=vec128#2,<carry2=vec128#6
# asm 2: a <r3235=$4,<r3235=$4,<carry2=$8
a $4,$4,$8

# qhasm: int32323232 r3639 += carry3
# asm 1: a <r3639=vec128#5,<r3639=vec128#5,<carry3=vec128#7
# asm 2: a <r3639=$7,<r3639=$7,<carry3=$9
a $7,$7,$9

# qhasm: int32323232 r03   += (r2023 & 0xffff) * (vec19 & 0xffff)
# asm 1: mpya >r03=vec128#3,<r2023=vec128#45,<vec19=vec128#77,<r03=vec128#3
# asm 2: mpya >r03=$5,<r2023=$47,<vec19=$79,<r03=$5
mpya $5,$47,$79,$5

# qhasm: int32323232 r1619 += (r3639 & 0xffff) * (vec19 & 0xffff)
# asm 1: mpya >r1619=vec128#4,<r3639=vec128#5,<vec19=vec128#77,<r1619=vec128#9
# asm 2: mpya >r1619=$6,<r3639=$7,<vec19=$79,<r1619=$11
mpya $6,$7,$79,$11

# qhasm: int32323232 r47   += (r2427 & 0xffff) * (vec19 & 0xffff)
# asm 1: mpya >r47=vec128#5,<r2427=vec128#10,<vec19=vec128#77,<r47=vec128#43
# asm 2: mpya >r47=$7,<r2427=$12,<vec19=$79,<r47=$45
mpya $7,$12,$79,$45

# qhasm: int32323232 r811  += (r2831 & 0xffff) * (vec19 & 0xffff)
# asm 1: mpya >r811=vec128#6,<r2831=vec128#13,<vec19=vec128#77,<r811=vec128#8
# asm 2: mpya >r811=$8,<r2831=$15,<vec19=$79,<r811=$10
mpya $8,$15,$79,$10

# qhasm: int32323232 r1215 += (r3235 & 0xffff) * (vec19 & 0xffff)
# asm 1: mpya >r1215=vec128#2,<r3235=vec128#2,<vec19=vec128#77,<r1215=vec128#50
# asm 2: mpya >r1215=$4,<r3235=$4,<vec19=$79,<r1215=$52
mpya $4,$4,$79,$52

# qhasm: lnop
 lnop

# qhasm: carry = select bytes from r1619 by sel01
# asm 1: shufb >carry=vec128#7,<r1619=vec128#4,<r1619=vec128#4,<sel01=vec128#29
# asm 2: shufb >carry=$9,<r1619=$6,<r1619=$6,<sel01=$31
shufb $9,$6,$6,$31

# qhasm: uint32323232 carry >>= 13
# asm 1: rotmi <carry=vec128#7,<carry=vec128#7,-13
# asm 2: rotmi <carry=$9,<carry=$9,-13
rotmi $9,$9,-13

# qhasm: int32323232 r1619 += carry
# asm 1: a <r1619=vec128#4,<r1619=vec128#4,<carry=vec128#7
# asm 2: a <r1619=$6,<r1619=$6,<carry=$9
a $6,$6,$9

# qhasm: carry = select bytes from r1619 by sel12
# asm 1: shufb >carry=vec128#7,<r1619=vec128#4,<r1619=vec128#4,<sel12=vec128#31
# asm 2: shufb >carry=$9,<r1619=$6,<r1619=$6,<sel12=$33
shufb $9,$6,$6,$33

# qhasm: uint32323232 carry >>= 13
# asm 1: rotmi <carry=vec128#7,<carry=vec128#7,-13
# asm 2: rotmi <carry=$9,<carry=$9,-13
rotmi $9,$9,-13

# qhasm: int32323232 r1619 += carry
# asm 1: a <r1619=vec128#4,<r1619=vec128#4,<carry=vec128#7
# asm 2: a <r1619=$6,<r1619=$6,<carry=$9
a $6,$6,$9

# qhasm: carry = select bytes from r1619 by sel23
# asm 1: shufb >carry=vec128#7,<r1619=vec128#4,<r1619=vec128#4,<sel23=vec128#33
# asm 2: shufb >carry=$9,<r1619=$6,<r1619=$6,<sel23=$35
shufb $9,$6,$6,$35

# qhasm: uint32323232 carry >>= 13
# asm 1: rotmi <carry=vec128#7,<carry=vec128#7,-13
# asm 2: rotmi <carry=$9,<carry=$9,-13
rotmi $9,$9,-13

# qhasm: int32323232 r1619 += carry
# asm 1: a <r1619=vec128#4,<r1619=vec128#4,<carry=vec128#7
# asm 2: a <r1619=$6,<r1619=$6,<carry=$9
a $6,$6,$9

# qhasm: carry = select bytes from r1619 by sel30
# asm 1: shufb >carry=vec128#7,<r1619=vec128#4,<r1619=vec128#4,<sel30=vec128#34
# asm 2: shufb >carry=$9,<r1619=$6,<r1619=$6,<sel30=$36
shufb $9,$6,$6,$36

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
# asm 1: a <r03=vec128#3,<r03=vec128#3,<red=vec128#7
# asm 2: a <r03=$5,<r03=$5,<red=$9
a $5,$5,$9

# qhasm: r1619 &= redcoeffmask
# asm 1: and <r1619=vec128#4,<r1619=vec128#4,<redcoeffmask=vec128#35
# asm 2: and <r1619=$6,<r1619=$6,<redcoeffmask=$37
and $6,$6,$37

# qhasm: carry0 = select bytes from r03 by sel01
# asm 1: shufb >carry0=vec128#7,<r03=vec128#3,<r03=vec128#3,<sel01=vec128#29
# asm 2: shufb >carry0=$9,<r03=$5,<r03=$5,<sel01=$31
shufb $9,$5,$5,$31

# qhasm: carry1 = select bytes from r47 by sel01
# asm 1: shufb >carry1=vec128#8,<r47=vec128#5,<r47=vec128#5,<sel01=vec128#29
# asm 2: shufb >carry1=$10,<r47=$7,<r47=$7,<sel01=$31
shufb $10,$7,$7,$31

# qhasm: carry2 = select bytes from r811 by sel01
# asm 1: shufb >carry2=vec128#9,<r811=vec128#6,<r811=vec128#6,<sel01=vec128#29
# asm 2: shufb >carry2=$11,<r811=$8,<r811=$8,<sel01=$31
shufb $11,$8,$8,$31

# qhasm: carry3 = select bytes from r1215 by sel01
# asm 1: shufb >carry3=vec128#10,<r1215=vec128#2,<r1215=vec128#2,<sel01=vec128#29
# asm 2: shufb >carry3=$12,<r1215=$4,<r1215=$4,<sel01=$31
shufb $12,$4,$4,$31

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
# asm 1: a <r03=vec128#3,<r03=vec128#3,<carry0=vec128#7
# asm 2: a <r03=$5,<r03=$5,<carry0=$9
a $5,$5,$9

# qhasm: int32323232 r47 += carry1
# asm 1: a <r47=vec128#5,<r47=vec128#5,<carry1=vec128#8
# asm 2: a <r47=$7,<r47=$7,<carry1=$10
a $7,$7,$10

# qhasm: int32323232 r811 += carry2
# asm 1: a <r811=vec128#6,<r811=vec128#6,<carry2=vec128#9
# asm 2: a <r811=$8,<r811=$8,<carry2=$11
a $8,$8,$11

# qhasm: int32323232 r1215 += carry3
# asm 1: a <r1215=vec128#2,<r1215=vec128#2,<carry3=vec128#10
# asm 2: a <r1215=$4,<r1215=$4,<carry3=$12
a $4,$4,$12

# qhasm: carry0 = select bytes from r03 by sel12
# asm 1: shufb >carry0=vec128#7,<r03=vec128#3,<r03=vec128#3,<sel12=vec128#31
# asm 2: shufb >carry0=$9,<r03=$5,<r03=$5,<sel12=$33
shufb $9,$5,$5,$33

# qhasm: carry1 = select bytes from r47 by sel12
# asm 1: shufb >carry1=vec128#8,<r47=vec128#5,<r47=vec128#5,<sel12=vec128#31
# asm 2: shufb >carry1=$10,<r47=$7,<r47=$7,<sel12=$33
shufb $10,$7,$7,$33

# qhasm: carry2 = select bytes from r811 by sel12
# asm 1: shufb >carry2=vec128#9,<r811=vec128#6,<r811=vec128#6,<sel12=vec128#31
# asm 2: shufb >carry2=$11,<r811=$8,<r811=$8,<sel12=$33
shufb $11,$8,$8,$33

# qhasm: carry3 = select bytes from r1215 by sel12
# asm 1: shufb >carry3=vec128#10,<r1215=vec128#2,<r1215=vec128#2,<sel12=vec128#31
# asm 2: shufb >carry3=$12,<r1215=$4,<r1215=$4,<sel12=$33
shufb $12,$4,$4,$33

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
# asm 1: a <r03=vec128#3,<r03=vec128#3,<carry0=vec128#7
# asm 2: a <r03=$5,<r03=$5,<carry0=$9
a $5,$5,$9

# qhasm: int32323232 r47 += carry1
# asm 1: a <r47=vec128#5,<r47=vec128#5,<carry1=vec128#8
# asm 2: a <r47=$7,<r47=$7,<carry1=$10
a $7,$7,$10

# qhasm: int32323232 r811 += carry2
# asm 1: a <r811=vec128#6,<r811=vec128#6,<carry2=vec128#9
# asm 2: a <r811=$8,<r811=$8,<carry2=$11
a $8,$8,$11

# qhasm: int32323232 r1215 += carry3
# asm 1: a <r1215=vec128#2,<r1215=vec128#2,<carry3=vec128#10
# asm 2: a <r1215=$4,<r1215=$4,<carry3=$12
a $4,$4,$12

# qhasm: carry0 = select bytes from r03 by sel23
# asm 1: shufb >carry0=vec128#7,<r03=vec128#3,<r03=vec128#3,<sel23=vec128#33
# asm 2: shufb >carry0=$9,<r03=$5,<r03=$5,<sel23=$35
shufb $9,$5,$5,$35

# qhasm: carry1 = select bytes from r47 by sel23
# asm 1: shufb >carry1=vec128#8,<r47=vec128#5,<r47=vec128#5,<sel23=vec128#33
# asm 2: shufb >carry1=$10,<r47=$7,<r47=$7,<sel23=$35
shufb $10,$7,$7,$35

# qhasm: carry2 = select bytes from r811 by sel23
# asm 1: shufb >carry2=vec128#9,<r811=vec128#6,<r811=vec128#6,<sel23=vec128#33
# asm 2: shufb >carry2=$11,<r811=$8,<r811=$8,<sel23=$35
shufb $11,$8,$8,$35

# qhasm: carry3 = select bytes from r1215 by sel23
# asm 1: shufb >carry3=vec128#10,<r1215=vec128#2,<r1215=vec128#2,<sel23=vec128#33
# asm 2: shufb >carry3=$12,<r1215=$4,<r1215=$4,<sel23=$35
shufb $12,$4,$4,$35

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
# asm 1: a <r03=vec128#3,<r03=vec128#3,<carry0=vec128#7
# asm 2: a <r03=$5,<r03=$5,<carry0=$9
a $5,$5,$9

# qhasm: int32323232 r47 += carry1
# asm 1: a <r47=vec128#5,<r47=vec128#5,<carry1=vec128#8
# asm 2: a <r47=$7,<r47=$7,<carry1=$10
a $7,$7,$10

# qhasm: int32323232 r811 += carry2
# asm 1: a <r811=vec128#6,<r811=vec128#6,<carry2=vec128#9
# asm 2: a <r811=$8,<r811=$8,<carry2=$11
a $8,$8,$11

# qhasm: int32323232 r1215 += carry3
# asm 1: a <r1215=vec128#2,<r1215=vec128#2,<carry3=vec128#10
# asm 2: a <r1215=$4,<r1215=$4,<carry3=$12
a $4,$4,$12

# qhasm: carry0 = select bytes from r03 by sel30
# asm 1: shufb >carry0=vec128#7,<r03=vec128#3,<r03=vec128#3,<sel30=vec128#34
# asm 2: shufb >carry0=$9,<r03=$5,<r03=$5,<sel30=$36
shufb $9,$5,$5,$36

# qhasm: carry1 = select bytes from r47 by sel30
# asm 1: shufb >carry1=vec128#8,<r47=vec128#5,<r47=vec128#5,<sel30=vec128#34
# asm 2: shufb >carry1=$10,<r47=$7,<r47=$7,<sel30=$36
shufb $10,$7,$7,$36

# qhasm: carry2 = select bytes from r811 by sel30
# asm 1: shufb >carry2=vec128#9,<r811=vec128#6,<r811=vec128#6,<sel30=vec128#34
# asm 2: shufb >carry2=$11,<r811=$8,<r811=$8,<sel30=$36
shufb $11,$8,$8,$36

# qhasm: carry3 = select bytes from r1215 by sel30
# asm 1: shufb >carry3=vec128#10,<r1215=vec128#2,<r1215=vec128#2,<sel30=vec128#34
# asm 2: shufb >carry3=$12,<r1215=$4,<r1215=$4,<sel30=$36
shufb $12,$4,$4,$36

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
# asm 1: and <r03=vec128#3,<r03=vec128#3,<redcoeffmask=vec128#35
# asm 2: and <r03=$5,<r03=$5,<redcoeffmask=$37
and $5,$5,$37

# qhasm: r47 &= redcoeffmask
# asm 1: and <r47=vec128#5,<r47=vec128#5,<redcoeffmask=vec128#35
# asm 2: and <r47=$7,<r47=$7,<redcoeffmask=$37
and $7,$7,$37

# qhasm: r811 &= redcoeffmask
# asm 1: and <r811=vec128#6,<r811=vec128#6,<redcoeffmask=vec128#35
# asm 2: and <r811=$8,<r811=$8,<redcoeffmask=$37
and $8,$8,$37

# qhasm: r1215 &= redcoeffmask
# asm 1: and <r1215=vec128#2,<r1215=vec128#2,<redcoeffmask=vec128#35
# asm 2: and <r1215=$4,<r1215=$4,<redcoeffmask=$37
and $4,$4,$37

# qhasm: int32323232 r47 += carry0
# asm 1: a <r47=vec128#5,<r47=vec128#5,<carry0=vec128#7
# asm 2: a <r47=$7,<r47=$7,<carry0=$9
a $7,$7,$9

# qhasm: int32323232 r811 += carry1
# asm 1: a <r811=vec128#6,<r811=vec128#6,<carry1=vec128#8
# asm 2: a <r811=$8,<r811=$8,<carry1=$10
a $8,$8,$10

# qhasm: int32323232 r1215 += carry2
# asm 1: a <r1215=vec128#2,<r1215=vec128#2,<carry2=vec128#9
# asm 2: a <r1215=$4,<r1215=$4,<carry2=$11
a $4,$4,$11

# qhasm: int32323232 r1619 += carry3
# asm 1: a <r1619=vec128#4,<r1619=vec128#4,<carry3=vec128#10
# asm 2: a <r1619=$6,<r1619=$6,<carry3=$12
a $6,$6,$12

# qhasm: carry1 = select bytes from r47 by sel01
# asm 1: shufb >carry1=vec128#7,<r47=vec128#5,<r47=vec128#5,<sel01=vec128#29
# asm 2: shufb >carry1=$9,<r47=$7,<r47=$7,<sel01=$31
shufb $9,$7,$7,$31

# qhasm: carry2 = select bytes from r811 by sel01
# asm 1: shufb >carry2=vec128#8,<r811=vec128#6,<r811=vec128#6,<sel01=vec128#29
# asm 2: shufb >carry2=$10,<r811=$8,<r811=$8,<sel01=$31
shufb $10,$8,$8,$31

# qhasm: carry3 = select bytes from r1215 by sel01
# asm 1: shufb >carry3=vec128#9,<r1215=vec128#2,<r1215=vec128#2,<sel01=vec128#29
# asm 2: shufb >carry3=$11,<r1215=$4,<r1215=$4,<sel01=$31
shufb $11,$4,$4,$31

# qhasm: carry4 = select bytes from r1619 by sel01
# asm 1: shufb >carry4=vec128#10,<r1619=vec128#4,<r1619=vec128#4,<sel01=vec128#29
# asm 2: shufb >carry4=$12,<r1619=$6,<r1619=$6,<sel01=$31
shufb $12,$6,$6,$31

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
# asm 1: a <r1215=vec128#2,<r1215=vec128#2,<carry3=vec128#9
# asm 2: a <r1215=$4,<r1215=$4,<carry3=$11
a $4,$4,$11

# qhasm: int32323232 r1619 += carry4
# asm 1: a <r1619=vec128#4,<r1619=vec128#4,<carry4=vec128#10
# asm 2: a <r1619=$6,<r1619=$6,<carry4=$12
a $6,$6,$12

# qhasm: carry1 = select bytes from r47 by sel12
# asm 1: shufb >carry1=vec128#7,<r47=vec128#5,<r47=vec128#5,<sel12=vec128#31
# asm 2: shufb >carry1=$9,<r47=$7,<r47=$7,<sel12=$33
shufb $9,$7,$7,$33

# qhasm: carry2 = select bytes from r811 by sel12
# asm 1: shufb >carry2=vec128#8,<r811=vec128#6,<r811=vec128#6,<sel12=vec128#31
# asm 2: shufb >carry2=$10,<r811=$8,<r811=$8,<sel12=$33
shufb $10,$8,$8,$33

# qhasm: carry3 = select bytes from r1215 by sel12
# asm 1: shufb >carry3=vec128#9,<r1215=vec128#2,<r1215=vec128#2,<sel12=vec128#31
# asm 2: shufb >carry3=$11,<r1215=$4,<r1215=$4,<sel12=$33
shufb $11,$4,$4,$33

# qhasm: carry4 = select bytes from r1619 by sel12
# asm 1: shufb >carry4=vec128#10,<r1619=vec128#4,<r1619=vec128#4,<sel12=vec128#31
# asm 2: shufb >carry4=$12,<r1619=$6,<r1619=$6,<sel12=$33
shufb $12,$6,$6,$33

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
# asm 1: a <r1215=vec128#2,<r1215=vec128#2,<carry3=vec128#9
# asm 2: a <r1215=$4,<r1215=$4,<carry3=$11
a $4,$4,$11

# qhasm: int32323232 r1619 += carry4
# asm 1: a <r1619=vec128#4,<r1619=vec128#4,<carry4=vec128#10
# asm 2: a <r1619=$6,<r1619=$6,<carry4=$12
a $6,$6,$12

# qhasm: carry1 = select bytes from r47 by sel23
# asm 1: shufb >carry1=vec128#7,<r47=vec128#5,<r47=vec128#5,<sel23=vec128#33
# asm 2: shufb >carry1=$9,<r47=$7,<r47=$7,<sel23=$35
shufb $9,$7,$7,$35

# qhasm: carry2 = select bytes from r811 by sel23
# asm 1: shufb >carry2=vec128#8,<r811=vec128#6,<r811=vec128#6,<sel23=vec128#33
# asm 2: shufb >carry2=$10,<r811=$8,<r811=$8,<sel23=$35
shufb $10,$8,$8,$35

# qhasm: carry3 = select bytes from r1215 by sel23
# asm 1: shufb >carry3=vec128#9,<r1215=vec128#2,<r1215=vec128#2,<sel23=vec128#33
# asm 2: shufb >carry3=$11,<r1215=$4,<r1215=$4,<sel23=$35
shufb $11,$4,$4,$35

# qhasm: carry4 = select bytes from r1619 by sel23
# asm 1: shufb >carry4=vec128#10,<r1619=vec128#4,<r1619=vec128#4,<sel23=vec128#33
# asm 2: shufb >carry4=$12,<r1619=$6,<r1619=$6,<sel23=$35
shufb $12,$6,$6,$35

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
# asm 1: a <r1215=vec128#2,<r1215=vec128#2,<carry3=vec128#9
# asm 2: a <r1215=$4,<r1215=$4,<carry3=$11
a $4,$4,$11

# qhasm: int32323232 r1619 += carry4
# asm 1: a <r1619=vec128#4,<r1619=vec128#4,<carry4=vec128#10
# asm 2: a <r1619=$6,<r1619=$6,<carry4=$12
a $6,$6,$12

# qhasm: r47 &= redcoeffmaskveryend
# asm 1: and <r47=vec128#5,<r47=vec128#5,<redcoeffmaskveryend=vec128#40
# asm 2: and <r47=$7,<r47=$7,<redcoeffmaskveryend=$42
and $7,$7,$42

# qhasm: r811 &= redcoeffmaskveryend
# asm 1: and <r811=vec128#6,<r811=vec128#6,<redcoeffmaskveryend=vec128#40
# asm 2: and <r811=$8,<r811=$8,<redcoeffmaskveryend=$42
and $8,$8,$42

# qhasm: r1215 &= redcoeffmaskveryend
# asm 1: and <r1215=vec128#2,<r1215=vec128#2,<redcoeffmaskveryend=vec128#40
# asm 2: and <r1215=$4,<r1215=$4,<redcoeffmaskveryend=$42
and $4,$4,$42

# qhasm: r1619 &= redcoeffmaskveryend
# asm 1: and <r1619=vec128#4,<r1619=vec128#4,<redcoeffmaskveryend=vec128#40
# asm 2: and <r1619=$6,<r1619=$6,<redcoeffmaskveryend=$42
and $6,$6,$42

# qhasm: *(vec128 *) ((retp + 0) & ~15) = r03
# asm 1: stqd <r03=vec128#3,0(<retp=vec128#1)
# asm 2: stqd <r03=$5,0(<retp=$3)
stqd $5,0($3)

# qhasm: *(vec128 *) ((retp + 16) & ~15) = r47
# asm 1: stqd <r47=vec128#5,16(<retp=vec128#1)
# asm 2: stqd <r47=$7,16(<retp=$3)
stqd $7,16($3)

# qhasm: *(vec128 *) ((retp + 32) & ~15) = r811
# asm 1: stqd <r811=vec128#6,32(<retp=vec128#1)
# asm 2: stqd <r811=$8,32(<retp=$3)
stqd $8,32($3)

# qhasm: *(vec128 *) ((retp + 48) & ~15) = r1215
# asm 1: stqd <r1215=vec128#2,48(<retp=vec128#1)
# asm 2: stqd <r1215=$4,48(<retp=$3)
stqd $4,48($3)

# qhasm: *(vec128 *) ((retp + 64) & ~15) = r1619
# asm 1: stqd <r1619=vec128#4,64(<retp=vec128#1)
# asm 2: stqd <r1619=$6,64(<retp=$3)
stqd $6,64($3)

# qhasm: call0 = call0_stack
# asm 1: lqd >call0=vec128#78,[32+<call0_stack=stack128#1]($sp)
# asm 2: lqd >call0=$80,[32+<call0_stack=0]($sp)
lqd $80,[32+0]($sp)

# qhasm: call1 = call1_stack
# asm 1: lqd >call1=vec128#79,[32+<call1_stack=stack128#2]($sp)
# asm 2: lqd >call1=$81,[32+<call1_stack=16]($sp)
lqd $81,[32+16]($sp)

# qhasm: call2 = call2_stack
# asm 1: lqd >call2=vec128#80,[32+<call2_stack=stack128#3]($sp)
# asm 2: lqd >call2=$82,[32+<call2_stack=32]($sp)
lqd $82,[32+32]($sp)

# qhasm: call3 = call3_stack
# asm 1: lqd >call3=vec128#81,[32+<call3_stack=stack128#4]($sp)
# asm 2: lqd >call3=$83,[32+<call3_stack=48]($sp)
lqd $83,[32+48]($sp)

# qhasm: leave
ai $sp,$sp,96
bi $lr
