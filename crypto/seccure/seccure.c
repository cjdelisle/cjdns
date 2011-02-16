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
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <assert.h>
#include <termios.h>
#include <getopt.h>
#include <sys/mman.h>
#include <gcrypt.h>

#include "curves.h"
#include "protocol.h"
#include "serialize.h"
#include "aes256ctr.h"

#define ANSI_CLEAR_LINE "\033[1K\r"
#define VERSION "0.4"

#define COPYBUF_SIZE (1 << 20)
#define DEFAULT_CURVE "p160"
#define DEFAULT_MAC_LEN 10

int opt_help = 0;
int opt_verbose = 0;
int opt_quiet = 0;
int opt_sigcopy = 0;
int opt_sigbin = 0;
int opt_sigappend = 0;
int opt_maclen = -1;
int opt_dblprompt = 0;
enum disp_format opt_format = DF_COMPACT;
char *opt_infile = NULL;
char *opt_outfile = NULL;
char *opt_curve = NULL;
char *opt_curve2 = NULL;
char *opt_pwfile = NULL;
char *opt_sigfile = NULL;

int opt_fdin = STDIN_FILENO;
int opt_fdout = STDOUT_FILENO;
int opt_fdpw = STDIN_FILENO;

/******************************************************************************/

void beep_on_terminal(FILE *term)
{
#if ! NOBEEP
  if (isatty(fileno(term)))
    fputc('\a', term);
#endif
}

#if 0
void warning(const char *msg)
{
  beep_on_terminal(stderr);
  fprintf(stderr, "WARNING: %s.\n", msg);
}

void warning_errno(const char *msg, int err)
{
  beep_on_terminal(stderr);
  fprintf(stderr, "WARNING: %s: %s.\n", msg, strerror(err));
}
#endif

void warning_gcrypt(const char *msg, gcry_error_t err)
{
  beep_on_terminal(stderr);
  fprintf(stderr, "WARNING: %s: %s.\n", msg, gcry_strerror(err));
}

void fatal(const char *msg)
{
  beep_on_terminal(stderr);
  fprintf(stderr, "FATAL: %s.\n", msg);
  exit(1);
}

void fatal_errno(const char *msg, int err)
{
  beep_on_terminal(stderr);
  fprintf(stderr, "FATAL: %s: %s.\n", msg, strerror(err));
  exit(1);
}

void fatal_gcrypt(const char *msg, gcry_error_t err)
{
  beep_on_terminal(stderr);
  fprintf(stderr, "FATAL: %s: %s.\n", msg, gcry_strerror(err));
  exit(1);
}

void print_quiet(const char *msg, int beep)
{
  if (! opt_quiet) {
    if (beep)
      beep_on_terminal(stderr);
    fprintf(stderr, "%s", msg);
  }
}

/******************************************************************************/

void write_block(int fd, const char *buf, int len)
{
  ssize_t c;
  while(len) {
    if ((c = write(fd, buf, len)) < 0)
      fatal_errno("Write error", errno);
    buf += c;
    len -= c;
  }
}

int read_block(int fd, char *buf, int len)
{
  ssize_t c;
  while(len) {
    if ((c = read(fd, buf, len)) < 0)
      fatal_errno("Read error", errno);
    if (c == 0)
      return 0;
    buf += c;
    len -= c;
  }
  return 1;
}

void encryption_loop(int fdin, int fdout, struct aes256ctr *ac,
		     gcry_md_hd_t *mh_pre, gcry_md_hd_t *mh_post)
{
  char buf[COPYBUF_SIZE];
  ssize_t c;
  while ((c = read(fdin, buf, COPYBUF_SIZE)) > 0) {
    if (mh_pre)
      gcry_md_write(*mh_pre, buf, c);
    aes256ctr_enc(ac, buf, c);
    if (mh_post)
      gcry_md_write(*mh_post, buf, c);
    write_block(fdout, buf, c);
  }
  if (c < 0)
    fatal_errno("Read error", errno);
}

void decryption_loop(int fdin, int fdout, struct aes256ctr *ac,
		     gcry_md_hd_t *mh_pre, gcry_md_hd_t *mh_post,
		     char *tail, int taillen)
{
  char buf[COPYBUF_SIZE];
  ssize_t c;
  if (! read_block(fdin, buf, taillen))
    fatal("Input too short");
  while ((c = read(fdin, buf + taillen, COPYBUF_SIZE - taillen)) > 0) {
    if (mh_pre)
      gcry_md_write(*mh_pre, buf, c);
    aes256ctr_dec(ac, buf, c);
    if (mh_post)
      gcry_md_write(*mh_post, buf, c);
    write_block(fdout, buf, c);
    memmove(buf, buf + c, taillen);
  }
  if (c < 0)
    fatal_errno("Read error", errno);
  memcpy(tail, buf, taillen);
}

void verisign_loop(int fdin, int fdout, gcry_md_hd_t *mh,
		   char *tail, int taillen, int copyflag)
{
  char buf[COPYBUF_SIZE];
  ssize_t c;
  if (! read_block(fdin, buf, taillen))
    fatal("Input too short");
  while((c = read(fdin, buf + taillen, COPYBUF_SIZE - taillen)) > 0) {
    gcry_md_write(*mh, buf, c);
    if (copyflag)
      write_block(fdout, buf, c);
    memmove(buf, buf + c, taillen);
  }
  if (c < 0)
    fatal_errno("Read error", errno);
  memcpy(tail, buf, taillen);
}

/******************************************************************************/

void do_read_passphrase(char *hash, const struct termios *err_tios)
{
  gcry_error_t err;
  gcry_md_hd_t mh;
  char *md, ch;
  ssize_t r;

  err = gcry_md_open(&mh, GCRY_MD_SHA256, GCRY_MD_FLAG_SECURE);
  if (gcry_err_code(err)) {
    if (err_tios)
      tcsetattr(opt_fdpw, TCSANOW, err_tios);
    fatal_gcrypt("Cannot initialize SHA256", err);
  }
  
  while (((r = read(opt_fdpw, &ch, 1)) > 0) && (ch != '\n'))
    if (ch != '\r')
      gcry_md_putc(mh, ch);
  if (r < 0) {
    int err = errno;
    if (err_tios)
      tcsetattr(opt_fdpw, TCSANOW, err_tios);
    fatal_errno("Cannot read text line", err);
  }
  
  gcry_md_final(mh);
  md = (char*)gcry_md_read(mh, 0);
  memcpy(hash, md, 32);
  gcry_md_close(mh);
}

void read_passphrase(char *hash, const char *name)
{
  if (isatty(opt_fdpw)) {
    struct termios echo_orig, echo_off;

    tcgetattr(opt_fdpw, &echo_orig);
    echo_off = echo_orig;
    echo_off.c_lflag &= ~ECHO;
    tcsetattr(opt_fdpw, TCSANOW, &echo_off);

    if (! opt_quiet)
      fprintf(stderr, "Enter %s: ", name);
    do_read_passphrase(hash, &echo_orig);
    if (! opt_quiet)
      fprintf(stderr, ANSI_CLEAR_LINE);

    if (opt_dblprompt) {
      char *hash2;
      if (! (hash2 = gcry_malloc_secure(32))) {
	tcsetattr(opt_fdpw, TCSANOW, &echo_orig);
	fatal("Out of secure memory");
      }

      if (! opt_quiet)
	fprintf(stderr, "Reenter %s: ", name);
      do_read_passphrase(hash2, &echo_orig);
      if (! opt_quiet)
	fprintf(stderr, ANSI_CLEAR_LINE);

      if (memcmp(hash, hash2, 32)) {
	tcsetattr(opt_fdpw, TCSANOW, &echo_orig);
	fatal("The two inputs differ");
      }

      gcry_free(hash2);
    }

    tcsetattr(opt_fdpw, TCSANOW, &echo_orig);
  }
  else {
    if (opt_dblprompt)
      print_quiet("Ignoring -d flag.\n", 0);
    do_read_passphrase(hash, NULL);
  }
}

/******************************************************************************/

void app_print_public_key(void)
{
  struct curve_params *cp;
  if (! opt_curve) {
    opt_curve = DEFAULT_CURVE;
    fprintf(stderr, "Assuming curve " DEFAULT_CURVE ".\n");
  }

  if ((cp = curve_by_name(opt_curve, opt_format))) {
    char pubkey[cp->pk_len_compact + 1];
    char *privkey;
    struct affine_point P;
    gcry_mpi_t d;

    if (opt_verbose) {
      print_quiet("VERSION: ", 0);
      fprintf(stderr, VERSION "\n"); 
      print_quiet("CURVE: ", 0); 
      fprintf(stderr, "%s\n", cp->name); 
    }

    if (! (privkey = gcry_malloc_secure(32)))
      fatal("Out of secure memory");
    read_passphrase(privkey, "private key");
    d = hash_to_exponent(privkey, cp);
    gcry_free(privkey);
    P = pointmul(&cp->dp.base, d, &cp->dp);
    gcry_mpi_release(d);

    compress_to_string(pubkey, opt_format, &P, cp);
    pubkey[cp->pk_len_compact] = 0;
    if (! opt_quiet)
      printf("The public key is: ");
    printf("%s\n", pubkey);
    point_release(&P);
    curve_release(cp);
  }
  else
    fatal("Invalid curve name");
}

void app_encrypt(const char *pubkey)
{
  struct affine_point P, R;
  struct curve_params *cp;

  if (opt_maclen < 0) {
    opt_maclen = DEFAULT_MAC_LEN;
    fprintf(stderr, "Assuming MAC length of %d bits.\n", 8 * DEFAULT_MAC_LEN);
  }

  if (opt_curve) {
    if (! (cp = curve_by_name(opt_curve, opt_format)))
      fatal("Invalid curve name");
  }
  else
    if (! (cp = curve_by_pk_len_compact(strlen(pubkey), opt_format)))
      fatal("Invalid encryption key (wrong length)");

  if (opt_verbose) {
    print_quiet("VERSION: ", 0);
    fprintf(stderr, VERSION "\n"); 
    print_quiet("CURVE: ", 0); 
    fprintf(stderr, "%s\n", cp->name); 
    print_quiet("MACLEN: ", 0); 
    fprintf(stderr, "%d\n", 8 * opt_maclen); 
  }

  if (strlen(pubkey) != (size_t)cp->pk_len_compact)
    fatal("Invalid encryption key (wrong length)");
    
  if (decompress_from_string(&P, pubkey, opt_format, cp)) {
    char rbuf[cp->pk_len_bin];
    struct aes256ctr *ac;
    char *keybuf, *md;
    gcry_md_hd_t mh;

    if (! (keybuf = gcry_malloc_secure(64)))
      fatal("Out of secure memory");
    R = ECIES_encryption(keybuf, &P, cp);
    compress_to_string(rbuf, DF_BIN, &R, cp);
    point_release(&P);
    point_release(&R);

    if (opt_verbose) {
      int i;
      print_quiet("K_ENC: ", 0); 
      for(i = 0; i < 32; i++)
	fprintf(stderr, "%02x", (unsigned char)keybuf[i]);
      fprintf(stderr, "\n");
      print_quiet("K_MAC: ", 0);
      for(i = 32; i < 64; i++)
	fprintf(stderr, "%02x", (unsigned char)keybuf[i]);
      fprintf(stderr, "\n");
    }

    if (! (ac = aes256ctr_init(keybuf)))
      fatal("Cannot initialize AES256-CTR");
    if (opt_maclen && ! hmacsha256_init(&mh, keybuf + 32, HMAC_KEY_SIZE))
      fatal("Cannot initialize HMAC-SHA256");
    gcry_free(keybuf);

    if (isatty(opt_fdin))
      print_quiet("Go ahead and type your message ...\n", 0);

    write_block(opt_fdout, rbuf, cp->pk_len_bin);
    encryption_loop(opt_fdin, opt_fdout, ac, NULL, opt_maclen ? &mh : NULL);

    aes256ctr_done(ac);

    if (opt_maclen) {
      gcry_md_final(mh);
      md = (char*)gcry_md_read(mh, 0);

      if (opt_verbose) {
	int i;
	print_quiet("HMAC: ", 0); 
	for(i = 0; i < opt_maclen; i++)
	  fprintf(stderr, "%02x", (unsigned char)md[i]);
	fprintf(stderr, "\n");
      }
      
      write_block(opt_fdout, md, opt_maclen);
      gcry_md_close(mh);
    }
  }
  else
    fatal("Invalid encryption key");
  curve_release(cp);
}

int app_decrypt(void)
{
  struct curve_params *cp;
  struct affine_point R;
  int res = 0;

  if (opt_maclen < 0) {
    opt_maclen = DEFAULT_MAC_LEN;
    fprintf(stderr, "Assuming MAC length of %d bits.\n", 8 * DEFAULT_MAC_LEN);
  }

  if (! opt_curve) {
    opt_curve = DEFAULT_CURVE;
    fprintf(stderr, "Assuming curve " DEFAULT_CURVE ".\n");
  }

  if ((cp = curve_by_name(opt_curve, opt_format))) {
    char *keybuf, *privkey;
    char rbuf[cp->pk_len_bin];
    char mdbuf[opt_maclen], *md;
    struct aes256ctr *ac;
    gcry_md_hd_t mh;
    gcry_mpi_t d;

    if (opt_verbose) {
      print_quiet("VERSION: ", 0);
      fprintf(stderr, VERSION "\n"); 
      print_quiet("CURVE: ", 0); 
      fprintf(stderr, "%s\n", cp->name); 
      print_quiet("MACLEN: ", 0); 
      fprintf(stderr, "%d\n", 8 * opt_maclen);
    }

    if (! (privkey = gcry_malloc_secure(32)))
      fatal("Out of secure memory");
    read_passphrase(privkey, "private key");
    d = hash_to_exponent(privkey, cp);
    gcry_free(privkey);

    if (isatty(opt_fdin))
      print_quiet("Go ahead and enter the ciphertext ...\n", 0);

    if (read_block(opt_fdin, rbuf, cp->pk_len_bin)) {
      if (decompress_from_string(&R, rbuf, DF_BIN, cp)) {
	if (! (keybuf = gcry_malloc_secure(64)))
	  fatal("Out of secure memory");
	if (ECIES_decryption(keybuf, &R, d, cp)) {

	  if (opt_verbose) {
	    int i;
	    print_quiet("K_ENC: ", 0); 
	    for(i = 0; i < 32; i++)
	      fprintf(stderr, "%02x", (unsigned char)keybuf[i]);
	    fprintf(stderr, "\n");
	    print_quiet("K_MAC: ", 0); 
	    for(i = 32; i < 64; i++)
	      fprintf(stderr, "%02x", (unsigned char)keybuf[i]);
	    fprintf(stderr, "\n");
	  }

	  if (! (ac = aes256ctr_init(keybuf)))
	    fatal("Cannot initialize AES256-CTR");
	  if (opt_maclen && ! hmacsha256_init(&mh, keybuf + 32, HMAC_KEY_SIZE))
	    fatal("Cannot initialize HMAC-SHA256");
	  memset(keybuf, 0x00, 64);
	
	  decryption_loop(opt_fdin, opt_fdout, ac, opt_maclen ? &mh : NULL, 
			  NULL, mdbuf, opt_maclen);

	  aes256ctr_done(ac);

	  if (opt_maclen) {
	    gcry_md_final(mh);
	    md = (char*)gcry_md_read(mh, 0);

	    if (opt_verbose) {
	      int i;
	      print_quiet("HMAC1: ", 0); 
	      for(i = 0; i < opt_maclen; i++)
		fprintf(stderr, "%02x", (unsigned char)md[i]);
	      fprintf(stderr, "\n");
	      print_quiet("HMAC2: ", 0); 
	      for(i = 0; i < opt_maclen; i++)
		fprintf(stderr, "%02x", (unsigned char)mdbuf[i]);
	      fprintf(stderr, "\n");
	    }
	  
	    if ((res = ! memcmp(mdbuf, md, opt_maclen)))
	      print_quiet("Integrity check successful, message unforged!\n", 0);
	    else
	      print_quiet("Integrity check failed, message forged!\n", 1);

	    gcry_md_close(mh);
	  }
	  else {
	    res = 1;
	    print_quiet("Warning: No MAC available, message integrity cannot "
			"be verified!\n", 0);
	  }
	}
	else
	  print_quiet("Abort: Inconsistent header.\n", 1);
	gcry_free(keybuf);
	point_release(&R);
      }
      else
	print_quiet("Abort: Inconsistent header.\n", 1);
    }
    else 
      print_quiet("Abort: Inconsistent header (too short).\n", 1);

    gcry_mpi_release(d);
    curve_release(cp);
  }
  else
    fatal("Invalid curve name");
  return ! res;
}

void app_sign(void)
{
  struct curve_params *cp;
  char *privkey, *md;
  gcry_md_hd_t mh;
  gcry_error_t err;
  gcry_mpi_t d, sig;
  FILE *sigfile;

  if (opt_sigappend) {
    opt_sigcopy = 1;
    if (opt_sigfile)
      fatal("The options -s and -a may not be combined");
  }

  if (! opt_curve) {
    opt_curve = DEFAULT_CURVE;
    fprintf(stderr, "Assuming curve " DEFAULT_CURVE ".\n");
  }

  if ((cp = curve_by_name(opt_curve, opt_format))) {

    if (opt_verbose) {
      print_quiet("VERSION: ", 0);
      fprintf(stderr, VERSION "\n"); 
      print_quiet("CURVE: ", 0); 
      fprintf(stderr, "%s\n", cp->name); 
    }

    if (! (privkey = gcry_malloc_secure(32)))
      fatal("Out of secure memory");
    read_passphrase(privkey, "private key");
    d = hash_to_exponent(privkey, cp);
    gcry_free(privkey);

    err = gcry_md_open(&mh, GCRY_MD_SHA512, 0);
    if (gcry_err_code(err))
      fatal_gcrypt("Cannot initialize SHA512", err);

    if (isatty(opt_fdin))
      print_quiet("Go ahead and type your message ...\n", 0);

    verisign_loop(opt_fdin, opt_fdout, &mh, NULL, 0, opt_sigcopy);

    gcry_md_final(mh);
    md = (char*)gcry_md_read(mh, 0);

    if (opt_verbose) {
      int i;
      print_quiet("SHA512: ", 0); 
      for(i = 0; i < 64; i++)
	fprintf(stderr, "%02x", (unsigned char)md[i]);
      fprintf(stderr, "\n");
    }

    sig = ECDSA_sign(md, d, cp);
    gcry_mpi_release(d);
    gcry_md_close(mh);

    if (opt_sigfile) {
      if (! (sigfile = fopen(opt_sigfile, "w")))
	fatal_errno("Cannot open signature file", errno);
    }
    else
      sigfile = stderr;

    if (opt_sigbin) {
      char sigbuf[cp->sig_len_bin];
      serialize_mpi(sigbuf, cp->sig_len_bin, DF_BIN, sig);
      if (opt_sigappend)
	write_block(opt_fdout, sigbuf, cp->sig_len_bin);
      else
	if (fwrite(sigbuf, cp->sig_len_bin, 1, sigfile) != 1)
	  fatal_errno("Cannot write signature", errno);
    }
    else {
      char sigbuf[cp->sig_len_compact + 1];
      serialize_mpi(sigbuf, cp->sig_len_compact, opt_format, sig);
      if (opt_sigappend)
	write_block(opt_fdout, sigbuf, cp->sig_len_compact);
      else {
	sigbuf[cp->sig_len_compact] = 0;
	if (sigfile == stderr)
	  print_quiet("Signature: ", 0);
	if (fprintf(sigfile, "%s\n", sigbuf) < 0)
	  fatal_errno("Cannot write signature", errno);
      }
    }

    if (opt_sigfile && fclose(sigfile))
      fatal_errno("Cannot close signature file", errno);
    
    gcry_mpi_release(sig);
    curve_release(cp);
  }
  else
    fatal("Invalid curve name");
}

int app_verify(const char *pubkey, const char *sig)
{
  struct curve_params *cp;
  struct affine_point Q;
  gcry_mpi_t s;
  gcry_md_hd_t mh;
  gcry_error_t err;
  char *md;
  int res = 0;

  if (!! sig + !! opt_sigfile + !! opt_sigappend != 1)
    fatal("Exactly one signature has to be specified");

  if (sig && opt_sigbin)
    fatal("Binary signature is not accepted here");
  
  if (opt_curve) {
    if (! (cp = curve_by_name(opt_curve, opt_format)))
      fatal("Invalid curve name");
  }
  else
    if (! (cp = curve_by_pk_len_compact(strlen(pubkey), opt_format)))
      fatal("Invalid verification key (wrong length)");
  
  if (opt_verbose) {
    print_quiet("VERSION: ", 0);
    fprintf(stderr, VERSION "\n"); 
    print_quiet("CURVE: ", 0);
    fprintf(stderr, "%s\n", cp->name); 
  }
  
  if (strlen(pubkey) != (size_t)cp->pk_len_compact)
    fatal("Invalid verification key (wrong length)");
  
  if (decompress_from_string(&Q, pubkey, opt_format, cp)) {
    union {
      char compact[cp->sig_len_compact + 2];
      char bin[cp->sig_len_bin];
    } sigbuf;
    
    err = gcry_md_open(&mh, GCRY_MD_SHA512, 0);
    if (gcry_err_code(err))
      fatal_gcrypt("Cannot initialize SHA512", err);
    
    if (opt_sigfile) {
      FILE *sigfile;
      if (! (sigfile = fopen(opt_sigfile, "r")))
	fatal_errno("Cannot open signature file", errno);

      if (opt_sigbin) {
	if (fread(sigbuf.bin, cp->sig_len_bin, 1, sigfile) != 1) {
	  if (ferror(sigfile))
	    fatal_errno("Cannot read signature", errno);
	  else {
	    print_quiet("Invalid signature (wrong length)!\n", 1);
	    fclose(sigfile);
	    goto error;
	  }
	}
      }
      else {
	sigbuf.compact[0] = 0;
	if (! fgets(sigbuf.compact, cp->sig_len_compact + 2, sigfile) && 
	    ferror(sigfile))
	  fatal_errno("Cannot read signature", errno);
	sigbuf.compact[strcspn(sigbuf.compact, " \r\n")] = '\0';
      }
      
      if (fclose(sigfile))
	fatal_errno("Cannot close signature file", errno);
    }

    if (isatty(opt_fdin))
      print_quiet("Go ahead and type your message ...\n", 0);

    if (opt_sigappend) {
      if (opt_sigbin)
	verisign_loop(opt_fdin, opt_fdout, &mh, sigbuf.bin, 
		      cp->sig_len_bin, opt_sigcopy);
      else {
	verisign_loop(opt_fdin, opt_fdout, &mh, sigbuf.compact,
		      cp->sig_len_compact, opt_sigcopy);
	sigbuf.compact[cp->sig_len_compact] = 0;
      }
    }
    else
      verisign_loop(opt_fdin, opt_fdout, &mh, NULL, 0, opt_sigcopy);

    gcry_md_final(mh);
    md = (char*)gcry_md_read(mh, 0);

    if (opt_verbose) {
      int i;
      print_quiet("SHA512: ", 0);
      for(i = 0; i < 64; i++)
	fprintf(stderr, "%02x", (unsigned char)md[i]);
      fprintf(stderr, "\n");
    }
	
    if (! opt_sigbin) {
      if (! sig)
	sig = sigbuf.compact;
      if (strlen(sig) != (size_t)cp->sig_len_compact) {
	print_quiet("Invalid signature (wrong length)!\n", 1);
	goto error;
      }
      else
	if (! deserialize_mpi(&s, opt_format, sig, cp->sig_len_compact)) {
	  print_quiet("Invalid signature (inconsistent structure)!\n", 1);
	  goto error; 
	}
    }
    else
      assert(deserialize_mpi(&s, DF_BIN, sigbuf.bin, cp->sig_len_bin));

    if ((res = ECDSA_verify(md, &Q, s, cp)))
      print_quiet("Signature successfully verified!\n", 0);
    else
      print_quiet("Invalid signature, message forged!\n", 1);

    gcry_mpi_release(s);

  error:
    gcry_md_close(mh);
    point_release(&Q);
  }
  else
    fatal("Invalid verification key");

  curve_release(cp);
  return ! res;
}

void app_signcrypt(const char *pubkey)
{
  struct curve_params *cp_enc, *cp_sig;
  struct affine_point P, R;

  if (! opt_curve) {
    opt_curve = DEFAULT_CURVE;
    fprintf(stderr, "Assuming signature curve " DEFAULT_CURVE ".\n");
  }
  if (! (cp_sig = curve_by_name(opt_curve, opt_format)))
    fatal("Invalid curve name");

  if (opt_curve2) {
    if (! (cp_enc = curve_by_name(opt_curve2, opt_format)))
      fatal("Invalid curve name");
  }
  else
    if (! (cp_enc = curve_by_pk_len_compact(strlen(pubkey), opt_format)))
      fatal("Invalid encryption key (wrong length)");

  if (opt_verbose) {
    print_quiet("VERSION: ", 0);
    fprintf(stderr, VERSION "\n"); 
    print_quiet("SIGNATURE CURVE: ", 0); 
    fprintf(stderr, "%s\n", cp_sig->name); 
    print_quiet("ENCRYPTION CURVE: ", 0); 
    fprintf(stderr, "%s\n", cp_enc->name); 
  }

  if (strlen(pubkey) != (size_t)cp_enc->pk_len_compact)
    fatal("Invalid encryption key (wrong length)");

  if (decompress_from_string(&P, pubkey, opt_format, cp_enc)) {
    char rbuf[cp_enc->pk_len_bin];
    char sigbuf[cp_sig->sig_len_bin];
    char *privkey, *keybuf, *md;
    struct aes256ctr *ac;
    gcry_mpi_t d, sig;
    gcry_md_hd_t mh;
    gcry_error_t err;

    if (! (privkey = gcry_malloc_secure(32)))
      fatal("Out of secure memory");
    read_passphrase(privkey, "private signing key");
    d = hash_to_exponent(privkey, cp_sig);
    gcry_free(privkey);

    if (! (keybuf = gcry_malloc_secure(64)))
      fatal("Out of secure memory");
    R = ECIES_encryption(keybuf, &P, cp_enc);
    compress_to_string(rbuf, DF_BIN, &R, cp_enc);
    point_release(&P);
    point_release(&R);

    if (opt_verbose) {
      int i;
      print_quiet("K_ENC: ", 0); 
      for(i = 0; i < 32; i++)
	fprintf(stderr, "%02x", (unsigned char)keybuf[i]);
      fprintf(stderr, "\n");
    }

    if (! (ac = aes256ctr_init(keybuf)))
      fatal("Cannot initialize AES256-CTR");
    gcry_free(keybuf);

    err = gcry_md_open(&mh, GCRY_MD_SHA512, GCRY_MD_FLAG_SECURE);
    if (gcry_err_code(err))
      fatal_gcrypt("Cannot initialize SHA512", err);

    if (isatty(opt_fdin))
      print_quiet("Go ahead and type your message ...\n", 0);

    write_block(opt_fdout, rbuf, cp_enc->pk_len_bin);
    encryption_loop(opt_fdin, opt_fdout, ac, &mh, NULL);

    gcry_md_final(mh);
    md = (char*)gcry_md_read(mh, 0);

    if (opt_verbose) {
      int i;
      print_quiet("SHA512: ", 0); 
      for(i = 0; i < 64; i++)
	fprintf(stderr, "%02x", (unsigned char)md[i]);
      fprintf(stderr, "\n");
    }

    sig = ECDSA_sign(md, d, cp_sig);
    gcry_mpi_release(d);
    gcry_md_close(mh);

    serialize_mpi(sigbuf, cp_sig->sig_len_bin, DF_BIN, sig);
    aes256ctr_enc(ac, sigbuf, cp_sig->sig_len_bin);
    write_block(opt_fdout, sigbuf, cp_sig->sig_len_bin);

    gcry_mpi_release(sig);
    aes256ctr_done(ac);
  }
  else
    fatal("Invalid encryption key");

  curve_release(cp_enc);
  curve_release(cp_sig);
}

int app_veridec(const char *pubkey)
{
  struct curve_params *cp_enc, *cp_sig;
  struct affine_point Q, R;
  int res = 0;

  if (! opt_curve) {
    opt_curve = DEFAULT_CURVE;
    fprintf(stderr, "Assuming encryption curve " DEFAULT_CURVE ".\n");
  }
  if (! (cp_enc = curve_by_name(opt_curve, opt_format)))
    fatal("Invalid curve name");

  if (opt_curve2) {
    if (! (cp_sig = curve_by_name(opt_curve2, opt_format)))
      fatal("Invalid curve name");
  }
  else
    if (! (cp_sig = curve_by_pk_len_compact(strlen(pubkey), opt_format)))
      fatal("Invalid verification key (wrong length)");

  if (opt_verbose) {
    print_quiet("VERSION: ", 0);
    fprintf(stderr, VERSION "\n"); 
    print_quiet("SIGNATURE CURVE: ", 0); 
    fprintf(stderr, "%s\n", cp_sig->name); 
    print_quiet("ENCRYPTION CURVE: ", 0); 
    fprintf(stderr, "%s\n", cp_enc->name); 
  }
  
  if (strlen(pubkey) != (size_t)cp_sig->pk_len_compact)
    fatal("Invalid verification key (wrong length)");

  if (decompress_from_string(&Q, pubkey, opt_format, cp_sig)) {
    char rbuf[cp_enc->pk_len_bin];
    char sigbuf[cp_sig->sig_len_bin];
    char *privkey, *keybuf, *md;
    gcry_mpi_t d, sig;
    struct aes256ctr *ac;
    gcry_md_hd_t mh;
    gcry_error_t err;
    
    if (! (privkey = gcry_malloc_secure(32)))
      fatal("Out of secure memory");
    read_passphrase(privkey, "private decryption key");
    d = hash_to_exponent(privkey, cp_enc);
    gcry_free(privkey);
    
    if (isatty(opt_fdin))
      print_quiet("Go ahead and enter the ciphertext ...\n", 0);
    
    if (read_block(opt_fdin, rbuf, cp_enc->pk_len_bin)) {
      if (decompress_from_string(&R, rbuf, DF_BIN, cp_enc)) {

	if (! (keybuf = gcry_malloc_secure(64)))
	  fatal("Out of secure memory");

	if (ECIES_decryption(keybuf, &R, d, cp_enc)) {
	  if (opt_verbose) {
	    int i;
	    print_quiet("K_ENC: ", 0); 
	    for(i = 0; i < 32; i++)
	      fprintf(stderr, "%02x", (unsigned char)keybuf[i]);
	    fprintf(stderr, "\n");
	  }

	  if (! (ac = aes256ctr_init(keybuf)))
	    fatal("Cannot initialize AES256-CTR");
	  memset(keybuf, 0xff, 64);

	  err = gcry_md_open(&mh, GCRY_MD_SHA512, GCRY_MD_FLAG_SECURE);
	  if (gcry_err_code(err))
	    fatal_gcrypt("Cannot initialize SHA512", err);

	  decryption_loop(opt_fdin, opt_fdout, ac, NULL, &mh,
			  sigbuf, cp_sig->sig_len_bin);

	  gcry_md_final(mh);
	  md = (char*)gcry_md_read(mh, 0);
	    
	  if (opt_verbose) {
	    int i;
	    print_quiet("SHA512: ", 0); 
	    for(i = 0; i < 64; i++)
	      fprintf(stderr, "%02x", (unsigned char)md[i]);
	    fprintf(stderr, "\n");
	  }

	  aes256ctr_dec(ac, sigbuf, cp_sig->sig_len_bin);
	  assert(deserialize_mpi(&sig, DF_BIN, sigbuf, cp_sig->sig_len_bin));

	  if ((res = ECDSA_verify(md, &Q, sig, cp_sig)))
	    print_quiet("Signature successfully verified!\n", 0);
	  else
	    print_quiet("WARNING: Invalid signature, message forged!\n", 1);

	  gcry_mpi_release(sig);
	  gcry_md_close(mh);
	  aes256ctr_done(ac);
	}
	else
	  print_quiet("Abort: Inconsistent header.\n", 1);

	gcry_free(keybuf);
	point_release(&R);
      }
      else
	print_quiet("Abort: Inconsistent header.\n", 1);
    }
    else 
      print_quiet("Abort: Inconsistent header (too short).\n", 1);

    gcry_mpi_release(d);
    point_release(&Q);
  }  
  else
    fatal("Invalid verification key");
    
  curve_release(cp_enc);
  curve_release(cp_sig);
  return ! res;
}

void app_dh(void)
{
  struct curve_params *cp;

  if (! opt_curve) {
    opt_curve = DEFAULT_CURVE;
    fprintf(stderr, "Assuming curve " DEFAULT_CURVE ".\n");
  }

  if ((cp = curve_by_name(opt_curve, opt_format))) {
    char keyA[cp->pk_len_compact + 1];
    char keyB[cp->pk_len_compact + 2];
    char *keybuf, *outbuf;
    struct affine_point A, B;
    gcry_mpi_t exp, h;

    if (opt_verbose) {
      print_quiet("VERSION: ", 0);
      fprintf(stderr, VERSION "\n"); 
      print_quiet("CURVE: ", 0); 
      fprintf(stderr, "%s\n", cp->name); 
    }
    
    exp = DH_step1(&A, cp);
    compress_to_string(keyA, opt_format, &A, cp);
    point_release(&A);
    keyA[cp->pk_len_compact] = 0;
    print_quiet("Pass this key to your peer: ", 0);
    fprintf(stderr, "%s\n", keyA);

    print_quiet("Enter your peer's key: ", 0);
    keyB[0] = 0;
    if (! fgets(keyB, cp->pk_len_compact + 2, stdin) && ferror(stdin))
      fatal_errno("Cannot read text line", errno);
    keyB[strcspn(keyB, "\r\n")] = 0;

    if (strlen(keyB) != (size_t)cp->pk_len_compact)
      fatal("Invalid key (wrong length)");

    if (decompress_from_string(&B, keyB, opt_format, cp)) {

      if (! (keybuf = gcry_malloc_secure(64)))
	fatal("Out of secure memory");

      if (DH_step2(keybuf, &B, exp, cp)) {
	assert(cp->dh_len_bin <= 32);

	if (opt_verbose) {
	  int i;
	  print_quiet("K_ESTABLISHED: ", 0); 
	  for(i = 0; i < cp->dh_len_bin; i++)
	    fprintf(stderr, "%02x", (unsigned char)keybuf[i]);
	  fprintf(stderr, "\n");
	  print_quiet("K_VERIFICATION: ", 0); 
	  for(i = 0; i < cp->dh_len_bin; i++)
	    fprintf(stderr, "%02x", (unsigned char)keybuf[32 + i]);
	  fprintf(stderr, "\n");
	}

	if (! (outbuf = gcry_malloc_secure(cp->dh_len_compact + 1)))
	  fatal("Out of secure memory");

	assert(deserialize_mpi(&h, DF_BIN, keybuf, cp->dh_len_bin));
	serialize_mpi(outbuf, cp->dh_len_compact, opt_format, h);
	outbuf[cp->dh_len_compact] = 0;
	gcry_mpi_release(h);

	if (! opt_quiet)
	  printf("Established key: ");
	printf("%s\n", outbuf);

	assert(deserialize_mpi(&h, DF_BIN, keybuf + 32, cp->dh_len_bin));
	serialize_mpi(outbuf, cp->dh_len_compact, opt_format, h);
	outbuf[cp->dh_len_compact] = 0;
	gcry_mpi_release(h);

	if (! opt_quiet)
	  printf("Verification key: ");
	printf("%s\n", outbuf);

	gcry_free(outbuf);
      }
      else
	fatal("Invalid key");

      gcry_free(keybuf);
      point_release(&B);
    }
    else
      fatal("Invalid key");

    gcry_mpi_release(exp);
    curve_release(cp);
  }
  else
    fatal("Invalid curve name");
}

/******************************************************************************/

int main(int argc, char **argv)
{
  gcry_error_t err;
  char *progname;
  int res = 0, i;

  assert(gcry_check_version("1.4.1"));

  err = gcry_control(GCRYCTL_INIT_SECMEM, 1);
  if (gcry_err_code(err))
    warning_gcrypt("Cannot enable gcrypt's secure memory management", err);

  err = gcry_control(GCRYCTL_USE_SECURE_RNDPOOL, 1);
  if (gcry_err_code(err))
    warning_gcrypt("Cannot enable gcrypt's secure "
		   "random number generator", err);

  if (getuid() != geteuid())
    seteuid(getuid());
  
  if ((progname = strrchr(argv[0], '/')) == NULL)
    progname = argv[0];
  
  while((i = getopt(argc, argv, "rfbadm:i:o:F:s:c:hvq")) != -1)
    switch(i) {
    case 'r': opt_format = DF_BASE36; break;
    case 'f': opt_sigcopy = 1; break;
    case 'b': opt_sigbin = 1; break;
    case 'a': opt_sigappend = 1; break;
    case 'd': opt_dblprompt = 1; break;
    case 'm':
      opt_maclen = atoi(optarg); 
      if (opt_maclen < 0 || opt_maclen > 256 || opt_maclen % 8)
	fatal("Invalid MAC length");
      opt_maclen /= 8;
      break;
    case 'i': opt_infile = optarg; break;
    case 'o': opt_outfile = optarg; break;
    case 'F': opt_pwfile = optarg; break;
    case 's': opt_sigfile = optarg; break;
    case 'c': 
      if (! opt_curve)
	opt_curve = optarg; 
      else
	opt_curve2 = optarg;
      break;
    case 'h': opt_help = 1; break;
    case 'v': opt_verbose = 1; break;
    case 'q': opt_quiet = 1; break;
    default:
      exit(1);
    }

  if (opt_infile)
    if ((opt_fdin = open(opt_infile, O_RDONLY)) < 0)
      fatal_errno("Cannot open input file", errno);

  if (opt_outfile) {
    int decmode = strstr(progname, "decrypt") || strstr(progname, "veridec");
    if ((opt_fdout = open(opt_outfile, O_WRONLY | O_CREAT | O_TRUNC, 
			  decmode ? 0600 : 0644)) < 0)
      fatal_errno("Cannot open output file", errno);
  }

  if (opt_pwfile) {
    if ((opt_fdpw = open(opt_pwfile, O_RDONLY)) < 0)
      fatal_errno("Cannot open password file", errno);
  }
  else
    if (! opt_infile && ! isatty(STDIN_FILENO))
      if ((opt_fdpw = open("/dev/tty", O_RDONLY)) < 0)
	fatal_errno("Cannot open tty", errno);

  if (strstr(progname, "key")) {
    if (opt_help || optind != argc)
      puts("Generate public key from private key (seccure version " VERSION ").\n"
	   "\n"
	   "seccure-key [-c curve] [-F pwfile] [-d]");
    else
      app_print_public_key();
  }
  else if (strstr(progname, "encrypt")) {
    if (opt_help || optind != argc - 1)
      puts("Encrypt a message with a public key (seccure version" VERSION ").\n"
	   "\n"
	   "seccure-encrypt [-m maclen] [-c curve] [-i infile] [-o outfile] key");
    else
      app_encrypt(argv[optind]);
  }
  else if (strstr(progname, "decrypt")) {
    if (opt_help || optind != argc)
      puts("Decrypt a message using a secret key (seccure version " VERSION ").\n"
	   "\n"
	   "seccure-decrypt [-m maclen] [-c curve] [-i infile] [-o outfile]\n"
	   "                [-F pwfile] [-d]");
    else
      res = app_decrypt();
  }
  else if (strstr(progname, "signcrypt")) {
    if (opt_help || optind != argc - 1)
      puts("Signcrypt a message (seccure version " VERSION ").\n"
	   "\n"
	   "seccure-signcrypt [-c sig_curve [-c enc_curve]] [-i infile] [-o outfile]\n" 
	   "                  [-F pwfile] [-d] key");
    else
      app_signcrypt(argv[optind]);
  }
  else if (strstr(progname, "veridec")) {
    if (opt_help || optind != argc - 1)
      puts("Decrypt and verify a signcrypted message (seccure version " VERSION ").\n"
	   "\n"
	   "seccure-veridec [-c enc_curve [-c sig_curve]] [-i infile] [-o outfile]\n" 
	   "                [-F pwfile] [-d] key");
    else
      res = app_veridec(argv[optind]);
  }
  else if (strstr(progname, "sign")) {
    if (opt_help || optind != argc)
      puts("Generate a signature (seccure version " VERSION ").\n"
	   "\n"
	   "seccure-sign [-f] [-b] [-a] [-c curve] [-s sigfile] [-i infile]\n"
	   "             [-o outfile] [-F pwfile] [-d]");
    else
      app_sign();
  }
  else if (strstr(progname, "verify")) {
    if (opt_help || (optind != argc - 2 && optind != argc - 1))
      puts("Verify the signature of a message (seccure version " VERSION ").\n"
	   "\n"
	   "seccure-verify [-f] [-b] [-a] [-c curve] [-s sigfile] [-i infile]\n" 
	   "               [-o outfile] key [signature]");
    else
      res = app_verify(argv[optind], argv[optind + 1]);
  }
  else if (strstr(progname, "dh")) {
    if (opt_help || optind != argc)
      puts("Perform an interactive Diffie-Hellman key exchange (seccure version " VERSION ").\n"
	   "\n"
	   "seccure-dh [-c curve]");
    else
      app_dh();
  }
  else 
    fatal("Unknown command");
  
  if (opt_infile)
    close(opt_fdin);
  if (opt_outfile)
    close(opt_fdout);
  if (opt_fdpw != opt_fdin)
    close(opt_fdpw);

  gcry_control(GCRYCTL_TERM_SECMEM, 1);
  exit(res);
}
