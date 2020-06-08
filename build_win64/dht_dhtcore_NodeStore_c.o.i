# 1 "dht/dhtcore/NodeStore.c"
# 1 "/home/jojapoppa/cjdns//"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "dht/dhtcore/NodeStore.c"
# 15 "dht/dhtcore/NodeStore.c"
# 1 "./dht/Address.h" 1
# 18 "./dht/Address.h"
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
struct Linker_x0575281025118316;
# 33 "./util/Assert.h"
__attribute__((__format__ (__printf__, 1, 2)))
__attribute__((__noreturn__))
void Assert_failure(const char* format, ...);
# 19 "./util/Identity.h" 2
# 1 "./util/Constant.h" 1
# 33 "./util/Constant.h"

# 20 "./util/Identity.h" 2



    
# 19 "./memory/Allocator.h" 2


struct Linker_x025219775547576284;





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
struct Linker_x06058658440449751;

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

struct Linker_x05697378836302622;

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
# 19 "./dht/Address.h" 2


struct Linker_x011578382652637309;
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
# 16 "dht/dhtcore/NodeStore.c" 2
# 1 "./dht/dhtcore/Node.h" 1
# 23 "./dht/dhtcore/Node.h"
# 1 "./switch/EncodingScheme.h" 1
# 19 "./switch/EncodingScheme.h"
# 1 "./benc/List.h" 1
# 21 "./benc/List.h"
struct Linker_x03369658509496254;

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


struct Linker_x08572538704979047;



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



struct Linker_x006684094598115764;

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
# 17 "dht/dhtcore/NodeStore.c" 2
# 1 "./dht/dhtcore/NodeStore.h" 1
# 24 "./dht/dhtcore/NodeStore.h"
# 1 "./dht/dhtcore/RumorMill.h" 1
# 24 "./dht/dhtcore/RumorMill.h"
# 1 "./util/log/Log.h" 1
# 20 "./util/log/Log.h"
struct Linker_x019834006062776344;

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
# 25 "./dht/dhtcore/RumorMill.h" 2


struct Linker_x08066730148034165;
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
struct Linker_x07688756399077206;

struct EventBase
{
    int unused;
};

struct EventBase* EventBase_new(struct Allocator* alloc);

int EventBase_eventCount(struct EventBase* eventBase);

void EventBase_beginLoop(struct EventBase* eventBase);

void EventBase_endLoop(struct EventBase* eventBase);
# 30 "./dht/dhtcore/NodeStore.h" 2
struct Linker_x02550519891785028;





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
# 18 "dht/dhtcore/NodeStore.c" 2
# 1 "./dht/dhtcore/NodeList.h" 1
# 25 "./dht/dhtcore/NodeList.h"
struct NodeList
{

    uint32_t size;


    struct Node_Two** nodes;
};
# 19 "dht/dhtcore/NodeStore.c" 2
# 1 "./util/AddrTools.h" 1
# 19 "./util/AddrTools.h"
struct Linker_x07344283182935447;




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
# 20 "dht/dhtcore/NodeStore.c" 2

# 1 "./util/Bits.h" 1
# 21 "./util/Bits.h"
struct Linker_x013561807970913575;


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
# 22 "dht/dhtcore/NodeStore.c" 2

# 1 "./util/version/Version.h" 1
# 19 "./util/version/Version.h"
struct Linker_x0293978556697043;
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
# 24 "dht/dhtcore/NodeStore.c" 2
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
# 25 "dht/dhtcore/NodeStore.c" 2
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
# 26 "dht/dhtcore/NodeStore.c" 2

# 1 "./util/Defined.h" 1
# 28 "dht/dhtcore/NodeStore.c" 2
# 1 "./util/Endian.h" 1
# 29 "dht/dhtcore/NodeStore.c" 2
# 1 "./util/events/Time.h" 1
# 20 "./util/events/Time.h"
struct Linker_x06816289293187257;




uint64_t Time_hrtime(void);



uint64_t Time_currentTimeMilliseconds(struct EventBase* eventBase);


uint64_t Time_currentTimeSeconds(struct EventBase* eventBase);
# 30 "dht/dhtcore/NodeStore.c" 2

# 1 "./node_build/dependencies/tree.h" 1
# 32 "dht/dhtcore/NodeStore.c" 2


struct NodeStore_pvt
{
    struct NodeStore pub;


    struct Node_Link* selfLink;


    struct NodeRBTree {
        struct Node_Two* rbh_root;
    } nodeTree;

    struct Allocator* alloc;




    struct Node_Link* linksToFree;


    struct RumorMill* renumberMill;


    struct Log* logger;


    struct EventBase* eventBase;



    int disarmCheck;

    int fullVerify;

    unsigned long Identity_verifier;
};




static int comparePeers(const struct Node_Link* la, const struct Node_Link* lb)
{
    (__extension__ ({ __typeof__(lb) Identity_ptr = (lb); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x0254ff724c14aca9ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("NodeStore.c"), (76), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x0254ff724c14aca9ull))"); } } while (0); Identity_ptr; }));
    uint64_t a = la->cannonicalLabel;
    uint64_t b = lb->cannonicalLabel;

    int log2Diff = Bits_log2x64(b) - Bits_log2x64(a);
    if (log2Diff) {
        return log2Diff;
    }
    if (Bits_bitReverse64(a) < Bits_bitReverse64(b)) {
        return 1;
    } else if (a == b) {
        return 0;
    }
    return -1;
}

__attribute__((unused)) static void PeerRBTree_RB_INSERT_COLOR(struct PeerRBTree *head, struct Node_Link *elm) { struct Node_Link *parent, *gparent, *tmp; while ((parent = (elm)->peerTree.rbe_parent) != 
# 92 "dht/dhtcore/NodeStore.c" 3 4
((void *)0) 
# 92 "dht/dhtcore/NodeStore.c"
&& (parent)->peerTree.rbe_color == 1) { gparent = (parent)->peerTree.rbe_parent; if (parent == (gparent)->peerTree.rbe_left) { tmp = (gparent)->peerTree.rbe_right; if (tmp && (tmp)->peerTree.rbe_color == 1) { (tmp)->peerTree.rbe_color = 0; do { (parent)->peerTree.rbe_color = 0; (gparent)->peerTree.rbe_color = 1; } while ( 0); elm = gparent; continue; } if ((parent)->peerTree.rbe_right == elm) { do { (tmp) = (parent)->peerTree.rbe_right; if (((parent)->peerTree.rbe_right = (tmp)->peerTree.rbe_left) != 
# 92 "dht/dhtcore/NodeStore.c" 3 4
((void *)0)
# 92 "dht/dhtcore/NodeStore.c"
) { ((tmp)->peerTree.rbe_left)->peerTree.rbe_parent = (parent); } do {} while (0); if (((tmp)->peerTree.rbe_parent = (parent)->peerTree.rbe_parent) != 
# 92 "dht/dhtcore/NodeStore.c" 3 4
((void *)0)
# 92 "dht/dhtcore/NodeStore.c"
) { if ((parent) == ((parent)->peerTree.rbe_parent)->peerTree.rbe_left) ((parent)->peerTree.rbe_parent)->peerTree.rbe_left = (tmp); else ((parent)->peerTree.rbe_parent)->peerTree.rbe_right = (tmp); } else (head)->rbh_root = (tmp); (tmp)->peerTree.rbe_left = (parent); (parent)->peerTree.rbe_parent = (tmp); do {} while (0); if (((tmp)->peerTree.rbe_parent)) do {} while (0); } while ( 0); tmp = parent; parent = elm; elm = tmp; } do { (parent)->peerTree.rbe_color = 0; (gparent)->peerTree.rbe_color = 1; } while ( 0); do { (tmp) = (gparent)->peerTree.rbe_left; if (((gparent)->peerTree.rbe_left = (tmp)->peerTree.rbe_right) != 
# 92 "dht/dhtcore/NodeStore.c" 3 4
((void *)0)
# 92 "dht/dhtcore/NodeStore.c"
) { ((tmp)->peerTree.rbe_right)->peerTree.rbe_parent = (gparent); } do {} while (0); if (((tmp)->peerTree.rbe_parent = (gparent)->peerTree.rbe_parent) != 
# 92 "dht/dhtcore/NodeStore.c" 3 4
((void *)0)
# 92 "dht/dhtcore/NodeStore.c"
) { if ((gparent) == ((gparent)->peerTree.rbe_parent)->peerTree.rbe_left) ((gparent)->peerTree.rbe_parent)->peerTree.rbe_left = (tmp); else ((gparent)->peerTree.rbe_parent)->peerTree.rbe_right = (tmp); } else (head)->rbh_root = (tmp); (tmp)->peerTree.rbe_right = (gparent); (gparent)->peerTree.rbe_parent = (tmp); do {} while (0); if (((tmp)->peerTree.rbe_parent)) do {} while (0); } while ( 0); } else { tmp = (gparent)->peerTree.rbe_left; if (tmp && (tmp)->peerTree.rbe_color == 1) { (tmp)->peerTree.rbe_color = 0; do { (parent)->peerTree.rbe_color = 0; (gparent)->peerTree.rbe_color = 1; } while ( 0); elm = gparent; continue; } if ((parent)->peerTree.rbe_left == elm) { do { (tmp) = (parent)->peerTree.rbe_left; if (((parent)->peerTree.rbe_left = (tmp)->peerTree.rbe_right) != 
# 92 "dht/dhtcore/NodeStore.c" 3 4
((void *)0)
# 92 "dht/dhtcore/NodeStore.c"
) { ((tmp)->peerTree.rbe_right)->peerTree.rbe_parent = (parent); } do {} while (0); if (((tmp)->peerTree.rbe_parent = (parent)->peerTree.rbe_parent) != 
# 92 "dht/dhtcore/NodeStore.c" 3 4
((void *)0)
# 92 "dht/dhtcore/NodeStore.c"
) { if ((parent) == ((parent)->peerTree.rbe_parent)->peerTree.rbe_left) ((parent)->peerTree.rbe_parent)->peerTree.rbe_left = (tmp); else ((parent)->peerTree.rbe_parent)->peerTree.rbe_right = (tmp); } else (head)->rbh_root = (tmp); (tmp)->peerTree.rbe_right = (parent); (parent)->peerTree.rbe_parent = (tmp); do {} while (0); if (((tmp)->peerTree.rbe_parent)) do {} while (0); } while ( 0); tmp = parent; parent = elm; elm = tmp; } do { (parent)->peerTree.rbe_color = 0; (gparent)->peerTree.rbe_color = 1; } while ( 0); do { (tmp) = (gparent)->peerTree.rbe_right; if (((gparent)->peerTree.rbe_right = (tmp)->peerTree.rbe_left) != 
# 92 "dht/dhtcore/NodeStore.c" 3 4
((void *)0)
# 92 "dht/dhtcore/NodeStore.c"
) { ((tmp)->peerTree.rbe_left)->peerTree.rbe_parent = (gparent); } do {} while (0); if (((tmp)->peerTree.rbe_parent = (gparent)->peerTree.rbe_parent) != 
# 92 "dht/dhtcore/NodeStore.c" 3 4
((void *)0)
# 92 "dht/dhtcore/NodeStore.c"
) { if ((gparent) == ((gparent)->peerTree.rbe_parent)->peerTree.rbe_left) ((gparent)->peerTree.rbe_parent)->peerTree.rbe_left = (tmp); else ((gparent)->peerTree.rbe_parent)->peerTree.rbe_right = (tmp); } else (head)->rbh_root = (tmp); (tmp)->peerTree.rbe_left = (gparent); (gparent)->peerTree.rbe_parent = (tmp); do {} while (0); if (((tmp)->peerTree.rbe_parent)) do {} while (0); } while ( 0); } } (head->rbh_root)->peerTree.rbe_color = 0; } __attribute__((unused)) static void PeerRBTree_RB_REMOVE_COLOR(struct PeerRBTree *head, struct Node_Link *parent, struct Node_Link *elm) { struct Node_Link *tmp; while ((elm == 
# 92 "dht/dhtcore/NodeStore.c" 3 4
((void *)0) 
# 92 "dht/dhtcore/NodeStore.c"
|| (elm)->peerTree.rbe_color == 0) && elm != (head)->rbh_root) { if ((parent)->peerTree.rbe_left == elm) { tmp = (parent)->peerTree.rbe_right; if ((tmp)->peerTree.rbe_color == 1) { do { (tmp)->peerTree.rbe_color = 0; (parent)->peerTree.rbe_color = 1; } while ( 0); do { (tmp) = (parent)->peerTree.rbe_right; if (((parent)->peerTree.rbe_right = (tmp)->peerTree.rbe_left) != 
# 92 "dht/dhtcore/NodeStore.c" 3 4
((void *)0)
# 92 "dht/dhtcore/NodeStore.c"
) { ((tmp)->peerTree.rbe_left)->peerTree.rbe_parent = (parent); } do {} while (0); if (((tmp)->peerTree.rbe_parent = (parent)->peerTree.rbe_parent) != 
# 92 "dht/dhtcore/NodeStore.c" 3 4
((void *)0)
# 92 "dht/dhtcore/NodeStore.c"
) { if ((parent) == ((parent)->peerTree.rbe_parent)->peerTree.rbe_left) ((parent)->peerTree.rbe_parent)->peerTree.rbe_left = (tmp); else ((parent)->peerTree.rbe_parent)->peerTree.rbe_right = (tmp); } else (head)->rbh_root = (tmp); (tmp)->peerTree.rbe_left = (parent); (parent)->peerTree.rbe_parent = (tmp); do {} while (0); if (((tmp)->peerTree.rbe_parent)) do {} while (0); } while ( 0); tmp = (parent)->peerTree.rbe_right; } if (((tmp)->peerTree.rbe_left == 
# 92 "dht/dhtcore/NodeStore.c" 3 4
((void *)0) 
# 92 "dht/dhtcore/NodeStore.c"
|| ((tmp)->peerTree.rbe_left)->peerTree.rbe_color == 0) && ((tmp)->peerTree.rbe_right == 
# 92 "dht/dhtcore/NodeStore.c" 3 4
((void *)0) 
# 92 "dht/dhtcore/NodeStore.c"
|| ((tmp)->peerTree.rbe_right)->peerTree.rbe_color == 0)) { (tmp)->peerTree.rbe_color = 1; elm = parent; parent = (elm)->peerTree.rbe_parent; } else { if ((tmp)->peerTree.rbe_right == 
# 92 "dht/dhtcore/NodeStore.c" 3 4
((void *)0) 
# 92 "dht/dhtcore/NodeStore.c"
|| ((tmp)->peerTree.rbe_right)->peerTree.rbe_color == 0) { struct Node_Link *oleft; if ((oleft = (tmp)->peerTree.rbe_left) != 
# 92 "dht/dhtcore/NodeStore.c" 3 4
((void *)0)
# 92 "dht/dhtcore/NodeStore.c"
) (oleft)->peerTree.rbe_color = 0; (tmp)->peerTree.rbe_color = 1; do { (oleft) = (tmp)->peerTree.rbe_left; if (((tmp)->peerTree.rbe_left = (oleft)->peerTree.rbe_right) != 
# 92 "dht/dhtcore/NodeStore.c" 3 4
((void *)0)
# 92 "dht/dhtcore/NodeStore.c"
) { ((oleft)->peerTree.rbe_right)->peerTree.rbe_parent = (tmp); } do {} while (0); if (((oleft)->peerTree.rbe_parent = (tmp)->peerTree.rbe_parent) != 
# 92 "dht/dhtcore/NodeStore.c" 3 4
((void *)0)
# 92 "dht/dhtcore/NodeStore.c"
) { if ((tmp) == ((tmp)->peerTree.rbe_parent)->peerTree.rbe_left) ((tmp)->peerTree.rbe_parent)->peerTree.rbe_left = (oleft); else ((tmp)->peerTree.rbe_parent)->peerTree.rbe_right = (oleft); } else (head)->rbh_root = (oleft); (oleft)->peerTree.rbe_right = (tmp); (tmp)->peerTree.rbe_parent = (oleft); do {} while (0); if (((oleft)->peerTree.rbe_parent)) do {} while (0); } while ( 0); tmp = (parent)->peerTree.rbe_right; } (tmp)->peerTree.rbe_color = (parent)->peerTree.rbe_color; (parent)->peerTree.rbe_color = 0; if ((tmp)->peerTree.rbe_right) ((tmp)->peerTree.rbe_right)->peerTree.rbe_color = 0; do { (tmp) = (parent)->peerTree.rbe_right; if (((parent)->peerTree.rbe_right = (tmp)->peerTree.rbe_left) != 
# 92 "dht/dhtcore/NodeStore.c" 3 4
((void *)0)
# 92 "dht/dhtcore/NodeStore.c"
) { ((tmp)->peerTree.rbe_left)->peerTree.rbe_parent = (parent); } do {} while (0); if (((tmp)->peerTree.rbe_parent = (parent)->peerTree.rbe_parent) != 
# 92 "dht/dhtcore/NodeStore.c" 3 4
((void *)0)
# 92 "dht/dhtcore/NodeStore.c"
) { if ((parent) == ((parent)->peerTree.rbe_parent)->peerTree.rbe_left) ((parent)->peerTree.rbe_parent)->peerTree.rbe_left = (tmp); else ((parent)->peerTree.rbe_parent)->peerTree.rbe_right = (tmp); } else (head)->rbh_root = (tmp); (tmp)->peerTree.rbe_left = (parent); (parent)->peerTree.rbe_parent = (tmp); do {} while (0); if (((tmp)->peerTree.rbe_parent)) do {} while (0); } while ( 0); elm = (head)->rbh_root; break; } } else { tmp = (parent)->peerTree.rbe_left; if ((tmp)->peerTree.rbe_color == 1) { do { (tmp)->peerTree.rbe_color = 0; (parent)->peerTree.rbe_color = 1; } while ( 0); do { (tmp) = (parent)->peerTree.rbe_left; if (((parent)->peerTree.rbe_left = (tmp)->peerTree.rbe_right) != 
# 92 "dht/dhtcore/NodeStore.c" 3 4
((void *)0)
# 92 "dht/dhtcore/NodeStore.c"
) { ((tmp)->peerTree.rbe_right)->peerTree.rbe_parent = (parent); } do {} while (0); if (((tmp)->peerTree.rbe_parent = (parent)->peerTree.rbe_parent) != 
# 92 "dht/dhtcore/NodeStore.c" 3 4
((void *)0)
# 92 "dht/dhtcore/NodeStore.c"
) { if ((parent) == ((parent)->peerTree.rbe_parent)->peerTree.rbe_left) ((parent)->peerTree.rbe_parent)->peerTree.rbe_left = (tmp); else ((parent)->peerTree.rbe_parent)->peerTree.rbe_right = (tmp); } else (head)->rbh_root = (tmp); (tmp)->peerTree.rbe_right = (parent); (parent)->peerTree.rbe_parent = (tmp); do {} while (0); if (((tmp)->peerTree.rbe_parent)) do {} while (0); } while ( 0); tmp = (parent)->peerTree.rbe_left; } if (((tmp)->peerTree.rbe_left == 
# 92 "dht/dhtcore/NodeStore.c" 3 4
((void *)0) 
# 92 "dht/dhtcore/NodeStore.c"
|| ((tmp)->peerTree.rbe_left)->peerTree.rbe_color == 0) && ((tmp)->peerTree.rbe_right == 
# 92 "dht/dhtcore/NodeStore.c" 3 4
((void *)0) 
# 92 "dht/dhtcore/NodeStore.c"
|| ((tmp)->peerTree.rbe_right)->peerTree.rbe_color == 0)) { (tmp)->peerTree.rbe_color = 1; elm = parent; parent = (elm)->peerTree.rbe_parent; } else { if ((tmp)->peerTree.rbe_left == 
# 92 "dht/dhtcore/NodeStore.c" 3 4
((void *)0) 
# 92 "dht/dhtcore/NodeStore.c"
|| ((tmp)->peerTree.rbe_left)->peerTree.rbe_color == 0) { struct Node_Link *oright; if ((oright = (tmp)->peerTree.rbe_right) != 
# 92 "dht/dhtcore/NodeStore.c" 3 4
((void *)0)
# 92 "dht/dhtcore/NodeStore.c"
) (oright)->peerTree.rbe_color = 0; (tmp)->peerTree.rbe_color = 1; do { (oright) = (tmp)->peerTree.rbe_right; if (((tmp)->peerTree.rbe_right = (oright)->peerTree.rbe_left) != 
# 92 "dht/dhtcore/NodeStore.c" 3 4
((void *)0)
# 92 "dht/dhtcore/NodeStore.c"
) { ((oright)->peerTree.rbe_left)->peerTree.rbe_parent = (tmp); } do {} while (0); if (((oright)->peerTree.rbe_parent = (tmp)->peerTree.rbe_parent) != 
# 92 "dht/dhtcore/NodeStore.c" 3 4
((void *)0)
# 92 "dht/dhtcore/NodeStore.c"
) { if ((tmp) == ((tmp)->peerTree.rbe_parent)->peerTree.rbe_left) ((tmp)->peerTree.rbe_parent)->peerTree.rbe_left = (oright); else ((tmp)->peerTree.rbe_parent)->peerTree.rbe_right = (oright); } else (head)->rbh_root = (oright); (oright)->peerTree.rbe_left = (tmp); (tmp)->peerTree.rbe_parent = (oright); do {} while (0); if (((oright)->peerTree.rbe_parent)) do {} while (0); } while ( 0); tmp = (parent)->peerTree.rbe_left; } (tmp)->peerTree.rbe_color = (parent)->peerTree.rbe_color; (parent)->peerTree.rbe_color = 0; if ((tmp)->peerTree.rbe_left) ((tmp)->peerTree.rbe_left)->peerTree.rbe_color = 0; do { (tmp) = (parent)->peerTree.rbe_left; if (((parent)->peerTree.rbe_left = (tmp)->peerTree.rbe_right) != 
# 92 "dht/dhtcore/NodeStore.c" 3 4
((void *)0)
# 92 "dht/dhtcore/NodeStore.c"
) { ((tmp)->peerTree.rbe_right)->peerTree.rbe_parent = (parent); } do {} while (0); if (((tmp)->peerTree.rbe_parent = (parent)->peerTree.rbe_parent) != 
# 92 "dht/dhtcore/NodeStore.c" 3 4
((void *)0)
# 92 "dht/dhtcore/NodeStore.c"
) { if ((parent) == ((parent)->peerTree.rbe_parent)->peerTree.rbe_left) ((parent)->peerTree.rbe_parent)->peerTree.rbe_left = (tmp); else ((parent)->peerTree.rbe_parent)->peerTree.rbe_right = (tmp); } else (head)->rbh_root = (tmp); (tmp)->peerTree.rbe_right = (parent); (parent)->peerTree.rbe_parent = (tmp); do {} while (0); if (((tmp)->peerTree.rbe_parent)) do {} while (0); } while ( 0); elm = (head)->rbh_root; break; } } } if (elm) (elm)->peerTree.rbe_color = 0; } __attribute__((unused)) static struct Node_Link * PeerRBTree_RB_REMOVE(struct PeerRBTree *head, struct Node_Link *elm) { struct Node_Link *child, *parent, *old = elm; int color; if ((elm)->peerTree.rbe_left == 
# 92 "dht/dhtcore/NodeStore.c" 3 4
((void *)0)
# 92 "dht/dhtcore/NodeStore.c"
) child = (elm)->peerTree.rbe_right; else if ((elm)->peerTree.rbe_right == 
# 92 "dht/dhtcore/NodeStore.c" 3 4
((void *)0)
# 92 "dht/dhtcore/NodeStore.c"
) child = (elm)->peerTree.rbe_left; else { struct Node_Link *left; elm = (elm)->peerTree.rbe_right; while ((left = (elm)->peerTree.rbe_left) != 
# 92 "dht/dhtcore/NodeStore.c" 3 4
((void *)0)
# 92 "dht/dhtcore/NodeStore.c"
) elm = left; child = (elm)->peerTree.rbe_right; parent = (elm)->peerTree.rbe_parent; color = (elm)->peerTree.rbe_color; if (child) (child)->peerTree.rbe_parent = parent; if (parent) { if ((parent)->peerTree.rbe_left == elm) (parent)->peerTree.rbe_left = child; else (parent)->peerTree.rbe_right = child; do {} while (0); } else (head)->rbh_root = child; if ((elm)->peerTree.rbe_parent == old) parent = elm; (elm)->peerTree = (old)->peerTree; if ((old)->peerTree.rbe_parent) { if (((old)->peerTree.rbe_parent)->peerTree.rbe_left == old) ((old)->peerTree.rbe_parent)->peerTree.rbe_left = elm; else ((old)->peerTree.rbe_parent)->peerTree.rbe_right = elm; do {} while (0); } else (head)->rbh_root = elm; ((old)->peerTree.rbe_left)->peerTree.rbe_parent = elm; if ((old)->peerTree.rbe_right) ((old)->peerTree.rbe_right)->peerTree.rbe_parent = elm; if (parent) { left = parent; do { do {} while (0); } while ((left = (left)->peerTree.rbe_parent) != 
# 92 "dht/dhtcore/NodeStore.c" 3 4
((void *)0)
# 92 "dht/dhtcore/NodeStore.c"
); } goto color; } parent = (elm)->peerTree.rbe_parent; color = (elm)->peerTree.rbe_color; if (child) (child)->peerTree.rbe_parent = parent; if (parent) { if ((parent)->peerTree.rbe_left == elm) (parent)->peerTree.rbe_left = child; else (parent)->peerTree.rbe_right = child; do {} while (0); } else (head)->rbh_root = child; color: if (color == 0) PeerRBTree_RB_REMOVE_COLOR(head, parent, child); return (old); } __attribute__((unused)) static struct Node_Link * PeerRBTree_RB_INSERT(struct PeerRBTree *head, struct Node_Link *elm) { struct Node_Link *tmp; struct Node_Link *parent = 
# 92 "dht/dhtcore/NodeStore.c" 3 4
((void *)0)
# 92 "dht/dhtcore/NodeStore.c"
; int comp = 0; tmp = (head)->rbh_root; while (tmp) { parent = tmp; comp = (comparePeers)(elm, parent); if (comp < 0) tmp = (tmp)->peerTree.rbe_left; else if (comp > 0) tmp = (tmp)->peerTree.rbe_right; else return (tmp); } do { (elm)->peerTree.rbe_parent = parent; (elm)->peerTree.rbe_left = (elm)->peerTree.rbe_right = 
# 92 "dht/dhtcore/NodeStore.c" 3 4
((void *)0)
# 92 "dht/dhtcore/NodeStore.c"
; (elm)->peerTree.rbe_color = 1; } while ( 0); if (parent != 
# 92 "dht/dhtcore/NodeStore.c" 3 4
((void *)0)
# 92 "dht/dhtcore/NodeStore.c"
) { if (comp < 0) (parent)->peerTree.rbe_left = elm; else (parent)->peerTree.rbe_right = elm; do {} while (0); } else (head)->rbh_root = elm; PeerRBTree_RB_INSERT_COLOR(head, elm); return (
# 92 "dht/dhtcore/NodeStore.c" 3 4
((void *)0)
# 92 "dht/dhtcore/NodeStore.c"
); } __attribute__((unused)) static struct Node_Link * PeerRBTree_RB_FIND(struct PeerRBTree *head, struct Node_Link *elm) { struct Node_Link *tmp = (head)->rbh_root; int comp; while (tmp) { comp = comparePeers(elm, tmp); if (comp < 0) tmp = (tmp)->peerTree.rbe_left; else if (comp > 0) tmp = (tmp)->peerTree.rbe_right; else return (tmp); } return (
# 92 "dht/dhtcore/NodeStore.c" 3 4
((void *)0)
# 92 "dht/dhtcore/NodeStore.c"
); } __attribute__((unused)) static struct Node_Link * PeerRBTree_RB_NFIND(struct PeerRBTree *head, struct Node_Link *elm) { struct Node_Link *tmp = (head)->rbh_root; struct Node_Link *res = 
# 92 "dht/dhtcore/NodeStore.c" 3 4
((void *)0)
# 92 "dht/dhtcore/NodeStore.c"
; int comp; while (tmp) { comp = comparePeers(elm, tmp); if (comp < 0) { res = tmp; tmp = (tmp)->peerTree.rbe_left; } else if (comp > 0) tmp = (tmp)->peerTree.rbe_right; else return (tmp); } return (res); } __attribute__((unused)) static struct Node_Link * PeerRBTree_RB_NEXT(struct Node_Link *elm) { if ((elm)->peerTree.rbe_right) { elm = (elm)->peerTree.rbe_right; while ((elm)->peerTree.rbe_left) elm = (elm)->peerTree.rbe_left; } else { if ((elm)->peerTree.rbe_parent && (elm == ((elm)->peerTree.rbe_parent)->peerTree.rbe_left)) elm = (elm)->peerTree.rbe_parent; else { while ((elm)->peerTree.rbe_parent && (elm == ((elm)->peerTree.rbe_parent)->peerTree.rbe_right)) elm = (elm)->peerTree.rbe_parent; elm = (elm)->peerTree.rbe_parent; } } return (elm); } __attribute__((unused)) static struct Node_Link * PeerRBTree_RB_PREV(struct Node_Link *elm) { if ((elm)->peerTree.rbe_left) { elm = (elm)->peerTree.rbe_left; while ((elm)->peerTree.rbe_right) elm = (elm)->peerTree.rbe_right; } else { if ((elm)->peerTree.rbe_parent && (elm == ((elm)->peerTree.rbe_parent)->peerTree.rbe_right)) elm = (elm)->peerTree.rbe_parent; else { while ((elm)->peerTree.rbe_parent && (elm == ((elm)->peerTree.rbe_parent)->peerTree.rbe_left)) elm = (elm)->peerTree.rbe_parent; elm = (elm)->peerTree.rbe_parent; } } return (elm); } __attribute__((unused)) static struct Node_Link * PeerRBTree_RB_MINMAX(struct PeerRBTree *head, int val) { struct Node_Link *tmp = (head)->rbh_root; struct Node_Link *parent = 
# 92 "dht/dhtcore/NodeStore.c" 3 4
((void *)0)
# 92 "dht/dhtcore/NodeStore.c"
; while (tmp) { parent = tmp; if (val < 0) tmp = (tmp)->peerTree.rbe_left; else tmp = (tmp)->peerTree.rbe_right; } return (parent); }

static int compareNodes(const struct Node_Two* na, const struct Node_Two* nb)
{
    (__extension__ ({ __typeof__(nb) Identity_ptr = (nb); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x0254ff724c14aca9ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("NodeStore.c"), (96), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x0254ff724c14aca9ull))"); } } while (0); Identity_ptr; }));
    int ret;
    ret = Address_xorcmp(0, na->address.ip6.ints.one_be, nb->address.ip6.ints.one_be);
    if (ret) { return ret; }
    ret = Address_xorcmp(0, na->address.ip6.ints.two_be, nb->address.ip6.ints.two_be);
    if (ret) { return ret; }
    ret = Address_xorcmp(0, na->address.ip6.ints.three_be, nb->address.ip6.ints.three_be);
    if (ret) { return ret; }
    ret = Address_xorcmp(0, na->address.ip6.ints.four_be, nb->address.ip6.ints.four_be);
    return ret;
}

__attribute__((unused)) static void NodeRBTree_RB_INSERT_COLOR(struct NodeRBTree *head, struct Node_Two *elm) { struct Node_Two *parent, *gparent, *tmp; while ((parent = (elm)->nodeTree.rbe_parent) != 
# 108 "dht/dhtcore/NodeStore.c" 3 4
((void *)0) 
# 108 "dht/dhtcore/NodeStore.c"
&& (parent)->nodeTree.rbe_color == 1) { gparent = (parent)->nodeTree.rbe_parent; if (parent == (gparent)->nodeTree.rbe_left) { tmp = (gparent)->nodeTree.rbe_right; if (tmp && (tmp)->nodeTree.rbe_color == 1) { (tmp)->nodeTree.rbe_color = 0; do { (parent)->nodeTree.rbe_color = 0; (gparent)->nodeTree.rbe_color = 1; } while ( 0); elm = gparent; continue; } if ((parent)->nodeTree.rbe_right == elm) { do { (tmp) = (parent)->nodeTree.rbe_right; if (((parent)->nodeTree.rbe_right = (tmp)->nodeTree.rbe_left) != 
# 108 "dht/dhtcore/NodeStore.c" 3 4
((void *)0)
# 108 "dht/dhtcore/NodeStore.c"
) { ((tmp)->nodeTree.rbe_left)->nodeTree.rbe_parent = (parent); } do {} while (0); if (((tmp)->nodeTree.rbe_parent = (parent)->nodeTree.rbe_parent) != 
# 108 "dht/dhtcore/NodeStore.c" 3 4
((void *)0)
# 108 "dht/dhtcore/NodeStore.c"
) { if ((parent) == ((parent)->nodeTree.rbe_parent)->nodeTree.rbe_left) ((parent)->nodeTree.rbe_parent)->nodeTree.rbe_left = (tmp); else ((parent)->nodeTree.rbe_parent)->nodeTree.rbe_right = (tmp); } else (head)->rbh_root = (tmp); (tmp)->nodeTree.rbe_left = (parent); (parent)->nodeTree.rbe_parent = (tmp); do {} while (0); if (((tmp)->nodeTree.rbe_parent)) do {} while (0); } while ( 0); tmp = parent; parent = elm; elm = tmp; } do { (parent)->nodeTree.rbe_color = 0; (gparent)->nodeTree.rbe_color = 1; } while ( 0); do { (tmp) = (gparent)->nodeTree.rbe_left; if (((gparent)->nodeTree.rbe_left = (tmp)->nodeTree.rbe_right) != 
# 108 "dht/dhtcore/NodeStore.c" 3 4
((void *)0)
# 108 "dht/dhtcore/NodeStore.c"
) { ((tmp)->nodeTree.rbe_right)->nodeTree.rbe_parent = (gparent); } do {} while (0); if (((tmp)->nodeTree.rbe_parent = (gparent)->nodeTree.rbe_parent) != 
# 108 "dht/dhtcore/NodeStore.c" 3 4
((void *)0)
# 108 "dht/dhtcore/NodeStore.c"
) { if ((gparent) == ((gparent)->nodeTree.rbe_parent)->nodeTree.rbe_left) ((gparent)->nodeTree.rbe_parent)->nodeTree.rbe_left = (tmp); else ((gparent)->nodeTree.rbe_parent)->nodeTree.rbe_right = (tmp); } else (head)->rbh_root = (tmp); (tmp)->nodeTree.rbe_right = (gparent); (gparent)->nodeTree.rbe_parent = (tmp); do {} while (0); if (((tmp)->nodeTree.rbe_parent)) do {} while (0); } while ( 0); } else { tmp = (gparent)->nodeTree.rbe_left; if (tmp && (tmp)->nodeTree.rbe_color == 1) { (tmp)->nodeTree.rbe_color = 0; do { (parent)->nodeTree.rbe_color = 0; (gparent)->nodeTree.rbe_color = 1; } while ( 0); elm = gparent; continue; } if ((parent)->nodeTree.rbe_left == elm) { do { (tmp) = (parent)->nodeTree.rbe_left; if (((parent)->nodeTree.rbe_left = (tmp)->nodeTree.rbe_right) != 
# 108 "dht/dhtcore/NodeStore.c" 3 4
((void *)0)
# 108 "dht/dhtcore/NodeStore.c"
) { ((tmp)->nodeTree.rbe_right)->nodeTree.rbe_parent = (parent); } do {} while (0); if (((tmp)->nodeTree.rbe_parent = (parent)->nodeTree.rbe_parent) != 
# 108 "dht/dhtcore/NodeStore.c" 3 4
((void *)0)
# 108 "dht/dhtcore/NodeStore.c"
) { if ((parent) == ((parent)->nodeTree.rbe_parent)->nodeTree.rbe_left) ((parent)->nodeTree.rbe_parent)->nodeTree.rbe_left = (tmp); else ((parent)->nodeTree.rbe_parent)->nodeTree.rbe_right = (tmp); } else (head)->rbh_root = (tmp); (tmp)->nodeTree.rbe_right = (parent); (parent)->nodeTree.rbe_parent = (tmp); do {} while (0); if (((tmp)->nodeTree.rbe_parent)) do {} while (0); } while ( 0); tmp = parent; parent = elm; elm = tmp; } do { (parent)->nodeTree.rbe_color = 0; (gparent)->nodeTree.rbe_color = 1; } while ( 0); do { (tmp) = (gparent)->nodeTree.rbe_right; if (((gparent)->nodeTree.rbe_right = (tmp)->nodeTree.rbe_left) != 
# 108 "dht/dhtcore/NodeStore.c" 3 4
((void *)0)
# 108 "dht/dhtcore/NodeStore.c"
) { ((tmp)->nodeTree.rbe_left)->nodeTree.rbe_parent = (gparent); } do {} while (0); if (((tmp)->nodeTree.rbe_parent = (gparent)->nodeTree.rbe_parent) != 
# 108 "dht/dhtcore/NodeStore.c" 3 4
((void *)0)
# 108 "dht/dhtcore/NodeStore.c"
) { if ((gparent) == ((gparent)->nodeTree.rbe_parent)->nodeTree.rbe_left) ((gparent)->nodeTree.rbe_parent)->nodeTree.rbe_left = (tmp); else ((gparent)->nodeTree.rbe_parent)->nodeTree.rbe_right = (tmp); } else (head)->rbh_root = (tmp); (tmp)->nodeTree.rbe_left = (gparent); (gparent)->nodeTree.rbe_parent = (tmp); do {} while (0); if (((tmp)->nodeTree.rbe_parent)) do {} while (0); } while ( 0); } } (head->rbh_root)->nodeTree.rbe_color = 0; } __attribute__((unused)) static void NodeRBTree_RB_REMOVE_COLOR(struct NodeRBTree *head, struct Node_Two *parent, struct Node_Two *elm) { struct Node_Two *tmp; while ((elm == 
# 108 "dht/dhtcore/NodeStore.c" 3 4
((void *)0) 
# 108 "dht/dhtcore/NodeStore.c"
|| (elm)->nodeTree.rbe_color == 0) && elm != (head)->rbh_root) { if ((parent)->nodeTree.rbe_left == elm) { tmp = (parent)->nodeTree.rbe_right; if ((tmp)->nodeTree.rbe_color == 1) { do { (tmp)->nodeTree.rbe_color = 0; (parent)->nodeTree.rbe_color = 1; } while ( 0); do { (tmp) = (parent)->nodeTree.rbe_right; if (((parent)->nodeTree.rbe_right = (tmp)->nodeTree.rbe_left) != 
# 108 "dht/dhtcore/NodeStore.c" 3 4
((void *)0)
# 108 "dht/dhtcore/NodeStore.c"
) { ((tmp)->nodeTree.rbe_left)->nodeTree.rbe_parent = (parent); } do {} while (0); if (((tmp)->nodeTree.rbe_parent = (parent)->nodeTree.rbe_parent) != 
# 108 "dht/dhtcore/NodeStore.c" 3 4
((void *)0)
# 108 "dht/dhtcore/NodeStore.c"
) { if ((parent) == ((parent)->nodeTree.rbe_parent)->nodeTree.rbe_left) ((parent)->nodeTree.rbe_parent)->nodeTree.rbe_left = (tmp); else ((parent)->nodeTree.rbe_parent)->nodeTree.rbe_right = (tmp); } else (head)->rbh_root = (tmp); (tmp)->nodeTree.rbe_left = (parent); (parent)->nodeTree.rbe_parent = (tmp); do {} while (0); if (((tmp)->nodeTree.rbe_parent)) do {} while (0); } while ( 0); tmp = (parent)->nodeTree.rbe_right; } if (((tmp)->nodeTree.rbe_left == 
# 108 "dht/dhtcore/NodeStore.c" 3 4
((void *)0) 
# 108 "dht/dhtcore/NodeStore.c"
|| ((tmp)->nodeTree.rbe_left)->nodeTree.rbe_color == 0) && ((tmp)->nodeTree.rbe_right == 
# 108 "dht/dhtcore/NodeStore.c" 3 4
((void *)0) 
# 108 "dht/dhtcore/NodeStore.c"
|| ((tmp)->nodeTree.rbe_right)->nodeTree.rbe_color == 0)) { (tmp)->nodeTree.rbe_color = 1; elm = parent; parent = (elm)->nodeTree.rbe_parent; } else { if ((tmp)->nodeTree.rbe_right == 
# 108 "dht/dhtcore/NodeStore.c" 3 4
((void *)0) 
# 108 "dht/dhtcore/NodeStore.c"
|| ((tmp)->nodeTree.rbe_right)->nodeTree.rbe_color == 0) { struct Node_Two *oleft; if ((oleft = (tmp)->nodeTree.rbe_left) != 
# 108 "dht/dhtcore/NodeStore.c" 3 4
((void *)0)
# 108 "dht/dhtcore/NodeStore.c"
) (oleft)->nodeTree.rbe_color = 0; (tmp)->nodeTree.rbe_color = 1; do { (oleft) = (tmp)->nodeTree.rbe_left; if (((tmp)->nodeTree.rbe_left = (oleft)->nodeTree.rbe_right) != 
# 108 "dht/dhtcore/NodeStore.c" 3 4
((void *)0)
# 108 "dht/dhtcore/NodeStore.c"
) { ((oleft)->nodeTree.rbe_right)->nodeTree.rbe_parent = (tmp); } do {} while (0); if (((oleft)->nodeTree.rbe_parent = (tmp)->nodeTree.rbe_parent) != 
# 108 "dht/dhtcore/NodeStore.c" 3 4
((void *)0)
# 108 "dht/dhtcore/NodeStore.c"
) { if ((tmp) == ((tmp)->nodeTree.rbe_parent)->nodeTree.rbe_left) ((tmp)->nodeTree.rbe_parent)->nodeTree.rbe_left = (oleft); else ((tmp)->nodeTree.rbe_parent)->nodeTree.rbe_right = (oleft); } else (head)->rbh_root = (oleft); (oleft)->nodeTree.rbe_right = (tmp); (tmp)->nodeTree.rbe_parent = (oleft); do {} while (0); if (((oleft)->nodeTree.rbe_parent)) do {} while (0); } while ( 0); tmp = (parent)->nodeTree.rbe_right; } (tmp)->nodeTree.rbe_color = (parent)->nodeTree.rbe_color; (parent)->nodeTree.rbe_color = 0; if ((tmp)->nodeTree.rbe_right) ((tmp)->nodeTree.rbe_right)->nodeTree.rbe_color = 0; do { (tmp) = (parent)->nodeTree.rbe_right; if (((parent)->nodeTree.rbe_right = (tmp)->nodeTree.rbe_left) != 
# 108 "dht/dhtcore/NodeStore.c" 3 4
((void *)0)
# 108 "dht/dhtcore/NodeStore.c"
) { ((tmp)->nodeTree.rbe_left)->nodeTree.rbe_parent = (parent); } do {} while (0); if (((tmp)->nodeTree.rbe_parent = (parent)->nodeTree.rbe_parent) != 
# 108 "dht/dhtcore/NodeStore.c" 3 4
((void *)0)
# 108 "dht/dhtcore/NodeStore.c"
) { if ((parent) == ((parent)->nodeTree.rbe_parent)->nodeTree.rbe_left) ((parent)->nodeTree.rbe_parent)->nodeTree.rbe_left = (tmp); else ((parent)->nodeTree.rbe_parent)->nodeTree.rbe_right = (tmp); } else (head)->rbh_root = (tmp); (tmp)->nodeTree.rbe_left = (parent); (parent)->nodeTree.rbe_parent = (tmp); do {} while (0); if (((tmp)->nodeTree.rbe_parent)) do {} while (0); } while ( 0); elm = (head)->rbh_root; break; } } else { tmp = (parent)->nodeTree.rbe_left; if ((tmp)->nodeTree.rbe_color == 1) { do { (tmp)->nodeTree.rbe_color = 0; (parent)->nodeTree.rbe_color = 1; } while ( 0); do { (tmp) = (parent)->nodeTree.rbe_left; if (((parent)->nodeTree.rbe_left = (tmp)->nodeTree.rbe_right) != 
# 108 "dht/dhtcore/NodeStore.c" 3 4
((void *)0)
# 108 "dht/dhtcore/NodeStore.c"
) { ((tmp)->nodeTree.rbe_right)->nodeTree.rbe_parent = (parent); } do {} while (0); if (((tmp)->nodeTree.rbe_parent = (parent)->nodeTree.rbe_parent) != 
# 108 "dht/dhtcore/NodeStore.c" 3 4
((void *)0)
# 108 "dht/dhtcore/NodeStore.c"
) { if ((parent) == ((parent)->nodeTree.rbe_parent)->nodeTree.rbe_left) ((parent)->nodeTree.rbe_parent)->nodeTree.rbe_left = (tmp); else ((parent)->nodeTree.rbe_parent)->nodeTree.rbe_right = (tmp); } else (head)->rbh_root = (tmp); (tmp)->nodeTree.rbe_right = (parent); (parent)->nodeTree.rbe_parent = (tmp); do {} while (0); if (((tmp)->nodeTree.rbe_parent)) do {} while (0); } while ( 0); tmp = (parent)->nodeTree.rbe_left; } if (((tmp)->nodeTree.rbe_left == 
# 108 "dht/dhtcore/NodeStore.c" 3 4
((void *)0) 
# 108 "dht/dhtcore/NodeStore.c"
|| ((tmp)->nodeTree.rbe_left)->nodeTree.rbe_color == 0) && ((tmp)->nodeTree.rbe_right == 
# 108 "dht/dhtcore/NodeStore.c" 3 4
((void *)0) 
# 108 "dht/dhtcore/NodeStore.c"
|| ((tmp)->nodeTree.rbe_right)->nodeTree.rbe_color == 0)) { (tmp)->nodeTree.rbe_color = 1; elm = parent; parent = (elm)->nodeTree.rbe_parent; } else { if ((tmp)->nodeTree.rbe_left == 
# 108 "dht/dhtcore/NodeStore.c" 3 4
((void *)0) 
# 108 "dht/dhtcore/NodeStore.c"
|| ((tmp)->nodeTree.rbe_left)->nodeTree.rbe_color == 0) { struct Node_Two *oright; if ((oright = (tmp)->nodeTree.rbe_right) != 
# 108 "dht/dhtcore/NodeStore.c" 3 4
((void *)0)
# 108 "dht/dhtcore/NodeStore.c"
) (oright)->nodeTree.rbe_color = 0; (tmp)->nodeTree.rbe_color = 1; do { (oright) = (tmp)->nodeTree.rbe_right; if (((tmp)->nodeTree.rbe_right = (oright)->nodeTree.rbe_left) != 
# 108 "dht/dhtcore/NodeStore.c" 3 4
((void *)0)
# 108 "dht/dhtcore/NodeStore.c"
) { ((oright)->nodeTree.rbe_left)->nodeTree.rbe_parent = (tmp); } do {} while (0); if (((oright)->nodeTree.rbe_parent = (tmp)->nodeTree.rbe_parent) != 
# 108 "dht/dhtcore/NodeStore.c" 3 4
((void *)0)
# 108 "dht/dhtcore/NodeStore.c"
) { if ((tmp) == ((tmp)->nodeTree.rbe_parent)->nodeTree.rbe_left) ((tmp)->nodeTree.rbe_parent)->nodeTree.rbe_left = (oright); else ((tmp)->nodeTree.rbe_parent)->nodeTree.rbe_right = (oright); } else (head)->rbh_root = (oright); (oright)->nodeTree.rbe_left = (tmp); (tmp)->nodeTree.rbe_parent = (oright); do {} while (0); if (((oright)->nodeTree.rbe_parent)) do {} while (0); } while ( 0); tmp = (parent)->nodeTree.rbe_left; } (tmp)->nodeTree.rbe_color = (parent)->nodeTree.rbe_color; (parent)->nodeTree.rbe_color = 0; if ((tmp)->nodeTree.rbe_left) ((tmp)->nodeTree.rbe_left)->nodeTree.rbe_color = 0; do { (tmp) = (parent)->nodeTree.rbe_left; if (((parent)->nodeTree.rbe_left = (tmp)->nodeTree.rbe_right) != 
# 108 "dht/dhtcore/NodeStore.c" 3 4
((void *)0)
# 108 "dht/dhtcore/NodeStore.c"
) { ((tmp)->nodeTree.rbe_right)->nodeTree.rbe_parent = (parent); } do {} while (0); if (((tmp)->nodeTree.rbe_parent = (parent)->nodeTree.rbe_parent) != 
# 108 "dht/dhtcore/NodeStore.c" 3 4
((void *)0)
# 108 "dht/dhtcore/NodeStore.c"
) { if ((parent) == ((parent)->nodeTree.rbe_parent)->nodeTree.rbe_left) ((parent)->nodeTree.rbe_parent)->nodeTree.rbe_left = (tmp); else ((parent)->nodeTree.rbe_parent)->nodeTree.rbe_right = (tmp); } else (head)->rbh_root = (tmp); (tmp)->nodeTree.rbe_right = (parent); (parent)->nodeTree.rbe_parent = (tmp); do {} while (0); if (((tmp)->nodeTree.rbe_parent)) do {} while (0); } while ( 0); elm = (head)->rbh_root; break; } } } if (elm) (elm)->nodeTree.rbe_color = 0; } __attribute__((unused)) static struct Node_Two * NodeRBTree_RB_REMOVE(struct NodeRBTree *head, struct Node_Two *elm) { struct Node_Two *child, *parent, *old = elm; int color; if ((elm)->nodeTree.rbe_left == 
# 108 "dht/dhtcore/NodeStore.c" 3 4
((void *)0)
# 108 "dht/dhtcore/NodeStore.c"
) child = (elm)->nodeTree.rbe_right; else if ((elm)->nodeTree.rbe_right == 
# 108 "dht/dhtcore/NodeStore.c" 3 4
((void *)0)
# 108 "dht/dhtcore/NodeStore.c"
) child = (elm)->nodeTree.rbe_left; else { struct Node_Two *left; elm = (elm)->nodeTree.rbe_right; while ((left = (elm)->nodeTree.rbe_left) != 
# 108 "dht/dhtcore/NodeStore.c" 3 4
((void *)0)
# 108 "dht/dhtcore/NodeStore.c"
) elm = left; child = (elm)->nodeTree.rbe_right; parent = (elm)->nodeTree.rbe_parent; color = (elm)->nodeTree.rbe_color; if (child) (child)->nodeTree.rbe_parent = parent; if (parent) { if ((parent)->nodeTree.rbe_left == elm) (parent)->nodeTree.rbe_left = child; else (parent)->nodeTree.rbe_right = child; do {} while (0); } else (head)->rbh_root = child; if ((elm)->nodeTree.rbe_parent == old) parent = elm; (elm)->nodeTree = (old)->nodeTree; if ((old)->nodeTree.rbe_parent) { if (((old)->nodeTree.rbe_parent)->nodeTree.rbe_left == old) ((old)->nodeTree.rbe_parent)->nodeTree.rbe_left = elm; else ((old)->nodeTree.rbe_parent)->nodeTree.rbe_right = elm; do {} while (0); } else (head)->rbh_root = elm; ((old)->nodeTree.rbe_left)->nodeTree.rbe_parent = elm; if ((old)->nodeTree.rbe_right) ((old)->nodeTree.rbe_right)->nodeTree.rbe_parent = elm; if (parent) { left = parent; do { do {} while (0); } while ((left = (left)->nodeTree.rbe_parent) != 
# 108 "dht/dhtcore/NodeStore.c" 3 4
((void *)0)
# 108 "dht/dhtcore/NodeStore.c"
); } goto color; } parent = (elm)->nodeTree.rbe_parent; color = (elm)->nodeTree.rbe_color; if (child) (child)->nodeTree.rbe_parent = parent; if (parent) { if ((parent)->nodeTree.rbe_left == elm) (parent)->nodeTree.rbe_left = child; else (parent)->nodeTree.rbe_right = child; do {} while (0); } else (head)->rbh_root = child; color: if (color == 0) NodeRBTree_RB_REMOVE_COLOR(head, parent, child); return (old); } __attribute__((unused)) static struct Node_Two * NodeRBTree_RB_INSERT(struct NodeRBTree *head, struct Node_Two *elm) { struct Node_Two *tmp; struct Node_Two *parent = 
# 108 "dht/dhtcore/NodeStore.c" 3 4
((void *)0)
# 108 "dht/dhtcore/NodeStore.c"
; int comp = 0; tmp = (head)->rbh_root; while (tmp) { parent = tmp; comp = (compareNodes)(elm, parent); if (comp < 0) tmp = (tmp)->nodeTree.rbe_left; else if (comp > 0) tmp = (tmp)->nodeTree.rbe_right; else return (tmp); } do { (elm)->nodeTree.rbe_parent = parent; (elm)->nodeTree.rbe_left = (elm)->nodeTree.rbe_right = 
# 108 "dht/dhtcore/NodeStore.c" 3 4
((void *)0)
# 108 "dht/dhtcore/NodeStore.c"
; (elm)->nodeTree.rbe_color = 1; } while ( 0); if (parent != 
# 108 "dht/dhtcore/NodeStore.c" 3 4
((void *)0)
# 108 "dht/dhtcore/NodeStore.c"
) { if (comp < 0) (parent)->nodeTree.rbe_left = elm; else (parent)->nodeTree.rbe_right = elm; do {} while (0); } else (head)->rbh_root = elm; NodeRBTree_RB_INSERT_COLOR(head, elm); return (
# 108 "dht/dhtcore/NodeStore.c" 3 4
((void *)0)
# 108 "dht/dhtcore/NodeStore.c"
); } __attribute__((unused)) static struct Node_Two * NodeRBTree_RB_FIND(struct NodeRBTree *head, struct Node_Two *elm) { struct Node_Two *tmp = (head)->rbh_root; int comp; while (tmp) { comp = compareNodes(elm, tmp); if (comp < 0) tmp = (tmp)->nodeTree.rbe_left; else if (comp > 0) tmp = (tmp)->nodeTree.rbe_right; else return (tmp); } return (
# 108 "dht/dhtcore/NodeStore.c" 3 4
((void *)0)
# 108 "dht/dhtcore/NodeStore.c"
); } __attribute__((unused)) static struct Node_Two * NodeRBTree_RB_NFIND(struct NodeRBTree *head, struct Node_Two *elm) { struct Node_Two *tmp = (head)->rbh_root; struct Node_Two *res = 
# 108 "dht/dhtcore/NodeStore.c" 3 4
((void *)0)
# 108 "dht/dhtcore/NodeStore.c"
; int comp; while (tmp) { comp = compareNodes(elm, tmp); if (comp < 0) { res = tmp; tmp = (tmp)->nodeTree.rbe_left; } else if (comp > 0) tmp = (tmp)->nodeTree.rbe_right; else return (tmp); } return (res); } __attribute__((unused)) static struct Node_Two * NodeRBTree_RB_NEXT(struct Node_Two *elm) { if ((elm)->nodeTree.rbe_right) { elm = (elm)->nodeTree.rbe_right; while ((elm)->nodeTree.rbe_left) elm = (elm)->nodeTree.rbe_left; } else { if ((elm)->nodeTree.rbe_parent && (elm == ((elm)->nodeTree.rbe_parent)->nodeTree.rbe_left)) elm = (elm)->nodeTree.rbe_parent; else { while ((elm)->nodeTree.rbe_parent && (elm == ((elm)->nodeTree.rbe_parent)->nodeTree.rbe_right)) elm = (elm)->nodeTree.rbe_parent; elm = (elm)->nodeTree.rbe_parent; } } return (elm); } __attribute__((unused)) static struct Node_Two * NodeRBTree_RB_PREV(struct Node_Two *elm) { if ((elm)->nodeTree.rbe_left) { elm = (elm)->nodeTree.rbe_left; while ((elm)->nodeTree.rbe_right) elm = (elm)->nodeTree.rbe_right; } else { if ((elm)->nodeTree.rbe_parent && (elm == ((elm)->nodeTree.rbe_parent)->nodeTree.rbe_right)) elm = (elm)->nodeTree.rbe_parent; else { while ((elm)->nodeTree.rbe_parent && (elm == ((elm)->nodeTree.rbe_parent)->nodeTree.rbe_left)) elm = (elm)->nodeTree.rbe_parent; elm = (elm)->nodeTree.rbe_parent; } } return (elm); } __attribute__((unused)) static struct Node_Two * NodeRBTree_RB_MINMAX(struct NodeRBTree *head, int val) { struct Node_Two *tmp = (head)->rbh_root; struct Node_Two *parent = 
# 108 "dht/dhtcore/NodeStore.c" 3 4
((void *)0)
# 108 "dht/dhtcore/NodeStore.c"
; while (tmp) { parent = tmp; if (val < 0) tmp = (tmp)->nodeTree.rbe_left; else tmp = (tmp)->nodeTree.rbe_right; } return (parent); }

static void freeLink(struct Node_Link* link, struct NodeStore_pvt* store)
{
    Allocator__realloc((store->alloc),(link),(0),"NodeStore.c",112);
    store->pub.linkCount--;
}

static struct Node_Link* getLink(struct NodeStore_pvt* store)
{
    store->pub.linkCount++;
    return Allocator__calloc((store->alloc),(sizeof(struct Node_Link)),(1),"NodeStore.c",119);
}

static void logLink(struct NodeStore_pvt* store,
                    struct Node_Link* link,
                    char* message)
{
    if (!1) {
        return;
    }
    uint8_t parent[40];
    uint8_t child[40];
    AddrTools_printIp(parent, link->parent->address.ip6.bytes);
    AddrTools_printIp(child, link->child->address.ip6.bytes);
    uint8_t path[20];
    AddrTools_printPath(path, link->cannonicalLabel);
    do { if (store->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(store->logger, Log_Level_DEBUG, "NodeStore.c", 135, "link[%s]->[%s] [%s] %s", parent, child, path, message); } } while (0);
}

static void _checkNode(struct Node_Two* node, struct NodeStore_pvt* store, char* file, int line)
{
    if (!1 || (store->disarmCheck && !store->fullVerify)) { return; }

    do { if (!((node->address.path == EncodingScheme_convertLabel(store->pub.selfNode->encodingScheme, node->address.path, (-5000))))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("NodeStore.c"), (142), "(node->address.path == EncodingScheme_convertLabel(store->pub.selfNode->encodingScheme, node->address.path, (-5000)))"); } } while (0)


                                                                                      ;

    struct Node_Link* link;
    for (link = node->reversePeers; link; link = link->nextPeer) {
        do { if (!(link->child == node)) { Assert_failure("Assertion failure [%s:%d] [%s]\n", (file), (line), "link->child == node"); } } while (0);
        do { if (!(PeerRBTree_RB_FIND(&link->parent->peerTree, link) == link)) { Assert_failure("Assertion failure [%s:%d] [%s]\n", (file), (line), "RB_FIND(PeerRBTree, &link->parent->peerTree, link) == link"); } } while (0);

        int ok = 0;
        struct Node_Link* nl = 
# 153 "dht/dhtcore/NodeStore.c" 3 4
                              ((void *)0)
# 153 "dht/dhtcore/NodeStore.c"
                                  ;
        while ((nl = NodeStore_nextLink(link->parent, nl))) {
            if (nl == link) { ok = 1; break; }
        }
        do { if (!(ok)) { Assert_failure("Assertion failure [%s:%d] [%s]\n", (file), (line), "ok"); } } while (0);

    }

    struct Node_Link* lastLink = 
# 161 "dht/dhtcore/NodeStore.c" 3 4
                                ((void *)0)
# 161 "dht/dhtcore/NodeStore.c"
                                    ;
    for ((link) = PeerRBTree_RB_MINMAX(&node->peerTree, 1); (link) != 
# 162 "dht/dhtcore/NodeStore.c" 3 4
   ((void *)0)
# 162 "dht/dhtcore/NodeStore.c"
   ; (link) = PeerRBTree_RB_PREV(link)) {
        do { if (!(!EncodingScheme_isSelfRoute(link->parent->encodingScheme, link->cannonicalLabel) || link == store->selfLink)) { Assert_failure("Assertion failure [%s:%d] [%s]\n", (file), (line), "!EncodingScheme_isSelfRoute(link->parent->encodingScheme, link->cannonicalLabel) || link == store->selfLink"); } } while (0)


                                   ;
        do { if (!(Node_getBestParent(node) || Node_getBestParent(link->child) != link)) { Assert_failure("Assertion failure [%s:%d] [%s]\n", (file), (line), "Node_getBestParent(node) || Node_getBestParent(link->child) != link"); } } while (0)
                                   ;
        do { if (!(link->parent == node)) { Assert_failure("Assertion failure [%s:%d] [%s]\n", (file), (line), "link->parent == node"); } } while (0);




        do { if (!(!lastLink || link->cannonicalLabel != lastLink->cannonicalLabel)) { Assert_failure("Assertion failure [%s:%d] [%s]\n", (file), (line), "!lastLink || link->cannonicalLabel != lastLink->cannonicalLabel"); } } while (0)
                                   ;
        do { if (!(link->cannonicalLabel < 
# 176 "dht/dhtcore/NodeStore.c" 3 4
       0xffffffffffffffffULL 
# 176 "dht/dhtcore/NodeStore.c"
       && link->cannonicalLabel > 0)) { Assert_failure("Assertion failure [%s:%d] [%s]\n", (file), (line), "link->cannonicalLabel < UINT64_MAX && link->cannonicalLabel > 0"); } } while (0)
                                   ;




        do { if (!(link->inverseLinkEncodingFormNumber < link->child->encodingScheme->count)) { Assert_failure("Assertion failure [%s:%d] [%s]\n", (file), (line), "link->inverseLinkEncodingFormNumber < link->child->encodingScheme->count"); } } while (0)
                                   ;

        struct Node_Link* rlink = 
# 185 "dht/dhtcore/NodeStore.c" 3 4
                                 ((void *)0)
# 185 "dht/dhtcore/NodeStore.c"
                                     ;
        for (rlink = link->child->reversePeers; rlink; rlink = rlink->nextPeer) {
            if (rlink == link) {
                break;
            }
        }
        do { if (!(rlink && "child contains reverse link")) { Assert_failure("Assertion failure [%s:%d] [%s]\n", (file), (line), "rlink && \"child contains reverse link\""); } } while (0);
        lastLink = link;
    }

    if (Node_getBestParent(node)) {
        do { if (!(node->address.path != 
# 196 "dht/dhtcore/NodeStore.c" 3 4
       0xffffffffffffffffULL
# 196 "dht/dhtcore/NodeStore.c"
       )) { Assert_failure("Assertion failure [%s:%d] [%s]\n", (file), (line), "node->address.path != UINT64_MAX"); } } while (0);
        do { if (!(Node_getCost(node) != 
# 197 "dht/dhtcore/NodeStore.c" 3 4
       0xffffffffffffffffULL
# 197 "dht/dhtcore/NodeStore.c"
       )) { Assert_failure("Assertion failure [%s:%d] [%s]\n", (file), (line), "Node_getCost(node) != UINT64_MAX"); } } while (0);

        struct Node_Two* nn = node;
        do {
            do { if (!(LabelSplicer_routesThrough(nn->address.path, Node_getBestParent(nn)->parent->address.path))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", (file), (line), "LabelSplicer_routesThrough(nn->address.path, Node_getBestParent(nn)->parent->address.path)"); } } while (0)




             ;
            nn = Node_getBestParent(nn)->parent;
        } while (nn != store->pub.selfNode);

    } else {
        do { if (!(node->address.path == 
# 211 "dht/dhtcore/NodeStore.c" 3 4
       0xffffffffffffffffULL
# 211 "dht/dhtcore/NodeStore.c"
       )) { Assert_failure("Assertion failure [%s:%d] [%s]\n", (file), (line), "node->address.path == UINT64_MAX"); } } while (0);
        do { if (!(Node_getCost(node) == 
# 212 "dht/dhtcore/NodeStore.c" 3 4
       0xffffffffffffffffULL
# 212 "dht/dhtcore/NodeStore.c"
       )) { Assert_failure("Assertion failure [%s:%d] [%s]\n", (file), (line), "Node_getCost(node) == UINT64_MAX"); } } while (0);
    }
}


static void _verifyNode(struct Node_Two* node,
                        struct NodeStore_pvt* store,
                        
# 219 "dht/dhtcore/NodeStore.c" 3 4
                       _Bool 
# 219 "dht/dhtcore/NodeStore.c"
                            full,
                        char* file,
                        int line)
{
    if (!1 || (store->disarmCheck && !store->fullVerify)) { return; }


    _checkNode(node, store, file, line);

    if (!full || !store->fullVerify) { return; }


    struct Node_Link* link = 
# 231 "dht/dhtcore/NodeStore.c" 3 4
                            ((void *)0)
# 231 "dht/dhtcore/NodeStore.c"
                                ;
    for ((link) = PeerRBTree_RB_MINMAX(&node->peerTree, 1); (link) != 
# 232 "dht/dhtcore/NodeStore.c" 3 4
   ((void *)0)
# 232 "dht/dhtcore/NodeStore.c"
   ; (link) = PeerRBTree_RB_PREV(link)) {

        struct Node_Link* linkB = link;
        struct Node_Link* linkC = link;
        for ((linkB) = (linkC); ((linkB) != 
# 236 "dht/dhtcore/NodeStore.c" 3 4
       ((void *)0)
# 236 "dht/dhtcore/NodeStore.c"
       ) && ((linkC) = PeerRBTree_RB_PREV(linkB), (linkB) != 
# 236 "dht/dhtcore/NodeStore.c" 3 4
       ((void *)0)
# 236 "dht/dhtcore/NodeStore.c"
       ); (linkB) = (linkC)) {
            if (linkB == link || link == store->selfLink) { continue; }
            do { if (!(!LabelSplicer_routesThrough(linkB->cannonicalLabel, link->cannonicalLabel))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", (file), (line), "!LabelSplicer_routesThrough(linkB->cannonicalLabel, link->cannonicalLabel)"); } } while (0)


             ;
        }

        do { if (!((!link->nextInSplitList))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("NodeStore.c"), (244), "(!link->nextInSplitList)"); } } while (0);
    }


    if (Node_getBestParent(node)) {
        do { if (!(node == NodeStore_closestNode(&store->pub, node->address.path))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", (file), (line), "node == NodeStore_closestNode(&store->pub, node->address.path)"); } } while (0);
    }


    do { if (!((!node->marked))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("NodeStore.c"), (253), "(!node->marked)"); } } while (0);


    struct Node_Link* bp = Node_getBestParent(node);
    if (!bp) {
        do { if (!((Node_getCost(node) == 
# 258 "dht/dhtcore/NodeStore.c" 3 4
       0xffffffffffffffffULL
# 258 "dht/dhtcore/NodeStore.c"
       ))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("NodeStore.c"), (258), "(Node_getCost(node) == 0xffffffffffffffffULL)"); } } while (0);
    } else {

        uint64_t cost = 0;
        while (bp->parent != bp->child) {
            cost += bp->linkCost;
            bp = Node_getBestParent(bp->parent);
        }
        do { if (!((Node_getCost(node) == cost))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("NodeStore.c"), (266), "(Node_getCost(node) == cost)"); } } while (0);
    }
}



static void _verify(struct NodeStore_pvt* store, 
# 272 "dht/dhtcore/NodeStore.c" 3 4
                                                _Bool 
# 272 "dht/dhtcore/NodeStore.c"
                                                     full, char* file, int line)
{
    if (!1 || (store->disarmCheck && !store->fullVerify)) {
        return;
    }
    do { if (!((Node_getBestParent(store->pub.selfNode) == store->selfLink || !store->selfLink))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("NodeStore.c"), (277), "(Node_getBestParent(store->pub.selfNode) == store->selfLink || !store->selfLink)"); } } while (0);
    int linkedNodes = 0;
    int nodeCount = 0;
    struct Node_Two* nn = 
# 280 "dht/dhtcore/NodeStore.c" 3 4
                         ((void *)0)
# 280 "dht/dhtcore/NodeStore.c"
                             ;
    for ((nn) = NodeRBTree_RB_MINMAX(&store->nodeTree, -1); (nn) != 
# 281 "dht/dhtcore/NodeStore.c" 3 4
   ((void *)0)
# 281 "dht/dhtcore/NodeStore.c"
   ; (nn) = NodeRBTree_RB_NEXT(nn)) {
        _verifyNode(nn, store, full, file, line);
        if (Node_getBestParent(nn)) { linkedNodes++; }
        nodeCount++;
    }
    do { if (!(linkedNodes == store->pub.linkedNodes)) { Assert_failure("Assertion failure [%s:%d] [%s]\n", (file), (line), "linkedNodes == store->pub.linkedNodes"); } } while (0);
    do { if (!(nodeCount == store->pub.nodeCount)) { Assert_failure("Assertion failure [%s:%d] [%s]\n", (file), (line), "nodeCount == store->pub.nodeCount"); } } while (0);
}
# 307 "dht/dhtcore/NodeStore.c"
static uint64_t extendRoute(uint64_t routeToParent,
                            struct EncodingScheme* parentScheme,
                            uint64_t routeParentToChild,
                            int previousLinkEncoding)
{
    do { if (!((routeParentToChild != (~((uint64_t)0))))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("NodeStore.c"), (312), "(routeParentToChild != (~((uint64_t)0)))"); } } while (0);


    int nextLinkEncoding = EncodingScheme_getFormNum(parentScheme, routeParentToChild);
    if (nextLinkEncoding == -1) { return (((uint64_t)~0)-1); }



    if (previousLinkEncoding > nextLinkEncoding) {
        routeParentToChild =
            EncodingScheme_convertLabel(parentScheme, routeParentToChild, previousLinkEncoding);
        do { if (!((routeParentToChild != (~((uint64_t)0))))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("NodeStore.c"), (323), "(routeParentToChild != (~((uint64_t)0)))"); } } while (0);
    }
    return LabelSplicer_splice(routeParentToChild, routeToParent);
}

static void update(struct Node_Link* link,
                   int64_t linkCostDiff,
                   struct NodeStore_pvt* store)
{
    if (linkCostDiff + link->linkCost > 
# 332 "dht/dhtcore/NodeStore.c" 3 4
                                       0xffffffffU
# 332 "dht/dhtcore/NodeStore.c"
                                                 ) {
        link->linkCost = 
# 333 "dht/dhtcore/NodeStore.c" 3 4
                        0xffffffffU
# 333 "dht/dhtcore/NodeStore.c"
                                  ;
        logLink(store, link, "link cost set to maximum");
    } else if (linkCostDiff + link->linkCost < 1024) {
        link->linkCost = 1024;

    } else {
        link->linkCost += linkCostDiff;
    }
    uint32_t minMultiHopCost = (uint32_t)1 << 20;
    if (!Node_isOneHopLink(link) && link->linkCost < minMultiHopCost) {

        link->linkCost = minMultiHopCost;
    }
}

static 
# 348 "dht/dhtcore/NodeStore.c" 3 4
      _Bool 
# 348 "dht/dhtcore/NodeStore.c"
           isPeer(struct Node_Two* node, struct NodeStore_pvt* store)
{
    struct Node_Link* bp = Node_getBestParent(node);
    return bp && bp->parent == store->pub.selfNode && Node_isOneHopLink(bp);
}

static void setParentCostAndPath(struct Node_Two* node,
                                 struct Node_Link* parent,
                                 uint64_t cost,
                                 uint64_t path,
                                 struct NodeStore_pvt* store)
{
    uint64_t oldPath = node->address.path;
    Node_setParentCostAndPath(node, parent, cost, path);
    if (oldPath != path && store->pub.onBestPathChange) {
        store->pub.onBestPathChange(store->pub.onBestPathChangeCtx, node);
    }
}







static uint64_t guessCostOfChild(struct Node_Link* link)
{

    uint64_t guess = Node_getCost(link->parent) + link->linkCost;
    if (guess < Node_getCost(link->parent)) {

        guess = 
# 379 "dht/dhtcore/NodeStore.c" 3 4
               0xffffffffffffffffULL
# 379 "dht/dhtcore/NodeStore.c"
                         ;
    }

    do { if (!((guess >= Node_getCost(link->parent)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("NodeStore.c"), (382), "(guess >= Node_getCost(link->parent))"); } } while (0);
    return guess;
}
# 396 "dht/dhtcore/NodeStore.c"
static 
# 396 "dht/dhtcore/NodeStore.c" 3 4
      _Bool 
# 396 "dht/dhtcore/NodeStore.c"
           findBestParent0(struct Node_Two* node, struct NodeStore_pvt* store)
{
    node->marked = 0;
    if (node == store->pub.selfNode) { return 
# 399 "dht/dhtcore/NodeStore.c" 3 4
                                             0
# 399 "dht/dhtcore/NodeStore.c"
                                                  ; }
    struct Node_Link* bestLink = 
# 400 "dht/dhtcore/NodeStore.c" 3 4
                                ((void *)0)
# 400 "dht/dhtcore/NodeStore.c"
                                    ;
    uint64_t bestCost = 
# 401 "dht/dhtcore/NodeStore.c" 3 4
                       0xffffffffffffffffULL
# 401 "dht/dhtcore/NodeStore.c"
                                 ;
    uint64_t bestPath = 
# 402 "dht/dhtcore/NodeStore.c" 3 4
                       0xffffffffffffffffULL
# 402 "dht/dhtcore/NodeStore.c"
                                 ;
    for (struct Node_Link* link = node->reversePeers; link; link = link->nextPeer) {
        if (link->linkCost == 
# 404 "dht/dhtcore/NodeStore.c" 3 4
                             0xffffffffU
# 404 "dht/dhtcore/NodeStore.c"
                                       ) { continue; }
        uint64_t cost = guessCostOfChild(link);
        if (bestCost <= cost) { continue; }
        if (bestLink && Node_isOneHopLink(bestLink) && !Node_isOneHopLink(link)) { continue; }
        if (!Node_getBestParent(link->parent)) { continue; }
        if (Node_isAncestorOf(node, link->parent)) { continue; }
        uint64_t path =
            extendRoute(link->parent->address.path,
            link->parent->encodingScheme,
            link->cannonicalLabel,
            Node_getBestParent(link->parent)->inverseLinkEncodingFormNumber);
        if (path == (((uint64_t)~0))) { continue; }
        if (path == (((uint64_t)~0)-1)) { continue; }
        do { if (!((LabelSplicer_routesThrough(path, link->parent->address.path)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("NodeStore.c"), (417), "(LabelSplicer_routesThrough(path, link->parent->address.path))"); } } while (0);
        bestCost = cost;
        bestPath = path;
        bestLink = link;
    }
    if (bestCost != Node_getCost(node) || bestPath != node->address.path) {
        if (!Node_getBestParent(node)) { store->pub.linkedNodes++; }
        if (!bestLink) { store->pub.linkedNodes--; }
        struct Node_Link* link = 
# 425 "dht/dhtcore/NodeStore.c" 3 4
                                ((void *)0)
# 425 "dht/dhtcore/NodeStore.c"
                                    ;
        for ((link) = PeerRBTree_RB_MINMAX(&node->peerTree, -1); (link) != 
# 426 "dht/dhtcore/NodeStore.c" 3 4
       ((void *)0)
# 426 "dht/dhtcore/NodeStore.c"
       ; (link) = PeerRBTree_RB_NEXT(link)) {
            if (Node_getCost(node) > bestCost || Node_getBestParent(link->child) == link) {
                link->child->marked = 1;
            }
        }
        setParentCostAndPath(node, bestLink, bestCost, bestPath, store);
        return 
# 432 "dht/dhtcore/NodeStore.c" 3 4
              1
# 432 "dht/dhtcore/NodeStore.c"
                  ;
    }
    return 
# 434 "dht/dhtcore/NodeStore.c" 3 4
          0
# 434 "dht/dhtcore/NodeStore.c"
               ;
}


static 
# 438 "dht/dhtcore/NodeStore.c" 3 4
      _Bool 
# 438 "dht/dhtcore/NodeStore.c"
           findBestParent(struct Node_Two* node, struct NodeStore_pvt* store)
{
    uint64_t time0 = Time_hrtime();
    if (!findBestParent0(node, store)) { return 
# 441 "dht/dhtcore/NodeStore.c" 3 4
                                               0
# 441 "dht/dhtcore/NodeStore.c"
                                                    ; }
    int ret = 0;
    int cycle = 0;
    do {
        do { if (!((cycle++ < 10000))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("NodeStore.c"), (445), "(cycle++ < 10000)"); } } while (0);
        ret = 0;
        for (struct Node_Two* n = NodeStore_getNextNode(&store->pub, 
# 447 "dht/dhtcore/NodeStore.c" 3 4
                                                                    ((void *)0)
# 447 "dht/dhtcore/NodeStore.c"
                                                                        );
             n;
             n = NodeStore_getNextNode(&store->pub, n))
        {
            if (n->marked) {
                ret |= findBestParent0(n, store);
            }
        }
    } while (ret);
    uint64_t time1 = Time_hrtime();
    if ((int64_t)(time1 - time0) > 1000000) {
        do { if (store->logger && Log_Level_WARN >= Log_Level_DEBUG) { Log_print(store->logger, Log_Level_WARN, "NodeStore.c", 458, "\n\nfindBestParent() took [%lld] ms\n\n", (long long) ((time1 - time0) / 1000000)); } } while (0)
                                                    ;
    }
    return 
# 461 "dht/dhtcore/NodeStore.c" 3 4
          1
# 461 "dht/dhtcore/NodeStore.c"
              ;
}







static void handleLinkNews(struct Node_Link* link,
                           uint32_t newLinkCost,
                           struct NodeStore_pvt* store)
{
    int64_t linkCostDiff = newLinkCost;
    linkCostDiff -= link->linkCost;
    update(link, linkCostDiff, store);

    if (findBestParent(link->child, store)) {

        _verify(store, 
# 480 "dht/dhtcore/NodeStore.c" 3 4
       0
# 480 "dht/dhtcore/NodeStore.c"
       , "NodeStore.c", 480);
    }
}

void NodeStore_unlinkNodes(struct NodeStore* nodeStore, struct Node_Link* link)
{
    struct NodeStore_pvt* store = (__extension__ ({ __typeof__((struct NodeStore_pvt*) nodeStore) Identity_ptr = ((struct NodeStore_pvt*) nodeStore); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x0254ff724c14aca9ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("NodeStore.c"), (486), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x0254ff724c14aca9ull))"); } } while (0); Identity_ptr; }));
    struct Node_Two* child = (__extension__ ({ __typeof__(link->child) Identity_ptr = (link->child); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x0254ff724c14aca9ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("NodeStore.c"), (487), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x0254ff724c14aca9ull))"); } } while (0); Identity_ptr; }));
    struct Node_Two* parent = (__extension__ ({ __typeof__(link->parent) Identity_ptr = (link->parent); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x0254ff724c14aca9ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("NodeStore.c"), (488), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x0254ff724c14aca9ull))"); } } while (0); Identity_ptr; }));
    _verify(store, 
# 489 "dht/dhtcore/NodeStore.c" 3 4
   0
# 489 "dht/dhtcore/NodeStore.c"
   , "NodeStore.c", 489);

    if (parent == store->pub.selfNode) {


        if (link == store->selfLink) { return; }

        do { if (!((Node_isOneHopLink(link)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("NodeStore.c"), (496), "(Node_isOneHopLink(link))"); } } while (0);
        store->pub.peerCount--;
        if (1) {
            uint8_t addr[60];
            Address_print(addr, &child->address);
            do { if (store->logger && Log_Level_INFO >= Log_Level_DEBUG) { Log_print(store->logger, Log_Level_INFO, "NodeStore.c", 501, "Direct peer [%s] has been unlinked", addr); } } while (0);
        }
    }

    handleLinkNews(link, 
# 505 "dht/dhtcore/NodeStore.c" 3 4
                        0xffffffffU
# 505 "dht/dhtcore/NodeStore.c"
                                  , store);

    _verify(store, 
# 507 "dht/dhtcore/NodeStore.c" 3 4
   0
# 507 "dht/dhtcore/NodeStore.c"
   , "NodeStore.c", 507);


    struct Node_Link* current = child->reversePeers;
    struct Node_Link** currentP = &child->reversePeers;
    while (current) {
        if (current == link) {
            *currentP = current->nextPeer;
            break;
        }
        currentP = &(current->nextPeer);
        current = *currentP;
    }
    do { if (!((current))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("NodeStore.c"), (520), "(current)"); } } while (0);


    do { if (!((link == PeerRBTree_RB_FIND(&parent->peerTree, link)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("NodeStore.c"), (523), "(link == PeerRBTree_RB_FIND(&parent->peerTree, link))"); } } while (0);
    PeerRBTree_RB_REMOVE(&parent->peerTree, link);

    link->nextPeer = store->linksToFree;
    store->linksToFree = link;


    link->parent = 
# 530 "dht/dhtcore/NodeStore.c" 3 4
                  ((void *)0)
# 530 "dht/dhtcore/NodeStore.c"
                      ;
    link->child = 
# 531 "dht/dhtcore/NodeStore.c" 3 4
                 ((void *)0)
# 531 "dht/dhtcore/NodeStore.c"
                     ;

    _verify(store, 
# 533 "dht/dhtcore/NodeStore.c" 3 4
   0
# 533 "dht/dhtcore/NodeStore.c"
   , "NodeStore.c", 533);
}
# 547 "dht/dhtcore/NodeStore.c"
static struct Node_Link* linkNodes(struct Node_Two* parent,
                                   struct Node_Two* child,
                                   uint64_t cannonicalLabel,
                                   int64_t linkCostDiff,
                                   int inverseLinkEncodingFormNumber,
                                   uint64_t discoveredPath,
                                   struct NodeStore_pvt* store)
{
    _verify(store, 
# 555 "dht/dhtcore/NodeStore.c" 3 4
   0
# 555 "dht/dhtcore/NodeStore.c"
   , "NodeStore.c", 555);

    if (1) {
        uint8_t parentIp[40];
        uint8_t childIp[40];
        AddrTools_printIp(parentIp, parent->address.ip6.bytes);
        AddrTools_printIp(childIp, child->address.ip6.bytes);
        uint8_t printedLabel[20];
        AddrTools_printPath(printedLabel, cannonicalLabel);
        do { if (store->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(store->logger, Log_Level_DEBUG, "NodeStore.c", 564, "Linking [%s] with [%s] with label fragment [%s]", parentIp, childIp, printedLabel); } } while (0)
                                                  ;
    }





    do { if (!((cannonicalLabel != 1 || store->selfLink == 
# 572 "dht/dhtcore/NodeStore.c" 3 4
   ((void *)0)
# 572 "dht/dhtcore/NodeStore.c"
   ))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("NodeStore.c"), (572), "(cannonicalLabel != 1 || store->selfLink == ((void *)0))"); } } while (0);

    if (1) {
        uint64_t definitelyCannonical =
            EncodingScheme_convertLabel(parent->encodingScheme,
                                        cannonicalLabel,
                                        (-5000));
        do { if (!((definitelyCannonical == cannonicalLabel))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("NodeStore.c"), (579), "(definitelyCannonical == cannonicalLabel)"); } } while (0);
    }

    {
        struct Node_Link* link;
        for ((link) = PeerRBTree_RB_MINMAX(&parent->peerTree, 1); (link) != 
# 584 "dht/dhtcore/NodeStore.c" 3 4
       ((void *)0)
# 584 "dht/dhtcore/NodeStore.c"
       ; (link) = PeerRBTree_RB_PREV(link)) {
            (__extension__ ({ __typeof__(link) Identity_ptr = (link); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x0254ff724c14aca9ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("NodeStore.c"), (585), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x0254ff724c14aca9ull))"); } } while (0); Identity_ptr; }));
            if (link->child == child) {
                if (link->cannonicalLabel != cannonicalLabel) {


                    continue;

                } else if (link->inverseLinkEncodingFormNumber != inverseLinkEncodingFormNumber) {
                    logLink(store, link, "Relinking nodes with different encoding form");


                    link->inverseLinkEncodingFormNumber = inverseLinkEncodingFormNumber;
                }
                handleLinkNews(link, linkCostDiff+link->linkCost, store);
                return link;
            }
        }
    }

    if (1) {
        struct Node_Link dummy = { .cannonicalLabel = cannonicalLabel };
        struct Node_Link* link = (__extension__ ({ __typeof__(PeerRBTree_RB_FIND(&parent->peerTree, &dummy)) Identity_ptr = (PeerRBTree_RB_FIND(&parent->peerTree, &dummy)); do { if (!((!Identity_ptr || Identity_ptr->Identity_verifier == ((unsigned long) 0x0254ff724c14aca9ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("NodeStore.c"), (606), "(!Identity_ptr || Identity_ptr->Identity_verifier == ((unsigned long) 0x0254ff724c14aca9ull))"); } } while (0); Identity_ptr; }));
        if (link) {
            logLink(store, link, "Attempted to create alternate link with same label!");
            do { if (!((0))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("NodeStore.c"), (609), "(0)"); } } while (0);
            return link;
        }
    }

    do { if (!((cannonicalLabel <= discoveredPath))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("NodeStore.c"), (614), "(cannonicalLabel <= discoveredPath)"); } } while (0);

    struct Node_Link* link = getLink(store);


    link->cannonicalLabel = cannonicalLabel;
    link->inverseLinkEncodingFormNumber = inverseLinkEncodingFormNumber;
    link->child = child;
    link->parent = parent;
    link->discoveredPath = discoveredPath;
    link->linkCost = 0;
    link->timeLastSeen = Time_currentTimeMilliseconds(store->eventBase);
    (link)->Identity_verifier = ((unsigned long) 0x0254ff724c14aca9ull);


    link->nextPeer = child->reversePeers;
    child->reversePeers = link;


    do { if (!((!PeerRBTree_RB_FIND(&parent->peerTree, link)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("NodeStore.c"), (633), "(!PeerRBTree_RB_FIND(&parent->peerTree, link))"); } } while (0);
    PeerRBTree_RB_INSERT(&parent->peerTree, link);


    if (!NodeRBTree_RB_FIND(&store->nodeTree, child)) {
        if (child == parent) {
            do { if (!((cannonicalLabel == 1))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("NodeStore.c"), (639), "(cannonicalLabel == 1)"); } } while (0);
            do { if (!((!store->pub.nodeCount))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("NodeStore.c"), (640), "(!store->pub.nodeCount)"); } } while (0);
            do { if (!((!store->selfLink))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("NodeStore.c"), (641), "(!store->selfLink)"); } } while (0);
            store->selfLink = link;
            Node_setParentCostAndPath(child, link, 0, 1);
            store->pub.linkedNodes++;
        }
        NodeRBTree_RB_INSERT(&store->nodeTree, child);
        store->pub.nodeCount++;
    }

    handleLinkNews(link, linkCostDiff+link->linkCost, store);

    if (parent == store->pub.selfNode && child != store->pub.selfNode) {
        do { if (!((Node_isOneHopLink(link)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("NodeStore.c"), (653), "(Node_isOneHopLink(link))"); } } while (0);
        store->pub.peerCount++;
        if (1) {
            uint8_t addr[60];
            Address_print(addr, &child->address);
            do { if (store->logger && Log_Level_INFO >= Log_Level_DEBUG) { Log_print(store->logger, Log_Level_INFO, "NodeStore.c", 658, "Direct peer [%s] has been linked", addr); } } while (0);
        }
    }

    _verify(store, 
# 662 "dht/dhtcore/NodeStore.c" 3 4
   0
# 662 "dht/dhtcore/NodeStore.c"
   , "NodeStore.c", 662);
    return link;
}





static uint64_t removeLinkFromLabel(struct Node_Link* link, uint64_t label)
{

    uint64_t unspliced = LabelSplicer_unsplice(label, link->cannonicalLabel);

    int formNum = EncodingScheme_getFormNum(link->child->encodingScheme, unspliced);

    if (formNum < link->inverseLinkEncodingFormNumber) {

        return 
# 679 "dht/dhtcore/NodeStore.c" 3 4
              0xffffffffffffffffULL
# 679 "dht/dhtcore/NodeStore.c"
                                            ;
    }

    uint64_t cannonical =
        EncodingScheme_convertLabel(link->child->encodingScheme,
                                    unspliced,
                                    (-5000));


    if (formNum > link->inverseLinkEncodingFormNumber && cannonical != unspliced) {
        return (
# 689 "dht/dhtcore/NodeStore.c" 3 4
              0xffffffffffffffffULL
# 689 "dht/dhtcore/NodeStore.c"
              -1);
    }

    do { if (!((cannonical != (~((uint64_t)0))))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("NodeStore.c"), (692), "(cannonical != (~((uint64_t)0)))"); } } while (0);

    return cannonical;
}
# 720 "dht/dhtcore/NodeStore.c"
static uint64_t firstHopInPath(uint64_t label,
                               struct Node_Link** outLink,
                               struct Node_Link* parentLink,
                               struct NodeStore_pvt* store)
{




    struct Node_Link tmpl = { .cannonicalLabel = label };
    struct Node_Link* nextLink =
        (__extension__ ({ __typeof__(PeerRBTree_RB_NFIND(&parentLink->child->peerTree, &tmpl)) Identity_ptr = (PeerRBTree_RB_NFIND(&parentLink->child->peerTree, &tmpl)); do { if (!((!Identity_ptr || Identity_ptr->Identity_verifier == ((unsigned long) 0x0254ff724c14aca9ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("NodeStore.c"), (731), "(!Identity_ptr || Identity_ptr->Identity_verifier == ((unsigned long) 0x0254ff724c14aca9ull))"); } } while (0); Identity_ptr; }));


    while (nextLink && !LabelSplicer_routesThrough(label, nextLink->cannonicalLabel)) {
        nextLink = (__extension__ ({ __typeof__(PeerRBTree_RB_NEXT(nextLink)) Identity_ptr = (PeerRBTree_RB_NEXT(nextLink)); do { if (!((!Identity_ptr || Identity_ptr->Identity_verifier == ((unsigned long) 0x0254ff724c14aca9ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("NodeStore.c"), (735), "(!Identity_ptr || Identity_ptr->Identity_verifier == ((unsigned long) 0x0254ff724c14aca9ull))"); } } while (0); Identity_ptr; }));
    }


    if (!nextLink || nextLink == store->selfLink) {
        return (
# 740 "dht/dhtcore/NodeStore.c" 3 4
              0xffffffffffffffffULL
# 740 "dht/dhtcore/NodeStore.c"
              -1);
    }
# 756 "dht/dhtcore/NodeStore.c"
    if (nextLink == parentLink) {
        return (
# 757 "dht/dhtcore/NodeStore.c" 3 4
              0xffffffffffffffffULL
# 757 "dht/dhtcore/NodeStore.c"
              -1);
    }

    if (label == nextLink->cannonicalLabel) {

        *outLink = nextLink;
        return 1;
    }

    if (!LabelSplicer_routesThrough(label, nextLink->cannonicalLabel)) {

        return (
# 768 "dht/dhtcore/NodeStore.c" 3 4
              0xffffffffffffffffULL
# 768 "dht/dhtcore/NodeStore.c"
              -1);
    }

    *outLink = nextLink;


    label = removeLinkFromLabel(nextLink, label);
    if ((((uint64_t)label) >> 63)) {
        return 
# 776 "dht/dhtcore/NodeStore.c" 3 4
              0xffffffffffffffffULL
# 776 "dht/dhtcore/NodeStore.c"
                                    ;
    }

    return label;
}


static uint64_t findClosest(uint64_t path,
                            struct Node_Link** output,
                            struct Node_Link* parentLink,
                            struct NodeStore_pvt* store)
{
    for (;;) {
        struct Node_Link* nextLink = 
# 789 "dht/dhtcore/NodeStore.c" 3 4
                                    ((void *)0)
# 789 "dht/dhtcore/NodeStore.c"
                                        ;
        uint64_t nextPath = firstHopInPath(path, &nextLink, parentLink, store);
        if (nextPath == (
# 791 "dht/dhtcore/NodeStore.c" 3 4
                       0xffffffffffffffffULL
# 791 "dht/dhtcore/NodeStore.c"
                       -1)) {
            *output = parentLink;
            return path;
        }
        if (
# 795 "dht/dhtcore/NodeStore.c" 3 4
           0xffffffffffffffffULL 
# 795 "dht/dhtcore/NodeStore.c"
                                  == nextPath) {
            return (~((uint64_t)0));
        }
        do { if (!((nextLink))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("NodeStore.c"), (798), "(nextLink)"); } } while (0);
        path = nextPath;
        parentLink = nextLink;
    }
}

static struct Node_Two* nodeForIp(struct NodeStore_pvt* store, uint8_t ip[16])
{
    struct Node_Two fakeNode;
    (&fakeNode)->Identity_verifier = ((unsigned long) 0x0254ff724c14aca9ull);
    Bits__memcpy(fakeNode.address.ip6.bytes,ip,16,"NodeStore.c",808);
    return (__extension__ ({ __typeof__(NodeRBTree_RB_FIND(&store->nodeTree, &fakeNode)) Identity_ptr = (NodeRBTree_RB_FIND(&store->nodeTree, &fakeNode)); do { if (!((!Identity_ptr || Identity_ptr->Identity_verifier == ((unsigned long) 0x0254ff724c14aca9ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("NodeStore.c"), (809), "(!Identity_ptr || Identity_ptr->Identity_verifier == ((unsigned long) 0x0254ff724c14aca9ull))"); } } while (0); Identity_ptr; }));
}

static void freePendingLinks(struct NodeStore_pvt* store)
{
    struct Node_Link* link;
    while ((link = store->linksToFree)) {
        store->linksToFree = link->nextPeer;
        freeLink(link, store);
    }
}

static struct Node_Link* discoverLinkC(struct NodeStore_pvt* store,
                                       struct Node_Link* closestKnown,
                                       uint64_t pathKnownParentChild,
                                       struct Node_Two* child,
                                       uint64_t discoveredPath,
                                       int inverseLinkEncodingFormNumber)
{

    struct Node_Link* closest = 
# 829 "dht/dhtcore/NodeStore.c" 3 4
                               ((void *)0)
# 829 "dht/dhtcore/NodeStore.c"
                                   ;
    uint64_t pathParentChild = findClosest(pathKnownParentChild, &closest, closestKnown, store);

    if (pathParentChild == (~((uint64_t)0))) {
        return 
# 833 "dht/dhtcore/NodeStore.c" 3 4
              ((void *)0)
# 833 "dht/dhtcore/NodeStore.c"
                  ;
    }
    if (!EncodingScheme_isOneHop(closest->child->encodingScheme, pathParentChild)) {
        do { if (store->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(store->logger, Log_Level_DEBUG, "NodeStore.c", 836, "Not discovering link because it's multi-hop"); } } while (0);
        return 
# 837 "dht/dhtcore/NodeStore.c" 3 4
              ((void *)0)
# 837 "dht/dhtcore/NodeStore.c"
                  ;
    }

    struct Node_Two* parent = closest->child;

    if (1) {
        uint8_t parentStr[40];
        uint8_t childStr[40];
        uint8_t pathStr[20];

        AddrTools_printIp(parentStr, parent->address.ip6.bytes);
        AddrTools_printIp(childStr, child->address.ip6.bytes);
        AddrTools_printPath(pathStr, pathParentChild);
        do { if (store->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(store->logger, Log_Level_DEBUG, "NodeStore.c", 850, "discoverLinkC( [%s]->[%s] [%s] )", parentStr, childStr, pathStr); } } while (0);
    }

    if (closest == store->selfLink &&
        !EncodingScheme_isOneHop(parent->encodingScheme, pathParentChild))
    {
        do { if (store->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(store->logger, Log_Level_DEBUG, "NodeStore.c", 856, "Attempting to create a link with no parent peer"); } } while (0);
        return 
# 857 "dht/dhtcore/NodeStore.c" 3 4
              ((void *)0)
# 857 "dht/dhtcore/NodeStore.c"
                  ;
    }

    if (parent == child) {
        if (pathParentChild == 1) {



            return closest;
        }

        do { if (store->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(store->logger, Log_Level_DEBUG, "NodeStore.c", 868, "Loopey route"); } } while (0);






        return 
# 875 "dht/dhtcore/NodeStore.c" 3 4
              ((void *)0)
# 875 "dht/dhtcore/NodeStore.c"
                  ;
    }

    if (EncodingScheme_isSelfRoute(parent->encodingScheme, pathParentChild)) {





        do { } while (0);





        if (discoveredPath == parent->address.path) {
            logLink(store, closest, "Double-checking path node change");

            uint64_t oldPath = child->address.path;
            child->address.path = parent->address.path;
            RumorMill__addNode((store->renumberMill), (&child->address), "NodeStore.c", 895);
            child->address.path = oldPath;

            _verify(store, 
# 898 "dht/dhtcore/NodeStore.c" 3 4
           0
# 898 "dht/dhtcore/NodeStore.c"
           , "NodeStore.c", 898);
            return 
# 899 "dht/dhtcore/NodeStore.c" 3 4
                  ((void *)0)
# 899 "dht/dhtcore/NodeStore.c"
                      ;
        } else {
            logLink(store, closest, "Unlinking node for path change");
            struct Node_Link* nextClosest = Node_getBestParent(closest->parent);
            uint64_t nextPPC = closest->cannonicalLabel;
            NodeStore_unlinkNodes(&store->pub, closest);
            closest = nextClosest;
            pathParentChild = nextPPC;
            parent = closest->child;
        }
    }
# 918 "dht/dhtcore/NodeStore.c"
    struct Node_Link* parentLink = linkNodes(parent,
                                             child,
                                             pathParentChild,
                                             0,
                                             inverseLinkEncodingFormNumber,
                                             discoveredPath,
                                             store);

    _verify(store, 
# 926 "dht/dhtcore/NodeStore.c" 3 4
   0
# 926 "dht/dhtcore/NodeStore.c"
   , "NodeStore.c", 926);

    return parentLink;
}

static void fixLink(struct Node_Link* parentLink,
                    struct Node_Link** outLinks,
                    struct NodeStore_pvt* store)
{
    int verifyOrder = 0;




    struct Node_Link* splitLink = PeerRBTree_RB_MINMAX(&parentLink->parent->peerTree, -1);
    while (splitLink) {
        if (splitLink == parentLink) {
            if (1) {
                verifyOrder = 1;
                splitLink = PeerRBTree_RB_NEXT(splitLink);
                continue;
            } else {

                break;
            }
        }

        if (!LabelSplicer_routesThrough(splitLink->cannonicalLabel, parentLink->cannonicalLabel)) {
            splitLink = PeerRBTree_RB_NEXT(splitLink);
            continue;
        }

        if (1) {
            do { if (!((!verifyOrder))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("NodeStore.c"), (959), "(!verifyOrder)"); } } while (0);
        }

        struct Node_Two* grandChild = splitLink->child;

        if (parentLink->child == grandChild) {


        } else {
            logLink(store, splitLink, "Splitting link");


            uint64_t childToGrandchild =
                LabelSplicer_unsplice(splitLink->cannonicalLabel, parentLink->cannonicalLabel);
            do { if (!((parentLink->child))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("NodeStore.c"), (973), "(parentLink->child)"); } } while (0);
            childToGrandchild =
                EncodingScheme_convertLabel(parentLink->child->encodingScheme,
                                            childToGrandchild,
                                            (-5000));

            do { if (!((childToGrandchild < 
# 979 "dht/dhtcore/NodeStore.c" 3 4
           0xffffffffffffffffULL
# 979 "dht/dhtcore/NodeStore.c"
           ))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("NodeStore.c"), (979), "(childToGrandchild < 0xffffffffffffffffULL)"); } } while (0);
            do { if (!((childToGrandchild != 1))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("NodeStore.c"), (980), "(childToGrandchild != 1)"); } } while (0);
            do { if (!((splitLink->cannonicalLabel != parentLink->cannonicalLabel))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("NodeStore.c"), (981), "(splitLink->cannonicalLabel != parentLink->cannonicalLabel)"); } } while (0);




            uint64_t discoveredPath = parentLink->discoveredPath;
            if (childToGrandchild > discoveredPath) { discoveredPath = childToGrandchild; }

            struct Node_Link* childLink =
                discoverLinkC(store, parentLink, childToGrandchild, grandChild,
                              discoveredPath, splitLink->inverseLinkEncodingFormNumber);
# 1001 "dht/dhtcore/NodeStore.c"
            if (childLink && !childLink->nextInSplitList) {


                for (struct Node_Link** x = outLinks;; x = &(*x)->nextInSplitList) {
                    if (*x == childLink) { break; }
                    if (*x && (*x)->cannonicalLabel <= childLink->cannonicalLabel) { continue; }
                    childLink->nextInSplitList = *x;
                    *x = childLink;
                    break;
                }
            }
        }

        _verify(store, 
# 1014 "dht/dhtcore/NodeStore.c" 3 4
       0
# 1014 "dht/dhtcore/NodeStore.c"
       , "NodeStore.c", 1014);

        struct Node_Link* next = PeerRBTree_RB_NEXT(splitLink);
        NodeStore_unlinkNodes(&store->pub, splitLink);
        splitLink = next;
    }
}

static void fixLinks(struct Node_Link* parentLinkList,
                     struct Node_Link** outLinks,
                     struct NodeStore_pvt* store)
{
    while (parentLinkList) {
        struct Node_Link* next = parentLinkList->nextInSplitList;
        parentLinkList->nextInSplitList = 
# 1028 "dht/dhtcore/NodeStore.c" 3 4
                                         ((void *)0)
# 1028 "dht/dhtcore/NodeStore.c"
                                             ;


        if (parentLinkList->child) {
            fixLink(parentLinkList, outLinks, store);
        }

        parentLinkList = next;
    }
}


static struct Node_Link* discoverLink(struct NodeStore_pvt* store,
                                      uint64_t path,
                                      struct Node_Two* child,
                                      int inverseLinkEncodingFormNumber)
{
    struct Node_Link* link =
        discoverLinkC(store, store->selfLink, path, child, path, inverseLinkEncodingFormNumber);

    if (!link) { return 
# 1048 "dht/dhtcore/NodeStore.c" 3 4
                       ((void *)0)
# 1048 "dht/dhtcore/NodeStore.c"
                           ; }






    struct Node_Link* ol = 
# 1055 "dht/dhtcore/NodeStore.c" 3 4
                          ((void *)0)
# 1055 "dht/dhtcore/NodeStore.c"
                              ;
    struct Node_Link* nl = 
# 1056 "dht/dhtcore/NodeStore.c" 3 4
                          ((void *)0)
# 1056 "dht/dhtcore/NodeStore.c"
                              ;
    fixLinks(link, &ol, store);
    for (;;) {
        if (ol) {
            fixLinks(ol, &nl, store);
            ol = 
# 1061 "dht/dhtcore/NodeStore.c" 3 4
                ((void *)0)
# 1061 "dht/dhtcore/NodeStore.c"
                    ;
        } else if (nl) {
            fixLinks(nl, &ol, store);
            nl = 
# 1064 "dht/dhtcore/NodeStore.c" 3 4
                ((void *)0)
# 1064 "dht/dhtcore/NodeStore.c"
                    ;
        } else {
            break;
        }
    }

    _verify(store, 
# 1070 "dht/dhtcore/NodeStore.c" 3 4
   1
# 1070 "dht/dhtcore/NodeStore.c"
   , "NodeStore.c", 1070);

    return link;
}

static struct Node_Two* whichIsWorse(struct Node_Two* one,
                                     struct Node_Two* two,
                                     struct NodeStore_pvt* store)
{

    int worse = isPeer(one, store) - isPeer(two, store);
    if (worse) {
        return (worse > 0) ? two : one;
    }

    worse = (one->address.path == 
# 1085 "dht/dhtcore/NodeStore.c" 3 4
                                 0xffffffffffffffffULL
# 1085 "dht/dhtcore/NodeStore.c"
                                           ) - (two->address.path == 
# 1085 "dht/dhtcore/NodeStore.c" 3 4
                                                                     0xffffffffffffffffULL
# 1085 "dht/dhtcore/NodeStore.c"
                                                                               );
    if (worse) {
        return (worse > 0) ? one : two;
    }

    if (one->address.protocolVersion != two->address.protocolVersion) {
        if (one->address.protocolVersion < 20) {
            if (two->address.protocolVersion >= 20) {
                return one;
            }
        } else if (two->address.protocolVersion < 20) {
            if (one->address.protocolVersion >= 20) {
                return two;
            }
        }
    }

    uint32_t selfPrefix = Address_getPrefix(&store->pub.selfNode->address);
    uint64_t distOne = Address_getPrefix(&one->address) ^ selfPrefix;
    uint64_t distTwo = Address_getPrefix(&two->address) ^ selfPrefix;
    distOne += Node_getCost(one);
    distTwo += Node_getCost(two);
    if (0) {
        distOne += Bits_log2x64(one->address.path) << 26;
        distTwo += Bits_log2x64(two->address.path) << 26;
    }
    if (distOne < distTwo) { return two; }
    return one;
}

struct NodeList* NodeStore_getNodesForBucket(struct NodeStore* nodeStore,
                                             struct Allocator* allocator,
                                             uint16_t bucket,
                                             const uint32_t count)
{
    struct NodeStore_pvt* store = (__extension__ ({ __typeof__((struct NodeStore_pvt*)nodeStore) Identity_ptr = ((struct NodeStore_pvt*)nodeStore); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x0254ff724c14aca9ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("NodeStore.c"), (1120), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x0254ff724c14aca9ull))"); } } while (0); Identity_ptr; }));
    struct NodeList* nodeList = Allocator__malloc((allocator),(sizeof(struct NodeList)),"NodeStore.c",1121);
    nodeList->nodes = Allocator__calloc((allocator),(count),(sizeof(char*)),"NodeStore.c",1122);
    nodeList->size = 0;
    struct Node_Two* nn = 
# 1124 "dht/dhtcore/NodeStore.c" 3 4
                         ((void *)0)
# 1124 "dht/dhtcore/NodeStore.c"
                             ;
    for ((nn) = NodeRBTree_RB_MINMAX(&store->nodeTree, -1); (nn) != 
# 1125 "dht/dhtcore/NodeStore.c" 3 4
   ((void *)0)
# 1125 "dht/dhtcore/NodeStore.c"
   ; (nn) = NodeRBTree_RB_NEXT(nn)) {
        if (Node_getCost(nn) == 
# 1126 "dht/dhtcore/NodeStore.c" 3 4
                               0xffffffffffffffffULL
# 1126 "dht/dhtcore/NodeStore.c"
                                         ) { continue; }
        if (NodeStore_bucketForAddr(store->pub.selfAddress, &nn->address) == bucket) {
            struct Node_Two* newNode = nn;
            struct Node_Two* tempNode = 
# 1129 "dht/dhtcore/NodeStore.c" 3 4
                                       ((void *)0)
# 1129 "dht/dhtcore/NodeStore.c"
                                           ;
            for (uint32_t i = 0 ; i < count ; i++) {
                if (nodeList->size < i+1) {

                    nodeList->size = i+1;
                    nodeList->nodes[i] = newNode;
                    break;
                }
                if ( (newNode->marked && !nodeList->nodes[i]->marked) ||
                      whichIsWorse(nodeList->nodes[i], newNode, store) == nodeList->nodes[i] ) {





                    tempNode = nodeList->nodes[i];
                    nodeList->nodes[i] = newNode;
                    newNode = tempNode;
                }
            }
        }
    }
    return nodeList;
}

static 
# 1154 "dht/dhtcore/NodeStore.c" 3 4
      _Bool 
# 1154 "dht/dhtcore/NodeStore.c"
           markNodesForBucket(struct NodeStore_pvt* store,
                               uint16_t bucket,
                               const uint32_t count)
{
    struct Allocator* nodeListAlloc = Allocator__child((store->alloc),"NodeStore.c",1158);
    struct NodeList* nodeList = NodeStore_getNodesForBucket(&store->pub,
                                                            nodeListAlloc,
                                                            bucket,
                                                            count);
    
# 1163 "dht/dhtcore/NodeStore.c" 3 4
   _Bool 
# 1163 "dht/dhtcore/NodeStore.c"
        retVal = 
# 1163 "dht/dhtcore/NodeStore.c" 3 4
                 0
# 1163 "dht/dhtcore/NodeStore.c"
                      ;
    if (nodeList->size > 0) { retVal = 
# 1164 "dht/dhtcore/NodeStore.c" 3 4
                                      1
# 1164 "dht/dhtcore/NodeStore.c"
                                          ; }

    for (uint32_t i = 0; i < nodeList->size; i++) {

        (__extension__ ({ __typeof__(nodeList->nodes[i]) Identity_ptr = (nodeList->nodes[i]); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x0254ff724c14aca9ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("NodeStore.c"), (1168), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x0254ff724c14aca9ull))"); } } while (0); Identity_ptr; }));
        nodeList->nodes[i]->marked = 1;
    }


    Allocator__free((nodeListAlloc),"NodeStore.c",1173);

    return retVal;
}

static void markKeyspaceNodes(struct NodeStore_pvt* store)
{
    for (uint16_t bucket = 0; bucket < 128 ; bucket++) {
        markNodesForBucket(store, bucket, 4);
    }
}
# 1193 "dht/dhtcore/NodeStore.c"
static struct Node_Two* getWorstNode(struct NodeStore_pvt* store)
{
    struct Node_Two* worst = 
# 1195 "dht/dhtcore/NodeStore.c" 3 4
                            ((void *)0)
# 1195 "dht/dhtcore/NodeStore.c"
                                ;
    struct Node_Two* nn = 
# 1196 "dht/dhtcore/NodeStore.c" 3 4
                         ((void *)0)
# 1196 "dht/dhtcore/NodeStore.c"
                             ;
    for ((nn) = NodeRBTree_RB_MINMAX(&store->nodeTree, -1); (nn) != 
# 1197 "dht/dhtcore/NodeStore.c" 3 4
   ((void *)0)
# 1197 "dht/dhtcore/NodeStore.c"
   ; (nn) = NodeRBTree_RB_NEXT(nn)) {

        struct Node_Link* parentLink = Node_getBestParent(nn);
        if (parentLink) {
            parentLink->parent->marked = 1;
        } else if (!worst || whichIsWorse(nn, worst, store) == nn) {

            worst = nn;
        }
    }
    if (worst) {
        for ((nn) = NodeRBTree_RB_MINMAX(&store->nodeTree, -1); (nn) != 
# 1208 "dht/dhtcore/NodeStore.c" 3 4
       ((void *)0)
# 1208 "dht/dhtcore/NodeStore.c"
       ; (nn) = NodeRBTree_RB_NEXT(nn)) {
            if (nn->marked) { nn->marked = 
# 1209 "dht/dhtcore/NodeStore.c" 3 4
                                          0
# 1209 "dht/dhtcore/NodeStore.c"
                                               ; }
        }
        return worst;
    }


    markKeyspaceNodes(store);

    for ((nn) = NodeRBTree_RB_MINMAX(&store->nodeTree, -1); (nn) != 
# 1217 "dht/dhtcore/NodeStore.c" 3 4
   ((void *)0)
# 1217 "dht/dhtcore/NodeStore.c"
   ; (nn) = NodeRBTree_RB_NEXT(nn)) {
        if (nn->marked) {
            nn->marked = 
# 1219 "dht/dhtcore/NodeStore.c" 3 4
                        0
# 1219 "dht/dhtcore/NodeStore.c"
                             ;
        } else if (!worst || whichIsWorse(nn, worst, store) == nn) {
            worst = nn;
        }
    }
    if (worst) { return worst; }

    for ((nn) = NodeRBTree_RB_MINMAX(&store->nodeTree, -1); (nn) != 
# 1226 "dht/dhtcore/NodeStore.c" 3 4
   ((void *)0)
# 1226 "dht/dhtcore/NodeStore.c"
   ; (nn) = NodeRBTree_RB_NEXT(nn)) {







        if (!worst || whichIsWorse(nn, worst, store) == nn) {
            worst = nn;
        }
    }


    do { if (!((worst))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("NodeStore.c"), (1240), "(worst)"); } } while (0);
    return worst;
}

static void destroyNode(struct Node_Two* node, struct NodeStore_pvt* store)
{

    uint8_t address_debug[60];
    if (1) {
        Address_print(address_debug, &node->address);
    }

    struct Node_Link* link;
    for ((link) = PeerRBTree_RB_MINMAX(&node->peerTree, -1); (link) != 
# 1253 "dht/dhtcore/NodeStore.c" 3 4
   ((void *)0)
# 1253 "dht/dhtcore/NodeStore.c"
   ; (link) = PeerRBTree_RB_NEXT(link)) {
        (__extension__ ({ __typeof__(link) Identity_ptr = (link); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x0254ff724c14aca9ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("NodeStore.c"), (1254), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x0254ff724c14aca9ull))"); } } while (0); Identity_ptr; }));
        NodeStore_unlinkNodes(&store->pub, link);
    }




    if (!1) {
        store->pub.linkedNodes--;
        setParentCostAndPath(node, 
# 1263 "dht/dhtcore/NodeStore.c" 3 4
                                  ((void *)0)
# 1263 "dht/dhtcore/NodeStore.c"
                                      , 
# 1263 "dht/dhtcore/NodeStore.c" 3 4
                                        0xffffffffffffffffULL
# 1263 "dht/dhtcore/NodeStore.c"
                                                  , 
# 1263 "dht/dhtcore/NodeStore.c" 3 4
                                                    0xffffffffffffffffULL
# 1263 "dht/dhtcore/NodeStore.c"
                                                              , store);
    }

    link = node->reversePeers;
    while (link) {
        struct Node_Link* nextLink = link->nextPeer;
        NodeStore_unlinkNodes(&store->pub, link);
        link = nextLink;
    }

    do { if (!((!Node_getBestParent(node)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("NodeStore.c"), (1273), "(!Node_getBestParent(node))"); } } while (0);

    do { if (!((node == NodeRBTree_RB_FIND(&store->nodeTree, node)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("NodeStore.c"), (1275), "(node == NodeRBTree_RB_FIND(&store->nodeTree, node))"); } } while (0);
    NodeRBTree_RB_REMOVE(&store->nodeTree, node);
    store->pub.nodeCount--;

    Allocator__free((node->alloc),"NodeStore.c",1279);
}






static uint32_t costAfterDecay(const uint32_t oldCost)
{


    int64_t newCost = oldCost - oldCost/8;
    if (newCost < 1024) {

        newCost = 1024;
    }
    return newCost;
}

static uint32_t costAfterTimeout(const uint64_t oldCost)
{
    int64_t newCost = oldCost;
    newCost *= 8;
    newCost /= 8 - 1;
    if (newCost > 
# 1304 "dht/dhtcore/NodeStore.c" 3 4
                 0xffffffffU
# 1304 "dht/dhtcore/NodeStore.c"
                           ) { newCost = 
# 1304 "dht/dhtcore/NodeStore.c" 3 4
                                         0xffffffffU
# 1304 "dht/dhtcore/NodeStore.c"
                                                   ; }
    return newCost;
}


static uint32_t calcNextCost(const uint64_t oldCost)
{

    uint64_t out = costAfterDecay(oldCost);

    do { if (!((out >= 1024 && out != 
# 1314 "dht/dhtcore/NodeStore.c" 3 4
   0xffffffffffffffffULL
# 1314 "dht/dhtcore/NodeStore.c"
   ))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("NodeStore.c"), (1314), "(out >= 1024 && out != 0xffffffffffffffffULL)"); } } while (0);
    return out;
}

static struct Node_Link* discoverNode(struct NodeStore_pvt* store,
                                      struct Address* addr,
                                      struct EncodingScheme* scheme,
                                      int inverseLinkEncodingFormNumber,
                                      uint64_t milliseconds)
{
    struct Node_Two* child = nodeForIp(store, addr->ip6.bytes);

    if (1) {
        uint8_t printedAddr[60];
        Address_print(printedAddr, addr);
        do { if (store->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(store->logger, Log_Level_DEBUG, "NodeStore.c", 1329, "Discover node [%s]", printedAddr); } } while (0);
    }

    if (child && child == store->selfLink->child) {
        return 
# 1333 "dht/dhtcore/NodeStore.c" 3 4
              ((void *)0)
# 1333 "dht/dhtcore/NodeStore.c"
                  ;
    }

    if (child && EncodingScheme_compare(child->encodingScheme, scheme)) {


        RumorMill__addNode((store->renumberMill), (&child->address), "NodeStore.c", 1339);
        if (addr->path > (child->address.path | (child->address.path << 3))) {
            do { if (store->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(store->logger, Log_Level_DEBUG, "NodeStore.c", 1341, "Node appears to have changed it's encoding scheme " "but the message came from far away and we will not trust it"); } } while (0)
                                                                                                   ;
            return 
# 1343 "dht/dhtcore/NodeStore.c" 3 4
                  ((void *)0)
# 1343 "dht/dhtcore/NodeStore.c"
                      ;
        } else {
            do { if (store->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(store->logger, Log_Level_DEBUG, "NodeStore.c", 1345, "Node appears to have changed it's encoding scheme " "dropping him from the table and re-inserting"); } } while (0)
                                                                                    ;
            destroyNode(child, store);
            child = 
# 1348 "dht/dhtcore/NodeStore.c" 3 4
                   ((void *)0)
# 1348 "dht/dhtcore/NodeStore.c"
                       ;
        }
    } else if (child && child->address.protocolVersion != addr->protocolVersion) {
        child->address.protocolVersion = addr->protocolVersion;
    }

    struct Allocator* alloc = 
# 1354 "dht/dhtcore/NodeStore.c" 3 4
                             ((void *)0)
# 1354 "dht/dhtcore/NodeStore.c"
                                 ;
    if (!child) {
        alloc = Allocator__child((store->alloc),"NodeStore.c",1356);
        child = Allocator__calloc((alloc),(sizeof(struct Node_Two)),(1),"NodeStore.c",1357);
        child->alloc = alloc;
        Bits__memcpy(&child->address,addr,sizeof(struct Address),"NodeStore.c",1359);
        child->encodingScheme = EncodingScheme_defineDynWidthScheme((scheme)->forms, (scheme)->count, (child->alloc));
        child->timeLastPinged = Time_currentTimeMilliseconds(store->eventBase);
        (child)->Identity_verifier = ((unsigned long) 0x0254ff724c14aca9ull);
    }

    struct Node_Link* link = 
# 1365 "dht/dhtcore/NodeStore.c" 3 4
                            ((void *)0)
# 1365 "dht/dhtcore/NodeStore.c"
                                ;
    for (;;) {
        link = discoverLink(store,
                            addr->path,
                            child,
                            inverseLinkEncodingFormNumber);

        if (link) { break; }
# 1382 "dht/dhtcore/NodeStore.c"
        uint64_t path = addr->path;
        struct Node_Link* lastLink = store->selfLink;
        do {
            struct Node_Link* nextLink = 
# 1385 "dht/dhtcore/NodeStore.c" 3 4
                                        ((void *)0)
# 1385 "dht/dhtcore/NodeStore.c"
                                            ;
            path = firstHopInPath(path, &nextLink, lastLink, store);
            lastLink = nextLink;
            if (path == (
# 1388 "dht/dhtcore/NodeStore.c" 3 4
                       0xffffffffffffffffULL
# 1388 "dht/dhtcore/NodeStore.c"
                       -1)) {

                lastLink = 
# 1390 "dht/dhtcore/NodeStore.c" 3 4
                          ((void *)0)
# 1390 "dht/dhtcore/NodeStore.c"
                              ;
                break;
            }
        } while (
# 1393 "dht/dhtcore/NodeStore.c" 3 4
                0xffffffffffffffffULL 
# 1393 "dht/dhtcore/NodeStore.c"
                                       != path);

        if (lastLink && LabelSplicer_routesThrough(addr->path, lastLink->child->address.path)) {

            do { if (!((lastLink != store->selfLink))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("NodeStore.c"), (1397), "(lastLink != store->selfLink)"); } } while (0);
            NodeStore_unlinkNodes(&store->pub, lastLink);
            continue;
        }

        if (alloc) {
            Allocator__free((alloc),"NodeStore.c",1403);
        }
        _verify(store, 
# 1405 "dht/dhtcore/NodeStore.c" 3 4
       1
# 1405 "dht/dhtcore/NodeStore.c"
       , "NodeStore.c", 1405);
        do { if (store->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(store->logger, Log_Level_DEBUG, "NodeStore.c", 1406, "Invalid path"); } } while (0);
        return 
# 1407 "dht/dhtcore/NodeStore.c" 3 4
              ((void *)0)
# 1407 "dht/dhtcore/NodeStore.c"
                  ;
    }

    do { if (!((link->child))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("NodeStore.c"), (1410), "(link->child)"); } } while (0);


        struct Node_Two* parent = link->parent;



    _verify(store, 
# 1417 "dht/dhtcore/NodeStore.c" 3 4
   1
# 1417 "dht/dhtcore/NodeStore.c"
   , "NodeStore.c", 1417);
    handleLinkNews(link, calcNextCost(link->linkCost), store);
    _verify(store, 
# 1419 "dht/dhtcore/NodeStore.c" 3 4
   1
# 1419 "dht/dhtcore/NodeStore.c"
   , "NodeStore.c", 1419);
    freePendingLinks(store);

    while ((store->pub.nodeCount - store->pub.peerCount) >
        store->pub.nodeCapacity
            || store->pub.linkCount > store->pub.linkCapacity)
    {
        struct Node_Two* worst = getWorstNode(store);
        if (1) {
            uint8_t worstAddr[60];
            Address_print(worstAddr, &worst->address);
            do { if (store->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(store->logger, Log_Level_DEBUG, "NodeStore.c", 1430, "store full, removing worst node: [%s] nodes [%d] links [%d]", worstAddr, store->pub.nodeCount, store->pub.linkCount); } } while (0)
                                                                            ;
        }

        do { if (!((!isPeer(worst, store)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("NodeStore.c"), (1434), "(!isPeer(worst, store))"); } } while (0);

        if (link && (worst == link->parent || worst == link->child)) { link = 
# 1436 "dht/dhtcore/NodeStore.c" 3 4
                                                                             ((void *)0)
# 1436 "dht/dhtcore/NodeStore.c"
                                                                                 ; }

        destroyNode(worst, store);
        freePendingLinks(store);
    }

    _verify(store, 
# 1442 "dht/dhtcore/NodeStore.c" 3 4
   1
# 1442 "dht/dhtcore/NodeStore.c"
   , "NodeStore.c", 1442);



    do { if (!((!link || PeerRBTree_RB_FIND(&parent->peerTree, link) == link))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("NodeStore.c"), (1446), "(!link || PeerRBTree_RB_FIND(&parent->peerTree, link) == link)"); } } while (0);

    return link;
}

struct Node_Link* NodeStore_discoverNode(struct NodeStore* nodeStore,
                                         struct Address* addr,
                                         struct EncodingScheme* scheme,
                                         int inverseLinkEncodingFormNumber,
                                         uint64_t milliseconds)
{
    struct NodeStore_pvt* store = (__extension__ ({ __typeof__((struct NodeStore_pvt*)nodeStore) Identity_ptr = ((struct NodeStore_pvt*)nodeStore); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x0254ff724c14aca9ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("NodeStore.c"), (1457), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x0254ff724c14aca9ull))"); } } while (0); Identity_ptr; }));
    store->disarmCheck++;
    struct Node_Link* out =
        discoverNode(store, addr, scheme, inverseLinkEncodingFormNumber, milliseconds);
    store->disarmCheck--;
    _verify(store, 
# 1462 "dht/dhtcore/NodeStore.c" 3 4
   1
# 1462 "dht/dhtcore/NodeStore.c"
   , "NodeStore.c", 1462);
    return out;
}

struct Node_Two* NodeStore_nodeForAddr(struct NodeStore* nodeStore, uint8_t addr[16])
{
    struct NodeStore_pvt* store = (__extension__ ({ __typeof__((struct NodeStore_pvt*)nodeStore) Identity_ptr = ((struct NodeStore_pvt*)nodeStore); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x0254ff724c14aca9ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("NodeStore.c"), (1468), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x0254ff724c14aca9ull))"); } } while (0); Identity_ptr; }));
    struct Node_Two* n = nodeForIp(store, addr);
    if (n && n->address.path == 
# 1470 "dht/dhtcore/NodeStore.c" 3 4
                               0xffffffffffffffffULL
# 1470 "dht/dhtcore/NodeStore.c"
                                         ) {
        if (1) {
            uint8_t addrStr[40];
            AddrTools_printIp(addrStr, n->address.ip6.bytes);
            do { if (store->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(store->logger, Log_Level_DEBUG, "NodeStore.c", 1474, "No way to represent path to [%s]", addrStr); } } while (0);
        }
        return 
# 1476 "dht/dhtcore/NodeStore.c" 3 4
              ((void *)0)
# 1476 "dht/dhtcore/NodeStore.c"
                  ;
    }
    return n;
}

struct Node_Two* NodeStore_closestNode(struct NodeStore* nodeStore, uint64_t path)
{
    struct NodeStore_pvt* store = (__extension__ ({ __typeof__((struct NodeStore_pvt*)nodeStore) Identity_ptr = ((struct NodeStore_pvt*)nodeStore); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x0254ff724c14aca9ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("NodeStore.c"), (1483), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x0254ff724c14aca9ull))"); } } while (0); Identity_ptr; }));
    struct Node_Link* out = 
# 1484 "dht/dhtcore/NodeStore.c" 3 4
                           ((void *)0)
# 1484 "dht/dhtcore/NodeStore.c"
                               ;
    findClosest(path, &out, store->selfLink, store);
    if (!out) { return 
# 1486 "dht/dhtcore/NodeStore.c" 3 4
                      ((void *)0)
# 1486 "dht/dhtcore/NodeStore.c"
                          ; }
    return (__extension__ ({ __typeof__(out->child) Identity_ptr = (out->child); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x0254ff724c14aca9ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("NodeStore.c"), (1487), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x0254ff724c14aca9ull))"); } } while (0); Identity_ptr; }));
}

struct Node_Link* NodeStore_linkForPath(struct NodeStore* nodeStore, uint64_t path)
{
    struct NodeStore_pvt* store = (__extension__ ({ __typeof__((struct NodeStore_pvt*)nodeStore) Identity_ptr = ((struct NodeStore_pvt*)nodeStore); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x0254ff724c14aca9ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("NodeStore.c"), (1492), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x0254ff724c14aca9ull))"); } } while (0); Identity_ptr; }));
    struct Node_Link* out = 
# 1493 "dht/dhtcore/NodeStore.c" 3 4
                           ((void *)0)
# 1493 "dht/dhtcore/NodeStore.c"
                               ;
    uint64_t pathParentChild = findClosest(path, &out, store->selfLink, store);
    if (pathParentChild != 1) { return 
# 1495 "dht/dhtcore/NodeStore.c" 3 4
                                      ((void *)0)
# 1495 "dht/dhtcore/NodeStore.c"
                                          ; }
    return (__extension__ ({ __typeof__(out) Identity_ptr = (out); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x0254ff724c14aca9ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("NodeStore.c"), (1496), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x0254ff724c14aca9ull))"); } } while (0); Identity_ptr; }));
}

struct Node_Link* NodeStore_firstHopInPath(struct NodeStore* nodeStore,
                                           uint64_t path,
                                           uint64_t* correctedPath,
                                           struct Node_Link* startingPoint)
{
    struct NodeStore_pvt* store = (__extension__ ({ __typeof__((struct NodeStore_pvt*)nodeStore) Identity_ptr = ((struct NodeStore_pvt*)nodeStore); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x0254ff724c14aca9ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("NodeStore.c"), (1504), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x0254ff724c14aca9ull))"); } } while (0); Identity_ptr; }));
    if (!startingPoint) { startingPoint = store->selfLink; }
    if (!Node_getBestParent(startingPoint->parent)) { return 
# 1506 "dht/dhtcore/NodeStore.c" 3 4
                                                            ((void *)0)
# 1506 "dht/dhtcore/NodeStore.c"
                                                                ; }
    struct Node_Link* out = 
# 1507 "dht/dhtcore/NodeStore.c" 3 4
                           ((void *)0)
# 1507 "dht/dhtcore/NodeStore.c"
                               ;
    path = firstHopInPath(path, &out, startingPoint, store);
    if ((path >= (
# 1509 "dht/dhtcore/NodeStore.c" 3 4
       0xffffffffffffffffULL
# 1509 "dht/dhtcore/NodeStore.c"
       -1))) { return 
# 1509 "dht/dhtcore/NodeStore.c" 3 4
                                          ((void *)0)
# 1509 "dht/dhtcore/NodeStore.c"
                                              ; }
    if (correctedPath) { *correctedPath = path; }
    return out;
}

char* NodeStore_getRouteLabel_strerror(uint64_t returnVal)
{
    switch (returnVal) {
        case ((~((uint64_t)0))-1):
            return "NodeStore_getRouteLabel_PARENT_NOT_FOUND";
        case ((~((uint64_t)0))-2):
            return "NodeStore_getRouteLabel_CHILD_NOT_FOUND";
        default: return 
# 1521 "dht/dhtcore/NodeStore.c" 3 4
                       ((void *)0)
# 1521 "dht/dhtcore/NodeStore.c"
                           ;
    }
}

uint64_t NodeStore_getRouteLabel(struct NodeStore* nodeStore,
                                 uint64_t pathToParent,
                                 uint64_t pathParentToChild)
{
    struct NodeStore_pvt* store = (__extension__ ({ __typeof__((struct NodeStore_pvt*)nodeStore) Identity_ptr = ((struct NodeStore_pvt*)nodeStore); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x0254ff724c14aca9ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("NodeStore.c"), (1529), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x0254ff724c14aca9ull))"); } } while (0); Identity_ptr; }));
    struct Node_Link* linkToParent;
    if (findClosest(pathToParent, &linkToParent, store->selfLink, store) != 1) {
        return ((~((uint64_t)0))-1);
    }

    struct Node_Link* linkToChild = 
# 1535 "dht/dhtcore/NodeStore.c" 3 4
                                   ((void *)0)
# 1535 "dht/dhtcore/NodeStore.c"
                                       ;
    for ((linkToChild) = PeerRBTree_RB_MINMAX(&linkToParent->child->peerTree, 1); (linkToChild) != 
# 1536 "dht/dhtcore/NodeStore.c" 3 4
   ((void *)0)
# 1536 "dht/dhtcore/NodeStore.c"
   ; (linkToChild) = PeerRBTree_RB_PREV(linkToChild)) {
        if (pathParentToChild == linkToChild->cannonicalLabel) {
            if (linkToParent == store->selfLink) {
                return linkToChild->cannonicalLabel;
            }

            return extendRoute(pathToParent,
                               linkToChild->parent->encodingScheme,
                               linkToChild->cannonicalLabel,
                               linkToParent->inverseLinkEncodingFormNumber);
        }
    }
    return ((~((uint64_t)0))-2);
}

uint64_t NodeStore_optimizePath(struct NodeStore* nodeStore, uint64_t path)
{
    struct NodeStore_pvt* store = (__extension__ ({ __typeof__((struct NodeStore_pvt*)nodeStore) Identity_ptr = ((struct NodeStore_pvt*)nodeStore); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x0254ff724c14aca9ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("NodeStore.c"), (1553), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x0254ff724c14aca9ull))"); } } while (0); Identity_ptr; }));
    struct Node_Link* linkToParent;
    uint64_t next = findClosest(path, &linkToParent, store->selfLink, store);
    if (next == (~((uint64_t)0))) {
        return (~((uint64_t)0));
    }
    if (EncodingScheme_isSelfRoute(linkToParent->child->encodingScheme, next)) {



        next = 1;
    }
    if (linkToParent == store->selfLink) {
        if (next == 1) { return 1; }
        return path;
    }

    if (next == 1) { return linkToParent->child->address.path; }

    struct Node_Link* childBestParent = Node_getBestParent(linkToParent->child);
    if (childBestParent) {
        linkToParent = childBestParent;
    }

    uint64_t optimized = extendRoute(linkToParent->child->address.path,
                                     linkToParent->child->encodingScheme,
                                     next,
                                     linkToParent->inverseLinkEncodingFormNumber);

    if (optimized != (((uint64_t)~0)-1) && optimized != (((uint64_t)~0))) {
        return optimized;
    }

    if (1) {
        do {
            uint8_t pathStr[20];
            uint8_t nextStr[20];
            uint8_t bestPathStr[20];
            AddrTools_printPath(pathStr, path);
            AddrTools_printPath(nextStr, next);
            AddrTools_printPath(bestPathStr, linkToParent->child->address.path);
            do { if (store->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(store->logger, Log_Level_DEBUG, "NodeStore.c", 1594, "Failed to optimize path [%s] with closest known [%s] and " "best path to closest known [%s]", pathStr, nextStr, bestPathStr); } } while (0)

                                                                   ;
        } while (0);
    }

    return path;
}

struct Node_Link* NodeStore_nextLink(struct Node_Two* parent, struct Node_Link* startLink)
{
    if (!startLink) {
        return PeerRBTree_RB_MINMAX(&parent->peerTree, -1);
    }
    return PeerRBTree_RB_NEXT(startLink);
}


struct NodeStore* NodeStore_new(struct Address* myAddress,
                                struct Allocator* allocator,
                                struct EventBase* eventBase,
                                struct Log* logger,
                                struct RumorMill* renumberMill)
{
    struct Allocator* alloc = Allocator__child((allocator),"NodeStore.c",1618);

    struct NodeStore_pvt* out = Allocator__clone((alloc),((&(struct NodeStore_pvt) { .pub = { .nodeCapacity = 128, .linkCapacity = 4096 }, .renumberMill = renumberMill, .logger = logger, .eventBase = eventBase, .alloc = alloc })),sizeof(*((&(struct NodeStore_pvt) { .pub = { .nodeCapacity = 128, .linkCapacity = 4096 }, .renumberMill = renumberMill, .logger = logger, .eventBase = eventBase, .alloc = alloc }))),"NodeStore.c",1620)
# 1629 "dht/dhtcore/NodeStore.c"
       ;
    (out)->Identity_verifier = ((unsigned long) 0x0254ff724c14aca9ull);


    struct Node_Two* selfNode = Allocator__calloc((alloc),(sizeof(struct Node_Two)),(1),"NodeStore.c",1633);
    do { if (!((selfNode))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("NodeStore.c"), (1634), "(selfNode)"); } } while (0);
    do { if (!((myAddress))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("NodeStore.c"), (1635), "(myAddress)"); } } while (0);
    Bits__memcpy(&selfNode->address,myAddress,sizeof(struct Address),"NodeStore.c",1636);
    selfNode->encodingScheme = NumberCompress_v3x5x8_defineScheme(alloc);
    selfNode->alloc = alloc;
    (selfNode)->Identity_verifier = ((unsigned long) 0x0254ff724c14aca9ull);
    out->pub.selfNode = selfNode;
    linkNodes(selfNode, selfNode, 1, 0, 0, 1, out);
    selfNode->timeLastPinged = Time_currentTimeMilliseconds(out->eventBase);

    out->pub.selfAddress = &out->selfLink->child->address;
    out->pub.selfAddress->protocolVersion = 20;

    return &out->pub;
}
# 1659 "dht/dhtcore/NodeStore.c"
struct Node_Two* NodeStore_dumpTable(struct NodeStore* nodeStore, uint32_t index)
{
    struct NodeStore_pvt* store = (__extension__ ({ __typeof__((struct NodeStore_pvt*)nodeStore) Identity_ptr = ((struct NodeStore_pvt*)nodeStore); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x0254ff724c14aca9ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("NodeStore.c"), (1661), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x0254ff724c14aca9ull))"); } } while (0); Identity_ptr; }));

    uint32_t i = 0;
    struct Node_Two* nn = 
# 1664 "dht/dhtcore/NodeStore.c" 3 4
                         ((void *)0)
# 1664 "dht/dhtcore/NodeStore.c"
                             ;
    for ((nn) = NodeRBTree_RB_MINMAX(&store->nodeTree, -1); (nn) != 
# 1665 "dht/dhtcore/NodeStore.c" 3 4
   ((void *)0)
# 1665 "dht/dhtcore/NodeStore.c"
   ; (nn) = NodeRBTree_RB_NEXT(nn)) {
        if (i++ == index) { return nn; }
    }
    return 
# 1668 "dht/dhtcore/NodeStore.c" 3 4
          ((void *)0)
# 1668 "dht/dhtcore/NodeStore.c"
              ;
}

struct Node_Link* NodeStore_getNextLink(struct NodeStore* nodeStore, struct Node_Link* last)
{
    struct NodeStore_pvt* store = (__extension__ ({ __typeof__((struct NodeStore_pvt*)nodeStore) Identity_ptr = ((struct NodeStore_pvt*)nodeStore); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x0254ff724c14aca9ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("NodeStore.c"), (1673), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x0254ff724c14aca9ull))"); } } while (0); Identity_ptr; }));
    struct Node_Two* nn;
    struct Node_Link* next;

    if (!last) {
        nn = (__extension__ ({ __typeof__(NodeRBTree_RB_MINMAX(&store->nodeTree, -1)) Identity_ptr = (NodeRBTree_RB_MINMAX(&store->nodeTree, -1)); do { if (!((!Identity_ptr || Identity_ptr->Identity_verifier == ((unsigned long) 0x0254ff724c14aca9ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("NodeStore.c"), (1678), "(!Identity_ptr || Identity_ptr->Identity_verifier == ((unsigned long) 0x0254ff724c14aca9ull))"); } } while (0); Identity_ptr; }));
        next = 
# 1679 "dht/dhtcore/NodeStore.c" 3 4
              ((void *)0)
# 1679 "dht/dhtcore/NodeStore.c"
                  ;
    } else {
        next = (__extension__ ({ __typeof__(PeerRBTree_RB_NEXT(last)) Identity_ptr = (PeerRBTree_RB_NEXT(last)); do { if (!((!Identity_ptr || Identity_ptr->Identity_verifier == ((unsigned long) 0x0254ff724c14aca9ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("NodeStore.c"), (1681), "(!Identity_ptr || Identity_ptr->Identity_verifier == ((unsigned long) 0x0254ff724c14aca9ull))"); } } while (0); Identity_ptr; }));
        if (next) { return next; }
        nn = (__extension__ ({ __typeof__(NodeRBTree_RB_NEXT(last->parent)) Identity_ptr = (NodeRBTree_RB_NEXT(last->parent)); do { if (!((!Identity_ptr || Identity_ptr->Identity_verifier == ((unsigned long) 0x0254ff724c14aca9ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("NodeStore.c"), (1683), "(!Identity_ptr || Identity_ptr->Identity_verifier == ((unsigned long) 0x0254ff724c14aca9ull))"); } } while (0); Identity_ptr; }));
    }

    while (!next) {
        if (!nn) { return 
# 1687 "dht/dhtcore/NodeStore.c" 3 4
                         ((void *)0)
# 1687 "dht/dhtcore/NodeStore.c"
                             ; }
        next = (__extension__ ({ __typeof__(PeerRBTree_RB_MINMAX(&nn->peerTree, -1)) Identity_ptr = (PeerRBTree_RB_MINMAX(&nn->peerTree, -1)); do { if (!((!Identity_ptr || Identity_ptr->Identity_verifier == ((unsigned long) 0x0254ff724c14aca9ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("NodeStore.c"), (1688), "(!Identity_ptr || Identity_ptr->Identity_verifier == ((unsigned long) 0x0254ff724c14aca9ull))"); } } while (0); Identity_ptr; }));
        nn = (__extension__ ({ __typeof__(NodeRBTree_RB_NEXT(nn)) Identity_ptr = (NodeRBTree_RB_NEXT(nn)); do { if (!((!Identity_ptr || Identity_ptr->Identity_verifier == ((unsigned long) 0x0254ff724c14aca9ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("NodeStore.c"), (1689), "(!Identity_ptr || Identity_ptr->Identity_verifier == ((unsigned long) 0x0254ff724c14aca9ull))"); } } while (0); Identity_ptr; }));
    }
    return next;
}

struct Node_Two* NodeStore_getNextNode(struct NodeStore* nodeStore, struct Node_Two* lastNode)
{
    struct NodeStore_pvt* store = (__extension__ ({ __typeof__((struct NodeStore_pvt*)nodeStore) Identity_ptr = ((struct NodeStore_pvt*)nodeStore); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x0254ff724c14aca9ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("NodeStore.c"), (1696), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x0254ff724c14aca9ull))"); } } while (0); Identity_ptr; }));
    if (!lastNode) {
        return (__extension__ ({ __typeof__(NodeRBTree_RB_MINMAX(&store->nodeTree, -1)) Identity_ptr = (NodeRBTree_RB_MINMAX(&store->nodeTree, -1)); do { if (!((!Identity_ptr || Identity_ptr->Identity_verifier == ((unsigned long) 0x0254ff724c14aca9ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("NodeStore.c"), (1698), "(!Identity_ptr || Identity_ptr->Identity_verifier == ((unsigned long) 0x0254ff724c14aca9ull))"); } } while (0); Identity_ptr; }));
    }
    return (__extension__ ({ __typeof__(NodeRBTree_RB_NEXT(lastNode)) Identity_ptr = (NodeRBTree_RB_NEXT(lastNode)); do { if (!((!Identity_ptr || Identity_ptr->Identity_verifier == ((unsigned long) 0x0254ff724c14aca9ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("NodeStore.c"), (1700), "(!Identity_ptr || Identity_ptr->Identity_verifier == ((unsigned long) 0x0254ff724c14aca9ull))"); } } while (0); Identity_ptr; }));
}

static struct Node_Two* getBestCycleB(struct Node_Two* node,
                                      uint8_t target[16],
                                      struct NodeStore_pvt* store)
{
    uint32_t targetPfx = Address_prefixForIp6(target);
    uint32_t ourDistance = Address_getPrefix(store->pub.selfAddress) ^ targetPfx;
    struct Node_Link* next = 
# 1709 "dht/dhtcore/NodeStore.c" 3 4
                            ((void *)0)
# 1709 "dht/dhtcore/NodeStore.c"
                                ;
    for ((next) = PeerRBTree_RB_MINMAX(&node->peerTree, 1); (next) != 
# 1710 "dht/dhtcore/NodeStore.c" 3 4
   ((void *)0)
# 1710 "dht/dhtcore/NodeStore.c"
   ; (next) = PeerRBTree_RB_PREV(next)) {
        if (Node_getBestParent(next->child) != next || next == store->selfLink) { continue; }
        if (next->child->address.path == 
# 1712 "dht/dhtcore/NodeStore.c" 3 4
                                        0xffffffffffffffffULL
# 1712 "dht/dhtcore/NodeStore.c"
                                                  ) { continue; }
        if ((Address_getPrefix(&next->child->address) ^ targetPfx) >= ourDistance) { continue; }
        return next->child;
    }

    return 
# 1717 "dht/dhtcore/NodeStore.c" 3 4
          ((void *)0)
# 1717 "dht/dhtcore/NodeStore.c"
              ;
}

static int getBestCycle(struct Node_Two* node,
                        uint8_t target[16],
                        struct Node_Two** output,
                        int limit,
                        int cycle,
                        struct NodeStore_pvt* store)
{
    do { if (!((cycle < 1000))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("NodeStore.c"), (1727), "(cycle < 1000)"); } } while (0);
    if (cycle < limit) {
        int total = 0;
        struct Node_Link* next = 
# 1730 "dht/dhtcore/NodeStore.c" 3 4
                                ((void *)0)
# 1730 "dht/dhtcore/NodeStore.c"
                                    ;
        for ((next) = PeerRBTree_RB_MINMAX(&node->peerTree, 1); (next) != 
# 1731 "dht/dhtcore/NodeStore.c" 3 4
       ((void *)0)
# 1731 "dht/dhtcore/NodeStore.c"
       ; (next) = PeerRBTree_RB_PREV(next)) {
            if (*output) { return total; }
            if (Node_getBestParent(next->child) != next || next == store->selfLink) { continue; }
            total += getBestCycle(next->child, target, output, limit, cycle+1, store);
        }
        return total;
    }

    *output = getBestCycleB(node, target, store);
    return 1;
}

struct Node_Two* NodeStore_getBest(struct NodeStore* nodeStore, uint8_t targetAddress[16])
{
    struct NodeStore_pvt* store = (__extension__ ({ __typeof__((struct NodeStore_pvt*)nodeStore) Identity_ptr = ((struct NodeStore_pvt*)nodeStore); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x0254ff724c14aca9ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("NodeStore.c"), (1745), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x0254ff724c14aca9ull))"); } } while (0); Identity_ptr; }));


    struct Node_Two* n = NodeStore_nodeForAddr(nodeStore, targetAddress);
    if (n && Node_getBestParent(n)) { return n; }


    for (int i = 0; i < 10000; i++) {
        int ret = getBestCycle(store->pub.selfNode, targetAddress, &n, i, 0, store);
        if (n || !ret) {
            if (n) { do { if (!((Node_getBestParent(n)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("NodeStore.c"), (1755), "(Node_getBestParent(n))"); } } while (0); }
            return n;
        }
    }


    return 
# 1761 "dht/dhtcore/NodeStore.c" 3 4
          ((void *)0)
# 1761 "dht/dhtcore/NodeStore.c"
              ;
}

struct NodeList* NodeStore_getPeers(uint64_t label,
                                    const uint32_t max,
                                    struct Allocator* allocator,
                                    struct NodeStore* nodeStore)
{
    struct NodeStore_pvt* store = (__extension__ ({ __typeof__((struct NodeStore_pvt*)nodeStore) Identity_ptr = ((struct NodeStore_pvt*)nodeStore); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x0254ff724c14aca9ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("NodeStore.c"), (1769), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x0254ff724c14aca9ull))"); } } while (0); Identity_ptr; }));
    do { if (store->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(store->logger, Log_Level_DEBUG, "NodeStore.c", 1770, "getPeers request for [%llx]", (unsigned long long) label); } } while (0);


    if (label > 1) {
        int bitsUsed = NumberCompress_v3x5x8_bitsUsedForLabel(label);
        label = (label & Bits_maxBits64(bitsUsed)) | 1 << bitsUsed;
    }
    struct NodeList* out = Allocator__calloc((allocator),(sizeof(struct NodeList)),(1),"NodeStore.c",1777);
    out->nodes = Allocator__calloc((allocator),(sizeof(char*)),(max),"NodeStore.c",1778);

    struct Node_Link* next = 
# 1780 "dht/dhtcore/NodeStore.c" 3 4
                            ((void *)0)
# 1780 "dht/dhtcore/NodeStore.c"
                                ;
    for ((next) = PeerRBTree_RB_MINMAX(&store->pub.selfNode->peerTree, -1); (next) != 
# 1781 "dht/dhtcore/NodeStore.c" 3 4
   ((void *)0)
# 1781 "dht/dhtcore/NodeStore.c"
   ; (next) = PeerRBTree_RB_NEXT(next)) {
        uint64_t p = next->cannonicalLabel;
        if (!Node_isOneHopLink(next) && p != 1) { continue; }
        if (p == 
# 1784 "dht/dhtcore/NodeStore.c" 3 4
                0xffffffffffffffffULL
# 1784 "dht/dhtcore/NodeStore.c"
                          ) { continue; }
        if (p < label) { continue; }
        if (next->child->address.path != p) { continue; }
        int j;
        for (j = 0; j < (int)max; j++) {
            if (!out->nodes[j]) { continue; }
            if ((out->nodes[j]->address.path - label) > (p - label)) { continue; }
            break;
        }
        switch (j) {
            default: __builtin_memmove(out->nodes,&out->nodes[1],(j - 1) * sizeof(char*));
                __attribute__((fallthrough));;
            case 1: out->nodes[j - 1] = next->child;
                __attribute__((fallthrough));;
            case 0:;
        }
    }

    out->size = 0;
    for (int i = 0; i < (int)max; i++) {
        if (out->nodes[i]) {
            out->nodes = &out->nodes[i];
            out->size = max - i;
            break;
        }
    }

    for (int i = 0; i < (int)out->size; i++) {
        (__extension__ ({ __typeof__(out->nodes[i]) Identity_ptr = (out->nodes[i]); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x0254ff724c14aca9ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("NodeStore.c"), (1812), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x0254ff724c14aca9ull))"); } } while (0); Identity_ptr; }));
        _checkNode(out->nodes[i], store, "NodeStore.c", 1813);
        do { if (!((out->nodes[i]->address.path))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("NodeStore.c"), (1814), "(out->nodes[i]->address.path)"); } } while (0);
        do { if (!((out->nodes[i]->address.path < (((uint64_t)1)<<63)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("NodeStore.c"), (1815), "(out->nodes[i]->address.path < (((uint64_t)1)<<63))"); } } while (0);
        out->nodes[i] = Allocator__clone((allocator),(out->nodes[i]),sizeof(*(out->nodes[i])),"NodeStore.c",1816);
    }
    return out;
}

static 
# 1821 "dht/dhtcore/NodeStore.c" 3 4
      _Bool 
# 1821 "dht/dhtcore/NodeStore.c"
           isOkAnswer(struct Node_Two* node,
                       uint32_t compatVer,
                       struct NodeStore_pvt* store)
{
    if (node->address.path == 
# 1825 "dht/dhtcore/NodeStore.c" 3 4
                             0xffffffffffffffffULL
# 1825 "dht/dhtcore/NodeStore.c"
                                       ) {

        return 
# 1827 "dht/dhtcore/NodeStore.c" 3 4
              0
# 1827 "dht/dhtcore/NodeStore.c"
                   ;
    }
    if (!Version_isCompatible(compatVer, node->address.protocolVersion)) {
        return 
# 1830 "dht/dhtcore/NodeStore.c" 3 4
              0
# 1830 "dht/dhtcore/NodeStore.c"
                   ;
    }
    if (node == store->pub.selfNode) {
        return 
# 1833 "dht/dhtcore/NodeStore.c" 3 4
              0
# 1833 "dht/dhtcore/NodeStore.c"
                   ;
    }
    return 
# 1835 "dht/dhtcore/NodeStore.c" 3 4
          1
# 1835 "dht/dhtcore/NodeStore.c"
              ;
}


struct NodeList* NodeStore_getClosestNodes(struct NodeStore* nodeStore,
                                           struct Address* targetAddress,
                                           const uint32_t count,
                                           uint32_t compatVer,
                                           struct Allocator* allocator)
{
    struct NodeStore_pvt* store = (__extension__ ({ __typeof__((struct NodeStore_pvt*)nodeStore) Identity_ptr = ((struct NodeStore_pvt*)nodeStore); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x0254ff724c14aca9ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("NodeStore.c"), (1845), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x0254ff724c14aca9ull))"); } } while (0); Identity_ptr; }));

    struct NodeList* out = Allocator__malloc((allocator),(sizeof(struct NodeList)),"NodeStore.c",1847);
    out->nodes = Allocator__calloc((allocator),(count),(sizeof(char*)),"NodeStore.c",1848);
    out->size = count;

    struct Node_Two fakeNode = { .marked = 0 };
    Bits__memcpy(&fakeNode.address,targetAddress,sizeof(struct Address),"NodeStore.c",1852);

    struct Node_Two* next = (__extension__ ({ __typeof__(NodeRBTree_RB_NFIND(&store->nodeTree, &fakeNode)) Identity_ptr = (NodeRBTree_RB_NFIND(&store->nodeTree, &fakeNode)); do { if (!((!Identity_ptr || Identity_ptr->Identity_verifier == ((unsigned long) 0x0254ff724c14aca9ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("NodeStore.c"), (1854), "(!Identity_ptr || Identity_ptr->Identity_verifier == ((unsigned long) 0x0254ff724c14aca9ull))"); } } while (0); Identity_ptr; }));
    if (!next) {
        next = (__extension__ ({ __typeof__(NodeRBTree_RB_MINMAX(&store->nodeTree, 1)) Identity_ptr = (NodeRBTree_RB_MINMAX(&store->nodeTree, 1)); do { if (!((!Identity_ptr || Identity_ptr->Identity_verifier == ((unsigned long) 0x0254ff724c14aca9ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("NodeStore.c"), (1856), "(!Identity_ptr || Identity_ptr->Identity_verifier == ((unsigned long) 0x0254ff724c14aca9ull))"); } } while (0); Identity_ptr; }));
    }
    if (!next) {
        out->size = 0;
        return out;
    }

    struct Node_Two* prev = (__extension__ ({ __typeof__(NodeRBTree_RB_PREV(next)) Identity_ptr = (NodeRBTree_RB_PREV(next)); do { if (!((!Identity_ptr || Identity_ptr->Identity_verifier == ((unsigned long) 0x0254ff724c14aca9ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("NodeStore.c"), (1863), "(!Identity_ptr || Identity_ptr->Identity_verifier == ((unsigned long) 0x0254ff724c14aca9ull))"); } } while (0); Identity_ptr; }));
    int idx = out->size-1;

    while (idx > -1) {
        if (prev && (!next || Address_closest(targetAddress, &next->address, &prev->address) > 0)) {
            if (isOkAnswer(prev, compatVer, store)) { out->nodes[idx--] = prev; }
            prev = (__extension__ ({ __typeof__(NodeRBTree_RB_PREV(prev)) Identity_ptr = (NodeRBTree_RB_PREV(prev)); do { if (!((!Identity_ptr || Identity_ptr->Identity_verifier == ((unsigned long) 0x0254ff724c14aca9ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("NodeStore.c"), (1869), "(!Identity_ptr || Identity_ptr->Identity_verifier == ((unsigned long) 0x0254ff724c14aca9ull))"); } } while (0); Identity_ptr; }));
            continue;
        }
        if (next && (!prev || Address_closest(targetAddress, &next->address, &prev->address) < 0)) {
            if (isOkAnswer(next, compatVer, store)) { out->nodes[idx--] = next; }
            next = (__extension__ ({ __typeof__(NodeRBTree_RB_NEXT(next)) Identity_ptr = (NodeRBTree_RB_NEXT(next)); do { if (!((!Identity_ptr || Identity_ptr->Identity_verifier == ((unsigned long) 0x0254ff724c14aca9ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("NodeStore.c"), (1874), "(!Identity_ptr || Identity_ptr->Identity_verifier == ((unsigned long) 0x0254ff724c14aca9ull))"); } } while (0); Identity_ptr; }));
            continue;
        }
        break;
    }

    out->nodes = &out->nodes[idx+1];
    out->size -= idx+1;

    for (int i = 0; i < (int)out->size; i++) {
        (__extension__ ({ __typeof__(out->nodes[i]) Identity_ptr = (out->nodes[i]); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x0254ff724c14aca9ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("NodeStore.c"), (1884), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x0254ff724c14aca9ull))"); } } while (0); Identity_ptr; }));
        do { if (!((out->nodes[i]->address.path))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("NodeStore.c"), (1885), "(out->nodes[i]->address.path)"); } } while (0);
        do { if (!((out->nodes[i]->address.path < (((uint64_t)1)<<63)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("NodeStore.c"), (1886), "(out->nodes[i]->address.path < (((uint64_t)1)<<63))"); } } while (0);
        out->nodes[i] = Allocator__clone((allocator),(out->nodes[i]),sizeof(*(out->nodes[i])),"NodeStore.c",1887);
    }
    return out;
}

void NodeStore_disconnectedPeer(struct NodeStore* nodeStore, uint64_t path)
{
    struct NodeStore_pvt* store = (__extension__ ({ __typeof__((struct NodeStore_pvt*)nodeStore) Identity_ptr = ((struct NodeStore_pvt*)nodeStore); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x0254ff724c14aca9ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("NodeStore.c"), (1894), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x0254ff724c14aca9ull))"); } } while (0); Identity_ptr; }));
    struct Node_Link* nl = NodeStore_linkForPath(nodeStore, path);
    if (!nl) { return; }
    if (1) {
        uint8_t pathStr[20];
        AddrTools_printPath(pathStr, path);
        do { if (store->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(store->logger, Log_Level_DEBUG, "NodeStore.c", 1900, "NodeStore_disconnectedPeer(%s)", pathStr); } } while (0);
    }
    NodeStore_unlinkNodes(&store->pub, nl);
}

static void brokenLink(struct NodeStore_pvt* store, struct Node_Link* brokenLink)
{
    NodeStore_unlinkNodes(&store->pub, brokenLink);
}

static void addLinkToMill(struct NodeStore_pvt* store, struct Node_Link* link)
{
    struct Address addr;
    Bits__memcpy(&addr,&link->child->address,sizeof(struct Address),"NodeStore.c",1913);
    addr.path =
        NodeStore_getRouteLabel(&store->pub, link->parent->address.path, link->cannonicalLabel);
    do { if (!((!NodeStore_getRouteLabel_ERR(addr.path)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("NodeStore.c"), (1916), "(!NodeStore_getRouteLabel_ERR(addr.path))"); } } while (0);
    RumorMill__addNode((store->renumberMill), (&addr), "NodeStore.c", 1917);
}

void NodeStore_brokenLink(struct NodeStore* nodeStore, uint64_t path, uint64_t pathAtErrorHop)
{
    struct NodeStore_pvt* store = (__extension__ ({ __typeof__((struct NodeStore_pvt*)nodeStore) Identity_ptr = ((struct NodeStore_pvt*)nodeStore); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x0254ff724c14aca9ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("NodeStore.c"), (1922), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x0254ff724c14aca9ull))"); } } while (0); Identity_ptr; }));
    if (1) {
        uint8_t pathStr[20];
        uint8_t pathAtErrorHopStr[20];
        AddrTools_printPath(pathStr, path);
        AddrTools_printPath(pathAtErrorHopStr, pathAtErrorHop);
        do { if (store->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(store->logger, Log_Level_DEBUG, "NodeStore.c", 1928, "NodeStore_brokenLink(%s, %s)", pathStr, pathAtErrorHopStr); } } while (0);
    }

    struct Node_Link* link = store->selfLink;
    uint64_t thisPath = path;
    for (;;) {
        uint64_t nextPath = firstHopInPath(thisPath, &link, link, store);
        uint64_t mask = (((uint64_t)1) << (Bits_log2x64(thisPath) + 1)) - 1;

        if (1) {
            uint8_t maskStr[20];
            uint8_t pathStr[20];
            AddrTools_printPath(pathStr, nextPath);
            AddrTools_printPath(maskStr, mask);
            do { if (store->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(store->logger, Log_Level_DEBUG, "NodeStore.c", 1942, "NodeStore_brokenLink() nextPath = [%s] mask = [%s]", pathStr, maskStr); } } while (0)
                                       ;
        }

        uint64_t cannonicalPath =
            NodeStore_getRouteLabel(&store->pub, link->parent->address.path, link->cannonicalLabel);
        do { if (!((!NodeStore_getRouteLabel_ERR(cannonicalPath) || cannonicalPath == 
# 1948 "dht/dhtcore/NodeStore.c" 3 4
       0xffffffffffffffffULL 
# 1948 "dht/dhtcore/NodeStore.c"
       || link->parent->address.path == 
# 1948 "dht/dhtcore/NodeStore.c" 3 4
       0xffffffffffffffffULL
# 1948 "dht/dhtcore/NodeStore.c"
       ))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("NodeStore.c"), (1948), "(!NodeStore_getRouteLabel_ERR(cannonicalPath) || cannonicalPath == 0xffffffffffffffffULL || link->parent->address.path == 0xffffffffffffffffULL)"); } } while (0)

                                                     ;

        if ((pathAtErrorHop & mask) >= nextPath) {
            uint64_t cannPathAtErrorHop =
                EncodingScheme_convertLabel(link->child->encodingScheme,
                                            (pathAtErrorHop & mask),
                                            (-5000));

            uint8_t cannPathAtErrorHopStr[20];
            AddrTools_printPath(cannPathAtErrorHopStr, cannPathAtErrorHop);
            do { if (store->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(store->logger, Log_Level_DEBUG, "NodeStore.c", 1960, "NodeStore_brokenLink() converted pathAtErrorHop to [%s]", cannPathAtErrorHopStr); } } while (0)
                                            ;

            if (cannPathAtErrorHop == 
# 1963 "dht/dhtcore/NodeStore.c" 3 4
                                     0xffffffffffffffffULL
# 1963 "dht/dhtcore/NodeStore.c"
                                               ) {

            } else if ((cannPathAtErrorHop & mask) != thisPath) {

            } else if (path != cannonicalPath && !NodeStore_getRouteLabel_ERR(cannonicalPath)) {
                logLink(store, link, "NodeStore_brokenLink() not cannonucal, sending ping");
                addLinkToMill(store, link);
                return;
            } else {
                logLink(store, link, "NodeStore_brokenLink() removing");
                brokenLink(store, link);
                return;
            }
        } else if ((
# 1976 "dht/dhtcore/NodeStore.c" 3 4
                  0xffffffffffffffffULL
# 1976 "dht/dhtcore/NodeStore.c"
                  -1) == nextPath && thisPath == 1) {
            do { if (!((NodeStore_linkForPath(nodeStore, path) == link))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("NodeStore.c"), (1977), "(NodeStore_linkForPath(nodeStore, path) == link)"); } } while (0);
            if (path >> 56) {
                logLink(store, link, "NodeStore_brokenLink() probably caused by long path");
            } else if (path != cannonicalPath && !NodeStore_getRouteLabel_ERR(cannonicalPath)) {
                logLink(store, link, "NodeStore_brokenLink() not cannonical, sending ping (1link)");
                addLinkToMill(store, link);
                return;
            } else {
                logLink(store, link, "NodeStore_brokenLink() removing (1link)");
                brokenLink(store, link);
            }
            return;
        }

        if ((
# 1991 "dht/dhtcore/NodeStore.c" 3 4
           0xffffffffffffffffULL
# 1991 "dht/dhtcore/NodeStore.c"
           -1) == nextPath) {
            do { if (store->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(store->logger, Log_Level_DEBUG, "NodeStore.c", 1992, "NodeStore_brokenLink() firstHopInPath_NO_NEXT_LINK"); } } while (0);


            do { } while (0);
            return;
        }

        if (
# 1999 "dht/dhtcore/NodeStore.c" 3 4
           0xffffffffffffffffULL 
# 1999 "dht/dhtcore/NodeStore.c"
                                  == nextPath) {
            do { if (store->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(store->logger, Log_Level_DEBUG, "NodeStore.c", 2000, "NodeStore_brokenLink() firstHopInPath_INVALID"); } } while (0);
            return;
        }

        do { if (!((link))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("NodeStore.c"), (2004), "(link)"); } } while (0);
        thisPath = nextPath;
    }
}


static void updatePathCost(struct NodeStore_pvt* store, const uint64_t path, uint64_t newCost)
{
    struct Node_Link* link = store->selfLink;
    uint64_t pathFrag = path;
    uint64_t now = Time_currentTimeMilliseconds(store->eventBase);
    for (;;) {
        struct Node_Link* nextLink = 
# 2016 "dht/dhtcore/NodeStore.c" 3 4
                                    ((void *)0)
# 2016 "dht/dhtcore/NodeStore.c"
                                        ;
        uint64_t nextPath = firstHopInPath(pathFrag, &nextLink, link, store);
        if ((nextPath >= (
# 2018 "dht/dhtcore/NodeStore.c" 3 4
           0xffffffffffffffffULL
# 2018 "dht/dhtcore/NodeStore.c"
           -1))) {
            break;
        }


        do { if (!((nextLink->parent == link->child))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("NodeStore.c"), (2023), "(nextLink->parent == link->child)"); } } while (0);


        int64_t newLinkCost = calcNextCost(nextLink->linkCost);
        _verify(store, 
# 2027 "dht/dhtcore/NodeStore.c" 3 4
       1
# 2027 "dht/dhtcore/NodeStore.c"
       , "NodeStore.c", 2027);
        handleLinkNews(nextLink, newLinkCost, store);
        _verify(store, 
# 2029 "dht/dhtcore/NodeStore.c" 3 4
       1
# 2029 "dht/dhtcore/NodeStore.c"
       , "NodeStore.c", 2029);

        nextLink->timeLastSeen = now;

        pathFrag = nextPath;
        link = nextLink;
        newCost++;
    }

    link->child->timeLastPinged = now;
}

void NodeStore_pathResponse(struct NodeStore* nodeStore, uint64_t path, uint64_t milliseconds)
{
    struct NodeStore_pvt* store = (__extension__ ({ __typeof__((struct NodeStore_pvt*)nodeStore) Identity_ptr = ((struct NodeStore_pvt*)nodeStore); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x0254ff724c14aca9ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("NodeStore.c"), (2043), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x0254ff724c14aca9ull))"); } } while (0); Identity_ptr; }));
    struct Node_Link* link = NodeStore_linkForPath(nodeStore, path);
    if (!link || link == store->selfLink) { return; }
    struct Node_Two* node = link->child;
    uint64_t newCost;
    if (node->address.path == path) {

        newCost = calcNextCost(Node_getCost(node));
    }
    else {




        newCost = calcNextCost(
# 2057 "dht/dhtcore/NodeStore.c" 3 4
                              0xffffffffffffffffULL
# 2057 "dht/dhtcore/NodeStore.c"
                                        );
    }
    store->disarmCheck++;
    updatePathCost(store, path, newCost);
    store->disarmCheck--;
    _verify(store, 
# 2062 "dht/dhtcore/NodeStore.c" 3 4
   1
# 2062 "dht/dhtcore/NodeStore.c"
   , "NodeStore.c", 2062);
}

void NodeStore_pathTimeout(struct NodeStore* nodeStore, uint64_t path)
{
    struct NodeStore_pvt* store = (__extension__ ({ __typeof__((struct NodeStore_pvt*)nodeStore) Identity_ptr = ((struct NodeStore_pvt*)nodeStore); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x0254ff724c14aca9ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("NodeStore.c"), (2067), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x0254ff724c14aca9ull))"); } } while (0); Identity_ptr; }));

    struct Node_Link* link = NodeStore_linkForPath(nodeStore, path);
    if (!link) { return; }
    struct Node_Two* node = link->child;
# 2082 "dht/dhtcore/NodeStore.c"
    RumorMill__addNode((store->renumberMill), (&node->address), "NodeStore.c", 2082);

    if (!link || link->child->address.path != path) { return; }

    if (link->parent != store->pub.selfNode) {




        RumorMill__addNode((store->renumberMill), (&link->parent->address), "NodeStore.c", 2091);
    }

    uint64_t oldCost = Node_getCost(node);
    int64_t newLinkCost = costAfterTimeout(link->linkCost);
    _verify(store, 
# 2096 "dht/dhtcore/NodeStore.c" 3 4
   1
# 2096 "dht/dhtcore/NodeStore.c"
   , "NodeStore.c", 2096);
    handleLinkNews(link, newLinkCost, store);
    _verify(store, 
# 2098 "dht/dhtcore/NodeStore.c" 3 4
   1
# 2098 "dht/dhtcore/NodeStore.c"
   , "NodeStore.c", 2098);
    if (1) {
        uint8_t addr[60];
        Address_print(addr, &node->address);
        do { if (store->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(store->logger, Log_Level_DEBUG, "NodeStore.c", 2102, "Ping timeout for %s. changing cost from %llu to %llu\n", addr, (unsigned long long)oldCost, (unsigned long long)Node_getCost(node)); } } while (0)



                                                         ;
    }
}

struct Address NodeStore_addrForBucket(struct Address* source, uint16_t bucket)
{
    void Assert_compileTime(char x[1 - (!(128 == 128))]);
    struct Address addr = *source;
    uint64_t* addrPart = (bucket < 64) ? &addr.ip6.longs.one_be : &addr.ip6.longs.two_be;
    uint64_t bitmask = (uint64_t)1 << (63 - (bucket % 64));
    *addrPart ^= ((uint64_t)__builtin_bswap64(((uint64_t)(bitmask))));
    do { if (!((bucket == NodeStore_bucketForAddr(source, &addr)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("NodeStore.c"), (2117), "(bucket == NodeStore_bucketForAddr(source, &addr))"); } } while (0);
    return addr;
}

uint16_t NodeStore_bucketForAddr(struct Address* source, struct Address* dest)
{
    void Assert_compileTime(char x[1 - (!(128 == 128))]);
    uint16_t bucket = 0;
    uint64_t addrPart = source->ip6.longs.one_be ^ dest->ip6.longs.one_be;
    if (!addrPart) {
        addrPart = source->ip6.longs.two_be ^ dest->ip6.longs.two_be;
        bucket += 64;
    }
    addrPart = ((uint64_t)__builtin_bswap64(((uint64_t)(addrPart))));
    bucket += 63 - Bits_log2x64(addrPart);
    return bucket;
}

uint64_t NodeStore_timeSinceLastPing(struct NodeStore* nodeStore, struct Node_Two* node)
{
    struct NodeStore_pvt* store = (__extension__ ({ __typeof__((struct NodeStore_pvt*)nodeStore) Identity_ptr = ((struct NodeStore_pvt*)nodeStore); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x0254ff724c14aca9ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("NodeStore.c"), (2137), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x0254ff724c14aca9ull))"); } } while (0); Identity_ptr; }));
    uint64_t now = Time_currentTimeMilliseconds(store->eventBase);
    uint64_t lastSeen = node->timeLastPinged;
    struct Node_Link* link = Node_getBestParent(node);
    while (link && link != store->selfLink) {
        lastSeen = (link->timeLastSeen < lastSeen) ? link->timeLastSeen : lastSeen;
        link = Node_getBestParent(link->parent);
    }
    return now - lastSeen;
}


# 2148 "dht/dhtcore/NodeStore.c" 3 4
_Bool 
# 2148 "dht/dhtcore/NodeStore.c"
    NodeStore_getFullVerify(struct NodeStore* nodeStore)
{
    struct NodeStore_pvt* store = (__extension__ ({ __typeof__((struct NodeStore_pvt*)nodeStore) Identity_ptr = ((struct NodeStore_pvt*)nodeStore); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x0254ff724c14aca9ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("NodeStore.c"), (2150), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x0254ff724c14aca9ull))"); } } while (0); Identity_ptr; }));
    return store->fullVerify != 0;
}

void NodeStore_setFullVerify(struct NodeStore* nodeStore, 
# 2154 "dht/dhtcore/NodeStore.c" 3 4
                                                         _Bool 
# 2154 "dht/dhtcore/NodeStore.c"
                                                              fullVerify)
{
    struct NodeStore_pvt* store = (__extension__ ({ __typeof__((struct NodeStore_pvt*)nodeStore) Identity_ptr = ((struct NodeStore_pvt*)nodeStore); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x0254ff724c14aca9ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("NodeStore.c"), (2156), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x0254ff724c14aca9ull))"); } } while (0); Identity_ptr; }));
    store->fullVerify = (fullVerify != 0);
}
