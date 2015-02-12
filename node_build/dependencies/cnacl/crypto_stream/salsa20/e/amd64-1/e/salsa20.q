int64 a
int64 b
int64 c
int64 d
int64 e

int64 A
int64 B
int64 C
int64 D
int64 E

int64 r
int64 s
int64 t
int64 u
int64 v

int64 R
int64 S
int64 T
int64 U
int64 V

int64 arg1
int64 arg2
int64 arg3
int64 arg4
input arg1
input arg2
input arg3
input arg4

int64 r11
int64 r12
int64 r13
int64 r14
int64 r15
int64 rbx
int64 rbp
caller r11
caller r12
caller r13
caller r14
caller r15
caller rbx
caller rbp
stack64 r11_stack
stack64 r12_stack
stack64 r13_stack
stack64 r14_stack
stack64 r15_stack
stack64 rbx_stack
stack64 rbp_stack


int64 k
int64 kbits
int64 iv

int64 i

stack64 x_backup
int64 x
stack64 m_backup
int64 m
stack64 out_backup
int64 out
stack64 bytes_backup
int64 bytes

int64 in0
int64 in2
int64 in4
int64 in6
int64 in8
int64 in10
int64 in12
int64 in14

int64 out0
int64 out2
int64 out4
int64 out6
int64 out8
int64 out10
int64 out12
int64 out14

stack64 x0
stack64 x1
stack64 x2
stack64 x3
stack64 x4
stack64 x5
stack64 x6
stack64 x7
stack64 x8
stack64 x9
stack64 x10
stack64 x11
stack64 x12
stack64 x13
stack64 x14
stack64 x15
stack64 j0
stack64 j2
stack64 j4
stack64 j6
stack64 j8
stack64 j10
stack64 j12
stack64 j14

stack512 tmp

int64 ctarget


enter ECRYPT_keystream_bytes

x = arg1
m = arg2
out = m
bytes = arg3

              unsigned>? bytes - 0
goto done if !unsigned>

a = 0
i = bytes
while (i) { *out++ = a; --i }
out -= bytes

goto start


enter ECRYPT_decrypt_bytes

x = arg1
m = arg2
out = arg3
bytes = arg4

              unsigned>? bytes - 0
goto done if !unsigned>

goto start


enter ECRYPT_encrypt_bytes

x = arg1
m = arg2
out = arg3
bytes = arg4

              unsigned>? bytes - 0
goto done if !unsigned>


start:

r11_stack = r11
r12_stack = r12
r13_stack = r13
r14_stack = r14
r15_stack = r15
rbx_stack = rbx
rbp_stack = rbp

in0 = *(uint64 *) (x + 0)
in2 = *(uint64 *) (x + 8)
in4 = *(uint64 *) (x + 16)
in6 = *(uint64 *) (x + 24)
in8 = *(uint64 *) (x + 32)
in10 = *(uint64 *) (x + 40)
in12 = *(uint64 *) (x + 48)
in14 = *(uint64 *) (x + 56)
j0 = in0
j2 = in2
j4 = in4
j6 = in6
j8 = in8
j10 = in10
j12 = in12
j14 = in14

x_backup = x


bytesatleast1:

                  unsigned<? bytes - 64
  goto nocopy if !unsigned<

    ctarget = out

    out = &tmp
    i = bytes
    while (i) { *out++ = *m++; --i }
    out = &tmp
    m = &tmp

  nocopy:

  out_backup = out
  m_backup = m
  bytes_backup = bytes

  in0 = j0
  in2 = j2
  in4 = j4
  in6 = j6
  in8 = j8
  in10 = j10
  in12 = j12
  in14 = j14
  x0 = in0
  x2 = in2
  x4 = in4
  x6 = in6
  x8 = in8
  x10 = in10
  x12 = in12
  x14 = in14
  (uint64) in0 >>= 32
  (uint64) in2 >>= 32
  (uint64) in4 >>= 32
  (uint64) in6 >>= 32
  (uint64) in8 >>= 32
  (uint64) in10 >>= 32
  (uint64) in12 >>= 32
  (uint64) in14 >>= 32
  x1 = in0
  x3 = in2
  x5 = in4
  x7 = in6
  x9 = in8
  x11 = in10
  x13 = in12
  x15 = in14

  i = 20
  mainloop:


a = x12
b = x0
c = x4
e = a + b
(uint32) e <<<= 7
c ^= e
x4 = c
d = x8
e = b + c
(uint32) e <<<= 9
d ^= e
x8 = d
c += d
(uint32) c <<<= 13
a ^= c
x12 = a
a += d
(uint32) a <<<= 18
b ^= a
x0 = b
		r = x1
		s = x5
		t = x9
		v = r + s
		(uint32) v <<<= 7
		t ^= v
		x9 = t
		u = x13
		v = s + t
		(uint32) v <<<= 9
		u ^= v
		x13 = u
		t += u
		(uint32) t <<<= 13
		r ^= t
		x1 = r
		r += u
		(uint32) r <<<= 18
		s ^= r
		x5 = s
				A = x6
				B = x10
				C = x14
				E = A + B
				(uint32) E <<<= 7
				C ^= E
				x14 = C
				D = x2
				E = B + C
				(uint32) E <<<= 9
				D ^= E
				x2 = D
				C += D
				(uint32) C <<<= 13
				A ^= C
				x6 = A
				A += D
				(uint32) A <<<= 18
				B ^= A
				x10 = B
						R = x11
						S = x15
						T = x3
						V = R + S
						(uint32) V <<<= 7
						T ^= V
						x3 = T
						U = x7
						V = S + T
						(uint32) V <<<= 9
						U ^= V
						x7 = U
						T += U
						(uint32) T <<<= 13
						R ^= T
						x11 = R
						R += U
						(uint32) R <<<= 18
						S ^= R
						x15 = S

a = x3
b = x0
c = x1
e = a + b
(uint32) e <<<= 7
c ^= e
x1 = c
d = x2
e = b + c
(uint32) e <<<= 9
d ^= e
x2 = d
c += d
(uint32) c <<<= 13
a ^= c
x3 = a
a += d
(uint32) a <<<= 18
b ^= a
x0 = b
		r = x4
		s = x5
		t = x6
		v = r + s
		(uint32) v <<<= 7
		t ^= v
		x6 = t
		u = x7
		v = s + t
		(uint32) v <<<= 9
		u ^= v
		x7 = u
		t += u
		(uint32) t <<<= 13
		r ^= t
		x4 = r
		r += u
		(uint32) r <<<= 18
		s ^= r
		x5 = s
				A = x9
				B = x10
				C = x11
				E = A + B
				(uint32) E <<<= 7
				C ^= E
				x11 = C
				D = x8
				E = B + C
				(uint32) E <<<= 9
				D ^= E
				x8 = D
				C += D
				(uint32) C <<<= 13
				A ^= C
				x9 = A
				A += D
				(uint32) A <<<= 18
				B ^= A
				x10 = B
						R = x14
						S = x15
						T = x12
						V = R + S
						(uint32) V <<<= 7
						T ^= V
						x12 = T
						U = x13
						V = S + T
						(uint32) V <<<= 9
						U ^= V
						x13 = U
						T += U
						(uint32) T <<<= 13
						R ^= T
						x14 = R
						R += U
						(uint32) R <<<= 18
						S ^= R
						x15 = S


                 unsigned>? i -= 2
goto mainloop if unsigned>



  out = out_backup
  m = m_backup

  in0 = j0
  out0 = in0
  (uint64) out0 >>= 32
  (uint32) in0 += x0
  (uint32) out0 += x1
  out0 <<= 32
  out0 ^= in0
  out0 ^= *(uint64 *) (m + 0)
  *(uint64 *) (out + 0) = out0

  in2 = j2
  out2 = in2
  (uint64) out2 >>= 32
  (uint32) in2 += x2
  (uint32) out2 += x3
  out2 <<= 32
  out2 ^= in2
  out2 ^= *(uint64 *) (m + 8)
  *(uint64 *) (out + 8) = out2

  in4 = j4
  out4 = in4
  (uint64) out4 >>= 32
  (uint32) in4 += x4
  (uint32) out4 += x5
  out4 <<= 32
  out4 ^= in4
  out4 ^= *(uint64 *) (m + 16)
  *(uint64 *) (out + 16) = out4

  in6 = j6
  out6 = in6
  (uint64) out6 >>= 32
  (uint32) in6 += x6
  (uint32) out6 += x7
  out6 <<= 32
  out6 ^= in6
  out6 ^= *(uint64 *) (m + 24)
  *(uint64 *) (out + 24) = out6

  in8 = j8
  out8 = in8
  (uint64) out8 >>= 32
  (uint32) in8 += x8
  (uint32) out8 += x9
  out8 <<= 32
  out8 ^= in8
  out8 ^= *(uint64 *) (m + 32)
  *(uint64 *) (out + 32) = out8

  in10 = j10
  out10 = in10
  (uint64) out10 >>= 32
  (uint32) in10 += x10
  (uint32) out10 += x11
  out10 <<= 32
  out10 ^= in10
  out10 ^= *(uint64 *) (m + 40)
  *(uint64 *) (out + 40) = out10

  in12 = j12
  out12 = in12
  (uint64) out12 >>= 32
  (uint32) in12 += x12
  (uint32) out12 += x13
  out12 <<= 32
  out12 ^= in12
  out12 ^= *(uint64 *) (m + 48)
  *(uint64 *) (out + 48) = out12

  in14 = j14
  out14 = in14
  (uint64) out14 >>= 32
  (uint32) in14 += x14
  (uint32) out14 += x15
  out14 <<= 32
  out14 ^= in14
  out14 ^= *(uint64 *) (m + 56)
  *(uint64 *) (out + 56) = out14


  bytes = bytes_backup

  in8 = j8
  in8 += 1
  j8 = in8

                         unsigned>? unsigned<? bytes - 64
  goto bytesatleast65 if unsigned>

    goto bytesatleast64 if !unsigned<
      m = out
      out = ctarget
      i = bytes
      while (i) { *out++ = *m++; --i }
    bytesatleast64:

    x = x_backup
    in8 = j8
    *(uint64 *) (x + 32) = in8

    r11 = r11_stack
    r12 = r12_stack
    r13 = r13_stack
    r14 = r14_stack
    r15 = r15_stack
    rbx = rbx_stack
    rbp = rbp_stack

    done:

    leave

  bytesatleast65:

  bytes -= 64
  out += 64
  m += 64
goto bytesatleast1


enter ECRYPT_init
leave


enter ECRYPT_keysetup

  k = arg2
  kbits = arg3
  x = arg1

  in0 = *(uint64 *) (k + 0)
  in2 = *(uint64 *) (k + 8)
  *(uint64 *) (x + 4) = in0
  *(uint64 *) (x + 12) = in2

                   unsigned<? kbits - 256
  goto kbits128 if unsigned<

  kbits256:

    in10 = *(uint64 *) (k + 16)
    in12 = *(uint64 *) (k + 24)
    *(uint64 *) (x + 44) = in10
    *(uint64 *) (x + 52) = in12

    in0 = 1634760805
    in4 = 857760878
    in10 = 2036477234
    in14 = 1797285236
    *(uint32 *) (x + 0) = in0
    *(uint32 *) (x + 20) = in4
    *(uint32 *) (x + 40) = in10
    *(uint32 *) (x + 60) = in14

  goto keysetupdone

  kbits128:

    in10 = *(uint64 *) (k + 0)
    in12 = *(uint64 *) (k + 8)
    *(uint64 *) (x + 44) = in10
    *(uint64 *) (x + 52) = in12

    in0 = 1634760805
    in4 = 824206446
    in10 = 2036477238
    in14 = 1797285236
    *(uint32 *) (x + 0) = in0
    *(uint32 *) (x + 20) = in4
    *(uint32 *) (x + 40) = in10
    *(uint32 *) (x + 60) = in14

  keysetupdone:

leave


enter ECRYPT_ivsetup

  iv = arg2
  x = arg1

  in6 = *(uint64 *) (iv + 0)
  in8 = 0
  *(uint64 *) (x + 24) = in6
  *(uint64 *) (x + 32) = in8

leave
