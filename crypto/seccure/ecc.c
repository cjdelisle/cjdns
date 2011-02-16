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

#include "ecc.h"
#include "numtheory.h"

/******************************************************************************/

/* Chapter 3.1.2 in the "Guide to Elliptic Curve Cryptography"                */

struct affine_point point_new(void)
{
  struct affine_point r;
  r.x = gcry_mpi_snew(0);
  r.y = gcry_mpi_snew(0);
  return r;
}

void point_release(struct affine_point *p)
{
  gcry_mpi_release(p->x);
  gcry_mpi_release(p->y);
}

void point_set(struct affine_point *p1, const struct affine_point *p2)
{
  gcry_mpi_set(p1->x, p2->x);
  gcry_mpi_set(p1->y, p2->y);
}

void point_load_zero(struct affine_point *p)
{
  gcry_mpi_set_ui(p->x, 0);
  gcry_mpi_set_ui(p->y, 0);
}

int point_is_zero(const struct affine_point *p)
{
  return ! gcry_mpi_cmp_ui(p->x, 0) && ! gcry_mpi_cmp_ui(p->y, 0);
}

int point_on_curve(const struct affine_point *p, const struct domain_params *dp)
{
  int res;
  if (! (res = point_is_zero(p))) {
    gcry_mpi_t h1, h2;
    h1 = gcry_mpi_snew(0);
    h2 = gcry_mpi_snew(0);
    gcry_mpi_mulm(h1, p->x, p->x, dp->m);
    gcry_mpi_addm(h1, h1, dp->a, dp->m);
    gcry_mpi_mulm(h1, h1, p->x, dp->m);
    gcry_mpi_addm(h1, h1, dp->b, dp->m);
    gcry_mpi_mulm(h2, p->y, p->y, dp->m);
    res = ! gcry_mpi_cmp(h1, h2);
    gcry_mpi_release(h1);
    gcry_mpi_release(h2);
  }
  return res;
}

int point_compress(const struct affine_point *p)
{
  return gcry_mpi_test_bit(p->y, 0);
}

int point_decompress(struct affine_point *p, const gcry_mpi_t x, int yflag, 
		     const struct domain_params *dp)
{
  gcry_mpi_t h, y;
  int res;
  h = gcry_mpi_snew(0);
  y = gcry_mpi_snew(0);
  gcry_mpi_mulm(h, x, x, dp->m);
  gcry_mpi_addm(h, h, dp->a, dp->m);
  gcry_mpi_mulm(h, h, x, dp->m);
  gcry_mpi_addm(h, h, dp->b, dp->m);
  if ((res = mod_root(y, h, dp->m)))
    if ((res = (gcry_mpi_cmp_ui(y, 0) || ! yflag))) {
      p->x = gcry_mpi_snew(0);
      p->y = gcry_mpi_snew(0);
      gcry_mpi_set(p->x, x);
      if (gcry_mpi_test_bit(y, 0) == yflag)
	gcry_mpi_set(p->y, y);
      else
	gcry_mpi_sub(p->y, dp->m, y);
      assert(point_on_curve(p, dp));
    }
  gcry_mpi_release(h);
  gcry_mpi_release(y);
  return res;
}

void point_double(struct affine_point *p, const struct domain_params *dp)
{
  if (gcry_mpi_cmp_ui(p->y, 0)) {
    gcry_mpi_t t1, t2;
    t1 = gcry_mpi_snew(0);
    t2 = gcry_mpi_snew(0);
    gcry_mpi_mulm(t2, p->x, p->x, dp->m);
    gcry_mpi_addm(t1, t2, t2, dp->m);
    gcry_mpi_addm(t1, t1, t2, dp->m);
    gcry_mpi_addm(t1, t1, dp->a, dp->m);
    gcry_mpi_addm(t2, p->y, p->y, dp->m);
    gcry_mpi_invm(t2, t2, dp->m);
    gcry_mpi_mulm(t1, t1, t2, dp->m);
    gcry_mpi_mulm(t2, t1, t1, dp->m);
    gcry_mpi_subm(t2, t2, p->x, dp->m);
    gcry_mpi_subm(t2, t2, p->x, dp->m);
    gcry_mpi_subm(p->x, p->x, t2, dp->m);
    gcry_mpi_mulm(t1, t1, p->x, dp->m);
    gcry_mpi_subm(p->y, t1, p->y, dp->m);
    gcry_mpi_set(p->x, t2);
    gcry_mpi_release(t1);
    gcry_mpi_release(t2);
  }
  else
    gcry_mpi_set_ui(p->x, 0);
}

void point_add(struct affine_point *p1, const struct affine_point *p2,
	       const struct domain_params *dp)
{
  if (! point_is_zero(p2)) {
    if (! point_is_zero(p1)) {
      if (! gcry_mpi_cmp(p1->x, p2->x)) {
	if (! gcry_mpi_cmp(p1->y, p2->y))
	  point_double(p1, dp);
	else
	  point_load_zero(p1);
      }
      else {
	gcry_mpi_t t;
	t = gcry_mpi_snew(0);
	gcry_mpi_subm(t, p1->y, p2->y, dp->m);
	gcry_mpi_subm(p1->y, p1->x, p2->x, dp->m);
	gcry_mpi_invm(p1->y, p1->y, dp->m);
	gcry_mpi_mulm(p1->y, t, p1->y, dp->m);
	gcry_mpi_mulm(t, p1->y, p1->y, dp->m);
	gcry_mpi_addm(p1->x, p1->x, p2->x, dp->m);
	gcry_mpi_subm(p1->x, t, p1->x, dp->m);
	gcry_mpi_subm(t, p2->x, p1->x, dp->m);
	gcry_mpi_mulm(p1->y, p1->y, t, dp->m);
	gcry_mpi_subm(p1->y, p1->y, p2->y, dp->m);
	gcry_mpi_release(t);
      }
    }
    else
      point_set(p1, p2);
  }
}

/******************************************************************************/

/* Chapter 3.2.2 in the "Guide to Elliptic Curve Cryptography"                */

struct jacobian_point jacobian_new(void)
{
  struct jacobian_point r;
  r.x = gcry_mpi_snew(0);
  r.y = gcry_mpi_snew(0);
  r.z = gcry_mpi_snew(0);
  return r;
}

void jacobian_release(struct jacobian_point *p)
{
  gcry_mpi_release(p->x);
  gcry_mpi_release(p->y);
  gcry_mpi_release(p->z);
}

void jacobian_load_affine(struct jacobian_point *p1,
			  const struct affine_point *p2)
{
  if (! point_is_zero(p2)) {
    gcry_mpi_set(p1->x, p2->x);
    gcry_mpi_set(p1->y, p2->y);
    gcry_mpi_set_ui(p1->z, 1);
  }
  else
    gcry_mpi_set_ui(p1->z, 0);
}

void jacobian_load_zero(struct jacobian_point *p)
{
  gcry_mpi_set_ui(p->z, 0);
}

int jacobian_is_zero(const struct jacobian_point *p)
{
  return ! gcry_mpi_cmp_ui(p->z, 0);
}

void jacobian_double(struct jacobian_point *p, const struct domain_params *dp)
{
  if (gcry_mpi_cmp_ui(p->z, 0)) {
    if (gcry_mpi_cmp_ui(p->y, 0)) {
      gcry_mpi_t t1, t2;
      t1 = gcry_mpi_snew(0);
      t2 = gcry_mpi_snew(0);
      gcry_mpi_mulm(t1, p->x, p->x, dp->m);
      gcry_mpi_addm(t2, t1, t1, dp->m);
      gcry_mpi_addm(t2, t2, t1, dp->m);
      gcry_mpi_mulm(t1, p->z, p->z, dp->m);
      gcry_mpi_mulm(t1, t1, t1, dp->m);
      gcry_mpi_mulm(t1, t1, dp->a, dp->m);
      gcry_mpi_addm(t1, t1, t2, dp->m);
      gcry_mpi_mulm(p->z, p->z, p->y, dp->m);
      gcry_mpi_addm(p->z, p->z, p->z, dp->m);
      gcry_mpi_mulm(p->y, p->y, p->y, dp->m);
      gcry_mpi_addm(p->y, p->y, p->y, dp->m);
      gcry_mpi_mulm(t2, p->x, p->y, dp->m);
      gcry_mpi_addm(t2, t2, t2, dp->m);
      gcry_mpi_mulm(p->x, t1, t1, dp->m);
      gcry_mpi_subm(p->x, p->x, t2, dp->m);
      gcry_mpi_subm(p->x, p->x, t2, dp->m);
      gcry_mpi_subm(t2, t2, p->x, dp->m);
      gcry_mpi_mulm(t1, t1, t2, dp->m);
      gcry_mpi_mulm(t2, p->y, p->y, dp->m);
      gcry_mpi_addm(t2, t2, t2, dp->m);
      gcry_mpi_subm(p->y, t1, t2, dp->m);
      gcry_mpi_release(t1);
      gcry_mpi_release(t2);
    }
    else
      gcry_mpi_set_ui(p->z, 0);
  }
}

void jacobian_affine_point_add(struct jacobian_point *p1, 
			       const struct affine_point *p2,
			       const struct domain_params *dp)
{
  if (! point_is_zero(p2)) {
    if (gcry_mpi_cmp_ui(p1->z, 0)) {
      gcry_mpi_t t1, t2, t3;
      t1 = gcry_mpi_snew(0);
      t2 = gcry_mpi_snew(0);
      gcry_mpi_mulm(t1, p1->z, p1->z, dp->m);
      gcry_mpi_mulm(t2, t1, p2->x, dp->m);
      gcry_mpi_mulm(t1, t1, p1->z, dp->m);
      gcry_mpi_mulm(t1, t1, p2->y, dp->m);
      if (! gcry_mpi_cmp(p1->x, t2)) {
	if (! gcry_mpi_cmp(p1->y, t1))
	  jacobian_double(p1, dp);
	else
	  jacobian_load_zero(p1);
      }
      else {
	t3 = gcry_mpi_snew(0);
	gcry_mpi_subm(p1->x, p1->x, t2, dp->m);
	gcry_mpi_subm(p1->y, p1->y, t1, dp->m);
	gcry_mpi_mulm(p1->z, p1->z, p1->x, dp->m);
	gcry_mpi_mulm(t3, p1->x, p1->x, dp->m);
	gcry_mpi_mulm(t2, t2, t3, dp->m);
	gcry_mpi_mulm(t3, t3, p1->x, dp->m);
	gcry_mpi_mulm(t1, t1, t3, dp->m);
	gcry_mpi_mulm(p1->x, p1->y, p1->y, dp->m);
	gcry_mpi_subm(p1->x, p1->x, t3, dp->m);
	gcry_mpi_subm(p1->x, p1->x, t2, dp->m);
	gcry_mpi_subm(p1->x, p1->x, t2, dp->m);
	gcry_mpi_subm(t2, t2, p1->x, dp->m);
	gcry_mpi_mulm(p1->y, p1->y, t2, dp->m);
	gcry_mpi_subm(p1->y, p1->y, t1, dp->m);
	gcry_mpi_release(t3);
      }
      gcry_mpi_release(t1);
      gcry_mpi_release(t2);
    }
    else
      jacobian_load_affine(p1, p2);
  }
}

struct affine_point jacobian_to_affine(const struct jacobian_point *p,
				       const struct domain_params *dp)
{
  struct affine_point r = point_new();
  if (gcry_mpi_cmp_ui(p->z, 0)) {
    gcry_mpi_t h;
    h = gcry_mpi_snew(0);
    gcry_mpi_invm(h, p->z, dp->m);
    gcry_mpi_mulm(r.y, h, h, dp->m);
    gcry_mpi_mulm(r.x, p->x, r.y, dp->m);
    gcry_mpi_mulm(r.y, r.y, h, dp->m);
    gcry_mpi_mulm(r.y, r.y, p->y, dp->m);
    gcry_mpi_release(h);
  }
  return r;
}

/******************************************************************************/

/* Algorithm 3.27 in the "Guide to Elliptic Curve Cryptography"               */

#if 0

struct affine_point pointmul(const struct affine_point *p,
			     const gcry_mpi_t exp, 
			     const struct domain_params *dp)
{
  struct affine_point r = point_new();
  int n = gcry_mpi_get_nbits(exp);
  while (n) {
    point_double(&r, dp);
    if (gcry_mpi_test_bit(exp, --n))
      point_add(&r, p, dp);
  }
  assert(point_on_curve(&r, dp));
  return r;
}

#else

struct affine_point pointmul(const struct affine_point *p,
			     const gcry_mpi_t exp, 
			     const struct domain_params *dp)
{
  struct jacobian_point r = jacobian_new();
  struct affine_point R;
  int n = gcry_mpi_get_nbits(exp);
  while (n) {
    jacobian_double(&r, dp);
    if (gcry_mpi_test_bit(exp, --n))
      jacobian_affine_point_add(&r, p, dp);
  }
  R = jacobian_to_affine(&r, dp);
  jacobian_release(&r);
  assert(point_on_curve(&R, dp));
  return R;
}

#endif

/******************************************************************************/

/* Algorithm 4.26 in the "Guide to Elliptic Curve Cryptography"               */
int embedded_key_validation(const struct affine_point *p,
			    const struct domain_params *dp)
{
  if (gcry_mpi_cmp_ui(p->x, 0) < 0 || gcry_mpi_cmp(p->x, dp->m) >= 0 ||
      gcry_mpi_cmp_ui(p->y, 0) < 0 || gcry_mpi_cmp(p->y, dp->m) >= 0)
    return 0;
  return ! point_is_zero(p) && point_on_curve(p, dp);
}

/* Algorithm 4.25 in the "Guide to Elliptic Curve Cryptography"               */
int full_key_validation(const struct affine_point *p,
			const struct domain_params *dp)
{
  if (! embedded_key_validation(p, dp))
    return 0;
  if (dp->cofactor != 1) {
    struct affine_point bp;
    int res;
    bp = pointmul(p, dp->order, dp);
    res = point_is_zero(&bp);
    point_release(&bp);
    return res;
  }
  else
    return 1;
}
