
# qhasm: int32 input_0

# qhasm: int32 input_1

# qhasm: int32 input_2

# qhasm: int32 input_3

# qhasm: stack32 input_4

# qhasm: stack32 input_5

# qhasm: stack32 input_6

# qhasm: stack32 input_7

# qhasm: int32 caller_r4

# qhasm: int32 caller_r5

# qhasm: int32 caller_r6

# qhasm: int32 caller_r7

# qhasm: int32 caller_r8

# qhasm: int32 caller_r9

# qhasm: int32 caller_r10

# qhasm: int32 caller_r11

# qhasm: int32 caller_r12

# qhasm: int32 caller_r14

# qhasm: reg128 caller_q4

# qhasm: reg128 caller_q5

# qhasm: reg128 caller_q6

# qhasm: reg128 caller_q7

# qhasm: startcode
.fpu neon
.text

# qhasm: stack3072 playground1

# qhasm: int32 playground1_ptr

# qhasm: int32 i

# qhasm: int32 j

# qhasm: int32 ptr

# qhasm: int32 swap

# qhasm: int32 pos

# qhasm: int32 bit

# qhasm: int32 byte

# qhasm: int32 word

# qhasm: int32 pos8

# qhasm: int32 pos7

# qhasm: int32 mulsource

# qhasm: int32 postcopy

# qhasm: int32 q

# qhasm: int32 p

# qhasm: int32 n

# qhasm: reg128 e0

# qhasm: reg128 e4

# qhasm: reg128 f0

# qhasm: reg128 f4

# qhasm: reg128 f8

# qhasm: reg128 g0

# qhasm: reg128 g4

# qhasm: reg128 g8

# qhasm: reg128 d0

# qhasm: reg128 d4

# qhasm: reg128 d8

# qhasm: reg128 x0

# qhasm: reg128 x4

# qhasm: reg128 x8

# qhasm: reg128 F0

# qhasm: reg128 F4

# qhasm: reg128 F8

# qhasm: reg128 G0

# qhasm: reg128 G4

# qhasm: reg128 G8

# qhasm: reg128 X0

# qhasm: reg128 X4

# qhasm: reg128 X8

# qhasm: reg128 f0plusF0

# qhasm: reg128 f0minusF0

# qhasm: reg128 g0plusG0

# qhasm: reg128 g0minusG0

# qhasm: reg128 f4plusF4

# qhasm: reg128 f4minusF4

# qhasm: reg128 g4plusG4

# qhasm: reg128 g4minusG4

# qhasm: reg128 f8plusF8

# qhasm: reg128 f8minusF8

# qhasm: reg128 g8plusG8

# qhasm: reg128 g8minusG8

# qhasm: reg128 fg01

# qhasm: reg128 fg23

# qhasm: reg128 fg45

# qhasm: reg128 fg67

# qhasm: reg128 fg89

# qhasm: reg128 fg01_2

# qhasm: reg128 fg23_2

# qhasm: reg128 fg45_2

# qhasm: reg128 fg67_2

# qhasm: reg128 fg45_19_38

# qhasm: reg128 fg67_19_38

# qhasm: reg128 fg89_19_38

# qhasm: reg128 h0

# qhasm: reg128 h1

# qhasm: reg128 h2

# qhasm: reg128 h3

# qhasm: reg128 h4

# qhasm: reg128 h5

# qhasm: reg128 h6

# qhasm: reg128 h7

# qhasm: reg128 h8

# qhasm: reg128 h9

# qhasm: stack64 h0stack

# qhasm: stack64 h1stack

# qhasm: stack64 h2stack

# qhasm: stack64 h3stack

# qhasm: stack64 h4stack

# qhasm: stack64 h5stack

# qhasm: stack64 h6stack

# qhasm: stack64 h7stack

# qhasm: stack64 h8stack

# qhasm: stack64 h9stack

# qhasm: reg128 t0

# qhasm: reg128 t1

# qhasm: reg128 t2

# qhasm: reg128 t3

# qhasm: reg128 t4

# qhasm: reg128 t5

# qhasm: reg128 t6

# qhasm: reg128 t7

# qhasm: reg128 t8

# qhasm: reg128 t9

# qhasm: reg128 c0

# qhasm: reg128 c1

# qhasm: reg128 c2

# qhasm: reg128 c3

# qhasm: reg128 c4

# qhasm: reg128 c5

# qhasm: reg128 c6

# qhasm: reg128 c7

# qhasm: reg128 c8

# qhasm: reg128 c9

# qhasm: reg128 f02

# qhasm: reg128 f13

# qhasm: reg128 f46

# qhasm: reg128 f57

# qhasm: reg128 f89

# qhasm: reg128 g02

# qhasm: reg128 g13

# qhasm: reg128 g46

# qhasm: reg128 g57

# qhasm: reg128 g89

# qhasm: reg128 f13_2

# qhasm: reg128 f57_2

# qhasm: reg128 f89_2

# qhasm: reg128 mix

# qhasm: reg128 g13_19

# qhasm: reg128 g46_19

# qhasm: reg128 g57_19

# qhasm: reg128 g89_19

# qhasm: stack128 f13_2_stack

# qhasm: stack128 f57_2_stack

# qhasm: stack128 mix_stack

# qhasm: stack128 g13_19_stack

# qhasm: stack128 g46_19_stack

# qhasm: stack128 g57_19_stack

# qhasm: stack128 g89_19_stack

# qhasm: stack128 h9_stack

# qhasm: stack128 h7_stack

# qhasm: stack128 h5_stack

# qhasm: reg128 t

# qhasm: reg128 s

# qhasm: reg128 s2

# qhasm: reg128 c

# qhasm: reg128 mask26

# qhasm: reg128 mask25

# qhasm: reg128 _0x2000000

# qhasm: reg128 _0x1000000

# qhasm: reg128 _19_19_38_38

# qhasm: stack128 _0x2000000_stack

# qhasm: stack128 _0x1000000_stack

# qhasm: stack128 _19_19_38_38_stack

# qhasm: reg128 h02

# qhasm: reg128 h24

# qhasm: reg128 h46

# qhasm: reg128 h68

# qhasm: reg128 h80

# qhasm: reg128 h31

# qhasm: reg128 h53

# qhasm: reg128 h75

# qhasm: reg128 h97

# qhasm: reg128 h19

# qhasm: reg128 h04

# qhasm: reg128 h15

# qhasm: reg128 h26

# qhasm: reg128 h37

# qhasm: reg128 h48

# qhasm: reg128 h59

# qhasm: reg128 f0_f1_f2_f3

# qhasm: reg128 f4_f5_f6_f7

# qhasm: reg128 f8_f9_g8_g9

# qhasm: reg128 19f8_19f9_19g8_19g9

# qhasm: reg128 f8_2f9_g8_g9

# qhasm: reg128 g0_g1_g2_g3

# qhasm: reg128 g4_g5_g6_g7

# qhasm: reg128 f0_2f1_f2_2f3

# qhasm: reg128 f4_2f5_f6_2f7

# qhasm: reg128 f8_2f9_f9_f6

# qhasm: reg128 g0_19g1_g2_19g3

# qhasm: reg128 19g0_19g1_19g2_19g3 

# qhasm: reg128 19g4_19g5_19g6_19g7

# qhasm: reg128 g4_19g5_g6_19g7

# qhasm: reg128 g8_19g9_19g8_19g9

# qhasm: reg128 f1_f8_f3_f0

# qhasm: reg128 f5_f2_f7_f4

# qhasm: reg128 19g8_g9_19g2_g3

# qhasm: reg128 19g4_g5_19g6_g7

# qhasm: reg128 _19_19_19_19

# qhasm: reg128 _0_1_0_1

# qhasm: reg128 _1_1_1_1

# qhasm: stack512 playground2

# qhasm: int32 playp

# qhasm: int32 binput

# qhasm: reg128 b

# qhasm: int32 pos0

# qhasm: int32 pos1

# qhasm: int32 pos2

# qhasm: int32 pos3

# qhasm: int32 posh

# qhasm: int32 posf

# qhasm: int32 posg

# qhasm: int32 posH

# qhasm: int32 posF

# qhasm: int32 posG

# qhasm: int32 posx

# qhasm: int32 posy

# qhasm: int32 out0

# qhasm: int32 out1

# qhasm: int32 out2

# qhasm: int32 out3

# qhasm: int32 out4

# qhasm: int32 out5

# qhasm: int32 out6

# qhasm: int32 out7

# qhasm: int32 out8

# qhasm: int32 out9

# qhasm: int32 carry0

# qhasm: int32 carry1

# qhasm: int32 carry2

# qhasm: int32 carry3

# qhasm: int32 carry4

# qhasm: int32 carry5

# qhasm: int32 carry6

# qhasm: int32 carry7

# qhasm: int32 carry8

# qhasm: int32 carry9

# qhasm: int32 carry

# qhasm: reg128 zero

# qhasm: reg128 one

# qhasm: qpushenter crypto_scalarmult_curve25519_neon2
.align 4
.global _crypto_scalarmult_curve25519_neon2
.global crypto_scalarmult_curve25519_neon2
.type _crypto_scalarmult_curve25519_neon2 STT_FUNC
.type crypto_scalarmult_curve25519_neon2 STT_FUNC
_crypto_scalarmult_curve25519_neon2:
crypto_scalarmult_curve25519_neon2:
vpush {q4,q5,q6,q7}
mov r12,sp
sub sp,sp,#736
and sp,sp,#0xffffffe0

# qhasm: stack64 stack_r45

# qhasm: stack64 stack_r67

# qhasm: stack64 stack_r89

# qhasm: stack64 stack_r1011

# qhasm: stack32 stack_r12

# qhasm: stack32 stack_r14

# qhasm: assign r4 r5 to caller_r4 caller_r5;stack_r45 = caller_r4 caller_r5
# asm 1: strd <caller_r4=int32#5,>stack_r45=stack64#1
# asm 2: strd <caller_r4=r4,>stack_r45=[sp,#0]
strd r4,[sp,#0]

# qhasm: assign r6 r7 to caller_r6 caller_r7;stack_r67 = caller_r6 caller_r7
# asm 1: strd <caller_r6=int32#7,>stack_r67=stack64#2
# asm 2: strd <caller_r6=r6,>stack_r67=[sp,#8]
strd r6,[sp,#8]

# qhasm: assign r8 r9 to caller_r8 caller_r9;stack_r89 = caller_r8 caller_r9
# asm 1: strd <caller_r8=int32#9,>stack_r89=stack64#3
# asm 2: strd <caller_r8=r8,>stack_r89=[sp,#16]
strd r8,[sp,#16]

# qhasm: assign r10 r11 to caller_r10 caller_r11;stack_r1011 = caller_r10 caller_r11
# asm 1: strd <caller_r10=int32#11,>stack_r1011=stack64#4
# asm 2: strd <caller_r10=r10,>stack_r1011=[sp,#24]
strd r10,[sp,#24]

# qhasm: stack_r12 = caller_r12
# asm 1: str <caller_r12=int32#13,>stack_r12=stack32#1
# asm 2: str <caller_r12=r12,>stack_r12=[sp,#480]
str r12,[sp,#480]

# qhasm: stack_r14 = caller_r14
# asm 1: str <caller_r14=int32#14,>stack_r14=stack32#2
# asm 2: str <caller_r14=r14,>stack_r14=[sp,#484]
str r14,[sp,#484]

# qhasm: stack32 swap_stack

# qhasm: stack32 pos_stack

# qhasm: stack256 e

# qhasm: int32 eptr

# qhasm: q = input_0
# asm 1: mov >q=int32#1,<input_0=int32#1
# asm 2: mov >q=r0,<input_0=r0
mov r0,r0

# qhasm: n = input_1
# asm 1: mov >n=int32#2,<input_1=int32#2
# asm 2: mov >n=r1,<input_1=r1
mov r1,r1

# qhasm: p = input_2
# asm 1: mov >p=int32#3,<input_2=int32#3
# asm 2: mov >p=r2,<input_2=r2
mov r2,r2

# qhasm: playground1_ptr = &playground1
# asm 1: lea >playground1_ptr=int32#4,<playground1=stack3072#1
# asm 2: lea >playground1_ptr=r3,<playground1=[sp,#32]
add r3,sp,#32

# qhasm: swap = 0
# asm 1: ldr >swap=int32#5,=0
# asm 2: ldr >swap=r4,=0
ldr r4,=0

# qhasm: pos = 254
# asm 1: ldr >pos=int32#6,=254
# asm 2: ldr >pos=r5,=254
ldr r5,=254

# qhasm: 4x _0x1000000 = 1
# asm 1: vmov.i32 >_0x1000000=reg128#1,#1
# asm 2: vmov.i32 >_0x1000000=q0,#1
vmov.i32 q0,#1

# qhasm: 2x _0x2000000 = _0x1000000 unsigned>> 7
# asm 1: vshr.u64 >_0x2000000=reg128#2,<_0x1000000=reg128#1,#7
# asm 2: vshr.u64 >_0x2000000=q1,<_0x1000000=q0,#7
vshr.u64 q1,q0,#7

# qhasm: 2x _0x1000000 = _0x1000000 unsigned>> 8
# asm 1: vshr.u64 >_0x1000000=reg128#1,<_0x1000000=reg128#1,#8
# asm 2: vshr.u64 >_0x1000000=q0,<_0x1000000=q0,#8
vshr.u64 q0,q0,#8

# qhasm: new _19_19_38_38

# qhasm: _19_19_38_38 = 19,19,_19_19_38_38[2,3]
# asm 1: vmov.i32 <_19_19_38_38=reg128#3%bot,#19
# asm 2: vmov.i32 <_19_19_38_38=d4,#19
vmov.i32 d4,#19

# qhasm: _19_19_38_38 = _19_19_38_38[0,1],38,38
# asm 1: vmov.i32 <_19_19_38_38=reg128#3%top,#38
# asm 2: vmov.i32 <_19_19_38_38=d5,#38
vmov.i32 d5,#38

# qhasm: ptr = &_0x2000000_stack
# asm 1: lea >ptr=int32#7,<_0x2000000_stack=stack128#1
# asm 2: lea >ptr=r6,<_0x2000000_stack=[sp,#512]
add r6,sp,#512

# qhasm: mem128[ptr] aligned= _0x2000000
# asm 1: vst1.8 {<_0x2000000=reg128#2%bot-<_0x2000000=reg128#2%top},[<ptr=int32#7,: 128]
# asm 2: vst1.8 {<_0x2000000=d2-<_0x2000000=d3},[<ptr=r6,: 128]
vst1.8 {d2-d3},[r6,: 128]

# qhasm: ptr = &_0x1000000_stack
# asm 1: lea >ptr=int32#7,<_0x1000000_stack=stack128#2
# asm 2: lea >ptr=r6,<_0x1000000_stack=[sp,#528]
add r6,sp,#528

# qhasm: mem128[ptr] aligned= _0x1000000
# asm 1: vst1.8 {<_0x1000000=reg128#1%bot-<_0x1000000=reg128#1%top},[<ptr=int32#7,: 128]
# asm 2: vst1.8 {<_0x1000000=d0-<_0x1000000=d1},[<ptr=r6,: 128]
vst1.8 {d0-d1},[r6,: 128]

# qhasm: ptr = &_19_19_38_38_stack
# asm 1: lea >ptr=int32#7,<_19_19_38_38_stack=stack128#3
# asm 2: lea >ptr=r6,<_19_19_38_38_stack=[sp,#544]
add r6,sp,#544

# qhasm: mem128[ptr] aligned= _19_19_38_38
# asm 1: vst1.8 {<_19_19_38_38=reg128#3%bot-<_19_19_38_38=reg128#3%top},[<ptr=int32#7,: 128]
# asm 2: vst1.8 {<_19_19_38_38=d4-<_19_19_38_38=d5},[<ptr=r6,: 128]
vst1.8 {d4-d5},[r6,: 128]

# qhasm:   posx = playground1_ptr + 0 
# asm 1: add >posx=int32#7,<playground1_ptr=int32#4,#0
# asm 2: add >posx=r6,<playground1_ptr=r3,#0
add r6,r3,#0

# qhasm:   4x zero = 0 
# asm 1: vmov.i32 >zero=reg128#3,#0
# asm 2: vmov.i32 >zero=q2,#0
vmov.i32 q2,#0

# qhasm:   mem128[posx] aligned= zero;posx += 16 
# asm 1: vst1.8 {<zero=reg128#3%bot-<zero=reg128#3%top},[<posx=int32#7,: 128]!
# asm 2: vst1.8 {<zero=d4-<zero=d5},[<posx=r6,: 128]!
vst1.8 {d4-d5},[r6,: 128]!

# qhasm:   mem128[posx] aligned= zero;posx += 16 
# asm 1: vst1.8 {<zero=reg128#3%bot-<zero=reg128#3%top},[<posx=int32#7,: 128]!
# asm 2: vst1.8 {<zero=d4-<zero=d5},[<posx=r6,: 128]!
vst1.8 {d4-d5},[r6,: 128]!

# qhasm:   mem64[posx] aligned= zero[0] 
# asm 1: vst1.8 <zero=reg128#3%bot,[<posx=int32#7,: 64]
# asm 2: vst1.8 <zero=d4,[<posx=r6,: 64]
vst1.8 d4,[r6,: 64]

# qhasm: ptr = playground1_ptr + 0
# asm 1: add >ptr=int32#7,<playground1_ptr=int32#4,#0
# asm 2: add >ptr=r6,<playground1_ptr=r3,#0
add r6,r3,#0

# qhasm: word = 960
# asm 1: ldr >word=int32#8,=960
# asm 2: ldr >word=r7,=960
ldr r7,=960

# qhasm: word = word - 2
# asm 1: sub >word=int32#8,<word=int32#8,#2
# asm 2: sub >word=r7,<word=r7,#2
sub r7,r7,#2

# qhasm: word = -word
# asm 1: neg >word=int32#8,<word=int32#8
# asm 2: neg >word=r7,<word=r7
neg r7,r7

# qhasm: word = word - (word << 7)
# asm 1: sub >word=int32#8,<word=int32#8,<word=int32#8,LSL #7
# asm 2: sub >word=r7,<word=r7,<word=r7,LSL #7
sub r7,r7,r7,LSL #7

# qhasm: mem32[ptr] = word
# asm 1: str <word=int32#8,[<ptr=int32#7]
# asm 2: str <word=r7,[<ptr=r6]
str r7,[r6]

# qhasm: eptr = &e
# asm 1: lea >eptr=int32#7,<e=stack256#1
# asm 2: lea >eptr=r6,<e=[sp,#704]
add r6,sp,#704

# qhasm: e0 = mem128[n];n += 16
# asm 1: vld1.8 {>e0=reg128#3%bot->e0=reg128#3%top},[<n=int32#2]!
# asm 2: vld1.8 {>e0=d4->e0=d5},[<n=r1]!
vld1.8 {d4-d5},[r1]!

# qhasm: e4 = mem128[n]
# asm 1: vld1.8 {>e4=reg128#4%bot->e4=reg128#4%top},[<n=int32#2]
# asm 2: vld1.8 {>e4=d6->e4=d7},[<n=r1]
vld1.8 {d6-d7},[r1]

# qhasm: mem128[eptr] aligned= e0;eptr += 16
# asm 1: vst1.8 {<e0=reg128#3%bot-<e0=reg128#3%top},[<eptr=int32#7,: 128]!
# asm 2: vst1.8 {<e0=d4-<e0=d5},[<eptr=r6,: 128]!
vst1.8 {d4-d5},[r6,: 128]!

# qhasm: mem128[eptr] aligned= e4
# asm 1: vst1.8 {<e4=reg128#4%bot-<e4=reg128#4%top},[<eptr=int32#7,: 128]
# asm 2: vst1.8 {<e4=d6-<e4=d7},[<eptr=r6,: 128]
vst1.8 {d6-d7},[r6,: 128]

# qhasm: eptr -= 16
# asm 1: sub >eptr=int32#2,<eptr=int32#7,#16
# asm 2: sub >eptr=r1,<eptr=r6,#16
sub r1,r6,#16

# qhasm: byte = mem8[eptr]
# asm 1: ldrb >byte=int32#7,[<eptr=int32#2]
# asm 2: ldrb >byte=r6,[<eptr=r1]
ldrb r6,[r1]

# qhasm: byte &= 248
# asm 1: and >byte=int32#7,<byte=int32#7,#248
# asm 2: and >byte=r6,<byte=r6,#248
and r6,r6,#248

# qhasm: mem8[eptr] = byte
# asm 1: strb <byte=int32#7,[<eptr=int32#2]
# asm 2: strb <byte=r6,[<eptr=r1]
strb r6,[r1]

# qhasm: byte = mem8[eptr + 31]
# asm 1: ldrb >byte=int32#7,[<eptr=int32#2,#31]
# asm 2: ldrb >byte=r6,[<eptr=r1,#31]
ldrb r6,[r1,#31]

# qhasm: byte &= 127
# asm 1: and >byte=int32#7,<byte=int32#7,#127
# asm 2: and >byte=r6,<byte=r6,#127
and r6,r6,#127

# qhasm: byte |= 64
# asm 1: orr >byte=int32#7,<byte=int32#7,#64
# asm 2: orr >byte=r6,<byte=r6,#64
orr r6,r6,#64

# qhasm: mem8[eptr + 31] = byte
# asm 1: strb <byte=int32#7,[<eptr=int32#2,#31]
# asm 2: strb <byte=r6,[<eptr=r1,#31]
strb r6,[r1,#31]

# qhasm: 2x mask26 = 0xffffffff
# asm 1: vmov.i64 >mask26=reg128#3,#0xffffffff
# asm 2: vmov.i64 >mask26=q2,#0xffffffff
vmov.i64 q2,#0xffffffff

# qhasm: 2x mask25 = mask26 unsigned>> 7
# asm 1: vshr.u64 >mask25=reg128#4,<mask26=reg128#3,#7
# asm 2: vshr.u64 >mask25=q3,<mask26=q2,#7
vshr.u64 q3,q2,#7

# qhasm: 2x mask26 = mask26 unsigned>> 6
# asm 1: vshr.u64 >mask26=reg128#3,<mask26=reg128#3,#6
# asm 2: vshr.u64 >mask26=q2,<mask26=q2,#6
vshr.u64 q2,q2,#6

# qhasm: new h0

# qhasm: new h1

# qhasm: new h2

# qhasm: new h3

# qhasm: new h4

# qhasm: new h5

# qhasm: new h6

# qhasm: new h7

# qhasm: new h8

# qhasm: new h9

# qhasm: h0 = mem64[p] h0[1]
# asm 1: vld1.8 {<h0=reg128#5%bot},[<p=int32#3]
# asm 2: vld1.8 {<h0=d8},[<p=r2]
vld1.8 {d8},[r2]

# qhasm: h1 = mem64[p] h1[1]
# asm 1: vld1.8 {<h1=reg128#6%bot},[<p=int32#3]
# asm 2: vld1.8 {<h1=d10},[<p=r2]
vld1.8 {d10},[r2]

# qhasm: p += 6
# asm 1: add >p=int32#3,<p=int32#3,#6
# asm 2: add >p=r2,<p=r2,#6
add r2,r2,#6

# qhasm: h2 = mem64[p] h2[1]
# asm 1: vld1.8 {<h2=reg128#7%bot},[<p=int32#3]
# asm 2: vld1.8 {<h2=d12},[<p=r2]
vld1.8 {d12},[r2]

# qhasm: h3 = mem64[p] h3[1]
# asm 1: vld1.8 {<h3=reg128#8%bot},[<p=int32#3]
# asm 2: vld1.8 {<h3=d14},[<p=r2]
vld1.8 {d14},[r2]

# qhasm: p += 6
# asm 1: add >p=int32#3,<p=int32#3,#6
# asm 2: add >p=r2,<p=r2,#6
add r2,r2,#6

# qhasm: h4 = mem64[p] h4[1]
# asm 1: vld1.8 {<h4=reg128#9%bot},[<p=int32#3]
# asm 2: vld1.8 {<h4=d16},[<p=r2]
vld1.8 {d16},[r2]

# qhasm: p += 4
# asm 1: add >p=int32#3,<p=int32#3,#4
# asm 2: add >p=r2,<p=r2,#4
add r2,r2,#4

# qhasm: h5 = mem64[p] h5[1]
# asm 1: vld1.8 {<h5=reg128#10%bot},[<p=int32#3]
# asm 2: vld1.8 {<h5=d18},[<p=r2]
vld1.8 {d18},[r2]

# qhasm: h6 = mem64[p] h6[1]
# asm 1: vld1.8 {<h6=reg128#11%bot},[<p=int32#3]
# asm 2: vld1.8 {<h6=d20},[<p=r2]
vld1.8 {d20},[r2]

# qhasm: p += 6
# asm 1: add >p=int32#3,<p=int32#3,#6
# asm 2: add >p=r2,<p=r2,#6
add r2,r2,#6

# qhasm: h7 = mem64[p] h7[1]
# asm 1: vld1.8 {<h7=reg128#12%bot},[<p=int32#3]
# asm 2: vld1.8 {<h7=d22},[<p=r2]
vld1.8 {d22},[r2]

# qhasm: p += 2
# asm 1: add >p=int32#3,<p=int32#3,#2
# asm 2: add >p=r2,<p=r2,#2
add r2,r2,#2

# qhasm: h8 = mem64[p] h8[1]
# asm 1: vld1.8 {<h8=reg128#13%bot},[<p=int32#3]
# asm 2: vld1.8 {<h8=d24},[<p=r2]
vld1.8 {d24},[r2]

# qhasm: h9 = mem64[p] h9[1]
# asm 1: vld1.8 {<h9=reg128#14%bot},[<p=int32#3]
# asm 2: vld1.8 {<h9=d26},[<p=r2]
vld1.8 {d26},[r2]

# qhasm: 2x h1 unsigned>>= 26
# asm 1: vshr.u64 >h1=reg128#6,<h1=reg128#6,#26
# asm 2: vshr.u64 >h1=q5,<h1=q5,#26
vshr.u64 q5,q5,#26

# qhasm: 2x h2 unsigned>>= 3
# asm 1: vshr.u64 >h2=reg128#7,<h2=reg128#7,#3
# asm 2: vshr.u64 >h2=q6,<h2=q6,#3
vshr.u64 q6,q6,#3

# qhasm: 2x h3 unsigned>>= 29
# asm 1: vshr.u64 >h3=reg128#8,<h3=reg128#8,#29
# asm 2: vshr.u64 >h3=q7,<h3=q7,#29
vshr.u64 q7,q7,#29

# qhasm: 2x h4 unsigned>>= 6
# asm 1: vshr.u64 >h4=reg128#9,<h4=reg128#9,#6
# asm 2: vshr.u64 >h4=q8,<h4=q8,#6
vshr.u64 q8,q8,#6

# qhasm: 2x h6 unsigned>>= 25
# asm 1: vshr.u64 >h6=reg128#11,<h6=reg128#11,#25
# asm 2: vshr.u64 >h6=q10,<h6=q10,#25
vshr.u64 q10,q10,#25

# qhasm: 2x h7 unsigned>>= 3
# asm 1: vshr.u64 >h7=reg128#12,<h7=reg128#12,#3
# asm 2: vshr.u64 >h7=q11,<h7=q11,#3
vshr.u64 q11,q11,#3

# qhasm: 2x h8 unsigned>>= 12
# asm 1: vshr.u64 >h8=reg128#13,<h8=reg128#13,#12
# asm 2: vshr.u64 >h8=q12,<h8=q12,#12
vshr.u64 q12,q12,#12

# qhasm: 2x h9 unsigned>>= 38
# asm 1: vshr.u64 >h9=reg128#14,<h9=reg128#14,#38
# asm 2: vshr.u64 >h9=q13,<h9=q13,#38
vshr.u64 q13,q13,#38

# qhasm: h0 &= mask26
# asm 1: vand >h0=reg128#5,<h0=reg128#5,<mask26=reg128#3
# asm 2: vand >h0=q4,<h0=q4,<mask26=q2
vand q4,q4,q2

# qhasm: h2 &= mask26
# asm 1: vand >h2=reg128#7,<h2=reg128#7,<mask26=reg128#3
# asm 2: vand >h2=q6,<h2=q6,<mask26=q2
vand q6,q6,q2

# qhasm: h4 &= mask26
# asm 1: vand >h4=reg128#9,<h4=reg128#9,<mask26=reg128#3
# asm 2: vand >h4=q8,<h4=q8,<mask26=q2
vand q8,q8,q2

# qhasm: h6 &= mask26
# asm 1: vand >h6=reg128#11,<h6=reg128#11,<mask26=reg128#3
# asm 2: vand >h6=q10,<h6=q10,<mask26=q2
vand q10,q10,q2

# qhasm: h8 &= mask26
# asm 1: vand >h8=reg128#3,<h8=reg128#13,<mask26=reg128#3
# asm 2: vand >h8=q2,<h8=q12,<mask26=q2
vand q2,q12,q2

# qhasm: h1 &= mask25
# asm 1: vand >h1=reg128#6,<h1=reg128#6,<mask25=reg128#4
# asm 2: vand >h1=q5,<h1=q5,<mask25=q3
vand q5,q5,q3

# qhasm: h3 &= mask25
# asm 1: vand >h3=reg128#8,<h3=reg128#8,<mask25=reg128#4
# asm 2: vand >h3=q7,<h3=q7,<mask25=q3
vand q7,q7,q3

# qhasm: h5 &= mask25
# asm 1: vand >h5=reg128#10,<h5=reg128#10,<mask25=reg128#4
# asm 2: vand >h5=q9,<h5=q9,<mask25=q3
vand q9,q9,q3

# qhasm: h7 &= mask25
# asm 1: vand >h7=reg128#12,<h7=reg128#12,<mask25=reg128#4
# asm 2: vand >h7=q11,<h7=q11,<mask25=q3
vand q11,q11,q3

# qhasm: h9 &= mask25
# asm 1: vand >h9=reg128#4,<h9=reg128#14,<mask25=reg128#4
# asm 2: vand >h9=q3,<h9=q13,<mask25=q3
vand q3,q13,q3

# qhasm: posh = playground1_ptr + 48
# asm 1: add >posh=int32#3,<playground1_ptr=int32#4,#48
# asm 2: add >posh=r2,<playground1_ptr=r3,#48
add r2,r3,#48

# qhasm: 2x t0 = h0 + _0x2000000
# asm 1: vadd.i64 >t0=reg128#13,<h0=reg128#5,<_0x2000000=reg128#2
# asm 2: vadd.i64 >t0=q12,<h0=q4,<_0x2000000=q1
vadd.i64 q12,q4,q1

# qhasm:     2x t6 = h6 + _0x2000000
# asm 1: vadd.i64 >t6=reg128#14,<h6=reg128#11,<_0x2000000=reg128#2
# asm 2: vadd.i64 >t6=q13,<h6=q10,<_0x2000000=q1
vadd.i64 q13,q10,q1

# qhasm: 2x c0 = t0 signed>> 26
# asm 1: vshr.s64 >c0=reg128#13,<t0=reg128#13,#26
# asm 2: vshr.s64 >c0=q12,<t0=q12,#26
vshr.s64 q12,q12,#26

# qhasm:     2x c6 = t6 signed>> 26
# asm 1: vshr.s64 >c6=reg128#14,<t6=reg128#14,#26
# asm 2: vshr.s64 >c6=q13,<t6=q13,#26
vshr.s64 q13,q13,#26

# qhasm: 2x h1 += c0
# asm 1: vadd.i64 >h1=reg128#6,<h1=reg128#6,<c0=reg128#13
# asm 2: vadd.i64 >h1=q5,<h1=q5,<c0=q12
vadd.i64 q5,q5,q12

# qhasm: 2x t0 = c0 << 26
# asm 1: vshl.i64 >t0=reg128#13,<c0=reg128#13,#26
# asm 2: vshl.i64 >t0=q12,<c0=q12,#26
vshl.i64 q12,q12,#26

# qhasm:  2x t1 = h1 + _0x1000000
# asm 1: vadd.i64 >t1=reg128#15,<h1=reg128#6,<_0x1000000=reg128#1
# asm 2: vadd.i64 >t1=q14,<h1=q5,<_0x1000000=q0
vadd.i64 q14,q5,q0

# qhasm:     2x h7 += c6
# asm 1: vadd.i64 >h7=reg128#12,<h7=reg128#12,<c6=reg128#14
# asm 2: vadd.i64 >h7=q11,<h7=q11,<c6=q13
vadd.i64 q11,q11,q13

# qhasm:     2x t6 = c6 << 26
# asm 1: vshl.i64 >t6=reg128#14,<c6=reg128#14,#26
# asm 2: vshl.i64 >t6=q13,<c6=q13,#26
vshl.i64 q13,q13,#26

# qhasm:      2x t7 = h7 + _0x1000000
# asm 1: vadd.i64 >t7=reg128#16,<h7=reg128#12,<_0x1000000=reg128#1
# asm 2: vadd.i64 >t7=q15,<h7=q11,<_0x1000000=q0
vadd.i64 q15,q11,q0

# qhasm: 2x h0 -= t0
# asm 1: vsub.i64 >h0=reg128#5,<h0=reg128#5,<t0=reg128#13
# asm 2: vsub.i64 >h0=q4,<h0=q4,<t0=q12
vsub.i64 q4,q4,q12

# qhasm:  2x c1 = t1 signed>> 25
# asm 1: vshr.s64 >c1=reg128#13,<t1=reg128#15,#25
# asm 2: vshr.s64 >c1=q12,<t1=q14,#25
vshr.s64 q12,q14,#25

# qhasm:     2x h6 -= t6
# asm 1: vsub.i64 >h6=reg128#11,<h6=reg128#11,<t6=reg128#14
# asm 2: vsub.i64 >h6=q10,<h6=q10,<t6=q13
vsub.i64 q10,q10,q13

# qhasm:      2x c7 = t7 signed>> 25
# asm 1: vshr.s64 >c7=reg128#14,<t7=reg128#16,#25
# asm 2: vshr.s64 >c7=q13,<t7=q15,#25
vshr.s64 q13,q15,#25

# qhasm:  2x h2 += c1
# asm 1: vadd.i64 >h2=reg128#7,<h2=reg128#7,<c1=reg128#13
# asm 2: vadd.i64 >h2=q6,<h2=q6,<c1=q12
vadd.i64 q6,q6,q12

# qhasm:  2x t1 = c1 << 25
# asm 1: vshl.i64 >t1=reg128#13,<c1=reg128#13,#25
# asm 2: vshl.i64 >t1=q12,<c1=q12,#25
vshl.i64 q12,q12,#25

# qhasm: 2x t2 = h2 + _0x2000000
# asm 1: vadd.i64 >t2=reg128#15,<h2=reg128#7,<_0x2000000=reg128#2
# asm 2: vadd.i64 >t2=q14,<h2=q6,<_0x2000000=q1
vadd.i64 q14,q6,q1

# qhasm:      2x h8 += c7
# asm 1: vadd.i64 >h8=reg128#3,<h8=reg128#3,<c7=reg128#14
# asm 2: vadd.i64 >h8=q2,<h8=q2,<c7=q13
vadd.i64 q2,q2,q13

# qhasm:  2x h1 -= t1
# asm 1: vsub.i64 >h1=reg128#6,<h1=reg128#6,<t1=reg128#13
# asm 2: vsub.i64 >h1=q5,<h1=q5,<t1=q12
vsub.i64 q5,q5,q12

# qhasm: 2x c2 = t2 signed>> 26
# asm 1: vshr.s64 >c2=reg128#13,<t2=reg128#15,#26
# asm 2: vshr.s64 >c2=q12,<t2=q14,#26
vshr.s64 q12,q14,#26

# qhasm:      2x t7 = c7 << 25
# asm 1: vshl.i64 >t7=reg128#14,<c7=reg128#14,#25
# asm 2: vshl.i64 >t7=q13,<c7=q13,#25
vshl.i64 q13,q13,#25

# qhasm:     2x t8 = h8 + _0x2000000
# asm 1: vadd.i64 >t8=reg128#15,<h8=reg128#3,<_0x2000000=reg128#2
# asm 2: vadd.i64 >t8=q14,<h8=q2,<_0x2000000=q1
vadd.i64 q14,q2,q1

# qhasm: 2x h3 += c2
# asm 1: vadd.i64 >h3=reg128#8,<h3=reg128#8,<c2=reg128#13
# asm 2: vadd.i64 >h3=q7,<h3=q7,<c2=q12
vadd.i64 q7,q7,q12

# qhasm: 2x t2 = c2 << 26
# asm 1: vshl.i64 >t2=reg128#13,<c2=reg128#13,#26
# asm 2: vshl.i64 >t2=q12,<c2=q12,#26
vshl.i64 q12,q12,#26

# qhasm:  2x t3 = h3 + _0x1000000
# asm 1: vadd.i64 >t3=reg128#16,<h3=reg128#8,<_0x1000000=reg128#1
# asm 2: vadd.i64 >t3=q15,<h3=q7,<_0x1000000=q0
vadd.i64 q15,q7,q0

# qhasm:      2x h7 -= t7
# asm 1: vsub.i64 >h7=reg128#12,<h7=reg128#12,<t7=reg128#14
# asm 2: vsub.i64 >h7=q11,<h7=q11,<t7=q13
vsub.i64 q11,q11,q13

# qhasm:     2x c8 = t8 signed>> 26
# asm 1: vshr.s64 >c8=reg128#14,<t8=reg128#15,#26
# asm 2: vshr.s64 >c8=q13,<t8=q14,#26
vshr.s64 q13,q14,#26

# qhasm: 2x h2 -= t2
# asm 1: vsub.i64 >h2=reg128#7,<h2=reg128#7,<t2=reg128#13
# asm 2: vsub.i64 >h2=q6,<h2=q6,<t2=q12
vsub.i64 q6,q6,q12

# qhasm:  2x c3 = t3 signed>> 25
# asm 1: vshr.s64 >c3=reg128#13,<t3=reg128#16,#25
# asm 2: vshr.s64 >c3=q12,<t3=q15,#25
vshr.s64 q12,q15,#25

# qhasm:     2x h9 += c8
# asm 1: vadd.i64 >h9=reg128#4,<h9=reg128#4,<c8=reg128#14
# asm 2: vadd.i64 >h9=q3,<h9=q3,<c8=q13
vadd.i64 q3,q3,q13

# qhasm:     2x t8 = c8 << 26
# asm 1: vshl.i64 >t8=reg128#14,<c8=reg128#14,#26
# asm 2: vshl.i64 >t8=q13,<c8=q13,#26
vshl.i64 q13,q13,#26

# qhasm:      2x t9 = h9 + _0x1000000
# asm 1: vadd.i64 >t9=reg128#15,<h9=reg128#4,<_0x1000000=reg128#1
# asm 2: vadd.i64 >t9=q14,<h9=q3,<_0x1000000=q0
vadd.i64 q14,q3,q0

# qhasm:  2x h4 += c3
# asm 1: vadd.i64 >h4=reg128#9,<h4=reg128#9,<c3=reg128#13
# asm 2: vadd.i64 >h4=q8,<h4=q8,<c3=q12
vadd.i64 q8,q8,q12

# qhasm:  2x t3 = c3 << 25
# asm 1: vshl.i64 >t3=reg128#13,<c3=reg128#13,#25
# asm 2: vshl.i64 >t3=q12,<c3=q12,#25
vshl.i64 q12,q12,#25

# qhasm: 2x t4 = h4 + _0x2000000
# asm 1: vadd.i64 >t4=reg128#16,<h4=reg128#9,<_0x2000000=reg128#2
# asm 2: vadd.i64 >t4=q15,<h4=q8,<_0x2000000=q1
vadd.i64 q15,q8,q1

# qhasm:   posh+=8
# asm 1: add >posh=int32#3,<posh=int32#3,#8
# asm 2: add >posh=r2,<posh=r2,#8
add r2,r2,#8

# qhasm:     2x h8 -= t8
# asm 1: vsub.i64 >h8=reg128#3,<h8=reg128#3,<t8=reg128#14
# asm 2: vsub.i64 >h8=q2,<h8=q2,<t8=q13
vsub.i64 q2,q2,q13

# qhasm:      2x c9 = t9 signed>> 25
# asm 1: vshr.s64 >c9=reg128#14,<t9=reg128#15,#25
# asm 2: vshr.s64 >c9=q13,<t9=q14,#25
vshr.s64 q13,q14,#25

# qhasm:  2x h3 -= t3
# asm 1: vsub.i64 >h3=reg128#8,<h3=reg128#8,<t3=reg128#13
# asm 2: vsub.i64 >h3=q7,<h3=q7,<t3=q12
vsub.i64 q7,q7,q12

# qhasm: 2x c4 = t4 signed>> 26
# asm 1: vshr.s64 >c4=reg128#13,<t4=reg128#16,#26
# asm 2: vshr.s64 >c4=q12,<t4=q15,#26
vshr.s64 q12,q15,#26

# qhasm:      2x s = c9 + c9
# asm 1: vadd.i64 >s=reg128#15,<c9=reg128#14,<c9=reg128#14
# asm 2: vadd.i64 >s=q14,<c9=q13,<c9=q13
vadd.i64 q14,q13,q13

# qhasm: 2x h5 += c4
# asm 1: vadd.i64 >h5=reg128#10,<h5=reg128#10,<c4=reg128#13
# asm 2: vadd.i64 >h5=q9,<h5=q9,<c4=q12
vadd.i64 q9,q9,q12

# qhasm:   h2 h3 = h2[0]h3[0]h2[2]h2[3] h2[1]h3[1]h3[2]h3[3]
# asm 1: vtrn.32 <h2=reg128#7%bot,<h3=reg128#8%bot
# asm 2: vtrn.32 <h2=d12,<h3=d14
vtrn.32 d12,d14

# qhasm: 2x t4 = c4 << 26
# asm 1: vshl.i64 >t4=reg128#13,<c4=reg128#13,#26
# asm 2: vshl.i64 >t4=q12,<c4=q12,#26
vshl.i64 q12,q12,#26

# qhasm:   h2 h3 = h2[0]h2[1]h2[2]h3[2] h3[0]h3[1]h2[3]h3[3]
# asm 1: vtrn.32 <h2=reg128#7%top,<h3=reg128#8%top
# asm 2: vtrn.32 <h2=d13,<h3=d15
vtrn.32 d13,d15

# qhasm:  2x t5 = h5 + _0x1000000
# asm 1: vadd.i64 >t5=reg128#1,<h5=reg128#10,<_0x1000000=reg128#1
# asm 2: vadd.i64 >t5=q0,<h5=q9,<_0x1000000=q0
vadd.i64 q0,q9,q0

# qhasm:      2x h0 += s
# asm 1: vadd.i64 >h0=reg128#5,<h0=reg128#5,<s=reg128#15
# asm 2: vadd.i64 >h0=q4,<h0=q4,<s=q14
vadd.i64 q4,q4,q14

# qhasm:   mem64[posh] aligned= h2[0];posh+=8
# asm 1: vst1.8 <h2=reg128#7%bot,[<posh=int32#3,: 64]!
# asm 2: vst1.8 <h2=d12,[<posh=r2,: 64]!
vst1.8 d12,[r2,: 64]!

# qhasm:      2x s = c9 << 4
# asm 1: vshl.i64 >s=reg128#7,<c9=reg128#14,#4
# asm 2: vshl.i64 >s=q6,<c9=q13,#4
vshl.i64 q6,q13,#4

# qhasm: 2x h4 -= t4
# asm 1: vsub.i64 >h4=reg128#8,<h4=reg128#9,<t4=reg128#13
# asm 2: vsub.i64 >h4=q7,<h4=q8,<t4=q12
vsub.i64 q7,q8,q12

# qhasm:  2x c5 = t5 signed>> 25
# asm 1: vshr.s64 >c5=reg128#1,<t5=reg128#1,#25
# asm 2: vshr.s64 >c5=q0,<t5=q0,#25
vshr.s64 q0,q0,#25

# qhasm:      2x h0 += s
# asm 1: vadd.i64 >h0=reg128#5,<h0=reg128#5,<s=reg128#7
# asm 2: vadd.i64 >h0=q4,<h0=q4,<s=q6
vadd.i64 q4,q4,q6

# qhasm:  2x h6 += c5
# asm 1: vadd.i64 >h6=reg128#7,<h6=reg128#11,<c5=reg128#1
# asm 2: vadd.i64 >h6=q6,<h6=q10,<c5=q0
vadd.i64 q6,q10,q0

# qhasm:  2x t5 = c5 << 25
# asm 1: vshl.i64 >t5=reg128#1,<c5=reg128#1,#25
# asm 2: vshl.i64 >t5=q0,<c5=q0,#25
vshl.i64 q0,q0,#25

# qhasm: 2x t6 = h6 + _0x2000000
# asm 1: vadd.i64 >t6=reg128#9,<h6=reg128#7,<_0x2000000=reg128#2
# asm 2: vadd.i64 >t6=q8,<h6=q6,<_0x2000000=q1
vadd.i64 q8,q6,q1

# qhasm:      2x h0 += c9
# asm 1: vadd.i64 >h0=reg128#5,<h0=reg128#5,<c9=reg128#14
# asm 2: vadd.i64 >h0=q4,<h0=q4,<c9=q13
vadd.i64 q4,q4,q13

# qhasm:      2x t9 = c9 << 25
# asm 1: vshl.i64 >t9=reg128#11,<c9=reg128#14,#25
# asm 2: vshl.i64 >t9=q10,<c9=q13,#25
vshl.i64 q10,q13,#25

# qhasm:     2x t0 = h0 + _0x2000000
# asm 1: vadd.i64 >t0=reg128#2,<h0=reg128#5,<_0x2000000=reg128#2
# asm 2: vadd.i64 >t0=q1,<h0=q4,<_0x2000000=q1
vadd.i64 q1,q4,q1

# qhasm:  2x h5 -= t5
# asm 1: vsub.i64 >h5=reg128#1,<h5=reg128#10,<t5=reg128#1
# asm 2: vsub.i64 >h5=q0,<h5=q9,<t5=q0
vsub.i64 q0,q9,q0

# qhasm: 2x c6 = t6 signed>> 26
# asm 1: vshr.s64 >c6=reg128#9,<t6=reg128#9,#26
# asm 2: vshr.s64 >c6=q8,<t6=q8,#26
vshr.s64 q8,q8,#26

# qhasm:      2x h9 -= t9
# asm 1: vsub.i64 >h9=reg128#4,<h9=reg128#4,<t9=reg128#11
# asm 2: vsub.i64 >h9=q3,<h9=q3,<t9=q10
vsub.i64 q3,q3,q10

# qhasm:   h4 h5 = h4[0]h5[0]h4[2]h4[3] h4[1]h5[1]h5[2]h5[3]
# asm 1: vtrn.32 <h4=reg128#8%bot,<h5=reg128#1%bot
# asm 2: vtrn.32 <h4=d14,<h5=d0
vtrn.32 d14,d0

# qhasm:     2x c0 = t0 signed>> 26
# asm 1: vshr.s64 >c0=reg128#2,<t0=reg128#2,#26
# asm 2: vshr.s64 >c0=q1,<t0=q1,#26
vshr.s64 q1,q1,#26

# qhasm:   h4 h5 = h4[0]h4[1]h4[2]h5[2] h5[0]h5[1]h4[3]h5[3]
# asm 1: vtrn.32 <h4=reg128#8%top,<h5=reg128#1%top
# asm 2: vtrn.32 <h4=d15,<h5=d1
vtrn.32 d15,d1

# qhasm: 2x h7 += c6
# asm 1: vadd.i64 >h7=reg128#1,<h7=reg128#12,<c6=reg128#9
# asm 2: vadd.i64 >h7=q0,<h7=q11,<c6=q8
vadd.i64 q0,q11,q8

# qhasm:   mem64[posh] aligned= h4[0]
# asm 1: vst1.8 <h4=reg128#8%bot,[<posh=int32#3,: 64]
# asm 2: vst1.8 <h4=d14,[<posh=r2,: 64]
vst1.8 d14,[r2,: 64]

# qhasm: 2x t6 = c6 << 26
# asm 1: vshl.i64 >t6=reg128#8,<c6=reg128#9,#26
# asm 2: vshl.i64 >t6=q7,<c6=q8,#26
vshl.i64 q7,q8,#26

# qhasm:     2x h1 += c0
# asm 1: vadd.i64 >h1=reg128#6,<h1=reg128#6,<c0=reg128#2
# asm 2: vadd.i64 >h1=q5,<h1=q5,<c0=q1
vadd.i64 q5,q5,q1

# qhasm:   h8 h9 = h8[0]h9[0]h8[2]h8[3] h8[1]h9[1]h9[2]h9[3]
# asm 1: vtrn.32 <h8=reg128#3%bot,<h9=reg128#4%bot
# asm 2: vtrn.32 <h8=d4,<h9=d6
vtrn.32 d4,d6

# qhasm:     2x t0 = c0 << 26
# asm 1: vshl.i64 >t0=reg128#2,<c0=reg128#2,#26
# asm 2: vshl.i64 >t0=q1,<c0=q1,#26
vshl.i64 q1,q1,#26

# qhasm:   h8 h9 = h8[0]h8[1]h8[2]h9[2] h9[0]h9[1]h8[3]h9[3]
# asm 1: vtrn.32 <h8=reg128#3%top,<h9=reg128#4%top
# asm 2: vtrn.32 <h8=d5,<h9=d7
vtrn.32 d5,d7

# qhasm: 2x h6 -= t6
# asm 1: vsub.i64 >h6=reg128#4,<h6=reg128#7,<t6=reg128#8
# asm 2: vsub.i64 >h6=q3,<h6=q6,<t6=q7
vsub.i64 q3,q6,q7

# qhasm:   posh+=16
# asm 1: add >posh=int32#3,<posh=int32#3,#16
# asm 2: add >posh=r2,<posh=r2,#16
add r2,r2,#16

# qhasm:     2x h0 -= t0
# asm 1: vsub.i64 >h0=reg128#2,<h0=reg128#5,<t0=reg128#2
# asm 2: vsub.i64 >h0=q1,<h0=q4,<t0=q1
vsub.i64 q1,q4,q1

# qhasm:   mem64[posh] aligned= h8[0]
# asm 1: vst1.8 <h8=reg128#3%bot,[<posh=int32#3,: 64]
# asm 2: vst1.8 <h8=d4,[<posh=r2,: 64]
vst1.8 d4,[r2,: 64]

# qhasm:   h6 h7 = h6[0]h7[0]h6[2]h6[3] h6[1]h7[1]h7[2]h7[3]
# asm 1: vtrn.32 <h6=reg128#4%bot,<h7=reg128#1%bot
# asm 2: vtrn.32 <h6=d6,<h7=d0
vtrn.32 d6,d0

# qhasm:   h6 h7 = h6[0]h6[1]h6[2]h7[2] h7[0]h7[1]h6[3]h7[3]
# asm 1: vtrn.32 <h6=reg128#4%top,<h7=reg128#1%top
# asm 2: vtrn.32 <h6=d7,<h7=d1
vtrn.32 d7,d1

# qhasm:   posh-=8
# asm 1: sub >posh=int32#3,<posh=int32#3,#8
# asm 2: sub >posh=r2,<posh=r2,#8
sub r2,r2,#8

# qhasm:   h0 h1 = h0[0]h1[0]h0[2]h0[3] h0[1]h1[1]h1[2]h1[3]
# asm 1: vtrn.32 <h0=reg128#2%bot,<h1=reg128#6%bot
# asm 2: vtrn.32 <h0=d2,<h1=d10
vtrn.32 d2,d10

# qhasm:   h0 h1 = h0[0]h0[1]h0[2]h1[2] h1[0]h1[1]h0[3]h1[3]
# asm 1: vtrn.32 <h0=reg128#2%top,<h1=reg128#6%top
# asm 2: vtrn.32 <h0=d3,<h1=d11
vtrn.32 d3,d11

# qhasm:   mem64[posh] aligned= h6[0]
# asm 1: vst1.8 <h6=reg128#4%bot,[<posh=int32#3,: 64]
# asm 2: vst1.8 <h6=d6,[<posh=r2,: 64]
vst1.8 d6,[r2,: 64]

# qhasm:   posh-=24
# asm 1: sub >posh=int32#3,<posh=int32#3,#24
# asm 2: sub >posh=r2,<posh=r2,#24
sub r2,r2,#24

# qhasm:   mem64[posh] aligned= h0[0]
# asm 1: vst1.8 <h0=reg128#2%bot,[<posh=int32#3,: 64]
# asm 2: vst1.8 <h0=d2,[<posh=r2,: 64]
vst1.8 d2,[r2,: 64]

# qhasm:   posx = playground1_ptr + 96 
# asm 1: add >posx=int32#3,<playground1_ptr=int32#4,#96
# asm 2: add >posx=r2,<playground1_ptr=r3,#96
add r2,r3,#96

# qhasm:   4x zero = 0 
# asm 1: vmov.i32 >zero=reg128#1,#0
# asm 2: vmov.i32 >zero=q0,#0
vmov.i32 q0,#0

# qhasm:   new one 

# qhasm:   one = 0xff,one[1] 
# asm 1: vmov.i64 <one=reg128#2%bot,#0xff
# asm 2: vmov.i64 <one=d2,#0xff
vmov.i64 d2,#0xff

# qhasm:   one = one[0],0 
# asm 1: vmov.i64 <one=reg128#2%top,#0
# asm 2: vmov.i64 <one=d3,#0
vmov.i64 d3,#0

# qhasm:   4x one unsigned>>= 7 
# asm 1: vshr.u32 >one=reg128#2,<one=reg128#2,#7
# asm 2: vshr.u32 >one=q1,<one=q1,#7
vshr.u32 q1,q1,#7

# qhasm:   mem128[posx] aligned= one;posx += 16 
# asm 1: vst1.8 {<one=reg128#2%bot-<one=reg128#2%top},[<posx=int32#3,: 128]!
# asm 2: vst1.8 {<one=d2-<one=d3},[<posx=r2,: 128]!
vst1.8 {d2-d3},[r2,: 128]!

# qhasm:   mem128[posx] aligned= zero;posx += 16 
# asm 1: vst1.8 {<zero=reg128#1%bot-<zero=reg128#1%top},[<posx=int32#3,: 128]!
# asm 2: vst1.8 {<zero=d0-<zero=d1},[<posx=r2,: 128]!
vst1.8 {d0-d1},[r2,: 128]!

# qhasm:   mem64[posx] aligned= zero[0] 
# asm 1: vst1.8 <zero=reg128#1%bot,[<posx=int32#3,: 64]
# asm 2: vst1.8 <zero=d0,[<posx=r2,: 64]
vst1.8 d0,[r2,: 64]

# qhasm:   posx = playground1_ptr + 144 
# asm 1: add >posx=int32#3,<playground1_ptr=int32#4,#144
# asm 2: add >posx=r2,<playground1_ptr=r3,#144
add r2,r3,#144

# qhasm:   4x zero = 0 
# asm 1: vmov.i32 >zero=reg128#1,#0
# asm 2: vmov.i32 >zero=q0,#0
vmov.i32 q0,#0

# qhasm:   mem128[posx] aligned= zero;posx += 16 
# asm 1: vst1.8 {<zero=reg128#1%bot-<zero=reg128#1%top},[<posx=int32#3,: 128]!
# asm 2: vst1.8 {<zero=d0-<zero=d1},[<posx=r2,: 128]!
vst1.8 {d0-d1},[r2,: 128]!

# qhasm:   mem128[posx] aligned= zero;posx += 16 
# asm 1: vst1.8 {<zero=reg128#1%bot-<zero=reg128#1%top},[<posx=int32#3,: 128]!
# asm 2: vst1.8 {<zero=d0-<zero=d1},[<posx=r2,: 128]!
vst1.8 {d0-d1},[r2,: 128]!

# qhasm:   mem64[posx] aligned= zero[0] 
# asm 1: vst1.8 <zero=reg128#1%bot,[<posx=int32#3,: 64]
# asm 2: vst1.8 <zero=d0,[<posx=r2,: 64]
vst1.8 d0,[r2,: 64]

# qhasm:   posx = playground1_ptr + 240 
# asm 1: add >posx=int32#3,<playground1_ptr=int32#4,#240
# asm 2: add >posx=r2,<playground1_ptr=r3,#240
add r2,r3,#240

# qhasm:   4x zero = 0 
# asm 1: vmov.i32 >zero=reg128#1,#0
# asm 2: vmov.i32 >zero=q0,#0
vmov.i32 q0,#0

# qhasm:   new one 

# qhasm:   one = 0xff,one[1] 
# asm 1: vmov.i64 <one=reg128#2%bot,#0xff
# asm 2: vmov.i64 <one=d2,#0xff
vmov.i64 d2,#0xff

# qhasm:   one = one[0],0 
# asm 1: vmov.i64 <one=reg128#2%top,#0
# asm 2: vmov.i64 <one=d3,#0
vmov.i64 d3,#0

# qhasm:   4x one unsigned>>= 7 
# asm 1: vshr.u32 >one=reg128#2,<one=reg128#2,#7
# asm 2: vshr.u32 >one=q1,<one=q1,#7
vshr.u32 q1,q1,#7

# qhasm:   mem128[posx] aligned= one;posx += 16 
# asm 1: vst1.8 {<one=reg128#2%bot-<one=reg128#2%top},[<posx=int32#3,: 128]!
# asm 2: vst1.8 {<one=d2-<one=d3},[<posx=r2,: 128]!
vst1.8 {d2-d3},[r2,: 128]!

# qhasm:   mem128[posx] aligned= zero;posx += 16 
# asm 1: vst1.8 {<zero=reg128#1%bot-<zero=reg128#1%top},[<posx=int32#3,: 128]!
# asm 2: vst1.8 {<zero=d0-<zero=d1},[<posx=r2,: 128]!
vst1.8 {d0-d1},[r2,: 128]!

# qhasm:   mem64[posx] aligned= zero[0] 
# asm 1: vst1.8 <zero=reg128#1%bot,[<posx=int32#3,: 64]
# asm 2: vst1.8 <zero=d0,[<posx=r2,: 64]
vst1.8 d0,[r2,: 64]

# qhasm:   posy = playground1_ptr + 48 
# asm 1: add >posy=int32#3,<playground1_ptr=int32#4,#48
# asm 2: add >posy=r2,<playground1_ptr=r3,#48
add r2,r3,#48

# qhasm:   posx = playground1_ptr + 192 
# asm 1: add >posx=int32#7,<playground1_ptr=int32#4,#192
# asm 2: add >posx=r6,<playground1_ptr=r3,#192
add r6,r3,#192

# qhasm:   f0 aligned= mem128[posy];posy += 16 
# asm 1: vld1.8 {>f0=reg128#1%bot->f0=reg128#1%top},[<posy=int32#3,: 128]!
# asm 2: vld1.8 {>f0=d0->f0=d1},[<posy=r2,: 128]!
vld1.8 {d0-d1},[r2,: 128]!

# qhasm:   f4 aligned= mem128[posy];posy += 16 
# asm 1: vld1.8 {>f4=reg128#2%bot->f4=reg128#2%top},[<posy=int32#3,: 128]!
# asm 2: vld1.8 {>f4=d2->f4=d3},[<posy=r2,: 128]!
vld1.8 {d2-d3},[r2,: 128]!

# qhasm:   new f8 

# qhasm:   f8 aligned= mem64[posy] f8[1] 
# asm 1: vld1.8 {<f8=reg128#3%bot},[<posy=int32#3,: 64]
# asm 2: vld1.8 {<f8=d4},[<posy=r2,: 64]
vld1.8 {d4},[r2,: 64]

# qhasm:   mem128[posx] aligned= f0;posx += 16 
# asm 1: vst1.8 {<f0=reg128#1%bot-<f0=reg128#1%top},[<posx=int32#7,: 128]!
# asm 2: vst1.8 {<f0=d0-<f0=d1},[<posx=r6,: 128]!
vst1.8 {d0-d1},[r6,: 128]!

# qhasm:   mem128[posx] aligned= f4;posx += 16 
# asm 1: vst1.8 {<f4=reg128#2%bot-<f4=reg128#2%top},[<posx=int32#7,: 128]!
# asm 2: vst1.8 {<f4=d2-<f4=d3},[<posx=r6,: 128]!
vst1.8 {d2-d3},[r6,: 128]!

# qhasm:   mem64[posx] aligned= f8[0] 
# asm 1: vst1.8 <f8=reg128#3%bot,[<posx=int32#7,: 64]
# asm 2: vst1.8 <f8=d4,[<posx=r6,: 64]
vst1.8 d4,[r6,: 64]

# qhasm: mainloop:
._mainloop:

# qhasm:   pos8 = (pos unsigned>> 3)
# asm 1: mov >pos8=int32#3,<pos=int32#6,LSR #3
# asm 2: mov >pos8=r2,<pos=r5,LSR #3
mov r2,r5,LSR #3

# qhasm:   pos7 = pos & 7
# asm 1: and >pos7=int32#7,<pos=int32#6,#7
# asm 2: and >pos7=r6,<pos=r5,#7
and r6,r5,#7

# qhasm:   bit = mem8[eptr + pos8]
# asm 1: ldrb >bit=int32#3,[<eptr=int32#2,<pos8=int32#3]
# asm 2: ldrb >bit=r2,[<eptr=r1,<pos8=r2]
ldrb r2,[r1,r2]

# qhasm:   bit unsigned>>= pos7
# asm 1: mov >bit=int32#3,<bit=int32#3,LSR <pos7=int32#7
# asm 2: mov >bit=r2,<bit=r2,LSR <pos7=r6
mov r2,r2,LSR r6

# qhasm:   bit &= 1
# asm 1: and >bit=int32#3,<bit=int32#3,#1
# asm 2: and >bit=r2,<bit=r2,#1
and r2,r2,#1

# qhasm:   pos_stack = pos
# asm 1: str <pos=int32#6,>pos_stack=stack32#3
# asm 2: str <pos=r5,>pos_stack=[sp,#488]
str r5,[sp,#488]

# qhasm:   swap ^= bit
# asm 1: eor >swap=int32#5,<swap=int32#5,<bit=int32#3
# asm 2: eor >swap=r4,<swap=r4,<bit=r2
eor r4,r4,r2

# qhasm:   swap_stack = bit
# asm 1: str <bit=int32#3,>swap_stack=stack32#4
# asm 2: str <bit=r2,>swap_stack=[sp,#492]
str r2,[sp,#492]

# qhasm:   swap = -swap
# asm 1: neg >swap=int32#3,<swap=int32#5
# asm 2: neg >swap=r2,<swap=r4
neg r2,r4

# qhasm:   new f8 

# qhasm:   new g8 

# qhasm:   new F8 

# qhasm:   new G8 

# qhasm:   pos0 = playground1_ptr + 96 
# asm 1: add >pos0=int32#5,<playground1_ptr=int32#4,#96
# asm 2: add >pos0=r4,<playground1_ptr=r3,#96
add r4,r3,#96

# qhasm:   pos1 = playground1_ptr + 192 
# asm 1: add >pos1=int32#6,<playground1_ptr=int32#4,#192
# asm 2: add >pos1=r5,<playground1_ptr=r3,#192
add r5,r3,#192

# qhasm:   pos2 = playground1_ptr + 144 
# asm 1: add >pos2=int32#7,<playground1_ptr=int32#4,#144
# asm 2: add >pos2=r6,<playground1_ptr=r3,#144
add r6,r3,#144

# qhasm:   f0 aligned= mem128[pos0];pos0 += 16 
# asm 1: vld1.8 {>f0=reg128#5%bot->f0=reg128#5%top},[<pos0=int32#5,: 128]!
# asm 2: vld1.8 {>f0=d8->f0=d9},[<pos0=r4,: 128]!
vld1.8 {d8-d9},[r4,: 128]!

# qhasm:   pos3 = playground1_ptr + 240 
# asm 1: add >pos3=int32#8,<playground1_ptr=int32#4,#240
# asm 2: add >pos3=r7,<playground1_ptr=r3,#240
add r7,r3,#240

# qhasm:   g0 aligned= mem128[pos1];pos1 += 16 
# asm 1: vld1.8 {>g0=reg128#6%bot->g0=reg128#6%top},[<pos1=int32#6,: 128]!
# asm 2: vld1.8 {>g0=d10->g0=d11},[<pos1=r5,: 128]!
vld1.8 {d10-d11},[r5,: 128]!

# qhasm:   x0 = f0 ^ g0 
# asm 1: veor >x0=reg128#7,<f0=reg128#5,<g0=reg128#6
# asm 2: veor >x0=q6,<f0=q4,<g0=q5
veor q6,q4,q5

# qhasm:   F0 aligned= mem128[pos2];pos2 += 16 
# asm 1: vld1.8 {>F0=reg128#8%bot->F0=reg128#8%top},[<pos2=int32#7,: 128]!
# asm 2: vld1.8 {>F0=d14->F0=d15},[<pos2=r6,: 128]!
vld1.8 {d14-d15},[r6,: 128]!

# qhasm:   b = swap,swap,swap,swap 
# asm 1: vdup.i32 >b=reg128#9,<swap=int32#3
# asm 2: vdup.i32 >b=q8,<swap=r2
vdup.i32 q8,r2

# qhasm:   G0 aligned= mem128[pos3];pos3 += 16 
# asm 1: vld1.8 {>G0=reg128#10%bot->G0=reg128#10%top},[<pos3=int32#8,: 128]!
# asm 2: vld1.8 {>G0=d18->G0=d19},[<pos3=r7,: 128]!
vld1.8 {d18-d19},[r7,: 128]!

# qhasm:   X0 = F0 ^ G0 
# asm 1: veor >X0=reg128#11,<F0=reg128#8,<G0=reg128#10
# asm 2: veor >X0=q10,<F0=q7,<G0=q9
veor q10,q7,q9

# qhasm:   f4 aligned= mem128[pos0];pos0 += 16 
# asm 1: vld1.8 {>f4=reg128#12%bot->f4=reg128#12%top},[<pos0=int32#5,: 128]!
# asm 2: vld1.8 {>f4=d22->f4=d23},[<pos0=r4,: 128]!
vld1.8 {d22-d23},[r4,: 128]!

# qhasm:   x0 &= b 
# asm 1: vand >x0=reg128#7,<x0=reg128#7,<b=reg128#9
# asm 2: vand >x0=q6,<x0=q6,<b=q8
vand q6,q6,q8

# qhasm:   g4 aligned= mem128[pos1];pos1 += 16 
# asm 1: vld1.8 {>g4=reg128#13%bot->g4=reg128#13%top},[<pos1=int32#6,: 128]!
# asm 2: vld1.8 {>g4=d24->g4=d25},[<pos1=r5,: 128]!
vld1.8 {d24-d25},[r5,: 128]!

# qhasm:   X0 &= b 
# asm 1: vand >X0=reg128#11,<X0=reg128#11,<b=reg128#9
# asm 2: vand >X0=q10,<X0=q10,<b=q8
vand q10,q10,q8

# qhasm:   F4 aligned= mem128[pos2];pos2 += 16 
# asm 1: vld1.8 {>F4=reg128#14%bot->F4=reg128#14%top},[<pos2=int32#7,: 128]!
# asm 2: vld1.8 {>F4=d26->F4=d27},[<pos2=r6,: 128]!
vld1.8 {d26-d27},[r6,: 128]!

# qhasm:   f0 ^= x0 
# asm 1: veor >f0=reg128#5,<f0=reg128#5,<x0=reg128#7
# asm 2: veor >f0=q4,<f0=q4,<x0=q6
veor q4,q4,q6

# qhasm:   G4 aligned= mem128[pos3];pos3 += 16 
# asm 1: vld1.8 {>G4=reg128#15%bot->G4=reg128#15%top},[<pos3=int32#8,: 128]!
# asm 2: vld1.8 {>G4=d28->G4=d29},[<pos3=r7,: 128]!
vld1.8 {d28-d29},[r7,: 128]!

# qhasm:   g0 ^= x0 
# asm 1: veor >g0=reg128#6,<g0=reg128#6,<x0=reg128#7
# asm 2: veor >g0=q5,<g0=q5,<x0=q6
veor q5,q5,q6

# qhasm:   f8 aligned= mem64[pos0] f8[1] 
# asm 1: vld1.8 {<f8=reg128#1%bot},[<pos0=int32#5,: 64]
# asm 2: vld1.8 {<f8=d0},[<pos0=r4,: 64]
vld1.8 {d0},[r4,: 64]

# qhasm:   F0 ^= X0 
# asm 1: veor >F0=reg128#7,<F0=reg128#8,<X0=reg128#11
# asm 2: veor >F0=q6,<F0=q7,<X0=q10
veor q6,q7,q10

# qhasm:   g8 aligned= mem64[pos1] g8[1] 
# asm 1: vld1.8 {<g8=reg128#2%bot},[<pos1=int32#6,: 64]
# asm 2: vld1.8 {<g8=d2},[<pos1=r5,: 64]
vld1.8 {d2},[r5,: 64]

# qhasm:   G0 ^= X0 
# asm 1: veor >G0=reg128#8,<G0=reg128#10,<X0=reg128#11
# asm 2: veor >G0=q7,<G0=q9,<X0=q10
veor q7,q9,q10

# qhasm:   F8 aligned= mem64[pos2] F8[1] 
# asm 1: vld1.8 {<F8=reg128#3%bot},[<pos2=int32#7,: 64]
# asm 2: vld1.8 {<F8=d4},[<pos2=r6,: 64]
vld1.8 {d4},[r6,: 64]

# qhasm:   x4 = f4 ^ g4 
# asm 1: veor >x4=reg128#10,<f4=reg128#12,<g4=reg128#13
# asm 2: veor >x4=q9,<f4=q11,<g4=q12
veor q9,q11,q12

# qhasm:   G8 aligned= mem64[pos3] G8[1] 
# asm 1: vld1.8 {<G8=reg128#4%bot},[<pos3=int32#8,: 64]
# asm 2: vld1.8 {<G8=d6},[<pos3=r7,: 64]
vld1.8 {d6},[r7,: 64]

# qhasm:   x8 = f8 ^ g8 
# asm 1: veor >x8=reg128#11,<f8=reg128#1,<g8=reg128#2
# asm 2: veor >x8=q10,<f8=q0,<g8=q1
veor q10,q0,q1

# qhasm:   pos0 -= 32 
# asm 1: sub >pos0=int32#3,<pos0=int32#5,#32
# asm 2: sub >pos0=r2,<pos0=r4,#32
sub r2,r4,#32

# qhasm:   x4 &= b 
# asm 1: vand >x4=reg128#10,<x4=reg128#10,<b=reg128#9
# asm 2: vand >x4=q9,<x4=q9,<b=q8
vand q9,q9,q8

# qhasm:   pos1 -= 32 
# asm 1: sub >pos1=int32#5,<pos1=int32#6,#32
# asm 2: sub >pos1=r4,<pos1=r5,#32
sub r4,r5,#32

# qhasm:   x8 &= b 
# asm 1: vand >x8=reg128#11,<x8=reg128#11,<b=reg128#9
# asm 2: vand >x8=q10,<x8=q10,<b=q8
vand q10,q10,q8

# qhasm:   pos2 -= 32 
# asm 1: sub >pos2=int32#6,<pos2=int32#7,#32
# asm 2: sub >pos2=r5,<pos2=r6,#32
sub r5,r6,#32

# qhasm:   f4 ^= x4 
# asm 1: veor >f4=reg128#12,<f4=reg128#12,<x4=reg128#10
# asm 2: veor >f4=q11,<f4=q11,<x4=q9
veor q11,q11,q9

# qhasm:   pos3 -= 32 
# asm 1: sub >pos3=int32#7,<pos3=int32#8,#32
# asm 2: sub >pos3=r6,<pos3=r7,#32
sub r6,r7,#32

# qhasm:   f8 ^= x8 
# asm 1: veor >f8=reg128#1,<f8=reg128#1,<x8=reg128#11
# asm 2: veor >f8=q0,<f8=q0,<x8=q10
veor q0,q0,q10

# qhasm:   g4 ^= x4 
# asm 1: veor >g4=reg128#10,<g4=reg128#13,<x4=reg128#10
# asm 2: veor >g4=q9,<g4=q12,<x4=q9
veor q9,q12,q9

# qhasm:   g8 ^= x8 
# asm 1: veor >g8=reg128#2,<g8=reg128#2,<x8=reg128#11
# asm 2: veor >g8=q1,<g8=q1,<x8=q10
veor q1,q1,q10

# qhasm:   X4 = F4 ^ G4 
# asm 1: veor >X4=reg128#11,<F4=reg128#14,<G4=reg128#15
# asm 2: veor >X4=q10,<F4=q13,<G4=q14
veor q10,q13,q14

# qhasm:   X8 = F8 ^ G8 
# asm 1: veor >X8=reg128#13,<F8=reg128#3,<G8=reg128#4
# asm 2: veor >X8=q12,<F8=q2,<G8=q3
veor q12,q2,q3

# qhasm:   X4 &= b 
# asm 1: vand >X4=reg128#11,<X4=reg128#11,<b=reg128#9
# asm 2: vand >X4=q10,<X4=q10,<b=q8
vand q10,q10,q8

# qhasm:   X8 &= b 
# asm 1: vand >X8=reg128#9,<X8=reg128#13,<b=reg128#9
# asm 2: vand >X8=q8,<X8=q12,<b=q8
vand q8,q12,q8

# qhasm:   F4 ^= X4 
# asm 1: veor >F4=reg128#13,<F4=reg128#14,<X4=reg128#11
# asm 2: veor >F4=q12,<F4=q13,<X4=q10
veor q12,q13,q10

# qhasm:   F8 ^= X8 
# asm 1: veor >F8=reg128#3,<F8=reg128#3,<X8=reg128#9
# asm 2: veor >F8=q2,<F8=q2,<X8=q8
veor q2,q2,q8

# qhasm:   G4 ^= X4 
# asm 1: veor >G4=reg128#11,<G4=reg128#15,<X4=reg128#11
# asm 2: veor >G4=q10,<G4=q14,<X4=q10
veor q10,q14,q10

# qhasm:   G8 ^= X8 
# asm 1: veor >G8=reg128#4,<G8=reg128#4,<X8=reg128#9
# asm 2: veor >G8=q3,<G8=q3,<X8=q8
veor q3,q3,q8

# qhasm:   4x f0plusF0 = f0 + F0 
# asm 1: vadd.i32 >f0plusF0=reg128#9,<f0=reg128#5,<F0=reg128#7
# asm 2: vadd.i32 >f0plusF0=q8,<f0=q4,<F0=q6
vadd.i32 q8,q4,q6

# qhasm:   4x f0minusF0 = f0 - F0 
# asm 1: vsub.i32 >f0minusF0=reg128#5,<f0=reg128#5,<F0=reg128#7
# asm 2: vsub.i32 >f0minusF0=q4,<f0=q4,<F0=q6
vsub.i32 q4,q4,q6

# qhasm:   mem128[pos0] aligned= f0plusF0;pos0 += 16 
# asm 1: vst1.8 {<f0plusF0=reg128#9%bot-<f0plusF0=reg128#9%top},[<pos0=int32#3,: 128]!
# asm 2: vst1.8 {<f0plusF0=d16-<f0plusF0=d17},[<pos0=r2,: 128]!
vst1.8 {d16-d17},[r2,: 128]!

# qhasm:   4x f4plusF4 = f4 + F4 
# asm 1: vadd.i32 >f4plusF4=reg128#7,<f4=reg128#12,<F4=reg128#13
# asm 2: vadd.i32 >f4plusF4=q6,<f4=q11,<F4=q12
vadd.i32 q6,q11,q12

# qhasm:   mem128[pos2] aligned= f0minusF0;pos2 += 16 
# asm 1: vst1.8 {<f0minusF0=reg128#5%bot-<f0minusF0=reg128#5%top},[<pos2=int32#6,: 128]!
# asm 2: vst1.8 {<f0minusF0=d8-<f0minusF0=d9},[<pos2=r5,: 128]!
vst1.8 {d8-d9},[r5,: 128]!

# qhasm:   4x f4minusF4 = f4 - F4 
# asm 1: vsub.i32 >f4minusF4=reg128#5,<f4=reg128#12,<F4=reg128#13
# asm 2: vsub.i32 >f4minusF4=q4,<f4=q11,<F4=q12
vsub.i32 q4,q11,q12

# qhasm:   mem128[pos0] aligned= f4plusF4;pos0 += 16 
# asm 1: vst1.8 {<f4plusF4=reg128#7%bot-<f4plusF4=reg128#7%top},[<pos0=int32#3,: 128]!
# asm 2: vst1.8 {<f4plusF4=d12-<f4plusF4=d13},[<pos0=r2,: 128]!
vst1.8 {d12-d13},[r2,: 128]!

# qhasm:   4x f8plusF8 = f8 + F8 
# asm 1: vadd.i32 >f8plusF8=reg128#7,<f8=reg128#1,<F8=reg128#3
# asm 2: vadd.i32 >f8plusF8=q6,<f8=q0,<F8=q2
vadd.i32 q6,q0,q2

# qhasm:   mem128[pos2] aligned= f4minusF4;pos2 += 16 
# asm 1: vst1.8 {<f4minusF4=reg128#5%bot-<f4minusF4=reg128#5%top},[<pos2=int32#6,: 128]!
# asm 2: vst1.8 {<f4minusF4=d8-<f4minusF4=d9},[<pos2=r5,: 128]!
vst1.8 {d8-d9},[r5,: 128]!

# qhasm:   4x f8minusF8 = f8 - F8 
# asm 1: vsub.i32 >f8minusF8=reg128#1,<f8=reg128#1,<F8=reg128#3
# asm 2: vsub.i32 >f8minusF8=q0,<f8=q0,<F8=q2
vsub.i32 q0,q0,q2

# qhasm:   mem64[pos0] aligned= f8plusF8[0] 
# asm 1: vst1.8 <f8plusF8=reg128#7%bot,[<pos0=int32#3,: 64]
# asm 2: vst1.8 <f8plusF8=d12,[<pos0=r2,: 64]
vst1.8 d12,[r2,: 64]

# qhasm:   4x g0plusG0 = g0 + G0 
# asm 1: vadd.i32 >g0plusG0=reg128#3,<g0=reg128#6,<G0=reg128#8
# asm 2: vadd.i32 >g0plusG0=q2,<g0=q5,<G0=q7
vadd.i32 q2,q5,q7

# qhasm:   mem64[pos2] aligned= f8minusF8[0] 
# asm 1: vst1.8 <f8minusF8=reg128#1%bot,[<pos2=int32#6,: 64]
# asm 2: vst1.8 <f8minusF8=d0,[<pos2=r5,: 64]
vst1.8 d0,[r5,: 64]

# qhasm:   4x g0minusG0 = g0 - G0 
# asm 1: vsub.i32 >g0minusG0=reg128#1,<g0=reg128#6,<G0=reg128#8
# asm 2: vsub.i32 >g0minusG0=q0,<g0=q5,<G0=q7
vsub.i32 q0,q5,q7

# qhasm:   mem128[pos1] aligned= g0plusG0;pos1 += 16 
# asm 1: vst1.8 {<g0plusG0=reg128#3%bot-<g0plusG0=reg128#3%top},[<pos1=int32#5,: 128]!
# asm 2: vst1.8 {<g0plusG0=d4-<g0plusG0=d5},[<pos1=r4,: 128]!
vst1.8 {d4-d5},[r4,: 128]!

# qhasm:   4x g4plusG4 = g4 + G4 
# asm 1: vadd.i32 >g4plusG4=reg128#3,<g4=reg128#10,<G4=reg128#11
# asm 2: vadd.i32 >g4plusG4=q2,<g4=q9,<G4=q10
vadd.i32 q2,q9,q10

# qhasm:   mem128[pos3] aligned= g0minusG0;pos3 += 16 
# asm 1: vst1.8 {<g0minusG0=reg128#1%bot-<g0minusG0=reg128#1%top},[<pos3=int32#7,: 128]!
# asm 2: vst1.8 {<g0minusG0=d0-<g0minusG0=d1},[<pos3=r6,: 128]!
vst1.8 {d0-d1},[r6,: 128]!

# qhasm:   4x g4minusG4 = g4 - G4 
# asm 1: vsub.i32 >g4minusG4=reg128#1,<g4=reg128#10,<G4=reg128#11
# asm 2: vsub.i32 >g4minusG4=q0,<g4=q9,<G4=q10
vsub.i32 q0,q9,q10

# qhasm:   mem128[pos1] aligned= g4plusG4;pos1 += 16 
# asm 1: vst1.8 {<g4plusG4=reg128#3%bot-<g4plusG4=reg128#3%top},[<pos1=int32#5,: 128]!
# asm 2: vst1.8 {<g4plusG4=d4-<g4plusG4=d5},[<pos1=r4,: 128]!
vst1.8 {d4-d5},[r4,: 128]!

# qhasm:   4x g8plusG8 = g8 + G8 
# asm 1: vadd.i32 >g8plusG8=reg128#3,<g8=reg128#2,<G8=reg128#4
# asm 2: vadd.i32 >g8plusG8=q2,<g8=q1,<G8=q3
vadd.i32 q2,q1,q3

# qhasm:   mem128[pos3] aligned= g4minusG4;pos3 += 16 
# asm 1: vst1.8 {<g4minusG4=reg128#1%bot-<g4minusG4=reg128#1%top},[<pos3=int32#7,: 128]!
# asm 2: vst1.8 {<g4minusG4=d0-<g4minusG4=d1},[<pos3=r6,: 128]!
vst1.8 {d0-d1},[r6,: 128]!

# qhasm:   4x g8minusG8 = g8 - G8 
# asm 1: vsub.i32 >g8minusG8=reg128#1,<g8=reg128#2,<G8=reg128#4
# asm 2: vsub.i32 >g8minusG8=q0,<g8=q1,<G8=q3
vsub.i32 q0,q1,q3

# qhasm:   mem64[pos1] aligned= g8plusG8[0] 
# asm 1: vst1.8 <g8plusG8=reg128#3%bot,[<pos1=int32#5,: 64]
# asm 2: vst1.8 <g8plusG8=d4,[<pos1=r4,: 64]
vst1.8 d4,[r4,: 64]

# qhasm:   mem64[pos3] aligned= g8minusG8[0] 
# asm 1: vst1.8 <g8minusG8=reg128#1%bot,[<pos3=int32#7,: 64]
# asm 2: vst1.8 <g8minusG8=d0,[<pos3=r6,: 64]
vst1.8 d0,[r6,: 64]

# qhasm:   ptr = &_19_19_38_38_stack 
# asm 1: lea >ptr=int32#3,<_19_19_38_38_stack=stack128#3
# asm 2: lea >ptr=r2,<_19_19_38_38_stack=[sp,#544]
add r2,sp,#544

# qhasm:   posf = playground1_ptr + 96 
# asm 1: add >posf=int32#5,<playground1_ptr=int32#4,#96
# asm 2: add >posf=r4,<playground1_ptr=r3,#96
add r4,r3,#96

# qhasm:   posF = playground1_ptr + 144 
# asm 1: add >posF=int32#6,<playground1_ptr=int32#4,#144
# asm 2: add >posF=r5,<playground1_ptr=r3,#144
add r5,r3,#144

# qhasm:   _19_19_38_38 aligned= mem128[ptr] 
# asm 1: vld1.8 {>_19_19_38_38=reg128#1%bot->_19_19_38_38=reg128#1%top},[<ptr=int32#3,: 128]
# asm 2: vld1.8 {>_19_19_38_38=d0->_19_19_38_38=d1},[<ptr=r2,: 128]
vld1.8 {d0-d1},[r2,: 128]

# qhasm:   fg01 aligned= mem128[posf];posf+=16 
# asm 1: vld1.8 {>fg01=reg128#2%bot->fg01=reg128#2%top},[<posf=int32#5,: 128]!
# asm 2: vld1.8 {>fg01=d2->fg01=d3},[<posf=r4,: 128]!
vld1.8 {d2-d3},[r4,: 128]!

# qhasm:   fg23 aligned= mem128[posF];posF+=16 
# asm 1: vld1.8 {>fg23=reg128#3%bot->fg23=reg128#3%top},[<posF=int32#6,: 128]!
# asm 2: vld1.8 {>fg23=d4->fg23=d5},[<posF=r5,: 128]!
vld1.8 {d4-d5},[r5,: 128]!

# qhasm:   fg01[0,1,2,3] fg23[0,1,2,3] = fg01[0]fg23[0]fg01[1]fg23[1] fg01[2]fg23[2]fg01[3]fg23[3] 
# asm 1: vzip.i32 <fg01=reg128#2,<fg23=reg128#3
# asm 2: vzip.i32 <fg01=q1,<fg23=q2
vzip.i32 q1,q2

# qhasm:   fg45 aligned= mem128[posf];posf+=16 
# asm 1: vld1.8 {>fg45=reg128#4%bot->fg45=reg128#4%top},[<posf=int32#5,: 128]!
# asm 2: vld1.8 {>fg45=d6->fg45=d7},[<posf=r4,: 128]!
vld1.8 {d6-d7},[r4,: 128]!

# qhasm:   fg67 aligned= mem128[posF];posF+=16 
# asm 1: vld1.8 {>fg67=reg128#5%bot->fg67=reg128#5%top},[<posF=int32#6,: 128]!
# asm 2: vld1.8 {>fg67=d8->fg67=d9},[<posF=r5,: 128]!
vld1.8 {d8-d9},[r5,: 128]!

# qhasm:   4x fg01_2 = fg01 << 1 
# asm 1: vshl.i32 >fg01_2=reg128#6,<fg01=reg128#2,#1
# asm 2: vshl.i32 >fg01_2=q5,<fg01=q1,#1
vshl.i32 q5,q1,#1

# qhasm:   fg45[0,1,2,3] fg67[0,1,2,3] = fg45[0]fg67[0]fg45[1]fg67[1] fg45[2]fg67[2]fg45[3]fg67[3] 
# asm 1: vzip.i32 <fg45=reg128#4,<fg67=reg128#5
# asm 2: vzip.i32 <fg45=q3,<fg67=q4
vzip.i32 q3,q4

# qhasm:   4x fg23_2 = fg23 << 1 
# asm 1: vshl.i32 >fg23_2=reg128#7,<fg23=reg128#3,#1
# asm 2: vshl.i32 >fg23_2=q6,<fg23=q2,#1
vshl.i32 q6,q2,#1

# qhasm:   new fg89 

# qhasm:   fg89 aligned= mem64[posf]fg89[1] 
# asm 1: vld1.8 {<fg89=reg128#8%bot},[<posf=int32#5,: 64]
# asm 2: vld1.8 {<fg89=d14},[<posf=r4,: 64]
vld1.8 {d14},[r4,: 64]

# qhasm:   4x fg45_2 = fg45 << 1 
# asm 1: vshl.i32 >fg45_2=reg128#9,<fg45=reg128#4,#1
# asm 2: vshl.i32 >fg45_2=q8,<fg45=q3,#1
vshl.i32 q8,q3,#1

# qhasm:   fg89 aligned= fg89[0]mem64[posF] 
# asm 1: vld1.8 {<fg89=reg128#8%top},[<posF=int32#6,: 64]
# asm 2: vld1.8 {<fg89=d15},[<posF=r5,: 64]
vld1.8 {d15},[r5,: 64]

# qhasm:   4x fg67_2 = fg67 << 1 
# asm 1: vshl.i32 >fg67_2=reg128#10,<fg67=reg128#5,#1
# asm 2: vshl.i32 >fg67_2=q9,<fg67=q4,#1
vshl.i32 q9,q4,#1

# qhasm:   fg45_19_38[0,1] = fg45_19_38[0,1];fg45_19_38[2] = fg45[2] * _19_19_38_38[2];fg45_19_38[3] = fg45[3] * _19_19_38_38[3] 
# asm 1: vmul.i32 >fg45_19_38=reg128#11%top,<fg45=reg128#4%top,<_19_19_38_38=reg128#1%top
# asm 2: vmul.i32 >fg45_19_38=d21,<fg45=d7,<_19_19_38_38=d1
vmul.i32 d21,d7,d1

# qhasm:   fg89 = fg89[0,2,1,3] 
# asm 1: vtrn.32 <fg89=reg128#8%bot,<fg89=reg128#8%top
# asm 2: vtrn.32 <fg89=d14,<fg89=d15
vtrn.32 d14,d15

# qhasm:   4x fg67_19_38 = fg67 * _19_19_38_38 
# asm 1: vmul.i32 >fg67_19_38=reg128#12,<fg67=reg128#5,<_19_19_38_38=reg128#1
# asm 2: vmul.i32 >fg67_19_38=q11,<fg67=q4,<_19_19_38_38=q0
vmul.i32 q11,q4,q0

# qhasm:   4x fg89_19_38 = fg89 * _19_19_38_38 
# asm 1: vmul.i32 >fg89_19_38=reg128#1,<fg89=reg128#8,<_19_19_38_38=reg128#1
# asm 2: vmul.i32 >fg89_19_38=q0,<fg89=q7,<_19_19_38_38=q0
vmul.i32 q0,q7,q0

# qhasm:   h0[0,1] = fg01[0] signed* fg01[0];h0[2,3] = fg01[1] signed* fg01[1] 
# asm 1: vmull.s32 >h0=reg128#13,<fg01=reg128#2%bot,<fg01=reg128#2%bot
# asm 2: vmull.s32 >h0=q12,<fg01=d2,<fg01=d2
vmull.s32 q12,d2,d2

# qhasm:   h0[0,1] += fg01_2[2] signed* fg89_19_38[2];h0[2,3] += fg01_2[3] signed* fg89_19_38[3] 
# asm 1: vmlal.s32 <h0=reg128#13,<fg01_2=reg128#6%top,<fg89_19_38=reg128#1%top
# asm 2: vmlal.s32 <h0=q12,<fg01_2=d11,<fg89_19_38=d1
vmlal.s32 q12,d11,d1

# qhasm:   h0[0,1] += fg23_2[0] signed* fg89_19_38[0];h0[2,3] += fg23_2[1] signed* fg89_19_38[1] 
# asm 1: vmlal.s32 <h0=reg128#13,<fg23_2=reg128#7%bot,<fg89_19_38=reg128#1%bot
# asm 2: vmlal.s32 <h0=q12,<fg23_2=d12,<fg89_19_38=d0
vmlal.s32 q12,d12,d0

# qhasm:   h0[0,1] += fg23_2[2] signed* fg67_19_38[2];h0[2,3] += fg23_2[3] signed* fg67_19_38[3] 
# asm 1: vmlal.s32 <h0=reg128#13,<fg23_2=reg128#7%top,<fg67_19_38=reg128#12%top
# asm 2: vmlal.s32 <h0=q12,<fg23_2=d13,<fg67_19_38=d23
vmlal.s32 q12,d13,d23

# qhasm:   h0[0,1] += fg45_2[0] signed* fg67_19_38[0];h0[2,3] += fg45_2[1] signed* fg67_19_38[1] 
# asm 1: vmlal.s32 <h0=reg128#13,<fg45_2=reg128#9%bot,<fg67_19_38=reg128#12%bot
# asm 2: vmlal.s32 <h0=q12,<fg45_2=d16,<fg67_19_38=d22
vmlal.s32 q12,d16,d22

# qhasm:   h0[0,1] += fg45[2] signed* fg45_19_38[2];h0[2,3] += fg45[3] signed* fg45_19_38[3] 
# asm 1: vmlal.s32 <h0=reg128#13,<fg45=reg128#4%top,<fg45_19_38=reg128#11%top
# asm 2: vmlal.s32 <h0=q12,<fg45=d7,<fg45_19_38=d21
vmlal.s32 q12,d7,d21

# qhasm:   h1[0,1] = fg01[0] signed* fg01_2[2];h1[2,3] = fg01[1] signed* fg01_2[3] 
# asm 1: vmull.s32 >h1=reg128#11,<fg01=reg128#2%bot,<fg01_2=reg128#6%top
# asm 2: vmull.s32 >h1=q10,<fg01=d2,<fg01_2=d11
vmull.s32 q10,d2,d11

# qhasm:   h1[0,1] += fg23[0] signed* fg89_19_38[2];h1[2,3] += fg23[1] signed* fg89_19_38[3] 
# asm 1: vmlal.s32 <h1=reg128#11,<fg23=reg128#3%bot,<fg89_19_38=reg128#1%top
# asm 2: vmlal.s32 <h1=q10,<fg23=d4,<fg89_19_38=d1
vmlal.s32 q10,d4,d1

# qhasm:   h1[0,1] += fg23_2[2] signed* fg89_19_38[0];h1[2,3] += fg23_2[3] signed* fg89_19_38[1] 
# asm 1: vmlal.s32 <h1=reg128#11,<fg23_2=reg128#7%top,<fg89_19_38=reg128#1%bot
# asm 2: vmlal.s32 <h1=q10,<fg23_2=d13,<fg89_19_38=d0
vmlal.s32 q10,d13,d0

# qhasm:   h1[0,1] += fg45[0] signed* fg67_19_38[2];h1[2,3] += fg45[1] signed* fg67_19_38[3] 
# asm 1: vmlal.s32 <h1=reg128#11,<fg45=reg128#4%bot,<fg67_19_38=reg128#12%top
# asm 2: vmlal.s32 <h1=q10,<fg45=d6,<fg67_19_38=d23
vmlal.s32 q10,d6,d23

# qhasm:   h1[0,1] += fg45_2[2] signed* fg67_19_38[0];h1[2,3] += fg45_2[3] signed* fg67_19_38[1] 
# asm 1: vmlal.s32 <h1=reg128#11,<fg45_2=reg128#9%top,<fg67_19_38=reg128#12%bot
# asm 2: vmlal.s32 <h1=q10,<fg45_2=d17,<fg67_19_38=d22
vmlal.s32 q10,d17,d22

# qhasm:   h2[0,1] = fg01_2[0] signed* fg23[0];h2[2,3] = fg01_2[1] signed* fg23[1] 
# asm 1: vmull.s32 >h2=reg128#14,<fg01_2=reg128#6%bot,<fg23=reg128#3%bot
# asm 2: vmull.s32 >h2=q13,<fg01_2=d10,<fg23=d4
vmull.s32 q13,d10,d4

# qhasm:   h2[0,1] += fg01_2[2] signed* fg01[2];h2[2,3] += fg01_2[3] signed* fg01[3] 
# asm 1: vmlal.s32 <h2=reg128#14,<fg01_2=reg128#6%top,<fg01=reg128#2%top
# asm 2: vmlal.s32 <h2=q13,<fg01_2=d11,<fg01=d3
vmlal.s32 q13,d11,d3

# qhasm:   h2[0,1] += fg23_2[2] signed* fg89_19_38[2];h2[2,3] += fg23_2[3] signed* fg89_19_38[3] 
# asm 1: vmlal.s32 <h2=reg128#14,<fg23_2=reg128#7%top,<fg89_19_38=reg128#1%top
# asm 2: vmlal.s32 <h2=q13,<fg23_2=d13,<fg89_19_38=d1
vmlal.s32 q13,d13,d1

# qhasm:   h2[0,1] += fg45_2[0] signed* fg89_19_38[0];h2[2,3] += fg45_2[1] signed* fg89_19_38[1] 
# asm 1: vmlal.s32 <h2=reg128#14,<fg45_2=reg128#9%bot,<fg89_19_38=reg128#1%bot
# asm 2: vmlal.s32 <h2=q13,<fg45_2=d16,<fg89_19_38=d0
vmlal.s32 q13,d16,d0

# qhasm:   h2[0,1] += fg45_2[2] signed* fg67_19_38[2];h2[2,3] += fg45_2[3] signed* fg67_19_38[3] 
# asm 1: vmlal.s32 <h2=reg128#14,<fg45_2=reg128#9%top,<fg67_19_38=reg128#12%top
# asm 2: vmlal.s32 <h2=q13,<fg45_2=d17,<fg67_19_38=d23
vmlal.s32 q13,d17,d23

# qhasm:   h2[0,1] += fg67[0] signed* fg67_19_38[0];h2[2,3] += fg67[1] signed* fg67_19_38[1] 
# asm 1: vmlal.s32 <h2=reg128#14,<fg67=reg128#5%bot,<fg67_19_38=reg128#12%bot
# asm 2: vmlal.s32 <h2=q13,<fg67=d8,<fg67_19_38=d22
vmlal.s32 q13,d8,d22

# qhasm:   h3[0,1] = fg01_2[0] signed* fg23[2];h3[2,3] = fg01_2[1] signed* fg23[3] 
# asm 1: vmull.s32 >h3=reg128#2,<fg01_2=reg128#6%bot,<fg23=reg128#3%top
# asm 2: vmull.s32 >h3=q1,<fg01_2=d10,<fg23=d5
vmull.s32 q1,d10,d5

# qhasm:   h3[0,1] += fg01_2[2] signed* fg23[0];h3[2,3] += fg01_2[3] signed* fg23[1] 
# asm 1: vmlal.s32 <h3=reg128#2,<fg01_2=reg128#6%top,<fg23=reg128#3%bot
# asm 2: vmlal.s32 <h3=q1,<fg01_2=d11,<fg23=d4
vmlal.s32 q1,d11,d4

# qhasm:   h3[0,1] += fg45[0] signed* fg89_19_38[2];h3[2,3] += fg45[1] signed* fg89_19_38[3] 
# asm 1: vmlal.s32 <h3=reg128#2,<fg45=reg128#4%bot,<fg89_19_38=reg128#1%top
# asm 2: vmlal.s32 <h3=q1,<fg45=d6,<fg89_19_38=d1
vmlal.s32 q1,d6,d1

# qhasm:   h3[0,1] += fg45_2[2] signed* fg89_19_38[0];h3[2,3] += fg45_2[3] signed* fg89_19_38[1] 
# asm 1: vmlal.s32 <h3=reg128#2,<fg45_2=reg128#9%top,<fg89_19_38=reg128#1%bot
# asm 2: vmlal.s32 <h3=q1,<fg45_2=d17,<fg89_19_38=d0
vmlal.s32 q1,d17,d0

# qhasm:   h3[0,1] += fg67[0] signed* fg67_19_38[2];h3[2,3] += fg67[1] signed* fg67_19_38[3] 
# asm 1: vmlal.s32 <h3=reg128#2,<fg67=reg128#5%bot,<fg67_19_38=reg128#12%top
# asm 2: vmlal.s32 <h3=q1,<fg67=d8,<fg67_19_38=d23
vmlal.s32 q1,d8,d23

# qhasm:   h4[0,1] = fg01_2[0] signed* fg45[0];h4[2,3] = fg01_2[1] signed* fg45[1] 
# asm 1: vmull.s32 >h4=reg128#15,<fg01_2=reg128#6%bot,<fg45=reg128#4%bot
# asm 2: vmull.s32 >h4=q14,<fg01_2=d10,<fg45=d6
vmull.s32 q14,d10,d6

# qhasm:   h4[0,1] += fg01_2[2] signed* fg23_2[2];h4[2,3] += fg01_2[3] signed* fg23_2[3] 
# asm 1: vmlal.s32 <h4=reg128#15,<fg01_2=reg128#6%top,<fg23_2=reg128#7%top
# asm 2: vmlal.s32 <h4=q14,<fg01_2=d11,<fg23_2=d13
vmlal.s32 q14,d11,d13

# qhasm:   h4[0,1] += fg23[0] signed* fg23[0];h4[2,3] += fg23[1] signed* fg23[1] 
# asm 1: vmlal.s32 <h4=reg128#15,<fg23=reg128#3%bot,<fg23=reg128#3%bot
# asm 2: vmlal.s32 <h4=q14,<fg23=d4,<fg23=d4
vmlal.s32 q14,d4,d4

# qhasm:   h4[0,1] += fg45_2[2] signed* fg89_19_38[2];h4[2,3] += fg45_2[3] signed* fg89_19_38[3] 
# asm 1: vmlal.s32 <h4=reg128#15,<fg45_2=reg128#9%top,<fg89_19_38=reg128#1%top
# asm 2: vmlal.s32 <h4=q14,<fg45_2=d17,<fg89_19_38=d1
vmlal.s32 q14,d17,d1

# qhasm:   h4[0,1] += fg67_2[0] signed* fg89_19_38[0];h4[2,3] += fg67_2[1] signed* fg89_19_38[1] 
# asm 1: vmlal.s32 <h4=reg128#15,<fg67_2=reg128#10%bot,<fg89_19_38=reg128#1%bot
# asm 2: vmlal.s32 <h4=q14,<fg67_2=d18,<fg89_19_38=d0
vmlal.s32 q14,d18,d0

# qhasm:   h4[0,1] += fg67[2] signed* fg67_19_38[2];h4[2,3] += fg67[3] signed* fg67_19_38[3] 
# asm 1: vmlal.s32 <h4=reg128#15,<fg67=reg128#5%top,<fg67_19_38=reg128#12%top
# asm 2: vmlal.s32 <h4=q14,<fg67=d9,<fg67_19_38=d23
vmlal.s32 q14,d9,d23

# qhasm:   h5[0,1] = fg01_2[0] signed* fg45[2];h5[2,3] = fg01_2[1] signed* fg45[3] 
# asm 1: vmull.s32 >h5=reg128#12,<fg01_2=reg128#6%bot,<fg45=reg128#4%top
# asm 2: vmull.s32 >h5=q11,<fg01_2=d10,<fg45=d7
vmull.s32 q11,d10,d7

# qhasm:   h5[0,1] += fg01_2[2] signed* fg45[0];h5[2,3] += fg01_2[3] signed* fg45[1] 
# asm 1: vmlal.s32 <h5=reg128#12,<fg01_2=reg128#6%top,<fg45=reg128#4%bot
# asm 2: vmlal.s32 <h5=q11,<fg01_2=d11,<fg45=d6
vmlal.s32 q11,d11,d6

# qhasm:   h5[0,1] += fg23_2[0] signed* fg23[2];h5[2,3] += fg23_2[1] signed* fg23[3] 
# asm 1: vmlal.s32 <h5=reg128#12,<fg23_2=reg128#7%bot,<fg23=reg128#3%top
# asm 2: vmlal.s32 <h5=q11,<fg23_2=d12,<fg23=d5
vmlal.s32 q11,d12,d5

# qhasm:   h5[0,1] += fg67[0] signed* fg89_19_38[2];h5[2,3] += fg67[1] signed* fg89_19_38[3] 
# asm 1: vmlal.s32 <h5=reg128#12,<fg67=reg128#5%bot,<fg89_19_38=reg128#1%top
# asm 2: vmlal.s32 <h5=q11,<fg67=d8,<fg89_19_38=d1
vmlal.s32 q11,d8,d1

# qhasm:   h5[0,1] += fg67_2[2] signed* fg89_19_38[0];h5[2,3] += fg67_2[3] signed* fg89_19_38[1] 
# asm 1: vmlal.s32 <h5=reg128#12,<fg67_2=reg128#10%top,<fg89_19_38=reg128#1%bot
# asm 2: vmlal.s32 <h5=q11,<fg67_2=d19,<fg89_19_38=d0
vmlal.s32 q11,d19,d0

# qhasm:   h6[0,1] = fg01_2[0] signed* fg67[0];h6[2,3] = fg01_2[1] signed* fg67[1] 
# asm 1: vmull.s32 >h6=reg128#16,<fg01_2=reg128#6%bot,<fg67=reg128#5%bot
# asm 2: vmull.s32 >h6=q15,<fg01_2=d10,<fg67=d8
vmull.s32 q15,d10,d8

# qhasm:   h6[0,1] += fg01_2[2] signed* fg45_2[2];h6[2,3] += fg01_2[3] signed* fg45_2[3] 
# asm 1: vmlal.s32 <h6=reg128#16,<fg01_2=reg128#6%top,<fg45_2=reg128#9%top
# asm 2: vmlal.s32 <h6=q15,<fg01_2=d11,<fg45_2=d17
vmlal.s32 q15,d11,d17

# qhasm:   h6[0,1] += fg23_2[0] signed* fg45[0];h6[2,3] += fg23_2[1] signed* fg45[1] 
# asm 1: vmlal.s32 <h6=reg128#16,<fg23_2=reg128#7%bot,<fg45=reg128#4%bot
# asm 2: vmlal.s32 <h6=q15,<fg23_2=d12,<fg45=d6
vmlal.s32 q15,d12,d6

# qhasm:   h6[0,1] += fg23_2[2] signed* fg23[2];h6[2,3] += fg23_2[3] signed* fg23[3] 
# asm 1: vmlal.s32 <h6=reg128#16,<fg23_2=reg128#7%top,<fg23=reg128#3%top
# asm 2: vmlal.s32 <h6=q15,<fg23_2=d13,<fg23=d5
vmlal.s32 q15,d13,d5

# qhasm:   h6[0,1] += fg67_2[2] signed* fg89_19_38[2];h6[2,3] += fg67_2[3] signed* fg89_19_38[3] 
# asm 1: vmlal.s32 <h6=reg128#16,<fg67_2=reg128#10%top,<fg89_19_38=reg128#1%top
# asm 2: vmlal.s32 <h6=q15,<fg67_2=d19,<fg89_19_38=d1
vmlal.s32 q15,d19,d1

# qhasm:   h6[0,1] += fg89[0] signed* fg89_19_38[0];h6[2,3] += fg89[1] signed* fg89_19_38[1] 
# asm 1: vmlal.s32 <h6=reg128#16,<fg89=reg128#8%bot,<fg89_19_38=reg128#1%bot
# asm 2: vmlal.s32 <h6=q15,<fg89=d14,<fg89_19_38=d0
vmlal.s32 q15,d14,d0

# qhasm:   h7[0,1] = fg01_2[0] signed* fg67[2];h7[2,3] = fg01_2[1] signed* fg67[3] 
# asm 1: vmull.s32 >h7=reg128#3,<fg01_2=reg128#6%bot,<fg67=reg128#5%top
# asm 2: vmull.s32 >h7=q2,<fg01_2=d10,<fg67=d9
vmull.s32 q2,d10,d9

# qhasm:   h7[0,1] += fg01_2[2] signed* fg67[0];h7[2,3] += fg01_2[3] signed* fg67[1] 
# asm 1: vmlal.s32 <h7=reg128#3,<fg01_2=reg128#6%top,<fg67=reg128#5%bot
# asm 2: vmlal.s32 <h7=q2,<fg01_2=d11,<fg67=d8
vmlal.s32 q2,d11,d8

# qhasm:   h7[0,1] += fg23_2[0] signed* fg45[2];h7[2,3] += fg23_2[1] signed* fg45[3] 
# asm 1: vmlal.s32 <h7=reg128#3,<fg23_2=reg128#7%bot,<fg45=reg128#4%top
# asm 2: vmlal.s32 <h7=q2,<fg23_2=d12,<fg45=d7
vmlal.s32 q2,d12,d7

# qhasm:   h7[0,1] += fg23_2[2] signed* fg45[0];h7[2,3] += fg23_2[3] signed* fg45[1] 
# asm 1: vmlal.s32 <h7=reg128#3,<fg23_2=reg128#7%top,<fg45=reg128#4%bot
# asm 2: vmlal.s32 <h7=q2,<fg23_2=d13,<fg45=d6
vmlal.s32 q2,d13,d6

# qhasm:   h7[0,1] += fg89[0] signed* fg89_19_38[2];h7[2,3] += fg89[1] signed* fg89_19_38[3] 
# asm 1: vmlal.s32 <h7=reg128#3,<fg89=reg128#8%bot,<fg89_19_38=reg128#1%top
# asm 2: vmlal.s32 <h7=q2,<fg89=d14,<fg89_19_38=d1
vmlal.s32 q2,d14,d1

# qhasm:   h8[0,1] = fg89[2] signed* fg89_19_38[2];h8[2,3] = fg89[3] signed* fg89_19_38[3] 
# asm 1: vmull.s32 >h8=reg128#1,<fg89=reg128#8%top,<fg89_19_38=reg128#1%top
# asm 2: vmull.s32 >h8=q0,<fg89=d15,<fg89_19_38=d1
vmull.s32 q0,d15,d1

# qhasm:   h8[0,1] += fg01_2[0] signed* fg89[0];h8[2,3] += fg01_2[1] signed* fg89[1] 
# asm 1: vmlal.s32 <h8=reg128#1,<fg01_2=reg128#6%bot,<fg89=reg128#8%bot
# asm 2: vmlal.s32 <h8=q0,<fg01_2=d10,<fg89=d14
vmlal.s32 q0,d10,d14

# qhasm:   h8[0,1] += fg01_2[2] signed* fg67_2[2];h8[2,3] += fg01_2[3] signed* fg67_2[3] 
# asm 1: vmlal.s32 <h8=reg128#1,<fg01_2=reg128#6%top,<fg67_2=reg128#10%top
# asm 2: vmlal.s32 <h8=q0,<fg01_2=d11,<fg67_2=d19
vmlal.s32 q0,d11,d19

# qhasm:   h8[0,1] += fg23_2[0] signed* fg67[0];h8[2,3] += fg23_2[1] signed* fg67[1] 
# asm 1: vmlal.s32 <h8=reg128#1,<fg23_2=reg128#7%bot,<fg67=reg128#5%bot
# asm 2: vmlal.s32 <h8=q0,<fg23_2=d12,<fg67=d8
vmlal.s32 q0,d12,d8

# qhasm:   h8[0,1] += fg23_2[2] signed* fg45_2[2];h8[2,3] += fg23_2[3] signed* fg45_2[3] 
# asm 1: vmlal.s32 <h8=reg128#1,<fg23_2=reg128#7%top,<fg45_2=reg128#9%top
# asm 2: vmlal.s32 <h8=q0,<fg23_2=d13,<fg45_2=d17
vmlal.s32 q0,d13,d17

# qhasm:   h8[0,1] += fg45[0] signed* fg45[0];h8[2,3] += fg45[1] signed* fg45[1] 
# asm 1: vmlal.s32 <h8=reg128#1,<fg45=reg128#4%bot,<fg45=reg128#4%bot
# asm 2: vmlal.s32 <h8=q0,<fg45=d6,<fg45=d6
vmlal.s32 q0,d6,d6

# qhasm:   ptr = &_0x2000000_stack 
# asm 1: lea >ptr=int32#3,<_0x2000000_stack=stack128#1
# asm 2: lea >ptr=r2,<_0x2000000_stack=[sp,#512]
add r2,sp,#512

# qhasm:   _0x2000000 aligned= mem128[ptr] 
# asm 1: vld1.8 {>_0x2000000=reg128#10%bot->_0x2000000=reg128#10%top},[<ptr=int32#3,: 128]
# asm 2: vld1.8 {>_0x2000000=d18->_0x2000000=d19},[<ptr=r2,: 128]
vld1.8 {d18-d19},[r2,: 128]

# qhasm:   h9[0,1] = fg45_2[0] signed* fg45[2];h9[2,3] = fg45_2[1] signed* fg45[3] 
# asm 1: vmull.s32 >h9=reg128#4,<fg45_2=reg128#9%bot,<fg45=reg128#4%top
# asm 2: vmull.s32 >h9=q3,<fg45_2=d16,<fg45=d7
vmull.s32 q3,d16,d7

# qhasm:   h9[0,1] += fg01_2[0] signed* fg89[2];h9[2,3] += fg01_2[1] signed* fg89[3] 
# asm 1: vmlal.s32 <h9=reg128#4,<fg01_2=reg128#6%bot,<fg89=reg128#8%top
# asm 2: vmlal.s32 <h9=q3,<fg01_2=d10,<fg89=d15
vmlal.s32 q3,d10,d15

# qhasm:   h9[0,1] += fg01_2[2] signed* fg89[0];h9[2,3] += fg01_2[3] signed* fg89[1] 
# asm 1: vmlal.s32 <h9=reg128#4,<fg01_2=reg128#6%top,<fg89=reg128#8%bot
# asm 2: vmlal.s32 <h9=q3,<fg01_2=d11,<fg89=d14
vmlal.s32 q3,d11,d14

# qhasm:   h9[0,1] += fg23_2[0] signed* fg67[2];h9[2,3] += fg23_2[1] signed* fg67[3] 
# asm 1: vmlal.s32 <h9=reg128#4,<fg23_2=reg128#7%bot,<fg67=reg128#5%top
# asm 2: vmlal.s32 <h9=q3,<fg23_2=d12,<fg67=d9
vmlal.s32 q3,d12,d9

# qhasm:   h9[0,1] += fg23_2[2] signed* fg67[0];h9[2,3] += fg23_2[3] signed* fg67[1] 
# asm 1: vmlal.s32 <h9=reg128#4,<fg23_2=reg128#7%top,<fg67=reg128#5%bot
# asm 2: vmlal.s32 <h9=q3,<fg23_2=d13,<fg67=d8
vmlal.s32 q3,d13,d8

# qhasm:   ptr = &_0x1000000_stack 
# asm 1: lea >ptr=int32#3,<_0x1000000_stack=stack128#2
# asm 2: lea >ptr=r2,<_0x1000000_stack=[sp,#528]
add r2,sp,#528

# qhasm:   _0x1000000 aligned= mem128[ptr] 
# asm 1: vld1.8 {>_0x1000000=reg128#5%bot->_0x1000000=reg128#5%top},[<ptr=int32#3,: 128]
# asm 2: vld1.8 {>_0x1000000=d8->_0x1000000=d9},[<ptr=r2,: 128]
vld1.8 {d8-d9},[r2,: 128]

# qhasm:   2x t0 = h0 + _0x2000000 
# asm 1: vadd.i64 >t0=reg128#6,<h0=reg128#13,<_0x2000000=reg128#10
# asm 2: vadd.i64 >t0=q5,<h0=q12,<_0x2000000=q9
vadd.i64 q5,q12,q9

# qhasm:   2x t6 = h6 + _0x2000000 
# asm 1: vadd.i64 >t6=reg128#7,<h6=reg128#16,<_0x2000000=reg128#10
# asm 2: vadd.i64 >t6=q6,<h6=q15,<_0x2000000=q9
vadd.i64 q6,q15,q9

# qhasm:   2x c0 = t0 signed>> 26 
# asm 1: vshr.s64 >c0=reg128#6,<t0=reg128#6,#26
# asm 2: vshr.s64 >c0=q5,<t0=q5,#26
vshr.s64 q5,q5,#26

# qhasm:   2x c6 = t6 signed>> 26 
# asm 1: vshr.s64 >c6=reg128#7,<t6=reg128#7,#26
# asm 2: vshr.s64 >c6=q6,<t6=q6,#26
vshr.s64 q6,q6,#26

# qhasm:   2x h1 += c0 
# asm 1: vadd.i64 >h1=reg128#8,<h1=reg128#11,<c0=reg128#6
# asm 2: vadd.i64 >h1=q7,<h1=q10,<c0=q5
vadd.i64 q7,q10,q5

# qhasm:   2x t0 = c0 << 26 
# asm 1: vshl.i64 >t0=reg128#6,<c0=reg128#6,#26
# asm 2: vshl.i64 >t0=q5,<c0=q5,#26
vshl.i64 q5,q5,#26

# qhasm:   2x t1 = h1 + _0x1000000 
# asm 1: vadd.i64 >t1=reg128#9,<h1=reg128#8,<_0x1000000=reg128#5
# asm 2: vadd.i64 >t1=q8,<h1=q7,<_0x1000000=q4
vadd.i64 q8,q7,q4

# qhasm:   2x h7 += c6 
# asm 1: vadd.i64 >h7=reg128#3,<h7=reg128#3,<c6=reg128#7
# asm 2: vadd.i64 >h7=q2,<h7=q2,<c6=q6
vadd.i64 q2,q2,q6

# qhasm:   2x t6 = c6 << 26 
# asm 1: vshl.i64 >t6=reg128#7,<c6=reg128#7,#26
# asm 2: vshl.i64 >t6=q6,<c6=q6,#26
vshl.i64 q6,q6,#26

# qhasm:   2x t7 = h7 + _0x1000000 
# asm 1: vadd.i64 >t7=reg128#11,<h7=reg128#3,<_0x1000000=reg128#5
# asm 2: vadd.i64 >t7=q10,<h7=q2,<_0x1000000=q4
vadd.i64 q10,q2,q4

# qhasm:   2x h0 -= t0 
# asm 1: vsub.i64 >h0=reg128#6,<h0=reg128#13,<t0=reg128#6
# asm 2: vsub.i64 >h0=q5,<h0=q12,<t0=q5
vsub.i64 q5,q12,q5

# qhasm:   2x c1 = t1 signed>> 25 
# asm 1: vshr.s64 >c1=reg128#9,<t1=reg128#9,#25
# asm 2: vshr.s64 >c1=q8,<t1=q8,#25
vshr.s64 q8,q8,#25

# qhasm:   2x h6 -= t6 
# asm 1: vsub.i64 >h6=reg128#7,<h6=reg128#16,<t6=reg128#7
# asm 2: vsub.i64 >h6=q6,<h6=q15,<t6=q6
vsub.i64 q6,q15,q6

# qhasm:   2x c7 = t7 signed>> 25 
# asm 1: vshr.s64 >c7=reg128#11,<t7=reg128#11,#25
# asm 2: vshr.s64 >c7=q10,<t7=q10,#25
vshr.s64 q10,q10,#25

# qhasm:   2x h2 += c1 
# asm 1: vadd.i64 >h2=reg128#13,<h2=reg128#14,<c1=reg128#9
# asm 2: vadd.i64 >h2=q12,<h2=q13,<c1=q8
vadd.i64 q12,q13,q8

# qhasm:   2x t1 = c1 << 25 
# asm 1: vshl.i64 >t1=reg128#9,<c1=reg128#9,#25
# asm 2: vshl.i64 >t1=q8,<c1=q8,#25
vshl.i64 q8,q8,#25

# qhasm:   2x t2 = h2 + _0x2000000 
# asm 1: vadd.i64 >t2=reg128#14,<h2=reg128#13,<_0x2000000=reg128#10
# asm 2: vadd.i64 >t2=q13,<h2=q12,<_0x2000000=q9
vadd.i64 q13,q12,q9

# qhasm:   2x h8 += c7 
# asm 1: vadd.i64 >h8=reg128#1,<h8=reg128#1,<c7=reg128#11
# asm 2: vadd.i64 >h8=q0,<h8=q0,<c7=q10
vadd.i64 q0,q0,q10

# qhasm:   2x h1 -= t1 
# asm 1: vsub.i64 >h1=reg128#8,<h1=reg128#8,<t1=reg128#9
# asm 2: vsub.i64 >h1=q7,<h1=q7,<t1=q8
vsub.i64 q7,q7,q8

# qhasm:   2x c2 = t2 signed>> 26 
# asm 1: vshr.s64 >c2=reg128#9,<t2=reg128#14,#26
# asm 2: vshr.s64 >c2=q8,<t2=q13,#26
vshr.s64 q8,q13,#26

# qhasm:   2x t7 = c7 << 25 
# asm 1: vshl.i64 >t7=reg128#11,<c7=reg128#11,#25
# asm 2: vshl.i64 >t7=q10,<c7=q10,#25
vshl.i64 q10,q10,#25

# qhasm:   2x t8 = h8 + _0x2000000 
# asm 1: vadd.i64 >t8=reg128#14,<h8=reg128#1,<_0x2000000=reg128#10
# asm 2: vadd.i64 >t8=q13,<h8=q0,<_0x2000000=q9
vadd.i64 q13,q0,q9

# qhasm:   2x h3 += c2 
# asm 1: vadd.i64 >h3=reg128#2,<h3=reg128#2,<c2=reg128#9
# asm 2: vadd.i64 >h3=q1,<h3=q1,<c2=q8
vadd.i64 q1,q1,q8

# qhasm:   2x t2 = c2 << 26 
# asm 1: vshl.i64 >t2=reg128#9,<c2=reg128#9,#26
# asm 2: vshl.i64 >t2=q8,<c2=q8,#26
vshl.i64 q8,q8,#26

# qhasm:   2x t3 = h3 + _0x1000000 
# asm 1: vadd.i64 >t3=reg128#16,<h3=reg128#2,<_0x1000000=reg128#5
# asm 2: vadd.i64 >t3=q15,<h3=q1,<_0x1000000=q4
vadd.i64 q15,q1,q4

# qhasm:   2x h7 -= t7 
# asm 1: vsub.i64 >h7=reg128#3,<h7=reg128#3,<t7=reg128#11
# asm 2: vsub.i64 >h7=q2,<h7=q2,<t7=q10
vsub.i64 q2,q2,q10

# qhasm:   2x c8 = t8 signed>> 26 
# asm 1: vshr.s64 >c8=reg128#11,<t8=reg128#14,#26
# asm 2: vshr.s64 >c8=q10,<t8=q13,#26
vshr.s64 q10,q13,#26

# qhasm:   2x h2 -= t2 
# asm 1: vsub.i64 >h2=reg128#9,<h2=reg128#13,<t2=reg128#9
# asm 2: vsub.i64 >h2=q8,<h2=q12,<t2=q8
vsub.i64 q8,q12,q8

# qhasm:   2x c3 = t3 signed>> 25 
# asm 1: vshr.s64 >c3=reg128#13,<t3=reg128#16,#25
# asm 2: vshr.s64 >c3=q12,<t3=q15,#25
vshr.s64 q12,q15,#25

# qhasm:   2x h9 += c8 
# asm 1: vadd.i64 >h9=reg128#4,<h9=reg128#4,<c8=reg128#11
# asm 2: vadd.i64 >h9=q3,<h9=q3,<c8=q10
vadd.i64 q3,q3,q10

# qhasm:   2x t8 = c8 << 26 
# asm 1: vshl.i64 >t8=reg128#11,<c8=reg128#11,#26
# asm 2: vshl.i64 >t8=q10,<c8=q10,#26
vshl.i64 q10,q10,#26

# qhasm:   2x t9 = h9 + _0x1000000 
# asm 1: vadd.i64 >t9=reg128#14,<h9=reg128#4,<_0x1000000=reg128#5
# asm 2: vadd.i64 >t9=q13,<h9=q3,<_0x1000000=q4
vadd.i64 q13,q3,q4

# qhasm:   2x h4 += c3 
# asm 1: vadd.i64 >h4=reg128#15,<h4=reg128#15,<c3=reg128#13
# asm 2: vadd.i64 >h4=q14,<h4=q14,<c3=q12
vadd.i64 q14,q14,q12

# qhasm:   posh = playground1_ptr + 288 
# asm 1: add >posh=int32#3,<playground1_ptr=int32#4,#288
# asm 2: add >posh=r2,<playground1_ptr=r3,#288
add r2,r3,#288

# qhasm:   2x t3 = c3 << 25 
# asm 1: vshl.i64 >t3=reg128#13,<c3=reg128#13,#25
# asm 2: vshl.i64 >t3=q12,<c3=q12,#25
vshl.i64 q12,q12,#25

# qhasm:   posH = playground1_ptr + 336 
# asm 1: add >posH=int32#5,<playground1_ptr=int32#4,#336
# asm 2: add >posH=r4,<playground1_ptr=r3,#336
add r4,r3,#336

# qhasm:   2x t4 = h4 + _0x2000000 
# asm 1: vadd.i64 >t4=reg128#16,<h4=reg128#15,<_0x2000000=reg128#10
# asm 2: vadd.i64 >t4=q15,<h4=q14,<_0x2000000=q9
vadd.i64 q15,q14,q9

# qhasm:   posh+=8
# asm 1: add >posh=int32#3,<posh=int32#3,#8
# asm 2: add >posh=r2,<posh=r2,#8
add r2,r2,#8

# qhasm:   2x h8 -= t8 
# asm 1: vsub.i64 >h8=reg128#1,<h8=reg128#1,<t8=reg128#11
# asm 2: vsub.i64 >h8=q0,<h8=q0,<t8=q10
vsub.i64 q0,q0,q10

# qhasm:   posH+=8
# asm 1: add >posH=int32#5,<posH=int32#5,#8
# asm 2: add >posH=r4,<posH=r4,#8
add r4,r4,#8

# qhasm:   2x c9 = t9 signed>> 25 
# asm 1: vshr.s64 >c9=reg128#11,<t9=reg128#14,#25
# asm 2: vshr.s64 >c9=q10,<t9=q13,#25
vshr.s64 q10,q13,#25

# qhasm:   2x h3 -= t3 
# asm 1: vsub.i64 >h3=reg128#2,<h3=reg128#2,<t3=reg128#13
# asm 2: vsub.i64 >h3=q1,<h3=q1,<t3=q12
vsub.i64 q1,q1,q12

# qhasm:   2x c4 = t4 signed>> 26 
# asm 1: vshr.s64 >c4=reg128#13,<t4=reg128#16,#26
# asm 2: vshr.s64 >c4=q12,<t4=q15,#26
vshr.s64 q12,q15,#26

# qhasm:   2x s = c9 + c9 
# asm 1: vadd.i64 >s=reg128#14,<c9=reg128#11,<c9=reg128#11
# asm 2: vadd.i64 >s=q13,<c9=q10,<c9=q10
vadd.i64 q13,q10,q10

# qhasm:   2x h5 += c4 
# asm 1: vadd.i64 >h5=reg128#12,<h5=reg128#12,<c4=reg128#13
# asm 2: vadd.i64 >h5=q11,<h5=q11,<c4=q12
vadd.i64 q11,q11,q12

# qhasm:   h2 h3 = h2[0]h3[0]h2[2]h2[3] h2[1]h3[1]h3[2]h3[3]
# asm 1: vtrn.32 <h2=reg128#9%bot,<h3=reg128#2%bot
# asm 2: vtrn.32 <h2=d16,<h3=d2
vtrn.32 d16,d2

# qhasm:   2x t4 = c4 << 26 
# asm 1: vshl.i64 >t4=reg128#13,<c4=reg128#13,#26
# asm 2: vshl.i64 >t4=q12,<c4=q12,#26
vshl.i64 q12,q12,#26

# qhasm:   h2 h3 = h2[0]h2[1]h2[2]h3[2] h3[0]h3[1]h2[3]h3[3]
# asm 1: vtrn.32 <h2=reg128#9%top,<h3=reg128#2%top
# asm 2: vtrn.32 <h2=d17,<h3=d3
vtrn.32 d17,d3

# qhasm:   2x t5 = h5 + _0x1000000 
# asm 1: vadd.i64 >t5=reg128#2,<h5=reg128#12,<_0x1000000=reg128#5
# asm 2: vadd.i64 >t5=q1,<h5=q11,<_0x1000000=q4
vadd.i64 q1,q11,q4

# qhasm:   2x h0 += s 
# asm 1: vadd.i64 >h0=reg128#5,<h0=reg128#6,<s=reg128#14
# asm 2: vadd.i64 >h0=q4,<h0=q5,<s=q13
vadd.i64 q4,q5,q13

# qhasm:   mem64[posh] aligned= h2[0];posh+=8 
# asm 1: vst1.8 <h2=reg128#9%bot,[<posh=int32#3,: 64]!
# asm 2: vst1.8 <h2=d16,[<posh=r2,: 64]!
vst1.8 d16,[r2,: 64]!

# qhasm:   2x s = c9 << 4 
# asm 1: vshl.i64 >s=reg128#6,<c9=reg128#11,#4
# asm 2: vshl.i64 >s=q5,<c9=q10,#4
vshl.i64 q5,q10,#4

# qhasm:   mem64[posH] aligned= h2[1];posH+=8 
# asm 1: vst1.8 <h2=reg128#9%top,[<posH=int32#5,: 64]!
# asm 2: vst1.8 <h2=d17,[<posH=r4,: 64]!
vst1.8 d17,[r4,: 64]!

# qhasm:   2x h4 -= t4 
# asm 1: vsub.i64 >h4=reg128#9,<h4=reg128#15,<t4=reg128#13
# asm 2: vsub.i64 >h4=q8,<h4=q14,<t4=q12
vsub.i64 q8,q14,q12

# qhasm:   2x c5 = t5 signed>> 25 
# asm 1: vshr.s64 >c5=reg128#2,<t5=reg128#2,#25
# asm 2: vshr.s64 >c5=q1,<t5=q1,#25
vshr.s64 q1,q1,#25

# qhasm:   2x h0 += s 
# asm 1: vadd.i64 >h0=reg128#5,<h0=reg128#5,<s=reg128#6
# asm 2: vadd.i64 >h0=q4,<h0=q4,<s=q5
vadd.i64 q4,q4,q5

# qhasm:   2x h6 += c5 
# asm 1: vadd.i64 >h6=reg128#6,<h6=reg128#7,<c5=reg128#2
# asm 2: vadd.i64 >h6=q5,<h6=q6,<c5=q1
vadd.i64 q5,q6,q1

# qhasm:   2x t5 = c5 << 25 
# asm 1: vshl.i64 >t5=reg128#2,<c5=reg128#2,#25
# asm 2: vshl.i64 >t5=q1,<c5=q1,#25
vshl.i64 q1,q1,#25

# qhasm:   2x t6 = h6 + _0x2000000 
# asm 1: vadd.i64 >t6=reg128#7,<h6=reg128#6,<_0x2000000=reg128#10
# asm 2: vadd.i64 >t6=q6,<h6=q5,<_0x2000000=q9
vadd.i64 q6,q5,q9

# qhasm:   2x h0 += c9 
# asm 1: vadd.i64 >h0=reg128#5,<h0=reg128#5,<c9=reg128#11
# asm 2: vadd.i64 >h0=q4,<h0=q4,<c9=q10
vadd.i64 q4,q4,q10

# qhasm:   2x t9 = c9 << 25 
# asm 1: vshl.i64 >t9=reg128#11,<c9=reg128#11,#25
# asm 2: vshl.i64 >t9=q10,<c9=q10,#25
vshl.i64 q10,q10,#25

# qhasm:   2x t0 = h0 + _0x2000000 
# asm 1: vadd.i64 >t0=reg128#10,<h0=reg128#5,<_0x2000000=reg128#10
# asm 2: vadd.i64 >t0=q9,<h0=q4,<_0x2000000=q9
vadd.i64 q9,q4,q9

# qhasm:   2x h5 -= t5 
# asm 1: vsub.i64 >h5=reg128#2,<h5=reg128#12,<t5=reg128#2
# asm 2: vsub.i64 >h5=q1,<h5=q11,<t5=q1
vsub.i64 q1,q11,q1

# qhasm:   2x c6 = t6 signed>> 26 
# asm 1: vshr.s64 >c6=reg128#7,<t6=reg128#7,#26
# asm 2: vshr.s64 >c6=q6,<t6=q6,#26
vshr.s64 q6,q6,#26

# qhasm:   2x h9 -= t9 
# asm 1: vsub.i64 >h9=reg128#4,<h9=reg128#4,<t9=reg128#11
# asm 2: vsub.i64 >h9=q3,<h9=q3,<t9=q10
vsub.i64 q3,q3,q10

# qhasm:   h4 h5 = h4[0]h5[0]h4[2]h4[3] h4[1]h5[1]h5[2]h5[3]
# asm 1: vtrn.32 <h4=reg128#9%bot,<h5=reg128#2%bot
# asm 2: vtrn.32 <h4=d16,<h5=d2
vtrn.32 d16,d2

# qhasm:   2x c0 = t0 signed>> 26 
# asm 1: vshr.s64 >c0=reg128#10,<t0=reg128#10,#26
# asm 2: vshr.s64 >c0=q9,<t0=q9,#26
vshr.s64 q9,q9,#26

# qhasm:   h4 h5 = h4[0]h4[1]h4[2]h5[2] h5[0]h5[1]h4[3]h5[3]
# asm 1: vtrn.32 <h4=reg128#9%top,<h5=reg128#2%top
# asm 2: vtrn.32 <h4=d17,<h5=d3
vtrn.32 d17,d3

# qhasm:   2x h7 += c6 
# asm 1: vadd.i64 >h7=reg128#2,<h7=reg128#3,<c6=reg128#7
# asm 2: vadd.i64 >h7=q1,<h7=q2,<c6=q6
vadd.i64 q1,q2,q6

# qhasm:   mem64[posh] aligned= h4[0] 
# asm 1: vst1.8 <h4=reg128#9%bot,[<posh=int32#3,: 64]
# asm 2: vst1.8 <h4=d16,[<posh=r2,: 64]
vst1.8 d16,[r2,: 64]

# qhasm:   2x t6 = c6 << 26 
# asm 1: vshl.i64 >t6=reg128#3,<c6=reg128#7,#26
# asm 2: vshl.i64 >t6=q2,<c6=q6,#26
vshl.i64 q2,q6,#26

# qhasm:   mem64[posH] aligned= h4[1] 
# asm 1: vst1.8 <h4=reg128#9%top,[<posH=int32#5,: 64]
# asm 2: vst1.8 <h4=d17,[<posH=r4,: 64]
vst1.8 d17,[r4,: 64]

# qhasm:   2x h1 += c0 
# asm 1: vadd.i64 >h1=reg128#7,<h1=reg128#8,<c0=reg128#10
# asm 2: vadd.i64 >h1=q6,<h1=q7,<c0=q9
vadd.i64 q6,q7,q9

# qhasm:   h8 h9 = h8[0]h9[0]h8[2]h8[3] h8[1]h9[1]h9[2]h9[3]
# asm 1: vtrn.32 <h8=reg128#1%bot,<h9=reg128#4%bot
# asm 2: vtrn.32 <h8=d0,<h9=d6
vtrn.32 d0,d6

# qhasm:   2x t0 = c0 << 26 
# asm 1: vshl.i64 >t0=reg128#8,<c0=reg128#10,#26
# asm 2: vshl.i64 >t0=q7,<c0=q9,#26
vshl.i64 q7,q9,#26

# qhasm:   h8 h9 = h8[0]h8[1]h8[2]h9[2] h9[0]h9[1]h8[3]h9[3]
# asm 1: vtrn.32 <h8=reg128#1%top,<h9=reg128#4%top
# asm 2: vtrn.32 <h8=d1,<h9=d7
vtrn.32 d1,d7

# qhasm:   2x h6 -= t6 
# asm 1: vsub.i64 >h6=reg128#3,<h6=reg128#6,<t6=reg128#3
# asm 2: vsub.i64 >h6=q2,<h6=q5,<t6=q2
vsub.i64 q2,q5,q2

# qhasm:   posh+=16
# asm 1: add >posh=int32#3,<posh=int32#3,#16
# asm 2: add >posh=r2,<posh=r2,#16
add r2,r2,#16

# qhasm:   2x h0 -= t0 
# asm 1: vsub.i64 >h0=reg128#4,<h0=reg128#5,<t0=reg128#8
# asm 2: vsub.i64 >h0=q3,<h0=q4,<t0=q7
vsub.i64 q3,q4,q7

# qhasm:   mem64[posh] aligned= h8[0] 
# asm 1: vst1.8 <h8=reg128#1%bot,[<posh=int32#3,: 64]
# asm 2: vst1.8 <h8=d0,[<posh=r2,: 64]
vst1.8 d0,[r2,: 64]

# qhasm:   posH+=16
# asm 1: add >posH=int32#5,<posH=int32#5,#16
# asm 2: add >posH=r4,<posH=r4,#16
add r4,r4,#16

# qhasm:   mem64[posH] aligned= h8[1] 
# asm 1: vst1.8 <h8=reg128#1%top,[<posH=int32#5,: 64]
# asm 2: vst1.8 <h8=d1,[<posH=r4,: 64]
vst1.8 d1,[r4,: 64]

# qhasm:   h6 h7 = h6[0]h7[0]h6[2]h6[3] h6[1]h7[1]h7[2]h7[3]
# asm 1: vtrn.32 <h6=reg128#3%bot,<h7=reg128#2%bot
# asm 2: vtrn.32 <h6=d4,<h7=d2
vtrn.32 d4,d2

# qhasm:   h6 h7 = h6[0]h6[1]h6[2]h7[2] h7[0]h7[1]h6[3]h7[3]
# asm 1: vtrn.32 <h6=reg128#3%top,<h7=reg128#2%top
# asm 2: vtrn.32 <h6=d5,<h7=d3
vtrn.32 d5,d3

# qhasm:   posh-=8
# asm 1: sub >posh=int32#3,<posh=int32#3,#8
# asm 2: sub >posh=r2,<posh=r2,#8
sub r2,r2,#8

# qhasm:   posH-=8
# asm 1: sub >posH=int32#5,<posH=int32#5,#8
# asm 2: sub >posH=r4,<posH=r4,#8
sub r4,r4,#8

# qhasm:   h0 h1 = h0[0]h1[0]h0[2]h0[3] h0[1]h1[1]h1[2]h1[3]
# asm 1: vtrn.32 <h0=reg128#4%bot,<h1=reg128#7%bot
# asm 2: vtrn.32 <h0=d6,<h1=d12
vtrn.32 d6,d12

# qhasm:   h0 h1 = h0[0]h0[1]h0[2]h1[2] h1[0]h1[1]h0[3]h1[3]
# asm 1: vtrn.32 <h0=reg128#4%top,<h1=reg128#7%top
# asm 2: vtrn.32 <h0=d7,<h1=d13
vtrn.32 d7,d13

# qhasm:   mem64[posh] aligned= h6[0] 
# asm 1: vst1.8 <h6=reg128#3%bot,[<posh=int32#3,: 64]
# asm 2: vst1.8 <h6=d4,[<posh=r2,: 64]
vst1.8 d4,[r2,: 64]

# qhasm:   mem64[posH] aligned= h6[1] 
# asm 1: vst1.8 <h6=reg128#3%top,[<posH=int32#5,: 64]
# asm 2: vst1.8 <h6=d5,[<posH=r4,: 64]
vst1.8 d5,[r4,: 64]

# qhasm:   posh-=24
# asm 1: sub >posh=int32#3,<posh=int32#3,#24
# asm 2: sub >posh=r2,<posh=r2,#24
sub r2,r2,#24

# qhasm:   posH-=24
# asm 1: sub >posH=int32#5,<posH=int32#5,#24
# asm 2: sub >posH=r4,<posH=r4,#24
sub r4,r4,#24

# qhasm:   mem64[posh] aligned= h0[0] 
# asm 1: vst1.8 <h0=reg128#4%bot,[<posh=int32#3,: 64]
# asm 2: vst1.8 <h0=d6,[<posh=r2,: 64]
vst1.8 d6,[r2,: 64]

# qhasm:   mem64[posH] aligned= h0[1] 
# asm 1: vst1.8 <h0=reg128#4%top,[<posH=int32#5,: 64]
# asm 2: vst1.8 <h0=d7,[<posH=r4,: 64]
vst1.8 d7,[r4,: 64]

# qhasm:   posf = playground1_ptr + 240 
# asm 1: add >posf=int32#3,<playground1_ptr=int32#4,#240
# asm 2: add >posf=r2,<playground1_ptr=r3,#240
add r2,r3,#240

# qhasm:   posg = playground1_ptr + 96 
# asm 1: add >posg=int32#5,<playground1_ptr=int32#4,#96
# asm 2: add >posg=r4,<playground1_ptr=r3,#96
add r4,r3,#96

# qhasm:   g02 aligned= mem128[posg];posg += 16 
# asm 1: vld1.8 {>g02=reg128#1%bot->g02=reg128#1%top},[<posg=int32#5,: 128]!
# asm 2: vld1.8 {>g02=d0->g02=d1},[<posg=r4,: 128]!
vld1.8 {d0-d1},[r4,: 128]!

# qhasm:   g46 aligned= mem128[posg];posg += 16 
# asm 1: vld1.8 {>g46=reg128#2%bot->g46=reg128#2%top},[<posg=int32#5,: 128]!
# asm 2: vld1.8 {>g46=d2->g46=d3},[<posg=r4,: 128]!
vld1.8 {d2-d3},[r4,: 128]!

# qhasm:   new g89 

# qhasm:   g89 aligned= mem64[posg] g89[1] 
# asm 1: vld1.8 {<g89=reg128#3%bot},[<posg=int32#5,: 64]
# asm 2: vld1.8 {<g89=d4},[<posg=r4,: 64]
vld1.8 {d4},[r4,: 64]

# qhasm:   posG = playground1_ptr + 144 
# asm 1: add >posG=int32#5,<playground1_ptr=int32#4,#144
# asm 2: add >posG=r4,<playground1_ptr=r3,#144
add r4,r3,#144

# qhasm:   g13 aligned= mem128[posG];posG += 16 
# asm 1: vld1.8 {>g13=reg128#4%bot->g13=reg128#4%top},[<posG=int32#5,: 128]!
# asm 2: vld1.8 {>g13=d6->g13=d7},[<posG=r4,: 128]!
vld1.8 {d6-d7},[r4,: 128]!

# qhasm:   g02 g13 = g02[0]g13[0] g02[2]g13[2] g02[1]g13[1] g02[3]g13[3] 
# asm 1: vtrn.32 <g02=reg128#1,<g13=reg128#4
# asm 2: vtrn.32 <g02=q0,<g13=q3
vtrn.32 q0,q3

# qhasm:   g57 aligned= mem128[posG];posG += 16 
# asm 1: vld1.8 {>g57=reg128#5%bot->g57=reg128#5%top},[<posG=int32#5,: 128]!
# asm 2: vld1.8 {>g57=d8->g57=d9},[<posG=r4,: 128]!
vld1.8 {d8-d9},[r4,: 128]!

# qhasm:   4x mix = g02 << 4 
# asm 1: vshl.i32 >mix=reg128#6,<g02=reg128#1,#4
# asm 2: vshl.i32 >mix=q5,<g02=q0,#4
vshl.i32 q5,q0,#4

# qhasm:   g46 g57 = g46[0]g57[0] g46[2]g57[2] g46[1]g57[1] g46[3]g57[3] 
# asm 1: vtrn.32 <g46=reg128#2,<g57=reg128#5
# asm 2: vtrn.32 <g46=q1,<g57=q4
vtrn.32 q1,q4

# qhasm:   4x g13_19 = g13 << 4 
# asm 1: vshl.i32 >g13_19=reg128#7,<g13=reg128#4,#4
# asm 2: vshl.i32 >g13_19=q6,<g13=q3,#4
vshl.i32 q6,q3,#4

# qhasm:   4x mix += g02 
# asm 1: vadd.i32 >mix=reg128#6,<mix=reg128#6,<g02=reg128#1
# asm 2: vadd.i32 >mix=q5,<mix=q5,<g02=q0
vadd.i32 q5,q5,q0

# qhasm:   4x g13_19 += g13 
# asm 1: vadd.i32 >g13_19=reg128#7,<g13_19=reg128#7,<g13=reg128#4
# asm 2: vadd.i32 >g13_19=q6,<g13_19=q6,<g13=q3
vadd.i32 q6,q6,q3

# qhasm:   4x g46_19 = g46 << 4 
# asm 1: vshl.i32 >g46_19=reg128#8,<g46=reg128#2,#4
# asm 2: vshl.i32 >g46_19=q7,<g46=q1,#4
vshl.i32 q7,q1,#4

# qhasm:   g89 aligned= g89[0] mem64[posG] 
# asm 1: vld1.8 {<g89=reg128#3%top},[<posG=int32#5,: 64]
# asm 2: vld1.8 {<g89=d5},[<posG=r4,: 64]
vld1.8 {d5},[r4,: 64]

# qhasm:   4x g57_19 = g57 << 4 
# asm 1: vshl.i32 >g57_19=reg128#9,<g57=reg128#5,#4
# asm 2: vshl.i32 >g57_19=q8,<g57=q4,#4
vshl.i32 q8,q4,#4

# qhasm:   g89 = g89[0] g89[2] g89[1] g89[3] 
# asm 1: vtrn.32 <g89=reg128#3%bot,<g89=reg128#3%top
# asm 2: vtrn.32 <g89=d4,<g89=d5
vtrn.32 d4,d5

# qhasm:   4x g46_19 += g46 
# asm 1: vadd.i32 >g46_19=reg128#8,<g46_19=reg128#8,<g46=reg128#2
# asm 2: vadd.i32 >g46_19=q7,<g46_19=q7,<g46=q1
vadd.i32 q7,q7,q1

# qhasm:   4x g57_19 += g57 
# asm 1: vadd.i32 >g57_19=reg128#9,<g57_19=reg128#9,<g57=reg128#5
# asm 2: vadd.i32 >g57_19=q8,<g57_19=q8,<g57=q4
vadd.i32 q8,q8,q4

# qhasm:   f02 aligned= mem128[posf];posf += 16 
# asm 1: vld1.8 {>f02=reg128#10%bot->f02=reg128#10%top},[<posf=int32#3,: 128]!
# asm 2: vld1.8 {>f02=d18->f02=d19},[<posf=r2,: 128]!
vld1.8 {d18-d19},[r2,: 128]!

# qhasm:   4x g89_19 = g89 << 4 
# asm 1: vshl.i32 >g89_19=reg128#11,<g89=reg128#3,#4
# asm 2: vshl.i32 >g89_19=q10,<g89=q2,#4
vshl.i32 q10,q2,#4

# qhasm:   f46 aligned= mem128[posf];posf += 16 
# asm 1: vld1.8 {>f46=reg128#12%bot->f46=reg128#12%top},[<posf=int32#3,: 128]!
# asm 2: vld1.8 {>f46=d22->f46=d23},[<posf=r2,: 128]!
vld1.8 {d22-d23},[r2,: 128]!

# qhasm:   4x g89_19 += g89 
# asm 1: vadd.i32 >g89_19=reg128#11,<g89_19=reg128#11,<g89=reg128#3
# asm 2: vadd.i32 >g89_19=q10,<g89_19=q10,<g89=q2
vadd.i32 q10,q10,q2

# qhasm:   new f89 

# qhasm:   f89 aligned= mem64[posf] f89[1] 
# asm 1: vld1.8 {<f89=reg128#13%bot},[<posf=int32#3,: 64]
# asm 2: vld1.8 {<f89=d24},[<posf=r2,: 64]
vld1.8 {d24},[r2,: 64]

# qhasm:   4x mix += g02 
# asm 1: vadd.i32 >mix=reg128#6,<mix=reg128#6,<g02=reg128#1
# asm 2: vadd.i32 >mix=q5,<mix=q5,<g02=q0
vadd.i32 q5,q5,q0

# qhasm:   posF = playground1_ptr + 192 
# asm 1: add >posF=int32#3,<playground1_ptr=int32#4,#192
# asm 2: add >posF=r2,<playground1_ptr=r3,#192
add r2,r3,#192

# qhasm:   f13 aligned= mem128[posF];posF += 16 
# asm 1: vld1.8 {>f13=reg128#14%bot->f13=reg128#14%top},[<posF=int32#3,: 128]!
# asm 2: vld1.8 {>f13=d26->f13=d27},[<posF=r2,: 128]!
vld1.8 {d26-d27},[r2,: 128]!

# qhasm:   4x g13_19 += g13 
# asm 1: vadd.i32 >g13_19=reg128#7,<g13_19=reg128#7,<g13=reg128#4
# asm 2: vadd.i32 >g13_19=q6,<g13_19=q6,<g13=q3
vadd.i32 q6,q6,q3

# qhasm:   f57 aligned= mem128[posF];posF += 16 
# asm 1: vld1.8 {>f57=reg128#15%bot->f57=reg128#15%top},[<posF=int32#3,: 128]!
# asm 2: vld1.8 {>f57=d28->f57=d29},[<posF=r2,: 128]!
vld1.8 {d28-d29},[r2,: 128]!

# qhasm:   4x g57_19 += g57 
# asm 1: vadd.i32 >g57_19=reg128#9,<g57_19=reg128#9,<g57=reg128#5
# asm 2: vadd.i32 >g57_19=q8,<g57_19=q8,<g57=q4
vadd.i32 q8,q8,q4

# qhasm:   f89 aligned= f89[0] mem64[posF] 
# asm 1: vld1.8 {<f89=reg128#13%top},[<posF=int32#3,: 64]
# asm 2: vld1.8 {<f89=d25},[<posF=r2,: 64]
vld1.8 {d25},[r2,: 64]

# qhasm:   4x g89_19 += g89 
# asm 1: vadd.i32 >g89_19=reg128#11,<g89_19=reg128#11,<g89=reg128#3
# asm 2: vadd.i32 >g89_19=q10,<g89_19=q10,<g89=q2
vadd.i32 q10,q10,q2

# qhasm:   f02 f13 = f02[0]f13[0] f02[2]f13[2] f02[1]f13[1] f02[3]f13[3] 
# asm 1: vtrn.32 <f02=reg128#10,<f13=reg128#14
# asm 2: vtrn.32 <f02=q9,<f13=q13
vtrn.32 q9,q13

# qhasm:   4x g46_19 += g46 
# asm 1: vadd.i32 >g46_19=reg128#8,<g46_19=reg128#8,<g46=reg128#2
# asm 2: vadd.i32 >g46_19=q7,<g46_19=q7,<g46=q1
vadd.i32 q7,q7,q1

# qhasm:   4x mix += g02 
# asm 1: vadd.i32 >mix=reg128#6,<mix=reg128#6,<g02=reg128#1
# asm 2: vadd.i32 >mix=q5,<mix=q5,<g02=q0
vadd.i32 q5,q5,q0

# qhasm:   f46 f57 = f46[0]f57[0] f46[2]f57[2] f46[1]f57[1] f46[3]f57[3] 
# asm 1: vtrn.32 <f46=reg128#12,<f57=reg128#15
# asm 2: vtrn.32 <f46=q11,<f57=q14
vtrn.32 q11,q14

# qhasm:   4x g13_19 += g13 
# asm 1: vadd.i32 >g13_19=reg128#7,<g13_19=reg128#7,<g13=reg128#4
# asm 2: vadd.i32 >g13_19=q6,<g13_19=q6,<g13=q3
vadd.i32 q6,q6,q3

# qhasm:   new g13_19_stack 

# qhasm:   ptr = &g13_19_stack 
# asm 1: lea >ptr=int32#3,<g13_19_stack=stack128#4
# asm 2: lea >ptr=r2,<g13_19_stack=[sp,#560]
add r2,sp,#560

# qhasm:   4x g89_19 += g89 
# asm 1: vadd.i32 >g89_19=reg128#11,<g89_19=reg128#11,<g89=reg128#3
# asm 2: vadd.i32 >g89_19=q10,<g89_19=q10,<g89=q2
vadd.i32 q10,q10,q2

# qhasm:   f89 = f89[0] f89[2] f89[1] f89[3] 
# asm 1: vtrn.32 <f89=reg128#13%bot,<f89=reg128#13%top
# asm 2: vtrn.32 <f89=d24,<f89=d25
vtrn.32 d24,d25

# qhasm:   mem128[ptr] aligned= g13_19 
# asm 1: vst1.8 {<g13_19=reg128#7%bot-<g13_19=reg128#7%top},[<ptr=int32#3,: 128]
# asm 2: vst1.8 {<g13_19=d12-<g13_19=d13},[<ptr=r2,: 128]
vst1.8 {d12-d13},[r2,: 128]

# qhasm:   4x f13_2 = f13 << 1 
# asm 1: vshl.i32 >f13_2=reg128#7,<f13=reg128#14,#1
# asm 2: vshl.i32 >f13_2=q6,<f13=q13,#1
vshl.i32 q6,q13,#1

# qhasm:   new g89_19_stack 

# qhasm:   ptr = &g89_19_stack 
# asm 1: lea >ptr=int32#3,<g89_19_stack=stack128#5
# asm 2: lea >ptr=r2,<g89_19_stack=[sp,#576]
add r2,sp,#576

# qhasm:   mem128[ptr] aligned= g89_19 
# asm 1: vst1.8 {<g89_19=reg128#11%bot-<g89_19=reg128#11%top},[<ptr=int32#3,: 128]
# asm 2: vst1.8 {<g89_19=d20-<g89_19=d21},[<ptr=r2,: 128]
vst1.8 {d20-d21},[r2,: 128]

# qhasm:   4x f57_2 = f57 << 1 
# asm 1: vshl.i32 >f57_2=reg128#11,<f57=reg128#15,#1
# asm 2: vshl.i32 >f57_2=q10,<f57=q14,#1
vshl.i32 q10,q14,#1

# qhasm:   new f13_2_stack 

# qhasm:   ptr = &f13_2_stack 
# asm 1: lea >ptr=int32#3,<f13_2_stack=stack128#6
# asm 2: lea >ptr=r2,<f13_2_stack=[sp,#592]
add r2,sp,#592

# qhasm:   mem128[ptr] aligned= f13_2 
# asm 1: vst1.8 {<f13_2=reg128#7%bot-<f13_2=reg128#7%top},[<ptr=int32#3,: 128]
# asm 2: vst1.8 {<f13_2=d12-<f13_2=d13},[<ptr=r2,: 128]
vst1.8 {d12-d13},[r2,: 128]

# qhasm:   4x f89_2 = f89 << 1 
# asm 1: vshl.i32 >f89_2=reg128#16,<f89=reg128#13,#1
# asm 2: vshl.i32 >f89_2=q15,<f89=q12,#1
vshl.i32 q15,q12,#1

# qhasm:   4x g57_19 += g57 
# asm 1: vadd.i32 >g57_19=reg128#9,<g57_19=reg128#9,<g57=reg128#5
# asm 2: vadd.i32 >g57_19=q8,<g57_19=q8,<g57=q4
vadd.i32 q8,q8,q4

# qhasm:   mix = f89_2[2,3] mix[2,3] 
# asm 1: vext.32 <mix=reg128#6%bot,<f89_2=reg128#16%top,<f89_2=reg128#16%bot,#0
# asm 2: vext.32 <mix=d10,<f89_2=d31,<f89_2=d30,#0
vext.32 d10,d31,d30,#0

# qhasm:   4x g46_19 += g46 
# asm 1: vadd.i32 >g46_19=reg128#8,<g46_19=reg128#8,<g46=reg128#2
# asm 2: vadd.i32 >g46_19=q7,<g46_19=q7,<g46=q1
vadd.i32 q7,q7,q1

# qhasm:   new g57_19_stack 

# qhasm:   ptr = &g57_19_stack 
# asm 1: lea >ptr=int32#3,<g57_19_stack=stack128#7
# asm 2: lea >ptr=r2,<g57_19_stack=[sp,#608]
add r2,sp,#608

# qhasm:   mem128[ptr] aligned= g57_19 
# asm 1: vst1.8 {<g57_19=reg128#9%bot-<g57_19=reg128#9%top},[<ptr=int32#3,: 128]
# asm 2: vst1.8 {<g57_19=d16-<g57_19=d17},[<ptr=r2,: 128]
vst1.8 {d16-d17},[r2,: 128]

# qhasm:   h9[0,1] = f02[0] signed* g89[2];h9[2,3] = f02[1] signed* g89[3] 
# asm 1: vmull.s32 >h9=reg128#9,<f02=reg128#10%bot,<g89=reg128#3%top
# asm 2: vmull.s32 >h9=q8,<f02=d18,<g89=d5
vmull.s32 q8,d18,d5

# qhasm:   h9[0,1] += f13[0] signed* g89[0];h9[2,3] += f13[1] signed* g89[1] 
# asm 1: vmlal.s32 <h9=reg128#9,<f13=reg128#14%bot,<g89=reg128#3%bot
# asm 2: vmlal.s32 <h9=q8,<f13=d26,<g89=d4
vmlal.s32 q8,d26,d4

# qhasm:   h9[0,1] += f02[2] signed* g57[2];h9[2,3] += f02[3] signed* g57[3] 
# asm 1: vmlal.s32 <h9=reg128#9,<f02=reg128#10%top,<g57=reg128#5%top
# asm 2: vmlal.s32 <h9=q8,<f02=d19,<g57=d9
vmlal.s32 q8,d19,d9

# qhasm:   h9[0,1] += f13[2] signed* g46[2];h9[2,3] += f13[3] signed* g46[3] 
# asm 1: vmlal.s32 <h9=reg128#9,<f13=reg128#14%top,<g46=reg128#2%top
# asm 2: vmlal.s32 <h9=q8,<f13=d27,<g46=d3
vmlal.s32 q8,d27,d3

# qhasm:   h9[0,1] += f46[0] signed* g57[0];h9[2,3] += f46[1] signed* g57[1] 
# asm 1: vmlal.s32 <h9=reg128#9,<f46=reg128#12%bot,<g57=reg128#5%bot
# asm 2: vmlal.s32 <h9=q8,<f46=d22,<g57=d8
vmlal.s32 q8,d22,d8

# qhasm:   h9[0,1] += f57[0] signed* g46[0];h9[2,3] += f57[1] signed* g46[1] 
# asm 1: vmlal.s32 <h9=reg128#9,<f57=reg128#15%bot,<g46=reg128#2%bot
# asm 2: vmlal.s32 <h9=q8,<f57=d28,<g46=d2
vmlal.s32 q8,d28,d2

# qhasm:   h9[0,1] += f46[2] signed* g13[2];h9[2,3] += f46[3] signed* g13[3] 
# asm 1: vmlal.s32 <h9=reg128#9,<f46=reg128#12%top,<g13=reg128#4%top
# asm 2: vmlal.s32 <h9=q8,<f46=d23,<g13=d7
vmlal.s32 q8,d23,d7

# qhasm:   h9[0,1] += f57[2] signed* g02[2];h9[2,3] += f57[3] signed* g02[3] 
# asm 1: vmlal.s32 <h9=reg128#9,<f57=reg128#15%top,<g02=reg128#1%top
# asm 2: vmlal.s32 <h9=q8,<f57=d29,<g02=d1
vmlal.s32 q8,d29,d1

# qhasm:   h9[0,1] += f89[0] signed* g13[0];h9[2,3] += f89[1] signed* g13[1] 
# asm 1: vmlal.s32 <h9=reg128#9,<f89=reg128#13%bot,<g13=reg128#4%bot
# asm 2: vmlal.s32 <h9=q8,<f89=d24,<g13=d6
vmlal.s32 q8,d24,d6

# qhasm:   h9[0,1] += f89[2] signed* g02[0];h9[2,3] += f89[3] signed* g02[1] 
# asm 1: vmlal.s32 <h9=reg128#9,<f89=reg128#13%top,<g02=reg128#1%bot
# asm 2: vmlal.s32 <h9=q8,<f89=d25,<g02=d0
vmlal.s32 q8,d25,d0

# qhasm:   new g46_19_stack 

# qhasm:   ptr = &g46_19_stack 
# asm 1: lea >ptr=int32#3,<g46_19_stack=stack128#8
# asm 2: lea >ptr=r2,<g46_19_stack=[sp,#624]
add r2,sp,#624

# qhasm:   mem128[ptr] aligned= g46_19 
# asm 1: vst1.8 {<g46_19=reg128#8%bot-<g46_19=reg128#8%top},[<ptr=int32#3,: 128]
# asm 2: vst1.8 {<g46_19=d14-<g46_19=d15},[<ptr=r2,: 128]
vst1.8 {d14-d15},[r2,: 128]

# qhasm:   h8[0,1] = f02[0] signed* g89[0];h8[2,3] = f02[1] signed* g89[1] 
# asm 1: vmull.s32 >h8=reg128#3,<f02=reg128#10%bot,<g89=reg128#3%bot
# asm 2: vmull.s32 >h8=q2,<f02=d18,<g89=d4
vmull.s32 q2,d18,d4

# qhasm:   h8[0,1] += f13_2[0] signed* g57[2];h8[2,3] += f13_2[1] signed* g57[3] 
# asm 1: vmlal.s32 <h8=reg128#3,<f13_2=reg128#7%bot,<g57=reg128#5%top
# asm 2: vmlal.s32 <h8=q2,<f13_2=d12,<g57=d9
vmlal.s32 q2,d12,d9

# qhasm:   h8[0,1] += f13_2[2] signed* g57[0];h8[2,3] += f13_2[3] signed* g57[1] 
# asm 1: vmlal.s32 <h8=reg128#3,<f13_2=reg128#7%top,<g57=reg128#5%bot
# asm 2: vmlal.s32 <h8=q2,<f13_2=d13,<g57=d8
vmlal.s32 q2,d13,d8

# qhasm:   h8[0,1] += f02[2] signed* g46[2];h8[2,3] += f02[3] signed* g46[3] 
# asm 1: vmlal.s32 <h8=reg128#3,<f02=reg128#10%top,<g46=reg128#2%top
# asm 2: vmlal.s32 <h8=q2,<f02=d19,<g46=d3
vmlal.s32 q2,d19,d3

# qhasm:   h8[0,1] += f46[0] signed* g46[0];h8[2,3] += f46[1] signed* g46[1] 
# asm 1: vmlal.s32 <h8=reg128#3,<f46=reg128#12%bot,<g46=reg128#2%bot
# asm 2: vmlal.s32 <h8=q2,<f46=d22,<g46=d2
vmlal.s32 q2,d22,d2

# qhasm:   h8[0,1] += f46[2] signed* g02[2];h8[2,3] += f46[3] signed* g02[3] 
# asm 1: vmlal.s32 <h8=reg128#3,<f46=reg128#12%top,<g02=reg128#1%top
# asm 2: vmlal.s32 <h8=q2,<f46=d23,<g02=d1
vmlal.s32 q2,d23,d1

# qhasm:   h8[0,1] += f89[0] signed* g02[0];h8[2,3] += f89[1] signed* g02[1] 
# asm 1: vmlal.s32 <h8=reg128#3,<f89=reg128#13%bot,<g02=reg128#1%bot
# asm 2: vmlal.s32 <h8=q2,<f89=d24,<g02=d0
vmlal.s32 q2,d24,d0

# qhasm:   new f57_2_stack 

# qhasm:   ptr = &f57_2_stack 
# asm 1: lea >ptr=int32#3,<f57_2_stack=stack128#9
# asm 2: lea >ptr=r2,<f57_2_stack=[sp,#640]
add r2,sp,#640

# qhasm:   mem128[ptr] aligned= f57_2 
# asm 1: vst1.8 {<f57_2=reg128#11%bot-<f57_2=reg128#11%top},[<ptr=int32#3,: 128]
# asm 2: vst1.8 {<f57_2=d20-<f57_2=d21},[<ptr=r2,: 128]
vst1.8 {d20-d21},[r2,: 128]

# qhasm:   h7[0,1] = f02[0] signed* g57[2];h7[2,3] = f02[1] signed* g57[3] 
# asm 1: vmull.s32 >h7=reg128#8,<f02=reg128#10%bot,<g57=reg128#5%top
# asm 2: vmull.s32 >h7=q7,<f02=d18,<g57=d9
vmull.s32 q7,d18,d9

# qhasm:   h7[0,1] += f13[0] signed* g46[2];h7[2,3] += f13[1] signed* g46[3] 
# asm 1: vmlal.s32 <h7=reg128#8,<f13=reg128#14%bot,<g46=reg128#2%top
# asm 2: vmlal.s32 <h7=q7,<f13=d26,<g46=d3
vmlal.s32 q7,d26,d3

# qhasm:   h7[0,1] += f02[2] signed* g57[0];h7[2,3] += f02[3] signed* g57[1] 
# asm 1: vmlal.s32 <h7=reg128#8,<f02=reg128#10%top,<g57=reg128#5%bot
# asm 2: vmlal.s32 <h7=q7,<f02=d19,<g57=d8
vmlal.s32 q7,d19,d8

# qhasm:   h7[0,1] += f13[2] signed* g46[0];h7[2,3] += f13[3] signed* g46[1] 
# asm 1: vmlal.s32 <h7=reg128#8,<f13=reg128#14%top,<g46=reg128#2%bot
# asm 2: vmlal.s32 <h7=q7,<f13=d27,<g46=d2
vmlal.s32 q7,d27,d2

# qhasm:   h7[0,1] += f46[0] signed* g13[2];h7[2,3] += f46[1] signed* g13[3] 
# asm 1: vmlal.s32 <h7=reg128#8,<f46=reg128#12%bot,<g13=reg128#4%top
# asm 2: vmlal.s32 <h7=q7,<f46=d22,<g13=d7
vmlal.s32 q7,d22,d7

# qhasm:   h7[0,1] += f57[0] signed* g02[2];h7[2,3] += f57[1] signed* g02[3] 
# asm 1: vmlal.s32 <h7=reg128#8,<f57=reg128#15%bot,<g02=reg128#1%top
# asm 2: vmlal.s32 <h7=q7,<f57=d28,<g02=d1
vmlal.s32 q7,d28,d1

# qhasm:   h7[0,1] += f46[2] signed* g13[0];h7[2,3] += f46[3] signed* g13[1] 
# asm 1: vmlal.s32 <h7=reg128#8,<f46=reg128#12%top,<g13=reg128#4%bot
# asm 2: vmlal.s32 <h7=q7,<f46=d23,<g13=d6
vmlal.s32 q7,d23,d6

# qhasm:   h7[0,1] += f57[2] signed* g02[0];h7[2,3] += f57[3] signed* g02[1] 
# asm 1: vmlal.s32 <h7=reg128#8,<f57=reg128#15%top,<g02=reg128#1%bot
# asm 2: vmlal.s32 <h7=q7,<f57=d29,<g02=d0
vmlal.s32 q7,d29,d0

# qhasm:   new mix_stack 

# qhasm:   ptr = &mix_stack 
# asm 1: lea >ptr=int32#3,<mix_stack=stack128#10
# asm 2: lea >ptr=r2,<mix_stack=[sp,#656]
add r2,sp,#656

# qhasm:   mem128[ptr] aligned= mix 
# asm 1: vst1.8 {<mix=reg128#6%bot-<mix=reg128#6%top},[<ptr=int32#3,: 128]
# asm 2: vst1.8 {<mix=d10-<mix=d11},[<ptr=r2,: 128]
vst1.8 {d10-d11},[r2,: 128]

# qhasm:   h6[0,1] = f02[0] signed* g46[2];h6[2,3] = f02[1] signed* g46[3] 
# asm 1: vmull.s32 >h6=reg128#6,<f02=reg128#10%bot,<g46=reg128#2%top
# asm 2: vmull.s32 >h6=q5,<f02=d18,<g46=d3
vmull.s32 q5,d18,d3

# qhasm:   h6[0,1] += f02[2] signed* g46[0];h6[2,3] += f02[3] signed* g46[1] 
# asm 1: vmlal.s32 <h6=reg128#6,<f02=reg128#10%top,<g46=reg128#2%bot
# asm 2: vmlal.s32 <h6=q5,<f02=d19,<g46=d2
vmlal.s32 q5,d19,d2

# qhasm:   h6[0,1] += f46[0] signed* g02[2];h6[2,3] += f46[1] signed* g02[3] 
# asm 1: vmlal.s32 <h6=reg128#6,<f46=reg128#12%bot,<g02=reg128#1%top
# asm 2: vmlal.s32 <h6=q5,<f46=d22,<g02=d1
vmlal.s32 q5,d22,d1

# qhasm:   h6[0,1] += f46[2] signed* g02[0];h6[2,3] += f46[3] signed* g02[1] 
# asm 1: vmlal.s32 <h6=reg128#6,<f46=reg128#12%top,<g02=reg128#1%bot
# asm 2: vmlal.s32 <h6=q5,<f46=d23,<g02=d0
vmlal.s32 q5,d23,d0

# qhasm:   h6[0,1] += f13_2[0] signed* g57[0];h6[2,3] += f13_2[1] signed* g57[1] 
# asm 1: vmlal.s32 <h6=reg128#6,<f13_2=reg128#7%bot,<g57=reg128#5%bot
# asm 2: vmlal.s32 <h6=q5,<f13_2=d12,<g57=d8
vmlal.s32 q5,d12,d8

# qhasm:   new h9_stack 

# qhasm:   ptr = &h9_stack 
# asm 1: lea >ptr=int32#3,<h9_stack=stack128#11
# asm 2: lea >ptr=r2,<h9_stack=[sp,#672]
add r2,sp,#672

# qhasm:   mem128[ptr] aligned= h9 
# asm 1: vst1.8 {<h9=reg128#9%bot-<h9=reg128#9%top},[<ptr=int32#3,: 128]
# asm 2: vst1.8 {<h9=d16-<h9=d17},[<ptr=r2,: 128]
vst1.8 {d16-d17},[r2,: 128]

# qhasm:   h5[0,1] = f02[0] signed* g57[0];h5[2,3] = f02[1] signed* g57[1] 
# asm 1: vmull.s32 >h5=reg128#5,<f02=reg128#10%bot,<g57=reg128#5%bot
# asm 2: vmull.s32 >h5=q4,<f02=d18,<g57=d8
vmull.s32 q4,d18,d8

# qhasm:   h5[0,1] += f13[0] signed* g46[0];h5[2,3] += f13[1] signed* g46[1] 
# asm 1: vmlal.s32 <h5=reg128#5,<f13=reg128#14%bot,<g46=reg128#2%bot
# asm 2: vmlal.s32 <h5=q4,<f13=d26,<g46=d2
vmlal.s32 q4,d26,d2

# qhasm:   h5[0,1] += f02[2] signed* g13[2];h5[2,3] += f02[3] signed* g13[3] 
# asm 1: vmlal.s32 <h5=reg128#5,<f02=reg128#10%top,<g13=reg128#4%top
# asm 2: vmlal.s32 <h5=q4,<f02=d19,<g13=d7
vmlal.s32 q4,d19,d7

# qhasm:   h5[0,1] += f13[2] signed* g02[2];h5[2,3] += f13[3] signed* g02[3] 
# asm 1: vmlal.s32 <h5=reg128#5,<f13=reg128#14%top,<g02=reg128#1%top
# asm 2: vmlal.s32 <h5=q4,<f13=d27,<g02=d1
vmlal.s32 q4,d27,d1

# qhasm:   h5[0,1] += f46[0] signed* g13[0];h5[2,3] += f46[1] signed* g13[1] 
# asm 1: vmlal.s32 <h5=reg128#5,<f46=reg128#12%bot,<g13=reg128#4%bot
# asm 2: vmlal.s32 <h5=q4,<f46=d22,<g13=d6
vmlal.s32 q4,d22,d6

# qhasm:   h5[0,1] += f57[0] signed* g02[0];h5[2,3] += f57[1] signed* g02[1] 
# asm 1: vmlal.s32 <h5=reg128#5,<f57=reg128#15%bot,<g02=reg128#1%bot
# asm 2: vmlal.s32 <h5=q4,<f57=d28,<g02=d0
vmlal.s32 q4,d28,d0

# qhasm:   h3[0,1] = f02[0] signed* g13[2];h3[2,3] = f02[1] signed* g13[3] 
# asm 1: vmull.s32 >h3=reg128#9,<f02=reg128#10%bot,<g13=reg128#4%top
# asm 2: vmull.s32 >h3=q8,<f02=d18,<g13=d7
vmull.s32 q8,d18,d7

# qhasm:   h3[0,1] += f13[0] signed* g02[2];h3[2,3] += f13[1] signed* g02[3] 
# asm 1: vmlal.s32 <h3=reg128#9,<f13=reg128#14%bot,<g02=reg128#1%top
# asm 2: vmlal.s32 <h3=q8,<f13=d26,<g02=d1
vmlal.s32 q8,d26,d1

# qhasm:   h3[0,1] += f02[2] signed* g13[0];h3[2,3] += f02[3] signed* g13[1] 
# asm 1: vmlal.s32 <h3=reg128#9,<f02=reg128#10%top,<g13=reg128#4%bot
# asm 2: vmlal.s32 <h3=q8,<f02=d19,<g13=d6
vmlal.s32 q8,d19,d6

# qhasm:   h3[0,1] += f13[2] signed* g02[0];h3[2,3] += f13[3] signed* g02[1] 
# asm 1: vmlal.s32 <h3=reg128#9,<f13=reg128#14%top,<g02=reg128#1%bot
# asm 2: vmlal.s32 <h3=q8,<f13=d27,<g02=d0
vmlal.s32 q8,d27,d0

# qhasm:   ptr = &g89_19_stack 
# asm 1: lea >ptr=int32#3,<g89_19_stack=stack128#5
# asm 2: lea >ptr=r2,<g89_19_stack=[sp,#576]
add r2,sp,#576

# qhasm:   g89_19 aligned= mem128[ptr] 
# asm 1: vld1.8 {>g89_19=reg128#11%bot->g89_19=reg128#11%top},[<ptr=int32#3,: 128]
# asm 2: vld1.8 {>g89_19=d20->g89_19=d21},[<ptr=r2,: 128]
vld1.8 {d20-d21},[r2,: 128]

# qhasm:   h7[0,1] += f89[0] signed* g89_19[2];h7[2,3] += f89[1] signed* g89_19[3] 
# asm 1: vmlal.s32 <h7=reg128#8,<f89=reg128#13%bot,<g89_19=reg128#11%top
# asm 2: vmlal.s32 <h7=q7,<f89=d24,<g89_19=d21
vmlal.s32 q7,d24,d21

# qhasm:   h7[0,1] += f89[2] signed* g89_19[0];h7[2,3] += f89[3] signed* g89_19[1] 
# asm 1: vmlal.s32 <h7=reg128#8,<f89=reg128#13%top,<g89_19=reg128#11%bot
# asm 2: vmlal.s32 <h7=q7,<f89=d25,<g89_19=d20
vmlal.s32 q7,d25,d20

# qhasm:   h5[0,1] += f46[2] signed* g89_19[2];h5[2,3] += f46[3] signed* g89_19[3] 
# asm 1: vmlal.s32 <h5=reg128#5,<f46=reg128#12%top,<g89_19=reg128#11%top
# asm 2: vmlal.s32 <h5=q4,<f46=d23,<g89_19=d21
vmlal.s32 q4,d23,d21

# qhasm:   h5[0,1] += f57[2] signed* g89_19[0];h5[2,3] += f57[3] signed* g89_19[1] 
# asm 1: vmlal.s32 <h5=reg128#5,<f57=reg128#15%top,<g89_19=reg128#11%bot
# asm 2: vmlal.s32 <h5=q4,<f57=d29,<g89_19=d20
vmlal.s32 q4,d29,d20

# qhasm:   h3[0,1] += f46[0] signed* g89_19[2];h3[2,3] += f46[1] signed* g89_19[3] 
# asm 1: vmlal.s32 <h3=reg128#9,<f46=reg128#12%bot,<g89_19=reg128#11%top
# asm 2: vmlal.s32 <h3=q8,<f46=d22,<g89_19=d21
vmlal.s32 q8,d22,d21

# qhasm:   h3[0,1] += f57[0] signed* g89_19[0];h3[2,3] += f57[1] signed* g89_19[1] 
# asm 1: vmlal.s32 <h3=reg128#9,<f57=reg128#15%bot,<g89_19=reg128#11%bot
# asm 2: vmlal.s32 <h3=q8,<f57=d28,<g89_19=d20
vmlal.s32 q8,d28,d20

# qhasm:   h6[0,1] += f89[0] signed* g89_19[0];h6[2,3] += f89[1] signed* g89_19[1] 
# asm 1: vmlal.s32 <h6=reg128#6,<f89=reg128#13%bot,<g89_19=reg128#11%bot
# asm 2: vmlal.s32 <h6=q5,<f89=d24,<g89_19=d20
vmlal.s32 q5,d24,d20

# qhasm:   new h7_stack 

# qhasm:   ptr = &h7_stack 
# asm 1: lea >ptr=int32#3,<h7_stack=stack128#5
# asm 2: lea >ptr=r2,<h7_stack=[sp,#576]
add r2,sp,#576

# qhasm:   mem128[ptr] aligned= h7 
# asm 1: vst1.8 {<h7=reg128#8%bot-<h7=reg128#8%top},[<ptr=int32#3,: 128]
# asm 2: vst1.8 {<h7=d14-<h7=d15},[<ptr=r2,: 128]
vst1.8 {d14-d15},[r2,: 128]

# qhasm:   h1[0,1] = f02[0] signed* g13[0];h1[2,3] = f02[1] signed* g13[1] 
# asm 1: vmull.s32 >h1=reg128#8,<f02=reg128#10%bot,<g13=reg128#4%bot
# asm 2: vmull.s32 >h1=q7,<f02=d18,<g13=d6
vmull.s32 q7,d18,d6

# qhasm:   h1[0,1] += f13[0] signed* g02[0];h1[2,3] += f13[1] signed* g02[1] 
# asm 1: vmlal.s32 <h1=reg128#8,<f13=reg128#14%bot,<g02=reg128#1%bot
# asm 2: vmlal.s32 <h1=q7,<f13=d26,<g02=d0
vmlal.s32 q7,d26,d0

# qhasm:   ptr = &mix_stack 
# asm 1: lea >ptr=int32#3,<mix_stack=stack128#10
# asm 2: lea >ptr=r2,<mix_stack=[sp,#656]
add r2,sp,#656

# qhasm:   mix aligned= mem128[ptr] 
# asm 1: vld1.8 {>mix=reg128#16%bot->mix=reg128#16%top},[<ptr=int32#3,: 128]
# asm 2: vld1.8 {>mix=d30->mix=d31},[<ptr=r2,: 128]
vld1.8 {d30-d31},[r2,: 128]

# qhasm:   h8[0,1] += mix[0] signed* g89_19[2];h8[2,3] += mix[1] signed* g89_19[3] 
# asm 1: vmlal.s32 <h8=reg128#3,<mix=reg128#16%bot,<g89_19=reg128#11%top
# asm 2: vmlal.s32 <h8=q2,<mix=d30,<g89_19=d21
vmlal.s32 q2,d30,d21

# qhasm:   h1[0,1] += f02[2] signed* g89_19[2];h1[2,3] += f02[3] signed* g89_19[3] 
# asm 1: vmlal.s32 <h1=reg128#8,<f02=reg128#10%top,<g89_19=reg128#11%top
# asm 2: vmlal.s32 <h1=q7,<f02=d19,<g89_19=d21
vmlal.s32 q7,d19,d21

# qhasm:   h1[0,1] += f13[2] signed* g89_19[0];h1[2,3] += f13[3] signed* g89_19[1] 
# asm 1: vmlal.s32 <h1=reg128#8,<f13=reg128#14%top,<g89_19=reg128#11%bot
# asm 2: vmlal.s32 <h1=q7,<f13=d27,<g89_19=d20
vmlal.s32 q7,d27,d20

# qhasm:   ptr = &g46_19_stack 
# asm 1: lea >ptr=int32#3,<g46_19_stack=stack128#8
# asm 2: lea >ptr=r2,<g46_19_stack=[sp,#624]
add r2,sp,#624

# qhasm:   g46_19 aligned= mem128[ptr] 
# asm 1: vld1.8 {>g46_19=reg128#14%bot->g46_19=reg128#14%top},[<ptr=int32#3,: 128]
# asm 2: vld1.8 {>g46_19=d26->g46_19=d27},[<ptr=r2,: 128]
vld1.8 {d26-d27},[r2,: 128]

# qhasm:   h5[0,1] += f89[2] signed* g46_19[2];h5[2,3] += f89[3] signed* g46_19[3] 
# asm 1: vmlal.s32 <h5=reg128#5,<f89=reg128#13%top,<g46_19=reg128#14%top
# asm 2: vmlal.s32 <h5=q4,<f89=d25,<g46_19=d27
vmlal.s32 q4,d25,d27

# qhasm:   h3[0,1] += f57[2] signed* g46_19[2];h3[2,3] += f57[3] signed* g46_19[3] 
# asm 1: vmlal.s32 <h3=reg128#9,<f57=reg128#15%top,<g46_19=reg128#14%top
# asm 2: vmlal.s32 <h3=q8,<f57=d29,<g46_19=d27
vmlal.s32 q8,d29,d27

# qhasm:   h3[0,1] += f89[2] signed* g46_19[0];h3[2,3] += f89[3] signed* g46_19[1] 
# asm 1: vmlal.s32 <h3=reg128#9,<f89=reg128#13%top,<g46_19=reg128#14%bot
# asm 2: vmlal.s32 <h3=q8,<f89=d25,<g46_19=d26
vmlal.s32 q8,d25,d26

# qhasm:   h1[0,1] += f57[0] signed* g46_19[2];h1[2,3] += f57[1] signed* g46_19[3] 
# asm 1: vmlal.s32 <h1=reg128#8,<f57=reg128#15%bot,<g46_19=reg128#14%top
# asm 2: vmlal.s32 <h1=q7,<f57=d28,<g46_19=d27
vmlal.s32 q7,d28,d27

# qhasm:   h1[0,1] += f57[2] signed* g46_19[0];h1[2,3] += f57[3] signed* g46_19[1] 
# asm 1: vmlal.s32 <h1=reg128#8,<f57=reg128#15%top,<g46_19=reg128#14%bot
# asm 2: vmlal.s32 <h1=q7,<f57=d29,<g46_19=d26
vmlal.s32 q7,d29,d26

# qhasm:   ptr = &g57_19_stack 
# asm 1: lea >ptr=int32#3,<g57_19_stack=stack128#7
# asm 2: lea >ptr=r2,<g57_19_stack=[sp,#608]
add r2,sp,#608

# qhasm:   g57_19 aligned= mem128[ptr] 
# asm 1: vld1.8 {>g57_19=reg128#15%bot->g57_19=reg128#15%top},[<ptr=int32#3,: 128]
# asm 2: vld1.8 {>g57_19=d28->g57_19=d29},[<ptr=r2,: 128]
vld1.8 {d28-d29},[r2,: 128]

# qhasm:   h5[0,1] += f89[0] signed* g57_19[2];h5[2,3] += f89[1] signed* g57_19[3] 
# asm 1: vmlal.s32 <h5=reg128#5,<f89=reg128#13%bot,<g57_19=reg128#15%top
# asm 2: vmlal.s32 <h5=q4,<f89=d24,<g57_19=d29
vmlal.s32 q4,d24,d29

# qhasm:   h3[0,1] += f46[2] signed* g57_19[2];h3[2,3] += f46[3] signed* g57_19[3] 
# asm 1: vmlal.s32 <h3=reg128#9,<f46=reg128#12%top,<g57_19=reg128#15%top
# asm 2: vmlal.s32 <h3=q8,<f46=d23,<g57_19=d29
vmlal.s32 q8,d23,d29

# qhasm:   h3[0,1] += f89[0] signed* g57_19[0];h3[2,3] += f89[1] signed* g57_19[1] 
# asm 1: vmlal.s32 <h3=reg128#9,<f89=reg128#13%bot,<g57_19=reg128#15%bot
# asm 2: vmlal.s32 <h3=q8,<f89=d24,<g57_19=d28
vmlal.s32 q8,d24,d28

# qhasm:   h1[0,1] += f46[0] signed* g57_19[2];h1[2,3] += f46[1] signed* g57_19[3] 
# asm 1: vmlal.s32 <h1=reg128#8,<f46=reg128#12%bot,<g57_19=reg128#15%top
# asm 2: vmlal.s32 <h1=q7,<f46=d22,<g57_19=d29
vmlal.s32 q7,d22,d29

# qhasm:   h1[0,1] += f46[2] signed* g57_19[0];h1[2,3] += f46[3] signed* g57_19[1] 
# asm 1: vmlal.s32 <h1=reg128#8,<f46=reg128#12%top,<g57_19=reg128#15%bot
# asm 2: vmlal.s32 <h1=q7,<f46=d23,<g57_19=d28
vmlal.s32 q7,d23,d28

# qhasm:   new h5_stack 

# qhasm:   ptr = &h5_stack 
# asm 1: lea >ptr=int32#3,<h5_stack=stack128#7
# asm 2: lea >ptr=r2,<h5_stack=[sp,#608]
add r2,sp,#608

# qhasm:   mem128[ptr] aligned= h5 
# asm 1: vst1.8 {<h5=reg128#5%bot-<h5=reg128#5%top},[<ptr=int32#3,: 128]
# asm 2: vst1.8 {<h5=d8-<h5=d9},[<ptr=r2,: 128]
vst1.8 {d8-d9},[r2,: 128]

# qhasm:   ptr = &g13_19_stack 
# asm 1: lea >ptr=int32#3,<g13_19_stack=stack128#4
# asm 2: lea >ptr=r2,<g13_19_stack=[sp,#560]
add r2,sp,#560

# qhasm:   g13_19 aligned= mem128[ptr] 
# asm 1: vld1.8 {>g13_19=reg128#5%bot->g13_19=reg128#5%top},[<ptr=int32#3,: 128]
# asm 2: vld1.8 {>g13_19=d8->g13_19=d9},[<ptr=r2,: 128]
vld1.8 {d8-d9},[r2,: 128]

# qhasm:   h1[0,1] += f89[0] signed* g13_19[2];h1[2,3] += f89[1] signed* g13_19[3] 
# asm 1: vmlal.s32 <h1=reg128#8,<f89=reg128#13%bot,<g13_19=reg128#5%top
# asm 2: vmlal.s32 <h1=q7,<f89=d24,<g13_19=d9
vmlal.s32 q7,d24,d9

# qhasm:   h1[0,1] += f89[2] signed* mix[2];h1[2,3] += f89[3] signed* mix[3] 
# asm 1: vmlal.s32 <h1=reg128#8,<f89=reg128#13%top,<mix=reg128#16%top
# asm 2: vmlal.s32 <h1=q7,<f89=d25,<mix=d31
vmlal.s32 q7,d25,d31

# qhasm:   h4[0,1] = f02[0] signed* g46[0];h4[2,3] = f02[1] signed* g46[1] 
# asm 1: vmull.s32 >h4=reg128#2,<f02=reg128#10%bot,<g46=reg128#2%bot
# asm 2: vmull.s32 >h4=q1,<f02=d18,<g46=d2
vmull.s32 q1,d18,d2

# qhasm:   h4[0,1] += f02[2] signed* g02[2];h4[2,3] += f02[3] signed* g02[3] 
# asm 1: vmlal.s32 <h4=reg128#2,<f02=reg128#10%top,<g02=reg128#1%top
# asm 2: vmlal.s32 <h4=q1,<f02=d19,<g02=d1
vmlal.s32 q1,d19,d1

# qhasm:   h4[0,1] += f46[0] signed* g02[0];h4[2,3] += f46[1] signed* g02[1] 
# asm 1: vmlal.s32 <h4=reg128#2,<f46=reg128#12%bot,<g02=reg128#1%bot
# asm 2: vmlal.s32 <h4=q1,<f46=d22,<g02=d0
vmlal.s32 q1,d22,d0

# qhasm:   h4[0,1] += f89[0] signed* g46_19[2];h4[2,3] += f89[1] signed* g46_19[3] 
# asm 1: vmlal.s32 <h4=reg128#2,<f89=reg128#13%bot,<g46_19=reg128#14%top
# asm 2: vmlal.s32 <h4=q1,<f89=d24,<g46_19=d27
vmlal.s32 q1,d24,d27

# qhasm:   h4[0,1] += f46[2] signed* g89_19[0];h4[2,3] += f46[3] signed* g89_19[1] 
# asm 1: vmlal.s32 <h4=reg128#2,<f46=reg128#12%top,<g89_19=reg128#11%bot
# asm 2: vmlal.s32 <h4=q1,<f46=d23,<g89_19=d20
vmlal.s32 q1,d23,d20

# qhasm:   h4[0,1] += f13_2[0] signed* g13[2];h4[2,3] += f13_2[1] signed* g13[3] 
# asm 1: vmlal.s32 <h4=reg128#2,<f13_2=reg128#7%bot,<g13=reg128#4%top
# asm 2: vmlal.s32 <h4=q1,<f13_2=d12,<g13=d7
vmlal.s32 q1,d12,d7

# qhasm:   h4[0,1] += f13_2[2] signed* g13[0];h4[2,3] += f13_2[3] signed* g13[1] 
# asm 1: vmlal.s32 <h4=reg128#2,<f13_2=reg128#7%top,<g13=reg128#4%bot
# asm 2: vmlal.s32 <h4=q1,<f13_2=d13,<g13=d6
vmlal.s32 q1,d13,d6

# qhasm:   h2[0,1] = f02[0] signed* g02[2];h2[2,3] = f02[1] signed* g02[3] 
# asm 1: vmull.s32 >h2=reg128#7,<f02=reg128#10%bot,<g02=reg128#1%top
# asm 2: vmull.s32 >h2=q6,<f02=d18,<g02=d1
vmull.s32 q6,d18,d1

# qhasm:   h2[0,1] += f02[2] signed* g02[0];h2[2,3] += f02[3] signed* g02[1] 
# asm 1: vmlal.s32 <h2=reg128#7,<f02=reg128#10%top,<g02=reg128#1%bot
# asm 2: vmlal.s32 <h2=q6,<f02=d19,<g02=d0
vmlal.s32 q6,d19,d0

# qhasm:   h2[0,1] += f46[2] signed* g46_19[2];h2[2,3] += f46[3] signed* g46_19[3] 
# asm 1: vmlal.s32 <h2=reg128#7,<f46=reg128#12%top,<g46_19=reg128#14%top
# asm 2: vmlal.s32 <h2=q6,<f46=d23,<g46_19=d27
vmlal.s32 q6,d23,d27

# qhasm:   h2[0,1] += f46[0] signed* g89_19[0];h2[2,3] += f46[1] signed* g89_19[1] 
# asm 1: vmlal.s32 <h2=reg128#7,<f46=reg128#12%bot,<g89_19=reg128#11%bot
# asm 2: vmlal.s32 <h2=q6,<f46=d22,<g89_19=d20
vmlal.s32 q6,d22,d20

# qhasm:   h2[0,1] += f89[0] signed* g46_19[0];h2[2,3] += f89[1] signed* g46_19[1] 
# asm 1: vmlal.s32 <h2=reg128#7,<f89=reg128#13%bot,<g46_19=reg128#14%bot
# asm 2: vmlal.s32 <h2=q6,<f89=d24,<g46_19=d26
vmlal.s32 q6,d24,d26

# qhasm:   h0[0,1] = f02[0] signed* g02[0];h0[2,3] = f02[1] signed* g02[1] 
# asm 1: vmull.s32 >h0=reg128#1,<f02=reg128#10%bot,<g02=reg128#1%bot
# asm 2: vmull.s32 >h0=q0,<f02=d18,<g02=d0
vmull.s32 q0,d18,d0

# qhasm:   h0[0,1] += f46[0] signed* g46_19[2];h0[2,3] += f46[1] signed* g46_19[3] 
# asm 1: vmlal.s32 <h0=reg128#1,<f46=reg128#12%bot,<g46_19=reg128#14%top
# asm 2: vmlal.s32 <h0=q0,<f46=d22,<g46_19=d27
vmlal.s32 q0,d22,d27

# qhasm:   h0[0,1] += f46[2] signed* g46_19[0];h0[2,3] += f46[3] signed* g46_19[1] 
# asm 1: vmlal.s32 <h0=reg128#1,<f46=reg128#12%top,<g46_19=reg128#14%bot
# asm 2: vmlal.s32 <h0=q0,<f46=d23,<g46_19=d26
vmlal.s32 q0,d23,d26

# qhasm:   h0[0,1] += f89[0] signed* mix[2];h0[2,3] += f89[1] signed* mix[3] 
# asm 1: vmlal.s32 <h0=reg128#1,<f89=reg128#13%bot,<mix=reg128#16%top
# asm 2: vmlal.s32 <h0=q0,<f89=d24,<mix=d31
vmlal.s32 q0,d24,d31

# qhasm:   h0[0,1] += f02[2] signed* g89_19[0];h0[2,3] += f02[3] signed* g89_19[1] 
# asm 1: vmlal.s32 <h0=reg128#1,<f02=reg128#10%top,<g89_19=reg128#11%bot
# asm 2: vmlal.s32 <h0=q0,<f02=d19,<g89_19=d20
vmlal.s32 q0,d19,d20

# qhasm:   ptr = &f57_2_stack 
# asm 1: lea >ptr=int32#3,<f57_2_stack=stack128#9
# asm 2: lea >ptr=r2,<f57_2_stack=[sp,#640]
add r2,sp,#640

# qhasm:   f57_2 aligned= mem128[ptr] 
# asm 1: vld1.8 {>f57_2=reg128#10%bot->f57_2=reg128#10%top},[<ptr=int32#3,: 128]
# asm 2: vld1.8 {>f57_2=d18->f57_2=d19},[<ptr=r2,: 128]
vld1.8 {d18-d19},[r2,: 128]

# qhasm:   h8[0,1] += f57_2[0] signed* g13[2];h8[2,3] += f57_2[1] signed* g13[3] 
# asm 1: vmlal.s32 <h8=reg128#3,<f57_2=reg128#10%bot,<g13=reg128#4%top
# asm 2: vmlal.s32 <h8=q2,<f57_2=d18,<g13=d7
vmlal.s32 q2,d18,d7

# qhasm:   h8[0,1] += f57_2[2] signed* g13[0];h8[2,3] += f57_2[3] signed* g13[1] 
# asm 1: vmlal.s32 <h8=reg128#3,<f57_2=reg128#10%top,<g13=reg128#4%bot
# asm 2: vmlal.s32 <h8=q2,<f57_2=d19,<g13=d6
vmlal.s32 q2,d19,d6

# qhasm:   h6[0,1] += f57_2[0] signed* g13[0];h6[2,3] += f57_2[1] signed* g13[1] 
# asm 1: vmlal.s32 <h6=reg128#6,<f57_2=reg128#10%bot,<g13=reg128#4%bot
# asm 2: vmlal.s32 <h6=q5,<f57_2=d18,<g13=d6
vmlal.s32 q5,d18,d6

# qhasm:   h6[0,1] += f57_2[2] signed* g89_19[2];h6[2,3] += f57_2[3] signed* g89_19[3] 
# asm 1: vmlal.s32 <h6=reg128#6,<f57_2=reg128#10%top,<g89_19=reg128#11%top
# asm 2: vmlal.s32 <h6=q5,<f57_2=d19,<g89_19=d21
vmlal.s32 q5,d19,d21

# qhasm:   h4[0,1] += f57_2[0] signed* g89_19[2];h4[2,3] += f57_2[1] signed* g89_19[3] 
# asm 1: vmlal.s32 <h4=reg128#2,<f57_2=reg128#10%bot,<g89_19=reg128#11%top
# asm 2: vmlal.s32 <h4=q1,<f57_2=d18,<g89_19=d21
vmlal.s32 q1,d18,d21

# qhasm:   h4[0,1] += f57_2[2] signed* g57_19[2];h4[2,3] += f57_2[3] signed* g57_19[3] 
# asm 1: vmlal.s32 <h4=reg128#2,<f57_2=reg128#10%top,<g57_19=reg128#15%top
# asm 2: vmlal.s32 <h4=q1,<f57_2=d19,<g57_19=d29
vmlal.s32 q1,d19,d29

# qhasm:   h0[0,1] += f57_2[0] signed* g57_19[0];h0[2,3] += f57_2[1] signed* g57_19[1] 
# asm 1: vmlal.s32 <h0=reg128#1,<f57_2=reg128#10%bot,<g57_19=reg128#15%bot
# asm 2: vmlal.s32 <h0=q0,<f57_2=d18,<g57_19=d28
vmlal.s32 q0,d18,d28

# qhasm:   h0[0,1] += f57_2[2] signed* g13_19[2];h0[2,3] += f57_2[3] signed* g13_19[3] 
# asm 1: vmlal.s32 <h0=reg128#1,<f57_2=reg128#10%top,<g13_19=reg128#5%top
# asm 2: vmlal.s32 <h0=q0,<f57_2=d19,<g13_19=d9
vmlal.s32 q0,d19,d9

# qhasm:   h2[0,1] += f57_2[0] signed* g57_19[2];h2[2,3] += f57_2[1] signed* g57_19[3] 
# asm 1: vmlal.s32 <h2=reg128#7,<f57_2=reg128#10%bot,<g57_19=reg128#15%top
# asm 2: vmlal.s32 <h2=q6,<f57_2=d18,<g57_19=d29
vmlal.s32 q6,d18,d29

# qhasm:   h2[0,1] += f57_2[2] signed* g57_19[0];h2[2,3] += f57_2[3] signed* g57_19[1] 
# asm 1: vmlal.s32 <h2=reg128#7,<f57_2=reg128#10%top,<g57_19=reg128#15%bot
# asm 2: vmlal.s32 <h2=q6,<f57_2=d19,<g57_19=d28
vmlal.s32 q6,d19,d28

# qhasm:   ptr = &f13_2_stack 
# asm 1: lea >ptr=int32#3,<f13_2_stack=stack128#6
# asm 2: lea >ptr=r2,<f13_2_stack=[sp,#592]
add r2,sp,#592

# qhasm:   f13_2 aligned= mem128[ptr] 
# asm 1: vld1.8 {>f13_2=reg128#10%bot->f13_2=reg128#10%top},[<ptr=int32#3,: 128]
# asm 2: vld1.8 {>f13_2=d18->f13_2=d19},[<ptr=r2,: 128]
vld1.8 {d18-d19},[r2,: 128]

# qhasm:   ptr = &_0x2000000_stack 
# asm 1: lea >ptr=int32#3,<_0x2000000_stack=stack128#1
# asm 2: lea >ptr=r2,<_0x2000000_stack=[sp,#512]
add r2,sp,#512

# qhasm:   _0x2000000 aligned= mem128[ptr] 
# asm 1: vld1.8 {>_0x2000000=reg128#12%bot->_0x2000000=reg128#12%top},[<ptr=int32#3,: 128]
# asm 2: vld1.8 {>_0x2000000=d22->_0x2000000=d23},[<ptr=r2,: 128]
vld1.8 {d22-d23},[r2,: 128]

# qhasm:   h6[0,1] += f13_2[2] signed* g13[2];h6[2,3] += f13_2[3] signed* g13[3] 
# asm 1: vmlal.s32 <h6=reg128#6,<f13_2=reg128#10%top,<g13=reg128#4%top
# asm 2: vmlal.s32 <h6=q5,<f13_2=d19,<g13=d7
vmlal.s32 q5,d19,d7

# qhasm:   h0[0,1] += f13_2[0] signed* g89_19[2];h0[2,3] += f13_2[1] signed* g89_19[3] 
# asm 1: vmlal.s32 <h0=reg128#1,<f13_2=reg128#10%bot,<g89_19=reg128#11%top
# asm 2: vmlal.s32 <h0=q0,<f13_2=d18,<g89_19=d21
vmlal.s32 q0,d18,d21

# qhasm:   h0[0,1] += f13_2[2] signed* g57_19[2];h0[2,3] += f13_2[3] signed* g57_19[3] 
# asm 1: vmlal.s32 <h0=reg128#1,<f13_2=reg128#10%top,<g57_19=reg128#15%top
# asm 2: vmlal.s32 <h0=q0,<f13_2=d19,<g57_19=d29
vmlal.s32 q0,d19,d29

# qhasm:   h2[0,1] += f13_2[0] signed* g13[0];h2[2,3] += f13_2[1] signed* g13[1] 
# asm 1: vmlal.s32 <h2=reg128#7,<f13_2=reg128#10%bot,<g13=reg128#4%bot
# asm 2: vmlal.s32 <h2=q6,<f13_2=d18,<g13=d6
vmlal.s32 q6,d18,d6

# qhasm:   ptr = &_0x1000000_stack 
# asm 1: lea >ptr=int32#3,<_0x1000000_stack=stack128#2
# asm 2: lea >ptr=r2,<_0x1000000_stack=[sp,#528]
add r2,sp,#528

# qhasm:   _0x1000000 aligned= mem128[ptr] 
# asm 1: vld1.8 {>_0x1000000=reg128#4%bot->_0x1000000=reg128#4%top},[<ptr=int32#3,: 128]
# asm 2: vld1.8 {>_0x1000000=d6->_0x1000000=d7},[<ptr=r2,: 128]
vld1.8 {d6-d7},[r2,: 128]

# qhasm:   h2[0,1] += f13_2[2] signed* g89_19[2];h2[2,3] += f13_2[3] signed* g89_19[3] 
# asm 1: vmlal.s32 <h2=reg128#7,<f13_2=reg128#10%top,<g89_19=reg128#11%top
# asm 2: vmlal.s32 <h2=q6,<f13_2=d19,<g89_19=d21
vmlal.s32 q6,d19,d21

# qhasm:   ptr = &h7_stack 
# asm 1: lea >ptr=int32#3,<h7_stack=stack128#5
# asm 2: lea >ptr=r2,<h7_stack=[sp,#576]
add r2,sp,#576

# qhasm:   h7 aligned= mem128[ptr] 
# asm 1: vld1.8 {>h7=reg128#10%bot->h7=reg128#10%top},[<ptr=int32#3,: 128]
# asm 2: vld1.8 {>h7=d18->h7=d19},[<ptr=r2,: 128]
vld1.8 {d18-d19},[r2,: 128]

# qhasm:   h0[0,1] += mix[0] signed* g13_19[0];h0[2,3] += mix[1] signed* g13_19[1] 
# asm 1: vmlal.s32 <h0=reg128#1,<mix=reg128#16%bot,<g13_19=reg128#5%bot
# asm 2: vmlal.s32 <h0=q0,<mix=d30,<g13_19=d8
vmlal.s32 q0,d30,d8

# qhasm:   ptr = &h9_stack 
# asm 1: lea >ptr=int32#3,<h9_stack=stack128#11
# asm 2: lea >ptr=r2,<h9_stack=[sp,#672]
add r2,sp,#672

# qhasm:   h9 aligned= mem128[ptr] 
# asm 1: vld1.8 {>h9=reg128#11%bot->h9=reg128#11%top},[<ptr=int32#3,: 128]
# asm 2: vld1.8 {>h9=d20->h9=d21},[<ptr=r2,: 128]
vld1.8 {d20-d21},[r2,: 128]

# qhasm:   h6[0,1] += mix[0] signed* g57_19[2];h6[2,3] += mix[1] signed* g57_19[3] 
# asm 1: vmlal.s32 <h6=reg128#6,<mix=reg128#16%bot,<g57_19=reg128#15%top
# asm 2: vmlal.s32 <h6=q5,<mix=d30,<g57_19=d29
vmlal.s32 q5,d30,d29

# qhasm:   ptr = &h5_stack 
# asm 1: lea >ptr=int32#3,<h5_stack=stack128#7
# asm 2: lea >ptr=r2,<h5_stack=[sp,#608]
add r2,sp,#608

# qhasm:   h5 aligned= mem128[ptr] 
# asm 1: vld1.8 {>h5=reg128#13%bot->h5=reg128#13%top},[<ptr=int32#3,: 128]
# asm 2: vld1.8 {>h5=d24->h5=d25},[<ptr=r2,: 128]
vld1.8 {d24-d25},[r2,: 128]

# qhasm:   h4[0,1] += mix[0] signed* g57_19[0];h4[2,3] += mix[1] signed* g57_19[1] 
# asm 1: vmlal.s32 <h4=reg128#2,<mix=reg128#16%bot,<g57_19=reg128#15%bot
# asm 2: vmlal.s32 <h4=q1,<mix=d30,<g57_19=d28
vmlal.s32 q1,d30,d28

# qhasm:   2x t0 = h0 + _0x2000000 
# asm 1: vadd.i64 >t0=reg128#14,<h0=reg128#1,<_0x2000000=reg128#12
# asm 2: vadd.i64 >t0=q13,<h0=q0,<_0x2000000=q11
vadd.i64 q13,q0,q11

# qhasm:   2x t6 = h6 + _0x2000000 
# asm 1: vadd.i64 >t6=reg128#15,<h6=reg128#6,<_0x2000000=reg128#12
# asm 2: vadd.i64 >t6=q14,<h6=q5,<_0x2000000=q11
vadd.i64 q14,q5,q11

# qhasm:   h2[0,1] += mix[0] signed* g13_19[2];h2[2,3] += mix[1] signed* g13_19[3] 
# asm 1: vmlal.s32 <h2=reg128#7,<mix=reg128#16%bot,<g13_19=reg128#5%top
# asm 2: vmlal.s32 <h2=q6,<mix=d30,<g13_19=d9
vmlal.s32 q6,d30,d9

# qhasm:   2x c0 = t0 signed>> 26 
# asm 1: vshr.s64 >c0=reg128#5,<t0=reg128#14,#26
# asm 2: vshr.s64 >c0=q4,<t0=q13,#26
vshr.s64 q4,q13,#26

# qhasm:   2x c6 = t6 signed>> 26 
# asm 1: vshr.s64 >c6=reg128#14,<t6=reg128#15,#26
# asm 2: vshr.s64 >c6=q13,<t6=q14,#26
vshr.s64 q13,q14,#26

# qhasm:   2x h1 += c0 
# asm 1: vadd.i64 >h1=reg128#8,<h1=reg128#8,<c0=reg128#5
# asm 2: vadd.i64 >h1=q7,<h1=q7,<c0=q4
vadd.i64 q7,q7,q4

# qhasm:   2x t0 = c0 << 26 
# asm 1: vshl.i64 >t0=reg128#5,<c0=reg128#5,#26
# asm 2: vshl.i64 >t0=q4,<c0=q4,#26
vshl.i64 q4,q4,#26

# qhasm:   2x t1 = h1 + _0x1000000 
# asm 1: vadd.i64 >t1=reg128#15,<h1=reg128#8,<_0x1000000=reg128#4
# asm 2: vadd.i64 >t1=q14,<h1=q7,<_0x1000000=q3
vadd.i64 q14,q7,q3

# qhasm:   2x h7 += c6 
# asm 1: vadd.i64 >h7=reg128#10,<h7=reg128#10,<c6=reg128#14
# asm 2: vadd.i64 >h7=q9,<h7=q9,<c6=q13
vadd.i64 q9,q9,q13

# qhasm:   2x t6 = c6 << 26 
# asm 1: vshl.i64 >t6=reg128#14,<c6=reg128#14,#26
# asm 2: vshl.i64 >t6=q13,<c6=q13,#26
vshl.i64 q13,q13,#26

# qhasm:   2x t7 = h7 + _0x1000000 
# asm 1: vadd.i64 >t7=reg128#16,<h7=reg128#10,<_0x1000000=reg128#4
# asm 2: vadd.i64 >t7=q15,<h7=q9,<_0x1000000=q3
vadd.i64 q15,q9,q3

# qhasm:   2x h0 -= t0 
# asm 1: vsub.i64 >h0=reg128#1,<h0=reg128#1,<t0=reg128#5
# asm 2: vsub.i64 >h0=q0,<h0=q0,<t0=q4
vsub.i64 q0,q0,q4

# qhasm:   2x c1 = t1 signed>> 25 
# asm 1: vshr.s64 >c1=reg128#5,<t1=reg128#15,#25
# asm 2: vshr.s64 >c1=q4,<t1=q14,#25
vshr.s64 q4,q14,#25

# qhasm:   2x h6 -= t6 
# asm 1: vsub.i64 >h6=reg128#6,<h6=reg128#6,<t6=reg128#14
# asm 2: vsub.i64 >h6=q5,<h6=q5,<t6=q13
vsub.i64 q5,q5,q13

# qhasm:   2x c7 = t7 signed>> 25 
# asm 1: vshr.s64 >c7=reg128#14,<t7=reg128#16,#25
# asm 2: vshr.s64 >c7=q13,<t7=q15,#25
vshr.s64 q13,q15,#25

# qhasm:   2x h2 += c1 
# asm 1: vadd.i64 >h2=reg128#7,<h2=reg128#7,<c1=reg128#5
# asm 2: vadd.i64 >h2=q6,<h2=q6,<c1=q4
vadd.i64 q6,q6,q4

# qhasm:   2x t1 = c1 << 25 
# asm 1: vshl.i64 >t1=reg128#5,<c1=reg128#5,#25
# asm 2: vshl.i64 >t1=q4,<c1=q4,#25
vshl.i64 q4,q4,#25

# qhasm:   2x t2 = h2 + _0x2000000 
# asm 1: vadd.i64 >t2=reg128#15,<h2=reg128#7,<_0x2000000=reg128#12
# asm 2: vadd.i64 >t2=q14,<h2=q6,<_0x2000000=q11
vadd.i64 q14,q6,q11

# qhasm:   2x h8 += c7 
# asm 1: vadd.i64 >h8=reg128#3,<h8=reg128#3,<c7=reg128#14
# asm 2: vadd.i64 >h8=q2,<h8=q2,<c7=q13
vadd.i64 q2,q2,q13

# qhasm:   2x h1 -= t1 
# asm 1: vsub.i64 >h1=reg128#5,<h1=reg128#8,<t1=reg128#5
# asm 2: vsub.i64 >h1=q4,<h1=q7,<t1=q4
vsub.i64 q4,q7,q4

# qhasm:   2x c2 = t2 signed>> 26 
# asm 1: vshr.s64 >c2=reg128#8,<t2=reg128#15,#26
# asm 2: vshr.s64 >c2=q7,<t2=q14,#26
vshr.s64 q7,q14,#26

# qhasm:   2x t7 = c7 << 25 
# asm 1: vshl.i64 >t7=reg128#14,<c7=reg128#14,#25
# asm 2: vshl.i64 >t7=q13,<c7=q13,#25
vshl.i64 q13,q13,#25

# qhasm:   2x t8 = h8 + _0x2000000 
# asm 1: vadd.i64 >t8=reg128#15,<h8=reg128#3,<_0x2000000=reg128#12
# asm 2: vadd.i64 >t8=q14,<h8=q2,<_0x2000000=q11
vadd.i64 q14,q2,q11

# qhasm:   2x h3 += c2 
# asm 1: vadd.i64 >h3=reg128#9,<h3=reg128#9,<c2=reg128#8
# asm 2: vadd.i64 >h3=q8,<h3=q8,<c2=q7
vadd.i64 q8,q8,q7

# qhasm:   2x t2 = c2 << 26 
# asm 1: vshl.i64 >t2=reg128#8,<c2=reg128#8,#26
# asm 2: vshl.i64 >t2=q7,<c2=q7,#26
vshl.i64 q7,q7,#26

# qhasm:   2x t3 = h3 + _0x1000000 
# asm 1: vadd.i64 >t3=reg128#16,<h3=reg128#9,<_0x1000000=reg128#4
# asm 2: vadd.i64 >t3=q15,<h3=q8,<_0x1000000=q3
vadd.i64 q15,q8,q3

# qhasm:   2x h7 -= t7 
# asm 1: vsub.i64 >h7=reg128#10,<h7=reg128#10,<t7=reg128#14
# asm 2: vsub.i64 >h7=q9,<h7=q9,<t7=q13
vsub.i64 q9,q9,q13

# qhasm:   2x c8 = t8 signed>> 26 
# asm 1: vshr.s64 >c8=reg128#14,<t8=reg128#15,#26
# asm 2: vshr.s64 >c8=q13,<t8=q14,#26
vshr.s64 q13,q14,#26

# qhasm:   2x h2 -= t2 
# asm 1: vsub.i64 >h2=reg128#7,<h2=reg128#7,<t2=reg128#8
# asm 2: vsub.i64 >h2=q6,<h2=q6,<t2=q7
vsub.i64 q6,q6,q7

# qhasm:   2x c3 = t3 signed>> 25 
# asm 1: vshr.s64 >c3=reg128#8,<t3=reg128#16,#25
# asm 2: vshr.s64 >c3=q7,<t3=q15,#25
vshr.s64 q7,q15,#25

# qhasm:   2x h9 += c8 
# asm 1: vadd.i64 >h9=reg128#11,<h9=reg128#11,<c8=reg128#14
# asm 2: vadd.i64 >h9=q10,<h9=q10,<c8=q13
vadd.i64 q10,q10,q13

# qhasm:   2x t8 = c8 << 26 
# asm 1: vshl.i64 >t8=reg128#14,<c8=reg128#14,#26
# asm 2: vshl.i64 >t8=q13,<c8=q13,#26
vshl.i64 q13,q13,#26

# qhasm:   2x t9 = h9 + _0x1000000 
# asm 1: vadd.i64 >t9=reg128#15,<h9=reg128#11,<_0x1000000=reg128#4
# asm 2: vadd.i64 >t9=q14,<h9=q10,<_0x1000000=q3
vadd.i64 q14,q10,q3

# qhasm:   2x h4 += c3 
# asm 1: vadd.i64 >h4=reg128#2,<h4=reg128#2,<c3=reg128#8
# asm 2: vadd.i64 >h4=q1,<h4=q1,<c3=q7
vadd.i64 q1,q1,q7

# qhasm:   posh = playground1_ptr + 144 
# asm 1: add >posh=int32#3,<playground1_ptr=int32#4,#144
# asm 2: add >posh=r2,<playground1_ptr=r3,#144
add r2,r3,#144

# qhasm:   2x t3 = c3 << 25 
# asm 1: vshl.i64 >t3=reg128#8,<c3=reg128#8,#25
# asm 2: vshl.i64 >t3=q7,<c3=q7,#25
vshl.i64 q7,q7,#25

# qhasm:   posH = playground1_ptr + 96 
# asm 1: add >posH=int32#5,<playground1_ptr=int32#4,#96
# asm 2: add >posH=r4,<playground1_ptr=r3,#96
add r4,r3,#96

# qhasm:   2x t4 = h4 + _0x2000000 
# asm 1: vadd.i64 >t4=reg128#16,<h4=reg128#2,<_0x2000000=reg128#12
# asm 2: vadd.i64 >t4=q15,<h4=q1,<_0x2000000=q11
vadd.i64 q15,q1,q11

# qhasm:   posh+=8 
# asm 1: add >posh=int32#3,<posh=int32#3,#8
# asm 2: add >posh=r2,<posh=r2,#8
add r2,r2,#8

# qhasm:   2x h8 -= t8 
# asm 1: vsub.i64 >h8=reg128#3,<h8=reg128#3,<t8=reg128#14
# asm 2: vsub.i64 >h8=q2,<h8=q2,<t8=q13
vsub.i64 q2,q2,q13

# qhasm:   posH+=8 
# asm 1: add >posH=int32#5,<posH=int32#5,#8
# asm 2: add >posH=r4,<posH=r4,#8
add r4,r4,#8

# qhasm:   2x c9 = t9 signed>> 25 
# asm 1: vshr.s64 >c9=reg128#14,<t9=reg128#15,#25
# asm 2: vshr.s64 >c9=q13,<t9=q14,#25
vshr.s64 q13,q14,#25

# qhasm:   2x h3 -= t3 
# asm 1: vsub.i64 >h3=reg128#8,<h3=reg128#9,<t3=reg128#8
# asm 2: vsub.i64 >h3=q7,<h3=q8,<t3=q7
vsub.i64 q7,q8,q7

# qhasm:   2x c4 = t4 signed>> 26 
# asm 1: vshr.s64 >c4=reg128#9,<t4=reg128#16,#26
# asm 2: vshr.s64 >c4=q8,<t4=q15,#26
vshr.s64 q8,q15,#26

# qhasm:   2x s = c9 + c9 
# asm 1: vadd.i64 >s=reg128#15,<c9=reg128#14,<c9=reg128#14
# asm 2: vadd.i64 >s=q14,<c9=q13,<c9=q13
vadd.i64 q14,q13,q13

# qhasm:   2x h5 += c4 
# asm 1: vadd.i64 >h5=reg128#13,<h5=reg128#13,<c4=reg128#9
# asm 2: vadd.i64 >h5=q12,<h5=q12,<c4=q8
vadd.i64 q12,q12,q8

# qhasm:   h2 h3 = h2[0]h3[0]h2[2]h2[3] h2[1]h3[1]h3[2]h3[3] 
# asm 1: vtrn.32 <h2=reg128#7%bot,<h3=reg128#8%bot
# asm 2: vtrn.32 <h2=d12,<h3=d14
vtrn.32 d12,d14

# qhasm:   2x t4 = c4 << 26 
# asm 1: vshl.i64 >t4=reg128#9,<c4=reg128#9,#26
# asm 2: vshl.i64 >t4=q8,<c4=q8,#26
vshl.i64 q8,q8,#26

# qhasm:   h2 h3 = h2[0]h2[1]h2[2]h3[2] h3[0]h3[1]h2[3]h3[3] 
# asm 1: vtrn.32 <h2=reg128#7%top,<h3=reg128#8%top
# asm 2: vtrn.32 <h2=d13,<h3=d15
vtrn.32 d13,d15

# qhasm:   2x t5 = h5 + _0x1000000 
# asm 1: vadd.i64 >t5=reg128#4,<h5=reg128#13,<_0x1000000=reg128#4
# asm 2: vadd.i64 >t5=q3,<h5=q12,<_0x1000000=q3
vadd.i64 q3,q12,q3

# qhasm:   2x h0 += s 
# asm 1: vadd.i64 >h0=reg128#1,<h0=reg128#1,<s=reg128#15
# asm 2: vadd.i64 >h0=q0,<h0=q0,<s=q14
vadd.i64 q0,q0,q14

# qhasm:   mem64[posh] aligned= h2[0];posh+=8 
# asm 1: vst1.8 <h2=reg128#7%bot,[<posh=int32#3,: 64]!
# asm 2: vst1.8 <h2=d12,[<posh=r2,: 64]!
vst1.8 d12,[r2,: 64]!

# qhasm:   2x s = c9 << 4 
# asm 1: vshl.i64 >s=reg128#8,<c9=reg128#14,#4
# asm 2: vshl.i64 >s=q7,<c9=q13,#4
vshl.i64 q7,q13,#4

# qhasm:   mem64[posH] aligned= h2[1];posH+=8 
# asm 1: vst1.8 <h2=reg128#7%top,[<posH=int32#5,: 64]!
# asm 2: vst1.8 <h2=d13,[<posH=r4,: 64]!
vst1.8 d13,[r4,: 64]!

# qhasm:   2x h4 -= t4 
# asm 1: vsub.i64 >h4=reg128#2,<h4=reg128#2,<t4=reg128#9
# asm 2: vsub.i64 >h4=q1,<h4=q1,<t4=q8
vsub.i64 q1,q1,q8

# qhasm:   2x c5 = t5 signed>> 25 
# asm 1: vshr.s64 >c5=reg128#4,<t5=reg128#4,#25
# asm 2: vshr.s64 >c5=q3,<t5=q3,#25
vshr.s64 q3,q3,#25

# qhasm:   2x h0 += s 
# asm 1: vadd.i64 >h0=reg128#1,<h0=reg128#1,<s=reg128#8
# asm 2: vadd.i64 >h0=q0,<h0=q0,<s=q7
vadd.i64 q0,q0,q7

# qhasm:   2x h6 += c5 
# asm 1: vadd.i64 >h6=reg128#6,<h6=reg128#6,<c5=reg128#4
# asm 2: vadd.i64 >h6=q5,<h6=q5,<c5=q3
vadd.i64 q5,q5,q3

# qhasm:   2x t5 = c5 << 25 
# asm 1: vshl.i64 >t5=reg128#4,<c5=reg128#4,#25
# asm 2: vshl.i64 >t5=q3,<c5=q3,#25
vshl.i64 q3,q3,#25

# qhasm:   2x t6 = h6 + _0x2000000 
# asm 1: vadd.i64 >t6=reg128#7,<h6=reg128#6,<_0x2000000=reg128#12
# asm 2: vadd.i64 >t6=q6,<h6=q5,<_0x2000000=q11
vadd.i64 q6,q5,q11

# qhasm:   2x h0 += c9 
# asm 1: vadd.i64 >h0=reg128#1,<h0=reg128#1,<c9=reg128#14
# asm 2: vadd.i64 >h0=q0,<h0=q0,<c9=q13
vadd.i64 q0,q0,q13

# qhasm:   2x t9 = c9 << 25 
# asm 1: vshl.i64 >t9=reg128#8,<c9=reg128#14,#25
# asm 2: vshl.i64 >t9=q7,<c9=q13,#25
vshl.i64 q7,q13,#25

# qhasm:   2x t0 = h0 + _0x2000000 
# asm 1: vadd.i64 >t0=reg128#9,<h0=reg128#1,<_0x2000000=reg128#12
# asm 2: vadd.i64 >t0=q8,<h0=q0,<_0x2000000=q11
vadd.i64 q8,q0,q11

# qhasm:   2x h5 -= t5 
# asm 1: vsub.i64 >h5=reg128#4,<h5=reg128#13,<t5=reg128#4
# asm 2: vsub.i64 >h5=q3,<h5=q12,<t5=q3
vsub.i64 q3,q12,q3

# qhasm:   2x c6 = t6 signed>> 26 
# asm 1: vshr.s64 >c6=reg128#7,<t6=reg128#7,#26
# asm 2: vshr.s64 >c6=q6,<t6=q6,#26
vshr.s64 q6,q6,#26

# qhasm:   2x h9 -= t9 
# asm 1: vsub.i64 >h9=reg128#8,<h9=reg128#11,<t9=reg128#8
# asm 2: vsub.i64 >h9=q7,<h9=q10,<t9=q7
vsub.i64 q7,q10,q7

# qhasm:   h4 h5 = h4[0]h5[0]h4[2]h4[3] h4[1]h5[1]h5[2]h5[3] 
# asm 1: vtrn.32 <h4=reg128#2%bot,<h5=reg128#4%bot
# asm 2: vtrn.32 <h4=d2,<h5=d6
vtrn.32 d2,d6

# qhasm:   2x c0 = t0 signed>> 26 
# asm 1: vshr.s64 >c0=reg128#9,<t0=reg128#9,#26
# asm 2: vshr.s64 >c0=q8,<t0=q8,#26
vshr.s64 q8,q8,#26

# qhasm:   h4 h5 = h4[0]h4[1]h4[2]h5[2] h5[0]h5[1]h4[3]h5[3] 
# asm 1: vtrn.32 <h4=reg128#2%top,<h5=reg128#4%top
# asm 2: vtrn.32 <h4=d3,<h5=d7
vtrn.32 d3,d7

# qhasm:   2x h7 += c6 
# asm 1: vadd.i64 >h7=reg128#4,<h7=reg128#10,<c6=reg128#7
# asm 2: vadd.i64 >h7=q3,<h7=q9,<c6=q6
vadd.i64 q3,q9,q6

# qhasm:   mem64[posh] aligned= h4[0] 
# asm 1: vst1.8 <h4=reg128#2%bot,[<posh=int32#3,: 64]
# asm 2: vst1.8 <h4=d2,[<posh=r2,: 64]
vst1.8 d2,[r2,: 64]

# qhasm:   2x t6 = c6 << 26 
# asm 1: vshl.i64 >t6=reg128#7,<c6=reg128#7,#26
# asm 2: vshl.i64 >t6=q6,<c6=q6,#26
vshl.i64 q6,q6,#26

# qhasm:   mem64[posH] aligned= h4[1] 
# asm 1: vst1.8 <h4=reg128#2%top,[<posH=int32#5,: 64]
# asm 2: vst1.8 <h4=d3,[<posH=r4,: 64]
vst1.8 d3,[r4,: 64]

# qhasm:   2x h1 += c0 
# asm 1: vadd.i64 >h1=reg128#2,<h1=reg128#5,<c0=reg128#9
# asm 2: vadd.i64 >h1=q1,<h1=q4,<c0=q8
vadd.i64 q1,q4,q8

# qhasm:   h8 h9 = h8[0]h9[0]h8[2]h8[3] h8[1]h9[1]h9[2]h9[3] 
# asm 1: vtrn.32 <h8=reg128#3%bot,<h9=reg128#8%bot
# asm 2: vtrn.32 <h8=d4,<h9=d14
vtrn.32 d4,d14

# qhasm:   2x t0 = c0 << 26 
# asm 1: vshl.i64 >t0=reg128#5,<c0=reg128#9,#26
# asm 2: vshl.i64 >t0=q4,<c0=q8,#26
vshl.i64 q4,q8,#26

# qhasm:   h8 h9 = h8[0]h8[1]h8[2]h9[2] h9[0]h9[1]h8[3]h9[3] 
# asm 1: vtrn.32 <h8=reg128#3%top,<h9=reg128#8%top
# asm 2: vtrn.32 <h8=d5,<h9=d15
vtrn.32 d5,d15

# qhasm:   2x h6 -= t6 
# asm 1: vsub.i64 >h6=reg128#6,<h6=reg128#6,<t6=reg128#7
# asm 2: vsub.i64 >h6=q5,<h6=q5,<t6=q6
vsub.i64 q5,q5,q6

# qhasm:   posh+=16 
# asm 1: add >posh=int32#3,<posh=int32#3,#16
# asm 2: add >posh=r2,<posh=r2,#16
add r2,r2,#16

# qhasm:   2x h0 -= t0 
# asm 1: vsub.i64 >h0=reg128#1,<h0=reg128#1,<t0=reg128#5
# asm 2: vsub.i64 >h0=q0,<h0=q0,<t0=q4
vsub.i64 q0,q0,q4

# qhasm:   mem64[posh] aligned= h8[0] 
# asm 1: vst1.8 <h8=reg128#3%bot,[<posh=int32#3,: 64]
# asm 2: vst1.8 <h8=d4,[<posh=r2,: 64]
vst1.8 d4,[r2,: 64]

# qhasm:   posH+=16 
# asm 1: add >posH=int32#5,<posH=int32#5,#16
# asm 2: add >posH=r4,<posH=r4,#16
add r4,r4,#16

# qhasm:   mem64[posH] aligned= h8[1] 
# asm 1: vst1.8 <h8=reg128#3%top,[<posH=int32#5,: 64]
# asm 2: vst1.8 <h8=d5,[<posH=r4,: 64]
vst1.8 d5,[r4,: 64]

# qhasm:   h6 h7 = h6[0]h7[0]h6[2]h6[3] h6[1]h7[1]h7[2]h7[3] 
# asm 1: vtrn.32 <h6=reg128#6%bot,<h7=reg128#4%bot
# asm 2: vtrn.32 <h6=d10,<h7=d6
vtrn.32 d10,d6

# qhasm:   h6 h7 = h6[0]h6[1]h6[2]h7[2] h7[0]h7[1]h6[3]h7[3] 
# asm 1: vtrn.32 <h6=reg128#6%top,<h7=reg128#4%top
# asm 2: vtrn.32 <h6=d11,<h7=d7
vtrn.32 d11,d7

# qhasm:   posh-=8 
# asm 1: sub >posh=int32#3,<posh=int32#3,#8
# asm 2: sub >posh=r2,<posh=r2,#8
sub r2,r2,#8

# qhasm:   posH-=8 
# asm 1: sub >posH=int32#5,<posH=int32#5,#8
# asm 2: sub >posH=r4,<posH=r4,#8
sub r4,r4,#8

# qhasm:   h0 h1 = h0[0]h1[0]h0[2]h0[3] h0[1]h1[1]h1[2]h1[3] 
# asm 1: vtrn.32 <h0=reg128#1%bot,<h1=reg128#2%bot
# asm 2: vtrn.32 <h0=d0,<h1=d2
vtrn.32 d0,d2

# qhasm:   h0 h1 = h0[0]h0[1]h0[2]h1[2] h1[0]h1[1]h0[3]h1[3] 
# asm 1: vtrn.32 <h0=reg128#1%top,<h1=reg128#2%top
# asm 2: vtrn.32 <h0=d1,<h1=d3
vtrn.32 d1,d3

# qhasm:   mem64[posh] aligned= h6[0] 
# asm 1: vst1.8 <h6=reg128#6%bot,[<posh=int32#3,: 64]
# asm 2: vst1.8 <h6=d10,[<posh=r2,: 64]
vst1.8 d10,[r2,: 64]

# qhasm:   mem64[posH] aligned= h6[1] 
# asm 1: vst1.8 <h6=reg128#6%top,[<posH=int32#5,: 64]
# asm 2: vst1.8 <h6=d11,[<posH=r4,: 64]
vst1.8 d11,[r4,: 64]

# qhasm:   posh-=24 
# asm 1: sub >posh=int32#3,<posh=int32#3,#24
# asm 2: sub >posh=r2,<posh=r2,#24
sub r2,r2,#24

# qhasm:   posH-=24 
# asm 1: sub >posH=int32#5,<posH=int32#5,#24
# asm 2: sub >posH=r4,<posH=r4,#24
sub r4,r4,#24

# qhasm:   mem64[posh] aligned= h0[0] 
# asm 1: vst1.8 <h0=reg128#1%bot,[<posh=int32#3,: 64]
# asm 2: vst1.8 <h0=d0,[<posh=r2,: 64]
vst1.8 d0,[r2,: 64]

# qhasm:   mem64[posH] aligned= h0[1] 
# asm 1: vst1.8 <h0=reg128#1%top,[<posH=int32#5,: 64]
# asm 2: vst1.8 <h0=d1,[<posH=r4,: 64]
vst1.8 d1,[r4,: 64]

# qhasm:   pos1 = playground1_ptr + 288 
# asm 1: add >pos1=int32#3,<playground1_ptr=int32#4,#288
# asm 2: add >pos1=r2,<playground1_ptr=r3,#288
add r2,r3,#288

# qhasm:   pos2 = playground1_ptr + 336 
# asm 1: add >pos2=int32#5,<playground1_ptr=int32#4,#336
# asm 2: add >pos2=r4,<playground1_ptr=r3,#336
add r4,r3,#336

# qhasm:   f0 aligned= mem128[pos1];pos1 += 16 
# asm 1: vld1.8 {>f0=reg128#1%bot->f0=reg128#1%top},[<pos1=int32#3,: 128]!
# asm 2: vld1.8 {>f0=d0->f0=d1},[<pos1=r2,: 128]!
vld1.8 {d0-d1},[r2,: 128]!

# qhasm:   g0 aligned= mem128[pos2];pos2 += 16 
# asm 1: vld1.8 {>g0=reg128#2%bot->g0=reg128#2%top},[<pos2=int32#5,: 128]!
# asm 2: vld1.8 {>g0=d2->g0=d3},[<pos2=r4,: 128]!
vld1.8 {d2-d3},[r4,: 128]!

# qhasm:   4x f0 -= g0 
# asm 1: vsub.i32 >f0=reg128#1,<f0=reg128#1,<g0=reg128#2
# asm 2: vsub.i32 >f0=q0,<f0=q0,<g0=q1
vsub.i32 q0,q0,q1

# qhasm:   f4 aligned= mem128[pos1];pos1 += 16 
# asm 1: vld1.8 {>f4=reg128#2%bot->f4=reg128#2%top},[<pos1=int32#3,: 128]!
# asm 2: vld1.8 {>f4=d2->f4=d3},[<pos1=r2,: 128]!
vld1.8 {d2-d3},[r2,: 128]!

# qhasm:   g4 aligned= mem128[pos2];pos2 += 16 
# asm 1: vld1.8 {>g4=reg128#3%bot->g4=reg128#3%top},[<pos2=int32#5,: 128]!
# asm 2: vld1.8 {>g4=d4->g4=d5},[<pos2=r4,: 128]!
vld1.8 {d4-d5},[r4,: 128]!

# qhasm:   4x f4 -= g4 
# asm 1: vsub.i32 >f4=reg128#2,<f4=reg128#2,<g4=reg128#3
# asm 2: vsub.i32 >f4=q1,<f4=q1,<g4=q2
vsub.i32 q1,q1,q2

# qhasm:   pos0 = playground1_ptr + 240 
# asm 1: add >pos0=int32#6,<playground1_ptr=int32#4,#240
# asm 2: add >pos0=r5,<playground1_ptr=r3,#240
add r5,r3,#240

# qhasm:   new f8 

# qhasm:   new g8 

# qhasm:   f8 aligned= mem64[pos1] f8[1] 
# asm 1: vld1.8 {<f8=reg128#3%bot},[<pos1=int32#3,: 64]
# asm 2: vld1.8 {<f8=d4},[<pos1=r2,: 64]
vld1.8 {d4},[r2,: 64]

# qhasm:   g8 aligned= mem64[pos2] g8[1] 
# asm 1: vld1.8 {<g8=reg128#4%bot},[<pos2=int32#5,: 64]
# asm 2: vld1.8 {<g8=d6},[<pos2=r4,: 64]
vld1.8 {d6},[r4,: 64]

# qhasm:   4x f8 -= g8 
# asm 1: vsub.i32 >f8=reg128#3,<f8=reg128#3,<g8=reg128#4
# asm 2: vsub.i32 >f8=q2,<f8=q2,<g8=q3
vsub.i32 q2,q2,q3

# qhasm:   mem128[pos0] aligned= f0;pos0 += 16 
# asm 1: vst1.8 {<f0=reg128#1%bot-<f0=reg128#1%top},[<pos0=int32#6,: 128]!
# asm 2: vst1.8 {<f0=d0-<f0=d1},[<pos0=r5,: 128]!
vst1.8 {d0-d1},[r5,: 128]!

# qhasm:   mem128[pos0] aligned= f4;pos0 += 16 
# asm 1: vst1.8 {<f4=reg128#2%bot-<f4=reg128#2%top},[<pos0=int32#6,: 128]!
# asm 2: vst1.8 {<f4=d2-<f4=d3},[<pos0=r5,: 128]!
vst1.8 {d2-d3},[r5,: 128]!

# qhasm:   mem64[pos0] aligned= f8[0] 
# asm 1: vst1.8 <f8=reg128#3%bot,[<pos0=int32#6,: 64]
# asm 2: vst1.8 <f8=d4,[<pos0=r5,: 64]
vst1.8 d4,[r5,: 64]

# qhasm:   pos1 = playground1_ptr + 144 
# asm 1: add >pos1=int32#3,<playground1_ptr=int32#4,#144
# asm 2: add >pos1=r2,<playground1_ptr=r3,#144
add r2,r3,#144

# qhasm:   pos2 = playground1_ptr + 96 
# asm 1: add >pos2=int32#5,<playground1_ptr=int32#4,#96
# asm 2: add >pos2=r4,<playground1_ptr=r3,#96
add r4,r3,#96

# qhasm:   pos3 = playground1_ptr + 144 
# asm 1: add >pos3=int32#6,<playground1_ptr=int32#4,#144
# asm 2: add >pos3=r5,<playground1_ptr=r3,#144
add r5,r3,#144

# qhasm:   pos0 = playground1_ptr + 192 
# asm 1: add >pos0=int32#7,<playground1_ptr=int32#4,#192
# asm 2: add >pos0=r6,<playground1_ptr=r3,#192
add r6,r3,#192

# qhasm:   f0 aligned= mem128[pos1];pos1 += 16 
# asm 1: vld1.8 {>f0=reg128#1%bot->f0=reg128#1%top},[<pos1=int32#3,: 128]!
# asm 2: vld1.8 {>f0=d0->f0=d1},[<pos1=r2,: 128]!
vld1.8 {d0-d1},[r2,: 128]!

# qhasm:   g0 aligned= mem128[pos2];pos2 += 16 
# asm 1: vld1.8 {>g0=reg128#2%bot->g0=reg128#2%top},[<pos2=int32#5,: 128]!
# asm 2: vld1.8 {>g0=d2->g0=d3},[<pos2=r4,: 128]!
vld1.8 {d2-d3},[r4,: 128]!

# qhasm:   4x d0 = f0 - g0 
# asm 1: vsub.i32 >d0=reg128#3,<f0=reg128#1,<g0=reg128#2
# asm 2: vsub.i32 >d0=q2,<f0=q0,<g0=q1
vsub.i32 q2,q0,q1

# qhasm:   4x f0 += g0 
# asm 1: vadd.i32 >f0=reg128#1,<f0=reg128#1,<g0=reg128#2
# asm 2: vadd.i32 >f0=q0,<f0=q0,<g0=q1
vadd.i32 q0,q0,q1

# qhasm:   f4 aligned= mem128[pos1];pos1 += 16 
# asm 1: vld1.8 {>f4=reg128#2%bot->f4=reg128#2%top},[<pos1=int32#3,: 128]!
# asm 2: vld1.8 {>f4=d2->f4=d3},[<pos1=r2,: 128]!
vld1.8 {d2-d3},[r2,: 128]!

# qhasm:   g4 aligned= mem128[pos2];pos2 += 16 
# asm 1: vld1.8 {>g4=reg128#4%bot->g4=reg128#4%top},[<pos2=int32#5,: 128]!
# asm 2: vld1.8 {>g4=d6->g4=d7},[<pos2=r4,: 128]!
vld1.8 {d6-d7},[r4,: 128]!

# qhasm:   4x d4 = f4 - g4 
# asm 1: vsub.i32 >d4=reg128#5,<f4=reg128#2,<g4=reg128#4
# asm 2: vsub.i32 >d4=q4,<f4=q1,<g4=q3
vsub.i32 q4,q1,q3

# qhasm:   4x f4 += g4 
# asm 1: vadd.i32 >f4=reg128#2,<f4=reg128#2,<g4=reg128#4
# asm 2: vadd.i32 >f4=q1,<f4=q1,<g4=q3
vadd.i32 q1,q1,q3

# qhasm:   new f8 

# qhasm:   f8 aligned= mem64[pos1] f8[1] 
# asm 1: vld1.8 {<f8=reg128#4%bot},[<pos1=int32#3,: 64]
# asm 2: vld1.8 {<f8=d6},[<pos1=r2,: 64]
vld1.8 {d6},[r2,: 64]

# qhasm:   new g8 

# qhasm:   g8 aligned= mem64[pos2] g8[1] 
# asm 1: vld1.8 {<g8=reg128#6%bot},[<pos2=int32#5,: 64]
# asm 2: vld1.8 {<g8=d10},[<pos2=r4,: 64]
vld1.8 {d10},[r4,: 64]

# qhasm:   4x d8 = f8 - g8 
# asm 1: vsub.i32 >d8=reg128#7,<f8=reg128#4,<g8=reg128#6
# asm 2: vsub.i32 >d8=q6,<f8=q3,<g8=q5
vsub.i32 q6,q3,q5

# qhasm:   4x f8 += g8 
# asm 1: vadd.i32 >f8=reg128#4,<f8=reg128#4,<g8=reg128#6
# asm 2: vadd.i32 >f8=q3,<f8=q3,<g8=q5
vadd.i32 q3,q3,q5

# qhasm:   mem128[pos3] aligned= d0;pos3 += 16 
# asm 1: vst1.8 {<d0=reg128#3%bot-<d0=reg128#3%top},[<pos3=int32#6,: 128]!
# asm 2: vst1.8 {<d0=d4-<d0=d5},[<pos3=r5,: 128]!
vst1.8 {d4-d5},[r5,: 128]!

# qhasm:   mem128[pos0] aligned= f0;pos0 += 16 
# asm 1: vst1.8 {<f0=reg128#1%bot-<f0=reg128#1%top},[<pos0=int32#7,: 128]!
# asm 2: vst1.8 {<f0=d0-<f0=d1},[<pos0=r6,: 128]!
vst1.8 {d0-d1},[r6,: 128]!

# qhasm:   mem128[pos3] aligned= d4;pos3 += 16 
# asm 1: vst1.8 {<d4=reg128#5%bot-<d4=reg128#5%top},[<pos3=int32#6,: 128]!
# asm 2: vst1.8 {<d4=d8-<d4=d9},[<pos3=r5,: 128]!
vst1.8 {d8-d9},[r5,: 128]!

# qhasm:   mem128[pos0] aligned= f4;pos0 += 16 
# asm 1: vst1.8 {<f4=reg128#2%bot-<f4=reg128#2%top},[<pos0=int32#7,: 128]!
# asm 2: vst1.8 {<f4=d2-<f4=d3},[<pos0=r6,: 128]!
vst1.8 {d2-d3},[r6,: 128]!

# qhasm:   mem64[pos3] aligned= d8[0] 
# asm 1: vst1.8 <d8=reg128#7%bot,[<pos3=int32#6,: 64]
# asm 2: vst1.8 <d8=d12,[<pos3=r5,: 64]
vst1.8 d12,[r5,: 64]

# qhasm:   mem64[pos0] aligned= f8[0] 
# asm 1: vst1.8 <f8=reg128#4%bot,[<pos0=int32#7,: 64]
# asm 2: vst1.8 <f8=d6,[<pos0=r6,: 64]
vst1.8 d6,[r6,: 64]

# qhasm:   posf = playground1_ptr + 0 
# asm 1: add >posf=int32#3,<playground1_ptr=int32#4,#0
# asm 2: add >posf=r2,<playground1_ptr=r3,#0
add r2,r3,#0

# qhasm:   posg = playground1_ptr + 240 
# asm 1: add >posg=int32#5,<playground1_ptr=int32#4,#240
# asm 2: add >posg=r4,<playground1_ptr=r3,#240
add r4,r3,#240

# qhasm:   g02 aligned= mem128[posg];posg += 16 
# asm 1: vld1.8 {>g02=reg128#1%bot->g02=reg128#1%top},[<posg=int32#5,: 128]!
# asm 2: vld1.8 {>g02=d0->g02=d1},[<posg=r4,: 128]!
vld1.8 {d0-d1},[r4,: 128]!

# qhasm:   g46 aligned= mem128[posg];posg += 16 
# asm 1: vld1.8 {>g46=reg128#2%bot->g46=reg128#2%top},[<posg=int32#5,: 128]!
# asm 2: vld1.8 {>g46=d2->g46=d3},[<posg=r4,: 128]!
vld1.8 {d2-d3},[r4,: 128]!

# qhasm:   new g89 

# qhasm:   g89 aligned= mem64[posg] g89[1] 
# asm 1: vld1.8 {<g89=reg128#3%bot},[<posg=int32#5,: 64]
# asm 2: vld1.8 {<g89=d4},[<posg=r4,: 64]
vld1.8 {d4},[r4,: 64]

# qhasm:   posG = playground1_ptr + 336 
# asm 1: add >posG=int32#5,<playground1_ptr=int32#4,#336
# asm 2: add >posG=r4,<playground1_ptr=r3,#336
add r4,r3,#336

# qhasm:   g13 aligned= mem128[posG];posG += 16 
# asm 1: vld1.8 {>g13=reg128#4%bot->g13=reg128#4%top},[<posG=int32#5,: 128]!
# asm 2: vld1.8 {>g13=d6->g13=d7},[<posG=r4,: 128]!
vld1.8 {d6-d7},[r4,: 128]!

# qhasm:   g02 g13 = g02[0]g13[0] g02[2]g13[2] g02[1]g13[1] g02[3]g13[3] 
# asm 1: vtrn.32 <g02=reg128#1,<g13=reg128#4
# asm 2: vtrn.32 <g02=q0,<g13=q3
vtrn.32 q0,q3

# qhasm:   g57 aligned= mem128[posG];posG += 16 
# asm 1: vld1.8 {>g57=reg128#5%bot->g57=reg128#5%top},[<posG=int32#5,: 128]!
# asm 2: vld1.8 {>g57=d8->g57=d9},[<posG=r4,: 128]!
vld1.8 {d8-d9},[r4,: 128]!

# qhasm:   4x mix = g02 << 4 
# asm 1: vshl.i32 >mix=reg128#6,<g02=reg128#1,#4
# asm 2: vshl.i32 >mix=q5,<g02=q0,#4
vshl.i32 q5,q0,#4

# qhasm:   g46 g57 = g46[0]g57[0] g46[2]g57[2] g46[1]g57[1] g46[3]g57[3] 
# asm 1: vtrn.32 <g46=reg128#2,<g57=reg128#5
# asm 2: vtrn.32 <g46=q1,<g57=q4
vtrn.32 q1,q4

# qhasm:   4x g13_19 = g13 << 4 
# asm 1: vshl.i32 >g13_19=reg128#7,<g13=reg128#4,#4
# asm 2: vshl.i32 >g13_19=q6,<g13=q3,#4
vshl.i32 q6,q3,#4

# qhasm:   4x mix += g02 
# asm 1: vadd.i32 >mix=reg128#6,<mix=reg128#6,<g02=reg128#1
# asm 2: vadd.i32 >mix=q5,<mix=q5,<g02=q0
vadd.i32 q5,q5,q0

# qhasm:   4x g13_19 += g13 
# asm 1: vadd.i32 >g13_19=reg128#7,<g13_19=reg128#7,<g13=reg128#4
# asm 2: vadd.i32 >g13_19=q6,<g13_19=q6,<g13=q3
vadd.i32 q6,q6,q3

# qhasm:   4x g46_19 = g46 << 4 
# asm 1: vshl.i32 >g46_19=reg128#8,<g46=reg128#2,#4
# asm 2: vshl.i32 >g46_19=q7,<g46=q1,#4
vshl.i32 q7,q1,#4

# qhasm:   g89 aligned= g89[0] mem64[posG] 
# asm 1: vld1.8 {<g89=reg128#3%top},[<posG=int32#5,: 64]
# asm 2: vld1.8 {<g89=d5},[<posG=r4,: 64]
vld1.8 {d5},[r4,: 64]

# qhasm:   4x g57_19 = g57 << 4 
# asm 1: vshl.i32 >g57_19=reg128#9,<g57=reg128#5,#4
# asm 2: vshl.i32 >g57_19=q8,<g57=q4,#4
vshl.i32 q8,q4,#4

# qhasm:   g89 = g89[0] g89[2] g89[1] g89[3] 
# asm 1: vtrn.32 <g89=reg128#3%bot,<g89=reg128#3%top
# asm 2: vtrn.32 <g89=d4,<g89=d5
vtrn.32 d4,d5

# qhasm:   4x g46_19 += g46 
# asm 1: vadd.i32 >g46_19=reg128#8,<g46_19=reg128#8,<g46=reg128#2
# asm 2: vadd.i32 >g46_19=q7,<g46_19=q7,<g46=q1
vadd.i32 q7,q7,q1

# qhasm:   4x g57_19 += g57 
# asm 1: vadd.i32 >g57_19=reg128#9,<g57_19=reg128#9,<g57=reg128#5
# asm 2: vadd.i32 >g57_19=q8,<g57_19=q8,<g57=q4
vadd.i32 q8,q8,q4

# qhasm:   f02 aligned= mem128[posf];posf += 16 
# asm 1: vld1.8 {>f02=reg128#10%bot->f02=reg128#10%top},[<posf=int32#3,: 128]!
# asm 2: vld1.8 {>f02=d18->f02=d19},[<posf=r2,: 128]!
vld1.8 {d18-d19},[r2,: 128]!

# qhasm:   4x g89_19 = g89 << 4 
# asm 1: vshl.i32 >g89_19=reg128#11,<g89=reg128#3,#4
# asm 2: vshl.i32 >g89_19=q10,<g89=q2,#4
vshl.i32 q10,q2,#4

# qhasm:   f46 aligned= mem128[posf];posf += 16 
# asm 1: vld1.8 {>f46=reg128#12%bot->f46=reg128#12%top},[<posf=int32#3,: 128]!
# asm 2: vld1.8 {>f46=d22->f46=d23},[<posf=r2,: 128]!
vld1.8 {d22-d23},[r2,: 128]!

# qhasm:   4x g89_19 += g89 
# asm 1: vadd.i32 >g89_19=reg128#11,<g89_19=reg128#11,<g89=reg128#3
# asm 2: vadd.i32 >g89_19=q10,<g89_19=q10,<g89=q2
vadd.i32 q10,q10,q2

# qhasm:   new f89 

# qhasm:   f89 aligned= mem64[posf] f89[1] 
# asm 1: vld1.8 {<f89=reg128#13%bot},[<posf=int32#3,: 64]
# asm 2: vld1.8 {<f89=d24},[<posf=r2,: 64]
vld1.8 {d24},[r2,: 64]

# qhasm:   4x mix += g02 
# asm 1: vadd.i32 >mix=reg128#6,<mix=reg128#6,<g02=reg128#1
# asm 2: vadd.i32 >mix=q5,<mix=q5,<g02=q0
vadd.i32 q5,q5,q0

# qhasm:   posF = playground1_ptr + 288 
# asm 1: add >posF=int32#3,<playground1_ptr=int32#4,#288
# asm 2: add >posF=r2,<playground1_ptr=r3,#288
add r2,r3,#288

# qhasm:   f13 aligned= mem128[posF];posF += 16 
# asm 1: vld1.8 {>f13=reg128#14%bot->f13=reg128#14%top},[<posF=int32#3,: 128]!
# asm 2: vld1.8 {>f13=d26->f13=d27},[<posF=r2,: 128]!
vld1.8 {d26-d27},[r2,: 128]!

# qhasm:   4x g13_19 += g13 
# asm 1: vadd.i32 >g13_19=reg128#7,<g13_19=reg128#7,<g13=reg128#4
# asm 2: vadd.i32 >g13_19=q6,<g13_19=q6,<g13=q3
vadd.i32 q6,q6,q3

# qhasm:   f57 aligned= mem128[posF];posF += 16 
# asm 1: vld1.8 {>f57=reg128#15%bot->f57=reg128#15%top},[<posF=int32#3,: 128]!
# asm 2: vld1.8 {>f57=d28->f57=d29},[<posF=r2,: 128]!
vld1.8 {d28-d29},[r2,: 128]!

# qhasm:   4x g57_19 += g57 
# asm 1: vadd.i32 >g57_19=reg128#9,<g57_19=reg128#9,<g57=reg128#5
# asm 2: vadd.i32 >g57_19=q8,<g57_19=q8,<g57=q4
vadd.i32 q8,q8,q4

# qhasm:   f89 aligned= f89[0] mem64[posF] 
# asm 1: vld1.8 {<f89=reg128#13%top},[<posF=int32#3,: 64]
# asm 2: vld1.8 {<f89=d25},[<posF=r2,: 64]
vld1.8 {d25},[r2,: 64]

# qhasm:   4x g89_19 += g89 
# asm 1: vadd.i32 >g89_19=reg128#11,<g89_19=reg128#11,<g89=reg128#3
# asm 2: vadd.i32 >g89_19=q10,<g89_19=q10,<g89=q2
vadd.i32 q10,q10,q2

# qhasm:   f02 f13 = f02[0]f13[0] f02[2]f13[2] f02[1]f13[1] f02[3]f13[3] 
# asm 1: vtrn.32 <f02=reg128#10,<f13=reg128#14
# asm 2: vtrn.32 <f02=q9,<f13=q13
vtrn.32 q9,q13

# qhasm:   4x g46_19 += g46 
# asm 1: vadd.i32 >g46_19=reg128#8,<g46_19=reg128#8,<g46=reg128#2
# asm 2: vadd.i32 >g46_19=q7,<g46_19=q7,<g46=q1
vadd.i32 q7,q7,q1

# qhasm:   4x mix += g02 
# asm 1: vadd.i32 >mix=reg128#6,<mix=reg128#6,<g02=reg128#1
# asm 2: vadd.i32 >mix=q5,<mix=q5,<g02=q0
vadd.i32 q5,q5,q0

# qhasm:   f46 f57 = f46[0]f57[0] f46[2]f57[2] f46[1]f57[1] f46[3]f57[3] 
# asm 1: vtrn.32 <f46=reg128#12,<f57=reg128#15
# asm 2: vtrn.32 <f46=q11,<f57=q14
vtrn.32 q11,q14

# qhasm:   4x g13_19 += g13 
# asm 1: vadd.i32 >g13_19=reg128#7,<g13_19=reg128#7,<g13=reg128#4
# asm 2: vadd.i32 >g13_19=q6,<g13_19=q6,<g13=q3
vadd.i32 q6,q6,q3

# qhasm:   new g13_19_stack 

# qhasm:   ptr = &g13_19_stack 
# asm 1: lea >ptr=int32#3,<g13_19_stack=stack128#4
# asm 2: lea >ptr=r2,<g13_19_stack=[sp,#560]
add r2,sp,#560

# qhasm:   4x g89_19 += g89 
# asm 1: vadd.i32 >g89_19=reg128#11,<g89_19=reg128#11,<g89=reg128#3
# asm 2: vadd.i32 >g89_19=q10,<g89_19=q10,<g89=q2
vadd.i32 q10,q10,q2

# qhasm:   f89 = f89[0] f89[2] f89[1] f89[3] 
# asm 1: vtrn.32 <f89=reg128#13%bot,<f89=reg128#13%top
# asm 2: vtrn.32 <f89=d24,<f89=d25
vtrn.32 d24,d25

# qhasm:   mem128[ptr] aligned= g13_19 
# asm 1: vst1.8 {<g13_19=reg128#7%bot-<g13_19=reg128#7%top},[<ptr=int32#3,: 128]
# asm 2: vst1.8 {<g13_19=d12-<g13_19=d13},[<ptr=r2,: 128]
vst1.8 {d12-d13},[r2,: 128]

# qhasm:   4x f13_2 = f13 << 1 
# asm 1: vshl.i32 >f13_2=reg128#7,<f13=reg128#14,#1
# asm 2: vshl.i32 >f13_2=q6,<f13=q13,#1
vshl.i32 q6,q13,#1

# qhasm:   new g89_19_stack 

# qhasm:   ptr = &g89_19_stack 
# asm 1: lea >ptr=int32#3,<g89_19_stack=stack128#5
# asm 2: lea >ptr=r2,<g89_19_stack=[sp,#576]
add r2,sp,#576

# qhasm:   mem128[ptr] aligned= g89_19 
# asm 1: vst1.8 {<g89_19=reg128#11%bot-<g89_19=reg128#11%top},[<ptr=int32#3,: 128]
# asm 2: vst1.8 {<g89_19=d20-<g89_19=d21},[<ptr=r2,: 128]
vst1.8 {d20-d21},[r2,: 128]

# qhasm:   4x f57_2 = f57 << 1 
# asm 1: vshl.i32 >f57_2=reg128#11,<f57=reg128#15,#1
# asm 2: vshl.i32 >f57_2=q10,<f57=q14,#1
vshl.i32 q10,q14,#1

# qhasm:   new f13_2_stack 

# qhasm:   ptr = &f13_2_stack 
# asm 1: lea >ptr=int32#3,<f13_2_stack=stack128#6
# asm 2: lea >ptr=r2,<f13_2_stack=[sp,#592]
add r2,sp,#592

# qhasm:   mem128[ptr] aligned= f13_2 
# asm 1: vst1.8 {<f13_2=reg128#7%bot-<f13_2=reg128#7%top},[<ptr=int32#3,: 128]
# asm 2: vst1.8 {<f13_2=d12-<f13_2=d13},[<ptr=r2,: 128]
vst1.8 {d12-d13},[r2,: 128]

# qhasm:   4x f89_2 = f89 << 1 
# asm 1: vshl.i32 >f89_2=reg128#16,<f89=reg128#13,#1
# asm 2: vshl.i32 >f89_2=q15,<f89=q12,#1
vshl.i32 q15,q12,#1

# qhasm:   4x g57_19 += g57 
# asm 1: vadd.i32 >g57_19=reg128#9,<g57_19=reg128#9,<g57=reg128#5
# asm 2: vadd.i32 >g57_19=q8,<g57_19=q8,<g57=q4
vadd.i32 q8,q8,q4

# qhasm:   mix = f89_2[2,3] mix[2,3] 
# asm 1: vext.32 <mix=reg128#6%bot,<f89_2=reg128#16%top,<f89_2=reg128#16%bot,#0
# asm 2: vext.32 <mix=d10,<f89_2=d31,<f89_2=d30,#0
vext.32 d10,d31,d30,#0

# qhasm:   4x g46_19 += g46 
# asm 1: vadd.i32 >g46_19=reg128#8,<g46_19=reg128#8,<g46=reg128#2
# asm 2: vadd.i32 >g46_19=q7,<g46_19=q7,<g46=q1
vadd.i32 q7,q7,q1

# qhasm:   new g57_19_stack 

# qhasm:   ptr = &g57_19_stack 
# asm 1: lea >ptr=int32#3,<g57_19_stack=stack128#7
# asm 2: lea >ptr=r2,<g57_19_stack=[sp,#608]
add r2,sp,#608

# qhasm:   mem128[ptr] aligned= g57_19 
# asm 1: vst1.8 {<g57_19=reg128#9%bot-<g57_19=reg128#9%top},[<ptr=int32#3,: 128]
# asm 2: vst1.8 {<g57_19=d16-<g57_19=d17},[<ptr=r2,: 128]
vst1.8 {d16-d17},[r2,: 128]

# qhasm:   h9[0,1] = f02[0] signed* g89[2];h9[2,3] = f02[1] signed* g89[3] 
# asm 1: vmull.s32 >h9=reg128#9,<f02=reg128#10%bot,<g89=reg128#3%top
# asm 2: vmull.s32 >h9=q8,<f02=d18,<g89=d5
vmull.s32 q8,d18,d5

# qhasm:   h9[0,1] += f13[0] signed* g89[0];h9[2,3] += f13[1] signed* g89[1] 
# asm 1: vmlal.s32 <h9=reg128#9,<f13=reg128#14%bot,<g89=reg128#3%bot
# asm 2: vmlal.s32 <h9=q8,<f13=d26,<g89=d4
vmlal.s32 q8,d26,d4

# qhasm:   h9[0,1] += f02[2] signed* g57[2];h9[2,3] += f02[3] signed* g57[3] 
# asm 1: vmlal.s32 <h9=reg128#9,<f02=reg128#10%top,<g57=reg128#5%top
# asm 2: vmlal.s32 <h9=q8,<f02=d19,<g57=d9
vmlal.s32 q8,d19,d9

# qhasm:   h9[0,1] += f13[2] signed* g46[2];h9[2,3] += f13[3] signed* g46[3] 
# asm 1: vmlal.s32 <h9=reg128#9,<f13=reg128#14%top,<g46=reg128#2%top
# asm 2: vmlal.s32 <h9=q8,<f13=d27,<g46=d3
vmlal.s32 q8,d27,d3

# qhasm:   h9[0,1] += f46[0] signed* g57[0];h9[2,3] += f46[1] signed* g57[1] 
# asm 1: vmlal.s32 <h9=reg128#9,<f46=reg128#12%bot,<g57=reg128#5%bot
# asm 2: vmlal.s32 <h9=q8,<f46=d22,<g57=d8
vmlal.s32 q8,d22,d8

# qhasm:   h9[0,1] += f57[0] signed* g46[0];h9[2,3] += f57[1] signed* g46[1] 
# asm 1: vmlal.s32 <h9=reg128#9,<f57=reg128#15%bot,<g46=reg128#2%bot
# asm 2: vmlal.s32 <h9=q8,<f57=d28,<g46=d2
vmlal.s32 q8,d28,d2

# qhasm:   h9[0,1] += f46[2] signed* g13[2];h9[2,3] += f46[3] signed* g13[3] 
# asm 1: vmlal.s32 <h9=reg128#9,<f46=reg128#12%top,<g13=reg128#4%top
# asm 2: vmlal.s32 <h9=q8,<f46=d23,<g13=d7
vmlal.s32 q8,d23,d7

# qhasm:   h9[0,1] += f57[2] signed* g02[2];h9[2,3] += f57[3] signed* g02[3] 
# asm 1: vmlal.s32 <h9=reg128#9,<f57=reg128#15%top,<g02=reg128#1%top
# asm 2: vmlal.s32 <h9=q8,<f57=d29,<g02=d1
vmlal.s32 q8,d29,d1

# qhasm:   h9[0,1] += f89[0] signed* g13[0];h9[2,3] += f89[1] signed* g13[1] 
# asm 1: vmlal.s32 <h9=reg128#9,<f89=reg128#13%bot,<g13=reg128#4%bot
# asm 2: vmlal.s32 <h9=q8,<f89=d24,<g13=d6
vmlal.s32 q8,d24,d6

# qhasm:   h9[0,1] += f89[2] signed* g02[0];h9[2,3] += f89[3] signed* g02[1] 
# asm 1: vmlal.s32 <h9=reg128#9,<f89=reg128#13%top,<g02=reg128#1%bot
# asm 2: vmlal.s32 <h9=q8,<f89=d25,<g02=d0
vmlal.s32 q8,d25,d0

# qhasm:   new g46_19_stack 

# qhasm:   ptr = &g46_19_stack 
# asm 1: lea >ptr=int32#3,<g46_19_stack=stack128#8
# asm 2: lea >ptr=r2,<g46_19_stack=[sp,#624]
add r2,sp,#624

# qhasm:   mem128[ptr] aligned= g46_19 
# asm 1: vst1.8 {<g46_19=reg128#8%bot-<g46_19=reg128#8%top},[<ptr=int32#3,: 128]
# asm 2: vst1.8 {<g46_19=d14-<g46_19=d15},[<ptr=r2,: 128]
vst1.8 {d14-d15},[r2,: 128]

# qhasm:   h8[0,1] = f02[0] signed* g89[0];h8[2,3] = f02[1] signed* g89[1] 
# asm 1: vmull.s32 >h8=reg128#3,<f02=reg128#10%bot,<g89=reg128#3%bot
# asm 2: vmull.s32 >h8=q2,<f02=d18,<g89=d4
vmull.s32 q2,d18,d4

# qhasm:   h8[0,1] += f13_2[0] signed* g57[2];h8[2,3] += f13_2[1] signed* g57[3] 
# asm 1: vmlal.s32 <h8=reg128#3,<f13_2=reg128#7%bot,<g57=reg128#5%top
# asm 2: vmlal.s32 <h8=q2,<f13_2=d12,<g57=d9
vmlal.s32 q2,d12,d9

# qhasm:   h8[0,1] += f13_2[2] signed* g57[0];h8[2,3] += f13_2[3] signed* g57[1] 
# asm 1: vmlal.s32 <h8=reg128#3,<f13_2=reg128#7%top,<g57=reg128#5%bot
# asm 2: vmlal.s32 <h8=q2,<f13_2=d13,<g57=d8
vmlal.s32 q2,d13,d8

# qhasm:   h8[0,1] += f02[2] signed* g46[2];h8[2,3] += f02[3] signed* g46[3] 
# asm 1: vmlal.s32 <h8=reg128#3,<f02=reg128#10%top,<g46=reg128#2%top
# asm 2: vmlal.s32 <h8=q2,<f02=d19,<g46=d3
vmlal.s32 q2,d19,d3

# qhasm:   h8[0,1] += f46[0] signed* g46[0];h8[2,3] += f46[1] signed* g46[1] 
# asm 1: vmlal.s32 <h8=reg128#3,<f46=reg128#12%bot,<g46=reg128#2%bot
# asm 2: vmlal.s32 <h8=q2,<f46=d22,<g46=d2
vmlal.s32 q2,d22,d2

# qhasm:   h8[0,1] += f46[2] signed* g02[2];h8[2,3] += f46[3] signed* g02[3] 
# asm 1: vmlal.s32 <h8=reg128#3,<f46=reg128#12%top,<g02=reg128#1%top
# asm 2: vmlal.s32 <h8=q2,<f46=d23,<g02=d1
vmlal.s32 q2,d23,d1

# qhasm:   h8[0,1] += f89[0] signed* g02[0];h8[2,3] += f89[1] signed* g02[1] 
# asm 1: vmlal.s32 <h8=reg128#3,<f89=reg128#13%bot,<g02=reg128#1%bot
# asm 2: vmlal.s32 <h8=q2,<f89=d24,<g02=d0
vmlal.s32 q2,d24,d0

# qhasm:   new f57_2_stack 

# qhasm:   ptr = &f57_2_stack 
# asm 1: lea >ptr=int32#3,<f57_2_stack=stack128#9
# asm 2: lea >ptr=r2,<f57_2_stack=[sp,#640]
add r2,sp,#640

# qhasm:   mem128[ptr] aligned= f57_2 
# asm 1: vst1.8 {<f57_2=reg128#11%bot-<f57_2=reg128#11%top},[<ptr=int32#3,: 128]
# asm 2: vst1.8 {<f57_2=d20-<f57_2=d21},[<ptr=r2,: 128]
vst1.8 {d20-d21},[r2,: 128]

# qhasm:   h7[0,1] = f02[0] signed* g57[2];h7[2,3] = f02[1] signed* g57[3] 
# asm 1: vmull.s32 >h7=reg128#8,<f02=reg128#10%bot,<g57=reg128#5%top
# asm 2: vmull.s32 >h7=q7,<f02=d18,<g57=d9
vmull.s32 q7,d18,d9

# qhasm:   h7[0,1] += f13[0] signed* g46[2];h7[2,3] += f13[1] signed* g46[3] 
# asm 1: vmlal.s32 <h7=reg128#8,<f13=reg128#14%bot,<g46=reg128#2%top
# asm 2: vmlal.s32 <h7=q7,<f13=d26,<g46=d3
vmlal.s32 q7,d26,d3

# qhasm:   h7[0,1] += f02[2] signed* g57[0];h7[2,3] += f02[3] signed* g57[1] 
# asm 1: vmlal.s32 <h7=reg128#8,<f02=reg128#10%top,<g57=reg128#5%bot
# asm 2: vmlal.s32 <h7=q7,<f02=d19,<g57=d8
vmlal.s32 q7,d19,d8

# qhasm:   h7[0,1] += f13[2] signed* g46[0];h7[2,3] += f13[3] signed* g46[1] 
# asm 1: vmlal.s32 <h7=reg128#8,<f13=reg128#14%top,<g46=reg128#2%bot
# asm 2: vmlal.s32 <h7=q7,<f13=d27,<g46=d2
vmlal.s32 q7,d27,d2

# qhasm:   h7[0,1] += f46[0] signed* g13[2];h7[2,3] += f46[1] signed* g13[3] 
# asm 1: vmlal.s32 <h7=reg128#8,<f46=reg128#12%bot,<g13=reg128#4%top
# asm 2: vmlal.s32 <h7=q7,<f46=d22,<g13=d7
vmlal.s32 q7,d22,d7

# qhasm:   h7[0,1] += f57[0] signed* g02[2];h7[2,3] += f57[1] signed* g02[3] 
# asm 1: vmlal.s32 <h7=reg128#8,<f57=reg128#15%bot,<g02=reg128#1%top
# asm 2: vmlal.s32 <h7=q7,<f57=d28,<g02=d1
vmlal.s32 q7,d28,d1

# qhasm:   h7[0,1] += f46[2] signed* g13[0];h7[2,3] += f46[3] signed* g13[1] 
# asm 1: vmlal.s32 <h7=reg128#8,<f46=reg128#12%top,<g13=reg128#4%bot
# asm 2: vmlal.s32 <h7=q7,<f46=d23,<g13=d6
vmlal.s32 q7,d23,d6

# qhasm:   h7[0,1] += f57[2] signed* g02[0];h7[2,3] += f57[3] signed* g02[1] 
# asm 1: vmlal.s32 <h7=reg128#8,<f57=reg128#15%top,<g02=reg128#1%bot
# asm 2: vmlal.s32 <h7=q7,<f57=d29,<g02=d0
vmlal.s32 q7,d29,d0

# qhasm:   new mix_stack 

# qhasm:   ptr = &mix_stack 
# asm 1: lea >ptr=int32#3,<mix_stack=stack128#10
# asm 2: lea >ptr=r2,<mix_stack=[sp,#656]
add r2,sp,#656

# qhasm:   mem128[ptr] aligned= mix 
# asm 1: vst1.8 {<mix=reg128#6%bot-<mix=reg128#6%top},[<ptr=int32#3,: 128]
# asm 2: vst1.8 {<mix=d10-<mix=d11},[<ptr=r2,: 128]
vst1.8 {d10-d11},[r2,: 128]

# qhasm:   h6[0,1] = f02[0] signed* g46[2];h6[2,3] = f02[1] signed* g46[3] 
# asm 1: vmull.s32 >h6=reg128#6,<f02=reg128#10%bot,<g46=reg128#2%top
# asm 2: vmull.s32 >h6=q5,<f02=d18,<g46=d3
vmull.s32 q5,d18,d3

# qhasm:   h6[0,1] += f02[2] signed* g46[0];h6[2,3] += f02[3] signed* g46[1] 
# asm 1: vmlal.s32 <h6=reg128#6,<f02=reg128#10%top,<g46=reg128#2%bot
# asm 2: vmlal.s32 <h6=q5,<f02=d19,<g46=d2
vmlal.s32 q5,d19,d2

# qhasm:   h6[0,1] += f46[0] signed* g02[2];h6[2,3] += f46[1] signed* g02[3] 
# asm 1: vmlal.s32 <h6=reg128#6,<f46=reg128#12%bot,<g02=reg128#1%top
# asm 2: vmlal.s32 <h6=q5,<f46=d22,<g02=d1
vmlal.s32 q5,d22,d1

# qhasm:   h6[0,1] += f46[2] signed* g02[0];h6[2,3] += f46[3] signed* g02[1] 
# asm 1: vmlal.s32 <h6=reg128#6,<f46=reg128#12%top,<g02=reg128#1%bot
# asm 2: vmlal.s32 <h6=q5,<f46=d23,<g02=d0
vmlal.s32 q5,d23,d0

# qhasm:   h6[0,1] += f13_2[0] signed* g57[0];h6[2,3] += f13_2[1] signed* g57[1] 
# asm 1: vmlal.s32 <h6=reg128#6,<f13_2=reg128#7%bot,<g57=reg128#5%bot
# asm 2: vmlal.s32 <h6=q5,<f13_2=d12,<g57=d8
vmlal.s32 q5,d12,d8

# qhasm:   new h9_stack 

# qhasm:   ptr = &h9_stack 
# asm 1: lea >ptr=int32#3,<h9_stack=stack128#11
# asm 2: lea >ptr=r2,<h9_stack=[sp,#672]
add r2,sp,#672

# qhasm:   mem128[ptr] aligned= h9 
# asm 1: vst1.8 {<h9=reg128#9%bot-<h9=reg128#9%top},[<ptr=int32#3,: 128]
# asm 2: vst1.8 {<h9=d16-<h9=d17},[<ptr=r2,: 128]
vst1.8 {d16-d17},[r2,: 128]

# qhasm:   h5[0,1] = f02[0] signed* g57[0];h5[2,3] = f02[1] signed* g57[1] 
# asm 1: vmull.s32 >h5=reg128#5,<f02=reg128#10%bot,<g57=reg128#5%bot
# asm 2: vmull.s32 >h5=q4,<f02=d18,<g57=d8
vmull.s32 q4,d18,d8

# qhasm:   h5[0,1] += f13[0] signed* g46[0];h5[2,3] += f13[1] signed* g46[1] 
# asm 1: vmlal.s32 <h5=reg128#5,<f13=reg128#14%bot,<g46=reg128#2%bot
# asm 2: vmlal.s32 <h5=q4,<f13=d26,<g46=d2
vmlal.s32 q4,d26,d2

# qhasm:   h5[0,1] += f02[2] signed* g13[2];h5[2,3] += f02[3] signed* g13[3] 
# asm 1: vmlal.s32 <h5=reg128#5,<f02=reg128#10%top,<g13=reg128#4%top
# asm 2: vmlal.s32 <h5=q4,<f02=d19,<g13=d7
vmlal.s32 q4,d19,d7

# qhasm:   h5[0,1] += f13[2] signed* g02[2];h5[2,3] += f13[3] signed* g02[3] 
# asm 1: vmlal.s32 <h5=reg128#5,<f13=reg128#14%top,<g02=reg128#1%top
# asm 2: vmlal.s32 <h5=q4,<f13=d27,<g02=d1
vmlal.s32 q4,d27,d1

# qhasm:   h5[0,1] += f46[0] signed* g13[0];h5[2,3] += f46[1] signed* g13[1] 
# asm 1: vmlal.s32 <h5=reg128#5,<f46=reg128#12%bot,<g13=reg128#4%bot
# asm 2: vmlal.s32 <h5=q4,<f46=d22,<g13=d6
vmlal.s32 q4,d22,d6

# qhasm:   h5[0,1] += f57[0] signed* g02[0];h5[2,3] += f57[1] signed* g02[1] 
# asm 1: vmlal.s32 <h5=reg128#5,<f57=reg128#15%bot,<g02=reg128#1%bot
# asm 2: vmlal.s32 <h5=q4,<f57=d28,<g02=d0
vmlal.s32 q4,d28,d0

# qhasm:   h3[0,1] = f02[0] signed* g13[2];h3[2,3] = f02[1] signed* g13[3] 
# asm 1: vmull.s32 >h3=reg128#9,<f02=reg128#10%bot,<g13=reg128#4%top
# asm 2: vmull.s32 >h3=q8,<f02=d18,<g13=d7
vmull.s32 q8,d18,d7

# qhasm:   h3[0,1] += f13[0] signed* g02[2];h3[2,3] += f13[1] signed* g02[3] 
# asm 1: vmlal.s32 <h3=reg128#9,<f13=reg128#14%bot,<g02=reg128#1%top
# asm 2: vmlal.s32 <h3=q8,<f13=d26,<g02=d1
vmlal.s32 q8,d26,d1

# qhasm:   h3[0,1] += f02[2] signed* g13[0];h3[2,3] += f02[3] signed* g13[1] 
# asm 1: vmlal.s32 <h3=reg128#9,<f02=reg128#10%top,<g13=reg128#4%bot
# asm 2: vmlal.s32 <h3=q8,<f02=d19,<g13=d6
vmlal.s32 q8,d19,d6

# qhasm:   h3[0,1] += f13[2] signed* g02[0];h3[2,3] += f13[3] signed* g02[1] 
# asm 1: vmlal.s32 <h3=reg128#9,<f13=reg128#14%top,<g02=reg128#1%bot
# asm 2: vmlal.s32 <h3=q8,<f13=d27,<g02=d0
vmlal.s32 q8,d27,d0

# qhasm:   ptr = &g89_19_stack 
# asm 1: lea >ptr=int32#3,<g89_19_stack=stack128#5
# asm 2: lea >ptr=r2,<g89_19_stack=[sp,#576]
add r2,sp,#576

# qhasm:   g89_19 aligned= mem128[ptr] 
# asm 1: vld1.8 {>g89_19=reg128#11%bot->g89_19=reg128#11%top},[<ptr=int32#3,: 128]
# asm 2: vld1.8 {>g89_19=d20->g89_19=d21},[<ptr=r2,: 128]
vld1.8 {d20-d21},[r2,: 128]

# qhasm:   h7[0,1] += f89[0] signed* g89_19[2];h7[2,3] += f89[1] signed* g89_19[3] 
# asm 1: vmlal.s32 <h7=reg128#8,<f89=reg128#13%bot,<g89_19=reg128#11%top
# asm 2: vmlal.s32 <h7=q7,<f89=d24,<g89_19=d21
vmlal.s32 q7,d24,d21

# qhasm:   h7[0,1] += f89[2] signed* g89_19[0];h7[2,3] += f89[3] signed* g89_19[1] 
# asm 1: vmlal.s32 <h7=reg128#8,<f89=reg128#13%top,<g89_19=reg128#11%bot
# asm 2: vmlal.s32 <h7=q7,<f89=d25,<g89_19=d20
vmlal.s32 q7,d25,d20

# qhasm:   h5[0,1] += f46[2] signed* g89_19[2];h5[2,3] += f46[3] signed* g89_19[3] 
# asm 1: vmlal.s32 <h5=reg128#5,<f46=reg128#12%top,<g89_19=reg128#11%top
# asm 2: vmlal.s32 <h5=q4,<f46=d23,<g89_19=d21
vmlal.s32 q4,d23,d21

# qhasm:   h5[0,1] += f57[2] signed* g89_19[0];h5[2,3] += f57[3] signed* g89_19[1] 
# asm 1: vmlal.s32 <h5=reg128#5,<f57=reg128#15%top,<g89_19=reg128#11%bot
# asm 2: vmlal.s32 <h5=q4,<f57=d29,<g89_19=d20
vmlal.s32 q4,d29,d20

# qhasm:   h3[0,1] += f46[0] signed* g89_19[2];h3[2,3] += f46[1] signed* g89_19[3] 
# asm 1: vmlal.s32 <h3=reg128#9,<f46=reg128#12%bot,<g89_19=reg128#11%top
# asm 2: vmlal.s32 <h3=q8,<f46=d22,<g89_19=d21
vmlal.s32 q8,d22,d21

# qhasm:   h3[0,1] += f57[0] signed* g89_19[0];h3[2,3] += f57[1] signed* g89_19[1] 
# asm 1: vmlal.s32 <h3=reg128#9,<f57=reg128#15%bot,<g89_19=reg128#11%bot
# asm 2: vmlal.s32 <h3=q8,<f57=d28,<g89_19=d20
vmlal.s32 q8,d28,d20

# qhasm:   h6[0,1] += f89[0] signed* g89_19[0];h6[2,3] += f89[1] signed* g89_19[1] 
# asm 1: vmlal.s32 <h6=reg128#6,<f89=reg128#13%bot,<g89_19=reg128#11%bot
# asm 2: vmlal.s32 <h6=q5,<f89=d24,<g89_19=d20
vmlal.s32 q5,d24,d20

# qhasm:   new h7_stack 

# qhasm:   ptr = &h7_stack 
# asm 1: lea >ptr=int32#3,<h7_stack=stack128#5
# asm 2: lea >ptr=r2,<h7_stack=[sp,#576]
add r2,sp,#576

# qhasm:   mem128[ptr] aligned= h7 
# asm 1: vst1.8 {<h7=reg128#8%bot-<h7=reg128#8%top},[<ptr=int32#3,: 128]
# asm 2: vst1.8 {<h7=d14-<h7=d15},[<ptr=r2,: 128]
vst1.8 {d14-d15},[r2,: 128]

# qhasm:   h1[0,1] = f02[0] signed* g13[0];h1[2,3] = f02[1] signed* g13[1] 
# asm 1: vmull.s32 >h1=reg128#8,<f02=reg128#10%bot,<g13=reg128#4%bot
# asm 2: vmull.s32 >h1=q7,<f02=d18,<g13=d6
vmull.s32 q7,d18,d6

# qhasm:   h1[0,1] += f13[0] signed* g02[0];h1[2,3] += f13[1] signed* g02[1] 
# asm 1: vmlal.s32 <h1=reg128#8,<f13=reg128#14%bot,<g02=reg128#1%bot
# asm 2: vmlal.s32 <h1=q7,<f13=d26,<g02=d0
vmlal.s32 q7,d26,d0

# qhasm:   ptr = &mix_stack 
# asm 1: lea >ptr=int32#3,<mix_stack=stack128#10
# asm 2: lea >ptr=r2,<mix_stack=[sp,#656]
add r2,sp,#656

# qhasm:   mix aligned= mem128[ptr] 
# asm 1: vld1.8 {>mix=reg128#16%bot->mix=reg128#16%top},[<ptr=int32#3,: 128]
# asm 2: vld1.8 {>mix=d30->mix=d31},[<ptr=r2,: 128]
vld1.8 {d30-d31},[r2,: 128]

# qhasm:   h8[0,1] += mix[0] signed* g89_19[2];h8[2,3] += mix[1] signed* g89_19[3] 
# asm 1: vmlal.s32 <h8=reg128#3,<mix=reg128#16%bot,<g89_19=reg128#11%top
# asm 2: vmlal.s32 <h8=q2,<mix=d30,<g89_19=d21
vmlal.s32 q2,d30,d21

# qhasm:   h1[0,1] += f02[2] signed* g89_19[2];h1[2,3] += f02[3] signed* g89_19[3] 
# asm 1: vmlal.s32 <h1=reg128#8,<f02=reg128#10%top,<g89_19=reg128#11%top
# asm 2: vmlal.s32 <h1=q7,<f02=d19,<g89_19=d21
vmlal.s32 q7,d19,d21

# qhasm:   h1[0,1] += f13[2] signed* g89_19[0];h1[2,3] += f13[3] signed* g89_19[1] 
# asm 1: vmlal.s32 <h1=reg128#8,<f13=reg128#14%top,<g89_19=reg128#11%bot
# asm 2: vmlal.s32 <h1=q7,<f13=d27,<g89_19=d20
vmlal.s32 q7,d27,d20

# qhasm:   ptr = &g46_19_stack 
# asm 1: lea >ptr=int32#3,<g46_19_stack=stack128#8
# asm 2: lea >ptr=r2,<g46_19_stack=[sp,#624]
add r2,sp,#624

# qhasm:   g46_19 aligned= mem128[ptr] 
# asm 1: vld1.8 {>g46_19=reg128#14%bot->g46_19=reg128#14%top},[<ptr=int32#3,: 128]
# asm 2: vld1.8 {>g46_19=d26->g46_19=d27},[<ptr=r2,: 128]
vld1.8 {d26-d27},[r2,: 128]

# qhasm:   h5[0,1] += f89[2] signed* g46_19[2];h5[2,3] += f89[3] signed* g46_19[3] 
# asm 1: vmlal.s32 <h5=reg128#5,<f89=reg128#13%top,<g46_19=reg128#14%top
# asm 2: vmlal.s32 <h5=q4,<f89=d25,<g46_19=d27
vmlal.s32 q4,d25,d27

# qhasm:   h3[0,1] += f57[2] signed* g46_19[2];h3[2,3] += f57[3] signed* g46_19[3] 
# asm 1: vmlal.s32 <h3=reg128#9,<f57=reg128#15%top,<g46_19=reg128#14%top
# asm 2: vmlal.s32 <h3=q8,<f57=d29,<g46_19=d27
vmlal.s32 q8,d29,d27

# qhasm:   h3[0,1] += f89[2] signed* g46_19[0];h3[2,3] += f89[3] signed* g46_19[1] 
# asm 1: vmlal.s32 <h3=reg128#9,<f89=reg128#13%top,<g46_19=reg128#14%bot
# asm 2: vmlal.s32 <h3=q8,<f89=d25,<g46_19=d26
vmlal.s32 q8,d25,d26

# qhasm:   h1[0,1] += f57[0] signed* g46_19[2];h1[2,3] += f57[1] signed* g46_19[3] 
# asm 1: vmlal.s32 <h1=reg128#8,<f57=reg128#15%bot,<g46_19=reg128#14%top
# asm 2: vmlal.s32 <h1=q7,<f57=d28,<g46_19=d27
vmlal.s32 q7,d28,d27

# qhasm:   h1[0,1] += f57[2] signed* g46_19[0];h1[2,3] += f57[3] signed* g46_19[1] 
# asm 1: vmlal.s32 <h1=reg128#8,<f57=reg128#15%top,<g46_19=reg128#14%bot
# asm 2: vmlal.s32 <h1=q7,<f57=d29,<g46_19=d26
vmlal.s32 q7,d29,d26

# qhasm:   ptr = &g57_19_stack 
# asm 1: lea >ptr=int32#3,<g57_19_stack=stack128#7
# asm 2: lea >ptr=r2,<g57_19_stack=[sp,#608]
add r2,sp,#608

# qhasm:   g57_19 aligned= mem128[ptr] 
# asm 1: vld1.8 {>g57_19=reg128#15%bot->g57_19=reg128#15%top},[<ptr=int32#3,: 128]
# asm 2: vld1.8 {>g57_19=d28->g57_19=d29},[<ptr=r2,: 128]
vld1.8 {d28-d29},[r2,: 128]

# qhasm:   h5[0,1] += f89[0] signed* g57_19[2];h5[2,3] += f89[1] signed* g57_19[3] 
# asm 1: vmlal.s32 <h5=reg128#5,<f89=reg128#13%bot,<g57_19=reg128#15%top
# asm 2: vmlal.s32 <h5=q4,<f89=d24,<g57_19=d29
vmlal.s32 q4,d24,d29

# qhasm:   h3[0,1] += f46[2] signed* g57_19[2];h3[2,3] += f46[3] signed* g57_19[3] 
# asm 1: vmlal.s32 <h3=reg128#9,<f46=reg128#12%top,<g57_19=reg128#15%top
# asm 2: vmlal.s32 <h3=q8,<f46=d23,<g57_19=d29
vmlal.s32 q8,d23,d29

# qhasm:   h3[0,1] += f89[0] signed* g57_19[0];h3[2,3] += f89[1] signed* g57_19[1] 
# asm 1: vmlal.s32 <h3=reg128#9,<f89=reg128#13%bot,<g57_19=reg128#15%bot
# asm 2: vmlal.s32 <h3=q8,<f89=d24,<g57_19=d28
vmlal.s32 q8,d24,d28

# qhasm:   h1[0,1] += f46[0] signed* g57_19[2];h1[2,3] += f46[1] signed* g57_19[3] 
# asm 1: vmlal.s32 <h1=reg128#8,<f46=reg128#12%bot,<g57_19=reg128#15%top
# asm 2: vmlal.s32 <h1=q7,<f46=d22,<g57_19=d29
vmlal.s32 q7,d22,d29

# qhasm:   h1[0,1] += f46[2] signed* g57_19[0];h1[2,3] += f46[3] signed* g57_19[1] 
# asm 1: vmlal.s32 <h1=reg128#8,<f46=reg128#12%top,<g57_19=reg128#15%bot
# asm 2: vmlal.s32 <h1=q7,<f46=d23,<g57_19=d28
vmlal.s32 q7,d23,d28

# qhasm:   new h5_stack 

# qhasm:   ptr = &h5_stack 
# asm 1: lea >ptr=int32#3,<h5_stack=stack128#7
# asm 2: lea >ptr=r2,<h5_stack=[sp,#608]
add r2,sp,#608

# qhasm:   mem128[ptr] aligned= h5 
# asm 1: vst1.8 {<h5=reg128#5%bot-<h5=reg128#5%top},[<ptr=int32#3,: 128]
# asm 2: vst1.8 {<h5=d8-<h5=d9},[<ptr=r2,: 128]
vst1.8 {d8-d9},[r2,: 128]

# qhasm:   ptr = &g13_19_stack 
# asm 1: lea >ptr=int32#3,<g13_19_stack=stack128#4
# asm 2: lea >ptr=r2,<g13_19_stack=[sp,#560]
add r2,sp,#560

# qhasm:   g13_19 aligned= mem128[ptr] 
# asm 1: vld1.8 {>g13_19=reg128#5%bot->g13_19=reg128#5%top},[<ptr=int32#3,: 128]
# asm 2: vld1.8 {>g13_19=d8->g13_19=d9},[<ptr=r2,: 128]
vld1.8 {d8-d9},[r2,: 128]

# qhasm:   h1[0,1] += f89[0] signed* g13_19[2];h1[2,3] += f89[1] signed* g13_19[3] 
# asm 1: vmlal.s32 <h1=reg128#8,<f89=reg128#13%bot,<g13_19=reg128#5%top
# asm 2: vmlal.s32 <h1=q7,<f89=d24,<g13_19=d9
vmlal.s32 q7,d24,d9

# qhasm:   h1[0,1] += f89[2] signed* mix[2];h1[2,3] += f89[3] signed* mix[3] 
# asm 1: vmlal.s32 <h1=reg128#8,<f89=reg128#13%top,<mix=reg128#16%top
# asm 2: vmlal.s32 <h1=q7,<f89=d25,<mix=d31
vmlal.s32 q7,d25,d31

# qhasm:   h4[0,1] = f02[0] signed* g46[0];h4[2,3] = f02[1] signed* g46[1] 
# asm 1: vmull.s32 >h4=reg128#2,<f02=reg128#10%bot,<g46=reg128#2%bot
# asm 2: vmull.s32 >h4=q1,<f02=d18,<g46=d2
vmull.s32 q1,d18,d2

# qhasm:   h4[0,1] += f02[2] signed* g02[2];h4[2,3] += f02[3] signed* g02[3] 
# asm 1: vmlal.s32 <h4=reg128#2,<f02=reg128#10%top,<g02=reg128#1%top
# asm 2: vmlal.s32 <h4=q1,<f02=d19,<g02=d1
vmlal.s32 q1,d19,d1

# qhasm:   h4[0,1] += f46[0] signed* g02[0];h4[2,3] += f46[1] signed* g02[1] 
# asm 1: vmlal.s32 <h4=reg128#2,<f46=reg128#12%bot,<g02=reg128#1%bot
# asm 2: vmlal.s32 <h4=q1,<f46=d22,<g02=d0
vmlal.s32 q1,d22,d0

# qhasm:   h4[0,1] += f89[0] signed* g46_19[2];h4[2,3] += f89[1] signed* g46_19[3] 
# asm 1: vmlal.s32 <h4=reg128#2,<f89=reg128#13%bot,<g46_19=reg128#14%top
# asm 2: vmlal.s32 <h4=q1,<f89=d24,<g46_19=d27
vmlal.s32 q1,d24,d27

# qhasm:   h4[0,1] += f46[2] signed* g89_19[0];h4[2,3] += f46[3] signed* g89_19[1] 
# asm 1: vmlal.s32 <h4=reg128#2,<f46=reg128#12%top,<g89_19=reg128#11%bot
# asm 2: vmlal.s32 <h4=q1,<f46=d23,<g89_19=d20
vmlal.s32 q1,d23,d20

# qhasm:   h4[0,1] += f13_2[0] signed* g13[2];h4[2,3] += f13_2[1] signed* g13[3] 
# asm 1: vmlal.s32 <h4=reg128#2,<f13_2=reg128#7%bot,<g13=reg128#4%top
# asm 2: vmlal.s32 <h4=q1,<f13_2=d12,<g13=d7
vmlal.s32 q1,d12,d7

# qhasm:   h4[0,1] += f13_2[2] signed* g13[0];h4[2,3] += f13_2[3] signed* g13[1] 
# asm 1: vmlal.s32 <h4=reg128#2,<f13_2=reg128#7%top,<g13=reg128#4%bot
# asm 2: vmlal.s32 <h4=q1,<f13_2=d13,<g13=d6
vmlal.s32 q1,d13,d6

# qhasm:   h2[0,1] = f02[0] signed* g02[2];h2[2,3] = f02[1] signed* g02[3] 
# asm 1: vmull.s32 >h2=reg128#7,<f02=reg128#10%bot,<g02=reg128#1%top
# asm 2: vmull.s32 >h2=q6,<f02=d18,<g02=d1
vmull.s32 q6,d18,d1

# qhasm:   h2[0,1] += f02[2] signed* g02[0];h2[2,3] += f02[3] signed* g02[1] 
# asm 1: vmlal.s32 <h2=reg128#7,<f02=reg128#10%top,<g02=reg128#1%bot
# asm 2: vmlal.s32 <h2=q6,<f02=d19,<g02=d0
vmlal.s32 q6,d19,d0

# qhasm:   h2[0,1] += f46[2] signed* g46_19[2];h2[2,3] += f46[3] signed* g46_19[3] 
# asm 1: vmlal.s32 <h2=reg128#7,<f46=reg128#12%top,<g46_19=reg128#14%top
# asm 2: vmlal.s32 <h2=q6,<f46=d23,<g46_19=d27
vmlal.s32 q6,d23,d27

# qhasm:   h2[0,1] += f46[0] signed* g89_19[0];h2[2,3] += f46[1] signed* g89_19[1] 
# asm 1: vmlal.s32 <h2=reg128#7,<f46=reg128#12%bot,<g89_19=reg128#11%bot
# asm 2: vmlal.s32 <h2=q6,<f46=d22,<g89_19=d20
vmlal.s32 q6,d22,d20

# qhasm:   h2[0,1] += f89[0] signed* g46_19[0];h2[2,3] += f89[1] signed* g46_19[1] 
# asm 1: vmlal.s32 <h2=reg128#7,<f89=reg128#13%bot,<g46_19=reg128#14%bot
# asm 2: vmlal.s32 <h2=q6,<f89=d24,<g46_19=d26
vmlal.s32 q6,d24,d26

# qhasm:   h0[0,1] = f02[0] signed* g02[0];h0[2,3] = f02[1] signed* g02[1] 
# asm 1: vmull.s32 >h0=reg128#1,<f02=reg128#10%bot,<g02=reg128#1%bot
# asm 2: vmull.s32 >h0=q0,<f02=d18,<g02=d0
vmull.s32 q0,d18,d0

# qhasm:   h0[0,1] += f46[0] signed* g46_19[2];h0[2,3] += f46[1] signed* g46_19[3] 
# asm 1: vmlal.s32 <h0=reg128#1,<f46=reg128#12%bot,<g46_19=reg128#14%top
# asm 2: vmlal.s32 <h0=q0,<f46=d22,<g46_19=d27
vmlal.s32 q0,d22,d27

# qhasm:   h0[0,1] += f46[2] signed* g46_19[0];h0[2,3] += f46[3] signed* g46_19[1] 
# asm 1: vmlal.s32 <h0=reg128#1,<f46=reg128#12%top,<g46_19=reg128#14%bot
# asm 2: vmlal.s32 <h0=q0,<f46=d23,<g46_19=d26
vmlal.s32 q0,d23,d26

# qhasm:   h0[0,1] += f89[0] signed* mix[2];h0[2,3] += f89[1] signed* mix[3] 
# asm 1: vmlal.s32 <h0=reg128#1,<f89=reg128#13%bot,<mix=reg128#16%top
# asm 2: vmlal.s32 <h0=q0,<f89=d24,<mix=d31
vmlal.s32 q0,d24,d31

# qhasm:   h0[0,1] += f02[2] signed* g89_19[0];h0[2,3] += f02[3] signed* g89_19[1] 
# asm 1: vmlal.s32 <h0=reg128#1,<f02=reg128#10%top,<g89_19=reg128#11%bot
# asm 2: vmlal.s32 <h0=q0,<f02=d19,<g89_19=d20
vmlal.s32 q0,d19,d20

# qhasm:   ptr = &f57_2_stack 
# asm 1: lea >ptr=int32#3,<f57_2_stack=stack128#9
# asm 2: lea >ptr=r2,<f57_2_stack=[sp,#640]
add r2,sp,#640

# qhasm:   f57_2 aligned= mem128[ptr] 
# asm 1: vld1.8 {>f57_2=reg128#10%bot->f57_2=reg128#10%top},[<ptr=int32#3,: 128]
# asm 2: vld1.8 {>f57_2=d18->f57_2=d19},[<ptr=r2,: 128]
vld1.8 {d18-d19},[r2,: 128]

# qhasm:   h8[0,1] += f57_2[0] signed* g13[2];h8[2,3] += f57_2[1] signed* g13[3] 
# asm 1: vmlal.s32 <h8=reg128#3,<f57_2=reg128#10%bot,<g13=reg128#4%top
# asm 2: vmlal.s32 <h8=q2,<f57_2=d18,<g13=d7
vmlal.s32 q2,d18,d7

# qhasm:   h8[0,1] += f57_2[2] signed* g13[0];h8[2,3] += f57_2[3] signed* g13[1] 
# asm 1: vmlal.s32 <h8=reg128#3,<f57_2=reg128#10%top,<g13=reg128#4%bot
# asm 2: vmlal.s32 <h8=q2,<f57_2=d19,<g13=d6
vmlal.s32 q2,d19,d6

# qhasm:   h6[0,1] += f57_2[0] signed* g13[0];h6[2,3] += f57_2[1] signed* g13[1] 
# asm 1: vmlal.s32 <h6=reg128#6,<f57_2=reg128#10%bot,<g13=reg128#4%bot
# asm 2: vmlal.s32 <h6=q5,<f57_2=d18,<g13=d6
vmlal.s32 q5,d18,d6

# qhasm:   h6[0,1] += f57_2[2] signed* g89_19[2];h6[2,3] += f57_2[3] signed* g89_19[3] 
# asm 1: vmlal.s32 <h6=reg128#6,<f57_2=reg128#10%top,<g89_19=reg128#11%top
# asm 2: vmlal.s32 <h6=q5,<f57_2=d19,<g89_19=d21
vmlal.s32 q5,d19,d21

# qhasm:   h4[0,1] += f57_2[0] signed* g89_19[2];h4[2,3] += f57_2[1] signed* g89_19[3] 
# asm 1: vmlal.s32 <h4=reg128#2,<f57_2=reg128#10%bot,<g89_19=reg128#11%top
# asm 2: vmlal.s32 <h4=q1,<f57_2=d18,<g89_19=d21
vmlal.s32 q1,d18,d21

# qhasm:   h4[0,1] += f57_2[2] signed* g57_19[2];h4[2,3] += f57_2[3] signed* g57_19[3] 
# asm 1: vmlal.s32 <h4=reg128#2,<f57_2=reg128#10%top,<g57_19=reg128#15%top
# asm 2: vmlal.s32 <h4=q1,<f57_2=d19,<g57_19=d29
vmlal.s32 q1,d19,d29

# qhasm:   h0[0,1] += f57_2[0] signed* g57_19[0];h0[2,3] += f57_2[1] signed* g57_19[1] 
# asm 1: vmlal.s32 <h0=reg128#1,<f57_2=reg128#10%bot,<g57_19=reg128#15%bot
# asm 2: vmlal.s32 <h0=q0,<f57_2=d18,<g57_19=d28
vmlal.s32 q0,d18,d28

# qhasm:   h0[0,1] += f57_2[2] signed* g13_19[2];h0[2,3] += f57_2[3] signed* g13_19[3] 
# asm 1: vmlal.s32 <h0=reg128#1,<f57_2=reg128#10%top,<g13_19=reg128#5%top
# asm 2: vmlal.s32 <h0=q0,<f57_2=d19,<g13_19=d9
vmlal.s32 q0,d19,d9

# qhasm:   h2[0,1] += f57_2[0] signed* g57_19[2];h2[2,3] += f57_2[1] signed* g57_19[3] 
# asm 1: vmlal.s32 <h2=reg128#7,<f57_2=reg128#10%bot,<g57_19=reg128#15%top
# asm 2: vmlal.s32 <h2=q6,<f57_2=d18,<g57_19=d29
vmlal.s32 q6,d18,d29

# qhasm:   h2[0,1] += f57_2[2] signed* g57_19[0];h2[2,3] += f57_2[3] signed* g57_19[1] 
# asm 1: vmlal.s32 <h2=reg128#7,<f57_2=reg128#10%top,<g57_19=reg128#15%bot
# asm 2: vmlal.s32 <h2=q6,<f57_2=d19,<g57_19=d28
vmlal.s32 q6,d19,d28

# qhasm:   ptr = &f13_2_stack 
# asm 1: lea >ptr=int32#3,<f13_2_stack=stack128#6
# asm 2: lea >ptr=r2,<f13_2_stack=[sp,#592]
add r2,sp,#592

# qhasm:   f13_2 aligned= mem128[ptr] 
# asm 1: vld1.8 {>f13_2=reg128#10%bot->f13_2=reg128#10%top},[<ptr=int32#3,: 128]
# asm 2: vld1.8 {>f13_2=d18->f13_2=d19},[<ptr=r2,: 128]
vld1.8 {d18-d19},[r2,: 128]

# qhasm:   ptr = &_0x2000000_stack 
# asm 1: lea >ptr=int32#3,<_0x2000000_stack=stack128#1
# asm 2: lea >ptr=r2,<_0x2000000_stack=[sp,#512]
add r2,sp,#512

# qhasm:   _0x2000000 aligned= mem128[ptr] 
# asm 1: vld1.8 {>_0x2000000=reg128#12%bot->_0x2000000=reg128#12%top},[<ptr=int32#3,: 128]
# asm 2: vld1.8 {>_0x2000000=d22->_0x2000000=d23},[<ptr=r2,: 128]
vld1.8 {d22-d23},[r2,: 128]

# qhasm:   h6[0,1] += f13_2[2] signed* g13[2];h6[2,3] += f13_2[3] signed* g13[3] 
# asm 1: vmlal.s32 <h6=reg128#6,<f13_2=reg128#10%top,<g13=reg128#4%top
# asm 2: vmlal.s32 <h6=q5,<f13_2=d19,<g13=d7
vmlal.s32 q5,d19,d7

# qhasm:   h0[0,1] += f13_2[0] signed* g89_19[2];h0[2,3] += f13_2[1] signed* g89_19[3] 
# asm 1: vmlal.s32 <h0=reg128#1,<f13_2=reg128#10%bot,<g89_19=reg128#11%top
# asm 2: vmlal.s32 <h0=q0,<f13_2=d18,<g89_19=d21
vmlal.s32 q0,d18,d21

# qhasm:   h0[0,1] += f13_2[2] signed* g57_19[2];h0[2,3] += f13_2[3] signed* g57_19[3] 
# asm 1: vmlal.s32 <h0=reg128#1,<f13_2=reg128#10%top,<g57_19=reg128#15%top
# asm 2: vmlal.s32 <h0=q0,<f13_2=d19,<g57_19=d29
vmlal.s32 q0,d19,d29

# qhasm:   h2[0,1] += f13_2[0] signed* g13[0];h2[2,3] += f13_2[1] signed* g13[1] 
# asm 1: vmlal.s32 <h2=reg128#7,<f13_2=reg128#10%bot,<g13=reg128#4%bot
# asm 2: vmlal.s32 <h2=q6,<f13_2=d18,<g13=d6
vmlal.s32 q6,d18,d6

# qhasm:   ptr = &_0x1000000_stack 
# asm 1: lea >ptr=int32#3,<_0x1000000_stack=stack128#2
# asm 2: lea >ptr=r2,<_0x1000000_stack=[sp,#528]
add r2,sp,#528

# qhasm:   _0x1000000 aligned= mem128[ptr] 
# asm 1: vld1.8 {>_0x1000000=reg128#4%bot->_0x1000000=reg128#4%top},[<ptr=int32#3,: 128]
# asm 2: vld1.8 {>_0x1000000=d6->_0x1000000=d7},[<ptr=r2,: 128]
vld1.8 {d6-d7},[r2,: 128]

# qhasm:   h2[0,1] += f13_2[2] signed* g89_19[2];h2[2,3] += f13_2[3] signed* g89_19[3] 
# asm 1: vmlal.s32 <h2=reg128#7,<f13_2=reg128#10%top,<g89_19=reg128#11%top
# asm 2: vmlal.s32 <h2=q6,<f13_2=d19,<g89_19=d21
vmlal.s32 q6,d19,d21

# qhasm:   ptr = &h7_stack 
# asm 1: lea >ptr=int32#3,<h7_stack=stack128#5
# asm 2: lea >ptr=r2,<h7_stack=[sp,#576]
add r2,sp,#576

# qhasm:   h7 aligned= mem128[ptr] 
# asm 1: vld1.8 {>h7=reg128#10%bot->h7=reg128#10%top},[<ptr=int32#3,: 128]
# asm 2: vld1.8 {>h7=d18->h7=d19},[<ptr=r2,: 128]
vld1.8 {d18-d19},[r2,: 128]

# qhasm:   h0[0,1] += mix[0] signed* g13_19[0];h0[2,3] += mix[1] signed* g13_19[1] 
# asm 1: vmlal.s32 <h0=reg128#1,<mix=reg128#16%bot,<g13_19=reg128#5%bot
# asm 2: vmlal.s32 <h0=q0,<mix=d30,<g13_19=d8
vmlal.s32 q0,d30,d8

# qhasm:   ptr = &h9_stack 
# asm 1: lea >ptr=int32#3,<h9_stack=stack128#11
# asm 2: lea >ptr=r2,<h9_stack=[sp,#672]
add r2,sp,#672

# qhasm:   h9 aligned= mem128[ptr] 
# asm 1: vld1.8 {>h9=reg128#11%bot->h9=reg128#11%top},[<ptr=int32#3,: 128]
# asm 2: vld1.8 {>h9=d20->h9=d21},[<ptr=r2,: 128]
vld1.8 {d20-d21},[r2,: 128]

# qhasm:   h6[0,1] += mix[0] signed* g57_19[2];h6[2,3] += mix[1] signed* g57_19[3] 
# asm 1: vmlal.s32 <h6=reg128#6,<mix=reg128#16%bot,<g57_19=reg128#15%top
# asm 2: vmlal.s32 <h6=q5,<mix=d30,<g57_19=d29
vmlal.s32 q5,d30,d29

# qhasm:   ptr = &h5_stack 
# asm 1: lea >ptr=int32#3,<h5_stack=stack128#7
# asm 2: lea >ptr=r2,<h5_stack=[sp,#608]
add r2,sp,#608

# qhasm:   h5 aligned= mem128[ptr] 
# asm 1: vld1.8 {>h5=reg128#13%bot->h5=reg128#13%top},[<ptr=int32#3,: 128]
# asm 2: vld1.8 {>h5=d24->h5=d25},[<ptr=r2,: 128]
vld1.8 {d24-d25},[r2,: 128]

# qhasm:   h4[0,1] += mix[0] signed* g57_19[0];h4[2,3] += mix[1] signed* g57_19[1] 
# asm 1: vmlal.s32 <h4=reg128#2,<mix=reg128#16%bot,<g57_19=reg128#15%bot
# asm 2: vmlal.s32 <h4=q1,<mix=d30,<g57_19=d28
vmlal.s32 q1,d30,d28

# qhasm:   2x t0 = h0 + _0x2000000 
# asm 1: vadd.i64 >t0=reg128#14,<h0=reg128#1,<_0x2000000=reg128#12
# asm 2: vadd.i64 >t0=q13,<h0=q0,<_0x2000000=q11
vadd.i64 q13,q0,q11

# qhasm:   2x t6 = h6 + _0x2000000 
# asm 1: vadd.i64 >t6=reg128#15,<h6=reg128#6,<_0x2000000=reg128#12
# asm 2: vadd.i64 >t6=q14,<h6=q5,<_0x2000000=q11
vadd.i64 q14,q5,q11

# qhasm:   h2[0,1] += mix[0] signed* g13_19[2];h2[2,3] += mix[1] signed* g13_19[3] 
# asm 1: vmlal.s32 <h2=reg128#7,<mix=reg128#16%bot,<g13_19=reg128#5%top
# asm 2: vmlal.s32 <h2=q6,<mix=d30,<g13_19=d9
vmlal.s32 q6,d30,d9

# qhasm:   2x c0 = t0 signed>> 26 
# asm 1: vshr.s64 >c0=reg128#5,<t0=reg128#14,#26
# asm 2: vshr.s64 >c0=q4,<t0=q13,#26
vshr.s64 q4,q13,#26

# qhasm:   2x c6 = t6 signed>> 26 
# asm 1: vshr.s64 >c6=reg128#14,<t6=reg128#15,#26
# asm 2: vshr.s64 >c6=q13,<t6=q14,#26
vshr.s64 q13,q14,#26

# qhasm:   2x h1 += c0 
# asm 1: vadd.i64 >h1=reg128#8,<h1=reg128#8,<c0=reg128#5
# asm 2: vadd.i64 >h1=q7,<h1=q7,<c0=q4
vadd.i64 q7,q7,q4

# qhasm:   2x t0 = c0 << 26 
# asm 1: vshl.i64 >t0=reg128#5,<c0=reg128#5,#26
# asm 2: vshl.i64 >t0=q4,<c0=q4,#26
vshl.i64 q4,q4,#26

# qhasm:   2x t1 = h1 + _0x1000000 
# asm 1: vadd.i64 >t1=reg128#15,<h1=reg128#8,<_0x1000000=reg128#4
# asm 2: vadd.i64 >t1=q14,<h1=q7,<_0x1000000=q3
vadd.i64 q14,q7,q3

# qhasm:   2x h7 += c6 
# asm 1: vadd.i64 >h7=reg128#10,<h7=reg128#10,<c6=reg128#14
# asm 2: vadd.i64 >h7=q9,<h7=q9,<c6=q13
vadd.i64 q9,q9,q13

# qhasm:   2x t6 = c6 << 26 
# asm 1: vshl.i64 >t6=reg128#14,<c6=reg128#14,#26
# asm 2: vshl.i64 >t6=q13,<c6=q13,#26
vshl.i64 q13,q13,#26

# qhasm:   2x t7 = h7 + _0x1000000 
# asm 1: vadd.i64 >t7=reg128#16,<h7=reg128#10,<_0x1000000=reg128#4
# asm 2: vadd.i64 >t7=q15,<h7=q9,<_0x1000000=q3
vadd.i64 q15,q9,q3

# qhasm:   2x h0 -= t0 
# asm 1: vsub.i64 >h0=reg128#1,<h0=reg128#1,<t0=reg128#5
# asm 2: vsub.i64 >h0=q0,<h0=q0,<t0=q4
vsub.i64 q0,q0,q4

# qhasm:   2x c1 = t1 signed>> 25 
# asm 1: vshr.s64 >c1=reg128#5,<t1=reg128#15,#25
# asm 2: vshr.s64 >c1=q4,<t1=q14,#25
vshr.s64 q4,q14,#25

# qhasm:   2x h6 -= t6 
# asm 1: vsub.i64 >h6=reg128#6,<h6=reg128#6,<t6=reg128#14
# asm 2: vsub.i64 >h6=q5,<h6=q5,<t6=q13
vsub.i64 q5,q5,q13

# qhasm:   2x c7 = t7 signed>> 25 
# asm 1: vshr.s64 >c7=reg128#14,<t7=reg128#16,#25
# asm 2: vshr.s64 >c7=q13,<t7=q15,#25
vshr.s64 q13,q15,#25

# qhasm:   2x h2 += c1 
# asm 1: vadd.i64 >h2=reg128#7,<h2=reg128#7,<c1=reg128#5
# asm 2: vadd.i64 >h2=q6,<h2=q6,<c1=q4
vadd.i64 q6,q6,q4

# qhasm:   2x t1 = c1 << 25 
# asm 1: vshl.i64 >t1=reg128#5,<c1=reg128#5,#25
# asm 2: vshl.i64 >t1=q4,<c1=q4,#25
vshl.i64 q4,q4,#25

# qhasm:   2x t2 = h2 + _0x2000000 
# asm 1: vadd.i64 >t2=reg128#15,<h2=reg128#7,<_0x2000000=reg128#12
# asm 2: vadd.i64 >t2=q14,<h2=q6,<_0x2000000=q11
vadd.i64 q14,q6,q11

# qhasm:   2x h8 += c7 
# asm 1: vadd.i64 >h8=reg128#3,<h8=reg128#3,<c7=reg128#14
# asm 2: vadd.i64 >h8=q2,<h8=q2,<c7=q13
vadd.i64 q2,q2,q13

# qhasm:   2x h1 -= t1 
# asm 1: vsub.i64 >h1=reg128#5,<h1=reg128#8,<t1=reg128#5
# asm 2: vsub.i64 >h1=q4,<h1=q7,<t1=q4
vsub.i64 q4,q7,q4

# qhasm:   2x c2 = t2 signed>> 26 
# asm 1: vshr.s64 >c2=reg128#8,<t2=reg128#15,#26
# asm 2: vshr.s64 >c2=q7,<t2=q14,#26
vshr.s64 q7,q14,#26

# qhasm:   2x t7 = c7 << 25 
# asm 1: vshl.i64 >t7=reg128#14,<c7=reg128#14,#25
# asm 2: vshl.i64 >t7=q13,<c7=q13,#25
vshl.i64 q13,q13,#25

# qhasm:   2x t8 = h8 + _0x2000000 
# asm 1: vadd.i64 >t8=reg128#15,<h8=reg128#3,<_0x2000000=reg128#12
# asm 2: vadd.i64 >t8=q14,<h8=q2,<_0x2000000=q11
vadd.i64 q14,q2,q11

# qhasm:   2x h3 += c2 
# asm 1: vadd.i64 >h3=reg128#9,<h3=reg128#9,<c2=reg128#8
# asm 2: vadd.i64 >h3=q8,<h3=q8,<c2=q7
vadd.i64 q8,q8,q7

# qhasm:   2x t2 = c2 << 26 
# asm 1: vshl.i64 >t2=reg128#8,<c2=reg128#8,#26
# asm 2: vshl.i64 >t2=q7,<c2=q7,#26
vshl.i64 q7,q7,#26

# qhasm:   2x t3 = h3 + _0x1000000 
# asm 1: vadd.i64 >t3=reg128#16,<h3=reg128#9,<_0x1000000=reg128#4
# asm 2: vadd.i64 >t3=q15,<h3=q8,<_0x1000000=q3
vadd.i64 q15,q8,q3

# qhasm:   2x h7 -= t7 
# asm 1: vsub.i64 >h7=reg128#10,<h7=reg128#10,<t7=reg128#14
# asm 2: vsub.i64 >h7=q9,<h7=q9,<t7=q13
vsub.i64 q9,q9,q13

# qhasm:   2x c8 = t8 signed>> 26 
# asm 1: vshr.s64 >c8=reg128#14,<t8=reg128#15,#26
# asm 2: vshr.s64 >c8=q13,<t8=q14,#26
vshr.s64 q13,q14,#26

# qhasm:   2x h2 -= t2 
# asm 1: vsub.i64 >h2=reg128#7,<h2=reg128#7,<t2=reg128#8
# asm 2: vsub.i64 >h2=q6,<h2=q6,<t2=q7
vsub.i64 q6,q6,q7

# qhasm:   2x c3 = t3 signed>> 25 
# asm 1: vshr.s64 >c3=reg128#8,<t3=reg128#16,#25
# asm 2: vshr.s64 >c3=q7,<t3=q15,#25
vshr.s64 q7,q15,#25

# qhasm:   2x h9 += c8 
# asm 1: vadd.i64 >h9=reg128#11,<h9=reg128#11,<c8=reg128#14
# asm 2: vadd.i64 >h9=q10,<h9=q10,<c8=q13
vadd.i64 q10,q10,q13

# qhasm:   2x t8 = c8 << 26 
# asm 1: vshl.i64 >t8=reg128#14,<c8=reg128#14,#26
# asm 2: vshl.i64 >t8=q13,<c8=q13,#26
vshl.i64 q13,q13,#26

# qhasm:   2x t9 = h9 + _0x1000000 
# asm 1: vadd.i64 >t9=reg128#15,<h9=reg128#11,<_0x1000000=reg128#4
# asm 2: vadd.i64 >t9=q14,<h9=q10,<_0x1000000=q3
vadd.i64 q14,q10,q3

# qhasm:   2x h4 += c3 
# asm 1: vadd.i64 >h4=reg128#2,<h4=reg128#2,<c3=reg128#8
# asm 2: vadd.i64 >h4=q1,<h4=q1,<c3=q7
vadd.i64 q1,q1,q7

# qhasm:   posh = playground1_ptr + 288 
# asm 1: add >posh=int32#3,<playground1_ptr=int32#4,#288
# asm 2: add >posh=r2,<playground1_ptr=r3,#288
add r2,r3,#288

# qhasm:   2x t3 = c3 << 25 
# asm 1: vshl.i64 >t3=reg128#8,<c3=reg128#8,#25
# asm 2: vshl.i64 >t3=q7,<c3=q7,#25
vshl.i64 q7,q7,#25

# qhasm:   posH = playground1_ptr + 96 
# asm 1: add >posH=int32#5,<playground1_ptr=int32#4,#96
# asm 2: add >posH=r4,<playground1_ptr=r3,#96
add r4,r3,#96

# qhasm:   2x t4 = h4 + _0x2000000 
# asm 1: vadd.i64 >t4=reg128#16,<h4=reg128#2,<_0x2000000=reg128#12
# asm 2: vadd.i64 >t4=q15,<h4=q1,<_0x2000000=q11
vadd.i64 q15,q1,q11

# qhasm:   posh+=8 
# asm 1: add >posh=int32#3,<posh=int32#3,#8
# asm 2: add >posh=r2,<posh=r2,#8
add r2,r2,#8

# qhasm:   2x h8 -= t8 
# asm 1: vsub.i64 >h8=reg128#3,<h8=reg128#3,<t8=reg128#14
# asm 2: vsub.i64 >h8=q2,<h8=q2,<t8=q13
vsub.i64 q2,q2,q13

# qhasm:   posH+=8 
# asm 1: add >posH=int32#5,<posH=int32#5,#8
# asm 2: add >posH=r4,<posH=r4,#8
add r4,r4,#8

# qhasm:   2x c9 = t9 signed>> 25 
# asm 1: vshr.s64 >c9=reg128#14,<t9=reg128#15,#25
# asm 2: vshr.s64 >c9=q13,<t9=q14,#25
vshr.s64 q13,q14,#25

# qhasm:   2x h3 -= t3 
# asm 1: vsub.i64 >h3=reg128#8,<h3=reg128#9,<t3=reg128#8
# asm 2: vsub.i64 >h3=q7,<h3=q8,<t3=q7
vsub.i64 q7,q8,q7

# qhasm:   2x c4 = t4 signed>> 26 
# asm 1: vshr.s64 >c4=reg128#9,<t4=reg128#16,#26
# asm 2: vshr.s64 >c4=q8,<t4=q15,#26
vshr.s64 q8,q15,#26

# qhasm:   2x s = c9 + c9 
# asm 1: vadd.i64 >s=reg128#15,<c9=reg128#14,<c9=reg128#14
# asm 2: vadd.i64 >s=q14,<c9=q13,<c9=q13
vadd.i64 q14,q13,q13

# qhasm:   2x h5 += c4 
# asm 1: vadd.i64 >h5=reg128#13,<h5=reg128#13,<c4=reg128#9
# asm 2: vadd.i64 >h5=q12,<h5=q12,<c4=q8
vadd.i64 q12,q12,q8

# qhasm:   h2 h3 = h2[0]h3[0]h2[2]h2[3] h2[1]h3[1]h3[2]h3[3] 
# asm 1: vtrn.32 <h2=reg128#7%bot,<h3=reg128#8%bot
# asm 2: vtrn.32 <h2=d12,<h3=d14
vtrn.32 d12,d14

# qhasm:   2x t4 = c4 << 26 
# asm 1: vshl.i64 >t4=reg128#9,<c4=reg128#9,#26
# asm 2: vshl.i64 >t4=q8,<c4=q8,#26
vshl.i64 q8,q8,#26

# qhasm:   h2 h3 = h2[0]h2[1]h2[2]h3[2] h3[0]h3[1]h2[3]h3[3] 
# asm 1: vtrn.32 <h2=reg128#7%top,<h3=reg128#8%top
# asm 2: vtrn.32 <h2=d13,<h3=d15
vtrn.32 d13,d15

# qhasm:   2x t5 = h5 + _0x1000000 
# asm 1: vadd.i64 >t5=reg128#4,<h5=reg128#13,<_0x1000000=reg128#4
# asm 2: vadd.i64 >t5=q3,<h5=q12,<_0x1000000=q3
vadd.i64 q3,q12,q3

# qhasm:   2x h0 += s 
# asm 1: vadd.i64 >h0=reg128#1,<h0=reg128#1,<s=reg128#15
# asm 2: vadd.i64 >h0=q0,<h0=q0,<s=q14
vadd.i64 q0,q0,q14

# qhasm:   mem64[posh] aligned= h2[0];posh+=8 
# asm 1: vst1.8 <h2=reg128#7%bot,[<posh=int32#3,: 64]!
# asm 2: vst1.8 <h2=d12,[<posh=r2,: 64]!
vst1.8 d12,[r2,: 64]!

# qhasm:   2x s = c9 << 4 
# asm 1: vshl.i64 >s=reg128#8,<c9=reg128#14,#4
# asm 2: vshl.i64 >s=q7,<c9=q13,#4
vshl.i64 q7,q13,#4

# qhasm:   mem64[posH] aligned= h2[1];posH+=8 
# asm 1: vst1.8 <h2=reg128#7%top,[<posH=int32#5,: 64]!
# asm 2: vst1.8 <h2=d13,[<posH=r4,: 64]!
vst1.8 d13,[r4,: 64]!

# qhasm:   2x h4 -= t4 
# asm 1: vsub.i64 >h4=reg128#2,<h4=reg128#2,<t4=reg128#9
# asm 2: vsub.i64 >h4=q1,<h4=q1,<t4=q8
vsub.i64 q1,q1,q8

# qhasm:   2x c5 = t5 signed>> 25 
# asm 1: vshr.s64 >c5=reg128#4,<t5=reg128#4,#25
# asm 2: vshr.s64 >c5=q3,<t5=q3,#25
vshr.s64 q3,q3,#25

# qhasm:   2x h0 += s 
# asm 1: vadd.i64 >h0=reg128#1,<h0=reg128#1,<s=reg128#8
# asm 2: vadd.i64 >h0=q0,<h0=q0,<s=q7
vadd.i64 q0,q0,q7

# qhasm:   2x h6 += c5 
# asm 1: vadd.i64 >h6=reg128#6,<h6=reg128#6,<c5=reg128#4
# asm 2: vadd.i64 >h6=q5,<h6=q5,<c5=q3
vadd.i64 q5,q5,q3

# qhasm:   2x t5 = c5 << 25 
# asm 1: vshl.i64 >t5=reg128#4,<c5=reg128#4,#25
# asm 2: vshl.i64 >t5=q3,<c5=q3,#25
vshl.i64 q3,q3,#25

# qhasm:   2x t6 = h6 + _0x2000000 
# asm 1: vadd.i64 >t6=reg128#7,<h6=reg128#6,<_0x2000000=reg128#12
# asm 2: vadd.i64 >t6=q6,<h6=q5,<_0x2000000=q11
vadd.i64 q6,q5,q11

# qhasm:   2x h0 += c9 
# asm 1: vadd.i64 >h0=reg128#1,<h0=reg128#1,<c9=reg128#14
# asm 2: vadd.i64 >h0=q0,<h0=q0,<c9=q13
vadd.i64 q0,q0,q13

# qhasm:   2x t9 = c9 << 25 
# asm 1: vshl.i64 >t9=reg128#8,<c9=reg128#14,#25
# asm 2: vshl.i64 >t9=q7,<c9=q13,#25
vshl.i64 q7,q13,#25

# qhasm:   2x t0 = h0 + _0x2000000 
# asm 1: vadd.i64 >t0=reg128#9,<h0=reg128#1,<_0x2000000=reg128#12
# asm 2: vadd.i64 >t0=q8,<h0=q0,<_0x2000000=q11
vadd.i64 q8,q0,q11

# qhasm:   2x h5 -= t5 
# asm 1: vsub.i64 >h5=reg128#4,<h5=reg128#13,<t5=reg128#4
# asm 2: vsub.i64 >h5=q3,<h5=q12,<t5=q3
vsub.i64 q3,q12,q3

# qhasm:   2x c6 = t6 signed>> 26 
# asm 1: vshr.s64 >c6=reg128#7,<t6=reg128#7,#26
# asm 2: vshr.s64 >c6=q6,<t6=q6,#26
vshr.s64 q6,q6,#26

# qhasm:   2x h9 -= t9 
# asm 1: vsub.i64 >h9=reg128#8,<h9=reg128#11,<t9=reg128#8
# asm 2: vsub.i64 >h9=q7,<h9=q10,<t9=q7
vsub.i64 q7,q10,q7

# qhasm:   h4 h5 = h4[0]h5[0]h4[2]h4[3] h4[1]h5[1]h5[2]h5[3] 
# asm 1: vtrn.32 <h4=reg128#2%bot,<h5=reg128#4%bot
# asm 2: vtrn.32 <h4=d2,<h5=d6
vtrn.32 d2,d6

# qhasm:   2x c0 = t0 signed>> 26 
# asm 1: vshr.s64 >c0=reg128#9,<t0=reg128#9,#26
# asm 2: vshr.s64 >c0=q8,<t0=q8,#26
vshr.s64 q8,q8,#26

# qhasm:   h4 h5 = h4[0]h4[1]h4[2]h5[2] h5[0]h5[1]h4[3]h5[3] 
# asm 1: vtrn.32 <h4=reg128#2%top,<h5=reg128#4%top
# asm 2: vtrn.32 <h4=d3,<h5=d7
vtrn.32 d3,d7

# qhasm:   2x h7 += c6 
# asm 1: vadd.i64 >h7=reg128#4,<h7=reg128#10,<c6=reg128#7
# asm 2: vadd.i64 >h7=q3,<h7=q9,<c6=q6
vadd.i64 q3,q9,q6

# qhasm:   mem64[posh] aligned= h4[0] 
# asm 1: vst1.8 <h4=reg128#2%bot,[<posh=int32#3,: 64]
# asm 2: vst1.8 <h4=d2,[<posh=r2,: 64]
vst1.8 d2,[r2,: 64]

# qhasm:   2x t6 = c6 << 26 
# asm 1: vshl.i64 >t6=reg128#7,<c6=reg128#7,#26
# asm 2: vshl.i64 >t6=q6,<c6=q6,#26
vshl.i64 q6,q6,#26

# qhasm:   mem64[posH] aligned= h4[1] 
# asm 1: vst1.8 <h4=reg128#2%top,[<posH=int32#5,: 64]
# asm 2: vst1.8 <h4=d3,[<posH=r4,: 64]
vst1.8 d3,[r4,: 64]

# qhasm:   2x h1 += c0 
# asm 1: vadd.i64 >h1=reg128#2,<h1=reg128#5,<c0=reg128#9
# asm 2: vadd.i64 >h1=q1,<h1=q4,<c0=q8
vadd.i64 q1,q4,q8

# qhasm:   h8 h9 = h8[0]h9[0]h8[2]h8[3] h8[1]h9[1]h9[2]h9[3] 
# asm 1: vtrn.32 <h8=reg128#3%bot,<h9=reg128#8%bot
# asm 2: vtrn.32 <h8=d4,<h9=d14
vtrn.32 d4,d14

# qhasm:   2x t0 = c0 << 26 
# asm 1: vshl.i64 >t0=reg128#5,<c0=reg128#9,#26
# asm 2: vshl.i64 >t0=q4,<c0=q8,#26
vshl.i64 q4,q8,#26

# qhasm:   h8 h9 = h8[0]h8[1]h8[2]h9[2] h9[0]h9[1]h8[3]h9[3] 
# asm 1: vtrn.32 <h8=reg128#3%top,<h9=reg128#8%top
# asm 2: vtrn.32 <h8=d5,<h9=d15
vtrn.32 d5,d15

# qhasm:   2x h6 -= t6 
# asm 1: vsub.i64 >h6=reg128#6,<h6=reg128#6,<t6=reg128#7
# asm 2: vsub.i64 >h6=q5,<h6=q5,<t6=q6
vsub.i64 q5,q5,q6

# qhasm:   posh+=16 
# asm 1: add >posh=int32#3,<posh=int32#3,#16
# asm 2: add >posh=r2,<posh=r2,#16
add r2,r2,#16

# qhasm:   2x h0 -= t0 
# asm 1: vsub.i64 >h0=reg128#1,<h0=reg128#1,<t0=reg128#5
# asm 2: vsub.i64 >h0=q0,<h0=q0,<t0=q4
vsub.i64 q0,q0,q4

# qhasm:   mem64[posh] aligned= h8[0] 
# asm 1: vst1.8 <h8=reg128#3%bot,[<posh=int32#3,: 64]
# asm 2: vst1.8 <h8=d4,[<posh=r2,: 64]
vst1.8 d4,[r2,: 64]

# qhasm:   posH+=16 
# asm 1: add >posH=int32#5,<posH=int32#5,#16
# asm 2: add >posH=r4,<posH=r4,#16
add r4,r4,#16

# qhasm:   mem64[posH] aligned= h8[1] 
# asm 1: vst1.8 <h8=reg128#3%top,[<posH=int32#5,: 64]
# asm 2: vst1.8 <h8=d5,[<posH=r4,: 64]
vst1.8 d5,[r4,: 64]

# qhasm:   h6 h7 = h6[0]h7[0]h6[2]h6[3] h6[1]h7[1]h7[2]h7[3] 
# asm 1: vtrn.32 <h6=reg128#6%bot,<h7=reg128#4%bot
# asm 2: vtrn.32 <h6=d10,<h7=d6
vtrn.32 d10,d6

# qhasm:   h6 h7 = h6[0]h6[1]h6[2]h7[2] h7[0]h7[1]h6[3]h7[3] 
# asm 1: vtrn.32 <h6=reg128#6%top,<h7=reg128#4%top
# asm 2: vtrn.32 <h6=d11,<h7=d7
vtrn.32 d11,d7

# qhasm:   posh-=8 
# asm 1: sub >posh=int32#3,<posh=int32#3,#8
# asm 2: sub >posh=r2,<posh=r2,#8
sub r2,r2,#8

# qhasm:   posH-=8 
# asm 1: sub >posH=int32#5,<posH=int32#5,#8
# asm 2: sub >posH=r4,<posH=r4,#8
sub r4,r4,#8

# qhasm:   h0 h1 = h0[0]h1[0]h0[2]h0[3] h0[1]h1[1]h1[2]h1[3] 
# asm 1: vtrn.32 <h0=reg128#1%bot,<h1=reg128#2%bot
# asm 2: vtrn.32 <h0=d0,<h1=d2
vtrn.32 d0,d2

# qhasm:   h0 h1 = h0[0]h0[1]h0[2]h1[2] h1[0]h1[1]h0[3]h1[3] 
# asm 1: vtrn.32 <h0=reg128#1%top,<h1=reg128#2%top
# asm 2: vtrn.32 <h0=d1,<h1=d3
vtrn.32 d1,d3

# qhasm:   mem64[posh] aligned= h6[0] 
# asm 1: vst1.8 <h6=reg128#6%bot,[<posh=int32#3,: 64]
# asm 2: vst1.8 <h6=d10,[<posh=r2,: 64]
vst1.8 d10,[r2,: 64]

# qhasm:   mem64[posH] aligned= h6[1] 
# asm 1: vst1.8 <h6=reg128#6%top,[<posH=int32#5,: 64]
# asm 2: vst1.8 <h6=d11,[<posH=r4,: 64]
vst1.8 d11,[r4,: 64]

# qhasm:   posh-=24 
# asm 1: sub >posh=int32#3,<posh=int32#3,#24
# asm 2: sub >posh=r2,<posh=r2,#24
sub r2,r2,#24

# qhasm:   posH-=24 
# asm 1: sub >posH=int32#5,<posH=int32#5,#24
# asm 2: sub >posH=r4,<posH=r4,#24
sub r4,r4,#24

# qhasm:   mem64[posh] aligned= h0[0] 
# asm 1: vst1.8 <h0=reg128#1%bot,[<posh=int32#3,: 64]
# asm 2: vst1.8 <h0=d0,[<posh=r2,: 64]
vst1.8 d0,[r2,: 64]

# qhasm:   mem64[posH] aligned= h0[1] 
# asm 1: vst1.8 <h0=reg128#1%top,[<posH=int32#5,: 64]
# asm 2: vst1.8 <h0=d1,[<posH=r4,: 64]
vst1.8 d1,[r4,: 64]

# qhasm:   ptr = &_19_19_38_38_stack 
# asm 1: lea >ptr=int32#3,<_19_19_38_38_stack=stack128#3
# asm 2: lea >ptr=r2,<_19_19_38_38_stack=[sp,#544]
add r2,sp,#544

# qhasm:   posf = playground1_ptr + 144 
# asm 1: add >posf=int32#5,<playground1_ptr=int32#4,#144
# asm 2: add >posf=r4,<playground1_ptr=r3,#144
add r4,r3,#144

# qhasm:   posF = playground1_ptr + 192 
# asm 1: add >posF=int32#6,<playground1_ptr=int32#4,#192
# asm 2: add >posF=r5,<playground1_ptr=r3,#192
add r5,r3,#192

# qhasm:   _19_19_38_38 aligned= mem128[ptr] 
# asm 1: vld1.8 {>_19_19_38_38=reg128#1%bot->_19_19_38_38=reg128#1%top},[<ptr=int32#3,: 128]
# asm 2: vld1.8 {>_19_19_38_38=d0->_19_19_38_38=d1},[<ptr=r2,: 128]
vld1.8 {d0-d1},[r2,: 128]

# qhasm:   fg01 aligned= mem128[posf];posf+=16 
# asm 1: vld1.8 {>fg01=reg128#2%bot->fg01=reg128#2%top},[<posf=int32#5,: 128]!
# asm 2: vld1.8 {>fg01=d2->fg01=d3},[<posf=r4,: 128]!
vld1.8 {d2-d3},[r4,: 128]!

# qhasm:   fg23 aligned= mem128[posF];posF+=16 
# asm 1: vld1.8 {>fg23=reg128#3%bot->fg23=reg128#3%top},[<posF=int32#6,: 128]!
# asm 2: vld1.8 {>fg23=d4->fg23=d5},[<posF=r5,: 128]!
vld1.8 {d4-d5},[r5,: 128]!

# qhasm:   fg01[0,1,2,3] fg23[0,1,2,3] = fg01[0]fg23[0]fg01[1]fg23[1] fg01[2]fg23[2]fg01[3]fg23[3] 
# asm 1: vzip.i32 <fg01=reg128#2,<fg23=reg128#3
# asm 2: vzip.i32 <fg01=q1,<fg23=q2
vzip.i32 q1,q2

# qhasm:   fg45 aligned= mem128[posf];posf+=16 
# asm 1: vld1.8 {>fg45=reg128#4%bot->fg45=reg128#4%top},[<posf=int32#5,: 128]!
# asm 2: vld1.8 {>fg45=d6->fg45=d7},[<posf=r4,: 128]!
vld1.8 {d6-d7},[r4,: 128]!

# qhasm:   fg67 aligned= mem128[posF];posF+=16 
# asm 1: vld1.8 {>fg67=reg128#5%bot->fg67=reg128#5%top},[<posF=int32#6,: 128]!
# asm 2: vld1.8 {>fg67=d8->fg67=d9},[<posF=r5,: 128]!
vld1.8 {d8-d9},[r5,: 128]!

# qhasm:   4x fg01_2 = fg01 << 1 
# asm 1: vshl.i32 >fg01_2=reg128#6,<fg01=reg128#2,#1
# asm 2: vshl.i32 >fg01_2=q5,<fg01=q1,#1
vshl.i32 q5,q1,#1

# qhasm:   fg45[0,1,2,3] fg67[0,1,2,3] = fg45[0]fg67[0]fg45[1]fg67[1] fg45[2]fg67[2]fg45[3]fg67[3] 
# asm 1: vzip.i32 <fg45=reg128#4,<fg67=reg128#5
# asm 2: vzip.i32 <fg45=q3,<fg67=q4
vzip.i32 q3,q4

# qhasm:   4x fg23_2 = fg23 << 1 
# asm 1: vshl.i32 >fg23_2=reg128#7,<fg23=reg128#3,#1
# asm 2: vshl.i32 >fg23_2=q6,<fg23=q2,#1
vshl.i32 q6,q2,#1

# qhasm:   new fg89 

# qhasm:   fg89 aligned= mem64[posf]fg89[1] 
# asm 1: vld1.8 {<fg89=reg128#8%bot},[<posf=int32#5,: 64]
# asm 2: vld1.8 {<fg89=d14},[<posf=r4,: 64]
vld1.8 {d14},[r4,: 64]

# qhasm:   4x fg45_2 = fg45 << 1 
# asm 1: vshl.i32 >fg45_2=reg128#9,<fg45=reg128#4,#1
# asm 2: vshl.i32 >fg45_2=q8,<fg45=q3,#1
vshl.i32 q8,q3,#1

# qhasm:   fg89 aligned= fg89[0]mem64[posF] 
# asm 1: vld1.8 {<fg89=reg128#8%top},[<posF=int32#6,: 64]
# asm 2: vld1.8 {<fg89=d15},[<posF=r5,: 64]
vld1.8 {d15},[r5,: 64]

# qhasm:   4x fg67_2 = fg67 << 1 
# asm 1: vshl.i32 >fg67_2=reg128#10,<fg67=reg128#5,#1
# asm 2: vshl.i32 >fg67_2=q9,<fg67=q4,#1
vshl.i32 q9,q4,#1

# qhasm:   fg45_19_38[0,1] = fg45_19_38[0,1];fg45_19_38[2] = fg45[2] * _19_19_38_38[2];fg45_19_38[3] = fg45[3] * _19_19_38_38[3] 
# asm 1: vmul.i32 >fg45_19_38=reg128#11%top,<fg45=reg128#4%top,<_19_19_38_38=reg128#1%top
# asm 2: vmul.i32 >fg45_19_38=d21,<fg45=d7,<_19_19_38_38=d1
vmul.i32 d21,d7,d1

# qhasm:   fg89 = fg89[0,2,1,3] 
# asm 1: vtrn.32 <fg89=reg128#8%bot,<fg89=reg128#8%top
# asm 2: vtrn.32 <fg89=d14,<fg89=d15
vtrn.32 d14,d15

# qhasm:   4x fg67_19_38 = fg67 * _19_19_38_38 
# asm 1: vmul.i32 >fg67_19_38=reg128#12,<fg67=reg128#5,<_19_19_38_38=reg128#1
# asm 2: vmul.i32 >fg67_19_38=q11,<fg67=q4,<_19_19_38_38=q0
vmul.i32 q11,q4,q0

# qhasm:   4x fg89_19_38 = fg89 * _19_19_38_38 
# asm 1: vmul.i32 >fg89_19_38=reg128#1,<fg89=reg128#8,<_19_19_38_38=reg128#1
# asm 2: vmul.i32 >fg89_19_38=q0,<fg89=q7,<_19_19_38_38=q0
vmul.i32 q0,q7,q0

# qhasm:   h0[0,1] = fg01[0] signed* fg01[0];h0[2,3] = fg01[1] signed* fg01[1] 
# asm 1: vmull.s32 >h0=reg128#13,<fg01=reg128#2%bot,<fg01=reg128#2%bot
# asm 2: vmull.s32 >h0=q12,<fg01=d2,<fg01=d2
vmull.s32 q12,d2,d2

# qhasm:   h0[0,1] += fg01_2[2] signed* fg89_19_38[2];h0[2,3] += fg01_2[3] signed* fg89_19_38[3] 
# asm 1: vmlal.s32 <h0=reg128#13,<fg01_2=reg128#6%top,<fg89_19_38=reg128#1%top
# asm 2: vmlal.s32 <h0=q12,<fg01_2=d11,<fg89_19_38=d1
vmlal.s32 q12,d11,d1

# qhasm:   h0[0,1] += fg23_2[0] signed* fg89_19_38[0];h0[2,3] += fg23_2[1] signed* fg89_19_38[1] 
# asm 1: vmlal.s32 <h0=reg128#13,<fg23_2=reg128#7%bot,<fg89_19_38=reg128#1%bot
# asm 2: vmlal.s32 <h0=q12,<fg23_2=d12,<fg89_19_38=d0
vmlal.s32 q12,d12,d0

# qhasm:   h0[0,1] += fg23_2[2] signed* fg67_19_38[2];h0[2,3] += fg23_2[3] signed* fg67_19_38[3] 
# asm 1: vmlal.s32 <h0=reg128#13,<fg23_2=reg128#7%top,<fg67_19_38=reg128#12%top
# asm 2: vmlal.s32 <h0=q12,<fg23_2=d13,<fg67_19_38=d23
vmlal.s32 q12,d13,d23

# qhasm:   h0[0,1] += fg45_2[0] signed* fg67_19_38[0];h0[2,3] += fg45_2[1] signed* fg67_19_38[1] 
# asm 1: vmlal.s32 <h0=reg128#13,<fg45_2=reg128#9%bot,<fg67_19_38=reg128#12%bot
# asm 2: vmlal.s32 <h0=q12,<fg45_2=d16,<fg67_19_38=d22
vmlal.s32 q12,d16,d22

# qhasm:   h0[0,1] += fg45[2] signed* fg45_19_38[2];h0[2,3] += fg45[3] signed* fg45_19_38[3] 
# asm 1: vmlal.s32 <h0=reg128#13,<fg45=reg128#4%top,<fg45_19_38=reg128#11%top
# asm 2: vmlal.s32 <h0=q12,<fg45=d7,<fg45_19_38=d21
vmlal.s32 q12,d7,d21

# qhasm:   h1[0,1] = fg01[0] signed* fg01_2[2];h1[2,3] = fg01[1] signed* fg01_2[3] 
# asm 1: vmull.s32 >h1=reg128#11,<fg01=reg128#2%bot,<fg01_2=reg128#6%top
# asm 2: vmull.s32 >h1=q10,<fg01=d2,<fg01_2=d11
vmull.s32 q10,d2,d11

# qhasm:   h1[0,1] += fg23[0] signed* fg89_19_38[2];h1[2,3] += fg23[1] signed* fg89_19_38[3] 
# asm 1: vmlal.s32 <h1=reg128#11,<fg23=reg128#3%bot,<fg89_19_38=reg128#1%top
# asm 2: vmlal.s32 <h1=q10,<fg23=d4,<fg89_19_38=d1
vmlal.s32 q10,d4,d1

# qhasm:   h1[0,1] += fg23_2[2] signed* fg89_19_38[0];h1[2,3] += fg23_2[3] signed* fg89_19_38[1] 
# asm 1: vmlal.s32 <h1=reg128#11,<fg23_2=reg128#7%top,<fg89_19_38=reg128#1%bot
# asm 2: vmlal.s32 <h1=q10,<fg23_2=d13,<fg89_19_38=d0
vmlal.s32 q10,d13,d0

# qhasm:   h1[0,1] += fg45[0] signed* fg67_19_38[2];h1[2,3] += fg45[1] signed* fg67_19_38[3] 
# asm 1: vmlal.s32 <h1=reg128#11,<fg45=reg128#4%bot,<fg67_19_38=reg128#12%top
# asm 2: vmlal.s32 <h1=q10,<fg45=d6,<fg67_19_38=d23
vmlal.s32 q10,d6,d23

# qhasm:   h1[0,1] += fg45_2[2] signed* fg67_19_38[0];h1[2,3] += fg45_2[3] signed* fg67_19_38[1] 
# asm 1: vmlal.s32 <h1=reg128#11,<fg45_2=reg128#9%top,<fg67_19_38=reg128#12%bot
# asm 2: vmlal.s32 <h1=q10,<fg45_2=d17,<fg67_19_38=d22
vmlal.s32 q10,d17,d22

# qhasm:   h2[0,1] = fg01_2[0] signed* fg23[0];h2[2,3] = fg01_2[1] signed* fg23[1] 
# asm 1: vmull.s32 >h2=reg128#14,<fg01_2=reg128#6%bot,<fg23=reg128#3%bot
# asm 2: vmull.s32 >h2=q13,<fg01_2=d10,<fg23=d4
vmull.s32 q13,d10,d4

# qhasm:   h2[0,1] += fg01_2[2] signed* fg01[2];h2[2,3] += fg01_2[3] signed* fg01[3] 
# asm 1: vmlal.s32 <h2=reg128#14,<fg01_2=reg128#6%top,<fg01=reg128#2%top
# asm 2: vmlal.s32 <h2=q13,<fg01_2=d11,<fg01=d3
vmlal.s32 q13,d11,d3

# qhasm:   h2[0,1] += fg23_2[2] signed* fg89_19_38[2];h2[2,3] += fg23_2[3] signed* fg89_19_38[3] 
# asm 1: vmlal.s32 <h2=reg128#14,<fg23_2=reg128#7%top,<fg89_19_38=reg128#1%top
# asm 2: vmlal.s32 <h2=q13,<fg23_2=d13,<fg89_19_38=d1
vmlal.s32 q13,d13,d1

# qhasm:   h2[0,1] += fg45_2[0] signed* fg89_19_38[0];h2[2,3] += fg45_2[1] signed* fg89_19_38[1] 
# asm 1: vmlal.s32 <h2=reg128#14,<fg45_2=reg128#9%bot,<fg89_19_38=reg128#1%bot
# asm 2: vmlal.s32 <h2=q13,<fg45_2=d16,<fg89_19_38=d0
vmlal.s32 q13,d16,d0

# qhasm:   h2[0,1] += fg45_2[2] signed* fg67_19_38[2];h2[2,3] += fg45_2[3] signed* fg67_19_38[3] 
# asm 1: vmlal.s32 <h2=reg128#14,<fg45_2=reg128#9%top,<fg67_19_38=reg128#12%top
# asm 2: vmlal.s32 <h2=q13,<fg45_2=d17,<fg67_19_38=d23
vmlal.s32 q13,d17,d23

# qhasm:   h2[0,1] += fg67[0] signed* fg67_19_38[0];h2[2,3] += fg67[1] signed* fg67_19_38[1] 
# asm 1: vmlal.s32 <h2=reg128#14,<fg67=reg128#5%bot,<fg67_19_38=reg128#12%bot
# asm 2: vmlal.s32 <h2=q13,<fg67=d8,<fg67_19_38=d22
vmlal.s32 q13,d8,d22

# qhasm:   h3[0,1] = fg01_2[0] signed* fg23[2];h3[2,3] = fg01_2[1] signed* fg23[3] 
# asm 1: vmull.s32 >h3=reg128#2,<fg01_2=reg128#6%bot,<fg23=reg128#3%top
# asm 2: vmull.s32 >h3=q1,<fg01_2=d10,<fg23=d5
vmull.s32 q1,d10,d5

# qhasm:   h3[0,1] += fg01_2[2] signed* fg23[0];h3[2,3] += fg01_2[3] signed* fg23[1] 
# asm 1: vmlal.s32 <h3=reg128#2,<fg01_2=reg128#6%top,<fg23=reg128#3%bot
# asm 2: vmlal.s32 <h3=q1,<fg01_2=d11,<fg23=d4
vmlal.s32 q1,d11,d4

# qhasm:   h3[0,1] += fg45[0] signed* fg89_19_38[2];h3[2,3] += fg45[1] signed* fg89_19_38[3] 
# asm 1: vmlal.s32 <h3=reg128#2,<fg45=reg128#4%bot,<fg89_19_38=reg128#1%top
# asm 2: vmlal.s32 <h3=q1,<fg45=d6,<fg89_19_38=d1
vmlal.s32 q1,d6,d1

# qhasm:   h3[0,1] += fg45_2[2] signed* fg89_19_38[0];h3[2,3] += fg45_2[3] signed* fg89_19_38[1] 
# asm 1: vmlal.s32 <h3=reg128#2,<fg45_2=reg128#9%top,<fg89_19_38=reg128#1%bot
# asm 2: vmlal.s32 <h3=q1,<fg45_2=d17,<fg89_19_38=d0
vmlal.s32 q1,d17,d0

# qhasm:   h3[0,1] += fg67[0] signed* fg67_19_38[2];h3[2,3] += fg67[1] signed* fg67_19_38[3] 
# asm 1: vmlal.s32 <h3=reg128#2,<fg67=reg128#5%bot,<fg67_19_38=reg128#12%top
# asm 2: vmlal.s32 <h3=q1,<fg67=d8,<fg67_19_38=d23
vmlal.s32 q1,d8,d23

# qhasm:   h4[0,1] = fg01_2[0] signed* fg45[0];h4[2,3] = fg01_2[1] signed* fg45[1] 
# asm 1: vmull.s32 >h4=reg128#15,<fg01_2=reg128#6%bot,<fg45=reg128#4%bot
# asm 2: vmull.s32 >h4=q14,<fg01_2=d10,<fg45=d6
vmull.s32 q14,d10,d6

# qhasm:   h4[0,1] += fg01_2[2] signed* fg23_2[2];h4[2,3] += fg01_2[3] signed* fg23_2[3] 
# asm 1: vmlal.s32 <h4=reg128#15,<fg01_2=reg128#6%top,<fg23_2=reg128#7%top
# asm 2: vmlal.s32 <h4=q14,<fg01_2=d11,<fg23_2=d13
vmlal.s32 q14,d11,d13

# qhasm:   h4[0,1] += fg23[0] signed* fg23[0];h4[2,3] += fg23[1] signed* fg23[1] 
# asm 1: vmlal.s32 <h4=reg128#15,<fg23=reg128#3%bot,<fg23=reg128#3%bot
# asm 2: vmlal.s32 <h4=q14,<fg23=d4,<fg23=d4
vmlal.s32 q14,d4,d4

# qhasm:   h4[0,1] += fg45_2[2] signed* fg89_19_38[2];h4[2,3] += fg45_2[3] signed* fg89_19_38[3] 
# asm 1: vmlal.s32 <h4=reg128#15,<fg45_2=reg128#9%top,<fg89_19_38=reg128#1%top
# asm 2: vmlal.s32 <h4=q14,<fg45_2=d17,<fg89_19_38=d1
vmlal.s32 q14,d17,d1

# qhasm:   h4[0,1] += fg67_2[0] signed* fg89_19_38[0];h4[2,3] += fg67_2[1] signed* fg89_19_38[1] 
# asm 1: vmlal.s32 <h4=reg128#15,<fg67_2=reg128#10%bot,<fg89_19_38=reg128#1%bot
# asm 2: vmlal.s32 <h4=q14,<fg67_2=d18,<fg89_19_38=d0
vmlal.s32 q14,d18,d0

# qhasm:   h4[0,1] += fg67[2] signed* fg67_19_38[2];h4[2,3] += fg67[3] signed* fg67_19_38[3] 
# asm 1: vmlal.s32 <h4=reg128#15,<fg67=reg128#5%top,<fg67_19_38=reg128#12%top
# asm 2: vmlal.s32 <h4=q14,<fg67=d9,<fg67_19_38=d23
vmlal.s32 q14,d9,d23

# qhasm:   h5[0,1] = fg01_2[0] signed* fg45[2];h5[2,3] = fg01_2[1] signed* fg45[3] 
# asm 1: vmull.s32 >h5=reg128#12,<fg01_2=reg128#6%bot,<fg45=reg128#4%top
# asm 2: vmull.s32 >h5=q11,<fg01_2=d10,<fg45=d7
vmull.s32 q11,d10,d7

# qhasm:   h5[0,1] += fg01_2[2] signed* fg45[0];h5[2,3] += fg01_2[3] signed* fg45[1] 
# asm 1: vmlal.s32 <h5=reg128#12,<fg01_2=reg128#6%top,<fg45=reg128#4%bot
# asm 2: vmlal.s32 <h5=q11,<fg01_2=d11,<fg45=d6
vmlal.s32 q11,d11,d6

# qhasm:   h5[0,1] += fg23_2[0] signed* fg23[2];h5[2,3] += fg23_2[1] signed* fg23[3] 
# asm 1: vmlal.s32 <h5=reg128#12,<fg23_2=reg128#7%bot,<fg23=reg128#3%top
# asm 2: vmlal.s32 <h5=q11,<fg23_2=d12,<fg23=d5
vmlal.s32 q11,d12,d5

# qhasm:   h5[0,1] += fg67[0] signed* fg89_19_38[2];h5[2,3] += fg67[1] signed* fg89_19_38[3] 
# asm 1: vmlal.s32 <h5=reg128#12,<fg67=reg128#5%bot,<fg89_19_38=reg128#1%top
# asm 2: vmlal.s32 <h5=q11,<fg67=d8,<fg89_19_38=d1
vmlal.s32 q11,d8,d1

# qhasm:   h5[0,1] += fg67_2[2] signed* fg89_19_38[0];h5[2,3] += fg67_2[3] signed* fg89_19_38[1] 
# asm 1: vmlal.s32 <h5=reg128#12,<fg67_2=reg128#10%top,<fg89_19_38=reg128#1%bot
# asm 2: vmlal.s32 <h5=q11,<fg67_2=d19,<fg89_19_38=d0
vmlal.s32 q11,d19,d0

# qhasm:   h6[0,1] = fg01_2[0] signed* fg67[0];h6[2,3] = fg01_2[1] signed* fg67[1] 
# asm 1: vmull.s32 >h6=reg128#16,<fg01_2=reg128#6%bot,<fg67=reg128#5%bot
# asm 2: vmull.s32 >h6=q15,<fg01_2=d10,<fg67=d8
vmull.s32 q15,d10,d8

# qhasm:   h6[0,1] += fg01_2[2] signed* fg45_2[2];h6[2,3] += fg01_2[3] signed* fg45_2[3] 
# asm 1: vmlal.s32 <h6=reg128#16,<fg01_2=reg128#6%top,<fg45_2=reg128#9%top
# asm 2: vmlal.s32 <h6=q15,<fg01_2=d11,<fg45_2=d17
vmlal.s32 q15,d11,d17

# qhasm:   h6[0,1] += fg23_2[0] signed* fg45[0];h6[2,3] += fg23_2[1] signed* fg45[1] 
# asm 1: vmlal.s32 <h6=reg128#16,<fg23_2=reg128#7%bot,<fg45=reg128#4%bot
# asm 2: vmlal.s32 <h6=q15,<fg23_2=d12,<fg45=d6
vmlal.s32 q15,d12,d6

# qhasm:   h6[0,1] += fg23_2[2] signed* fg23[2];h6[2,3] += fg23_2[3] signed* fg23[3] 
# asm 1: vmlal.s32 <h6=reg128#16,<fg23_2=reg128#7%top,<fg23=reg128#3%top
# asm 2: vmlal.s32 <h6=q15,<fg23_2=d13,<fg23=d5
vmlal.s32 q15,d13,d5

# qhasm:   h6[0,1] += fg67_2[2] signed* fg89_19_38[2];h6[2,3] += fg67_2[3] signed* fg89_19_38[3] 
# asm 1: vmlal.s32 <h6=reg128#16,<fg67_2=reg128#10%top,<fg89_19_38=reg128#1%top
# asm 2: vmlal.s32 <h6=q15,<fg67_2=d19,<fg89_19_38=d1
vmlal.s32 q15,d19,d1

# qhasm:   h6[0,1] += fg89[0] signed* fg89_19_38[0];h6[2,3] += fg89[1] signed* fg89_19_38[1] 
# asm 1: vmlal.s32 <h6=reg128#16,<fg89=reg128#8%bot,<fg89_19_38=reg128#1%bot
# asm 2: vmlal.s32 <h6=q15,<fg89=d14,<fg89_19_38=d0
vmlal.s32 q15,d14,d0

# qhasm:   h7[0,1] = fg01_2[0] signed* fg67[2];h7[2,3] = fg01_2[1] signed* fg67[3] 
# asm 1: vmull.s32 >h7=reg128#3,<fg01_2=reg128#6%bot,<fg67=reg128#5%top
# asm 2: vmull.s32 >h7=q2,<fg01_2=d10,<fg67=d9
vmull.s32 q2,d10,d9

# qhasm:   h7[0,1] += fg01_2[2] signed* fg67[0];h7[2,3] += fg01_2[3] signed* fg67[1] 
# asm 1: vmlal.s32 <h7=reg128#3,<fg01_2=reg128#6%top,<fg67=reg128#5%bot
# asm 2: vmlal.s32 <h7=q2,<fg01_2=d11,<fg67=d8
vmlal.s32 q2,d11,d8

# qhasm:   h7[0,1] += fg23_2[0] signed* fg45[2];h7[2,3] += fg23_2[1] signed* fg45[3] 
# asm 1: vmlal.s32 <h7=reg128#3,<fg23_2=reg128#7%bot,<fg45=reg128#4%top
# asm 2: vmlal.s32 <h7=q2,<fg23_2=d12,<fg45=d7
vmlal.s32 q2,d12,d7

# qhasm:   h7[0,1] += fg23_2[2] signed* fg45[0];h7[2,3] += fg23_2[3] signed* fg45[1] 
# asm 1: vmlal.s32 <h7=reg128#3,<fg23_2=reg128#7%top,<fg45=reg128#4%bot
# asm 2: vmlal.s32 <h7=q2,<fg23_2=d13,<fg45=d6
vmlal.s32 q2,d13,d6

# qhasm:   h7[0,1] += fg89[0] signed* fg89_19_38[2];h7[2,3] += fg89[1] signed* fg89_19_38[3] 
# asm 1: vmlal.s32 <h7=reg128#3,<fg89=reg128#8%bot,<fg89_19_38=reg128#1%top
# asm 2: vmlal.s32 <h7=q2,<fg89=d14,<fg89_19_38=d1
vmlal.s32 q2,d14,d1

# qhasm:   h8[0,1] = fg89[2] signed* fg89_19_38[2];h8[2,3] = fg89[3] signed* fg89_19_38[3] 
# asm 1: vmull.s32 >h8=reg128#1,<fg89=reg128#8%top,<fg89_19_38=reg128#1%top
# asm 2: vmull.s32 >h8=q0,<fg89=d15,<fg89_19_38=d1
vmull.s32 q0,d15,d1

# qhasm:   h8[0,1] += fg01_2[0] signed* fg89[0];h8[2,3] += fg01_2[1] signed* fg89[1] 
# asm 1: vmlal.s32 <h8=reg128#1,<fg01_2=reg128#6%bot,<fg89=reg128#8%bot
# asm 2: vmlal.s32 <h8=q0,<fg01_2=d10,<fg89=d14
vmlal.s32 q0,d10,d14

# qhasm:   h8[0,1] += fg01_2[2] signed* fg67_2[2];h8[2,3] += fg01_2[3] signed* fg67_2[3] 
# asm 1: vmlal.s32 <h8=reg128#1,<fg01_2=reg128#6%top,<fg67_2=reg128#10%top
# asm 2: vmlal.s32 <h8=q0,<fg01_2=d11,<fg67_2=d19
vmlal.s32 q0,d11,d19

# qhasm:   h8[0,1] += fg23_2[0] signed* fg67[0];h8[2,3] += fg23_2[1] signed* fg67[1] 
# asm 1: vmlal.s32 <h8=reg128#1,<fg23_2=reg128#7%bot,<fg67=reg128#5%bot
# asm 2: vmlal.s32 <h8=q0,<fg23_2=d12,<fg67=d8
vmlal.s32 q0,d12,d8

# qhasm:   h8[0,1] += fg23_2[2] signed* fg45_2[2];h8[2,3] += fg23_2[3] signed* fg45_2[3] 
# asm 1: vmlal.s32 <h8=reg128#1,<fg23_2=reg128#7%top,<fg45_2=reg128#9%top
# asm 2: vmlal.s32 <h8=q0,<fg23_2=d13,<fg45_2=d17
vmlal.s32 q0,d13,d17

# qhasm:   h8[0,1] += fg45[0] signed* fg45[0];h8[2,3] += fg45[1] signed* fg45[1] 
# asm 1: vmlal.s32 <h8=reg128#1,<fg45=reg128#4%bot,<fg45=reg128#4%bot
# asm 2: vmlal.s32 <h8=q0,<fg45=d6,<fg45=d6
vmlal.s32 q0,d6,d6

# qhasm:   ptr = &_0x2000000_stack 
# asm 1: lea >ptr=int32#3,<_0x2000000_stack=stack128#1
# asm 2: lea >ptr=r2,<_0x2000000_stack=[sp,#512]
add r2,sp,#512

# qhasm:   _0x2000000 aligned= mem128[ptr] 
# asm 1: vld1.8 {>_0x2000000=reg128#10%bot->_0x2000000=reg128#10%top},[<ptr=int32#3,: 128]
# asm 2: vld1.8 {>_0x2000000=d18->_0x2000000=d19},[<ptr=r2,: 128]
vld1.8 {d18-d19},[r2,: 128]

# qhasm:   h9[0,1] = fg45_2[0] signed* fg45[2];h9[2,3] = fg45_2[1] signed* fg45[3] 
# asm 1: vmull.s32 >h9=reg128#4,<fg45_2=reg128#9%bot,<fg45=reg128#4%top
# asm 2: vmull.s32 >h9=q3,<fg45_2=d16,<fg45=d7
vmull.s32 q3,d16,d7

# qhasm:   h9[0,1] += fg01_2[0] signed* fg89[2];h9[2,3] += fg01_2[1] signed* fg89[3] 
# asm 1: vmlal.s32 <h9=reg128#4,<fg01_2=reg128#6%bot,<fg89=reg128#8%top
# asm 2: vmlal.s32 <h9=q3,<fg01_2=d10,<fg89=d15
vmlal.s32 q3,d10,d15

# qhasm:   h9[0,1] += fg01_2[2] signed* fg89[0];h9[2,3] += fg01_2[3] signed* fg89[1] 
# asm 1: vmlal.s32 <h9=reg128#4,<fg01_2=reg128#6%top,<fg89=reg128#8%bot
# asm 2: vmlal.s32 <h9=q3,<fg01_2=d11,<fg89=d14
vmlal.s32 q3,d11,d14

# qhasm:   h9[0,1] += fg23_2[0] signed* fg67[2];h9[2,3] += fg23_2[1] signed* fg67[3] 
# asm 1: vmlal.s32 <h9=reg128#4,<fg23_2=reg128#7%bot,<fg67=reg128#5%top
# asm 2: vmlal.s32 <h9=q3,<fg23_2=d12,<fg67=d9
vmlal.s32 q3,d12,d9

# qhasm:   h9[0,1] += fg23_2[2] signed* fg67[0];h9[2,3] += fg23_2[3] signed* fg67[1] 
# asm 1: vmlal.s32 <h9=reg128#4,<fg23_2=reg128#7%top,<fg67=reg128#5%bot
# asm 2: vmlal.s32 <h9=q3,<fg23_2=d13,<fg67=d8
vmlal.s32 q3,d13,d8

# qhasm:   ptr = &_0x1000000_stack 
# asm 1: lea >ptr=int32#3,<_0x1000000_stack=stack128#2
# asm 2: lea >ptr=r2,<_0x1000000_stack=[sp,#528]
add r2,sp,#528

# qhasm:   _0x1000000 aligned= mem128[ptr] 
# asm 1: vld1.8 {>_0x1000000=reg128#5%bot->_0x1000000=reg128#5%top},[<ptr=int32#3,: 128]
# asm 2: vld1.8 {>_0x1000000=d8->_0x1000000=d9},[<ptr=r2,: 128]
vld1.8 {d8-d9},[r2,: 128]

# qhasm:   2x t0 = h0 + _0x2000000 
# asm 1: vadd.i64 >t0=reg128#6,<h0=reg128#13,<_0x2000000=reg128#10
# asm 2: vadd.i64 >t0=q5,<h0=q12,<_0x2000000=q9
vadd.i64 q5,q12,q9

# qhasm:   2x t6 = h6 + _0x2000000 
# asm 1: vadd.i64 >t6=reg128#7,<h6=reg128#16,<_0x2000000=reg128#10
# asm 2: vadd.i64 >t6=q6,<h6=q15,<_0x2000000=q9
vadd.i64 q6,q15,q9

# qhasm:   2x c0 = t0 signed>> 26 
# asm 1: vshr.s64 >c0=reg128#6,<t0=reg128#6,#26
# asm 2: vshr.s64 >c0=q5,<t0=q5,#26
vshr.s64 q5,q5,#26

# qhasm:   2x c6 = t6 signed>> 26 
# asm 1: vshr.s64 >c6=reg128#7,<t6=reg128#7,#26
# asm 2: vshr.s64 >c6=q6,<t6=q6,#26
vshr.s64 q6,q6,#26

# qhasm:   2x h1 += c0 
# asm 1: vadd.i64 >h1=reg128#8,<h1=reg128#11,<c0=reg128#6
# asm 2: vadd.i64 >h1=q7,<h1=q10,<c0=q5
vadd.i64 q7,q10,q5

# qhasm:   2x t0 = c0 << 26 
# asm 1: vshl.i64 >t0=reg128#6,<c0=reg128#6,#26
# asm 2: vshl.i64 >t0=q5,<c0=q5,#26
vshl.i64 q5,q5,#26

# qhasm:   2x t1 = h1 + _0x1000000 
# asm 1: vadd.i64 >t1=reg128#9,<h1=reg128#8,<_0x1000000=reg128#5
# asm 2: vadd.i64 >t1=q8,<h1=q7,<_0x1000000=q4
vadd.i64 q8,q7,q4

# qhasm:   2x h7 += c6 
# asm 1: vadd.i64 >h7=reg128#3,<h7=reg128#3,<c6=reg128#7
# asm 2: vadd.i64 >h7=q2,<h7=q2,<c6=q6
vadd.i64 q2,q2,q6

# qhasm:   2x t6 = c6 << 26 
# asm 1: vshl.i64 >t6=reg128#7,<c6=reg128#7,#26
# asm 2: vshl.i64 >t6=q6,<c6=q6,#26
vshl.i64 q6,q6,#26

# qhasm:   2x t7 = h7 + _0x1000000 
# asm 1: vadd.i64 >t7=reg128#11,<h7=reg128#3,<_0x1000000=reg128#5
# asm 2: vadd.i64 >t7=q10,<h7=q2,<_0x1000000=q4
vadd.i64 q10,q2,q4

# qhasm:   2x h0 -= t0 
# asm 1: vsub.i64 >h0=reg128#6,<h0=reg128#13,<t0=reg128#6
# asm 2: vsub.i64 >h0=q5,<h0=q12,<t0=q5
vsub.i64 q5,q12,q5

# qhasm:   2x c1 = t1 signed>> 25 
# asm 1: vshr.s64 >c1=reg128#9,<t1=reg128#9,#25
# asm 2: vshr.s64 >c1=q8,<t1=q8,#25
vshr.s64 q8,q8,#25

# qhasm:   2x h6 -= t6 
# asm 1: vsub.i64 >h6=reg128#7,<h6=reg128#16,<t6=reg128#7
# asm 2: vsub.i64 >h6=q6,<h6=q15,<t6=q6
vsub.i64 q6,q15,q6

# qhasm:   2x c7 = t7 signed>> 25 
# asm 1: vshr.s64 >c7=reg128#11,<t7=reg128#11,#25
# asm 2: vshr.s64 >c7=q10,<t7=q10,#25
vshr.s64 q10,q10,#25

# qhasm:   2x h2 += c1 
# asm 1: vadd.i64 >h2=reg128#13,<h2=reg128#14,<c1=reg128#9
# asm 2: vadd.i64 >h2=q12,<h2=q13,<c1=q8
vadd.i64 q12,q13,q8

# qhasm:   2x t1 = c1 << 25 
# asm 1: vshl.i64 >t1=reg128#9,<c1=reg128#9,#25
# asm 2: vshl.i64 >t1=q8,<c1=q8,#25
vshl.i64 q8,q8,#25

# qhasm:   2x t2 = h2 + _0x2000000 
# asm 1: vadd.i64 >t2=reg128#14,<h2=reg128#13,<_0x2000000=reg128#10
# asm 2: vadd.i64 >t2=q13,<h2=q12,<_0x2000000=q9
vadd.i64 q13,q12,q9

# qhasm:   2x h8 += c7 
# asm 1: vadd.i64 >h8=reg128#1,<h8=reg128#1,<c7=reg128#11
# asm 2: vadd.i64 >h8=q0,<h8=q0,<c7=q10
vadd.i64 q0,q0,q10

# qhasm:   2x h1 -= t1 
# asm 1: vsub.i64 >h1=reg128#8,<h1=reg128#8,<t1=reg128#9
# asm 2: vsub.i64 >h1=q7,<h1=q7,<t1=q8
vsub.i64 q7,q7,q8

# qhasm:   2x c2 = t2 signed>> 26 
# asm 1: vshr.s64 >c2=reg128#9,<t2=reg128#14,#26
# asm 2: vshr.s64 >c2=q8,<t2=q13,#26
vshr.s64 q8,q13,#26

# qhasm:   2x t7 = c7 << 25 
# asm 1: vshl.i64 >t7=reg128#11,<c7=reg128#11,#25
# asm 2: vshl.i64 >t7=q10,<c7=q10,#25
vshl.i64 q10,q10,#25

# qhasm:   2x t8 = h8 + _0x2000000 
# asm 1: vadd.i64 >t8=reg128#14,<h8=reg128#1,<_0x2000000=reg128#10
# asm 2: vadd.i64 >t8=q13,<h8=q0,<_0x2000000=q9
vadd.i64 q13,q0,q9

# qhasm:   2x h3 += c2 
# asm 1: vadd.i64 >h3=reg128#2,<h3=reg128#2,<c2=reg128#9
# asm 2: vadd.i64 >h3=q1,<h3=q1,<c2=q8
vadd.i64 q1,q1,q8

# qhasm:   2x t2 = c2 << 26 
# asm 1: vshl.i64 >t2=reg128#9,<c2=reg128#9,#26
# asm 2: vshl.i64 >t2=q8,<c2=q8,#26
vshl.i64 q8,q8,#26

# qhasm:   2x t3 = h3 + _0x1000000 
# asm 1: vadd.i64 >t3=reg128#16,<h3=reg128#2,<_0x1000000=reg128#5
# asm 2: vadd.i64 >t3=q15,<h3=q1,<_0x1000000=q4
vadd.i64 q15,q1,q4

# qhasm:   2x h7 -= t7 
# asm 1: vsub.i64 >h7=reg128#3,<h7=reg128#3,<t7=reg128#11
# asm 2: vsub.i64 >h7=q2,<h7=q2,<t7=q10
vsub.i64 q2,q2,q10

# qhasm:   2x c8 = t8 signed>> 26 
# asm 1: vshr.s64 >c8=reg128#11,<t8=reg128#14,#26
# asm 2: vshr.s64 >c8=q10,<t8=q13,#26
vshr.s64 q10,q13,#26

# qhasm:   2x h2 -= t2 
# asm 1: vsub.i64 >h2=reg128#9,<h2=reg128#13,<t2=reg128#9
# asm 2: vsub.i64 >h2=q8,<h2=q12,<t2=q8
vsub.i64 q8,q12,q8

# qhasm:   2x c3 = t3 signed>> 25 
# asm 1: vshr.s64 >c3=reg128#13,<t3=reg128#16,#25
# asm 2: vshr.s64 >c3=q12,<t3=q15,#25
vshr.s64 q12,q15,#25

# qhasm:   2x h9 += c8 
# asm 1: vadd.i64 >h9=reg128#4,<h9=reg128#4,<c8=reg128#11
# asm 2: vadd.i64 >h9=q3,<h9=q3,<c8=q10
vadd.i64 q3,q3,q10

# qhasm:   2x t8 = c8 << 26 
# asm 1: vshl.i64 >t8=reg128#11,<c8=reg128#11,#26
# asm 2: vshl.i64 >t8=q10,<c8=q10,#26
vshl.i64 q10,q10,#26

# qhasm:   2x t9 = h9 + _0x1000000 
# asm 1: vadd.i64 >t9=reg128#14,<h9=reg128#4,<_0x1000000=reg128#5
# asm 2: vadd.i64 >t9=q13,<h9=q3,<_0x1000000=q4
vadd.i64 q13,q3,q4

# qhasm:   2x h4 += c3 
# asm 1: vadd.i64 >h4=reg128#15,<h4=reg128#15,<c3=reg128#13
# asm 2: vadd.i64 >h4=q14,<h4=q14,<c3=q12
vadd.i64 q14,q14,q12

# qhasm:   posh = playground1_ptr + 144 
# asm 1: add >posh=int32#3,<playground1_ptr=int32#4,#144
# asm 2: add >posh=r2,<playground1_ptr=r3,#144
add r2,r3,#144

# qhasm:   2x t3 = c3 << 25 
# asm 1: vshl.i64 >t3=reg128#13,<c3=reg128#13,#25
# asm 2: vshl.i64 >t3=q12,<c3=q12,#25
vshl.i64 q12,q12,#25

# qhasm:   posH = playground1_ptr + 192 
# asm 1: add >posH=int32#5,<playground1_ptr=int32#4,#192
# asm 2: add >posH=r4,<playground1_ptr=r3,#192
add r4,r3,#192

# qhasm:   2x t4 = h4 + _0x2000000 
# asm 1: vadd.i64 >t4=reg128#16,<h4=reg128#15,<_0x2000000=reg128#10
# asm 2: vadd.i64 >t4=q15,<h4=q14,<_0x2000000=q9
vadd.i64 q15,q14,q9

# qhasm:   posh+=8
# asm 1: add >posh=int32#3,<posh=int32#3,#8
# asm 2: add >posh=r2,<posh=r2,#8
add r2,r2,#8

# qhasm:   2x h8 -= t8 
# asm 1: vsub.i64 >h8=reg128#1,<h8=reg128#1,<t8=reg128#11
# asm 2: vsub.i64 >h8=q0,<h8=q0,<t8=q10
vsub.i64 q0,q0,q10

# qhasm:   posH+=8
# asm 1: add >posH=int32#5,<posH=int32#5,#8
# asm 2: add >posH=r4,<posH=r4,#8
add r4,r4,#8

# qhasm:   2x c9 = t9 signed>> 25 
# asm 1: vshr.s64 >c9=reg128#11,<t9=reg128#14,#25
# asm 2: vshr.s64 >c9=q10,<t9=q13,#25
vshr.s64 q10,q13,#25

# qhasm:   2x h3 -= t3 
# asm 1: vsub.i64 >h3=reg128#2,<h3=reg128#2,<t3=reg128#13
# asm 2: vsub.i64 >h3=q1,<h3=q1,<t3=q12
vsub.i64 q1,q1,q12

# qhasm:   2x c4 = t4 signed>> 26 
# asm 1: vshr.s64 >c4=reg128#13,<t4=reg128#16,#26
# asm 2: vshr.s64 >c4=q12,<t4=q15,#26
vshr.s64 q12,q15,#26

# qhasm:   2x s = c9 + c9 
# asm 1: vadd.i64 >s=reg128#14,<c9=reg128#11,<c9=reg128#11
# asm 2: vadd.i64 >s=q13,<c9=q10,<c9=q10
vadd.i64 q13,q10,q10

# qhasm:   2x h5 += c4 
# asm 1: vadd.i64 >h5=reg128#12,<h5=reg128#12,<c4=reg128#13
# asm 2: vadd.i64 >h5=q11,<h5=q11,<c4=q12
vadd.i64 q11,q11,q12

# qhasm:   h2 h3 = h2[0]h3[0]h2[2]h2[3] h2[1]h3[1]h3[2]h3[3]
# asm 1: vtrn.32 <h2=reg128#9%bot,<h3=reg128#2%bot
# asm 2: vtrn.32 <h2=d16,<h3=d2
vtrn.32 d16,d2

# qhasm:   2x t4 = c4 << 26 
# asm 1: vshl.i64 >t4=reg128#13,<c4=reg128#13,#26
# asm 2: vshl.i64 >t4=q12,<c4=q12,#26
vshl.i64 q12,q12,#26

# qhasm:   h2 h3 = h2[0]h2[1]h2[2]h3[2] h3[0]h3[1]h2[3]h3[3]
# asm 1: vtrn.32 <h2=reg128#9%top,<h3=reg128#2%top
# asm 2: vtrn.32 <h2=d17,<h3=d3
vtrn.32 d17,d3

# qhasm:   2x t5 = h5 + _0x1000000 
# asm 1: vadd.i64 >t5=reg128#2,<h5=reg128#12,<_0x1000000=reg128#5
# asm 2: vadd.i64 >t5=q1,<h5=q11,<_0x1000000=q4
vadd.i64 q1,q11,q4

# qhasm:   2x h0 += s 
# asm 1: vadd.i64 >h0=reg128#5,<h0=reg128#6,<s=reg128#14
# asm 2: vadd.i64 >h0=q4,<h0=q5,<s=q13
vadd.i64 q4,q5,q13

# qhasm:   mem64[posh] aligned= h2[0];posh+=8 
# asm 1: vst1.8 <h2=reg128#9%bot,[<posh=int32#3,: 64]!
# asm 2: vst1.8 <h2=d16,[<posh=r2,: 64]!
vst1.8 d16,[r2,: 64]!

# qhasm:   2x s = c9 << 4 
# asm 1: vshl.i64 >s=reg128#6,<c9=reg128#11,#4
# asm 2: vshl.i64 >s=q5,<c9=q10,#4
vshl.i64 q5,q10,#4

# qhasm:   mem64[posH] aligned= h2[1];posH+=8 
# asm 1: vst1.8 <h2=reg128#9%top,[<posH=int32#5,: 64]!
# asm 2: vst1.8 <h2=d17,[<posH=r4,: 64]!
vst1.8 d17,[r4,: 64]!

# qhasm:   2x h4 -= t4 
# asm 1: vsub.i64 >h4=reg128#9,<h4=reg128#15,<t4=reg128#13
# asm 2: vsub.i64 >h4=q8,<h4=q14,<t4=q12
vsub.i64 q8,q14,q12

# qhasm:   2x c5 = t5 signed>> 25 
# asm 1: vshr.s64 >c5=reg128#2,<t5=reg128#2,#25
# asm 2: vshr.s64 >c5=q1,<t5=q1,#25
vshr.s64 q1,q1,#25

# qhasm:   2x h0 += s 
# asm 1: vadd.i64 >h0=reg128#5,<h0=reg128#5,<s=reg128#6
# asm 2: vadd.i64 >h0=q4,<h0=q4,<s=q5
vadd.i64 q4,q4,q5

# qhasm:   2x h6 += c5 
# asm 1: vadd.i64 >h6=reg128#6,<h6=reg128#7,<c5=reg128#2
# asm 2: vadd.i64 >h6=q5,<h6=q6,<c5=q1
vadd.i64 q5,q6,q1

# qhasm:   2x t5 = c5 << 25 
# asm 1: vshl.i64 >t5=reg128#2,<c5=reg128#2,#25
# asm 2: vshl.i64 >t5=q1,<c5=q1,#25
vshl.i64 q1,q1,#25

# qhasm:   2x t6 = h6 + _0x2000000 
# asm 1: vadd.i64 >t6=reg128#7,<h6=reg128#6,<_0x2000000=reg128#10
# asm 2: vadd.i64 >t6=q6,<h6=q5,<_0x2000000=q9
vadd.i64 q6,q5,q9

# qhasm:   2x h0 += c9 
# asm 1: vadd.i64 >h0=reg128#5,<h0=reg128#5,<c9=reg128#11
# asm 2: vadd.i64 >h0=q4,<h0=q4,<c9=q10
vadd.i64 q4,q4,q10

# qhasm:   2x t9 = c9 << 25 
# asm 1: vshl.i64 >t9=reg128#11,<c9=reg128#11,#25
# asm 2: vshl.i64 >t9=q10,<c9=q10,#25
vshl.i64 q10,q10,#25

# qhasm:   2x t0 = h0 + _0x2000000 
# asm 1: vadd.i64 >t0=reg128#10,<h0=reg128#5,<_0x2000000=reg128#10
# asm 2: vadd.i64 >t0=q9,<h0=q4,<_0x2000000=q9
vadd.i64 q9,q4,q9

# qhasm:   2x h5 -= t5 
# asm 1: vsub.i64 >h5=reg128#2,<h5=reg128#12,<t5=reg128#2
# asm 2: vsub.i64 >h5=q1,<h5=q11,<t5=q1
vsub.i64 q1,q11,q1

# qhasm:   2x c6 = t6 signed>> 26 
# asm 1: vshr.s64 >c6=reg128#7,<t6=reg128#7,#26
# asm 2: vshr.s64 >c6=q6,<t6=q6,#26
vshr.s64 q6,q6,#26

# qhasm:   2x h9 -= t9 
# asm 1: vsub.i64 >h9=reg128#4,<h9=reg128#4,<t9=reg128#11
# asm 2: vsub.i64 >h9=q3,<h9=q3,<t9=q10
vsub.i64 q3,q3,q10

# qhasm:   h4 h5 = h4[0]h5[0]h4[2]h4[3] h4[1]h5[1]h5[2]h5[3]
# asm 1: vtrn.32 <h4=reg128#9%bot,<h5=reg128#2%bot
# asm 2: vtrn.32 <h4=d16,<h5=d2
vtrn.32 d16,d2

# qhasm:   2x c0 = t0 signed>> 26 
# asm 1: vshr.s64 >c0=reg128#10,<t0=reg128#10,#26
# asm 2: vshr.s64 >c0=q9,<t0=q9,#26
vshr.s64 q9,q9,#26

# qhasm:   h4 h5 = h4[0]h4[1]h4[2]h5[2] h5[0]h5[1]h4[3]h5[3]
# asm 1: vtrn.32 <h4=reg128#9%top,<h5=reg128#2%top
# asm 2: vtrn.32 <h4=d17,<h5=d3
vtrn.32 d17,d3

# qhasm:   2x h7 += c6 
# asm 1: vadd.i64 >h7=reg128#2,<h7=reg128#3,<c6=reg128#7
# asm 2: vadd.i64 >h7=q1,<h7=q2,<c6=q6
vadd.i64 q1,q2,q6

# qhasm:   mem64[posh] aligned= h4[0] 
# asm 1: vst1.8 <h4=reg128#9%bot,[<posh=int32#3,: 64]
# asm 2: vst1.8 <h4=d16,[<posh=r2,: 64]
vst1.8 d16,[r2,: 64]

# qhasm:   2x t6 = c6 << 26 
# asm 1: vshl.i64 >t6=reg128#3,<c6=reg128#7,#26
# asm 2: vshl.i64 >t6=q2,<c6=q6,#26
vshl.i64 q2,q6,#26

# qhasm:   mem64[posH] aligned= h4[1] 
# asm 1: vst1.8 <h4=reg128#9%top,[<posH=int32#5,: 64]
# asm 2: vst1.8 <h4=d17,[<posH=r4,: 64]
vst1.8 d17,[r4,: 64]

# qhasm:   2x h1 += c0 
# asm 1: vadd.i64 >h1=reg128#7,<h1=reg128#8,<c0=reg128#10
# asm 2: vadd.i64 >h1=q6,<h1=q7,<c0=q9
vadd.i64 q6,q7,q9

# qhasm:   h8 h9 = h8[0]h9[0]h8[2]h8[3] h8[1]h9[1]h9[2]h9[3]
# asm 1: vtrn.32 <h8=reg128#1%bot,<h9=reg128#4%bot
# asm 2: vtrn.32 <h8=d0,<h9=d6
vtrn.32 d0,d6

# qhasm:   2x t0 = c0 << 26 
# asm 1: vshl.i64 >t0=reg128#8,<c0=reg128#10,#26
# asm 2: vshl.i64 >t0=q7,<c0=q9,#26
vshl.i64 q7,q9,#26

# qhasm:   h8 h9 = h8[0]h8[1]h8[2]h9[2] h9[0]h9[1]h8[3]h9[3]
# asm 1: vtrn.32 <h8=reg128#1%top,<h9=reg128#4%top
# asm 2: vtrn.32 <h8=d1,<h9=d7
vtrn.32 d1,d7

# qhasm:   2x h6 -= t6 
# asm 1: vsub.i64 >h6=reg128#3,<h6=reg128#6,<t6=reg128#3
# asm 2: vsub.i64 >h6=q2,<h6=q5,<t6=q2
vsub.i64 q2,q5,q2

# qhasm:   posh+=16
# asm 1: add >posh=int32#3,<posh=int32#3,#16
# asm 2: add >posh=r2,<posh=r2,#16
add r2,r2,#16

# qhasm:   2x h0 -= t0 
# asm 1: vsub.i64 >h0=reg128#4,<h0=reg128#5,<t0=reg128#8
# asm 2: vsub.i64 >h0=q3,<h0=q4,<t0=q7
vsub.i64 q3,q4,q7

# qhasm:   mem64[posh] aligned= h8[0] 
# asm 1: vst1.8 <h8=reg128#1%bot,[<posh=int32#3,: 64]
# asm 2: vst1.8 <h8=d0,[<posh=r2,: 64]
vst1.8 d0,[r2,: 64]

# qhasm:   posH+=16
# asm 1: add >posH=int32#5,<posH=int32#5,#16
# asm 2: add >posH=r4,<posH=r4,#16
add r4,r4,#16

# qhasm:   mem64[posH] aligned= h8[1] 
# asm 1: vst1.8 <h8=reg128#1%top,[<posH=int32#5,: 64]
# asm 2: vst1.8 <h8=d1,[<posH=r4,: 64]
vst1.8 d1,[r4,: 64]

# qhasm:   h6 h7 = h6[0]h7[0]h6[2]h6[3] h6[1]h7[1]h7[2]h7[3]
# asm 1: vtrn.32 <h6=reg128#3%bot,<h7=reg128#2%bot
# asm 2: vtrn.32 <h6=d4,<h7=d2
vtrn.32 d4,d2

# qhasm:   h6 h7 = h6[0]h6[1]h6[2]h7[2] h7[0]h7[1]h6[3]h7[3]
# asm 1: vtrn.32 <h6=reg128#3%top,<h7=reg128#2%top
# asm 2: vtrn.32 <h6=d5,<h7=d3
vtrn.32 d5,d3

# qhasm:   posh-=8
# asm 1: sub >posh=int32#3,<posh=int32#3,#8
# asm 2: sub >posh=r2,<posh=r2,#8
sub r2,r2,#8

# qhasm:   posH-=8
# asm 1: sub >posH=int32#5,<posH=int32#5,#8
# asm 2: sub >posH=r4,<posH=r4,#8
sub r4,r4,#8

# qhasm:   h0 h1 = h0[0]h1[0]h0[2]h0[3] h0[1]h1[1]h1[2]h1[3]
# asm 1: vtrn.32 <h0=reg128#4%bot,<h1=reg128#7%bot
# asm 2: vtrn.32 <h0=d6,<h1=d12
vtrn.32 d6,d12

# qhasm:   h0 h1 = h0[0]h0[1]h0[2]h1[2] h1[0]h1[1]h0[3]h1[3]
# asm 1: vtrn.32 <h0=reg128#4%top,<h1=reg128#7%top
# asm 2: vtrn.32 <h0=d7,<h1=d13
vtrn.32 d7,d13

# qhasm:   mem64[posh] aligned= h6[0] 
# asm 1: vst1.8 <h6=reg128#3%bot,[<posh=int32#3,: 64]
# asm 2: vst1.8 <h6=d4,[<posh=r2,: 64]
vst1.8 d4,[r2,: 64]

# qhasm:   mem64[posH] aligned= h6[1] 
# asm 1: vst1.8 <h6=reg128#3%top,[<posH=int32#5,: 64]
# asm 2: vst1.8 <h6=d5,[<posH=r4,: 64]
vst1.8 d5,[r4,: 64]

# qhasm:   posh-=24
# asm 1: sub >posh=int32#3,<posh=int32#3,#24
# asm 2: sub >posh=r2,<posh=r2,#24
sub r2,r2,#24

# qhasm:   posH-=24
# asm 1: sub >posH=int32#5,<posH=int32#5,#24
# asm 2: sub >posH=r4,<posH=r4,#24
sub r4,r4,#24

# qhasm:   mem64[posh] aligned= h0[0] 
# asm 1: vst1.8 <h0=reg128#4%bot,[<posh=int32#3,: 64]
# asm 2: vst1.8 <h0=d6,[<posh=r2,: 64]
vst1.8 d6,[r2,: 64]

# qhasm:   mem64[posH] aligned= h0[1] 
# asm 1: vst1.8 <h0=reg128#4%top,[<posH=int32#5,: 64]
# asm 2: vst1.8 <h0=d7,[<posH=r4,: 64]
vst1.8 d7,[r4,: 64]

# qhasm:   pos1 = playground1_ptr + 336 
# asm 1: add >pos1=int32#3,<playground1_ptr=int32#4,#336
# asm 2: add >pos1=r2,<playground1_ptr=r3,#336
add r2,r3,#336

# qhasm:   pos2 = playground1_ptr + 288 
# asm 1: add >pos2=int32#5,<playground1_ptr=int32#4,#288
# asm 2: add >pos2=r4,<playground1_ptr=r3,#288
add r4,r3,#288

# qhasm:   f0 aligned= mem128[pos1];pos1 += 16 
# asm 1: vld1.8 {>f0=reg128#1%bot->f0=reg128#1%top},[<pos1=int32#3,: 128]!
# asm 2: vld1.8 {>f0=d0->f0=d1},[<pos1=r2,: 128]!
vld1.8 {d0-d1},[r2,: 128]!

# qhasm:   g0 aligned= mem128[pos2];pos2 += 16 
# asm 1: vld1.8 {>g0=reg128#2%bot->g0=reg128#2%top},[<pos2=int32#5,: 128]!
# asm 2: vld1.8 {>g0=d2->g0=d3},[<pos2=r4,: 128]!
vld1.8 {d2-d3},[r4,: 128]!

# qhasm:   4x f0 += g0 
# asm 1: vadd.i32 >f0=reg128#1,<f0=reg128#1,<g0=reg128#2
# asm 2: vadd.i32 >f0=q0,<f0=q0,<g0=q1
vadd.i32 q0,q0,q1

# qhasm:   f4 aligned= mem128[pos1];pos1 += 16 
# asm 1: vld1.8 {>f4=reg128#2%bot->f4=reg128#2%top},[<pos1=int32#3,: 128]!
# asm 2: vld1.8 {>f4=d2->f4=d3},[<pos1=r2,: 128]!
vld1.8 {d2-d3},[r2,: 128]!

# qhasm:   g4 aligned= mem128[pos2];pos2 += 16 
# asm 1: vld1.8 {>g4=reg128#3%bot->g4=reg128#3%top},[<pos2=int32#5,: 128]!
# asm 2: vld1.8 {>g4=d4->g4=d5},[<pos2=r4,: 128]!
vld1.8 {d4-d5},[r4,: 128]!

# qhasm:   4x f4 += g4 
# asm 1: vadd.i32 >f4=reg128#2,<f4=reg128#2,<g4=reg128#3
# asm 2: vadd.i32 >f4=q1,<f4=q1,<g4=q2
vadd.i32 q1,q1,q2

# qhasm:   pos0 = playground1_ptr + 288 
# asm 1: add >pos0=int32#6,<playground1_ptr=int32#4,#288
# asm 2: add >pos0=r5,<playground1_ptr=r3,#288
add r5,r3,#288

# qhasm:   new f8 

# qhasm:   new g8 

# qhasm:   f8 aligned= mem64[pos1] f8[1] 
# asm 1: vld1.8 {<f8=reg128#3%bot},[<pos1=int32#3,: 64]
# asm 2: vld1.8 {<f8=d4},[<pos1=r2,: 64]
vld1.8 {d4},[r2,: 64]

# qhasm:   g8 aligned= mem64[pos2] g8[1] 
# asm 1: vld1.8 {<g8=reg128#4%bot},[<pos2=int32#5,: 64]
# asm 2: vld1.8 {<g8=d6},[<pos2=r4,: 64]
vld1.8 {d6},[r4,: 64]

# qhasm:   4x f8 += g8 
# asm 1: vadd.i32 >f8=reg128#3,<f8=reg128#3,<g8=reg128#4
# asm 2: vadd.i32 >f8=q2,<f8=q2,<g8=q3
vadd.i32 q2,q2,q3

# qhasm:   mem128[pos0] aligned= f0;pos0 += 16 
# asm 1: vst1.8 {<f0=reg128#1%bot-<f0=reg128#1%top},[<pos0=int32#6,: 128]!
# asm 2: vst1.8 {<f0=d0-<f0=d1},[<pos0=r5,: 128]!
vst1.8 {d0-d1},[r5,: 128]!

# qhasm:   mem128[pos0] aligned= f4;pos0 += 16 
# asm 1: vst1.8 {<f4=reg128#2%bot-<f4=reg128#2%top},[<pos0=int32#6,: 128]!
# asm 2: vst1.8 {<f4=d2-<f4=d3},[<pos0=r5,: 128]!
vst1.8 {d2-d3},[r5,: 128]!

# qhasm:   mem64[pos0] aligned= f8[0] 
# asm 1: vst1.8 <f8=reg128#3%bot,[<pos0=int32#6,: 64]
# asm 2: vst1.8 <f8=d4,[<pos0=r5,: 64]
vst1.8 d4,[r5,: 64]

# qhasm:   posf = playground1_ptr + 48 
# asm 1: add >posf=int32#3,<playground1_ptr=int32#4,#48
# asm 2: add >posf=r2,<playground1_ptr=r3,#48
add r2,r3,#48

# qhasm:   posg = playground1_ptr + 144 
# asm 1: add >posg=int32#5,<playground1_ptr=int32#4,#144
# asm 2: add >posg=r4,<playground1_ptr=r3,#144
add r4,r3,#144

# qhasm:   g02 aligned= mem128[posg];posg += 16 
# asm 1: vld1.8 {>g02=reg128#1%bot->g02=reg128#1%top},[<posg=int32#5,: 128]!
# asm 2: vld1.8 {>g02=d0->g02=d1},[<posg=r4,: 128]!
vld1.8 {d0-d1},[r4,: 128]!

# qhasm:   g46 aligned= mem128[posg];posg += 16 
# asm 1: vld1.8 {>g46=reg128#2%bot->g46=reg128#2%top},[<posg=int32#5,: 128]!
# asm 2: vld1.8 {>g46=d2->g46=d3},[<posg=r4,: 128]!
vld1.8 {d2-d3},[r4,: 128]!

# qhasm:   new g89 

# qhasm:   g89 aligned= mem64[posg] g89[1] 
# asm 1: vld1.8 {<g89=reg128#3%bot},[<posg=int32#5,: 64]
# asm 2: vld1.8 {<g89=d4},[<posg=r4,: 64]
vld1.8 {d4},[r4,: 64]

# qhasm:   posG = playground1_ptr + 288 
# asm 1: add >posG=int32#5,<playground1_ptr=int32#4,#288
# asm 2: add >posG=r4,<playground1_ptr=r3,#288
add r4,r3,#288

# qhasm:   g13 aligned= mem128[posG];posG += 16 
# asm 1: vld1.8 {>g13=reg128#4%bot->g13=reg128#4%top},[<posG=int32#5,: 128]!
# asm 2: vld1.8 {>g13=d6->g13=d7},[<posG=r4,: 128]!
vld1.8 {d6-d7},[r4,: 128]!

# qhasm:   g02 g13 = g02[0]g13[0] g02[2]g13[2] g02[1]g13[1] g02[3]g13[3] 
# asm 1: vtrn.32 <g02=reg128#1,<g13=reg128#4
# asm 2: vtrn.32 <g02=q0,<g13=q3
vtrn.32 q0,q3

# qhasm:   g57 aligned= mem128[posG];posG += 16 
# asm 1: vld1.8 {>g57=reg128#5%bot->g57=reg128#5%top},[<posG=int32#5,: 128]!
# asm 2: vld1.8 {>g57=d8->g57=d9},[<posG=r4,: 128]!
vld1.8 {d8-d9},[r4,: 128]!

# qhasm:   4x mix = g02 << 4 
# asm 1: vshl.i32 >mix=reg128#6,<g02=reg128#1,#4
# asm 2: vshl.i32 >mix=q5,<g02=q0,#4
vshl.i32 q5,q0,#4

# qhasm:   g46 g57 = g46[0]g57[0] g46[2]g57[2] g46[1]g57[1] g46[3]g57[3] 
# asm 1: vtrn.32 <g46=reg128#2,<g57=reg128#5
# asm 2: vtrn.32 <g46=q1,<g57=q4
vtrn.32 q1,q4

# qhasm:   4x g13_19 = g13 << 4 
# asm 1: vshl.i32 >g13_19=reg128#7,<g13=reg128#4,#4
# asm 2: vshl.i32 >g13_19=q6,<g13=q3,#4
vshl.i32 q6,q3,#4

# qhasm:   4x mix += g02 
# asm 1: vadd.i32 >mix=reg128#6,<mix=reg128#6,<g02=reg128#1
# asm 2: vadd.i32 >mix=q5,<mix=q5,<g02=q0
vadd.i32 q5,q5,q0

# qhasm:   4x g13_19 += g13 
# asm 1: vadd.i32 >g13_19=reg128#7,<g13_19=reg128#7,<g13=reg128#4
# asm 2: vadd.i32 >g13_19=q6,<g13_19=q6,<g13=q3
vadd.i32 q6,q6,q3

# qhasm:   4x g46_19 = g46 << 4 
# asm 1: vshl.i32 >g46_19=reg128#8,<g46=reg128#2,#4
# asm 2: vshl.i32 >g46_19=q7,<g46=q1,#4
vshl.i32 q7,q1,#4

# qhasm:   g89 aligned= g89[0] mem64[posG] 
# asm 1: vld1.8 {<g89=reg128#3%top},[<posG=int32#5,: 64]
# asm 2: vld1.8 {<g89=d5},[<posG=r4,: 64]
vld1.8 {d5},[r4,: 64]

# qhasm:   4x g57_19 = g57 << 4 
# asm 1: vshl.i32 >g57_19=reg128#9,<g57=reg128#5,#4
# asm 2: vshl.i32 >g57_19=q8,<g57=q4,#4
vshl.i32 q8,q4,#4

# qhasm:   g89 = g89[0] g89[2] g89[1] g89[3] 
# asm 1: vtrn.32 <g89=reg128#3%bot,<g89=reg128#3%top
# asm 2: vtrn.32 <g89=d4,<g89=d5
vtrn.32 d4,d5

# qhasm:   4x g46_19 += g46 
# asm 1: vadd.i32 >g46_19=reg128#8,<g46_19=reg128#8,<g46=reg128#2
# asm 2: vadd.i32 >g46_19=q7,<g46_19=q7,<g46=q1
vadd.i32 q7,q7,q1

# qhasm:   4x g57_19 += g57 
# asm 1: vadd.i32 >g57_19=reg128#9,<g57_19=reg128#9,<g57=reg128#5
# asm 2: vadd.i32 >g57_19=q8,<g57_19=q8,<g57=q4
vadd.i32 q8,q8,q4

# qhasm:   f02 aligned= mem128[posf];posf += 16 
# asm 1: vld1.8 {>f02=reg128#10%bot->f02=reg128#10%top},[<posf=int32#3,: 128]!
# asm 2: vld1.8 {>f02=d18->f02=d19},[<posf=r2,: 128]!
vld1.8 {d18-d19},[r2,: 128]!

# qhasm:   4x g89_19 = g89 << 4 
# asm 1: vshl.i32 >g89_19=reg128#11,<g89=reg128#3,#4
# asm 2: vshl.i32 >g89_19=q10,<g89=q2,#4
vshl.i32 q10,q2,#4

# qhasm:   f46 aligned= mem128[posf];posf += 16 
# asm 1: vld1.8 {>f46=reg128#12%bot->f46=reg128#12%top},[<posf=int32#3,: 128]!
# asm 2: vld1.8 {>f46=d22->f46=d23},[<posf=r2,: 128]!
vld1.8 {d22-d23},[r2,: 128]!

# qhasm:   4x g89_19 += g89 
# asm 1: vadd.i32 >g89_19=reg128#11,<g89_19=reg128#11,<g89=reg128#3
# asm 2: vadd.i32 >g89_19=q10,<g89_19=q10,<g89=q2
vadd.i32 q10,q10,q2

# qhasm:   new f89 

# qhasm:   f89 aligned= mem64[posf] f89[1] 
# asm 1: vld1.8 {<f89=reg128#13%bot},[<posf=int32#3,: 64]
# asm 2: vld1.8 {<f89=d24},[<posf=r2,: 64]
vld1.8 {d24},[r2,: 64]

# qhasm:   4x mix += g02 
# asm 1: vadd.i32 >mix=reg128#6,<mix=reg128#6,<g02=reg128#1
# asm 2: vadd.i32 >mix=q5,<mix=q5,<g02=q0
vadd.i32 q5,q5,q0

# qhasm:   posF = playground1_ptr + 240 
# asm 1: add >posF=int32#3,<playground1_ptr=int32#4,#240
# asm 2: add >posF=r2,<playground1_ptr=r3,#240
add r2,r3,#240

# qhasm:   f13 aligned= mem128[posF];posF += 16 
# asm 1: vld1.8 {>f13=reg128#14%bot->f13=reg128#14%top},[<posF=int32#3,: 128]!
# asm 2: vld1.8 {>f13=d26->f13=d27},[<posF=r2,: 128]!
vld1.8 {d26-d27},[r2,: 128]!

# qhasm:   4x g13_19 += g13 
# asm 1: vadd.i32 >g13_19=reg128#7,<g13_19=reg128#7,<g13=reg128#4
# asm 2: vadd.i32 >g13_19=q6,<g13_19=q6,<g13=q3
vadd.i32 q6,q6,q3

# qhasm:   f57 aligned= mem128[posF];posF += 16 
# asm 1: vld1.8 {>f57=reg128#15%bot->f57=reg128#15%top},[<posF=int32#3,: 128]!
# asm 2: vld1.8 {>f57=d28->f57=d29},[<posF=r2,: 128]!
vld1.8 {d28-d29},[r2,: 128]!

# qhasm:   4x g57_19 += g57 
# asm 1: vadd.i32 >g57_19=reg128#9,<g57_19=reg128#9,<g57=reg128#5
# asm 2: vadd.i32 >g57_19=q8,<g57_19=q8,<g57=q4
vadd.i32 q8,q8,q4

# qhasm:   f89 aligned= f89[0] mem64[posF] 
# asm 1: vld1.8 {<f89=reg128#13%top},[<posF=int32#3,: 64]
# asm 2: vld1.8 {<f89=d25},[<posF=r2,: 64]
vld1.8 {d25},[r2,: 64]

# qhasm:   4x g89_19 += g89 
# asm 1: vadd.i32 >g89_19=reg128#11,<g89_19=reg128#11,<g89=reg128#3
# asm 2: vadd.i32 >g89_19=q10,<g89_19=q10,<g89=q2
vadd.i32 q10,q10,q2

# qhasm:   f02 f13 = f02[0]f13[0] f02[2]f13[2] f02[1]f13[1] f02[3]f13[3] 
# asm 1: vtrn.32 <f02=reg128#10,<f13=reg128#14
# asm 2: vtrn.32 <f02=q9,<f13=q13
vtrn.32 q9,q13

# qhasm:   4x g46_19 += g46 
# asm 1: vadd.i32 >g46_19=reg128#8,<g46_19=reg128#8,<g46=reg128#2
# asm 2: vadd.i32 >g46_19=q7,<g46_19=q7,<g46=q1
vadd.i32 q7,q7,q1

# qhasm:   4x mix += g02 
# asm 1: vadd.i32 >mix=reg128#6,<mix=reg128#6,<g02=reg128#1
# asm 2: vadd.i32 >mix=q5,<mix=q5,<g02=q0
vadd.i32 q5,q5,q0

# qhasm:   f46 f57 = f46[0]f57[0] f46[2]f57[2] f46[1]f57[1] f46[3]f57[3] 
# asm 1: vtrn.32 <f46=reg128#12,<f57=reg128#15
# asm 2: vtrn.32 <f46=q11,<f57=q14
vtrn.32 q11,q14

# qhasm:   4x g13_19 += g13 
# asm 1: vadd.i32 >g13_19=reg128#7,<g13_19=reg128#7,<g13=reg128#4
# asm 2: vadd.i32 >g13_19=q6,<g13_19=q6,<g13=q3
vadd.i32 q6,q6,q3

# qhasm:   new g13_19_stack 

# qhasm:   ptr = &g13_19_stack 
# asm 1: lea >ptr=int32#3,<g13_19_stack=stack128#4
# asm 2: lea >ptr=r2,<g13_19_stack=[sp,#560]
add r2,sp,#560

# qhasm:   4x g89_19 += g89 
# asm 1: vadd.i32 >g89_19=reg128#11,<g89_19=reg128#11,<g89=reg128#3
# asm 2: vadd.i32 >g89_19=q10,<g89_19=q10,<g89=q2
vadd.i32 q10,q10,q2

# qhasm:   f89 = f89[0] f89[2] f89[1] f89[3] 
# asm 1: vtrn.32 <f89=reg128#13%bot,<f89=reg128#13%top
# asm 2: vtrn.32 <f89=d24,<f89=d25
vtrn.32 d24,d25

# qhasm:   mem128[ptr] aligned= g13_19 
# asm 1: vst1.8 {<g13_19=reg128#7%bot-<g13_19=reg128#7%top},[<ptr=int32#3,: 128]
# asm 2: vst1.8 {<g13_19=d12-<g13_19=d13},[<ptr=r2,: 128]
vst1.8 {d12-d13},[r2,: 128]

# qhasm:   4x f13_2 = f13 << 1 
# asm 1: vshl.i32 >f13_2=reg128#7,<f13=reg128#14,#1
# asm 2: vshl.i32 >f13_2=q6,<f13=q13,#1
vshl.i32 q6,q13,#1

# qhasm:   new g89_19_stack 

# qhasm:   ptr = &g89_19_stack 
# asm 1: lea >ptr=int32#3,<g89_19_stack=stack128#5
# asm 2: lea >ptr=r2,<g89_19_stack=[sp,#576]
add r2,sp,#576

# qhasm:   mem128[ptr] aligned= g89_19 
# asm 1: vst1.8 {<g89_19=reg128#11%bot-<g89_19=reg128#11%top},[<ptr=int32#3,: 128]
# asm 2: vst1.8 {<g89_19=d20-<g89_19=d21},[<ptr=r2,: 128]
vst1.8 {d20-d21},[r2,: 128]

# qhasm:   4x f57_2 = f57 << 1 
# asm 1: vshl.i32 >f57_2=reg128#11,<f57=reg128#15,#1
# asm 2: vshl.i32 >f57_2=q10,<f57=q14,#1
vshl.i32 q10,q14,#1

# qhasm:   new f13_2_stack 

# qhasm:   ptr = &f13_2_stack 
# asm 1: lea >ptr=int32#3,<f13_2_stack=stack128#6
# asm 2: lea >ptr=r2,<f13_2_stack=[sp,#592]
add r2,sp,#592

# qhasm:   mem128[ptr] aligned= f13_2 
# asm 1: vst1.8 {<f13_2=reg128#7%bot-<f13_2=reg128#7%top},[<ptr=int32#3,: 128]
# asm 2: vst1.8 {<f13_2=d12-<f13_2=d13},[<ptr=r2,: 128]
vst1.8 {d12-d13},[r2,: 128]

# qhasm:   4x f89_2 = f89 << 1 
# asm 1: vshl.i32 >f89_2=reg128#16,<f89=reg128#13,#1
# asm 2: vshl.i32 >f89_2=q15,<f89=q12,#1
vshl.i32 q15,q12,#1

# qhasm:   4x g57_19 += g57 
# asm 1: vadd.i32 >g57_19=reg128#9,<g57_19=reg128#9,<g57=reg128#5
# asm 2: vadd.i32 >g57_19=q8,<g57_19=q8,<g57=q4
vadd.i32 q8,q8,q4

# qhasm:   mix = f89_2[2,3] mix[2,3] 
# asm 1: vext.32 <mix=reg128#6%bot,<f89_2=reg128#16%top,<f89_2=reg128#16%bot,#0
# asm 2: vext.32 <mix=d10,<f89_2=d31,<f89_2=d30,#0
vext.32 d10,d31,d30,#0

# qhasm:   4x g46_19 += g46 
# asm 1: vadd.i32 >g46_19=reg128#8,<g46_19=reg128#8,<g46=reg128#2
# asm 2: vadd.i32 >g46_19=q7,<g46_19=q7,<g46=q1
vadd.i32 q7,q7,q1

# qhasm:   new g57_19_stack 

# qhasm:   ptr = &g57_19_stack 
# asm 1: lea >ptr=int32#3,<g57_19_stack=stack128#7
# asm 2: lea >ptr=r2,<g57_19_stack=[sp,#608]
add r2,sp,#608

# qhasm:   mem128[ptr] aligned= g57_19 
# asm 1: vst1.8 {<g57_19=reg128#9%bot-<g57_19=reg128#9%top},[<ptr=int32#3,: 128]
# asm 2: vst1.8 {<g57_19=d16-<g57_19=d17},[<ptr=r2,: 128]
vst1.8 {d16-d17},[r2,: 128]

# qhasm:   h9[0,1] = f02[0] signed* g89[2];h9[2,3] = f02[1] signed* g89[3] 
# asm 1: vmull.s32 >h9=reg128#9,<f02=reg128#10%bot,<g89=reg128#3%top
# asm 2: vmull.s32 >h9=q8,<f02=d18,<g89=d5
vmull.s32 q8,d18,d5

# qhasm:   h9[0,1] += f13[0] signed* g89[0];h9[2,3] += f13[1] signed* g89[1] 
# asm 1: vmlal.s32 <h9=reg128#9,<f13=reg128#14%bot,<g89=reg128#3%bot
# asm 2: vmlal.s32 <h9=q8,<f13=d26,<g89=d4
vmlal.s32 q8,d26,d4

# qhasm:   h9[0,1] += f02[2] signed* g57[2];h9[2,3] += f02[3] signed* g57[3] 
# asm 1: vmlal.s32 <h9=reg128#9,<f02=reg128#10%top,<g57=reg128#5%top
# asm 2: vmlal.s32 <h9=q8,<f02=d19,<g57=d9
vmlal.s32 q8,d19,d9

# qhasm:   h9[0,1] += f13[2] signed* g46[2];h9[2,3] += f13[3] signed* g46[3] 
# asm 1: vmlal.s32 <h9=reg128#9,<f13=reg128#14%top,<g46=reg128#2%top
# asm 2: vmlal.s32 <h9=q8,<f13=d27,<g46=d3
vmlal.s32 q8,d27,d3

# qhasm:   h9[0,1] += f46[0] signed* g57[0];h9[2,3] += f46[1] signed* g57[1] 
# asm 1: vmlal.s32 <h9=reg128#9,<f46=reg128#12%bot,<g57=reg128#5%bot
# asm 2: vmlal.s32 <h9=q8,<f46=d22,<g57=d8
vmlal.s32 q8,d22,d8

# qhasm:   h9[0,1] += f57[0] signed* g46[0];h9[2,3] += f57[1] signed* g46[1] 
# asm 1: vmlal.s32 <h9=reg128#9,<f57=reg128#15%bot,<g46=reg128#2%bot
# asm 2: vmlal.s32 <h9=q8,<f57=d28,<g46=d2
vmlal.s32 q8,d28,d2

# qhasm:   h9[0,1] += f46[2] signed* g13[2];h9[2,3] += f46[3] signed* g13[3] 
# asm 1: vmlal.s32 <h9=reg128#9,<f46=reg128#12%top,<g13=reg128#4%top
# asm 2: vmlal.s32 <h9=q8,<f46=d23,<g13=d7
vmlal.s32 q8,d23,d7

# qhasm:   h9[0,1] += f57[2] signed* g02[2];h9[2,3] += f57[3] signed* g02[3] 
# asm 1: vmlal.s32 <h9=reg128#9,<f57=reg128#15%top,<g02=reg128#1%top
# asm 2: vmlal.s32 <h9=q8,<f57=d29,<g02=d1
vmlal.s32 q8,d29,d1

# qhasm:   h9[0,1] += f89[0] signed* g13[0];h9[2,3] += f89[1] signed* g13[1] 
# asm 1: vmlal.s32 <h9=reg128#9,<f89=reg128#13%bot,<g13=reg128#4%bot
# asm 2: vmlal.s32 <h9=q8,<f89=d24,<g13=d6
vmlal.s32 q8,d24,d6

# qhasm:   h9[0,1] += f89[2] signed* g02[0];h9[2,3] += f89[3] signed* g02[1] 
# asm 1: vmlal.s32 <h9=reg128#9,<f89=reg128#13%top,<g02=reg128#1%bot
# asm 2: vmlal.s32 <h9=q8,<f89=d25,<g02=d0
vmlal.s32 q8,d25,d0

# qhasm:   new g46_19_stack 

# qhasm:   ptr = &g46_19_stack 
# asm 1: lea >ptr=int32#3,<g46_19_stack=stack128#8
# asm 2: lea >ptr=r2,<g46_19_stack=[sp,#624]
add r2,sp,#624

# qhasm:   mem128[ptr] aligned= g46_19 
# asm 1: vst1.8 {<g46_19=reg128#8%bot-<g46_19=reg128#8%top},[<ptr=int32#3,: 128]
# asm 2: vst1.8 {<g46_19=d14-<g46_19=d15},[<ptr=r2,: 128]
vst1.8 {d14-d15},[r2,: 128]

# qhasm:   h8[0,1] = f02[0] signed* g89[0];h8[2,3] = f02[1] signed* g89[1] 
# asm 1: vmull.s32 >h8=reg128#3,<f02=reg128#10%bot,<g89=reg128#3%bot
# asm 2: vmull.s32 >h8=q2,<f02=d18,<g89=d4
vmull.s32 q2,d18,d4

# qhasm:   h8[0,1] += f13_2[0] signed* g57[2];h8[2,3] += f13_2[1] signed* g57[3] 
# asm 1: vmlal.s32 <h8=reg128#3,<f13_2=reg128#7%bot,<g57=reg128#5%top
# asm 2: vmlal.s32 <h8=q2,<f13_2=d12,<g57=d9
vmlal.s32 q2,d12,d9

# qhasm:   h8[0,1] += f13_2[2] signed* g57[0];h8[2,3] += f13_2[3] signed* g57[1] 
# asm 1: vmlal.s32 <h8=reg128#3,<f13_2=reg128#7%top,<g57=reg128#5%bot
# asm 2: vmlal.s32 <h8=q2,<f13_2=d13,<g57=d8
vmlal.s32 q2,d13,d8

# qhasm:   h8[0,1] += f02[2] signed* g46[2];h8[2,3] += f02[3] signed* g46[3] 
# asm 1: vmlal.s32 <h8=reg128#3,<f02=reg128#10%top,<g46=reg128#2%top
# asm 2: vmlal.s32 <h8=q2,<f02=d19,<g46=d3
vmlal.s32 q2,d19,d3

# qhasm:   h8[0,1] += f46[0] signed* g46[0];h8[2,3] += f46[1] signed* g46[1] 
# asm 1: vmlal.s32 <h8=reg128#3,<f46=reg128#12%bot,<g46=reg128#2%bot
# asm 2: vmlal.s32 <h8=q2,<f46=d22,<g46=d2
vmlal.s32 q2,d22,d2

# qhasm:   h8[0,1] += f46[2] signed* g02[2];h8[2,3] += f46[3] signed* g02[3] 
# asm 1: vmlal.s32 <h8=reg128#3,<f46=reg128#12%top,<g02=reg128#1%top
# asm 2: vmlal.s32 <h8=q2,<f46=d23,<g02=d1
vmlal.s32 q2,d23,d1

# qhasm:   h8[0,1] += f89[0] signed* g02[0];h8[2,3] += f89[1] signed* g02[1] 
# asm 1: vmlal.s32 <h8=reg128#3,<f89=reg128#13%bot,<g02=reg128#1%bot
# asm 2: vmlal.s32 <h8=q2,<f89=d24,<g02=d0
vmlal.s32 q2,d24,d0

# qhasm:   new f57_2_stack 

# qhasm:   ptr = &f57_2_stack 
# asm 1: lea >ptr=int32#3,<f57_2_stack=stack128#9
# asm 2: lea >ptr=r2,<f57_2_stack=[sp,#640]
add r2,sp,#640

# qhasm:   mem128[ptr] aligned= f57_2 
# asm 1: vst1.8 {<f57_2=reg128#11%bot-<f57_2=reg128#11%top},[<ptr=int32#3,: 128]
# asm 2: vst1.8 {<f57_2=d20-<f57_2=d21},[<ptr=r2,: 128]
vst1.8 {d20-d21},[r2,: 128]

# qhasm:   h7[0,1] = f02[0] signed* g57[2];h7[2,3] = f02[1] signed* g57[3] 
# asm 1: vmull.s32 >h7=reg128#8,<f02=reg128#10%bot,<g57=reg128#5%top
# asm 2: vmull.s32 >h7=q7,<f02=d18,<g57=d9
vmull.s32 q7,d18,d9

# qhasm:   h7[0,1] += f13[0] signed* g46[2];h7[2,3] += f13[1] signed* g46[3] 
# asm 1: vmlal.s32 <h7=reg128#8,<f13=reg128#14%bot,<g46=reg128#2%top
# asm 2: vmlal.s32 <h7=q7,<f13=d26,<g46=d3
vmlal.s32 q7,d26,d3

# qhasm:   h7[0,1] += f02[2] signed* g57[0];h7[2,3] += f02[3] signed* g57[1] 
# asm 1: vmlal.s32 <h7=reg128#8,<f02=reg128#10%top,<g57=reg128#5%bot
# asm 2: vmlal.s32 <h7=q7,<f02=d19,<g57=d8
vmlal.s32 q7,d19,d8

# qhasm:   h7[0,1] += f13[2] signed* g46[0];h7[2,3] += f13[3] signed* g46[1] 
# asm 1: vmlal.s32 <h7=reg128#8,<f13=reg128#14%top,<g46=reg128#2%bot
# asm 2: vmlal.s32 <h7=q7,<f13=d27,<g46=d2
vmlal.s32 q7,d27,d2

# qhasm:   h7[0,1] += f46[0] signed* g13[2];h7[2,3] += f46[1] signed* g13[3] 
# asm 1: vmlal.s32 <h7=reg128#8,<f46=reg128#12%bot,<g13=reg128#4%top
# asm 2: vmlal.s32 <h7=q7,<f46=d22,<g13=d7
vmlal.s32 q7,d22,d7

# qhasm:   h7[0,1] += f57[0] signed* g02[2];h7[2,3] += f57[1] signed* g02[3] 
# asm 1: vmlal.s32 <h7=reg128#8,<f57=reg128#15%bot,<g02=reg128#1%top
# asm 2: vmlal.s32 <h7=q7,<f57=d28,<g02=d1
vmlal.s32 q7,d28,d1

# qhasm:   h7[0,1] += f46[2] signed* g13[0];h7[2,3] += f46[3] signed* g13[1] 
# asm 1: vmlal.s32 <h7=reg128#8,<f46=reg128#12%top,<g13=reg128#4%bot
# asm 2: vmlal.s32 <h7=q7,<f46=d23,<g13=d6
vmlal.s32 q7,d23,d6

# qhasm:   h7[0,1] += f57[2] signed* g02[0];h7[2,3] += f57[3] signed* g02[1] 
# asm 1: vmlal.s32 <h7=reg128#8,<f57=reg128#15%top,<g02=reg128#1%bot
# asm 2: vmlal.s32 <h7=q7,<f57=d29,<g02=d0
vmlal.s32 q7,d29,d0

# qhasm:   new mix_stack 

# qhasm:   ptr = &mix_stack 
# asm 1: lea >ptr=int32#3,<mix_stack=stack128#10
# asm 2: lea >ptr=r2,<mix_stack=[sp,#656]
add r2,sp,#656

# qhasm:   mem128[ptr] aligned= mix 
# asm 1: vst1.8 {<mix=reg128#6%bot-<mix=reg128#6%top},[<ptr=int32#3,: 128]
# asm 2: vst1.8 {<mix=d10-<mix=d11},[<ptr=r2,: 128]
vst1.8 {d10-d11},[r2,: 128]

# qhasm:   h6[0,1] = f02[0] signed* g46[2];h6[2,3] = f02[1] signed* g46[3] 
# asm 1: vmull.s32 >h6=reg128#6,<f02=reg128#10%bot,<g46=reg128#2%top
# asm 2: vmull.s32 >h6=q5,<f02=d18,<g46=d3
vmull.s32 q5,d18,d3

# qhasm:   h6[0,1] += f02[2] signed* g46[0];h6[2,3] += f02[3] signed* g46[1] 
# asm 1: vmlal.s32 <h6=reg128#6,<f02=reg128#10%top,<g46=reg128#2%bot
# asm 2: vmlal.s32 <h6=q5,<f02=d19,<g46=d2
vmlal.s32 q5,d19,d2

# qhasm:   h6[0,1] += f46[0] signed* g02[2];h6[2,3] += f46[1] signed* g02[3] 
# asm 1: vmlal.s32 <h6=reg128#6,<f46=reg128#12%bot,<g02=reg128#1%top
# asm 2: vmlal.s32 <h6=q5,<f46=d22,<g02=d1
vmlal.s32 q5,d22,d1

# qhasm:   h6[0,1] += f46[2] signed* g02[0];h6[2,3] += f46[3] signed* g02[1] 
# asm 1: vmlal.s32 <h6=reg128#6,<f46=reg128#12%top,<g02=reg128#1%bot
# asm 2: vmlal.s32 <h6=q5,<f46=d23,<g02=d0
vmlal.s32 q5,d23,d0

# qhasm:   h6[0,1] += f13_2[0] signed* g57[0];h6[2,3] += f13_2[1] signed* g57[1] 
# asm 1: vmlal.s32 <h6=reg128#6,<f13_2=reg128#7%bot,<g57=reg128#5%bot
# asm 2: vmlal.s32 <h6=q5,<f13_2=d12,<g57=d8
vmlal.s32 q5,d12,d8

# qhasm:   new h9_stack 

# qhasm:   ptr = &h9_stack 
# asm 1: lea >ptr=int32#3,<h9_stack=stack128#11
# asm 2: lea >ptr=r2,<h9_stack=[sp,#672]
add r2,sp,#672

# qhasm:   mem128[ptr] aligned= h9 
# asm 1: vst1.8 {<h9=reg128#9%bot-<h9=reg128#9%top},[<ptr=int32#3,: 128]
# asm 2: vst1.8 {<h9=d16-<h9=d17},[<ptr=r2,: 128]
vst1.8 {d16-d17},[r2,: 128]

# qhasm:   h5[0,1] = f02[0] signed* g57[0];h5[2,3] = f02[1] signed* g57[1] 
# asm 1: vmull.s32 >h5=reg128#5,<f02=reg128#10%bot,<g57=reg128#5%bot
# asm 2: vmull.s32 >h5=q4,<f02=d18,<g57=d8
vmull.s32 q4,d18,d8

# qhasm:   h5[0,1] += f13[0] signed* g46[0];h5[2,3] += f13[1] signed* g46[1] 
# asm 1: vmlal.s32 <h5=reg128#5,<f13=reg128#14%bot,<g46=reg128#2%bot
# asm 2: vmlal.s32 <h5=q4,<f13=d26,<g46=d2
vmlal.s32 q4,d26,d2

# qhasm:   h5[0,1] += f02[2] signed* g13[2];h5[2,3] += f02[3] signed* g13[3] 
# asm 1: vmlal.s32 <h5=reg128#5,<f02=reg128#10%top,<g13=reg128#4%top
# asm 2: vmlal.s32 <h5=q4,<f02=d19,<g13=d7
vmlal.s32 q4,d19,d7

# qhasm:   h5[0,1] += f13[2] signed* g02[2];h5[2,3] += f13[3] signed* g02[3] 
# asm 1: vmlal.s32 <h5=reg128#5,<f13=reg128#14%top,<g02=reg128#1%top
# asm 2: vmlal.s32 <h5=q4,<f13=d27,<g02=d1
vmlal.s32 q4,d27,d1

# qhasm:   h5[0,1] += f46[0] signed* g13[0];h5[2,3] += f46[1] signed* g13[1] 
# asm 1: vmlal.s32 <h5=reg128#5,<f46=reg128#12%bot,<g13=reg128#4%bot
# asm 2: vmlal.s32 <h5=q4,<f46=d22,<g13=d6
vmlal.s32 q4,d22,d6

# qhasm:   h5[0,1] += f57[0] signed* g02[0];h5[2,3] += f57[1] signed* g02[1] 
# asm 1: vmlal.s32 <h5=reg128#5,<f57=reg128#15%bot,<g02=reg128#1%bot
# asm 2: vmlal.s32 <h5=q4,<f57=d28,<g02=d0
vmlal.s32 q4,d28,d0

# qhasm:   h3[0,1] = f02[0] signed* g13[2];h3[2,3] = f02[1] signed* g13[3] 
# asm 1: vmull.s32 >h3=reg128#9,<f02=reg128#10%bot,<g13=reg128#4%top
# asm 2: vmull.s32 >h3=q8,<f02=d18,<g13=d7
vmull.s32 q8,d18,d7

# qhasm:   h3[0,1] += f13[0] signed* g02[2];h3[2,3] += f13[1] signed* g02[3] 
# asm 1: vmlal.s32 <h3=reg128#9,<f13=reg128#14%bot,<g02=reg128#1%top
# asm 2: vmlal.s32 <h3=q8,<f13=d26,<g02=d1
vmlal.s32 q8,d26,d1

# qhasm:   h3[0,1] += f02[2] signed* g13[0];h3[2,3] += f02[3] signed* g13[1] 
# asm 1: vmlal.s32 <h3=reg128#9,<f02=reg128#10%top,<g13=reg128#4%bot
# asm 2: vmlal.s32 <h3=q8,<f02=d19,<g13=d6
vmlal.s32 q8,d19,d6

# qhasm:   h3[0,1] += f13[2] signed* g02[0];h3[2,3] += f13[3] signed* g02[1] 
# asm 1: vmlal.s32 <h3=reg128#9,<f13=reg128#14%top,<g02=reg128#1%bot
# asm 2: vmlal.s32 <h3=q8,<f13=d27,<g02=d0
vmlal.s32 q8,d27,d0

# qhasm:   ptr = &g89_19_stack 
# asm 1: lea >ptr=int32#3,<g89_19_stack=stack128#5
# asm 2: lea >ptr=r2,<g89_19_stack=[sp,#576]
add r2,sp,#576

# qhasm:   g89_19 aligned= mem128[ptr] 
# asm 1: vld1.8 {>g89_19=reg128#11%bot->g89_19=reg128#11%top},[<ptr=int32#3,: 128]
# asm 2: vld1.8 {>g89_19=d20->g89_19=d21},[<ptr=r2,: 128]
vld1.8 {d20-d21},[r2,: 128]

# qhasm:   h7[0,1] += f89[0] signed* g89_19[2];h7[2,3] += f89[1] signed* g89_19[3] 
# asm 1: vmlal.s32 <h7=reg128#8,<f89=reg128#13%bot,<g89_19=reg128#11%top
# asm 2: vmlal.s32 <h7=q7,<f89=d24,<g89_19=d21
vmlal.s32 q7,d24,d21

# qhasm:   h7[0,1] += f89[2] signed* g89_19[0];h7[2,3] += f89[3] signed* g89_19[1] 
# asm 1: vmlal.s32 <h7=reg128#8,<f89=reg128#13%top,<g89_19=reg128#11%bot
# asm 2: vmlal.s32 <h7=q7,<f89=d25,<g89_19=d20
vmlal.s32 q7,d25,d20

# qhasm:   h5[0,1] += f46[2] signed* g89_19[2];h5[2,3] += f46[3] signed* g89_19[3] 
# asm 1: vmlal.s32 <h5=reg128#5,<f46=reg128#12%top,<g89_19=reg128#11%top
# asm 2: vmlal.s32 <h5=q4,<f46=d23,<g89_19=d21
vmlal.s32 q4,d23,d21

# qhasm:   h5[0,1] += f57[2] signed* g89_19[0];h5[2,3] += f57[3] signed* g89_19[1] 
# asm 1: vmlal.s32 <h5=reg128#5,<f57=reg128#15%top,<g89_19=reg128#11%bot
# asm 2: vmlal.s32 <h5=q4,<f57=d29,<g89_19=d20
vmlal.s32 q4,d29,d20

# qhasm:   h3[0,1] += f46[0] signed* g89_19[2];h3[2,3] += f46[1] signed* g89_19[3] 
# asm 1: vmlal.s32 <h3=reg128#9,<f46=reg128#12%bot,<g89_19=reg128#11%top
# asm 2: vmlal.s32 <h3=q8,<f46=d22,<g89_19=d21
vmlal.s32 q8,d22,d21

# qhasm:   h3[0,1] += f57[0] signed* g89_19[0];h3[2,3] += f57[1] signed* g89_19[1] 
# asm 1: vmlal.s32 <h3=reg128#9,<f57=reg128#15%bot,<g89_19=reg128#11%bot
# asm 2: vmlal.s32 <h3=q8,<f57=d28,<g89_19=d20
vmlal.s32 q8,d28,d20

# qhasm:   h6[0,1] += f89[0] signed* g89_19[0];h6[2,3] += f89[1] signed* g89_19[1] 
# asm 1: vmlal.s32 <h6=reg128#6,<f89=reg128#13%bot,<g89_19=reg128#11%bot
# asm 2: vmlal.s32 <h6=q5,<f89=d24,<g89_19=d20
vmlal.s32 q5,d24,d20

# qhasm:   new h7_stack 

# qhasm:   ptr = &h7_stack 
# asm 1: lea >ptr=int32#3,<h7_stack=stack128#5
# asm 2: lea >ptr=r2,<h7_stack=[sp,#576]
add r2,sp,#576

# qhasm:   mem128[ptr] aligned= h7 
# asm 1: vst1.8 {<h7=reg128#8%bot-<h7=reg128#8%top},[<ptr=int32#3,: 128]
# asm 2: vst1.8 {<h7=d14-<h7=d15},[<ptr=r2,: 128]
vst1.8 {d14-d15},[r2,: 128]

# qhasm:   h1[0,1] = f02[0] signed* g13[0];h1[2,3] = f02[1] signed* g13[1] 
# asm 1: vmull.s32 >h1=reg128#8,<f02=reg128#10%bot,<g13=reg128#4%bot
# asm 2: vmull.s32 >h1=q7,<f02=d18,<g13=d6
vmull.s32 q7,d18,d6

# qhasm:   h1[0,1] += f13[0] signed* g02[0];h1[2,3] += f13[1] signed* g02[1] 
# asm 1: vmlal.s32 <h1=reg128#8,<f13=reg128#14%bot,<g02=reg128#1%bot
# asm 2: vmlal.s32 <h1=q7,<f13=d26,<g02=d0
vmlal.s32 q7,d26,d0

# qhasm:   ptr = &mix_stack 
# asm 1: lea >ptr=int32#3,<mix_stack=stack128#10
# asm 2: lea >ptr=r2,<mix_stack=[sp,#656]
add r2,sp,#656

# qhasm:   mix aligned= mem128[ptr] 
# asm 1: vld1.8 {>mix=reg128#16%bot->mix=reg128#16%top},[<ptr=int32#3,: 128]
# asm 2: vld1.8 {>mix=d30->mix=d31},[<ptr=r2,: 128]
vld1.8 {d30-d31},[r2,: 128]

# qhasm:   h8[0,1] += mix[0] signed* g89_19[2];h8[2,3] += mix[1] signed* g89_19[3] 
# asm 1: vmlal.s32 <h8=reg128#3,<mix=reg128#16%bot,<g89_19=reg128#11%top
# asm 2: vmlal.s32 <h8=q2,<mix=d30,<g89_19=d21
vmlal.s32 q2,d30,d21

# qhasm:   h1[0,1] += f02[2] signed* g89_19[2];h1[2,3] += f02[3] signed* g89_19[3] 
# asm 1: vmlal.s32 <h1=reg128#8,<f02=reg128#10%top,<g89_19=reg128#11%top
# asm 2: vmlal.s32 <h1=q7,<f02=d19,<g89_19=d21
vmlal.s32 q7,d19,d21

# qhasm:   h1[0,1] += f13[2] signed* g89_19[0];h1[2,3] += f13[3] signed* g89_19[1] 
# asm 1: vmlal.s32 <h1=reg128#8,<f13=reg128#14%top,<g89_19=reg128#11%bot
# asm 2: vmlal.s32 <h1=q7,<f13=d27,<g89_19=d20
vmlal.s32 q7,d27,d20

# qhasm:   ptr = &g46_19_stack 
# asm 1: lea >ptr=int32#3,<g46_19_stack=stack128#8
# asm 2: lea >ptr=r2,<g46_19_stack=[sp,#624]
add r2,sp,#624

# qhasm:   g46_19 aligned= mem128[ptr] 
# asm 1: vld1.8 {>g46_19=reg128#14%bot->g46_19=reg128#14%top},[<ptr=int32#3,: 128]
# asm 2: vld1.8 {>g46_19=d26->g46_19=d27},[<ptr=r2,: 128]
vld1.8 {d26-d27},[r2,: 128]

# qhasm:   h5[0,1] += f89[2] signed* g46_19[2];h5[2,3] += f89[3] signed* g46_19[3] 
# asm 1: vmlal.s32 <h5=reg128#5,<f89=reg128#13%top,<g46_19=reg128#14%top
# asm 2: vmlal.s32 <h5=q4,<f89=d25,<g46_19=d27
vmlal.s32 q4,d25,d27

# qhasm:   h3[0,1] += f57[2] signed* g46_19[2];h3[2,3] += f57[3] signed* g46_19[3] 
# asm 1: vmlal.s32 <h3=reg128#9,<f57=reg128#15%top,<g46_19=reg128#14%top
# asm 2: vmlal.s32 <h3=q8,<f57=d29,<g46_19=d27
vmlal.s32 q8,d29,d27

# qhasm:   h3[0,1] += f89[2] signed* g46_19[0];h3[2,3] += f89[3] signed* g46_19[1] 
# asm 1: vmlal.s32 <h3=reg128#9,<f89=reg128#13%top,<g46_19=reg128#14%bot
# asm 2: vmlal.s32 <h3=q8,<f89=d25,<g46_19=d26
vmlal.s32 q8,d25,d26

# qhasm:   h1[0,1] += f57[0] signed* g46_19[2];h1[2,3] += f57[1] signed* g46_19[3] 
# asm 1: vmlal.s32 <h1=reg128#8,<f57=reg128#15%bot,<g46_19=reg128#14%top
# asm 2: vmlal.s32 <h1=q7,<f57=d28,<g46_19=d27
vmlal.s32 q7,d28,d27

# qhasm:   h1[0,1] += f57[2] signed* g46_19[0];h1[2,3] += f57[3] signed* g46_19[1] 
# asm 1: vmlal.s32 <h1=reg128#8,<f57=reg128#15%top,<g46_19=reg128#14%bot
# asm 2: vmlal.s32 <h1=q7,<f57=d29,<g46_19=d26
vmlal.s32 q7,d29,d26

# qhasm:   ptr = &g57_19_stack 
# asm 1: lea >ptr=int32#3,<g57_19_stack=stack128#7
# asm 2: lea >ptr=r2,<g57_19_stack=[sp,#608]
add r2,sp,#608

# qhasm:   g57_19 aligned= mem128[ptr] 
# asm 1: vld1.8 {>g57_19=reg128#15%bot->g57_19=reg128#15%top},[<ptr=int32#3,: 128]
# asm 2: vld1.8 {>g57_19=d28->g57_19=d29},[<ptr=r2,: 128]
vld1.8 {d28-d29},[r2,: 128]

# qhasm:   h5[0,1] += f89[0] signed* g57_19[2];h5[2,3] += f89[1] signed* g57_19[3] 
# asm 1: vmlal.s32 <h5=reg128#5,<f89=reg128#13%bot,<g57_19=reg128#15%top
# asm 2: vmlal.s32 <h5=q4,<f89=d24,<g57_19=d29
vmlal.s32 q4,d24,d29

# qhasm:   h3[0,1] += f46[2] signed* g57_19[2];h3[2,3] += f46[3] signed* g57_19[3] 
# asm 1: vmlal.s32 <h3=reg128#9,<f46=reg128#12%top,<g57_19=reg128#15%top
# asm 2: vmlal.s32 <h3=q8,<f46=d23,<g57_19=d29
vmlal.s32 q8,d23,d29

# qhasm:   h3[0,1] += f89[0] signed* g57_19[0];h3[2,3] += f89[1] signed* g57_19[1] 
# asm 1: vmlal.s32 <h3=reg128#9,<f89=reg128#13%bot,<g57_19=reg128#15%bot
# asm 2: vmlal.s32 <h3=q8,<f89=d24,<g57_19=d28
vmlal.s32 q8,d24,d28

# qhasm:   h1[0,1] += f46[0] signed* g57_19[2];h1[2,3] += f46[1] signed* g57_19[3] 
# asm 1: vmlal.s32 <h1=reg128#8,<f46=reg128#12%bot,<g57_19=reg128#15%top
# asm 2: vmlal.s32 <h1=q7,<f46=d22,<g57_19=d29
vmlal.s32 q7,d22,d29

# qhasm:   h1[0,1] += f46[2] signed* g57_19[0];h1[2,3] += f46[3] signed* g57_19[1] 
# asm 1: vmlal.s32 <h1=reg128#8,<f46=reg128#12%top,<g57_19=reg128#15%bot
# asm 2: vmlal.s32 <h1=q7,<f46=d23,<g57_19=d28
vmlal.s32 q7,d23,d28

# qhasm:   new h5_stack 

# qhasm:   ptr = &h5_stack 
# asm 1: lea >ptr=int32#3,<h5_stack=stack128#7
# asm 2: lea >ptr=r2,<h5_stack=[sp,#608]
add r2,sp,#608

# qhasm:   mem128[ptr] aligned= h5 
# asm 1: vst1.8 {<h5=reg128#5%bot-<h5=reg128#5%top},[<ptr=int32#3,: 128]
# asm 2: vst1.8 {<h5=d8-<h5=d9},[<ptr=r2,: 128]
vst1.8 {d8-d9},[r2,: 128]

# qhasm:   ptr = &g13_19_stack 
# asm 1: lea >ptr=int32#3,<g13_19_stack=stack128#4
# asm 2: lea >ptr=r2,<g13_19_stack=[sp,#560]
add r2,sp,#560

# qhasm:   g13_19 aligned= mem128[ptr] 
# asm 1: vld1.8 {>g13_19=reg128#5%bot->g13_19=reg128#5%top},[<ptr=int32#3,: 128]
# asm 2: vld1.8 {>g13_19=d8->g13_19=d9},[<ptr=r2,: 128]
vld1.8 {d8-d9},[r2,: 128]

# qhasm:   h1[0,1] += f89[0] signed* g13_19[2];h1[2,3] += f89[1] signed* g13_19[3] 
# asm 1: vmlal.s32 <h1=reg128#8,<f89=reg128#13%bot,<g13_19=reg128#5%top
# asm 2: vmlal.s32 <h1=q7,<f89=d24,<g13_19=d9
vmlal.s32 q7,d24,d9

# qhasm:   h1[0,1] += f89[2] signed* mix[2];h1[2,3] += f89[3] signed* mix[3] 
# asm 1: vmlal.s32 <h1=reg128#8,<f89=reg128#13%top,<mix=reg128#16%top
# asm 2: vmlal.s32 <h1=q7,<f89=d25,<mix=d31
vmlal.s32 q7,d25,d31

# qhasm:   h4[0,1] = f02[0] signed* g46[0];h4[2,3] = f02[1] signed* g46[1] 
# asm 1: vmull.s32 >h4=reg128#2,<f02=reg128#10%bot,<g46=reg128#2%bot
# asm 2: vmull.s32 >h4=q1,<f02=d18,<g46=d2
vmull.s32 q1,d18,d2

# qhasm:   h4[0,1] += f02[2] signed* g02[2];h4[2,3] += f02[3] signed* g02[3] 
# asm 1: vmlal.s32 <h4=reg128#2,<f02=reg128#10%top,<g02=reg128#1%top
# asm 2: vmlal.s32 <h4=q1,<f02=d19,<g02=d1
vmlal.s32 q1,d19,d1

# qhasm:   h4[0,1] += f46[0] signed* g02[0];h4[2,3] += f46[1] signed* g02[1] 
# asm 1: vmlal.s32 <h4=reg128#2,<f46=reg128#12%bot,<g02=reg128#1%bot
# asm 2: vmlal.s32 <h4=q1,<f46=d22,<g02=d0
vmlal.s32 q1,d22,d0

# qhasm:   h4[0,1] += f89[0] signed* g46_19[2];h4[2,3] += f89[1] signed* g46_19[3] 
# asm 1: vmlal.s32 <h4=reg128#2,<f89=reg128#13%bot,<g46_19=reg128#14%top
# asm 2: vmlal.s32 <h4=q1,<f89=d24,<g46_19=d27
vmlal.s32 q1,d24,d27

# qhasm:   h4[0,1] += f46[2] signed* g89_19[0];h4[2,3] += f46[3] signed* g89_19[1] 
# asm 1: vmlal.s32 <h4=reg128#2,<f46=reg128#12%top,<g89_19=reg128#11%bot
# asm 2: vmlal.s32 <h4=q1,<f46=d23,<g89_19=d20
vmlal.s32 q1,d23,d20

# qhasm:   h4[0,1] += f13_2[0] signed* g13[2];h4[2,3] += f13_2[1] signed* g13[3] 
# asm 1: vmlal.s32 <h4=reg128#2,<f13_2=reg128#7%bot,<g13=reg128#4%top
# asm 2: vmlal.s32 <h4=q1,<f13_2=d12,<g13=d7
vmlal.s32 q1,d12,d7

# qhasm:   h4[0,1] += f13_2[2] signed* g13[0];h4[2,3] += f13_2[3] signed* g13[1] 
# asm 1: vmlal.s32 <h4=reg128#2,<f13_2=reg128#7%top,<g13=reg128#4%bot
# asm 2: vmlal.s32 <h4=q1,<f13_2=d13,<g13=d6
vmlal.s32 q1,d13,d6

# qhasm:   h2[0,1] = f02[0] signed* g02[2];h2[2,3] = f02[1] signed* g02[3] 
# asm 1: vmull.s32 >h2=reg128#7,<f02=reg128#10%bot,<g02=reg128#1%top
# asm 2: vmull.s32 >h2=q6,<f02=d18,<g02=d1
vmull.s32 q6,d18,d1

# qhasm:   h2[0,1] += f02[2] signed* g02[0];h2[2,3] += f02[3] signed* g02[1] 
# asm 1: vmlal.s32 <h2=reg128#7,<f02=reg128#10%top,<g02=reg128#1%bot
# asm 2: vmlal.s32 <h2=q6,<f02=d19,<g02=d0
vmlal.s32 q6,d19,d0

# qhasm:   h2[0,1] += f46[2] signed* g46_19[2];h2[2,3] += f46[3] signed* g46_19[3] 
# asm 1: vmlal.s32 <h2=reg128#7,<f46=reg128#12%top,<g46_19=reg128#14%top
# asm 2: vmlal.s32 <h2=q6,<f46=d23,<g46_19=d27
vmlal.s32 q6,d23,d27

# qhasm:   h2[0,1] += f46[0] signed* g89_19[0];h2[2,3] += f46[1] signed* g89_19[1] 
# asm 1: vmlal.s32 <h2=reg128#7,<f46=reg128#12%bot,<g89_19=reg128#11%bot
# asm 2: vmlal.s32 <h2=q6,<f46=d22,<g89_19=d20
vmlal.s32 q6,d22,d20

# qhasm:   h2[0,1] += f89[0] signed* g46_19[0];h2[2,3] += f89[1] signed* g46_19[1] 
# asm 1: vmlal.s32 <h2=reg128#7,<f89=reg128#13%bot,<g46_19=reg128#14%bot
# asm 2: vmlal.s32 <h2=q6,<f89=d24,<g46_19=d26
vmlal.s32 q6,d24,d26

# qhasm:   h0[0,1] = f02[0] signed* g02[0];h0[2,3] = f02[1] signed* g02[1] 
# asm 1: vmull.s32 >h0=reg128#1,<f02=reg128#10%bot,<g02=reg128#1%bot
# asm 2: vmull.s32 >h0=q0,<f02=d18,<g02=d0
vmull.s32 q0,d18,d0

# qhasm:   h0[0,1] += f46[0] signed* g46_19[2];h0[2,3] += f46[1] signed* g46_19[3] 
# asm 1: vmlal.s32 <h0=reg128#1,<f46=reg128#12%bot,<g46_19=reg128#14%top
# asm 2: vmlal.s32 <h0=q0,<f46=d22,<g46_19=d27
vmlal.s32 q0,d22,d27

# qhasm:   h0[0,1] += f46[2] signed* g46_19[0];h0[2,3] += f46[3] signed* g46_19[1] 
# asm 1: vmlal.s32 <h0=reg128#1,<f46=reg128#12%top,<g46_19=reg128#14%bot
# asm 2: vmlal.s32 <h0=q0,<f46=d23,<g46_19=d26
vmlal.s32 q0,d23,d26

# qhasm:   h0[0,1] += f89[0] signed* mix[2];h0[2,3] += f89[1] signed* mix[3] 
# asm 1: vmlal.s32 <h0=reg128#1,<f89=reg128#13%bot,<mix=reg128#16%top
# asm 2: vmlal.s32 <h0=q0,<f89=d24,<mix=d31
vmlal.s32 q0,d24,d31

# qhasm:   h0[0,1] += f02[2] signed* g89_19[0];h0[2,3] += f02[3] signed* g89_19[1] 
# asm 1: vmlal.s32 <h0=reg128#1,<f02=reg128#10%top,<g89_19=reg128#11%bot
# asm 2: vmlal.s32 <h0=q0,<f02=d19,<g89_19=d20
vmlal.s32 q0,d19,d20

# qhasm:   ptr = &f57_2_stack 
# asm 1: lea >ptr=int32#3,<f57_2_stack=stack128#9
# asm 2: lea >ptr=r2,<f57_2_stack=[sp,#640]
add r2,sp,#640

# qhasm:   f57_2 aligned= mem128[ptr] 
# asm 1: vld1.8 {>f57_2=reg128#10%bot->f57_2=reg128#10%top},[<ptr=int32#3,: 128]
# asm 2: vld1.8 {>f57_2=d18->f57_2=d19},[<ptr=r2,: 128]
vld1.8 {d18-d19},[r2,: 128]

# qhasm:   h8[0,1] += f57_2[0] signed* g13[2];h8[2,3] += f57_2[1] signed* g13[3] 
# asm 1: vmlal.s32 <h8=reg128#3,<f57_2=reg128#10%bot,<g13=reg128#4%top
# asm 2: vmlal.s32 <h8=q2,<f57_2=d18,<g13=d7
vmlal.s32 q2,d18,d7

# qhasm:   h8[0,1] += f57_2[2] signed* g13[0];h8[2,3] += f57_2[3] signed* g13[1] 
# asm 1: vmlal.s32 <h8=reg128#3,<f57_2=reg128#10%top,<g13=reg128#4%bot
# asm 2: vmlal.s32 <h8=q2,<f57_2=d19,<g13=d6
vmlal.s32 q2,d19,d6

# qhasm:   h6[0,1] += f57_2[0] signed* g13[0];h6[2,3] += f57_2[1] signed* g13[1] 
# asm 1: vmlal.s32 <h6=reg128#6,<f57_2=reg128#10%bot,<g13=reg128#4%bot
# asm 2: vmlal.s32 <h6=q5,<f57_2=d18,<g13=d6
vmlal.s32 q5,d18,d6

# qhasm:   h6[0,1] += f57_2[2] signed* g89_19[2];h6[2,3] += f57_2[3] signed* g89_19[3] 
# asm 1: vmlal.s32 <h6=reg128#6,<f57_2=reg128#10%top,<g89_19=reg128#11%top
# asm 2: vmlal.s32 <h6=q5,<f57_2=d19,<g89_19=d21
vmlal.s32 q5,d19,d21

# qhasm:   h4[0,1] += f57_2[0] signed* g89_19[2];h4[2,3] += f57_2[1] signed* g89_19[3] 
# asm 1: vmlal.s32 <h4=reg128#2,<f57_2=reg128#10%bot,<g89_19=reg128#11%top
# asm 2: vmlal.s32 <h4=q1,<f57_2=d18,<g89_19=d21
vmlal.s32 q1,d18,d21

# qhasm:   h4[0,1] += f57_2[2] signed* g57_19[2];h4[2,3] += f57_2[3] signed* g57_19[3] 
# asm 1: vmlal.s32 <h4=reg128#2,<f57_2=reg128#10%top,<g57_19=reg128#15%top
# asm 2: vmlal.s32 <h4=q1,<f57_2=d19,<g57_19=d29
vmlal.s32 q1,d19,d29

# qhasm:   h0[0,1] += f57_2[0] signed* g57_19[0];h0[2,3] += f57_2[1] signed* g57_19[1] 
# asm 1: vmlal.s32 <h0=reg128#1,<f57_2=reg128#10%bot,<g57_19=reg128#15%bot
# asm 2: vmlal.s32 <h0=q0,<f57_2=d18,<g57_19=d28
vmlal.s32 q0,d18,d28

# qhasm:   h0[0,1] += f57_2[2] signed* g13_19[2];h0[2,3] += f57_2[3] signed* g13_19[3] 
# asm 1: vmlal.s32 <h0=reg128#1,<f57_2=reg128#10%top,<g13_19=reg128#5%top
# asm 2: vmlal.s32 <h0=q0,<f57_2=d19,<g13_19=d9
vmlal.s32 q0,d19,d9

# qhasm:   h2[0,1] += f57_2[0] signed* g57_19[2];h2[2,3] += f57_2[1] signed* g57_19[3] 
# asm 1: vmlal.s32 <h2=reg128#7,<f57_2=reg128#10%bot,<g57_19=reg128#15%top
# asm 2: vmlal.s32 <h2=q6,<f57_2=d18,<g57_19=d29
vmlal.s32 q6,d18,d29

# qhasm:   h2[0,1] += f57_2[2] signed* g57_19[0];h2[2,3] += f57_2[3] signed* g57_19[1] 
# asm 1: vmlal.s32 <h2=reg128#7,<f57_2=reg128#10%top,<g57_19=reg128#15%bot
# asm 2: vmlal.s32 <h2=q6,<f57_2=d19,<g57_19=d28
vmlal.s32 q6,d19,d28

# qhasm:   ptr = &f13_2_stack 
# asm 1: lea >ptr=int32#3,<f13_2_stack=stack128#6
# asm 2: lea >ptr=r2,<f13_2_stack=[sp,#592]
add r2,sp,#592

# qhasm:   f13_2 aligned= mem128[ptr] 
# asm 1: vld1.8 {>f13_2=reg128#10%bot->f13_2=reg128#10%top},[<ptr=int32#3,: 128]
# asm 2: vld1.8 {>f13_2=d18->f13_2=d19},[<ptr=r2,: 128]
vld1.8 {d18-d19},[r2,: 128]

# qhasm:   ptr = &_0x2000000_stack 
# asm 1: lea >ptr=int32#3,<_0x2000000_stack=stack128#1
# asm 2: lea >ptr=r2,<_0x2000000_stack=[sp,#512]
add r2,sp,#512

# qhasm:   _0x2000000 aligned= mem128[ptr] 
# asm 1: vld1.8 {>_0x2000000=reg128#12%bot->_0x2000000=reg128#12%top},[<ptr=int32#3,: 128]
# asm 2: vld1.8 {>_0x2000000=d22->_0x2000000=d23},[<ptr=r2,: 128]
vld1.8 {d22-d23},[r2,: 128]

# qhasm:   h6[0,1] += f13_2[2] signed* g13[2];h6[2,3] += f13_2[3] signed* g13[3] 
# asm 1: vmlal.s32 <h6=reg128#6,<f13_2=reg128#10%top,<g13=reg128#4%top
# asm 2: vmlal.s32 <h6=q5,<f13_2=d19,<g13=d7
vmlal.s32 q5,d19,d7

# qhasm:   h0[0,1] += f13_2[0] signed* g89_19[2];h0[2,3] += f13_2[1] signed* g89_19[3] 
# asm 1: vmlal.s32 <h0=reg128#1,<f13_2=reg128#10%bot,<g89_19=reg128#11%top
# asm 2: vmlal.s32 <h0=q0,<f13_2=d18,<g89_19=d21
vmlal.s32 q0,d18,d21

# qhasm:   h0[0,1] += f13_2[2] signed* g57_19[2];h0[2,3] += f13_2[3] signed* g57_19[3] 
# asm 1: vmlal.s32 <h0=reg128#1,<f13_2=reg128#10%top,<g57_19=reg128#15%top
# asm 2: vmlal.s32 <h0=q0,<f13_2=d19,<g57_19=d29
vmlal.s32 q0,d19,d29

# qhasm:   h2[0,1] += f13_2[0] signed* g13[0];h2[2,3] += f13_2[1] signed* g13[1] 
# asm 1: vmlal.s32 <h2=reg128#7,<f13_2=reg128#10%bot,<g13=reg128#4%bot
# asm 2: vmlal.s32 <h2=q6,<f13_2=d18,<g13=d6
vmlal.s32 q6,d18,d6

# qhasm:   ptr = &_0x1000000_stack 
# asm 1: lea >ptr=int32#3,<_0x1000000_stack=stack128#2
# asm 2: lea >ptr=r2,<_0x1000000_stack=[sp,#528]
add r2,sp,#528

# qhasm:   _0x1000000 aligned= mem128[ptr] 
# asm 1: vld1.8 {>_0x1000000=reg128#4%bot->_0x1000000=reg128#4%top},[<ptr=int32#3,: 128]
# asm 2: vld1.8 {>_0x1000000=d6->_0x1000000=d7},[<ptr=r2,: 128]
vld1.8 {d6-d7},[r2,: 128]

# qhasm:   h2[0,1] += f13_2[2] signed* g89_19[2];h2[2,3] += f13_2[3] signed* g89_19[3] 
# asm 1: vmlal.s32 <h2=reg128#7,<f13_2=reg128#10%top,<g89_19=reg128#11%top
# asm 2: vmlal.s32 <h2=q6,<f13_2=d19,<g89_19=d21
vmlal.s32 q6,d19,d21

# qhasm:   ptr = &h7_stack 
# asm 1: lea >ptr=int32#3,<h7_stack=stack128#5
# asm 2: lea >ptr=r2,<h7_stack=[sp,#576]
add r2,sp,#576

# qhasm:   h7 aligned= mem128[ptr] 
# asm 1: vld1.8 {>h7=reg128#10%bot->h7=reg128#10%top},[<ptr=int32#3,: 128]
# asm 2: vld1.8 {>h7=d18->h7=d19},[<ptr=r2,: 128]
vld1.8 {d18-d19},[r2,: 128]

# qhasm:   h0[0,1] += mix[0] signed* g13_19[0];h0[2,3] += mix[1] signed* g13_19[1] 
# asm 1: vmlal.s32 <h0=reg128#1,<mix=reg128#16%bot,<g13_19=reg128#5%bot
# asm 2: vmlal.s32 <h0=q0,<mix=d30,<g13_19=d8
vmlal.s32 q0,d30,d8

# qhasm:   ptr = &h9_stack 
# asm 1: lea >ptr=int32#3,<h9_stack=stack128#11
# asm 2: lea >ptr=r2,<h9_stack=[sp,#672]
add r2,sp,#672

# qhasm:   h9 aligned= mem128[ptr] 
# asm 1: vld1.8 {>h9=reg128#11%bot->h9=reg128#11%top},[<ptr=int32#3,: 128]
# asm 2: vld1.8 {>h9=d20->h9=d21},[<ptr=r2,: 128]
vld1.8 {d20-d21},[r2,: 128]

# qhasm:   h6[0,1] += mix[0] signed* g57_19[2];h6[2,3] += mix[1] signed* g57_19[3] 
# asm 1: vmlal.s32 <h6=reg128#6,<mix=reg128#16%bot,<g57_19=reg128#15%top
# asm 2: vmlal.s32 <h6=q5,<mix=d30,<g57_19=d29
vmlal.s32 q5,d30,d29

# qhasm:   ptr = &h5_stack 
# asm 1: lea >ptr=int32#3,<h5_stack=stack128#7
# asm 2: lea >ptr=r2,<h5_stack=[sp,#608]
add r2,sp,#608

# qhasm:   h5 aligned= mem128[ptr] 
# asm 1: vld1.8 {>h5=reg128#13%bot->h5=reg128#13%top},[<ptr=int32#3,: 128]
# asm 2: vld1.8 {>h5=d24->h5=d25},[<ptr=r2,: 128]
vld1.8 {d24-d25},[r2,: 128]

# qhasm:   h4[0,1] += mix[0] signed* g57_19[0];h4[2,3] += mix[1] signed* g57_19[1] 
# asm 1: vmlal.s32 <h4=reg128#2,<mix=reg128#16%bot,<g57_19=reg128#15%bot
# asm 2: vmlal.s32 <h4=q1,<mix=d30,<g57_19=d28
vmlal.s32 q1,d30,d28

# qhasm:   2x t0 = h0 + _0x2000000 
# asm 1: vadd.i64 >t0=reg128#14,<h0=reg128#1,<_0x2000000=reg128#12
# asm 2: vadd.i64 >t0=q13,<h0=q0,<_0x2000000=q11
vadd.i64 q13,q0,q11

# qhasm:   2x t6 = h6 + _0x2000000 
# asm 1: vadd.i64 >t6=reg128#15,<h6=reg128#6,<_0x2000000=reg128#12
# asm 2: vadd.i64 >t6=q14,<h6=q5,<_0x2000000=q11
vadd.i64 q14,q5,q11

# qhasm:   h2[0,1] += mix[0] signed* g13_19[2];h2[2,3] += mix[1] signed* g13_19[3] 
# asm 1: vmlal.s32 <h2=reg128#7,<mix=reg128#16%bot,<g13_19=reg128#5%top
# asm 2: vmlal.s32 <h2=q6,<mix=d30,<g13_19=d9
vmlal.s32 q6,d30,d9

# qhasm:   2x c0 = t0 signed>> 26 
# asm 1: vshr.s64 >c0=reg128#5,<t0=reg128#14,#26
# asm 2: vshr.s64 >c0=q4,<t0=q13,#26
vshr.s64 q4,q13,#26

# qhasm:   2x c6 = t6 signed>> 26 
# asm 1: vshr.s64 >c6=reg128#14,<t6=reg128#15,#26
# asm 2: vshr.s64 >c6=q13,<t6=q14,#26
vshr.s64 q13,q14,#26

# qhasm:   2x h1 += c0 
# asm 1: vadd.i64 >h1=reg128#8,<h1=reg128#8,<c0=reg128#5
# asm 2: vadd.i64 >h1=q7,<h1=q7,<c0=q4
vadd.i64 q7,q7,q4

# qhasm:   2x t0 = c0 << 26 
# asm 1: vshl.i64 >t0=reg128#5,<c0=reg128#5,#26
# asm 2: vshl.i64 >t0=q4,<c0=q4,#26
vshl.i64 q4,q4,#26

# qhasm:   2x t1 = h1 + _0x1000000 
# asm 1: vadd.i64 >t1=reg128#15,<h1=reg128#8,<_0x1000000=reg128#4
# asm 2: vadd.i64 >t1=q14,<h1=q7,<_0x1000000=q3
vadd.i64 q14,q7,q3

# qhasm:   2x h7 += c6 
# asm 1: vadd.i64 >h7=reg128#10,<h7=reg128#10,<c6=reg128#14
# asm 2: vadd.i64 >h7=q9,<h7=q9,<c6=q13
vadd.i64 q9,q9,q13

# qhasm:   2x t6 = c6 << 26 
# asm 1: vshl.i64 >t6=reg128#14,<c6=reg128#14,#26
# asm 2: vshl.i64 >t6=q13,<c6=q13,#26
vshl.i64 q13,q13,#26

# qhasm:   2x t7 = h7 + _0x1000000 
# asm 1: vadd.i64 >t7=reg128#16,<h7=reg128#10,<_0x1000000=reg128#4
# asm 2: vadd.i64 >t7=q15,<h7=q9,<_0x1000000=q3
vadd.i64 q15,q9,q3

# qhasm:   2x h0 -= t0 
# asm 1: vsub.i64 >h0=reg128#1,<h0=reg128#1,<t0=reg128#5
# asm 2: vsub.i64 >h0=q0,<h0=q0,<t0=q4
vsub.i64 q0,q0,q4

# qhasm:   2x c1 = t1 signed>> 25 
# asm 1: vshr.s64 >c1=reg128#5,<t1=reg128#15,#25
# asm 2: vshr.s64 >c1=q4,<t1=q14,#25
vshr.s64 q4,q14,#25

# qhasm:   2x h6 -= t6 
# asm 1: vsub.i64 >h6=reg128#6,<h6=reg128#6,<t6=reg128#14
# asm 2: vsub.i64 >h6=q5,<h6=q5,<t6=q13
vsub.i64 q5,q5,q13

# qhasm:   2x c7 = t7 signed>> 25 
# asm 1: vshr.s64 >c7=reg128#14,<t7=reg128#16,#25
# asm 2: vshr.s64 >c7=q13,<t7=q15,#25
vshr.s64 q13,q15,#25

# qhasm:   2x h2 += c1 
# asm 1: vadd.i64 >h2=reg128#7,<h2=reg128#7,<c1=reg128#5
# asm 2: vadd.i64 >h2=q6,<h2=q6,<c1=q4
vadd.i64 q6,q6,q4

# qhasm:   2x t1 = c1 << 25 
# asm 1: vshl.i64 >t1=reg128#5,<c1=reg128#5,#25
# asm 2: vshl.i64 >t1=q4,<c1=q4,#25
vshl.i64 q4,q4,#25

# qhasm:   2x t2 = h2 + _0x2000000 
# asm 1: vadd.i64 >t2=reg128#15,<h2=reg128#7,<_0x2000000=reg128#12
# asm 2: vadd.i64 >t2=q14,<h2=q6,<_0x2000000=q11
vadd.i64 q14,q6,q11

# qhasm:   2x h8 += c7 
# asm 1: vadd.i64 >h8=reg128#3,<h8=reg128#3,<c7=reg128#14
# asm 2: vadd.i64 >h8=q2,<h8=q2,<c7=q13
vadd.i64 q2,q2,q13

# qhasm:   2x h1 -= t1 
# asm 1: vsub.i64 >h1=reg128#5,<h1=reg128#8,<t1=reg128#5
# asm 2: vsub.i64 >h1=q4,<h1=q7,<t1=q4
vsub.i64 q4,q7,q4

# qhasm:   2x c2 = t2 signed>> 26 
# asm 1: vshr.s64 >c2=reg128#8,<t2=reg128#15,#26
# asm 2: vshr.s64 >c2=q7,<t2=q14,#26
vshr.s64 q7,q14,#26

# qhasm:   2x t7 = c7 << 25 
# asm 1: vshl.i64 >t7=reg128#14,<c7=reg128#14,#25
# asm 2: vshl.i64 >t7=q13,<c7=q13,#25
vshl.i64 q13,q13,#25

# qhasm:   2x t8 = h8 + _0x2000000 
# asm 1: vadd.i64 >t8=reg128#15,<h8=reg128#3,<_0x2000000=reg128#12
# asm 2: vadd.i64 >t8=q14,<h8=q2,<_0x2000000=q11
vadd.i64 q14,q2,q11

# qhasm:   2x h3 += c2 
# asm 1: vadd.i64 >h3=reg128#9,<h3=reg128#9,<c2=reg128#8
# asm 2: vadd.i64 >h3=q8,<h3=q8,<c2=q7
vadd.i64 q8,q8,q7

# qhasm:   2x t2 = c2 << 26 
# asm 1: vshl.i64 >t2=reg128#8,<c2=reg128#8,#26
# asm 2: vshl.i64 >t2=q7,<c2=q7,#26
vshl.i64 q7,q7,#26

# qhasm:   2x t3 = h3 + _0x1000000 
# asm 1: vadd.i64 >t3=reg128#16,<h3=reg128#9,<_0x1000000=reg128#4
# asm 2: vadd.i64 >t3=q15,<h3=q8,<_0x1000000=q3
vadd.i64 q15,q8,q3

# qhasm:   2x h7 -= t7 
# asm 1: vsub.i64 >h7=reg128#10,<h7=reg128#10,<t7=reg128#14
# asm 2: vsub.i64 >h7=q9,<h7=q9,<t7=q13
vsub.i64 q9,q9,q13

# qhasm:   2x c8 = t8 signed>> 26 
# asm 1: vshr.s64 >c8=reg128#14,<t8=reg128#15,#26
# asm 2: vshr.s64 >c8=q13,<t8=q14,#26
vshr.s64 q13,q14,#26

# qhasm:   2x h2 -= t2 
# asm 1: vsub.i64 >h2=reg128#7,<h2=reg128#7,<t2=reg128#8
# asm 2: vsub.i64 >h2=q6,<h2=q6,<t2=q7
vsub.i64 q6,q6,q7

# qhasm:   2x c3 = t3 signed>> 25 
# asm 1: vshr.s64 >c3=reg128#8,<t3=reg128#16,#25
# asm 2: vshr.s64 >c3=q7,<t3=q15,#25
vshr.s64 q7,q15,#25

# qhasm:   2x h9 += c8 
# asm 1: vadd.i64 >h9=reg128#11,<h9=reg128#11,<c8=reg128#14
# asm 2: vadd.i64 >h9=q10,<h9=q10,<c8=q13
vadd.i64 q10,q10,q13

# qhasm:   2x t8 = c8 << 26 
# asm 1: vshl.i64 >t8=reg128#14,<c8=reg128#14,#26
# asm 2: vshl.i64 >t8=q13,<c8=q13,#26
vshl.i64 q13,q13,#26

# qhasm:   2x t9 = h9 + _0x1000000 
# asm 1: vadd.i64 >t9=reg128#15,<h9=reg128#11,<_0x1000000=reg128#4
# asm 2: vadd.i64 >t9=q14,<h9=q10,<_0x1000000=q3
vadd.i64 q14,q10,q3

# qhasm:   2x h4 += c3 
# asm 1: vadd.i64 >h4=reg128#2,<h4=reg128#2,<c3=reg128#8
# asm 2: vadd.i64 >h4=q1,<h4=q1,<c3=q7
vadd.i64 q1,q1,q7

# qhasm:   posh = playground1_ptr + 240 
# asm 1: add >posh=int32#3,<playground1_ptr=int32#4,#240
# asm 2: add >posh=r2,<playground1_ptr=r3,#240
add r2,r3,#240

# qhasm:   2x t3 = c3 << 25 
# asm 1: vshl.i64 >t3=reg128#8,<c3=reg128#8,#25
# asm 2: vshl.i64 >t3=q7,<c3=q7,#25
vshl.i64 q7,q7,#25

# qhasm:   posH = playground1_ptr + 144 
# asm 1: add >posH=int32#5,<playground1_ptr=int32#4,#144
# asm 2: add >posH=r4,<playground1_ptr=r3,#144
add r4,r3,#144

# qhasm:   2x t4 = h4 + _0x2000000 
# asm 1: vadd.i64 >t4=reg128#16,<h4=reg128#2,<_0x2000000=reg128#12
# asm 2: vadd.i64 >t4=q15,<h4=q1,<_0x2000000=q11
vadd.i64 q15,q1,q11

# qhasm:   posh+=8 
# asm 1: add >posh=int32#3,<posh=int32#3,#8
# asm 2: add >posh=r2,<posh=r2,#8
add r2,r2,#8

# qhasm:   2x h8 -= t8 
# asm 1: vsub.i64 >h8=reg128#3,<h8=reg128#3,<t8=reg128#14
# asm 2: vsub.i64 >h8=q2,<h8=q2,<t8=q13
vsub.i64 q2,q2,q13

# qhasm:   posH+=8 
# asm 1: add >posH=int32#5,<posH=int32#5,#8
# asm 2: add >posH=r4,<posH=r4,#8
add r4,r4,#8

# qhasm:   2x c9 = t9 signed>> 25 
# asm 1: vshr.s64 >c9=reg128#14,<t9=reg128#15,#25
# asm 2: vshr.s64 >c9=q13,<t9=q14,#25
vshr.s64 q13,q14,#25

# qhasm:   2x h3 -= t3 
# asm 1: vsub.i64 >h3=reg128#8,<h3=reg128#9,<t3=reg128#8
# asm 2: vsub.i64 >h3=q7,<h3=q8,<t3=q7
vsub.i64 q7,q8,q7

# qhasm:   2x c4 = t4 signed>> 26 
# asm 1: vshr.s64 >c4=reg128#9,<t4=reg128#16,#26
# asm 2: vshr.s64 >c4=q8,<t4=q15,#26
vshr.s64 q8,q15,#26

# qhasm:   2x s = c9 + c9 
# asm 1: vadd.i64 >s=reg128#15,<c9=reg128#14,<c9=reg128#14
# asm 2: vadd.i64 >s=q14,<c9=q13,<c9=q13
vadd.i64 q14,q13,q13

# qhasm:   2x h5 += c4 
# asm 1: vadd.i64 >h5=reg128#13,<h5=reg128#13,<c4=reg128#9
# asm 2: vadd.i64 >h5=q12,<h5=q12,<c4=q8
vadd.i64 q12,q12,q8

# qhasm:   h2 h3 = h2[0]h3[0]h2[2]h2[3] h2[1]h3[1]h3[2]h3[3] 
# asm 1: vtrn.32 <h2=reg128#7%bot,<h3=reg128#8%bot
# asm 2: vtrn.32 <h2=d12,<h3=d14
vtrn.32 d12,d14

# qhasm:   2x t4 = c4 << 26 
# asm 1: vshl.i64 >t4=reg128#9,<c4=reg128#9,#26
# asm 2: vshl.i64 >t4=q8,<c4=q8,#26
vshl.i64 q8,q8,#26

# qhasm:   h2 h3 = h2[0]h2[1]h2[2]h3[2] h3[0]h3[1]h2[3]h3[3] 
# asm 1: vtrn.32 <h2=reg128#7%top,<h3=reg128#8%top
# asm 2: vtrn.32 <h2=d13,<h3=d15
vtrn.32 d13,d15

# qhasm:   2x t5 = h5 + _0x1000000 
# asm 1: vadd.i64 >t5=reg128#4,<h5=reg128#13,<_0x1000000=reg128#4
# asm 2: vadd.i64 >t5=q3,<h5=q12,<_0x1000000=q3
vadd.i64 q3,q12,q3

# qhasm:   2x h0 += s 
# asm 1: vadd.i64 >h0=reg128#1,<h0=reg128#1,<s=reg128#15
# asm 2: vadd.i64 >h0=q0,<h0=q0,<s=q14
vadd.i64 q0,q0,q14

# qhasm:   mem64[posh] aligned= h2[0];posh+=8 
# asm 1: vst1.8 <h2=reg128#7%bot,[<posh=int32#3,: 64]!
# asm 2: vst1.8 <h2=d12,[<posh=r2,: 64]!
vst1.8 d12,[r2,: 64]!

# qhasm:   2x s = c9 << 4 
# asm 1: vshl.i64 >s=reg128#8,<c9=reg128#14,#4
# asm 2: vshl.i64 >s=q7,<c9=q13,#4
vshl.i64 q7,q13,#4

# qhasm:   mem64[posH] aligned= h2[1];posH+=8 
# asm 1: vst1.8 <h2=reg128#7%top,[<posH=int32#5,: 64]!
# asm 2: vst1.8 <h2=d13,[<posH=r4,: 64]!
vst1.8 d13,[r4,: 64]!

# qhasm:   2x h4 -= t4 
# asm 1: vsub.i64 >h4=reg128#2,<h4=reg128#2,<t4=reg128#9
# asm 2: vsub.i64 >h4=q1,<h4=q1,<t4=q8
vsub.i64 q1,q1,q8

# qhasm:   2x c5 = t5 signed>> 25 
# asm 1: vshr.s64 >c5=reg128#4,<t5=reg128#4,#25
# asm 2: vshr.s64 >c5=q3,<t5=q3,#25
vshr.s64 q3,q3,#25

# qhasm:   2x h0 += s 
# asm 1: vadd.i64 >h0=reg128#1,<h0=reg128#1,<s=reg128#8
# asm 2: vadd.i64 >h0=q0,<h0=q0,<s=q7
vadd.i64 q0,q0,q7

# qhasm:   2x h6 += c5 
# asm 1: vadd.i64 >h6=reg128#6,<h6=reg128#6,<c5=reg128#4
# asm 2: vadd.i64 >h6=q5,<h6=q5,<c5=q3
vadd.i64 q5,q5,q3

# qhasm:   2x t5 = c5 << 25 
# asm 1: vshl.i64 >t5=reg128#4,<c5=reg128#4,#25
# asm 2: vshl.i64 >t5=q3,<c5=q3,#25
vshl.i64 q3,q3,#25

# qhasm:   2x t6 = h6 + _0x2000000 
# asm 1: vadd.i64 >t6=reg128#7,<h6=reg128#6,<_0x2000000=reg128#12
# asm 2: vadd.i64 >t6=q6,<h6=q5,<_0x2000000=q11
vadd.i64 q6,q5,q11

# qhasm:   2x h0 += c9 
# asm 1: vadd.i64 >h0=reg128#1,<h0=reg128#1,<c9=reg128#14
# asm 2: vadd.i64 >h0=q0,<h0=q0,<c9=q13
vadd.i64 q0,q0,q13

# qhasm:   2x t9 = c9 << 25 
# asm 1: vshl.i64 >t9=reg128#8,<c9=reg128#14,#25
# asm 2: vshl.i64 >t9=q7,<c9=q13,#25
vshl.i64 q7,q13,#25

# qhasm:   2x t0 = h0 + _0x2000000 
# asm 1: vadd.i64 >t0=reg128#9,<h0=reg128#1,<_0x2000000=reg128#12
# asm 2: vadd.i64 >t0=q8,<h0=q0,<_0x2000000=q11
vadd.i64 q8,q0,q11

# qhasm:   2x h5 -= t5 
# asm 1: vsub.i64 >h5=reg128#4,<h5=reg128#13,<t5=reg128#4
# asm 2: vsub.i64 >h5=q3,<h5=q12,<t5=q3
vsub.i64 q3,q12,q3

# qhasm:   2x c6 = t6 signed>> 26 
# asm 1: vshr.s64 >c6=reg128#7,<t6=reg128#7,#26
# asm 2: vshr.s64 >c6=q6,<t6=q6,#26
vshr.s64 q6,q6,#26

# qhasm:   2x h9 -= t9 
# asm 1: vsub.i64 >h9=reg128#8,<h9=reg128#11,<t9=reg128#8
# asm 2: vsub.i64 >h9=q7,<h9=q10,<t9=q7
vsub.i64 q7,q10,q7

# qhasm:   h4 h5 = h4[0]h5[0]h4[2]h4[3] h4[1]h5[1]h5[2]h5[3] 
# asm 1: vtrn.32 <h4=reg128#2%bot,<h5=reg128#4%bot
# asm 2: vtrn.32 <h4=d2,<h5=d6
vtrn.32 d2,d6

# qhasm:   2x c0 = t0 signed>> 26 
# asm 1: vshr.s64 >c0=reg128#9,<t0=reg128#9,#26
# asm 2: vshr.s64 >c0=q8,<t0=q8,#26
vshr.s64 q8,q8,#26

# qhasm:   h4 h5 = h4[0]h4[1]h4[2]h5[2] h5[0]h5[1]h4[3]h5[3] 
# asm 1: vtrn.32 <h4=reg128#2%top,<h5=reg128#4%top
# asm 2: vtrn.32 <h4=d3,<h5=d7
vtrn.32 d3,d7

# qhasm:   2x h7 += c6 
# asm 1: vadd.i64 >h7=reg128#4,<h7=reg128#10,<c6=reg128#7
# asm 2: vadd.i64 >h7=q3,<h7=q9,<c6=q6
vadd.i64 q3,q9,q6

# qhasm:   mem64[posh] aligned= h4[0] 
# asm 1: vst1.8 <h4=reg128#2%bot,[<posh=int32#3,: 64]
# asm 2: vst1.8 <h4=d2,[<posh=r2,: 64]
vst1.8 d2,[r2,: 64]

# qhasm:   2x t6 = c6 << 26 
# asm 1: vshl.i64 >t6=reg128#7,<c6=reg128#7,#26
# asm 2: vshl.i64 >t6=q6,<c6=q6,#26
vshl.i64 q6,q6,#26

# qhasm:   mem64[posH] aligned= h4[1] 
# asm 1: vst1.8 <h4=reg128#2%top,[<posH=int32#5,: 64]
# asm 2: vst1.8 <h4=d3,[<posH=r4,: 64]
vst1.8 d3,[r4,: 64]

# qhasm:   2x h1 += c0 
# asm 1: vadd.i64 >h1=reg128#2,<h1=reg128#5,<c0=reg128#9
# asm 2: vadd.i64 >h1=q1,<h1=q4,<c0=q8
vadd.i64 q1,q4,q8

# qhasm:   h8 h9 = h8[0]h9[0]h8[2]h8[3] h8[1]h9[1]h9[2]h9[3] 
# asm 1: vtrn.32 <h8=reg128#3%bot,<h9=reg128#8%bot
# asm 2: vtrn.32 <h8=d4,<h9=d14
vtrn.32 d4,d14

# qhasm:   2x t0 = c0 << 26 
# asm 1: vshl.i64 >t0=reg128#5,<c0=reg128#9,#26
# asm 2: vshl.i64 >t0=q4,<c0=q8,#26
vshl.i64 q4,q8,#26

# qhasm:   h8 h9 = h8[0]h8[1]h8[2]h9[2] h9[0]h9[1]h8[3]h9[3] 
# asm 1: vtrn.32 <h8=reg128#3%top,<h9=reg128#8%top
# asm 2: vtrn.32 <h8=d5,<h9=d15
vtrn.32 d5,d15

# qhasm:   2x h6 -= t6 
# asm 1: vsub.i64 >h6=reg128#6,<h6=reg128#6,<t6=reg128#7
# asm 2: vsub.i64 >h6=q5,<h6=q5,<t6=q6
vsub.i64 q5,q5,q6

# qhasm:   posh+=16 
# asm 1: add >posh=int32#3,<posh=int32#3,#16
# asm 2: add >posh=r2,<posh=r2,#16
add r2,r2,#16

# qhasm:   2x h0 -= t0 
# asm 1: vsub.i64 >h0=reg128#1,<h0=reg128#1,<t0=reg128#5
# asm 2: vsub.i64 >h0=q0,<h0=q0,<t0=q4
vsub.i64 q0,q0,q4

# qhasm:   mem64[posh] aligned= h8[0] 
# asm 1: vst1.8 <h8=reg128#3%bot,[<posh=int32#3,: 64]
# asm 2: vst1.8 <h8=d4,[<posh=r2,: 64]
vst1.8 d4,[r2,: 64]

# qhasm:   posH+=16 
# asm 1: add >posH=int32#5,<posH=int32#5,#16
# asm 2: add >posH=r4,<posH=r4,#16
add r4,r4,#16

# qhasm:   mem64[posH] aligned= h8[1] 
# asm 1: vst1.8 <h8=reg128#3%top,[<posH=int32#5,: 64]
# asm 2: vst1.8 <h8=d5,[<posH=r4,: 64]
vst1.8 d5,[r4,: 64]

# qhasm:   h6 h7 = h6[0]h7[0]h6[2]h6[3] h6[1]h7[1]h7[2]h7[3] 
# asm 1: vtrn.32 <h6=reg128#6%bot,<h7=reg128#4%bot
# asm 2: vtrn.32 <h6=d10,<h7=d6
vtrn.32 d10,d6

# qhasm:   h6 h7 = h6[0]h6[1]h6[2]h7[2] h7[0]h7[1]h6[3]h7[3] 
# asm 1: vtrn.32 <h6=reg128#6%top,<h7=reg128#4%top
# asm 2: vtrn.32 <h6=d11,<h7=d7
vtrn.32 d11,d7

# qhasm:   posh-=8 
# asm 1: sub >posh=int32#3,<posh=int32#3,#8
# asm 2: sub >posh=r2,<posh=r2,#8
sub r2,r2,#8

# qhasm:   posH-=8 
# asm 1: sub >posH=int32#5,<posH=int32#5,#8
# asm 2: sub >posH=r4,<posH=r4,#8
sub r4,r4,#8

# qhasm:   h0 h1 = h0[0]h1[0]h0[2]h0[3] h0[1]h1[1]h1[2]h1[3] 
# asm 1: vtrn.32 <h0=reg128#1%bot,<h1=reg128#2%bot
# asm 2: vtrn.32 <h0=d0,<h1=d2
vtrn.32 d0,d2

# qhasm:   h0 h1 = h0[0]h0[1]h0[2]h1[2] h1[0]h1[1]h0[3]h1[3] 
# asm 1: vtrn.32 <h0=reg128#1%top,<h1=reg128#2%top
# asm 2: vtrn.32 <h0=d1,<h1=d3
vtrn.32 d1,d3

# qhasm:   mem64[posh] aligned= h6[0] 
# asm 1: vst1.8 <h6=reg128#6%bot,[<posh=int32#3,: 64]
# asm 2: vst1.8 <h6=d10,[<posh=r2,: 64]
vst1.8 d10,[r2,: 64]

# qhasm:   mem64[posH] aligned= h6[1] 
# asm 1: vst1.8 <h6=reg128#6%top,[<posH=int32#5,: 64]
# asm 2: vst1.8 <h6=d11,[<posH=r4,: 64]
vst1.8 d11,[r4,: 64]

# qhasm:   posh-=24 
# asm 1: sub >posh=int32#3,<posh=int32#3,#24
# asm 2: sub >posh=r2,<posh=r2,#24
sub r2,r2,#24

# qhasm:   posH-=24 
# asm 1: sub >posH=int32#5,<posH=int32#5,#24
# asm 2: sub >posH=r4,<posH=r4,#24
sub r4,r4,#24

# qhasm:   mem64[posh] aligned= h0[0] 
# asm 1: vst1.8 <h0=reg128#1%bot,[<posh=int32#3,: 64]
# asm 2: vst1.8 <h0=d0,[<posh=r2,: 64]
vst1.8 d0,[r2,: 64]

# qhasm:   mem64[posH] aligned= h0[1] 
# asm 1: vst1.8 <h0=reg128#1%top,[<posH=int32#5,: 64]
# asm 2: vst1.8 <h0=d1,[<posH=r4,: 64]
vst1.8 d1,[r4,: 64]

# qhasm:   pos = pos_stack
# asm 1: ldr >pos=int32#3,<pos_stack=stack32#3
# asm 2: ldr >pos=r2,<pos_stack=[sp,#488]
ldr r2,[sp,#488]

# qhasm:   swap = swap_stack
# asm 1: ldr >swap=int32#5,<swap_stack=stack32#4
# asm 2: ldr >swap=r4,<swap_stack=[sp,#492]
ldr r4,[sp,#492]

# qhasm:                   signed<? pos -= 1
# asm 1: subs >pos=int32#6,<pos=int32#3,#1
# asm 2: subs >pos=r5,<pos=r2,#1
subs r5,r2,#1

# qhasm: goto mainloop if !signed<
bge ._mainloop

# qhasm:   posy = playground1_ptr + 144 
# asm 1: add >posy=int32#2,<playground1_ptr=int32#4,#144
# asm 2: add >posy=r1,<playground1_ptr=r3,#144
add r1,r3,#144

# qhasm:   posx = playground1_ptr + 336 
# asm 1: add >posx=int32#3,<playground1_ptr=int32#4,#336
# asm 2: add >posx=r2,<playground1_ptr=r3,#336
add r2,r3,#336

# qhasm:   f0 aligned= mem128[posy];posy += 16 
# asm 1: vld1.8 {>f0=reg128#1%bot->f0=reg128#1%top},[<posy=int32#2,: 128]!
# asm 2: vld1.8 {>f0=d0->f0=d1},[<posy=r1,: 128]!
vld1.8 {d0-d1},[r1,: 128]!

# qhasm:   f4 aligned= mem128[posy];posy += 16 
# asm 1: vld1.8 {>f4=reg128#2%bot->f4=reg128#2%top},[<posy=int32#2,: 128]!
# asm 2: vld1.8 {>f4=d2->f4=d3},[<posy=r1,: 128]!
vld1.8 {d2-d3},[r1,: 128]!

# qhasm:   new f8 

# qhasm:   f8 aligned= mem64[posy] f8[1] 
# asm 1: vld1.8 {<f8=reg128#3%bot},[<posy=int32#2,: 64]
# asm 2: vld1.8 {<f8=d4},[<posy=r1,: 64]
vld1.8 {d4},[r1,: 64]

# qhasm:   mem128[posx] aligned= f0;posx += 16 
# asm 1: vst1.8 {<f0=reg128#1%bot-<f0=reg128#1%top},[<posx=int32#3,: 128]!
# asm 2: vst1.8 {<f0=d0-<f0=d1},[<posx=r2,: 128]!
vst1.8 {d0-d1},[r2,: 128]!

# qhasm:   mem128[posx] aligned= f4;posx += 16 
# asm 1: vst1.8 {<f4=reg128#2%bot-<f4=reg128#2%top},[<posx=int32#3,: 128]!
# asm 2: vst1.8 {<f4=d2-<f4=d3},[<posx=r2,: 128]!
vst1.8 {d2-d3},[r2,: 128]!

# qhasm:   mem64[posx] aligned= f8[0] 
# asm 1: vst1.8 <f8=reg128#3%bot,[<posx=int32#3,: 64]
# asm 2: vst1.8 <f8=d4,[<posx=r2,: 64]
vst1.8 d4,[r2,: 64]

# qhasm: i = 0
# asm 1: ldr >i=int32#2,=0
# asm 2: ldr >i=r1,=0
ldr r1,=0

# qhasm: invertloop:
._invertloop:

# qhasm:   mulsource = playground1_ptr + 144
# asm 1: add >mulsource=int32#3,<playground1_ptr=int32#4,#144
# asm 2: add >mulsource=r2,<playground1_ptr=r3,#144
add r2,r3,#144

# qhasm:   postcopy = 0
# asm 1: ldr >postcopy=int32#5,=0
# asm 2: ldr >postcopy=r4,=0
ldr r4,=0

# qhasm:   j = 2
# asm 1: ldr >j=int32#6,=2
# asm 2: ldr >j=r5,=2
ldr r5,=2

# qhasm:            =? i - 1
# asm 1: cmp <i=int32#2,#1
# asm 2: cmp <i=r1,#1
cmp r1,#1

# qhasm:   j = 1 if =
# asm 1: ldreq <j=int32#6,=1
# asm 2: ldreq <j=r5,=1
ldreq r5,=1

# qhasm:   mulsource = playground1_ptr + 336 if =
# asm 1: addeq <mulsource=int32#3,<playground1_ptr=int32#4,#336
# asm 2: addeq <mulsource=r2,<playground1_ptr=r3,#336
addeq r2,r3,#336

# qhasm:   postcopy = playground1_ptr + 48 if =
# asm 1: addeq <postcopy=int32#5,<playground1_ptr=int32#4,#48
# asm 2: addeq <postcopy=r4,<playground1_ptr=r3,#48
addeq r4,r3,#48

# qhasm:            =? i - 2
# asm 1: cmp <i=int32#2,#2
# asm 2: cmp <i=r1,#2
cmp r1,#2

# qhasm:   j = 1 if =
# asm 1: ldreq <j=int32#6,=1
# asm 2: ldreq <j=r5,=1
ldreq r5,=1

# qhasm:   mulsource = playground1_ptr + 48 if =
# asm 1: addeq <mulsource=int32#3,<playground1_ptr=int32#4,#48
# asm 2: addeq <mulsource=r2,<playground1_ptr=r3,#48
addeq r2,r3,#48

# qhasm:            =? i - 3
# asm 1: cmp <i=int32#2,#3
# asm 2: cmp <i=r1,#3
cmp r1,#3

# qhasm:   j = 5 if =
# asm 1: ldreq <j=int32#6,=5
# asm 2: ldreq <j=r5,=5
ldreq r5,=5

# qhasm:   postcopy = playground1_ptr + 336 if =
# asm 1: addeq <postcopy=int32#5,<playground1_ptr=int32#4,#336
# asm 2: addeq <postcopy=r4,<playground1_ptr=r3,#336
addeq r4,r3,#336

# qhasm:             =? i - 4
# asm 1: cmp <i=int32#2,#4
# asm 2: cmp <i=r1,#4
cmp r1,#4

# qhasm:   j = 10 if =
# asm 1: ldreq <j=int32#6,=10
# asm 2: ldreq <j=r5,=10
ldreq r5,=10

# qhasm:             =? i - 5
# asm 1: cmp <i=int32#2,#5
# asm 2: cmp <i=r1,#5
cmp r1,#5

# qhasm:   j = 20 if =
# asm 1: ldreq <j=int32#6,=20
# asm 2: ldreq <j=r5,=20
ldreq r5,=20

# qhasm:             =? i - 6
# asm 1: cmp <i=int32#2,#6
# asm 2: cmp <i=r1,#6
cmp r1,#6

# qhasm:   j = 10 if =
# asm 1: ldreq <j=int32#6,=10
# asm 2: ldreq <j=r5,=10
ldreq r5,=10

# qhasm:   mulsource = playground1_ptr + 336 if =
# asm 1: addeq <mulsource=int32#3,<playground1_ptr=int32#4,#336
# asm 2: addeq <mulsource=r2,<playground1_ptr=r3,#336
addeq r2,r3,#336

# qhasm:   postcopy = playground1_ptr + 336 if =
# asm 1: addeq <postcopy=int32#5,<playground1_ptr=int32#4,#336
# asm 2: addeq <postcopy=r4,<playground1_ptr=r3,#336
addeq r4,r3,#336

# qhasm:             =? i - 7
# asm 1: cmp <i=int32#2,#7
# asm 2: cmp <i=r1,#7
cmp r1,#7

# qhasm:   j = 50 if =
# asm 1: ldreq <j=int32#6,=50
# asm 2: ldreq <j=r5,=50
ldreq r5,=50

# qhasm:              =? i - 8
# asm 1: cmp <i=int32#2,#8
# asm 2: cmp <i=r1,#8
cmp r1,#8

# qhasm:   j = 100 if =
# asm 1: ldreq <j=int32#6,=100
# asm 2: ldreq <j=r5,=100
ldreq r5,=100

# qhasm:             =? i - 9
# asm 1: cmp <i=int32#2,#9
# asm 2: cmp <i=r1,#9
cmp r1,#9

# qhasm:   j = 50 if =
# asm 1: ldreq <j=int32#6,=50
# asm 2: ldreq <j=r5,=50
ldreq r5,=50

# qhasm:   mulsource = playground1_ptr + 336 if =
# asm 1: addeq <mulsource=int32#3,<playground1_ptr=int32#4,#336
# asm 2: addeq <mulsource=r2,<playground1_ptr=r3,#336
addeq r2,r3,#336

# qhasm:            =? i - 10
# asm 1: cmp <i=int32#2,#10
# asm 2: cmp <i=r1,#10
cmp r1,#10

# qhasm:   j = 5 if =
# asm 1: ldreq <j=int32#6,=5
# asm 2: ldreq <j=r5,=5
ldreq r5,=5

# qhasm:   mulsource = playground1_ptr + 48 if =
# asm 1: addeq <mulsource=int32#3,<playground1_ptr=int32#4,#48
# asm 2: addeq <mulsource=r2,<playground1_ptr=r3,#48
addeq r2,r3,#48

# qhasm:            =? i - 11
# asm 1: cmp <i=int32#2,#11
# asm 2: cmp <i=r1,#11
cmp r1,#11

# qhasm:   j = 0 if =
# asm 1: ldreq <j=int32#6,=0
# asm 2: ldreq <j=r5,=0
ldreq r5,=0

# qhasm:   mulsource = playground1_ptr + 96 if =
# asm 1: addeq <mulsource=int32#3,<playground1_ptr=int32#4,#96
# asm 2: addeq <mulsource=r2,<playground1_ptr=r3,#96
addeq r2,r3,#96

# qhasm:   posy = playground1_ptr + 144 
# asm 1: add >posy=int32#7,<playground1_ptr=int32#4,#144
# asm 2: add >posy=r6,<playground1_ptr=r3,#144
add r6,r3,#144

# qhasm:   posx = playground1_ptr + 288 
# asm 1: add >posx=int32#8,<playground1_ptr=int32#4,#288
# asm 2: add >posx=r7,<playground1_ptr=r3,#288
add r7,r3,#288

# qhasm:   f0 aligned= mem128[posy];posy += 16 
# asm 1: vld1.8 {>f0=reg128#1%bot->f0=reg128#1%top},[<posy=int32#7,: 128]!
# asm 2: vld1.8 {>f0=d0->f0=d1},[<posy=r6,: 128]!
vld1.8 {d0-d1},[r6,: 128]!

# qhasm:   f4 aligned= mem128[posy];posy += 16 
# asm 1: vld1.8 {>f4=reg128#2%bot->f4=reg128#2%top},[<posy=int32#7,: 128]!
# asm 2: vld1.8 {>f4=d2->f4=d3},[<posy=r6,: 128]!
vld1.8 {d2-d3},[r6,: 128]!

# qhasm:   new f8 

# qhasm:   f8 aligned= mem64[posy] f8[1] 
# asm 1: vld1.8 {<f8=reg128#3%bot},[<posy=int32#7,: 64]
# asm 2: vld1.8 {<f8=d4},[<posy=r6,: 64]
vld1.8 {d4},[r6,: 64]

# qhasm:   mem128[posx] aligned= f0;posx += 16 
# asm 1: vst1.8 {<f0=reg128#1%bot-<f0=reg128#1%top},[<posx=int32#8,: 128]!
# asm 2: vst1.8 {<f0=d0-<f0=d1},[<posx=r7,: 128]!
vst1.8 {d0-d1},[r7,: 128]!

# qhasm:   mem128[posx] aligned= f4;posx += 16 
# asm 1: vst1.8 {<f4=reg128#2%bot-<f4=reg128#2%top},[<posx=int32#8,: 128]!
# asm 2: vst1.8 {<f4=d2-<f4=d3},[<posx=r7,: 128]!
vst1.8 {d2-d3},[r7,: 128]!

# qhasm:   mem64[posx] aligned= f8[0] 
# asm 1: vst1.8 <f8=reg128#3%bot,[<posx=int32#8,: 64]
# asm 2: vst1.8 <f8=d4,[<posx=r7,: 64]
vst1.8 d4,[r7,: 64]

# qhasm:                            =? j - 0
# asm 1: cmp <j=int32#6,#0
# asm 2: cmp <j=r5,#0
cmp r5,#0

# qhasm:   goto skipsquaringloop if =
beq ._skipsquaringloop

# qhasm:   squaringloop:
._squaringloop:

# qhasm:   posf = playground1_ptr + 288 
# asm 1: add >posf=int32#7,<playground1_ptr=int32#4,#288
# asm 2: add >posf=r6,<playground1_ptr=r3,#288
add r6,r3,#288

# qhasm:   posg = playground1_ptr + 288 
# asm 1: add >posg=int32#8,<playground1_ptr=int32#4,#288
# asm 2: add >posg=r7,<playground1_ptr=r3,#288
add r7,r3,#288

# qhasm:   posh = playground1_ptr + 288 
# asm 1: add >posh=int32#9,<playground1_ptr=int32#4,#288
# asm 2: add >posh=r8,<playground1_ptr=r3,#288
add r8,r3,#288

# qhasm:   4x _19_19_19_19 = 19 
# asm 1: vmov.i32 >_19_19_19_19=reg128#1,#19
# asm 2: vmov.i32 >_19_19_19_19=q0,#19
vmov.i32 q0,#19

# qhasm:   4x _0_1_0_1 = 0 
# asm 1: vmov.i32 >_0_1_0_1=reg128#2,#0
# asm 2: vmov.i32 >_0_1_0_1=q1,#0
vmov.i32 q1,#0

# qhasm:   4x _1_1_1_1 = 1 
# asm 1: vmov.i32 >_1_1_1_1=reg128#3,#1
# asm 2: vmov.i32 >_1_1_1_1=q2,#1
vmov.i32 q2,#1

# qhasm:   _0_1_0_1[0,1,2,3] _1_1_1_1[0,1,2,3] = _0_1_0_1[0]_1_1_1_1[0]_0_1_0_1[1]_1_1_1_1[1] _0_1_0_1[2]_1_1_1_1[2]_0_1_0_1[3]_1_1_1_1[3] 
# asm 1: vzip.i32 <_0_1_0_1=reg128#2,<_1_1_1_1=reg128#3
# asm 2: vzip.i32 <_0_1_0_1=q1,<_1_1_1_1=q2
vzip.i32 q1,q2

# qhasm:   g0_g1_g2_g3 aligned= mem128[posg];posg+=16 
# asm 1: vld1.8 {>g0_g1_g2_g3=reg128#3%bot->g0_g1_g2_g3=reg128#3%top},[<posg=int32#8,: 128]!
# asm 2: vld1.8 {>g0_g1_g2_g3=d4->g0_g1_g2_g3=d5},[<posg=r7,: 128]!
vld1.8 {d4-d5},[r7,: 128]!

# qhasm:   g4_g5_g6_g7 aligned= mem128[posg];posg+=16 
# asm 1: vld1.8 {>g4_g5_g6_g7=reg128#4%bot->g4_g5_g6_g7=reg128#4%top},[<posg=int32#8,: 128]!
# asm 2: vld1.8 {>g4_g5_g6_g7=d6->g4_g5_g6_g7=d7},[<posg=r7,: 128]!
vld1.8 {d6-d7},[r7,: 128]!

# qhasm:   new f8_f9_g8_g9 

# qhasm:   f8_f9_g8_g9 aligned= f8_f9_g8_g9[0]mem64[posg] 
# asm 1: vld1.8 {<f8_f9_g8_g9=reg128#5%top},[<posg=int32#8,: 64]
# asm 2: vld1.8 {<f8_f9_g8_g9=d9},[<posg=r7,: 64]
vld1.8 {d9},[r7,: 64]

# qhasm:   f0_f1_f2_f3 aligned= mem128[posf];posf+=16 
# asm 1: vld1.8 {>f0_f1_f2_f3=reg128#6%bot->f0_f1_f2_f3=reg128#6%top},[<posf=int32#7,: 128]!
# asm 2: vld1.8 {>f0_f1_f2_f3=d10->f0_f1_f2_f3=d11},[<posf=r6,: 128]!
vld1.8 {d10-d11},[r6,: 128]!

# qhasm:   playp = &playground2 
# asm 1: lea >playp=int32#8,<playground2=stack512#1
# asm 2: lea >playp=r7,<playground2=[sp,#416]
add r7,sp,#416

# qhasm:   f4_f5_f6_f7 aligned= mem128[posf];posf+=16 
# asm 1: vld1.8 {>f4_f5_f6_f7=reg128#7%bot->f4_f5_f6_f7=reg128#7%top},[<posf=int32#7,: 128]!
# asm 2: vld1.8 {>f4_f5_f6_f7=d12->f4_f5_f6_f7=d13},[<posf=r6,: 128]!
vld1.8 {d12-d13},[r6,: 128]!

# qhasm:   4x 19g0_19g1_19g2_19g3 = g0_g1_g2_g3 * _19_19_19_19 
# asm 1: vmul.i32 >19g0_19g1_19g2_19g3=reg128#8,<g0_g1_g2_g3=reg128#3,<_19_19_19_19=reg128#1
# asm 2: vmul.i32 >19g0_19g1_19g2_19g3=q7,<g0_g1_g2_g3=q2,<_19_19_19_19=q0
vmul.i32 q7,q2,q0

# qhasm:   f8_f9_g8_g9 aligned= mem64[posf]f8_f9_g8_g9[1] 
# asm 1: vld1.8 {<f8_f9_g8_g9=reg128#5%bot},[<posf=int32#7,: 64]
# asm 2: vld1.8 {<f8_f9_g8_g9=d8},[<posf=r6,: 64]
vld1.8 {d8},[r6,: 64]

# qhasm:   new f1_f8_f3_f0 

# qhasm:   f1_f8_f3_f0 = f1_f8_f3_f0[0,1]f0_f1_f2_f3[3]f0_f1_f2_f3[0] 
# asm 1: vext.32 <f1_f8_f3_f0=reg128#9%top,<f0_f1_f2_f3=reg128#6%top,<f0_f1_f2_f3=reg128#6%bot,#1
# asm 2: vext.32 <f1_f8_f3_f0=d17,<f0_f1_f2_f3=d11,<f0_f1_f2_f3=d10,#1
vext.32 d17,d11,d10,#1

# qhasm:   4x 19g4_19g5_19g6_19g7 = g4_g5_g6_g7 * _19_19_19_19 
# asm 1: vmul.i32 >19g4_19g5_19g6_19g7=reg128#10,<g4_g5_g6_g7=reg128#4,<_19_19_19_19=reg128#1
# asm 2: vmul.i32 >19g4_19g5_19g6_19g7=q9,<g4_g5_g6_g7=q3,<_19_19_19_19=q0
vmul.i32 q9,q3,q0

# qhasm:   f1_f8_f3_f0 = f0_f1_f2_f3[1]f8_f9_g8_g9[0]f1_f8_f3_f0[2,3] 
# asm 1: vext.32 <f1_f8_f3_f0=reg128#9%bot,<f0_f1_f2_f3=reg128#6%bot,<f8_f9_g8_g9=reg128#5%bot,#1
# asm 2: vext.32 <f1_f8_f3_f0=d16,<f0_f1_f2_f3=d10,<f8_f9_g8_g9=d8,#1
vext.32 d16,d10,d8,#1

# qhasm:   4x f0_2f1_f2_2f3 = f0_f1_f2_f3 << _0_1_0_1 
# asm 1: vshl.u32 >f0_2f1_f2_2f3=reg128#11,<f0_f1_f2_f3=reg128#6,<_0_1_0_1=reg128#2
# asm 2: vshl.u32 >f0_2f1_f2_2f3=q10,<f0_f1_f2_f3=q5,<_0_1_0_1=q1
vshl.u32 q10,q5,q1

# qhasm:   new g0_19g1_g2_19g3 

# qhasm:   g0_19g1_g2_19g3 = 19g0_19g1_19g2_19g3[1]g0_g1_g2_g3[0]g0_19g1_g2_19g3[2,3] 
# asm 1: vext.32 <g0_19g1_g2_19g3=reg128#12%bot,<19g0_19g1_19g2_19g3=reg128#8%bot,<g0_g1_g2_g3=reg128#3%bot,#1
# asm 2: vext.32 <g0_19g1_g2_19g3=d22,<19g0_19g1_19g2_19g3=d14,<g0_g1_g2_g3=d4,#1
vext.32 d22,d14,d4,#1

# qhasm:   new g4_19g5_g6_19g7 

# qhasm:   g4_19g5_g6_19g7 = 19g4_19g5_19g6_19g7[1]g4_g5_g6_g7[0]g4_19g5_g6_19g7[2,3] 
# asm 1: vext.32 <g4_19g5_g6_19g7=reg128#13%bot,<19g4_19g5_19g6_19g7=reg128#10%bot,<g4_g5_g6_g7=reg128#4%bot,#1
# asm 2: vext.32 <g4_19g5_g6_19g7=d24,<19g4_19g5_19g6_19g7=d18,<g4_g5_g6_g7=d6,#1
vext.32 d24,d18,d6,#1

# qhasm:   4x f4_2f5_f6_2f7 = f4_f5_f6_f7 << _0_1_0_1 
# asm 1: vshl.u32 >f4_2f5_f6_2f7=reg128#14,<f4_f5_f6_f7=reg128#7,<_0_1_0_1=reg128#2
# asm 2: vshl.u32 >f4_2f5_f6_2f7=q13,<f4_f5_f6_f7=q6,<_0_1_0_1=q1
vshl.u32 q13,q6,q1

# qhasm:   new f8_2f9_f9_f6 

# qhasm:   f8_2f9_f9_f6 = f8_f9_g8_g9[0] << _0_1_0_1[0],f8_f9_g8_g9[1] << _0_1_0_1[1],f8_2f9_f9_f6[2,3] 
# asm 1: vshl.u32 <f8_2f9_f9_f6=reg128#15%bot,<f8_f9_g8_g9=reg128#5%bot,<_0_1_0_1=reg128#2%bot
# asm 2: vshl.u32 <f8_2f9_f9_f6=d28,<f8_f9_g8_g9=d8,<_0_1_0_1=d2
vshl.u32 d28,d8,d2

# qhasm:   g0_19g1_g2_19g3 = g0_19g1_g2_19g3[1]g0_19g1_g2_19g3[0]g0_19g1_g2_19g3[2,3] 
# asm 1: vrev64.i32 <g0_19g1_g2_19g3=reg128#12%bot,<g0_19g1_g2_19g3=reg128#12%bot
# asm 2: vrev64.i32 <g0_19g1_g2_19g3=d22,<g0_19g1_g2_19g3=d22
vrev64.i32 d22,d22

# qhasm:   g8_19g9_19g8_19g9[0,1] = g8_19g9_19g8_19g9[0,1];g8_19g9_19g8_19g9[2] = f8_f9_g8_g9[2] * _19_19_19_19[2];g8_19g9_19g8_19g9[3] = f8_f9_g8_g9[3] * _19_19_19_19[3] 
# asm 1: vmul.i32 >g8_19g9_19g8_19g9=reg128#1%top,<f8_f9_g8_g9=reg128#5%top,<_19_19_19_19=reg128#1%top
# asm 2: vmul.i32 >g8_19g9_19g8_19g9=d1,<f8_f9_g8_g9=d9,<_19_19_19_19=d1
vmul.i32 d1,d9,d1

# qhasm:   g4_19g5_g6_19g7 = g4_19g5_g6_19g7[1]g4_19g5_g6_19g7[0]g4_19g5_g6_19g7[2,3] 
# asm 1: vrev64.i32 <g4_19g5_g6_19g7=reg128#13%bot,<g4_19g5_g6_19g7=reg128#13%bot
# asm 2: vrev64.i32 <g4_19g5_g6_19g7=d24,<g4_19g5_g6_19g7=d24
vrev64.i32 d24,d24

# qhasm:   f8_2f9_f9_f6 = f8_2f9_f9_f6[0,1]f8_f9_g8_g9[1]f4_f5_f6_f7[2] 
# asm 1: vext.32 <f8_2f9_f9_f6=reg128#15%top,<f8_f9_g8_g9=reg128#5%bot,<f4_f5_f6_f7=reg128#7%top,#1
# asm 2: vext.32 <f8_2f9_f9_f6=d29,<f8_f9_g8_g9=d8,<f4_f5_f6_f7=d13,#1
vext.32 d29,d8,d13,#1

# qhasm:   g8_19g9_19g8_19g9 = g8_19g9_19g8_19g9[3]f8_f9_g8_g9[2]g8_19g9_19g8_19g9[2,3] 
# asm 1: vext.32 <g8_19g9_19g8_19g9=reg128#1%bot,<g8_19g9_19g8_19g9=reg128#1%top,<f8_f9_g8_g9=reg128#5%top,#1
# asm 2: vext.32 <g8_19g9_19g8_19g9=d0,<g8_19g9_19g8_19g9=d1,<f8_f9_g8_g9=d9,#1
vext.32 d0,d1,d9,#1

# qhasm:   g8_19g9_19g8_19g9 = g8_19g9_19g8_19g9[1]g8_19g9_19g8_19g9[0]g8_19g9_19g8_19g9[2,3] 
# asm 1: vrev64.i32 <g8_19g9_19g8_19g9=reg128#1%bot,<g8_19g9_19g8_19g9=reg128#1%bot
# asm 2: vrev64.i32 <g8_19g9_19g8_19g9=d0,<g8_19g9_19g8_19g9=d0
vrev64.i32 d0,d0

# qhasm:   new 19g8_g9_19g2_g3 

# qhasm:   19g8_g9_19g2_g3 = f8_f9_g8_g9[3]g8_19g9_19g8_19g9[2]19g8_g9_19g2_g3[2,3] 
# asm 1: vext.32 <19g8_g9_19g2_g3=reg128#2%bot,<f8_f9_g8_g9=reg128#5%top,<g8_19g9_19g8_19g9=reg128#1%top,#1
# asm 2: vext.32 <19g8_g9_19g2_g3=d2,<f8_f9_g8_g9=d9,<g8_19g9_19g8_19g9=d1,#1
vext.32 d2,d9,d1,#1

# qhasm:   g0_19g1_g2_19g3 = g0_19g1_g2_19g3[0,1]19g0_19g1_19g2_19g3[3]g0_g1_g2_g3[2] 
# asm 1: vext.32 <g0_19g1_g2_19g3=reg128#12%top,<19g0_19g1_19g2_19g3=reg128#8%top,<g0_g1_g2_g3=reg128#3%top,#1
# asm 2: vext.32 <g0_19g1_g2_19g3=d23,<19g0_19g1_19g2_19g3=d15,<g0_g1_g2_g3=d5,#1
vext.32 d23,d15,d5,#1

# qhasm:   h02[0,1] = f0_2f1_f2_2f3[0] signed* g0_g1_g2_g3[0];h02[2,3] = f0_2f1_f2_2f3[1] signed* g0_g1_g2_g3[1] 
# asm 1: vmull.s32 >h02=reg128#5,<f0_2f1_f2_2f3=reg128#11%bot,<g0_g1_g2_g3=reg128#3%bot
# asm 2: vmull.s32 >h02=q4,<f0_2f1_f2_2f3=d20,<g0_g1_g2_g3=d4
vmull.s32 q4,d20,d4

# qhasm:   g0_19g1_g2_19g3 = g0_19g1_g2_19g3[0,1]g0_19g1_g2_19g3[3]g0_19g1_g2_19g3[2] 
# asm 1: vrev64.i32 <g0_19g1_g2_19g3=reg128#12%top,<g0_19g1_g2_19g3=reg128#12%top
# asm 2: vrev64.i32 <g0_19g1_g2_19g3=d23,<g0_19g1_g2_19g3=d23
vrev64.i32 d23,d23

# qhasm:   h02[0,1] += f0_2f1_f2_2f3[2] signed* g8_19g9_19g8_19g9[2];h02[2,3] += f0_2f1_f2_2f3[3] signed* g8_19g9_19g8_19g9[3] 
# asm 1: vmlal.s32 <h02=reg128#5,<f0_2f1_f2_2f3=reg128#11%top,<g8_19g9_19g8_19g9=reg128#1%top
# asm 2: vmlal.s32 <h02=q4,<f0_2f1_f2_2f3=d21,<g8_19g9_19g8_19g9=d1
vmlal.s32 q4,d21,d1

# qhasm:   19g8_g9_19g2_g3 = 19g8_g9_19g2_g3[1]19g8_g9_19g2_g3[0]19g8_g9_19g2_g3[2,3] 
# asm 1: vrev64.i32 <19g8_g9_19g2_g3=reg128#2%bot,<19g8_g9_19g2_g3=reg128#2%bot
# asm 2: vrev64.i32 <19g8_g9_19g2_g3=d2,<19g8_g9_19g2_g3=d2
vrev64.i32 d2,d2

# qhasm:   h02[0,1] += f4_2f5_f6_2f7[0] signed* 19g4_19g5_19g6_19g7[2];h02[2,3] += f4_2f5_f6_2f7[1] signed* 19g4_19g5_19g6_19g7[3] 
# asm 1: vmlal.s32 <h02=reg128#5,<f4_2f5_f6_2f7=reg128#14%bot,<19g4_19g5_19g6_19g7=reg128#10%top
# asm 2: vmlal.s32 <h02=q4,<f4_2f5_f6_2f7=d26,<19g4_19g5_19g6_19g7=d19
vmlal.s32 q4,d26,d19

# qhasm:   19g8_g9_19g2_g3 = 19g8_g9_19g2_g3[0,1]g0_g1_g2_g3[3]19g0_19g1_19g2_19g3[2] 
# asm 1: vext.32 <19g8_g9_19g2_g3=reg128#2%top,<g0_g1_g2_g3=reg128#3%top,<19g0_19g1_19g2_19g3=reg128#8%top,#1
# asm 2: vext.32 <19g8_g9_19g2_g3=d3,<g0_g1_g2_g3=d5,<19g0_19g1_19g2_19g3=d15,#1
vext.32 d3,d5,d15,#1

# qhasm:   h02[0,1] += f4_2f5_f6_2f7[2] signed* 19g4_19g5_19g6_19g7[0];h02[2,3] += f4_2f5_f6_2f7[3] signed* 19g4_19g5_19g6_19g7[1] 
# asm 1: vmlal.s32 <h02=reg128#5,<f4_2f5_f6_2f7=reg128#14%top,<19g4_19g5_19g6_19g7=reg128#10%bot
# asm 2: vmlal.s32 <h02=q4,<f4_2f5_f6_2f7=d27,<19g4_19g5_19g6_19g7=d18
vmlal.s32 q4,d27,d18

# qhasm:   19g8_g9_19g2_g3 = 19g8_g9_19g2_g3[0,1]19g8_g9_19g2_g3[3]19g8_g9_19g2_g3[2] 
# asm 1: vrev64.i32 <19g8_g9_19g2_g3=reg128#2%top,<19g8_g9_19g2_g3=reg128#2%top
# asm 2: vrev64.i32 <19g8_g9_19g2_g3=d3,<19g8_g9_19g2_g3=d3
vrev64.i32 d3,d3

# qhasm:   h02[0,1] += f8_2f9_f9_f6[0] signed* 19g0_19g1_19g2_19g3[2];h02[2,3] += f8_2f9_f9_f6[1] signed* 19g0_19g1_19g2_19g3[3] 
# asm 1: vmlal.s32 <h02=reg128#5,<f8_2f9_f9_f6=reg128#15%bot,<19g0_19g1_19g2_19g3=reg128#8%top
# asm 2: vmlal.s32 <h02=q4,<f8_2f9_f9_f6=d28,<19g0_19g1_19g2_19g3=d15
vmlal.s32 q4,d28,d15

# qhasm:   new f5_f2_f7_f4 

# qhasm:   f5_f2_f7_f4 = f4_f5_f6_f7[1]f0_f1_f2_f3[2]f5_f2_f7_f4[2,3] 
# asm 1: vext.32 <f5_f2_f7_f4=reg128#8%bot,<f4_f5_f6_f7=reg128#7%bot,<f0_f1_f2_f3=reg128#6%top,#1
# asm 2: vext.32 <f5_f2_f7_f4=d14,<f4_f5_f6_f7=d12,<f0_f1_f2_f3=d11,#1
vext.32 d14,d12,d11,#1

# qhasm:   h31[0,1] = f1_f8_f3_f0[0] signed* g0_19g1_g2_19g3[2];h31[2,3] = f1_f8_f3_f0[1] signed* g0_19g1_g2_19g3[3] 
# asm 1: vmull.s32 >h31=reg128#6,<f1_f8_f3_f0=reg128#9%bot,<g0_19g1_g2_19g3=reg128#12%top
# asm 2: vmull.s32 >h31=q5,<f1_f8_f3_f0=d16,<g0_19g1_g2_19g3=d23
vmull.s32 q5,d16,d23

# qhasm:   f5_f2_f7_f4 = f5_f2_f7_f4[0,1]f4_f5_f6_f7[3]f4_f5_f6_f7[0] 
# asm 1: vext.32 <f5_f2_f7_f4=reg128#8%top,<f4_f5_f6_f7=reg128#7%top,<f4_f5_f6_f7=reg128#7%bot,#1
# asm 2: vext.32 <f5_f2_f7_f4=d15,<f4_f5_f6_f7=d13,<f4_f5_f6_f7=d12,#1
vext.32 d15,d13,d12,#1

# qhasm:   h31[0,1] += f1_f8_f3_f0[2] signed* g0_g1_g2_g3[0];h31[2,3] += f1_f8_f3_f0[3] signed* g0_g1_g2_g3[1] 
# asm 1: vmlal.s32 <h31=reg128#6,<f1_f8_f3_f0=reg128#9%top,<g0_g1_g2_g3=reg128#3%bot
# asm 2: vmlal.s32 <h31=q5,<f1_f8_f3_f0=d17,<g0_g1_g2_g3=d4
vmlal.s32 q5,d17,d4

# qhasm:   mem64[playp] aligned= h02[0];playp+=8 
# asm 1: vst1.8 <h02=reg128#5%bot,[<playp=int32#8,: 64]!
# asm 2: vst1.8 <h02=d8,[<playp=r7,: 64]!
vst1.8 d8,[r7,: 64]!

# qhasm:   h31[0,1] += f5_f2_f7_f4[0] signed* g8_19g9_19g8_19g9[2];h31[2,3] += f5_f2_f7_f4[1] signed* g8_19g9_19g8_19g9[3] 
# asm 1: vmlal.s32 <h31=reg128#6,<f5_f2_f7_f4=reg128#8%bot,<g8_19g9_19g8_19g9=reg128#1%top
# asm 2: vmlal.s32 <h31=q5,<f5_f2_f7_f4=d14,<g8_19g9_19g8_19g9=d1
vmlal.s32 q5,d14,d1

# qhasm:   new h24 

# qhasm:   h24 = h02[2,3]h24[2,3] 
# asm 1: vext.32 <h24=reg128#7%bot,<h02=reg128#5%top,<h02=reg128#5%bot,#0
# asm 2: vext.32 <h24=d12,<h02=d9,<h02=d8,#0
vext.32 d12,d9,d8,#0

# qhasm:   h31[0,1] += f5_f2_f7_f4[2] signed* 19g4_19g5_19g6_19g7[2];h31[2,3] += f5_f2_f7_f4[3] signed* 19g4_19g5_19g6_19g7[3] 
# asm 1: vmlal.s32 <h31=reg128#6,<f5_f2_f7_f4=reg128#8%top,<19g4_19g5_19g6_19g7=reg128#10%top
# asm 2: vmlal.s32 <h31=q5,<f5_f2_f7_f4=d15,<19g4_19g5_19g6_19g7=d19
vmlal.s32 q5,d15,d19

# qhasm:   h24 = h24[0],0 
# asm 1: vmov.i64 <h24=reg128#7%top,#0
# asm 2: vmov.i64 <h24=d13,#0
vmov.i64 d13,#0

# qhasm:   h31[0,1] += f8_2f9_f9_f6[2] signed* 19g4_19g5_19g6_19g7[0];h31[2,3] += f8_2f9_f9_f6[3] signed* 19g4_19g5_19g6_19g7[1] 
# asm 1: vmlal.s32 <h31=reg128#6,<f8_2f9_f9_f6=reg128#15%top,<19g4_19g5_19g6_19g7=reg128#10%bot
# asm 2: vmlal.s32 <h31=q5,<f8_2f9_f9_f6=d29,<19g4_19g5_19g6_19g7=d18
vmlal.s32 q5,d29,d18

# qhasm:   g4_19g5_g6_19g7 = g4_19g5_g6_19g7[0,1]19g4_19g5_19g6_19g7[3]g4_g5_g6_g7[2] 
# asm 1: vext.32 <g4_19g5_g6_19g7=reg128#13%top,<19g4_19g5_19g6_19g7=reg128#10%top,<g4_g5_g6_g7=reg128#4%top,#1
# asm 2: vext.32 <g4_19g5_g6_19g7=d25,<19g4_19g5_19g6_19g7=d19,<g4_g5_g6_g7=d7,#1
vext.32 d25,d19,d7,#1

# qhasm:   h24[0,1] += f0_2f1_f2_2f3[0] signed* g0_g1_g2_g3[2];h24[2,3] += f0_2f1_f2_2f3[1] signed* g0_g1_g2_g3[3] 
# asm 1: vmlal.s32 <h24=reg128#7,<f0_2f1_f2_2f3=reg128#11%bot,<g0_g1_g2_g3=reg128#3%top
# asm 2: vmlal.s32 <h24=q6,<f0_2f1_f2_2f3=d20,<g0_g1_g2_g3=d5
vmlal.s32 q6,d20,d5

# qhasm:   g4_19g5_g6_19g7 = g4_19g5_g6_19g7[0,1]g4_19g5_g6_19g7[3]g4_19g5_g6_19g7[2] 
# asm 1: vrev64.i32 <g4_19g5_g6_19g7=reg128#13%top,<g4_19g5_g6_19g7=reg128#13%top
# asm 2: vrev64.i32 <g4_19g5_g6_19g7=d25,<g4_19g5_g6_19g7=d25
vrev64.i32 d25,d25

# qhasm:   h24[0,1] += f0_2f1_f2_2f3[2] signed* g0_g1_g2_g3[0];h24[2,3] += f0_2f1_f2_2f3[3] signed* g0_g1_g2_g3[1] 
# asm 1: vmlal.s32 <h24=reg128#7,<f0_2f1_f2_2f3=reg128#11%top,<g0_g1_g2_g3=reg128#3%bot
# asm 2: vmlal.s32 <h24=q6,<f0_2f1_f2_2f3=d21,<g0_g1_g2_g3=d4
vmlal.s32 q6,d21,d4

# qhasm:   mem64[playp] aligned= h31[1];playp+=8 
# asm 1: vst1.8 <h31=reg128#6%top,[<playp=int32#8,: 64]!
# asm 2: vst1.8 <h31=d11,[<playp=r7,: 64]!
vst1.8 d11,[r7,: 64]!

# qhasm:   h24[0,1] += f4_2f5_f6_2f7[0] signed* g8_19g9_19g8_19g9[2];h24[2,3] += f4_2f5_f6_2f7[1] signed* g8_19g9_19g8_19g9[3] 
# asm 1: vmlal.s32 <h24=reg128#7,<f4_2f5_f6_2f7=reg128#14%bot,<g8_19g9_19g8_19g9=reg128#1%top
# asm 2: vmlal.s32 <h24=q6,<f4_2f5_f6_2f7=d26,<g8_19g9_19g8_19g9=d1
vmlal.s32 q6,d26,d1

# qhasm:   new h53 

# qhasm:   h53 = h53[0,1]h31[0,1] 
# asm 1: vext.32 <h53=reg128#5%top,<h31=reg128#6%bot,<h31=reg128#6%bot,#0
# asm 2: vext.32 <h53=d9,<h31=d10,<h31=d10,#0
vext.32 d9,d10,d10,#0

# qhasm:   h24[0,1] += f4_2f5_f6_2f7[2] signed* 19g4_19g5_19g6_19g7[2];h24[2,3] += f4_2f5_f6_2f7[3] signed* 19g4_19g5_19g6_19g7[3] 
# asm 1: vmlal.s32 <h24=reg128#7,<f4_2f5_f6_2f7=reg128#14%top,<19g4_19g5_19g6_19g7=reg128#10%top
# asm 2: vmlal.s32 <h24=q6,<f4_2f5_f6_2f7=d27,<19g4_19g5_19g6_19g7=d19
vmlal.s32 q6,d27,d19

# qhasm:   h53 = 0,h53[1] 
# asm 1: vmov.i64 <h53=reg128#5%bot,#0
# asm 2: vmov.i64 <h53=d8,#0
vmov.i64 d8,#0

# qhasm:   h24[0,1] += f8_2f9_f9_f6[0] signed* 19g4_19g5_19g6_19g7[0];h24[2,3] += f8_2f9_f9_f6[1] signed* 19g4_19g5_19g6_19g7[1] 
# asm 1: vmlal.s32 <h24=reg128#7,<f8_2f9_f9_f6=reg128#15%bot,<19g4_19g5_19g6_19g7=reg128#10%bot
# asm 2: vmlal.s32 <h24=q6,<f8_2f9_f9_f6=d28,<19g4_19g5_19g6_19g7=d18
vmlal.s32 q6,d28,d18

# qhasm:   h53[0,1] += f1_f8_f3_f0[0] signed* g4_19g5_g6_19g7[0];h53[2,3] += f1_f8_f3_f0[1] signed* g4_19g5_g6_19g7[1] 
# asm 1: vmlal.s32 <h53=reg128#5,<f1_f8_f3_f0=reg128#9%bot,<g4_19g5_g6_19g7=reg128#13%bot
# asm 2: vmlal.s32 <h53=q4,<f1_f8_f3_f0=d16,<g4_19g5_g6_19g7=d24
vmlal.s32 q4,d16,d24

# qhasm:   h53[0,1] += f1_f8_f3_f0[2] signed* g0_g1_g2_g3[2];h53[2,3] += f1_f8_f3_f0[3] signed* g0_g1_g2_g3[3] 
# asm 1: vmlal.s32 <h53=reg128#5,<f1_f8_f3_f0=reg128#9%top,<g0_g1_g2_g3=reg128#3%top
# asm 2: vmlal.s32 <h53=q4,<f1_f8_f3_f0=d17,<g0_g1_g2_g3=d5
vmlal.s32 q4,d17,d5

# qhasm:   h53[0,1] += f5_f2_f7_f4[0] signed* g0_g1_g2_g3[0];h53[2,3] += f5_f2_f7_f4[1] signed* g0_g1_g2_g3[1] 
# asm 1: vmlal.s32 <h53=reg128#5,<f5_f2_f7_f4=reg128#8%bot,<g0_g1_g2_g3=reg128#3%bot
# asm 2: vmlal.s32 <h53=q4,<f5_f2_f7_f4=d14,<g0_g1_g2_g3=d4
vmlal.s32 q4,d14,d4

# qhasm:   mem64[playp] aligned= h24[0];playp+=8 
# asm 1: vst1.8 <h24=reg128#7%bot,[<playp=int32#8,: 64]!
# asm 2: vst1.8 <h24=d12,[<playp=r7,: 64]!
vst1.8 d12,[r7,: 64]!

# qhasm:   h53[0,1] += f5_f2_f7_f4[2] signed* g8_19g9_19g8_19g9[2];h53[2,3] += f5_f2_f7_f4[3] signed* g8_19g9_19g8_19g9[3] 
# asm 1: vmlal.s32 <h53=reg128#5,<f5_f2_f7_f4=reg128#8%top,<g8_19g9_19g8_19g9=reg128#1%top
# asm 2: vmlal.s32 <h53=q4,<f5_f2_f7_f4=d15,<g8_19g9_19g8_19g9=d1
vmlal.s32 q4,d15,d1

# qhasm:   new h46 

# qhasm:   h46 = h24[2,3]h46[2,3] 
# asm 1: vext.32 <h46=reg128#6%bot,<h24=reg128#7%top,<h24=reg128#7%bot,#0
# asm 2: vext.32 <h46=d10,<h24=d13,<h24=d12,#0
vext.32 d10,d13,d12,#0

# qhasm:   h53[0,1] += f8_2f9_f9_f6[2] signed* 19g4_19g5_19g6_19g7[2];h53[2,3] += f8_2f9_f9_f6[3] signed* 19g4_19g5_19g6_19g7[3] 
# asm 1: vmlal.s32 <h53=reg128#5,<f8_2f9_f9_f6=reg128#15%top,<19g4_19g5_19g6_19g7=reg128#10%top
# asm 2: vmlal.s32 <h53=q4,<f8_2f9_f9_f6=d29,<19g4_19g5_19g6_19g7=d19
vmlal.s32 q4,d29,d19

# qhasm:   h46 = h46[0],0 
# asm 1: vmov.i64 <h46=reg128#6%top,#0
# asm 2: vmov.i64 <h46=d11,#0
vmov.i64 d11,#0

# qhasm:   h46[0,1] += f0_2f1_f2_2f3[0] signed* g4_g5_g6_g7[0];h46[2,3] += f0_2f1_f2_2f3[1] signed* g4_g5_g6_g7[1] 
# asm 1: vmlal.s32 <h46=reg128#6,<f0_2f1_f2_2f3=reg128#11%bot,<g4_g5_g6_g7=reg128#4%bot
# asm 2: vmlal.s32 <h46=q5,<f0_2f1_f2_2f3=d20,<g4_g5_g6_g7=d6
vmlal.s32 q5,d20,d6

# qhasm:   h46[0,1] += f0_2f1_f2_2f3[2] signed* g0_g1_g2_g3[2];h46[2,3] += f0_2f1_f2_2f3[3] signed* g0_g1_g2_g3[3] 
# asm 1: vmlal.s32 <h46=reg128#6,<f0_2f1_f2_2f3=reg128#11%top,<g0_g1_g2_g3=reg128#3%top
# asm 2: vmlal.s32 <h46=q5,<f0_2f1_f2_2f3=d21,<g0_g1_g2_g3=d5
vmlal.s32 q5,d21,d5

# qhasm:   h46[0,1] += f4_2f5_f6_2f7[0] signed* g0_g1_g2_g3[0];h46[2,3] += f4_2f5_f6_2f7[1] signed* g0_g1_g2_g3[1] 
# asm 1: vmlal.s32 <h46=reg128#6,<f4_2f5_f6_2f7=reg128#14%bot,<g0_g1_g2_g3=reg128#3%bot
# asm 2: vmlal.s32 <h46=q5,<f4_2f5_f6_2f7=d26,<g0_g1_g2_g3=d4
vmlal.s32 q5,d26,d4

# qhasm:   new h75 

# qhasm:   h75 = h75[0,1]h53[0,1] 
# asm 1: vext.32 <h75=reg128#7%top,<h53=reg128#5%bot,<h53=reg128#5%bot,#0
# asm 2: vext.32 <h75=d13,<h53=d8,<h53=d8,#0
vext.32 d13,d8,d8,#0

# qhasm:   h46[0,1] += f4_2f5_f6_2f7[2] signed* g8_19g9_19g8_19g9[2];h46[2,3] += f4_2f5_f6_2f7[3] signed* g8_19g9_19g8_19g9[3] 
# asm 1: vmlal.s32 <h46=reg128#6,<f4_2f5_f6_2f7=reg128#14%top,<g8_19g9_19g8_19g9=reg128#1%top
# asm 2: vmlal.s32 <h46=q5,<f4_2f5_f6_2f7=d27,<g8_19g9_19g8_19g9=d1
vmlal.s32 q5,d27,d1

# qhasm:   h75 = 0,h75[1] 
# asm 1: vmov.i64 <h75=reg128#7%bot,#0
# asm 2: vmov.i64 <h75=d12,#0
vmov.i64 d12,#0

# qhasm:   h46[0,1] += f8_2f9_f9_f6[0] signed* 19g4_19g5_19g6_19g7[2];h46[2,3] += f8_2f9_f9_f6[1] signed* 19g4_19g5_19g6_19g7[3] 
# asm 1: vmlal.s32 <h46=reg128#6,<f8_2f9_f9_f6=reg128#15%bot,<19g4_19g5_19g6_19g7=reg128#10%top
# asm 2: vmlal.s32 <h46=q5,<f8_2f9_f9_f6=d28,<19g4_19g5_19g6_19g7=d19
vmlal.s32 q5,d28,d19

# qhasm:   mem64[playp] aligned= h53[1];playp+=8 
# asm 1: vst1.8 <h53=reg128#5%top,[<playp=int32#8,: 64]!
# asm 2: vst1.8 <h53=d9,[<playp=r7,: 64]!
vst1.8 d9,[r7,: 64]!

# qhasm:   h75[0,1] += f1_f8_f3_f0[0] signed* g4_19g5_g6_19g7[2];h75[2,3] += f1_f8_f3_f0[1] signed* g4_19g5_g6_19g7[3] 
# asm 1: vmlal.s32 <h75=reg128#7,<f1_f8_f3_f0=reg128#9%bot,<g4_19g5_g6_19g7=reg128#13%top
# asm 2: vmlal.s32 <h75=q6,<f1_f8_f3_f0=d16,<g4_19g5_g6_19g7=d25
vmlal.s32 q6,d16,d25

# qhasm:   h75[0,1] += f1_f8_f3_f0[2] signed* g4_g5_g6_g7[0];h75[2,3] += f1_f8_f3_f0[3] signed* g4_g5_g6_g7[1] 
# asm 1: vmlal.s32 <h75=reg128#7,<f1_f8_f3_f0=reg128#9%top,<g4_g5_g6_g7=reg128#4%bot
# asm 2: vmlal.s32 <h75=q6,<f1_f8_f3_f0=d17,<g4_g5_g6_g7=d6
vmlal.s32 q6,d17,d6

# qhasm:   mem64[playp] aligned= h46[0] 
# asm 1: vst1.8 <h46=reg128#6%bot,[<playp=int32#8,: 64]
# asm 2: vst1.8 <h46=d10,[<playp=r7,: 64]
vst1.8 d10,[r7,: 64]

# qhasm:   h75[0,1] += f5_f2_f7_f4[0] signed* g0_g1_g2_g3[2];h75[2,3] += f5_f2_f7_f4[1] signed* g0_g1_g2_g3[3] 
# asm 1: vmlal.s32 <h75=reg128#7,<f5_f2_f7_f4=reg128#8%bot,<g0_g1_g2_g3=reg128#3%top
# asm 2: vmlal.s32 <h75=q6,<f5_f2_f7_f4=d14,<g0_g1_g2_g3=d5
vmlal.s32 q6,d14,d5

# qhasm:   new h68 

# qhasm:   h68 = h46[2,3]h68[2,3] 
# asm 1: vext.32 <h68=reg128#5%bot,<h46=reg128#6%top,<h46=reg128#6%bot,#0
# asm 2: vext.32 <h68=d8,<h46=d11,<h46=d10,#0
vext.32 d8,d11,d10,#0

# qhasm:   h75[0,1] += f5_f2_f7_f4[2] signed* g0_g1_g2_g3[0];h75[2,3] += f5_f2_f7_f4[3] signed* g0_g1_g2_g3[1] 
# asm 1: vmlal.s32 <h75=reg128#7,<f5_f2_f7_f4=reg128#8%top,<g0_g1_g2_g3=reg128#3%bot
# asm 2: vmlal.s32 <h75=q6,<f5_f2_f7_f4=d15,<g0_g1_g2_g3=d4
vmlal.s32 q6,d15,d4

# qhasm:   h68 = h68[0],0 
# asm 1: vmov.i64 <h68=reg128#5%top,#0
# asm 2: vmov.i64 <h68=d9,#0
vmov.i64 d9,#0

# qhasm:   h75[0,1] += f8_2f9_f9_f6[2] signed* g8_19g9_19g8_19g9[2];h75[2,3] += f8_2f9_f9_f6[3] signed* g8_19g9_19g8_19g9[3] 
# asm 1: vmlal.s32 <h75=reg128#7,<f8_2f9_f9_f6=reg128#15%top,<g8_19g9_19g8_19g9=reg128#1%top
# asm 2: vmlal.s32 <h75=q6,<f8_2f9_f9_f6=d29,<g8_19g9_19g8_19g9=d1
vmlal.s32 q6,d29,d1

# qhasm:   h68[0,1] += f0_2f1_f2_2f3[0] signed* g4_g5_g6_g7[2];h68[2,3] += f0_2f1_f2_2f3[1] signed* g4_g5_g6_g7[3] 
# asm 1: vmlal.s32 <h68=reg128#5,<f0_2f1_f2_2f3=reg128#11%bot,<g4_g5_g6_g7=reg128#4%top
# asm 2: vmlal.s32 <h68=q4,<f0_2f1_f2_2f3=d20,<g4_g5_g6_g7=d7
vmlal.s32 q4,d20,d7

# qhasm:   h68[0,1] += f0_2f1_f2_2f3[2] signed* g4_g5_g6_g7[0];h68[2,3] += f0_2f1_f2_2f3[3] signed* g4_g5_g6_g7[1] 
# asm 1: vmlal.s32 <h68=reg128#5,<f0_2f1_f2_2f3=reg128#11%top,<g4_g5_g6_g7=reg128#4%bot
# asm 2: vmlal.s32 <h68=q4,<f0_2f1_f2_2f3=d21,<g4_g5_g6_g7=d6
vmlal.s32 q4,d21,d6

# qhasm:   h68[0,1] += f4_2f5_f6_2f7[0] signed* g0_g1_g2_g3[2];h68[2,3] += f4_2f5_f6_2f7[1] signed* g0_g1_g2_g3[3] 
# asm 1: vmlal.s32 <h68=reg128#5,<f4_2f5_f6_2f7=reg128#14%bot,<g0_g1_g2_g3=reg128#3%top
# asm 2: vmlal.s32 <h68=q4,<f4_2f5_f6_2f7=d26,<g0_g1_g2_g3=d5
vmlal.s32 q4,d26,d5

# qhasm:   new h97 

# qhasm:   h97 = h97[0,1]h75[0,1] 
# asm 1: vext.32 <h97=reg128#6%top,<h75=reg128#7%bot,<h75=reg128#7%bot,#0
# asm 2: vext.32 <h97=d11,<h75=d12,<h75=d12,#0
vext.32 d11,d12,d12,#0

# qhasm:   h68[0,1] += f4_2f5_f6_2f7[2] signed* g0_g1_g2_g3[0];h68[2,3] += f4_2f5_f6_2f7[3] signed* g0_g1_g2_g3[1] 
# asm 1: vmlal.s32 <h68=reg128#5,<f4_2f5_f6_2f7=reg128#14%top,<g0_g1_g2_g3=reg128#3%bot
# asm 2: vmlal.s32 <h68=q4,<f4_2f5_f6_2f7=d27,<g0_g1_g2_g3=d4
vmlal.s32 q4,d27,d4

# qhasm:   h97 = 0,h97[1] 
# asm 1: vmov.i64 <h97=reg128#6%bot,#0
# asm 2: vmov.i64 <h97=d10,#0
vmov.i64 d10,#0

# qhasm:   h68[0,1] += f8_2f9_f9_f6[0] signed* g8_19g9_19g8_19g9[2];h68[2,3] += f8_2f9_f9_f6[1] signed* g8_19g9_19g8_19g9[3] 
# asm 1: vmlal.s32 <h68=reg128#5,<f8_2f9_f9_f6=reg128#15%bot,<g8_19g9_19g8_19g9=reg128#1%top
# asm 2: vmlal.s32 <h68=q4,<f8_2f9_f9_f6=d28,<g8_19g9_19g8_19g9=d1
vmlal.s32 q4,d28,d1

# qhasm:   h97[0,1] += f1_f8_f3_f0[0] signed* g8_19g9_19g8_19g9[0];h97[2,3] += f1_f8_f3_f0[1] signed* g8_19g9_19g8_19g9[1] 
# asm 1: vmlal.s32 <h97=reg128#6,<f1_f8_f3_f0=reg128#9%bot,<g8_19g9_19g8_19g9=reg128#1%bot
# asm 2: vmlal.s32 <h97=q5,<f1_f8_f3_f0=d16,<g8_19g9_19g8_19g9=d0
vmlal.s32 q5,d16,d0

# qhasm:   playp -= 32 
# asm 1: sub >playp=int32#7,<playp=int32#8,#32
# asm 2: sub >playp=r6,<playp=r7,#32
sub r6,r7,#32

# qhasm:   h97[0,1] += f1_f8_f3_f0[2] signed* g4_g5_g6_g7[2];h97[2,3] += f1_f8_f3_f0[3] signed* g4_g5_g6_g7[3] 
# asm 1: vmlal.s32 <h97=reg128#6,<f1_f8_f3_f0=reg128#9%top,<g4_g5_g6_g7=reg128#4%top
# asm 2: vmlal.s32 <h97=q5,<f1_f8_f3_f0=d17,<g4_g5_g6_g7=d7
vmlal.s32 q5,d17,d7

# qhasm:   h97[0,1] += f5_f2_f7_f4[0] signed* g4_g5_g6_g7[0];h97[2,3] += f5_f2_f7_f4[1] signed* g4_g5_g6_g7[1] 
# asm 1: vmlal.s32 <h97=reg128#6,<f5_f2_f7_f4=reg128#8%bot,<g4_g5_g6_g7=reg128#4%bot
# asm 2: vmlal.s32 <h97=q5,<f5_f2_f7_f4=d14,<g4_g5_g6_g7=d6
vmlal.s32 q5,d14,d6

# qhasm:   new h80 

# qhasm:   h80 = h68[2,3]h80[2,3] 
# asm 1: vext.32 <h80=reg128#16%bot,<h68=reg128#5%top,<h68=reg128#5%bot,#0
# asm 2: vext.32 <h80=d30,<h68=d9,<h68=d8,#0
vext.32 d30,d9,d8,#0

# qhasm:   h97[0,1] += f5_f2_f7_f4[2] signed* g0_g1_g2_g3[2];h97[2,3] += f5_f2_f7_f4[3] signed* g0_g1_g2_g3[3] 
# asm 1: vmlal.s32 <h97=reg128#6,<f5_f2_f7_f4=reg128#8%top,<g0_g1_g2_g3=reg128#3%top
# asm 2: vmlal.s32 <h97=q5,<f5_f2_f7_f4=d15,<g0_g1_g2_g3=d5
vmlal.s32 q5,d15,d5

# qhasm:   h80 aligned= h80[0]mem64[playp];playp+=8 
# asm 1: vld1.8 {<h80=reg128#16%top},[<playp=int32#7,: 64]!
# asm 2: vld1.8 {<h80=d31},[<playp=r6,: 64]!
vld1.8 {d31},[r6,: 64]!

# qhasm:   h97[0,1] += f8_2f9_f9_f6[2] signed* g0_g1_g2_g3[0];h97[2,3] += f8_2f9_f9_f6[3] signed* g0_g1_g2_g3[1] 
# asm 1: vmlal.s32 <h97=reg128#6,<f8_2f9_f9_f6=reg128#15%top,<g0_g1_g2_g3=reg128#3%bot
# asm 2: vmlal.s32 <h97=q5,<f8_2f9_f9_f6=d29,<g0_g1_g2_g3=d4
vmlal.s32 q5,d29,d4

# qhasm:   h80[0,1] += f0_2f1_f2_2f3[0] signed* g8_19g9_19g8_19g9[0];h80[2,3] += f0_2f1_f2_2f3[1] signed* g8_19g9_19g8_19g9[1] 
# asm 1: vmlal.s32 <h80=reg128#16,<f0_2f1_f2_2f3=reg128#11%bot,<g8_19g9_19g8_19g9=reg128#1%bot
# asm 2: vmlal.s32 <h80=q15,<f0_2f1_f2_2f3=d20,<g8_19g9_19g8_19g9=d0
vmlal.s32 q15,d20,d0

# qhasm:   new 19g4_g5_19g6_g7 

# qhasm:   19g4_g5_19g6_g7 = g4_g5_g6_g7[1]19g4_19g5_19g6_19g7[0]19g4_g5_19g6_g7[2,3] 
# asm 1: vext.32 <19g4_g5_19g6_g7=reg128#1%bot,<g4_g5_g6_g7=reg128#4%bot,<19g4_19g5_19g6_19g7=reg128#10%bot,#1
# asm 2: vext.32 <19g4_g5_19g6_g7=d0,<g4_g5_g6_g7=d6,<19g4_19g5_19g6_19g7=d18,#1
vext.32 d0,d6,d18,#1

# qhasm:   h80[0,1] += f0_2f1_f2_2f3[2] signed* g4_19g5_g6_19g7[2];h80[2,3] += f0_2f1_f2_2f3[3] signed* g4_19g5_g6_19g7[3] 
# asm 1: vmlal.s32 <h80=reg128#16,<f0_2f1_f2_2f3=reg128#11%top,<g4_19g5_g6_19g7=reg128#13%top
# asm 2: vmlal.s32 <h80=q15,<f0_2f1_f2_2f3=d21,<g4_19g5_g6_19g7=d25
vmlal.s32 q15,d21,d25

# qhasm:   19g4_g5_19g6_g7 = 19g4_g5_19g6_g7[1]19g4_g5_19g6_g7[0]19g4_g5_19g6_g7[2,3] 
# asm 1: vrev64.i32 <19g4_g5_19g6_g7=reg128#1%bot,<19g4_g5_19g6_g7=reg128#1%bot
# asm 2: vrev64.i32 <19g4_g5_19g6_g7=d0,<19g4_g5_19g6_g7=d0
vrev64.i32 d0,d0

# qhasm:   h80[0,1] += f4_2f5_f6_2f7[0] signed* g4_19g5_g6_19g7[0];h80[2,3] += f4_2f5_f6_2f7[1] signed* g4_19g5_g6_19g7[1] 
# asm 1: vmlal.s32 <h80=reg128#16,<f4_2f5_f6_2f7=reg128#14%bot,<g4_19g5_g6_19g7=reg128#13%bot
# asm 2: vmlal.s32 <h80=q15,<f4_2f5_f6_2f7=d26,<g4_19g5_g6_19g7=d24
vmlal.s32 q15,d26,d24

# qhasm:   19g4_g5_19g6_g7 = 19g4_g5_19g6_g7[0,1]g4_g5_g6_g7[3]19g4_19g5_19g6_19g7[2] 
# asm 1: vext.32 <19g4_g5_19g6_g7=reg128#1%top,<g4_g5_g6_g7=reg128#4%top,<19g4_19g5_19g6_19g7=reg128#10%top,#1
# asm 2: vext.32 <19g4_g5_19g6_g7=d1,<g4_g5_g6_g7=d7,<19g4_19g5_19g6_19g7=d19,#1
vext.32 d1,d7,d19,#1

# qhasm:   new h19 

# qhasm:   h19 = h19[0,1]h97[0,1] 
# asm 1: vext.32 <h19=reg128#4%top,<h97=reg128#6%bot,<h97=reg128#6%bot,#0
# asm 2: vext.32 <h19=d7,<h97=d10,<h97=d10,#0
vext.32 d7,d10,d10,#0

# qhasm:   h80[0,1] += f4_2f5_f6_2f7[2] signed* g0_19g1_g2_19g3[2];h80[2,3] += f4_2f5_f6_2f7[3] signed* g0_19g1_g2_19g3[3] 
# asm 1: vmlal.s32 <h80=reg128#16,<f4_2f5_f6_2f7=reg128#14%top,<g0_19g1_g2_19g3=reg128#12%top
# asm 2: vmlal.s32 <h80=q15,<f4_2f5_f6_2f7=d27,<g0_19g1_g2_19g3=d23
vmlal.s32 q15,d27,d23

# qhasm:   19g4_g5_19g6_g7 = 19g4_g5_19g6_g7[0,1]19g4_g5_19g6_g7[3]19g4_g5_19g6_g7[2] 
# asm 1: vrev64.i32 <19g4_g5_19g6_g7=reg128#1%top,<19g4_g5_19g6_g7=reg128#1%top
# asm 2: vrev64.i32 <19g4_g5_19g6_g7=d1,<19g4_g5_19g6_g7=d1
vrev64.i32 d1,d1

# qhasm:   h19 aligned= mem64[playp]h19[1] 
# asm 1: vld1.8 {<h19=reg128#4%bot},[<playp=int32#7,: 64]
# asm 2: vld1.8 {<h19=d6},[<playp=r6,: 64]
vld1.8 {d6},[r6,: 64]

# qhasm:   h80[0,1] += f8_2f9_f9_f6[0] signed* g0_19g1_g2_19g3[0];h80[2,3] += f8_2f9_f9_f6[1] signed* g0_19g1_g2_19g3[1] 
# asm 1: vmlal.s32 <h80=reg128#16,<f8_2f9_f9_f6=reg128#15%bot,<g0_19g1_g2_19g3=reg128#12%bot
# asm 2: vmlal.s32 <h80=q15,<f8_2f9_f9_f6=d28,<g0_19g1_g2_19g3=d22
vmlal.s32 q15,d28,d22

# qhasm:   h19[0,1] += f1_f8_f3_f0[0] signed* g0_g1_g2_g3[0];h19[2,3] += f1_f8_f3_f0[1] signed* g0_g1_g2_g3[1] 
# asm 1: vmlal.s32 <h19=reg128#4,<f1_f8_f3_f0=reg128#9%bot,<g0_g1_g2_g3=reg128#3%bot
# asm 2: vmlal.s32 <h19=q3,<f1_f8_f3_f0=d16,<g0_g1_g2_g3=d4
vmlal.s32 q3,d16,d4

# qhasm:   playp+=24 
# asm 1: add >playp=int32#7,<playp=int32#7,#24
# asm 2: add >playp=r6,<playp=r6,#24
add r6,r6,#24

# qhasm:   h19[0,1] += f1_f8_f3_f0[2] signed* 19g8_g9_19g2_g3[0];h19[2,3] += f1_f8_f3_f0[3] signed* 19g8_g9_19g2_g3[1] 
# asm 1: vmlal.s32 <h19=reg128#4,<f1_f8_f3_f0=reg128#9%top,<19g8_g9_19g2_g3=reg128#2%bot
# asm 2: vmlal.s32 <h19=q3,<f1_f8_f3_f0=d17,<19g8_g9_19g2_g3=d2
vmlal.s32 q3,d17,d2

# qhasm:   new h04 

# qhasm:   h04 = h80[2,3]h04[2,3] 
# asm 1: vext.32 <h04=reg128#3%bot,<h80=reg128#16%top,<h80=reg128#16%bot,#0
# asm 2: vext.32 <h04=d4,<h80=d31,<h80=d30,#0
vext.32 d4,d31,d30,#0

# qhasm:   new h37 

# qhasm:   h37 = h37[0]h97[1] 
# asm 1: vmov <h37=reg128#9%top,<h97=reg128#6%top
# asm 2: vmov <h37=d17,<h97=d11
vmov d17,d11

# qhasm:   h19[0,1] += f5_f2_f7_f4[0] signed* 19g4_g5_19g6_g7[2];h19[2,3] += f5_f2_f7_f4[1] signed* 19g4_g5_19g6_g7[3] 
# asm 1: vmlal.s32 <h19=reg128#4,<f5_f2_f7_f4=reg128#8%bot,<19g4_g5_19g6_g7=reg128#1%top
# asm 2: vmlal.s32 <h19=q3,<f5_f2_f7_f4=d14,<19g4_g5_19g6_g7=d1
vmlal.s32 q3,d14,d1

# qhasm:   new h15 

# qhasm:   h15 = h15[0,1]h75[2,3] 
# asm 1: vext.32 <h15=reg128#6%top,<h75=reg128#7%top,<h75=reg128#7%top,#0
# asm 2: vext.32 <h15=d11,<h75=d13,<h75=d13,#0
vext.32 d11,d13,d13,#0

# qhasm:   new h48 

# qhasm:   h48 = h48[0,1]h80[0,1] 
# asm 1: vext.32 <h48=reg128#7%top,<h80=reg128#16%bot,<h80=reg128#16%bot,#0
# asm 2: vext.32 <h48=d13,<h80=d30,<h80=d30,#0
vext.32 d13,d30,d30,#0

# qhasm:   h19[0,1] += f5_f2_f7_f4[2] signed* 19g4_g5_19g6_g7[0];h19[2,3] += f5_f2_f7_f4[3] signed* 19g4_g5_19g6_g7[1] 
# asm 1: vmlal.s32 <h19=reg128#4,<f5_f2_f7_f4=reg128#8%top,<19g4_g5_19g6_g7=reg128#1%bot
# asm 2: vmlal.s32 <h19=q3,<f5_f2_f7_f4=d15,<19g4_g5_19g6_g7=d0
vmlal.s32 q3,d15,d0

# qhasm:   new h26 

# qhasm:   h26 = h26[0,1]h68[0,1] 
# asm 1: vext.32 <h26=reg128#1%top,<h68=reg128#5%bot,<h68=reg128#5%bot,#0
# asm 2: vext.32 <h26=d1,<h68=d8,<h68=d8,#0
vext.32 d1,d8,d8,#0

# qhasm:   h19[0,1] += f8_2f9_f9_f6[2] signed* 19g8_g9_19g2_g3[2];h19[2,3] += f8_2f9_f9_f6[3] signed* 19g8_g9_19g2_g3[3] 
# asm 1: vmlal.s32 <h19=reg128#4,<f8_2f9_f9_f6=reg128#15%top,<19g8_g9_19g2_g3=reg128#2%top
# asm 2: vmlal.s32 <h19=q3,<f8_2f9_f9_f6=d29,<19g8_g9_19g2_g3=d3
vmlal.s32 q3,d29,d3

# qhasm:   h04 aligned= h04[0]mem64[playp] 
# asm 1: vld1.8 {<h04=reg128#3%top},[<playp=int32#7,: 64]
# asm 2: vld1.8 {<h04=d5},[<playp=r6,: 64]
vld1.8 {d5},[r6,: 64]

# qhasm:   playp -= 16 
# asm 1: sub >playp=int32#7,<playp=int32#7,#16
# asm 2: sub >playp=r6,<playp=r6,#16
sub r6,r6,#16

# qhasm:   h15 = h19[0,1]h15[2,3] 
# asm 1: vext.32 <h15=reg128#6%bot,<h19=reg128#4%bot,<h19=reg128#4%bot,#0
# asm 2: vext.32 <h15=d10,<h19=d6,<h19=d6,#0
vext.32 d10,d6,d6,#0

# qhasm:   4x mask26 = 0xffffffff 
# asm 1: vmov.i32 >mask26=reg128#2,#0xffffffff
# asm 2: vmov.i32 >mask26=q1,#0xffffffff
vmov.i32 q1,#0xffffffff

# qhasm:   2x mask25 = mask26 << 25 
# asm 1: vshl.i64 >mask25=reg128#5,<mask26=reg128#2,#25
# asm 2: vshl.i64 >mask25=q4,<mask26=q1,#25
vshl.i64 q4,q1,#25

# qhasm:   ptr = &_0x2000000_stack 
# asm 1: lea >ptr=int32#8,<_0x2000000_stack=stack128#1
# asm 2: lea >ptr=r7,<_0x2000000_stack=[sp,#512]
add r7,sp,#512

# qhasm:   _0x2000000 aligned= mem128[ptr] 
# asm 1: vld1.8 {>_0x2000000=reg128#8%bot->_0x2000000=reg128#8%top},[<ptr=int32#8,: 128]
# asm 2: vld1.8 {>_0x2000000=d14->_0x2000000=d15},[<ptr=r7,: 128]
vld1.8 {d14-d15},[r7,: 128]

# qhasm:   2x t0 = h04 + _0x2000000 
# asm 1: vadd.i64 >t0=reg128#10,<h04=reg128#3,<_0x2000000=reg128#8
# asm 2: vadd.i64 >t0=q9,<h04=q2,<_0x2000000=q7
vadd.i64 q9,q2,q7

# qhasm:   2x mask26 <<= 26 
# asm 1: vshl.i64 >mask26=reg128#2,<mask26=reg128#2,#26
# asm 2: vshl.i64 >mask26=q1,<mask26=q1,#26
vshl.i64 q1,q1,#26

# qhasm:   2x c = t0 signed>> 26 
# asm 1: vshr.s64 >c=reg128#11,<t0=reg128#10,#26
# asm 2: vshr.s64 >c=q10,<t0=q9,#26
vshr.s64 q10,q9,#26

# qhasm:   h26 aligned= mem64[playp]h26[1];playp += 8 
# asm 1: vld1.8 {<h26=reg128#1%bot},[<playp=int32#7,: 64]!
# asm 2: vld1.8 {<h26=d0},[<playp=r6,: 64]!
vld1.8 {d0},[r6,: 64]!

# qhasm:   2x h15 += c 
# asm 1: vadd.i64 >h15=reg128#6,<h15=reg128#6,<c=reg128#11
# asm 2: vadd.i64 >h15=q5,<h15=q5,<c=q10
vadd.i64 q5,q5,q10

# qhasm:   t0 &= mask26 
# asm 1: vand >t0=reg128#10,<t0=reg128#10,<mask26=reg128#2
# asm 2: vand >t0=q9,<t0=q9,<mask26=q1
vand q9,q9,q1

# qhasm:   h37 aligned= mem64[playp]h37[1];playp += 8 
# asm 1: vld1.8 {<h37=reg128#9%bot},[<playp=int32#7,: 64]!
# asm 2: vld1.8 {<h37=d16},[<playp=r6,: 64]!
vld1.8 {d16},[r6,: 64]!

# qhasm:   ptr = &_0x1000000_stack 
# asm 1: lea >ptr=int32#7,<_0x1000000_stack=stack128#2
# asm 2: lea >ptr=r6,<_0x1000000_stack=[sp,#528]
add r6,sp,#528

# qhasm:   _0x1000000 aligned= mem128[ptr] 
# asm 1: vld1.8 {>_0x1000000=reg128#11%bot->_0x1000000=reg128#11%top},[<ptr=int32#7,: 128]
# asm 2: vld1.8 {>_0x1000000=d20->_0x1000000=d21},[<ptr=r6,: 128]
vld1.8 {d20-d21},[r6,: 128]

# qhasm:   2x t1 = h15 + _0x1000000 
# asm 1: vadd.i64 >t1=reg128#12,<h15=reg128#6,<_0x1000000=reg128#11
# asm 2: vadd.i64 >t1=q11,<h15=q5,<_0x1000000=q10
vadd.i64 q11,q5,q10

# qhasm:   2x h04 -= t0 
# asm 1: vsub.i64 >h04=reg128#3,<h04=reg128#3,<t0=reg128#10
# asm 2: vsub.i64 >h04=q2,<h04=q2,<t0=q9
vsub.i64 q2,q2,q9

# qhasm:   2x c = t1 signed>> 25 
# asm 1: vshr.s64 >c=reg128#10,<t1=reg128#12,#25
# asm 2: vshr.s64 >c=q9,<t1=q11,#25
vshr.s64 q9,q11,#25

# qhasm:   h48 = h04[2,3]h48[2,3] 
# asm 1: vext.32 <h48=reg128#7%bot,<h04=reg128#3%top,<h04=reg128#3%bot,#0
# asm 2: vext.32 <h48=d12,<h04=d5,<h04=d4,#0
vext.32 d12,d5,d4,#0

# qhasm:   t1 &= mask25 
# asm 1: vand >t1=reg128#12,<t1=reg128#12,<mask25=reg128#5
# asm 2: vand >t1=q11,<t1=q11,<mask25=q4
vand q11,q11,q4

# qhasm:   2x h26 += c 
# asm 1: vadd.i64 >h26=reg128#1,<h26=reg128#1,<c=reg128#10
# asm 2: vadd.i64 >h26=q0,<h26=q0,<c=q9
vadd.i64 q0,q0,q9

# qhasm:   new h59 

# qhasm:   h59 = h59[0]h19[1] 
# asm 1: vmov <h59=reg128#10%top,<h19=reg128#4%top
# asm 2: vmov <h59=d19,<h19=d7
vmov d19,d7

# qhasm:   2x t0 = h26 + _0x2000000 
# asm 1: vadd.i64 >t0=reg128#4,<h26=reg128#1,<_0x2000000=reg128#8
# asm 2: vadd.i64 >t0=q3,<h26=q0,<_0x2000000=q7
vadd.i64 q3,q0,q7

# qhasm:   2x h15 -= t1 
# asm 1: vsub.i64 >h15=reg128#6,<h15=reg128#6,<t1=reg128#12
# asm 2: vsub.i64 >h15=q5,<h15=q5,<t1=q11
vsub.i64 q5,q5,q11

# qhasm:   2x c = t0 signed>> 26 
# asm 1: vshr.s64 >c=reg128#12,<t0=reg128#4,#26
# asm 2: vshr.s64 >c=q11,<t0=q3,#26
vshr.s64 q11,q3,#26

# qhasm:   h59 = h15[2,3]h59[2,3] 
# asm 1: vext.32 <h59=reg128#10%bot,<h15=reg128#6%top,<h15=reg128#6%bot,#0
# asm 2: vext.32 <h59=d18,<h15=d11,<h15=d10,#0
vext.32 d18,d11,d10,#0

# qhasm:   t0 &= mask26 
# asm 1: vand >t0=reg128#4,<t0=reg128#4,<mask26=reg128#2
# asm 2: vand >t0=q3,<t0=q3,<mask26=q1
vand q3,q3,q1

# qhasm:   2x h37 += c 
# asm 1: vadd.i64 >h37=reg128#9,<h37=reg128#9,<c=reg128#12
# asm 2: vadd.i64 >h37=q8,<h37=q8,<c=q11
vadd.i64 q8,q8,q11

# qhasm:   2x t1 = h37 + _0x1000000 
# asm 1: vadd.i64 >t1=reg128#12,<h37=reg128#9,<_0x1000000=reg128#11
# asm 2: vadd.i64 >t1=q11,<h37=q8,<_0x1000000=q10
vadd.i64 q11,q8,q10

# qhasm:   2x h26 -= t0 
# asm 1: vsub.i64 >h26=reg128#1,<h26=reg128#1,<t0=reg128#4
# asm 2: vsub.i64 >h26=q0,<h26=q0,<t0=q3
vsub.i64 q0,q0,q3

# qhasm:   2x c = t1 signed>> 25 
# asm 1: vshr.s64 >c=reg128#4,<t1=reg128#12,#25
# asm 2: vshr.s64 >c=q3,<t1=q11,#25
vshr.s64 q3,q11,#25

# qhasm:   t1 &= mask25 
# asm 1: vand >t1=reg128#12,<t1=reg128#12,<mask25=reg128#5
# asm 2: vand >t1=q11,<t1=q11,<mask25=q4
vand q11,q11,q4

# qhasm:   2x h48 += c 
# asm 1: vadd.i64 >h48=reg128#4,<h48=reg128#7,<c=reg128#4
# asm 2: vadd.i64 >h48=q3,<h48=q6,<c=q3
vadd.i64 q3,q6,q3

# qhasm:   2x t0 = h48 + _0x2000000 
# asm 1: vadd.i64 >t0=reg128#7,<h48=reg128#4,<_0x2000000=reg128#8
# asm 2: vadd.i64 >t0=q6,<h48=q3,<_0x2000000=q7
vadd.i64 q6,q3,q7

# qhasm:   2x h37 -= t1 
# asm 1: vsub.i64 >h37=reg128#9,<h37=reg128#9,<t1=reg128#12
# asm 2: vsub.i64 >h37=q8,<h37=q8,<t1=q11
vsub.i64 q8,q8,q11

# qhasm:   2x c = t0 signed>> 26 
# asm 1: vshr.s64 >c=reg128#12,<t0=reg128#7,#26
# asm 2: vshr.s64 >c=q11,<t0=q6,#26
vshr.s64 q11,q6,#26

# qhasm:   t0 &= mask26 
# asm 1: vand >t0=reg128#7,<t0=reg128#7,<mask26=reg128#2
# asm 2: vand >t0=q6,<t0=q6,<mask26=q1
vand q6,q6,q1

# qhasm:   2x h59 += c 
# asm 1: vadd.i64 >h59=reg128#10,<h59=reg128#10,<c=reg128#12
# asm 2: vadd.i64 >h59=q9,<h59=q9,<c=q11
vadd.i64 q9,q9,q11

# qhasm:   new t 

# qhasm:   t = t[0], h59[1] + _0x1000000[1] 
# asm 1: vadd.i64 <t=reg128#13%top,<h59=reg128#10%top,<_0x1000000=reg128#11%top
# asm 2: vadd.i64 <t=d25,<h59=d19,<_0x1000000=d21
vadd.i64 d25,d19,d21

# qhasm:   2x h48 -= t0 
# asm 1: vsub.i64 >h48=reg128#4,<h48=reg128#4,<t0=reg128#7
# asm 2: vsub.i64 >h48=q3,<h48=q3,<t0=q6
vsub.i64 q3,q3,q6

# qhasm:   c = c[0],t[1] signed>> 25 
# asm 1: vshr.s64 <c=reg128#12%top,<t=reg128#13%top,#25
# asm 2: vshr.s64 <c=d23,<t=d25,#25
vshr.s64 d23,d25,#25

# qhasm:   t &= mask25 
# asm 1: vand >t=reg128#5,<t=reg128#13,<mask25=reg128#5
# asm 2: vand >t=q4,<t=q12,<mask25=q4
vand q4,q12,q4

# qhasm:   new s2 

# qhasm:   s2 = s2[0],c[1] + c[1] 
# asm 1: vadd.i64 <s2=reg128#11%top,<c=reg128#12%top,<c=reg128#12%top
# asm 2: vadd.i64 <s2=d21,<c=d23,<c=d23
vadd.i64 d21,d23,d23

# qhasm:   new s 

# qhasm:   s = s[0],c[1] << 4 
# asm 1: vshl.i64 <s=reg128#13%top,<c=reg128#12%top,#4
# asm 2: vshl.i64 <s=d25,<c=d23,#4
vshl.i64 d25,d23,#4

# qhasm:   s2 = s2[0],s2[1] + c[1] 
# asm 1: vadd.i64 <s2=reg128#11%top,<s2=reg128#11%top,<c=reg128#12%top
# asm 2: vadd.i64 <s2=d21,<s2=d21,<c=d23
vadd.i64 d21,d21,d23

# qhasm:   s = s[0],s[1] + s2[1] 
# asm 1: vadd.i64 <s=reg128#13%top,<s=reg128#13%top,<s2=reg128#11%top
# asm 2: vadd.i64 <s=d25,<s=d25,<s2=d21
vadd.i64 d25,d25,d21

# qhasm:   h04 = h04[0] + s[1],h04[1] 
# asm 1: vadd.i64 <h04=reg128#3%bot,<h04=reg128#3%bot,<s=reg128#13%top
# asm 2: vadd.i64 <h04=d4,<h04=d4,<s=d25
vadd.i64 d4,d4,d25

# qhasm:   h26[0,1,2,3] h37[0,1,2,3] = h26[0]h37[0]h26[1]h37[1] h26[2]h37[2]h26[3]h37[3] 
# asm 1: vzip.i32 <h26=reg128#1,<h37=reg128#9
# asm 2: vzip.i32 <h26=q0,<h37=q8
vzip.i32 q0,q8

# qhasm:   t0 = h04[0] + _0x2000000[0],t0[1] 
# asm 1: vadd.i64 <t0=reg128#7%bot,<h04=reg128#3%bot,<_0x2000000=reg128#8%bot
# asm 2: vadd.i64 <t0=d12,<h04=d4,<_0x2000000=d14
vadd.i64 d12,d4,d14

# qhasm:   posh += 8 
# asm 1: add >posh=int32#7,<posh=int32#9,#8
# asm 2: add >posh=r6,<posh=r8,#8
add r6,r8,#8

# qhasm:   mem64[posh] aligned= h26[0] 
# asm 1: vst1.8 <h26=reg128#1%bot,[<posh=int32#7,: 64]
# asm 2: vst1.8 <h26=d0,[<posh=r6,: 64]
vst1.8 d0,[r6,: 64]

# qhasm:   h59 = h59[0],h59[1] - t[1] 
# asm 1: vsub.i64 <h59=reg128#10%top,<h59=reg128#10%top,<t=reg128#5%top
# asm 2: vsub.i64 <h59=d19,<h59=d19,<t=d9
vsub.i64 d19,d19,d9

# qhasm:   posh += 16 
# asm 1: add >posh=int32#7,<posh=int32#7,#16
# asm 2: add >posh=r6,<posh=r6,#16
add r6,r6,#16

# qhasm:   mem64[posh] aligned= h37[0] 
# asm 1: vst1.8 <h37=reg128#9%bot,[<posh=int32#7,: 64]
# asm 2: vst1.8 <h37=d16,[<posh=r6,: 64]
vst1.8 d16,[r6,: 64]

# qhasm:   c = t0[0] signed>> 26,c[1] 
# asm 1: vshr.s64 <c=reg128#12%bot,<t0=reg128#7%bot,#26
# asm 2: vshr.s64 <c=d22,<t0=d12,#26
vshr.s64 d22,d12,#26

# qhasm:   t0 &= mask26 
# asm 1: vand >t0=reg128#1,<t0=reg128#7,<mask26=reg128#2
# asm 2: vand >t0=q0,<t0=q6,<mask26=q1
vand q0,q6,q1

# qhasm:   h15 = h15[0] + c[0],h15[1] 
# asm 1: vadd.i64 <h15=reg128#6%bot,<h15=reg128#6%bot,<c=reg128#12%bot
# asm 2: vadd.i64 <h15=d10,<h15=d10,<c=d22
vadd.i64 d10,d10,d22

# qhasm:   h48[0,1,2,3] h59[0,1,2,3] = h48[0]h59[0]h48[1]h59[1] h48[2]h59[2]h48[3]h59[3] 
# asm 1: vzip.i32 <h48=reg128#4,<h59=reg128#10
# asm 2: vzip.i32 <h48=q3,<h59=q9
vzip.i32 q3,q9

# qhasm:   h04 = h04[0] - t0[0],h04[1] 
# asm 1: vsub.i64 <h04=reg128#3%bot,<h04=reg128#3%bot,<t0=reg128#1%bot
# asm 2: vsub.i64 <h04=d4,<h04=d4,<t0=d0
vsub.i64 d4,d4,d0

# qhasm:   posh -= 8 
# asm 1: sub >posh=int32#7,<posh=int32#7,#8
# asm 2: sub >posh=r6,<posh=r6,#8
sub r6,r6,#8

# qhasm:   mem64[posh] aligned= h48[0] 
# asm 1: vst1.8 <h48=reg128#4%bot,[<posh=int32#7,: 64]
# asm 2: vst1.8 <h48=d6,[<posh=r6,: 64]
vst1.8 d6,[r6,: 64]

# qhasm:   posh += 16 
# asm 1: add >posh=int32#7,<posh=int32#7,#16
# asm 2: add >posh=r6,<posh=r6,#16
add r6,r6,#16

# qhasm:   mem64[posh] aligned= h59[0] 
# asm 1: vst1.8 <h59=reg128#10%bot,[<posh=int32#7,: 64]
# asm 2: vst1.8 <h59=d18,[<posh=r6,: 64]
vst1.8 d18,[r6,: 64]

# qhasm:   h04[0,1,2,3] h15[0,1,2,3] = h04[0]h15[0]h04[1]h15[1] h04[2]h15[2]h04[3]h15[3] 
# asm 1: vzip.i32 <h04=reg128#3,<h15=reg128#6
# asm 2: vzip.i32 <h04=q2,<h15=q5
vzip.i32 q2,q5

# qhasm:   posh -= 32 
# asm 1: sub >posh=int32#7,<posh=int32#7,#32
# asm 2: sub >posh=r6,<posh=r6,#32
sub r6,r6,#32

# qhasm:   mem64[posh] aligned= h04[0] 
# asm 1: vst1.8 <h04=reg128#3%bot,[<posh=int32#7,: 64]
# asm 2: vst1.8 <h04=d4,[<posh=r6,: 64]
vst1.8 d4,[r6,: 64]

# qhasm:                        unsigned>? j -= 1
# asm 1: subs >j=int32#6,<j=int32#6,#1
# asm 2: subs >j=r5,<j=r5,#1
subs r5,r5,#1

# qhasm:   goto squaringloop if unsigned>
bhi ._squaringloop

# qhasm:   skipsquaringloop:
._skipsquaringloop:

# qhasm:   posf = mulsource 
# asm 1: mov >posf=int32#3,<mulsource=int32#3
# asm 2: mov >posf=r2,<mulsource=r2
mov r2,r2

# qhasm:   posg = playground1_ptr + 288 
# asm 1: add >posg=int32#6,<playground1_ptr=int32#4,#288
# asm 2: add >posg=r5,<playground1_ptr=r3,#288
add r5,r3,#288

# qhasm:   posh = playground1_ptr + 144 
# asm 1: add >posh=int32#7,<playground1_ptr=int32#4,#144
# asm 2: add >posh=r6,<playground1_ptr=r3,#144
add r6,r3,#144

# qhasm:   4x _19_19_19_19 = 19 
# asm 1: vmov.i32 >_19_19_19_19=reg128#1,#19
# asm 2: vmov.i32 >_19_19_19_19=q0,#19
vmov.i32 q0,#19

# qhasm:   4x _0_1_0_1 = 0 
# asm 1: vmov.i32 >_0_1_0_1=reg128#2,#0
# asm 2: vmov.i32 >_0_1_0_1=q1,#0
vmov.i32 q1,#0

# qhasm:   4x _1_1_1_1 = 1 
# asm 1: vmov.i32 >_1_1_1_1=reg128#3,#1
# asm 2: vmov.i32 >_1_1_1_1=q2,#1
vmov.i32 q2,#1

# qhasm:   _0_1_0_1[0,1,2,3] _1_1_1_1[0,1,2,3] = _0_1_0_1[0]_1_1_1_1[0]_0_1_0_1[1]_1_1_1_1[1] _0_1_0_1[2]_1_1_1_1[2]_0_1_0_1[3]_1_1_1_1[3] 
# asm 1: vzip.i32 <_0_1_0_1=reg128#2,<_1_1_1_1=reg128#3
# asm 2: vzip.i32 <_0_1_0_1=q1,<_1_1_1_1=q2
vzip.i32 q1,q2

# qhasm:   g0_g1_g2_g3 aligned= mem128[posg];posg+=16 
# asm 1: vld1.8 {>g0_g1_g2_g3=reg128#3%bot->g0_g1_g2_g3=reg128#3%top},[<posg=int32#6,: 128]!
# asm 2: vld1.8 {>g0_g1_g2_g3=d4->g0_g1_g2_g3=d5},[<posg=r5,: 128]!
vld1.8 {d4-d5},[r5,: 128]!

# qhasm:   g4_g5_g6_g7 aligned= mem128[posg];posg+=16 
# asm 1: vld1.8 {>g4_g5_g6_g7=reg128#4%bot->g4_g5_g6_g7=reg128#4%top},[<posg=int32#6,: 128]!
# asm 2: vld1.8 {>g4_g5_g6_g7=d6->g4_g5_g6_g7=d7},[<posg=r5,: 128]!
vld1.8 {d6-d7},[r5,: 128]!

# qhasm:   new f8_f9_g8_g9 

# qhasm:   f8_f9_g8_g9 aligned= f8_f9_g8_g9[0]mem64[posg] 
# asm 1: vld1.8 {<f8_f9_g8_g9=reg128#5%top},[<posg=int32#6,: 64]
# asm 2: vld1.8 {<f8_f9_g8_g9=d9},[<posg=r5,: 64]
vld1.8 {d9},[r5,: 64]

# qhasm:   f0_f1_f2_f3 aligned= mem128[posf];posf+=16 
# asm 1: vld1.8 {>f0_f1_f2_f3=reg128#6%bot->f0_f1_f2_f3=reg128#6%top},[<posf=int32#3,: 128]!
# asm 2: vld1.8 {>f0_f1_f2_f3=d10->f0_f1_f2_f3=d11},[<posf=r2,: 128]!
vld1.8 {d10-d11},[r2,: 128]!

# qhasm:   playp = &playground2 
# asm 1: lea >playp=int32#6,<playground2=stack512#1
# asm 2: lea >playp=r5,<playground2=[sp,#416]
add r5,sp,#416

# qhasm:   f4_f5_f6_f7 aligned= mem128[posf];posf+=16 
# asm 1: vld1.8 {>f4_f5_f6_f7=reg128#7%bot->f4_f5_f6_f7=reg128#7%top},[<posf=int32#3,: 128]!
# asm 2: vld1.8 {>f4_f5_f6_f7=d12->f4_f5_f6_f7=d13},[<posf=r2,: 128]!
vld1.8 {d12-d13},[r2,: 128]!

# qhasm:   4x 19g0_19g1_19g2_19g3 = g0_g1_g2_g3 * _19_19_19_19 
# asm 1: vmul.i32 >19g0_19g1_19g2_19g3=reg128#8,<g0_g1_g2_g3=reg128#3,<_19_19_19_19=reg128#1
# asm 2: vmul.i32 >19g0_19g1_19g2_19g3=q7,<g0_g1_g2_g3=q2,<_19_19_19_19=q0
vmul.i32 q7,q2,q0

# qhasm:   f8_f9_g8_g9 aligned= mem64[posf]f8_f9_g8_g9[1] 
# asm 1: vld1.8 {<f8_f9_g8_g9=reg128#5%bot},[<posf=int32#3,: 64]
# asm 2: vld1.8 {<f8_f9_g8_g9=d8},[<posf=r2,: 64]
vld1.8 {d8},[r2,: 64]

# qhasm:   new f1_f8_f3_f0 

# qhasm:   f1_f8_f3_f0 = f1_f8_f3_f0[0,1]f0_f1_f2_f3[3]f0_f1_f2_f3[0] 
# asm 1: vext.32 <f1_f8_f3_f0=reg128#9%top,<f0_f1_f2_f3=reg128#6%top,<f0_f1_f2_f3=reg128#6%bot,#1
# asm 2: vext.32 <f1_f8_f3_f0=d17,<f0_f1_f2_f3=d11,<f0_f1_f2_f3=d10,#1
vext.32 d17,d11,d10,#1

# qhasm:   4x 19g4_19g5_19g6_19g7 = g4_g5_g6_g7 * _19_19_19_19 
# asm 1: vmul.i32 >19g4_19g5_19g6_19g7=reg128#10,<g4_g5_g6_g7=reg128#4,<_19_19_19_19=reg128#1
# asm 2: vmul.i32 >19g4_19g5_19g6_19g7=q9,<g4_g5_g6_g7=q3,<_19_19_19_19=q0
vmul.i32 q9,q3,q0

# qhasm:   f1_f8_f3_f0 = f0_f1_f2_f3[1]f8_f9_g8_g9[0]f1_f8_f3_f0[2,3] 
# asm 1: vext.32 <f1_f8_f3_f0=reg128#9%bot,<f0_f1_f2_f3=reg128#6%bot,<f8_f9_g8_g9=reg128#5%bot,#1
# asm 2: vext.32 <f1_f8_f3_f0=d16,<f0_f1_f2_f3=d10,<f8_f9_g8_g9=d8,#1
vext.32 d16,d10,d8,#1

# qhasm:   4x f0_2f1_f2_2f3 = f0_f1_f2_f3 << _0_1_0_1 
# asm 1: vshl.u32 >f0_2f1_f2_2f3=reg128#11,<f0_f1_f2_f3=reg128#6,<_0_1_0_1=reg128#2
# asm 2: vshl.u32 >f0_2f1_f2_2f3=q10,<f0_f1_f2_f3=q5,<_0_1_0_1=q1
vshl.u32 q10,q5,q1

# qhasm:   new g0_19g1_g2_19g3 

# qhasm:   g0_19g1_g2_19g3 = 19g0_19g1_19g2_19g3[1]g0_g1_g2_g3[0]g0_19g1_g2_19g3[2,3] 
# asm 1: vext.32 <g0_19g1_g2_19g3=reg128#12%bot,<19g0_19g1_19g2_19g3=reg128#8%bot,<g0_g1_g2_g3=reg128#3%bot,#1
# asm 2: vext.32 <g0_19g1_g2_19g3=d22,<19g0_19g1_19g2_19g3=d14,<g0_g1_g2_g3=d4,#1
vext.32 d22,d14,d4,#1

# qhasm:   new g4_19g5_g6_19g7 

# qhasm:   g4_19g5_g6_19g7 = 19g4_19g5_19g6_19g7[1]g4_g5_g6_g7[0]g4_19g5_g6_19g7[2,3] 
# asm 1: vext.32 <g4_19g5_g6_19g7=reg128#13%bot,<19g4_19g5_19g6_19g7=reg128#10%bot,<g4_g5_g6_g7=reg128#4%bot,#1
# asm 2: vext.32 <g4_19g5_g6_19g7=d24,<19g4_19g5_19g6_19g7=d18,<g4_g5_g6_g7=d6,#1
vext.32 d24,d18,d6,#1

# qhasm:   4x f4_2f5_f6_2f7 = f4_f5_f6_f7 << _0_1_0_1 
# asm 1: vshl.u32 >f4_2f5_f6_2f7=reg128#14,<f4_f5_f6_f7=reg128#7,<_0_1_0_1=reg128#2
# asm 2: vshl.u32 >f4_2f5_f6_2f7=q13,<f4_f5_f6_f7=q6,<_0_1_0_1=q1
vshl.u32 q13,q6,q1

# qhasm:   new f8_2f9_f9_f6 

# qhasm:   f8_2f9_f9_f6 = f8_f9_g8_g9[0] << _0_1_0_1[0],f8_f9_g8_g9[1] << _0_1_0_1[1],f8_2f9_f9_f6[2,3] 
# asm 1: vshl.u32 <f8_2f9_f9_f6=reg128#15%bot,<f8_f9_g8_g9=reg128#5%bot,<_0_1_0_1=reg128#2%bot
# asm 2: vshl.u32 <f8_2f9_f9_f6=d28,<f8_f9_g8_g9=d8,<_0_1_0_1=d2
vshl.u32 d28,d8,d2

# qhasm:   g0_19g1_g2_19g3 = g0_19g1_g2_19g3[1]g0_19g1_g2_19g3[0]g0_19g1_g2_19g3[2,3] 
# asm 1: vrev64.i32 <g0_19g1_g2_19g3=reg128#12%bot,<g0_19g1_g2_19g3=reg128#12%bot
# asm 2: vrev64.i32 <g0_19g1_g2_19g3=d22,<g0_19g1_g2_19g3=d22
vrev64.i32 d22,d22

# qhasm:   g8_19g9_19g8_19g9[0,1] = g8_19g9_19g8_19g9[0,1];g8_19g9_19g8_19g9[2] = f8_f9_g8_g9[2] * _19_19_19_19[2];g8_19g9_19g8_19g9[3] = f8_f9_g8_g9[3] * _19_19_19_19[3] 
# asm 1: vmul.i32 >g8_19g9_19g8_19g9=reg128#1%top,<f8_f9_g8_g9=reg128#5%top,<_19_19_19_19=reg128#1%top
# asm 2: vmul.i32 >g8_19g9_19g8_19g9=d1,<f8_f9_g8_g9=d9,<_19_19_19_19=d1
vmul.i32 d1,d9,d1

# qhasm:   g4_19g5_g6_19g7 = g4_19g5_g6_19g7[1]g4_19g5_g6_19g7[0]g4_19g5_g6_19g7[2,3] 
# asm 1: vrev64.i32 <g4_19g5_g6_19g7=reg128#13%bot,<g4_19g5_g6_19g7=reg128#13%bot
# asm 2: vrev64.i32 <g4_19g5_g6_19g7=d24,<g4_19g5_g6_19g7=d24
vrev64.i32 d24,d24

# qhasm:   f8_2f9_f9_f6 = f8_2f9_f9_f6[0,1]f8_f9_g8_g9[1]f4_f5_f6_f7[2] 
# asm 1: vext.32 <f8_2f9_f9_f6=reg128#15%top,<f8_f9_g8_g9=reg128#5%bot,<f4_f5_f6_f7=reg128#7%top,#1
# asm 2: vext.32 <f8_2f9_f9_f6=d29,<f8_f9_g8_g9=d8,<f4_f5_f6_f7=d13,#1
vext.32 d29,d8,d13,#1

# qhasm:   g8_19g9_19g8_19g9 = g8_19g9_19g8_19g9[3]f8_f9_g8_g9[2]g8_19g9_19g8_19g9[2,3] 
# asm 1: vext.32 <g8_19g9_19g8_19g9=reg128#1%bot,<g8_19g9_19g8_19g9=reg128#1%top,<f8_f9_g8_g9=reg128#5%top,#1
# asm 2: vext.32 <g8_19g9_19g8_19g9=d0,<g8_19g9_19g8_19g9=d1,<f8_f9_g8_g9=d9,#1
vext.32 d0,d1,d9,#1

# qhasm:   g8_19g9_19g8_19g9 = g8_19g9_19g8_19g9[1]g8_19g9_19g8_19g9[0]g8_19g9_19g8_19g9[2,3] 
# asm 1: vrev64.i32 <g8_19g9_19g8_19g9=reg128#1%bot,<g8_19g9_19g8_19g9=reg128#1%bot
# asm 2: vrev64.i32 <g8_19g9_19g8_19g9=d0,<g8_19g9_19g8_19g9=d0
vrev64.i32 d0,d0

# qhasm:   new 19g8_g9_19g2_g3 

# qhasm:   19g8_g9_19g2_g3 = f8_f9_g8_g9[3]g8_19g9_19g8_19g9[2]19g8_g9_19g2_g3[2,3] 
# asm 1: vext.32 <19g8_g9_19g2_g3=reg128#2%bot,<f8_f9_g8_g9=reg128#5%top,<g8_19g9_19g8_19g9=reg128#1%top,#1
# asm 2: vext.32 <19g8_g9_19g2_g3=d2,<f8_f9_g8_g9=d9,<g8_19g9_19g8_19g9=d1,#1
vext.32 d2,d9,d1,#1

# qhasm:   g0_19g1_g2_19g3 = g0_19g1_g2_19g3[0,1]19g0_19g1_19g2_19g3[3]g0_g1_g2_g3[2] 
# asm 1: vext.32 <g0_19g1_g2_19g3=reg128#12%top,<19g0_19g1_19g2_19g3=reg128#8%top,<g0_g1_g2_g3=reg128#3%top,#1
# asm 2: vext.32 <g0_19g1_g2_19g3=d23,<19g0_19g1_19g2_19g3=d15,<g0_g1_g2_g3=d5,#1
vext.32 d23,d15,d5,#1

# qhasm:   h02[0,1] = f0_2f1_f2_2f3[0] signed* g0_g1_g2_g3[0];h02[2,3] = f0_2f1_f2_2f3[1] signed* g0_g1_g2_g3[1] 
# asm 1: vmull.s32 >h02=reg128#5,<f0_2f1_f2_2f3=reg128#11%bot,<g0_g1_g2_g3=reg128#3%bot
# asm 2: vmull.s32 >h02=q4,<f0_2f1_f2_2f3=d20,<g0_g1_g2_g3=d4
vmull.s32 q4,d20,d4

# qhasm:   g0_19g1_g2_19g3 = g0_19g1_g2_19g3[0,1]g0_19g1_g2_19g3[3]g0_19g1_g2_19g3[2] 
# asm 1: vrev64.i32 <g0_19g1_g2_19g3=reg128#12%top,<g0_19g1_g2_19g3=reg128#12%top
# asm 2: vrev64.i32 <g0_19g1_g2_19g3=d23,<g0_19g1_g2_19g3=d23
vrev64.i32 d23,d23

# qhasm:   h02[0,1] += f0_2f1_f2_2f3[2] signed* g8_19g9_19g8_19g9[2];h02[2,3] += f0_2f1_f2_2f3[3] signed* g8_19g9_19g8_19g9[3] 
# asm 1: vmlal.s32 <h02=reg128#5,<f0_2f1_f2_2f3=reg128#11%top,<g8_19g9_19g8_19g9=reg128#1%top
# asm 2: vmlal.s32 <h02=q4,<f0_2f1_f2_2f3=d21,<g8_19g9_19g8_19g9=d1
vmlal.s32 q4,d21,d1

# qhasm:   19g8_g9_19g2_g3 = 19g8_g9_19g2_g3[1]19g8_g9_19g2_g3[0]19g8_g9_19g2_g3[2,3] 
# asm 1: vrev64.i32 <19g8_g9_19g2_g3=reg128#2%bot,<19g8_g9_19g2_g3=reg128#2%bot
# asm 2: vrev64.i32 <19g8_g9_19g2_g3=d2,<19g8_g9_19g2_g3=d2
vrev64.i32 d2,d2

# qhasm:   h02[0,1] += f4_2f5_f6_2f7[0] signed* 19g4_19g5_19g6_19g7[2];h02[2,3] += f4_2f5_f6_2f7[1] signed* 19g4_19g5_19g6_19g7[3] 
# asm 1: vmlal.s32 <h02=reg128#5,<f4_2f5_f6_2f7=reg128#14%bot,<19g4_19g5_19g6_19g7=reg128#10%top
# asm 2: vmlal.s32 <h02=q4,<f4_2f5_f6_2f7=d26,<19g4_19g5_19g6_19g7=d19
vmlal.s32 q4,d26,d19

# qhasm:   19g8_g9_19g2_g3 = 19g8_g9_19g2_g3[0,1]g0_g1_g2_g3[3]19g0_19g1_19g2_19g3[2] 
# asm 1: vext.32 <19g8_g9_19g2_g3=reg128#2%top,<g0_g1_g2_g3=reg128#3%top,<19g0_19g1_19g2_19g3=reg128#8%top,#1
# asm 2: vext.32 <19g8_g9_19g2_g3=d3,<g0_g1_g2_g3=d5,<19g0_19g1_19g2_19g3=d15,#1
vext.32 d3,d5,d15,#1

# qhasm:   h02[0,1] += f4_2f5_f6_2f7[2] signed* 19g4_19g5_19g6_19g7[0];h02[2,3] += f4_2f5_f6_2f7[3] signed* 19g4_19g5_19g6_19g7[1] 
# asm 1: vmlal.s32 <h02=reg128#5,<f4_2f5_f6_2f7=reg128#14%top,<19g4_19g5_19g6_19g7=reg128#10%bot
# asm 2: vmlal.s32 <h02=q4,<f4_2f5_f6_2f7=d27,<19g4_19g5_19g6_19g7=d18
vmlal.s32 q4,d27,d18

# qhasm:   19g8_g9_19g2_g3 = 19g8_g9_19g2_g3[0,1]19g8_g9_19g2_g3[3]19g8_g9_19g2_g3[2] 
# asm 1: vrev64.i32 <19g8_g9_19g2_g3=reg128#2%top,<19g8_g9_19g2_g3=reg128#2%top
# asm 2: vrev64.i32 <19g8_g9_19g2_g3=d3,<19g8_g9_19g2_g3=d3
vrev64.i32 d3,d3

# qhasm:   h02[0,1] += f8_2f9_f9_f6[0] signed* 19g0_19g1_19g2_19g3[2];h02[2,3] += f8_2f9_f9_f6[1] signed* 19g0_19g1_19g2_19g3[3] 
# asm 1: vmlal.s32 <h02=reg128#5,<f8_2f9_f9_f6=reg128#15%bot,<19g0_19g1_19g2_19g3=reg128#8%top
# asm 2: vmlal.s32 <h02=q4,<f8_2f9_f9_f6=d28,<19g0_19g1_19g2_19g3=d15
vmlal.s32 q4,d28,d15

# qhasm:   new f5_f2_f7_f4 

# qhasm:   f5_f2_f7_f4 = f4_f5_f6_f7[1]f0_f1_f2_f3[2]f5_f2_f7_f4[2,3] 
# asm 1: vext.32 <f5_f2_f7_f4=reg128#8%bot,<f4_f5_f6_f7=reg128#7%bot,<f0_f1_f2_f3=reg128#6%top,#1
# asm 2: vext.32 <f5_f2_f7_f4=d14,<f4_f5_f6_f7=d12,<f0_f1_f2_f3=d11,#1
vext.32 d14,d12,d11,#1

# qhasm:   h31[0,1] = f1_f8_f3_f0[0] signed* g0_19g1_g2_19g3[2];h31[2,3] = f1_f8_f3_f0[1] signed* g0_19g1_g2_19g3[3] 
# asm 1: vmull.s32 >h31=reg128#6,<f1_f8_f3_f0=reg128#9%bot,<g0_19g1_g2_19g3=reg128#12%top
# asm 2: vmull.s32 >h31=q5,<f1_f8_f3_f0=d16,<g0_19g1_g2_19g3=d23
vmull.s32 q5,d16,d23

# qhasm:   f5_f2_f7_f4 = f5_f2_f7_f4[0,1]f4_f5_f6_f7[3]f4_f5_f6_f7[0] 
# asm 1: vext.32 <f5_f2_f7_f4=reg128#8%top,<f4_f5_f6_f7=reg128#7%top,<f4_f5_f6_f7=reg128#7%bot,#1
# asm 2: vext.32 <f5_f2_f7_f4=d15,<f4_f5_f6_f7=d13,<f4_f5_f6_f7=d12,#1
vext.32 d15,d13,d12,#1

# qhasm:   h31[0,1] += f1_f8_f3_f0[2] signed* g0_g1_g2_g3[0];h31[2,3] += f1_f8_f3_f0[3] signed* g0_g1_g2_g3[1] 
# asm 1: vmlal.s32 <h31=reg128#6,<f1_f8_f3_f0=reg128#9%top,<g0_g1_g2_g3=reg128#3%bot
# asm 2: vmlal.s32 <h31=q5,<f1_f8_f3_f0=d17,<g0_g1_g2_g3=d4
vmlal.s32 q5,d17,d4

# qhasm:   mem64[playp] aligned= h02[0];playp+=8 
# asm 1: vst1.8 <h02=reg128#5%bot,[<playp=int32#6,: 64]!
# asm 2: vst1.8 <h02=d8,[<playp=r5,: 64]!
vst1.8 d8,[r5,: 64]!

# qhasm:   h31[0,1] += f5_f2_f7_f4[0] signed* g8_19g9_19g8_19g9[2];h31[2,3] += f5_f2_f7_f4[1] signed* g8_19g9_19g8_19g9[3] 
# asm 1: vmlal.s32 <h31=reg128#6,<f5_f2_f7_f4=reg128#8%bot,<g8_19g9_19g8_19g9=reg128#1%top
# asm 2: vmlal.s32 <h31=q5,<f5_f2_f7_f4=d14,<g8_19g9_19g8_19g9=d1
vmlal.s32 q5,d14,d1

# qhasm:   new h24 

# qhasm:   h24 = h02[2,3]h24[2,3] 
# asm 1: vext.32 <h24=reg128#7%bot,<h02=reg128#5%top,<h02=reg128#5%bot,#0
# asm 2: vext.32 <h24=d12,<h02=d9,<h02=d8,#0
vext.32 d12,d9,d8,#0

# qhasm:   h31[0,1] += f5_f2_f7_f4[2] signed* 19g4_19g5_19g6_19g7[2];h31[2,3] += f5_f2_f7_f4[3] signed* 19g4_19g5_19g6_19g7[3] 
# asm 1: vmlal.s32 <h31=reg128#6,<f5_f2_f7_f4=reg128#8%top,<19g4_19g5_19g6_19g7=reg128#10%top
# asm 2: vmlal.s32 <h31=q5,<f5_f2_f7_f4=d15,<19g4_19g5_19g6_19g7=d19
vmlal.s32 q5,d15,d19

# qhasm:   h24 = h24[0],0 
# asm 1: vmov.i64 <h24=reg128#7%top,#0
# asm 2: vmov.i64 <h24=d13,#0
vmov.i64 d13,#0

# qhasm:   h31[0,1] += f8_2f9_f9_f6[2] signed* 19g4_19g5_19g6_19g7[0];h31[2,3] += f8_2f9_f9_f6[3] signed* 19g4_19g5_19g6_19g7[1] 
# asm 1: vmlal.s32 <h31=reg128#6,<f8_2f9_f9_f6=reg128#15%top,<19g4_19g5_19g6_19g7=reg128#10%bot
# asm 2: vmlal.s32 <h31=q5,<f8_2f9_f9_f6=d29,<19g4_19g5_19g6_19g7=d18
vmlal.s32 q5,d29,d18

# qhasm:   g4_19g5_g6_19g7 = g4_19g5_g6_19g7[0,1]19g4_19g5_19g6_19g7[3]g4_g5_g6_g7[2] 
# asm 1: vext.32 <g4_19g5_g6_19g7=reg128#13%top,<19g4_19g5_19g6_19g7=reg128#10%top,<g4_g5_g6_g7=reg128#4%top,#1
# asm 2: vext.32 <g4_19g5_g6_19g7=d25,<19g4_19g5_19g6_19g7=d19,<g4_g5_g6_g7=d7,#1
vext.32 d25,d19,d7,#1

# qhasm:   h24[0,1] += f0_2f1_f2_2f3[0] signed* g0_g1_g2_g3[2];h24[2,3] += f0_2f1_f2_2f3[1] signed* g0_g1_g2_g3[3] 
# asm 1: vmlal.s32 <h24=reg128#7,<f0_2f1_f2_2f3=reg128#11%bot,<g0_g1_g2_g3=reg128#3%top
# asm 2: vmlal.s32 <h24=q6,<f0_2f1_f2_2f3=d20,<g0_g1_g2_g3=d5
vmlal.s32 q6,d20,d5

# qhasm:   g4_19g5_g6_19g7 = g4_19g5_g6_19g7[0,1]g4_19g5_g6_19g7[3]g4_19g5_g6_19g7[2] 
# asm 1: vrev64.i32 <g4_19g5_g6_19g7=reg128#13%top,<g4_19g5_g6_19g7=reg128#13%top
# asm 2: vrev64.i32 <g4_19g5_g6_19g7=d25,<g4_19g5_g6_19g7=d25
vrev64.i32 d25,d25

# qhasm:   h24[0,1] += f0_2f1_f2_2f3[2] signed* g0_g1_g2_g3[0];h24[2,3] += f0_2f1_f2_2f3[3] signed* g0_g1_g2_g3[1] 
# asm 1: vmlal.s32 <h24=reg128#7,<f0_2f1_f2_2f3=reg128#11%top,<g0_g1_g2_g3=reg128#3%bot
# asm 2: vmlal.s32 <h24=q6,<f0_2f1_f2_2f3=d21,<g0_g1_g2_g3=d4
vmlal.s32 q6,d21,d4

# qhasm:   mem64[playp] aligned= h31[1];playp+=8 
# asm 1: vst1.8 <h31=reg128#6%top,[<playp=int32#6,: 64]!
# asm 2: vst1.8 <h31=d11,[<playp=r5,: 64]!
vst1.8 d11,[r5,: 64]!

# qhasm:   h24[0,1] += f4_2f5_f6_2f7[0] signed* g8_19g9_19g8_19g9[2];h24[2,3] += f4_2f5_f6_2f7[1] signed* g8_19g9_19g8_19g9[3] 
# asm 1: vmlal.s32 <h24=reg128#7,<f4_2f5_f6_2f7=reg128#14%bot,<g8_19g9_19g8_19g9=reg128#1%top
# asm 2: vmlal.s32 <h24=q6,<f4_2f5_f6_2f7=d26,<g8_19g9_19g8_19g9=d1
vmlal.s32 q6,d26,d1

# qhasm:   new h53 

# qhasm:   h53 = h53[0,1]h31[0,1] 
# asm 1: vext.32 <h53=reg128#5%top,<h31=reg128#6%bot,<h31=reg128#6%bot,#0
# asm 2: vext.32 <h53=d9,<h31=d10,<h31=d10,#0
vext.32 d9,d10,d10,#0

# qhasm:   h24[0,1] += f4_2f5_f6_2f7[2] signed* 19g4_19g5_19g6_19g7[2];h24[2,3] += f4_2f5_f6_2f7[3] signed* 19g4_19g5_19g6_19g7[3] 
# asm 1: vmlal.s32 <h24=reg128#7,<f4_2f5_f6_2f7=reg128#14%top,<19g4_19g5_19g6_19g7=reg128#10%top
# asm 2: vmlal.s32 <h24=q6,<f4_2f5_f6_2f7=d27,<19g4_19g5_19g6_19g7=d19
vmlal.s32 q6,d27,d19

# qhasm:   h53 = 0,h53[1] 
# asm 1: vmov.i64 <h53=reg128#5%bot,#0
# asm 2: vmov.i64 <h53=d8,#0
vmov.i64 d8,#0

# qhasm:   h24[0,1] += f8_2f9_f9_f6[0] signed* 19g4_19g5_19g6_19g7[0];h24[2,3] += f8_2f9_f9_f6[1] signed* 19g4_19g5_19g6_19g7[1] 
# asm 1: vmlal.s32 <h24=reg128#7,<f8_2f9_f9_f6=reg128#15%bot,<19g4_19g5_19g6_19g7=reg128#10%bot
# asm 2: vmlal.s32 <h24=q6,<f8_2f9_f9_f6=d28,<19g4_19g5_19g6_19g7=d18
vmlal.s32 q6,d28,d18

# qhasm:   h53[0,1] += f1_f8_f3_f0[0] signed* g4_19g5_g6_19g7[0];h53[2,3] += f1_f8_f3_f0[1] signed* g4_19g5_g6_19g7[1] 
# asm 1: vmlal.s32 <h53=reg128#5,<f1_f8_f3_f0=reg128#9%bot,<g4_19g5_g6_19g7=reg128#13%bot
# asm 2: vmlal.s32 <h53=q4,<f1_f8_f3_f0=d16,<g4_19g5_g6_19g7=d24
vmlal.s32 q4,d16,d24

# qhasm:   h53[0,1] += f1_f8_f3_f0[2] signed* g0_g1_g2_g3[2];h53[2,3] += f1_f8_f3_f0[3] signed* g0_g1_g2_g3[3] 
# asm 1: vmlal.s32 <h53=reg128#5,<f1_f8_f3_f0=reg128#9%top,<g0_g1_g2_g3=reg128#3%top
# asm 2: vmlal.s32 <h53=q4,<f1_f8_f3_f0=d17,<g0_g1_g2_g3=d5
vmlal.s32 q4,d17,d5

# qhasm:   h53[0,1] += f5_f2_f7_f4[0] signed* g0_g1_g2_g3[0];h53[2,3] += f5_f2_f7_f4[1] signed* g0_g1_g2_g3[1] 
# asm 1: vmlal.s32 <h53=reg128#5,<f5_f2_f7_f4=reg128#8%bot,<g0_g1_g2_g3=reg128#3%bot
# asm 2: vmlal.s32 <h53=q4,<f5_f2_f7_f4=d14,<g0_g1_g2_g3=d4
vmlal.s32 q4,d14,d4

# qhasm:   mem64[playp] aligned= h24[0];playp+=8 
# asm 1: vst1.8 <h24=reg128#7%bot,[<playp=int32#6,: 64]!
# asm 2: vst1.8 <h24=d12,[<playp=r5,: 64]!
vst1.8 d12,[r5,: 64]!

# qhasm:   h53[0,1] += f5_f2_f7_f4[2] signed* g8_19g9_19g8_19g9[2];h53[2,3] += f5_f2_f7_f4[3] signed* g8_19g9_19g8_19g9[3] 
# asm 1: vmlal.s32 <h53=reg128#5,<f5_f2_f7_f4=reg128#8%top,<g8_19g9_19g8_19g9=reg128#1%top
# asm 2: vmlal.s32 <h53=q4,<f5_f2_f7_f4=d15,<g8_19g9_19g8_19g9=d1
vmlal.s32 q4,d15,d1

# qhasm:   new h46 

# qhasm:   h46 = h24[2,3]h46[2,3] 
# asm 1: vext.32 <h46=reg128#6%bot,<h24=reg128#7%top,<h24=reg128#7%bot,#0
# asm 2: vext.32 <h46=d10,<h24=d13,<h24=d12,#0
vext.32 d10,d13,d12,#0

# qhasm:   h53[0,1] += f8_2f9_f9_f6[2] signed* 19g4_19g5_19g6_19g7[2];h53[2,3] += f8_2f9_f9_f6[3] signed* 19g4_19g5_19g6_19g7[3] 
# asm 1: vmlal.s32 <h53=reg128#5,<f8_2f9_f9_f6=reg128#15%top,<19g4_19g5_19g6_19g7=reg128#10%top
# asm 2: vmlal.s32 <h53=q4,<f8_2f9_f9_f6=d29,<19g4_19g5_19g6_19g7=d19
vmlal.s32 q4,d29,d19

# qhasm:   h46 = h46[0],0 
# asm 1: vmov.i64 <h46=reg128#6%top,#0
# asm 2: vmov.i64 <h46=d11,#0
vmov.i64 d11,#0

# qhasm:   h46[0,1] += f0_2f1_f2_2f3[0] signed* g4_g5_g6_g7[0];h46[2,3] += f0_2f1_f2_2f3[1] signed* g4_g5_g6_g7[1] 
# asm 1: vmlal.s32 <h46=reg128#6,<f0_2f1_f2_2f3=reg128#11%bot,<g4_g5_g6_g7=reg128#4%bot
# asm 2: vmlal.s32 <h46=q5,<f0_2f1_f2_2f3=d20,<g4_g5_g6_g7=d6
vmlal.s32 q5,d20,d6

# qhasm:   h46[0,1] += f0_2f1_f2_2f3[2] signed* g0_g1_g2_g3[2];h46[2,3] += f0_2f1_f2_2f3[3] signed* g0_g1_g2_g3[3] 
# asm 1: vmlal.s32 <h46=reg128#6,<f0_2f1_f2_2f3=reg128#11%top,<g0_g1_g2_g3=reg128#3%top
# asm 2: vmlal.s32 <h46=q5,<f0_2f1_f2_2f3=d21,<g0_g1_g2_g3=d5
vmlal.s32 q5,d21,d5

# qhasm:   h46[0,1] += f4_2f5_f6_2f7[0] signed* g0_g1_g2_g3[0];h46[2,3] += f4_2f5_f6_2f7[1] signed* g0_g1_g2_g3[1] 
# asm 1: vmlal.s32 <h46=reg128#6,<f4_2f5_f6_2f7=reg128#14%bot,<g0_g1_g2_g3=reg128#3%bot
# asm 2: vmlal.s32 <h46=q5,<f4_2f5_f6_2f7=d26,<g0_g1_g2_g3=d4
vmlal.s32 q5,d26,d4

# qhasm:   new h75 

# qhasm:   h75 = h75[0,1]h53[0,1] 
# asm 1: vext.32 <h75=reg128#7%top,<h53=reg128#5%bot,<h53=reg128#5%bot,#0
# asm 2: vext.32 <h75=d13,<h53=d8,<h53=d8,#0
vext.32 d13,d8,d8,#0

# qhasm:   h46[0,1] += f4_2f5_f6_2f7[2] signed* g8_19g9_19g8_19g9[2];h46[2,3] += f4_2f5_f6_2f7[3] signed* g8_19g9_19g8_19g9[3] 
# asm 1: vmlal.s32 <h46=reg128#6,<f4_2f5_f6_2f7=reg128#14%top,<g8_19g9_19g8_19g9=reg128#1%top
# asm 2: vmlal.s32 <h46=q5,<f4_2f5_f6_2f7=d27,<g8_19g9_19g8_19g9=d1
vmlal.s32 q5,d27,d1

# qhasm:   h75 = 0,h75[1] 
# asm 1: vmov.i64 <h75=reg128#7%bot,#0
# asm 2: vmov.i64 <h75=d12,#0
vmov.i64 d12,#0

# qhasm:   h46[0,1] += f8_2f9_f9_f6[0] signed* 19g4_19g5_19g6_19g7[2];h46[2,3] += f8_2f9_f9_f6[1] signed* 19g4_19g5_19g6_19g7[3] 
# asm 1: vmlal.s32 <h46=reg128#6,<f8_2f9_f9_f6=reg128#15%bot,<19g4_19g5_19g6_19g7=reg128#10%top
# asm 2: vmlal.s32 <h46=q5,<f8_2f9_f9_f6=d28,<19g4_19g5_19g6_19g7=d19
vmlal.s32 q5,d28,d19

# qhasm:   mem64[playp] aligned= h53[1];playp+=8 
# asm 1: vst1.8 <h53=reg128#5%top,[<playp=int32#6,: 64]!
# asm 2: vst1.8 <h53=d9,[<playp=r5,: 64]!
vst1.8 d9,[r5,: 64]!

# qhasm:   h75[0,1] += f1_f8_f3_f0[0] signed* g4_19g5_g6_19g7[2];h75[2,3] += f1_f8_f3_f0[1] signed* g4_19g5_g6_19g7[3] 
# asm 1: vmlal.s32 <h75=reg128#7,<f1_f8_f3_f0=reg128#9%bot,<g4_19g5_g6_19g7=reg128#13%top
# asm 2: vmlal.s32 <h75=q6,<f1_f8_f3_f0=d16,<g4_19g5_g6_19g7=d25
vmlal.s32 q6,d16,d25

# qhasm:   h75[0,1] += f1_f8_f3_f0[2] signed* g4_g5_g6_g7[0];h75[2,3] += f1_f8_f3_f0[3] signed* g4_g5_g6_g7[1] 
# asm 1: vmlal.s32 <h75=reg128#7,<f1_f8_f3_f0=reg128#9%top,<g4_g5_g6_g7=reg128#4%bot
# asm 2: vmlal.s32 <h75=q6,<f1_f8_f3_f0=d17,<g4_g5_g6_g7=d6
vmlal.s32 q6,d17,d6

# qhasm:   mem64[playp] aligned= h46[0] 
# asm 1: vst1.8 <h46=reg128#6%bot,[<playp=int32#6,: 64]
# asm 2: vst1.8 <h46=d10,[<playp=r5,: 64]
vst1.8 d10,[r5,: 64]

# qhasm:   h75[0,1] += f5_f2_f7_f4[0] signed* g0_g1_g2_g3[2];h75[2,3] += f5_f2_f7_f4[1] signed* g0_g1_g2_g3[3] 
# asm 1: vmlal.s32 <h75=reg128#7,<f5_f2_f7_f4=reg128#8%bot,<g0_g1_g2_g3=reg128#3%top
# asm 2: vmlal.s32 <h75=q6,<f5_f2_f7_f4=d14,<g0_g1_g2_g3=d5
vmlal.s32 q6,d14,d5

# qhasm:   new h68 

# qhasm:   h68 = h46[2,3]h68[2,3] 
# asm 1: vext.32 <h68=reg128#5%bot,<h46=reg128#6%top,<h46=reg128#6%bot,#0
# asm 2: vext.32 <h68=d8,<h46=d11,<h46=d10,#0
vext.32 d8,d11,d10,#0

# qhasm:   h75[0,1] += f5_f2_f7_f4[2] signed* g0_g1_g2_g3[0];h75[2,3] += f5_f2_f7_f4[3] signed* g0_g1_g2_g3[1] 
# asm 1: vmlal.s32 <h75=reg128#7,<f5_f2_f7_f4=reg128#8%top,<g0_g1_g2_g3=reg128#3%bot
# asm 2: vmlal.s32 <h75=q6,<f5_f2_f7_f4=d15,<g0_g1_g2_g3=d4
vmlal.s32 q6,d15,d4

# qhasm:   h68 = h68[0],0 
# asm 1: vmov.i64 <h68=reg128#5%top,#0
# asm 2: vmov.i64 <h68=d9,#0
vmov.i64 d9,#0

# qhasm:   h75[0,1] += f8_2f9_f9_f6[2] signed* g8_19g9_19g8_19g9[2];h75[2,3] += f8_2f9_f9_f6[3] signed* g8_19g9_19g8_19g9[3] 
# asm 1: vmlal.s32 <h75=reg128#7,<f8_2f9_f9_f6=reg128#15%top,<g8_19g9_19g8_19g9=reg128#1%top
# asm 2: vmlal.s32 <h75=q6,<f8_2f9_f9_f6=d29,<g8_19g9_19g8_19g9=d1
vmlal.s32 q6,d29,d1

# qhasm:   h68[0,1] += f0_2f1_f2_2f3[0] signed* g4_g5_g6_g7[2];h68[2,3] += f0_2f1_f2_2f3[1] signed* g4_g5_g6_g7[3] 
# asm 1: vmlal.s32 <h68=reg128#5,<f0_2f1_f2_2f3=reg128#11%bot,<g4_g5_g6_g7=reg128#4%top
# asm 2: vmlal.s32 <h68=q4,<f0_2f1_f2_2f3=d20,<g4_g5_g6_g7=d7
vmlal.s32 q4,d20,d7

# qhasm:   h68[0,1] += f0_2f1_f2_2f3[2] signed* g4_g5_g6_g7[0];h68[2,3] += f0_2f1_f2_2f3[3] signed* g4_g5_g6_g7[1] 
# asm 1: vmlal.s32 <h68=reg128#5,<f0_2f1_f2_2f3=reg128#11%top,<g4_g5_g6_g7=reg128#4%bot
# asm 2: vmlal.s32 <h68=q4,<f0_2f1_f2_2f3=d21,<g4_g5_g6_g7=d6
vmlal.s32 q4,d21,d6

# qhasm:   h68[0,1] += f4_2f5_f6_2f7[0] signed* g0_g1_g2_g3[2];h68[2,3] += f4_2f5_f6_2f7[1] signed* g0_g1_g2_g3[3] 
# asm 1: vmlal.s32 <h68=reg128#5,<f4_2f5_f6_2f7=reg128#14%bot,<g0_g1_g2_g3=reg128#3%top
# asm 2: vmlal.s32 <h68=q4,<f4_2f5_f6_2f7=d26,<g0_g1_g2_g3=d5
vmlal.s32 q4,d26,d5

# qhasm:   new h97 

# qhasm:   h97 = h97[0,1]h75[0,1] 
# asm 1: vext.32 <h97=reg128#6%top,<h75=reg128#7%bot,<h75=reg128#7%bot,#0
# asm 2: vext.32 <h97=d11,<h75=d12,<h75=d12,#0
vext.32 d11,d12,d12,#0

# qhasm:   h68[0,1] += f4_2f5_f6_2f7[2] signed* g0_g1_g2_g3[0];h68[2,3] += f4_2f5_f6_2f7[3] signed* g0_g1_g2_g3[1] 
# asm 1: vmlal.s32 <h68=reg128#5,<f4_2f5_f6_2f7=reg128#14%top,<g0_g1_g2_g3=reg128#3%bot
# asm 2: vmlal.s32 <h68=q4,<f4_2f5_f6_2f7=d27,<g0_g1_g2_g3=d4
vmlal.s32 q4,d27,d4

# qhasm:   h97 = 0,h97[1] 
# asm 1: vmov.i64 <h97=reg128#6%bot,#0
# asm 2: vmov.i64 <h97=d10,#0
vmov.i64 d10,#0

# qhasm:   h68[0,1] += f8_2f9_f9_f6[0] signed* g8_19g9_19g8_19g9[2];h68[2,3] += f8_2f9_f9_f6[1] signed* g8_19g9_19g8_19g9[3] 
# asm 1: vmlal.s32 <h68=reg128#5,<f8_2f9_f9_f6=reg128#15%bot,<g8_19g9_19g8_19g9=reg128#1%top
# asm 2: vmlal.s32 <h68=q4,<f8_2f9_f9_f6=d28,<g8_19g9_19g8_19g9=d1
vmlal.s32 q4,d28,d1

# qhasm:   h97[0,1] += f1_f8_f3_f0[0] signed* g8_19g9_19g8_19g9[0];h97[2,3] += f1_f8_f3_f0[1] signed* g8_19g9_19g8_19g9[1] 
# asm 1: vmlal.s32 <h97=reg128#6,<f1_f8_f3_f0=reg128#9%bot,<g8_19g9_19g8_19g9=reg128#1%bot
# asm 2: vmlal.s32 <h97=q5,<f1_f8_f3_f0=d16,<g8_19g9_19g8_19g9=d0
vmlal.s32 q5,d16,d0

# qhasm:   playp -= 32 
# asm 1: sub >playp=int32#3,<playp=int32#6,#32
# asm 2: sub >playp=r2,<playp=r5,#32
sub r2,r5,#32

# qhasm:   h97[0,1] += f1_f8_f3_f0[2] signed* g4_g5_g6_g7[2];h97[2,3] += f1_f8_f3_f0[3] signed* g4_g5_g6_g7[3] 
# asm 1: vmlal.s32 <h97=reg128#6,<f1_f8_f3_f0=reg128#9%top,<g4_g5_g6_g7=reg128#4%top
# asm 2: vmlal.s32 <h97=q5,<f1_f8_f3_f0=d17,<g4_g5_g6_g7=d7
vmlal.s32 q5,d17,d7

# qhasm:   h97[0,1] += f5_f2_f7_f4[0] signed* g4_g5_g6_g7[0];h97[2,3] += f5_f2_f7_f4[1] signed* g4_g5_g6_g7[1] 
# asm 1: vmlal.s32 <h97=reg128#6,<f5_f2_f7_f4=reg128#8%bot,<g4_g5_g6_g7=reg128#4%bot
# asm 2: vmlal.s32 <h97=q5,<f5_f2_f7_f4=d14,<g4_g5_g6_g7=d6
vmlal.s32 q5,d14,d6

# qhasm:   new h80 

# qhasm:   h80 = h68[2,3]h80[2,3] 
# asm 1: vext.32 <h80=reg128#16%bot,<h68=reg128#5%top,<h68=reg128#5%bot,#0
# asm 2: vext.32 <h80=d30,<h68=d9,<h68=d8,#0
vext.32 d30,d9,d8,#0

# qhasm:   h97[0,1] += f5_f2_f7_f4[2] signed* g0_g1_g2_g3[2];h97[2,3] += f5_f2_f7_f4[3] signed* g0_g1_g2_g3[3] 
# asm 1: vmlal.s32 <h97=reg128#6,<f5_f2_f7_f4=reg128#8%top,<g0_g1_g2_g3=reg128#3%top
# asm 2: vmlal.s32 <h97=q5,<f5_f2_f7_f4=d15,<g0_g1_g2_g3=d5
vmlal.s32 q5,d15,d5

# qhasm:   h80 aligned= h80[0]mem64[playp];playp+=8 
# asm 1: vld1.8 {<h80=reg128#16%top},[<playp=int32#3,: 64]!
# asm 2: vld1.8 {<h80=d31},[<playp=r2,: 64]!
vld1.8 {d31},[r2,: 64]!

# qhasm:   h97[0,1] += f8_2f9_f9_f6[2] signed* g0_g1_g2_g3[0];h97[2,3] += f8_2f9_f9_f6[3] signed* g0_g1_g2_g3[1] 
# asm 1: vmlal.s32 <h97=reg128#6,<f8_2f9_f9_f6=reg128#15%top,<g0_g1_g2_g3=reg128#3%bot
# asm 2: vmlal.s32 <h97=q5,<f8_2f9_f9_f6=d29,<g0_g1_g2_g3=d4
vmlal.s32 q5,d29,d4

# qhasm:   h80[0,1] += f0_2f1_f2_2f3[0] signed* g8_19g9_19g8_19g9[0];h80[2,3] += f0_2f1_f2_2f3[1] signed* g8_19g9_19g8_19g9[1] 
# asm 1: vmlal.s32 <h80=reg128#16,<f0_2f1_f2_2f3=reg128#11%bot,<g8_19g9_19g8_19g9=reg128#1%bot
# asm 2: vmlal.s32 <h80=q15,<f0_2f1_f2_2f3=d20,<g8_19g9_19g8_19g9=d0
vmlal.s32 q15,d20,d0

# qhasm:   new 19g4_g5_19g6_g7 

# qhasm:   19g4_g5_19g6_g7 = g4_g5_g6_g7[1]19g4_19g5_19g6_19g7[0]19g4_g5_19g6_g7[2,3] 
# asm 1: vext.32 <19g4_g5_19g6_g7=reg128#1%bot,<g4_g5_g6_g7=reg128#4%bot,<19g4_19g5_19g6_19g7=reg128#10%bot,#1
# asm 2: vext.32 <19g4_g5_19g6_g7=d0,<g4_g5_g6_g7=d6,<19g4_19g5_19g6_19g7=d18,#1
vext.32 d0,d6,d18,#1

# qhasm:   h80[0,1] += f0_2f1_f2_2f3[2] signed* g4_19g5_g6_19g7[2];h80[2,3] += f0_2f1_f2_2f3[3] signed* g4_19g5_g6_19g7[3] 
# asm 1: vmlal.s32 <h80=reg128#16,<f0_2f1_f2_2f3=reg128#11%top,<g4_19g5_g6_19g7=reg128#13%top
# asm 2: vmlal.s32 <h80=q15,<f0_2f1_f2_2f3=d21,<g4_19g5_g6_19g7=d25
vmlal.s32 q15,d21,d25

# qhasm:   19g4_g5_19g6_g7 = 19g4_g5_19g6_g7[1]19g4_g5_19g6_g7[0]19g4_g5_19g6_g7[2,3] 
# asm 1: vrev64.i32 <19g4_g5_19g6_g7=reg128#1%bot,<19g4_g5_19g6_g7=reg128#1%bot
# asm 2: vrev64.i32 <19g4_g5_19g6_g7=d0,<19g4_g5_19g6_g7=d0
vrev64.i32 d0,d0

# qhasm:   h80[0,1] += f4_2f5_f6_2f7[0] signed* g4_19g5_g6_19g7[0];h80[2,3] += f4_2f5_f6_2f7[1] signed* g4_19g5_g6_19g7[1] 
# asm 1: vmlal.s32 <h80=reg128#16,<f4_2f5_f6_2f7=reg128#14%bot,<g4_19g5_g6_19g7=reg128#13%bot
# asm 2: vmlal.s32 <h80=q15,<f4_2f5_f6_2f7=d26,<g4_19g5_g6_19g7=d24
vmlal.s32 q15,d26,d24

# qhasm:   19g4_g5_19g6_g7 = 19g4_g5_19g6_g7[0,1]g4_g5_g6_g7[3]19g4_19g5_19g6_19g7[2] 
# asm 1: vext.32 <19g4_g5_19g6_g7=reg128#1%top,<g4_g5_g6_g7=reg128#4%top,<19g4_19g5_19g6_19g7=reg128#10%top,#1
# asm 2: vext.32 <19g4_g5_19g6_g7=d1,<g4_g5_g6_g7=d7,<19g4_19g5_19g6_19g7=d19,#1
vext.32 d1,d7,d19,#1

# qhasm:   new h19 

# qhasm:   h19 = h19[0,1]h97[0,1] 
# asm 1: vext.32 <h19=reg128#4%top,<h97=reg128#6%bot,<h97=reg128#6%bot,#0
# asm 2: vext.32 <h19=d7,<h97=d10,<h97=d10,#0
vext.32 d7,d10,d10,#0

# qhasm:   h80[0,1] += f4_2f5_f6_2f7[2] signed* g0_19g1_g2_19g3[2];h80[2,3] += f4_2f5_f6_2f7[3] signed* g0_19g1_g2_19g3[3] 
# asm 1: vmlal.s32 <h80=reg128#16,<f4_2f5_f6_2f7=reg128#14%top,<g0_19g1_g2_19g3=reg128#12%top
# asm 2: vmlal.s32 <h80=q15,<f4_2f5_f6_2f7=d27,<g0_19g1_g2_19g3=d23
vmlal.s32 q15,d27,d23

# qhasm:   19g4_g5_19g6_g7 = 19g4_g5_19g6_g7[0,1]19g4_g5_19g6_g7[3]19g4_g5_19g6_g7[2] 
# asm 1: vrev64.i32 <19g4_g5_19g6_g7=reg128#1%top,<19g4_g5_19g6_g7=reg128#1%top
# asm 2: vrev64.i32 <19g4_g5_19g6_g7=d1,<19g4_g5_19g6_g7=d1
vrev64.i32 d1,d1

# qhasm:   h19 aligned= mem64[playp]h19[1] 
# asm 1: vld1.8 {<h19=reg128#4%bot},[<playp=int32#3,: 64]
# asm 2: vld1.8 {<h19=d6},[<playp=r2,: 64]
vld1.8 {d6},[r2,: 64]

# qhasm:   h80[0,1] += f8_2f9_f9_f6[0] signed* g0_19g1_g2_19g3[0];h80[2,3] += f8_2f9_f9_f6[1] signed* g0_19g1_g2_19g3[1] 
# asm 1: vmlal.s32 <h80=reg128#16,<f8_2f9_f9_f6=reg128#15%bot,<g0_19g1_g2_19g3=reg128#12%bot
# asm 2: vmlal.s32 <h80=q15,<f8_2f9_f9_f6=d28,<g0_19g1_g2_19g3=d22
vmlal.s32 q15,d28,d22

# qhasm:   h19[0,1] += f1_f8_f3_f0[0] signed* g0_g1_g2_g3[0];h19[2,3] += f1_f8_f3_f0[1] signed* g0_g1_g2_g3[1] 
# asm 1: vmlal.s32 <h19=reg128#4,<f1_f8_f3_f0=reg128#9%bot,<g0_g1_g2_g3=reg128#3%bot
# asm 2: vmlal.s32 <h19=q3,<f1_f8_f3_f0=d16,<g0_g1_g2_g3=d4
vmlal.s32 q3,d16,d4

# qhasm:   playp+=24 
# asm 1: add >playp=int32#3,<playp=int32#3,#24
# asm 2: add >playp=r2,<playp=r2,#24
add r2,r2,#24

# qhasm:   h19[0,1] += f1_f8_f3_f0[2] signed* 19g8_g9_19g2_g3[0];h19[2,3] += f1_f8_f3_f0[3] signed* 19g8_g9_19g2_g3[1] 
# asm 1: vmlal.s32 <h19=reg128#4,<f1_f8_f3_f0=reg128#9%top,<19g8_g9_19g2_g3=reg128#2%bot
# asm 2: vmlal.s32 <h19=q3,<f1_f8_f3_f0=d17,<19g8_g9_19g2_g3=d2
vmlal.s32 q3,d17,d2

# qhasm:   new h04 

# qhasm:   h04 = h80[2,3]h04[2,3] 
# asm 1: vext.32 <h04=reg128#3%bot,<h80=reg128#16%top,<h80=reg128#16%bot,#0
# asm 2: vext.32 <h04=d4,<h80=d31,<h80=d30,#0
vext.32 d4,d31,d30,#0

# qhasm:   new h37 

# qhasm:   h37 = h37[0]h97[1] 
# asm 1: vmov <h37=reg128#9%top,<h97=reg128#6%top
# asm 2: vmov <h37=d17,<h97=d11
vmov d17,d11

# qhasm:   h19[0,1] += f5_f2_f7_f4[0] signed* 19g4_g5_19g6_g7[2];h19[2,3] += f5_f2_f7_f4[1] signed* 19g4_g5_19g6_g7[3] 
# asm 1: vmlal.s32 <h19=reg128#4,<f5_f2_f7_f4=reg128#8%bot,<19g4_g5_19g6_g7=reg128#1%top
# asm 2: vmlal.s32 <h19=q3,<f5_f2_f7_f4=d14,<19g4_g5_19g6_g7=d1
vmlal.s32 q3,d14,d1

# qhasm:   new h15 

# qhasm:   h15 = h15[0,1]h75[2,3] 
# asm 1: vext.32 <h15=reg128#6%top,<h75=reg128#7%top,<h75=reg128#7%top,#0
# asm 2: vext.32 <h15=d11,<h75=d13,<h75=d13,#0
vext.32 d11,d13,d13,#0

# qhasm:   new h48 

# qhasm:   h48 = h48[0,1]h80[0,1] 
# asm 1: vext.32 <h48=reg128#7%top,<h80=reg128#16%bot,<h80=reg128#16%bot,#0
# asm 2: vext.32 <h48=d13,<h80=d30,<h80=d30,#0
vext.32 d13,d30,d30,#0

# qhasm:   h19[0,1] += f5_f2_f7_f4[2] signed* 19g4_g5_19g6_g7[0];h19[2,3] += f5_f2_f7_f4[3] signed* 19g4_g5_19g6_g7[1] 
# asm 1: vmlal.s32 <h19=reg128#4,<f5_f2_f7_f4=reg128#8%top,<19g4_g5_19g6_g7=reg128#1%bot
# asm 2: vmlal.s32 <h19=q3,<f5_f2_f7_f4=d15,<19g4_g5_19g6_g7=d0
vmlal.s32 q3,d15,d0

# qhasm:   new h26 

# qhasm:   h26 = h26[0,1]h68[0,1] 
# asm 1: vext.32 <h26=reg128#1%top,<h68=reg128#5%bot,<h68=reg128#5%bot,#0
# asm 2: vext.32 <h26=d1,<h68=d8,<h68=d8,#0
vext.32 d1,d8,d8,#0

# qhasm:   h19[0,1] += f8_2f9_f9_f6[2] signed* 19g8_g9_19g2_g3[2];h19[2,3] += f8_2f9_f9_f6[3] signed* 19g8_g9_19g2_g3[3] 
# asm 1: vmlal.s32 <h19=reg128#4,<f8_2f9_f9_f6=reg128#15%top,<19g8_g9_19g2_g3=reg128#2%top
# asm 2: vmlal.s32 <h19=q3,<f8_2f9_f9_f6=d29,<19g8_g9_19g2_g3=d3
vmlal.s32 q3,d29,d3

# qhasm:   h04 aligned= h04[0]mem64[playp] 
# asm 1: vld1.8 {<h04=reg128#3%top},[<playp=int32#3,: 64]
# asm 2: vld1.8 {<h04=d5},[<playp=r2,: 64]
vld1.8 {d5},[r2,: 64]

# qhasm:   playp -= 16 
# asm 1: sub >playp=int32#3,<playp=int32#3,#16
# asm 2: sub >playp=r2,<playp=r2,#16
sub r2,r2,#16

# qhasm:   h15 = h19[0,1]h15[2,3] 
# asm 1: vext.32 <h15=reg128#6%bot,<h19=reg128#4%bot,<h19=reg128#4%bot,#0
# asm 2: vext.32 <h15=d10,<h19=d6,<h19=d6,#0
vext.32 d10,d6,d6,#0

# qhasm:   4x mask26 = 0xffffffff 
# asm 1: vmov.i32 >mask26=reg128#2,#0xffffffff
# asm 2: vmov.i32 >mask26=q1,#0xffffffff
vmov.i32 q1,#0xffffffff

# qhasm:   2x mask25 = mask26 << 25 
# asm 1: vshl.i64 >mask25=reg128#5,<mask26=reg128#2,#25
# asm 2: vshl.i64 >mask25=q4,<mask26=q1,#25
vshl.i64 q4,q1,#25

# qhasm:   ptr = &_0x2000000_stack 
# asm 1: lea >ptr=int32#6,<_0x2000000_stack=stack128#1
# asm 2: lea >ptr=r5,<_0x2000000_stack=[sp,#512]
add r5,sp,#512

# qhasm:   _0x2000000 aligned= mem128[ptr] 
# asm 1: vld1.8 {>_0x2000000=reg128#8%bot->_0x2000000=reg128#8%top},[<ptr=int32#6,: 128]
# asm 2: vld1.8 {>_0x2000000=d14->_0x2000000=d15},[<ptr=r5,: 128]
vld1.8 {d14-d15},[r5,: 128]

# qhasm:   2x t0 = h04 + _0x2000000 
# asm 1: vadd.i64 >t0=reg128#10,<h04=reg128#3,<_0x2000000=reg128#8
# asm 2: vadd.i64 >t0=q9,<h04=q2,<_0x2000000=q7
vadd.i64 q9,q2,q7

# qhasm:   2x mask26 <<= 26 
# asm 1: vshl.i64 >mask26=reg128#2,<mask26=reg128#2,#26
# asm 2: vshl.i64 >mask26=q1,<mask26=q1,#26
vshl.i64 q1,q1,#26

# qhasm:   2x c = t0 signed>> 26 
# asm 1: vshr.s64 >c=reg128#11,<t0=reg128#10,#26
# asm 2: vshr.s64 >c=q10,<t0=q9,#26
vshr.s64 q10,q9,#26

# qhasm:   h26 aligned= mem64[playp]h26[1];playp += 8 
# asm 1: vld1.8 {<h26=reg128#1%bot},[<playp=int32#3,: 64]!
# asm 2: vld1.8 {<h26=d0},[<playp=r2,: 64]!
vld1.8 {d0},[r2,: 64]!

# qhasm:   2x h15 += c 
# asm 1: vadd.i64 >h15=reg128#6,<h15=reg128#6,<c=reg128#11
# asm 2: vadd.i64 >h15=q5,<h15=q5,<c=q10
vadd.i64 q5,q5,q10

# qhasm:   t0 &= mask26 
# asm 1: vand >t0=reg128#10,<t0=reg128#10,<mask26=reg128#2
# asm 2: vand >t0=q9,<t0=q9,<mask26=q1
vand q9,q9,q1

# qhasm:   h37 aligned= mem64[playp]h37[1];playp += 8 
# asm 1: vld1.8 {<h37=reg128#9%bot},[<playp=int32#3,: 64]!
# asm 2: vld1.8 {<h37=d16},[<playp=r2,: 64]!
vld1.8 {d16},[r2,: 64]!

# qhasm:   ptr = &_0x1000000_stack 
# asm 1: lea >ptr=int32#3,<_0x1000000_stack=stack128#2
# asm 2: lea >ptr=r2,<_0x1000000_stack=[sp,#528]
add r2,sp,#528

# qhasm:   _0x1000000 aligned= mem128[ptr] 
# asm 1: vld1.8 {>_0x1000000=reg128#11%bot->_0x1000000=reg128#11%top},[<ptr=int32#3,: 128]
# asm 2: vld1.8 {>_0x1000000=d20->_0x1000000=d21},[<ptr=r2,: 128]
vld1.8 {d20-d21},[r2,: 128]

# qhasm:   2x t1 = h15 + _0x1000000 
# asm 1: vadd.i64 >t1=reg128#12,<h15=reg128#6,<_0x1000000=reg128#11
# asm 2: vadd.i64 >t1=q11,<h15=q5,<_0x1000000=q10
vadd.i64 q11,q5,q10

# qhasm:   2x h04 -= t0 
# asm 1: vsub.i64 >h04=reg128#3,<h04=reg128#3,<t0=reg128#10
# asm 2: vsub.i64 >h04=q2,<h04=q2,<t0=q9
vsub.i64 q2,q2,q9

# qhasm:   2x c = t1 signed>> 25 
# asm 1: vshr.s64 >c=reg128#10,<t1=reg128#12,#25
# asm 2: vshr.s64 >c=q9,<t1=q11,#25
vshr.s64 q9,q11,#25

# qhasm:   h48 = h04[2,3]h48[2,3] 
# asm 1: vext.32 <h48=reg128#7%bot,<h04=reg128#3%top,<h04=reg128#3%bot,#0
# asm 2: vext.32 <h48=d12,<h04=d5,<h04=d4,#0
vext.32 d12,d5,d4,#0

# qhasm:   t1 &= mask25 
# asm 1: vand >t1=reg128#12,<t1=reg128#12,<mask25=reg128#5
# asm 2: vand >t1=q11,<t1=q11,<mask25=q4
vand q11,q11,q4

# qhasm:   2x h26 += c 
# asm 1: vadd.i64 >h26=reg128#1,<h26=reg128#1,<c=reg128#10
# asm 2: vadd.i64 >h26=q0,<h26=q0,<c=q9
vadd.i64 q0,q0,q9

# qhasm:   new h59 

# qhasm:   h59 = h59[0]h19[1] 
# asm 1: vmov <h59=reg128#10%top,<h19=reg128#4%top
# asm 2: vmov <h59=d19,<h19=d7
vmov d19,d7

# qhasm:   2x t0 = h26 + _0x2000000 
# asm 1: vadd.i64 >t0=reg128#4,<h26=reg128#1,<_0x2000000=reg128#8
# asm 2: vadd.i64 >t0=q3,<h26=q0,<_0x2000000=q7
vadd.i64 q3,q0,q7

# qhasm:   2x h15 -= t1 
# asm 1: vsub.i64 >h15=reg128#6,<h15=reg128#6,<t1=reg128#12
# asm 2: vsub.i64 >h15=q5,<h15=q5,<t1=q11
vsub.i64 q5,q5,q11

# qhasm:   2x c = t0 signed>> 26 
# asm 1: vshr.s64 >c=reg128#12,<t0=reg128#4,#26
# asm 2: vshr.s64 >c=q11,<t0=q3,#26
vshr.s64 q11,q3,#26

# qhasm:   h59 = h15[2,3]h59[2,3] 
# asm 1: vext.32 <h59=reg128#10%bot,<h15=reg128#6%top,<h15=reg128#6%bot,#0
# asm 2: vext.32 <h59=d18,<h15=d11,<h15=d10,#0
vext.32 d18,d11,d10,#0

# qhasm:   t0 &= mask26 
# asm 1: vand >t0=reg128#4,<t0=reg128#4,<mask26=reg128#2
# asm 2: vand >t0=q3,<t0=q3,<mask26=q1
vand q3,q3,q1

# qhasm:   2x h37 += c 
# asm 1: vadd.i64 >h37=reg128#9,<h37=reg128#9,<c=reg128#12
# asm 2: vadd.i64 >h37=q8,<h37=q8,<c=q11
vadd.i64 q8,q8,q11

# qhasm:   2x t1 = h37 + _0x1000000 
# asm 1: vadd.i64 >t1=reg128#12,<h37=reg128#9,<_0x1000000=reg128#11
# asm 2: vadd.i64 >t1=q11,<h37=q8,<_0x1000000=q10
vadd.i64 q11,q8,q10

# qhasm:   2x h26 -= t0 
# asm 1: vsub.i64 >h26=reg128#1,<h26=reg128#1,<t0=reg128#4
# asm 2: vsub.i64 >h26=q0,<h26=q0,<t0=q3
vsub.i64 q0,q0,q3

# qhasm:   2x c = t1 signed>> 25 
# asm 1: vshr.s64 >c=reg128#4,<t1=reg128#12,#25
# asm 2: vshr.s64 >c=q3,<t1=q11,#25
vshr.s64 q3,q11,#25

# qhasm:   t1 &= mask25 
# asm 1: vand >t1=reg128#12,<t1=reg128#12,<mask25=reg128#5
# asm 2: vand >t1=q11,<t1=q11,<mask25=q4
vand q11,q11,q4

# qhasm:   2x h48 += c 
# asm 1: vadd.i64 >h48=reg128#4,<h48=reg128#7,<c=reg128#4
# asm 2: vadd.i64 >h48=q3,<h48=q6,<c=q3
vadd.i64 q3,q6,q3

# qhasm:   2x t0 = h48 + _0x2000000 
# asm 1: vadd.i64 >t0=reg128#7,<h48=reg128#4,<_0x2000000=reg128#8
# asm 2: vadd.i64 >t0=q6,<h48=q3,<_0x2000000=q7
vadd.i64 q6,q3,q7

# qhasm:   2x h37 -= t1 
# asm 1: vsub.i64 >h37=reg128#9,<h37=reg128#9,<t1=reg128#12
# asm 2: vsub.i64 >h37=q8,<h37=q8,<t1=q11
vsub.i64 q8,q8,q11

# qhasm:   2x c = t0 signed>> 26 
# asm 1: vshr.s64 >c=reg128#12,<t0=reg128#7,#26
# asm 2: vshr.s64 >c=q11,<t0=q6,#26
vshr.s64 q11,q6,#26

# qhasm:   t0 &= mask26 
# asm 1: vand >t0=reg128#7,<t0=reg128#7,<mask26=reg128#2
# asm 2: vand >t0=q6,<t0=q6,<mask26=q1
vand q6,q6,q1

# qhasm:   2x h59 += c 
# asm 1: vadd.i64 >h59=reg128#10,<h59=reg128#10,<c=reg128#12
# asm 2: vadd.i64 >h59=q9,<h59=q9,<c=q11
vadd.i64 q9,q9,q11

# qhasm:   new t 

# qhasm:   t = t[0], h59[1] + _0x1000000[1] 
# asm 1: vadd.i64 <t=reg128#13%top,<h59=reg128#10%top,<_0x1000000=reg128#11%top
# asm 2: vadd.i64 <t=d25,<h59=d19,<_0x1000000=d21
vadd.i64 d25,d19,d21

# qhasm:   2x h48 -= t0 
# asm 1: vsub.i64 >h48=reg128#4,<h48=reg128#4,<t0=reg128#7
# asm 2: vsub.i64 >h48=q3,<h48=q3,<t0=q6
vsub.i64 q3,q3,q6

# qhasm:   c = c[0],t[1] signed>> 25 
# asm 1: vshr.s64 <c=reg128#12%top,<t=reg128#13%top,#25
# asm 2: vshr.s64 <c=d23,<t=d25,#25
vshr.s64 d23,d25,#25

# qhasm:   t &= mask25 
# asm 1: vand >t=reg128#5,<t=reg128#13,<mask25=reg128#5
# asm 2: vand >t=q4,<t=q12,<mask25=q4
vand q4,q12,q4

# qhasm:   new s2 

# qhasm:   s2 = s2[0],c[1] + c[1] 
# asm 1: vadd.i64 <s2=reg128#11%top,<c=reg128#12%top,<c=reg128#12%top
# asm 2: vadd.i64 <s2=d21,<c=d23,<c=d23
vadd.i64 d21,d23,d23

# qhasm:   new s 

# qhasm:   s = s[0],c[1] << 4 
# asm 1: vshl.i64 <s=reg128#13%top,<c=reg128#12%top,#4
# asm 2: vshl.i64 <s=d25,<c=d23,#4
vshl.i64 d25,d23,#4

# qhasm:   s2 = s2[0],s2[1] + c[1] 
# asm 1: vadd.i64 <s2=reg128#11%top,<s2=reg128#11%top,<c=reg128#12%top
# asm 2: vadd.i64 <s2=d21,<s2=d21,<c=d23
vadd.i64 d21,d21,d23

# qhasm:   s = s[0],s[1] + s2[1] 
# asm 1: vadd.i64 <s=reg128#13%top,<s=reg128#13%top,<s2=reg128#11%top
# asm 2: vadd.i64 <s=d25,<s=d25,<s2=d21
vadd.i64 d25,d25,d21

# qhasm:   h04 = h04[0] + s[1],h04[1] 
# asm 1: vadd.i64 <h04=reg128#3%bot,<h04=reg128#3%bot,<s=reg128#13%top
# asm 2: vadd.i64 <h04=d4,<h04=d4,<s=d25
vadd.i64 d4,d4,d25

# qhasm:   h26[0,1,2,3] h37[0,1,2,3] = h26[0]h37[0]h26[1]h37[1] h26[2]h37[2]h26[3]h37[3] 
# asm 1: vzip.i32 <h26=reg128#1,<h37=reg128#9
# asm 2: vzip.i32 <h26=q0,<h37=q8
vzip.i32 q0,q8

# qhasm:   t0 = h04[0] + _0x2000000[0],t0[1] 
# asm 1: vadd.i64 <t0=reg128#7%bot,<h04=reg128#3%bot,<_0x2000000=reg128#8%bot
# asm 2: vadd.i64 <t0=d12,<h04=d4,<_0x2000000=d14
vadd.i64 d12,d4,d14

# qhasm:   posh += 8 
# asm 1: add >posh=int32#3,<posh=int32#7,#8
# asm 2: add >posh=r2,<posh=r6,#8
add r2,r6,#8

# qhasm:   mem64[posh] aligned= h26[0] 
# asm 1: vst1.8 <h26=reg128#1%bot,[<posh=int32#3,: 64]
# asm 2: vst1.8 <h26=d0,[<posh=r2,: 64]
vst1.8 d0,[r2,: 64]

# qhasm:   h59 = h59[0],h59[1] - t[1] 
# asm 1: vsub.i64 <h59=reg128#10%top,<h59=reg128#10%top,<t=reg128#5%top
# asm 2: vsub.i64 <h59=d19,<h59=d19,<t=d9
vsub.i64 d19,d19,d9

# qhasm:   posh += 16 
# asm 1: add >posh=int32#3,<posh=int32#3,#16
# asm 2: add >posh=r2,<posh=r2,#16
add r2,r2,#16

# qhasm:   mem64[posh] aligned= h37[0] 
# asm 1: vst1.8 <h37=reg128#9%bot,[<posh=int32#3,: 64]
# asm 2: vst1.8 <h37=d16,[<posh=r2,: 64]
vst1.8 d16,[r2,: 64]

# qhasm:   c = t0[0] signed>> 26,c[1] 
# asm 1: vshr.s64 <c=reg128#12%bot,<t0=reg128#7%bot,#26
# asm 2: vshr.s64 <c=d22,<t0=d12,#26
vshr.s64 d22,d12,#26

# qhasm:   t0 &= mask26 
# asm 1: vand >t0=reg128#1,<t0=reg128#7,<mask26=reg128#2
# asm 2: vand >t0=q0,<t0=q6,<mask26=q1
vand q0,q6,q1

# qhasm:   h15 = h15[0] + c[0],h15[1] 
# asm 1: vadd.i64 <h15=reg128#6%bot,<h15=reg128#6%bot,<c=reg128#12%bot
# asm 2: vadd.i64 <h15=d10,<h15=d10,<c=d22
vadd.i64 d10,d10,d22

# qhasm:   h48[0,1,2,3] h59[0,1,2,3] = h48[0]h59[0]h48[1]h59[1] h48[2]h59[2]h48[3]h59[3] 
# asm 1: vzip.i32 <h48=reg128#4,<h59=reg128#10
# asm 2: vzip.i32 <h48=q3,<h59=q9
vzip.i32 q3,q9

# qhasm:   h04 = h04[0] - t0[0],h04[1] 
# asm 1: vsub.i64 <h04=reg128#3%bot,<h04=reg128#3%bot,<t0=reg128#1%bot
# asm 2: vsub.i64 <h04=d4,<h04=d4,<t0=d0
vsub.i64 d4,d4,d0

# qhasm:   posh -= 8 
# asm 1: sub >posh=int32#3,<posh=int32#3,#8
# asm 2: sub >posh=r2,<posh=r2,#8
sub r2,r2,#8

# qhasm:   mem64[posh] aligned= h48[0] 
# asm 1: vst1.8 <h48=reg128#4%bot,[<posh=int32#3,: 64]
# asm 2: vst1.8 <h48=d6,[<posh=r2,: 64]
vst1.8 d6,[r2,: 64]

# qhasm:   posh += 16 
# asm 1: add >posh=int32#3,<posh=int32#3,#16
# asm 2: add >posh=r2,<posh=r2,#16
add r2,r2,#16

# qhasm:   mem64[posh] aligned= h59[0] 
# asm 1: vst1.8 <h59=reg128#10%bot,[<posh=int32#3,: 64]
# asm 2: vst1.8 <h59=d18,[<posh=r2,: 64]
vst1.8 d18,[r2,: 64]

# qhasm:   h04[0,1,2,3] h15[0,1,2,3] = h04[0]h15[0]h04[1]h15[1] h04[2]h15[2]h04[3]h15[3] 
# asm 1: vzip.i32 <h04=reg128#3,<h15=reg128#6
# asm 2: vzip.i32 <h04=q2,<h15=q5
vzip.i32 q2,q5

# qhasm:   posh -= 32 
# asm 1: sub >posh=int32#3,<posh=int32#3,#32
# asm 2: sub >posh=r2,<posh=r2,#32
sub r2,r2,#32

# qhasm:   mem64[posh] aligned= h04[0] 
# asm 1: vst1.8 <h04=reg128#3%bot,[<posh=int32#3,: 64]
# asm 2: vst1.8 <h04=d4,[<posh=r2,: 64]
vst1.8 d4,[r2,: 64]

# qhasm:                        =? postcopy - 0
# asm 1: cmp <postcopy=int32#5,#0
# asm 2: cmp <postcopy=r4,#0
cmp r4,#0

# qhasm:   goto skippostcopy if =
beq ._skippostcopy

# qhasm:   posy = playground1_ptr + 144 
# asm 1: add >posy=int32#3,<playground1_ptr=int32#4,#144
# asm 2: add >posy=r2,<playground1_ptr=r3,#144
add r2,r3,#144

# qhasm:   posx = postcopy 
# asm 1: mov >posx=int32#5,<postcopy=int32#5
# asm 2: mov >posx=r4,<postcopy=r4
mov r4,r4

# qhasm:   f0 aligned= mem128[posy];posy += 16 
# asm 1: vld1.8 {>f0=reg128#1%bot->f0=reg128#1%top},[<posy=int32#3,: 128]!
# asm 2: vld1.8 {>f0=d0->f0=d1},[<posy=r2,: 128]!
vld1.8 {d0-d1},[r2,: 128]!

# qhasm:   f4 aligned= mem128[posy];posy += 16 
# asm 1: vld1.8 {>f4=reg128#2%bot->f4=reg128#2%top},[<posy=int32#3,: 128]!
# asm 2: vld1.8 {>f4=d2->f4=d3},[<posy=r2,: 128]!
vld1.8 {d2-d3},[r2,: 128]!

# qhasm:   new f8 

# qhasm:   f8 aligned= mem64[posy] f8[1] 
# asm 1: vld1.8 {<f8=reg128#3%bot},[<posy=int32#3,: 64]
# asm 2: vld1.8 {<f8=d4},[<posy=r2,: 64]
vld1.8 {d4},[r2,: 64]

# qhasm:   mem128[posx] aligned= f0;posx += 16 
# asm 1: vst1.8 {<f0=reg128#1%bot-<f0=reg128#1%top},[<posx=int32#5,: 128]!
# asm 2: vst1.8 {<f0=d0-<f0=d1},[<posx=r4,: 128]!
vst1.8 {d0-d1},[r4,: 128]!

# qhasm:   mem128[posx] aligned= f4;posx += 16 
# asm 1: vst1.8 {<f4=reg128#2%bot-<f4=reg128#2%top},[<posx=int32#5,: 128]!
# asm 2: vst1.8 {<f4=d2-<f4=d3},[<posx=r4,: 128]!
vst1.8 {d2-d3},[r4,: 128]!

# qhasm:   mem64[posx] aligned= f8[0] 
# asm 1: vst1.8 <f8=reg128#3%bot,[<posx=int32#5,: 64]
# asm 2: vst1.8 <f8=d4,[<posx=r4,: 64]
vst1.8 d4,[r4,: 64]

# qhasm:   skippostcopy:
._skippostcopy:

# qhasm:                          =? i - 1
# asm 1: cmp <i=int32#2,#1
# asm 2: cmp <i=r1,#1
cmp r1,#1

# qhasm:   goto skipfinalcopy if !=
bne ._skipfinalcopy

# qhasm:   posy = playground1_ptr + 288 
# asm 1: add >posy=int32#3,<playground1_ptr=int32#4,#288
# asm 2: add >posy=r2,<playground1_ptr=r3,#288
add r2,r3,#288

# qhasm:   posx = playground1_ptr + 144 
# asm 1: add >posx=int32#5,<playground1_ptr=int32#4,#144
# asm 2: add >posx=r4,<playground1_ptr=r3,#144
add r4,r3,#144

# qhasm:   f0 aligned= mem128[posy];posy += 16 
# asm 1: vld1.8 {>f0=reg128#1%bot->f0=reg128#1%top},[<posy=int32#3,: 128]!
# asm 2: vld1.8 {>f0=d0->f0=d1},[<posy=r2,: 128]!
vld1.8 {d0-d1},[r2,: 128]!

# qhasm:   f4 aligned= mem128[posy];posy += 16 
# asm 1: vld1.8 {>f4=reg128#2%bot->f4=reg128#2%top},[<posy=int32#3,: 128]!
# asm 2: vld1.8 {>f4=d2->f4=d3},[<posy=r2,: 128]!
vld1.8 {d2-d3},[r2,: 128]!

# qhasm:   new f8 

# qhasm:   f8 aligned= mem64[posy] f8[1] 
# asm 1: vld1.8 {<f8=reg128#3%bot},[<posy=int32#3,: 64]
# asm 2: vld1.8 {<f8=d4},[<posy=r2,: 64]
vld1.8 {d4},[r2,: 64]

# qhasm:   mem128[posx] aligned= f0;posx += 16 
# asm 1: vst1.8 {<f0=reg128#1%bot-<f0=reg128#1%top},[<posx=int32#5,: 128]!
# asm 2: vst1.8 {<f0=d0-<f0=d1},[<posx=r4,: 128]!
vst1.8 {d0-d1},[r4,: 128]!

# qhasm:   mem128[posx] aligned= f4;posx += 16 
# asm 1: vst1.8 {<f4=reg128#2%bot-<f4=reg128#2%top},[<posx=int32#5,: 128]!
# asm 2: vst1.8 {<f4=d2-<f4=d3},[<posx=r4,: 128]!
vst1.8 {d2-d3},[r4,: 128]!

# qhasm:   mem64[posx] aligned= f8[0] 
# asm 1: vst1.8 <f8=reg128#3%bot,[<posx=int32#5,: 64]
# asm 2: vst1.8 <f8=d4,[<posx=r4,: 64]
vst1.8 d4,[r4,: 64]

# qhasm:   skipfinalcopy:
._skipfinalcopy:

# qhasm:                               i += 1
# asm 1: add >i=int32#2,<i=int32#2,#1
# asm 2: add >i=r1,<i=r1,#1
add r1,r1,#1

# qhasm:                    unsigned<? i - 12
# asm 1: cmp <i=int32#2,#12
# asm 2: cmp <i=r1,#12
cmp r1,#12

# qhasm: goto invertloop if unsigned<
blo ._invertloop

# qhasm:   posf = playground1_ptr + 144
# asm 1: add >posf=int32#2,<playground1_ptr=int32#4,#144
# asm 2: add >posf=r1,<playground1_ptr=r3,#144
add r1,r3,#144

# qhasm:   out0 = mem32[posf];posf += 4
# asm 1: ldr >out0=int32#3,[<posf=int32#2],#4
# asm 2: ldr >out0=r2,[<posf=r1],#4
ldr r2,[r1],#4

# qhasm:   out1 = mem32[posf];posf += 4
# asm 1: ldr >out1=int32#4,[<posf=int32#2],#4
# asm 2: ldr >out1=r3,[<posf=r1],#4
ldr r3,[r1],#4

# qhasm:   out2 = mem32[posf];posf += 4
# asm 1: ldr >out2=int32#5,[<posf=int32#2],#4
# asm 2: ldr >out2=r4,[<posf=r1],#4
ldr r4,[r1],#4

# qhasm:   out3 = mem32[posf];posf += 4
# asm 1: ldr >out3=int32#6,[<posf=int32#2],#4
# asm 2: ldr >out3=r5,[<posf=r1],#4
ldr r5,[r1],#4

# qhasm:   out4 = mem32[posf];posf += 4
# asm 1: ldr >out4=int32#7,[<posf=int32#2],#4
# asm 2: ldr >out4=r6,[<posf=r1],#4
ldr r6,[r1],#4

# qhasm:   out5 = mem32[posf];posf += 4
# asm 1: ldr >out5=int32#8,[<posf=int32#2],#4
# asm 2: ldr >out5=r7,[<posf=r1],#4
ldr r7,[r1],#4

# qhasm:   out6 = mem32[posf];posf += 4
# asm 1: ldr >out6=int32#9,[<posf=int32#2],#4
# asm 2: ldr >out6=r8,[<posf=r1],#4
ldr r8,[r1],#4

# qhasm:   out7 = mem32[posf];posf += 4
# asm 1: ldr >out7=int32#10,[<posf=int32#2],#4
# asm 2: ldr >out7=r9,[<posf=r1],#4
ldr r9,[r1],#4

# qhasm:   out8 = mem32[posf];posf += 4
# asm 1: ldr >out8=int32#11,[<posf=int32#2],#4
# asm 2: ldr >out8=r10,[<posf=r1],#4
ldr r10,[r1],#4

# qhasm:   out9 = mem32[posf]
# asm 1: ldr >out9=int32#2,[<posf=int32#2]
# asm 2: ldr >out9=r1,[<posf=r1]
ldr r1,[r1]

# qhasm:   carry = out9 + (out9 << 4)
# asm 1: add >carry=int32#12,<out9=int32#2,<out9=int32#2,LSL #4
# asm 2: add >carry=r11,<out9=r1,<out9=r1,LSL #4
add r11,r1,r1,LSL #4

# qhasm:   carry = carry + (out9 << 1)
# asm 1: add >carry=int32#12,<carry=int32#12,<out9=int32#2,LSL #1
# asm 2: add >carry=r11,<carry=r11,<out9=r1,LSL #1
add r11,r11,r1,LSL #1

# qhasm:   carry += 16777216
# asm 1: add >carry=int32#12,<carry=int32#12,#16777216
# asm 2: add >carry=r11,<carry=r11,#16777216
add r11,r11,#16777216

# qhasm:   carry signed>>= 25
# asm 1: mov >carry=int32#12,<carry=int32#12,ASR #25
# asm 2: mov >carry=r11,<carry=r11,ASR #25
mov r11,r11,ASR #25

# qhasm:   carry += out0
# asm 1: add >carry=int32#12,<carry=int32#12,<out0=int32#3
# asm 2: add >carry=r11,<carry=r11,<out0=r2
add r11,r11,r2

# qhasm:   carry signed>>= 26
# asm 1: mov >carry=int32#12,<carry=int32#12,ASR #26
# asm 2: mov >carry=r11,<carry=r11,ASR #26
mov r11,r11,ASR #26

# qhasm:   carry += out1
# asm 1: add >carry=int32#12,<carry=int32#12,<out1=int32#4
# asm 2: add >carry=r11,<carry=r11,<out1=r3
add r11,r11,r3

# qhasm:   carry signed>>= 25
# asm 1: mov >carry=int32#12,<carry=int32#12,ASR #25
# asm 2: mov >carry=r11,<carry=r11,ASR #25
mov r11,r11,ASR #25

# qhasm:   carry += out2
# asm 1: add >carry=int32#12,<carry=int32#12,<out2=int32#5
# asm 2: add >carry=r11,<carry=r11,<out2=r4
add r11,r11,r4

# qhasm:   carry signed>>= 26
# asm 1: mov >carry=int32#12,<carry=int32#12,ASR #26
# asm 2: mov >carry=r11,<carry=r11,ASR #26
mov r11,r11,ASR #26

# qhasm:   carry += out3
# asm 1: add >carry=int32#12,<carry=int32#12,<out3=int32#6
# asm 2: add >carry=r11,<carry=r11,<out3=r5
add r11,r11,r5

# qhasm:   carry signed>>= 25
# asm 1: mov >carry=int32#12,<carry=int32#12,ASR #25
# asm 2: mov >carry=r11,<carry=r11,ASR #25
mov r11,r11,ASR #25

# qhasm:   carry += out4
# asm 1: add >carry=int32#12,<carry=int32#12,<out4=int32#7
# asm 2: add >carry=r11,<carry=r11,<out4=r6
add r11,r11,r6

# qhasm:   carry signed>>= 26
# asm 1: mov >carry=int32#12,<carry=int32#12,ASR #26
# asm 2: mov >carry=r11,<carry=r11,ASR #26
mov r11,r11,ASR #26

# qhasm:   carry += out5
# asm 1: add >carry=int32#12,<carry=int32#12,<out5=int32#8
# asm 2: add >carry=r11,<carry=r11,<out5=r7
add r11,r11,r7

# qhasm:   carry signed>>= 25
# asm 1: mov >carry=int32#12,<carry=int32#12,ASR #25
# asm 2: mov >carry=r11,<carry=r11,ASR #25
mov r11,r11,ASR #25

# qhasm:   carry += out6
# asm 1: add >carry=int32#12,<carry=int32#12,<out6=int32#9
# asm 2: add >carry=r11,<carry=r11,<out6=r8
add r11,r11,r8

# qhasm:   carry signed>>= 26
# asm 1: mov >carry=int32#12,<carry=int32#12,ASR #26
# asm 2: mov >carry=r11,<carry=r11,ASR #26
mov r11,r11,ASR #26

# qhasm:   carry += out7
# asm 1: add >carry=int32#12,<carry=int32#12,<out7=int32#10
# asm 2: add >carry=r11,<carry=r11,<out7=r9
add r11,r11,r9

# qhasm:   carry signed>>= 25
# asm 1: mov >carry=int32#12,<carry=int32#12,ASR #25
# asm 2: mov >carry=r11,<carry=r11,ASR #25
mov r11,r11,ASR #25

# qhasm:   carry += out8
# asm 1: add >carry=int32#12,<carry=int32#12,<out8=int32#11
# asm 2: add >carry=r11,<carry=r11,<out8=r10
add r11,r11,r10

# qhasm:   carry signed>>= 26
# asm 1: mov >carry=int32#12,<carry=int32#12,ASR #26
# asm 2: mov >carry=r11,<carry=r11,ASR #26
mov r11,r11,ASR #26

# qhasm:   carry += out9
# asm 1: add >carry=int32#12,<carry=int32#12,<out9=int32#2
# asm 2: add >carry=r11,<carry=r11,<out9=r1
add r11,r11,r1

# qhasm:   carry signed>>= 25
# asm 1: mov >carry=int32#12,<carry=int32#12,ASR #25
# asm 2: mov >carry=r11,<carry=r11,ASR #25
mov r11,r11,ASR #25

# qhasm:   out0 += carry
# asm 1: add >out0=int32#3,<out0=int32#3,<carry=int32#12
# asm 2: add >out0=r2,<out0=r2,<carry=r11
add r2,r2,r11

# qhasm:   out0 += (carry << 1)
# asm 1: add >out0=int32#3,<out0=int32#3,<carry=int32#12,LSL #1
# asm 2: add >out0=r2,<out0=r2,<carry=r11,LSL #1
add r2,r2,r11,LSL #1

# qhasm:   out0 += (carry << 4)
# asm 1: add >out0=int32#3,<out0=int32#3,<carry=int32#12,LSL #4
# asm 2: add >out0=r2,<out0=r2,<carry=r11,LSL #4
add r2,r2,r11,LSL #4

# qhasm:   carry0 = (out0 signed>> 26)
# asm 1: mov >carry0=int32#12,<out0=int32#3,ASR #26
# asm 2: mov >carry0=r11,<out0=r2,ASR #26
mov r11,r2,ASR #26

# qhasm:   out1 += carry0
# asm 1: add >out1=int32#4,<out1=int32#4,<carry0=int32#12
# asm 2: add >out1=r3,<out1=r3,<carry0=r11
add r3,r3,r11

# qhasm:   out0 -= (carry0 << 26)
# asm 1: sub >out0=int32#3,<out0=int32#3,<carry0=int32#12,LSL #26
# asm 2: sub >out0=r2,<out0=r2,<carry0=r11,LSL #26
sub r2,r2,r11,LSL #26

# qhasm:   carry1 = (out1 signed>> 25)
# asm 1: mov >carry1=int32#12,<out1=int32#4,ASR #25
# asm 2: mov >carry1=r11,<out1=r3,ASR #25
mov r11,r3,ASR #25

# qhasm:   out2 += carry1
# asm 1: add >out2=int32#5,<out2=int32#5,<carry1=int32#12
# asm 2: add >out2=r4,<out2=r4,<carry1=r11
add r4,r4,r11

# qhasm:   out1 -= (carry1 << 25)
# asm 1: sub >out1=int32#4,<out1=int32#4,<carry1=int32#12,LSL #25
# asm 2: sub >out1=r3,<out1=r3,<carry1=r11,LSL #25
sub r3,r3,r11,LSL #25

# qhasm:   carry2 = (out2 signed>> 26)
# asm 1: mov >carry2=int32#12,<out2=int32#5,ASR #26
# asm 2: mov >carry2=r11,<out2=r4,ASR #26
mov r11,r4,ASR #26

# qhasm:   out3 += carry2
# asm 1: add >out3=int32#6,<out3=int32#6,<carry2=int32#12
# asm 2: add >out3=r5,<out3=r5,<carry2=r11
add r5,r5,r11

# qhasm:   out2 -= (carry2 << 26)
# asm 1: sub >out2=int32#5,<out2=int32#5,<carry2=int32#12,LSL #26
# asm 2: sub >out2=r4,<out2=r4,<carry2=r11,LSL #26
sub r4,r4,r11,LSL #26

# qhasm:   carry3 = (out3 signed>> 25)
# asm 1: mov >carry3=int32#12,<out3=int32#6,ASR #25
# asm 2: mov >carry3=r11,<out3=r5,ASR #25
mov r11,r5,ASR #25

# qhasm:   out4 += carry3
# asm 1: add >out4=int32#7,<out4=int32#7,<carry3=int32#12
# asm 2: add >out4=r6,<out4=r6,<carry3=r11
add r6,r6,r11

# qhasm:   out3 -= (carry3 << 25)
# asm 1: sub >out3=int32#6,<out3=int32#6,<carry3=int32#12,LSL #25
# asm 2: sub >out3=r5,<out3=r5,<carry3=r11,LSL #25
sub r5,r5,r11,LSL #25

# qhasm:   carry4 = (out4 signed>> 26)
# asm 1: mov >carry4=int32#12,<out4=int32#7,ASR #26
# asm 2: mov >carry4=r11,<out4=r6,ASR #26
mov r11,r6,ASR #26

# qhasm:   out5 += carry4
# asm 1: add >out5=int32#8,<out5=int32#8,<carry4=int32#12
# asm 2: add >out5=r7,<out5=r7,<carry4=r11
add r7,r7,r11

# qhasm:   out4 -= (carry4 << 26)
# asm 1: sub >out4=int32#7,<out4=int32#7,<carry4=int32#12,LSL #26
# asm 2: sub >out4=r6,<out4=r6,<carry4=r11,LSL #26
sub r6,r6,r11,LSL #26

# qhasm:   carry5 = (out5 signed>> 25)
# asm 1: mov >carry5=int32#12,<out5=int32#8,ASR #25
# asm 2: mov >carry5=r11,<out5=r7,ASR #25
mov r11,r7,ASR #25

# qhasm:   out6 += carry5
# asm 1: add >out6=int32#9,<out6=int32#9,<carry5=int32#12
# asm 2: add >out6=r8,<out6=r8,<carry5=r11
add r8,r8,r11

# qhasm:   out5 -= (carry5 << 25)
# asm 1: sub >out5=int32#8,<out5=int32#8,<carry5=int32#12,LSL #25
# asm 2: sub >out5=r7,<out5=r7,<carry5=r11,LSL #25
sub r7,r7,r11,LSL #25

# qhasm:   carry6 = (out6 signed>> 26)
# asm 1: mov >carry6=int32#12,<out6=int32#9,ASR #26
# asm 2: mov >carry6=r11,<out6=r8,ASR #26
mov r11,r8,ASR #26

# qhasm:   out7 += carry6
# asm 1: add >out7=int32#10,<out7=int32#10,<carry6=int32#12
# asm 2: add >out7=r9,<out7=r9,<carry6=r11
add r9,r9,r11

# qhasm:   out6 -= (carry6 << 26)
# asm 1: sub >out6=int32#9,<out6=int32#9,<carry6=int32#12,LSL #26
# asm 2: sub >out6=r8,<out6=r8,<carry6=r11,LSL #26
sub r8,r8,r11,LSL #26

# qhasm:   carry7 = (out7 signed>> 25)
# asm 1: mov >carry7=int32#12,<out7=int32#10,ASR #25
# asm 2: mov >carry7=r11,<out7=r9,ASR #25
mov r11,r9,ASR #25

# qhasm:   out8 += carry7
# asm 1: add >out8=int32#11,<out8=int32#11,<carry7=int32#12
# asm 2: add >out8=r10,<out8=r10,<carry7=r11
add r10,r10,r11

# qhasm:   out7 -= (carry7 << 25)
# asm 1: sub >out7=int32#10,<out7=int32#10,<carry7=int32#12,LSL #25
# asm 2: sub >out7=r9,<out7=r9,<carry7=r11,LSL #25
sub r9,r9,r11,LSL #25

# qhasm:   carry8 = (out8 signed>> 26)
# asm 1: mov >carry8=int32#12,<out8=int32#11,ASR #26
# asm 2: mov >carry8=r11,<out8=r10,ASR #26
mov r11,r10,ASR #26

# qhasm:   out9 += carry8
# asm 1: add >out9=int32#2,<out9=int32#2,<carry8=int32#12
# asm 2: add >out9=r1,<out9=r1,<carry8=r11
add r1,r1,r11

# qhasm:   out8 -= (carry8 << 26)
# asm 1: sub >out8=int32#11,<out8=int32#11,<carry8=int32#12,LSL #26
# asm 2: sub >out8=r10,<out8=r10,<carry8=r11,LSL #26
sub r10,r10,r11,LSL #26

# qhasm:   carry9 = (out9 signed>> 25)
# asm 1: mov >carry9=int32#12,<out9=int32#2,ASR #25
# asm 2: mov >carry9=r11,<out9=r1,ASR #25
mov r11,r1,ASR #25

# qhasm:   out9 -= (carry9 << 25)
# asm 1: sub >out9=int32#2,<out9=int32#2,<carry9=int32#12,LSL #25
# asm 2: sub >out9=r1,<out9=r1,<carry9=r11,LSL #25
sub r1,r1,r11,LSL #25

# qhasm:   out0 += (out1 << 26)
# asm 1: add >out0=int32#3,<out0=int32#3,<out1=int32#4,LSL #26
# asm 2: add >out0=r2,<out0=r2,<out1=r3,LSL #26
add r2,r2,r3,LSL #26

# qhasm:   out1 unsigned>>= 6
# asm 1: mov >out1=int32#4,<out1=int32#4,LSR #6
# asm 2: mov >out1=r3,<out1=r3,LSR #6
mov r3,r3,LSR #6

# qhasm:   out1 += (out2 << 19)
# asm 1: add >out1=int32#4,<out1=int32#4,<out2=int32#5,LSL #19
# asm 2: add >out1=r3,<out1=r3,<out2=r4,LSL #19
add r3,r3,r4,LSL #19

# qhasm:   out2 unsigned>>= 13
# asm 1: mov >out2=int32#5,<out2=int32#5,LSR #13
# asm 2: mov >out2=r4,<out2=r4,LSR #13
mov r4,r4,LSR #13

# qhasm:   out2 += (out3 << 13)
# asm 1: add >out2=int32#5,<out2=int32#5,<out3=int32#6,LSL #13
# asm 2: add >out2=r4,<out2=r4,<out3=r5,LSL #13
add r4,r4,r5,LSL #13

# qhasm:   out3 unsigned>>= 19
# asm 1: mov >out3=int32#6,<out3=int32#6,LSR #19
# asm 2: mov >out3=r5,<out3=r5,LSR #19
mov r5,r5,LSR #19

# qhasm:   out3 += (out4 << 6)
# asm 1: add >out3=int32#6,<out3=int32#6,<out4=int32#7,LSL #6
# asm 2: add >out3=r5,<out3=r5,<out4=r6,LSL #6
add r5,r5,r6,LSL #6

# qhasm:   out5 += (out6 << 25)
# asm 1: add >out5=int32#7,<out5=int32#8,<out6=int32#9,LSL #25
# asm 2: add >out5=r6,<out5=r7,<out6=r8,LSL #25
add r6,r7,r8,LSL #25

# qhasm:   out6 unsigned>>= 7
# asm 1: mov >out6=int32#8,<out6=int32#9,LSR #7
# asm 2: mov >out6=r7,<out6=r8,LSR #7
mov r7,r8,LSR #7

# qhasm:   out6 += (out7 << 19)
# asm 1: add >out6=int32#8,<out6=int32#8,<out7=int32#10,LSL #19
# asm 2: add >out6=r7,<out6=r7,<out7=r9,LSL #19
add r7,r7,r9,LSL #19

# qhasm:   out7 unsigned>>= 13
# asm 1: mov >out7=int32#9,<out7=int32#10,LSR #13
# asm 2: mov >out7=r8,<out7=r9,LSR #13
mov r8,r9,LSR #13

# qhasm:   out7 += (out8 << 12)
# asm 1: add >out7=int32#9,<out7=int32#9,<out8=int32#11,LSL #12
# asm 2: add >out7=r8,<out7=r8,<out8=r10,LSL #12
add r8,r8,r10,LSL #12

# qhasm:   out8 unsigned>>= 20
# asm 1: mov >out8=int32#10,<out8=int32#11,LSR #20
# asm 2: mov >out8=r9,<out8=r10,LSR #20
mov r9,r10,LSR #20

# qhasm:   out8 += (out9 << 6)
# asm 1: add >out8=int32#2,<out8=int32#10,<out9=int32#2,LSL #6
# asm 2: add >out8=r1,<out8=r9,<out9=r1,LSL #6
add r1,r9,r1,LSL #6

# qhasm:   mem32[q] = out0;q += 4
# asm 1: str <out0=int32#3,[<q=int32#1],#4
# asm 2: str <out0=r2,[<q=r0],#4
str r2,[r0],#4

# qhasm:   mem32[q] = out1;q += 4
# asm 1: str <out1=int32#4,[<q=int32#1],#4
# asm 2: str <out1=r3,[<q=r0],#4
str r3,[r0],#4

# qhasm:   mem32[q] = out2;q += 4
# asm 1: str <out2=int32#5,[<q=int32#1],#4
# asm 2: str <out2=r4,[<q=r0],#4
str r4,[r0],#4

# qhasm:   mem32[q] = out3;q += 4
# asm 1: str <out3=int32#6,[<q=int32#1],#4
# asm 2: str <out3=r5,[<q=r0],#4
str r5,[r0],#4

# qhasm:   mem32[q] = out5;q += 4
# asm 1: str <out5=int32#7,[<q=int32#1],#4
# asm 2: str <out5=r6,[<q=r0],#4
str r6,[r0],#4

# qhasm:   mem32[q] = out6;q += 4
# asm 1: str <out6=int32#8,[<q=int32#1],#4
# asm 2: str <out6=r7,[<q=r0],#4
str r7,[r0],#4

# qhasm:   mem32[q] = out7;q += 4
# asm 1: str <out7=int32#9,[<q=int32#1],#4
# asm 2: str <out7=r8,[<q=r0],#4
str r8,[r0],#4

# qhasm:   mem32[q] = out8
# asm 1: str <out8=int32#2,[<q=int32#1]
# asm 2: str <out8=r1,[<q=r0]
str r1,[r0]

# qhasm: assign r4 r5 to caller_r4 caller_r5 = stack_r45
# asm 1: ldrd >caller_r4=int32#5,<stack_r45=stack64#1
# asm 2: ldrd >caller_r4=r4,<stack_r45=[sp,#0]
ldrd r4,[sp,#0]

# qhasm: assign r6 r7 to caller_r6 caller_r7 = stack_r67
# asm 1: ldrd >caller_r6=int32#7,<stack_r67=stack64#2
# asm 2: ldrd >caller_r6=r6,<stack_r67=[sp,#8]
ldrd r6,[sp,#8]

# qhasm: assign r8 r9 to caller_r8 caller_r9 = stack_r89
# asm 1: ldrd >caller_r8=int32#9,<stack_r89=stack64#3
# asm 2: ldrd >caller_r8=r8,<stack_r89=[sp,#16]
ldrd r8,[sp,#16]

# qhasm: assign r10 r11 to caller_r10 caller_r11 = stack_r1011
# asm 1: ldrd >caller_r10=int32#11,<stack_r1011=stack64#4
# asm 2: ldrd >caller_r10=r10,<stack_r1011=[sp,#24]
ldrd r10,[sp,#24]

# qhasm: caller_r12 = stack_r12
# asm 1: ldr >caller_r12=int32#13,<stack_r12=stack32#1
# asm 2: ldr >caller_r12=r12,<stack_r12=[sp,#480]
ldr r12,[sp,#480]

# qhasm: caller_r14 = stack_r14
# asm 1: ldr >caller_r14=int32#14,<stack_r14=stack32#2
# asm 2: ldr >caller_r14=r14,<stack_r14=[sp,#484]
ldr r14,[sp,#484]

# qhasm: int32 result

# qhasm: result = 0
# asm 1: ldr >result=int32#1,=0
# asm 2: ldr >result=r0,=0
ldr r0,=0

# qhasm: qpopreturn result
mov sp,r12
vpop {q4,q5,q6,q7}
bx lr
