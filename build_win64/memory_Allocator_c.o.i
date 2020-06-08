# 1 "memory/Allocator.c"
# 1 "/home/jojapoppa/cjdns//"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "memory/Allocator.c"
# 16 "memory/Allocator.c"
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
struct Linker_x05953623209225556;
# 33 "./util/Assert.h"
__attribute__((__format__ (__printf__, 1, 2)))
__attribute__((__noreturn__))
void Assert_failure(const char* format, ...);
# 19 "./util/Identity.h" 2
# 1 "./util/Constant.h" 1
# 33 "./util/Constant.h"

# 20 "./util/Identity.h" 2



    
# 19 "./memory/Allocator.h" 2


struct Linker_x07457234959282539;





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
# 17 "memory/Allocator.c" 2


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
# 1 "./memory/Allocator_pvt.h" 1
# 22 "./memory/Allocator_pvt.h"
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
# 23 "./memory/Allocator_pvt.h" 2


# 24 "./memory/Allocator_pvt.h"
struct Allocator_pvt;

struct Allocator_OnFreeJob_pvt;
struct Allocator_OnFreeJob_pvt {
    struct Allocator_OnFreeJob pub;
    struct Allocator_pvt* alloc;
    struct Allocator_OnFreeJob_pvt* next;


    int called;

    int line;
    const char* file;


    unsigned long Identity_verifier;
};

struct Allocator_Allocation_pvt;






struct Allocator_Allocation_pvt {
    struct Allocator_Allocation pub;
    struct Allocator_Allocation_pvt* next;






    long lineNum;
    const char* fileName;
};

void Assert_compileTime(char x[1 - (!(sizeof(struct Allocator_Allocation_pvt) == ( 4 + 8 + 4 + 8 )))]);
void Assert_compileTime(char x[1 - (!(!(( 4 + 8 + 4 + 8 ) % 16)))]);


struct Allocator_List;
struct Allocator_List {
    struct Allocator_pvt* alloc;
    struct Allocator_List* next;
};

struct Allocator_Adoptions {
    struct Allocator_List* parents;
    struct Allocator_List* children;
};


struct Allocator_pvt
{

    struct Allocator pub;





    struct Allocator_Allocation_pvt* allocations;


    struct Allocator_OnFreeJob_pvt* onFree;


    struct Allocator_pvt* parent;





    struct Allocator_pvt* lastSibling;


    struct Allocator_pvt* nextSibling;


    struct Allocator_pvt* firstChild;


    struct Allocator_FirstCtx* rootAlloc;


    unsigned long allocatedHere;






    struct Allocator_Adoptions* adoptions;



        unsigned long canary;


        unsigned long nextCanary;



    unsigned long Identity_verifier;
};


struct Allocator_FirstCtx
{

    struct Allocator_pvt context;

    Allocator_Provider provider;

    void* providerContext;


    int64_t spaceAvailable;


    int64_t maxSpace;

    unsigned long Identity_verifier;
};
# 22 "memory/Allocator.c" 2


# 1 "./util/Bits.h" 1
# 21 "./util/Bits.h"
struct Linker_x03385941140094606;


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
# 25 "memory/Allocator.c" 2
# 1 "./util/Defined.h" 1
# 26 "memory/Allocator.c" 2

# 1 "/usr/share/mingw-w64/include/stdio.h" 1 3
# 11 "/usr/share/mingw-w64/include/stdio.h" 3
# 1 "/usr/share/mingw-w64/include/_mingw_print_push.h" 1 3
# 12 "/usr/share/mingw-w64/include/stdio.h" 2 3

#pragma pack(push,_CRT_PACKING)
# 26 "/usr/share/mingw-w64/include/stdio.h" 3
  
# 26 "/usr/share/mingw-w64/include/stdio.h" 3
 struct _iobuf {
    char *_ptr;
    int _cnt;
    char *_base;
    int _flag;
    int _file;
    int _charbuf;
    int _bufsiz;
    char *_tmpfname;
  };
  typedef struct _iobuf FILE;
# 80 "/usr/share/mingw-w64/include/stdio.h" 3
# 1 "/usr/share/mingw-w64/include/_mingw_off_t.h" 1 3




  typedef long _off_t;

  typedef long off32_t;





  __extension__ typedef long long _off64_t;

  __extension__ typedef long long off64_t;
# 26 "/usr/share/mingw-w64/include/_mingw_off_t.h" 3
typedef off32_t off_t;
# 81 "/usr/share/mingw-w64/include/stdio.h" 2 3

__attribute__ ((__dllimport__)) FILE *__attribute__((__cdecl__)) __acrt_iob_func(unsigned index);


  __attribute__ ((__dllimport__)) FILE *__attribute__((__cdecl__)) __iob_func(void);
# 104 "/usr/share/mingw-w64/include/stdio.h" 3
  __extension__ typedef long long fpos_t;
# 162 "/usr/share/mingw-w64/include/stdio.h" 3
extern
  __attribute__((__format__ (gnu_scanf, 2, 3))) __attribute__ ((__nonnull__ (2)))
  int __attribute__((__cdecl__)) __mingw_sscanf(const char * __restrict__ _Src,const char * __restrict__ _Format,...);
extern
  __attribute__((__format__ (gnu_scanf, 2, 0))) __attribute__ ((__nonnull__ (2)))
  int __attribute__((__cdecl__)) __mingw_vsscanf (const char * __restrict__ _Str,const char * __restrict__ Format,va_list argp);
extern
  __attribute__((__format__ (gnu_scanf, 1, 2))) __attribute__ ((__nonnull__ (1)))
  int __attribute__((__cdecl__)) __mingw_scanf(const char * __restrict__ _Format,...);
extern
  __attribute__((__format__ (gnu_scanf, 1, 0))) __attribute__ ((__nonnull__ (1)))
  int __attribute__((__cdecl__)) __mingw_vscanf(const char * __restrict__ Format, va_list argp);
extern
  __attribute__((__format__ (gnu_scanf, 2, 3))) __attribute__ ((__nonnull__ (2)))
  int __attribute__((__cdecl__)) __mingw_fscanf(FILE * __restrict__ _File,const char * __restrict__ _Format,...);
extern
  __attribute__((__format__ (gnu_scanf, 2, 0))) __attribute__ ((__nonnull__ (2)))
  int __attribute__((__cdecl__)) __mingw_vfscanf (FILE * __restrict__ fp, const char * __restrict__ Format,va_list argp);

extern
  __attribute__((__format__ (gnu_printf, 3, 0))) __attribute__ ((__nonnull__ (3)))
  int __attribute__((__cdecl__)) __mingw_vsnprintf(char * __restrict__ _DstBuf,size_t _MaxCount,const char * __restrict__ _Format,
                               va_list _ArgList);
extern
  __attribute__((__format__ (gnu_printf, 3, 4))) __attribute__ ((__nonnull__ (3)))
  int __attribute__((__cdecl__)) __mingw_snprintf(char * __restrict__ s, size_t n, const char * __restrict__ format, ...);
extern
  __attribute__((__format__ (gnu_printf, 1, 2))) __attribute__ ((__nonnull__ (1)))
  int __attribute__((__cdecl__)) __mingw_printf(const char * __restrict__ , ... ) __attribute__ ((__nothrow__));
extern
  __attribute__((__format__ (gnu_printf, 1, 0))) __attribute__ ((__nonnull__ (1)))
  int __attribute__((__cdecl__)) __mingw_vprintf (const char * __restrict__ , va_list) __attribute__ ((__nothrow__));
extern
  __attribute__((__format__ (gnu_printf, 2, 3))) __attribute__ ((__nonnull__ (2)))
  int __attribute__((__cdecl__)) __mingw_fprintf (FILE * __restrict__ , const char * __restrict__ , ...) __attribute__ ((__nothrow__));
extern
  __attribute__((__format__ (gnu_printf, 2, 0))) __attribute__ ((__nonnull__ (2)))
  int __attribute__((__cdecl__)) __mingw_vfprintf (FILE * __restrict__ , const char * __restrict__ , va_list) __attribute__ ((__nothrow__));
extern
  __attribute__((__format__ (gnu_printf, 2, 3))) __attribute__ ((__nonnull__ (2)))
  int __attribute__((__cdecl__)) __mingw_sprintf (char * __restrict__ , const char * __restrict__ , ...) __attribute__ ((__nothrow__));
extern
  __attribute__((__format__ (gnu_printf, 2, 0))) __attribute__ ((__nonnull__ (2)))
  int __attribute__((__cdecl__)) __mingw_vsprintf (char * __restrict__ , const char * __restrict__ , va_list) __attribute__ ((__nothrow__));
extern
  __attribute__((__format__ (gnu_printf, 2, 3))) __attribute__((nonnull (1,2)))
  int __attribute__((__cdecl__)) __mingw_asprintf(char ** __restrict__ , const char * __restrict__ , ...) __attribute__ ((__nothrow__));
extern
  __attribute__((__format__ (gnu_printf, 2, 0))) __attribute__((nonnull (1,2)))
  int __attribute__((__cdecl__)) __mingw_vasprintf(char ** __restrict__ , const char * __restrict__ , va_list) __attribute__ ((__nothrow__));
# 506 "/usr/share/mingw-w64/include/stdio.h" 3
  __attribute__((__format__ (ms_printf, 2, 3))) __attribute__ ((__nonnull__ (2)))
  int __attribute__((__cdecl__)) fprintf(FILE * __restrict__ _File,const char * __restrict__ _Format,...);
  __attribute__((__format__ (ms_printf, 1, 2))) __attribute__ ((__nonnull__ (1)))
  int __attribute__((__cdecl__)) printf(const char * __restrict__ _Format,...);
  __attribute__((__format__ (ms_printf, 2, 3))) __attribute__ ((__nonnull__ (2)))
  int __attribute__((__cdecl__)) sprintf(char * __restrict__ _Dest,const char * __restrict__ _Format,...) ;

  __attribute__((__format__ (ms_printf, 2, 0))) __attribute__ ((__nonnull__ (2)))
  int __attribute__((__cdecl__)) vfprintf(FILE * __restrict__ _File,const char * __restrict__ _Format,va_list _ArgList);
  __attribute__((__format__ (ms_printf, 1, 0))) __attribute__ ((__nonnull__ (1)))
  int __attribute__((__cdecl__)) vprintf(const char * __restrict__ _Format,va_list _ArgList);
  __attribute__((__format__ (ms_printf, 2, 0))) __attribute__ ((__nonnull__ (2)))
  int __attribute__((__cdecl__)) vsprintf(char * __restrict__ _Dest,const char * __restrict__ _Format,va_list _Args) ;

  __attribute__((__format__ (ms_scanf, 2, 3))) __attribute__ ((__nonnull__ (2)))
  int __attribute__((__cdecl__)) fscanf(FILE * __restrict__ _File,const char * __restrict__ _Format,...) ;
  __attribute__((__format__ (ms_scanf, 1, 2))) __attribute__ ((__nonnull__ (1)))
  int __attribute__((__cdecl__)) scanf(const char * __restrict__ _Format,...) ;
  __attribute__((__format__ (ms_scanf, 2, 3))) __attribute__ ((__nonnull__ (2)))
  int __attribute__((__cdecl__)) sscanf(const char * __restrict__ _Src,const char * __restrict__ _Format,...) ;






#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow"


  __attribute__((__format__ (ms_scanf, 1, 0))) __attribute__ ((__nonnull__ (1)))
  int __attribute__((__cdecl__)) __ms_vscanf(const char * __restrict__ Format, va_list argp);
  __attribute__((__format__ (ms_scanf, 2, 0))) __attribute__ ((__nonnull__ (2)))
  int __attribute__((__cdecl__)) __ms_vfscanf (FILE * __restrict__ fp, const char * __restrict__ Format,va_list argp);
  __attribute__((__format__ (ms_scanf, 2, 0))) __attribute__ ((__nonnull__ (2)))
  int __attribute__((__cdecl__)) __ms_vsscanf (const char * __restrict__ _Str,const char * __restrict__ Format,va_list argp);

  static __attribute__ ((__unused__)) __inline__ __attribute__((__cdecl__))
  __attribute__((__format__ (ms_scanf, 2, 0))) __attribute__ ((__nonnull__ (2)))
  int vfscanf (FILE *__stream, const char *__format, __builtin_va_list __local_argv)
  {
    return __ms_vfscanf (__stream, __format, __local_argv);
  }

  static __attribute__ ((__unused__)) __inline__ __attribute__((__cdecl__))
  __attribute__((__format__ (ms_scanf, 2, 0))) __attribute__ ((__nonnull__ (2)))
  int vsscanf (const char * __restrict__ __source, const char * __restrict__ __format, __builtin_va_list __local_argv)
  {
    return __ms_vsscanf( __source, __format, __local_argv );
  }
  static __attribute__ ((__unused__)) __inline__ __attribute__((__cdecl__))
  __attribute__((__format__ (ms_scanf, 1, 0))) __attribute__ ((__nonnull__ (1)))
  int vscanf(const char *__format, __builtin_va_list __local_argv)
  {
    return __ms_vscanf (__format, __local_argv);
  }


#pragma GCC diagnostic pop






  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _filbuf(FILE *_File);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _flsbuf(int _Ch,FILE *_File);



  __attribute__ ((__dllimport__)) FILE *__attribute__((__cdecl__)) _fsopen(const char *_Filename,const char *_Mode,int _ShFlag);

  void __attribute__((__cdecl__)) clearerr(FILE *_File);
  int __attribute__((__cdecl__)) fclose(FILE *_File);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _fcloseall(void);



  __attribute__ ((__dllimport__)) FILE *__attribute__((__cdecl__)) _fdopen(int _FileHandle,const char *_Mode);

  int __attribute__((__cdecl__)) feof(FILE *_File);
  int __attribute__((__cdecl__)) ferror(FILE *_File);
  int __attribute__((__cdecl__)) fflush(FILE *_File);
  int __attribute__((__cdecl__)) fgetc(FILE *_File);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _fgetchar(void);
  int __attribute__((__cdecl__)) fgetpos(FILE * __restrict__ _File ,fpos_t * __restrict__ _Pos);
  int __attribute__((__cdecl__)) fgetpos64(FILE * __restrict__ _File ,fpos_t * __restrict__ _Pos);
  char *__attribute__((__cdecl__)) fgets(char * __restrict__ _Buf,int _MaxCount,FILE * __restrict__ _File);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _fileno(FILE *_File);



  __attribute__ ((__dllimport__)) char *__attribute__((__cdecl__)) _tempnam(const char *_DirName,const char *_FilePrefix);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _flushall(void);
  FILE *__attribute__((__cdecl__)) fopen(const char * __restrict__ _Filename,const char * __restrict__ _Mode) ;
  FILE *fopen64(const char * __restrict__ filename,const char * __restrict__ mode);
  int __attribute__((__cdecl__)) fputc(int _Ch,FILE *_File);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _fputchar(int _Ch);
  int __attribute__((__cdecl__)) fputs(const char * __restrict__ _Str,FILE * __restrict__ _File);
  size_t __attribute__((__cdecl__)) fread(void * __restrict__ _DstBuf,size_t _ElementSize,size_t _Count,FILE * __restrict__ _File);
  FILE *__attribute__((__cdecl__)) freopen(const char * __restrict__ _Filename,const char * __restrict__ _Mode,FILE * __restrict__ _File) ;
  int __attribute__((__cdecl__)) fsetpos(FILE *_File,const fpos_t *_Pos);
  int __attribute__((__cdecl__)) fsetpos64(FILE *_File,const fpos_t *_Pos);
  int __attribute__((__cdecl__)) fseek(FILE *_File,long _Offset,int _Origin);
  long __attribute__((__cdecl__)) ftell(FILE *_File);
# 631 "/usr/share/mingw-w64/include/stdio.h" 3
  __extension__ int __attribute__((__cdecl__)) _fseeki64(FILE *_File,long long _Offset,int _Origin);
  __extension__ long long __attribute__((__cdecl__)) _ftelli64(FILE *_File);
  int fseeko64(FILE* stream, _off64_t offset, int whence);
  int fseeko(FILE* stream, _off_t offset, int whence);

  _off_t ftello(FILE * stream);
  _off64_t ftello64(FILE * stream);
# 654 "/usr/share/mingw-w64/include/stdio.h" 3
  size_t __attribute__((__cdecl__)) fwrite(const void * __restrict__ _Str,size_t _Size,size_t _Count,FILE * __restrict__ _File);
  int __attribute__((__cdecl__)) getc(FILE *_File);
  int __attribute__((__cdecl__)) getchar(void);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _getmaxstdio(void);
  char *__attribute__((__cdecl__)) gets(char *_Buffer) ;
  int __attribute__((__cdecl__)) _getw(FILE *_File);


  void __attribute__((__cdecl__)) perror(const char *_ErrMsg);

  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _pclose(FILE *_File);
  __attribute__ ((__dllimport__)) FILE *__attribute__((__cdecl__)) _popen(const char *_Command,const char *_Mode);




  int __attribute__((__cdecl__)) putc(int _Ch,FILE *_File);
  int __attribute__((__cdecl__)) putchar(int _Ch);
  int __attribute__((__cdecl__)) puts(const char *_Str);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _putw(int _Word,FILE *_File);


  int __attribute__((__cdecl__)) remove(const char *_Filename);
  int __attribute__((__cdecl__)) rename(const char *_OldFilename,const char *_NewFilename);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _unlink(const char *_Filename);

  int __attribute__((__cdecl__)) unlink(const char *_Filename) ;


  void __attribute__((__cdecl__)) rewind(FILE *_File);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _rmtmp(void);
  void __attribute__((__cdecl__)) setbuf(FILE * __restrict__ _File,char * __restrict__ _Buffer) ;
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _setmaxstdio(int _Max);
  __attribute__ ((__dllimport__)) unsigned int __attribute__((__cdecl__)) _set_output_format(unsigned int _Format);
  __attribute__ ((__dllimport__)) unsigned int __attribute__((__cdecl__)) _get_output_format(void);
  int __attribute__((__cdecl__)) setvbuf(FILE * __restrict__ _File,char * __restrict__ _Buf,int _Mode,size_t _Size);
# 712 "/usr/share/mingw-w64/include/stdio.h" 3
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _scprintf(const char * __restrict__ _Format,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _snscanf(const char * __restrict__ _Src,size_t _MaxCount,const char * __restrict__ _Format,...) ;

  FILE *__attribute__((__cdecl__)) tmpfile(void) ;
  char *__attribute__((__cdecl__)) tmpnam(char *_Buffer);
  int __attribute__((__cdecl__)) ungetc(int _Ch,FILE *_File);
# 734 "/usr/share/mingw-w64/include/stdio.h" 3
  __attribute__((__format__ (ms_printf, 3, 4))) __attribute__ ((__nonnull__ (3)))
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _snprintf(char * __restrict__ _Dest,size_t _Count,const char * __restrict__ _Format,...) ;
  __attribute__((__format__ (ms_printf, 3, 0))) __attribute__ ((__nonnull__ (3)))
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vsnprintf(char * __restrict__ _Dest,size_t _Count,const char * __restrict__ _Format,va_list _Args) ;
# 761 "/usr/share/mingw-w64/include/stdio.h" 3
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow"


       
       


  __attribute__((__format__ (ms_printf, 3, 0))) __attribute__ ((__nonnull__ (3)))
  int __attribute__((__cdecl__)) __ms_vsnprintf(char * __restrict__ d,size_t n,const char * __restrict__ format,va_list arg)
    ;

  static __attribute__ ((__unused__)) __inline__ __attribute__((__cdecl__))
  __attribute__((__format__ (ms_printf, 3, 0))) __attribute__ ((__nonnull__ (3)))
  int vsnprintf (char * __restrict__ __stream, size_t __n, const char * __restrict__ __format, va_list __local_argv)
  {
    return __ms_vsnprintf (__stream, __n, __format, __local_argv);
  }

  __attribute__((__format__ (ms_printf, 3, 4))) __attribute__ ((__nonnull__ (3)))
  int __attribute__((__cdecl__)) __ms_snprintf(char * __restrict__ s, size_t n, const char * __restrict__ format, ...);


static __attribute__ ((__unused__)) __inline__ __attribute__((__cdecl__))
__attribute__((__format__ (ms_printf, 3, 4))) __attribute__ ((__nonnull__ (3)))
int snprintf (char * __restrict__ __stream, size_t __n, const char * __restrict__ __format, ...)
{
  int __retval;
  __builtin_va_list __local_argv; __builtin_va_start( __local_argv, __format );
  __retval = __ms_vsnprintf (__stream, __n, __format, __local_argv);
  __builtin_va_end( __local_argv );
  return __retval;
}


       
       

#pragma GCC diagnostic pop
# 811 "/usr/share/mingw-w64/include/stdio.h" 3
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vscprintf(const char * __restrict__ _Format,va_list _ArgList);


  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _set_printf_count_output(int _Value);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _get_printf_count_output(void);




                                                     __attribute__ ((__nonnull__ (2)))
  int __attribute__((__cdecl__)) __mingw_swscanf(const wchar_t * __restrict__ _Src,const wchar_t * __restrict__ _Format,...);
                                                     __attribute__ ((__nonnull__ (2)))
  int __attribute__((__cdecl__)) __mingw_vswscanf (const wchar_t * __restrict__ _Str,const wchar_t * __restrict__ Format,va_list argp);
                                                     __attribute__ ((__nonnull__ (1)))
  int __attribute__((__cdecl__)) __mingw_wscanf(const wchar_t * __restrict__ _Format,...);
                                                     __attribute__ ((__nonnull__ (1)))
  int __attribute__((__cdecl__)) __mingw_vwscanf(const wchar_t * __restrict__ Format, va_list argp);
                                                     __attribute__ ((__nonnull__ (2)))
  int __attribute__((__cdecl__)) __mingw_fwscanf(FILE * __restrict__ _File,const wchar_t * __restrict__ _Format,...);
                                                     __attribute__ ((__nonnull__ (2)))
  int __attribute__((__cdecl__)) __mingw_vfwscanf (FILE * __restrict__ fp, const wchar_t * __restrict__ Format,va_list argp);

                                                      __attribute__ ((__nonnull__ (2)))
  int __attribute__((__cdecl__)) __mingw_fwprintf(FILE * __restrict__ _File,const wchar_t * __restrict__ _Format,...);
                                                      __attribute__ ((__nonnull__ (1)))
  int __attribute__((__cdecl__)) __mingw_wprintf(const wchar_t * __restrict__ _Format,...);
                                                     __attribute__ ((__nonnull__ (2)))
  int __attribute__((__cdecl__)) __mingw_vfwprintf(FILE * __restrict__ _File,const wchar_t * __restrict__ _Format,va_list _ArgList);
                                                     __attribute__ ((__nonnull__ (1)))
  int __attribute__((__cdecl__)) __mingw_vwprintf(const wchar_t * __restrict__ _Format,va_list _ArgList);
                                                      __attribute__ ((__nonnull__ (3)))
  int __attribute__((__cdecl__)) __mingw_snwprintf (wchar_t * __restrict__ s, size_t n, const wchar_t * __restrict__ format, ...);
                                                      __attribute__ ((__nonnull__ (3)))
  int __attribute__((__cdecl__)) __mingw_vsnwprintf (wchar_t * __restrict__ , size_t, const wchar_t * __restrict__ , va_list);
                                                      __attribute__ ((__nonnull__ (2)))
  int __attribute__((__cdecl__)) __mingw_swprintf(wchar_t * __restrict__ , const wchar_t * __restrict__ , ...);
                                                      __attribute__ ((__nonnull__ (2)))
  int __attribute__((__cdecl__)) __mingw_vswprintf(wchar_t * __restrict__ , const wchar_t * __restrict__ ,va_list);
# 1061 "/usr/share/mingw-w64/include/stdio.h" 3
  int __attribute__((__cdecl__)) fwscanf(FILE * __restrict__ _File,const wchar_t * __restrict__ _Format,...) ;
  int __attribute__((__cdecl__)) swscanf(const wchar_t * __restrict__ _Src,const wchar_t * __restrict__ _Format,...) ;
  int __attribute__((__cdecl__)) wscanf(const wchar_t * __restrict__ _Format,...) ;

  int __attribute__((__cdecl__)) __ms_vwscanf (const wchar_t * __restrict__ , va_list);
  int __attribute__((__cdecl__)) __ms_vfwscanf (FILE * __restrict__ ,const wchar_t * __restrict__ ,va_list);
  int __attribute__((__cdecl__)) __ms_vswscanf (const wchar_t * __restrict__ ,const wchar_t * __restrict__ ,va_list);

  static __attribute__ ((__unused__)) __inline__ __attribute__((__cdecl__))
  __attribute__ ((__nonnull__ (2)))
  int vfwscanf (FILE *__stream, const wchar_t *__format, __builtin_va_list __local_argv)
  {
    return __ms_vfwscanf (__stream, __format, __local_argv);
  }

  static __attribute__ ((__unused__)) __inline__ __attribute__((__cdecl__))
  __attribute__ ((__nonnull__ (2)))
  int vswscanf (const wchar_t * __restrict__ __source, const wchar_t * __restrict__ __format, __builtin_va_list __local_argv)
  {
    return __ms_vswscanf( __source, __format, __local_argv );
  }
  static __attribute__ ((__unused__)) __inline__ __attribute__((__cdecl__))
  __attribute__ ((__nonnull__ (1)))
  int vwscanf(const wchar_t *__format, __builtin_va_list __local_argv)
  {
    return __ms_vwscanf (__format, __local_argv);
  }



  int __attribute__((__cdecl__)) fwprintf(FILE * __restrict__ _File,const wchar_t * __restrict__ _Format,...);
  int __attribute__((__cdecl__)) wprintf(const wchar_t * __restrict__ _Format,...);
  int __attribute__((__cdecl__)) vfwprintf(FILE * __restrict__ _File,const wchar_t * __restrict__ _Format,va_list _ArgList);
  int __attribute__((__cdecl__)) vwprintf(const wchar_t * __restrict__ _Format,va_list _ArgList);
# 1105 "/usr/share/mingw-w64/include/stdio.h" 3
  __attribute__ ((__dllimport__)) FILE *__attribute__((__cdecl__)) _wfsopen(const wchar_t *_Filename,const wchar_t *_Mode,int _ShFlag);


  wint_t __attribute__((__cdecl__)) fgetwc(FILE *_File);
  __attribute__ ((__dllimport__)) wint_t __attribute__((__cdecl__)) _fgetwchar(void);
  wint_t __attribute__((__cdecl__)) fputwc(wchar_t _Ch,FILE *_File);
  __attribute__ ((__dllimport__)) wint_t __attribute__((__cdecl__)) _fputwchar(wchar_t _Ch);
  wint_t __attribute__((__cdecl__)) getwc(FILE *_File);
  wint_t __attribute__((__cdecl__)) getwchar(void);
  wint_t __attribute__((__cdecl__)) putwc(wchar_t _Ch,FILE *_File);
  wint_t __attribute__((__cdecl__)) putwchar(wchar_t _Ch);
  wint_t __attribute__((__cdecl__)) ungetwc(wint_t _Ch,FILE *_File);
  wchar_t *__attribute__((__cdecl__)) fgetws(wchar_t * __restrict__ _Dst,int _SizeInWords,FILE * __restrict__ _File);
  int __attribute__((__cdecl__)) fputws(const wchar_t * __restrict__ _Str,FILE * __restrict__ _File);
  __attribute__ ((__dllimport__)) wchar_t *__attribute__((__cdecl__)) _getws(wchar_t *_String) ;
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _putws(const wchar_t *_Str);
# 1186 "/usr/share/mingw-w64/include/stdio.h" 3
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _scwprintf(const wchar_t * __restrict__ _Format,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _swprintf_c(wchar_t * __restrict__ _DstBuf,size_t _SizeInWords,const wchar_t * __restrict__ _Format,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vswprintf_c(wchar_t * __restrict__ _DstBuf,size_t _SizeInWords,const wchar_t * __restrict__ _Format,va_list _ArgList);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _snwprintf(wchar_t * __restrict__ _Dest,size_t _Count,const wchar_t * __restrict__ _Format,...) ;
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vsnwprintf(wchar_t * __restrict__ _Dest,size_t _Count,const wchar_t * __restrict__ _Format,va_list _Args) ;
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vscwprintf(const wchar_t * __restrict__ _Format,va_list _ArgList);




       
       


  int __attribute__((__cdecl__)) __ms_snwprintf (wchar_t * __restrict__ s, size_t n, const wchar_t * __restrict__ format, ...);
  int __attribute__((__cdecl__)) __ms_vsnwprintf (wchar_t * __restrict__ , size_t, const wchar_t * __restrict__ , va_list);
  static __attribute__ ((__unused__)) __inline__ __attribute__((__cdecl__))
  int snwprintf (wchar_t * __restrict__ s, size_t n, const wchar_t * __restrict__ format, ...)
  {
    int r;
    va_list argp;
    __builtin_va_start (argp, format);
    r = _vsnwprintf (s, n, format, argp);
    __builtin_va_end (argp);
    return r;
  }
  static __attribute__ ((__unused__)) __inline__ __attribute__((__cdecl__))
  int __attribute__((__cdecl__)) vsnwprintf (wchar_t * __restrict__ s, size_t n, const wchar_t * __restrict__ format, va_list arg)
  {
    return _vsnwprintf(s,n,format,arg);
  }
       
       



  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _swprintf(wchar_t * __restrict__ _Dest,const wchar_t * __restrict__ _Format,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vswprintf(wchar_t * __restrict__ _Dest,const wchar_t * __restrict__ _Format,va_list _Args);



# 1 "/usr/share/mingw-w64/include/swprintf.inl" 1 3
# 25 "/usr/share/mingw-w64/include/swprintf.inl" 3
static __attribute__ ((__unused__)) __inline__ __attribute__((__cdecl__))
                                                      __attribute__ ((__nonnull__ (3)))
int vswprintf (wchar_t *__stream, size_t __count, const wchar_t *__format, __builtin_va_list __local_argv)
{
  return vsnwprintf( __stream, __count, __format, __local_argv );
}

static __attribute__ ((__unused__)) __inline__ __attribute__((__cdecl__))
                                                      __attribute__ ((__nonnull__ (3)))
int swprintf (wchar_t *__stream, size_t __count, const wchar_t *__format, ...)
{
  int __retval;
  __builtin_va_list __local_argv;

  __builtin_va_start( __local_argv, __format );
  __retval = vswprintf( __stream, __count, __format, __local_argv );
  __builtin_va_end( __local_argv );
  return __retval;
}
# 1228 "/usr/share/mingw-w64/include/stdio.h" 2 3
# 1237 "/usr/share/mingw-w64/include/stdio.h" 3
  __attribute__ ((__dllimport__)) wchar_t *__attribute__((__cdecl__)) _wtempnam(const wchar_t *_Directory,const wchar_t *_FilePrefix);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _snwscanf(const wchar_t * __restrict__ _Src,size_t _MaxCount,const wchar_t * __restrict__ _Format,...);
  __attribute__ ((__dllimport__)) FILE *__attribute__((__cdecl__)) _wfdopen(int _FileHandle ,const wchar_t *_Mode);
  __attribute__ ((__dllimport__)) FILE *__attribute__((__cdecl__)) _wfopen(const wchar_t * __restrict__ _Filename,const wchar_t *__restrict__ _Mode) ;
  __attribute__ ((__dllimport__)) FILE *__attribute__((__cdecl__)) _wfreopen(const wchar_t * __restrict__ _Filename,const wchar_t * __restrict__ _Mode,FILE * __restrict__ _OldFile) ;



  __attribute__ ((__dllimport__)) void __attribute__((__cdecl__)) _wperror(const wchar_t *_ErrMsg);

  __attribute__ ((__dllimport__)) FILE *__attribute__((__cdecl__)) _wpopen(const wchar_t *_Command,const wchar_t *_Mode);




  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wremove(const wchar_t *_Filename);
  __attribute__ ((__dllimport__)) wchar_t *__attribute__((__cdecl__)) _wtmpnam(wchar_t *_Buffer);
  __attribute__ ((__dllimport__)) wint_t __attribute__((__cdecl__)) _fgetwc_nolock(FILE *_File);
  __attribute__ ((__dllimport__)) wint_t __attribute__((__cdecl__)) _fputwc_nolock(wchar_t _Ch,FILE *_File);
  __attribute__ ((__dllimport__)) wint_t __attribute__((__cdecl__)) _ungetwc_nolock(wint_t _Ch,FILE *_File);
# 1293 "/usr/share/mingw-w64/include/stdio.h" 3
  __attribute__ ((__dllimport__)) void __attribute__((__cdecl__)) _lock_file(FILE *_File);
  __attribute__ ((__dllimport__)) void __attribute__((__cdecl__)) _unlock_file(FILE *_File);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _fclose_nolock(FILE *_File);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _fflush_nolock(FILE *_File);
  __attribute__ ((__dllimport__)) size_t __attribute__((__cdecl__)) _fread_nolock(void * __restrict__ _DstBuf,size_t _ElementSize,size_t _Count,FILE * __restrict__ _File);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _fseek_nolock(FILE *_File,long _Offset,int _Origin);
  __attribute__ ((__dllimport__)) long __attribute__((__cdecl__)) _ftell_nolock(FILE *_File);
  __extension__ __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _fseeki64_nolock(FILE *_File,long long _Offset,int _Origin);
  __extension__ __attribute__ ((__dllimport__)) long long __attribute__((__cdecl__)) _ftelli64_nolock(FILE *_File);
  __attribute__ ((__dllimport__)) size_t __attribute__((__cdecl__)) _fwrite_nolock(const void * __restrict__ _DstBuf,size_t _Size,size_t _Count,FILE * __restrict__ _File);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _ungetc_nolock(int _Ch,FILE *_File);





  char *__attribute__((__cdecl__)) tempnam(const char *_Directory,const char *_FilePrefix) ;
  int __attribute__((__cdecl__)) fcloseall(void) ;
  FILE *__attribute__((__cdecl__)) fdopen(int _FileHandle,const char *_Format) ;
  int __attribute__((__cdecl__)) fgetchar(void) ;
  int __attribute__((__cdecl__)) fileno(FILE *_File) ;
  int __attribute__((__cdecl__)) flushall(void) ;
  int __attribute__((__cdecl__)) fputchar(int _Ch) ;
  int __attribute__((__cdecl__)) getw(FILE *_File) ;
  int __attribute__((__cdecl__)) putw(int _Ch,FILE *_File) ;
  int __attribute__((__cdecl__)) rmtmp(void) ;
# 1335 "/usr/share/mingw-w64/include/stdio.h" 3
int __attribute__((__cdecl__)) __mingw_str_wide_utf8 (const wchar_t * const wptr, char **mbptr, size_t * buflen);
# 1349 "/usr/share/mingw-w64/include/stdio.h" 3
int __attribute__((__cdecl__)) __mingw_str_utf8_wide (const char *const mbptr, wchar_t ** wptr, size_t * buflen);
# 1358 "/usr/share/mingw-w64/include/stdio.h" 3
void __attribute__((__cdecl__)) __mingw_str_free(void *ptr);





  __attribute__ ((__dllimport__)) intptr_t __attribute__((__cdecl__)) _wspawnl(int _Mode,const wchar_t *_Filename,const wchar_t *_ArgList,...);
  __attribute__ ((__dllimport__)) intptr_t __attribute__((__cdecl__)) _wspawnle(int _Mode,const wchar_t *_Filename,const wchar_t *_ArgList,...);
  __attribute__ ((__dllimport__)) intptr_t __attribute__((__cdecl__)) _wspawnlp(int _Mode,const wchar_t *_Filename,const wchar_t *_ArgList,...);
  __attribute__ ((__dllimport__)) intptr_t __attribute__((__cdecl__)) _wspawnlpe(int _Mode,const wchar_t *_Filename,const wchar_t *_ArgList,...);
  __attribute__ ((__dllimport__)) intptr_t __attribute__((__cdecl__)) _wspawnv(int _Mode,const wchar_t *_Filename,const wchar_t *const *_ArgList);
  __attribute__ ((__dllimport__)) intptr_t __attribute__((__cdecl__)) _wspawnve(int _Mode,const wchar_t *_Filename,const wchar_t *const *_ArgList,const wchar_t *const *_Env);
  __attribute__ ((__dllimport__)) intptr_t __attribute__((__cdecl__)) _wspawnvp(int _Mode,const wchar_t *_Filename,const wchar_t *const *_ArgList);
  __attribute__ ((__dllimport__)) intptr_t __attribute__((__cdecl__)) _wspawnvpe(int _Mode,const wchar_t *_Filename,const wchar_t *const *_ArgList,const wchar_t *const *_Env);
# 1388 "/usr/share/mingw-w64/include/stdio.h" 3
  __attribute__ ((__dllimport__)) intptr_t __attribute__((__cdecl__)) _spawnv(int _Mode,const char *_Filename,const char *const *_ArgList);
  __attribute__ ((__dllimport__)) intptr_t __attribute__((__cdecl__)) _spawnve(int _Mode,const char *_Filename,const char *const *_ArgList,const char *const *_Env);
  __attribute__ ((__dllimport__)) intptr_t __attribute__((__cdecl__)) _spawnvp(int _Mode,const char *_Filename,const char *const *_ArgList);
  __attribute__ ((__dllimport__)) intptr_t __attribute__((__cdecl__)) _spawnvpe(int _Mode,const char *_Filename,const char *const *_ArgList,const char *const *_Env);






#pragma pack(pop)

# 1 "/usr/share/mingw-w64/include/sec_api/stdio_s.h" 1 3
# 9 "/usr/share/mingw-w64/include/sec_api/stdio_s.h" 3
# 1 "/usr/share/mingw-w64/include/stdio.h" 1 3
# 10 "/usr/share/mingw-w64/include/sec_api/stdio_s.h" 2 3
# 28 "/usr/share/mingw-w64/include/sec_api/stdio_s.h" 3
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) clearerr_s(FILE *_File);

  size_t __attribute__((__cdecl__)) fread_s(void *_DstBuf,size_t _DstSize,size_t _ElementSize,size_t _Count,FILE *_File);
# 471 "/usr/share/mingw-w64/include/sec_api/stdio_s.h" 3
  int __attribute__((__cdecl__)) fprintf_s(FILE *_File,const char *_Format,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _fscanf_s_l(FILE *_File,const char *_Format,_locale_t _Locale,...);
  int __attribute__((__cdecl__)) printf_s(const char *_Format,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _scanf_l(const char *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _scanf_s_l(const char *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _snprintf_c(char *_DstBuf,size_t _MaxCount,const char *_Format,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vsnprintf_c(char *_DstBuf,size_t _MaxCount,const char *_Format,va_list _ArgList);

  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _fscanf_l(FILE *_File,const char *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _sscanf_l(const char *_Src,const char *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _sscanf_s_l(const char *_Src,const char *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) sscanf_s(const char *_Src,const char *_Format,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _snscanf_s(const char *_Src,size_t _MaxCount,const char *_Format,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _snscanf_l(const char *_Src,size_t _MaxCount,const char *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _snscanf_s_l(const char *_Src,size_t _MaxCount,const char *_Format,_locale_t _Locale,...);
  int __attribute__((__cdecl__)) vfprintf_s(FILE *_File,const char *_Format,va_list _ArgList);
  int __attribute__((__cdecl__)) vprintf_s(const char *_Format,va_list _ArgList);

  int __attribute__((__cdecl__)) vsnprintf_s(char *_DstBuf,size_t _DstSize,size_t _MaxCount,const char *_Format,va_list _ArgList);

  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vsnprintf_s(char *_DstBuf,size_t _DstSize,size_t _MaxCount,const char *_Format,va_list _ArgList);

  __attribute__((dllimport)) int __attribute__((__cdecl__)) vsprintf_s(char *_DstBuf,size_t _Size,const char *_Format,va_list _ArgList);

  __attribute__((dllimport)) int __attribute__((__cdecl__)) sprintf_s(char *_DstBuf,size_t _DstSize,const char *_Format,...);

  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _snprintf_s(char *_DstBuf,size_t _DstSize,size_t _MaxCount,const char *_Format,...);

  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _fprintf_p(FILE *_File,const char *_Format,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _printf_p(const char *_Format,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _sprintf_p(char *_Dst,size_t _MaxCount,const char *_Format,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vfprintf_p(FILE *_File,const char *_Format,va_list _ArgList);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vprintf_p(const char *_Format,va_list _ArgList);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vsprintf_p(char *_Dst,size_t _MaxCount,const char *_Format,va_list _ArgList);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _scprintf_p(const char *_Format,...);
  __attribute__((dllimport)) int __attribute__((__cdecl__)) _vscprintf_p(const char *_Format,va_list _ArgList);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _printf_l(const char *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _printf_p_l(const char *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vprintf_l(const char *_Format,_locale_t _Locale,va_list _ArgList);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vprintf_p_l(const char *_Format,_locale_t _Locale,va_list _ArgList);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _fprintf_l(FILE *_File,const char *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _fprintf_p_l(FILE *_File,const char *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vfprintf_l(FILE *_File,const char *_Format,_locale_t _Locale,va_list _ArgList);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vfprintf_p_l(FILE *_File,const char *_Format,_locale_t _Locale,va_list _ArgList);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _sprintf_l(char *_DstBuf,const char *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _sprintf_p_l(char *_DstBuf,size_t _MaxCount,const char *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vsprintf_l(char *_DstBuf,const char *_Format,_locale_t,va_list _ArgList);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vsprintf_p_l(char *_DstBuf,size_t _MaxCount,const char *_Format,_locale_t _Locale,va_list _ArgList);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _scprintf_l(const char *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _scprintf_p_l(const char *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vscprintf_l(const char *_Format,_locale_t _Locale,va_list _ArgList);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vscprintf_p_l(const char *_Format,_locale_t _Locale,va_list _ArgList);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _printf_s_l(const char *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vprintf_s_l(const char *_Format,_locale_t _Locale,va_list _ArgList);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _fprintf_s_l(FILE *_File,const char *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vfprintf_s_l(FILE *_File,const char *_Format,_locale_t _Locale,va_list _ArgList);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _sprintf_s_l(char *_DstBuf,size_t _DstSize,const char *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vsprintf_s_l(char *_DstBuf,size_t _DstSize,const char *_Format,_locale_t _Locale,va_list _ArgList);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _snprintf_s_l(char *_DstBuf,size_t _DstSize,size_t _MaxCount,const char *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vsnprintf_s_l(char *_DstBuf,size_t _DstSize,size_t _MaxCount,const char *_Format,_locale_t _Locale,va_list _ArgList);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _snprintf_l(char *_DstBuf,size_t _MaxCount,const char *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _snprintf_c_l(char *_DstBuf,size_t _MaxCount,const char *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vsnprintf_l(char *_DstBuf,size_t _MaxCount,const char *_Format,_locale_t _Locale,va_list _ArgList);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vsnprintf_c_l(char *_DstBuf,size_t _MaxCount,const char *,_locale_t _Locale,va_list _ArgList);


 
 
 
 
 

  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) fopen_s(FILE **_File,const char *_Filename,const char *_Mode);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) freopen_s(FILE** _File, const char *_Filename, const char *_Mode, FILE *_Stream);

  __attribute__ ((__dllimport__)) char* __attribute__((__cdecl__)) gets_s(char*,rsize_t);
 

  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) tmpnam_s(char*,rsize_t);
 




  __attribute__ ((__dllimport__)) wchar_t *__attribute__((__cdecl__)) _getws_s(wchar_t *_Str,size_t _SizeInWords);
 
# 739 "/usr/share/mingw-w64/include/sec_api/stdio_s.h" 3
  int __attribute__((__cdecl__)) fwprintf_s(FILE *_File,const wchar_t *_Format,...);
  int __attribute__((__cdecl__)) wprintf_s(const wchar_t *_Format,...);
  int __attribute__((__cdecl__)) vfwprintf_s(FILE *_File,const wchar_t *_Format,va_list _ArgList);
  int __attribute__((__cdecl__)) vwprintf_s(const wchar_t *_Format,va_list _ArgList);

  int __attribute__((__cdecl__)) vswprintf_s(wchar_t *_Dst,size_t _SizeInWords,const wchar_t *_Format,va_list _ArgList);

  int __attribute__((__cdecl__)) swprintf_s(wchar_t *_Dst,size_t _SizeInWords,const wchar_t *_Format,...);

  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vsnwprintf_s(wchar_t *_DstBuf,size_t _DstSizeInWords,size_t _MaxCount,const wchar_t *_Format,va_list _ArgList);

  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _snwprintf_s(wchar_t *_DstBuf,size_t _DstSizeInWords,size_t _MaxCount,const wchar_t *_Format,...);


  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wprintf_s_l(const wchar_t *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vwprintf_s_l(const wchar_t *_Format,_locale_t _Locale,va_list _ArgList);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _fwprintf_s_l(FILE *_File,const wchar_t *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vfwprintf_s_l(FILE *_File,const wchar_t *_Format,_locale_t _Locale,va_list _ArgList);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _swprintf_s_l(wchar_t *_DstBuf,size_t _DstSize,const wchar_t *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vswprintf_s_l(wchar_t *_DstBuf,size_t _DstSize,const wchar_t *_Format,_locale_t _Locale,va_list _ArgList);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _snwprintf_s_l(wchar_t *_DstBuf,size_t _DstSize,size_t _MaxCount,const wchar_t *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vsnwprintf_s_l(wchar_t *_DstBuf,size_t _DstSize,size_t _MaxCount,const wchar_t *_Format,_locale_t _Locale,va_list _ArgList);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _fwscanf_s_l(FILE *_File,const wchar_t *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _swscanf_s_l(const wchar_t *_Src,const wchar_t *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) swscanf_s(const wchar_t *_Src,const wchar_t *_Format,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _snwscanf_s(const wchar_t *_Src,size_t _MaxCount,const wchar_t *_Format,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _snwscanf_s_l(const wchar_t *_Src,size_t _MaxCount,const wchar_t *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wscanf_s_l(const wchar_t *_Format,_locale_t _Locale,...);


 
 
 
 

  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _wfopen_s(FILE **_File,const wchar_t *_Filename,const wchar_t *_Mode);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _wfreopen_s(FILE **_File,const wchar_t *_Filename,const wchar_t *_Mode,FILE *_OldFile);

  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _wtmpnam_s(wchar_t *_DstBuf,size_t _SizeInWords);
 


  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _fwprintf_p(FILE *_File,const wchar_t *_Format,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wprintf_p(const wchar_t *_Format,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vfwprintf_p(FILE *_File,const wchar_t *_Format,va_list _ArgList);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vwprintf_p(const wchar_t *_Format,va_list _ArgList);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _swprintf_p(wchar_t *_DstBuf,size_t _MaxCount,const wchar_t *_Format,...);
  __attribute__((dllimport)) int __attribute__((__cdecl__)) _vswprintf_p(wchar_t *_DstBuf,size_t _MaxCount,const wchar_t *_Format,va_list _ArgList);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _scwprintf_p(const wchar_t *_Format,...);
  __attribute__((dllimport)) int __attribute__((__cdecl__)) _vscwprintf_p(const wchar_t *_Format,va_list _ArgList);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wprintf_l(const wchar_t *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wprintf_p_l(const wchar_t *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vwprintf_l(const wchar_t *_Format,_locale_t _Locale,va_list _ArgList);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vwprintf_p_l(const wchar_t *_Format,_locale_t _Locale,va_list _ArgList);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _fwprintf_l(FILE *_File,const wchar_t *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _fwprintf_p_l(FILE *_File,const wchar_t *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vfwprintf_l(FILE *_File,const wchar_t *_Format,_locale_t _Locale,va_list _ArgList);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vfwprintf_p_l(FILE *_File,const wchar_t *_Format,_locale_t _Locale,va_list _ArgList);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _swprintf_c_l(wchar_t *_DstBuf,size_t _MaxCount,const wchar_t *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _swprintf_p_l(wchar_t *_DstBuf,size_t _MaxCount,const wchar_t *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vswprintf_c_l(wchar_t *_DstBuf,size_t _MaxCount,const wchar_t *_Format,_locale_t _Locale,va_list _ArgList);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vswprintf_p_l(wchar_t *_DstBuf,size_t _MaxCount,const wchar_t *_Format,_locale_t _Locale,va_list _ArgList);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _scwprintf_l(const wchar_t *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _scwprintf_p_l(const wchar_t *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vscwprintf_p_l(const wchar_t *_Format,_locale_t _Locale,va_list _ArgList);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _snwprintf_l(wchar_t *_DstBuf,size_t _MaxCount,const wchar_t *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vsnwprintf_l(wchar_t *_DstBuf,size_t _MaxCount,const wchar_t *_Format,_locale_t _Locale,va_list _ArgList);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) __swprintf_l(wchar_t *_Dest,const wchar_t *_Format,_locale_t _Plocinfo,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) __vswprintf_l(wchar_t *_Dest,const wchar_t *_Format,_locale_t _Plocinfo,va_list _Args);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vscwprintf_l(const wchar_t *_Format,_locale_t _Locale,va_list _ArgList);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _fwscanf_l(FILE *_File,const wchar_t *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _swscanf_l(const wchar_t *_Src,const wchar_t *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _snwscanf_l(const wchar_t *_Src,size_t _MaxCount,const wchar_t *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wscanf_l(const wchar_t *_Format,_locale_t _Locale,...);





  __attribute__ ((__dllimport__)) size_t __attribute__((__cdecl__)) _fread_nolock_s(void *_DstBuf,size_t _DstSize,size_t _ElementSize,size_t _Count,FILE *_File);
# 1401 "/usr/share/mingw-w64/include/stdio.h" 2 3

# 1 "/usr/share/mingw-w64/include/_mingw_print_pop.h" 1 3
# 1403 "/usr/share/mingw-w64/include/stdio.h" 2 3
# 28 "memory/Allocator.c" 2



# 30 "memory/Allocator.c"
struct Unroller;
struct Unroller
{
    const char* const content;
    const struct Unroller* const last;
};
static void writeUnroller(const struct Unroller* unroller)
{
    if (unroller) {
        writeUnroller(unroller->last);
        fprintf(
# 40 "memory/Allocator.c" 3
               (__acrt_iob_func(2))
# 40 "memory/Allocator.c"
                     , "%s", unroller->content);
    }
}
static void unroll(struct Allocator_pvt* context,
                   int includeAllocations,
                   struct Unroller* unroller)
{
    writeUnroller(unroller);
    const char* ident = (context->pub.fileName) ? context->pub.fileName : "UNKNOWN";

    fprintf(
# 50 "memory/Allocator.c" 3
           (__acrt_iob_func(2))
# 50 "memory/Allocator.c"
                 , "%s:%d [%lu] bytes%s\n",
            ident,
            context->pub.lineNum,
            context->allocatedHere,
            (context->pub.isFreeing) ? " (freeing)" : "");

    struct Unroller childUnroller = {
        .content = ((context->nextSibling) ? "| " : "  "),
        .last = unroller
    };
    if (context->firstChild) {
        unroll(context->firstChild, includeAllocations, &childUnroller);
    }
    struct Allocator_Allocation_pvt* allocation = context->allocations;
    while (allocation && includeAllocations) {
        writeUnroller(&childUnroller);
        fprintf(
# 66 "memory/Allocator.c" 3
               (__acrt_iob_func(2))
# 66 "memory/Allocator.c"
                     , "%s:%ld [%lu] bytes at [0x%lx]\n",
                allocation->fileName,
                allocation->lineNum,
                allocation->pub.size,
                (long)(uintptr_t)allocation);
        allocation = allocation->next;
    }
    if (context->nextSibling) {
        unroll(context->nextSibling, includeAllocations, unroller);
    }
}

static inline uint64_t bytesAllocated(struct Allocator_pvt* ctx)
{
    uint64_t bytes = ctx->allocatedHere;
    for (struct Allocator_pvt* child = ctx->firstChild; child; child = child->nextSibling) {
        bytes += bytesAllocated(child);
    }
    return bytes;
}

static void check(struct Allocator_pvt* alloc)
{
    if (!0) { return; }
    uint64_t totalAllocated = alloc->rootAlloc->maxSpace - alloc->rootAlloc->spaceAvailable;
    uint64_t accounted = bytesAllocated((__extension__ ({ __typeof__((struct Allocator_pvt*)alloc->rootAlloc) Identity_ptr = ((struct Allocator_pvt*)alloc->rootAlloc); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x856c1578c6e6bdeeull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("Allocator.c"), (91), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x856c1578c6e6bdeeull))"); } } while (0); Identity_ptr; })));
    do { if (!((totalAllocated == accounted))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("Allocator.c"), (92), "(totalAllocated == accounted)"); } } while (0);
}

void Allocator_snapshot(struct Allocator* allocator, int includeAllocations)
{

    struct Allocator_pvt* alloc = (__extension__ ({ __typeof__((struct Allocator_pvt*)allocator) Identity_ptr = ((struct Allocator_pvt*)allocator); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x856c1578c6e6bdeeull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("Allocator.c"), (98), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x856c1578c6e6bdeeull))"); } } while (0); Identity_ptr; }));
    struct Allocator_FirstCtx* rootAlloc = (__extension__ ({ __typeof__(alloc->rootAlloc) Identity_ptr = (alloc->rootAlloc); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x856c1578c6e6bdeeull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("Allocator.c"), (99), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x856c1578c6e6bdeeull))"); } } while (0); Identity_ptr; }));
    alloc = (__extension__ ({ __typeof__((struct Allocator_pvt*)rootAlloc) Identity_ptr = ((struct Allocator_pvt*)rootAlloc); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x856c1578c6e6bdeeull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("Allocator.c"), (100), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x856c1578c6e6bdeeull))"); } } while (0); Identity_ptr; }));

    fprintf(
# 102 "memory/Allocator.c" 3
           (__acrt_iob_func(2))
# 102 "memory/Allocator.c"
                 , "----- %scjdns memory snapshot -----\n", "");

    uint64_t totalAllocated = rootAlloc->maxSpace - rootAlloc->spaceAvailable;
    uint64_t realAllocated = bytesAllocated(alloc);

    unroll(alloc, includeAllocations, 
# 107 "memory/Allocator.c" 3 4
                                     ((void *)0)
# 107 "memory/Allocator.c"
                                         );

    if (totalAllocated != realAllocated) {
        fprintf(
# 110 "memory/Allocator.c" 3
               (__acrt_iob_func(2))
# 110 "memory/Allocator.c"
                     , "!!!!!! INTERNAL ERROR totalAllocated = [%lu] realAllocated = [%lu] !!!!!",
                (unsigned long)totalAllocated, (unsigned long)realAllocated);
    }
    fprintf(
# 113 "memory/Allocator.c" 3
           (__acrt_iob_func(2))
# 113 "memory/Allocator.c"
                 , "totalBytes [%ld] remaining [%ld]\n",
                    (long)rootAlloc->maxSpace,
                    (long)rootAlloc->spaceAvailable);

    fprintf(
# 117 "memory/Allocator.c" 3
           (__acrt_iob_func(2))
# 117 "memory/Allocator.c"
                 , "----- %scjdns memory snapshot -----\n", "end ");
}

__attribute__((__noreturn__))
static void failure(struct Allocator_pvt* context,
                    const char* message,
                    const char* fileName,
                    int lineNum)
{
    Allocator_snapshot(&context->pub, 1);
    Assert_failure("%s:%d Fatal error: [%s]", fileName, lineNum, message);
}

static inline unsigned long getRealSize(unsigned long requestedSize)
{
    return ((requestedSize + (sizeof(char*) - 1)) & ~(sizeof(char*) - 1))
        + sizeof(struct Allocator_Allocation_pvt)

        + sizeof(unsigned long)

    ;
}



static inline void setCanaries(struct Allocator_Allocation_pvt* alloc,
                               struct Allocator_pvt* context)
{

        ((unsigned long*) alloc)[ (alloc->pub.size / sizeof(unsigned long)) - 1 ] = context->canary ^ (uintptr_t)alloc->fileName;

}

static inline void checkCanaries(struct Allocator_Allocation_pvt* alloc,
                                 struct Allocator_pvt* context)
{

        if (((unsigned long*) alloc)[ (alloc->pub.size / sizeof(unsigned long)) - 1 ] == ((uintptr_t)alloc->fileName ^ context->canary)) { return; }
        Assert_failure("%s:%d Fatal error: invalid canary\n",
                       context->pub.fileName, context->pub.lineNum);

}

__attribute__ ((alloc_size(2)))
static inline void* newAllocation(struct Allocator_pvt* context,
                                  unsigned long size,
                                  const char* fileName,
                                  int lineNum)
{
    check(context);
    int64_t realSize = getRealSize(size);
    struct Allocator_FirstCtx* rootAlloc = (__extension__ ({ __typeof__(context->rootAlloc) Identity_ptr = (context->rootAlloc); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x856c1578c6e6bdeeull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("Allocator.c"), (168), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x856c1578c6e6bdeeull))"); } } while (0); Identity_ptr; }));
    if (rootAlloc->spaceAvailable <= realSize) {
        failure(context, "Out of memory, limit exceeded", fileName, lineNum);
    }

    rootAlloc->spaceAvailable -= realSize;
    context->allocatedHere += realSize;

    struct Allocator_Allocation_pvt* alloc =
        rootAlloc->provider(rootAlloc->providerContext,
                            
# 178 "memory/Allocator.c" 3 4
                           ((void *)0)
# 178 "memory/Allocator.c"
                               ,
                            realSize,
                            &context->pub);
    if (alloc == 
# 181 "memory/Allocator.c" 3 4
                ((void *)0)
# 181 "memory/Allocator.c"
                    ) {
        failure(context, "Out of memory, malloc() returned NULL", fileName, lineNum);
    }
    alloc->next = context->allocations;
    alloc->pub.size = realSize;
    alloc->fileName = fileName;
    alloc->lineNum = lineNum;
    context->allocations = alloc;
    setCanaries(alloc, context);

    return (void*) (alloc + 1);
}

struct Allocator_Allocation* Allocator_getAllocation(struct Allocator* alloc, int allocNum)
{
    struct Allocator_pvt* ctx = (__extension__ ({ __typeof__((struct Allocator_pvt*)alloc) Identity_ptr = ((struct Allocator_pvt*)alloc); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x856c1578c6e6bdeeull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("Allocator.c"), (196), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x856c1578c6e6bdeeull))"); } } while (0); Identity_ptr; }));
    if (allocNum < 0) {
        return 
# 198 "memory/Allocator.c" 3 4
              ((void *)0)
# 198 "memory/Allocator.c"
                  ;
    }
    struct Allocator_Allocation_pvt* allocation = ctx->allocations;
    for (;allocation && allocNum > 0; allocNum--) {
        allocation = allocation->next;
    }
    return (allocation) ? &allocation->pub : 
# 204 "memory/Allocator.c" 3 4
                                            ((void *)0)
# 204 "memory/Allocator.c"
                                                ;
}

struct Allocator* Allocator_getChild(struct Allocator* alloc, int childNumber)
{
    struct Allocator_pvt* ctx = (__extension__ ({ __typeof__((struct Allocator_pvt*)alloc) Identity_ptr = ((struct Allocator_pvt*)alloc); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x856c1578c6e6bdeeull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("Allocator.c"), (209), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x856c1578c6e6bdeeull))"); } } while (0); Identity_ptr; }));
    if (childNumber < 0) {
        return 
# 211 "memory/Allocator.c" 3 4
              ((void *)0)
# 211 "memory/Allocator.c"
                  ;
    }
    struct Allocator_pvt* child = ctx->firstChild;
    for (;child && childNumber > 0; childNumber--) {
        child = child->nextSibling;
    }
    return (child) ? &child->pub : 
# 217 "memory/Allocator.c" 3 4
                                  ((void *)0)
# 217 "memory/Allocator.c"
                                      ;
}

static int removeJob(struct Allocator_OnFreeJob_pvt* job)
{
    struct Allocator_pvt* context = (__extension__ ({ __typeof__(job->alloc) Identity_ptr = (job->alloc); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x856c1578c6e6bdeeull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("Allocator.c"), (222), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x856c1578c6e6bdeeull))"); } } while (0); Identity_ptr; }));
    struct Allocator_OnFreeJob_pvt* j = context->onFree;
    struct Allocator_OnFreeJob_pvt** jP = &context->onFree;
    while (j && j != job) {
        jP = &j->next;
        j = j->next;
    }
    if (j == job) {
        *jP = j->next;
        return 0;
    } else {
        return -1;
        failure(context, "Allocator_onFreeComplete() called multiple times", job->file, job->line);
    }
}

static void releaseAllocation(struct Allocator_pvt* context,
                              struct Allocator_Allocation_pvt* allocation,
                              Allocator_Provider provider,
                              void* providerCtx)
{
    checkCanaries(allocation, context);


    __builtin_memset(&(&allocation->pub)[1],0xee,allocation->pub.size - sizeof(struct Allocator_Allocation))

                                                                           ;

    provider(providerCtx,
             &allocation->pub,
             0,
             ((char*)context != (char*)allocation) ? &context->pub : 
# 253 "memory/Allocator.c" 3 4
                                                                    ((void *)0)
# 253 "memory/Allocator.c"
                                                                        );
}

static void releaseMemory(struct Allocator_pvt* context,
                          Allocator_Provider provider,
                          void* providerCtx)
{


        unsigned long allocatedHere = context->allocatedHere;


    (__extension__ ({ __typeof__(context->rootAlloc) Identity_ptr = (context->rootAlloc); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x856c1578c6e6bdeeull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("Allocator.c"), (265), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x856c1578c6e6bdeeull))"); } } while (0); Identity_ptr; }))->spaceAvailable += context->allocatedHere;

    struct Allocator_Allocation_pvt* loc = context->allocations;
    while (loc != 
# 268 "memory/Allocator.c" 3 4
                 ((void *)0)
# 268 "memory/Allocator.c"
                     ) {

            allocatedHere -= loc->pub.size;


        struct Allocator_Allocation_pvt* nextLoc = loc->next;
        releaseAllocation(context, loc, provider, providerCtx);
        loc = nextLoc;
    }

        do { if (!((allocatedHere == 0))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("Allocator.c"), (278), "(allocatedHere == 0)"); } } while (0);

}


static void disconnect(struct Allocator_pvt* context)
{

    do { if (!((context->parent))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("Allocator.c"), (286), "(context->parent)"); } } while (0);

    if (context->lastSibling) {
        do { if (!((context->lastSibling->nextSibling == context))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("Allocator.c"), (289), "(context->lastSibling->nextSibling == context)"); } } while (0);
        do { if (!((context->parent->firstChild != context))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("Allocator.c"), (290), "(context->parent->firstChild != context)"); } } while (0);
        context->lastSibling->nextSibling = context->nextSibling;
    } else {

        do { if (!((context->parent->firstChild == context || context->parent == context))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("Allocator.c"), (294), "(context->parent->firstChild == context || context->parent == context)"); } } while (0);
        do { if (!((context->parent != context || !context->nextSibling))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("Allocator.c"), (295), "(context->parent != context || !context->nextSibling)"); } } while (0);
        context->parent->firstChild = context->nextSibling;
    }
    if (context->nextSibling) {
        do { if (!((context->nextSibling->lastSibling == context))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("Allocator.c"), (299), "(context->nextSibling->lastSibling == context)"); } } while (0);
        context->nextSibling->lastSibling = context->lastSibling;
    }
    context->lastSibling = 
# 302 "memory/Allocator.c" 3 4
                          ((void *)0)
# 302 "memory/Allocator.c"
                              ;
    context->nextSibling = 
# 303 "memory/Allocator.c" 3 4
                          ((void *)0)
# 303 "memory/Allocator.c"
                              ;
    context->parent = 
# 304 "memory/Allocator.c" 3 4
                     ((void *)0)
# 304 "memory/Allocator.c"
                         ;
}


static void connect(struct Allocator_pvt* parent,
                    struct Allocator_pvt* child,
                    const char* file,
                    int line)
{
    do { if (!((child->parent == 
# 313 "memory/Allocator.c" 3 4
   ((void *)0)
# 313 "memory/Allocator.c"
   ))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("Allocator.c"), (313), "(child->parent == ((void *)0))"); } } while (0);
    do { if (!((child->lastSibling == 
# 314 "memory/Allocator.c" 3 4
   ((void *)0)
# 314 "memory/Allocator.c"
   ))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("Allocator.c"), (314), "(child->lastSibling == ((void *)0))"); } } while (0);
    do { if (!((child->nextSibling == 
# 315 "memory/Allocator.c" 3 4
   ((void *)0)
# 315 "memory/Allocator.c"
   ))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("Allocator.c"), (315), "(child->nextSibling == ((void *)0))"); } } while (0);
    do { if (!((parent != child))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("Allocator.c"), (316), "(parent != child)"); } } while (0);
    if (1) {
        for (struct Allocator_pvt* c = parent->firstChild; c; c = c->nextSibling) {
            do { if (!((child != c))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("Allocator.c"), (319), "(child != c)"); } } while (0);
        }
    }
    child->nextSibling = parent->firstChild;
    if (parent->firstChild) {
        parent->firstChild->lastSibling = child;
    }
    parent->firstChild = child;
    child->parent = parent;
}

static int disconnectAllocator(struct Allocator_pvt* target, struct Allocator_List** cpp)
{
    int found = 0;
    struct Allocator_List* cp;
    while ((cp = *cpp)) {
        if (cp->alloc == target) {
            *cpp = cp->next;
            found = 1;
            break;
        }
        cpp = &cp->next;
    }
    return found;
}

static void disconnectAdopted(struct Allocator_pvt* parent, struct Allocator_pvt* child)
{
    do { if (!((parent->adoptions))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("Allocator.c"), (347), "(parent->adoptions)"); } } while (0);
    do { if (!((parent->adoptions->children))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("Allocator.c"), (348), "(parent->adoptions->children)"); } } while (0);
    do { if (!((child->adoptions))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("Allocator.c"), (349), "(child->adoptions)"); } } while (0);
    do { if (!((child->adoptions->parents))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("Allocator.c"), (350), "(child->adoptions->parents)"); } } while (0);
    do { if (!((disconnectAllocator(child, &parent->adoptions->children)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("Allocator.c"), (351), "(disconnectAllocator(child, &parent->adoptions->children))"); } } while (0);
    do { if (!((disconnectAllocator(parent, &child->adoptions->parents)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("Allocator.c"), (352), "(disconnectAllocator(parent, &child->adoptions->parents))"); } } while (0);
}


static int pivotChildrenToAdoptedParents0(struct Allocator_pvt* context,
                                          int depth,
                                          int maxDepth,
                                          const char* file,
                                          int line)
{
    int out = 0;
    if (depth == maxDepth) {
        if (context->pub.isFreeing) { return 0; }
        if (context->adoptions) {

            if (context->adoptions->parents) {
                do { if (!((!context->adoptions->parents->alloc->pub.isFreeing))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("Allocator.c"), (368), "(!context->adoptions->parents->alloc->pub.isFreeing)"); } } while (0);
                disconnect(context);
                connect(context->adoptions->parents->alloc, context, file, line);
                disconnectAdopted(context->adoptions->parents->alloc, context);
                return 0;
            }

            for (struct Allocator_List* c = context->adoptions->children; c; c = c->next) {
                do { if (!((!c->alloc->pub.isFreeing))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("Allocator.c"), (376), "(!c->alloc->pub.isFreeing)"); } } while (0);
                disconnectAdopted(context, c->alloc);
            }
        }
        do { if (!((!context->pub.isFreeing))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("Allocator.c"), (380), "(!context->pub.isFreeing)"); } } while (0);
        context->pub.isFreeing = 1;
        out++;
    } else {
        struct Allocator_pvt* child = context->firstChild;
        while (child) {
            do { if (!((child != context))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("Allocator.c"), (386), "(child != context)"); } } while (0);
            struct Allocator_pvt* nextChild = child->nextSibling;
            out += pivotChildrenToAdoptedParents0(child, depth+1, maxDepth, file, line);
            child = nextChild;
        }
    }
    return out;
}

static int pivotChildrenToAdoptedParents(struct Allocator_pvt* context, const char* file, int line)
{
    for (int i = 0; i < 10000; i++) {
        if (!pivotChildrenToAdoptedParents0(context, 0, i, file, line)) {

            return (i != 0);
        }
    }
    Assert_failure("Didn't free all allocators in 10000 deep iterations");
}






static void marshalOnFreeJobs(struct Allocator_pvt* context, struct Allocator_pvt* rootToFree)
{
    do { if (!((context->pub.isFreeing))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("Allocator.c"), (413), "(context->pub.isFreeing)"); } } while (0);
    struct Allocator_pvt* child = context->firstChild;
    while (child) {

        struct Allocator_OnFreeJob_pvt** jobP = &rootToFree->onFree;
        while (*jobP != 
# 418 "memory/Allocator.c" 3 4
                       ((void *)0)
# 418 "memory/Allocator.c"
                           ) {
            struct Allocator_OnFreeJob_pvt* job = *jobP;
            jobP = &job->next;
        }
        *jobP = child->onFree;
        child->onFree = 
# 423 "memory/Allocator.c" 3 4
                       ((void *)0)
# 423 "memory/Allocator.c"
                           ;

        while (*jobP != 
# 425 "memory/Allocator.c" 3 4
                       ((void *)0)
# 425 "memory/Allocator.c"
                           ) {
            struct Allocator_OnFreeJob_pvt* job = *jobP;
            job->alloc = rootToFree;
            jobP = &job->next;
        }

        struct Allocator_pvt* nextChild = child->nextSibling;
        marshalOnFreeJobs(child, rootToFree);
        child = nextChild;
    }
}

static void doOnFreeJobs(struct Allocator_pvt* context)
{

    struct Allocator_OnFreeJob_pvt** jobP = &context->onFree;
    while (*jobP != 
# 441 "memory/Allocator.c" 3 4
                   ((void *)0)
# 441 "memory/Allocator.c"
                       ) {
        struct Allocator_OnFreeJob_pvt* job = *jobP;
        if (!job->pub.callback) {

            do { if (!((!removeJob(job)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("Allocator.c"), (445), "(!removeJob(job))"); } } while (0);
            continue;
        } else if (!job->called) {
            if (job->pub.callback(&job->pub) != 10000) {
                do { if (!((!removeJob(job)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("Allocator.c"), (449), "(!removeJob(job))"); } } while (0);
                continue;
            } else {
                job->called = 1;
            }
        }
        jobP = &job->next;
    }
}

static void freeAllocator(struct Allocator_pvt* context)
{
    do { if (!((context->pub.isFreeing))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("Allocator.c"), (461), "(context->pub.isFreeing)"); } } while (0);
    int isTop = !context->parent->pub.isFreeing;
    if (isTop) {
        check(context);
        disconnect(context);
    }
    struct Allocator_pvt* child = context->firstChild;
    while (child) {
        struct Allocator_pvt* nextChild = child->nextSibling;
        freeAllocator(child);
        child = nextChild;
    }


    struct Allocator_FirstCtx* rootAlloc = (__extension__ ({ __typeof__(context->rootAlloc) Identity_ptr = (context->rootAlloc); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x856c1578c6e6bdeeull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("Allocator.c"), (475), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x856c1578c6e6bdeeull))"); } } while (0); Identity_ptr; }));
    Allocator_Provider provider = rootAlloc->provider;
    void* providerCtx = rootAlloc->providerContext;

    releaseMemory(context, provider, providerCtx);
    if (isTop) {
        check((struct Allocator_pvt*)rootAlloc);
    }
}

void Allocator_onFreeComplete(struct Allocator_OnFreeJob* onFreeJob)
{
    struct Allocator_OnFreeJob_pvt* job = (struct Allocator_OnFreeJob_pvt*) onFreeJob;
    struct Allocator_pvt* context = (__extension__ ({ __typeof__(job->alloc) Identity_ptr = (job->alloc); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x856c1578c6e6bdeeull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("Allocator.c"), (488), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x856c1578c6e6bdeeull))"); } } while (0); Identity_ptr; }));

    if (removeJob(job)) {
        failure(context, "OnFreeJob->complete() called multiple times", job->file, job->line);
    }

    if (!context->onFree) {

        freeAllocator(context);
    }
}

void Allocator__free(struct Allocator* alloc, const char* file, int line)
{
    struct Allocator_pvt* context = (__extension__ ({ __typeof__((struct Allocator_pvt*) alloc) Identity_ptr = ((struct Allocator_pvt*) alloc); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x856c1578c6e6bdeeull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("Allocator.c"), (502), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x856c1578c6e6bdeeull))"); } } while (0); Identity_ptr; }));
    check(context);




    if (context->pub.isFreeing) { return; }

    if (context->rootAlloc == (struct Allocator_FirstCtx*)context) {
        struct Allocator_FirstCtx* rootAlloc = (__extension__ ({ __typeof__((struct Allocator_FirstCtx*)context) Identity_ptr = ((struct Allocator_FirstCtx*)context); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x856c1578c6e6bdeeull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("Allocator.c"), (511), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x856c1578c6e6bdeeull))"); } } while (0); Identity_ptr; }));
        if (bytesAllocated(context) + rootAlloc->spaceAvailable != (uint64_t)rootAlloc->maxSpace) {
            failure(context, "unaccounted for memory", file, line);
        }
    }

    check(context);
    if (!pivotChildrenToAdoptedParents(context, file, line)) { return; }
    check(context);
    marshalOnFreeJobs(context, context);
    check(context);
    doOnFreeJobs(context);
    check(context);
    if (!context->onFree) {
        freeAllocator(context);
    }
}

void* Allocator__malloc(struct Allocator* allocator,
                        unsigned long length,
                        const char* fileName,
                        int lineNum)
{
    struct Allocator_pvt* ctx = (__extension__ ({ __typeof__((struct Allocator_pvt*) allocator) Identity_ptr = ((struct Allocator_pvt*) allocator); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x856c1578c6e6bdeeull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("Allocator.c"), (534), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x856c1578c6e6bdeeull))"); } } while (0); Identity_ptr; }));
    void* out = newAllocation(ctx, length, fileName, lineNum);
    check(ctx);
    return out;
}

void* Allocator__calloc(struct Allocator* alloc,
                        unsigned long length,
                        unsigned long count,
                        const char* fileName,
                        int lineNum)
{
    void* pointer = Allocator__malloc(alloc, length * count, fileName, lineNum);
    __builtin_memset(pointer,0,length * count);
    return pointer;
}

void* Allocator__realloc(struct Allocator* allocator,
                         const void* original,
                         unsigned long size,
                         const char* fileName,
                         int lineNum)
{
    if (original == 
# 557 "memory/Allocator.c" 3 4
                   ((void *)0)
# 557 "memory/Allocator.c"
                       ) {
        return Allocator__malloc(allocator, size, fileName, lineNum);
    }

    struct Allocator_pvt* context = (__extension__ ({ __typeof__((struct Allocator_pvt*) allocator) Identity_ptr = ((struct Allocator_pvt*) allocator); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x856c1578c6e6bdeeull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("Allocator.c"), (561), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x856c1578c6e6bdeeull))"); } } while (0); Identity_ptr; }));
    check(context);
    struct Allocator_Allocation_pvt** locPtr = &context->allocations;
    struct Allocator_Allocation_pvt* origLoc =
        ((struct Allocator_Allocation_pvt*) original) - 1;
    for (;;) {
        struct Allocator_Allocation_pvt* loc = *locPtr;
        if (loc == 
# 568 "memory/Allocator.c" 3 4
                  ((void *)0)
# 568 "memory/Allocator.c"
                      ) {
            failure(context,
                    "Reallocation of memory which was not allocated using this allocator.",
                    fileName,
                    lineNum);
        }
        checkCanaries(loc, context);
        if (loc == origLoc) {
            break;
        }
        locPtr = &loc->next;
    }

    struct Allocator_Allocation_pvt* nextLoc = origLoc->next;

    if (size == 0) {

        *locPtr = nextLoc;
        do { if (!((origLoc->pub.size <= context->allocatedHere))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("Allocator.c"), (586), "(origLoc->pub.size <= context->allocatedHere)"); } } while (0);
        context->rootAlloc->spaceAvailable += origLoc->pub.size;
        context->allocatedHere -= origLoc->pub.size;
        releaseAllocation(context,
                          origLoc,
                          context->rootAlloc->provider,
                          context->rootAlloc->providerContext);
        check(context);
        return 
# 594 "memory/Allocator.c" 3 4
              ((void *)0)
# 594 "memory/Allocator.c"
                  ;
    }

    size_t realSize = getRealSize(size);


    if (context->rootAlloc->spaceAvailable + origLoc->pub.size < (int64_t)realSize) {
        failure(context, "Out of memory, limit exceeded.", fileName, lineNum);
    }






    context->rootAlloc->spaceAvailable += origLoc->pub.size;
    context->rootAlloc->spaceAvailable -= realSize;
    context->allocatedHere -= origLoc->pub.size;
    context->allocatedHere += realSize;

    struct Allocator_Allocation_pvt* alloc =
        context->rootAlloc->provider(context->rootAlloc->providerContext,
                                     &origLoc->pub,
                                     realSize,
                                     allocator);

    if (alloc == 
# 620 "memory/Allocator.c" 3 4
                ((void *)0)
# 620 "memory/Allocator.c"
                    ) {
        failure(context, "Out of memory, realloc() returned NULL.", fileName, lineNum);
    }
    alloc->next = nextLoc;
    alloc->pub.size = realSize;
    *locPtr = alloc;

    setCanaries(alloc, context);
    check(context);

    return (void*) (alloc + 1);
}

void* Allocator__clone(struct Allocator* allocator,
                       const void* toClone,
                       unsigned long length,
                       const char* fileName,
                       int lineNum)
{
    void* pointer = Allocator__malloc(allocator, length, fileName, lineNum);
    Bits__memcpy(pointer,toClone,length,"Allocator.c",640);
    return pointer;
}

struct Allocator* Allocator__child(struct Allocator* allocator, const char* file, int line)
{
    struct Allocator_pvt* parent = (__extension__ ({ __typeof__((struct Allocator_pvt*) allocator) Identity_ptr = ((struct Allocator_pvt*) allocator); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x856c1578c6e6bdeeull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("Allocator.c"), (646), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x856c1578c6e6bdeeull))"); } } while (0); Identity_ptr; }));
    check(parent);

    struct Allocator_pvt stackChild = {
        .pub = {
            .fileName = file,
            .lineNum = line,
            .isFreeing = parent->pub.isFreeing
        },
        .rootAlloc = parent->rootAlloc
    };
    (&stackChild)->Identity_verifier = ((unsigned long) 0x856c1578c6e6bdeeull);

        stackChild.nextCanary = stackChild.canary = parent->nextCanary;


    struct Allocator_pvt* child =
        newAllocation(&stackChild, sizeof(struct Allocator_pvt), file, line);
    Bits__memcpy(child,&stackChild,sizeof(struct Allocator_pvt),"Allocator.c",664);


    connect(parent, child, file, line);

    check(parent);
    return &child->pub;
}

int Allocator_cancelOnFree(struct Allocator_OnFreeJob* toRemove)
{
    struct Allocator_OnFreeJob_pvt* job = (struct Allocator_OnFreeJob_pvt*) toRemove;
    struct Allocator_pvt* context = (__extension__ ({ __typeof__(job->alloc) Identity_ptr = (job->alloc); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x856c1578c6e6bdeeull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("Allocator.c"), (676), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x856c1578c6e6bdeeull))"); } } while (0); Identity_ptr; }));
    struct Allocator_OnFreeJob_pvt** jobPtr = &(context->onFree);
    while (*jobPtr != 
# 678 "memory/Allocator.c" 3 4
                     ((void *)0)
# 678 "memory/Allocator.c"
                         ) {
        if (*jobPtr == job) {
            *jobPtr = (*jobPtr)->next;
            return 0;
        }
        jobPtr = &(*jobPtr)->next;
    }
    return -1;
}


static int isAncestorOf(struct Allocator_pvt* maybeParent,
                        struct Allocator_pvt* maybeChild)
{
    if (maybeParent == maybeChild) {
        return 1;
    }
    if (maybeParent == 
# 695 "memory/Allocator.c" 3 4
                      ((void *)0) 
# 695 "memory/Allocator.c"
                           || maybeChild == 
# 695 "memory/Allocator.c" 3 4
                                            ((void *)0) 
# 695 "memory/Allocator.c"
                                                 || maybeChild->parent == maybeChild) {
        return 0;
    }
    if (isAncestorOf(maybeParent, maybeChild->parent)) {
        return 1;
    }
    if (maybeChild->adoptions) {
        struct Allocator_List* al = maybeChild->adoptions->parents;
        while (al) {
            if (isAncestorOf(maybeParent, al->alloc)) {
                return 1;
            }
        }
    }
    return 0;
}

void Allocator__disown(struct Allocator* parentAlloc,
                       struct Allocator* allocToDisown,
                       const char* fileName,
                       int lineNum)
{
    struct Allocator_pvt* parent = (__extension__ ({ __typeof__((struct Allocator_pvt*) parentAlloc) Identity_ptr = ((struct Allocator_pvt*) parentAlloc); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x856c1578c6e6bdeeull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("Allocator.c"), (717), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x856c1578c6e6bdeeull))"); } } while (0); Identity_ptr; }));
    struct Allocator_pvt* child = (__extension__ ({ __typeof__((struct Allocator_pvt*) allocToDisown) Identity_ptr = ((struct Allocator_pvt*) allocToDisown); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x856c1578c6e6bdeeull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("Allocator.c"), (718), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x856c1578c6e6bdeeull))"); } } while (0); Identity_ptr; }));

    if (parent->pub.isFreeing || child->pub.isFreeing) { return; }

    if (child->parent == parent) {



        Allocator__free(&child->pub, fileName, lineNum);
        return;
    }

    if (isAncestorOf(child, parent)) {

        return;
    }

    disconnectAdopted(parent, child);
}

void Allocator__adopt(struct Allocator* adoptedParent,
                      struct Allocator* childToAdopt,
                      const char* file,
                      int line)
{
    struct Allocator_pvt* parent = (__extension__ ({ __typeof__((struct Allocator_pvt*) adoptedParent) Identity_ptr = ((struct Allocator_pvt*) adoptedParent); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x856c1578c6e6bdeeull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("Allocator.c"), (743), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x856c1578c6e6bdeeull))"); } } while (0); Identity_ptr; }));
    struct Allocator_pvt* child = (__extension__ ({ __typeof__((struct Allocator_pvt*) childToAdopt) Identity_ptr = ((struct Allocator_pvt*) childToAdopt); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x856c1578c6e6bdeeull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("Allocator.c"), (744), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x856c1578c6e6bdeeull))"); } } while (0); Identity_ptr; }));

    if (parent->pub.isFreeing) { return; }
    do { if (!((!child->pub.isFreeing))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("Allocator.c"), (747), "(!child->pub.isFreeing)"); } } while (0);

    if (isAncestorOf(child, parent)) {


        return;
    }

    if (!parent->adoptions) {
        parent->adoptions =
            Allocator__calloc(adoptedParent, sizeof(struct Allocator_Adoptions), 1, file, line);
    }
    if (!child->adoptions) {
        child->adoptions =
            Allocator__calloc(childToAdopt, sizeof(struct Allocator_Adoptions), 1, file, line);
    }

    struct Allocator_List* pl =
        Allocator__calloc(childToAdopt, sizeof(struct Allocator_List), 1, file, line);
    pl->alloc = child;
    pl->next = parent->adoptions->children;
    parent->adoptions->children = pl;

    struct Allocator_List* cl =
        Allocator__calloc(childToAdopt, sizeof(struct Allocator_List), 1, file, line);
    cl->alloc = parent;
    cl->next = child->adoptions->parents;
    child->adoptions->parents = cl;
}

struct Allocator_OnFreeJob* Allocator__onFree(struct Allocator* alloc,
                                              Allocator_OnFreeCallback callback,
                                              void* callbackContext,
                                              const char* file,
                                              int line)
{
    struct Allocator_pvt* context = (__extension__ ({ __typeof__((struct Allocator_pvt*) alloc) Identity_ptr = ((struct Allocator_pvt*) alloc); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x856c1578c6e6bdeeull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("Allocator.c"), (783), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x856c1578c6e6bdeeull))"); } } while (0); Identity_ptr; }));

    while (context->pub.isFreeing) {

        if (context->parent == context || !context->parent->pub.isFreeing) {
            break;
        }
        context = context->parent;
    }

    struct Allocator_OnFreeJob_pvt* newJob =
        Allocator__clone((alloc),((&(struct Allocator_OnFreeJob_pvt) { .pub = { .callback = callback, .userData = callbackContext }, .alloc = context, .file = file, .line = line })),sizeof(*((&(struct Allocator_OnFreeJob_pvt) { .pub = { .callback = callback, .userData = callbackContext }, .alloc = context, .file = file, .line = line }))),"Allocator.c",794)







           ;
    (newJob)->Identity_verifier = ((unsigned long) 0x856c1578c6e6bdeeull);

    struct Allocator_OnFreeJob_pvt* job = context->onFree;
    if (job == 
# 806 "memory/Allocator.c" 3 4
              ((void *)0)
# 806 "memory/Allocator.c"
                  ) {
        context->onFree = newJob;
    } else {
        while (job->next != 
# 809 "memory/Allocator.c" 3 4
                           ((void *)0)
# 809 "memory/Allocator.c"
                               ) {
            job = job->next;
        }
        job->next = newJob;
    }
    return &newJob->pub;
}

struct Allocator* Allocator_new(unsigned long sizeLimit,
                                Allocator_Provider provider,
                                void* providerContext,
                                const char* fileName,
                                int lineNum)
{
    if (sizeLimit == 0) {
        sizeLimit = 
# 824 "memory/Allocator.c" 3 4
                   9223372036854775807LL 
# 824 "memory/Allocator.c"
                             - getRealSize(sizeof(struct Allocator_FirstCtx));
    }

    sizeLimit += getRealSize(sizeof(struct Allocator_FirstCtx));

    struct Allocator_FirstCtx stackContext = {
        .spaceAvailable = sizeLimit,
        .provider = provider,
        .providerContext = providerContext,
        .context = {
            .pub = {
                .fileName = fileName,
                .lineNum = lineNum,
            },

            .canary = (unsigned long) ((uint64_t) 0x347db462bfa8894eull),
            .nextCanary = (unsigned long) ((uint64_t) 0xb0b0e42ef9ebc8b7ull),

        }
    };
    stackContext.maxSpace = stackContext.spaceAvailable;
    stackContext.context.rootAlloc = &stackContext;
    (&stackContext)->Identity_verifier = ((unsigned long) 0x856c1578c6e6bdeeull);
    (&stackContext.context)->Identity_verifier = ((unsigned long) 0x856c1578c6e6bdeeull);

    struct Allocator_FirstCtx* firstContext =
        Allocator__clone(&stackContext.context.pub,
                         &stackContext,
                         sizeof(struct Allocator_FirstCtx),
                         fileName,
                         lineNum);

    struct Allocator_pvt* context = &firstContext->context;
    context->rootAlloc = firstContext;
    context->parent = context;

    check(context);
    return &context->pub;
}

unsigned long Allocator_bytesAllocated(struct Allocator* allocator)
{
    struct Allocator_pvt* context = (__extension__ ({ __typeof__((struct Allocator_pvt*) allocator) Identity_ptr = ((struct Allocator_pvt*) allocator); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x856c1578c6e6bdeeull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("Allocator.c"), (866), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x856c1578c6e6bdeeull))"); } } while (0); Identity_ptr; }));
    return bytesAllocated(context);
}

void Allocator_setCanary(struct Allocator* alloc, unsigned long value)
{

        struct Allocator_pvt* context = (__extension__ ({ __typeof__((struct Allocator_pvt*) alloc) Identity_ptr = ((struct Allocator_pvt*) alloc); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x856c1578c6e6bdeeull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("Allocator.c"), (873), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x856c1578c6e6bdeeull))"); } } while (0); Identity_ptr; }));
        context->nextCanary ^= value;

}
