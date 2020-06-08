# 1 "net/SessionManager.c"
# 1 "/home/jojapoppa/cjdns//"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "net/SessionManager.c"
# 15 "net/SessionManager.c"
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
struct Linker_x03742183781222521;
# 33 "./util/Assert.h"
__attribute__((__format__ (__printf__, 1, 2)))
__attribute__((__noreturn__))
void Assert_failure(const char* format, ...);
# 19 "./util/Identity.h" 2
# 1 "./util/Constant.h" 1
# 33 "./util/Constant.h"

# 20 "./util/Identity.h" 2



    
# 19 "./memory/Allocator.h" 2


struct Linker_x034503090237519674;





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
# 16 "net/SessionManager.c" 2
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
struct Linker_x08909708188035332;
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

       
# 17 "net/SessionManager.c" 2
# 1 "./net/SessionManager.h" 1
# 18 "./net/SessionManager.h"
# 1 "./crypto/random/Random.h" 1
# 19 "./crypto/random/Random.h"
# 1 "./exception/Except.h" 1
# 18 "./exception/Except.h"
# 1 "./exception/Er.h" 1
# 23 "./exception/Er.h"
struct Linker_x028436264464697003;

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


struct Linker_x0671059517274665;



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
struct Linker_x06574480802723204;

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
struct Linker_x018291714438843587;



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

struct Linker_x019452760637498923;



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
# 19 "./net/SessionManager.h" 2
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
struct Linker_x014717368523934482;

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

struct Linker_x0197469263384072;


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
struct Linker_x022766960908062361;





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
struct Linker_x0049178114307413257;


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



# 1 "./util/events/EventBase.h" 1
# 20 "./util/events/EventBase.h"
struct Linker_x07452858532854445;

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
struct Linker_x08011439907064604;



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

struct Linker_x047197461921960127;







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
# 20 "./net/SessionManager.h" 2


# 1 "./net/EventEmitter.h" 1
# 18 "./net/EventEmitter.h"
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
# 19 "./net/EventEmitter.h" 2




struct Linker_x04783932908247075;

struct EventEmitter
{
    int unused;
};






void EventEmitter_regCore(struct EventEmitter* ee,
                          struct Iface* iface,
                          enum PFChan_Pathfinder ev);

void EventEmitter_regPathfinderIface(struct EventEmitter* ee, struct Iface* iface);

struct EventEmitter* EventEmitter_new(struct Allocator* alloc, struct Log* log, uint8_t* publicKey);
# 23 "./net/SessionManager.h" 2

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

struct Linker_x07329183463105635;
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
# 18 "net/SessionManager.c" 2
# 1 "./crypto/AddressCalc.h" 1
# 19 "./crypto/AddressCalc.h"
struct Linker_x06965737261097256;
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
# 19 "net/SessionManager.c" 2
# 1 "./util/AddrTools.h" 1
# 19 "./util/AddrTools.h"
struct Linker_x03800889054113705;




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
# 20 "net/SessionManager.c" 2
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
# 21 "net/SessionManager.c" 2
# 1 "./util/events/Time.h" 1
# 20 "./util/events/Time.h"
struct Linker_x0739938845824351;




uint64_t Time_hrtime(void);



uint64_t Time_currentTimeMilliseconds(struct EventBase* eventBase);


uint64_t Time_currentTimeSeconds(struct EventBase* eventBase);
# 22 "net/SessionManager.c" 2


# 1 "./util/events/Timeout.h" 1
# 20 "./util/events/Timeout.h"
struct Linker_x00680815885988606;



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
# 25 "net/SessionManager.c" 2
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
# 26 "net/SessionManager.c" 2






struct BufferedMessage
{
    struct Message* msg;
    struct Allocator* alloc;
    uint64_t timeSentMilliseconds;
};

struct Ip6 {
    uint8_t bytes[16];
};



# 1 "./util/Map.h" 1
# 23 "./util/Map.h"
    void Assert_compileTime(char x[1 - (!(!(sizeof(struct Ip6) % 4)))]);
# 42 "./util/Map.h"
    static inline uint32_t Map_BufferedMessages_hash(struct Ip6* key);

# 1 "./util/Hash.h" 1
# 21 "./util/Hash.h"
static uint32_t Hash_compute(uint8_t* str, int length)
{
    uint32_t hash = 5381;
    for (int i = 0; i < length; i++) {
        hash = ((hash << 5) + hash) ^ str[i];
    }
    return hash;
}
# 45 "./util/Map.h" 2

        static inline uint32_t Map_BufferedMessages_hash(struct Ip6* key)
        {
            return Hash_compute((uint8_t*)key, sizeof(struct Ip6));
        }



    static inline int Map_BufferedMessages_compare(struct Ip6* keyA, struct Ip6* keyB);


        static inline int Map_BufferedMessages_compare(struct Ip6* keyA, struct Ip6* keyB)
        {
            return __builtin_memcmp(keyA,keyB,sizeof(struct Ip6));
        }



struct Map_BufferedMessages
{

        uint32_t* hashCodes;
        struct Ip6* keys;







    struct BufferedMessage** values;

    uint32_t count;
    uint32_t capacity;

    struct Allocator* allocator;
};

static inline struct Map_BufferedMessages* Map_BufferedMessages_new(struct Allocator* allocator)
{
    return Allocator__clone((allocator),((&(struct Map_BufferedMessages) { .allocator = allocator })),sizeof(*((&(struct Map_BufferedMessages) { .allocator = allocator }))),"Map.h",85)

       ;
}






static inline int Map_BufferedMessages_indexForKey(struct Ip6* key, struct Map_BufferedMessages* map)
{
    uint32_t hashCode = (Map_BufferedMessages_hash(key));
    for (uint32_t i = 0; i < map->count; i++) {
        if (map->hashCodes[i] == hashCode
            && Map_BufferedMessages_compare(key, &map->keys[i]) == 0)
        {
            return i;
        }
    }
    return -1;
}
# 132 "./util/Map.h"
static inline int Map_BufferedMessages_remove(int index, struct Map_BufferedMessages* map)
{
    if (index >= 0 && index < (int) map->count - 1) {
# 157 "./util/Map.h"
            map->count--;
            map->hashCodes[index] = map->hashCodes[map->count];
            Bits__memcpy(&map->keys[index],&map->keys[map->count],sizeof(struct Ip6),"Map.h",159);
            Bits__memcpy(&map->values[index],&map->values[map->count],sizeof(struct BufferedMessage*),"Map.h",160);

        return 0;
    } else if (index == (int) map->count - 1) {
        map->count--;
        return 0;
    }
    return -1;
}


static inline int Map_BufferedMessages_put(struct Ip6* key,
                                    struct BufferedMessage** value,
                                    struct Map_BufferedMessages* map)




{
    if (map->count == map->capacity) {

            map->hashCodes = Allocator__realloc((map->allocator),(map->hashCodes),(sizeof(uint32_t) * (map->count + 10)),"Map.h",181)

                                                                                    ;
            map->keys = Allocator__realloc((map->allocator),(map->keys),(sizeof(struct Ip6) * (map->count + 10)),"Map.h",184)

                                                                                   ;
# 195 "./util/Map.h"
        map->values = Allocator__realloc((map->allocator),(map->values),(sizeof(struct BufferedMessage*) * (map->count + 10)),"Map.h",195)

                                                                                   ;

        map->capacity += 10;
    }

    int i = -1;


        i = Map_BufferedMessages_indexForKey(key, map);


    if (i < 0) {
        i = map->count;
        map->count++;




            map->hashCodes[i] = (Map_BufferedMessages_hash(key));
            Bits__memcpy(&map->keys[i],key,sizeof(struct Ip6),"Map.h",216);

    }

    Bits__memcpy(&map->values[i],value,sizeof(struct BufferedMessage*),"Map.h",220);

    return i;
}
# 46 "net/SessionManager.c" 2





# 1 "./util/Map.h" 1
# 23 "./util/Map.h"
    void Assert_compileTime(char x[1 - (!(!(sizeof(struct Ip6) % 4)))]);
# 42 "./util/Map.h"
    static inline uint32_t Map_OfSessionsByIp6_hash(struct Ip6* key);



        static inline uint32_t Map_OfSessionsByIp6_hash(struct Ip6* key)
        {
            return Hash_compute((uint8_t*)key, sizeof(struct Ip6));
        }



    static inline int Map_OfSessionsByIp6_compare(struct Ip6* keyA, struct Ip6* keyB);


        static inline int Map_OfSessionsByIp6_compare(struct Ip6* keyA, struct Ip6* keyB)
        {
            return __builtin_memcmp(keyA,keyB,sizeof(struct Ip6));
        }



struct Map_OfSessionsByIp6
{

        uint32_t* hashCodes;
        struct Ip6* keys;



        uint32_t* handles;
        uint32_t nextHandle;


    struct SessionManager_Session_pvt** values;

    uint32_t count;
    uint32_t capacity;

    struct Allocator* allocator;
};

static inline struct Map_OfSessionsByIp6* Map_OfSessionsByIp6_new(struct Allocator* allocator)
{
    return Allocator__clone((allocator),((&(struct Map_OfSessionsByIp6) { .allocator = allocator })),sizeof(*((&(struct Map_OfSessionsByIp6) { .allocator = allocator }))),"Map.h",85)

       ;
}






static inline int Map_OfSessionsByIp6_indexForKey(struct Ip6* key, struct Map_OfSessionsByIp6* map)
{
    uint32_t hashCode = (Map_OfSessionsByIp6_hash(key));
    for (uint32_t i = 0; i < map->count; i++) {
        if (map->hashCodes[i] == hashCode
            && Map_OfSessionsByIp6_compare(key, &map->keys[i]) == 0)
        {
            return i;
        }
    }
    return -1;
}



static inline int Map_OfSessionsByIp6_indexForHandle(uint32_t handle, struct Map_OfSessionsByIp6* map)
{
    uint32_t base = 0;
    for (uint32_t bufferLen = map->count; bufferLen != 0; bufferLen /= 2) {
        uint32_t currentHandle = map->handles[base + (bufferLen / 2)];
        if (handle >= currentHandle) {
            if (currentHandle == handle) {
                return base + (bufferLen / 2);
            }
            base += (bufferLen / 2) + 1;
            bufferLen--;
        }
    }
    return -1;
}







static inline int Map_OfSessionsByIp6_remove(int index, struct Map_OfSessionsByIp6* map)
{
    if (index >= 0 && index < (int) map->count - 1) {



                __builtin_memmove(&map->hashCodes[index],&map->hashCodes[index + 1],(map->count - index - 1) * sizeof(uint32_t))

                                                                         ;

                __builtin_memmove(&map->keys[index],&map->keys[index + 1],(map->count - index - 1) * sizeof(struct Ip6))

                                                                             ;

            __builtin_memmove(&map->handles[index],&map->handles[index + 1],(map->count - index - 1) * sizeof(uint32_t))

                                                                     ;

            __builtin_memmove(&map->values[index],&map->values[index + 1],(map->count - index - 1) * sizeof(struct SessionManager_Session_pvt*))

                                                                           ;

            map->count--;







        return 0;
    } else if (index == (int) map->count - 1) {
        map->count--;
        return 0;
    }
    return -1;
}


static inline int Map_OfSessionsByIp6_put(struct Ip6* key,
                                    struct SessionManager_Session_pvt** value,
                                    struct Map_OfSessionsByIp6* map)




{
    if (map->count == map->capacity) {

            map->hashCodes = Allocator__realloc((map->allocator),(map->hashCodes),(sizeof(uint32_t) * (map->count + 10)),"Map.h",181)

                                                                                    ;
            map->keys = Allocator__realloc((map->allocator),(map->keys),(sizeof(struct Ip6) * (map->count + 10)),"Map.h",184)

                                                                                   ;



            map->handles = Allocator__realloc((map->allocator),(map->handles),(sizeof(uint32_t) * (map->count + 10)),"Map.h",190)

                                                                                  ;


        map->values = Allocator__realloc((map->allocator),(map->values),(sizeof(struct SessionManager_Session_pvt*) * (map->count + 10)),"Map.h",195)

                                                                                   ;

        map->capacity += 10;
    }

    int i = -1;


        i = Map_OfSessionsByIp6_indexForKey(key, map);


    if (i < 0) {
        i = map->count;
        map->count++;

            map->handles[i] = map->nextHandle++;


            map->hashCodes[i] = (Map_OfSessionsByIp6_hash(key));
            Bits__memcpy(&map->keys[i],key,sizeof(struct Ip6),"Map.h",216);

    }

    Bits__memcpy(&map->values[i],value,sizeof(struct SessionManager_Session_pvt*),"Map.h",220);

    return i;
}
# 52 "net/SessionManager.c" 2

struct SessionManager_pvt
{
    struct SessionManager pub;
    struct Iface eventIf;
    struct Allocator* alloc;
    struct Map_BufferedMessages bufMap;
    struct Map_OfSessionsByIp6 ifaceMap;
    struct Log* log;
    struct CryptoAuth* cryptoAuth;
    struct EventBase* eventBase;
    uint32_t firstHandle;
    unsigned long Identity_verifier;
};

struct SessionManager_Session_pvt
{
    struct SessionManager_Session pub;

    struct SessionManager_pvt* sessionManager;

    struct Allocator* alloc;

    
# 75 "net/SessionManager.c" 3 4
   _Bool 
# 75 "net/SessionManager.c"
        foundKey;

    unsigned long Identity_verifier;
};
# 121 "net/SessionManager.c"
static void sendSession(struct SessionManager_Session_pvt* sess,
                        uint64_t path,
                        uint32_t destPf,
                        enum PFChan_Core ev)
{
    struct PFChan_Node session = {
        .path_be = ((uint64_t)__builtin_bswap64(((uint64_t)(path)))),
        .metric_be = ((uint32_t)__builtin_bswap32(((uint32_t)(sess->pub.metric)))),
        .version_be = ((uint32_t)__builtin_bswap32(((uint32_t)(sess->pub.version))))
    };
    Bits__memcpy(session.ip6,sess->pub.caSession->herIp6,16,"SessionManager.c",131);
    Bits__memcpy(session.publicKey,sess->pub.caSession->herPublicKey,32,"SessionManager.c",132);

    struct Allocator* alloc = Allocator__child((sess->alloc),"SessionManager.c",134);
    struct Message* msg = Message_new(0, 64 + 512, alloc);
    do {             struct Er_Ret* Er_ret = Message_epush(msg, &session, 64);                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("SessionManager.c", 136, Er_ret);                 } else {                     Er__assertFail(Er_unwind("SessionManager.c", 136, Er_ret));                 }             }                  } while (0);
    do {             struct Er_Ret* Er_ret = Message_epush32h(msg, ((uint32_t)__builtin_bswap32(((uint32_t)(destPf)))));                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("SessionManager.c", 137, Er_ret);                 } else {                     Er__assertFail(Er_unwind("SessionManager.c", 137, Er_ret));                 }             }                  } while (0);
    do {             struct Er_Ret* Er_ret = Message_epush32h(msg, ((uint32_t)__builtin_bswap32(((uint32_t)(ev)))));                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("SessionManager.c", 138, Er_ret);                 } else {                     Er__assertFail(Er_unwind("SessionManager.c", 138, Er_ret));                 }             }                  } while (0);
    Iface_send(&sess->sessionManager->eventIf, msg);
    Allocator__free((alloc),"SessionManager.c",140);
}

static inline void check(struct SessionManager_pvt* sm, int mapIndex)
{
    struct SessionManager_Session_pvt* ssp = (__extension__ ({ __typeof__(sm->ifaceMap.values[mapIndex]) Identity_ptr = (sm->ifaceMap.values[mapIndex]); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0xe6c7333919662984ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("SessionManager.c"), (145), "(Identity_ptr->Identity_verifier == ((unsigned long) 0xe6c7333919662984ull))"); } } while (0); Identity_ptr; }));
    if (ssp->foundKey) { return; }
    uint8_t* herPubKey = ssp->pub.caSession->herPublicKey;
    if (!Bits_isZero(herPubKey, 32)) {
        uint8_t ip6[16];
        AddressCalc_addressForPublicKey(ip6, herPubKey);
        do { if (!((!__builtin_memcmp(&sm->ifaceMap.keys[mapIndex],ip6,16)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("SessionManager.c"), (151), "(!__builtin_memcmp(&sm->ifaceMap.keys[mapIndex],ip6,16))"); } } while (0);
        ssp->foundKey = 
# 152 "net/SessionManager.c" 3 4
                       1
# 152 "net/SessionManager.c"
                           ;
    }
}

static inline struct SessionManager_Session_pvt* sessionForHandle(uint32_t handle,
                                                                  struct SessionManager_pvt* sm)
{
    int index = Map_OfSessionsByIp6_indexForHandle(handle - sm->firstHandle, &sm->ifaceMap);
    if (index < 0) { return 
# 160 "net/SessionManager.c" 3 4
                           ((void *)0)
# 160 "net/SessionManager.c"
                               ; }
    check(sm, index);
    return (__extension__ ({ __typeof__(sm->ifaceMap.values[index]) Identity_ptr = (sm->ifaceMap.values[index]); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0xe6c7333919662984ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("SessionManager.c"), (162), "(Identity_ptr->Identity_verifier == ((unsigned long) 0xe6c7333919662984ull))"); } } while (0); Identity_ptr; }));
}

struct SessionManager_Session* SessionManager_sessionForHandle(uint32_t handle,
                                                               struct SessionManager* manager)
{
    struct SessionManager_pvt* sm = (__extension__ ({ __typeof__((struct SessionManager_pvt*) manager) Identity_ptr = ((struct SessionManager_pvt*) manager); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0xe6c7333919662984ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("SessionManager.c"), (168), "(Identity_ptr->Identity_verifier == ((unsigned long) 0xe6c7333919662984ull))"); } } while (0); Identity_ptr; }));
    return (struct SessionManager_Session*) sessionForHandle(handle, sm);
}

static inline struct SessionManager_Session_pvt* sessionForIp6(uint8_t ip6[16],
                                                               struct SessionManager_pvt* sm)
{
    int ifaceIndex = Map_OfSessionsByIp6_indexForKey((struct Ip6*)ip6, &sm->ifaceMap);
    if (ifaceIndex == -1) { return 
# 176 "net/SessionManager.c" 3 4
                                  ((void *)0)
# 176 "net/SessionManager.c"
                                      ; }
    check(sm, ifaceIndex);
    return (__extension__ ({ __typeof__(sm->ifaceMap.values[ifaceIndex]) Identity_ptr = (sm->ifaceMap.values[ifaceIndex]); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0xe6c7333919662984ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("SessionManager.c"), (178), "(Identity_ptr->Identity_verifier == ((unsigned long) 0xe6c7333919662984ull))"); } } while (0); Identity_ptr; }));
}

struct SessionManager_Session* SessionManager_sessionForIp6(uint8_t* ip6,
                                                            struct SessionManager* manager)
{
    struct SessionManager_pvt* sm = (__extension__ ({ __typeof__((struct SessionManager_pvt*) manager) Identity_ptr = ((struct SessionManager_pvt*) manager); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0xe6c7333919662984ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("SessionManager.c"), (184), "(Identity_ptr->Identity_verifier == ((unsigned long) 0xe6c7333919662984ull))"); } } while (0); Identity_ptr; }));
    return (struct SessionManager_Session*) sessionForIp6(ip6, sm);
}

struct SessionManager_HandleList* SessionManager_getHandleList(struct SessionManager* manager,
                                                               struct Allocator* alloc)
{
    struct SessionManager_pvt* sm = (__extension__ ({ __typeof__((struct SessionManager_pvt*) manager) Identity_ptr = ((struct SessionManager_pvt*) manager); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0xe6c7333919662984ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("SessionManager.c"), (191), "(Identity_ptr->Identity_verifier == ((unsigned long) 0xe6c7333919662984ull))"); } } while (0); Identity_ptr; }));
    struct SessionManager_HandleList* out =
        Allocator__calloc((alloc),(sizeof(struct SessionManager_HandleList)),(1),"SessionManager.c",193);
    uint32_t* buff = Allocator__calloc((alloc),(4),(sm->ifaceMap.count),"SessionManager.c",194);
    out->length = sm->ifaceMap.count;
    out->handles = buff;
    for (int i = 0; i < out->length; i++) {
        buff[i] = sm->ifaceMap.handles[i] + sm->firstHandle;
    }
    return out;
}

static struct SessionManager_Session_pvt* getSession(struct SessionManager_pvt* sm,
                                                     uint8_t ip6[16],
                                                     uint8_t pubKey[32],
                                                     uint32_t version,
                                                     uint64_t label,
                                                     uint32_t metric,
                                                     int maintainSession)
{
    do { if (!((AddressCalc_validAddress(ip6)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("SessionManager.c"), (211), "(AddressCalc_validAddress(ip6))"); } } while (0);
    struct SessionManager_Session_pvt* sess = sessionForIp6(ip6, sm);
    if (sess) {
        sess->pub.version = (sess->pub.version) ? sess->pub.version : version;
        sess->pub.maintainSession |= maintainSession;
        if (metric == 0xffffffff) {

            if (sess->pub.sendSwitchLabel == label) {
                do { if (!1) { break; } uint8_t sendPath[20]; uint8_t recvPath[20]; uint8_t ip[40]; AddrTools_printPath(sendPath, (sess)->pub.sendSwitchLabel); AddrTools_printPath(recvPath, (sess)->pub.recvSwitchLabel); AddrTools_printIp(ip, (sess)->pub.caSession->herIp6); do { if ((sm->log) && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print((sm->log), Log_Level_DEBUG, "SessionManager.c", 219, "Session[%p] sendPath[%s] recvPath[%s] ip[%s] " "%s", (void*)sess, sendPath, recvPath, ip, "broken path"); } } while (0); } while (0);
                if (sess->pub.sendSwitchLabel == sess->pub.recvSwitchLabel) {
                    sess->pub.sendSwitchLabel = 0;
                    sess->pub.metric = 0xffffffff;
                } else {
                    sess->pub.sendSwitchLabel = sess->pub.recvSwitchLabel;
                    sess->pub.metric = 0xfffffff0;
                }
            }
        } else {
            if (metric <= sess->pub.metric) {
                sess->pub.sendSwitchLabel = label;
                sess->pub.version = (version) ? version : sess->pub.version;
                sess->pub.metric = metric;
                do { if (!1) { break; } uint8_t sendPath[20]; uint8_t recvPath[20]; uint8_t ip[40]; AddrTools_printPath(sendPath, (sess)->pub.sendSwitchLabel); AddrTools_printPath(recvPath, (sess)->pub.recvSwitchLabel); AddrTools_printIp(ip, (sess)->pub.caSession->herIp6); do { if ((sm->log) && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print((sm->log), Log_Level_DEBUG, "SessionManager.c", 233, "Session[%p] sendPath[%s] recvPath[%s] ip[%s] " "%s", (void*)sess, sendPath, recvPath, ip, "discovered path"); } } while (0); } while (0);
            }
        }
        return sess;
    }
    struct Allocator* alloc = Allocator__child((sm->alloc),"SessionManager.c",238);
    sess = Allocator__calloc((alloc),(sizeof(struct SessionManager_Session_pvt)),(1),"SessionManager.c",239);
    (sess)->Identity_verifier = ((unsigned long) 0xe6c7333919662984ull);

    sess->pub.caSession = CryptoAuth_newSession(sm->cryptoAuth, alloc, pubKey, 
# 242 "net/SessionManager.c" 3 4
                                                                              0
# 242 "net/SessionManager.c"
                                                                                   , "inner");

    sess->foundKey = !Bits_isZero(pubKey, 32);
    if (sess->foundKey) {
        uint8_t realIp6[16];
        AddressCalc_addressForPublicKey(realIp6, pubKey);
        do { if (!((!__builtin_memcmp(realIp6,ip6,16)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("SessionManager.c"), (248), "(!__builtin_memcmp(realIp6,ip6,16))"); } } while (0);
    }

    int ifaceIndex = Map_OfSessionsByIp6_put((struct Ip6*)ip6, &sess, &sm->ifaceMap);
    sess->pub.receiveHandle = sm->ifaceMap.handles[ifaceIndex] + sm->firstHandle;

    if (1) {
        uint8_t printedIp6[40];
        AddrTools_printIp(printedIp6, ip6);
        do { if (sm->log && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(sm->log, Log_Level_DEBUG, "SessionManager.c", 257, "Created session for [%s] handle [%u]", printedIp6, sess->pub.receiveHandle); } } while (0)
                                                      ;
    }

    sess->alloc = alloc;
    sess->sessionManager = sm;
    sess->pub.version = version;
    sess->pub.timeOfLastIn = Time_currentTimeMilliseconds(sm->eventBase);
    sess->pub.timeOfKeepAliveIn = Time_currentTimeMilliseconds(sm->eventBase);
    sess->pub.timeOfLastOut = Time_currentTimeMilliseconds(sm->eventBase);
    sess->pub.sendSwitchLabel = label;
    sess->pub.metric = metric;
    sess->pub.maintainSession = maintainSession;

    sendSession(sess, label, 0xffffffff, PFChan_Core_SESSION);
    check(sm, ifaceIndex);
    return sess;
}

static __attribute__ ((warn_unused_result)) struct Iface* ctrlFrame(struct Message* msg, struct SessionManager_pvt* sm)
{
    struct RouteHeader rh;
    __builtin_memset(&rh,0,(56 + 12));
    do {             struct Er_Ret* Er_ret = Message_epop(msg, &rh.sh, 12);                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("SessionManager.c", 280, Er_ret);                 } else {                     Er__assertFail(Er_unwind("SessionManager.c", 280, Er_ret));                 }             }                  } while (0);
    do {             struct Er_Ret* Er_ret = Message_epop(msg,    ((void *)0)   , 4);                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("SessionManager.c", 281, Er_ret);                 } else {                     Er__assertFail(Er_unwind("SessionManager.c", 281, Er_ret));                 }             }                  } while (0);
    rh.flags = 1 | (1<<1);
    do {             struct Er_Ret* Er_ret = Message_epush(msg, &rh, (56 + 12));                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("SessionManager.c", 283, Er_ret);                 } else {                     Er__assertFail(Er_unwind("SessionManager.c", 283, Er_ret));                 }             }                  } while (0);
    return Iface_next(&sm->pub.insideIf, msg);
}

static __attribute__ ((warn_unused_result)) struct Iface* failedDecrypt(struct Message* msg,
                                 uint64_t label_be,
                                 struct SessionManager_pvt* sm)
{
    do {             struct Er_Ret* Er_ret = Message_epush32h(msg, ((uint32_t)__builtin_bswap32(((uint32_t)(6)))));                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("SessionManager.c", 291, Er_ret);                 } else {                     Er__assertFail(Er_unwind("SessionManager.c", 291, Er_ret));                 }             }                  } while (0);
    do {             struct Er_Ret* Er_ret = Message_epush16h(msg, ((uint16_t)(__extension__ ({ uint16_t _x = (((uint16_t)(2))); _x >> 8 | _x << 8; }))));                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("SessionManager.c", 292, Er_ret);                 } else {                     Er__assertFail(Er_unwind("SessionManager.c", 292, Er_ret));                 }             }                  } while (0);
    do {             struct Er_Ret* Er_ret = Message_epush16h(msg, ((uint16_t)(__extension__ ({ uint16_t _x = (((uint16_t)(0))); _x >> 8 | _x << 8; }))));                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("SessionManager.c", 293, Er_ret);                 } else {                     Er__assertFail(Er_unwind("SessionManager.c", 293, Er_ret));                 }             }                  } while (0);
    uint16_t csum = Checksum_engine(msg->bytes, msg->length);
    (__extension__({             uint16_t Er_returnVal;             __builtin_memset(&Er_returnVal, 0, sizeof(Er_returnVal));             struct Er_Ret* Er_ret = Message_epop16be(&Er_returnVal, msg);                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("SessionManager.c", 295, Er_ret);                 } else {                     Er__assertFail(Er_unwind("SessionManager.c", 295, Er_ret));                 }             }                      Er_returnVal;         }));
    do {             struct Er_Ret* Er_ret = Message_epush16h(msg, ((uint16_t)(__extension__ ({ uint16_t _x = (((uint16_t)(csum))); _x >> 8 | _x << 8; }))));                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("SessionManager.c", 296, Er_ret);                 } else {                     Er__assertFail(Er_unwind("SessionManager.c", 296, Er_ret));                 }             }                  } while (0);

    do {             struct Er_Ret* Er_ret = Message_epush32h(msg, ((uint32_t)__builtin_bswap32(((uint32_t)(0xffffffff)))));                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("SessionManager.c", 298, Er_ret);                 } else {                     Er__assertFail(Er_unwind("SessionManager.c", 298, Er_ret));                 }             }                  } while (0);

    struct SwitchHeader sh;
    __builtin_memset(&sh,0,12);
    SwitchHeader_setSuppressErrors(&sh, 
# 302 "net/SessionManager.c" 3 4
                                       1
# 302 "net/SessionManager.c"
                                           );
    SwitchHeader_setVersion(&sh, 1);
    sh.label_be = label_be;
    do {             struct Er_Ret* Er_ret = Message_epush(msg, &sh, 12);                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("SessionManager.c", 305, Er_ret);                 } else {                     Er__assertFail(Er_unwind("SessionManager.c", 305, Er_ret));                 }             }                  } while (0);

    return Iface_next(&sm->pub.switchIf, msg);
}

static __attribute__ ((warn_unused_result)) struct Iface* incomingFromSwitchIf(struct Message* msg, struct Iface* iface)
{
    struct SessionManager_pvt* sm =
        (__extension__ ({ const __typeof__(((struct SessionManager_pvt*)0)->pub.switchIf)*__mptr = (iface); (__extension__ ({ __typeof__((struct SessionManager_pvt*)((char*)__mptr - 
# 313 "net/SessionManager.c" 3 4
       __builtin_offsetof (
# 313 "net/SessionManager.c"
       struct SessionManager_pvt
# 313 "net/SessionManager.c" 3 4
       , 
# 313 "net/SessionManager.c"
       pub.switchIf
# 313 "net/SessionManager.c" 3 4
       )
# 313 "net/SessionManager.c"
       )) Identity_ptr = ((struct SessionManager_pvt*)((char*)__mptr - 
# 313 "net/SessionManager.c" 3 4
       __builtin_offsetof (
# 313 "net/SessionManager.c"
       struct SessionManager_pvt
# 313 "net/SessionManager.c" 3 4
       , 
# 313 "net/SessionManager.c"
       pub.switchIf
# 313 "net/SessionManager.c" 3 4
       )
# 313 "net/SessionManager.c"
       )); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0xe6c7333919662984ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("SessionManager.c"), (313), "(Identity_ptr->Identity_verifier == ((unsigned long) 0xe6c7333919662984ull))"); } } while (0); Identity_ptr; })); }));


    if (msg->length < 12 + 4) {
        do { if (sm->log && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(sm->log, Log_Level_DEBUG, "SessionManager.c", 317, "DROP runt"); } } while (0);
        return 
# 318 "net/SessionManager.c" 3 4
              ((void *)0)
# 318 "net/SessionManager.c"
                  ;
    }

    struct SwitchHeader* switchHeader = (struct SwitchHeader*) msg->bytes;
    do {             struct Er_Ret* Er_ret = Message_eshift(msg, -12);                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("SessionManager.c", 322, Er_ret);                 } else {                     Er__assertFail(Er_unwind("SessionManager.c", 322, Er_ret));                 }             }                  } while (0);




    switchHeader->label_be = Bits_bitReverse64(switchHeader->label_be);

    struct SessionManager_Session_pvt* session;
    uint32_t nonceOrHandle = ((uint32_t)__builtin_bswap32(((uint32_t)(((uint32_t*)msg->bytes)[0]))));
    if (nonceOrHandle == 0xffffffff) {
        do {             struct Er_Ret* Er_ret = Message_eshift(msg, 12);                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("SessionManager.c", 332, Er_ret);                 } else {                     Er__assertFail(Er_unwind("SessionManager.c", 332, Er_ret));                 }             }                  } while (0);
        return ctrlFrame(msg, sm);
    }


    if (msg->length < 4 + 20) {
        do { if (sm->log && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(sm->log, Log_Level_DEBUG, "SessionManager.c", 338, "DROP runt"); } } while (0);
        return 
# 339 "net/SessionManager.c" 3 4
              ((void *)0)
# 339 "net/SessionManager.c"
                  ;
    }


    uint8_t firstSixteen[16];
    uint32_t length0 = msg->length;
    do { if (!((msg->length >= 16))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("SessionManager.c"), (345), "(msg->length >= 16)"); } } while (0);
    Bits__memcpy(firstSixteen,msg->bytes,16,"SessionManager.c",346);

    if (nonceOrHandle > 3) {

        session = sessionForHandle(nonceOrHandle, sm);
        if (!session) {
            do { if (sm->log && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(sm->log, Log_Level_DEBUG, "SessionManager.c", 352, "DROP message with unrecognized handle [%u]", nonceOrHandle); } } while (0);
            return 
# 353 "net/SessionManager.c" 3 4
                  ((void *)0)
# 353 "net/SessionManager.c"
                      ;
        }
        do {             struct Er_Ret* Er_ret = Message_eshift(msg, -4);                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("SessionManager.c", 355, Er_ret);                 } else {                     Er__assertFail(Er_unwind("SessionManager.c", 355, Er_ret));                 }             }                  } while (0);
        uint32_t nonce = ((uint32_t)__builtin_bswap32(((uint32_t)(((uint32_t*)msg->bytes)[0]))));
        if (nonce < 4) {
            do { if (sm->log && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(sm->log, Log_Level_DEBUG, "SessionManager.c", 358, "DROP setup message [%u] with specified handle [%u]", nonce, nonceOrHandle); } } while (0)
                                     ;
            return 
# 360 "net/SessionManager.c" 3 4
                  ((void *)0)
# 360 "net/SessionManager.c"
                      ;
        }
    } else {

        if (msg->length < 120 + 4) {
            do { if (sm->log && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(sm->log, Log_Level_DEBUG, "SessionManager.c", 365, "DROP runt"); } } while (0);
            return 
# 366 "net/SessionManager.c" 3 4
                  ((void *)0)
# 366 "net/SessionManager.c"
                      ;
        }
        struct CryptoHeader* caHeader = (struct CryptoHeader*) msg->bytes;
        uint8_t ip6[16];

        if (!AddressCalc_addressForPublicKey(ip6, caHeader->publicKey)) {
            do { if (sm->log && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(sm->log, Log_Level_DEBUG, "SessionManager.c", 372, "DROP Handshake with non-fc key"); } } while (0);
            return 
# 373 "net/SessionManager.c" 3 4
                  ((void *)0)
# 373 "net/SessionManager.c"
                      ;
        }

        if (!__builtin_memcmp(caHeader->publicKey,sm->cryptoAuth->publicKey,32)) {
            do { if (sm->log && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(sm->log, Log_Level_DEBUG, "SessionManager.c", 377, "DROP Handshake from 'ourselves'"); } } while (0);
            return 
# 378 "net/SessionManager.c" 3 4
                  ((void *)0)
# 378 "net/SessionManager.c"
                      ;
        }

        uint64_t label = ((uint64_t)__builtin_bswap64(((uint64_t)(switchHeader->label_be))));
        session = getSession(sm, ip6, caHeader->publicKey, 0, label, 0xfffff000, 0);
        CryptoAuth_resetIfTimeout(session->pub.caSession);
        do { if (!1) { break; } uint8_t path[20]; AddrTools_printPath(path, label); uint8_t ip[40]; AddrTools_printIp(ip, session->pub.caSession->herIp6); do { if (sm->log && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(sm->log, Log_Level_DEBUG, "SessionManager.c", 384, "ver[%u] send[%d] recv[%u] ip[%s] path[%s] " "new session nonce[%d]", session->pub.version, session->pub.sendHandle, session->pub.receiveHandle, ip, path, nonceOrHandle); } } while (0); } while (0);
    }

    
# 387 "net/SessionManager.c" 3 4
   _Bool 
# 387 "net/SessionManager.c"
        currentMessageSetup = (nonceOrHandle <= 3);

    enum CryptoAuth_DecryptErr ret = CryptoAuth_decrypt(session->pub.caSession, msg);
    if (ret) {
        do { if (!1) { break; } uint8_t path[20]; AddrTools_printPath(path, ((uint64_t)__builtin_bswap64(((uint64_t)(switchHeader->label_be))))); uint8_t ip[40]; AddrTools_printIp(ip, session->pub.caSession->herIp6); do { if (sm->log && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(sm->log, Log_Level_DEBUG, "SessionManager.c", 391, "ver[%u] send[%d] recv[%u] ip[%s] path[%s] " "DROP Failed decrypting message NoH[%d] state[%s]", session->pub.version, session->pub.sendHandle, session->pub.receiveHandle, ip, path, nonceOrHandle, CryptoAuth_stateString(CryptoAuth_getState(session->pub.caSession))); } } while (0); } while (0)



                                                                                                 ;
        do {             struct Er_Ret* Er_ret = Message_eshift(msg, length0 - msg->length - 24);                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("SessionManager.c", 396, Er_ret);                 } else {                     Er__assertFail(Er_unwind("SessionManager.c", 396, Er_ret));                 }             }                  } while (0);
        msg->length = 0;
        do {             struct Er_Ret* Er_ret = Message_epush32h(msg, ((uint32_t)__builtin_bswap32(((uint32_t)(CryptoAuth_getState(session->pub.caSession))))));                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("SessionManager.c", 398, Er_ret);                 } else {                     Er__assertFail(Er_unwind("SessionManager.c", 398, Er_ret));                 }             }                  } while (0);
        do {             struct Er_Ret* Er_ret = Message_epush32h(msg, ((uint32_t)__builtin_bswap32(((uint32_t)(ret)))));                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("SessionManager.c", 399, Er_ret);                 } else {                     Er__assertFail(Er_unwind("SessionManager.c", 399, Er_ret));                 }             }                  } while (0);
        do {             struct Er_Ret* Er_ret = Message_epush(msg, firstSixteen, 16);                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("SessionManager.c", 400, Er_ret);                 } else {                     Er__assertFail(Er_unwind("SessionManager.c", 400, Er_ret));                 }             }                  } while (0);
        do {             struct Er_Ret* Er_ret = Message_eshift(msg, 12);                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("SessionManager.c", 401, Er_ret);                 } else {                     Er__assertFail(Er_unwind("SessionManager.c", 401, Er_ret));                 }             }                  } while (0);
        do { if (!((msg->bytes == (uint8_t*)switchHeader))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("SessionManager.c"), (402), "(msg->bytes == (uint8_t*)switchHeader)"); } } while (0);
        uint64_t label_be = switchHeader->label_be;
        switchHeader->label_be = Bits_bitReverse64(switchHeader->label_be);
        return failedDecrypt(msg, label_be, sm);
    }

    if (currentMessageSetup) {
        session->pub.sendHandle = (__extension__({             uint32_t Er_returnVal;             __builtin_memset(&Er_returnVal, 0, sizeof(Er_returnVal));             struct Er_Ret* Er_ret = Message_epop32be(&Er_returnVal, msg);                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("SessionManager.c", 409, Er_ret);                 } else {                     Er__assertFail(Er_unwind("SessionManager.c", 409, Er_ret));                 }             }                      Er_returnVal;         }));
    }

    do {             struct Er_Ret* Er_ret = Message_eshift(msg, (56 + 12));                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("SessionManager.c", 412, Er_ret);                 } else {                     Er__assertFail(Er_unwind("SessionManager.c", 412, Er_ret));                 }             }                  } while (0);
    struct RouteHeader* header = (struct RouteHeader*) msg->bytes;

    do { if (!((msg->length >= 4))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("SessionManager.c"), (415), "(msg->length >= 4)"); } } while (0);
    struct DataHeader* dh = (struct DataHeader*) &header[1];
    if (DataHeader_getContentType(dh) != ContentType_CJDHT) {
        session->pub.timeOfLastIn = Time_currentTimeMilliseconds(sm->eventBase);
    }
    session->pub.bytesIn += msg->length;
    session->pub.timeOfKeepAliveIn = Time_currentTimeMilliseconds(sm->eventBase);

    if (currentMessageSetup) {
        Bits__memcpy(&header->sh,switchHeader,12,"SessionManager.c",424);
        do { if (!1) { break; } uint8_t path[20]; AddrTools_printPath(path, ((uint64_t)__builtin_bswap64(((uint64_t)(switchHeader->label_be))))); uint8_t ip[40]; AddrTools_printIp(ip, session->pub.caSession->herIp6); do { if (sm->log && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(sm->log, Log_Level_DEBUG, "SessionManager.c", 425, "ver[%u] send[%d] recv[%u] ip[%s] path[%s] " "%s", session->pub.version, session->pub.sendHandle, session->pub.receiveHandle, ip, path, "received start message"); } } while (0); } while (0)


                                                       ;
    } else {

        do { if (!((&header->sh == switchHeader))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("SessionManager.c"), (431), "(&header->sh == switchHeader)"); } } while (0);
        if (0) {
        do { if (!1) { break; } uint8_t path[20]; AddrTools_printPath(path, ((uint64_t)__builtin_bswap64(((uint64_t)(switchHeader->label_be))))); uint8_t ip[40]; AddrTools_printIp(ip, session->pub.caSession->herIp6); do { if (sm->log && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(sm->log, Log_Level_DEBUG, "SessionManager.c", 433, "ver[%u] send[%d] recv[%u] ip[%s] path[%s] " "%s", session->pub.version, session->pub.sendHandle, session->pub.receiveHandle, ip, path, "received run message"); } } while (0); } while (0)


                                                     ;
        }
    }

    header->version_be = ((uint32_t)__builtin_bswap32(((uint32_t)(session->pub.version))));
    Bits__memcpy(header->ip6,session->pub.caSession->herIp6,16,"SessionManager.c",441);
    Bits__memcpy(header->publicKey,session->pub.caSession->herPublicKey,32,"SessionManager.c",442);

    header->unused = 0;
    header->flags = 1;

    uint64_t path = ((uint64_t)__builtin_bswap64(((uint64_t)(switchHeader->label_be))));
    if (!session->pub.sendSwitchLabel) {
        session->pub.sendSwitchLabel = path;
    }
    if (path != session->pub.recvSwitchLabel) {
        session->pub.recvSwitchLabel = path;
        sendSession(session, path, 0xffffffff, PFChan_Core_DISCOVERED_PATH);
    }

    return Iface_next(&sm->pub.insideIf, msg);
}

static void checkTimedOutBuffers(struct SessionManager_pvt* sm)
{
    for (int i = 0; i < (int)sm->bufMap.count; i++) {
        struct BufferedMessage* buffered = sm->bufMap.values[i];
        int64_t lag = Time_currentTimeMilliseconds(sm->eventBase) - buffered->timeSentMilliseconds;
        if (lag < 10000) { continue; }
        Map_BufferedMessages_remove(i, &sm->bufMap);
        Allocator__free((buffered->alloc),"SessionManager.c",466);
        i--;
    }
}

static void unsetupSession(struct SessionManager_pvt* sm, struct SessionManager_Session_pvt* sess)
{
    if (!sess->pub.version || !(sess->pub.sendSwitchLabel || sess->pub.recvSwitchLabel)) {

        return;
    }
    struct Allocator* eventAlloc = Allocator__child((sm->alloc),"SessionManager.c",477);
    struct Message* eventMsg = Message_new(0, 512, eventAlloc);
    struct PFChan_Node n;
    n.path_be = ((uint64_t)__builtin_bswap64(((uint64_t)(sess->pub.sendSwitchLabel ? sess->pub.sendSwitchLabel : sess->pub.recvSwitchLabel))))
                                                                                               ;
    n.version_be = ((uint32_t)__builtin_bswap32(((uint32_t)(sess->pub.version))));
    Bits__memcpy(n.publicKey,sess->pub.caSession->herPublicKey,32,"SessionManager.c",483);
    Bits__memcpy(n.ip6,sess->pub.caSession->herIp6,16,"SessionManager.c",484);
    do {             struct Er_Ret* Er_ret = Message_epush(eventMsg, &n, 64);                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("SessionManager.c", 485, Er_ret);                 } else {                     Er__assertFail(Er_unwind("SessionManager.c", 485, Er_ret));                 }             }                  } while (0);
    do {             struct Er_Ret* Er_ret = Message_epush32h(eventMsg, ((uint32_t)__builtin_bswap32(((uint32_t)(0xffffffff)))));                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("SessionManager.c", 486, Er_ret);                 } else {                     Er__assertFail(Er_unwind("SessionManager.c", 486, Er_ret));                 }             }                  } while (0);
    do {             struct Er_Ret* Er_ret = Message_epush32h(eventMsg, ((uint32_t)__builtin_bswap32(((uint32_t)(PFChan_Core_UNSETUP_SESSION)))));                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("SessionManager.c", 487, Er_ret);                 } else {                     Er__assertFail(Er_unwind("SessionManager.c", 487, Er_ret));                 }             }                  } while (0);
    Iface_send(&sm->eventIf, eventMsg);
    Allocator__free((eventAlloc),"SessionManager.c",489);
}

static void triggerSearch(struct SessionManager_pvt* sm, uint8_t target[16], uint32_t version)
{
    struct Allocator* eventAlloc = Allocator__child((sm->alloc),"SessionManager.c",494);
    struct Message* eventMsg = Message_new(0, 512, eventAlloc);
    do {             struct Er_Ret* Er_ret = Message_epush32h(eventMsg, ((uint32_t)__builtin_bswap32(((uint32_t)(version)))));                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("SessionManager.c", 496, Er_ret);                 } else {                     Er__assertFail(Er_unwind("SessionManager.c", 496, Er_ret));                 }             }                  } while (0);
    do {             struct Er_Ret* Er_ret = Message_epush32h(eventMsg, ((uint32_t)__builtin_bswap32(((uint32_t)(0)))));                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("SessionManager.c", 497, Er_ret);                 } else {                     Er__assertFail(Er_unwind("SessionManager.c", 497, Er_ret));                 }             }                  } while (0);
    do {             struct Er_Ret* Er_ret = Message_epush(eventMsg, target, 16);                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("SessionManager.c", 498, Er_ret);                 } else {                     Er__assertFail(Er_unwind("SessionManager.c", 498, Er_ret));                 }             }                  } while (0);
    do {             struct Er_Ret* Er_ret = Message_epush32h(eventMsg, ((uint32_t)__builtin_bswap32(((uint32_t)(0xffffffff)))));                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("SessionManager.c", 499, Er_ret);                 } else {                     Er__assertFail(Er_unwind("SessionManager.c", 499, Er_ret));                 }             }                  } while (0);
    do {             struct Er_Ret* Er_ret = Message_epush32h(eventMsg, ((uint32_t)__builtin_bswap32(((uint32_t)(PFChan_Core_SEARCH_REQ)))));                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("SessionManager.c", 500, Er_ret);                 } else {                     Er__assertFail(Er_unwind("SessionManager.c", 500, Er_ret));                 }             }                  } while (0);
    Iface_send(&sm->eventIf, eventMsg);
    Allocator__free((eventAlloc),"SessionManager.c",502);
}

static void checkTimedOutSessions(struct SessionManager_pvt* sm)
{
    
# 507 "net/SessionManager.c" 3 4
   _Bool 
# 507 "net/SessionManager.c"
        searchTriggered = 
# 507 "net/SessionManager.c" 3 4
                          0
# 507 "net/SessionManager.c"
                               ;
    for (int i = (int)sm->ifaceMap.count - 1; i >= 0; i--) {
        struct SessionManager_Session_pvt* sess = sm->ifaceMap.values[i];
        int64_t now = Time_currentTimeMilliseconds(sm->eventBase);


        if (now - sess->pub.timeOfKeepAliveIn > sm->pub.sessionTimeoutMilliseconds) {
            do { if (!1) { break; } uint8_t sendPath[20]; uint8_t recvPath[20]; uint8_t ip[40]; AddrTools_printPath(sendPath, (sess)->pub.sendSwitchLabel); AddrTools_printPath(recvPath, (sess)->pub.recvSwitchLabel); AddrTools_printIp(ip, (sess)->pub.caSession->herIp6); do { if ((sm->log) && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print((sm->log), Log_Level_DEBUG, "SessionManager.c", 514, "Session[%p] sendPath[%s] recvPath[%s] ip[%s] " "%s", (void*)sess, sendPath, recvPath, ip, "ended"); } } while (0); } while (0);
            sendSession(sess, sess->pub.sendSwitchLabel, 0xffffffff, PFChan_Core_SESSION_ENDED);
            Map_OfSessionsByIp6_remove(i, &sm->ifaceMap);
            Allocator__free((sess->alloc),"SessionManager.c",517);
            continue;
        }

        if (!sess->pub.maintainSession) {

        } else if (now - sess->pub.lastSearchTime >= sm->pub.sessionSearchAfterMilliseconds) {



            if (searchTriggered && sess->pub.version < 20) { continue; }
            do { if (!1) { break; } uint8_t sendPath[20]; uint8_t recvPath[20]; uint8_t ip[40]; AddrTools_printPath(sendPath, (sess)->pub.sendSwitchLabel); AddrTools_printPath(recvPath, (sess)->pub.recvSwitchLabel); AddrTools_printIp(ip, (sess)->pub.caSession->herIp6); do { if ((sm->log) && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print((sm->log), Log_Level_DEBUG, "SessionManager.c", 528, "Session[%p] sendPath[%s] recvPath[%s] ip[%s] " "%s", (void*)sess, sendPath, recvPath, ip, "triggering search"); } } while (0); } while (0);
            triggerSearch(sm, sess->pub.caSession->herIp6, sess->pub.version);
            sess->pub.lastSearchTime = now;
            searchTriggered = 
# 531 "net/SessionManager.c" 3 4
                             1
# 531 "net/SessionManager.c"
                                 ;
        } else if (CryptoAuth_getState(sess->pub.caSession) < CryptoAuth_State_RECEIVED_KEY) {
            do { if (!1) { break; } uint8_t sendPath[20]; uint8_t recvPath[20]; uint8_t ip[40]; AddrTools_printPath(sendPath, (sess)->pub.sendSwitchLabel); AddrTools_printPath(recvPath, (sess)->pub.recvSwitchLabel); AddrTools_printIp(ip, (sess)->pub.caSession->herIp6); do { if ((sm->log) && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print((sm->log), Log_Level_DEBUG, "SessionManager.c", 533, "Session[%p] sendPath[%s] recvPath[%s] ip[%s] " "%s", (void*)sess, sendPath, recvPath, ip, "triggering unsetupSession"); } } while (0); } while (0);
            unsetupSession(sm, sess);
        }
    }
}

static void periodically(void* vSessionManager)
{
    struct SessionManager_pvt* sm = (__extension__ ({ __typeof__((struct SessionManager_pvt*) vSessionManager) Identity_ptr = ((struct SessionManager_pvt*) vSessionManager); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0xe6c7333919662984ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("SessionManager.c"), (541), "(Identity_ptr->Identity_verifier == ((unsigned long) 0xe6c7333919662984ull))"); } } while (0); Identity_ptr; }));
    checkTimedOutSessions(sm);
    checkTimedOutBuffers(sm);
}

static void needsLookup(struct SessionManager_pvt* sm, struct Message* msg, 
# 546 "net/SessionManager.c" 3 4
                                                                           _Bool 
# 546 "net/SessionManager.c"
                                                                                setupSession)
{
    do { if (!((msg->length >= ((56 + 12) + 4)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("SessionManager.c"), (548), "(msg->length >= ((56 + 12) + 4))"); } } while (0);
    struct RouteHeader* header = (struct RouteHeader*) msg->bytes;


    struct DataHeader* dataHeader = (struct DataHeader*) &header[1];
    do { if (!((DataHeader_getContentType(dataHeader) != ContentType_CJDHT))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("SessionManager.c"), (553), "(DataHeader_getContentType(dataHeader) != ContentType_CJDHT)"); } } while (0);

    if (1) {
        uint8_t ipStr[40];
        AddrTools_printIp(ipStr, header->ip6);
        do { if (sm->log && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(sm->log, Log_Level_DEBUG, "SessionManager.c", 558, "Buffering a packet to [%s] and beginning a search", ipStr); } } while (0);
    }
    int index = Map_BufferedMessages_indexForKey((struct Ip6*)header->ip6, &sm->bufMap);
    if (index > -1) {
        struct BufferedMessage* buffered = sm->bufMap.values[index];
        Map_BufferedMessages_remove(index, &sm->bufMap);
        Allocator__free((buffered->alloc),"SessionManager.c",564);
        do { if (sm->log && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(sm->log, Log_Level_DEBUG, "SessionManager.c", 565, "DROP message which needs lookup because new one received"); } } while (0);
    }
    if ((int)sm->bufMap.count >= sm->pub.maxBufferedMessages) {
        checkTimedOutBuffers(sm);
        if ((int)sm->bufMap.count >= sm->pub.maxBufferedMessages) {
            do { if (sm->log && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(sm->log, Log_Level_DEBUG, "SessionManager.c", 570, "DROP message needing lookup maxBufferedMessages ([%d]) is reached", sm->pub.maxBufferedMessages); } } while (0)
                                                  ;
            return;
        }
    }
    struct Allocator* lookupAlloc = Allocator__child((sm->alloc),"SessionManager.c",575);
    struct BufferedMessage* buffered =
        Allocator__calloc((lookupAlloc),(sizeof(struct BufferedMessage)),(1),"SessionManager.c",577);
    buffered->msg = msg;
    buffered->alloc = lookupAlloc;
    buffered->timeSentMilliseconds = Time_currentTimeMilliseconds(sm->eventBase);
    Allocator__adopt((lookupAlloc),(msg->alloc),"SessionManager.c",581);
    do { if (!((Map_BufferedMessages_put((struct Ip6*)header->ip6, &buffered, &sm->bufMap) > -1))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("SessionManager.c"), (582), "(Map_BufferedMessages_put((struct Ip6*)header->ip6, &buffered, &sm->bufMap) > -1)"); } } while (0);

    triggerSearch(sm, header->ip6, ((uint32_t)__builtin_bswap32(((uint32_t)(header->version_be)))));
}

static __attribute__ ((warn_unused_result)) struct Iface* readyToSend(struct Message* msg,
                               struct SessionManager_pvt* sm,
                               struct SessionManager_Session_pvt* sess)
{
    struct RouteHeader* header = (struct RouteHeader*) msg->bytes;
    struct DataHeader* dh = (struct DataHeader*) &header[1];
    if (DataHeader_getContentType(dh) != ContentType_CJDHT) {
        sess->pub.timeOfLastOut = Time_currentTimeMilliseconds(sm->eventBase);
    }
    do {             struct Er_Ret* Er_ret = Message_eshift(msg, -(56 + 12));                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("SessionManager.c", 596, Er_ret);                 } else {                     Er__assertFail(Er_unwind("SessionManager.c", 596, Er_ret));                 }             }                  } while (0);
    struct SwitchHeader* sh;
    CryptoAuth_resetIfTimeout(sess->pub.caSession);
    if (CryptoAuth_getState(sess->pub.caSession) < CryptoAuth_State_RECEIVED_KEY) {

        do {             struct Er_Ret* Er_ret = Message_epush32h(msg, ((uint32_t)__builtin_bswap32(((uint32_t)(sess->pub.receiveHandle)))));                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("SessionManager.c", 601, Er_ret);                 } else {                     Er__assertFail(Er_unwind("SessionManager.c", 601, Er_ret));                 }             }                  } while (0);


        do {             struct Er_Ret* Er_ret = Message_eshift(msg, (120 + 12));                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("SessionManager.c", 604, Er_ret);                 } else {                     Er__assertFail(Er_unwind("SessionManager.c", 604, Er_ret));                 }             }                  } while (0);
        Bits__memcpy(msg->bytes,&header->sh,12,"SessionManager.c",605);
        sh = (struct SwitchHeader*) msg->bytes;
        do {             struct Er_Ret* Er_ret = Message_eshift(msg, -(120 + 12));                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("SessionManager.c", 607, Er_ret);                 } else {                     Er__assertFail(Er_unwind("SessionManager.c", 607, Er_ret));                 }             }                  } while (0);
    } else {
        sh = &header->sh;
    }


    header = 
# 613 "net/SessionManager.c" 3 4
            ((void *)0)
# 613 "net/SessionManager.c"
                ;

    sess->pub.bytesOut += msg->length;

    do { if (!((!CryptoAuth_encrypt(sess->pub.caSession, msg)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("SessionManager.c"), (617), "(!CryptoAuth_encrypt(sess->pub.caSession, msg))"); } } while (0);

    if (CryptoAuth_getState(sess->pub.caSession) >= CryptoAuth_State_RECEIVED_KEY) {
        if (0) {
            do { if (!1) { break; } uint8_t path[20]; AddrTools_printPath(path, ((uint64_t)__builtin_bswap64(((uint64_t)(sh->label_be))))); uint8_t ip[40]; AddrTools_printIp(ip, sess->pub.caSession->herIp6); do { if (sm->log && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(sm->log, Log_Level_DEBUG, "SessionManager.c", 621, "ver[%u] send[%d] recv[%u] ip[%s] path[%s] " "%s", sess->pub.version, sess->pub.sendHandle, sess->pub.receiveHandle, ip, path, "sending run message"); } } while (0); } while (0)


                                                        ;
        }
        do {             struct Er_Ret* Er_ret = Message_epush32h(msg, ((uint32_t)__builtin_bswap32(((uint32_t)(sess->pub.sendHandle)))));                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("SessionManager.c", 626, Er_ret);                 } else {                     Er__assertFail(Er_unwind("SessionManager.c", 626, Er_ret));                 }             }                  } while (0);
    } else {
        do { if (!1) { break; } uint8_t path[20]; AddrTools_printPath(path, ((uint64_t)__builtin_bswap64(((uint64_t)(sh->label_be))))); uint8_t ip[40]; AddrTools_printIp(ip, sess->pub.caSession->herIp6); do { if (sm->log && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(sm->log, Log_Level_DEBUG, "SessionManager.c", 628, "ver[%u] send[%d] recv[%u] ip[%s] path[%s] " "%s", sess->pub.version, sess->pub.sendHandle, sess->pub.receiveHandle, ip, path, "sending start message"); } } while (0); } while (0)


                                                      ;
    }


    do {             struct Er_Ret* Er_ret = Message_eshift(msg, 12);                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("SessionManager.c", 635, Er_ret);                 } else {                     Er__assertFail(Er_unwind("SessionManager.c", 635, Er_ret));                 }             }                  } while (0);
    do { if (!(((uint8_t*)sh == msg->bytes))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("SessionManager.c"), (636), "((uint8_t*)sh == msg->bytes)"); } } while (0);

    if (!sh->label_be) {
        __builtin_memset(sh,0,12);
        sh->label_be = ((uint64_t)__builtin_bswap64(((uint64_t)(sess->pub.sendSwitchLabel))));
        SwitchHeader_setVersion(sh, 1);
    }

    return Iface_next(&sm->pub.switchIf, msg);
}

static __attribute__ ((warn_unused_result)) struct Iface* outgoingCtrlFrame(struct Message* msg, struct SessionManager_pvt* sm)
{
    do { if (!((msg->length >= (56 + 12)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("SessionManager.c"), (649), "(msg->length >= (56 + 12))"); } } while (0);
    struct RouteHeader* header = (struct RouteHeader*) msg->bytes;
    if (!Bits_isZero(header->publicKey, 32) || !Bits_isZero(header->ip6, 16)) {
        do { if (sm->log && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(sm->log, Log_Level_DEBUG, "SessionManager.c", 652, "DROP Ctrl frame with non-zero destination key or IP"); } } while (0);
        return 
# 653 "net/SessionManager.c" 3 4
              ((void *)0)
# 653 "net/SessionManager.c"
                  ;
    }
    if (!(header->flags & (1<<1))) {
        do { if (sm->log && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(sm->log, Log_Level_DEBUG, "SessionManager.c", 656, "DROP Ctrl frame w/o RouteHeader_flags_CTRLMSG flag"); } } while (0);
        return 
# 657 "net/SessionManager.c" 3 4
              ((void *)0)
# 657 "net/SessionManager.c"
                  ;
    }
    struct SwitchHeader sh;
    Bits__memcpy(&sh,&header->sh,12,"SessionManager.c",660);
    do {             struct Er_Ret* Er_ret = Message_epop(msg,    ((void *)0)   , (56 + 12));                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("SessionManager.c", 661, Er_ret);                 } else {                     Er__assertFail(Er_unwind("SessionManager.c", 661, Er_ret));                 }             }                  } while (0);
    do {             struct Er_Ret* Er_ret = Message_epush32h(msg, ((uint32_t)__builtin_bswap32(((uint32_t)(0xffffffff)))));                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("SessionManager.c", 662, Er_ret);                 } else {                     Er__assertFail(Er_unwind("SessionManager.c", 662, Er_ret));                 }             }                  } while (0);
    do {             struct Er_Ret* Er_ret = Message_epush(msg, &sh, 12);                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("SessionManager.c", 663, Er_ret);                 } else {                     Er__assertFail(Er_unwind("SessionManager.c", 663, Er_ret));                 }             }                  } while (0);
    return Iface_next(&sm->pub.switchIf, msg);
}

static __attribute__ ((warn_unused_result)) struct Iface* incomingFromInsideIf(struct Message* msg, struct Iface* iface)
{
    struct SessionManager_pvt* sm =
        (__extension__ ({ const __typeof__(((struct SessionManager_pvt*)0)->pub.insideIf)*__mptr = (iface); (__extension__ ({ __typeof__((struct SessionManager_pvt*)((char*)__mptr - 
# 670 "net/SessionManager.c" 3 4
       __builtin_offsetof (
# 670 "net/SessionManager.c"
       struct SessionManager_pvt
# 670 "net/SessionManager.c" 3 4
       , 
# 670 "net/SessionManager.c"
       pub.insideIf
# 670 "net/SessionManager.c" 3 4
       )
# 670 "net/SessionManager.c"
       )) Identity_ptr = ((struct SessionManager_pvt*)((char*)__mptr - 
# 670 "net/SessionManager.c" 3 4
       __builtin_offsetof (
# 670 "net/SessionManager.c"
       struct SessionManager_pvt
# 670 "net/SessionManager.c" 3 4
       , 
# 670 "net/SessionManager.c"
       pub.insideIf
# 670 "net/SessionManager.c" 3 4
       )
# 670 "net/SessionManager.c"
       )); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0xe6c7333919662984ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("SessionManager.c"), (670), "(Identity_ptr->Identity_verifier == ((unsigned long) 0xe6c7333919662984ull))"); } } while (0); Identity_ptr; })); }));
    do { if (!((msg->length >= (56 + 12)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("SessionManager.c"), (671), "(msg->length >= (56 + 12))"); } } while (0);
    struct RouteHeader* header = (struct RouteHeader*) msg->bytes;
    if (header->flags & (1<<1)) {
        return outgoingCtrlFrame(msg, sm);
    }
    do { if (!((msg->length >= (56 + 12) + 4))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("SessionManager.c"), (676), "(msg->length >= (56 + 12) + 4)"); } } while (0);
    struct DataHeader* dataHeader = (struct DataHeader*) &header[1];

    struct SessionManager_Session_pvt* sess = sessionForIp6(header->ip6, sm);
    if (!sess) {
        if (!Bits_isZero(header->publicKey, 32) && header->version_be) {
            sess = getSession(sm,
                              header->ip6,
                              header->publicKey,
                              ((uint32_t)__builtin_bswap32(((uint32_t)(header->version_be)))),
                              ((uint64_t)__builtin_bswap64(((uint64_t)(header->sh.label_be)))),
                              0xfffffff0,
                              !(header->flags & (1<<2)));
        } else {
            needsLookup(sm, msg, 
# 690 "net/SessionManager.c" 3 4
                                0
# 690 "net/SessionManager.c"
                                     );
            return 
# 691 "net/SessionManager.c" 3 4
                  ((void *)0)
# 691 "net/SessionManager.c"
                      ;
        }
    }

    if (header->version_be) { sess->pub.version = ((uint32_t)__builtin_bswap32(((uint32_t)(header->version_be)))); }

    if (!sess->pub.version) {
        needsLookup(sm, msg, 
# 698 "net/SessionManager.c" 3 4
                            0
# 698 "net/SessionManager.c"
                                 );
        return 
# 699 "net/SessionManager.c" 3 4
              ((void *)0)
# 699 "net/SessionManager.c"
                  ;
    }

    if (header->sh.label_be) {

    } else if (sess->pub.sendSwitchLabel) {
        __builtin_memset(&header->sh,0,12);
        header->sh.label_be = ((uint64_t)__builtin_bswap64(((uint64_t)(sess->pub.sendSwitchLabel))));
        SwitchHeader_setVersion(&header->sh, 1);
    } else {
        needsLookup(sm, msg, 
# 709 "net/SessionManager.c" 3 4
                            0
# 709 "net/SessionManager.c"
                                 );
        return 
# 710 "net/SessionManager.c" 3 4
              ((void *)0)
# 710 "net/SessionManager.c"
                  ;
    }


    CryptoAuth_resetIfTimeout(sess->pub.caSession);
    if (DataHeader_getContentType(dataHeader) != ContentType_CJDHT &&
        CryptoAuth_getState(sess->pub.caSession) < CryptoAuth_State_RECEIVED_KEY)
    {
        needsLookup(sm, msg, 
# 718 "net/SessionManager.c" 3 4
                            1
# 718 "net/SessionManager.c"
                                );
        return 
# 719 "net/SessionManager.c" 3 4
              ((void *)0)
# 719 "net/SessionManager.c"
                  ;
    }

    return readyToSend(msg, sm, sess);
}

static __attribute__ ((warn_unused_result)) struct Iface* sessions(struct SessionManager_pvt* sm,
                            uint32_t sourcePf,
                            struct Allocator* tempAlloc)
{
    for (int i = 0; i < (int)sm->ifaceMap.count; i++) {
        struct SessionManager_Session_pvt* sess = sm->ifaceMap.values[i];
        sendSession(sess, sess->pub.sendSwitchLabel, sourcePf, PFChan_Core_SESSION);
    }
    return 
# 733 "net/SessionManager.c" 3 4
          ((void *)0)
# 733 "net/SessionManager.c"
              ;
}

static __attribute__ ((warn_unused_result)) struct Iface* incomingFromEventIf(struct Message* msg, struct Iface* iface)
{
    struct SessionManager_pvt* sm = (__extension__ ({ const __typeof__(((struct SessionManager_pvt*)0)->eventIf)*__mptr = (iface); (__extension__ ({ __typeof__((struct SessionManager_pvt*)((char*)__mptr - 
# 738 "net/SessionManager.c" 3 4
                                   __builtin_offsetof (
# 738 "net/SessionManager.c"
                                   struct SessionManager_pvt
# 738 "net/SessionManager.c" 3 4
                                   , 
# 738 "net/SessionManager.c"
                                   eventIf
# 738 "net/SessionManager.c" 3 4
                                   )
# 738 "net/SessionManager.c"
                                   )) Identity_ptr = ((struct SessionManager_pvt*)((char*)__mptr - 
# 738 "net/SessionManager.c" 3 4
                                   __builtin_offsetof (
# 738 "net/SessionManager.c"
                                   struct SessionManager_pvt
# 738 "net/SessionManager.c" 3 4
                                   , 
# 738 "net/SessionManager.c"
                                   eventIf
# 738 "net/SessionManager.c" 3 4
                                   )
# 738 "net/SessionManager.c"
                                   )); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0xe6c7333919662984ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("SessionManager.c"), (738), "(Identity_ptr->Identity_verifier == ((unsigned long) 0xe6c7333919662984ull))"); } } while (0); Identity_ptr; })); }));
    enum PFChan_Pathfinder ev = (__extension__({             uint32_t Er_returnVal;             __builtin_memset(&Er_returnVal, 0, sizeof(Er_returnVal));             struct Er_Ret* Er_ret = Message_epop32be(&Er_returnVal, msg);                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("SessionManager.c", 739, Er_ret);                 } else {                     Er__assertFail(Er_unwind("SessionManager.c", 739, Er_ret));                 }             }                      Er_returnVal;         }));
    uint32_t sourcePf = (__extension__({             uint32_t Er_returnVal;             __builtin_memset(&Er_returnVal, 0, sizeof(Er_returnVal));             struct Er_Ret* Er_ret = Message_epop32be(&Er_returnVal, msg);                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("SessionManager.c", 740, Er_ret);                 } else {                     Er__assertFail(Er_unwind("SessionManager.c", 740, Er_ret));                 }             }                      Er_returnVal;         }));
    if (ev == PFChan_Pathfinder_SESSIONS) {
        do { if (!((!msg->length))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("SessionManager.c"), (742), "(!msg->length)"); } } while (0);
        return sessions(sm, sourcePf, msg->alloc);
    }
    do { if (!((ev == PFChan_Pathfinder_NODE))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("SessionManager.c"), (745), "(ev == PFChan_Pathfinder_NODE)"); } } while (0);

    struct PFChan_Node node;
    do {             struct Er_Ret* Er_ret = Message_epop(msg, &node, 64);                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("SessionManager.c", 748, Er_ret);                 } else {                     Er__assertFail(Er_unwind("SessionManager.c", 748, Er_ret));                 }             }                  } while (0);
    do { if (!((!msg->length))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("SessionManager.c"), (749), "(!msg->length)"); } } while (0);
    int index = Map_BufferedMessages_indexForKey((struct Ip6*)node.ip6, &sm->bufMap);
    struct SessionManager_Session_pvt* sess = sessionForIp6(node.ip6, sm);
    if (!sess) {

        if (index == -1) { return 
# 754 "net/SessionManager.c" 3 4
                                 ((void *)0)
# 754 "net/SessionManager.c"
                                     ; }

        if (node.metric_be == 0xffffffff) { return 
# 756 "net/SessionManager.c" 3 4
                                                  ((void *)0)
# 756 "net/SessionManager.c"
                                                      ; }
    }
    sess = getSession(sm,
                      node.ip6,
                      node.publicKey,
                      ((uint32_t)__builtin_bswap32(((uint32_t)(node.version_be)))),
                      ((uint64_t)__builtin_bswap64(((uint64_t)(node.path_be)))),
                      ((uint32_t)__builtin_bswap32(((uint32_t)(node.metric_be)))),
                      0);


    if (index > -1 && CryptoAuth_getState(sess->pub.caSession) >= CryptoAuth_State_RECEIVED_KEY) {
        struct BufferedMessage* bm = sm->bufMap.values[index];
        do { do { if (!((!bm->msg->currentIface))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("SessionManager.c"), (769), "(!bm->msg->currentIface)"); } } while (0); struct Iface Iface_y = { .currentMsg = bm->msg }; bm->msg->currentIface = &Iface_y; struct Iface* Iface_x = readyToSend(bm->msg, sm, sess); bm->msg->currentIface = 
# 769 "net/SessionManager.c" 3 4
       ((void *)0)
# 769 "net/SessionManager.c"
       ; if (Iface_x) { Iface_send(Iface_x, bm->msg); } } while (0);
        Map_BufferedMessages_remove(index, &sm->bufMap);
        Allocator__free((bm->alloc),"SessionManager.c",771);
    } else if (CryptoAuth_getState(sess->pub.caSession) < CryptoAuth_State_RECEIVED_KEY) {
        unsetupSession(sm, sess);
    }
    return 
# 775 "net/SessionManager.c" 3 4
          ((void *)0)
# 775 "net/SessionManager.c"
              ;
}

struct SessionManager* SessionManager_new(struct Allocator* allocator,
                                          struct EventBase* eventBase,
                                          struct CryptoAuth* cryptoAuth,
                                          struct Random* rand,
                                          struct Log* log,
                                          struct EventEmitter* ee)
{
    struct Allocator* alloc = Allocator__child((allocator),"SessionManager.c",785);
    struct SessionManager_pvt* sm = Allocator__calloc((alloc),(sizeof(struct SessionManager_pvt)),(1),"SessionManager.c",786);
    sm->alloc = alloc;
    sm->pub.switchIf.send = incomingFromSwitchIf;
    sm->pub.insideIf.send = incomingFromInsideIf;
    sm->bufMap.allocator = alloc;
    sm->ifaceMap.allocator = alloc;
    sm->log = log;
    sm->cryptoAuth = cryptoAuth;
    sm->eventBase = eventBase;
    sm->pub.sessionTimeoutMilliseconds = 120000;
    sm->pub.maxBufferedMessages = 30;
    sm->pub.sessionSearchAfterMilliseconds =
        30000;

    sm->eventIf.send = incomingFromEventIf;
    EventEmitter_regCore(ee, &sm->eventIf, PFChan_Pathfinder_NODE);
    EventEmitter_regCore(ee, &sm->eventIf, PFChan_Pathfinder_SESSIONS);

    sm->firstHandle =
        (Random_uint32(rand) % (100000 - 4)) + 4;

    Timeout__setInterval((periodically), (sm), (10000), (eventBase), (alloc), "SessionManager.c", 807);

    (sm)->Identity_verifier = ((unsigned long) 0xe6c7333919662984ull);

    return &sm->pub;
}
