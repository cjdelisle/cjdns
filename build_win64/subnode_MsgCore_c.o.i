# 1 "subnode/MsgCore.c"
# 1 "/home/jojapoppa/cjdns//"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "subnode/MsgCore.c"
# 15 "subnode/MsgCore.c"
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
struct Linker_x07178083666129302;
# 33 "./util/Assert.h"
__attribute__((__format__ (__printf__, 1, 2)))
__attribute__((__noreturn__))
void Assert_failure(const char* format, ...);
# 19 "./util/Identity.h" 2
# 1 "./util/Constant.h" 1
# 33 "./util/Constant.h"

# 20 "./util/Identity.h" 2



    
# 19 "./memory/Allocator.h" 2


struct Linker_x04708837772173833;





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

struct Linker_x06676499943359004;

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
# 16 "subnode/MsgCore.c" 2
# 1 "./crypto/AddressCalc.h" 1
# 19 "./crypto/AddressCalc.h"
struct Linker_x09390803291941896;


# 1 "/usr/lib/gcc/x86_64-w64-mingw32/9.2-win32/include/stdbool.h" 1 3 4
# 23 "./crypto/AddressCalc.h" 2
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
# 17 "subnode/MsgCore.c" 2

# 1 "./dht/Address.h" 1
# 18 "./dht/Address.h"
# 1 "./benc/String.h" 1
# 20 "./benc/String.h"
# 1 "./util/CString.h" 1
# 21 "./util/CString.h"
struct Linker_x09977889360487295;

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

struct Linker_x041058357460471906;


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


struct Linker_x05094094396212985;
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
# 19 "subnode/MsgCore.c" 2
# 1 "./dht/CJDHTConstants.h" 1
# 19 "./dht/CJDHTConstants.h"
# 1 "./util/version/Version.h" 1
# 19 "./util/version/Version.h"
struct Linker_x04249143202353671;
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
# 20 "subnode/MsgCore.c" 2
# 1 "./util/Pinger.h" 1
# 20 "./util/Pinger.h"
# 1 "./util/log/Log.h" 1
# 20 "./util/log/Log.h"
struct Linker_x0187126786666429;

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
# 21 "./util/Pinger.h" 2
# 1 "./crypto/random/Random.h" 1
# 19 "./crypto/random/Random.h"
# 1 "./exception/Except.h" 1
# 18 "./exception/Except.h"
# 1 "./exception/Er.h" 1
# 23 "./exception/Er.h"
struct Linker_x07209327243282146;

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


struct Linker_x08024080126821227;



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

# 1 "./crypto/random/seed/RandomSeed.h" 1
# 21 "./crypto/random/seed/RandomSeed.h"
struct Linker_x0061394693968741976;



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

struct Linker_x06442289905137508;



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
# 22 "./util/Pinger.h" 2
# 1 "./util/events/EventBase.h" 1
# 20 "./util/events/EventBase.h"
struct Linker_x01165618353010005;

struct EventBase
{
    int unused;
};

struct EventBase* EventBase_new(struct Allocator* alloc);

int EventBase_eventCount(struct EventBase* eventBase);

void EventBase_beginLoop(struct EventBase* eventBase);

void EventBase_endLoop(struct EventBase* eventBase);
# 23 "./util/Pinger.h" 2

struct Linker_x015745143019406727;
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
# 21 "subnode/MsgCore.c" 2
# 1 "./subnode/MsgCore.h" 1
# 18 "./subnode/MsgCore.h"
# 1 "./interface/Iface.h" 1
# 20 "./interface/Iface.h"
# 1 "./wire/Message.h" 1
# 20 "./wire/Message.h"
# 1 "./util/Bits.h" 1
# 21 "./util/Bits.h"
struct Linker_x05704197496528494;


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

struct Linker_x09920492842592241;



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
struct Linker_x010723260586112127;

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


struct Linker_x03383306530840806;



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


struct Linker_x0791355185137097;

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
# 22 "subnode/MsgCore.c" 2
# 1 "./benc/serialization/standard/BencMessageReader.h" 1
# 23 "./benc/serialization/standard/BencMessageReader.h"
struct Linker_x035772072582430425;

__attribute__ ((warn_unused_result)) struct Er_Ret* BencMessageReader_read(Dict* *Er_returnValP, struct Message* msg, struct Allocator* alloc);

const char* BencMessageReader_readNoExcept(
    struct Message* msg, struct Allocator* alloc, Dict** outPtr);
# 23 "subnode/MsgCore.c" 2
# 1 "./benc/serialization/standard/BencMessageWriter.h" 1
# 22 "./benc/serialization/standard/BencMessageWriter.h"
struct Linker_x034449716838903854;

__attribute__ ((warn_unused_result)) struct Er_Ret* BencMessageWriter_write(Dict* toWrite, struct Message* msg);
# 24 "subnode/MsgCore.c" 2

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
# 26 "subnode/MsgCore.c" 2


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
# 29 "subnode/MsgCore.c" 2
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
# 30 "subnode/MsgCore.c" 2



struct ReplyContext
{
    struct Address* src;
    Dict* content;
    struct Message* msg;
    unsigned long Identity_verifier;
};

struct QueryHandler
{
    struct MsgCore_Handler pub;
    String* queryType;
    struct Allocator* alloc;
    struct MsgCore_pvt* mcp;
    unsigned long Identity_verifier;
};



# 1 "./util/ArrayList.h" 1
# 21 "./util/ArrayList.h"
struct Linker_x020715988776743655;





struct ArrayList;

void* ArrayList_new(struct Allocator* alloc, int initialCapacity);
int ArrayList_add(struct ArrayList* list, void* val);
void* ArrayList_get(struct ArrayList* list, int number);
int ArrayList_put(struct ArrayList* list, int number, void* val);
void* ArrayList_remove(struct ArrayList* list, int num);
void ArrayList_sort(struct ArrayList* list, int (* compare)(const void* a, const void* b));
void* ArrayList_clone(struct ArrayList* vlist, struct Allocator* alloc);
# 60 "./util/ArrayList.h"
struct ArrayList_OfQueryHandlers {
    int length;
};

static inline struct ArrayList_OfQueryHandlers* ArrayList_OfQueryHandlers_new(struct Allocator* alloc)
{
    return (struct ArrayList_OfQueryHandlers*) ArrayList_new(alloc, 8);
}

static inline struct QueryHandler* ArrayList_OfQueryHandlers_get(struct ArrayList_OfQueryHandlers* list, int number)
{
    return (struct QueryHandler*) ArrayList_get((struct ArrayList*) list, number);
}

static inline int ArrayList_OfQueryHandlers_put(struct ArrayList_OfQueryHandlers* list,
                                          int number,
                                          struct QueryHandler* val)
{
    return ArrayList_put((struct ArrayList*) list, number, val);
}

static inline int ArrayList_OfQueryHandlers_add(struct ArrayList_OfQueryHandlers* list, void* val)
{
    return ArrayList_put((struct ArrayList*) list, list->length, val);
}

static inline struct QueryHandler* ArrayList_OfQueryHandlers_shift(struct ArrayList_OfQueryHandlers* list)
{
    return (struct QueryHandler*) ArrayList_remove((struct ArrayList*) list, 0);
}

static inline struct QueryHandler* ArrayList_OfQueryHandlers_pop(struct ArrayList_OfQueryHandlers* list)
{
    return (struct QueryHandler*) ArrayList_remove((struct ArrayList*) list, list->length - 1);
}

static inline struct QueryHandler* ArrayList_OfQueryHandlers_remove(struct ArrayList_OfQueryHandlers* list, int num)
{
    return (struct QueryHandler*) ArrayList_remove((struct ArrayList*) list, num);
}
# 113 "./util/ArrayList.h"
static inline void* ArrayList_OfQueryHandlers_clone(struct ArrayList_OfQueryHandlers* l, struct Allocator* alloc)
{
    return ArrayList_clone((struct ArrayList*) l, alloc);
}
# 53 "subnode/MsgCore.c" 2

struct MsgCore_pvt
{
    struct MsgCore pub;
    struct ArrayList_OfQueryHandlers* qh;
    struct Pinger* pinger;
    struct Log* log;
    String* schemeDefinition;
    struct EncodingScheme* scheme;


    struct ReplyContext* currentReply;

    unsigned long Identity_verifier;
};

struct MsgCore_Promise_pvt
{
    struct MsgCore_Promise pub;
    struct MsgCore_pvt* mcp;
    struct Pinger_Ping* ping;
    unsigned long Identity_verifier;
};

static __attribute__ ((warn_unused_result)) struct Iface* replyMsg(struct MsgCore_pvt* mcp,
                            Dict* content,
                            struct Address* src,
                            struct Message* msg)
{
    do { if (mcp->log && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(mcp->log, Log_Level_DEBUG, "MsgCore.c", 82, "Got reply from [%s]", Address_toString(src, msg->alloc)->bytes); } } while (0);
    String* txid = Dict_getString(content, (&(String) { .bytes = "txid", .len = CString_strlen("txid") }));
    if (!txid) {
        do { if (mcp->log && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(mcp->log, Log_Level_DEBUG, "MsgCore.c", 85, "DROP Message with no txid"); } } while (0);
        return 
# 86 "subnode/MsgCore.c" 3 4
              ((void *)0)
# 86 "subnode/MsgCore.c"
                  ;
    }

    struct ReplyContext* rc = Allocator__calloc((msg->alloc),(sizeof(struct ReplyContext)),(1),"MsgCore.c",89);
    rc->src = src;
    rc->content = content;
    rc->msg = msg;
    (rc)->Identity_verifier = ((unsigned long) 0x9b5549fdd37e1714ull);
    do { if (!((!mcp->currentReply))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("MsgCore.c"), (94), "(!mcp->currentReply)"); } } while (0);
    mcp->currentReply = rc;

    Pinger_pongReceived(txid, mcp->pinger);
    mcp->currentReply = 
# 98 "subnode/MsgCore.c" 3 4
                       ((void *)0)
# 98 "subnode/MsgCore.c"
                           ;
    return 
# 99 "subnode/MsgCore.c" 3 4
          ((void *)0)
# 99 "subnode/MsgCore.c"
              ;
}

static void pingerOnResponse(String* data, uint32_t milliseconds, void* context)
{
    struct MsgCore_Promise_pvt* pp = (__extension__ ({ __typeof__((struct MsgCore_Promise_pvt*) context) Identity_ptr = ((struct MsgCore_Promise_pvt*) context); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x9b5549fdd37e1714ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("MsgCore.c"), (104), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x9b5549fdd37e1714ull))"); } } while (0); Identity_ptr; }));
    struct MsgCore_pvt* mcp = (__extension__ ({ __typeof__(pp->mcp) Identity_ptr = (pp->mcp); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x9b5549fdd37e1714ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("MsgCore.c"), (105), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x9b5549fdd37e1714ull))"); } } while (0); Identity_ptr; }));
    struct ReplyContext* rc = 
# 106 "subnode/MsgCore.c" 3 4
                             ((void *)0)
# 106 "subnode/MsgCore.c"
                                 ;
    if (mcp->currentReply) {
        rc = (__extension__ ({ __typeof__(mcp->currentReply) Identity_ptr = (mcp->currentReply); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x9b5549fdd37e1714ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("MsgCore.c"), (108), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x9b5549fdd37e1714ull))"); } } while (0); Identity_ptr; }));
    }
    pp->pub.lag = milliseconds;

    if (pp->pub.cb) {
        pp->pub.cb((rc) ? rc->content : 
# 113 "subnode/MsgCore.c" 3 4
                                       ((void *)0)
# 113 "subnode/MsgCore.c"
                                           ,
                   (rc) ? rc->src : 
# 114 "subnode/MsgCore.c" 3 4
                                   ((void *)0)
# 114 "subnode/MsgCore.c"
                                       ,
                   &pp->pub);
    }
}

static void sendMsg(struct MsgCore_pvt* mcp,
                    Dict* msgDict,
                    struct Address* addr,
                    struct Allocator* allocator)
{
    struct Allocator* alloc = Allocator__child((allocator),"MsgCore.c",124);


    Dict_putString(msgDict, CJDHTConstants_ENC_SCHEME, mcp->schemeDefinition, allocator);


    int encIdx = EncodingScheme_getFormNum(mcp->scheme, addr->path);
    do { if (!((encIdx != -1))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("MsgCore.c"), (131), "(encIdx != -1)"); } } while (0);
    Dict_putInt(msgDict, CJDHTConstants_ENC_INDEX, encIdx, allocator);


    Dict_putInt(msgDict, CJDHTConstants_PROTOCOL, 20, allocator);

    if (!0) {
        String* q = Dict_getString(msgDict, (&(String) { .bytes = "q", .len = CString_strlen("q") }));
        String* sq = Dict_getString(msgDict, (&(String) { .bytes = "sq", .len = CString_strlen("sq") }));
        if (q || sq) {
            do { if (mcp->log && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(mcp->log, Log_Level_DEBUG, "MsgCore.c", 141, "Send query [%s] to [%s]", ((q) ? q->bytes : sq->bytes), Address_toString(addr, alloc)->bytes); } } while (0)

                                                     ;
            String* txid = Dict_getString(msgDict, (&(String) { .bytes = "txid", .len = CString_strlen("txid") }));
            do { if (!((txid))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("MsgCore.c"), (145), "(txid)"); } } while (0);
            String* newTxid = String_newBinary(
# 146 "subnode/MsgCore.c" 3 4
                                              ((void *)0)
# 146 "subnode/MsgCore.c"
                                                  , txid->len + 1, alloc);
            Bits__memcpy(&newTxid->bytes[1],txid->bytes,txid->len,"MsgCore.c",147);
            newTxid->bytes[0] = '1';
            if (String_equals(q, (&(String) { .bytes = "gp", .len = CString_strlen("gp") }))) {

                newTxid->bytes[0] = '0';
            }
            Dict_putString(msgDict, String_new("txid", alloc), newTxid, alloc);
        }
    }

    struct Message* msg = Message_new(0, 2048, alloc);
    do {             struct Er_Ret* Er_ret = BencMessageWriter_write(msgDict, msg);                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("MsgCore.c", 158, Er_ret);                 } else {                     Er__assertFail(Er_unwind("MsgCore.c", 158, Er_ret));                 }             }                  } while (0);




    do { if (!((AddressCalc_validAddress(addr->ip6.bytes)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("MsgCore.c"), (163), "(AddressCalc_validAddress(addr->ip6.bytes))"); } } while (0);

    struct DataHeader data;
    __builtin_memset(&data,0,sizeof(struct DataHeader));
    DataHeader_setVersion(&data, 1);
    DataHeader_setContentType(&data, ContentType_CJDHT);
    do {             struct Er_Ret* Er_ret = Message_epush(msg, &data, sizeof(struct DataHeader));                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("MsgCore.c", 169, Er_ret);                 } else {                     Er__assertFail(Er_unwind("MsgCore.c", 169, Er_ret));                 }             }                  } while (0);

    struct RouteHeader route;
    __builtin_memset(&route,0,sizeof(struct RouteHeader));
    Bits__memcpy(route.ip6,addr->ip6.bytes,16,"MsgCore.c",173);
    route.version_be = ((uint32_t)__builtin_bswap32(((uint32_t)(addr->protocolVersion))));
    route.sh.label_be = ((uint64_t)__builtin_bswap64(((uint64_t)(addr->path))));
    Bits__memcpy(route.publicKey,addr->key,32,"MsgCore.c",176);
    do {             struct Er_Ret* Er_ret = Message_epush(msg, &route, sizeof(struct RouteHeader));                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("MsgCore.c", 177, Er_ret);                 } else {                     Er__assertFail(Er_unwind("MsgCore.c", 177, Er_ret));                 }             }                  } while (0);

    Iface_send(&mcp->pub.interRouterIf, msg);
}

void MsgCore_sendResponse(struct MsgCore* core,
                          Dict* msgDict,
                          struct Address* target,
                          struct Allocator* alloc)
{
    struct MsgCore_pvt* mcp = (__extension__ ({ __typeof__((struct MsgCore_pvt*) core) Identity_ptr = ((struct MsgCore_pvt*) core); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x9b5549fdd37e1714ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("MsgCore.c"), (187), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x9b5549fdd37e1714ull))"); } } while (0); Identity_ptr; }));
    sendMsg(mcp, msgDict, target, alloc);
}

static void pingerSendPing(String* data, void* context)
{
    struct MsgCore_Promise_pvt* pp = (__extension__ ({ __typeof__((struct MsgCore_Promise_pvt*) context) Identity_ptr = ((struct MsgCore_Promise_pvt*) context); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x9b5549fdd37e1714ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("MsgCore.c"), (193), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x9b5549fdd37e1714ull))"); } } while (0); Identity_ptr; }));
    do { if (!((pp->pub.target))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("MsgCore.c"), (194), "(pp->pub.target)"); } } while (0);
    do { if (!((pp->pub.msg))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("MsgCore.c"), (195), "(pp->pub.msg)"); } } while (0);
    Dict_putString(pp->pub.msg, String_new("txid", pp->pub.alloc), data, pp->pub.alloc);
    sendMsg(pp->mcp, pp->pub.msg, pp->pub.target, pp->pub.alloc);
}

struct MsgCore_Promise* MsgCore_createQuery(struct MsgCore* core,
                                            uint32_t timeoutMilliseconds,
                                            struct Allocator* allocator)
{
    struct MsgCore_pvt* mcp = (__extension__ ({ __typeof__((struct MsgCore_pvt*) core) Identity_ptr = ((struct MsgCore_pvt*) core); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x9b5549fdd37e1714ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("MsgCore.c"), (204), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x9b5549fdd37e1714ull))"); } } while (0); Identity_ptr; }));
    if (!timeoutMilliseconds) {
        timeoutMilliseconds = 6000;
    }
    struct Pinger_Ping* p = Pinger_newPing(
        
# 209 "subnode/MsgCore.c" 3 4
       ((void *)0)
# 209 "subnode/MsgCore.c"
           , pingerOnResponse, pingerSendPing, timeoutMilliseconds, allocator, mcp->pinger);
    struct MsgCore_Promise_pvt* out =
        Allocator__calloc((p->pingAlloc),(sizeof(struct MsgCore_Promise_pvt)),(1),"MsgCore.c",211);
    (out)->Identity_verifier = ((unsigned long) 0x9b5549fdd37e1714ull);
    p->context = out;
    out->pub.alloc = p->pingAlloc;
    out->mcp = mcp;
    out->ping = p;
    return &out->pub;
}

static __attribute__ ((warn_unused_result)) struct Iface* queryMsg(struct MsgCore_pvt* mcp,
                            Dict* content,
                            struct Address* src,
                            struct Message* msg)
{
    String* q = Dict_getString(content, (&(String) { .bytes = "q", .len = CString_strlen("q") }));
    struct QueryHandler* qh = 
# 226 "subnode/MsgCore.c" 3 4
                             ((void *)0)
# 226 "subnode/MsgCore.c"
                                 ;
    for (int i = 0; i < mcp->qh->length; i++) {
        struct QueryHandler* qhx = ArrayList_OfQueryHandlers_get(mcp->qh, i);
        (__extension__ ({ __typeof__(qhx) Identity_ptr = (qhx); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x9b5549fdd37e1714ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("MsgCore.c"), (229), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x9b5549fdd37e1714ull))"); } } while (0); Identity_ptr; }));
        if (String_equals(qhx->queryType, q)) {
            qh = qhx;
            break;
        }
    }
    if (!qh) {
        do { if (mcp->log && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(mcp->log, Log_Level_DEBUG, "MsgCore.c", 236, "Unhandled query type [%s]", q->bytes); } } while (0);
        return 
# 237 "subnode/MsgCore.c" 3 4
              ((void *)0)
# 237 "subnode/MsgCore.c"
                  ;
    }
    if (!qh->pub.cb) {
        do { if (mcp->log && Log_Level_INFO >= Log_Level_DEBUG) { Log_print(mcp->log, Log_Level_INFO, "MsgCore.c", 240, "Query handler for [%s] not setup", q->bytes); } } while (0);
        return 
# 241 "subnode/MsgCore.c" 3 4
              ((void *)0)
# 241 "subnode/MsgCore.c"
                  ;
    }
    qh->pub.cb(content, src, msg->alloc, &qh->pub);
    return 
# 244 "subnode/MsgCore.c" 3 4
          ((void *)0)
# 244 "subnode/MsgCore.c"
              ;
}

static int qhOnFree(struct Allocator_OnFreeJob* job)
{
    struct QueryHandler* qh = (__extension__ ({ __typeof__((struct QueryHandler*) job->userData) Identity_ptr = ((struct QueryHandler*) job->userData); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x9b5549fdd37e1714ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("MsgCore.c"), (249), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x9b5549fdd37e1714ull))"); } } while (0); Identity_ptr; }));
    struct MsgCore_pvt* mcp = (__extension__ ({ __typeof__((struct MsgCore_pvt*) qh->mcp) Identity_ptr = ((struct MsgCore_pvt*) qh->mcp); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x9b5549fdd37e1714ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("MsgCore.c"), (250), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x9b5549fdd37e1714ull))"); } } while (0); Identity_ptr; }));
    for (int i = 0; i < mcp->qh->length; i++) {
        struct QueryHandler* qhx = ArrayList_OfQueryHandlers_get(mcp->qh, i);
        if (qhx == qh) {
            ArrayList_OfQueryHandlers_remove(mcp->qh, i);
            return 0;
        }
    }
    return 0;
}

struct MsgCore_Handler* MsgCore_onQuery(struct MsgCore* core,
                                        char* queryType,
                                        struct Allocator* allocator)
{
    struct MsgCore_pvt* mcp = (__extension__ ({ __typeof__((struct MsgCore_pvt*) core) Identity_ptr = ((struct MsgCore_pvt*) core); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x9b5549fdd37e1714ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("MsgCore.c"), (265), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x9b5549fdd37e1714ull))"); } } while (0); Identity_ptr; }));
    struct Allocator* alloc = Allocator__child((allocator),"MsgCore.c",266);
    struct QueryHandler* qh = Allocator__calloc((alloc),(sizeof(struct QueryHandler)),(1),"MsgCore.c",267);
    qh->queryType = String_new(queryType, alloc);
    qh->alloc = alloc;
    qh->mcp = mcp;
    (qh)->Identity_verifier = ((unsigned long) 0x9b5549fdd37e1714ull);
    ArrayList_OfQueryHandlers_add(mcp->qh, qh);
    Allocator__onFree((alloc), (qhOnFree), (qh), "MsgCore.c", 273);
    return &qh->pub;
}

static __attribute__ ((warn_unused_result)) struct Iface* incoming(struct Message* msg, struct Iface* interRouterIf)
{
    struct MsgCore_pvt* mcp =
        (__extension__ ({ const __typeof__(((struct MsgCore_pvt*)0)->pub.interRouterIf)*__mptr = (interRouterIf); (__extension__ ({ __typeof__((struct MsgCore_pvt*)((char*)__mptr - 
# 280 "subnode/MsgCore.c" 3 4
       __builtin_offsetof (
# 280 "subnode/MsgCore.c"
       struct MsgCore_pvt
# 280 "subnode/MsgCore.c" 3 4
       , 
# 280 "subnode/MsgCore.c"
       pub.interRouterIf
# 280 "subnode/MsgCore.c" 3 4
       )
# 280 "subnode/MsgCore.c"
       )) Identity_ptr = ((struct MsgCore_pvt*)((char*)__mptr - 
# 280 "subnode/MsgCore.c" 3 4
       __builtin_offsetof (
# 280 "subnode/MsgCore.c"
       struct MsgCore_pvt
# 280 "subnode/MsgCore.c" 3 4
       , 
# 280 "subnode/MsgCore.c"
       pub.interRouterIf
# 280 "subnode/MsgCore.c" 3 4
       )
# 280 "subnode/MsgCore.c"
       )); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x9b5549fdd37e1714ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("MsgCore.c"), (280), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x9b5549fdd37e1714ull))"); } } while (0); Identity_ptr; })); }));

    struct Address addr = { .padding = 0 };
    struct RouteHeader* hdr = (struct RouteHeader*) msg->bytes;
    do {             struct Er_Ret* Er_ret = Message_eshift(msg, -((56 + 12) + 4));                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("MsgCore.c", 284, Er_ret);                 } else {                     Er__assertFail(Er_unwind("MsgCore.c", 284, Er_ret));                 }             }                  } while (0);
    Bits__memcpy(addr.ip6.bytes,hdr->ip6,16,"MsgCore.c",285);
    Bits__memcpy(addr.key,hdr->publicKey,32,"MsgCore.c",286);
    addr.protocolVersion = ((uint32_t)__builtin_bswap32(((uint32_t)(hdr->version_be))));
    addr.path = ((uint64_t)__builtin_bswap64(((uint64_t)(hdr->sh.label_be))));

    Dict* content = 
# 290 "subnode/MsgCore.c" 3 4
                   ((void *)0)
# 290 "subnode/MsgCore.c"
                       ;
    uint8_t* msgBytes = msg->bytes;
    int length = msg->length;




    BencMessageReader_readNoExcept(msg, msg->alloc, &content);
    if (!content) {
        char* esc = Escape_getEscaped(msgBytes, length, msg->alloc);
        do { if (mcp->log && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(mcp->log, Log_Level_DEBUG, "MsgCore.c", 300, "DROP Malformed message [%s]", esc); } } while (0);
        return 
# 301 "subnode/MsgCore.c" 3 4
              ((void *)0)
# 301 "subnode/MsgCore.c"
                  ;
    }

    int64_t* verP = Dict_getInt(content, (&(String) { .bytes = "p", .len = CString_strlen("p") }));
    if (!verP) {
        do { if (mcp->log && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(mcp->log, Log_Level_DEBUG, "MsgCore.c", 306, "DROP Message without version"); } } while (0);
        return 
# 307 "subnode/MsgCore.c" 3 4
              ((void *)0)
# 307 "subnode/MsgCore.c"
                  ;
    }
    addr.protocolVersion = *verP;
    if (!addr.protocolVersion) {
        do { if (mcp->log && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(mcp->log, Log_Level_DEBUG, "MsgCore.c", 311, "DROP Message with zero version"); } } while (0);
        return 
# 312 "subnode/MsgCore.c" 3 4
              ((void *)0)
# 312 "subnode/MsgCore.c"
                  ;
    }

    String* q = Dict_getString(content, (&(String) { .bytes = "q", .len = CString_strlen("q") }));

    String* txid = Dict_getString(content, (&(String) { .bytes = "txid", .len = CString_strlen("txid") }));
    if (!txid || !txid->len) {
        do { if (mcp->log && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(mcp->log, Log_Level_DEBUG, "MsgCore.c", 319, "Message with no txid [%s]", q ? (q->bytes) : "(no query)"); } } while (0);
        return 
# 320 "subnode/MsgCore.c" 3 4
              ((void *)0)
# 320 "subnode/MsgCore.c"
                  ;
    }

    if (!0) {
        if (q) {
            if (txid->bytes[0] != '1') {
                do { if (mcp->log && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(mcp->log, Log_Level_DEBUG, "MsgCore.c", 326, "DROP query which begins with 0 and is for old pathfinder"); } } while (0);
                return 
# 327 "subnode/MsgCore.c" 3 4
                      ((void *)0)
# 327 "subnode/MsgCore.c"
                          ;
            }
        } else {
            String* newTxid = String_newBinary(
# 330 "subnode/MsgCore.c" 3 4
                                              ((void *)0)
# 330 "subnode/MsgCore.c"
                                                  , txid->len - 1, msg->alloc);
            Bits__memcpy(newTxid->bytes,&txid->bytes[1],txid->len - 1,"MsgCore.c",331);
            Dict_putString(content, String_new("txid", msg->alloc), newTxid, msg->alloc);
            txid = newTxid;
        }
    }

    if (q) {
        return queryMsg(mcp, content, &addr, msg);
    } else {
        return replyMsg(mcp, content, &addr, msg);
    }
}

struct MsgCore* MsgCore_new(struct EventBase* base,
                            struct Random* rand,
                            struct Allocator* allocator,
                            struct Log* log,
                            struct EncodingScheme* scheme)
{
    struct Allocator* alloc = Allocator__child((allocator),"MsgCore.c",350);
    struct MsgCore_pvt* mcp = Allocator__calloc((alloc),(sizeof(struct MsgCore_pvt)),(1),"MsgCore.c",351);
    (mcp)->Identity_verifier = ((unsigned long) 0x9b5549fdd37e1714ull);
    mcp->pub.interRouterIf.send = incoming;
    mcp->qh = ArrayList_OfQueryHandlers_new(alloc);
    mcp->pinger = Pinger_new(base, rand, log, alloc);
    mcp->log = log;

    mcp->scheme = scheme;
    mcp->schemeDefinition = EncodingScheme_serialize(scheme, alloc);

    return &mcp->pub;
}
