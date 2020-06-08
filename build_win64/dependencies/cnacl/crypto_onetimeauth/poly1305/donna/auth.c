/* https://github.com/floodyberry/poly1305-donna */

#include "crypto_onetimeauth.h"

#include <stddef.h>

typedef struct poly1305_context {
	size_t aligner;
	unsigned char opaque[136];
} poly1305_context;

#if defined(POLY1305_8BIT)
#include "poly1305-donna-8.h"
#elif defined(POLY1305_16BIT)
#include "poly1305-donna-16.h"
#elif defined(POLY1305_32BIT)
#include "poly1305-donna-32.h"
#elif defined(POLY1305_64BIT)
#include "poly1305-donna-64.h"
#else

/* auto detect between 32bit / 64bit */
#define HAS_SIZEOF_INT128_64BIT (defined(__SIZEOF_INT128__) && defined(__LP64__))
#define HAS_MSVC_64BIT (defined(_MSC_VER) && defined(_M_X64))
#define HAS_GCC_4_4_64BIT (defined(__GNUC__) && defined(__LP64__) && ((__GNUC__ > 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 4))))

#if (HAS_SIZEOF_INT128_64BIT || HAS_MSVC_64BIT || HAS_GCC_4_4_64BIT)
#include "poly1305-donna-64.h"
#else
#include "poly1305-donna-32.h"
#endif

#endif

static void
poly1305_update(poly1305_context *ctx, const unsigned char *m, size_t bytes) {
	poly1305_state_internal_t *st = (poly1305_state_internal_t *)ctx;
	size_t i;

	/* handle leftover */
	if (st->leftover) {
		size_t want = (poly1305_block_size - st->leftover);
		if (want > bytes)
			want = bytes;
		for (i = 0; i < want; i++)
			st->buffer[st->leftover + i] = m[i];
		bytes -= want;
		m += want;
		st->leftover += want;
		if (st->leftover < poly1305_block_size)
			return;
		poly1305_blocks(st, st->buffer, poly1305_block_size);
		st->leftover = 0;
	}

	/* process full blocks */
	if (bytes >= poly1305_block_size) {
		size_t want = (bytes & ~(poly1305_block_size - 1));
		poly1305_blocks(st, m, want);
		m += want;
		bytes -= want;
	}

	/* store leftover */
	if (bytes) {
		for (i = 0; i < bytes; i++)
			st->buffer[st->leftover + i] = m[i];
		st->leftover += bytes;
	}
}

int
crypto_onetimeauth(unsigned char* out, const unsigned char *m, unsigned long long inlen, const unsigned char* key) {
	poly1305_context ctx;
	poly1305_init(&ctx, key);
	poly1305_update(&ctx, m, inlen);
	poly1305_finish(&ctx, out);
	return 0;
}
