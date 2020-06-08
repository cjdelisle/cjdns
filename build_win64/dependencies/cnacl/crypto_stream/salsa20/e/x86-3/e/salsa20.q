int32 a
int32 p
int32 s
int32 t
int32 w
int32 r
int32 v

stack32 arg1
stack32 arg2
stack32 arg3
stack32 arg4
input arg1
input arg2
input arg3
input arg4

int32 eax
int32 ebx
int32 esi
int32 edi
int32 ebp
caller eax
caller ebx
caller esi
caller edi
caller ebp

int32 k
int32 kbits
int32 iv

int32 i

stack32 x_backup
int32 x
stack32 m_backup
int32 m
stack32 out_backup
int32 out
stack32 bytes_backup
int32 bytes

stack32 eax_stack
stack32 ebx_stack
stack32 esi_stack
stack32 edi_stack
stack32 ebp_stack

int32 in0
int32 in1
int32 in2
int32 in3
int32 in4
int32 in5
int32 in6
int32 in7
int32 in8
int32 in9
int32 in10
int32 in11
int32 in12
int32 in13
int32 in14
int32 in15

stack32 x0
stack32 x1
stack32 x2
stack32 x3
stack32 x4
stack32 x5
stack32 x6
stack32 x7
stack32 x8
stack32 x9
stack32 x10
stack32 x11
stack32 x12
stack32 x13
stack32 x14
stack32 x15
stack32 j0
stack32 j1
stack32 j2
stack32 j3
stack32 j4
stack32 j5
stack32 j6
stack32 j7
stack32 j8
stack32 j9
stack32 j10
stack32 j11
stack32 j12
stack32 j13
stack32 j14
stack32 j15

stack512 tmp

stack32 ctarget




enter ECRYPT_keystream_bytes

eax_stack = eax
ebx_stack = ebx
esi_stack = esi
edi_stack = edi
ebp_stack = ebp

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

x0 = a
x1 = a
x2 = a
x3 = a
x4 = a
x5 = a
x6 = a
x7 = a
x8 = a
x9 = a
x10 = a
x11 = a
x12 = a
x13 = a
x14 = a
x15 = a

goto start


enter ECRYPT_decrypt_bytes

eax_stack = eax
ebx_stack = ebx
esi_stack = esi
edi_stack = edi
ebp_stack = ebp

x = arg1
m = arg2
out = arg3
bytes = arg4

              unsigned>? bytes - 0
goto done if !unsigned>

goto start


enter ECRYPT_encrypt_bytes

eax_stack = eax
ebx_stack = ebx
esi_stack = esi
edi_stack = edi
ebp_stack = ebp

x = arg1
m = arg2
out = arg3
bytes = arg4

              unsigned>? bytes - 0
goto done if !unsigned>


start:

in0 = *(uint32 *) (x + 0)
in1 = *(uint32 *) (x + 4)
in2 = *(uint32 *) (x + 8)
j0 = in0
in3 = *(uint32 *) (x + 12)
j1 = in1
in4 = *(uint32 *) (x + 16)
j2 = in2
in5 = *(uint32 *) (x + 20)
j3 = in3
in6 = *(uint32 *) (x + 24)
j4 = in4
in7 = *(uint32 *) (x + 28)
j5 = in5
in8 = *(uint32 *) (x + 32)
j6 = in6
in9 = *(uint32 *) (x + 36)
j7 = in7
in10 = *(uint32 *) (x + 40)
j8 = in8
in11 = *(uint32 *) (x + 44)
j9 = in9
in12 = *(uint32 *) (x + 48)
j10 = in10
in13 = *(uint32 *) (x + 52)
j11 = in11
in14 = *(uint32 *) (x + 56)
j12 = in12
in15 = *(uint32 *) (x + 60)
j13 = in13
j14 = in14
j15 = in15

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
  in1 = j1
  in2 = j2
  in3 = j3
  x0 = in0
  x1 = in1
  x2 = in2
  x3 = in3
  in4 = j4
  in5 = j5
  in6 = j6
  in7 = j7
  x4 = in4
  x5 = in5
  x6 = in6
  x7 = in7
  in8 = j8
  in9 = j9
  in10 = j10
  in11 = j11
  x8 = in8
  x9 = in9
  x10 = in10
  x11 = in11
  in12 = j12
  in13 = j13
  in14 = j14
  in15 = j15
  x12 = in12
  x13 = in13
  x14 = in14
  x15 = in15

  i = 20

p = x0
s = x5
t = x10
w = x15

mainloop:



x0 = p
				x10 = t
p += x12
		x5 = s
				t += x6
						x15 = w
		r = x1
		r += s
						v = x11
						v += w
p <<<= 7
p ^= x4
				t <<<= 7
				t ^= x14
		r <<<= 7
		r ^= x9
						v <<<= 7
						v ^= x3
x4 = p
				x14 = t
p += x0
		x9 = r
				t += x10
						x3 = v
p <<<= 9
p ^= x8
				t <<<= 9
				t ^= x2
		s += r
		s <<<= 9
		s ^= x13
						w += v
						w <<<= 9
						w ^= x7
x8 = p
				x2 = t
p += x4
		x13 = s
				t += x14
						x7 = w
p <<<= 13
p ^= x12
				t <<<= 13
				t ^= x6
		r += s
		r <<<= 13
		r ^= x1
						v += w
						v <<<= 13
						v ^= x11
x12 = p
				x6 = t
p += x8
		x1 = r
				t += x2
						x11 = v
p <<<= 18
p ^= x0
				t <<<= 18
				t ^= x10
		s += r
		s <<<= 18
		s ^= x5
						w += v
						w <<<= 18
						w ^= x15
x0 = p
				x10 = t
p += x3
p <<<= 7
		x5 = s
				t += x9
						x15 = w
		r = x4
		r += s
						v = x14
						v += w
p ^= x1
				t <<<= 7
				t ^= x11
		r <<<= 7
		r ^= x6
						v <<<= 7
						v ^= x12
x1 = p
				x11 = t
p += x0
		x6 = r
				t += x10
						x12 = v
p <<<= 9
p ^= x2
				t <<<= 9
				t ^= x8
		s += r
		s <<<= 9
		s ^= x7
						w += v
						w <<<= 9
						w ^= x13
x2 = p
				x8 = t
p += x1
		x7 = s
				t += x11
						x13 = w
p <<<= 13
p ^= x3
				t <<<= 13
				t ^= x9
		r += s
		r <<<= 13
		r ^= x4
						v += w
						v <<<= 13
						v ^= x14
x3 = p
				x9 = t
p += x2
		x4 = r
				t += x8
						x14 = v
p <<<= 18
p ^= x0
				t <<<= 18
				t ^= x10
		s += r
		s <<<= 18
		s ^= x5
						w += v
						w <<<= 18
						w ^= x15


x0 = p
				x10 = t
p += x12
		x5 = s
				t += x6
						x15 = w
		r = x1
		r += s
						v = x11
						v += w
p <<<= 7
p ^= x4
				t <<<= 7
				t ^= x14
		r <<<= 7
		r ^= x9
						v <<<= 7
						v ^= x3
x4 = p
				x14 = t
p += x0
		x9 = r
				t += x10
						x3 = v
p <<<= 9
p ^= x8
				t <<<= 9
				t ^= x2
		s += r
		s <<<= 9
		s ^= x13
						w += v
						w <<<= 9
						w ^= x7
x8 = p
				x2 = t
p += x4
		x13 = s
				t += x14
						x7 = w
p <<<= 13
p ^= x12
				t <<<= 13
				t ^= x6
		r += s
		r <<<= 13
		r ^= x1
						v += w
						v <<<= 13
						v ^= x11
x12 = p
				x6 = t
p += x8
		x1 = r
				t += x2
						x11 = v
p <<<= 18
p ^= x0
				t <<<= 18
				t ^= x10
		s += r
		s <<<= 18
		s ^= x5
						w += v
						w <<<= 18
						w ^= x15
x0 = p
				x10 = t
p += x3
p <<<= 7
		x5 = s
				t += x9
						x15 = w
		r = x4
		r += s
						v = x14
						v += w
p ^= x1
				t <<<= 7
				t ^= x11
		r <<<= 7
		r ^= x6
						v <<<= 7
						v ^= x12
x1 = p
				x11 = t
p += x0
		x6 = r
				t += x10
						x12 = v
p <<<= 9
p ^= x2
				t <<<= 9
				t ^= x8
		s += r
		s <<<= 9
		s ^= x7
						w += v
						w <<<= 9
						w ^= x13
x2 = p
				x8 = t
p += x1
		x7 = s
				t += x11
						x13 = w
p <<<= 13
p ^= x3
				t <<<= 13
				t ^= x9
		r += s
		r <<<= 13
		r ^= x4
						v += w
						v <<<= 13
						v ^= x14
x3 = p
				x9 = t
p += x2
		x4 = r
				t += x8
						x14 = v
p <<<= 18
p ^= x0
				t <<<= 18
				t ^= x10
		s += r
		s <<<= 18
		s ^= x5
						w += v
						w <<<= 18
						w ^= x15



                 unsigned>? i -= 4
goto mainloop if unsigned>

x0 = p
x5 = s
x10 = t
x15 = w


  out = out_backup
  m = m_backup

  in0 = x0
  in1 = x1
  in0 += j0
  in1 += j1
  in0 ^= *(uint32 *) (m + 0)
  in1 ^= *(uint32 *) (m + 4)
  *(uint32 *) (out + 0) = in0
  *(uint32 *) (out + 4) = in1
  in2 = x2
  in3 = x3
  in2 += j2
  in3 += j3
  in2 ^= *(uint32 *) (m + 8)
  in3 ^= *(uint32 *) (m + 12)
  *(uint32 *) (out + 8) = in2
  *(uint32 *) (out + 12) = in3
  in4 = x4
  in5 = x5
  in4 += j4
  in5 += j5
  in4 ^= *(uint32 *) (m + 16)
  in5 ^= *(uint32 *) (m + 20)
  *(uint32 *) (out + 16) = in4
  *(uint32 *) (out + 20) = in5
  in6 = x6
  in7 = x7
  in6 += j6
  in7 += j7
  in6 ^= *(uint32 *) (m + 24)
  in7 ^= *(uint32 *) (m + 28)
  *(uint32 *) (out + 24) = in6
  *(uint32 *) (out + 28) = in7
  in8 = x8
  in9 = x9
  in8 += j8
  in9 += j9
  in8 ^= *(uint32 *) (m + 32)
  in9 ^= *(uint32 *) (m + 36)
  *(uint32 *) (out + 32) = in8
  *(uint32 *) (out + 36) = in9
  in10 = x10
  in11 = x11
  in10 += j10
  in11 += j11
  in10 ^= *(uint32 *) (m + 40)
  in11 ^= *(uint32 *) (m + 44)
  *(uint32 *) (out + 40) = in10
  *(uint32 *) (out + 44) = in11
  in12 = x12
  in13 = x13
  in12 += j12
  in13 += j13
  in12 ^= *(uint32 *) (m + 48)
  in13 ^= *(uint32 *) (m + 52)
  *(uint32 *) (out + 48) = in12
  *(uint32 *) (out + 52) = in13
  in14 = x14
  in15 = x15
  in14 += j14
  in15 += j15
  in14 ^= *(uint32 *) (m + 56)
  in15 ^= *(uint32 *) (m + 60)
  *(uint32 *) (out + 56) = in14
  *(uint32 *) (out + 60) = in15

  bytes = bytes_backup

  in8 = j8
  in9 = j9
  carry? in8 += 1
  in9 += 0 + carry
  j8 = in8
  j9 = in9

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
    in9 = j9
    *(uint32 *) (x + 32) = in8
    *(uint32 *) (x + 36) = in9

    done:

    eax = eax_stack
    ebx = ebx_stack
    esi = esi_stack
    edi = edi_stack
    ebp = ebp_stack

    leave

  bytesatleast65:

  bytes -= 64
  out += 64
  m += 64
goto bytesatleast1


enter ECRYPT_init
leave


enter ECRYPT_keysetup

  eax_stack = eax
  ebx_stack = ebx
  esi_stack = esi
  edi_stack = edi
  ebp_stack = ebp
  
  k = arg2
  kbits = arg3
  x = arg1

  in1 = *(uint32 *) (k + 0)
  in2 = *(uint32 *) (k + 4)
  in3 = *(uint32 *) (k + 8)
  in4 = *(uint32 *) (k + 12)
  *(uint32 *) (x + 4) = in1
  *(uint32 *) (x + 8) = in2
  *(uint32 *) (x + 12) = in3
  *(uint32 *) (x + 16) = in4

                   unsigned<? kbits - 256
  goto kbits128 if unsigned<

  kbits256:

    in11 = *(uint32 *) (k + 16)
    in12 = *(uint32 *) (k + 20)
    in13 = *(uint32 *) (k + 24)
    in14 = *(uint32 *) (k + 28)
    *(uint32 *) (x + 44) = in11
    *(uint32 *) (x + 48) = in12
    *(uint32 *) (x + 52) = in13
    *(uint32 *) (x + 56) = in14

    in0 = 1634760805
    in5 = 857760878
    in10 = 2036477234
    in15 = 1797285236
    *(uint32 *) (x + 0) = in0
    *(uint32 *) (x + 20) = in5
    *(uint32 *) (x + 40) = in10
    *(uint32 *) (x + 60) = in15

  goto keysetupdone

  kbits128:

    in11 = *(uint32 *) (k + 0)
    in12 = *(uint32 *) (k + 4)
    in13 = *(uint32 *) (k + 8)
    in14 = *(uint32 *) (k + 12)
    *(uint32 *) (x + 44) = in11
    *(uint32 *) (x + 48) = in12
    *(uint32 *) (x + 52) = in13
    *(uint32 *) (x + 56) = in14

    in0 = 1634760805
    in5 = 824206446
    in10 = 2036477238
    in15 = 1797285236
    *(uint32 *) (x + 0) = in0
    *(uint32 *) (x + 20) = in5
    *(uint32 *) (x + 40) = in10
    *(uint32 *) (x + 60) = in15

  keysetupdone:

  eax = eax_stack
  ebx = ebx_stack
  esi = esi_stack
  edi = edi_stack
  ebp = ebp_stack

leave


enter ECRYPT_ivsetup

  eax_stack = eax
  ebx_stack = ebx
  esi_stack = esi
  edi_stack = edi
  ebp_stack = ebp
  
  iv = arg2
  x = arg1

  in6 = *(uint32 *) (iv + 0)
  in7 = *(uint32 *) (iv + 4)
  in8 = 0
  in9 = 0
  *(uint32 *) (x + 24) = in6
  *(uint32 *) (x + 28) = in7
  *(uint32 *) (x + 32) = in8
  *(uint32 *) (x + 36) = in9

  eax = eax_stack
  ebx = ebx_stack
  esi = esi_stack
  edi = edi_stack
  ebp = ebp_stack

leave
