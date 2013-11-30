reg128 r0
reg128 r1
reg128 r2
reg128 r3
reg128 r4

reg128 x01
reg128 x23
reg128 x4

reg128 y01
reg128 y23
reg128 y4

reg128 _5y01
reg128 _5y23
reg128 _5y4

reg128 c01
reg128 c23
reg128 c4

reg128 t0
reg128 t1
reg128 t2
reg128 t3
reg128 t4

reg128 mask

enter crypto_onetimeauth_poly1305_neon2_addmulmod

				2x mask = 0xffffffff
  y01 aligned= mem128[input_2];input_2+=16
4x _5y01 = y01 << 2
  y23 aligned= mem128[input_2];input_2+=16
4x _5y23 = y23 << 2
  y4  aligned= mem64[input_2]y4[1]
4x _5y4 = y4 << 2
  x01 aligned= mem128[input_1];input_1+=16
4x _5y01 += y01
  x23 aligned= mem128[input_1];input_1+=16
4x _5y23 += y23
4x _5y4 += y4
  c01 aligned= mem128[input_3];input_3+=16
4x x01 += c01
  c23 aligned= mem128[input_3];input_3+=16
4x x23 += c23
  x4  aligned= mem64[input_1]x4[1]
				2x mask unsigned>>=6
  c4  aligned= mem64[input_3]c4[1]
4x x4 += c4

r0[0,1]  = x01[0] unsigned* y01[0];   r0[2,3]  = x01[1] unsigned* y01[1]
r0[0,1] += x01[2] unsigned*  _5y4[0]; r0[2,3] += x01[3] unsigned*  _5y4[1]
r0[0,1] += x23[0] unsigned* _5y23[2]; r0[2,3] += x23[1] unsigned* _5y23[3]
r0[0,1] += x23[2] unsigned* _5y23[0]; r0[2,3] += x23[3] unsigned* _5y23[1]
r0[0,1] +=  x4[0] unsigned* _5y01[2]; r0[2,3] +=  x4[1] unsigned* _5y01[3]

r1[0,1]  = x01[0] unsigned* y01[2];   r1[2,3]  = x01[1] unsigned* y01[3]
r1[0,1] += x01[2] unsigned* y01[0];   r1[2,3] += x01[3] unsigned* y01[1]
r1[0,1] += x23[0] unsigned*  _5y4[0]; r1[2,3] += x23[1] unsigned*  _5y4[1]
r1[0,1] += x23[2] unsigned* _5y23[2]; r1[2,3] += x23[3] unsigned* _5y23[3]
r1[0,1] +=  x4[0] unsigned* _5y23[0]; r1[2,3] +=  x4[1] unsigned* _5y23[1]

r2[0,1]  = x01[0] unsigned* y23[0];   r2[2,3]  = x01[1] unsigned* y23[1]
r2[0,1] += x01[2] unsigned* y01[2];   r2[2,3] += x01[3] unsigned* y01[3]
r2[0,1] += x23[0] unsigned* y01[0];   r2[2,3] += x23[1] unsigned* y01[1]
r2[0,1] += x23[2] unsigned*  _5y4[0]; r2[2,3] += x23[3] unsigned*  _5y4[1]
r2[0,1] +=  x4[0] unsigned* _5y23[2]; r2[2,3] +=  x4[1] unsigned* _5y23[3]

r3[0,1]  = x01[0] unsigned* y23[2];   r3[2,3]  = x01[1] unsigned* y23[3]
r3[0,1] += x01[2] unsigned* y23[0];   r3[2,3] += x01[3] unsigned* y23[1]
r3[0,1] += x23[0] unsigned* y01[2];   r3[2,3] += x23[1] unsigned* y01[3]
r3[0,1] += x23[2] unsigned* y01[0];   r3[2,3] += x23[3] unsigned* y01[1]
r3[0,1] +=  x4[0] unsigned*  _5y4[0]; r3[2,3] +=  x4[1] unsigned*  _5y4[1]

r4[0,1]  = x01[0] unsigned*  y4[0];  r4[2,3]  = x01[1] unsigned*  y4[1]
r4[0,1] += x01[2] unsigned* y23[2];  r4[2,3] += x01[3] unsigned* y23[3]
r4[0,1] += x23[0] unsigned* y23[0];  r4[2,3] += x23[1] unsigned* y23[1]
r4[0,1] += x23[2] unsigned* y01[2];  r4[2,3] += x23[3] unsigned* y01[3]
r4[0,1] +=  x4[0] unsigned* y01[0];  r4[2,3] +=  x4[1] unsigned* y01[1]

2x t1 = r0 unsigned>> 26
   r0 &= mask
2x r1 += t1
                2x t4 = r3 unsigned>> 26
                   r3 &= mask
                2x r4 += t4
2x t2 = r1 unsigned>> 26
   r1 &= mask
                2x t0 = r4 unsigned>> 26
2x r2 += t2
                   r4 &= mask
                2x r0 += t0
                2x t0 <<= 2
2x t3 = r2 unsigned>> 26
                2x r0 += t0
   x23 = r2 & mask
2x r3 += t3
                2x t1 = r0 unsigned>> 26
				x23 = x23[0,2,1,3]
                   x01 = r0 & mask
                2x r1 += t1
2x t4 = r3 unsigned>> 26
				x01 = x01[0,2,1,3]
   r3 &= mask
				r1 = r1[0,2,1,3]
2x x4 = r4 + t4
				r3 = r3[0,2,1,3]
				x01 = x01[0,1] r1[0,1]
				x23 = x23[0,1] r3[0,1]
				x4 = x4[0,2,1,3]
mem128[input_0] aligned= x01;input_0+=16
mem128[input_0] aligned= x23;input_0+=16
mem64[input_0] aligned= x4[0]

return
