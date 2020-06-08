# 1 "test/TestFramework.c"
# 1 "/home/jojapoppa/cjdns//"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "test/TestFramework.c"
# 15 "test/TestFramework.c"
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
struct Linker_x06684346702311661;
# 33 "./util/Assert.h"
__attribute__((__format__ (__printf__, 1, 2)))
__attribute__((__noreturn__))
void Assert_failure(const char* format, ...);
# 19 "./util/Identity.h" 2
# 1 "./util/Constant.h" 1
# 33 "./util/Constant.h"

# 20 "./util/Identity.h" 2



    
# 19 "./memory/Allocator.h" 2


struct Linker_x022910141185203026;





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
struct Linker_x06033480059431007;

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


struct Linker_x046325726547067925;



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
struct Linker_x08045729460251512;

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
struct Linker_x08099877256262484;

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

struct Linker_x09136848712329109;



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
# 16 "test/TestFramework.c" 2
# 1 "./crypto/CryptoAuth.h" 1
# 18 "./crypto/CryptoAuth.h"
# 1 "./benc/StringList.h" 1
# 18 "./benc/StringList.h"
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
struct Linker_x08857245791094566;

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

struct Linker_x03816652698854539;

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
struct Linker_x0514595745450185;





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

# 1 "./crypto/ReplayProtector.h" 1
# 18 "./crypto/ReplayProtector.h"
# 1 "./util/Bits.h" 1
# 21 "./util/Bits.h"
struct Linker_x04875242281228809;


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
struct Linker_x00775995061070176;

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
struct Linker_x007263915798229226;



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

struct Linker_x06686169617288775;







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
# 17 "test/TestFramework.c" 2
# 1 "./crypto/AddressCalc.h" 1
# 19 "./crypto/AddressCalc.h"
struct Linker_x037709151606593116;
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
# 18 "test/TestFramework.c" 2

# 1 "./dht/Pathfinder.h" 1
# 26 "./dht/Pathfinder.h"
# 1 "./admin/Admin.h" 1
# 18 "./admin/Admin.h"
# 1 "./interface/Iface.h" 1
# 21 "./interface/Iface.h"
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
# 19 "./admin/Admin.h" 2
# 1 "./interface/addressable/AddrIface.h" 1
# 19 "./interface/addressable/AddrIface.h"
# 1 "./util/platform/Sockaddr.h" 1
# 21 "./util/platform/Sockaddr.h"
struct Linker_x09846658402833257;



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
# 20 "./interface/addressable/AddrIface.h" 2
# 28 "./interface/addressable/AddrIface.h"
struct AddrIface
{

    struct Iface iface;






    struct Sockaddr* addr;

    struct Allocator* alloc;
};

static inline __attribute__ ((warn_unused_result)) struct Er_Ret* AddrIface_pushAddr(struct Message* msg, struct Sockaddr* addr)
{
    do {             struct Er_Ret* Er_ret = Message_epush(msg, addr, addr->addrLen);             if (Er_ret) { return Er_unwind("AddrIface.h", 45, Er_ret); }         } while (0);
    return (struct Er_Ret*)0;
}

static inline __attribute__ ((warn_unused_result)) struct Er_Ret* AddrIface_popAddr(struct Sockaddr* *Er_returnValP, struct Message* msg)
{
    struct Sockaddr* out = (struct Sockaddr*) msg->bytes;
    uint16_t len = (__extension__({             uint16_t Er_returnVal;             __builtin_memset(&Er_returnVal, 0, sizeof(Er_returnVal));             struct Er_Ret* Er_ret = Message_epop16h(&Er_returnVal, msg);             if (Er_ret) { return Er_unwind("AddrIface.h", 52, Er_ret); }             Er_returnVal;         }));
    do {             struct Er_Ret* Er_ret = Message_epop(msg,    ((void *)0)   , len - 2);             if (Er_ret) { return Er_unwind("AddrIface.h", 53, Er_ret); }         } while (0);
    *Er_returnValP = out; return (struct Er_Ret*)0;
}
# 20 "./admin/Admin.h" 2
# 1 "./benc/Dict.h" 1
# 21 "./benc/Dict.h"
struct Linker_x05368209013698146;

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
# 21 "./admin/Admin.h" 2



# 1 "./util/UniqueName.h" 1
# 22 "./util/UniqueName.h"

# 25 "./admin/Admin.h" 2


struct Linker_x09312197717182669;



typedef void (* Admin_Function)
    (Dict* args, void* vcontext, String* txid, struct Allocator* requestAlloc);

struct Admin
{
    int unused;
};

struct Admin_FunctionArg
{
    char* name;
    char* type;
    
# 43 "./admin/Admin.h" 3 4
   _Bool 
# 43 "./admin/Admin.h"
        required;
};
# 61 "./admin/Admin.h"
void Admin_registerFunctionWithArgCount(char* name,
                                        Admin_Function callback,
                                        void* callbackContext,
                                        
# 64 "./admin/Admin.h" 3 4
                                       _Bool 
# 64 "./admin/Admin.h"
                                            needsAuth,
                                        struct Admin_FunctionArg* arguments,
                                        int argCount,
                                        struct Admin* admin);





int Admin_sendMessage(Dict* message, String* txid, struct Admin* admin);

struct Admin* Admin_new(struct AddrIface* ai,
                        struct Log* logger,
                        struct EventBase* eventBase,
                        String* password);
# 27 "./dht/Pathfinder.h" 2

struct Linker_x047201358921791625;

struct Pathfinder
{
    struct Iface eventIf;
    
# 33 "./dht/Pathfinder.h" 3 4
   _Bool 
# 33 "./dht/Pathfinder.h"
        fullVerify;
};

struct Pathfinder* Pathfinder_register(struct Allocator* alloc,
                                       struct Log* logger,
                                       struct EventBase* base,
                                       struct Random* rand,
                                       struct Admin* admin);
# 20 "test/TestFramework.c" 2

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
# 22 "test/TestFramework.c" 2
# 1 "./io/FileWriter.h" 1
# 21 "./io/FileWriter.h"
struct Linker_x027055790654559053;
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
# 23 "./io/FileWriter.h" 2








# 30 "./io/FileWriter.h"
struct Writer* FileWriter_new(FILE* writeTo, struct Allocator* allocator);
# 23 "test/TestFramework.c" 2

# 1 "./memory/MallocAllocator.h" 1
# 21 "./memory/MallocAllocator.h"
struct Linker_x08917124238304319;
# 30 "./memory/MallocAllocator.h"
struct Allocator* MallocAllocator__new(unsigned long sizeLimit, const char* file, int line);
# 25 "test/TestFramework.c" 2

# 1 "./switch/SwitchCore.h" 1
# 23 "./switch/SwitchCore.h"
struct Linker_x06446238782668183;




struct SwitchCore
{
    struct Iface* routerIf;
};







struct SwitchCore* SwitchCore_new(struct Log* logger,
                                  struct Allocator* allocator,
                                  struct EventBase* base);


int SwitchCore_addInterface(struct SwitchCore* switchCore,
                            struct Iface* iface,
                            struct Allocator* alloc,
                            uint64_t* labelOut);

void SwitchCore_swapInterfaces(struct Iface* if1, struct Iface* if2);



void SwitchCore_setInterfaceState(struct Iface* iface, int ifaceState);
# 27 "test/TestFramework.c" 2
# 1 "./subnode/SubnodePathfinder.h" 1
# 18 "./subnode/SubnodePathfinder.h"
# 1 "./admin/Admin.h" 1
# 19 "./subnode/SubnodePathfinder.h" 2





# 1 "./subnode/SupernodeHunter.h" 1
# 21 "./subnode/SupernodeHunter.h"
# 1 "./subnode/AddrSet.h" 1
# 19 "./subnode/AddrSet.h"
# 1 "./dht/Address.h" 1
# 21 "./dht/Address.h"
struct Linker_x05135604407768946;
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
# 20 "./subnode/AddrSet.h" 2

struct Linker_x05024381079844371;

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
# 24 "./subnode/MsgCore.h"
# 1 "./switch/EncodingScheme.h" 1
# 19 "./switch/EncodingScheme.h"
# 1 "./benc/List.h" 1
# 21 "./benc/List.h"
struct Linker_x0025871271755558922;

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


struct Linker_x04217705872833586;



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


struct Linker_x09727548034919742;

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
struct Linker_x030263951228022923;

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
struct Linker_x0526291630957272;
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

struct Linker_x028856910030603755;

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
struct Linker_x03103593071482331;





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

struct Linker_x07905999250432345;

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
# 25 "./subnode/SubnodePathfinder.h" 2


struct Linker_x05014712789835987;

struct SubnodePathfinder
{
    struct Iface eventIf;
    struct SupernodeHunter* snh;
    struct ReachabilityCollector* rc;
};

void SubnodePathfinder_start(struct SubnodePathfinder*);

struct SubnodePathfinder* SubnodePathfinder_new(struct Allocator* allocator,
                                                struct Log* log,
                                                struct EventBase* base,
                                                struct Random* rand,
                                                struct Address* myAddress,
                                                uint8_t* privateKey,
                                                struct EncodingScheme* myScheme);
# 28 "test/TestFramework.c" 2
# 1 "./test/TestFramework.h" 1
# 19 "./test/TestFramework.h"
# 1 "./net/NetCore.h" 1
# 25 "./net/NetCore.h"
# 1 "./net/ControlHandler.h" 1
# 20 "./net/ControlHandler.h"
# 1 "./net/EventEmitter.h" 1
# 20 "./net/EventEmitter.h"
# 1 "./wire/PFChan.h" 1
# 19 "./wire/PFChan.h"
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
# 20 "./wire/PFChan.h" 2
# 1 "./wire/DataHeader.h" 1
# 20 "./wire/DataHeader.h"
# 1 "./wire/ContentType.h" 1
# 18 "./wire/ContentType.h"
enum ContentType
{






    ContentType_IP6_HOP = 0,
    ContentType_IP6_ICMP = 1,
    ContentType_IP6_IGMP = 2,
    ContentType_IP6_IPV4 = 4,
    ContentType_IP6_TCP = 6,
    ContentType_IP6_EGP = 8,
    ContentType_IP6_PUP = 12,
    ContentType_IP6_UDP = 17,
    ContentType_IP6_IDP = 22,
    ContentType_IP6_TP = 29,
    ContentType_IP6_DCCP = 33,
    ContentType_IP6_IPV6 = 41,
    ContentType_IP6_RSVP = 46,
    ContentType_IP6_GRE = 47,
    ContentType_IP6_ESP = 50,
    ContentType_IP6_AH = 51,
    ContentType_IP6_ICMPV6 = 58,
    ContentType_IP6_MTP = 92,
    ContentType_IP6_BEETPH = 94,
    ContentType_IP6_ENCAP = 98,
    ContentType_IP6_PIM = 103,
    ContentType_IP6_COMP = 108,
    ContentType_IP6_SCTP = 132,
    ContentType_IP6_UDPLITE = 136,
    ContentType_IP6_MAX = 255,


    ContentType_CJDHT = 256,
    ContentType_IPTUN = 257,


    ContentType_RESERVED = 258,
    ContentType_RESERVED_MAX = 0x7fff,







    ContentType_AVAILABLE = 0x8000,


    ContentType_CTRL = 0xffff + 1,

    ContentType_MAX = 0xffff + 2
};
# 21 "./wire/DataHeader.h" 2
# 32 "./wire/DataHeader.h"
struct DataHeader
{

    uint8_t versionAndFlags;

    uint8_t unused;

    uint16_t contentType_be;
};

void Assert_compileTime(char x[1 - (!(sizeof(struct DataHeader) == 4))]);




static inline enum ContentType DataHeader_getContentType(struct DataHeader* hdr)
{
    return ((uint16_t)(__extension__ ({ uint16_t _x = (((uint16_t)(hdr->contentType_be))); _x >> 8 | _x << 8; })));
}

static inline void DataHeader_setContentType(struct DataHeader* hdr, enum ContentType type)
{
    do { if (!((type <= 0xffff))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("DataHeader.h"), (54), "(type <= 0xffff)"); } } while (0);
    hdr->contentType_be = ((uint16_t)(__extension__ ({ uint16_t _x = (((uint16_t)(type))); _x >> 8 | _x << 8; })));
}

static inline void DataHeader_setVersion(struct DataHeader* hdr, uint8_t version)
{
    hdr->versionAndFlags = (hdr->versionAndFlags & 0x0f) | (version << 4);
}

static inline uint8_t DataHeader_getVersion(struct DataHeader* hdr)
{
    return hdr->versionAndFlags >> 4;
}
# 21 "./wire/PFChan.h" 2

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
# 23 "./wire/PFChan.h" 2

struct PFChan_Node
{
    uint8_t ip6[16];

    uint8_t publicKey[32];

    uint64_t path_be;


    uint32_t metric_be;

    uint32_t version_be;
};

void Assert_compileTime(char x[1 - (!(sizeof(struct PFChan_Node) == 64))]);

struct PFChan_Msg
{
    struct RouteHeader route;
    struct DataHeader data;

};

void Assert_compileTime(char x[1 - (!(sizeof(struct PFChan_Msg) == ((56 + 12) + 4)))]);
       

struct PFChan_CtrlMsg
{
    struct RouteHeader route;
    struct Control_Header ctrlHdr;
};

void Assert_compileTime(char x[1 - (!(sizeof(struct PFChan_Msg) == ((56 + 12) + 4)))]);
       

struct PFChan_Ping
{
    uint64_t cookie;
};

void Assert_compileTime(char x[1 - (!(sizeof(struct PFChan_Ping) == 8))]);

struct PFChan_Pathfinder_Connect
{





    uint32_t superiority_be;


    uint32_t version_be;


    uint8_t userAgent[64];
};

void Assert_compileTime(char x[1 - (!(sizeof(struct PFChan_Pathfinder_Connect) == 72))]);
# 91 "./wire/PFChan.h"
struct PFChan_Pathfinder_Superiority
{
    uint32_t superiority_be;
};

void Assert_compileTime(char x[1 - (!(sizeof(struct PFChan_Pathfinder_Superiority) == 4))])
                                                                                       ;

enum PFChan_Pathfinder
{

    PFChan_Pathfinder__TOO_LOW = 511,





    PFChan_Pathfinder_CONNECT = 512,





    PFChan_Pathfinder_SUPERIORITY = 513,







    PFChan_Pathfinder_NODE = 514,





    PFChan_Pathfinder_SENDMSG = 515,





    PFChan_Pathfinder_PING = 516,





    PFChan_Pathfinder_PONG = 517,







    PFChan_Pathfinder_SESSIONS = 518,





    PFChan_Pathfinder_PEERS = 519,





    PFChan_Pathfinder_PATHFINDERS = 520,






    PFChan_Pathfinder_CTRL_SENDMSG = 521,






    PFChan_Pathfinder_SNODE = 522,

    PFChan_Pathfinder__TOO_HIGH = 523,
};

struct PFChan_FromPathfinder
{
    enum PFChan_Pathfinder event_be;


    uint8_t target_be;

    union {
        struct PFChan_Pathfinder_Connect connect;
        struct PFChan_Pathfinder_Superiority superiority;
        struct PFChan_Node node;
        struct PFChan_Msg sendmsg;
        struct PFChan_Ping ping;
        struct PFChan_Ping pong;
        uint8_t bytes[1];
    } content;
};



enum PFChan_Core
{

    PFChan_Core__TOO_LOW = 1023,






    PFChan_Core_CONNECT = 1024,





    PFChan_Core_PATHFINDER = 1025,





    PFChan_Core_PATHFINDER_GONE = 1026,





    PFChan_Core_SWITCH_ERR = 1027,





    PFChan_Core_SEARCH_REQ = 1028,






    PFChan_Core_PEER = 1029,





    PFChan_Core_PEER_GONE = 1030,






    PFChan_Core_SESSION = 1031,





    PFChan_Core_SESSION_ENDED = 1032,





    PFChan_Core_DISCOVERED_PATH = 1033,





    PFChan_Core_MSG = 1034,






    PFChan_Core_PING = 1035,





    PFChan_Core_PONG = 1036,





    PFChan_Core_CTRL_MSG = 1037,






    PFChan_Core_UNSETUP_SESSION = 1038,






    PFChan_Core_LINK_STATE = 1039,

    PFChan_Core__TOO_HIGH = 1040,
};


struct PFChan_LinkState_Entry {
    uint64_t peerLabel;
    uint64_t sumOfPackets;
    uint64_t sumOfDrops;
    uint64_t sumOfKb;
};

void Assert_compileTime(char x[1 - (!(sizeof(struct PFChan_LinkState_Entry) == 32))]);

struct PFChan_Core_SearchReq
{
    uint8_t ipv6[16];

    uint32_t pad;

    uint32_t version_be;
};

void Assert_compileTime(char x[1 - (!(sizeof(struct PFChan_Core_SearchReq) == 24))]);

struct PFChan_Core_Pathfinder
{

    uint32_t superiority_be;


    uint32_t pathfinderId_be;


    uint8_t userAgent[64];
};

void Assert_compileTime(char x[1 - (!(sizeof(struct PFChan_Core_Pathfinder) == 72))]);

struct PFChan_Core_Connect
{

    uint32_t version_be;


    uint32_t pathfinderId_be;


    uint8_t publicKey[32];
};

void Assert_compileTime(char x[1 - (!(sizeof(struct PFChan_Core_Connect) == 40))]);

struct PFChan_Core_SwitchErr
{
    struct SwitchHeader sh;
    struct Control_Header ctrlHeader;
    struct Control_Error ctrlErr;
    struct SwitchHeader shAtErrorHop;
};
       


void Assert_compileTime(char x[1 - (!(sizeof(struct PFChan_Core_SwitchErr) == (12 + 4 + (4 + 12 + 4) + 12)))]);

struct PFChan_FromCore
{
    enum PFChan_Core event_be;


    uint8_t target_be;

    union {
        struct PFChan_Core_Connect connect;
        struct PFChan_Core_Pathfinder pathfinder;
        struct PFChan_Core_Pathfinder pathfinderGone;
        struct PFChan_Core_SwitchErr switchErr;
        struct PFChan_Core_SearchReq searchReq;
        struct PFChan_Node peer;
        struct PFChan_Node peerGone;
        struct PFChan_Node session;
        struct PFChan_Node sessionEnded;
        struct PFChan_Node discoveredPath;
        struct PFChan_Msg msg;
        struct PFChan_Ping ping;
        struct PFChan_Ping pong;
        struct PFChan_LinkState_Entry linkState;
        uint8_t bytes[4];
    } content;
};

       
# 21 "./net/EventEmitter.h" 2


struct Linker_x012744933813954207;

struct EventEmitter
{
    int unused;
};






void EventEmitter_regCore(struct EventEmitter* ee,
                          struct Iface* iface,
                          enum PFChan_Pathfinder ev);

void EventEmitter_regPathfinderIface(struct EventEmitter* ee, struct Iface* iface);

struct EventEmitter* EventEmitter_new(struct Allocator* alloc, struct Log* log, uint8_t* publicKey);
# 21 "./net/ControlHandler.h" 2

struct Linker_x07645147236142027;

struct ControlHandler
{

    struct Iface coreIf;





    struct Iface switchPingerIf;
};

struct ControlHandler* ControlHandler_new(struct Allocator* alloc,
                                          struct Log* logger,
                                          struct EventEmitter* ee,
                                          uint8_t myPublicKey[32]);
# 26 "./net/NetCore.h" 2
# 1 "./net/InterfaceController.h" 1
# 29 "./net/InterfaceController.h"
struct Linker_x01741375353072554;




enum InterfaceController_PeerState
{




    InterfaceController_PeerState_INIT = CryptoAuth_State_INIT,

    InterfaceController_PeerState_SENT_HELLO = CryptoAuth_State_SENT_HELLO,

    InterfaceController_PeerState_RECEIVED_HELLO = CryptoAuth_State_RECEIVED_HELLO,

    InterfaceController_PeerState_SENT_KEY = CryptoAuth_State_SENT_KEY,

    InterfaceController_PeerState_RECEIVED_KEY = CryptoAuth_State_RECEIVED_KEY,


    InterfaceController_PeerState_ESTABLISHED = CryptoAuth_State_ESTABLISHED,


    InterfaceController_PeerState_UNRESPONSIVE = -1,


    InterfaceController_PeerState_UNAUTHENTICATED = -2,
};

static inline char* InterfaceController_stateString(enum InterfaceController_PeerState ps)
{
    switch (ps) {
        case InterfaceController_PeerState_INIT: return "INIT";
        case InterfaceController_PeerState_SENT_HELLO: return "SENT_HELLO";
        case InterfaceController_PeerState_RECEIVED_HELLO: return "RECEIVED_HELLO";
        case InterfaceController_PeerState_SENT_KEY: return "SENT_KEY";
        case InterfaceController_PeerState_RECEIVED_KEY: return "RECEIVED_KEY";
        case InterfaceController_PeerState_ESTABLISHED: return "ESTABLISHED";
        case InterfaceController_PeerState_UNRESPONSIVE: return "UNRESPONSIVE";
        case InterfaceController_PeerState_UNAUTHENTICATED: return "UNAUTHENTICATED";
        default: return "INVALID";
    }
}

enum InterfaceController_BeaconState
{
    InterfaceController_BeaconState_DISABLED,
    InterfaceController_BeaconState_ACCEPTING,
    InterfaceController_BeaconState_SENDING
};

static inline char* InterfaceController_beaconStateString(enum InterfaceController_BeaconState bs)
{
    switch (bs) {
        case InterfaceController_BeaconState_DISABLED: return "DISABLED";
        case InterfaceController_BeaconState_ACCEPTING: return "ACCEPTING";
        case InterfaceController_BeaconState_SENDING: return "SENDING";
        default: return "INVALID";
    }
}




struct InterfaceController_PeerStats
{
    struct Address addr;
    struct Sockaddr* lladdr;
    int state;
    int ifNum;
    uint64_t timeOfLastMessage;
    uint64_t bytesOut;
    uint64_t bytesIn;
    
# 104 "./net/InterfaceController.h" 3 4
   _Bool 
# 104 "./net/InterfaceController.h"
        isIncomingConnection;
    String* user;


    uint32_t duplicates;
    uint32_t lostPackets;
    uint32_t receivedPackets;
    uint32_t receivedOutOfRange;

    uint32_t sendKbps;
    uint32_t recvKbps;
};

struct InterfaceController
{





    
# 124 "./net/InterfaceController.h" 3 4
   _Bool 
# 124 "./net/InterfaceController.h"
        timestampPackets;
};

struct InterfaceController_Iface
{
    struct Iface addrIf;


    int ifNum;

    enum InterfaceController_BeaconState beaconState;

    String* name;
};
# 148 "./net/InterfaceController.h"
struct InterfaceController_Iface* InterfaceController_newIface(struct InterfaceController* ifc,
                                                 String* name,
                                                 struct Allocator* alloc);


int InterfaceController_ifaceCount(struct InterfaceController* ifc);


struct InterfaceController_Iface* InterfaceController_getIface(struct InterfaceController* ifc,
                                                               int ifNum);
# 186 "./net/InterfaceController.h"
int InterfaceController_bootstrapPeer(struct InterfaceController* ifc,
                                      int interfaceNumber,
                                      uint8_t* herPublicKey,
                                      const struct Sockaddr* lladdr,
                                      String* password,
                                      String* login,
                                      String* displayName,
                                      struct Allocator* alloc);






int InterfaceController_beaconState(struct InterfaceController* ifc,
                                    int interfaceNumber,
                                    int newState);
# 211 "./net/InterfaceController.h"
void InterfaceController_resetPeering(struct InterfaceController* ifController,
                                      uint8_t herPublicKey[32]);
# 223 "./net/InterfaceController.h"
int InterfaceController_disconnectPeer(struct InterfaceController* ifc, uint8_t herPublicKey[32]);
# 233 "./net/InterfaceController.h"
int InterfaceController_getPeerStats(struct InterfaceController* ic,
                              struct Allocator* alloc,
                              struct InterfaceController_PeerStats** statsOut);

struct InterfaceController* InterfaceController_new(struct CryptoAuth* ca,
                                      struct SwitchCore* switchCore,
                                      struct Log* logger,
                                      struct EventBase* eventBase,
                                      struct SwitchPinger* switchPinger,
                                      struct Random* rand,
                                      struct Allocator* allocator,
                                      struct EventEmitter* ee);
# 27 "./net/NetCore.h" 2

# 1 "./tunnel/IpTunnel.h" 1
# 24 "./tunnel/IpTunnel.h"
# 1 "./util/GlobalConfig.h" 1
# 21 "./util/GlobalConfig.h"
struct Linker_x07524225298172855;
# 38 "./util/GlobalConfig.h"
struct GlobalConfig {
    int unused;
};

String* GlobalConfig_getTunName(struct GlobalConfig* conf);
void GlobalConfig_setTunName(struct GlobalConfig* conf, String* name);

struct GlobalConfig* GlobalConfig_new(struct Allocator* alloc);
# 25 "./tunnel/IpTunnel.h" 2

# 1 "./tunnel/RouteGen.h" 1
# 23 "./tunnel/RouteGen.h"
struct Linker_x021162699027048748;



struct RouteGen
{
    
# 29 "./tunnel/RouteGen.h" 3 4
   _Bool 
# 29 "./tunnel/RouteGen.h"
        hasUncommittedChanges;
};

void RouteGen_addException(struct RouteGen* rg, struct Sockaddr* exempt);

void RouteGen_addPrefix(struct RouteGen* rg, struct Sockaddr* destination);

void RouteGen_addLocalPrefix(struct RouteGen* rg, struct Sockaddr* destination);

__attribute__ ((warn_unused_result)) struct Er_Ret* RouteGen_commit(struct RouteGen* rg, const char* tunName, struct Allocator* tempAlloc)

                                                           ;

Dict* RouteGen_getPrefixes(struct RouteGen* rg, struct Allocator* alloc);

Dict* RouteGen_getLocalPrefixes(struct RouteGen* rg, struct Allocator* alloc);

Dict* RouteGen_getExceptions(struct RouteGen* rg, struct Allocator* alloc);

Dict* RouteGen_getGeneratedRoutes(struct RouteGen* rg, struct Allocator* alloc);


# 50 "./tunnel/RouteGen.h" 3 4
_Bool 
# 50 "./tunnel/RouteGen.h"
    RouteGen_removePrefix(struct RouteGen* rg, struct Sockaddr* toRemove);


# 52 "./tunnel/RouteGen.h" 3 4
_Bool 
# 52 "./tunnel/RouteGen.h"
    RouteGen_removeLocalPrefix(struct RouteGen* rg, struct Sockaddr* toRemove);


# 54 "./tunnel/RouteGen.h" 3 4
_Bool 
# 54 "./tunnel/RouteGen.h"
    RouteGen_removeException(struct RouteGen* rg, struct Sockaddr* toRemove);

struct RouteGen* RouteGen_new(struct Allocator* allocator, struct Log* log);
# 27 "./tunnel/IpTunnel.h" 2

struct Linker_x0002979821965248597;

struct IpTunnel_Connection
{

    struct RouteHeader routeHeader;


    uint8_t connectionIp6[16];


    uint8_t connectionIp4[4];


    uint8_t connectionIp6Prefix;


    uint8_t connectionIp6Alloc;


    uint8_t connectionIp4Prefix;


    uint8_t connectionIp4Alloc;


    int isOutgoing : 1;


    int number : 31;
};

struct IpTunnel
{

    struct Iface tunInterface;





    struct Iface nodeInterface;





    struct {
        uint32_t count;
        struct IpTunnel_Connection* connections;
    } connectionList;
};
# 90 "./tunnel/IpTunnel.h"
struct IpTunnel* IpTunnel_new(struct Log* logger,
                              struct EventBase* eventBase,
                              struct Allocator* alloc,
                              struct Random* rand,
                              struct RouteGen* rg,
                              struct GlobalConfig* globalConf);
# 110 "./tunnel/IpTunnel.h"
int IpTunnel_allowConnection(uint8_t publicKeyOfAuthorizedNode[32],
                             struct Sockaddr* ip6Addr,
                             uint8_t ip6Prefix, uint8_t ip6Alloc,
                             struct Sockaddr* ip4Addr,
                             uint8_t ip4Prefix, uint8_t ip4Alloc,
                             struct IpTunnel* tunnel);
# 124 "./tunnel/IpTunnel.h"
int IpTunnel_connectTo(uint8_t publicKeyOfNodeToConnectTo[32], struct IpTunnel* tunnel);
# 135 "./tunnel/IpTunnel.h"
int IpTunnel_removeConnection(int connectionNumber, struct IpTunnel* tunnel);
# 29 "./net/NetCore.h" 2

# 1 "./net/SessionManager.h" 1
# 24 "./net/SessionManager.h"
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
# 25 "./net/SessionManager.h" 2

struct Linker_x08931200408158892;
# 37 "./net/SessionManager.h"
struct SessionManager
{

    struct Iface switchIf;
# 51 "./net/SessionManager.h"
    struct Iface insideIf;





    int maxBufferedMessages;



    int64_t sessionTimeoutMilliseconds;







    int64_t sessionSearchAfterMilliseconds;
};

struct SessionManager_Session
{
    struct CryptoAuth_Session* caSession;





    int64_t timeOfKeepAliveIn;


    int64_t timeOfLastIn;


    int64_t timeOfLastOut;

    uint64_t bytesOut;

    uint64_t bytesIn;


    int64_t lastSearchTime;


    uint32_t receiveHandle;


    uint32_t sendHandle;


    uint32_t version;

    uint32_t metric;


    uint64_t sendSwitchLabel;


    uint64_t recvSwitchLabel;


    int maintainSession;
};

struct SessionManager_HandleList
{
    int length;
    uint32_t* handles;
};
# 129 "./net/SessionManager.h"
struct SessionManager_Session* SessionManager_sessionForHandle(uint32_t handle,
                                                               struct SessionManager* sm);

struct SessionManager_Session* SessionManager_sessionForIp6(uint8_t* lookupKey,
                                                            struct SessionManager* sm);




struct SessionManager_HandleList* SessionManager_getHandleList(struct SessionManager* sm,
                                                               struct Allocator* alloc);

struct SessionManager* SessionManager_new(struct Allocator* alloc,
                                          struct EventBase* eventBase,
                                          struct CryptoAuth* cryptoAuth,
                                          struct Random* rand,
                                          struct Log* log,
                                          struct EventEmitter* ee);
# 31 "./net/NetCore.h" 2
# 1 "./net/UpperDistributor.h" 1
# 23 "./net/UpperDistributor.h"
struct Linker_x07438682718852694;





struct UpperDistributor
{
    struct Iface sessionManagerIf;

    struct Iface tunAdapterIf;

    struct Iface ipTunnelIf;

    struct Iface controlHandlerIf;
};

struct UpperDistributor_Handler
{
    enum ContentType type;
    int udpPort;
};





int UpperDistributor_unregisterHandler(struct UpperDistributor* ud, int regNum);





int UpperDistributor_listHandlers(struct UpperDistributor* ud,
                                  struct UpperDistributor_Handler** outputList,
                                  struct Allocator* alloc);
# 67 "./net/UpperDistributor.h"
int UpperDistributor_registerHandler(struct UpperDistributor* ud,
                                     enum ContentType ct,
                                     int udpPort);

struct UpperDistributor* UpperDistributor_new(struct Allocator* alloc,
                                              struct Log* log,
                                              struct EventEmitter* ee,
                                              struct Address* myAddress);
# 32 "./net/NetCore.h" 2
# 1 "./net/TUNAdapter.h" 1
# 22 "./net/TUNAdapter.h"
struct Linker_x0005825798193339393;

struct TUNAdapter
{
    struct Iface upperDistributorIf;

    struct Iface tunIf;

    struct Iface ipTunnelIf;
};

struct TUNAdapter* TUNAdapter_new(struct Allocator* alloc, struct Log* log, uint8_t myAddr[16]);
# 33 "./net/NetCore.h" 2

struct Linker_x02632015956186595;

struct NetCore
{
    struct Allocator* alloc;
    struct EventBase* base;
    struct Random* rand;
    struct Log* log;
    struct CryptoAuth* ca;
    struct EventEmitter* ee;
    struct Address* myAddress;
    struct SwitchCore* switchCore;
    struct ControlHandler* controlHandler;
    struct SwitchPinger* sp;
    struct InterfaceController* ifController;
    struct SessionManager* sm;
    struct UpperDistributor* upper;
    struct TUNAdapter* tunAdapt;
};

struct NetCore* NetCore_new(uint8_t* privateKey,
                            struct Allocator* alloc,
                            struct EventBase* base,
                            struct Random* rand,
                            struct Log* log);
# 20 "./test/TestFramework.h" 2

struct Linker_x0789512692346392;

struct TestFramework
{
    struct Allocator* alloc;
    struct Random* rand;
    struct EventBase* eventBase;
    struct Log* logger;


    struct Pathfinder* pathfinder;

    struct SubnodePathfinder* subnodePathfinder;

    struct Iface* tunIf;
    struct NetCore* nc;
    struct EncodingScheme* scheme;


    struct Message* lastMsg;





    struct Message* lastMsgBackup;

    uint8_t* publicKey;
    uint8_t* ip;

    unsigned long Identity_verifier;
};




struct TestFramework* TestFramework_setUp(char* privateKey,
                                          struct Allocator* allocator,
                                          struct EventBase* base,
                                          struct Random* rand,
                                          struct Log* logger);

void TestFramework_linkNodes(struct TestFramework* client,
                             struct TestFramework* server,
                             
# 65 "./test/TestFramework.h" 3 4
                            _Bool 
# 65 "./test/TestFramework.h"
                                 beacon);

void TestFramework_craftIPHeader(struct Message* msg, uint8_t srcAddr[16], uint8_t destAddr[16]);


void TestFramework_assertLastMessageUnaltered(struct TestFramework* tf);

int TestFramework_peerCount(struct TestFramework* node);

int TestFramework_sessionCount(struct TestFramework* node);
# 29 "test/TestFramework.c" 2
# 1 "./util/log/WriterLog.h" 1
# 22 "./util/log/WriterLog.h"
struct Linker_x014917360264755075;

struct Log* WriterLog_new(struct Writer* w, struct Allocator* alloc);
# 30 "test/TestFramework.c" 2





# 1 "./interface/ASynchronizer.h" 1
# 23 "./interface/ASynchronizer.h"
struct Linker_x03675674018482864;

struct ASynchronizer
{
    struct Iface ifA;
    struct Iface ifB;
};

struct ASynchronizer* ASynchronizer_new(struct Allocator* alloc,
                                        struct EventBase* base,
                                        struct Log* log);
# 36 "test/TestFramework.c" 2






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
# 43 "test/TestFramework.c" 2



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
# 47 "test/TestFramework.c" 2

struct TestFramework_Link
{
    struct Iface clientIf;
    struct Iface serverIf;
    struct TestFramework* client;
    struct TestFramework* server;
    int serverIfNum;
    int clientIfNum;
    unsigned long Identity_verifier;
};

static __attribute__ ((warn_unused_result)) struct Iface* sendTo(struct Message* msg,
                          struct Iface* dest,
                          struct TestFramework* srcTf,
                          struct TestFramework* destTf)
{
    do { if (!((!((uintptr_t)msg->bytes % 4) || !"alignment fault"))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("TestFramework.c"), (64), "(!((uintptr_t)msg->bytes % 4) || !\"alignment fault\")"); } } while (0);
    do { if (!((!(msg->capacity % 4) || !"length fault"))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("TestFramework.c"), (65), "(!(msg->capacity % 4) || !\"length fault\")"); } } while (0);
    do { if (!((((int)msg->capacity >= msg->length) || !"length fault0"))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("TestFramework.c"), (66), "(((int)msg->capacity >= msg->length) || !\"length fault0\")"); } } while (0);

    do { if (srcTf->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(srcTf->logger, Log_Level_DEBUG, "TestFramework.c", 68, "Transferring message to [%p] - message length [%d]\n", (void*)dest, msg->length); } } while (0)
                                       ;


    srcTf->lastMsgBackup = Message_clone(msg, srcTf->alloc);
    srcTf->lastMsg = msg;
    if (msg->alloc) {


        Allocator__adopt((srcTf->alloc),(msg->alloc),"TestFramework.c",77);
    }



    struct Message* sendMsg = Message_clone(msg, destTf->alloc);
    Iface_send(dest, sendMsg);
    return 0;
}

static __attribute__ ((warn_unused_result)) struct Iface* sendClient(struct Message* msg, struct Iface* clientIf)
{
    struct TestFramework_Link* link =
        (__extension__ ({ const __typeof__(((struct TestFramework_Link*)0)->clientIf)*__mptr = (clientIf); (__extension__ ({ __typeof__((struct TestFramework_Link*)((char*)__mptr - 
# 90 "test/TestFramework.c" 3 4
       __builtin_offsetof (
# 90 "test/TestFramework.c"
       struct TestFramework_Link
# 90 "test/TestFramework.c" 3 4
       , 
# 90 "test/TestFramework.c"
       clientIf
# 90 "test/TestFramework.c" 3 4
       )
# 90 "test/TestFramework.c"
       )) Identity_ptr = ((struct TestFramework_Link*)((char*)__mptr - 
# 90 "test/TestFramework.c" 3 4
       __builtin_offsetof (
# 90 "test/TestFramework.c"
       struct TestFramework_Link
# 90 "test/TestFramework.c" 3 4
       , 
# 90 "test/TestFramework.c"
       clientIf
# 90 "test/TestFramework.c" 3 4
       )
# 90 "test/TestFramework.c"
       )); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x94af82cdfb404f3bull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("TestFramework.c"), (90), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x94af82cdfb404f3bull))"); } } while (0); Identity_ptr; })); }));
    return sendTo(msg, &link->serverIf, link->client, link->server);
}

static __attribute__ ((warn_unused_result)) struct Iface* sendServer(struct Message* msg, struct Iface* serverIf)
{
    struct TestFramework_Link* link =
        (__extension__ ({ const __typeof__(((struct TestFramework_Link*)0)->serverIf)*__mptr = (serverIf); (__extension__ ({ __typeof__((struct TestFramework_Link*)((char*)__mptr - 
# 97 "test/TestFramework.c" 3 4
       __builtin_offsetof (
# 97 "test/TestFramework.c"
       struct TestFramework_Link
# 97 "test/TestFramework.c" 3 4
       , 
# 97 "test/TestFramework.c"
       serverIf
# 97 "test/TestFramework.c" 3 4
       )
# 97 "test/TestFramework.c"
       )) Identity_ptr = ((struct TestFramework_Link*)((char*)__mptr - 
# 97 "test/TestFramework.c" 3 4
       __builtin_offsetof (
# 97 "test/TestFramework.c"
       struct TestFramework_Link
# 97 "test/TestFramework.c" 3 4
       , 
# 97 "test/TestFramework.c"
       serverIf
# 97 "test/TestFramework.c" 3 4
       )
# 97 "test/TestFramework.c"
       )); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x94af82cdfb404f3bull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("TestFramework.c"), (97), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x94af82cdfb404f3bull))"); } } while (0); Identity_ptr; })); }));
    return sendTo(msg, &link->clientIf, link->server, link->client);
}

struct TestFramework* TestFramework_setUp(char* privateKey,
                                          struct Allocator* allocator,
                                          struct EventBase* base,
                                          struct Random* rand,
                                          struct Log* logger)
{
    if (!logger) {
        struct Writer* logwriter = FileWriter_new(
# 108 "test/TestFramework.c" 3
                                                 (__acrt_iob_func(1))
# 108 "test/TestFramework.c"
                                                       , allocator);
        logger = WriterLog_new(logwriter, allocator);
    }

    if (!rand) {
        rand = Random_new(allocator, logger, 
# 113 "test/TestFramework.c" 3 4
                                            ((void *)0)
# 113 "test/TestFramework.c"
                                                );
    }

    if (!base) {
        base = EventBase_new(allocator);
    }

    uint64_t pks[4];
    if (!privateKey) {
        Random_longs(rand, pks, 4);
        privateKey = (char*)pks;
    }

    struct EncodingScheme* scheme = NumberCompress_v3x5x8_defineScheme(allocator);

    struct NetCore* nc =
        NetCore_new(privateKey, allocator, base, rand, logger);

    struct RouteGen* rg = RouteGen_new(allocator, logger);

    struct GlobalConfig* globalConf = GlobalConfig_new(allocator);
    struct IpTunnel* ipTunnel =
        IpTunnel_new(logger, base, allocator, rand, rg, globalConf);
    Iface_plumb(&nc->tunAdapt->ipTunnelIf, &ipTunnel->tunInterface);
    Iface_plumb(&nc->upper->ipTunnelIf, &ipTunnel->nodeInterface);

    struct SubnodePathfinder* spf = SubnodePathfinder_new(
        allocator, logger, base, rand, nc->myAddress, privateKey, scheme);
    struct ASynchronizer* spfAsync = ASynchronizer_new(allocator, base, logger);
    Iface_plumb(&spfAsync->ifA, &spf->eventIf);
    EventEmitter_regPathfinderIface(nc->ee, &spfAsync->ifB);


        struct Pathfinder* pf = Pathfinder_register(allocator, logger, base, rand, 
# 146 "test/TestFramework.c" 3 4
                                                                                  ((void *)0)
# 146 "test/TestFramework.c"
                                                                                      );
        pf->fullVerify = 
# 147 "test/TestFramework.c" 3 4
                        1
# 147 "test/TestFramework.c"
                            ;
        struct ASynchronizer* pfAsync = ASynchronizer_new(allocator, base, logger);
        Iface_plumb(&pfAsync->ifA, &pf->eventIf);
        EventEmitter_regPathfinderIface(nc->ee, &pfAsync->ifB);


    SubnodePathfinder_start(spf);

    struct TestFramework* tf = Allocator__calloc((allocator),(sizeof(struct TestFramework)),(1),"TestFramework.c",155);
    (tf)->Identity_verifier = ((unsigned long) 0x94af82cdfb404f3bull);
    tf->alloc = allocator;
    tf->rand = rand;
    tf->eventBase = base;
    tf->logger = logger;
    tf->nc = nc;
    tf->tunIf = &nc->tunAdapt->tunIf;
    tf->publicKey = nc->myAddress->key;
    tf->ip = nc->myAddress->ip6.bytes;

        tf->pathfinder = pf;

    tf->subnodePathfinder = spf;
    tf->scheme = scheme;

    return tf;
}

void TestFramework_assertLastMessageUnaltered(struct TestFramework* tf)
{
    if (!tf->lastMsg) {
        return;
    }
    struct Message* a = tf->lastMsg;
    struct Message* b = tf->lastMsgBackup;
    do { if (!((a->length == b->length))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("TestFramework.c"), (181), "(a->length == b->length)"); } } while (0);
    do { if (!((a->padding == b->padding))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("TestFramework.c"), (182), "(a->padding == b->padding)"); } } while (0);
    do { if (!((!__builtin_memcmp(a->bytes,b->bytes,a->length)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("TestFramework.c"), (183), "(!__builtin_memcmp(a->bytes,b->bytes,a->length))"); } } while (0);
}

void TestFramework_linkNodes(struct TestFramework* client,
                             struct TestFramework* server,
                             
# 188 "test/TestFramework.c" 3 4
                            _Bool 
# 188 "test/TestFramework.c"
                                 beacon)
{

    struct TestFramework_Link* link =
        Allocator__calloc((client->alloc),(sizeof(struct TestFramework_Link)),(1),"TestFramework.c",192);
    (link)->Identity_verifier = ((unsigned long) 0x94af82cdfb404f3bull);
    link->clientIf.send = sendClient;
    link->serverIf.send = sendServer;
    link->client = client;
    link->server = server;

    struct InterfaceController_Iface* clientIci = InterfaceController_newIface(
        client->nc->ifController, (&(String) { .bytes = "client", .len = CString_strlen("client") }), client->alloc);
    link->clientIfNum = clientIci->ifNum;
    Iface_plumb(&link->clientIf, &clientIci->addrIf);

    struct InterfaceController_Iface* serverIci = InterfaceController_newIface(
        server->nc->ifController, (&(String) { .bytes = "server", .len = CString_strlen("server") }), server->alloc);
    link->serverIfNum = serverIci->ifNum;
    Iface_plumb(&link->serverIf, &serverIci->addrIf);

    if (beacon) {
        int ret = InterfaceController_beaconState(client->nc->ifController,
                                           link->clientIfNum,
                                           1);
        do { if (!((!ret))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("TestFramework.c"), (213), "(!ret)"); } } while (0);

        ret = InterfaceController_beaconState(server->nc->ifController,
                                       link->serverIfNum,
                                       2);
        do { if (!((!ret))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("TestFramework.c"), (218), "(!ret)"); } } while (0);
    } else {

        CryptoAuth_addUser((&(String) { .bytes = "abcdefg123", .len = CString_strlen("abcdefg123") }), (&(String) { .bytes = "TEST", .len = CString_strlen("TEST") }), server->nc->ca);


        InterfaceController_bootstrapPeer(client->nc->ifController,
                                   link->clientIfNum,
                                   server->publicKey,
                                   (0 ? Sockaddr_LOOPBACK_be : Sockaddr_LOOPBACK_le),
                                   (&(String) { .bytes = "abcdefg123", .len = CString_strlen("abcdefg123") }),
                                   
# 229 "test/TestFramework.c" 3 4
                                  ((void *)0)
# 229 "test/TestFramework.c"
                                      ,
                                   
# 230 "test/TestFramework.c" 3 4
                                  ((void *)0)
# 230 "test/TestFramework.c"
                                      ,
                                   client->alloc);
    }
}

void TestFramework_craftIPHeader(struct Message* msg, uint8_t srcAddr[16], uint8_t destAddr[16])
{
    do {             struct Er_Ret* Er_ret = Message_eshift(msg, 40);                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("TestFramework.c", 237, Er_ret);                 } else {                     Er__assertFail(Er_unwind("TestFramework.c", 237, Er_ret));                 }             }                  } while (0);
    struct Headers_IP6Header* ip = (struct Headers_IP6Header*) msg->bytes;

    ip->versionClassAndFlowLabel = 0;
    ip->flowLabelLow_be = 0;
    ip->payloadLength_be = ((uint16_t)(__extension__ ({ uint16_t _x = (((uint16_t)(msg->length - 40))); _x >> 8 | _x << 8; })));
    ip->nextHeader = 123;
    ip->hopLimit = 255;
    Bits__memcpy(ip->sourceAddr,srcAddr,16,"TestFramework.c",245);
    Bits__memcpy(ip->destinationAddr,destAddr,16,"TestFramework.c",246);
    (((uint8_t*) ip)[0] |= ( (sizeof(*ip) == 20) ? 4 : 6 ) << 4);
}

int TestFramework_peerCount(struct TestFramework* node)
{
    struct Allocator* alloc = Allocator__child((node->alloc),"TestFramework.c",252);
    struct InterfaceController_PeerStats* statsOut = 
# 253 "test/TestFramework.c" 3 4
                                                    ((void *)0)
# 253 "test/TestFramework.c"
                                                        ;
    int len = InterfaceController_getPeerStats(node->nc->ifController, alloc, &statsOut);
    int out = 0;
    for (int i = 0; i < len; i++) {
        out += (statsOut[i].state == InterfaceController_PeerState_ESTABLISHED);
    }
    Allocator__free((alloc),"TestFramework.c",259);
    return out;
}

int TestFramework_sessionCount(struct TestFramework* node)
{
    struct Allocator* alloc = Allocator__child((node->alloc),"TestFramework.c",265);
    struct SessionManager_HandleList* list = SessionManager_getHandleList(node->nc->sm, alloc);
    int count = 0;
    for (int i = 0; i < list->length; i++) {
        struct SessionManager_Session* sess =
            SessionManager_sessionForHandle(list->handles[i], node->nc->sm);
        count += (CryptoAuth_getState(sess->caSession) == CryptoAuth_State_ESTABLISHED);
    }
    Allocator__free((alloc),"TestFramework.c",273);
    return count;
}
