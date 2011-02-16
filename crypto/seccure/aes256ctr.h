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

#ifndef INC_AES256CTR_H
#define INC_AES256CTR_H

#include <gcrypt.h>

#define CIPHER_BLOCK_SIZE 16
#define CIPHER_KEY_SIZE 32

#define HMAC_KEY_SIZE 32

struct aes256ctr {
  gcry_cipher_hd_t ch;
  int idx;
  char buf[CIPHER_BLOCK_SIZE];
};

struct aes256ctr* aes256ctr_init(const char *key);
void aes256ctr_enc(struct aes256ctr *ac, char *buf, int len);
#define aes256ctr_dec aes256ctr_enc
void aes256ctr_done(struct aes256ctr *ac);

int hmacsha256_init(gcry_md_hd_t *mh, const char *key, int len);

#define aes256cprng aes256ctr
#define aes256cprng_init aes256ctr_init
void aes256cprng_fillbuf(struct aes256cprng *cprng, char *buf, int len);
#define aes256cprng_done aes256ctr_done

#endif /* INC_AES256CTR_H */
