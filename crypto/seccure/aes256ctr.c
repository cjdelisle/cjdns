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

#include <stdlib.h>
#include <gcrypt.h>
#include <assert.h>

#include "aes256ctr.h"

/******************************************************************************/

struct aes256ctr* aes256ctr_init(const char *key)
{
  struct aes256ctr *ac;
  gcry_error_t err;

  if (! (ac = gcry_malloc_secure(sizeof(struct aes256ctr))))
    return NULL;

  err = gcry_cipher_open(&ac->ch, GCRY_CIPHER_AES256, 
			 GCRY_CIPHER_MODE_CTR, GCRY_CIPHER_SECURE);
  if (gcry_err_code(err))
    goto error;
  
  err = gcry_cipher_setkey(ac->ch, key, CIPHER_KEY_SIZE);
  if (gcry_err_code(err))
    goto error;
  
  err = gcry_cipher_setctr(ac->ch, NULL, 0);
  if (gcry_err_code(err))
    goto error;

  ac->idx = CIPHER_BLOCK_SIZE;
  return ac;

 error:
  gcry_free(ac);
  return NULL;
}

void aes256ctr_enc(struct aes256ctr *ac, char *buf, int len)
{
  gcry_error_t err;
  int full_blocks;

  for(; len && (ac->idx < CIPHER_BLOCK_SIZE); len--)
    *buf++ ^= ac->buf[ac->idx++];

  full_blocks = (len / CIPHER_BLOCK_SIZE) * CIPHER_BLOCK_SIZE;
  err = gcry_cipher_encrypt(ac->ch, buf, full_blocks, NULL, 0);
  assert(! gcry_err_code(err));
  len -= full_blocks;
  buf += full_blocks;

  if (len) {
    memset(ac->buf, 0, CIPHER_BLOCK_SIZE);
    err = gcry_cipher_encrypt(ac->ch, ac->buf, CIPHER_BLOCK_SIZE, NULL, 0);
    assert(! gcry_err_code(err));
    ac->idx = 0;
    
    for(; len && (ac->idx < CIPHER_BLOCK_SIZE); len--)
      *buf++ ^= ac->buf[ac->idx++];
  }
}

void aes256ctr_done(struct aes256ctr *ac)
{
  gcry_cipher_close(ac->ch);
  memset(ac->buf, 0, CIPHER_BLOCK_SIZE);
  gcry_free(ac);
}

int hmacsha256_init(gcry_md_hd_t *mh, const char *key, int len)
{
  gcry_error_t err;

  err = gcry_md_open(mh, GCRY_MD_SHA256, 
		     GCRY_MD_FLAG_HMAC | GCRY_MD_FLAG_SECURE);
  if (gcry_err_code(err))
    return 0;
  
  err = gcry_md_setkey(*mh, key, len);
  return ! gcry_err_code(err);
}

void aes256cprng_fillbuf(struct aes256cprng *cprng, char *buf, int len)
{
  memset(buf, 0, len);
  aes256ctr_enc(cprng, buf, len);
}
