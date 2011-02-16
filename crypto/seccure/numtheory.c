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

#include "numtheory.h"

/******************************************************************************/

/* Fact 2.146(i) in the "Handbook of Applied Cryptography"                    */
int mod_issquare(const gcry_mpi_t a, const gcry_mpi_t p) 
{
  if (gcry_mpi_cmp_ui(a, 0)) {
    gcry_mpi_t p1, p2;
    int res;
    p1 = gcry_mpi_snew(0);
    p2 = gcry_mpi_snew(0);
    gcry_mpi_rshift(p1, p, 1);
    gcry_mpi_powm(p2, a, p1, p);
    res = ! gcry_mpi_cmp_ui(p2, 1);
    gcry_mpi_release(p1);
    gcry_mpi_release(p2);
    return res;
  }
  else
    return 1;
}

/* Algorithm II.8 in "Elliptic Curves in Cryptography"                        */
int mod_root(gcry_mpi_t x, const gcry_mpi_t a, const gcry_mpi_t p)
{
  gcry_mpi_t h, n, q, y, b, t;
  int r, m;
  if (! gcry_mpi_cmp_ui(a, 0)) {
    gcry_mpi_set_ui(x, 0);
    return 1;
  }
  if (! mod_issquare(a, p))
    return 0;
  h = gcry_mpi_snew(0);
  n = gcry_mpi_snew(0);
  gcry_mpi_set_ui(n, 2);
  while (mod_issquare(n, p))
    gcry_mpi_add_ui(n, n, 1);
  q = gcry_mpi_snew(0);
  gcry_mpi_sub_ui(q, p, 1);
  for(r = 0; ! gcry_mpi_test_bit(q, r); r++);
  gcry_mpi_rshift(q, q, r);
  y = gcry_mpi_snew(0);
  gcry_mpi_powm(y, n, q, p);
  b = gcry_mpi_snew(0);
  gcry_mpi_rshift(h, q, 1);
  gcry_mpi_powm(b, a, h, p);
  gcry_mpi_mulm(x, a, b, p);
  gcry_mpi_mulm(b, b, x, p);
  t = gcry_mpi_snew(0);
  while (gcry_mpi_cmp_ui(b, 1)) {
    gcry_mpi_mulm(h, b, b, p);
    for(m = 1; gcry_mpi_cmp_ui(h, 1); m++)
      gcry_mpi_mulm(h, h, h, p);
    gcry_mpi_set_ui(h, 0);
    gcry_mpi_set_bit(h, r - m - 1);
    gcry_mpi_powm(t, y, h, p);
    gcry_mpi_mulm(y, t, t, p);
    r = m;
    gcry_mpi_mulm(x, x, t, p);
    gcry_mpi_mulm(b, b, y, p);
  }
  gcry_mpi_release(h);
  gcry_mpi_release(n);
  gcry_mpi_release(q);
  gcry_mpi_release(y);
  gcry_mpi_release(b);
  gcry_mpi_release(t);
  return 1;
}
