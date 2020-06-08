# 1 "dht/dhtcore/ReplySerializer.c"
# 1 "/home/jojapoppa/cjdns//"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "dht/dhtcore/ReplySerializer.c"
# 15 "dht/dhtcore/ReplySerializer.c"
# 1 "./benc/Dict.h" 1
# 18 "./benc/Dict.h"
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








__attribute__((__format__ (__printf__, 1, 2)))
static inline void Gcc_checkPrintf(const char* format, ...)
{

}
# 19 "./util/Assert.h" 2
# 1 "./util/Linker.h" 1
# 20 "./util/Assert.h" 2
struct Linker_x07984288604348637;
# 33 "./util/Assert.h"
__attribute__((__format__ (__printf__, 1, 2)))
__attribute__((__noreturn__))
void Assert_failure(const char* format, ...);
# 19 "./util/Identity.h" 2
# 1 "./util/Constant.h" 1
# 33 "./util/Constant.h"

# 20 "./util/Identity.h" 2



    
# 19 "./memory/Allocator.h" 2


struct Linker_x04297048032494246;





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
# 19 "./benc/Dict.h" 2
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





# 23 "./benc/Object.h"
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
# 20 "./benc/Dict.h" 2

struct Linker_x04858917248658483;

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
# 16 "dht/dhtcore/ReplySerializer.c" 2
# 1 "./benc/String.h" 1
# 20 "./benc/String.h"
# 1 "./util/CString.h" 1
# 21 "./util/CString.h"
struct Linker_x09123961746691982;

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

struct Linker_x09410521391203532;

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
# 17 "dht/dhtcore/ReplySerializer.c" 2
# 1 "./crypto/AddressCalc.h" 1
# 19 "./crypto/AddressCalc.h"
struct Linker_x004904377474562538;
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
# 18 "dht/dhtcore/ReplySerializer.c" 2
# 1 "./dht/CJDHTConstants.h" 1
# 19 "./dht/CJDHTConstants.h"
# 1 "./util/version/Version.h" 1
# 19 "./util/version/Version.h"
struct Linker_x05711799211987658;
# 42 "./util/version/Version.h"

# 139 "./util/version/Version.h"

# 150 "./util/version/Version.h"

# 178 "./util/version/Version.h"

# 188 "./util/version/Version.h"

# 200 "./util/version/Version.h"









# 219 "./util/version/Version.h"

# 230 "./util/version/Version.h"

























# 263 "./util/version/Version.h"

# 283 "./util/version/Version.h"

# 295 "./util/version/Version.h"

# 305 "./util/version/Version.h"

# 341 "./util/version/Version.h"

# 365 "./util/version/Version.h"

# 416 "./util/version/Version.h"

# 429 "./util/version/Version.h"

# 442 "./util/version/Version.h"

# 466 "./util/version/Version.h"
int Version_isCompatible(uint32_t one, uint32_t two);
# 20 "./dht/CJDHTConstants.h" 2


static String* const CJDHTConstants_QUERY = (&(String) { .bytes = "q", .len = sizeof("q") - 1 });


static String* const CJDHTConstants_QUERY_NH = (&(String) { .bytes = "nh", .len = sizeof("nh") - 1 });


static String* const CJDHTConstants_QUERY_FN = (&(String) { .bytes = "fn", .len = sizeof("fn") - 1 });


static String* const CJDHTConstants_QUERY_GP = (&(String) { .bytes = "gp", .len = sizeof("gp") - 1 });


static String* const CJDHTConstants_QUERY_PING = (&(String) { .bytes = "pn", .len = sizeof("pn") - 1 });


static String* const CJDHTConstants_TARGET = (&(String) { .bytes = "tar", .len = sizeof("tar") - 1 });


static String* const CJDHTConstants_NODES = (&(String) { .bytes = "n", .len = sizeof("n") - 1 });


static String* const CJDHTConstants_TXID = (&(String) { .bytes = "txid", .len = sizeof("txid") - 1 });


static String* const CJDHTConstants_VERSION = (&(String) { .bytes = "v", .len = sizeof("v") - 1 });


static String* const CJDHTConstants_NODE_PROTOCOLS = (&(String) { .bytes = "np", .len = sizeof("np") - 1 });


static String* const CJDHTConstants_PROTOCOL = (&(String) { .bytes = "p", .len = sizeof("p") - 1 });


static String* const CJDHTConstants_ENC_SCHEME = (&(String) { .bytes = "es", .len = sizeof("es") - 1 });



static String* const CJDHTConstants_ENC_INDEX = (&(String) { .bytes = "ei", .len = sizeof("ei") - 1 });


static String* const CJDHTConstants_PEER_ENC_SCHEME = (&(String) { .bytes = "pes", .len = sizeof("pes") - 1 });
static String* const CJDHTConstants_PEER_ENC_INDEX = (&(String) { .bytes = "pei", .len = sizeof("pei") - 1 });
# 19 "dht/dhtcore/ReplySerializer.c" 2
# 1 "./dht/dhtcore/ReplySerializer.h" 1
# 19 "./dht/dhtcore/ReplySerializer.h"
# 1 "./dht/Address.h" 1
# 21 "./dht/Address.h"
struct Linker_x07128648362750774;
# 34 "./dht/Address.h"
struct Address
{

    uint32_t protocolVersion;


    uint32_t padding;

    union {
        struct {

            uint32_t three_be;
            uint32_t four_be;
            uint32_t one_be;
            uint32_t two_be;
        } ints;

        struct {
            uint64_t two_be;
            uint64_t one_be;
        } longs;

        uint8_t bytes[16];
    } ip6;

    uint8_t key[32];

    uint64_t path;
};

void Assert_compileTime(char x[1 - (!(sizeof(struct Address) == (8 + 16 + 32 + 8)))]);

struct Address_List
{
    int length;
    struct Address* elems;
};

struct Address_List* Address_List_new(uint32_t length, struct Allocator* alloc);

uint32_t Address_getPrefix(struct Address* addr);

uint32_t Address_prefixForIp6(uint8_t ip6[16]);

uint32_t Address_prefixForSearchTarget(const uint8_t searchTarget[16]);
# 108 "./dht/Address.h"
void Address_serialize(uint8_t output[40], const struct Address* addr);

void Address_parse(struct Address* addr, const uint8_t input[40]);


# 112 "./dht/Address.h" 3 4
_Bool 
# 112 "./dht/Address.h"
    Address_isSame(const struct Address* addr,
                    const struct Address* addr2);


# 115 "./dht/Address.h" 3 4
_Bool 
# 115 "./dht/Address.h"
    Address_isSameIp(const struct Address* addr,
                      const struct Address* addr2);


# 118 "./dht/Address.h" 3 4
_Bool 
# 118 "./dht/Address.h"
    Address_equalsSearchTarget(struct Address* addr,
                                const uint8_t searchTarget[16]);

void Address_forKey(struct Address* out, const uint8_t key[32]);

void Address_printIp(uint8_t output[40], struct Address* addr);

void Address_printShortIp(uint8_t output[40], struct Address* addr);

void Address_print(uint8_t output[60], struct Address* addr);

String* Address_toString(struct Address* addr, struct Allocator* alloc);

struct Address* Address_fromString(String* str, struct Allocator* alloc);

struct Address* Address_clone(struct Address* orig, struct Allocator* alloc);

int Address_xorcmp(uint32_t target,
                   uint32_t negativeIfCloser,
                   uint32_t positiveIfCloser);
# 148 "./dht/Address.h"
int Address_closest(struct Address* target,
                    struct Address* negativeIfCloser,
                    struct Address* positiveIfCloser);
# 20 "./dht/dhtcore/ReplySerializer.h" 2

# 1 "./util/log/Log.h" 1
# 20 "./util/log/Log.h"
struct Linker_x019565567624686042;

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
# 22 "./dht/dhtcore/ReplySerializer.h" 2

struct Linker_x08637337833365646;




struct Address_List* ReplySerializer_parse(struct Address* fromNode,
                                           Dict* result,
                                           struct Log* log,
                                           
# 31 "./dht/dhtcore/ReplySerializer.h" 3 4
                                          _Bool 
# 31 "./dht/dhtcore/ReplySerializer.h"
                                               splicePath,
                                           struct Allocator* alloc);

void ReplySerializer_serialize(struct Address_List* addrs,
                               Dict* out,
                               struct Address* convertDirectorFor,
                               struct Allocator* alloc);
# 20 "dht/dhtcore/ReplySerializer.c" 2
# 1 "./dht/dhtcore/VersionList.h" 1
# 21 "./dht/dhtcore/VersionList.h"
struct Linker_x08091682013591655;



struct VersionList
{
    uint32_t length;
    struct Allocator* alloc;
    uint32_t* versions;
};

struct VersionList* VersionList_parse(String* str, struct Allocator* alloc);

String* VersionList_stringify(struct VersionList* list, struct Allocator* alloc);

struct VersionList* VersionList_new(uint32_t count, struct Allocator* alloc);
# 21 "dht/dhtcore/ReplySerializer.c" 2


# 1 "./switch/LabelSplicer.h" 1
# 18 "./switch/LabelSplicer.h"
# 1 "./util/Bits.h" 1
# 21 "./util/Bits.h"
struct Linker_x017558842799347385;


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
# 19 "./switch/LabelSplicer.h" 2
# 31 "./switch/LabelSplicer.h"
static inline uint64_t LabelSplicer_unsplice(uint64_t fullPath, uint64_t midPath)
{
    return fullPath >> Bits_log2x64(midPath);
}





static inline uint64_t LabelSplicer_splice(uint64_t goHere, uint64_t viaHere)
{
    uint64_t log2ViaHere = Bits_log2x64(viaHere);

    if (Bits_log2x64(goHere) + log2ViaHere > 59) {

        return 
# 46 "./switch/LabelSplicer.h" 3 4
              0xffffffffffffffffULL
# 46 "./switch/LabelSplicer.h"
                        ;
    }

    return ((goHere ^ 1) << log2ViaHere) ^ viaHere;
}
# 63 "./switch/LabelSplicer.h"
static inline 
# 63 "./switch/LabelSplicer.h" 3 4
             _Bool 
# 63 "./switch/LabelSplicer.h"
                  LabelSplicer_routesThrough(uint64_t destination, uint64_t midPath)
{
    if (midPath > destination) {
        return 
# 66 "./switch/LabelSplicer.h" 3 4
              0
# 66 "./switch/LabelSplicer.h"
                   ;
    } if (midPath < 2) {
        return 
# 68 "./switch/LabelSplicer.h" 3 4
              1
# 68 "./switch/LabelSplicer.h"
                  ;
    }
    uint64_t mask = 
# 70 "./switch/LabelSplicer.h" 3 4
                   0xffffffffffffffffULL 
# 70 "./switch/LabelSplicer.h"
                              >> (64 - Bits_log2x64(midPath));
    return (destination & mask) == (midPath & mask);
}
# 24 "dht/dhtcore/ReplySerializer.c" 2


# 1 "./switch/NumberCompress.h" 1
# 22 "./switch/NumberCompress.h"
# 1 "./switch/EncodingScheme.h" 1
# 19 "./switch/EncodingScheme.h"
# 1 "./benc/List.h" 1
# 21 "./benc/List.h"
struct Linker_x018777836206609866;

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
# 20 "./switch/EncodingScheme.h" 2


struct Linker_x06356881686988569;



struct EncodingScheme_Form
{


    uint16_t bitCount;
    uint16_t prefixLen;
    uint32_t prefix;
};

struct EncodingScheme
{
    struct EncodingScheme_Form* forms;
    int count;
};







int EncodingScheme_getFormNum(struct EncodingScheme* scheme, uint64_t routeLabel);
# 62 "./switch/EncodingScheme.h"
uint64_t EncodingScheme_convertLabel(struct EncodingScheme* scheme,
                                     uint64_t routeLabel,
                                     int convertTo);








# 72 "./switch/EncodingScheme.h" 3 4
_Bool 
# 72 "./switch/EncodingScheme.h"
    EncodingScheme_isSane(struct EncodingScheme* scheme);

String* EncodingScheme_serialize(struct EncodingScheme* list,
                                 struct Allocator* alloc);

struct EncodingScheme* EncodingScheme_deserialize(String* data,
                                                  struct Allocator* alloc);

struct EncodingScheme* EncodingScheme_defineFixedWidthScheme(int bitCount, struct Allocator* alloc);


struct EncodingScheme* EncodingScheme_defineDynWidthScheme(struct EncodingScheme_Form* forms,
                                                           int formCount,
                                                           struct Allocator* alloc);




static inline int EncodingScheme_formSize(const struct EncodingScheme_Form* form)
{
    return form->bitCount + form->prefixLen;
}

int EncodingScheme_compare(struct EncodingScheme* a, struct EncodingScheme* b);



struct EncodingScheme* EncodingScheme_fromList(List* scheme, struct Allocator* alloc);
List* EncodingScheme_asList(struct EncodingScheme* list, struct Allocator* alloc);




int EncodingScheme_isSelfRoute(struct EncodingScheme* scheme, uint64_t routeLabel);




int EncodingScheme_isOneHop(struct EncodingScheme* scheme, uint64_t routeLabel);


int EncodingScheme_parseDirector(struct EncodingScheme* scheme, uint64_t label);

uint64_t EncodingScheme_serializeDirector(struct EncodingScheme* scheme, int number, int formNum);




# 119 "./switch/EncodingScheme.h" 3 4
_Bool 
# 119 "./switch/EncodingScheme.h"
    EncodingScheme_is358(struct EncodingScheme* scheme);
# 23 "./switch/NumberCompress.h" 2
# 43 "./switch/NumberCompress.h"
static inline struct EncodingScheme* NumberCompress_f4_defineScheme(struct Allocator* alloc)
{
    return EncodingScheme_defineFixedWidthScheme(4, alloc);
}

static inline uint32_t NumberCompress_f4_bitsUsedForLabel(const uint64_t label)
{
    return 4;
}

static inline uint32_t NumberCompress_f4_bitsUsedForNumber(const uint32_t number)
{
    do { if (!((number < 16))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("NumberCompress.h"), (55), "(number < 16)"); } } while (0);
    return 4;
}

static inline uint64_t NumberCompress_f4_getCompressed(const uint32_t number,
                                                       const uint32_t bitsUsed)
{
    do { if (!((number < 16))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("NumberCompress.h"), (62), "(number < 16)"); } } while (0);
    return number;
}

static inline uint32_t NumberCompress_f4_getDecompressed(const uint64_t label,
                                                         const uint32_t bitsUsed)
{
    return label & 0xf;
}




static inline struct EncodingScheme* NumberCompress_f8_defineScheme(struct Allocator* alloc)
{
    return EncodingScheme_defineFixedWidthScheme(8, alloc);
}

static inline uint32_t NumberCompress_f8_bitsUsedForLabel(const uint64_t label)
{
    return 8;
}

static inline uint32_t NumberCompress_f8_bitsUsedForNumber(const uint32_t number)
{
    do { if (!((number < 256))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("NumberCompress.h"), (87), "(number < 256)"); } } while (0);
    return 8;
}

static inline uint64_t NumberCompress_f8_getCompressed(const uint32_t number,
                                                       const uint32_t bitsUsed)
{
    do { if (!((number < 256))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("NumberCompress.h"), (94), "(number < 256)"); } } while (0);
    return number;
}

static inline uint32_t NumberCompress_f8_getDecompressed(const uint64_t label,
                                                         const uint32_t bitsUsed)
{
    return label & 0xff;
}
# 115 "./switch/NumberCompress.h"
static inline struct EncodingScheme* NumberCompress_v3x5x8_defineScheme(struct Allocator* alloc)
{
    return EncodingScheme_defineDynWidthScheme(
        ((struct EncodingScheme_Form[3]) {
            { .bitCount = 3, .prefixLen = 1, .prefix = 1, },
            { .bitCount = 5, .prefixLen = 2, .prefix = 1<<1, },
            { .bitCount = 8, .prefixLen = 2, .prefix = 0, }
        }),
        3,
        alloc);
}

static inline uint32_t NumberCompress_v3x5x8_bitsUsedForLabel(const uint64_t label)
{
    if (0 != (label & 0x1)) {
        return 4;
    }

    if (0 != (label & 0x2)) {
        return 7;
    }

    return 10;
}

static inline uint32_t NumberCompress_v3x5x8_bitsUsedForNumber(const uint32_t number)
{
    if (number < 8) {
        return 4;
    } else if (number < 33) {
        return 7;
    } else {
        return 10;
    }
}

static inline uint64_t NumberCompress_v3x5x8_getCompressed(const uint32_t number,
                                                          const uint32_t bitsUsed)
{
    if (1 == number) {
        return 1;
    }

    switch (bitsUsed) {
        case 4:
            if (0 == number) {
                return 3;
            }
            return (number << 1) | 1;
        case 7:
            if (0 == number) {
                return 2;
            }

            return ((number-1) << 2) | 2;
        case 10:
            if (0 == number) {
                return 0;
            }

            return ((number-1) << 2);
        default: return 0;
    }
}

static inline uint32_t NumberCompress_v3x5x8_getDecompressed(const uint64_t label,
                                                            const uint32_t bitsUsed)
{
    uint32_t number;
    switch (bitsUsed) {
        case 4:
            number = (label >> 1) & 0x7u;
            if (0 == number) {
                return 1;
            }
            if (1 == number) {
                return 0;
            }
            return number;
        case 7:
            number = (label >> 2) & 0x1fu;
            if (0 != number) {
                ++number;
            }
            return number;
        case 10:
            number = (label >> 2) & 0xffu;
            if (0 != number) {
                ++number;
            }
            return number;
        default: return 0;
    }
}
# 219 "./switch/NumberCompress.h"
static inline struct EncodingScheme* NumberCompress_v4x8_defineScheme(struct Allocator* alloc)
{
    return EncodingScheme_defineDynWidthScheme(
        ((struct EncodingScheme_Form[]) {
            { .bitCount = 4, .prefixLen = 1, .prefix = 1, },
            { .bitCount = 8, .prefixLen = 1, .prefix = 0, }
        }),
        2,
        alloc);
}

static inline uint32_t NumberCompress_v4x8_getDecompressed(const uint64_t label,
                                                           const uint32_t bitsUsed)
{
    if ((label & 0x1f) == 1) { return 1; }
    switch (bitsUsed) {
        case 5: {
            return ((label >> 1) & 0xfu) ^ 1;
        }

        case 9: {
            return ((label >> 1) & 0xffu) ^ 1;
        }

        default: do { } while (0);
    }
    return 0;
}


static inline uint64_t NumberCompress_v4x8_getCompressed(uint32_t number,
                                                         const uint32_t bitsUsed)
{
    if (1 == number) { return 1; }

    switch (bitsUsed) {
        case 5: {
            do { } while (0);
            return (number << 1) ^ 3;
        }
        case 9: {
            do { } while (0);
            return (number << 1) ^ 2;
        }
        default: do { } while (0);
    }
    return 0;
}

static inline uint32_t NumberCompress_v4x8_bitsUsedForLabel(const uint64_t label)
{
    return (label & 1) ? 5 : 9;
}

static inline uint32_t NumberCompress_v4x8_bitsUsedForNumber(const uint32_t number)
{
    do { } while (0);
    return (number < 16) ? 5 : 9;
}
# 304 "./switch/NumberCompress.h"
static inline uint64_t NumberCompress_getLabelFor(uint64_t target, uint64_t whoIsAsking)
{
    uint32_t targetBits = NumberCompress_v3x5x8_bitsUsedForLabel(target);
    uint32_t whoIsAskingBits = NumberCompress_v3x5x8_bitsUsedForLabel(whoIsAsking);

    if (targetBits >= whoIsAskingBits) {
        return target;
    }

    uint32_t targetIfaceNum = NumberCompress_v3x5x8_getDecompressed(target, targetBits);

    return ((target & (
# 315 "./switch/NumberCompress.h" 3 4
                      0xffffffffffffffffULL 
# 315 "./switch/NumberCompress.h"
                                 << targetBits)) << (whoIsAskingBits - targetBits))
        | NumberCompress_v3x5x8_getCompressed(targetIfaceNum, whoIsAskingBits);
}
# 27 "dht/dhtcore/ReplySerializer.c" 2
# 35 "dht/dhtcore/ReplySerializer.c"
struct Address_List* ReplySerializer_parse(struct Address* fromNode,
                                           Dict* result,
                                           struct Log* log,
                                           
# 38 "dht/dhtcore/ReplySerializer.c" 3 4
                                          _Bool 
# 38 "dht/dhtcore/ReplySerializer.c"
                                               splicePath,
                                           struct Allocator* alloc)
{
    String* nodes = Dict_getString(result, CJDHTConstants_NODES);

    if (!nodes) {
        do { if (log && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(log, Log_Level_DEBUG, "ReplySerializer.c", 44, "Missing 'n' field in reply"); } } while (0);
        return 
# 45 "dht/dhtcore/ReplySerializer.c" 3 4
              ((void *)0)
# 45 "dht/dhtcore/ReplySerializer.c"
                  ;
    }

    if (nodes->len == 0 || nodes->len % 40 != 0) {
        do { if (log && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(log, Log_Level_DEBUG, "ReplySerializer.c", 49, "Dropping unrecognized reply"); } } while (0);
        return 
# 50 "dht/dhtcore/ReplySerializer.c" 3 4
              ((void *)0)
# 50 "dht/dhtcore/ReplySerializer.c"
                  ;
    }

    struct VersionList* versions = 
# 53 "dht/dhtcore/ReplySerializer.c" 3 4
                                  ((void *)0)
# 53 "dht/dhtcore/ReplySerializer.c"
                                      ;
    String* versionsStr = Dict_getString(result, CJDHTConstants_NODE_PROTOCOLS);
    if (versionsStr) {
        versions = VersionList_parse(versionsStr, alloc);
    }
    if (!versions || versions->length != (nodes->len / 40)) {
        do { if (log && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(log, Log_Level_DEBUG, "ReplySerializer.c", 59, "Reply with missing or invalid versions"); } } while (0);
        return 
# 60 "dht/dhtcore/ReplySerializer.c" 3 4
              ((void *)0)
# 60 "dht/dhtcore/ReplySerializer.c"
                  ;
    }

    struct Address_List* out = Address_List_new(versions->length, alloc);

    uint32_t j = 0;
    for (uint32_t i = 0; nodes && i < nodes->len; i += 40) {

        struct Address addr = { .path = 0 };
        Address_parse(&addr, (uint8_t*) &nodes->bytes[i]);
        addr.protocolVersion = versions->versions[i / 40];


        Address_getPrefix(&addr);

        if (splicePath) {


            uint64_t path = LabelSplicer_splice(addr.path, fromNode->path);

            if (path == 
# 80 "dht/dhtcore/ReplySerializer.c" 3 4
                       0xffffffffffffffffULL
# 80 "dht/dhtcore/ReplySerializer.c"
                                 ) {
# 89 "dht/dhtcore/ReplySerializer.c"
                continue;
            }

            addr.path = path;
        }







        Address_getPrefix(&addr);
        if (!AddressCalc_validAddress(addr.ip6.bytes)) {
            struct Allocator* tmpAlloc = Allocator__child((alloc),"ReplySerializer.c",103);
            String* printed = Address_toString(&addr, tmpAlloc);
            uint8_t ipPrinted[40];
            Address_printIp(ipPrinted, &addr);
            do { if (log && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(log, Log_Level_DEBUG, "ReplySerializer.c", 107, "Was told garbage addr [%s] [%s]", printed->bytes, ipPrinted); } } while (0);
            Allocator__free((tmpAlloc),"ReplySerializer.c",108);

            continue;
        }

        Bits__memcpy(&out->elems[j++],&addr,sizeof(struct Address),"ReplySerializer.c",113);
    }
    out->length = j;
    return out;
}

void ReplySerializer_serialize(struct Address_List* addrs,
                               Dict* out,
                               struct Address* convertDirectorFor,
                               struct Allocator* alloc)
{
    if (!addrs->length) { return; }
    String* nodes = String_newBinary(
# 125 "dht/dhtcore/ReplySerializer.c" 3 4
                                    ((void *)0)
# 125 "dht/dhtcore/ReplySerializer.c"
                                        , addrs->length * 40, alloc);
    struct VersionList* versions = VersionList_new(addrs->length, alloc);
    for (int i = 0; i < addrs->length; i++) {
        versions->versions[i] = addrs->elems[i].protocolVersion;
        if (!convertDirectorFor) {
            Address_serialize(&nodes->bytes[i * 40], &addrs->elems[i]);
        } else {
            struct Address addr;
            Bits__memcpy(&addr,&addrs->elems[i],sizeof(struct Address),"ReplySerializer.c",133);
            addr.path = NumberCompress_getLabelFor(addr.path, convertDirectorFor->path);
            Address_serialize(&nodes->bytes[i * 40], &addr);
        }
    }
    Dict_putString(out, String_new("n", alloc), nodes, alloc);
    Dict_putString(out, String_new("np", alloc), VersionList_stringify(versions, alloc), alloc);
}
