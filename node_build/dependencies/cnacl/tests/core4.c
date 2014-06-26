#include <stdio.h>
#include "crypto_core_salsa20.h"

unsigned char k[32] = {
   1,  2,  3,  4,  5,  6,  7,  8
,  9, 10, 11, 12, 13, 14, 15, 16
,201,202,203,204,205,206,207,208
,209,210,211,212,213,214,215,216
} ;

unsigned char in[16] = {
 101,102,103,104,105,106,107,108
,109,110,111,112,113,114,115,116
} ;

unsigned char c[16] = {
 101,120,112, 97,110,100, 32, 51
, 50, 45, 98,121,116,101, 32,107
} ;

unsigned char out[64];

main()
{
  int i;
  crypto_core_salsa20(out,in,k,c);
  for (i = 0;i < 64;++i) {
    if (i > 0) printf(","); else printf(" ");
    printf("%3d",(unsigned int) out[i]);
    if (i % 8 == 7) printf("\n");
  }
  return 0;
}
