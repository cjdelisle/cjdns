# 1 "net/EventEmitter.c"
# 1 "/home/jojapoppa/cjdns//"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "net/EventEmitter.c"
# 15 "net/EventEmitter.c"
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
struct Linker_x03261851144928438;
# 33 "./util/Assert.h"
__attribute__((__format__ (__printf__, 1, 2)))
__attribute__((__noreturn__))
void Assert_failure(const char* format, ...);
# 19 "./util/Identity.h" 2
# 1 "./util/Constant.h" 1
# 33 "./util/Constant.h"

# 20 "./util/Identity.h" 2



    
# 19 "./memory/Allocator.h" 2


struct Linker_x06712664537722284;





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
# 16 "net/EventEmitter.c" 2
# 1 "./wire/PFChan.h" 1
# 19 "./wire/PFChan.h"
# 1 "./wire/RouteHeader.h" 1
# 19 "./wire/RouteHeader.h"
# 1 "./wire/SwitchHeader.h" 1
# 19 "./wire/SwitchHeader.h"
# 1 "./util/Endian.h" 1
# 18 "./util/Endian.h"
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
# 19 "./util/Endian.h" 2
# 20 "./wire/SwitchHeader.h" 2
# 1 "./util/version/Version.h" 1
# 19 "./util/version/Version.h"

# 19 "./util/version/Version.h"
struct Linker_x08849607064392022;
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


# 1 "/usr/lib/gcc/x86_64-w64-mingw32/9.2-win32/include/stdbool.h" 1 3 4
# 24 "./wire/SwitchHeader.h" 2
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

       
# 17 "net/EventEmitter.c" 2
# 1 "./net/EventEmitter.h" 1
# 18 "./net/EventEmitter.h"
# 1 "./interface/Iface.h" 1
# 20 "./interface/Iface.h"
# 1 "./wire/Message.h" 1
# 18 "./wire/Message.h"
# 1 "./exception/Er.h" 1
# 23 "./exception/Er.h"
struct Linker_x000604053713116337;

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
# 19 "./wire/Message.h" 2

# 1 "./util/Bits.h" 1
# 21 "./util/Bits.h"
struct Linker_x05431316589459172;


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


struct Linker_x05448619136940565;



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
# 19 "./net/EventEmitter.h" 2


# 1 "./util/log/Log.h" 1
# 20 "./util/log/Log.h"
struct Linker_x037879200231604715;

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
# 22 "./net/EventEmitter.h" 2

struct Linker_x07501174597989813;

struct EventEmitter
{
    int unused;
};






void EventEmitter_regCore(struct EventEmitter* ee,
                          struct Iface* iface,
                          enum PFChan_Pathfinder ev);

void EventEmitter_regPathfinderIface(struct EventEmitter* ee, struct Iface* iface);

struct EventEmitter* EventEmitter_new(struct Allocator* alloc, struct Log* log, uint8_t* publicKey);
# 18 "net/EventEmitter.c" 2







# 1 "./util/ArrayList.h" 1
# 21 "./util/ArrayList.h"
struct Linker_x0852598370247027;





struct ArrayList;

void* ArrayList_new(struct Allocator* alloc, int initialCapacity);
int ArrayList_add(struct ArrayList* list, void* val);
void* ArrayList_get(struct ArrayList* list, int number);
int ArrayList_put(struct ArrayList* list, int number, void* val);
void* ArrayList_remove(struct ArrayList* list, int num);
void ArrayList_sort(struct ArrayList* list, int (* compare)(const void* a, const void* b));
void* ArrayList_clone(struct ArrayList* vlist, struct Allocator* alloc);
# 60 "./util/ArrayList.h"
struct ArrayList_Ifaces {
    int length;
};

static inline struct ArrayList_Ifaces* ArrayList_Ifaces_new(struct Allocator* alloc)
{
    return (struct ArrayList_Ifaces*) ArrayList_new(alloc, 8);
}

static inline struct Iface* ArrayList_Ifaces_get(struct ArrayList_Ifaces* list, int number)
{
    return (struct Iface*) ArrayList_get((struct ArrayList*) list, number);
}

static inline int ArrayList_Ifaces_put(struct ArrayList_Ifaces* list,
                                          int number,
                                          struct Iface* val)
{
    return ArrayList_put((struct ArrayList*) list, number, val);
}

static inline int ArrayList_Ifaces_add(struct ArrayList_Ifaces* list, void* val)
{
    return ArrayList_put((struct ArrayList*) list, list->length, val);
}

static inline struct Iface* ArrayList_Ifaces_shift(struct ArrayList_Ifaces* list)
{
    return (struct Iface*) ArrayList_remove((struct ArrayList*) list, 0);
}

static inline struct Iface* ArrayList_Ifaces_pop(struct ArrayList_Ifaces* list)
{
    return (struct Iface*) ArrayList_remove((struct ArrayList*) list, list->length - 1);
}

static inline struct Iface* ArrayList_Ifaces_remove(struct ArrayList_Ifaces* list, int num)
{
    return (struct Iface*) ArrayList_remove((struct ArrayList*) list, num);
}
# 113 "./util/ArrayList.h"
static inline void* ArrayList_Ifaces_clone(struct ArrayList_Ifaces* l, struct Allocator* alloc)
{
    return ArrayList_clone((struct ArrayList*) l, alloc);
}
# 26 "net/EventEmitter.c" 2



struct Pathfinder
{
    struct EventEmitter_pvt* ee;
    struct Iface iface;
    struct Allocator* alloc;

    uint8_t userAgent[64];

    uint32_t superiority;

    uint32_t version;




    uint16_t state;
    uint16_t pathfinderId;






    uint32_t bytesSinceLastPing;

    unsigned long Identity_verifier;
};


# 1 "./util/ArrayList.h" 1
# 60 "./util/ArrayList.h"
struct ArrayList_Pathfinders {
    int length;
};

static inline struct ArrayList_Pathfinders* ArrayList_Pathfinders_new(struct Allocator* alloc)
{
    return (struct ArrayList_Pathfinders*) ArrayList_new(alloc, 8);
}

static inline struct Pathfinder* ArrayList_Pathfinders_get(struct ArrayList_Pathfinders* list, int number)
{
    return (struct Pathfinder*) ArrayList_get((struct ArrayList*) list, number);
}

static inline int ArrayList_Pathfinders_put(struct ArrayList_Pathfinders* list,
                                          int number,
                                          struct Pathfinder* val)
{
    return ArrayList_put((struct ArrayList*) list, number, val);
}

static inline int ArrayList_Pathfinders_add(struct ArrayList_Pathfinders* list, void* val)
{
    return ArrayList_put((struct ArrayList*) list, list->length, val);
}

static inline struct Pathfinder* ArrayList_Pathfinders_shift(struct ArrayList_Pathfinders* list)
{
    return (struct Pathfinder*) ArrayList_remove((struct ArrayList*) list, 0);
}

static inline struct Pathfinder* ArrayList_Pathfinders_pop(struct ArrayList_Pathfinders* list)
{
    return (struct Pathfinder*) ArrayList_remove((struct ArrayList*) list, list->length - 1);
}

static inline struct Pathfinder* ArrayList_Pathfinders_remove(struct ArrayList_Pathfinders* list, int num)
{
    return (struct Pathfinder*) ArrayList_remove((struct ArrayList*) list, num);
}
# 113 "./util/ArrayList.h"
static inline void* ArrayList_Pathfinders_clone(struct ArrayList_Pathfinders* l, struct Allocator* alloc)
{
    return ArrayList_clone((struct ArrayList*) l, alloc);
}
# 59 "net/EventEmitter.c" 2

struct EventEmitter_pvt
{
    struct EventEmitter pub;
    struct Iface trickIf;
    struct Allocator* alloc;
    struct Log* log;
    struct ArrayList_Ifaces* listTable[PFChan_Pathfinder__TOO_HIGH - PFChan_Pathfinder__TOO_LOW];
    struct ArrayList_Pathfinders* pathfinders;
    uint8_t publicKey[32];
    unsigned long Identity_verifier;
};



static struct ArrayList_Ifaces* getHandlers(struct EventEmitter_pvt* ee,
                                            enum PFChan_Pathfinder ev,
                                            
# 76 "net/EventEmitter.c" 3 4
                                           _Bool 
# 76 "net/EventEmitter.c"
                                                create)
{
    if (ev <= PFChan_Pathfinder__TOO_LOW || ev >= PFChan_Pathfinder__TOO_HIGH) { return 
# 78 "net/EventEmitter.c" 3 4
                                                                                       ((void *)0)
# 78 "net/EventEmitter.c"
                                                                                           ; }
    struct ArrayList_Ifaces* out = ee->listTable[(ev - PFChan_Pathfinder__TOO_LOW - 1)];
    if (!out) {
        out = ee->listTable[(ev - PFChan_Pathfinder__TOO_LOW - 1)] = ArrayList_Ifaces_new(ee->alloc);
    }
    return out;
}

static __attribute__ ((warn_unused_result)) struct Iface* sendToPathfinder(struct Message* msg, struct Pathfinder* pf)
{
    if (!pf || pf->state != 1) { return 
# 88 "net/EventEmitter.c" 3 4
                                                                ((void *)0)
# 88 "net/EventEmitter.c"
                                                                    ; }
    if (pf->bytesSinceLastPing < 8192 && pf->bytesSinceLastPing + msg->length >= 8192) {
        struct Message* ping = Message_new(0, 512, msg->alloc);
        do {             struct Er_Ret* Er_ret = Message_epush32h(ping, ((uint32_t)__builtin_bswap32(((uint32_t)(pf->bytesSinceLastPing)))));                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("EventEmitter.c", 91, Er_ret);                 } else {                     Er__assertFail(Er_unwind("EventEmitter.c", 91, Er_ret));                 }             }                  } while (0);
        do {             struct Er_Ret* Er_ret = Message_epush32h(ping, ((uint32_t)__builtin_bswap32(((uint32_t)(0x01234567)))));                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("EventEmitter.c", 92, Er_ret);                 } else {                     Er__assertFail(Er_unwind("EventEmitter.c", 92, Er_ret));                 }             }                  } while (0);
        do {             struct Er_Ret* Er_ret = Message_epush32h(ping, ((uint32_t)__builtin_bswap32(((uint32_t)(PFChan_Core_PING)))));                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("EventEmitter.c", 93, Er_ret);                 } else {                     Er__assertFail(Er_unwind("EventEmitter.c", 93, Er_ret));                 }             }                  } while (0);
        Iface_send(&pf->iface, ping);
    }
    pf->bytesSinceLastPing += msg->length;
    return Iface_next(&pf->iface, msg);
}

static 
# 100 "net/EventEmitter.c" 3 4
      _Bool 
# 100 "net/EventEmitter.c"
           PFChan_Pathfinder_sizeOk(enum PFChan_Pathfinder ev, int size)
{
    switch (ev) {
        case PFChan_Pathfinder_CONNECT:
            return (size == 8 + 72);
        case PFChan_Pathfinder_SUPERIORITY:
            return (size == 8 + 4);
        case PFChan_Pathfinder_NODE:
        case PFChan_Pathfinder_SNODE:
            return (size == 8 + 64);
        case PFChan_Pathfinder_SENDMSG:
            return (size >= 8 + ((56 + 12) + 4));
        case PFChan_Pathfinder_PING:
        case PFChan_Pathfinder_PONG:
            return (size >= 8 + 8);
        case PFChan_Pathfinder_SESSIONS:
        case PFChan_Pathfinder_PEERS:
        case PFChan_Pathfinder_PATHFINDERS:
            return (size == 8);
        case PFChan_Pathfinder_CTRL_SENDMSG:
            return (size >= 8 + ((56 + 12) + 4));
        default:;
    }
    Assert_failure("invalid event [%d]", ev);
}

void Assert_compileTime(char x[1 - (!(PFChan_Pathfinder__TOO_LOW == 511))]);
void Assert_compileTime(char x[1 - (!(PFChan_Pathfinder__TOO_HIGH == 523))]);

static 
# 129 "net/EventEmitter.c" 3 4
      _Bool 
# 129 "net/EventEmitter.c"
           PFChan_Core_sizeOk(enum PFChan_Core ev, int size)
{
    switch (ev) {
        case PFChan_Core_CONNECT:
            return (size == 8 + 40);

        case PFChan_Core_PATHFINDER:
        case PFChan_Core_PATHFINDER_GONE:
            return (size == 8 + 72);

        case PFChan_Core_SWITCH_ERR:
            return (size >= 8 + (12 + 4 + (4 + 12 + 4) + 12));

        case PFChan_Core_SEARCH_REQ:
            return (size == 8 + 24);

        case PFChan_Core_PEER:
        case PFChan_Core_PEER_GONE:
        case PFChan_Core_SESSION:
        case PFChan_Core_SESSION_ENDED:
        case PFChan_Core_DISCOVERED_PATH:
        case PFChan_Core_UNSETUP_SESSION:
            return (size == 8 + 64);

        case PFChan_Core_MSG:
            return (size >= 8 + ((56 + 12) + 4));

        case PFChan_Core_PING:
        case PFChan_Core_PONG:
            return (size == 8 + 8);

        case PFChan_Core_CTRL_MSG:
            return (size >= 8 + ((56 + 12) + 4));

        case PFChan_Core_LINK_STATE:
            return (size >= 8 + 32) &&
                !((size - 8) % 32);

        default:;
    }
    Assert_failure("invalid event [%d]", ev);
}

void Assert_compileTime(char x[1 - (!(PFChan_Core__TOO_LOW == 1023))]);
void Assert_compileTime(char x[1 - (!(PFChan_Core__TOO_HIGH == 1040))]);

static __attribute__ ((warn_unused_result)) struct Iface* incomingFromCore(struct Message* msg, struct Iface* trickIf)
{
    struct EventEmitter_pvt* ee = (__extension__ ({ const __typeof__(((struct EventEmitter_pvt*)0)->trickIf)*__mptr = (trickIf); (__extension__ ({ __typeof__((struct EventEmitter_pvt*)((char*)__mptr - 
# 177 "net/EventEmitter.c" 3 4
                                 __builtin_offsetof (
# 177 "net/EventEmitter.c"
                                 struct EventEmitter_pvt
# 177 "net/EventEmitter.c" 3 4
                                 , 
# 177 "net/EventEmitter.c"
                                 trickIf
# 177 "net/EventEmitter.c" 3 4
                                 )
# 177 "net/EventEmitter.c"
                                 )) Identity_ptr = ((struct EventEmitter_pvt*)((char*)__mptr - 
# 177 "net/EventEmitter.c" 3 4
                                 __builtin_offsetof (
# 177 "net/EventEmitter.c"
                                 struct EventEmitter_pvt
# 177 "net/EventEmitter.c" 3 4
                                 , 
# 177 "net/EventEmitter.c"
                                 trickIf
# 177 "net/EventEmitter.c" 3 4
                                 )
# 177 "net/EventEmitter.c"
                                 )); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x4b1ab94c7a70012eull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("EventEmitter.c"), (177), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x4b1ab94c7a70012eull))"); } } while (0); Identity_ptr; })); }));
    do { if (!((!((uintptr_t)msg->bytes % 4) && "alignment"))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("EventEmitter.c"), (178), "(!((uintptr_t)msg->bytes % 4) && \"alignment\")"); } } while (0);
    enum PFChan_Core ev = (__extension__({             uint32_t Er_returnVal;             __builtin_memset(&Er_returnVal, 0, sizeof(Er_returnVal));             struct Er_Ret* Er_ret = Message_epop32be(&Er_returnVal, msg);                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("EventEmitter.c", 179, Er_ret);                 } else {                     Er__assertFail(Er_unwind("EventEmitter.c", 179, Er_ret));                 }             }                      Er_returnVal;         }));
    do { if (!((PFChan_Core_sizeOk(ev, msg->length+4)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("EventEmitter.c"), (180), "(PFChan_Core_sizeOk(ev, msg->length+4))"); } } while (0);
    uint32_t pathfinderNum = (__extension__({             uint32_t Er_returnVal;             __builtin_memset(&Er_returnVal, 0, sizeof(Er_returnVal));             struct Er_Ret* Er_ret = Message_epop32be(&Er_returnVal, msg);                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("EventEmitter.c", 181, Er_ret);                 } else {                     Er__assertFail(Er_unwind("EventEmitter.c", 181, Er_ret));                 }             }                      Er_returnVal;         }));
    do {             struct Er_Ret* Er_ret = Message_epush32h(msg, ((uint32_t)__builtin_bswap32(((uint32_t)(ev)))));                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("EventEmitter.c", 182, Er_ret);                 } else {                     Er__assertFail(Er_unwind("EventEmitter.c", 182, Er_ret));                 }             }                  } while (0);
    if (pathfinderNum != 0xffffffff) {
        struct Pathfinder* pf = ArrayList_Pathfinders_get(ee->pathfinders, pathfinderNum);
        do { if (!((pf && pf->state == 1))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("EventEmitter.c"), (185), "(pf && pf->state == 1)"); } } while (0);
        return sendToPathfinder(msg, pf);
    } else {
        for (int i = 0; i < ee->pathfinders->length; i++) {
            struct Pathfinder* pf = ArrayList_Pathfinders_get(ee->pathfinders, i);
            if (!pf || pf->state != 1) { continue; }
            struct Message* messageClone = Message_clone(msg, msg->alloc);
            do { do { if (!((!messageClone->currentIface))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("EventEmitter.c"), (192), "(!messageClone->currentIface)"); } } while (0); struct Iface Iface_y = { .currentMsg = messageClone }; messageClone->currentIface = &Iface_y; struct Iface* Iface_x = sendToPathfinder(messageClone, pf); messageClone->currentIface = 
# 192 "net/EventEmitter.c" 3 4
           ((void *)0)
# 192 "net/EventEmitter.c"
           ; if (Iface_x) { Iface_send(Iface_x, messageClone); } } while (0);
        }
    }
    return 
# 195 "net/EventEmitter.c" 3 4
          ((void *)0)
# 195 "net/EventEmitter.c"
              ;
}

static struct Message* pathfinderMsg(enum PFChan_Core ev,
                                     struct Pathfinder* pf,
                                     struct Allocator* alloc)
{
    struct Message* msg = Message_new(72, 512, alloc);
    struct PFChan_Core_Pathfinder* pathfinder = (struct PFChan_Core_Pathfinder*) msg->bytes;
    pathfinder->superiority_be = ((uint32_t)__builtin_bswap32(((uint32_t)(pf->superiority))));
    pathfinder->pathfinderId_be = ((uint32_t)__builtin_bswap32(((uint32_t)(pf->pathfinderId))));
    Bits__memcpy(pathfinder->userAgent,pf->userAgent,64,"EventEmitter.c",206);
    do {             struct Er_Ret* Er_ret = Message_epush32h(msg, ((uint32_t)__builtin_bswap32(((uint32_t)(0xffffffff)))));                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("EventEmitter.c", 207, Er_ret);                 } else {                     Er__assertFail(Er_unwind("EventEmitter.c", 207, Er_ret));                 }             }                  } while (0);
    do {             struct Er_Ret* Er_ret = Message_epush32h(msg, ((uint32_t)__builtin_bswap32(((uint32_t)(ev)))));                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("EventEmitter.c", 208, Er_ret);                 } else {                     Er__assertFail(Er_unwind("EventEmitter.c", 208, Er_ret));                 }             }                  } while (0);
    return msg;
}

static int handleFromPathfinder(enum PFChan_Pathfinder ev,
                                struct Message* msg,
                                struct EventEmitter_pvt* ee,
                                struct Pathfinder* pf)
{
    switch (ev) {
        default: return 
# 218 "net/EventEmitter.c" 3 4
                       0
# 218 "net/EventEmitter.c"
                            ;

        case PFChan_Pathfinder_CONNECT: {
            struct PFChan_Pathfinder_Connect connect;
            do {             struct Er_Ret* Er_ret = Message_eshift(msg, -8);                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("EventEmitter.c", 222, Er_ret);                 } else {                     Er__assertFail(Er_unwind("EventEmitter.c", 222, Er_ret));                 }             }                  } while (0);
            do {             struct Er_Ret* Er_ret = Message_epop(msg, &connect, 72);                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("EventEmitter.c", 223, Er_ret);                 } else {                     Er__assertFail(Er_unwind("EventEmitter.c", 223, Er_ret));                 }             }                  } while (0);
            pf->superiority = ((uint32_t)__builtin_bswap32(((uint32_t)(connect.superiority_be))));
            pf->version = ((uint32_t)__builtin_bswap32(((uint32_t)(connect.version_be))));
            Bits__memcpy(pf->userAgent,connect.userAgent,64,"EventEmitter.c",226);
            pf->state = 1;

            struct PFChan_Core_Connect resp;
            resp.version_be = ((uint32_t)__builtin_bswap32(((uint32_t)(20))));
            resp.pathfinderId_be = ((uint32_t)__builtin_bswap32(((uint32_t)(pf->pathfinderId))));
            Bits__memcpy(resp.publicKey,ee->publicKey,32,"EventEmitter.c",232);
            do {             struct Er_Ret* Er_ret = Message_epush(msg, &resp, 40);                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("EventEmitter.c", 233, Er_ret);                 } else {                     Er__assertFail(Er_unwind("EventEmitter.c", 233, Er_ret));                 }             }                  } while (0);
            do {             struct Er_Ret* Er_ret = Message_epush32h(msg, ((uint32_t)__builtin_bswap32(((uint32_t)(PFChan_Core_CONNECT)))));                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("EventEmitter.c", 234, Er_ret);                 } else {                     Er__assertFail(Er_unwind("EventEmitter.c", 234, Er_ret));                 }             }                  } while (0);
            struct Message* sendMsg = Message_clone(msg, msg->alloc);
            do { do { if (!((!sendMsg->currentIface))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("EventEmitter.c"), (236), "(!sendMsg->currentIface)"); } } while (0); struct Iface Iface_y = { .currentMsg = sendMsg }; sendMsg->currentIface = &Iface_y; struct Iface* Iface_x = sendToPathfinder(sendMsg, pf); sendMsg->currentIface = 
# 236 "net/EventEmitter.c" 3 4
           ((void *)0)
# 236 "net/EventEmitter.c"
           ; if (Iface_x) { Iface_send(Iface_x, sendMsg); } } while (0);
            break;
        }
        case PFChan_Pathfinder_SUPERIORITY: {
            do {             struct Er_Ret* Er_ret = Message_eshift(msg, -8);                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("EventEmitter.c", 240, Er_ret);                 } else {                     Er__assertFail(Er_unwind("EventEmitter.c", 240, Er_ret));                 }             }                  } while (0);
            pf->superiority = (__extension__({             uint32_t Er_returnVal;             __builtin_memset(&Er_returnVal, 0, sizeof(Er_returnVal));             struct Er_Ret* Er_ret = Message_epop32be(&Er_returnVal, msg);                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("EventEmitter.c", 241, Er_ret);                 } else {                     Er__assertFail(Er_unwind("EventEmitter.c", 241, Er_ret));                 }             }                      Er_returnVal;         }));
            struct Message* resp = pathfinderMsg(PFChan_Core_PATHFINDER, pf, msg->alloc);
            do { do { if (!((!resp->currentIface))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("EventEmitter.c"), (243), "(!resp->currentIface)"); } } while (0); struct Iface Iface_y = { .currentMsg = resp }; resp->currentIface = &Iface_y; struct Iface* Iface_x = incomingFromCore(resp, &ee->trickIf); resp->currentIface = 
# 243 "net/EventEmitter.c" 3 4
           ((void *)0)
# 243 "net/EventEmitter.c"
           ; if (Iface_x) { Iface_send(Iface_x, resp); } } while (0);
            break;
        }

        case PFChan_Pathfinder_PING: {
            struct Message* sendMsg = Message_clone(msg, msg->alloc);
            Iface_send(&pf->iface, sendMsg);
            break;
        }
        case PFChan_Pathfinder_PONG: {
            do {             struct Er_Ret* Er_ret = Message_eshift(msg, -8);                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("EventEmitter.c", 253, Er_ret);                 } else {                     Er__assertFail(Er_unwind("EventEmitter.c", 253, Er_ret));                 }             }                  } while (0);
            uint32_t cookie = (__extension__({             uint32_t Er_returnVal;             __builtin_memset(&Er_returnVal, 0, sizeof(Er_returnVal));             struct Er_Ret* Er_ret = Message_epop32be(&Er_returnVal, msg);                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("EventEmitter.c", 254, Er_ret);                 } else {                     Er__assertFail(Er_unwind("EventEmitter.c", 254, Er_ret));                 }             }                      Er_returnVal;         }));
            uint32_t count = (__extension__({             uint32_t Er_returnVal;             __builtin_memset(&Er_returnVal, 0, sizeof(Er_returnVal));             struct Er_Ret* Er_ret = Message_epop32be(&Er_returnVal, msg);                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("EventEmitter.c", 255, Er_ret);                 } else {                     Er__assertFail(Er_unwind("EventEmitter.c", 255, Er_ret));                 }             }                      Er_returnVal;         }));
            if (cookie != 0x01234567 || count > pf->bytesSinceLastPing) {
                pf->state = 2;
                struct Message* resp = pathfinderMsg(PFChan_Core_PATHFINDER_GONE, pf, msg->alloc);
                do { do { if (!((!resp->currentIface))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("EventEmitter.c"), (259), "(!resp->currentIface)"); } } while (0); struct Iface Iface_y = { .currentMsg = resp }; resp->currentIface = &Iface_y; struct Iface* Iface_x = incomingFromCore(resp, &ee->trickIf); resp->currentIface = 
# 259 "net/EventEmitter.c" 3 4
               ((void *)0)
# 259 "net/EventEmitter.c"
               ; if (Iface_x) { Iface_send(Iface_x, resp); } } while (0);
            } else {
                pf->bytesSinceLastPing -= count;
            }
            break;
        }
        case PFChan_Pathfinder_PATHFINDERS: {
            for (int i = 0; i < ee->pathfinders->length; i++) {
                struct Pathfinder* xpf = ArrayList_Pathfinders_get(ee->pathfinders, i);
                if (!xpf || xpf->state != 1) { continue; }
                struct Allocator* alloc = Allocator__child((msg->alloc),"EventEmitter.c",269);
                struct Message* resp = pathfinderMsg(PFChan_Core_PATHFINDER, pf, alloc);
                do { do { if (!((!resp->currentIface))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("EventEmitter.c"), (271), "(!resp->currentIface)"); } } while (0); struct Iface Iface_y = { .currentMsg = resp }; resp->currentIface = &Iface_y; struct Iface* Iface_x = sendToPathfinder(resp, pf); resp->currentIface = 
# 271 "net/EventEmitter.c" 3 4
               ((void *)0)
# 271 "net/EventEmitter.c"
               ; if (Iface_x) { Iface_send(Iface_x, resp); } } while (0);
                Allocator__free((alloc),"EventEmitter.c",272);
            }
            break;
        }
    }
    return 
# 277 "net/EventEmitter.c" 3 4
          1
# 277 "net/EventEmitter.c"
              ;
}

static __attribute__ ((warn_unused_result)) struct Iface* incomingFromPathfinder(struct Message* msg, struct Iface* iface)
{
    struct Pathfinder* pf = (__extension__ ({ const __typeof__(((struct Pathfinder*)0)->iface)*__mptr = (iface); (__extension__ ({ __typeof__((struct Pathfinder*)((char*)__mptr - 
# 282 "net/EventEmitter.c" 3 4
                           __builtin_offsetof (
# 282 "net/EventEmitter.c"
                           struct Pathfinder
# 282 "net/EventEmitter.c" 3 4
                           , 
# 282 "net/EventEmitter.c"
                           iface
# 282 "net/EventEmitter.c" 3 4
                           )
# 282 "net/EventEmitter.c"
                           )) Identity_ptr = ((struct Pathfinder*)((char*)__mptr - 
# 282 "net/EventEmitter.c" 3 4
                           __builtin_offsetof (
# 282 "net/EventEmitter.c"
                           struct Pathfinder
# 282 "net/EventEmitter.c" 3 4
                           , 
# 282 "net/EventEmitter.c"
                           iface
# 282 "net/EventEmitter.c" 3 4
                           )
# 282 "net/EventEmitter.c"
                           )); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x4b1ab94c7a70012eull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("EventEmitter.c"), (282), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x4b1ab94c7a70012eull))"); } } while (0); Identity_ptr; })); }));
    struct EventEmitter_pvt* ee = (__extension__ ({ __typeof__((struct EventEmitter_pvt*) pf->ee) Identity_ptr = ((struct EventEmitter_pvt*) pf->ee); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x4b1ab94c7a70012eull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("EventEmitter.c"), (283), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x4b1ab94c7a70012eull))"); } } while (0); Identity_ptr; }));
    if (msg->length < 4) {
        do { if (ee->log && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(ee->log, Log_Level_DEBUG, "EventEmitter.c", 285, "DROPPF runt"); } } while (0);
        return 
# 286 "net/EventEmitter.c" 3 4
              ((void *)0)
# 286 "net/EventEmitter.c"
                  ;
    }
    enum PFChan_Pathfinder ev = (__extension__({             uint32_t Er_returnVal;             __builtin_memset(&Er_returnVal, 0, sizeof(Er_returnVal));             struct Er_Ret* Er_ret = Message_epop32be(&Er_returnVal, msg);                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("EventEmitter.c", 288, Er_ret);                 } else {                     Er__assertFail(Er_unwind("EventEmitter.c", 288, Er_ret));                 }             }                      Er_returnVal;         }));
    do {             struct Er_Ret* Er_ret = Message_epush32h(msg, ((uint32_t)__builtin_bswap32(((uint32_t)(pf->pathfinderId)))));                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("EventEmitter.c", 289, Er_ret);                 } else {                     Er__assertFail(Er_unwind("EventEmitter.c", 289, Er_ret));                 }             }                  } while (0);
    do {             struct Er_Ret* Er_ret = Message_epush32h(msg, ((uint32_t)__builtin_bswap32(((uint32_t)(ev)))));                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("EventEmitter.c", 290, Er_ret);                 } else {                     Er__assertFail(Er_unwind("EventEmitter.c", 290, Er_ret));                 }             }                  } while (0);
    if (ev <= PFChan_Pathfinder__TOO_LOW || ev >= PFChan_Pathfinder__TOO_HIGH) {
        do { if (ee->log && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(ee->log, Log_Level_DEBUG, "EventEmitter.c", 292, "DROPPF invalid type [%d]", ev); } } while (0);
        return 
# 293 "net/EventEmitter.c" 3 4
              ((void *)0)
# 293 "net/EventEmitter.c"
                  ;
    }
    if (!PFChan_Pathfinder_sizeOk(ev, msg->length)) {
        do { if (ee->log && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(ee->log, Log_Level_DEBUG, "EventEmitter.c", 296, "DROPPF incorrect length[%d] for type [%d]", msg->length, ev); } } while (0);
        return 
# 297 "net/EventEmitter.c" 3 4
              ((void *)0)
# 297 "net/EventEmitter.c"
                  ;
    }

    if (pf->state == 0) {
        if (ev != PFChan_Pathfinder_CONNECT) {
            do { if (ee->log && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(ee->log, Log_Level_DEBUG, "EventEmitter.c", 302, "DROPPF disconnected and event != CONNECT event:[%d]", ev); } } while (0);
            return 
# 303 "net/EventEmitter.c" 3 4
                  ((void *)0)
# 303 "net/EventEmitter.c"
                      ;
        }
    } else if (pf->state != 1) {
        do { if (ee->log && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(ee->log, Log_Level_DEBUG, "EventEmitter.c", 306, "DROPPF error state"); } } while (0);
        return 
# 307 "net/EventEmitter.c" 3 4
              ((void *)0)
# 307 "net/EventEmitter.c"
                  ;
    }

    if (handleFromPathfinder(ev, msg, ee, pf)) { return 
# 310 "net/EventEmitter.c" 3 4
                                                       ((void *)0)
# 310 "net/EventEmitter.c"
                                                           ; }

    struct ArrayList_Ifaces* handlers = getHandlers(ee, ev, 
# 312 "net/EventEmitter.c" 3 4
                                                           0
# 312 "net/EventEmitter.c"
                                                                );
    if (!handlers) { return 
# 313 "net/EventEmitter.c" 3 4
                           ((void *)0)
# 313 "net/EventEmitter.c"
                               ; }
    for (int i = 0; i < handlers->length; i++) {
        struct Message* messageClone = Message_clone(msg, msg->alloc);
        struct Iface* iface = ArrayList_Ifaces_get(handlers, i);


        do { if (!((iface))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("EventEmitter.c"), (319), "(iface)"); } } while (0);
        do { if (!((iface->send))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("EventEmitter.c"), (320), "(iface->send)"); } } while (0);
        do { do { if (!((!messageClone->currentIface))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("EventEmitter.c"), (321), "(!messageClone->currentIface)"); } } while (0); struct Iface Iface_y = { .currentMsg = messageClone }; messageClone->currentIface = &Iface_y; struct Iface* Iface_x = iface->send(messageClone, iface); messageClone->currentIface = 
# 321 "net/EventEmitter.c" 3 4
       ((void *)0)
# 321 "net/EventEmitter.c"
       ; if (Iface_x) { Iface_send(Iface_x, messageClone); } } while (0);
    }
    return 
# 323 "net/EventEmitter.c" 3 4
          ((void *)0)
# 323 "net/EventEmitter.c"
              ;
}

void EventEmitter_regCore(struct EventEmitter* eventEmitter,
                          struct Iface* iface,
                          enum PFChan_Pathfinder ev)
{
    struct EventEmitter_pvt* ee = (__extension__ ({ __typeof__((struct EventEmitter_pvt*) eventEmitter) Identity_ptr = ((struct EventEmitter_pvt*) eventEmitter); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x4b1ab94c7a70012eull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("EventEmitter.c"), (330), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x4b1ab94c7a70012eull))"); } } while (0); Identity_ptr; }));
    iface->connectedIf = &ee->trickIf;
    struct ArrayList_Ifaces* l = getHandlers(ee, ev, 
# 332 "net/EventEmitter.c" 3 4
                                                    1
# 332 "net/EventEmitter.c"
                                                        );
    if (!l) {
        do { if (!((ev == 0))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("EventEmitter.c"), (334), "(ev == 0)"); } } while (0);
        return;
    }
    ArrayList_Ifaces_add(l, iface);
}

void EventEmitter_regPathfinderIface(struct EventEmitter* emitter, struct Iface* iface)
{
    struct EventEmitter_pvt* ee = (__extension__ ({ __typeof__((struct EventEmitter_pvt*) emitter) Identity_ptr = ((struct EventEmitter_pvt*) emitter); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x4b1ab94c7a70012eull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("EventEmitter.c"), (342), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x4b1ab94c7a70012eull))"); } } while (0); Identity_ptr; }));
    struct Allocator* alloc = Allocator__child((ee->alloc),"EventEmitter.c",343);
    struct Pathfinder* pf = Allocator__calloc((alloc),(sizeof(struct Pathfinder)),(1),"EventEmitter.c",344);
    pf->ee = ee;
    pf->iface.send = incomingFromPathfinder;
    pf->alloc = alloc;
    Iface_plumb(&pf->iface, iface);
    (pf)->Identity_verifier = ((unsigned long) 0x4b1ab94c7a70012eull);
    int i = 0;
    for (; i < ee->pathfinders->length; i++) {
        struct Pathfinder* xpf = ArrayList_Pathfinders_get(ee->pathfinders, i);
        if (!xpf) { break; }
    }
    pf->pathfinderId = ArrayList_Pathfinders_put(ee->pathfinders, i, pf);
}

struct EventEmitter* EventEmitter_new(struct Allocator* allocator,
                                      struct Log* log,
                                      uint8_t* publicKey)
{
    struct Allocator* alloc = Allocator__child((allocator),"EventEmitter.c",362);
    struct EventEmitter_pvt* ee = Allocator__calloc((alloc),(sizeof(struct EventEmitter_pvt)),(1),"EventEmitter.c",363);
    ee->log = log;
    ee->alloc = alloc;
    ee->trickIf.send = incomingFromCore;
    ee->pathfinders = ArrayList_Pathfinders_new(ee->alloc);
    Bits__memcpy(ee->publicKey,publicKey,32,"EventEmitter.c",368);
    (ee)->Identity_verifier = ((unsigned long) 0x4b1ab94c7a70012eull);
    return &ee->pub;
}
