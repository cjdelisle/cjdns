/*
version 20090331
Peter Schwabe & Neil Costigan
Public domain.
*/

#include "mul.h"

void invert(vector unsigned int ret[5], const vector unsigned int in[5])
{
	vector unsigned int z2[5];
	vector unsigned int z9[5];
	vector unsigned int z11[5];
	vector unsigned int z2_5_0[5];
	vector unsigned int z2_10_0[5];
	vector unsigned int z2_20_0[5];
	vector unsigned int z2_50_0[5];
	vector unsigned int z2_100_0[5];
	vector unsigned int t0[5];
	vector unsigned int t1[5];
	int i;
	
	/* 2 */ square(z2,in);
	/* 4 */ square(t1,z2);
	/* 8 */ square(t0,t1);
	/* 9 */ mul(z9,t0,in);
	/* 11 */ mul(z11,z9,z2);
	/* 22 */ square(t0,z11);
	/* 2^5 - 2^0 = 31 */ mul(z2_5_0,t0,z9);

	/* 2^6 - 2^1 */ square(t0,z2_5_0);
	/* 2^7 - 2^2 */ square(t1,t0);
	/* 2^8 - 2^3 */ square(t0,t1);
	/* 2^9 - 2^4 */ square(t1,t0);
	/* 2^10 - 2^5 */ square(t0,t1);
	/* 2^10 - 2^0 */ mul(z2_10_0,t0,z2_5_0);

	/* 2^11 - 2^1 */ square(t0,z2_10_0);
	/* 2^12 - 2^2 */ square(t1,t0);
	/* 2^20 - 2^10 */ for (i = 2;i < 10;i += 2) { square(t0,t1); square(t1,t0); }
	/* 2^20 - 2^0 */ mul(z2_20_0,t1,z2_10_0);

	/* 2^21 - 2^1 */ square(t0,z2_20_0);
	/* 2^22 - 2^2 */ square(t1,t0);
	/* 2^40 - 2^20 */ for (i = 2;i < 20;i += 2) { square(t0,t1); square(t1,t0); }
	/* 2^40 - 2^0 */ mul(t0,t1,z2_20_0);

	/* 2^41 - 2^1 */ square(t1,t0);
	/* 2^42 - 2^2 */ square(t0,t1);
	/* 2^50 - 2^10 */ for (i = 2;i < 10;i += 2) { square(t1,t0); square(t0,t1); }
	/* 2^50 - 2^0 */ mul(z2_50_0,t0,z2_10_0);

	/* 2^51 - 2^1 */ square(t0,z2_50_0);
	/* 2^52 - 2^2 */ square(t1,t0);
	/* 2^100 - 2^50 */ for (i = 2;i < 50;i += 2) { square(t0,t1); square(t1,t0); }
	/* 2^100 - 2^0 */ mul(z2_100_0,t1,z2_50_0);

	/* 2^101 - 2^1 */ square(t1,z2_100_0);
	/* 2^102 - 2^2 */ square(t0,t1);
	/* 2^200 - 2^100 */ for (i = 2;i < 100;i += 2) { square(t1,t0); square(t0,t1); }
	/* 2^200 - 2^0 */ mul(t1,t0,z2_100_0);

	/* 2^201 - 2^1 */ square(t0,t1);
	/* 2^202 - 2^2 */ square(t1,t0);
	/* 2^250 - 2^50 */ for (i = 2;i < 50;i += 2) { square(t0,t1); square(t1,t0); }
	/* 2^250 - 2^0 */ mul(t0,t1,z2_50_0);

	/* 2^251 - 2^1 */ square(t1,t0);
	/* 2^252 - 2^2 */ square(t0,t1);
	/* 2^253 - 2^3 */ square(t1,t0);
	/* 2^254 - 2^4 */ square(t0,t1);
	/* 2^255 - 2^5 */ square(t1,t0);
	/* 2^255 - 21 */ mul(ret,t1,z11);
}
