# 1 "util/platform/netdev/NetDev.c"
# 1 "/home/jojapoppa/cjdns//"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "util/platform/netdev/NetDev.c"
# 15 "util/platform/netdev/NetDev.c"
# 1 "./util/platform/Sockaddr.h" 1
# 18 "./util/platform/Sockaddr.h"
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
struct Linker_x08670821392099848;
# 33 "./util/Assert.h"
__attribute__((__format__ (__printf__, 1, 2)))
__attribute__((__noreturn__))
void Assert_failure(const char* format, ...);
# 19 "./util/Identity.h" 2
# 1 "./util/Constant.h" 1
# 33 "./util/Constant.h"

# 20 "./util/Identity.h" 2



    
# 19 "./memory/Allocator.h" 2


struct Linker_x05226399510083695;





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
# 19 "./util/platform/Sockaddr.h" 2
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
# 20 "./util/platform/Sockaddr.h" 2


# 21 "./util/platform/Sockaddr.h"
struct Linker_x09277314841295019;



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
# 16 "util/platform/netdev/NetDev.c" 2
# 1 "./util/log/Log.h" 1
# 20 "./util/log/Log.h"
struct Linker_x04576612766520065;

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
# 17 "util/platform/netdev/NetDev.c" 2
# 1 "./exception/Except.h" 1
# 18 "./exception/Except.h"
# 1 "./exception/Er.h" 1
# 23 "./exception/Er.h"
struct Linker_x05585669285456061;

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


struct Linker_x05767075390753231;



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
# 18 "util/platform/netdev/NetDev.c" 2

# 1 "./memory/BufferAllocator.h" 1
# 19 "./memory/BufferAllocator.h"
# 1 "./util/UniqueName.h" 1
# 22 "./util/UniqueName.h"

# 20 "./memory/BufferAllocator.h" 2


struct Linker_x05513455261945455;
# 34 "./memory/BufferAllocator.h"
struct Allocator* BufferAllocator__new(void* buffer,
                                       unsigned long length,
                                       char* file,
                                       int line);
# 20 "util/platform/netdev/NetDev.c" 2
# 1 "./util/platform/netdev/NetDev.h" 1
# 22 "./util/platform/netdev/NetDev.h"
struct Linker_x00973274757811522;
# 34 "./util/platform/netdev/NetDev.h"
__attribute__ ((warn_unused_result)) struct Er_Ret* NetDev_addAddress(const char* ifName, struct Sockaddr* sa, struct Log* logger, struct Allocator* alloc)


                                                ;
# 47 "./util/platform/netdev/NetDev.h"
__attribute__ ((warn_unused_result)) struct Er_Ret* NetDev_setMTU(const char* interfaceName, uint32_t mtu, struct Log* logger, struct Allocator* alloc)


                                            ;

__attribute__ ((warn_unused_result)) struct Er_Ret* NetDev_flushAddresses(const char* deviceName, struct Allocator* alloc);




__attribute__ ((warn_unused_result)) struct Er_Ret* NetDev_setRoutes(const char* ifName, struct Sockaddr** prefixSet, int prefixCount, struct Log* logger, struct Allocator* tempAlloc)



                                                   ;
# 21 "util/platform/netdev/NetDev.c" 2
# 1 "./util/platform/netdev/NetPlatform.h" 1
# 23 "./util/platform/netdev/NetPlatform.h"
struct Linker_x014580912429745863;



__attribute__ ((warn_unused_result)) struct Er_Ret* NetPlatform_flushAddresses(const char* deviceName, struct Allocator* alloc);

__attribute__ ((warn_unused_result)) struct Er_Ret* NetPlatform_addAddress(const char* interfaceName, const uint8_t* address, int prefixLen, int addrFam, struct Log* logger, struct Allocator* tempAlloc)




                                                         ;

__attribute__ ((warn_unused_result)) struct Er_Ret* NetPlatform_setMTU(const char* interfaceName, uint32_t mtu, struct Log* logger, struct Allocator* errAlloc)


                                                    ;

__attribute__ ((warn_unused_result)) struct Er_Ret* NetPlatform_setRoutes(const char* ifName, struct Sockaddr** prefixSet, int prefixCount, struct Log* logger, struct Allocator* tempAlloc)



                                                        ;
# 22 "util/platform/netdev/NetDev.c" 2

static __attribute__ ((warn_unused_result)) struct Er_Ret* checkAddressAndPrefix(struct Sockaddr* sa, int* addrFam, char** printedAddr, void** addr, struct Allocator* alloc)




{
    *printedAddr = Sockaddr_print(sa, alloc);
    *addrFam = Sockaddr_getFamily(sa);
    if (*addrFam != Sockaddr_AF_INET && *addrFam != Sockaddr_AF_INET6) {
        do { struct Er_Ret* Er_ret = Er__raise("NetDev.c", 32, alloc, "Unknown address type for address [%s]", *printedAddr); return Er_ret;; } while (0);
    }
    int prefixMax = (*addrFam == Sockaddr_AF_INET6) ? 128 : 32;
    if (!(sa->flags & (1<<1))) {
        sa->prefix = prefixMax;
    }
    if (sa->prefix > prefixMax) {
        do { struct Er_Ret* Er_ret = Er__raise("NetDev.c", 39, alloc, "prefix [%u] must be less than %d", sa->prefix, prefixMax); return Er_ret;; } while (0);
    }

    int len = Sockaddr_getAddress(sa, addr);
    if (len < 0 || len != prefixMax / 8) {
        do { struct Er_Ret* Er_ret = Er__raise("NetDev.c", 44, alloc, "Invalid sockaddr [%s]", *printedAddr); return Er_ret;; } while (0);
    }
    return (struct Er_Ret*)0;
}

__attribute__ ((warn_unused_result)) struct Er_Ret* NetDev_addAddress(const char* ifName, struct Sockaddr* sa, struct Log* logger, struct Allocator* alloc)



{
    int addrFam;
    char* printedAddr;
    void* addr;

    do {             struct Er_Ret* Er_ret = checkAddressAndPrefix(sa, &addrFam, &printedAddr, &addr, alloc);             if (Er_ret) { return Er_unwind("NetDev.c", 58, Er_ret); }         } while (0);

    do { if (logger && Log_Level_INFO >= Log_Level_DEBUG) { Log_print(logger, Log_Level_INFO, "NetDev.c", 60, "Setting IP address [%s] on interface [%s]", printedAddr, ifName); } } while (0)
                                 ;

    do {             struct Er_Ret* Er_ret = NetPlatform_addAddress(ifName, addr, sa->prefix, addrFam, logger, alloc);             if (Er_ret) { return Er_unwind("NetDev.c", 63, Er_ret); }         } while (0);
    return (struct Er_Ret*)0;
}

__attribute__ ((warn_unused_result)) struct Er_Ret* NetDev_setMTU(const char* interfaceName, uint32_t mtu, struct Log* logger, struct Allocator* alloc)



{
     do {             struct Er_Ret* Er_ret = NetPlatform_setMTU(interfaceName, mtu, logger, alloc);             if (Er_ret) { return Er_unwind("NetDev.c", 72, Er_ret); }         } while (0);
     return (struct Er_Ret*)0;
}

__attribute__ ((warn_unused_result)) struct Er_Ret* NetDev_flushAddresses(const char* deviceName, struct Allocator* alloc)
{

        do {             struct Er_Ret* Er_ret = NetPlatform_flushAddresses(deviceName, alloc);             if (Er_ret) { return Er_unwind("NetDev.c", 79, Er_ret); }         } while (0);

    return (struct Er_Ret*)0;
}
# 104 "util/platform/netdev/NetDev.c"
__attribute__ ((warn_unused_result)) struct Er_Ret* NetDev_setRoutes(const char* ifName, struct Sockaddr** prefixSet, int prefixCount, struct Log* logger, struct Allocator* tempAlloc)




{
    for (int i = 0; i < prefixCount; i++) {
        struct Allocator* alloc = Allocator__child((tempAlloc),"NetDev.c",111);
        int addrFam;
        char* printedAddr;
        void* addr;
        do {             struct Er_Ret* Er_ret = checkAddressAndPrefix(prefixSet[i], &addrFam, &printedAddr, &addr, alloc);             if (Er_ret) { return Er_unwind("NetDev.c", 115, Er_ret); }         } while (0);
        Allocator__free((alloc),"NetDev.c",116);
    }

    do {             struct Er_Ret* Er_ret = NetPlatform_setRoutes(ifName, prefixSet, prefixCount, logger, tempAlloc);             if (Er_ret) { return Er_unwind("NetDev.c", 119, Er_ret); }         } while (0);
    return (struct Er_Ret*)0;
}
