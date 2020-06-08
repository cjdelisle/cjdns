# 1 "dht/dhtcore/RouterModule.c"
# 1 "/home/jojapoppa/cjdns//"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "dht/dhtcore/RouterModule.c"
# 15 "dht/dhtcore/RouterModule.c"
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
struct Linker_x07677765672717598;
# 33 "./util/Assert.h"
__attribute__((__format__ (__printf__, 1, 2)))
__attribute__((__noreturn__))
void Assert_failure(const char* format, ...);
# 19 "./util/Identity.h" 2
# 1 "./util/Constant.h" 1
# 33 "./util/Constant.h"

# 20 "./util/Identity.h" 2



    
# 19 "./memory/Allocator.h" 2


struct Linker_x004812193484185401;





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
struct Linker_x006469788336409898;

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

struct Linker_x08732352313716223;

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
# 16 "dht/dhtcore/RouterModule.c" 2
# 1 "./dht/Address.h" 1
# 21 "./dht/Address.h"
struct Linker_x08859057366265708;
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
# 17 "dht/dhtcore/RouterModule.c" 2
# 1 "./dht/dhtcore/RouterModule.h" 1
# 22 "./dht/dhtcore/RouterModule.h"
# 1 "./crypto/random/Random.h" 1
# 19 "./crypto/random/Random.h"
# 1 "./exception/Except.h" 1
# 18 "./exception/Except.h"
# 1 "./exception/Er.h" 1
# 23 "./exception/Er.h"
struct Linker_x028229763899983684;

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


struct Linker_x08335443095395665;



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
struct Linker_x07877561763168128;

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
struct Linker_x04554823047908667;



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

struct Linker_x08113046474433032;



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
# 23 "./dht/dhtcore/RouterModule.h" 2

# 1 "./dht/DHTModuleRegistry.h" 1
# 22 "./dht/DHTModuleRegistry.h"
# 1 "./benc/Dict.h" 1
# 21 "./benc/Dict.h"
struct Linker_x08983218136478832;

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


struct Linker_x0510188869013086;



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
struct Linker_x06926870441528243;

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


struct Linker_x04779750923596686;



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



struct Linker_x05507377645835192;

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
struct Linker_x017724706055841355;
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
struct Linker_x04542776376666193;

struct EventBase
{
    int unused;
};

struct EventBase* EventBase_new(struct Allocator* alloc);

int EventBase_eventCount(struct EventBase* eventBase);

void EventBase_beginLoop(struct EventBase* eventBase);

void EventBase_endLoop(struct EventBase* eventBase);
# 30 "./dht/dhtcore/NodeStore.h" 2
struct Linker_x004341078121602249;





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




struct Linker_x03265415138939267;
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
# 18 "dht/dhtcore/RouterModule.c" 2
# 1 "./dht/dhtcore/RouterModule_pvt.h" 1
# 22 "./dht/dhtcore/RouterModule_pvt.h"
struct RouterModule_Ping;


struct RouterModule
{

    struct Address address;


    struct AverageRoller* gmrtRoller;


    struct NodeStore* nodeStore;


    uint64_t totalReach;


    struct DHTModuleRegistry* registry;


    struct EventBase* eventBase;

    struct Log* logger;

    struct Allocator* allocator;

    struct Pinger* pinger;

    struct Admin* admin;

    struct Random* rand;

    uint32_t pingsInFlight;






    struct DHTMessage* currentMessage;

    unsigned long Identity_verifier;
};
# 19 "dht/dhtcore/RouterModule.c" 2

# 1 "./dht/dhtcore/NodeList.h" 1
# 25 "./dht/dhtcore/NodeList.h"
struct NodeList
{

    uint32_t size;


    struct Node_Two** nodes;
};
# 21 "dht/dhtcore/RouterModule.c" 2

# 1 "./dht/dhtcore/VersionList.h" 1
# 21 "./dht/dhtcore/VersionList.h"
struct Linker_x03179769555284513;



struct VersionList
{
    uint32_t length;
    struct Allocator* alloc;
    uint32_t* versions;
};

struct VersionList* VersionList_parse(String* str, struct Allocator* alloc);

String* VersionList_stringify(struct VersionList* list, struct Allocator* alloc);

struct VersionList* VersionList_new(uint32_t count, struct Allocator* alloc);
# 23 "dht/dhtcore/RouterModule.c" 2
# 1 "./dht/CJDHTConstants.h" 1
# 19 "./dht/CJDHTConstants.h"
# 1 "./util/version/Version.h" 1
# 19 "./util/version/Version.h"
struct Linker_x08038278878532894;
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
# 24 "dht/dhtcore/RouterModule.c" 2

# 1 "./dht/DHTModule.h" 1
# 29 "./dht/DHTModule.h"
struct DHTModule;
struct DHTModule {




    const char* const name;




    void* context;
# 49 "./dht/DHTModule.h"
    int (* handleIncoming)(struct DHTMessage* message, void* context);







    int (* handleOutgoing)(struct DHTMessage* message, void* context);
};
# 26 "dht/dhtcore/RouterModule.c" 2



# 1 "./switch/LabelSplicer.h" 1
# 18 "./switch/LabelSplicer.h"
# 1 "./util/Bits.h" 1
# 21 "./util/Bits.h"
struct Linker_x08935684592658264;


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
# 30 "dht/dhtcore/RouterModule.c" 2
# 1 "./switch/NumberCompress.h" 1
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
# 31 "dht/dhtcore/RouterModule.c" 2

# 1 "./util/AverageRoller.h" 1
# 23 "./util/AverageRoller.h"
struct Linker_x09554393461837878;

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
# 33 "dht/dhtcore/RouterModule.c" 2

# 1 "./util/Hex.h" 1
# 21 "./util/Hex.h"
struct Linker_x06598016069307158;
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
# 35 "dht/dhtcore/RouterModule.c" 2
# 1 "./util/Endian.h" 1
# 36 "dht/dhtcore/RouterModule.c" 2
# 1 "./util/Pinger.h" 1
# 24 "./util/Pinger.h"
struct Linker_x05648730816390666;
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
# 37 "dht/dhtcore/RouterModule.c" 2
# 1 "./util/events/Time.h" 1
# 20 "./util/events/Time.h"
struct Linker_x039236776678114826;




uint64_t Time_hrtime(void);



uint64_t Time_currentTimeMilliseconds(struct EventBase* eventBase);


uint64_t Time_currentTimeSeconds(struct EventBase* eventBase);
# 38 "dht/dhtcore/RouterModule.c" 2
# 1 "./util/events/Timeout.h" 1
# 20 "./util/events/Timeout.h"
struct Linker_x023241965098359496;



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
# 39 "dht/dhtcore/RouterModule.c" 2

# 1 "./wire/Message.h" 1
# 23 "./wire/Message.h"
struct Linker_x06476288783605673;



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
# 41 "dht/dhtcore/RouterModule.c" 2
# 189 "dht/dhtcore/RouterModule.c"
static int handleIncoming(struct DHTMessage* message, void* vcontext);
static int handleOutgoing(struct DHTMessage* message, void* vcontext);
# 204 "dht/dhtcore/RouterModule.c"
struct RouterModule* RouterModule_register(struct DHTModuleRegistry* registry,
                                           struct Allocator* allocator,
                                           const uint8_t myAddress[32],
                                           struct EventBase* eventBase,
                                           struct Log* logger,
                                           struct Random* rand,
                                           struct NodeStore* nodeStore)
{
    struct RouterModule* const out = Allocator__calloc((allocator),(sizeof(struct RouterModule)),(1),"RouterModule.c",212);

    struct DHTModule* dm = Allocator__clone((allocator),((&(struct DHTModule) { .name = "RouterModule", .context = out, .handleIncoming = handleIncoming, .handleOutgoing = handleOutgoing })),sizeof(*((&(struct DHTModule) { .name = "RouterModule", .context = out, .handleIncoming = handleIncoming, .handleOutgoing = handleOutgoing }))),"RouterModule.c",214)




       ;
    DHTModuleRegistry_register(dm, registry);

    Address_forKey(&out->address, myAddress);

    out->gmrtRoller = AverageRoller_new(256, eventBase, allocator);
    AverageRoller_update(out->gmrtRoller, 5000);
    out->nodeStore = nodeStore;
    out->registry = registry;
    out->eventBase = eventBase;
    out->logger = logger;
    out->allocator = allocator;
    out->rand = rand;
    out->pinger = Pinger_new(eventBase, rand, logger, allocator);

    (out)->Identity_verifier = ((unsigned long) 0x78a2cb843c088bb5ull);
    return out;
}
# 245 "dht/dhtcore/RouterModule.c"
uint64_t RouterModule_searchTimeoutMilliseconds(struct RouterModule* module)
{
    uint64_t x = (((uint64_t) AverageRoller_getAverage(module->gmrtRoller)) * 4);
    x = x + (Random_uint32(module->rand) % (x | 1)) / 2;
    return (x > 10000) ? 10000 : (x < 10) ? 10 : x;
}

static inline int sendNodes(struct NodeList* nodeList,
                            struct DHTMessage* message,
                            struct RouterModule* module,
                            uint32_t askerVersion)
{
    struct DHTMessage* query = message->replyTo;
    String* nodes =
        String_newBinary(
# 259 "dht/dhtcore/RouterModule.c" 3 4
                        ((void *)0)
# 259 "dht/dhtcore/RouterModule.c"
                            , nodeList->size * 40, message->allocator);

    struct VersionList* versions = VersionList_new(nodeList->size, message->allocator);

    int i = 0;
    for (; i < (int)nodeList->size; i++) {




        struct Address addr;
        Bits__memcpy(&addr,&nodeList->nodes[i]->address,sizeof(struct Address),"RouterModule.c",270);

        addr.path = NumberCompress_getLabelFor(addr.path, query->address->path);

        Address_serialize(&nodes->bytes[i * 40], &addr);

        versions->versions[i] = nodeList->nodes[i]->address.protocolVersion;

        do { if (!((!Bits_isZero(&nodes->bytes[i * 40], 40)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("RouterModule.c"), (278), "(!Bits_isZero(&nodes->bytes[i * 40], 40))"); } } while (0)
                                                                ;
    }
    nodes->len = i * 40;
    versions->length = i;
    if (i > 0) {
        Dict_putString(message->asDict, CJDHTConstants_NODES, nodes, message->allocator);
        String* versionsStr = VersionList_stringify(versions, message->allocator);
        Dict_putString(message->asDict,
                       CJDHTConstants_NODE_PROTOCOLS,
                       versionsStr,
                       message->allocator);
    }
    return 0;
}
# 304 "dht/dhtcore/RouterModule.c"
static inline int handleQuery(struct DHTMessage* message,
                              struct RouterModule* module)
{
    struct DHTMessage* query = message->replyTo;

    int64_t* versionPtr = Dict_getInt(query->asDict, CJDHTConstants_PROTOCOL);
    uint32_t version = (versionPtr && *versionPtr <= 
# 310 "dht/dhtcore/RouterModule.c" 3 4
                                                    0xffffffffU
# 310 "dht/dhtcore/RouterModule.c"
                                                              ) ? *versionPtr : 0;

    struct NodeList* nodeList = 
# 312 "dht/dhtcore/RouterModule.c" 3 4
                               ((void *)0)
# 312 "dht/dhtcore/RouterModule.c"
                                   ;

    String* queryType = Dict_getString(query->asDict, CJDHTConstants_QUERY);
    if (String_equals(queryType, CJDHTConstants_QUERY_FN)) {
        do { if (module->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(module->logger, Log_Level_DEBUG, "RouterModule.c", 316, "FindNode Query"); } } while (0);

        String* target = Dict_getString(query->asDict, CJDHTConstants_TARGET);
        if (target == 
# 319 "dht/dhtcore/RouterModule.c" 3 4
                     ((void *)0) 
# 319 "dht/dhtcore/RouterModule.c"
                          || target->len != 16) {
            return 0;
        }

        struct Address targetAddr = { .path = 0 };
        Bits__memcpy(targetAddr.ip6.bytes,target->bytes,16,"RouterModule.c",324);


        nodeList = NodeStore_getClosestNodes(module->nodeStore,
                                             &targetAddr,
                                             8,
                                             version,
                                             message->allocator);

    } else if (String_equals(queryType, CJDHTConstants_QUERY_GP)) {
        do { if (module->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(module->logger, Log_Level_DEBUG, "RouterModule.c", 334, "GetPeers Query"); } } while (0);

        String* target = Dict_getString(query->asDict, CJDHTConstants_TARGET);
        if (target == 
# 337 "dht/dhtcore/RouterModule.c" 3 4
                     ((void *)0) 
# 337 "dht/dhtcore/RouterModule.c"
                          || target->len != 8) {
            return 0;
        }
        uint64_t targetPath;
        Bits__memcpy(&targetPath,target->bytes,8,"RouterModule.c",341);
        targetPath = ((uint64_t)__builtin_bswap64(((uint64_t)(targetPath))));

        nodeList =
            NodeStore_getPeers(targetPath, 8, message->allocator, module->nodeStore);

    } else if (String_equals(queryType, CJDHTConstants_QUERY_NH)) {
        do { if (module->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(module->logger, Log_Level_DEBUG, "RouterModule.c", 348, "HN Query"); } } while (0);

        String* target = Dict_getString(query->asDict, CJDHTConstants_TARGET);
        if (target == 
# 351 "dht/dhtcore/RouterModule.c" 3 4
                     ((void *)0) 
# 351 "dht/dhtcore/RouterModule.c"
                          || target->len != 16) {
            return 0;
        }
        struct Node_Two* nn = NodeStore_getBest(module->nodeStore, target->bytes);
        nodeList = Allocator__calloc((message->allocator),(sizeof(struct NodeList)),(1),"RouterModule.c",355);
        if (nn) {
            nodeList->size = 1;
            nodeList->nodes = Allocator__calloc((message->allocator),(sizeof(char*)),(1),"RouterModule.c",358);
            nodeList->nodes[0] = nn;
        }
    }

    return (nodeList) ? sendNodes(nodeList, message, module, version) : 0;
}
# 373 "dht/dhtcore/RouterModule.c"
static int handleOutgoing(struct DHTMessage* message, void* vcontext)
{
    struct RouterModule* module = (__extension__ ({ __typeof__((struct RouterModule*) vcontext) Identity_ptr = ((struct RouterModule*) vcontext); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x78a2cb843c088bb5ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("RouterModule.c"), (375), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x78a2cb843c088bb5ull))"); } } while (0); Identity_ptr; }));

    Dict_putInt(message->asDict,
                CJDHTConstants_PROTOCOL,
                20,
                message->allocator);

    if (message->replyTo != 
# 382 "dht/dhtcore/RouterModule.c" 3 4
                           ((void *)0)
# 382 "dht/dhtcore/RouterModule.c"
                               ) {
        return handleQuery(message, module);
    }

    return 0;
}

struct PingContext
{
    struct RouterModule_Promise pub;


    struct SearchContext* search;

    struct RouterModule* router;

    struct Address address;


    struct Pinger_Ping* pp;


    Dict* messageDict;

    unsigned long Identity_verifier;
};

static void sendMsg(String* txid, void* vpingContext)
{
    struct PingContext* pc = (__extension__ ({ __typeof__((struct PingContext*) vpingContext) Identity_ptr = ((struct PingContext*) vpingContext); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x78a2cb843c088bb5ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("RouterModule.c"), (411), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x78a2cb843c088bb5ull))"); } } while (0); Identity_ptr; }));


    Dict_putString(pc->messageDict, CJDHTConstants_TXID, txid, pc->pp->pingAlloc);

    struct Allocator* temp = Allocator__child((pc->pp->pingAlloc),"RouterModule.c",416);
    struct Message* msg = Message_new(0, 1536 + 512, temp);
    struct DHTMessage* dmesg = Allocator__calloc((temp),(sizeof(struct DHTMessage)),(1),"RouterModule.c",418);
    dmesg->binMessage = msg;
    dmesg->address = &pc->address;
    dmesg->asDict = pc->messageDict;
    dmesg->allocator = temp;

    DHTModuleRegistry_handleOutgoing(dmesg, pc->router->registry);
}

static void onTimeout(uint32_t milliseconds, struct PingContext* pctx)
{
    struct Node_Two* n = NodeStore_closestNode(pctx->router->nodeStore, pctx->address.path);


    if (n && !__builtin_memcmp(pctx->address.key,n->address.key,32)) {
        NodeStore_pathTimeout(pctx->router->nodeStore, pctx->address.path);
    }

    if (pctx->pub.callback) {
        pctx->pub.callback(&pctx->pub, milliseconds, 
# 437 "dht/dhtcore/RouterModule.c" 3 4
                                                    ((void *)0)
# 437 "dht/dhtcore/RouterModule.c"
                                                        , 
# 437 "dht/dhtcore/RouterModule.c" 3 4
                                                          ((void *)0)
# 437 "dht/dhtcore/RouterModule.c"
                                                              );
    }
}

static uint64_t pingTimeoutMilliseconds(struct RouterModule* module)
{
    uint64_t out = AverageRoller_getAverage(module->gmrtRoller) * 10;
    out = (out < 3000) ? 3000 : out;
    return (out > 30000) ? 30000 : out;
}





static int handleIncoming(struct DHTMessage* message, void* vcontext)
{
    String* txid = Dict_getString(message->asDict, CJDHTConstants_TXID);
    String* query = Dict_getString(message->asDict, CJDHTConstants_QUERY);
    if (query || !txid) {
        return 0;
    }

    struct RouterModule* module = (__extension__ ({ __typeof__((struct RouterModule*) vcontext) Identity_ptr = ((struct RouterModule*) vcontext); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x78a2cb843c088bb5ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("RouterModule.c"), (460), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x78a2cb843c088bb5ull))"); } } while (0); Identity_ptr; }));


    module->currentMessage = message;
    Pinger_pongReceived(txid, module->pinger);
    module->currentMessage = 
# 465 "dht/dhtcore/RouterModule.c" 3 4
                            ((void *)0)
# 465 "dht/dhtcore/RouterModule.c"
                                ;

    return 0;
}


static void onResponseOrTimeout(String* data, uint32_t milliseconds, void* vping)
{
    struct PingContext* pctx = (__extension__ ({ __typeof__((struct PingContext*) vping) Identity_ptr = ((struct PingContext*) vping); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x78a2cb843c088bb5ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("RouterModule.c"), (473), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x78a2cb843c088bb5ull))"); } } while (0); Identity_ptr; }));
    struct RouterModule* module = pctx->router;
    module->pingsInFlight--;
    if (data == 
# 476 "dht/dhtcore/RouterModule.c" 3 4
               ((void *)0)
# 476 "dht/dhtcore/RouterModule.c"
                   ) {

        onTimeout(milliseconds, pctx);
        return;
    }


    struct DHTMessage* message = module->currentMessage;
    module->currentMessage = 
# 484 "dht/dhtcore/RouterModule.c" 3 4
                            ((void *)0)
# 484 "dht/dhtcore/RouterModule.c"
                                ;


    if (!Address_isSameIp(&pctx->address, message->address)) {

            uint8_t expectedAddr[60];
            Address_print(expectedAddr, &pctx->address);
            uint8_t receivedAddr[60];
            Address_print(receivedAddr, message->address);
            do { if (module->logger && Log_Level_WARN >= Log_Level_DEBUG) { Log_print(module->logger, Log_Level_WARN, "RouterModule.c", 493, "Got return packet from different address than search was sent!\n" "Expected:%s\n" "     Got:%s\n", expectedAddr, receivedAddr); } } while (0)




                                  ;

        return;
    }


    AverageRoller_update(pctx->router->gmrtRoller, milliseconds);

    do { if (pctx->router->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(pctx->router->logger, Log_Level_DEBUG, "RouterModule.c", 506, "Received response in %u milliseconds, gmrt now %u\n", milliseconds, AverageRoller_getAverage(pctx->router->gmrtRoller)); } } while (0)


                                                                  ;



    if (milliseconds == 0) { milliseconds++; }

    struct Node_Two* node = NodeStore_closestNode(module->nodeStore, message->address->path);
    if (node && !__builtin_memcmp(node->address.key,message->address->key,32)) {
        NodeStore_pathResponse(module->nodeStore, message->address->path, milliseconds);
    } else {
        NodeStore_discoverNode(module->nodeStore,
                               message->address,
                               message->encodingScheme,
                               message->encIndex,
                               milliseconds);
    }


        String* versionBin = Dict_getString(message->asDict, CJDHTConstants_VERSION);
        if (versionBin && versionBin->len == 20) {
            uint8_t printedAddr[60];
            Address_print(printedAddr, message->address);

            uint8_t versionStr[41];
            Hex_encode(versionStr, 41, (uint8_t*) versionBin->bytes, 20);

            do { if (module->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(module->logger, Log_Level_DEBUG, "RouterModule.c", 535, "Got pong! [%s] ver[%s]\n", printedAddr, versionStr); } } while (0);
        }


    if (pctx->pub.callback) {
        pctx->pub.callback(&pctx->pub, milliseconds, message->address, message->asDict);
    }
}

struct RouterModule_Promise* RouterModule_newMessage(struct Address* addr,
                                                     uint32_t timeoutMilliseconds,
                                                     struct RouterModule* module,
                                                     struct Allocator* alloc)
{
    do { if (!((addr->path == EncodingScheme_convertLabel(module->nodeStore->selfNode->encodingScheme, addr->path, (-5000))))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("RouterModule.c"), (549), "(addr->path == EncodingScheme_convertLabel(module->nodeStore->selfNode->encodingScheme, addr->path, (-5000)))"); } } while (0)


                                                                                      ;

    do { if (!((addr->protocolVersion))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("RouterModule.c"), (554), "(addr->protocolVersion)"); } } while (0);

    module->pingsInFlight++;
    if (timeoutMilliseconds == 0) {
        timeoutMilliseconds = pingTimeoutMilliseconds(module);
    }
    do { if (module->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(module->logger, Log_Level_DEBUG, "RouterModule.c", 560, "Sending ping with [%u] millisecond timeout, [%u] in flight now", timeoutMilliseconds, module->pingsInFlight); } } while (0)
                                                         ;

    struct Pinger_Ping* pp = Pinger_newPing(
# 563 "dht/dhtcore/RouterModule.c" 3 4
                                           ((void *)0)
# 563 "dht/dhtcore/RouterModule.c"
                                               ,
                                            onResponseOrTimeout,
                                            sendMsg,
                                            timeoutMilliseconds,
                                            alloc,
                                            module->pinger);

    struct PingContext* pctx = Allocator__clone((pp->pingAlloc),((&(struct PingContext) { .pub = { .alloc = pp->pingAlloc }, .router = module, .pp = pp })),sizeof(*((&(struct PingContext) { .pub = { .alloc = pp->pingAlloc }, .router = module, .pp = pp }))),"RouterModule.c",570)





       ;
    (pctx)->Identity_verifier = ((unsigned long) 0x78a2cb843c088bb5ull);
    Bits__memcpy(&pctx->address,addr,sizeof(struct Address),"RouterModule.c",578);

    pp->context = pctx;

    return &pctx->pub;
}

void RouterModule_sendMessage(struct RouterModule_Promise* promise, Dict* request)
{
    struct PingContext* pctx = (__extension__ ({ __typeof__((struct PingContext*)promise) Identity_ptr = ((struct PingContext*)promise); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x78a2cb843c088bb5ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("RouterModule.c"), (587), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x78a2cb843c088bb5ull))"); } } while (0); Identity_ptr; }));
    pctx->messageDict = request;

}

struct RouterModule_Promise* RouterModule_pingNode(struct Address* addr,
                                                   uint32_t timeoutMilliseconds,
                                                   struct RouterModule* module,
                                                   struct Allocator* alloc)
{
    struct RouterModule_Promise* promise =
        RouterModule_newMessage(addr, timeoutMilliseconds, module, alloc);
    Dict* d = Dict_new(promise->alloc);
    Dict_putString(d, CJDHTConstants_QUERY, CJDHTConstants_QUERY_PING, promise->alloc);
    RouterModule_sendMessage(promise, d);


        uint8_t buff[60];
        Address_print(buff, addr);
        do { if (module->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(module->logger, Log_Level_DEBUG, "RouterModule.c", 606, "Sending ping [%u] to [%s]", ((struct PingContext*)promise)->pp->handle, buff); } } while (0)
                                                                   ;


    do { if (!((addr->path != 0))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("RouterModule.c"), (610), "(addr->path != 0)"); } } while (0);

    return promise;
}

struct RouterModule_Promise* RouterModule_nextHop(struct Address* whoToAsk,
                                                  uint8_t target[16],
                                                  uint32_t timeoutMilliseconds,
                                                  struct RouterModule* module,
                                                  struct Allocator* alloc)
{
    struct RouterModule_Promise* promise =
        RouterModule_newMessage(whoToAsk, timeoutMilliseconds, module, alloc);
    Dict* d = Dict_new(promise->alloc);
    Dict_putString(d, CJDHTConstants_QUERY, CJDHTConstants_QUERY_NH, promise->alloc);
    String* targetStr = String_newBinary(target, 16, promise->alloc);
    Dict_putString(d, CJDHTConstants_TARGET, targetStr, promise->alloc);
    RouterModule_sendMessage(promise, d);
    return promise;
}

struct RouterModule_Promise* RouterModule_findNode(struct Address* whoToAsk,
                                                   uint8_t target[16],
                                                   uint32_t timeoutMilliseconds,
                                                   struct RouterModule* module,
                                                   struct Allocator* alloc)
{
    struct RouterModule_Promise* promise =
        RouterModule_newMessage(whoToAsk, timeoutMilliseconds, module, alloc);
    Dict* d = Dict_new(promise->alloc);
    Dict_putString(d, CJDHTConstants_QUERY, CJDHTConstants_QUERY_FN, promise->alloc);
    String* targetStr = String_newBinary(target, 16, promise->alloc);
    Dict_putString(d, CJDHTConstants_TARGET, targetStr, promise->alloc);
    RouterModule_sendMessage(promise, d);
    return promise;
}

struct RouterModule_Promise* RouterModule_getPeers(struct Address* addr,
                                                   uint64_t nearbyLabel,
                                                   uint32_t timeoutMilliseconds,
                                                   struct RouterModule* module,
                                                   struct Allocator* alloc)
{
    struct RouterModule_Promise* promise =
        RouterModule_newMessage(addr, timeoutMilliseconds, module, alloc);
    Dict* d = Dict_new(promise->alloc);
    Dict_putString(d, CJDHTConstants_QUERY, CJDHTConstants_QUERY_GP, promise->alloc);

    uint64_t nearbyLabel_be = ((uint64_t)__builtin_bswap64(((uint64_t)(nearbyLabel))));
    uint8_t nearbyLabelBytes[8];
    Bits__memcpy(nearbyLabelBytes,&nearbyLabel_be,8,"RouterModule.c",660);
    String* target = String_newBinary(nearbyLabelBytes, 8, promise->alloc);
    Dict_putString(d, CJDHTConstants_TARGET, target, promise->alloc);

    RouterModule_sendMessage(promise, d);
    return promise;
}

struct Node_Two* RouterModule_nodeForPath(uint64_t path, struct RouterModule* module)
{
    struct Node_Link* link = NodeStore_linkForPath(module->nodeStore, path);
    if (!link) { return 
# 671 "dht/dhtcore/RouterModule.c" 3 4
                       ((void *)0)
# 671 "dht/dhtcore/RouterModule.c"
                           ; }
    return link->child;
}






uint32_t RouterModule_globalMeanResponseTime(struct RouterModule* module)
{
    return (uint32_t) AverageRoller_getAverage(module->gmrtRoller);
}

void RouterModule_peerIsReachable(uint64_t pathToPeer,
                                  uint64_t lagMilliseconds,
                                  struct RouterModule* module)
{
    do { if (!((EncodingScheme_isOneHop(module->nodeStore->selfNode->encodingScheme, pathToPeer)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("RouterModule.c"), (689), "(EncodingScheme_isOneHop(module->nodeStore->selfNode->encodingScheme, pathToPeer))"); } } while (0)
                                                          ;
    struct Node_Two* nn = RouterModule_nodeForPath(pathToPeer, module);
    for (struct Node_Link* peerLink = nn->reversePeers; peerLink; peerLink = peerLink->nextPeer) {
        if (peerLink->parent != module->nodeStore->selfNode) { continue; }
        if (peerLink->cannonicalLabel != pathToPeer) { continue; }
        struct Address address = { .path = 0 };
        Bits__memcpy(&address,&nn->address,sizeof(struct Address),"RouterModule.c",696);
        address.path = pathToPeer;
        NodeStore_discoverNode(module->nodeStore,
                               &address,
                               nn->encodingScheme,
                               peerLink->inverseLinkEncodingFormNumber,
                               lagMilliseconds);
        return;
    }
    do { if (!((0))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("RouterModule.c"), (705), "(0)"); } } while (0);
}
