This is a rewrite by R. Dolbeau of the ASM code "amd64-xmm6" by D. J. Bernstein
for salsa20 into C+intrinsics, with some extra modifications such as a
transposition-based storage block. The use of intrinsics allows compilation to
both SSE (2-operands) and AVX-128 (3-operands) assembly code by ICC (tested with
version  14.0.3.174) for 128 bits intrinsics. Please note that GCC 4.7.2 doesn't
produce very good code with this if you don't use AVX2 (and not that great
with AVX2 either).

The code also includes AVX2 using the whole 256 bits (Haswell & later), and
AVX-512 (upcoming Knights Landing & upcoming Xeons).

= Rational =
The original amd64-xmm6 by D. J. Bernstein was produced as assembly code
from a higher level description (using the tool 'qhasm' with 'q' language
as input). While this produces fast code, it hardwires some assumption
in the code, such as the type of instructions and the scheduling.

The rewrite as C+intrinsics might lose in pure speed in theory, but
has the following advantages:

1) It doesn't constrain the instructions as much. If the code is compiled
with only SSE support in the compiler (ICC preferably, GCC as well), then
the resulting binary will only feature 2-operands SSE instructions (same
as amd64-xmm6, but a bit slower, except for improvements described later).
However, when compiled with AVX support (not requiring AVX2 here!), then
the resulting binary will feature VEX-128-encoded 3-operands instructions,
thus being a tiny bit faster (less register pressure, less reg-to-reg moves).

2) It delegates register allocation and instruction scheduling to the
compiler. While it might be more efficient to do everything by hand in
assembly, it's a lot of work and quite micro-architecture-dependent. A
good compiler (i.e., ICC...) will be able to do most of the work, and
will be able to schedule for all supported micro-architecture.

3) I, personally, find C+intrinsics a lot more readable than assembly, even
though I can read assembly just fine :-)

= Features =

The code uses the data storage format for the 'input' created by D. J. Bernstein.
It's essentially a diagonally-stored version of the 4x4 matrix:

  0  5 10 15
 12  1  6 11
  8 13  2  7
  4  9 14  3

The code contains four main computational blocks:

1) u1.h : does a single block at a time, using 128 bits SIMD operations.
Rewrite from amd64-xmm2 (also appear as the remainder loop in amd64-xmm6).

2) u4.h : does 4 blocks at a time, using 128 bits SIMD operations. Each
lane computes a different block (256 bytes at a time). The way the computed
input blocks are xor'ed with the message and then stored has been changed,
replacing some scalar instructions by a in-register matrix transpose and SIMD
instructions. This change is what makes the code faster than amd64-xmm6 in
SSE mode when using ICC. Rewrite + modifications from amd64-xmm6.

3) u8.h : does 8 blocks at a time, using 256 bits SIMD operations. Each lane
computes a different block (512 bytes at a time). This requires AVX2 support
in both the compiler and the hardware. The xor'ing and storing at the end are
still using 128 bits vector. This is simply the natural extension in AVX2 of
the code in u4.h.

4) u16.h : does 16 blocks at a time, using 512 bits SIMD operations. Each
lane computes a different block (1024 bytes at a time). There is significantly
less instructions, as AVX-512 supports 'rotate left', replacing 2 shift and one
xor by a single rotate. This requires AVX-512 support in both the compiler and
the hardware. The xor'ing and storing at the end are using scatter-gather on 512
bits vector. This is simply the natural extension in AVX-512 of the code in u8.h.
As of the writing of this (2014/07/15), there is no publicly available hardware
supporting AVX-512, one needs to use the Intel SDE to test the validity of the
code:
<https://software.intel.com/en-us/articles/intel-software-development-emulator>.

= Performance =

GCC 4.7.2 (from Debian Squeeze) does not produce good code when _not_ using
AVX2. The code generated from the intrinsics is slower than the original
assembly amd64-xmm6. However, it gains a factor of 1.9x when using AVX2.

ICC 14.0.2.144 on my system (using an Intel Core i5 4570S) produces code
about 12% faster than the original assembly when _not_ using AVX2. The AVX2
again makes the code about 1.9 times faster.

All of that is for messages of 4096 bytes, and assuming I understand the output
of 'supercop' :-). Shorter messages gain less.

Not all compilers defined in 'okcompilers/c' were tested. The subset tested
(shown below) was checked to produce speed comparable to that reported on
the 'supercop' website for similar CPUs. Also, the machine options chosen for
ICC in 'supercop' (-xP, ...) are obsolete and might not cover all cases.

Compiler tested (i.e., the 'okcompilers/c' file):

gcc -m64 -march=native -mtune=native -O3 -fomit-frame-pointer
gcc -m64 -march=core-avx2 -mtune=core-avx2 -O3 -fomit-frame-pointer
gcc -m64 -march=core-avx-i -mtune=core-avx-i -O3 -fomit-frame-pointer
gcc -m64 -march=corei7-avx -mtune=corei7-avx -O3 -fomit-frame-pointer
gcc -m64 -march=corei7 -mtune=corei7 -O3 -fomit-frame-pointer
icc -m64 -march=native -mtune=native -O3 -fomit-frame-pointer
icc -m64 -march=core-avx2 -mtune=core-avx2 -O3 -fomit-frame-pointer
icc -m64 -march=core-avx-i -mtune=core-avx-i -O3 -fomit-frame-pointer
icc -m64 -march=corei7-avx -mtune=corei7-avx -O3 -fomit-frame-pointer
icc -m64 -march=corei7 -mtune=corei7 -O3 -fomit-frame-pointer

The faster is almost always ICC in 'native' mode. The 'corei7' arch
does not include AVX or AVX2, 'corei7-avx' and 'core-avx-i' do not
include AVX2.

To compile the code for AVX-512, the current option of choice is '-xMIC-AVX512'
with ICC. But you need to do that by hand, as supercop will notice the
compiler produces non-executable binary and will not add it to 'c-compatible'.

-- 
Romain Dolbeau
$Date: 2014/08/23 07:44:51 $
