# 1 "crypto/CryptoAuth.c"
# 1 "/home/jojapoppa/cjdns//"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "crypto/CryptoAuth.c"
# 17 "crypto/CryptoAuth.c"
# 1 "./util/crypto.h" 1
# 18 "./util/crypto.h"
# 1 "./util/Linker.h" 1
# 19 "./util/crypto.h" 2
struct Linker_x04331853275081825;
# 30 "./util/crypto.h"
extern int crypto_auth_hmacsha512256_tweet(unsigned char *,const unsigned char *,
    unsigned long long,const unsigned char *);
extern int crypto_auth_hmacsha512256_tweet_verify(const unsigned char *,
    const unsigned char *,unsigned long long,const unsigned char *);
# 61 "./util/crypto.h"
extern int crypto_box_curve25519xsalsa20poly1305_tweet(unsigned char *,
    const unsigned char *,unsigned long long,const unsigned char *,
    const unsigned char *,const unsigned char *);
extern int crypto_box_curve25519xsalsa20poly1305_tweet_open(unsigned char *,
    const unsigned char *,unsigned long long,const unsigned char *,
    const unsigned char *,const unsigned char *);
extern int crypto_box_curve25519xsalsa20poly1305_tweet_keypair(unsigned char *,
    unsigned char *);
extern int crypto_box_beforenm(unsigned char *,
    const unsigned char *,const unsigned char *);
extern int crypto_box_curve25519xsalsa20poly1305_tweet_afternm(unsigned char *,
    const unsigned char *,unsigned long long,const unsigned char *,const unsigned char *);
extern int crypto_box_curve25519xsalsa20poly1305_tweet_open_afternm(unsigned char *,
    const unsigned char *,unsigned long long,const unsigned char *,const unsigned char *);
# 114 "./util/crypto.h"
extern int crypto_core_salsa20_tweet(unsigned char *,const unsigned char *,
    const unsigned char *,const unsigned char *);
# 128 "./util/crypto.h"
extern int crypto_core_hsalsa20_tweet(unsigned char *,const unsigned char *,
    const unsigned char *,const unsigned char *);
# 159 "./util/crypto.h"
extern int crypto_hashblocks_sha256_tweet(unsigned char *,const unsigned char *,unsigned long long);
# 183 "./util/crypto.h"
extern int crypto_hash_sha256_hex(unsigned char *,const unsigned char *,unsigned long long);
extern int crypto_hash_sha256(unsigned char *,const unsigned char *,unsigned long long);
# 200 "./util/crypto.h"
extern int crypto_onetimeauth_poly1305_tweet(unsigned char *,const unsigned char *,
    unsigned long long,const unsigned char *);
extern int crypto_onetimeauth_poly1305_tweet_verify(const unsigned char *,
    const unsigned char *,unsigned long long,const unsigned char *);
# 218 "./util/crypto.h"
extern int crypto_scalarmult(unsigned char *,
    const unsigned char *,const unsigned char *);
extern int crypto_scalarmult_base(unsigned char *,
    const unsigned char *);
# 242 "./util/crypto.h"
extern int crypto_secretbox_xsalsa20poly1305_tweet(unsigned char *,const unsigned char *,
    unsigned long long,const unsigned char *,const unsigned char *);
extern int crypto_secretbox_xsalsa20poly1305_tweet_open(unsigned char *,
    const unsigned char *,unsigned long long,const unsigned char *,const unsigned char *);
# 275 "./util/crypto.h"
extern int crypto_sign_ed25519_tweet(unsigned char *,unsigned long long *,
    const unsigned char *,unsigned long long,const unsigned char *);
extern int crypto_sign_ed25519_tweet_open(unsigned char *,unsigned long long *,
    const unsigned char *,unsigned long long,const unsigned char *);
extern int crypto_sign_ed25519_tweet_keypair(unsigned char *,unsigned char *);
# 298 "./util/crypto.h"
extern int crypto_stream_xsalsa20_tweet(unsigned char *,unsigned long long,
    const unsigned char *,const unsigned char *);
extern int crypto_stream_xsalsa20_tweet_xor(unsigned char *,const unsigned char *,
    unsigned long long,const unsigned char *,const unsigned char *);
# 311 "./util/crypto.h"
extern int crypto_stream_salsa20_tweet(unsigned char *,unsigned long long,
    const unsigned char *,const unsigned char *);
extern int crypto_stream_salsa20_tweet_xor(unsigned char *,const unsigned char *,
    unsigned long long,const unsigned char *,const unsigned char *);
# 328 "./util/crypto.h"
extern int crypto_verify_16_tweet(const unsigned char *,const unsigned char *);






extern int crypto_verify_32_tweet(const unsigned char *,const unsigned char *);
# 18 "crypto/CryptoAuth.c" 2


# 1 "./crypto/CryptoAuth_pvt.h" 1
# 17 "./crypto/CryptoAuth_pvt.h"
# 1 "./crypto/CryptoAuth.h" 1
# 18 "./crypto/CryptoAuth.h"
# 1 "./benc/StringList.h" 1
# 18 "./benc/StringList.h"
# 1 "./benc/String.h" 1
# 18 "./benc/String.h"
# 1 "./benc/Object.h" 1
# 18 "./benc/Object.h"
# 1 "/usr/share/mingw-w64/include/inttypes.h" 1 3
# 11 "/usr/share/mingw-w64/include/inttypes.h" 3
# 1 "/usr/share/mingw-w64/include/crtdefs.h" 1 3
# 10 "/usr/share/mingw-w64/include/crtdefs.h" 3
# 1 "/usr/share/mingw-w64/include/_mingw.h" 1 3
# 12 "/usr/share/mingw-w64/include/_mingw.h" 3
# 1 "/usr/share/mingw-w64/include/_mingw_mac.h" 1 3
# 98 "/usr/share/mingw-w64/include/_mingw_mac.h" 3
             
# 107 "/usr/share/mingw-w64/include/_mingw_mac.h" 3
             
# 13 "/usr/share/mingw-w64/include/_mingw.h" 2 3
# 1 "/usr/share/mingw-w64/include/_mingw_secapi.h" 1 3
# 14 "/usr/share/mingw-w64/include/_mingw.h" 2 3
# 282 "/usr/share/mingw-w64/include/_mingw.h" 3
# 1 "/usr/share/mingw-w64/include/vadefs.h" 1 3
# 9 "/usr/share/mingw-w64/include/vadefs.h" 3
# 1 "/usr/share/mingw-w64/include/_mingw.h" 1 3
# 595 "/usr/share/mingw-w64/include/_mingw.h" 3
# 1 "/usr/share/mingw-w64/include/sdks/_mingw_directx.h" 1 3
# 596 "/usr/share/mingw-w64/include/_mingw.h" 2 3
# 1 "/usr/share/mingw-w64/include/sdks/_mingw_ddk.h" 1 3
# 597 "/usr/share/mingw-w64/include/_mingw.h" 2 3
# 10 "/usr/share/mingw-w64/include/vadefs.h" 2 3




#pragma pack(push,_CRT_PACKING)
# 24 "/usr/share/mingw-w64/include/vadefs.h" 3
  
# 24 "/usr/share/mingw-w64/include/vadefs.h" 3
 typedef __builtin_va_list __gnuc_va_list;






  typedef __gnuc_va_list va_list;
# 103 "/usr/share/mingw-w64/include/vadefs.h" 3
#pragma pack(pop)
# 283 "/usr/share/mingw-w64/include/_mingw.h" 2 3
# 569 "/usr/share/mingw-w64/include/_mingw.h" 3
void __attribute__((__cdecl__)) __debugbreak(void);
extern __inline__ __attribute__((__always_inline__,__gnu_inline__)) void __attribute__((__cdecl__)) __debugbreak(void)
{
  __asm__ __volatile__("int {$}3":);
}




const char *__mingw_get_crt_info (void);
# 11 "/usr/share/mingw-w64/include/crtdefs.h" 2 3




#pragma pack(push,_CRT_PACKING)
# 35 "/usr/share/mingw-w64/include/crtdefs.h" 3
__extension__ typedef unsigned long long size_t;
# 45 "/usr/share/mingw-w64/include/crtdefs.h" 3
__extension__ typedef long long ssize_t;






typedef size_t rsize_t;
# 62 "/usr/share/mingw-w64/include/crtdefs.h" 3
__extension__ typedef long long intptr_t;
# 75 "/usr/share/mingw-w64/include/crtdefs.h" 3
__extension__ typedef unsigned long long uintptr_t;
# 88 "/usr/share/mingw-w64/include/crtdefs.h" 3
__extension__ typedef long long ptrdiff_t;
# 98 "/usr/share/mingw-w64/include/crtdefs.h" 3
typedef unsigned short wchar_t;







typedef unsigned short wint_t;
typedef unsigned short wctype_t;





typedef int errno_t;




typedef long __time32_t;




__extension__ typedef long long __time64_t;
# 138 "/usr/share/mingw-w64/include/crtdefs.h" 3
typedef __time64_t time_t;
# 422 "/usr/share/mingw-w64/include/crtdefs.h" 3
struct threadlocaleinfostruct;
struct threadmbcinfostruct;
typedef struct threadlocaleinfostruct *pthreadlocinfo;
typedef struct threadmbcinfostruct *pthreadmbcinfo;
struct __lc_time_data;

typedef struct localeinfo_struct {
  pthreadlocinfo locinfo;
  pthreadmbcinfo mbcinfo;
} _locale_tstruct,*_locale_t;



typedef struct tagLC_ID {
  unsigned short wLanguage;
  unsigned short wCountry;
  unsigned short wCodePage;
} LC_ID,*LPLC_ID;




typedef struct threadlocaleinfostruct {
  int refcount;
  unsigned int lc_codepage;
  unsigned int lc_collate_cp;
  unsigned long lc_handle[6];
  LC_ID lc_id[6];
  struct {
    char *locale;
    wchar_t *wlocale;
    int *refcount;
    int *wrefcount;
  } lc_category[6];
  int lc_clike;
  int mb_cur_max;
  int *lconv_intl_refcount;
  int *lconv_num_refcount;
  int *lconv_mon_refcount;
  struct lconv *lconv;
  int *ctype1_refcount;
  unsigned short *ctype1;
  const unsigned short *pctype;
  const unsigned char *pclmap;
  const unsigned char *pcumap;
  struct __lc_time_data *lc_time_curr;
} threadlocinfo;







#pragma pack(pop)
# 12 "/usr/share/mingw-w64/include/inttypes.h" 2 3
# 1 "/usr/lib/gcc/x86_64-w64-mingw32/9.2-win32/include/stdint.h" 1 3 4
# 9 "/usr/lib/gcc/x86_64-w64-mingw32/9.2-win32/include/stdint.h" 3 4
# 1 "/usr/share/mingw-w64/include/stdint.h" 1 3 4
# 32 "/usr/share/mingw-w64/include/stdint.h" 3 4
# 1 "/usr/lib/gcc/x86_64-w64-mingw32/9.2-win32/include/stddef.h" 1 3 4
# 1 "/usr/share/mingw-w64/include/stddef.h" 1 3 4
# 18 "/usr/share/mingw-w64/include/stddef.h" 3 4
  __attribute__ ((__dllimport__)) extern int *__attribute__((__cdecl__)) _errno(void);

  errno_t __attribute__((__cdecl__)) _set_errno(int _Value);
  errno_t __attribute__((__cdecl__)) _get_errno(int *_Value);


  __attribute__ ((__dllimport__)) extern unsigned long __attribute__((__cdecl__)) __threadid(void);

  __attribute__ ((__dllimport__)) extern uintptr_t __attribute__((__cdecl__)) __threadhandle(void);
# 2 "/usr/lib/gcc/x86_64-w64-mingw32/9.2-win32/include/stddef.h" 2 3 4
# 416 "/usr/lib/gcc/x86_64-w64-mingw32/9.2-win32/include/stddef.h" 3 4
typedef struct {
  long long __max_align_ll __attribute__((__aligned__(__alignof__(long long))));
  long double __max_align_ld __attribute__((__aligned__(__alignof__(long double))));
# 427 "/usr/lib/gcc/x86_64-w64-mingw32/9.2-win32/include/stddef.h" 3 4
} max_align_t;
# 33 "/usr/share/mingw-w64/include/stdint.h" 2 3 4


typedef signed char int8_t;
typedef unsigned char uint8_t;
typedef short int16_t;
typedef unsigned short uint16_t;
typedef int int32_t;
typedef unsigned uint32_t;
__extension__ typedef long long int64_t;
__extension__ typedef unsigned long long uint64_t;


typedef signed char int_least8_t;
typedef unsigned char uint_least8_t;
typedef short int_least16_t;
typedef unsigned short uint_least16_t;
typedef int int_least32_t;
typedef unsigned uint_least32_t;
__extension__ typedef long long int_least64_t;
__extension__ typedef unsigned long long uint_least64_t;





typedef signed char int_fast8_t;
typedef unsigned char uint_fast8_t;
typedef short int_fast16_t;
typedef unsigned short uint_fast16_t;
typedef int int_fast32_t;
typedef unsigned int uint_fast32_t;
__extension__ typedef long long int_fast64_t;
__extension__ typedef unsigned long long uint_fast64_t;


__extension__ typedef long long intmax_t;
__extension__ typedef unsigned long long uintmax_t;
# 10 "/usr/lib/gcc/x86_64-w64-mingw32/9.2-win32/include/stdint.h" 2 3 4
# 13 "/usr/share/mingw-w64/include/inttypes.h" 2 3

# 1 "/usr/lib/gcc/x86_64-w64-mingw32/9.2-win32/include/stddef.h" 1 3 4
# 1 "/usr/share/mingw-w64/include/stddef.h" 1 3 4
# 2 "/usr/lib/gcc/x86_64-w64-mingw32/9.2-win32/include/stddef.h" 2 3 4
# 15 "/usr/share/mingw-w64/include/inttypes.h" 2 3





typedef struct {
 intmax_t quot;
 intmax_t rem;
 } imaxdiv_t;


# 1 "/usr/share/mingw-w64/include/_mingw_print_push.h" 1 3
# 27 "/usr/share/mingw-w64/include/inttypes.h" 2 3
# 279 "/usr/share/mingw-w64/include/inttypes.h" 3
intmax_t __attribute__((__cdecl__)) imaxabs (intmax_t j);




imaxdiv_t __attribute__((__cdecl__)) imaxdiv (intmax_t numer, intmax_t denom);



intmax_t __attribute__((__cdecl__)) strtoimax (const char* __restrict__ nptr,
                            char** __restrict__ endptr, int base);
uintmax_t __attribute__((__cdecl__)) strtoumax (const char* __restrict__ nptr,
        char** __restrict__ endptr, int base);

intmax_t __attribute__((__cdecl__)) wcstoimax (const wchar_t* __restrict__ nptr,
                            wchar_t** __restrict__ endptr, int base);
uintmax_t __attribute__((__cdecl__)) wcstoumax (const wchar_t* __restrict__ nptr,
        wchar_t** __restrict__ endptr, int base);


# 1 "/usr/share/mingw-w64/include/_mingw_print_pop.h" 1 3
# 300 "/usr/share/mingw-w64/include/inttypes.h" 2 3
# 19 "./benc/Object.h" 2

# 1 "./memory/Allocator.h" 1
# 18 "./memory/Allocator.h"
# 1 "./util/Identity.h" 1
# 18 "./util/Identity.h"
# 1 "./util/Assert.h" 1
# 18 "./util/Assert.h"
# 1 "./util/Gcc.h" 1
# 18 "./util/Gcc.h"
# 1 "./util/Js.h" 1
# 19 "./util/Gcc.h" 2
# 97 "./util/Gcc.h"

# 97 "./util/Gcc.h"








__attribute__((__format__ (__printf__, 1, 2)))
static inline void Gcc_checkPrintf(const char* format, ...)
{

}
# 19 "./util/Assert.h" 2

struct Linker_x0011710950721655511;
# 33 "./util/Assert.h"
__attribute__((__format__ (__printf__, 1, 2)))
__attribute__((__noreturn__))
void Assert_failure(const char* format, ...);
# 19 "./util/Identity.h" 2
# 1 "./util/Constant.h" 1
# 33 "./util/Constant.h"

# 20 "./util/Identity.h" 2



    
# 19 "./memory/Allocator.h" 2


struct Linker_x08610927714772509;





struct Allocator_OnFreeJob;
typedef int (* Allocator_OnFreeCallback)(struct Allocator_OnFreeJob* job);
struct Allocator_OnFreeJob
{

    Allocator_OnFreeCallback callback;
    void* userData;
};
# 99 "./memory/Allocator.h"
struct Allocator
{

    const char* fileName;


    int lineNum;


    int isFreeing;
};

struct Allocator_Allocation
{
    unsigned long size;
};
# 124 "./memory/Allocator.h"
struct Allocator* Allocator_getChild(struct Allocator* alloc, int childNumber);
# 133 "./memory/Allocator.h"
struct Allocator_Allocation* Allocator_getAllocation(struct Allocator* alloc, int allocNum);
# 145 "./memory/Allocator.h"
__attribute__ ((alloc_size(2)))
void* Allocator__malloc(struct Allocator* allocator,
                        unsigned long length,
                        const char* fileName,
                        int lineNum);
# 164 "./memory/Allocator.h"
__attribute__ ((alloc_size(2,3)))
void* Allocator__calloc(struct Allocator* alloc,
                        unsigned long length,
                        unsigned long count,
                        const char* fileName,
                        int lineNum);
# 186 "./memory/Allocator.h"
__attribute__ ((alloc_size(3)))
void* Allocator__realloc(struct Allocator* allocator,
                         const void* original,
                         unsigned long size,
                         const char* fileName,
                         int lineNum);
# 205 "./memory/Allocator.h"
__attribute__ ((alloc_size(3)))
void* Allocator__clone(struct Allocator* allocator,
                       const void* toClone,
                       unsigned long length,
                       const char* fileName,
                       int lineNum);
# 221 "./memory/Allocator.h"
struct Allocator* Allocator__child(struct Allocator* alloc, const char* fileName, int lineNum);
# 236 "./memory/Allocator.h"
void Allocator__free(struct Allocator* alloc, const char* file, int line);
# 247 "./memory/Allocator.h"
struct Allocator_OnFreeJob* Allocator__onFree(struct Allocator* alloc,
                                              Allocator_OnFreeCallback callback,
                                              void* context,
                                              const char* file,
                                              int line);
# 260 "./memory/Allocator.h"
int Allocator_cancelOnFree(struct Allocator_OnFreeJob* toRemove);






void Allocator_onFreeComplete(struct Allocator_OnFreeJob* onFreeJob);
# 294 "./memory/Allocator.h"
void Allocator__adopt(struct Allocator* parentAlloc,
                      struct Allocator* alloc,
                      const char* fileName,
                      int lineNum);
# 310 "./memory/Allocator.h"
void Allocator__disown(struct Allocator* parentAlloc,
                       struct Allocator* allocToDisown,
                       const char* fileName,
                       int lineNum);
# 324 "./memory/Allocator.h"
void Allocator_setCanary(struct Allocator* alloc, unsigned long value);




unsigned long Allocator_bytesAllocated(struct Allocator* allocator);







void Allocator_snapshot(struct Allocator* alloc, int includeAllocations);
# 367 "./memory/Allocator.h"
    __attribute__ ((alloc_size(3)))

typedef void* (* Allocator_Provider)(void* ctx,
                                     struct Allocator_Allocation* original,
                                     unsigned long size,
                                     struct Allocator* group);

struct Allocator* Allocator_new(unsigned long sizeLimit,
                                Allocator_Provider provider,
                                void* providerContext,
                                const char* fileName,
                                int lineNum);
# 21 "./benc/Object.h" 2


typedef struct Dict_Entry* Dict;
typedef struct List_Item* List;
typedef struct {
    unsigned long len;
    char* bytes;
} String;

enum Object_Type {
    Object_INTEGER,
    Object_STRING,
    Object_LIST,
    Object_DICT,
    Object_UNPARSABLE
};

typedef struct {
    enum Object_Type type;
    union {
        int64_t number;
        String* string;
        List* list;
        Dict* dictionary;
    } as;
} Object;
# 19 "./benc/String.h" 2

# 1 "./util/CString.h" 1
# 21 "./util/CString.h"
struct Linker_x01185331655903965;

# 1 "/usr/lib/gcc/x86_64-w64-mingw32/9.2-win32/include/stddef.h" 1 3 4
# 1 "/usr/share/mingw-w64/include/stddef.h" 1 3 4
# 2 "/usr/lib/gcc/x86_64-w64-mingw32/9.2-win32/include/stddef.h" 2 3 4
# 24 "./util/CString.h" 2

__attribute__ ((__pure__))
__attribute__((__nonnull__(1)))
unsigned long CString_strlen(const char* str);

__attribute__ ((__pure__))
__attribute__((__nonnull__(1,2)))
int CString_strcmp(const char* a, const char* b);

__attribute__ ((__pure__))
__attribute__((__nonnull__(1,2)))
int CString_strncmp(const char* a, const char *b, size_t n);

__attribute__ ((__pure__))
__attribute__((__nonnull__(1)))
char* CString_strchr(const char *a, int b);

__attribute__ ((__pure__))
__attribute__((__nonnull__(1)))
char* CString_strrchr(const char *a, int b);

__attribute__ ((__pure__))
int CString_strcasecmp(const char *a, const char *b);


__attribute__ ((__pure__))
__attribute__((__nonnull__(1,2)))
char* CString_strstr(const char* a, const char* b);

__attribute__((__nonnull__(1,2)))
char* CString_strcpy(char* restrict dest, const char* restrict src);

__attribute__((__nonnull__(1,2)))
char* CString_safeStrncpy(char* restrict dest, const char *restrict src, size_t n);

char* CString_strdup(const char* string, struct Allocator* alloc);
# 21 "./benc/String.h" 2

struct Linker_x0011992127228792615;

# 1 "/usr/lib/gcc/x86_64-w64-mingw32/9.2-win32/include/stdbool.h" 1 3 4
# 25 "./benc/String.h" 2
# 1 "/usr/lib/gcc/x86_64-w64-mingw32/9.2-win32/include/stddef.h" 1 3 4
# 1 "/usr/share/mingw-w64/include/stddef.h" 1 3 4
# 2 "/usr/lib/gcc/x86_64-w64-mingw32/9.2-win32/include/stddef.h" 2 3 4
# 26 "./benc/String.h" 2
# 35 "./benc/String.h"
String* String_new(const char* bytes, struct Allocator* allocator);
# 57 "./benc/String.h"
String* String_newBinary(const char* bytes, unsigned long length, struct Allocator* allocator);
# 71 "./benc/String.h"
String* String_printf(struct Allocator* allocator, const char* format, ...);
# 94 "./benc/String.h"
int String_compare(const String* a, const String* b);
# 103 "./benc/String.h"
int String_equals(const String* a, const String* b);
# 19 "./benc/StringList.h" 2




# 1 "./util/ArrayList.h" 1
# 21 "./util/ArrayList.h"
struct Linker_x08481162709888088;





struct ArrayList;

void* ArrayList_new(struct Allocator* alloc, int initialCapacity);
int ArrayList_add(struct ArrayList* list, void* val);
void* ArrayList_get(struct ArrayList* list, int number);
int ArrayList_put(struct ArrayList* list, int number, void* val);
void* ArrayList_remove(struct ArrayList* list, int num);
void ArrayList_sort(struct ArrayList* list, int (* compare)(const void* a, const void* b));
void* ArrayList_clone(struct ArrayList* vlist, struct Allocator* alloc);
# 60 "./util/ArrayList.h"
struct StringList {
    int length;
};

static inline struct StringList* StringList_new(struct Allocator* alloc)
{
    return (struct StringList*) ArrayList_new(alloc, 8);
}

static inline String* StringList_get(struct StringList* list, int number)
{
    return (String*) ArrayList_get((struct ArrayList*) list, number);
}

static inline int StringList_put(struct StringList* list,
                                          int number,
                                          String* val)
{
    return ArrayList_put((struct ArrayList*) list, number, val);
}

static inline int StringList_add(struct StringList* list, void* val)
{
    return ArrayList_put((struct ArrayList*) list, list->length, val);
}

static inline String* StringList_shift(struct StringList* list)
{
    return (String*) ArrayList_remove((struct ArrayList*) list, 0);
}

static inline String* StringList_pop(struct StringList* list)
{
    return (String*) ArrayList_remove((struct ArrayList*) list, list->length - 1);
}

static inline String* StringList_remove(struct StringList* list, int num)
{
    return (String*) ArrayList_remove((struct ArrayList*) list, num);
}
# 113 "./util/ArrayList.h"
static inline void* StringList_clone(struct StringList* l, struct Allocator* alloc)
{
    return ArrayList_clone((struct ArrayList*) l, alloc);
}
# 24 "./benc/StringList.h" 2
# 19 "./crypto/CryptoAuth.h" 2
# 1 "./crypto/random/Random.h" 1
# 19 "./crypto/random/Random.h"
# 1 "./exception/Except.h" 1
# 18 "./exception/Except.h"
# 1 "./exception/Er.h" 1
# 23 "./exception/Er.h"
struct Linker_x019935272321362896;

struct Er_Ret
{
    const char* message;
};






__attribute__((__format__ (__printf__, 4, 5)))
__attribute__ ((warn_unused_result))
struct Er_Ret* Er__raise(char* file, int line, struct Allocator* alloc, char* format, ...);
# 55 "./exception/Er.h"
static inline struct Er_Ret* Er_unwind(const char* file, int line, struct Er_Ret* ret)
{
    return ret;
}

void Er__assertFail(struct Er_Ret* er);
# 19 "./exception/Except.h" 2


struct Linker_x009197279505202016;



struct Except;
struct Except
{
    struct Except* next;

    void (* exception)(char* message, struct Except* handler);

    char message[1024];
};

__attribute__((__noreturn__))
__attribute__((__format__ (__printf__, 4, 5)))
void Except__throw(char* file, int line, struct Except* eh, char* format, ...);
# 20 "./crypto/random/Random.h" 2
# 1 "./util/log/Log.h" 1
# 20 "./util/log/Log.h"
struct Linker_x06055581812284749;

enum Log_Level
{
    Log_Level_KEYS,
    Log_Level_DEBUG,
    Log_Level_INFO,
    Log_Level_WARN,
    Log_Level_ERROR,
    Log_Level_CRITICAL,
    Log_Level_INVALID
};

struct Log;

char* Log_nameForLevel(enum Log_Level logLevel);

enum Log_Level Log_levelForName(char* name);
# 63 "./util/log/Log.h"
__attribute__((__format__ (__printf__, 5, 6)))
void Log_print(struct Log* log,
               enum Log_Level logLevel,
               const char* file,
               int line,
               const char* format,
               ...);
# 21 "./crypto/random/Random.h" 2
# 1 "./crypto/random/seed/RandomSeed.h" 1
# 21 "./crypto/random/seed/RandomSeed.h"
struct Linker_x07236150229429092;



struct RandomSeed;

typedef struct RandomSeed* (* RandomSeed_Provider)(struct Allocator* alloc);
typedef int (* RandomSeed_Get)(struct RandomSeed* context, uint64_t buff[8]);

struct RandomSeed
{







    RandomSeed_Get get;


    const char* name;
};

struct RandomSeed* RandomSeed_new(RandomSeed_Provider* providers,
                                  int providerCount,
                                  struct Log* logger,
                                  struct Allocator* alloc);

int RandomSeed_get(struct RandomSeed* rs, uint64_t buffer[8]);
# 22 "./crypto/random/Random.h" 2

struct Linker_x08230613182773474;



struct Random;

void Random_bytes(struct Random* rand, uint8_t* location, uint64_t count);







void Random_base32(struct Random* rand, uint8_t* output, uint32_t length);

void Random_addRandom(struct Random* rand, uint32_t randomNumber);


static inline void Random_longs(struct Random* rand, uint64_t* location, uint64_t count)
{
    Random_bytes(rand, (uint8_t*) location, count*8);
}

static inline void Random_ints(struct Random* rand, uint32_t* location, uint64_t count)
{
    Random_bytes(rand, (uint8_t*) location, count*4);
}

static inline int8_t Random_int8(struct Random* rand)
{
    int8_t ret;
    Random_bytes(rand, (uint8_t*)&ret, 1);
    return ret;
}
static inline uint8_t Random_uint8(struct Random* rand)
{
    uint8_t ret;
    Random_bytes(rand, (uint8_t*)&ret, 1);
    return ret;
}

static inline int16_t Random_int16(struct Random* rand)
{
    int16_t ret;
    Random_bytes(rand, (uint8_t*)&ret, 2);
    return ret;
}
static inline uint16_t Random_uint16(struct Random* rand)
{
    uint16_t ret;
    Random_bytes(rand, (uint8_t*)&ret, 2);
    return ret;
}

static inline int32_t Random_int32(struct Random* rand)
{
    int32_t ret;
    Random_bytes(rand, (uint8_t*)&ret, 4);
    return ret;
}
static inline uint32_t Random_uint32(struct Random* rand)
{
    uint32_t ret;
    Random_bytes(rand, (uint8_t*)&ret, 4);
    return ret;
}

static inline int64_t Random_int64(struct Random* rand)
{
    int64_t ret;
    Random_longs(rand, (uint64_t*)&ret, 1);
    return ret;
}
static inline uint64_t Random_uint64(struct Random* rand)
{
    uint64_t ret;
    Random_longs(rand, &ret, 1);
    return ret;
}

struct Random* Random_newWithSeed(struct Allocator* alloc,
                                  struct Log* logger,
                                  struct RandomSeed* seed,
                                  struct Except* eh);

struct Random* Random_new(struct Allocator* alloc, struct Log* logger, struct Except* eh);
# 20 "./crypto/CryptoAuth.h" 2
# 1 "./crypto/ReplayProtector.h" 1
# 18 "./crypto/ReplayProtector.h"
# 1 "./util/Bits.h" 1
# 21 "./util/Bits.h"
struct Linker_x011948871989887522;


# 1 "/usr/lib/gcc/x86_64-w64-mingw32/9.2-win32/include/stddef.h" 1 3 4
# 1 "/usr/share/mingw-w64/include/stddef.h" 1 3 4
# 2 "/usr/lib/gcc/x86_64-w64-mingw32/9.2-win32/include/stddef.h" 2 3 4
# 25 "./util/Bits.h" 2




static inline int Bits_ffs64(uint64_t number)
{
    if (!number) {
        return 0;
    }
    int out = 1;
    while (!(number & 1)) {
        number >>= 1;
        out++;
    }
    return out;
}

static inline int Bits_popCountx64(uint64_t number)
{
    return __builtin_popcountll(number);
}

static inline int Bits_popCountx32(uint32_t number)
{
    int out = 0;
    for (int i = 0; i < 32; i++) {
        out += ((number >> i) & 1);
    }
    return out;
}

static inline int Bits_log2x64(uint64_t number)
{
    if (!number) { return 0; }
    return 63 - __builtin_clzll(number);
}
int Bits_log2x64_stupid(uint64_t number);


static inline uint64_t Bits_maxBits64(uint32_t bitCount)
{
    do { if (!((bitCount < 64))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("Bits.h"), (66), "(bitCount < 64)"); } } while (0);
    return (((uint64_t)1) << bitCount) - 1;
}

static inline int Bits_log2x32(uint32_t number)
{
    int out = 0;
    while (number >>= 1) {
        out++;
    }
    return out;
}





static inline uint64_t Bits_bitReverse64(uint64_t toReverse)
{



    toReverse = ((toReverse >> 1) & 0x5555555555555555ull) | ((toReverse & 0x5555555555555555ull) << 1);
    toReverse = ((toReverse >> 2) & 0x3333333333333333ull) | ((toReverse & 0x3333333333333333ull) << 2);
    toReverse = ((toReverse >> 4) & 0x0F0F0F0F0F0F0F0Full) | ((toReverse & 0x0F0F0F0F0F0F0F0Full) << 4);
    return __builtin_bswap64(toReverse);


}






static inline int Bits_isZero(void* buffer, size_t length)
{
    uint8_t* buff = (uint8_t*) buffer;
    for (size_t i = 0; i < length; i++) {
        if (buff[i]) {
            return 0;
        }
    }
    return 1;
}
# 129 "./util/Bits.h"
static inline void* Bits__memcpy(void* out,
                                 const void* in,
                                 size_t length,
                                 char* file,
                                 int line)
{
    const char* inc = in;
    const char* outc = out;

    if (outc >= inc && outc < inc + length) {
        Assert_failure(file, line, "memcpy() pointers alias each other");
    }
    return __builtin_memcpy(out, in, length);
}

void* Bits_memmem(const void* haystack, size_t haystackLen, const void* needle, size_t needleLen);

static inline uint16_t Bits_get16(uint8_t* bytes)
{
    uint16_t x = 0;
    Bits__memcpy(&x,bytes,2,"Bits.h",149);
    return x;
}

static inline void Bits_put16(uint8_t* bytes, uint16_t num)
{
    Bits__memcpy(bytes,&num,2,"Bits.h",155);
}
# 19 "./crypto/ReplayProtector.h" 2



struct ReplayProtector
{

    uint64_t bitfield;


    uint32_t baseOffset;


    uint32_t duplicates;


    uint32_t lostPackets;







    uint32_t receivedOutOfRange;
};

static inline int RelayProtector_lostInShift(uint64_t bitfield, int shiftAmount)
{
    if (shiftAmount == 0) {
        return 0;
    }
    if (shiftAmount > 63) {
        return shiftAmount - Bits_popCountx64(bitfield);
    }
    return shiftAmount - Bits_popCountx64(bitfield << (64 - shiftAmount));
}
# 66 "./crypto/ReplayProtector.h"
static inline 
# 66 "./crypto/ReplayProtector.h" 3 4
             _Bool 
# 66 "./crypto/ReplayProtector.h"
                  ReplayProtector_checkNonce(const uint32_t nonce, struct ReplayProtector* context)
{
    if (nonce < context->baseOffset) {
        context->receivedOutOfRange++;
        return 
# 70 "./crypto/ReplayProtector.h" 3 4
              0
# 70 "./crypto/ReplayProtector.h"
                   ;
    }

    uint32_t offset = nonce - context->baseOffset;

    while (offset > 63) {
# 86 "./crypto/ReplayProtector.h"
        if ((context->bitfield & 0xffffffffu) == 0xffffffffu) {

            context->baseOffset += (32); if ((32) > 63) { context->bitfield = 0; } else { context->bitfield >>= (32); } offset -= (32);;

        } else {


            context->lostPackets += RelayProtector_lostInShift(context->bitfield, offset - 47);
            context->baseOffset += (offset - 47); if ((offset - 47) > 63) { context->bitfield = 0; } else { context->bitfield >>= (offset - 47); } offset -= (offset - 47);;
        }
    }

    if (context->bitfield & (((uint64_t)1) << offset)) {
        context->duplicates++;
        return 
# 100 "./crypto/ReplayProtector.h" 3 4
              0
# 100 "./crypto/ReplayProtector.h"
                   ;
    }
    context->bitfield |= (((uint64_t)1) << offset);
    return 
# 103 "./crypto/ReplayProtector.h" 3 4
          1
# 103 "./crypto/ReplayProtector.h"
              ;
}
# 21 "./crypto/CryptoAuth.h" 2

# 1 "./util/Endian.h" 1
# 23 "./crypto/CryptoAuth.h" 2

# 1 "./util/events/EventBase.h" 1
# 20 "./util/events/EventBase.h"
struct Linker_x09034798910400239;

struct EventBase
{
    int unused;
};

struct EventBase* EventBase_new(struct Allocator* alloc);

int EventBase_eventCount(struct EventBase* eventBase);

void EventBase_beginLoop(struct EventBase* eventBase);

void EventBase_endLoop(struct EventBase* eventBase);
# 25 "./crypto/CryptoAuth.h" 2
# 1 "./wire/Message.h" 1
# 23 "./wire/Message.h"
struct Linker_x016428773196722757;



struct Message
{

    int32_t length;


    int32_t padding;


    uint8_t* bytes;


    int32_t capacity;






    int associatedFd;



        struct Iface* currentIface;



    struct Allocator* alloc;
};

struct Message* Message_new(uint32_t messageLength,
                                          uint32_t amountOfPadding,
                                          struct Allocator* alloc);

void Message_setAssociatedFd(struct Message* msg, int fd);

int Message_getAssociatedFd(struct Message* msg);

struct Message* Message_clone(struct Message* toClone, struct Allocator* alloc);

void Message_copyOver(struct Message* output,
                                    struct Message* input,
                                    struct Allocator* allocator);





static inline __attribute__ ((warn_unused_result)) struct Er_Ret* Message_eshift(struct Message* toShift, int32_t amount)
{
    if (amount > 0 && toShift->padding < amount) {
        do { struct Er_Ret* Er_ret = Er__raise("Message.h", 78, toShift->alloc, "buffer overflow adding %d to length %d", amount, toShift->length); return Er_ret;; } while (0)
                                    ;
    } else if (toShift->length < (-amount)) {
        do { struct Er_Ret* Er_ret = Er__raise("Message.h", 81, toShift->alloc, "buffer underflow"); return Er_ret;; } while (0);
    }

    toShift->length += amount;
    toShift->capacity += amount;
    toShift->bytes -= amount;
    toShift->padding -= amount;

    return (struct Er_Ret*)0;
}

static inline void Message_reset(struct Message* toShift)
{
    do { if (!((toShift->length <= toShift->capacity))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("Message.h"), (94), "(toShift->length <= toShift->capacity)"); } } while (0);
    toShift->length = toShift->capacity;
    do {             struct Er_Ret* Er_ret = Message_eshift(toShift, -toShift->length);                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("Message.h", 96, Er_ret);                 } else {                     Er__assertFail(Er_unwind("Message.h", 96, Er_ret));                 }             }                  } while (0);
}

static inline __attribute__ ((warn_unused_result)) struct Er_Ret* Message_epush(struct Message* restrict msg, const void* restrict object, size_t size)


{
    do {             struct Er_Ret* Er_ret = Message_eshift(msg, (int)size);             if (Er_ret) { return Er_unwind("Message.h", 103, Er_ret); }         } while (0);
    if (object) {
        Bits__memcpy(msg->bytes,object,size,"Message.h",105);
    } else {
        __builtin_memset(msg->bytes,0x00,size);
    }
    return (struct Er_Ret*)0;
}

static inline __attribute__ ((warn_unused_result)) struct Er_Ret* Message_epop(struct Message* restrict msg, void* restrict object, size_t size)


{
    do {             struct Er_Ret* Er_ret = Message_eshift(msg, -(int)size);             if (Er_ret) { return Er_unwind("Message.h", 116, Er_ret); }         } while (0);
    if (object) {
        Bits__memcpy(object,&msg->bytes[-((int)size)],size,"Message.h",118);
    }
    return (struct Er_Ret*)0;
}
# 157 "./wire/Message.h"
static inline __attribute__ ((warn_unused_result)) struct Er_Ret* Message_epush8h(struct Message* msg, uint8_t dat) { do {             struct Er_Ret* Er_ret = Message_epush(msg, &dat, (8)/8);             if (Er_ret) { return Er_unwind("Message.h", 157, Er_ret); }         } while (0); return (struct Er_Ret*)0; }
static inline __attribute__ ((warn_unused_result)) struct Er_Ret* Message_epop8h(uint8_t *Er_returnValP, struct Message* msg) { uint8_t out; do {             struct Er_Ret* Er_ret = Message_epop(msg, &out, (8)/8);             if (Er_ret) { return Er_unwind("Message.h", 158, Er_ret); }         } while (0); *Er_returnValP = out; return (struct Er_Ret*)0; }




static inline __attribute__ ((warn_unused_result)) struct Er_Ret* Message_epush16h(struct Message* msg, uint16_t dat) { do {             struct Er_Ret* Er_ret = Message_epush(msg, &dat, (16)/8);             if (Er_ret) { return Er_unwind("Message.h", 163, Er_ret); }         } while (0); return (struct Er_Ret*)0; } static inline __attribute__ ((warn_unused_result)) struct Er_Ret* Message_epop16h(uint16_t *Er_returnValP, struct Message* msg) { uint16_t out; do {             struct Er_Ret* Er_ret = Message_epop(msg, &out, (16)/8);             if (Er_ret) { return Er_unwind("Message.h", 163, Er_ret); }         } while (0); *Er_returnValP = out; return (struct Er_Ret*)0; } static inline __attribute__ ((warn_unused_result)) struct Er_Ret* Message_epop16be(uint16_t *Er_returnValP, struct Message* msg) { uint16_t out = (__extension__({             uint16_t Er_returnVal;             __builtin_memset(&Er_returnVal, 0, sizeof(Er_returnVal));             struct Er_Ret* Er_ret = Message_epop16h(&Er_returnVal, msg);             if (Er_ret) { return Er_unwind("Message.h", 163, Er_ret); }             Er_returnVal;         })); uint16_t out1 = ((uint16_t)(__extension__ ({ uint16_t _x = (((uint16_t)(out))); _x >> 8 | _x << 8; }))); *Er_returnValP = out1; return (struct Er_Ret*)0; } static inline __attribute__ ((warn_unused_result)) struct Er_Ret* Message_epop16le(uint16_t *Er_returnValP, struct Message* msg) { uint16_t out = (__extension__({             uint16_t Er_returnVal;             __builtin_memset(&Er_returnVal, 0, sizeof(Er_returnVal));             struct Er_Ret* Er_ret = Message_epop16h(&Er_returnVal, msg);             if (Er_ret) { return Er_unwind("Message.h", 163, Er_ret); }             Er_returnVal;         })); uint16_t out1 = (out); *Er_returnValP = out1; return (struct Er_Ret*)0; }
static inline __attribute__ ((warn_unused_result)) struct Er_Ret* Message_epush32h(struct Message* msg, uint32_t dat) { do {             struct Er_Ret* Er_ret = Message_epush(msg, &dat, (32)/8);             if (Er_ret) { return Er_unwind("Message.h", 164, Er_ret); }         } while (0); return (struct Er_Ret*)0; } static inline __attribute__ ((warn_unused_result)) struct Er_Ret* Message_epop32h(uint32_t *Er_returnValP, struct Message* msg) { uint32_t out; do {             struct Er_Ret* Er_ret = Message_epop(msg, &out, (32)/8);             if (Er_ret) { return Er_unwind("Message.h", 164, Er_ret); }         } while (0); *Er_returnValP = out; return (struct Er_Ret*)0; } static inline __attribute__ ((warn_unused_result)) struct Er_Ret* Message_epop32be(uint32_t *Er_returnValP, struct Message* msg) { uint32_t out = (__extension__({             uint32_t Er_returnVal;             __builtin_memset(&Er_returnVal, 0, sizeof(Er_returnVal));             struct Er_Ret* Er_ret = Message_epop32h(&Er_returnVal, msg);             if (Er_ret) { return Er_unwind("Message.h", 164, Er_ret); }             Er_returnVal;         })); uint32_t out1 = ((uint32_t)__builtin_bswap32(((uint32_t)(out)))); *Er_returnValP = out1; return (struct Er_Ret*)0; } static inline __attribute__ ((warn_unused_result)) struct Er_Ret* Message_epop32le(uint32_t *Er_returnValP, struct Message* msg) { uint32_t out = (__extension__({             uint32_t Er_returnVal;             __builtin_memset(&Er_returnVal, 0, sizeof(Er_returnVal));             struct Er_Ret* Er_ret = Message_epop32h(&Er_returnVal, msg);             if (Er_ret) { return Er_unwind("Message.h", 164, Er_ret); }             Er_returnVal;         })); uint32_t out1 = (out); *Er_returnValP = out1; return (struct Er_Ret*)0; }
static inline __attribute__ ((warn_unused_result)) struct Er_Ret* Message_epush64h(struct Message* msg, uint64_t dat) { do {             struct Er_Ret* Er_ret = Message_epush(msg, &dat, (64)/8);             if (Er_ret) { return Er_unwind("Message.h", 165, Er_ret); }         } while (0); return (struct Er_Ret*)0; } static inline __attribute__ ((warn_unused_result)) struct Er_Ret* Message_epop64h(uint64_t *Er_returnValP, struct Message* msg) { uint64_t out; do {             struct Er_Ret* Er_ret = Message_epop(msg, &out, (64)/8);             if (Er_ret) { return Er_unwind("Message.h", 165, Er_ret); }         } while (0); *Er_returnValP = out; return (struct Er_Ret*)0; } static inline __attribute__ ((warn_unused_result)) struct Er_Ret* Message_epop64be(uint64_t *Er_returnValP, struct Message* msg) { uint64_t out = (__extension__({             uint64_t Er_returnVal;             __builtin_memset(&Er_returnVal, 0, sizeof(Er_returnVal));             struct Er_Ret* Er_ret = Message_epop64h(&Er_returnVal, msg);             if (Er_ret) { return Er_unwind("Message.h", 165, Er_ret); }             Er_returnVal;         })); uint64_t out1 = ((uint64_t)__builtin_bswap64(((uint64_t)(out)))); *Er_returnValP = out1; return (struct Er_Ret*)0; } static inline __attribute__ ((warn_unused_result)) struct Er_Ret* Message_epop64le(uint64_t *Er_returnValP, struct Message* msg) { uint64_t out = (__extension__({             uint64_t Er_returnVal;             __builtin_memset(&Er_returnVal, 0, sizeof(Er_returnVal));             struct Er_Ret* Er_ret = Message_epop64h(&Er_returnVal, msg);             if (Er_ret) { return Er_unwind("Message.h", 165, Er_ret); }             Er_returnVal;         })); uint64_t out1 = (out); *Er_returnValP = out1; return (struct Er_Ret*)0; }
# 26 "./crypto/CryptoAuth.h" 2

struct Linker_x07070361899108664;







struct CryptoAuth
{
    uint8_t publicKey[32];
};

struct CryptoAuth_Session
{
    uint8_t herPublicKey[32];

    String* displayName;

    struct ReplayProtector replayProtector;





    uint8_t herIp6[16];





    uint32_t resetAfterInactivitySeconds;


    uint32_t setupResetAfterInactivitySeconds;
};
# 83 "./crypto/CryptoAuth.h"
int CryptoAuth_addUser_ipv6(String* password,
                            String* login,
                            uint8_t ipv6[16],
                            struct CryptoAuth* ca);

static inline int CryptoAuth_addUser(String* password, String* login, struct CryptoAuth* ca)
{
    return CryptoAuth_addUser_ipv6(password, login, 
# 90 "./crypto/CryptoAuth.h" 3 4
                                                   ((void *)0)
# 90 "./crypto/CryptoAuth.h"
                                                       , ca);
}
# 100 "./crypto/CryptoAuth.h"
int CryptoAuth_removeUsers(struct CryptoAuth* context, String* user);
# 109 "./crypto/CryptoAuth.h"
struct StringList* CryptoAuth_getUsers(struct CryptoAuth* context, struct Allocator* alloc);
# 122 "./crypto/CryptoAuth.h"
struct CryptoAuth* CryptoAuth_new(struct Allocator* allocator,
                                  const uint8_t* privateKey,
                                  struct EventBase* eventBase,
                                  struct Log* logger,
                                  struct Random* rand);
# 142 "./crypto/CryptoAuth.h"
struct CryptoAuth_Session* CryptoAuth_newSession(struct CryptoAuth* ca,
                                                 struct Allocator* alloc,
                                                 const uint8_t herPublicKey[32],
                                                 const 
# 145 "./crypto/CryptoAuth.h" 3 4
                                                      _Bool 
# 145 "./crypto/CryptoAuth.h"
                                                           requireAuth,
                                                 char* name);


int CryptoAuth_encrypt(struct CryptoAuth_Session* session, struct Message* msg);

enum CryptoAuth_DecryptErr {
    CryptoAuth_DecryptErr_NONE = 0,


    CryptoAuth_DecryptErr_RUNT = 1,


    CryptoAuth_DecryptErr_NO_SESSION = 2,

    CryptoAuth_DecryptErr_FINAL_SHAKE_FAIL = 3,

    CryptoAuth_DecryptErr_FAILED_DECRYPT_RUN_MSG = 4,

    CryptoAuth_DecryptErr_KEY_PKT_ESTABLISHED_SESSION = 5,

    CryptoAuth_DecryptErr_WRONG_PERM_PUBKEY = 6,


    CryptoAuth_DecryptErr_IP_RESTRICTED = 7,


    CryptoAuth_DecryptErr_AUTH_REQUIRED = 8,


    CryptoAuth_DecryptErr_UNRECOGNIZED_AUTH = 9,


    CryptoAuth_DecryptErr_STRAY_KEY = 10,

    CryptoAuth_DecryptErr_HANDSHAKE_DECRYPT_FAILED = 11,


    CryptoAuth_DecryptErr_WISEGUY = 12,





    CryptoAuth_DecryptErr_INVALID_PACKET = 13,


    CryptoAuth_DecryptErr_REPLAY = 14,


    CryptoAuth_DecryptErr_DECRYPT = 15
};



enum CryptoAuth_DecryptErr CryptoAuth_decrypt(struct CryptoAuth_Session* sess, struct Message* msg);
# 213 "./crypto/CryptoAuth.h"
void CryptoAuth_setAuth(const String* password,
                        const String* login,
                        struct CryptoAuth_Session* caSession);




void CryptoAuth_resetIfTimeout(struct CryptoAuth_Session* session);

void CryptoAuth_reset(struct CryptoAuth_Session* caSession);

enum CryptoAuth_State {

    CryptoAuth_State_INIT = 0,


    CryptoAuth_State_SENT_HELLO = 1,


    CryptoAuth_State_RECEIVED_HELLO = 2,


    CryptoAuth_State_SENT_KEY = 3,



    CryptoAuth_State_RECEIVED_KEY = 4,


    CryptoAuth_State_ESTABLISHED = 100
};

static inline char* CryptoAuth_stateString(int state)
{
    switch (state) {
        case CryptoAuth_State_INIT: return "INIT";
        case CryptoAuth_State_SENT_HELLO: return "SENT_HELLO";
        case CryptoAuth_State_RECEIVED_HELLO: return "RECEIVED_HELLO";
        case CryptoAuth_State_SENT_KEY: return "SENT_KEY";
        case CryptoAuth_State_RECEIVED_KEY: return "RECEIVED_KEY";
        case CryptoAuth_State_ESTABLISHED: return "ESTABLISHED";
        default: return "INVALID";
    }
}

enum CryptoAuth_State CryptoAuth_getState(struct CryptoAuth_Session* session);
# 18 "./crypto/CryptoAuth_pvt.h" 2





# 1 "./wire/CryptoHeader.h" 1
# 54 "./wire/CryptoHeader.h"
struct CryptoHeader_Challenge
{
    uint8_t type;
    uint8_t lookup[7];





    uint16_t requirePacketAuthAndDerivationCount;

    uint16_t additional;
};


void Assert_compileTime(char x[1 - (!(sizeof(struct CryptoHeader_Challenge) == 12))]);




static inline uint16_t CryptoHeader_getAuthChallengeDerivations(struct CryptoHeader_Challenge* ac)
{
    return ((uint16_t)(__extension__ ({ uint16_t _x = (((uint16_t)(ac->requirePacketAuthAndDerivationCount))); _x >> 8 | _x << 8; })))
        & (((uint16_t)~0)>>1);
}

static inline void CryptoHeader_setAuthChallengeDerivations(struct CryptoHeader_Challenge* ac,
                                                            uint16_t derivations)
{
    ac->requirePacketAuthAndDerivationCount = ((uint16_t)(__extension__ ({ uint16_t _x = (((uint16_t)(derivations))); _x >> 8 | _x << 8; })));
}
# 178 "./wire/CryptoHeader.h"
struct CryptoHeader
{




    uint32_t nonce;


    struct CryptoHeader_Challenge auth;


    uint8_t handshakeNonce[24];


    uint8_t publicKey[32];


    uint8_t authenticator[16];


    uint8_t encryptedTempKey[32];
};

void Assert_compileTime(char x[1 - (!(sizeof(struct CryptoHeader) == 120))]);
# 24 "./crypto/CryptoAuth_pvt.h" 2





struct CryptoAuth_User;
struct CryptoAuth_User {

    uint8_t passwordHash[8];


    uint8_t userNameHash[8];

    uint8_t secret[32];

    String* login;

    uint8_t restrictedToip6[16];

    struct CryptoAuth_User* next;

    struct Allocator* alloc;

    unsigned long Identity_verifier;
};

struct CryptoAuth_pvt
{
    struct CryptoAuth pub;

    uint8_t privateKey[32];

    struct CryptoAuth_User* users;

    struct Log* logger;
    struct EventBase* eventBase;

    struct Allocator* allocator;
    struct Random* rand;

    unsigned long Identity_verifier;
};


struct CryptoAuth_Session_pvt
{
    struct CryptoAuth_Session pub;

    struct Allocator* alloc;


    uint8_t sharedSecret[32];

    uint8_t herTempPubKey[32];

    uint8_t ourTempPrivKey[32];

    uint8_t ourTempPubKey[32];


    struct Allocator* passwdAlloc;
    String* password;


    struct Allocator* loginAlloc;
    String* login;


    uint32_t nextNonce;


    uint32_t timeOfLastPacket;


    int authType : 8;


    
# 101 "./crypto/CryptoAuth_pvt.h" 3 4
   _Bool 
# 101 "./crypto/CryptoAuth_pvt.h"
        isInitiator : 1;


    
# 104 "./crypto/CryptoAuth_pvt.h" 3 4
   _Bool 
# 104 "./crypto/CryptoAuth_pvt.h"
        requireAuth : 1;

    
# 106 "./crypto/CryptoAuth_pvt.h" 3 4
   _Bool 
# 106 "./crypto/CryptoAuth_pvt.h"
        established : 1;


    struct CryptoAuth_pvt* context;

    unsigned long Identity_verifier;
};
# 121 "./crypto/CryptoAuth_pvt.h"
int CryptoAuth_decryptRndNonce(uint8_t nonce[24], struct Message* msg, uint8_t secret[32]);

void CryptoAuth_encryptRndNonce(uint8_t nonce[24], struct Message* msg, uint8_t secret[32]);
# 21 "crypto/CryptoAuth.c" 2
# 1 "./crypto/AddressCalc.h" 1
# 19 "./crypto/AddressCalc.h"
struct Linker_x05220073449011975;
# 51 "./crypto/AddressCalc.h"

# 51 "./crypto/AddressCalc.h" 3 4
_Bool 
# 51 "./crypto/AddressCalc.h"
    AddressCalc_validAddress(const uint8_t address[16]);





void AddressCalc_makeValidAddress(uint8_t address[16]);
# 66 "./crypto/AddressCalc.h"

# 66 "./crypto/AddressCalc.h" 3 4
_Bool 
# 66 "./crypto/AddressCalc.h"
    AddressCalc_addressForPublicKey(uint8_t addressOut[16], const uint8_t key[32]);
# 22 "crypto/CryptoAuth.c" 2


# 1 "./benc/Dict.h" 1
# 21 "./benc/Dict.h"
struct Linker_x07043757570618474;

struct Dict_Entry;
struct Dict_Entry {
    struct Dict_Entry* next;
    String* key;
    Object* val;
};
# 37 "./benc/Dict.h"
int32_t Dict_remove(Dict* dictionary, const String* key);
# 47 "./benc/Dict.h"
int32_t Dict_size(const Dict* dictionary);
# 67 "./benc/Dict.h"
int64_t* Dict_getInt(const Dict* dictionary, const String* key);
# 79 "./benc/Dict.h"
String* Dict_getString(const Dict* dictionary, const String* key);
# 91 "./benc/Dict.h"
Dict* Dict_getDict(const Dict* dictionary, const String* key);
# 103 "./benc/Dict.h"
List* Dict_getList(const Dict* dictionary, const String* key);
# 119 "./benc/Dict.h"
Object* Dict_putInt(Dict* putIntoThis,
                    const String* key,
                    int64_t value,
                    struct Allocator* allocator);
# 138 "./benc/Dict.h"
Object* Dict_putString(Dict* putIntoThis,
                       const String* key,
                       String* value,
                       struct Allocator* allocator);
# 160 "./benc/Dict.h"
Object* Dict_putDict(Dict* putIntoThis,
                     const String* key,
                     Dict* value,
                     struct Allocator* allocator);
# 179 "./benc/Dict.h"
Object* Dict_putList(Dict* putIntoThis,
                     const String* key,
                     List* value,
                     struct Allocator* allocator);
# 194 "./benc/Dict.h"
Dict* Dict_new(struct Allocator* allocator);
# 25 "crypto/CryptoAuth.c" 2
# 1 "./benc/List.h" 1
# 21 "./benc/List.h"
struct Linker_x05916931193353123;

struct List_Item;
struct List_Item {
    struct List_Item* next;
    Object* elem;
};

List* List_new(struct Allocator* alloc);







int32_t List_size(const List* list);
# 46 "./benc/List.h"
void List_addInt(List* list, int64_t toAdd, struct Allocator* allocator);
# 56 "./benc/List.h"
void List_addString(List* list, String* toAdd, struct Allocator* allocator);
# 68 "./benc/List.h"
void List_addDict(List* list, Dict* toAdd, struct Allocator* allocator);
# 78 "./benc/List.h"
void List_addList(List* list, List* toAdd, struct Allocator* allocator);
# 88 "./benc/List.h"
int64_t* List_getInt(const List* list, uint32_t index);
# 98 "./benc/List.h"
String* List_getString(const List* list, uint32_t index);
# 108 "./benc/List.h"
Dict* List_getDict(const List* list, uint32_t index);
# 118 "./benc/List.h"
List* List_getList(const List* list, uint32_t index);
# 26 "crypto/CryptoAuth.c" 2




# 1 "./util/AddrTools.h" 1
# 19 "./util/AddrTools.h"
struct Linker_x07007093234621864;




void AddrTools_printPath(uint8_t out[20], uint64_t path);
# 33 "./util/AddrTools.h"
int AddrTools_parsePath(uint64_t* out, const uint8_t netAddr[20]);

void AddrTools_printIp(uint8_t output[40], const uint8_t binIp[16]);

void AddrTools_printShortIp(uint8_t output[40], const uint8_t binIp[16]);
# 47 "./util/AddrTools.h"
int AddrTools_parseIp(uint8_t out[16], const uint8_t hexAddr[40]);
# 57 "./util/AddrTools.h"
int AddrTools_parseMac(uint8_t out[6], const uint8_t hexAddr[17]);

void AddrTools_printMac(uint8_t output[18], const uint8_t binMac[6]);
# 31 "crypto/CryptoAuth.c" 2

# 1 "./util/Defined.h" 1
# 33 "crypto/CryptoAuth.c" 2

# 1 "./util/Hex.h" 1
# 21 "./util/Hex.h"
struct Linker_x02780030274155363;
# 32 "./util/Hex.h"
int Hex_encode(uint8_t* output,
               const uint32_t outputLength,
               const uint8_t* in,
               const uint32_t inputLength);

int Hex_decode(uint8_t* output,
               const uint32_t outLength,
               const uint8_t* in,
               const uint32_t inputLength);


# 42 "./util/Hex.h" 3 4
_Bool 
# 42 "./util/Hex.h"
    Hex_isHexEntity(const uint8_t character);

int Hex_decodeByte(const uint8_t highNibble, const uint8_t lowNibble);

uint8_t Hex_encodeLowNibble(const uint8_t nibble);

char* Hex_print(void* bytes, uint32_t length, struct Allocator* alloc);
# 35 "crypto/CryptoAuth.c" 2
# 1 "./util/events/Time.h" 1
# 20 "./util/events/Time.h"
struct Linker_x0552231083553018;




uint64_t Time_hrtime(void);



uint64_t Time_currentTimeMilliseconds(struct EventBase* eventBase);


uint64_t Time_currentTimeSeconds(struct EventBase* eventBase);
# 36 "crypto/CryptoAuth.c" 2
# 1 "./wire/Error.h" 1
# 51 "./wire/Error.h"
static inline char* Error_strerror(int err)
{
    switch (err) {
        case 0: return "Error_NONE";
        case 1: return "Error_MALFORMED_ADDRESS";
        case 2: return "Error_FLOOD";
        case 3: return "Error_LINK_LIMIT_EXCEEDED";
        case 4: return "Error_OVERSIZE_MESSAGE";
        case 5: return "Error_UNDERSIZE_MESSAGE";
        case 6: return "Error_AUTHENTICATION";
        case 7: return "Error_INVALID";
        case 8: return "Error_UNDELIVERABLE";
        case 9: return "Error_LOOP_ROUTE";
        case 10: return "Error_RETURN_PATH_INVALID";
        default: return "UNKNOWN";
    }
}
# 37 "crypto/CryptoAuth.c" 2
# 1 "./wire/Headers.h" 1
# 23 "./wire/Headers.h"
struct Headers_IP6Header
{
    uint16_t versionClassAndFlowLabel;


    uint16_t flowLabelLow_be;


    uint16_t payloadLength_be;

    uint8_t nextHeader;
    uint8_t hopLimit;
    uint8_t sourceAddr[16];
    uint8_t destinationAddr[16];
};

void Assert_compileTime(char x[1 - (!(sizeof(struct Headers_IP6Header) == 40))]);

struct Headers_IP6Fragment
{
    uint8_t nextHeader;
    uint8_t zero;
    uint16_t fragmentOffsetAndMoreFragments_be;
    uint32_t identifier;
};

void Assert_compileTime(char x[1 - (!(sizeof(struct Headers_IP6Fragment) == 8))]);


static inline uint32_t Headers_IP6Fragment_getOffset(struct Headers_IP6Fragment* frag)
{
    return ((uint16_t)(__extension__ ({ uint16_t _x = (((uint16_t)(frag->fragmentOffsetAndMoreFragments_be))); _x >> 8 | _x << 8; }))) >> 3;
}

static inline void Headers_IP6Fragment_setOffset(struct Headers_IP6Fragment* frag, uint16_t offset)
{
    frag->fragmentOffsetAndMoreFragments_be &= ((uint16_t)(__extension__ ({ uint16_t _x = (((uint16_t)(7))); _x >> 8 | _x << 8; })));
    frag->fragmentOffsetAndMoreFragments_be |= ((uint16_t)(__extension__ ({ uint16_t _x = (((uint16_t)(offset << 3))); _x >> 8 | _x << 8; })));
}

static inline int Headers_IP6Fragment_hasMoreFragments(struct Headers_IP6Fragment* frag)
{
    return frag->fragmentOffsetAndMoreFragments_be & ((uint16_t)(__extension__ ({ uint16_t _x = (((uint16_t)(1))); _x >> 8 | _x << 8; })));
}

static inline void Headers_IP6Fragment_setMoreFragments(struct Headers_IP6Fragment* frag, int more)
{
    if (more) {
        frag->fragmentOffsetAndMoreFragments_be |= ((uint16_t)(__extension__ ({ uint16_t _x = (((uint16_t)(1))); _x >> 8 | _x << 8; })));
    } else {
        frag->fragmentOffsetAndMoreFragments_be &= ((uint16_t)(__extension__ ({ uint16_t _x = (((uint16_t)(0xFFFF << 1))); _x >> 8 | _x << 8; })));
    }
}

struct Headers_IP4Header
{
    uint8_t versionAndHeaderLength;
    uint8_t differentiatedServices;
    uint16_t totalLength_be;
    uint16_t identification_be;
    uint16_t flagsAndFragmentOffset;
    uint8_t ttl;
    uint8_t protocol;
    uint16_t checksum_be;
    uint8_t sourceAddr[4];
    uint8_t destAddr[4];
};

void Assert_compileTime(char x[1 - (!(sizeof(struct Headers_IP4Header) == 20))]);

static inline int Headers_getIpVersion(void* header)
{
    return (((uint8_t*) header)[0] & 0xF0) >> 4;
}






struct Headers_UDPHeader {
    uint16_t srcPort_be;
    uint16_t destPort_be;
    uint16_t length_be;
    uint16_t checksum_be;
};

void Assert_compileTime(char x[1 - (!(sizeof(struct Headers_UDPHeader) == 8))]);

struct Headers_ICMP6Header {
    uint8_t type;
    uint8_t code;
    uint16_t checksum;
    uint32_t additional;
};

void Assert_compileTime(char x[1 - (!(sizeof(struct Headers_ICMP6Header) == 8))]);





struct Headers_Beacon {
    uint32_t version_be;
    uint8_t password[20];
    uint8_t publicKey[32];
};

void Assert_compileTime(char x[1 - (!(sizeof(struct Headers_Beacon) == 56))]);
# 38 "crypto/CryptoAuth.c" 2
# 49 "crypto/CryptoAuth.c"
enum Nonce {
    Nonce_HELLO = 0,
    Nonce_REPEAT_HELLO = 1,
    Nonce_KEY = 2,
    Nonce_REPEAT_KEY = 3,
    Nonce_FIRST_TRAFFIC_PACKET = 4
};

static inline void printHexKey(uint8_t output[65], uint8_t key[32])
{
    if (key) {
        Hex_encode(output, 65, key, 32);
    } else {
        Bits__memcpy(output,"NULL",5,"CryptoAuth.c",62);
    }
}

static inline void printHexPubKey(uint8_t output[65], uint8_t privateKey[32])
{
    if (privateKey) {
        uint8_t publicKey[32];
        crypto_scalarmult_base(publicKey, privateKey);
        printHexKey(output, publicKey);
    } else {
        printHexKey(output, 
# 73 "crypto/CryptoAuth.c" 3 4
                           ((void *)0)
# 73 "crypto/CryptoAuth.c"
                               );
    }
}
# 89 "crypto/CryptoAuth.c"
static inline void getSharedSecret(uint8_t outputSecret[32],
                                   uint8_t myPrivateKey[32],
                                   uint8_t herPublicKey[32],
                                   uint8_t passwordHash[32],
                                   struct Log* logger)
{
    if (passwordHash == 
# 95 "crypto/CryptoAuth.c" 3 4
                       ((void *)0)
# 95 "crypto/CryptoAuth.c"
                           ) {
        crypto_box_beforenm(outputSecret, herPublicKey, myPrivateKey);
    } else {
        union {
            struct {
                uint8_t key[32];
                uint8_t passwd[32];
            } components;
            uint8_t bytes[64];
        } buff;

        crypto_scalarmult(buff.components.key, myPrivateKey, herPublicKey);
        Bits__memcpy(buff.components.passwd,passwordHash,32,"CryptoAuth.c",107);
        crypto_hash_sha256(outputSecret, buff.bytes, 64);
    }
    if (0) {
        uint8_t myPublicKeyHex[65];
        printHexPubKey(myPublicKeyHex, myPrivateKey);
        uint8_t herPublicKeyHex[65];
        printHexKey(herPublicKeyHex, herPublicKey);
        uint8_t passwordHashHex[65];
        printHexKey(passwordHashHex, passwordHash);
        uint8_t outputSecretHex[65] = "NULL";
        printHexKey(outputSecretHex, outputSecret);
        do { if (logger && Log_Level_KEYS >= Log_Level_DEBUG) { Log_print(logger, Log_Level_KEYS, "CryptoAuth.c", 119, "Generated a shared secret:\n" "     myPublicKey=%s\n" "    herPublicKey=%s\n" "    passwordHash=%s\n" "    outputSecret=%s\n", myPublicKeyHex, herPublicKeyHex, passwordHashHex, outputSecretHex); } } while (0)





                                                                                    ;
    }
}

static inline void hashPassword(uint8_t secretOut[32],
                                struct CryptoHeader_Challenge* challengeOut,
                                const String* login,
                                const String* password,
                                const uint8_t authType)
{
    crypto_hash_sha256(secretOut, (uint8_t*) password->bytes, password->len);
    uint8_t tempBuff[32];
    if (authType == 1) {
        crypto_hash_sha256(tempBuff, secretOut, 32);
    } else if (authType == 2) {
        crypto_hash_sha256(tempBuff, (uint8_t*) login->bytes, login->len);
    } else {
        Assert_failure("Unsupported auth type [%u]", authType);
    }
    Bits__memcpy(challengeOut,tempBuff,12,"CryptoAuth.c",144);
    CryptoHeader_setAuthChallengeDerivations(challengeOut, 0);
    challengeOut->type = authType;
    challengeOut->additional = 0;
}
# 157 "crypto/CryptoAuth.c"
static inline struct CryptoAuth_User* getAuth(struct CryptoHeader_Challenge* auth,
                                              struct CryptoAuth_pvt* ca)
{
    if (auth->type == 0) {
        return 
# 161 "crypto/CryptoAuth.c" 3 4
              ((void *)0)
# 161 "crypto/CryptoAuth.c"
                  ;
    }
    int count = 0;
    for (struct CryptoAuth_User* u = ca->users; u; u = u->next) {
        count++;
        if (auth->type == 1 &&
            !__builtin_memcmp(auth,u->passwordHash,8))
        {
            return u;
        } else if (auth->type == 2 &&
            !__builtin_memcmp(auth,u->userNameHash,8))
        {
            return u;
        }
    }
    do { if (ca->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(ca->logger, Log_Level_DEBUG, "CryptoAuth.c", 176, "Got unrecognized auth, password count = [%d]", count); } } while (0);
    return 
# 177 "crypto/CryptoAuth.c" 3 4
          ((void *)0)
# 177 "crypto/CryptoAuth.c"
              ;
}
# 188 "crypto/CryptoAuth.c"
static inline __attribute__ ((warn_unused_result)) int decryptRndNonce(uint8_t nonce[24],
                                              struct Message* msg,
                                              uint8_t secret[32])
{
    if (msg->length < 16) {
        return -1;
    }
    do { if (!((msg->padding >= 16))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("CryptoAuth.c"), (195), "(msg->padding >= 16)"); } } while (0);
    uint8_t* startAt = msg->bytes - 16;
    uint8_t paddingSpace[16];
    Bits__memcpy(paddingSpace,startAt,16,"CryptoAuth.c",198);
    __builtin_memset(startAt,0,16);
    if (!0) {
        if (crypto_box_curve25519xsalsa20poly1305_tweet_open_afternm(
                startAt, startAt, msg->length + 16, nonce, secret) != 0)
        {
            return -1;
        }
    }

    Bits__memcpy(startAt,paddingSpace,16,"CryptoAuth.c",208);
    do {             struct Er_Ret* Er_ret = Message_eshift(msg, -16);                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("CryptoAuth.c", 209, Er_ret);                 } else {                     Er__assertFail(Er_unwind("CryptoAuth.c", 209, Er_ret));                 }             }                  } while (0);
    return 0;
}
# 221 "crypto/CryptoAuth.c"
static inline void encryptRndNonce(uint8_t nonce[24],
                                   struct Message* msg,
                                   uint8_t secret[32])
{
    do { if (!((msg->padding >= 32))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("CryptoAuth.c"), (225), "(msg->padding >= 32)"); } } while (0);
    uint8_t* startAt = msg->bytes - 32;

    uint8_t paddingSpace[16];
    Bits__memcpy(paddingSpace,startAt,16,"CryptoAuth.c",229);
    __builtin_memset(startAt,0,32);
    if (!0) {
        crypto_box_curve25519xsalsa20poly1305_tweet_afternm(
            startAt, startAt, msg->length + 32, nonce, secret);
    }

    Bits__memcpy(startAt,paddingSpace,16,"CryptoAuth.c",236);
    do {             struct Er_Ret* Er_ret = Message_eshift(msg, 16);                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("CryptoAuth.c", 237, Er_ret);                 } else {                     Er__assertFail(Er_unwind("CryptoAuth.c", 237, Er_ret));                 }             }                  } while (0);
}
# 248 "crypto/CryptoAuth.c"
static inline __attribute__ ((warn_unused_result)) int decrypt(uint32_t nonce,
                                      struct Message* msg,
                                      uint8_t secret[32],
                                      
# 251 "crypto/CryptoAuth.c" 3 4
                                     _Bool 
# 251 "crypto/CryptoAuth.c"
                                          isInitiator)
{
    union {
        uint32_t ints[2];
        uint8_t bytes[24];
    } nonceAs = { .ints = {0, 0} };
    nonceAs.ints[!isInitiator] = (nonce);

    return decryptRndNonce(nonceAs.bytes, msg, secret);
}
# 270 "crypto/CryptoAuth.c"
static inline void encrypt(uint32_t nonce,
                           struct Message* msg,
                           uint8_t secret[32],
                           
# 273 "crypto/CryptoAuth.c" 3 4
                          _Bool 
# 273 "crypto/CryptoAuth.c"
                               isInitiator)
{
    union {
        uint32_t ints[2];
        uint8_t bytes[24];
    } nonceAs = { .ints = {0, 0} };
    nonceAs.ints[isInitiator] = (nonce);

    encryptRndNonce(nonceAs.bytes, msg, secret);
}

static inline 
# 284 "crypto/CryptoAuth.c" 3 4
             _Bool 
# 284 "crypto/CryptoAuth.c"
                  knowHerKey(struct CryptoAuth_Session_pvt* session)
{
    return !Bits_isZero(session->pub.herPublicKey, 32);
}

static void getIp6(struct CryptoAuth_Session_pvt* session, uint8_t* addr)
{
    do { if (!((knowHerKey(session)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("CryptoAuth.c"), (291), "(knowHerKey(session))"); } } while (0);
    uint8_t ip6[16];
    AddressCalc_addressForPublicKey(ip6, session->pub.herPublicKey);
    AddrTools_printIp(addr, ip6);
}
# 311 "crypto/CryptoAuth.c"
static void reset(struct CryptoAuth_Session_pvt* session)
{
    session->nextNonce = CryptoAuth_State_INIT;
    session->isInitiator = 
# 314 "crypto/CryptoAuth.c" 3 4
                          0
# 314 "crypto/CryptoAuth.c"
                               ;

    __builtin_memset(session->ourTempPrivKey,0,32);
    __builtin_memset(session->ourTempPubKey,0,32);
    __builtin_memset(session->herTempPubKey,0,32);
    __builtin_memset(session->sharedSecret,0,32);
    session->established = 
# 320 "crypto/CryptoAuth.c" 3 4
                          0
# 320 "crypto/CryptoAuth.c"
                               ;

    __builtin_memset(&session->pub.replayProtector,0,sizeof(struct ReplayProtector));
}

static void resetIfTimeout(struct CryptoAuth_Session_pvt* session)
{
    if (session->nextNonce == CryptoAuth_State_SENT_HELLO) {



        return;
    }

    uint64_t nowSecs = Time_currentTimeSeconds(session->context->eventBase);
    if (nowSecs - session->timeOfLastPacket < session->pub.setupResetAfterInactivitySeconds) {
        return;
    } else if (nowSecs - session->timeOfLastPacket < session->pub.resetAfterInactivitySeconds) {
        if (session->established) { return; }
    }

    do { if (!1) { break; } uint8_t addr[40] = "unknown"; getIp6((session), addr); String* dn = (session)->pub.displayName; do { if ((session)->context->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print((session)->context->logger, Log_Level_DEBUG, "CryptoAuth.c", 341, "%p %s [%s] state[%d]: " "No traffic in [%d] seconds, resetting connection.", (void*)(session), dn ? dn->bytes : "", addr, (session)->nextNonce, (int) (nowSecs - session->timeOfLastPacket)); } } while (0); } while (0)
                                                          ;

    session->timeOfLastPacket = nowSecs;
    reset(session);
}

static void encryptHandshake(struct Message* message,
                             struct CryptoAuth_Session_pvt* session,
                             int setupMessage)
{
    do {             struct Er_Ret* Er_ret = Message_eshift(message, 120);                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("CryptoAuth.c", 352, Er_ret);                 } else {                     Er__assertFail(Er_unwind("CryptoAuth.c", 352, Er_ret));                 }             }                  } while (0);

    struct CryptoHeader* header = (struct CryptoHeader*) message->bytes;


    Random_bytes(session->context->rand, (uint8_t*) &header->auth,
                 12 + 24);


    Bits__memcpy(header->publicKey,session->context->pub.publicKey,32,"CryptoAuth.c",361);

    do { if (!((knowHerKey(session)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("CryptoAuth.c"), (363), "(knowHerKey(session))"); } } while (0);


    uint8_t* passwordHash = 
# 366 "crypto/CryptoAuth.c" 3 4
                           ((void *)0)
# 366 "crypto/CryptoAuth.c"
                               ;
    uint8_t passwordHashStore[32];
    if (session->password != 
# 368 "crypto/CryptoAuth.c" 3 4
                            ((void *)0)
# 368 "crypto/CryptoAuth.c"
                                ) {
        hashPassword(passwordHashStore,
                     &header->auth,
                     session->login,
                     session->password,
                     session->authType);
        passwordHash = passwordHashStore;
    } else {
        header->auth.type = session->authType;
        header->auth.additional = 0;
    }


    header->nonce = ((uint32_t)__builtin_bswap32(((uint32_t)(session->nextNonce))));

    if (session->nextNonce == CryptoAuth_State_INIT ||
        session->nextNonce == CryptoAuth_State_RECEIVED_HELLO)
    {


        Random_bytes(session->context->rand, session->ourTempPrivKey, 32);
        crypto_scalarmult_base(session->ourTempPubKey, session->ourTempPrivKey);

        if (0) {
            uint8_t tempPrivateKeyHex[65];
            Hex_encode(tempPrivateKeyHex, 65, session->ourTempPrivKey, 32);
            uint8_t tempPubKeyHex[65];
            Hex_encode(tempPubKeyHex, 65, session->ourTempPubKey, 32);
            do { if (session->context->logger && Log_Level_KEYS >= Log_Level_DEBUG) { Log_print(session->context->logger, Log_Level_KEYS, "CryptoAuth.c", 396, "Generating temporary keypair\n" "    myTempPrivateKey=%s\n" "     myTempPublicKey=%s\n", tempPrivateKeyHex, tempPubKeyHex); } } while (0)


                                                       ;
        }
    }

    Bits__memcpy(header->encryptedTempKey,session->ourTempPubKey,32,"CryptoAuth.c",403);

    if (0) {
        uint8_t tempKeyHex[65];
        Hex_encode(tempKeyHex, 65, header->encryptedTempKey, 32);
        do { if (session->context->logger && Log_Level_KEYS >= Log_Level_DEBUG) { Log_print(session->context->logger, Log_Level_KEYS, "CryptoAuth.c", 408, "Wrapping temp public key:\n" "    %s\n", tempKeyHex); } } while (0)


                             ;
    }

    do { if (!1) { break; } uint8_t addr[40] = "unknown"; getIp6((session), addr); String* dn = (session)->pub.displayName; do { if ((session)->context->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print((session)->context->logger, Log_Level_DEBUG, "CryptoAuth.c", 414, "%p %s [%s] state[%d]: " "Sending %s%s packet", (void*)(session), dn ? dn->bytes : "", addr, (session)->nextNonce, ((session->nextNonce & 1) ? "repeat " : ""), ((session->nextNonce < CryptoAuth_State_RECEIVED_HELLO) ? "hello" : "key")); } } while (0); } while (0)

                                                                                               ;

    uint8_t sharedSecret[32];
    if (session->nextNonce < CryptoAuth_State_RECEIVED_HELLO) {
        getSharedSecret(sharedSecret,
                        session->context->privateKey,
                        session->pub.herPublicKey,
                        passwordHash,
                        session->context->logger);

        session->isInitiator = 
# 426 "crypto/CryptoAuth.c" 3 4
                              1
# 426 "crypto/CryptoAuth.c"
                                  ;

        do { if (!((session->nextNonce <= CryptoAuth_State_SENT_HELLO))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("CryptoAuth.c"), (428), "(session->nextNonce <= CryptoAuth_State_SENT_HELLO)"); } } while (0);
        session->nextNonce = CryptoAuth_State_SENT_HELLO;
    } else {


        do { if (!((!Bits_isZero(session->herTempPubKey, 32)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("CryptoAuth.c"), (433), "(!Bits_isZero(session->herTempPubKey, 32))"); } } while (0);
        getSharedSecret(sharedSecret,
                        session->context->privateKey,
                        session->herTempPubKey,
                        passwordHash,
                        session->context->logger);

        do { if (!((session->nextNonce <= CryptoAuth_State_SENT_KEY))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("CryptoAuth.c"), (440), "(session->nextNonce <= CryptoAuth_State_SENT_KEY)"); } } while (0);
        session->nextNonce = CryptoAuth_State_SENT_KEY;

        if (0) {
            uint8_t tempKeyHex[65];
            Hex_encode(tempKeyHex, 65, session->herTempPubKey, 32);
            do { if (session->context->logger && Log_Level_KEYS >= Log_Level_DEBUG) { Log_print(session->context->logger, Log_Level_KEYS, "CryptoAuth.c", 446, "Using their temp public key:\n" "    %s\n", tempKeyHex); } } while (0)


                                 ;
        }
    }

    do { if (!(((session->nextNonce < CryptoAuth_State_RECEIVED_HELLO) == Bits_isZero(session->herTempPubKey, 32)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("CryptoAuth.c"), (453), "((session->nextNonce < CryptoAuth_State_RECEIVED_HELLO) == Bits_isZero(session->herTempPubKey, 32))"); } } while (0)
                                                        ;


    do {             struct Er_Ret* Er_ret = Message_eshift(message, 32 - 120);                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("CryptoAuth.c", 457, Er_ret);                 } else {                     Er__assertFail(Er_unwind("CryptoAuth.c", 457, Er_ret));                 }             }                  } while (0);

    encryptRndNonce(header->handshakeNonce, message, sharedSecret);

    if (0) {
        uint8_t sharedSecretHex[65];
        printHexKey(sharedSecretHex, sharedSecret);
        uint8_t nonceHex[49];
        Hex_encode(nonceHex, 49, header->handshakeNonce, 24);
        uint8_t cipherHex[65];
        printHexKey(cipherHex, message->bytes);
        do { if (session->context->logger && Log_Level_KEYS >= Log_Level_DEBUG) { Log_print(session->context->logger, Log_Level_KEYS, "CryptoAuth.c", 468, "Encrypting message with:\n" "    nonce: %s\n" "   secret: %s\n" "   cipher: %s\n", nonceHex, sharedSecretHex, cipherHex); } } while (0)




                                                       ;
    }


    do {             struct Er_Ret* Er_ret = Message_eshift(message, 120 - 32 - 16);                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("CryptoAuth.c", 477, Er_ret);                 } else {                     Er__assertFail(Er_unwind("CryptoAuth.c", 477, Er_ret));                 }             }                  } while (0);
}


int CryptoAuth_encrypt(struct CryptoAuth_Session* sessionPub, struct Message* msg)
{
    struct CryptoAuth_Session_pvt* session =
        (__extension__ ({ __typeof__((struct CryptoAuth_Session_pvt*) sessionPub) Identity_ptr = ((struct CryptoAuth_Session_pvt*) sessionPub); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0xc96f76410f348c21ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("CryptoAuth.c"), (484), "(Identity_ptr->Identity_verifier == ((unsigned long) 0xc96f76410f348c21ull))"); } } while (0); Identity_ptr; }));




    resetIfTimeout(session);


    if (session->nextNonce >= 0xfffffff0) {
        reset(session);
    }

    do { if (!((!((uintptr_t)msg->bytes % 4) || !"alignment fault"))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("CryptoAuth.c"), (496), "(!((uintptr_t)msg->bytes % 4) || !\"alignment fault\")"); } } while (0);
# 506 "crypto/CryptoAuth.c"
    if (session->nextNonce <= CryptoAuth_State_RECEIVED_KEY) {
        if (session->nextNonce < CryptoAuth_State_RECEIVED_KEY) {
            encryptHandshake(msg, session, 0);
            return 0;
        } else {
            do { if (!1) { break; } uint8_t addr[40] = "unknown"; getIp6((session), addr); String* dn = (session)->pub.displayName; do { if ((session)->context->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print((session)->context->logger, Log_Level_DEBUG, "CryptoAuth.c", 511, "%p %s [%s] state[%d]: " "Doing final step to send message. nonce=4" "%s", (void*)(session), dn ? dn->bytes : "", addr, (session)->nextNonce, ""); } } while (0); } while (0);
            do { if (!((!Bits_isZero(session->ourTempPrivKey, 32)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("CryptoAuth.c"), (512), "(!Bits_isZero(session->ourTempPrivKey, 32))"); } } while (0);
            do { if (!((!Bits_isZero(session->herTempPubKey, 32)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("CryptoAuth.c"), (513), "(!Bits_isZero(session->herTempPubKey, 32))"); } } while (0);
            getSharedSecret(session->sharedSecret,
                            session->ourTempPrivKey,
                            session->herTempPubKey,
                            
# 517 "crypto/CryptoAuth.c" 3 4
                           ((void *)0)
# 517 "crypto/CryptoAuth.c"
                               ,
                            session->context->logger);
        }
    }

    do { if (!((msg->length > 0 && "Empty packet during handshake"))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("CryptoAuth.c"), (522), "(msg->length > 0 && \"Empty packet during handshake\")"); } } while (0);
    do { if (!((msg->padding >= 36 || !"not enough padding"))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("CryptoAuth.c"), (523), "(msg->padding >= 36 || !\"not enough padding\")"); } } while (0);

    encrypt(session->nextNonce, msg, session->sharedSecret, session->isInitiator);

    do {             struct Er_Ret* Er_ret = Message_epush32h(msg, ((uint32_t)__builtin_bswap32(((uint32_t)(session->nextNonce)))));                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("CryptoAuth.c", 527, Er_ret);                 } else {                     Er__assertFail(Er_unwind("CryptoAuth.c", 527, Er_ret));                 }             }                  } while (0);
    session->nextNonce++;
    return 0;
}


static inline void updateTime(struct CryptoAuth_Session_pvt* session, struct Message* message)
{
    session->timeOfLastPacket = Time_currentTimeSeconds(session->context->eventBase);
}

static inline enum CryptoAuth_DecryptErr decryptMessage(struct CryptoAuth_Session_pvt* session,
                                                        uint32_t nonce,
                                                        struct Message* content,
                                                        uint8_t secret[32])
{

    if (decrypt(nonce, content, secret, session->isInitiator)) {
        do { if (!1) { break; } uint8_t addr[40] = "unknown"; getIp6((session), addr); String* dn = (session)->pub.displayName; do { if ((session)->context->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print((session)->context->logger, Log_Level_DEBUG, "CryptoAuth.c", 545, "%p %s [%s] state[%d]: " "DROP authenticated decryption failed" "%s", (void*)(session), dn ? dn->bytes : "", addr, (session)->nextNonce, ""); } } while (0); } while (0);
        return CryptoAuth_DecryptErr_DECRYPT;
    }
    if (!ReplayProtector_checkNonce(nonce, &session->pub.replayProtector)) {
        do { if (!1) { break; } uint8_t addr[40] = "unknown"; getIp6((session), addr); String* dn = (session)->pub.displayName; do { if ((session)->context->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print((session)->context->logger, Log_Level_DEBUG, "CryptoAuth.c", 549, "%p %s [%s] state[%d]: " "DROP nonce checking failed nonce=[%u]", (void*)(session), dn ? dn->bytes : "", addr, (session)->nextNonce, nonce); } } while (0); } while (0);
        return CryptoAuth_DecryptErr_REPLAY;
    }
    return 0;
}

static 
# 555 "crypto/CryptoAuth.c" 3 4
      _Bool 
# 555 "crypto/CryptoAuth.c"
           ip6MatchesKey(uint8_t ip6[16], uint8_t key[32])
{
    uint8_t calculatedIp6[16];
    AddressCalc_addressForPublicKey(calculatedIp6, key);
    return !__builtin_memcmp(ip6,calculatedIp6,16);
}

static enum CryptoAuth_DecryptErr decryptHandshake(struct CryptoAuth_Session_pvt* session,
                                                   const uint32_t nonce,
                                                   struct Message* message,
                                                   struct CryptoHeader* header)
{
    if (message->length < 120) {
        do { if (!1) { break; } uint8_t addr[40] = "unknown"; getIp6((session), addr); String* dn = (session)->pub.displayName; do { if ((session)->context->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print((session)->context->logger, Log_Level_DEBUG, "CryptoAuth.c", 568, "%p %s [%s] state[%d]: " "DROP runt" "%s", (void*)(session), dn ? dn->bytes : "", addr, (session)->nextNonce, ""); } } while (0); } while (0);
        return CryptoAuth_DecryptErr_RUNT;
    }
# 579 "crypto/CryptoAuth.c"
    do { if (!((knowHerKey(session)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("CryptoAuth.c"), (579), "(knowHerKey(session))"); } } while (0);
    if (__builtin_memcmp(session->pub.herPublicKey,header->publicKey,32)) {
        do { if (!1) { break; } uint8_t addr[40] = "unknown"; getIp6((session), addr); String* dn = (session)->pub.displayName; do { if ((session)->context->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print((session)->context->logger, Log_Level_DEBUG, "CryptoAuth.c", 581, "%p %s [%s] state[%d]: " "DROP a packet with different public key than this session" "%s", (void*)(session), dn ? dn->bytes : "", addr, (session)->nextNonce, ""); } } while (0); } while (0);
        return CryptoAuth_DecryptErr_WRONG_PERM_PUBKEY;
    }

    do { if (!(((session->nextNonce < CryptoAuth_State_RECEIVED_HELLO) == Bits_isZero(session->herTempPubKey, 32)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("CryptoAuth.c"), (585), "((session->nextNonce < CryptoAuth_State_RECEIVED_HELLO) == Bits_isZero(session->herTempPubKey, 32))"); } } while (0)
                                                        ;

    struct CryptoAuth_User* userObj = getAuth(&header->auth, session->context);
    uint8_t* restrictedToip6 = 
# 589 "crypto/CryptoAuth.c" 3 4
                              ((void *)0)
# 589 "crypto/CryptoAuth.c"
                                  ;
    uint8_t* passwordHash = 
# 590 "crypto/CryptoAuth.c" 3 4
                           ((void *)0)
# 590 "crypto/CryptoAuth.c"
                               ;
    if (userObj) {
        passwordHash = userObj->secret;
        if (userObj->restrictedToip6[0]) {
            restrictedToip6 = userObj->restrictedToip6;
            if (!ip6MatchesKey(restrictedToip6, session->pub.herPublicKey)) {
                do { if (!1) { break; } uint8_t addr[40] = "unknown"; getIp6((session), addr); String* dn = (session)->pub.displayName; do { if ((session)->context->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print((session)->context->logger, Log_Level_DEBUG, "CryptoAuth.c", 596, "%p %s [%s] state[%d]: " "DROP packet with key not matching restrictedToip6" "%s", (void*)(session), dn ? dn->bytes : "", addr, (session)->nextNonce, ""); } } while (0); } while (0);
                return CryptoAuth_DecryptErr_IP_RESTRICTED;
            }
        }
    }
    if (session->requireAuth && !userObj) {
        do { if (!1) { break; } uint8_t addr[40] = "unknown"; getIp6((session), addr); String* dn = (session)->pub.displayName; do { if ((session)->context->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print((session)->context->logger, Log_Level_DEBUG, "CryptoAuth.c", 602, "%p %s [%s] state[%d]: " "DROP message because auth was not given" "%s", (void*)(session), dn ? dn->bytes : "", addr, (session)->nextNonce, ""); } } while (0); } while (0);
        return CryptoAuth_DecryptErr_AUTH_REQUIRED;
    }
    if (!userObj && header->auth.type != 0) {
        do { if (!1) { break; } uint8_t addr[40] = "unknown"; getIp6((session), addr); String* dn = (session)->pub.displayName; do { if ((session)->context->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print((session)->context->logger, Log_Level_DEBUG, "CryptoAuth.c", 606, "%p %s [%s] state[%d]: " "DROP message with unrecognized authenticator" "%s", (void*)(session), dn ? dn->bytes : "", addr, (session)->nextNonce, ""); } } while (0); } while (0);
        return CryptoAuth_DecryptErr_UNRECOGNIZED_AUTH;
    }

    uint32_t nextNonce;


    uint8_t sharedSecret[32];

    if (nonce < Nonce_KEY) {
        if (nonce == Nonce_HELLO) {
            do { if (!1) { break; } uint8_t addr[40] = "unknown"; getIp6((session), addr); String* dn = (session)->pub.displayName; do { if ((session)->context->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print((session)->context->logger, Log_Level_DEBUG, "CryptoAuth.c", 617, "%p %s [%s] state[%d]: " "Received a hello packet, using auth: %d", (void*)(session), dn ? dn->bytes : "", addr, (session)->nextNonce, (userObj != 
# 617 "crypto/CryptoAuth.c" 3 4
           ((void *)0)
# 617 "crypto/CryptoAuth.c"
           )); } } while (0); } while (0)
                                              ;
        } else {
            do { if (!((nonce == Nonce_REPEAT_HELLO))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("CryptoAuth.c"), (620), "(nonce == Nonce_REPEAT_HELLO)"); } } while (0);
            do { if (!1) { break; } uint8_t addr[40] = "unknown"; getIp6((session), addr); String* dn = (session)->pub.displayName; do { if ((session)->context->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print((session)->context->logger, Log_Level_DEBUG, "CryptoAuth.c", 621, "%p %s [%s] state[%d]: " "Received a repeat hello packet" "%s", (void*)(session), dn ? dn->bytes : "", addr, (session)->nextNonce, ""); } } while (0); } while (0);
        }

        getSharedSecret(sharedSecret,
                        session->context->privateKey,
                        session->pub.herPublicKey,
                        passwordHash,
                        session->context->logger);
        nextNonce = CryptoAuth_State_RECEIVED_HELLO;
    } else {
        if (nonce == Nonce_KEY) {
            do { if (!1) { break; } uint8_t addr[40] = "unknown"; getIp6((session), addr); String* dn = (session)->pub.displayName; do { if ((session)->context->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print((session)->context->logger, Log_Level_DEBUG, "CryptoAuth.c", 632, "%p %s [%s] state[%d]: " "Received a key packet" "%s", (void*)(session), dn ? dn->bytes : "", addr, (session)->nextNonce, ""); } } while (0); } while (0);
        } else {
            do { if (!((nonce == Nonce_REPEAT_KEY))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("CryptoAuth.c"), (634), "(nonce == Nonce_REPEAT_KEY)"); } } while (0);
            do { if (!1) { break; } uint8_t addr[40] = "unknown"; getIp6((session), addr); String* dn = (session)->pub.displayName; do { if ((session)->context->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print((session)->context->logger, Log_Level_DEBUG, "CryptoAuth.c", 635, "%p %s [%s] state[%d]: " "Received a repeat key packet" "%s", (void*)(session), dn ? dn->bytes : "", addr, (session)->nextNonce, ""); } } while (0); } while (0);
        }
        if (!session->isInitiator) {
            do { if (!1) { break; } uint8_t addr[40] = "unknown"; getIp6((session), addr); String* dn = (session)->pub.displayName; do { if ((session)->context->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print((session)->context->logger, Log_Level_DEBUG, "CryptoAuth.c", 638, "%p %s [%s] state[%d]: " "DROP a stray key packet" "%s", (void*)(session), dn ? dn->bytes : "", addr, (session)->nextNonce, ""); } } while (0); } while (0);
            return CryptoAuth_DecryptErr_STRAY_KEY;
        }

        getSharedSecret(sharedSecret,
                        session->ourTempPrivKey,
                        session->pub.herPublicKey,
                        passwordHash,
                        session->context->logger);
        nextNonce = CryptoAuth_State_RECEIVED_KEY;
    }


    do {             struct Er_Ret* Er_ret = Message_eshift(message, 48 - 120);                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("CryptoAuth.c", 651, Er_ret);                 } else {                     Er__assertFail(Er_unwind("CryptoAuth.c", 651, Er_ret));                 }             }                  } while (0);

    if (0) {
        uint8_t sharedSecretHex[65];
        printHexKey(sharedSecretHex, sharedSecret);
        uint8_t nonceHex[49];
        Hex_encode(nonceHex, 49, header->handshakeNonce, 24);
        uint8_t cipherHex[65];
        printHexKey(cipherHex, message->bytes);
        do { if (session->context->logger && Log_Level_KEYS >= Log_Level_DEBUG) { Log_print(session->context->logger, Log_Level_KEYS, "CryptoAuth.c", 660, "Decrypting message with:\n" "    nonce: %s\n" "   secret: %s\n" "   cipher: %s\n", nonceHex, sharedSecretHex, cipherHex); } } while (0)




                                                       ;
    }


    if (decryptRndNonce(header->handshakeNonce, message, sharedSecret)) {

        __builtin_memset(header,0,120);
        do { if (!1) { break; } uint8_t addr[40] = "unknown"; getIp6((session), addr); String* dn = (session)->pub.displayName; do { if ((session)->context->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print((session)->context->logger, Log_Level_DEBUG, "CryptoAuth.c", 672, "%p %s [%s] state[%d]: " "DROP message with nonce [%d], decryption failed", (void*)(session), dn ? dn->bytes : "", addr, (session)->nextNonce, nonce); } } while (0); } while (0);
        return CryptoAuth_DecryptErr_HANDSHAKE_DECRYPT_FAILED;
    }

    if (Bits_isZero(header->encryptedTempKey, 32)) {

        do { if (!1) { break; } uint8_t addr[40] = "unknown"; getIp6((session), addr); String* dn = (session)->pub.displayName; do { if ((session)->context->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print((session)->context->logger, Log_Level_DEBUG, "CryptoAuth.c", 678, "%p %s [%s] state[%d]: " "DROP message with zero as temp public key" "%s", (void*)(session), dn ? dn->bytes : "", addr, (session)->nextNonce, ""); } } while (0); } while (0);
        return CryptoAuth_DecryptErr_WISEGUY;
    }

    if (0) {
        uint8_t tempKeyHex[65];
        Hex_encode(tempKeyHex, 65, header->encryptedTempKey, 32);
        do { if (session->context->logger && Log_Level_KEYS >= Log_Level_DEBUG) { Log_print(session->context->logger, Log_Level_KEYS, "CryptoAuth.c", 685, "Unwrapping temp public key:\n" "    %s\n", tempKeyHex); } } while (0)


                             ;
    }

    do {             struct Er_Ret* Er_ret = Message_eshift(message, -32);                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("CryptoAuth.c", 691, Er_ret);                 } else {                     Er__assertFail(Er_unwind("CryptoAuth.c", 691, Er_ret));                 }             }                  } while (0);


    if (nonce == Nonce_HELLO) {

        if (!__builtin_memcmp(session->herTempPubKey,header->encryptedTempKey,32)) {

            do { if (!1) { break; } uint8_t addr[40] = "unknown"; getIp6((session), addr); String* dn = (session)->pub.displayName; do { if ((session)->context->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print((session)->context->logger, Log_Level_DEBUG, "CryptoAuth.c", 698, "%p %s [%s] state[%d]: " "DROP dupe hello packet with same temp key" "%s", (void*)(session), dn ? dn->bytes : "", addr, (session)->nextNonce, ""); } } while (0); } while (0);
            return CryptoAuth_DecryptErr_INVALID_PACKET;
        }
    } else if (nonce == Nonce_KEY && session->nextNonce >= CryptoAuth_State_RECEIVED_KEY) {



        if (!__builtin_memcmp(session->herTempPubKey,header->encryptedTempKey,32)) {
            do { if (!((!Bits_isZero(session->herTempPubKey, 32)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("CryptoAuth.c"), (706), "(!Bits_isZero(session->herTempPubKey, 32))"); } } while (0);
            do { if (!1) { break; } uint8_t addr[40] = "unknown"; getIp6((session), addr); String* dn = (session)->pub.displayName; do { if ((session)->context->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print((session)->context->logger, Log_Level_DEBUG, "CryptoAuth.c", 707, "%p %s [%s] state[%d]: " "DROP dupe key packet with same temp key" "%s", (void*)(session), dn ? dn->bytes : "", addr, (session)->nextNonce, ""); } } while (0); } while (0);
            return CryptoAuth_DecryptErr_INVALID_PACKET;
        }

    } else if (nonce == Nonce_REPEAT_KEY && session->nextNonce >= CryptoAuth_State_RECEIVED_KEY) {

        if (__builtin_memcmp(session->herTempPubKey,header->encryptedTempKey,32)) {
            do { if (!((!Bits_isZero(session->herTempPubKey, 32)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("CryptoAuth.c"), (714), "(!Bits_isZero(session->herTempPubKey, 32))"); } } while (0);
            do { if (!1) { break; } uint8_t addr[40] = "unknown"; getIp6((session), addr); String* dn = (session)->pub.displayName; do { if ((session)->context->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print((session)->context->logger, Log_Level_DEBUG, "CryptoAuth.c", 715, "%p %s [%s] state[%d]: " "DROP repeat key packet with different temp key" "%s", (void*)(session), dn ? dn->bytes : "", addr, (session)->nextNonce, ""); } } while (0); } while (0);
            return CryptoAuth_DecryptErr_INVALID_PACKET;
        }
    }
# 727 "crypto/CryptoAuth.c"
    if (nextNonce == CryptoAuth_State_RECEIVED_KEY) {
        do { if (!((nonce == Nonce_KEY || nonce == Nonce_REPEAT_KEY))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("CryptoAuth.c"), (728), "(nonce == Nonce_KEY || nonce == Nonce_REPEAT_KEY)"); } } while (0);
        switch (session->nextNonce) {
            case CryptoAuth_State_INIT:
            case CryptoAuth_State_RECEIVED_HELLO:
            case CryptoAuth_State_SENT_KEY: {
                do { if (!1) { break; } uint8_t addr[40] = "unknown"; getIp6((session), addr); String* dn = (session)->pub.displayName; do { if ((session)->context->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print((session)->context->logger, Log_Level_DEBUG, "CryptoAuth.c", 733, "%p %s [%s] state[%d]: " "DROP stray key packet" "%s", (void*)(session), dn ? dn->bytes : "", addr, (session)->nextNonce, ""); } } while (0); } while (0);
                return CryptoAuth_DecryptErr_STRAY_KEY;
            }
            case CryptoAuth_State_SENT_HELLO: {
                Bits__memcpy(session->herTempPubKey,header->encryptedTempKey,32,"CryptoAuth.c",737);
                break;
            }
            case CryptoAuth_State_RECEIVED_KEY: {
                if (nonce == Nonce_KEY) {
                    Bits__memcpy(session->herTempPubKey,header->encryptedTempKey,32,"CryptoAuth.c",742);
                } else {
                    do { if (!((!__builtin_memcmp(session->herTempPubKey,header->encryptedTempKey,32)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("CryptoAuth.c"), (744), "(!__builtin_memcmp(session->herTempPubKey,header->encryptedTempKey,32))"); } } while (0);
                }
                break;
            }
            default: {
                do { if (!((!session->established))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("CryptoAuth.c"), (749), "(!session->established)"); } } while (0);
                if (nonce == Nonce_KEY) {
                    Bits__memcpy(session->herTempPubKey,header->encryptedTempKey,32,"CryptoAuth.c",751);
                    do { if (!1) { break; } uint8_t addr[40] = "unknown"; getIp6((session), addr); String* dn = (session)->pub.displayName; do { if ((session)->context->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print((session)->context->logger, Log_Level_DEBUG, "CryptoAuth.c", 752, "%p %s [%s] state[%d]: " "New key packet, recalculating shared secret" "%s", (void*)(session), dn ? dn->bytes : "", addr, (session)->nextNonce, ""); } } while (0); } while (0);
                    do { if (!((!Bits_isZero(session->ourTempPrivKey, 32)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("CryptoAuth.c"), (753), "(!Bits_isZero(session->ourTempPrivKey, 32))"); } } while (0);
                    do { if (!((!Bits_isZero(session->herTempPubKey, 32)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("CryptoAuth.c"), (754), "(!Bits_isZero(session->herTempPubKey, 32))"); } } while (0);
                    getSharedSecret(session->sharedSecret,
                                    session->ourTempPrivKey,
                                    session->herTempPubKey,
                                    
# 758 "crypto/CryptoAuth.c" 3 4
                                   ((void *)0)
# 758 "crypto/CryptoAuth.c"
                                       ,
                                    session->context->logger);
                } else {
                    do { if (!((!__builtin_memcmp(session->herTempPubKey,header->encryptedTempKey,32)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("CryptoAuth.c"), (761), "(!__builtin_memcmp(session->herTempPubKey,header->encryptedTempKey,32))"); } } while (0);
                }
                nextNonce = session->nextNonce + 1;
                do { if (!1) { break; } uint8_t addr[40] = "unknown"; getIp6((session), addr); String* dn = (session)->pub.displayName; do { if ((session)->context->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print((session)->context->logger, Log_Level_DEBUG, "CryptoAuth.c", 764, "%p %s [%s] state[%d]: " "New key packet but we are already sending data" "%s", (void*)(session), dn ? dn->bytes : "", addr, (session)->nextNonce, ""); } } while (0); } while (0);
            }
        }

    } else if (nextNonce == CryptoAuth_State_RECEIVED_HELLO) {
        do { if (!((nonce == Nonce_HELLO || nonce == Nonce_REPEAT_HELLO))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("CryptoAuth.c"), (769), "(nonce == Nonce_HELLO || nonce == Nonce_REPEAT_HELLO)"); } } while (0);
        if (__builtin_memcmp(session->herTempPubKey,header->encryptedTempKey,32)) {

            switch (session->nextNonce) {
                case CryptoAuth_State_SENT_HELLO: {
                    if (__builtin_memcmp(session->pub.herPublicKey,session->context->pub.publicKey,32)
                                                                         < 0)
                    {




                        do { if (!1) { break; } uint8_t addr[40] = "unknown"; getIp6((session), addr); String* dn = (session)->pub.displayName; do { if ((session)->context->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print((session)->context->logger, Log_Level_DEBUG, "CryptoAuth.c", 781, "%p %s [%s] state[%d]: " "Incoming hello from node with lower key, resetting" "%s", (void*)(session), dn ? dn->bytes : "", addr, (session)->nextNonce, ""); } } while (0); } while (0)
                                                                                 ;
                        reset(session);

                        Bits__memcpy(session->herTempPubKey,header->encryptedTempKey,32,"CryptoAuth.c",785);
                        break;
                    } else {




                        do { if (!1) { break; } uint8_t addr[40] = "unknown"; getIp6((session), addr); String* dn = (session)->pub.displayName; do { if ((session)->context->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print((session)->context->logger, Log_Level_DEBUG, "CryptoAuth.c", 792, "%p %s [%s] state[%d]: " "Incoming hello from node with higher key, not resetting" "%s", (void*)(session), dn ? dn->bytes : "", addr, (session)->nextNonce, ""); } } while (0); } while (0)
                                                                                      ;
                        return 0;
                    }
                }
                case CryptoAuth_State_INIT: {
                    Bits__memcpy(session->herTempPubKey,header->encryptedTempKey,32,"CryptoAuth.c",798);
                    break;
                }
                default: {
                    do { if (!1) { break; } uint8_t addr[40] = "unknown"; getIp6((session), addr); String* dn = (session)->pub.displayName; do { if ((session)->context->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print((session)->context->logger, Log_Level_DEBUG, "CryptoAuth.c", 802, "%p %s [%s] state[%d]: " "Incoming hello packet resetting session" "%s", (void*)(session), dn ? dn->bytes : "", addr, (session)->nextNonce, ""); } } while (0); } while (0);
                    reset(session);
                    Bits__memcpy(session->herTempPubKey,header->encryptedTempKey,32,"CryptoAuth.c",804);
                    break;
                }
            }
        } else {

            switch (session->nextNonce) {
                case CryptoAuth_State_RECEIVED_HELLO:
                case CryptoAuth_State_SENT_KEY: {
                    nextNonce = session->nextNonce;
                    break;
                }
                default: {
                    do { if (!1) { break; } uint8_t addr[40] = "unknown"; getIp6((session), addr); String* dn = (session)->pub.displayName; do { if ((session)->context->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print((session)->context->logger, Log_Level_DEBUG, "CryptoAuth.c", 817, "%p %s [%s] state[%d]: " "DROP Incoming repeat hello" "%s", (void*)(session), dn ? dn->bytes : "", addr, (session)->nextNonce, ""); } } while (0); } while (0);




                    return CryptoAuth_DecryptErr_INVALID_PACKET;
                }
            }
        }
    } else {
        Assert_failure("should never happen");
    }


    do { if (!((session->nextNonce < nextNonce || (session->nextNonce <= CryptoAuth_State_RECEIVED_KEY && nextNonce == session->nextNonce)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("CryptoAuth.c"), (831), "(session->nextNonce < nextNonce || (session->nextNonce <= CryptoAuth_State_RECEIVED_KEY && nextNonce == session->nextNonce))"); } } while (0)

     ;
    session->nextNonce = nextNonce;

    __builtin_memset(&session->pub.replayProtector,0,sizeof(struct ReplayProtector));

    return 0;
}


enum CryptoAuth_DecryptErr CryptoAuth_decrypt(struct CryptoAuth_Session* sessionPub,
                                              struct Message* msg)
{
    struct CryptoAuth_Session_pvt* session =
        (__extension__ ({ __typeof__((struct CryptoAuth_Session_pvt*) sessionPub) Identity_ptr = ((struct CryptoAuth_Session_pvt*) sessionPub); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0xc96f76410f348c21ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("CryptoAuth.c"), (846), "(Identity_ptr->Identity_verifier == ((unsigned long) 0xc96f76410f348c21ull))"); } } while (0); Identity_ptr; }));
    struct CryptoHeader* header = (struct CryptoHeader*) msg->bytes;

    if (msg->length < 20) {
        do { if (!1) { break; } uint8_t addr[40] = "unknown"; getIp6((session), addr); String* dn = (session)->pub.displayName; do { if ((session)->context->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print((session)->context->logger, Log_Level_DEBUG, "CryptoAuth.c", 850, "%p %s [%s] state[%d]: " "DROP runt" "%s", (void*)(session), dn ? dn->bytes : "", addr, (session)->nextNonce, ""); } } while (0); } while (0);
        return CryptoAuth_DecryptErr_RUNT;
    }
    do { if (!((msg->padding >= 12 || "need at least 12 bytes of padding in incoming message"))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("CryptoAuth.c"), (853), "(msg->padding >= 12 || \"need at least 12 bytes of padding in incoming message\")"); } } while (0);
    do { if (!((!((uintptr_t)msg->bytes % 4) || !"alignment fault"))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("CryptoAuth.c"), (854), "(!((uintptr_t)msg->bytes % 4) || !\"alignment fault\")"); } } while (0);
    do { if (!((!(msg->capacity % 4) || !"length fault"))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("CryptoAuth.c"), (855), "(!(msg->capacity % 4) || !\"length fault\")"); } } while (0);

    do {             struct Er_Ret* Er_ret = Message_eshift(msg, -4);                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("CryptoAuth.c", 857, Er_ret);                 } else {                     Er__assertFail(Er_unwind("CryptoAuth.c", 857, Er_ret));                 }             }                  } while (0);

    uint32_t nonce = ((uint32_t)__builtin_bswap32(((uint32_t)(header->nonce))));

    if (!session->established) {
        if (nonce >= Nonce_FIRST_TRAFFIC_PACKET) {
            if (session->nextNonce < CryptoAuth_State_SENT_KEY) {

                do { if (!1) { break; } uint8_t addr[40] = "unknown"; getIp6((session), addr); String* dn = (session)->pub.displayName; do { if ((session)->context->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print((session)->context->logger, Log_Level_DEBUG, "CryptoAuth.c", 865, "%p %s [%s] state[%d]: " "DROP Received a run message to an un-setup session" "%s", (void*)(session), dn ? dn->bytes : "", addr, (session)->nextNonce, ""); } } while (0); } while (0);
                return CryptoAuth_DecryptErr_NO_SESSION;
            }
            do { if (!1) { break; } uint8_t addr[40] = "unknown"; getIp6((session), addr); String* dn = (session)->pub.displayName; do { if ((session)->context->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print((session)->context->logger, Log_Level_DEBUG, "CryptoAuth.c", 868, "%p %s [%s] state[%d]: " "Trying final handshake step, nonce=%u\n", (void*)(session), dn ? dn->bytes : "", addr, (session)->nextNonce, nonce); } } while (0); } while (0);
            uint8_t secret[32];
            do { if (!((!Bits_isZero(session->ourTempPrivKey, 32)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("CryptoAuth.c"), (870), "(!Bits_isZero(session->ourTempPrivKey, 32))"); } } while (0);
            do { if (!((!Bits_isZero(session->herTempPubKey, 32)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("CryptoAuth.c"), (871), "(!Bits_isZero(session->herTempPubKey, 32))"); } } while (0);
            getSharedSecret(secret,
                            session->ourTempPrivKey,
                            session->herTempPubKey,
                            
# 875 "crypto/CryptoAuth.c" 3 4
                           ((void *)0)
# 875 "crypto/CryptoAuth.c"
                               ,
                            session->context->logger);

            enum CryptoAuth_DecryptErr ret = decryptMessage(session, nonce, msg, secret);


            session->pub.replayProtector.baseOffset = nonce + 1;
            session->pub.replayProtector.bitfield = 0;

            if (!ret) {
                do { if (!1) { break; } uint8_t addr[40] = "unknown"; getIp6((session), addr); String* dn = (session)->pub.displayName; do { if ((session)->context->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print((session)->context->logger, Log_Level_DEBUG, "CryptoAuth.c", 885, "%p %s [%s] state[%d]: " "Final handshake step succeeded" "%s", (void*)(session), dn ? dn->bytes : "", addr, (session)->nextNonce, ""); } } while (0); } while (0);
                Bits__memcpy(session->sharedSecret,secret,32,"CryptoAuth.c",886);


                session->established = 
# 889 "crypto/CryptoAuth.c" 3 4
                                      1
# 889 "crypto/CryptoAuth.c"
                                          ;
                session->nextNonce += 3;
                updateTime(session, msg);
                return 0;
            }
            do { if (!1) { break; } uint8_t addr[40] = "unknown"; getIp6((session), addr); String* dn = (session)->pub.displayName; do { if ((session)->context->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print((session)->context->logger, Log_Level_DEBUG, "CryptoAuth.c", 894, "%p %s [%s] state[%d]: " "DROP Final handshake step failed" "%s", (void*)(session), dn ? dn->bytes : "", addr, (session)->nextNonce, ""); } } while (0); } while (0);
            return ret;
        }

        do {             struct Er_Ret* Er_ret = Message_eshift(msg, 4);                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("CryptoAuth.c", 898, Er_ret);                 } else {                     Er__assertFail(Er_unwind("CryptoAuth.c", 898, Er_ret));                 }             }                  } while (0);
        return decryptHandshake(session, nonce, msg, header);

    } else if (nonce >= Nonce_FIRST_TRAFFIC_PACKET) {
        do { if (!((!Bits_isZero(session->sharedSecret, 32)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("CryptoAuth.c"), (902), "(!Bits_isZero(session->sharedSecret, 32))"); } } while (0);
        enum CryptoAuth_DecryptErr ret = decryptMessage(session, nonce, msg, session->sharedSecret);
        if (!ret) {
            updateTime(session, msg);
            return 0;
        } else {
            do { if (!1) { break; } uint8_t addr[40] = "unknown"; getIp6((session), addr); String* dn = (session)->pub.displayName; do { if ((session)->context->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print((session)->context->logger, Log_Level_DEBUG, "CryptoAuth.c", 908, "%p %s [%s] state[%d]: " "DROP Failed to [%s] message", (void*)(session), dn ? dn->bytes : "", addr, (session)->nextNonce, ((ret == CryptoAuth_DecryptErr_REPLAY) ? "replay check" : "decrypt")); } } while (0); } while (0)
                                                                                     ;
            return ret;
        }
    } else if (nonce <= Nonce_REPEAT_HELLO) {
        do { if (!1) { break; } uint8_t addr[40] = "unknown"; getIp6((session), addr); String* dn = (session)->pub.displayName; do { if ((session)->context->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print((session)->context->logger, Log_Level_DEBUG, "CryptoAuth.c", 913, "%p %s [%s] state[%d]: " "hello packet during established session nonce=[%d]", (void*)(session), dn ? dn->bytes : "", addr, (session)->nextNonce, nonce); } } while (0); } while (0);
        do {             struct Er_Ret* Er_ret = Message_eshift(msg, 4);                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("CryptoAuth.c", 914, Er_ret);                 } else {                     Er__assertFail(Er_unwind("CryptoAuth.c", 914, Er_ret));                 }             }                  } while (0);
        return decryptHandshake(session, nonce, msg, header);
    } else {
        do { if (!1) { break; } uint8_t addr[40] = "unknown"; getIp6((session), addr); String* dn = (session)->pub.displayName; do { if ((session)->context->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print((session)->context->logger, Log_Level_DEBUG, "CryptoAuth.c", 917, "%p %s [%s] state[%d]: " "DROP key packet during established session nonce=[%d]", (void*)(session), dn ? dn->bytes : "", addr, (session)->nextNonce, nonce); } } while (0); } while (0);
        return CryptoAuth_DecryptErr_KEY_PKT_ESTABLISHED_SESSION;
    }
    Assert_failure("unreachable");
}



struct CryptoAuth* CryptoAuth_new(struct Allocator* allocator,
                                  const uint8_t* privateKey,
                                  struct EventBase* eventBase,
                                  struct Log* logger,
                                  struct Random* rand)
{
    struct CryptoAuth_pvt* ca = Allocator__calloc((allocator),(sizeof(struct CryptoAuth_pvt)),(1),"CryptoAuth.c",931);
    (ca)->Identity_verifier = ((unsigned long) 0xc96f76410f348c21ull);
    ca->allocator = allocator;
    ca->eventBase = eventBase;
    ca->logger = logger;
    ca->rand = rand;

    if (privateKey != 
# 938 "crypto/CryptoAuth.c" 3 4
                     ((void *)0)
# 938 "crypto/CryptoAuth.c"
                         ) {
        Bits__memcpy(ca->privateKey,privateKey,32,"CryptoAuth.c",939);
    } else {
        Random_bytes(rand, ca->privateKey, 32);
    }
    crypto_scalarmult_base(ca->pub.publicKey, ca->privateKey);

    if (0) {
        uint8_t publicKeyHex[65];
        printHexKey(publicKeyHex, ca->pub.publicKey);
        uint8_t privateKeyHex[65];
        printHexKey(privateKeyHex, ca->privateKey);
        do { if (logger && Log_Level_KEYS >= Log_Level_DEBUG) { Log_print(logger, Log_Level_KEYS, "CryptoAuth.c", 950, "Initialized CryptoAuth:\n    myPrivateKey=%s\n     myPublicKey=%s\n", privateKeyHex, publicKeyHex); } } while (0)


                               ;
    }

    return &ca->pub;
}

int CryptoAuth_addUser_ipv6(String* password,
                            String* login,
                            uint8_t ipv6[16],
                            struct CryptoAuth* cryptoAuth)
{
    struct CryptoAuth_pvt* ca = (__extension__ ({ __typeof__((struct CryptoAuth_pvt*) cryptoAuth) Identity_ptr = ((struct CryptoAuth_pvt*) cryptoAuth); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0xc96f76410f348c21ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("CryptoAuth.c"), (964), "(Identity_ptr->Identity_verifier == ((unsigned long) 0xc96f76410f348c21ull))"); } } while (0); Identity_ptr; }));

    struct Allocator* alloc = Allocator__child((ca->allocator),"CryptoAuth.c",966);
    struct CryptoAuth_User* user = Allocator__calloc((alloc),(sizeof(struct CryptoAuth_User)),(1),"CryptoAuth.c",967);
    user->alloc = alloc;
    (user)->Identity_verifier = ((unsigned long) 0xc96f76410f348c21ull);

    if (!login) {
        int i = 0;
        for (struct CryptoAuth_User* u = ca->users; u; u = u->next) { i++; }
        user->login = login = String_printf(alloc, "Anon #%d", i);
    } else {
        user->login = ((login) ? String_newBinary(login->bytes, login->len, alloc) : 
# 976 "crypto/CryptoAuth.c" 3 4
                     ((void *)0)
# 976 "crypto/CryptoAuth.c"
                     );
    }

    struct CryptoHeader_Challenge ac;

    hashPassword(user->secret, &ac, login, password, 2);
    Bits__memcpy(user->userNameHash,&ac,8,"CryptoAuth.c",982);
    hashPassword(user->secret, &ac, 
# 983 "crypto/CryptoAuth.c" 3 4
                                   ((void *)0)
# 983 "crypto/CryptoAuth.c"
                                       , password, 1);
    Bits__memcpy(user->passwordHash,&ac,8,"CryptoAuth.c",984);

    for (struct CryptoAuth_User* u = ca->users; u; u = u->next) {
        if (__builtin_memcmp(user->secret,u->secret,32)) {
        } else if (!login) {
        } else if (String_equals(login, u->login)) {
            Allocator__free((alloc),"CryptoAuth.c",990);
            return -3;
        }
    }

    if (ipv6) {
        Bits__memcpy(user->restrictedToip6,ipv6,16,"CryptoAuth.c",996);
    }


    for (struct CryptoAuth_User** up = &ca->users; ; up = &(*up)->next) {
        if (!*up) {
            *up = user;
            break;
        }
    }

    return 0;
}

int CryptoAuth_removeUsers(struct CryptoAuth* context, String* login)
{
    struct CryptoAuth_pvt* ca = (__extension__ ({ __typeof__((struct CryptoAuth_pvt*) context) Identity_ptr = ((struct CryptoAuth_pvt*) context); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0xc96f76410f348c21ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("CryptoAuth.c"), (1012), "(Identity_ptr->Identity_verifier == ((unsigned long) 0xc96f76410f348c21ull))"); } } while (0); Identity_ptr; }));

    int count = 0;
    struct CryptoAuth_User** up = &ca->users;
    struct CryptoAuth_User* u = *up;
    while ((u = *up)) {
        if (!login || String_equals(login, u->login)) {
            *up = u->next;
            Allocator__free((u->alloc),"CryptoAuth.c",1020);
            count++;
        } else {
            up = &u->next;
        }
    }

    if (!login) {
        do { if (ca->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(ca->logger, Log_Level_DEBUG, "CryptoAuth.c", 1028, "Flushing [%d] users", count); } } while (0);
    } else {
        do { if (ca->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(ca->logger, Log_Level_DEBUG, "CryptoAuth.c", 1030, "Removing [%d] user(s) identified by [%s]", count, login->bytes); } } while (0);
    }
    return count;
}

struct StringList* CryptoAuth_getUsers(struct CryptoAuth* context, struct Allocator* alloc)
{
    struct CryptoAuth_pvt* ca = (__extension__ ({ __typeof__((struct CryptoAuth_pvt*) context) Identity_ptr = ((struct CryptoAuth_pvt*) context); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0xc96f76410f348c21ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("CryptoAuth.c"), (1037), "(Identity_ptr->Identity_verifier == ((unsigned long) 0xc96f76410f348c21ull))"); } } while (0); Identity_ptr; }));

    struct StringList* users = StringList_new(alloc);
    for (struct CryptoAuth_User* u = ca->users; u; u = u->next) {
        StringList_add(users, ((u->login) ? String_newBinary(u->login->bytes, u->login->len, alloc) : 
# 1041 "crypto/CryptoAuth.c" 3 4
                             ((void *)0)
# 1041 "crypto/CryptoAuth.c"
                             ));
    }

    return users;
}

struct CryptoAuth_Session* CryptoAuth_newSession(struct CryptoAuth* ca,
                                                 struct Allocator* alloc,
                                                 const uint8_t herPublicKey[32],
                                                 const 
# 1050 "crypto/CryptoAuth.c" 3 4
                                                      _Bool 
# 1050 "crypto/CryptoAuth.c"
                                                           requireAuth,
                                                 char* displayName)
{
    struct CryptoAuth_pvt* context = (__extension__ ({ __typeof__((struct CryptoAuth_pvt*) ca) Identity_ptr = ((struct CryptoAuth_pvt*) ca); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0xc96f76410f348c21ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("CryptoAuth.c"), (1053), "(Identity_ptr->Identity_verifier == ((unsigned long) 0xc96f76410f348c21ull))"); } } while (0); Identity_ptr; }));
    struct CryptoAuth_Session_pvt* session =
        Allocator__calloc((alloc),(sizeof(struct CryptoAuth_Session_pvt)),(1),"CryptoAuth.c",1055);
    (session)->Identity_verifier = ((unsigned long) 0xc96f76410f348c21ull);
    session->context = context;
    session->requireAuth = requireAuth;
    session->pub.displayName = displayName ? String_new(displayName, alloc) : 
# 1059 "crypto/CryptoAuth.c" 3 4
                                                                             ((void *)0)
# 1059 "crypto/CryptoAuth.c"
                                                                                 ;
    session->timeOfLastPacket = Time_currentTimeSeconds(context->eventBase);
    session->alloc = alloc;

    session->pub.resetAfterInactivitySeconds = 60;
    session->pub.setupResetAfterInactivitySeconds =
        10;

    do { if (!((herPublicKey))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("CryptoAuth.c"), (1067), "(herPublicKey)"); } } while (0);
    Bits__memcpy(session->pub.herPublicKey,herPublicKey,32,"CryptoAuth.c",1068);
    uint8_t calculatedIp6[16];
    AddressCalc_addressForPublicKey(calculatedIp6, herPublicKey);
    Bits__memcpy(session->pub.herIp6,calculatedIp6,16,"CryptoAuth.c",1071);

    return &session->pub;
}

void CryptoAuth_setAuth(const String* password,
                        const String* login,
                        struct CryptoAuth_Session* caSession)
{
    struct CryptoAuth_Session_pvt* session =
        (__extension__ ({ __typeof__((struct CryptoAuth_Session_pvt*)caSession) Identity_ptr = ((struct CryptoAuth_Session_pvt*)caSession); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0xc96f76410f348c21ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("CryptoAuth.c"), (1081), "(Identity_ptr->Identity_verifier == ((unsigned long) 0xc96f76410f348c21ull))"); } } while (0); Identity_ptr; }));

    if (!password && (session->password || session->authType)) {
        if (session->passwdAlloc) {
            Allocator__free((session->passwdAlloc),"CryptoAuth.c",1085);
            session->passwdAlloc = 
# 1086 "crypto/CryptoAuth.c" 3 4
                                  ((void *)0)
# 1086 "crypto/CryptoAuth.c"
                                      ;
        }
        session->password = 
# 1088 "crypto/CryptoAuth.c" 3 4
                           ((void *)0)
# 1088 "crypto/CryptoAuth.c"
                               ;
        session->authType = 0;
    } else if (!session->password || !String_equals(session->password, password)) {
        if (session->passwdAlloc) {
            Allocator__free((session->passwdAlloc),"CryptoAuth.c",1092);
        }
        session->passwdAlloc = Allocator__child((session->alloc),"CryptoAuth.c",1094);
        session->password = ((password) ? String_newBinary(password->bytes, password->len, session->passwdAlloc) : 
# 1095 "crypto/CryptoAuth.c" 3 4
                           ((void *)0)
# 1095 "crypto/CryptoAuth.c"
                           );
        session->authType = 1;
        if (login) {
            session->authType = 2;
            if (session->loginAlloc) {
                Allocator__free((session->loginAlloc),"CryptoAuth.c",1100);
            }
            session->loginAlloc = Allocator__child((session->alloc),"CryptoAuth.c",1102);
            session->login = ((login) ? String_newBinary(login->bytes, login->len, session->loginAlloc) : 
# 1103 "crypto/CryptoAuth.c" 3 4
                            ((void *)0)
# 1103 "crypto/CryptoAuth.c"
                            );
        }
    } else {
        return;
    }
    reset(session);
}

enum CryptoAuth_State CryptoAuth_getState(struct CryptoAuth_Session* caSession)
{
    struct CryptoAuth_Session_pvt* session =
        (__extension__ ({ __typeof__((struct CryptoAuth_Session_pvt*)caSession) Identity_ptr = ((struct CryptoAuth_Session_pvt*)caSession); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0xc96f76410f348c21ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("CryptoAuth.c"), (1114), "(Identity_ptr->Identity_verifier == ((unsigned long) 0xc96f76410f348c21ull))"); } } while (0); Identity_ptr; }));

    if (session->nextNonce <= CryptoAuth_State_RECEIVED_KEY) {
        return session->nextNonce;
    }
    return (session->established) ? CryptoAuth_State_ESTABLISHED : CryptoAuth_State_RECEIVED_KEY;
}

void CryptoAuth_resetIfTimeout(struct CryptoAuth_Session* caSession)
{
    struct CryptoAuth_Session_pvt* session =
        (__extension__ ({ __typeof__((struct CryptoAuth_Session_pvt*)caSession) Identity_ptr = ((struct CryptoAuth_Session_pvt*)caSession); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0xc96f76410f348c21ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("CryptoAuth.c"), (1125), "(Identity_ptr->Identity_verifier == ((unsigned long) 0xc96f76410f348c21ull))"); } } while (0); Identity_ptr; }));
    resetIfTimeout(session);
}

void CryptoAuth_reset(struct CryptoAuth_Session* caSession)
{
    struct CryptoAuth_Session_pvt* session =
        (__extension__ ({ __typeof__((struct CryptoAuth_Session_pvt*)caSession) Identity_ptr = ((struct CryptoAuth_Session_pvt*)caSession); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0xc96f76410f348c21ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("CryptoAuth.c"), (1132), "(Identity_ptr->Identity_verifier == ((unsigned long) 0xc96f76410f348c21ull))"); } } while (0); Identity_ptr; }));
    reset(session);
}


void CryptoAuth_encryptRndNonce(uint8_t nonce[24], struct Message* msg, uint8_t secret[32])
{
    encryptRndNonce(nonce, msg, secret);
}

int CryptoAuth_decryptRndNonce(uint8_t nonce[24], struct Message* msg, uint8_t secret[32])
{
    return decryptRndNonce(nonce, msg, secret);
}
