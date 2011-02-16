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

#ifndef INC_PROTOCOL_H
#define INC_PROTOCOL_H

#include "curves.h"
#include "serialize.h"

gcry_mpi_t buf_to_exponent(const char *buf, int buflen,
			   const struct curve_params *cp);
gcry_mpi_t hash_to_exponent(const char *hash, const struct curve_params *cp);
gcry_mpi_t get_random_exponent(const struct curve_params *cp);

void compress_to_string(char *buf, enum disp_format df, 
			const struct affine_point *P, 
			const struct curve_params *cp);
int decompress_from_string(struct affine_point *P, const char *buf, 
			   enum disp_format df, const struct curve_params *cp);

gcry_mpi_t ECDSA_sign(const char *msg, const gcry_mpi_t d, 
		      const struct curve_params *cp);
int ECDSA_verify(const char *msg, const struct affine_point *Q, 
		 const gcry_mpi_t sig, const struct curve_params *cp);

struct affine_point ECIES_encryption(char *key, const struct affine_point *Q, 
				     const struct curve_params *cp);
int ECIES_decryption(char *key, const struct affine_point *R, 
		     const gcry_mpi_t d, const struct curve_params *cp);

gcry_mpi_t DH_step1(struct affine_point *A, const struct curve_params *cp);
int DH_step2(char *key, const struct affine_point *B, const gcry_mpi_t exp, 
	     const struct curve_params *cp);

#endif /* INC_PROTOCOL_H */
