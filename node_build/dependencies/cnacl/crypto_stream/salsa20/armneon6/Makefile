xor.s.pure: xor.s
	grep -v -w asm < xor.s | grep -v -w qhasm | grep . > xor.s.pure

xor.s: xor.q
	qhasm-arm-align < xor.q > xor.s
