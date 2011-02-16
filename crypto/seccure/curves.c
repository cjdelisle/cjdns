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

#include <gcrypt.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "curves.h"
#include "ecc.h"
#include "serialize.h"

/******************************************************************************/

#define CURVE_NUM 8

struct curve {
  const char *name, *a, *b, *m, *base_x, *base_y, *order;
  int cofactor;
  int pk_len_compact;
};

static const struct curve curves[CURVE_NUM] = {
  { "secp112r1",
    "db7c2abf62e35e668076bead2088", 
    "659ef8ba043916eede8911702b22", 
    "db7c2abf62e35e668076bead208b",
    "09487239995a5ee76b55f9c2f098",
    "a89ce5af8724c0a23e0e0ff77500", 
    "db7c2abf62e35e7628dfac6561c5", 
    1, 18 },
  
  { "secp128r1",
    "fffffffdfffffffffffffffffffffffc", 
    "e87579c11079f43dd824993c2cee5ed3", 
    "fffffffdffffffffffffffffffffffff",
    "161ff7528b899b2d0c28607ca52c5b86", 
    "cf5ac8395bafeb13c02da292dded7a83",
    "fffffffe0000000075a30d1b9038a115", 
    1, 20 },

  { "secp160r1", 
    "ffffffffffffffffffffffffffffffff7ffffffc",
    "1c97befc54bd7a8b65acf89f81d4d4adc565fa45",
    "ffffffffffffffffffffffffffffffff7fffffff",
    "4a96b5688ef573284664698968c38bb913cbfc82",
    "23a628553168947d59dcc912042351377ac5fb32",
    "0100000000000000000001f4c8f927aed3ca752257", 
    1, 25 },

  { "secp192r1/nistp192",
    "fffffffffffffffffffffffffffffffefffffffffffffffc",
    "64210519e59c80e70fa7e9ab72243049feb8deecc146b9b1", 
    "fffffffffffffffffffffffffffffffeffffffffffffffff",
    "188da80eb03090f67cbf20eb43a18800f4ff0afd82ff1012",
    "07192b95ffc8da78631011ed6b24cdd573f977a11e794811",
    "ffffffffffffffffffffffff99def836146bc9b1b4d22831", 
    1, 30 },

  { "secp224r1/nistp224",
    "fffffffffffffffffffffffffffffffefffffffffffffffffffffffe",
    "b4050a850c04b3abf54132565044b0b7d7bfd8ba270b39432355ffb4",
    "ffffffffffffffffffffffffffffffff000000000000000000000001",
    "b70e0cbd6bb4bf7f321390b94a03c1d356c21122343280d6115c1d21",
    "bd376388b5f723fb4c22dfe6cd4375a05a07476444d5819985007e34",
    "ffffffffffffffffffffffffffff16a2e0b8f03e13dd29455c5c2a3d", 
    1, 35 },

  { "secp256r1/nistp256",
    "ffffffff00000001000000000000000000000000fffffffffffffffffffffffc", 
    "5ac635d8aa3a93e7b3ebbd55769886bc651d06b0cc53b0f63bce3c3e27d2604b", 
    "ffffffff00000001000000000000000000000000ffffffffffffffffffffffff",
    "6b17d1f2e12c4247f8bce6e563a440f277037d812deb33a0f4a13945d898c296", 
    "4fe342e2fe1a7f9b8ee7eb4a7c0f9e162bce33576b315ececbb6406837bf51f5", 
    "ffffffff00000000ffffffffffffffffbce6faada7179e84f3b9cac2fc632551", 
    1, 40 },

  { "secp384r1/nistp384",
    "fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffeffffffff0000000000000000fffffffc",
    "b3312fa7e23ee7e4988e056be3f82d19181d9c6efe8141120314088f5013875ac656398d8a2ed19d2a85c8edd3ec2aef", 
    "fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffeffffffff0000000000000000ffffffff",
    "aa87ca22be8b05378eb1c71ef320ad746e1d3b628ba79b9859f741e082542a385502f25dbf55296c3a545e3872760ab7",
    "3617de4a96262c6f5d9e98bf9292dc29f8f41dbd289a147ce9da3113b5f0b8c00a60b1ce1d7e819d7a431d7c90ea0e5f", 
    "ffffffffffffffffffffffffffffffffffffffffffffffffc7634d81f4372ddf581a0db248b0a77aecec196accc52973", 
    1, 60 },

  { "secp521r1/nistp521",
    "1fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffc",
    "051953eb9618e1c9a1f929a21a0b68540eea2da725b99b315f3b8b489918ef109e156193951ec7e937b1652c0bd3bb1bf073573df883d2c34f1ef451fd46b503f00",
    "1ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff",
    "0c6858e06b70404e9cd9e3ecb662395b4429c648139053fb521f828af606b4d3dbaa14b5e77efe75928fe1dc127a2ffa8de3348b3c1856a429bf97e7e31c2e5bd66",
    "11839296a789a3bc0045c8a5fb42c7d1bd998f54449579b446817afbd17273e662c97ee72995ef42640c550b9013fad0761353c7086a272c24088be94769fd16650",
    "1fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffa51868783bf2f966b7fcc0148f709a5d03bb5c9b8899c47aebb6fb71e91386409", 
    1, 81 },
};

/******************************************************************************/

#define SCAN(x, s) do {                                            \
    assert(! gcry_mpi_scan(x, GCRYMPI_FMT_HEX, s, 0, NULL));       \
    gcry_mpi_set_flag(*x, GCRYMPI_FLAG_SECURE);		           \
  } while(0)

static struct curve_params* load_curve(const struct curve *c, enum disp_format df)
{
  gcry_mpi_t h;
  struct curve_params *cp;
  struct domain_params *dp;

  if (! (cp = malloc(sizeof(struct curve_params))))
    return NULL;

  cp->name = c->name;

  dp = &cp->dp;
  SCAN(&dp->a, c->a);
  SCAN(&dp->b, c->b);
  SCAN(&dp->m, c->m);
  SCAN(&dp->order, c->order);
  SCAN(&dp->base.x, c->base_x);
  SCAN(&dp->base.y, c->base_y);
  dp->cofactor = c->cofactor;

  h = gcry_mpi_new(0);

  gcry_mpi_add(h, dp->m, dp->m);
  gcry_mpi_sub_ui(h, h, 1);
  cp->pk_len_bin = get_serialization_len(h, DF_BIN);
  cp->pk_len_compact = get_serialization_len(h, df);

  gcry_mpi_mul(h, dp->order, dp->order);
  gcry_mpi_sub_ui(h, h, 1);
  cp->sig_len_bin = get_serialization_len(h, DF_BIN);
  cp->sig_len_compact = get_serialization_len(h, df);

  cp->dh_len_bin = (gcry_mpi_get_nbits(dp->order) / 2 + 7) / 8;
  if (cp->dh_len_bin > 32)
    cp->dh_len_bin = 32;

  gcry_mpi_set_ui(h, 0);
  gcry_mpi_set_bit(h, 8 * cp->dh_len_bin);
  gcry_mpi_sub_ui(h, h, 1);
  cp->dh_len_compact = get_serialization_len(h, df);

  cp->elem_len_bin = get_serialization_len(dp->m, DF_BIN);
  cp->order_len_bin = get_serialization_len(dp->order, DF_BIN);

#if 0   /* enable this when adding a new curve to do some sanity checks */
  if (! gcry_mpi_cmp_ui(dp->b, 0)) {
    fprintf(stderr, "FATAL: b == 0\n");
    exit(1);
  }
  if (cp->pk_len_compact != c->pk_len_compact) {
    fprintf(stderr, "FATAL: c->pk_len_compact != %d\n", cp->pk_len_compact);
    exit(1);
  }
  if (! point_on_curve(&dp->base, dp)) {
    fprintf(stderr, "FATAL: base point not on curve!\n");
    exit(1);
  }
  struct affine_point p = pointmul(&dp->base, dp->order, dp);
  if (! point_is_zero(&p)) {
    fprintf(stderr, "FATAL: wrong point order!\n");
    exit(1);
  }
  point_release(&p);

  gcry_mpi_mul_ui(h, dp->order, dp->cofactor);
  gcry_mpi_sub(h, h, dp->m);
  gcry_mpi_sub_ui(h, h, 1);
  gcry_mpi_mul(h, h, h);
  gcry_mpi_rshift(h, h, 2);
  if (gcry_mpi_cmp(h, dp->m) > 0) {
    fprintf(stderr, "FATAL: invalid cofactor!\n");
    exit(1);
  }
#endif

  gcry_mpi_release(h);
  return cp;
}

struct curve_params* curve_by_name(const char *name, enum disp_format df)
{
  const struct curve *c = curves;
  int i;
  for(i = 0; i < CURVE_NUM; i++, c++)
    if (strstr(c->name, name))
      return load_curve(c, df);
  return NULL;
}

struct curve_params* curve_by_pk_len_compact(int len, enum disp_format df)
{
  const struct curve *c = curves;
  int i;
  for(i = 0; i < CURVE_NUM; i++, c++) {
    struct curve_params *cp = load_curve(c, df);
    if( cp->pk_len_compact == len ) {
        return cp;
    }
    curve_release(cp);
  }
  return NULL;
}
 
void curve_release(struct curve_params *cp)
{
  struct domain_params *dp = &cp->dp;
  gcry_mpi_release(dp->a);
  gcry_mpi_release(dp->b);
  gcry_mpi_release(dp->m);
  gcry_mpi_release(dp->order);
  gcry_mpi_release(dp->base.x);
  gcry_mpi_release(dp->base.y);
  free(cp);
}
