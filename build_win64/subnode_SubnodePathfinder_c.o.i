# 1 "subnode/SubnodePathfinder.c"
# 1 "/home/jojapoppa/cjdns//"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "subnode/SubnodePathfinder.c"
# 15 "subnode/SubnodePathfinder.c"
# 1 "./subnode/SubnodePathfinder.h" 1
# 18 "./subnode/SubnodePathfinder.h"
# 1 "./admin/Admin.h" 1
# 18 "./admin/Admin.h"
# 1 "./interface/Iface.h" 1
# 18 "./interface/Iface.h"
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
# 19 "./interface/Iface.h" 2

# 1 "./wire/Message.h" 1
# 18 "./wire/Message.h"
# 1 "./exception/Er.h" 1
# 18 "./exception/Er.h"
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
struct Linker_x035283125813315563;
# 33 "./util/Assert.h"
__attribute__((__format__ (__printf__, 1, 2)))
__attribute__((__noreturn__))
void Assert_failure(const char* format, ...);
# 19 "./util/Identity.h" 2
# 1 "./util/Constant.h" 1
# 33 "./util/Constant.h"

# 20 "./util/Identity.h" 2



    
# 19 "./memory/Allocator.h" 2


struct Linker_x0767384465313387;





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
# 19 "./exception/Er.h" 2




struct Linker_x06443921661986571;

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
struct Linker_x07421407289531796;


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

struct Linker_x09526136922732267;



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
# 19 "./interface/addressable/AddrIface.h"
# 1 "./util/platform/Sockaddr.h" 1
# 21 "./util/platform/Sockaddr.h"
struct Linker_x08918705106155027;



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
# 20 "./interface/addressable/AddrIface.h" 2
# 1 "./exception/Except.h" 1
# 21 "./exception/Except.h"
struct Linker_x008836002861500081;



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
# 21 "./interface/addressable/AddrIface.h" 2







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
# 1 "./benc/Dict.h" 1
# 19 "./benc/Dict.h"
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
# 20 "./benc/Dict.h" 2

struct Linker_x02641678853262355;

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
# 21 "./admin/Admin.h" 2


# 1 "./util/log/Log.h" 1
# 20 "./util/log/Log.h"
struct Linker_x06207194619320606;

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
# 24 "./admin/Admin.h" 2
# 1 "./util/UniqueName.h" 1
# 22 "./util/UniqueName.h"

# 25 "./admin/Admin.h" 2
# 1 "./util/events/EventBase.h" 1
# 20 "./util/events/EventBase.h"
struct Linker_x040823348617891697;

struct EventBase
{
    int unused;
};

struct EventBase* EventBase_new(struct Allocator* alloc);

int EventBase_eventCount(struct EventBase* eventBase);

void EventBase_beginLoop(struct EventBase* eventBase);

void EventBase_endLoop(struct EventBase* eventBase);
# 26 "./admin/Admin.h" 2

struct Linker_x08806485940905904;

# 1 "/usr/lib/gcc/x86_64-w64-mingw32/9.2-win32/include/stdbool.h" 1 3 4
# 30 "./admin/Admin.h" 2

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
# 19 "./subnode/SubnodePathfinder.h" 2




# 1 "./crypto/random/Random.h" 1
# 21 "./crypto/random/Random.h"
# 1 "./crypto/random/seed/RandomSeed.h" 1
# 21 "./crypto/random/seed/RandomSeed.h"
struct Linker_x036156369886369943;



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

struct Linker_x002800210717373175;



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
# 24 "./subnode/SubnodePathfinder.h" 2
# 1 "./subnode/SupernodeHunter.h" 1
# 21 "./subnode/SupernodeHunter.h"
# 1 "./subnode/AddrSet.h" 1
# 19 "./subnode/AddrSet.h"
# 1 "./dht/Address.h" 1
# 18 "./dht/Address.h"
# 1 "./benc/String.h" 1
# 20 "./benc/String.h"
# 1 "./util/CString.h" 1
# 21 "./util/CString.h"
struct Linker_x006183582913635011;

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

struct Linker_x035595849771520616;


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


struct Linker_x01927455556590103;
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
# 20 "./subnode/AddrSet.h" 2

struct Linker_x0219423697514203;

struct AddrSet
{
    int length;
};


int AddrSet_indexOf(struct AddrSet* as, struct Address* addr);

void AddrSet_add(struct AddrSet* as, struct Address* addr);

void AddrSet_remove(struct AddrSet* as, struct Address* addr);

struct Address* AddrSet_get(struct AddrSet* as, int i);

struct AddrSet* AddrSet_new(struct Allocator* alloc);

void AddrSet_flush(struct AddrSet* as);
# 22 "./subnode/SupernodeHunter.h" 2
# 1 "./subnode/MsgCore.h" 1
# 24 "./subnode/MsgCore.h"
# 1 "./switch/EncodingScheme.h" 1
# 19 "./switch/EncodingScheme.h"
# 1 "./benc/List.h" 1
# 21 "./benc/List.h"
struct Linker_x06120168643750272;

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


struct Linker_x08177942869743111;



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


struct Linker_x05390371409758115;

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
# 23 "./subnode/SupernodeHunter.h" 2
# 1 "./subnode/ReachabilityCollector.h" 1
# 17 "./subnode/ReachabilityCollector.h"
# 1 "./subnode/BoilerplateResponder.h" 1
# 24 "./subnode/BoilerplateResponder.h"
struct Linker_x0606324924922083;

struct BoilerplateResponder
{
    int unused;
};

void BoilerplateResponder_addBoilerplate(struct BoilerplateResponder* br,
                                         Dict* msgDict,
                                         struct Address* src,
                                         struct Allocator* alloc);

struct BoilerplateResponder* BoilerplateResponder_new(struct EncodingScheme* myScheme,
                                                      struct Allocator* alloc);
# 18 "./subnode/ReachabilityCollector.h" 2



# 1 "./subnode/LinkState.h" 1
# 19 "./subnode/LinkState.h"
# 1 "./util/VarInt.h" 1
# 25 "./util/VarInt.h"
struct VarInt_Iter {
    uint8_t* ptr;
    uint8_t* end;
    uint8_t* start;
};

static inline void VarInt_clone(struct VarInt_Iter* out, const struct VarInt_Iter* in)
{
    out->ptr = in->ptr;
    out->start = in->start;
    out->end = in->end;
}

static inline void VarInt_mk(struct VarInt_Iter* out, uint8_t* ptr, int length)
{
    out->ptr = ptr;
    out->end = ptr + length;
    out->start = ptr;
}

static inline void VarInt_toStart(struct VarInt_Iter* iter)
{
    iter->ptr = iter->start;
}

static inline void VarInt_toEnd(struct VarInt_Iter* iter)
{
    iter->ptr = iter->end;
}

static inline int VarInt_hasMore(struct VarInt_Iter* iter)
{
    return iter->end > iter->ptr;
}

static inline int VarInt_sizeOf(uint64_t val)
{
    return (!!(val >> 32)) * 4 + (!!(val >> 16)) * 2 + (!!((val + 3) >> 8)) + 1;
}

static inline int VarInt_pop(struct VarInt_Iter* iter, uint64_t* _out)
{
    uint64_t out = 0;
    uint8_t* bytes = iter->ptr;
    int len = iter->end - bytes;
    if (len < 9) {
        if (len < 5) {
            if (len < 3) {
                if (len < 1) { return -1; }
                if (*bytes >= 0xfd) { return -1; }
            } else if (*bytes >= 0xfe) { return -1; }
        } else if (*bytes >= 0xff) { return -1; }
    }
    switch (*bytes) {
        case 0xff:
            out |= *++bytes; out <<= 8;
            out |= *++bytes; out <<= 8;
            out |= *++bytes; out <<= 8;
            out |= *++bytes; out <<= 8;
            __attribute__((fallthrough));
        case 0xfe:
            out |= *++bytes; out <<= 8;
            out |= *++bytes; out <<= 8;
            __attribute__((fallthrough));
        case 0xfd:
            out |= *++bytes; out <<= 8;
            bytes++;
            __attribute__((fallthrough));
        default:
            out |= *bytes++;
    }
    iter->ptr = bytes;
    if (_out) { *_out = out; }
    return 0;
}

static inline int VarInt_push(struct VarInt_Iter* iter, uint64_t val)
{
    uint8_t* ptr = iter->ptr;
    int padding = ptr - iter->start;
    if (padding < 9) {
        if (padding < 5) {
            if (padding < 3) {
                if (padding < 1) { return -1; }
                if (val > 0xfc) { return -1; }
            } else if (val > 0xffff) { return -1; }
        } else if (val > 0xffffffff) { return -1; }
    }
    int i = VarInt_sizeOf(val);
    for (int j = 0; j < i; j++) { *--ptr = val & 0xff; val >>= 8; }
    switch (i) {
        case 8: *--ptr = 0xff; break;
        case 4: *--ptr = 0xfe; break;
        case 2: *--ptr = 0xfd; break;
    }
    iter->ptr = ptr;
    return 0;
}
# 20 "./subnode/LinkState.h" 2

# 1 "./wire/Announce.h" 1
# 18 "./wire/Announce.h"
# 1 "./util/version/Version.h" 1
# 19 "./util/version/Version.h"
struct Linker_x08097117269457983;
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
# 19 "./wire/Announce.h" 2
# 28 "./wire/Announce.h"
enum Announce_Type {
    Announce_Type_ENCODING_SCHEME,
    Announce_Type_PEER,
    Announce_Type_VERSION,
    Announce_Type_LINK_STATE
};
# 42 "./wire/Announce.h"
struct Announce_Version
{

    uint8_t length;


    uint8_t type;

    uint16_t version_be;
};

void Assert_compileTime(char x[1 - (!(sizeof(struct Announce_Version) == 4))]);

static inline void Announce_Version_init(struct Announce_Version* v)
{
    v->length = 4;
    v->type = Announce_Type_VERSION;
    v->version_be = ((uint16_t)(__extension__ ({ uint16_t _x = (((uint16_t)(20))); _x >> 8 | _x << 8; })));
}

struct Announce_EncodingScheme
{

    uint8_t length;


    uint8_t type;


    uint8_t scheme[2];
};

static inline void Announce_EncodingScheme_push(struct Message* pushTo, String* compressedScheme)
{
    do { if (!((compressedScheme->len + 2 < 256))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("Announce.h"), (76), "(compressedScheme->len + 2 < 256)"); } } while (0);
    do {             struct Er_Ret* Er_ret = Message_epush(pushTo, compressedScheme->bytes, compressedScheme->len);                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("Announce.h", 77, Er_ret);                 } else {                     Er__assertFail(Er_unwind("Announce.h", 77, Er_ret));                 }             }                  } while (0);
    do {             struct Er_Ret* Er_ret = Message_epush8h(pushTo, Announce_Type_ENCODING_SCHEME);                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("Announce.h", 78, Er_ret);                 } else {                     Er__assertFail(Er_unwind("Announce.h", 78, Er_ret));                 }             }                  } while (0);
    do {             struct Er_Ret* Er_ret = Message_epush8h(pushTo, compressedScheme->len + 2);                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("Announce.h", 79, Er_ret);                 } else {                     Er__assertFail(Er_unwind("Announce.h", 79, Er_ret));                 }             }                  } while (0);
    while ((uintptr_t)pushTo->bytes % 4) {
        do {             struct Er_Ret* Er_ret = Message_epush8h(pushTo, 1);                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("Announce.h", 81, Er_ret);                 } else {                     Er__assertFail(Er_unwind("Announce.h", 81, Er_ret));                 }             }                  } while (0);
    }
}
# 106 "./wire/Announce.h"
struct Announce_Peer
{

    uint8_t length;


    uint8_t type;


    uint8_t encodingFormNum;


    uint8_t flags;




    uint16_t mtu8_be;



    uint16_t peerNum_be;


    uint32_t unused;


    uint8_t ipv6[16];



    uint32_t label_be;
};

void Assert_compileTime(char x[1 - (!(sizeof(struct Announce_Peer) == 32))]);

static inline void Announce_Peer_init(struct Announce_Peer* peer)
{
    __builtin_memset(peer,0,32);
    peer->length = 32;
    peer->type = Announce_Type_PEER;
    peer->unused = 0xffffffff;
    peer->peerNum_be = 0xffff;
}
# 161 "./wire/Announce.h"
struct Announce_LinkState
{

    uint8_t length;


    uint8_t type;


    uint8_t padding;


    uint8_t linkState[1];
};

static inline void Announce_LinkState_applyHeader(struct Message* pushTo)
{
    Assert_failure("todo implement");
}

struct Announce_ItemHeader
{
    uint8_t length;
    uint8_t type;
};
# 252 "./wire/Announce.h"
struct Announce_Header
{

    uint8_t signature[64];


    uint8_t pubSigningKey[32];






    uint8_t snodeIp[16];


    uint8_t timeStampVersionFlags_be[8];
};

void Assert_compileTime(char x[1 - (!(sizeof(struct Announce_Header) == 120))]);

static inline int64_t Announce_Header_getTimestamp(struct Announce_Header* hdr)
{
    uint64_t ts_be;
    Bits__memcpy(&ts_be,hdr->timeStampVersionFlags_be,sizeof(uint64_t),"Announce.h",276);
    return ((uint64_t)__builtin_bswap64(((uint64_t)(ts_be)))) >> 4;
}

static inline void Announce_Header_setTimestamp(struct Announce_Header* hdr,
                                                    int64_t timestamp)
{
    uint64_t uTime = (uint64_t) timestamp;



    do { if (!((!(uTime >> 60)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("Announce.h"), (287), "(!(uTime >> 60))"); } } while (0);

    uint64_t ts_be;
    Bits__memcpy(&ts_be,hdr->timeStampVersionFlags_be,sizeof(uint64_t),"Announce.h",290);
    ts_be = (ts_be & ((uint64_t)__builtin_bswap64(((uint64_t)(0x0f))))) | ((uint64_t)__builtin_bswap64(((uint64_t)(uTime << 4))));
    Bits__memcpy(hdr->timeStampVersionFlags_be,&ts_be,sizeof(uint64_t),"Announce.h",292);
}

static inline 
# 295 "./wire/Announce.h" 3 4
             _Bool 
# 295 "./wire/Announce.h"
                  Announce_Header_isReset(struct Announce_Header* hdr)
{
    uint64_t ts_be;
    Bits__memcpy(&ts_be,hdr->timeStampVersionFlags_be,sizeof(uint64_t),"Announce.h",298);
    return (((uint64_t)__builtin_bswap64(((uint64_t)(ts_be)))) >> 3) & 1;
}

static inline void Announce_Header_setReset(struct Announce_Header* hdr, 
# 302 "./wire/Announce.h" 3 4
                                                                        _Bool 
# 302 "./wire/Announce.h"
                                                                             isReset)
{
    uint64_t ts_be;
    Bits__memcpy(&ts_be,hdr->timeStampVersionFlags_be,sizeof(uint64_t),"Announce.h",305);
    if (isReset) {
        ts_be |= ((uint64_t)__builtin_bswap64(((uint64_t)(1<<3))));
    } else {
        ts_be &= ~((uint64_t)__builtin_bswap64(((uint64_t)(1<<3))));
    }
    Bits__memcpy(hdr->timeStampVersionFlags_be,&ts_be,sizeof(uint64_t),"Announce.h",311);
}

static inline int Announce_Header_getVersion(struct Announce_Header* hdr)
{
    uint64_t ts_be;
    Bits__memcpy(&ts_be,hdr->timeStampVersionFlags_be,sizeof(uint64_t),"Announce.h",317);
    return ((uint64_t)__builtin_bswap64(((uint64_t)(ts_be)))) & 0x07;
}


static inline void Announce_Header_setVersion(struct Announce_Header* hdr, int version)
{
    uint64_t ts_be;
    Bits__memcpy(&ts_be,hdr->timeStampVersionFlags_be,sizeof(uint64_t),"Announce.h",325);
    ts_be = (ts_be & ~((uint64_t)__builtin_bswap64(((uint64_t)(0x07))))) | ((uint64_t)__builtin_bswap64(((uint64_t)(version & 0x07))));
    Bits__memcpy(hdr->timeStampVersionFlags_be,&ts_be,sizeof(uint64_t),"Announce.h",327);
}

static inline struct Announce_ItemHeader* Announce_ItemHeader_next(struct Message* msg, void* last)
{
    struct Announce_ItemHeader* ih = (struct Announce_ItemHeader*) last;
    if (ih) {
        do { if (!(((uint8_t*)ih > &msg->bytes[-msg->padding]))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("Announce.h"), (334), "((uint8_t*)ih > &msg->bytes[-msg->padding])"); } } while (0);
        do { if (!(((uint8_t*)ih < &msg->bytes[msg->length]))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("Announce.h"), (335), "((uint8_t*)ih < &msg->bytes[msg->length])"); } } while (0);
        ih = (struct Announce_ItemHeader*) ( &((uint8_t*) ih)[ih->length] );
    } else {
        ih = (struct Announce_ItemHeader*) &msg->bytes[120];
    }
    while ((uint8_t*)ih < &msg->bytes[msg->length]) {
        if (!ih->length) { return 
# 341 "./wire/Announce.h" 3 4
                                 ((void *)0)
# 341 "./wire/Announce.h"
                                     ; }
        if (ih->length > 1) {
            if ( &((uint8_t*) ih)[ih->length] > &msg->bytes[msg->length] ) {

                return 
# 345 "./wire/Announce.h" 3 4
                      ((void *)0)
# 345 "./wire/Announce.h"
                          ;
            }
            return ih;
        }
        ih = (struct Announce_ItemHeader*) ( &((uint8_t*) ih)[ih->length] );
    }
    return 
# 351 "./wire/Announce.h" 3 4
          ((void *)0)
# 351 "./wire/Announce.h"
              ;
}

static inline 
# 354 "./wire/Announce.h" 3 4
             _Bool 
# 354 "./wire/Announce.h"
                  Announce_isValid(struct Message* msg)
{
    struct Announce_ItemHeader* ih = 
# 356 "./wire/Announce.h" 3 4
                                    ((void *)0)
# 356 "./wire/Announce.h"
                                        ;
    for (;;) {
        ih = Announce_ItemHeader_next(msg, ih);
        if (!ih) { return 
# 359 "./wire/Announce.h" 3 4
                         0
# 359 "./wire/Announce.h"
                              ; }
        if ((uint8_t*)ih == &msg->bytes[msg->length - ih->length]) { return 
# 360 "./wire/Announce.h" 3 4
                                                                           1
# 360 "./wire/Announce.h"
                                                                               ; }
    }
}

static inline struct Announce_Peer* Announce_Peer_next(struct Message* msg, void* last)
{
    struct Announce_ItemHeader* ih = (struct Announce_ItemHeader*) last;
    do {
        ih = Announce_ItemHeader_next(msg, ih);
    } while (ih && ih->type != Announce_Type_PEER);
    return (struct Announce_Peer*) ih;
}
# 22 "./subnode/LinkState.h" 2






struct LinkState {
    uint16_t lagSlots[18];
    uint16_t dropSlots[18];
    uint32_t kbRecvSlots[18];
    uint32_t samples;
    uint16_t nodeId;
    uint16_t _pad;
};

static inline int LinkState_encode(
    struct Message* msg,
    struct LinkState* ls,
    uint32_t lastSamples)
{



    if (msg->padding < 255) { return 1; }

    struct VarInt_Iter iter = {
        .ptr = msg->bytes,
        .end = msg->bytes,
        .start = &msg->bytes[-msg->padding]
    };


    uint32_t i = ls->samples - 1;
    int count = 0;
    int err = 0;
    for (; i >= lastSamples && count < 18; i--, count++) {
        int idx = i % 18;
        err |= VarInt_push(&iter, ls->kbRecvSlots[idx]);
        err |= VarInt_push(&iter, ls->dropSlots[idx]);
        err |= VarInt_push(&iter, ls->lagSlots[idx]);
    }
    if (err) { return 1; }
    if (!count) { return 0; }


    do { if (!((!VarInt_push(&iter, (i + 1) % 18)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("LinkState.h"), (67), "(!VarInt_push(&iter, (i + 1) % 18))"); } } while (0);
    do { if (!((!VarInt_push(&iter, ls->nodeId)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("LinkState.h"), (68), "(!VarInt_push(&iter, ls->nodeId))"); } } while (0);

    int beginLength = msg->length;
    do {             struct Er_Ret* Er_ret = Message_eshift(msg, (msg->bytes - iter.ptr));                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("LinkState.h", 71, Er_ret);                 } else {                     Er__assertFail(Er_unwind("LinkState.h", 71, Er_ret));                 }             }                  } while (0);
    do { if (!((msg->bytes == iter.ptr))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("LinkState.h"), (72), "(msg->bytes == iter.ptr)"); } } while (0);

    int padCount = 0;
    while ((uintptr_t)(&msg->bytes[-3]) & 7) {
        do {             struct Er_Ret* Er_ret = Message_epush8h(msg, 0);                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("LinkState.h", 76, Er_ret);                 } else {                     Er__assertFail(Er_unwind("LinkState.h", 76, Er_ret));                 }             }                  } while (0);
        padCount++;
    }
    do {             struct Er_Ret* Er_ret = Message_epush8h(msg, padCount);                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("LinkState.h", 79, Er_ret);                 } else {                     Er__assertFail(Er_unwind("LinkState.h", 79, Er_ret));                 }             }                  } while (0);

    do {             struct Er_Ret* Er_ret = Message_epush8h(msg, Announce_Type_LINK_STATE);                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("LinkState.h", 81, Er_ret);                 } else {                     Er__assertFail(Er_unwind("LinkState.h", 81, Er_ret));                 }             }                  } while (0);
    int finalLength = msg->length - beginLength;
    do {             struct Er_Ret* Er_ret = Message_epush8h(msg, finalLength + 1);                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("LinkState.h", 83, Er_ret);                 } else {                     Er__assertFail(Er_unwind("LinkState.h", 83, Er_ret));                 }             }                  } while (0);

    do { if (!((!(((uintptr_t)msg->bytes) & 7)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("LinkState.h"), (85), "(!(((uintptr_t)msg->bytes) & 7))"); } } while (0);
    return 0;
}

static inline int LinkState_mkDecoder(struct Message* msg, struct VarInt_Iter* it)
{
    if (!msg->length) { return 1; }
    uint8_t len = msg->bytes[0];
    if (msg->length < len) { return 1; }
    if (len < 3) { return 1; }
    it->ptr = &msg->bytes[1];
    it->start = it->ptr;
    it->end = &msg->bytes[len];

    uint64_t type = 0;
    if (VarInt_pop(it, &type)) { return 1; }
    if (type != Announce_Type_LINK_STATE) { return 1; }

    uint64_t padCount = 0;
    if (VarInt_pop(it, &padCount)) { return 1; }
    for (unsigned int i = 0; i < padCount; i++) {
        if (VarInt_pop(it, 
# 106 "./subnode/LinkState.h" 3 4
                          ((void *)0)
# 106 "./subnode/LinkState.h"
                              )) { return 1; }
    }
    return 0;
}

static inline int LinkState_getNodeId(const struct VarInt_Iter* iter, uint32_t* nodeId)
{
    struct VarInt_Iter it;
    VarInt_clone(&it, iter);
    uint64_t id;
    if (VarInt_pop(&it, &id)) { return 1; }
    *nodeId = id;
    return 0;
}







static inline int LinkState_decode(const struct VarInt_Iter* iter, struct LinkState* ls)
{
    struct VarInt_Iter it;
    VarInt_clone(&it, iter);
    ls->nodeId = (__extension__ ({ uint64_t x; if (VarInt_pop((&it), &x)) { return 1; } x; }));
    uint32_t i = (__extension__ ({ uint64_t x; if (VarInt_pop((&it), &x)) { return 1; } x; }));
    uint32_t count = 0;
    if (i >= 18) { return 1; }
    for (;;) {
        if (it.ptr == it.end) { break; }
        ls->lagSlots[i] = (__extension__ ({ uint64_t x; if (VarInt_pop((&it), &x)) { return 1; } x; }));
        ls->dropSlots[i] = (__extension__ ({ uint64_t x; if (VarInt_pop((&it), &x)) { return 1; } x; }));
        ls->kbRecvSlots[i] = (__extension__ ({ uint64_t x; if (VarInt_pop((&it), &x)) { return 1; } x; }));
        i = (i + 1) % 18;
        count++;
    }
    ls->samples += count;
    return 0;
}
# 22 "./subnode/ReachabilityCollector.h" 2

struct Linker_x017916429295779013;

struct ReachabilityCollector_PeerInfo
{

    struct Address addr;


    uint64_t pathThemToUs;

    
# 33 "./subnode/ReachabilityCollector.h" 3 4
   _Bool 
# 33 "./subnode/ReachabilityCollector.h"
        querying;

    struct LinkState linkState;
};

struct ReachabilityCollector;


typedef void (* ReachabilityCollector_OnChange)(struct ReachabilityCollector* rc,
                                                uint8_t ipv6[16],
                                                struct ReachabilityCollector_PeerInfo* pi);

struct ReachabilityCollector
{
    ReachabilityCollector_OnChange onChange;
    void* userData;
};

struct ReachabilityCollector_PeerInfo*
    ReachabilityCollector_getPeerInfo(struct ReachabilityCollector* rc, int peerNum);


void ReachabilityCollector_change(struct ReachabilityCollector* rc, struct Address* nodeAddr);

void ReachabilityCollector_lagSample(
    struct ReachabilityCollector* rc, uint64_t label, uint32_t milliseconds);

void ReachabilityCollector_updateBandwidthAndDrops(
    struct ReachabilityCollector* rc,
    uint64_t label,
    uint64_t sumOfPackets,
    uint64_t sumOfDrops,
    uint64_t sumOfKb);

struct ReachabilityCollector* ReachabilityCollector_new(struct Allocator* allocator,
                                                        struct MsgCore* mc,
                                                        struct Log* log,
                                                        struct EventBase* base,
                                                        struct BoilerplateResponder* br,
                                                        struct Address* myAddr,
                                                        struct EncodingScheme* myScheme);
# 24 "./subnode/SupernodeHunter.h" 2

# 1 "./net/SwitchPinger.h" 1
# 25 "./net/SwitchPinger.h"
struct Linker_x00034935944147949094;





enum SwitchPinger_Result
{

    SwitchPinger_Result_OK,


    SwitchPinger_Result_LABEL_MISMATCH,


    SwitchPinger_Result_WRONG_DATA,


    SwitchPinger_Result_ERROR_RESPONSE,


    SwitchPinger_Result_LOOP_ROUTE,


    SwitchPinger_Result_TIMEOUT
};

struct SwitchPinger_Ping;

struct SwitchPinger_Response
{

    enum SwitchPinger_Result res;


    uint64_t label;


    String* data;


    uint64_t milliseconds;


    uint32_t version;


    uint8_t key[32];


    uint32_t kbpsLimit;
    struct Address snode;

    struct SwitchPinger_Ping* ping;
};


typedef void (* SwitchPinger_ResponseCallback)(struct SwitchPinger_Response* resp, void* userData);

enum SwitchPinger_Type
{
    SwitchPinger_Type_PING,
    SwitchPinger_Type_KEYPING,
    SwitchPinger_Type_GETSNODE
};

struct SwitchPinger_Ping
{




    struct Allocator* pingAlloc;

    enum SwitchPinger_Type type;


    uint32_t kbpsLimit;
    struct Address snode;





    void* onResponseContext;
};

struct SwitchPinger
{
    struct Iface controlHandlerIf;
};

String* SwitchPinger_resultString(enum SwitchPinger_Result result);
# 130 "./net/SwitchPinger.h"
struct SwitchPinger_Ping* SwitchPinger_newPing(uint64_t label,
                                               String* data,
                                               uint32_t timeoutMilliseconds,
                                               SwitchPinger_ResponseCallback onResponse,
                                               struct Allocator* alloc,
                                               struct SwitchPinger* ctx);

struct SwitchPinger* SwitchPinger_new(struct EventBase* eventBase,
                                      struct Random* rand,
                                      struct Log* logger,
                                      struct Address* myAddr,
                                      struct Allocator* alloc);
# 26 "./subnode/SupernodeHunter.h" 2

struct Linker_x05898508157236049;

struct SupernodeHunter;

typedef void (* SupernodeHunter_Callback)(struct SupernodeHunter* sh,
                                          int64_t sendTime,
                                          int64_t snodeRecvTime);

struct SupernodeHunter
{







    int snodeIsReachable;

    struct Address snodeAddr;

    SupernodeHunter_Callback onSnodeChange;
    SupernodeHunter_Callback onSnodeUnreachable;
    void* userData;
};


int SupernodeHunter_addSnode(struct SupernodeHunter* snh, struct Address* snodeAddr);

int SupernodeHunter_listSnodes(struct SupernodeHunter* snh,
                               struct Address*** out,
                               struct Allocator* alloc);


int SupernodeHunter_removeSnode(struct SupernodeHunter* snh, struct Address* toRemove);






struct SupernodeHunter* SupernodeHunter_new(struct Allocator* allocator,
                                            struct Log* log,
                                            struct EventBase* base,
                                            struct SwitchPinger* sp,
                                            struct AddrSet* peers,
                                            struct MsgCore* msgCore,
                                            struct Address* myAddress,
                                            struct ReachabilityCollector* rc);
# 25 "./subnode/SubnodePathfinder.h" 2


struct Linker_x09306906057481075;

struct SubnodePathfinder
{
    struct Iface eventIf;
    struct SupernodeHunter* snh;
    struct ReachabilityCollector* rc;
};

void SubnodePathfinder_start(struct SubnodePathfinder*);

struct SubnodePathfinder* SubnodePathfinder_new(struct Allocator* allocator,
                                                struct Log* log,
                                                struct EventBase* base,
                                                struct Random* rand,
                                                struct Address* myAddress,
                                                uint8_t* privateKey,
                                                struct EncodingScheme* myScheme);
# 16 "subnode/SubnodePathfinder.c" 2



# 1 "./subnode/GetPeersResponder.h" 1
# 26 "./subnode/GetPeersResponder.h"
struct Linker_x03983659774447841;

struct GetPeersResponder
{
    int unused;
};

struct GetPeersResponder* GetPeersResponder_new(struct Allocator* allocator,
                                                struct Log* log,
                                                struct AddrSet* peers,
                                                struct Address* selfAddr,
                                                struct MsgCore* msgCore,
                                                struct BoilerplateResponder* br,
                                                struct EncodingScheme* myScheme);
# 20 "subnode/SubnodePathfinder.c" 2
# 1 "./subnode/PingResponder.h" 1
# 24 "./subnode/PingResponder.h"
struct Linker_x07348162824540103;

struct PingResponder
{
    int unused;
};

struct PingResponder* PingResponder_new(struct Allocator* allocator,
                                        struct Log* log,
                                        struct MsgCore* msgCore,
                                        struct BoilerplateResponder* br);
# 21 "subnode/SubnodePathfinder.c" 2


# 1 "./crypto/AddressCalc.h" 1
# 19 "./crypto/AddressCalc.h"
struct Linker_x08018659924157272;
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
# 24 "subnode/SubnodePathfinder.c" 2

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
# 26 "subnode/SubnodePathfinder.c" 2
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
# 27 "subnode/SubnodePathfinder.c" 2
# 1 "./dht/dhtcore/ReplySerializer.h" 1
# 23 "./dht/dhtcore/ReplySerializer.h"
struct Linker_x036851289533345866;




struct Address_List* ReplySerializer_parse(struct Address* fromNode,
                                           Dict* result,
                                           struct Log* log,
                                           
# 31 "./dht/dhtcore/ReplySerializer.h" 3 4
                                          _Bool 
# 31 "./dht/dhtcore/ReplySerializer.h"
                                               splicePath,
                                           struct Allocator* alloc);

void ReplySerializer_serialize(struct Address_List* addrs,
                               Dict* out,
                               struct Address* convertDirectorFor,
                               struct Allocator* alloc);
# 28 "subnode/SubnodePathfinder.c" 2
# 1 "./util/AddrTools.h" 1
# 19 "./util/AddrTools.h"
struct Linker_x03993255978438579;




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
# 29 "subnode/SubnodePathfinder.c" 2
# 1 "./util/events/Timeout.h" 1
# 20 "./util/events/Timeout.h"
struct Linker_x09114840072052754;



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
# 30 "subnode/SubnodePathfinder.c" 2

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
# 32 "subnode/SubnodePathfinder.c" 2
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
# 33 "subnode/SubnodePathfinder.c" 2
# 1 "./wire/PFChan.h" 1
# 22 "./wire/PFChan.h"
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

       
# 34 "subnode/SubnodePathfinder.c" 2



# 1 "./subnode/ReachabilityAnnouncer.h" 1
# 28 "./subnode/ReachabilityAnnouncer.h"
struct Linker_x007141918634261502;

struct ReachabilityAnnouncer
{
    int unused;
};


void ReachabilityAnnouncer_updatePeer(struct ReachabilityAnnouncer* ra,
                                      uint8_t ipv6[16],
                                      struct ReachabilityCollector_PeerInfo* pi);

struct ReachabilityAnnouncer* ReachabilityAnnouncer_new(struct Allocator* allocator,
                                                        struct Log* log,
                                                        struct EventBase* base,
                                                        struct Random* rand,
                                                        struct MsgCore* msgCore,
                                                        struct SupernodeHunter* snh,
                                                        uint8_t* privateKey,
                                                        struct EncodingScheme* myScheme);
# 38 "subnode/SubnodePathfinder.c" 2

struct Query {
    struct Address target;
    uint8_t routeFrom[16];
    uint8_t routeTo[16];
};




# 1 "./util/Map.h" 1
# 23 "./util/Map.h"
    void Assert_compileTime(char x[1 - (!(!(sizeof(struct Query) % 4)))]);
# 42 "./util/Map.h"
    static inline uint32_t Map_OfPromiseByQuery_hash(struct Query* key);

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

        static inline uint32_t Map_OfPromiseByQuery_hash(struct Query* key)
        {
            return Hash_compute((uint8_t*)key, sizeof(struct Query));
        }



    static inline int Map_OfPromiseByQuery_compare(struct Query* keyA, struct Query* keyB);


        static inline int Map_OfPromiseByQuery_compare(struct Query* keyA, struct Query* keyB)
        {
            return __builtin_memcmp(keyA,keyB,sizeof(struct Query));
        }



struct Map_OfPromiseByQuery
{

        uint32_t* hashCodes;
        struct Query* keys;



        uint32_t* handles;
        uint32_t nextHandle;


    struct MsgCore_Promise** values;

    uint32_t count;
    uint32_t capacity;

    struct Allocator* allocator;
};

static inline struct Map_OfPromiseByQuery* Map_OfPromiseByQuery_new(struct Allocator* allocator)
{
    return Allocator__clone((allocator),((&(struct Map_OfPromiseByQuery) { .allocator = allocator })),sizeof(*((&(struct Map_OfPromiseByQuery) { .allocator = allocator }))),"Map.h",85)

       ;
}






static inline int Map_OfPromiseByQuery_indexForKey(struct Query* key, struct Map_OfPromiseByQuery* map)
{
    uint32_t hashCode = (Map_OfPromiseByQuery_hash(key));
    for (uint32_t i = 0; i < map->count; i++) {
        if (map->hashCodes[i] == hashCode
            && Map_OfPromiseByQuery_compare(key, &map->keys[i]) == 0)
        {
            return i;
        }
    }
    return -1;
}



static inline int Map_OfPromiseByQuery_indexForHandle(uint32_t handle, struct Map_OfPromiseByQuery* map)
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







static inline int Map_OfPromiseByQuery_remove(int index, struct Map_OfPromiseByQuery* map)
{
    if (index >= 0 && index < (int) map->count - 1) {



                __builtin_memmove(&map->hashCodes[index],&map->hashCodes[index + 1],(map->count - index - 1) * sizeof(uint32_t))

                                                                         ;

                __builtin_memmove(&map->keys[index],&map->keys[index + 1],(map->count - index - 1) * sizeof(struct Query))

                                                                             ;

            __builtin_memmove(&map->handles[index],&map->handles[index + 1],(map->count - index - 1) * sizeof(uint32_t))

                                                                     ;

            __builtin_memmove(&map->values[index],&map->values[index + 1],(map->count - index - 1) * sizeof(struct MsgCore_Promise*))

                                                                           ;

            map->count--;







        return 0;
    } else if (index == (int) map->count - 1) {
        map->count--;
        return 0;
    }
    return -1;
}


static inline int Map_OfPromiseByQuery_put(struct Query* key,
                                    struct MsgCore_Promise** value,
                                    struct Map_OfPromiseByQuery* map)




{
    if (map->count == map->capacity) {

            map->hashCodes = Allocator__realloc((map->allocator),(map->hashCodes),(sizeof(uint32_t) * (map->count + 10)),"Map.h",181)

                                                                                    ;
            map->keys = Allocator__realloc((map->allocator),(map->keys),(sizeof(struct Query) * (map->count + 10)),"Map.h",184)

                                                                                   ;



            map->handles = Allocator__realloc((map->allocator),(map->handles),(sizeof(uint32_t) * (map->count + 10)),"Map.h",190)

                                                                                  ;


        map->values = Allocator__realloc((map->allocator),(map->values),(sizeof(struct MsgCore_Promise*) * (map->count + 10)),"Map.h",195)

                                                                                   ;

        map->capacity += 10;
    }

    int i = -1;


        i = Map_OfPromiseByQuery_indexForKey(key, map);


    if (i < 0) {
        i = map->count;
        map->count++;

            map->handles[i] = map->nextHandle++;


            map->hashCodes[i] = (Map_OfPromiseByQuery_hash(key));
            Bits__memcpy(&map->keys[i],key,sizeof(struct Query),"Map.h",216);

    }

    Bits__memcpy(&map->values[i],value,sizeof(struct MsgCore_Promise*),"Map.h",220);

    return i;
}
# 49 "subnode/SubnodePathfinder.c" 2

struct SubnodePathfinder_pvt
{
    struct SubnodePathfinder pub;

    struct Iface msgCoreIf;

    struct Allocator* alloc;
    struct Log* log;
    struct EventBase* base;
    struct Random* rand;



    int state;

    struct Address* myAddress;

    struct AddrSet* myPeers;

    struct MsgCore* msgCore;

    struct Admin* admin;

    struct BoilerplateResponder* br;

    struct ReachabilityAnnouncer* ra;

    struct Map_OfPromiseByQuery queryMap;

    struct SwitchPinger* sp;
    struct Iface switchPingerIf;

    struct EncodingScheme* myScheme;

    uint8_t* privateKey;

    String* encodingSchemeStr;

    unsigned long Identity_verifier;
};

static void nodeForAddress(struct PFChan_Node* nodeOut, struct Address* addr, uint32_t metric)
{
    __builtin_memset(nodeOut,0,64);
    nodeOut->version_be = ((uint32_t)__builtin_bswap32(((uint32_t)(addr->protocolVersion))));
    nodeOut->metric_be = ((uint32_t)__builtin_bswap32(((uint32_t)(metric))));
    nodeOut->path_be = ((uint64_t)__builtin_bswap64(((uint64_t)(addr->path))));
    Bits__memcpy(nodeOut->publicKey,addr->key,32,"SubnodePathfinder.c",97);
    Bits__memcpy(nodeOut->ip6,addr->ip6.bytes,16,"SubnodePathfinder.c",98);
}

static __attribute__ ((warn_unused_result)) struct Iface* sendNode(struct Message* msg,
                            struct Address* addr,
                            uint32_t metric,
                            enum PFChan_Pathfinder msgType,
                            struct SubnodePathfinder_pvt* pf)
{
    Message_reset(msg);
    do {             struct Er_Ret* Er_ret = Message_eshift(msg, 64);                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("SubnodePathfinder.c", 108, Er_ret);                 } else {                     Er__assertFail(Er_unwind("SubnodePathfinder.c", 108, Er_ret));                 }             }                  } while (0);
    nodeForAddress((struct PFChan_Node*) msg->bytes, addr, metric);
    if (addr->path == 
# 110 "subnode/SubnodePathfinder.c" 3 4
                     0xffffffffffffffffULL
# 110 "subnode/SubnodePathfinder.c"
                               ) {
        ((struct PFChan_Node*) msg->bytes)->path_be = 0;
    }
    do {             struct Er_Ret* Er_ret = Message_epush32h(msg, ((uint32_t)__builtin_bswap32(((uint32_t)(msgType)))));                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("SubnodePathfinder.c", 113, Er_ret);                 } else {                     Er__assertFail(Er_unwind("SubnodePathfinder.c", 113, Er_ret));                 }             }                  } while (0);
    return Iface_next(&pf->pub.eventIf, msg);
}

static __attribute__ ((warn_unused_result)) struct Iface* connected(struct SubnodePathfinder_pvt* pf, struct Message* msg)
{
    do { if (pf->log && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(pf->log, Log_Level_DEBUG, "SubnodePathfinder.c", 119, "INIT"); } } while (0);
    pf->state = 1;
    return 
# 121 "subnode/SubnodePathfinder.c" 3 4
          ((void *)0)
# 121 "subnode/SubnodePathfinder.c"
              ;
}

static uint32_t addressForNode(struct Address* addrOut, struct Message* msg)
{
    struct PFChan_Node node;
    do {             struct Er_Ret* Er_ret = Message_epop(msg, &node, 64);                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("SubnodePathfinder.c", 127, Er_ret);                 } else {                     Er__assertFail(Er_unwind("SubnodePathfinder.c", 127, Er_ret));                 }             }                  } while (0);
    do { if (!((!msg->length))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("SubnodePathfinder.c"), (128), "(!msg->length)"); } } while (0);
    addrOut->protocolVersion = ((uint32_t)__builtin_bswap32(((uint32_t)(node.version_be))));
    addrOut->path = ((uint64_t)__builtin_bswap64(((uint64_t)(node.path_be))));
    Bits__memcpy(addrOut->key,node.publicKey,32,"SubnodePathfinder.c",131);
    Bits__memcpy(addrOut->ip6.bytes,node.ip6,16,"SubnodePathfinder.c",132);
    return ((uint32_t)__builtin_bswap32(((uint32_t)(node.metric_be))));
}

static __attribute__ ((warn_unused_result)) struct Iface* switchErr(struct Message* msg, struct SubnodePathfinder_pvt* pf)
{
    struct PFChan_Core_SwitchErr switchErr;
    do {             struct Er_Ret* Er_ret = Message_epop(msg, &switchErr, (12 + 4 + (4 + 12 + 4) + 12));                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("SubnodePathfinder.c", 139, Er_ret);                 } else {                     Er__assertFail(Er_unwind("SubnodePathfinder.c", 139, Er_ret));                 }             }                  } while (0);

    uint64_t path = ((uint64_t)__builtin_bswap64(((uint64_t)(switchErr.sh.label_be))));

    if (pf->pub.snh->snodeAddr.path &&
        pf->pub.snh->snodeAddr.path != path &&
        LabelSplicer_routesThrough(pf->pub.snh->snodeAddr.path, path)) {
        uint8_t pathStr[20];
        AddrTools_printPath(pathStr, path);
        int err = ((uint32_t)__builtin_bswap32(((uint32_t)(switchErr.ctrlErr.errorType_be))));
        do { if (pf->log && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(pf->log, Log_Level_DEBUG, "SubnodePathfinder.c", 149, "switch err from active snode [%s] type [%s][%d]", pathStr, Error_strerror(err), err); } } while (0)
                                              ;
        pf->pub.snh->snodeIsReachable = 
# 151 "subnode/SubnodePathfinder.c" 3 4
                                       0
# 151 "subnode/SubnodePathfinder.c"
                                            ;
        if (pf->pub.snh->onSnodeUnreachable) {
            pf->pub.snh->onSnodeUnreachable(pf->pub.snh, 0, 0);
        }
    }

    return 
# 157 "subnode/SubnodePathfinder.c" 3 4
          ((void *)0)
# 157 "subnode/SubnodePathfinder.c"
              ;
}

struct SnodeQuery {
    struct SubnodePathfinder_pvt* pf;
    uint32_t mapHandle;
    unsigned long Identity_verifier;
};

static void getRouteReply(Dict* msg, struct Address* src, struct MsgCore_Promise* prom)
{
    struct SnodeQuery* snq = (__extension__ ({ __typeof__((struct SnodeQuery*) prom->userData) Identity_ptr = ((struct SnodeQuery*) prom->userData); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0xe2caa450712d6515ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("SubnodePathfinder.c"), (168), "(Identity_ptr->Identity_verifier == ((unsigned long) 0xe2caa450712d6515ull))"); } } while (0); Identity_ptr; }));
    struct SubnodePathfinder_pvt* pf = (__extension__ ({ __typeof__(snq->pf) Identity_ptr = (snq->pf); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0xe2caa450712d6515ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("SubnodePathfinder.c"), (169), "(Identity_ptr->Identity_verifier == ((unsigned long) 0xe2caa450712d6515ull))"); } } while (0); Identity_ptr; }));
    int index = Map_OfPromiseByQuery_indexForHandle(snq->mapHandle, &pf->queryMap);
    do { if (!((index > -1))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("SubnodePathfinder.c"), (171), "(index > -1)"); } } while (0);
    Map_OfPromiseByQuery_remove(index, &pf->queryMap);

    if (!src) {
        do { if (pf->log && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(pf->log, Log_Level_DEBUG, "SubnodePathfinder.c", 175, "GetRoute timeout"); } } while (0);
        return;
    }
    do { if (pf->log && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(pf->log, Log_Level_DEBUG, "SubnodePathfinder.c", 178, "Search reply!"); } } while (0);
    struct Address_List* al = ReplySerializer_parse(src, msg, pf->log, 
# 179 "subnode/SubnodePathfinder.c" 3 4
                                                                      0
# 179 "subnode/SubnodePathfinder.c"
                                                                           , prom->alloc);
    if (!al || al->length == 0) { return; }
    do { if (pf->log && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(pf->log, Log_Level_DEBUG, "SubnodePathfinder.c", 181, "reply with[%s]", Address_toString(&al->elems[0], prom->alloc)->bytes); } } while (0);

    if (al->elems[0].protocolVersion < 20) {
        do { if (pf->log && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(pf->log, Log_Level_DEBUG, "SubnodePathfinder.c", 184, "not sending [%s] because version is old", Address_toString(&al->elems[0], prom->alloc)->bytes); } } while (0)
                                                                ;
        return;
    }


    struct Message* msgToCore = Message_new(0, 512, prom->alloc);
    do { do { if (!((!msgToCore->currentIface))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("SubnodePathfinder.c"), (191), "(!msgToCore->currentIface)"); } } while (0); struct Iface Iface_y = { .currentMsg = msgToCore }; msgToCore->currentIface = &Iface_y; struct Iface* Iface_x = sendNode(msgToCore, &al->elems[0], 0xfff00033, PFChan_Pathfinder_NODE, pf); msgToCore->currentIface = 
# 191 "subnode/SubnodePathfinder.c" 3 4
   ((void *)0)
# 191 "subnode/SubnodePathfinder.c"
   ; if (Iface_x) { Iface_send(Iface_x, msgToCore); } } while (0);
}

static __attribute__ ((warn_unused_result)) struct Iface* searchReq(struct Message* msg, struct SubnodePathfinder_pvt* pf)
{
    uint8_t addr[16];
    do {             struct Er_Ret* Er_ret = Message_epop(msg, addr, 16);                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("SubnodePathfinder.c", 197, Er_ret);                 } else {                     Er__assertFail(Er_unwind("SubnodePathfinder.c", 197, Er_ret));                 }             }                  } while (0);
    (__extension__({             uint32_t Er_returnVal;             __builtin_memset(&Er_returnVal, 0, sizeof(Er_returnVal));             struct Er_Ret* Er_ret = Message_epop32be(&Er_returnVal, msg);                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("SubnodePathfinder.c", 198, Er_ret);                 } else {                     Er__assertFail(Er_unwind("SubnodePathfinder.c", 198, Er_ret));                 }             }                      Er_returnVal;         }));
    uint32_t version = (__extension__({             uint32_t Er_returnVal;             __builtin_memset(&Er_returnVal, 0, sizeof(Er_returnVal));             struct Er_Ret* Er_ret = Message_epop32be(&Er_returnVal, msg);                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("SubnodePathfinder.c", 199, Er_ret);                 } else {                     Er__assertFail(Er_unwind("SubnodePathfinder.c", 199, Er_ret));                 }             }                      Er_returnVal;         }));
    if (version && version < 20) { return 
# 200 "subnode/SubnodePathfinder.c" 3 4
                                         ((void *)0)
# 200 "subnode/SubnodePathfinder.c"
                                             ; }
    do { if (!((!msg->length))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("SubnodePathfinder.c"), (201), "(!msg->length)"); } } while (0);
    uint8_t printedAddr[40];
    AddrTools_printIp(printedAddr, addr);
    do { if (pf->log && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(pf->log, Log_Level_DEBUG, "SubnodePathfinder.c", 204, "Search req [%s]", printedAddr); } } while (0);

    for (int i = 0; i < pf->myPeers->length; ++i) {
        struct Address* myPeer = AddrSet_get(pf->myPeers, i);
        if (!__builtin_memcmp(myPeer->ip6.bytes,addr,16)) {
            return sendNode(msg, myPeer, 0xfff00000, PFChan_Pathfinder_NODE, pf);
        }
    }

    if (!pf->pub.snh || !pf->pub.snh->snodeAddr.path) { return 
# 213 "subnode/SubnodePathfinder.c" 3 4
                                                              ((void *)0)
# 213 "subnode/SubnodePathfinder.c"
                                                                  ; }

    if (!__builtin_memcmp(pf->pub.snh->snodeAddr.ip6.bytes,addr,16)) {
        return sendNode(msg, &pf->pub.snh->snodeAddr, 0xfff00000, PFChan_Pathfinder_NODE, pf);
    }

    struct Query q = { .routeFrom = { 0 } };
    Bits__memcpy(&q.target,&pf->pub.snh->snodeAddr,sizeof(struct Address),"SubnodePathfinder.c",220);
    Bits__memcpy(q.routeFrom,pf->myAddress->ip6.bytes,16,"SubnodePathfinder.c",221);
    Bits__memcpy(q.routeFrom,addr,16,"SubnodePathfinder.c",222);
    if (Map_OfPromiseByQuery_indexForKey(&q, &pf->queryMap) > -1) {
        do { if (pf->log && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(pf->log, Log_Level_DEBUG, "SubnodePathfinder.c", 224, "Skipping snode query because one is outstanding"); } } while (0);
        return 
# 225 "subnode/SubnodePathfinder.c" 3 4
              ((void *)0)
# 225 "subnode/SubnodePathfinder.c"
                  ;
    }

    struct MsgCore_Promise* qp = MsgCore_createQuery(pf->msgCore, 0, pf->alloc);

    struct SnodeQuery* snq = Allocator__calloc((qp->alloc),(sizeof(struct SnodeQuery)),(1),"SubnodePathfinder.c",230);
    (snq)->Identity_verifier = ((unsigned long) 0xe2caa450712d6515ull);
    snq->pf = pf;

    Dict* dict = qp->msg = Dict_new(qp->alloc);
    qp->cb = getRouteReply;
    qp->userData = snq;

    do { if (!((AddressCalc_validAddress(pf->pub.snh->snodeAddr.ip6.bytes)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("SubnodePathfinder.c"), (238), "(AddressCalc_validAddress(pf->pub.snh->snodeAddr.ip6.bytes))"); } } while (0);
    qp->target = &pf->pub.snh->snodeAddr;

    do { if (pf->log && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(pf->log, Log_Level_DEBUG, "SubnodePathfinder.c", 241, "Sending getRoute to snode %s", Address_toString(qp->target, qp->alloc)->bytes); } } while (0)
                                                       ;
    Dict_putString(dict, String_new("sq", qp->alloc), String_new("gr", qp->alloc), qp->alloc);
    String* src = String_newBinary(pf->myAddress->ip6.bytes, 16, qp->alloc);
    Dict_putString(dict, String_new("src", qp->alloc), src, qp->alloc);
    String* target = String_newBinary(addr, 16, qp->alloc);
    Dict_putString(dict, String_new("tar", qp->alloc), target, qp->alloc);

    int index = Map_OfPromiseByQuery_put(&q, &qp, &pf->queryMap);
    snq->mapHandle = pf->queryMap.handles[index];

    return 
# 252 "subnode/SubnodePathfinder.c" 3 4
          ((void *)0)
# 252 "subnode/SubnodePathfinder.c"
              ;
}

static void rcChange(struct ReachabilityCollector* rc,
                     uint8_t nodeIpv6[16],
                     struct ReachabilityCollector_PeerInfo* pi)
{
    struct SubnodePathfinder_pvt* pf = (__extension__ ({ __typeof__((struct SubnodePathfinder_pvt*) rc->userData) Identity_ptr = ((struct SubnodePathfinder_pvt*) rc->userData); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0xe2caa450712d6515ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("SubnodePathfinder.c"), (259), "(Identity_ptr->Identity_verifier == ((unsigned long) 0xe2caa450712d6515ull))"); } } while (0); Identity_ptr; }));
    ReachabilityAnnouncer_updatePeer(pf->ra, nodeIpv6, pi);
}

static __attribute__ ((warn_unused_result)) struct Iface* peer(struct Message* msg, struct SubnodePathfinder_pvt* pf)
{
    struct Address addr;
    uint32_t metric = addressForNode(&addr, msg);
    String* str = Address_toString(&addr, msg->alloc);
    do { if (pf->log && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(pf->log, Log_Level_DEBUG, "SubnodePathfinder.c", 268, "Peer [%s]", str->bytes); } } while (0);

    int index = AddrSet_indexOf(pf->myPeers, &addr);
    if (index > -1) {
        struct Address* myPeer = AddrSet_get(pf->myPeers, index);
        if (myPeer->path == addr.path && myPeer->protocolVersion == addr.protocolVersion) {
            return 
# 274 "subnode/SubnodePathfinder.c" 3 4
                  ((void *)0)
# 274 "subnode/SubnodePathfinder.c"
                      ;
        }
        AddrSet_remove(pf->myPeers, myPeer);
    }

    AddrSet_add(pf->myPeers, &addr);



    ReachabilityCollector_change(pf->pub.rc, &addr);
    if ((metric & 0xffff) < 0xffff) {
        ReachabilityCollector_lagSample(pf->pub.rc, addr.path, (metric & 0xffff));
    }

    return sendNode(msg, &addr, 0xfff00000, PFChan_Pathfinder_NODE, pf);
}

static __attribute__ ((warn_unused_result)) struct Iface* peerGone(struct Message* msg, struct SubnodePathfinder_pvt* pf)
{
    struct Address addr;
    addressForNode(&addr, msg);

    for (int i = pf->myPeers->length - 1; i >= 0; i--) {
        struct Address* myPeer = AddrSet_get(pf->myPeers, i);
        if (myPeer->path == addr.path) {
            String* str = Address_toString(myPeer, msg->alloc);
            AddrSet_remove(pf->myPeers, myPeer);
            do { if (pf->log && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(pf->log, Log_Level_DEBUG, "SubnodePathfinder.c", 301, "Peer gone [%s]", str->bytes); } } while (0);
        }
    }



    struct Address zaddr;
    Bits__memcpy(&zaddr,&addr,(8 + 16 + 32 + 8),"SubnodePathfinder.c",308);
    zaddr.path = 0;
    ReachabilityCollector_change(pf->pub.rc, &zaddr);


    return sendNode(msg, &addr, 0xffffffff, PFChan_Pathfinder_NODE, pf);
}

static __attribute__ ((warn_unused_result)) struct Iface* session(struct Message* msg, struct SubnodePathfinder_pvt* pf)
{
    struct Address addr;
    addressForNode(&addr, msg);
    String* str = Address_toString(&addr, msg->alloc);
    do { if (pf->log && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(pf->log, Log_Level_DEBUG, "SubnodePathfinder.c", 321, "Session [%s]", str->bytes); } } while (0);

    return 
# 323 "subnode/SubnodePathfinder.c" 3 4
          ((void *)0)
# 323 "subnode/SubnodePathfinder.c"
              ;
}

static __attribute__ ((warn_unused_result)) struct Iface* sessionEnded(struct Message* msg, struct SubnodePathfinder_pvt* pf)
{
    struct Address addr;
    addressForNode(&addr, msg);
    String* str = Address_toString(&addr, msg->alloc);
    do { if (pf->log && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(pf->log, Log_Level_DEBUG, "SubnodePathfinder.c", 331, "Session ended [%s]", str->bytes); } } while (0);

    return 
# 333 "subnode/SubnodePathfinder.c" 3 4
          ((void *)0)
# 333 "subnode/SubnodePathfinder.c"
              ;
}

static __attribute__ ((warn_unused_result)) struct Iface* discoveredPath(struct Message* msg, struct SubnodePathfinder_pvt* pf)
{




    return 
# 342 "subnode/SubnodePathfinder.c" 3 4
          ((void *)0)
# 342 "subnode/SubnodePathfinder.c"
              ;
}

static __attribute__ ((warn_unused_result)) struct Iface* handlePing(struct Message* msg, struct SubnodePathfinder_pvt* pf)
{

    do {             struct Er_Ret* Er_ret = Message_epush32h(msg, ((uint32_t)__builtin_bswap32(((uint32_t)(PFChan_Pathfinder_PONG)))));                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("SubnodePathfinder.c", 348, Er_ret);                 } else {                     Er__assertFail(Er_unwind("SubnodePathfinder.c", 348, Er_ret));                 }             }                  } while (0);
    return Iface_next(&pf->pub.eventIf, msg);
}

static __attribute__ ((warn_unused_result)) struct Iface* handlePong(struct Message* msg, struct SubnodePathfinder_pvt* pf)
{

    return 
# 355 "subnode/SubnodePathfinder.c" 3 4
          ((void *)0)
# 355 "subnode/SubnodePathfinder.c"
              ;
}

static __attribute__ ((warn_unused_result)) struct Iface* ctrlMsgFromSwitchPinger(struct Message* msg, struct Iface* iface)
{
    struct SubnodePathfinder_pvt* pf =
        (__extension__ ({ const __typeof__(((struct SubnodePathfinder_pvt*)0)->switchPingerIf)*__mptr = (iface); (__extension__ ({ __typeof__((struct SubnodePathfinder_pvt*)((char*)__mptr - 
# 361 "subnode/SubnodePathfinder.c" 3 4
       __builtin_offsetof (
# 361 "subnode/SubnodePathfinder.c"
       struct SubnodePathfinder_pvt
# 361 "subnode/SubnodePathfinder.c" 3 4
       , 
# 361 "subnode/SubnodePathfinder.c"
       switchPingerIf
# 361 "subnode/SubnodePathfinder.c" 3 4
       )
# 361 "subnode/SubnodePathfinder.c"
       )) Identity_ptr = ((struct SubnodePathfinder_pvt*)((char*)__mptr - 
# 361 "subnode/SubnodePathfinder.c" 3 4
       __builtin_offsetof (
# 361 "subnode/SubnodePathfinder.c"
       struct SubnodePathfinder_pvt
# 361 "subnode/SubnodePathfinder.c" 3 4
       , 
# 361 "subnode/SubnodePathfinder.c"
       switchPingerIf
# 361 "subnode/SubnodePathfinder.c" 3 4
       )
# 361 "subnode/SubnodePathfinder.c"
       )); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0xe2caa450712d6515ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("SubnodePathfinder.c"), (361), "(Identity_ptr->Identity_verifier == ((unsigned long) 0xe2caa450712d6515ull))"); } } while (0); Identity_ptr; })); }));
    do {             struct Er_Ret* Er_ret = Message_epush32h(msg, ((uint32_t)__builtin_bswap32(((uint32_t)(PFChan_Pathfinder_CTRL_SENDMSG)))));                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("SubnodePathfinder.c", 362, Er_ret);                 } else {                     Er__assertFail(Er_unwind("SubnodePathfinder.c", 362, Er_ret));                 }             }                  } while (0);
    return Iface_next(&pf->pub.eventIf, msg);
}

static __attribute__ ((warn_unused_result)) struct Iface* ctrlMsg(struct Message* msg, struct SubnodePathfinder_pvt* pf)
{
    return Iface_next(&pf->switchPingerIf, msg);
}

struct UnsetupSessionPing {
    struct SubnodePathfinder_pvt* pf;
    uint32_t mapHandle;
    unsigned long Identity_verifier;
};

static void unsetupSessionPingReply(Dict* msg, struct Address* src, struct MsgCore_Promise* prom)
{
    struct UnsetupSessionPing* usp =
        (__extension__ ({ __typeof__((struct UnsetupSessionPing*) prom->userData) Identity_ptr = ((struct UnsetupSessionPing*) prom->userData); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0xe2caa450712d6515ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("SubnodePathfinder.c"), (380), "(Identity_ptr->Identity_verifier == ((unsigned long) 0xe2caa450712d6515ull))"); } } while (0); Identity_ptr; }));
    struct SubnodePathfinder_pvt* pf = (__extension__ ({ __typeof__(usp->pf) Identity_ptr = (usp->pf); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0xe2caa450712d6515ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("SubnodePathfinder.c"), (381), "(Identity_ptr->Identity_verifier == ((unsigned long) 0xe2caa450712d6515ull))"); } } while (0); Identity_ptr; }));
    int index = Map_OfPromiseByQuery_indexForHandle(usp->mapHandle, &pf->queryMap);
    do { if (!((index > -1))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("SubnodePathfinder.c"), (383), "(index > -1)"); } } while (0);
    Map_OfPromiseByQuery_remove(index, &pf->queryMap);

    if (!src) {

        return;
    }


    struct Message* msgToCore = Message_new(0, 512, prom->alloc);
    do { do { if (!((!msgToCore->currentIface))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("SubnodePathfinder.c"), (393), "(!msgToCore->currentIface)"); } } while (0); struct Iface Iface_y = { .currentMsg = msgToCore }; msgToCore->currentIface = &Iface_y; struct Iface* Iface_x = sendNode(msgToCore, src, 0xfffffff0, PFChan_Pathfinder_NODE, pf); msgToCore->currentIface = 
# 393 "subnode/SubnodePathfinder.c" 3 4
   ((void *)0)
# 393 "subnode/SubnodePathfinder.c"
   ; if (Iface_x) { Iface_send(Iface_x, msgToCore); } } while (0);
}

static __attribute__ ((warn_unused_result)) struct Iface* unsetupSession(struct Message* msg, struct SubnodePathfinder_pvt* pf)
{
    struct PFChan_Node node;
    do {             struct Er_Ret* Er_ret = Message_epop(msg, &node, 64);                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("SubnodePathfinder.c", 399, Er_ret);                 } else {                     Er__assertFail(Er_unwind("SubnodePathfinder.c", 399, Er_ret));                 }             }                  } while (0);
    do { if (!((!msg->length))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("SubnodePathfinder.c"), (400), "(!msg->length)"); } } while (0);
    struct Query q = { .routeFrom = { 0 } };
    struct Address* addr = &q.target;
    Bits__memcpy(addr->ip6.bytes,node.ip6,16,"SubnodePathfinder.c",403);
    Bits__memcpy(addr->key,node.publicKey,32,"SubnodePathfinder.c",404);
    addr->protocolVersion = ((uint32_t)__builtin_bswap32(((uint32_t)(node.version_be))));
    addr->path = ((uint64_t)__builtin_bswap64(((uint64_t)(node.path_be))));

    if (Map_OfPromiseByQuery_indexForKey(&q, &pf->queryMap) > -1) {
        do { if (pf->log && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(pf->log, Log_Level_DEBUG, "SubnodePathfinder.c", 409, "Skipping ping because one is already outstanding"); } } while (0);
        return 
# 410 "subnode/SubnodePathfinder.c" 3 4
              ((void *)0)
# 410 "subnode/SubnodePathfinder.c"
                  ;
    }


    struct MsgCore_Promise* qp = MsgCore_createQuery(pf->msgCore, 0, pf->alloc);

    struct UnsetupSessionPing* usp =
        Allocator__calloc((qp->alloc),(sizeof(struct UnsetupSessionPing)),(1),"SubnodePathfinder.c",417);
    (usp)->Identity_verifier = ((unsigned long) 0xe2caa450712d6515ull);
    usp->pf = pf;

    Dict* dict = qp->msg = Dict_new(qp->alloc);
    qp->cb = unsetupSessionPingReply;
    qp->userData = usp;

    do { if (!((AddressCalc_validAddress(addr->ip6.bytes)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("SubnodePathfinder.c"), (425), "(AddressCalc_validAddress(addr->ip6.bytes))"); } } while (0);
    do { if (!((addr->path))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("SubnodePathfinder.c"), (426), "(addr->path)"); } } while (0);
    qp->target = Address_clone(addr, qp->alloc);



    Dict_putString(dict, String_new("q", qp->alloc), String_new("pn", qp->alloc), qp->alloc);

    BoilerplateResponder_addBoilerplate(pf->br, dict, addr, qp->alloc);

    int index = Map_OfPromiseByQuery_put(&q, &qp, &pf->queryMap);
    usp->mapHandle = pf->queryMap.handles[index];

    return 
# 438 "subnode/SubnodePathfinder.c" 3 4
          ((void *)0)
# 438 "subnode/SubnodePathfinder.c"
              ;
}

static __attribute__ ((warn_unused_result)) struct Iface* incomingMsg(struct Message* msg, struct SubnodePathfinder_pvt* pf)
{
    return Iface_next(&pf->msgCoreIf, msg);
}

static __attribute__ ((warn_unused_result)) struct Iface* linkState(struct Message* msg, struct SubnodePathfinder_pvt* pf)
{
    while (msg->length) {
        struct PFChan_LinkState_Entry lse;
        do {             struct Er_Ret* Er_ret = Message_epop(msg, &lse, 32);                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("SubnodePathfinder.c", 450, Er_ret);                 } else {                     Er__assertFail(Er_unwind("SubnodePathfinder.c", 450, Er_ret));                 }             }                  } while (0);
        ReachabilityCollector_updateBandwidthAndDrops(
            pf->pub.rc,
            lse.peerLabel,
            lse.sumOfPackets,
            lse.sumOfDrops,
            lse.sumOfKb);
    }
    return 
# 458 "subnode/SubnodePathfinder.c" 3 4
          ((void *)0)
# 458 "subnode/SubnodePathfinder.c"
              ;
}

static __attribute__ ((warn_unused_result)) struct Iface* incomingFromMsgCore(struct Message* msg, struct Iface* iface)
{
    struct SubnodePathfinder_pvt* pf =
        (__extension__ ({ const __typeof__(((struct SubnodePathfinder_pvt*)0)->msgCoreIf)*__mptr = (iface); (__extension__ ({ __typeof__((struct SubnodePathfinder_pvt*)((char*)__mptr - 
# 464 "subnode/SubnodePathfinder.c" 3 4
       __builtin_offsetof (
# 464 "subnode/SubnodePathfinder.c"
       struct SubnodePathfinder_pvt
# 464 "subnode/SubnodePathfinder.c" 3 4
       , 
# 464 "subnode/SubnodePathfinder.c"
       msgCoreIf
# 464 "subnode/SubnodePathfinder.c" 3 4
       )
# 464 "subnode/SubnodePathfinder.c"
       )) Identity_ptr = ((struct SubnodePathfinder_pvt*)((char*)__mptr - 
# 464 "subnode/SubnodePathfinder.c" 3 4
       __builtin_offsetof (
# 464 "subnode/SubnodePathfinder.c"
       struct SubnodePathfinder_pvt
# 464 "subnode/SubnodePathfinder.c" 3 4
       , 
# 464 "subnode/SubnodePathfinder.c"
       msgCoreIf
# 464 "subnode/SubnodePathfinder.c" 3 4
       )
# 464 "subnode/SubnodePathfinder.c"
       )); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0xe2caa450712d6515ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("SubnodePathfinder.c"), (464), "(Identity_ptr->Identity_verifier == ((unsigned long) 0xe2caa450712d6515ull))"); } } while (0); Identity_ptr; })); }));
    do { if (!((msg->length >= ((56 + 12) + 4)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("SubnodePathfinder.c"), (465), "(msg->length >= ((56 + 12) + 4))"); } } while (0);
    struct RouteHeader* rh = (struct RouteHeader*) msg->bytes;
    struct DataHeader* dh = (struct DataHeader*) &rh[1];
    do { if (!((DataHeader_getContentType(dh) == ContentType_CJDHT))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("SubnodePathfinder.c"), (468), "(DataHeader_getContentType(dh) == ContentType_CJDHT)"); } } while (0);
    do { if (!((!Bits_isZero(rh->publicKey, 32)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("SubnodePathfinder.c"), (469), "(!Bits_isZero(rh->publicKey, 32))"); } } while (0);
    do { if (!((rh->version_be))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("SubnodePathfinder.c"), (470), "(rh->version_be)"); } } while (0);
    do { if (!((rh->sh.label_be))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("SubnodePathfinder.c"), (471), "(rh->sh.label_be)"); } } while (0);
    do {             struct Er_Ret* Er_ret = Message_epush32h(msg, ((uint32_t)__builtin_bswap32(((uint32_t)(PFChan_Pathfinder_SENDMSG)))));                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("SubnodePathfinder.c", 472, Er_ret);                 } else {                     Er__assertFail(Er_unwind("SubnodePathfinder.c", 472, Er_ret));                 }             }                  } while (0);
    return Iface_next(&pf->pub.eventIf, msg);
}

static __attribute__ ((warn_unused_result)) struct Iface* incomingFromEventIf(struct Message* msg, struct Iface* eventIf)
{
    struct SubnodePathfinder_pvt* pf =
        (__extension__ ({ const __typeof__(((struct SubnodePathfinder_pvt*)0)->pub.eventIf)*__mptr = (eventIf); (__extension__ ({ __typeof__((struct SubnodePathfinder_pvt*)((char*)__mptr - 
# 479 "subnode/SubnodePathfinder.c" 3 4
       __builtin_offsetof (
# 479 "subnode/SubnodePathfinder.c"
       struct SubnodePathfinder_pvt
# 479 "subnode/SubnodePathfinder.c" 3 4
       , 
# 479 "subnode/SubnodePathfinder.c"
       pub.eventIf
# 479 "subnode/SubnodePathfinder.c" 3 4
       )
# 479 "subnode/SubnodePathfinder.c"
       )) Identity_ptr = ((struct SubnodePathfinder_pvt*)((char*)__mptr - 
# 479 "subnode/SubnodePathfinder.c" 3 4
       __builtin_offsetof (
# 479 "subnode/SubnodePathfinder.c"
       struct SubnodePathfinder_pvt
# 479 "subnode/SubnodePathfinder.c" 3 4
       , 
# 479 "subnode/SubnodePathfinder.c"
       pub.eventIf
# 479 "subnode/SubnodePathfinder.c" 3 4
       )
# 479 "subnode/SubnodePathfinder.c"
       )); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0xe2caa450712d6515ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("SubnodePathfinder.c"), (479), "(Identity_ptr->Identity_verifier == ((unsigned long) 0xe2caa450712d6515ull))"); } } while (0); Identity_ptr; })); }));
    enum PFChan_Core ev = (__extension__({             uint32_t Er_returnVal;             __builtin_memset(&Er_returnVal, 0, sizeof(Er_returnVal));             struct Er_Ret* Er_ret = Message_epop32be(&Er_returnVal, msg);                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("SubnodePathfinder.c", 480, Er_ret);                 } else {                     Er__assertFail(Er_unwind("SubnodePathfinder.c", 480, Er_ret));                 }             }                      Er_returnVal;         }));
    if (0 == pf->state) {
        do { if (!((ev == PFChan_Core_CONNECT))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("SubnodePathfinder.c"), (482), "(ev == PFChan_Core_CONNECT)"); } } while (0);
        return connected(pf, msg);
    }
    switch (ev) {
        case PFChan_Core_SWITCH_ERR: return switchErr(msg, pf);
        case PFChan_Core_SEARCH_REQ: return searchReq(msg, pf);
        case PFChan_Core_PEER: return peer(msg, pf);
        case PFChan_Core_PEER_GONE: return peerGone(msg, pf);
        case PFChan_Core_SESSION: return session(msg, pf);
        case PFChan_Core_SESSION_ENDED: return sessionEnded(msg, pf);
        case PFChan_Core_DISCOVERED_PATH: return discoveredPath(msg, pf);
        case PFChan_Core_MSG: return incomingMsg(msg, pf);
        case PFChan_Core_PING: return handlePing(msg, pf);
        case PFChan_Core_PONG: return handlePong(msg, pf);
        case PFChan_Core_CTRL_MSG: return ctrlMsg(msg, pf);
        case PFChan_Core_UNSETUP_SESSION: return unsetupSession(msg, pf);
        case PFChan_Core_LINK_STATE: return linkState(msg, pf);
        default:;
    }
    Assert_failure("unexpected event [%d]", ev);
}

static void sendEvent(struct SubnodePathfinder_pvt* pf,
                      enum PFChan_Pathfinder ev,
                      void* data,
                      int size)
{
    struct Allocator* alloc = Allocator__child((pf->alloc),"SubnodePathfinder.c",509);
    struct Message* msg = Message_new(0, 512+size, alloc);
    do {             struct Er_Ret* Er_ret = Message_epush(msg, data, size);                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("SubnodePathfinder.c", 511, Er_ret);                 } else {                     Er__assertFail(Er_unwind("SubnodePathfinder.c", 511, Er_ret));                 }             }                  } while (0);
    do {             struct Er_Ret* Er_ret = Message_epush32h(msg, ((uint32_t)__builtin_bswap32(((uint32_t)(ev)))));                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("SubnodePathfinder.c", 512, Er_ret);                 } else {                     Er__assertFail(Er_unwind("SubnodePathfinder.c", 512, Er_ret));                 }             }                  } while (0);
    Iface_send(&pf->pub.eventIf, msg);
    Allocator__free((alloc),"SubnodePathfinder.c",514);
}

void SubnodePathfinder_start(struct SubnodePathfinder* sp)
{
    struct SubnodePathfinder_pvt* pf = (__extension__ ({ __typeof__((struct SubnodePathfinder_pvt*) sp) Identity_ptr = ((struct SubnodePathfinder_pvt*) sp); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0xe2caa450712d6515ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("SubnodePathfinder.c"), (519), "(Identity_ptr->Identity_verifier == ((unsigned long) 0xe2caa450712d6515ull))"); } } while (0); Identity_ptr; }));
    struct MsgCore* msgCore = pf->msgCore =
        MsgCore_new(pf->base, pf->rand, pf->alloc, pf->log, pf->myScheme);
    Iface_plumb(&pf->msgCoreIf, &msgCore->interRouterIf);

    PingResponder_new(pf->alloc, pf->log, msgCore, pf->br);

    GetPeersResponder_new(
        pf->alloc, pf->log, pf->myPeers, pf->myAddress, msgCore, pf->br, pf->myScheme);

    struct ReachabilityCollector* rc = pf->pub.rc = ReachabilityCollector_new(
        pf->alloc, msgCore, pf->log, pf->base, pf->br, pf->myAddress, pf->myScheme);
    rc->userData = pf;
    rc->onChange = rcChange;

    struct SupernodeHunter* snh = pf->pub.snh = SupernodeHunter_new(
        pf->alloc, pf->log, pf->base, pf->sp, pf->myPeers, msgCore, pf->myAddress, rc);

    pf->ra = ReachabilityAnnouncer_new(
        pf->alloc, pf->log, pf->base, pf->rand, msgCore, snh, pf->privateKey, pf->myScheme);

    struct PFChan_Pathfinder_Connect conn = {
        .superiority_be = ((uint32_t)__builtin_bswap32(((uint32_t)(1)))),
        .version_be = ((uint32_t)__builtin_bswap32(((uint32_t)(20))))
    };
    CString_safeStrncpy(conn.userAgent, "Cjdns subnode pathfinder", 64);
    sendEvent(pf, PFChan_Pathfinder_CONNECT, &conn, 72);
}

static void sendCurrentSupernode(void* vsp)
{
    struct SubnodePathfinder_pvt* pf = (__extension__ ({ __typeof__((struct SubnodePathfinder_pvt*) vsp) Identity_ptr = ((struct SubnodePathfinder_pvt*) vsp); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0xe2caa450712d6515ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("SubnodePathfinder.c"), (550), "(Identity_ptr->Identity_verifier == ((unsigned long) 0xe2caa450712d6515ull))"); } } while (0); Identity_ptr; }));
    struct Allocator* alloc = Allocator__child((pf->alloc),"SubnodePathfinder.c",551);
    struct Message* msgToCore = Message_new(0, 512, alloc);
    do { do { if (!((!msgToCore->currentIface))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("SubnodePathfinder.c"), (553), "(!msgToCore->currentIface)"); } } while (0); struct Iface Iface_y = { .currentMsg = msgToCore }; msgToCore->currentIface = &Iface_y; struct Iface* Iface_x = sendNode(msgToCore, &pf->pub.snh->snodeAddr, 0, PFChan_Pathfinder_SNODE, pf); msgToCore->currentIface = 
# 553 "subnode/SubnodePathfinder.c" 3 4
   ((void *)0)
# 553 "subnode/SubnodePathfinder.c"
   ; if (Iface_x) { Iface_send(Iface_x, msgToCore); } } while (0);
    Allocator__free((alloc),"SubnodePathfinder.c",554);
}

struct SubnodePathfinder* SubnodePathfinder_new(struct Allocator* allocator,
                                                struct Log* log,
                                                struct EventBase* base,
                                                struct Random* rand,
                                                struct Address* myAddress,
                                                uint8_t* privateKey,
                                                struct EncodingScheme* myScheme)
{
    struct Allocator* alloc = Allocator__child((allocator),"SubnodePathfinder.c",565);
    struct SubnodePathfinder_pvt* pf =
        Allocator__calloc((alloc),(sizeof(struct SubnodePathfinder_pvt)),(1),"SubnodePathfinder.c",567);
    (pf)->Identity_verifier = ((unsigned long) 0xe2caa450712d6515ull);
    pf->alloc = alloc;
    pf->log = log;
    pf->base = base;
    pf->rand = rand;
    pf->myAddress = myAddress;
    pf->myPeers = AddrSet_new(alloc);
    pf->pub.eventIf.send = incomingFromEventIf;
    pf->msgCoreIf.send = incomingFromMsgCore;
    pf->privateKey = privateKey;
    pf->queryMap.allocator = Allocator__child((alloc),"SubnodePathfinder.c",578);

    pf->myScheme = myScheme;
    pf->br = BoilerplateResponder_new(myScheme, alloc);

    pf->sp = SwitchPinger_new(base, rand, log, myAddress, alloc);
    pf->switchPingerIf.send = ctrlMsgFromSwitchPinger;
    Iface_plumb(&pf->switchPingerIf, &pf->sp->controlHandlerIf);

    Timeout__setInterval((sendCurrentSupernode), (pf), (3000), (base), (alloc), "SubnodePathfinder.c", 587);

    return &pf->pub;
}
