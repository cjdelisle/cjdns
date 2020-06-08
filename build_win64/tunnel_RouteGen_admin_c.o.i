# 1 "tunnel/RouteGen_admin.c"
# 1 "/home/jojapoppa/cjdns//"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "tunnel/RouteGen_admin.c"
# 15 "tunnel/RouteGen_admin.c"
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
struct Linker_x040177609413931203;
# 33 "./util/Assert.h"
__attribute__((__format__ (__printf__, 1, 2)))
__attribute__((__noreturn__))
void Assert_failure(const char* format, ...);
# 19 "./util/Identity.h" 2
# 1 "./util/Constant.h" 1
# 33 "./util/Constant.h"

# 20 "./util/Identity.h" 2



    
# 19 "./memory/Allocator.h" 2


struct Linker_x06712323482162772;





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
struct Linker_x02023925082541005;

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

struct Linker_x015713722953872988;

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
# 16 "tunnel/RouteGen_admin.c" 2
# 1 "./benc/Dict.h" 1
# 21 "./benc/Dict.h"
struct Linker_x009291745837719789;

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
# 17 "tunnel/RouteGen_admin.c" 2
# 1 "./benc/List.h" 1
# 21 "./benc/List.h"
struct Linker_x018892178041197472;

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
# 18 "tunnel/RouteGen_admin.c" 2
# 1 "./exception/Er.h" 1
# 23 "./exception/Er.h"
struct Linker_x025879738865837965;

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
# 19 "tunnel/RouteGen_admin.c" 2

# 1 "./tunnel/RouteGen.h" 1
# 18 "./tunnel/RouteGen.h"
# 1 "./util/platform/Sockaddr.h" 1
# 19 "./util/platform/Sockaddr.h"
# 1 "./util/Endian.h" 1
# 20 "./util/platform/Sockaddr.h" 2

struct Linker_x07368638929774187;



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
# 19 "./tunnel/RouteGen.h" 2
# 1 "./exception/Except.h" 1
# 21 "./exception/Except.h"
struct Linker_x0781243003292297;



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
# 20 "./tunnel/RouteGen.h" 2

# 1 "./util/log/Log.h" 1
# 20 "./util/log/Log.h"
struct Linker_x07904874475104482;

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
# 22 "./tunnel/RouteGen.h" 2

struct Linker_x07232387455330755;



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
# 21 "tunnel/RouteGen_admin.c" 2
# 1 "./admin/Admin.h" 1
# 18 "./admin/Admin.h"
# 1 "./interface/Iface.h" 1
# 20 "./interface/Iface.h"
# 1 "./wire/Message.h" 1
# 20 "./wire/Message.h"
# 1 "./util/Bits.h" 1
# 21 "./util/Bits.h"
struct Linker_x03785117177457662;


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


struct Linker_x003605048435525582;



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
# 19 "./admin/Admin.h" 2
# 1 "./interface/addressable/AddrIface.h" 1
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




# 1 "./util/UniqueName.h" 1
# 22 "./util/UniqueName.h"

# 25 "./admin/Admin.h" 2
# 1 "./util/events/EventBase.h" 1
# 20 "./util/events/EventBase.h"
struct Linker_x06262165882830468;

struct EventBase
{
    int unused;
};

struct EventBase* EventBase_new(struct Allocator* alloc);

int EventBase_eventCount(struct EventBase* eventBase);

void EventBase_beginLoop(struct EventBase* eventBase);

void EventBase_endLoop(struct EventBase* eventBase);
# 26 "./admin/Admin.h" 2

struct Linker_x04639156313689372;



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
# 22 "tunnel/RouteGen_admin.c" 2
# 1 "./tunnel/RouteGen_admin.h" 1
# 20 "./tunnel/RouteGen_admin.h"
# 1 "./admin/Admin.h" 1
# 21 "./tunnel/RouteGen_admin.h" 2

struct Linker_x05179611076940744;

void RouteGen_admin_register(struct RouteGen* rg, struct Admin* admin, struct Allocator* alloc);
# 23 "tunnel/RouteGen_admin.c" 2
# 1 "./util/Base10.h" 1
# 21 "./util/Base10.h"
struct Linker_x07498051305987106;



__attribute__ ((warn_unused_result)) struct Er_Ret* Base10_write(struct Message* msg, int64_t num);
__attribute__ ((warn_unused_result)) struct Er_Ret* Base10_read(int64_t *Er_returnValP, struct Message* msg);
int Base10_fromString(uint8_t* str, int64_t* numOut);
# 24 "tunnel/RouteGen_admin.c" 2


struct RouteGen_admin_Ctx
{
    struct RouteGen* rg;
    struct Admin* admin;
    unsigned long Identity_verifier;
};

static int getIntVal(Dict* d, String* name)
{
    int64_t* valP = Dict_getInt(d, name);
    return (valP && (*valP > 0)) ? *valP : 0;
}


static void getSomething(Dict* args,
                         struct RouteGen_admin_Ctx* ctx,
                         String* txid,
                         struct Allocator* requestAlloc,
                         Dict* genRoutes)
{
    int page = getIntVal(args, (&(String) { .bytes = "page", .len = CString_strlen("page") }));
    List* routes;
    if (getIntVal(args, (&(String) { .bytes = "ip6", .len = CString_strlen("ip6") }))) {
        routes = Dict_getList(genRoutes, (&(String) { .bytes = "ipv6", .len = CString_strlen("ipv6") }));
    } else {
        routes = Dict_getList(genRoutes, (&(String) { .bytes = "ipv4", .len = CString_strlen("ipv4") }));
    }
    do { if (!((routes))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("RouteGen_admin.c"), (53), "(routes)"); } } while (0);
    List* outList = List_new(requestAlloc);
    
# 55 "tunnel/RouteGen_admin.c" 3 4
   _Bool 
# 55 "tunnel/RouteGen_admin.c"
        more = 
# 55 "tunnel/RouteGen_admin.c" 3 4
               0
# 55 "tunnel/RouteGen_admin.c"
                    ;
    for (int i = page * 20, j = 0; i < List_size(routes) && j < 20; j++) {
        String* route = List_getString(routes, i);
        do { if (!((route))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("RouteGen_admin.c"), (58), "(route)"); } } while (0);
        List_addString(outList, route, requestAlloc);
        if (++i >= List_size(routes)) {
            more = 
# 61 "tunnel/RouteGen_admin.c" 3 4
                  0
# 61 "tunnel/RouteGen_admin.c"
                       ;
            break;
        }
        more = 
# 64 "tunnel/RouteGen_admin.c" 3 4
              1
# 64 "tunnel/RouteGen_admin.c"
                  ;
    }
    Dict* out = Dict_new(requestAlloc);
    if (more) {
        Dict_putInt(out, String_new("more", requestAlloc), 1, requestAlloc);
    }
    Dict_putList(out, String_new("routes", requestAlloc), outList, requestAlloc);
    Admin_sendMessage(out, txid, ctx->admin);
}
# 81 "tunnel/RouteGen_admin.c"
static void getPrefixes(Dict* args, void* vcontext, String* txid, struct Allocator* requestAlloc) { struct RouteGen_admin_Ctx* ctx = (__extension__ ({ __typeof__((struct RouteGen_admin_Ctx*) vcontext) Identity_ptr = ((struct RouteGen_admin_Ctx*) vcontext); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0xf60afebc0aa81fa1ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("RouteGen_admin.c"), (81), "(Identity_ptr->Identity_verifier == ((unsigned long) 0xf60afebc0aa81fa1ull))"); } } while (0); Identity_ptr; })); Dict* genRoutes = RouteGen_getPrefixes (ctx->rg, requestAlloc); getSomething(args, ctx, txid, requestAlloc, genRoutes); }
static void getLocalPrefixes(Dict* args, void* vcontext, String* txid, struct Allocator* requestAlloc) { struct RouteGen_admin_Ctx* ctx = (__extension__ ({ __typeof__((struct RouteGen_admin_Ctx*) vcontext) Identity_ptr = ((struct RouteGen_admin_Ctx*) vcontext); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0xf60afebc0aa81fa1ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("RouteGen_admin.c"), (82), "(Identity_ptr->Identity_verifier == ((unsigned long) 0xf60afebc0aa81fa1ull))"); } } while (0); Identity_ptr; })); Dict* genRoutes = RouteGen_getLocalPrefixes (ctx->rg, requestAlloc); getSomething(args, ctx, txid, requestAlloc, genRoutes); }
static void getExceptions(Dict* args, void* vcontext, String* txid, struct Allocator* requestAlloc) { struct RouteGen_admin_Ctx* ctx = (__extension__ ({ __typeof__((struct RouteGen_admin_Ctx*) vcontext) Identity_ptr = ((struct RouteGen_admin_Ctx*) vcontext); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0xf60afebc0aa81fa1ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("RouteGen_admin.c"), (83), "(Identity_ptr->Identity_verifier == ((unsigned long) 0xf60afebc0aa81fa1ull))"); } } while (0); Identity_ptr; })); Dict* genRoutes = RouteGen_getExceptions (ctx->rg, requestAlloc); getSomething(args, ctx, txid, requestAlloc, genRoutes); }
static void getGeneratedRoutes(Dict* args, void* vcontext, String* txid, struct Allocator* requestAlloc) { struct RouteGen_admin_Ctx* ctx = (__extension__ ({ __typeof__((struct RouteGen_admin_Ctx*) vcontext) Identity_ptr = ((struct RouteGen_admin_Ctx*) vcontext); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0xf60afebc0aa81fa1ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("RouteGen_admin.c"), (84), "(Identity_ptr->Identity_verifier == ((unsigned long) 0xf60afebc0aa81fa1ull))"); } } while (0); Identity_ptr; })); Dict* genRoutes = RouteGen_getGeneratedRoutes (ctx->rg, requestAlloc); getSomething(args, ctx, txid, requestAlloc, genRoutes); }
# 93 "tunnel/RouteGen_admin.c"
enum addRemoveSomething_What {
    addRemoveSomething_What_ADD_EXCEPTION,
    addRemoveSomething_What_RM_EXCEPTION,
    addRemoveSomething_What_ADD_PREFIX,
    addRemoveSomething_What_RM_PREFIX,
    addRemoveSomething_What_ADD_LOCALPREFIX,
    addRemoveSomething_What_RM_LOCALPREFIX,
};
static void addRemoveSomething(Dict* args,
                               void* vcontext,
                               String* txid,
                               struct Allocator* requestAlloc,
                               enum addRemoveSomething_What what)
{
    struct RouteGen_admin_Ctx* ctx = (__extension__ ({ __typeof__((struct RouteGen_admin_Ctx*) vcontext) Identity_ptr = ((struct RouteGen_admin_Ctx*) vcontext); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0xf60afebc0aa81fa1ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("RouteGen_admin.c"), (107), "(Identity_ptr->Identity_verifier == ((unsigned long) 0xf60afebc0aa81fa1ull))"); } } while (0); Identity_ptr; }));
    String* route = Dict_getString(args, (&(String) { .bytes = "route", .len = CString_strlen("route") }));
    char* error = 
# 109 "tunnel/RouteGen_admin.c" 3 4
                 ((void *)0)
# 109 "tunnel/RouteGen_admin.c"
                     ;

    struct Sockaddr_storage ss;
    if (route->len > 63) {
        error = "parse_failed";
    }
    if (!error) {
        if (Sockaddr_parse(route->bytes, &ss)) {
            error = "parse_failed";
        } else {
            int family = Sockaddr_getFamily(&ss.addr);
            if (family != Sockaddr_AF_INET && family != Sockaddr_AF_INET6) {
                error = "unexpected_af";
            }
        }
    }
    int retVal = -1;
    Dict* out = Dict_new(requestAlloc);
    if (!error) {
        switch (what) {
            case addRemoveSomething_What_ADD_EXCEPTION:
                RouteGen_addException(ctx->rg, &ss.addr); break;
            case addRemoveSomething_What_ADD_PREFIX:
                RouteGen_addPrefix(ctx->rg, &ss.addr); break;
            case addRemoveSomething_What_ADD_LOCALPREFIX:
                RouteGen_addLocalPrefix(ctx->rg, &ss.addr); break;
            case addRemoveSomething_What_RM_EXCEPTION:
                retVal = RouteGen_removeException(ctx->rg, &ss.addr); break;
            case addRemoveSomething_What_RM_PREFIX:
                retVal = RouteGen_removePrefix(ctx->rg, &ss.addr); break;
            case addRemoveSomething_What_RM_LOCALPREFIX:
                retVal = RouteGen_removeLocalPrefix(ctx->rg, &ss.addr); break;
            default: Assert_failure("invalid op");
        }
        if (!retVal) {
            error = "no_such_route";
        } else {
            error = "none";
        }
    }
    Dict_putString(out,
                   String_new("error", requestAlloc),
                   String_new(error, requestAlloc),
                   requestAlloc);
    Admin_sendMessage(out, txid, ctx->admin);
}






static void addException(Dict* args, void* vcontext, String* txid, struct Allocator* requestAlloc) { addRemoveSomething(args, vcontext, txid, requestAlloc, addRemoveSomething_What_ADD_EXCEPTION); }
static void addPrefix(Dict* args, void* vcontext, String* txid, struct Allocator* requestAlloc) { addRemoveSomething(args, vcontext, txid, requestAlloc, addRemoveSomething_What_ADD_PREFIX); }
static void addLocalPrefix(Dict* args, void* vcontext, String* txid, struct Allocator* requestAlloc) { addRemoveSomething(args, vcontext, txid, requestAlloc, addRemoveSomething_What_ADD_LOCALPREFIX); }
static void removePrefix(Dict* args, void* vcontext, String* txid, struct Allocator* requestAlloc) { addRemoveSomething(args, vcontext, txid, requestAlloc, addRemoveSomething_What_RM_PREFIX); }
static void removeLocalPrefix(Dict* args, void* vcontext, String* txid, struct Allocator* requestAlloc) { addRemoveSomething(args, vcontext, txid, requestAlloc, addRemoveSomething_What_RM_LOCALPREFIX); }
static void removeException(Dict* args, void* vcontext, String* txid, struct Allocator* requestAlloc) { addRemoveSomething(args, vcontext, txid, requestAlloc, addRemoveSomething_What_RM_EXCEPTION); }






static void commit(Dict* args,
                   void* vcontext,
                   String* txid,
                   struct Allocator* requestAlloc)
{
    struct RouteGen_admin_Ctx* const ctx = (__extension__ ({ __typeof__((struct RouteGen_admin_Ctx*) vcontext) Identity_ptr = ((struct RouteGen_admin_Ctx*) vcontext); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0xf60afebc0aa81fa1ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("RouteGen_admin.c"), (178), "(Identity_ptr->Identity_verifier == ((unsigned long) 0xf60afebc0aa81fa1ull))"); } } while (0); Identity_ptr; }));
    String* const tunName = Dict_getString(args, (&(String) { .bytes = "tunName", .len = CString_strlen("tunName") }));
    Dict* const ret = Dict_new(requestAlloc);
    const char* error = "none";

    struct Er_Ret* er = 
# 183 "tunnel/RouteGen_admin.c" 3 4
                       ((void *)0)
# 183 "tunnel/RouteGen_admin.c"
                           ;
    do {             struct Er_Ret* Er_ret = RouteGen_commit(ctx->rg, tunName->bytes, requestAlloc);                          if (Er_ret) {                 struct Er_Ret** Er_errOut = &er;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("RouteGen_admin.c", 184, Er_ret);                 } else {                     Er__assertFail(Er_unwind("RouteGen_admin.c", 184, Er_ret));                 }             }                  } while (0);
    if (er) {
        error = er->message;
    }
    Dict_putString(ret, String_new("error", requestAlloc), String_new(error, requestAlloc), requestAlloc);
    Admin_sendMessage(ret, txid, ctx->admin);
}

void RouteGen_admin_register(struct RouteGen* rg, struct Admin* admin, struct Allocator* alloc)
{
    struct RouteGen_admin_Ctx* ctx = Allocator__calloc((alloc),(sizeof(struct RouteGen_admin_Ctx)),(1),"RouteGen_admin.c",194);
    ctx->rg = rg;
    ctx->admin = admin;
    (ctx)->Identity_verifier = ((unsigned long) 0xf60afebc0aa81fa1ull);

    Admin_registerFunctionWithArgCount( "RouteGen_" "getPrefixes", getPrefixes, ctx, 
# 199 "tunnel/RouteGen_admin.c" 3 4
   1
# 199 "tunnel/RouteGen_admin.c"
   , ((struct Admin_FunctionArg[]) { { .name = "page", .required = 0, .type = "Int" }, { .name = "ip6", .required = 0, .type = "Int" } }), (sizeof(((struct Admin_FunctionArg[]) { { .name = "page", .required = 0, .type = "Int" }, { .name = "ip6", .required = 0, .type = "Int" } })) / sizeof(struct Admin_FunctionArg)), admin);
    Admin_registerFunctionWithArgCount( "RouteGen_" "getLocalPrefixes", getLocalPrefixes, ctx, 
# 200 "tunnel/RouteGen_admin.c" 3 4
   1
# 200 "tunnel/RouteGen_admin.c"
   , ((struct Admin_FunctionArg[]) { { .name = "page", .required = 0, .type = "Int" }, { .name = "ip6", .required = 0, .type = "Int" } }), (sizeof(((struct Admin_FunctionArg[]) { { .name = "page", .required = 0, .type = "Int" }, { .name = "ip6", .required = 0, .type = "Int" } })) / sizeof(struct Admin_FunctionArg)), admin);
    Admin_registerFunctionWithArgCount( "RouteGen_" "getExceptions", getExceptions, ctx, 
# 201 "tunnel/RouteGen_admin.c" 3 4
   1
# 201 "tunnel/RouteGen_admin.c"
   , ((struct Admin_FunctionArg[]) { { .name = "page", .required = 0, .type = "Int" }, { .name = "ip6", .required = 0, .type = "Int" } }), (sizeof(((struct Admin_FunctionArg[]) { { .name = "page", .required = 0, .type = "Int" }, { .name = "ip6", .required = 0, .type = "Int" } })) / sizeof(struct Admin_FunctionArg)), admin);
    Admin_registerFunctionWithArgCount( "RouteGen_" "getGeneratedRoutes", getGeneratedRoutes, ctx, 
# 202 "tunnel/RouteGen_admin.c" 3 4
   1
# 202 "tunnel/RouteGen_admin.c"
   , ((struct Admin_FunctionArg[]) { { .name = "page", .required = 0, .type = "Int" }, { .name = "ip6", .required = 0, .type = "Int" } }), (sizeof(((struct Admin_FunctionArg[]) { { .name = "page", .required = 0, .type = "Int" }, { .name = "ip6", .required = 0, .type = "Int" } })) / sizeof(struct Admin_FunctionArg)), admin);

    Admin_registerFunctionWithArgCount( "RouteGen_" "addException", addException, ctx, 
# 204 "tunnel/RouteGen_admin.c" 3 4
   1
# 204 "tunnel/RouteGen_admin.c"
   , ((struct Admin_FunctionArg[]) { { .name = "route", .required = 1, .type = "String" }, }), (sizeof(((struct Admin_FunctionArg[]) { { .name = "route", .required = 1, .type = "String" }, })) / sizeof(struct Admin_FunctionArg)), admin);
    Admin_registerFunctionWithArgCount( "RouteGen_" "addPrefix", addPrefix, ctx, 
# 205 "tunnel/RouteGen_admin.c" 3 4
   1
# 205 "tunnel/RouteGen_admin.c"
   , ((struct Admin_FunctionArg[]) { { .name = "route", .required = 1, .type = "String" }, }), (sizeof(((struct Admin_FunctionArg[]) { { .name = "route", .required = 1, .type = "String" }, })) / sizeof(struct Admin_FunctionArg)), admin);
    Admin_registerFunctionWithArgCount( "RouteGen_" "addLocalPrefix", addLocalPrefix, ctx, 
# 206 "tunnel/RouteGen_admin.c" 3 4
   1
# 206 "tunnel/RouteGen_admin.c"
   , ((struct Admin_FunctionArg[]) { { .name = "route", .required = 1, .type = "String" }, }), (sizeof(((struct Admin_FunctionArg[]) { { .name = "route", .required = 1, .type = "String" }, })) / sizeof(struct Admin_FunctionArg)), admin);
    Admin_registerFunctionWithArgCount( "RouteGen_" "removePrefix", removePrefix, ctx, 
# 207 "tunnel/RouteGen_admin.c" 3 4
   1
# 207 "tunnel/RouteGen_admin.c"
   , ((struct Admin_FunctionArg[]) { { .name = "route", .required = 1, .type = "String" }, }), (sizeof(((struct Admin_FunctionArg[]) { { .name = "route", .required = 1, .type = "String" }, })) / sizeof(struct Admin_FunctionArg)), admin);
    Admin_registerFunctionWithArgCount( "RouteGen_" "removeLocalPrefix", removeLocalPrefix, ctx, 
# 208 "tunnel/RouteGen_admin.c" 3 4
   1
# 208 "tunnel/RouteGen_admin.c"
   , ((struct Admin_FunctionArg[]) { { .name = "route", .required = 1, .type = "String" }, }), (sizeof(((struct Admin_FunctionArg[]) { { .name = "route", .required = 1, .type = "String" }, })) / sizeof(struct Admin_FunctionArg)), admin);
    Admin_registerFunctionWithArgCount( "RouteGen_" "removeException", removeException, ctx, 
# 209 "tunnel/RouteGen_admin.c" 3 4
   1
# 209 "tunnel/RouteGen_admin.c"
   , ((struct Admin_FunctionArg[]) { { .name = "route", .required = 1, .type = "String" }, }), (sizeof(((struct Admin_FunctionArg[]) { { .name = "route", .required = 1, .type = "String" }, })) / sizeof(struct Admin_FunctionArg)), admin);

    Admin_registerFunctionWithArgCount( "RouteGen_commit", commit, ctx, 
# 211 "tunnel/RouteGen_admin.c" 3 4
   1
# 211 "tunnel/RouteGen_admin.c"
   , ((struct Admin_FunctionArg[]) { { .name = "tunName", .required = 1, .type = "String" }, }), (sizeof(((struct Admin_FunctionArg[]) { { .name = "tunName", .required = 1, .type = "String" }, })) / sizeof(struct Admin_FunctionArg)), admin)


                  ;
}
