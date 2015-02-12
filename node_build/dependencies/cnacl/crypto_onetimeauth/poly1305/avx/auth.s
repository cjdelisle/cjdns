/**
 * @author Billy Brumley <billy.brumley at aalto dot fi>
 * @version 1.0
 * @since 28 Oct 2011
 *
 * Bernstein's Poly1305 for chips featuring Intel AVX.
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

.data
.p2align 5
SCALE:		.quad 0x37f4000000000000, 0x37f4000000000000, 0x37f4000000000000, 0x37f4000000000000
ALPHA22:	.quad 0x4498000000000000, 0x4498000000000000, 0x4498000000000000, 0x4498000000000000
ALPHA44:	.quad 0x45f8000000000000, 0x45f8000000000000, 0x45f8000000000000, 0x45f8000000000000
ALPHA65:	.quad 0x4748000000000000, 0x4748000000000000, 0x4748000000000000, 0x4748000000000000
ALPHA87:	.quad 0x48a8000000000000, 0x48a8000000000000, 0x48a8000000000000, 0x48a8000000000000
ALPHA109:	.quad 0x4a08000000000000, 0x4a08000000000000, 0x4a08000000000000, 0x4a08000000000000
ALPHA130:	.quad 0x4b58000000000000, 0x4b58000000000000, 0x4b58000000000000, 0x4b58000000000000
POW232:		.quad 0x41f0000000000000, 0x41f0000000000000, 0x41f0000000000000, 0x41f0000000000000
POW264:		.quad 0x43f0000000000000, 0x43f0000000000000, 0x43f0000000000000, 0x43f0000000000000
POW296:		.quad 0x45f0000000000000, 0x45f0000000000000, 0x45f0000000000000, 0x45f0000000000000
POW2128:	.quad 0x47f0000000000000, 0x47f0000000000000, 0x47f0000000000000, 0x47f0000000000000
POWMIX:		.quad 0x3ff0000000000000, 0x41f0000000000000, 0x43f0000000000000, 0x45f0000000000000
P0:		.quad 0x414ffffb00000000, 0x414ffffb00000000, 0x414ffffb00000000, 0x414ffffb00000000
P22:		.quad 0x42afffff80000000, 0x42afffff80000000, 0x42afffff80000000, 0x42afffff80000000
P44:		.quad 0x440fffff80000000, 0x440fffff80000000, 0x440fffff80000000, 0x440fffff80000000
P65:		.quad 0x456fffff80000000, 0x456fffff80000000, 0x456fffff80000000, 0x456fffff80000000
P87:		.quad 0x46cfffff80000000, 0x46cfffff80000000, 0x46cfffff80000000, 0x46cfffff80000000
P109:		.quad 0x481fffff00000000, 0x481fffff00000000, 0x481fffff00000000, 0x481fffff00000000
POW222I:	.quad 0x3e90000000000000, 0x3e90000000000000, 0x3e90000000000000, 0x3e90000000000000
POW244I:	.quad 0x3d30000000000000, 0x3d30000000000000, 0x3d30000000000000, 0x3d30000000000000
POW265I:	.quad 0x3be0000000000000, 0x3be0000000000000, 0x3be0000000000000, 0x3be0000000000000
POW287I:	.quad 0x3a80000000000000, 0x3a80000000000000, 0x3a80000000000000, 0x3a80000000000000
POW2109I:	.quad 0x3920000000000000, 0x3920000000000000, 0x3920000000000000, 0x3920000000000000
PMASK:		.quad 0x0FFFFFFC0FFFFFFF, 0x0FFFFFFC0FFFFFFC, 0x0FFFFFFC0FFFFFFF, 0x0FFFFFFC0FFFFFFC

.macro MSTEP z0, z1, z2, z3, z4, z5, x0, x1, x2, x3, x4, x5, y0, t0, t1, t2
        vmulpd SCALE, \z5, \z5
        vmulpd \y0, \x5, \t2
        vmulpd \y0, \x4, \t1
        vmulpd \y0, \x3, \t0
        vaddpd \t2, \z4, \z4
        vaddpd \t1, \z3, \z3
        vaddpd \t0, \z2, \z2
        vmulpd \y0, \x2, \t2
        vmulpd \y0, \x1, \t1
        vmulpd \y0, \x0, \t0
        vaddpd \t2, \z1, \z1
        vaddpd \t1, \z0, \z0
        vaddpd \t0, \z5, \z5
.endm

.macro CSTEP z0, z1, alpha, t0, t1
	vmovapd \alpha, \t0
        vaddpd \t0, \z0, \t1
        vsubpd \t0, \t1, \t1
        vsubpd \t1, \z0, \z0
        vaddpd \t1, \z1, \z1
.endm

.macro CARRY z0, z1, z2, z3, z4, z5, t0, t1
        CSTEP \z0, \z1, ALPHA22, \t0, \t1
        CSTEP \z1, \z2, ALPHA44, \t0, \t1
        CSTEP \z2, \z3, ALPHA65, \t0, \t1
        CSTEP \z3, \z4, ALPHA87, \t0, \t1
        CSTEP \z4, \z5, ALPHA109, \t0, \t1
.endm

.macro CARRYR z0, z1, z2, z3, z4, z5, t0, t1
        CSTEP \z1, \z2, ALPHA44, \t0, \t1
        CSTEP \z2, \z3, ALPHA65, \t0, \t1
        CSTEP \z3, \z4, ALPHA87, \t0, \t1
        CSTEP \z4, \z5, ALPHA109, \t0, \t1
        vmovapd ALPHA130, \t0
        vaddpd \t0, \z5, \t1
        vsubpd \t0, \t1, \t1
        vsubpd \t1, \z5, \z5
	vmulpd SCALE, \t1, \t1
        vaddpd \t1, \z0, \z0
        CSTEP \z0, \z1, ALPHA22, \t0, \t1
        CSTEP \z1, \z2, ALPHA44, \t0, \t1
.endm

.macro MULI c0, c1, c2, c3, c4, a0, a1, a2, b0, b1
	movq \a0, %rax
	mulq \b1
	movq %rax, \c1
	movq %rdx, \c2
	movq \a2, %rax
	mulq \b1
	movq %rax, \c3
	movq %rdx, \c4
	movq \a1, %rax
	mulq \b1
	addq %rax, \c2
	adcq %rdx, \c3
	adcq $0, \c4
	movq \a0, %rax
	mulq \b0
	movq %rax, \c0
	addq %rdx, \c1
	adcq $0, \c2
	adcq $0, \c3
	adcq $0, \c4
	movq \a1, %rax
	mulq \b0
	addq %rax, \c1
	adcq %rdx, \c2
	adcq $0, \c3
	adcq $0, \c4
	movq \a2, %rax
	mulq \b0
	addq %rax, \c2
	adcq %rdx, \c3
	adcq $0, \c4
	movq \c2, %rdx
	andq $0x3, \c2
	andq $0xFFFFFFFFFFFFFFFC, %rdx
	addq %rdx, \c0
	adcq \c3, \c1
	adcq \c4, \c2
	shrq $1, \c4
	rcrq $1, \c3
	rcrq $1, %rdx
	shrq $1, \c4
	rcrq $1, \c3
	rcrq $1, %rdx
	addq %rdx, \c0
	adcq \c3, \c1
	adcq \c4, \c2
.endm

/* extern void poly1305_tag_asm(unsigned char *tag, const unsigned char *key, const unsigned char *data, int len); */
/*                                             rdi                       rsi                       rdx       rcx   */
/* int crypto_onetimeauth(unsigned char *out,const unsigned char *in,unsigned long long inlen,const unsigned char *k); */
/*                                       rdi                      rsi                   rdx                        rcx */
.globl _crypto_onetimeauth_poly1305_avx
.globl crypto_onetimeauth_poly1305_avx
_crypto_onetimeauth_poly1305_avx:
crypto_onetimeauth_poly1305_avx:
	/* retrofit API: (rsi, rdx, rcx) := (rcx, rsi, rdx) */
	xchgq %rsi, %rdx
	xchgq %rsi, %rcx

	cmp $1, %rcx
	jge Lstart

	/* handle this corner case immediately. */
	movq 16(%rsi), %r8
	movq 24(%rsi), %r9
	movq %r8, 0(%rdi)
	movq %r9, 8(%rdi)
	xorq %rax,%rax
	xorq %rdx,%rdx
	ret

Lstart:
	pushq %r15
	pushq %r14
	pushq %r13
	pushq %r12
	pushq %rbp
	pushq %rsp
	pushq %rbx
	pushq %rdi

	/* skip all the cool stuff for short messages */
	xorq %r10, %r10
	xorq %r11, %r11
	xorq %r12, %r12
	cmp $64, %rcx
	jl Lfinalize

	/* save, align stack */
	movq %rsp, %rax
	andq $0x1f, %rax
	subq %rax, %rsp
	addq %rsp, %rax

	/* load point */
	movdqu (%rsi), %xmm4
	pand PMASK, %xmm4

        /* *signed* convert, then fix sign */
        vcvtdq2pd %xmm4, %ymm5
        vxorpd %ymm6, %ymm6, %ymm6
        vblendvpd %ymm5, POW232, %ymm6, %ymm0
        vaddpd %ymm0, %ymm5, %ymm5
	vmulpd POWMIX, %ymm5, %ymm5

        /* ymm0..5 := (a, a, a, a) */
	vunpcklpd %ymm5, %ymm5, %ymm6
	vperm2f128 $0x00, %ymm6, %ymm6, %ymm0
	vperm2f128 $0x11, %ymm6, %ymm6, %ymm2
	vunpckhpd %ymm5, %ymm5, %ymm9
        vperm2f128 $0x00, %ymm9, %ymm9, %ymm1
        vperm2f128 $0x11, %ymm9, %ymm9, %ymm4

	/* balance bits across polynomial */
	vxorpd %ymm3, %ymm3, %ymm3
	vxorpd %ymm5, %ymm5, %ymm5
	CARRY %ymm0, %ymm1, %ymm2, %ymm3, %ymm4, %ymm5, %ymm14, %ymm15

	/* ymm6..11 := (a, a, a, a) * (a, a, a, a) -> (a**2, a**2, a**2, a**2) */
	vmulpd %ymm0, %ymm5, %ymm11
        vmulpd %ymm1, %ymm5, %ymm6
        vmulpd %ymm2, %ymm5, %ymm7
        vmulpd %ymm3, %ymm5, %ymm8
        vmulpd %ymm4, %ymm5, %ymm9
        vmulpd %ymm5, %ymm5, %ymm10

	MSTEP %ymm11, %ymm6, %ymm7, %ymm8, %ymm9, %ymm10, %ymm0, %ymm1, %ymm2, %ymm3, %ymm4, %ymm5, %ymm4, %ymm13, %ymm14, %ymm15
        MSTEP %ymm10, %ymm11, %ymm6, %ymm7, %ymm8, %ymm9, %ymm0, %ymm1, %ymm2, %ymm3, %ymm4, %ymm5, %ymm3, %ymm13, %ymm14, %ymm15
        MSTEP %ymm9, %ymm10, %ymm11, %ymm6, %ymm7, %ymm8, %ymm0, %ymm1, %ymm2, %ymm3, %ymm4, %ymm5, %ymm2, %ymm13, %ymm14, %ymm15
        MSTEP %ymm8, %ymm9, %ymm10, %ymm11, %ymm6, %ymm7, %ymm0, %ymm1, %ymm2, %ymm3, %ymm4, %ymm5, %ymm1, %ymm13, %ymm14, %ymm15
        MSTEP %ymm7, %ymm8, %ymm9, %ymm10, %ymm11, %ymm6, %ymm0, %ymm1, %ymm2, %ymm3, %ymm4, %ymm5, %ymm0, %ymm13, %ymm14, %ymm15
	CARRYR %ymm6, %ymm7, %ymm8, %ymm9, %ymm10, %ymm11, %ymm14, %ymm15

	/* ymm0..5 := (a, a, a, a) (a**2, a**2, a**2, a**2) -> (a**2, a, a**2, a) */
	vblendpd $0x5, %ymm6, %ymm0, %ymm0
        vblendpd $0x5, %ymm7, %ymm1, %ymm1
        vblendpd $0x5, %ymm8, %ymm2, %ymm2
        vblendpd $0x5, %ymm9, %ymm3, %ymm3
        vblendpd $0x5, %ymm10, %ymm4, %ymm4
        vblendpd $0x5, %ymm11, %ymm5, %ymm5

	/* ymm6..11 := (a**2, a, a**2, a) * (a**2, a**2, a**2, a**2) -> (a**4, a**3, a**4, a**3) */
        vmulpd %ymm1, %ymm11, %ymm6
        vmulpd %ymm2, %ymm11, %ymm7
        vmulpd %ymm3, %ymm11, %ymm8
        vmulpd %ymm4, %ymm11, %ymm9
        vmulpd %ymm5, %ymm11, %ymm10
        vmulpd %ymm0, %ymm11, %ymm11

	/* ymm12 := (a**2, a, a**2, a) -> (a**2, a**2, a**2, a**2) */
	/* vmovddup avoids stack */
	vmovddup %ymm4, %ymm12
        MSTEP %ymm11, %ymm6, %ymm7, %ymm8, %ymm9, %ymm10, %ymm0, %ymm1, %ymm2, %ymm3, %ymm4, %ymm5, %ymm12, %ymm13, %ymm14, %ymm15
	vmovddup %ymm3, %ymm12
        MSTEP %ymm10, %ymm11, %ymm6, %ymm7, %ymm8, %ymm9, %ymm0, %ymm1, %ymm2, %ymm3, %ymm4, %ymm5, %ymm12, %ymm13, %ymm14, %ymm15
        vmovddup %ymm2, %ymm12
        MSTEP %ymm9, %ymm10, %ymm11, %ymm6, %ymm7, %ymm8, %ymm0, %ymm1, %ymm2, %ymm3, %ymm4, %ymm5, %ymm12, %ymm13, %ymm14, %ymm15
        vmovddup %ymm1, %ymm12
        MSTEP %ymm8, %ymm9, %ymm10, %ymm11, %ymm6, %ymm7, %ymm0, %ymm1, %ymm2, %ymm3, %ymm4, %ymm5, %ymm12, %ymm13, %ymm14, %ymm15
        vmovddup %ymm0, %ymm12
        MSTEP %ymm7, %ymm8, %ymm9, %ymm10, %ymm11, %ymm6, %ymm0, %ymm1, %ymm2, %ymm3, %ymm4, %ymm5, %ymm12, %ymm13, %ymm14, %ymm15
	CARRYR %ymm6, %ymm7, %ymm8, %ymm9, %ymm10, %ymm11, %ymm14, %ymm15

	/* ymm0..5 := (a**2, a, a**2, a) (a**4, a**3, a**4, a**3) -> (a**4, a**3, a**2, a) */
	vblendpd $0x3, %ymm6, %ymm0, %ymm0
        vblendpd $0x3, %ymm7, %ymm1, %ymm1
        vblendpd $0x3, %ymm8, %ymm2, %ymm2
        vblendpd $0x3, %ymm9, %ymm3, %ymm3
        vblendpd $0x3, %ymm10, %ymm4, %ymm4
        vblendpd $0x3, %ymm11, %ymm5, %ymm5

	/* (a**4, a**3, a**2, a) to stack for after main loop */
	leaq -192(%rsp), %rsp
	vmovapd %ymm0, 0(%rsp)
        vmovapd %ymm1, 32(%rsp)
        vmovapd %ymm2, 64(%rsp)
        vmovapd %ymm3, 96(%rsp)
        vmovapd %ymm4, 128(%rsp)
        vmovapd %ymm5, 160(%rsp)

	/* ymm6..11 := (a**4, a**3, a**4, a**3) -> (a**4, a**4, a**4, a**4) */
	vmovddup %ymm6, %ymm6
        vmovddup %ymm7, %ymm7
        vmovddup %ymm8, %ymm8
        vmovddup %ymm9, %ymm9
        vmovddup %ymm10, %ymm10
        vmovddup %ymm11, %ymm11

	/* (a**4, a**4, a**4, a**4) to stack for main loop */
	leaq -192(%rsp), %rsp
        vmovupd %ymm6, 0(%rsp)
        vmovupd %ymm7, 32(%rsp)
        vmovupd %ymm8, 64(%rsp)
        vmovupd %ymm9, 96(%rsp)
        vmovupd %ymm10, 128(%rsp)
        vmovupd %ymm11, 160(%rsp)

	/* initialize accumulator ymm0..5 */
	vxorpd %ymm0, %ymm0, %ymm0
        vxorpd %ymm1, %ymm1, %ymm1
        vxorpd %ymm2, %ymm2, %ymm2
        vxorpd %ymm3, %ymm3, %ymm3
        vxorpd %ymm4, %ymm4, %ymm4
        vxorpd %ymm5, %ymm5, %ymm5

Laccumulate:
	/* NB: careful not to clobber accumulator ymm0..5 */
	/* load, slice message data */
	movdqu 0(%rdx), %xmm9		# 0123
	movdqu 16(%rdx), %xmm10		# 4567
	movdqu 32(%rdx), %xmm11		# 89ab
	movdqu 48(%rdx), %xmm12		# cdef

	/* slice columns 0-1 */
	movdqa %xmm9, %xmm13
	movdqa %xmm11, %xmm14
	punpckldq %xmm10, %xmm13	# 0415
	punpckldq %xmm12, %xmm14	# 8c9d
	movdqa %xmm13, %xmm15
	punpcklqdq %xmm14, %xmm13	# 048c
	punpckhqdq %xmm14, %xmm15	# 159d

	/* slice columns 2-3 */
	punpckhdq %xmm10, %xmm9		# 2637
	punpckhdq %xmm12, %xmm11	# aebf
	movdqa %xmm9, %xmm14
	punpcklqdq %xmm11, %xmm9	# 26ae
	punpckhqdq %xmm11, %xmm14	# 37bf

	/* *signed* convert */
	vcvtdq2pd %xmm13, %ymm6		# 048c
	vcvtdq2pd %xmm15, %ymm7		# 159d
	vcvtdq2pd %xmm9, %ymm8		# 26ae
	vcvtdq2pd %xmm14, %ymm9		# 37bf

	/* fix sign */
	vmovapd POW232, %ymm14
	vxorpd %ymm15, %ymm15, %ymm15
	vblendvpd %ymm6, %ymm14, %ymm15, %ymm10
	vblendvpd %ymm7, %ymm14, %ymm15, %ymm11
	vblendvpd %ymm8, %ymm14, %ymm15, %ymm12
	vblendvpd %ymm9, %ymm14, %ymm15, %ymm13
	vaddpd %ymm10, %ymm6, %ymm6
	vaddpd %ymm11, %ymm7, %ymm7
	vaddpd %ymm12, %ymm8, %ymm8
	vaddpd %ymm13, %ymm9, %ymm10

	/* adjust exponent */
	vmulpd %ymm14, %ymm7, %ymm7
	vmulpd POW264, %ymm8, %ymm8
	vmulpd POW296, %ymm10, %ymm10

	/* accumulate, add in message data, padding */
	vaddpd %ymm6, %ymm0, %ymm0
	vaddpd %ymm7, %ymm1, %ymm1
	vaddpd %ymm8, %ymm2, %ymm2
	vaddpd %ymm10, %ymm4, %ymm4
	vaddpd POW2128, %ymm5, %ymm5

	/* balance bits across polynomial */
	/* this 2-way parallel chain wins over generic */
	vmovapd ALPHA44, %ymm12
	vmovapd ALPHA109, %ymm14
	vaddpd %ymm12, %ymm1, %ymm13
	vaddpd %ymm14, %ymm4, %ymm15
	vsubpd %ymm12, %ymm13, %ymm13
	vsubpd %ymm14, %ymm15, %ymm15
	vsubpd %ymm13, %ymm1, %ymm1
	vsubpd %ymm15, %ymm4, %ymm4
	vaddpd %ymm13, %ymm2, %ymm2
	vaddpd %ymm15, %ymm5, %ymm5
	vmovapd ALPHA65, %ymm12
	vmovapd ALPHA130, %ymm14
	vaddpd %ymm12, %ymm2, %ymm13
	vaddpd %ymm14, %ymm5, %ymm15
	vsubpd %ymm12, %ymm13, %ymm13
	vsubpd %ymm14, %ymm15, %ymm15
	vsubpd %ymm13, %ymm2, %ymm2
	vsubpd %ymm15, %ymm5, %ymm5
	vmulpd SCALE, %ymm15, %ymm15
	vaddpd %ymm13, %ymm3, %ymm3
	vaddpd %ymm15, %ymm0, %ymm0
	vmovapd ALPHA22, %ymm12
	vmovapd ALPHA87, %ymm14
	vaddpd %ymm12, %ymm0, %ymm13
	vaddpd %ymm14, %ymm3, %ymm15
	vsubpd %ymm12, %ymm13, %ymm13
	vsubpd %ymm14, %ymm15, %ymm15
	vsubpd %ymm13, %ymm0, %ymm6	# ymm6 := ymm0
	vsubpd %ymm15, %ymm3, %ymm9	# ymm9 := ymm3
	vaddpd %ymm13, %ymm1, %ymm1
	vaddpd %ymm15, %ymm4, %ymm4
	vmovapd ALPHA44, %ymm12
	vmovapd ALPHA109, %ymm14
	vaddpd %ymm12, %ymm1, %ymm13
	vaddpd %ymm14, %ymm4, %ymm15
	vsubpd %ymm12, %ymm13, %ymm13
	vsubpd %ymm14, %ymm15, %ymm15
	vsubpd %ymm13, %ymm1, %ymm7	# ymm7 := ymm1
	vsubpd %ymm15, %ymm4, %ymm10	# ymm10 := ymm4
	vaddpd %ymm13, %ymm2, %ymm8	# ymm8 := ymm2
	vaddpd %ymm15, %ymm5, %ymm11	# ymm11 := ymm5
	/* this is the generic carry chain */
	#CARRYR %ymm6, %ymm7, %ymm8, %ymm9, %ymm10, %ymm11, %ymm14, %ymm15
	/* ymm6..11 now holds the operand */

	/* adjust message data pointer, len */
	leaq 64(%rdx), %rdx
	leaq -64(%rcx), %rcx
	/* jump down a bit if there's still data */
	testq $0xFFFFFFFFFFFFFFC0, %rcx
	jnz Lmultiply

	/* no remaining data, pop stack */
	leaq 192(%rsp), %rsp

Lmultiply:
	/**
	 * multiply by some form of the point; two cases.
	 * ymm0..5 := (z0, z1, z2, z3) * (a**4, a**4, a**4, a**4)
	 * ymm0..5 := (z0, z1, z2, z3) * (a**4, a**3, a**2, a**1)
	 */
        vmovapd 160(%rsp), %ymm12
        vmulpd %ymm6, %ymm12, %ymm5
        vmulpd %ymm7, %ymm12, %ymm0
        vmulpd %ymm8, %ymm12, %ymm1
        vmulpd %ymm9, %ymm12, %ymm2
        vmulpd %ymm10, %ymm12, %ymm3
        vmulpd %ymm11, %ymm12, %ymm4

        vmovapd 128(%rsp), %ymm12
        MSTEP %ymm5, %ymm0, %ymm1, %ymm2, %ymm3, %ymm4, %ymm6, %ymm7, %ymm8, %ymm9, %ymm10, %ymm11, %ymm12, %ymm13, %ymm14, %ymm15
        vmovapd 96(%rsp), %ymm12
        MSTEP %ymm4, %ymm5, %ymm0, %ymm1, %ymm2, %ymm3, %ymm6, %ymm7, %ymm8, %ymm9, %ymm10, %ymm11, %ymm12, %ymm13, %ymm14, %ymm15
        vmovapd 64(%rsp), %ymm12
        MSTEP %ymm3, %ymm4, %ymm5, %ymm0, %ymm1, %ymm2, %ymm6, %ymm7, %ymm8, %ymm9, %ymm10, %ymm11, %ymm12, %ymm13, %ymm14, %ymm15
        vmovapd 32(%rsp), %ymm12
        MSTEP %ymm2, %ymm3, %ymm4, %ymm5, %ymm0, %ymm1, %ymm6, %ymm7, %ymm8, %ymm9, %ymm10, %ymm11, %ymm12, %ymm13, %ymm14, %ymm15
        vmovapd 0(%rsp), %ymm12
        MSTEP %ymm1, %ymm2, %ymm3, %ymm4, %ymm5, %ymm0, %ymm6, %ymm7, %ymm8, %ymm9, %ymm10, %ymm11, %ymm12, %ymm13, %ymm14, %ymm15

	/* balance the polynomial elsewhere */
        #CARRYR %ymm0, %ymm1, %ymm2, %ymm3, %ymm4, %ymm5, %ymm14, %ymm15

	/* jump back if there's still data */
	/* NB: careful not to clobber zero flag */
	jnz Laccumulate

	/* pop final mult operand from the stack */
	leaq 192(%rsp), %rsp

	/* balance bits across polynomial */
        CARRYR %ymm0, %ymm1, %ymm2, %ymm3, %ymm4, %ymm5, %ymm14, %ymm15

	/* add in (p+p) to make coefficients positive. */
	vaddpd P0, %ymm0, %ymm0
	vaddpd P22, %ymm1, %ymm1
	vaddpd P44, %ymm2, %ymm2
	vaddpd P65, %ymm3, %ymm3
	vaddpd P87, %ymm4, %ymm4
	vaddpd P109, %ymm5, %ymm5

	/* scale coefficients down */
	vmulpd POW222I, %ymm1, %ymm1
	vmulpd POW244I, %ymm2, %ymm2
	vmulpd POW265I, %ymm3, %ymm3
	vmulpd POW287I, %ymm4, %ymm4
	vmulpd POW2109I, %ymm5, %ymm5

	/* convert to integers */
	vcvttpd2dq %ymm0, %xmm0
	vcvttpd2dq %ymm1, %xmm1
	vcvttpd2dq %ymm2, %xmm2
	vcvttpd2dq %ymm3, %xmm3
	vcvttpd2dq %ymm4, %xmm4
	vcvttpd2dq %ymm5, %xmm5

	/* sum the polynomials */
	phaddd %xmm0, %xmm0
	phaddd %xmm1, %xmm1
	phaddd %xmm2, %xmm2
	phaddd %xmm3, %xmm3
	phaddd %xmm4, %xmm4
	phaddd %xmm5, %xmm5
	phaddd %xmm0, %xmm0
	phaddd %xmm1, %xmm1
	phaddd %xmm2, %xmm2
	phaddd %xmm3, %xmm3
	phaddd %xmm4, %xmm4
	phaddd %xmm5, %xmm5

	/* rcx < 64 holds at this point. */
	/* restore stack */
	movq %rax, %rsp

	/* construct top two words first */
	pextrq $0, %xmm2, %r8
	pextrq $0, %xmm3, %r9
	pextrq $0, %xmm4, %r10
	pextrq $0, %xmm5, %r12
	andq $0x7FFFFFFF, %r8
	andq $0x7FFFFFFF, %r9
	andq $0x7FFFFFFF, %r10
	andq $0x7FFFFFFF, %r12
	movq %r8, %rax
	shrq $20, %rax
	shlq $1, %r9
	shlq $23, %r10
	movq %r12, %r11
	shlq $45, %r11
	shrq $19, %r12
	addq %rax, %r9
	addq %r9, %r10
	addq %r10, %r11
	adcq $0, %r12

	/* construct bottom word */
	shlq $44, %r8
	pextrq $0, %xmm0, %r9
	pextrq $0, %xmm1, %r10
	andq $0x7FFFFFFF, %r9
	andq $0x7FFFFFFF, %r10
	shlq $22, %r10
	addq %r9, %r10
	addq %r8, %r10
	adcq $0, %r11
	adcq $0, %r12
	/* (r10, r11, r12) holds result r */

Lfinalize:
	/* handle end of msg. */
	cmp $0, %rcx
	jle Loutput

	/* save stack */
	movq %rsp, %r15

	/* rdi := remaining message byte count */
	/* rcx := remaining message block count */
	/* rbp := padding mask, 2**rcx - 1 */
	movq %rcx, %rdi
	addq $0xF, %rcx
	shrq $4, %rcx
	xorq %rbp, %rbp

	/* push empty blocks on the stack, build padding mask */
Lploopa:
	pushq $0
	pushq $0
	leaq 1(%rbp, %rbp), %rbp
	loop Lploopa

	/* rcx := remaining message byte count */
	movq %rdi, %rcx
	testq $0xF, %rcx
	jz Lploopb

	/* pad last block manually */
	movb $1, (%rsp, %rcx)
	shrq $1, %rbp

	/* move remaining message bytes to said blocks on stack */
Lploopb:
	movzbq -1(%rdx, %rcx), %rax
	movb %al, -1(%rsp, %rcx)
	loop Lploopb

	/* fetch the point again */
	movq 0(%rsi), %rbx
	movq 8(%rsi), %rdi
	movq $0x0FFFFFFC0FFFFFFF, %r8
	movq $0x0FFFFFFC0FFFFFFC, %r9
	andq %r8, %rbx
	andq %r9, %rdi

Lploopc:
	/* pop next message block */
	popq %r8
	popq %r9
	/* accumulate */
	addq %r8, %r10
	adcq %r9, %r11
	adcq $0, %r12
	/* throw in the padding */
	shrq $1, %rbp
	adcq $0, %r12
	/* multiply */
	MULI %r8, %r9, %r13, %r14, %rcx, %r10, %r11, %r12, %rbx, %rdi
	movq %r8, %r10
	movq %r9, %r11
	movq %r13, %r12
	cmp %rsp, %r15
	jg Lploopc

Loutput:
	/* r mod p, first pass */
	movq %r12, %r13
	andq $0x3, %r12
	shrq $2, %r13
	leaq (%r13, %r13, 4), %r13
	addq %r13, %r10
	adcq $0, %r11
	adcq $0, %r12
	/* 0 <= r <= 2**130 + c for some very small positive c. */

	/* construct r - p */
	movq %r10, %r13
	movq %r11, %r14
	movq %r12, %r15
	subq $0xFFFFFFFFFFFFFFFB, %r13
	sbbq $0xFFFFFFFFFFFFFFFF, %r14
	sbbq $0x3, %r15
	sbbq %rcx, %rcx
	/* (r13, r14, r15) holds r - p */

	/* 2-to-1 multiplex, select r or r - p using borrow (rcx) as control wire */
	andq %rcx, %r10
	andq %rcx, %r11
	andq %rcx, %r12
	notq %rcx
	andq %rcx, %r13
	andq %rcx, %r14
	andq %rcx, %r15
	orq %r13, %r10
	orq %r14, %r11
	orq %r15, %r12

        /* fetch one time pad, add it in mod 2**128 */
        movq 16(%rsi), %r8
        movq 24(%rsi), %r9
	addq %r8, %r10
	adcq %r9, %r11
	adcq $0, %r12

	/* fetch output address, store tag */
	popq %rdi
	movq %r10, 0(%rdi)
	movq %r11, 8(%rdi)
	#movq %r12, 16(%rdi)

	/* restore state */
	popq %rbx
	popq %rsp
	popq %rbp
	popq %r12
	popq %r13
	popq %r14
	popq %r15

	/* done */
	xorq %rax,%rax
	xorq %rdx,%rdx
	ret

/*
radix
0 22 44 65 87 109
gp regs
rax rbx rcx rdx rbp rsp rsi rdi r8 r9 r10 r11 r12 r13 r14 r15
scratch regs
rax rcx rdx rsi rdi r8 r9 r10 r11
calling convention
rdi rsi rdx rcx r8 r9
mulq %foo # (rax, rdx) := foo*rax (l,h)
K = GF(2**130-5)
R.<x> = K[]
*/
