# 1 "./switch/test/EncodingScheme_test.c"
# 1 "/home/jojapoppa/cjdns//"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "./switch/test/EncodingScheme_test.c"
# 15 "./switch/test/EncodingScheme_test.c"
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
struct Linker_x0175522062473225;
# 33 "./util/Assert.h"
__attribute__((__format__ (__printf__, 1, 2)))
__attribute__((__noreturn__))
void Assert_failure(const char* format, ...);
# 19 "./util/Identity.h" 2
# 1 "./util/Constant.h" 1
# 33 "./util/Constant.h"

# 20 "./util/Identity.h" 2



    
# 19 "./memory/Allocator.h" 2


struct Linker_x03473653262937928;





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
struct Linker_x022516154695802015;

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

struct Linker_x08328684033299936;

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
# 16 "./switch/test/EncodingScheme_test.c" 2
# 1 "./crypto/random/Random.h" 1
# 19 "./crypto/random/Random.h"
# 1 "./exception/Except.h" 1
# 18 "./exception/Except.h"
# 1 "./exception/Er.h" 1
# 23 "./exception/Er.h"
struct Linker_x07019798558196486;

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


struct Linker_x04115477268639951;



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
struct Linker_x05162634637608856;

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
struct Linker_x05181384889327505;



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

struct Linker_x07697428819278067;



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
# 17 "./switch/test/EncodingScheme_test.c" 2
# 1 "./switch/EncodingScheme.h" 1
# 19 "./switch/EncodingScheme.h"
# 1 "./benc/List.h" 1
# 21 "./benc/List.h"
struct Linker_x0810536811280929;

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


struct Linker_x0049743653829681156;



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
# 18 "./switch/test/EncodingScheme_test.c" 2

# 1 "./switch/NumberCompress.h" 1
# 23 "./switch/NumberCompress.h"
# 1 "./util/Bits.h" 1
# 21 "./util/Bits.h"
struct Linker_x02552972923558674;


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
# 24 "./switch/NumberCompress.h" 2
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
# 20 "./switch/test/EncodingScheme_test.c" 2

# 1 "./memory/MallocAllocator.h" 1
# 21 "./memory/MallocAllocator.h"
struct Linker_x016665143232899116;
# 30 "./memory/MallocAllocator.h"
struct Allocator* MallocAllocator__new(unsigned long sizeLimit, const char* file, int line);
# 22 "./switch/test/EncodingScheme_test.c" 2





# 1 "./util/Order.h" 1
# 19 "./util/Order.h"
struct Linker_x007679541218785801;

# 1 "/usr/lib/gcc/x86_64-w64-mingw32/9.2-win32/include/stddef.h" 1 3 4
# 1 "/usr/share/mingw-w64/include/stddef.h" 1 3 4
# 2 "/usr/lib/gcc/x86_64-w64-mingw32/9.2-win32/include/stddef.h" 2 3 4
# 22 "./util/Order.h" 2
typedef int (* Order_Comparator)(const void* a, const void* b);
void Order_qsort(void* base, size_t num, size_t size, Order_Comparator compare);
# 39 "./util/Order.h"
static inline int compareEncodingForms(const struct EncodingScheme_Form* one, const struct EncodingScheme_Form* two);

static inline void Order_OfEncodingForms_qsort(struct EncodingScheme_Form* array, int size)
{
    Order_qsort(array, size, sizeof(struct EncodingScheme_Form), (Order_Comparator) compareEncodingForms);
}
# 28 "./switch/test/EncodingScheme_test.c" 2
static inline int compareEncodingForms(const struct EncodingScheme_Form* a,
                                       const struct EncodingScheme_Form* b)
{
    int diff = a->bitCount;
    diff -= b->bitCount;
    return diff == 0 ? 0 : diff > 0 ? 1 : -1;
}

static void randomForm(struct EncodingScheme_Form* form, struct Random* rand)
{
    for (;;) {
        Random_bytes(rand, (uint8_t*)form, sizeof(struct EncodingScheme_Form));

        form->bitCount &= ((1<<5)-1);
        if (!form->bitCount) {
            form->bitCount++;
        }
        form->prefixLen &= ((1<<5)-1);
        if (!form->prefixLen) {
            form->prefixLen++;
        }
        if (EncodingScheme_formSize(form) > 59) { continue; }
        if (form->prefixLen > 3 && (form->prefix & 0xf) == 1) { continue; }
        break;
    }
    form->prefix &= ((((uint64_t)1)<<form->prefixLen)-1);
}

static struct EncodingScheme* randomScheme(struct Random* rand, struct Allocator* alloc)
{
    struct EncodingScheme* out =
        Allocator__malloc((alloc),(sizeof(struct EncodingScheme)),"EncodingScheme_test.c",59);
    do {
        out->count = Random_uint32(rand) % 32;
    } while (out->count < 2);
    out->forms = Allocator__malloc((alloc),(sizeof(struct EncodingScheme_Form) * out->count),"EncodingScheme_test.c",63);
    for (int i = 0; i < (int)out->count; i++) {
        randomForm(&out->forms[i], rand);
        for (int j = 0; j < i; j++) {
            if (out->forms[i].bitCount == out->forms[j].bitCount) {
                i--;
                break;
            }
            int minPfx = (out->forms[i].prefixLen < out->forms[j].prefixLen)
                ? out->forms[i].prefixLen : out->forms[j].prefixLen;
            if (((out->forms[j].prefix ^ out->forms[i].prefix) & ((1<<minPfx)-1)) == 0) {

                if (j != i-1) {
                    Bits__memcpy(&out->forms[j],&out->forms[i-1],sizeof(struct EncodingScheme_Form),"EncodingScheme_test.c",76)

                                                                        ;
                }
                i -= 2;
                break;
            }
        }
    }
    Order_OfEncodingForms_qsort(out->forms, out->count);
    return out;
}

static void assertEqual(struct EncodingScheme* a,
                        struct EncodingScheme* b)
{
    do { if (!((b))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("EncodingScheme_test.c"), (92), "(b)"); } } while (0);
    do { if (!((a->count == b->count))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("EncodingScheme_test.c"), (93), "(a->count == b->count)"); } } while (0);
    do { if (!((!__builtin_memcmp(a->forms,b->forms,sizeof(struct EncodingScheme_Form) * a->count)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("EncodingScheme_test.c"), (94), "(!__builtin_memcmp(a->forms,b->forms,sizeof(struct EncodingScheme_Form) * a->count))"); } } while (0)
                                                                                        ;
}

static void randomTest(struct Allocator* parent, struct Random* rand)
{
    struct Allocator* alloc = Allocator__child((parent),"EncodingScheme_test.c",100);
    struct EncodingScheme* control = randomScheme(rand, alloc);
    String* data = EncodingScheme_serialize(control, alloc);
    struct EncodingScheme* test = EncodingScheme_deserialize(data, alloc);
    assertEqual(control, test);
    Allocator__free((alloc),"EncodingScheme_test.c",105);
}


static void fuzzTest(struct Allocator* parent, struct Random* rand)
{
    struct Allocator* alloc = Allocator__child((parent),"EncodingScheme_test.c",111);
    String* data = String_newBinary(
# 112 "./switch/test/EncodingScheme_test.c" 3 4
                                   ((void *)0)
# 112 "./switch/test/EncodingScheme_test.c"
                                       , Random_uint32(rand) % 1024, alloc);
    Random_bytes(rand, (uint8_t*)data->bytes, data->len);
    EncodingScheme_deserialize(data, alloc);
    Allocator__free((alloc),"EncodingScheme_test.c",115);
}




static void encoding(struct Allocator* parent)
{
    struct EncodingScheme_Form forms[3] = {
        {
            .prefixLen = 15,
            .bitCount = 2,
            .prefix = ((1<<15)-1) ^ (1<<1),
        }, {
            .prefixLen = 20,
            .bitCount = 4,
            .prefix = ((1<<20)-1) ^ (1<<2),
        }, {
            .prefixLen = 18,
            .bitCount = 8,
            .prefix = ((1<<18)-1) ^ (1<<3),
        }
    };
# 149 "./switch/test/EncodingScheme_test.c"
    struct EncodingScheme list = {
        .count = 3,
        .forms = forms
    };
    struct Allocator* alloc = Allocator__child((parent),"EncodingScheme_test.c",153);
    String* data = EncodingScheme_serialize(&list, alloc);
    do { if (!((data->len == 11))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("EncodingScheme_test.c"), (155), "(data->len == 11)"); } } while (0);
    do { if (!((!__builtin_memcmp(data->bytes,"\x4f\xf4\xff\x29\xd9\xff\x7f\x89\xee\xff\x07",11)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("EncodingScheme_test.c"), (156), "(!__builtin_memcmp(data->bytes,\"\\x4f\\xf4\\xff\\x29\\xd9\\xff\\x7f\\x89\\xee\\xff\\x07\",11))"); } } while (0);
    Allocator__free((alloc),"EncodingScheme_test.c",157);
}



static int convertLabel(struct EncodingScheme_Form* iform,
                        struct EncodingScheme_Form* oform,
                        uint64_t label)
{
    struct {
        struct EncodingScheme scheme;
        struct EncodingScheme_Form forms[2];
    } s;
    s.scheme.count = 2;
    s.scheme.forms = s.forms;
    Bits__memcpy(&s.forms[0],iform,sizeof(struct EncodingScheme_Form),"EncodingScheme_test.c",172);
    Bits__memcpy(&s.forms[1],oform,sizeof(struct EncodingScheme_Form),"EncodingScheme_test.c",173);

    int iformNum = 0;
    int oformNum = 1;
    struct EncodingScheme* scheme = &s.scheme;

    do { if (!((EncodingScheme_getFormNum(scheme, label) == iformNum))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("EncodingScheme_test.c"), (179), "(EncodingScheme_getFormNum(scheme, label) == iformNum)"); } } while (0);

    uint64_t label2 = EncodingScheme_convertLabel(scheme, label, oformNum);

    if ((label & ((((uint64_t)1)<<(s.forms[0].prefixLen + s.forms[0].bitCount))-1)) == 1) {
        do { if (!((label2 == (~((uint64_t)0))))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("EncodingScheme_test.c"), (184), "(label2 == (~((uint64_t)0)))"); } } while (0);
        return 1;
    }

    if (Bits_log2x64(label) + EncodingScheme_formSize(oform) -
        EncodingScheme_formSize(iform) > 59)
    {
        do { if (!((label2 == (~((uint64_t)0))))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("EncodingScheme_test.c"), (191), "(label2 == (~((uint64_t)0)))"); } } while (0);
        return 2;
    }

    uint64_t additional = label >> s.forms[0].prefixLen;
    uint64_t director = additional & ((((uint64_t)1)<<(s.forms[0].bitCount))-1);

    if (!EncodingScheme_is358(&s.scheme)) {
    } else if (1 == (s.forms[0].prefix & ((((uint64_t)1)<<(s.forms[0].prefixLen))-1))) {
        director = director - (director == 1) + (director == 0);
    } else {
        director += (director > 0);
    }



    if (!EncodingScheme_is358(&s.scheme)) {
    } else if (1 == (s.forms[1].prefix & ((((uint64_t)1)<<(s.forms[1].prefixLen))-1))) {
        director = director - (director == 1) + (director == 0);
    } else {
        director -= (director > 0);
    }

    additional = additional >> s.forms[0].bitCount;

    uint64_t converted = (additional << s.forms[1].bitCount) | director;
    converted = (converted << s.forms[1].prefixLen) | s.forms[1].prefix;

    if ((converted & ((((uint64_t)1)<<(s.forms[1].prefixLen + s.forms[1].bitCount))-1)) == 1) {

        do { if (!((label2 == (~((uint64_t)0))))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("EncodingScheme_test.c"), (221), "(label2 == (~((uint64_t)0)))"); } } while (0);
        return 1;
    }


    do { if (!((label2 == converted))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("EncodingScheme_test.c"), (226), "(label2 == converted)"); } } while (0);

    uint64_t label3 = EncodingScheme_convertLabel(scheme, label2, iformNum);
    do { if (!((label3 == label))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("EncodingScheme_test.c"), (229), "(label3 == label)"); } } while (0);
    return 0;
}

static void convertLabelRand(struct Random* rand, struct EncodingScheme* scheme)
{
    for (int i = 0; i < 100; i++) {
        int oformNum;
        do {
            oformNum = Random_uint8(rand) % scheme->count;
        } while (!oformNum);
        int iformNum = Random_uint8(rand) % oformNum;
        uint64_t label = Random_uint64(rand);
        label &= (
# 242 "./switch/test/EncodingScheme_test.c" 3 4
                 0xffffffffffffffffULL 
# 242 "./switch/test/EncodingScheme_test.c"
                            >> (Random_uint8(rand) % 63));

        for (;;) {

            label <<= scheme->forms[iformNum].prefixLen;
            label |= scheme->forms[iformNum].prefix;
            do { if (!((EncodingScheme_getFormNum(scheme, label) == iformNum))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("EncodingScheme_test.c"), (248), "(EncodingScheme_getFormNum(scheme, label) == iformNum)"); } } while (0);

            if (Bits_log2x64(label) > 59) {

            } else {
                int ret = convertLabel(&scheme->forms[iformNum], &scheme->forms[oformNum], label);
                if (ret == 1) {
                    i--;
                    break;
                } else if (ret == 2) {

                } else if (!ret) {

                    break;
                } else {
                    do { if (!((0))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("EncodingScheme_test.c"), (263), "(0)"); } } while (0);
                }
            }
            label >>= scheme->forms[iformNum].prefixLen + 1;
        }
    }
}

static void isOneHopScheme(struct Allocator* allocator)
{
    struct Allocator* alloc = Allocator__child((allocator),"EncodingScheme_test.c",273);
    struct EncodingScheme* s4x8 = NumberCompress_v4x8_defineScheme(alloc);
    do { if (!((!EncodingScheme_isOneHop(s4x8, 1)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("EncodingScheme_test.c"), (275), "(!EncodingScheme_isOneHop(s4x8, 1))"); } } while (0);
    do { if (!((EncodingScheme_isOneHop(s4x8, 0x21)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("EncodingScheme_test.c"), (276), "(EncodingScheme_isOneHop(s4x8, 0x21))"); } } while (0);
    do { if (!((EncodingScheme_isOneHop(s4x8, 0x23)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("EncodingScheme_test.c"), (277), "(EncodingScheme_isOneHop(s4x8, 0x23))"); } } while (0);
    do { if (!((!EncodingScheme_isOneHop(s4x8, 0x12)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("EncodingScheme_test.c"), (278), "(!EncodingScheme_isOneHop(s4x8, 0x12))"); } } while (0);
    do { if (!((EncodingScheme_isOneHop(s4x8, 0x220)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("EncodingScheme_test.c"), (279), "(EncodingScheme_isOneHop(s4x8, 0x220))"); } } while (0);
    do { if (!((EncodingScheme_isOneHop(s4x8, 0x210)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("EncodingScheme_test.c"), (280), "(EncodingScheme_isOneHop(s4x8, 0x210))"); } } while (0);
    do { if (!((!EncodingScheme_isOneHop(s4x8, 0x110)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("EncodingScheme_test.c"), (281), "(!EncodingScheme_isOneHop(s4x8, 0x110))"); } } while (0);

    struct EncodingScheme* s3x5x8 = NumberCompress_v3x5x8_defineScheme(alloc);
    do { if (!((!EncodingScheme_isOneHop(s3x5x8, 1)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("EncodingScheme_test.c"), (284), "(!EncodingScheme_isOneHop(s3x5x8, 1))"); } } while (0);
    do { if (!((EncodingScheme_isOneHop(s3x5x8, 0x13)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("EncodingScheme_test.c"), (285), "(EncodingScheme_isOneHop(s3x5x8, 0x13))"); } } while (0);
    do { if (!((EncodingScheme_isOneHop(s3x5x8, 0x15)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("EncodingScheme_test.c"), (286), "(EncodingScheme_isOneHop(s3x5x8, 0x15))"); } } while (0);
    do { if (!((EncodingScheme_isOneHop(s3x5x8, 0x96)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("EncodingScheme_test.c"), (287), "(EncodingScheme_isOneHop(s3x5x8, 0x96))"); } } while (0);
    do { if (!((EncodingScheme_isOneHop(s3x5x8, 0x400)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("EncodingScheme_test.c"), (288), "(EncodingScheme_isOneHop(s3x5x8, 0x400))"); } } while (0);
    do { if (!((!EncodingScheme_isOneHop(s3x5x8, 0x115)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("EncodingScheme_test.c"), (289), "(!EncodingScheme_isOneHop(s3x5x8, 0x115))"); } } while (0);
    do { if (!((!EncodingScheme_isOneHop(s3x5x8, 0x166)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("EncodingScheme_test.c"), (290), "(!EncodingScheme_isOneHop(s3x5x8, 0x166))"); } } while (0);
    do { if (!((!EncodingScheme_isOneHop(s3x5x8, 0x1400)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("EncodingScheme_test.c"), (291), "(!EncodingScheme_isOneHop(s3x5x8, 0x1400))"); } } while (0);
    Allocator__free((alloc),"EncodingScheme_test.c",292);
}

typedef uint64_t (* EncodeNum)(uint32_t num);
typedef uint32_t (* DecodeNum)(uint64_t label);
static uint64_t encode358(uint32_t num)
{
    uint32_t bits = NumberCompress_v3x5x8_bitsUsedForNumber(num);
    return NumberCompress_v3x5x8_getCompressed(num, bits);
}
static uint32_t decode358(uint64_t label)
{
    uint32_t bits = NumberCompress_v3x5x8_bitsUsedForLabel(label);
    return NumberCompress_v3x5x8_getDecompressed(label, bits);
}
static uint64_t encode48(uint32_t num)
{
    uint32_t bits = NumberCompress_v4x8_bitsUsedForNumber(num);
    return NumberCompress_v4x8_getCompressed(num, bits);
}
static uint32_t decode48(uint64_t label)
{
    uint32_t bits = NumberCompress_v4x8_bitsUsedForLabel(label);
    return NumberCompress_v4x8_getDecompressed(label, bits);
}

static uint64_t encodef4(uint32_t num)
{
    uint32_t bits = NumberCompress_f4_bitsUsedForNumber(num);
    return NumberCompress_f4_getCompressed(num, bits);
}
static uint32_t decodef4(uint64_t label)
{
    uint32_t bits = NumberCompress_f4_bitsUsedForLabel(label);
    return NumberCompress_f4_getDecompressed(label, bits);
}

static void parseSerializeDir(struct EncodingScheme* scheme, EncodeNum en, DecodeNum dn)
{
    int max = ((((uint64_t)1)<<(scheme->forms[scheme->count - 1].bitCount))-1);
    for (int i = 0; i <= max; i++) {

        uint64_t dir = EncodingScheme_serializeDirector(scheme, i, -1);

        do { if (!((dir == en(i)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("EncodingScheme_test.c"), (336), "(dir == en(i))"); } } while (0);
        if (dir < ~0ull) {
            int out = EncodingScheme_parseDirector(scheme, dir);

            do { if (!(((uint32_t)i == dn(dir)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("EncodingScheme_test.c"), (340), "((uint32_t)i == dn(dir))"); } } while (0);

            do { if (!((i == out))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("EncodingScheme_test.c"), (342), "(i == out)"); } } while (0);
        }
        for (int j = 0; j < scheme->count; j++) {
            if (i >> scheme->forms[j].bitCount) { continue; }

            dir = scheme->forms[j].prefix | ( i << scheme->forms[j].prefixLen );
            int out = EncodingScheme_parseDirector(scheme, dir);
            do { if (!((out >= 0))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("EncodingScheme_test.c"), (349), "(out >= 0)"); } } while (0);
            uint64_t dir2 = EncodingScheme_serializeDirector(scheme, out, j);

            do { if (!((dir2 == dir))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("EncodingScheme_test.c"), (352), "(dir2 == dir)"); } } while (0);
        }
    }
}

static void bitsUsed(struct EncodingScheme* es, DecodeNum getBits, struct Random* rand)
{
    uint64_t label = Random_uint64(rand);
    int fn = EncodingScheme_getFormNum(es, label);
    do { if (!((fn > -1))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("EncodingScheme_test.c"), (361), "(fn > -1)"); } } while (0);
    uint32_t bits = getBits(label);
    uint32_t esbits = es->forms[fn].bitCount + es->forms[fn].prefixLen;

    do { if (!((bits == esbits))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("EncodingScheme_test.c"), (365), "(bits == esbits)"); } } while (0);
}

int EncodingScheme_test_main(int argc, char** argv);int EncodingScheme_test_main(int argc, char** argv)
{
    struct Allocator* alloc = MallocAllocator__new((20000000),"EncodingScheme_test.c",370);
    struct Random* rand = Random_new(alloc, 
# 371 "./switch/test/EncodingScheme_test.c" 3 4
                                           ((void *)0)
# 371 "./switch/test/EncodingScheme_test.c"
                                               , 
# 371 "./switch/test/EncodingScheme_test.c" 3 4
                                                 ((void *)0)
# 371 "./switch/test/EncodingScheme_test.c"
                                                     );

    encoding(alloc);
    isOneHopScheme(alloc);

    for (int i = 0; i < 1000; i++) {
        randomTest(alloc, rand);
    }

    for (int i = 0; i < 1000; i++) {
        fuzzTest(alloc, rand);
    }

    struct EncodingScheme* es48 = NumberCompress_v4x8_defineScheme(alloc);
    struct EncodingScheme* es358 = NumberCompress_v3x5x8_defineScheme(alloc);
    struct EncodingScheme* esf4 = NumberCompress_f4_defineScheme(alloc);

    for (int i = 0; i < 1000; i++) {
        bitsUsed(es48, NumberCompress_v4x8_bitsUsedForLabel, rand);
    }
    for (int i = 0; i < 1000; i++) {
        bitsUsed(es358, NumberCompress_v3x5x8_bitsUsedForLabel, rand);
    }

    parseSerializeDir(es358, encode358, decode358);
    parseSerializeDir(es48, encode48, decode48);
    parseSerializeDir(esf4, encodef4, decodef4);
# 406 "./switch/test/EncodingScheme_test.c"
    for (int i = 0; i < 10; i++) {
        struct Allocator* tempAlloc = Allocator__child((alloc),"EncodingScheme_test.c",407);
        struct EncodingScheme* scheme = randomScheme(rand, tempAlloc);
        convertLabelRand(rand, scheme);
        convertLabelRand(rand, es48);
        convertLabelRand(rand, es358);
        Allocator__free((tempAlloc),"EncodingScheme_test.c",412);
    }

    struct Allocator* tempAlloc = Allocator__child((alloc),"EncodingScheme_test.c",415);
    struct EncodingScheme* scheme = NumberCompress_v3x5x8_defineScheme(alloc);
    for (int i = 0; i < 257; i++) {
        int bits = NumberCompress_v3x5x8_bitsUsedForNumber(i);
        uint64_t expected = NumberCompress_v3x5x8_getCompressed(i, bits);
        do { if (!((expected == EncodingScheme_convertLabel(scheme, expected, (-5000))))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("EncodingScheme_test.c"), (420), "(expected == EncodingScheme_convertLabel(scheme, expected, (-5000)))"); } } while (0)
                                                                      ;
    }
    Allocator__free((tempAlloc),"EncodingScheme_test.c",423);

    Allocator__free((alloc),"EncodingScheme_test.c",425);

    return 0;
}
