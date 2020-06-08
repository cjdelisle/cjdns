
# qhasm: int64 workp

# qhasm: int64 swap

# qhasm: input workp

# qhasm: input swap

# qhasm: int64 w0

# qhasm: int64 w1

# qhasm: int64 w2

# qhasm: int64 w3

# qhasm: int64 w4

# qhasm: int64 w5

# qhasm: int64 w6

# qhasm: int64 w7

# qhasm: int64 w8

# qhasm: int64 w9

# qhasm: int64 w10

# qhasm: int64 w11

# qhasm: int64 w12

# qhasm: int64 w13

# qhasm: int64 w14

# qhasm: int64 w15

# qhasm: int64 t

# qhasm:   int64 caller1

# qhasm:   int64 caller2

# qhasm:   int64 caller3

# qhasm:   int64 caller4

# qhasm:   int64 caller5

# qhasm:   int64 caller6

# qhasm:   int64 caller7

# qhasm:   caller caller1

# qhasm:   caller caller2

# qhasm:   caller caller3

# qhasm:   caller caller4

# qhasm:   caller caller5

# qhasm:   caller caller6

# qhasm:   caller caller7

# qhasm:   stack64 caller1_stack

# qhasm:   stack64 caller2_stack

# qhasm:   stack64 caller3_stack

# qhasm:   stack64 caller4_stack

# qhasm:   stack64 caller5_stack

# qhasm:   stack64 caller6_stack

# qhasm:   stack64 caller7_stack

# qhasm: enter crypto_scalarmult_curve25519_amd64_64_work_cswap
.text
.p2align 5
.globl _crypto_scalarmult_curve25519_amd64_64_work_cswap
.globl crypto_scalarmult_curve25519_amd64_64_work_cswap
_crypto_scalarmult_curve25519_amd64_64_work_cswap:
crypto_scalarmult_curve25519_amd64_64_work_cswap:
mov %rsp,%r11
and $31,%r11
add $0,%r11
sub %r11,%rsp

# qhasm: =? swap - 1
# asm 1: cmp  $1,<swap=int64#2
# asm 2: cmp  $1,<swap=%rsi
cmp  $1,%rsi

# qhasm: w0  = *(uint64 *)(workp +   0)
# asm 1: movq   0(<workp=int64#1),>w0=int64#2
# asm 2: movq   0(<workp=%rdi),>w0=%rsi
movq   0(%rdi),%rsi

# qhasm: w8  = *(uint64 *)(workp +  64)
# asm 1: movq   64(<workp=int64#1),>w8=int64#3
# asm 2: movq   64(<workp=%rdi),>w8=%rdx
movq   64(%rdi),%rdx

# qhasm: t = w0
# asm 1: mov  <w0=int64#2,>t=int64#4
# asm 2: mov  <w0=%rsi,>t=%rcx
mov  %rsi,%rcx

# qhasm: w0  = w8 if =
# asm 1: cmove <w8=int64#3,<w0=int64#2
# asm 2: cmove <w8=%rdx,<w0=%rsi
cmove %rdx,%rsi

# qhasm: w8  = t if =
# asm 1: cmove <t=int64#4,<w8=int64#3
# asm 2: cmove <t=%rcx,<w8=%rdx
cmove %rcx,%rdx

# qhasm: *(uint64 *)(workp +   0) = w0
# asm 1: movq   <w0=int64#2,0(<workp=int64#1)
# asm 2: movq   <w0=%rsi,0(<workp=%rdi)
movq   %rsi,0(%rdi)

# qhasm: *(uint64 *)(workp +  64) = w8
# asm 1: movq   <w8=int64#3,64(<workp=int64#1)
# asm 2: movq   <w8=%rdx,64(<workp=%rdi)
movq   %rdx,64(%rdi)

# qhasm: w1  = *(uint64 *)(workp +   8)
# asm 1: movq   8(<workp=int64#1),>w1=int64#2
# asm 2: movq   8(<workp=%rdi),>w1=%rsi
movq   8(%rdi),%rsi

# qhasm: w9  = *(uint64 *)(workp +  72)
# asm 1: movq   72(<workp=int64#1),>w9=int64#3
# asm 2: movq   72(<workp=%rdi),>w9=%rdx
movq   72(%rdi),%rdx

# qhasm: t = w1
# asm 1: mov  <w1=int64#2,>t=int64#4
# asm 2: mov  <w1=%rsi,>t=%rcx
mov  %rsi,%rcx

# qhasm: w1  = w9 if =
# asm 1: cmove <w9=int64#3,<w1=int64#2
# asm 2: cmove <w9=%rdx,<w1=%rsi
cmove %rdx,%rsi

# qhasm: w9  = t if =
# asm 1: cmove <t=int64#4,<w9=int64#3
# asm 2: cmove <t=%rcx,<w9=%rdx
cmove %rcx,%rdx

# qhasm: *(uint64 *)(workp +   8) = w1
# asm 1: movq   <w1=int64#2,8(<workp=int64#1)
# asm 2: movq   <w1=%rsi,8(<workp=%rdi)
movq   %rsi,8(%rdi)

# qhasm: *(uint64 *)(workp +  72) = w9
# asm 1: movq   <w9=int64#3,72(<workp=int64#1)
# asm 2: movq   <w9=%rdx,72(<workp=%rdi)
movq   %rdx,72(%rdi)

# qhasm: w2  = *(uint64 *)(workp +  16)
# asm 1: movq   16(<workp=int64#1),>w2=int64#2
# asm 2: movq   16(<workp=%rdi),>w2=%rsi
movq   16(%rdi),%rsi

# qhasm: w10 = *(uint64 *)(workp +  80)
# asm 1: movq   80(<workp=int64#1),>w10=int64#3
# asm 2: movq   80(<workp=%rdi),>w10=%rdx
movq   80(%rdi),%rdx

# qhasm: t = w2
# asm 1: mov  <w2=int64#2,>t=int64#4
# asm 2: mov  <w2=%rsi,>t=%rcx
mov  %rsi,%rcx

# qhasm: w2  = w10 if =
# asm 1: cmove <w10=int64#3,<w2=int64#2
# asm 2: cmove <w10=%rdx,<w2=%rsi
cmove %rdx,%rsi

# qhasm: w10 = t if =
# asm 1: cmove <t=int64#4,<w10=int64#3
# asm 2: cmove <t=%rcx,<w10=%rdx
cmove %rcx,%rdx

# qhasm: *(uint64 *)(workp +  16) = w2
# asm 1: movq   <w2=int64#2,16(<workp=int64#1)
# asm 2: movq   <w2=%rsi,16(<workp=%rdi)
movq   %rsi,16(%rdi)

# qhasm: *(uint64 *)(workp +  80) = w10
# asm 1: movq   <w10=int64#3,80(<workp=int64#1)
# asm 2: movq   <w10=%rdx,80(<workp=%rdi)
movq   %rdx,80(%rdi)

# qhasm: w3  = *(uint64 *)(workp +  24)
# asm 1: movq   24(<workp=int64#1),>w3=int64#2
# asm 2: movq   24(<workp=%rdi),>w3=%rsi
movq   24(%rdi),%rsi

# qhasm: w11 = *(uint64 *)(workp +  88)
# asm 1: movq   88(<workp=int64#1),>w11=int64#3
# asm 2: movq   88(<workp=%rdi),>w11=%rdx
movq   88(%rdi),%rdx

# qhasm: t = w3
# asm 1: mov  <w3=int64#2,>t=int64#4
# asm 2: mov  <w3=%rsi,>t=%rcx
mov  %rsi,%rcx

# qhasm: w3  = w11 if =
# asm 1: cmove <w11=int64#3,<w3=int64#2
# asm 2: cmove <w11=%rdx,<w3=%rsi
cmove %rdx,%rsi

# qhasm: w11 = t if =
# asm 1: cmove <t=int64#4,<w11=int64#3
# asm 2: cmove <t=%rcx,<w11=%rdx
cmove %rcx,%rdx

# qhasm: *(uint64 *)(workp +  24) = w3
# asm 1: movq   <w3=int64#2,24(<workp=int64#1)
# asm 2: movq   <w3=%rsi,24(<workp=%rdi)
movq   %rsi,24(%rdi)

# qhasm: *(uint64 *)(workp +  88) = w11
# asm 1: movq   <w11=int64#3,88(<workp=int64#1)
# asm 2: movq   <w11=%rdx,88(<workp=%rdi)
movq   %rdx,88(%rdi)

# qhasm: w4  = *(uint64 *)(workp +  32)
# asm 1: movq   32(<workp=int64#1),>w4=int64#2
# asm 2: movq   32(<workp=%rdi),>w4=%rsi
movq   32(%rdi),%rsi

# qhasm: w12 = *(uint64 *)(workp +  96)
# asm 1: movq   96(<workp=int64#1),>w12=int64#3
# asm 2: movq   96(<workp=%rdi),>w12=%rdx
movq   96(%rdi),%rdx

# qhasm: t = w4
# asm 1: mov  <w4=int64#2,>t=int64#4
# asm 2: mov  <w4=%rsi,>t=%rcx
mov  %rsi,%rcx

# qhasm: w4  = w12 if =
# asm 1: cmove <w12=int64#3,<w4=int64#2
# asm 2: cmove <w12=%rdx,<w4=%rsi
cmove %rdx,%rsi

# qhasm: w12 = t if =
# asm 1: cmove <t=int64#4,<w12=int64#3
# asm 2: cmove <t=%rcx,<w12=%rdx
cmove %rcx,%rdx

# qhasm: *(uint64 *)(workp +  32) = w4
# asm 1: movq   <w4=int64#2,32(<workp=int64#1)
# asm 2: movq   <w4=%rsi,32(<workp=%rdi)
movq   %rsi,32(%rdi)

# qhasm: *(uint64 *)(workp +  96) = w12
# asm 1: movq   <w12=int64#3,96(<workp=int64#1)
# asm 2: movq   <w12=%rdx,96(<workp=%rdi)
movq   %rdx,96(%rdi)

# qhasm: w5  = *(uint64 *)(workp +  40)
# asm 1: movq   40(<workp=int64#1),>w5=int64#2
# asm 2: movq   40(<workp=%rdi),>w5=%rsi
movq   40(%rdi),%rsi

# qhasm: w13 = *(uint64 *)(workp + 104)
# asm 1: movq   104(<workp=int64#1),>w13=int64#3
# asm 2: movq   104(<workp=%rdi),>w13=%rdx
movq   104(%rdi),%rdx

# qhasm: t = w5
# asm 1: mov  <w5=int64#2,>t=int64#4
# asm 2: mov  <w5=%rsi,>t=%rcx
mov  %rsi,%rcx

# qhasm: w5  = w13 if =
# asm 1: cmove <w13=int64#3,<w5=int64#2
# asm 2: cmove <w13=%rdx,<w5=%rsi
cmove %rdx,%rsi

# qhasm: w13 = t if =
# asm 1: cmove <t=int64#4,<w13=int64#3
# asm 2: cmove <t=%rcx,<w13=%rdx
cmove %rcx,%rdx

# qhasm: *(uint64 *)(workp +  40) = w5
# asm 1: movq   <w5=int64#2,40(<workp=int64#1)
# asm 2: movq   <w5=%rsi,40(<workp=%rdi)
movq   %rsi,40(%rdi)

# qhasm: *(uint64 *)(workp + 104) = w13
# asm 1: movq   <w13=int64#3,104(<workp=int64#1)
# asm 2: movq   <w13=%rdx,104(<workp=%rdi)
movq   %rdx,104(%rdi)

# qhasm: w6  = *(uint64 *)(workp +  48)
# asm 1: movq   48(<workp=int64#1),>w6=int64#2
# asm 2: movq   48(<workp=%rdi),>w6=%rsi
movq   48(%rdi),%rsi

# qhasm: w14 = *(uint64 *)(workp + 112)
# asm 1: movq   112(<workp=int64#1),>w14=int64#3
# asm 2: movq   112(<workp=%rdi),>w14=%rdx
movq   112(%rdi),%rdx

# qhasm: t = w6
# asm 1: mov  <w6=int64#2,>t=int64#4
# asm 2: mov  <w6=%rsi,>t=%rcx
mov  %rsi,%rcx

# qhasm: w6  = w14 if =
# asm 1: cmove <w14=int64#3,<w6=int64#2
# asm 2: cmove <w14=%rdx,<w6=%rsi
cmove %rdx,%rsi

# qhasm: w14 = t if =
# asm 1: cmove <t=int64#4,<w14=int64#3
# asm 2: cmove <t=%rcx,<w14=%rdx
cmove %rcx,%rdx

# qhasm: *(uint64 *)(workp +  48) = w6
# asm 1: movq   <w6=int64#2,48(<workp=int64#1)
# asm 2: movq   <w6=%rsi,48(<workp=%rdi)
movq   %rsi,48(%rdi)

# qhasm: *(uint64 *)(workp + 112) = w14
# asm 1: movq   <w14=int64#3,112(<workp=int64#1)
# asm 2: movq   <w14=%rdx,112(<workp=%rdi)
movq   %rdx,112(%rdi)

# qhasm: w7  = *(uint64 *)(workp +  56)
# asm 1: movq   56(<workp=int64#1),>w7=int64#2
# asm 2: movq   56(<workp=%rdi),>w7=%rsi
movq   56(%rdi),%rsi

# qhasm: w15 = *(uint64 *)(workp + 120)
# asm 1: movq   120(<workp=int64#1),>w15=int64#3
# asm 2: movq   120(<workp=%rdi),>w15=%rdx
movq   120(%rdi),%rdx

# qhasm: t = w7
# asm 1: mov  <w7=int64#2,>t=int64#4
# asm 2: mov  <w7=%rsi,>t=%rcx
mov  %rsi,%rcx

# qhasm: w7  = w15 if =
# asm 1: cmove <w15=int64#3,<w7=int64#2
# asm 2: cmove <w15=%rdx,<w7=%rsi
cmove %rdx,%rsi

# qhasm: w15 = t if =
# asm 1: cmove <t=int64#4,<w15=int64#3
# asm 2: cmove <t=%rcx,<w15=%rdx
cmove %rcx,%rdx

# qhasm: *(uint64 *)(workp +  56) = w7
# asm 1: movq   <w7=int64#2,56(<workp=int64#1)
# asm 2: movq   <w7=%rsi,56(<workp=%rdi)
movq   %rsi,56(%rdi)

# qhasm: *(uint64 *)(workp + 120) = w15
# asm 1: movq   <w15=int64#3,120(<workp=int64#1)
# asm 2: movq   <w15=%rdx,120(<workp=%rdi)
movq   %rdx,120(%rdi)

# qhasm: leave
add %r11,%rsp
mov %rdi,%rax
mov %rsi,%rdx
ret
