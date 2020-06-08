# 1 "switch/EncodingScheme.c"
# 1 "/home/jojapoppa/cjdns//"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "switch/EncodingScheme.c"
# 15 "switch/EncodingScheme.c"
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
struct Linker_x03293418323918915;
# 33 "./util/Assert.h"
__attribute__((__format__ (__printf__, 1, 2)))
__attribute__((__noreturn__))
void Assert_failure(const char* format, ...);
# 19 "./util/Identity.h" 2
# 1 "./util/Constant.h" 1
# 33 "./util/Constant.h"

# 20 "./util/Identity.h" 2



    
# 19 "./memory/Allocator.h" 2


struct Linker_x08164115477215748;





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
struct Linker_x0951478744528431;

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

struct Linker_x06212291189821195;

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
# 16 "switch/EncodingScheme.c" 2
# 1 "./benc/Dict.h" 1
# 21 "./benc/Dict.h"
struct Linker_x023920684504749112;

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
# 17 "switch/EncodingScheme.c" 2

# 1 "./switch/EncodingScheme.h" 1
# 19 "./switch/EncodingScheme.h"
# 1 "./benc/List.h" 1
# 21 "./benc/List.h"
struct Linker_x03893398941144217;

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


struct Linker_x0953412709099035;



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
# 19 "switch/EncodingScheme.c" 2
# 1 "./util/Bits.h" 1
# 21 "./util/Bits.h"
struct Linker_x0038849420752490316;


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
# 20 "switch/EncodingScheme.c" 2
# 1 "./util/Endian.h" 1
# 21 "switch/EncodingScheme.c" 2
# 1 "./util/Hex.h" 1
# 21 "./util/Hex.h"
struct Linker_x00013005642341241686;
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
# 22 "switch/EncodingScheme.c" 2

int EncodingScheme_getFormNum(struct EncodingScheme* scheme, uint64_t routeLabel)
{
    if (scheme->count == 1) {
        return 0;
    }

    for (int i = 0; i < scheme->count; i++) {
        struct EncodingScheme_Form* form = &scheme->forms[i];
        do { if (!((form->prefixLen > 0 && form->prefixLen < 32))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("EncodingScheme.c"), (31), "(form->prefixLen > 0 && form->prefixLen < 32)"); } } while (0);
        do { if (!((form->bitCount > 0 && form->bitCount < 32))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("EncodingScheme.c"), (32), "(form->bitCount > 0 && form->bitCount < 32)"); } } while (0);
        if (0 == ((form->prefix ^ (uint32_t)routeLabel) << (32 - form->prefixLen))) {
            return i;
        }
    }
    return -1;
}


# 40 "switch/EncodingScheme.c" 3 4
_Bool 
# 40 "switch/EncodingScheme.c"
    EncodingScheme_is358(struct EncodingScheme* scheme)
{
    struct EncodingScheme_Form v358[3] = {
        { .bitCount = 3, .prefixLen = 1, .prefix = 1, },
        { .bitCount = 5, .prefixLen = 2, .prefix = 1<<1, },
        { .bitCount = 8, .prefixLen = 2, .prefix = 0, }
    };
    if (scheme->count != 3) { return 
# 47 "switch/EncodingScheme.c" 3 4
                                    0
# 47 "switch/EncodingScheme.c"
                                         ; }
    for (int i = 0; i < 3; i++) {
        if (__builtin_memcmp(&v358[i],&scheme->forms[i],sizeof(struct EncodingScheme_Form))) {
            return 
# 50 "switch/EncodingScheme.c" 3 4
                  0
# 50 "switch/EncodingScheme.c"
                       ;
        }
    }
    return 
# 53 "switch/EncodingScheme.c" 3 4
          1
# 53 "switch/EncodingScheme.c"
              ;
}

int EncodingScheme_parseDirector(struct EncodingScheme* scheme, uint64_t label)
{
    int formNum = EncodingScheme_getFormNum(scheme, label);
    if (formNum == -1) {
        return -1;
    }
    struct EncodingScheme_Form* currentForm = &scheme->forms[formNum];

    int dir = (label >> currentForm->prefixLen) & Bits_maxBits64(currentForm->bitCount);

    if (!EncodingScheme_is358(scheme)) {

        return dir ^ (scheme->count > 1);
    } else {

        if (formNum > 0) {
            dir += (dir > 0);
        } else {
            dir += (dir == 0) - (dir == 1);
        }
        return dir;
    }
}

uint64_t EncodingScheme_serializeDirector(struct EncodingScheme* scheme, int dir, int formNum)
{
    if (!EncodingScheme_is358(scheme)) {
        if (formNum < 0) {
            for (formNum = 0; formNum < scheme->count; formNum++) {
                if (!(dir >> scheme->forms[formNum].bitCount)) { break; }
            }
        }

        dir ^= (scheme->count > 1);
    } else {
        if (formNum < 0) {
            for (formNum = 0; formNum < scheme->count; formNum++) {
                if (!((dir - (!!formNum)) >> scheme->forms[formNum].bitCount)) { break; }
            }
        }

        if (formNum) {

            dir -= (dir > 0);
        } else {

            dir += (dir == 0) - (dir == 1);
        }
    }

    if (formNum >= scheme->count) { return ~0ull; }

    struct EncodingScheme_Form* f = &scheme->forms[formNum];
    return (dir << f->prefixLen) | f->prefix;
}

uint64_t EncodingScheme_convertLabel(struct EncodingScheme* scheme,
                                     uint64_t routeLabel,
                                     int convertTo)
{
    int formNum = EncodingScheme_getFormNum(scheme, routeLabel);
    if (formNum == -1) {
        return (~((uint64_t)0));
    }

    struct EncodingScheme_Form* currentForm = &scheme->forms[formNum];

    if (scheme->count == 1
        || (routeLabel & Bits_maxBits64(currentForm->prefixLen + currentForm->bitCount)) == 1)
    {

        switch (convertTo) {
            case 0:
            case (-5000): return routeLabel;
            default: return (~((uint64_t)0));
        }
    }

    routeLabel >>= currentForm->prefixLen;
    uint64_t director = routeLabel & Bits_maxBits64(currentForm->bitCount);
    routeLabel >>= currentForm->bitCount;






    if (!EncodingScheme_is358(scheme)) {

    } else if ((currentForm->prefix & Bits_maxBits64(currentForm->prefixLen)) == 1) {


        director = director - (director == 1) + (director == 0);
    } else {

        director += (director > 0);
    }

    if (convertTo == (-5000)) {


        int minBitsA = Bits_log2x64(director) + 1;
        int minBitsB = Bits_log2x64(director - (director > 0)) + 1;
        for (int i = 0; i < scheme->count; i++) {
            struct EncodingScheme_Form* form = &scheme->forms[i];
            int minBits = ((form->prefix & Bits_maxBits64(form->prefixLen)) == 1)
                ? minBitsA : minBitsB;
            if (form->bitCount >= minBits) {
                convertTo = i;
                break;
            }
        }
    }

    if (convertTo < 0 || convertTo >= scheme->count) {

        return (~((uint64_t)0));
    }

    struct EncodingScheme_Form* nextForm = &scheme->forms[convertTo];

    if (!EncodingScheme_is358(scheme)) {

    } else if ((nextForm->prefix & Bits_maxBits64(nextForm->prefixLen)) == 1) {

        director = director - (director == 1) + (director == 0);
    } else {

        director -= (director > 0);
    }

    if ((Bits_log2x64(director) + 1) > nextForm->bitCount) {

        return (~((uint64_t)0));
    }
    if (Bits_log2x64(routeLabel) + EncodingScheme_formSize(nextForm) > 59) {
        return (~((uint64_t)0));
    }

    routeLabel <<= nextForm->bitCount;
    routeLabel |= director;
    routeLabel <<= nextForm->prefixLen;
    routeLabel |= nextForm->prefix;

    if ((routeLabel & Bits_maxBits64(nextForm->prefixLen + nextForm->bitCount)) == 1) {

        return (~((uint64_t)0));
    }

    return routeLabel;
}
# 231 "switch/EncodingScheme.c"
static inline int decodeForm(struct EncodingScheme_Form* out, uint64_t d)
{
    out->prefixLen = d & Bits_maxBits64(5);
    d >>= 5;
    int bitCount = d & Bits_maxBits64(5);
    if (bitCount < 1) {
        return 0;
    }
    out->bitCount = bitCount;
    d >>= 5;
    out->prefix = d & Bits_maxBits64(out->prefixLen);
    return 5 + 5 + out->prefixLen;
}

static inline int encodeForm(struct EncodingScheme_Form* in, uint64_t* data, int bits)
{
    *data |= ((uint64_t)in->prefixLen & Bits_maxBits64(5)) << bits;
    bits += 5;

    *data |= ((uint64_t)in->bitCount & Bits_maxBits64(5)) << bits;
    bits += 5;

    *data |= ((uint64_t)in->prefix & Bits_maxBits64(in->prefixLen)) << bits;

    return 5 + 5 + in->prefixLen;
}


# 258 "switch/EncodingScheme.c" 3 4
_Bool 
# 258 "switch/EncodingScheme.c"
    EncodingScheme_isSane(struct EncodingScheme* scheme)
{

    if (scheme->count == 0) {

        return 
# 263 "switch/EncodingScheme.c" 3 4
              0
# 263 "switch/EncodingScheme.c"
                   ;
    }

    if (scheme->count > 31) {



        return 
# 270 "switch/EncodingScheme.c" 3 4
              0
# 270 "switch/EncodingScheme.c"
                   ;
    }

    if (scheme->count == 1) {

        if (scheme->forms[0].prefixLen != 0 || scheme->forms[0].prefix != 0) {

            return 
# 277 "switch/EncodingScheme.c" 3 4
                  0
# 277 "switch/EncodingScheme.c"
                       ;
        }
        if (scheme->forms[0].bitCount == 0 || scheme->forms[0].bitCount > 31) {

            return 
# 281 "switch/EncodingScheme.c" 3 4
                  0
# 281 "switch/EncodingScheme.c"
                       ;
        }
        return 
# 283 "switch/EncodingScheme.c" 3 4
              1
# 283 "switch/EncodingScheme.c"
                  ;
    }


    for (int i = 0; i < scheme->count; i++) {
        struct EncodingScheme_Form* form = &scheme->forms[i];

        if (form->prefixLen == 0 || form->prefixLen > 31) {

            return 
# 292 "switch/EncodingScheme.c" 3 4
                  0
# 292 "switch/EncodingScheme.c"
                       ;
        }
        if (form->bitCount == 0 || form->bitCount > 31) {

            return 
# 296 "switch/EncodingScheme.c" 3 4
                  0
# 296 "switch/EncodingScheme.c"
                       ;
        }
        if (EncodingScheme_formSize(form) > 59) {

            return 
# 300 "switch/EncodingScheme.c" 3 4
                  0
# 300 "switch/EncodingScheme.c"
                       ;
        }
        if (i > 0 && form->bitCount <= scheme->forms[i-1].bitCount) {

            return 
# 304 "switch/EncodingScheme.c" 3 4
                  0
# 304 "switch/EncodingScheme.c"
                       ;
        }
        for (int j = 0; j < scheme->count; j++) {

            if (j != i
                && (scheme->forms[j].prefix & Bits_maxBits64(form->prefixLen)) == form->prefix)
            {
                return 
# 311 "switch/EncodingScheme.c" 3 4
                      0
# 311 "switch/EncodingScheme.c"
                           ;
            }
        }
    }
    return 
# 315 "switch/EncodingScheme.c" 3 4
          1
# 315 "switch/EncodingScheme.c"
              ;
}

List* EncodingScheme_asList(struct EncodingScheme* list, struct Allocator* alloc)
{
    do { if (!((EncodingScheme_isSane(list)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("EncodingScheme.c"), (320), "(EncodingScheme_isSane(list))"); } } while (0);
    List* scheme = List_new(alloc);
    for (int i = (int)list->count - 1; i >= 0; i--) {
        Dict* form = Dict_new(alloc);
        Dict_putInt(form, String_new("prefixLen", alloc), list->forms[i].prefixLen, alloc);
        Dict_putInt(form, String_new("bitCount", alloc), list->forms[i].bitCount, alloc);
        if (list->forms[i].prefixLen == 0) {
            Dict_putString(form, String_new("prefix", alloc), String_new("", alloc), alloc);
        } else {
            String* pfx = String_newBinary(
# 329 "switch/EncodingScheme.c" 3 4
                                          ((void *)0)
# 329 "switch/EncodingScheme.c"
                                              , 8, alloc);
            uint32_t prefix_be = ((uint32_t)__builtin_bswap32(((uint32_t)(list->forms[i].prefix))));
            Hex_encode(pfx->bytes, 8, (uint8_t*)&prefix_be, 4);
            while (pfx->bytes[0] == '0' && pfx->len > 2) {
                pfx->bytes += 2;
                pfx->len -= 2;
            }
            Dict_putString(form, String_new("prefix", alloc), pfx, alloc);
        }
        List_addDict(scheme, form, alloc);
    }
    return scheme;
}

struct EncodingScheme* EncodingScheme_fromList(List* scheme, struct Allocator* alloc)
{
    struct EncodingScheme* list = Allocator__malloc((alloc),(sizeof(struct EncodingScheme)),"EncodingScheme.c",345);
    list->count = List_size(scheme);
    list->forms = Allocator__malloc((alloc),(sizeof(struct EncodingScheme_Form) * list->count),"EncodingScheme.c",347);
    for (int i = 0; i < (int)list->count; i++) {
        Dict* form = List_getDict(scheme, i);
        uint64_t* prefixLen = Dict_getInt(form, (&(String) { .bytes = "prefixLen", .len = CString_strlen("prefixLen") }));
        uint64_t* bitCount = Dict_getInt(form, (&(String) { .bytes = "bitCount", .len = CString_strlen("bitCount") }));
        String* prefixStr = Dict_getString(form, (&(String) { .bytes = "prefix", .len = CString_strlen("prefix") }));
        if (!prefixLen || !bitCount || !prefixStr || prefixStr->len != 8) {
            return 
# 354 "switch/EncodingScheme.c" 3 4
                  ((void *)0)
# 354 "switch/EncodingScheme.c"
                      ;
        }
        uint32_t prefix_be;
        if (Hex_decode((uint8_t*)&prefix_be, 4, prefixStr->bytes, 8) != 4) {
            return 
# 358 "switch/EncodingScheme.c" 3 4
                  ((void *)0)
# 358 "switch/EncodingScheme.c"
                      ;
        }
        list->forms[i].prefixLen = *prefixLen;
        list->forms[i].bitCount = *bitCount;
        list->forms[i].prefix = ((uint32_t)__builtin_bswap32(((uint32_t)(prefix_be))));
    }
    if (!EncodingScheme_isSane(list)) {
        return 
# 365 "switch/EncodingScheme.c" 3 4
              ((void *)0)
# 365 "switch/EncodingScheme.c"
                  ;
    }
    return list;
}

String* EncodingScheme_serialize(struct EncodingScheme* list,
                                 struct Allocator* alloc)
{
    do { if (!((EncodingScheme_isSane(list)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("EncodingScheme.c"), (373), "(EncodingScheme_isSane(list))"); } } while (0);


    String* out = String_newBinary(
# 376 "switch/EncodingScheme.c" 3 4
                                  ((void *)0)
# 376 "switch/EncodingScheme.c"
                                      , list->count * 6, alloc);

    int bits = 0;
    int outIndex = 0;
    uint64_t block = 0;
    for (int listIndex = 0; listIndex < (int)list->count; listIndex++) {
        bits += encodeForm(&list->forms[listIndex], &block, bits);
        while (bits > 8) {
            do { if (!((outIndex < (int)out->len))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("EncodingScheme.c"), (384), "(outIndex < (int)out->len)"); } } while (0);
            out->bytes[outIndex++] = (uint8_t) (block & 0xff);
            bits -= 8;
            block >>= 8;
        }
    }

    if (bits > 0) {
        out->bytes[outIndex++] = (uint8_t) (block & 0xff);
    }

    out->len = outIndex;

    return out;
}

struct EncodingScheme* EncodingScheme_deserialize(String* data,
                                                  struct Allocator* alloc)
{
    struct EncodingScheme_Form* forms = 
# 403 "switch/EncodingScheme.c" 3 4
                                       ((void *)0)
# 403 "switch/EncodingScheme.c"
                                           ;
    int outCount = 0;

    uint64_t block = 0;
    int bits = 0;
    int dataIndex = 0;

    for (;;) {

        while (bits < 56 && dataIndex < (int)data->len) {
            block |= (((uint64_t)data->bytes[dataIndex++] & 0xff) << bits);
            bits += 8;
        }
        struct EncodingScheme_Form next;
        int ret = decodeForm(&next, block);
        bits -= ret;
        if (!ret || bits < 0) {
            if (block || dataIndex < (int)data->len || bits < 0) {

                return 
# 422 "switch/EncodingScheme.c" 3 4
                      ((void *)0)
# 422 "switch/EncodingScheme.c"
                          ;
            }
            break;
        }
        block >>= ret;

        do { if (!(((next.prefix >> next.prefixLen) == 0))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("EncodingScheme.c"), (428), "((next.prefix >> next.prefixLen) == 0)"); } } while (0);

        outCount += 1;
        forms = Allocator__realloc((alloc),(forms),(outCount * sizeof(struct EncodingScheme_Form)),"EncodingScheme.c",431);
        Bits__memcpy(&forms[outCount-1],&next,sizeof(struct EncodingScheme_Form),"EncodingScheme.c",432);
    }

    struct EncodingScheme* out = Allocator__clone((alloc),((&(struct EncodingScheme) { .forms = forms, .count = outCount })),sizeof(*((&(struct EncodingScheme) { .forms = forms, .count = outCount }))),"EncodingScheme.c",435)


       ;

    return EncodingScheme_isSane(out) ? out : 
# 440 "switch/EncodingScheme.c" 3 4
                                             ((void *)0)
# 440 "switch/EncodingScheme.c"
                                                 ;
}

struct EncodingScheme* EncodingScheme_defineFixedWidthScheme(int bitCount, struct Allocator* alloc)
{
    struct NumberCompress_FixedWidthScheme
    {
        struct EncodingScheme scheme;
        struct EncodingScheme_Form form;
    };

    struct NumberCompress_FixedWidthScheme* out =
        Allocator__malloc((alloc),(sizeof(struct NumberCompress_FixedWidthScheme)),"EncodingScheme.c",452);

    struct NumberCompress_FixedWidthScheme scheme = {
        .scheme = { .count = 1, .forms = &out->form },
        .form = { .bitCount = bitCount, .prefixLen = 0, .prefix = 0, },
    };
    Bits__memcpy(out,&scheme,sizeof(struct NumberCompress_FixedWidthScheme),"EncodingScheme.c",458);

    do { if (!((EncodingScheme_isSane(&out->scheme)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("EncodingScheme.c"), (460), "(EncodingScheme_isSane(&out->scheme))"); } } while (0);

    return &out->scheme;
}


struct EncodingScheme* EncodingScheme_defineDynWidthScheme(struct EncodingScheme_Form* forms,
                                                           int formCount,
                                                           struct Allocator* alloc)
{
    struct EncodingScheme_Form* formsCopy =
        Allocator__malloc((alloc),(sizeof(struct EncodingScheme_Form) * formCount),"EncodingScheme.c",471);
    Bits__memcpy(formsCopy,forms,sizeof(struct EncodingScheme_Form) * formCount,"EncodingScheme.c",472);

    struct EncodingScheme* scheme = Allocator__clone((alloc),((&(struct EncodingScheme) { .count = formCount, .forms = formsCopy })),sizeof(*((&(struct EncodingScheme) { .count = formCount, .forms = formsCopy }))),"EncodingScheme.c",474)


       ;

    do { if (!((EncodingScheme_isSane(scheme)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("EncodingScheme.c"), (479), "(EncodingScheme_isSane(scheme))"); } } while (0);
    return scheme;
}

int EncodingScheme_compare(struct EncodingScheme* a, struct EncodingScheme* b)
{
    if (a->count == b->count) {
        return __builtin_memcmp(a->forms,b->forms,sizeof(struct EncodingScheme_Form) * a->count);
    }
    return a->count > b->count ? 1 : -1;
}




int EncodingScheme_isSelfRoute(struct EncodingScheme* scheme, uint64_t routeLabel)
{
    int formNum = EncodingScheme_getFormNum(scheme, routeLabel);
    if (formNum == -1) {
        return 0;
    }

    struct EncodingScheme_Form* currentForm = &scheme->forms[formNum];

    return (routeLabel & Bits_maxBits64(currentForm->prefixLen + currentForm->bitCount)) == 1;
}

int EncodingScheme_isOneHop(struct EncodingScheme* scheme, uint64_t routeLabel)
{
    int fn = EncodingScheme_getFormNum(scheme, routeLabel);
    if (fn == -1) { return 0; }
    struct EncodingScheme_Form* form = &scheme->forms[fn];
    return (Bits_log2x64(routeLabel) == form->prefixLen + form->bitCount);
}
