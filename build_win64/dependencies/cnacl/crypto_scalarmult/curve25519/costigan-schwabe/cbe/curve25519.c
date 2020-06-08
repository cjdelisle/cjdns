/*
version 20090331
Peter Schwabe & Neil Costigan
Public domain.
*/

#include "fromred.h"
#include "tored.h"
#include "invert.h"
#include "mul.h"
#include "mladder.h"

#include "crypto_scalarmult.h"

int crypto_scalarmult(unsigned char skp[32], 
		const unsigned char sk[32],
		const unsigned char p[32])
{
	unsigned char e[32];
	unsigned int i;

	for (i = 0;i < 32;++i) e[i] = sk[i];
	e[0] &= 248;
	e[31] &= 127;
	e[31] |= 64;

	vector unsigned int work[15];
	tored(work, p);
	mladder(work+5, e, work);
	invert(work+10, work+10);
	mul(work+5, work+5, work+10);
	fromred(skp, work+5);
	return 0;
}
