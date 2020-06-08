# 1 "tunnel/IpTunnel.c"
# 1 "/home/jojapoppa/cjdns//"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "tunnel/IpTunnel.c"
# 15 "tunnel/IpTunnel.c"
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
struct Linker_x05313458778755846;
# 33 "./util/Assert.h"
__attribute__((__format__ (__printf__, 1, 2)))
__attribute__((__noreturn__))
void Assert_failure(const char* format, ...);
# 19 "./util/Identity.h" 2
# 1 "./util/Constant.h" 1
# 33 "./util/Constant.h"

# 20 "./util/Identity.h" 2



    
# 19 "./memory/Allocator.h" 2


struct Linker_x024440390493901187;





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
struct Linker_x05189880996706586;

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

struct Linker_x004844990268618399;

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
# 16 "tunnel/IpTunnel.c" 2
# 1 "./benc/Dict.h" 1
# 21 "./benc/Dict.h"
struct Linker_x02345764891847868;

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
# 17 "tunnel/IpTunnel.c" 2
# 1 "./benc/List.h" 1
# 21 "./benc/List.h"
struct Linker_x031130038301601326;

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
# 18 "tunnel/IpTunnel.c" 2
# 1 "./benc/Int.h" 1
# 19 "tunnel/IpTunnel.c" 2
# 1 "./benc/serialization/standard/BencMessageWriter.h" 1
# 19 "./benc/serialization/standard/BencMessageWriter.h"
# 1 "./exception/Except.h" 1
# 18 "./exception/Except.h"
# 1 "./exception/Er.h" 1
# 23 "./exception/Er.h"
struct Linker_x045272473908142374;

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


struct Linker_x09998483747500133;



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
# 20 "./benc/serialization/standard/BencMessageWriter.h" 2
# 1 "./wire/Message.h" 1
# 20 "./wire/Message.h"
# 1 "./util/Bits.h" 1
# 21 "./util/Bits.h"
struct Linker_x06242042634620268;


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

struct Linker_x05731762696419143;



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
# 21 "./benc/serialization/standard/BencMessageWriter.h" 2

struct Linker_x04154161843396469;

__attribute__ ((warn_unused_result)) struct Er_Ret* BencMessageWriter_write(Dict* toWrite, struct Message* msg);
# 20 "tunnel/IpTunnel.c" 2
# 1 "./benc/serialization/standard/BencMessageReader.h" 1
# 23 "./benc/serialization/standard/BencMessageReader.h"
struct Linker_x0017599663271716803;

__attribute__ ((warn_unused_result)) struct Er_Ret* BencMessageReader_read(Dict* *Er_returnValP, struct Message* msg, struct Allocator* alloc);

const char* BencMessageReader_readNoExcept(
    struct Message* msg, struct Allocator* alloc, Dict** outPtr);
# 21 "tunnel/IpTunnel.c" 2
# 1 "./crypto/AddressCalc.h" 1
# 19 "./crypto/AddressCalc.h"
struct Linker_x05347331650219025;
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
# 22 "tunnel/IpTunnel.c" 2
# 1 "./crypto/random/Random.h" 1
# 20 "./crypto/random/Random.h"
# 1 "./util/log/Log.h" 1
# 20 "./util/log/Log.h"
struct Linker_x02867769967148657;

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
struct Linker_x0277310019298308;



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

struct Linker_x03181399485258871;



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
# 23 "tunnel/IpTunnel.c" 2
# 1 "./interface/tuntap/TUNMessageType.h" 1
# 18 "./interface/tuntap/TUNMessageType.h"
# 1 "./util/Defined.h" 1
# 19 "./interface/tuntap/TUNMessageType.h" 2


static inline __attribute__ ((warn_unused_result)) struct Er_Ret* TUNMessageType_push(struct Message* message, uint16_t ethertype)

{
    do {             struct Er_Ret* Er_ret = Message_eshift(message, 4);             if (Er_ret) { return Er_unwind("TUNMessageType.h", 24, Er_ret); }         } while (0);
    ((uint16_t*) message->bytes)[0] = 0;
    ((uint16_t*) message->bytes)[1] = ethertype;
    return (struct Er_Ret*)0;
}

static inline __attribute__ ((warn_unused_result)) struct Er_Ret* TUNMessageType_pop(uint16_t *Er_returnValP, struct Message* message)
{
    do {             struct Er_Ret* Er_ret = Message_eshift(message, -4);             if (Er_ret) { return Er_unwind("TUNMessageType.h", 32, Er_ret); }         } while (0);
    *Er_returnValP = ((uint16_t*) message->bytes)[-1]; return (struct Er_Ret*)0;
}

enum TUNMessageType {

    TUNMessageType_ETHERTYPE = 0,


    TUNMessageType_NONE = 1,


    TUNMessageType_AF = 2,
};

static inline enum TUNMessageType TUNMessageType_guess()
{
    if (0 || 0) {
        return TUNMessageType_NONE;
    } else if (0 || 0) {
        return TUNMessageType_ETHERTYPE;
    } else {
        return TUNMessageType_AF;
    }
}
# 24 "tunnel/IpTunnel.c" 2

# 1 "./tunnel/IpTunnel.h" 1
# 20 "./tunnel/IpTunnel.h"
# 1 "./interface/Iface.h" 1
# 23 "./interface/Iface.h"
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
# 21 "./tunnel/IpTunnel.h" 2

# 1 "./util/events/EventBase.h" 1
# 20 "./util/events/EventBase.h"
struct Linker_x08361276155752921;

struct EventBase
{
    int unused;
};

struct EventBase* EventBase_new(struct Allocator* alloc);

int EventBase_eventCount(struct EventBase* eventBase);

void EventBase_beginLoop(struct EventBase* eventBase);

void EventBase_endLoop(struct EventBase* eventBase);
# 23 "./tunnel/IpTunnel.h" 2
# 1 "./util/platform/Sockaddr.h" 1
# 21 "./util/platform/Sockaddr.h"
struct Linker_x06964901283438438;



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
# 24 "./tunnel/IpTunnel.h" 2
# 1 "./util/GlobalConfig.h" 1
# 21 "./util/GlobalConfig.h"
struct Linker_x006572701464062503;
# 38 "./util/GlobalConfig.h"
struct GlobalConfig {
    int unused;
};

String* GlobalConfig_getTunName(struct GlobalConfig* conf);
void GlobalConfig_setTunName(struct GlobalConfig* conf, String* name);

struct GlobalConfig* GlobalConfig_new(struct Allocator* alloc);
# 25 "./tunnel/IpTunnel.h" 2
# 1 "./wire/RouteHeader.h" 1
# 19 "./wire/RouteHeader.h"
# 1 "./wire/SwitchHeader.h" 1
# 20 "./wire/SwitchHeader.h"
# 1 "./util/version/Version.h" 1
# 19 "./util/version/Version.h"
struct Linker_x07304332694389706;
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
# 21 "./wire/SwitchHeader.h" 2
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
# 26 "./tunnel/IpTunnel.h" 2
# 1 "./tunnel/RouteGen.h" 1
# 23 "./tunnel/RouteGen.h"
struct Linker_x0704917661486725;



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

struct Linker_x06073601689379968;

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
# 26 "tunnel/IpTunnel.c" 2


# 1 "./util/platform/netdev/NetDev.h" 1
# 22 "./util/platform/netdev/NetDev.h"
struct Linker_x05197127028769626;
# 34 "./util/platform/netdev/NetDev.h"
__attribute__ ((warn_unused_result)) struct Er_Ret* NetDev_addAddress(const char* ifName, struct Sockaddr* sa, struct Log* logger, struct Allocator* alloc)


                                                ;
# 47 "./util/platform/netdev/NetDev.h"
__attribute__ ((warn_unused_result)) struct Er_Ret* NetDev_setMTU(const char* interfaceName, uint32_t mtu, struct Log* logger, struct Allocator* alloc)


                                            ;

__attribute__ ((warn_unused_result)) struct Er_Ret* NetDev_flushAddresses(const char* deviceName, struct Allocator* alloc);




__attribute__ ((warn_unused_result)) struct Er_Ret* NetDev_setRoutes(const char* ifName, struct Sockaddr** prefixSet, int prefixCount, struct Log* logger, struct Allocator* tempAlloc)



                                                   ;
# 29 "tunnel/IpTunnel.c" 2
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
# 30 "tunnel/IpTunnel.c" 2
# 1 "./util/AddrTools.h" 1
# 19 "./util/AddrTools.h"
struct Linker_x08973553180726237;




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
# 31 "tunnel/IpTunnel.c" 2


# 1 "./util/events/Timeout.h" 1
# 20 "./util/events/Timeout.h"
struct Linker_x007092959458422166;



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
# 34 "tunnel/IpTunnel.c" 2

# 1 "./util/Escape.h" 1
# 21 "./util/Escape.h"
static inline char* Escape_getEscaped(uint8_t* buff, int length, struct Allocator* alloc)
{
    const char* hexSymbols = "0123456789abcdef";
    int finalSize = 1;
    for (int i = 0; i < length; i++) {
        if (buff[i] > 31 && buff[i] < 127) {
            finalSize++;
        } else {
            finalSize += 4;
        }
    }
    char* output = Allocator__malloc((alloc),(finalSize),"Escape.h",32);
    int j = 0;
    for (int i = 0; i < length; i++) {
        if (buff[i] > 31 && buff[i] < 127) {
            output[j++] = buff[i];
        } else {
            output[j++] = '\\';
            output[j++] = 'x';
            output[j++] = hexSymbols[buff[i] >> 4];
            output[j++] = hexSymbols[buff[i] & 0x0f];
        }
    }
    output[j++] = '\0';
    do { if (!((j == finalSize))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("Escape.h"), (45), "(j == finalSize)"); } } while (0);
    return output;
}
# 36 "tunnel/IpTunnel.c" 2
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
# 37 "tunnel/IpTunnel.c" 2
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
# 38 "tunnel/IpTunnel.c" 2
# 1 "./wire/Ethernet.h" 1
# 23 "./wire/Ethernet.h"
struct Ethernet
{
    uint16_t pad;
    uint8_t destAddr[6];
    uint8_t srcAddr[6];
    uint16_t ethertype;
};

void Assert_compileTime(char x[1 - (!(sizeof(struct Ethernet) == 16))]);
# 39 "tunnel/IpTunnel.c" 2
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
# 40 "tunnel/IpTunnel.c" 2

# 1 "/usr/lib/gcc/x86_64-w64-mingw32/9.2-win32/include/stddef.h" 1 3 4
# 1 "/usr/share/mingw-w64/include/stddef.h" 1 3 4
# 2 "/usr/lib/gcc/x86_64-w64-mingw32/9.2-win32/include/stddef.h" 2 3 4
# 42 "tunnel/IpTunnel.c" 2


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"


struct IpTunnel_pvt
{
    struct IpTunnel pub;

    struct Allocator* allocator;
    struct Log* logger;

    struct RouteGen* rg;

    uint32_t connectionCapacity;


    uint32_t nextConnectionNumber;


    struct GlobalConfig* globalConf;





    struct Timeout* timeout;
    struct Random* rand;


    unsigned long Identity_verifier;
};

static struct IpTunnel_Connection* newConnection(
# 76 "tunnel/IpTunnel.c" 3 4
                                                _Bool 
# 76 "tunnel/IpTunnel.c"
                                                     isOutgoing, struct IpTunnel_pvt* context)
{
    if (context->pub.connectionList.count == context->connectionCapacity) {
        uint32_t newSize = (context->connectionCapacity + 4) * sizeof(struct IpTunnel_Connection);
        context->pub.connectionList.connections =
            Allocator__realloc((context->allocator),(context->pub.connectionList.connections),(newSize),"IpTunnel.c",81);
        context->connectionCapacity += 4;
    }
    struct IpTunnel_Connection* conn =
        &context->pub.connectionList.connections[context->pub.connectionList.count];


    if (!isOutgoing) {
        for (int i = (int)context->pub.connectionList.count - 1; i >= 0; i--) {
            if (!context->pub.connectionList.connections[i].isOutgoing
                && conn != &context->pub.connectionList.connections[i + 1])
            {
                Bits__memcpy(conn,&context->pub.connectionList.connections[i + 1],sizeof(struct IpTunnel_Connection),"IpTunnel.c",93)

                                                                    ;
                conn = &context->pub.connectionList.connections[i + 1];
            }
        }
    }

    context->pub.connectionList.count++;

    __builtin_memset(conn,0,sizeof(struct IpTunnel_Connection));
    conn->number = context->nextConnectionNumber++;
    conn->isOutgoing = isOutgoing;


    do { if (!((context->nextConnectionNumber < (
# 108 "tunnel/IpTunnel.c" 3 4
   0xffffffffU 
# 108 "tunnel/IpTunnel.c"
   >> 1)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("IpTunnel.c"), (108), "(context->nextConnectionNumber < (0xffffffffU >> 1))"); } } while (0);

    return conn;
}

static void deleteConnection(struct IpTunnel_Connection* conn, struct IpTunnel_pvt* context)
{


    int i = (((char *)conn)-((char *)&context->pub.connectionList.connections[0]))
                                                  / sizeof(struct IpTunnel_Connection);


    do { if (!((i >= 0 && i < (signed int)context->pub.connectionList.count))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("IpTunnel.c"), (121), "(i >= 0 && i < (signed int)context->pub.connectionList.count)"); } } while (0);

    for (; (unsigned int)i < context->pub.connectionList.count-1; ++i) {
        Bits__memcpy(&context->pub.connectionList.connections[i],&context->pub.connectionList.connections[i + 1],sizeof(struct IpTunnel_Connection),"IpTunnel.c",124)

                                                       ;
    }

    int last = context->pub.connectionList.count-1;
    if (last > 0) {
        __builtin_memset(&context->pub.connectionList.connections[last],0,sizeof(struct IpTunnel_Connection))
                                                       ;
    }

    context->pub.connectionList.count--;
}

static struct IpTunnel_Connection* connectionByPubKey(uint8_t pubKey[32],
                                                      struct IpTunnel_pvt* context)
{
    for (int i = 0; i < (int)context->pub.connectionList.count; i++) {
        struct IpTunnel_Connection* conn = &context->pub.connectionList.connections[i];
        if (!__builtin_memcmp(pubKey,conn->routeHeader.publicKey,32)) {
            return conn;
        }
    }
    return 
# 147 "tunnel/IpTunnel.c" 3 4
          ((void *)0)
# 147 "tunnel/IpTunnel.c"
              ;
}
# 161 "tunnel/IpTunnel.c"
int IpTunnel_allowConnection(uint8_t publicKeyOfAuthorizedNode[32],
                             struct Sockaddr* ip6Addr,
                             uint8_t ip6Prefix,
                             uint8_t ip6Alloc,
                             struct Sockaddr* ip4Addr,
                             uint8_t ip4Prefix,
                             uint8_t ip4Alloc,
                             struct IpTunnel* tunnel)
{
    struct IpTunnel_pvt* context = (__extension__ ({ __typeof__((struct IpTunnel_pvt*)tunnel) Identity_ptr = ((struct IpTunnel_pvt*)tunnel); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0xe6f97dcf58e89230ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("IpTunnel.c"), (170), "(Identity_ptr->Identity_verifier == ((unsigned long) 0xe6f97dcf58e89230ull))"); } } while (0); Identity_ptr; }));

    do { if (context->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(context->logger, Log_Level_DEBUG, "IpTunnel.c", 172, "IPv4 Prefix to allow: %d", ip4Prefix); } } while (0);

    uint8_t* ip6Address = 
# 174 "tunnel/IpTunnel.c" 3 4
                         ((void *)0)
# 174 "tunnel/IpTunnel.c"
                             ;
    uint8_t* ip4Address = 
# 175 "tunnel/IpTunnel.c" 3 4
                         ((void *)0)
# 175 "tunnel/IpTunnel.c"
                             ;
    if (ip6Addr) {
        Sockaddr_getAddress(ip6Addr, &ip6Address);
    }
    if (ip4Addr) {
        Sockaddr_getAddress(ip4Addr, &ip4Address);
    }

    struct IpTunnel_Connection* conn = newConnection(
# 183 "tunnel/IpTunnel.c" 3 4
                                                    0
# 183 "tunnel/IpTunnel.c"
                                                         , context);
    Bits__memcpy(conn->routeHeader.publicKey,publicKeyOfAuthorizedNode,32,"IpTunnel.c",184);
    AddressCalc_addressForPublicKey(conn->routeHeader.ip6, publicKeyOfAuthorizedNode);
    if (ip4Address) {
        Bits__memcpy(conn->connectionIp4,ip4Address,4,"IpTunnel.c",187);
        conn->connectionIp4Prefix = ip4Prefix;
        conn->connectionIp4Alloc = ip4Alloc;
        do { if (!((ip4Alloc))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("IpTunnel.c"), (190), "(ip4Alloc)"); } } while (0);
    }

    if (ip6Address) {
        Bits__memcpy(conn->connectionIp6,ip6Address,16,"IpTunnel.c",194);
        conn->connectionIp6Prefix = ip6Prefix;
        conn->connectionIp6Alloc = ip6Alloc;
        do { if (!((ip6Alloc))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("IpTunnel.c"), (197), "(ip6Alloc)"); } } while (0);
    }

    return conn->number;
}

static __attribute__ ((warn_unused_result)) struct Iface* sendToNode(struct Message* message,
                              struct IpTunnel_Connection* connection,
                              struct IpTunnel_pvt* context)
{
    do {             struct Er_Ret* Er_ret = Message_epush(message,    ((void *)0)   , 4);                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("IpTunnel.c", 207, Er_ret);                 } else {                     Er__assertFail(Er_unwind("IpTunnel.c", 207, Er_ret));                 }             }                  } while (0);
    struct DataHeader* dh = (struct DataHeader*) message->bytes;
    DataHeader_setContentType(dh, ContentType_IPTUN);
    DataHeader_setVersion(dh, 1);
    do {             struct Er_Ret* Er_ret = Message_epush(message, &connection->routeHeader, (56 + 12));                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("IpTunnel.c", 211, Er_ret);                 } else {                     Er__assertFail(Er_unwind("IpTunnel.c", 211, Er_ret));                 }             }                  } while (0);
    return Iface_next(&context->pub.nodeInterface, message);
}

static void sendControlMessage(Dict* dict,
                               struct IpTunnel_Connection* connection,
                               struct Allocator* requestAlloc,
                               struct IpTunnel_pvt* context)
{
    struct Message* msg = Message_new(0, 1024, requestAlloc);
    do {             struct Er_Ret* Er_ret = BencMessageWriter_write(dict, msg);                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("IpTunnel.c", 221, Er_ret);                 } else {                     Er__assertFail(Er_unwind("IpTunnel.c", 221, Er_ret));                 }             }                  } while (0);

    int length = msg->length;


    do {             struct Er_Ret* Er_ret = Message_eshift(msg, 8);                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("IpTunnel.c", 226, Er_ret);                 } else {                     Er__assertFail(Er_unwind("IpTunnel.c", 226, Er_ret));                 }             }                  } while (0);
    struct Headers_UDPHeader* uh = (struct Headers_UDPHeader*) msg->bytes;
    uh->srcPort_be = 0;
    uh->destPort_be = 0;
    uh->length_be = ((uint16_t)(__extension__ ({ uint16_t _x = (((uint16_t)(length))); _x >> 8 | _x << 8; })));
    uh->checksum_be = 0;

    uint16_t payloadLength = msg->length;

    do {             struct Er_Ret* Er_ret = Message_eshift(msg, 40);                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("IpTunnel.c", 235, Er_ret);                 } else {                     Er__assertFail(Er_unwind("IpTunnel.c", 235, Er_ret));                 }             }                  } while (0);
    struct Headers_IP6Header* header = (struct Headers_IP6Header*) msg->bytes;
    header->versionClassAndFlowLabel = 0;
    header->flowLabelLow_be = 0;
    header->nextHeader = 17;
    header->hopLimit = 0;
    header->payloadLength_be = ((uint16_t)(__extension__ ({ uint16_t _x = (((uint16_t)(payloadLength))); _x >> 8 | _x << 8; })));
    (((uint8_t*) header)[0] |= ( (sizeof(*header) == 20) ? 4 : 6 ) << 4);


    __builtin_memset(header->sourceAddr,0,32);

    uh->checksum_be = Checksum_udpIp6(header->sourceAddr,
                                      (uint8_t*) uh,
                                      msg->length - 40);

    do { do { if (!((!msg->currentIface))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("IpTunnel.c"), (251), "(!msg->currentIface)"); } } while (0); struct Iface Iface_y = { .currentMsg = msg }; msg->currentIface = &Iface_y; struct Iface* Iface_x = sendToNode(msg, connection, context); msg->currentIface = 
# 251 "tunnel/IpTunnel.c" 3 4
   ((void *)0)
# 251 "tunnel/IpTunnel.c"
   ; if (Iface_x) { Iface_send(Iface_x, msg); } } while (0);
}

static void requestAddresses(struct IpTunnel_Connection* conn, struct IpTunnel_pvt* context)
{
    if (1) {
        uint8_t addr[40];
        AddrTools_printIp(addr, conn->routeHeader.ip6);
        do { if (context->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(context->logger, Log_Level_DEBUG, "IpTunnel.c", 259, "Requesting addresses from [%s] for connection [%d]", addr, conn->number); } } while (0)
                                     ;
    }

    int number = conn->number;
    Dict d = (&(struct Dict_Entry) { .key = (&(String) { .bytes = "q", .len = CString_strlen("q") }), .val = (&(Object) { .type = Object_STRING, .as.string = (&(String) { .bytes = "IpTunnel_getAddresses", .len = CString_strlen("IpTunnel_getAddresses") }) }), .next = (&(struct Dict_Entry) { .key = (&(String) { .bytes = "txid", .len = CString_strlen("txid") }), .val = (&(Object) { .type = Object_STRING, .as.string = (&(String){ .len = 4, .bytes = (char*)&number }) }), .next = 
# 264 "tunnel/IpTunnel.c" 3 4
            ((void *)0) 
# 264 "tunnel/IpTunnel.c"
            }) })



      ;
    struct Allocator* msgAlloc = Allocator__child((context->allocator),"IpTunnel.c",269);
    sendControlMessage(&d, conn, msgAlloc, context);
    Allocator__free((msgAlloc),"IpTunnel.c",271);
}
# 281 "tunnel/IpTunnel.c"
int IpTunnel_connectTo(uint8_t publicKeyOfNodeToConnectTo[32], struct IpTunnel* tunnel)
{
    struct IpTunnel_pvt* context = (__extension__ ({ __typeof__((struct IpTunnel_pvt*)tunnel) Identity_ptr = ((struct IpTunnel_pvt*)tunnel); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0xe6f97dcf58e89230ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("IpTunnel.c"), (283), "(Identity_ptr->Identity_verifier == ((unsigned long) 0xe6f97dcf58e89230ull))"); } } while (0); Identity_ptr; }));

    struct IpTunnel_Connection* conn = newConnection(
# 285 "tunnel/IpTunnel.c" 3 4
                                                    1
# 285 "tunnel/IpTunnel.c"
                                                        , context);
    Bits__memcpy(conn->routeHeader.publicKey,publicKeyOfNodeToConnectTo,32,"IpTunnel.c",286);
    AddressCalc_addressForPublicKey(conn->routeHeader.ip6, publicKeyOfNodeToConnectTo);

    if (1) {
        uint8_t addr[40];
        AddrTools_printIp(addr, conn->routeHeader.ip6);
        do { if (context->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(context->logger, Log_Level_DEBUG, "IpTunnel.c", 292, "Trying to connect to [%s]", addr); } } while (0);
    }

    requestAddresses(conn, context);

    return conn->number;
}







int IpTunnel_removeConnection(int connectionNumber, struct IpTunnel* tunnel)
{
    struct IpTunnel_pvt* context = (__extension__ ({ __typeof__((struct IpTunnel_pvt*)tunnel) Identity_ptr = ((struct IpTunnel_pvt*)tunnel); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0xe6f97dcf58e89230ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("IpTunnel.c"), (308), "(Identity_ptr->Identity_verifier == ((unsigned long) 0xe6f97dcf58e89230ull))"); } } while (0); Identity_ptr; }));

    for (int i = 0; i < (int)tunnel->connectionList.count; ++i)
    {
        if (tunnel->connectionList.connections[i].number==connectionNumber)
        {
            deleteConnection(&tunnel->connectionList.connections[i], context);
            return 0;
        }
    }

    return -1;
}

static 
# 322 "tunnel/IpTunnel.c" 3 4
      _Bool 
# 322 "tunnel/IpTunnel.c"
           isControlMessageInvalid(struct Message* message, struct IpTunnel_pvt* context)
{
    struct Headers_IP6Header* header = (struct Headers_IP6Header*) message->bytes;
    uint16_t length = ((uint16_t)(__extension__ ({ uint16_t _x = (((uint16_t)(header->payloadLength_be))); _x >> 8 | _x << 8; })));
    if (header->nextHeader != 17 || message->length < length + 40) {
        do { if (context->logger && Log_Level_WARN >= Log_Level_DEBUG) { Log_print(context->logger, Log_Level_WARN, "IpTunnel.c", 327, "Invalid IPv6 packet (not UDP or length field too big)"); } } while (0);
        return 
# 328 "tunnel/IpTunnel.c" 3 4
              1
# 328 "tunnel/IpTunnel.c"
                  ;
    }

    do {             struct Er_Ret* Er_ret = Message_eshift(message, -40);                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("IpTunnel.c", 331, Er_ret);                 } else {                     Er__assertFail(Er_unwind("IpTunnel.c", 331, Er_ret));                 }             }                  } while (0);
    struct Headers_UDPHeader* udp = (struct Headers_UDPHeader*) message->bytes;

    if (Checksum_udpIp6(header->sourceAddr, message->bytes, length)) {
        do { if (context->logger && Log_Level_WARN >= Log_Level_DEBUG) { Log_print(context->logger, Log_Level_WARN, "IpTunnel.c", 335, "Checksum mismatch"); } } while (0);
        return 
# 336 "tunnel/IpTunnel.c" 3 4
              1
# 336 "tunnel/IpTunnel.c"
                  ;
    }

    length -= 8;
    if (((uint16_t)(__extension__ ({ uint16_t _x = (((uint16_t)(udp->length_be))); _x >> 8 | _x << 8; }))) != length
        || udp->srcPort_be != 0
        || udp->destPort_be != 0)
    {
        do { if (context->logger && Log_Level_WARN >= Log_Level_DEBUG) { Log_print(context->logger, Log_Level_WARN, "IpTunnel.c", 344, "Invalid UDP packet (length mismatch or wrong ports)"); } } while (0);
        return 
# 345 "tunnel/IpTunnel.c" 3 4
              1
# 345 "tunnel/IpTunnel.c"
                  ;
    }

    do {             struct Er_Ret* Er_ret = Message_eshift(message, -8);                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("IpTunnel.c", 348, Er_ret);                 } else {                     Er__assertFail(Er_unwind("IpTunnel.c", 348, Er_ret));                 }             }                  } while (0);

    message->length = length;
    return 
# 351 "tunnel/IpTunnel.c" 3 4
          0
# 351 "tunnel/IpTunnel.c"
               ;
}

static __attribute__ ((warn_unused_result)) struct Iface* requestForAddresses(Dict* request,
                                       struct IpTunnel_Connection* conn,
                                       struct Allocator* requestAlloc,
                                       struct IpTunnel_pvt* context)
{
    if (1) {
        uint8_t addr[40];
        AddrTools_printIp(addr, conn->routeHeader.ip6);
        do { if (context->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(context->logger, Log_Level_DEBUG, "IpTunnel.c", 362, "Got request for addresses from [%s]", addr); } } while (0);
    }

    if (conn->isOutgoing) {
        do { if (context->logger && Log_Level_WARN >= Log_Level_DEBUG) { Log_print(context->logger, Log_Level_WARN, "IpTunnel.c", 366, "got request for addresses from outgoing connection"); } } while (0);
        return 0;
    }
    Dict* addresses = Dict_new(requestAlloc);
    
# 370 "tunnel/IpTunnel.c" 3 4
   _Bool 
# 370 "tunnel/IpTunnel.c"
        noAddresses = 
# 370 "tunnel/IpTunnel.c" 3 4
                      1
# 370 "tunnel/IpTunnel.c"
                          ;
    if (!Bits_isZero(conn->connectionIp6, 16)) {
        Dict_putString(addresses, String_new("ip6", requestAlloc), String_newBinary((char*)conn->connectionIp6, 16, requestAlloc), requestAlloc)


                                    ;
        Dict_putInt(addresses, String_new("ip6Prefix", requestAlloc), (int64_t)conn->connectionIp6Prefix, requestAlloc)

                                 ;
        Dict_putInt(addresses, String_new("ip6Alloc", requestAlloc), (int64_t)conn->connectionIp6Alloc, requestAlloc)

                                 ;

        noAddresses = 
# 383 "tunnel/IpTunnel.c" 3 4
                     0
# 383 "tunnel/IpTunnel.c"
                          ;
    }
    if (!Bits_isZero(conn->connectionIp4, 4)) {
        Dict_putString(addresses, String_new("ip4", requestAlloc), String_newBinary((char*)conn->connectionIp4, 4, requestAlloc), requestAlloc)


                                    ;
        Dict_putInt(addresses, String_new("ip4Prefix", requestAlloc), (int64_t)conn->connectionIp4Prefix, requestAlloc)

                                 ;
        Dict_putInt(addresses, String_new("ip4Alloc", requestAlloc), (int64_t)conn->connectionIp4Alloc, requestAlloc)

                                 ;

        noAddresses = 
# 397 "tunnel/IpTunnel.c" 3 4
                     0
# 397 "tunnel/IpTunnel.c"
                          ;
    }
    if (noAddresses) {
        do { if (context->logger && Log_Level_WARN >= Log_Level_DEBUG) { Log_print(context->logger, Log_Level_WARN, "IpTunnel.c", 400, "no addresses to provide"); } } while (0);
        return 0;
    }

    Dict* msg = Dict_new(requestAlloc);
    Dict_putDict(msg, String_new("addresses", requestAlloc), addresses, requestAlloc);

    String* txid = Dict_getString(request, (&(String) { .bytes = "txid", .len = CString_strlen("txid") }));
    if (txid) {
        Dict_putString(msg, String_new("txid", requestAlloc), txid, requestAlloc);
    }

    sendControlMessage(msg, conn, requestAlloc, context);
    return 0;
}

static void addAddress(char* printedAddr, uint8_t prefixLen,
                       uint8_t allocSize, struct IpTunnel_pvt* ctx,
                       struct Allocator* tempAlloc)
{
    String* tunName = GlobalConfig_getTunName(ctx->globalConf);
    if (!tunName) {
        do { if (ctx->logger && Log_Level_ERROR >= Log_Level_DEBUG) { Log_print(ctx->logger, Log_Level_ERROR, "IpTunnel.c", 422, "Failed to set IP address because TUN interface is not setup"); } } while (0);
        return;
    }
    struct Sockaddr_storage ss;
    if (Sockaddr_parse(printedAddr, &ss)) {
        do { if (ctx->logger && Log_Level_ERROR >= Log_Level_DEBUG) { Log_print(ctx->logger, Log_Level_ERROR, "IpTunnel.c", 427, "Invalid ip, setting ip address on TUN"); } } while (0);
        return;
    }
    ss.addr.flags |= (1<<1);

    ss.addr.prefix = allocSize;
    struct Er_Ret* er = 
# 433 "tunnel/IpTunnel.c" 3 4
                       ((void *)0)
# 433 "tunnel/IpTunnel.c"
                           ;
    do {             struct Er_Ret* Er_ret = NetDev_addAddress(tunName->bytes, &ss.addr, ctx->logger, tempAlloc);                          if (Er_ret) {                 struct Er_Ret** Er_errOut = &er;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("IpTunnel.c", 434, Er_ret);                 } else {                     Er__assertFail(Er_unwind("IpTunnel.c", 434, Er_ret));                 }             }                  } while (0);
    if (er) {
        do { if (ctx->logger && Log_Level_ERROR >= Log_Level_DEBUG) { Log_print(ctx->logger, Log_Level_ERROR, "IpTunnel.c", 436, "Error setting ip address on TUN [%s]", er->message); } } while (0);
        return;
    }

    ss.addr.prefix = prefixLen;
    
# 441 "tunnel/IpTunnel.c" 3 4
   _Bool 
# 441 "tunnel/IpTunnel.c"
        installRoute = 
# 441 "tunnel/IpTunnel.c" 3 4
                       0
# 441 "tunnel/IpTunnel.c"
                            ;
    if (Sockaddr_getFamily(&ss.addr) == Sockaddr_AF_INET) {
        installRoute = (prefixLen < 32);
    } else if (Sockaddr_getFamily(&ss.addr) == Sockaddr_AF_INET6) {
        installRoute = (prefixLen < 128);
    } else {
        Assert_failure("bad address family");
    }
    if (installRoute) {
        RouteGen_addPrefix(ctx->rg, &ss.addr);
    }
}

static __attribute__ ((warn_unused_result)) struct Iface* incomingAddresses(Dict* d,
                                     struct IpTunnel_Connection* conn,
                                     struct Allocator* alloc,
                                     struct IpTunnel_pvt* context)
{
    if (!conn->isOutgoing) {
        do { if (context->logger && Log_Level_WARN >= Log_Level_DEBUG) { Log_print(context->logger, Log_Level_WARN, "IpTunnel.c", 460, "got offer of addresses from incoming connection"); } } while (0);
        return 0;
    }

    String* txid = Dict_getString(d, (&(String) { .bytes = "txid", .len = CString_strlen("txid") }));
    if (!txid || txid->len != 4) {
        do { if (context->logger && Log_Level_INFO >= Log_Level_DEBUG) { Log_print(context->logger, Log_Level_INFO, "IpTunnel.c", 466, "missing or wrong length txid"); } } while (0);
        return 0;
    }

    int number;
    Bits__memcpy(&number,txid->bytes,4,"IpTunnel.c",471);
    if (number < 0 || number >= (int)context->nextConnectionNumber) {
        do { if (context->logger && Log_Level_INFO >= Log_Level_DEBUG) { Log_print(context->logger, Log_Level_INFO, "IpTunnel.c", 473, "txid out of range"); } } while (0);
        return 0;
    }

    if (number != conn->number) {
        for (int i = 0; i < (int)context->pub.connectionList.count; i++) {
            if (context->pub.connectionList.connections[i].number == number) {
                if (__builtin_memcmp(conn->routeHeader.publicKey,context->pub.connectionList.connections[i].routeHeader.publicKey,32)

                                   )
                {
                    do { if (context->logger && Log_Level_INFO >= Log_Level_DEBUG) { Log_print(context->logger, Log_Level_INFO, "IpTunnel.c", 484, "txid doesn't match origin"); } } while (0);
                    return 0;
                } else {
                    conn = &context->pub.connectionList.connections[i];
                }
            }
        }
    }

    Dict* addresses = Dict_getDict(d, (&(String) { .bytes = "addresses", .len = CString_strlen("addresses") }));

    String* ip4 = Dict_getString(addresses, (&(String) { .bytes = "ip4", .len = CString_strlen("ip4") }));
    int64_t* ip4Prefix = Dict_getInt(addresses, (&(String) { .bytes = "ip4Prefix", .len = CString_strlen("ip4Prefix") }));
    int64_t* ip4Alloc = Dict_getInt(addresses, (&(String) { .bytes = "ip4Alloc", .len = CString_strlen("ip4Alloc") }));

    if (ip4 && ip4->len == 4) {
        Bits__memcpy(conn->connectionIp4,ip4->bytes,4,"IpTunnel.c",500);

        if (ip4Prefix && *ip4Prefix >= 0 && *ip4Prefix <= 32) {
            conn->connectionIp4Prefix = (uint8_t) *ip4Prefix;
        } else {
            conn->connectionIp4Prefix = 32;
        }

        if (ip4Alloc && *ip4Alloc >= 0 && *ip4Alloc <= 32) {
            conn->connectionIp4Alloc = (uint8_t) *ip4Alloc;
        } else {
            conn->connectionIp4Alloc = 32;
        }

        struct Sockaddr* sa = Sockaddr_clone((0 ? Sockaddr_LOOPBACK_be : Sockaddr_LOOPBACK_le), alloc);
        uint8_t* addrBytes = 
# 515 "tunnel/IpTunnel.c" 3 4
                            ((void *)0)
# 515 "tunnel/IpTunnel.c"
                                ;
        Sockaddr_getAddress(sa, &addrBytes);
        Bits__memcpy(addrBytes,ip4->bytes,4,"IpTunnel.c",517);
        char* printedAddr = Sockaddr_print(sa, alloc);

        do { if (context->logger && Log_Level_INFO >= Log_Level_DEBUG) { Log_print(context->logger, Log_Level_INFO, "IpTunnel.c", 520, "Got issued address [%s/%d:%d] for connection [%d]", printedAddr, conn->connectionIp4Alloc, conn->connectionIp4Prefix, conn->number); } } while (0)
                                                                                                ;

        addAddress(printedAddr,
            conn->connectionIp4Prefix, conn->connectionIp4Alloc, context, alloc);
    }

    String* ip6 = Dict_getString(addresses, (&(String) { .bytes = "ip6", .len = CString_strlen("ip6") }));
    int64_t* ip6Prefix = Dict_getInt(addresses, (&(String) { .bytes = "ip6Prefix", .len = CString_strlen("ip6Prefix") }));
    int64_t* ip6Alloc = Dict_getInt(addresses, (&(String) { .bytes = "ip6Alloc", .len = CString_strlen("ip6Alloc") }));

    if (ip6 && ip6->len == 16) {
        Bits__memcpy(conn->connectionIp6,ip6->bytes,16,"IpTunnel.c",532);

        if (ip6Prefix && *ip6Prefix >= 0 && *ip6Prefix <= 128) {
            conn->connectionIp6Prefix = (uint8_t) *ip6Prefix;
        } else {
            conn->connectionIp6Prefix = 128;
        }

        if (ip6Alloc && *ip6Alloc >= 0 && *ip6Alloc <= 128) {
            conn->connectionIp6Alloc = (uint8_t) *ip6Alloc;
        } else {
            conn->connectionIp6Alloc = 128;
        }

        struct Sockaddr* sa = Sockaddr_clone(Sockaddr_LOOPBACK6, alloc);
        uint8_t* addrBytes = 
# 547 "tunnel/IpTunnel.c" 3 4
                            ((void *)0)
# 547 "tunnel/IpTunnel.c"
                                ;
        Sockaddr_getAddress(sa, &addrBytes);
        Bits__memcpy(addrBytes,ip6->bytes,16,"IpTunnel.c",549);
        char* printedAddr = Sockaddr_print(sa, alloc);

        do { if (context->logger && Log_Level_INFO >= Log_Level_DEBUG) { Log_print(context->logger, Log_Level_INFO, "IpTunnel.c", 552, "Got issued address block [%s/%d:%d] for connection [%d]", printedAddr, conn->connectionIp6Alloc, conn->connectionIp6Prefix, conn->number); } } while (0)
                                                                                                ;

        addAddress(printedAddr,
            conn->connectionIp6Prefix, conn->connectionIp6Alloc, context, alloc);
    }
    if (context->rg->hasUncommittedChanges) {
        String* tunName = GlobalConfig_getTunName(context->globalConf);
        if (!tunName) {
            do { if (context->logger && Log_Level_ERROR >= Log_Level_DEBUG) { Log_print(context->logger, Log_Level_ERROR, "IpTunnel.c", 561, "Failed to set routes because TUN interface is not setup"); } } while (0);
            return 0;
        }
        struct Er_Ret* er = 
# 564 "tunnel/IpTunnel.c" 3 4
                           ((void *)0)
# 564 "tunnel/IpTunnel.c"
                               ;
        do {             struct Er_Ret* Er_ret = RouteGen_commit(context->rg, tunName->bytes, alloc);                          if (Er_ret) {                 struct Er_Ret** Er_errOut = &er;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("IpTunnel.c", 565, Er_ret);                 } else {                     Er__assertFail(Er_unwind("IpTunnel.c", 565, Er_ret));                 }             }                  } while (0);
        if (er) {
            do { if (context->logger && Log_Level_ERROR >= Log_Level_DEBUG) { Log_print(context->logger, Log_Level_ERROR, "IpTunnel.c", 567, "Error setting routes for TUN [%s]", er->message); } } while (0);
            return 0;
        }
    }
    return 0;
}

static __attribute__ ((warn_unused_result)) struct Iface* incomingControlMessage(struct Message* message,
                                          struct IpTunnel_Connection* conn,
                                          struct IpTunnel_pvt* context)
{
    if (1) {
        uint8_t addr[40];
        AddrTools_printIp(addr, conn->routeHeader.ip6);
        do { if (context->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(context->logger, Log_Level_DEBUG, "IpTunnel.c", 581, "Got incoming message from [%s]", addr); } } while (0);
    }


    if (isControlMessageInvalid(message, context)) {
        return 0;
    }

    do { if (context->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(context->logger, Log_Level_DEBUG, "IpTunnel.c", 589, "Message content [%s]", Escape_getEscaped(message->bytes, message->length, message->alloc)); } } while (0)
                                                                           ;

    struct Allocator* alloc = Allocator__child((message->alloc),"IpTunnel.c",592);

    Dict* d = 
# 594 "tunnel/IpTunnel.c" 3 4
             ((void *)0)
# 594 "tunnel/IpTunnel.c"
                 ;
    const char* err = BencMessageReader_readNoExcept(message, alloc, &d);
    if (err) {
        do { if (context->logger && Log_Level_INFO >= Log_Level_DEBUG) { Log_print(context->logger, Log_Level_INFO, "IpTunnel.c", 597, "Failed to parse message [%s]", err); } } while (0);
        return 0;
    }

    if (Dict_getDict(d, (&(String) { .bytes = "addresses", .len = CString_strlen("addresses") }))) {
        return incomingAddresses(d, conn, alloc, context);
    }
    if (String_equals((&(String) { .bytes = "IpTunnel_getAddresses", .len = CString_strlen("IpTunnel_getAddresses") }),
                      Dict_getString(d, (&(String) { .bytes = "q", .len = CString_strlen("q") }))))
    {
        return requestForAddresses(d, conn, alloc, context);
    }
    do { if (context->logger && Log_Level_WARN >= Log_Level_DEBUG) { Log_print(context->logger, Log_Level_WARN, "IpTunnel.c", 609, "Message which is unhandled"); } } while (0);
    return 0;
}
# 643 "tunnel/IpTunnel.c"
static 
# 643 "tunnel/IpTunnel.c" 3 4
      _Bool 
# 643 "tunnel/IpTunnel.c"
           prefixMatches6(uint8_t* addressA, uint8_t* refAddr, uint8_t prefixLen)
{
    if (!prefixLen) {
        do { if (!((Bits_isZero(refAddr, 16)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("IpTunnel.c"), (646), "(Bits_isZero(refAddr, 16))"); } } while (0);
        return 
# 647 "tunnel/IpTunnel.c" 3 4
              0
# 647 "tunnel/IpTunnel.c"
                   ;
    }
    do { if (!((prefixLen && prefixLen <= 128))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("IpTunnel.c"), (649), "(prefixLen && prefixLen <= 128)"); } } while (0);
    uint64_t a0 = ({ uint64_t x = (uint64_t) (((uint32_t*)(addressA))[0]); x |= (( (uint64_t) ((uint32_t*)(addressA))[1]) << 32); ((uint64_t)__builtin_bswap64(((uint64_t)(x)))); });
    uint64_t b0 = ({ uint64_t x = (uint64_t) (((uint32_t*)(refAddr))[0]); x |= (( (uint64_t) ((uint32_t*)(refAddr))[1]) << 32); ((uint64_t)__builtin_bswap64(((uint64_t)(x)))); });
    if (prefixLen <= 64) {
        return !( (a0 ^ b0) >> (64 - prefixLen) );
    }
    uint64_t a1 = ({ uint64_t x = (uint64_t) (((uint32_t*)(addressA + 8))[0]); x |= (( (uint64_t) ((uint32_t*)(addressA + 8))[1]) << 32); ((uint64_t)__builtin_bswap64(((uint64_t)(x)))); });
    uint64_t b1 = ({ uint64_t x = (uint64_t) (((uint32_t*)(refAddr + 8))[0]); x |= (( (uint64_t) ((uint32_t*)(refAddr + 8))[1]) << 32); ((uint64_t)__builtin_bswap64(((uint64_t)(x)))); });
    return !( (a0 ^ b0) | ((a1 ^ b1) >> (128 - prefixLen)) );
}

static 
# 660 "tunnel/IpTunnel.c" 3 4
      _Bool 
# 660 "tunnel/IpTunnel.c"
           prefixMatches4(uint8_t* addressA, uint8_t* refAddr, uint32_t prefixLen)
{
    if (!prefixLen) {
        do { if (!((Bits_isZero(refAddr, 4)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("IpTunnel.c"), (663), "(Bits_isZero(refAddr, 4))"); } } while (0);
        return 
# 664 "tunnel/IpTunnel.c" 3 4
              0
# 664 "tunnel/IpTunnel.c"
                   ;
    }
    do { if (!((prefixLen && prefixLen <= 32))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("IpTunnel.c"), (666), "(prefixLen && prefixLen <= 32)"); } } while (0);
    uint32_t a = ({ uint32_t x = (((uint32_t*)(addressA))[0]); ((uint32_t)__builtin_bswap32(((uint32_t)(x)))); });
    uint32_t b = ({ uint32_t x = (((uint32_t*)(refAddr))[0]); ((uint32_t)__builtin_bswap32(((uint32_t)(x)))); });
    return !((a ^ b) >> (32 - prefixLen));
}

static 
# 672 "tunnel/IpTunnel.c" 3 4
      _Bool 
# 672 "tunnel/IpTunnel.c"
           isValidAddress4(uint8_t sourceAndDestIp4[8],
                            
# 673 "tunnel/IpTunnel.c" 3 4
                           _Bool 
# 673 "tunnel/IpTunnel.c"
                                isFromTun,
                            struct IpTunnel_Connection* conn)
{
    uint8_t* compareAddr = (isFromTun)
        ? ((conn->isOutgoing) ? sourceAndDestIp4 : &sourceAndDestIp4[4])
        : ((conn->isOutgoing) ? &sourceAndDestIp4[4] : sourceAndDestIp4);
    return prefixMatches4(compareAddr, conn->connectionIp4, conn->connectionIp4Alloc);
}

static 
# 682 "tunnel/IpTunnel.c" 3 4
      _Bool 
# 682 "tunnel/IpTunnel.c"
           isValidAddress6(uint8_t sourceAndDestIp6[32],
                            
# 683 "tunnel/IpTunnel.c" 3 4
                           _Bool 
# 683 "tunnel/IpTunnel.c"
                                isFromTun,
                            struct IpTunnel_Connection* conn)
{
    if (AddressCalc_validAddress(sourceAndDestIp6)
        || AddressCalc_validAddress(&sourceAndDestIp6[16])) {
        return 
# 688 "tunnel/IpTunnel.c" 3 4
              0
# 688 "tunnel/IpTunnel.c"
                   ;
    }
    uint8_t* compareAddr = (isFromTun)
        ? ((conn->isOutgoing) ? sourceAndDestIp6 : &sourceAndDestIp6[16])
        : ((conn->isOutgoing) ? &sourceAndDestIp6[16] : sourceAndDestIp6);
    return prefixMatches6(compareAddr, conn->connectionIp6, conn->connectionIp6Alloc);
}

static struct IpTunnel_Connection* findConnection(uint8_t sourceAndDestIp6[32],
                                                  uint8_t sourceAndDestIp4[8],
                                                  
# 698 "tunnel/IpTunnel.c" 3 4
                                                 _Bool 
# 698 "tunnel/IpTunnel.c"
                                                      isFromTun,
                                                  struct IpTunnel_pvt* context)
{
    for (int i = 0; i < (int)context->pub.connectionList.count; i++) {
        struct IpTunnel_Connection* conn = &context->pub.connectionList.connections[i];
        if (sourceAndDestIp6 && isValidAddress6(sourceAndDestIp6, isFromTun, conn)) {
            return conn;
        }
        if (sourceAndDestIp4 && isValidAddress4(sourceAndDestIp4, isFromTun, conn)) {
            return conn;
        }
    }
    return 
# 710 "tunnel/IpTunnel.c" 3 4
          ((void *)0)
# 710 "tunnel/IpTunnel.c"
              ;
}

static __attribute__ ((warn_unused_result)) struct Iface* incomingFromTun(struct Message* message, struct Iface* tunIf)
{
    struct IpTunnel_pvt* context = (__extension__ ({ __typeof__((struct IpTunnel_pvt*)tunIf) Identity_ptr = ((struct IpTunnel_pvt*)tunIf); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0xe6f97dcf58e89230ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("IpTunnel.c"), (715), "(Identity_ptr->Identity_verifier == ((unsigned long) 0xe6f97dcf58e89230ull))"); } } while (0); Identity_ptr; }));

    if (message->length < 20) {
        do { if (context->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(context->logger, Log_Level_DEBUG, "IpTunnel.c", 718, "DROP runt"); } } while (0);
    }

    struct IpTunnel_Connection* conn = 
# 721 "tunnel/IpTunnel.c" 3 4
                                      ((void *)0)
# 721 "tunnel/IpTunnel.c"
                                          ;
    if (!context->pub.connectionList.connections) {

    } else if (message->length > 40 && Headers_getIpVersion(message->bytes) == 6) {
        struct Headers_IP6Header* header = (struct Headers_IP6Header*) message->bytes;
        conn = findConnection(header->sourceAddr, 
# 726 "tunnel/IpTunnel.c" 3 4
                                                 ((void *)0)
# 726 "tunnel/IpTunnel.c"
                                                     , 
# 726 "tunnel/IpTunnel.c" 3 4
                                                       1
# 726 "tunnel/IpTunnel.c"
                                                           , context);
    } else if (message->length > 20 && Headers_getIpVersion(message->bytes) == 4) {
        struct Headers_IP4Header* header = (struct Headers_IP4Header*) message->bytes;
        conn = findConnection(
# 729 "tunnel/IpTunnel.c" 3 4
                             ((void *)0)
# 729 "tunnel/IpTunnel.c"
                                 , header->sourceAddr, 
# 729 "tunnel/IpTunnel.c" 3 4
                                                       1
# 729 "tunnel/IpTunnel.c"
                                                           , context);
    } else {
        do { if (context->logger && Log_Level_INFO >= Log_Level_DEBUG) { Log_print(context->logger, Log_Level_INFO, "IpTunnel.c", 731, "Message of unknown type from TUN"); } } while (0);
        return 0;
    }

    if (!conn) {
        do { if (context->logger && Log_Level_INFO >= Log_Level_DEBUG) { Log_print(context->logger, Log_Level_INFO, "IpTunnel.c", 736, "Message with unrecognized address from TUN"); } } while (0);
        return 0;
    }

    return sendToNode(message, conn, context);
}

static __attribute__ ((warn_unused_result)) struct Iface* ip6FromNode(struct Message* message,
                               struct IpTunnel_Connection* conn,
                               struct IpTunnel_pvt* context)
{
    struct Headers_IP6Header* header = (struct Headers_IP6Header*) message->bytes;
    if (Bits_isZero(header->sourceAddr, 16) || Bits_isZero(header->destinationAddr, 16)) {
        if (Bits_isZero(header->sourceAddr, 32)) {
            return incomingControlMessage(message, conn, context);
        }
        do { if (context->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(context->logger, Log_Level_DEBUG, "IpTunnel.c", 752, "Got message with zero address"); } } while (0);
        return 0;
    }
    if (!isValidAddress6(header->sourceAddr, 
# 755 "tunnel/IpTunnel.c" 3 4
                                            0
# 755 "tunnel/IpTunnel.c"
                                                 , conn)) {
        uint8_t addr[40];
        AddrTools_printIp(addr, header->sourceAddr);
        do { if (context->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(context->logger, Log_Level_DEBUG, "IpTunnel.c", 758, "Got message with wrong address for connection [%s]", addr); } } while (0);
        return 0;
    }

    do {             struct Er_Ret* Er_ret = TUNMessageType_push(message, ((uint16_t)(__extension__ ({ uint16_t _x = (((uint16_t)(0x86DD))); _x >> 8 | _x << 8; }))));                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("IpTunnel.c", 762, Er_ret);                 } else {                     Er__assertFail(Er_unwind("IpTunnel.c", 762, Er_ret));                 }             }                  } while (0);
    return Iface_next(&context->pub.tunInterface, message);
}

static __attribute__ ((warn_unused_result)) struct Iface* ip4FromNode(struct Message* message,
                               struct IpTunnel_Connection* conn,
                               struct IpTunnel_pvt* context)
{
    struct Headers_IP4Header* header = (struct Headers_IP4Header*) message->bytes;
    if (Bits_isZero(header->sourceAddr, 4) || Bits_isZero(header->destAddr, 4)) {
        do { if (context->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(context->logger, Log_Level_DEBUG, "IpTunnel.c", 772, "Got message with zero address"); } } while (0);
        return 0;
    } else if (!isValidAddress4(header->sourceAddr, 
# 774 "tunnel/IpTunnel.c" 3 4
                                                   0
# 774 "tunnel/IpTunnel.c"
                                                        , conn)) {
        do { if (context->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(context->logger, Log_Level_DEBUG, "IpTunnel.c", 775, "Got message with wrong address [%d.%d.%d.%d] for connection " "[%d.%d.%d.%d/%d:%d]", header->sourceAddr[0], header->sourceAddr[1], header->sourceAddr[2], header->sourceAddr[3], conn->connectionIp4[0], conn->connectionIp4[1], conn->connectionIp4[2], conn->connectionIp4[3], conn->connectionIp4Alloc, conn->connectionIp4Prefix); } } while (0)





                                                                      ;
        return 0;
    }

    do {             struct Er_Ret* Er_ret = TUNMessageType_push(message, ((uint16_t)(__extension__ ({ uint16_t _x = (((uint16_t)(0x0800))); _x >> 8 | _x << 8; }))));                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("IpTunnel.c", 785, Er_ret);                 } else {                     Er__assertFail(Er_unwind("IpTunnel.c", 785, Er_ret));                 }             }                  } while (0);
    return Iface_next(&context->pub.tunInterface, message);
}

static __attribute__ ((warn_unused_result)) struct Iface* incomingFromNode(struct Message* message, struct Iface* nodeIf)
{
    struct IpTunnel_pvt* context =
        (__extension__ ({ const __typeof__(((struct IpTunnel_pvt*)0)->pub.nodeInterface)*__mptr = (nodeIf); (__extension__ ({ __typeof__((struct IpTunnel_pvt*)((char*)__mptr - 
# 792 "tunnel/IpTunnel.c" 3 4
       __builtin_offsetof (
# 792 "tunnel/IpTunnel.c"
       struct IpTunnel_pvt
# 792 "tunnel/IpTunnel.c" 3 4
       , 
# 792 "tunnel/IpTunnel.c"
       pub.nodeInterface
# 792 "tunnel/IpTunnel.c" 3 4
       )
# 792 "tunnel/IpTunnel.c"
       )) Identity_ptr = ((struct IpTunnel_pvt*)((char*)__mptr - 
# 792 "tunnel/IpTunnel.c" 3 4
       __builtin_offsetof (
# 792 "tunnel/IpTunnel.c"
       struct IpTunnel_pvt
# 792 "tunnel/IpTunnel.c" 3 4
       , 
# 792 "tunnel/IpTunnel.c"
       pub.nodeInterface
# 792 "tunnel/IpTunnel.c" 3 4
       )
# 792 "tunnel/IpTunnel.c"
       )); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0xe6f97dcf58e89230ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("IpTunnel.c"), (792), "(Identity_ptr->Identity_verifier == ((unsigned long) 0xe6f97dcf58e89230ull))"); } } while (0); Identity_ptr; })); }));



    do { if (!((message->length >= (56 + 12) + 4))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("IpTunnel.c"), (796), "(message->length >= (56 + 12) + 4)"); } } while (0);
    struct RouteHeader* rh = (struct RouteHeader*) message->bytes;
    struct DataHeader* dh = (struct DataHeader*) &rh[1];
    do { if (!((DataHeader_getContentType(dh) == ContentType_IPTUN))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("IpTunnel.c"), (799), "(DataHeader_getContentType(dh) == ContentType_IPTUN)"); } } while (0);
    struct IpTunnel_Connection* conn = connectionByPubKey(rh->publicKey, context);
    if (!conn) {
        if (1) {
            uint8_t addr[40];
            AddrTools_printIp(addr, rh->ip6);
            do { if (context->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(context->logger, Log_Level_DEBUG, "IpTunnel.c", 805, "Got message from unrecognized node [%s]", addr); } } while (0);
        }
        return 0;
    }

    do {             struct Er_Ret* Er_ret = Message_eshift(message, -((56 + 12) + 4));                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("IpTunnel.c", 810, Er_ret);                 } else {                     Er__assertFail(Er_unwind("IpTunnel.c", 810, Er_ret));                 }             }                  } while (0);

    if (message->length > 40 && Headers_getIpVersion(message->bytes) == 6) {
        return ip6FromNode(message, conn, context);
    }
    if (message->length > 20 && Headers_getIpVersion(message->bytes) == 4) {
        return ip4FromNode(message, conn, context);
    }

    if (1) {
        uint8_t addr[40];
        AddrTools_printIp(addr, rh->ip6);
        do { if (context->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(context->logger, Log_Level_DEBUG, "IpTunnel.c", 822, "Got message of unknown type, length: [%d], IP version [%d] from [%s]", message->length, (message->length > 1) ? Headers_getIpVersion(message->bytes) : 0, addr); } } while (0)



                       ;
    }
    return 0;
}

static void timeout(void* vcontext)
{
    struct IpTunnel_pvt* context = vcontext;
    if (!context->pub.connectionList.count) {
        return;
    }
    do { if (context->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(context->logger, Log_Level_DEBUG, "IpTunnel.c", 837, "Checking for connections to poll. Total connections [%u]", context->pub.connectionList.count); } } while (0)
                                                                  ;
    uint32_t beginning = Random_uint32(context->rand) % context->pub.connectionList.count;
    uint32_t i = beginning;
    do {
        do { if (!((i < context->pub.connectionList.count))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("IpTunnel.c"), (842), "(i < context->pub.connectionList.count)"); } } while (0);
        struct IpTunnel_Connection* conn = &context->pub.connectionList.connections[i];
        if (conn->isOutgoing
            && Bits_isZero(conn->connectionIp6, 16)
            && Bits_isZero(conn->connectionIp4, 4))
        {
            requestAddresses(conn, context);
            break;
        }
        i = (i + 1) % context->pub.connectionList.count;
    } while (i != beginning);
}

struct IpTunnel* IpTunnel_new(struct Log* logger,
                              struct EventBase* eventBase,
                              struct Allocator* alloc,
                              struct Random* rand,
                              struct RouteGen* rg,
                              struct GlobalConfig* globalConf)
{
    struct IpTunnel_pvt* context = Allocator__clone((alloc),((&(struct IpTunnel_pvt) { .pub = { .tunInterface = { .send = incomingFromTun }, .nodeInterface = { .send = incomingFromNode } }, .allocator = alloc, .logger = logger, .rand = rand, .rg = rg, .globalConf = globalConf })),sizeof(*((&(struct IpTunnel_pvt) { .pub = { .tunInterface = { .send = incomingFromTun }, .nodeInterface = { .send = incomingFromNode } }, .allocator = alloc, .logger = logger, .rand = rand, .rg = rg, .globalConf = globalConf }))),"IpTunnel.c",862)
# 872 "tunnel/IpTunnel.c"
       ;
    context->timeout = Timeout__setInterval((timeout), (context), (10000), (eventBase), (alloc), "IpTunnel.c", 873);
    (context)->Identity_verifier = ((unsigned long) 0xe6f97dcf58e89230ull);

    return &context->pub;
}
