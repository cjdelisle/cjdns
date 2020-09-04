NODEJS = node
REF10 = $(wildcard crypto/ref10/*.c)
CFLAGS = -g -O2 -std=c99 -Wall -Wextra -Werror \
	-Wmissing-prototypes -Wno-pointer-sign -Wno-unused-parameter
LDLIBS = -lsodium -luv

%.o: %.i
	$(CC) $(CFLAGS) $(X_CFLAGS) -c -o $@ $<

%.i: %.ii
	$(NODEJS) ./jsmacro.js $< $@

%.ii: %.c
	$(CC) -E $(CPPFLAGS) $(X_CPPFLAGS) -o $@ $<

# Cancel out the implicit rule
%.o: %.c

# Do not auto-remove .i and .ii files
.SECONDARY:

crypto/CryptoAuth.o: X_CFLAGS = -Wno-unused-result
$(REF10:.c=.ii): X_CPPFLAGS = -I./crypto/ref10

include jscfg.mk
-include config.mk

