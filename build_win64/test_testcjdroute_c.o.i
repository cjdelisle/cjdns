# 1 "test/testcjdroute.c"
# 1 "/home/jojapoppa/cjdns//"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "test/testcjdroute.c"
# 15 "test/testcjdroute.c"
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
# 1 "./util/Linker.h" 1
# 20 "./util/Assert.h" 2
struct Linker_x08179033046377691;
# 33 "./util/Assert.h"
__attribute__((__format__ (__printf__, 1, 2)))
__attribute__((__noreturn__))
void Assert_failure(const char* format, ...);
# 19 "./util/Identity.h" 2
# 1 "./util/Constant.h" 1
# 33 "./util/Constant.h"

# 20 "./util/Identity.h" 2



    
# 19 "./memory/Allocator.h" 2


struct Linker_x08392228434311537;





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
struct Linker_x09380203100938957;

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


struct Linker_x06041914839221365;



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
struct Linker_x039000983091066166;

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
struct Linker_x015423624129011237;

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
# 24 "./crypto/random/seed/RandomSeed.h" 2


# 25 "./crypto/random/seed/RandomSeed.h"
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

struct Linker_x0004746880894922034;



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
# 16 "test/testcjdroute.c" 2
# 1 "./crypto/random/test/DeterminentRandomSeed.h" 1
# 21 "./crypto/random/test/DeterminentRandomSeed.h"
struct Linker_x08912947140794392;

struct RandomSeed* DeterminentRandomSeed_new(struct Allocator* alloc, uint8_t buff[64]);
# 17 "test/testcjdroute.c" 2

# 1 "./util/events/Time.h" 1
# 18 "./util/events/Time.h"
# 1 "./util/events/EventBase.h" 1
# 20 "./util/events/EventBase.h"
struct Linker_x035794586421008434;

struct EventBase
{
    int unused;
};

struct EventBase* EventBase_new(struct Allocator* alloc);

int EventBase_eventCount(struct EventBase* eventBase);

void EventBase_beginLoop(struct EventBase* eventBase);

void EventBase_endLoop(struct EventBase* eventBase);
# 19 "./util/events/Time.h" 2

struct Linker_x06861903358342147;




uint64_t Time_hrtime(void);



uint64_t Time_currentTimeMilliseconds(struct EventBase* eventBase);


uint64_t Time_currentTimeSeconds(struct EventBase* eventBase);
# 19 "test/testcjdroute.c" 2

# 1 "./util/CString.h" 1
# 21 "./util/CString.h"
struct Linker_x07222374194087691;

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
# 21 "test/testcjdroute.c" 2
# 1 "./memory/MallocAllocator.h" 1
# 21 "./memory/MallocAllocator.h"
struct Linker_x09453436236676231;
# 30 "./memory/MallocAllocator.h"
struct Allocator* MallocAllocator__new(unsigned long sizeLimit, const char* file, int line);
# 22 "test/testcjdroute.c" 2
# 1 "./wire/Message.h" 1
# 20 "./wire/Message.h"
# 1 "./util/Bits.h" 1
# 21 "./util/Bits.h"
struct Linker_x033862580287380384;


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

struct Linker_x014579222037482076;



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
# 23 "test/testcjdroute.c" 2
# 1 "./test/FuzzTest.h" 1
# 22 "./test/FuzzTest.h"
void CJDNS_FUZZ_MAIN(void* vctx, struct Message* fuzz);
void* CJDNS_FUZZ_INIT(struct Allocator* alloc, struct Random* rand);
# 24 "test/testcjdroute.c" 2


# 1 "/usr/share/mingw-w64/include/stdio.h" 1 3
# 11 "/usr/share/mingw-w64/include/stdio.h" 3
# 1 "/usr/share/mingw-w64/include/_mingw_print_push.h" 1 3
# 12 "/usr/share/mingw-w64/include/stdio.h" 2 3

#pragma pack(push,_CRT_PACKING)
# 26 "/usr/share/mingw-w64/include/stdio.h" 3
  
# 26 "/usr/share/mingw-w64/include/stdio.h" 3
 struct _iobuf {
    char *_ptr;
    int _cnt;
    char *_base;
    int _flag;
    int _file;
    int _charbuf;
    int _bufsiz;
    char *_tmpfname;
  };
  typedef struct _iobuf FILE;
# 80 "/usr/share/mingw-w64/include/stdio.h" 3
# 1 "/usr/share/mingw-w64/include/_mingw_off_t.h" 1 3




  typedef long _off_t;

  typedef long off32_t;





  __extension__ typedef long long _off64_t;

  __extension__ typedef long long off64_t;
# 26 "/usr/share/mingw-w64/include/_mingw_off_t.h" 3
typedef off32_t off_t;
# 81 "/usr/share/mingw-w64/include/stdio.h" 2 3

__attribute__ ((__dllimport__)) FILE *__attribute__((__cdecl__)) __acrt_iob_func(unsigned index);


  __attribute__ ((__dllimport__)) FILE *__attribute__((__cdecl__)) __iob_func(void);
# 104 "/usr/share/mingw-w64/include/stdio.h" 3
  __extension__ typedef long long fpos_t;
# 162 "/usr/share/mingw-w64/include/stdio.h" 3
extern
  __attribute__((__format__ (gnu_scanf, 2, 3))) __attribute__ ((__nonnull__ (2)))
  int __attribute__((__cdecl__)) __mingw_sscanf(const char * __restrict__ _Src,const char * __restrict__ _Format,...);
extern
  __attribute__((__format__ (gnu_scanf, 2, 0))) __attribute__ ((__nonnull__ (2)))
  int __attribute__((__cdecl__)) __mingw_vsscanf (const char * __restrict__ _Str,const char * __restrict__ Format,va_list argp);
extern
  __attribute__((__format__ (gnu_scanf, 1, 2))) __attribute__ ((__nonnull__ (1)))
  int __attribute__((__cdecl__)) __mingw_scanf(const char * __restrict__ _Format,...);
extern
  __attribute__((__format__ (gnu_scanf, 1, 0))) __attribute__ ((__nonnull__ (1)))
  int __attribute__((__cdecl__)) __mingw_vscanf(const char * __restrict__ Format, va_list argp);
extern
  __attribute__((__format__ (gnu_scanf, 2, 3))) __attribute__ ((__nonnull__ (2)))
  int __attribute__((__cdecl__)) __mingw_fscanf(FILE * __restrict__ _File,const char * __restrict__ _Format,...);
extern
  __attribute__((__format__ (gnu_scanf, 2, 0))) __attribute__ ((__nonnull__ (2)))
  int __attribute__((__cdecl__)) __mingw_vfscanf (FILE * __restrict__ fp, const char * __restrict__ Format,va_list argp);

extern
  __attribute__((__format__ (gnu_printf, 3, 0))) __attribute__ ((__nonnull__ (3)))
  int __attribute__((__cdecl__)) __mingw_vsnprintf(char * __restrict__ _DstBuf,size_t _MaxCount,const char * __restrict__ _Format,
                               va_list _ArgList);
extern
  __attribute__((__format__ (gnu_printf, 3, 4))) __attribute__ ((__nonnull__ (3)))
  int __attribute__((__cdecl__)) __mingw_snprintf(char * __restrict__ s, size_t n, const char * __restrict__ format, ...);
extern
  __attribute__((__format__ (gnu_printf, 1, 2))) __attribute__ ((__nonnull__ (1)))
  int __attribute__((__cdecl__)) __mingw_printf(const char * __restrict__ , ... ) __attribute__ ((__nothrow__));
extern
  __attribute__((__format__ (gnu_printf, 1, 0))) __attribute__ ((__nonnull__ (1)))
  int __attribute__((__cdecl__)) __mingw_vprintf (const char * __restrict__ , va_list) __attribute__ ((__nothrow__));
extern
  __attribute__((__format__ (gnu_printf, 2, 3))) __attribute__ ((__nonnull__ (2)))
  int __attribute__((__cdecl__)) __mingw_fprintf (FILE * __restrict__ , const char * __restrict__ , ...) __attribute__ ((__nothrow__));
extern
  __attribute__((__format__ (gnu_printf, 2, 0))) __attribute__ ((__nonnull__ (2)))
  int __attribute__((__cdecl__)) __mingw_vfprintf (FILE * __restrict__ , const char * __restrict__ , va_list) __attribute__ ((__nothrow__));
extern
  __attribute__((__format__ (gnu_printf, 2, 3))) __attribute__ ((__nonnull__ (2)))
  int __attribute__((__cdecl__)) __mingw_sprintf (char * __restrict__ , const char * __restrict__ , ...) __attribute__ ((__nothrow__));
extern
  __attribute__((__format__ (gnu_printf, 2, 0))) __attribute__ ((__nonnull__ (2)))
  int __attribute__((__cdecl__)) __mingw_vsprintf (char * __restrict__ , const char * __restrict__ , va_list) __attribute__ ((__nothrow__));
extern
  __attribute__((__format__ (gnu_printf, 2, 3))) __attribute__((nonnull (1,2)))
  int __attribute__((__cdecl__)) __mingw_asprintf(char ** __restrict__ , const char * __restrict__ , ...) __attribute__ ((__nothrow__));
extern
  __attribute__((__format__ (gnu_printf, 2, 0))) __attribute__((nonnull (1,2)))
  int __attribute__((__cdecl__)) __mingw_vasprintf(char ** __restrict__ , const char * __restrict__ , va_list) __attribute__ ((__nothrow__));
# 506 "/usr/share/mingw-w64/include/stdio.h" 3
  __attribute__((__format__ (ms_printf, 2, 3))) __attribute__ ((__nonnull__ (2)))
  int __attribute__((__cdecl__)) fprintf(FILE * __restrict__ _File,const char * __restrict__ _Format,...);
  __attribute__((__format__ (ms_printf, 1, 2))) __attribute__ ((__nonnull__ (1)))
  int __attribute__((__cdecl__)) printf(const char * __restrict__ _Format,...);
  __attribute__((__format__ (ms_printf, 2, 3))) __attribute__ ((__nonnull__ (2)))
  int __attribute__((__cdecl__)) sprintf(char * __restrict__ _Dest,const char * __restrict__ _Format,...) ;

  __attribute__((__format__ (ms_printf, 2, 0))) __attribute__ ((__nonnull__ (2)))
  int __attribute__((__cdecl__)) vfprintf(FILE * __restrict__ _File,const char * __restrict__ _Format,va_list _ArgList);
  __attribute__((__format__ (ms_printf, 1, 0))) __attribute__ ((__nonnull__ (1)))
  int __attribute__((__cdecl__)) vprintf(const char * __restrict__ _Format,va_list _ArgList);
  __attribute__((__format__ (ms_printf, 2, 0))) __attribute__ ((__nonnull__ (2)))
  int __attribute__((__cdecl__)) vsprintf(char * __restrict__ _Dest,const char * __restrict__ _Format,va_list _Args) ;

  __attribute__((__format__ (ms_scanf, 2, 3))) __attribute__ ((__nonnull__ (2)))
  int __attribute__((__cdecl__)) fscanf(FILE * __restrict__ _File,const char * __restrict__ _Format,...) ;
  __attribute__((__format__ (ms_scanf, 1, 2))) __attribute__ ((__nonnull__ (1)))
  int __attribute__((__cdecl__)) scanf(const char * __restrict__ _Format,...) ;
  __attribute__((__format__ (ms_scanf, 2, 3))) __attribute__ ((__nonnull__ (2)))
  int __attribute__((__cdecl__)) sscanf(const char * __restrict__ _Src,const char * __restrict__ _Format,...) ;






#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow"


  __attribute__((__format__ (ms_scanf, 1, 0))) __attribute__ ((__nonnull__ (1)))
  int __attribute__((__cdecl__)) __ms_vscanf(const char * __restrict__ Format, va_list argp);
  __attribute__((__format__ (ms_scanf, 2, 0))) __attribute__ ((__nonnull__ (2)))
  int __attribute__((__cdecl__)) __ms_vfscanf (FILE * __restrict__ fp, const char * __restrict__ Format,va_list argp);
  __attribute__((__format__ (ms_scanf, 2, 0))) __attribute__ ((__nonnull__ (2)))
  int __attribute__((__cdecl__)) __ms_vsscanf (const char * __restrict__ _Str,const char * __restrict__ Format,va_list argp);

  static __attribute__ ((__unused__)) __inline__ __attribute__((__cdecl__))
  __attribute__((__format__ (ms_scanf, 2, 0))) __attribute__ ((__nonnull__ (2)))
  int vfscanf (FILE *__stream, const char *__format, __builtin_va_list __local_argv)
  {
    return __ms_vfscanf (__stream, __format, __local_argv);
  }

  static __attribute__ ((__unused__)) __inline__ __attribute__((__cdecl__))
  __attribute__((__format__ (ms_scanf, 2, 0))) __attribute__ ((__nonnull__ (2)))
  int vsscanf (const char * __restrict__ __source, const char * __restrict__ __format, __builtin_va_list __local_argv)
  {
    return __ms_vsscanf( __source, __format, __local_argv );
  }
  static __attribute__ ((__unused__)) __inline__ __attribute__((__cdecl__))
  __attribute__((__format__ (ms_scanf, 1, 0))) __attribute__ ((__nonnull__ (1)))
  int vscanf(const char *__format, __builtin_va_list __local_argv)
  {
    return __ms_vscanf (__format, __local_argv);
  }


#pragma GCC diagnostic pop






  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _filbuf(FILE *_File);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _flsbuf(int _Ch,FILE *_File);



  __attribute__ ((__dllimport__)) FILE *__attribute__((__cdecl__)) _fsopen(const char *_Filename,const char *_Mode,int _ShFlag);

  void __attribute__((__cdecl__)) clearerr(FILE *_File);
  int __attribute__((__cdecl__)) fclose(FILE *_File);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _fcloseall(void);



  __attribute__ ((__dllimport__)) FILE *__attribute__((__cdecl__)) _fdopen(int _FileHandle,const char *_Mode);

  int __attribute__((__cdecl__)) feof(FILE *_File);
  int __attribute__((__cdecl__)) ferror(FILE *_File);
  int __attribute__((__cdecl__)) fflush(FILE *_File);
  int __attribute__((__cdecl__)) fgetc(FILE *_File);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _fgetchar(void);
  int __attribute__((__cdecl__)) fgetpos(FILE * __restrict__ _File ,fpos_t * __restrict__ _Pos);
  int __attribute__((__cdecl__)) fgetpos64(FILE * __restrict__ _File ,fpos_t * __restrict__ _Pos);
  char *__attribute__((__cdecl__)) fgets(char * __restrict__ _Buf,int _MaxCount,FILE * __restrict__ _File);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _fileno(FILE *_File);



  __attribute__ ((__dllimport__)) char *__attribute__((__cdecl__)) _tempnam(const char *_DirName,const char *_FilePrefix);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _flushall(void);
  FILE *__attribute__((__cdecl__)) fopen(const char * __restrict__ _Filename,const char * __restrict__ _Mode) ;
  FILE *fopen64(const char * __restrict__ filename,const char * __restrict__ mode);
  int __attribute__((__cdecl__)) fputc(int _Ch,FILE *_File);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _fputchar(int _Ch);
  int __attribute__((__cdecl__)) fputs(const char * __restrict__ _Str,FILE * __restrict__ _File);
  size_t __attribute__((__cdecl__)) fread(void * __restrict__ _DstBuf,size_t _ElementSize,size_t _Count,FILE * __restrict__ _File);
  FILE *__attribute__((__cdecl__)) freopen(const char * __restrict__ _Filename,const char * __restrict__ _Mode,FILE * __restrict__ _File) ;
  int __attribute__((__cdecl__)) fsetpos(FILE *_File,const fpos_t *_Pos);
  int __attribute__((__cdecl__)) fsetpos64(FILE *_File,const fpos_t *_Pos);
  int __attribute__((__cdecl__)) fseek(FILE *_File,long _Offset,int _Origin);
  long __attribute__((__cdecl__)) ftell(FILE *_File);
# 631 "/usr/share/mingw-w64/include/stdio.h" 3
  __extension__ int __attribute__((__cdecl__)) _fseeki64(FILE *_File,long long _Offset,int _Origin);
  __extension__ long long __attribute__((__cdecl__)) _ftelli64(FILE *_File);
  int fseeko64(FILE* stream, _off64_t offset, int whence);
  int fseeko(FILE* stream, _off_t offset, int whence);

  _off_t ftello(FILE * stream);
  _off64_t ftello64(FILE * stream);
# 654 "/usr/share/mingw-w64/include/stdio.h" 3
  size_t __attribute__((__cdecl__)) fwrite(const void * __restrict__ _Str,size_t _Size,size_t _Count,FILE * __restrict__ _File);
  int __attribute__((__cdecl__)) getc(FILE *_File);
  int __attribute__((__cdecl__)) getchar(void);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _getmaxstdio(void);
  char *__attribute__((__cdecl__)) gets(char *_Buffer) ;
  int __attribute__((__cdecl__)) _getw(FILE *_File);


  void __attribute__((__cdecl__)) perror(const char *_ErrMsg);

  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _pclose(FILE *_File);
  __attribute__ ((__dllimport__)) FILE *__attribute__((__cdecl__)) _popen(const char *_Command,const char *_Mode);




  int __attribute__((__cdecl__)) putc(int _Ch,FILE *_File);
  int __attribute__((__cdecl__)) putchar(int _Ch);
  int __attribute__((__cdecl__)) puts(const char *_Str);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _putw(int _Word,FILE *_File);


  int __attribute__((__cdecl__)) remove(const char *_Filename);
  int __attribute__((__cdecl__)) rename(const char *_OldFilename,const char *_NewFilename);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _unlink(const char *_Filename);

  int __attribute__((__cdecl__)) unlink(const char *_Filename) ;


  void __attribute__((__cdecl__)) rewind(FILE *_File);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _rmtmp(void);
  void __attribute__((__cdecl__)) setbuf(FILE * __restrict__ _File,char * __restrict__ _Buffer) ;
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _setmaxstdio(int _Max);
  __attribute__ ((__dllimport__)) unsigned int __attribute__((__cdecl__)) _set_output_format(unsigned int _Format);
  __attribute__ ((__dllimport__)) unsigned int __attribute__((__cdecl__)) _get_output_format(void);
  int __attribute__((__cdecl__)) setvbuf(FILE * __restrict__ _File,char * __restrict__ _Buf,int _Mode,size_t _Size);
# 712 "/usr/share/mingw-w64/include/stdio.h" 3
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _scprintf(const char * __restrict__ _Format,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _snscanf(const char * __restrict__ _Src,size_t _MaxCount,const char * __restrict__ _Format,...) ;

  FILE *__attribute__((__cdecl__)) tmpfile(void) ;
  char *__attribute__((__cdecl__)) tmpnam(char *_Buffer);
  int __attribute__((__cdecl__)) ungetc(int _Ch,FILE *_File);
# 734 "/usr/share/mingw-w64/include/stdio.h" 3
  __attribute__((__format__ (ms_printf, 3, 4))) __attribute__ ((__nonnull__ (3)))
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _snprintf(char * __restrict__ _Dest,size_t _Count,const char * __restrict__ _Format,...) ;
  __attribute__((__format__ (ms_printf, 3, 0))) __attribute__ ((__nonnull__ (3)))
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vsnprintf(char * __restrict__ _Dest,size_t _Count,const char * __restrict__ _Format,va_list _Args) ;
# 761 "/usr/share/mingw-w64/include/stdio.h" 3
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow"


       
       


  __attribute__((__format__ (ms_printf, 3, 0))) __attribute__ ((__nonnull__ (3)))
  int __attribute__((__cdecl__)) __ms_vsnprintf(char * __restrict__ d,size_t n,const char * __restrict__ format,va_list arg)
    ;

  static __attribute__ ((__unused__)) __inline__ __attribute__((__cdecl__))
  __attribute__((__format__ (ms_printf, 3, 0))) __attribute__ ((__nonnull__ (3)))
  int vsnprintf (char * __restrict__ __stream, size_t __n, const char * __restrict__ __format, va_list __local_argv)
  {
    return __ms_vsnprintf (__stream, __n, __format, __local_argv);
  }

  __attribute__((__format__ (ms_printf, 3, 4))) __attribute__ ((__nonnull__ (3)))
  int __attribute__((__cdecl__)) __ms_snprintf(char * __restrict__ s, size_t n, const char * __restrict__ format, ...);


static __attribute__ ((__unused__)) __inline__ __attribute__((__cdecl__))
__attribute__((__format__ (ms_printf, 3, 4))) __attribute__ ((__nonnull__ (3)))
int snprintf (char * __restrict__ __stream, size_t __n, const char * __restrict__ __format, ...)
{
  int __retval;
  __builtin_va_list __local_argv; __builtin_va_start( __local_argv, __format );
  __retval = __ms_vsnprintf (__stream, __n, __format, __local_argv);
  __builtin_va_end( __local_argv );
  return __retval;
}


       
       

#pragma GCC diagnostic pop
# 811 "/usr/share/mingw-w64/include/stdio.h" 3
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vscprintf(const char * __restrict__ _Format,va_list _ArgList);


  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _set_printf_count_output(int _Value);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _get_printf_count_output(void);




                                                     __attribute__ ((__nonnull__ (2)))
  int __attribute__((__cdecl__)) __mingw_swscanf(const wchar_t * __restrict__ _Src,const wchar_t * __restrict__ _Format,...);
                                                     __attribute__ ((__nonnull__ (2)))
  int __attribute__((__cdecl__)) __mingw_vswscanf (const wchar_t * __restrict__ _Str,const wchar_t * __restrict__ Format,va_list argp);
                                                     __attribute__ ((__nonnull__ (1)))
  int __attribute__((__cdecl__)) __mingw_wscanf(const wchar_t * __restrict__ _Format,...);
                                                     __attribute__ ((__nonnull__ (1)))
  int __attribute__((__cdecl__)) __mingw_vwscanf(const wchar_t * __restrict__ Format, va_list argp);
                                                     __attribute__ ((__nonnull__ (2)))
  int __attribute__((__cdecl__)) __mingw_fwscanf(FILE * __restrict__ _File,const wchar_t * __restrict__ _Format,...);
                                                     __attribute__ ((__nonnull__ (2)))
  int __attribute__((__cdecl__)) __mingw_vfwscanf (FILE * __restrict__ fp, const wchar_t * __restrict__ Format,va_list argp);

                                                      __attribute__ ((__nonnull__ (2)))
  int __attribute__((__cdecl__)) __mingw_fwprintf(FILE * __restrict__ _File,const wchar_t * __restrict__ _Format,...);
                                                      __attribute__ ((__nonnull__ (1)))
  int __attribute__((__cdecl__)) __mingw_wprintf(const wchar_t * __restrict__ _Format,...);
                                                     __attribute__ ((__nonnull__ (2)))
  int __attribute__((__cdecl__)) __mingw_vfwprintf(FILE * __restrict__ _File,const wchar_t * __restrict__ _Format,va_list _ArgList);
                                                     __attribute__ ((__nonnull__ (1)))
  int __attribute__((__cdecl__)) __mingw_vwprintf(const wchar_t * __restrict__ _Format,va_list _ArgList);
                                                      __attribute__ ((__nonnull__ (3)))
  int __attribute__((__cdecl__)) __mingw_snwprintf (wchar_t * __restrict__ s, size_t n, const wchar_t * __restrict__ format, ...);
                                                      __attribute__ ((__nonnull__ (3)))
  int __attribute__((__cdecl__)) __mingw_vsnwprintf (wchar_t * __restrict__ , size_t, const wchar_t * __restrict__ , va_list);
                                                      __attribute__ ((__nonnull__ (2)))
  int __attribute__((__cdecl__)) __mingw_swprintf(wchar_t * __restrict__ , const wchar_t * __restrict__ , ...);
                                                      __attribute__ ((__nonnull__ (2)))
  int __attribute__((__cdecl__)) __mingw_vswprintf(wchar_t * __restrict__ , const wchar_t * __restrict__ ,va_list);
# 1061 "/usr/share/mingw-w64/include/stdio.h" 3
  int __attribute__((__cdecl__)) fwscanf(FILE * __restrict__ _File,const wchar_t * __restrict__ _Format,...) ;
  int __attribute__((__cdecl__)) swscanf(const wchar_t * __restrict__ _Src,const wchar_t * __restrict__ _Format,...) ;
  int __attribute__((__cdecl__)) wscanf(const wchar_t * __restrict__ _Format,...) ;

  int __attribute__((__cdecl__)) __ms_vwscanf (const wchar_t * __restrict__ , va_list);
  int __attribute__((__cdecl__)) __ms_vfwscanf (FILE * __restrict__ ,const wchar_t * __restrict__ ,va_list);
  int __attribute__((__cdecl__)) __ms_vswscanf (const wchar_t * __restrict__ ,const wchar_t * __restrict__ ,va_list);

  static __attribute__ ((__unused__)) __inline__ __attribute__((__cdecl__))
  __attribute__ ((__nonnull__ (2)))
  int vfwscanf (FILE *__stream, const wchar_t *__format, __builtin_va_list __local_argv)
  {
    return __ms_vfwscanf (__stream, __format, __local_argv);
  }

  static __attribute__ ((__unused__)) __inline__ __attribute__((__cdecl__))
  __attribute__ ((__nonnull__ (2)))
  int vswscanf (const wchar_t * __restrict__ __source, const wchar_t * __restrict__ __format, __builtin_va_list __local_argv)
  {
    return __ms_vswscanf( __source, __format, __local_argv );
  }
  static __attribute__ ((__unused__)) __inline__ __attribute__((__cdecl__))
  __attribute__ ((__nonnull__ (1)))
  int vwscanf(const wchar_t *__format, __builtin_va_list __local_argv)
  {
    return __ms_vwscanf (__format, __local_argv);
  }



  int __attribute__((__cdecl__)) fwprintf(FILE * __restrict__ _File,const wchar_t * __restrict__ _Format,...);
  int __attribute__((__cdecl__)) wprintf(const wchar_t * __restrict__ _Format,...);
  int __attribute__((__cdecl__)) vfwprintf(FILE * __restrict__ _File,const wchar_t * __restrict__ _Format,va_list _ArgList);
  int __attribute__((__cdecl__)) vwprintf(const wchar_t * __restrict__ _Format,va_list _ArgList);
# 1105 "/usr/share/mingw-w64/include/stdio.h" 3
  __attribute__ ((__dllimport__)) FILE *__attribute__((__cdecl__)) _wfsopen(const wchar_t *_Filename,const wchar_t *_Mode,int _ShFlag);


  wint_t __attribute__((__cdecl__)) fgetwc(FILE *_File);
  __attribute__ ((__dllimport__)) wint_t __attribute__((__cdecl__)) _fgetwchar(void);
  wint_t __attribute__((__cdecl__)) fputwc(wchar_t _Ch,FILE *_File);
  __attribute__ ((__dllimport__)) wint_t __attribute__((__cdecl__)) _fputwchar(wchar_t _Ch);
  wint_t __attribute__((__cdecl__)) getwc(FILE *_File);
  wint_t __attribute__((__cdecl__)) getwchar(void);
  wint_t __attribute__((__cdecl__)) putwc(wchar_t _Ch,FILE *_File);
  wint_t __attribute__((__cdecl__)) putwchar(wchar_t _Ch);
  wint_t __attribute__((__cdecl__)) ungetwc(wint_t _Ch,FILE *_File);
  wchar_t *__attribute__((__cdecl__)) fgetws(wchar_t * __restrict__ _Dst,int _SizeInWords,FILE * __restrict__ _File);
  int __attribute__((__cdecl__)) fputws(const wchar_t * __restrict__ _Str,FILE * __restrict__ _File);
  __attribute__ ((__dllimport__)) wchar_t *__attribute__((__cdecl__)) _getws(wchar_t *_String) ;
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _putws(const wchar_t *_Str);
# 1186 "/usr/share/mingw-w64/include/stdio.h" 3
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _scwprintf(const wchar_t * __restrict__ _Format,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _swprintf_c(wchar_t * __restrict__ _DstBuf,size_t _SizeInWords,const wchar_t * __restrict__ _Format,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vswprintf_c(wchar_t * __restrict__ _DstBuf,size_t _SizeInWords,const wchar_t * __restrict__ _Format,va_list _ArgList);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _snwprintf(wchar_t * __restrict__ _Dest,size_t _Count,const wchar_t * __restrict__ _Format,...) ;
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vsnwprintf(wchar_t * __restrict__ _Dest,size_t _Count,const wchar_t * __restrict__ _Format,va_list _Args) ;
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vscwprintf(const wchar_t * __restrict__ _Format,va_list _ArgList);




       
       


  int __attribute__((__cdecl__)) __ms_snwprintf (wchar_t * __restrict__ s, size_t n, const wchar_t * __restrict__ format, ...);
  int __attribute__((__cdecl__)) __ms_vsnwprintf (wchar_t * __restrict__ , size_t, const wchar_t * __restrict__ , va_list);
  static __attribute__ ((__unused__)) __inline__ __attribute__((__cdecl__))
  int snwprintf (wchar_t * __restrict__ s, size_t n, const wchar_t * __restrict__ format, ...)
  {
    int r;
    va_list argp;
    __builtin_va_start (argp, format);
    r = _vsnwprintf (s, n, format, argp);
    __builtin_va_end (argp);
    return r;
  }
  static __attribute__ ((__unused__)) __inline__ __attribute__((__cdecl__))
  int __attribute__((__cdecl__)) vsnwprintf (wchar_t * __restrict__ s, size_t n, const wchar_t * __restrict__ format, va_list arg)
  {
    return _vsnwprintf(s,n,format,arg);
  }
       
       



  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _swprintf(wchar_t * __restrict__ _Dest,const wchar_t * __restrict__ _Format,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vswprintf(wchar_t * __restrict__ _Dest,const wchar_t * __restrict__ _Format,va_list _Args);



# 1 "/usr/share/mingw-w64/include/swprintf.inl" 1 3
# 25 "/usr/share/mingw-w64/include/swprintf.inl" 3
static __attribute__ ((__unused__)) __inline__ __attribute__((__cdecl__))
                                                      __attribute__ ((__nonnull__ (3)))
int vswprintf (wchar_t *__stream, size_t __count, const wchar_t *__format, __builtin_va_list __local_argv)
{
  return vsnwprintf( __stream, __count, __format, __local_argv );
}

static __attribute__ ((__unused__)) __inline__ __attribute__((__cdecl__))
                                                      __attribute__ ((__nonnull__ (3)))
int swprintf (wchar_t *__stream, size_t __count, const wchar_t *__format, ...)
{
  int __retval;
  __builtin_va_list __local_argv;

  __builtin_va_start( __local_argv, __format );
  __retval = vswprintf( __stream, __count, __format, __local_argv );
  __builtin_va_end( __local_argv );
  return __retval;
}
# 1228 "/usr/share/mingw-w64/include/stdio.h" 2 3
# 1237 "/usr/share/mingw-w64/include/stdio.h" 3
  __attribute__ ((__dllimport__)) wchar_t *__attribute__((__cdecl__)) _wtempnam(const wchar_t *_Directory,const wchar_t *_FilePrefix);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _snwscanf(const wchar_t * __restrict__ _Src,size_t _MaxCount,const wchar_t * __restrict__ _Format,...);
  __attribute__ ((__dllimport__)) FILE *__attribute__((__cdecl__)) _wfdopen(int _FileHandle ,const wchar_t *_Mode);
  __attribute__ ((__dllimport__)) FILE *__attribute__((__cdecl__)) _wfopen(const wchar_t * __restrict__ _Filename,const wchar_t *__restrict__ _Mode) ;
  __attribute__ ((__dllimport__)) FILE *__attribute__((__cdecl__)) _wfreopen(const wchar_t * __restrict__ _Filename,const wchar_t * __restrict__ _Mode,FILE * __restrict__ _OldFile) ;



  __attribute__ ((__dllimport__)) void __attribute__((__cdecl__)) _wperror(const wchar_t *_ErrMsg);

  __attribute__ ((__dllimport__)) FILE *__attribute__((__cdecl__)) _wpopen(const wchar_t *_Command,const wchar_t *_Mode);




  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wremove(const wchar_t *_Filename);
  __attribute__ ((__dllimport__)) wchar_t *__attribute__((__cdecl__)) _wtmpnam(wchar_t *_Buffer);
  __attribute__ ((__dllimport__)) wint_t __attribute__((__cdecl__)) _fgetwc_nolock(FILE *_File);
  __attribute__ ((__dllimport__)) wint_t __attribute__((__cdecl__)) _fputwc_nolock(wchar_t _Ch,FILE *_File);
  __attribute__ ((__dllimport__)) wint_t __attribute__((__cdecl__)) _ungetwc_nolock(wint_t _Ch,FILE *_File);
# 1293 "/usr/share/mingw-w64/include/stdio.h" 3
  __attribute__ ((__dllimport__)) void __attribute__((__cdecl__)) _lock_file(FILE *_File);
  __attribute__ ((__dllimport__)) void __attribute__((__cdecl__)) _unlock_file(FILE *_File);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _fclose_nolock(FILE *_File);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _fflush_nolock(FILE *_File);
  __attribute__ ((__dllimport__)) size_t __attribute__((__cdecl__)) _fread_nolock(void * __restrict__ _DstBuf,size_t _ElementSize,size_t _Count,FILE * __restrict__ _File);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _fseek_nolock(FILE *_File,long _Offset,int _Origin);
  __attribute__ ((__dllimport__)) long __attribute__((__cdecl__)) _ftell_nolock(FILE *_File);
  __extension__ __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _fseeki64_nolock(FILE *_File,long long _Offset,int _Origin);
  __extension__ __attribute__ ((__dllimport__)) long long __attribute__((__cdecl__)) _ftelli64_nolock(FILE *_File);
  __attribute__ ((__dllimport__)) size_t __attribute__((__cdecl__)) _fwrite_nolock(const void * __restrict__ _DstBuf,size_t _Size,size_t _Count,FILE * __restrict__ _File);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _ungetc_nolock(int _Ch,FILE *_File);





  char *__attribute__((__cdecl__)) tempnam(const char *_Directory,const char *_FilePrefix) ;
  int __attribute__((__cdecl__)) fcloseall(void) ;
  FILE *__attribute__((__cdecl__)) fdopen(int _FileHandle,const char *_Format) ;
  int __attribute__((__cdecl__)) fgetchar(void) ;
  int __attribute__((__cdecl__)) fileno(FILE *_File) ;
  int __attribute__((__cdecl__)) flushall(void) ;
  int __attribute__((__cdecl__)) fputchar(int _Ch) ;
  int __attribute__((__cdecl__)) getw(FILE *_File) ;
  int __attribute__((__cdecl__)) putw(int _Ch,FILE *_File) ;
  int __attribute__((__cdecl__)) rmtmp(void) ;
# 1335 "/usr/share/mingw-w64/include/stdio.h" 3
int __attribute__((__cdecl__)) __mingw_str_wide_utf8 (const wchar_t * const wptr, char **mbptr, size_t * buflen);
# 1349 "/usr/share/mingw-w64/include/stdio.h" 3
int __attribute__((__cdecl__)) __mingw_str_utf8_wide (const char *const mbptr, wchar_t ** wptr, size_t * buflen);
# 1358 "/usr/share/mingw-w64/include/stdio.h" 3
void __attribute__((__cdecl__)) __mingw_str_free(void *ptr);





  __attribute__ ((__dllimport__)) intptr_t __attribute__((__cdecl__)) _wspawnl(int _Mode,const wchar_t *_Filename,const wchar_t *_ArgList,...);
  __attribute__ ((__dllimport__)) intptr_t __attribute__((__cdecl__)) _wspawnle(int _Mode,const wchar_t *_Filename,const wchar_t *_ArgList,...);
  __attribute__ ((__dllimport__)) intptr_t __attribute__((__cdecl__)) _wspawnlp(int _Mode,const wchar_t *_Filename,const wchar_t *_ArgList,...);
  __attribute__ ((__dllimport__)) intptr_t __attribute__((__cdecl__)) _wspawnlpe(int _Mode,const wchar_t *_Filename,const wchar_t *_ArgList,...);
  __attribute__ ((__dllimport__)) intptr_t __attribute__((__cdecl__)) _wspawnv(int _Mode,const wchar_t *_Filename,const wchar_t *const *_ArgList);
  __attribute__ ((__dllimport__)) intptr_t __attribute__((__cdecl__)) _wspawnve(int _Mode,const wchar_t *_Filename,const wchar_t *const *_ArgList,const wchar_t *const *_Env);
  __attribute__ ((__dllimport__)) intptr_t __attribute__((__cdecl__)) _wspawnvp(int _Mode,const wchar_t *_Filename,const wchar_t *const *_ArgList);
  __attribute__ ((__dllimport__)) intptr_t __attribute__((__cdecl__)) _wspawnvpe(int _Mode,const wchar_t *_Filename,const wchar_t *const *_ArgList,const wchar_t *const *_Env);
# 1388 "/usr/share/mingw-w64/include/stdio.h" 3
  __attribute__ ((__dllimport__)) intptr_t __attribute__((__cdecl__)) _spawnv(int _Mode,const char *_Filename,const char *const *_ArgList);
  __attribute__ ((__dllimport__)) intptr_t __attribute__((__cdecl__)) _spawnve(int _Mode,const char *_Filename,const char *const *_ArgList,const char *const *_Env);
  __attribute__ ((__dllimport__)) intptr_t __attribute__((__cdecl__)) _spawnvp(int _Mode,const char *_Filename,const char *const *_ArgList);
  __attribute__ ((__dllimport__)) intptr_t __attribute__((__cdecl__)) _spawnvpe(int _Mode,const char *_Filename,const char *const *_ArgList,const char *const *_Env);






#pragma pack(pop)

# 1 "/usr/share/mingw-w64/include/sec_api/stdio_s.h" 1 3
# 9 "/usr/share/mingw-w64/include/sec_api/stdio_s.h" 3
# 1 "/usr/share/mingw-w64/include/stdio.h" 1 3
# 10 "/usr/share/mingw-w64/include/sec_api/stdio_s.h" 2 3
# 28 "/usr/share/mingw-w64/include/sec_api/stdio_s.h" 3
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) clearerr_s(FILE *_File);

  size_t __attribute__((__cdecl__)) fread_s(void *_DstBuf,size_t _DstSize,size_t _ElementSize,size_t _Count,FILE *_File);
# 471 "/usr/share/mingw-w64/include/sec_api/stdio_s.h" 3
  int __attribute__((__cdecl__)) fprintf_s(FILE *_File,const char *_Format,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _fscanf_s_l(FILE *_File,const char *_Format,_locale_t _Locale,...);
  int __attribute__((__cdecl__)) printf_s(const char *_Format,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _scanf_l(const char *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _scanf_s_l(const char *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _snprintf_c(char *_DstBuf,size_t _MaxCount,const char *_Format,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vsnprintf_c(char *_DstBuf,size_t _MaxCount,const char *_Format,va_list _ArgList);

  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _fscanf_l(FILE *_File,const char *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _sscanf_l(const char *_Src,const char *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _sscanf_s_l(const char *_Src,const char *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) sscanf_s(const char *_Src,const char *_Format,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _snscanf_s(const char *_Src,size_t _MaxCount,const char *_Format,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _snscanf_l(const char *_Src,size_t _MaxCount,const char *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _snscanf_s_l(const char *_Src,size_t _MaxCount,const char *_Format,_locale_t _Locale,...);
  int __attribute__((__cdecl__)) vfprintf_s(FILE *_File,const char *_Format,va_list _ArgList);
  int __attribute__((__cdecl__)) vprintf_s(const char *_Format,va_list _ArgList);

  int __attribute__((__cdecl__)) vsnprintf_s(char *_DstBuf,size_t _DstSize,size_t _MaxCount,const char *_Format,va_list _ArgList);

  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vsnprintf_s(char *_DstBuf,size_t _DstSize,size_t _MaxCount,const char *_Format,va_list _ArgList);

  __attribute__((dllimport)) int __attribute__((__cdecl__)) vsprintf_s(char *_DstBuf,size_t _Size,const char *_Format,va_list _ArgList);

  __attribute__((dllimport)) int __attribute__((__cdecl__)) sprintf_s(char *_DstBuf,size_t _DstSize,const char *_Format,...);

  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _snprintf_s(char *_DstBuf,size_t _DstSize,size_t _MaxCount,const char *_Format,...);

  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _fprintf_p(FILE *_File,const char *_Format,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _printf_p(const char *_Format,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _sprintf_p(char *_Dst,size_t _MaxCount,const char *_Format,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vfprintf_p(FILE *_File,const char *_Format,va_list _ArgList);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vprintf_p(const char *_Format,va_list _ArgList);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vsprintf_p(char *_Dst,size_t _MaxCount,const char *_Format,va_list _ArgList);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _scprintf_p(const char *_Format,...);
  __attribute__((dllimport)) int __attribute__((__cdecl__)) _vscprintf_p(const char *_Format,va_list _ArgList);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _printf_l(const char *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _printf_p_l(const char *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vprintf_l(const char *_Format,_locale_t _Locale,va_list _ArgList);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vprintf_p_l(const char *_Format,_locale_t _Locale,va_list _ArgList);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _fprintf_l(FILE *_File,const char *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _fprintf_p_l(FILE *_File,const char *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vfprintf_l(FILE *_File,const char *_Format,_locale_t _Locale,va_list _ArgList);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vfprintf_p_l(FILE *_File,const char *_Format,_locale_t _Locale,va_list _ArgList);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _sprintf_l(char *_DstBuf,const char *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _sprintf_p_l(char *_DstBuf,size_t _MaxCount,const char *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vsprintf_l(char *_DstBuf,const char *_Format,_locale_t,va_list _ArgList);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vsprintf_p_l(char *_DstBuf,size_t _MaxCount,const char *_Format,_locale_t _Locale,va_list _ArgList);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _scprintf_l(const char *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _scprintf_p_l(const char *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vscprintf_l(const char *_Format,_locale_t _Locale,va_list _ArgList);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vscprintf_p_l(const char *_Format,_locale_t _Locale,va_list _ArgList);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _printf_s_l(const char *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vprintf_s_l(const char *_Format,_locale_t _Locale,va_list _ArgList);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _fprintf_s_l(FILE *_File,const char *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vfprintf_s_l(FILE *_File,const char *_Format,_locale_t _Locale,va_list _ArgList);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _sprintf_s_l(char *_DstBuf,size_t _DstSize,const char *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vsprintf_s_l(char *_DstBuf,size_t _DstSize,const char *_Format,_locale_t _Locale,va_list _ArgList);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _snprintf_s_l(char *_DstBuf,size_t _DstSize,size_t _MaxCount,const char *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vsnprintf_s_l(char *_DstBuf,size_t _DstSize,size_t _MaxCount,const char *_Format,_locale_t _Locale,va_list _ArgList);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _snprintf_l(char *_DstBuf,size_t _MaxCount,const char *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _snprintf_c_l(char *_DstBuf,size_t _MaxCount,const char *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vsnprintf_l(char *_DstBuf,size_t _MaxCount,const char *_Format,_locale_t _Locale,va_list _ArgList);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vsnprintf_c_l(char *_DstBuf,size_t _MaxCount,const char *,_locale_t _Locale,va_list _ArgList);


 
 
 
 
 

  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) fopen_s(FILE **_File,const char *_Filename,const char *_Mode);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) freopen_s(FILE** _File, const char *_Filename, const char *_Mode, FILE *_Stream);

  __attribute__ ((__dllimport__)) char* __attribute__((__cdecl__)) gets_s(char*,rsize_t);
 

  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) tmpnam_s(char*,rsize_t);
 




  __attribute__ ((__dllimport__)) wchar_t *__attribute__((__cdecl__)) _getws_s(wchar_t *_Str,size_t _SizeInWords);
 
# 739 "/usr/share/mingw-w64/include/sec_api/stdio_s.h" 3
  int __attribute__((__cdecl__)) fwprintf_s(FILE *_File,const wchar_t *_Format,...);
  int __attribute__((__cdecl__)) wprintf_s(const wchar_t *_Format,...);
  int __attribute__((__cdecl__)) vfwprintf_s(FILE *_File,const wchar_t *_Format,va_list _ArgList);
  int __attribute__((__cdecl__)) vwprintf_s(const wchar_t *_Format,va_list _ArgList);

  int __attribute__((__cdecl__)) vswprintf_s(wchar_t *_Dst,size_t _SizeInWords,const wchar_t *_Format,va_list _ArgList);

  int __attribute__((__cdecl__)) swprintf_s(wchar_t *_Dst,size_t _SizeInWords,const wchar_t *_Format,...);

  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vsnwprintf_s(wchar_t *_DstBuf,size_t _DstSizeInWords,size_t _MaxCount,const wchar_t *_Format,va_list _ArgList);

  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _snwprintf_s(wchar_t *_DstBuf,size_t _DstSizeInWords,size_t _MaxCount,const wchar_t *_Format,...);


  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wprintf_s_l(const wchar_t *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vwprintf_s_l(const wchar_t *_Format,_locale_t _Locale,va_list _ArgList);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _fwprintf_s_l(FILE *_File,const wchar_t *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vfwprintf_s_l(FILE *_File,const wchar_t *_Format,_locale_t _Locale,va_list _ArgList);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _swprintf_s_l(wchar_t *_DstBuf,size_t _DstSize,const wchar_t *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vswprintf_s_l(wchar_t *_DstBuf,size_t _DstSize,const wchar_t *_Format,_locale_t _Locale,va_list _ArgList);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _snwprintf_s_l(wchar_t *_DstBuf,size_t _DstSize,size_t _MaxCount,const wchar_t *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vsnwprintf_s_l(wchar_t *_DstBuf,size_t _DstSize,size_t _MaxCount,const wchar_t *_Format,_locale_t _Locale,va_list _ArgList);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _fwscanf_s_l(FILE *_File,const wchar_t *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _swscanf_s_l(const wchar_t *_Src,const wchar_t *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) swscanf_s(const wchar_t *_Src,const wchar_t *_Format,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _snwscanf_s(const wchar_t *_Src,size_t _MaxCount,const wchar_t *_Format,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _snwscanf_s_l(const wchar_t *_Src,size_t _MaxCount,const wchar_t *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wscanf_s_l(const wchar_t *_Format,_locale_t _Locale,...);


 
 
 
 

  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _wfopen_s(FILE **_File,const wchar_t *_Filename,const wchar_t *_Mode);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _wfreopen_s(FILE **_File,const wchar_t *_Filename,const wchar_t *_Mode,FILE *_OldFile);

  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _wtmpnam_s(wchar_t *_DstBuf,size_t _SizeInWords);
 


  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _fwprintf_p(FILE *_File,const wchar_t *_Format,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wprintf_p(const wchar_t *_Format,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vfwprintf_p(FILE *_File,const wchar_t *_Format,va_list _ArgList);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vwprintf_p(const wchar_t *_Format,va_list _ArgList);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _swprintf_p(wchar_t *_DstBuf,size_t _MaxCount,const wchar_t *_Format,...);
  __attribute__((dllimport)) int __attribute__((__cdecl__)) _vswprintf_p(wchar_t *_DstBuf,size_t _MaxCount,const wchar_t *_Format,va_list _ArgList);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _scwprintf_p(const wchar_t *_Format,...);
  __attribute__((dllimport)) int __attribute__((__cdecl__)) _vscwprintf_p(const wchar_t *_Format,va_list _ArgList);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wprintf_l(const wchar_t *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wprintf_p_l(const wchar_t *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vwprintf_l(const wchar_t *_Format,_locale_t _Locale,va_list _ArgList);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vwprintf_p_l(const wchar_t *_Format,_locale_t _Locale,va_list _ArgList);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _fwprintf_l(FILE *_File,const wchar_t *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _fwprintf_p_l(FILE *_File,const wchar_t *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vfwprintf_l(FILE *_File,const wchar_t *_Format,_locale_t _Locale,va_list _ArgList);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vfwprintf_p_l(FILE *_File,const wchar_t *_Format,_locale_t _Locale,va_list _ArgList);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _swprintf_c_l(wchar_t *_DstBuf,size_t _MaxCount,const wchar_t *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _swprintf_p_l(wchar_t *_DstBuf,size_t _MaxCount,const wchar_t *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vswprintf_c_l(wchar_t *_DstBuf,size_t _MaxCount,const wchar_t *_Format,_locale_t _Locale,va_list _ArgList);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vswprintf_p_l(wchar_t *_DstBuf,size_t _MaxCount,const wchar_t *_Format,_locale_t _Locale,va_list _ArgList);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _scwprintf_l(const wchar_t *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _scwprintf_p_l(const wchar_t *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vscwprintf_p_l(const wchar_t *_Format,_locale_t _Locale,va_list _ArgList);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _snwprintf_l(wchar_t *_DstBuf,size_t _MaxCount,const wchar_t *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vsnwprintf_l(wchar_t *_DstBuf,size_t _MaxCount,const wchar_t *_Format,_locale_t _Locale,va_list _ArgList);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) __swprintf_l(wchar_t *_Dest,const wchar_t *_Format,_locale_t _Plocinfo,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) __vswprintf_l(wchar_t *_Dest,const wchar_t *_Format,_locale_t _Plocinfo,va_list _Args);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vscwprintf_l(const wchar_t *_Format,_locale_t _Locale,va_list _ArgList);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _fwscanf_l(FILE *_File,const wchar_t *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _swscanf_l(const wchar_t *_Src,const wchar_t *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _snwscanf_l(const wchar_t *_Src,size_t _MaxCount,const wchar_t *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wscanf_l(const wchar_t *_Format,_locale_t _Locale,...);





  __attribute__ ((__dllimport__)) size_t __attribute__((__cdecl__)) _fread_nolock_s(void *_DstBuf,size_t _DstSize,size_t _ElementSize,size_t _Count,FILE *_File);
# 1401 "/usr/share/mingw-w64/include/stdio.h" 2 3

# 1 "/usr/share/mingw-w64/include/_mingw_print_pop.h" 1 3
# 1403 "/usr/share/mingw-w64/include/stdio.h" 2 3
# 27 "test/testcjdroute.c" 2
# 1 "/usr/share/mingw-w64/include/unistd.h" 1 3
# 10 "/usr/share/mingw-w64/include/unistd.h" 3
# 1 "/usr/share/mingw-w64/include/io.h" 1 3
# 10 "/usr/share/mingw-w64/include/io.h" 3
# 1 "/usr/share/mingw-w64/include/string.h" 1 3
# 45 "/usr/share/mingw-w64/include/string.h" 3
  __attribute__ ((__dllimport__)) void *__attribute__((__cdecl__)) _memccpy(void *_Dst,const void *_Src,int _Val,size_t _MaxCount);
  void *__attribute__((__cdecl__)) memchr(const void *_Buf ,int _Val,size_t _MaxCount);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _memicmp(const void *_Buf1,const void *_Buf2,size_t _Size);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _memicmp_l(const void *_Buf1,const void *_Buf2,size_t _Size,_locale_t _Locale);
  int __attribute__((__cdecl__)) memcmp(const void *_Buf1,const void *_Buf2,size_t _Size);
  void * __attribute__((__cdecl__)) memcpy(void * __restrict__ _Dst,const void * __restrict__ _Src,size_t _Size) ;
  __attribute__((dllimport)) errno_t __attribute__((__cdecl__)) memcpy_s (void *_dest,size_t _numberOfElements,const void *_src,size_t _count);
  void * __attribute__((__cdecl__)) mempcpy (void *_Dst, const void *_Src, size_t _Size);
  void * __attribute__((__cdecl__)) memset(void *_Dst,int _Val,size_t _Size);

  void * __attribute__((__cdecl__)) memccpy(void *_Dst,const void *_Src,int _Val,size_t _Size) ;
  int __attribute__((__cdecl__)) memicmp(const void *_Buf1,const void *_Buf2,size_t _Size) ;


  char * __attribute__((__cdecl__)) _strset(char *_Str,int _Val) ;
  char * __attribute__((__cdecl__)) _strset_l(char *_Str,int _Val,_locale_t _Locale) ;
  char * __attribute__((__cdecl__)) strcpy(char * __restrict__ _Dest,const char * __restrict__ _Source);
  char * __attribute__((__cdecl__)) strcat(char * __restrict__ _Dest,const char * __restrict__ _Source);
  int __attribute__((__cdecl__)) strcmp(const char *_Str1,const char *_Str2);
  size_t __attribute__((__cdecl__)) strlen(const char *_Str);
  size_t __attribute__((__cdecl__)) strnlen(const char *_Str,size_t _MaxCount);
  void *__attribute__((__cdecl__)) memmove(void *_Dst,const void *_Src,size_t _Size) ;
  __attribute__ ((__dllimport__)) char *__attribute__((__cdecl__)) _strdup(const char *_Src);
  char *__attribute__((__cdecl__)) strchr(const char *_Str,int _Val);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _stricmp(const char *_Str1,const char *_Str2);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _strcmpi(const char *_Str1,const char *_Str2);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _stricmp_l(const char *_Str1,const char *_Str2,_locale_t _Locale);
  int __attribute__((__cdecl__)) strcoll(const char *_Str1,const char *_Str2);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _strcoll_l(const char *_Str1,const char *_Str2,_locale_t _Locale);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _stricoll(const char *_Str1,const char *_Str2);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _stricoll_l(const char *_Str1,const char *_Str2,_locale_t _Locale);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _strncoll (const char *_Str1,const char *_Str2,size_t _MaxCount);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _strncoll_l(const char *_Str1,const char *_Str2,size_t _MaxCount,_locale_t _Locale);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _strnicoll (const char *_Str1,const char *_Str2,size_t _MaxCount);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _strnicoll_l(const char *_Str1,const char *_Str2,size_t _MaxCount,_locale_t _Locale);
  size_t __attribute__((__cdecl__)) strcspn(const char *_Str,const char *_Control);
  __attribute__ ((__dllimport__)) char *__attribute__((__cdecl__)) _strerror(const char *_ErrMsg) ;
  char *__attribute__((__cdecl__)) strerror(int) ;
  __attribute__ ((__dllimport__)) char *__attribute__((__cdecl__)) _strlwr(char *_String) ;
  char *strlwr_l(char *_String,_locale_t _Locale) ;
  char *__attribute__((__cdecl__)) strncat(char * __restrict__ _Dest,const char * __restrict__ _Source,size_t _Count) ;
  int __attribute__((__cdecl__)) strncmp(const char *_Str1,const char *_Str2,size_t _MaxCount);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _strnicmp(const char *_Str1,const char *_Str2,size_t _MaxCount);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _strnicmp_l(const char *_Str1,const char *_Str2,size_t _MaxCount,_locale_t _Locale);
  char *strncpy(char * __restrict__ _Dest,const char * __restrict__ _Source,size_t _Count) ;
  __attribute__ ((__dllimport__)) char *__attribute__((__cdecl__)) _strnset(char *_Str,int _Val,size_t _MaxCount) ;
  __attribute__ ((__dllimport__)) char *__attribute__((__cdecl__)) _strnset_l(char *str,int c,size_t count,_locale_t _Locale) ;
  char *__attribute__((__cdecl__)) strpbrk(const char *_Str,const char *_Control);
  char *__attribute__((__cdecl__)) strrchr(const char *_Str,int _Ch);
  __attribute__ ((__dllimport__)) char *__attribute__((__cdecl__)) _strrev(char *_Str);
  size_t __attribute__((__cdecl__)) strspn(const char *_Str,const char *_Control);
  char *__attribute__((__cdecl__)) strstr(const char *_Str,const char *_SubStr);
  char *__attribute__((__cdecl__)) strtok(char * __restrict__ _Str,const char * __restrict__ _Delim) ;
       

  char *strtok_r(char * __restrict__ _Str, const char * __restrict__ _Delim, char ** __restrict__ __last);
       
  __attribute__ ((__dllimport__)) char *__attribute__((__cdecl__)) _strupr(char *_String) ;
  __attribute__ ((__dllimport__)) char *_strupr_l(char *_String,_locale_t _Locale) ;
  size_t __attribute__((__cdecl__)) strxfrm(char * __restrict__ _Dst,const char * __restrict__ _Src,size_t _MaxCount);
  __attribute__ ((__dllimport__)) size_t __attribute__((__cdecl__)) _strxfrm_l(char * __restrict__ _Dst,const char * __restrict__ _Src,size_t _MaxCount,_locale_t _Locale);


  char *__attribute__((__cdecl__)) strdup(const char *_Src) ;
  int __attribute__((__cdecl__)) strcmpi(const char *_Str1,const char *_Str2) ;
  int __attribute__((__cdecl__)) stricmp(const char *_Str1,const char *_Str2) ;
  char *__attribute__((__cdecl__)) strlwr(char *_Str) ;
  int __attribute__((__cdecl__)) strnicmp(const char *_Str1,const char *_Str,size_t _MaxCount) ;
  int __attribute__((__cdecl__)) strncasecmp (const char *, const char *, size_t);
  int __attribute__((__cdecl__)) strcasecmp (const char *, const char *);







  char *__attribute__((__cdecl__)) strnset(char *_Str,int _Val,size_t _MaxCount) ;
  char *__attribute__((__cdecl__)) strrev(char *_Str) ;
  char *__attribute__((__cdecl__)) strset(char *_Str,int _Val) ;
  char *__attribute__((__cdecl__)) strupr(char *_Str) ;





  __attribute__ ((__dllimport__)) wchar_t *__attribute__((__cdecl__)) _wcsdup(const wchar_t *_Str);
  wchar_t *__attribute__((__cdecl__)) wcscat(wchar_t * __restrict__ _Dest,const wchar_t * __restrict__ _Source) ;
  wchar_t *__attribute__((__cdecl__)) wcschr(const wchar_t *_Str,wchar_t _Ch);
  int __attribute__((__cdecl__)) wcscmp(const wchar_t *_Str1,const wchar_t *_Str2);
  wchar_t *__attribute__((__cdecl__)) wcscpy(wchar_t * __restrict__ _Dest,const wchar_t * __restrict__ _Source) ;
  size_t __attribute__((__cdecl__)) wcscspn(const wchar_t *_Str,const wchar_t *_Control);
  size_t __attribute__((__cdecl__)) wcslen(const wchar_t *_Str);
  size_t __attribute__((__cdecl__)) wcsnlen(const wchar_t *_Src,size_t _MaxCount);
  wchar_t *wcsncat(wchar_t * __restrict__ _Dest,const wchar_t * __restrict__ _Source,size_t _Count) ;
  int __attribute__((__cdecl__)) wcsncmp(const wchar_t *_Str1,const wchar_t *_Str2,size_t _MaxCount);
  wchar_t *wcsncpy(wchar_t * __restrict__ _Dest,const wchar_t * __restrict__ _Source,size_t _Count) ;
  wchar_t *__attribute__((__cdecl__)) _wcsncpy_l(wchar_t * __restrict__ _Dest,const wchar_t * __restrict__ _Source,size_t _Count,_locale_t _Locale) ;
  wchar_t *__attribute__((__cdecl__)) wcspbrk(const wchar_t *_Str,const wchar_t *_Control);
  wchar_t *__attribute__((__cdecl__)) wcsrchr(const wchar_t *_Str,wchar_t _Ch);
  size_t __attribute__((__cdecl__)) wcsspn(const wchar_t *_Str,const wchar_t *_Control);
  wchar_t *__attribute__((__cdecl__)) wcsstr(const wchar_t *_Str,const wchar_t *_SubStr);
  wchar_t *__attribute__((__cdecl__)) wcstok(wchar_t * __restrict__ _Str,const wchar_t * __restrict__ _Delim) ;
  __attribute__ ((__dllimport__)) wchar_t *__attribute__((__cdecl__)) _wcserror(int _ErrNum) ;
  __attribute__ ((__dllimport__)) wchar_t *__attribute__((__cdecl__)) __wcserror(const wchar_t *_Str) ;
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wcsicmp(const wchar_t *_Str1,const wchar_t *_Str2);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wcsicmp_l(const wchar_t *_Str1,const wchar_t *_Str2,_locale_t _Locale);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wcsnicmp(const wchar_t *_Str1,const wchar_t *_Str2,size_t _MaxCount);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wcsnicmp_l(const wchar_t *_Str1,const wchar_t *_Str2,size_t _MaxCount,_locale_t _Locale);
  __attribute__ ((__dllimport__)) wchar_t *__attribute__((__cdecl__)) _wcsnset(wchar_t *_Str,wchar_t _Val,size_t _MaxCount) ;
  __attribute__ ((__dllimport__)) wchar_t *__attribute__((__cdecl__)) _wcsrev(wchar_t *_Str);
  __attribute__ ((__dllimport__)) wchar_t *__attribute__((__cdecl__)) _wcsset(wchar_t *_Str,wchar_t _Val) ;
  __attribute__ ((__dllimport__)) wchar_t *__attribute__((__cdecl__)) _wcslwr(wchar_t *_String) ;
  __attribute__ ((__dllimport__)) wchar_t *_wcslwr_l(wchar_t *_String,_locale_t _Locale) ;
  __attribute__ ((__dllimport__)) wchar_t *__attribute__((__cdecl__)) _wcsupr(wchar_t *_String) ;
  __attribute__ ((__dllimport__)) wchar_t *_wcsupr_l(wchar_t *_String,_locale_t _Locale) ;
  size_t __attribute__((__cdecl__)) wcsxfrm(wchar_t * __restrict__ _Dst,const wchar_t * __restrict__ _Src,size_t _MaxCount);
  __attribute__ ((__dllimport__)) size_t __attribute__((__cdecl__)) _wcsxfrm_l(wchar_t * __restrict__ _Dst,const wchar_t * __restrict__ _Src,size_t _MaxCount,_locale_t _Locale);
  int __attribute__((__cdecl__)) wcscoll(const wchar_t *_Str1,const wchar_t *_Str2);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wcscoll_l(const wchar_t *_Str1,const wchar_t *_Str2,_locale_t _Locale);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wcsicoll(const wchar_t *_Str1,const wchar_t *_Str2);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wcsicoll_l(const wchar_t *_Str1,const wchar_t *_Str2,_locale_t _Locale);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wcsncoll(const wchar_t *_Str1,const wchar_t *_Str2,size_t _MaxCount);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wcsncoll_l(const wchar_t *_Str1,const wchar_t *_Str2,size_t _MaxCount,_locale_t _Locale);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wcsnicoll(const wchar_t *_Str1,const wchar_t *_Str2,size_t _MaxCount);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wcsnicoll_l(const wchar_t *_Str1,const wchar_t *_Str2,size_t _MaxCount,_locale_t _Locale);


  wchar_t *__attribute__((__cdecl__)) wcsdup(const wchar_t *_Str) ;

  int __attribute__((__cdecl__)) wcsicmp(const wchar_t *_Str1,const wchar_t *_Str2) ;
  int __attribute__((__cdecl__)) wcsnicmp(const wchar_t *_Str1,const wchar_t *_Str2,size_t _MaxCount) ;
  wchar_t *__attribute__((__cdecl__)) wcsnset(wchar_t *_Str,wchar_t _Val,size_t _MaxCount) ;
  wchar_t *__attribute__((__cdecl__)) wcsrev(wchar_t *_Str) ;
  wchar_t *__attribute__((__cdecl__)) wcsset(wchar_t *_Str,wchar_t _Val) ;
  wchar_t *__attribute__((__cdecl__)) wcslwr(wchar_t *_Str) ;
  wchar_t *__attribute__((__cdecl__)) wcsupr(wchar_t *_Str) ;
  int __attribute__((__cdecl__)) wcsicoll(const wchar_t *_Str1,const wchar_t *_Str2) ;







# 1 "/usr/share/mingw-w64/include/sec_api/string_s.h" 1 3
# 9 "/usr/share/mingw-w64/include/sec_api/string_s.h" 3
# 1 "/usr/share/mingw-w64/include/string.h" 1 3
# 10 "/usr/share/mingw-w64/include/sec_api/string_s.h" 2 3
# 26 "/usr/share/mingw-w64/include/sec_api/string_s.h" 3
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _strset_s(char *_Dst,size_t _DstSize,int _Value);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _strerror_s(char *_Buf,size_t _SizeInBytes,const char *_ErrMsg);
  __attribute__((dllimport)) errno_t __attribute__((__cdecl__)) strerror_s(char *_Buf,size_t _SizeInBytes,int _ErrNum);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _strlwr_s(char *_Str,size_t _Size);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _strlwr_s_l(char *_Str,size_t _Size,_locale_t _Locale);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _strnset_s(char *_Str,size_t _Size,int _Val,size_t _MaxCount);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _strupr_s(char *_Str,size_t _Size);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _strupr_s_l(char *_Str,size_t _Size,_locale_t _Locale);

  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) strncat_s(char *_Dst,size_t _DstSizeInChars,const char *_Src,size_t _MaxCount);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _strncat_s_l(char *_Dst,size_t _DstSizeInChars,const char *_Src,size_t _MaxCount,_locale_t _Locale);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) strcpy_s(char *_Dst, rsize_t _SizeInBytes, const char *_Src);
 
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) strncpy_s(char *_Dst, size_t _DstSizeInChars, const char *_Src, size_t _MaxCount);
 
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _strncpy_s_l(char *_Dst, size_t _DstSizeInChars, const char *_Src, size_t _MaxCount, _locale_t _Locale);
  ;
  __attribute__ ((__dllimport__)) char *__attribute__((__cdecl__)) strtok_s(char *_Str,const char *_Delim,char **_Context);
  __attribute__ ((__dllimport__)) char *__attribute__((__cdecl__)) _strtok_s_l(char *_Str,const char *_Delim,char **_Context,_locale_t _Locale);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) strcat_s(char *_Dst, rsize_t _SizeInBytes, const char * _Src);
 

  __attribute__((dllimport)) errno_t __attribute__((__cdecl__)) memmove_s(void *_dest,size_t _numberOfElements,const void *_src,size_t _count);


  __attribute__ ((__dllimport__)) wchar_t *__attribute__((__cdecl__)) wcstok_s(wchar_t *_Str,const wchar_t *_Delim,wchar_t **_Context);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _wcserror_s(wchar_t *_Buf,size_t _SizeInWords,int _ErrNum);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) __wcserror_s(wchar_t *_Buffer,size_t _SizeInWords,const wchar_t *_ErrMsg);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _wcsnset_s(wchar_t *_Dst,size_t _DstSizeInWords,wchar_t _Val,size_t _MaxCount);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _wcsset_s(wchar_t *_Str,size_t _SizeInWords,wchar_t _Val);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _wcslwr_s(wchar_t *_Str,size_t _SizeInWords);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _wcslwr_s_l(wchar_t *_Str,size_t _SizeInWords,_locale_t _Locale);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _wcsupr_s(wchar_t *_Str,size_t _Size);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _wcsupr_s_l(wchar_t *_Str,size_t _Size,_locale_t _Locale);

  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) wcscpy_s(wchar_t *_Dst, rsize_t _SizeInWords, const wchar_t *_Src);
 
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) wcscat_s(wchar_t * _Dst, rsize_t _SizeInWords, const wchar_t *_Src);
 

  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) wcsncat_s(wchar_t *_Dst,size_t _DstSizeInChars,const wchar_t *_Src,size_t _MaxCount);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _wcsncat_s_l(wchar_t *_Dst,size_t _DstSizeInChars,const wchar_t *_Src,size_t _MaxCount,_locale_t _Locale);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) wcsncpy_s(wchar_t *_Dst, size_t _DstSizeInChars, const wchar_t *_Src, size_t _MaxCount);
  ;
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _wcsncpy_s_l(wchar_t *_Dst, size_t _DstSizeInChars, const wchar_t *_Src, size_t _MaxCount, _locale_t _Locale);
  ;
  __attribute__ ((__dllimport__)) wchar_t *__attribute__((__cdecl__)) _wcstok_s_l(wchar_t *_Str,const wchar_t *_Delim,wchar_t **_Context,_locale_t _Locale);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _wcsset_s_l(wchar_t *_Str,size_t _SizeInChars,unsigned int _Val,_locale_t _Locale);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _wcsnset_s_l(wchar_t *_Str,size_t _SizeInChars,unsigned int _Val, size_t _Count,_locale_t _Locale);

  extern __inline__ __attribute__((__always_inline__,__gnu_inline__)) size_t __attribute__((__cdecl__)) wcsnlen_s(const wchar_t * _src, size_t _count) {
    return _src ? wcsnlen(_src, _count) : 0;
  }
# 191 "/usr/share/mingw-w64/include/string.h" 2 3
# 11 "/usr/share/mingw-w64/include/io.h" 2 3
# 21 "/usr/share/mingw-w64/include/io.h" 3
#pragma pack(push,_CRT_PACKING)





__attribute__ ((__dllimport__)) char* __attribute__((__cdecl__)) _getcwd (char*, int);

  typedef unsigned long _fsize_t;





  struct _finddata32_t {
    unsigned attrib;
    __time32_t time_create;
    __time32_t time_access;
    __time32_t time_write;
    _fsize_t size;
    char name[260];
  };

  struct _finddata32i64_t {
    unsigned attrib;
    __time32_t time_create;
    __time32_t time_access;
    __time32_t time_write;
    __extension__ long long size;
    char name[260];
  };

  struct _finddata64i32_t {
    unsigned attrib;
    __time64_t time_create;
    __time64_t time_access;
    __time64_t time_write;
    _fsize_t size;
    char name[260];
  };

  struct __finddata64_t {
    unsigned attrib;
    __time64_t time_create;
    __time64_t time_access;
    __time64_t time_write;
    __extension__ long long size;
    char name[260];
  };
# 94 "/usr/share/mingw-w64/include/io.h" 3
  struct _wfinddata32_t {
    unsigned attrib;
    __time32_t time_create;
    __time32_t time_access;
    __time32_t time_write;
    _fsize_t size;
    wchar_t name[260];
  };

  struct _wfinddata32i64_t {
    unsigned attrib;
    __time32_t time_create;
    __time32_t time_access;
    __time32_t time_write;
    __extension__ long long size;
    wchar_t name[260];
  };

  struct _wfinddata64i32_t {
    unsigned attrib;
    __time64_t time_create;
    __time64_t time_access;
    __time64_t time_write;
    _fsize_t size;
    wchar_t name[260];
  };

  struct _wfinddata64_t {
    unsigned attrib;
    __time64_t time_create;
    __time64_t time_access;
    __time64_t time_write;
    __extension__ long long size;
    wchar_t name[260];
  };
# 187 "/usr/share/mingw-w64/include/io.h" 3
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _access(const char *_Filename,int _AccessMode);
  __attribute__((dllimport)) errno_t __attribute__((__cdecl__)) _access_s(const char *_Filename,int _AccessMode);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _chmod(const char *_Filename,int _Mode);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _chsize(int _FileHandle,long _Size) ;
  __attribute__((dllimport)) errno_t __attribute__((__cdecl__)) _chsize_s (int _FileHandle,long long _Size);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _close(int _FileHandle);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _commit(int _FileHandle);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _creat(const char *_Filename,int _PermissionMode) ;
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _dup(int _FileHandle);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _dup2(int _FileHandleSrc,int _FileHandleDst);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _eof(int _FileHandle);
  __attribute__ ((__dllimport__)) long __attribute__((__cdecl__)) _filelength(int _FileHandle);
  __attribute__ ((__dllimport__)) intptr_t __attribute__((__cdecl__)) _findfirst32(const char *_Filename,struct _finddata32_t *_FindData);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _findnext32(intptr_t _FindHandle,struct _finddata32_t *_FindData);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _findclose(intptr_t _FindHandle);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _isatty(int _FileHandle);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _locking(int _FileHandle,int _LockMode,long _NumOfBytes);
  __attribute__ ((__dllimport__)) long __attribute__((__cdecl__)) _lseek(int _FileHandle,long _Offset,int _Origin);
  _off64_t lseek64(int fd,_off64_t offset, int whence);
  __attribute__ ((__dllimport__)) char *__attribute__((__cdecl__)) _mktemp(char *_TemplateName) ;
  __attribute__((dllimport)) errno_t __attribute__((__cdecl__)) _mktemp_s (char *_TemplateName,size_t _Size);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _pipe(int *_PtHandles,unsigned int _PipeSize,int _TextMode);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _read(int _FileHandle,void *_DstBuf,unsigned int _MaxCharCount);
# 221 "/usr/share/mingw-w64/include/io.h" 3
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _setmode(int _FileHandle,int _Mode);
  __attribute__ ((__dllimport__)) long __attribute__((__cdecl__)) _tell(int _FileHandle);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _umask(int _Mode) ;
  __attribute__((dllimport)) errno_t __attribute__((__cdecl__)) _umask_s (int _NewMode,int *_OldMode);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _write(int _FileHandle,const void *_Buf,unsigned int _MaxCharCount);

  __extension__ __attribute__ ((__dllimport__)) long long __attribute__((__cdecl__)) _filelengthi64(int _FileHandle);
  __attribute__ ((__dllimport__)) intptr_t __attribute__((__cdecl__)) _findfirst32i64(const char *_Filename,struct _finddata32i64_t *_FindData);
  __attribute__ ((__dllimport__)) intptr_t __attribute__((__cdecl__)) _findfirst64(const char *_Filename,struct __finddata64_t *_FindData);



  intptr_t __attribute__((__cdecl__)) _findfirst64i32(const char *_Filename,struct _finddata64i32_t *_FindData);
# 252 "/usr/share/mingw-w64/include/io.h" 3
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _findnext32i64(intptr_t _FindHandle,struct _finddata32i64_t *_FindData);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _findnext64(intptr_t _FindHandle,struct __finddata64_t *_FindData);
  int __attribute__((__cdecl__)) _findnext64i32(intptr_t _FindHandle,struct _finddata64i32_t *_FindData);
# 273 "/usr/share/mingw-w64/include/io.h" 3
  __extension__ long long __attribute__((__cdecl__)) _lseeki64(int _FileHandle,long long _Offset,int _Origin);
  __extension__ long long __attribute__((__cdecl__)) _telli64(int _FileHandle);



  int __attribute__((__cdecl__)) chdir (const char *) ;
  char *__attribute__((__cdecl__)) getcwd (char *, int) ;
  int __attribute__((__cdecl__)) mkdir (const char *) ;
  char *__attribute__((__cdecl__)) mktemp(char *) ;
  int __attribute__((__cdecl__)) rmdir (const char*) ;
  int __attribute__((__cdecl__)) chmod (const char *, int) ;



  __attribute__((dllimport)) errno_t __attribute__((__cdecl__)) _sopen_s(int *_FileHandle,const char *_Filename,int _OpenFlag,int _ShareFlag,int _PermissionMode);

  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _open(const char *_Filename,int _OpenFlag,...) ;
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _sopen(const char *_Filename,int _OpenFlag,int _ShareFlag,...) ;



  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _waccess(const wchar_t *_Filename,int _AccessMode);
  __attribute__((dllimport)) errno_t __attribute__((__cdecl__)) _waccess_s (const wchar_t *_Filename,int _AccessMode);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wchmod(const wchar_t *_Filename,int _Mode);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wcreat(const wchar_t *_Filename,int _PermissionMode) ;
  __attribute__ ((__dllimport__)) intptr_t __attribute__((__cdecl__)) _wfindfirst32(const wchar_t *_Filename,struct _wfinddata32_t *_FindData);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wfindnext32(intptr_t _FindHandle,struct _wfinddata32_t *_FindData);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wunlink(const wchar_t *_Filename);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wrename(const wchar_t *_OldFilename,const wchar_t *_NewFilename);
  __attribute__ ((__dllimport__)) wchar_t *__attribute__((__cdecl__)) _wmktemp(wchar_t *_TemplateName) ;
  __attribute__((dllimport)) errno_t __attribute__((__cdecl__)) _wmktemp_s (wchar_t *_TemplateName, size_t _SizeInWords);

  __attribute__ ((__dllimport__)) intptr_t __attribute__((__cdecl__)) _wfindfirst32i64(const wchar_t *_Filename,struct _wfinddata32i64_t *_FindData);
  intptr_t __attribute__((__cdecl__)) _wfindfirst64i32(const wchar_t *_Filename,struct _wfinddata64i32_t *_FindData);
  __attribute__ ((__dllimport__)) intptr_t __attribute__((__cdecl__)) _wfindfirst64(const wchar_t *_Filename,struct _wfinddata64_t *_FindData);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wfindnext32i64(intptr_t _FindHandle,struct _wfinddata32i64_t *_FindData);
  int __attribute__((__cdecl__)) _wfindnext64i32(intptr_t _FindHandle,struct _wfinddata64i32_t *_FindData);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wfindnext64(intptr_t _FindHandle,struct _wfinddata64_t *_FindData);

  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _wsopen_s(int *_FileHandle,const wchar_t *_Filename,int _OpenFlag,int _ShareFlag,int _PermissionFlag);

  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wopen(const wchar_t *_Filename,int _OpenFlag,...) ;
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wsopen(const wchar_t *_Filename,int _OpenFlag,int _ShareFlag,...) ;



  int __attribute__((__cdecl__)) __lock_fhandle(int _Filehandle);
  void __attribute__((__cdecl__)) _unlock_fhandle(int _Filehandle);
  __attribute__ ((__dllimport__)) intptr_t __attribute__((__cdecl__)) _get_osfhandle(int _FileHandle);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _open_osfhandle(intptr_t _OSFileHandle,int _Flags);


  int __attribute__((__cdecl__)) access(const char *_Filename,int _AccessMode) ;
  int __attribute__((__cdecl__)) chmod(const char *_Filename,int _AccessMode) ;
  int __attribute__((__cdecl__)) chsize(int _FileHandle,long _Size) ;
  int __attribute__((__cdecl__)) close(int _FileHandle) ;
  int __attribute__((__cdecl__)) creat(const char *_Filename,int _PermissionMode) ;
  int __attribute__((__cdecl__)) dup(int _FileHandle) ;
  int __attribute__((__cdecl__)) dup2(int _FileHandleSrc,int _FileHandleDst) ;
  int __attribute__((__cdecl__)) eof(int _FileHandle) ;
  long __attribute__((__cdecl__)) filelength(int _FileHandle) ;
  int __attribute__((__cdecl__)) isatty(int _FileHandle) ;
  int __attribute__((__cdecl__)) locking(int _FileHandle,int _LockMode,long _NumOfBytes) ;
  long __attribute__((__cdecl__)) lseek(int _FileHandle,long _Offset,int _Origin) ;
  char *__attribute__((__cdecl__)) mktemp(char *_TemplateName) ;
  int __attribute__((__cdecl__)) open(const char *_Filename,int _OpenFlag,...) ;
  int __attribute__((__cdecl__)) read(int _FileHandle,void *_DstBuf,unsigned int _MaxCharCount) ;
  int __attribute__((__cdecl__)) setmode(int _FileHandle,int _Mode) ;
  int __attribute__((__cdecl__)) sopen(const char *_Filename,int _OpenFlag,int _ShareFlag,...) ;
  long __attribute__((__cdecl__)) tell(int _FileHandle) ;
  int __attribute__((__cdecl__)) umask(int _Mode) ;
  int __attribute__((__cdecl__)) write(int _Filehandle,const void *_Buf,unsigned int _MaxCharCount) ;
# 381 "/usr/share/mingw-w64/include/io.h" 3
#pragma pack(pop)
# 11 "/usr/share/mingw-w64/include/unistd.h" 2 3
# 1 "/usr/share/mingw-w64/include/process.h" 1 3
# 10 "/usr/share/mingw-w64/include/process.h" 3
# 1 "/usr/share/mingw-w64/include/corecrt_startup.h" 1 3
# 14 "/usr/share/mingw-w64/include/corecrt_startup.h" 3
__attribute__ ((__dllimport__)) char **__attribute__((__cdecl__)) __p__acmdln(void);


__attribute__ ((__dllimport__)) wchar_t **__attribute__((__cdecl__)) __p__wcmdln(void);


typedef void (__attribute__((__cdecl__)) *_PVFV)(void);
typedef int (__attribute__((__cdecl__)) *_PIFV)(void);
typedef void (__attribute__((__cdecl__)) *_PVFI)(int);

typedef struct _onexit_table_t {
    _PVFV* _first;
    _PVFV* _last;
    _PVFV* _end;
} _onexit_table_t;

typedef int (__attribute__((__cdecl__)) *_onexit_t)(void);

__attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _initialize_onexit_table(_onexit_table_t*);
__attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _register_onexit_function(_onexit_table_t*,_onexit_t);
__attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _execute_onexit_table(_onexit_table_t*);
# 11 "/usr/share/mingw-w64/include/process.h" 2 3


# 1 "/usr/share/mingw-w64/include/sys/types.h" 1 3
# 43 "/usr/share/mingw-w64/include/sys/types.h" 3
typedef unsigned short _ino_t;

typedef unsigned short ino_t;





typedef unsigned int _dev_t;

typedef unsigned int dev_t;
# 62 "/usr/share/mingw-w64/include/sys/types.h" 3
__extension__
typedef long long _pid_t;




typedef _pid_t pid_t;





typedef unsigned short _mode_t;


typedef _mode_t mode_t;






typedef unsigned int useconds_t;




struct timespec {
  time_t tv_sec;
  long tv_nsec;
};

struct itimerspec {
  struct timespec it_interval;
  struct timespec it_value;
};





__extension__
typedef unsigned long long _sigset_t;
# 14 "/usr/share/mingw-w64/include/process.h" 2 3
# 32 "/usr/share/mingw-w64/include/process.h" 3
  __attribute__ ((__dllimport__)) uintptr_t __attribute__((__cdecl__)) _beginthread(void (__attribute__((__cdecl__)) *_StartAddress) (void *),unsigned _StackSize,void *_ArgList);
  __attribute__ ((__dllimport__)) void __attribute__((__cdecl__)) _endthread(void) __attribute__ ((__noreturn__));
  __attribute__ ((__dllimport__)) uintptr_t __attribute__((__cdecl__)) _beginthreadex(void *_Security,unsigned _StackSize,unsigned ( *_StartAddress) (void *),void *_ArgList,unsigned _InitFlag,unsigned *_ThrdAddr);
  __attribute__ ((__dllimport__)) void __attribute__((__cdecl__)) _endthreadex(unsigned _Retval) __attribute__ ((__noreturn__));



  void __attribute__((__cdecl__)) __attribute__ ((__nothrow__)) exit(int _Code) __attribute__ ((__noreturn__));
  void __attribute__((__cdecl__)) __attribute__ ((__nothrow__)) _exit(int _Code) __attribute__ ((__noreturn__));



  void __attribute__((__cdecl__)) _Exit(int) __attribute__ ((__noreturn__));






       

  void __attribute__((__cdecl__)) __attribute__ ((__noreturn__)) abort(void);
       



  typedef void ( *_tls_callback_type)(void*,unsigned long,void*);
  __attribute__ ((__dllimport__)) void __attribute__((__cdecl__)) _register_thread_local_exe_atexit_callback(_tls_callback_type callback);

  void __attribute__((__cdecl__)) __attribute__ ((__nothrow__)) _cexit(void);
  void __attribute__((__cdecl__)) __attribute__ ((__nothrow__)) _c_exit(void);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _getpid(void);
  __attribute__ ((__dllimport__)) intptr_t __attribute__((__cdecl__)) _cwait(int *_TermStat,intptr_t _ProcHandle,int _Action);
  __attribute__ ((__dllimport__)) intptr_t __attribute__((__cdecl__)) _execl(const char *_Filename,const char *_ArgList,...);
  __attribute__ ((__dllimport__)) intptr_t __attribute__((__cdecl__)) _execle(const char *_Filename,const char *_ArgList,...);
  __attribute__ ((__dllimport__)) intptr_t __attribute__((__cdecl__)) _execlp(const char *_Filename,const char *_ArgList,...);
  __attribute__ ((__dllimport__)) intptr_t __attribute__((__cdecl__)) _execlpe(const char *_Filename,const char *_ArgList,...);
  __attribute__ ((__dllimport__)) intptr_t __attribute__((__cdecl__)) _execv(const char *_Filename,const char *const *_ArgList);
  __attribute__ ((__dllimport__)) intptr_t __attribute__((__cdecl__)) _execve(const char *_Filename,const char *const *_ArgList,const char *const *_Env);
  __attribute__ ((__dllimport__)) intptr_t __attribute__((__cdecl__)) _execvp(const char *_Filename,const char *const *_ArgList);
  __attribute__ ((__dllimport__)) intptr_t __attribute__((__cdecl__)) _execvpe(const char *_Filename,const char *const *_ArgList,const char *const *_Env);
  __attribute__ ((__dllimport__)) intptr_t __attribute__((__cdecl__)) _spawnl(int _Mode,const char *_Filename,const char *_ArgList,...);
  __attribute__ ((__dllimport__)) intptr_t __attribute__((__cdecl__)) _spawnle(int _Mode,const char *_Filename,const char *_ArgList,...);
  __attribute__ ((__dllimport__)) intptr_t __attribute__((__cdecl__)) _spawnlp(int _Mode,const char *_Filename,const char *_ArgList,...);
  __attribute__ ((__dllimport__)) intptr_t __attribute__((__cdecl__)) _spawnlpe(int _Mode,const char *_Filename,const char *_ArgList,...);
# 88 "/usr/share/mingw-w64/include/process.h" 3
  int __attribute__((__cdecl__)) system(const char *_Command);




  __attribute__ ((__dllimport__)) intptr_t __attribute__((__cdecl__)) _wexecl(const wchar_t *_Filename,const wchar_t *_ArgList,...);
  __attribute__ ((__dllimport__)) intptr_t __attribute__((__cdecl__)) _wexecle(const wchar_t *_Filename,const wchar_t *_ArgList,...);
  __attribute__ ((__dllimport__)) intptr_t __attribute__((__cdecl__)) _wexeclp(const wchar_t *_Filename,const wchar_t *_ArgList,...);
  __attribute__ ((__dllimport__)) intptr_t __attribute__((__cdecl__)) _wexeclpe(const wchar_t *_Filename,const wchar_t *_ArgList,...);
  __attribute__ ((__dllimport__)) intptr_t __attribute__((__cdecl__)) _wexecv(const wchar_t *_Filename,const wchar_t *const *_ArgList);
  __attribute__ ((__dllimport__)) intptr_t __attribute__((__cdecl__)) _wexecve(const wchar_t *_Filename,const wchar_t *const *_ArgList,const wchar_t *const *_Env);
  __attribute__ ((__dllimport__)) intptr_t __attribute__((__cdecl__)) _wexecvp(const wchar_t *_Filename,const wchar_t *const *_ArgList);
  __attribute__ ((__dllimport__)) intptr_t __attribute__((__cdecl__)) _wexecvpe(const wchar_t *_Filename,const wchar_t *const *_ArgList,const wchar_t *const *_Env);
# 117 "/usr/share/mingw-w64/include/process.h" 3
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wsystem(const wchar_t *_Command);


  void __attribute__((__cdecl__)) __security_init_cookie(void);




  void __attribute__((__cdecl__)) __security_check_cookie(uintptr_t _StackCookie);
  __attribute__ ((__noreturn__)) void __attribute__((__cdecl__)) __report_gsfailure(uintptr_t _StackCookie);

  extern uintptr_t __security_cookie;

  intptr_t __attribute__((__cdecl__)) _loaddll(char *_Filename);
  int __attribute__((__cdecl__)) _unloaddll(intptr_t _Handle);
  int (__attribute__((__cdecl__)) *__attribute__((__cdecl__)) _getdllprocaddr(intptr_t _Handle,char *_ProcedureName,intptr_t _Ordinal))(void);
# 158 "/usr/share/mingw-w64/include/process.h" 3
  intptr_t __attribute__((__cdecl__)) cwait(int *_TermStat,intptr_t _ProcHandle,int _Action) ;

  int __attribute__((__cdecl__)) execl(const char *_Filename,const char *_ArgList,...) ;
  int __attribute__((__cdecl__)) execle(const char *_Filename,const char *_ArgList,...) ;
  int __attribute__((__cdecl__)) execlp(const char *_Filename,const char *_ArgList,...) ;
  int __attribute__((__cdecl__)) execlpe(const char *_Filename,const char *_ArgList,...) ;






  intptr_t __attribute__((__cdecl__)) spawnl(int,const char *_Filename,const char *_ArgList,...) ;
  intptr_t __attribute__((__cdecl__)) spawnle(int,const char *_Filename,const char *_ArgList,...) ;
  intptr_t __attribute__((__cdecl__)) spawnlp(int,const char *_Filename,const char *_ArgList,...) ;
  intptr_t __attribute__((__cdecl__)) spawnlpe(int,const char *_Filename,const char *_ArgList,...) ;


  int __attribute__((__cdecl__)) getpid(void) ;






  int __attribute__((__cdecl__)) execv(const char *_Filename,char *const _ArgList[]) ;
  int __attribute__((__cdecl__)) execve(const char *_Filename,char *const _ArgList[],char *const _Env[]) ;
  int __attribute__((__cdecl__)) execvp(const char *_Filename,char *const _ArgList[]) ;
  int __attribute__((__cdecl__)) execvpe(const char *_Filename,char *const _ArgList[],char *const _Env[]) ;






  intptr_t __attribute__((__cdecl__)) spawnv(int,const char *_Filename,char *const _ArgList[]) ;
  intptr_t __attribute__((__cdecl__)) spawnve(int,const char *_Filename,char *const _ArgList[],char *const _Env[]) ;
  intptr_t __attribute__((__cdecl__)) spawnvp(int,const char *_Filename,char *const _ArgList[]) ;
  intptr_t __attribute__((__cdecl__)) spawnvpe(int,const char *_Filename,char *const _ArgList[],char *const _Env[]) ;
# 12 "/usr/share/mingw-w64/include/unistd.h" 2 3
# 1 "/usr/share/mingw-w64/include/getopt.h" 1 3
# 22 "/usr/share/mingw-w64/include/getopt.h" 3
extern int optind;
extern int optopt;
extern int opterr;


extern char *optarg;

extern int getopt(int nargc, char * const *nargv, const char *options);
# 13 "/usr/share/mingw-w64/include/unistd.h" 2 3
# 44 "/usr/share/mingw-w64/include/unistd.h" 3
       

unsigned int __attribute__((__cdecl__)) sleep (unsigned int);
       




int __attribute__((__cdecl__)) __attribute__ ((__nothrow__)) usleep(useconds_t);







int ftruncate(int, off32_t);
int ftruncate64(int, off64_t);
int truncate(const char *, off32_t);
int truncate64(const char *, off64_t);
# 93 "/usr/share/mingw-w64/include/unistd.h" 3
  void __attribute__((__cdecl__)) swab(char *_Buf1,char *_Buf2,int _SizeInBytes) ;
# 105 "/usr/share/mingw-w64/include/unistd.h" 3
# 1 "/usr/share/mingw-w64/include/pthread_unistd.h" 1 3
# 106 "/usr/share/mingw-w64/include/unistd.h" 2 3
# 28 "test/testcjdroute.c" 2
# 1 "/usr/share/mingw-w64/include/fcntl.h" 1 3
# 29 "test/testcjdroute.c" 2








# 36 "test/testcjdroute.c"




int Beacon_test_main(int argc, char** argv); int CryptoAddress_test_main(int argc, char** argv); void* Main_fuzz_test_FUZZ_INIT(struct Allocator* alloc, struct Random* rand); void Main_fuzz_test_FUZZ_MAIN(void* ctx, struct Message* fuzz); int printIp_test_main(int argc, char** argv); void* CryptoAuth_fuzz_test_FUZZ_INIT(struct Allocator* alloc, struct Random* rand); void CryptoAuth_fuzz_test_FUZZ_MAIN(void* ctx, struct Message* fuzz); int CryptoAuth_test_main(int argc, char** argv); int CryptoAuth_unit_test_main(int argc, char** argv); int ReplayProtector_test_main(int argc, char** argv); int Sign_test_main(int argc, char** argv); int DHTModules_handleIncoming_test_main(int argc, char** argv); int DHTModules_handleOutgoing_test_main(int argc, char** argv); void* FramingIface_fuzz_test_FUZZ_INIT(struct Allocator* alloc, struct Random* rand); void FramingIface_fuzz_test_FUZZ_MAIN(void* ctx, struct Message* fuzz); int FileReader_test_main(int argc, char** argv); int Allocator_test_main(int argc, char** argv); int LinkState_test_main(int argc, char** argv); int EncodingScheme_test_main(int argc, char** argv); int LabelSplicer_test_main(int argc, char** argv); int NumberCompress_test_main(int argc, char** argv); int IpTunnel_test_main(int argc, char** argv); int RouteGen_test_main(int argc, char** argv); int AddrTools_test_main(int argc, char** argv); int AverageRoller_test_main(int argc, char** argv); int Base10_test_main(int argc, char** argv); int Base32_test_main(int argc, char** argv); int Bits_test_main(int argc, char** argv); int Checksum_test_main(int argc, char** argv); int Endian_test_main(int argc, char** argv); int Hex_test_main(int argc, char** argv); int Identity_test_main(int argc, char** argv); void* Map_fuzz_test_FUZZ_INIT(struct Allocator* alloc, struct Random* rand); void Map_fuzz_test_FUZZ_MAIN(void* ctx, struct Message* fuzz); int Map_test_main(int argc, char** argv); int Process_test_main(int argc, char** argv); int QSort_test_main(int argc, char** argv); int Seccomp_test_main(int argc, char** argv); int Set_test_main(int argc, char** argv); int UniqueName_test_main(int argc, char** argv); int VarInt_test_main(int argc, char** argv); int Random_test_main(int argc, char** argv); int NodeStore_test_main(int argc, char** argv); int VersionList_test_main(int argc, char** argv); int BSDMessageTypeWrapper_test_main(int argc, char** argv); int TAPWrapper_root_test_main(int argc, char** argv); int TUNInterface_ipv4_root_test_main(int argc, char** argv); int TUNInterface_ipv6_root_test_main(int argc, char** argv); int TUNInterface_ipv6_withroute_root_test_main(int argc, char** argv); int Sockaddr_test_main(int argc, char** argv); void* JsonBencMessageReader_fuzz_test_FUZZ_INIT(struct Allocator* alloc, struct Random* rand); void JsonBencMessageReader_fuzz_test_FUZZ_MAIN(void* ctx, struct Message* fuzz); int TAPDevice_root_test_main(int argc, char** argv); int TAPInterface_root_test_main(int argc, char** argv);

typedef int (* Test)(int argc, char** argv);
typedef void* (* FuzzTestInit)(struct Allocator* alloc, struct Random* rand);
typedef void (* FuzzTest)(void* ctx, struct Message* fuzz);
typedef struct FuzzTest* (* MkFuzz)(struct Allocator* alloc);

static const struct {
    Test func;
    char* name;
} TESTS[] = { { .func = Beacon_test_main, .name = "Beacon_test" }, { .func = CryptoAddress_test_main, .name = "CryptoAddress_test" }, { .func = printIp_test_main, .name = "printIp_test" }, { .func = CryptoAuth_test_main, .name = "CryptoAuth_test" }, { .func = CryptoAuth_unit_test_main, .name = "CryptoAuth_unit_test" }, { .func = ReplayProtector_test_main, .name = "ReplayProtector_test" }, { .func = Sign_test_main, .name = "Sign_test" }, { .func = DHTModules_handleIncoming_test_main, .name = "DHTModules_handleIncoming_test" }, { .func = DHTModules_handleOutgoing_test_main, .name = "DHTModules_handleOutgoing_test" }, { .func = FileReader_test_main, .name = "FileReader_test" }, { .func = Allocator_test_main, .name = "Allocator_test" }, { .func = LinkState_test_main, .name = "LinkState_test" }, { .func = EncodingScheme_test_main, .name = "EncodingScheme_test" }, { .func = LabelSplicer_test_main, .name = "LabelSplicer_test" }, { .func = NumberCompress_test_main, .name = "NumberCompress_test" }, { .func = IpTunnel_test_main, .name = "IpTunnel_test" }, { .func = RouteGen_test_main, .name = "RouteGen_test" }, { .func = AddrTools_test_main, .name = "AddrTools_test" }, { .func = AverageRoller_test_main, .name = "AverageRoller_test" }, { .func = Base10_test_main, .name = "Base10_test" }, { .func = Base32_test_main, .name = "Base32_test" }, { .func = Bits_test_main, .name = "Bits_test" }, { .func = Checksum_test_main, .name = "Checksum_test" }, { .func = Endian_test_main, .name = "Endian_test" }, { .func = Hex_test_main, .name = "Hex_test" }, { .func = Identity_test_main, .name = "Identity_test" }, { .func = Map_test_main, .name = "Map_test" }, { .func = Process_test_main, .name = "Process_test" }, { .func = QSort_test_main, .name = "QSort_test" }, { .func = Seccomp_test_main, .name = "Seccomp_test" }, { .func = Set_test_main, .name = "Set_test" }, { .func = UniqueName_test_main, .name = "UniqueName_test" }, { .func = VarInt_test_main, .name = "VarInt_test" }, { .func = Random_test_main, .name = "Random_test" }, { .func = NodeStore_test_main, .name = "NodeStore_test" }, { .func = VersionList_test_main, .name = "VersionList_test" }, { .func = BSDMessageTypeWrapper_test_main, .name = "BSDMessageTypeWrapper_test" }, { .func = TAPWrapper_root_test_main, .name = "TAPWrapper_root_test" }, { .func = TUNInterface_ipv4_root_test_main, .name = "TUNInterface_ipv4_root_test" }, { .func = TUNInterface_ipv6_root_test_main, .name = "TUNInterface_ipv6_root_test" }, { .func = TUNInterface_ipv6_withroute_root_test_main, .name = "TUNInterface_ipv6_withroute_root_test" }, { .func = Sockaddr_test_main, .name = "Sockaddr_test" }, { .func = TAPDevice_root_test_main, .name = "TAPDevice_root_test" }, { .func = TAPInterface_root_test_main, .name = "TAPInterface_root_test" }, };
static const int TEST_COUNT = (int) (sizeof(TESTS) / sizeof(*TESTS));

static const struct {
    FuzzTestInit init;
    FuzzTest fuzz;
    char* name;
} FUZZ_TESTS[] = { {.init = Main_fuzz_test_FUZZ_INIT, .name = "Main_fuzz_test", .fuzz = Main_fuzz_test_FUZZ_MAIN, }, {.init = CryptoAuth_fuzz_test_FUZZ_INIT, .name = "CryptoAuth_fuzz_test", .fuzz = CryptoAuth_fuzz_test_FUZZ_MAIN, }, {.init = FramingIface_fuzz_test_FUZZ_INIT, .name = "FramingIface_fuzz_test", .fuzz = FramingIface_fuzz_test_FUZZ_MAIN, }, {.init = Map_fuzz_test_FUZZ_INIT, .name = "Map_fuzz_test", .fuzz = Map_fuzz_test_FUZZ_MAIN, }, {.init = JsonBencMessageReader_fuzz_test_FUZZ_INIT, .name = "JsonBencMessageReader_fuzz_test", .fuzz = JsonBencMessageReader_fuzz_test_FUZZ_MAIN, }, };
static const int FUZZ_TEST_COUNT = (int) (sizeof(FUZZ_TESTS) / sizeof(*FUZZ_TESTS));

static const char* FUZZ_CASES[] = { "build_win64/fuzz_inputs/__test_Main_fuzz_test_cCtrlAddrErr", "build_win64/fuzz_inputs/__crypto_test_CryptoAuth_fuzz_test_cDefault", "build_win64/fuzz_inputs/__interface_test_FramingIface_fuzz_test_cDefault", "build_win64/fuzz_inputs/__util_test_Map_fuzz_test_cDefault", "build_win64/fuzz_inputs/__benc_serialization_json_test_JsonBencMessageReader_fuzz_test_cConfFile", "build_win64/fuzz_inputs/__test_Main_fuzz_test_cCtrlAuthErr", "build_win64/fuzz_inputs/__test_Main_fuzz_test_cCtrlPing", "build_win64/fuzz_inputs/__test_Main_fuzz_test_cCtrlPong", "build_win64/fuzz_inputs/__test_Main_fuzz_test_cCtrlUndeliverable", "build_win64/fuzz_inputs/__test_Main_fuzz_test_cDhtFindNodeQuery", "build_win64/fuzz_inputs/__test_Main_fuzz_test_cDhtGetPeersQuery", "build_win64/fuzz_inputs/__test_Main_fuzz_test_cDhtPingQuery", };
static const int FUZZ_CASE_COUNT = (int) (sizeof(FUZZ_CASES) / sizeof(*FUZZ_CASES));


static uint64_t runTest(Test test,
                        char* name,
                        uint64_t startTime,
                        int argc,
                        char** argv,
                        int quiet)
{
    if (!quiet) { fprintf(
# 71 "test/testcjdroute.c" 3
                         (__acrt_iob_func(2))
# 71 "test/testcjdroute.c"
                               , "Running test %s", name); }
    do { if (!((!test(argc, argv)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("testcjdroute.c"), (72), "(!test(argc, argv))"); } } while (0);
    if (!quiet) {
        uint64_t now = Time_hrtime();
        char* seventySpaces =
            "                                                                      ";
        int count = CString_strlen(name);
        if (count > 69) { count = 69; }
        fprintf(
# 79 "test/testcjdroute.c" 3
               (__acrt_iob_func(2))
# 79 "test/testcjdroute.c"
                     , "%s%d.%d ms\n",
                &seventySpaces[count],
                (int)((now - startTime)/1000000),
                (int)((now - startTime)/1000)%1000);
        return now;
    }
    return startTime;
}

static void usage(char* appName)
{
    printf("%s <test>     run one test\n", appName);
    printf("%s all        run every test\n\n", appName);
    printf("Flags:\n");
    printf("  --quiet                # Don't write test timings to stderr");
    printf("  --stderr-to <path>     # Redirect stderr to a file (append mode)");
    printf("  --inittests            # When using `fuzz` first initialize ALL tests");
    printf("\n");
    printf("Available Tests:\n");
    for (int i = 0; i < TEST_COUNT; i++) {
        printf("%s %s\n", appName, TESTS[i].name);
    }
    printf("\nAvailable Fuzz Tests:\n");
    for (int i = 0; i < FUZZ_CASE_COUNT; i++) {
        printf("%s fuzz < %s\n", appName, FUZZ_CASES[i]);
    }
}

static void readFile(int fileNo, struct Allocator* alloc, struct Message* fuzz)
{
    ssize_t length = read(fileNo, fuzz->bytes, fuzz->length);
    if (length >= fuzz->length) {
        printf("No test files over [%d] bytes\n", fuzz->length);
        length = 0;
    }
    fuzz->length = length;
}

static void** initFuzzTests(struct Allocator* alloc, struct Random* rand)
{
    void** contexts = Allocator__calloc((alloc),(sizeof(char*)),(FUZZ_TEST_COUNT),"testcjdroute.c",119);
    for (int i = 0; i < FUZZ_TEST_COUNT; i++) {
        contexts[i] = FUZZ_TESTS[i].init(alloc, rand);
    }
    return contexts;
}

static int runFuzzTest(
    void** ctxs,
    struct Allocator* alloc,
    struct Random* rand,
    struct Message* fuzz,
    const char* testCase,
    int quiet)
{
    if (fuzz->length < 4) { return 100; }
    uint32_t selector = (__extension__({             uint32_t Er_returnVal;             __builtin_memset(&Er_returnVal, 0, sizeof(Er_returnVal));             struct Er_Ret* Er_ret = Message_epop32be(&Er_returnVal, fuzz);                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("testcjdroute.c", 135, Er_ret);                 } else {                     Er__assertFail(Er_unwind("testcjdroute.c", 135, Er_ret));                 }             }                      Er_returnVal;         }));
    if (selector >= (uint32_t)FUZZ_TEST_COUNT) {
        printf("selector [%x] out of bounds [%u]\n", selector, FUZZ_TEST_COUNT);
        return 100;
    }
    if (!testCase) { testCase = FUZZ_TESTS[selector].name; }
    if (!quiet) { fprintf(
# 141 "test/testcjdroute.c" 3
                         (__acrt_iob_func(2))
# 141 "test/testcjdroute.c"
                               , "Running fuzz %s", testCase); }
    void* ctx = ctxs ? ctxs[selector] : FUZZ_TESTS[selector].init(alloc, rand);
    FUZZ_TESTS[selector].fuzz(ctx, fuzz);
    return 0;
}

static uint64_t runFuzzTestManual(
    struct Allocator* alloc,
    struct Random* detRand,
    const char* testCase,
    uint64_t startTime,
    int quiet)
{
    int f = open(testCase, 
# 154 "test/testcjdroute.c" 3
                          0x0000
# 154 "test/testcjdroute.c"
                                  );
    do { if (!((f > -1))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("testcjdroute.c"), (155), "(f > -1)"); } } while (0);
    struct Message* fuzz = Message_new(4096, 128, alloc);
    readFile(f, alloc, fuzz);
    close(f);

    runFuzzTest(
# 160 "test/testcjdroute.c" 3 4
               ((void *)0)
# 160 "test/testcjdroute.c"
                   , alloc, detRand, fuzz, testCase, quiet);

    if (!quiet) {
        uint64_t now = Time_hrtime();
        char* seventySpaces =
            "                                                                      ";
        int count = CString_strlen(testCase);
        if (count > 69) { count = 69; }
        fprintf(
# 168 "test/testcjdroute.c" 3
               (__acrt_iob_func(2))
# 168 "test/testcjdroute.c"
                     , "%s%d.%d ms\n",
                &seventySpaces[count],
                (int)((now - startTime)/1000000),
                (int)((now - startTime)/1000)%1000);
        return now;
    }
    return startTime;
}
# 188 "test/testcjdroute.c"
static int fuzzMain(struct Allocator* alloc, struct Random* detRand, int initTests, int quiet)
{
    struct Message* fuzz = Message_new(4096, 128, alloc);






    void** ctxs = (initTests) ? initFuzzTests(alloc, detRand) : 
# 197 "test/testcjdroute.c" 3 4
                                                               ((void *)0)
# 197 "test/testcjdroute.c"
                                                                   ;





    readFile(
# 203 "test/testcjdroute.c" 3
            0
# 203 "test/testcjdroute.c"
                        , alloc, fuzz);
    int out = runFuzzTest(ctxs, alloc, detRand, fuzz, 
# 204 "test/testcjdroute.c" 3 4
                                                     ((void *)0)
# 204 "test/testcjdroute.c"
                                                         , quiet);
    printf("\n");
    return out;
}

static void stderrTo(char* file)
{
    int f = open(file, 
# 211 "test/testcjdroute.c" 3
                      0x0100 
# 211 "test/testcjdroute.c"
                              | 
# 211 "test/testcjdroute.c" 3
                                0x0008 
# 211 "test/testcjdroute.c"
                                         | 
# 211 "test/testcjdroute.c" 3
                                           0x0001
# 211 "test/testcjdroute.c"
                                                   , 0666);
    do { if (!((f > -1))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("testcjdroute.c"), (212), "(f > -1)"); } } while (0);
    do { if (!((dup2(f, 
# 213 "test/testcjdroute.c" 3
   2
# 213 "test/testcjdroute.c"
   ) == 
# 213 "test/testcjdroute.c" 3
   2
# 213 "test/testcjdroute.c"
   ))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("testcjdroute.c"), (213), "(dup2(f, 2) == 2)"); } } while (0);
}

static int main2(int argc, char** argv, struct Allocator* alloc, struct Random* detRand)
{
    int initTests = 0;
    int quiet = 0;
    for (int i = 0; i < argc; i++) {
        if (!CString_strcmp("--inittests", argv[i])) { initTests = 1; }
        if (!CString_strcmp("--stderr-to", argv[i]) && argc > i + 1) { stderrTo(argv[i + 1]); }
        if (!CString_strcmp("--quiet", argv[i])) { quiet = 1; }
    }
    if (argc > 1 && !CString_strcmp("fuzz", argv[1])) {
        return fuzzMain(alloc, detRand, initTests, quiet);
    }
    uint64_t now = Time_hrtime();
    uint64_t startTime = now;
    if (argc < 2) {
        do { if (!((argc > 0))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("testcjdroute.c"), (231), "(argc > 0)"); } } while (0);
        usage(argv[0]);
        return 100;
    }
    if (argc > 1 && CString_strcmp("all", argv[1])) {
        for (int i = 0; i < TEST_COUNT; i++) {
            if (!CString_strcmp(TESTS[i].name, argv[1])) {
                TESTS[i].func(argc, argv);
                return 0;
            }
        }
        for (int i = 0; i < FUZZ_CASE_COUNT; i++) {
            if (!CString_strcmp(FUZZ_CASES[i], argv[1])) {
                runFuzzTestManual(alloc, detRand, FUZZ_CASES[i], now, quiet);
                return 0;
            }
        }
        usage(argv[0]);
        return 100;
    }
    for (int i = 0; i < TEST_COUNT; i++) {
        now = runTest(TESTS[i].func, TESTS[i].name, now, argc, argv, quiet);
    }
    for (int i = 0; i < FUZZ_CASE_COUNT; i++) {





        struct Allocator* child = MallocAllocator__new((1<<24),"testcjdroute.c",260);

        now = runFuzzTestManual(child, detRand, FUZZ_CASES[i], now, quiet);
        Allocator__free((child),"testcjdroute.c",263);
    }
    if (!quiet) {
        fprintf(
# 266 "test/testcjdroute.c" 3
               (__acrt_iob_func(2))
# 266 "test/testcjdroute.c"
                     , "Total test time %d.%d ms\n",
                (int)((now - startTime)/1000000),
                (int)((now - startTime)/1000)%1000);
    }
    return 0;
}

int main(int argc, char** argv)
{
    struct Allocator* alloc = MallocAllocator__new((1<<24),"testcjdroute.c",275);
    struct RandomSeed* rs = DeterminentRandomSeed_new(alloc, "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef");
    struct Random* detRand = Random_newWithSeed(alloc, 
# 277 "test/testcjdroute.c" 3 4
                                                      ((void *)0)
# 277 "test/testcjdroute.c"
                                                          , rs, 
# 277 "test/testcjdroute.c" 3 4
                                                                ((void *)0)
# 277 "test/testcjdroute.c"
                                                                    );
    int out = main2(argc, argv, alloc, detRand);
    Allocator__free((alloc),"testcjdroute.c",279);
    return out;
}
