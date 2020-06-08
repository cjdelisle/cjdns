# 1 "tunnel/RouteGen.c"
# 1 "/home/jojapoppa/cjdns//"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "tunnel/RouteGen.c"
# 15 "tunnel/RouteGen.c"
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
struct Linker_x0027579669038267918;
# 33 "./util/Assert.h"
__attribute__((__format__ (__printf__, 1, 2)))
__attribute__((__noreturn__))
void Assert_failure(const char* format, ...);
# 19 "./util/Identity.h" 2
# 1 "./util/Constant.h" 1
# 33 "./util/Constant.h"

# 20 "./util/Identity.h" 2



    
# 19 "./memory/Allocator.h" 2


struct Linker_x09007036767864904;





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
struct Linker_x04142584171494246;

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

struct Linker_x009356904473185845;

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
# 16 "tunnel/RouteGen.c" 2
# 1 "./benc/Dict.h" 1
# 21 "./benc/Dict.h"
struct Linker_x08279695658755037;

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
# 17 "tunnel/RouteGen.c" 2
# 1 "./util/platform/Sockaddr.h" 1
# 19 "./util/platform/Sockaddr.h"
# 1 "./util/Endian.h" 1
# 20 "./util/platform/Sockaddr.h" 2

struct Linker_x09871015269079939;



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
# 18 "tunnel/RouteGen.c" 2
# 1 "./exception/Except.h" 1
# 18 "./exception/Except.h"
# 1 "./exception/Er.h" 1
# 23 "./exception/Er.h"
struct Linker_x020355539694306524;

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


struct Linker_x012918512961408646;



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
# 19 "tunnel/RouteGen.c" 2
# 1 "./benc/List.h" 1
# 21 "./benc/List.h"
struct Linker_x019301339833008502;

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
# 20 "tunnel/RouteGen.c" 2
# 1 "./tunnel/RouteGen.h" 1
# 21 "./tunnel/RouteGen.h"
# 1 "./util/log/Log.h" 1
# 20 "./util/log/Log.h"
struct Linker_x031818780140210534;

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

struct Linker_x07482090439572961;



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
# 21 "tunnel/RouteGen.c" 2


# 1 "./util/Bits.h" 1
# 21 "./util/Bits.h"
struct Linker_x07574223497520212;


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
# 24 "tunnel/RouteGen.c" 2
# 1 "./util/platform/netdev/NetDev.h" 1
# 22 "./util/platform/netdev/NetDev.h"
struct Linker_x044187576208933543;
# 34 "./util/platform/netdev/NetDev.h"
__attribute__ ((warn_unused_result)) struct Er_Ret* NetDev_addAddress(const char* ifName, struct Sockaddr* sa, struct Log* logger, struct Allocator* alloc)


                                                ;
# 47 "./util/platform/netdev/NetDev.h"
__attribute__ ((warn_unused_result)) struct Er_Ret* NetDev_setMTU(const char* interfaceName, uint32_t mtu, struct Log* logger, struct Allocator* alloc)


                                            ;

__attribute__ ((warn_unused_result)) struct Er_Ret* NetDev_flushAddresses(const char* deviceName, struct Allocator* alloc);




__attribute__ ((warn_unused_result)) struct Er_Ret* NetDev_setRoutes(const char* ifName, struct Sockaddr** prefixSet, int prefixCount, struct Log* logger, struct Allocator* tempAlloc)



                                                   ;
# 25 "tunnel/RouteGen.c" 2

struct Prefix6
{
    uint64_t highBits;
    uint64_t lowBits;
    int prefix;
    struct Allocator* alloc;
};
static int comparePrefixes6(struct Prefix6* a, struct Prefix6* b)
{
    if (a->prefix != b->prefix) {
        return (a->prefix < b->prefix) ? -1 : 1;
    }
    if (a->highBits != b->highBits) {
        return (a->highBits < b->highBits) ? 1 : -1;
    }
    if (a->lowBits != b->lowBits) {
        return (a->lowBits < b->lowBits) ? 1 : -1;
    }
    return 0;
}



# 1 "./util/ArrayList.h" 1
# 21 "./util/ArrayList.h"
struct Linker_x002168558757256145;





struct ArrayList;

void* ArrayList_new(struct Allocator* alloc, int initialCapacity);
int ArrayList_add(struct ArrayList* list, void* val);
void* ArrayList_get(struct ArrayList* list, int number);
int ArrayList_put(struct ArrayList* list, int number, void* val);
void* ArrayList_remove(struct ArrayList* list, int num);
void ArrayList_sort(struct ArrayList* list, int (* compare)(const void* a, const void* b));
void* ArrayList_clone(struct ArrayList* vlist, struct Allocator* alloc);
# 60 "./util/ArrayList.h"
struct ArrayList_OfPrefix6 {
    int length;
};

static inline struct ArrayList_OfPrefix6* ArrayList_OfPrefix6_new(struct Allocator* alloc)
{
    return (struct ArrayList_OfPrefix6*) ArrayList_new(alloc, 8);
}

static inline struct Prefix6* ArrayList_OfPrefix6_get(struct ArrayList_OfPrefix6* list, int number)
{
    return (struct Prefix6*) ArrayList_get((struct ArrayList*) list, number);
}

static inline int ArrayList_OfPrefix6_put(struct ArrayList_OfPrefix6* list,
                                          int number,
                                          struct Prefix6* val)
{
    return ArrayList_put((struct ArrayList*) list, number, val);
}

static inline int ArrayList_OfPrefix6_add(struct ArrayList_OfPrefix6* list, void* val)
{
    return ArrayList_put((struct ArrayList*) list, list->length, val);
}

static inline struct Prefix6* ArrayList_OfPrefix6_shift(struct ArrayList_OfPrefix6* list)
{
    return (struct Prefix6*) ArrayList_remove((struct ArrayList*) list, 0);
}

static inline struct Prefix6* ArrayList_OfPrefix6_pop(struct ArrayList_OfPrefix6* list)
{
    return (struct Prefix6*) ArrayList_remove((struct ArrayList*) list, list->length - 1);
}

static inline struct Prefix6* ArrayList_OfPrefix6_remove(struct ArrayList_OfPrefix6* list, int num)
{
    return (struct Prefix6*) ArrayList_remove((struct ArrayList*) list, num);
}


static inline int ArrayList_OfPrefix6_sort_compare(const void* a, const void* b)
{
    return comparePrefixes6(((struct Prefix6**)a)[0], ((struct Prefix6**)b)[0]);
}
static inline void ArrayList_OfPrefix6_sort(struct ArrayList_OfPrefix6* list)
{
    ArrayList_sort((struct ArrayList*) list, ArrayList_OfPrefix6_sort_compare);
}



static inline void* ArrayList_OfPrefix6_clone(struct ArrayList_OfPrefix6* l, struct Allocator* alloc)
{
    return ArrayList_clone((struct ArrayList*) l, alloc);
}
# 50 "tunnel/RouteGen.c" 2

struct Prefix4
{
    uint32_t bits;
    int prefix;
    struct Allocator* alloc;
};
static int comparePrefixes4(struct Prefix4* a, struct Prefix4* b)
{
    if (a->prefix != b->prefix) {
        return (a->prefix < b->prefix) ? -1 : 1;
    }
    if (a->bits != b->bits) {
        return (a->bits < b->bits) ? 1 : -1;
    }
    return 0;
}



# 1 "./util/ArrayList.h" 1
# 60 "./util/ArrayList.h"
struct ArrayList_OfPrefix4 {
    int length;
};

static inline struct ArrayList_OfPrefix4* ArrayList_OfPrefix4_new(struct Allocator* alloc)
{
    return (struct ArrayList_OfPrefix4*) ArrayList_new(alloc, 8);
}

static inline struct Prefix4* ArrayList_OfPrefix4_get(struct ArrayList_OfPrefix4* list, int number)
{
    return (struct Prefix4*) ArrayList_get((struct ArrayList*) list, number);
}

static inline int ArrayList_OfPrefix4_put(struct ArrayList_OfPrefix4* list,
                                          int number,
                                          struct Prefix4* val)
{
    return ArrayList_put((struct ArrayList*) list, number, val);
}

static inline int ArrayList_OfPrefix4_add(struct ArrayList_OfPrefix4* list, void* val)
{
    return ArrayList_put((struct ArrayList*) list, list->length, val);
}

static inline struct Prefix4* ArrayList_OfPrefix4_shift(struct ArrayList_OfPrefix4* list)
{
    return (struct Prefix4*) ArrayList_remove((struct ArrayList*) list, 0);
}

static inline struct Prefix4* ArrayList_OfPrefix4_pop(struct ArrayList_OfPrefix4* list)
{
    return (struct Prefix4*) ArrayList_remove((struct ArrayList*) list, list->length - 1);
}

static inline struct Prefix4* ArrayList_OfPrefix4_remove(struct ArrayList_OfPrefix4* list, int num)
{
    return (struct Prefix4*) ArrayList_remove((struct ArrayList*) list, num);
}


static inline int ArrayList_OfPrefix4_sort_compare(const void* a, const void* b)
{
    return comparePrefixes4(((struct Prefix4**)a)[0], ((struct Prefix4**)b)[0]);
}
static inline void ArrayList_OfPrefix4_sort(struct ArrayList_OfPrefix4* list)
{
    ArrayList_sort((struct ArrayList*) list, ArrayList_OfPrefix4_sort_compare);
}



static inline void* ArrayList_OfPrefix4_clone(struct ArrayList_OfPrefix4* l, struct Allocator* alloc)
{
    return ArrayList_clone((struct ArrayList*) l, alloc);
}
# 71 "tunnel/RouteGen.c" 2

struct Prefix46 {
    struct ArrayList_OfPrefix4* prefix4;
    struct ArrayList_OfPrefix6* prefix6;
};

struct RouteGen_pvt
{
    struct RouteGen pub;
    struct ArrayList_OfPrefix6* prefixes6;
    struct ArrayList_OfPrefix6* localPrefixes6;
    struct ArrayList_OfPrefix6* exceptions6;

    struct ArrayList_OfPrefix4* prefixes4;
    struct ArrayList_OfPrefix4* localPrefixes4;
    struct ArrayList_OfPrefix4* exceptions4;

    struct Allocator* alloc;
    struct Log* log;

    unsigned long Identity_verifier;
};

static struct Sockaddr* sockaddrForPrefix4(struct Allocator* alloc, struct Prefix4* pfx4)
{
    union {
        uint32_t addr_be;
        uint8_t bytes[4];
    } un;
    un.addr_be = ((uint32_t)__builtin_bswap32(((uint32_t)(pfx4->bits))));
    struct Sockaddr* out = Sockaddr_fromBytes(un.bytes, Sockaddr_AF_INET, alloc);
    out->flags |= (1<<1);
    out->prefix = pfx4->prefix;
    return out;
}

static String* printPrefix4(struct Allocator* alloc, struct Prefix4* pfx4)
{
    return String_new(Sockaddr_print(sockaddrForPrefix4(alloc, pfx4), alloc), alloc);
}

static struct Sockaddr* sockaddrForPrefix6(struct Allocator* alloc, struct Prefix6* pfx6)
{
    union {
        struct {
            uint64_t highBits_be;
            uint64_t lowBits_be;
        } longs;
        uint8_t bytes[16];
    } un;
    un.longs.highBits_be = ((uint64_t)__builtin_bswap64(((uint64_t)(pfx6->highBits))));
    un.longs.lowBits_be = ((uint64_t)__builtin_bswap64(((uint64_t)(pfx6->lowBits))));
    struct Sockaddr* out = Sockaddr_fromBytes(un.bytes, Sockaddr_AF_INET6, alloc);
    out->flags |= (1<<1);
    out->prefix = pfx6->prefix;
    return out;
}

static String* printPrefix6(struct Allocator* alloc, struct Prefix6* pfx6)
{
    return String_new(Sockaddr_print(sockaddrForPrefix6(alloc, pfx6), alloc), alloc);
}

static struct Prefix4* sockaddrToPrefix4(struct Sockaddr* sa, struct Allocator* allocator)
{
    uint32_t addrNum;
    uint8_t* addr;
    do { if (!((Sockaddr_getAddress(sa, &addr) == 4))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("RouteGen.c"), (138), "(Sockaddr_getAddress(sa, &addr) == 4)"); } } while (0);
    Bits__memcpy(&addrNum,addr,4,"RouteGen.c",139);
    struct Allocator* alloc = Allocator__child((allocator),"RouteGen.c",140);
    struct Prefix4* out = Allocator__calloc((alloc),(sizeof(struct Prefix4)),(1),"RouteGen.c",141);
    out->bits = ((uint32_t)__builtin_bswap32(((uint32_t)(addrNum))));
    int pfx = Sockaddr_getPrefix(sa);
    do { if (!((pfx > -1))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("RouteGen.c"), (144), "(pfx > -1)"); } } while (0);
    out->prefix = pfx;
    out->alloc = alloc;
    return out;
}

static struct Prefix6* sockaddrToPrefix6(struct Sockaddr* sa, struct Allocator* allocator)
{
    struct {
        uint64_t highBits_be;
        uint64_t lowBits_be;
    } longs;
    uint8_t* addr;
    do { if (!((Sockaddr_getAddress(sa, &addr) == 16))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("RouteGen.c"), (157), "(Sockaddr_getAddress(sa, &addr) == 16)"); } } while (0);
    Bits__memcpy(&longs,addr,16,"RouteGen.c",158);
    struct Allocator* alloc = Allocator__child((allocator),"RouteGen.c",159);
    struct Prefix6* out = Allocator__calloc((alloc),(sizeof(struct Prefix6)),(1),"RouteGen.c",160);
    out->highBits = ((uint64_t)__builtin_bswap64(((uint64_t)(longs.highBits_be))));
    out->lowBits = ((uint64_t)__builtin_bswap64(((uint64_t)(longs.lowBits_be))));
    int pfx = Sockaddr_getPrefix(sa);
    do { if (!((pfx > -1))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("RouteGen.c"), (164), "(pfx > -1)"); } } while (0);
    out->prefix = pfx;
    out->alloc = alloc;
    return out;
}

static void addSomething(struct RouteGen_pvt* rp,
                         struct Sockaddr* exempt,
                         struct ArrayList_OfPrefix6* list6,
                         struct ArrayList_OfPrefix4* list4)
{
    if (Sockaddr_getFamily(exempt) == Sockaddr_AF_INET) {
        struct Prefix4* p4 = sockaddrToPrefix4(exempt, rp->alloc);
        ArrayList_OfPrefix4_add(list4, p4);
    } else if (Sockaddr_getFamily(exempt) == Sockaddr_AF_INET6) {
        struct Prefix6* p6 = sockaddrToPrefix6(exempt, rp->alloc);
        ArrayList_OfPrefix6_add(list6, p6);
    } else {
        Assert_failure("unexpected addr type");
    }
    rp->pub.hasUncommittedChanges = 
# 184 "tunnel/RouteGen.c" 3 4
                                   1
# 184 "tunnel/RouteGen.c"
                                       ;
}

void RouteGen_addException(struct RouteGen* rg, struct Sockaddr* destination)
{
    struct RouteGen_pvt* rp = (__extension__ ({ __typeof__((struct RouteGen_pvt*) rg) Identity_ptr = ((struct RouteGen_pvt*) rg); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x864f1c641e0af5b5ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("RouteGen.c"), (189), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x864f1c641e0af5b5ull))"); } } while (0); Identity_ptr; }));
    addSomething(rp, destination, rp->exceptions6, rp->exceptions4);
}

void RouteGen_addPrefix(struct RouteGen* rg, struct Sockaddr* destination)
{
    struct RouteGen_pvt* rp = (__extension__ ({ __typeof__((struct RouteGen_pvt*) rg) Identity_ptr = ((struct RouteGen_pvt*) rg); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x864f1c641e0af5b5ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("RouteGen.c"), (195), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x864f1c641e0af5b5ull))"); } } while (0); Identity_ptr; }));
    addSomething(rp, destination, rp->prefixes6, rp->prefixes4);
}

void RouteGen_addLocalPrefix(struct RouteGen* rg, struct Sockaddr* destination)
{
    struct RouteGen_pvt* rp = (__extension__ ({ __typeof__((struct RouteGen_pvt*) rg) Identity_ptr = ((struct RouteGen_pvt*) rg); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x864f1c641e0af5b5ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("RouteGen.c"), (201), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x864f1c641e0af5b5ull))"); } } while (0); Identity_ptr; }));
    addSomething(rp, destination, rp->localPrefixes6, rp->localPrefixes4);
}

static Dict* getSomething(struct RouteGen_pvt* rp,
                          struct Allocator* alloc,
                          struct ArrayList_OfPrefix6* list6,
                          struct ArrayList_OfPrefix4* list4)
{
    ArrayList_OfPrefix6_sort(list6);
    ArrayList_OfPrefix4_sort(list4);
    List* prefixes4 = List_new(alloc);
    for (int i = 0; i < list4->length; i++) {
        struct Prefix4* pfx4 = ArrayList_OfPrefix4_get(list4, i);
        List_addString(prefixes4, printPrefix4(alloc, pfx4), alloc);
    }
    List* prefixes6 = List_new(alloc);
    for (int i = 0; i < list6->length; i++) {
        struct Prefix6* pfx6 = ArrayList_OfPrefix6_get(list6, i);
        List_addString(prefixes6, printPrefix6(alloc, pfx6), alloc);
    }
    Dict* out = Dict_new(alloc);
    Dict_putList(out, String_new("ipv4", alloc), prefixes4, alloc);
    Dict_putList(out, String_new("ipv6", alloc), prefixes6, alloc);
    return out;
}

Dict* RouteGen_getPrefixes(struct RouteGen* rg, struct Allocator* alloc)
{
    struct RouteGen_pvt* rp = (__extension__ ({ __typeof__((struct RouteGen_pvt*) rg) Identity_ptr = ((struct RouteGen_pvt*) rg); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x864f1c641e0af5b5ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("RouteGen.c"), (230), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x864f1c641e0af5b5ull))"); } } while (0); Identity_ptr; }));
    return getSomething(rp, alloc, rp->prefixes6, rp->prefixes4);
}

Dict* RouteGen_getLocalPrefixes(struct RouteGen* rg, struct Allocator* alloc)
{
    struct RouteGen_pvt* rp = (__extension__ ({ __typeof__((struct RouteGen_pvt*) rg) Identity_ptr = ((struct RouteGen_pvt*) rg); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x864f1c641e0af5b5ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("RouteGen.c"), (236), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x864f1c641e0af5b5ull))"); } } while (0); Identity_ptr; }));
    return getSomething(rp, alloc, rp->localPrefixes6, rp->localPrefixes4);
}

Dict* RouteGen_getExceptions(struct RouteGen* rg, struct Allocator* alloc)
{
    struct RouteGen_pvt* rp = (__extension__ ({ __typeof__((struct RouteGen_pvt*) rg) Identity_ptr = ((struct RouteGen_pvt*) rg); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x864f1c641e0af5b5ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("RouteGen.c"), (242), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x864f1c641e0af5b5ull))"); } } while (0); Identity_ptr; }));
    return getSomething(rp, alloc, rp->exceptions6, rp->exceptions4);
}

static 
# 246 "tunnel/RouteGen.c" 3 4
      _Bool 
# 246 "tunnel/RouteGen.c"
           removeSomething(struct RouteGen_pvt* rp,
                            struct Sockaddr* toRemove,
                            struct ArrayList_OfPrefix6* list6,
                            struct ArrayList_OfPrefix4* list4)
{
    struct Allocator* tempAlloc = Allocator__child((rp->alloc),"RouteGen.c",251);
    
# 252 "tunnel/RouteGen.c" 3 4
   _Bool 
# 252 "tunnel/RouteGen.c"
        ret = 
# 252 "tunnel/RouteGen.c" 3 4
              0
# 252 "tunnel/RouteGen.c"
                   ;
    if (Sockaddr_getFamily(toRemove) == Sockaddr_AF_INET) {
        struct Prefix4* p4 = sockaddrToPrefix4(toRemove, tempAlloc);
        for (int i = list4->length - 1; i >= 0; i--) {
            struct Prefix4* p42 = ArrayList_OfPrefix4_get(list4, i);
            if (!comparePrefixes4(p4, p42)) {
                ArrayList_OfPrefix4_remove(list4, i);
                ret = 
# 259 "tunnel/RouteGen.c" 3 4
                     1
# 259 "tunnel/RouteGen.c"
                         ;
            }
        }
    } else if (Sockaddr_getFamily(toRemove) == Sockaddr_AF_INET6) {
        struct Prefix6* p6 = sockaddrToPrefix6(toRemove, tempAlloc);
        for (int i = list6->length - 1; i >= 0; i--) {
            struct Prefix6* p62 = ArrayList_OfPrefix6_get(list6, i);
            if (!comparePrefixes6(p6, p62)) {
                ArrayList_OfPrefix6_remove(list6, i);
                ret = 
# 268 "tunnel/RouteGen.c" 3 4
                     1
# 268 "tunnel/RouteGen.c"
                         ;
            }
        }
    } else {
        Assert_failure("unexpected addr type");
    }
    Allocator__free((tempAlloc),"RouteGen.c",274);
    return ret;
}


# 278 "tunnel/RouteGen.c" 3 4
_Bool 
# 278 "tunnel/RouteGen.c"
    RouteGen_removePrefix(struct RouteGen* rg, struct Sockaddr* toRemove)
{
    struct RouteGen_pvt* rp = (__extension__ ({ __typeof__((struct RouteGen_pvt*) rg) Identity_ptr = ((struct RouteGen_pvt*) rg); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x864f1c641e0af5b5ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("RouteGen.c"), (280), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x864f1c641e0af5b5ull))"); } } while (0); Identity_ptr; }));
    return removeSomething(rp, toRemove, rp->prefixes6, rp->prefixes4);
}


# 284 "tunnel/RouteGen.c" 3 4
_Bool 
# 284 "tunnel/RouteGen.c"
    RouteGen_removeLocalPrefix(struct RouteGen* rg, struct Sockaddr* toRemove)
{
    struct RouteGen_pvt* rp = (__extension__ ({ __typeof__((struct RouteGen_pvt*) rg) Identity_ptr = ((struct RouteGen_pvt*) rg); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x864f1c641e0af5b5ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("RouteGen.c"), (286), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x864f1c641e0af5b5ull))"); } } while (0); Identity_ptr; }));
    return removeSomething(rp, toRemove, rp->localPrefixes6, rp->localPrefixes4);
}


# 290 "tunnel/RouteGen.c" 3 4
_Bool 
# 290 "tunnel/RouteGen.c"
    RouteGen_removeException(struct RouteGen* rg, struct Sockaddr* toRemove)
{
    struct RouteGen_pvt* rp = (__extension__ ({ __typeof__((struct RouteGen_pvt*) rg) Identity_ptr = ((struct RouteGen_pvt*) rg); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x864f1c641e0af5b5ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("RouteGen.c"), (292), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x864f1c641e0af5b5ull))"); } } while (0); Identity_ptr; }));
    return removeSomething(rp, toRemove, rp->exceptions6, rp->exceptions4);
}

static struct ArrayList_OfPrefix4* invertPrefix4(struct Prefix4* toInvert, struct Allocator* alloc)
{
    struct ArrayList_OfPrefix4* result = ArrayList_OfPrefix4_new(alloc);
    for (int i = 32 - toInvert->prefix; i < 32; i++) {
        struct Prefix4* pfx = Allocator__calloc((alloc),(sizeof(struct Prefix4)),(1),"RouteGen.c",300);
        pfx->bits = ( toInvert->bits & ((uint32_t)~0 << i) ) ^ ((uint32_t)1 << i);
        pfx->prefix = 32 - i;
        ArrayList_OfPrefix4_add(result, pfx);
    }
    return result;
}

static struct ArrayList_OfPrefix6* invertPrefix6(struct Prefix6* toInvert, struct Allocator* alloc)
{
    struct ArrayList_OfPrefix6* result = ArrayList_OfPrefix6_new(alloc);
    for (int i = 128 - toInvert->prefix; i < 128; i++) {
        struct Prefix6* pfx = Allocator__calloc((alloc),(sizeof(struct Prefix6)),(1),"RouteGen.c",312);
        if (i >= 64) {
            pfx->highBits = ( toInvert->highBits & (~((uint64_t)0) << (i-64)) ) ^
                (((uint64_t)1) << (i-64));
            pfx->lowBits = 0;
        } else {
            pfx->highBits = toInvert->highBits;
            pfx->lowBits = ( toInvert->lowBits & (~((uint64_t)0) << i) ) ^ (((uint64_t)1) << i);
        }
        pfx->prefix = 128 - i;
        ArrayList_OfPrefix6_add(result, pfx);
    }
    return result;
}

static 
# 327 "tunnel/RouteGen.c" 3 4
      _Bool 
# 327 "tunnel/RouteGen.c"
           isSubsetOf4(struct Prefix4* isSubset, struct Prefix4* isSuperset)
{
    if (isSuperset->prefix > isSubset->prefix) { return 
# 329 "tunnel/RouteGen.c" 3 4
                                                       0
# 329 "tunnel/RouteGen.c"
                                                            ; }
    if (isSuperset->prefix >= 32) {
        return isSuperset->bits == isSubset->bits;
    }
    if (!isSuperset->prefix) { return 
# 333 "tunnel/RouteGen.c" 3 4
                                     1
# 333 "tunnel/RouteGen.c"
                                         ; }
    uint32_t shift = 32 - isSuperset->prefix;
    return (isSuperset->bits >> shift) == (isSubset->bits >> shift);
}

static 
# 338 "tunnel/RouteGen.c" 3 4
      _Bool 
# 338 "tunnel/RouteGen.c"
           isSubsetOf6(struct Prefix6* isSubset, struct Prefix6* isSuperset)
{
    if (isSuperset->prefix > isSubset->prefix) { return 
# 340 "tunnel/RouteGen.c" 3 4
                                                       0
# 340 "tunnel/RouteGen.c"
                                                            ; }
    if (isSuperset->prefix > 64) {
        uint64_t shift = 128 - isSuperset->prefix;
        return isSuperset->highBits == isSubset->highBits &&
            (isSuperset->lowBits >> shift) == (isSubset->lowBits >> shift);
    } else if (isSuperset->prefix) {
        uint64_t shift = 64 - isSuperset->prefix;
        return (isSuperset->highBits >> shift) == (isSubset->highBits >> shift);
    } else {
        return 
# 349 "tunnel/RouteGen.c" 3 4
              1
# 349 "tunnel/RouteGen.c"
                  ;
    }
}

static void mergePrefixSets4(struct ArrayList_OfPrefix4* mergeInto,
                             struct ArrayList_OfPrefix4* prefixes)
{
    struct Prefix4* highestPrefix = 
# 356 "tunnel/RouteGen.c" 3 4
                                   ((void *)0)
# 356 "tunnel/RouteGen.c"
                                       ;
    for (int j = 0; j < prefixes->length; j++) {
        struct Prefix4* result = ArrayList_OfPrefix4_get(prefixes, j);
        do { if (!((result))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("RouteGen.c"), (359), "(result)"); } } while (0);
        if (!highestPrefix || highestPrefix->prefix < result->prefix) {
            highestPrefix = result;
        }
    }

    struct Prefix4 target;
    Bits__memcpy(&target,highestPrefix,sizeof(struct Prefix4),"RouteGen.c",366);
    target.bits ^= (target.prefix) ? ((uint32_t)1 << (32 - target.prefix)) : 0;
    for (int i = mergeInto->length - 1; i >= 0; i--) {
        struct Prefix4* result = ArrayList_OfPrefix4_get(mergeInto, i);
        do { if (!((result))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("RouteGen.c"), (370), "(result)"); } } while (0);
        if (isSubsetOf4(&target, result)) {
            ArrayList_OfPrefix4_remove(mergeInto, i);
        }
    }

    for (int i = 0; i < prefixes->length; i++) {
        
# 377 "tunnel/RouteGen.c" 3 4
       _Bool 
# 377 "tunnel/RouteGen.c"
            include = 
# 377 "tunnel/RouteGen.c" 3 4
                      1
# 377 "tunnel/RouteGen.c"
                          ;
        struct Prefix4* toInclude = ArrayList_OfPrefix4_get(prefixes, i);
        for (int j = 0; j < mergeInto->length; j++) {
            struct Prefix4* test = ArrayList_OfPrefix4_get(mergeInto, j);
            if (isSubsetOf4(test, toInclude)) {
                include = 
# 382 "tunnel/RouteGen.c" 3 4
                         0
# 382 "tunnel/RouteGen.c"
                              ;
                break;
            }
        }
        if (include) {
            ArrayList_OfPrefix4_add(mergeInto, toInclude);
        }
    }
}

static void mergePrefixSets6(struct ArrayList_OfPrefix6* mergeInto,
                             struct ArrayList_OfPrefix6* prefixes, struct Allocator* alloc)
{
    struct Prefix6* highestPrefix = 
# 395 "tunnel/RouteGen.c" 3 4
                                   ((void *)0)
# 395 "tunnel/RouteGen.c"
                                       ;
    for (int j = 0; j < prefixes->length; j++) {
        struct Prefix6* result = ArrayList_OfPrefix6_get(prefixes, j);
        do { if (!((result))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("RouteGen.c"), (398), "(result)"); } } while (0);
        if (!highestPrefix || highestPrefix->prefix < result->prefix) {
            highestPrefix = result;
        }
    }

    struct Prefix6 target;
    Bits__memcpy(&target,highestPrefix,sizeof(struct Prefix6),"RouteGen.c",405);
    if (target.prefix > 64) {
        target.lowBits ^= (((uint64_t)1) << (128 - target.prefix));
    } else if (target.prefix) {
        target.highBits ^= (((uint64_t)1) << (64 - target.prefix));
        target.lowBits = 0;
    }

    for (int i = mergeInto->length - 1; i >= 0; i--) {
        struct Prefix6* result = ArrayList_OfPrefix6_get(mergeInto, i);
        do { if (!((result))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("RouteGen.c"), (415), "(result)"); } } while (0);
        if (isSubsetOf6(&target, result)) {
            ArrayList_OfPrefix6_remove(mergeInto, i);
        }
    }

    for (int i = 0; i < prefixes->length; i++) {
        
# 422 "tunnel/RouteGen.c" 3 4
       _Bool 
# 422 "tunnel/RouteGen.c"
            include = 
# 422 "tunnel/RouteGen.c" 3 4
                      1
# 422 "tunnel/RouteGen.c"
                          ;
        struct Prefix6* toInclude = ArrayList_OfPrefix6_get(prefixes, i);
        for (int j = 0; j < mergeInto->length; j++) {
            struct Prefix6* test = ArrayList_OfPrefix6_get(mergeInto, j);
            if (isSubsetOf6(test, toInclude)) {
                include = 
# 427 "tunnel/RouteGen.c" 3 4
                         0
# 427 "tunnel/RouteGen.c"
                              ;
                break;
            }
        }
        if (include) {
            ArrayList_OfPrefix6_add(mergeInto, toInclude);
        }
    }
}

static struct Prefix4* clonePrefix4(struct Prefix4* original, struct Allocator* alloc)
{
    struct Prefix4* clone = Allocator__clone((alloc),(original),sizeof(*(original)),"RouteGen.c",439);
    clone->alloc = alloc;
    return clone;
}

static struct Prefix6* clonePrefix6(struct Prefix6* original, struct Allocator* alloc)
{
    struct Prefix6* clone = Allocator__clone((alloc),(original),sizeof(*(original)),"RouteGen.c",446);
    clone->alloc = alloc;
    return clone;
}

static struct ArrayList_OfPrefix4* genPrefixes4(struct ArrayList_OfPrefix4* prefixes,
                                                struct ArrayList_OfPrefix4* exceptions,
                                                struct ArrayList_OfPrefix4* localPrefixes,
                                                struct Allocator* alloc)
{
    struct Allocator* tempAlloc = Allocator__child((alloc),"RouteGen.c",456);

    struct ArrayList_OfPrefix4* effectiveLocalPrefixes = ArrayList_OfPrefix4_new(tempAlloc);
    for (int i = 0; i < localPrefixes->length; i++) {
        
# 460 "tunnel/RouteGen.c" 3 4
       _Bool 
# 460 "tunnel/RouteGen.c"
            add = 
# 460 "tunnel/RouteGen.c" 3 4
                  1
# 460 "tunnel/RouteGen.c"
                      ;
        struct Prefix4* localPfx = ArrayList_OfPrefix4_get(localPrefixes, i);
        for (int j = 0; j < prefixes->length; j++) {
            struct Prefix4* pfx = ArrayList_OfPrefix4_get(prefixes, j);
            if (isSubsetOf4(pfx, localPfx)) {
                add = 
# 465 "tunnel/RouteGen.c" 3 4
                     0
# 465 "tunnel/RouteGen.c"
                          ;
                break;
            }
        }
        if (add) {
            ArrayList_OfPrefix4_add(effectiveLocalPrefixes, localPfx);
        }
    }

    struct ArrayList_OfPrefix4* allPrefixes = ArrayList_OfPrefix4_new(tempAlloc);
    for (int i = 0; i < exceptions->length; i++) {
        struct Prefix4* pfxToInvert = ArrayList_OfPrefix4_get(exceptions, i);
        
# 477 "tunnel/RouteGen.c" 3 4
       _Bool 
# 477 "tunnel/RouteGen.c"
            add = 
# 477 "tunnel/RouteGen.c" 3 4
                  1
# 477 "tunnel/RouteGen.c"
                      ;
        for (int j = 0; j < effectiveLocalPrefixes->length; j++) {
            struct Prefix4* localPfx = ArrayList_OfPrefix4_get(effectiveLocalPrefixes, j);
            if (isSubsetOf4(pfxToInvert, localPfx)) {
                add = 
# 481 "tunnel/RouteGen.c" 3 4
                     0
# 481 "tunnel/RouteGen.c"
                          ;
                break;
            }
        }
        if (add) {
            struct ArrayList_OfPrefix4* prefixes4 = invertPrefix4(pfxToInvert, tempAlloc);
            mergePrefixSets4(allPrefixes, prefixes4);
        }
    }

    for (int i = allPrefixes->length - 2; i >= 0; i--) {
        struct Prefix4* pfx = ArrayList_OfPrefix4_get(allPrefixes, i);
        struct Prefix4* pfx2 = ArrayList_OfPrefix4_get(allPrefixes, i+1);
        if (isSubsetOf4(pfx2, pfx)) {
            ArrayList_OfPrefix4_remove(allPrefixes, i+1);
            if (i < (allPrefixes->length - 2)) { i++; }
        }
    }

    for (int i = 0; i < prefixes->length; i++) {
        struct Prefix4* pfx = ArrayList_OfPrefix4_get(prefixes, i);
        int addPrefix = 
# 502 "tunnel/RouteGen.c" 3 4
                       1
# 502 "tunnel/RouteGen.c"
                           ;
        for (int j = allPrefixes->length - 1; j >= 0; j--) {
            struct Prefix4* pfx2 = ArrayList_OfPrefix4_get(allPrefixes, j);
            if (isSubsetOf4(pfx2, pfx)) {
                addPrefix = 
# 506 "tunnel/RouteGen.c" 3 4
                           0
# 506 "tunnel/RouteGen.c"
                                ;
            }
        }
        if (addPrefix) {
            ArrayList_OfPrefix4_add(allPrefixes, pfx);
        }
    }

    ArrayList_OfPrefix4_sort(allPrefixes);

    struct ArrayList_OfPrefix4* out = ArrayList_OfPrefix4_new(alloc);
    for (int i = 0; i < allPrefixes->length; i++) {
        struct Prefix4* pfx = ArrayList_OfPrefix4_get(allPrefixes, i);
        for (int j = 0; j < prefixes->length; j++) {
            struct Prefix4* pfx2 = ArrayList_OfPrefix4_get(prefixes, j);
            if (isSubsetOf4(pfx, pfx2)) {
                ArrayList_OfPrefix4_add(out, clonePrefix4(pfx, alloc));
                break;
            }
        }
    }
    Allocator__free((tempAlloc),"RouteGen.c",527);
    return out;
}



static struct ArrayList_OfPrefix6* genPrefixes6(struct ArrayList_OfPrefix6* prefixes,
                                                struct ArrayList_OfPrefix6* exceptions,
                                                struct ArrayList_OfPrefix6* localPrefixes,
                                                struct Allocator* alloc)
{
    struct Allocator* tempAlloc = Allocator__child((alloc),"RouteGen.c",538);

    struct ArrayList_OfPrefix6* effectiveLocalPrefixes = ArrayList_OfPrefix6_new(tempAlloc);
    for (int i = 0; i < localPrefixes->length; i++) {
        
# 542 "tunnel/RouteGen.c" 3 4
       _Bool 
# 542 "tunnel/RouteGen.c"
            add = 
# 542 "tunnel/RouteGen.c" 3 4
                  1
# 542 "tunnel/RouteGen.c"
                      ;
        struct Prefix6* localPfx = ArrayList_OfPrefix6_get(localPrefixes, i);
        for (int j = 0; j < prefixes->length; j++) {
            struct Prefix6* pfx = ArrayList_OfPrefix6_get(prefixes, j);
            if (isSubsetOf6(pfx, localPfx)) {
                add = 
# 547 "tunnel/RouteGen.c" 3 4
                     0
# 547 "tunnel/RouteGen.c"
                          ;
                break;
            }
        }
        if (add) {
            ArrayList_OfPrefix6_add(effectiveLocalPrefixes, localPfx);
        }
    }

    struct ArrayList_OfPrefix6* allPrefixes = ArrayList_OfPrefix6_new(tempAlloc);
    for (int i = 0; i < exceptions->length; i++) {
        struct Prefix6* pfxToInvert = ArrayList_OfPrefix6_get(exceptions, i);
        
# 559 "tunnel/RouteGen.c" 3 4
       _Bool 
# 559 "tunnel/RouteGen.c"
            add = 
# 559 "tunnel/RouteGen.c" 3 4
                  1
# 559 "tunnel/RouteGen.c"
                      ;
        for (int j = 0; j < effectiveLocalPrefixes->length; j++) {
            struct Prefix6* localPfx = ArrayList_OfPrefix6_get(effectiveLocalPrefixes, j);
            if (isSubsetOf6(pfxToInvert, localPfx)) {
                add = 
# 563 "tunnel/RouteGen.c" 3 4
                     0
# 563 "tunnel/RouteGen.c"
                          ;
                break;
            }
        }
        if (add) {
            struct ArrayList_OfPrefix6* prefixes6 = invertPrefix6(pfxToInvert, tempAlloc);
            mergePrefixSets6(allPrefixes, prefixes6, alloc);
        }
    }

    ArrayList_OfPrefix6_sort(allPrefixes);

    for (int i = allPrefixes->length - 2; i >= 0; i--) {
        struct Prefix6* pfx = ArrayList_OfPrefix6_get(allPrefixes, i);
        struct Prefix6* pfx2 = ArrayList_OfPrefix6_get(allPrefixes, i+1);
        if (isSubsetOf6(pfx2, pfx)) {
            ArrayList_OfPrefix6_remove(allPrefixes, i+1);
            if (i < (allPrefixes->length - 2)) { i++; }
        }
    }

    for (int i = 0; i < prefixes->length; i++) {
        struct Prefix6* pfx = ArrayList_OfPrefix6_get(prefixes, i);
        int addPrefix = 
# 586 "tunnel/RouteGen.c" 3 4
                       1
# 586 "tunnel/RouteGen.c"
                           ;
        for (int j = allPrefixes->length - 1; j >= 0; j--) {
            struct Prefix6* pfx2 = ArrayList_OfPrefix6_get(allPrefixes, j);
            if (isSubsetOf6(pfx2, pfx)) {
                addPrefix = 
# 590 "tunnel/RouteGen.c" 3 4
                           0
# 590 "tunnel/RouteGen.c"
                                ;
            }
        }
        if (addPrefix) {
            ArrayList_OfPrefix6_add(allPrefixes, pfx);
        }
    }

    ArrayList_OfPrefix6_sort(allPrefixes);

    struct ArrayList_OfPrefix6* out = ArrayList_OfPrefix6_new(alloc);
    for (int i = 0; i < allPrefixes->length; i++) {
        struct Prefix6* pfx = ArrayList_OfPrefix6_get(allPrefixes, i);
        for (int j = 0; j < prefixes->length; j++) {
            struct Prefix6* pfx2 = ArrayList_OfPrefix6_get(prefixes, j);
            if (isSubsetOf6(pfx, pfx2)) {
                ArrayList_OfPrefix6_add(out, clonePrefix6(pfx, alloc));
                break;
            }
        }
    }
    Allocator__free((tempAlloc),"RouteGen.c",611);
    return out;
}

static struct Prefix46* getGeneratedRoutes(struct RouteGen_pvt* rp, struct Allocator* alloc)
{
    struct Prefix46* out = Allocator__calloc((alloc),(sizeof(struct Prefix46)),(1),"RouteGen.c",617);
    if (rp->prefixes4->length > 0) {
        out->prefix4 = genPrefixes4(rp->prefixes4, rp->exceptions4, rp->localPrefixes4, alloc);
    } else {
        out->prefix4 = ArrayList_OfPrefix4_new(alloc);
    }
    if (rp->prefixes6->length > 0) {
        out->prefix6 = genPrefixes6(rp->prefixes6, rp->exceptions6, rp->localPrefixes6, alloc);
    } else {
        out->prefix6 = ArrayList_OfPrefix6_new(alloc);
    }
    return out;
}

Dict* RouteGen_getGeneratedRoutes(struct RouteGen* rg, struct Allocator* alloc)
{
    struct RouteGen_pvt* rp = (__extension__ ({ __typeof__((struct RouteGen_pvt*) rg) Identity_ptr = ((struct RouteGen_pvt*) rg); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x864f1c641e0af5b5ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("RouteGen.c"), (633), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x864f1c641e0af5b5ull))"); } } while (0); Identity_ptr; }));
    struct Prefix46* p46 = getGeneratedRoutes(rp, alloc);
    return getSomething(rp, alloc, p46->prefix6, p46->prefix4);
}

__attribute__ ((warn_unused_result)) struct Er_Ret* RouteGen_commit(struct RouteGen* rg, const char* tunName, struct Allocator* tempAlloc)


{
    struct RouteGen_pvt* rp = (__extension__ ({ __typeof__((struct RouteGen_pvt*) rg) Identity_ptr = ((struct RouteGen_pvt*) rg); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x864f1c641e0af5b5ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("RouteGen.c"), (642), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x864f1c641e0af5b5ull))"); } } while (0); Identity_ptr; }));
    struct Prefix46* p46 = getGeneratedRoutes(rp, tempAlloc);
    struct Sockaddr** prefixSet =
        Allocator__calloc((tempAlloc),(sizeof(char*)),(p46->prefix4->length + p46->prefix6->length),"RouteGen.c",645);
    int prefixNum = 0;
    for (int i = 0; i < p46->prefix4->length; i++) {
        struct Prefix4* pfx4 = ArrayList_OfPrefix4_get(p46->prefix4, i);
        prefixSet[prefixNum++] = sockaddrForPrefix4(tempAlloc, pfx4);
    }
    for (int i = 0; i < p46->prefix6->length; i++) {
        struct Prefix6* pfx6 = ArrayList_OfPrefix6_get(p46->prefix6, i);
        prefixSet[prefixNum++] = sockaddrForPrefix6(tempAlloc, pfx6);
    }
    do { if (!((prefixNum == p46->prefix4->length + p46->prefix6->length))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("RouteGen.c"), (655), "(prefixNum == p46->prefix4->length + p46->prefix6->length)"); } } while (0);
    do {             struct Er_Ret* Er_ret = NetDev_setRoutes(tunName, prefixSet, prefixNum, rp->log, tempAlloc);             if (Er_ret) { return Er_unwind("RouteGen.c", 656, Er_ret); }         } while (0);
    rp->pub.hasUncommittedChanges = 
# 657 "tunnel/RouteGen.c" 3 4
                                   0
# 657 "tunnel/RouteGen.c"
                                        ;
    return (struct Er_Ret*)0;
}

static void setupDefaultLocalPrefixes(struct RouteGen_pvt* rp)
{
    struct Sockaddr_storage ss;




    do { if (!((!Sockaddr_parse("fe80::/10", &ss)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("RouteGen.c"), (668), "(!Sockaddr_parse(\"fe80::/10\", &ss))"); } } while (0); RouteGen_addLocalPrefix(&rp->pub, &ss.addr);
    do { if (!((!Sockaddr_parse("fd00::/8", &ss)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("RouteGen.c"), (669), "(!Sockaddr_parse(\"fd00::/8\", &ss))"); } } while (0); RouteGen_addLocalPrefix(&rp->pub, &ss.addr);

    do { if (!((!Sockaddr_parse("10.0.0.0/8", &ss)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("RouteGen.c"), (671), "(!Sockaddr_parse(\"10.0.0.0/8\", &ss))"); } } while (0); RouteGen_addLocalPrefix(&rp->pub, &ss.addr);
    do { if (!((!Sockaddr_parse("172.16.0.0/12", &ss)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("RouteGen.c"), (672), "(!Sockaddr_parse(\"172.16.0.0/12\", &ss))"); } } while (0); RouteGen_addLocalPrefix(&rp->pub, &ss.addr);
    do { if (!((!Sockaddr_parse("192.168.0.0/16", &ss)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("RouteGen.c"), (673), "(!Sockaddr_parse(\"192.168.0.0/16\", &ss))"); } } while (0); RouteGen_addLocalPrefix(&rp->pub, &ss.addr);
    do { if (!((!Sockaddr_parse("127.0.0.0/8", &ss)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("RouteGen.c"), (674), "(!Sockaddr_parse(\"127.0.0.0/8\", &ss))"); } } while (0); RouteGen_addLocalPrefix(&rp->pub, &ss.addr);


}

struct RouteGen* RouteGen_new(struct Allocator* allocator, struct Log* log)
{
    struct Allocator* alloc = Allocator__child((allocator),"RouteGen.c",681);
    struct RouteGen_pvt* rp = Allocator__calloc((alloc),(sizeof(struct RouteGen_pvt)),(1),"RouteGen.c",682);
    rp->prefixes6 = ArrayList_OfPrefix6_new(alloc);
    rp->localPrefixes6 = ArrayList_OfPrefix6_new(alloc);
    rp->exceptions6 = ArrayList_OfPrefix6_new(alloc);
    rp->prefixes4 = ArrayList_OfPrefix4_new(alloc);
    rp->localPrefixes4 = ArrayList_OfPrefix4_new(alloc);
    rp->exceptions4 = ArrayList_OfPrefix4_new(alloc);
    rp->log = log;
    rp->alloc = alloc;
    (rp)->Identity_verifier = ((unsigned long) 0x864f1c641e0af5b5ull);
    setupDefaultLocalPrefixes(rp);
    return &rp->pub;
}
