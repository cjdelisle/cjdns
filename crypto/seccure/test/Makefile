SECCURE-PATH = "../"
SECCURE-KEY = $(SECCURE-PATH)"seccure-key"
SECCURE-ENCRYPT = $(SECCURE-PATH)"seccure-encrypt"
SECCURE-DECRYPT = $(SECCURE-PATH)"seccure-decrypt"
SECCURE-SIGN = $(SECCURE-PATH)"seccure-sign"
SECCURE-VERIFY = $(SECCURE-PATH)"seccure-verify"
SECCURE-SIGNCRYPT = $(SECCURE-PATH)"seccure-signcrypt"
SECCURE-VERIDEC = $(SECCURE-PATH)"seccure-veridec"

ENCCURVE = "p128"
SIGCURVE = "p256"
MACLEN = "64"


default: encdec-test signveri-test signcrypt-test

clean:
	rm -f public-encryption-key public-signature-key message.enc message.aux message.sig

rebuild: clean default

public-encryption-key: secret-encryption-key
	$(SECCURE-KEY) -c $(ENCCURVE) -F secret-encryption-key -q > public-encryption-key

public-signature-key: secret-signature-key
	$(SECCURE-KEY) -c $(SIGCURVE) -F secret-signature-key -q > public-signature-key

encdec-test: public-encryption-key
	$(SECCURE-ENCRYPT) -m $(MACLEN) -i message.txt -o message.enc -- `cat public-encryption-key`
	$(SECCURE-DECRYPT) -m $(MACLEN) -c $(ENCCURVE) -i message.enc -o message.aux -F secret-encryption-key
	cmp message.txt message.aux
	rm -f message.enc message.aux

signveri-test: public-signature-key
	$(SECCURE-SIGN) -c $(SIGCURVE) -s message.sig -i message.txt -F secret-signature-key
	$(SECCURE-VERIFY) -s message.sig -i message.txt -- `cat public-signature-key`
	rm -f message.sig

signcrypt-test: public-encryption-key public-signature-key
	$(SECCURE-SIGNCRYPT) -c $(SIGCURVE) -i message.txt -o message.enc -F secret-signature-key -- `cat public-encryption-key`
	$(SECCURE-VERIDEC) -c $(ENCCURVE) -i message.enc -o message.aux -F secret-encryption-key -- `cat public-signature-key`
	cmp message.txt message.aux
	rm -f message.enc message.aux
