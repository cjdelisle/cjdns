/*
 *  seccure  -  Copyright 2009 B. Poettering
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License as
 *  published by the Free Software Foundation; either version 2 of the
 *  License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 *  02111-1307 USA
 */

/* 
 *   SECCURE Elliptic Curve Crypto Utility for Reliable Encryption
 *
 *              http://point-at-infinity.org/seccure/
 *
 *
 * seccure implements a selection of asymmetric algorithms based on  
 * elliptic curve cryptography (ECC). See the manpage or the project's  
 * homepage for further details.
 *
 * This code links against the GNU gcrypt library "libgcrypt" (which
 * is part of the GnuPG project). Use the included Makefile to build
 * the binary.
 * 
 * Report bugs to: seccure AT point-at-infinity.org
 *
 */

#include <string.h>
#include <gcrypt.h>
#include <assert.h>

#include "serialize.h"

/******************************************************************************/

/* All ASCII characters in the range 33..126 excluding '\' and all quotes: */
const char compact_digits[COMPACT_DIGITS_COUNT] = { 
  '!', '#', '$', '%', '&', '(', ')', '*', '+', ',', '-', '.', '/', '0', '1', 
  '2', '3', '4', '5', '6', '7', '8', '9', ':', ';', '<', '=', '>', '?', '@', 
  'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 
  'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '[', ']', '^', '_', 
  'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 
  'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '{', '|', '}', '~' };

const char base36_digits[BASE36_DIGIT_COUNT] = {
    '0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f','g','h',
    'i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'
};

const char *get_digits( enum disp_format df ) {
    switch( df ) {
    case DF_BASE36:
        return &base36_digits[0];
    case DF_COMPACT:
        return &compact_digits[0];
    default:
        assert( 0 );
    }    
}

unsigned int get_digit_count( enum disp_format df ) {
    switch( df ) {
    case DF_BASE36:
        return BASE36_DIGIT_COUNT;
    case DF_COMPACT:
        return COMPACT_DIGITS_COUNT;
    default:
        assert( 0 );
    }
}

int get_serialization_len(const gcry_mpi_t x, enum disp_format df)
{
  int res;
  switch(df) {
  case DF_BIN:
    res = (gcry_mpi_get_nbits(x) + 7) / 8;
    break;
  case DF_BASE36:
  case DF_COMPACT:  
    do {
      gcry_mpi_t base, Q;
      base = gcry_mpi_set_ui(NULL, get_digit_count(df));
      Q = gcry_mpi_copy(x);
      for(res = 0; gcry_mpi_cmp_ui(Q, 0); res++)
	gcry_mpi_div(Q, NULL, Q, base, 0);
      gcry_mpi_release(base);
      gcry_mpi_release(Q);
    } while (0);
    break;
  default:
    assert(0);
  }
  return res;
}

void serialize_mpi(char *outbuf, int outlen, enum disp_format df, 
		   const gcry_mpi_t x)
{
  switch(df) {
  case DF_BIN: do {
      int len = (gcry_mpi_get_nbits(x) + 7) / 8;
      assert(len <= outlen);
      memset(outbuf, 0, outlen - len);
      gcry_mpi_print(GCRYMPI_FMT_USG, (unsigned char*)outbuf + (outlen - len), 
		     len, NULL, x);
    } while (0);
    break;
    
  case DF_COMPACT:
  case DF_BASE36: do {
    const char *digits = get_digits(df);
    unsigned int digit_count = get_digit_count(df);
    gcry_mpi_t base, Q, R;
    int i;
    base = gcry_mpi_set_ui(NULL, digit_count);
    Q = gcry_mpi_copy(x);
    R = gcry_mpi_snew(0);
    for(i = outlen - 1; i >= 0; i--) {
        unsigned char digit = 0;
        gcry_mpi_div(Q, R, Q, base, 0);        
        gcry_mpi_print(GCRYMPI_FMT_USG, &digit, 1, NULL, R);
        assert(digit < digit_count);
        outbuf[i] = digits[digit];
    }    
    assert(! gcry_mpi_cmp_ui(Q, 0));
    gcry_mpi_release(base);
    gcry_mpi_release(Q);
    gcry_mpi_release(R);
    } while(0);
    break;
  default: 
    assert(0);
  }
}

int deserialize_mpi(gcry_mpi_t *x, enum disp_format df, const char *buf, 
		    int inlen)
{
  switch(df) {
  case DF_BIN:
    gcry_mpi_scan(x, GCRYMPI_FMT_USG, buf, inlen, NULL);
    gcry_mpi_set_flag(*x, GCRYMPI_FLAG_SECURE);
    break;
  case DF_COMPACT:
  case DF_BASE36:
    do {
    const char *digits = get_digits(df);
    unsigned int digit_count = get_digit_count(df);
    char *d;
    int i;
    *x = gcry_mpi_snew(0);
    for(i = 0; i < inlen; i++) {
        if (! (d = memchr(digits, buf[i], digit_count))) {
          gcry_mpi_release(*x);
          return 0;
        }
        gcry_mpi_mul_ui(*x, *x, digit_count);
        gcry_mpi_add_ui(*x, *x, d - digits);
    }
    } while (0);
    break;
  default: 
    assert(0);
  }
  return 1;
}
