int64 x
int64 arg2
int64 arg3
int64 arg4

input x
input arg2
input arg3
input arg4

int64 i
int64 a
int64 m
int64 out
int64 bytes
stack64 ctarget
stack512 tmp

stack64 bytes_stack
stack64 out_stack
stack64 m_stack
stack64 x_stack

int64 z0
int64 z1
int64 z2
int64 z3
int64 z4
int64 z5
int64 z6
int64 z7
int64 z8
int64 z9
int64 z10
int64 z11
int64 z12
int64 z13
int64 z14
int64 z15

int64 u0
int64 u1
int64 u2
int64 u3
int64 u4
int64 u5
int64 u6
int64 u7
int64 u8
int64 u9
int64 u10
int64 u11
int64 u12
int64 u13
int64 u14
int64 u15

int64 y0
int64 y1
int64 y2
int64 y3
int64 y4
int64 y5
int64 y6
int64 y7
int64 y8
int64 y9
int64 y10
int64 y11
int64 y12
int64 y13
int64 y14
int64 y15

int64 x0
int64 x1
int64 x2
int64 x3
int64 x4
int64 x5
int64 x6
int64 x7
int64 x8
int64 x9
int64 x10
int64 x11
int64 x12
int64 x13
int64 x14
int64 x15

int64 q0
int64 q1
int64 q2
int64 q3
int64 q4
int64 q5
int64 q6
int64 q7
int64 q8
int64 q9
int64 q10
int64 q11
int64 q12
int64 q13
int64 q14
int64 q15

int64 m0
int64 m1
int64 m2
int64 m3
int64 m4
int64 m5
int64 m6
int64 m7
int64 m8
int64 m9
int64 m10
int64 m11
int64 m12
int64 m13
int64 m14
int64 m15

enter ECRYPT_init
leave


enter ECRYPT_ivsetup
  x6 = *(uint32 *) (arg2 + 0)
  x8 = 0
  x7 = *(uint32 *) (arg2 + 4)
  x9 = 0
  x += 24
  *(swapendian int32 *) x = x6
  x += 4
  *(swapendian int32 *) x = x7
  x += 4
  *(int32 *) (x + 0) = x8
  x += 4
  *(int32 *) (x + 0) = x9
leave


enter ECRYPT_keysetup

                 unsigned>? arg3 - 128
goto kbits256 if unsigned>

kbits128:

  x1 = *(uint32 *) (arg2 + 0)
  x0 = 1634760805 & 0xfffffc00
  x2 = *(uint32 *) (arg2 + 4)
  x5 = 824206446 & 0xfffffc00
  x3 = *(uint32 *) (arg2 + 8)
  x10 = 2036477238 & 0xfffffc00
  x4 = *(uint32 *) (arg2 + 12)
  x15 = 1797285236 & 0xfffffc00
  x11 = *(uint32 *) (arg2 + 0)
  x0 |= 1634760805 & 0x3ff
  x12 = *(uint32 *) (arg2 + 4)
  x5 |= 824206446 & 0x3ff
  x13 = *(uint32 *) (arg2 + 8)
  x10 |= 2036477238 & 0x3ff
  x14 = *(uint32 *) (arg2 + 12)
  x15 |= 1797285236 & 0x3ff

goto storekey

kbits256:

  x1 = *(uint32 *) (arg2 + 0)
  x0 = 1634760805 & 0xfffffc00
  x2 = *(uint32 *) (arg2 + 4)
  x5 = 857760878 & 0xfffffc00
  x3 = *(uint32 *) (arg2 + 8)
  x10 = 2036477234 & 0xfffffc00
  x4 = *(uint32 *) (arg2 + 12)
  x15 = 1797285236 & 0xfffffc00
  x11 = *(uint32 *) (arg2 + 16)
  x0 |= 1634760805 & 0x3ff
  x12 = *(uint32 *) (arg2 + 20)
  x5 |= 857760878 & 0x3ff
  x13 = *(uint32 *) (arg2 + 24)
  x10 |= 2036477234 & 0x3ff
  x14 = *(uint32 *) (arg2 + 28)
  x15 |= 1797285236 & 0x3ff

storekey:

  *(int32 *) (x + 0) = x0
  x += 4
  *(swapendian int32 *) x = x1
  x += 4
  *(swapendian int32 *) x = x2
  x += 4
  *(swapendian int32 *) x = x3
  x += 4
  *(swapendian int32 *) x = x4
  x += 4
  *(int32 *) (x + 0) = x5
  x += 20
  *(int32 *) (x + 0) = x10
  x += 4
  *(swapendian int32 *) x = x11
  x += 4
  *(swapendian int32 *) x = x12
  x += 4
  *(swapendian int32 *) x = x13
  x += 4
  *(swapendian int32 *) x = x14
  x += 4
  *(int32 *) (x + 0) = x15

leave


enter ECRYPT_keystream_bytes

bytes = arg3
m = arg2
out = arg2

              unsigned>? bytes - 0
goto done if !unsigned>

  a = 0
  i = bytes
  zeroloop:
    *(int8 *) (out + 0) = a
    out += 1
                   unsigned>? i -= 1
  goto zeroloop if unsigned>
  out -= bytes

goto bytesatleast1

enter ECRYPT_decrypt_bytes

bytes = arg4
m = arg2
out = arg3

              unsigned>? bytes - 0
goto done if !unsigned>
goto bytesatleast1

enter ECRYPT_encrypt_bytes

bytes = arg4
m = arg2
out = arg3

              unsigned>? bytes - 0
goto done if !unsigned>
bytesatleast1:

                          unsigned<? bytes - 64
  goto bytesatleast64 if !unsigned<

    ctarget = out
    out = &tmp
    i = 0
    mcopyloop:
      a = *(int8 *) (m + i)
      *(int8 *) (out + i) = a
      i += 1
                      unsigned<? i - bytes
    goto mcopyloop if unsigned<
    m = &tmp

  bytesatleast64:

    x0 = *(uint32 *) (x + 0)
    x1 = *(uint32 *) (x + 4)
    x2 = *(uint32 *) (x + 8)
    x3 = *(uint32 *) (x + 12)
    x4 = *(uint32 *) (x + 16)
    x5 = *(uint32 *) (x + 20)
    x6 = *(uint32 *) (x + 24)
    x7 = *(uint32 *) (x + 28)
    x8 = *(uint32 *) (x + 32)
    x9 = *(uint32 *) (x + 36)
    x10 = *(uint32 *) (x + 40)
    x11 = *(uint32 *) (x + 44)
    x12 = *(uint32 *) (x + 48)
    x13 = *(uint32 *) (x + 52)
    x14 = *(uint32 *) (x + 56)
    x15 = *(uint32 *) (x + 60)

    i = 20

    bytes_stack = bytes
    out_stack = out
    m_stack = m
    x_stack = x

    mainloop:



y4 = x0 + x12
		y9 = x5 + x1
z4 = (uint32) y4 << 7
				y14 = x10 + x6
y4 = (uint32) y4 >> 25
						y3 = x15 + x11
		z9 = (uint32) y9 << 7
y4 |= z4
		y9 = (uint32) y9 >> 25
x4 ^= y4
				z14 = (uint32) y14 << 7
		y9 |= z9
				y14 = (uint32) y14 >> 25
y8 = x4 + x0
						z3 = (uint32) y3 << 7
		x9 ^= y9
						y3 = (uint32) y3 >> 25
				y14 |= z14
z8 = (uint32) y8 << 9
		y13 = x9 + x5
y8 = (uint32) y8 >> 23
				x14 ^= y14
						y3 |= z3
				y2 = x14 + x10
		z13 = (uint32) y13 << 9
						x3 ^= y3
		y13 = (uint32) y13 >> 23
y8 |= z8
				z2 = (uint32) y2 << 9
						y7 = x3 + x15
				y2 = (uint32) y2 >> 23
x8 ^= y8
		y13 |= z13
y12 = x8 + x4
						z7 = (uint32) y7 << 9
		x13 ^= y13
						y7 = (uint32) y7 >> 23
				y2 |= z2
z12 = (uint32) y12 << 13
		y1 = x13 + x9
y12 = (uint32) y12 >> 19
				x2 ^= y2
						y7 |= z7
				y6 = x2 + x14
		z1 = (uint32) y1 << 13
						x7 ^= y7
		y1 = (uint32) y1 >> 19
y12 |= z12
				z6 = (uint32) y6 << 13
						y11 = x7 + x3
				y6 = (uint32) y6 >> 19
x12 ^= y12
		y1 |= z1
y0 = x12 + x8
						z11 = (uint32) y11 << 13
		x1 ^= y1
						y11 = (uint32) y11 >> 19
				y6 |= z6
z0 = (uint32) y0 << 18
		y5 = x1 + x13
y0 = (uint32) y0 >> 14
				x6 ^= y6
						y11 |= z11
				y10 = x6 + x2
		z5 = (uint32) y5 << 18
						x11 ^= y11
		y5 = (uint32) y5 >> 14
y0 |= z0
				z10 = (uint32) y10 << 18
						y15 = x11 + x7
				y10 = (uint32) y10 >> 14
x0 ^= y0
		y5 |= z5
u1 = x0 + x3
						z15 = (uint32) y15 << 18
		x5 ^= y5
						y15 = (uint32) y15 >> 14
				y10 |= z10
z1 = (uint32) u1 << 7
		u6 = x5 + x4
u1 = (uint32) u1 >> 25
				x10 ^= y10
						y15 |= z15
				u11 = x10 + x9
		z6 = (uint32) u6 << 7
						x15 ^= y15
		u6 = (uint32) u6 >> 25
u1 |= z1
				z11 = (uint32) u11 << 7
						u12 = x15 + x14
				u11 = (uint32) u11 >> 25
		u6 |= z6
x1 ^= u1
						z12 = (uint32) u12 << 7
		x6 ^= u6
						u12 = (uint32) u12 >> 25
				u11 |= z11
u2 = x1 + x0
		u7 = x6 + x5
z2 = (uint32) u2 << 9
				x11 ^= u11
u2 = (uint32) u2 >> 23
						u12 |= z12
		z7 = (uint32) u7 << 9
				u8 = x11 + x10
		u7 = (uint32) u7 >> 23
						x12 ^= u12
				z8 = (uint32) u8 << 9
u2 |= z2
				u8 = (uint32) u8 >> 23
						u13 = x12 + x15
		u7 |= z7
x2 ^= u2
						z13 = (uint32) u13 << 9
		x7 ^= u7
						u13 = (uint32) u13 >> 23
				u8 |= z8
u3 = x2 + x1
		u4 = x7 + x6
z3 = (uint32) u3 << 13
				x8 ^= u8
		z4 = (uint32) u4 << 13
						u13 |= z13
u3 = (uint32) u3 >> 19
				u9 = x8 + x11
		u4 = (uint32) u4 >> 19
						x13 ^= u13
				z9 = (uint32) u9 << 13
u3 |= z3
				u9 = (uint32) u9 >> 19
						u14 = x13 + x12
		u4 |= z4
x3 ^= u3
						z14 = (uint32) u14 << 13
		x4 ^= u4
						u14 = (uint32) u14 >> 19
				u9 |= z9
u0 = x3 + x2
		u5 = x4 + x7
z0 = (uint32) u0 << 18
				x9 ^= u9
		z5 = (uint32) u5 << 18
						u14 |= z14
u0 = (uint32) u0 >> 14
				u10 = x9 + x8
		u5 = (uint32) u5 >> 14
						x14 ^= u14
				z10 = (uint32) u10 << 18
u0 |= z0
				u10 = (uint32) u10 >> 14
						u15 = x14 + x13
		u5 |= z5
x0 ^= u0
						z15 = (uint32) u15 << 18
		x5 ^= u5
						u15 = (uint32) u15 >> 14
                 unsigned>? i -= 2
				u10 |= z10
						u15 |= z15
				x10 ^= u10
						x15 ^= u15


goto mainloop if unsigned>

  x = x_stack

  q0 = *(uint32 *) (x + 0)
  q1 = *(uint32 *) (x + 4)
  q2 = *(uint32 *) (x + 8)
  q3 = *(uint32 *) (x + 12)
  x0 += q0
  q4 = *(uint32 *) (x + 16)
  x1 += q1
  q5 = *(uint32 *) (x + 20)
  x2 += q2
  q6 = *(uint32 *) (x + 24)
  x3 += q3
  q7 = *(uint32 *) (x + 28)
  x4 += q4
  q8 = *(uint32 *) (x + 32)
  x5 += q5
  q9 = *(uint32 *) (x + 36)
  x6 += q6
  q10 = *(uint32 *) (x + 40)
  x7 += q7
  q11 = *(uint32 *) (x + 44)
  x8 += q8
  q8 += 1
  *(uint32 *) (x + 32) = q8
  q8 = (uint64) q8 >> 32
  q12 = *(uint32 *) (x + 48)
  x9 += q9
  q9 += q8
  *(uint32 *) (x + 36) = q9
  q13 = *(uint32 *) (x + 52)
  x10 += q10
  q14 = *(uint32 *) (x + 56)
  x11 += q11
  q15 = *(uint32 *) (x + 60)
  x12 += q12
  x13 += q13
  x14 += q14
  x15 += q15

  m = m_stack

  m0 = *(swapendian uint32 *) m
  m += 4
  m1 = *(swapendian uint32 *) m
  m += 4
  m2 = *(swapendian uint32 *) m
  m += 4
  m3 = *(swapendian uint32 *) m
  m += 4
  x0 ^= m0
  m4 = *(swapendian uint32 *) m
  m += 4
  x1 ^= m1
  m5 = *(swapendian uint32 *) m
  m += 4
  x2 ^= m2
  m6 = *(swapendian uint32 *) m
  m += 4
  x3 ^= m3
  m7 = *(swapendian uint32 *) m
  m += 4
  x4 ^= m4
  m8 = *(swapendian uint32 *) m
  m += 4
  x5 ^= m5
  m9 = *(swapendian uint32 *) m
  m += 4
  x6 ^= m6
  m10 = *(swapendian uint32 *) m
  m += 4
  x7 ^= m7
  m11 = *(swapendian uint32 *) m
  m += 4
  x8 ^= m8
  m12 = *(swapendian uint32 *) m
  m += 4
  x9 ^= m9
  m13 = *(swapendian uint32 *) m
  m += 4
  x10 ^= m10
  m14 = *(swapendian uint32 *) m
  m += 4
  x11 ^= m11
  m15 = *(swapendian uint32 *) m
  m += 4
  x12 ^= m12
  x13 ^= m13
  x14 ^= m14
  x15 ^= m15

  out = out_stack
  *(swapendian uint32 *) out = x0
  out += 4
  *(swapendian uint32 *) out = x1
  out += 4
  *(swapendian uint32 *) out = x2
  out += 4
  *(swapendian uint32 *) out = x3
  out += 4
  *(swapendian uint32 *) out = x4
  out += 4
  *(swapendian uint32 *) out = x5
  out += 4
  *(swapendian uint32 *) out = x6
  out += 4
  *(swapendian uint32 *) out = x7
  out += 4
  *(swapendian uint32 *) out = x8
  out += 4
  *(swapendian uint32 *) out = x9
  out += 4
  *(swapendian uint32 *) out = x10
  out += 4
  *(swapendian uint32 *) out = x11
  out += 4
  *(swapendian uint32 *) out = x12
  out += 4
  *(swapendian uint32 *) out = x13
  out += 4
  *(swapendian uint32 *) out = x14
  out += 4
  *(swapendian uint32 *) out = x15
  out += 4

  bytes = bytes_stack
                        unsigned>? bytes -= 64
  goto bytesatleast1 if unsigned>
  goto done if !unsigned<

    m = ctarget
    bytes += 64
    out -= 64
    i = 0
    ccopyloop:
      a = *(int8 *) (out + i)
      *(int8 *) (m + i) = a
      i += 1
                      unsigned<? i - bytes
    goto ccopyloop if unsigned<

done:
leave
