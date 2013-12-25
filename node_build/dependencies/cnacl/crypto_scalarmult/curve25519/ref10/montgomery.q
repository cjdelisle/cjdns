:name:fe:x2:z2:x3:z3:tmp0:tmp1:
fe r:var/r=fe:

enter f:enter/f:>X2=fe#1:>Z2=fe#2:>X3=fe#3:>Z3=fe#4:
return:nofallthrough:<X4=fe#1:<Z4=fe#2:<X5=fe#3:<Z5=fe#4:leave:

h=f+g:<f=fe:<g=fe:>h=fe:asm/fe_add(>h,<f,<g);:
h=f-g:<f=fe:<g=fe:>h=fe:asm/fe_sub(>h,<f,<g);:
h=f*g:<f=fe:<g=fe:>h=fe:asm/fe_mul(>h,<f,<g);:
h=f^2:<f=fe:>h=fe:asm/fe_sq(>h,<f);:

h=X1*g:<g=fe:>h=fe:asm/fe_mul(>h,x1,<g);:
h=a24*g:<g=fe:>h=fe:asm/fe_mul121666(>h,<g);:

assign x2 to h:<h=fe#1:
assign z2 to h:<h=fe#2:
assign x3 to h:<h=fe#3:
assign z3 to h:<h=fe#4:

:

fe X2
fe Z2
fe X3
fe Z3
fe X4
fe Z4
fe X5
fe Z5
fe A
fe B
fe C
fe D
fe E
fe AA
fe BB
fe DA
fe CB
fe t0
fe t1
fe t2
fe t3
fe t4

enter ladder

# http://hyperelliptic.org/EFD/g1p/auto-code/montgom/xz/ladder/mladd-1987-m.op3
# reordered to fit into 2 temporaries

D = X3-Z3
B = X2-Z2
A = X2+Z2
C = X3+Z3
DA = D*A
CB = C*B
BB = B^2
AA = A^2
t0 = DA+CB
assign x3 to t0
t1 = DA-CB
X4 = AA*BB
E = AA-BB
t2 = t1^2
t3 = a24*E
X5 = t0^2
t4 = BB+t3
Z5 = X1*t2
Z4 = E*t4

# double-check against Theorem B.1 in Curve25519 paper:
# X4-(X2^2-Z2^2)^2
# Z4-4*X2*Z2*(X2^2+(4*a24-2)*X2*Z2+Z2^2)
# X5-4*(X2*X3-Z2*Z3)^2
# Z5-4*(X2*Z3-Z2*X3)^2*X1

return
