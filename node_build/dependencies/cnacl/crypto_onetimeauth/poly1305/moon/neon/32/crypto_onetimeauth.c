#define poly1305_auth_fn poly1305_auth_neon

#include <stddef.h>
#include "crypto_onetimeauth.h"
#include "crypto_verify_16.h"

extern void poly1305_auth_fn(unsigned char *out, const unsigned char *in, size_t inlen, const unsigned char *k);

int crypto_onetimeauth(unsigned char *out, const unsigned char *in, unsigned long long inlen, const unsigned char *k) {
	poly1305_auth_fn(out, in, (size_t)inlen, k);
	return 0;
}

int crypto_onetimeauth_verify(const unsigned char *h, const unsigned char *in, unsigned long long inlen, const unsigned char *k) {
	unsigned char mac[16];
	poly1305_auth_fn(mac, in, (size_t)inlen, k);
	return crypto_verify_16(h, mac);
}

