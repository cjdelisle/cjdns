# 1 "dht/dhtcore/Janitor.c"
# 1 "/home/jojapoppa/cjdns//"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "dht/dhtcore/Janitor.c"
# 15 "dht/dhtcore/Janitor.c"
# 1 "./crypto/AddressCalc.h" 1
# 18 "./crypto/AddressCalc.h"
# 1 "./util/Linker.h" 1
# 19 "./crypto/AddressCalc.h" 2
struct Linker_x007821714511260103;

# 1 "/usr/lib/gcc/x86_64-w64-mingw32/9.2-win32/include/stdint.h" 1 3 4
# 9 "/usr/lib/gcc/x86_64-w64-mingw32/9.2-win32/include/stdint.h" 3 4
# 1 "/usr/share/mingw-w64/include/stdint.h" 1 3 4
# 28 "/usr/share/mingw-w64/include/stdint.h" 3 4
# 1 "/usr/share/mingw-w64/include/crtdefs.h" 1 3 4
# 10 "/usr/share/mingw-w64/include/crtdefs.h" 3 4
# 1 "/usr/share/mingw-w64/include/_mingw.h" 1 3 4
# 12 "/usr/share/mingw-w64/include/_mingw.h" 3 4
# 1 "/usr/share/mingw-w64/include/_mingw_mac.h" 1 3 4
# 98 "/usr/share/mingw-w64/include/_mingw_mac.h" 3 4
             
# 107 "/usr/share/mingw-w64/include/_mingw_mac.h" 3 4
             
# 13 "/usr/share/mingw-w64/include/_mingw.h" 2 3 4
# 1 "/usr/share/mingw-w64/include/_mingw_secapi.h" 1 3 4
# 14 "/usr/share/mingw-w64/include/_mingw.h" 2 3 4
# 282 "/usr/share/mingw-w64/include/_mingw.h" 3 4
# 1 "/usr/share/mingw-w64/include/vadefs.h" 1 3 4
# 9 "/usr/share/mingw-w64/include/vadefs.h" 3 4
# 1 "/usr/share/mingw-w64/include/_mingw.h" 1 3 4
# 595 "/usr/share/mingw-w64/include/_mingw.h" 3 4
# 1 "/usr/share/mingw-w64/include/sdks/_mingw_directx.h" 1 3 4
# 596 "/usr/share/mingw-w64/include/_mingw.h" 2 3 4
# 1 "/usr/share/mingw-w64/include/sdks/_mingw_ddk.h" 1 3 4
# 597 "/usr/share/mingw-w64/include/_mingw.h" 2 3 4
# 10 "/usr/share/mingw-w64/include/vadefs.h" 2 3 4




#pragma pack(push,_CRT_PACKING)
# 24 "/usr/share/mingw-w64/include/vadefs.h" 3 4
  
# 24 "/usr/share/mingw-w64/include/vadefs.h" 3 4
 typedef __builtin_va_list __gnuc_va_list;






  typedef __gnuc_va_list va_list;
# 103 "/usr/share/mingw-w64/include/vadefs.h" 3 4
#pragma pack(pop)
# 283 "/usr/share/mingw-w64/include/_mingw.h" 2 3 4
# 569 "/usr/share/mingw-w64/include/_mingw.h" 3 4
void __attribute__((__cdecl__)) __debugbreak(void);
extern __inline__ __attribute__((__always_inline__,__gnu_inline__)) void __attribute__((__cdecl__)) __debugbreak(void)
{
  __asm__ __volatile__("int {$}3":);
}




const char *__mingw_get_crt_info (void);
# 11 "/usr/share/mingw-w64/include/crtdefs.h" 2 3 4




#pragma pack(push,_CRT_PACKING)
# 35 "/usr/share/mingw-w64/include/crtdefs.h" 3 4
__extension__ typedef unsigned long long size_t;
# 45 "/usr/share/mingw-w64/include/crtdefs.h" 3 4
__extension__ typedef long long ssize_t;






typedef size_t rsize_t;
# 62 "/usr/share/mingw-w64/include/crtdefs.h" 3 4
__extension__ typedef long long intptr_t;
# 75 "/usr/share/mingw-w64/include/crtdefs.h" 3 4
__extension__ typedef unsigned long long uintptr_t;
# 88 "/usr/share/mingw-w64/include/crtdefs.h" 3 4
__extension__ typedef long long ptrdiff_t;
# 98 "/usr/share/mingw-w64/include/crtdefs.h" 3 4
typedef unsigned short wchar_t;







typedef unsigned short wint_t;
typedef unsigned short wctype_t;





typedef int errno_t;




typedef long __time32_t;




__extension__ typedef long long __time64_t;
# 138 "/usr/share/mingw-w64/include/crtdefs.h" 3 4
typedef __time64_t time_t;
# 422 "/usr/share/mingw-w64/include/crtdefs.h" 3 4
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
# 29 "/usr/share/mingw-w64/include/stdint.h" 2 3 4



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
# 22 "./crypto/AddressCalc.h" 2
# 1 "/usr/lib/gcc/x86_64-w64-mingw32/9.2-win32/include/stdbool.h" 1 3 4
# 23 "./crypto/AddressCalc.h" 2
# 51 "./crypto/AddressCalc.h"
_Bool 
# 51 "./crypto/AddressCalc.h"
    AddressCalc_validAddress(const uint8_t address[16]);





void AddressCalc_makeValidAddress(uint8_t address[16]);
# 66 "./crypto/AddressCalc.h"

# 66 "./crypto/AddressCalc.h" 3 4
_Bool 
# 66 "./crypto/AddressCalc.h"
    AddressCalc_addressForPublicKey(uint8_t addressOut[16], const uint8_t key[32]);
# 16 "dht/dhtcore/Janitor.c" 2
# 1 "./crypto/random/Random.h" 1
# 18 "./crypto/random/Random.h"
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

struct Linker_x024916270368322269;
# 33 "./util/Assert.h"
__attribute__((__format__ (__printf__, 1, 2)))
__attribute__((__noreturn__))
void Assert_failure(const char* format, ...);
# 19 "./util/Identity.h" 2
# 1 "./util/Constant.h" 1
# 33 "./util/Constant.h"

# 20 "./util/Identity.h" 2



    
# 19 "./memory/Allocator.h" 2


struct Linker_x07026609748914037;





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
# 19 "./crypto/random/Random.h" 2
# 1 "./exception/Except.h" 1
# 18 "./exception/Except.h"
# 1 "./exception/Er.h" 1
# 23 "./exception/Er.h"
struct Linker_x0007980532754944702;

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


struct Linker_x0923289700854454;



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
struct Linker_x04696465219307975;

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
struct Linker_x0169631886285859;



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

struct Linker_x06057547242075398;



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
# 17 "dht/dhtcore/Janitor.c" 2
# 1 "./dht/Address.h" 1
# 18 "./dht/Address.h"
# 1 "./benc/String.h" 1
# 18 "./benc/String.h"
# 1 "./benc/Object.h" 1
# 18 "./benc/Object.h"
# 1 "/usr/share/mingw-w64/include/inttypes.h" 1 3
# 14 "/usr/share/mingw-w64/include/inttypes.h" 3
# 1 "/usr/lib/gcc/x86_64-w64-mingw32/9.2-win32/include/stddef.h" 1 3 4
# 1 "/usr/share/mingw-w64/include/stddef.h" 1 3 4
# 2 "/usr/lib/gcc/x86_64-w64-mingw32/9.2-win32/include/stddef.h" 2 3 4
# 15 "/usr/share/mingw-w64/include/inttypes.h" 2 3






# 20 "/usr/share/mingw-w64/include/inttypes.h" 3
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
# 19 "./benc/String.h" 2

# 1 "./util/CString.h" 1
# 21 "./util/CString.h"
struct Linker_x09576682346394336;

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

struct Linker_x05114936583755028;


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
# 19 "./dht/Address.h" 2


struct Linker_x09954892926075132;
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
# 18 "dht/dhtcore/Janitor.c" 2
# 1 "./dht/dhtcore/Janitor.h" 1
# 24 "./dht/dhtcore/Janitor.h"
# 1 "./dht/dhtcore/RouterModule.h" 1
# 24 "./dht/dhtcore/RouterModule.h"
# 1 "./dht/DHTModuleRegistry.h" 1
# 22 "./dht/DHTModuleRegistry.h"
# 1 "./benc/Dict.h" 1
# 21 "./benc/Dict.h"
struct Linker_x045946145185364373;

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
# 23 "./dht/DHTModuleRegistry.h" 2
# 1 "./dht/DHTMessage.h" 1
# 37 "./dht/DHTMessage.h"
struct DHTMessage;
struct DHTMessage
{
    struct Address* address;

    struct Message* binMessage;


    Dict* asDict;

    struct EncodingScheme* encodingScheme;
    int encIndex;

    int pleaseRespond;





    struct DHTMessage* replyTo;


    struct Allocator* allocator;
};
# 24 "./dht/DHTModuleRegistry.h" 2

# 1 "./io/Reader.h" 1
# 23 "./io/Reader.h"
struct Reader {
# 33 "./io/Reader.h"
    int (* const read)(struct Reader* thisReader, void* readInto, unsigned long length);
# 42 "./io/Reader.h"
    void (* const skip)(struct Reader* thisReader, unsigned long byteCount);


    uint64_t bytesRead;
};
# 57 "./io/Reader.h"
static inline uint8_t Reader_read8 (struct Reader* reader) { uint8_t num; reader->read(reader, &num, 8/8); return num; }
static inline uint16_t Reader_read16 (struct Reader* reader) { uint16_t num; reader->read(reader, &num, 16/8); return num; }
static inline uint32_t Reader_read32 (struct Reader* reader) { uint32_t num; reader->read(reader, &num, 32/8); return num; }
static inline uint64_t Reader_read64 (struct Reader* reader) { uint64_t num; reader->read(reader, &num, 64/8); return num; }
# 26 "./dht/DHTModuleRegistry.h" 2
# 1 "./io/Writer.h" 1
# 23 "./io/Writer.h"
struct Writer {
# 35 "./io/Writer.h"
    int (* const write)(struct Writer* w, const void* toWrite, unsigned long length);

    uint64_t bytesWritten;
};
# 47 "./io/Writer.h"
static inline int Writer_write8 (struct Writer* writer, uint8_t number) { uint8_t num = number; return writer->write(writer, &num, 8/8); }
static inline int Writer_write16 (struct Writer* writer, uint16_t number) { uint16_t num = number; return writer->write(writer, &num, 16/8); }
static inline int Writer_write32 (struct Writer* writer, uint32_t number) { uint32_t num = number; return writer->write(writer, &num, 32/8); }
static inline int Writer_write64 (struct Writer* writer, uint64_t number) { uint64_t num = number; return writer->write(writer, &num, 64/8); }
# 27 "./dht/DHTModuleRegistry.h" 2


struct Linker_x05475400523133247;



struct DHTModuleRegistry {


    int memberCount;


    struct DHTModule** members;





    Dict* serializedContexts;


    struct Allocator* allocator;
};







struct DHTModuleRegistry* DHTModuleRegistry_new(struct Allocator* allocator);







int DHTModuleRegistry_register(struct DHTModule* module,
                               struct DHTModuleRegistry* registry);
# 78 "./dht/DHTModuleRegistry.h"
void DHTModuleRegistry_handleIncoming(struct DHTMessage* message,
                                      const struct DHTModuleRegistry* registry);




void DHTModuleRegistry_handleOutgoing(struct DHTMessage* message,
                                      const struct DHTModuleRegistry* registry);
# 25 "./dht/dhtcore/RouterModule.h" 2
# 1 "./dht/dhtcore/Node.h" 1
# 23 "./dht/dhtcore/Node.h"
# 1 "./switch/EncodingScheme.h" 1
# 19 "./switch/EncodingScheme.h"
# 1 "./benc/List.h" 1
# 21 "./benc/List.h"
struct Linker_x008484959486748589;

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


struct Linker_x04347090011298109;



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
# 24 "./dht/dhtcore/Node.h" 2



struct Linker_x024848091238666825;

struct Node_Link;

struct Node_Two
{




    uint64_t cost_pvt;


    uint32_t marked;


    uint64_t timeLastPinged;


    struct Address address;


    struct EncodingScheme* encodingScheme;





    struct PeerRBTree {
        struct Node_Link* rbh_root;
    } peerTree;


    struct Node_Link* reversePeers;


    struct Node_Link* bestParent_pvt;


    struct {
        struct Node_Two* rbe_left;
        struct Node_Two* rbe_right;
        struct Node_Two* rbe_parent;
        int rbe_color;
    } nodeTree;

    struct Allocator* alloc;

    unsigned long Identity_verifier;
};






struct Node_Link
{

    struct {
        struct Node_Link* rbe_left;
        struct Node_Link* rbe_right;
        struct Node_Link* rbe_parent;
        int rbe_color;
    } peerTree;





    int inverseLinkEncodingFormNumber;





    uint32_t linkCost;


    uint64_t timeLastSeen;


    struct Node_Two* parent;


    struct Node_Two* child;






    struct Node_Link* nextPeer;


    struct Node_Link* nextInSplitList;







    uint64_t cannonicalLabel;


    uint64_t discoveredPath;

    unsigned long Identity_verifier;
};

static inline uint64_t Node_getCost(struct Node_Two* node)
{
    return node->cost_pvt;
}

static inline struct Node_Link* Node_getBestParent(struct Node_Two* node)
{
    return node->bestParent_pvt;
}


# 148 "./dht/dhtcore/Node.h" 3 4
_Bool 
# 148 "./dht/dhtcore/Node.h"
    Node_isAncestorOf(struct Node_Two* ancestor, struct Node_Two* child);

void Node_setParentCostAndPath(struct Node_Two* node,
                                struct Node_Link* bestParent,
                                uint64_t cost,
                                uint64_t path);


# 155 "./dht/dhtcore/Node.h" 3 4
_Bool 
# 155 "./dht/dhtcore/Node.h"
    Node_isOneHopLink(struct Node_Link* link);
# 26 "./dht/dhtcore/RouterModule.h" 2
# 1 "./dht/dhtcore/NodeStore.h" 1
# 24 "./dht/dhtcore/NodeStore.h"
# 1 "./dht/dhtcore/RumorMill.h" 1
# 27 "./dht/dhtcore/RumorMill.h"
struct Linker_x07577641656290657;
# 36 "./dht/dhtcore/RumorMill.h"
struct RumorMill
{
    struct Address* addresses;

    int count;

    const char* name;
};

void RumorMill__addNode(struct RumorMill* mill, struct Address* addr, const char* file, int line);



# 48 "./dht/dhtcore/RumorMill.h" 3 4
_Bool 
# 48 "./dht/dhtcore/RumorMill.h"
    RumorMill_getNode(struct RumorMill* mill, struct Address* output);







struct RumorMill* RumorMill_new(struct Allocator* allocator,
                                struct Address* selfAddr,
                                int capacity,
                                struct Log* log,
                                const char* name);
# 25 "./dht/dhtcore/NodeStore.h" 2




# 1 "./util/events/EventBase.h" 1
# 20 "./util/events/EventBase.h"
struct Linker_x04355314467607556;

struct EventBase
{
    int unused;
};

struct EventBase* EventBase_new(struct Allocator* alloc);

int EventBase_eventCount(struct EventBase* eventBase);

void EventBase_beginLoop(struct EventBase* eventBase);

void EventBase_endLoop(struct EventBase* eventBase);
# 30 "./dht/dhtcore/NodeStore.h" 2
struct Linker_x05546301434157159;





typedef void (* NodeStore_OnBestPathChange)(void* userData, struct Node_Two* node);

struct NodeStore
{
    struct Address* selfAddress;

    struct Node_Two* selfNode;

    int pinnedNodes;
    int peerCount;
    int linkedNodes;

    int nodeCount;
    int nodeCapacity;

    int linkCount;
    int linkCapacity;

    NodeStore_OnBestPathChange onBestPathChange;
    void* onBestPathChangeCtx;
};
# 68 "./dht/dhtcore/NodeStore.h"
struct NodeStore* NodeStore_new(struct Address* myAddress,
                                struct Allocator* allocator,
                                struct EventBase* eventBase,
                                struct Log* logger,
                                struct RumorMill* renumberMill);
# 84 "./dht/dhtcore/NodeStore.h"
struct Node_Link* NodeStore_discoverNode(struct NodeStore* nodeStore,
                                         struct Address* addr,
                                         struct EncodingScheme* scheme,
                                         int inverseLinkEncodingFormNumber,
                                         uint64_t milliseconds);

struct Node_Two* NodeStore_nodeForAddr(struct NodeStore* nodeStore, uint8_t addr[16]);

struct Node_Two* NodeStore_closestNode(struct NodeStore* nodeStore, uint64_t path);

struct Node_Two* NodeStore_dumpTable(struct NodeStore* nodeStore, uint32_t index);

struct Node_Link* NodeStore_linkForPath(struct NodeStore* nodeStore, uint64_t path);

void NodeStore_unlinkNodes(struct NodeStore* nodeStore, struct Node_Link* link);
# 108 "./dht/dhtcore/NodeStore.h"
struct Node_Link* NodeStore_nextLink(struct Node_Two* parent, struct Node_Link* startLink);
# 123 "./dht/dhtcore/NodeStore.h"
struct Node_Link* NodeStore_firstHopInPath(struct NodeStore* nodeStore,
                                           uint64_t path,
                                           uint64_t* correctedPath,
                                           struct Node_Link* startingPoint);


uint64_t NodeStore_optimizePath(struct NodeStore* nodeStore, uint64_t path);
# 146 "./dht/dhtcore/NodeStore.h"
static inline 
# 146 "./dht/dhtcore/NodeStore.h" 3 4
             _Bool 
# 146 "./dht/dhtcore/NodeStore.h"
                  NodeStore_getRouteLabel_ERR(uint64_t x)
{
    return ((~((uint64_t)0))-3) <= x;
}
uint64_t NodeStore_getRouteLabel(struct NodeStore* nodeStore,
                                 uint64_t pathToParent,
                                 uint64_t pathParentToChild);





char* NodeStore_getRouteLabel_strerror(uint64_t returnVal);
# 172 "./dht/dhtcore/NodeStore.h"
struct Node_Two* NodeStore_getBest(struct NodeStore* nodeStore, uint8_t targetAddress[16]);
# 183 "./dht/dhtcore/NodeStore.h"
struct NodeList* NodeStore_getPeers(uint64_t label,
                                    const uint32_t max,
                                    struct Allocator* allocator,
                                    struct NodeStore* store);
# 199 "./dht/dhtcore/NodeStore.h"
struct NodeList* NodeStore_getClosestNodes(struct NodeStore* store,
                                           struct Address* targetAddress,
                                           const uint32_t count,
                                           uint32_t versionOfRequestingNode,
                                           struct Allocator* allocator);


void NodeStore_pathResponse(struct NodeStore* nodeStore, uint64_t path, uint64_t milliseconds);
void NodeStore_pathTimeout(struct NodeStore* nodeStore, uint64_t path);
# 217 "./dht/dhtcore/NodeStore.h"
void NodeStore_brokenLink(struct NodeStore* nodeStore, uint64_t path, uint64_t pathAtErrorPoint);

void NodeStore_disconnectedPeer(struct NodeStore* nodeStore, uint64_t path);

struct Node_Two* NodeStore_getNextNode(struct NodeStore* nodeStore, struct Node_Two* lastNode);
struct Node_Link* NodeStore_getNextLink(struct NodeStore* nodeStore, struct Node_Link* last);

uint64_t NodeStore_timeSinceLastPing(struct NodeStore* nodeStore, struct Node_Two* node);




struct Address NodeStore_addrForBucket(struct Address* source, uint16_t bucket);
uint16_t NodeStore_bucketForAddr(struct Address* source, struct Address* dest);
struct NodeList* NodeStore_getNodesForBucket(struct NodeStore* nodeStore,
                                             struct Allocator* allocator,
                                             uint16_t bucket,
                                             const uint32_t count);



# 237 "./dht/dhtcore/NodeStore.h" 3 4
_Bool 
# 237 "./dht/dhtcore/NodeStore.h"
    NodeStore_getFullVerify(struct NodeStore* nodeStore);
void NodeStore_setFullVerify(struct NodeStore* nodeStore, 
# 238 "./dht/dhtcore/NodeStore.h" 3 4
                                                         _Bool 
# 238 "./dht/dhtcore/NodeStore.h"
                                                              fullVerify);
# 27 "./dht/dhtcore/RouterModule.h" 2




struct Linker_x05425456814097922;
# 42 "./dht/dhtcore/RouterModule.h"
struct RouterModule;

struct RouterModule_Promise;
struct RouterModule_Promise
{
    void (* callback)(struct RouterModule_Promise* promise,
                      uint32_t lag,
                      struct Address* from,
                      Dict* result);
    void* userData;
    struct Allocator* alloc;
};
# 69 "./dht/dhtcore/RouterModule.h"
struct RouterModule* RouterModule_register(struct DHTModuleRegistry* registry,
                                           struct Allocator* allocator,
                                           const uint8_t myAddress[32],
                                           struct EventBase* eventBase,
                                           struct Log* logger,
                                           struct Random* rand,
                                           struct NodeStore* nodeStore);
# 84 "./dht/dhtcore/RouterModule.h"
uint64_t RouterModule_searchTimeoutMilliseconds(struct RouterModule* module);
# 97 "./dht/dhtcore/RouterModule.h"
struct RouterModule_Promise* RouterModule_pingNode(struct Address* addr,
                                                   uint32_t timeoutMilliseconds,
                                                   struct RouterModule* module,
                                                   struct Allocator* alloc);

struct RouterModule_Promise* RouterModule_newMessage(struct Address* addr,
                                                     uint32_t timeoutMilliseconds,
                                                     struct RouterModule* module,
                                                     struct Allocator* alloc);

void RouterModule_sendMessage(struct RouterModule_Promise* promise, Dict* request);



struct Node_Two* RouterModule_nodeForPath(uint64_t path, struct RouterModule* module);




uint32_t RouterModule_globalMeanResponseTime(struct RouterModule* module);

struct RouterModule_Promise* RouterModule_nextHop(struct Address* whoToAsk,
                                                  uint8_t target[16],
                                                  uint32_t timeoutMilliseconds,
                                                  struct RouterModule* module,
                                                  struct Allocator* alloc);

struct RouterModule_Promise* RouterModule_getPeers(struct Address* addr,
                                                   uint64_t nearbyLabel,
                                                   uint32_t timeoutMilliseconds,
                                                   struct RouterModule* module,
                                                   struct Allocator* alloc);

struct RouterModule_Promise* RouterModule_findNode(struct Address* whoToAsk,
                                                   uint8_t target[16],
                                                   uint32_t timeoutMilliseconds,
                                                   struct RouterModule* module,
                                                   struct Allocator* alloc);

void RouterModule_peerIsReachable(uint64_t pathToPeer,
                                  uint64_t lagMilliseconds,
                                  struct RouterModule* module);
# 25 "./dht/dhtcore/Janitor.h" 2
# 1 "./dht/dhtcore/SearchRunner.h" 1
# 27 "./dht/dhtcore/SearchRunner.h"
# 1 "./util/Bits.h" 1
# 21 "./util/Bits.h"
struct Linker_x04675666831769376;


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
# 28 "./dht/dhtcore/SearchRunner.h" 2


struct Linker_x045609885896676916;

struct SearchRunner_SearchData
{

    uint8_t target[16];


    struct Address lastNodeAsked;


    int totalRequests;


    int activeSearches;
};

struct SearchRunner
{
    int unused;
};
# 71 "./dht/dhtcore/SearchRunner.h"
struct RouterModule_Promise* SearchRunner_search(uint8_t target[16],
                                                 int maxRequests,
                                                 int maxRequestsIfFound,
                                                 struct SearchRunner* searchRunner,
                                                 struct Allocator* allocator);
# 86 "./dht/dhtcore/SearchRunner.h"
struct SearchRunner_SearchData* SearchRunner_showActiveSearch(struct SearchRunner* runner,
                                                              int number,
                                                              struct Allocator* alloc);

struct SearchRunner* SearchRunner_new(struct NodeStore* nodeStore,
                                      struct Log* logger,
                                      struct EventBase* base,
                                      struct RouterModule* module,
                                      uint8_t myAddress[16],
                                      struct RumorMill* rumorMill,
                                      struct Allocator* alloc);
# 26 "./dht/dhtcore/Janitor.h" 2






struct Linker_x03854203644560181;



struct Janitor
{




    struct RumorMill* externalMill;





    struct RumorMill* linkMill;





    struct RumorMill* nodeMill;


    struct RumorMill* dhtMill;


    struct RumorMill* splitMill;






    int64_t blacklistPathForMilliseconds;



    uint64_t localMaintainenceMilliseconds;






    uint64_t globalMaintainenceMilliseconds;
};

struct Janitor* Janitor_new(struct RouterModule* routerModule,
                            struct NodeStore* nodeStore,
                            struct SearchRunner* searchRunner,
                            struct RumorMill* rumorMill,
                            struct Log* logger,
                            struct Allocator* alloc,
                            struct EventBase* eventBase,
                            struct Random* rand);
# 19 "dht/dhtcore/Janitor.c" 2

# 1 "./dht/dhtcore/NodeList.h" 1
# 25 "./dht/dhtcore/NodeList.h"
struct NodeList
{

    uint32_t size;


    struct Node_Two** nodes;
};
# 21 "dht/dhtcore/Janitor.c" 2



# 1 "./dht/dhtcore/ReplySerializer.h" 1
# 23 "./dht/dhtcore/ReplySerializer.h"
struct Linker_x0912119481700308;




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
# 25 "dht/dhtcore/Janitor.c" 2


# 1 "./switch/LabelSplicer.h" 1
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
# 28 "dht/dhtcore/Janitor.c" 2
# 1 "./util/AddrTools.h" 1
# 19 "./util/AddrTools.h"
struct Linker_x007365410489165258;




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
# 29 "dht/dhtcore/Janitor.c" 2
# 1 "./util/AverageRoller.h" 1
# 23 "./util/AverageRoller.h"
struct Linker_x0706324036324899;

struct AverageRoller
{
    int unused;
};







struct AverageRoller* AverageRoller_new(const uint32_t windowSeconds,
                                        struct EventBase* eventBase,
                                        struct Allocator* allocator);







uint32_t AverageRoller_getAverage(struct AverageRoller* roller);
# 56 "./util/AverageRoller.h"
uint32_t AverageRoller_updateAtTime(struct AverageRoller* averageRoller,
                                    const uint64_t now,
                                    const uint32_t newEntry);
# 71 "./util/AverageRoller.h"
uint32_t AverageRoller_update(struct AverageRoller* roller, const uint32_t newEntry);
# 30 "dht/dhtcore/Janitor.c" 2


# 1 "./util/Hex.h" 1
# 21 "./util/Hex.h"
struct Linker_x0761722818062269;
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
# 33 "dht/dhtcore/Janitor.c" 2
# 1 "./util/events/Timeout.h" 1
# 20 "./util/events/Timeout.h"
struct Linker_x0041871076215445946;



struct Timeout;
# 36 "./util/events/Timeout.h"
struct Timeout* Timeout__setTimeout(void (* const callback)(void* callbackContext),
                                    void* const callbackContext,
                                    const uint64_t milliseconds,
                                    struct EventBase* eventBase,
                                    struct Allocator* allocator,
                                    char* file,
                                    int line);
# 57 "./util/events/Timeout.h"
struct Timeout* Timeout__setInterval(void (* const callback)(void* callbackContext),
                                     void* const callbackContext,
                                     const uint64_t milliseconds,
                                     struct EventBase* eventBase,
                                     struct Allocator* allocator,
                                     char* file,
                                     int line);
# 75 "./util/events/Timeout.h"
void Timeout_resetTimeout(struct Timeout* timeout,
                          const uint64_t milliseconds);







void Timeout_clearTimeout(struct Timeout* timeout);

void Timeout_clearAll(struct EventBase* eventBase);

int Timeout_isActive(struct Timeout* timeout);
# 34 "dht/dhtcore/Janitor.c" 2
# 1 "./util/events/Time.h" 1
# 20 "./util/events/Time.h"
struct Linker_x03969471585453477;




uint64_t Time_hrtime(void);



uint64_t Time_currentTimeMilliseconds(struct EventBase* eventBase);


uint64_t Time_currentTimeSeconds(struct EventBase* eventBase);
# 35 "dht/dhtcore/Janitor.c" 2
# 1 "./util/Defined.h" 1
# 36 "dht/dhtcore/Janitor.c" 2







struct Janitor_Blacklist
{
    int64_t timeAdded;
    uint64_t path;
};
# 56 "dht/dhtcore/Janitor.c"
struct Janitor_pvt
{
    struct Janitor pub;

    struct RouterModule* routerModule;

    struct NodeStore* nodeStore;

    struct SearchRunner* searchRunner;

    struct Timeout* timeout;

    struct Log* logger;


    struct Janitor_Blacklist blacklist[64];

    uint64_t timeOfNextGlobalMaintainence;

    struct Allocator* allocator;

    uint64_t timeOfNextSearchRepeat;
    uint64_t searchRepeatMilliseconds;

    struct EventBase* eventBase;
    struct Random* rand;


    int searches;

    unsigned long Identity_verifier;
};

struct Janitor_Search
{
    struct Janitor_pvt* janitor;

    struct Address best;

    uint8_t target[16];

    struct Allocator* alloc;

    unsigned long Identity_verifier;
};

static 
# 102 "dht/dhtcore/Janitor.c" 3 4
      _Bool 
# 102 "dht/dhtcore/Janitor.c"
           isBlacklisted(struct Janitor_pvt* j, uint64_t path)
{
    int64_t now = Time_currentTimeMilliseconds(j->eventBase);
    for (int i = 0; i < 64; i++) {
        struct Janitor_Blacklist* qp = &j->blacklist[i];
        if (qp->path == path && now - qp->timeAdded < j->pub.blacklistPathForMilliseconds) {
            return 
# 108 "dht/dhtcore/Janitor.c" 3 4
                  1
# 108 "dht/dhtcore/Janitor.c"
                      ;
        }
    }
    return 
# 111 "dht/dhtcore/Janitor.c" 3 4
          0
# 111 "dht/dhtcore/Janitor.c"
               ;
}

static void blacklist(struct Janitor_pvt* j, uint64_t path)
{
    int64_t now = Time_currentTimeMilliseconds(j->eventBase);
    int oldestIndex = 0;
    int64_t oldestTime = 
# 118 "dht/dhtcore/Janitor.c" 3 4
                        9223372036854775807LL
# 118 "dht/dhtcore/Janitor.c"
                                 ;
    for (int i = 0; i < 64; i++) {
        struct Janitor_Blacklist* qp = &j->blacklist[i];
        if (qp->path == path || now - qp->timeAdded > j->pub.blacklistPathForMilliseconds) {
            qp->timeAdded = now;
            qp->path = path;
            return;
        } else if (qp->timeAdded < oldestTime) {
            oldestIndex = i;
            oldestTime = qp->timeAdded;
        }
    }
    do { if (j->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(j->logger, Log_Level_DEBUG, "Janitor.c", 130, "Replacing [%lld]ms old blacklist node because blacklist is full", (long long)(now - oldestTime)); } } while (0)
                                      ;
    j->blacklist[oldestIndex].timeAdded = now;
    j->blacklist[oldestIndex].path = path;
}

static void responseCallback(struct RouterModule_Promise* promise,
                             uint32_t lagMilliseconds,
                             struct Address* from,
                             Dict* result)
{
    struct Janitor_Search* search = (__extension__ ({ __typeof__((struct Janitor_Search*)promise->userData) Identity_ptr = ((struct Janitor_Search*)promise->userData); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x677da4dcc8892296ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("Janitor.c"), (141), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x677da4dcc8892296ull))"); } } while (0); Identity_ptr; }));
    if (from) {
        blacklist(search->janitor, from->path);
        Bits__memcpy(&search->best,from,sizeof(struct Address),"Janitor.c",144);
        return;
    }

    search->janitor->searches--;

    if (!search->best.path) {
        do { if (search->janitor->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(search->janitor->logger, Log_Level_DEBUG, "Janitor.c", 151, "Search completed with no nodes found"); } } while (0);
    }
    Allocator__free((search->alloc),"Janitor.c",153);
}

static void search(uint8_t target[16], struct Janitor_pvt* janitor)
{
    if (janitor->searches >= 10) {
        do { if (janitor->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(janitor->logger, Log_Level_DEBUG, "Janitor.c", 159, "Skipping search because 20 are in progress"); } } while (0);
        return;
    }

        uint8_t targetStr[40];
        AddrTools_printIp(targetStr, target);
        do { if (janitor->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(janitor->logger, Log_Level_DEBUG, "Janitor.c", 165, "Beginning search for [%s]", targetStr); } } while (0);


    struct Allocator* searchAlloc = Allocator__child((janitor->allocator),"Janitor.c",168);
    struct RouterModule_Promise* rp =
        SearchRunner_search(target, -1, -1, janitor->searchRunner, searchAlloc);

    if (!rp) {
        do { if (janitor->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(janitor->logger, Log_Level_DEBUG, "Janitor.c", 173, "SearchRunner_search() returned NULL, probably full."); } } while (0);
        Allocator__free((searchAlloc),"Janitor.c",174);
        return;
    }

    janitor->searches++;

    struct Janitor_Search* search = Allocator__clone((rp->alloc),((&(struct Janitor_Search) { .janitor = janitor, .alloc = searchAlloc, })),sizeof(*((&(struct Janitor_Search) { .janitor = janitor, .alloc = searchAlloc, }))),"Janitor.c",180)


       ;
    (search)->Identity_verifier = ((unsigned long) 0x677da4dcc8892296ull);
    Bits__memcpy(search->target,target,16,"Janitor.c",185);

    rp->callback = responseCallback;
    rp->userData = search;
}

static void searchNoDupe(uint8_t target[16], struct Janitor_pvt* janitor)
{

    struct Allocator* seachListAlloc = Allocator__child((janitor->allocator),"Janitor.c",194);
    struct SearchRunner_SearchData* searchData;
    for (int i = 0; i < 30; i++) {
        searchData = SearchRunner_showActiveSearch(janitor->searchRunner,
                                                   i,
                                                   seachListAlloc);
        if (!searchData) { continue; }
        if (!__builtin_memcmp(searchData->target,target,16)) {

            Allocator__free((seachListAlloc),"Janitor.c",203);
            return;
        }
    }
    Allocator__free((seachListAlloc),"Janitor.c",207);


    search(target, janitor);

        uint8_t addrStr[40];
        AddrTools_printIp(addrStr, target);
        do { if (janitor->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(janitor->logger, Log_Level_DEBUG, "Janitor.c", 214, "No active search for [%s], starting one.", addrStr); } } while (0);

}

static void dhtResponseCallback(struct RouterModule_Promise* promise,
                                uint32_t lagMilliseconds,
                                struct Address* from,
                                Dict* result)
{
    struct Janitor_pvt* janitor = (__extension__ ({ __typeof__((struct Janitor_pvt*)promise->userData) Identity_ptr = ((struct Janitor_pvt*)promise->userData); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x677da4dcc8892296ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("Janitor.c"), (223), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x677da4dcc8892296ull))"); } } while (0); Identity_ptr; }));
    if (!from) { return; }
    blacklist(janitor, from->path);
    struct Address_List* addresses =
        ReplySerializer_parse(from, result, janitor->logger, 
# 227 "dht/dhtcore/Janitor.c" 3 4
                                                            1
# 227 "dht/dhtcore/Janitor.c"
                                                                , promise->alloc);

    struct Node_Two* parent = NodeStore_nodeForAddr(janitor->nodeStore, from->ip6.bytes);
    if (!parent) { return; }

    struct Address* selfAddr = janitor->nodeStore->selfAddress;
    for (int i = 0; addresses && i < addresses->length; i++) {
        if (addresses->elems[i].path == (~((uint64_t)0))) {

            continue;
        }
        if (Address_closest(selfAddr, from, &addresses->elems[i]) < 0) {


            continue;
        }

        struct Node_Link* link = NodeStore_linkForPath(janitor->nodeStore,
                                                       addresses->elems[i].path);
        if (link) {

            continue;
        }


        RumorMill__addNode((janitor->pub.dhtMill), (&addresses->elems[i]), "Janitor.c", 252);
    }
}

static void peersResponseCallback(struct RouterModule_Promise* promise,
                                  uint32_t lagMilliseconds,
                                  struct Address* from,
                                  Dict* result)
{
    struct Janitor_pvt* janitor = (__extension__ ({ __typeof__((struct Janitor_pvt*)promise->userData) Identity_ptr = ((struct Janitor_pvt*)promise->userData); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x677da4dcc8892296ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("Janitor.c"), (261), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x677da4dcc8892296ull))"); } } while (0); Identity_ptr; }));
    if (!from) { return; }
    blacklist(janitor, from->path);
    if (1) {
        String* addr = Address_toString(from, promise->alloc);
        do { if (janitor->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(janitor->logger, Log_Level_DEBUG, "Janitor.c", 266, "Got peers response from [%s]", addr->bytes); } } while (0);
    }
    struct Address_List* addresses =
        ReplySerializer_parse(from, result, janitor->logger, 
# 269 "dht/dhtcore/Janitor.c" 3 4
                                                            1
# 269 "dht/dhtcore/Janitor.c"
                                                                , promise->alloc);

    struct Node_Two* parent = NodeStore_nodeForAddr(janitor->nodeStore, from->ip6.bytes);
    if (!parent) { return; }

    int loopCount = 0;
    for (int i = 0; addresses && i < addresses->length; i++) {


        if (!__builtin_memcmp(addresses->elems[i].key,from->key,32)) { continue; }

        struct Node_Link* nl = NodeStore_linkForPath(janitor->nodeStore, addresses->elems[i].path);
        if (!nl || nl->linkCost == 
# 281 "dht/dhtcore/Janitor.c" 3 4
                                  0xffffffffU 
# 281 "dht/dhtcore/Janitor.c"
                                             || __builtin_memcmp(nl->child->address.ip6.bytes,addresses->elems[i].ip6.bytes,16)

                                                                                        )
        {
            struct Node_Two* node = NodeStore_nodeForAddr(janitor->nodeStore,
                                                          addresses->elems[i].ip6.bytes);
            if (node) {
                RumorMill__addNode((janitor->pub.linkMill), (&addresses->elems[i]), "Janitor.c", 288);
            } else {

                uint16_t bucketNodes = 0;
                uint16_t bucket = NodeStore_bucketForAddr(janitor->nodeStore->selfAddress,
                                                          &addresses->elems[i]);
                struct Allocator* nodeListAlloc = Allocator__child((janitor->allocator),"Janitor.c",294);
                struct NodeList* nodeList = NodeStore_getNodesForBucket(janitor->nodeStore,
                                                                        nodeListAlloc,
                                                                        bucket,
                                                                        4);
                for (uint32_t i = 0 ; i < nodeList->size ; i++) {
                    if (nodeList->nodes[i] == janitor->nodeStore->selfNode) { continue; }
                    if (nodeList->nodes[i]->address.path == 
# 301 "dht/dhtcore/Janitor.c" 3 4
                                                           0xffffffffffffffffULL
# 301 "dht/dhtcore/Janitor.c"
                                                                     ) { continue; }
                    bucketNodes++;
                }
                Allocator__free((nodeListAlloc),"Janitor.c",304);
                if (bucketNodes < 4) {

                    RumorMill__addNode((janitor->pub.nodeMill), (&addresses->elems[i]), "Janitor.c", 307);
                    continue;
                }
# 324 "dht/dhtcore/Janitor.c"
                struct Node_Link* link = 
# 324 "dht/dhtcore/Janitor.c" 3 4
                                        ((void *)0)
# 324 "dht/dhtcore/Janitor.c"
                                            ;
                while ((link = NodeStore_nextLink(parent, link))) {
                    if (Node_isOneHopLink(link)) { continue; }
                    uint64_t label = NodeStore_getRouteLabel(janitor->nodeStore,
                                                             from->path,
                                                             link->cannonicalLabel);
                    if (!LabelSplicer_routesThrough(label, addresses->elems[i].path)) { continue; }
                    RumorMill__addNode((janitor->pub.nodeMill), (&addresses->elems[i]), "Janitor.c", 331);
                }
            }
        } else if (!Address_isSameIp(&addresses->elems[i], &nl->child->address)) {
            if (nl->parent != parent) {

                    uint8_t newAddr[60];
                    Address_print(newAddr, from);
                    uint8_t labelStr[20];
                    AddrTools_printPath(labelStr, nl->cannonicalLabel);
                    do { if (janitor->logger && Log_Level_INFO >= Log_Level_DEBUG) { Log_print(janitor->logger, Log_Level_INFO, "Janitor.c", 341, "Apparently [%s] reported [%s] as it's peer", newAddr, labelStr); } } while (0)
                                               ;

                continue;
            }

                uint8_t newAddr[60];
                Address_print(newAddr, from);
                do { if (janitor->logger && Log_Level_INFO >= Log_Level_DEBUG) { Log_print(janitor->logger, Log_Level_INFO, "Janitor.c", 349, "Apparently [%s] has renumbered it's switch", newAddr); } } while (0);

            struct Node_Link* link = NodeStore_nextLink(parent, 
# 351 "dht/dhtcore/Janitor.c" 3 4
                                                               ((void *)0)
# 351 "dht/dhtcore/Janitor.c"
                                                                   );
            while (link) {
                struct Node_Link* nextLink = NodeStore_nextLink(parent, link);
                NodeStore_unlinkNodes(janitor->nodeStore, link);
                link = nextLink;

                i = 0;
                do { if (!((!loopCount))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("Janitor.c"), (358), "(!loopCount)"); } } while (0);
            }
            do { if (!((!NodeStore_nextLink(parent, 
# 360 "dht/dhtcore/Janitor.c" 3 4
           ((void *)0)
# 360 "dht/dhtcore/Janitor.c"
           )))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("Janitor.c"), (360), "(!NodeStore_nextLink(parent, ((void *)0)))"); } } while (0);
            loopCount++;
        }
    }
}







static void keyspaceMaintenance(struct Janitor_pvt* janitor)
{
    struct Address addr;
    struct Address* selfAddr = janitor->nodeStore->selfAddress;
    if (!RumorMill_getNode(janitor->pub.dhtMill, &addr)) {

        for (uint16_t bucket = 0; bucket < 128 ; bucket++) {

            struct Allocator* nodeListAlloc = Allocator__child((janitor->allocator),"Janitor.c",380);
            struct NodeList* nodeList = NodeStore_getNodesForBucket(janitor->nodeStore,
                                                                    nodeListAlloc,
                                                                    bucket,
                                                                    4);
            for (uint32_t i = 0 ; i < nodeList->size ; i++) {
                if (nodeList->nodes[i] == janitor->nodeStore->selfNode) { continue; }
                if (nodeList->nodes[i]->address.path == 
# 387 "dht/dhtcore/Janitor.c" 3 4
                                                       0xffffffffffffffffULL
# 387 "dht/dhtcore/Janitor.c"
                                                                 ) { continue; }


                RumorMill__addNode((janitor->pub.dhtMill), (&nodeList->nodes[i]->address), "Janitor.c", 390);
            }
            Allocator__free((nodeListAlloc),"Janitor.c",392);
        }
        return;
    }

    struct Node_Two* node = NodeStore_nodeForAddr(janitor->nodeStore, addr.ip6.bytes);
    if (node && node->address.path == addr.path) {

        uint16_t bucket = NodeStore_bucketForAddr(selfAddr, &addr);
        struct Address target = NodeStore_addrForBucket(&addr, bucket);
        struct RouterModule_Promise* rp = RouterModule_findNode(&addr,
                                                                target.ip6.bytes,
                                                                0,
                                                                janitor->routerModule,
                                                                janitor->allocator);
        rp->callback = dhtResponseCallback;
        rp->userData = janitor;

            uint8_t addrStr[60];
            Address_print(addrStr, &addr);
            do { if (janitor->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(janitor->logger, Log_Level_DEBUG, "Janitor.c", 412, "Sending findNode to [%s] from " "dht-checking RumorMill", addrStr); } } while (0)
                                                                         ;

    } else {


        RouterModule_pingNode(&addr, 0, janitor->routerModule, janitor->allocator);

            uint8_t addrStr[60];
            Address_print(addrStr, &addr);
            do { if (janitor->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(janitor->logger, Log_Level_DEBUG, "Janitor.c", 422, "Pinging possible node [%s] from " "dht-checking RumorMill", addrStr); } } while (0)
                                                                         ;

    }
    return;
    searchNoDupe(addr.ip6.bytes, janitor);
}

static struct Node_Two* getRandomNode(struct Random* rand, struct NodeStore* store)
{
    uint32_t index = Random_uint32(rand) % (store->nodeCount);
    struct Node_Two* node = 
# 433 "dht/dhtcore/Janitor.c" 3 4
                           ((void *)0)
# 433 "dht/dhtcore/Janitor.c"
                               ;
    do {
        node = NodeStore_getNextNode(store, node);
    } while (index--);

    do { if (!((node))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("Janitor.c"), (438), "(node)"); } } while (0);
    return node;
}

static void getPeersMill(struct Janitor_pvt* janitor, struct Address* addr)
{





    uint64_t targetLabel = Random_uint32(janitor->rand);
    struct Node_Link* nl = NodeStore_linkForPath(janitor->nodeStore, addr->path);
    if (nl) {
        addr = &nl->child->address;
        struct Node_Link* link = 
# 453 "dht/dhtcore/Janitor.c" 3 4
                                ((void *)0)
# 453 "dht/dhtcore/Janitor.c"
                                    ;
        while ((link = NodeStore_nextLink(nl->child, link))) {
            if (!Node_isOneHopLink(link) && link == Node_getBestParent(link->child)) {
                targetLabel = nl->cannonicalLabel;
                break;
            }
        }
    }

    struct RouterModule_Promise* rp =
        RouterModule_getPeers(addr,
                              targetLabel,
                              0,
                              janitor->routerModule,
                              janitor->allocator);
    rp->callback = peersResponseCallback;
    rp->userData = janitor;
}
# 481 "dht/dhtcore/Janitor.c"
static 
# 481 "dht/dhtcore/Janitor.c" 3 4
      _Bool 
# 481 "dht/dhtcore/Janitor.c"
           tryExistingNode(struct Janitor_pvt* janitor)
{
    struct Node_Two* worst = 
# 483 "dht/dhtcore/Janitor.c" 3 4
                            ((void *)0)
# 483 "dht/dhtcore/Janitor.c"
                                ;
    uint64_t worstTime = 0;
    struct Node_Two* node = NodeStore_getNextNode(janitor->nodeStore, 
# 485 "dht/dhtcore/Janitor.c" 3 4
                                                                     ((void *)0)
# 485 "dht/dhtcore/Janitor.c"
                                                                         );
    while (node) {
        uint64_t nodeTime = NodeStore_timeSinceLastPing(janitor->nodeStore, node);
        if (node == janitor->nodeStore->selfNode) {

        } else if (node->address.path != 
# 490 "dht/dhtcore/Janitor.c" 3 4
                                        0xffffffffffffffffULL 
# 490 "dht/dhtcore/Janitor.c"
                                                   &&
                  (!worst || nodeTime > worstTime))
        {
            worst = node;
            worstTime = nodeTime;
        }
        node = NodeStore_getNextNode(janitor->nodeStore, node);
    }
    if (worst) {
        getPeersMill(janitor, &worst->address);
        if (1) { uint8_t addrStr[60]; Address_print(addrStr, (&worst->address)); do { if ((janitor)->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print((janitor)->logger, Log_Level_DEBUG, "Janitor.c", 500, "%s [%s]", ("Pinging existing node"), addrStr); } } while (0); } do { } while (0);
        return 
# 501 "dht/dhtcore/Janitor.c" 3 4
              1
# 501 "dht/dhtcore/Janitor.c"
                  ;
    }
    return 
# 503 "dht/dhtcore/Janitor.c" 3 4
          0
# 503 "dht/dhtcore/Janitor.c"
               ;
}



static 
# 508 "dht/dhtcore/Janitor.c" 3 4
      _Bool 
# 508 "dht/dhtcore/Janitor.c"
           tryMill(struct Janitor_pvt* janitor, struct RumorMill* mill, int rules)
{
    struct Address addr = { .protocolVersion = 0 };
    while (RumorMill_getNode(mill, &addr)) {
        if (rules & (1<<0)) {
            if (isBlacklisted(janitor, addr.path)) {
                if (1) { uint8_t addrStr[60]; Address_print(addrStr, (&addr)); do { if ((janitor)->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print((janitor)->logger, Log_Level_DEBUG, "Janitor.c", 514, "%s [%s]", ("Not pinging blacklisted node"), addrStr); } } while (0); } do { } while (0);
                continue;
            }
        }
        if (rules & (1<<1)) {
            if (!NodeStore_nodeForAddr(janitor->nodeStore, addr.ip6.bytes)) {
                if (1) { uint8_t addrStr[60]; Address_print(addrStr, (&addr)); do { if ((janitor)->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print((janitor)->logger, Log_Level_DEBUG, "Janitor.c", 520, "%s [%s]", ("Not pinging node not in nodeStore"), addrStr); } } while (0); } do { } while (0);
                continue;
            }
        }
        getPeersMill(janitor, &addr);
        if (1) {
            uint8_t addrStr[60];
            Address_print(addrStr, &addr);
            do { if (janitor->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(janitor->logger, Log_Level_DEBUG, "Janitor.c", 528, "Pinging possible node [%s] from RumorMill [%s]", addrStr, mill->name); } } while (0)
                                          ;
        }
        return 
# 531 "dht/dhtcore/Janitor.c" 3 4
              1
# 531 "dht/dhtcore/Janitor.c"
                  ;
    }
    return 
# 533 "dht/dhtcore/Janitor.c" 3 4
          0
# 533 "dht/dhtcore/Janitor.c"
               ;
}


static 
# 537 "dht/dhtcore/Janitor.c" 3 4
      _Bool 
# 537 "dht/dhtcore/Janitor.c"
           splitLinks(struct Janitor_pvt* janitor)
{
    struct Node_Link* link = 
# 539 "dht/dhtcore/Janitor.c" 3 4
                            ((void *)0)
# 539 "dht/dhtcore/Janitor.c"
                                ;
    if (!janitor->pub.splitMill->count) {
        while ((link = NodeStore_getNextLink(janitor->nodeStore, link))) {
            if (link != Node_getBestParent(link->child)) { continue; }
            if (Node_isOneHopLink(link)) { continue; }
            if (link->child == janitor->nodeStore->selfNode) { continue; }
            RumorMill__addNode((janitor->pub.splitMill), (&link->parent->address), "Janitor.c", 545);
        }
    }
    return tryMill(janitor, janitor->pub.splitMill, (1<<0));
}

static 
# 551 "dht/dhtcore/Janitor.c" 3 4
      _Bool 
# 551 "dht/dhtcore/Janitor.c"
           tryRandomLink(struct Janitor_pvt* janitor)
{

    struct Node_Two* node = getRandomNode(janitor->rand, janitor->nodeStore);

    struct Node_Link* link = NodeStore_nextLink(node, 
# 556 "dht/dhtcore/Janitor.c" 3 4
                                                     ((void *)0)
# 556 "dht/dhtcore/Janitor.c"
                                                         );
    uint32_t linkCount = 0;
    while (link) {
        linkCount++;
        link = NodeStore_nextLink(node, link);
    }
    if (linkCount) {

        uint32_t randLinkIndex = Random_uint32(janitor->rand) % linkCount;
        link = NodeStore_nextLink(node, 
# 565 "dht/dhtcore/Janitor.c" 3 4
                                       ((void *)0)
# 565 "dht/dhtcore/Janitor.c"
                                           );
        linkCount = 0;
        while (linkCount < randLinkIndex) {
            linkCount++;
            link = NodeStore_nextLink(node, link);
        }
    }

    if (link && link->parent != link->child) {
        struct Address addr = link->child->address;
        uint64_t path = NodeStore_getRouteLabel(janitor->nodeStore,
                                                link->parent->address.path,
                                                link->cannonicalLabel);
        if (path != ((~((uint64_t)0))-1) &&
            path != ((~((uint64_t)0))-2))
        {
            addr.path = path;
        }
        if (addr.path < 
# 583 "dht/dhtcore/Janitor.c" 3 4
                       0xffffffffffffffffULL
# 583 "dht/dhtcore/Janitor.c"
                                 ) {
            getPeersMill(janitor, &addr);

                uint8_t addrStr[60];
                Address_print(addrStr, &addr);
                do { if (janitor->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(janitor->logger, Log_Level_DEBUG, "Janitor.c", 588, "Pinging random node link [%s] for maintenance.", addrStr); } } while (0)
                                                                                           ;

            return 
# 591 "dht/dhtcore/Janitor.c" 3 4
                  1
# 591 "dht/dhtcore/Janitor.c"
                      ;
        }
    }
    return 
# 594 "dht/dhtcore/Janitor.c" 3 4
          0
# 594 "dht/dhtcore/Janitor.c"
               ;
}

static void maintanenceCycle(void* vcontext)
{
    struct Janitor_pvt* const janitor = (__extension__ ({ __typeof__((struct Janitor_pvt*) vcontext) Identity_ptr = ((struct Janitor_pvt*) vcontext); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x677da4dcc8892296ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("Janitor.c"), (599), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x677da4dcc8892296ull))"); } } while (0); Identity_ptr; }));

    uint64_t now = Time_currentTimeMilliseconds(janitor->eventBase);

    uint64_t nextTimeout = (janitor->pub.localMaintainenceMilliseconds / 2);
    nextTimeout += Random_uint32(janitor->rand) % (nextTimeout * 2);
    Timeout_resetTimeout(janitor->timeout, nextTimeout);

    if (janitor->nodeStore->nodeCount == 0 && janitor->pub.externalMill->count == 0) {
        if (now > janitor->timeOfNextGlobalMaintainence) {
            do { if (janitor->logger && Log_Level_WARN >= Log_Level_DEBUG) { Log_print(janitor->logger, Log_Level_WARN, "Janitor.c", 609, "No nodes in routing table, check network connection and configuration."); } } while (0)
                                                                                              ;
            janitor->timeOfNextGlobalMaintainence += janitor->pub.globalMaintainenceMilliseconds;
        }
        return;
    }

    struct Address addr = { .protocolVersion = 0 };

    if (tryMill(janitor, janitor->pub.externalMill, (1<<0))) {


    } else if (tryMill(janitor,
                       janitor->pub.linkMill,
                       (1<<0) | (1<<1)))
    {


    } else if (tryMill(janitor, janitor->pub.nodeMill, (1<<0))) {


    } else if (splitLinks(janitor)) {


    } else if (tryRandomLink(janitor)) {


    } else {

    }


    tryExistingNode(janitor);


    keyspaceMaintenance(janitor);


    Random_bytes(janitor->rand, addr.ip6.bytes, 16);

    AddressCalc_makeValidAddress(addr.ip6.bytes);

    struct Node_Two* n = NodeStore_getBest(janitor->nodeStore, addr.ip6.bytes);


    if (n == 
# 654 "dht/dhtcore/Janitor.c" 3 4
            ((void *)0) 
# 654 "dht/dhtcore/Janitor.c"
                 || Node_getCost(n) == 
# 654 "dht/dhtcore/Janitor.c" 3 4
                                       0xffffffffffffffffULL
# 654 "dht/dhtcore/Janitor.c"
                                                 ) {




    }
# 668 "dht/dhtcore/Janitor.c"
    if (now > janitor->timeOfNextGlobalMaintainence) {

        janitor->timeOfNextGlobalMaintainence += janitor->pub.globalMaintainenceMilliseconds;
    }
}

struct Janitor* Janitor_new(struct RouterModule* routerModule,
                            struct NodeStore* nodeStore,
                            struct SearchRunner* searchRunner,
                            struct RumorMill* rumorMill,
                            struct Log* logger,
                            struct Allocator* allocator,
                            struct EventBase* eventBase,
                            struct Random* rand)
{
    struct Allocator* alloc = Allocator__child((allocator),"Janitor.c",683);
    struct Janitor_pvt* janitor = Allocator__clone((alloc),((&(struct Janitor_pvt) { .eventBase = eventBase, .routerModule = routerModule, .nodeStore = nodeStore, .searchRunner = searchRunner, .logger = logger, .allocator = alloc, .rand = rand })),sizeof(*((&(struct Janitor_pvt) { .eventBase = eventBase, .routerModule = routerModule, .nodeStore = nodeStore, .searchRunner = searchRunner, .logger = logger, .allocator = alloc, .rand = rand }))),"Janitor.c",684)







       ;
    (janitor)->Identity_verifier = ((unsigned long) 0x677da4dcc8892296ull);

    janitor->pub.externalMill = rumorMill;
    janitor->pub.linkMill = RumorMill_new(alloc, nodeStore->selfAddress, 64, logger, "linkMill");
    janitor->pub.nodeMill = RumorMill_new(alloc, nodeStore->selfAddress, 64, logger, "nodeMill");
    janitor->pub.dhtMill = RumorMill_new(alloc,
                                         nodeStore->selfAddress,
                                         (128 * 4),
                                         logger,
                                         "dhtMill");
    janitor->pub.splitMill = RumorMill_new(alloc, nodeStore->selfAddress, 16, logger, "splitMill");

    janitor->pub.globalMaintainenceMilliseconds = 30000;
    janitor->pub.localMaintainenceMilliseconds = 1000;
    janitor->pub.blacklistPathForMilliseconds = 30000;

    janitor->timeOfNextGlobalMaintainence = Time_currentTimeMilliseconds(eventBase);

    janitor->timeout = Timeout__setTimeout((maintanenceCycle), (janitor), (janitor->pub.localMaintainenceMilliseconds), (eventBase), (alloc), "Janitor.c", 711)



                                                ;

    return &janitor->pub;
}
