# 1 "subnode/SupernodeHunter.c"
# 1 "/home/jojapoppa/cjdns//"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "subnode/SupernodeHunter.c"
# 15 "subnode/SupernodeHunter.c"
# 1 "./crypto/AddressCalc.h" 1
# 18 "./crypto/AddressCalc.h"
# 1 "./util/Linker.h" 1
# 19 "./crypto/AddressCalc.h" 2
struct Linker_x037121836374234674;

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
# 16 "subnode/SupernodeHunter.c" 2
# 1 "./crypto/Key.h" 1
# 18 "./crypto/Key.h"
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

struct Linker_x03216765781444282;
# 33 "./util/Assert.h"
__attribute__((__format__ (__printf__, 1, 2)))
__attribute__((__noreturn__))
void Assert_failure(const char* format, ...);
# 19 "./util/Identity.h" 2
# 1 "./util/Constant.h" 1
# 33 "./util/Constant.h"

# 20 "./util/Identity.h" 2



    
# 19 "./memory/Allocator.h" 2


struct Linker_x08674230509603686;





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
struct Linker_x03857742450525743;

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

struct Linker_x07342412144685011;


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
# 19 "./crypto/Key.h" 2
# 1 "./crypto/random/Random.h" 1
# 19 "./crypto/random/Random.h"
# 1 "./exception/Except.h" 1
# 18 "./exception/Except.h"
# 1 "./exception/Er.h" 1
# 23 "./exception/Er.h"
struct Linker_x0779640264396682;

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


struct Linker_x06990940451943091;



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
struct Linker_x03692697904066682;

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
struct Linker_x022714190691396774;



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

struct Linker_x01815372926595613;



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
# 20 "./crypto/Key.h" 2


struct Linker_x0545695803335805;






int Key_gen(uint8_t addressOut[16],
            uint8_t publicKeyOut[32],
            uint8_t privateKeyOut[32],
            struct Random* rand);

char* Key_parse_strerror(int error);
# 52 "./crypto/Key.h"
int Key_parse(String* key, uint8_t keyBytesOut[32], uint8_t ip6Out[16]);

String* Key_stringify(uint8_t key[32], struct Allocator* alloc);
# 17 "subnode/SupernodeHunter.c" 2
# 1 "./dht/dhtcore/ReplySerializer.h" 1
# 18 "./dht/dhtcore/ReplySerializer.h"
# 1 "./benc/Dict.h" 1
# 21 "./benc/Dict.h"
struct Linker_x02119321048436198;

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
# 19 "./dht/dhtcore/ReplySerializer.h" 2
# 1 "./dht/Address.h" 1
# 21 "./dht/Address.h"
struct Linker_x008484607343621575;
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



struct Linker_x031037100204081947;




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
# 18 "subnode/SupernodeHunter.c" 2
# 1 "./subnode/SupernodeHunter.h" 1
# 20 "./subnode/SupernodeHunter.h"
# 1 "./util/events/EventBase.h" 1
# 20 "./util/events/EventBase.h"
struct Linker_x032934558579620776;

struct EventBase
{
    int unused;
};

struct EventBase* EventBase_new(struct Allocator* alloc);

int EventBase_eventCount(struct EventBase* eventBase);

void EventBase_beginLoop(struct EventBase* eventBase);

void EventBase_endLoop(struct EventBase* eventBase);
# 21 "./subnode/SupernodeHunter.h" 2
# 1 "./subnode/AddrSet.h" 1
# 21 "./subnode/AddrSet.h"
struct Linker_x04009854934132624;

struct AddrSet
{
    int length;
};


int AddrSet_indexOf(struct AddrSet* as, struct Address* addr);

void AddrSet_add(struct AddrSet* as, struct Address* addr);

void AddrSet_remove(struct AddrSet* as, struct Address* addr);

struct Address* AddrSet_get(struct AddrSet* as, int i);

struct AddrSet* AddrSet_new(struct Allocator* alloc);

void AddrSet_flush(struct AddrSet* as);
# 22 "./subnode/SupernodeHunter.h" 2
# 1 "./subnode/MsgCore.h" 1
# 18 "./subnode/MsgCore.h"
# 1 "./interface/Iface.h" 1
# 20 "./interface/Iface.h"
# 1 "./wire/Message.h" 1
# 20 "./wire/Message.h"
# 1 "./util/Bits.h" 1
# 21 "./util/Bits.h"
struct Linker_x02850984449232725;


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

struct Linker_x06210494938611024;



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
# 19 "./subnode/MsgCore.h" 2





# 1 "./switch/EncodingScheme.h" 1
# 19 "./switch/EncodingScheme.h"
# 1 "./benc/List.h" 1
# 21 "./benc/List.h"
struct Linker_x04725918937564262;

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


struct Linker_x06820215216192052;



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
# 25 "./subnode/MsgCore.h" 2


struct Linker_x02012753463267214;

struct MsgCore_Handler;
struct MsgCore;

typedef void (* MsgCore_HandlerCb)(Dict* msg,
                                   struct Address* src,
                                   struct Allocator* tmpAlloc,
                                   struct MsgCore_Handler* handler);

struct MsgCore_Handler
{
    MsgCore_HandlerCb cb;
    void* userData;
};

void MsgCore_sendResponse(struct MsgCore* core,
                          Dict* msgDict,
                          struct Address* target,
                          struct Allocator* alloc);

struct MsgCore_Promise;

typedef void (* MsgCore_PromiseCb)(Dict* msg, struct Address* src, struct MsgCore_Promise* promise);

struct MsgCore_Promise
{
    Dict* msg;
    struct Address* target;
    struct Allocator* alloc;
    void* userData;
    MsgCore_PromiseCb cb;
    uint32_t lag;
};

struct MsgCore_Promise* MsgCore_createQuery(struct MsgCore* core,
                                            uint32_t timeoutMilliseconds,
                                            struct Allocator* alloc);

struct MsgCore_Handler* MsgCore_onQuery(struct MsgCore* core,
                                        char* queryType,
                                        struct Allocator* alloc);

struct MsgCore
{
    struct Iface interRouterIf;
};

struct MsgCore* MsgCore_new(struct EventBase* base,
                            struct Random* rand,
                            struct Allocator* allocator,
                            struct Log* log,
                            struct EncodingScheme* scheme);
# 23 "./subnode/SupernodeHunter.h" 2
# 1 "./subnode/ReachabilityCollector.h" 1
# 17 "./subnode/ReachabilityCollector.h"
# 1 "./subnode/BoilerplateResponder.h" 1
# 24 "./subnode/BoilerplateResponder.h"
struct Linker_x06226325189336428;

struct BoilerplateResponder
{
    int unused;
};

void BoilerplateResponder_addBoilerplate(struct BoilerplateResponder* br,
                                         Dict* msgDict,
                                         struct Address* src,
                                         struct Allocator* alloc);

struct BoilerplateResponder* BoilerplateResponder_new(struct EncodingScheme* myScheme,
                                                      struct Allocator* alloc);
# 18 "./subnode/ReachabilityCollector.h" 2



# 1 "./subnode/LinkState.h" 1
# 19 "./subnode/LinkState.h"
# 1 "./util/VarInt.h" 1
# 25 "./util/VarInt.h"
struct VarInt_Iter {
    uint8_t* ptr;
    uint8_t* end;
    uint8_t* start;
};

static inline void VarInt_clone(struct VarInt_Iter* out, const struct VarInt_Iter* in)
{
    out->ptr = in->ptr;
    out->start = in->start;
    out->end = in->end;
}

static inline void VarInt_mk(struct VarInt_Iter* out, uint8_t* ptr, int length)
{
    out->ptr = ptr;
    out->end = ptr + length;
    out->start = ptr;
}

static inline void VarInt_toStart(struct VarInt_Iter* iter)
{
    iter->ptr = iter->start;
}

static inline void VarInt_toEnd(struct VarInt_Iter* iter)
{
    iter->ptr = iter->end;
}

static inline int VarInt_hasMore(struct VarInt_Iter* iter)
{
    return iter->end > iter->ptr;
}

static inline int VarInt_sizeOf(uint64_t val)
{
    return (!!(val >> 32)) * 4 + (!!(val >> 16)) * 2 + (!!((val + 3) >> 8)) + 1;
}

static inline int VarInt_pop(struct VarInt_Iter* iter, uint64_t* _out)
{
    uint64_t out = 0;
    uint8_t* bytes = iter->ptr;
    int len = iter->end - bytes;
    if (len < 9) {
        if (len < 5) {
            if (len < 3) {
                if (len < 1) { return -1; }
                if (*bytes >= 0xfd) { return -1; }
            } else if (*bytes >= 0xfe) { return -1; }
        } else if (*bytes >= 0xff) { return -1; }
    }
    switch (*bytes) {
        case 0xff:
            out |= *++bytes; out <<= 8;
            out |= *++bytes; out <<= 8;
            out |= *++bytes; out <<= 8;
            out |= *++bytes; out <<= 8;
            __attribute__((fallthrough));
        case 0xfe:
            out |= *++bytes; out <<= 8;
            out |= *++bytes; out <<= 8;
            __attribute__((fallthrough));
        case 0xfd:
            out |= *++bytes; out <<= 8;
            bytes++;
            __attribute__((fallthrough));
        default:
            out |= *bytes++;
    }
    iter->ptr = bytes;
    if (_out) { *_out = out; }
    return 0;
}

static inline int VarInt_push(struct VarInt_Iter* iter, uint64_t val)
{
    uint8_t* ptr = iter->ptr;
    int padding = ptr - iter->start;
    if (padding < 9) {
        if (padding < 5) {
            if (padding < 3) {
                if (padding < 1) { return -1; }
                if (val > 0xfc) { return -1; }
            } else if (val > 0xffff) { return -1; }
        } else if (val > 0xffffffff) { return -1; }
    }
    int i = VarInt_sizeOf(val);
    for (int j = 0; j < i; j++) { *--ptr = val & 0xff; val >>= 8; }
    switch (i) {
        case 8: *--ptr = 0xff; break;
        case 4: *--ptr = 0xfe; break;
        case 2: *--ptr = 0xfd; break;
    }
    iter->ptr = ptr;
    return 0;
}
# 20 "./subnode/LinkState.h" 2

# 1 "./wire/Announce.h" 1
# 18 "./wire/Announce.h"
# 1 "./util/version/Version.h" 1
# 19 "./util/version/Version.h"
struct Linker_x05610160957039259;
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
# 19 "./wire/Announce.h" 2
# 28 "./wire/Announce.h"
enum Announce_Type {
    Announce_Type_ENCODING_SCHEME,
    Announce_Type_PEER,
    Announce_Type_VERSION,
    Announce_Type_LINK_STATE
};
# 42 "./wire/Announce.h"
struct Announce_Version
{

    uint8_t length;


    uint8_t type;

    uint16_t version_be;
};

void Assert_compileTime(char x[1 - (!(sizeof(struct Announce_Version) == 4))]);

static inline void Announce_Version_init(struct Announce_Version* v)
{
    v->length = 4;
    v->type = Announce_Type_VERSION;
    v->version_be = ((uint16_t)(__extension__ ({ uint16_t _x = (((uint16_t)(20))); _x >> 8 | _x << 8; })));
}

struct Announce_EncodingScheme
{

    uint8_t length;


    uint8_t type;


    uint8_t scheme[2];
};

static inline void Announce_EncodingScheme_push(struct Message* pushTo, String* compressedScheme)
{
    do { if (!((compressedScheme->len + 2 < 256))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("Announce.h"), (76), "(compressedScheme->len + 2 < 256)"); } } while (0);
    do {             struct Er_Ret* Er_ret = Message_epush(pushTo, compressedScheme->bytes, compressedScheme->len);                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("Announce.h", 77, Er_ret);                 } else {                     Er__assertFail(Er_unwind("Announce.h", 77, Er_ret));                 }             }                  } while (0);
    do {             struct Er_Ret* Er_ret = Message_epush8h(pushTo, Announce_Type_ENCODING_SCHEME);                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("Announce.h", 78, Er_ret);                 } else {                     Er__assertFail(Er_unwind("Announce.h", 78, Er_ret));                 }             }                  } while (0);
    do {             struct Er_Ret* Er_ret = Message_epush8h(pushTo, compressedScheme->len + 2);                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("Announce.h", 79, Er_ret);                 } else {                     Er__assertFail(Er_unwind("Announce.h", 79, Er_ret));                 }             }                  } while (0);
    while ((uintptr_t)pushTo->bytes % 4) {
        do {             struct Er_Ret* Er_ret = Message_epush8h(pushTo, 1);                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("Announce.h", 81, Er_ret);                 } else {                     Er__assertFail(Er_unwind("Announce.h", 81, Er_ret));                 }             }                  } while (0);
    }
}
# 106 "./wire/Announce.h"
struct Announce_Peer
{

    uint8_t length;


    uint8_t type;


    uint8_t encodingFormNum;


    uint8_t flags;




    uint16_t mtu8_be;



    uint16_t peerNum_be;


    uint32_t unused;


    uint8_t ipv6[16];



    uint32_t label_be;
};

void Assert_compileTime(char x[1 - (!(sizeof(struct Announce_Peer) == 32))]);

static inline void Announce_Peer_init(struct Announce_Peer* peer)
{
    __builtin_memset(peer,0,32);
    peer->length = 32;
    peer->type = Announce_Type_PEER;
    peer->unused = 0xffffffff;
    peer->peerNum_be = 0xffff;
}
# 161 "./wire/Announce.h"
struct Announce_LinkState
{

    uint8_t length;


    uint8_t type;


    uint8_t padding;


    uint8_t linkState[1];
};

static inline void Announce_LinkState_applyHeader(struct Message* pushTo)
{
    Assert_failure("todo implement");
}

struct Announce_ItemHeader
{
    uint8_t length;
    uint8_t type;
};
# 252 "./wire/Announce.h"
struct Announce_Header
{

    uint8_t signature[64];


    uint8_t pubSigningKey[32];






    uint8_t snodeIp[16];


    uint8_t timeStampVersionFlags_be[8];
};

void Assert_compileTime(char x[1 - (!(sizeof(struct Announce_Header) == 120))]);

static inline int64_t Announce_Header_getTimestamp(struct Announce_Header* hdr)
{
    uint64_t ts_be;
    Bits__memcpy(&ts_be,hdr->timeStampVersionFlags_be,sizeof(uint64_t),"Announce.h",276);
    return ((uint64_t)__builtin_bswap64(((uint64_t)(ts_be)))) >> 4;
}

static inline void Announce_Header_setTimestamp(struct Announce_Header* hdr,
                                                    int64_t timestamp)
{
    uint64_t uTime = (uint64_t) timestamp;



    do { if (!((!(uTime >> 60)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("Announce.h"), (287), "(!(uTime >> 60))"); } } while (0);

    uint64_t ts_be;
    Bits__memcpy(&ts_be,hdr->timeStampVersionFlags_be,sizeof(uint64_t),"Announce.h",290);
    ts_be = (ts_be & ((uint64_t)__builtin_bswap64(((uint64_t)(0x0f))))) | ((uint64_t)__builtin_bswap64(((uint64_t)(uTime << 4))));
    Bits__memcpy(hdr->timeStampVersionFlags_be,&ts_be,sizeof(uint64_t),"Announce.h",292);
}

static inline 
# 295 "./wire/Announce.h" 3 4
             _Bool 
# 295 "./wire/Announce.h"
                  Announce_Header_isReset(struct Announce_Header* hdr)
{
    uint64_t ts_be;
    Bits__memcpy(&ts_be,hdr->timeStampVersionFlags_be,sizeof(uint64_t),"Announce.h",298);
    return (((uint64_t)__builtin_bswap64(((uint64_t)(ts_be)))) >> 3) & 1;
}

static inline void Announce_Header_setReset(struct Announce_Header* hdr, 
# 302 "./wire/Announce.h" 3 4
                                                                        _Bool 
# 302 "./wire/Announce.h"
                                                                             isReset)
{
    uint64_t ts_be;
    Bits__memcpy(&ts_be,hdr->timeStampVersionFlags_be,sizeof(uint64_t),"Announce.h",305);
    if (isReset) {
        ts_be |= ((uint64_t)__builtin_bswap64(((uint64_t)(1<<3))));
    } else {
        ts_be &= ~((uint64_t)__builtin_bswap64(((uint64_t)(1<<3))));
    }
    Bits__memcpy(hdr->timeStampVersionFlags_be,&ts_be,sizeof(uint64_t),"Announce.h",311);
}

static inline int Announce_Header_getVersion(struct Announce_Header* hdr)
{
    uint64_t ts_be;
    Bits__memcpy(&ts_be,hdr->timeStampVersionFlags_be,sizeof(uint64_t),"Announce.h",317);
    return ((uint64_t)__builtin_bswap64(((uint64_t)(ts_be)))) & 0x07;
}


static inline void Announce_Header_setVersion(struct Announce_Header* hdr, int version)
{
    uint64_t ts_be;
    Bits__memcpy(&ts_be,hdr->timeStampVersionFlags_be,sizeof(uint64_t),"Announce.h",325);
    ts_be = (ts_be & ~((uint64_t)__builtin_bswap64(((uint64_t)(0x07))))) | ((uint64_t)__builtin_bswap64(((uint64_t)(version & 0x07))));
    Bits__memcpy(hdr->timeStampVersionFlags_be,&ts_be,sizeof(uint64_t),"Announce.h",327);
}

static inline struct Announce_ItemHeader* Announce_ItemHeader_next(struct Message* msg, void* last)
{
    struct Announce_ItemHeader* ih = (struct Announce_ItemHeader*) last;
    if (ih) {
        do { if (!(((uint8_t*)ih > &msg->bytes[-msg->padding]))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("Announce.h"), (334), "((uint8_t*)ih > &msg->bytes[-msg->padding])"); } } while (0);
        do { if (!(((uint8_t*)ih < &msg->bytes[msg->length]))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("Announce.h"), (335), "((uint8_t*)ih < &msg->bytes[msg->length])"); } } while (0);
        ih = (struct Announce_ItemHeader*) ( &((uint8_t*) ih)[ih->length] );
    } else {
        ih = (struct Announce_ItemHeader*) &msg->bytes[120];
    }
    while ((uint8_t*)ih < &msg->bytes[msg->length]) {
        if (!ih->length) { return 
# 341 "./wire/Announce.h" 3 4
                                 ((void *)0)
# 341 "./wire/Announce.h"
                                     ; }
        if (ih->length > 1) {
            if ( &((uint8_t*) ih)[ih->length] > &msg->bytes[msg->length] ) {

                return 
# 345 "./wire/Announce.h" 3 4
                      ((void *)0)
# 345 "./wire/Announce.h"
                          ;
            }
            return ih;
        }
        ih = (struct Announce_ItemHeader*) ( &((uint8_t*) ih)[ih->length] );
    }
    return 
# 351 "./wire/Announce.h" 3 4
          ((void *)0)
# 351 "./wire/Announce.h"
              ;
}

static inline 
# 354 "./wire/Announce.h" 3 4
             _Bool 
# 354 "./wire/Announce.h"
                  Announce_isValid(struct Message* msg)
{
    struct Announce_ItemHeader* ih = 
# 356 "./wire/Announce.h" 3 4
                                    ((void *)0)
# 356 "./wire/Announce.h"
                                        ;
    for (;;) {
        ih = Announce_ItemHeader_next(msg, ih);
        if (!ih) { return 
# 359 "./wire/Announce.h" 3 4
                         0
# 359 "./wire/Announce.h"
                              ; }
        if ((uint8_t*)ih == &msg->bytes[msg->length - ih->length]) { return 
# 360 "./wire/Announce.h" 3 4
                                                                           1
# 360 "./wire/Announce.h"
                                                                               ; }
    }
}

static inline struct Announce_Peer* Announce_Peer_next(struct Message* msg, void* last)
{
    struct Announce_ItemHeader* ih = (struct Announce_ItemHeader*) last;
    do {
        ih = Announce_ItemHeader_next(msg, ih);
    } while (ih && ih->type != Announce_Type_PEER);
    return (struct Announce_Peer*) ih;
}
# 22 "./subnode/LinkState.h" 2






struct LinkState {
    uint16_t lagSlots[18];
    uint16_t dropSlots[18];
    uint32_t kbRecvSlots[18];
    uint32_t samples;
    uint16_t nodeId;
    uint16_t _pad;
};

static inline int LinkState_encode(
    struct Message* msg,
    struct LinkState* ls,
    uint32_t lastSamples)
{



    if (msg->padding < 255) { return 1; }

    struct VarInt_Iter iter = {
        .ptr = msg->bytes,
        .end = msg->bytes,
        .start = &msg->bytes[-msg->padding]
    };


    uint32_t i = ls->samples - 1;
    int count = 0;
    int err = 0;
    for (; i >= lastSamples && count < 18; i--, count++) {
        int idx = i % 18;
        err |= VarInt_push(&iter, ls->kbRecvSlots[idx]);
        err |= VarInt_push(&iter, ls->dropSlots[idx]);
        err |= VarInt_push(&iter, ls->lagSlots[idx]);
    }
    if (err) { return 1; }
    if (!count) { return 0; }


    do { if (!((!VarInt_push(&iter, (i + 1) % 18)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("LinkState.h"), (67), "(!VarInt_push(&iter, (i + 1) % 18))"); } } while (0);
    do { if (!((!VarInt_push(&iter, ls->nodeId)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("LinkState.h"), (68), "(!VarInt_push(&iter, ls->nodeId))"); } } while (0);

    int beginLength = msg->length;
    do {             struct Er_Ret* Er_ret = Message_eshift(msg, (msg->bytes - iter.ptr));                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("LinkState.h", 71, Er_ret);                 } else {                     Er__assertFail(Er_unwind("LinkState.h", 71, Er_ret));                 }             }                  } while (0);
    do { if (!((msg->bytes == iter.ptr))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("LinkState.h"), (72), "(msg->bytes == iter.ptr)"); } } while (0);

    int padCount = 0;
    while ((uintptr_t)(&msg->bytes[-3]) & 7) {
        do {             struct Er_Ret* Er_ret = Message_epush8h(msg, 0);                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("LinkState.h", 76, Er_ret);                 } else {                     Er__assertFail(Er_unwind("LinkState.h", 76, Er_ret));                 }             }                  } while (0);
        padCount++;
    }
    do {             struct Er_Ret* Er_ret = Message_epush8h(msg, padCount);                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("LinkState.h", 79, Er_ret);                 } else {                     Er__assertFail(Er_unwind("LinkState.h", 79, Er_ret));                 }             }                  } while (0);

    do {             struct Er_Ret* Er_ret = Message_epush8h(msg, Announce_Type_LINK_STATE);                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("LinkState.h", 81, Er_ret);                 } else {                     Er__assertFail(Er_unwind("LinkState.h", 81, Er_ret));                 }             }                  } while (0);
    int finalLength = msg->length - beginLength;
    do {             struct Er_Ret* Er_ret = Message_epush8h(msg, finalLength + 1);                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("LinkState.h", 83, Er_ret);                 } else {                     Er__assertFail(Er_unwind("LinkState.h", 83, Er_ret));                 }             }                  } while (0);

    do { if (!((!(((uintptr_t)msg->bytes) & 7)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("LinkState.h"), (85), "(!(((uintptr_t)msg->bytes) & 7))"); } } while (0);
    return 0;
}

static inline int LinkState_mkDecoder(struct Message* msg, struct VarInt_Iter* it)
{
    if (!msg->length) { return 1; }
    uint8_t len = msg->bytes[0];
    if (msg->length < len) { return 1; }
    if (len < 3) { return 1; }
    it->ptr = &msg->bytes[1];
    it->start = it->ptr;
    it->end = &msg->bytes[len];

    uint64_t type = 0;
    if (VarInt_pop(it, &type)) { return 1; }
    if (type != Announce_Type_LINK_STATE) { return 1; }

    uint64_t padCount = 0;
    if (VarInt_pop(it, &padCount)) { return 1; }
    for (unsigned int i = 0; i < padCount; i++) {
        if (VarInt_pop(it, 
# 106 "./subnode/LinkState.h" 3 4
                          ((void *)0)
# 106 "./subnode/LinkState.h"
                              )) { return 1; }
    }
    return 0;
}

static inline int LinkState_getNodeId(const struct VarInt_Iter* iter, uint32_t* nodeId)
{
    struct VarInt_Iter it;
    VarInt_clone(&it, iter);
    uint64_t id;
    if (VarInt_pop(&it, &id)) { return 1; }
    *nodeId = id;
    return 0;
}







static inline int LinkState_decode(const struct VarInt_Iter* iter, struct LinkState* ls)
{
    struct VarInt_Iter it;
    VarInt_clone(&it, iter);
    ls->nodeId = (__extension__ ({ uint64_t x; if (VarInt_pop((&it), &x)) { return 1; } x; }));
    uint32_t i = (__extension__ ({ uint64_t x; if (VarInt_pop((&it), &x)) { return 1; } x; }));
    uint32_t count = 0;
    if (i >= 18) { return 1; }
    for (;;) {
        if (it.ptr == it.end) { break; }
        ls->lagSlots[i] = (__extension__ ({ uint64_t x; if (VarInt_pop((&it), &x)) { return 1; } x; }));
        ls->dropSlots[i] = (__extension__ ({ uint64_t x; if (VarInt_pop((&it), &x)) { return 1; } x; }));
        ls->kbRecvSlots[i] = (__extension__ ({ uint64_t x; if (VarInt_pop((&it), &x)) { return 1; } x; }));
        i = (i + 1) % 18;
        count++;
    }
    ls->samples += count;
    return 0;
}
# 22 "./subnode/ReachabilityCollector.h" 2

struct Linker_x08920565189727703;

struct ReachabilityCollector_PeerInfo
{

    struct Address addr;


    uint64_t pathThemToUs;

    
# 33 "./subnode/ReachabilityCollector.h" 3 4
   _Bool 
# 33 "./subnode/ReachabilityCollector.h"
        querying;

    struct LinkState linkState;
};

struct ReachabilityCollector;


typedef void (* ReachabilityCollector_OnChange)(struct ReachabilityCollector* rc,
                                                uint8_t ipv6[16],
                                                struct ReachabilityCollector_PeerInfo* pi);

struct ReachabilityCollector
{
    ReachabilityCollector_OnChange onChange;
    void* userData;
};

struct ReachabilityCollector_PeerInfo*
    ReachabilityCollector_getPeerInfo(struct ReachabilityCollector* rc, int peerNum);


void ReachabilityCollector_change(struct ReachabilityCollector* rc, struct Address* nodeAddr);

void ReachabilityCollector_lagSample(
    struct ReachabilityCollector* rc, uint64_t label, uint32_t milliseconds);

void ReachabilityCollector_updateBandwidthAndDrops(
    struct ReachabilityCollector* rc,
    uint64_t label,
    uint64_t sumOfPackets,
    uint64_t sumOfDrops,
    uint64_t sumOfKb);

struct ReachabilityCollector* ReachabilityCollector_new(struct Allocator* allocator,
                                                        struct MsgCore* mc,
                                                        struct Log* log,
                                                        struct EventBase* base,
                                                        struct BoilerplateResponder* br,
                                                        struct Address* myAddr,
                                                        struct EncodingScheme* myScheme);
# 24 "./subnode/SupernodeHunter.h" 2

# 1 "./net/SwitchPinger.h" 1
# 25 "./net/SwitchPinger.h"
struct Linker_x012150430559987657;





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
# 26 "./subnode/SupernodeHunter.h" 2

struct Linker_x08604806215953895;

struct SupernodeHunter;

typedef void (* SupernodeHunter_Callback)(struct SupernodeHunter* sh,
                                          int64_t sendTime,
                                          int64_t snodeRecvTime);

struct SupernodeHunter
{







    int snodeIsReachable;

    struct Address snodeAddr;

    SupernodeHunter_Callback onSnodeChange;
    SupernodeHunter_Callback onSnodeUnreachable;
    void* userData;
};


int SupernodeHunter_addSnode(struct SupernodeHunter* snh, struct Address* snodeAddr);

int SupernodeHunter_listSnodes(struct SupernodeHunter* snh,
                               struct Address*** out,
                               struct Allocator* alloc);


int SupernodeHunter_removeSnode(struct SupernodeHunter* snh, struct Address* toRemove);






struct SupernodeHunter* SupernodeHunter_new(struct Allocator* allocator,
                                            struct Log* log,
                                            struct EventBase* base,
                                            struct SwitchPinger* sp,
                                            struct AddrSet* peers,
                                            struct MsgCore* msgCore,
                                            struct Address* myAddress,
                                            struct ReachabilityCollector* rc);
# 19 "subnode/SupernodeHunter.c" 2


# 1 "./util/platform/Sockaddr.h" 1
# 21 "./util/platform/Sockaddr.h"
struct Linker_x032387714955970925;



struct Sockaddr
{

    uint16_t addrLen;



    uint8_t flags;



    uint8_t type;


    uint8_t prefix;

    uint8_t pad1;
    uint16_t pad2;
};






struct Sockaddr_storage
{
    struct Sockaddr addr;
    uint64_t nativeAddr[128 / 8];
};


extern const struct Sockaddr* const Sockaddr_LOOPBACK_be;
extern const struct Sockaddr* const Sockaddr_LOOPBACK_le;


extern const struct Sockaddr* const Sockaddr_LOOPBACK6;







uint32_t Sockaddr_addrHandle(const struct Sockaddr* addr);





void Sockaddr_addrFromHandle(struct Sockaddr* addr, uint32_t handle);


int Sockaddr_getPrefix(struct Sockaddr* addr);
# 88 "./util/platform/Sockaddr.h"
int Sockaddr_parse(const char* str, struct Sockaddr_storage* out);




char* Sockaddr_print(struct Sockaddr* addr, struct Allocator* alloc);







int Sockaddr_getPort(struct Sockaddr* sa);
# 110 "./util/platform/Sockaddr.h"
int Sockaddr_setPort(struct Sockaddr* sa, uint16_t port);







extern const int Sockaddr_AF_INET;
extern const int Sockaddr_AF_INET6;
int Sockaddr_getFamily(struct Sockaddr* sa);
# 130 "./util/platform/Sockaddr.h"
int Sockaddr_getAddress(struct Sockaddr* sa, void* addrPtr);




struct Sockaddr* Sockaddr_fromNative(const void* ss, int addrLen, struct Allocator* alloc);




static inline void* Sockaddr_asNative(struct Sockaddr* sa)
{
    return (void*)(&sa[1]);
}
static inline const void* Sockaddr_asNativeConst(const struct Sockaddr* sa)
{
    return (const void*)(&sa[1]);
}





struct Sockaddr* Sockaddr_fromBytes(const uint8_t* bytes, int addrFamily, struct Allocator* alloc);




struct Sockaddr* Sockaddr_clone(const struct Sockaddr* addr, struct Allocator* alloc);




void Sockaddr_normalizeNative(void* nativeSockaddr);




uint32_t Sockaddr_hash(const struct Sockaddr* addr);




int Sockaddr_compare(const struct Sockaddr* a, const struct Sockaddr* b);
# 22 "subnode/SupernodeHunter.c" 2
# 1 "./util/events/Timeout.h" 1
# 20 "./util/events/Timeout.h"
struct Linker_x039551007524100856;



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
# 23 "subnode/SupernodeHunter.c" 2
# 1 "./util/AddrTools.h" 1
# 19 "./util/AddrTools.h"
struct Linker_x08722830032591624;




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
# 24 "subnode/SupernodeHunter.c" 2
# 1 "./util/events/Time.h" 1
# 20 "./util/events/Time.h"
struct Linker_x05407137792396906;




uint64_t Time_hrtime(void);



uint64_t Time_currentTimeMilliseconds(struct EventBase* eventBase);


uint64_t Time_currentTimeSeconds(struct EventBase* eventBase);
# 25 "subnode/SupernodeHunter.c" 2
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
# 26 "subnode/SupernodeHunter.c" 2



struct SupernodeHunter_pvt
{
    struct SupernodeHunter pub;


    struct AddrSet* authorizedSnodes;


    struct AddrSet* peers;


    int nextPeer;


    struct Address snodeCandidate;

    
# 45 "subnode/SupernodeHunter.c" 3 4
   _Bool 
# 45 "subnode/SupernodeHunter.c"
        snodePathUpdated;

    struct Allocator* alloc;

    struct Log* log;

    struct MsgCore* msgCore;

    struct EventBase* base;

    struct SwitchPinger* sp;

    struct Address* myAddress;
    String* selfAddrStr;

    struct ReachabilityCollector* rc;

    unsigned long Identity_verifier;
};

struct Query
{
    struct SupernodeHunter_pvt* snp;


    struct Address* searchTar;

    int64_t sendTime;

    
# 74 "subnode/SupernodeHunter.c" 3 4
   _Bool 
# 74 "subnode/SupernodeHunter.c"
        isGetRoute;

    unsigned long Identity_verifier;
};

int SupernodeHunter_addSnode(struct SupernodeHunter* snh, struct Address* snodeAddr)
{
    struct SupernodeHunter_pvt* snp = (__extension__ ({ __typeof__((struct SupernodeHunter_pvt*) snh) Identity_ptr = ((struct SupernodeHunter_pvt*) snh); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x37adfb99272e13b9ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("SupernodeHunter.c"), (81), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x37adfb99272e13b9ull))"); } } while (0); Identity_ptr; }));
    int length0 = snp->authorizedSnodes->length;
    AddrSet_add(snp->authorizedSnodes, snodeAddr);
    if (snp->authorizedSnodes->length == length0) {
        return -1;
    }
    return 0;
}

int SupernodeHunter_listSnodes(struct SupernodeHunter* snh,
                               struct Address*** outP,
                               struct Allocator* alloc)
{
    struct SupernodeHunter_pvt* snp = (__extension__ ({ __typeof__((struct SupernodeHunter_pvt*) snh) Identity_ptr = ((struct SupernodeHunter_pvt*) snh); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x37adfb99272e13b9ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("SupernodeHunter.c"), (94), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x37adfb99272e13b9ull))"); } } while (0); Identity_ptr; }));
    struct Address** out = Allocator__calloc((alloc),(sizeof(char*)),(snp->authorizedSnodes->length),"SupernodeHunter.c",95);
    for (int i = 0; i < snp->authorizedSnodes->length; i++) {
        out[i] = AddrSet_get(snp->authorizedSnodes, i);
    }
    *outP = out;
    return snp->authorizedSnodes->length;
}

int SupernodeHunter_removeSnode(struct SupernodeHunter* snh, struct Address* toRemove)
{
    struct SupernodeHunter_pvt* snp = (__extension__ ({ __typeof__((struct SupernodeHunter_pvt*) snh) Identity_ptr = ((struct SupernodeHunter_pvt*) snh); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x37adfb99272e13b9ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("SupernodeHunter.c"), (105), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x37adfb99272e13b9ull))"); } } while (0); Identity_ptr; }));
    int length0 = snp->authorizedSnodes->length;
    AddrSet_remove(snp->authorizedSnodes, toRemove);
    if (snp->authorizedSnodes->length == length0) {
        return -1;
    }
    return 0;
}

static struct Address* getPeerByNpn(struct SupernodeHunter_pvt* snp, int npn)
{
    npn = npn % snp->peers->length;
    int i = npn;
    do {
        struct Address* peer = AddrSet_get(snp->peers, i);
        if (peer && peer->protocolVersion > 19) { return peer; }
        i = (i + 1) % snp->peers->length;
    } while (i != npn);
    return 
# 123 "subnode/SupernodeHunter.c" 3 4
          ((void *)0)
# 123 "subnode/SupernodeHunter.c"
              ;
}

static void adoptSupernode2(Dict* msg, struct Address* src, struct MsgCore_Promise* prom)
{
    struct Query* q = (__extension__ ({ __typeof__((struct Query*) prom->userData) Identity_ptr = ((struct Query*) prom->userData); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x37adfb99272e13b9ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("SupernodeHunter.c"), (128), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x37adfb99272e13b9ull))"); } } while (0); Identity_ptr; }));
    struct SupernodeHunter_pvt* snp = (__extension__ ({ __typeof__(q->snp) Identity_ptr = (q->snp); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x37adfb99272e13b9ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("SupernodeHunter.c"), (129), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x37adfb99272e13b9ull))"); } } while (0); Identity_ptr; }));

    if (!src) {
        String* addrStr = Address_toString(prom->target, prom->alloc);
        do { if (snp->log && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(snp->log, Log_Level_DEBUG, "SupernodeHunter.c", 133, "timeout sending to %s", addrStr->bytes); } } while (0);
        return;
    }
    String* addrStr = Address_toString(src, prom->alloc);
    do { if (snp->log && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(snp->log, Log_Level_DEBUG, "SupernodeHunter.c", 137, "Reply from %s", addrStr->bytes); } } while (0);

    int64_t* snodeRecvTime = Dict_getInt(msg, (&(String) { .bytes = "recvTime", .len = CString_strlen("recvTime") }));
    if (!snodeRecvTime) {
        do { if (snp->log && Log_Level_INFO >= Log_Level_DEBUG) { Log_print(snp->log, Log_Level_INFO, "SupernodeHunter.c", 141, "getRoute reply with no timeStamp, bad snode"); } } while (0);
        return;
    }
    do { if (snp->log && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(snp->log, Log_Level_DEBUG, "SupernodeHunter.c", 144, "\n\nSupernode location confirmed [%s]\n\n", Address_toString(src, prom->alloc)->bytes); } } while (0)
                                                  ;
    if (snp->pub.snodeIsReachable) {


    } else if (snp->pub.onSnodeChange) {
        Bits__memcpy(&snp->pub.snodeAddr,src,(8 + 16 + 32 + 8),"SupernodeHunter.c",150);
        snp->pub.snodeIsReachable = (AddrSet_indexOf(snp->authorizedSnodes, src) != -1) ? 2 : 1;
        snp->pub.onSnodeChange(&snp->pub, q->sendTime, *snodeRecvTime);
    } else {
        do { if (snp->log && Log_Level_WARN >= Log_Level_DEBUG) { Log_print(snp->log, Log_Level_WARN, "SupernodeHunter.c", 154, "onSnodeChange is not set"); } } while (0);
    }
}

static void adoptSupernode(struct SupernodeHunter_pvt* snp, struct Address* candidate)
{
    struct MsgCore_Promise* qp = MsgCore_createQuery(snp->msgCore, 0, snp->alloc);
    struct Query* q = Allocator__calloc((qp->alloc),(sizeof(struct Query)),(1),"SupernodeHunter.c",161);
    (q)->Identity_verifier = ((unsigned long) 0x37adfb99272e13b9ull);
    q->snp = snp;
    q->sendTime = Time_currentTimeMilliseconds(snp->base);

    Dict* msg = qp->msg = Dict_new(qp->alloc);
    qp->cb = adoptSupernode2;
    qp->userData = q;
    qp->target = Address_clone(candidate, qp->alloc);

    do { if (snp->log && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(snp->log, Log_Level_DEBUG, "SupernodeHunter.c", 171, "Pinging snode [%s]", Address_toString(qp->target, qp->alloc)->bytes); } } while (0);
    Dict_putString(msg, String_new("sq", qp->alloc), String_new("pn", qp->alloc), qp->alloc);

    do { if (!((AddressCalc_validAddress(candidate->ip6.bytes)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("SupernodeHunter.c"), (174), "(AddressCalc_validAddress(candidate->ip6.bytes))"); } } while (0);
    return;
}

static void updateSnodePath2(Dict* msg, struct Address* src, struct MsgCore_Promise* prom)
{
    struct Query* q = (__extension__ ({ __typeof__((struct Query*) prom->userData) Identity_ptr = ((struct Query*) prom->userData); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x37adfb99272e13b9ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("SupernodeHunter.c"), (180), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x37adfb99272e13b9ull))"); } } while (0); Identity_ptr; }));
    struct SupernodeHunter_pvt* snp = (__extension__ ({ __typeof__(q->snp) Identity_ptr = (q->snp); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x37adfb99272e13b9ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("SupernodeHunter.c"), (181), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x37adfb99272e13b9ull))"); } } while (0); Identity_ptr; }));

    if (!src) {
        String* addrStr = Address_toString(prom->target, prom->alloc);
        do { if (snp->log && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(snp->log, Log_Level_DEBUG, "SupernodeHunter.c", 185, "timeout sending to %s", addrStr->bytes); } } while (0);
        return;
    }
    int64_t* snodeRecvTime = Dict_getInt(msg, (&(String) { .bytes = "recvTime", .len = CString_strlen("recvTime") }));
    if (!snodeRecvTime) {
        do { if (snp->log && Log_Level_INFO >= Log_Level_DEBUG) { Log_print(snp->log, Log_Level_INFO, "SupernodeHunter.c", 190, "getRoute reply with no timeStamp, bad snode"); } } while (0);
        return;
    }
    struct Address_List* al = ReplySerializer_parse(src, msg, snp->log, 
# 193 "subnode/SupernodeHunter.c" 3 4
                                                                       0
# 193 "subnode/SupernodeHunter.c"
                                                                            , prom->alloc);
    if (!al || al->length == 0) { return; }
    do { if (snp->log && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(snp->log, Log_Level_DEBUG, "SupernodeHunter.c", 195, "Supernode path updated with [%s]", Address_toString(&al->elems[0], prom->alloc)->bytes); } } while (0)
                                                                            ;

    snp->snodePathUpdated = 
# 198 "subnode/SupernodeHunter.c" 3 4
                           1
# 198 "subnode/SupernodeHunter.c"
                               ;
    if (!__builtin_memcmp(&snp->pub.snodeAddr,&al->elems[0],(8 + 16 + 32 + 8))) {
        return;
    }
    Bits__memcpy(&snp->pub.snodeAddr,&al->elems[0],(8 + 16 + 32 + 8),"SupernodeHunter.c",202);
    Bits__memcpy(&snp->snodeCandidate,&al->elems[0],(8 + 16 + 32 + 8),"SupernodeHunter.c",203);
    if (snp->pub.onSnodeChange) {
        snp->pub.snodeIsReachable = (AddrSet_indexOf(snp->authorizedSnodes, src) != -1) ? 2 : 1;
        snp->pub.onSnodeChange(&snp->pub, q->sendTime, *snodeRecvTime);
    }
}

static void updateSnodePath(struct SupernodeHunter_pvt* snp)
{
    struct MsgCore_Promise* qp = MsgCore_createQuery(snp->msgCore, 0, snp->alloc);
    struct Query* q = Allocator__calloc((qp->alloc),(sizeof(struct Query)),(1),"SupernodeHunter.c",213);
    (q)->Identity_verifier = ((unsigned long) 0x37adfb99272e13b9ull);
    q->snp = snp;
    q->sendTime = Time_currentTimeMilliseconds(snp->base);

    Dict* msg = qp->msg = Dict_new(qp->alloc);
    qp->cb = updateSnodePath2;
    qp->userData = q;
    qp->target = Address_clone(&snp->pub.snodeAddr, qp->alloc);;

    do { if (snp->log && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(snp->log, Log_Level_DEBUG, "SupernodeHunter.c", 223, "Update snode [%s] path", Address_toString(qp->target, qp->alloc)->bytes); } } while (0);
    Dict_putString(msg, String_new("sq", qp->alloc), String_new("gr", qp->alloc), qp->alloc);
    String* src = String_newBinary(snp->myAddress->ip6.bytes, 16, qp->alloc);
    Dict_putString(msg, String_new("src", qp->alloc), src, qp->alloc);
    String* target = String_newBinary(snp->pub.snodeAddr.ip6.bytes, 16, qp->alloc);
    Dict_putString(msg, String_new("tar", qp->alloc), target, qp->alloc);
}

static void queryForAuthorized(struct SupernodeHunter_pvt* snp, struct Address* snode)
{
# 248 "subnode/SupernodeHunter.c"
}

static void peerResponseOK(struct SwitchPinger_Response* resp, struct SupernodeHunter_pvt* snp)
{
    ReachabilityCollector_lagSample(snp->rc, resp->label, resp->milliseconds);
    struct Address snode;
    Bits__memcpy(&snode,&resp->snode,sizeof(struct Address),"SupernodeHunter.c",254);
    if (!snode.path) {
        uint8_t label[20];
        AddrTools_printPath(label, resp->label);
        do { if (snp->log && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(snp->log, Log_Level_DEBUG, "SupernodeHunter.c", 258, "Peer [%s] reports no supernode", label); } } while (0);
        return;
    }

    uint64_t path = LabelSplicer_splice(snode.path, resp->label);
    if (path == 
# 263 "subnode/SupernodeHunter.c" 3 4
               0xffffffffffffffffULL
# 263 "subnode/SupernodeHunter.c"
                         ) {
        do { if (snp->log && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(snp->log, Log_Level_DEBUG, "SupernodeHunter.c", 264, "Supernode path could not be spliced"); } } while (0);
        return;
    }
    snode.path = path;

    struct Address* firstPeer = getPeerByNpn(snp, 0);
    if (!firstPeer) {
        do { if (snp->log && Log_Level_INFO >= Log_Level_DEBUG) { Log_print(snp->log, Log_Level_INFO, "SupernodeHunter.c", 271, "All peers have gone away while packet was outstanding"); } } while (0);
        return;
    }




    if (!snp->pub.snodeIsReachable && snp->nextPeer > 1 && firstPeer->path == resp->label) {
        if (!snp->snodeCandidate.path) {
            do { if (snp->log && Log_Level_INFO >= Log_Level_DEBUG) { Log_print(snp->log, Log_Level_INFO, "SupernodeHunter.c", 280, "No snode candidate found"); } } while (0);
            snp->nextPeer = 0;
            return;
        }
        adoptSupernode(snp, &snp->snodeCandidate);
        return;
    }



    if (!snp->authorizedSnodes->length || AddrSet_indexOf(snp->authorizedSnodes, &snode) > -1) {
        Address_getPrefix(&snode);
        adoptSupernode(snp, &snode);
        return;
    }

    if (!snp->snodeCandidate.path) {
        Bits__memcpy(&snp->snodeCandidate,&snode,sizeof(struct Address),"SupernodeHunter.c",297);
        Address_getPrefix(&snp->snodeCandidate);
    }



    queryForAuthorized(snp, &snode);
}

static void peerResponse(struct SwitchPinger_Response* resp, void* userData)
{
    struct SupernodeHunter_pvt* snp = (__extension__ ({ __typeof__((struct SupernodeHunter_pvt*) userData) Identity_ptr = ((struct SupernodeHunter_pvt*) userData); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x37adfb99272e13b9ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("SupernodeHunter.c"), (308), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x37adfb99272e13b9ull))"); } } while (0); Identity_ptr; }));
    char* err = "";
    switch (resp->res) {
        case SwitchPinger_Result_OK: peerResponseOK(resp, snp); return;
        case SwitchPinger_Result_LABEL_MISMATCH: err = "LABEL_MISMATCH"; break;
        case SwitchPinger_Result_WRONG_DATA: err = "WRONG_DATA"; break;
        case SwitchPinger_Result_ERROR_RESPONSE: err = "ERROR_RESPONSE"; break;
        case SwitchPinger_Result_LOOP_ROUTE: err = "LOOP_ROUTE"; break;
        case SwitchPinger_Result_TIMEOUT: err = "TIMEOUT"; break;
        default: err = "unknown error"; break;
    }
    do { if (snp->log && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(snp->log, Log_Level_DEBUG, "SupernodeHunter.c", 319, "Error sending snp query to peer [%s]", err); } } while (0);
}

static void probePeerCycle(void* vsn)
{
    struct SupernodeHunter_pvt* snp = (__extension__ ({ __typeof__((struct SupernodeHunter_pvt*) vsn) Identity_ptr = ((struct SupernodeHunter_pvt*) vsn); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x37adfb99272e13b9ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("SupernodeHunter.c"), (324), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x37adfb99272e13b9ull))"); } } while (0); Identity_ptr; }));

    if (snp->pub.snodeIsReachable && !snp->snodePathUpdated) {
        updateSnodePath(snp);
    }

    if (snp->pub.snodeIsReachable > 1) { return; }
    if (snp->pub.snodeIsReachable && !snp->authorizedSnodes->length) { return; }
    if (!snp->peers->length) { return; }



    if (AddrSet_indexOf(snp->authorizedSnodes, snp->myAddress) != -1) {
        do { if (snp->log && Log_Level_INFO >= Log_Level_DEBUG) { Log_print(snp->log, Log_Level_INFO, "SupernodeHunter.c", 337, "Self is specified as supernode, pinging..."); } } while (0);
        adoptSupernode(snp, snp->myAddress);
        return;
    }

    struct Address* peer = getPeerByNpn(snp, snp->nextPeer);
    if (!peer) {
        do { if (snp->log && Log_Level_INFO >= Log_Level_DEBUG) { Log_print(snp->log, Log_Level_INFO, "SupernodeHunter.c", 344, "No peer found who is version >= 20"); } } while (0);
        return;
    }
    snp->nextPeer++;

    struct SwitchPinger_Ping* p =
        SwitchPinger_newPing(peer->path, (&(String) { .bytes = "", .len = CString_strlen("") }), 3000, peerResponse, snp->alloc, snp->sp);
    do { if (!((p))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("SupernodeHunter.c"), (351), "(p)"); } } while (0);

    p->type = SwitchPinger_Type_GETSNODE;
    if (snp->pub.snodeIsReachable) {
        Bits__memcpy(&p->snode,&snp->pub.snodeAddr,sizeof(struct Address),"SupernodeHunter.c",355);
    }

    p->onResponseContext = snp;
}

static void onSnodeUnreachable(struct SupernodeHunter* snh,
                               int64_t sendTime,
                               int64_t snodeRecvTime)
{
    struct SupernodeHunter_pvt* snp = (__extension__ ({ __typeof__((struct SupernodeHunter_pvt*) snh) Identity_ptr = ((struct SupernodeHunter_pvt*) snh); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x37adfb99272e13b9ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("SupernodeHunter.c"), (365), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x37adfb99272e13b9ull))"); } } while (0); Identity_ptr; }));
    do { if (snp->log && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(snp->log, Log_Level_DEBUG, "SupernodeHunter.c", 366, "Supernode unreachable."); } } while (0);
    snp->snodePathUpdated = 
# 367 "subnode/SupernodeHunter.c" 3 4
                           0
# 367 "subnode/SupernodeHunter.c"
                                ;

    __builtin_memset(&snp->snodeCandidate,0,(8 + 16 + 32 + 8));
}

struct SupernodeHunter* SupernodeHunter_new(struct Allocator* allocator,
                                            struct Log* log,
                                            struct EventBase* base,
                                            struct SwitchPinger* sp,
                                            struct AddrSet* peers,
                                            struct MsgCore* msgCore,
                                            struct Address* myAddress,
                                            struct ReachabilityCollector* rc)
{
    struct Allocator* alloc = Allocator__child((allocator),"SupernodeHunter.c",381);
    struct SupernodeHunter_pvt* out =
        Allocator__calloc((alloc),(sizeof(struct SupernodeHunter_pvt)),(1),"SupernodeHunter.c",383);
    (out)->Identity_verifier = ((unsigned long) 0x37adfb99272e13b9ull);
    out->authorizedSnodes = AddrSet_new(alloc);
    out->peers = peers;
    out->base = base;




    out->log = log;
    out->alloc = alloc;
    out->msgCore = msgCore;
    out->myAddress = myAddress;
    out->rc = rc;
    out->selfAddrStr = String_newBinary(myAddress->ip6.bytes, 16, alloc);
    out->sp = sp;
    out->snodePathUpdated = 
# 399 "subnode/SupernodeHunter.c" 3 4
                           0
# 399 "subnode/SupernodeHunter.c"
                                ;
    out->pub.onSnodeUnreachable = onSnodeUnreachable;
    Timeout__setInterval((probePeerCycle), (out), (3000), (base), (alloc), "SupernodeHunter.c", 401);
    return &out->pub;
}
