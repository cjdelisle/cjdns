LDFLAGS = -L../../libgcrypt-root/lib
CFLAGS = -O0 -ggdb -Wall -I../../libgcrypt-root/include $(LDFLAGS) # -D NOBEEP

default: binaries # doc

binaries: seccure-key seccure-encrypt seccure-decrypt seccure-sign \
	seccure-verify seccure-signcrypt seccure-veridec seccure-dh

doc: seccure.1 seccure.1.html

install: default
	install -m0755 seccure-key $(DESTDIR)/usr/bin
	ln -f $(DESTDIR)/usr/bin/seccure-key $(DESTDIR)/usr/bin/seccure-encrypt
	ln -f $(DESTDIR)/usr/bin/seccure-key $(DESTDIR)/usr/bin/seccure-decrypt
	ln -f $(DESTDIR)/usr/bin/seccure-key $(DESTDIR)/usr/bin/seccure-sign
	ln -f $(DESTDIR)/usr/bin/seccure-key $(DESTDIR)/usr/bin/seccure-verify
	ln -f $(DESTDIR)/usr/bin/seccure-key $(DESTDIR)/usr/bin/seccure-signcrypt
	ln -f $(DESTDIR)/usr/bin/seccure-key $(DESTDIR)/usr/bin/seccure-veridec
	ln -f $(DESTDIR)/usr/bin/seccure-key $(DESTDIR)/usr/bin/seccure-dh
	install -m0644 seccure.1 $(DESTDIR)/usr/share/man/man1

clean:
	rm -f *.o *~ seccure-key seccure-encrypt seccure-decrypt seccure-sign \
	seccure-verify seccure-signcrypt seccure-veridec \
	seccure-dh # seccure.1 seccure.1.html

rebuild: clean default



seccure-key: seccure.o numtheory.o ecc.o serialize.o protocol.o curves.o aes256ctr.o
	$(CC) $(CFLAGS) -o seccure-key -lgcrypt seccure.o numtheory.o ecc.o \
	curves.o serialize.o protocol.o aes256ctr.o
	#strip seccure-key

seccure-encrypt: seccure-key
	ln -f seccure-key seccure-encrypt

seccure-decrypt: seccure-key
	ln -f seccure-key seccure-decrypt

seccure-sign: seccure-key
	ln -f seccure-key seccure-sign

seccure-verify: seccure-key
	ln -f seccure-key seccure-verify

seccure-signcrypt: seccure-key
	ln -f seccure-key seccure-signcrypt

seccure-veridec: seccure-key
	ln -f seccure-key seccure-veridec

seccure-dh: seccure-key
	ln -f seccure-key seccure-dh

seccure.o: seccure.c
	$(CC) $(CFLAGS) -c seccure.c

numtheory.o: numtheory.c numtheory.h
	$(CC) $(CFLAGS) -c numtheory.c

ecc.o: ecc.c ecc.h
	$(CC) $(CFLAGS) -c ecc.c

curves.o: curves.c curves.h
	$(CC) $(CFLAGS) -c curves.c

serialize.o: serialize.c serialize.h
	$(CC) $(CFLAGS) -c serialize.c

protocol.o: protocol.c protocol.h
	$(CC) $(CFLAGS) -c protocol.c

aes256ctr.o: aes256ctr.c aes256ctr.h
	$(CC) $(CFLAGS) -c aes256ctr.c


seccure.1: seccure.manpage.xml
	xmltoman seccure.manpage.xml > seccure.1

seccure.1.html: seccure.manpage.xml
	xmlmantohtml seccure.manpage.xml > seccure.1.html