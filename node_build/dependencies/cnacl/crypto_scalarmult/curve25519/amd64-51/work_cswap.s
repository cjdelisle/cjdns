
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

# qhasm: int64 w16

# qhasm: int64 w17

# qhasm: int64 w18

# qhasm: int64 w19

# qhasm: int64 t0

# qhasm: int64 t1

# qhasm: int64 t2

# qhasm: int64 t3

# qhasm: int64 t4

# qhasm: int64 t5

# qhasm: int64 t6

# qhasm: int64 t7

# qhasm: int64 t8

# qhasm: int64 t9

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

# qhasm: enter crypto_scalarmult_curve25519_amd64_51_work_cswap
.text
.p2align 5
.globl _crypto_scalarmult_curve25519_amd64_51_work_cswap
.globl crypto_scalarmult_curve25519_amd64_51_work_cswap
_crypto_scalarmult_curve25519_amd64_51_work_cswap:
crypto_scalarmult_curve25519_amd64_51_work_cswap:
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

# qhasm: w10 = *(uint64 *)(workp +  80)
# asm 1: movq   80(<workp=int64#1),>w10=int64#3
# asm 2: movq   80(<workp=%rdi),>w10=%rdx
movq   80(%rdi),%rdx

# qhasm:         w1  = *(uint64 *)(workp +   8)
# asm 1: movq   8(<workp=int64#1),>w1=int64#4
# asm 2: movq   8(<workp=%rdi),>w1=%rcx
movq   8(%rdi),%rcx

# qhasm:         w11 = *(uint64 *)(workp +  88)
# asm 1: movq   88(<workp=int64#1),>w11=int64#5
# asm 2: movq   88(<workp=%rdi),>w11=%r8
movq   88(%rdi),%r8

# qhasm: t0 = w0
# asm 1: mov  <w0=int64#2,>t0=int64#6
# asm 2: mov  <w0=%rsi,>t0=%r9
mov  %rsi,%r9

# qhasm: w0  = w10 if =
# asm 1: cmove <w10=int64#3,<w0=int64#2
# asm 2: cmove <w10=%rdx,<w0=%rsi
cmove %rdx,%rsi

# qhasm: w10 = t0 if =
# asm 1: cmove <t0=int64#6,<w10=int64#3
# asm 2: cmove <t0=%r9,<w10=%rdx
cmove %r9,%rdx

# qhasm:         t1 = w1
# asm 1: mov  <w1=int64#4,>t1=int64#6
# asm 2: mov  <w1=%rcx,>t1=%r9
mov  %rcx,%r9

# qhasm:         w1  = w11 if =
# asm 1: cmove <w11=int64#5,<w1=int64#4
# asm 2: cmove <w11=%r8,<w1=%rcx
cmove %r8,%rcx

# qhasm:         w11 = t1 if =
# asm 1: cmove <t1=int64#6,<w11=int64#5
# asm 2: cmove <t1=%r9,<w11=%r8
cmove %r9,%r8

# qhasm: *(uint64 *)(workp +   0) = w0
# asm 1: movq   <w0=int64#2,0(<workp=int64#1)
# asm 2: movq   <w0=%rsi,0(<workp=%rdi)
movq   %rsi,0(%rdi)

# qhasm: *(uint64 *)(workp +  80) = w10
# asm 1: movq   <w10=int64#3,80(<workp=int64#1)
# asm 2: movq   <w10=%rdx,80(<workp=%rdi)
movq   %rdx,80(%rdi)

# qhasm:         *(uint64 *)(workp +   8) = w1
# asm 1: movq   <w1=int64#4,8(<workp=int64#1)
# asm 2: movq   <w1=%rcx,8(<workp=%rdi)
movq   %rcx,8(%rdi)

# qhasm:         *(uint64 *)(workp +  88) = w11
# asm 1: movq   <w11=int64#5,88(<workp=int64#1)
# asm 2: movq   <w11=%r8,88(<workp=%rdi)
movq   %r8,88(%rdi)

# qhasm: w2  = *(uint64 *)(workp +  16)
# asm 1: movq   16(<workp=int64#1),>w2=int64#2
# asm 2: movq   16(<workp=%rdi),>w2=%rsi
movq   16(%rdi),%rsi

# qhasm: w12 = *(uint64 *)(workp +  96)
# asm 1: movq   96(<workp=int64#1),>w12=int64#3
# asm 2: movq   96(<workp=%rdi),>w12=%rdx
movq   96(%rdi),%rdx

# qhasm:         w3  = *(uint64 *)(workp +  24)
# asm 1: movq   24(<workp=int64#1),>w3=int64#4
# asm 2: movq   24(<workp=%rdi),>w3=%rcx
movq   24(%rdi),%rcx

# qhasm:         w13 = *(uint64 *)(workp + 104)
# asm 1: movq   104(<workp=int64#1),>w13=int64#5
# asm 2: movq   104(<workp=%rdi),>w13=%r8
movq   104(%rdi),%r8

# qhasm: t2 = w2
# asm 1: mov  <w2=int64#2,>t2=int64#6
# asm 2: mov  <w2=%rsi,>t2=%r9
mov  %rsi,%r9

# qhasm: w2  = w12 if =
# asm 1: cmove <w12=int64#3,<w2=int64#2
# asm 2: cmove <w12=%rdx,<w2=%rsi
cmove %rdx,%rsi

# qhasm: w12 = t2 if =
# asm 1: cmove <t2=int64#6,<w12=int64#3
# asm 2: cmove <t2=%r9,<w12=%rdx
cmove %r9,%rdx

# qhasm:         t3 = w3
# asm 1: mov  <w3=int64#4,>t3=int64#6
# asm 2: mov  <w3=%rcx,>t3=%r9
mov  %rcx,%r9

# qhasm:         w3  = w13 if =
# asm 1: cmove <w13=int64#5,<w3=int64#4
# asm 2: cmove <w13=%r8,<w3=%rcx
cmove %r8,%rcx

# qhasm:         w13 = t3 if =
# asm 1: cmove <t3=int64#6,<w13=int64#5
# asm 2: cmove <t3=%r9,<w13=%r8
cmove %r9,%r8

# qhasm: *(uint64 *)(workp +  16) = w2
# asm 1: movq   <w2=int64#2,16(<workp=int64#1)
# asm 2: movq   <w2=%rsi,16(<workp=%rdi)
movq   %rsi,16(%rdi)

# qhasm: *(uint64 *)(workp +  96) = w12
# asm 1: movq   <w12=int64#3,96(<workp=int64#1)
# asm 2: movq   <w12=%rdx,96(<workp=%rdi)
movq   %rdx,96(%rdi)

# qhasm:         *(uint64 *)(workp +  24) = w3
# asm 1: movq   <w3=int64#4,24(<workp=int64#1)
# asm 2: movq   <w3=%rcx,24(<workp=%rdi)
movq   %rcx,24(%rdi)

# qhasm:         *(uint64 *)(workp + 104) = w13
# asm 1: movq   <w13=int64#5,104(<workp=int64#1)
# asm 2: movq   <w13=%r8,104(<workp=%rdi)
movq   %r8,104(%rdi)

# qhasm: w4  = *(uint64 *)(workp +  32)
# asm 1: movq   32(<workp=int64#1),>w4=int64#2
# asm 2: movq   32(<workp=%rdi),>w4=%rsi
movq   32(%rdi),%rsi

# qhasm: w14 = *(uint64 *)(workp + 112)
# asm 1: movq   112(<workp=int64#1),>w14=int64#3
# asm 2: movq   112(<workp=%rdi),>w14=%rdx
movq   112(%rdi),%rdx

# qhasm:         w5  = *(uint64 *)(workp +  40)
# asm 1: movq   40(<workp=int64#1),>w5=int64#4
# asm 2: movq   40(<workp=%rdi),>w5=%rcx
movq   40(%rdi),%rcx

# qhasm:         w15 = *(uint64 *)(workp + 120)
# asm 1: movq   120(<workp=int64#1),>w15=int64#5
# asm 2: movq   120(<workp=%rdi),>w15=%r8
movq   120(%rdi),%r8

# qhasm: t4 = w4
# asm 1: mov  <w4=int64#2,>t4=int64#6
# asm 2: mov  <w4=%rsi,>t4=%r9
mov  %rsi,%r9

# qhasm: w4  = w14 if =
# asm 1: cmove <w14=int64#3,<w4=int64#2
# asm 2: cmove <w14=%rdx,<w4=%rsi
cmove %rdx,%rsi

# qhasm: w14 = t4 if =
# asm 1: cmove <t4=int64#6,<w14=int64#3
# asm 2: cmove <t4=%r9,<w14=%rdx
cmove %r9,%rdx

# qhasm:         t5 = w5
# asm 1: mov  <w5=int64#4,>t5=int64#6
# asm 2: mov  <w5=%rcx,>t5=%r9
mov  %rcx,%r9

# qhasm:         w5  = w15 if =
# asm 1: cmove <w15=int64#5,<w5=int64#4
# asm 2: cmove <w15=%r8,<w5=%rcx
cmove %r8,%rcx

# qhasm:         w15 = t5 if =
# asm 1: cmove <t5=int64#6,<w15=int64#5
# asm 2: cmove <t5=%r9,<w15=%r8
cmove %r9,%r8

# qhasm: *(uint64 *)(workp +  32) = w4
# asm 1: movq   <w4=int64#2,32(<workp=int64#1)
# asm 2: movq   <w4=%rsi,32(<workp=%rdi)
movq   %rsi,32(%rdi)

# qhasm: *(uint64 *)(workp + 112) = w14
# asm 1: movq   <w14=int64#3,112(<workp=int64#1)
# asm 2: movq   <w14=%rdx,112(<workp=%rdi)
movq   %rdx,112(%rdi)

# qhasm:         *(uint64 *)(workp +  40) = w5
# asm 1: movq   <w5=int64#4,40(<workp=int64#1)
# asm 2: movq   <w5=%rcx,40(<workp=%rdi)
movq   %rcx,40(%rdi)

# qhasm:         *(uint64 *)(workp + 120) = w15
# asm 1: movq   <w15=int64#5,120(<workp=int64#1)
# asm 2: movq   <w15=%r8,120(<workp=%rdi)
movq   %r8,120(%rdi)

# qhasm: w6  = *(uint64 *)(workp +  48)
# asm 1: movq   48(<workp=int64#1),>w6=int64#2
# asm 2: movq   48(<workp=%rdi),>w6=%rsi
movq   48(%rdi),%rsi

# qhasm: w16 = *(uint64 *)(workp + 128)
# asm 1: movq   128(<workp=int64#1),>w16=int64#3
# asm 2: movq   128(<workp=%rdi),>w16=%rdx
movq   128(%rdi),%rdx

# qhasm:         w7  = *(uint64 *)(workp +  56)
# asm 1: movq   56(<workp=int64#1),>w7=int64#4
# asm 2: movq   56(<workp=%rdi),>w7=%rcx
movq   56(%rdi),%rcx

# qhasm:         w17 = *(uint64 *)(workp + 136)
# asm 1: movq   136(<workp=int64#1),>w17=int64#5
# asm 2: movq   136(<workp=%rdi),>w17=%r8
movq   136(%rdi),%r8

# qhasm: t6 = w6
# asm 1: mov  <w6=int64#2,>t6=int64#6
# asm 2: mov  <w6=%rsi,>t6=%r9
mov  %rsi,%r9

# qhasm: w6  = w16 if =
# asm 1: cmove <w16=int64#3,<w6=int64#2
# asm 2: cmove <w16=%rdx,<w6=%rsi
cmove %rdx,%rsi

# qhasm: w16 = t6 if =
# asm 1: cmove <t6=int64#6,<w16=int64#3
# asm 2: cmove <t6=%r9,<w16=%rdx
cmove %r9,%rdx

# qhasm:         t7 = w7
# asm 1: mov  <w7=int64#4,>t7=int64#6
# asm 2: mov  <w7=%rcx,>t7=%r9
mov  %rcx,%r9

# qhasm:         w7  = w17 if =
# asm 1: cmove <w17=int64#5,<w7=int64#4
# asm 2: cmove <w17=%r8,<w7=%rcx
cmove %r8,%rcx

# qhasm:         w17 = t7 if =
# asm 1: cmove <t7=int64#6,<w17=int64#5
# asm 2: cmove <t7=%r9,<w17=%r8
cmove %r9,%r8

# qhasm: *(uint64 *)(workp +  48) = w6
# asm 1: movq   <w6=int64#2,48(<workp=int64#1)
# asm 2: movq   <w6=%rsi,48(<workp=%rdi)
movq   %rsi,48(%rdi)

# qhasm: *(uint64 *)(workp + 128) = w16
# asm 1: movq   <w16=int64#3,128(<workp=int64#1)
# asm 2: movq   <w16=%rdx,128(<workp=%rdi)
movq   %rdx,128(%rdi)

# qhasm:         *(uint64 *)(workp +  56) = w7
# asm 1: movq   <w7=int64#4,56(<workp=int64#1)
# asm 2: movq   <w7=%rcx,56(<workp=%rdi)
movq   %rcx,56(%rdi)

# qhasm:         *(uint64 *)(workp + 136) = w17
# asm 1: movq   <w17=int64#5,136(<workp=int64#1)
# asm 2: movq   <w17=%r8,136(<workp=%rdi)
movq   %r8,136(%rdi)

# qhasm: w8  = *(uint64 *)(workp +  64)
# asm 1: movq   64(<workp=int64#1),>w8=int64#2
# asm 2: movq   64(<workp=%rdi),>w8=%rsi
movq   64(%rdi),%rsi

# qhasm: w18 = *(uint64 *)(workp + 144)
# asm 1: movq   144(<workp=int64#1),>w18=int64#3
# asm 2: movq   144(<workp=%rdi),>w18=%rdx
movq   144(%rdi),%rdx

# qhasm:         w9  = *(uint64 *)(workp +  72)
# asm 1: movq   72(<workp=int64#1),>w9=int64#4
# asm 2: movq   72(<workp=%rdi),>w9=%rcx
movq   72(%rdi),%rcx

# qhasm:         w19 = *(uint64 *)(workp + 152)
# asm 1: movq   152(<workp=int64#1),>w19=int64#5
# asm 2: movq   152(<workp=%rdi),>w19=%r8
movq   152(%rdi),%r8

# qhasm: t8 = w8
# asm 1: mov  <w8=int64#2,>t8=int64#6
# asm 2: mov  <w8=%rsi,>t8=%r9
mov  %rsi,%r9

# qhasm: w8  = w18 if =
# asm 1: cmove <w18=int64#3,<w8=int64#2
# asm 2: cmove <w18=%rdx,<w8=%rsi
cmove %rdx,%rsi

# qhasm: w18 = t8 if =
# asm 1: cmove <t8=int64#6,<w18=int64#3
# asm 2: cmove <t8=%r9,<w18=%rdx
cmove %r9,%rdx

# qhasm:         t9 = w9
# asm 1: mov  <w9=int64#4,>t9=int64#6
# asm 2: mov  <w9=%rcx,>t9=%r9
mov  %rcx,%r9

# qhasm:         w9  = w19 if =
# asm 1: cmove <w19=int64#5,<w9=int64#4
# asm 2: cmove <w19=%r8,<w9=%rcx
cmove %r8,%rcx

# qhasm:         w19 = t9 if =
# asm 1: cmove <t9=int64#6,<w19=int64#5
# asm 2: cmove <t9=%r9,<w19=%r8
cmove %r9,%r8

# qhasm: *(uint64 *)(workp +  64) = w8
# asm 1: movq   <w8=int64#2,64(<workp=int64#1)
# asm 2: movq   <w8=%rsi,64(<workp=%rdi)
movq   %rsi,64(%rdi)

# qhasm: *(uint64 *)(workp + 144) = w18
# asm 1: movq   <w18=int64#3,144(<workp=int64#1)
# asm 2: movq   <w18=%rdx,144(<workp=%rdi)
movq   %rdx,144(%rdi)

# qhasm:         *(uint64 *)(workp +  72) = w9
# asm 1: movq   <w9=int64#4,72(<workp=int64#1)
# asm 2: movq   <w9=%rcx,72(<workp=%rdi)
movq   %rcx,72(%rdi)

# qhasm:         *(uint64 *)(workp + 152) = w19
# asm 1: movq   <w19=int64#5,152(<workp=int64#1)
# asm 2: movq   <w19=%r8,152(<workp=%rdi)
movq   %r8,152(%rdi)

# qhasm: leave
add %r11,%rsp
mov %rdi,%rax
mov %rsi,%rdx
ret
