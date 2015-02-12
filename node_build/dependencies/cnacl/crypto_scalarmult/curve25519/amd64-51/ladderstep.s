
# qhasm: int64 workp

# qhasm: input workp

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

# qhasm: int64 t10

# qhasm: int64 t11

# qhasm: int64 t12

# qhasm: int64 t13

# qhasm: int64 t14

# qhasm: stack64 t10_stack

# qhasm: stack64 t11_stack

# qhasm: stack64 t12_stack

# qhasm: stack64 t13_stack

# qhasm: stack64 t14_stack

# qhasm: int64 t20

# qhasm: int64 t21

# qhasm: int64 t22

# qhasm: int64 t23

# qhasm: int64 t24

# qhasm: stack64 t20_stack

# qhasm: stack64 t21_stack

# qhasm: stack64 t22_stack

# qhasm: stack64 t23_stack

# qhasm: stack64 t24_stack

# qhasm: int64 t30

# qhasm: int64 t31

# qhasm: int64 t32

# qhasm: int64 t33

# qhasm: int64 t34

# qhasm: stack64 t30_stack

# qhasm: stack64 t31_stack

# qhasm: stack64 t32_stack

# qhasm: stack64 t33_stack

# qhasm: stack64 t34_stack

# qhasm: int64 t40

# qhasm: int64 t41

# qhasm: int64 t42

# qhasm: int64 t43

# qhasm: int64 t44

# qhasm: stack64 t40_stack

# qhasm: stack64 t41_stack

# qhasm: stack64 t42_stack

# qhasm: stack64 t43_stack

# qhasm: stack64 t44_stack

# qhasm: int64 t50

# qhasm: int64 t51

# qhasm: int64 t52

# qhasm: int64 t53

# qhasm: int64 t54

# qhasm: stack64 t50_stack

# qhasm: stack64 t51_stack

# qhasm: stack64 t52_stack

# qhasm: stack64 t53_stack

# qhasm: stack64 t54_stack

# qhasm: int64 t60

# qhasm: int64 t61

# qhasm: int64 t62

# qhasm: int64 t63

# qhasm: int64 t64

# qhasm: stack64 t60_stack

# qhasm: stack64 t61_stack

# qhasm: stack64 t62_stack

# qhasm: stack64 t63_stack

# qhasm: stack64 t64_stack

# qhasm: int64 t70

# qhasm: int64 t71

# qhasm: int64 t72

# qhasm: int64 t73

# qhasm: int64 t74

# qhasm: stack64 t70_stack

# qhasm: stack64 t71_stack

# qhasm: stack64 t72_stack

# qhasm: stack64 t73_stack

# qhasm: stack64 t74_stack

# qhasm: int64 t80

# qhasm: int64 t81

# qhasm: int64 t82

# qhasm: int64 t83

# qhasm: int64 t84

# qhasm: stack64 t80_stack

# qhasm: stack64 t81_stack

# qhasm: stack64 t82_stack

# qhasm: stack64 t83_stack

# qhasm: stack64 t84_stack

# qhasm: int64 t90

# qhasm: int64 t91

# qhasm: int64 t92

# qhasm: int64 t93

# qhasm: int64 t94

# qhasm: stack64 t90_stack

# qhasm: stack64 t91_stack

# qhasm: stack64 t92_stack

# qhasm: stack64 t93_stack

# qhasm: stack64 t94_stack

# qhasm: int64 xp0

# qhasm: int64 xp1

# qhasm: int64 xp2

# qhasm: int64 xp3

# qhasm: int64 xp4

# qhasm: int64 zp0

# qhasm: int64 zp1

# qhasm: int64 zp2

# qhasm: int64 zp3

# qhasm: int64 zp4

# qhasm: int64 xq0

# qhasm: int64 xq1

# qhasm: int64 xq2

# qhasm: int64 xq3

# qhasm: int64 xq4

# qhasm: int64 zq0

# qhasm: int64 zq1

# qhasm: int64 zq2

# qhasm: int64 zq3

# qhasm: int64 zq4

# qhasm: int64 mulr01

# qhasm: int64 mulr11

# qhasm: int64 mulr21

# qhasm: int64 mulr31

# qhasm: int64 mulr41

# qhasm: int64 mulrax

# qhasm: int64 mulrdx

# qhasm: int64 mult

# qhasm: int64 mulredmask

# qhasm: stack64 mulx219_stack

# qhasm: stack64 mulx319_stack

# qhasm: stack64 mulx419_stack

# qhasm: int64 squarer01

# qhasm: int64 squarer11

# qhasm: int64 squarer21

# qhasm: int64 squarer31

# qhasm: int64 squarer41

# qhasm: int64 squarerax

# qhasm: int64 squarerdx

# qhasm: int64 squaret

# qhasm: int64 squareredmask

# qhasm: int64 mul121666rax

# qhasm: int64 mul121666rdx

# qhasm: enter crypto_scalarmult_curve25519_amd64_51_ladderstep
.text
.p2align 5
.globl _crypto_scalarmult_curve25519_amd64_51_ladderstep
.globl crypto_scalarmult_curve25519_amd64_51_ladderstep
_crypto_scalarmult_curve25519_amd64_51_ladderstep:
crypto_scalarmult_curve25519_amd64_51_ladderstep:
mov %rsp,%r11
and $31,%r11
add $352,%r11
sub %r11,%rsp

# qhasm:   caller1_stack = caller1
# asm 1: movq <caller1=int64#9,>caller1_stack=stack64#1
# asm 2: movq <caller1=%r11,>caller1_stack=0(%rsp)
movq %r11,0(%rsp)

# qhasm:   caller2_stack = caller2
# asm 1: movq <caller2=int64#10,>caller2_stack=stack64#2
# asm 2: movq <caller2=%r12,>caller2_stack=8(%rsp)
movq %r12,8(%rsp)

# qhasm:   caller3_stack = caller3
# asm 1: movq <caller3=int64#11,>caller3_stack=stack64#3
# asm 2: movq <caller3=%r13,>caller3_stack=16(%rsp)
movq %r13,16(%rsp)

# qhasm:   caller4_stack = caller4
# asm 1: movq <caller4=int64#12,>caller4_stack=stack64#4
# asm 2: movq <caller4=%r14,>caller4_stack=24(%rsp)
movq %r14,24(%rsp)

# qhasm:   caller5_stack = caller5
# asm 1: movq <caller5=int64#13,>caller5_stack=stack64#5
# asm 2: movq <caller5=%r15,>caller5_stack=32(%rsp)
movq %r15,32(%rsp)

# qhasm:   caller6_stack = caller6
# asm 1: movq <caller6=int64#14,>caller6_stack=stack64#6
# asm 2: movq <caller6=%rbx,>caller6_stack=40(%rsp)
movq %rbx,40(%rsp)

# qhasm:   caller7_stack = caller7
# asm 1: movq <caller7=int64#15,>caller7_stack=stack64#7
# asm 2: movq <caller7=%rbp,>caller7_stack=48(%rsp)
movq %rbp,48(%rsp)

# qhasm: t10 = *(uint64 *)(workp + 40)
# asm 1: movq   40(<workp=int64#1),>t10=int64#2
# asm 2: movq   40(<workp=%rdi),>t10=%rsi
movq   40(%rdi),%rsi

# qhasm: t11 = *(uint64 *)(workp + 48)
# asm 1: movq   48(<workp=int64#1),>t11=int64#3
# asm 2: movq   48(<workp=%rdi),>t11=%rdx
movq   48(%rdi),%rdx

# qhasm: t12 = *(uint64 *)(workp + 56)
# asm 1: movq   56(<workp=int64#1),>t12=int64#4
# asm 2: movq   56(<workp=%rdi),>t12=%rcx
movq   56(%rdi),%rcx

# qhasm: t13 = *(uint64 *)(workp + 64)
# asm 1: movq   64(<workp=int64#1),>t13=int64#5
# asm 2: movq   64(<workp=%rdi),>t13=%r8
movq   64(%rdi),%r8

# qhasm: t14 = *(uint64 *)(workp + 72)
# asm 1: movq   72(<workp=int64#1),>t14=int64#6
# asm 2: movq   72(<workp=%rdi),>t14=%r9
movq   72(%rdi),%r9

# qhasm: t20 = t10
# asm 1: mov  <t10=int64#2,>t20=int64#7
# asm 2: mov  <t10=%rsi,>t20=%rax
mov  %rsi,%rax

# qhasm: t21 = t11
# asm 1: mov  <t11=int64#3,>t21=int64#8
# asm 2: mov  <t11=%rdx,>t21=%r10
mov  %rdx,%r10

# qhasm: t22 = t12
# asm 1: mov  <t12=int64#4,>t22=int64#9
# asm 2: mov  <t12=%rcx,>t22=%r11
mov  %rcx,%r11

# qhasm: t23 = t13
# asm 1: mov  <t13=int64#5,>t23=int64#10
# asm 2: mov  <t13=%r8,>t23=%r12
mov  %r8,%r12

# qhasm: t24 = t14
# asm 1: mov  <t14=int64#6,>t24=int64#11
# asm 2: mov  <t14=%r9,>t24=%r13
mov  %r9,%r13

# qhasm: t20 += *(uint64 *) &crypto_scalarmult_curve25519_amd64_51_2P0
# asm 1: add  crypto_scalarmult_curve25519_amd64_51_2P0,<t20=int64#7
# asm 2: add  crypto_scalarmult_curve25519_amd64_51_2P0,<t20=%rax
add  crypto_scalarmult_curve25519_amd64_51_2P0,%rax

# qhasm: t21 += *(uint64 *) &crypto_scalarmult_curve25519_amd64_51_2P1234
# asm 1: add  crypto_scalarmult_curve25519_amd64_51_2P1234,<t21=int64#8
# asm 2: add  crypto_scalarmult_curve25519_amd64_51_2P1234,<t21=%r10
add  crypto_scalarmult_curve25519_amd64_51_2P1234,%r10

# qhasm: t22 += *(uint64 *) &crypto_scalarmult_curve25519_amd64_51_2P1234
# asm 1: add  crypto_scalarmult_curve25519_amd64_51_2P1234,<t22=int64#9
# asm 2: add  crypto_scalarmult_curve25519_amd64_51_2P1234,<t22=%r11
add  crypto_scalarmult_curve25519_amd64_51_2P1234,%r11

# qhasm: t23 += *(uint64 *) &crypto_scalarmult_curve25519_amd64_51_2P1234
# asm 1: add  crypto_scalarmult_curve25519_amd64_51_2P1234,<t23=int64#10
# asm 2: add  crypto_scalarmult_curve25519_amd64_51_2P1234,<t23=%r12
add  crypto_scalarmult_curve25519_amd64_51_2P1234,%r12

# qhasm: t24 += *(uint64 *) &crypto_scalarmult_curve25519_amd64_51_2P1234
# asm 1: add  crypto_scalarmult_curve25519_amd64_51_2P1234,<t24=int64#11
# asm 2: add  crypto_scalarmult_curve25519_amd64_51_2P1234,<t24=%r13
add  crypto_scalarmult_curve25519_amd64_51_2P1234,%r13

# qhasm: t10 += *(uint64 *)(workp + 80)
# asm 1: addq 80(<workp=int64#1),<t10=int64#2
# asm 2: addq 80(<workp=%rdi),<t10=%rsi
addq 80(%rdi),%rsi

# qhasm: t11 += *(uint64 *)(workp + 88)
# asm 1: addq 88(<workp=int64#1),<t11=int64#3
# asm 2: addq 88(<workp=%rdi),<t11=%rdx
addq 88(%rdi),%rdx

# qhasm: t12 += *(uint64 *)(workp + 96)
# asm 1: addq 96(<workp=int64#1),<t12=int64#4
# asm 2: addq 96(<workp=%rdi),<t12=%rcx
addq 96(%rdi),%rcx

# qhasm: t13 += *(uint64 *)(workp + 104)
# asm 1: addq 104(<workp=int64#1),<t13=int64#5
# asm 2: addq 104(<workp=%rdi),<t13=%r8
addq 104(%rdi),%r8

# qhasm: t14 += *(uint64 *)(workp + 112)
# asm 1: addq 112(<workp=int64#1),<t14=int64#6
# asm 2: addq 112(<workp=%rdi),<t14=%r9
addq 112(%rdi),%r9

# qhasm: t20 -= *(uint64 *)(workp + 80)
# asm 1: subq 80(<workp=int64#1),<t20=int64#7
# asm 2: subq 80(<workp=%rdi),<t20=%rax
subq 80(%rdi),%rax

# qhasm: t21 -= *(uint64 *)(workp + 88)
# asm 1: subq 88(<workp=int64#1),<t21=int64#8
# asm 2: subq 88(<workp=%rdi),<t21=%r10
subq 88(%rdi),%r10

# qhasm: t22 -= *(uint64 *)(workp + 96)
# asm 1: subq 96(<workp=int64#1),<t22=int64#9
# asm 2: subq 96(<workp=%rdi),<t22=%r11
subq 96(%rdi),%r11

# qhasm: t23 -= *(uint64 *)(workp + 104)
# asm 1: subq 104(<workp=int64#1),<t23=int64#10
# asm 2: subq 104(<workp=%rdi),<t23=%r12
subq 104(%rdi),%r12

# qhasm: t24 -= *(uint64 *)(workp + 112)
# asm 1: subq 112(<workp=int64#1),<t24=int64#11
# asm 2: subq 112(<workp=%rdi),<t24=%r13
subq 112(%rdi),%r13

# qhasm: t10_stack = t10
# asm 1: movq <t10=int64#2,>t10_stack=stack64#8
# asm 2: movq <t10=%rsi,>t10_stack=56(%rsp)
movq %rsi,56(%rsp)

# qhasm: t11_stack = t11
# asm 1: movq <t11=int64#3,>t11_stack=stack64#9
# asm 2: movq <t11=%rdx,>t11_stack=64(%rsp)
movq %rdx,64(%rsp)

# qhasm: t12_stack = t12
# asm 1: movq <t12=int64#4,>t12_stack=stack64#10
# asm 2: movq <t12=%rcx,>t12_stack=72(%rsp)
movq %rcx,72(%rsp)

# qhasm: t13_stack = t13
# asm 1: movq <t13=int64#5,>t13_stack=stack64#11
# asm 2: movq <t13=%r8,>t13_stack=80(%rsp)
movq %r8,80(%rsp)

# qhasm: t14_stack = t14
# asm 1: movq <t14=int64#6,>t14_stack=stack64#12
# asm 2: movq <t14=%r9,>t14_stack=88(%rsp)
movq %r9,88(%rsp)

# qhasm: t20_stack = t20
# asm 1: movq <t20=int64#7,>t20_stack=stack64#13
# asm 2: movq <t20=%rax,>t20_stack=96(%rsp)
movq %rax,96(%rsp)

# qhasm: t21_stack = t21
# asm 1: movq <t21=int64#8,>t21_stack=stack64#14
# asm 2: movq <t21=%r10,>t21_stack=104(%rsp)
movq %r10,104(%rsp)

# qhasm: t22_stack = t22
# asm 1: movq <t22=int64#9,>t22_stack=stack64#15
# asm 2: movq <t22=%r11,>t22_stack=112(%rsp)
movq %r11,112(%rsp)

# qhasm: t23_stack = t23
# asm 1: movq <t23=int64#10,>t23_stack=stack64#16
# asm 2: movq <t23=%r12,>t23_stack=120(%rsp)
movq %r12,120(%rsp)

# qhasm: t24_stack = t24
# asm 1: movq <t24=int64#11,>t24_stack=stack64#17
# asm 2: movq <t24=%r13,>t24_stack=128(%rsp)
movq %r13,128(%rsp)

# qhasm:   squarerax = t20_stack
# asm 1: movq <t20_stack=stack64#13,>squarerax=int64#7
# asm 2: movq <t20_stack=96(%rsp),>squarerax=%rax
movq 96(%rsp),%rax

# qhasm:   (uint128) squarerdx squarerax = squarerax * t20_stack
# asm 1: mulq  <t20_stack=stack64#13
# asm 2: mulq  <t20_stack=96(%rsp)
mulq  96(%rsp)

# qhasm:   t70 = squarerax
# asm 1: mov  <squarerax=int64#7,>t70=int64#2
# asm 2: mov  <squarerax=%rax,>t70=%rsi
mov  %rax,%rsi

# qhasm:   squarer01 = squarerdx
# asm 1: mov  <squarerdx=int64#3,>squarer01=int64#4
# asm 2: mov  <squarerdx=%rdx,>squarer01=%rcx
mov  %rdx,%rcx

# qhasm:   squarerax = t20_stack
# asm 1: movq <t20_stack=stack64#13,>squarerax=int64#7
# asm 2: movq <t20_stack=96(%rsp),>squarerax=%rax
movq 96(%rsp),%rax

# qhasm:   squarerax <<= 1
# asm 1: shl  $1,<squarerax=int64#7
# asm 2: shl  $1,<squarerax=%rax
shl  $1,%rax

# qhasm:   (uint128) squarerdx squarerax = squarerax * t21_stack
# asm 1: mulq  <t21_stack=stack64#14
# asm 2: mulq  <t21_stack=104(%rsp)
mulq  104(%rsp)

# qhasm:   t71 = squarerax
# asm 1: mov  <squarerax=int64#7,>t71=int64#5
# asm 2: mov  <squarerax=%rax,>t71=%r8
mov  %rax,%r8

# qhasm:   squarer11 = squarerdx
# asm 1: mov  <squarerdx=int64#3,>squarer11=int64#6
# asm 2: mov  <squarerdx=%rdx,>squarer11=%r9
mov  %rdx,%r9

# qhasm:   squarerax = t20_stack
# asm 1: movq <t20_stack=stack64#13,>squarerax=int64#7
# asm 2: movq <t20_stack=96(%rsp),>squarerax=%rax
movq 96(%rsp),%rax

# qhasm:   squarerax <<= 1
# asm 1: shl  $1,<squarerax=int64#7
# asm 2: shl  $1,<squarerax=%rax
shl  $1,%rax

# qhasm:   (uint128) squarerdx squarerax = squarerax * t22_stack
# asm 1: mulq  <t22_stack=stack64#15
# asm 2: mulq  <t22_stack=112(%rsp)
mulq  112(%rsp)

# qhasm:   t72 = squarerax
# asm 1: mov  <squarerax=int64#7,>t72=int64#8
# asm 2: mov  <squarerax=%rax,>t72=%r10
mov  %rax,%r10

# qhasm:   squarer21 = squarerdx
# asm 1: mov  <squarerdx=int64#3,>squarer21=int64#9
# asm 2: mov  <squarerdx=%rdx,>squarer21=%r11
mov  %rdx,%r11

# qhasm:   squarerax = t20_stack
# asm 1: movq <t20_stack=stack64#13,>squarerax=int64#7
# asm 2: movq <t20_stack=96(%rsp),>squarerax=%rax
movq 96(%rsp),%rax

# qhasm:   squarerax <<= 1
# asm 1: shl  $1,<squarerax=int64#7
# asm 2: shl  $1,<squarerax=%rax
shl  $1,%rax

# qhasm:   (uint128) squarerdx squarerax = squarerax * t23_stack
# asm 1: mulq  <t23_stack=stack64#16
# asm 2: mulq  <t23_stack=120(%rsp)
mulq  120(%rsp)

# qhasm:   t73 = squarerax
# asm 1: mov  <squarerax=int64#7,>t73=int64#10
# asm 2: mov  <squarerax=%rax,>t73=%r12
mov  %rax,%r12

# qhasm:   squarer31 = squarerdx
# asm 1: mov  <squarerdx=int64#3,>squarer31=int64#11
# asm 2: mov  <squarerdx=%rdx,>squarer31=%r13
mov  %rdx,%r13

# qhasm:   squarerax = t20_stack
# asm 1: movq <t20_stack=stack64#13,>squarerax=int64#7
# asm 2: movq <t20_stack=96(%rsp),>squarerax=%rax
movq 96(%rsp),%rax

# qhasm:   squarerax <<= 1
# asm 1: shl  $1,<squarerax=int64#7
# asm 2: shl  $1,<squarerax=%rax
shl  $1,%rax

# qhasm:   (uint128) squarerdx squarerax = squarerax * t24_stack
# asm 1: mulq  <t24_stack=stack64#17
# asm 2: mulq  <t24_stack=128(%rsp)
mulq  128(%rsp)

# qhasm:   t74 = squarerax
# asm 1: mov  <squarerax=int64#7,>t74=int64#12
# asm 2: mov  <squarerax=%rax,>t74=%r14
mov  %rax,%r14

# qhasm:   squarer41 = squarerdx
# asm 1: mov  <squarerdx=int64#3,>squarer41=int64#13
# asm 2: mov  <squarerdx=%rdx,>squarer41=%r15
mov  %rdx,%r15

# qhasm:   squarerax = t21_stack
# asm 1: movq <t21_stack=stack64#14,>squarerax=int64#7
# asm 2: movq <t21_stack=104(%rsp),>squarerax=%rax
movq 104(%rsp),%rax

# qhasm:   (uint128) squarerdx squarerax = squarerax * t21_stack
# asm 1: mulq  <t21_stack=stack64#14
# asm 2: mulq  <t21_stack=104(%rsp)
mulq  104(%rsp)

# qhasm:   carry? t72 += squarerax
# asm 1: add  <squarerax=int64#7,<t72=int64#8
# asm 2: add  <squarerax=%rax,<t72=%r10
add  %rax,%r10

# qhasm:   squarer21 += squarerdx + carry
# asm 1: adc <squarerdx=int64#3,<squarer21=int64#9
# asm 2: adc <squarerdx=%rdx,<squarer21=%r11
adc %rdx,%r11

# qhasm:   squarerax = t21_stack
# asm 1: movq <t21_stack=stack64#14,>squarerax=int64#7
# asm 2: movq <t21_stack=104(%rsp),>squarerax=%rax
movq 104(%rsp),%rax

# qhasm:   squarerax <<= 1
# asm 1: shl  $1,<squarerax=int64#7
# asm 2: shl  $1,<squarerax=%rax
shl  $1,%rax

# qhasm:   (uint128) squarerdx squarerax = squarerax * t22_stack
# asm 1: mulq  <t22_stack=stack64#15
# asm 2: mulq  <t22_stack=112(%rsp)
mulq  112(%rsp)

# qhasm:   carry? t73 += squarerax
# asm 1: add  <squarerax=int64#7,<t73=int64#10
# asm 2: add  <squarerax=%rax,<t73=%r12
add  %rax,%r12

# qhasm:   squarer31 += squarerdx + carry
# asm 1: adc <squarerdx=int64#3,<squarer31=int64#11
# asm 2: adc <squarerdx=%rdx,<squarer31=%r13
adc %rdx,%r13

# qhasm:   squarerax = t21_stack
# asm 1: movq <t21_stack=stack64#14,>squarerax=int64#7
# asm 2: movq <t21_stack=104(%rsp),>squarerax=%rax
movq 104(%rsp),%rax

# qhasm:   squarerax <<= 1
# asm 1: shl  $1,<squarerax=int64#7
# asm 2: shl  $1,<squarerax=%rax
shl  $1,%rax

# qhasm:   (uint128) squarerdx squarerax = squarerax * t23_stack
# asm 1: mulq  <t23_stack=stack64#16
# asm 2: mulq  <t23_stack=120(%rsp)
mulq  120(%rsp)

# qhasm:   carry? t74 += squarerax
# asm 1: add  <squarerax=int64#7,<t74=int64#12
# asm 2: add  <squarerax=%rax,<t74=%r14
add  %rax,%r14

# qhasm:   squarer41 += squarerdx + carry
# asm 1: adc <squarerdx=int64#3,<squarer41=int64#13
# asm 2: adc <squarerdx=%rdx,<squarer41=%r15
adc %rdx,%r15

# qhasm:   squarerax = t21_stack
# asm 1: movq <t21_stack=stack64#14,>squarerax=int64#3
# asm 2: movq <t21_stack=104(%rsp),>squarerax=%rdx
movq 104(%rsp),%rdx

# qhasm:   squarerax *= 38
# asm 1: imulq  $38,<squarerax=int64#3,>squarerax=int64#7
# asm 2: imulq  $38,<squarerax=%rdx,>squarerax=%rax
imulq  $38,%rdx,%rax

# qhasm:   (uint128) squarerdx squarerax = squarerax * t24_stack
# asm 1: mulq  <t24_stack=stack64#17
# asm 2: mulq  <t24_stack=128(%rsp)
mulq  128(%rsp)

# qhasm:   carry? t70 += squarerax
# asm 1: add  <squarerax=int64#7,<t70=int64#2
# asm 2: add  <squarerax=%rax,<t70=%rsi
add  %rax,%rsi

# qhasm:   squarer01 += squarerdx + carry
# asm 1: adc <squarerdx=int64#3,<squarer01=int64#4
# asm 2: adc <squarerdx=%rdx,<squarer01=%rcx
adc %rdx,%rcx

# qhasm:   squarerax = t22_stack
# asm 1: movq <t22_stack=stack64#15,>squarerax=int64#7
# asm 2: movq <t22_stack=112(%rsp),>squarerax=%rax
movq 112(%rsp),%rax

# qhasm:   (uint128) squarerdx squarerax = squarerax * t22_stack
# asm 1: mulq  <t22_stack=stack64#15
# asm 2: mulq  <t22_stack=112(%rsp)
mulq  112(%rsp)

# qhasm:   carry? t74 += squarerax
# asm 1: add  <squarerax=int64#7,<t74=int64#12
# asm 2: add  <squarerax=%rax,<t74=%r14
add  %rax,%r14

# qhasm:   squarer41 += squarerdx + carry
# asm 1: adc <squarerdx=int64#3,<squarer41=int64#13
# asm 2: adc <squarerdx=%rdx,<squarer41=%r15
adc %rdx,%r15

# qhasm:   squarerax = t22_stack
# asm 1: movq <t22_stack=stack64#15,>squarerax=int64#3
# asm 2: movq <t22_stack=112(%rsp),>squarerax=%rdx
movq 112(%rsp),%rdx

# qhasm:   squarerax *= 38
# asm 1: imulq  $38,<squarerax=int64#3,>squarerax=int64#7
# asm 2: imulq  $38,<squarerax=%rdx,>squarerax=%rax
imulq  $38,%rdx,%rax

# qhasm:   (uint128) squarerdx squarerax = squarerax * t23_stack
# asm 1: mulq  <t23_stack=stack64#16
# asm 2: mulq  <t23_stack=120(%rsp)
mulq  120(%rsp)

# qhasm:   carry? t70 += squarerax
# asm 1: add  <squarerax=int64#7,<t70=int64#2
# asm 2: add  <squarerax=%rax,<t70=%rsi
add  %rax,%rsi

# qhasm:   squarer01 += squarerdx + carry
# asm 1: adc <squarerdx=int64#3,<squarer01=int64#4
# asm 2: adc <squarerdx=%rdx,<squarer01=%rcx
adc %rdx,%rcx

# qhasm:   squarerax = t22_stack
# asm 1: movq <t22_stack=stack64#15,>squarerax=int64#3
# asm 2: movq <t22_stack=112(%rsp),>squarerax=%rdx
movq 112(%rsp),%rdx

# qhasm:   squarerax *= 38
# asm 1: imulq  $38,<squarerax=int64#3,>squarerax=int64#7
# asm 2: imulq  $38,<squarerax=%rdx,>squarerax=%rax
imulq  $38,%rdx,%rax

# qhasm:   (uint128) squarerdx squarerax = squarerax * t24_stack
# asm 1: mulq  <t24_stack=stack64#17
# asm 2: mulq  <t24_stack=128(%rsp)
mulq  128(%rsp)

# qhasm:   carry? t71 += squarerax
# asm 1: add  <squarerax=int64#7,<t71=int64#5
# asm 2: add  <squarerax=%rax,<t71=%r8
add  %rax,%r8

# qhasm:   squarer11 += squarerdx + carry
# asm 1: adc <squarerdx=int64#3,<squarer11=int64#6
# asm 2: adc <squarerdx=%rdx,<squarer11=%r9
adc %rdx,%r9

# qhasm:   squarerax = t23_stack
# asm 1: movq <t23_stack=stack64#16,>squarerax=int64#3
# asm 2: movq <t23_stack=120(%rsp),>squarerax=%rdx
movq 120(%rsp),%rdx

# qhasm:   squarerax *= 19
# asm 1: imulq  $19,<squarerax=int64#3,>squarerax=int64#7
# asm 2: imulq  $19,<squarerax=%rdx,>squarerax=%rax
imulq  $19,%rdx,%rax

# qhasm:   (uint128) squarerdx squarerax = squarerax * t23_stack
# asm 1: mulq  <t23_stack=stack64#16
# asm 2: mulq  <t23_stack=120(%rsp)
mulq  120(%rsp)

# qhasm:   carry? t71 += squarerax
# asm 1: add  <squarerax=int64#7,<t71=int64#5
# asm 2: add  <squarerax=%rax,<t71=%r8
add  %rax,%r8

# qhasm:   squarer11 += squarerdx + carry
# asm 1: adc <squarerdx=int64#3,<squarer11=int64#6
# asm 2: adc <squarerdx=%rdx,<squarer11=%r9
adc %rdx,%r9

# qhasm:   squarerax = t23_stack
# asm 1: movq <t23_stack=stack64#16,>squarerax=int64#3
# asm 2: movq <t23_stack=120(%rsp),>squarerax=%rdx
movq 120(%rsp),%rdx

# qhasm:   squarerax *= 38
# asm 1: imulq  $38,<squarerax=int64#3,>squarerax=int64#7
# asm 2: imulq  $38,<squarerax=%rdx,>squarerax=%rax
imulq  $38,%rdx,%rax

# qhasm:   (uint128) squarerdx squarerax = squarerax * t24_stack
# asm 1: mulq  <t24_stack=stack64#17
# asm 2: mulq  <t24_stack=128(%rsp)
mulq  128(%rsp)

# qhasm:   carry? t72 += squarerax
# asm 1: add  <squarerax=int64#7,<t72=int64#8
# asm 2: add  <squarerax=%rax,<t72=%r10
add  %rax,%r10

# qhasm:   squarer21 += squarerdx + carry
# asm 1: adc <squarerdx=int64#3,<squarer21=int64#9
# asm 2: adc <squarerdx=%rdx,<squarer21=%r11
adc %rdx,%r11

# qhasm:   squarerax = t24_stack
# asm 1: movq <t24_stack=stack64#17,>squarerax=int64#3
# asm 2: movq <t24_stack=128(%rsp),>squarerax=%rdx
movq 128(%rsp),%rdx

# qhasm:   squarerax *= 19
# asm 1: imulq  $19,<squarerax=int64#3,>squarerax=int64#7
# asm 2: imulq  $19,<squarerax=%rdx,>squarerax=%rax
imulq  $19,%rdx,%rax

# qhasm:   (uint128) squarerdx squarerax = squarerax * t24_stack
# asm 1: mulq  <t24_stack=stack64#17
# asm 2: mulq  <t24_stack=128(%rsp)
mulq  128(%rsp)

# qhasm:   carry? t73 += squarerax
# asm 1: add  <squarerax=int64#7,<t73=int64#10
# asm 2: add  <squarerax=%rax,<t73=%r12
add  %rax,%r12

# qhasm:   squarer31 += squarerdx + carry
# asm 1: adc <squarerdx=int64#3,<squarer31=int64#11
# asm 2: adc <squarerdx=%rdx,<squarer31=%r13
adc %rdx,%r13

# qhasm:   squareredmask = *(uint64 *) &crypto_scalarmult_curve25519_amd64_51_REDMASK51
# asm 1: movq crypto_scalarmult_curve25519_amd64_51_REDMASK51,>squareredmask=int64#3
# asm 2: movq crypto_scalarmult_curve25519_amd64_51_REDMASK51,>squareredmask=%rdx
movq crypto_scalarmult_curve25519_amd64_51_REDMASK51,%rdx

# qhasm:   squarer01 = (squarer01.t70) << 13
# asm 1: shld $13,<t70=int64#2,<squarer01=int64#4
# asm 2: shld $13,<t70=%rsi,<squarer01=%rcx
shld $13,%rsi,%rcx

# qhasm:   t70 &= squareredmask
# asm 1: and  <squareredmask=int64#3,<t70=int64#2
# asm 2: and  <squareredmask=%rdx,<t70=%rsi
and  %rdx,%rsi

# qhasm:   squarer11 = (squarer11.t71) << 13
# asm 1: shld $13,<t71=int64#5,<squarer11=int64#6
# asm 2: shld $13,<t71=%r8,<squarer11=%r9
shld $13,%r8,%r9

# qhasm:   t71 &= squareredmask
# asm 1: and  <squareredmask=int64#3,<t71=int64#5
# asm 2: and  <squareredmask=%rdx,<t71=%r8
and  %rdx,%r8

# qhasm:   t71 += squarer01
# asm 1: add  <squarer01=int64#4,<t71=int64#5
# asm 2: add  <squarer01=%rcx,<t71=%r8
add  %rcx,%r8

# qhasm:   squarer21 = (squarer21.t72) << 13
# asm 1: shld $13,<t72=int64#8,<squarer21=int64#9
# asm 2: shld $13,<t72=%r10,<squarer21=%r11
shld $13,%r10,%r11

# qhasm:   t72 &= squareredmask
# asm 1: and  <squareredmask=int64#3,<t72=int64#8
# asm 2: and  <squareredmask=%rdx,<t72=%r10
and  %rdx,%r10

# qhasm:   t72 += squarer11
# asm 1: add  <squarer11=int64#6,<t72=int64#8
# asm 2: add  <squarer11=%r9,<t72=%r10
add  %r9,%r10

# qhasm:   squarer31 = (squarer31.t73) << 13
# asm 1: shld $13,<t73=int64#10,<squarer31=int64#11
# asm 2: shld $13,<t73=%r12,<squarer31=%r13
shld $13,%r12,%r13

# qhasm:   t73 &= squareredmask
# asm 1: and  <squareredmask=int64#3,<t73=int64#10
# asm 2: and  <squareredmask=%rdx,<t73=%r12
and  %rdx,%r12

# qhasm:   t73 += squarer21
# asm 1: add  <squarer21=int64#9,<t73=int64#10
# asm 2: add  <squarer21=%r11,<t73=%r12
add  %r11,%r12

# qhasm:   squarer41 = (squarer41.t74) << 13
# asm 1: shld $13,<t74=int64#12,<squarer41=int64#13
# asm 2: shld $13,<t74=%r14,<squarer41=%r15
shld $13,%r14,%r15

# qhasm:   t74 &= squareredmask
# asm 1: and  <squareredmask=int64#3,<t74=int64#12
# asm 2: and  <squareredmask=%rdx,<t74=%r14
and  %rdx,%r14

# qhasm:   t74 += squarer31
# asm 1: add  <squarer31=int64#11,<t74=int64#12
# asm 2: add  <squarer31=%r13,<t74=%r14
add  %r13,%r14

# qhasm:   squarer41 = squarer41 * 19
# asm 1: imulq  $19,<squarer41=int64#13,>squarer41=int64#4
# asm 2: imulq  $19,<squarer41=%r15,>squarer41=%rcx
imulq  $19,%r15,%rcx

# qhasm:   t70 += squarer41
# asm 1: add  <squarer41=int64#4,<t70=int64#2
# asm 2: add  <squarer41=%rcx,<t70=%rsi
add  %rcx,%rsi

# qhasm:   squaret = t70
# asm 1: mov  <t70=int64#2,>squaret=int64#4
# asm 2: mov  <t70=%rsi,>squaret=%rcx
mov  %rsi,%rcx

# qhasm:   (uint64) squaret >>= 51
# asm 1: shr  $51,<squaret=int64#4
# asm 2: shr  $51,<squaret=%rcx
shr  $51,%rcx

# qhasm:   squaret += t71
# asm 1: add  <t71=int64#5,<squaret=int64#4
# asm 2: add  <t71=%r8,<squaret=%rcx
add  %r8,%rcx

# qhasm:   t70 &= squareredmask
# asm 1: and  <squareredmask=int64#3,<t70=int64#2
# asm 2: and  <squareredmask=%rdx,<t70=%rsi
and  %rdx,%rsi

# qhasm:   t71 = squaret
# asm 1: mov  <squaret=int64#4,>t71=int64#5
# asm 2: mov  <squaret=%rcx,>t71=%r8
mov  %rcx,%r8

# qhasm:   (uint64) squaret >>= 51
# asm 1: shr  $51,<squaret=int64#4
# asm 2: shr  $51,<squaret=%rcx
shr  $51,%rcx

# qhasm:   squaret += t72
# asm 1: add  <t72=int64#8,<squaret=int64#4
# asm 2: add  <t72=%r10,<squaret=%rcx
add  %r10,%rcx

# qhasm:   t71 &= squareredmask
# asm 1: and  <squareredmask=int64#3,<t71=int64#5
# asm 2: and  <squareredmask=%rdx,<t71=%r8
and  %rdx,%r8

# qhasm:   t72 = squaret
# asm 1: mov  <squaret=int64#4,>t72=int64#6
# asm 2: mov  <squaret=%rcx,>t72=%r9
mov  %rcx,%r9

# qhasm:   (uint64) squaret >>= 51
# asm 1: shr  $51,<squaret=int64#4
# asm 2: shr  $51,<squaret=%rcx
shr  $51,%rcx

# qhasm:   squaret += t73
# asm 1: add  <t73=int64#10,<squaret=int64#4
# asm 2: add  <t73=%r12,<squaret=%rcx
add  %r12,%rcx

# qhasm:   t72 &= squareredmask
# asm 1: and  <squareredmask=int64#3,<t72=int64#6
# asm 2: and  <squareredmask=%rdx,<t72=%r9
and  %rdx,%r9

# qhasm:   t73 = squaret
# asm 1: mov  <squaret=int64#4,>t73=int64#7
# asm 2: mov  <squaret=%rcx,>t73=%rax
mov  %rcx,%rax

# qhasm:   (uint64) squaret >>= 51
# asm 1: shr  $51,<squaret=int64#4
# asm 2: shr  $51,<squaret=%rcx
shr  $51,%rcx

# qhasm:   squaret += t74
# asm 1: add  <t74=int64#12,<squaret=int64#4
# asm 2: add  <t74=%r14,<squaret=%rcx
add  %r14,%rcx

# qhasm:   t73 &= squareredmask
# asm 1: and  <squareredmask=int64#3,<t73=int64#7
# asm 2: and  <squareredmask=%rdx,<t73=%rax
and  %rdx,%rax

# qhasm:   t74 = squaret
# asm 1: mov  <squaret=int64#4,>t74=int64#8
# asm 2: mov  <squaret=%rcx,>t74=%r10
mov  %rcx,%r10

# qhasm:   (uint64) squaret >>= 51
# asm 1: shr  $51,<squaret=int64#4
# asm 2: shr  $51,<squaret=%rcx
shr  $51,%rcx

# qhasm:   squaret *= 19
# asm 1: imulq  $19,<squaret=int64#4,>squaret=int64#4
# asm 2: imulq  $19,<squaret=%rcx,>squaret=%rcx
imulq  $19,%rcx,%rcx

# qhasm:   t70 += squaret
# asm 1: add  <squaret=int64#4,<t70=int64#2
# asm 2: add  <squaret=%rcx,<t70=%rsi
add  %rcx,%rsi

# qhasm:   t74 &= squareredmask
# asm 1: and  <squareredmask=int64#3,<t74=int64#8
# asm 2: and  <squareredmask=%rdx,<t74=%r10
and  %rdx,%r10

# qhasm: t70_stack = t70
# asm 1: movq <t70=int64#2,>t70_stack=stack64#18
# asm 2: movq <t70=%rsi,>t70_stack=136(%rsp)
movq %rsi,136(%rsp)

# qhasm: t71_stack = t71
# asm 1: movq <t71=int64#5,>t71_stack=stack64#19
# asm 2: movq <t71=%r8,>t71_stack=144(%rsp)
movq %r8,144(%rsp)

# qhasm: t72_stack = t72
# asm 1: movq <t72=int64#6,>t72_stack=stack64#20
# asm 2: movq <t72=%r9,>t72_stack=152(%rsp)
movq %r9,152(%rsp)

# qhasm: t73_stack = t73
# asm 1: movq <t73=int64#7,>t73_stack=stack64#21
# asm 2: movq <t73=%rax,>t73_stack=160(%rsp)
movq %rax,160(%rsp)

# qhasm: t74_stack = t74
# asm 1: movq <t74=int64#8,>t74_stack=stack64#22
# asm 2: movq <t74=%r10,>t74_stack=168(%rsp)
movq %r10,168(%rsp)

# qhasm:   squarerax = t10_stack
# asm 1: movq <t10_stack=stack64#8,>squarerax=int64#7
# asm 2: movq <t10_stack=56(%rsp),>squarerax=%rax
movq 56(%rsp),%rax

# qhasm:   (uint128) squarerdx squarerax = squarerax * t10_stack
# asm 1: mulq  <t10_stack=stack64#8
# asm 2: mulq  <t10_stack=56(%rsp)
mulq  56(%rsp)

# qhasm:   t60 = squarerax
# asm 1: mov  <squarerax=int64#7,>t60=int64#2
# asm 2: mov  <squarerax=%rax,>t60=%rsi
mov  %rax,%rsi

# qhasm:   squarer01 = squarerdx
# asm 1: mov  <squarerdx=int64#3,>squarer01=int64#4
# asm 2: mov  <squarerdx=%rdx,>squarer01=%rcx
mov  %rdx,%rcx

# qhasm:   squarerax = t10_stack
# asm 1: movq <t10_stack=stack64#8,>squarerax=int64#7
# asm 2: movq <t10_stack=56(%rsp),>squarerax=%rax
movq 56(%rsp),%rax

# qhasm:   squarerax <<= 1
# asm 1: shl  $1,<squarerax=int64#7
# asm 2: shl  $1,<squarerax=%rax
shl  $1,%rax

# qhasm:   (uint128) squarerdx squarerax = squarerax * t11_stack
# asm 1: mulq  <t11_stack=stack64#9
# asm 2: mulq  <t11_stack=64(%rsp)
mulq  64(%rsp)

# qhasm:   t61 = squarerax
# asm 1: mov  <squarerax=int64#7,>t61=int64#5
# asm 2: mov  <squarerax=%rax,>t61=%r8
mov  %rax,%r8

# qhasm:   squarer11 = squarerdx
# asm 1: mov  <squarerdx=int64#3,>squarer11=int64#6
# asm 2: mov  <squarerdx=%rdx,>squarer11=%r9
mov  %rdx,%r9

# qhasm:   squarerax = t10_stack
# asm 1: movq <t10_stack=stack64#8,>squarerax=int64#7
# asm 2: movq <t10_stack=56(%rsp),>squarerax=%rax
movq 56(%rsp),%rax

# qhasm:   squarerax <<= 1
# asm 1: shl  $1,<squarerax=int64#7
# asm 2: shl  $1,<squarerax=%rax
shl  $1,%rax

# qhasm:   (uint128) squarerdx squarerax = squarerax * t12_stack
# asm 1: mulq  <t12_stack=stack64#10
# asm 2: mulq  <t12_stack=72(%rsp)
mulq  72(%rsp)

# qhasm:   t62 = squarerax
# asm 1: mov  <squarerax=int64#7,>t62=int64#8
# asm 2: mov  <squarerax=%rax,>t62=%r10
mov  %rax,%r10

# qhasm:   squarer21 = squarerdx
# asm 1: mov  <squarerdx=int64#3,>squarer21=int64#9
# asm 2: mov  <squarerdx=%rdx,>squarer21=%r11
mov  %rdx,%r11

# qhasm:   squarerax = t10_stack
# asm 1: movq <t10_stack=stack64#8,>squarerax=int64#7
# asm 2: movq <t10_stack=56(%rsp),>squarerax=%rax
movq 56(%rsp),%rax

# qhasm:   squarerax <<= 1
# asm 1: shl  $1,<squarerax=int64#7
# asm 2: shl  $1,<squarerax=%rax
shl  $1,%rax

# qhasm:   (uint128) squarerdx squarerax = squarerax * t13_stack
# asm 1: mulq  <t13_stack=stack64#11
# asm 2: mulq  <t13_stack=80(%rsp)
mulq  80(%rsp)

# qhasm:   t63 = squarerax
# asm 1: mov  <squarerax=int64#7,>t63=int64#10
# asm 2: mov  <squarerax=%rax,>t63=%r12
mov  %rax,%r12

# qhasm:   squarer31 = squarerdx
# asm 1: mov  <squarerdx=int64#3,>squarer31=int64#11
# asm 2: mov  <squarerdx=%rdx,>squarer31=%r13
mov  %rdx,%r13

# qhasm:   squarerax = t10_stack
# asm 1: movq <t10_stack=stack64#8,>squarerax=int64#7
# asm 2: movq <t10_stack=56(%rsp),>squarerax=%rax
movq 56(%rsp),%rax

# qhasm:   squarerax <<= 1
# asm 1: shl  $1,<squarerax=int64#7
# asm 2: shl  $1,<squarerax=%rax
shl  $1,%rax

# qhasm:   (uint128) squarerdx squarerax = squarerax * t14_stack
# asm 1: mulq  <t14_stack=stack64#12
# asm 2: mulq  <t14_stack=88(%rsp)
mulq  88(%rsp)

# qhasm:   t64 = squarerax
# asm 1: mov  <squarerax=int64#7,>t64=int64#12
# asm 2: mov  <squarerax=%rax,>t64=%r14
mov  %rax,%r14

# qhasm:   squarer41 = squarerdx
# asm 1: mov  <squarerdx=int64#3,>squarer41=int64#13
# asm 2: mov  <squarerdx=%rdx,>squarer41=%r15
mov  %rdx,%r15

# qhasm:   squarerax = t11_stack
# asm 1: movq <t11_stack=stack64#9,>squarerax=int64#7
# asm 2: movq <t11_stack=64(%rsp),>squarerax=%rax
movq 64(%rsp),%rax

# qhasm:   (uint128) squarerdx squarerax = squarerax * t11_stack
# asm 1: mulq  <t11_stack=stack64#9
# asm 2: mulq  <t11_stack=64(%rsp)
mulq  64(%rsp)

# qhasm:   carry? t62 += squarerax
# asm 1: add  <squarerax=int64#7,<t62=int64#8
# asm 2: add  <squarerax=%rax,<t62=%r10
add  %rax,%r10

# qhasm:   squarer21 += squarerdx + carry
# asm 1: adc <squarerdx=int64#3,<squarer21=int64#9
# asm 2: adc <squarerdx=%rdx,<squarer21=%r11
adc %rdx,%r11

# qhasm:   squarerax = t11_stack
# asm 1: movq <t11_stack=stack64#9,>squarerax=int64#7
# asm 2: movq <t11_stack=64(%rsp),>squarerax=%rax
movq 64(%rsp),%rax

# qhasm:   squarerax <<= 1
# asm 1: shl  $1,<squarerax=int64#7
# asm 2: shl  $1,<squarerax=%rax
shl  $1,%rax

# qhasm:   (uint128) squarerdx squarerax = squarerax * t12_stack
# asm 1: mulq  <t12_stack=stack64#10
# asm 2: mulq  <t12_stack=72(%rsp)
mulq  72(%rsp)

# qhasm:   carry? t63 += squarerax
# asm 1: add  <squarerax=int64#7,<t63=int64#10
# asm 2: add  <squarerax=%rax,<t63=%r12
add  %rax,%r12

# qhasm:   squarer31 += squarerdx + carry
# asm 1: adc <squarerdx=int64#3,<squarer31=int64#11
# asm 2: adc <squarerdx=%rdx,<squarer31=%r13
adc %rdx,%r13

# qhasm:   squarerax = t11_stack
# asm 1: movq <t11_stack=stack64#9,>squarerax=int64#7
# asm 2: movq <t11_stack=64(%rsp),>squarerax=%rax
movq 64(%rsp),%rax

# qhasm:   squarerax <<= 1
# asm 1: shl  $1,<squarerax=int64#7
# asm 2: shl  $1,<squarerax=%rax
shl  $1,%rax

# qhasm:   (uint128) squarerdx squarerax = squarerax * t13_stack
# asm 1: mulq  <t13_stack=stack64#11
# asm 2: mulq  <t13_stack=80(%rsp)
mulq  80(%rsp)

# qhasm:   carry? t64 += squarerax
# asm 1: add  <squarerax=int64#7,<t64=int64#12
# asm 2: add  <squarerax=%rax,<t64=%r14
add  %rax,%r14

# qhasm:   squarer41 += squarerdx + carry
# asm 1: adc <squarerdx=int64#3,<squarer41=int64#13
# asm 2: adc <squarerdx=%rdx,<squarer41=%r15
adc %rdx,%r15

# qhasm:   squarerax = t11_stack
# asm 1: movq <t11_stack=stack64#9,>squarerax=int64#3
# asm 2: movq <t11_stack=64(%rsp),>squarerax=%rdx
movq 64(%rsp),%rdx

# qhasm:   squarerax *= 38
# asm 1: imulq  $38,<squarerax=int64#3,>squarerax=int64#7
# asm 2: imulq  $38,<squarerax=%rdx,>squarerax=%rax
imulq  $38,%rdx,%rax

# qhasm:   (uint128) squarerdx squarerax = squarerax * t14_stack
# asm 1: mulq  <t14_stack=stack64#12
# asm 2: mulq  <t14_stack=88(%rsp)
mulq  88(%rsp)

# qhasm:   carry? t60 += squarerax
# asm 1: add  <squarerax=int64#7,<t60=int64#2
# asm 2: add  <squarerax=%rax,<t60=%rsi
add  %rax,%rsi

# qhasm:   squarer01 += squarerdx + carry
# asm 1: adc <squarerdx=int64#3,<squarer01=int64#4
# asm 2: adc <squarerdx=%rdx,<squarer01=%rcx
adc %rdx,%rcx

# qhasm:   squarerax = t12_stack
# asm 1: movq <t12_stack=stack64#10,>squarerax=int64#7
# asm 2: movq <t12_stack=72(%rsp),>squarerax=%rax
movq 72(%rsp),%rax

# qhasm:   (uint128) squarerdx squarerax = squarerax * t12_stack
# asm 1: mulq  <t12_stack=stack64#10
# asm 2: mulq  <t12_stack=72(%rsp)
mulq  72(%rsp)

# qhasm:   carry? t64 += squarerax
# asm 1: add  <squarerax=int64#7,<t64=int64#12
# asm 2: add  <squarerax=%rax,<t64=%r14
add  %rax,%r14

# qhasm:   squarer41 += squarerdx + carry
# asm 1: adc <squarerdx=int64#3,<squarer41=int64#13
# asm 2: adc <squarerdx=%rdx,<squarer41=%r15
adc %rdx,%r15

# qhasm:   squarerax = t12_stack
# asm 1: movq <t12_stack=stack64#10,>squarerax=int64#3
# asm 2: movq <t12_stack=72(%rsp),>squarerax=%rdx
movq 72(%rsp),%rdx

# qhasm:   squarerax *= 38
# asm 1: imulq  $38,<squarerax=int64#3,>squarerax=int64#7
# asm 2: imulq  $38,<squarerax=%rdx,>squarerax=%rax
imulq  $38,%rdx,%rax

# qhasm:   (uint128) squarerdx squarerax = squarerax * t13_stack
# asm 1: mulq  <t13_stack=stack64#11
# asm 2: mulq  <t13_stack=80(%rsp)
mulq  80(%rsp)

# qhasm:   carry? t60 += squarerax
# asm 1: add  <squarerax=int64#7,<t60=int64#2
# asm 2: add  <squarerax=%rax,<t60=%rsi
add  %rax,%rsi

# qhasm:   squarer01 += squarerdx + carry
# asm 1: adc <squarerdx=int64#3,<squarer01=int64#4
# asm 2: adc <squarerdx=%rdx,<squarer01=%rcx
adc %rdx,%rcx

# qhasm:   squarerax = t12_stack
# asm 1: movq <t12_stack=stack64#10,>squarerax=int64#3
# asm 2: movq <t12_stack=72(%rsp),>squarerax=%rdx
movq 72(%rsp),%rdx

# qhasm:   squarerax *= 38
# asm 1: imulq  $38,<squarerax=int64#3,>squarerax=int64#7
# asm 2: imulq  $38,<squarerax=%rdx,>squarerax=%rax
imulq  $38,%rdx,%rax

# qhasm:   (uint128) squarerdx squarerax = squarerax * t14_stack
# asm 1: mulq  <t14_stack=stack64#12
# asm 2: mulq  <t14_stack=88(%rsp)
mulq  88(%rsp)

# qhasm:   carry? t61 += squarerax
# asm 1: add  <squarerax=int64#7,<t61=int64#5
# asm 2: add  <squarerax=%rax,<t61=%r8
add  %rax,%r8

# qhasm:   squarer11 += squarerdx + carry
# asm 1: adc <squarerdx=int64#3,<squarer11=int64#6
# asm 2: adc <squarerdx=%rdx,<squarer11=%r9
adc %rdx,%r9

# qhasm:   squarerax = t13_stack
# asm 1: movq <t13_stack=stack64#11,>squarerax=int64#3
# asm 2: movq <t13_stack=80(%rsp),>squarerax=%rdx
movq 80(%rsp),%rdx

# qhasm:   squarerax *= 19
# asm 1: imulq  $19,<squarerax=int64#3,>squarerax=int64#7
# asm 2: imulq  $19,<squarerax=%rdx,>squarerax=%rax
imulq  $19,%rdx,%rax

# qhasm:   (uint128) squarerdx squarerax = squarerax * t13_stack
# asm 1: mulq  <t13_stack=stack64#11
# asm 2: mulq  <t13_stack=80(%rsp)
mulq  80(%rsp)

# qhasm:   carry? t61 += squarerax
# asm 1: add  <squarerax=int64#7,<t61=int64#5
# asm 2: add  <squarerax=%rax,<t61=%r8
add  %rax,%r8

# qhasm:   squarer11 += squarerdx + carry
# asm 1: adc <squarerdx=int64#3,<squarer11=int64#6
# asm 2: adc <squarerdx=%rdx,<squarer11=%r9
adc %rdx,%r9

# qhasm:   squarerax = t13_stack
# asm 1: movq <t13_stack=stack64#11,>squarerax=int64#3
# asm 2: movq <t13_stack=80(%rsp),>squarerax=%rdx
movq 80(%rsp),%rdx

# qhasm:   squarerax *= 38
# asm 1: imulq  $38,<squarerax=int64#3,>squarerax=int64#7
# asm 2: imulq  $38,<squarerax=%rdx,>squarerax=%rax
imulq  $38,%rdx,%rax

# qhasm:   (uint128) squarerdx squarerax = squarerax * t14_stack
# asm 1: mulq  <t14_stack=stack64#12
# asm 2: mulq  <t14_stack=88(%rsp)
mulq  88(%rsp)

# qhasm:   carry? t62 += squarerax
# asm 1: add  <squarerax=int64#7,<t62=int64#8
# asm 2: add  <squarerax=%rax,<t62=%r10
add  %rax,%r10

# qhasm:   squarer21 += squarerdx + carry
# asm 1: adc <squarerdx=int64#3,<squarer21=int64#9
# asm 2: adc <squarerdx=%rdx,<squarer21=%r11
adc %rdx,%r11

# qhasm:   squarerax = t14_stack
# asm 1: movq <t14_stack=stack64#12,>squarerax=int64#3
# asm 2: movq <t14_stack=88(%rsp),>squarerax=%rdx
movq 88(%rsp),%rdx

# qhasm:   squarerax *= 19
# asm 1: imulq  $19,<squarerax=int64#3,>squarerax=int64#7
# asm 2: imulq  $19,<squarerax=%rdx,>squarerax=%rax
imulq  $19,%rdx,%rax

# qhasm:   (uint128) squarerdx squarerax = squarerax * t14_stack
# asm 1: mulq  <t14_stack=stack64#12
# asm 2: mulq  <t14_stack=88(%rsp)
mulq  88(%rsp)

# qhasm:   carry? t63 += squarerax
# asm 1: add  <squarerax=int64#7,<t63=int64#10
# asm 2: add  <squarerax=%rax,<t63=%r12
add  %rax,%r12

# qhasm:   squarer31 += squarerdx + carry
# asm 1: adc <squarerdx=int64#3,<squarer31=int64#11
# asm 2: adc <squarerdx=%rdx,<squarer31=%r13
adc %rdx,%r13

# qhasm:   squareredmask = *(uint64 *) &crypto_scalarmult_curve25519_amd64_51_REDMASK51
# asm 1: movq crypto_scalarmult_curve25519_amd64_51_REDMASK51,>squareredmask=int64#3
# asm 2: movq crypto_scalarmult_curve25519_amd64_51_REDMASK51,>squareredmask=%rdx
movq crypto_scalarmult_curve25519_amd64_51_REDMASK51,%rdx

# qhasm:   squarer01 = (squarer01.t60) << 13
# asm 1: shld $13,<t60=int64#2,<squarer01=int64#4
# asm 2: shld $13,<t60=%rsi,<squarer01=%rcx
shld $13,%rsi,%rcx

# qhasm:   t60 &= squareredmask
# asm 1: and  <squareredmask=int64#3,<t60=int64#2
# asm 2: and  <squareredmask=%rdx,<t60=%rsi
and  %rdx,%rsi

# qhasm:   squarer11 = (squarer11.t61) << 13
# asm 1: shld $13,<t61=int64#5,<squarer11=int64#6
# asm 2: shld $13,<t61=%r8,<squarer11=%r9
shld $13,%r8,%r9

# qhasm:   t61 &= squareredmask
# asm 1: and  <squareredmask=int64#3,<t61=int64#5
# asm 2: and  <squareredmask=%rdx,<t61=%r8
and  %rdx,%r8

# qhasm:   t61 += squarer01
# asm 1: add  <squarer01=int64#4,<t61=int64#5
# asm 2: add  <squarer01=%rcx,<t61=%r8
add  %rcx,%r8

# qhasm:   squarer21 = (squarer21.t62) << 13
# asm 1: shld $13,<t62=int64#8,<squarer21=int64#9
# asm 2: shld $13,<t62=%r10,<squarer21=%r11
shld $13,%r10,%r11

# qhasm:   t62 &= squareredmask
# asm 1: and  <squareredmask=int64#3,<t62=int64#8
# asm 2: and  <squareredmask=%rdx,<t62=%r10
and  %rdx,%r10

# qhasm:   t62 += squarer11
# asm 1: add  <squarer11=int64#6,<t62=int64#8
# asm 2: add  <squarer11=%r9,<t62=%r10
add  %r9,%r10

# qhasm:   squarer31 = (squarer31.t63) << 13
# asm 1: shld $13,<t63=int64#10,<squarer31=int64#11
# asm 2: shld $13,<t63=%r12,<squarer31=%r13
shld $13,%r12,%r13

# qhasm:   t63 &= squareredmask
# asm 1: and  <squareredmask=int64#3,<t63=int64#10
# asm 2: and  <squareredmask=%rdx,<t63=%r12
and  %rdx,%r12

# qhasm:   t63 += squarer21
# asm 1: add  <squarer21=int64#9,<t63=int64#10
# asm 2: add  <squarer21=%r11,<t63=%r12
add  %r11,%r12

# qhasm:   squarer41 = (squarer41.t64) << 13
# asm 1: shld $13,<t64=int64#12,<squarer41=int64#13
# asm 2: shld $13,<t64=%r14,<squarer41=%r15
shld $13,%r14,%r15

# qhasm:   t64 &= squareredmask
# asm 1: and  <squareredmask=int64#3,<t64=int64#12
# asm 2: and  <squareredmask=%rdx,<t64=%r14
and  %rdx,%r14

# qhasm:   t64 += squarer31
# asm 1: add  <squarer31=int64#11,<t64=int64#12
# asm 2: add  <squarer31=%r13,<t64=%r14
add  %r13,%r14

# qhasm:   squarer41 = squarer41 * 19
# asm 1: imulq  $19,<squarer41=int64#13,>squarer41=int64#4
# asm 2: imulq  $19,<squarer41=%r15,>squarer41=%rcx
imulq  $19,%r15,%rcx

# qhasm:   t60 += squarer41
# asm 1: add  <squarer41=int64#4,<t60=int64#2
# asm 2: add  <squarer41=%rcx,<t60=%rsi
add  %rcx,%rsi

# qhasm:   squaret = t60
# asm 1: mov  <t60=int64#2,>squaret=int64#4
# asm 2: mov  <t60=%rsi,>squaret=%rcx
mov  %rsi,%rcx

# qhasm:   (uint64) squaret >>= 51
# asm 1: shr  $51,<squaret=int64#4
# asm 2: shr  $51,<squaret=%rcx
shr  $51,%rcx

# qhasm:   squaret += t61
# asm 1: add  <t61=int64#5,<squaret=int64#4
# asm 2: add  <t61=%r8,<squaret=%rcx
add  %r8,%rcx

# qhasm:   t60 &= squareredmask
# asm 1: and  <squareredmask=int64#3,<t60=int64#2
# asm 2: and  <squareredmask=%rdx,<t60=%rsi
and  %rdx,%rsi

# qhasm:   t61 = squaret
# asm 1: mov  <squaret=int64#4,>t61=int64#5
# asm 2: mov  <squaret=%rcx,>t61=%r8
mov  %rcx,%r8

# qhasm:   (uint64) squaret >>= 51
# asm 1: shr  $51,<squaret=int64#4
# asm 2: shr  $51,<squaret=%rcx
shr  $51,%rcx

# qhasm:   squaret += t62
# asm 1: add  <t62=int64#8,<squaret=int64#4
# asm 2: add  <t62=%r10,<squaret=%rcx
add  %r10,%rcx

# qhasm:   t61 &= squareredmask
# asm 1: and  <squareredmask=int64#3,<t61=int64#5
# asm 2: and  <squareredmask=%rdx,<t61=%r8
and  %rdx,%r8

# qhasm:   t62 = squaret
# asm 1: mov  <squaret=int64#4,>t62=int64#6
# asm 2: mov  <squaret=%rcx,>t62=%r9
mov  %rcx,%r9

# qhasm:   (uint64) squaret >>= 51
# asm 1: shr  $51,<squaret=int64#4
# asm 2: shr  $51,<squaret=%rcx
shr  $51,%rcx

# qhasm:   squaret += t63
# asm 1: add  <t63=int64#10,<squaret=int64#4
# asm 2: add  <t63=%r12,<squaret=%rcx
add  %r12,%rcx

# qhasm:   t62 &= squareredmask
# asm 1: and  <squareredmask=int64#3,<t62=int64#6
# asm 2: and  <squareredmask=%rdx,<t62=%r9
and  %rdx,%r9

# qhasm:   t63 = squaret
# asm 1: mov  <squaret=int64#4,>t63=int64#7
# asm 2: mov  <squaret=%rcx,>t63=%rax
mov  %rcx,%rax

# qhasm:   (uint64) squaret >>= 51
# asm 1: shr  $51,<squaret=int64#4
# asm 2: shr  $51,<squaret=%rcx
shr  $51,%rcx

# qhasm:   squaret += t64
# asm 1: add  <t64=int64#12,<squaret=int64#4
# asm 2: add  <t64=%r14,<squaret=%rcx
add  %r14,%rcx

# qhasm:   t63 &= squareredmask
# asm 1: and  <squareredmask=int64#3,<t63=int64#7
# asm 2: and  <squareredmask=%rdx,<t63=%rax
and  %rdx,%rax

# qhasm:   t64 = squaret
# asm 1: mov  <squaret=int64#4,>t64=int64#8
# asm 2: mov  <squaret=%rcx,>t64=%r10
mov  %rcx,%r10

# qhasm:   (uint64) squaret >>= 51
# asm 1: shr  $51,<squaret=int64#4
# asm 2: shr  $51,<squaret=%rcx
shr  $51,%rcx

# qhasm:   squaret *= 19
# asm 1: imulq  $19,<squaret=int64#4,>squaret=int64#4
# asm 2: imulq  $19,<squaret=%rcx,>squaret=%rcx
imulq  $19,%rcx,%rcx

# qhasm:   t60 += squaret
# asm 1: add  <squaret=int64#4,<t60=int64#2
# asm 2: add  <squaret=%rcx,<t60=%rsi
add  %rcx,%rsi

# qhasm:   t64 &= squareredmask
# asm 1: and  <squareredmask=int64#3,<t64=int64#8
# asm 2: and  <squareredmask=%rdx,<t64=%r10
and  %rdx,%r10

# qhasm: t60_stack = t60
# asm 1: movq <t60=int64#2,>t60_stack=stack64#23
# asm 2: movq <t60=%rsi,>t60_stack=176(%rsp)
movq %rsi,176(%rsp)

# qhasm: t61_stack = t61
# asm 1: movq <t61=int64#5,>t61_stack=stack64#24
# asm 2: movq <t61=%r8,>t61_stack=184(%rsp)
movq %r8,184(%rsp)

# qhasm: t62_stack = t62
# asm 1: movq <t62=int64#6,>t62_stack=stack64#25
# asm 2: movq <t62=%r9,>t62_stack=192(%rsp)
movq %r9,192(%rsp)

# qhasm: t63_stack = t63
# asm 1: movq <t63=int64#7,>t63_stack=stack64#26
# asm 2: movq <t63=%rax,>t63_stack=200(%rsp)
movq %rax,200(%rsp)

# qhasm: t64_stack = t64
# asm 1: movq <t64=int64#8,>t64_stack=stack64#27
# asm 2: movq <t64=%r10,>t64_stack=208(%rsp)
movq %r10,208(%rsp)

# qhasm: t50 = t60
# asm 1: mov  <t60=int64#2,>t50=int64#2
# asm 2: mov  <t60=%rsi,>t50=%rsi
mov  %rsi,%rsi

# qhasm: t51 = t61
# asm 1: mov  <t61=int64#5,>t51=int64#3
# asm 2: mov  <t61=%r8,>t51=%rdx
mov  %r8,%rdx

# qhasm: t52 = t62
# asm 1: mov  <t62=int64#6,>t52=int64#4
# asm 2: mov  <t62=%r9,>t52=%rcx
mov  %r9,%rcx

# qhasm: t53 = t63
# asm 1: mov  <t63=int64#7,>t53=int64#5
# asm 2: mov  <t63=%rax,>t53=%r8
mov  %rax,%r8

# qhasm: t54 = t64
# asm 1: mov  <t64=int64#8,>t54=int64#6
# asm 2: mov  <t64=%r10,>t54=%r9
mov  %r10,%r9

# qhasm: t50 += *(uint64 *) &crypto_scalarmult_curve25519_amd64_51_2P0
# asm 1: add  crypto_scalarmult_curve25519_amd64_51_2P0,<t50=int64#2
# asm 2: add  crypto_scalarmult_curve25519_amd64_51_2P0,<t50=%rsi
add  crypto_scalarmult_curve25519_amd64_51_2P0,%rsi

# qhasm: t51 += *(uint64 *) &crypto_scalarmult_curve25519_amd64_51_2P1234
# asm 1: add  crypto_scalarmult_curve25519_amd64_51_2P1234,<t51=int64#3
# asm 2: add  crypto_scalarmult_curve25519_amd64_51_2P1234,<t51=%rdx
add  crypto_scalarmult_curve25519_amd64_51_2P1234,%rdx

# qhasm: t52 += *(uint64 *) &crypto_scalarmult_curve25519_amd64_51_2P1234
# asm 1: add  crypto_scalarmult_curve25519_amd64_51_2P1234,<t52=int64#4
# asm 2: add  crypto_scalarmult_curve25519_amd64_51_2P1234,<t52=%rcx
add  crypto_scalarmult_curve25519_amd64_51_2P1234,%rcx

# qhasm: t53 += *(uint64 *) &crypto_scalarmult_curve25519_amd64_51_2P1234
# asm 1: add  crypto_scalarmult_curve25519_amd64_51_2P1234,<t53=int64#5
# asm 2: add  crypto_scalarmult_curve25519_amd64_51_2P1234,<t53=%r8
add  crypto_scalarmult_curve25519_amd64_51_2P1234,%r8

# qhasm: t54 += *(uint64 *) &crypto_scalarmult_curve25519_amd64_51_2P1234
# asm 1: add  crypto_scalarmult_curve25519_amd64_51_2P1234,<t54=int64#6
# asm 2: add  crypto_scalarmult_curve25519_amd64_51_2P1234,<t54=%r9
add  crypto_scalarmult_curve25519_amd64_51_2P1234,%r9

# qhasm: t50 -= t70_stack
# asm 1: subq <t70_stack=stack64#18,<t50=int64#2
# asm 2: subq <t70_stack=136(%rsp),<t50=%rsi
subq 136(%rsp),%rsi

# qhasm: t51 -= t71_stack
# asm 1: subq <t71_stack=stack64#19,<t51=int64#3
# asm 2: subq <t71_stack=144(%rsp),<t51=%rdx
subq 144(%rsp),%rdx

# qhasm: t52 -= t72_stack
# asm 1: subq <t72_stack=stack64#20,<t52=int64#4
# asm 2: subq <t72_stack=152(%rsp),<t52=%rcx
subq 152(%rsp),%rcx

# qhasm: t53 -= t73_stack
# asm 1: subq <t73_stack=stack64#21,<t53=int64#5
# asm 2: subq <t73_stack=160(%rsp),<t53=%r8
subq 160(%rsp),%r8

# qhasm: t54 -= t74_stack
# asm 1: subq <t74_stack=stack64#22,<t54=int64#6
# asm 2: subq <t74_stack=168(%rsp),<t54=%r9
subq 168(%rsp),%r9

# qhasm: t50_stack = t50
# asm 1: movq <t50=int64#2,>t50_stack=stack64#28
# asm 2: movq <t50=%rsi,>t50_stack=216(%rsp)
movq %rsi,216(%rsp)

# qhasm: t51_stack = t51
# asm 1: movq <t51=int64#3,>t51_stack=stack64#29
# asm 2: movq <t51=%rdx,>t51_stack=224(%rsp)
movq %rdx,224(%rsp)

# qhasm: t52_stack = t52
# asm 1: movq <t52=int64#4,>t52_stack=stack64#30
# asm 2: movq <t52=%rcx,>t52_stack=232(%rsp)
movq %rcx,232(%rsp)

# qhasm: t53_stack = t53
# asm 1: movq <t53=int64#5,>t53_stack=stack64#31
# asm 2: movq <t53=%r8,>t53_stack=240(%rsp)
movq %r8,240(%rsp)

# qhasm: t54_stack = t54
# asm 1: movq <t54=int64#6,>t54_stack=stack64#32
# asm 2: movq <t54=%r9,>t54_stack=248(%rsp)
movq %r9,248(%rsp)

# qhasm: t30 = *(uint64 *)(workp + 120)
# asm 1: movq   120(<workp=int64#1),>t30=int64#2
# asm 2: movq   120(<workp=%rdi),>t30=%rsi
movq   120(%rdi),%rsi

# qhasm: t31 = *(uint64 *)(workp + 128)
# asm 1: movq   128(<workp=int64#1),>t31=int64#3
# asm 2: movq   128(<workp=%rdi),>t31=%rdx
movq   128(%rdi),%rdx

# qhasm: t32 = *(uint64 *)(workp + 136)
# asm 1: movq   136(<workp=int64#1),>t32=int64#4
# asm 2: movq   136(<workp=%rdi),>t32=%rcx
movq   136(%rdi),%rcx

# qhasm: t33 = *(uint64 *)(workp + 144)
# asm 1: movq   144(<workp=int64#1),>t33=int64#5
# asm 2: movq   144(<workp=%rdi),>t33=%r8
movq   144(%rdi),%r8

# qhasm: t34 = *(uint64 *)(workp + 152)
# asm 1: movq   152(<workp=int64#1),>t34=int64#6
# asm 2: movq   152(<workp=%rdi),>t34=%r9
movq   152(%rdi),%r9

# qhasm: t40 = t30
# asm 1: mov  <t30=int64#2,>t40=int64#7
# asm 2: mov  <t30=%rsi,>t40=%rax
mov  %rsi,%rax

# qhasm: t41 = t31
# asm 1: mov  <t31=int64#3,>t41=int64#8
# asm 2: mov  <t31=%rdx,>t41=%r10
mov  %rdx,%r10

# qhasm: t42 = t32
# asm 1: mov  <t32=int64#4,>t42=int64#9
# asm 2: mov  <t32=%rcx,>t42=%r11
mov  %rcx,%r11

# qhasm: t43 = t33
# asm 1: mov  <t33=int64#5,>t43=int64#10
# asm 2: mov  <t33=%r8,>t43=%r12
mov  %r8,%r12

# qhasm: t44 = t34
# asm 1: mov  <t34=int64#6,>t44=int64#11
# asm 2: mov  <t34=%r9,>t44=%r13
mov  %r9,%r13

# qhasm: t40 += *(uint64 *) &crypto_scalarmult_curve25519_amd64_51_2P0
# asm 1: add  crypto_scalarmult_curve25519_amd64_51_2P0,<t40=int64#7
# asm 2: add  crypto_scalarmult_curve25519_amd64_51_2P0,<t40=%rax
add  crypto_scalarmult_curve25519_amd64_51_2P0,%rax

# qhasm: t41 += *(uint64 *) &crypto_scalarmult_curve25519_amd64_51_2P1234
# asm 1: add  crypto_scalarmult_curve25519_amd64_51_2P1234,<t41=int64#8
# asm 2: add  crypto_scalarmult_curve25519_amd64_51_2P1234,<t41=%r10
add  crypto_scalarmult_curve25519_amd64_51_2P1234,%r10

# qhasm: t42 += *(uint64 *) &crypto_scalarmult_curve25519_amd64_51_2P1234
# asm 1: add  crypto_scalarmult_curve25519_amd64_51_2P1234,<t42=int64#9
# asm 2: add  crypto_scalarmult_curve25519_amd64_51_2P1234,<t42=%r11
add  crypto_scalarmult_curve25519_amd64_51_2P1234,%r11

# qhasm: t43 += *(uint64 *) &crypto_scalarmult_curve25519_amd64_51_2P1234
# asm 1: add  crypto_scalarmult_curve25519_amd64_51_2P1234,<t43=int64#10
# asm 2: add  crypto_scalarmult_curve25519_amd64_51_2P1234,<t43=%r12
add  crypto_scalarmult_curve25519_amd64_51_2P1234,%r12

# qhasm: t44 += *(uint64 *) &crypto_scalarmult_curve25519_amd64_51_2P1234
# asm 1: add  crypto_scalarmult_curve25519_amd64_51_2P1234,<t44=int64#11
# asm 2: add  crypto_scalarmult_curve25519_amd64_51_2P1234,<t44=%r13
add  crypto_scalarmult_curve25519_amd64_51_2P1234,%r13

# qhasm: t30 += *(uint64 *)(workp + 160)
# asm 1: addq 160(<workp=int64#1),<t30=int64#2
# asm 2: addq 160(<workp=%rdi),<t30=%rsi
addq 160(%rdi),%rsi

# qhasm: t31 += *(uint64 *)(workp + 168)
# asm 1: addq 168(<workp=int64#1),<t31=int64#3
# asm 2: addq 168(<workp=%rdi),<t31=%rdx
addq 168(%rdi),%rdx

# qhasm: t32 += *(uint64 *)(workp + 176)
# asm 1: addq 176(<workp=int64#1),<t32=int64#4
# asm 2: addq 176(<workp=%rdi),<t32=%rcx
addq 176(%rdi),%rcx

# qhasm: t33 += *(uint64 *)(workp + 184)
# asm 1: addq 184(<workp=int64#1),<t33=int64#5
# asm 2: addq 184(<workp=%rdi),<t33=%r8
addq 184(%rdi),%r8

# qhasm: t34 += *(uint64 *)(workp + 192)
# asm 1: addq 192(<workp=int64#1),<t34=int64#6
# asm 2: addq 192(<workp=%rdi),<t34=%r9
addq 192(%rdi),%r9

# qhasm: t40 -= *(uint64 *)(workp + 160)
# asm 1: subq 160(<workp=int64#1),<t40=int64#7
# asm 2: subq 160(<workp=%rdi),<t40=%rax
subq 160(%rdi),%rax

# qhasm: t41 -= *(uint64 *)(workp + 168)
# asm 1: subq 168(<workp=int64#1),<t41=int64#8
# asm 2: subq 168(<workp=%rdi),<t41=%r10
subq 168(%rdi),%r10

# qhasm: t42 -= *(uint64 *)(workp + 176)
# asm 1: subq 176(<workp=int64#1),<t42=int64#9
# asm 2: subq 176(<workp=%rdi),<t42=%r11
subq 176(%rdi),%r11

# qhasm: t43 -= *(uint64 *)(workp + 184)
# asm 1: subq 184(<workp=int64#1),<t43=int64#10
# asm 2: subq 184(<workp=%rdi),<t43=%r12
subq 184(%rdi),%r12

# qhasm: t44 -= *(uint64 *)(workp + 192)
# asm 1: subq 192(<workp=int64#1),<t44=int64#11
# asm 2: subq 192(<workp=%rdi),<t44=%r13
subq 192(%rdi),%r13

# qhasm: t30_stack = t30
# asm 1: movq <t30=int64#2,>t30_stack=stack64#33
# asm 2: movq <t30=%rsi,>t30_stack=256(%rsp)
movq %rsi,256(%rsp)

# qhasm: t31_stack = t31
# asm 1: movq <t31=int64#3,>t31_stack=stack64#34
# asm 2: movq <t31=%rdx,>t31_stack=264(%rsp)
movq %rdx,264(%rsp)

# qhasm: t32_stack = t32
# asm 1: movq <t32=int64#4,>t32_stack=stack64#35
# asm 2: movq <t32=%rcx,>t32_stack=272(%rsp)
movq %rcx,272(%rsp)

# qhasm: t33_stack = t33
# asm 1: movq <t33=int64#5,>t33_stack=stack64#36
# asm 2: movq <t33=%r8,>t33_stack=280(%rsp)
movq %r8,280(%rsp)

# qhasm: t34_stack = t34
# asm 1: movq <t34=int64#6,>t34_stack=stack64#37
# asm 2: movq <t34=%r9,>t34_stack=288(%rsp)
movq %r9,288(%rsp)

# qhasm: t40_stack = t40
# asm 1: movq <t40=int64#7,>t40_stack=stack64#38
# asm 2: movq <t40=%rax,>t40_stack=296(%rsp)
movq %rax,296(%rsp)

# qhasm: t41_stack = t41
# asm 1: movq <t41=int64#8,>t41_stack=stack64#39
# asm 2: movq <t41=%r10,>t41_stack=304(%rsp)
movq %r10,304(%rsp)

# qhasm: t42_stack = t42
# asm 1: movq <t42=int64#9,>t42_stack=stack64#40
# asm 2: movq <t42=%r11,>t42_stack=312(%rsp)
movq %r11,312(%rsp)

# qhasm: t43_stack = t43
# asm 1: movq <t43=int64#10,>t43_stack=stack64#41
# asm 2: movq <t43=%r12,>t43_stack=320(%rsp)
movq %r12,320(%rsp)

# qhasm: t44_stack = t44
# asm 1: movq <t44=int64#11,>t44_stack=stack64#42
# asm 2: movq <t44=%r13,>t44_stack=328(%rsp)
movq %r13,328(%rsp)

# qhasm:   mulrax = t33_stack
# asm 1: movq <t33_stack=stack64#36,>mulrax=int64#2
# asm 2: movq <t33_stack=280(%rsp),>mulrax=%rsi
movq 280(%rsp),%rsi

# qhasm:   mulrax *= 19
# asm 1: imulq  $19,<mulrax=int64#2,>mulrax=int64#7
# asm 2: imulq  $19,<mulrax=%rsi,>mulrax=%rax
imulq  $19,%rsi,%rax

# qhasm:   mulx319_stack = mulrax
# asm 1: movq <mulrax=int64#7,>mulx319_stack=stack64#43
# asm 2: movq <mulrax=%rax,>mulx319_stack=336(%rsp)
movq %rax,336(%rsp)

# qhasm:   (uint128) mulrdx mulrax = mulrax * t22_stack
# asm 1: mulq  <t22_stack=stack64#15
# asm 2: mulq  <t22_stack=112(%rsp)
mulq  112(%rsp)

# qhasm:   t90 = mulrax
# asm 1: mov  <mulrax=int64#7,>t90=int64#2
# asm 2: mov  <mulrax=%rax,>t90=%rsi
mov  %rax,%rsi

# qhasm:   mulr01 = mulrdx
# asm 1: mov  <mulrdx=int64#3,>mulr01=int64#4
# asm 2: mov  <mulrdx=%rdx,>mulr01=%rcx
mov  %rdx,%rcx

# qhasm:   mulrax = t34_stack
# asm 1: movq <t34_stack=stack64#37,>mulrax=int64#3
# asm 2: movq <t34_stack=288(%rsp),>mulrax=%rdx
movq 288(%rsp),%rdx

# qhasm:   mulrax *= 19
# asm 1: imulq  $19,<mulrax=int64#3,>mulrax=int64#7
# asm 2: imulq  $19,<mulrax=%rdx,>mulrax=%rax
imulq  $19,%rdx,%rax

# qhasm:   mulx419_stack = mulrax
# asm 1: movq <mulrax=int64#7,>mulx419_stack=stack64#44
# asm 2: movq <mulrax=%rax,>mulx419_stack=344(%rsp)
movq %rax,344(%rsp)

# qhasm:   (uint128) mulrdx mulrax = mulrax * t21_stack
# asm 1: mulq  <t21_stack=stack64#14
# asm 2: mulq  <t21_stack=104(%rsp)
mulq  104(%rsp)

# qhasm:   carry? t90 += mulrax
# asm 1: add  <mulrax=int64#7,<t90=int64#2
# asm 2: add  <mulrax=%rax,<t90=%rsi
add  %rax,%rsi

# qhasm:   mulr01 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr01=int64#4
# asm 2: adc <mulrdx=%rdx,<mulr01=%rcx
adc %rdx,%rcx

# qhasm:   mulrax = t30_stack
# asm 1: movq <t30_stack=stack64#33,>mulrax=int64#7
# asm 2: movq <t30_stack=256(%rsp),>mulrax=%rax
movq 256(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * t20_stack
# asm 1: mulq  <t20_stack=stack64#13
# asm 2: mulq  <t20_stack=96(%rsp)
mulq  96(%rsp)

# qhasm:   carry? t90 += mulrax
# asm 1: add  <mulrax=int64#7,<t90=int64#2
# asm 2: add  <mulrax=%rax,<t90=%rsi
add  %rax,%rsi

# qhasm:   mulr01 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr01=int64#4
# asm 2: adc <mulrdx=%rdx,<mulr01=%rcx
adc %rdx,%rcx

# qhasm:   mulrax = t30_stack
# asm 1: movq <t30_stack=stack64#33,>mulrax=int64#7
# asm 2: movq <t30_stack=256(%rsp),>mulrax=%rax
movq 256(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * t21_stack
# asm 1: mulq  <t21_stack=stack64#14
# asm 2: mulq  <t21_stack=104(%rsp)
mulq  104(%rsp)

# qhasm:   t91 = mulrax
# asm 1: mov  <mulrax=int64#7,>t91=int64#5
# asm 2: mov  <mulrax=%rax,>t91=%r8
mov  %rax,%r8

# qhasm:   mulr11 = mulrdx
# asm 1: mov  <mulrdx=int64#3,>mulr11=int64#6
# asm 2: mov  <mulrdx=%rdx,>mulr11=%r9
mov  %rdx,%r9

# qhasm:   mulrax = t30_stack
# asm 1: movq <t30_stack=stack64#33,>mulrax=int64#7
# asm 2: movq <t30_stack=256(%rsp),>mulrax=%rax
movq 256(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * t22_stack
# asm 1: mulq  <t22_stack=stack64#15
# asm 2: mulq  <t22_stack=112(%rsp)
mulq  112(%rsp)

# qhasm:   t92 = mulrax
# asm 1: mov  <mulrax=int64#7,>t92=int64#8
# asm 2: mov  <mulrax=%rax,>t92=%r10
mov  %rax,%r10

# qhasm:   mulr21 = mulrdx
# asm 1: mov  <mulrdx=int64#3,>mulr21=int64#9
# asm 2: mov  <mulrdx=%rdx,>mulr21=%r11
mov  %rdx,%r11

# qhasm:   mulrax = t30_stack
# asm 1: movq <t30_stack=stack64#33,>mulrax=int64#7
# asm 2: movq <t30_stack=256(%rsp),>mulrax=%rax
movq 256(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * t23_stack
# asm 1: mulq  <t23_stack=stack64#16
# asm 2: mulq  <t23_stack=120(%rsp)
mulq  120(%rsp)

# qhasm:   t93 = mulrax
# asm 1: mov  <mulrax=int64#7,>t93=int64#10
# asm 2: mov  <mulrax=%rax,>t93=%r12
mov  %rax,%r12

# qhasm:   mulr31 = mulrdx
# asm 1: mov  <mulrdx=int64#3,>mulr31=int64#11
# asm 2: mov  <mulrdx=%rdx,>mulr31=%r13
mov  %rdx,%r13

# qhasm:   mulrax = t30_stack
# asm 1: movq <t30_stack=stack64#33,>mulrax=int64#7
# asm 2: movq <t30_stack=256(%rsp),>mulrax=%rax
movq 256(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * t24_stack
# asm 1: mulq  <t24_stack=stack64#17
# asm 2: mulq  <t24_stack=128(%rsp)
mulq  128(%rsp)

# qhasm:   t94 = mulrax
# asm 1: mov  <mulrax=int64#7,>t94=int64#12
# asm 2: mov  <mulrax=%rax,>t94=%r14
mov  %rax,%r14

# qhasm:   mulr41 = mulrdx
# asm 1: mov  <mulrdx=int64#3,>mulr41=int64#13
# asm 2: mov  <mulrdx=%rdx,>mulr41=%r15
mov  %rdx,%r15

# qhasm:   mulrax = t31_stack
# asm 1: movq <t31_stack=stack64#34,>mulrax=int64#7
# asm 2: movq <t31_stack=264(%rsp),>mulrax=%rax
movq 264(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * t20_stack
# asm 1: mulq  <t20_stack=stack64#13
# asm 2: mulq  <t20_stack=96(%rsp)
mulq  96(%rsp)

# qhasm:   carry? t91 += mulrax
# asm 1: add  <mulrax=int64#7,<t91=int64#5
# asm 2: add  <mulrax=%rax,<t91=%r8
add  %rax,%r8

# qhasm:   mulr11 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr11=int64#6
# asm 2: adc <mulrdx=%rdx,<mulr11=%r9
adc %rdx,%r9

# qhasm:   mulrax = t31_stack
# asm 1: movq <t31_stack=stack64#34,>mulrax=int64#7
# asm 2: movq <t31_stack=264(%rsp),>mulrax=%rax
movq 264(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * t21_stack
# asm 1: mulq  <t21_stack=stack64#14
# asm 2: mulq  <t21_stack=104(%rsp)
mulq  104(%rsp)

# qhasm:   carry? t92 += mulrax
# asm 1: add  <mulrax=int64#7,<t92=int64#8
# asm 2: add  <mulrax=%rax,<t92=%r10
add  %rax,%r10

# qhasm:   mulr21 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr21=int64#9
# asm 2: adc <mulrdx=%rdx,<mulr21=%r11
adc %rdx,%r11

# qhasm:   mulrax = t31_stack
# asm 1: movq <t31_stack=stack64#34,>mulrax=int64#7
# asm 2: movq <t31_stack=264(%rsp),>mulrax=%rax
movq 264(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * t22_stack
# asm 1: mulq  <t22_stack=stack64#15
# asm 2: mulq  <t22_stack=112(%rsp)
mulq  112(%rsp)

# qhasm:   carry? t93 += mulrax
# asm 1: add  <mulrax=int64#7,<t93=int64#10
# asm 2: add  <mulrax=%rax,<t93=%r12
add  %rax,%r12

# qhasm:   mulr31 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr31=int64#11
# asm 2: adc <mulrdx=%rdx,<mulr31=%r13
adc %rdx,%r13

# qhasm:   mulrax = t31_stack
# asm 1: movq <t31_stack=stack64#34,>mulrax=int64#7
# asm 2: movq <t31_stack=264(%rsp),>mulrax=%rax
movq 264(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * t23_stack
# asm 1: mulq  <t23_stack=stack64#16
# asm 2: mulq  <t23_stack=120(%rsp)
mulq  120(%rsp)

# qhasm:   carry? t94 += mulrax
# asm 1: add  <mulrax=int64#7,<t94=int64#12
# asm 2: add  <mulrax=%rax,<t94=%r14
add  %rax,%r14

# qhasm:   mulr41 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr41=int64#13
# asm 2: adc <mulrdx=%rdx,<mulr41=%r15
adc %rdx,%r15

# qhasm:   mulrax = t31_stack
# asm 1: movq <t31_stack=stack64#34,>mulrax=int64#3
# asm 2: movq <t31_stack=264(%rsp),>mulrax=%rdx
movq 264(%rsp),%rdx

# qhasm:   mulrax *= 19
# asm 1: imulq  $19,<mulrax=int64#3,>mulrax=int64#7
# asm 2: imulq  $19,<mulrax=%rdx,>mulrax=%rax
imulq  $19,%rdx,%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * t24_stack
# asm 1: mulq  <t24_stack=stack64#17
# asm 2: mulq  <t24_stack=128(%rsp)
mulq  128(%rsp)

# qhasm:   carry? t90 += mulrax
# asm 1: add  <mulrax=int64#7,<t90=int64#2
# asm 2: add  <mulrax=%rax,<t90=%rsi
add  %rax,%rsi

# qhasm:   mulr01 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr01=int64#4
# asm 2: adc <mulrdx=%rdx,<mulr01=%rcx
adc %rdx,%rcx

# qhasm:   mulrax = t32_stack
# asm 1: movq <t32_stack=stack64#35,>mulrax=int64#7
# asm 2: movq <t32_stack=272(%rsp),>mulrax=%rax
movq 272(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * t20_stack
# asm 1: mulq  <t20_stack=stack64#13
# asm 2: mulq  <t20_stack=96(%rsp)
mulq  96(%rsp)

# qhasm:   carry? t92 += mulrax
# asm 1: add  <mulrax=int64#7,<t92=int64#8
# asm 2: add  <mulrax=%rax,<t92=%r10
add  %rax,%r10

# qhasm:   mulr21 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr21=int64#9
# asm 2: adc <mulrdx=%rdx,<mulr21=%r11
adc %rdx,%r11

# qhasm:   mulrax = t32_stack
# asm 1: movq <t32_stack=stack64#35,>mulrax=int64#7
# asm 2: movq <t32_stack=272(%rsp),>mulrax=%rax
movq 272(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * t21_stack
# asm 1: mulq  <t21_stack=stack64#14
# asm 2: mulq  <t21_stack=104(%rsp)
mulq  104(%rsp)

# qhasm:   carry? t93 += mulrax
# asm 1: add  <mulrax=int64#7,<t93=int64#10
# asm 2: add  <mulrax=%rax,<t93=%r12
add  %rax,%r12

# qhasm:   mulr31 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr31=int64#11
# asm 2: adc <mulrdx=%rdx,<mulr31=%r13
adc %rdx,%r13

# qhasm:   mulrax = t32_stack
# asm 1: movq <t32_stack=stack64#35,>mulrax=int64#7
# asm 2: movq <t32_stack=272(%rsp),>mulrax=%rax
movq 272(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * t22_stack
# asm 1: mulq  <t22_stack=stack64#15
# asm 2: mulq  <t22_stack=112(%rsp)
mulq  112(%rsp)

# qhasm:   carry? t94 += mulrax
# asm 1: add  <mulrax=int64#7,<t94=int64#12
# asm 2: add  <mulrax=%rax,<t94=%r14
add  %rax,%r14

# qhasm:   mulr41 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr41=int64#13
# asm 2: adc <mulrdx=%rdx,<mulr41=%r15
adc %rdx,%r15

# qhasm:   mulrax = t32_stack
# asm 1: movq <t32_stack=stack64#35,>mulrax=int64#3
# asm 2: movq <t32_stack=272(%rsp),>mulrax=%rdx
movq 272(%rsp),%rdx

# qhasm:   mulrax *= 19
# asm 1: imulq  $19,<mulrax=int64#3,>mulrax=int64#7
# asm 2: imulq  $19,<mulrax=%rdx,>mulrax=%rax
imulq  $19,%rdx,%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * t23_stack
# asm 1: mulq  <t23_stack=stack64#16
# asm 2: mulq  <t23_stack=120(%rsp)
mulq  120(%rsp)

# qhasm:   carry? t90 += mulrax
# asm 1: add  <mulrax=int64#7,<t90=int64#2
# asm 2: add  <mulrax=%rax,<t90=%rsi
add  %rax,%rsi

# qhasm:   mulr01 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr01=int64#4
# asm 2: adc <mulrdx=%rdx,<mulr01=%rcx
adc %rdx,%rcx

# qhasm:   mulrax = t32_stack
# asm 1: movq <t32_stack=stack64#35,>mulrax=int64#3
# asm 2: movq <t32_stack=272(%rsp),>mulrax=%rdx
movq 272(%rsp),%rdx

# qhasm:   mulrax *= 19
# asm 1: imulq  $19,<mulrax=int64#3,>mulrax=int64#7
# asm 2: imulq  $19,<mulrax=%rdx,>mulrax=%rax
imulq  $19,%rdx,%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * t24_stack
# asm 1: mulq  <t24_stack=stack64#17
# asm 2: mulq  <t24_stack=128(%rsp)
mulq  128(%rsp)

# qhasm:   carry? t91 += mulrax
# asm 1: add  <mulrax=int64#7,<t91=int64#5
# asm 2: add  <mulrax=%rax,<t91=%r8
add  %rax,%r8

# qhasm:   mulr11 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr11=int64#6
# asm 2: adc <mulrdx=%rdx,<mulr11=%r9
adc %rdx,%r9

# qhasm:   mulrax = t33_stack
# asm 1: movq <t33_stack=stack64#36,>mulrax=int64#7
# asm 2: movq <t33_stack=280(%rsp),>mulrax=%rax
movq 280(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * t20_stack
# asm 1: mulq  <t20_stack=stack64#13
# asm 2: mulq  <t20_stack=96(%rsp)
mulq  96(%rsp)

# qhasm:   carry? t93 += mulrax
# asm 1: add  <mulrax=int64#7,<t93=int64#10
# asm 2: add  <mulrax=%rax,<t93=%r12
add  %rax,%r12

# qhasm:   mulr31 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr31=int64#11
# asm 2: adc <mulrdx=%rdx,<mulr31=%r13
adc %rdx,%r13

# qhasm:   mulrax = t33_stack
# asm 1: movq <t33_stack=stack64#36,>mulrax=int64#7
# asm 2: movq <t33_stack=280(%rsp),>mulrax=%rax
movq 280(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * t21_stack
# asm 1: mulq  <t21_stack=stack64#14
# asm 2: mulq  <t21_stack=104(%rsp)
mulq  104(%rsp)

# qhasm:   carry? t94 += mulrax
# asm 1: add  <mulrax=int64#7,<t94=int64#12
# asm 2: add  <mulrax=%rax,<t94=%r14
add  %rax,%r14

# qhasm:   mulr41 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr41=int64#13
# asm 2: adc <mulrdx=%rdx,<mulr41=%r15
adc %rdx,%r15

# qhasm:   mulrax = mulx319_stack
# asm 1: movq <mulx319_stack=stack64#43,>mulrax=int64#7
# asm 2: movq <mulx319_stack=336(%rsp),>mulrax=%rax
movq 336(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * t23_stack
# asm 1: mulq  <t23_stack=stack64#16
# asm 2: mulq  <t23_stack=120(%rsp)
mulq  120(%rsp)

# qhasm:   carry? t91 += mulrax
# asm 1: add  <mulrax=int64#7,<t91=int64#5
# asm 2: add  <mulrax=%rax,<t91=%r8
add  %rax,%r8

# qhasm:   mulr11 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr11=int64#6
# asm 2: adc <mulrdx=%rdx,<mulr11=%r9
adc %rdx,%r9

# qhasm:   mulrax = mulx319_stack
# asm 1: movq <mulx319_stack=stack64#43,>mulrax=int64#7
# asm 2: movq <mulx319_stack=336(%rsp),>mulrax=%rax
movq 336(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * t24_stack
# asm 1: mulq  <t24_stack=stack64#17
# asm 2: mulq  <t24_stack=128(%rsp)
mulq  128(%rsp)

# qhasm:   carry? t92 += mulrax
# asm 1: add  <mulrax=int64#7,<t92=int64#8
# asm 2: add  <mulrax=%rax,<t92=%r10
add  %rax,%r10

# qhasm:   mulr21 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr21=int64#9
# asm 2: adc <mulrdx=%rdx,<mulr21=%r11
adc %rdx,%r11

# qhasm:   mulrax = t34_stack
# asm 1: movq <t34_stack=stack64#37,>mulrax=int64#7
# asm 2: movq <t34_stack=288(%rsp),>mulrax=%rax
movq 288(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * t20_stack
# asm 1: mulq  <t20_stack=stack64#13
# asm 2: mulq  <t20_stack=96(%rsp)
mulq  96(%rsp)

# qhasm:   carry? t94 += mulrax
# asm 1: add  <mulrax=int64#7,<t94=int64#12
# asm 2: add  <mulrax=%rax,<t94=%r14
add  %rax,%r14

# qhasm:   mulr41 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr41=int64#13
# asm 2: adc <mulrdx=%rdx,<mulr41=%r15
adc %rdx,%r15

# qhasm:   mulrax = mulx419_stack
# asm 1: movq <mulx419_stack=stack64#44,>mulrax=int64#7
# asm 2: movq <mulx419_stack=344(%rsp),>mulrax=%rax
movq 344(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * t22_stack
# asm 1: mulq  <t22_stack=stack64#15
# asm 2: mulq  <t22_stack=112(%rsp)
mulq  112(%rsp)

# qhasm:   carry? t91 += mulrax
# asm 1: add  <mulrax=int64#7,<t91=int64#5
# asm 2: add  <mulrax=%rax,<t91=%r8
add  %rax,%r8

# qhasm:   mulr11 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr11=int64#6
# asm 2: adc <mulrdx=%rdx,<mulr11=%r9
adc %rdx,%r9

# qhasm:   mulrax = mulx419_stack
# asm 1: movq <mulx419_stack=stack64#44,>mulrax=int64#7
# asm 2: movq <mulx419_stack=344(%rsp),>mulrax=%rax
movq 344(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * t23_stack
# asm 1: mulq  <t23_stack=stack64#16
# asm 2: mulq  <t23_stack=120(%rsp)
mulq  120(%rsp)

# qhasm:   carry? t92 += mulrax
# asm 1: add  <mulrax=int64#7,<t92=int64#8
# asm 2: add  <mulrax=%rax,<t92=%r10
add  %rax,%r10

# qhasm:   mulr21 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr21=int64#9
# asm 2: adc <mulrdx=%rdx,<mulr21=%r11
adc %rdx,%r11

# qhasm:   mulrax = mulx419_stack
# asm 1: movq <mulx419_stack=stack64#44,>mulrax=int64#7
# asm 2: movq <mulx419_stack=344(%rsp),>mulrax=%rax
movq 344(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * t24_stack
# asm 1: mulq  <t24_stack=stack64#17
# asm 2: mulq  <t24_stack=128(%rsp)
mulq  128(%rsp)

# qhasm:   carry? t93 += mulrax
# asm 1: add  <mulrax=int64#7,<t93=int64#10
# asm 2: add  <mulrax=%rax,<t93=%r12
add  %rax,%r12

# qhasm:   mulr31 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr31=int64#11
# asm 2: adc <mulrdx=%rdx,<mulr31=%r13
adc %rdx,%r13

# qhasm:   mulredmask = *(uint64 *) &crypto_scalarmult_curve25519_amd64_51_REDMASK51
# asm 1: movq crypto_scalarmult_curve25519_amd64_51_REDMASK51,>mulredmask=int64#3
# asm 2: movq crypto_scalarmult_curve25519_amd64_51_REDMASK51,>mulredmask=%rdx
movq crypto_scalarmult_curve25519_amd64_51_REDMASK51,%rdx

# qhasm:   mulr01 = (mulr01.t90) << 13
# asm 1: shld $13,<t90=int64#2,<mulr01=int64#4
# asm 2: shld $13,<t90=%rsi,<mulr01=%rcx
shld $13,%rsi,%rcx

# qhasm:   t90 &= mulredmask
# asm 1: and  <mulredmask=int64#3,<t90=int64#2
# asm 2: and  <mulredmask=%rdx,<t90=%rsi
and  %rdx,%rsi

# qhasm:   mulr11 = (mulr11.t91) << 13
# asm 1: shld $13,<t91=int64#5,<mulr11=int64#6
# asm 2: shld $13,<t91=%r8,<mulr11=%r9
shld $13,%r8,%r9

# qhasm:   t91 &= mulredmask
# asm 1: and  <mulredmask=int64#3,<t91=int64#5
# asm 2: and  <mulredmask=%rdx,<t91=%r8
and  %rdx,%r8

# qhasm:   t91 += mulr01
# asm 1: add  <mulr01=int64#4,<t91=int64#5
# asm 2: add  <mulr01=%rcx,<t91=%r8
add  %rcx,%r8

# qhasm:   mulr21 = (mulr21.t92) << 13
# asm 1: shld $13,<t92=int64#8,<mulr21=int64#9
# asm 2: shld $13,<t92=%r10,<mulr21=%r11
shld $13,%r10,%r11

# qhasm:   t92 &= mulredmask
# asm 1: and  <mulredmask=int64#3,<t92=int64#8
# asm 2: and  <mulredmask=%rdx,<t92=%r10
and  %rdx,%r10

# qhasm:   t92 += mulr11
# asm 1: add  <mulr11=int64#6,<t92=int64#8
# asm 2: add  <mulr11=%r9,<t92=%r10
add  %r9,%r10

# qhasm:   mulr31 = (mulr31.t93) << 13
# asm 1: shld $13,<t93=int64#10,<mulr31=int64#11
# asm 2: shld $13,<t93=%r12,<mulr31=%r13
shld $13,%r12,%r13

# qhasm:   t93 &= mulredmask
# asm 1: and  <mulredmask=int64#3,<t93=int64#10
# asm 2: and  <mulredmask=%rdx,<t93=%r12
and  %rdx,%r12

# qhasm:   t93 += mulr21
# asm 1: add  <mulr21=int64#9,<t93=int64#10
# asm 2: add  <mulr21=%r11,<t93=%r12
add  %r11,%r12

# qhasm:   mulr41 = (mulr41.t94) << 13
# asm 1: shld $13,<t94=int64#12,<mulr41=int64#13
# asm 2: shld $13,<t94=%r14,<mulr41=%r15
shld $13,%r14,%r15

# qhasm:   t94 &= mulredmask
# asm 1: and  <mulredmask=int64#3,<t94=int64#12
# asm 2: and  <mulredmask=%rdx,<t94=%r14
and  %rdx,%r14

# qhasm:   t94 += mulr31
# asm 1: add  <mulr31=int64#11,<t94=int64#12
# asm 2: add  <mulr31=%r13,<t94=%r14
add  %r13,%r14

# qhasm:   mulr41 = mulr41 * 19
# asm 1: imulq  $19,<mulr41=int64#13,>mulr41=int64#4
# asm 2: imulq  $19,<mulr41=%r15,>mulr41=%rcx
imulq  $19,%r15,%rcx

# qhasm:   t90 += mulr41
# asm 1: add  <mulr41=int64#4,<t90=int64#2
# asm 2: add  <mulr41=%rcx,<t90=%rsi
add  %rcx,%rsi

# qhasm:   mult = t90
# asm 1: mov  <t90=int64#2,>mult=int64#4
# asm 2: mov  <t90=%rsi,>mult=%rcx
mov  %rsi,%rcx

# qhasm:   (uint64) mult >>= 51
# asm 1: shr  $51,<mult=int64#4
# asm 2: shr  $51,<mult=%rcx
shr  $51,%rcx

# qhasm:   mult += t91
# asm 1: add  <t91=int64#5,<mult=int64#4
# asm 2: add  <t91=%r8,<mult=%rcx
add  %r8,%rcx

# qhasm:   t91 = mult
# asm 1: mov  <mult=int64#4,>t91=int64#5
# asm 2: mov  <mult=%rcx,>t91=%r8
mov  %rcx,%r8

# qhasm:   (uint64) mult >>= 51
# asm 1: shr  $51,<mult=int64#4
# asm 2: shr  $51,<mult=%rcx
shr  $51,%rcx

# qhasm:   t90 &= mulredmask
# asm 1: and  <mulredmask=int64#3,<t90=int64#2
# asm 2: and  <mulredmask=%rdx,<t90=%rsi
and  %rdx,%rsi

# qhasm:   mult += t92
# asm 1: add  <t92=int64#8,<mult=int64#4
# asm 2: add  <t92=%r10,<mult=%rcx
add  %r10,%rcx

# qhasm:   t92 = mult
# asm 1: mov  <mult=int64#4,>t92=int64#6
# asm 2: mov  <mult=%rcx,>t92=%r9
mov  %rcx,%r9

# qhasm:   (uint64) mult >>= 51
# asm 1: shr  $51,<mult=int64#4
# asm 2: shr  $51,<mult=%rcx
shr  $51,%rcx

# qhasm:   t91 &= mulredmask
# asm 1: and  <mulredmask=int64#3,<t91=int64#5
# asm 2: and  <mulredmask=%rdx,<t91=%r8
and  %rdx,%r8

# qhasm:   mult += t93
# asm 1: add  <t93=int64#10,<mult=int64#4
# asm 2: add  <t93=%r12,<mult=%rcx
add  %r12,%rcx

# qhasm:   t93 = mult
# asm 1: mov  <mult=int64#4,>t93=int64#7
# asm 2: mov  <mult=%rcx,>t93=%rax
mov  %rcx,%rax

# qhasm:   (uint64) mult >>= 51
# asm 1: shr  $51,<mult=int64#4
# asm 2: shr  $51,<mult=%rcx
shr  $51,%rcx

# qhasm:   t92 &= mulredmask
# asm 1: and  <mulredmask=int64#3,<t92=int64#6
# asm 2: and  <mulredmask=%rdx,<t92=%r9
and  %rdx,%r9

# qhasm:   mult += t94
# asm 1: add  <t94=int64#12,<mult=int64#4
# asm 2: add  <t94=%r14,<mult=%rcx
add  %r14,%rcx

# qhasm:   t94 = mult
# asm 1: mov  <mult=int64#4,>t94=int64#8
# asm 2: mov  <mult=%rcx,>t94=%r10
mov  %rcx,%r10

# qhasm:   (uint64) mult >>= 51
# asm 1: shr  $51,<mult=int64#4
# asm 2: shr  $51,<mult=%rcx
shr  $51,%rcx

# qhasm:   t93 &= mulredmask
# asm 1: and  <mulredmask=int64#3,<t93=int64#7
# asm 2: and  <mulredmask=%rdx,<t93=%rax
and  %rdx,%rax

# qhasm:   mult *= 19
# asm 1: imulq  $19,<mult=int64#4,>mult=int64#4
# asm 2: imulq  $19,<mult=%rcx,>mult=%rcx
imulq  $19,%rcx,%rcx

# qhasm:   t90 += mult
# asm 1: add  <mult=int64#4,<t90=int64#2
# asm 2: add  <mult=%rcx,<t90=%rsi
add  %rcx,%rsi

# qhasm:   t94 &= mulredmask
# asm 1: and  <mulredmask=int64#3,<t94=int64#8
# asm 2: and  <mulredmask=%rdx,<t94=%r10
and  %rdx,%r10

# qhasm: t90_stack = t90
# asm 1: movq <t90=int64#2,>t90_stack=stack64#13
# asm 2: movq <t90=%rsi,>t90_stack=96(%rsp)
movq %rsi,96(%rsp)

# qhasm: t91_stack = t91
# asm 1: movq <t91=int64#5,>t91_stack=stack64#14
# asm 2: movq <t91=%r8,>t91_stack=104(%rsp)
movq %r8,104(%rsp)

# qhasm: t92_stack = t92
# asm 1: movq <t92=int64#6,>t92_stack=stack64#15
# asm 2: movq <t92=%r9,>t92_stack=112(%rsp)
movq %r9,112(%rsp)

# qhasm: t93_stack = t93
# asm 1: movq <t93=int64#7,>t93_stack=stack64#16
# asm 2: movq <t93=%rax,>t93_stack=120(%rsp)
movq %rax,120(%rsp)

# qhasm: t94_stack = t94
# asm 1: movq <t94=int64#8,>t94_stack=stack64#17
# asm 2: movq <t94=%r10,>t94_stack=128(%rsp)
movq %r10,128(%rsp)

# qhasm:   mulrax = t43_stack
# asm 1: movq <t43_stack=stack64#41,>mulrax=int64#2
# asm 2: movq <t43_stack=320(%rsp),>mulrax=%rsi
movq 320(%rsp),%rsi

# qhasm:   mulrax *= 19
# asm 1: imulq  $19,<mulrax=int64#2,>mulrax=int64#7
# asm 2: imulq  $19,<mulrax=%rsi,>mulrax=%rax
imulq  $19,%rsi,%rax

# qhasm:   mulx319_stack = mulrax
# asm 1: movq <mulrax=int64#7,>mulx319_stack=stack64#33
# asm 2: movq <mulrax=%rax,>mulx319_stack=256(%rsp)
movq %rax,256(%rsp)

# qhasm:   (uint128) mulrdx mulrax = mulrax * t12_stack
# asm 1: mulq  <t12_stack=stack64#10
# asm 2: mulq  <t12_stack=72(%rsp)
mulq  72(%rsp)

# qhasm:   t80 = mulrax
# asm 1: mov  <mulrax=int64#7,>t80=int64#2
# asm 2: mov  <mulrax=%rax,>t80=%rsi
mov  %rax,%rsi

# qhasm:   mulr01 = mulrdx
# asm 1: mov  <mulrdx=int64#3,>mulr01=int64#4
# asm 2: mov  <mulrdx=%rdx,>mulr01=%rcx
mov  %rdx,%rcx

# qhasm:   mulrax = t44_stack
# asm 1: movq <t44_stack=stack64#42,>mulrax=int64#3
# asm 2: movq <t44_stack=328(%rsp),>mulrax=%rdx
movq 328(%rsp),%rdx

# qhasm:   mulrax *= 19
# asm 1: imulq  $19,<mulrax=int64#3,>mulrax=int64#7
# asm 2: imulq  $19,<mulrax=%rdx,>mulrax=%rax
imulq  $19,%rdx,%rax

# qhasm:   mulx419_stack = mulrax
# asm 1: movq <mulrax=int64#7,>mulx419_stack=stack64#34
# asm 2: movq <mulrax=%rax,>mulx419_stack=264(%rsp)
movq %rax,264(%rsp)

# qhasm:   (uint128) mulrdx mulrax = mulrax * t11_stack
# asm 1: mulq  <t11_stack=stack64#9
# asm 2: mulq  <t11_stack=64(%rsp)
mulq  64(%rsp)

# qhasm:   carry? t80 += mulrax
# asm 1: add  <mulrax=int64#7,<t80=int64#2
# asm 2: add  <mulrax=%rax,<t80=%rsi
add  %rax,%rsi

# qhasm:   mulr01 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr01=int64#4
# asm 2: adc <mulrdx=%rdx,<mulr01=%rcx
adc %rdx,%rcx

# qhasm:   mulrax = t40_stack
# asm 1: movq <t40_stack=stack64#38,>mulrax=int64#7
# asm 2: movq <t40_stack=296(%rsp),>mulrax=%rax
movq 296(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * t10_stack
# asm 1: mulq  <t10_stack=stack64#8
# asm 2: mulq  <t10_stack=56(%rsp)
mulq  56(%rsp)

# qhasm:   carry? t80 += mulrax
# asm 1: add  <mulrax=int64#7,<t80=int64#2
# asm 2: add  <mulrax=%rax,<t80=%rsi
add  %rax,%rsi

# qhasm:   mulr01 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr01=int64#4
# asm 2: adc <mulrdx=%rdx,<mulr01=%rcx
adc %rdx,%rcx

# qhasm:   mulrax = t40_stack
# asm 1: movq <t40_stack=stack64#38,>mulrax=int64#7
# asm 2: movq <t40_stack=296(%rsp),>mulrax=%rax
movq 296(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * t11_stack
# asm 1: mulq  <t11_stack=stack64#9
# asm 2: mulq  <t11_stack=64(%rsp)
mulq  64(%rsp)

# qhasm:   t81 = mulrax
# asm 1: mov  <mulrax=int64#7,>t81=int64#5
# asm 2: mov  <mulrax=%rax,>t81=%r8
mov  %rax,%r8

# qhasm:   mulr11 = mulrdx
# asm 1: mov  <mulrdx=int64#3,>mulr11=int64#6
# asm 2: mov  <mulrdx=%rdx,>mulr11=%r9
mov  %rdx,%r9

# qhasm:   mulrax = t40_stack
# asm 1: movq <t40_stack=stack64#38,>mulrax=int64#7
# asm 2: movq <t40_stack=296(%rsp),>mulrax=%rax
movq 296(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * t12_stack
# asm 1: mulq  <t12_stack=stack64#10
# asm 2: mulq  <t12_stack=72(%rsp)
mulq  72(%rsp)

# qhasm:   t82 = mulrax
# asm 1: mov  <mulrax=int64#7,>t82=int64#8
# asm 2: mov  <mulrax=%rax,>t82=%r10
mov  %rax,%r10

# qhasm:   mulr21 = mulrdx
# asm 1: mov  <mulrdx=int64#3,>mulr21=int64#9
# asm 2: mov  <mulrdx=%rdx,>mulr21=%r11
mov  %rdx,%r11

# qhasm:   mulrax = t40_stack
# asm 1: movq <t40_stack=stack64#38,>mulrax=int64#7
# asm 2: movq <t40_stack=296(%rsp),>mulrax=%rax
movq 296(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * t13_stack
# asm 1: mulq  <t13_stack=stack64#11
# asm 2: mulq  <t13_stack=80(%rsp)
mulq  80(%rsp)

# qhasm:   t83 = mulrax
# asm 1: mov  <mulrax=int64#7,>t83=int64#10
# asm 2: mov  <mulrax=%rax,>t83=%r12
mov  %rax,%r12

# qhasm:   mulr31 = mulrdx
# asm 1: mov  <mulrdx=int64#3,>mulr31=int64#11
# asm 2: mov  <mulrdx=%rdx,>mulr31=%r13
mov  %rdx,%r13

# qhasm:   mulrax = t40_stack
# asm 1: movq <t40_stack=stack64#38,>mulrax=int64#7
# asm 2: movq <t40_stack=296(%rsp),>mulrax=%rax
movq 296(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * t14_stack
# asm 1: mulq  <t14_stack=stack64#12
# asm 2: mulq  <t14_stack=88(%rsp)
mulq  88(%rsp)

# qhasm:   t84 = mulrax
# asm 1: mov  <mulrax=int64#7,>t84=int64#12
# asm 2: mov  <mulrax=%rax,>t84=%r14
mov  %rax,%r14

# qhasm:   mulr41 = mulrdx
# asm 1: mov  <mulrdx=int64#3,>mulr41=int64#13
# asm 2: mov  <mulrdx=%rdx,>mulr41=%r15
mov  %rdx,%r15

# qhasm:   mulrax = t41_stack
# asm 1: movq <t41_stack=stack64#39,>mulrax=int64#7
# asm 2: movq <t41_stack=304(%rsp),>mulrax=%rax
movq 304(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * t10_stack
# asm 1: mulq  <t10_stack=stack64#8
# asm 2: mulq  <t10_stack=56(%rsp)
mulq  56(%rsp)

# qhasm:   carry? t81 += mulrax
# asm 1: add  <mulrax=int64#7,<t81=int64#5
# asm 2: add  <mulrax=%rax,<t81=%r8
add  %rax,%r8

# qhasm:   mulr11 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr11=int64#6
# asm 2: adc <mulrdx=%rdx,<mulr11=%r9
adc %rdx,%r9

# qhasm:   mulrax = t41_stack
# asm 1: movq <t41_stack=stack64#39,>mulrax=int64#7
# asm 2: movq <t41_stack=304(%rsp),>mulrax=%rax
movq 304(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * t11_stack
# asm 1: mulq  <t11_stack=stack64#9
# asm 2: mulq  <t11_stack=64(%rsp)
mulq  64(%rsp)

# qhasm:   carry? t82 += mulrax
# asm 1: add  <mulrax=int64#7,<t82=int64#8
# asm 2: add  <mulrax=%rax,<t82=%r10
add  %rax,%r10

# qhasm:   mulr21 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr21=int64#9
# asm 2: adc <mulrdx=%rdx,<mulr21=%r11
adc %rdx,%r11

# qhasm:   mulrax = t41_stack
# asm 1: movq <t41_stack=stack64#39,>mulrax=int64#7
# asm 2: movq <t41_stack=304(%rsp),>mulrax=%rax
movq 304(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * t12_stack
# asm 1: mulq  <t12_stack=stack64#10
# asm 2: mulq  <t12_stack=72(%rsp)
mulq  72(%rsp)

# qhasm:   carry? t83 += mulrax
# asm 1: add  <mulrax=int64#7,<t83=int64#10
# asm 2: add  <mulrax=%rax,<t83=%r12
add  %rax,%r12

# qhasm:   mulr31 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr31=int64#11
# asm 2: adc <mulrdx=%rdx,<mulr31=%r13
adc %rdx,%r13

# qhasm:   mulrax = t41_stack
# asm 1: movq <t41_stack=stack64#39,>mulrax=int64#7
# asm 2: movq <t41_stack=304(%rsp),>mulrax=%rax
movq 304(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * t13_stack
# asm 1: mulq  <t13_stack=stack64#11
# asm 2: mulq  <t13_stack=80(%rsp)
mulq  80(%rsp)

# qhasm:   carry? t84 += mulrax
# asm 1: add  <mulrax=int64#7,<t84=int64#12
# asm 2: add  <mulrax=%rax,<t84=%r14
add  %rax,%r14

# qhasm:   mulr41 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr41=int64#13
# asm 2: adc <mulrdx=%rdx,<mulr41=%r15
adc %rdx,%r15

# qhasm:   mulrax = t41_stack
# asm 1: movq <t41_stack=stack64#39,>mulrax=int64#3
# asm 2: movq <t41_stack=304(%rsp),>mulrax=%rdx
movq 304(%rsp),%rdx

# qhasm:   mulrax *= 19
# asm 1: imulq  $19,<mulrax=int64#3,>mulrax=int64#7
# asm 2: imulq  $19,<mulrax=%rdx,>mulrax=%rax
imulq  $19,%rdx,%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * t14_stack
# asm 1: mulq  <t14_stack=stack64#12
# asm 2: mulq  <t14_stack=88(%rsp)
mulq  88(%rsp)

# qhasm:   carry? t80 += mulrax
# asm 1: add  <mulrax=int64#7,<t80=int64#2
# asm 2: add  <mulrax=%rax,<t80=%rsi
add  %rax,%rsi

# qhasm:   mulr01 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr01=int64#4
# asm 2: adc <mulrdx=%rdx,<mulr01=%rcx
adc %rdx,%rcx

# qhasm:   mulrax = t42_stack
# asm 1: movq <t42_stack=stack64#40,>mulrax=int64#7
# asm 2: movq <t42_stack=312(%rsp),>mulrax=%rax
movq 312(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * t10_stack
# asm 1: mulq  <t10_stack=stack64#8
# asm 2: mulq  <t10_stack=56(%rsp)
mulq  56(%rsp)

# qhasm:   carry? t82 += mulrax
# asm 1: add  <mulrax=int64#7,<t82=int64#8
# asm 2: add  <mulrax=%rax,<t82=%r10
add  %rax,%r10

# qhasm:   mulr21 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr21=int64#9
# asm 2: adc <mulrdx=%rdx,<mulr21=%r11
adc %rdx,%r11

# qhasm:   mulrax = t42_stack
# asm 1: movq <t42_stack=stack64#40,>mulrax=int64#7
# asm 2: movq <t42_stack=312(%rsp),>mulrax=%rax
movq 312(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * t11_stack
# asm 1: mulq  <t11_stack=stack64#9
# asm 2: mulq  <t11_stack=64(%rsp)
mulq  64(%rsp)

# qhasm:   carry? t83 += mulrax
# asm 1: add  <mulrax=int64#7,<t83=int64#10
# asm 2: add  <mulrax=%rax,<t83=%r12
add  %rax,%r12

# qhasm:   mulr31 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr31=int64#11
# asm 2: adc <mulrdx=%rdx,<mulr31=%r13
adc %rdx,%r13

# qhasm:   mulrax = t42_stack
# asm 1: movq <t42_stack=stack64#40,>mulrax=int64#7
# asm 2: movq <t42_stack=312(%rsp),>mulrax=%rax
movq 312(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * t12_stack
# asm 1: mulq  <t12_stack=stack64#10
# asm 2: mulq  <t12_stack=72(%rsp)
mulq  72(%rsp)

# qhasm:   carry? t84 += mulrax
# asm 1: add  <mulrax=int64#7,<t84=int64#12
# asm 2: add  <mulrax=%rax,<t84=%r14
add  %rax,%r14

# qhasm:   mulr41 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr41=int64#13
# asm 2: adc <mulrdx=%rdx,<mulr41=%r15
adc %rdx,%r15

# qhasm:   mulrax = t42_stack
# asm 1: movq <t42_stack=stack64#40,>mulrax=int64#3
# asm 2: movq <t42_stack=312(%rsp),>mulrax=%rdx
movq 312(%rsp),%rdx

# qhasm:   mulrax *= 19
# asm 1: imulq  $19,<mulrax=int64#3,>mulrax=int64#7
# asm 2: imulq  $19,<mulrax=%rdx,>mulrax=%rax
imulq  $19,%rdx,%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * t13_stack
# asm 1: mulq  <t13_stack=stack64#11
# asm 2: mulq  <t13_stack=80(%rsp)
mulq  80(%rsp)

# qhasm:   carry? t80 += mulrax
# asm 1: add  <mulrax=int64#7,<t80=int64#2
# asm 2: add  <mulrax=%rax,<t80=%rsi
add  %rax,%rsi

# qhasm:   mulr01 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr01=int64#4
# asm 2: adc <mulrdx=%rdx,<mulr01=%rcx
adc %rdx,%rcx

# qhasm:   mulrax = t42_stack
# asm 1: movq <t42_stack=stack64#40,>mulrax=int64#3
# asm 2: movq <t42_stack=312(%rsp),>mulrax=%rdx
movq 312(%rsp),%rdx

# qhasm:   mulrax *= 19
# asm 1: imulq  $19,<mulrax=int64#3,>mulrax=int64#7
# asm 2: imulq  $19,<mulrax=%rdx,>mulrax=%rax
imulq  $19,%rdx,%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * t14_stack
# asm 1: mulq  <t14_stack=stack64#12
# asm 2: mulq  <t14_stack=88(%rsp)
mulq  88(%rsp)

# qhasm:   carry? t81 += mulrax
# asm 1: add  <mulrax=int64#7,<t81=int64#5
# asm 2: add  <mulrax=%rax,<t81=%r8
add  %rax,%r8

# qhasm:   mulr11 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr11=int64#6
# asm 2: adc <mulrdx=%rdx,<mulr11=%r9
adc %rdx,%r9

# qhasm:   mulrax = t43_stack
# asm 1: movq <t43_stack=stack64#41,>mulrax=int64#7
# asm 2: movq <t43_stack=320(%rsp),>mulrax=%rax
movq 320(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * t10_stack
# asm 1: mulq  <t10_stack=stack64#8
# asm 2: mulq  <t10_stack=56(%rsp)
mulq  56(%rsp)

# qhasm:   carry? t83 += mulrax
# asm 1: add  <mulrax=int64#7,<t83=int64#10
# asm 2: add  <mulrax=%rax,<t83=%r12
add  %rax,%r12

# qhasm:   mulr31 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr31=int64#11
# asm 2: adc <mulrdx=%rdx,<mulr31=%r13
adc %rdx,%r13

# qhasm:   mulrax = t43_stack
# asm 1: movq <t43_stack=stack64#41,>mulrax=int64#7
# asm 2: movq <t43_stack=320(%rsp),>mulrax=%rax
movq 320(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * t11_stack
# asm 1: mulq  <t11_stack=stack64#9
# asm 2: mulq  <t11_stack=64(%rsp)
mulq  64(%rsp)

# qhasm:   carry? t84 += mulrax
# asm 1: add  <mulrax=int64#7,<t84=int64#12
# asm 2: add  <mulrax=%rax,<t84=%r14
add  %rax,%r14

# qhasm:   mulr41 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr41=int64#13
# asm 2: adc <mulrdx=%rdx,<mulr41=%r15
adc %rdx,%r15

# qhasm:   mulrax = mulx319_stack
# asm 1: movq <mulx319_stack=stack64#33,>mulrax=int64#7
# asm 2: movq <mulx319_stack=256(%rsp),>mulrax=%rax
movq 256(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * t13_stack
# asm 1: mulq  <t13_stack=stack64#11
# asm 2: mulq  <t13_stack=80(%rsp)
mulq  80(%rsp)

# qhasm:   carry? t81 += mulrax
# asm 1: add  <mulrax=int64#7,<t81=int64#5
# asm 2: add  <mulrax=%rax,<t81=%r8
add  %rax,%r8

# qhasm:   mulr11 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr11=int64#6
# asm 2: adc <mulrdx=%rdx,<mulr11=%r9
adc %rdx,%r9

# qhasm:   mulrax = mulx319_stack
# asm 1: movq <mulx319_stack=stack64#33,>mulrax=int64#7
# asm 2: movq <mulx319_stack=256(%rsp),>mulrax=%rax
movq 256(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * t14_stack
# asm 1: mulq  <t14_stack=stack64#12
# asm 2: mulq  <t14_stack=88(%rsp)
mulq  88(%rsp)

# qhasm:   carry? t82 += mulrax
# asm 1: add  <mulrax=int64#7,<t82=int64#8
# asm 2: add  <mulrax=%rax,<t82=%r10
add  %rax,%r10

# qhasm:   mulr21 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr21=int64#9
# asm 2: adc <mulrdx=%rdx,<mulr21=%r11
adc %rdx,%r11

# qhasm:   mulrax = t44_stack
# asm 1: movq <t44_stack=stack64#42,>mulrax=int64#7
# asm 2: movq <t44_stack=328(%rsp),>mulrax=%rax
movq 328(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * t10_stack
# asm 1: mulq  <t10_stack=stack64#8
# asm 2: mulq  <t10_stack=56(%rsp)
mulq  56(%rsp)

# qhasm:   carry? t84 += mulrax
# asm 1: add  <mulrax=int64#7,<t84=int64#12
# asm 2: add  <mulrax=%rax,<t84=%r14
add  %rax,%r14

# qhasm:   mulr41 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr41=int64#13
# asm 2: adc <mulrdx=%rdx,<mulr41=%r15
adc %rdx,%r15

# qhasm:   mulrax = mulx419_stack
# asm 1: movq <mulx419_stack=stack64#34,>mulrax=int64#7
# asm 2: movq <mulx419_stack=264(%rsp),>mulrax=%rax
movq 264(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * t12_stack
# asm 1: mulq  <t12_stack=stack64#10
# asm 2: mulq  <t12_stack=72(%rsp)
mulq  72(%rsp)

# qhasm:   carry? t81 += mulrax
# asm 1: add  <mulrax=int64#7,<t81=int64#5
# asm 2: add  <mulrax=%rax,<t81=%r8
add  %rax,%r8

# qhasm:   mulr11 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr11=int64#6
# asm 2: adc <mulrdx=%rdx,<mulr11=%r9
adc %rdx,%r9

# qhasm:   mulrax = mulx419_stack
# asm 1: movq <mulx419_stack=stack64#34,>mulrax=int64#7
# asm 2: movq <mulx419_stack=264(%rsp),>mulrax=%rax
movq 264(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * t13_stack
# asm 1: mulq  <t13_stack=stack64#11
# asm 2: mulq  <t13_stack=80(%rsp)
mulq  80(%rsp)

# qhasm:   carry? t82 += mulrax
# asm 1: add  <mulrax=int64#7,<t82=int64#8
# asm 2: add  <mulrax=%rax,<t82=%r10
add  %rax,%r10

# qhasm:   mulr21 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr21=int64#9
# asm 2: adc <mulrdx=%rdx,<mulr21=%r11
adc %rdx,%r11

# qhasm:   mulrax = mulx419_stack
# asm 1: movq <mulx419_stack=stack64#34,>mulrax=int64#7
# asm 2: movq <mulx419_stack=264(%rsp),>mulrax=%rax
movq 264(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * t14_stack
# asm 1: mulq  <t14_stack=stack64#12
# asm 2: mulq  <t14_stack=88(%rsp)
mulq  88(%rsp)

# qhasm:   carry? t83 += mulrax
# asm 1: add  <mulrax=int64#7,<t83=int64#10
# asm 2: add  <mulrax=%rax,<t83=%r12
add  %rax,%r12

# qhasm:   mulr31 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr31=int64#11
# asm 2: adc <mulrdx=%rdx,<mulr31=%r13
adc %rdx,%r13

# qhasm:   mulredmask = *(uint64 *) &crypto_scalarmult_curve25519_amd64_51_REDMASK51
# asm 1: movq crypto_scalarmult_curve25519_amd64_51_REDMASK51,>mulredmask=int64#3
# asm 2: movq crypto_scalarmult_curve25519_amd64_51_REDMASK51,>mulredmask=%rdx
movq crypto_scalarmult_curve25519_amd64_51_REDMASK51,%rdx

# qhasm:   mulr01 = (mulr01.t80) << 13
# asm 1: shld $13,<t80=int64#2,<mulr01=int64#4
# asm 2: shld $13,<t80=%rsi,<mulr01=%rcx
shld $13,%rsi,%rcx

# qhasm:   t80 &= mulredmask
# asm 1: and  <mulredmask=int64#3,<t80=int64#2
# asm 2: and  <mulredmask=%rdx,<t80=%rsi
and  %rdx,%rsi

# qhasm:   mulr11 = (mulr11.t81) << 13
# asm 1: shld $13,<t81=int64#5,<mulr11=int64#6
# asm 2: shld $13,<t81=%r8,<mulr11=%r9
shld $13,%r8,%r9

# qhasm:   t81 &= mulredmask
# asm 1: and  <mulredmask=int64#3,<t81=int64#5
# asm 2: and  <mulredmask=%rdx,<t81=%r8
and  %rdx,%r8

# qhasm:   t81 += mulr01
# asm 1: add  <mulr01=int64#4,<t81=int64#5
# asm 2: add  <mulr01=%rcx,<t81=%r8
add  %rcx,%r8

# qhasm:   mulr21 = (mulr21.t82) << 13
# asm 1: shld $13,<t82=int64#8,<mulr21=int64#9
# asm 2: shld $13,<t82=%r10,<mulr21=%r11
shld $13,%r10,%r11

# qhasm:   t82 &= mulredmask
# asm 1: and  <mulredmask=int64#3,<t82=int64#8
# asm 2: and  <mulredmask=%rdx,<t82=%r10
and  %rdx,%r10

# qhasm:   t82 += mulr11
# asm 1: add  <mulr11=int64#6,<t82=int64#8
# asm 2: add  <mulr11=%r9,<t82=%r10
add  %r9,%r10

# qhasm:   mulr31 = (mulr31.t83) << 13
# asm 1: shld $13,<t83=int64#10,<mulr31=int64#11
# asm 2: shld $13,<t83=%r12,<mulr31=%r13
shld $13,%r12,%r13

# qhasm:   t83 &= mulredmask
# asm 1: and  <mulredmask=int64#3,<t83=int64#10
# asm 2: and  <mulredmask=%rdx,<t83=%r12
and  %rdx,%r12

# qhasm:   t83 += mulr21
# asm 1: add  <mulr21=int64#9,<t83=int64#10
# asm 2: add  <mulr21=%r11,<t83=%r12
add  %r11,%r12

# qhasm:   mulr41 = (mulr41.t84) << 13
# asm 1: shld $13,<t84=int64#12,<mulr41=int64#13
# asm 2: shld $13,<t84=%r14,<mulr41=%r15
shld $13,%r14,%r15

# qhasm:   t84 &= mulredmask
# asm 1: and  <mulredmask=int64#3,<t84=int64#12
# asm 2: and  <mulredmask=%rdx,<t84=%r14
and  %rdx,%r14

# qhasm:   t84 += mulr31
# asm 1: add  <mulr31=int64#11,<t84=int64#12
# asm 2: add  <mulr31=%r13,<t84=%r14
add  %r13,%r14

# qhasm:   mulr41 = mulr41 * 19
# asm 1: imulq  $19,<mulr41=int64#13,>mulr41=int64#4
# asm 2: imulq  $19,<mulr41=%r15,>mulr41=%rcx
imulq  $19,%r15,%rcx

# qhasm:   t80 += mulr41
# asm 1: add  <mulr41=int64#4,<t80=int64#2
# asm 2: add  <mulr41=%rcx,<t80=%rsi
add  %rcx,%rsi

# qhasm:   mult = t80
# asm 1: mov  <t80=int64#2,>mult=int64#4
# asm 2: mov  <t80=%rsi,>mult=%rcx
mov  %rsi,%rcx

# qhasm:   (uint64) mult >>= 51
# asm 1: shr  $51,<mult=int64#4
# asm 2: shr  $51,<mult=%rcx
shr  $51,%rcx

# qhasm:   mult += t81
# asm 1: add  <t81=int64#5,<mult=int64#4
# asm 2: add  <t81=%r8,<mult=%rcx
add  %r8,%rcx

# qhasm:   t81 = mult
# asm 1: mov  <mult=int64#4,>t81=int64#5
# asm 2: mov  <mult=%rcx,>t81=%r8
mov  %rcx,%r8

# qhasm:   (uint64) mult >>= 51
# asm 1: shr  $51,<mult=int64#4
# asm 2: shr  $51,<mult=%rcx
shr  $51,%rcx

# qhasm:   t80 &= mulredmask
# asm 1: and  <mulredmask=int64#3,<t80=int64#2
# asm 2: and  <mulredmask=%rdx,<t80=%rsi
and  %rdx,%rsi

# qhasm:   mult += t82
# asm 1: add  <t82=int64#8,<mult=int64#4
# asm 2: add  <t82=%r10,<mult=%rcx
add  %r10,%rcx

# qhasm:   t82 = mult
# asm 1: mov  <mult=int64#4,>t82=int64#6
# asm 2: mov  <mult=%rcx,>t82=%r9
mov  %rcx,%r9

# qhasm:   (uint64) mult >>= 51
# asm 1: shr  $51,<mult=int64#4
# asm 2: shr  $51,<mult=%rcx
shr  $51,%rcx

# qhasm:   t81 &= mulredmask
# asm 1: and  <mulredmask=int64#3,<t81=int64#5
# asm 2: and  <mulredmask=%rdx,<t81=%r8
and  %rdx,%r8

# qhasm:   mult += t83
# asm 1: add  <t83=int64#10,<mult=int64#4
# asm 2: add  <t83=%r12,<mult=%rcx
add  %r12,%rcx

# qhasm:   t83 = mult
# asm 1: mov  <mult=int64#4,>t83=int64#7
# asm 2: mov  <mult=%rcx,>t83=%rax
mov  %rcx,%rax

# qhasm:   (uint64) mult >>= 51
# asm 1: shr  $51,<mult=int64#4
# asm 2: shr  $51,<mult=%rcx
shr  $51,%rcx

# qhasm:   t82 &= mulredmask
# asm 1: and  <mulredmask=int64#3,<t82=int64#6
# asm 2: and  <mulredmask=%rdx,<t82=%r9
and  %rdx,%r9

# qhasm:   mult += t84
# asm 1: add  <t84=int64#12,<mult=int64#4
# asm 2: add  <t84=%r14,<mult=%rcx
add  %r14,%rcx

# qhasm:   t84 = mult
# asm 1: mov  <mult=int64#4,>t84=int64#8
# asm 2: mov  <mult=%rcx,>t84=%r10
mov  %rcx,%r10

# qhasm:   (uint64) mult >>= 51
# asm 1: shr  $51,<mult=int64#4
# asm 2: shr  $51,<mult=%rcx
shr  $51,%rcx

# qhasm:   t83 &= mulredmask
# asm 1: and  <mulredmask=int64#3,<t83=int64#7
# asm 2: and  <mulredmask=%rdx,<t83=%rax
and  %rdx,%rax

# qhasm:   mult *= 19
# asm 1: imulq  $19,<mult=int64#4,>mult=int64#4
# asm 2: imulq  $19,<mult=%rcx,>mult=%rcx
imulq  $19,%rcx,%rcx

# qhasm:   t80 += mult
# asm 1: add  <mult=int64#4,<t80=int64#2
# asm 2: add  <mult=%rcx,<t80=%rsi
add  %rcx,%rsi

# qhasm:   t84 &= mulredmask
# asm 1: and  <mulredmask=int64#3,<t84=int64#8
# asm 2: and  <mulredmask=%rdx,<t84=%r10
and  %rdx,%r10

# qhasm: zq0 = t80
# asm 1: mov  <t80=int64#2,>zq0=int64#3
# asm 2: mov  <t80=%rsi,>zq0=%rdx
mov  %rsi,%rdx

# qhasm: zq1 = t81
# asm 1: mov  <t81=int64#5,>zq1=int64#4
# asm 2: mov  <t81=%r8,>zq1=%rcx
mov  %r8,%rcx

# qhasm: zq2 = t82
# asm 1: mov  <t82=int64#6,>zq2=int64#9
# asm 2: mov  <t82=%r9,>zq2=%r11
mov  %r9,%r11

# qhasm: zq3 = t83
# asm 1: mov  <t83=int64#7,>zq3=int64#10
# asm 2: mov  <t83=%rax,>zq3=%r12
mov  %rax,%r12

# qhasm: zq4 = t84
# asm 1: mov  <t84=int64#8,>zq4=int64#11
# asm 2: mov  <t84=%r10,>zq4=%r13
mov  %r10,%r13

# qhasm: zq0 += *(uint64 *) &crypto_scalarmult_curve25519_amd64_51_2P0
# asm 1: add  crypto_scalarmult_curve25519_amd64_51_2P0,<zq0=int64#3
# asm 2: add  crypto_scalarmult_curve25519_amd64_51_2P0,<zq0=%rdx
add  crypto_scalarmult_curve25519_amd64_51_2P0,%rdx

# qhasm: zq1 += *(uint64 *) &crypto_scalarmult_curve25519_amd64_51_2P1234
# asm 1: add  crypto_scalarmult_curve25519_amd64_51_2P1234,<zq1=int64#4
# asm 2: add  crypto_scalarmult_curve25519_amd64_51_2P1234,<zq1=%rcx
add  crypto_scalarmult_curve25519_amd64_51_2P1234,%rcx

# qhasm: zq2 += *(uint64 *) &crypto_scalarmult_curve25519_amd64_51_2P1234
# asm 1: add  crypto_scalarmult_curve25519_amd64_51_2P1234,<zq2=int64#9
# asm 2: add  crypto_scalarmult_curve25519_amd64_51_2P1234,<zq2=%r11
add  crypto_scalarmult_curve25519_amd64_51_2P1234,%r11

# qhasm: zq3 += *(uint64 *) &crypto_scalarmult_curve25519_amd64_51_2P1234
# asm 1: add  crypto_scalarmult_curve25519_amd64_51_2P1234,<zq3=int64#10
# asm 2: add  crypto_scalarmult_curve25519_amd64_51_2P1234,<zq3=%r12
add  crypto_scalarmult_curve25519_amd64_51_2P1234,%r12

# qhasm: zq4 += *(uint64 *) &crypto_scalarmult_curve25519_amd64_51_2P1234
# asm 1: add  crypto_scalarmult_curve25519_amd64_51_2P1234,<zq4=int64#11
# asm 2: add  crypto_scalarmult_curve25519_amd64_51_2P1234,<zq4=%r13
add  crypto_scalarmult_curve25519_amd64_51_2P1234,%r13

# qhasm: t80 += t90_stack
# asm 1: addq <t90_stack=stack64#13,<t80=int64#2
# asm 2: addq <t90_stack=96(%rsp),<t80=%rsi
addq 96(%rsp),%rsi

# qhasm: t81 += t91_stack
# asm 1: addq <t91_stack=stack64#14,<t81=int64#5
# asm 2: addq <t91_stack=104(%rsp),<t81=%r8
addq 104(%rsp),%r8

# qhasm: t82 += t92_stack
# asm 1: addq <t92_stack=stack64#15,<t82=int64#6
# asm 2: addq <t92_stack=112(%rsp),<t82=%r9
addq 112(%rsp),%r9

# qhasm: t83 += t93_stack
# asm 1: addq <t93_stack=stack64#16,<t83=int64#7
# asm 2: addq <t93_stack=120(%rsp),<t83=%rax
addq 120(%rsp),%rax

# qhasm: t84 += t94_stack
# asm 1: addq <t94_stack=stack64#17,<t84=int64#8
# asm 2: addq <t94_stack=128(%rsp),<t84=%r10
addq 128(%rsp),%r10

# qhasm: zq0 -= t90_stack
# asm 1: subq <t90_stack=stack64#13,<zq0=int64#3
# asm 2: subq <t90_stack=96(%rsp),<zq0=%rdx
subq 96(%rsp),%rdx

# qhasm: zq1 -= t91_stack
# asm 1: subq <t91_stack=stack64#14,<zq1=int64#4
# asm 2: subq <t91_stack=104(%rsp),<zq1=%rcx
subq 104(%rsp),%rcx

# qhasm: zq2 -= t92_stack
# asm 1: subq <t92_stack=stack64#15,<zq2=int64#9
# asm 2: subq <t92_stack=112(%rsp),<zq2=%r11
subq 112(%rsp),%r11

# qhasm: zq3 -= t93_stack
# asm 1: subq <t93_stack=stack64#16,<zq3=int64#10
# asm 2: subq <t93_stack=120(%rsp),<zq3=%r12
subq 120(%rsp),%r12

# qhasm: zq4 -= t94_stack
# asm 1: subq <t94_stack=stack64#17,<zq4=int64#11
# asm 2: subq <t94_stack=128(%rsp),<zq4=%r13
subq 128(%rsp),%r13

# qhasm: *(uint64 *)(workp + 120) = t80
# asm 1: movq   <t80=int64#2,120(<workp=int64#1)
# asm 2: movq   <t80=%rsi,120(<workp=%rdi)
movq   %rsi,120(%rdi)

# qhasm: *(uint64 *)(workp + 128) = t81
# asm 1: movq   <t81=int64#5,128(<workp=int64#1)
# asm 2: movq   <t81=%r8,128(<workp=%rdi)
movq   %r8,128(%rdi)

# qhasm: *(uint64 *)(workp + 136) = t82
# asm 1: movq   <t82=int64#6,136(<workp=int64#1)
# asm 2: movq   <t82=%r9,136(<workp=%rdi)
movq   %r9,136(%rdi)

# qhasm: *(uint64 *)(workp + 144) = t83
# asm 1: movq   <t83=int64#7,144(<workp=int64#1)
# asm 2: movq   <t83=%rax,144(<workp=%rdi)
movq   %rax,144(%rdi)

# qhasm: *(uint64 *)(workp + 152) = t84
# asm 1: movq   <t84=int64#8,152(<workp=int64#1)
# asm 2: movq   <t84=%r10,152(<workp=%rdi)
movq   %r10,152(%rdi)

# qhasm: *(uint64 *)(workp + 160) = zq0
# asm 1: movq   <zq0=int64#3,160(<workp=int64#1)
# asm 2: movq   <zq0=%rdx,160(<workp=%rdi)
movq   %rdx,160(%rdi)

# qhasm: *(uint64 *)(workp + 168) = zq1
# asm 1: movq   <zq1=int64#4,168(<workp=int64#1)
# asm 2: movq   <zq1=%rcx,168(<workp=%rdi)
movq   %rcx,168(%rdi)

# qhasm: *(uint64 *)(workp + 176) = zq2
# asm 1: movq   <zq2=int64#9,176(<workp=int64#1)
# asm 2: movq   <zq2=%r11,176(<workp=%rdi)
movq   %r11,176(%rdi)

# qhasm: *(uint64 *)(workp + 184) = zq3
# asm 1: movq   <zq3=int64#10,184(<workp=int64#1)
# asm 2: movq   <zq3=%r12,184(<workp=%rdi)
movq   %r12,184(%rdi)

# qhasm: *(uint64 *)(workp + 192) = zq4
# asm 1: movq   <zq4=int64#11,192(<workp=int64#1)
# asm 2: movq   <zq4=%r13,192(<workp=%rdi)
movq   %r13,192(%rdi)

# qhasm:   squarerax = *(uint64 *)(workp + 120)
# asm 1: movq   120(<workp=int64#1),>squarerax=int64#7
# asm 2: movq   120(<workp=%rdi),>squarerax=%rax
movq   120(%rdi),%rax

# qhasm:   (uint128) squarerdx squarerax = squarerax * *(uint64 *)(workp + 120)
# asm 1: mulq  120(<workp=int64#1)
# asm 2: mulq  120(<workp=%rdi)
mulq  120(%rdi)

# qhasm:   xq0 = squarerax
# asm 1: mov  <squarerax=int64#7,>xq0=int64#2
# asm 2: mov  <squarerax=%rax,>xq0=%rsi
mov  %rax,%rsi

# qhasm:   squarer01 = squarerdx
# asm 1: mov  <squarerdx=int64#3,>squarer01=int64#4
# asm 2: mov  <squarerdx=%rdx,>squarer01=%rcx
mov  %rdx,%rcx

# qhasm:   squarerax = *(uint64 *)(workp + 120)
# asm 1: movq   120(<workp=int64#1),>squarerax=int64#7
# asm 2: movq   120(<workp=%rdi),>squarerax=%rax
movq   120(%rdi),%rax

# qhasm:   squarerax <<= 1
# asm 1: shl  $1,<squarerax=int64#7
# asm 2: shl  $1,<squarerax=%rax
shl  $1,%rax

# qhasm:   (uint128) squarerdx squarerax = squarerax * *(uint64 *)(workp + 128)
# asm 1: mulq  128(<workp=int64#1)
# asm 2: mulq  128(<workp=%rdi)
mulq  128(%rdi)

# qhasm:   xq1 = squarerax
# asm 1: mov  <squarerax=int64#7,>xq1=int64#5
# asm 2: mov  <squarerax=%rax,>xq1=%r8
mov  %rax,%r8

# qhasm:   squarer11 = squarerdx
# asm 1: mov  <squarerdx=int64#3,>squarer11=int64#6
# asm 2: mov  <squarerdx=%rdx,>squarer11=%r9
mov  %rdx,%r9

# qhasm:   squarerax = *(uint64 *)(workp + 120)
# asm 1: movq   120(<workp=int64#1),>squarerax=int64#7
# asm 2: movq   120(<workp=%rdi),>squarerax=%rax
movq   120(%rdi),%rax

# qhasm:   squarerax <<= 1
# asm 1: shl  $1,<squarerax=int64#7
# asm 2: shl  $1,<squarerax=%rax
shl  $1,%rax

# qhasm:   (uint128) squarerdx squarerax = squarerax * *(uint64 *)(workp + 136)
# asm 1: mulq  136(<workp=int64#1)
# asm 2: mulq  136(<workp=%rdi)
mulq  136(%rdi)

# qhasm:   xq2 = squarerax
# asm 1: mov  <squarerax=int64#7,>xq2=int64#8
# asm 2: mov  <squarerax=%rax,>xq2=%r10
mov  %rax,%r10

# qhasm:   squarer21 = squarerdx
# asm 1: mov  <squarerdx=int64#3,>squarer21=int64#9
# asm 2: mov  <squarerdx=%rdx,>squarer21=%r11
mov  %rdx,%r11

# qhasm:   squarerax = *(uint64 *)(workp + 120)
# asm 1: movq   120(<workp=int64#1),>squarerax=int64#7
# asm 2: movq   120(<workp=%rdi),>squarerax=%rax
movq   120(%rdi),%rax

# qhasm:   squarerax <<= 1
# asm 1: shl  $1,<squarerax=int64#7
# asm 2: shl  $1,<squarerax=%rax
shl  $1,%rax

# qhasm:   (uint128) squarerdx squarerax = squarerax * *(uint64 *)(workp + 144)
# asm 1: mulq  144(<workp=int64#1)
# asm 2: mulq  144(<workp=%rdi)
mulq  144(%rdi)

# qhasm:   xq3 = squarerax
# asm 1: mov  <squarerax=int64#7,>xq3=int64#10
# asm 2: mov  <squarerax=%rax,>xq3=%r12
mov  %rax,%r12

# qhasm:   squarer31 = squarerdx
# asm 1: mov  <squarerdx=int64#3,>squarer31=int64#11
# asm 2: mov  <squarerdx=%rdx,>squarer31=%r13
mov  %rdx,%r13

# qhasm:   squarerax = *(uint64 *)(workp + 120)
# asm 1: movq   120(<workp=int64#1),>squarerax=int64#7
# asm 2: movq   120(<workp=%rdi),>squarerax=%rax
movq   120(%rdi),%rax

# qhasm:   squarerax <<= 1
# asm 1: shl  $1,<squarerax=int64#7
# asm 2: shl  $1,<squarerax=%rax
shl  $1,%rax

# qhasm:   (uint128) squarerdx squarerax = squarerax * *(uint64 *)(workp + 152)
# asm 1: mulq  152(<workp=int64#1)
# asm 2: mulq  152(<workp=%rdi)
mulq  152(%rdi)

# qhasm:   xq4 = squarerax
# asm 1: mov  <squarerax=int64#7,>xq4=int64#12
# asm 2: mov  <squarerax=%rax,>xq4=%r14
mov  %rax,%r14

# qhasm:   squarer41 = squarerdx
# asm 1: mov  <squarerdx=int64#3,>squarer41=int64#13
# asm 2: mov  <squarerdx=%rdx,>squarer41=%r15
mov  %rdx,%r15

# qhasm:   squarerax = *(uint64 *)(workp + 128)
# asm 1: movq   128(<workp=int64#1),>squarerax=int64#7
# asm 2: movq   128(<workp=%rdi),>squarerax=%rax
movq   128(%rdi),%rax

# qhasm:   (uint128) squarerdx squarerax = squarerax * *(uint64 *)(workp + 128)
# asm 1: mulq  128(<workp=int64#1)
# asm 2: mulq  128(<workp=%rdi)
mulq  128(%rdi)

# qhasm:   carry? xq2 += squarerax
# asm 1: add  <squarerax=int64#7,<xq2=int64#8
# asm 2: add  <squarerax=%rax,<xq2=%r10
add  %rax,%r10

# qhasm:   squarer21 += squarerdx + carry
# asm 1: adc <squarerdx=int64#3,<squarer21=int64#9
# asm 2: adc <squarerdx=%rdx,<squarer21=%r11
adc %rdx,%r11

# qhasm:   squarerax = *(uint64 *)(workp + 128)
# asm 1: movq   128(<workp=int64#1),>squarerax=int64#7
# asm 2: movq   128(<workp=%rdi),>squarerax=%rax
movq   128(%rdi),%rax

# qhasm:   squarerax <<= 1
# asm 1: shl  $1,<squarerax=int64#7
# asm 2: shl  $1,<squarerax=%rax
shl  $1,%rax

# qhasm:   (uint128) squarerdx squarerax = squarerax * *(uint64 *)(workp + 136)
# asm 1: mulq  136(<workp=int64#1)
# asm 2: mulq  136(<workp=%rdi)
mulq  136(%rdi)

# qhasm:   carry? xq3 += squarerax
# asm 1: add  <squarerax=int64#7,<xq3=int64#10
# asm 2: add  <squarerax=%rax,<xq3=%r12
add  %rax,%r12

# qhasm:   squarer31 += squarerdx + carry
# asm 1: adc <squarerdx=int64#3,<squarer31=int64#11
# asm 2: adc <squarerdx=%rdx,<squarer31=%r13
adc %rdx,%r13

# qhasm:   squarerax = *(uint64 *)(workp + 128)
# asm 1: movq   128(<workp=int64#1),>squarerax=int64#7
# asm 2: movq   128(<workp=%rdi),>squarerax=%rax
movq   128(%rdi),%rax

# qhasm:   squarerax <<= 1
# asm 1: shl  $1,<squarerax=int64#7
# asm 2: shl  $1,<squarerax=%rax
shl  $1,%rax

# qhasm:   (uint128) squarerdx squarerax = squarerax * *(uint64 *)(workp + 144)
# asm 1: mulq  144(<workp=int64#1)
# asm 2: mulq  144(<workp=%rdi)
mulq  144(%rdi)

# qhasm:   carry? xq4 += squarerax
# asm 1: add  <squarerax=int64#7,<xq4=int64#12
# asm 2: add  <squarerax=%rax,<xq4=%r14
add  %rax,%r14

# qhasm:   squarer41 += squarerdx + carry
# asm 1: adc <squarerdx=int64#3,<squarer41=int64#13
# asm 2: adc <squarerdx=%rdx,<squarer41=%r15
adc %rdx,%r15

# qhasm:   squarerax = *(uint64 *)(workp + 128)
# asm 1: movq   128(<workp=int64#1),>squarerax=int64#3
# asm 2: movq   128(<workp=%rdi),>squarerax=%rdx
movq   128(%rdi),%rdx

# qhasm:   squarerax *= 38
# asm 1: imulq  $38,<squarerax=int64#3,>squarerax=int64#7
# asm 2: imulq  $38,<squarerax=%rdx,>squarerax=%rax
imulq  $38,%rdx,%rax

# qhasm:   (uint128) squarerdx squarerax = squarerax * *(uint64 *)(workp + 152)
# asm 1: mulq  152(<workp=int64#1)
# asm 2: mulq  152(<workp=%rdi)
mulq  152(%rdi)

# qhasm:   carry? xq0 += squarerax
# asm 1: add  <squarerax=int64#7,<xq0=int64#2
# asm 2: add  <squarerax=%rax,<xq0=%rsi
add  %rax,%rsi

# qhasm:   squarer01 += squarerdx + carry
# asm 1: adc <squarerdx=int64#3,<squarer01=int64#4
# asm 2: adc <squarerdx=%rdx,<squarer01=%rcx
adc %rdx,%rcx

# qhasm:   squarerax = *(uint64 *)(workp + 136)
# asm 1: movq   136(<workp=int64#1),>squarerax=int64#7
# asm 2: movq   136(<workp=%rdi),>squarerax=%rax
movq   136(%rdi),%rax

# qhasm:   (uint128) squarerdx squarerax = squarerax * *(uint64 *)(workp + 136)
# asm 1: mulq  136(<workp=int64#1)
# asm 2: mulq  136(<workp=%rdi)
mulq  136(%rdi)

# qhasm:   carry? xq4 += squarerax
# asm 1: add  <squarerax=int64#7,<xq4=int64#12
# asm 2: add  <squarerax=%rax,<xq4=%r14
add  %rax,%r14

# qhasm:   squarer41 += squarerdx + carry
# asm 1: adc <squarerdx=int64#3,<squarer41=int64#13
# asm 2: adc <squarerdx=%rdx,<squarer41=%r15
adc %rdx,%r15

# qhasm:   squarerax = *(uint64 *)(workp + 136)
# asm 1: movq   136(<workp=int64#1),>squarerax=int64#3
# asm 2: movq   136(<workp=%rdi),>squarerax=%rdx
movq   136(%rdi),%rdx

# qhasm:   squarerax *= 38
# asm 1: imulq  $38,<squarerax=int64#3,>squarerax=int64#7
# asm 2: imulq  $38,<squarerax=%rdx,>squarerax=%rax
imulq  $38,%rdx,%rax

# qhasm:   (uint128) squarerdx squarerax = squarerax * *(uint64 *)(workp + 144)
# asm 1: mulq  144(<workp=int64#1)
# asm 2: mulq  144(<workp=%rdi)
mulq  144(%rdi)

# qhasm:   carry? xq0 += squarerax
# asm 1: add  <squarerax=int64#7,<xq0=int64#2
# asm 2: add  <squarerax=%rax,<xq0=%rsi
add  %rax,%rsi

# qhasm:   squarer01 += squarerdx + carry
# asm 1: adc <squarerdx=int64#3,<squarer01=int64#4
# asm 2: adc <squarerdx=%rdx,<squarer01=%rcx
adc %rdx,%rcx

# qhasm:   squarerax = *(uint64 *)(workp + 136)
# asm 1: movq   136(<workp=int64#1),>squarerax=int64#3
# asm 2: movq   136(<workp=%rdi),>squarerax=%rdx
movq   136(%rdi),%rdx

# qhasm:   squarerax *= 38
# asm 1: imulq  $38,<squarerax=int64#3,>squarerax=int64#7
# asm 2: imulq  $38,<squarerax=%rdx,>squarerax=%rax
imulq  $38,%rdx,%rax

# qhasm:   (uint128) squarerdx squarerax = squarerax * *(uint64 *)(workp + 152)
# asm 1: mulq  152(<workp=int64#1)
# asm 2: mulq  152(<workp=%rdi)
mulq  152(%rdi)

# qhasm:   carry? xq1 += squarerax
# asm 1: add  <squarerax=int64#7,<xq1=int64#5
# asm 2: add  <squarerax=%rax,<xq1=%r8
add  %rax,%r8

# qhasm:   squarer11 += squarerdx + carry
# asm 1: adc <squarerdx=int64#3,<squarer11=int64#6
# asm 2: adc <squarerdx=%rdx,<squarer11=%r9
adc %rdx,%r9

# qhasm:   squarerax = *(uint64 *)(workp + 144)
# asm 1: movq   144(<workp=int64#1),>squarerax=int64#3
# asm 2: movq   144(<workp=%rdi),>squarerax=%rdx
movq   144(%rdi),%rdx

# qhasm:   squarerax *= 19
# asm 1: imulq  $19,<squarerax=int64#3,>squarerax=int64#7
# asm 2: imulq  $19,<squarerax=%rdx,>squarerax=%rax
imulq  $19,%rdx,%rax

# qhasm:   (uint128) squarerdx squarerax = squarerax * *(uint64 *)(workp + 144)
# asm 1: mulq  144(<workp=int64#1)
# asm 2: mulq  144(<workp=%rdi)
mulq  144(%rdi)

# qhasm:   carry? xq1 += squarerax
# asm 1: add  <squarerax=int64#7,<xq1=int64#5
# asm 2: add  <squarerax=%rax,<xq1=%r8
add  %rax,%r8

# qhasm:   squarer11 += squarerdx + carry
# asm 1: adc <squarerdx=int64#3,<squarer11=int64#6
# asm 2: adc <squarerdx=%rdx,<squarer11=%r9
adc %rdx,%r9

# qhasm:   squarerax = *(uint64 *)(workp + 144)
# asm 1: movq   144(<workp=int64#1),>squarerax=int64#3
# asm 2: movq   144(<workp=%rdi),>squarerax=%rdx
movq   144(%rdi),%rdx

# qhasm:   squarerax *= 38
# asm 1: imulq  $38,<squarerax=int64#3,>squarerax=int64#7
# asm 2: imulq  $38,<squarerax=%rdx,>squarerax=%rax
imulq  $38,%rdx,%rax

# qhasm:   (uint128) squarerdx squarerax = squarerax * *(uint64 *)(workp + 152)
# asm 1: mulq  152(<workp=int64#1)
# asm 2: mulq  152(<workp=%rdi)
mulq  152(%rdi)

# qhasm:   carry? xq2 += squarerax
# asm 1: add  <squarerax=int64#7,<xq2=int64#8
# asm 2: add  <squarerax=%rax,<xq2=%r10
add  %rax,%r10

# qhasm:   squarer21 += squarerdx + carry
# asm 1: adc <squarerdx=int64#3,<squarer21=int64#9
# asm 2: adc <squarerdx=%rdx,<squarer21=%r11
adc %rdx,%r11

# qhasm:   squarerax = *(uint64 *)(workp + 152)
# asm 1: movq   152(<workp=int64#1),>squarerax=int64#3
# asm 2: movq   152(<workp=%rdi),>squarerax=%rdx
movq   152(%rdi),%rdx

# qhasm:   squarerax *= 19
# asm 1: imulq  $19,<squarerax=int64#3,>squarerax=int64#7
# asm 2: imulq  $19,<squarerax=%rdx,>squarerax=%rax
imulq  $19,%rdx,%rax

# qhasm:   (uint128) squarerdx squarerax = squarerax * *(uint64 *)(workp + 152)
# asm 1: mulq  152(<workp=int64#1)
# asm 2: mulq  152(<workp=%rdi)
mulq  152(%rdi)

# qhasm:   carry? xq3 += squarerax
# asm 1: add  <squarerax=int64#7,<xq3=int64#10
# asm 2: add  <squarerax=%rax,<xq3=%r12
add  %rax,%r12

# qhasm:   squarer31 += squarerdx + carry
# asm 1: adc <squarerdx=int64#3,<squarer31=int64#11
# asm 2: adc <squarerdx=%rdx,<squarer31=%r13
adc %rdx,%r13

# qhasm:   squareredmask = *(uint64 *) &crypto_scalarmult_curve25519_amd64_51_REDMASK51
# asm 1: movq crypto_scalarmult_curve25519_amd64_51_REDMASK51,>squareredmask=int64#3
# asm 2: movq crypto_scalarmult_curve25519_amd64_51_REDMASK51,>squareredmask=%rdx
movq crypto_scalarmult_curve25519_amd64_51_REDMASK51,%rdx

# qhasm:   squarer01 = (squarer01.xq0) << 13
# asm 1: shld $13,<xq0=int64#2,<squarer01=int64#4
# asm 2: shld $13,<xq0=%rsi,<squarer01=%rcx
shld $13,%rsi,%rcx

# qhasm:   xq0 &= squareredmask
# asm 1: and  <squareredmask=int64#3,<xq0=int64#2
# asm 2: and  <squareredmask=%rdx,<xq0=%rsi
and  %rdx,%rsi

# qhasm:   squarer11 = (squarer11.xq1) << 13
# asm 1: shld $13,<xq1=int64#5,<squarer11=int64#6
# asm 2: shld $13,<xq1=%r8,<squarer11=%r9
shld $13,%r8,%r9

# qhasm:   xq1 &= squareredmask
# asm 1: and  <squareredmask=int64#3,<xq1=int64#5
# asm 2: and  <squareredmask=%rdx,<xq1=%r8
and  %rdx,%r8

# qhasm:   xq1 += squarer01
# asm 1: add  <squarer01=int64#4,<xq1=int64#5
# asm 2: add  <squarer01=%rcx,<xq1=%r8
add  %rcx,%r8

# qhasm:   squarer21 = (squarer21.xq2) << 13
# asm 1: shld $13,<xq2=int64#8,<squarer21=int64#9
# asm 2: shld $13,<xq2=%r10,<squarer21=%r11
shld $13,%r10,%r11

# qhasm:   xq2 &= squareredmask
# asm 1: and  <squareredmask=int64#3,<xq2=int64#8
# asm 2: and  <squareredmask=%rdx,<xq2=%r10
and  %rdx,%r10

# qhasm:   xq2 += squarer11
# asm 1: add  <squarer11=int64#6,<xq2=int64#8
# asm 2: add  <squarer11=%r9,<xq2=%r10
add  %r9,%r10

# qhasm:   squarer31 = (squarer31.xq3) << 13
# asm 1: shld $13,<xq3=int64#10,<squarer31=int64#11
# asm 2: shld $13,<xq3=%r12,<squarer31=%r13
shld $13,%r12,%r13

# qhasm:   xq3 &= squareredmask
# asm 1: and  <squareredmask=int64#3,<xq3=int64#10
# asm 2: and  <squareredmask=%rdx,<xq3=%r12
and  %rdx,%r12

# qhasm:   xq3 += squarer21
# asm 1: add  <squarer21=int64#9,<xq3=int64#10
# asm 2: add  <squarer21=%r11,<xq3=%r12
add  %r11,%r12

# qhasm:   squarer41 = (squarer41.xq4) << 13
# asm 1: shld $13,<xq4=int64#12,<squarer41=int64#13
# asm 2: shld $13,<xq4=%r14,<squarer41=%r15
shld $13,%r14,%r15

# qhasm:   xq4 &= squareredmask
# asm 1: and  <squareredmask=int64#3,<xq4=int64#12
# asm 2: and  <squareredmask=%rdx,<xq4=%r14
and  %rdx,%r14

# qhasm:   xq4 += squarer31
# asm 1: add  <squarer31=int64#11,<xq4=int64#12
# asm 2: add  <squarer31=%r13,<xq4=%r14
add  %r13,%r14

# qhasm:   squarer41 = squarer41 * 19
# asm 1: imulq  $19,<squarer41=int64#13,>squarer41=int64#4
# asm 2: imulq  $19,<squarer41=%r15,>squarer41=%rcx
imulq  $19,%r15,%rcx

# qhasm:   xq0 += squarer41
# asm 1: add  <squarer41=int64#4,<xq0=int64#2
# asm 2: add  <squarer41=%rcx,<xq0=%rsi
add  %rcx,%rsi

# qhasm:   squaret = xq0
# asm 1: mov  <xq0=int64#2,>squaret=int64#4
# asm 2: mov  <xq0=%rsi,>squaret=%rcx
mov  %rsi,%rcx

# qhasm:   (uint64) squaret >>= 51
# asm 1: shr  $51,<squaret=int64#4
# asm 2: shr  $51,<squaret=%rcx
shr  $51,%rcx

# qhasm:   squaret += xq1
# asm 1: add  <xq1=int64#5,<squaret=int64#4
# asm 2: add  <xq1=%r8,<squaret=%rcx
add  %r8,%rcx

# qhasm:   xq0 &= squareredmask
# asm 1: and  <squareredmask=int64#3,<xq0=int64#2
# asm 2: and  <squareredmask=%rdx,<xq0=%rsi
and  %rdx,%rsi

# qhasm:   xq1 = squaret
# asm 1: mov  <squaret=int64#4,>xq1=int64#5
# asm 2: mov  <squaret=%rcx,>xq1=%r8
mov  %rcx,%r8

# qhasm:   (uint64) squaret >>= 51
# asm 1: shr  $51,<squaret=int64#4
# asm 2: shr  $51,<squaret=%rcx
shr  $51,%rcx

# qhasm:   squaret += xq2
# asm 1: add  <xq2=int64#8,<squaret=int64#4
# asm 2: add  <xq2=%r10,<squaret=%rcx
add  %r10,%rcx

# qhasm:   xq1 &= squareredmask
# asm 1: and  <squareredmask=int64#3,<xq1=int64#5
# asm 2: and  <squareredmask=%rdx,<xq1=%r8
and  %rdx,%r8

# qhasm:   xq2 = squaret
# asm 1: mov  <squaret=int64#4,>xq2=int64#6
# asm 2: mov  <squaret=%rcx,>xq2=%r9
mov  %rcx,%r9

# qhasm:   (uint64) squaret >>= 51
# asm 1: shr  $51,<squaret=int64#4
# asm 2: shr  $51,<squaret=%rcx
shr  $51,%rcx

# qhasm:   squaret += xq3
# asm 1: add  <xq3=int64#10,<squaret=int64#4
# asm 2: add  <xq3=%r12,<squaret=%rcx
add  %r12,%rcx

# qhasm:   xq2 &= squareredmask
# asm 1: and  <squareredmask=int64#3,<xq2=int64#6
# asm 2: and  <squareredmask=%rdx,<xq2=%r9
and  %rdx,%r9

# qhasm:   xq3 = squaret
# asm 1: mov  <squaret=int64#4,>xq3=int64#7
# asm 2: mov  <squaret=%rcx,>xq3=%rax
mov  %rcx,%rax

# qhasm:   (uint64) squaret >>= 51
# asm 1: shr  $51,<squaret=int64#4
# asm 2: shr  $51,<squaret=%rcx
shr  $51,%rcx

# qhasm:   squaret += xq4
# asm 1: add  <xq4=int64#12,<squaret=int64#4
# asm 2: add  <xq4=%r14,<squaret=%rcx
add  %r14,%rcx

# qhasm:   xq3 &= squareredmask
# asm 1: and  <squareredmask=int64#3,<xq3=int64#7
# asm 2: and  <squareredmask=%rdx,<xq3=%rax
and  %rdx,%rax

# qhasm:   xq4 = squaret
# asm 1: mov  <squaret=int64#4,>xq4=int64#8
# asm 2: mov  <squaret=%rcx,>xq4=%r10
mov  %rcx,%r10

# qhasm:   (uint64) squaret >>= 51
# asm 1: shr  $51,<squaret=int64#4
# asm 2: shr  $51,<squaret=%rcx
shr  $51,%rcx

# qhasm:   squaret *= 19
# asm 1: imulq  $19,<squaret=int64#4,>squaret=int64#4
# asm 2: imulq  $19,<squaret=%rcx,>squaret=%rcx
imulq  $19,%rcx,%rcx

# qhasm:   xq0 += squaret
# asm 1: add  <squaret=int64#4,<xq0=int64#2
# asm 2: add  <squaret=%rcx,<xq0=%rsi
add  %rcx,%rsi

# qhasm:   xq4 &= squareredmask
# asm 1: and  <squareredmask=int64#3,<xq4=int64#8
# asm 2: and  <squareredmask=%rdx,<xq4=%r10
and  %rdx,%r10

# qhasm: *(uint64 *)(workp + 120) = xq0
# asm 1: movq   <xq0=int64#2,120(<workp=int64#1)
# asm 2: movq   <xq0=%rsi,120(<workp=%rdi)
movq   %rsi,120(%rdi)

# qhasm: *(uint64 *)(workp + 128) = xq1
# asm 1: movq   <xq1=int64#5,128(<workp=int64#1)
# asm 2: movq   <xq1=%r8,128(<workp=%rdi)
movq   %r8,128(%rdi)

# qhasm: *(uint64 *)(workp + 136) = xq2
# asm 1: movq   <xq2=int64#6,136(<workp=int64#1)
# asm 2: movq   <xq2=%r9,136(<workp=%rdi)
movq   %r9,136(%rdi)

# qhasm: *(uint64 *)(workp + 144) = xq3
# asm 1: movq   <xq3=int64#7,144(<workp=int64#1)
# asm 2: movq   <xq3=%rax,144(<workp=%rdi)
movq   %rax,144(%rdi)

# qhasm: *(uint64 *)(workp + 152) = xq4
# asm 1: movq   <xq4=int64#8,152(<workp=int64#1)
# asm 2: movq   <xq4=%r10,152(<workp=%rdi)
movq   %r10,152(%rdi)

# qhasm:   squarerax = *(uint64 *)(workp + 160)
# asm 1: movq   160(<workp=int64#1),>squarerax=int64#7
# asm 2: movq   160(<workp=%rdi),>squarerax=%rax
movq   160(%rdi),%rax

# qhasm:   (uint128) squarerdx squarerax = squarerax * *(uint64 *)(workp + 160)
# asm 1: mulq  160(<workp=int64#1)
# asm 2: mulq  160(<workp=%rdi)
mulq  160(%rdi)

# qhasm:   zq0 = squarerax
# asm 1: mov  <squarerax=int64#7,>zq0=int64#2
# asm 2: mov  <squarerax=%rax,>zq0=%rsi
mov  %rax,%rsi

# qhasm:   squarer01 = squarerdx
# asm 1: mov  <squarerdx=int64#3,>squarer01=int64#4
# asm 2: mov  <squarerdx=%rdx,>squarer01=%rcx
mov  %rdx,%rcx

# qhasm:   squarerax = *(uint64 *)(workp + 160)
# asm 1: movq   160(<workp=int64#1),>squarerax=int64#7
# asm 2: movq   160(<workp=%rdi),>squarerax=%rax
movq   160(%rdi),%rax

# qhasm:   squarerax <<= 1
# asm 1: shl  $1,<squarerax=int64#7
# asm 2: shl  $1,<squarerax=%rax
shl  $1,%rax

# qhasm:   (uint128) squarerdx squarerax = squarerax * *(uint64 *)(workp + 168)
# asm 1: mulq  168(<workp=int64#1)
# asm 2: mulq  168(<workp=%rdi)
mulq  168(%rdi)

# qhasm:   zq1 = squarerax
# asm 1: mov  <squarerax=int64#7,>zq1=int64#5
# asm 2: mov  <squarerax=%rax,>zq1=%r8
mov  %rax,%r8

# qhasm:   squarer11 = squarerdx
# asm 1: mov  <squarerdx=int64#3,>squarer11=int64#6
# asm 2: mov  <squarerdx=%rdx,>squarer11=%r9
mov  %rdx,%r9

# qhasm:   squarerax = *(uint64 *)(workp + 160)
# asm 1: movq   160(<workp=int64#1),>squarerax=int64#7
# asm 2: movq   160(<workp=%rdi),>squarerax=%rax
movq   160(%rdi),%rax

# qhasm:   squarerax <<= 1
# asm 1: shl  $1,<squarerax=int64#7
# asm 2: shl  $1,<squarerax=%rax
shl  $1,%rax

# qhasm:   (uint128) squarerdx squarerax = squarerax * *(uint64 *)(workp + 176)
# asm 1: mulq  176(<workp=int64#1)
# asm 2: mulq  176(<workp=%rdi)
mulq  176(%rdi)

# qhasm:   zq2 = squarerax
# asm 1: mov  <squarerax=int64#7,>zq2=int64#8
# asm 2: mov  <squarerax=%rax,>zq2=%r10
mov  %rax,%r10

# qhasm:   squarer21 = squarerdx
# asm 1: mov  <squarerdx=int64#3,>squarer21=int64#9
# asm 2: mov  <squarerdx=%rdx,>squarer21=%r11
mov  %rdx,%r11

# qhasm:   squarerax = *(uint64 *)(workp + 160)
# asm 1: movq   160(<workp=int64#1),>squarerax=int64#7
# asm 2: movq   160(<workp=%rdi),>squarerax=%rax
movq   160(%rdi),%rax

# qhasm:   squarerax <<= 1
# asm 1: shl  $1,<squarerax=int64#7
# asm 2: shl  $1,<squarerax=%rax
shl  $1,%rax

# qhasm:   (uint128) squarerdx squarerax = squarerax * *(uint64 *)(workp + 184)
# asm 1: mulq  184(<workp=int64#1)
# asm 2: mulq  184(<workp=%rdi)
mulq  184(%rdi)

# qhasm:   zq3 = squarerax
# asm 1: mov  <squarerax=int64#7,>zq3=int64#10
# asm 2: mov  <squarerax=%rax,>zq3=%r12
mov  %rax,%r12

# qhasm:   squarer31 = squarerdx
# asm 1: mov  <squarerdx=int64#3,>squarer31=int64#11
# asm 2: mov  <squarerdx=%rdx,>squarer31=%r13
mov  %rdx,%r13

# qhasm:   squarerax = *(uint64 *)(workp + 160)
# asm 1: movq   160(<workp=int64#1),>squarerax=int64#7
# asm 2: movq   160(<workp=%rdi),>squarerax=%rax
movq   160(%rdi),%rax

# qhasm:   squarerax <<= 1
# asm 1: shl  $1,<squarerax=int64#7
# asm 2: shl  $1,<squarerax=%rax
shl  $1,%rax

# qhasm:   (uint128) squarerdx squarerax = squarerax * *(uint64 *)(workp + 192)
# asm 1: mulq  192(<workp=int64#1)
# asm 2: mulq  192(<workp=%rdi)
mulq  192(%rdi)

# qhasm:   zq4 = squarerax
# asm 1: mov  <squarerax=int64#7,>zq4=int64#12
# asm 2: mov  <squarerax=%rax,>zq4=%r14
mov  %rax,%r14

# qhasm:   squarer41 = squarerdx
# asm 1: mov  <squarerdx=int64#3,>squarer41=int64#13
# asm 2: mov  <squarerdx=%rdx,>squarer41=%r15
mov  %rdx,%r15

# qhasm:   squarerax = *(uint64 *)(workp + 168)
# asm 1: movq   168(<workp=int64#1),>squarerax=int64#7
# asm 2: movq   168(<workp=%rdi),>squarerax=%rax
movq   168(%rdi),%rax

# qhasm:   (uint128) squarerdx squarerax = squarerax * *(uint64 *)(workp + 168)
# asm 1: mulq  168(<workp=int64#1)
# asm 2: mulq  168(<workp=%rdi)
mulq  168(%rdi)

# qhasm:   carry? zq2 += squarerax
# asm 1: add  <squarerax=int64#7,<zq2=int64#8
# asm 2: add  <squarerax=%rax,<zq2=%r10
add  %rax,%r10

# qhasm:   squarer21 += squarerdx + carry
# asm 1: adc <squarerdx=int64#3,<squarer21=int64#9
# asm 2: adc <squarerdx=%rdx,<squarer21=%r11
adc %rdx,%r11

# qhasm:   squarerax = *(uint64 *)(workp + 168)
# asm 1: movq   168(<workp=int64#1),>squarerax=int64#7
# asm 2: movq   168(<workp=%rdi),>squarerax=%rax
movq   168(%rdi),%rax

# qhasm:   squarerax <<= 1
# asm 1: shl  $1,<squarerax=int64#7
# asm 2: shl  $1,<squarerax=%rax
shl  $1,%rax

# qhasm:   (uint128) squarerdx squarerax = squarerax * *(uint64 *)(workp + 176)
# asm 1: mulq  176(<workp=int64#1)
# asm 2: mulq  176(<workp=%rdi)
mulq  176(%rdi)

# qhasm:   carry? zq3 += squarerax
# asm 1: add  <squarerax=int64#7,<zq3=int64#10
# asm 2: add  <squarerax=%rax,<zq3=%r12
add  %rax,%r12

# qhasm:   squarer31 += squarerdx + carry
# asm 1: adc <squarerdx=int64#3,<squarer31=int64#11
# asm 2: adc <squarerdx=%rdx,<squarer31=%r13
adc %rdx,%r13

# qhasm:   squarerax = *(uint64 *)(workp + 168)
# asm 1: movq   168(<workp=int64#1),>squarerax=int64#7
# asm 2: movq   168(<workp=%rdi),>squarerax=%rax
movq   168(%rdi),%rax

# qhasm:   squarerax <<= 1
# asm 1: shl  $1,<squarerax=int64#7
# asm 2: shl  $1,<squarerax=%rax
shl  $1,%rax

# qhasm:   (uint128) squarerdx squarerax = squarerax * *(uint64 *)(workp + 184)
# asm 1: mulq  184(<workp=int64#1)
# asm 2: mulq  184(<workp=%rdi)
mulq  184(%rdi)

# qhasm:   carry? zq4 += squarerax
# asm 1: add  <squarerax=int64#7,<zq4=int64#12
# asm 2: add  <squarerax=%rax,<zq4=%r14
add  %rax,%r14

# qhasm:   squarer41 += squarerdx + carry
# asm 1: adc <squarerdx=int64#3,<squarer41=int64#13
# asm 2: adc <squarerdx=%rdx,<squarer41=%r15
adc %rdx,%r15

# qhasm:   squarerax = *(uint64 *)(workp + 168)
# asm 1: movq   168(<workp=int64#1),>squarerax=int64#3
# asm 2: movq   168(<workp=%rdi),>squarerax=%rdx
movq   168(%rdi),%rdx

# qhasm:   squarerax *= 38
# asm 1: imulq  $38,<squarerax=int64#3,>squarerax=int64#7
# asm 2: imulq  $38,<squarerax=%rdx,>squarerax=%rax
imulq  $38,%rdx,%rax

# qhasm:   (uint128) squarerdx squarerax = squarerax * *(uint64 *)(workp + 192)
# asm 1: mulq  192(<workp=int64#1)
# asm 2: mulq  192(<workp=%rdi)
mulq  192(%rdi)

# qhasm:   carry? zq0 += squarerax
# asm 1: add  <squarerax=int64#7,<zq0=int64#2
# asm 2: add  <squarerax=%rax,<zq0=%rsi
add  %rax,%rsi

# qhasm:   squarer01 += squarerdx + carry
# asm 1: adc <squarerdx=int64#3,<squarer01=int64#4
# asm 2: adc <squarerdx=%rdx,<squarer01=%rcx
adc %rdx,%rcx

# qhasm:   squarerax = *(uint64 *)(workp + 176)
# asm 1: movq   176(<workp=int64#1),>squarerax=int64#7
# asm 2: movq   176(<workp=%rdi),>squarerax=%rax
movq   176(%rdi),%rax

# qhasm:   (uint128) squarerdx squarerax = squarerax * *(uint64 *)(workp + 176)
# asm 1: mulq  176(<workp=int64#1)
# asm 2: mulq  176(<workp=%rdi)
mulq  176(%rdi)

# qhasm:   carry? zq4 += squarerax
# asm 1: add  <squarerax=int64#7,<zq4=int64#12
# asm 2: add  <squarerax=%rax,<zq4=%r14
add  %rax,%r14

# qhasm:   squarer41 += squarerdx + carry
# asm 1: adc <squarerdx=int64#3,<squarer41=int64#13
# asm 2: adc <squarerdx=%rdx,<squarer41=%r15
adc %rdx,%r15

# qhasm:   squarerax = *(uint64 *)(workp + 176)
# asm 1: movq   176(<workp=int64#1),>squarerax=int64#3
# asm 2: movq   176(<workp=%rdi),>squarerax=%rdx
movq   176(%rdi),%rdx

# qhasm:   squarerax *= 38
# asm 1: imulq  $38,<squarerax=int64#3,>squarerax=int64#7
# asm 2: imulq  $38,<squarerax=%rdx,>squarerax=%rax
imulq  $38,%rdx,%rax

# qhasm:   (uint128) squarerdx squarerax = squarerax * *(uint64 *)(workp + 184)
# asm 1: mulq  184(<workp=int64#1)
# asm 2: mulq  184(<workp=%rdi)
mulq  184(%rdi)

# qhasm:   carry? zq0 += squarerax
# asm 1: add  <squarerax=int64#7,<zq0=int64#2
# asm 2: add  <squarerax=%rax,<zq0=%rsi
add  %rax,%rsi

# qhasm:   squarer01 += squarerdx + carry
# asm 1: adc <squarerdx=int64#3,<squarer01=int64#4
# asm 2: adc <squarerdx=%rdx,<squarer01=%rcx
adc %rdx,%rcx

# qhasm:   squarerax = *(uint64 *)(workp + 176)
# asm 1: movq   176(<workp=int64#1),>squarerax=int64#3
# asm 2: movq   176(<workp=%rdi),>squarerax=%rdx
movq   176(%rdi),%rdx

# qhasm:   squarerax *= 38
# asm 1: imulq  $38,<squarerax=int64#3,>squarerax=int64#7
# asm 2: imulq  $38,<squarerax=%rdx,>squarerax=%rax
imulq  $38,%rdx,%rax

# qhasm:   (uint128) squarerdx squarerax = squarerax * *(uint64 *)(workp + 192)
# asm 1: mulq  192(<workp=int64#1)
# asm 2: mulq  192(<workp=%rdi)
mulq  192(%rdi)

# qhasm:   carry? zq1 += squarerax
# asm 1: add  <squarerax=int64#7,<zq1=int64#5
# asm 2: add  <squarerax=%rax,<zq1=%r8
add  %rax,%r8

# qhasm:   squarer11 += squarerdx + carry
# asm 1: adc <squarerdx=int64#3,<squarer11=int64#6
# asm 2: adc <squarerdx=%rdx,<squarer11=%r9
adc %rdx,%r9

# qhasm:   squarerax = *(uint64 *)(workp + 184)
# asm 1: movq   184(<workp=int64#1),>squarerax=int64#3
# asm 2: movq   184(<workp=%rdi),>squarerax=%rdx
movq   184(%rdi),%rdx

# qhasm:   squarerax *= 19
# asm 1: imulq  $19,<squarerax=int64#3,>squarerax=int64#7
# asm 2: imulq  $19,<squarerax=%rdx,>squarerax=%rax
imulq  $19,%rdx,%rax

# qhasm:   (uint128) squarerdx squarerax = squarerax * *(uint64 *)(workp + 184)
# asm 1: mulq  184(<workp=int64#1)
# asm 2: mulq  184(<workp=%rdi)
mulq  184(%rdi)

# qhasm:   carry? zq1 += squarerax
# asm 1: add  <squarerax=int64#7,<zq1=int64#5
# asm 2: add  <squarerax=%rax,<zq1=%r8
add  %rax,%r8

# qhasm:   squarer11 += squarerdx + carry
# asm 1: adc <squarerdx=int64#3,<squarer11=int64#6
# asm 2: adc <squarerdx=%rdx,<squarer11=%r9
adc %rdx,%r9

# qhasm:   squarerax = *(uint64 *)(workp + 184)
# asm 1: movq   184(<workp=int64#1),>squarerax=int64#3
# asm 2: movq   184(<workp=%rdi),>squarerax=%rdx
movq   184(%rdi),%rdx

# qhasm:   squarerax *= 38
# asm 1: imulq  $38,<squarerax=int64#3,>squarerax=int64#7
# asm 2: imulq  $38,<squarerax=%rdx,>squarerax=%rax
imulq  $38,%rdx,%rax

# qhasm:   (uint128) squarerdx squarerax = squarerax * *(uint64 *)(workp + 192)
# asm 1: mulq  192(<workp=int64#1)
# asm 2: mulq  192(<workp=%rdi)
mulq  192(%rdi)

# qhasm:   carry? zq2 += squarerax
# asm 1: add  <squarerax=int64#7,<zq2=int64#8
# asm 2: add  <squarerax=%rax,<zq2=%r10
add  %rax,%r10

# qhasm:   squarer21 += squarerdx + carry
# asm 1: adc <squarerdx=int64#3,<squarer21=int64#9
# asm 2: adc <squarerdx=%rdx,<squarer21=%r11
adc %rdx,%r11

# qhasm:   squarerax = *(uint64 *)(workp + 192)
# asm 1: movq   192(<workp=int64#1),>squarerax=int64#3
# asm 2: movq   192(<workp=%rdi),>squarerax=%rdx
movq   192(%rdi),%rdx

# qhasm:   squarerax *= 19
# asm 1: imulq  $19,<squarerax=int64#3,>squarerax=int64#7
# asm 2: imulq  $19,<squarerax=%rdx,>squarerax=%rax
imulq  $19,%rdx,%rax

# qhasm:   (uint128) squarerdx squarerax = squarerax * *(uint64 *)(workp + 192)
# asm 1: mulq  192(<workp=int64#1)
# asm 2: mulq  192(<workp=%rdi)
mulq  192(%rdi)

# qhasm:   carry? zq3 += squarerax
# asm 1: add  <squarerax=int64#7,<zq3=int64#10
# asm 2: add  <squarerax=%rax,<zq3=%r12
add  %rax,%r12

# qhasm:   squarer31 += squarerdx + carry
# asm 1: adc <squarerdx=int64#3,<squarer31=int64#11
# asm 2: adc <squarerdx=%rdx,<squarer31=%r13
adc %rdx,%r13

# qhasm:   squareredmask = *(uint64 *) &crypto_scalarmult_curve25519_amd64_51_REDMASK51
# asm 1: movq crypto_scalarmult_curve25519_amd64_51_REDMASK51,>squareredmask=int64#3
# asm 2: movq crypto_scalarmult_curve25519_amd64_51_REDMASK51,>squareredmask=%rdx
movq crypto_scalarmult_curve25519_amd64_51_REDMASK51,%rdx

# qhasm:   squarer01 = (squarer01.zq0) << 13
# asm 1: shld $13,<zq0=int64#2,<squarer01=int64#4
# asm 2: shld $13,<zq0=%rsi,<squarer01=%rcx
shld $13,%rsi,%rcx

# qhasm:   zq0 &= squareredmask
# asm 1: and  <squareredmask=int64#3,<zq0=int64#2
# asm 2: and  <squareredmask=%rdx,<zq0=%rsi
and  %rdx,%rsi

# qhasm:   squarer11 = (squarer11.zq1) << 13
# asm 1: shld $13,<zq1=int64#5,<squarer11=int64#6
# asm 2: shld $13,<zq1=%r8,<squarer11=%r9
shld $13,%r8,%r9

# qhasm:   zq1 &= squareredmask
# asm 1: and  <squareredmask=int64#3,<zq1=int64#5
# asm 2: and  <squareredmask=%rdx,<zq1=%r8
and  %rdx,%r8

# qhasm:   zq1 += squarer01
# asm 1: add  <squarer01=int64#4,<zq1=int64#5
# asm 2: add  <squarer01=%rcx,<zq1=%r8
add  %rcx,%r8

# qhasm:   squarer21 = (squarer21.zq2) << 13
# asm 1: shld $13,<zq2=int64#8,<squarer21=int64#9
# asm 2: shld $13,<zq2=%r10,<squarer21=%r11
shld $13,%r10,%r11

# qhasm:   zq2 &= squareredmask
# asm 1: and  <squareredmask=int64#3,<zq2=int64#8
# asm 2: and  <squareredmask=%rdx,<zq2=%r10
and  %rdx,%r10

# qhasm:   zq2 += squarer11
# asm 1: add  <squarer11=int64#6,<zq2=int64#8
# asm 2: add  <squarer11=%r9,<zq2=%r10
add  %r9,%r10

# qhasm:   squarer31 = (squarer31.zq3) << 13
# asm 1: shld $13,<zq3=int64#10,<squarer31=int64#11
# asm 2: shld $13,<zq3=%r12,<squarer31=%r13
shld $13,%r12,%r13

# qhasm:   zq3 &= squareredmask
# asm 1: and  <squareredmask=int64#3,<zq3=int64#10
# asm 2: and  <squareredmask=%rdx,<zq3=%r12
and  %rdx,%r12

# qhasm:   zq3 += squarer21
# asm 1: add  <squarer21=int64#9,<zq3=int64#10
# asm 2: add  <squarer21=%r11,<zq3=%r12
add  %r11,%r12

# qhasm:   squarer41 = (squarer41.zq4) << 13
# asm 1: shld $13,<zq4=int64#12,<squarer41=int64#13
# asm 2: shld $13,<zq4=%r14,<squarer41=%r15
shld $13,%r14,%r15

# qhasm:   zq4 &= squareredmask
# asm 1: and  <squareredmask=int64#3,<zq4=int64#12
# asm 2: and  <squareredmask=%rdx,<zq4=%r14
and  %rdx,%r14

# qhasm:   zq4 += squarer31
# asm 1: add  <squarer31=int64#11,<zq4=int64#12
# asm 2: add  <squarer31=%r13,<zq4=%r14
add  %r13,%r14

# qhasm:   squarer41 = squarer41 * 19
# asm 1: imulq  $19,<squarer41=int64#13,>squarer41=int64#4
# asm 2: imulq  $19,<squarer41=%r15,>squarer41=%rcx
imulq  $19,%r15,%rcx

# qhasm:   zq0 += squarer41
# asm 1: add  <squarer41=int64#4,<zq0=int64#2
# asm 2: add  <squarer41=%rcx,<zq0=%rsi
add  %rcx,%rsi

# qhasm:   squaret = zq0
# asm 1: mov  <zq0=int64#2,>squaret=int64#4
# asm 2: mov  <zq0=%rsi,>squaret=%rcx
mov  %rsi,%rcx

# qhasm:   (uint64) squaret >>= 51
# asm 1: shr  $51,<squaret=int64#4
# asm 2: shr  $51,<squaret=%rcx
shr  $51,%rcx

# qhasm:   squaret += zq1
# asm 1: add  <zq1=int64#5,<squaret=int64#4
# asm 2: add  <zq1=%r8,<squaret=%rcx
add  %r8,%rcx

# qhasm:   zq0 &= squareredmask
# asm 1: and  <squareredmask=int64#3,<zq0=int64#2
# asm 2: and  <squareredmask=%rdx,<zq0=%rsi
and  %rdx,%rsi

# qhasm:   zq1 = squaret
# asm 1: mov  <squaret=int64#4,>zq1=int64#5
# asm 2: mov  <squaret=%rcx,>zq1=%r8
mov  %rcx,%r8

# qhasm:   (uint64) squaret >>= 51
# asm 1: shr  $51,<squaret=int64#4
# asm 2: shr  $51,<squaret=%rcx
shr  $51,%rcx

# qhasm:   squaret += zq2
# asm 1: add  <zq2=int64#8,<squaret=int64#4
# asm 2: add  <zq2=%r10,<squaret=%rcx
add  %r10,%rcx

# qhasm:   zq1 &= squareredmask
# asm 1: and  <squareredmask=int64#3,<zq1=int64#5
# asm 2: and  <squareredmask=%rdx,<zq1=%r8
and  %rdx,%r8

# qhasm:   zq2 = squaret
# asm 1: mov  <squaret=int64#4,>zq2=int64#6
# asm 2: mov  <squaret=%rcx,>zq2=%r9
mov  %rcx,%r9

# qhasm:   (uint64) squaret >>= 51
# asm 1: shr  $51,<squaret=int64#4
# asm 2: shr  $51,<squaret=%rcx
shr  $51,%rcx

# qhasm:   squaret += zq3
# asm 1: add  <zq3=int64#10,<squaret=int64#4
# asm 2: add  <zq3=%r12,<squaret=%rcx
add  %r12,%rcx

# qhasm:   zq2 &= squareredmask
# asm 1: and  <squareredmask=int64#3,<zq2=int64#6
# asm 2: and  <squareredmask=%rdx,<zq2=%r9
and  %rdx,%r9

# qhasm:   zq3 = squaret
# asm 1: mov  <squaret=int64#4,>zq3=int64#7
# asm 2: mov  <squaret=%rcx,>zq3=%rax
mov  %rcx,%rax

# qhasm:   (uint64) squaret >>= 51
# asm 1: shr  $51,<squaret=int64#4
# asm 2: shr  $51,<squaret=%rcx
shr  $51,%rcx

# qhasm:   squaret += zq4
# asm 1: add  <zq4=int64#12,<squaret=int64#4
# asm 2: add  <zq4=%r14,<squaret=%rcx
add  %r14,%rcx

# qhasm:   zq3 &= squareredmask
# asm 1: and  <squareredmask=int64#3,<zq3=int64#7
# asm 2: and  <squareredmask=%rdx,<zq3=%rax
and  %rdx,%rax

# qhasm:   zq4 = squaret
# asm 1: mov  <squaret=int64#4,>zq4=int64#8
# asm 2: mov  <squaret=%rcx,>zq4=%r10
mov  %rcx,%r10

# qhasm:   (uint64) squaret >>= 51
# asm 1: shr  $51,<squaret=int64#4
# asm 2: shr  $51,<squaret=%rcx
shr  $51,%rcx

# qhasm:   squaret *= 19
# asm 1: imulq  $19,<squaret=int64#4,>squaret=int64#4
# asm 2: imulq  $19,<squaret=%rcx,>squaret=%rcx
imulq  $19,%rcx,%rcx

# qhasm:   zq0 += squaret
# asm 1: add  <squaret=int64#4,<zq0=int64#2
# asm 2: add  <squaret=%rcx,<zq0=%rsi
add  %rcx,%rsi

# qhasm:   zq4 &= squareredmask
# asm 1: and  <squareredmask=int64#3,<zq4=int64#8
# asm 2: and  <squareredmask=%rdx,<zq4=%r10
and  %rdx,%r10

# qhasm: *(uint64 *)(workp + 160) = zq0
# asm 1: movq   <zq0=int64#2,160(<workp=int64#1)
# asm 2: movq   <zq0=%rsi,160(<workp=%rdi)
movq   %rsi,160(%rdi)

# qhasm: *(uint64 *)(workp + 168) = zq1
# asm 1: movq   <zq1=int64#5,168(<workp=int64#1)
# asm 2: movq   <zq1=%r8,168(<workp=%rdi)
movq   %r8,168(%rdi)

# qhasm: *(uint64 *)(workp + 176) = zq2
# asm 1: movq   <zq2=int64#6,176(<workp=int64#1)
# asm 2: movq   <zq2=%r9,176(<workp=%rdi)
movq   %r9,176(%rdi)

# qhasm: *(uint64 *)(workp + 184) = zq3
# asm 1: movq   <zq3=int64#7,184(<workp=int64#1)
# asm 2: movq   <zq3=%rax,184(<workp=%rdi)
movq   %rax,184(%rdi)

# qhasm: *(uint64 *)(workp + 192) = zq4
# asm 1: movq   <zq4=int64#8,192(<workp=int64#1)
# asm 2: movq   <zq4=%r10,192(<workp=%rdi)
movq   %r10,192(%rdi)

# qhasm:   mulrax = *(uint64 *)(workp + 184)
# asm 1: movq   184(<workp=int64#1),>mulrax=int64#2
# asm 2: movq   184(<workp=%rdi),>mulrax=%rsi
movq   184(%rdi),%rsi

# qhasm:   mulrax *= 19
# asm 1: imulq  $19,<mulrax=int64#2,>mulrax=int64#7
# asm 2: imulq  $19,<mulrax=%rsi,>mulrax=%rax
imulq  $19,%rsi,%rax

# qhasm:   mulx319_stack = mulrax
# asm 1: movq <mulrax=int64#7,>mulx319_stack=stack64#8
# asm 2: movq <mulrax=%rax,>mulx319_stack=56(%rsp)
movq %rax,56(%rsp)

# qhasm:   (uint128) mulrdx mulrax = mulrax * *(uint64 *)(workp + 16)
# asm 1: mulq  16(<workp=int64#1)
# asm 2: mulq  16(<workp=%rdi)
mulq  16(%rdi)

# qhasm:   zq0 = mulrax
# asm 1: mov  <mulrax=int64#7,>zq0=int64#2
# asm 2: mov  <mulrax=%rax,>zq0=%rsi
mov  %rax,%rsi

# qhasm:   mulr01 = mulrdx
# asm 1: mov  <mulrdx=int64#3,>mulr01=int64#4
# asm 2: mov  <mulrdx=%rdx,>mulr01=%rcx
mov  %rdx,%rcx

# qhasm:   mulrax = *(uint64 *)(workp + 192)
# asm 1: movq   192(<workp=int64#1),>mulrax=int64#3
# asm 2: movq   192(<workp=%rdi),>mulrax=%rdx
movq   192(%rdi),%rdx

# qhasm:   mulrax *= 19
# asm 1: imulq  $19,<mulrax=int64#3,>mulrax=int64#7
# asm 2: imulq  $19,<mulrax=%rdx,>mulrax=%rax
imulq  $19,%rdx,%rax

# qhasm:   mulx419_stack = mulrax
# asm 1: movq <mulrax=int64#7,>mulx419_stack=stack64#9
# asm 2: movq <mulrax=%rax,>mulx419_stack=64(%rsp)
movq %rax,64(%rsp)

# qhasm:   (uint128) mulrdx mulrax = mulrax * *(uint64 *)(workp + 8)
# asm 1: mulq  8(<workp=int64#1)
# asm 2: mulq  8(<workp=%rdi)
mulq  8(%rdi)

# qhasm:   carry? zq0 += mulrax
# asm 1: add  <mulrax=int64#7,<zq0=int64#2
# asm 2: add  <mulrax=%rax,<zq0=%rsi
add  %rax,%rsi

# qhasm:   mulr01 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr01=int64#4
# asm 2: adc <mulrdx=%rdx,<mulr01=%rcx
adc %rdx,%rcx

# qhasm:   mulrax = *(uint64 *)(workp + 160)
# asm 1: movq   160(<workp=int64#1),>mulrax=int64#7
# asm 2: movq   160(<workp=%rdi),>mulrax=%rax
movq   160(%rdi),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * *(uint64 *)(workp + 0)
# asm 1: mulq  0(<workp=int64#1)
# asm 2: mulq  0(<workp=%rdi)
mulq  0(%rdi)

# qhasm:   carry? zq0 += mulrax
# asm 1: add  <mulrax=int64#7,<zq0=int64#2
# asm 2: add  <mulrax=%rax,<zq0=%rsi
add  %rax,%rsi

# qhasm:   mulr01 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr01=int64#4
# asm 2: adc <mulrdx=%rdx,<mulr01=%rcx
adc %rdx,%rcx

# qhasm:   mulrax = *(uint64 *)(workp + 160)
# asm 1: movq   160(<workp=int64#1),>mulrax=int64#7
# asm 2: movq   160(<workp=%rdi),>mulrax=%rax
movq   160(%rdi),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * *(uint64 *)(workp + 8)
# asm 1: mulq  8(<workp=int64#1)
# asm 2: mulq  8(<workp=%rdi)
mulq  8(%rdi)

# qhasm:   zq1 = mulrax
# asm 1: mov  <mulrax=int64#7,>zq1=int64#5
# asm 2: mov  <mulrax=%rax,>zq1=%r8
mov  %rax,%r8

# qhasm:   mulr11 = mulrdx
# asm 1: mov  <mulrdx=int64#3,>mulr11=int64#6
# asm 2: mov  <mulrdx=%rdx,>mulr11=%r9
mov  %rdx,%r9

# qhasm:   mulrax = *(uint64 *)(workp + 160)
# asm 1: movq   160(<workp=int64#1),>mulrax=int64#7
# asm 2: movq   160(<workp=%rdi),>mulrax=%rax
movq   160(%rdi),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * *(uint64 *)(workp + 16)
# asm 1: mulq  16(<workp=int64#1)
# asm 2: mulq  16(<workp=%rdi)
mulq  16(%rdi)

# qhasm:   zq2 = mulrax
# asm 1: mov  <mulrax=int64#7,>zq2=int64#8
# asm 2: mov  <mulrax=%rax,>zq2=%r10
mov  %rax,%r10

# qhasm:   mulr21 = mulrdx
# asm 1: mov  <mulrdx=int64#3,>mulr21=int64#9
# asm 2: mov  <mulrdx=%rdx,>mulr21=%r11
mov  %rdx,%r11

# qhasm:   mulrax = *(uint64 *)(workp + 160)
# asm 1: movq   160(<workp=int64#1),>mulrax=int64#7
# asm 2: movq   160(<workp=%rdi),>mulrax=%rax
movq   160(%rdi),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * *(uint64 *)(workp + 24)
# asm 1: mulq  24(<workp=int64#1)
# asm 2: mulq  24(<workp=%rdi)
mulq  24(%rdi)

# qhasm:   zq3 = mulrax
# asm 1: mov  <mulrax=int64#7,>zq3=int64#10
# asm 2: mov  <mulrax=%rax,>zq3=%r12
mov  %rax,%r12

# qhasm:   mulr31 = mulrdx
# asm 1: mov  <mulrdx=int64#3,>mulr31=int64#11
# asm 2: mov  <mulrdx=%rdx,>mulr31=%r13
mov  %rdx,%r13

# qhasm:   mulrax = *(uint64 *)(workp + 160)
# asm 1: movq   160(<workp=int64#1),>mulrax=int64#7
# asm 2: movq   160(<workp=%rdi),>mulrax=%rax
movq   160(%rdi),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * *(uint64 *)(workp + 32)
# asm 1: mulq  32(<workp=int64#1)
# asm 2: mulq  32(<workp=%rdi)
mulq  32(%rdi)

# qhasm:   zq4 = mulrax
# asm 1: mov  <mulrax=int64#7,>zq4=int64#12
# asm 2: mov  <mulrax=%rax,>zq4=%r14
mov  %rax,%r14

# qhasm:   mulr41 = mulrdx
# asm 1: mov  <mulrdx=int64#3,>mulr41=int64#13
# asm 2: mov  <mulrdx=%rdx,>mulr41=%r15
mov  %rdx,%r15

# qhasm:   mulrax = *(uint64 *)(workp + 168)
# asm 1: movq   168(<workp=int64#1),>mulrax=int64#7
# asm 2: movq   168(<workp=%rdi),>mulrax=%rax
movq   168(%rdi),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * *(uint64 *)(workp + 0)
# asm 1: mulq  0(<workp=int64#1)
# asm 2: mulq  0(<workp=%rdi)
mulq  0(%rdi)

# qhasm:   carry? zq1 += mulrax
# asm 1: add  <mulrax=int64#7,<zq1=int64#5
# asm 2: add  <mulrax=%rax,<zq1=%r8
add  %rax,%r8

# qhasm:   mulr11 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr11=int64#6
# asm 2: adc <mulrdx=%rdx,<mulr11=%r9
adc %rdx,%r9

# qhasm:   mulrax = *(uint64 *)(workp + 168)
# asm 1: movq   168(<workp=int64#1),>mulrax=int64#7
# asm 2: movq   168(<workp=%rdi),>mulrax=%rax
movq   168(%rdi),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * *(uint64 *)(workp + 8)
# asm 1: mulq  8(<workp=int64#1)
# asm 2: mulq  8(<workp=%rdi)
mulq  8(%rdi)

# qhasm:   carry? zq2 += mulrax
# asm 1: add  <mulrax=int64#7,<zq2=int64#8
# asm 2: add  <mulrax=%rax,<zq2=%r10
add  %rax,%r10

# qhasm:   mulr21 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr21=int64#9
# asm 2: adc <mulrdx=%rdx,<mulr21=%r11
adc %rdx,%r11

# qhasm:   mulrax = *(uint64 *)(workp + 168)
# asm 1: movq   168(<workp=int64#1),>mulrax=int64#7
# asm 2: movq   168(<workp=%rdi),>mulrax=%rax
movq   168(%rdi),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * *(uint64 *)(workp + 16)
# asm 1: mulq  16(<workp=int64#1)
# asm 2: mulq  16(<workp=%rdi)
mulq  16(%rdi)

# qhasm:   carry? zq3 += mulrax
# asm 1: add  <mulrax=int64#7,<zq3=int64#10
# asm 2: add  <mulrax=%rax,<zq3=%r12
add  %rax,%r12

# qhasm:   mulr31 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr31=int64#11
# asm 2: adc <mulrdx=%rdx,<mulr31=%r13
adc %rdx,%r13

# qhasm:   mulrax = *(uint64 *)(workp + 168)
# asm 1: movq   168(<workp=int64#1),>mulrax=int64#7
# asm 2: movq   168(<workp=%rdi),>mulrax=%rax
movq   168(%rdi),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * *(uint64 *)(workp + 24)
# asm 1: mulq  24(<workp=int64#1)
# asm 2: mulq  24(<workp=%rdi)
mulq  24(%rdi)

# qhasm:   carry? zq4 += mulrax
# asm 1: add  <mulrax=int64#7,<zq4=int64#12
# asm 2: add  <mulrax=%rax,<zq4=%r14
add  %rax,%r14

# qhasm:   mulr41 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr41=int64#13
# asm 2: adc <mulrdx=%rdx,<mulr41=%r15
adc %rdx,%r15

# qhasm:   mulrax = *(uint64 *)(workp + 168)
# asm 1: movq   168(<workp=int64#1),>mulrax=int64#3
# asm 2: movq   168(<workp=%rdi),>mulrax=%rdx
movq   168(%rdi),%rdx

# qhasm:   mulrax *= 19
# asm 1: imulq  $19,<mulrax=int64#3,>mulrax=int64#7
# asm 2: imulq  $19,<mulrax=%rdx,>mulrax=%rax
imulq  $19,%rdx,%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * *(uint64 *)(workp + 32)
# asm 1: mulq  32(<workp=int64#1)
# asm 2: mulq  32(<workp=%rdi)
mulq  32(%rdi)

# qhasm:   carry? zq0 += mulrax
# asm 1: add  <mulrax=int64#7,<zq0=int64#2
# asm 2: add  <mulrax=%rax,<zq0=%rsi
add  %rax,%rsi

# qhasm:   mulr01 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr01=int64#4
# asm 2: adc <mulrdx=%rdx,<mulr01=%rcx
adc %rdx,%rcx

# qhasm:   mulrax = *(uint64 *)(workp + 176)
# asm 1: movq   176(<workp=int64#1),>mulrax=int64#7
# asm 2: movq   176(<workp=%rdi),>mulrax=%rax
movq   176(%rdi),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * *(uint64 *)(workp + 0)
# asm 1: mulq  0(<workp=int64#1)
# asm 2: mulq  0(<workp=%rdi)
mulq  0(%rdi)

# qhasm:   carry? zq2 += mulrax
# asm 1: add  <mulrax=int64#7,<zq2=int64#8
# asm 2: add  <mulrax=%rax,<zq2=%r10
add  %rax,%r10

# qhasm:   mulr21 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr21=int64#9
# asm 2: adc <mulrdx=%rdx,<mulr21=%r11
adc %rdx,%r11

# qhasm:   mulrax = *(uint64 *)(workp + 176)
# asm 1: movq   176(<workp=int64#1),>mulrax=int64#7
# asm 2: movq   176(<workp=%rdi),>mulrax=%rax
movq   176(%rdi),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * *(uint64 *)(workp + 8)
# asm 1: mulq  8(<workp=int64#1)
# asm 2: mulq  8(<workp=%rdi)
mulq  8(%rdi)

# qhasm:   carry? zq3 += mulrax
# asm 1: add  <mulrax=int64#7,<zq3=int64#10
# asm 2: add  <mulrax=%rax,<zq3=%r12
add  %rax,%r12

# qhasm:   mulr31 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr31=int64#11
# asm 2: adc <mulrdx=%rdx,<mulr31=%r13
adc %rdx,%r13

# qhasm:   mulrax = *(uint64 *)(workp + 176)
# asm 1: movq   176(<workp=int64#1),>mulrax=int64#7
# asm 2: movq   176(<workp=%rdi),>mulrax=%rax
movq   176(%rdi),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * *(uint64 *)(workp + 16)
# asm 1: mulq  16(<workp=int64#1)
# asm 2: mulq  16(<workp=%rdi)
mulq  16(%rdi)

# qhasm:   carry? zq4 += mulrax
# asm 1: add  <mulrax=int64#7,<zq4=int64#12
# asm 2: add  <mulrax=%rax,<zq4=%r14
add  %rax,%r14

# qhasm:   mulr41 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr41=int64#13
# asm 2: adc <mulrdx=%rdx,<mulr41=%r15
adc %rdx,%r15

# qhasm:   mulrax = *(uint64 *)(workp + 176)
# asm 1: movq   176(<workp=int64#1),>mulrax=int64#3
# asm 2: movq   176(<workp=%rdi),>mulrax=%rdx
movq   176(%rdi),%rdx

# qhasm:   mulrax *= 19
# asm 1: imulq  $19,<mulrax=int64#3,>mulrax=int64#7
# asm 2: imulq  $19,<mulrax=%rdx,>mulrax=%rax
imulq  $19,%rdx,%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * *(uint64 *)(workp + 24)
# asm 1: mulq  24(<workp=int64#1)
# asm 2: mulq  24(<workp=%rdi)
mulq  24(%rdi)

# qhasm:   carry? zq0 += mulrax
# asm 1: add  <mulrax=int64#7,<zq0=int64#2
# asm 2: add  <mulrax=%rax,<zq0=%rsi
add  %rax,%rsi

# qhasm:   mulr01 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr01=int64#4
# asm 2: adc <mulrdx=%rdx,<mulr01=%rcx
adc %rdx,%rcx

# qhasm:   mulrax = *(uint64 *)(workp + 176)
# asm 1: movq   176(<workp=int64#1),>mulrax=int64#3
# asm 2: movq   176(<workp=%rdi),>mulrax=%rdx
movq   176(%rdi),%rdx

# qhasm:   mulrax *= 19
# asm 1: imulq  $19,<mulrax=int64#3,>mulrax=int64#7
# asm 2: imulq  $19,<mulrax=%rdx,>mulrax=%rax
imulq  $19,%rdx,%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * *(uint64 *)(workp + 32)
# asm 1: mulq  32(<workp=int64#1)
# asm 2: mulq  32(<workp=%rdi)
mulq  32(%rdi)

# qhasm:   carry? zq1 += mulrax
# asm 1: add  <mulrax=int64#7,<zq1=int64#5
# asm 2: add  <mulrax=%rax,<zq1=%r8
add  %rax,%r8

# qhasm:   mulr11 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr11=int64#6
# asm 2: adc <mulrdx=%rdx,<mulr11=%r9
adc %rdx,%r9

# qhasm:   mulrax = *(uint64 *)(workp + 184)
# asm 1: movq   184(<workp=int64#1),>mulrax=int64#7
# asm 2: movq   184(<workp=%rdi),>mulrax=%rax
movq   184(%rdi),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * *(uint64 *)(workp + 0)
# asm 1: mulq  0(<workp=int64#1)
# asm 2: mulq  0(<workp=%rdi)
mulq  0(%rdi)

# qhasm:   carry? zq3 += mulrax
# asm 1: add  <mulrax=int64#7,<zq3=int64#10
# asm 2: add  <mulrax=%rax,<zq3=%r12
add  %rax,%r12

# qhasm:   mulr31 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr31=int64#11
# asm 2: adc <mulrdx=%rdx,<mulr31=%r13
adc %rdx,%r13

# qhasm:   mulrax = *(uint64 *)(workp + 184)
# asm 1: movq   184(<workp=int64#1),>mulrax=int64#7
# asm 2: movq   184(<workp=%rdi),>mulrax=%rax
movq   184(%rdi),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * *(uint64 *)(workp + 8)
# asm 1: mulq  8(<workp=int64#1)
# asm 2: mulq  8(<workp=%rdi)
mulq  8(%rdi)

# qhasm:   carry? zq4 += mulrax
# asm 1: add  <mulrax=int64#7,<zq4=int64#12
# asm 2: add  <mulrax=%rax,<zq4=%r14
add  %rax,%r14

# qhasm:   mulr41 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr41=int64#13
# asm 2: adc <mulrdx=%rdx,<mulr41=%r15
adc %rdx,%r15

# qhasm:   mulrax = mulx319_stack
# asm 1: movq <mulx319_stack=stack64#8,>mulrax=int64#7
# asm 2: movq <mulx319_stack=56(%rsp),>mulrax=%rax
movq 56(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * *(uint64 *)(workp + 24)
# asm 1: mulq  24(<workp=int64#1)
# asm 2: mulq  24(<workp=%rdi)
mulq  24(%rdi)

# qhasm:   carry? zq1 += mulrax
# asm 1: add  <mulrax=int64#7,<zq1=int64#5
# asm 2: add  <mulrax=%rax,<zq1=%r8
add  %rax,%r8

# qhasm:   mulr11 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr11=int64#6
# asm 2: adc <mulrdx=%rdx,<mulr11=%r9
adc %rdx,%r9

# qhasm:   mulrax = mulx319_stack
# asm 1: movq <mulx319_stack=stack64#8,>mulrax=int64#7
# asm 2: movq <mulx319_stack=56(%rsp),>mulrax=%rax
movq 56(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * *(uint64 *)(workp + 32)
# asm 1: mulq  32(<workp=int64#1)
# asm 2: mulq  32(<workp=%rdi)
mulq  32(%rdi)

# qhasm:   carry? zq2 += mulrax
# asm 1: add  <mulrax=int64#7,<zq2=int64#8
# asm 2: add  <mulrax=%rax,<zq2=%r10
add  %rax,%r10

# qhasm:   mulr21 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr21=int64#9
# asm 2: adc <mulrdx=%rdx,<mulr21=%r11
adc %rdx,%r11

# qhasm:   mulrax = *(uint64 *)(workp + 192)
# asm 1: movq   192(<workp=int64#1),>mulrax=int64#7
# asm 2: movq   192(<workp=%rdi),>mulrax=%rax
movq   192(%rdi),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * *(uint64 *)(workp + 0)
# asm 1: mulq  0(<workp=int64#1)
# asm 2: mulq  0(<workp=%rdi)
mulq  0(%rdi)

# qhasm:   carry? zq4 += mulrax
# asm 1: add  <mulrax=int64#7,<zq4=int64#12
# asm 2: add  <mulrax=%rax,<zq4=%r14
add  %rax,%r14

# qhasm:   mulr41 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr41=int64#13
# asm 2: adc <mulrdx=%rdx,<mulr41=%r15
adc %rdx,%r15

# qhasm:   mulrax = mulx419_stack
# asm 1: movq <mulx419_stack=stack64#9,>mulrax=int64#7
# asm 2: movq <mulx419_stack=64(%rsp),>mulrax=%rax
movq 64(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * *(uint64 *)(workp + 16)
# asm 1: mulq  16(<workp=int64#1)
# asm 2: mulq  16(<workp=%rdi)
mulq  16(%rdi)

# qhasm:   carry? zq1 += mulrax
# asm 1: add  <mulrax=int64#7,<zq1=int64#5
# asm 2: add  <mulrax=%rax,<zq1=%r8
add  %rax,%r8

# qhasm:   mulr11 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr11=int64#6
# asm 2: adc <mulrdx=%rdx,<mulr11=%r9
adc %rdx,%r9

# qhasm:   mulrax = mulx419_stack
# asm 1: movq <mulx419_stack=stack64#9,>mulrax=int64#7
# asm 2: movq <mulx419_stack=64(%rsp),>mulrax=%rax
movq 64(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * *(uint64 *)(workp + 24)
# asm 1: mulq  24(<workp=int64#1)
# asm 2: mulq  24(<workp=%rdi)
mulq  24(%rdi)

# qhasm:   carry? zq2 += mulrax
# asm 1: add  <mulrax=int64#7,<zq2=int64#8
# asm 2: add  <mulrax=%rax,<zq2=%r10
add  %rax,%r10

# qhasm:   mulr21 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr21=int64#9
# asm 2: adc <mulrdx=%rdx,<mulr21=%r11
adc %rdx,%r11

# qhasm:   mulrax = mulx419_stack
# asm 1: movq <mulx419_stack=stack64#9,>mulrax=int64#7
# asm 2: movq <mulx419_stack=64(%rsp),>mulrax=%rax
movq 64(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * *(uint64 *)(workp + 32)
# asm 1: mulq  32(<workp=int64#1)
# asm 2: mulq  32(<workp=%rdi)
mulq  32(%rdi)

# qhasm:   carry? zq3 += mulrax
# asm 1: add  <mulrax=int64#7,<zq3=int64#10
# asm 2: add  <mulrax=%rax,<zq3=%r12
add  %rax,%r12

# qhasm:   mulr31 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr31=int64#11
# asm 2: adc <mulrdx=%rdx,<mulr31=%r13
adc %rdx,%r13

# qhasm:   mulredmask = *(uint64 *) &crypto_scalarmult_curve25519_amd64_51_REDMASK51
# asm 1: movq crypto_scalarmult_curve25519_amd64_51_REDMASK51,>mulredmask=int64#3
# asm 2: movq crypto_scalarmult_curve25519_amd64_51_REDMASK51,>mulredmask=%rdx
movq crypto_scalarmult_curve25519_amd64_51_REDMASK51,%rdx

# qhasm:   mulr01 = (mulr01.zq0) << 13
# asm 1: shld $13,<zq0=int64#2,<mulr01=int64#4
# asm 2: shld $13,<zq0=%rsi,<mulr01=%rcx
shld $13,%rsi,%rcx

# qhasm:   zq0 &= mulredmask
# asm 1: and  <mulredmask=int64#3,<zq0=int64#2
# asm 2: and  <mulredmask=%rdx,<zq0=%rsi
and  %rdx,%rsi

# qhasm:   mulr11 = (mulr11.zq1) << 13
# asm 1: shld $13,<zq1=int64#5,<mulr11=int64#6
# asm 2: shld $13,<zq1=%r8,<mulr11=%r9
shld $13,%r8,%r9

# qhasm:   zq1 &= mulredmask
# asm 1: and  <mulredmask=int64#3,<zq1=int64#5
# asm 2: and  <mulredmask=%rdx,<zq1=%r8
and  %rdx,%r8

# qhasm:   zq1 += mulr01
# asm 1: add  <mulr01=int64#4,<zq1=int64#5
# asm 2: add  <mulr01=%rcx,<zq1=%r8
add  %rcx,%r8

# qhasm:   mulr21 = (mulr21.zq2) << 13
# asm 1: shld $13,<zq2=int64#8,<mulr21=int64#9
# asm 2: shld $13,<zq2=%r10,<mulr21=%r11
shld $13,%r10,%r11

# qhasm:   zq2 &= mulredmask
# asm 1: and  <mulredmask=int64#3,<zq2=int64#8
# asm 2: and  <mulredmask=%rdx,<zq2=%r10
and  %rdx,%r10

# qhasm:   zq2 += mulr11
# asm 1: add  <mulr11=int64#6,<zq2=int64#8
# asm 2: add  <mulr11=%r9,<zq2=%r10
add  %r9,%r10

# qhasm:   mulr31 = (mulr31.zq3) << 13
# asm 1: shld $13,<zq3=int64#10,<mulr31=int64#11
# asm 2: shld $13,<zq3=%r12,<mulr31=%r13
shld $13,%r12,%r13

# qhasm:   zq3 &= mulredmask
# asm 1: and  <mulredmask=int64#3,<zq3=int64#10
# asm 2: and  <mulredmask=%rdx,<zq3=%r12
and  %rdx,%r12

# qhasm:   zq3 += mulr21
# asm 1: add  <mulr21=int64#9,<zq3=int64#10
# asm 2: add  <mulr21=%r11,<zq3=%r12
add  %r11,%r12

# qhasm:   mulr41 = (mulr41.zq4) << 13
# asm 1: shld $13,<zq4=int64#12,<mulr41=int64#13
# asm 2: shld $13,<zq4=%r14,<mulr41=%r15
shld $13,%r14,%r15

# qhasm:   zq4 &= mulredmask
# asm 1: and  <mulredmask=int64#3,<zq4=int64#12
# asm 2: and  <mulredmask=%rdx,<zq4=%r14
and  %rdx,%r14

# qhasm:   zq4 += mulr31
# asm 1: add  <mulr31=int64#11,<zq4=int64#12
# asm 2: add  <mulr31=%r13,<zq4=%r14
add  %r13,%r14

# qhasm:   mulr41 = mulr41 * 19
# asm 1: imulq  $19,<mulr41=int64#13,>mulr41=int64#4
# asm 2: imulq  $19,<mulr41=%r15,>mulr41=%rcx
imulq  $19,%r15,%rcx

# qhasm:   zq0 += mulr41
# asm 1: add  <mulr41=int64#4,<zq0=int64#2
# asm 2: add  <mulr41=%rcx,<zq0=%rsi
add  %rcx,%rsi

# qhasm:   mult = zq0
# asm 1: mov  <zq0=int64#2,>mult=int64#4
# asm 2: mov  <zq0=%rsi,>mult=%rcx
mov  %rsi,%rcx

# qhasm:   (uint64) mult >>= 51
# asm 1: shr  $51,<mult=int64#4
# asm 2: shr  $51,<mult=%rcx
shr  $51,%rcx

# qhasm:   mult += zq1
# asm 1: add  <zq1=int64#5,<mult=int64#4
# asm 2: add  <zq1=%r8,<mult=%rcx
add  %r8,%rcx

# qhasm:   zq1 = mult
# asm 1: mov  <mult=int64#4,>zq1=int64#5
# asm 2: mov  <mult=%rcx,>zq1=%r8
mov  %rcx,%r8

# qhasm:   (uint64) mult >>= 51
# asm 1: shr  $51,<mult=int64#4
# asm 2: shr  $51,<mult=%rcx
shr  $51,%rcx

# qhasm:   zq0 &= mulredmask
# asm 1: and  <mulredmask=int64#3,<zq0=int64#2
# asm 2: and  <mulredmask=%rdx,<zq0=%rsi
and  %rdx,%rsi

# qhasm:   mult += zq2
# asm 1: add  <zq2=int64#8,<mult=int64#4
# asm 2: add  <zq2=%r10,<mult=%rcx
add  %r10,%rcx

# qhasm:   zq2 = mult
# asm 1: mov  <mult=int64#4,>zq2=int64#6
# asm 2: mov  <mult=%rcx,>zq2=%r9
mov  %rcx,%r9

# qhasm:   (uint64) mult >>= 51
# asm 1: shr  $51,<mult=int64#4
# asm 2: shr  $51,<mult=%rcx
shr  $51,%rcx

# qhasm:   zq1 &= mulredmask
# asm 1: and  <mulredmask=int64#3,<zq1=int64#5
# asm 2: and  <mulredmask=%rdx,<zq1=%r8
and  %rdx,%r8

# qhasm:   mult += zq3
# asm 1: add  <zq3=int64#10,<mult=int64#4
# asm 2: add  <zq3=%r12,<mult=%rcx
add  %r12,%rcx

# qhasm:   zq3 = mult
# asm 1: mov  <mult=int64#4,>zq3=int64#7
# asm 2: mov  <mult=%rcx,>zq3=%rax
mov  %rcx,%rax

# qhasm:   (uint64) mult >>= 51
# asm 1: shr  $51,<mult=int64#4
# asm 2: shr  $51,<mult=%rcx
shr  $51,%rcx

# qhasm:   zq2 &= mulredmask
# asm 1: and  <mulredmask=int64#3,<zq2=int64#6
# asm 2: and  <mulredmask=%rdx,<zq2=%r9
and  %rdx,%r9

# qhasm:   mult += zq4
# asm 1: add  <zq4=int64#12,<mult=int64#4
# asm 2: add  <zq4=%r14,<mult=%rcx
add  %r14,%rcx

# qhasm:   zq4 = mult
# asm 1: mov  <mult=int64#4,>zq4=int64#8
# asm 2: mov  <mult=%rcx,>zq4=%r10
mov  %rcx,%r10

# qhasm:   (uint64) mult >>= 51
# asm 1: shr  $51,<mult=int64#4
# asm 2: shr  $51,<mult=%rcx
shr  $51,%rcx

# qhasm:   zq3 &= mulredmask
# asm 1: and  <mulredmask=int64#3,<zq3=int64#7
# asm 2: and  <mulredmask=%rdx,<zq3=%rax
and  %rdx,%rax

# qhasm:   mult *= 19
# asm 1: imulq  $19,<mult=int64#4,>mult=int64#4
# asm 2: imulq  $19,<mult=%rcx,>mult=%rcx
imulq  $19,%rcx,%rcx

# qhasm:   zq0 += mult
# asm 1: add  <mult=int64#4,<zq0=int64#2
# asm 2: add  <mult=%rcx,<zq0=%rsi
add  %rcx,%rsi

# qhasm:   zq4 &= mulredmask
# asm 1: and  <mulredmask=int64#3,<zq4=int64#8
# asm 2: and  <mulredmask=%rdx,<zq4=%r10
and  %rdx,%r10

# qhasm: *(uint64 *)(workp + 160) = zq0
# asm 1: movq   <zq0=int64#2,160(<workp=int64#1)
# asm 2: movq   <zq0=%rsi,160(<workp=%rdi)
movq   %rsi,160(%rdi)

# qhasm: *(uint64 *)(workp + 168) = zq1
# asm 1: movq   <zq1=int64#5,168(<workp=int64#1)
# asm 2: movq   <zq1=%r8,168(<workp=%rdi)
movq   %r8,168(%rdi)

# qhasm: *(uint64 *)(workp + 176) = zq2
# asm 1: movq   <zq2=int64#6,176(<workp=int64#1)
# asm 2: movq   <zq2=%r9,176(<workp=%rdi)
movq   %r9,176(%rdi)

# qhasm: *(uint64 *)(workp + 184) = zq3
# asm 1: movq   <zq3=int64#7,184(<workp=int64#1)
# asm 2: movq   <zq3=%rax,184(<workp=%rdi)
movq   %rax,184(%rdi)

# qhasm: *(uint64 *)(workp + 192) = zq4
# asm 1: movq   <zq4=int64#8,192(<workp=int64#1)
# asm 2: movq   <zq4=%r10,192(<workp=%rdi)
movq   %r10,192(%rdi)

# qhasm:   mulrax = t63_stack
# asm 1: movq <t63_stack=stack64#26,>mulrax=int64#2
# asm 2: movq <t63_stack=200(%rsp),>mulrax=%rsi
movq 200(%rsp),%rsi

# qhasm:   mulrax *= 19
# asm 1: imulq  $19,<mulrax=int64#2,>mulrax=int64#7
# asm 2: imulq  $19,<mulrax=%rsi,>mulrax=%rax
imulq  $19,%rsi,%rax

# qhasm:   mulx319_stack = mulrax
# asm 1: movq <mulrax=int64#7,>mulx319_stack=stack64#8
# asm 2: movq <mulrax=%rax,>mulx319_stack=56(%rsp)
movq %rax,56(%rsp)

# qhasm:   (uint128) mulrdx mulrax = mulrax * t72_stack
# asm 1: mulq  <t72_stack=stack64#20
# asm 2: mulq  <t72_stack=152(%rsp)
mulq  152(%rsp)

# qhasm:   xp0 = mulrax
# asm 1: mov  <mulrax=int64#7,>xp0=int64#2
# asm 2: mov  <mulrax=%rax,>xp0=%rsi
mov  %rax,%rsi

# qhasm:   mulr01 = mulrdx
# asm 1: mov  <mulrdx=int64#3,>mulr01=int64#4
# asm 2: mov  <mulrdx=%rdx,>mulr01=%rcx
mov  %rdx,%rcx

# qhasm:   mulrax = t64_stack
# asm 1: movq <t64_stack=stack64#27,>mulrax=int64#3
# asm 2: movq <t64_stack=208(%rsp),>mulrax=%rdx
movq 208(%rsp),%rdx

# qhasm:   mulrax *= 19
# asm 1: imulq  $19,<mulrax=int64#3,>mulrax=int64#7
# asm 2: imulq  $19,<mulrax=%rdx,>mulrax=%rax
imulq  $19,%rdx,%rax

# qhasm:   mulx419_stack = mulrax
# asm 1: movq <mulrax=int64#7,>mulx419_stack=stack64#9
# asm 2: movq <mulrax=%rax,>mulx419_stack=64(%rsp)
movq %rax,64(%rsp)

# qhasm:   (uint128) mulrdx mulrax = mulrax * t71_stack
# asm 1: mulq  <t71_stack=stack64#19
# asm 2: mulq  <t71_stack=144(%rsp)
mulq  144(%rsp)

# qhasm:   carry? xp0 += mulrax
# asm 1: add  <mulrax=int64#7,<xp0=int64#2
# asm 2: add  <mulrax=%rax,<xp0=%rsi
add  %rax,%rsi

# qhasm:   mulr01 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr01=int64#4
# asm 2: adc <mulrdx=%rdx,<mulr01=%rcx
adc %rdx,%rcx

# qhasm:   mulrax = t60_stack
# asm 1: movq <t60_stack=stack64#23,>mulrax=int64#7
# asm 2: movq <t60_stack=176(%rsp),>mulrax=%rax
movq 176(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * t70_stack
# asm 1: mulq  <t70_stack=stack64#18
# asm 2: mulq  <t70_stack=136(%rsp)
mulq  136(%rsp)

# qhasm:   carry? xp0 += mulrax
# asm 1: add  <mulrax=int64#7,<xp0=int64#2
# asm 2: add  <mulrax=%rax,<xp0=%rsi
add  %rax,%rsi

# qhasm:   mulr01 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr01=int64#4
# asm 2: adc <mulrdx=%rdx,<mulr01=%rcx
adc %rdx,%rcx

# qhasm:   mulrax = t60_stack
# asm 1: movq <t60_stack=stack64#23,>mulrax=int64#7
# asm 2: movq <t60_stack=176(%rsp),>mulrax=%rax
movq 176(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * t71_stack
# asm 1: mulq  <t71_stack=stack64#19
# asm 2: mulq  <t71_stack=144(%rsp)
mulq  144(%rsp)

# qhasm:   xp1 = mulrax
# asm 1: mov  <mulrax=int64#7,>xp1=int64#5
# asm 2: mov  <mulrax=%rax,>xp1=%r8
mov  %rax,%r8

# qhasm:   mulr11 = mulrdx
# asm 1: mov  <mulrdx=int64#3,>mulr11=int64#6
# asm 2: mov  <mulrdx=%rdx,>mulr11=%r9
mov  %rdx,%r9

# qhasm:   mulrax = t60_stack
# asm 1: movq <t60_stack=stack64#23,>mulrax=int64#7
# asm 2: movq <t60_stack=176(%rsp),>mulrax=%rax
movq 176(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * t72_stack
# asm 1: mulq  <t72_stack=stack64#20
# asm 2: mulq  <t72_stack=152(%rsp)
mulq  152(%rsp)

# qhasm:   xp2 = mulrax
# asm 1: mov  <mulrax=int64#7,>xp2=int64#8
# asm 2: mov  <mulrax=%rax,>xp2=%r10
mov  %rax,%r10

# qhasm:   mulr21 = mulrdx
# asm 1: mov  <mulrdx=int64#3,>mulr21=int64#9
# asm 2: mov  <mulrdx=%rdx,>mulr21=%r11
mov  %rdx,%r11

# qhasm:   mulrax = t60_stack
# asm 1: movq <t60_stack=stack64#23,>mulrax=int64#7
# asm 2: movq <t60_stack=176(%rsp),>mulrax=%rax
movq 176(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * t73_stack
# asm 1: mulq  <t73_stack=stack64#21
# asm 2: mulq  <t73_stack=160(%rsp)
mulq  160(%rsp)

# qhasm:   xp3 = mulrax
# asm 1: mov  <mulrax=int64#7,>xp3=int64#10
# asm 2: mov  <mulrax=%rax,>xp3=%r12
mov  %rax,%r12

# qhasm:   mulr31 = mulrdx
# asm 1: mov  <mulrdx=int64#3,>mulr31=int64#11
# asm 2: mov  <mulrdx=%rdx,>mulr31=%r13
mov  %rdx,%r13

# qhasm:   mulrax = t60_stack
# asm 1: movq <t60_stack=stack64#23,>mulrax=int64#7
# asm 2: movq <t60_stack=176(%rsp),>mulrax=%rax
movq 176(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * t74_stack
# asm 1: mulq  <t74_stack=stack64#22
# asm 2: mulq  <t74_stack=168(%rsp)
mulq  168(%rsp)

# qhasm:   xp4 = mulrax
# asm 1: mov  <mulrax=int64#7,>xp4=int64#12
# asm 2: mov  <mulrax=%rax,>xp4=%r14
mov  %rax,%r14

# qhasm:   mulr41 = mulrdx
# asm 1: mov  <mulrdx=int64#3,>mulr41=int64#13
# asm 2: mov  <mulrdx=%rdx,>mulr41=%r15
mov  %rdx,%r15

# qhasm:   mulrax = t61_stack
# asm 1: movq <t61_stack=stack64#24,>mulrax=int64#7
# asm 2: movq <t61_stack=184(%rsp),>mulrax=%rax
movq 184(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * t70_stack
# asm 1: mulq  <t70_stack=stack64#18
# asm 2: mulq  <t70_stack=136(%rsp)
mulq  136(%rsp)

# qhasm:   carry? xp1 += mulrax
# asm 1: add  <mulrax=int64#7,<xp1=int64#5
# asm 2: add  <mulrax=%rax,<xp1=%r8
add  %rax,%r8

# qhasm:   mulr11 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr11=int64#6
# asm 2: adc <mulrdx=%rdx,<mulr11=%r9
adc %rdx,%r9

# qhasm:   mulrax = t61_stack
# asm 1: movq <t61_stack=stack64#24,>mulrax=int64#7
# asm 2: movq <t61_stack=184(%rsp),>mulrax=%rax
movq 184(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * t71_stack
# asm 1: mulq  <t71_stack=stack64#19
# asm 2: mulq  <t71_stack=144(%rsp)
mulq  144(%rsp)

# qhasm:   carry? xp2 += mulrax
# asm 1: add  <mulrax=int64#7,<xp2=int64#8
# asm 2: add  <mulrax=%rax,<xp2=%r10
add  %rax,%r10

# qhasm:   mulr21 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr21=int64#9
# asm 2: adc <mulrdx=%rdx,<mulr21=%r11
adc %rdx,%r11

# qhasm:   mulrax = t61_stack
# asm 1: movq <t61_stack=stack64#24,>mulrax=int64#7
# asm 2: movq <t61_stack=184(%rsp),>mulrax=%rax
movq 184(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * t72_stack
# asm 1: mulq  <t72_stack=stack64#20
# asm 2: mulq  <t72_stack=152(%rsp)
mulq  152(%rsp)

# qhasm:   carry? xp3 += mulrax
# asm 1: add  <mulrax=int64#7,<xp3=int64#10
# asm 2: add  <mulrax=%rax,<xp3=%r12
add  %rax,%r12

# qhasm:   mulr31 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr31=int64#11
# asm 2: adc <mulrdx=%rdx,<mulr31=%r13
adc %rdx,%r13

# qhasm:   mulrax = t61_stack
# asm 1: movq <t61_stack=stack64#24,>mulrax=int64#7
# asm 2: movq <t61_stack=184(%rsp),>mulrax=%rax
movq 184(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * t73_stack
# asm 1: mulq  <t73_stack=stack64#21
# asm 2: mulq  <t73_stack=160(%rsp)
mulq  160(%rsp)

# qhasm:   carry? xp4 += mulrax
# asm 1: add  <mulrax=int64#7,<xp4=int64#12
# asm 2: add  <mulrax=%rax,<xp4=%r14
add  %rax,%r14

# qhasm:   mulr41 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr41=int64#13
# asm 2: adc <mulrdx=%rdx,<mulr41=%r15
adc %rdx,%r15

# qhasm:   mulrax = t61_stack
# asm 1: movq <t61_stack=stack64#24,>mulrax=int64#3
# asm 2: movq <t61_stack=184(%rsp),>mulrax=%rdx
movq 184(%rsp),%rdx

# qhasm:   mulrax *= 19
# asm 1: imulq  $19,<mulrax=int64#3,>mulrax=int64#7
# asm 2: imulq  $19,<mulrax=%rdx,>mulrax=%rax
imulq  $19,%rdx,%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * t74_stack
# asm 1: mulq  <t74_stack=stack64#22
# asm 2: mulq  <t74_stack=168(%rsp)
mulq  168(%rsp)

# qhasm:   carry? xp0 += mulrax
# asm 1: add  <mulrax=int64#7,<xp0=int64#2
# asm 2: add  <mulrax=%rax,<xp0=%rsi
add  %rax,%rsi

# qhasm:   mulr01 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr01=int64#4
# asm 2: adc <mulrdx=%rdx,<mulr01=%rcx
adc %rdx,%rcx

# qhasm:   mulrax = t62_stack
# asm 1: movq <t62_stack=stack64#25,>mulrax=int64#7
# asm 2: movq <t62_stack=192(%rsp),>mulrax=%rax
movq 192(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * t70_stack
# asm 1: mulq  <t70_stack=stack64#18
# asm 2: mulq  <t70_stack=136(%rsp)
mulq  136(%rsp)

# qhasm:   carry? xp2 += mulrax
# asm 1: add  <mulrax=int64#7,<xp2=int64#8
# asm 2: add  <mulrax=%rax,<xp2=%r10
add  %rax,%r10

# qhasm:   mulr21 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr21=int64#9
# asm 2: adc <mulrdx=%rdx,<mulr21=%r11
adc %rdx,%r11

# qhasm:   mulrax = t62_stack
# asm 1: movq <t62_stack=stack64#25,>mulrax=int64#7
# asm 2: movq <t62_stack=192(%rsp),>mulrax=%rax
movq 192(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * t71_stack
# asm 1: mulq  <t71_stack=stack64#19
# asm 2: mulq  <t71_stack=144(%rsp)
mulq  144(%rsp)

# qhasm:   carry? xp3 += mulrax
# asm 1: add  <mulrax=int64#7,<xp3=int64#10
# asm 2: add  <mulrax=%rax,<xp3=%r12
add  %rax,%r12

# qhasm:   mulr31 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr31=int64#11
# asm 2: adc <mulrdx=%rdx,<mulr31=%r13
adc %rdx,%r13

# qhasm:   mulrax = t62_stack
# asm 1: movq <t62_stack=stack64#25,>mulrax=int64#7
# asm 2: movq <t62_stack=192(%rsp),>mulrax=%rax
movq 192(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * t72_stack
# asm 1: mulq  <t72_stack=stack64#20
# asm 2: mulq  <t72_stack=152(%rsp)
mulq  152(%rsp)

# qhasm:   carry? xp4 += mulrax
# asm 1: add  <mulrax=int64#7,<xp4=int64#12
# asm 2: add  <mulrax=%rax,<xp4=%r14
add  %rax,%r14

# qhasm:   mulr41 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr41=int64#13
# asm 2: adc <mulrdx=%rdx,<mulr41=%r15
adc %rdx,%r15

# qhasm:   mulrax = t62_stack
# asm 1: movq <t62_stack=stack64#25,>mulrax=int64#3
# asm 2: movq <t62_stack=192(%rsp),>mulrax=%rdx
movq 192(%rsp),%rdx

# qhasm:   mulrax *= 19
# asm 1: imulq  $19,<mulrax=int64#3,>mulrax=int64#7
# asm 2: imulq  $19,<mulrax=%rdx,>mulrax=%rax
imulq  $19,%rdx,%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * t73_stack
# asm 1: mulq  <t73_stack=stack64#21
# asm 2: mulq  <t73_stack=160(%rsp)
mulq  160(%rsp)

# qhasm:   carry? xp0 += mulrax
# asm 1: add  <mulrax=int64#7,<xp0=int64#2
# asm 2: add  <mulrax=%rax,<xp0=%rsi
add  %rax,%rsi

# qhasm:   mulr01 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr01=int64#4
# asm 2: adc <mulrdx=%rdx,<mulr01=%rcx
adc %rdx,%rcx

# qhasm:   mulrax = t62_stack
# asm 1: movq <t62_stack=stack64#25,>mulrax=int64#3
# asm 2: movq <t62_stack=192(%rsp),>mulrax=%rdx
movq 192(%rsp),%rdx

# qhasm:   mulrax *= 19
# asm 1: imulq  $19,<mulrax=int64#3,>mulrax=int64#7
# asm 2: imulq  $19,<mulrax=%rdx,>mulrax=%rax
imulq  $19,%rdx,%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * t74_stack
# asm 1: mulq  <t74_stack=stack64#22
# asm 2: mulq  <t74_stack=168(%rsp)
mulq  168(%rsp)

# qhasm:   carry? xp1 += mulrax
# asm 1: add  <mulrax=int64#7,<xp1=int64#5
# asm 2: add  <mulrax=%rax,<xp1=%r8
add  %rax,%r8

# qhasm:   mulr11 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr11=int64#6
# asm 2: adc <mulrdx=%rdx,<mulr11=%r9
adc %rdx,%r9

# qhasm:   mulrax = t63_stack
# asm 1: movq <t63_stack=stack64#26,>mulrax=int64#7
# asm 2: movq <t63_stack=200(%rsp),>mulrax=%rax
movq 200(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * t70_stack
# asm 1: mulq  <t70_stack=stack64#18
# asm 2: mulq  <t70_stack=136(%rsp)
mulq  136(%rsp)

# qhasm:   carry? xp3 += mulrax
# asm 1: add  <mulrax=int64#7,<xp3=int64#10
# asm 2: add  <mulrax=%rax,<xp3=%r12
add  %rax,%r12

# qhasm:   mulr31 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr31=int64#11
# asm 2: adc <mulrdx=%rdx,<mulr31=%r13
adc %rdx,%r13

# qhasm:   mulrax = t63_stack
# asm 1: movq <t63_stack=stack64#26,>mulrax=int64#7
# asm 2: movq <t63_stack=200(%rsp),>mulrax=%rax
movq 200(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * t71_stack
# asm 1: mulq  <t71_stack=stack64#19
# asm 2: mulq  <t71_stack=144(%rsp)
mulq  144(%rsp)

# qhasm:   carry? xp4 += mulrax
# asm 1: add  <mulrax=int64#7,<xp4=int64#12
# asm 2: add  <mulrax=%rax,<xp4=%r14
add  %rax,%r14

# qhasm:   mulr41 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr41=int64#13
# asm 2: adc <mulrdx=%rdx,<mulr41=%r15
adc %rdx,%r15

# qhasm:   mulrax = mulx319_stack
# asm 1: movq <mulx319_stack=stack64#8,>mulrax=int64#7
# asm 2: movq <mulx319_stack=56(%rsp),>mulrax=%rax
movq 56(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * t73_stack
# asm 1: mulq  <t73_stack=stack64#21
# asm 2: mulq  <t73_stack=160(%rsp)
mulq  160(%rsp)

# qhasm:   carry? xp1 += mulrax
# asm 1: add  <mulrax=int64#7,<xp1=int64#5
# asm 2: add  <mulrax=%rax,<xp1=%r8
add  %rax,%r8

# qhasm:   mulr11 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr11=int64#6
# asm 2: adc <mulrdx=%rdx,<mulr11=%r9
adc %rdx,%r9

# qhasm:   mulrax = mulx319_stack
# asm 1: movq <mulx319_stack=stack64#8,>mulrax=int64#7
# asm 2: movq <mulx319_stack=56(%rsp),>mulrax=%rax
movq 56(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * t74_stack
# asm 1: mulq  <t74_stack=stack64#22
# asm 2: mulq  <t74_stack=168(%rsp)
mulq  168(%rsp)

# qhasm:   carry? xp2 += mulrax
# asm 1: add  <mulrax=int64#7,<xp2=int64#8
# asm 2: add  <mulrax=%rax,<xp2=%r10
add  %rax,%r10

# qhasm:   mulr21 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr21=int64#9
# asm 2: adc <mulrdx=%rdx,<mulr21=%r11
adc %rdx,%r11

# qhasm:   mulrax = t64_stack
# asm 1: movq <t64_stack=stack64#27,>mulrax=int64#7
# asm 2: movq <t64_stack=208(%rsp),>mulrax=%rax
movq 208(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * t70_stack
# asm 1: mulq  <t70_stack=stack64#18
# asm 2: mulq  <t70_stack=136(%rsp)
mulq  136(%rsp)

# qhasm:   carry? xp4 += mulrax
# asm 1: add  <mulrax=int64#7,<xp4=int64#12
# asm 2: add  <mulrax=%rax,<xp4=%r14
add  %rax,%r14

# qhasm:   mulr41 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr41=int64#13
# asm 2: adc <mulrdx=%rdx,<mulr41=%r15
adc %rdx,%r15

# qhasm:   mulrax = mulx419_stack
# asm 1: movq <mulx419_stack=stack64#9,>mulrax=int64#7
# asm 2: movq <mulx419_stack=64(%rsp),>mulrax=%rax
movq 64(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * t72_stack
# asm 1: mulq  <t72_stack=stack64#20
# asm 2: mulq  <t72_stack=152(%rsp)
mulq  152(%rsp)

# qhasm:   carry? xp1 += mulrax
# asm 1: add  <mulrax=int64#7,<xp1=int64#5
# asm 2: add  <mulrax=%rax,<xp1=%r8
add  %rax,%r8

# qhasm:   mulr11 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr11=int64#6
# asm 2: adc <mulrdx=%rdx,<mulr11=%r9
adc %rdx,%r9

# qhasm:   mulrax = mulx419_stack
# asm 1: movq <mulx419_stack=stack64#9,>mulrax=int64#7
# asm 2: movq <mulx419_stack=64(%rsp),>mulrax=%rax
movq 64(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * t73_stack
# asm 1: mulq  <t73_stack=stack64#21
# asm 2: mulq  <t73_stack=160(%rsp)
mulq  160(%rsp)

# qhasm:   carry? xp2 += mulrax
# asm 1: add  <mulrax=int64#7,<xp2=int64#8
# asm 2: add  <mulrax=%rax,<xp2=%r10
add  %rax,%r10

# qhasm:   mulr21 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr21=int64#9
# asm 2: adc <mulrdx=%rdx,<mulr21=%r11
adc %rdx,%r11

# qhasm:   mulrax = mulx419_stack
# asm 1: movq <mulx419_stack=stack64#9,>mulrax=int64#7
# asm 2: movq <mulx419_stack=64(%rsp),>mulrax=%rax
movq 64(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * t74_stack
# asm 1: mulq  <t74_stack=stack64#22
# asm 2: mulq  <t74_stack=168(%rsp)
mulq  168(%rsp)

# qhasm:   carry? xp3 += mulrax
# asm 1: add  <mulrax=int64#7,<xp3=int64#10
# asm 2: add  <mulrax=%rax,<xp3=%r12
add  %rax,%r12

# qhasm:   mulr31 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr31=int64#11
# asm 2: adc <mulrdx=%rdx,<mulr31=%r13
adc %rdx,%r13

# qhasm:   mulredmask = *(uint64 *) &crypto_scalarmult_curve25519_amd64_51_REDMASK51
# asm 1: movq crypto_scalarmult_curve25519_amd64_51_REDMASK51,>mulredmask=int64#3
# asm 2: movq crypto_scalarmult_curve25519_amd64_51_REDMASK51,>mulredmask=%rdx
movq crypto_scalarmult_curve25519_amd64_51_REDMASK51,%rdx

# qhasm:   mulr01 = (mulr01.xp0) << 13
# asm 1: shld $13,<xp0=int64#2,<mulr01=int64#4
# asm 2: shld $13,<xp0=%rsi,<mulr01=%rcx
shld $13,%rsi,%rcx

# qhasm:   xp0 &= mulredmask
# asm 1: and  <mulredmask=int64#3,<xp0=int64#2
# asm 2: and  <mulredmask=%rdx,<xp0=%rsi
and  %rdx,%rsi

# qhasm:   mulr11 = (mulr11.xp1) << 13
# asm 1: shld $13,<xp1=int64#5,<mulr11=int64#6
# asm 2: shld $13,<xp1=%r8,<mulr11=%r9
shld $13,%r8,%r9

# qhasm:   xp1 &= mulredmask
# asm 1: and  <mulredmask=int64#3,<xp1=int64#5
# asm 2: and  <mulredmask=%rdx,<xp1=%r8
and  %rdx,%r8

# qhasm:   xp1 += mulr01
# asm 1: add  <mulr01=int64#4,<xp1=int64#5
# asm 2: add  <mulr01=%rcx,<xp1=%r8
add  %rcx,%r8

# qhasm:   mulr21 = (mulr21.xp2) << 13
# asm 1: shld $13,<xp2=int64#8,<mulr21=int64#9
# asm 2: shld $13,<xp2=%r10,<mulr21=%r11
shld $13,%r10,%r11

# qhasm:   xp2 &= mulredmask
# asm 1: and  <mulredmask=int64#3,<xp2=int64#8
# asm 2: and  <mulredmask=%rdx,<xp2=%r10
and  %rdx,%r10

# qhasm:   xp2 += mulr11
# asm 1: add  <mulr11=int64#6,<xp2=int64#8
# asm 2: add  <mulr11=%r9,<xp2=%r10
add  %r9,%r10

# qhasm:   mulr31 = (mulr31.xp3) << 13
# asm 1: shld $13,<xp3=int64#10,<mulr31=int64#11
# asm 2: shld $13,<xp3=%r12,<mulr31=%r13
shld $13,%r12,%r13

# qhasm:   xp3 &= mulredmask
# asm 1: and  <mulredmask=int64#3,<xp3=int64#10
# asm 2: and  <mulredmask=%rdx,<xp3=%r12
and  %rdx,%r12

# qhasm:   xp3 += mulr21
# asm 1: add  <mulr21=int64#9,<xp3=int64#10
# asm 2: add  <mulr21=%r11,<xp3=%r12
add  %r11,%r12

# qhasm:   mulr41 = (mulr41.xp4) << 13
# asm 1: shld $13,<xp4=int64#12,<mulr41=int64#13
# asm 2: shld $13,<xp4=%r14,<mulr41=%r15
shld $13,%r14,%r15

# qhasm:   xp4 &= mulredmask
# asm 1: and  <mulredmask=int64#3,<xp4=int64#12
# asm 2: and  <mulredmask=%rdx,<xp4=%r14
and  %rdx,%r14

# qhasm:   xp4 += mulr31
# asm 1: add  <mulr31=int64#11,<xp4=int64#12
# asm 2: add  <mulr31=%r13,<xp4=%r14
add  %r13,%r14

# qhasm:   mulr41 = mulr41 * 19
# asm 1: imulq  $19,<mulr41=int64#13,>mulr41=int64#4
# asm 2: imulq  $19,<mulr41=%r15,>mulr41=%rcx
imulq  $19,%r15,%rcx

# qhasm:   xp0 += mulr41
# asm 1: add  <mulr41=int64#4,<xp0=int64#2
# asm 2: add  <mulr41=%rcx,<xp0=%rsi
add  %rcx,%rsi

# qhasm:   mult = xp0
# asm 1: mov  <xp0=int64#2,>mult=int64#4
# asm 2: mov  <xp0=%rsi,>mult=%rcx
mov  %rsi,%rcx

# qhasm:   (uint64) mult >>= 51
# asm 1: shr  $51,<mult=int64#4
# asm 2: shr  $51,<mult=%rcx
shr  $51,%rcx

# qhasm:   mult += xp1
# asm 1: add  <xp1=int64#5,<mult=int64#4
# asm 2: add  <xp1=%r8,<mult=%rcx
add  %r8,%rcx

# qhasm:   xp1 = mult
# asm 1: mov  <mult=int64#4,>xp1=int64#5
# asm 2: mov  <mult=%rcx,>xp1=%r8
mov  %rcx,%r8

# qhasm:   (uint64) mult >>= 51
# asm 1: shr  $51,<mult=int64#4
# asm 2: shr  $51,<mult=%rcx
shr  $51,%rcx

# qhasm:   xp0 &= mulredmask
# asm 1: and  <mulredmask=int64#3,<xp0=int64#2
# asm 2: and  <mulredmask=%rdx,<xp0=%rsi
and  %rdx,%rsi

# qhasm:   mult += xp2
# asm 1: add  <xp2=int64#8,<mult=int64#4
# asm 2: add  <xp2=%r10,<mult=%rcx
add  %r10,%rcx

# qhasm:   xp2 = mult
# asm 1: mov  <mult=int64#4,>xp2=int64#6
# asm 2: mov  <mult=%rcx,>xp2=%r9
mov  %rcx,%r9

# qhasm:   (uint64) mult >>= 51
# asm 1: shr  $51,<mult=int64#4
# asm 2: shr  $51,<mult=%rcx
shr  $51,%rcx

# qhasm:   xp1 &= mulredmask
# asm 1: and  <mulredmask=int64#3,<xp1=int64#5
# asm 2: and  <mulredmask=%rdx,<xp1=%r8
and  %rdx,%r8

# qhasm:   mult += xp3
# asm 1: add  <xp3=int64#10,<mult=int64#4
# asm 2: add  <xp3=%r12,<mult=%rcx
add  %r12,%rcx

# qhasm:   xp3 = mult
# asm 1: mov  <mult=int64#4,>xp3=int64#7
# asm 2: mov  <mult=%rcx,>xp3=%rax
mov  %rcx,%rax

# qhasm:   (uint64) mult >>= 51
# asm 1: shr  $51,<mult=int64#4
# asm 2: shr  $51,<mult=%rcx
shr  $51,%rcx

# qhasm:   xp2 &= mulredmask
# asm 1: and  <mulredmask=int64#3,<xp2=int64#6
# asm 2: and  <mulredmask=%rdx,<xp2=%r9
and  %rdx,%r9

# qhasm:   mult += xp4
# asm 1: add  <xp4=int64#12,<mult=int64#4
# asm 2: add  <xp4=%r14,<mult=%rcx
add  %r14,%rcx

# qhasm:   xp4 = mult
# asm 1: mov  <mult=int64#4,>xp4=int64#8
# asm 2: mov  <mult=%rcx,>xp4=%r10
mov  %rcx,%r10

# qhasm:   (uint64) mult >>= 51
# asm 1: shr  $51,<mult=int64#4
# asm 2: shr  $51,<mult=%rcx
shr  $51,%rcx

# qhasm:   xp3 &= mulredmask
# asm 1: and  <mulredmask=int64#3,<xp3=int64#7
# asm 2: and  <mulredmask=%rdx,<xp3=%rax
and  %rdx,%rax

# qhasm:   mult *= 19
# asm 1: imulq  $19,<mult=int64#4,>mult=int64#4
# asm 2: imulq  $19,<mult=%rcx,>mult=%rcx
imulq  $19,%rcx,%rcx

# qhasm:   xp0 += mult
# asm 1: add  <mult=int64#4,<xp0=int64#2
# asm 2: add  <mult=%rcx,<xp0=%rsi
add  %rcx,%rsi

# qhasm:   xp4 &= mulredmask
# asm 1: and  <mulredmask=int64#3,<xp4=int64#8
# asm 2: and  <mulredmask=%rdx,<xp4=%r10
and  %rdx,%r10

# qhasm: *(uint64 *)(workp + 40) = xp0
# asm 1: movq   <xp0=int64#2,40(<workp=int64#1)
# asm 2: movq   <xp0=%rsi,40(<workp=%rdi)
movq   %rsi,40(%rdi)

# qhasm: *(uint64 *)(workp + 48) = xp1
# asm 1: movq   <xp1=int64#5,48(<workp=int64#1)
# asm 2: movq   <xp1=%r8,48(<workp=%rdi)
movq   %r8,48(%rdi)

# qhasm: *(uint64 *)(workp + 56) = xp2
# asm 1: movq   <xp2=int64#6,56(<workp=int64#1)
# asm 2: movq   <xp2=%r9,56(<workp=%rdi)
movq   %r9,56(%rdi)

# qhasm: *(uint64 *)(workp + 64) = xp3
# asm 1: movq   <xp3=int64#7,64(<workp=int64#1)
# asm 2: movq   <xp3=%rax,64(<workp=%rdi)
movq   %rax,64(%rdi)

# qhasm: *(uint64 *)(workp + 72) = xp4
# asm 1: movq   <xp4=int64#8,72(<workp=int64#1)
# asm 2: movq   <xp4=%r10,72(<workp=%rdi)
movq   %r10,72(%rdi)

# qhasm:   mul121666rax = t50_stack
# asm 1: movq <t50_stack=stack64#28,>mul121666rax=int64#7
# asm 2: movq <t50_stack=216(%rsp),>mul121666rax=%rax
movq 216(%rsp),%rax

# qhasm:   (uint128) mul121666rdx mul121666rax = mul121666rax * *(uint64 *) &crypto_scalarmult_curve25519_amd64_51_121666_213
mulq  crypto_scalarmult_curve25519_amd64_51_121666_213

# qhasm:   (uint64) mul121666rax >>= 13
# asm 1: shr  $13,<mul121666rax=int64#7
# asm 2: shr  $13,<mul121666rax=%rax
shr  $13,%rax

# qhasm:   zp0 = mul121666rax
# asm 1: mov  <mul121666rax=int64#7,>zp0=int64#2
# asm 2: mov  <mul121666rax=%rax,>zp0=%rsi
mov  %rax,%rsi

# qhasm:   zp1 = mul121666rdx
# asm 1: mov  <mul121666rdx=int64#3,>zp1=int64#4
# asm 2: mov  <mul121666rdx=%rdx,>zp1=%rcx
mov  %rdx,%rcx

# qhasm:   mul121666rax = t51_stack
# asm 1: movq <t51_stack=stack64#29,>mul121666rax=int64#7
# asm 2: movq <t51_stack=224(%rsp),>mul121666rax=%rax
movq 224(%rsp),%rax

# qhasm:   (uint128) mul121666rdx mul121666rax = mul121666rax * *(uint64 *) &crypto_scalarmult_curve25519_amd64_51_121666_213
mulq  crypto_scalarmult_curve25519_amd64_51_121666_213

# qhasm:   (uint64) mul121666rax >>= 13
# asm 1: shr  $13,<mul121666rax=int64#7
# asm 2: shr  $13,<mul121666rax=%rax
shr  $13,%rax

# qhasm:   zp1 += mul121666rax
# asm 1: add  <mul121666rax=int64#7,<zp1=int64#4
# asm 2: add  <mul121666rax=%rax,<zp1=%rcx
add  %rax,%rcx

# qhasm:   zp2 = mul121666rdx
# asm 1: mov  <mul121666rdx=int64#3,>zp2=int64#5
# asm 2: mov  <mul121666rdx=%rdx,>zp2=%r8
mov  %rdx,%r8

# qhasm:   mul121666rax = t52_stack
# asm 1: movq <t52_stack=stack64#30,>mul121666rax=int64#7
# asm 2: movq <t52_stack=232(%rsp),>mul121666rax=%rax
movq 232(%rsp),%rax

# qhasm:   (uint128) mul121666rdx mul121666rax = mul121666rax * *(uint64 *) &crypto_scalarmult_curve25519_amd64_51_121666_213
mulq  crypto_scalarmult_curve25519_amd64_51_121666_213

# qhasm:   (uint64) mul121666rax >>= 13
# asm 1: shr  $13,<mul121666rax=int64#7
# asm 2: shr  $13,<mul121666rax=%rax
shr  $13,%rax

# qhasm:   zp2 += mul121666rax
# asm 1: add  <mul121666rax=int64#7,<zp2=int64#5
# asm 2: add  <mul121666rax=%rax,<zp2=%r8
add  %rax,%r8

# qhasm:   zp3 = mul121666rdx
# asm 1: mov  <mul121666rdx=int64#3,>zp3=int64#6
# asm 2: mov  <mul121666rdx=%rdx,>zp3=%r9
mov  %rdx,%r9

# qhasm:   mul121666rax = t53_stack
# asm 1: movq <t53_stack=stack64#31,>mul121666rax=int64#7
# asm 2: movq <t53_stack=240(%rsp),>mul121666rax=%rax
movq 240(%rsp),%rax

# qhasm:   (uint128) mul121666rdx mul121666rax = mul121666rax * *(uint64 *) &crypto_scalarmult_curve25519_amd64_51_121666_213
mulq  crypto_scalarmult_curve25519_amd64_51_121666_213

# qhasm:   (uint64) mul121666rax >>= 13
# asm 1: shr  $13,<mul121666rax=int64#7
# asm 2: shr  $13,<mul121666rax=%rax
shr  $13,%rax

# qhasm:   zp3 += mul121666rax
# asm 1: add  <mul121666rax=int64#7,<zp3=int64#6
# asm 2: add  <mul121666rax=%rax,<zp3=%r9
add  %rax,%r9

# qhasm:   zp4 = mul121666rdx
# asm 1: mov  <mul121666rdx=int64#3,>zp4=int64#8
# asm 2: mov  <mul121666rdx=%rdx,>zp4=%r10
mov  %rdx,%r10

# qhasm:   mul121666rax = t54_stack
# asm 1: movq <t54_stack=stack64#32,>mul121666rax=int64#7
# asm 2: movq <t54_stack=248(%rsp),>mul121666rax=%rax
movq 248(%rsp),%rax

# qhasm:   (uint128) mul121666rdx mul121666rax = mul121666rax * *(uint64 *) &crypto_scalarmult_curve25519_amd64_51_121666_213
mulq  crypto_scalarmult_curve25519_amd64_51_121666_213

# qhasm:   (uint64) mul121666rax >>= 13
# asm 1: shr  $13,<mul121666rax=int64#7
# asm 2: shr  $13,<mul121666rax=%rax
shr  $13,%rax

# qhasm:   zp4 += mul121666rax
# asm 1: add  <mul121666rax=int64#7,<zp4=int64#8
# asm 2: add  <mul121666rax=%rax,<zp4=%r10
add  %rax,%r10

# qhasm:   mul121666rdx *= 19
# asm 1: imulq  $19,<mul121666rdx=int64#3,>mul121666rdx=int64#3
# asm 2: imulq  $19,<mul121666rdx=%rdx,>mul121666rdx=%rdx
imulq  $19,%rdx,%rdx

# qhasm:   zp0 += mul121666rdx
# asm 1: add  <mul121666rdx=int64#3,<zp0=int64#2
# asm 2: add  <mul121666rdx=%rdx,<zp0=%rsi
add  %rdx,%rsi

# qhasm: zp0 += t70_stack
# asm 1: addq <t70_stack=stack64#18,<zp0=int64#2
# asm 2: addq <t70_stack=136(%rsp),<zp0=%rsi
addq 136(%rsp),%rsi

# qhasm: zp1 += t71_stack
# asm 1: addq <t71_stack=stack64#19,<zp1=int64#4
# asm 2: addq <t71_stack=144(%rsp),<zp1=%rcx
addq 144(%rsp),%rcx

# qhasm: zp2 += t72_stack
# asm 1: addq <t72_stack=stack64#20,<zp2=int64#5
# asm 2: addq <t72_stack=152(%rsp),<zp2=%r8
addq 152(%rsp),%r8

# qhasm: zp3 += t73_stack
# asm 1: addq <t73_stack=stack64#21,<zp3=int64#6
# asm 2: addq <t73_stack=160(%rsp),<zp3=%r9
addq 160(%rsp),%r9

# qhasm: zp4 += t74_stack
# asm 1: addq <t74_stack=stack64#22,<zp4=int64#8
# asm 2: addq <t74_stack=168(%rsp),<zp4=%r10
addq 168(%rsp),%r10

# qhasm: *(uint64 *)(workp + 80) = zp0
# asm 1: movq   <zp0=int64#2,80(<workp=int64#1)
# asm 2: movq   <zp0=%rsi,80(<workp=%rdi)
movq   %rsi,80(%rdi)

# qhasm: *(uint64 *)(workp + 88) = zp1
# asm 1: movq   <zp1=int64#4,88(<workp=int64#1)
# asm 2: movq   <zp1=%rcx,88(<workp=%rdi)
movq   %rcx,88(%rdi)

# qhasm: *(uint64 *)(workp + 96) = zp2
# asm 1: movq   <zp2=int64#5,96(<workp=int64#1)
# asm 2: movq   <zp2=%r8,96(<workp=%rdi)
movq   %r8,96(%rdi)

# qhasm: *(uint64 *)(workp + 104) = zp3
# asm 1: movq   <zp3=int64#6,104(<workp=int64#1)
# asm 2: movq   <zp3=%r9,104(<workp=%rdi)
movq   %r9,104(%rdi)

# qhasm: *(uint64 *)(workp + 112) = zp4
# asm 1: movq   <zp4=int64#8,112(<workp=int64#1)
# asm 2: movq   <zp4=%r10,112(<workp=%rdi)
movq   %r10,112(%rdi)

# qhasm:   mulrax = *(uint64 *)(workp + 104)
# asm 1: movq   104(<workp=int64#1),>mulrax=int64#2
# asm 2: movq   104(<workp=%rdi),>mulrax=%rsi
movq   104(%rdi),%rsi

# qhasm:   mulrax *= 19
# asm 1: imulq  $19,<mulrax=int64#2,>mulrax=int64#7
# asm 2: imulq  $19,<mulrax=%rsi,>mulrax=%rax
imulq  $19,%rsi,%rax

# qhasm:   mulx319_stack = mulrax
# asm 1: movq <mulrax=int64#7,>mulx319_stack=stack64#8
# asm 2: movq <mulrax=%rax,>mulx319_stack=56(%rsp)
movq %rax,56(%rsp)

# qhasm:   (uint128) mulrdx mulrax = mulrax * t52_stack
# asm 1: mulq  <t52_stack=stack64#30
# asm 2: mulq  <t52_stack=232(%rsp)
mulq  232(%rsp)

# qhasm:   zp0 = mulrax
# asm 1: mov  <mulrax=int64#7,>zp0=int64#2
# asm 2: mov  <mulrax=%rax,>zp0=%rsi
mov  %rax,%rsi

# qhasm:   mulr01 = mulrdx
# asm 1: mov  <mulrdx=int64#3,>mulr01=int64#4
# asm 2: mov  <mulrdx=%rdx,>mulr01=%rcx
mov  %rdx,%rcx

# qhasm:   mulrax = *(uint64 *)(workp + 112)
# asm 1: movq   112(<workp=int64#1),>mulrax=int64#3
# asm 2: movq   112(<workp=%rdi),>mulrax=%rdx
movq   112(%rdi),%rdx

# qhasm:   mulrax *= 19
# asm 1: imulq  $19,<mulrax=int64#3,>mulrax=int64#7
# asm 2: imulq  $19,<mulrax=%rdx,>mulrax=%rax
imulq  $19,%rdx,%rax

# qhasm:   mulx419_stack = mulrax
# asm 1: movq <mulrax=int64#7,>mulx419_stack=stack64#9
# asm 2: movq <mulrax=%rax,>mulx419_stack=64(%rsp)
movq %rax,64(%rsp)

# qhasm:   (uint128) mulrdx mulrax = mulrax * t51_stack
# asm 1: mulq  <t51_stack=stack64#29
# asm 2: mulq  <t51_stack=224(%rsp)
mulq  224(%rsp)

# qhasm:   carry? zp0 += mulrax
# asm 1: add  <mulrax=int64#7,<zp0=int64#2
# asm 2: add  <mulrax=%rax,<zp0=%rsi
add  %rax,%rsi

# qhasm:   mulr01 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr01=int64#4
# asm 2: adc <mulrdx=%rdx,<mulr01=%rcx
adc %rdx,%rcx

# qhasm:   mulrax = *(uint64 *)(workp + 80)
# asm 1: movq   80(<workp=int64#1),>mulrax=int64#7
# asm 2: movq   80(<workp=%rdi),>mulrax=%rax
movq   80(%rdi),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * t50_stack
# asm 1: mulq  <t50_stack=stack64#28
# asm 2: mulq  <t50_stack=216(%rsp)
mulq  216(%rsp)

# qhasm:   carry? zp0 += mulrax
# asm 1: add  <mulrax=int64#7,<zp0=int64#2
# asm 2: add  <mulrax=%rax,<zp0=%rsi
add  %rax,%rsi

# qhasm:   mulr01 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr01=int64#4
# asm 2: adc <mulrdx=%rdx,<mulr01=%rcx
adc %rdx,%rcx

# qhasm:   mulrax = *(uint64 *)(workp + 80)
# asm 1: movq   80(<workp=int64#1),>mulrax=int64#7
# asm 2: movq   80(<workp=%rdi),>mulrax=%rax
movq   80(%rdi),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * t51_stack
# asm 1: mulq  <t51_stack=stack64#29
# asm 2: mulq  <t51_stack=224(%rsp)
mulq  224(%rsp)

# qhasm:   zp1 = mulrax
# asm 1: mov  <mulrax=int64#7,>zp1=int64#5
# asm 2: mov  <mulrax=%rax,>zp1=%r8
mov  %rax,%r8

# qhasm:   mulr11 = mulrdx
# asm 1: mov  <mulrdx=int64#3,>mulr11=int64#6
# asm 2: mov  <mulrdx=%rdx,>mulr11=%r9
mov  %rdx,%r9

# qhasm:   mulrax = *(uint64 *)(workp + 80)
# asm 1: movq   80(<workp=int64#1),>mulrax=int64#7
# asm 2: movq   80(<workp=%rdi),>mulrax=%rax
movq   80(%rdi),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * t52_stack
# asm 1: mulq  <t52_stack=stack64#30
# asm 2: mulq  <t52_stack=232(%rsp)
mulq  232(%rsp)

# qhasm:   zp2 = mulrax
# asm 1: mov  <mulrax=int64#7,>zp2=int64#8
# asm 2: mov  <mulrax=%rax,>zp2=%r10
mov  %rax,%r10

# qhasm:   mulr21 = mulrdx
# asm 1: mov  <mulrdx=int64#3,>mulr21=int64#9
# asm 2: mov  <mulrdx=%rdx,>mulr21=%r11
mov  %rdx,%r11

# qhasm:   mulrax = *(uint64 *)(workp + 80)
# asm 1: movq   80(<workp=int64#1),>mulrax=int64#7
# asm 2: movq   80(<workp=%rdi),>mulrax=%rax
movq   80(%rdi),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * t53_stack
# asm 1: mulq  <t53_stack=stack64#31
# asm 2: mulq  <t53_stack=240(%rsp)
mulq  240(%rsp)

# qhasm:   zp3 = mulrax
# asm 1: mov  <mulrax=int64#7,>zp3=int64#10
# asm 2: mov  <mulrax=%rax,>zp3=%r12
mov  %rax,%r12

# qhasm:   mulr31 = mulrdx
# asm 1: mov  <mulrdx=int64#3,>mulr31=int64#11
# asm 2: mov  <mulrdx=%rdx,>mulr31=%r13
mov  %rdx,%r13

# qhasm:   mulrax = *(uint64 *)(workp + 80)
# asm 1: movq   80(<workp=int64#1),>mulrax=int64#7
# asm 2: movq   80(<workp=%rdi),>mulrax=%rax
movq   80(%rdi),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * t54_stack
# asm 1: mulq  <t54_stack=stack64#32
# asm 2: mulq  <t54_stack=248(%rsp)
mulq  248(%rsp)

# qhasm:   zp4 = mulrax
# asm 1: mov  <mulrax=int64#7,>zp4=int64#12
# asm 2: mov  <mulrax=%rax,>zp4=%r14
mov  %rax,%r14

# qhasm:   mulr41 = mulrdx
# asm 1: mov  <mulrdx=int64#3,>mulr41=int64#13
# asm 2: mov  <mulrdx=%rdx,>mulr41=%r15
mov  %rdx,%r15

# qhasm:   mulrax = *(uint64 *)(workp + 88)
# asm 1: movq   88(<workp=int64#1),>mulrax=int64#7
# asm 2: movq   88(<workp=%rdi),>mulrax=%rax
movq   88(%rdi),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * t50_stack
# asm 1: mulq  <t50_stack=stack64#28
# asm 2: mulq  <t50_stack=216(%rsp)
mulq  216(%rsp)

# qhasm:   carry? zp1 += mulrax
# asm 1: add  <mulrax=int64#7,<zp1=int64#5
# asm 2: add  <mulrax=%rax,<zp1=%r8
add  %rax,%r8

# qhasm:   mulr11 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr11=int64#6
# asm 2: adc <mulrdx=%rdx,<mulr11=%r9
adc %rdx,%r9

# qhasm:   mulrax = *(uint64 *)(workp + 88)
# asm 1: movq   88(<workp=int64#1),>mulrax=int64#7
# asm 2: movq   88(<workp=%rdi),>mulrax=%rax
movq   88(%rdi),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * t51_stack
# asm 1: mulq  <t51_stack=stack64#29
# asm 2: mulq  <t51_stack=224(%rsp)
mulq  224(%rsp)

# qhasm:   carry? zp2 += mulrax
# asm 1: add  <mulrax=int64#7,<zp2=int64#8
# asm 2: add  <mulrax=%rax,<zp2=%r10
add  %rax,%r10

# qhasm:   mulr21 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr21=int64#9
# asm 2: adc <mulrdx=%rdx,<mulr21=%r11
adc %rdx,%r11

# qhasm:   mulrax = *(uint64 *)(workp + 88)
# asm 1: movq   88(<workp=int64#1),>mulrax=int64#7
# asm 2: movq   88(<workp=%rdi),>mulrax=%rax
movq   88(%rdi),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * t52_stack
# asm 1: mulq  <t52_stack=stack64#30
# asm 2: mulq  <t52_stack=232(%rsp)
mulq  232(%rsp)

# qhasm:   carry? zp3 += mulrax
# asm 1: add  <mulrax=int64#7,<zp3=int64#10
# asm 2: add  <mulrax=%rax,<zp3=%r12
add  %rax,%r12

# qhasm:   mulr31 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr31=int64#11
# asm 2: adc <mulrdx=%rdx,<mulr31=%r13
adc %rdx,%r13

# qhasm:   mulrax = *(uint64 *)(workp + 88)
# asm 1: movq   88(<workp=int64#1),>mulrax=int64#7
# asm 2: movq   88(<workp=%rdi),>mulrax=%rax
movq   88(%rdi),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * t53_stack
# asm 1: mulq  <t53_stack=stack64#31
# asm 2: mulq  <t53_stack=240(%rsp)
mulq  240(%rsp)

# qhasm:   carry? zp4 += mulrax
# asm 1: add  <mulrax=int64#7,<zp4=int64#12
# asm 2: add  <mulrax=%rax,<zp4=%r14
add  %rax,%r14

# qhasm:   mulr41 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr41=int64#13
# asm 2: adc <mulrdx=%rdx,<mulr41=%r15
adc %rdx,%r15

# qhasm:   mulrax = *(uint64 *)(workp + 88)
# asm 1: movq   88(<workp=int64#1),>mulrax=int64#3
# asm 2: movq   88(<workp=%rdi),>mulrax=%rdx
movq   88(%rdi),%rdx

# qhasm:   mulrax *= 19
# asm 1: imulq  $19,<mulrax=int64#3,>mulrax=int64#7
# asm 2: imulq  $19,<mulrax=%rdx,>mulrax=%rax
imulq  $19,%rdx,%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * t54_stack
# asm 1: mulq  <t54_stack=stack64#32
# asm 2: mulq  <t54_stack=248(%rsp)
mulq  248(%rsp)

# qhasm:   carry? zp0 += mulrax
# asm 1: add  <mulrax=int64#7,<zp0=int64#2
# asm 2: add  <mulrax=%rax,<zp0=%rsi
add  %rax,%rsi

# qhasm:   mulr01 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr01=int64#4
# asm 2: adc <mulrdx=%rdx,<mulr01=%rcx
adc %rdx,%rcx

# qhasm:   mulrax = *(uint64 *)(workp + 96)
# asm 1: movq   96(<workp=int64#1),>mulrax=int64#7
# asm 2: movq   96(<workp=%rdi),>mulrax=%rax
movq   96(%rdi),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * t50_stack
# asm 1: mulq  <t50_stack=stack64#28
# asm 2: mulq  <t50_stack=216(%rsp)
mulq  216(%rsp)

# qhasm:   carry? zp2 += mulrax
# asm 1: add  <mulrax=int64#7,<zp2=int64#8
# asm 2: add  <mulrax=%rax,<zp2=%r10
add  %rax,%r10

# qhasm:   mulr21 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr21=int64#9
# asm 2: adc <mulrdx=%rdx,<mulr21=%r11
adc %rdx,%r11

# qhasm:   mulrax = *(uint64 *)(workp + 96)
# asm 1: movq   96(<workp=int64#1),>mulrax=int64#7
# asm 2: movq   96(<workp=%rdi),>mulrax=%rax
movq   96(%rdi),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * t51_stack
# asm 1: mulq  <t51_stack=stack64#29
# asm 2: mulq  <t51_stack=224(%rsp)
mulq  224(%rsp)

# qhasm:   carry? zp3 += mulrax
# asm 1: add  <mulrax=int64#7,<zp3=int64#10
# asm 2: add  <mulrax=%rax,<zp3=%r12
add  %rax,%r12

# qhasm:   mulr31 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr31=int64#11
# asm 2: adc <mulrdx=%rdx,<mulr31=%r13
adc %rdx,%r13

# qhasm:   mulrax = *(uint64 *)(workp + 96)
# asm 1: movq   96(<workp=int64#1),>mulrax=int64#7
# asm 2: movq   96(<workp=%rdi),>mulrax=%rax
movq   96(%rdi),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * t52_stack
# asm 1: mulq  <t52_stack=stack64#30
# asm 2: mulq  <t52_stack=232(%rsp)
mulq  232(%rsp)

# qhasm:   carry? zp4 += mulrax
# asm 1: add  <mulrax=int64#7,<zp4=int64#12
# asm 2: add  <mulrax=%rax,<zp4=%r14
add  %rax,%r14

# qhasm:   mulr41 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr41=int64#13
# asm 2: adc <mulrdx=%rdx,<mulr41=%r15
adc %rdx,%r15

# qhasm:   mulrax = *(uint64 *)(workp + 96)
# asm 1: movq   96(<workp=int64#1),>mulrax=int64#3
# asm 2: movq   96(<workp=%rdi),>mulrax=%rdx
movq   96(%rdi),%rdx

# qhasm:   mulrax *= 19
# asm 1: imulq  $19,<mulrax=int64#3,>mulrax=int64#7
# asm 2: imulq  $19,<mulrax=%rdx,>mulrax=%rax
imulq  $19,%rdx,%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * t53_stack
# asm 1: mulq  <t53_stack=stack64#31
# asm 2: mulq  <t53_stack=240(%rsp)
mulq  240(%rsp)

# qhasm:   carry? zp0 += mulrax
# asm 1: add  <mulrax=int64#7,<zp0=int64#2
# asm 2: add  <mulrax=%rax,<zp0=%rsi
add  %rax,%rsi

# qhasm:   mulr01 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr01=int64#4
# asm 2: adc <mulrdx=%rdx,<mulr01=%rcx
adc %rdx,%rcx

# qhasm:   mulrax = *(uint64 *)(workp + 96)
# asm 1: movq   96(<workp=int64#1),>mulrax=int64#3
# asm 2: movq   96(<workp=%rdi),>mulrax=%rdx
movq   96(%rdi),%rdx

# qhasm:   mulrax *= 19
# asm 1: imulq  $19,<mulrax=int64#3,>mulrax=int64#7
# asm 2: imulq  $19,<mulrax=%rdx,>mulrax=%rax
imulq  $19,%rdx,%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * t54_stack
# asm 1: mulq  <t54_stack=stack64#32
# asm 2: mulq  <t54_stack=248(%rsp)
mulq  248(%rsp)

# qhasm:   carry? zp1 += mulrax
# asm 1: add  <mulrax=int64#7,<zp1=int64#5
# asm 2: add  <mulrax=%rax,<zp1=%r8
add  %rax,%r8

# qhasm:   mulr11 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr11=int64#6
# asm 2: adc <mulrdx=%rdx,<mulr11=%r9
adc %rdx,%r9

# qhasm:   mulrax = *(uint64 *)(workp + 104)
# asm 1: movq   104(<workp=int64#1),>mulrax=int64#7
# asm 2: movq   104(<workp=%rdi),>mulrax=%rax
movq   104(%rdi),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * t50_stack
# asm 1: mulq  <t50_stack=stack64#28
# asm 2: mulq  <t50_stack=216(%rsp)
mulq  216(%rsp)

# qhasm:   carry? zp3 += mulrax
# asm 1: add  <mulrax=int64#7,<zp3=int64#10
# asm 2: add  <mulrax=%rax,<zp3=%r12
add  %rax,%r12

# qhasm:   mulr31 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr31=int64#11
# asm 2: adc <mulrdx=%rdx,<mulr31=%r13
adc %rdx,%r13

# qhasm:   mulrax = *(uint64 *)(workp + 104)
# asm 1: movq   104(<workp=int64#1),>mulrax=int64#7
# asm 2: movq   104(<workp=%rdi),>mulrax=%rax
movq   104(%rdi),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * t51_stack
# asm 1: mulq  <t51_stack=stack64#29
# asm 2: mulq  <t51_stack=224(%rsp)
mulq  224(%rsp)

# qhasm:   carry? zp4 += mulrax
# asm 1: add  <mulrax=int64#7,<zp4=int64#12
# asm 2: add  <mulrax=%rax,<zp4=%r14
add  %rax,%r14

# qhasm:   mulr41 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr41=int64#13
# asm 2: adc <mulrdx=%rdx,<mulr41=%r15
adc %rdx,%r15

# qhasm:   mulrax = mulx319_stack
# asm 1: movq <mulx319_stack=stack64#8,>mulrax=int64#7
# asm 2: movq <mulx319_stack=56(%rsp),>mulrax=%rax
movq 56(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * t53_stack
# asm 1: mulq  <t53_stack=stack64#31
# asm 2: mulq  <t53_stack=240(%rsp)
mulq  240(%rsp)

# qhasm:   carry? zp1 += mulrax
# asm 1: add  <mulrax=int64#7,<zp1=int64#5
# asm 2: add  <mulrax=%rax,<zp1=%r8
add  %rax,%r8

# qhasm:   mulr11 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr11=int64#6
# asm 2: adc <mulrdx=%rdx,<mulr11=%r9
adc %rdx,%r9

# qhasm:   mulrax = mulx319_stack
# asm 1: movq <mulx319_stack=stack64#8,>mulrax=int64#7
# asm 2: movq <mulx319_stack=56(%rsp),>mulrax=%rax
movq 56(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * t54_stack
# asm 1: mulq  <t54_stack=stack64#32
# asm 2: mulq  <t54_stack=248(%rsp)
mulq  248(%rsp)

# qhasm:   carry? zp2 += mulrax
# asm 1: add  <mulrax=int64#7,<zp2=int64#8
# asm 2: add  <mulrax=%rax,<zp2=%r10
add  %rax,%r10

# qhasm:   mulr21 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr21=int64#9
# asm 2: adc <mulrdx=%rdx,<mulr21=%r11
adc %rdx,%r11

# qhasm:   mulrax = *(uint64 *)(workp + 112)
# asm 1: movq   112(<workp=int64#1),>mulrax=int64#7
# asm 2: movq   112(<workp=%rdi),>mulrax=%rax
movq   112(%rdi),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * t50_stack
# asm 1: mulq  <t50_stack=stack64#28
# asm 2: mulq  <t50_stack=216(%rsp)
mulq  216(%rsp)

# qhasm:   carry? zp4 += mulrax
# asm 1: add  <mulrax=int64#7,<zp4=int64#12
# asm 2: add  <mulrax=%rax,<zp4=%r14
add  %rax,%r14

# qhasm:   mulr41 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr41=int64#13
# asm 2: adc <mulrdx=%rdx,<mulr41=%r15
adc %rdx,%r15

# qhasm:   mulrax = mulx419_stack
# asm 1: movq <mulx419_stack=stack64#9,>mulrax=int64#7
# asm 2: movq <mulx419_stack=64(%rsp),>mulrax=%rax
movq 64(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * t52_stack
# asm 1: mulq  <t52_stack=stack64#30
# asm 2: mulq  <t52_stack=232(%rsp)
mulq  232(%rsp)

# qhasm:   carry? zp1 += mulrax
# asm 1: add  <mulrax=int64#7,<zp1=int64#5
# asm 2: add  <mulrax=%rax,<zp1=%r8
add  %rax,%r8

# qhasm:   mulr11 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr11=int64#6
# asm 2: adc <mulrdx=%rdx,<mulr11=%r9
adc %rdx,%r9

# qhasm:   mulrax = mulx419_stack
# asm 1: movq <mulx419_stack=stack64#9,>mulrax=int64#7
# asm 2: movq <mulx419_stack=64(%rsp),>mulrax=%rax
movq 64(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * t53_stack
# asm 1: mulq  <t53_stack=stack64#31
# asm 2: mulq  <t53_stack=240(%rsp)
mulq  240(%rsp)

# qhasm:   carry? zp2 += mulrax
# asm 1: add  <mulrax=int64#7,<zp2=int64#8
# asm 2: add  <mulrax=%rax,<zp2=%r10
add  %rax,%r10

# qhasm:   mulr21 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr21=int64#9
# asm 2: adc <mulrdx=%rdx,<mulr21=%r11
adc %rdx,%r11

# qhasm:   mulrax = mulx419_stack
# asm 1: movq <mulx419_stack=stack64#9,>mulrax=int64#7
# asm 2: movq <mulx419_stack=64(%rsp),>mulrax=%rax
movq 64(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * t54_stack
# asm 1: mulq  <t54_stack=stack64#32
# asm 2: mulq  <t54_stack=248(%rsp)
mulq  248(%rsp)

# qhasm:   carry? zp3 += mulrax
# asm 1: add  <mulrax=int64#7,<zp3=int64#10
# asm 2: add  <mulrax=%rax,<zp3=%r12
add  %rax,%r12

# qhasm:   mulr31 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr31=int64#11
# asm 2: adc <mulrdx=%rdx,<mulr31=%r13
adc %rdx,%r13

# qhasm:   mulredmask = *(uint64 *) &crypto_scalarmult_curve25519_amd64_51_REDMASK51
# asm 1: movq crypto_scalarmult_curve25519_amd64_51_REDMASK51,>mulredmask=int64#3
# asm 2: movq crypto_scalarmult_curve25519_amd64_51_REDMASK51,>mulredmask=%rdx
movq crypto_scalarmult_curve25519_amd64_51_REDMASK51,%rdx

# qhasm:   mulr01 = (mulr01.zp0) << 13
# asm 1: shld $13,<zp0=int64#2,<mulr01=int64#4
# asm 2: shld $13,<zp0=%rsi,<mulr01=%rcx
shld $13,%rsi,%rcx

# qhasm:   zp0 &= mulredmask
# asm 1: and  <mulredmask=int64#3,<zp0=int64#2
# asm 2: and  <mulredmask=%rdx,<zp0=%rsi
and  %rdx,%rsi

# qhasm:   mulr11 = (mulr11.zp1) << 13
# asm 1: shld $13,<zp1=int64#5,<mulr11=int64#6
# asm 2: shld $13,<zp1=%r8,<mulr11=%r9
shld $13,%r8,%r9

# qhasm:   zp1 &= mulredmask
# asm 1: and  <mulredmask=int64#3,<zp1=int64#5
# asm 2: and  <mulredmask=%rdx,<zp1=%r8
and  %rdx,%r8

# qhasm:   zp1 += mulr01
# asm 1: add  <mulr01=int64#4,<zp1=int64#5
# asm 2: add  <mulr01=%rcx,<zp1=%r8
add  %rcx,%r8

# qhasm:   mulr21 = (mulr21.zp2) << 13
# asm 1: shld $13,<zp2=int64#8,<mulr21=int64#9
# asm 2: shld $13,<zp2=%r10,<mulr21=%r11
shld $13,%r10,%r11

# qhasm:   zp2 &= mulredmask
# asm 1: and  <mulredmask=int64#3,<zp2=int64#8
# asm 2: and  <mulredmask=%rdx,<zp2=%r10
and  %rdx,%r10

# qhasm:   zp2 += mulr11
# asm 1: add  <mulr11=int64#6,<zp2=int64#8
# asm 2: add  <mulr11=%r9,<zp2=%r10
add  %r9,%r10

# qhasm:   mulr31 = (mulr31.zp3) << 13
# asm 1: shld $13,<zp3=int64#10,<mulr31=int64#11
# asm 2: shld $13,<zp3=%r12,<mulr31=%r13
shld $13,%r12,%r13

# qhasm:   zp3 &= mulredmask
# asm 1: and  <mulredmask=int64#3,<zp3=int64#10
# asm 2: and  <mulredmask=%rdx,<zp3=%r12
and  %rdx,%r12

# qhasm:   zp3 += mulr21
# asm 1: add  <mulr21=int64#9,<zp3=int64#10
# asm 2: add  <mulr21=%r11,<zp3=%r12
add  %r11,%r12

# qhasm:   mulr41 = (mulr41.zp4) << 13
# asm 1: shld $13,<zp4=int64#12,<mulr41=int64#13
# asm 2: shld $13,<zp4=%r14,<mulr41=%r15
shld $13,%r14,%r15

# qhasm:   zp4 &= mulredmask
# asm 1: and  <mulredmask=int64#3,<zp4=int64#12
# asm 2: and  <mulredmask=%rdx,<zp4=%r14
and  %rdx,%r14

# qhasm:   zp4 += mulr31
# asm 1: add  <mulr31=int64#11,<zp4=int64#12
# asm 2: add  <mulr31=%r13,<zp4=%r14
add  %r13,%r14

# qhasm:   mulr41 = mulr41 * 19
# asm 1: imulq  $19,<mulr41=int64#13,>mulr41=int64#4
# asm 2: imulq  $19,<mulr41=%r15,>mulr41=%rcx
imulq  $19,%r15,%rcx

# qhasm:   zp0 += mulr41
# asm 1: add  <mulr41=int64#4,<zp0=int64#2
# asm 2: add  <mulr41=%rcx,<zp0=%rsi
add  %rcx,%rsi

# qhasm:   mult = zp0
# asm 1: mov  <zp0=int64#2,>mult=int64#4
# asm 2: mov  <zp0=%rsi,>mult=%rcx
mov  %rsi,%rcx

# qhasm:   (uint64) mult >>= 51
# asm 1: shr  $51,<mult=int64#4
# asm 2: shr  $51,<mult=%rcx
shr  $51,%rcx

# qhasm:   mult += zp1
# asm 1: add  <zp1=int64#5,<mult=int64#4
# asm 2: add  <zp1=%r8,<mult=%rcx
add  %r8,%rcx

# qhasm:   zp1 = mult
# asm 1: mov  <mult=int64#4,>zp1=int64#5
# asm 2: mov  <mult=%rcx,>zp1=%r8
mov  %rcx,%r8

# qhasm:   (uint64) mult >>= 51
# asm 1: shr  $51,<mult=int64#4
# asm 2: shr  $51,<mult=%rcx
shr  $51,%rcx

# qhasm:   zp0 &= mulredmask
# asm 1: and  <mulredmask=int64#3,<zp0=int64#2
# asm 2: and  <mulredmask=%rdx,<zp0=%rsi
and  %rdx,%rsi

# qhasm:   mult += zp2
# asm 1: add  <zp2=int64#8,<mult=int64#4
# asm 2: add  <zp2=%r10,<mult=%rcx
add  %r10,%rcx

# qhasm:   zp2 = mult
# asm 1: mov  <mult=int64#4,>zp2=int64#6
# asm 2: mov  <mult=%rcx,>zp2=%r9
mov  %rcx,%r9

# qhasm:   (uint64) mult >>= 51
# asm 1: shr  $51,<mult=int64#4
# asm 2: shr  $51,<mult=%rcx
shr  $51,%rcx

# qhasm:   zp1 &= mulredmask
# asm 1: and  <mulredmask=int64#3,<zp1=int64#5
# asm 2: and  <mulredmask=%rdx,<zp1=%r8
and  %rdx,%r8

# qhasm:   mult += zp3
# asm 1: add  <zp3=int64#10,<mult=int64#4
# asm 2: add  <zp3=%r12,<mult=%rcx
add  %r12,%rcx

# qhasm:   zp3 = mult
# asm 1: mov  <mult=int64#4,>zp3=int64#7
# asm 2: mov  <mult=%rcx,>zp3=%rax
mov  %rcx,%rax

# qhasm:   (uint64) mult >>= 51
# asm 1: shr  $51,<mult=int64#4
# asm 2: shr  $51,<mult=%rcx
shr  $51,%rcx

# qhasm:   zp2 &= mulredmask
# asm 1: and  <mulredmask=int64#3,<zp2=int64#6
# asm 2: and  <mulredmask=%rdx,<zp2=%r9
and  %rdx,%r9

# qhasm:   mult += zp4
# asm 1: add  <zp4=int64#12,<mult=int64#4
# asm 2: add  <zp4=%r14,<mult=%rcx
add  %r14,%rcx

# qhasm:   zp4 = mult
# asm 1: mov  <mult=int64#4,>zp4=int64#8
# asm 2: mov  <mult=%rcx,>zp4=%r10
mov  %rcx,%r10

# qhasm:   (uint64) mult >>= 51
# asm 1: shr  $51,<mult=int64#4
# asm 2: shr  $51,<mult=%rcx
shr  $51,%rcx

# qhasm:   zp3 &= mulredmask
# asm 1: and  <mulredmask=int64#3,<zp3=int64#7
# asm 2: and  <mulredmask=%rdx,<zp3=%rax
and  %rdx,%rax

# qhasm:   mult *= 19
# asm 1: imulq  $19,<mult=int64#4,>mult=int64#4
# asm 2: imulq  $19,<mult=%rcx,>mult=%rcx
imulq  $19,%rcx,%rcx

# qhasm:   zp0 += mult
# asm 1: add  <mult=int64#4,<zp0=int64#2
# asm 2: add  <mult=%rcx,<zp0=%rsi
add  %rcx,%rsi

# qhasm:   zp4 &= mulredmask
# asm 1: and  <mulredmask=int64#3,<zp4=int64#8
# asm 2: and  <mulredmask=%rdx,<zp4=%r10
and  %rdx,%r10

# qhasm: *(uint64 *)(workp + 80) = zp0
# asm 1: movq   <zp0=int64#2,80(<workp=int64#1)
# asm 2: movq   <zp0=%rsi,80(<workp=%rdi)
movq   %rsi,80(%rdi)

# qhasm: *(uint64 *)(workp + 88) = zp1
# asm 1: movq   <zp1=int64#5,88(<workp=int64#1)
# asm 2: movq   <zp1=%r8,88(<workp=%rdi)
movq   %r8,88(%rdi)

# qhasm: *(uint64 *)(workp + 96) = zp2
# asm 1: movq   <zp2=int64#6,96(<workp=int64#1)
# asm 2: movq   <zp2=%r9,96(<workp=%rdi)
movq   %r9,96(%rdi)

# qhasm: *(uint64 *)(workp + 104) = zp3
# asm 1: movq   <zp3=int64#7,104(<workp=int64#1)
# asm 2: movq   <zp3=%rax,104(<workp=%rdi)
movq   %rax,104(%rdi)

# qhasm: *(uint64 *)(workp + 112) = zp4
# asm 1: movq   <zp4=int64#8,112(<workp=int64#1)
# asm 2: movq   <zp4=%r10,112(<workp=%rdi)
movq   %r10,112(%rdi)

# qhasm:   caller1 = caller1_stack
# asm 1: movq <caller1_stack=stack64#1,>caller1=int64#9
# asm 2: movq <caller1_stack=0(%rsp),>caller1=%r11
movq 0(%rsp),%r11

# qhasm:   caller2 = caller2_stack
# asm 1: movq <caller2_stack=stack64#2,>caller2=int64#10
# asm 2: movq <caller2_stack=8(%rsp),>caller2=%r12
movq 8(%rsp),%r12

# qhasm:   caller3 = caller3_stack
# asm 1: movq <caller3_stack=stack64#3,>caller3=int64#11
# asm 2: movq <caller3_stack=16(%rsp),>caller3=%r13
movq 16(%rsp),%r13

# qhasm:   caller4 = caller4_stack
# asm 1: movq <caller4_stack=stack64#4,>caller4=int64#12
# asm 2: movq <caller4_stack=24(%rsp),>caller4=%r14
movq 24(%rsp),%r14

# qhasm:   caller5 = caller5_stack
# asm 1: movq <caller5_stack=stack64#5,>caller5=int64#13
# asm 2: movq <caller5_stack=32(%rsp),>caller5=%r15
movq 32(%rsp),%r15

# qhasm:   caller6 = caller6_stack
# asm 1: movq <caller6_stack=stack64#6,>caller6=int64#14
# asm 2: movq <caller6_stack=40(%rsp),>caller6=%rbx
movq 40(%rsp),%rbx

# qhasm:   caller7 = caller7_stack
# asm 1: movq <caller7_stack=stack64#7,>caller7=int64#15
# asm 2: movq <caller7_stack=48(%rsp),>caller7=%rbp
movq 48(%rsp),%rbp

# qhasm: leave
add %r11,%rsp
mov %rdi,%rax
mov %rsi,%rdx
ret
