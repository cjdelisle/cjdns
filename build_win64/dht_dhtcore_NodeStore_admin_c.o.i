# 1 "dht/dhtcore/NodeStore_admin.c"
# 1 "/home/jojapoppa/cjdns//"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "dht/dhtcore/NodeStore_admin.c"
# 15 "dht/dhtcore/NodeStore_admin.c"
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
struct Linker_x07435667972954565;
# 33 "./util/Assert.h"
__attribute__((__format__ (__printf__, 1, 2)))
__attribute__((__noreturn__))
void Assert_failure(const char* format, ...);
# 19 "./util/Identity.h" 2
# 1 "./util/Constant.h" 1
# 33 "./util/Constant.h"

# 20 "./util/Identity.h" 2



    
# 19 "./memory/Allocator.h" 2


struct Linker_x08154522731947489;





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




struct Linker_x03745441685754638;

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
struct Linker_x09606579388823329;


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

struct Linker_x05860005611367212;



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
struct Linker_x05087811779775906;



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
struct Linker_x04862666654420682;



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

struct Linker_x06460041238868868;

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
struct Linker_x05496292587788363;

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
struct Linker_x09610645726648943;

struct EventBase
{
    int unused;
};

struct EventBase* EventBase_new(struct Allocator* alloc);

int EventBase_eventCount(struct EventBase* eventBase);

void EventBase_beginLoop(struct EventBase* eventBase);

void EventBase_endLoop(struct EventBase* eventBase);
# 26 "./admin/Admin.h" 2

struct Linker_x05980485608450385;

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
# 16 "dht/dhtcore/NodeStore_admin.c" 2

# 1 "./benc/String.h" 1
# 20 "./benc/String.h"
# 1 "./util/CString.h" 1
# 21 "./util/CString.h"
struct Linker_x0656845566829175;

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

struct Linker_x08218717859946569;


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
# 18 "dht/dhtcore/NodeStore_admin.c" 2
# 1 "./benc/Int.h" 1
# 19 "dht/dhtcore/NodeStore_admin.c" 2
# 1 "./crypto/Key.h" 1
# 19 "./crypto/Key.h"
# 1 "./crypto/random/Random.h" 1
# 21 "./crypto/random/Random.h"
# 1 "./crypto/random/seed/RandomSeed.h" 1
# 21 "./crypto/random/seed/RandomSeed.h"
struct Linker_x0032506539782120036;



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

struct Linker_x04258997082465026;



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
# 20 "./crypto/Key.h" 2


struct Linker_x0012443467795353325;






int Key_gen(uint8_t addressOut[16],
            uint8_t publicKeyOut[32],
            uint8_t privateKeyOut[32],
            struct Random* rand);

char* Key_parse_strerror(int error);
# 52 "./crypto/Key.h"
int Key_parse(String* key, uint8_t keyBytesOut[32], uint8_t ip6Out[16]);

String* Key_stringify(uint8_t key[32], struct Allocator* alloc);
# 20 "dht/dhtcore/NodeStore_admin.c" 2
# 1 "./dht/dhtcore/Node.h" 1
# 22 "./dht/dhtcore/Node.h"
# 1 "./dht/Address.h" 1
# 21 "./dht/Address.h"
struct Linker_x012757284095757515;
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
# 23 "./dht/dhtcore/Node.h" 2
# 1 "./switch/EncodingScheme.h" 1
# 19 "./switch/EncodingScheme.h"
# 1 "./benc/List.h" 1
# 21 "./benc/List.h"
struct Linker_x049721582145723486;

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


struct Linker_x045672809205024856;



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



struct Linker_x016422090637577114;

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
# 21 "dht/dhtcore/NodeStore_admin.c" 2
# 1 "./dht/dhtcore/NodeStore.h" 1
# 24 "./dht/dhtcore/NodeStore.h"
# 1 "./dht/dhtcore/RumorMill.h" 1
# 27 "./dht/dhtcore/RumorMill.h"
struct Linker_x08173697135295297;
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





struct Linker_x02817171667807825;





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
# 22 "dht/dhtcore/NodeStore_admin.c" 2
# 1 "./dht/dhtcore/NodeStore_admin.h" 1
# 22 "./dht/dhtcore/NodeStore_admin.h"
# 1 "./admin/Admin.h" 1
# 23 "./dht/dhtcore/NodeStore_admin.h" 2



struct Linker_x01378108014379087;

void NodeStore_admin_register(struct NodeStore* module,
                              struct Admin* admin,
                              struct Allocator* alloc);
# 23 "dht/dhtcore/NodeStore_admin.c" 2


# 1 "./util/AddrTools.h" 1
# 19 "./util/AddrTools.h"
struct Linker_x07267939910563554;




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
# 26 "dht/dhtcore/NodeStore_admin.c" 2
# 1 "./util/version/Version.h" 1
# 19 "./util/version/Version.h"
struct Linker_x009013193729562152;
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
# 27 "dht/dhtcore/NodeStore_admin.c" 2

struct Context {
    struct Admin* admin;
    struct Allocator* alloc;
    struct NodeStore* store;
    unsigned long Identity_verifier;
};



static void dumpTable(Dict* args, void* vcontext, String* txid, struct Allocator* requestAlloc)
{
    struct Context* ctx = (__extension__ ({ __typeof__((struct Context*) vcontext) Identity_ptr = ((struct Context*) vcontext); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0xdef86945d1746141ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("NodeStore_admin.c"), (39), "(Identity_ptr->Identity_verifier == ((unsigned long) 0xdef86945d1746141ull))"); } } while (0); Identity_ptr; }));
    int64_t* page = Dict_getInt(args, (&(String) { .bytes = "page", .len = CString_strlen("page") }));
    int ctr = (page) ? *page * 4 : 0;

    Dict* out = Dict_new(requestAlloc);
    List* table = List_new(requestAlloc);
    struct Node_Two* nn = 
# 45 "dht/dhtcore/NodeStore_admin.c" 3 4
                         ((void *)0)
# 45 "dht/dhtcore/NodeStore_admin.c"
                             ;
    for (int i = 0; i < ctr+4; i++) {
        nn = NodeStore_getNextNode(ctx->store, nn);
        if (!nn) { break; }
        if (i < ctr) { continue; }
        Dict* nodeDict = Dict_new(requestAlloc);

        String* ip = String_newBinary(
# 52 "dht/dhtcore/NodeStore_admin.c" 3 4
                                     ((void *)0)
# 52 "dht/dhtcore/NodeStore_admin.c"
                                         , 39, requestAlloc);
        Address_printIp(ip->bytes, &nn->address);
        Dict_putString(nodeDict, String_new("ip", requestAlloc), ip, requestAlloc);

        String* addr = Address_toString(&nn->address, requestAlloc);
        Dict_putString(nodeDict, String_new("addr", requestAlloc), addr, requestAlloc);

        String* path = String_newBinary(
# 59 "dht/dhtcore/NodeStore_admin.c" 3 4
                                       ((void *)0)
# 59 "dht/dhtcore/NodeStore_admin.c"
                                           , 19, requestAlloc);
        AddrTools_printPath(path->bytes, nn->address.path);
        Dict_putString(nodeDict, String_new("path", requestAlloc), path, requestAlloc);

        Dict_putInt(nodeDict, String_new("link", requestAlloc), Node_getCost(nn), requestAlloc);
        Dict_putInt(nodeDict, String_new("version", requestAlloc), nn->address.protocolVersion, requestAlloc);

        Dict_putInt(nodeDict, String_new("time", requestAlloc), NodeStore_timeSinceLastPing(ctx->store, nn), requestAlloc)


                                 ;

        Dict_putInt(nodeDict, String_new("bucket", requestAlloc), NodeStore_bucketForAddr(ctx->store->selfAddress, &nn->address), requestAlloc)


                                 ;

        List_addDict(table, nodeDict, requestAlloc);
    }
    Dict_putList(out, String_new("routingTable", requestAlloc), table, requestAlloc);

    if (nn) {
        Dict_putInt(out, String_new("more", requestAlloc), 1, requestAlloc);
    }
    Dict_putInt(out, String_new("count", requestAlloc), ctx->store->nodeCount, requestAlloc);
    Dict_putInt(out, String_new("peers", requestAlloc), ctx->store->peerCount, requestAlloc);

    Dict_putString(out, String_new("deprecation", requestAlloc), (&(String) { .bytes = "ip,path,version will soon be removed", .len = CString_strlen("ip,path,version will soon be removed") }), requestAlloc)
                                                                           ;

    Admin_sendMessage(out, txid, ctx->admin);
}

static int linkCount(struct Node_Two* parent)
{
    struct Node_Link* link = 
# 94 "dht/dhtcore/NodeStore_admin.c" 3 4
                            ((void *)0)
# 94 "dht/dhtcore/NodeStore_admin.c"
                                ;
    int i = 0;
    do {
        link = NodeStore_nextLink(parent, link);
        i++;
    } while (link);
    return i;
}

static struct Node_Link* getLinkByNum(struct Node_Two* parent, int linkNum)
{
    struct Node_Link* link = 
# 105 "dht/dhtcore/NodeStore_admin.c" 3 4
                            ((void *)0)
# 105 "dht/dhtcore/NodeStore_admin.c"
                                ;
    for (int i = 0; i <= linkNum; i++) {
        link = NodeStore_nextLink(parent, link);
        if (!link) { break; }
    }
    return link;
}

static void getLink(Dict* args, void* vcontext, String* txid, struct Allocator* alloc)
{
    struct Context* ctx = (__extension__ ({ __typeof__((struct Context*) vcontext) Identity_ptr = ((struct Context*) vcontext); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0xdef86945d1746141ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("NodeStore_admin.c"), (115), "(Identity_ptr->Identity_verifier == ((unsigned long) 0xdef86945d1746141ull))"); } } while (0); Identity_ptr; }));

    Dict* ret = Dict_new(alloc);
    Dict* result = Dict_new(alloc);
    Dict_putDict(ret, String_new("result", alloc), result, alloc);
    Dict_putString(ret, String_new("error", alloc), String_new("none", alloc), alloc);

    struct Node_Link* link = 
# 122 "dht/dhtcore/NodeStore_admin.c" 3 4
                            ((void *)0)
# 122 "dht/dhtcore/NodeStore_admin.c"
                                ;
    struct Node_Two* node = 
# 123 "dht/dhtcore/NodeStore_admin.c" 3 4
                           ((void *)0)
# 123 "dht/dhtcore/NodeStore_admin.c"
                               ;

    String* ipStr = Dict_getString(args, String_new("parent", alloc));
    int64_t* linkNum = Dict_getInt(args, String_new("linkNum", alloc));
    if (ipStr && ipStr->len) {
        uint8_t ip[16];
        if (AddrTools_parseIp(ip, ipStr->bytes)) {
            Dict_remove(ret, (&(String) { .bytes = "result", .len = CString_strlen("result") }));
            Dict_putString(ret,
                           String_new("error", alloc),
                           String_new("parse_parent", alloc),
                           alloc);
            Admin_sendMessage(ret, txid, ctx->admin);
            return;

        } else if (!(node = NodeStore_nodeForAddr(ctx->store, ip))) {
            Dict_putString(ret,
                           String_new("error", alloc),
                           String_new("not_found", alloc),
                           alloc);
            Admin_sendMessage(ret, txid, ctx->admin);
            return;
        } else if (!(link = getLinkByNum(node, *linkNum))) {
            Dict_putString(ret,
                           String_new("error", alloc),
                           String_new("unknown", alloc),
                           alloc);
            Admin_sendMessage(ret, txid, ctx->admin);
            return;
        }
    } else {
        for (int i = 0; i <= *linkNum; i++) {
            link = NodeStore_getNextLink(ctx->store, link);
            if (!link) { break; }
        }
        if (!link) {
            Dict_putString(ret,
                           String_new("error", alloc),
                           String_new("not_found", alloc),
                           alloc);
            Admin_sendMessage(ret, txid, ctx->admin);
            return;
        }
    }

    Dict_putInt(result,
                String_new("inverseLinkEncodingFormNumber", alloc),
                link->inverseLinkEncodingFormNumber,
                alloc);
    Dict_putInt(result, String_new("linkCost", alloc), link->linkCost, alloc);

    Dict_putInt(result, String_new("isOneHop", alloc), Node_isOneHopLink(link), alloc);

    int bestParent = (Node_getBestParent(link->child) == link);
    Dict_putInt(result, String_new("bestParent", alloc), bestParent, alloc);

    String* cannonicalLabel = String_newBinary(
# 179 "dht/dhtcore/NodeStore_admin.c" 3 4
                                              ((void *)0)
# 179 "dht/dhtcore/NodeStore_admin.c"
                                                  , 19, alloc);
    AddrTools_printPath(cannonicalLabel->bytes, link->cannonicalLabel);
    Dict_putString(result, String_new("cannonicalLabel", alloc), cannonicalLabel, alloc);

    String* parent = Address_toString(&link->parent->address, alloc);
    Dict_putString(result, String_new("parent", alloc), parent, alloc);

    String* child = Address_toString(&link->child->address, alloc);
    Dict_putString(result, String_new("child", alloc), child, alloc);

    Admin_sendMessage(ret, txid, ctx->admin);
}

static void nodeForAddr(Dict* args, void* vcontext, String* txid, struct Allocator* alloc)
{
    struct Context* ctx = (__extension__ ({ __typeof__((struct Context*) vcontext) Identity_ptr = ((struct Context*) vcontext); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0xdef86945d1746141ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("NodeStore_admin.c"), (194), "(Identity_ptr->Identity_verifier == ((unsigned long) 0xdef86945d1746141ull))"); } } while (0); Identity_ptr; }));

    Dict* ret = Dict_new(alloc);
    Dict* result = Dict_new(alloc);
    Dict_putDict(ret, String_new("result", alloc), result, alloc);
    Dict_putString(ret, String_new("error", alloc), String_new("none", alloc), alloc);


    struct Node_Two* node = ctx->store->selfNode;

    String* ipStr = Dict_getString(args, String_new("ip", alloc));
    uint8_t ip[16];
    while (ipStr) {
        if (AddrTools_parseIp(ip, ipStr->bytes)) {
            Dict_remove(ret, (&(String) { .bytes = "result", .len = CString_strlen("result") }));
            Dict_putString(ret,
                           String_new("error", alloc),
                           String_new("parse_ip", alloc),
                           alloc);

        } else if (!(node = NodeStore_nodeForAddr(ctx->store, ip))) {

        } else {
            break;
        }

        Admin_sendMessage(ret, txid, ctx->admin);
        return;
    }

    Dict_putInt(result, String_new("protocolVersion", alloc), node->address.protocolVersion, alloc);

    String* key = Key_stringify(node->address.key, alloc);
    Dict_putString(result, String_new("key", alloc), key, alloc);

    uint32_t count = linkCount(node);
    Dict_putInt(result, String_new("linkCount", alloc), count, alloc);

    Dict_putInt(result, String_new("cost", alloc), Node_getCost(node), alloc);

    List* encScheme = EncodingScheme_asList(node->encodingScheme, alloc);
    Dict_putList(result, String_new("encodingScheme", alloc), encScheme, alloc);

    Dict* bestParent = Dict_new(alloc);
    String* parentIp = String_newBinary(
# 238 "dht/dhtcore/NodeStore_admin.c" 3 4
                                       ((void *)0)
# 238 "dht/dhtcore/NodeStore_admin.c"
                                           , 39, alloc);
    AddrTools_printIp(parentIp->bytes, Node_getBestParent(node)->parent->address.ip6.bytes);
    Dict_putString(bestParent, String_new("ip", alloc), parentIp, alloc);

    String* parentChildLabel = String_newBinary(
# 242 "dht/dhtcore/NodeStore_admin.c" 3 4
                                               ((void *)0)
# 242 "dht/dhtcore/NodeStore_admin.c"
                                                   , 19, alloc);
    AddrTools_printPath(parentChildLabel->bytes, Node_getBestParent(node)->cannonicalLabel);
    Dict_putString(bestParent, String_new("parentChildLabel", alloc), parentChildLabel, alloc);

    int isOneHop = Node_isOneHopLink(Node_getBestParent(node));
    Dict_putInt(bestParent, String_new("isOneHop", alloc), isOneHop, alloc);

    Dict_putDict(result, String_new("bestParent", alloc), bestParent, alloc);

    String* bestLabel = String_newBinary(
# 251 "dht/dhtcore/NodeStore_admin.c" 3 4
                                        ((void *)0)
# 251 "dht/dhtcore/NodeStore_admin.c"
                                            , 19, alloc);
    AddrTools_printPath(bestLabel->bytes, node->address.path);
    Dict_putString(result, String_new("routeLabel", alloc), bestLabel, alloc);

    Admin_sendMessage(ret, txid, ctx->admin);
}

static void getRouteLabel(Dict* args, void* vcontext, String* txid, struct Allocator* requestAlloc)
{
    struct Context* ctx = (__extension__ ({ __typeof__((struct Context*) vcontext) Identity_ptr = ((struct Context*) vcontext); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0xdef86945d1746141ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("NodeStore_admin.c"), (260), "(Identity_ptr->Identity_verifier == ((unsigned long) 0xdef86945d1746141ull))"); } } while (0); Identity_ptr; }));

    char* err = 
# 262 "dht/dhtcore/NodeStore_admin.c" 3 4
               ((void *)0)
# 262 "dht/dhtcore/NodeStore_admin.c"
                   ;

    String* pathToParentS = Dict_getString(args, (&(String) { .bytes = "pathToParent", .len = CString_strlen("pathToParent") }));
    uint64_t pathToParent = 0;
    if (pathToParentS->len != 19 || AddrTools_parsePath(&pathToParent, pathToParentS->bytes)) {
        err = "parse_pathToParent";
    }

    String* pathParentToChildS = Dict_getString(args, (&(String) { .bytes = "pathParentToChild", .len = CString_strlen("pathParentToChild") }));
    uint64_t pathParentToChild = 0;
    if (pathParentToChildS->len != 19
        || AddrTools_parsePath(&pathParentToChild, pathParentToChildS->bytes))
    {
        err = "parse_pathParentToChild";
    }

    uint64_t label = 
# 278 "dht/dhtcore/NodeStore_admin.c" 3 4
                    0xffffffffffffffffULL
# 278 "dht/dhtcore/NodeStore_admin.c"
                              ;
    if (!err) {
        label = NodeStore_getRouteLabel(ctx->store, pathToParent, pathParentToChild);
        err = NodeStore_getRouteLabel_strerror(label);
    }
    Dict* response = Dict_new(requestAlloc);
    if (!err) {
        String* printedPath = String_newBinary(
# 285 "dht/dhtcore/NodeStore_admin.c" 3 4
                                              ((void *)0)
# 285 "dht/dhtcore/NodeStore_admin.c"
                                                  , 19, requestAlloc);
        AddrTools_printPath(printedPath->bytes, label);
        Dict_putString(response, String_new("result", requestAlloc), printedPath, requestAlloc);
        Dict_putString(response,
                       String_new("error", requestAlloc),
                       String_new("none", requestAlloc),
                       requestAlloc);
        Admin_sendMessage(response, txid, ctx->admin);
    } else {
        Dict_putString(response,
                       String_new("error", requestAlloc),
                       String_new(err, requestAlloc),
                       requestAlloc);
        Admin_sendMessage(response, txid, ctx->admin);
    }
}

static void getFullVerify(Dict* args, void* vcontext, String* txid, struct Allocator* requestAlloc)
{
    struct Context* ctx = (__extension__ ({ __typeof__((struct Context*) vcontext) Identity_ptr = ((struct Context*) vcontext); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0xdef86945d1746141ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("NodeStore_admin.c"), (304), "(Identity_ptr->Identity_verifier == ((unsigned long) 0xdef86945d1746141ull))"); } } while (0); Identity_ptr; }));
    Dict* response = Dict_new(requestAlloc);
    Dict_putString(response, String_new("error", requestAlloc), String_new("none", requestAlloc), requestAlloc);
    Dict_putInt(response, String_new("fullVerify", requestAlloc), NodeStore_getFullVerify(ctx->store), requestAlloc);
    Admin_sendMessage(response, txid, ctx->admin);
}

static void setFullVerify(Dict* args, void* vcontext, String* txid, struct Allocator* requestAlloc)
{
    struct Context* ctx = (__extension__ ({ __typeof__((struct Context*) vcontext) Identity_ptr = ((struct Context*) vcontext); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0xdef86945d1746141ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("NodeStore_admin.c"), (313), "(Identity_ptr->Identity_verifier == ((unsigned long) 0xdef86945d1746141ull))"); } } while (0); Identity_ptr; }));
    int64_t* fv = Dict_getInt(args, (&(String) { .bytes = "fullVerify", .len = CString_strlen("fullVerify") }));
    NodeStore_setFullVerify(ctx->store, (int) *fv);
    Dict* response = Dict_new(requestAlloc);
    Dict_putString(response, String_new("error", requestAlloc), String_new("none", requestAlloc), requestAlloc);
    Admin_sendMessage(response, txid, ctx->admin);
}

void NodeStore_admin_register(struct NodeStore* nodeStore,
                              struct Admin* admin,
                              struct Allocator* alloc)
{
    struct Context* ctx = Allocator__clone((alloc),((&(struct Context) { .admin = admin, .alloc = alloc, .store = nodeStore })),sizeof(*((&(struct Context) { .admin = admin, .alloc = alloc, .store = nodeStore }))),"NodeStore_admin.c",325)



       ;
    (ctx)->Identity_verifier = ((unsigned long) 0xdef86945d1746141ull);

    Admin_registerFunctionWithArgCount( "NodeStore_dumpTable", dumpTable, ctx, 
# 332 "dht/dhtcore/NodeStore_admin.c" 3 4
   0
# 332 "dht/dhtcore/NodeStore_admin.c"
   , ((struct Admin_FunctionArg[]) { { .name = "page", .required = 
# 332 "dht/dhtcore/NodeStore_admin.c" 3 4
   1
# 332 "dht/dhtcore/NodeStore_admin.c"
   , .type = "Int" }, }), (sizeof(((struct Admin_FunctionArg[]) { { .name = "page", .required = 
# 332 "dht/dhtcore/NodeStore_admin.c" 3 4
   1
# 332 "dht/dhtcore/NodeStore_admin.c"
   , .type = "Int" }, })) / sizeof(struct Admin_FunctionArg)), admin)


                  ;

    Admin_registerFunctionWithArgCount( "NodeStore_getLink", getLink, ctx, 
# 337 "dht/dhtcore/NodeStore_admin.c" 3 4
   0
# 337 "dht/dhtcore/NodeStore_admin.c"
   , ((struct Admin_FunctionArg[]) { { .name = "parent", .required = 
# 337 "dht/dhtcore/NodeStore_admin.c" 3 4
   0
# 337 "dht/dhtcore/NodeStore_admin.c"
   , .type = "String" }, { .name = "linkNum", .required = 
# 337 "dht/dhtcore/NodeStore_admin.c" 3 4
   1
# 337 "dht/dhtcore/NodeStore_admin.c"
   , .type = "Int" }, }), (sizeof(((struct Admin_FunctionArg[]) { { .name = "parent", .required = 
# 337 "dht/dhtcore/NodeStore_admin.c" 3 4
   0
# 337 "dht/dhtcore/NodeStore_admin.c"
   , .type = "String" }, { .name = "linkNum", .required = 
# 337 "dht/dhtcore/NodeStore_admin.c" 3 4
   1
# 337 "dht/dhtcore/NodeStore_admin.c"
   , .type = "Int" }, })) / sizeof(struct Admin_FunctionArg)), admin)



                  ;
    Admin_registerFunctionWithArgCount( "NodeStore_nodeForAddr", nodeForAddr, ctx, 
# 342 "dht/dhtcore/NodeStore_admin.c" 3 4
   0
# 342 "dht/dhtcore/NodeStore_admin.c"
   , ((struct Admin_FunctionArg[]) { { .name = "ip", .required = 
# 342 "dht/dhtcore/NodeStore_admin.c" 3 4
   0
# 342 "dht/dhtcore/NodeStore_admin.c"
   , .type = "String" }, }), (sizeof(((struct Admin_FunctionArg[]) { { .name = "ip", .required = 
# 342 "dht/dhtcore/NodeStore_admin.c" 3 4
   0
# 342 "dht/dhtcore/NodeStore_admin.c"
   , .type = "String" }, })) / sizeof(struct Admin_FunctionArg)), admin)


                  ;
    Admin_registerFunctionWithArgCount( "NodeStore_getRouteLabel", getRouteLabel, ctx, 
# 346 "dht/dhtcore/NodeStore_admin.c" 3 4
   1
# 346 "dht/dhtcore/NodeStore_admin.c"
   , ((struct Admin_FunctionArg[]) { { .name = "pathToParent", .required = 
# 346 "dht/dhtcore/NodeStore_admin.c" 3 4
   1
# 346 "dht/dhtcore/NodeStore_admin.c"
   , .type = "String" }, { .name = "pathParentToChild", .required = 
# 346 "dht/dhtcore/NodeStore_admin.c" 3 4
   1
# 346 "dht/dhtcore/NodeStore_admin.c"
   , .type = "String" } }), (sizeof(((struct Admin_FunctionArg[]) { { .name = "pathToParent", .required = 
# 346 "dht/dhtcore/NodeStore_admin.c" 3 4
   1
# 346 "dht/dhtcore/NodeStore_admin.c"
   , .type = "String" }, { .name = "pathParentToChild", .required = 
# 346 "dht/dhtcore/NodeStore_admin.c" 3 4
   1
# 346 "dht/dhtcore/NodeStore_admin.c"
   , .type = "String" } })) / sizeof(struct Admin_FunctionArg)), admin)



                  ;
    Admin_registerFunctionWithArgCount( "NodeStore_setFullVerify", setFullVerify, ctx, 
# 351 "dht/dhtcore/NodeStore_admin.c" 3 4
   1
# 351 "dht/dhtcore/NodeStore_admin.c"
   , ((struct Admin_FunctionArg[]) { { .name = "fullVerify", .required = 
# 351 "dht/dhtcore/NodeStore_admin.c" 3 4
   1
# 351 "dht/dhtcore/NodeStore_admin.c"
   , .type = "Int" }, }), (sizeof(((struct Admin_FunctionArg[]) { { .name = "fullVerify", .required = 
# 351 "dht/dhtcore/NodeStore_admin.c" 3 4
   1
# 351 "dht/dhtcore/NodeStore_admin.c"
   , .type = "Int" }, })) / sizeof(struct Admin_FunctionArg)), admin)


                  ;
    Admin_registerFunctionWithArgCount( "NodeStore_getFullVerify", getFullVerify, ctx, 
# 355 "dht/dhtcore/NodeStore_admin.c" 3 4
   0
# 355 "dht/dhtcore/NodeStore_admin.c"
   , 
# 355 "dht/dhtcore/NodeStore_admin.c" 3 4
   ((void *)0)
# 355 "dht/dhtcore/NodeStore_admin.c"
   , (sizeof(
# 355 "dht/dhtcore/NodeStore_admin.c" 3 4
   ((void *)0)
# 355 "dht/dhtcore/NodeStore_admin.c"
   ) / sizeof(struct Admin_FunctionArg)), admin);
}
