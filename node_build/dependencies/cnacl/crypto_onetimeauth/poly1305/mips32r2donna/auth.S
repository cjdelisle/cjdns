/* -*- Mode:MAL */
/**
 * @author Caleb James DeLisle <cjd at cjdns dot fr>
 * @version 1.0
 * @since 28 Feb 2014
 *
 * Bernstein's Poly1305 ported to mips32r2 processors.
 * Based on the poly1305-donna algorithm by Floodyberry.
 *
 * This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 *
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */
#define SC $2
#define CA $3

#define OUT $4
#define MSG $5
#define LEN $6
#define KEY $7

#define H0 $8
#define H1 $9
#define H2 $10
#define H3 $11
#define H4 $12

#define R0 $13
#define R1 $14
#define R2 $15
#define R3 $16
#define R4 $17

#define O0 $18
#define O1 $19
#define O2 $20
#define O3 $21
#define O4 $22

#define S1 $23
#define S2 $24
#define S3 $25
#define S4 $30

#if defined(__MIPSEL) || defined(_MIPSEL) || defined(MIPSEL) || defined(__MIPSEL__)
    #define LITTLE_ENDIAN
#elif defined(__MIPSEB) || defined(_MIPSEB) || defined(MIPSEB) || defined(__MIPSEB__)
    #define BIG_ENDIAN
#else
    #error cannot determine byte order of target processor
#endif

.macro CARRY out, carryOut, carryIn, scratch
    mflo \out
    mfhi \scratch
    addu \carryOut,\out,\carryIn
    sltu \out,\carryOut,\out
    addu \scratch,\out,\scratch
    ext	\out,\carryOut,0,26
    srl \carryOut,\carryOut,26
    sll \scratch,\scratch,6
    addu \carryOut,\carryOut,\scratch
.endm

.macro MULT_ROW p1b,p2b,p3b,p4b,p5b, p1a,p2a,p3a,p4a,p5a
    multu \p1a,\p1b
    maddu \p2a,\p2b
    maddu \p3a,\p3b
    maddu \p4a,\p4b
    maddu \p5a,\p5b
.endm

# Add 5 times input2...    output = input + input2 * 5
.macro PLUS_5X output, input, input2, scratch
    addiu \scratch,$zero,5
    multu SC,\input2
    mflo \scratch
    addu \output,\input,\scratch
.endm

.macro BYTESWAP reg
    wsbh \reg,\reg
    rotr \reg,\reg,16
.endm

#ifdef BIG_ENDIAN
    .macro LITTLE_ENDIAN_TO_HOST reg
        BYTESWAP \reg
    .endm
#else
    .macro LITTLE_ENDIAN_TO_HOST reg
    .endm
#endif
.macro HOST_TO_LITTLE_ENDIAN reg
    LITTLE_ENDIAN_TO_HOST \reg
.endm

####### Begin

.abicalls
.text

.set	nomips16
.set	nomicromips
.globl crypto_onetimeauth_poly1305_mips32r2donna
.ent	crypto_onetimeauth_poly1305_mips32r2donna
.type	crypto_onetimeauth_poly1305_mips32r2donna, @function
crypto_onetimeauth_poly1305_mips32r2donna:

.frame  $sp,52,$31

## Save all of the callee-saved registers to the stack...
addiu $sp,$sp,-52
sw $fp,48($sp)
sw $23,44($sp)
sw $22,40($sp)
sw $21,36($sp)
sw $20,32($sp)
sw $19,28($sp)
sw $18,24($sp)
sw $17,20($sp)
sw $16,16($sp)

# This is a 32 bit machine so it is physically impossible for this function
# to handle a message larger than 4GB but the message argument is passed as
# an unsigned long long (64 bits) so the key field is currently holding the
# low bits of the message length and LEN is the high bits, move low to high
# and load the key pointer off the stack.
#ifdef BIG_ENDIAN
  move LEN,KEY
#endif
lw KEY,68($sp)


#	t0 = U8TO32_LE(key+0);
#	t1 = U8TO32_LE(key+4);
#	t2 = U8TO32_LE(key+8);
#	t3 = U8TO32_LE(key+12);
lw O0,0(KEY)
lw O1,4(KEY)
lw O2,8(KEY)
lw O3,12(KEY)

LITTLE_ENDIAN_TO_HOST O0
LITTLE_ENDIAN_TO_HOST O1
LITTLE_ENDIAN_TO_HOST O2
LITTLE_ENDIAN_TO_HOST O3

#	r0 = t0 & 0x3ffffff; t0 >>= 26; t0 |= t1 << 6;
ext R0,O0,0,26
srl O0,O0,26
sll SC,O1,6
or O0,SC,O0

## note: 0xffffff03 == (uint32_t) -253
addiu O4,$zero,-253

#	r1 = t0 & 0x3ffff03; t1 >>= 20; t1 |= t2 << 12;
ext R1,O0,0,26
and R1,R1,O4
srl O1,O1,20
sll SC,O2,12
or O1,SC,O1

## note: 0xffffc0ff == rotl((uint32_t) -253, 6)
rotr O4,O4,-6

#	r2 = t1 & 0x3ffc0ff; t2 >>= 14; t2 |= t3 << 18;
ext R2,O1,0,26
and R2,R2,O4
srl O2,O2,14
sll SC,O3,18
or O2,SC,O2

## note: 0xfff03fff == rotl((uint32_t) -253, 12)
rotr O4,O4,-6

#	r3 = t2 & 0x3f03fff; t3 >>= 8;
ext R3,O2,0,26
and R3,R3,O4
srl O3,O3,8

#	r4 = t3 & 0x00fffff;
ext R4,O3,0,20

#	s1 = r1 * 5;
#	s2 = r2 * 5;
#	s3 = r3 * 5;
#	s4 = r4 * 5;
PLUS_5X S1,$zero,R1,SC
PLUS_5X S2,$zero,R2,SC
PLUS_5X S3,$zero,R3,SC
PLUS_5X S4,$zero,R4,SC

# Initial state
move H0,$zero
move H1,$zero
move H2,$zero
move H3,$zero
move H4,$zero

addiu SC,LEN,-16
bltz SC,poly1305_mips32r2donna_atmost15bytes

poly1305_mips32r2donna_16bytes:

addiu MSG,MSG,16
addiu LEN,LEN,-16

lw O0,-16(MSG)
lw O1,-12(MSG)
lw O2,-8(MSG)
lw O3,-4(MSG)

# h0 += t0 & 0x3ffffff;
LITTLE_ENDIAN_TO_HOST O0
ext	SC,O0,0,26
addu H0,SC,H0

# h1 += ((((uint64_t)t1 << 32) | t0) >> 26) & 0x3ffffff;
srl SC,O0,26
addu H1,SC,H1
LITTLE_ENDIAN_TO_HOST O1
ext SC,O1,0,20 # 26 - (32 - 26)
sll SC,SC,6    # 32 - 26
addu H1,SC,H1

# h2 += ((((uint64_t)t2 << 32) | t1) >> 20) & 0x3ffffff;
srl SC,O1,20
addu H2,SC,H2
LITTLE_ENDIAN_TO_HOST O2
ext SC,O2,0,14 # 26 - (32 - 20)
sll SC,SC,12   # 32 - 20
addu H2,SC,H2

# h3 += ((((uint64_t)t3 << 32) | t2) >> 14) & 0x3ffffff;
srl SC,O2,14
addu H3,SC,H3
LITTLE_ENDIAN_TO_HOST O3
ext SC,O3,0,8 # 26 - (32 - 14)
sll SC,SC,18  # 32 - 14
addu H3,SC,H3

# h4 += (t3 >> 8) | (1 << 24);
addiu SC,$zero,1
sll SC,SC,24
addu H4,SC,H4
srl SC,O3,8
addu H4,SC,H4

poly1305_mips32r2donna_mult:

MULT_ROW H0,H1,H2,H3,H4, R0,S4,S3,S2,S1 ; CARRY O0, CA, $zero, SC
MULT_ROW H0,H1,H2,H3,H4, R1,R0,S4,S3,S2 ; CARRY O1,CA,CA,SC
MULT_ROW H0,H1,H2,H3,H4, R2,R1,R0,S4,S3 ; CARRY O2,CA,CA,SC
MULT_ROW H0,H1,H2,H3,H4, R3,R2,R1,R0,S4 ; CARRY O3,CA,CA,SC
MULT_ROW H0,H1,H2,H3,H4, R4,R3,R2,R1,R0 ; CARRY O4,CA,CA,SC

# h0 += b * 5;
PLUS_5X H0,O0,CA,SC

move H1,O1
move H2,O2
move H3,O3
move H4,O4

## if (inlen >= 16) goto poly1305_donna_16bytes;
addiu SC,LEN,-16
bgez SC,poly1305_mips32r2donna_16bytes

###

poly1305_mips32r2donna_atmost15bytes:
beq LEN,$zero,poly1305_mips32r2donna_finish

sw $zero,0($sp)
sw $zero,4($sp)
sw $zero,8($sp)
sw $zero,12($sp)

# for (j = 0; j < inlen; j++) mp[j] = m[j];
move O2,$sp
addu O1,MSG,LEN
poly1305_mips32r2donna_loadbyte:
lbu SC,0(MSG)
sb SC,0(O2)
addiu MSG,MSG,1
addiu O2,O2,1
bne MSG,O1,poly1305_mips32r2donna_loadbyte

# mp[j++] = 1;
addiu SC,$zero,1
sb SC,0(O2)

move LEN,$zero

lw O0,0($sp)
lw O1,4($sp)
lw O2,8($sp)
lw O3,12($sp)


# h0 += t0 & 0x3ffffff;
LITTLE_ENDIAN_TO_HOST O0
ext	SC,O0,0,26
addu H0,H0,SC

# h1 += ((((uint64_t)t1 << 32) | t0) >> 26) & 0x3ffffff;
srl SC,O0,26
addu H1,SC,H1
LITTLE_ENDIAN_TO_HOST O1
ext SC,O1,0,20 # 26 - (32 - 26)
sll SC,SC,6    # 32 - 26
addu H1,SC,H1

# h2 += ((((uint64_t)t2 << 32) | t1) >> 20) & 0x3ffffff;
srl SC,O1,20
addu H2,SC,H2
LITTLE_ENDIAN_TO_HOST O2
ext SC,O2,0,14 # 26 - (32 - 20)
sll SC,SC,12   # 32 - 20
addu H2,SC,H2

# h3 += ((((uint64_t)t3 << 32) | t2) >> 14) & 0x3ffffff;
srl SC,O2,14
addu H3,SC,H3
LITTLE_ENDIAN_TO_HOST O3
ext SC,O3,0,8 # 26 - (32 - 14)
sll SC,SC,18  # 32 - 14
addu H3,SC,H3

# h4 += (t3 >> 8);
srl SC,O3,8
addu H4,SC,H4

j poly1305_mips32r2donna_mult

poly1305_mips32r2donna_finish:

##	             b = h0 >> 26; h0 = h0 & 0x3ffffff;
srl CA,H0,26
ext H0,H0,0,26
##	h1 +=     b; b = h1 >> 26; h1 = h1 & 0x3ffffff;
addu H1,CA,H1
srl CA,H1,26
ext H1,H1,0,26
##	h2 +=     b; b = h2 >> 26; h2 = h2 & 0x3ffffff;
addu H2,CA,H2
srl CA,H2,26
ext H2,H2,0,26
##	h3 +=     b; b = h3 >> 26; h3 = h3 & 0x3ffffff;
addu H3,CA,H3
srl CA,H3,26
ext H3,H3,0,26
##	h4 +=     b; b = h4 >> 26; h4 = h4 & 0x3ffffff;
addu H4,CA,H4
srl CA,H4,26
ext H4,H4,0,26
##	h0 += b * 5;
PLUS_5X H0,H0,CA,SC

#	g0 = h0 + 5; b = g0 >> 26; g0 &= 0x3ffffff;
addiu O0,H0,5
srl CA,O0,26
ext O0,O0,0,26
#	g1 = h1 + b; b = g1 >> 26; g1 &= 0x3ffffff;
addu O1,H1,CA
srl CA,O1,26
ext O1,O1,0,26
#	g2 = h2 + b; b = g2 >> 26; g2 &= 0x3ffffff;
addu O2,H2,CA
srl CA,O2,26
ext O2,O2,0,26
#	g3 = h3 + b; b = g3 >> 26; g3 &= 0x3ffffff;
addu O3,H3,CA
srl CA,O3,26
ext O3,O3,0,26
#	g4 = h4 + b - (1 << 26);
addu O4,H4,CA
addiu SC,$zero,1
sll SC,SC,26
subu O4,O4,SC


#	b = (g4 >> 31) - 1;
srl CA,O4,31
addiu CA,CA,-1
#	nb = ~b;
addiu SC,$zero,-1
xor SC,CA,SC
#	h0 = (h0 & nb) | (g0 & b);
and H0,H0,SC
and O0,O0,CA
or H0,O0,H0
#	h1 = (h1 & nb) | (g1 & b);
and H1,H1,SC
and O1,O1,CA
or H1,O1,H1
#	h2 = (h2 & nb) | (g2 & b);
and H2,H2,SC
and O2,O2,CA
or H2,O2,H2
#	h3 = (h3 & nb) | (g3 & b);
and H3,H3,SC
and O3,O3,CA
or H3,O3,H3
#	h4 = (h4 & nb) | (g4 & b);
and H4,H4,SC
and O4,O4,CA
or H4,O4,H4

#
# f0 = ((h0      ) | (h1 << 26)) + (uint64_t)U8TO32_LE(&key[16]);
# f1 = ((h1 >>  6) | (h2 << 20)) + (uint64_t)U8TO32_LE(&key[20]);
# f2 = ((h2 >> 12) | (h3 << 14)) + (uint64_t)U8TO32_LE(&key[24]);
# f3 = ((h3 >> 18) | (h4 <<  8)) + (uint64_t)U8TO32_LE(&key[28]);
# Done in stages...
#
lw O0,16(KEY)
lw O1,20(KEY)
lw O2,24(KEY)
lw O3,28(KEY)

# h0 = ((h0      ) | (h1 << 26));
sll SC,H1,26
or H0,SC,H0

# h1 = ((h1 >>  6) | (h2 << 20));
sll SC,H2,20
srl H1,H1,6
or H1,SC,H1

# h2 = ((h2 >> 12) | (h3 << 14));
sll SC,H3,14
srl H2,H2,12
or H2,SC,H2

# h3 = ((h3 >> 18) | (h4 <<  8));
sll SC,H4,8
srl H3,H3,18
or H3,SC,H3

# o0 = h0 + U8TO32_LE(&key[16]);
LITTLE_ENDIAN_TO_HOST O0
addu O0,O0,H0
sltu CA,O0,H0

LITTLE_ENDIAN_TO_HOST O1
addu O1,O1,H1
sltu SC,O1,H1
addu O1,O1,CA
sltu CA,O1,CA
addu CA,SC,CA

LITTLE_ENDIAN_TO_HOST O2
addu O2,O2,H2
sltu SC,O2,H2
addu O2,O2,CA
sltu CA,O2,CA
addu CA,SC,CA

LITTLE_ENDIAN_TO_HOST O3
addu O3,O3,H3
sltu SC,O3,H3
addu O3,O3,CA
sltu CA,O3,CA
addu CA,SC,CA

HOST_TO_LITTLE_ENDIAN O0
HOST_TO_LITTLE_ENDIAN O1
HOST_TO_LITTLE_ENDIAN O2
HOST_TO_LITTLE_ENDIAN O3

sw O0,0(OUT)
sw O1,4(OUT)
sw O2,8(OUT)
sw O3,12(OUT)

# return 0;
move $2,$zero

## Pop callee-save registers from stack
lw $16,16($sp)
lw $17,20($sp)
lw $18,24($sp)
lw $19,28($sp)
lw $20,32($sp)
lw $21,36($sp)
lw $22,40($sp)
lw $23,44($sp)
lw $fp,48($sp)
addiu $sp,$sp,52
j $31

.end crypto_onetimeauth_poly1305_mips32r2donna
