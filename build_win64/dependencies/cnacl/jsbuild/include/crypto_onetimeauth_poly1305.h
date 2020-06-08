#ifndef crypto_onetimeauth_poly1305_H
#define crypto_onetimeauth_poly1305_H

#define crypto_onetimeauth_poly1305_amd64_BYTES 16
#define crypto_onetimeauth_poly1305_amd64_KEYBYTES 32

#ifdef __cplusplus
#include <string>
extern std::string crypto_onetimeauth_poly1305_amd64(const std::string &,const std::string &);
extern void crypto_onetimeauth_poly1305_amd64_verify(const std::string &,const std::string &,const std::string &);
extern "C" {
#endif
extern int crypto_onetimeauth_poly1305_amd64(unsigned char *,const unsigned char *,unsigned long long,const unsigned char *);
extern int crypto_onetimeauth_poly1305_amd64_verify(const unsigned char *,const unsigned char *,unsigned long long,const unsigned char *);
#ifdef __cplusplus
}
#endif

#define crypto_onetimeauth_poly1305 crypto_onetimeauth_poly1305_amd64
#define crypto_onetimeauth_poly1305 crypto_onetimeauth_poly1305_amd64
#define crypto_onetimeauth_poly1305_verify crypto_onetimeauth_poly1305_amd64_verify
#define crypto_onetimeauth_poly1305_BYTES crypto_onetimeauth_poly1305_amd64_BYTES
#define crypto_onetimeauth_poly1305_KEYBYTES crypto_onetimeauth_poly1305_amd64_KEYBYTES
#define crypto_onetimeauth_poly1305_IMPLEMENTATION "crypto_onetimeauth/poly1305/amd64"
#ifndef crypto_onetimeauth_poly1305_amd64_VERSION
#define crypto_onetimeauth_poly1305_amd64_VERSION "-"
#endif
#define crypto_onetimeauth_poly1305_VERSION crypto_onetimeauth_poly1305_amd64_VERSION

#endif