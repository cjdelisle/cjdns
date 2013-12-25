stack3072 playground1
int32 playground1_ptr
@define const121666 playground1_ptr + 0
@define x1 playground1_ptr + 48
@define x2 playground1_ptr + 96
@define z2 playground1_ptr + 144
@define x3 playground1_ptr + 192
@define z3 playground1_ptr + 240
@define tmp0 playground1_ptr + 288
@define tmp1 playground1_ptr + 336
@define z11_copy x1

int32 i
int32 j
int32 ptr
int32 swap
int32 pos
int32 bit
int32 byte
int32 word
int32 pos8
int32 pos7
int32 mulsource
int32 postcopy

int32 q
int32 p
int32 n

reg128 e0
reg128 e4
reg128 f0
reg128 f4
reg128 f8
reg128 g0
reg128 g4
reg128 g8
reg128 d0
reg128 d4
reg128 d8
reg128 x0
reg128 x4
reg128 x8
reg128 F0
reg128 F4
reg128 F8
reg128 G0
reg128 G4
reg128 G8
reg128 X0
reg128 X4
reg128 X8
reg128 f0plusF0
reg128 f0minusF0
reg128 g0plusG0
reg128 g0minusG0
reg128 f4plusF4
reg128 f4minusF4
reg128 g4plusG4
reg128 g4minusG4
reg128 f8plusF8
reg128 f8minusF8
reg128 g8plusG8
reg128 g8minusG8
reg128 fg01
reg128 fg23
reg128 fg45
reg128 fg67
reg128 fg89

reg128 fg01_2
reg128 fg23_2
reg128 fg45_2
reg128 fg67_2

reg128 fg45_19_38
reg128 fg67_19_38
reg128 fg89_19_38

reg128 h0
reg128 h1
reg128 h2
reg128 h3
reg128 h4
reg128 h5
reg128 h6
reg128 h7
reg128 h8
reg128 h9

stack64 h0stack
stack64 h1stack
stack64 h2stack
stack64 h3stack
stack64 h4stack
stack64 h5stack
stack64 h6stack
stack64 h7stack
stack64 h8stack
stack64 h9stack

reg128 t0
reg128 t1
reg128 t2
reg128 t3
reg128 t4
reg128 t5
reg128 t6
reg128 t7
reg128 t8
reg128 t9

reg128 c0
reg128 c1
reg128 c2
reg128 c3
reg128 c4
reg128 c5
reg128 c6
reg128 c7
reg128 c8
reg128 c9

reg128 f02
reg128 f13
reg128 f46
reg128 f57
reg128 f89

reg128 g02
reg128 g13
reg128 g46
reg128 g57
reg128 g89

reg128 f13_2
reg128 f57_2
reg128 f89_2

reg128 mix
reg128 g13_19
reg128 g46_19
reg128 g57_19
reg128 g89_19

stack128 f13_2_stack
stack128 f57_2_stack
stack128 mix_stack
stack128 g13_19_stack
stack128 g46_19_stack
stack128 g57_19_stack
stack128 g89_19_stack
stack128 h9_stack
stack128 h7_stack
stack128 h5_stack

reg128 t
reg128 s
reg128 s2
reg128 c

reg128 mask26
reg128 mask25

reg128 _0x2000000
reg128 _0x1000000
reg128 _19_19_38_38
stack128 _0x2000000_stack
stack128 _0x1000000_stack
stack128 _19_19_38_38_stack

reg128 h02
reg128 h24
reg128 h46
reg128 h68
reg128 h80
reg128 h31
reg128 h53
reg128 h75
reg128 h97
reg128 h19

reg128 h04
reg128 h15
reg128 h26
reg128 h37
reg128 h48
reg128 h59

reg128 f0_f1_f2_f3
reg128 f4_f5_f6_f7
reg128 f8_f9_g8_g9

reg128 19f8_19f9_19g8_19g9

reg128 f8_2f9_g8_g9

# required for even (and partially also odd)
reg128 g0_g1_g2_g3
reg128 g4_g5_g6_g7
reg128 f0_2f1_f2_2f3
reg128 f4_2f5_f6_2f7
reg128 f8_2f9_f9_f6
reg128 g0_19g1_g2_19g3
reg128 19g0_19g1_19g2_19g3 # This one is going to be freed very early
reg128 19g4_19g5_19g6_19g7
reg128 g4_19g5_g6_19g7
reg128 g8_19g9_19g8_19g9

#required only for odd
reg128 f1_f8_f3_f0
reg128 f5_f2_f7_f4
reg128 19g8_g9_19g2_g3
reg128 19g4_g5_19g6_g7

reg128 _19_19_19_19
reg128 _0_1_0_1
reg128 _1_1_1_1


stack512 playground2
int32 playp

int32 binput
reg128 b

int32 pos0
int32 pos1
int32 pos2
int32 pos3
int32 posh
int32 posf
int32 posg
int32 posH
int32 posF
int32 posG
int32 posx
int32 posy

int32 out0
int32 out1
int32 out2
int32 out3
int32 out4
int32 out5
int32 out6
int32 out7
int32 out8
int32 out9
int32 carry0
int32 carry1
int32 carry2
int32 carry3
int32 carry4
int32 carry5
int32 carry6
int32 carry7
int32 carry8
int32 carry9
int32 carry

reg128 zero
reg128 one

@define fe_0(x) ;\
posx = x ;\
4x zero = 0 ;\
mem128[posx] aligned= zero; posx += 16 ;\
mem128[posx] aligned= zero; posx += 16 ;\
mem64[posx] aligned= zero[0] ;\

@define fe_1(x) ;\
posx = x ;\
4x zero = 0 ;\
new one ;\
one = 0xff,one[1] ;\
one = one[0],0 ;\
4x one unsigned>>= 7 ;\
mem128[posx] aligned= one; posx += 16 ;\
mem128[posx] aligned= zero; posx += 16 ;\
mem64[posx] aligned= zero[0] ;\

@define fe_copy(x,y) ;\
posy = y ;\
posx = x ;\
f0 aligned= mem128[posy]; posy += 16 ;\
f4 aligned= mem128[posy]; posy += 16 ;\
new f8 ;\
f8 aligned= mem64[posy] f8[1] ;\
mem128[posx] aligned= f0; posx += 16 ;\
mem128[posx] aligned= f4; posx += 16 ;\
mem64[posx] aligned= f8[0] ;\

@define fe_add(sum,x,y) ;\
  pos1 = x ;\
  pos2 = y ;\
f0 aligned= mem128[pos1]; pos1 += 16 ;\
g0 aligned= mem128[pos2]; pos2 += 16 ;\
4x f0 += g0 ;\
 ;\
f4 aligned= mem128[pos1]; pos1 += 16 ;\
g4 aligned= mem128[pos2]; pos2 += 16 ;\
4x f4 += g4 ;\
  pos0 = sum ;\
 ;\
new f8 ;\
new g8 ;\
f8 aligned= mem64[pos1] f8[1] ;\
g8 aligned= mem64[pos2] g8[1] ;\
4x f8 += g8 ;\
 ;\
mem128[pos0] aligned= f0; pos0 += 16 ;\
mem128[pos0] aligned= f4; pos0 += 16 ;\
mem64[pos0] aligned= f8[0] ;\


@define fe_sub(diff,x,y) ;\
  pos1 = x ;\
  pos2 = y ;\
f0 aligned= mem128[pos1]; pos1 += 16 ;\
g0 aligned= mem128[pos2]; pos2 += 16 ;\
4x f0 -= g0 ;\
 ;\
f4 aligned= mem128[pos1]; pos1 += 16 ;\
g4 aligned= mem128[pos2]; pos2 += 16 ;\
4x f4 -= g4 ;\
  pos0 = diff ;\
 ;\
new f8 ;\
new g8 ;\
f8 aligned= mem64[pos1] f8[1] ;\
g8 aligned= mem64[pos2] g8[1] ;\
4x f8 -= g8 ;\
 ;\
mem128[pos0] aligned= f0; pos0 += 16 ;\
mem128[pos0] aligned= f4; pos0 += 16 ;\
mem64[pos0] aligned= f8[0] ;\


@define fe_addsub(sum,x,y,diff) ;\
pos1 = x ;\
pos2 = y ;\
pos3 = diff ;\
pos0 = sum ;\
f0 aligned= mem128[pos1]; pos1 += 16 ;\
 ;\
g0 aligned= mem128[pos2]; pos2 += 16 ;\
4x d0 = f0 - g0 ;\
 ;\
4x f0 += g0 ;\
f4 aligned= mem128[pos1]; pos1 += 16 ;\
 ;\
g4 aligned= mem128[pos2]; pos2 += 16 ;\
4x d4 = f4 - g4 ;\
 ;\
4x f4 += g4 ;\
new f8 ;\
f8 aligned= mem64[pos1] f8[1] ;\
 ;\
new g8 ;\
g8 aligned= mem64[pos2] g8[1] ;\
4x d8 = f8 - g8 ;\
 ;\
4x f8 += g8 ;\
mem128[pos3] aligned= d0; pos3 += 16 ;\
 ;\
mem128[pos0] aligned= f0; pos0 += 16 ;\
 ;\
mem128[pos3] aligned= d4; pos3 += 16 ;\
 ;\
mem128[pos0] aligned= f4; pos0 += 16 ;\
 ;\
mem64[pos3] aligned= d8[0] ;\
 ;\
mem64[pos0] aligned= f8[0] ;\


@define fe_negcswap2addsub(x2,x3,z2,z3,swap) ;\
                new f8 ;\
                new g8 ;\
                new F8 ;\
                new G8 ;\
pos0 = x2 ;\
pos1 = x3 ;\
pos2 = z2 ;\
f0 aligned= mem128[pos0]; pos0 += 16 ;\
pos3 = z3 ;\
g0 aligned= mem128[pos1]; pos1 += 16 ;\
x0 = f0 ^ g0 ;\
F0 aligned= mem128[pos2]; pos2 += 16 ;\
b = swap,swap,swap,swap ;\
G0 aligned= mem128[pos3]; pos3 += 16 ;\
X0 = F0 ^ G0 ;\
        f4 aligned= mem128[pos0]; pos0 += 16 ;\
x0 &= b ;\
        g4 aligned= mem128[pos1]; pos1 += 16 ;\
X0 &= b ;\
        F4 aligned= mem128[pos2]; pos2 += 16 ;\
f0 ^= x0 ;\
        G4 aligned= mem128[pos3]; pos3 += 16 ;\
g0 ^= x0 ;\
                f8 aligned= mem64[pos0] f8[1] ;\
F0 ^= X0 ;\
                g8 aligned= mem64[pos1] g8[1] ;\
G0 ^= X0 ;\
                F8 aligned= mem64[pos2] F8[1] ;\
        x4 = f4 ^ g4 ;\
                G8 aligned= mem64[pos3] G8[1] ;\
                x8 = f8 ^ g8 ;\
                                                pos0 -= 32 ;\
        x4 &= b ;\
                                                pos1 -= 32 ;\
                x8 &= b ;\
                                                pos2 -= 32 ;\
        f4 ^= x4 ;\
                                                pos3 -= 32 ;\
                f8 ^= x8 ;\
        g4 ^= x4 ;\
                g8 ^= x8 ;\
        X4 = F4 ^ G4 ;\
                X8 = F8 ^ G8 ;\
        X4 &= b ;\
                X8 &= b ;\
        F4 ^= X4 ;\
                F8 ^= X8 ;\
        G4 ^= X4 ;\
                G8 ^= X8 ;\
4x f0plusF0 = f0 + F0 ;\
4x f0minusF0 = f0 - F0 ;\
mem128[pos0] aligned= f0plusF0; pos0 += 16 ;\
	4x f4plusF4 = f4 + F4 ;\
mem128[pos2] aligned= f0minusF0; pos2 += 16 ;\
	4x f4minusF4 = f4 - F4 ;\
        mem128[pos0] aligned= f4plusF4; pos0 += 16 ;\
		4x f8plusF8 = f8 + F8 ;\
        mem128[pos2] aligned= f4minusF4; pos2 += 16 ;\
		4x f8minusF8 = f8 - F8 ;\
                mem64[pos0] aligned= f8plusF8[0] ;\
4x g0plusG0 = g0 + G0 ;\
                mem64[pos2] aligned= f8minusF8[0] ;\
4x g0minusG0 = g0 - G0 ;\
mem128[pos1] aligned= g0plusG0; pos1 += 16 ;\
	4x g4plusG4 = g4 + G4 ;\
mem128[pos3] aligned= g0minusG0; pos3 += 16 ;\
	4x g4minusG4 = g4 - G4 ;\
        mem128[pos1] aligned= g4plusG4; pos1 += 16 ;\
		4x g8plusG8 = g8 + G8 ;\
        mem128[pos3] aligned= g4minusG4; pos3 += 16 ;\
		4x g8minusG8 = g8 - G8 ;\
                mem64[pos1] aligned= g8plusG8[0] ;\
                mem64[pos3] aligned= g8minusG8[0] ;\


@define fe_sqsq(h,f,H,F) ;\
ptr = &_19_19_38_38_stack ;\
  posf = f ;\
  posF = F ;\
_19_19_38_38 aligned= mem128[ptr] ;\
 ;\
fg01 aligned= mem128[posf];posf+=16 ;\
fg23 aligned= mem128[posF];posF+=16 ;\
fg01[0,1,2,3] fg23[0,1,2,3] = fg01[0]fg23[0]fg01[1]fg23[1] fg01[2]fg23[2]fg01[3]fg23[3] ;\
 ;\
fg45 aligned= mem128[posf];posf+=16 ;\
fg67 aligned= mem128[posF];posF+=16 ;\
 ;\
4x fg01_2 = fg01 << 1 ;\
fg45[0,1,2,3] fg67[0,1,2,3] = fg45[0]fg67[0]fg45[1]fg67[1] fg45[2]fg67[2]fg45[3]fg67[3] ;\
4x fg23_2 = fg23 << 1 ;\
new fg89 ;\
fg89 aligned= mem64[posf]fg89[1] ;\
4x fg45_2 = fg45 << 1 ;\
fg89 aligned= fg89[0]mem64[posF] ;\
4x fg67_2 = fg67 << 1 ;\
;\
fg45_19_38[0,1] = fg45_19_38[0,1];fg45_19_38[2] = fg45[2] * _19_19_38_38[2];fg45_19_38[3] = fg45[3] * _19_19_38_38[3] ;\
fg89 = fg89[0,2,1,3] ;\
4x fg67_19_38 = fg67 * _19_19_38_38 ;\
4x fg89_19_38 = fg89 * _19_19_38_38 ;\
 ;\
# f0f0 + f1f9_76 + f2f8_38 + f3f7_76 + f4f6_38 + f5f5_38; ;\
h0[0,1]  = fg01[0]   signed* fg01[0];       h0[2,3] = fg01[1] signed* fg01[1] ;\
h0[0,1] += fg01_2[2] signed* fg89_19_38[2]; h0[2,3] += fg01_2[3] signed* fg89_19_38[3] ;\
h0[0,1] += fg23_2[0] signed* fg89_19_38[0]; h0[2,3] += fg23_2[1] signed* fg89_19_38[1] ;\
h0[0,1] += fg23_2[2] signed* fg67_19_38[2]; h0[2,3] += fg23_2[3] signed* fg67_19_38[3] ;\
h0[0,1] += fg45_2[0] signed* fg67_19_38[0]; h0[2,3] += fg45_2[1] signed* fg67_19_38[1] ;\
h0[0,1] += fg45[2] signed* fg45_19_38[2]; h0[2,3] += fg45[3] signed* fg45_19_38[3] ;\
 ;\
# f0f1_2 + f2f9_38 + f3f8_38 + f4f7_38 + f5f6_38; ;\
h1[0,1]  = fg01[0]   signed* fg01_2[2];     h1[2,3]  = fg01[1]   signed* fg01_2[3] ;\
h1[0,1] += fg23[0]   signed* fg89_19_38[2]; h1[2,3] += fg23[1]   signed* fg89_19_38[3]  ;\
h1[0,1] += fg23_2[2] signed* fg89_19_38[0]; h1[2,3] += fg23_2[3] signed* fg89_19_38[1] ;\
h1[0,1] += fg45[0]   signed* fg67_19_38[2]; h1[2,3] += fg45[1]   signed* fg67_19_38[3] ;\
h1[0,1] += fg45_2[2] signed* fg67_19_38[0]; h1[2,3] += fg45_2[3] signed* fg67_19_38[1] ;\
 ;\
# f0f2_2 + f1f1_2 + f3f9_76 + f4f8_38 + f5f7_76 + f6f6_19 ;\
h2[0,1]  = fg01_2[0] signed* fg23[0];       h2[2,3]  = fg01_2[1] signed* fg23[1] ;\
h2[0,1] += fg01_2[2] signed* fg01[2];       h2[2,3] += fg01_2[3] signed* fg01[3] ;\
h2[0,1] += fg23_2[2] signed* fg89_19_38[2]; h2[2,3] += fg23_2[3] signed* fg89_19_38[3] ;\
h2[0,1] += fg45_2[0] signed* fg89_19_38[0]; h2[2,3] += fg45_2[1] signed* fg89_19_38[1] ;\
h2[0,1] += fg45_2[2] signed* fg67_19_38[2]; h2[2,3] += fg45_2[3] signed* fg67_19_38[3] ;\
h2[0,1] += fg67[0]   signed* fg67_19_38[0]; h2[2,3] += fg67[1]   signed* fg67_19_38[1] ;\
 ;\
# f0f3_2+f1f2_2 +f4f9_38+f5f8_38+f6f7_38; ;\
h3[0,1]  = fg01_2[0] signed* fg23[2];       h3[2,3]  = fg01_2[1] signed* fg23[3] ;\
h3[0,1] += fg01_2[2] signed* fg23[0];       h3[2,3] += fg01_2[3] signed* fg23[1] ;\
h3[0,1] += fg45[0]   signed* fg89_19_38[2]; h3[2,3] += fg45[1]   signed* fg89_19_38[3] ;\
h3[0,1] += fg45_2[2] signed* fg89_19_38[0]; h3[2,3] += fg45_2[3] signed* fg89_19_38[1] ;\
h3[0,1] += fg67[0]   signed* fg67_19_38[2]; h3[2,3] += fg67[1]   signed* fg67_19_38[3] ;\
 ;\
# f0f4_2 + f1f3_4 + f2f2   + f5f9_76 + f6f8_38 + f7f7_38; ;\
h4[0,1]  = fg01_2[0] signed* fg45[0];       h4[2,3]  = fg01_2[1] signed* fg45[1] ;\
h4[0,1] += fg01_2[2] signed* fg23_2[2];     h4[2,3] += fg01_2[3] signed* fg23_2[3] ;\
h4[0,1] += fg23[0]   signed* fg23[0];       h4[2,3] += fg23[1]   signed* fg23[1] ;\
h4[0,1] += fg45_2[2] signed* fg89_19_38[2]; h4[2,3] += fg45_2[3] signed* fg89_19_38[3] ;\
h4[0,1] += fg67_2[0] signed* fg89_19_38[0]; h4[2,3] += fg67_2[1] signed* fg89_19_38[1] ;\
h4[0,1] += fg67[2]   signed* fg67_19_38[2]; h4[2,3] += fg67[3]   signed* fg67_19_38[3] ;\
 ;\
# f0f5_2+f1f4_2 +f2f3_2 +f6f9_38+f7f8_38; ;\
h5[0,1]  = fg01_2[0] signed* fg45[2];       h5[2,3]  = fg01_2[1] signed* fg45[3] ;\
h5[0,1] += fg01_2[2] signed* fg45[0];       h5[2,3] += fg01_2[3] signed* fg45[1] ;\
h5[0,1] += fg23_2[0] signed* fg23[2];       h5[2,3] += fg23_2[1] signed* fg23[3] ;\
h5[0,1] += fg67[0]   signed* fg89_19_38[2]; h5[2,3] += fg67[1]   signed* fg89_19_38[3]  ;\
h5[0,1] += fg67_2[2] signed* fg89_19_38[0]; h5[2,3] += fg67_2[3] signed* fg89_19_38[1] ;\
 ;\
# f0f6_2+f1f5_4 +f2f4_2 +f3f3_2 +f7f9_76+f8f8_19; ;\
h6[0,1]  = fg01_2[0] signed* fg67[0];       h6[2,3]  = fg01_2[1] signed* fg67[1] ;\
h6[0,1] += fg01_2[2] signed* fg45_2[2];     h6[2,3] += fg01_2[3] signed* fg45_2[3] ;\
h6[0,1] += fg23_2[0] signed* fg45[0];       h6[2,3] += fg23_2[1] signed* fg45[1] ;\
h6[0,1] += fg23_2[2] signed* fg23[2];       h6[2,3] += fg23_2[3] signed* fg23[3] ;\
h6[0,1] += fg67_2[2] signed* fg89_19_38[2]; h6[2,3] += fg67_2[3] signed* fg89_19_38[3] ;\
h6[0,1] += fg89[0]  signed* fg89_19_38[0]; h6[2,3] += fg89[1]  signed* fg89_19_38[1] ;\
 ;\
# f0f7_2+f1f6_2 +f2f5_2 +f3f4_2 +f8f9_38; ;\
h7[0,1]  = fg01_2[0] signed* fg67[2];       h7[2,3]  = fg01_2[1] signed* fg67[3] ;\
h7[0,1] += fg01_2[2] signed* fg67[0];       h7[2,3] += fg01_2[3] signed* fg67[1] ;\
h7[0,1] += fg23_2[0] signed* fg45[2];       h7[2,3] += fg23_2[1] signed* fg45[3] ;\
h7[0,1] += fg23_2[2] signed* fg45[0];       h7[2,3] += fg23_2[3] signed* fg45[1] ;\
h7[0,1] += fg89[0]   signed* fg89_19_38[2]; h7[2,3] += fg89[1]   signed* fg89_19_38[3] ;\
 ;\
# f0f8_2+f1f7_4 +f2f6_2 +f3f5_4 +f4f4   +f9f9_38; ;\
h8[0,1]  = fg89[2]   signed* fg89_19_38[2]; h8[2,3]  = fg89[3]   signed* fg89_19_38[3]  ;\
h8[0,1] += fg01_2[0] signed* fg89[0];       h8[2,3] += fg01_2[1] signed* fg89[1]   ;\
h8[0,1] += fg01_2[2] signed* fg67_2[2];     h8[2,3] += fg01_2[3] signed* fg67_2[3] ;\
h8[0,1] += fg23_2[0] signed* fg67[0];       h8[2,3] += fg23_2[1] signed* fg67[1] ;\
h8[0,1] += fg23_2[2] signed* fg45_2[2];     h8[2,3] += fg23_2[3] signed* fg45_2[3] ;\
h8[0,1] += fg45[0]   signed* fg45[0];       h8[2,3] += fg45[1]   signed* fg45[1] ;\
 ;\
ptr = &_0x2000000_stack ;\
_0x2000000 aligned= mem128[ptr] ;\
# f0f9_2+f1f8_2 +f2f7_2 +f3f6_2 +f4f5_2; ;\
h9[0,1]  = fg45_2[0] signed* fg45[2];       h9[2,3]  = fg45_2[1] signed* fg45[3] ;\
h9[0,1] += fg01_2[0] signed* fg89[2];       h9[2,3] += fg01_2[1] signed* fg89[3] ;\
h9[0,1] += fg01_2[2] signed* fg89[0];       h9[2,3] += fg01_2[3] signed* fg89[1] ;\
h9[0,1] += fg23_2[0] signed* fg67[2];       h9[2,3] += fg23_2[1] signed* fg67[3] ;\
h9[0,1] += fg23_2[2] signed* fg67[0];       h9[2,3] += fg23_2[3] signed* fg67[1] ;\
;\
ptr = &_0x1000000_stack ;\
_0x1000000 aligned= mem128[ptr] ;\
2x t0   = h0 + _0x2000000 ;\
				2x t6   = h6 + _0x2000000 ;\
;\
2x c0   = t0 signed>> 26 ;\
				2x c6   = t6 signed>> 26 ;\
2x h1 += c0 ;\
2x t0 = c0 << 26 ;\
	2x t1   = h1 + _0x1000000 ;\
				2x h7 += c6 ;\
				2x t6 = c6 << 26 ;\
					2x t7   = h7 + _0x1000000 ;\
2x h0 -= t0 ;\
	2x c1   = t1 signed>> 25 ;\
				2x h6 -= t6 ;\
					2x c7   = t7 signed>> 25 ;\
	2x h2 += c1 ;\
	2x t1 = c1 << 25 ;\
2x t2   = h2 + _0x2000000 ;\
					2x h8 += c7 ;\
	2x h1 -= t1 ;\
2x c2   = t2 signed>> 26 ;\
					2x t7 = c7 << 25 ;\
				2x t8   = h8 + _0x2000000 ;\
2x h3 += c2 ;\
2x t2 = c2 << 26 ;\
	2x t3   = h3 + _0x1000000 ;\
					2x h7 -= t7 ;\
				2x c8   = t8 signed>> 26 ;\
2x h2 -= t2 ;\
	2x c3   = t3 signed>> 25 ;\
				2x h9 += c8 ;\
				2x t8 = c8 << 26 ;\
					2x t9   = h9 + _0x1000000 ;\
	2x h4 += c3 ;\
  posh = h ;\
	2x t3 = c3 << 25 ;\
  posH = H ;\
2x t4   = h4 + _0x2000000 ;\
		posh+=8;\
				2x h8 -= t8 ;\
		posH+=8;\
					2x c9   = t9 signed>> 25 ;\
	2x h3 -= t3 ;\
2x c4   = t4 signed>> 26 ;\
					2x s   = c9 + c9 ;\
2x h5 += c4 ;\
		h2 h3 = h2[0]h3[0]h2[2]h2[3] h2[1]h3[1]h3[2]h3[3];\
2x t4 = c4 << 26 ;\
		h2 h3 = h2[0]h2[1]h2[2]h3[2] h3[0]h3[1]h2[3]h3[3];\
	2x t5   = h5 + _0x1000000 ;\
					2x h0 += s ;\
		mem64[posh] aligned= h2[0];posh+=8 ;\
					2x s   = c9 << 4 ;\
		mem64[posH] aligned= h2[1];posH+=8 ;\
2x h4 -= t4 ;\
	2x c5   = t5 signed>> 25 ;\
					2x h0 += s ;\
	2x h6 += c5 ;\
	2x t5 = c5 << 25 ;\
2x t6   = h6 + _0x2000000 ;\
					2x h0 += c9 ;\
					2x t9 = c9 << 25 ;\
				2x t0   = h0 + _0x2000000 ;\
	2x h5 -= t5 ;\
2x c6   = t6 signed>> 26 ;\
					2x h9 -= t9 ;\
		h4 h5 = h4[0]h5[0]h4[2]h4[3] h4[1]h5[1]h5[2]h5[3];\
				2x c0   = t0 signed>> 26 ;\
		h4 h5 = h4[0]h4[1]h4[2]h5[2] h5[0]h5[1]h4[3]h5[3];\
2x h7 += c6 ;\
		mem64[posh] aligned= h4[0] ;\
2x t6 = c6 << 26 ;\
		mem64[posH] aligned= h4[1] ;\
				2x h1 += c0 ;\
		h8 h9 = h8[0]h9[0]h8[2]h8[3] h8[1]h9[1]h9[2]h9[3];\
				2x t0 = c0 << 26 ;\
		h8 h9 = h8[0]h8[1]h8[2]h9[2] h9[0]h9[1]h8[3]h9[3];\
2x h6 -= t6 ;\
		posh+=16;\
				2x h0 -= t0 ;\
		mem64[posh] aligned= h8[0] ;\
		posH+=16;\
		mem64[posH] aligned= h8[1] ;\
;\
		h6 h7 = h6[0]h7[0]h6[2]h6[3] h6[1]h7[1]h7[2]h7[3];\
		h6 h7 = h6[0]h6[1]h6[2]h7[2] h7[0]h7[1]h6[3]h7[3];\
		posh-=8;\
		posH-=8;\
;\
		h0 h1 = h0[0]h1[0]h0[2]h0[3] h0[1]h1[1]h1[2]h1[3];\
		h0 h1 = h0[0]h0[1]h0[2]h1[2] h1[0]h1[1]h0[3]h1[3];\
;\
		mem64[posh] aligned= h6[0] ;\
		mem64[posH] aligned= h6[1] ;\
		posh-=24;\
		posH-=24;\
		mem64[posh] aligned= h0[0] ;\
		mem64[posH] aligned= h0[1] ;\


@define fe_mulmul(h,f,g,H,F,G) ;\
posf = f ;\
posg = g ;\
 ;\
g02 aligned= mem128[posg]; posg += 16 # g0 g1 g2 g3 ;\
 ;\
g46 aligned= mem128[posg]; posg += 16 # g4 g5 g6 g7 ;\
 ;\
new g89 ;\
g89 aligned= mem64[posg] g89[1]          # g8 g9 ? ? ;\
 ;\
posG = G ;\
g13 aligned= mem128[posG]; posG += 16 # G0 G1 G2 G3 ;\
 ;\
g02 g13 = g02[0]g13[0] g02[2]g13[2] g02[1]g13[1] g02[3]g13[3] # g0 G0 g2 G2 g1 G1 g3 G3 ;\
 ;\
g57 aligned= mem128[posG]; posG += 16 # G4 G5 G6 G7 ;\
 ;\
4x mix = g02 << 4 ;\
g46 g57 = g46[0]g57[0] g46[2]g57[2] g46[1]g57[1] g46[3]g57[3] # g4 G4 g6 G6 g5 G5 g7 G7 ;\
 ;\
	4x g13_19 = g13 << 4 ;\
4x mix += g02 ;\
	4x g13_19 += g13 ;\
	4x g46_19 = g46 << 4 ;\
g89 aligned= g89[0] mem64[posG]       # g8 g9 G8 G9 ;\
	4x g57_19 = g57 << 4 ;\
g89 = g89[0] g89[2] g89[1] g89[3]     # g8 G8 g9 G9 ;\
	4x g46_19 += g46 ;\
	4x g57_19 += g57 ;\
				f02 aligned= mem128[posf]; posf += 16 # f0 f1 f2 f3 ;\
	4x g89_19 = g89 << 4 ;\
				f46 aligned= mem128[posf]; posf += 16 # f4 f5 f6 f7 ;\
	4x g89_19 += g89 ;\
				new f89 ;\
				f89 aligned= mem64[posf] f89[1]          # f8 f9 ? ? ;\
4x mix += g02 ;\
posF = F ;\
				f13 aligned= mem128[posF]; posF += 16 # F0 F1 F2 F3 ;\
	4x g13_19 += g13 ;\
				f57 aligned= mem128[posF]; posF += 16 # F4 F5 F6 F7 ;\
	4x g57_19 += g57 ;\
				f89 aligned= f89[0] mem64[posF]       # f8 f9 F8 F9 ;\
	4x g89_19 += g89 ;\
				f02 f13 = f02[0]f13[0] f02[2]f13[2] f02[1]f13[1] f02[3]f13[3] # f0 F0 f2 F2 f1 F1 f3 F3 ;\
	4x g46_19 += g46 ;\
				 ;\
4x mix += g02 # 19g0 19G0 19g2 19G2 ;\
				f46 f57 = f46[0]f57[0] f46[2]f57[2] f46[1]f57[1] f46[3]f57[3] # f4 F4 f6 F6 f5 F5 f7 F7 ;\
	4x g13_19 += g13 # 19g1 19G1 19g3 19G3 ;\
new g13_19_stack ;\
ptr = &g13_19_stack ;\
	4x g89_19 += g89 # 19g8 19G8 19g9 19G9 ;\
				f89 = f89[0] f89[2] f89[1] f89[3]     # f8 F8 f9 F9 ;\
 ;\
mem128[ptr] aligned= g13_19 ;\
				4x f13_2 = f13 << 1 # 2f1 2F1 2f3 2F3 ;\
new g89_19_stack ;\
ptr = &g89_19_stack ;\
mem128[ptr] aligned= g89_19 ;\
				4x f57_2 = f57 << 1 # 2f5 2F5 2f7 2F7 ;\
 ;\
				new f13_2_stack ;\
				ptr = &f13_2_stack ;\
				mem128[ptr] aligned= f13_2 ;\
 ;\
				4x f89_2 = f89 << 1 # 2f8 2F8 2f9 2F9 ;\
	4x g57_19 += g57 # 19g5 19G5 19g7 19G7 ;\
				mix = f89_2[2,3] mix[2,3] # 2f9 2F9 19g2 19G2 ;\
 ;\
	4x g46_19 += g46 # 19g4 19G4 19g6 19G6 ;\
 ;\
new g57_19_stack ;\
ptr = &g57_19_stack ;\
mem128[ptr] aligned= g57_19 ;\
 ;\
# h9 = f0g9+f1g8   +f2g7   +f3g6   +f4g5   +f5g4   +f6g3   +f7g2   +f8g1   +f9g0 ;\
h9[0,1]  = f02[0] signed* g89[2]; h9[2,3]  = f02[1] signed* g89[3] ;\
h9[0,1] += f13[0] signed* g89[0]; h9[2,3] += f13[1] signed* g89[1] ;\
h9[0,1] += f02[2] signed* g57[2]; h9[2,3] += f02[3] signed* g57[3] ;\
h9[0,1] += f13[2] signed* g46[2]; h9[2,3] += f13[3] signed* g46[3] ;\
h9[0,1] += f46[0] signed* g57[0]; h9[2,3] += f46[1] signed* g57[1] ;\
h9[0,1] += f57[0] signed* g46[0]; h9[2,3] += f57[1] signed* g46[1] ;\
h9[0,1] += f46[2] signed* g13[2]; h9[2,3] += f46[3] signed* g13[3] ;\
h9[0,1] += f57[2] signed* g02[2]; h9[2,3] += f57[3] signed* g02[3] ;\
h9[0,1] += f89[0] signed* g13[0]; h9[2,3] += f89[1] signed* g13[1] ;\
h9[0,1] += f89[2] signed* g02[0]; h9[2,3] += f89[3] signed* g02[1] ;\
 ;\
new g46_19_stack ;\
ptr = &g46_19_stack ;\
mem128[ptr] aligned= g46_19 ;\
 ;\
# h8 = f0g8+f1g7_2 +f2g6   +f3g5_2 +f4g4   +f5g3_2 +f6g2   +f7g1_2 +f8g0 +f9g9_38 ;\
h8[0,1]  = f02[0]   signed* g89[0];    h8[2,3]  = f02[1]   signed* g89[1] ;\
h8[0,1] += f13_2[0] signed* g57[2];    h8[2,3] += f13_2[1] signed* g57[3] ;\
h8[0,1] += f13_2[2] signed* g57[0];    h8[2,3] += f13_2[3] signed* g57[1] ;\
h8[0,1] += f02[2]   signed* g46[2];    h8[2,3] += f02[3]   signed* g46[3] ;\
h8[0,1] += f46[0]   signed* g46[0];    h8[2,3] += f46[1]   signed* g46[1] ;\
h8[0,1] += f46[2]   signed* g02[2];    h8[2,3] += f46[3]   signed* g02[3] ;\
h8[0,1] += f89[0]   signed* g02[0];    h8[2,3] += f89[1]   signed* g02[1] ;\
 ;\
				new f57_2_stack ;\
				ptr = &f57_2_stack ;\
				mem128[ptr] aligned= f57_2 ;\
				 ;\
# h7 = f0g7+f1g6   +f2g5   +f3g4   +f4g3   +f5g2   +f6g1   +f7g0 +f8g9_19+f9g8_19 ;\
h7[0,1]  = f02[0] signed* g57[2];    h7[2,3]  = f02[1] signed* g57[3] ;\
h7[0,1] += f13[0] signed* g46[2];    h7[2,3] += f13[1] signed* g46[3] ;\
h7[0,1] += f02[2] signed* g57[0];    h7[2,3] += f02[3] signed* g57[1] ;\
h7[0,1] += f13[2] signed* g46[0];    h7[2,3] += f13[3] signed* g46[1] ;\
h7[0,1] += f46[0] signed* g13[2];    h7[2,3] += f46[1] signed* g13[3] ;\
h7[0,1] += f57[0] signed* g02[2];    h7[2,3] += f57[1] signed* g02[3] ;\
h7[0,1] += f46[2] signed* g13[0];    h7[2,3] += f46[3] signed* g13[1] ;\
h7[0,1] += f57[2] signed* g02[0];    h7[2,3] += f57[3] signed* g02[1] ;\
 ;\
				new mix_stack ;\
				ptr = &mix_stack ;\
				mem128[ptr] aligned= mix ;\
 ;\
# h6 = f0g6+f1g5_2 +f2g4   +f3g3_2 +f4g2   +f5g1_2 +f6g0 +f7g9_38+f8g8_19+f9g7_38 ;\
h6[0,1]  = f02[0]   signed* g46[2];    h6[2,3]  = f02[1]   signed* g46[3] ;\
h6[0,1] += f02[2]   signed* g46[0];    h6[2,3] += f02[3]   signed* g46[1] ;\
h6[0,1] += f46[0]   signed* g02[2];    h6[2,3] += f46[1]   signed* g02[3] ;\
h6[0,1] += f46[2]   signed* g02[0];    h6[2,3] += f46[3]   signed* g02[1] ;\
h6[0,1] += f13_2[0] signed* g57[0];    h6[2,3] += f13_2[1] signed* g57[1] ;\
 ;\
new h9_stack ;\
ptr = &h9_stack ;\
mem128[ptr] aligned= h9 ;\
 ;\
# h5 = f0g5+f1g4   +f2g3   +f3g2   +f4g1   +f5g0 +f6g9_19+f7g8_19+f8g7_19+f9g6_19 ;\
h5[0,1]  = f02[0] signed* g57[0];    h5[2,3]  = f02[1] signed* g57[1] ;\
h5[0,1] += f13[0] signed* g46[0];    h5[2,3] += f13[1] signed* g46[1] ;\
h5[0,1] += f02[2] signed* g13[2];    h5[2,3] += f02[3] signed* g13[3] ;\
h5[0,1] += f13[2] signed* g02[2];    h5[2,3] += f13[3] signed* g02[3] ;\
h5[0,1] += f46[0] signed* g13[0];    h5[2,3] += f46[1] signed* g13[1] ;\
h5[0,1] += f57[0] signed* g02[0];    h5[2,3] += f57[1] signed* g02[1] ;\
 ;\
# h3 = f0g3+f1g2   +f2g1   +f3g0 +f4g9_19+f5g8_19+f6g7_19+f7g6_19+f8g5_19+f9g4_19 ;\
h3[0,1]  = f02[0] signed* g13[2];    h3[2,3]  = f02[1] signed* g13[3] ;\
h3[0,1] += f13[0] signed* g02[2];    h3[2,3] += f13[1] signed* g02[3] ;\
h3[0,1] += f02[2] signed* g13[0];    h3[2,3] += f02[3] signed* g13[1] ;\
h3[0,1] += f13[2] signed* g02[0];    h3[2,3] += f13[3] signed* g02[1] ;\
 ;\
	ptr = &g89_19_stack ;\
	g89_19 aligned= mem128[ptr] ;\
 ;\
h7[0,1] += f89[0] signed* g89_19[2]; h7[2,3] += f89[1] signed* g89_19[3] ;\
h7[0,1] += f89[2] signed* g89_19[0]; h7[2,3] += f89[3] signed* g89_19[1] ;\
h5[0,1] += f46[2] signed* g89_19[2]; h5[2,3] += f46[3] signed* g89_19[3] ;\
h5[0,1] += f57[2] signed* g89_19[0]; h5[2,3] += f57[3] signed* g89_19[1] ;\
h3[0,1] += f46[0] signed* g89_19[2]; h3[2,3] += f46[1] signed* g89_19[3] ;\
h3[0,1] += f57[0] signed* g89_19[0]; h3[2,3] += f57[1] signed* g89_19[1] ;\
h6[0,1] += f89[0]   signed* g89_19[0]; h6[2,3] += f89[1]   signed* g89_19[1] ;\
 ;\
new h7_stack ;\
ptr = &h7_stack ;\
mem128[ptr] aligned= h7 ;\
 ;\
 ;\
# h1 = f0g1+f1g0 +f2g9_19+f3g8_19+f4g7_19+f5g6_19+f6g5_19+f7g4_19+f8g3_19+f9g2_19 ;\
h1[0,1]  = f02[0] signed* g13[0];    h1[2,3]  = f02[1] signed* g13[1] ;\
h1[0,1] += f13[0] signed* g02[0];    h1[2,3] += f13[1] signed* g02[1] ;\
 ;\
	ptr = &mix_stack ;\
	mix aligned= mem128[ptr] ;\
 ;\
h8[0,1] += mix[0]   signed* g89_19[2]; h8[2,3] += mix[1]   signed* g89_19[3] ;\
h1[0,1] += f02[2] signed* g89_19[2]; h1[2,3] += f02[3] signed* g89_19[3] ;\
h1[0,1] += f13[2] signed* g89_19[0]; h1[2,3] += f13[3] signed* g89_19[1] ;\
 ;\
	ptr = &g46_19_stack ;\
	g46_19 aligned= mem128[ptr] ;\
 ;\
h5[0,1] += f89[2] signed* g46_19[2]; h5[2,3] += f89[3] signed* g46_19[3] ;\
h3[0,1] += f57[2] signed* g46_19[2]; h3[2,3] += f57[3] signed* g46_19[3] ;\
h3[0,1] += f89[2] signed* g46_19[0]; h3[2,3] += f89[3] signed* g46_19[1] ;\
h1[0,1] += f57[0] signed* g46_19[2]; h1[2,3] += f57[1] signed* g46_19[3] ;\
h1[0,1] += f57[2] signed* g46_19[0]; h1[2,3] += f57[3] signed* g46_19[1] ;\
 ;\
	ptr = &g57_19_stack ;\
	g57_19 aligned= mem128[ptr] ;\
 ;\
h5[0,1] += f89[0] signed* g57_19[2]; h5[2,3] += f89[1] signed* g57_19[3] ;\
h3[0,1] += f46[2] signed* g57_19[2]; h3[2,3] += f46[3] signed* g57_19[3] ;\
h3[0,1] += f89[0] signed* g57_19[0]; h3[2,3] += f89[1] signed* g57_19[1] ;\
h1[0,1] += f46[0] signed* g57_19[2]; h1[2,3] += f46[1] signed* g57_19[3] ;\
h1[0,1] += f46[2] signed* g57_19[0]; h1[2,3] += f46[3] signed* g57_19[1] ;\
 ;\
new h5_stack ;\
ptr = &h5_stack ;\
mem128[ptr] aligned= h5 ;\
 ;\
 ;\
 ;\
	ptr = &g13_19_stack ;\
	g13_19 aligned= mem128[ptr] ;\
h1[0,1] += f89[0] signed* g13_19[2]; h1[2,3] += f89[1] signed* g13_19[3] ;\
h1[0,1] += f89[2] signed* mix[2];    h1[2,3] += f89[3] signed* mix[3] ;\
 ;\
 ;\
# h4 = f0g4+f1g3_2 +f2g2   +f3g1_2 +f4g0 +f5g9_38+f6g8_19+f7g7_38+f8g6_19+f9g5_38 ;\
h4[0,1]  = f02[0]   signed* g46[0];    h4[2,3]  = f02[1]   signed* g46[1] ;\
h4[0,1] += f02[2]   signed* g02[2];    h4[2,3] += f02[3]   signed* g02[3] ;\
h4[0,1] += f46[0]   signed* g02[0];    h4[2,3] += f46[1]   signed* g02[1] ;\
h4[0,1] += f89[0]   signed* g46_19[2]; h4[2,3] += f89[1]   signed* g46_19[3] ;\
h4[0,1] += f46[2]   signed* g89_19[0]; h4[2,3] += f46[3]   signed* g89_19[1] ;\
h4[0,1] += f13_2[0] signed* g13[2];    h4[2,3] += f13_2[1] signed* g13[3] ;\
h4[0,1] += f13_2[2] signed* g13[0];    h4[2,3] += f13_2[3] signed* g13[1] ;\
 ;\
# h2 = f0g2+f1g1_2 +f2g0 +f3g9_38+f4g8_19+f5g7_38+f6g6_19+f7g5_38+f8g4_19+f9g3_38 ;\
h2[0,1]  = f02[0]   signed* g02[2];    h2[2,3]  = f02[1]   signed* g02[3] ;\
h2[0,1] += f02[2]   signed* g02[0];    h2[2,3] += f02[3]   signed* g02[1] ;\
h2[0,1] += f46[2]   signed* g46_19[2]; h2[2,3] += f46[3]   signed* g46_19[3] ;\
h2[0,1] += f46[0]   signed* g89_19[0]; h2[2,3] += f46[1]   signed* g89_19[1] ;\
h2[0,1] += f89[0]   signed* g46_19[0]; h2[2,3] += f89[1]   signed* g46_19[1] ;\
 ;\
# h0 = f0g0+f1g9_38+f2g8_19+f3g7_38+f4g6_19+f5g5_38+f6g4_19+f7g3_38+f8g2_19+f9g1_38 ;\
h0[0,1]  = f02[0]   signed* g02[0];    h0[2,3]  = f02[1]   signed* g02[1] ;\
h0[0,1] += f46[0]   signed* g46_19[2]; h0[2,3] += f46[1]   signed* g46_19[3] ;\
h0[0,1] += f46[2]   signed* g46_19[0]; h0[2,3] += f46[3]   signed* g46_19[1] ;\
h0[0,1] += f89[0]   signed* mix[2];    h0[2,3] += f89[1]   signed* mix[3] ;\
h0[0,1] += f02[2]   signed* g89_19[0]; h0[2,3] += f02[3]   signed* g89_19[1] ;\
 ;\
	ptr = &f57_2_stack ;\
	f57_2 aligned= mem128[ptr] ;\
 ;\
h8[0,1] += f57_2[0] signed* g13[2];    h8[2,3] += f57_2[1] signed* g13[3] ;\
h8[0,1] += f57_2[2] signed* g13[0];    h8[2,3] += f57_2[3] signed* g13[1] ;\
h6[0,1] += f57_2[0] signed* g13[0];    h6[2,3] += f57_2[1] signed* g13[1] ;\
h6[0,1] += f57_2[2] signed* g89_19[2]; h6[2,3] += f57_2[3] signed* g89_19[3] ;\
h4[0,1] += f57_2[0] signed* g89_19[2]; h4[2,3] += f57_2[1] signed* g89_19[3] ;\
h4[0,1] += f57_2[2] signed* g57_19[2]; h4[2,3] += f57_2[3] signed* g57_19[3] ;\
h0[0,1] += f57_2[0] signed* g57_19[0]; h0[2,3] += f57_2[1] signed* g57_19[1] ;\
h0[0,1] += f57_2[2] signed* g13_19[2]; h0[2,3] += f57_2[3] signed* g13_19[3] ;\
h2[0,1] += f57_2[0] signed* g57_19[2]; h2[2,3] += f57_2[1] signed* g57_19[3] ;\
h2[0,1] += f57_2[2] signed* g57_19[0]; h2[2,3] += f57_2[3] signed* g57_19[1] ;\
 ;\
	ptr = &f13_2_stack ;\
	f13_2 aligned= mem128[ptr] ;\
 ;\
ptr = &_0x2000000_stack ;\
_0x2000000 aligned= mem128[ptr] ;\
h6[0,1] += f13_2[2] signed* g13[2];    h6[2,3] += f13_2[3] signed* g13[3] ;\
h0[0,1] += f13_2[0] signed* g89_19[2]; h0[2,3] += f13_2[1] signed* g89_19[3] ;\
h0[0,1] += f13_2[2] signed* g57_19[2]; h0[2,3] += f13_2[3] signed* g57_19[3] ;\
h2[0,1] += f13_2[0] signed* g13[0];    h2[2,3] += f13_2[1] signed* g13[1] ;\
ptr = &_0x1000000_stack ;\
_0x1000000 aligned= mem128[ptr] ;\
h2[0,1] += f13_2[2] signed* g89_19[2]; h2[2,3] += f13_2[3] signed* g89_19[3] ;\
 ;\
  ptr = &h7_stack ;\
  h7 aligned= mem128[ptr] ;\
 ;\
h0[0,1] += mix[0]   signed* g13_19[0]; h0[2,3] += mix[1]   signed* g13_19[1] ;\
  ptr = &h9_stack ;\
  h9 aligned= mem128[ptr] ;\
 ;\
h6[0,1] += mix[0]   signed* g57_19[2]; h6[2,3] += mix[1]   signed* g57_19[3] ;\
  ptr = &h5_stack ;\
  h5 aligned= mem128[ptr] ;\
 ;\
h4[0,1] += mix[0]   signed* g57_19[0]; h4[2,3] += mix[1]   signed* g57_19[1] ;\
 ;\
2x t0   = h0 + _0x2000000  ;\
				2x t6   = h6 + _0x2000000  ;\
 ;\
h2[0,1] += mix[0]   signed* g13_19[2]; h2[2,3] += mix[1]   signed* g13_19[3] ;\
  ;\
2x c0   = t0 signed>> 26  ;\
				2x c6   = t6 signed>> 26  ;\
2x h1 += c0  ;\
2x t0 = c0 << 26  ;\
	2x t1   = h1 + _0x1000000  ;\
				2x h7 += c6  ;\
				2x t6 = c6 << 26  ;\
					2x t7   = h7 + _0x1000000  ;\
2x h0 -= t0  ;\
	2x c1   = t1 signed>> 25  ;\
				2x h6 -= t6  ;\
					2x c7   = t7 signed>> 25  ;\
	2x h2 += c1  ;\
	2x t1 = c1 << 25  ;\
2x t2   = h2 + _0x2000000  ;\
					2x h8 += c7  ;\
	2x h1 -= t1  ;\
2x c2   = t2 signed>> 26  ;\
					2x t7 = c7 << 25  ;\
				2x t8   = h8 + _0x2000000  ;\
2x h3 += c2  ;\
2x t2 = c2 << 26  ;\
	2x t3   = h3 + _0x1000000  ;\
					2x h7 -= t7  ;\
				2x c8   = t8 signed>> 26  ;\
2x h2 -= t2  ;\
	2x c3   = t3 signed>> 25  ;\
				2x h9 += c8  ;\
				2x t8 = c8 << 26  ;\
					2x t9   = h9 + _0x1000000  ;\
	2x h4 += c3  ;\
posh = h ;\
	2x t3 = c3 << 25  ;\
posH = H ;\
2x t4   = h4 + _0x2000000  ;\
		posh+=8 ;\
				2x h8 -= t8  ;\
		posH+=8 ;\
					2x c9   = t9 signed>> 25  ;\
	2x h3 -= t3  ;\
2x c4   = t4 signed>> 26  ;\
					2x s   = c9 + c9  ;\
2x h5 += c4  ;\
		h2 h3 = h2[0]h3[0]h2[2]h2[3] h2[1]h3[1]h3[2]h3[3] ;\
2x t4 = c4 << 26  ;\
		h2 h3 = h2[0]h2[1]h2[2]h3[2] h3[0]h3[1]h2[3]h3[3] ;\
	2x t5   = h5 + _0x1000000  ;\
					2x h0 += s  ;\
		mem64[posh] aligned= h2[0];posh+=8  ;\
					2x s   = c9 << 4  ;\
		mem64[posH] aligned= h2[1];posH+=8  ;\
2x h4 -= t4  ;\
	2x c5   = t5 signed>> 25  ;\
					2x h0 += s  ;\
	2x h6 += c5  ;\
	2x t5 = c5 << 25  ;\
2x t6   = h6 + _0x2000000  ;\
					2x h0 += c9  ;\
					2x t9 = c9 << 25  ;\
				2x t0   = h0 + _0x2000000  ;\
	2x h5 -= t5  ;\
2x c6   = t6 signed>> 26  ;\
					2x h9 -= t9  ;\
		h4 h5 = h4[0]h5[0]h4[2]h4[3] h4[1]h5[1]h5[2]h5[3] ;\
				2x c0   = t0 signed>> 26  ;\
		h4 h5 = h4[0]h4[1]h4[2]h5[2] h5[0]h5[1]h4[3]h5[3] ;\
2x h7 += c6  ;\
		mem64[posh] aligned= h4[0]  ;\
2x t6 = c6 << 26  ;\
		mem64[posH] aligned= h4[1]  ;\
				2x h1 += c0  ;\
		h8 h9 = h8[0]h9[0]h8[2]h8[3] h8[1]h9[1]h9[2]h9[3] ;\
				2x t0 = c0 << 26  ;\
		h8 h9 = h8[0]h8[1]h8[2]h9[2] h9[0]h9[1]h8[3]h9[3] ;\
2x h6 -= t6  ;\
		posh+=16 ;\
				2x h0 -= t0  ;\
		mem64[posh] aligned= h8[0]  ;\
		posH+=16 ;\
		mem64[posH] aligned= h8[1]  ;\
 ;\
		h6 h7 = h6[0]h7[0]h6[2]h6[3] h6[1]h7[1]h7[2]h7[3] ;\
		h6 h7 = h6[0]h6[1]h6[2]h7[2] h7[0]h7[1]h6[3]h7[3] ;\
		posh-=8 ;\
		posH-=8 ;\
 ;\
		h0 h1 = h0[0]h1[0]h0[2]h0[3] h0[1]h1[1]h1[2]h1[3] ;\
		h0 h1 = h0[0]h0[1]h0[2]h1[2] h1[0]h1[1]h0[3]h1[3] ;\
 ;\
		mem64[posh] aligned= h6[0]  ;\
		mem64[posH] aligned= h6[1]  ;\
		posh-=24 ;\
		posH-=24 ;\
		mem64[posh] aligned= h0[0]  ;\
		mem64[posH] aligned= h0[1]  ;\


@define fe_mul(h,f,g) ;\
posf = f ;\
posg = g ;\
posh = h ;\
 ;\
4x _19_19_19_19 = 19 ;\
 ;\
4x _0_1_0_1 = 0 ;\
 ;\
4x _1_1_1_1 = 1 ;\
 ;\
_0_1_0_1[0,1,2,3] _1_1_1_1[0,1,2,3] = _0_1_0_1[0]_1_1_1_1[0]_0_1_0_1[1]_1_1_1_1[1] _0_1_0_1[2]_1_1_1_1[2]_0_1_0_1[3]_1_1_1_1[3] ;\
 ;\
g0_g1_g2_g3 aligned= mem128[posg];posg+=16 ;\
 ;\
g4_g5_g6_g7 aligned= mem128[posg];posg+=16 ;\
 ;\
new f8_f9_g8_g9 ;\
f8_f9_g8_g9 aligned= f8_f9_g8_g9[0]mem64[posg] ;\
 ;\
f0_f1_f2_f3 aligned= mem128[posf];posf+=16 ;\
playp = &playground2 ;\
 ;\
f4_f5_f6_f7 aligned= mem128[posf];posf+=16 ;\
4x 19g0_19g1_19g2_19g3 = g0_g1_g2_g3 * _19_19_19_19 ;\
f8_f9_g8_g9 aligned= mem64[posf]f8_f9_g8_g9[1] ;\
 ;\
new f1_f8_f3_f0 ;\
f1_f8_f3_f0 = f1_f8_f3_f0[0,1]f0_f1_f2_f3[3]f0_f1_f2_f3[0] ;\
  4x 19g4_19g5_19g6_19g7 = g4_g5_g6_g7 * _19_19_19_19 ;\
 ;\
f1_f8_f3_f0 = f0_f1_f2_f3[1]f8_f9_g8_g9[0]f1_f8_f3_f0[2,3] ;\
4x f0_2f1_f2_2f3 = f0_f1_f2_f3 << _0_1_0_1 ;\
new g0_19g1_g2_19g3 ;\
g0_19g1_g2_19g3 = 19g0_19g1_19g2_19g3[1]g0_g1_g2_g3[0]g0_19g1_g2_19g3[2,3] # ;\
 ;\
new g4_19g5_g6_19g7 ;\
g4_19g5_g6_19g7 = 19g4_19g5_19g6_19g7[1]g4_g5_g6_g7[0]g4_19g5_g6_19g7[2,3] # ;\
  4x f4_2f5_f6_2f7 = f4_f5_f6_f7 << _0_1_0_1 ;\
 ;\
new f8_2f9_f9_f6 ;\
f8_2f9_f9_f6 = f8_f9_g8_g9[0] << _0_1_0_1[0],f8_f9_g8_g9[1] << _0_1_0_1[1],f8_2f9_f9_f6[2,3] ;\
 ;\
g0_19g1_g2_19g3 = g0_19g1_g2_19g3[1]g0_19g1_g2_19g3[0]g0_19g1_g2_19g3[2,3] ;\
g8_19g9_19g8_19g9[0,1] = g8_19g9_19g8_19g9[0,1];g8_19g9_19g8_19g9[2] = f8_f9_g8_g9[2] * _19_19_19_19[2];g8_19g9_19g8_19g9[3] = f8_f9_g8_g9[3] * _19_19_19_19[3] # wants to move up ;\
g4_19g5_g6_19g7 = g4_19g5_g6_19g7[1]g4_19g5_g6_19g7[0]g4_19g5_g6_19g7[2,3]  ;\
 ;\
 ;\
f8_2f9_f9_f6 = f8_2f9_f9_f6[0,1]f8_f9_g8_g9[1]f4_f5_f6_f7[2] ;\
 ;\
 ;\
g8_19g9_19g8_19g9 = g8_19g9_19g8_19g9[3]f8_f9_g8_g9[2]g8_19g9_19g8_19g9[2,3] # ;\
 ;\
g8_19g9_19g8_19g9 = g8_19g9_19g8_19g9[1]g8_19g9_19g8_19g9[0]g8_19g9_19g8_19g9[2,3] ;\
 ;\
new 19g8_g9_19g2_g3 ;\
19g8_g9_19g2_g3 = f8_f9_g8_g9[3]g8_19g9_19g8_19g9[2]19g8_g9_19g2_g3[2,3] # ;\
g0_19g1_g2_19g3 = g0_19g1_g2_19g3[0,1]19g0_19g1_19g2_19g3[3]g0_g1_g2_g3[2] # ;\
 ;\
 ;\
 ;\
h02[0,1]  = f0_2f1_f2_2f3[0] signed* g0_g1_g2_g3[0];          h02[2,3]  = f0_2f1_f2_2f3[1] signed* g0_g1_g2_g3[1] ;\
g0_19g1_g2_19g3 = g0_19g1_g2_19g3[0,1]g0_19g1_g2_19g3[3]g0_19g1_g2_19g3[2] ;\
 ;\
h02[0,1] += f0_2f1_f2_2f3[2] signed* g8_19g9_19g8_19g9[2];    h02[2,3] += f0_2f1_f2_2f3[3] signed* g8_19g9_19g8_19g9[3] ;\
 ;\
19g8_g9_19g2_g3 = 19g8_g9_19g2_g3[1]19g8_g9_19g2_g3[0]19g8_g9_19g2_g3[2,3] ;\
h02[0,1] += f4_2f5_f6_2f7[0] signed* 19g4_19g5_19g6_19g7[2];  h02[2,3] += f4_2f5_f6_2f7[1] signed* 19g4_19g5_19g6_19g7[3] ;\
 ;\
19g8_g9_19g2_g3 = 19g8_g9_19g2_g3[0,1]g0_g1_g2_g3[3]19g0_19g1_19g2_19g3[2] # ;\
h02[0,1] += f4_2f5_f6_2f7[2] signed* 19g4_19g5_19g6_19g7[0];  h02[2,3] += f4_2f5_f6_2f7[3] signed* 19g4_19g5_19g6_19g7[1] ;\
 ;\
19g8_g9_19g2_g3 = 19g8_g9_19g2_g3[0,1]19g8_g9_19g2_g3[3]19g8_g9_19g2_g3[2]  ;\
h02[0,1] += f8_2f9_f9_f6[0]  signed* 19g0_19g1_19g2_19g3[2];  h02[2,3] += f8_2f9_f9_f6[1]  signed* 19g0_19g1_19g2_19g3[3]    ;\
 ;\
new f5_f2_f7_f4 ;\
f5_f2_f7_f4 = f4_f5_f6_f7[1]f0_f1_f2_f3[2]f5_f2_f7_f4[2,3] ;\
h31[0,1]  = f1_f8_f3_f0[0]   signed* g0_19g1_g2_19g3[2];      h31[2,3]  = f1_f8_f3_f0[1]   signed* g0_19g1_g2_19g3[3] ;\
 ;\
f5_f2_f7_f4 = f5_f2_f7_f4[0,1]f4_f5_f6_f7[3]f4_f5_f6_f7[0] ;\
h31[0,1] += f1_f8_f3_f0[2]   signed* g0_g1_g2_g3[0];          h31[2,3] += f1_f8_f3_f0[3]   signed* g0_g1_g2_g3[1] ;\
 ;\
mem64[playp] aligned= h02[0];playp+=8 ;\
h31[0,1] += f5_f2_f7_f4[0]   signed* g8_19g9_19g8_19g9[2];    h31[2,3] += f5_f2_f7_f4[1]   signed* g8_19g9_19g8_19g9[3] ;\
 ;\
new h24 ;\
h24 = h02[2,3]h24[2,3] ;\
h31[0,1] += f5_f2_f7_f4[2]   signed* 19g4_19g5_19g6_19g7[2];  h31[2,3] += f5_f2_f7_f4[3]   signed* 19g4_19g5_19g6_19g7[3] ;\
 ;\
 ;\
h24 = h24[0],0 ;\
h31[0,1] += f8_2f9_f9_f6[2]  signed* 19g4_19g5_19g6_19g7[0];  h31[2,3] += f8_2f9_f9_f6[3]  signed* 19g4_19g5_19g6_19g7[1] ;\
 ;\
g4_19g5_g6_19g7 = g4_19g5_g6_19g7[0,1]19g4_19g5_19g6_19g7[3]g4_g5_g6_g7[2] # ;\
h24[0,1] += f0_2f1_f2_2f3[0] signed* g0_g1_g2_g3[2];          h24[2,3] += f0_2f1_f2_2f3[1] signed* g0_g1_g2_g3[3] ;\
 ;\
g4_19g5_g6_19g7 = g4_19g5_g6_19g7[0,1]g4_19g5_g6_19g7[3]g4_19g5_g6_19g7[2] ;\
h24[0,1] += f0_2f1_f2_2f3[2] signed* g0_g1_g2_g3[0];          h24[2,3] += f0_2f1_f2_2f3[3] signed* g0_g1_g2_g3[1] ;\
 ;\
mem64[playp] aligned= h31[1];playp+=8 ;\
h24[0,1] += f4_2f5_f6_2f7[0] signed* g8_19g9_19g8_19g9[2];    h24[2,3] += f4_2f5_f6_2f7[1] signed* g8_19g9_19g8_19g9[3] ;\
 ;\
new h53 ;\
h53 = h53[0,1]h31[0,1] ;\
h24[0,1] += f4_2f5_f6_2f7[2] signed* 19g4_19g5_19g6_19g7[2];  h24[2,3] += f4_2f5_f6_2f7[3] signed* 19g4_19g5_19g6_19g7[3] ;\
 ;\
h53 = 0,h53[1] ;\
h24[0,1] += f8_2f9_f9_f6[0]  signed* 19g4_19g5_19g6_19g7[0];  h24[2,3] += f8_2f9_f9_f6[1]  signed* 19g4_19g5_19g6_19g7[1]    ;\
 ;\
h53[0,1] += f1_f8_f3_f0[0]   signed* g4_19g5_g6_19g7[0];      h53[2,3] += f1_f8_f3_f0[1]   signed* g4_19g5_g6_19g7[1] ;\
 ;\
h53[0,1] += f1_f8_f3_f0[2]   signed* g0_g1_g2_g3[2];          h53[2,3] += f1_f8_f3_f0[3]   signed* g0_g1_g2_g3[3] ;\
 ;\
h53[0,1] += f5_f2_f7_f4[0]   signed* g0_g1_g2_g3[0];          h53[2,3] += f5_f2_f7_f4[1]   signed* g0_g1_g2_g3[1] ;\
 ;\
mem64[playp] aligned= h24[0];playp+=8 ;\
h53[0,1] += f5_f2_f7_f4[2]   signed* g8_19g9_19g8_19g9[2];    h53[2,3] += f5_f2_f7_f4[3]   signed* g8_19g9_19g8_19g9[3] ;\
 ;\
new h46 ;\
h46 = h24[2,3]h46[2,3] ;\
h53[0,1] += f8_2f9_f9_f6[2]  signed* 19g4_19g5_19g6_19g7[2];  h53[2,3] += f8_2f9_f9_f6[3]  signed* 19g4_19g5_19g6_19g7[3] ;\
 ;\
h46 = h46[0],0 ;\
h46[0,1] += f0_2f1_f2_2f3[0] signed* g4_g5_g6_g7[0];          h46[2,3] += f0_2f1_f2_2f3[1] signed* g4_g5_g6_g7[1]  ;\
 ;\
h46[0,1] += f0_2f1_f2_2f3[2] signed* g0_g1_g2_g3[2];          h46[2,3] += f0_2f1_f2_2f3[3] signed* g0_g1_g2_g3[3]  ;\
 ;\
h46[0,1] += f4_2f5_f6_2f7[0] signed* g0_g1_g2_g3[0];          h46[2,3] += f4_2f5_f6_2f7[1] signed* g0_g1_g2_g3[1]  ;\
 ;\
new h75 ;\
h75 = h75[0,1]h53[0,1] ;\
h46[0,1] += f4_2f5_f6_2f7[2] signed* g8_19g9_19g8_19g9[2];    h46[2,3] += f4_2f5_f6_2f7[3] signed* g8_19g9_19g8_19g9[3] ;\
 ;\
h75 = 0,h75[1] ;\
h46[0,1] += f8_2f9_f9_f6[0]  signed* 19g4_19g5_19g6_19g7[2];  h46[2,3] += f8_2f9_f9_f6[1]  signed* 19g4_19g5_19g6_19g7[3] ;\
 ;\
mem64[playp] aligned= h53[1];playp+=8 ;\
h75[0,1] += f1_f8_f3_f0[0]   signed* g4_19g5_g6_19g7[2];      h75[2,3] += f1_f8_f3_f0[1]   signed* g4_19g5_g6_19g7[3] ;\
 ;\
h75[0,1] += f1_f8_f3_f0[2]   signed* g4_g5_g6_g7[0];          h75[2,3] += f1_f8_f3_f0[3]   signed* g4_g5_g6_g7[1] ;\
 ;\
mem64[playp] aligned= h46[0] ;\
h75[0,1] += f5_f2_f7_f4[0]   signed* g0_g1_g2_g3[2];          h75[2,3] += f5_f2_f7_f4[1]   signed* g0_g1_g2_g3[3] ;\
 ;\
new h68 ;\
h68 = h46[2,3]h68[2,3] ;\
h75[0,1] += f5_f2_f7_f4[2]   signed* g0_g1_g2_g3[0];          h75[2,3] += f5_f2_f7_f4[3]   signed* g0_g1_g2_g3[1] ;\
 ;\
h68 = h68[0],0 ;\
h75[0,1] += f8_2f9_f9_f6[2]  signed* g8_19g9_19g8_19g9[2];    h75[2,3] += f8_2f9_f9_f6[3]  signed* g8_19g9_19g8_19g9[3] ;\
 ;\
h68[0,1] += f0_2f1_f2_2f3[0] signed* g4_g5_g6_g7[2];          h68[2,3] += f0_2f1_f2_2f3[1] signed* g4_g5_g6_g7[3] ;\
 ;\
h68[0,1] += f0_2f1_f2_2f3[2] signed* g4_g5_g6_g7[0];          h68[2,3] += f0_2f1_f2_2f3[3] signed* g4_g5_g6_g7[1] ;\
 ;\
h68[0,1] += f4_2f5_f6_2f7[0] signed* g0_g1_g2_g3[2];          h68[2,3] += f4_2f5_f6_2f7[1] signed* g0_g1_g2_g3[3] ;\
 ;\
new h97 ;\
h97 = h97[0,1]h75[0,1] ;\
h68[0,1] += f4_2f5_f6_2f7[2] signed* g0_g1_g2_g3[0];          h68[2,3] += f4_2f5_f6_2f7[3] signed* g0_g1_g2_g3[1] ;\
 ;\
h97 = 0,h97[1] ;\
h68[0,1] += f8_2f9_f9_f6[0]  signed* g8_19g9_19g8_19g9[2];    h68[2,3] += f8_2f9_f9_f6[1]  signed* g8_19g9_19g8_19g9[3] ;\
 ;\
h97[0,1] += f1_f8_f3_f0[0]   signed* g8_19g9_19g8_19g9[0];    h97[2,3] += f1_f8_f3_f0[1]   signed* g8_19g9_19g8_19g9[1] ;\
 ;\
playp -= 32 ;\
h97[0,1] += f1_f8_f3_f0[2]   signed* g4_g5_g6_g7[2];          h97[2,3] += f1_f8_f3_f0[3]   signed* g4_g5_g6_g7[3] ;\
 ;\
h97[0,1] += f5_f2_f7_f4[0]   signed* g4_g5_g6_g7[0];          h97[2,3] += f5_f2_f7_f4[1]   signed* g4_g5_g6_g7[1] ;\
new h80 ;\
h80 = h68[2,3]h80[2,3] ;\
 ;\
h97[0,1] += f5_f2_f7_f4[2]   signed* g0_g1_g2_g3[2];          h97[2,3] += f5_f2_f7_f4[3]   signed* g0_g1_g2_g3[3] ;\
h80 aligned= h80[0]mem64[playp];playp+=8 ;\
 ;\
h97[0,1] += f8_2f9_f9_f6[2]  signed* g0_g1_g2_g3[0];          h97[2,3] += f8_2f9_f9_f6[3]  signed* g0_g1_g2_g3[1] ;\
 ;\
h80[0,1] += f0_2f1_f2_2f3[0] signed* g8_19g9_19g8_19g9[0];    h80[2,3] += f0_2f1_f2_2f3[1] signed* g8_19g9_19g8_19g9[1] ;\
new 19g4_g5_19g6_g7 ;\
19g4_g5_19g6_g7 = g4_g5_g6_g7[1]19g4_19g5_19g6_19g7[0]19g4_g5_19g6_g7[2,3] ;\
 ;\
h80[0,1] += f0_2f1_f2_2f3[2] signed* g4_19g5_g6_19g7[2];      h80[2,3] += f0_2f1_f2_2f3[3] signed* g4_19g5_g6_19g7[3] ;\
19g4_g5_19g6_g7 = 19g4_g5_19g6_g7[1]19g4_g5_19g6_g7[0]19g4_g5_19g6_g7[2,3] ;\
 ;\
h80[0,1] += f4_2f5_f6_2f7[0] signed* g4_19g5_g6_19g7[0];      h80[2,3] += f4_2f5_f6_2f7[1] signed* g4_19g5_g6_19g7[1] ;\
19g4_g5_19g6_g7 = 19g4_g5_19g6_g7[0,1]g4_g5_g6_g7[3]19g4_19g5_19g6_19g7[2] ;\
 ;\
new h19 ;\
h19 = h19[0,1]h97[0,1] ;\
h80[0,1] += f4_2f5_f6_2f7[2] signed* g0_19g1_g2_19g3[2];      h80[2,3] += f4_2f5_f6_2f7[3] signed* g0_19g1_g2_19g3[3] ;\
19g4_g5_19g6_g7 = 19g4_g5_19g6_g7[0,1]19g4_g5_19g6_g7[3]19g4_g5_19g6_g7[2]  ;\
 ;\
 ;\
h19 aligned= mem64[playp]h19[1] ;\
h80[0,1] += f8_2f9_f9_f6[0]  signed* g0_19g1_g2_19g3[0];      h80[2,3] += f8_2f9_f9_f6[1]  signed* g0_19g1_g2_19g3[1] ;\
 ;\
h19[0,1] += f1_f8_f3_f0[0]   signed* g0_g1_g2_g3[0];          h19[2,3] += f1_f8_f3_f0[1]   signed* g0_g1_g2_g3[1] ;\
 ;\
playp+=24 ;\
h19[0,1] += f1_f8_f3_f0[2]   signed* 19g8_g9_19g2_g3[0];      h19[2,3] += f1_f8_f3_f0[3]   signed* 19g8_g9_19g2_g3[1] ;\
new h04 ;\
h04 = h80[2,3]h04[2,3] ;\
 ;\
new h37 ;\
h37 = h37[0]h97[1] ;\
h19[0,1] += f5_f2_f7_f4[0]   signed* 19g4_g5_19g6_g7[2];      h19[2,3] += f5_f2_f7_f4[1]   signed* 19g4_g5_19g6_g7[3] ;\
new h15 ;\
h15 = h15[0,1]h75[2,3] ;\
 ;\
new h48 ;\
h48 = h48[0,1]h80[0,1] ;\
h19[0,1] += f5_f2_f7_f4[2]   signed* 19g4_g5_19g6_g7[0];      h19[2,3] += f5_f2_f7_f4[3]   signed* 19g4_g5_19g6_g7[1] ;\
new h26 ;\
h26 = h26[0,1]h68[0,1] ;\
 ;\
h19[0,1] += f8_2f9_f9_f6[2]  signed* 19g8_g9_19g2_g3[2];      h19[2,3] += f8_2f9_f9_f6[3]  signed* 19g8_g9_19g2_g3[3] ;\
h04 aligned= h04[0]mem64[playp] ;\
 ;\
playp -= 16 ;\
 ;\
h15 = h19[0,1]h15[2,3] ;\
 ;\
4x mask26 = 0xffffffff ;\
2x mask25 = mask26 << 25 ;\
 ;\
ptr = &_0x2000000_stack ;\
_0x2000000 aligned= mem128[ptr] ;\
2x t0 = h04 + _0x2000000 ;\
 ;\
2x mask26 <<= 26 ;\
 ;\
#waiting for t0 ;\
 ;\
2x c = t0 signed>> 26 ;\
h26 aligned= mem64[playp]h26[1];playp += 8 ;\
 ;\
#waiting for c ;\
 ;\
2x h15 += c ;\
 ;\
t0 &= mask26 ;\
h37 aligned= mem64[playp]h37[1];playp += 8 ;\
 ;\
ptr = &_0x1000000_stack ;\
_0x1000000 aligned= mem128[ptr] ;\
2x t1 = h15 + _0x1000000 ;\
 ;\
2x h04 -= t0 ;\
 ;\
#waiting for t1 ;\
 ;\
2x c = t1 signed>> 25 ;\
h48 = h04[2,3]h48[2,3] ;\
 ;\
#waiting for t1 ;\
 ;\
t1 &= mask25 ;\
 ;\
2x h26 += c ;\
new h59 ;\
h59 = h59[0]h19[1] ;\
 ;\
2x t0 = h26 + _0x2000000 ;\
 ;\
2x h15 -= t1 ;\
 ;\
#waiting for t0 ;\
 ;\
2x c = t0 signed>> 26 ;\
h59 = h15[2,3]h59[2,3] ;\
 ;\
t0 &= mask26 ;\
 ;\
2x h37 += c ;\
 ;\
2x t1 = h37 + _0x1000000 ;\
 ;\
2x h26 -= t0 ;\
 ;\
#waiting for t1 ;\
 ;\
2x c = t1 signed>> 25 ;\
 ;\
t1 &= mask25 ;\
 ;\
2x h48 += c ;\
 ;\
2x t0 = h48 + _0x2000000 ;\
 ;\
2x h37 -= t1 ;\
 ;\
#waiting for t0 ;\
 ;\
2x c = t0 signed>> 26 ;\
 ;\
t0 &= mask26 ;\
 ;\
2x h59 += c ;\
 ;\
new t ;\
t = t[0], h59[1] +  _0x1000000[1] ;\
 ;\
2x h48 -= t0 ;\
 ;\
#waiting for t ;\
 ;\
c = c[0],t[1] signed>> 25 ;\
 ;\
t &= mask25 ;\
 ;\
new s2 ;\
s2 = s2[0],c[1] + c[1] ;\
 ;\
new s ;\
s = s[0],c[1] << 4 ;\
 ;\
s2 = s2[0],s2[1] + c[1] ;\
 ;\
#waiting for s2 ;\
 ;\
s = s[0],s[1] + s2[1] ;\
 ;\
#waiting for s ;\
 ;\
h04 = h04[0] + s[1],h04[1] ;\
 ;\
h26[0,1,2,3] h37[0,1,2,3] = h26[0]h37[0]h26[1]h37[1] h26[2]h37[2]h26[3]h37[3] # h26 now contains 0,0,h2,h3, h37 contains 0,0,h6,h7 ;\
 ;\
t0 = h04[0] + _0x2000000[0],t0[1] ;\
 ;\
posh += 8 ;\
mem64[posh] aligned= h26[0] ;\
h59 = h59[0],h59[1] - t[1] ;\
 ;\
posh += 16 ;\
mem64[posh] aligned= h37[0] ;\
c = t0[0] signed>> 26,c[1] ;\
 ;\
t0 &= mask26 ;\
 ;\
h15 = h15[0] + c[0],h15[1] ;\
 ;\
h48[0,1,2,3] h59[0,1,2,3] = h48[0]h59[0]h48[1]h59[1] h48[2]h59[2]h48[3]h59[3] # h48 now contains 0,0,h4,h5, h59 contains 0,0,h8,h9 ;\
 ;\
h04 = h04[0] - t0[0],h04[1] ;\
 ;\
#waiting for h04 ;\
posh -= 8 ;\
mem64[posh] aligned= h48[0] ;\
posh += 16 ;\
mem64[posh] aligned= h59[0] ;\
#waiting for h04 ;\
 ;\
# Costs 3 cycles! ;\
h04[0,1,2,3] h15[0,1,2,3] = h04[0]h15[0]h04[1]h15[1] h04[2]h15[2]h04[3]h15[3] # h04 now contains 0,0,h0,h1,  ;\
 ;\
posh -= 32 ;\
mem64[posh] aligned= h04[0] ;\


qpushenter crypto_scalarmult_curve25519_neon2

stack64 stack_r45
stack64 stack_r67
stack64 stack_r89
stack64 stack_r1011
stack32 stack_r12
stack32 stack_r14
assign r4 r5 to caller_r4 caller_r5; stack_r45 = caller_r4 caller_r5
assign r6 r7 to caller_r6 caller_r7; stack_r67 = caller_r6 caller_r7
assign r8 r9 to caller_r8 caller_r9; stack_r89 = caller_r8 caller_r9
assign r10 r11 to caller_r10 caller_r11; stack_r1011 = caller_r10 caller_r11
stack_r12 = caller_r12
stack_r14 = caller_r14

stack32 swap_stack
stack32 pos_stack
stack256 e
int32 eptr

q = input_0
n = input_1
p = input_2
playground1_ptr = &playground1

swap = 0
pos = 254

4x _0x1000000 = 1
2x _0x2000000 = _0x1000000 unsigned>> 7
2x _0x1000000 = _0x1000000 unsigned>> 8
new _19_19_38_38
_19_19_38_38 = 19,19,_19_19_38_38[2,3]
_19_19_38_38 = _19_19_38_38[0,1],38,38

ptr = &_0x2000000_stack
mem128[ptr] aligned= _0x2000000

ptr = &_0x1000000_stack
mem128[ptr] aligned= _0x1000000

ptr = &_19_19_38_38_stack
mem128[ptr] aligned= _19_19_38_38

fe_0(const121666)
ptr = const121666
word = 960
word = word - 2
word = -word
word = word - (word << 7)
mem32[ptr] = word

eptr = &e
e0 = mem128[n]; n += 16
e4 = mem128[n]
mem128[eptr] aligned= e0; eptr += 16
mem128[eptr] aligned= e4
eptr -= 16

byte = mem8[eptr]
byte &= 248
mem8[eptr] = byte

byte = mem8[eptr + 31]
byte &= 127
byte |= 64
mem8[eptr + 31] = byte

2x mask26 = 0xffffffff
2x mask25 = mask26 unsigned>> 7
2x mask26 = mask26 unsigned>> 6

new h0
new h1
new h2
new h3
new h4
new h5
new h6
new h7
new h8
new h9

h0 = mem64[p] h0[1]
h1 = mem64[p] h1[1]
p += 6
h2 = mem64[p] h2[1]
h3 = mem64[p] h3[1]
p += 6
h4 = mem64[p] h4[1]
p += 4
h5 = mem64[p] h5[1]
h6 = mem64[p] h6[1]
p += 6
h7 = mem64[p] h7[1]
p += 2
h8 = mem64[p] h8[1]
h9 = mem64[p] h9[1]

2x h1 unsigned>>= 26
2x h2 unsigned>>= 3
2x h3 unsigned>>= 29
2x h4 unsigned>>= 6
2x h6 unsigned>>= 25
2x h7 unsigned>>= 3
2x h8 unsigned>>= 12
2x h9 unsigned>>= 38

h0 &= mask26
h2 &= mask26
h4 &= mask26
h6 &= mask26
h8 &= mask26
h1 &= mask25
h3 &= mask25
h5 &= mask25
h7 &= mask25
h9 &= mask25

posh = x1

2x t0   = h0 + _0x2000000 
				2x t6   = h6 + _0x2000000 

2x c0   = t0 signed>> 26 
				2x c6   = t6 signed>> 26 
2x h1 += c0 
2x t0 = c0 << 26 
	2x t1   = h1 + _0x1000000 
				2x h7 += c6 
				2x t6 = c6 << 26 
					2x t7   = h7 + _0x1000000 
2x h0 -= t0 
	2x c1   = t1 signed>> 25 
				2x h6 -= t6 
					2x c7   = t7 signed>> 25 
	2x h2 += c1 
	2x t1 = c1 << 25 
2x t2   = h2 + _0x2000000 
					2x h8 += c7 
	2x h1 -= t1 
2x c2   = t2 signed>> 26 
					2x t7 = c7 << 25 
				2x t8   = h8 + _0x2000000 
2x h3 += c2 
2x t2 = c2 << 26 
	2x t3   = h3 + _0x1000000 
					2x h7 -= t7 
				2x c8   = t8 signed>> 26 
2x h2 -= t2 
	2x c3   = t3 signed>> 25 
				2x h9 += c8 
				2x t8 = c8 << 26 
					2x t9   = h9 + _0x1000000 
	2x h4 += c3 
	2x t3 = c3 << 25 
2x t4   = h4 + _0x2000000 
		posh+=8
				2x h8 -= t8 
					2x c9   = t9 signed>> 25 
	2x h3 -= t3 
2x c4   = t4 signed>> 26 
					2x s   = c9 + c9 
2x h5 += c4 
		h2 h3 = h2[0]h3[0]h2[2]h2[3] h2[1]h3[1]h3[2]h3[3]
2x t4 = c4 << 26 
		h2 h3 = h2[0]h2[1]h2[2]h3[2] h3[0]h3[1]h2[3]h3[3]
	2x t5   = h5 + _0x1000000 
					2x h0 += s 
		mem64[posh] aligned= h2[0];posh+=8 
					2x s   = c9 << 4 
2x h4 -= t4 
	2x c5   = t5 signed>> 25 
					2x h0 += s 
	2x h6 += c5 
	2x t5 = c5 << 25 
2x t6   = h6 + _0x2000000 
					2x h0 += c9 
					2x t9 = c9 << 25 
				2x t0   = h0 + _0x2000000 
	2x h5 -= t5 
2x c6   = t6 signed>> 26 
					2x h9 -= t9 
		h4 h5 = h4[0]h5[0]h4[2]h4[3] h4[1]h5[1]h5[2]h5[3]
				2x c0   = t0 signed>> 26 
		h4 h5 = h4[0]h4[1]h4[2]h5[2] h5[0]h5[1]h4[3]h5[3]
2x h7 += c6 
		mem64[posh] aligned= h4[0] 
2x t6 = c6 << 26 
				2x h1 += c0 
		h8 h9 = h8[0]h9[0]h8[2]h8[3] h8[1]h9[1]h9[2]h9[3]
				2x t0 = c0 << 26 
		h8 h9 = h8[0]h8[1]h8[2]h9[2] h9[0]h9[1]h8[3]h9[3]
2x h6 -= t6 
		posh+=16
				2x h0 -= t0 
		mem64[posh] aligned= h8[0] 

		h6 h7 = h6[0]h7[0]h6[2]h6[3] h6[1]h7[1]h7[2]h7[3]
		h6 h7 = h6[0]h6[1]h6[2]h7[2] h7[0]h7[1]h6[3]h7[3]
		posh-=8

		h0 h1 = h0[0]h1[0]h0[2]h0[3] h0[1]h1[1]h1[2]h1[3]
		h0 h1 = h0[0]h0[1]h0[2]h1[2] h1[0]h1[1]h0[3]h1[3]

		mem64[posh] aligned= h6[0] 
		posh-=24
		mem64[posh] aligned= h0[0] 

fe_1(x2)
fe_0(z2)
fe_1(z3)
fe_copy(x3,x1)

mainloop:
  pos8 = (pos unsigned>> 3)
  pos7 = pos & 7

  bit = mem8[eptr + pos8]
  bit unsigned>>= pos7
  bit &= 1
  pos_stack = pos
  swap ^= bit
  swap_stack = bit
  swap = -swap

  fe_negcswap2addsub(x2,x3,z2,z3,swap)
  fe_sqsq(tmp0,x2,tmp1,z2)
  fe_mulmul(z2,z3,x2,x2,x3,z2)
  fe_sub(z3,tmp0,tmp1)
  fe_addsub(x3,z2,x2,z2)
  fe_mulmul(tmp0,const121666,z3,x2,tmp0,tmp1)
  fe_sqsq(z2,z2,x3,x3)
  fe_add(tmp0,tmp1,tmp0)
  fe_mulmul(z3,x1,z2,z2,z3,tmp0)

  pos = pos_stack
  swap = swap_stack

                  signed<? pos -= 1
goto mainloop if !signed<

# skip: swap is always 0 here since last exponent bit is 0
# swap = -swap
# fe_negcswap2(x2,x3,z2,z3,swap)

fe_copy(tmp1,z2)

i = 0
invertloop:

  mulsource = z2
  postcopy = 0
  j = 2
           =? i - 1
  j = 1 if =
  mulsource = tmp1 if =
  postcopy = z11_copy if =
           =? i - 2
  j = 1 if =
  mulsource = z11_copy if =
           =? i - 3
  j = 5 if =
  postcopy = tmp1 if =
            =? i - 4
  j = 10 if =
            =? i - 5
  j = 20 if =
            =? i - 6
  j = 10 if =
  mulsource = tmp1 if =
  postcopy = tmp1 if =
            =? i - 7
  j = 50 if =
             =? i - 8
  j = 100 if =
            =? i - 9
  j = 50 if =
  mulsource = tmp1 if =
           =? i - 10
  j = 5 if =
  mulsource = z11_copy if =
           =? i - 11
  j = 0 if =
  mulsource = x2 if =

  fe_copy(tmp0,z2)

                           =? j - 0
  goto skipsquaringloop if =
  squaringloop:
    fe_mul(tmp0,tmp0,tmp0)
                       unsigned>? j -= 1
  goto squaringloop if unsigned>
  skipsquaringloop:

  fe_mul(z2,mulsource,tmp0)

                       =? postcopy - 0
  goto skippostcopy if =
    fe_copy(postcopy,z2)
  skippostcopy:

                         =? i - 1
  goto skipfinalcopy if !=
    fe_copy(z2,tmp0)
  skipfinalcopy:

                              i += 1
                   unsigned<? i - 12
goto invertloop if unsigned<

  posf = z2
  out0 = mem32[posf]; posf += 4
  out1 = mem32[posf]; posf += 4
  out2 = mem32[posf]; posf += 4
  out3 = mem32[posf]; posf += 4
  out4 = mem32[posf]; posf += 4
  out5 = mem32[posf]; posf += 4
  out6 = mem32[posf]; posf += 4
  out7 = mem32[posf]; posf += 4
  out8 = mem32[posf]; posf += 4
  out9 = mem32[posf]

  carry = out9 + (out9 << 4)
  carry = carry + (out9 << 1)
  carry += 16777216
  carry signed>>= 25
  carry += out0
  carry signed>>= 26
  carry += out1
  carry signed>>= 25
  carry += out2
  carry signed>>= 26
  carry += out3
  carry signed>>= 25
  carry += out4
  carry signed>>= 26
  carry += out5
  carry signed>>= 25
  carry += out6
  carry signed>>= 26
  carry += out7
  carry signed>>= 25
  carry += out8
  carry signed>>= 26
  carry += out9
  carry signed>>= 25

  out0 += carry
  out0 += (carry << 1)
  out0 += (carry << 4)

  carry0 = (out0 signed>> 26)
  out1 += carry0
  out0 -= (carry0 << 26)

  carry1 = (out1 signed>> 25)
  out2 += carry1
  out1 -= (carry1 << 25)

  carry2 = (out2 signed>> 26)
  out3 += carry2
  out2 -= (carry2 << 26)

  carry3 = (out3 signed>> 25)
  out4 += carry3
  out3 -= (carry3 << 25)

  carry4 = (out4 signed>> 26)
  out5 += carry4
  out4 -= (carry4 << 26)

  carry5 = (out5 signed>> 25)
  out6 += carry5
  out5 -= (carry5 << 25)

  carry6 = (out6 signed>> 26)
  out7 += carry6
  out6 -= (carry6 << 26)

  carry7 = (out7 signed>> 25)
  out8 += carry7
  out7 -= (carry7 << 25)

  carry8 = (out8 signed>> 26)
  out9 += carry8
  out8 -= (carry8 << 26)

  carry9 = (out9 signed>> 25)
  out9 -= (carry9 << 25)

  out0 += (out1 << 26)

  out1 unsigned>>= 6
  out1 += (out2 << 19)

  out2 unsigned>>= 13
  out2 += (out3 << 13)

  out3 unsigned>>= 19
  out3 += (out4 << 6)

  out5 += (out6 << 25)

  out6 unsigned>>= 7
  out6 += (out7 << 19)

  out7 unsigned>>= 13
  out7 += (out8 << 12)

  out8 unsigned>>= 20
  out8 += (out9 << 6)

  mem32[q] = out0; q += 4
  mem32[q] = out1; q += 4
  mem32[q] = out2; q += 4
  mem32[q] = out3; q += 4
  mem32[q] = out5; q += 4
  mem32[q] = out6; q += 4
  mem32[q] = out7; q += 4
  mem32[q] = out8

assign r4 r5 to caller_r4 caller_r5 = stack_r45
assign r6 r7 to caller_r6 caller_r7 = stack_r67
assign r8 r9 to caller_r8 caller_r9 = stack_r89
assign r10 r11 to caller_r10 caller_r11 = stack_r1011
caller_r12 = stack_r12
caller_r14 = stack_r14

int32 result
result = 0
qpopreturn result
