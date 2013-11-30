reg128 r0
reg128 r1
reg128 r2
reg128 r3
reg128 r4

reg128 x01
reg128 x23
reg128 x4

reg128 y0
reg128 y12
reg128 y34
reg128 5y12
reg128 5y34
stack128 y0_stack
stack128 y12_stack
stack128 y34_stack
stack128 5y12_stack
stack128 5y34_stack

reg128 z0
reg128 z12
reg128 z34
reg128 5z12
reg128 5z34
stack128 z0_stack
stack128 z12_stack
stack128 z34_stack
stack128 5z12_stack
stack128 5z34_stack

stack128 two24

int32 ptr

reg128 c01
reg128 c23
reg128 d01
reg128 d23

reg128 t0
reg128 t1
reg128 t2
reg128 t3
reg128 t4
reg128 mask

reg128 u0
reg128 u1
reg128 u2
reg128 u3
reg128 u4

reg128 v01
reg128 mid
reg128 v23
reg128 v4

int32 len

qpushenter crypto_onetimeauth_poly1305_neon2_blocks

len = input_3

# XXX: change storage format so the y,z loads are aligned
new y0
y0  = mem64[input_1]y0[1]; input_1 += 8
y12 = mem128[input_1]; input_1 += 16
y34 = mem128[input_1]; input_1 += 16
input_1 += 8
new z0
z0  = mem64[input_1]z0[1]; input_1 += 8
z12 = mem128[input_1]; input_1 += 16
z34 = mem128[input_1]; input_1 += 16

2x mask = 0xffffffff
2x u4 = 0xff

x01 aligned= mem128[input_0];input_0+=16
x23 aligned= mem128[input_0];input_0+=16
x4  aligned= mem64[input_0]x4[1]
input_0 -= 32

2x mask unsigned>>=6
2x u4 unsigned>>= 7

4x 5y12 = y12 << 2
4x 5y34 = y34 << 2
4x 5y12 += y12
4x 5y34 += y34

2x u4 <<= 24

4x 5z12 = z12 << 2
4x 5z34 = z34 << 2
4x 5z12 += z12
4x 5z34 += z34

new two24
new y0_stack
new y12_stack
new y34_stack
new 5y12_stack
new 5y34_stack
new z0_stack
new z12_stack
new z34_stack
new 5z12_stack
new 5z34_stack

ptr = &two24
mem128[ptr] aligned= u4

r4 = u4
r0 = u4

ptr = &y0_stack
mem128[ptr] aligned= y0
ptr = &y12_stack
mem128[ptr] aligned= y12
ptr = &y34_stack
mem128[ptr] aligned= y34
ptr = &z0_stack
mem128[ptr] aligned= z0
ptr = &z12_stack
mem128[ptr] aligned= z12
ptr = &z34_stack
mem128[ptr] aligned= z34
ptr = &5y12_stack
mem128[ptr] aligned= 5y12
ptr = &5y34_stack
mem128[ptr] aligned= 5y34
ptr = &5z12_stack
mem128[ptr] aligned= 5z12
ptr = &5z34_stack
mem128[ptr] aligned= 5z34

                      unsigned>? len - 64
goto below64bytes if !unsigned>

input_2 += 32

mainloop2:
  
  c01 = mem128[input_2];input_2+=16 # m0 m1 m2 m3
  c23 = mem128[input_2];input_2+=16 # m4 m5 m6 m7

r4[0,1] += x01[0] unsigned*  z34[2];  r4[2,3] += x01[1] unsigned*  z34[3]

  ptr = &z12_stack
  z12 aligned= mem128[ptr]
r4[0,1] += x01[2] unsigned* z34[0];  r4[2,3] += x01[3] unsigned* z34[1]

  ptr = &z0_stack
  z0 aligned= mem128[ptr]
r4[0,1] += x23[0] unsigned* z12[2];  r4[2,3] += x23[1] unsigned* z12[3]

  c01 c23 = c01[0]c01[1]c01[2]c23[2]c23[0]c23[1]c01[3]c23[3]
r4[0,1] += x23[2] unsigned* z12[0];  r4[2,3] += x23[3] unsigned* z12[1]

r4[0,1] +=  x4[0] unsigned* z0[0];  r4[2,3] +=  x4[1] unsigned* z0[1]

r3[0,1] = c23[2]<<18; r3[2,3] = c23[3]<<18 # m3<<18 0 m7<<18 0
  c01 c23 = c01[0]c23[0]c01[2]c01[3]c01[1]c23[1]c23[2]c23[3]
# c01 c23 is now m0 m4 m2 m6; m1 m5 m3 m7

r3[0,1] += x01[0] unsigned* z34[0];   r3[2,3] += x01[1] unsigned* z34[1]

r3[0,1] += x01[2] unsigned* z12[2];   r3[2,3] += x01[3] unsigned* z12[3]

  r0 = r0[1]c01[0]r0[2,3] # 0 m0 2^24 0
r3[0,1] += x23[0] unsigned* z12[0];   r3[2,3] += x23[1] unsigned* z12[1]
								input_2 -= 64

r3[0,1] += x23[2] unsigned* z0[0];   r3[2,3] += x23[3] unsigned* z0[1]

  ptr = &5z34_stack
  5z34 aligned= mem128[ptr]
r3[0,1] +=  x4[0] unsigned*  5z34[2]; r3[2,3] +=  x4[1] unsigned*  5z34[3]

  r0 = r0[1]r0[0]r0[3]r0[2] # m0 0 0 2^24
  r2[0,1] = c01[2]<<12; r2[2,3] = c01[3]<<12 # m2<<12 0 m6<<12 0
  		d01 = mem128[input_2];input_2+=16 # m0 m1 m2 m3
r2[0,1] += x01[0] unsigned* z12[2];   r2[2,3] += x01[1] unsigned* z12[3]
r2[0,1] += x01[2] unsigned* z12[0];   r2[2,3] += x01[3] unsigned* z12[1]
r2[0,1] += x23[0] unsigned* z0[0];   r2[2,3] += x23[1] unsigned* z0[1]
r2[0,1] += x23[2] unsigned*  5z34[2]; r2[2,3] += x23[3] unsigned*  5z34[3]
r2[0,1] +=  x4[0] unsigned* 5z34[0]; r2[2,3] +=  x4[1] unsigned* 5z34[1]

  r0 = r0[0,1]c01[1]r0[2] # m0 0 m4 0
  r1[0,1] = c23[0]<<6; r1[2,3] = c23[1]<<6 # m1<<6 0 m5<<6 0
r1[0,1] += x01[0] unsigned* z12[0];   r1[2,3] += x01[1] unsigned* z12[1]
r1[0,1] += x01[2] unsigned* z0[0];   r1[2,3] += x01[3] unsigned* z0[1]
r1[0,1] += x23[0] unsigned*  5z34[2]; r1[2,3] += x23[1] unsigned*  5z34[3]
r1[0,1] += x23[2] unsigned* 5z34[0]; r1[2,3] += x23[3] unsigned* 5z34[1]
ptr = &5z12_stack
5z12 aligned= mem128[ptr]
r1[0,1] +=  x4[0] unsigned* 5z12[2]; r1[2,3] +=  x4[1] unsigned* 5z12[3]
  		d23 = mem128[input_2];input_2+=16 # m4 m5 m6 m7
  		input_2 += 32

r0[0,1] +=  x4[0] unsigned* 5z12[0]; r0[2,3] +=  x4[1] unsigned* 5z12[1]
r0[0,1] += x23[0] unsigned* 5z34[0]; r0[2,3] += x23[1] unsigned* 5z34[1]
  		d01 d23 = d01[0] d23[0] d01[1] d23[1] # m0 m1 m4 m5; m2 m3 m6 m7
r0[0,1] += x23[2] unsigned* 5z12[2]; r0[2,3] += x23[3] unsigned* 5z12[3]
r0[0,1] += x01[0] unsigned* z0[0];   r0[2,3] += x01[1] unsigned* z0[1]

  		new mid
  		2x v4 = d23 unsigned>> 40  # m2m3>>40, m6m7>>40
  		mid = d01[1]d23[0] mid[2,3] # m1 m2 ? ?

  		new v23
  		v23[2] = d23[0,1] unsigned>> 14; v23[3] = d23[2,3] unsigned>> 14
  		mid = mid[0,1] d01[3]d23[2] # m1 m2 m5 m6

  		new v01
  		v01[2] = d01[0,1] unsigned>> 26; v01[3] = d01[2,3] unsigned>> 26
  		v01 = d01[1]d01[0] v01[2,3] # m1, m0, m0m1>>26, m4m5>>26

r0[0,1] += x01[2] unsigned*  5z34[2]; r0[2,3] += x01[3] unsigned*  5z34[3]

  		v01 = v01[1]d01[2] v01[2,3] # m0, m4, m0m1>>26, m4m5>>26

  		v23[0] = mid[0,1] unsigned>> 20; v23[1] = mid[2,3] unsigned>> 20
  		# v23: m1m2>>20, m5m6>>20, m2m3>>14, m6m7>>14

  		v4 = v4[0]v4[2]v4[1]v4[3]  # m2m3>>40, m6m7>>40, 0, 0

  		4x v01 &= 0x03ffffff
ptr = &y34_stack
y34 aligned= mem128[ptr]
  		4x v23 &= 0x03ffffff
ptr = &y12_stack
y12 aligned= mem128[ptr]
  		4x v4 |= 0x01000000
ptr = &y0_stack
y0 aligned= mem128[ptr]

r4[0,1] += v01[0] unsigned*  y34[2];  r4[2,3] += v01[1] unsigned*  y34[3]
r4[0,1] += v01[2] unsigned* y34[0];  r4[2,3] += v01[3] unsigned* y34[1]
r4[0,1] += v23[0] unsigned* y12[2];  r4[2,3] += v23[1] unsigned* y12[3]
r4[0,1] += v23[2] unsigned* y12[0];  r4[2,3] += v23[3] unsigned* y12[1]
r4[0,1] +=  v4[0] unsigned* y0[0];  r4[2,3] +=  v4[1] unsigned* y0[1]

ptr = &5y34_stack
5y34 aligned= mem128[ptr]
r3[0,1] += v01[0] unsigned* y34[0];   r3[2,3] += v01[1] unsigned* y34[1]
r3[0,1] += v01[2] unsigned* y12[2];   r3[2,3] += v01[3] unsigned* y12[3]
r3[0,1] += v23[0] unsigned* y12[0];   r3[2,3] += v23[1] unsigned* y12[1]
r3[0,1] += v23[2] unsigned* y0[0];   r3[2,3] += v23[3] unsigned* y0[1]
r3[0,1] +=  v4[0] unsigned*  5y34[2]; r3[2,3] +=  v4[1] unsigned*  5y34[3]

ptr = &5y12_stack
5y12 aligned= mem128[ptr]
r0[0,1] +=  v4[0] unsigned* 5y12[0]; r0[2,3] +=  v4[1] unsigned* 5y12[1]
r0[0,1] += v23[0] unsigned* 5y34[0]; r0[2,3] += v23[1] unsigned* 5y34[1]
r0[0,1] += v23[2] unsigned* 5y12[2]; r0[2,3] += v23[3] unsigned* 5y12[3]
r0[0,1] += v01[0] unsigned* y0[0];   r0[2,3] += v01[1] unsigned* y0[1]
r0[0,1] += v01[2] unsigned*  5y34[2]; r0[2,3] += v01[3] unsigned*  5y34[3]

r1[0,1] += v01[0] unsigned* y12[0];   r1[2,3] += v01[1] unsigned* y12[1]
r1[0,1] += v01[2] unsigned* y0[0];   r1[2,3] += v01[3] unsigned* y0[1]
r1[0,1] += v23[0] unsigned*  5y34[2]; r1[2,3] += v23[1] unsigned*  5y34[3]
r1[0,1] += v23[2] unsigned* 5y34[0]; r1[2,3] += v23[3] unsigned* 5y34[1]
r1[0,1] +=  v4[0] unsigned* 5y12[2]; r1[2,3] +=  v4[1] unsigned* 5y12[3]

r2[0,1] += v01[0] unsigned* y12[2];   r2[2,3] += v01[1] unsigned* y12[3]
r2[0,1] += v01[2] unsigned* y12[0];   r2[2,3] += v01[3] unsigned* y12[1]
r2[0,1] += v23[0] unsigned* y0[0];   r2[2,3] += v23[1] unsigned* y0[1]
r2[0,1] += v23[2] unsigned*  5y34[2]; r2[2,3] += v23[3] unsigned*  5y34[3]
r2[0,1] +=  v4[0] unsigned* 5y34[0]; r2[2,3] +=  v4[1] unsigned* 5y34[1]

				ptr = &two24
2x t1 = r0 unsigned>> 26
  				len -= 64
   r0 &= mask
2x r1 += t1
		2x t4 = r3 unsigned>> 26
		   r3 &= mask
		2x x4 = r4 + t4
				r4 aligned= mem128[ptr]
2x t2 = r1 unsigned>> 26
   r1 &= mask
		2x t0 = x4 unsigned>> 26
2x r2 += t2
		   x4 &= mask
		2x x01 = r0 + t0
				r0 aligned= mem128[ptr]
				ptr = &z34_stack
		2x t0 <<= 2
2x t3 = r2 unsigned>> 26
		2x x01 += t0
				z34 aligned= mem128[ptr]
   x23 = r2 & mask
2x r3 += t3
								input_2 += 32
		2x t1 = x01 unsigned>> 26
						x23 = x23[0,2,1,3]
		   x01 = x01 & mask
		2x r1 += t1
2x t4 = r3 unsigned>> 26
						x01 = x01[0,2,1,3]
   r3 &= mask
						r1 = r1[0,2,1,3]
2x x4 += t4
						r3 = r3[0,2,1,3]
						x01 = x01[0,1] r1[0,1]
						x23 = x23[0,1] r3[0,1]
						x4 = x4[0,2,1,3]

						# before these final permutations:
						# x01 had f0,0,f0',0
						# r1  had f1,0,f1',0
						# x23 had f2,0,f2',0
						# r3  had f3,0,f3',0
						# x4  had f4,0,f4',0

						# x01 has f0,f0',f1,f1'
						# x23 has f2,f2',f3,f3'
						# x4  has f4,f4',?,?

                  unsigned>? len - 64
goto mainloop2 if unsigned>

input_2 -= 32

below64bytes:

             unsigned>? len - 32
goto end if !unsigned>

mainloop:

  new r0

ptr = &two24
r4 aligned= mem128[ptr]
u4 aligned= mem128[ptr]

  c01 = mem128[input_2];input_2+=16 # m0 m1 m2 m3
r4[0,1] += x01[0] unsigned*  y34[2];  r4[2,3] += x01[1] unsigned*  y34[3]
  c23 = mem128[input_2];input_2+=16 # m4 m5 m6 m7
r4[0,1] += x01[2] unsigned* y34[0];  r4[2,3] += x01[3] unsigned* y34[1]
  r0 = u4[1]c01[0]r0[2,3] # 0 m0 ? ?
r4[0,1] += x23[0] unsigned* y12[2];  r4[2,3] += x23[1] unsigned* y12[3]
  r0 = r0[0,1]u4[1]c23[0] # 0 m0 0 m4
r4[0,1] += x23[2] unsigned* y12[0];  r4[2,3] += x23[3] unsigned* y12[1]
  r0 = r0[1]r0[0]r0[3]r0[2] # m0 0 m4 0
r4[0,1] +=  x4[0] unsigned* y0[0];  r4[2,3] +=  x4[1] unsigned* y0[1]

r0[0,1] +=  x4[0] unsigned* 5y12[0]; r0[2,3] +=  x4[1] unsigned* 5y12[1]
r0[0,1] += x23[0] unsigned* 5y34[0]; r0[2,3] += x23[1] unsigned* 5y34[1]
r0[0,1] += x23[2] unsigned* 5y12[2]; r0[2,3] += x23[3] unsigned* 5y12[3]
  c01 c23 = c01[0]c23[0]c01[2]c23[2]c01[1]c23[1]c01[3]c23[3] # m0 m4 m2 m6; m1 m5 m3 m7
r0[0,1] += x01[0] unsigned* y0[0];   r0[2,3] += x01[1] unsigned* y0[1]

  r3[0,1] = c23[2]<<18; r3[2,3] = c23[3]<<18 # m3<<18 0 m7<<18 0
r0[0,1] += x01[2] unsigned*  5y34[2]; r0[2,3] += x01[3] unsigned*  5y34[3]

r3[0,1] += x01[0] unsigned* y34[0];   r3[2,3] += x01[1] unsigned* y34[1]
r3[0,1] += x01[2] unsigned* y12[2];   r3[2,3] += x01[3] unsigned* y12[3]
r3[0,1] += x23[0] unsigned* y12[0];   r3[2,3] += x23[1] unsigned* y12[1]
r3[0,1] += x23[2] unsigned* y0[0];   r3[2,3] += x23[3] unsigned* y0[1]

  r1[0,1] = c23[0]<<6; r1[2,3] = c23[1]<<6 # m1<<6 0 m5<<6 0
r3[0,1] +=  x4[0] unsigned*  5y34[2]; r3[2,3] +=  x4[1] unsigned*  5y34[3]

r1[0,1] += x01[0] unsigned* y12[0];   r1[2,3] += x01[1] unsigned* y12[1]
r1[0,1] += x01[2] unsigned* y0[0];   r1[2,3] += x01[3] unsigned* y0[1]
r1[0,1] += x23[0] unsigned*  5y34[2]; r1[2,3] += x23[1] unsigned*  5y34[3]
r1[0,1] += x23[2] unsigned* 5y34[0]; r1[2,3] += x23[3] unsigned* 5y34[1]

  r2[0,1] = c01[2]<<12; r2[2,3] = c01[3]<<12 # m2<<12 0 m6<<12 0
r1[0,1] +=  x4[0] unsigned* 5y12[2]; r1[2,3] +=  x4[1] unsigned* 5y12[3]

r2[0,1] += x01[0] unsigned* y12[2];   r2[2,3] += x01[1] unsigned* y12[3]
r2[0,1] += x01[2] unsigned* y12[0];   r2[2,3] += x01[3] unsigned* y12[1]
r2[0,1] += x23[0] unsigned* y0[0];   r2[2,3] += x23[1] unsigned* y0[1]
r2[0,1] += x23[2] unsigned*  5y34[2]; r2[2,3] += x23[3] unsigned*  5y34[3]
r2[0,1] +=  x4[0] unsigned* 5y34[0]; r2[2,3] +=  x4[1] unsigned* 5y34[1]

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
		   x01 = r0 & mask
		2x r1 += t1
2x t4 = r3 unsigned>> 26
   r3 &= mask
2x x4 = r4 + t4

  len -= 32

x01 = x01[0,2,1,3]
x23 = x23[0,2,1,3]
r1 = r1[0,2,1,3]
r3 = r3[0,2,1,3]
x4 = x4[0,2,1,3]
x01 = x01[0,1] r1[0,1]
x23 = x23[0,1] r3[0,1]

unsigned>? len - 32
goto mainloop if unsigned>

end:

mem128[input_0] = x01;input_0+=16
mem128[input_0] = x23;input_0+=16
mem64[input_0] = x4[0]

len = len
qpopreturn len
