NODEJS = node
CFLAGS = -g -O2 -std=c99 -Wall -Wextra -Werror \
	-Wmissing-prototypes -Wno-pointer-sign -Wno-unused-parameter
LDLIBS = ./build_linux/dependencies/cnacl/jsbuild/libnacl.a \
	./build_linux/dependencies/libuv/out/Release/obj.target/libuv.a -lpthread

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

include jscfg.mk
-include config.mk

