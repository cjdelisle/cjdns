# 1 "net/SwitchPinger.c"
# 1 "/home/jojapoppa/cjdns//"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "net/SwitchPinger.c"
# 15 "net/SwitchPinger.c"
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
# 1 "./util/Linker.h" 1
# 20 "./util/Assert.h" 2
struct Linker_x05242731969594143;
# 33 "./util/Assert.h"
__attribute__((__format__ (__printf__, 1, 2)))
__attribute__((__noreturn__))
void Assert_failure(const char* format, ...);
# 19 "./util/Identity.h" 2
# 1 "./util/Constant.h" 1
# 33 "./util/Constant.h"

# 20 "./util/Identity.h" 2



    
# 19 "./memory/Allocator.h" 2


struct Linker_x04971764997811292;





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
struct Linker_x05304145603500687;

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

struct Linker_x04690095697557919;

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
# 16 "net/SwitchPinger.c" 2
# 1 "./crypto/AddressCalc.h" 1
# 19 "./crypto/AddressCalc.h"
struct Linker_x03756570275102791;
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
# 17 "net/SwitchPinger.c" 2
# 1 "./net/SwitchPinger.h" 1
# 19 "./net/SwitchPinger.h"
# 1 "./dht/Address.h" 1
# 21 "./dht/Address.h"
struct Linker_x06165519081931481;
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
# 20 "./net/SwitchPinger.h" 2
# 1 "./crypto/random/Random.h" 1
# 19 "./crypto/random/Random.h"
# 1 "./exception/Except.h" 1
# 18 "./exception/Except.h"
# 1 "./exception/Er.h" 1
# 23 "./exception/Er.h"
struct Linker_x0976126047286693;

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


struct Linker_x08864616763448978;



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
struct Linker_x0009004370749231994;

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
struct Linker_x08620945530071211;



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

struct Linker_x0735168380166378;



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
# 21 "./net/SwitchPinger.h" 2
# 1 "./interface/Iface.h" 1
# 20 "./interface/Iface.h"
# 1 "./wire/Message.h" 1
# 20 "./wire/Message.h"
# 1 "./util/Bits.h" 1
# 21 "./util/Bits.h"
struct Linker_x07567442787980534;


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
# 21 "./wire/Message.h" 2
# 1 "./util/Endian.h" 1
# 22 "./wire/Message.h" 2

struct Linker_x013890533354621226;



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
# 21 "./interface/Iface.h" 2
# 1 "./util/Defined.h" 1
# 22 "./interface/Iface.h" 2

struct Iface;





typedef struct Iface* (* Iface_Callback)(struct Message* message, struct Iface* thisInterface);



struct Iface
{

    Iface_Callback send;



        struct Message* currentMsg;



    struct Iface* connectedIf;
};
# 55 "./interface/Iface.h"
static inline void Iface_send(struct Iface* iface, struct Message* msg)
{
    do {
        struct Iface* conn = iface->connectedIf;


            do { if (!((conn))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("Iface.h"), (61), "(conn)"); } } while (0);
            do { if (!((conn->send))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("Iface.h"), (62), "(conn->send)"); } } while (0);
            do { if (!((msg))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("Iface.h"), (63), "(msg)"); } } while (0);
            struct Message* currentMsg = conn->currentMsg;
            msg->currentIface = conn;
            conn->currentMsg = msg;


        iface = conn->send(msg, conn);


            msg->currentIface = 
# 72 "./interface/Iface.h" 3 4
                               ((void *)0)
# 72 "./interface/Iface.h"
                                   ;
            conn->currentMsg = currentMsg;


        if (!0) {
            do { if (!((!iface))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("Iface.h"), (77), "(!iface)"); } } while (0);
        }
    } while (iface);
}
# 101 "./interface/Iface.h"
static inline __attribute__ ((warn_unused_result)) struct Iface* Iface_next(struct Iface* iface, struct Message* msg)
{

        struct Iface* conn = iface->connectedIf;
        struct Message* currentMsg = conn->currentMsg;
        do { if (!((msg->currentIface))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("Iface.h"), (106), "(msg->currentIface)"); } } while (0);
        do { if (!((msg->currentIface->currentMsg == msg))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("Iface.h"), (107), "(msg->currentIface->currentMsg == msg)"); } } while (0);
        msg->currentIface->currentMsg = 
# 108 "./interface/Iface.h" 3 4
                                       ((void *)0)
# 108 "./interface/Iface.h"
                                           ;


    if (0) {

            msg->currentIface = conn;
            conn->currentMsg = msg;

        return iface;
    }



        msg->currentIface = 
# 121 "./interface/Iface.h" 3 4
                           ((void *)0)
# 121 "./interface/Iface.h"
                               ;
        conn->currentMsg = 
# 122 "./interface/Iface.h" 3 4
                          ((void *)0)
# 122 "./interface/Iface.h"
                              ;


    Iface_send(iface, msg);


        conn->currentMsg = currentMsg;


    return 
# 131 "./interface/Iface.h" 3 4
          ((void *)0)
# 131 "./interface/Iface.h"
              ;
}
# 167 "./interface/Iface.h"
static inline void Iface_plumb(struct Iface* a, struct Iface* b)
{
    do { if (!((!a->connectedIf))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("Iface.h"), (169), "(!a->connectedIf)"); } } while (0);
    do { if (!((!b->connectedIf))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("Iface.h"), (170), "(!b->connectedIf)"); } } while (0);
    a->connectedIf = b;
    b->connectedIf = a;
}

static inline void Iface_unplumb(struct Iface* a, struct Iface* b)
{
    do { if (!((a->connectedIf == b))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("Iface.h"), (177), "(a->connectedIf == b)"); } } while (0);
    do { if (!((b->connectedIf == a))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("Iface.h"), (178), "(b->connectedIf == a)"); } } while (0);
    a->connectedIf = 
# 179 "./interface/Iface.h" 3 4
                    ((void *)0)
# 179 "./interface/Iface.h"
                        ;
    b->connectedIf = 
# 180 "./interface/Iface.h" 3 4
                    ((void *)0)
# 180 "./interface/Iface.h"
                        ;
}
# 22 "./net/SwitchPinger.h" 2
# 1 "./util/events/EventBase.h" 1
# 20 "./util/events/EventBase.h"
struct Linker_x09653609616176497;

struct EventBase
{
    int unused;
};

struct EventBase* EventBase_new(struct Allocator* alloc);

int EventBase_eventCount(struct EventBase* eventBase);

void EventBase_beginLoop(struct EventBase* eventBase);

void EventBase_endLoop(struct EventBase* eventBase);
# 23 "./net/SwitchPinger.h" 2


struct Linker_x09959817953731556;





enum SwitchPinger_Result
{

    SwitchPinger_Result_OK,


    SwitchPinger_Result_LABEL_MISMATCH,


    SwitchPinger_Result_WRONG_DATA,


    SwitchPinger_Result_ERROR_RESPONSE,


    SwitchPinger_Result_LOOP_ROUTE,


    SwitchPinger_Result_TIMEOUT
};

struct SwitchPinger_Ping;

struct SwitchPinger_Response
{

    enum SwitchPinger_Result res;


    uint64_t label;


    String* data;


    uint64_t milliseconds;


    uint32_t version;


    uint8_t key[32];


    uint32_t kbpsLimit;
    struct Address snode;

    struct SwitchPinger_Ping* ping;
};


typedef void (* SwitchPinger_ResponseCallback)(struct SwitchPinger_Response* resp, void* userData);

enum SwitchPinger_Type
{
    SwitchPinger_Type_PING,
    SwitchPinger_Type_KEYPING,
    SwitchPinger_Type_GETSNODE
};

struct SwitchPinger_Ping
{




    struct Allocator* pingAlloc;

    enum SwitchPinger_Type type;


    uint32_t kbpsLimit;
    struct Address snode;





    void* onResponseContext;
};

struct SwitchPinger
{
    struct Iface controlHandlerIf;
};

String* SwitchPinger_resultString(enum SwitchPinger_Result result);
# 130 "./net/SwitchPinger.h"
struct SwitchPinger_Ping* SwitchPinger_newPing(uint64_t label,
                                               String* data,
                                               uint32_t timeoutMilliseconds,
                                               SwitchPinger_ResponseCallback onResponse,
                                               struct Allocator* alloc,
                                               struct SwitchPinger* ctx);

struct SwitchPinger* SwitchPinger_new(struct EventBase* eventBase,
                                      struct Random* rand,
                                      struct Log* logger,
                                      struct Address* myAddr,
                                      struct Allocator* alloc);
# 18 "net/SwitchPinger.c" 2


# 1 "./util/Checksum.h" 1
# 26 "./util/Checksum.h"
static uint32_t Checksum_step(const uint8_t* buffer,
                              uint16_t length,
                              uint32_t state)
{

    for (uint32_t i = 0; i < length / 2; i++) {
        state += ((uint16_t*) buffer)[i];
    }


    if (length % 2) {
        state += 0 ? (buffer[length - 1] << 8) : (buffer[length - 1]);
    }
    return state;
}

static uint32_t Checksum_step32(uint32_t content, uint32_t state)
{
    return state + (content >> 16) + (content & 0xFFFF);
}

static uint16_t Checksum_complete(uint32_t state)
{
    while (state > 0xFFFF) {
        state = (state >> 16) + (state & 0xFFFF);
    }
    return ~state;
}
# 63 "./util/Checksum.h"
static inline uint16_t Checksum_engine(const uint8_t* buffer, uint16_t length)
{
    do { if (!((!((uintptr_t)buffer % 2)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("Checksum.h"), (65), "(!((uintptr_t)buffer % 2))"); } } while (0);
    return Checksum_complete(Checksum_step(buffer, length, 0));
}
# 81 "./util/Checksum.h"
static inline uint16_t Checksum_Ip6(const uint8_t* restrict sourceAndDestAddrs,
                                    const uint8_t* restrict packetHeaderAndContent,
                                    uint16_t length,
                                    uint32_t packetType_be)
{
    do { if (!((!((uintptr_t)sourceAndDestAddrs % 2)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("Checksum.h"), (86), "(!((uintptr_t)sourceAndDestAddrs % 2))"); } } while (0);
    do { if (!((!((uintptr_t)packetHeaderAndContent % 2)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("Checksum.h"), (87), "(!((uintptr_t)packetHeaderAndContent % 2))"); } } while (0);


    uint64_t sum = Checksum_step(sourceAndDestAddrs, 32, 0);

    const uint32_t length_be = ((uint32_t)__builtin_bswap32(((uint32_t)(length))));
    sum = Checksum_step32(length_be, sum);
    sum = Checksum_step32(packetType_be, sum);
    sum = Checksum_step(packetHeaderAndContent, length, sum);

    return Checksum_complete(sum);
}
# 110 "./util/Checksum.h"
static inline uint16_t Checksum_udpIp6(const uint8_t* restrict sourceAndDestAddrs,
                                       const uint8_t* restrict udpHeaderAndContent,
                                       uint16_t length)
{
    return Checksum_Ip6(sourceAndDestAddrs,
                        udpHeaderAndContent,
                        length,
                        ((uint32_t)__builtin_bswap32(((uint32_t)(17)))));
}
# 131 "./util/Checksum.h"
static inline uint16_t Checksum_icmp6(const uint8_t* restrict sourceAndDestAddrs,
                                      const uint8_t* restrict icmpHeaderAndContent,
                                      uint16_t length)
{
    return Checksum_Ip6(sourceAndDestAddrs,
                        icmpHeaderAndContent,
                        length,
                        ((uint32_t)__builtin_bswap32(((uint32_t)(58)))));
}
# 21 "net/SwitchPinger.c" 2

# 1 "./util/Pinger.h" 1
# 24 "./util/Pinger.h"
struct Linker_x07519593507018072;
# 50 "./util/Pinger.h"
struct Pinger;

struct Pinger_Ping
{




    struct Allocator* pingAlloc;


    uint32_t handle;





    void* context;
};
# 79 "./util/Pinger.h"
struct Pinger_Ping* Pinger_newPing(String* data,
                                   void (* onResponse)(String* data, uint32_t milliseconds, void* context),
                                   void (* sendPing)(String* data, void* context),
                                   uint32_t timeoutMilliseconds,
                                   struct Allocator* allocator,
                                   struct Pinger* pinger);







void Pinger_pongReceived(String* data, struct Pinger* pinger);
# 101 "./util/Pinger.h"
struct Pinger* Pinger_new(struct EventBase* eventBase,
                          struct Random* rand,
                          struct Log* logger,
                          struct Allocator* alloc);
# 23 "net/SwitchPinger.c" 2
# 1 "./util/version/Version.h" 1
# 19 "./util/version/Version.h"
struct Linker_x034730940434702484;
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
# 24 "net/SwitchPinger.c" 2

# 1 "./wire/RouteHeader.h" 1
# 19 "./wire/RouteHeader.h"
# 1 "./wire/SwitchHeader.h" 1
# 50 "./wire/SwitchHeader.h"
#pragma pack(push)
#pragma pack(4)
struct SwitchHeader
{

    uint64_t label_be;






    uint8_t congestAndSuppressErrors;






    uint8_t versionAndLabelShift;
# 78 "./wire/SwitchHeader.h"
    uint16_t trafficClass_be;
};

void Assert_compileTime(char x[1 - (!(sizeof(struct SwitchHeader) == 12))]);
#pragma pack(pop)





static inline uint32_t SwitchHeader_getVersion(const struct SwitchHeader* header)
{
    return header->versionAndLabelShift >> 6;
}

static inline void SwitchHeader_setVersion(struct SwitchHeader* header, uint8_t version)
{
    do { if (!((version < 4))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("SwitchHeader.h"), (95), "(version < 4)"); } } while (0);
    header->versionAndLabelShift =
        (version << 6) | (header->versionAndLabelShift & ( (1 << (6)) - 1 ));
}

static inline uint32_t SwitchHeader_getLabelShift(const struct SwitchHeader* header)
{
    return header->versionAndLabelShift & ( (1 << (6)) - 1 );
}

static inline void SwitchHeader_setLabelShift(struct SwitchHeader* header, uint32_t shift)
{
    do { if (!((shift < 64))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("SwitchHeader.h"), (107), "(shift < 64)"); } } while (0);
    header->versionAndLabelShift = header->versionAndLabelShift >> 6 << 6;
    header->versionAndLabelShift |= shift;
}

static inline uint32_t SwitchHeader_getCongestion(const struct SwitchHeader* header)
{
    return header->congestAndSuppressErrors >> 1;
}

static inline void SwitchHeader_setCongestion(struct SwitchHeader* header, uint32_t cong)
{
    do { if (!((cong <= 127))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("SwitchHeader.h"), (119), "(cong <= 127)"); } } while (0);
    if (!cong) { cong++; }
    header->congestAndSuppressErrors = (header->congestAndSuppressErrors & 1) | (cong << 1);
}

static inline uint16_t SwitchHeader_getTrafficClass(const struct SwitchHeader* header)
{
    return ((uint16_t)(__extension__ ({ uint16_t _x = (((uint16_t)(header->trafficClass_be))); _x >> 8 | _x << 8; })));
}

static inline void SwitchHeader_setTrafficClass(struct SwitchHeader* header, uint16_t tc)
{
    header->trafficClass_be = ((uint16_t)(__extension__ ({ uint16_t _x = (((uint16_t)(tc))); _x >> 8 | _x << 8; })));
}

static inline 
# 134 "./wire/SwitchHeader.h" 3 4
             _Bool 
# 134 "./wire/SwitchHeader.h"
                  SwitchHeader_getSuppressErrors(const struct SwitchHeader* header)
{
    return header->congestAndSuppressErrors & 1;
}

static inline void SwitchHeader_setSuppressErrors(struct SwitchHeader* header, 
# 139 "./wire/SwitchHeader.h" 3 4
                                                                              _Bool 
# 139 "./wire/SwitchHeader.h"
                                                                                   suppress)
{
    header->congestAndSuppressErrors = header->congestAndSuppressErrors >> 1 << 1;
    header->congestAndSuppressErrors |= suppress;
}
# 20 "./wire/RouteHeader.h" 2
# 36 "./wire/RouteHeader.h"
struct RouteHeader
{

    uint8_t publicKey[32];






    struct SwitchHeader sh;


    uint32_t version_be;
# 58 "./wire/RouteHeader.h"
    uint8_t flags;

    uint8_t unused;
    uint16_t alsoUnused;


    uint8_t ip6[16];
};

void Assert_compileTime(char x[1 - (!((56 + 12) == sizeof(struct RouteHeader)))]);
# 26 "net/SwitchPinger.c" 2
# 1 "./wire/Control.h" 1
# 30 "./wire/Control.h"
struct Control_Error
{

    uint32_t errorType_be;


    struct SwitchHeader cause;


    uint32_t causeHandle;
};
void Assert_compileTime(char x[1 - (!(sizeof(struct Control_Error) == (4 + 12 + 4)))]);
# 51 "./wire/Control.h"
struct Control_Ping
{

    uint32_t magic;


    uint32_t version_be;






    uint8_t data[4];
};
void Assert_compileTime(char x[1 - (!(sizeof(struct Control_Ping) == 8 + 4))]);
# 90 "./wire/Control.h"
struct Control_KeyPing
{

    uint32_t magic;


    uint32_t version_be;


    uint8_t key[32];
};
void Assert_compileTime(char x[1 - (!(sizeof(struct Control_KeyPing) == 40))]);
# 114 "./wire/Control.h"
struct Control_GetSnode
{


    uint32_t magic;


    uint32_t version_be;



    uint32_t snodeVersion_be;
# 135 "./wire/Control.h"
    uint32_t kbps_be;



    uint8_t snodeKey[32];



    uint8_t pathToSnode_be[8];
};
void Assert_compileTime(char x[1 - (!(sizeof(struct Control_GetSnode) == 56))]);

static inline char* Control_typeString(uint16_t type_be)
{
    if (type_be == ((uint16_t)(__extension__ ({ uint16_t _x = (((uint16_t)(2))); _x >> 8 | _x << 8; })))) {
        return "ERROR";
    } else if (type_be == ((uint16_t)(__extension__ ({ uint16_t _x = (((uint16_t)(3))); _x >> 8 | _x << 8; })))) {
        return "PING";
    } else if (type_be == ((uint16_t)(__extension__ ({ uint16_t _x = (((uint16_t)(4))); _x >> 8 | _x << 8; })))) {
        return "PONG";
    } else if (type_be == ((uint16_t)(__extension__ ({ uint16_t _x = (((uint16_t)(5))); _x >> 8 | _x << 8; })))) {
        return "KEYPING";
    } else if (type_be == ((uint16_t)(__extension__ ({ uint16_t _x = (((uint16_t)(6))); _x >> 8 | _x << 8; })))) {
        return "KEYPONG";
    } else if (type_be == ((uint16_t)(__extension__ ({ uint16_t _x = (((uint16_t)(7))); _x >> 8 | _x << 8; })))) {
        return "GETSNODE_QUERY";
    } else if (type_be == ((uint16_t)(__extension__ ({ uint16_t _x = (((uint16_t)(8))); _x >> 8 | _x << 8; })))) {
        return "GETSNODE_REPLY";
    } else {
        return "UNKNOWN";
    }
}

struct Control_Header
{




    uint16_t checksum_be;


    uint16_t type_be;
};

void Assert_compileTime(char x[1 - (!(sizeof(struct Control_Header) == 4))]);
# 195 "./wire/Control.h"
struct Control
{
    struct Control_Header header;

    union {
        struct Control_Error error;
        struct Control_Ping ping;
        struct Control_Ping pong;
        struct Control_KeyPing keyPing;
        struct Control_Ping keyPong;
        struct Control_GetSnode getSnode;


        uint8_t bytes[4];
    } content;
};

void Assert_compileTime(char x[1 - (!(sizeof(struct Control) == (4 + 56)))])

 ;
# 27 "net/SwitchPinger.c" 2
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
# 28 "net/SwitchPinger.c" 2


struct SwitchPinger_pvt
{
    struct SwitchPinger pub;

    struct Pinger* pinger;

    struct Admin* admin;

    struct Log* logger;

    struct Allocator* allocator;

    struct Address* myAddr;





    uint64_t incomingLabel;


    uint32_t incomingVersion;

    uint8_t incomingKey[32];
    struct Address incomingSnodeAddr;
    uint32_t incomingSnodeKbps;


    int error;


    int outstandingPings;


    int maxConcurrentPings;

    unsigned long Identity_verifier;
};

struct Ping
{
    struct SwitchPinger_Ping pub;
    uint64_t label;
    String* data;
    struct SwitchPinger_pvt* context;
    SwitchPinger_ResponseCallback onResponse;
    void* onResponseContext;
    struct Pinger_Ping* pingerPing;
    unsigned long Identity_verifier;
};


static __attribute__ ((warn_unused_result)) struct Iface* messageFromControlHandler(struct Message* msg, struct Iface* iface)
{
    struct SwitchPinger_pvt* ctx = (__extension__ ({ __typeof__((struct SwitchPinger_pvt*) iface) Identity_ptr = ((struct SwitchPinger_pvt*) iface); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0xbc9d7023841e85b1ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("SwitchPinger.c"), (84), "(Identity_ptr->Identity_verifier == ((unsigned long) 0xbc9d7023841e85b1ull))"); } } while (0); Identity_ptr; }));
    struct RouteHeader rh;
    do {             struct Er_Ret* Er_ret = Message_epop(msg, &rh, (56 + 12));                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("SwitchPinger.c", 86, Er_ret);                 } else {                     Er__assertFail(Er_unwind("SwitchPinger.c", 86, Er_ret));                 }             }                  } while (0);
    ctx->incomingLabel = ((uint64_t)__builtin_bswap64(((uint64_t)(rh.sh.label_be))));
    ctx->incomingVersion = 0;

    struct Control* ctrl = (struct Control*) msg->bytes;
    if (ctrl->header.type_be == ((uint16_t)(__extension__ ({ uint16_t _x = (((uint16_t)(4))); _x >> 8 | _x << 8; })))) {
        do {             struct Er_Ret* Er_ret = Message_eshift(msg, -4);                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("SwitchPinger.c", 92, Er_ret);                 } else {                     Er__assertFail(Er_unwind("SwitchPinger.c", 92, Er_ret));                 }             }                  } while (0);
        ctx->error = 0;
        if (msg->length >= 8) {
            struct Control_Ping* pongHeader = (struct Control_Ping*) msg->bytes;
            ctx->incomingVersion = ((uint32_t)__builtin_bswap32(((uint32_t)(pongHeader->version_be))));
            if (pongHeader->magic != ((uint32_t)__builtin_bswap32(((uint32_t)(0x9d74e35b))))) {
                do { if (ctx->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(ctx->logger, Log_Level_DEBUG, "SwitchPinger.c", 98, "dropped invalid switch pong"); } } while (0);
                return 
# 99 "net/SwitchPinger.c" 3 4
                      ((void *)0)
# 99 "net/SwitchPinger.c"
                          ;
            }
            do {             struct Er_Ret* Er_ret = Message_eshift(msg, -8);                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("SwitchPinger.c", 101, Er_ret);                 } else {                     Er__assertFail(Er_unwind("SwitchPinger.c", 101, Er_ret));                 }             }                  } while (0);
        } else {
            do { if (ctx->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(ctx->logger, Log_Level_DEBUG, "SwitchPinger.c", 103, "got runt pong message, length: [%d]", msg->length); } } while (0);
            return 
# 104 "net/SwitchPinger.c" 3 4
                  ((void *)0)
# 104 "net/SwitchPinger.c"
                      ;
        }

    } else if (ctrl->header.type_be == ((uint16_t)(__extension__ ({ uint16_t _x = (((uint16_t)(6))); _x >> 8 | _x << 8; })))) {
        do {             struct Er_Ret* Er_ret = Message_eshift(msg, -4);                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("SwitchPinger.c", 108, Er_ret);                 } else {                     Er__assertFail(Er_unwind("SwitchPinger.c", 108, Er_ret));                 }             }                  } while (0);
        ctx->error = 0;
        if (msg->length >= 40 && msg->length <= (40 + 64)) {
            struct Control_KeyPing* pongHeader = (struct Control_KeyPing*) msg->bytes;
            ctx->incomingVersion = ((uint32_t)__builtin_bswap32(((uint32_t)(pongHeader->version_be))));
            if (pongHeader->magic != ((uint32_t)__builtin_bswap32(((uint32_t)(0x89abcdef))))) {
                do { if (ctx->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(ctx->logger, Log_Level_DEBUG, "SwitchPinger.c", 114, "dropped invalid switch key-pong"); } } while (0);
                return 
# 115 "net/SwitchPinger.c" 3 4
                      ((void *)0)
# 115 "net/SwitchPinger.c"
                          ;
            }
            Bits__memcpy(ctx->incomingKey,pongHeader->key,32,"SwitchPinger.c",117);
            do {             struct Er_Ret* Er_ret = Message_eshift(msg, -40);                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("SwitchPinger.c", 118, Er_ret);                 } else {                     Er__assertFail(Er_unwind("SwitchPinger.c", 118, Er_ret));                 }             }                  } while (0);
        } else if (msg->length > (40 + 64)) {
            do { if (ctx->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(ctx->logger, Log_Level_DEBUG, "SwitchPinger.c", 120, "got overlong key-pong message, length: [%d]", msg->length); } } while (0);
            return 
# 121 "net/SwitchPinger.c" 3 4
                  ((void *)0)
# 121 "net/SwitchPinger.c"
                      ;
        } else {
            do { if (ctx->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(ctx->logger, Log_Level_DEBUG, "SwitchPinger.c", 123, "got runt key-pong message, length: [%d]", msg->length); } } while (0);
            return 
# 124 "net/SwitchPinger.c" 3 4
                  ((void *)0)
# 124 "net/SwitchPinger.c"
                      ;
        }

    } else if (ctrl->header.type_be == ((uint16_t)(__extension__ ({ uint16_t _x = (((uint16_t)(8))); _x >> 8 | _x << 8; })))) {
        do {             struct Er_Ret* Er_ret = Message_eshift(msg, -4);                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("SwitchPinger.c", 128, Er_ret);                 } else {                     Er__assertFail(Er_unwind("SwitchPinger.c", 128, Er_ret));                 }             }                  } while (0);
        ctx->error = 0;
        if (msg->length < 56) {
            do { if (ctx->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(ctx->logger, Log_Level_DEBUG, "SwitchPinger.c", 131, "got runt GetSnode message, length: [%d]", msg->length); } } while (0);
            return 
# 132 "net/SwitchPinger.c" 3 4
                  ((void *)0)
# 132 "net/SwitchPinger.c"
                      ;
        }
        struct Control_GetSnode* hdr = (struct Control_GetSnode*) msg->bytes;
        if (hdr->magic != ((uint32_t)__builtin_bswap32(((uint32_t)(0x736e6f72))))) {
            do { if (ctx->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(ctx->logger, Log_Level_DEBUG, "SwitchPinger.c", 136, "dropped invalid GetSnode"); } } while (0);
            return 
# 137 "net/SwitchPinger.c" 3 4
                  ((void *)0)
# 137 "net/SwitchPinger.c"
                      ;
        }
        if (!AddressCalc_addressForPublicKey(ctx->incomingSnodeAddr.ip6.bytes, hdr->snodeKey)) {
            do { if (ctx->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(ctx->logger, Log_Level_DEBUG, "SwitchPinger.c", 140, "dropped invalid GetSnode key"); } } while (0);
            return 
# 141 "net/SwitchPinger.c" 3 4
                  ((void *)0)
# 141 "net/SwitchPinger.c"
                      ;
        }
        ctx->incomingVersion = ((uint32_t)__builtin_bswap32(((uint32_t)(hdr->version_be))));
        Bits__memcpy(ctx->incomingSnodeAddr.key,hdr->snodeKey,32,"SwitchPinger.c",144);
        uint64_t pathToSnode_be;
        Bits__memcpy(&pathToSnode_be,hdr->pathToSnode_be,8,"SwitchPinger.c",146);
        ctx->incomingSnodeAddr.path = ((uint64_t)__builtin_bswap64(((uint64_t)(pathToSnode_be))));
        ctx->incomingSnodeAddr.protocolVersion = ((uint32_t)__builtin_bswap32(((uint32_t)(hdr->snodeVersion_be))));
        ctx->incomingSnodeKbps = ((uint32_t)__builtin_bswap32(((uint32_t)(hdr->kbps_be))));
        do {             struct Er_Ret* Er_ret = Message_eshift(msg, -56);                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("SwitchPinger.c", 150, Er_ret);                 } else {                     Er__assertFail(Er_unwind("SwitchPinger.c", 150, Er_ret));                 }             }                  } while (0);

    } else if (ctrl->header.type_be == ((uint16_t)(__extension__ ({ uint16_t _x = (((uint16_t)(2))); _x >> 8 | _x << 8; })))) {
        do {             struct Er_Ret* Er_ret = Message_eshift(msg, -4);                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("SwitchPinger.c", 153, Er_ret);                 } else {                     Er__assertFail(Er_unwind("SwitchPinger.c", 153, Er_ret));                 }             }                  } while (0);
        do { if (!(((uint8_t*)&ctrl->content.error.errorType_be == msg->bytes))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("SwitchPinger.c"), (154), "((uint8_t*)&ctrl->content.error.errorType_be == msg->bytes)"); } } while (0);
        if (msg->length < (4 + 12 + 4)) {
            do { if (ctx->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(ctx->logger, Log_Level_DEBUG, "SwitchPinger.c", 156, "runt error packet"); } } while (0);
            return 
# 157 "net/SwitchPinger.c" 3 4
                  ((void *)0)
# 157 "net/SwitchPinger.c"
                      ;
        }

        ctx->error = (__extension__({             uint32_t Er_returnVal;             __builtin_memset(&Er_returnVal, 0, sizeof(Er_returnVal));             struct Er_Ret* Er_ret = Message_epop32be(&Er_returnVal, msg);                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("SwitchPinger.c", 160, Er_ret);                 } else {                     Er__assertFail(Er_unwind("SwitchPinger.c", 160, Er_ret));                 }             }                      Er_returnVal;         }));
        do {             struct Er_Ret* Er_ret = Message_epush32h(msg, ((uint32_t)__builtin_bswap32(((uint32_t)(0)))));                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("SwitchPinger.c", 161, Er_ret);                 } else {                     Er__assertFail(Er_unwind("SwitchPinger.c", 161, Er_ret));                 }             }                  } while (0);

        do {             struct Er_Ret* Er_ret = Message_eshift(msg, -(4 + 12));                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("SwitchPinger.c", 163, Er_ret);                 } else {                     Er__assertFail(Er_unwind("SwitchPinger.c", 163, Er_ret));                 }             }                  } while (0);

        struct Control* origCtrl = (struct Control*) msg->bytes;

        do { if (ctx->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(ctx->logger, Log_Level_DEBUG, "SwitchPinger.c", 167, "error [%s] was caused by our [%s]", Error_strerror(ctx->error), Control_typeString(origCtrl->header.type_be)); } } while (0)

                                                               ;

        int shift;
        if (origCtrl->header.type_be == ((uint16_t)(__extension__ ({ uint16_t _x = (((uint16_t)(3))); _x >> 8 | _x << 8; })))) {
            shift = -(4 + 8);
        } else if (origCtrl->header.type_be == ((uint16_t)(__extension__ ({ uint16_t _x = (((uint16_t)(5))); _x >> 8 | _x << 8; })))) {
            shift = -(4 + 40);
        } else {
            Assert_failure("problem in Ducttape.c");
        }
        if (msg->length < -shift) {
            do { if (ctx->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(ctx->logger, Log_Level_DEBUG, "SwitchPinger.c", 180, "runt error packet"); } } while (0);
        }
        do {             struct Er_Ret* Er_ret = Message_eshift(msg, shift);                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("SwitchPinger.c", 182, Er_ret);                 } else {                     Er__assertFail(Er_unwind("SwitchPinger.c", 182, Er_ret));                 }             }                  } while (0);

    } else {

        do { if (!((
# 186 "net/SwitchPinger.c" 3 4
       0
# 186 "net/SwitchPinger.c"
       ))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("SwitchPinger.c"), (186), "(0)"); } } while (0);
    }

    String* msgStr = &(String) { .bytes = (char*) msg->bytes, .len = msg->length };
    Pinger_pongReceived(msgStr, ctx->pinger);
    __builtin_memset(ctx->incomingKey,0,32);
    return 
# 192 "net/SwitchPinger.c" 3 4
          ((void *)0)
# 192 "net/SwitchPinger.c"
              ;
}

static void onPingResponse(String* data, uint32_t milliseconds, void* vping)
{
    struct Ping* p = (__extension__ ({ __typeof__((struct Ping*) vping) Identity_ptr = ((struct Ping*) vping); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0xbc9d7023841e85b1ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("SwitchPinger.c"), (197), "(Identity_ptr->Identity_verifier == ((unsigned long) 0xbc9d7023841e85b1ull))"); } } while (0); Identity_ptr; }));
    enum SwitchPinger_Result err = SwitchPinger_Result_OK;
    uint64_t label = p->context->incomingLabel;
    if (data) {
        if (label != p->label) {
            err = SwitchPinger_Result_LABEL_MISMATCH;
        } else if ((p->data || data->len > 0) && !String_equals(data, p->data)) {
            err = SwitchPinger_Result_WRONG_DATA;
        } else if (p->context->error == 9) {
            err = SwitchPinger_Result_LOOP_ROUTE;
        } else if (p->context->error) {
            err = SwitchPinger_Result_ERROR_RESPONSE;
        }
    } else {
        err = SwitchPinger_Result_TIMEOUT;
    }

    uint32_t version = p->context->incomingVersion;
    struct SwitchPinger_Response* resp =
        Allocator__calloc((p->pub.pingAlloc),(sizeof(struct SwitchPinger_Response)),(1),"SwitchPinger.c",216);
    resp->version = p->context->incomingVersion;
    resp->res = err;
    resp->label = label;
    resp->data = data;
    resp->milliseconds = milliseconds;
    resp->version = version;
    Bits__memcpy(resp->key,p->context->incomingKey,32,"SwitchPinger.c",223);
    Bits__memcpy(&resp->snode,&p->context->incomingSnodeAddr,sizeof(struct Address),"SwitchPinger.c",224);
    resp->kbpsLimit = p->context->incomingSnodeKbps;
    resp->ping = &p->pub;
    p->onResponse(resp, p->pub.onResponseContext);
}

static void sendPing(String* data, void* sendPingContext)
{
    struct Ping* p = (__extension__ ({ __typeof__((struct Ping*) sendPingContext) Identity_ptr = ((struct Ping*) sendPingContext); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0xbc9d7023841e85b1ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("SwitchPinger.c"), (232), "(Identity_ptr->Identity_verifier == ((unsigned long) 0xbc9d7023841e85b1ull))"); } } while (0); Identity_ptr; }));

    struct Message* msg = Message_new(0, data->len + 512, p->pub.pingAlloc);

    while (((uintptr_t)msg->bytes - data->len) % 4) {
        do {             struct Er_Ret* Er_ret = Message_epush8h(msg, 0);                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("SwitchPinger.c", 237, Er_ret);                 } else {                     Er__assertFail(Er_unwind("SwitchPinger.c", 237, Er_ret));                 }             }                  } while (0);
    }
    msg->length = 0;

    do {             struct Er_Ret* Er_ret = Message_epush(msg, data->bytes, data->len);                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("SwitchPinger.c", 241, Er_ret);                 } else {                     Er__assertFail(Er_unwind("SwitchPinger.c", 241, Er_ret));                 }             }                  } while (0);
    do { if (!((!((uintptr_t)msg->bytes % 4) && "alignment fault"))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("SwitchPinger.c"), (242), "(!((uintptr_t)msg->bytes % 4) && \"alignment fault\")"); } } while (0);

    if (p->pub.type == SwitchPinger_Type_KEYPING) {
        do {             struct Er_Ret* Er_ret = Message_epush(msg,        ((void *)0)       , 40);                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("SwitchPinger.c", 245, Er_ret);                 } else {                     Er__assertFail(Er_unwind("SwitchPinger.c", 245, Er_ret));                 }             }                  } while (0);
        struct Control_KeyPing* keyPingHeader = (struct Control_KeyPing*) msg->bytes;
        keyPingHeader->magic = ((uint32_t)__builtin_bswap32(((uint32_t)(0x01234567))));
        keyPingHeader->version_be = ((uint32_t)__builtin_bswap32(((uint32_t)(20))));
        Bits__memcpy(keyPingHeader->key,p->context->myAddr->key,32,"SwitchPinger.c",249);
    } else if (p->pub.type == SwitchPinger_Type_PING) {
        do {             struct Er_Ret* Er_ret = Message_epush(msg,        ((void *)0)       , 8);                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("SwitchPinger.c", 251, Er_ret);                 } else {                     Er__assertFail(Er_unwind("SwitchPinger.c", 251, Er_ret));                 }             }                  } while (0);
        struct Control_Ping* pingHeader = (struct Control_Ping*) msg->bytes;
        pingHeader->magic = ((uint32_t)__builtin_bswap32(((uint32_t)(0x09f91102))));
        pingHeader->version_be = ((uint32_t)__builtin_bswap32(((uint32_t)(20))));
    } else if (p->pub.type == SwitchPinger_Type_GETSNODE) {
        do {             struct Er_Ret* Er_ret = Message_epush(msg,        ((void *)0)       , 56);                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("SwitchPinger.c", 256, Er_ret);                 } else {                     Er__assertFail(Er_unwind("SwitchPinger.c", 256, Er_ret));                 }             }                  } while (0);
        struct Control_GetSnode* hdr = (struct Control_GetSnode*) msg->bytes;
        hdr->magic = ((uint32_t)__builtin_bswap32(((uint32_t)(0x736e6f71))));
        hdr->version_be = ((uint32_t)__builtin_bswap32(((uint32_t)(20))));
        hdr->kbps_be = ((uint32_t)__builtin_bswap32(((uint32_t)(p->pub.kbpsLimit))));
        Bits__memcpy(hdr->snodeKey,p->pub.snode.key,32,"SwitchPinger.c",261);
        uint64_t pathToSnode_be = ((uint64_t)__builtin_bswap64(((uint64_t)(p->pub.snode.path))));
        Bits__memcpy(hdr->pathToSnode_be,&pathToSnode_be,8,"SwitchPinger.c",263);
        hdr->snodeVersion_be = ((uint32_t)__builtin_bswap32(((uint32_t)(p->pub.snode.protocolVersion))));
    } else {
        Assert_failure("unexpected ping type");
    }

    do {             struct Er_Ret* Er_ret = Message_eshift(msg, 4);                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("SwitchPinger.c", 269, Er_ret);                 } else {                     Er__assertFail(Er_unwind("SwitchPinger.c", 269, Er_ret));                 }             }                  } while (0);
    struct Control* ctrl = (struct Control*) msg->bytes;
    ctrl->header.checksum_be = 0;
    if (p->pub.type == SwitchPinger_Type_PING) {
        ctrl->header.type_be = ((uint16_t)(__extension__ ({ uint16_t _x = (((uint16_t)(3))); _x >> 8 | _x << 8; })));
    } else if (p->pub.type == SwitchPinger_Type_KEYPING) {
        ctrl->header.type_be = ((uint16_t)(__extension__ ({ uint16_t _x = (((uint16_t)(5))); _x >> 8 | _x << 8; })));
    } else if (p->pub.type == SwitchPinger_Type_GETSNODE) {
        ctrl->header.type_be = ((uint16_t)(__extension__ ({ uint16_t _x = (((uint16_t)(7))); _x >> 8 | _x << 8; })));
    } else {
        Assert_failure("unexpected type");
    }
    ctrl->header.checksum_be = Checksum_engine(msg->bytes, msg->length);

    struct RouteHeader rh;
    __builtin_memset(&rh,0,(56 + 12));
    rh.flags |= (1<<1);
    rh.sh.label_be = ((uint64_t)__builtin_bswap64(((uint64_t)(p->label))));
    SwitchHeader_setVersion(&rh.sh, 1);

    do {             struct Er_Ret* Er_ret = Message_epush(msg, &rh, (56 + 12));                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("SwitchPinger.c", 289, Er_ret);                 } else {                     Er__assertFail(Er_unwind("SwitchPinger.c", 289, Er_ret));                 }             }                  } while (0);

    Iface_send(&p->context->pub.controlHandlerIf, msg);
}

static String* RESULT_STRING_OK = (&(String) { .bytes = "pong", .len = sizeof("pong") - 1 });
static String* RESULT_STRING_LABEL_MISMATCH = (&(String) { .bytes = "diff_label", .len = sizeof("diff_label") - 1 });
static String* RESULT_STRING_WRONG_DATA = (&(String) { .bytes = "diff_data", .len = sizeof("diff_data") - 1 });
static String* RESULT_STRING_ERROR_RESPONSE = (&(String) { .bytes = "err_switch", .len = sizeof("err_switch") - 1 });
static String* RESULT_STRING_TIMEOUT = (&(String) { .bytes = "timeout", .len = sizeof("timeout") - 1 });
static String* RESULT_STRING_UNKNOWN = (&(String) { .bytes = "err_unknown", .len = sizeof("err_unknown") - 1 });
static String* RESULT_STRING_LOOP = (&(String) { .bytes = "err_loop", .len = sizeof("err_loop") - 1 });

String* SwitchPinger_resultString(enum SwitchPinger_Result result)
{
    switch (result) {
        case SwitchPinger_Result_OK:
            return RESULT_STRING_OK;

        case SwitchPinger_Result_LABEL_MISMATCH:
            return RESULT_STRING_LABEL_MISMATCH;

        case SwitchPinger_Result_WRONG_DATA:
            return RESULT_STRING_WRONG_DATA;

        case SwitchPinger_Result_ERROR_RESPONSE:
            return RESULT_STRING_ERROR_RESPONSE;

        case SwitchPinger_Result_TIMEOUT:
            return RESULT_STRING_TIMEOUT;

        case SwitchPinger_Result_LOOP_ROUTE:
            return RESULT_STRING_LOOP;

        default:
            return RESULT_STRING_UNKNOWN;
    };
}

static int onPingFree(struct Allocator_OnFreeJob* job)
{
    struct Ping* ping = (__extension__ ({ __typeof__((struct Ping*)job->userData) Identity_ptr = ((struct Ping*)job->userData); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0xbc9d7023841e85b1ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("SwitchPinger.c"), (330), "(Identity_ptr->Identity_verifier == ((unsigned long) 0xbc9d7023841e85b1ull))"); } } while (0); Identity_ptr; }));
    struct SwitchPinger_pvt* ctx = (__extension__ ({ __typeof__(ping->context) Identity_ptr = (ping->context); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0xbc9d7023841e85b1ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("SwitchPinger.c"), (331), "(Identity_ptr->Identity_verifier == ((unsigned long) 0xbc9d7023841e85b1ull))"); } } while (0); Identity_ptr; }));
    ctx->outstandingPings--;
    do { if (!((ctx->outstandingPings >= 0))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("SwitchPinger.c"), (333), "(ctx->outstandingPings >= 0)"); } } while (0);
    return 0;
}

struct SwitchPinger_Ping* SwitchPinger_newPing(uint64_t label,
                                               String* data,
                                               uint32_t timeoutMilliseconds,
                                               SwitchPinger_ResponseCallback onResponse,
                                               struct Allocator* alloc,
                                               struct SwitchPinger* context)
{
    struct SwitchPinger_pvt* ctx = (__extension__ ({ __typeof__((struct SwitchPinger_pvt*)context) Identity_ptr = ((struct SwitchPinger_pvt*)context); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0xbc9d7023841e85b1ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("SwitchPinger.c"), (344), "(Identity_ptr->Identity_verifier == ((unsigned long) 0xbc9d7023841e85b1ull))"); } } while (0); Identity_ptr; }));
    if (data && data->len > 256) {
        return 
# 346 "net/SwitchPinger.c" 3 4
              ((void *)0)
# 346 "net/SwitchPinger.c"
                  ;
    }

    if (ctx->outstandingPings > ctx->maxConcurrentPings) {
        do { if (ctx->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(ctx->logger, Log_Level_DEBUG, "SwitchPinger.c", 350, "Skipping switch ping because there are already [%d] outstanding", ctx->outstandingPings); } } while (0)
                                        ;
        return 
# 352 "net/SwitchPinger.c" 3 4
              ((void *)0)
# 352 "net/SwitchPinger.c"
                  ;
    }

    struct Pinger_Ping* pp =
        Pinger_newPing(data, onPingResponse, sendPing, timeoutMilliseconds, alloc, ctx->pinger);

    struct Ping* ping = Allocator__clone((pp->pingAlloc),((&(struct Ping) { .pub = { .pingAlloc = pp->pingAlloc }, .label = label, .data = ((data) ? String_newBinary(data->bytes, data->len, pp->pingAlloc) : 
# 358 "net/SwitchPinger.c" 3 4
                       ((void *)0)
# 358 "net/SwitchPinger.c"
                       ), .context = ctx, .onResponse = onResponse, .pingerPing = pp })),sizeof(*((&(struct Ping) { .pub = { .pingAlloc = pp->pingAlloc }, .label = label, .data = ((data) ? String_newBinary(data->bytes, data->len, pp->pingAlloc) : 
# 358 "net/SwitchPinger.c" 3 4
                       ((void *)0)
# 358 "net/SwitchPinger.c"
                       ), .context = ctx, .onResponse = onResponse, .pingerPing = pp }))),"SwitchPinger.c",358)
# 367 "net/SwitchPinger.c"
       ;
    (ping)->Identity_verifier = ((unsigned long) 0xbc9d7023841e85b1ull);
    Allocator__onFree((pp->pingAlloc), (onPingFree), (ping), "SwitchPinger.c", 369);
    pp->context = ping;
    ctx->outstandingPings++;

    return &ping->pub;
}

struct SwitchPinger* SwitchPinger_new(struct EventBase* eventBase,
                                      struct Random* rand,
                                      struct Log* logger,
                                      struct Address* myAddr,
                                      struct Allocator* allocator)
{
    struct Allocator* alloc = Allocator__child((allocator),"SwitchPinger.c",382);
    struct SwitchPinger_pvt* sp = Allocator__malloc((alloc),(sizeof(struct SwitchPinger_pvt)),"SwitchPinger.c",383);
    Bits__memcpy(sp,(&(struct SwitchPinger_pvt) { .pub = { .controlHandlerIf = { .send = messageFromControlHandler } }, .pinger = Pinger_new(eventBase, rand, logger, alloc), .logger = logger, .allocator = alloc, .myAddr = myAddr, .maxConcurrentPings = 50, }),sizeof(struct SwitchPinger_pvt),"SwitchPinger.c",384)
# 395 "net/SwitchPinger.c"
                                        ;
    (sp)->Identity_verifier = ((unsigned long) 0xbc9d7023841e85b1ull);
    return &sp->pub;
}
