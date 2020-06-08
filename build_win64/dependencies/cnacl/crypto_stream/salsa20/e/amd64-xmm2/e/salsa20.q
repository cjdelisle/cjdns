int64 a

int64 arg1
int64 arg2
int64 arg3
int64 arg4
input arg1
input arg2
input arg3
input arg4

int64 k
int64 kbits
int64 iv

int64 i

int64 x
int64 m
int64 out
int64 bytes

int6464 diag0
int6464 diag1
int6464 diag2
int6464 diag3
# situation at beginning of first round:
# diag0: x0 x5 x10 x15
# diag1: x12 x1 x6 x11
# diag2: x8 x13 x2 x7
# diag3: x4 x9 x14 x3
# situation at beginning of second round:
# diag0: x0 x5 x10 x15
# diag1: x1 x6 x11 x12
# diag2: x2 x7 x8 x13
# diag3: x3 x4 x9 x14

int6464 a0
int6464 a1
int6464 a2
int6464 a3
int6464 a4
int6464 a5
int6464 a6
int6464 a7
int6464 b0
int6464 b1
int6464 b2
int6464 b3
int6464 b4
int6464 b5
int6464 b6
int6464 b7

int64 in0
int64 in1
int64 in2
int64 in3
int64 in4
int64 in5
int64 in6
int64 in7
int64 in8
int64 in9
int64 in10
int64 in11
int64 in12
int64 in13
int64 in14
int64 in15

stack512 tmp

int64 ctarget
stack64 bytes_backup


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

bytes_backup = bytes


diag0 = *(int128 *) (x + 0)
diag1 = *(int128 *) (x + 16)
diag2 = *(int128 *) (x + 32)
diag3 = *(int128 *) (x + 48)


            	a0 = diag1
i = 20

mainloop:

uint32323232	a0 += diag0
				a1 = diag0
            	b0 = a0
uint32323232	a0 <<= 7
uint32323232	b0 >>= 25
                diag3 ^= a0

                diag3 ^= b0

uint32323232			a1 += diag3
						a2 = diag3
            			b1 = a1
uint32323232			a1 <<= 9
uint32323232			b1 >>= 23
				diag2 ^= a1
		diag3 <<<= 32
				diag2 ^= b1

uint32323232					a2 += diag2
								a3 = diag2
            					b2 = a2
uint32323232					a2 <<= 13
uint32323232					b2 >>= 19
						diag1 ^= a2
				diag2 <<<= 64
						diag1 ^= b2

uint32323232							a3 += diag1
		a4 = diag3
            							b3 = a3
uint32323232							a3 <<= 18
uint32323232							b3 >>= 14
								diag0 ^= a3
						diag1 <<<= 96
								diag0 ^= b3

uint32323232	a4 += diag0
				a5 = diag0
            	b4 = a4
uint32323232	a4 <<= 7
uint32323232	b4 >>= 25
                diag1 ^= a4

                diag1 ^= b4

uint32323232			a5 += diag1
						a6 = diag1
            			b5 = a5
uint32323232			a5 <<= 9
uint32323232			b5 >>= 23
				diag2 ^= a5
		diag1 <<<= 32
				diag2 ^= b5

uint32323232					a6 += diag2
								a7 = diag2
            					b6 = a6
uint32323232					a6 <<= 13
uint32323232					b6 >>= 19
						diag3 ^= a6
				diag2 <<<= 64
						diag3 ^= b6

uint32323232							a7 += diag3
		a0 = diag1
            							b7 = a7
uint32323232							a7 <<= 18
uint32323232							b7 >>= 14
								diag0 ^= a7
						diag3 <<<= 96
								diag0 ^= b7


uint32323232	a0 += diag0
				a1 = diag0
            	b0 = a0
uint32323232	a0 <<= 7
uint32323232	b0 >>= 25
                diag3 ^= a0

                diag3 ^= b0

uint32323232			a1 += diag3
						a2 = diag3
            			b1 = a1
uint32323232			a1 <<= 9
uint32323232			b1 >>= 23
				diag2 ^= a1
		diag3 <<<= 32
				diag2 ^= b1

uint32323232					a2 += diag2
								a3 = diag2
            					b2 = a2
uint32323232					a2 <<= 13
uint32323232					b2 >>= 19
						diag1 ^= a2
				diag2 <<<= 64
						diag1 ^= b2

uint32323232							a3 += diag1
		a4 = diag3
            							b3 = a3
uint32323232							a3 <<= 18
uint32323232							b3 >>= 14
								diag0 ^= a3
						diag1 <<<= 96
								diag0 ^= b3

uint32323232	a4 += diag0
				a5 = diag0
            	b4 = a4
uint32323232	a4 <<= 7
uint32323232	b4 >>= 25
                diag1 ^= a4

                diag1 ^= b4

uint32323232			a5 += diag1
						a6 = diag1
            			b5 = a5
uint32323232			a5 <<= 9
uint32323232			b5 >>= 23
				diag2 ^= a5
		diag1 <<<= 32
				diag2 ^= b5

uint32323232					a6 += diag2
								a7 = diag2
            					b6 = a6
uint32323232					a6 <<= 13
uint32323232					b6 >>= 19
						diag3 ^= a6
				diag2 <<<= 64
						diag3 ^= b6
                 unsigned>? i -= 4
uint32323232							a7 += diag3
		a0 = diag1
            							b7 = a7
uint32323232							a7 <<= 18
uint32323232							b7 >>= 14
								diag0 ^= a7
						diag3 <<<= 96
								diag0 ^= b7
goto mainloop if unsigned>


uint32323232 diag0 += *(int128 *) (x + 0)
uint32323232 diag1 += *(int128 *) (x + 16)
uint32323232 diag2 += *(int128 *) (x + 32)
uint32323232 diag3 += *(int128 *) (x + 48)


in0 = diag0
in12 = diag1
in8 = diag2
in4 = diag3
diag0 <<<= 96
diag1 <<<= 96
diag2 <<<= 96
diag3 <<<= 96
(uint32) in0 ^= *(uint32 *) (m + 0)
(uint32) in12 ^= *(uint32 *) (m + 48)
(uint32) in8 ^= *(uint32 *) (m + 32)
(uint32) in4 ^= *(uint32 *) (m + 16)
*(uint32 *) (out + 0) = in0
*(uint32 *) (out + 48) = in12
*(uint32 *) (out + 32) = in8
*(uint32 *) (out + 16) = in4

in5 = diag0
in1 = diag1
in13 = diag2
in9 = diag3
diag0 <<<= 96
diag1 <<<= 96
diag2 <<<= 96
diag3 <<<= 96
(uint32) in5 ^= *(uint32 *) (m + 20)
(uint32) in1 ^= *(uint32 *) (m + 4)
(uint32) in13 ^= *(uint32 *) (m + 52)
(uint32) in9 ^= *(uint32 *) (m + 36)
*(uint32 *) (out + 20) = in5
*(uint32 *) (out + 4) = in1
*(uint32 *) (out + 52) = in13
*(uint32 *) (out + 36) = in9

in10 = diag0
in6 = diag1
in2 = diag2
in14 = diag3
diag0 <<<= 96
diag1 <<<= 96
diag2 <<<= 96
diag3 <<<= 96
(uint32) in10 ^= *(uint32 *) (m + 40)
(uint32) in6 ^= *(uint32 *) (m + 24)
(uint32) in2 ^= *(uint32 *) (m + 8)
(uint32) in14 ^= *(uint32 *) (m + 56)
*(uint32 *) (out + 40) = in10
*(uint32 *) (out + 24) = in6
*(uint32 *) (out + 8) = in2
*(uint32 *) (out + 56) = in14

in15 = diag0
in11 = diag1
in7 = diag2
in3 = diag3
(uint32) in15 ^= *(uint32 *) (m + 60)
(uint32) in11 ^= *(uint32 *) (m + 44)
(uint32) in7 ^= *(uint32 *) (m + 28)
(uint32) in3 ^= *(uint32 *) (m + 12)
*(uint32 *) (out + 60) = in15
*(uint32 *) (out + 44) = in11
*(uint32 *) (out + 28) = in7
*(uint32 *) (out + 12) = in3


bytes = bytes_backup

  in8 = *(uint32 *) (x + 32)
  in9 = *(uint32 *) (x + 52)
  in8 += 1
  in9 <<= 32
  in8 += in9
  *(uint32 *) (x + 32) = in8
  (uint64) in8 >>= 32
  *(uint32 *) (x + 52) = in8


                         unsigned>? unsigned<? bytes - 64
  goto bytesatleast65 if unsigned>

    goto bytesatleast64 if !unsigned<
      m = out
      out = ctarget
      i = bytes
      while (i) { *out++ = *m++; --i }
    bytesatleast64:
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

# situation at beginning of first round:
# diag0: x0 x5 x10 x15
# diag1: x12 x1 x6 x11
# diag2: x8 x13 x2 x7
# diag3: x4 x9 x14 x3

  in1 = *(uint32 *) (k + 0)
  in2 = *(uint32 *) (k + 4)
  in3 = *(uint32 *) (k + 8)
  in4 = *(uint32 *) (k + 12)
  *(uint32 *) (x + 20) = in1
  *(uint32 *) (x + 40) = in2
  *(uint32 *) (x + 60) = in3
  *(uint32 *) (x + 48) = in4

                   unsigned<? kbits - 256
  goto kbits128 if unsigned<

  kbits256:

    in11 = *(uint32 *) (k + 16)
    in12 = *(uint32 *) (k + 20)
    in13 = *(uint32 *) (k + 24)
    in14 = *(uint32 *) (k + 28)
    *(uint32 *) (x + 28) = in11
    *(uint32 *) (x + 16) = in12
    *(uint32 *) (x + 36) = in13
    *(uint32 *) (x + 56) = in14

    in0 = 1634760805
    in5 = 857760878
    in10 = 2036477234
    in15 = 1797285236
    *(uint32 *) (x + 0) = in0
    *(uint32 *) (x + 4) = in5
    *(uint32 *) (x + 8) = in10
    *(uint32 *) (x + 12) = in15

  goto keysetupdone

  kbits128:

    in11 = *(uint32 *) (k + 0)
    in12 = *(uint32 *) (k + 4)
    in13 = *(uint32 *) (k + 8)
    in14 = *(uint32 *) (k + 12)
    *(uint32 *) (x + 28) = in11
    *(uint32 *) (x + 16) = in12
    *(uint32 *) (x + 36) = in13
    *(uint32 *) (x + 56) = in14

    in0 = 1634760805
    in5 = 824206446
    in10 = 2036477238
    in15 = 1797285236
    *(uint32 *) (x + 0) = in0
    *(uint32 *) (x + 4) = in5
    *(uint32 *) (x + 8) = in10
    *(uint32 *) (x + 12) = in15

  keysetupdone:

leave


enter ECRYPT_ivsetup

  iv = arg2
  x = arg1

# situation at beginning of first round:
# diag0: x0 x5 x10 x15
# diag1: x12 x1 x6 x11
# diag2: x8 x13 x2 x7
# diag3: x4 x9 x14 x3

  in6 = *(uint32 *) (iv + 0)
  in7 = *(uint32 *) (iv + 4)
  in8 = 0
  in9 = 0
  *(uint32 *) (x + 24) = in6
  *(uint32 *) (x + 44) = in7
  *(uint32 *) (x + 32) = in8
  *(uint32 *) (x + 52) = in9

leave
