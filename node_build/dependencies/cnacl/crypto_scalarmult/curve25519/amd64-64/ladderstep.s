
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

# qhasm: stack64 t10_stack

# qhasm: stack64 t11_stack

# qhasm: stack64 t12_stack

# qhasm: stack64 t13_stack

# qhasm: int64 t20

# qhasm: int64 t21

# qhasm: int64 t22

# qhasm: int64 t23

# qhasm: stack64 t20_stack

# qhasm: stack64 t21_stack

# qhasm: stack64 t22_stack

# qhasm: stack64 t23_stack

# qhasm: int64 t30

# qhasm: int64 t31

# qhasm: int64 t32

# qhasm: int64 t33

# qhasm: stack64 t30_stack

# qhasm: stack64 t31_stack

# qhasm: stack64 t32_stack

# qhasm: stack64 t33_stack

# qhasm: int64 t40

# qhasm: int64 t41

# qhasm: int64 t42

# qhasm: int64 t43

# qhasm: stack64 t40_stack

# qhasm: stack64 t41_stack

# qhasm: stack64 t42_stack

# qhasm: stack64 t43_stack

# qhasm: int64 t50

# qhasm: int64 t51

# qhasm: int64 t52

# qhasm: int64 t53

# qhasm: stack64 t50_stack

# qhasm: stack64 t51_stack

# qhasm: stack64 t52_stack

# qhasm: stack64 t53_stack

# qhasm: int64 t60

# qhasm: int64 t61

# qhasm: int64 t62

# qhasm: int64 t63

# qhasm: stack64 t60_stack

# qhasm: stack64 t61_stack

# qhasm: stack64 t62_stack

# qhasm: stack64 t63_stack

# qhasm: int64 t70

# qhasm: int64 t71

# qhasm: int64 t72

# qhasm: int64 t73

# qhasm: stack64 t70_stack

# qhasm: stack64 t71_stack

# qhasm: stack64 t72_stack

# qhasm: stack64 t73_stack

# qhasm: int64 t80

# qhasm: int64 t81

# qhasm: int64 t82

# qhasm: int64 t83

# qhasm: stack64 t80_stack

# qhasm: stack64 t81_stack

# qhasm: stack64 t82_stack

# qhasm: stack64 t83_stack

# qhasm: int64 t90

# qhasm: int64 t91

# qhasm: int64 t92

# qhasm: int64 t93

# qhasm: stack64 t90_stack

# qhasm: stack64 t91_stack

# qhasm: stack64 t92_stack

# qhasm: stack64 t93_stack

# qhasm: int64 xp0

# qhasm: int64 xp1

# qhasm: int64 xp2

# qhasm: int64 xp3

# qhasm: int64 zp0

# qhasm: int64 zp1

# qhasm: int64 zp2

# qhasm: int64 zp3

# qhasm: int64 xq0

# qhasm: int64 xq1

# qhasm: int64 xq2

# qhasm: int64 xq3

# qhasm: int64 zq0

# qhasm: int64 zq1

# qhasm: int64 zq2

# qhasm: int64 zq3

# qhasm: int64 mulr4

# qhasm: int64 mulr5

# qhasm: int64 mulr6

# qhasm: int64 mulr7

# qhasm: int64 mulr8

# qhasm: int64 mulrax

# qhasm: int64 mulrdx

# qhasm: int64 mulx0

# qhasm: int64 mulx1

# qhasm: int64 mulx2

# qhasm: int64 mulx3

# qhasm: int64 mulc

# qhasm: int64 mulzero

# qhasm: int64 muli38

# qhasm: int64 squarer4

# qhasm: int64 squarer5

# qhasm: int64 squarer6

# qhasm: int64 squarer7

# qhasm: int64 squarer8

# qhasm: int64 squarerax

# qhasm: int64 squarerdx

# qhasm: int64 squaret1

# qhasm: int64 squaret2

# qhasm: int64 squaret3

# qhasm: int64 squarec

# qhasm: int64 squarezero

# qhasm: int64 squarei38

# qhasm: int64 mul121666rax

# qhasm: int64 mul121666rdx

# qhasm: int64 mul121666r4

# qhasm: int64 mul121666t1

# qhasm: int64 mul121666t2

# qhasm: int64 mul121666t3

# qhasm: int64 addt0

# qhasm: int64 addt1

# qhasm: int64 subt0

# qhasm: int64 subt1

# qhasm: enter crypto_scalarmult_curve25519_amd64_64_ladderstep
.text
.p2align 5
.globl _crypto_scalarmult_curve25519_amd64_64_ladderstep
.globl crypto_scalarmult_curve25519_amd64_64_ladderstep
_crypto_scalarmult_curve25519_amd64_64_ladderstep:
crypto_scalarmult_curve25519_amd64_64_ladderstep:
mov %rsp,%r11
and $31,%r11
add $288,%r11
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

# qhasm: t10 = *(uint64 *)(workp + 32)
# asm 1: movq   32(<workp=int64#1),>t10=int64#2
# asm 2: movq   32(<workp=%rdi),>t10=%rsi
movq   32(%rdi),%rsi

# qhasm: t11 = *(uint64 *)(workp + 40)
# asm 1: movq   40(<workp=int64#1),>t11=int64#3
# asm 2: movq   40(<workp=%rdi),>t11=%rdx
movq   40(%rdi),%rdx

# qhasm: t12 = *(uint64 *)(workp + 48)
# asm 1: movq   48(<workp=int64#1),>t12=int64#4
# asm 2: movq   48(<workp=%rdi),>t12=%rcx
movq   48(%rdi),%rcx

# qhasm: t13 = *(uint64 *)(workp + 56)
# asm 1: movq   56(<workp=int64#1),>t13=int64#5
# asm 2: movq   56(<workp=%rdi),>t13=%r8
movq   56(%rdi),%r8

# qhasm: t20 = t10
# asm 1: mov  <t10=int64#2,>t20=int64#6
# asm 2: mov  <t10=%rsi,>t20=%r9
mov  %rsi,%r9

# qhasm: t21 = t11
# asm 1: mov  <t11=int64#3,>t21=int64#7
# asm 2: mov  <t11=%rdx,>t21=%rax
mov  %rdx,%rax

# qhasm: t22 = t12
# asm 1: mov  <t12=int64#4,>t22=int64#8
# asm 2: mov  <t12=%rcx,>t22=%r10
mov  %rcx,%r10

# qhasm: t23 = t13
# asm 1: mov  <t13=int64#5,>t23=int64#9
# asm 2: mov  <t13=%r8,>t23=%r11
mov  %r8,%r11

# qhasm:   carry? t10 += *(uint64 *)(workp + 64) 
# asm 1: addq 64(<workp=int64#1),<t10=int64#2
# asm 2: addq 64(<workp=%rdi),<t10=%rsi
addq 64(%rdi),%rsi

# qhasm:   carry? t11 += *(uint64 *)(workp + 72) + carry
# asm 1: adcq 72(<workp=int64#1),<t11=int64#3
# asm 2: adcq 72(<workp=%rdi),<t11=%rdx
adcq 72(%rdi),%rdx

# qhasm:   carry? t12 += *(uint64 *)(workp + 80) + carry
# asm 1: adcq 80(<workp=int64#1),<t12=int64#4
# asm 2: adcq 80(<workp=%rdi),<t12=%rcx
adcq 80(%rdi),%rcx

# qhasm:   carry? t13 += *(uint64 *)(workp + 88) + carry
# asm 1: adcq 88(<workp=int64#1),<t13=int64#5
# asm 2: adcq 88(<workp=%rdi),<t13=%r8
adcq 88(%rdi),%r8

# qhasm:   addt0 = 0
# asm 1: mov  $0,>addt0=int64#10
# asm 2: mov  $0,>addt0=%r12
mov  $0,%r12

# qhasm:   addt1 = 38
# asm 1: mov  $38,>addt1=int64#11
# asm 2: mov  $38,>addt1=%r13
mov  $38,%r13

# qhasm:   addt1 = addt0 if !carry
# asm 1: cmovae <addt0=int64#10,<addt1=int64#11
# asm 2: cmovae <addt0=%r12,<addt1=%r13
cmovae %r12,%r13

# qhasm:   carry? t10 += addt1
# asm 1: add  <addt1=int64#11,<t10=int64#2
# asm 2: add  <addt1=%r13,<t10=%rsi
add  %r13,%rsi

# qhasm:   carry? t11 += addt0 + carry
# asm 1: adc <addt0=int64#10,<t11=int64#3
# asm 2: adc <addt0=%r12,<t11=%rdx
adc %r12,%rdx

# qhasm:   carry? t12 += addt0 + carry
# asm 1: adc <addt0=int64#10,<t12=int64#4
# asm 2: adc <addt0=%r12,<t12=%rcx
adc %r12,%rcx

# qhasm:   carry? t13 += addt0 + carry
# asm 1: adc <addt0=int64#10,<t13=int64#5
# asm 2: adc <addt0=%r12,<t13=%r8
adc %r12,%r8

# qhasm:   addt0 = addt1 if carry
# asm 1: cmovc <addt1=int64#11,<addt0=int64#10
# asm 2: cmovc <addt1=%r13,<addt0=%r12
cmovc %r13,%r12

# qhasm:   t10 += addt0
# asm 1: add  <addt0=int64#10,<t10=int64#2
# asm 2: add  <addt0=%r12,<t10=%rsi
add  %r12,%rsi

# qhasm:   carry? t20 -= *(uint64 *)(workp + 64) 
# asm 1: subq 64(<workp=int64#1),<t20=int64#6
# asm 2: subq 64(<workp=%rdi),<t20=%r9
subq 64(%rdi),%r9

# qhasm:   carry? t21 -= *(uint64 *)(workp + 72) - carry
# asm 1: sbbq 72(<workp=int64#1),<t21=int64#7
# asm 2: sbbq 72(<workp=%rdi),<t21=%rax
sbbq 72(%rdi),%rax

# qhasm:   carry? t22 -= *(uint64 *)(workp + 80) - carry
# asm 1: sbbq 80(<workp=int64#1),<t22=int64#8
# asm 2: sbbq 80(<workp=%rdi),<t22=%r10
sbbq 80(%rdi),%r10

# qhasm:   carry? t23 -= *(uint64 *)(workp + 88) - carry
# asm 1: sbbq 88(<workp=int64#1),<t23=int64#9
# asm 2: sbbq 88(<workp=%rdi),<t23=%r11
sbbq 88(%rdi),%r11

# qhasm:   subt0 = 0
# asm 1: mov  $0,>subt0=int64#10
# asm 2: mov  $0,>subt0=%r12
mov  $0,%r12

# qhasm:   subt1 = 38
# asm 1: mov  $38,>subt1=int64#11
# asm 2: mov  $38,>subt1=%r13
mov  $38,%r13

# qhasm:   subt1 = subt0 if !carry
# asm 1: cmovae <subt0=int64#10,<subt1=int64#11
# asm 2: cmovae <subt0=%r12,<subt1=%r13
cmovae %r12,%r13

# qhasm:   carry? t20 -= subt1
# asm 1: sub  <subt1=int64#11,<t20=int64#6
# asm 2: sub  <subt1=%r13,<t20=%r9
sub  %r13,%r9

# qhasm:   carry? t21 -= subt0 - carry
# asm 1: sbb  <subt0=int64#10,<t21=int64#7
# asm 2: sbb  <subt0=%r12,<t21=%rax
sbb  %r12,%rax

# qhasm:   carry? t22 -= subt0 - carry
# asm 1: sbb  <subt0=int64#10,<t22=int64#8
# asm 2: sbb  <subt0=%r12,<t22=%r10
sbb  %r12,%r10

# qhasm:   carry? t23 -= subt0 - carry
# asm 1: sbb  <subt0=int64#10,<t23=int64#9
# asm 2: sbb  <subt0=%r12,<t23=%r11
sbb  %r12,%r11

# qhasm:   subt0 = subt1 if carry
# asm 1: cmovc <subt1=int64#11,<subt0=int64#10
# asm 2: cmovc <subt1=%r13,<subt0=%r12
cmovc %r13,%r12

# qhasm:   t20 -= subt0
# asm 1: sub  <subt0=int64#10,<t20=int64#6
# asm 2: sub  <subt0=%r12,<t20=%r9
sub  %r12,%r9

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

# qhasm: t20_stack = t20
# asm 1: movq <t20=int64#6,>t20_stack=stack64#12
# asm 2: movq <t20=%r9,>t20_stack=88(%rsp)
movq %r9,88(%rsp)

# qhasm: t21_stack = t21
# asm 1: movq <t21=int64#7,>t21_stack=stack64#13
# asm 2: movq <t21=%rax,>t21_stack=96(%rsp)
movq %rax,96(%rsp)

# qhasm: t22_stack = t22
# asm 1: movq <t22=int64#8,>t22_stack=stack64#14
# asm 2: movq <t22=%r10,>t22_stack=104(%rsp)
movq %r10,104(%rsp)

# qhasm: t23_stack = t23
# asm 1: movq <t23=int64#9,>t23_stack=stack64#15
# asm 2: movq <t23=%r11,>t23_stack=112(%rsp)
movq %r11,112(%rsp)

# qhasm:   squarer7 = 0
# asm 1: mov  $0,>squarer7=int64#2
# asm 2: mov  $0,>squarer7=%rsi
mov  $0,%rsi

# qhasm:   squarerax = t21_stack
# asm 1: movq <t21_stack=stack64#13,>squarerax=int64#7
# asm 2: movq <t21_stack=96(%rsp),>squarerax=%rax
movq 96(%rsp),%rax

# qhasm:   (uint128) squarerdx squarerax = squarerax * t20_stack
# asm 1: mulq  <t20_stack=stack64#12
# asm 2: mulq  <t20_stack=88(%rsp)
mulq  88(%rsp)

# qhasm:   t71 = squarerax
# asm 1: mov  <squarerax=int64#7,>t71=int64#4
# asm 2: mov  <squarerax=%rax,>t71=%rcx
mov  %rax,%rcx

# qhasm:   t72 = squarerdx
# asm 1: mov  <squarerdx=int64#3,>t72=int64#5
# asm 2: mov  <squarerdx=%rdx,>t72=%r8
mov  %rdx,%r8

# qhasm:   squarerax = t22_stack
# asm 1: movq <t22_stack=stack64#14,>squarerax=int64#7
# asm 2: movq <t22_stack=104(%rsp),>squarerax=%rax
movq 104(%rsp),%rax

# qhasm:   (uint128) squarerdx squarerax = squarerax * t21_stack
# asm 1: mulq  <t21_stack=stack64#13
# asm 2: mulq  <t21_stack=96(%rsp)
mulq  96(%rsp)

# qhasm:   t73 = squarerax 
# asm 1: mov  <squarerax=int64#7,>t73=int64#6
# asm 2: mov  <squarerax=%rax,>t73=%r9
mov  %rax,%r9

# qhasm:   squarer4 = squarerdx
# asm 1: mov  <squarerdx=int64#3,>squarer4=int64#8
# asm 2: mov  <squarerdx=%rdx,>squarer4=%r10
mov  %rdx,%r10

# qhasm:   squarerax = t23_stack
# asm 1: movq <t23_stack=stack64#15,>squarerax=int64#7
# asm 2: movq <t23_stack=112(%rsp),>squarerax=%rax
movq 112(%rsp),%rax

# qhasm:   (uint128) squarerdx squarerax = squarerax * t22_stack
# asm 1: mulq  <t22_stack=stack64#14
# asm 2: mulq  <t22_stack=104(%rsp)
mulq  104(%rsp)

# qhasm:   squarer5 = squarerax
# asm 1: mov  <squarerax=int64#7,>squarer5=int64#9
# asm 2: mov  <squarerax=%rax,>squarer5=%r11
mov  %rax,%r11

# qhasm:   squarer6 = squarerdx
# asm 1: mov  <squarerdx=int64#3,>squarer6=int64#10
# asm 2: mov  <squarerdx=%rdx,>squarer6=%r12
mov  %rdx,%r12

# qhasm:   squarerax = t22_stack
# asm 1: movq <t22_stack=stack64#14,>squarerax=int64#7
# asm 2: movq <t22_stack=104(%rsp),>squarerax=%rax
movq 104(%rsp),%rax

# qhasm:   (uint128) squarerdx squarerax = squarerax * t20_stack
# asm 1: mulq  <t20_stack=stack64#12
# asm 2: mulq  <t20_stack=88(%rsp)
mulq  88(%rsp)

# qhasm:   carry? t72 += squarerax
# asm 1: add  <squarerax=int64#7,<t72=int64#5
# asm 2: add  <squarerax=%rax,<t72=%r8
add  %rax,%r8

# qhasm:   carry? t73 += squarerdx + carry
# asm 1: adc <squarerdx=int64#3,<t73=int64#6
# asm 2: adc <squarerdx=%rdx,<t73=%r9
adc %rdx,%r9

# qhasm:   squarer4 += 0 + carry
# asm 1: adc $0,<squarer4=int64#8
# asm 2: adc $0,<squarer4=%r10
adc $0,%r10

# qhasm:   squarerax = t23_stack
# asm 1: movq <t23_stack=stack64#15,>squarerax=int64#7
# asm 2: movq <t23_stack=112(%rsp),>squarerax=%rax
movq 112(%rsp),%rax

# qhasm:   (uint128) squarerdx squarerax = squarerax * t21_stack
# asm 1: mulq  <t21_stack=stack64#13
# asm 2: mulq  <t21_stack=96(%rsp)
mulq  96(%rsp)

# qhasm:   carry? squarer4 += squarerax
# asm 1: add  <squarerax=int64#7,<squarer4=int64#8
# asm 2: add  <squarerax=%rax,<squarer4=%r10
add  %rax,%r10

# qhasm:   carry? squarer5 += squarerdx + carry
# asm 1: adc <squarerdx=int64#3,<squarer5=int64#9
# asm 2: adc <squarerdx=%rdx,<squarer5=%r11
adc %rdx,%r11

# qhasm:   squarer6 += 0 + carry
# asm 1: adc $0,<squarer6=int64#10
# asm 2: adc $0,<squarer6=%r12
adc $0,%r12

# qhasm:   squarerax = t23_stack
# asm 1: movq <t23_stack=stack64#15,>squarerax=int64#7
# asm 2: movq <t23_stack=112(%rsp),>squarerax=%rax
movq 112(%rsp),%rax

# qhasm:   (uint128) squarerdx squarerax = squarerax * t20_stack
# asm 1: mulq  <t20_stack=stack64#12
# asm 2: mulq  <t20_stack=88(%rsp)
mulq  88(%rsp)

# qhasm:   carry? t73 += squarerax
# asm 1: add  <squarerax=int64#7,<t73=int64#6
# asm 2: add  <squarerax=%rax,<t73=%r9
add  %rax,%r9

# qhasm:   carry? squarer4 += squarerdx + carry
# asm 1: adc <squarerdx=int64#3,<squarer4=int64#8
# asm 2: adc <squarerdx=%rdx,<squarer4=%r10
adc %rdx,%r10

# qhasm:   carry? squarer5 += 0 + carry
# asm 1: adc $0,<squarer5=int64#9
# asm 2: adc $0,<squarer5=%r11
adc $0,%r11

# qhasm:   carry? squarer6 += 0 + carry
# asm 1: adc $0,<squarer6=int64#10
# asm 2: adc $0,<squarer6=%r12
adc $0,%r12

# qhasm:   squarer7 += 0 + carry
# asm 1: adc $0,<squarer7=int64#2
# asm 2: adc $0,<squarer7=%rsi
adc $0,%rsi

# qhasm:   carry? t71 += t71
# asm 1: add  <t71=int64#4,<t71=int64#4
# asm 2: add  <t71=%rcx,<t71=%rcx
add  %rcx,%rcx

# qhasm:   carry? t72 += t72 + carry
# asm 1: adc <t72=int64#5,<t72=int64#5
# asm 2: adc <t72=%r8,<t72=%r8
adc %r8,%r8

# qhasm:   carry? t73 += t73 + carry
# asm 1: adc <t73=int64#6,<t73=int64#6
# asm 2: adc <t73=%r9,<t73=%r9
adc %r9,%r9

# qhasm:   carry? squarer4 += squarer4 + carry
# asm 1: adc <squarer4=int64#8,<squarer4=int64#8
# asm 2: adc <squarer4=%r10,<squarer4=%r10
adc %r10,%r10

# qhasm:   carry? squarer5 += squarer5 + carry
# asm 1: adc <squarer5=int64#9,<squarer5=int64#9
# asm 2: adc <squarer5=%r11,<squarer5=%r11
adc %r11,%r11

# qhasm:   carry? squarer6 += squarer6 + carry
# asm 1: adc <squarer6=int64#10,<squarer6=int64#10
# asm 2: adc <squarer6=%r12,<squarer6=%r12
adc %r12,%r12

# qhasm:   squarer7 += squarer7 + carry
# asm 1: adc <squarer7=int64#2,<squarer7=int64#2
# asm 2: adc <squarer7=%rsi,<squarer7=%rsi
adc %rsi,%rsi

# qhasm:   squarerax = t20_stack
# asm 1: movq <t20_stack=stack64#12,>squarerax=int64#7
# asm 2: movq <t20_stack=88(%rsp),>squarerax=%rax
movq 88(%rsp),%rax

# qhasm:   (uint128) squarerdx squarerax = squarerax * t20_stack
# asm 1: mulq  <t20_stack=stack64#12
# asm 2: mulq  <t20_stack=88(%rsp)
mulq  88(%rsp)

# qhasm:   t70 = squarerax
# asm 1: mov  <squarerax=int64#7,>t70=int64#11
# asm 2: mov  <squarerax=%rax,>t70=%r13
mov  %rax,%r13

# qhasm:   squaret1 = squarerdx
# asm 1: mov  <squarerdx=int64#3,>squaret1=int64#12
# asm 2: mov  <squarerdx=%rdx,>squaret1=%r14
mov  %rdx,%r14

# qhasm:   squarerax = t21_stack
# asm 1: movq <t21_stack=stack64#13,>squarerax=int64#7
# asm 2: movq <t21_stack=96(%rsp),>squarerax=%rax
movq 96(%rsp),%rax

# qhasm:   (uint128) squarerdx squarerax = squarerax * t21_stack
# asm 1: mulq  <t21_stack=stack64#13
# asm 2: mulq  <t21_stack=96(%rsp)
mulq  96(%rsp)

# qhasm:   squaret2 = squarerax
# asm 1: mov  <squarerax=int64#7,>squaret2=int64#13
# asm 2: mov  <squarerax=%rax,>squaret2=%r15
mov  %rax,%r15

# qhasm:   squaret3 = squarerdx
# asm 1: mov  <squarerdx=int64#3,>squaret3=int64#14
# asm 2: mov  <squarerdx=%rdx,>squaret3=%rbx
mov  %rdx,%rbx

# qhasm:   squarerax = t22_stack
# asm 1: movq <t22_stack=stack64#14,>squarerax=int64#7
# asm 2: movq <t22_stack=104(%rsp),>squarerax=%rax
movq 104(%rsp),%rax

# qhasm:   (uint128) squarerdx squarerax = squarerax * t22_stack
# asm 1: mulq  <t22_stack=stack64#14
# asm 2: mulq  <t22_stack=104(%rsp)
mulq  104(%rsp)

# qhasm:   carry? t71 += squaret1
# asm 1: add  <squaret1=int64#12,<t71=int64#4
# asm 2: add  <squaret1=%r14,<t71=%rcx
add  %r14,%rcx

# qhasm:   carry? t72 += squaret2 + carry
# asm 1: adc <squaret2=int64#13,<t72=int64#5
# asm 2: adc <squaret2=%r15,<t72=%r8
adc %r15,%r8

# qhasm:   carry? t73 += squaret3 + carry
# asm 1: adc <squaret3=int64#14,<t73=int64#6
# asm 2: adc <squaret3=%rbx,<t73=%r9
adc %rbx,%r9

# qhasm:   carry? squarer4 += squarerax + carry
# asm 1: adc <squarerax=int64#7,<squarer4=int64#8
# asm 2: adc <squarerax=%rax,<squarer4=%r10
adc %rax,%r10

# qhasm:   carry? squarer5 += squarerdx + carry
# asm 1: adc <squarerdx=int64#3,<squarer5=int64#9
# asm 2: adc <squarerdx=%rdx,<squarer5=%r11
adc %rdx,%r11

# qhasm:   carry? squarer6 += 0 + carry
# asm 1: adc $0,<squarer6=int64#10
# asm 2: adc $0,<squarer6=%r12
adc $0,%r12

# qhasm:   squarer7 += 0 + carry 
# asm 1: adc $0,<squarer7=int64#2
# asm 2: adc $0,<squarer7=%rsi
adc $0,%rsi

# qhasm:   squarerax = t23_stack
# asm 1: movq <t23_stack=stack64#15,>squarerax=int64#7
# asm 2: movq <t23_stack=112(%rsp),>squarerax=%rax
movq 112(%rsp),%rax

# qhasm:   (uint128) squarerdx squarerax = squarerax * t23_stack
# asm 1: mulq  <t23_stack=stack64#15
# asm 2: mulq  <t23_stack=112(%rsp)
mulq  112(%rsp)

# qhasm:   carry? squarer6 += squarerax
# asm 1: add  <squarerax=int64#7,<squarer6=int64#10
# asm 2: add  <squarerax=%rax,<squarer6=%r12
add  %rax,%r12

# qhasm:   squarer7 += squarerdx + carry
# asm 1: adc <squarerdx=int64#3,<squarer7=int64#2
# asm 2: adc <squarerdx=%rdx,<squarer7=%rsi
adc %rdx,%rsi

# qhasm:   squarerax = squarer4
# asm 1: mov  <squarer4=int64#8,>squarerax=int64#7
# asm 2: mov  <squarer4=%r10,>squarerax=%rax
mov  %r10,%rax

# qhasm:   (uint128) squarerdx squarerax = squarerax * *(uint64 *)&crypto_scalarmult_curve25519_amd64_64_38
mulq  crypto_scalarmult_curve25519_amd64_64_38

# qhasm:   squarer4 = squarerax
# asm 1: mov  <squarerax=int64#7,>squarer4=int64#8
# asm 2: mov  <squarerax=%rax,>squarer4=%r10
mov  %rax,%r10

# qhasm:   squarerax = squarer5
# asm 1: mov  <squarer5=int64#9,>squarerax=int64#7
# asm 2: mov  <squarer5=%r11,>squarerax=%rax
mov  %r11,%rax

# qhasm:   squarer5 = squarerdx
# asm 1: mov  <squarerdx=int64#3,>squarer5=int64#9
# asm 2: mov  <squarerdx=%rdx,>squarer5=%r11
mov  %rdx,%r11

# qhasm:   (uint128) squarerdx squarerax = squarerax * *(uint64 *)&crypto_scalarmult_curve25519_amd64_64_38
mulq  crypto_scalarmult_curve25519_amd64_64_38

# qhasm:   carry? squarer5 += squarerax
# asm 1: add  <squarerax=int64#7,<squarer5=int64#9
# asm 2: add  <squarerax=%rax,<squarer5=%r11
add  %rax,%r11

# qhasm:   squarerax = squarer6
# asm 1: mov  <squarer6=int64#10,>squarerax=int64#7
# asm 2: mov  <squarer6=%r12,>squarerax=%rax
mov  %r12,%rax

# qhasm:   squarer6 = 0
# asm 1: mov  $0,>squarer6=int64#10
# asm 2: mov  $0,>squarer6=%r12
mov  $0,%r12

# qhasm:   squarer6 += squarerdx + carry
# asm 1: adc <squarerdx=int64#3,<squarer6=int64#10
# asm 2: adc <squarerdx=%rdx,<squarer6=%r12
adc %rdx,%r12

# qhasm:   (uint128) squarerdx squarerax = squarerax * *(uint64 *)&crypto_scalarmult_curve25519_amd64_64_38
mulq  crypto_scalarmult_curve25519_amd64_64_38

# qhasm:   carry? squarer6 += squarerax
# asm 1: add  <squarerax=int64#7,<squarer6=int64#10
# asm 2: add  <squarerax=%rax,<squarer6=%r12
add  %rax,%r12

# qhasm:   squarerax = squarer7
# asm 1: mov  <squarer7=int64#2,>squarerax=int64#7
# asm 2: mov  <squarer7=%rsi,>squarerax=%rax
mov  %rsi,%rax

# qhasm:   squarer7 = 0
# asm 1: mov  $0,>squarer7=int64#2
# asm 2: mov  $0,>squarer7=%rsi
mov  $0,%rsi

# qhasm:   squarer7 += squarerdx + carry
# asm 1: adc <squarerdx=int64#3,<squarer7=int64#2
# asm 2: adc <squarerdx=%rdx,<squarer7=%rsi
adc %rdx,%rsi

# qhasm:   (uint128) squarerdx squarerax = squarerax * *(uint64 *)&crypto_scalarmult_curve25519_amd64_64_38
mulq  crypto_scalarmult_curve25519_amd64_64_38

# qhasm:   carry? squarer7 += squarerax
# asm 1: add  <squarerax=int64#7,<squarer7=int64#2
# asm 2: add  <squarerax=%rax,<squarer7=%rsi
add  %rax,%rsi

# qhasm:   squarer8 = 0
# asm 1: mov  $0,>squarer8=int64#7
# asm 2: mov  $0,>squarer8=%rax
mov  $0,%rax

# qhasm:   squarer8 += squarerdx + carry
# asm 1: adc <squarerdx=int64#3,<squarer8=int64#7
# asm 2: adc <squarerdx=%rdx,<squarer8=%rax
adc %rdx,%rax

# qhasm:   carry? t70 += squarer4
# asm 1: add  <squarer4=int64#8,<t70=int64#11
# asm 2: add  <squarer4=%r10,<t70=%r13
add  %r10,%r13

# qhasm:   carry? t71 += squarer5 + carry
# asm 1: adc <squarer5=int64#9,<t71=int64#4
# asm 2: adc <squarer5=%r11,<t71=%rcx
adc %r11,%rcx

# qhasm:   carry? t72 += squarer6 + carry
# asm 1: adc <squarer6=int64#10,<t72=int64#5
# asm 2: adc <squarer6=%r12,<t72=%r8
adc %r12,%r8

# qhasm:   carry? t73 += squarer7 + carry
# asm 1: adc <squarer7=int64#2,<t73=int64#6
# asm 2: adc <squarer7=%rsi,<t73=%r9
adc %rsi,%r9

# qhasm:   squarezero = 0
# asm 1: mov  $0,>squarezero=int64#2
# asm 2: mov  $0,>squarezero=%rsi
mov  $0,%rsi

# qhasm:   squarer8 += squarezero + carry
# asm 1: adc <squarezero=int64#2,<squarer8=int64#7
# asm 2: adc <squarezero=%rsi,<squarer8=%rax
adc %rsi,%rax

# qhasm:   squarer8 *= 38
# asm 1: imulq  $38,<squarer8=int64#7,>squarer8=int64#3
# asm 2: imulq  $38,<squarer8=%rax,>squarer8=%rdx
imulq  $38,%rax,%rdx

# qhasm:   carry? t70 += squarer8
# asm 1: add  <squarer8=int64#3,<t70=int64#11
# asm 2: add  <squarer8=%rdx,<t70=%r13
add  %rdx,%r13

# qhasm:   carry? t71 += squarezero + carry
# asm 1: adc <squarezero=int64#2,<t71=int64#4
# asm 2: adc <squarezero=%rsi,<t71=%rcx
adc %rsi,%rcx

# qhasm:   carry? t72 += squarezero + carry
# asm 1: adc <squarezero=int64#2,<t72=int64#5
# asm 2: adc <squarezero=%rsi,<t72=%r8
adc %rsi,%r8

# qhasm:   carry? t73 += squarezero + carry
# asm 1: adc <squarezero=int64#2,<t73=int64#6
# asm 2: adc <squarezero=%rsi,<t73=%r9
adc %rsi,%r9

# qhasm:   squarezero += squarezero + carry
# asm 1: adc <squarezero=int64#2,<squarezero=int64#2
# asm 2: adc <squarezero=%rsi,<squarezero=%rsi
adc %rsi,%rsi

# qhasm:   squarezero *= 38
# asm 1: imulq  $38,<squarezero=int64#2,>squarezero=int64#2
# asm 2: imulq  $38,<squarezero=%rsi,>squarezero=%rsi
imulq  $38,%rsi,%rsi

# qhasm:   t70 += squarezero
# asm 1: add  <squarezero=int64#2,<t70=int64#11
# asm 2: add  <squarezero=%rsi,<t70=%r13
add  %rsi,%r13

# qhasm: t70_stack = t70
# asm 1: movq <t70=int64#11,>t70_stack=stack64#16
# asm 2: movq <t70=%r13,>t70_stack=120(%rsp)
movq %r13,120(%rsp)

# qhasm: t71_stack = t71
# asm 1: movq <t71=int64#4,>t71_stack=stack64#17
# asm 2: movq <t71=%rcx,>t71_stack=128(%rsp)
movq %rcx,128(%rsp)

# qhasm: t72_stack = t72
# asm 1: movq <t72=int64#5,>t72_stack=stack64#18
# asm 2: movq <t72=%r8,>t72_stack=136(%rsp)
movq %r8,136(%rsp)

# qhasm: t73_stack = t73
# asm 1: movq <t73=int64#6,>t73_stack=stack64#19
# asm 2: movq <t73=%r9,>t73_stack=144(%rsp)
movq %r9,144(%rsp)

# qhasm:   squarer7 = 0
# asm 1: mov  $0,>squarer7=int64#2
# asm 2: mov  $0,>squarer7=%rsi
mov  $0,%rsi

# qhasm:   squarerax = t11_stack
# asm 1: movq <t11_stack=stack64#9,>squarerax=int64#7
# asm 2: movq <t11_stack=64(%rsp),>squarerax=%rax
movq 64(%rsp),%rax

# qhasm:   (uint128) squarerdx squarerax = squarerax * t10_stack
# asm 1: mulq  <t10_stack=stack64#8
# asm 2: mulq  <t10_stack=56(%rsp)
mulq  56(%rsp)

# qhasm:   t61 = squarerax
# asm 1: mov  <squarerax=int64#7,>t61=int64#4
# asm 2: mov  <squarerax=%rax,>t61=%rcx
mov  %rax,%rcx

# qhasm:   t62 = squarerdx
# asm 1: mov  <squarerdx=int64#3,>t62=int64#5
# asm 2: mov  <squarerdx=%rdx,>t62=%r8
mov  %rdx,%r8

# qhasm:   squarerax = t12_stack
# asm 1: movq <t12_stack=stack64#10,>squarerax=int64#7
# asm 2: movq <t12_stack=72(%rsp),>squarerax=%rax
movq 72(%rsp),%rax

# qhasm:   (uint128) squarerdx squarerax = squarerax * t11_stack
# asm 1: mulq  <t11_stack=stack64#9
# asm 2: mulq  <t11_stack=64(%rsp)
mulq  64(%rsp)

# qhasm:   t63 = squarerax 
# asm 1: mov  <squarerax=int64#7,>t63=int64#6
# asm 2: mov  <squarerax=%rax,>t63=%r9
mov  %rax,%r9

# qhasm:   squarer4 = squarerdx
# asm 1: mov  <squarerdx=int64#3,>squarer4=int64#8
# asm 2: mov  <squarerdx=%rdx,>squarer4=%r10
mov  %rdx,%r10

# qhasm:   squarerax = t13_stack
# asm 1: movq <t13_stack=stack64#11,>squarerax=int64#7
# asm 2: movq <t13_stack=80(%rsp),>squarerax=%rax
movq 80(%rsp),%rax

# qhasm:   (uint128) squarerdx squarerax = squarerax * t12_stack
# asm 1: mulq  <t12_stack=stack64#10
# asm 2: mulq  <t12_stack=72(%rsp)
mulq  72(%rsp)

# qhasm:   squarer5 = squarerax
# asm 1: mov  <squarerax=int64#7,>squarer5=int64#9
# asm 2: mov  <squarerax=%rax,>squarer5=%r11
mov  %rax,%r11

# qhasm:   squarer6 = squarerdx
# asm 1: mov  <squarerdx=int64#3,>squarer6=int64#10
# asm 2: mov  <squarerdx=%rdx,>squarer6=%r12
mov  %rdx,%r12

# qhasm:   squarerax = t12_stack
# asm 1: movq <t12_stack=stack64#10,>squarerax=int64#7
# asm 2: movq <t12_stack=72(%rsp),>squarerax=%rax
movq 72(%rsp),%rax

# qhasm:   (uint128) squarerdx squarerax = squarerax * t10_stack
# asm 1: mulq  <t10_stack=stack64#8
# asm 2: mulq  <t10_stack=56(%rsp)
mulq  56(%rsp)

# qhasm:   carry? t62 += squarerax
# asm 1: add  <squarerax=int64#7,<t62=int64#5
# asm 2: add  <squarerax=%rax,<t62=%r8
add  %rax,%r8

# qhasm:   carry? t63 += squarerdx + carry
# asm 1: adc <squarerdx=int64#3,<t63=int64#6
# asm 2: adc <squarerdx=%rdx,<t63=%r9
adc %rdx,%r9

# qhasm:   squarer4 += 0 + carry
# asm 1: adc $0,<squarer4=int64#8
# asm 2: adc $0,<squarer4=%r10
adc $0,%r10

# qhasm:   squarerax = t13_stack
# asm 1: movq <t13_stack=stack64#11,>squarerax=int64#7
# asm 2: movq <t13_stack=80(%rsp),>squarerax=%rax
movq 80(%rsp),%rax

# qhasm:   (uint128) squarerdx squarerax = squarerax * t11_stack
# asm 1: mulq  <t11_stack=stack64#9
# asm 2: mulq  <t11_stack=64(%rsp)
mulq  64(%rsp)

# qhasm:   carry? squarer4 += squarerax
# asm 1: add  <squarerax=int64#7,<squarer4=int64#8
# asm 2: add  <squarerax=%rax,<squarer4=%r10
add  %rax,%r10

# qhasm:   carry? squarer5 += squarerdx + carry
# asm 1: adc <squarerdx=int64#3,<squarer5=int64#9
# asm 2: adc <squarerdx=%rdx,<squarer5=%r11
adc %rdx,%r11

# qhasm:   squarer6 += 0 + carry
# asm 1: adc $0,<squarer6=int64#10
# asm 2: adc $0,<squarer6=%r12
adc $0,%r12

# qhasm:   squarerax = t13_stack
# asm 1: movq <t13_stack=stack64#11,>squarerax=int64#7
# asm 2: movq <t13_stack=80(%rsp),>squarerax=%rax
movq 80(%rsp),%rax

# qhasm:   (uint128) squarerdx squarerax = squarerax * t10_stack
# asm 1: mulq  <t10_stack=stack64#8
# asm 2: mulq  <t10_stack=56(%rsp)
mulq  56(%rsp)

# qhasm:   carry? t63 += squarerax
# asm 1: add  <squarerax=int64#7,<t63=int64#6
# asm 2: add  <squarerax=%rax,<t63=%r9
add  %rax,%r9

# qhasm:   carry? squarer4 += squarerdx + carry
# asm 1: adc <squarerdx=int64#3,<squarer4=int64#8
# asm 2: adc <squarerdx=%rdx,<squarer4=%r10
adc %rdx,%r10

# qhasm:   carry? squarer5 += 0 + carry
# asm 1: adc $0,<squarer5=int64#9
# asm 2: adc $0,<squarer5=%r11
adc $0,%r11

# qhasm:   carry? squarer6 += 0 + carry
# asm 1: adc $0,<squarer6=int64#10
# asm 2: adc $0,<squarer6=%r12
adc $0,%r12

# qhasm:   squarer7 += 0 + carry
# asm 1: adc $0,<squarer7=int64#2
# asm 2: adc $0,<squarer7=%rsi
adc $0,%rsi

# qhasm:   carry? t61 += t61
# asm 1: add  <t61=int64#4,<t61=int64#4
# asm 2: add  <t61=%rcx,<t61=%rcx
add  %rcx,%rcx

# qhasm:   carry? t62 += t62 + carry
# asm 1: adc <t62=int64#5,<t62=int64#5
# asm 2: adc <t62=%r8,<t62=%r8
adc %r8,%r8

# qhasm:   carry? t63 += t63 + carry
# asm 1: adc <t63=int64#6,<t63=int64#6
# asm 2: adc <t63=%r9,<t63=%r9
adc %r9,%r9

# qhasm:   carry? squarer4 += squarer4 + carry
# asm 1: adc <squarer4=int64#8,<squarer4=int64#8
# asm 2: adc <squarer4=%r10,<squarer4=%r10
adc %r10,%r10

# qhasm:   carry? squarer5 += squarer5 + carry
# asm 1: adc <squarer5=int64#9,<squarer5=int64#9
# asm 2: adc <squarer5=%r11,<squarer5=%r11
adc %r11,%r11

# qhasm:   carry? squarer6 += squarer6 + carry
# asm 1: adc <squarer6=int64#10,<squarer6=int64#10
# asm 2: adc <squarer6=%r12,<squarer6=%r12
adc %r12,%r12

# qhasm:   squarer7 += squarer7 + carry
# asm 1: adc <squarer7=int64#2,<squarer7=int64#2
# asm 2: adc <squarer7=%rsi,<squarer7=%rsi
adc %rsi,%rsi

# qhasm:   squarerax = t10_stack
# asm 1: movq <t10_stack=stack64#8,>squarerax=int64#7
# asm 2: movq <t10_stack=56(%rsp),>squarerax=%rax
movq 56(%rsp),%rax

# qhasm:   (uint128) squarerdx squarerax = squarerax * t10_stack
# asm 1: mulq  <t10_stack=stack64#8
# asm 2: mulq  <t10_stack=56(%rsp)
mulq  56(%rsp)

# qhasm:   t60 = squarerax
# asm 1: mov  <squarerax=int64#7,>t60=int64#11
# asm 2: mov  <squarerax=%rax,>t60=%r13
mov  %rax,%r13

# qhasm:   squaret1 = squarerdx
# asm 1: mov  <squarerdx=int64#3,>squaret1=int64#12
# asm 2: mov  <squarerdx=%rdx,>squaret1=%r14
mov  %rdx,%r14

# qhasm:   squarerax = t11_stack
# asm 1: movq <t11_stack=stack64#9,>squarerax=int64#7
# asm 2: movq <t11_stack=64(%rsp),>squarerax=%rax
movq 64(%rsp),%rax

# qhasm:   (uint128) squarerdx squarerax = squarerax * t11_stack
# asm 1: mulq  <t11_stack=stack64#9
# asm 2: mulq  <t11_stack=64(%rsp)
mulq  64(%rsp)

# qhasm:   squaret2 = squarerax
# asm 1: mov  <squarerax=int64#7,>squaret2=int64#13
# asm 2: mov  <squarerax=%rax,>squaret2=%r15
mov  %rax,%r15

# qhasm:   squaret3 = squarerdx
# asm 1: mov  <squarerdx=int64#3,>squaret3=int64#14
# asm 2: mov  <squarerdx=%rdx,>squaret3=%rbx
mov  %rdx,%rbx

# qhasm:   squarerax = t12_stack
# asm 1: movq <t12_stack=stack64#10,>squarerax=int64#7
# asm 2: movq <t12_stack=72(%rsp),>squarerax=%rax
movq 72(%rsp),%rax

# qhasm:   (uint128) squarerdx squarerax = squarerax * t12_stack
# asm 1: mulq  <t12_stack=stack64#10
# asm 2: mulq  <t12_stack=72(%rsp)
mulq  72(%rsp)

# qhasm:   carry? t61 += squaret1
# asm 1: add  <squaret1=int64#12,<t61=int64#4
# asm 2: add  <squaret1=%r14,<t61=%rcx
add  %r14,%rcx

# qhasm:   carry? t62 += squaret2 + carry
# asm 1: adc <squaret2=int64#13,<t62=int64#5
# asm 2: adc <squaret2=%r15,<t62=%r8
adc %r15,%r8

# qhasm:   carry? t63 += squaret3 + carry
# asm 1: adc <squaret3=int64#14,<t63=int64#6
# asm 2: adc <squaret3=%rbx,<t63=%r9
adc %rbx,%r9

# qhasm:   carry? squarer4 += squarerax + carry
# asm 1: adc <squarerax=int64#7,<squarer4=int64#8
# asm 2: adc <squarerax=%rax,<squarer4=%r10
adc %rax,%r10

# qhasm:   carry? squarer5 += squarerdx + carry
# asm 1: adc <squarerdx=int64#3,<squarer5=int64#9
# asm 2: adc <squarerdx=%rdx,<squarer5=%r11
adc %rdx,%r11

# qhasm:   carry? squarer6 += 0 + carry
# asm 1: adc $0,<squarer6=int64#10
# asm 2: adc $0,<squarer6=%r12
adc $0,%r12

# qhasm:   squarer7 += 0 + carry 
# asm 1: adc $0,<squarer7=int64#2
# asm 2: adc $0,<squarer7=%rsi
adc $0,%rsi

# qhasm:   squarerax = t13_stack
# asm 1: movq <t13_stack=stack64#11,>squarerax=int64#7
# asm 2: movq <t13_stack=80(%rsp),>squarerax=%rax
movq 80(%rsp),%rax

# qhasm:   (uint128) squarerdx squarerax = squarerax * t13_stack
# asm 1: mulq  <t13_stack=stack64#11
# asm 2: mulq  <t13_stack=80(%rsp)
mulq  80(%rsp)

# qhasm:   carry? squarer6 += squarerax
# asm 1: add  <squarerax=int64#7,<squarer6=int64#10
# asm 2: add  <squarerax=%rax,<squarer6=%r12
add  %rax,%r12

# qhasm:   squarer7 += squarerdx + carry
# asm 1: adc <squarerdx=int64#3,<squarer7=int64#2
# asm 2: adc <squarerdx=%rdx,<squarer7=%rsi
adc %rdx,%rsi

# qhasm:   squarerax = squarer4
# asm 1: mov  <squarer4=int64#8,>squarerax=int64#7
# asm 2: mov  <squarer4=%r10,>squarerax=%rax
mov  %r10,%rax

# qhasm:   (uint128) squarerdx squarerax = squarerax * *(uint64 *)&crypto_scalarmult_curve25519_amd64_64_38
mulq  crypto_scalarmult_curve25519_amd64_64_38

# qhasm:   squarer4 = squarerax
# asm 1: mov  <squarerax=int64#7,>squarer4=int64#8
# asm 2: mov  <squarerax=%rax,>squarer4=%r10
mov  %rax,%r10

# qhasm:   squarerax = squarer5
# asm 1: mov  <squarer5=int64#9,>squarerax=int64#7
# asm 2: mov  <squarer5=%r11,>squarerax=%rax
mov  %r11,%rax

# qhasm:   squarer5 = squarerdx
# asm 1: mov  <squarerdx=int64#3,>squarer5=int64#9
# asm 2: mov  <squarerdx=%rdx,>squarer5=%r11
mov  %rdx,%r11

# qhasm:   (uint128) squarerdx squarerax = squarerax * *(uint64 *)&crypto_scalarmult_curve25519_amd64_64_38
mulq  crypto_scalarmult_curve25519_amd64_64_38

# qhasm:   carry? squarer5 += squarerax
# asm 1: add  <squarerax=int64#7,<squarer5=int64#9
# asm 2: add  <squarerax=%rax,<squarer5=%r11
add  %rax,%r11

# qhasm:   squarerax = squarer6
# asm 1: mov  <squarer6=int64#10,>squarerax=int64#7
# asm 2: mov  <squarer6=%r12,>squarerax=%rax
mov  %r12,%rax

# qhasm:   squarer6 = 0
# asm 1: mov  $0,>squarer6=int64#10
# asm 2: mov  $0,>squarer6=%r12
mov  $0,%r12

# qhasm:   squarer6 += squarerdx + carry
# asm 1: adc <squarerdx=int64#3,<squarer6=int64#10
# asm 2: adc <squarerdx=%rdx,<squarer6=%r12
adc %rdx,%r12

# qhasm:   (uint128) squarerdx squarerax = squarerax * *(uint64 *)&crypto_scalarmult_curve25519_amd64_64_38
mulq  crypto_scalarmult_curve25519_amd64_64_38

# qhasm:   carry? squarer6 += squarerax
# asm 1: add  <squarerax=int64#7,<squarer6=int64#10
# asm 2: add  <squarerax=%rax,<squarer6=%r12
add  %rax,%r12

# qhasm:   squarerax = squarer7
# asm 1: mov  <squarer7=int64#2,>squarerax=int64#7
# asm 2: mov  <squarer7=%rsi,>squarerax=%rax
mov  %rsi,%rax

# qhasm:   squarer7 = 0
# asm 1: mov  $0,>squarer7=int64#2
# asm 2: mov  $0,>squarer7=%rsi
mov  $0,%rsi

# qhasm:   squarer7 += squarerdx + carry
# asm 1: adc <squarerdx=int64#3,<squarer7=int64#2
# asm 2: adc <squarerdx=%rdx,<squarer7=%rsi
adc %rdx,%rsi

# qhasm:   (uint128) squarerdx squarerax = squarerax * *(uint64 *)&crypto_scalarmult_curve25519_amd64_64_38
mulq  crypto_scalarmult_curve25519_amd64_64_38

# qhasm:   carry? squarer7 += squarerax
# asm 1: add  <squarerax=int64#7,<squarer7=int64#2
# asm 2: add  <squarerax=%rax,<squarer7=%rsi
add  %rax,%rsi

# qhasm:   squarer8 = 0
# asm 1: mov  $0,>squarer8=int64#7
# asm 2: mov  $0,>squarer8=%rax
mov  $0,%rax

# qhasm:   squarer8 += squarerdx + carry
# asm 1: adc <squarerdx=int64#3,<squarer8=int64#7
# asm 2: adc <squarerdx=%rdx,<squarer8=%rax
adc %rdx,%rax

# qhasm:   carry? t60 += squarer4
# asm 1: add  <squarer4=int64#8,<t60=int64#11
# asm 2: add  <squarer4=%r10,<t60=%r13
add  %r10,%r13

# qhasm:   carry? t61 += squarer5 + carry
# asm 1: adc <squarer5=int64#9,<t61=int64#4
# asm 2: adc <squarer5=%r11,<t61=%rcx
adc %r11,%rcx

# qhasm:   carry? t62 += squarer6 + carry
# asm 1: adc <squarer6=int64#10,<t62=int64#5
# asm 2: adc <squarer6=%r12,<t62=%r8
adc %r12,%r8

# qhasm:   carry? t63 += squarer7 + carry
# asm 1: adc <squarer7=int64#2,<t63=int64#6
# asm 2: adc <squarer7=%rsi,<t63=%r9
adc %rsi,%r9

# qhasm:   squarezero = 0
# asm 1: mov  $0,>squarezero=int64#2
# asm 2: mov  $0,>squarezero=%rsi
mov  $0,%rsi

# qhasm:   squarer8 += squarezero + carry
# asm 1: adc <squarezero=int64#2,<squarer8=int64#7
# asm 2: adc <squarezero=%rsi,<squarer8=%rax
adc %rsi,%rax

# qhasm:   squarer8 *= 38
# asm 1: imulq  $38,<squarer8=int64#7,>squarer8=int64#3
# asm 2: imulq  $38,<squarer8=%rax,>squarer8=%rdx
imulq  $38,%rax,%rdx

# qhasm:   carry? t60 += squarer8
# asm 1: add  <squarer8=int64#3,<t60=int64#11
# asm 2: add  <squarer8=%rdx,<t60=%r13
add  %rdx,%r13

# qhasm:   carry? t61 += squarezero + carry
# asm 1: adc <squarezero=int64#2,<t61=int64#4
# asm 2: adc <squarezero=%rsi,<t61=%rcx
adc %rsi,%rcx

# qhasm:   carry? t62 += squarezero + carry
# asm 1: adc <squarezero=int64#2,<t62=int64#5
# asm 2: adc <squarezero=%rsi,<t62=%r8
adc %rsi,%r8

# qhasm:   carry? t63 += squarezero + carry
# asm 1: adc <squarezero=int64#2,<t63=int64#6
# asm 2: adc <squarezero=%rsi,<t63=%r9
adc %rsi,%r9

# qhasm:   squarezero += squarezero + carry
# asm 1: adc <squarezero=int64#2,<squarezero=int64#2
# asm 2: adc <squarezero=%rsi,<squarezero=%rsi
adc %rsi,%rsi

# qhasm:   squarezero *= 38
# asm 1: imulq  $38,<squarezero=int64#2,>squarezero=int64#2
# asm 2: imulq  $38,<squarezero=%rsi,>squarezero=%rsi
imulq  $38,%rsi,%rsi

# qhasm:   t60 += squarezero
# asm 1: add  <squarezero=int64#2,<t60=int64#11
# asm 2: add  <squarezero=%rsi,<t60=%r13
add  %rsi,%r13

# qhasm: t60_stack = t60
# asm 1: movq <t60=int64#11,>t60_stack=stack64#20
# asm 2: movq <t60=%r13,>t60_stack=152(%rsp)
movq %r13,152(%rsp)

# qhasm: t61_stack = t61
# asm 1: movq <t61=int64#4,>t61_stack=stack64#21
# asm 2: movq <t61=%rcx,>t61_stack=160(%rsp)
movq %rcx,160(%rsp)

# qhasm: t62_stack = t62
# asm 1: movq <t62=int64#5,>t62_stack=stack64#22
# asm 2: movq <t62=%r8,>t62_stack=168(%rsp)
movq %r8,168(%rsp)

# qhasm: t63_stack = t63
# asm 1: movq <t63=int64#6,>t63_stack=stack64#23
# asm 2: movq <t63=%r9,>t63_stack=176(%rsp)
movq %r9,176(%rsp)

# qhasm: t50 = t60
# asm 1: mov  <t60=int64#11,>t50=int64#2
# asm 2: mov  <t60=%r13,>t50=%rsi
mov  %r13,%rsi

# qhasm: t51 = t61
# asm 1: mov  <t61=int64#4,>t51=int64#3
# asm 2: mov  <t61=%rcx,>t51=%rdx
mov  %rcx,%rdx

# qhasm: t52 = t62
# asm 1: mov  <t62=int64#5,>t52=int64#4
# asm 2: mov  <t62=%r8,>t52=%rcx
mov  %r8,%rcx

# qhasm: t53 = t63
# asm 1: mov  <t63=int64#6,>t53=int64#5
# asm 2: mov  <t63=%r9,>t53=%r8
mov  %r9,%r8

# qhasm:   carry? t50 -= t70_stack 
# asm 1: subq <t70_stack=stack64#16,<t50=int64#2
# asm 2: subq <t70_stack=120(%rsp),<t50=%rsi
subq 120(%rsp),%rsi

# qhasm:   carry? t51 -= t71_stack - carry
# asm 1: sbbq <t71_stack=stack64#17,<t51=int64#3
# asm 2: sbbq <t71_stack=128(%rsp),<t51=%rdx
sbbq 128(%rsp),%rdx

# qhasm:   carry? t52 -= t72_stack - carry
# asm 1: sbbq <t72_stack=stack64#18,<t52=int64#4
# asm 2: sbbq <t72_stack=136(%rsp),<t52=%rcx
sbbq 136(%rsp),%rcx

# qhasm:   carry? t53 -= t73_stack - carry
# asm 1: sbbq <t73_stack=stack64#19,<t53=int64#5
# asm 2: sbbq <t73_stack=144(%rsp),<t53=%r8
sbbq 144(%rsp),%r8

# qhasm:   subt0 = 0
# asm 1: mov  $0,>subt0=int64#6
# asm 2: mov  $0,>subt0=%r9
mov  $0,%r9

# qhasm:   subt1 = 38
# asm 1: mov  $38,>subt1=int64#7
# asm 2: mov  $38,>subt1=%rax
mov  $38,%rax

# qhasm:   subt1 = subt0 if !carry
# asm 1: cmovae <subt0=int64#6,<subt1=int64#7
# asm 2: cmovae <subt0=%r9,<subt1=%rax
cmovae %r9,%rax

# qhasm:   carry? t50 -= subt1
# asm 1: sub  <subt1=int64#7,<t50=int64#2
# asm 2: sub  <subt1=%rax,<t50=%rsi
sub  %rax,%rsi

# qhasm:   carry? t51 -= subt0 - carry
# asm 1: sbb  <subt0=int64#6,<t51=int64#3
# asm 2: sbb  <subt0=%r9,<t51=%rdx
sbb  %r9,%rdx

# qhasm:   carry? t52 -= subt0 - carry
# asm 1: sbb  <subt0=int64#6,<t52=int64#4
# asm 2: sbb  <subt0=%r9,<t52=%rcx
sbb  %r9,%rcx

# qhasm:   carry? t53 -= subt0 - carry
# asm 1: sbb  <subt0=int64#6,<t53=int64#5
# asm 2: sbb  <subt0=%r9,<t53=%r8
sbb  %r9,%r8

# qhasm:   subt0 = subt1 if carry
# asm 1: cmovc <subt1=int64#7,<subt0=int64#6
# asm 2: cmovc <subt1=%rax,<subt0=%r9
cmovc %rax,%r9

# qhasm:   t50 -= subt0
# asm 1: sub  <subt0=int64#6,<t50=int64#2
# asm 2: sub  <subt0=%r9,<t50=%rsi
sub  %r9,%rsi

# qhasm: t50_stack = t50
# asm 1: movq <t50=int64#2,>t50_stack=stack64#24
# asm 2: movq <t50=%rsi,>t50_stack=184(%rsp)
movq %rsi,184(%rsp)

# qhasm: t51_stack = t51
# asm 1: movq <t51=int64#3,>t51_stack=stack64#25
# asm 2: movq <t51=%rdx,>t51_stack=192(%rsp)
movq %rdx,192(%rsp)

# qhasm: t52_stack = t52
# asm 1: movq <t52=int64#4,>t52_stack=stack64#26
# asm 2: movq <t52=%rcx,>t52_stack=200(%rsp)
movq %rcx,200(%rsp)

# qhasm: t53_stack = t53
# asm 1: movq <t53=int64#5,>t53_stack=stack64#27
# asm 2: movq <t53=%r8,>t53_stack=208(%rsp)
movq %r8,208(%rsp)

# qhasm: t30 = *(uint64 *)(workp + 96)
# asm 1: movq   96(<workp=int64#1),>t30=int64#2
# asm 2: movq   96(<workp=%rdi),>t30=%rsi
movq   96(%rdi),%rsi

# qhasm: t31 = *(uint64 *)(workp + 104)
# asm 1: movq   104(<workp=int64#1),>t31=int64#3
# asm 2: movq   104(<workp=%rdi),>t31=%rdx
movq   104(%rdi),%rdx

# qhasm: t32 = *(uint64 *)(workp + 112)
# asm 1: movq   112(<workp=int64#1),>t32=int64#4
# asm 2: movq   112(<workp=%rdi),>t32=%rcx
movq   112(%rdi),%rcx

# qhasm: t33 = *(uint64 *)(workp + 120)
# asm 1: movq   120(<workp=int64#1),>t33=int64#5
# asm 2: movq   120(<workp=%rdi),>t33=%r8
movq   120(%rdi),%r8

# qhasm: t40 = t30
# asm 1: mov  <t30=int64#2,>t40=int64#6
# asm 2: mov  <t30=%rsi,>t40=%r9
mov  %rsi,%r9

# qhasm: t41 = t31
# asm 1: mov  <t31=int64#3,>t41=int64#7
# asm 2: mov  <t31=%rdx,>t41=%rax
mov  %rdx,%rax

# qhasm: t42 = t32
# asm 1: mov  <t32=int64#4,>t42=int64#8
# asm 2: mov  <t32=%rcx,>t42=%r10
mov  %rcx,%r10

# qhasm: t43 = t33
# asm 1: mov  <t33=int64#5,>t43=int64#9
# asm 2: mov  <t33=%r8,>t43=%r11
mov  %r8,%r11

# qhasm:   carry? t30 += *(uint64 *)(workp + 128) 
# asm 1: addq 128(<workp=int64#1),<t30=int64#2
# asm 2: addq 128(<workp=%rdi),<t30=%rsi
addq 128(%rdi),%rsi

# qhasm:   carry? t31 += *(uint64 *)(workp + 136) + carry
# asm 1: adcq 136(<workp=int64#1),<t31=int64#3
# asm 2: adcq 136(<workp=%rdi),<t31=%rdx
adcq 136(%rdi),%rdx

# qhasm:   carry? t32 += *(uint64 *)(workp + 144) + carry
# asm 1: adcq 144(<workp=int64#1),<t32=int64#4
# asm 2: adcq 144(<workp=%rdi),<t32=%rcx
adcq 144(%rdi),%rcx

# qhasm:   carry? t33 += *(uint64 *)(workp + 152) + carry
# asm 1: adcq 152(<workp=int64#1),<t33=int64#5
# asm 2: adcq 152(<workp=%rdi),<t33=%r8
adcq 152(%rdi),%r8

# qhasm:   addt0 = 0
# asm 1: mov  $0,>addt0=int64#10
# asm 2: mov  $0,>addt0=%r12
mov  $0,%r12

# qhasm:   addt1 = 38
# asm 1: mov  $38,>addt1=int64#11
# asm 2: mov  $38,>addt1=%r13
mov  $38,%r13

# qhasm:   addt1 = addt0 if !carry
# asm 1: cmovae <addt0=int64#10,<addt1=int64#11
# asm 2: cmovae <addt0=%r12,<addt1=%r13
cmovae %r12,%r13

# qhasm:   carry? t30 += addt1
# asm 1: add  <addt1=int64#11,<t30=int64#2
# asm 2: add  <addt1=%r13,<t30=%rsi
add  %r13,%rsi

# qhasm:   carry? t31 += addt0 + carry
# asm 1: adc <addt0=int64#10,<t31=int64#3
# asm 2: adc <addt0=%r12,<t31=%rdx
adc %r12,%rdx

# qhasm:   carry? t32 += addt0 + carry
# asm 1: adc <addt0=int64#10,<t32=int64#4
# asm 2: adc <addt0=%r12,<t32=%rcx
adc %r12,%rcx

# qhasm:   carry? t33 += addt0 + carry
# asm 1: adc <addt0=int64#10,<t33=int64#5
# asm 2: adc <addt0=%r12,<t33=%r8
adc %r12,%r8

# qhasm:   addt0 = addt1 if carry
# asm 1: cmovc <addt1=int64#11,<addt0=int64#10
# asm 2: cmovc <addt1=%r13,<addt0=%r12
cmovc %r13,%r12

# qhasm:   t30 += addt0
# asm 1: add  <addt0=int64#10,<t30=int64#2
# asm 2: add  <addt0=%r12,<t30=%rsi
add  %r12,%rsi

# qhasm:   carry? t40 -= *(uint64 *)(workp + 128) 
# asm 1: subq 128(<workp=int64#1),<t40=int64#6
# asm 2: subq 128(<workp=%rdi),<t40=%r9
subq 128(%rdi),%r9

# qhasm:   carry? t41 -= *(uint64 *)(workp + 136) - carry
# asm 1: sbbq 136(<workp=int64#1),<t41=int64#7
# asm 2: sbbq 136(<workp=%rdi),<t41=%rax
sbbq 136(%rdi),%rax

# qhasm:   carry? t42 -= *(uint64 *)(workp + 144) - carry
# asm 1: sbbq 144(<workp=int64#1),<t42=int64#8
# asm 2: sbbq 144(<workp=%rdi),<t42=%r10
sbbq 144(%rdi),%r10

# qhasm:   carry? t43 -= *(uint64 *)(workp + 152) - carry
# asm 1: sbbq 152(<workp=int64#1),<t43=int64#9
# asm 2: sbbq 152(<workp=%rdi),<t43=%r11
sbbq 152(%rdi),%r11

# qhasm:   subt0 = 0
# asm 1: mov  $0,>subt0=int64#10
# asm 2: mov  $0,>subt0=%r12
mov  $0,%r12

# qhasm:   subt1 = 38
# asm 1: mov  $38,>subt1=int64#11
# asm 2: mov  $38,>subt1=%r13
mov  $38,%r13

# qhasm:   subt1 = subt0 if !carry
# asm 1: cmovae <subt0=int64#10,<subt1=int64#11
# asm 2: cmovae <subt0=%r12,<subt1=%r13
cmovae %r12,%r13

# qhasm:   carry? t40 -= subt1
# asm 1: sub  <subt1=int64#11,<t40=int64#6
# asm 2: sub  <subt1=%r13,<t40=%r9
sub  %r13,%r9

# qhasm:   carry? t41 -= subt0 - carry
# asm 1: sbb  <subt0=int64#10,<t41=int64#7
# asm 2: sbb  <subt0=%r12,<t41=%rax
sbb  %r12,%rax

# qhasm:   carry? t42 -= subt0 - carry
# asm 1: sbb  <subt0=int64#10,<t42=int64#8
# asm 2: sbb  <subt0=%r12,<t42=%r10
sbb  %r12,%r10

# qhasm:   carry? t43 -= subt0 - carry
# asm 1: sbb  <subt0=int64#10,<t43=int64#9
# asm 2: sbb  <subt0=%r12,<t43=%r11
sbb  %r12,%r11

# qhasm:   subt0 = subt1 if carry
# asm 1: cmovc <subt1=int64#11,<subt0=int64#10
# asm 2: cmovc <subt1=%r13,<subt0=%r12
cmovc %r13,%r12

# qhasm:   t40 -= subt0
# asm 1: sub  <subt0=int64#10,<t40=int64#6
# asm 2: sub  <subt0=%r12,<t40=%r9
sub  %r12,%r9

# qhasm: t30_stack = t30
# asm 1: movq <t30=int64#2,>t30_stack=stack64#28
# asm 2: movq <t30=%rsi,>t30_stack=216(%rsp)
movq %rsi,216(%rsp)

# qhasm: t31_stack = t31
# asm 1: movq <t31=int64#3,>t31_stack=stack64#29
# asm 2: movq <t31=%rdx,>t31_stack=224(%rsp)
movq %rdx,224(%rsp)

# qhasm: t32_stack = t32
# asm 1: movq <t32=int64#4,>t32_stack=stack64#30
# asm 2: movq <t32=%rcx,>t32_stack=232(%rsp)
movq %rcx,232(%rsp)

# qhasm: t33_stack = t33
# asm 1: movq <t33=int64#5,>t33_stack=stack64#31
# asm 2: movq <t33=%r8,>t33_stack=240(%rsp)
movq %r8,240(%rsp)

# qhasm: t40_stack = t40
# asm 1: movq <t40=int64#6,>t40_stack=stack64#32
# asm 2: movq <t40=%r9,>t40_stack=248(%rsp)
movq %r9,248(%rsp)

# qhasm: t41_stack = t41
# asm 1: movq <t41=int64#7,>t41_stack=stack64#33
# asm 2: movq <t41=%rax,>t41_stack=256(%rsp)
movq %rax,256(%rsp)

# qhasm: t42_stack = t42
# asm 1: movq <t42=int64#8,>t42_stack=stack64#34
# asm 2: movq <t42=%r10,>t42_stack=264(%rsp)
movq %r10,264(%rsp)

# qhasm: t43_stack = t43
# asm 1: movq <t43=int64#9,>t43_stack=stack64#35
# asm 2: movq <t43=%r11,>t43_stack=272(%rsp)
movq %r11,272(%rsp)

# qhasm:   mulr4 = 0
# asm 1: mov  $0,>mulr4=int64#2
# asm 2: mov  $0,>mulr4=%rsi
mov  $0,%rsi

# qhasm:   mulr5 = 0
# asm 1: mov  $0,>mulr5=int64#4
# asm 2: mov  $0,>mulr5=%rcx
mov  $0,%rcx

# qhasm:   mulr6 = 0
# asm 1: mov  $0,>mulr6=int64#5
# asm 2: mov  $0,>mulr6=%r8
mov  $0,%r8

# qhasm:   mulr7 = 0
# asm 1: mov  $0,>mulr7=int64#6
# asm 2: mov  $0,>mulr7=%r9
mov  $0,%r9

# qhasm:   mulx0 = t30_stack
# asm 1: movq <t30_stack=stack64#28,>mulx0=int64#8
# asm 2: movq <t30_stack=216(%rsp),>mulx0=%r10
movq 216(%rsp),%r10

# qhasm:   mulrax = t20_stack
# asm 1: movq <t20_stack=stack64#12,>mulrax=int64#7
# asm 2: movq <t20_stack=88(%rsp),>mulrax=%rax
movq 88(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * mulx0
# asm 1: mul  <mulx0=int64#8
# asm 2: mul  <mulx0=%r10
mul  %r10

# qhasm:   t90 = mulrax
# asm 1: mov  <mulrax=int64#7,>t90=int64#9
# asm 2: mov  <mulrax=%rax,>t90=%r11
mov  %rax,%r11

# qhasm:   t91 = mulrdx
# asm 1: mov  <mulrdx=int64#3,>t91=int64#10
# asm 2: mov  <mulrdx=%rdx,>t91=%r12
mov  %rdx,%r12

# qhasm:   mulrax = t21_stack
# asm 1: movq <t21_stack=stack64#13,>mulrax=int64#7
# asm 2: movq <t21_stack=96(%rsp),>mulrax=%rax
movq 96(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * mulx0
# asm 1: mul  <mulx0=int64#8
# asm 2: mul  <mulx0=%r10
mul  %r10

# qhasm:   carry? t91 += mulrax
# asm 1: add  <mulrax=int64#7,<t91=int64#10
# asm 2: add  <mulrax=%rax,<t91=%r12
add  %rax,%r12

# qhasm:   t92 = 0
# asm 1: mov  $0,>t92=int64#11
# asm 2: mov  $0,>t92=%r13
mov  $0,%r13

# qhasm:   t92 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<t92=int64#11
# asm 2: adc <mulrdx=%rdx,<t92=%r13
adc %rdx,%r13

# qhasm:   mulrax = t22_stack
# asm 1: movq <t22_stack=stack64#14,>mulrax=int64#7
# asm 2: movq <t22_stack=104(%rsp),>mulrax=%rax
movq 104(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * mulx0
# asm 1: mul  <mulx0=int64#8
# asm 2: mul  <mulx0=%r10
mul  %r10

# qhasm:   carry? t92 += mulrax
# asm 1: add  <mulrax=int64#7,<t92=int64#11
# asm 2: add  <mulrax=%rax,<t92=%r13
add  %rax,%r13

# qhasm:   t93 = 0
# asm 1: mov  $0,>t93=int64#12
# asm 2: mov  $0,>t93=%r14
mov  $0,%r14

# qhasm:   t93 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<t93=int64#12
# asm 2: adc <mulrdx=%rdx,<t93=%r14
adc %rdx,%r14

# qhasm:   mulrax = t23_stack
# asm 1: movq <t23_stack=stack64#15,>mulrax=int64#7
# asm 2: movq <t23_stack=112(%rsp),>mulrax=%rax
movq 112(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * mulx0
# asm 1: mul  <mulx0=int64#8
# asm 2: mul  <mulx0=%r10
mul  %r10

# qhasm:   carry? t93 += mulrax
# asm 1: add  <mulrax=int64#7,<t93=int64#12
# asm 2: add  <mulrax=%rax,<t93=%r14
add  %rax,%r14

# qhasm:   mulr4 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr4=int64#2
# asm 2: adc <mulrdx=%rdx,<mulr4=%rsi
adc %rdx,%rsi

# qhasm:   mulx1 = t31_stack
# asm 1: movq <t31_stack=stack64#29,>mulx1=int64#8
# asm 2: movq <t31_stack=224(%rsp),>mulx1=%r10
movq 224(%rsp),%r10

# qhasm:   mulrax = t20_stack
# asm 1: movq <t20_stack=stack64#12,>mulrax=int64#7
# asm 2: movq <t20_stack=88(%rsp),>mulrax=%rax
movq 88(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * mulx1
# asm 1: mul  <mulx1=int64#8
# asm 2: mul  <mulx1=%r10
mul  %r10

# qhasm:   carry? t91 += mulrax
# asm 1: add  <mulrax=int64#7,<t91=int64#10
# asm 2: add  <mulrax=%rax,<t91=%r12
add  %rax,%r12

# qhasm:   mulc = 0
# asm 1: mov  $0,>mulc=int64#13
# asm 2: mov  $0,>mulc=%r15
mov  $0,%r15

# qhasm:   mulc += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulc=int64#13
# asm 2: adc <mulrdx=%rdx,<mulc=%r15
adc %rdx,%r15

# qhasm:   mulrax = t21_stack
# asm 1: movq <t21_stack=stack64#13,>mulrax=int64#7
# asm 2: movq <t21_stack=96(%rsp),>mulrax=%rax
movq 96(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * mulx1
# asm 1: mul  <mulx1=int64#8
# asm 2: mul  <mulx1=%r10
mul  %r10

# qhasm:   carry? t92 += mulrax
# asm 1: add  <mulrax=int64#7,<t92=int64#11
# asm 2: add  <mulrax=%rax,<t92=%r13
add  %rax,%r13

# qhasm:   mulrdx += 0 + carry
# asm 1: adc $0,<mulrdx=int64#3
# asm 2: adc $0,<mulrdx=%rdx
adc $0,%rdx

# qhasm:   carry? t92 += mulc
# asm 1: add  <mulc=int64#13,<t92=int64#11
# asm 2: add  <mulc=%r15,<t92=%r13
add  %r15,%r13

# qhasm:   mulc = 0
# asm 1: mov  $0,>mulc=int64#13
# asm 2: mov  $0,>mulc=%r15
mov  $0,%r15

# qhasm:   mulc += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulc=int64#13
# asm 2: adc <mulrdx=%rdx,<mulc=%r15
adc %rdx,%r15

# qhasm:   mulrax = t22_stack
# asm 1: movq <t22_stack=stack64#14,>mulrax=int64#7
# asm 2: movq <t22_stack=104(%rsp),>mulrax=%rax
movq 104(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * mulx1
# asm 1: mul  <mulx1=int64#8
# asm 2: mul  <mulx1=%r10
mul  %r10

# qhasm:   carry? t93 += mulrax 
# asm 1: add  <mulrax=int64#7,<t93=int64#12
# asm 2: add  <mulrax=%rax,<t93=%r14
add  %rax,%r14

# qhasm:   mulrdx += 0 + carry
# asm 1: adc $0,<mulrdx=int64#3
# asm 2: adc $0,<mulrdx=%rdx
adc $0,%rdx

# qhasm:   carry? t93 += mulc
# asm 1: add  <mulc=int64#13,<t93=int64#12
# asm 2: add  <mulc=%r15,<t93=%r14
add  %r15,%r14

# qhasm:   mulc = 0
# asm 1: mov  $0,>mulc=int64#13
# asm 2: mov  $0,>mulc=%r15
mov  $0,%r15

# qhasm:   mulc += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulc=int64#13
# asm 2: adc <mulrdx=%rdx,<mulc=%r15
adc %rdx,%r15

# qhasm:   mulrax = t23_stack
# asm 1: movq <t23_stack=stack64#15,>mulrax=int64#7
# asm 2: movq <t23_stack=112(%rsp),>mulrax=%rax
movq 112(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * mulx1
# asm 1: mul  <mulx1=int64#8
# asm 2: mul  <mulx1=%r10
mul  %r10

# qhasm:   carry? mulr4 += mulrax
# asm 1: add  <mulrax=int64#7,<mulr4=int64#2
# asm 2: add  <mulrax=%rax,<mulr4=%rsi
add  %rax,%rsi

# qhasm:   mulrdx += 0 + carry
# asm 1: adc $0,<mulrdx=int64#3
# asm 2: adc $0,<mulrdx=%rdx
adc $0,%rdx

# qhasm:   carry? mulr4 += mulc
# asm 1: add  <mulc=int64#13,<mulr4=int64#2
# asm 2: add  <mulc=%r15,<mulr4=%rsi
add  %r15,%rsi

# qhasm:   mulr5 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr5=int64#4
# asm 2: adc <mulrdx=%rdx,<mulr5=%rcx
adc %rdx,%rcx

# qhasm:   mulx2 = t32_stack
# asm 1: movq <t32_stack=stack64#30,>mulx2=int64#8
# asm 2: movq <t32_stack=232(%rsp),>mulx2=%r10
movq 232(%rsp),%r10

# qhasm:   mulrax = t20_stack
# asm 1: movq <t20_stack=stack64#12,>mulrax=int64#7
# asm 2: movq <t20_stack=88(%rsp),>mulrax=%rax
movq 88(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * mulx2
# asm 1: mul  <mulx2=int64#8
# asm 2: mul  <mulx2=%r10
mul  %r10

# qhasm:   carry? t92 += mulrax
# asm 1: add  <mulrax=int64#7,<t92=int64#11
# asm 2: add  <mulrax=%rax,<t92=%r13
add  %rax,%r13

# qhasm:   mulc = 0
# asm 1: mov  $0,>mulc=int64#13
# asm 2: mov  $0,>mulc=%r15
mov  $0,%r15

# qhasm:   mulc += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulc=int64#13
# asm 2: adc <mulrdx=%rdx,<mulc=%r15
adc %rdx,%r15

# qhasm:   mulrax = t21_stack
# asm 1: movq <t21_stack=stack64#13,>mulrax=int64#7
# asm 2: movq <t21_stack=96(%rsp),>mulrax=%rax
movq 96(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * mulx2
# asm 1: mul  <mulx2=int64#8
# asm 2: mul  <mulx2=%r10
mul  %r10

# qhasm:   carry? t93 += mulrax
# asm 1: add  <mulrax=int64#7,<t93=int64#12
# asm 2: add  <mulrax=%rax,<t93=%r14
add  %rax,%r14

# qhasm:   mulrdx += 0 + carry
# asm 1: adc $0,<mulrdx=int64#3
# asm 2: adc $0,<mulrdx=%rdx
adc $0,%rdx

# qhasm:   carry? t93 += mulc
# asm 1: add  <mulc=int64#13,<t93=int64#12
# asm 2: add  <mulc=%r15,<t93=%r14
add  %r15,%r14

# qhasm:   mulc = 0
# asm 1: mov  $0,>mulc=int64#13
# asm 2: mov  $0,>mulc=%r15
mov  $0,%r15

# qhasm:   mulc += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulc=int64#13
# asm 2: adc <mulrdx=%rdx,<mulc=%r15
adc %rdx,%r15

# qhasm:   mulrax = t22_stack
# asm 1: movq <t22_stack=stack64#14,>mulrax=int64#7
# asm 2: movq <t22_stack=104(%rsp),>mulrax=%rax
movq 104(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * mulx2
# asm 1: mul  <mulx2=int64#8
# asm 2: mul  <mulx2=%r10
mul  %r10

# qhasm:   carry? mulr4 += mulrax
# asm 1: add  <mulrax=int64#7,<mulr4=int64#2
# asm 2: add  <mulrax=%rax,<mulr4=%rsi
add  %rax,%rsi

# qhasm:   mulrdx += 0 + carry
# asm 1: adc $0,<mulrdx=int64#3
# asm 2: adc $0,<mulrdx=%rdx
adc $0,%rdx

# qhasm:   carry? mulr4 += mulc
# asm 1: add  <mulc=int64#13,<mulr4=int64#2
# asm 2: add  <mulc=%r15,<mulr4=%rsi
add  %r15,%rsi

# qhasm:   mulc = 0
# asm 1: mov  $0,>mulc=int64#13
# asm 2: mov  $0,>mulc=%r15
mov  $0,%r15

# qhasm:   mulc += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulc=int64#13
# asm 2: adc <mulrdx=%rdx,<mulc=%r15
adc %rdx,%r15

# qhasm:   mulrax = t23_stack
# asm 1: movq <t23_stack=stack64#15,>mulrax=int64#7
# asm 2: movq <t23_stack=112(%rsp),>mulrax=%rax
movq 112(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * mulx2
# asm 1: mul  <mulx2=int64#8
# asm 2: mul  <mulx2=%r10
mul  %r10

# qhasm:   carry? mulr5 += mulrax
# asm 1: add  <mulrax=int64#7,<mulr5=int64#4
# asm 2: add  <mulrax=%rax,<mulr5=%rcx
add  %rax,%rcx

# qhasm:   mulrdx += 0 + carry
# asm 1: adc $0,<mulrdx=int64#3
# asm 2: adc $0,<mulrdx=%rdx
adc $0,%rdx

# qhasm:   carry? mulr5 += mulc
# asm 1: add  <mulc=int64#13,<mulr5=int64#4
# asm 2: add  <mulc=%r15,<mulr5=%rcx
add  %r15,%rcx

# qhasm:   mulr6 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr6=int64#5
# asm 2: adc <mulrdx=%rdx,<mulr6=%r8
adc %rdx,%r8

# qhasm:   mulx3 = t33_stack
# asm 1: movq <t33_stack=stack64#31,>mulx3=int64#8
# asm 2: movq <t33_stack=240(%rsp),>mulx3=%r10
movq 240(%rsp),%r10

# qhasm:   mulrax = t20_stack
# asm 1: movq <t20_stack=stack64#12,>mulrax=int64#7
# asm 2: movq <t20_stack=88(%rsp),>mulrax=%rax
movq 88(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * mulx3
# asm 1: mul  <mulx3=int64#8
# asm 2: mul  <mulx3=%r10
mul  %r10

# qhasm:   carry? t93 += mulrax
# asm 1: add  <mulrax=int64#7,<t93=int64#12
# asm 2: add  <mulrax=%rax,<t93=%r14
add  %rax,%r14

# qhasm:   mulc = 0
# asm 1: mov  $0,>mulc=int64#13
# asm 2: mov  $0,>mulc=%r15
mov  $0,%r15

# qhasm:   mulc += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulc=int64#13
# asm 2: adc <mulrdx=%rdx,<mulc=%r15
adc %rdx,%r15

# qhasm:   mulrax = t21_stack
# asm 1: movq <t21_stack=stack64#13,>mulrax=int64#7
# asm 2: movq <t21_stack=96(%rsp),>mulrax=%rax
movq 96(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * mulx3
# asm 1: mul  <mulx3=int64#8
# asm 2: mul  <mulx3=%r10
mul  %r10

# qhasm:   carry? mulr4 += mulrax
# asm 1: add  <mulrax=int64#7,<mulr4=int64#2
# asm 2: add  <mulrax=%rax,<mulr4=%rsi
add  %rax,%rsi

# qhasm:   mulrdx += 0 + carry
# asm 1: adc $0,<mulrdx=int64#3
# asm 2: adc $0,<mulrdx=%rdx
adc $0,%rdx

# qhasm:   carry? mulr4 += mulc
# asm 1: add  <mulc=int64#13,<mulr4=int64#2
# asm 2: add  <mulc=%r15,<mulr4=%rsi
add  %r15,%rsi

# qhasm:   mulc = 0
# asm 1: mov  $0,>mulc=int64#13
# asm 2: mov  $0,>mulc=%r15
mov  $0,%r15

# qhasm:   mulc += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulc=int64#13
# asm 2: adc <mulrdx=%rdx,<mulc=%r15
adc %rdx,%r15

# qhasm:   mulrax = t22_stack
# asm 1: movq <t22_stack=stack64#14,>mulrax=int64#7
# asm 2: movq <t22_stack=104(%rsp),>mulrax=%rax
movq 104(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * mulx3
# asm 1: mul  <mulx3=int64#8
# asm 2: mul  <mulx3=%r10
mul  %r10

# qhasm:   carry? mulr5 += mulrax
# asm 1: add  <mulrax=int64#7,<mulr5=int64#4
# asm 2: add  <mulrax=%rax,<mulr5=%rcx
add  %rax,%rcx

# qhasm:   mulrdx += 0 + carry
# asm 1: adc $0,<mulrdx=int64#3
# asm 2: adc $0,<mulrdx=%rdx
adc $0,%rdx

# qhasm:   carry? mulr5 += mulc
# asm 1: add  <mulc=int64#13,<mulr5=int64#4
# asm 2: add  <mulc=%r15,<mulr5=%rcx
add  %r15,%rcx

# qhasm:   mulc = 0
# asm 1: mov  $0,>mulc=int64#13
# asm 2: mov  $0,>mulc=%r15
mov  $0,%r15

# qhasm:   mulc += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulc=int64#13
# asm 2: adc <mulrdx=%rdx,<mulc=%r15
adc %rdx,%r15

# qhasm:   mulrax = t23_stack
# asm 1: movq <t23_stack=stack64#15,>mulrax=int64#7
# asm 2: movq <t23_stack=112(%rsp),>mulrax=%rax
movq 112(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * mulx3
# asm 1: mul  <mulx3=int64#8
# asm 2: mul  <mulx3=%r10
mul  %r10

# qhasm:   carry? mulr6 += mulrax
# asm 1: add  <mulrax=int64#7,<mulr6=int64#5
# asm 2: add  <mulrax=%rax,<mulr6=%r8
add  %rax,%r8

# qhasm:   mulrdx += 0 + carry
# asm 1: adc $0,<mulrdx=int64#3
# asm 2: adc $0,<mulrdx=%rdx
adc $0,%rdx

# qhasm:   carry? mulr6 += mulc
# asm 1: add  <mulc=int64#13,<mulr6=int64#5
# asm 2: add  <mulc=%r15,<mulr6=%r8
add  %r15,%r8

# qhasm:   mulr7 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr7=int64#6
# asm 2: adc <mulrdx=%rdx,<mulr7=%r9
adc %rdx,%r9

# qhasm:   mulrax = mulr4
# asm 1: mov  <mulr4=int64#2,>mulrax=int64#7
# asm 2: mov  <mulr4=%rsi,>mulrax=%rax
mov  %rsi,%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * *(uint64 *)&crypto_scalarmult_curve25519_amd64_64_38
mulq  crypto_scalarmult_curve25519_amd64_64_38

# qhasm:   mulr4 = mulrax
# asm 1: mov  <mulrax=int64#7,>mulr4=int64#2
# asm 2: mov  <mulrax=%rax,>mulr4=%rsi
mov  %rax,%rsi

# qhasm:   mulrax = mulr5
# asm 1: mov  <mulr5=int64#4,>mulrax=int64#7
# asm 2: mov  <mulr5=%rcx,>mulrax=%rax
mov  %rcx,%rax

# qhasm:   mulr5 = mulrdx
# asm 1: mov  <mulrdx=int64#3,>mulr5=int64#4
# asm 2: mov  <mulrdx=%rdx,>mulr5=%rcx
mov  %rdx,%rcx

# qhasm:   (uint128) mulrdx mulrax = mulrax * *(uint64 *)&crypto_scalarmult_curve25519_amd64_64_38
mulq  crypto_scalarmult_curve25519_amd64_64_38

# qhasm:   carry? mulr5 += mulrax
# asm 1: add  <mulrax=int64#7,<mulr5=int64#4
# asm 2: add  <mulrax=%rax,<mulr5=%rcx
add  %rax,%rcx

# qhasm:   mulrax = mulr6
# asm 1: mov  <mulr6=int64#5,>mulrax=int64#7
# asm 2: mov  <mulr6=%r8,>mulrax=%rax
mov  %r8,%rax

# qhasm:   mulr6 = 0
# asm 1: mov  $0,>mulr6=int64#5
# asm 2: mov  $0,>mulr6=%r8
mov  $0,%r8

# qhasm:   mulr6 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr6=int64#5
# asm 2: adc <mulrdx=%rdx,<mulr6=%r8
adc %rdx,%r8

# qhasm:   (uint128) mulrdx mulrax = mulrax * *(uint64 *)&crypto_scalarmult_curve25519_amd64_64_38
mulq  crypto_scalarmult_curve25519_amd64_64_38

# qhasm:   carry? mulr6 += mulrax
# asm 1: add  <mulrax=int64#7,<mulr6=int64#5
# asm 2: add  <mulrax=%rax,<mulr6=%r8
add  %rax,%r8

# qhasm:   mulrax = mulr7
# asm 1: mov  <mulr7=int64#6,>mulrax=int64#7
# asm 2: mov  <mulr7=%r9,>mulrax=%rax
mov  %r9,%rax

# qhasm:   mulr7 = 0
# asm 1: mov  $0,>mulr7=int64#6
# asm 2: mov  $0,>mulr7=%r9
mov  $0,%r9

# qhasm:   mulr7 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr7=int64#6
# asm 2: adc <mulrdx=%rdx,<mulr7=%r9
adc %rdx,%r9

# qhasm:   (uint128) mulrdx mulrax = mulrax * *(uint64 *)&crypto_scalarmult_curve25519_amd64_64_38
mulq  crypto_scalarmult_curve25519_amd64_64_38

# qhasm:   carry? mulr7 += mulrax
# asm 1: add  <mulrax=int64#7,<mulr7=int64#6
# asm 2: add  <mulrax=%rax,<mulr7=%r9
add  %rax,%r9

# qhasm:   mulr8 = 0
# asm 1: mov  $0,>mulr8=int64#7
# asm 2: mov  $0,>mulr8=%rax
mov  $0,%rax

# qhasm:   mulr8 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr8=int64#7
# asm 2: adc <mulrdx=%rdx,<mulr8=%rax
adc %rdx,%rax

# qhasm:   carry? t90 += mulr4
# asm 1: add  <mulr4=int64#2,<t90=int64#9
# asm 2: add  <mulr4=%rsi,<t90=%r11
add  %rsi,%r11

# qhasm:   carry? t91 += mulr5 + carry
# asm 1: adc <mulr5=int64#4,<t91=int64#10
# asm 2: adc <mulr5=%rcx,<t91=%r12
adc %rcx,%r12

# qhasm:   carry? t92 += mulr6 + carry
# asm 1: adc <mulr6=int64#5,<t92=int64#11
# asm 2: adc <mulr6=%r8,<t92=%r13
adc %r8,%r13

# qhasm:   carry? t93 += mulr7 + carry
# asm 1: adc <mulr7=int64#6,<t93=int64#12
# asm 2: adc <mulr7=%r9,<t93=%r14
adc %r9,%r14

# qhasm:   mulzero = 0
# asm 1: mov  $0,>mulzero=int64#2
# asm 2: mov  $0,>mulzero=%rsi
mov  $0,%rsi

# qhasm:   mulr8 += mulzero + carry
# asm 1: adc <mulzero=int64#2,<mulr8=int64#7
# asm 2: adc <mulzero=%rsi,<mulr8=%rax
adc %rsi,%rax

# qhasm:   mulr8 *= 38
# asm 1: imulq  $38,<mulr8=int64#7,>mulr8=int64#3
# asm 2: imulq  $38,<mulr8=%rax,>mulr8=%rdx
imulq  $38,%rax,%rdx

# qhasm:   carry? t90 += mulr8
# asm 1: add  <mulr8=int64#3,<t90=int64#9
# asm 2: add  <mulr8=%rdx,<t90=%r11
add  %rdx,%r11

# qhasm:   carry? t91 += mulzero + carry
# asm 1: adc <mulzero=int64#2,<t91=int64#10
# asm 2: adc <mulzero=%rsi,<t91=%r12
adc %rsi,%r12

# qhasm:   carry? t92 += mulzero + carry
# asm 1: adc <mulzero=int64#2,<t92=int64#11
# asm 2: adc <mulzero=%rsi,<t92=%r13
adc %rsi,%r13

# qhasm:   carry? t93 += mulzero + carry
# asm 1: adc <mulzero=int64#2,<t93=int64#12
# asm 2: adc <mulzero=%rsi,<t93=%r14
adc %rsi,%r14

# qhasm:   mulzero += mulzero + carry
# asm 1: adc <mulzero=int64#2,<mulzero=int64#2
# asm 2: adc <mulzero=%rsi,<mulzero=%rsi
adc %rsi,%rsi

# qhasm:   mulzero *= 38
# asm 1: imulq  $38,<mulzero=int64#2,>mulzero=int64#2
# asm 2: imulq  $38,<mulzero=%rsi,>mulzero=%rsi
imulq  $38,%rsi,%rsi

# qhasm:   t90 += mulzero
# asm 1: add  <mulzero=int64#2,<t90=int64#9
# asm 2: add  <mulzero=%rsi,<t90=%r11
add  %rsi,%r11

# qhasm: t90_stack = t90
# asm 1: movq <t90=int64#9,>t90_stack=stack64#12
# asm 2: movq <t90=%r11,>t90_stack=88(%rsp)
movq %r11,88(%rsp)

# qhasm: t91_stack = t91
# asm 1: movq <t91=int64#10,>t91_stack=stack64#13
# asm 2: movq <t91=%r12,>t91_stack=96(%rsp)
movq %r12,96(%rsp)

# qhasm: t92_stack = t92
# asm 1: movq <t92=int64#11,>t92_stack=stack64#14
# asm 2: movq <t92=%r13,>t92_stack=104(%rsp)
movq %r13,104(%rsp)

# qhasm: t93_stack = t93
# asm 1: movq <t93=int64#12,>t93_stack=stack64#15
# asm 2: movq <t93=%r14,>t93_stack=112(%rsp)
movq %r14,112(%rsp)

# qhasm:   mulr4 = 0
# asm 1: mov  $0,>mulr4=int64#2
# asm 2: mov  $0,>mulr4=%rsi
mov  $0,%rsi

# qhasm:   mulr5 = 0
# asm 1: mov  $0,>mulr5=int64#4
# asm 2: mov  $0,>mulr5=%rcx
mov  $0,%rcx

# qhasm:   mulr6 = 0
# asm 1: mov  $0,>mulr6=int64#5
# asm 2: mov  $0,>mulr6=%r8
mov  $0,%r8

# qhasm:   mulr7 = 0
# asm 1: mov  $0,>mulr7=int64#6
# asm 2: mov  $0,>mulr7=%r9
mov  $0,%r9

# qhasm:   mulx0 = t40_stack
# asm 1: movq <t40_stack=stack64#32,>mulx0=int64#8
# asm 2: movq <t40_stack=248(%rsp),>mulx0=%r10
movq 248(%rsp),%r10

# qhasm:   mulrax = t10_stack
# asm 1: movq <t10_stack=stack64#8,>mulrax=int64#7
# asm 2: movq <t10_stack=56(%rsp),>mulrax=%rax
movq 56(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * mulx0
# asm 1: mul  <mulx0=int64#8
# asm 2: mul  <mulx0=%r10
mul  %r10

# qhasm:   t80 = mulrax
# asm 1: mov  <mulrax=int64#7,>t80=int64#9
# asm 2: mov  <mulrax=%rax,>t80=%r11
mov  %rax,%r11

# qhasm:   t81 = mulrdx
# asm 1: mov  <mulrdx=int64#3,>t81=int64#10
# asm 2: mov  <mulrdx=%rdx,>t81=%r12
mov  %rdx,%r12

# qhasm:   mulrax = t11_stack
# asm 1: movq <t11_stack=stack64#9,>mulrax=int64#7
# asm 2: movq <t11_stack=64(%rsp),>mulrax=%rax
movq 64(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * mulx0
# asm 1: mul  <mulx0=int64#8
# asm 2: mul  <mulx0=%r10
mul  %r10

# qhasm:   carry? t81 += mulrax
# asm 1: add  <mulrax=int64#7,<t81=int64#10
# asm 2: add  <mulrax=%rax,<t81=%r12
add  %rax,%r12

# qhasm:   t82 = 0
# asm 1: mov  $0,>t82=int64#11
# asm 2: mov  $0,>t82=%r13
mov  $0,%r13

# qhasm:   t82 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<t82=int64#11
# asm 2: adc <mulrdx=%rdx,<t82=%r13
adc %rdx,%r13

# qhasm:   mulrax = t12_stack
# asm 1: movq <t12_stack=stack64#10,>mulrax=int64#7
# asm 2: movq <t12_stack=72(%rsp),>mulrax=%rax
movq 72(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * mulx0
# asm 1: mul  <mulx0=int64#8
# asm 2: mul  <mulx0=%r10
mul  %r10

# qhasm:   carry? t82 += mulrax
# asm 1: add  <mulrax=int64#7,<t82=int64#11
# asm 2: add  <mulrax=%rax,<t82=%r13
add  %rax,%r13

# qhasm:   t83 = 0
# asm 1: mov  $0,>t83=int64#12
# asm 2: mov  $0,>t83=%r14
mov  $0,%r14

# qhasm:   t83 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<t83=int64#12
# asm 2: adc <mulrdx=%rdx,<t83=%r14
adc %rdx,%r14

# qhasm:   mulrax = t13_stack
# asm 1: movq <t13_stack=stack64#11,>mulrax=int64#7
# asm 2: movq <t13_stack=80(%rsp),>mulrax=%rax
movq 80(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * mulx0
# asm 1: mul  <mulx0=int64#8
# asm 2: mul  <mulx0=%r10
mul  %r10

# qhasm:   carry? t83 += mulrax
# asm 1: add  <mulrax=int64#7,<t83=int64#12
# asm 2: add  <mulrax=%rax,<t83=%r14
add  %rax,%r14

# qhasm:   mulr4 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr4=int64#2
# asm 2: adc <mulrdx=%rdx,<mulr4=%rsi
adc %rdx,%rsi

# qhasm:   mulx1 = t41_stack
# asm 1: movq <t41_stack=stack64#33,>mulx1=int64#8
# asm 2: movq <t41_stack=256(%rsp),>mulx1=%r10
movq 256(%rsp),%r10

# qhasm:   mulrax = t10_stack
# asm 1: movq <t10_stack=stack64#8,>mulrax=int64#7
# asm 2: movq <t10_stack=56(%rsp),>mulrax=%rax
movq 56(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * mulx1
# asm 1: mul  <mulx1=int64#8
# asm 2: mul  <mulx1=%r10
mul  %r10

# qhasm:   carry? t81 += mulrax
# asm 1: add  <mulrax=int64#7,<t81=int64#10
# asm 2: add  <mulrax=%rax,<t81=%r12
add  %rax,%r12

# qhasm:   mulc = 0
# asm 1: mov  $0,>mulc=int64#13
# asm 2: mov  $0,>mulc=%r15
mov  $0,%r15

# qhasm:   mulc += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulc=int64#13
# asm 2: adc <mulrdx=%rdx,<mulc=%r15
adc %rdx,%r15

# qhasm:   mulrax = t11_stack
# asm 1: movq <t11_stack=stack64#9,>mulrax=int64#7
# asm 2: movq <t11_stack=64(%rsp),>mulrax=%rax
movq 64(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * mulx1
# asm 1: mul  <mulx1=int64#8
# asm 2: mul  <mulx1=%r10
mul  %r10

# qhasm:   carry? t82 += mulrax
# asm 1: add  <mulrax=int64#7,<t82=int64#11
# asm 2: add  <mulrax=%rax,<t82=%r13
add  %rax,%r13

# qhasm:   mulrdx += 0 + carry
# asm 1: adc $0,<mulrdx=int64#3
# asm 2: adc $0,<mulrdx=%rdx
adc $0,%rdx

# qhasm:   carry? t82 += mulc
# asm 1: add  <mulc=int64#13,<t82=int64#11
# asm 2: add  <mulc=%r15,<t82=%r13
add  %r15,%r13

# qhasm:   mulc = 0
# asm 1: mov  $0,>mulc=int64#13
# asm 2: mov  $0,>mulc=%r15
mov  $0,%r15

# qhasm:   mulc += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulc=int64#13
# asm 2: adc <mulrdx=%rdx,<mulc=%r15
adc %rdx,%r15

# qhasm:   mulrax = t12_stack
# asm 1: movq <t12_stack=stack64#10,>mulrax=int64#7
# asm 2: movq <t12_stack=72(%rsp),>mulrax=%rax
movq 72(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * mulx1
# asm 1: mul  <mulx1=int64#8
# asm 2: mul  <mulx1=%r10
mul  %r10

# qhasm:   carry? t83 += mulrax 
# asm 1: add  <mulrax=int64#7,<t83=int64#12
# asm 2: add  <mulrax=%rax,<t83=%r14
add  %rax,%r14

# qhasm:   mulrdx += 0 + carry
# asm 1: adc $0,<mulrdx=int64#3
# asm 2: adc $0,<mulrdx=%rdx
adc $0,%rdx

# qhasm:   carry? t83 += mulc
# asm 1: add  <mulc=int64#13,<t83=int64#12
# asm 2: add  <mulc=%r15,<t83=%r14
add  %r15,%r14

# qhasm:   mulc = 0
# asm 1: mov  $0,>mulc=int64#13
# asm 2: mov  $0,>mulc=%r15
mov  $0,%r15

# qhasm:   mulc += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulc=int64#13
# asm 2: adc <mulrdx=%rdx,<mulc=%r15
adc %rdx,%r15

# qhasm:   mulrax = t13_stack
# asm 1: movq <t13_stack=stack64#11,>mulrax=int64#7
# asm 2: movq <t13_stack=80(%rsp),>mulrax=%rax
movq 80(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * mulx1
# asm 1: mul  <mulx1=int64#8
# asm 2: mul  <mulx1=%r10
mul  %r10

# qhasm:   carry? mulr4 += mulrax
# asm 1: add  <mulrax=int64#7,<mulr4=int64#2
# asm 2: add  <mulrax=%rax,<mulr4=%rsi
add  %rax,%rsi

# qhasm:   mulrdx += 0 + carry
# asm 1: adc $0,<mulrdx=int64#3
# asm 2: adc $0,<mulrdx=%rdx
adc $0,%rdx

# qhasm:   carry? mulr4 += mulc
# asm 1: add  <mulc=int64#13,<mulr4=int64#2
# asm 2: add  <mulc=%r15,<mulr4=%rsi
add  %r15,%rsi

# qhasm:   mulr5 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr5=int64#4
# asm 2: adc <mulrdx=%rdx,<mulr5=%rcx
adc %rdx,%rcx

# qhasm:   mulx2 = t42_stack
# asm 1: movq <t42_stack=stack64#34,>mulx2=int64#8
# asm 2: movq <t42_stack=264(%rsp),>mulx2=%r10
movq 264(%rsp),%r10

# qhasm:   mulrax = t10_stack
# asm 1: movq <t10_stack=stack64#8,>mulrax=int64#7
# asm 2: movq <t10_stack=56(%rsp),>mulrax=%rax
movq 56(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * mulx2
# asm 1: mul  <mulx2=int64#8
# asm 2: mul  <mulx2=%r10
mul  %r10

# qhasm:   carry? t82 += mulrax
# asm 1: add  <mulrax=int64#7,<t82=int64#11
# asm 2: add  <mulrax=%rax,<t82=%r13
add  %rax,%r13

# qhasm:   mulc = 0
# asm 1: mov  $0,>mulc=int64#13
# asm 2: mov  $0,>mulc=%r15
mov  $0,%r15

# qhasm:   mulc += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulc=int64#13
# asm 2: adc <mulrdx=%rdx,<mulc=%r15
adc %rdx,%r15

# qhasm:   mulrax = t11_stack
# asm 1: movq <t11_stack=stack64#9,>mulrax=int64#7
# asm 2: movq <t11_stack=64(%rsp),>mulrax=%rax
movq 64(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * mulx2
# asm 1: mul  <mulx2=int64#8
# asm 2: mul  <mulx2=%r10
mul  %r10

# qhasm:   carry? t83 += mulrax
# asm 1: add  <mulrax=int64#7,<t83=int64#12
# asm 2: add  <mulrax=%rax,<t83=%r14
add  %rax,%r14

# qhasm:   mulrdx += 0 + carry
# asm 1: adc $0,<mulrdx=int64#3
# asm 2: adc $0,<mulrdx=%rdx
adc $0,%rdx

# qhasm:   carry? t83 += mulc
# asm 1: add  <mulc=int64#13,<t83=int64#12
# asm 2: add  <mulc=%r15,<t83=%r14
add  %r15,%r14

# qhasm:   mulc = 0
# asm 1: mov  $0,>mulc=int64#13
# asm 2: mov  $0,>mulc=%r15
mov  $0,%r15

# qhasm:   mulc += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulc=int64#13
# asm 2: adc <mulrdx=%rdx,<mulc=%r15
adc %rdx,%r15

# qhasm:   mulrax = t12_stack
# asm 1: movq <t12_stack=stack64#10,>mulrax=int64#7
# asm 2: movq <t12_stack=72(%rsp),>mulrax=%rax
movq 72(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * mulx2
# asm 1: mul  <mulx2=int64#8
# asm 2: mul  <mulx2=%r10
mul  %r10

# qhasm:   carry? mulr4 += mulrax
# asm 1: add  <mulrax=int64#7,<mulr4=int64#2
# asm 2: add  <mulrax=%rax,<mulr4=%rsi
add  %rax,%rsi

# qhasm:   mulrdx += 0 + carry
# asm 1: adc $0,<mulrdx=int64#3
# asm 2: adc $0,<mulrdx=%rdx
adc $0,%rdx

# qhasm:   carry? mulr4 += mulc
# asm 1: add  <mulc=int64#13,<mulr4=int64#2
# asm 2: add  <mulc=%r15,<mulr4=%rsi
add  %r15,%rsi

# qhasm:   mulc = 0
# asm 1: mov  $0,>mulc=int64#13
# asm 2: mov  $0,>mulc=%r15
mov  $0,%r15

# qhasm:   mulc += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulc=int64#13
# asm 2: adc <mulrdx=%rdx,<mulc=%r15
adc %rdx,%r15

# qhasm:   mulrax = t13_stack
# asm 1: movq <t13_stack=stack64#11,>mulrax=int64#7
# asm 2: movq <t13_stack=80(%rsp),>mulrax=%rax
movq 80(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * mulx2
# asm 1: mul  <mulx2=int64#8
# asm 2: mul  <mulx2=%r10
mul  %r10

# qhasm:   carry? mulr5 += mulrax
# asm 1: add  <mulrax=int64#7,<mulr5=int64#4
# asm 2: add  <mulrax=%rax,<mulr5=%rcx
add  %rax,%rcx

# qhasm:   mulrdx += 0 + carry
# asm 1: adc $0,<mulrdx=int64#3
# asm 2: adc $0,<mulrdx=%rdx
adc $0,%rdx

# qhasm:   carry? mulr5 += mulc
# asm 1: add  <mulc=int64#13,<mulr5=int64#4
# asm 2: add  <mulc=%r15,<mulr5=%rcx
add  %r15,%rcx

# qhasm:   mulr6 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr6=int64#5
# asm 2: adc <mulrdx=%rdx,<mulr6=%r8
adc %rdx,%r8

# qhasm:   mulx3 = t43_stack
# asm 1: movq <t43_stack=stack64#35,>mulx3=int64#8
# asm 2: movq <t43_stack=272(%rsp),>mulx3=%r10
movq 272(%rsp),%r10

# qhasm:   mulrax = t10_stack
# asm 1: movq <t10_stack=stack64#8,>mulrax=int64#7
# asm 2: movq <t10_stack=56(%rsp),>mulrax=%rax
movq 56(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * mulx3
# asm 1: mul  <mulx3=int64#8
# asm 2: mul  <mulx3=%r10
mul  %r10

# qhasm:   carry? t83 += mulrax
# asm 1: add  <mulrax=int64#7,<t83=int64#12
# asm 2: add  <mulrax=%rax,<t83=%r14
add  %rax,%r14

# qhasm:   mulc = 0
# asm 1: mov  $0,>mulc=int64#13
# asm 2: mov  $0,>mulc=%r15
mov  $0,%r15

# qhasm:   mulc += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulc=int64#13
# asm 2: adc <mulrdx=%rdx,<mulc=%r15
adc %rdx,%r15

# qhasm:   mulrax = t11_stack
# asm 1: movq <t11_stack=stack64#9,>mulrax=int64#7
# asm 2: movq <t11_stack=64(%rsp),>mulrax=%rax
movq 64(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * mulx3
# asm 1: mul  <mulx3=int64#8
# asm 2: mul  <mulx3=%r10
mul  %r10

# qhasm:   carry? mulr4 += mulrax
# asm 1: add  <mulrax=int64#7,<mulr4=int64#2
# asm 2: add  <mulrax=%rax,<mulr4=%rsi
add  %rax,%rsi

# qhasm:   mulrdx += 0 + carry
# asm 1: adc $0,<mulrdx=int64#3
# asm 2: adc $0,<mulrdx=%rdx
adc $0,%rdx

# qhasm:   carry? mulr4 += mulc
# asm 1: add  <mulc=int64#13,<mulr4=int64#2
# asm 2: add  <mulc=%r15,<mulr4=%rsi
add  %r15,%rsi

# qhasm:   mulc = 0
# asm 1: mov  $0,>mulc=int64#13
# asm 2: mov  $0,>mulc=%r15
mov  $0,%r15

# qhasm:   mulc += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulc=int64#13
# asm 2: adc <mulrdx=%rdx,<mulc=%r15
adc %rdx,%r15

# qhasm:   mulrax = t12_stack
# asm 1: movq <t12_stack=stack64#10,>mulrax=int64#7
# asm 2: movq <t12_stack=72(%rsp),>mulrax=%rax
movq 72(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * mulx3
# asm 1: mul  <mulx3=int64#8
# asm 2: mul  <mulx3=%r10
mul  %r10

# qhasm:   carry? mulr5 += mulrax
# asm 1: add  <mulrax=int64#7,<mulr5=int64#4
# asm 2: add  <mulrax=%rax,<mulr5=%rcx
add  %rax,%rcx

# qhasm:   mulrdx += 0 + carry
# asm 1: adc $0,<mulrdx=int64#3
# asm 2: adc $0,<mulrdx=%rdx
adc $0,%rdx

# qhasm:   carry? mulr5 += mulc
# asm 1: add  <mulc=int64#13,<mulr5=int64#4
# asm 2: add  <mulc=%r15,<mulr5=%rcx
add  %r15,%rcx

# qhasm:   mulc = 0
# asm 1: mov  $0,>mulc=int64#13
# asm 2: mov  $0,>mulc=%r15
mov  $0,%r15

# qhasm:   mulc += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulc=int64#13
# asm 2: adc <mulrdx=%rdx,<mulc=%r15
adc %rdx,%r15

# qhasm:   mulrax = t13_stack
# asm 1: movq <t13_stack=stack64#11,>mulrax=int64#7
# asm 2: movq <t13_stack=80(%rsp),>mulrax=%rax
movq 80(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * mulx3
# asm 1: mul  <mulx3=int64#8
# asm 2: mul  <mulx3=%r10
mul  %r10

# qhasm:   carry? mulr6 += mulrax
# asm 1: add  <mulrax=int64#7,<mulr6=int64#5
# asm 2: add  <mulrax=%rax,<mulr6=%r8
add  %rax,%r8

# qhasm:   mulrdx += 0 + carry
# asm 1: adc $0,<mulrdx=int64#3
# asm 2: adc $0,<mulrdx=%rdx
adc $0,%rdx

# qhasm:   carry? mulr6 += mulc
# asm 1: add  <mulc=int64#13,<mulr6=int64#5
# asm 2: add  <mulc=%r15,<mulr6=%r8
add  %r15,%r8

# qhasm:   mulr7 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr7=int64#6
# asm 2: adc <mulrdx=%rdx,<mulr7=%r9
adc %rdx,%r9

# qhasm:   mulrax = mulr4
# asm 1: mov  <mulr4=int64#2,>mulrax=int64#7
# asm 2: mov  <mulr4=%rsi,>mulrax=%rax
mov  %rsi,%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * *(uint64 *)&crypto_scalarmult_curve25519_amd64_64_38
mulq  crypto_scalarmult_curve25519_amd64_64_38

# qhasm:   mulr4 = mulrax
# asm 1: mov  <mulrax=int64#7,>mulr4=int64#2
# asm 2: mov  <mulrax=%rax,>mulr4=%rsi
mov  %rax,%rsi

# qhasm:   mulrax = mulr5
# asm 1: mov  <mulr5=int64#4,>mulrax=int64#7
# asm 2: mov  <mulr5=%rcx,>mulrax=%rax
mov  %rcx,%rax

# qhasm:   mulr5 = mulrdx
# asm 1: mov  <mulrdx=int64#3,>mulr5=int64#4
# asm 2: mov  <mulrdx=%rdx,>mulr5=%rcx
mov  %rdx,%rcx

# qhasm:   (uint128) mulrdx mulrax = mulrax * *(uint64 *)&crypto_scalarmult_curve25519_amd64_64_38
mulq  crypto_scalarmult_curve25519_amd64_64_38

# qhasm:   carry? mulr5 += mulrax
# asm 1: add  <mulrax=int64#7,<mulr5=int64#4
# asm 2: add  <mulrax=%rax,<mulr5=%rcx
add  %rax,%rcx

# qhasm:   mulrax = mulr6
# asm 1: mov  <mulr6=int64#5,>mulrax=int64#7
# asm 2: mov  <mulr6=%r8,>mulrax=%rax
mov  %r8,%rax

# qhasm:   mulr6 = 0
# asm 1: mov  $0,>mulr6=int64#5
# asm 2: mov  $0,>mulr6=%r8
mov  $0,%r8

# qhasm:   mulr6 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr6=int64#5
# asm 2: adc <mulrdx=%rdx,<mulr6=%r8
adc %rdx,%r8

# qhasm:   (uint128) mulrdx mulrax = mulrax * *(uint64 *)&crypto_scalarmult_curve25519_amd64_64_38
mulq  crypto_scalarmult_curve25519_amd64_64_38

# qhasm:   carry? mulr6 += mulrax
# asm 1: add  <mulrax=int64#7,<mulr6=int64#5
# asm 2: add  <mulrax=%rax,<mulr6=%r8
add  %rax,%r8

# qhasm:   mulrax = mulr7
# asm 1: mov  <mulr7=int64#6,>mulrax=int64#7
# asm 2: mov  <mulr7=%r9,>mulrax=%rax
mov  %r9,%rax

# qhasm:   mulr7 = 0
# asm 1: mov  $0,>mulr7=int64#6
# asm 2: mov  $0,>mulr7=%r9
mov  $0,%r9

# qhasm:   mulr7 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr7=int64#6
# asm 2: adc <mulrdx=%rdx,<mulr7=%r9
adc %rdx,%r9

# qhasm:   (uint128) mulrdx mulrax = mulrax * *(uint64 *)&crypto_scalarmult_curve25519_amd64_64_38
mulq  crypto_scalarmult_curve25519_amd64_64_38

# qhasm:   carry? mulr7 += mulrax
# asm 1: add  <mulrax=int64#7,<mulr7=int64#6
# asm 2: add  <mulrax=%rax,<mulr7=%r9
add  %rax,%r9

# qhasm:   mulr8 = 0
# asm 1: mov  $0,>mulr8=int64#7
# asm 2: mov  $0,>mulr8=%rax
mov  $0,%rax

# qhasm:   mulr8 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr8=int64#7
# asm 2: adc <mulrdx=%rdx,<mulr8=%rax
adc %rdx,%rax

# qhasm:   carry? t80 += mulr4
# asm 1: add  <mulr4=int64#2,<t80=int64#9
# asm 2: add  <mulr4=%rsi,<t80=%r11
add  %rsi,%r11

# qhasm:   carry? t81 += mulr5 + carry
# asm 1: adc <mulr5=int64#4,<t81=int64#10
# asm 2: adc <mulr5=%rcx,<t81=%r12
adc %rcx,%r12

# qhasm:   carry? t82 += mulr6 + carry
# asm 1: adc <mulr6=int64#5,<t82=int64#11
# asm 2: adc <mulr6=%r8,<t82=%r13
adc %r8,%r13

# qhasm:   carry? t83 += mulr7 + carry
# asm 1: adc <mulr7=int64#6,<t83=int64#12
# asm 2: adc <mulr7=%r9,<t83=%r14
adc %r9,%r14

# qhasm:   mulzero = 0
# asm 1: mov  $0,>mulzero=int64#2
# asm 2: mov  $0,>mulzero=%rsi
mov  $0,%rsi

# qhasm:   mulr8 += mulzero + carry
# asm 1: adc <mulzero=int64#2,<mulr8=int64#7
# asm 2: adc <mulzero=%rsi,<mulr8=%rax
adc %rsi,%rax

# qhasm:   mulr8 *= 38
# asm 1: imulq  $38,<mulr8=int64#7,>mulr8=int64#3
# asm 2: imulq  $38,<mulr8=%rax,>mulr8=%rdx
imulq  $38,%rax,%rdx

# qhasm:   carry? t80 += mulr8
# asm 1: add  <mulr8=int64#3,<t80=int64#9
# asm 2: add  <mulr8=%rdx,<t80=%r11
add  %rdx,%r11

# qhasm:   carry? t81 += mulzero + carry
# asm 1: adc <mulzero=int64#2,<t81=int64#10
# asm 2: adc <mulzero=%rsi,<t81=%r12
adc %rsi,%r12

# qhasm:   carry? t82 += mulzero + carry
# asm 1: adc <mulzero=int64#2,<t82=int64#11
# asm 2: adc <mulzero=%rsi,<t82=%r13
adc %rsi,%r13

# qhasm:   carry? t83 += mulzero + carry
# asm 1: adc <mulzero=int64#2,<t83=int64#12
# asm 2: adc <mulzero=%rsi,<t83=%r14
adc %rsi,%r14

# qhasm:   mulzero += mulzero + carry
# asm 1: adc <mulzero=int64#2,<mulzero=int64#2
# asm 2: adc <mulzero=%rsi,<mulzero=%rsi
adc %rsi,%rsi

# qhasm:   mulzero *= 38
# asm 1: imulq  $38,<mulzero=int64#2,>mulzero=int64#2
# asm 2: imulq  $38,<mulzero=%rsi,>mulzero=%rsi
imulq  $38,%rsi,%rsi

# qhasm:   t80 += mulzero
# asm 1: add  <mulzero=int64#2,<t80=int64#9
# asm 2: add  <mulzero=%rsi,<t80=%r11
add  %rsi,%r11

# qhasm: zq0 = t80
# asm 1: mov  <t80=int64#9,>zq0=int64#2
# asm 2: mov  <t80=%r11,>zq0=%rsi
mov  %r11,%rsi

# qhasm: zq1 = t81
# asm 1: mov  <t81=int64#10,>zq1=int64#3
# asm 2: mov  <t81=%r12,>zq1=%rdx
mov  %r12,%rdx

# qhasm: zq2 = t82
# asm 1: mov  <t82=int64#11,>zq2=int64#4
# asm 2: mov  <t82=%r13,>zq2=%rcx
mov  %r13,%rcx

# qhasm: zq3 = t83
# asm 1: mov  <t83=int64#12,>zq3=int64#5
# asm 2: mov  <t83=%r14,>zq3=%r8
mov  %r14,%r8

# qhasm:   carry? zq0 -= t90_stack 
# asm 1: subq <t90_stack=stack64#12,<zq0=int64#2
# asm 2: subq <t90_stack=88(%rsp),<zq0=%rsi
subq 88(%rsp),%rsi

# qhasm:   carry? zq1 -= t91_stack - carry
# asm 1: sbbq <t91_stack=stack64#13,<zq1=int64#3
# asm 2: sbbq <t91_stack=96(%rsp),<zq1=%rdx
sbbq 96(%rsp),%rdx

# qhasm:   carry? zq2 -= t92_stack - carry
# asm 1: sbbq <t92_stack=stack64#14,<zq2=int64#4
# asm 2: sbbq <t92_stack=104(%rsp),<zq2=%rcx
sbbq 104(%rsp),%rcx

# qhasm:   carry? zq3 -= t93_stack - carry
# asm 1: sbbq <t93_stack=stack64#15,<zq3=int64#5
# asm 2: sbbq <t93_stack=112(%rsp),<zq3=%r8
sbbq 112(%rsp),%r8

# qhasm:   subt0 = 0
# asm 1: mov  $0,>subt0=int64#6
# asm 2: mov  $0,>subt0=%r9
mov  $0,%r9

# qhasm:   subt1 = 38
# asm 1: mov  $38,>subt1=int64#7
# asm 2: mov  $38,>subt1=%rax
mov  $38,%rax

# qhasm:   subt1 = subt0 if !carry
# asm 1: cmovae <subt0=int64#6,<subt1=int64#7
# asm 2: cmovae <subt0=%r9,<subt1=%rax
cmovae %r9,%rax

# qhasm:   carry? zq0 -= subt1
# asm 1: sub  <subt1=int64#7,<zq0=int64#2
# asm 2: sub  <subt1=%rax,<zq0=%rsi
sub  %rax,%rsi

# qhasm:   carry? zq1 -= subt0 - carry
# asm 1: sbb  <subt0=int64#6,<zq1=int64#3
# asm 2: sbb  <subt0=%r9,<zq1=%rdx
sbb  %r9,%rdx

# qhasm:   carry? zq2 -= subt0 - carry
# asm 1: sbb  <subt0=int64#6,<zq2=int64#4
# asm 2: sbb  <subt0=%r9,<zq2=%rcx
sbb  %r9,%rcx

# qhasm:   carry? zq3 -= subt0 - carry
# asm 1: sbb  <subt0=int64#6,<zq3=int64#5
# asm 2: sbb  <subt0=%r9,<zq3=%r8
sbb  %r9,%r8

# qhasm:   subt0 = subt1 if carry
# asm 1: cmovc <subt1=int64#7,<subt0=int64#6
# asm 2: cmovc <subt1=%rax,<subt0=%r9
cmovc %rax,%r9

# qhasm:   zq0 -= subt0
# asm 1: sub  <subt0=int64#6,<zq0=int64#2
# asm 2: sub  <subt0=%r9,<zq0=%rsi
sub  %r9,%rsi

# qhasm:   carry? t80 += t90_stack 
# asm 1: addq <t90_stack=stack64#12,<t80=int64#9
# asm 2: addq <t90_stack=88(%rsp),<t80=%r11
addq 88(%rsp),%r11

# qhasm:   carry? t81 += t91_stack + carry
# asm 1: adcq <t91_stack=stack64#13,<t81=int64#10
# asm 2: adcq <t91_stack=96(%rsp),<t81=%r12
adcq 96(%rsp),%r12

# qhasm:   carry? t82 += t92_stack + carry
# asm 1: adcq <t92_stack=stack64#14,<t82=int64#11
# asm 2: adcq <t92_stack=104(%rsp),<t82=%r13
adcq 104(%rsp),%r13

# qhasm:   carry? t83 += t93_stack + carry
# asm 1: adcq <t93_stack=stack64#15,<t83=int64#12
# asm 2: adcq <t93_stack=112(%rsp),<t83=%r14
adcq 112(%rsp),%r14

# qhasm:   addt0 = 0
# asm 1: mov  $0,>addt0=int64#6
# asm 2: mov  $0,>addt0=%r9
mov  $0,%r9

# qhasm:   addt1 = 38
# asm 1: mov  $38,>addt1=int64#7
# asm 2: mov  $38,>addt1=%rax
mov  $38,%rax

# qhasm:   addt1 = addt0 if !carry
# asm 1: cmovae <addt0=int64#6,<addt1=int64#7
# asm 2: cmovae <addt0=%r9,<addt1=%rax
cmovae %r9,%rax

# qhasm:   carry? t80 += addt1
# asm 1: add  <addt1=int64#7,<t80=int64#9
# asm 2: add  <addt1=%rax,<t80=%r11
add  %rax,%r11

# qhasm:   carry? t81 += addt0 + carry
# asm 1: adc <addt0=int64#6,<t81=int64#10
# asm 2: adc <addt0=%r9,<t81=%r12
adc %r9,%r12

# qhasm:   carry? t82 += addt0 + carry
# asm 1: adc <addt0=int64#6,<t82=int64#11
# asm 2: adc <addt0=%r9,<t82=%r13
adc %r9,%r13

# qhasm:   carry? t83 += addt0 + carry
# asm 1: adc <addt0=int64#6,<t83=int64#12
# asm 2: adc <addt0=%r9,<t83=%r14
adc %r9,%r14

# qhasm:   addt0 = addt1 if carry
# asm 1: cmovc <addt1=int64#7,<addt0=int64#6
# asm 2: cmovc <addt1=%rax,<addt0=%r9
cmovc %rax,%r9

# qhasm:   t80 += addt0
# asm 1: add  <addt0=int64#6,<t80=int64#9
# asm 2: add  <addt0=%r9,<t80=%r11
add  %r9,%r11

# qhasm: *(uint64 *)(workp + 96) = t80
# asm 1: movq   <t80=int64#9,96(<workp=int64#1)
# asm 2: movq   <t80=%r11,96(<workp=%rdi)
movq   %r11,96(%rdi)

# qhasm: *(uint64 *)(workp + 104) = t81
# asm 1: movq   <t81=int64#10,104(<workp=int64#1)
# asm 2: movq   <t81=%r12,104(<workp=%rdi)
movq   %r12,104(%rdi)

# qhasm: *(uint64 *)(workp + 112) = t82
# asm 1: movq   <t82=int64#11,112(<workp=int64#1)
# asm 2: movq   <t82=%r13,112(<workp=%rdi)
movq   %r13,112(%rdi)

# qhasm: *(uint64 *)(workp + 120) = t83
# asm 1: movq   <t83=int64#12,120(<workp=int64#1)
# asm 2: movq   <t83=%r14,120(<workp=%rdi)
movq   %r14,120(%rdi)

# qhasm: *(uint64 *)(workp + 128) = zq0
# asm 1: movq   <zq0=int64#2,128(<workp=int64#1)
# asm 2: movq   <zq0=%rsi,128(<workp=%rdi)
movq   %rsi,128(%rdi)

# qhasm: *(uint64 *)(workp + 136) = zq1
# asm 1: movq   <zq1=int64#3,136(<workp=int64#1)
# asm 2: movq   <zq1=%rdx,136(<workp=%rdi)
movq   %rdx,136(%rdi)

# qhasm: *(uint64 *)(workp + 144) = zq2
# asm 1: movq   <zq2=int64#4,144(<workp=int64#1)
# asm 2: movq   <zq2=%rcx,144(<workp=%rdi)
movq   %rcx,144(%rdi)

# qhasm: *(uint64 *)(workp + 152) = zq3
# asm 1: movq   <zq3=int64#5,152(<workp=int64#1)
# asm 2: movq   <zq3=%r8,152(<workp=%rdi)
movq   %r8,152(%rdi)

# qhasm:   squarer7 = 0
# asm 1: mov  $0,>squarer7=int64#2
# asm 2: mov  $0,>squarer7=%rsi
mov  $0,%rsi

# qhasm:   squarerax = *(uint64 *)(workp + 104)
# asm 1: movq   104(<workp=int64#1),>squarerax=int64#7
# asm 2: movq   104(<workp=%rdi),>squarerax=%rax
movq   104(%rdi),%rax

# qhasm:   (uint128) squarerdx squarerax = squarerax * *(uint64 *)(workp + 96)
# asm 1: mulq  96(<workp=int64#1)
# asm 2: mulq  96(<workp=%rdi)
mulq  96(%rdi)

# qhasm:   xq1 = squarerax
# asm 1: mov  <squarerax=int64#7,>xq1=int64#4
# asm 2: mov  <squarerax=%rax,>xq1=%rcx
mov  %rax,%rcx

# qhasm:   xq2 = squarerdx
# asm 1: mov  <squarerdx=int64#3,>xq2=int64#5
# asm 2: mov  <squarerdx=%rdx,>xq2=%r8
mov  %rdx,%r8

# qhasm:   squarerax = *(uint64 *)(workp + 112)
# asm 1: movq   112(<workp=int64#1),>squarerax=int64#7
# asm 2: movq   112(<workp=%rdi),>squarerax=%rax
movq   112(%rdi),%rax

# qhasm:   (uint128) squarerdx squarerax = squarerax * *(uint64 *)(workp + 104)
# asm 1: mulq  104(<workp=int64#1)
# asm 2: mulq  104(<workp=%rdi)
mulq  104(%rdi)

# qhasm:   xq3 = squarerax 
# asm 1: mov  <squarerax=int64#7,>xq3=int64#6
# asm 2: mov  <squarerax=%rax,>xq3=%r9
mov  %rax,%r9

# qhasm:   squarer4 = squarerdx
# asm 1: mov  <squarerdx=int64#3,>squarer4=int64#8
# asm 2: mov  <squarerdx=%rdx,>squarer4=%r10
mov  %rdx,%r10

# qhasm:   squarerax = *(uint64 *)(workp + 120)
# asm 1: movq   120(<workp=int64#1),>squarerax=int64#7
# asm 2: movq   120(<workp=%rdi),>squarerax=%rax
movq   120(%rdi),%rax

# qhasm:   (uint128) squarerdx squarerax = squarerax * *(uint64 *)(workp + 112)
# asm 1: mulq  112(<workp=int64#1)
# asm 2: mulq  112(<workp=%rdi)
mulq  112(%rdi)

# qhasm:   squarer5 = squarerax
# asm 1: mov  <squarerax=int64#7,>squarer5=int64#9
# asm 2: mov  <squarerax=%rax,>squarer5=%r11
mov  %rax,%r11

# qhasm:   squarer6 = squarerdx
# asm 1: mov  <squarerdx=int64#3,>squarer6=int64#10
# asm 2: mov  <squarerdx=%rdx,>squarer6=%r12
mov  %rdx,%r12

# qhasm:   squarerax = *(uint64 *)(workp + 112)
# asm 1: movq   112(<workp=int64#1),>squarerax=int64#7
# asm 2: movq   112(<workp=%rdi),>squarerax=%rax
movq   112(%rdi),%rax

# qhasm:   (uint128) squarerdx squarerax = squarerax * *(uint64 *)(workp + 96)
# asm 1: mulq  96(<workp=int64#1)
# asm 2: mulq  96(<workp=%rdi)
mulq  96(%rdi)

# qhasm:   carry? xq2 += squarerax
# asm 1: add  <squarerax=int64#7,<xq2=int64#5
# asm 2: add  <squarerax=%rax,<xq2=%r8
add  %rax,%r8

# qhasm:   carry? xq3 += squarerdx + carry
# asm 1: adc <squarerdx=int64#3,<xq3=int64#6
# asm 2: adc <squarerdx=%rdx,<xq3=%r9
adc %rdx,%r9

# qhasm:   squarer4 += 0 + carry
# asm 1: adc $0,<squarer4=int64#8
# asm 2: adc $0,<squarer4=%r10
adc $0,%r10

# qhasm:   squarerax = *(uint64 *)(workp + 120)
# asm 1: movq   120(<workp=int64#1),>squarerax=int64#7
# asm 2: movq   120(<workp=%rdi),>squarerax=%rax
movq   120(%rdi),%rax

# qhasm:   (uint128) squarerdx squarerax = squarerax * *(uint64 *)(workp + 104)
# asm 1: mulq  104(<workp=int64#1)
# asm 2: mulq  104(<workp=%rdi)
mulq  104(%rdi)

# qhasm:   carry? squarer4 += squarerax
# asm 1: add  <squarerax=int64#7,<squarer4=int64#8
# asm 2: add  <squarerax=%rax,<squarer4=%r10
add  %rax,%r10

# qhasm:   carry? squarer5 += squarerdx + carry
# asm 1: adc <squarerdx=int64#3,<squarer5=int64#9
# asm 2: adc <squarerdx=%rdx,<squarer5=%r11
adc %rdx,%r11

# qhasm:   squarer6 += 0 + carry
# asm 1: adc $0,<squarer6=int64#10
# asm 2: adc $0,<squarer6=%r12
adc $0,%r12

# qhasm:   squarerax = *(uint64 *)(workp + 120)
# asm 1: movq   120(<workp=int64#1),>squarerax=int64#7
# asm 2: movq   120(<workp=%rdi),>squarerax=%rax
movq   120(%rdi),%rax

# qhasm:   (uint128) squarerdx squarerax = squarerax * *(uint64 *)(workp + 96)
# asm 1: mulq  96(<workp=int64#1)
# asm 2: mulq  96(<workp=%rdi)
mulq  96(%rdi)

# qhasm:   carry? xq3 += squarerax
# asm 1: add  <squarerax=int64#7,<xq3=int64#6
# asm 2: add  <squarerax=%rax,<xq3=%r9
add  %rax,%r9

# qhasm:   carry? squarer4 += squarerdx + carry
# asm 1: adc <squarerdx=int64#3,<squarer4=int64#8
# asm 2: adc <squarerdx=%rdx,<squarer4=%r10
adc %rdx,%r10

# qhasm:   carry? squarer5 += 0 + carry
# asm 1: adc $0,<squarer5=int64#9
# asm 2: adc $0,<squarer5=%r11
adc $0,%r11

# qhasm:   carry? squarer6 += 0 + carry
# asm 1: adc $0,<squarer6=int64#10
# asm 2: adc $0,<squarer6=%r12
adc $0,%r12

# qhasm:   squarer7 += 0 + carry
# asm 1: adc $0,<squarer7=int64#2
# asm 2: adc $0,<squarer7=%rsi
adc $0,%rsi

# qhasm:   carry? xq1 += xq1
# asm 1: add  <xq1=int64#4,<xq1=int64#4
# asm 2: add  <xq1=%rcx,<xq1=%rcx
add  %rcx,%rcx

# qhasm:   carry? xq2 += xq2 + carry
# asm 1: adc <xq2=int64#5,<xq2=int64#5
# asm 2: adc <xq2=%r8,<xq2=%r8
adc %r8,%r8

# qhasm:   carry? xq3 += xq3 + carry
# asm 1: adc <xq3=int64#6,<xq3=int64#6
# asm 2: adc <xq3=%r9,<xq3=%r9
adc %r9,%r9

# qhasm:   carry? squarer4 += squarer4 + carry
# asm 1: adc <squarer4=int64#8,<squarer4=int64#8
# asm 2: adc <squarer4=%r10,<squarer4=%r10
adc %r10,%r10

# qhasm:   carry? squarer5 += squarer5 + carry
# asm 1: adc <squarer5=int64#9,<squarer5=int64#9
# asm 2: adc <squarer5=%r11,<squarer5=%r11
adc %r11,%r11

# qhasm:   carry? squarer6 += squarer6 + carry
# asm 1: adc <squarer6=int64#10,<squarer6=int64#10
# asm 2: adc <squarer6=%r12,<squarer6=%r12
adc %r12,%r12

# qhasm:   squarer7 += squarer7 + carry
# asm 1: adc <squarer7=int64#2,<squarer7=int64#2
# asm 2: adc <squarer7=%rsi,<squarer7=%rsi
adc %rsi,%rsi

# qhasm:   squarerax = *(uint64 *)(workp + 96)
# asm 1: movq   96(<workp=int64#1),>squarerax=int64#7
# asm 2: movq   96(<workp=%rdi),>squarerax=%rax
movq   96(%rdi),%rax

# qhasm:   (uint128) squarerdx squarerax = squarerax * *(uint64 *)(workp + 96)
# asm 1: mulq  96(<workp=int64#1)
# asm 2: mulq  96(<workp=%rdi)
mulq  96(%rdi)

# qhasm:   xq0 = squarerax
# asm 1: mov  <squarerax=int64#7,>xq0=int64#11
# asm 2: mov  <squarerax=%rax,>xq0=%r13
mov  %rax,%r13

# qhasm:   squaret1 = squarerdx
# asm 1: mov  <squarerdx=int64#3,>squaret1=int64#12
# asm 2: mov  <squarerdx=%rdx,>squaret1=%r14
mov  %rdx,%r14

# qhasm:   squarerax = *(uint64 *)(workp + 104)
# asm 1: movq   104(<workp=int64#1),>squarerax=int64#7
# asm 2: movq   104(<workp=%rdi),>squarerax=%rax
movq   104(%rdi),%rax

# qhasm:   (uint128) squarerdx squarerax = squarerax * *(uint64 *)(workp + 104)
# asm 1: mulq  104(<workp=int64#1)
# asm 2: mulq  104(<workp=%rdi)
mulq  104(%rdi)

# qhasm:   squaret2 = squarerax
# asm 1: mov  <squarerax=int64#7,>squaret2=int64#13
# asm 2: mov  <squarerax=%rax,>squaret2=%r15
mov  %rax,%r15

# qhasm:   squaret3 = squarerdx
# asm 1: mov  <squarerdx=int64#3,>squaret3=int64#14
# asm 2: mov  <squarerdx=%rdx,>squaret3=%rbx
mov  %rdx,%rbx

# qhasm:   squarerax = *(uint64 *)(workp + 112)
# asm 1: movq   112(<workp=int64#1),>squarerax=int64#7
# asm 2: movq   112(<workp=%rdi),>squarerax=%rax
movq   112(%rdi),%rax

# qhasm:   (uint128) squarerdx squarerax = squarerax * *(uint64 *)(workp + 112)
# asm 1: mulq  112(<workp=int64#1)
# asm 2: mulq  112(<workp=%rdi)
mulq  112(%rdi)

# qhasm:   carry? xq1 += squaret1
# asm 1: add  <squaret1=int64#12,<xq1=int64#4
# asm 2: add  <squaret1=%r14,<xq1=%rcx
add  %r14,%rcx

# qhasm:   carry? xq2 += squaret2 + carry
# asm 1: adc <squaret2=int64#13,<xq2=int64#5
# asm 2: adc <squaret2=%r15,<xq2=%r8
adc %r15,%r8

# qhasm:   carry? xq3 += squaret3 + carry
# asm 1: adc <squaret3=int64#14,<xq3=int64#6
# asm 2: adc <squaret3=%rbx,<xq3=%r9
adc %rbx,%r9

# qhasm:   carry? squarer4 += squarerax + carry
# asm 1: adc <squarerax=int64#7,<squarer4=int64#8
# asm 2: adc <squarerax=%rax,<squarer4=%r10
adc %rax,%r10

# qhasm:   carry? squarer5 += squarerdx + carry
# asm 1: adc <squarerdx=int64#3,<squarer5=int64#9
# asm 2: adc <squarerdx=%rdx,<squarer5=%r11
adc %rdx,%r11

# qhasm:   carry? squarer6 += 0 + carry
# asm 1: adc $0,<squarer6=int64#10
# asm 2: adc $0,<squarer6=%r12
adc $0,%r12

# qhasm:   squarer7 += 0 + carry 
# asm 1: adc $0,<squarer7=int64#2
# asm 2: adc $0,<squarer7=%rsi
adc $0,%rsi

# qhasm:   squarerax = *(uint64 *)(workp + 120)
# asm 1: movq   120(<workp=int64#1),>squarerax=int64#7
# asm 2: movq   120(<workp=%rdi),>squarerax=%rax
movq   120(%rdi),%rax

# qhasm:   (uint128) squarerdx squarerax = squarerax * *(uint64 *)(workp + 120)
# asm 1: mulq  120(<workp=int64#1)
# asm 2: mulq  120(<workp=%rdi)
mulq  120(%rdi)

# qhasm:   carry? squarer6 += squarerax
# asm 1: add  <squarerax=int64#7,<squarer6=int64#10
# asm 2: add  <squarerax=%rax,<squarer6=%r12
add  %rax,%r12

# qhasm:   squarer7 += squarerdx + carry
# asm 1: adc <squarerdx=int64#3,<squarer7=int64#2
# asm 2: adc <squarerdx=%rdx,<squarer7=%rsi
adc %rdx,%rsi

# qhasm:   squarerax = squarer4
# asm 1: mov  <squarer4=int64#8,>squarerax=int64#7
# asm 2: mov  <squarer4=%r10,>squarerax=%rax
mov  %r10,%rax

# qhasm:   (uint128) squarerdx squarerax = squarerax * *(uint64 *)&crypto_scalarmult_curve25519_amd64_64_38
mulq  crypto_scalarmult_curve25519_amd64_64_38

# qhasm:   squarer4 = squarerax
# asm 1: mov  <squarerax=int64#7,>squarer4=int64#8
# asm 2: mov  <squarerax=%rax,>squarer4=%r10
mov  %rax,%r10

# qhasm:   squarerax = squarer5
# asm 1: mov  <squarer5=int64#9,>squarerax=int64#7
# asm 2: mov  <squarer5=%r11,>squarerax=%rax
mov  %r11,%rax

# qhasm:   squarer5 = squarerdx
# asm 1: mov  <squarerdx=int64#3,>squarer5=int64#9
# asm 2: mov  <squarerdx=%rdx,>squarer5=%r11
mov  %rdx,%r11

# qhasm:   (uint128) squarerdx squarerax = squarerax * *(uint64 *)&crypto_scalarmult_curve25519_amd64_64_38
mulq  crypto_scalarmult_curve25519_amd64_64_38

# qhasm:   carry? squarer5 += squarerax
# asm 1: add  <squarerax=int64#7,<squarer5=int64#9
# asm 2: add  <squarerax=%rax,<squarer5=%r11
add  %rax,%r11

# qhasm:   squarerax = squarer6
# asm 1: mov  <squarer6=int64#10,>squarerax=int64#7
# asm 2: mov  <squarer6=%r12,>squarerax=%rax
mov  %r12,%rax

# qhasm:   squarer6 = 0
# asm 1: mov  $0,>squarer6=int64#10
# asm 2: mov  $0,>squarer6=%r12
mov  $0,%r12

# qhasm:   squarer6 += squarerdx + carry
# asm 1: adc <squarerdx=int64#3,<squarer6=int64#10
# asm 2: adc <squarerdx=%rdx,<squarer6=%r12
adc %rdx,%r12

# qhasm:   (uint128) squarerdx squarerax = squarerax * *(uint64 *)&crypto_scalarmult_curve25519_amd64_64_38
mulq  crypto_scalarmult_curve25519_amd64_64_38

# qhasm:   carry? squarer6 += squarerax
# asm 1: add  <squarerax=int64#7,<squarer6=int64#10
# asm 2: add  <squarerax=%rax,<squarer6=%r12
add  %rax,%r12

# qhasm:   squarerax = squarer7
# asm 1: mov  <squarer7=int64#2,>squarerax=int64#7
# asm 2: mov  <squarer7=%rsi,>squarerax=%rax
mov  %rsi,%rax

# qhasm:   squarer7 = 0
# asm 1: mov  $0,>squarer7=int64#2
# asm 2: mov  $0,>squarer7=%rsi
mov  $0,%rsi

# qhasm:   squarer7 += squarerdx + carry
# asm 1: adc <squarerdx=int64#3,<squarer7=int64#2
# asm 2: adc <squarerdx=%rdx,<squarer7=%rsi
adc %rdx,%rsi

# qhasm:   (uint128) squarerdx squarerax = squarerax * *(uint64 *)&crypto_scalarmult_curve25519_amd64_64_38
mulq  crypto_scalarmult_curve25519_amd64_64_38

# qhasm:   carry? squarer7 += squarerax
# asm 1: add  <squarerax=int64#7,<squarer7=int64#2
# asm 2: add  <squarerax=%rax,<squarer7=%rsi
add  %rax,%rsi

# qhasm:   squarer8 = 0
# asm 1: mov  $0,>squarer8=int64#7
# asm 2: mov  $0,>squarer8=%rax
mov  $0,%rax

# qhasm:   squarer8 += squarerdx + carry
# asm 1: adc <squarerdx=int64#3,<squarer8=int64#7
# asm 2: adc <squarerdx=%rdx,<squarer8=%rax
adc %rdx,%rax

# qhasm:   carry? xq0 += squarer4
# asm 1: add  <squarer4=int64#8,<xq0=int64#11
# asm 2: add  <squarer4=%r10,<xq0=%r13
add  %r10,%r13

# qhasm:   carry? xq1 += squarer5 + carry
# asm 1: adc <squarer5=int64#9,<xq1=int64#4
# asm 2: adc <squarer5=%r11,<xq1=%rcx
adc %r11,%rcx

# qhasm:   carry? xq2 += squarer6 + carry
# asm 1: adc <squarer6=int64#10,<xq2=int64#5
# asm 2: adc <squarer6=%r12,<xq2=%r8
adc %r12,%r8

# qhasm:   carry? xq3 += squarer7 + carry
# asm 1: adc <squarer7=int64#2,<xq3=int64#6
# asm 2: adc <squarer7=%rsi,<xq3=%r9
adc %rsi,%r9

# qhasm:   squarezero = 0
# asm 1: mov  $0,>squarezero=int64#2
# asm 2: mov  $0,>squarezero=%rsi
mov  $0,%rsi

# qhasm:   squarer8 += squarezero + carry
# asm 1: adc <squarezero=int64#2,<squarer8=int64#7
# asm 2: adc <squarezero=%rsi,<squarer8=%rax
adc %rsi,%rax

# qhasm:   squarer8 *= 38
# asm 1: imulq  $38,<squarer8=int64#7,>squarer8=int64#3
# asm 2: imulq  $38,<squarer8=%rax,>squarer8=%rdx
imulq  $38,%rax,%rdx

# qhasm:   carry? xq0 += squarer8
# asm 1: add  <squarer8=int64#3,<xq0=int64#11
# asm 2: add  <squarer8=%rdx,<xq0=%r13
add  %rdx,%r13

# qhasm:   carry? xq1 += squarezero + carry
# asm 1: adc <squarezero=int64#2,<xq1=int64#4
# asm 2: adc <squarezero=%rsi,<xq1=%rcx
adc %rsi,%rcx

# qhasm:   carry? xq2 += squarezero + carry
# asm 1: adc <squarezero=int64#2,<xq2=int64#5
# asm 2: adc <squarezero=%rsi,<xq2=%r8
adc %rsi,%r8

# qhasm:   carry? xq3 += squarezero + carry
# asm 1: adc <squarezero=int64#2,<xq3=int64#6
# asm 2: adc <squarezero=%rsi,<xq3=%r9
adc %rsi,%r9

# qhasm:   squarezero += squarezero + carry
# asm 1: adc <squarezero=int64#2,<squarezero=int64#2
# asm 2: adc <squarezero=%rsi,<squarezero=%rsi
adc %rsi,%rsi

# qhasm:   squarezero *= 38
# asm 1: imulq  $38,<squarezero=int64#2,>squarezero=int64#2
# asm 2: imulq  $38,<squarezero=%rsi,>squarezero=%rsi
imulq  $38,%rsi,%rsi

# qhasm:   xq0 += squarezero
# asm 1: add  <squarezero=int64#2,<xq0=int64#11
# asm 2: add  <squarezero=%rsi,<xq0=%r13
add  %rsi,%r13

# qhasm: *(uint64 *)(workp + 96) = xq0
# asm 1: movq   <xq0=int64#11,96(<workp=int64#1)
# asm 2: movq   <xq0=%r13,96(<workp=%rdi)
movq   %r13,96(%rdi)

# qhasm: *(uint64 *)(workp + 104) = xq1
# asm 1: movq   <xq1=int64#4,104(<workp=int64#1)
# asm 2: movq   <xq1=%rcx,104(<workp=%rdi)
movq   %rcx,104(%rdi)

# qhasm: *(uint64 *)(workp + 112) = xq2
# asm 1: movq   <xq2=int64#5,112(<workp=int64#1)
# asm 2: movq   <xq2=%r8,112(<workp=%rdi)
movq   %r8,112(%rdi)

# qhasm: *(uint64 *)(workp + 120) = xq3
# asm 1: movq   <xq3=int64#6,120(<workp=int64#1)
# asm 2: movq   <xq3=%r9,120(<workp=%rdi)
movq   %r9,120(%rdi)

# qhasm:   squarer7 = 0
# asm 1: mov  $0,>squarer7=int64#2
# asm 2: mov  $0,>squarer7=%rsi
mov  $0,%rsi

# qhasm:   squarerax = *(uint64 *)(workp + 136)
# asm 1: movq   136(<workp=int64#1),>squarerax=int64#7
# asm 2: movq   136(<workp=%rdi),>squarerax=%rax
movq   136(%rdi),%rax

# qhasm:   (uint128) squarerdx squarerax = squarerax * *(uint64 *)(workp + 128)
# asm 1: mulq  128(<workp=int64#1)
# asm 2: mulq  128(<workp=%rdi)
mulq  128(%rdi)

# qhasm:   zq1 = squarerax
# asm 1: mov  <squarerax=int64#7,>zq1=int64#4
# asm 2: mov  <squarerax=%rax,>zq1=%rcx
mov  %rax,%rcx

# qhasm:   zq2 = squarerdx
# asm 1: mov  <squarerdx=int64#3,>zq2=int64#5
# asm 2: mov  <squarerdx=%rdx,>zq2=%r8
mov  %rdx,%r8

# qhasm:   squarerax = *(uint64 *)(workp + 144)
# asm 1: movq   144(<workp=int64#1),>squarerax=int64#7
# asm 2: movq   144(<workp=%rdi),>squarerax=%rax
movq   144(%rdi),%rax

# qhasm:   (uint128) squarerdx squarerax = squarerax * *(uint64 *)(workp + 136)
# asm 1: mulq  136(<workp=int64#1)
# asm 2: mulq  136(<workp=%rdi)
mulq  136(%rdi)

# qhasm:   zq3 = squarerax 
# asm 1: mov  <squarerax=int64#7,>zq3=int64#6
# asm 2: mov  <squarerax=%rax,>zq3=%r9
mov  %rax,%r9

# qhasm:   squarer4 = squarerdx
# asm 1: mov  <squarerdx=int64#3,>squarer4=int64#8
# asm 2: mov  <squarerdx=%rdx,>squarer4=%r10
mov  %rdx,%r10

# qhasm:   squarerax = *(uint64 *)(workp + 152)
# asm 1: movq   152(<workp=int64#1),>squarerax=int64#7
# asm 2: movq   152(<workp=%rdi),>squarerax=%rax
movq   152(%rdi),%rax

# qhasm:   (uint128) squarerdx squarerax = squarerax * *(uint64 *)(workp + 144)
# asm 1: mulq  144(<workp=int64#1)
# asm 2: mulq  144(<workp=%rdi)
mulq  144(%rdi)

# qhasm:   squarer5 = squarerax
# asm 1: mov  <squarerax=int64#7,>squarer5=int64#9
# asm 2: mov  <squarerax=%rax,>squarer5=%r11
mov  %rax,%r11

# qhasm:   squarer6 = squarerdx
# asm 1: mov  <squarerdx=int64#3,>squarer6=int64#10
# asm 2: mov  <squarerdx=%rdx,>squarer6=%r12
mov  %rdx,%r12

# qhasm:   squarerax = *(uint64 *)(workp + 144)
# asm 1: movq   144(<workp=int64#1),>squarerax=int64#7
# asm 2: movq   144(<workp=%rdi),>squarerax=%rax
movq   144(%rdi),%rax

# qhasm:   (uint128) squarerdx squarerax = squarerax * *(uint64 *)(workp + 128)
# asm 1: mulq  128(<workp=int64#1)
# asm 2: mulq  128(<workp=%rdi)
mulq  128(%rdi)

# qhasm:   carry? zq2 += squarerax
# asm 1: add  <squarerax=int64#7,<zq2=int64#5
# asm 2: add  <squarerax=%rax,<zq2=%r8
add  %rax,%r8

# qhasm:   carry? zq3 += squarerdx + carry
# asm 1: adc <squarerdx=int64#3,<zq3=int64#6
# asm 2: adc <squarerdx=%rdx,<zq3=%r9
adc %rdx,%r9

# qhasm:   squarer4 += 0 + carry
# asm 1: adc $0,<squarer4=int64#8
# asm 2: adc $0,<squarer4=%r10
adc $0,%r10

# qhasm:   squarerax = *(uint64 *)(workp + 152)
# asm 1: movq   152(<workp=int64#1),>squarerax=int64#7
# asm 2: movq   152(<workp=%rdi),>squarerax=%rax
movq   152(%rdi),%rax

# qhasm:   (uint128) squarerdx squarerax = squarerax * *(uint64 *)(workp + 136)
# asm 1: mulq  136(<workp=int64#1)
# asm 2: mulq  136(<workp=%rdi)
mulq  136(%rdi)

# qhasm:   carry? squarer4 += squarerax
# asm 1: add  <squarerax=int64#7,<squarer4=int64#8
# asm 2: add  <squarerax=%rax,<squarer4=%r10
add  %rax,%r10

# qhasm:   carry? squarer5 += squarerdx + carry
# asm 1: adc <squarerdx=int64#3,<squarer5=int64#9
# asm 2: adc <squarerdx=%rdx,<squarer5=%r11
adc %rdx,%r11

# qhasm:   squarer6 += 0 + carry
# asm 1: adc $0,<squarer6=int64#10
# asm 2: adc $0,<squarer6=%r12
adc $0,%r12

# qhasm:   squarerax = *(uint64 *)(workp + 152)
# asm 1: movq   152(<workp=int64#1),>squarerax=int64#7
# asm 2: movq   152(<workp=%rdi),>squarerax=%rax
movq   152(%rdi),%rax

# qhasm:   (uint128) squarerdx squarerax = squarerax * *(uint64 *)(workp + 128)
# asm 1: mulq  128(<workp=int64#1)
# asm 2: mulq  128(<workp=%rdi)
mulq  128(%rdi)

# qhasm:   carry? zq3 += squarerax
# asm 1: add  <squarerax=int64#7,<zq3=int64#6
# asm 2: add  <squarerax=%rax,<zq3=%r9
add  %rax,%r9

# qhasm:   carry? squarer4 += squarerdx + carry
# asm 1: adc <squarerdx=int64#3,<squarer4=int64#8
# asm 2: adc <squarerdx=%rdx,<squarer4=%r10
adc %rdx,%r10

# qhasm:   carry? squarer5 += 0 + carry
# asm 1: adc $0,<squarer5=int64#9
# asm 2: adc $0,<squarer5=%r11
adc $0,%r11

# qhasm:   carry? squarer6 += 0 + carry
# asm 1: adc $0,<squarer6=int64#10
# asm 2: adc $0,<squarer6=%r12
adc $0,%r12

# qhasm:   squarer7 += 0 + carry
# asm 1: adc $0,<squarer7=int64#2
# asm 2: adc $0,<squarer7=%rsi
adc $0,%rsi

# qhasm:   carry? zq1 += zq1
# asm 1: add  <zq1=int64#4,<zq1=int64#4
# asm 2: add  <zq1=%rcx,<zq1=%rcx
add  %rcx,%rcx

# qhasm:   carry? zq2 += zq2 + carry
# asm 1: adc <zq2=int64#5,<zq2=int64#5
# asm 2: adc <zq2=%r8,<zq2=%r8
adc %r8,%r8

# qhasm:   carry? zq3 += zq3 + carry
# asm 1: adc <zq3=int64#6,<zq3=int64#6
# asm 2: adc <zq3=%r9,<zq3=%r9
adc %r9,%r9

# qhasm:   carry? squarer4 += squarer4 + carry
# asm 1: adc <squarer4=int64#8,<squarer4=int64#8
# asm 2: adc <squarer4=%r10,<squarer4=%r10
adc %r10,%r10

# qhasm:   carry? squarer5 += squarer5 + carry
# asm 1: adc <squarer5=int64#9,<squarer5=int64#9
# asm 2: adc <squarer5=%r11,<squarer5=%r11
adc %r11,%r11

# qhasm:   carry? squarer6 += squarer6 + carry
# asm 1: adc <squarer6=int64#10,<squarer6=int64#10
# asm 2: adc <squarer6=%r12,<squarer6=%r12
adc %r12,%r12

# qhasm:   squarer7 += squarer7 + carry
# asm 1: adc <squarer7=int64#2,<squarer7=int64#2
# asm 2: adc <squarer7=%rsi,<squarer7=%rsi
adc %rsi,%rsi

# qhasm:   squarerax = *(uint64 *)(workp + 128)
# asm 1: movq   128(<workp=int64#1),>squarerax=int64#7
# asm 2: movq   128(<workp=%rdi),>squarerax=%rax
movq   128(%rdi),%rax

# qhasm:   (uint128) squarerdx squarerax = squarerax * *(uint64 *)(workp + 128)
# asm 1: mulq  128(<workp=int64#1)
# asm 2: mulq  128(<workp=%rdi)
mulq  128(%rdi)

# qhasm:   zq0 = squarerax
# asm 1: mov  <squarerax=int64#7,>zq0=int64#11
# asm 2: mov  <squarerax=%rax,>zq0=%r13
mov  %rax,%r13

# qhasm:   squaret1 = squarerdx
# asm 1: mov  <squarerdx=int64#3,>squaret1=int64#12
# asm 2: mov  <squarerdx=%rdx,>squaret1=%r14
mov  %rdx,%r14

# qhasm:   squarerax = *(uint64 *)(workp + 136)
# asm 1: movq   136(<workp=int64#1),>squarerax=int64#7
# asm 2: movq   136(<workp=%rdi),>squarerax=%rax
movq   136(%rdi),%rax

# qhasm:   (uint128) squarerdx squarerax = squarerax * *(uint64 *)(workp + 136)
# asm 1: mulq  136(<workp=int64#1)
# asm 2: mulq  136(<workp=%rdi)
mulq  136(%rdi)

# qhasm:   squaret2 = squarerax
# asm 1: mov  <squarerax=int64#7,>squaret2=int64#13
# asm 2: mov  <squarerax=%rax,>squaret2=%r15
mov  %rax,%r15

# qhasm:   squaret3 = squarerdx
# asm 1: mov  <squarerdx=int64#3,>squaret3=int64#14
# asm 2: mov  <squarerdx=%rdx,>squaret3=%rbx
mov  %rdx,%rbx

# qhasm:   squarerax = *(uint64 *)(workp + 144)
# asm 1: movq   144(<workp=int64#1),>squarerax=int64#7
# asm 2: movq   144(<workp=%rdi),>squarerax=%rax
movq   144(%rdi),%rax

# qhasm:   (uint128) squarerdx squarerax = squarerax * *(uint64 *)(workp + 144)
# asm 1: mulq  144(<workp=int64#1)
# asm 2: mulq  144(<workp=%rdi)
mulq  144(%rdi)

# qhasm:   carry? zq1 += squaret1
# asm 1: add  <squaret1=int64#12,<zq1=int64#4
# asm 2: add  <squaret1=%r14,<zq1=%rcx
add  %r14,%rcx

# qhasm:   carry? zq2 += squaret2 + carry
# asm 1: adc <squaret2=int64#13,<zq2=int64#5
# asm 2: adc <squaret2=%r15,<zq2=%r8
adc %r15,%r8

# qhasm:   carry? zq3 += squaret3 + carry
# asm 1: adc <squaret3=int64#14,<zq3=int64#6
# asm 2: adc <squaret3=%rbx,<zq3=%r9
adc %rbx,%r9

# qhasm:   carry? squarer4 += squarerax + carry
# asm 1: adc <squarerax=int64#7,<squarer4=int64#8
# asm 2: adc <squarerax=%rax,<squarer4=%r10
adc %rax,%r10

# qhasm:   carry? squarer5 += squarerdx + carry
# asm 1: adc <squarerdx=int64#3,<squarer5=int64#9
# asm 2: adc <squarerdx=%rdx,<squarer5=%r11
adc %rdx,%r11

# qhasm:   carry? squarer6 += 0 + carry
# asm 1: adc $0,<squarer6=int64#10
# asm 2: adc $0,<squarer6=%r12
adc $0,%r12

# qhasm:   squarer7 += 0 + carry 
# asm 1: adc $0,<squarer7=int64#2
# asm 2: adc $0,<squarer7=%rsi
adc $0,%rsi

# qhasm:   squarerax = *(uint64 *)(workp + 152)
# asm 1: movq   152(<workp=int64#1),>squarerax=int64#7
# asm 2: movq   152(<workp=%rdi),>squarerax=%rax
movq   152(%rdi),%rax

# qhasm:   (uint128) squarerdx squarerax = squarerax * *(uint64 *)(workp + 152)
# asm 1: mulq  152(<workp=int64#1)
# asm 2: mulq  152(<workp=%rdi)
mulq  152(%rdi)

# qhasm:   carry? squarer6 += squarerax
# asm 1: add  <squarerax=int64#7,<squarer6=int64#10
# asm 2: add  <squarerax=%rax,<squarer6=%r12
add  %rax,%r12

# qhasm:   squarer7 += squarerdx + carry
# asm 1: adc <squarerdx=int64#3,<squarer7=int64#2
# asm 2: adc <squarerdx=%rdx,<squarer7=%rsi
adc %rdx,%rsi

# qhasm:   squarerax = squarer4
# asm 1: mov  <squarer4=int64#8,>squarerax=int64#7
# asm 2: mov  <squarer4=%r10,>squarerax=%rax
mov  %r10,%rax

# qhasm:   (uint128) squarerdx squarerax = squarerax * *(uint64 *)&crypto_scalarmult_curve25519_amd64_64_38
mulq  crypto_scalarmult_curve25519_amd64_64_38

# qhasm:   squarer4 = squarerax
# asm 1: mov  <squarerax=int64#7,>squarer4=int64#8
# asm 2: mov  <squarerax=%rax,>squarer4=%r10
mov  %rax,%r10

# qhasm:   squarerax = squarer5
# asm 1: mov  <squarer5=int64#9,>squarerax=int64#7
# asm 2: mov  <squarer5=%r11,>squarerax=%rax
mov  %r11,%rax

# qhasm:   squarer5 = squarerdx
# asm 1: mov  <squarerdx=int64#3,>squarer5=int64#9
# asm 2: mov  <squarerdx=%rdx,>squarer5=%r11
mov  %rdx,%r11

# qhasm:   (uint128) squarerdx squarerax = squarerax * *(uint64 *)&crypto_scalarmult_curve25519_amd64_64_38
mulq  crypto_scalarmult_curve25519_amd64_64_38

# qhasm:   carry? squarer5 += squarerax
# asm 1: add  <squarerax=int64#7,<squarer5=int64#9
# asm 2: add  <squarerax=%rax,<squarer5=%r11
add  %rax,%r11

# qhasm:   squarerax = squarer6
# asm 1: mov  <squarer6=int64#10,>squarerax=int64#7
# asm 2: mov  <squarer6=%r12,>squarerax=%rax
mov  %r12,%rax

# qhasm:   squarer6 = 0
# asm 1: mov  $0,>squarer6=int64#10
# asm 2: mov  $0,>squarer6=%r12
mov  $0,%r12

# qhasm:   squarer6 += squarerdx + carry
# asm 1: adc <squarerdx=int64#3,<squarer6=int64#10
# asm 2: adc <squarerdx=%rdx,<squarer6=%r12
adc %rdx,%r12

# qhasm:   (uint128) squarerdx squarerax = squarerax * *(uint64 *)&crypto_scalarmult_curve25519_amd64_64_38
mulq  crypto_scalarmult_curve25519_amd64_64_38

# qhasm:   carry? squarer6 += squarerax
# asm 1: add  <squarerax=int64#7,<squarer6=int64#10
# asm 2: add  <squarerax=%rax,<squarer6=%r12
add  %rax,%r12

# qhasm:   squarerax = squarer7
# asm 1: mov  <squarer7=int64#2,>squarerax=int64#7
# asm 2: mov  <squarer7=%rsi,>squarerax=%rax
mov  %rsi,%rax

# qhasm:   squarer7 = 0
# asm 1: mov  $0,>squarer7=int64#2
# asm 2: mov  $0,>squarer7=%rsi
mov  $0,%rsi

# qhasm:   squarer7 += squarerdx + carry
# asm 1: adc <squarerdx=int64#3,<squarer7=int64#2
# asm 2: adc <squarerdx=%rdx,<squarer7=%rsi
adc %rdx,%rsi

# qhasm:   (uint128) squarerdx squarerax = squarerax * *(uint64 *)&crypto_scalarmult_curve25519_amd64_64_38
mulq  crypto_scalarmult_curve25519_amd64_64_38

# qhasm:   carry? squarer7 += squarerax
# asm 1: add  <squarerax=int64#7,<squarer7=int64#2
# asm 2: add  <squarerax=%rax,<squarer7=%rsi
add  %rax,%rsi

# qhasm:   squarer8 = 0
# asm 1: mov  $0,>squarer8=int64#7
# asm 2: mov  $0,>squarer8=%rax
mov  $0,%rax

# qhasm:   squarer8 += squarerdx + carry
# asm 1: adc <squarerdx=int64#3,<squarer8=int64#7
# asm 2: adc <squarerdx=%rdx,<squarer8=%rax
adc %rdx,%rax

# qhasm:   carry? zq0 += squarer4
# asm 1: add  <squarer4=int64#8,<zq0=int64#11
# asm 2: add  <squarer4=%r10,<zq0=%r13
add  %r10,%r13

# qhasm:   carry? zq1 += squarer5 + carry
# asm 1: adc <squarer5=int64#9,<zq1=int64#4
# asm 2: adc <squarer5=%r11,<zq1=%rcx
adc %r11,%rcx

# qhasm:   carry? zq2 += squarer6 + carry
# asm 1: adc <squarer6=int64#10,<zq2=int64#5
# asm 2: adc <squarer6=%r12,<zq2=%r8
adc %r12,%r8

# qhasm:   carry? zq3 += squarer7 + carry
# asm 1: adc <squarer7=int64#2,<zq3=int64#6
# asm 2: adc <squarer7=%rsi,<zq3=%r9
adc %rsi,%r9

# qhasm:   squarezero = 0
# asm 1: mov  $0,>squarezero=int64#2
# asm 2: mov  $0,>squarezero=%rsi
mov  $0,%rsi

# qhasm:   squarer8 += squarezero + carry
# asm 1: adc <squarezero=int64#2,<squarer8=int64#7
# asm 2: adc <squarezero=%rsi,<squarer8=%rax
adc %rsi,%rax

# qhasm:   squarer8 *= 38
# asm 1: imulq  $38,<squarer8=int64#7,>squarer8=int64#3
# asm 2: imulq  $38,<squarer8=%rax,>squarer8=%rdx
imulq  $38,%rax,%rdx

# qhasm:   carry? zq0 += squarer8
# asm 1: add  <squarer8=int64#3,<zq0=int64#11
# asm 2: add  <squarer8=%rdx,<zq0=%r13
add  %rdx,%r13

# qhasm:   carry? zq1 += squarezero + carry
# asm 1: adc <squarezero=int64#2,<zq1=int64#4
# asm 2: adc <squarezero=%rsi,<zq1=%rcx
adc %rsi,%rcx

# qhasm:   carry? zq2 += squarezero + carry
# asm 1: adc <squarezero=int64#2,<zq2=int64#5
# asm 2: adc <squarezero=%rsi,<zq2=%r8
adc %rsi,%r8

# qhasm:   carry? zq3 += squarezero + carry
# asm 1: adc <squarezero=int64#2,<zq3=int64#6
# asm 2: adc <squarezero=%rsi,<zq3=%r9
adc %rsi,%r9

# qhasm:   squarezero += squarezero + carry
# asm 1: adc <squarezero=int64#2,<squarezero=int64#2
# asm 2: adc <squarezero=%rsi,<squarezero=%rsi
adc %rsi,%rsi

# qhasm:   squarezero *= 38
# asm 1: imulq  $38,<squarezero=int64#2,>squarezero=int64#2
# asm 2: imulq  $38,<squarezero=%rsi,>squarezero=%rsi
imulq  $38,%rsi,%rsi

# qhasm:   zq0 += squarezero
# asm 1: add  <squarezero=int64#2,<zq0=int64#11
# asm 2: add  <squarezero=%rsi,<zq0=%r13
add  %rsi,%r13

# qhasm: *(uint64 *)(workp + 128) = zq0
# asm 1: movq   <zq0=int64#11,128(<workp=int64#1)
# asm 2: movq   <zq0=%r13,128(<workp=%rdi)
movq   %r13,128(%rdi)

# qhasm: *(uint64 *)(workp + 136) = zq1
# asm 1: movq   <zq1=int64#4,136(<workp=int64#1)
# asm 2: movq   <zq1=%rcx,136(<workp=%rdi)
movq   %rcx,136(%rdi)

# qhasm: *(uint64 *)(workp + 144) = zq2
# asm 1: movq   <zq2=int64#5,144(<workp=int64#1)
# asm 2: movq   <zq2=%r8,144(<workp=%rdi)
movq   %r8,144(%rdi)

# qhasm: *(uint64 *)(workp + 152) = zq3
# asm 1: movq   <zq3=int64#6,152(<workp=int64#1)
# asm 2: movq   <zq3=%r9,152(<workp=%rdi)
movq   %r9,152(%rdi)

# qhasm:   mulr4 = 0
# asm 1: mov  $0,>mulr4=int64#2
# asm 2: mov  $0,>mulr4=%rsi
mov  $0,%rsi

# qhasm:   mulr5 = 0
# asm 1: mov  $0,>mulr5=int64#4
# asm 2: mov  $0,>mulr5=%rcx
mov  $0,%rcx

# qhasm:   mulr6 = 0
# asm 1: mov  $0,>mulr6=int64#5
# asm 2: mov  $0,>mulr6=%r8
mov  $0,%r8

# qhasm:   mulr7 = 0
# asm 1: mov  $0,>mulr7=int64#6
# asm 2: mov  $0,>mulr7=%r9
mov  $0,%r9

# qhasm:   mulx0 = *(uint64 *)(workp + 128)
# asm 1: movq   128(<workp=int64#1),>mulx0=int64#8
# asm 2: movq   128(<workp=%rdi),>mulx0=%r10
movq   128(%rdi),%r10

# qhasm:   mulrax = *(uint64 *)(workp + 0)
# asm 1: movq   0(<workp=int64#1),>mulrax=int64#7
# asm 2: movq   0(<workp=%rdi),>mulrax=%rax
movq   0(%rdi),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * mulx0
# asm 1: mul  <mulx0=int64#8
# asm 2: mul  <mulx0=%r10
mul  %r10

# qhasm:   zq0 = mulrax
# asm 1: mov  <mulrax=int64#7,>zq0=int64#9
# asm 2: mov  <mulrax=%rax,>zq0=%r11
mov  %rax,%r11

# qhasm:   zq1 = mulrdx
# asm 1: mov  <mulrdx=int64#3,>zq1=int64#10
# asm 2: mov  <mulrdx=%rdx,>zq1=%r12
mov  %rdx,%r12

# qhasm:   mulrax = *(uint64 *)(workp + 8)
# asm 1: movq   8(<workp=int64#1),>mulrax=int64#7
# asm 2: movq   8(<workp=%rdi),>mulrax=%rax
movq   8(%rdi),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * mulx0
# asm 1: mul  <mulx0=int64#8
# asm 2: mul  <mulx0=%r10
mul  %r10

# qhasm:   carry? zq1 += mulrax
# asm 1: add  <mulrax=int64#7,<zq1=int64#10
# asm 2: add  <mulrax=%rax,<zq1=%r12
add  %rax,%r12

# qhasm:   zq2 = 0
# asm 1: mov  $0,>zq2=int64#11
# asm 2: mov  $0,>zq2=%r13
mov  $0,%r13

# qhasm:   zq2 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<zq2=int64#11
# asm 2: adc <mulrdx=%rdx,<zq2=%r13
adc %rdx,%r13

# qhasm:   mulrax = *(uint64 *)(workp + 16)
# asm 1: movq   16(<workp=int64#1),>mulrax=int64#7
# asm 2: movq   16(<workp=%rdi),>mulrax=%rax
movq   16(%rdi),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * mulx0
# asm 1: mul  <mulx0=int64#8
# asm 2: mul  <mulx0=%r10
mul  %r10

# qhasm:   carry? zq2 += mulrax
# asm 1: add  <mulrax=int64#7,<zq2=int64#11
# asm 2: add  <mulrax=%rax,<zq2=%r13
add  %rax,%r13

# qhasm:   zq3 = 0
# asm 1: mov  $0,>zq3=int64#12
# asm 2: mov  $0,>zq3=%r14
mov  $0,%r14

# qhasm:   zq3 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<zq3=int64#12
# asm 2: adc <mulrdx=%rdx,<zq3=%r14
adc %rdx,%r14

# qhasm:   mulrax = *(uint64 *)(workp + 24)
# asm 1: movq   24(<workp=int64#1),>mulrax=int64#7
# asm 2: movq   24(<workp=%rdi),>mulrax=%rax
movq   24(%rdi),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * mulx0
# asm 1: mul  <mulx0=int64#8
# asm 2: mul  <mulx0=%r10
mul  %r10

# qhasm:   carry? zq3 += mulrax
# asm 1: add  <mulrax=int64#7,<zq3=int64#12
# asm 2: add  <mulrax=%rax,<zq3=%r14
add  %rax,%r14

# qhasm:   mulr4 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr4=int64#2
# asm 2: adc <mulrdx=%rdx,<mulr4=%rsi
adc %rdx,%rsi

# qhasm:   mulx1 = *(uint64 *)(workp + 136)
# asm 1: movq   136(<workp=int64#1),>mulx1=int64#8
# asm 2: movq   136(<workp=%rdi),>mulx1=%r10
movq   136(%rdi),%r10

# qhasm:   mulrax = *(uint64 *)(workp + 0)
# asm 1: movq   0(<workp=int64#1),>mulrax=int64#7
# asm 2: movq   0(<workp=%rdi),>mulrax=%rax
movq   0(%rdi),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * mulx1
# asm 1: mul  <mulx1=int64#8
# asm 2: mul  <mulx1=%r10
mul  %r10

# qhasm:   carry? zq1 += mulrax
# asm 1: add  <mulrax=int64#7,<zq1=int64#10
# asm 2: add  <mulrax=%rax,<zq1=%r12
add  %rax,%r12

# qhasm:   mulc = 0
# asm 1: mov  $0,>mulc=int64#13
# asm 2: mov  $0,>mulc=%r15
mov  $0,%r15

# qhasm:   mulc += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulc=int64#13
# asm 2: adc <mulrdx=%rdx,<mulc=%r15
adc %rdx,%r15

# qhasm:   mulrax = *(uint64 *)(workp + 8)
# asm 1: movq   8(<workp=int64#1),>mulrax=int64#7
# asm 2: movq   8(<workp=%rdi),>mulrax=%rax
movq   8(%rdi),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * mulx1
# asm 1: mul  <mulx1=int64#8
# asm 2: mul  <mulx1=%r10
mul  %r10

# qhasm:   carry? zq2 += mulrax
# asm 1: add  <mulrax=int64#7,<zq2=int64#11
# asm 2: add  <mulrax=%rax,<zq2=%r13
add  %rax,%r13

# qhasm:   mulrdx += 0 + carry
# asm 1: adc $0,<mulrdx=int64#3
# asm 2: adc $0,<mulrdx=%rdx
adc $0,%rdx

# qhasm:   carry? zq2 += mulc
# asm 1: add  <mulc=int64#13,<zq2=int64#11
# asm 2: add  <mulc=%r15,<zq2=%r13
add  %r15,%r13

# qhasm:   mulc = 0
# asm 1: mov  $0,>mulc=int64#13
# asm 2: mov  $0,>mulc=%r15
mov  $0,%r15

# qhasm:   mulc += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulc=int64#13
# asm 2: adc <mulrdx=%rdx,<mulc=%r15
adc %rdx,%r15

# qhasm:   mulrax = *(uint64 *)(workp + 16)
# asm 1: movq   16(<workp=int64#1),>mulrax=int64#7
# asm 2: movq   16(<workp=%rdi),>mulrax=%rax
movq   16(%rdi),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * mulx1
# asm 1: mul  <mulx1=int64#8
# asm 2: mul  <mulx1=%r10
mul  %r10

# qhasm:   carry? zq3 += mulrax 
# asm 1: add  <mulrax=int64#7,<zq3=int64#12
# asm 2: add  <mulrax=%rax,<zq3=%r14
add  %rax,%r14

# qhasm:   mulrdx += 0 + carry
# asm 1: adc $0,<mulrdx=int64#3
# asm 2: adc $0,<mulrdx=%rdx
adc $0,%rdx

# qhasm:   carry? zq3 += mulc
# asm 1: add  <mulc=int64#13,<zq3=int64#12
# asm 2: add  <mulc=%r15,<zq3=%r14
add  %r15,%r14

# qhasm:   mulc = 0
# asm 1: mov  $0,>mulc=int64#13
# asm 2: mov  $0,>mulc=%r15
mov  $0,%r15

# qhasm:   mulc += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulc=int64#13
# asm 2: adc <mulrdx=%rdx,<mulc=%r15
adc %rdx,%r15

# qhasm:   mulrax = *(uint64 *)(workp + 24)
# asm 1: movq   24(<workp=int64#1),>mulrax=int64#7
# asm 2: movq   24(<workp=%rdi),>mulrax=%rax
movq   24(%rdi),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * mulx1
# asm 1: mul  <mulx1=int64#8
# asm 2: mul  <mulx1=%r10
mul  %r10

# qhasm:   carry? mulr4 += mulrax
# asm 1: add  <mulrax=int64#7,<mulr4=int64#2
# asm 2: add  <mulrax=%rax,<mulr4=%rsi
add  %rax,%rsi

# qhasm:   mulrdx += 0 + carry
# asm 1: adc $0,<mulrdx=int64#3
# asm 2: adc $0,<mulrdx=%rdx
adc $0,%rdx

# qhasm:   carry? mulr4 += mulc
# asm 1: add  <mulc=int64#13,<mulr4=int64#2
# asm 2: add  <mulc=%r15,<mulr4=%rsi
add  %r15,%rsi

# qhasm:   mulr5 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr5=int64#4
# asm 2: adc <mulrdx=%rdx,<mulr5=%rcx
adc %rdx,%rcx

# qhasm:   mulx2 = *(uint64 *)(workp + 144)
# asm 1: movq   144(<workp=int64#1),>mulx2=int64#8
# asm 2: movq   144(<workp=%rdi),>mulx2=%r10
movq   144(%rdi),%r10

# qhasm:   mulrax = *(uint64 *)(workp + 0)
# asm 1: movq   0(<workp=int64#1),>mulrax=int64#7
# asm 2: movq   0(<workp=%rdi),>mulrax=%rax
movq   0(%rdi),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * mulx2
# asm 1: mul  <mulx2=int64#8
# asm 2: mul  <mulx2=%r10
mul  %r10

# qhasm:   carry? zq2 += mulrax
# asm 1: add  <mulrax=int64#7,<zq2=int64#11
# asm 2: add  <mulrax=%rax,<zq2=%r13
add  %rax,%r13

# qhasm:   mulc = 0
# asm 1: mov  $0,>mulc=int64#13
# asm 2: mov  $0,>mulc=%r15
mov  $0,%r15

# qhasm:   mulc += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulc=int64#13
# asm 2: adc <mulrdx=%rdx,<mulc=%r15
adc %rdx,%r15

# qhasm:   mulrax = *(uint64 *)(workp + 8)
# asm 1: movq   8(<workp=int64#1),>mulrax=int64#7
# asm 2: movq   8(<workp=%rdi),>mulrax=%rax
movq   8(%rdi),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * mulx2
# asm 1: mul  <mulx2=int64#8
# asm 2: mul  <mulx2=%r10
mul  %r10

# qhasm:   carry? zq3 += mulrax
# asm 1: add  <mulrax=int64#7,<zq3=int64#12
# asm 2: add  <mulrax=%rax,<zq3=%r14
add  %rax,%r14

# qhasm:   mulrdx += 0 + carry
# asm 1: adc $0,<mulrdx=int64#3
# asm 2: adc $0,<mulrdx=%rdx
adc $0,%rdx

# qhasm:   carry? zq3 += mulc
# asm 1: add  <mulc=int64#13,<zq3=int64#12
# asm 2: add  <mulc=%r15,<zq3=%r14
add  %r15,%r14

# qhasm:   mulc = 0
# asm 1: mov  $0,>mulc=int64#13
# asm 2: mov  $0,>mulc=%r15
mov  $0,%r15

# qhasm:   mulc += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulc=int64#13
# asm 2: adc <mulrdx=%rdx,<mulc=%r15
adc %rdx,%r15

# qhasm:   mulrax = *(uint64 *)(workp + 16)
# asm 1: movq   16(<workp=int64#1),>mulrax=int64#7
# asm 2: movq   16(<workp=%rdi),>mulrax=%rax
movq   16(%rdi),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * mulx2
# asm 1: mul  <mulx2=int64#8
# asm 2: mul  <mulx2=%r10
mul  %r10

# qhasm:   carry? mulr4 += mulrax
# asm 1: add  <mulrax=int64#7,<mulr4=int64#2
# asm 2: add  <mulrax=%rax,<mulr4=%rsi
add  %rax,%rsi

# qhasm:   mulrdx += 0 + carry
# asm 1: adc $0,<mulrdx=int64#3
# asm 2: adc $0,<mulrdx=%rdx
adc $0,%rdx

# qhasm:   carry? mulr4 += mulc
# asm 1: add  <mulc=int64#13,<mulr4=int64#2
# asm 2: add  <mulc=%r15,<mulr4=%rsi
add  %r15,%rsi

# qhasm:   mulc = 0
# asm 1: mov  $0,>mulc=int64#13
# asm 2: mov  $0,>mulc=%r15
mov  $0,%r15

# qhasm:   mulc += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulc=int64#13
# asm 2: adc <mulrdx=%rdx,<mulc=%r15
adc %rdx,%r15

# qhasm:   mulrax = *(uint64 *)(workp + 24)
# asm 1: movq   24(<workp=int64#1),>mulrax=int64#7
# asm 2: movq   24(<workp=%rdi),>mulrax=%rax
movq   24(%rdi),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * mulx2
# asm 1: mul  <mulx2=int64#8
# asm 2: mul  <mulx2=%r10
mul  %r10

# qhasm:   carry? mulr5 += mulrax
# asm 1: add  <mulrax=int64#7,<mulr5=int64#4
# asm 2: add  <mulrax=%rax,<mulr5=%rcx
add  %rax,%rcx

# qhasm:   mulrdx += 0 + carry
# asm 1: adc $0,<mulrdx=int64#3
# asm 2: adc $0,<mulrdx=%rdx
adc $0,%rdx

# qhasm:   carry? mulr5 += mulc
# asm 1: add  <mulc=int64#13,<mulr5=int64#4
# asm 2: add  <mulc=%r15,<mulr5=%rcx
add  %r15,%rcx

# qhasm:   mulr6 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr6=int64#5
# asm 2: adc <mulrdx=%rdx,<mulr6=%r8
adc %rdx,%r8

# qhasm:   mulx3 = *(uint64 *)(workp + 152)
# asm 1: movq   152(<workp=int64#1),>mulx3=int64#8
# asm 2: movq   152(<workp=%rdi),>mulx3=%r10
movq   152(%rdi),%r10

# qhasm:   mulrax = *(uint64 *)(workp + 0)
# asm 1: movq   0(<workp=int64#1),>mulrax=int64#7
# asm 2: movq   0(<workp=%rdi),>mulrax=%rax
movq   0(%rdi),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * mulx3
# asm 1: mul  <mulx3=int64#8
# asm 2: mul  <mulx3=%r10
mul  %r10

# qhasm:   carry? zq3 += mulrax
# asm 1: add  <mulrax=int64#7,<zq3=int64#12
# asm 2: add  <mulrax=%rax,<zq3=%r14
add  %rax,%r14

# qhasm:   mulc = 0
# asm 1: mov  $0,>mulc=int64#13
# asm 2: mov  $0,>mulc=%r15
mov  $0,%r15

# qhasm:   mulc += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulc=int64#13
# asm 2: adc <mulrdx=%rdx,<mulc=%r15
adc %rdx,%r15

# qhasm:   mulrax = *(uint64 *)(workp + 8)
# asm 1: movq   8(<workp=int64#1),>mulrax=int64#7
# asm 2: movq   8(<workp=%rdi),>mulrax=%rax
movq   8(%rdi),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * mulx3
# asm 1: mul  <mulx3=int64#8
# asm 2: mul  <mulx3=%r10
mul  %r10

# qhasm:   carry? mulr4 += mulrax
# asm 1: add  <mulrax=int64#7,<mulr4=int64#2
# asm 2: add  <mulrax=%rax,<mulr4=%rsi
add  %rax,%rsi

# qhasm:   mulrdx += 0 + carry
# asm 1: adc $0,<mulrdx=int64#3
# asm 2: adc $0,<mulrdx=%rdx
adc $0,%rdx

# qhasm:   carry? mulr4 += mulc
# asm 1: add  <mulc=int64#13,<mulr4=int64#2
# asm 2: add  <mulc=%r15,<mulr4=%rsi
add  %r15,%rsi

# qhasm:   mulc = 0
# asm 1: mov  $0,>mulc=int64#13
# asm 2: mov  $0,>mulc=%r15
mov  $0,%r15

# qhasm:   mulc += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulc=int64#13
# asm 2: adc <mulrdx=%rdx,<mulc=%r15
adc %rdx,%r15

# qhasm:   mulrax = *(uint64 *)(workp + 16)
# asm 1: movq   16(<workp=int64#1),>mulrax=int64#7
# asm 2: movq   16(<workp=%rdi),>mulrax=%rax
movq   16(%rdi),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * mulx3
# asm 1: mul  <mulx3=int64#8
# asm 2: mul  <mulx3=%r10
mul  %r10

# qhasm:   carry? mulr5 += mulrax
# asm 1: add  <mulrax=int64#7,<mulr5=int64#4
# asm 2: add  <mulrax=%rax,<mulr5=%rcx
add  %rax,%rcx

# qhasm:   mulrdx += 0 + carry
# asm 1: adc $0,<mulrdx=int64#3
# asm 2: adc $0,<mulrdx=%rdx
adc $0,%rdx

# qhasm:   carry? mulr5 += mulc
# asm 1: add  <mulc=int64#13,<mulr5=int64#4
# asm 2: add  <mulc=%r15,<mulr5=%rcx
add  %r15,%rcx

# qhasm:   mulc = 0
# asm 1: mov  $0,>mulc=int64#13
# asm 2: mov  $0,>mulc=%r15
mov  $0,%r15

# qhasm:   mulc += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulc=int64#13
# asm 2: adc <mulrdx=%rdx,<mulc=%r15
adc %rdx,%r15

# qhasm:   mulrax = *(uint64 *)(workp + 24)
# asm 1: movq   24(<workp=int64#1),>mulrax=int64#7
# asm 2: movq   24(<workp=%rdi),>mulrax=%rax
movq   24(%rdi),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * mulx3
# asm 1: mul  <mulx3=int64#8
# asm 2: mul  <mulx3=%r10
mul  %r10

# qhasm:   carry? mulr6 += mulrax
# asm 1: add  <mulrax=int64#7,<mulr6=int64#5
# asm 2: add  <mulrax=%rax,<mulr6=%r8
add  %rax,%r8

# qhasm:   mulrdx += 0 + carry
# asm 1: adc $0,<mulrdx=int64#3
# asm 2: adc $0,<mulrdx=%rdx
adc $0,%rdx

# qhasm:   carry? mulr6 += mulc
# asm 1: add  <mulc=int64#13,<mulr6=int64#5
# asm 2: add  <mulc=%r15,<mulr6=%r8
add  %r15,%r8

# qhasm:   mulr7 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr7=int64#6
# asm 2: adc <mulrdx=%rdx,<mulr7=%r9
adc %rdx,%r9

# qhasm:   mulrax = mulr4
# asm 1: mov  <mulr4=int64#2,>mulrax=int64#7
# asm 2: mov  <mulr4=%rsi,>mulrax=%rax
mov  %rsi,%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * *(uint64 *)&crypto_scalarmult_curve25519_amd64_64_38
mulq  crypto_scalarmult_curve25519_amd64_64_38

# qhasm:   mulr4 = mulrax
# asm 1: mov  <mulrax=int64#7,>mulr4=int64#2
# asm 2: mov  <mulrax=%rax,>mulr4=%rsi
mov  %rax,%rsi

# qhasm:   mulrax = mulr5
# asm 1: mov  <mulr5=int64#4,>mulrax=int64#7
# asm 2: mov  <mulr5=%rcx,>mulrax=%rax
mov  %rcx,%rax

# qhasm:   mulr5 = mulrdx
# asm 1: mov  <mulrdx=int64#3,>mulr5=int64#4
# asm 2: mov  <mulrdx=%rdx,>mulr5=%rcx
mov  %rdx,%rcx

# qhasm:   (uint128) mulrdx mulrax = mulrax * *(uint64 *)&crypto_scalarmult_curve25519_amd64_64_38
mulq  crypto_scalarmult_curve25519_amd64_64_38

# qhasm:   carry? mulr5 += mulrax
# asm 1: add  <mulrax=int64#7,<mulr5=int64#4
# asm 2: add  <mulrax=%rax,<mulr5=%rcx
add  %rax,%rcx

# qhasm:   mulrax = mulr6
# asm 1: mov  <mulr6=int64#5,>mulrax=int64#7
# asm 2: mov  <mulr6=%r8,>mulrax=%rax
mov  %r8,%rax

# qhasm:   mulr6 = 0
# asm 1: mov  $0,>mulr6=int64#5
# asm 2: mov  $0,>mulr6=%r8
mov  $0,%r8

# qhasm:   mulr6 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr6=int64#5
# asm 2: adc <mulrdx=%rdx,<mulr6=%r8
adc %rdx,%r8

# qhasm:   (uint128) mulrdx mulrax = mulrax * *(uint64 *)&crypto_scalarmult_curve25519_amd64_64_38
mulq  crypto_scalarmult_curve25519_amd64_64_38

# qhasm:   carry? mulr6 += mulrax
# asm 1: add  <mulrax=int64#7,<mulr6=int64#5
# asm 2: add  <mulrax=%rax,<mulr6=%r8
add  %rax,%r8

# qhasm:   mulrax = mulr7
# asm 1: mov  <mulr7=int64#6,>mulrax=int64#7
# asm 2: mov  <mulr7=%r9,>mulrax=%rax
mov  %r9,%rax

# qhasm:   mulr7 = 0
# asm 1: mov  $0,>mulr7=int64#6
# asm 2: mov  $0,>mulr7=%r9
mov  $0,%r9

# qhasm:   mulr7 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr7=int64#6
# asm 2: adc <mulrdx=%rdx,<mulr7=%r9
adc %rdx,%r9

# qhasm:   (uint128) mulrdx mulrax = mulrax * *(uint64 *)&crypto_scalarmult_curve25519_amd64_64_38
mulq  crypto_scalarmult_curve25519_amd64_64_38

# qhasm:   carry? mulr7 += mulrax
# asm 1: add  <mulrax=int64#7,<mulr7=int64#6
# asm 2: add  <mulrax=%rax,<mulr7=%r9
add  %rax,%r9

# qhasm:   mulr8 = 0
# asm 1: mov  $0,>mulr8=int64#7
# asm 2: mov  $0,>mulr8=%rax
mov  $0,%rax

# qhasm:   mulr8 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr8=int64#7
# asm 2: adc <mulrdx=%rdx,<mulr8=%rax
adc %rdx,%rax

# qhasm:   carry? zq0 += mulr4
# asm 1: add  <mulr4=int64#2,<zq0=int64#9
# asm 2: add  <mulr4=%rsi,<zq0=%r11
add  %rsi,%r11

# qhasm:   carry? zq1 += mulr5 + carry
# asm 1: adc <mulr5=int64#4,<zq1=int64#10
# asm 2: adc <mulr5=%rcx,<zq1=%r12
adc %rcx,%r12

# qhasm:   carry? zq2 += mulr6 + carry
# asm 1: adc <mulr6=int64#5,<zq2=int64#11
# asm 2: adc <mulr6=%r8,<zq2=%r13
adc %r8,%r13

# qhasm:   carry? zq3 += mulr7 + carry
# asm 1: adc <mulr7=int64#6,<zq3=int64#12
# asm 2: adc <mulr7=%r9,<zq3=%r14
adc %r9,%r14

# qhasm:   mulzero = 0
# asm 1: mov  $0,>mulzero=int64#2
# asm 2: mov  $0,>mulzero=%rsi
mov  $0,%rsi

# qhasm:   mulr8 += mulzero + carry
# asm 1: adc <mulzero=int64#2,<mulr8=int64#7
# asm 2: adc <mulzero=%rsi,<mulr8=%rax
adc %rsi,%rax

# qhasm:   mulr8 *= 38
# asm 1: imulq  $38,<mulr8=int64#7,>mulr8=int64#3
# asm 2: imulq  $38,<mulr8=%rax,>mulr8=%rdx
imulq  $38,%rax,%rdx

# qhasm:   carry? zq0 += mulr8
# asm 1: add  <mulr8=int64#3,<zq0=int64#9
# asm 2: add  <mulr8=%rdx,<zq0=%r11
add  %rdx,%r11

# qhasm:   carry? zq1 += mulzero + carry
# asm 1: adc <mulzero=int64#2,<zq1=int64#10
# asm 2: adc <mulzero=%rsi,<zq1=%r12
adc %rsi,%r12

# qhasm:   carry? zq2 += mulzero + carry
# asm 1: adc <mulzero=int64#2,<zq2=int64#11
# asm 2: adc <mulzero=%rsi,<zq2=%r13
adc %rsi,%r13

# qhasm:   carry? zq3 += mulzero + carry
# asm 1: adc <mulzero=int64#2,<zq3=int64#12
# asm 2: adc <mulzero=%rsi,<zq3=%r14
adc %rsi,%r14

# qhasm:   mulzero += mulzero + carry
# asm 1: adc <mulzero=int64#2,<mulzero=int64#2
# asm 2: adc <mulzero=%rsi,<mulzero=%rsi
adc %rsi,%rsi

# qhasm:   mulzero *= 38
# asm 1: imulq  $38,<mulzero=int64#2,>mulzero=int64#2
# asm 2: imulq  $38,<mulzero=%rsi,>mulzero=%rsi
imulq  $38,%rsi,%rsi

# qhasm:   zq0 += mulzero
# asm 1: add  <mulzero=int64#2,<zq0=int64#9
# asm 2: add  <mulzero=%rsi,<zq0=%r11
add  %rsi,%r11

# qhasm: *(uint64 *)(workp + 128) = zq0
# asm 1: movq   <zq0=int64#9,128(<workp=int64#1)
# asm 2: movq   <zq0=%r11,128(<workp=%rdi)
movq   %r11,128(%rdi)

# qhasm: *(uint64 *)(workp + 136) = zq1
# asm 1: movq   <zq1=int64#10,136(<workp=int64#1)
# asm 2: movq   <zq1=%r12,136(<workp=%rdi)
movq   %r12,136(%rdi)

# qhasm: *(uint64 *)(workp + 144) = zq2
# asm 1: movq   <zq2=int64#11,144(<workp=int64#1)
# asm 2: movq   <zq2=%r13,144(<workp=%rdi)
movq   %r13,144(%rdi)

# qhasm: *(uint64 *)(workp + 152) = zq3
# asm 1: movq   <zq3=int64#12,152(<workp=int64#1)
# asm 2: movq   <zq3=%r14,152(<workp=%rdi)
movq   %r14,152(%rdi)

# qhasm:   mulr4 = 0
# asm 1: mov  $0,>mulr4=int64#2
# asm 2: mov  $0,>mulr4=%rsi
mov  $0,%rsi

# qhasm:   mulr5 = 0
# asm 1: mov  $0,>mulr5=int64#4
# asm 2: mov  $0,>mulr5=%rcx
mov  $0,%rcx

# qhasm:   mulr6 = 0
# asm 1: mov  $0,>mulr6=int64#5
# asm 2: mov  $0,>mulr6=%r8
mov  $0,%r8

# qhasm:   mulr7 = 0
# asm 1: mov  $0,>mulr7=int64#6
# asm 2: mov  $0,>mulr7=%r9
mov  $0,%r9

# qhasm:   mulx0 = t60_stack
# asm 1: movq <t60_stack=stack64#20,>mulx0=int64#8
# asm 2: movq <t60_stack=152(%rsp),>mulx0=%r10
movq 152(%rsp),%r10

# qhasm:   mulrax = t70_stack
# asm 1: movq <t70_stack=stack64#16,>mulrax=int64#7
# asm 2: movq <t70_stack=120(%rsp),>mulrax=%rax
movq 120(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * mulx0
# asm 1: mul  <mulx0=int64#8
# asm 2: mul  <mulx0=%r10
mul  %r10

# qhasm:   xp0 = mulrax
# asm 1: mov  <mulrax=int64#7,>xp0=int64#9
# asm 2: mov  <mulrax=%rax,>xp0=%r11
mov  %rax,%r11

# qhasm:   xp1 = mulrdx
# asm 1: mov  <mulrdx=int64#3,>xp1=int64#10
# asm 2: mov  <mulrdx=%rdx,>xp1=%r12
mov  %rdx,%r12

# qhasm:   mulrax = t71_stack
# asm 1: movq <t71_stack=stack64#17,>mulrax=int64#7
# asm 2: movq <t71_stack=128(%rsp),>mulrax=%rax
movq 128(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * mulx0
# asm 1: mul  <mulx0=int64#8
# asm 2: mul  <mulx0=%r10
mul  %r10

# qhasm:   carry? xp1 += mulrax
# asm 1: add  <mulrax=int64#7,<xp1=int64#10
# asm 2: add  <mulrax=%rax,<xp1=%r12
add  %rax,%r12

# qhasm:   xp2 = 0
# asm 1: mov  $0,>xp2=int64#11
# asm 2: mov  $0,>xp2=%r13
mov  $0,%r13

# qhasm:   xp2 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<xp2=int64#11
# asm 2: adc <mulrdx=%rdx,<xp2=%r13
adc %rdx,%r13

# qhasm:   mulrax = t72_stack
# asm 1: movq <t72_stack=stack64#18,>mulrax=int64#7
# asm 2: movq <t72_stack=136(%rsp),>mulrax=%rax
movq 136(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * mulx0
# asm 1: mul  <mulx0=int64#8
# asm 2: mul  <mulx0=%r10
mul  %r10

# qhasm:   carry? xp2 += mulrax
# asm 1: add  <mulrax=int64#7,<xp2=int64#11
# asm 2: add  <mulrax=%rax,<xp2=%r13
add  %rax,%r13

# qhasm:   xp3 = 0
# asm 1: mov  $0,>xp3=int64#12
# asm 2: mov  $0,>xp3=%r14
mov  $0,%r14

# qhasm:   xp3 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<xp3=int64#12
# asm 2: adc <mulrdx=%rdx,<xp3=%r14
adc %rdx,%r14

# qhasm:   mulrax = t73_stack
# asm 1: movq <t73_stack=stack64#19,>mulrax=int64#7
# asm 2: movq <t73_stack=144(%rsp),>mulrax=%rax
movq 144(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * mulx0
# asm 1: mul  <mulx0=int64#8
# asm 2: mul  <mulx0=%r10
mul  %r10

# qhasm:   carry? xp3 += mulrax
# asm 1: add  <mulrax=int64#7,<xp3=int64#12
# asm 2: add  <mulrax=%rax,<xp3=%r14
add  %rax,%r14

# qhasm:   mulr4 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr4=int64#2
# asm 2: adc <mulrdx=%rdx,<mulr4=%rsi
adc %rdx,%rsi

# qhasm:   mulx1 = t61_stack
# asm 1: movq <t61_stack=stack64#21,>mulx1=int64#8
# asm 2: movq <t61_stack=160(%rsp),>mulx1=%r10
movq 160(%rsp),%r10

# qhasm:   mulrax = t70_stack
# asm 1: movq <t70_stack=stack64#16,>mulrax=int64#7
# asm 2: movq <t70_stack=120(%rsp),>mulrax=%rax
movq 120(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * mulx1
# asm 1: mul  <mulx1=int64#8
# asm 2: mul  <mulx1=%r10
mul  %r10

# qhasm:   carry? xp1 += mulrax
# asm 1: add  <mulrax=int64#7,<xp1=int64#10
# asm 2: add  <mulrax=%rax,<xp1=%r12
add  %rax,%r12

# qhasm:   mulc = 0
# asm 1: mov  $0,>mulc=int64#13
# asm 2: mov  $0,>mulc=%r15
mov  $0,%r15

# qhasm:   mulc += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulc=int64#13
# asm 2: adc <mulrdx=%rdx,<mulc=%r15
adc %rdx,%r15

# qhasm:   mulrax = t71_stack
# asm 1: movq <t71_stack=stack64#17,>mulrax=int64#7
# asm 2: movq <t71_stack=128(%rsp),>mulrax=%rax
movq 128(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * mulx1
# asm 1: mul  <mulx1=int64#8
# asm 2: mul  <mulx1=%r10
mul  %r10

# qhasm:   carry? xp2 += mulrax
# asm 1: add  <mulrax=int64#7,<xp2=int64#11
# asm 2: add  <mulrax=%rax,<xp2=%r13
add  %rax,%r13

# qhasm:   mulrdx += 0 + carry
# asm 1: adc $0,<mulrdx=int64#3
# asm 2: adc $0,<mulrdx=%rdx
adc $0,%rdx

# qhasm:   carry? xp2 += mulc
# asm 1: add  <mulc=int64#13,<xp2=int64#11
# asm 2: add  <mulc=%r15,<xp2=%r13
add  %r15,%r13

# qhasm:   mulc = 0
# asm 1: mov  $0,>mulc=int64#13
# asm 2: mov  $0,>mulc=%r15
mov  $0,%r15

# qhasm:   mulc += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulc=int64#13
# asm 2: adc <mulrdx=%rdx,<mulc=%r15
adc %rdx,%r15

# qhasm:   mulrax = t72_stack
# asm 1: movq <t72_stack=stack64#18,>mulrax=int64#7
# asm 2: movq <t72_stack=136(%rsp),>mulrax=%rax
movq 136(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * mulx1
# asm 1: mul  <mulx1=int64#8
# asm 2: mul  <mulx1=%r10
mul  %r10

# qhasm:   carry? xp3 += mulrax 
# asm 1: add  <mulrax=int64#7,<xp3=int64#12
# asm 2: add  <mulrax=%rax,<xp3=%r14
add  %rax,%r14

# qhasm:   mulrdx += 0 + carry
# asm 1: adc $0,<mulrdx=int64#3
# asm 2: adc $0,<mulrdx=%rdx
adc $0,%rdx

# qhasm:   carry? xp3 += mulc
# asm 1: add  <mulc=int64#13,<xp3=int64#12
# asm 2: add  <mulc=%r15,<xp3=%r14
add  %r15,%r14

# qhasm:   mulc = 0
# asm 1: mov  $0,>mulc=int64#13
# asm 2: mov  $0,>mulc=%r15
mov  $0,%r15

# qhasm:   mulc += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulc=int64#13
# asm 2: adc <mulrdx=%rdx,<mulc=%r15
adc %rdx,%r15

# qhasm:   mulrax = t73_stack
# asm 1: movq <t73_stack=stack64#19,>mulrax=int64#7
# asm 2: movq <t73_stack=144(%rsp),>mulrax=%rax
movq 144(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * mulx1
# asm 1: mul  <mulx1=int64#8
# asm 2: mul  <mulx1=%r10
mul  %r10

# qhasm:   carry? mulr4 += mulrax
# asm 1: add  <mulrax=int64#7,<mulr4=int64#2
# asm 2: add  <mulrax=%rax,<mulr4=%rsi
add  %rax,%rsi

# qhasm:   mulrdx += 0 + carry
# asm 1: adc $0,<mulrdx=int64#3
# asm 2: adc $0,<mulrdx=%rdx
adc $0,%rdx

# qhasm:   carry? mulr4 += mulc
# asm 1: add  <mulc=int64#13,<mulr4=int64#2
# asm 2: add  <mulc=%r15,<mulr4=%rsi
add  %r15,%rsi

# qhasm:   mulr5 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr5=int64#4
# asm 2: adc <mulrdx=%rdx,<mulr5=%rcx
adc %rdx,%rcx

# qhasm:   mulx2 = t62_stack
# asm 1: movq <t62_stack=stack64#22,>mulx2=int64#8
# asm 2: movq <t62_stack=168(%rsp),>mulx2=%r10
movq 168(%rsp),%r10

# qhasm:   mulrax = t70_stack
# asm 1: movq <t70_stack=stack64#16,>mulrax=int64#7
# asm 2: movq <t70_stack=120(%rsp),>mulrax=%rax
movq 120(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * mulx2
# asm 1: mul  <mulx2=int64#8
# asm 2: mul  <mulx2=%r10
mul  %r10

# qhasm:   carry? xp2 += mulrax
# asm 1: add  <mulrax=int64#7,<xp2=int64#11
# asm 2: add  <mulrax=%rax,<xp2=%r13
add  %rax,%r13

# qhasm:   mulc = 0
# asm 1: mov  $0,>mulc=int64#13
# asm 2: mov  $0,>mulc=%r15
mov  $0,%r15

# qhasm:   mulc += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulc=int64#13
# asm 2: adc <mulrdx=%rdx,<mulc=%r15
adc %rdx,%r15

# qhasm:   mulrax = t71_stack
# asm 1: movq <t71_stack=stack64#17,>mulrax=int64#7
# asm 2: movq <t71_stack=128(%rsp),>mulrax=%rax
movq 128(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * mulx2
# asm 1: mul  <mulx2=int64#8
# asm 2: mul  <mulx2=%r10
mul  %r10

# qhasm:   carry? xp3 += mulrax
# asm 1: add  <mulrax=int64#7,<xp3=int64#12
# asm 2: add  <mulrax=%rax,<xp3=%r14
add  %rax,%r14

# qhasm:   mulrdx += 0 + carry
# asm 1: adc $0,<mulrdx=int64#3
# asm 2: adc $0,<mulrdx=%rdx
adc $0,%rdx

# qhasm:   carry? xp3 += mulc
# asm 1: add  <mulc=int64#13,<xp3=int64#12
# asm 2: add  <mulc=%r15,<xp3=%r14
add  %r15,%r14

# qhasm:   mulc = 0
# asm 1: mov  $0,>mulc=int64#13
# asm 2: mov  $0,>mulc=%r15
mov  $0,%r15

# qhasm:   mulc += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulc=int64#13
# asm 2: adc <mulrdx=%rdx,<mulc=%r15
adc %rdx,%r15

# qhasm:   mulrax = t72_stack
# asm 1: movq <t72_stack=stack64#18,>mulrax=int64#7
# asm 2: movq <t72_stack=136(%rsp),>mulrax=%rax
movq 136(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * mulx2
# asm 1: mul  <mulx2=int64#8
# asm 2: mul  <mulx2=%r10
mul  %r10

# qhasm:   carry? mulr4 += mulrax
# asm 1: add  <mulrax=int64#7,<mulr4=int64#2
# asm 2: add  <mulrax=%rax,<mulr4=%rsi
add  %rax,%rsi

# qhasm:   mulrdx += 0 + carry
# asm 1: adc $0,<mulrdx=int64#3
# asm 2: adc $0,<mulrdx=%rdx
adc $0,%rdx

# qhasm:   carry? mulr4 += mulc
# asm 1: add  <mulc=int64#13,<mulr4=int64#2
# asm 2: add  <mulc=%r15,<mulr4=%rsi
add  %r15,%rsi

# qhasm:   mulc = 0
# asm 1: mov  $0,>mulc=int64#13
# asm 2: mov  $0,>mulc=%r15
mov  $0,%r15

# qhasm:   mulc += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulc=int64#13
# asm 2: adc <mulrdx=%rdx,<mulc=%r15
adc %rdx,%r15

# qhasm:   mulrax = t73_stack
# asm 1: movq <t73_stack=stack64#19,>mulrax=int64#7
# asm 2: movq <t73_stack=144(%rsp),>mulrax=%rax
movq 144(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * mulx2
# asm 1: mul  <mulx2=int64#8
# asm 2: mul  <mulx2=%r10
mul  %r10

# qhasm:   carry? mulr5 += mulrax
# asm 1: add  <mulrax=int64#7,<mulr5=int64#4
# asm 2: add  <mulrax=%rax,<mulr5=%rcx
add  %rax,%rcx

# qhasm:   mulrdx += 0 + carry
# asm 1: adc $0,<mulrdx=int64#3
# asm 2: adc $0,<mulrdx=%rdx
adc $0,%rdx

# qhasm:   carry? mulr5 += mulc
# asm 1: add  <mulc=int64#13,<mulr5=int64#4
# asm 2: add  <mulc=%r15,<mulr5=%rcx
add  %r15,%rcx

# qhasm:   mulr6 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr6=int64#5
# asm 2: adc <mulrdx=%rdx,<mulr6=%r8
adc %rdx,%r8

# qhasm:   mulx3 = t63_stack
# asm 1: movq <t63_stack=stack64#23,>mulx3=int64#8
# asm 2: movq <t63_stack=176(%rsp),>mulx3=%r10
movq 176(%rsp),%r10

# qhasm:   mulrax = t70_stack
# asm 1: movq <t70_stack=stack64#16,>mulrax=int64#7
# asm 2: movq <t70_stack=120(%rsp),>mulrax=%rax
movq 120(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * mulx3
# asm 1: mul  <mulx3=int64#8
# asm 2: mul  <mulx3=%r10
mul  %r10

# qhasm:   carry? xp3 += mulrax
# asm 1: add  <mulrax=int64#7,<xp3=int64#12
# asm 2: add  <mulrax=%rax,<xp3=%r14
add  %rax,%r14

# qhasm:   mulc = 0
# asm 1: mov  $0,>mulc=int64#13
# asm 2: mov  $0,>mulc=%r15
mov  $0,%r15

# qhasm:   mulc += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulc=int64#13
# asm 2: adc <mulrdx=%rdx,<mulc=%r15
adc %rdx,%r15

# qhasm:   mulrax = t71_stack
# asm 1: movq <t71_stack=stack64#17,>mulrax=int64#7
# asm 2: movq <t71_stack=128(%rsp),>mulrax=%rax
movq 128(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * mulx3
# asm 1: mul  <mulx3=int64#8
# asm 2: mul  <mulx3=%r10
mul  %r10

# qhasm:   carry? mulr4 += mulrax
# asm 1: add  <mulrax=int64#7,<mulr4=int64#2
# asm 2: add  <mulrax=%rax,<mulr4=%rsi
add  %rax,%rsi

# qhasm:   mulrdx += 0 + carry
# asm 1: adc $0,<mulrdx=int64#3
# asm 2: adc $0,<mulrdx=%rdx
adc $0,%rdx

# qhasm:   carry? mulr4 += mulc
# asm 1: add  <mulc=int64#13,<mulr4=int64#2
# asm 2: add  <mulc=%r15,<mulr4=%rsi
add  %r15,%rsi

# qhasm:   mulc = 0
# asm 1: mov  $0,>mulc=int64#13
# asm 2: mov  $0,>mulc=%r15
mov  $0,%r15

# qhasm:   mulc += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulc=int64#13
# asm 2: adc <mulrdx=%rdx,<mulc=%r15
adc %rdx,%r15

# qhasm:   mulrax = t72_stack
# asm 1: movq <t72_stack=stack64#18,>mulrax=int64#7
# asm 2: movq <t72_stack=136(%rsp),>mulrax=%rax
movq 136(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * mulx3
# asm 1: mul  <mulx3=int64#8
# asm 2: mul  <mulx3=%r10
mul  %r10

# qhasm:   carry? mulr5 += mulrax
# asm 1: add  <mulrax=int64#7,<mulr5=int64#4
# asm 2: add  <mulrax=%rax,<mulr5=%rcx
add  %rax,%rcx

# qhasm:   mulrdx += 0 + carry
# asm 1: adc $0,<mulrdx=int64#3
# asm 2: adc $0,<mulrdx=%rdx
adc $0,%rdx

# qhasm:   carry? mulr5 += mulc
# asm 1: add  <mulc=int64#13,<mulr5=int64#4
# asm 2: add  <mulc=%r15,<mulr5=%rcx
add  %r15,%rcx

# qhasm:   mulc = 0
# asm 1: mov  $0,>mulc=int64#13
# asm 2: mov  $0,>mulc=%r15
mov  $0,%r15

# qhasm:   mulc += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulc=int64#13
# asm 2: adc <mulrdx=%rdx,<mulc=%r15
adc %rdx,%r15

# qhasm:   mulrax = t73_stack
# asm 1: movq <t73_stack=stack64#19,>mulrax=int64#7
# asm 2: movq <t73_stack=144(%rsp),>mulrax=%rax
movq 144(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * mulx3
# asm 1: mul  <mulx3=int64#8
# asm 2: mul  <mulx3=%r10
mul  %r10

# qhasm:   carry? mulr6 += mulrax
# asm 1: add  <mulrax=int64#7,<mulr6=int64#5
# asm 2: add  <mulrax=%rax,<mulr6=%r8
add  %rax,%r8

# qhasm:   mulrdx += 0 + carry
# asm 1: adc $0,<mulrdx=int64#3
# asm 2: adc $0,<mulrdx=%rdx
adc $0,%rdx

# qhasm:   carry? mulr6 += mulc
# asm 1: add  <mulc=int64#13,<mulr6=int64#5
# asm 2: add  <mulc=%r15,<mulr6=%r8
add  %r15,%r8

# qhasm:   mulr7 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr7=int64#6
# asm 2: adc <mulrdx=%rdx,<mulr7=%r9
adc %rdx,%r9

# qhasm:   mulrax = mulr4
# asm 1: mov  <mulr4=int64#2,>mulrax=int64#7
# asm 2: mov  <mulr4=%rsi,>mulrax=%rax
mov  %rsi,%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * *(uint64 *)&crypto_scalarmult_curve25519_amd64_64_38
mulq  crypto_scalarmult_curve25519_amd64_64_38

# qhasm:   mulr4 = mulrax
# asm 1: mov  <mulrax=int64#7,>mulr4=int64#2
# asm 2: mov  <mulrax=%rax,>mulr4=%rsi
mov  %rax,%rsi

# qhasm:   mulrax = mulr5
# asm 1: mov  <mulr5=int64#4,>mulrax=int64#7
# asm 2: mov  <mulr5=%rcx,>mulrax=%rax
mov  %rcx,%rax

# qhasm:   mulr5 = mulrdx
# asm 1: mov  <mulrdx=int64#3,>mulr5=int64#4
# asm 2: mov  <mulrdx=%rdx,>mulr5=%rcx
mov  %rdx,%rcx

# qhasm:   (uint128) mulrdx mulrax = mulrax * *(uint64 *)&crypto_scalarmult_curve25519_amd64_64_38
mulq  crypto_scalarmult_curve25519_amd64_64_38

# qhasm:   carry? mulr5 += mulrax
# asm 1: add  <mulrax=int64#7,<mulr5=int64#4
# asm 2: add  <mulrax=%rax,<mulr5=%rcx
add  %rax,%rcx

# qhasm:   mulrax = mulr6
# asm 1: mov  <mulr6=int64#5,>mulrax=int64#7
# asm 2: mov  <mulr6=%r8,>mulrax=%rax
mov  %r8,%rax

# qhasm:   mulr6 = 0
# asm 1: mov  $0,>mulr6=int64#5
# asm 2: mov  $0,>mulr6=%r8
mov  $0,%r8

# qhasm:   mulr6 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr6=int64#5
# asm 2: adc <mulrdx=%rdx,<mulr6=%r8
adc %rdx,%r8

# qhasm:   (uint128) mulrdx mulrax = mulrax * *(uint64 *)&crypto_scalarmult_curve25519_amd64_64_38
mulq  crypto_scalarmult_curve25519_amd64_64_38

# qhasm:   carry? mulr6 += mulrax
# asm 1: add  <mulrax=int64#7,<mulr6=int64#5
# asm 2: add  <mulrax=%rax,<mulr6=%r8
add  %rax,%r8

# qhasm:   mulrax = mulr7
# asm 1: mov  <mulr7=int64#6,>mulrax=int64#7
# asm 2: mov  <mulr7=%r9,>mulrax=%rax
mov  %r9,%rax

# qhasm:   mulr7 = 0
# asm 1: mov  $0,>mulr7=int64#6
# asm 2: mov  $0,>mulr7=%r9
mov  $0,%r9

# qhasm:   mulr7 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr7=int64#6
# asm 2: adc <mulrdx=%rdx,<mulr7=%r9
adc %rdx,%r9

# qhasm:   (uint128) mulrdx mulrax = mulrax * *(uint64 *)&crypto_scalarmult_curve25519_amd64_64_38
mulq  crypto_scalarmult_curve25519_amd64_64_38

# qhasm:   carry? mulr7 += mulrax
# asm 1: add  <mulrax=int64#7,<mulr7=int64#6
# asm 2: add  <mulrax=%rax,<mulr7=%r9
add  %rax,%r9

# qhasm:   mulr8 = 0
# asm 1: mov  $0,>mulr8=int64#7
# asm 2: mov  $0,>mulr8=%rax
mov  $0,%rax

# qhasm:   mulr8 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr8=int64#7
# asm 2: adc <mulrdx=%rdx,<mulr8=%rax
adc %rdx,%rax

# qhasm:   carry? xp0 += mulr4
# asm 1: add  <mulr4=int64#2,<xp0=int64#9
# asm 2: add  <mulr4=%rsi,<xp0=%r11
add  %rsi,%r11

# qhasm:   carry? xp1 += mulr5 + carry
# asm 1: adc <mulr5=int64#4,<xp1=int64#10
# asm 2: adc <mulr5=%rcx,<xp1=%r12
adc %rcx,%r12

# qhasm:   carry? xp2 += mulr6 + carry
# asm 1: adc <mulr6=int64#5,<xp2=int64#11
# asm 2: adc <mulr6=%r8,<xp2=%r13
adc %r8,%r13

# qhasm:   carry? xp3 += mulr7 + carry
# asm 1: adc <mulr7=int64#6,<xp3=int64#12
# asm 2: adc <mulr7=%r9,<xp3=%r14
adc %r9,%r14

# qhasm:   mulzero = 0
# asm 1: mov  $0,>mulzero=int64#2
# asm 2: mov  $0,>mulzero=%rsi
mov  $0,%rsi

# qhasm:   mulr8 += mulzero + carry
# asm 1: adc <mulzero=int64#2,<mulr8=int64#7
# asm 2: adc <mulzero=%rsi,<mulr8=%rax
adc %rsi,%rax

# qhasm:   mulr8 *= 38
# asm 1: imulq  $38,<mulr8=int64#7,>mulr8=int64#3
# asm 2: imulq  $38,<mulr8=%rax,>mulr8=%rdx
imulq  $38,%rax,%rdx

# qhasm:   carry? xp0 += mulr8
# asm 1: add  <mulr8=int64#3,<xp0=int64#9
# asm 2: add  <mulr8=%rdx,<xp0=%r11
add  %rdx,%r11

# qhasm:   carry? xp1 += mulzero + carry
# asm 1: adc <mulzero=int64#2,<xp1=int64#10
# asm 2: adc <mulzero=%rsi,<xp1=%r12
adc %rsi,%r12

# qhasm:   carry? xp2 += mulzero + carry
# asm 1: adc <mulzero=int64#2,<xp2=int64#11
# asm 2: adc <mulzero=%rsi,<xp2=%r13
adc %rsi,%r13

# qhasm:   carry? xp3 += mulzero + carry
# asm 1: adc <mulzero=int64#2,<xp3=int64#12
# asm 2: adc <mulzero=%rsi,<xp3=%r14
adc %rsi,%r14

# qhasm:   mulzero += mulzero + carry
# asm 1: adc <mulzero=int64#2,<mulzero=int64#2
# asm 2: adc <mulzero=%rsi,<mulzero=%rsi
adc %rsi,%rsi

# qhasm:   mulzero *= 38
# asm 1: imulq  $38,<mulzero=int64#2,>mulzero=int64#2
# asm 2: imulq  $38,<mulzero=%rsi,>mulzero=%rsi
imulq  $38,%rsi,%rsi

# qhasm:   xp0 += mulzero
# asm 1: add  <mulzero=int64#2,<xp0=int64#9
# asm 2: add  <mulzero=%rsi,<xp0=%r11
add  %rsi,%r11

# qhasm: *(uint64 *)(workp + 32) = xp0
# asm 1: movq   <xp0=int64#9,32(<workp=int64#1)
# asm 2: movq   <xp0=%r11,32(<workp=%rdi)
movq   %r11,32(%rdi)

# qhasm: *(uint64 *)(workp + 40) = xp1
# asm 1: movq   <xp1=int64#10,40(<workp=int64#1)
# asm 2: movq   <xp1=%r12,40(<workp=%rdi)
movq   %r12,40(%rdi)

# qhasm: *(uint64 *)(workp + 48) = xp2
# asm 1: movq   <xp2=int64#11,48(<workp=int64#1)
# asm 2: movq   <xp2=%r13,48(<workp=%rdi)
movq   %r13,48(%rdi)

# qhasm: *(uint64 *)(workp + 56) = xp3
# asm 1: movq   <xp3=int64#12,56(<workp=int64#1)
# asm 2: movq   <xp3=%r14,56(<workp=%rdi)
movq   %r14,56(%rdi)

# qhasm:   mul121666rax = t50_stack
# asm 1: movq <t50_stack=stack64#24,>mul121666rax=int64#7
# asm 2: movq <t50_stack=184(%rsp),>mul121666rax=%rax
movq 184(%rsp),%rax

# qhasm:   (uint128) mul121666rdx mul121666rax = mul121666rax * *(uint64 *)&crypto_scalarmult_curve25519_amd64_64_121666
mulq  crypto_scalarmult_curve25519_amd64_64_121666

# qhasm:   zp0 = mul121666rax
# asm 1: mov  <mul121666rax=int64#7,>zp0=int64#2
# asm 2: mov  <mul121666rax=%rax,>zp0=%rsi
mov  %rax,%rsi

# qhasm:   zp1 = mul121666rdx
# asm 1: mov  <mul121666rdx=int64#3,>zp1=int64#4
# asm 2: mov  <mul121666rdx=%rdx,>zp1=%rcx
mov  %rdx,%rcx

# qhasm:   mul121666rax = t52_stack
# asm 1: movq <t52_stack=stack64#26,>mul121666rax=int64#7
# asm 2: movq <t52_stack=200(%rsp),>mul121666rax=%rax
movq 200(%rsp),%rax

# qhasm:   (uint128) mul121666rdx mul121666rax = mul121666rax * *(uint64 *)&crypto_scalarmult_curve25519_amd64_64_121666
mulq  crypto_scalarmult_curve25519_amd64_64_121666

# qhasm:   zp2 = mul121666rax
# asm 1: mov  <mul121666rax=int64#7,>zp2=int64#5
# asm 2: mov  <mul121666rax=%rax,>zp2=%r8
mov  %rax,%r8

# qhasm:   zp3 = mul121666rdx
# asm 1: mov  <mul121666rdx=int64#3,>zp3=int64#6
# asm 2: mov  <mul121666rdx=%rdx,>zp3=%r9
mov  %rdx,%r9

# qhasm:   mul121666rax = t51_stack
# asm 1: movq <t51_stack=stack64#25,>mul121666rax=int64#7
# asm 2: movq <t51_stack=192(%rsp),>mul121666rax=%rax
movq 192(%rsp),%rax

# qhasm:   (uint128) mul121666rdx mul121666rax = mul121666rax * *(uint64 *)&crypto_scalarmult_curve25519_amd64_64_121666
mulq  crypto_scalarmult_curve25519_amd64_64_121666

# qhasm:   mul121666t1 = mul121666rax
# asm 1: mov  <mul121666rax=int64#7,>mul121666t1=int64#8
# asm 2: mov  <mul121666rax=%rax,>mul121666t1=%r10
mov  %rax,%r10

# qhasm:   mul121666t2 = mul121666rdx
# asm 1: mov  <mul121666rdx=int64#3,>mul121666t2=int64#9
# asm 2: mov  <mul121666rdx=%rdx,>mul121666t2=%r11
mov  %rdx,%r11

# qhasm:   mul121666rax = t53_stack
# asm 1: movq <t53_stack=stack64#27,>mul121666rax=int64#7
# asm 2: movq <t53_stack=208(%rsp),>mul121666rax=%rax
movq 208(%rsp),%rax

# qhasm:   (uint128) mul121666rdx mul121666rax = mul121666rax * *(uint64 *)&crypto_scalarmult_curve25519_amd64_64_121666
mulq  crypto_scalarmult_curve25519_amd64_64_121666

# qhasm:   mul121666t3 = mul121666rax
# asm 1: mov  <mul121666rax=int64#7,>mul121666t3=int64#7
# asm 2: mov  <mul121666rax=%rax,>mul121666t3=%rax
mov  %rax,%rax

# qhasm:   mul121666r4 = mul121666rdx
# asm 1: mov  <mul121666rdx=int64#3,>mul121666r4=int64#3
# asm 2: mov  <mul121666rdx=%rdx,>mul121666r4=%rdx
mov  %rdx,%rdx

# qhasm:   carry? zp1 += mul121666t1
# asm 1: add  <mul121666t1=int64#8,<zp1=int64#4
# asm 2: add  <mul121666t1=%r10,<zp1=%rcx
add  %r10,%rcx

# qhasm:   carry? zp2 += mul121666t2 + carry
# asm 1: adc <mul121666t2=int64#9,<zp2=int64#5
# asm 2: adc <mul121666t2=%r11,<zp2=%r8
adc %r11,%r8

# qhasm:   carry? zp3 += mul121666t3 + carry
# asm 1: adc <mul121666t3=int64#7,<zp3=int64#6
# asm 2: adc <mul121666t3=%rax,<zp3=%r9
adc %rax,%r9

# qhasm:   mul121666r4 += 0 + carry
# asm 1: adc $0,<mul121666r4=int64#3
# asm 2: adc $0,<mul121666r4=%rdx
adc $0,%rdx

# qhasm:   mul121666r4 *= 38
# asm 1: imulq  $38,<mul121666r4=int64#3,>mul121666r4=int64#3
# asm 2: imulq  $38,<mul121666r4=%rdx,>mul121666r4=%rdx
imulq  $38,%rdx,%rdx

# qhasm:   carry? zp0 += mul121666r4
# asm 1: add  <mul121666r4=int64#3,<zp0=int64#2
# asm 2: add  <mul121666r4=%rdx,<zp0=%rsi
add  %rdx,%rsi

# qhasm:   carry? zp1 += 0 + carry
# asm 1: adc $0,<zp1=int64#4
# asm 2: adc $0,<zp1=%rcx
adc $0,%rcx

# qhasm:   carry? zp2 += 0 + carry
# asm 1: adc $0,<zp2=int64#5
# asm 2: adc $0,<zp2=%r8
adc $0,%r8

# qhasm:   carry? zp3 += 0 + carry
# asm 1: adc $0,<zp3=int64#6
# asm 2: adc $0,<zp3=%r9
adc $0,%r9

# qhasm:   mul121666t1 = 38
# asm 1: mov  $38,>mul121666t1=int64#3
# asm 2: mov  $38,>mul121666t1=%rdx
mov  $38,%rdx

# qhasm:   mul121666t2 = 0
# asm 1: mov  $0,>mul121666t2=int64#7
# asm 2: mov  $0,>mul121666t2=%rax
mov  $0,%rax

# qhasm:   mul121666t1 = mul121666t2 if !carry
# asm 1: cmovae <mul121666t2=int64#7,<mul121666t1=int64#3
# asm 2: cmovae <mul121666t2=%rax,<mul121666t1=%rdx
cmovae %rax,%rdx

# qhasm:   zp0 += mul121666t1
# asm 1: add  <mul121666t1=int64#3,<zp0=int64#2
# asm 2: add  <mul121666t1=%rdx,<zp0=%rsi
add  %rdx,%rsi

# qhasm:   carry? zp0 += t70_stack 
# asm 1: addq <t70_stack=stack64#16,<zp0=int64#2
# asm 2: addq <t70_stack=120(%rsp),<zp0=%rsi
addq 120(%rsp),%rsi

# qhasm:   carry? zp1 += t71_stack + carry
# asm 1: adcq <t71_stack=stack64#17,<zp1=int64#4
# asm 2: adcq <t71_stack=128(%rsp),<zp1=%rcx
adcq 128(%rsp),%rcx

# qhasm:   carry? zp2 += t72_stack + carry
# asm 1: adcq <t72_stack=stack64#18,<zp2=int64#5
# asm 2: adcq <t72_stack=136(%rsp),<zp2=%r8
adcq 136(%rsp),%r8

# qhasm:   carry? zp3 += t73_stack + carry
# asm 1: adcq <t73_stack=stack64#19,<zp3=int64#6
# asm 2: adcq <t73_stack=144(%rsp),<zp3=%r9
adcq 144(%rsp),%r9

# qhasm:   addt0 = 0
# asm 1: mov  $0,>addt0=int64#3
# asm 2: mov  $0,>addt0=%rdx
mov  $0,%rdx

# qhasm:   addt1 = 38
# asm 1: mov  $38,>addt1=int64#7
# asm 2: mov  $38,>addt1=%rax
mov  $38,%rax

# qhasm:   addt1 = addt0 if !carry
# asm 1: cmovae <addt0=int64#3,<addt1=int64#7
# asm 2: cmovae <addt0=%rdx,<addt1=%rax
cmovae %rdx,%rax

# qhasm:   carry? zp0 += addt1
# asm 1: add  <addt1=int64#7,<zp0=int64#2
# asm 2: add  <addt1=%rax,<zp0=%rsi
add  %rax,%rsi

# qhasm:   carry? zp1 += addt0 + carry
# asm 1: adc <addt0=int64#3,<zp1=int64#4
# asm 2: adc <addt0=%rdx,<zp1=%rcx
adc %rdx,%rcx

# qhasm:   carry? zp2 += addt0 + carry
# asm 1: adc <addt0=int64#3,<zp2=int64#5
# asm 2: adc <addt0=%rdx,<zp2=%r8
adc %rdx,%r8

# qhasm:   carry? zp3 += addt0 + carry
# asm 1: adc <addt0=int64#3,<zp3=int64#6
# asm 2: adc <addt0=%rdx,<zp3=%r9
adc %rdx,%r9

# qhasm:   addt0 = addt1 if carry
# asm 1: cmovc <addt1=int64#7,<addt0=int64#3
# asm 2: cmovc <addt1=%rax,<addt0=%rdx
cmovc %rax,%rdx

# qhasm:   zp0 += addt0
# asm 1: add  <addt0=int64#3,<zp0=int64#2
# asm 2: add  <addt0=%rdx,<zp0=%rsi
add  %rdx,%rsi

# qhasm: *(uint64 *)(workp + 64) = zp0
# asm 1: movq   <zp0=int64#2,64(<workp=int64#1)
# asm 2: movq   <zp0=%rsi,64(<workp=%rdi)
movq   %rsi,64(%rdi)

# qhasm: *(uint64 *)(workp + 72) = zp1
# asm 1: movq   <zp1=int64#4,72(<workp=int64#1)
# asm 2: movq   <zp1=%rcx,72(<workp=%rdi)
movq   %rcx,72(%rdi)

# qhasm: *(uint64 *)(workp + 80) = zp2
# asm 1: movq   <zp2=int64#5,80(<workp=int64#1)
# asm 2: movq   <zp2=%r8,80(<workp=%rdi)
movq   %r8,80(%rdi)

# qhasm: *(uint64 *)(workp + 88) = zp3
# asm 1: movq   <zp3=int64#6,88(<workp=int64#1)
# asm 2: movq   <zp3=%r9,88(<workp=%rdi)
movq   %r9,88(%rdi)

# qhasm:   mulr4 = 0
# asm 1: mov  $0,>mulr4=int64#2
# asm 2: mov  $0,>mulr4=%rsi
mov  $0,%rsi

# qhasm:   mulr5 = 0
# asm 1: mov  $0,>mulr5=int64#4
# asm 2: mov  $0,>mulr5=%rcx
mov  $0,%rcx

# qhasm:   mulr6 = 0
# asm 1: mov  $0,>mulr6=int64#5
# asm 2: mov  $0,>mulr6=%r8
mov  $0,%r8

# qhasm:   mulr7 = 0
# asm 1: mov  $0,>mulr7=int64#6
# asm 2: mov  $0,>mulr7=%r9
mov  $0,%r9

# qhasm:   mulx0 = *(uint64 *)(workp + 64)
# asm 1: movq   64(<workp=int64#1),>mulx0=int64#8
# asm 2: movq   64(<workp=%rdi),>mulx0=%r10
movq   64(%rdi),%r10

# qhasm:   mulrax = t50_stack
# asm 1: movq <t50_stack=stack64#24,>mulrax=int64#7
# asm 2: movq <t50_stack=184(%rsp),>mulrax=%rax
movq 184(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * mulx0
# asm 1: mul  <mulx0=int64#8
# asm 2: mul  <mulx0=%r10
mul  %r10

# qhasm:   zp0 = mulrax
# asm 1: mov  <mulrax=int64#7,>zp0=int64#9
# asm 2: mov  <mulrax=%rax,>zp0=%r11
mov  %rax,%r11

# qhasm:   zp1 = mulrdx
# asm 1: mov  <mulrdx=int64#3,>zp1=int64#10
# asm 2: mov  <mulrdx=%rdx,>zp1=%r12
mov  %rdx,%r12

# qhasm:   mulrax = t51_stack
# asm 1: movq <t51_stack=stack64#25,>mulrax=int64#7
# asm 2: movq <t51_stack=192(%rsp),>mulrax=%rax
movq 192(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * mulx0
# asm 1: mul  <mulx0=int64#8
# asm 2: mul  <mulx0=%r10
mul  %r10

# qhasm:   carry? zp1 += mulrax
# asm 1: add  <mulrax=int64#7,<zp1=int64#10
# asm 2: add  <mulrax=%rax,<zp1=%r12
add  %rax,%r12

# qhasm:   zp2 = 0
# asm 1: mov  $0,>zp2=int64#11
# asm 2: mov  $0,>zp2=%r13
mov  $0,%r13

# qhasm:   zp2 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<zp2=int64#11
# asm 2: adc <mulrdx=%rdx,<zp2=%r13
adc %rdx,%r13

# qhasm:   mulrax = t52_stack
# asm 1: movq <t52_stack=stack64#26,>mulrax=int64#7
# asm 2: movq <t52_stack=200(%rsp),>mulrax=%rax
movq 200(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * mulx0
# asm 1: mul  <mulx0=int64#8
# asm 2: mul  <mulx0=%r10
mul  %r10

# qhasm:   carry? zp2 += mulrax
# asm 1: add  <mulrax=int64#7,<zp2=int64#11
# asm 2: add  <mulrax=%rax,<zp2=%r13
add  %rax,%r13

# qhasm:   zp3 = 0
# asm 1: mov  $0,>zp3=int64#12
# asm 2: mov  $0,>zp3=%r14
mov  $0,%r14

# qhasm:   zp3 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<zp3=int64#12
# asm 2: adc <mulrdx=%rdx,<zp3=%r14
adc %rdx,%r14

# qhasm:   mulrax = t53_stack
# asm 1: movq <t53_stack=stack64#27,>mulrax=int64#7
# asm 2: movq <t53_stack=208(%rsp),>mulrax=%rax
movq 208(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * mulx0
# asm 1: mul  <mulx0=int64#8
# asm 2: mul  <mulx0=%r10
mul  %r10

# qhasm:   carry? zp3 += mulrax
# asm 1: add  <mulrax=int64#7,<zp3=int64#12
# asm 2: add  <mulrax=%rax,<zp3=%r14
add  %rax,%r14

# qhasm:   mulr4 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr4=int64#2
# asm 2: adc <mulrdx=%rdx,<mulr4=%rsi
adc %rdx,%rsi

# qhasm:   mulx1 = *(uint64 *)(workp + 72)
# asm 1: movq   72(<workp=int64#1),>mulx1=int64#8
# asm 2: movq   72(<workp=%rdi),>mulx1=%r10
movq   72(%rdi),%r10

# qhasm:   mulrax = t50_stack
# asm 1: movq <t50_stack=stack64#24,>mulrax=int64#7
# asm 2: movq <t50_stack=184(%rsp),>mulrax=%rax
movq 184(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * mulx1
# asm 1: mul  <mulx1=int64#8
# asm 2: mul  <mulx1=%r10
mul  %r10

# qhasm:   carry? zp1 += mulrax
# asm 1: add  <mulrax=int64#7,<zp1=int64#10
# asm 2: add  <mulrax=%rax,<zp1=%r12
add  %rax,%r12

# qhasm:   mulc = 0
# asm 1: mov  $0,>mulc=int64#13
# asm 2: mov  $0,>mulc=%r15
mov  $0,%r15

# qhasm:   mulc += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulc=int64#13
# asm 2: adc <mulrdx=%rdx,<mulc=%r15
adc %rdx,%r15

# qhasm:   mulrax = t51_stack
# asm 1: movq <t51_stack=stack64#25,>mulrax=int64#7
# asm 2: movq <t51_stack=192(%rsp),>mulrax=%rax
movq 192(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * mulx1
# asm 1: mul  <mulx1=int64#8
# asm 2: mul  <mulx1=%r10
mul  %r10

# qhasm:   carry? zp2 += mulrax
# asm 1: add  <mulrax=int64#7,<zp2=int64#11
# asm 2: add  <mulrax=%rax,<zp2=%r13
add  %rax,%r13

# qhasm:   mulrdx += 0 + carry
# asm 1: adc $0,<mulrdx=int64#3
# asm 2: adc $0,<mulrdx=%rdx
adc $0,%rdx

# qhasm:   carry? zp2 += mulc
# asm 1: add  <mulc=int64#13,<zp2=int64#11
# asm 2: add  <mulc=%r15,<zp2=%r13
add  %r15,%r13

# qhasm:   mulc = 0
# asm 1: mov  $0,>mulc=int64#13
# asm 2: mov  $0,>mulc=%r15
mov  $0,%r15

# qhasm:   mulc += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulc=int64#13
# asm 2: adc <mulrdx=%rdx,<mulc=%r15
adc %rdx,%r15

# qhasm:   mulrax = t52_stack
# asm 1: movq <t52_stack=stack64#26,>mulrax=int64#7
# asm 2: movq <t52_stack=200(%rsp),>mulrax=%rax
movq 200(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * mulx1
# asm 1: mul  <mulx1=int64#8
# asm 2: mul  <mulx1=%r10
mul  %r10

# qhasm:   carry? zp3 += mulrax 
# asm 1: add  <mulrax=int64#7,<zp3=int64#12
# asm 2: add  <mulrax=%rax,<zp3=%r14
add  %rax,%r14

# qhasm:   mulrdx += 0 + carry
# asm 1: adc $0,<mulrdx=int64#3
# asm 2: adc $0,<mulrdx=%rdx
adc $0,%rdx

# qhasm:   carry? zp3 += mulc
# asm 1: add  <mulc=int64#13,<zp3=int64#12
# asm 2: add  <mulc=%r15,<zp3=%r14
add  %r15,%r14

# qhasm:   mulc = 0
# asm 1: mov  $0,>mulc=int64#13
# asm 2: mov  $0,>mulc=%r15
mov  $0,%r15

# qhasm:   mulc += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulc=int64#13
# asm 2: adc <mulrdx=%rdx,<mulc=%r15
adc %rdx,%r15

# qhasm:   mulrax = t53_stack
# asm 1: movq <t53_stack=stack64#27,>mulrax=int64#7
# asm 2: movq <t53_stack=208(%rsp),>mulrax=%rax
movq 208(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * mulx1
# asm 1: mul  <mulx1=int64#8
# asm 2: mul  <mulx1=%r10
mul  %r10

# qhasm:   carry? mulr4 += mulrax
# asm 1: add  <mulrax=int64#7,<mulr4=int64#2
# asm 2: add  <mulrax=%rax,<mulr4=%rsi
add  %rax,%rsi

# qhasm:   mulrdx += 0 + carry
# asm 1: adc $0,<mulrdx=int64#3
# asm 2: adc $0,<mulrdx=%rdx
adc $0,%rdx

# qhasm:   carry? mulr4 += mulc
# asm 1: add  <mulc=int64#13,<mulr4=int64#2
# asm 2: add  <mulc=%r15,<mulr4=%rsi
add  %r15,%rsi

# qhasm:   mulr5 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr5=int64#4
# asm 2: adc <mulrdx=%rdx,<mulr5=%rcx
adc %rdx,%rcx

# qhasm:   mulx2 = *(uint64 *)(workp + 80)
# asm 1: movq   80(<workp=int64#1),>mulx2=int64#8
# asm 2: movq   80(<workp=%rdi),>mulx2=%r10
movq   80(%rdi),%r10

# qhasm:   mulrax = t50_stack
# asm 1: movq <t50_stack=stack64#24,>mulrax=int64#7
# asm 2: movq <t50_stack=184(%rsp),>mulrax=%rax
movq 184(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * mulx2
# asm 1: mul  <mulx2=int64#8
# asm 2: mul  <mulx2=%r10
mul  %r10

# qhasm:   carry? zp2 += mulrax
# asm 1: add  <mulrax=int64#7,<zp2=int64#11
# asm 2: add  <mulrax=%rax,<zp2=%r13
add  %rax,%r13

# qhasm:   mulc = 0
# asm 1: mov  $0,>mulc=int64#13
# asm 2: mov  $0,>mulc=%r15
mov  $0,%r15

# qhasm:   mulc += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulc=int64#13
# asm 2: adc <mulrdx=%rdx,<mulc=%r15
adc %rdx,%r15

# qhasm:   mulrax = t51_stack
# asm 1: movq <t51_stack=stack64#25,>mulrax=int64#7
# asm 2: movq <t51_stack=192(%rsp),>mulrax=%rax
movq 192(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * mulx2
# asm 1: mul  <mulx2=int64#8
# asm 2: mul  <mulx2=%r10
mul  %r10

# qhasm:   carry? zp3 += mulrax
# asm 1: add  <mulrax=int64#7,<zp3=int64#12
# asm 2: add  <mulrax=%rax,<zp3=%r14
add  %rax,%r14

# qhasm:   mulrdx += 0 + carry
# asm 1: adc $0,<mulrdx=int64#3
# asm 2: adc $0,<mulrdx=%rdx
adc $0,%rdx

# qhasm:   carry? zp3 += mulc
# asm 1: add  <mulc=int64#13,<zp3=int64#12
# asm 2: add  <mulc=%r15,<zp3=%r14
add  %r15,%r14

# qhasm:   mulc = 0
# asm 1: mov  $0,>mulc=int64#13
# asm 2: mov  $0,>mulc=%r15
mov  $0,%r15

# qhasm:   mulc += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulc=int64#13
# asm 2: adc <mulrdx=%rdx,<mulc=%r15
adc %rdx,%r15

# qhasm:   mulrax = t52_stack
# asm 1: movq <t52_stack=stack64#26,>mulrax=int64#7
# asm 2: movq <t52_stack=200(%rsp),>mulrax=%rax
movq 200(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * mulx2
# asm 1: mul  <mulx2=int64#8
# asm 2: mul  <mulx2=%r10
mul  %r10

# qhasm:   carry? mulr4 += mulrax
# asm 1: add  <mulrax=int64#7,<mulr4=int64#2
# asm 2: add  <mulrax=%rax,<mulr4=%rsi
add  %rax,%rsi

# qhasm:   mulrdx += 0 + carry
# asm 1: adc $0,<mulrdx=int64#3
# asm 2: adc $0,<mulrdx=%rdx
adc $0,%rdx

# qhasm:   carry? mulr4 += mulc
# asm 1: add  <mulc=int64#13,<mulr4=int64#2
# asm 2: add  <mulc=%r15,<mulr4=%rsi
add  %r15,%rsi

# qhasm:   mulc = 0
# asm 1: mov  $0,>mulc=int64#13
# asm 2: mov  $0,>mulc=%r15
mov  $0,%r15

# qhasm:   mulc += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulc=int64#13
# asm 2: adc <mulrdx=%rdx,<mulc=%r15
adc %rdx,%r15

# qhasm:   mulrax = t53_stack
# asm 1: movq <t53_stack=stack64#27,>mulrax=int64#7
# asm 2: movq <t53_stack=208(%rsp),>mulrax=%rax
movq 208(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * mulx2
# asm 1: mul  <mulx2=int64#8
# asm 2: mul  <mulx2=%r10
mul  %r10

# qhasm:   carry? mulr5 += mulrax
# asm 1: add  <mulrax=int64#7,<mulr5=int64#4
# asm 2: add  <mulrax=%rax,<mulr5=%rcx
add  %rax,%rcx

# qhasm:   mulrdx += 0 + carry
# asm 1: adc $0,<mulrdx=int64#3
# asm 2: adc $0,<mulrdx=%rdx
adc $0,%rdx

# qhasm:   carry? mulr5 += mulc
# asm 1: add  <mulc=int64#13,<mulr5=int64#4
# asm 2: add  <mulc=%r15,<mulr5=%rcx
add  %r15,%rcx

# qhasm:   mulr6 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr6=int64#5
# asm 2: adc <mulrdx=%rdx,<mulr6=%r8
adc %rdx,%r8

# qhasm:   mulx3 = *(uint64 *)(workp + 88)
# asm 1: movq   88(<workp=int64#1),>mulx3=int64#8
# asm 2: movq   88(<workp=%rdi),>mulx3=%r10
movq   88(%rdi),%r10

# qhasm:   mulrax = t50_stack
# asm 1: movq <t50_stack=stack64#24,>mulrax=int64#7
# asm 2: movq <t50_stack=184(%rsp),>mulrax=%rax
movq 184(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * mulx3
# asm 1: mul  <mulx3=int64#8
# asm 2: mul  <mulx3=%r10
mul  %r10

# qhasm:   carry? zp3 += mulrax
# asm 1: add  <mulrax=int64#7,<zp3=int64#12
# asm 2: add  <mulrax=%rax,<zp3=%r14
add  %rax,%r14

# qhasm:   mulc = 0
# asm 1: mov  $0,>mulc=int64#13
# asm 2: mov  $0,>mulc=%r15
mov  $0,%r15

# qhasm:   mulc += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulc=int64#13
# asm 2: adc <mulrdx=%rdx,<mulc=%r15
adc %rdx,%r15

# qhasm:   mulrax = t51_stack
# asm 1: movq <t51_stack=stack64#25,>mulrax=int64#7
# asm 2: movq <t51_stack=192(%rsp),>mulrax=%rax
movq 192(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * mulx3
# asm 1: mul  <mulx3=int64#8
# asm 2: mul  <mulx3=%r10
mul  %r10

# qhasm:   carry? mulr4 += mulrax
# asm 1: add  <mulrax=int64#7,<mulr4=int64#2
# asm 2: add  <mulrax=%rax,<mulr4=%rsi
add  %rax,%rsi

# qhasm:   mulrdx += 0 + carry
# asm 1: adc $0,<mulrdx=int64#3
# asm 2: adc $0,<mulrdx=%rdx
adc $0,%rdx

# qhasm:   carry? mulr4 += mulc
# asm 1: add  <mulc=int64#13,<mulr4=int64#2
# asm 2: add  <mulc=%r15,<mulr4=%rsi
add  %r15,%rsi

# qhasm:   mulc = 0
# asm 1: mov  $0,>mulc=int64#13
# asm 2: mov  $0,>mulc=%r15
mov  $0,%r15

# qhasm:   mulc += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulc=int64#13
# asm 2: adc <mulrdx=%rdx,<mulc=%r15
adc %rdx,%r15

# qhasm:   mulrax = t52_stack
# asm 1: movq <t52_stack=stack64#26,>mulrax=int64#7
# asm 2: movq <t52_stack=200(%rsp),>mulrax=%rax
movq 200(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * mulx3
# asm 1: mul  <mulx3=int64#8
# asm 2: mul  <mulx3=%r10
mul  %r10

# qhasm:   carry? mulr5 += mulrax
# asm 1: add  <mulrax=int64#7,<mulr5=int64#4
# asm 2: add  <mulrax=%rax,<mulr5=%rcx
add  %rax,%rcx

# qhasm:   mulrdx += 0 + carry
# asm 1: adc $0,<mulrdx=int64#3
# asm 2: adc $0,<mulrdx=%rdx
adc $0,%rdx

# qhasm:   carry? mulr5 += mulc
# asm 1: add  <mulc=int64#13,<mulr5=int64#4
# asm 2: add  <mulc=%r15,<mulr5=%rcx
add  %r15,%rcx

# qhasm:   mulc = 0
# asm 1: mov  $0,>mulc=int64#13
# asm 2: mov  $0,>mulc=%r15
mov  $0,%r15

# qhasm:   mulc += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulc=int64#13
# asm 2: adc <mulrdx=%rdx,<mulc=%r15
adc %rdx,%r15

# qhasm:   mulrax = t53_stack
# asm 1: movq <t53_stack=stack64#27,>mulrax=int64#7
# asm 2: movq <t53_stack=208(%rsp),>mulrax=%rax
movq 208(%rsp),%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * mulx3
# asm 1: mul  <mulx3=int64#8
# asm 2: mul  <mulx3=%r10
mul  %r10

# qhasm:   carry? mulr6 += mulrax
# asm 1: add  <mulrax=int64#7,<mulr6=int64#5
# asm 2: add  <mulrax=%rax,<mulr6=%r8
add  %rax,%r8

# qhasm:   mulrdx += 0 + carry
# asm 1: adc $0,<mulrdx=int64#3
# asm 2: adc $0,<mulrdx=%rdx
adc $0,%rdx

# qhasm:   carry? mulr6 += mulc
# asm 1: add  <mulc=int64#13,<mulr6=int64#5
# asm 2: add  <mulc=%r15,<mulr6=%r8
add  %r15,%r8

# qhasm:   mulr7 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr7=int64#6
# asm 2: adc <mulrdx=%rdx,<mulr7=%r9
adc %rdx,%r9

# qhasm:   mulrax = mulr4
# asm 1: mov  <mulr4=int64#2,>mulrax=int64#7
# asm 2: mov  <mulr4=%rsi,>mulrax=%rax
mov  %rsi,%rax

# qhasm:   (uint128) mulrdx mulrax = mulrax * *(uint64 *)&crypto_scalarmult_curve25519_amd64_64_38
mulq  crypto_scalarmult_curve25519_amd64_64_38

# qhasm:   mulr4 = mulrax
# asm 1: mov  <mulrax=int64#7,>mulr4=int64#2
# asm 2: mov  <mulrax=%rax,>mulr4=%rsi
mov  %rax,%rsi

# qhasm:   mulrax = mulr5
# asm 1: mov  <mulr5=int64#4,>mulrax=int64#7
# asm 2: mov  <mulr5=%rcx,>mulrax=%rax
mov  %rcx,%rax

# qhasm:   mulr5 = mulrdx
# asm 1: mov  <mulrdx=int64#3,>mulr5=int64#4
# asm 2: mov  <mulrdx=%rdx,>mulr5=%rcx
mov  %rdx,%rcx

# qhasm:   (uint128) mulrdx mulrax = mulrax * *(uint64 *)&crypto_scalarmult_curve25519_amd64_64_38
mulq  crypto_scalarmult_curve25519_amd64_64_38

# qhasm:   carry? mulr5 += mulrax
# asm 1: add  <mulrax=int64#7,<mulr5=int64#4
# asm 2: add  <mulrax=%rax,<mulr5=%rcx
add  %rax,%rcx

# qhasm:   mulrax = mulr6
# asm 1: mov  <mulr6=int64#5,>mulrax=int64#7
# asm 2: mov  <mulr6=%r8,>mulrax=%rax
mov  %r8,%rax

# qhasm:   mulr6 = 0
# asm 1: mov  $0,>mulr6=int64#5
# asm 2: mov  $0,>mulr6=%r8
mov  $0,%r8

# qhasm:   mulr6 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr6=int64#5
# asm 2: adc <mulrdx=%rdx,<mulr6=%r8
adc %rdx,%r8

# qhasm:   (uint128) mulrdx mulrax = mulrax * *(uint64 *)&crypto_scalarmult_curve25519_amd64_64_38
mulq  crypto_scalarmult_curve25519_amd64_64_38

# qhasm:   carry? mulr6 += mulrax
# asm 1: add  <mulrax=int64#7,<mulr6=int64#5
# asm 2: add  <mulrax=%rax,<mulr6=%r8
add  %rax,%r8

# qhasm:   mulrax = mulr7
# asm 1: mov  <mulr7=int64#6,>mulrax=int64#7
# asm 2: mov  <mulr7=%r9,>mulrax=%rax
mov  %r9,%rax

# qhasm:   mulr7 = 0
# asm 1: mov  $0,>mulr7=int64#6
# asm 2: mov  $0,>mulr7=%r9
mov  $0,%r9

# qhasm:   mulr7 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr7=int64#6
# asm 2: adc <mulrdx=%rdx,<mulr7=%r9
adc %rdx,%r9

# qhasm:   (uint128) mulrdx mulrax = mulrax * *(uint64 *)&crypto_scalarmult_curve25519_amd64_64_38
mulq  crypto_scalarmult_curve25519_amd64_64_38

# qhasm:   carry? mulr7 += mulrax
# asm 1: add  <mulrax=int64#7,<mulr7=int64#6
# asm 2: add  <mulrax=%rax,<mulr7=%r9
add  %rax,%r9

# qhasm:   mulr8 = 0
# asm 1: mov  $0,>mulr8=int64#7
# asm 2: mov  $0,>mulr8=%rax
mov  $0,%rax

# qhasm:   mulr8 += mulrdx + carry
# asm 1: adc <mulrdx=int64#3,<mulr8=int64#7
# asm 2: adc <mulrdx=%rdx,<mulr8=%rax
adc %rdx,%rax

# qhasm:   carry? zp0 += mulr4
# asm 1: add  <mulr4=int64#2,<zp0=int64#9
# asm 2: add  <mulr4=%rsi,<zp0=%r11
add  %rsi,%r11

# qhasm:   carry? zp1 += mulr5 + carry
# asm 1: adc <mulr5=int64#4,<zp1=int64#10
# asm 2: adc <mulr5=%rcx,<zp1=%r12
adc %rcx,%r12

# qhasm:   carry? zp2 += mulr6 + carry
# asm 1: adc <mulr6=int64#5,<zp2=int64#11
# asm 2: adc <mulr6=%r8,<zp2=%r13
adc %r8,%r13

# qhasm:   carry? zp3 += mulr7 + carry
# asm 1: adc <mulr7=int64#6,<zp3=int64#12
# asm 2: adc <mulr7=%r9,<zp3=%r14
adc %r9,%r14

# qhasm:   mulzero = 0
# asm 1: mov  $0,>mulzero=int64#2
# asm 2: mov  $0,>mulzero=%rsi
mov  $0,%rsi

# qhasm:   mulr8 += mulzero + carry
# asm 1: adc <mulzero=int64#2,<mulr8=int64#7
# asm 2: adc <mulzero=%rsi,<mulr8=%rax
adc %rsi,%rax

# qhasm:   mulr8 *= 38
# asm 1: imulq  $38,<mulr8=int64#7,>mulr8=int64#3
# asm 2: imulq  $38,<mulr8=%rax,>mulr8=%rdx
imulq  $38,%rax,%rdx

# qhasm:   carry? zp0 += mulr8
# asm 1: add  <mulr8=int64#3,<zp0=int64#9
# asm 2: add  <mulr8=%rdx,<zp0=%r11
add  %rdx,%r11

# qhasm:   carry? zp1 += mulzero + carry
# asm 1: adc <mulzero=int64#2,<zp1=int64#10
# asm 2: adc <mulzero=%rsi,<zp1=%r12
adc %rsi,%r12

# qhasm:   carry? zp2 += mulzero + carry
# asm 1: adc <mulzero=int64#2,<zp2=int64#11
# asm 2: adc <mulzero=%rsi,<zp2=%r13
adc %rsi,%r13

# qhasm:   carry? zp3 += mulzero + carry
# asm 1: adc <mulzero=int64#2,<zp3=int64#12
# asm 2: adc <mulzero=%rsi,<zp3=%r14
adc %rsi,%r14

# qhasm:   mulzero += mulzero + carry
# asm 1: adc <mulzero=int64#2,<mulzero=int64#2
# asm 2: adc <mulzero=%rsi,<mulzero=%rsi
adc %rsi,%rsi

# qhasm:   mulzero *= 38
# asm 1: imulq  $38,<mulzero=int64#2,>mulzero=int64#2
# asm 2: imulq  $38,<mulzero=%rsi,>mulzero=%rsi
imulq  $38,%rsi,%rsi

# qhasm:   zp0 += mulzero
# asm 1: add  <mulzero=int64#2,<zp0=int64#9
# asm 2: add  <mulzero=%rsi,<zp0=%r11
add  %rsi,%r11

# qhasm: *(uint64 *)(workp + 64) = zp0
# asm 1: movq   <zp0=int64#9,64(<workp=int64#1)
# asm 2: movq   <zp0=%r11,64(<workp=%rdi)
movq   %r11,64(%rdi)

# qhasm: *(uint64 *)(workp + 72) = zp1
# asm 1: movq   <zp1=int64#10,72(<workp=int64#1)
# asm 2: movq   <zp1=%r12,72(<workp=%rdi)
movq   %r12,72(%rdi)

# qhasm: *(uint64 *)(workp + 80) = zp2
# asm 1: movq   <zp2=int64#11,80(<workp=int64#1)
# asm 2: movq   <zp2=%r13,80(<workp=%rdi)
movq   %r13,80(%rdi)

# qhasm: *(uint64 *)(workp + 88) = zp3
# asm 1: movq   <zp3=int64#12,88(<workp=int64#1)
# asm 2: movq   <zp3=%r14,88(<workp=%rdi)
movq   %r14,88(%rdi)

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
