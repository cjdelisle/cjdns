# 1 "net/InterfaceController_admin.c"
# 1 "/home/jojapoppa/cjdns//"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "net/InterfaceController_admin.c"
# 15 "net/InterfaceController_admin.c"
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
struct Linker_x018921030067476985;
# 33 "./util/Assert.h"
__attribute__((__format__ (__printf__, 1, 2)))
__attribute__((__noreturn__))
void Assert_failure(const char* format, ...);
# 19 "./util/Identity.h" 2
# 1 "./util/Constant.h" 1
# 33 "./util/Constant.h"

# 20 "./util/Identity.h" 2



    
# 19 "./memory/Allocator.h" 2


struct Linker_x021054393155339035;





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




struct Linker_x04114247691676072;

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
struct Linker_x009151707811540066;


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

struct Linker_x018751925173550688;



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
struct Linker_x07594977138045418;



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
struct Linker_x06330718909266773;



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

struct Linker_x03727100494377369;

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
struct Linker_x048782572406891855;

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
struct Linker_x06263097280945717;

struct EventBase
{
    int unused;
};

struct EventBase* EventBase_new(struct Allocator* alloc);

int EventBase_eventCount(struct EventBase* eventBase);

void EventBase_beginLoop(struct EventBase* eventBase);

void EventBase_endLoop(struct EventBase* eventBase);
# 26 "./admin/Admin.h" 2

struct Linker_x012733277166115609;

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
# 16 "net/InterfaceController_admin.c" 2
# 1 "./benc/String.h" 1
# 20 "./benc/String.h"
# 1 "./util/CString.h" 1
# 21 "./util/CString.h"
struct Linker_x01408624225345394;

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

struct Linker_x03870146147718827;


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
# 17 "net/InterfaceController_admin.c" 2

# 1 "./benc/List.h" 1
# 21 "./benc/List.h"
struct Linker_x042293263609782183;

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
# 19 "net/InterfaceController_admin.c" 2
# 1 "./benc/Int.h" 1
# 20 "net/InterfaceController_admin.c" 2
# 1 "./crypto/AddressCalc.h" 1
# 19 "./crypto/AddressCalc.h"
struct Linker_x08897417589437366;
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
# 21 "net/InterfaceController_admin.c" 2
# 1 "./crypto/Key.h" 1
# 19 "./crypto/Key.h"
# 1 "./crypto/random/Random.h" 1
# 21 "./crypto/random/Random.h"
# 1 "./crypto/random/seed/RandomSeed.h" 1
# 21 "./crypto/random/seed/RandomSeed.h"
struct Linker_x07124115530058135;



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

struct Linker_x004608550189375138;



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


struct Linker_x08890369605931743;






int Key_gen(uint8_t addressOut[16],
            uint8_t publicKeyOut[32],
            uint8_t privateKeyOut[32],
            struct Random* rand);

char* Key_parse_strerror(int error);
# 52 "./crypto/Key.h"
int Key_parse(String* key, uint8_t keyBytesOut[32], uint8_t ip6Out[16]);

String* Key_stringify(uint8_t key[32], struct Allocator* alloc);
# 22 "net/InterfaceController_admin.c" 2



# 1 "./net/InterfaceController.h" 1
# 19 "./net/InterfaceController.h"
# 1 "./crypto/CryptoAuth.h" 1
# 18 "./crypto/CryptoAuth.h"
# 1 "./benc/StringList.h" 1
# 23 "./benc/StringList.h"
# 1 "./util/ArrayList.h" 1
# 21 "./util/ArrayList.h"
struct Linker_x07387515489405962;





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
# 22 "./crypto/ReplayProtector.h"
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






struct Linker_x0011382857582591166;







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
# 20 "./net/InterfaceController.h" 2
# 1 "./dht/Address.h" 1
# 21 "./dht/Address.h"
struct Linker_x03601711864516839;
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
# 21 "./net/InterfaceController.h" 2


# 1 "./switch/SwitchCore.h" 1
# 23 "./switch/SwitchCore.h"
struct Linker_x024015365894990803;




struct SwitchCore
{
    struct Iface* routerIf;
};







struct SwitchCore* SwitchCore_new(struct Log* logger,
                                  struct Allocator* allocator,
                                  struct EventBase* base);


int SwitchCore_addInterface(struct SwitchCore* switchCore,
                            struct Iface* iface,
                            struct Allocator* alloc,
                            uint64_t* labelOut);

void SwitchCore_swapInterfaces(struct Iface* if1, struct Iface* if2);



void SwitchCore_setInterfaceState(struct Iface* iface, int ifaceState);
# 24 "./net/InterfaceController.h" 2
# 1 "./net/SwitchPinger.h" 1
# 25 "./net/SwitchPinger.h"
struct Linker_x09935284738666057;





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
# 25 "./net/InterfaceController.h" 2
# 1 "./net/EventEmitter.h" 1
# 20 "./net/EventEmitter.h"
# 1 "./wire/PFChan.h" 1
# 19 "./wire/PFChan.h"
# 1 "./wire/RouteHeader.h" 1
# 19 "./wire/RouteHeader.h"
# 1 "./wire/SwitchHeader.h" 1
# 20 "./wire/SwitchHeader.h"
# 1 "./util/version/Version.h" 1
# 19 "./util/version/Version.h"
struct Linker_x017532927803487897;
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

       
# 21 "./net/EventEmitter.h" 2


struct Linker_x06803446386590672;

struct EventEmitter
{
    int unused;
};






void EventEmitter_regCore(struct EventEmitter* ee,
                          struct Iface* iface,
                          enum PFChan_Pathfinder ev);

void EventEmitter_regPathfinderIface(struct EventEmitter* ee, struct Iface* iface);

struct EventEmitter* EventEmitter_new(struct Allocator* alloc, struct Log* log, uint8_t* publicKey);
# 26 "./net/InterfaceController.h" 2



struct Linker_x07907819468184827;




enum InterfaceController_PeerState
{




    InterfaceController_PeerState_INIT = CryptoAuth_State_INIT,

    InterfaceController_PeerState_SENT_HELLO = CryptoAuth_State_SENT_HELLO,

    InterfaceController_PeerState_RECEIVED_HELLO = CryptoAuth_State_RECEIVED_HELLO,

    InterfaceController_PeerState_SENT_KEY = CryptoAuth_State_SENT_KEY,

    InterfaceController_PeerState_RECEIVED_KEY = CryptoAuth_State_RECEIVED_KEY,


    InterfaceController_PeerState_ESTABLISHED = CryptoAuth_State_ESTABLISHED,


    InterfaceController_PeerState_UNRESPONSIVE = -1,


    InterfaceController_PeerState_UNAUTHENTICATED = -2,
};

static inline char* InterfaceController_stateString(enum InterfaceController_PeerState ps)
{
    switch (ps) {
        case InterfaceController_PeerState_INIT: return "INIT";
        case InterfaceController_PeerState_SENT_HELLO: return "SENT_HELLO";
        case InterfaceController_PeerState_RECEIVED_HELLO: return "RECEIVED_HELLO";
        case InterfaceController_PeerState_SENT_KEY: return "SENT_KEY";
        case InterfaceController_PeerState_RECEIVED_KEY: return "RECEIVED_KEY";
        case InterfaceController_PeerState_ESTABLISHED: return "ESTABLISHED";
        case InterfaceController_PeerState_UNRESPONSIVE: return "UNRESPONSIVE";
        case InterfaceController_PeerState_UNAUTHENTICATED: return "UNAUTHENTICATED";
        default: return "INVALID";
    }
}

enum InterfaceController_BeaconState
{
    InterfaceController_BeaconState_DISABLED,
    InterfaceController_BeaconState_ACCEPTING,
    InterfaceController_BeaconState_SENDING
};

static inline char* InterfaceController_beaconStateString(enum InterfaceController_BeaconState bs)
{
    switch (bs) {
        case InterfaceController_BeaconState_DISABLED: return "DISABLED";
        case InterfaceController_BeaconState_ACCEPTING: return "ACCEPTING";
        case InterfaceController_BeaconState_SENDING: return "SENDING";
        default: return "INVALID";
    }
}




struct InterfaceController_PeerStats
{
    struct Address addr;
    struct Sockaddr* lladdr;
    int state;
    int ifNum;
    uint64_t timeOfLastMessage;
    uint64_t bytesOut;
    uint64_t bytesIn;
    
# 104 "./net/InterfaceController.h" 3 4
   _Bool 
# 104 "./net/InterfaceController.h"
        isIncomingConnection;
    String* user;


    uint32_t duplicates;
    uint32_t lostPackets;
    uint32_t receivedPackets;
    uint32_t receivedOutOfRange;

    uint32_t sendKbps;
    uint32_t recvKbps;
};

struct InterfaceController
{





    
# 124 "./net/InterfaceController.h" 3 4
   _Bool 
# 124 "./net/InterfaceController.h"
        timestampPackets;
};

struct InterfaceController_Iface
{
    struct Iface addrIf;


    int ifNum;

    enum InterfaceController_BeaconState beaconState;

    String* name;
};
# 148 "./net/InterfaceController.h"
struct InterfaceController_Iface* InterfaceController_newIface(struct InterfaceController* ifc,
                                                 String* name,
                                                 struct Allocator* alloc);


int InterfaceController_ifaceCount(struct InterfaceController* ifc);


struct InterfaceController_Iface* InterfaceController_getIface(struct InterfaceController* ifc,
                                                               int ifNum);
# 186 "./net/InterfaceController.h"
int InterfaceController_bootstrapPeer(struct InterfaceController* ifc,
                                      int interfaceNumber,
                                      uint8_t* herPublicKey,
                                      const struct Sockaddr* lladdr,
                                      String* password,
                                      String* login,
                                      String* displayName,
                                      struct Allocator* alloc);






int InterfaceController_beaconState(struct InterfaceController* ifc,
                                    int interfaceNumber,
                                    int newState);
# 211 "./net/InterfaceController.h"
void InterfaceController_resetPeering(struct InterfaceController* ifController,
                                      uint8_t herPublicKey[32]);
# 223 "./net/InterfaceController.h"
int InterfaceController_disconnectPeer(struct InterfaceController* ifc, uint8_t herPublicKey[32]);
# 233 "./net/InterfaceController.h"
int InterfaceController_getPeerStats(struct InterfaceController* ic,
                              struct Allocator* alloc,
                              struct InterfaceController_PeerStats** statsOut);

struct InterfaceController* InterfaceController_new(struct CryptoAuth* ca,
                                      struct SwitchCore* switchCore,
                                      struct Log* logger,
                                      struct EventBase* eventBase,
                                      struct SwitchPinger* switchPinger,
                                      struct Random* rand,
                                      struct Allocator* allocator,
                                      struct EventEmitter* ee);
# 26 "net/InterfaceController_admin.c" 2
# 1 "./net/InterfaceController_admin.h" 1
# 18 "./net/InterfaceController_admin.h"
# 1 "./admin/Admin.h" 1
# 19 "./net/InterfaceController_admin.h" 2



struct Linker_x03437588484015437;

void InterfaceController_admin_register(struct InterfaceController* ic,
                                        struct Admin* admin,
                                        struct Allocator* alloc);
# 27 "net/InterfaceController_admin.c" 2
# 1 "./util/AddrTools.h" 1
# 19 "./util/AddrTools.h"
struct Linker_x02466293064170313;




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
# 28 "net/InterfaceController_admin.c" 2

struct Context
{
    struct Allocator* alloc;
    struct InterfaceController* ic;
    struct Admin* admin;
    unsigned long Identity_verifier;
};




static void adminInterfaces(Dict* args,
                            void* vcontext,
                            String* txid,
                            struct Allocator* alloc)
{
    struct Context* context = (__extension__ ({ __typeof__((struct Context*)vcontext) Identity_ptr = ((struct Context*)vcontext); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x476c44aef90fa187ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("InterfaceController_admin.c"), (45), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x476c44aef90fa187ull))"); } } while (0); Identity_ptr; }));

    int64_t* page = Dict_getInt(args, (&(String) { .bytes = "page", .len = CString_strlen("page") }));
    int i = (page) ? *page * 6 : 0;

    int count = InterfaceController_ifaceCount(context->ic);


    List* list = List_new(alloc);
    for (int counter = 0; i < count && counter++ < 6; i++) {
        struct InterfaceController_Iface* iface = InterfaceController_getIface(context->ic, i);
        Dict* d = Dict_new(alloc);
        Dict_putInt(d, String_new("ifNum", alloc), iface->ifNum, alloc);
        Dict_putString(d, String_new("name", alloc), iface->name, alloc);
        char* bs = InterfaceController_beaconStateString(iface->beaconState);
        Dict_putString(d, String_new("beaconState", alloc), String_new(bs, alloc), alloc);
        List_addDict(list, d, alloc);
    }

    Dict* resp = Dict_new(alloc);
    Dict_putList(resp, String_new("ifaces", alloc), list, alloc);
    Dict_putInt(resp, String_new("total", alloc), count, alloc);
    if (i < count) { Dict_putInt(resp, String_new("more", alloc), 1, alloc); }
    Admin_sendMessage(resp, txid, context->admin);
}

static void adminPeerStats(Dict* args, void* vcontext, String* txid, struct Allocator* alloc)
{
    struct Context* context = (__extension__ ({ __typeof__((struct Context*)vcontext) Identity_ptr = ((struct Context*)vcontext); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x476c44aef90fa187ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("InterfaceController_admin.c"), (73), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x476c44aef90fa187ull))"); } } while (0); Identity_ptr; }));
    struct InterfaceController_PeerStats* stats = 
# 74 "net/InterfaceController_admin.c" 3 4
                                                 ((void *)0)
# 74 "net/InterfaceController_admin.c"
                                                     ;

    int64_t* page = Dict_getInt(args, (&(String) { .bytes = "page", .len = CString_strlen("page") }));
    int i = (page) ? *page * 6 : 0;

    int count = InterfaceController_getPeerStats(context->ic, alloc, &stats);

    List* list = List_new(alloc);
    for (int counter=0; i < count && counter++ < 6; i++) {
        Dict* d = Dict_new(alloc);
        Dict_putInt(d, String_new("bytesIn", alloc), stats[i].bytesIn, alloc);
        Dict_putInt(d, String_new("bytesOut", alloc), stats[i].bytesOut, alloc);

        Dict_putInt(d, String_new("recvKbps", alloc), stats[i].recvKbps, alloc);
        Dict_putInt(d, String_new("sendKbps", alloc), stats[i].sendKbps, alloc);

        Dict_putString(d, String_new("addr", alloc), Address_toString(&stats[i].addr, alloc), alloc);

        String* lladdrString;
# 103 "net/InterfaceController_admin.c"
        lladdrString = String_new(Sockaddr_print(stats[i].lladdr, alloc), alloc);

        Dict_putString(d, String_new("lladdr", alloc), lladdrString, alloc);

        String* stateString = String_new(InterfaceController_stateString(stats[i].state), alloc);
        Dict_putString(d, String_new("state", alloc), stateString, alloc);

        Dict_putInt(d, String_new("last", alloc), stats[i].timeOfLastMessage, alloc);

        Dict_putInt(d, String_new("isIncoming", alloc), stats[i].isIncomingConnection, alloc);
        Dict_putInt(d, String_new("duplicates", alloc), stats[i].duplicates, alloc);
        Dict_putInt(d, String_new("lostPackets", alloc), stats[i].lostPackets, alloc);
        Dict_putInt(d, String_new("receivedOutOfRange", alloc), stats[i].receivedOutOfRange, alloc);

        Dict_putInt(d, String_new("ifNum", alloc), stats[i].ifNum, alloc);

        if (stats[i].user) {
            Dict_putString(d, String_new("user", alloc), stats[i].user, alloc);
        }

        Dict_putInt(d, String_new("receivedPackets", alloc), stats[i].receivedPackets, alloc);

        List_addDict(list, d, alloc);
    }

    Dict* resp = Dict_new(alloc);
    Dict_putList(resp, String_new("peers", alloc), list, alloc);
    Dict_putInt(resp, String_new("total", alloc), count, alloc);

    if (i < count) {
        Dict_putInt(resp, String_new("more", alloc), 1, alloc);
    }

    Admin_sendMessage(resp, txid, context->admin);
}

static void adminDisconnectPeer(Dict* args,
                                void* vcontext,
                                String* txid,
                                struct Allocator* requestAlloc)
{
    struct Context* context = (__extension__ ({ __typeof__((struct Context*)vcontext) Identity_ptr = ((struct Context*)vcontext); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x476c44aef90fa187ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("InterfaceController_admin.c"), (144), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x476c44aef90fa187ull))"); } } while (0); Identity_ptr; }));
    String* pubkeyString = Dict_getString(args, (&(String) { .bytes = "pubkey", .len = CString_strlen("pubkey") }));


    uint8_t pubkey[32];
    uint8_t addr[16];
    int error = Key_parse(pubkeyString, pubkey, addr);

    char* errorMsg = 
# 152 "net/InterfaceController_admin.c" 3 4
                    ((void *)0)
# 152 "net/InterfaceController_admin.c"
                        ;
    if (error) {
        errorMsg = "bad key";
    } else {

        error = InterfaceController_disconnectPeer(context->ic,pubkey);
        if (error) {
            errorMsg = "no peer found for that key";
        }
    }

    Dict* response = Dict_new(requestAlloc);
    Dict_putInt(response, String_new("success", requestAlloc), error ? 0 : 1, requestAlloc);
    if (error) {
        Dict_putString(response, String_new("error", requestAlloc), String_new(errorMsg, requestAlloc), requestAlloc);
    }

    Admin_sendMessage(response, txid, context->admin);
}

static void adminResetPeering(Dict* args,
                              void* vcontext,
                              String* txid,
                              struct Allocator* requestAlloc)
{
    struct Context* context = (__extension__ ({ __typeof__((struct Context*)vcontext) Identity_ptr = ((struct Context*)vcontext); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x476c44aef90fa187ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("InterfaceController_admin.c"), (177), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x476c44aef90fa187ull))"); } } while (0); Identity_ptr; }));
    String* pubkeyString = Dict_getString(args, (&(String) { .bytes = "pubkey", .len = CString_strlen("pubkey") }));

    int error = 0;
    char* errorMsg = 
# 181 "net/InterfaceController_admin.c" 3 4
                    ((void *)0)
# 181 "net/InterfaceController_admin.c"
                        ;

    if (pubkeyString) {

        uint8_t pubkey[32];
        uint8_t addr[16];
        error = Key_parse(pubkeyString, pubkey, addr);

        if (error) {
            errorMsg = "bad key";
        } else {
            InterfaceController_resetPeering(context->ic, pubkey);
        }
    } else {

        InterfaceController_resetPeering(context->ic, 
# 196 "net/InterfaceController_admin.c" 3 4
                                                     ((void *)0)
# 196 "net/InterfaceController_admin.c"
                                                         );
    }

    Dict* response = Dict_new(requestAlloc);
    Dict_putInt(response, String_new("success", requestAlloc), error ? 0 : 1, requestAlloc);
    if (error) {
        Dict_putString(response, String_new("error", requestAlloc), String_new(errorMsg, requestAlloc), requestAlloc);
    }

    Admin_sendMessage(response, txid, context->admin);
}

static void timestampPackets(Dict* args,
                             void* vcontext,
                             String* txid,
                             struct Allocator* requestAlloc)
{
    struct Context* context = (__extension__ ({ __typeof__((struct Context*)vcontext) Identity_ptr = ((struct Context*)vcontext); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x476c44aef90fa187ull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("InterfaceController_admin.c"), (213), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x476c44aef90fa187ull))"); } } while (0); Identity_ptr; }));
    int64_t* enable = Dict_getInt(args, (&(String) { .bytes = "enable", .len = CString_strlen("enable") }));
    Dict* response = Dict_new(requestAlloc);
    Dict_putString(response, String_new("error", requestAlloc), String_new("none", requestAlloc), requestAlloc);
    if (!enable) {
        Dict_putInt(response, String_new("enabled", requestAlloc), context->ic->timestampPackets, requestAlloc);
    } else {
        context->ic->timestampPackets = *enable;
    }
    Admin_sendMessage(response, txid, context->admin);
}
# 256 "net/InterfaceController_admin.c"
void InterfaceController_admin_register(struct InterfaceController* ic,
                                        struct Admin* admin,
                                        struct Allocator* allocator)
{
    struct Allocator* alloc = Allocator__child((allocator),"InterfaceController_admin.c",260);
    struct Context* ctx = Allocator__clone((alloc),((&(struct Context) { .alloc = alloc, .ic = ic, .admin = admin })),sizeof(*((&(struct Context) { .alloc = alloc, .ic = ic, .admin = admin }))),"InterfaceController_admin.c",261)



       ;
    (ctx)->Identity_verifier = ((unsigned long) 0x476c44aef90fa187ull);

    Admin_registerFunctionWithArgCount( "InterfaceController_interfaces", adminInterfaces, ctx, 
# 268 "net/InterfaceController_admin.c" 3 4
   1
# 268 "net/InterfaceController_admin.c"
   , ((struct Admin_FunctionArg[]) { { .name = "page", .required = 0, .type = "Int" } }), (sizeof(((struct Admin_FunctionArg[]) { { .name = "page", .required = 0, .type = "Int" } })) / sizeof(struct Admin_FunctionArg)), admin)


                  ;

    Admin_registerFunctionWithArgCount( "InterfaceController_peerStats", adminPeerStats, ctx, 
# 273 "net/InterfaceController_admin.c" 3 4
   0
# 273 "net/InterfaceController_admin.c"
   , ((struct Admin_FunctionArg[]) { { .name = "page", .required = 0, .type = "Int" } }), (sizeof(((struct Admin_FunctionArg[]) { { .name = "page", .required = 0, .type = "Int" } })) / sizeof(struct Admin_FunctionArg)), admin)


                  ;

    Admin_registerFunctionWithArgCount( "InterfaceController_resetPeering", adminResetPeering, ctx, 
# 278 "net/InterfaceController_admin.c" 3 4
   1
# 278 "net/InterfaceController_admin.c"
   , ((struct Admin_FunctionArg[]) { { .name = "pubkey", .required = 0, .type = "String" } }), (sizeof(((struct Admin_FunctionArg[]) { { .name = "pubkey", .required = 0, .type = "String" } })) / sizeof(struct Admin_FunctionArg)), admin)


                  ;

    Admin_registerFunctionWithArgCount( "InterfaceController_disconnectPeer", adminDisconnectPeer, ctx, 
# 283 "net/InterfaceController_admin.c" 3 4
   1
# 283 "net/InterfaceController_admin.c"
   , ((struct Admin_FunctionArg[]) { { .name = "pubkey", .required = 1, .type = "String" } }), (sizeof(((struct Admin_FunctionArg[]) { { .name = "pubkey", .required = 1, .type = "String" } })) / sizeof(struct Admin_FunctionArg)), admin)


                  ;

    Admin_registerFunctionWithArgCount( "InterfaceController_timestampPackets", timestampPackets, ctx, 
# 288 "net/InterfaceController_admin.c" 3 4
   1
# 288 "net/InterfaceController_admin.c"
   , ((struct Admin_FunctionArg[]) { { .name = "enable", .required = 0, .type = "Int" } }), (sizeof(((struct Admin_FunctionArg[]) { { .name = "enable", .required = 0, .type = "Int" } })) / sizeof(struct Admin_FunctionArg)), admin)


                  ;

}
