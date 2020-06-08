# 1 "util/Set.c"
# 1 "/home/jojapoppa/cjdns//"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "util/Set.c"
# 16 "util/Set.c"
# 1 "./util/Set.h" 1
# 18 "./util/Set.h"
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
struct Linker_x0530169396016394;
# 33 "./util/Assert.h"
__attribute__((__format__ (__printf__, 1, 2)))
__attribute__((__noreturn__))
void Assert_failure(const char* format, ...);
# 19 "./util/Identity.h" 2
# 1 "./util/Constant.h" 1
# 33 "./util/Constant.h"

# 20 "./util/Identity.h" 2



    
# 19 "./memory/Allocator.h" 2


struct Linker_x025141975382827497;





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
# 19 "./util/Set.h" 2
# 1 "./util/Bits.h" 1
# 21 "./util/Bits.h"
struct Linker_x0020636458948296355;

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
# 24 "./util/Bits.h" 2
# 1 "/usr/lib/gcc/x86_64-w64-mingw32/9.2-win32/include/stddef.h" 1 3 4
# 1 "/usr/share/mingw-w64/include/stddef.h" 1 3 4
# 2 "/usr/lib/gcc/x86_64-w64-mingw32/9.2-win32/include/stddef.h" 2 3 4
# 25 "./util/Bits.h" 2





# 29 "./util/Bits.h"
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
# 20 "./util/Set.h" 2
# 1 "./util/UniqueName.h" 1
# 22 "./util/UniqueName.h"

# 21 "./util/Set.h" 2


struct Linker_x09923717609916054;





struct Set;
struct Set_Iter;

typedef int (* Set_Compare_t)(const void* a, const void* b);
typedef uint32_t (* Set_HashCode_t)(const void* a);

int Set_add(struct Set* as, void* val);
void* Set_remove(struct Set* as, void* val);
void* Set_get(struct Set* as, void* val);
struct Set* Set_new(struct Allocator* alloc, Set_HashCode_t, Set_Compare_t);
void Set_iter(struct Set* _set, struct Set_Iter* iter);
void Set_iterNext(struct Set_Iter* iter);
int Set_addCopy(struct Set* _set, void* val, uint32_t size);
# 17 "util/Set.c" 2


# 1 "/usr/lib/gcc/x86_64-w64-mingw32/9.2-win32/include/stddef.h" 1 3 4
# 1 "/usr/share/mingw-w64/include/stddef.h" 1 3 4
# 2 "/usr/lib/gcc/x86_64-w64-mingw32/9.2-win32/include/stddef.h" 2 3 4
# 20 "util/Set.c" 2
# 1 "/usr/share/mingw-w64/include/stdlib.h" 1 3
# 10 "/usr/share/mingw-w64/include/stdlib.h" 3
# 1 "/usr/lib/gcc/x86_64-w64-mingw32/9.2-win32/include-fixed/limits.h" 1 3 4
# 34 "/usr/lib/gcc/x86_64-w64-mingw32/9.2-win32/include-fixed/limits.h" 3 4
# 1 "/usr/lib/gcc/x86_64-w64-mingw32/9.2-win32/include-fixed/syslimits.h" 1 3 4






# 1 "/usr/lib/gcc/x86_64-w64-mingw32/9.2-win32/include-fixed/limits.h" 1 3 4
# 194 "/usr/lib/gcc/x86_64-w64-mingw32/9.2-win32/include-fixed/limits.h" 3 4
# 1 "/usr/share/mingw-w64/include/limits.h" 1 3 4
# 195 "/usr/lib/gcc/x86_64-w64-mingw32/9.2-win32/include-fixed/limits.h" 2 3 4
# 8 "/usr/lib/gcc/x86_64-w64-mingw32/9.2-win32/include-fixed/syslimits.h" 2 3 4
# 35 "/usr/lib/gcc/x86_64-w64-mingw32/9.2-win32/include-fixed/limits.h" 2 3 4
# 11 "/usr/share/mingw-w64/include/stdlib.h" 2 3
# 25 "/usr/share/mingw-w64/include/stdlib.h" 3
#pragma pack(push,_CRT_PACKING)
# 49 "/usr/share/mingw-w64/include/stdlib.h" 3
  
# 49 "/usr/share/mingw-w64/include/stdlib.h" 3
 typedef int (__attribute__((__cdecl__)) *_onexit_t)(void);
# 59 "/usr/share/mingw-w64/include/stdlib.h" 3
  typedef struct _div_t {
    int quot;
    int rem;
  } div_t;

  typedef struct _ldiv_t {
    long quot;
    long rem;
  } ldiv_t;





#pragma pack(4)
  typedef struct {
    unsigned char ld[10];
  } _LDOUBLE;
#pragma pack()



  typedef struct {
    double x;
  } _CRT_DOUBLE;

  typedef struct {
    float f;
  } _CRT_FLOAT;

       


  typedef struct {
    long double x;
  } _LONGDOUBLE;

       

#pragma pack(4)
  typedef struct {
    unsigned char ld12[12];
  } _LDBL12;
#pragma pack()
# 115 "/usr/share/mingw-w64/include/stdlib.h" 3
  extern int * __imp___mb_cur_max;




__attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) ___mb_cur_max_func(void);
# 142 "/usr/share/mingw-w64/include/stdlib.h" 3
  typedef void (__attribute__((__cdecl__)) *_purecall_handler)(void);

  __attribute__ ((__dllimport__)) _purecall_handler __attribute__((__cdecl__)) _set_purecall_handler(_purecall_handler _Handler);
  __attribute__ ((__dllimport__)) _purecall_handler __attribute__((__cdecl__)) _get_purecall_handler(void);

  typedef void (__attribute__((__cdecl__)) *_invalid_parameter_handler)(const wchar_t *,const wchar_t *,const wchar_t *,unsigned int,uintptr_t);
  __attribute__ ((__dllimport__)) _invalid_parameter_handler __attribute__((__cdecl__)) _set_invalid_parameter_handler(_invalid_parameter_handler _Handler);
  __attribute__ ((__dllimport__)) _invalid_parameter_handler __attribute__((__cdecl__)) _get_invalid_parameter_handler(void);
# 158 "/usr/share/mingw-w64/include/stdlib.h" 3
  __attribute__ ((__dllimport__)) unsigned long *__attribute__((__cdecl__)) __doserrno(void);

  errno_t __attribute__((__cdecl__)) _set_doserrno(unsigned long _Value);
  errno_t __attribute__((__cdecl__)) _get_doserrno(unsigned long *_Value);
# 172 "/usr/share/mingw-w64/include/stdlib.h" 3
  extern __attribute__((dllimport)) char *_sys_errlist[1];
  extern __attribute__((dllimport)) int _sys_nerr;





  __attribute__ ((__dllimport__)) char ***__attribute__((__cdecl__)) __p___argv(void);
  __attribute__ ((__dllimport__)) int *__attribute__((__cdecl__)) __p__fmode(void);
# 190 "/usr/share/mingw-w64/include/stdlib.h" 3
  errno_t __attribute__((__cdecl__)) _get_pgmptr(char **_Value);
  errno_t __attribute__((__cdecl__)) _get_wpgmptr(wchar_t **_Value);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _set_fmode(int _Mode);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _get_fmode(int *_PMode);
# 281 "/usr/share/mingw-w64/include/stdlib.h" 3
  extern int * __imp___argc;



  extern char *** __imp___argv;



  extern wchar_t *** __imp___wargv;





  extern char *** __imp__environ;




  extern wchar_t *** __imp__wenviron;





  extern char ** __imp__pgmptr;




  extern wchar_t ** __imp__wpgmptr;




  extern unsigned int * __imp__osplatform;




  extern unsigned int * __imp__osver;




  extern unsigned int * __imp__winver;




  extern unsigned int * __imp__winmajor;




  extern unsigned int * __imp__winminor;





  errno_t __attribute__((__cdecl__)) _get_osplatform(unsigned int *_Value);
  errno_t __attribute__((__cdecl__)) _get_osver(unsigned int *_Value);
  errno_t __attribute__((__cdecl__)) _get_winver(unsigned int *_Value);
  errno_t __attribute__((__cdecl__)) _get_winmajor(unsigned int *_Value);
  errno_t __attribute__((__cdecl__)) _get_winminor(unsigned int *_Value);
# 360 "/usr/share/mingw-w64/include/stdlib.h" 3
  void __attribute__((__cdecl__)) __attribute__ ((__nothrow__)) exit(int _Code) __attribute__ ((__noreturn__));
  void __attribute__((__cdecl__)) __attribute__ ((__nothrow__)) _exit(int _Code) __attribute__ ((__noreturn__));



  void __attribute__((__cdecl__)) _Exit(int) __attribute__ ((__noreturn__));






       

  void __attribute__((__cdecl__)) __attribute__ ((__noreturn__)) abort(void);
       



  __attribute__ ((__dllimport__)) unsigned int __attribute__((__cdecl__)) _set_abort_behavior(unsigned int _Flags,unsigned int _Mask);



  int __attribute__((__cdecl__)) abs(int _X);
  long __attribute__((__cdecl__)) labs(long _X);


  __extension__ long long __attribute__((__cdecl__)) _abs64(long long);

  extern __inline__ __attribute__((__always_inline__,__gnu_inline__)) long long __attribute__((__cdecl__)) _abs64(long long x) {
    return __builtin_llabs(x);
  }


  int __attribute__((__cdecl__)) atexit(void (__attribute__((__cdecl__)) *)(void));


  double __attribute__((__cdecl__)) atof(const char *_String);
  double __attribute__((__cdecl__)) _atof_l(const char *_String,_locale_t _Locale);

  int __attribute__((__cdecl__)) atoi(const char *_Str);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _atoi_l(const char *_Str,_locale_t _Locale);
  long __attribute__((__cdecl__)) atol(const char *_Str);
  __attribute__ ((__dllimport__)) long __attribute__((__cdecl__)) _atol_l(const char *_Str,_locale_t _Locale);


  void *__attribute__((__cdecl__)) bsearch(const void *_Key,const void *_Base,size_t _NumOfElements,size_t _SizeOfElements,int (__attribute__((__cdecl__)) *_PtFuncCompare)(const void *,const void *));
  void __attribute__((__cdecl__)) qsort(void *_Base,size_t _NumOfElements,size_t _SizeOfElements,int (__attribute__((__cdecl__)) *_PtFuncCompare)(const void *,const void *));

  unsigned short __attribute__((__cdecl__)) _byteswap_ushort(unsigned short _Short);
  unsigned long __attribute__((__cdecl__)) _byteswap_ulong (unsigned long _Long);
  __extension__ unsigned long long __attribute__((__cdecl__)) _byteswap_uint64(unsigned long long _Int64);
  div_t __attribute__((__cdecl__)) div(int _Numerator,int _Denominator);
  char *__attribute__((__cdecl__)) getenv(const char *_VarName) ;
  __attribute__ ((__dllimport__)) char *__attribute__((__cdecl__)) _itoa(int _Value,char *_Dest,int _Radix);
  __extension__ __attribute__ ((__dllimport__)) char *__attribute__((__cdecl__)) _i64toa(long long _Val,char *_DstBuf,int _Radix) ;
  __extension__ __attribute__ ((__dllimport__)) char *__attribute__((__cdecl__)) _ui64toa(unsigned long long _Val,char *_DstBuf,int _Radix) ;
  __extension__ __attribute__ ((__dllimport__)) long long __attribute__((__cdecl__)) _atoi64(const char *_String);
  __extension__ __attribute__ ((__dllimport__)) long long __attribute__((__cdecl__)) _atoi64_l(const char *_String,_locale_t _Locale);
  __extension__ __attribute__ ((__dllimport__)) long long __attribute__((__cdecl__)) _strtoi64(const char *_String,char **_EndPtr,int _Radix);
  __extension__ __attribute__ ((__dllimport__)) long long __attribute__((__cdecl__)) _strtoi64_l(const char *_String,char **_EndPtr,int _Radix,_locale_t _Locale);
  __extension__ __attribute__ ((__dllimport__)) unsigned long long __attribute__((__cdecl__)) _strtoui64(const char *_String,char **_EndPtr,int _Radix);
  __extension__ __attribute__ ((__dllimport__)) unsigned long long __attribute__((__cdecl__)) _strtoui64_l(const char *_String,char **_EndPtr,int _Radix,_locale_t _Locale);
  ldiv_t __attribute__((__cdecl__)) ldiv(long _Numerator,long _Denominator);
  __attribute__ ((__dllimport__)) char *__attribute__((__cdecl__)) _ltoa(long _Value,char *_Dest,int _Radix) ;
  int __attribute__((__cdecl__)) mblen(const char *_Ch,size_t _MaxCount);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _mblen_l(const char *_Ch,size_t _MaxCount,_locale_t _Locale);
  __attribute__ ((__dllimport__)) size_t __attribute__((__cdecl__)) _mbstrlen(const char *_Str);
  __attribute__ ((__dllimport__)) size_t __attribute__((__cdecl__)) _mbstrlen_l(const char *_Str,_locale_t _Locale);
  __attribute__ ((__dllimport__)) size_t __attribute__((__cdecl__)) _mbstrnlen(const char *_Str,size_t _MaxCount);
  __attribute__ ((__dllimport__)) size_t __attribute__((__cdecl__)) _mbstrnlen_l(const char *_Str,size_t _MaxCount,_locale_t _Locale);
  int __attribute__((__cdecl__)) mbtowc(wchar_t * __restrict__ _DstCh,const char * __restrict__ _SrcCh,size_t _SrcSizeInBytes);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _mbtowc_l(wchar_t * __restrict__ _DstCh,const char * __restrict__ _SrcCh,size_t _SrcSizeInBytes,_locale_t _Locale);
  size_t __attribute__((__cdecl__)) mbstowcs(wchar_t * __restrict__ _Dest,const char * __restrict__ _Source,size_t _MaxCount);
  __attribute__ ((__dllimport__)) size_t __attribute__((__cdecl__)) _mbstowcs_l(wchar_t * __restrict__ _Dest,const char * __restrict__ _Source,size_t _MaxCount,_locale_t _Locale);
  int __attribute__((__cdecl__)) mkstemp(char *template_name);
  int __attribute__((__cdecl__)) rand(void);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _set_error_mode(int _Mode);
  void __attribute__((__cdecl__)) srand(unsigned int _Seed);
# 465 "/usr/share/mingw-w64/include/stdlib.h" 3
  double __attribute__((__cdecl__)) __attribute__ ((__nothrow__)) strtod(const char * __restrict__ _Str,char ** __restrict__ _EndPtr);
  float __attribute__((__cdecl__)) __attribute__ ((__nothrow__)) strtof(const char * __restrict__ nptr, char ** __restrict__ endptr);

  long double __attribute__((__cdecl__)) __attribute__ ((__nothrow__)) strtold(const char * __restrict__ , char ** __restrict__ );


  extern double __attribute__((__cdecl__)) __attribute__ ((__nothrow__))
  __strtod (const char * __restrict__ , char ** __restrict__);







  float __attribute__((__cdecl__)) __mingw_strtof (const char * __restrict__, char ** __restrict__);
  double __attribute__((__cdecl__)) __mingw_strtod (const char * __restrict__, char ** __restrict__);
  long double __attribute__((__cdecl__)) __mingw_strtold(const char * __restrict__, char ** __restrict__);

  __attribute__ ((__dllimport__)) double __attribute__((__cdecl__)) _strtod_l(const char * __restrict__ _Str,char ** __restrict__ _EndPtr,_locale_t _Locale);
  long __attribute__((__cdecl__)) strtol(const char * __restrict__ _Str,char ** __restrict__ _EndPtr,int _Radix);
  __attribute__ ((__dllimport__)) long __attribute__((__cdecl__)) _strtol_l(const char * __restrict__ _Str,char ** __restrict__ _EndPtr,int _Radix,_locale_t _Locale);
  unsigned long __attribute__((__cdecl__)) strtoul(const char * __restrict__ _Str,char ** __restrict__ _EndPtr,int _Radix);
  __attribute__ ((__dllimport__)) unsigned long __attribute__((__cdecl__)) _strtoul_l(const char * __restrict__ _Str,char ** __restrict__ _EndPtr,int _Radix,_locale_t _Locale);


  int __attribute__((__cdecl__)) system(const char *_Command);

  __attribute__ ((__dllimport__)) char *__attribute__((__cdecl__)) _ultoa(unsigned long _Value,char *_Dest,int _Radix) ;
  int __attribute__((__cdecl__)) wctomb(char *_MbCh,wchar_t _WCh) ;
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wctomb_l(char *_MbCh,wchar_t _WCh,_locale_t _Locale) ;
  size_t __attribute__((__cdecl__)) wcstombs(char * __restrict__ _Dest,const wchar_t * __restrict__ _Source,size_t _MaxCount) ;
  __attribute__ ((__dllimport__)) size_t __attribute__((__cdecl__)) _wcstombs_l(char * __restrict__ _Dest,const wchar_t * __restrict__ _Source,size_t _MaxCount,_locale_t _Locale) ;



  void *__attribute__((__cdecl__)) calloc(size_t _NumOfElements,size_t _SizeOfElements);
  void __attribute__((__cdecl__)) free(void *_Memory);
  void *__attribute__((__cdecl__)) malloc(size_t _Size);
  void *__attribute__((__cdecl__)) realloc(void *_Memory,size_t _NewSize);
  __attribute__ ((__dllimport__)) void *__attribute__((__cdecl__)) _recalloc(void *_Memory,size_t _Count,size_t _Size);


       
       


  __attribute__ ((__dllimport__)) void __attribute__((__cdecl__)) _aligned_free(void *_Memory);
  __attribute__ ((__dllimport__)) void *__attribute__((__cdecl__)) _aligned_malloc(size_t _Size,size_t _Alignment);
       
       

  __attribute__ ((__dllimport__)) void *__attribute__((__cdecl__)) _aligned_offset_malloc(size_t _Size,size_t _Alignment,size_t _Offset);
  __attribute__ ((__dllimport__)) void *__attribute__((__cdecl__)) _aligned_realloc(void *_Memory,size_t _Size,size_t _Alignment);
  __attribute__ ((__dllimport__)) void *__attribute__((__cdecl__)) _aligned_recalloc(void *_Memory,size_t _Count,size_t _Size,size_t _Alignment);
  __attribute__ ((__dllimport__)) void *__attribute__((__cdecl__)) _aligned_offset_realloc(void *_Memory,size_t _Size,size_t _Alignment,size_t _Offset);
  __attribute__ ((__dllimport__)) void *__attribute__((__cdecl__)) _aligned_offset_recalloc(void *_Memory,size_t _Count,size_t _Size,size_t _Alignment,size_t _Offset);





  __attribute__ ((__dllimport__)) wchar_t *__attribute__((__cdecl__)) _itow(int _Value,wchar_t *_Dest,int _Radix) ;
  __attribute__ ((__dllimport__)) wchar_t *__attribute__((__cdecl__)) _ltow(long _Value,wchar_t *_Dest,int _Radix) ;
  __attribute__ ((__dllimport__)) wchar_t *__attribute__((__cdecl__)) _ultow(unsigned long _Value,wchar_t *_Dest,int _Radix) ;

  double __attribute__((__cdecl__)) __mingw_wcstod(const wchar_t * __restrict__ _Str,wchar_t ** __restrict__ _EndPtr);
  float __attribute__((__cdecl__)) __mingw_wcstof(const wchar_t * __restrict__ nptr, wchar_t ** __restrict__ endptr);
  long double __attribute__((__cdecl__)) __mingw_wcstold(const wchar_t * __restrict__, wchar_t ** __restrict__);
# 546 "/usr/share/mingw-w64/include/stdlib.h" 3
  double __attribute__((__cdecl__)) wcstod(const wchar_t * __restrict__ _Str,wchar_t ** __restrict__ _EndPtr);
  float __attribute__((__cdecl__)) wcstof(const wchar_t * __restrict__ nptr, wchar_t ** __restrict__ endptr);


  long double __attribute__((__cdecl__)) wcstold(const wchar_t * __restrict__, wchar_t ** __restrict__);

  __attribute__ ((__dllimport__)) double __attribute__((__cdecl__)) _wcstod_l(const wchar_t * __restrict__ _Str,wchar_t ** __restrict__ _EndPtr,_locale_t _Locale);
  long __attribute__((__cdecl__)) wcstol(const wchar_t * __restrict__ _Str,wchar_t ** __restrict__ _EndPtr,int _Radix);
  __attribute__ ((__dllimport__)) long __attribute__((__cdecl__)) _wcstol_l(const wchar_t * __restrict__ _Str,wchar_t ** __restrict__ _EndPtr,int _Radix,_locale_t _Locale);
  unsigned long __attribute__((__cdecl__)) wcstoul(const wchar_t * __restrict__ _Str,wchar_t ** __restrict__ _EndPtr,int _Radix);
  __attribute__ ((__dllimport__)) unsigned long __attribute__((__cdecl__)) _wcstoul_l(const wchar_t * __restrict__ _Str,wchar_t ** __restrict__ _EndPtr,int _Radix,_locale_t _Locale);
  __attribute__ ((__dllimport__)) wchar_t *__attribute__((__cdecl__)) _wgetenv(const wchar_t *_VarName) ;


  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wsystem(const wchar_t *_Command);

  __attribute__ ((__dllimport__)) double __attribute__((__cdecl__)) _wtof(const wchar_t *_Str);
  __attribute__ ((__dllimport__)) double __attribute__((__cdecl__)) _wtof_l(const wchar_t *_Str,_locale_t _Locale);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wtoi(const wchar_t *_Str);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wtoi_l(const wchar_t *_Str,_locale_t _Locale);
  __attribute__ ((__dllimport__)) long __attribute__((__cdecl__)) _wtol(const wchar_t *_Str);
  __attribute__ ((__dllimport__)) long __attribute__((__cdecl__)) _wtol_l(const wchar_t *_Str,_locale_t _Locale);

  __extension__ __attribute__ ((__dllimport__)) wchar_t *__attribute__((__cdecl__)) _i64tow(long long _Val,wchar_t *_DstBuf,int _Radix) ;
  __extension__ __attribute__ ((__dllimport__)) wchar_t *__attribute__((__cdecl__)) _ui64tow(unsigned long long _Val,wchar_t *_DstBuf,int _Radix) ;
  __extension__ __attribute__ ((__dllimport__)) long long __attribute__((__cdecl__)) _wtoi64(const wchar_t *_Str);
  __extension__ __attribute__ ((__dllimport__)) long long __attribute__((__cdecl__)) _wtoi64_l(const wchar_t *_Str,_locale_t _Locale);
  __extension__ __attribute__ ((__dllimport__)) long long __attribute__((__cdecl__)) _wcstoi64(const wchar_t *_Str,wchar_t **_EndPtr,int _Radix);
  __extension__ __attribute__ ((__dllimport__)) long long __attribute__((__cdecl__)) _wcstoi64_l(const wchar_t *_Str,wchar_t **_EndPtr,int _Radix,_locale_t _Locale);
  __extension__ __attribute__ ((__dllimport__)) unsigned long long __attribute__((__cdecl__)) _wcstoui64(const wchar_t *_Str,wchar_t **_EndPtr,int _Radix);
  __extension__ __attribute__ ((__dllimport__)) unsigned long long __attribute__((__cdecl__)) _wcstoui64_l(const wchar_t *_Str ,wchar_t **_EndPtr,int _Radix,_locale_t _Locale);


  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _putenv(const char *_EnvString);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wputenv(const wchar_t *_EnvString);



  __attribute__ ((__dllimport__)) char *__attribute__((__cdecl__)) _fullpath(char *_FullPath,const char *_Path,size_t _SizeInBytes);
  __attribute__ ((__dllimport__)) char *__attribute__((__cdecl__)) _ecvt(double _Val,int _NumOfDigits,int *_PtDec,int *_PtSign) ;
  __attribute__ ((__dllimport__)) char *__attribute__((__cdecl__)) _fcvt(double _Val,int _NumOfDec,int *_PtDec,int *_PtSign) ;
  __attribute__ ((__dllimport__)) char *__attribute__((__cdecl__)) _gcvt(double _Val,int _NumOfDigits,char *_DstBuf) ;
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _atodbl(_CRT_DOUBLE *_Result,char *_Str);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _atoldbl(_LDOUBLE *_Result,char *_Str);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _atoflt(_CRT_FLOAT *_Result,char *_Str);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _atodbl_l(_CRT_DOUBLE *_Result,char *_Str,_locale_t _Locale);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _atoldbl_l(_LDOUBLE *_Result,char *_Str,_locale_t _Locale);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _atoflt_l(_CRT_FLOAT *_Result,char *_Str,_locale_t _Locale);
# 609 "/usr/share/mingw-w64/include/stdlib.h" 3
unsigned long __attribute__((__cdecl__)) _lrotl(unsigned long,int);
unsigned long __attribute__((__cdecl__)) _lrotr(unsigned long,int);





  __attribute__ ((__dllimport__)) void __attribute__((__cdecl__)) _makepath(char *_Path,const char *_Drive,const char *_Dir,const char *_Filename,const char *_Ext);
  _onexit_t __attribute__((__cdecl__)) _onexit(_onexit_t _Func);



  void __attribute__((__cdecl__)) perror(const char *_ErrMsg);

       
       


  __extension__ unsigned long long __attribute__((__cdecl__)) _rotl64(unsigned long long _Val,int _Shift);
  __extension__ unsigned long long __attribute__((__cdecl__)) _rotr64(unsigned long long Value,int Shift);
       
       
       
       


  unsigned int __attribute__((__cdecl__)) _rotr(unsigned int _Val,int _Shift);
  unsigned int __attribute__((__cdecl__)) _rotl(unsigned int _Val,int _Shift);
       
       
  __extension__ unsigned long long __attribute__((__cdecl__)) _rotr64(unsigned long long _Val,int _Shift);
  __attribute__ ((__dllimport__)) void __attribute__((__cdecl__)) _searchenv(const char *_Filename,const char *_EnvVar,char *_ResultPath) ;
  __attribute__ ((__dllimport__)) void __attribute__((__cdecl__)) _splitpath(const char *_FullPath,char *_Drive,char *_Dir,char *_Filename,char *_Ext) ;
  __attribute__ ((__dllimport__)) void __attribute__((__cdecl__)) _swab(char *_Buf1,char *_Buf2,int _SizeInBytes);



  __attribute__ ((__dllimport__)) wchar_t *__attribute__((__cdecl__)) _wfullpath(wchar_t *_FullPath,const wchar_t *_Path,size_t _SizeInWords);
  __attribute__ ((__dllimport__)) void __attribute__((__cdecl__)) _wmakepath(wchar_t *_ResultPath,const wchar_t *_Drive,const wchar_t *_Dir,const wchar_t *_Filename,const wchar_t *_Ext);


  __attribute__ ((__dllimport__)) void __attribute__((__cdecl__)) _wperror(const wchar_t *_ErrMsg);

  __attribute__ ((__dllimport__)) void __attribute__((__cdecl__)) _wsearchenv(const wchar_t *_Filename,const wchar_t *_EnvVar,wchar_t *_ResultPath) ;
  __attribute__ ((__dllimport__)) void __attribute__((__cdecl__)) _wsplitpath(const wchar_t *_FullPath,wchar_t *_Drive,wchar_t *_Dir,wchar_t *_Filename,wchar_t *_Ext) ;


  __attribute__ ((__dllimport__)) void __attribute__((__cdecl__)) _beep(unsigned _Frequency,unsigned _Duration) __attribute__ ((__deprecated__));

  __attribute__ ((__dllimport__)) void __attribute__((__cdecl__)) _seterrormode(int _Mode) __attribute__ ((__deprecated__));
  __attribute__ ((__dllimport__)) void __attribute__((__cdecl__)) _sleep(unsigned long _Duration) __attribute__ ((__deprecated__));
# 680 "/usr/share/mingw-w64/include/stdlib.h" 3
  char *__attribute__((__cdecl__)) ecvt(double _Val,int _NumOfDigits,int *_PtDec,int *_PtSign) ;
  char *__attribute__((__cdecl__)) fcvt(double _Val,int _NumOfDec,int *_PtDec,int *_PtSign) ;
  char *__attribute__((__cdecl__)) gcvt(double _Val,int _NumOfDigits,char *_DstBuf) ;
  char *__attribute__((__cdecl__)) itoa(int _Val,char *_DstBuf,int _Radix) ;
  char *__attribute__((__cdecl__)) ltoa(long _Val,char *_DstBuf,int _Radix) ;
  int __attribute__((__cdecl__)) putenv(const char *_EnvString) ;



  void __attribute__((__cdecl__)) swab(char *_Buf1,char *_Buf2,int _SizeInBytes) ;


  char *__attribute__((__cdecl__)) ultoa(unsigned long _Val,char *_Dstbuf,int _Radix) ;
  _onexit_t __attribute__((__cdecl__)) onexit(_onexit_t _Func);





  typedef struct { __extension__ long long quot, rem; } lldiv_t;

  __extension__ lldiv_t __attribute__((__cdecl__)) lldiv(long long, long long);

  __extension__ long long __attribute__((__cdecl__)) llabs(long long);




  __extension__ long long __attribute__((__cdecl__)) strtoll(const char * __restrict__, char ** __restrict, int);
  __extension__ unsigned long long __attribute__((__cdecl__)) strtoull(const char * __restrict__, char ** __restrict__, int);


  __extension__ long long __attribute__((__cdecl__)) atoll (const char *);


  __extension__ long long __attribute__((__cdecl__)) wtoll (const wchar_t *);
  __extension__ char *__attribute__((__cdecl__)) lltoa (long long, char *, int);
  __extension__ char *__attribute__((__cdecl__)) ulltoa (unsigned long long , char *, int);
  __extension__ wchar_t *__attribute__((__cdecl__)) lltow (long long, wchar_t *, int);
  __extension__ wchar_t *__attribute__((__cdecl__)) ulltow (unsigned long long, wchar_t *, int);
# 738 "/usr/share/mingw-w64/include/stdlib.h" 3
#pragma pack(pop)

# 1 "/usr/share/mingw-w64/include/sec_api/stdlib_s.h" 1 3
# 9 "/usr/share/mingw-w64/include/sec_api/stdlib_s.h" 3
# 1 "/usr/share/mingw-w64/include/stdlib.h" 1 3
# 10 "/usr/share/mingw-w64/include/sec_api/stdlib_s.h" 2 3







  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _dupenv_s(char **_PBuffer,size_t *_PBufferSizeInBytes,const char *_VarName);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _itoa_s(int _Value,char *_DstBuf,size_t _Size,int _Radix);
 
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _i64toa_s(long long _Val,char *_DstBuf,size_t _Size,int _Radix);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _ui64toa_s(unsigned long long _Val,char *_DstBuf,size_t _Size,int _Radix);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _ltoa_s(long _Val,char *_DstBuf,size_t _Size,int _Radix);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) mbstowcs_s(size_t *_PtNumOfCharConverted,wchar_t *_DstBuf,size_t _SizeInWords,const char *_SrcBuf,size_t _MaxCount);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _mbstowcs_s_l(size_t *_PtNumOfCharConverted,wchar_t *_DstBuf,size_t _SizeInWords,const char *_SrcBuf,size_t _MaxCount,_locale_t _Locale);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _ultoa_s(unsigned long _Val,char *_DstBuf,size_t _Size,int _Radix);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _wctomb_s_l(int *_SizeConverted,char *_MbCh,size_t _SizeInBytes,wchar_t _WCh,_locale_t _Locale);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) wcstombs_s(size_t *_PtNumOfCharConverted,char *_Dst,size_t _DstSizeInBytes,const wchar_t *_Src,size_t _MaxCountInBytes);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _wcstombs_s_l(size_t *_PtNumOfCharConverted,char *_Dst,size_t _DstSizeInBytes,const wchar_t *_Src,size_t _MaxCountInBytes,_locale_t _Locale);



  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _itow_s (int _Val,wchar_t *_DstBuf,size_t _SizeInWords,int _Radix);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _ltow_s (long _Val,wchar_t *_DstBuf,size_t _SizeInWords,int _Radix);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _ultow_s (unsigned long _Val,wchar_t *_DstBuf,size_t _SizeInWords,int _Radix);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _wgetenv_s(size_t *_ReturnSize,wchar_t *_DstBuf,size_t _DstSizeInWords,const wchar_t *_VarName);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _wdupenv_s(wchar_t **_Buffer,size_t *_BufferSizeInWords,const wchar_t *_VarName);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _i64tow_s(long long _Val,wchar_t *_DstBuf,size_t _SizeInWords,int _Radix);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _ui64tow_s(unsigned long long _Val,wchar_t *_DstBuf,size_t _SizeInWords,int _Radix);



  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _ecvt_s(char *_DstBuf,size_t _Size,double _Val,int _NumOfDights,int *_PtDec,int *_PtSign);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _fcvt_s(char *_DstBuf,size_t _Size,double _Val,int _NumOfDec,int *_PtDec,int *_PtSign);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _gcvt_s(char *_DstBuf,size_t _Size,double _Val,int _NumOfDigits);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _makepath_s(char *_PathResult,size_t _Size,const char *_Drive,const char *_Dir,const char *_Filename,const char *_Ext);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _putenv_s(const char *_Name,const char *_Value);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _searchenv_s(const char *_Filename,const char *_EnvVar,char *_ResultPath,size_t _SizeInBytes);

  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _splitpath_s(const char *_FullPath,char *_Drive,size_t _DriveSize,char *_Dir,size_t _DirSize,char *_Filename,size_t _FilenameSize,char *_Ext,size_t _ExtSize);
 



  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _wmakepath_s(wchar_t *_PathResult,size_t _SizeInWords,const wchar_t *_Drive,const wchar_t *_Dir,const wchar_t *_Filename,const wchar_t *_Ext);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _wputenv_s(const wchar_t *_Name,const wchar_t *_Value);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _wsearchenv_s(const wchar_t *_Filename,const wchar_t *_EnvVar,wchar_t *_ResultPath,size_t _SizeInWords);

  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _wsplitpath_s(const wchar_t *_FullPath,wchar_t *_Drive,size_t _DriveSizeInWords,wchar_t *_Dir,size_t _DirSizeInWords,wchar_t *_Filename,size_t _FilenameSizeInWords,wchar_t *_Ext,size_t _ExtSizeInWords);
 





  __attribute__ ((__dllimport__)) void __attribute__((__cdecl__)) qsort_s(void *_Base,size_t _NumOfElements,size_t _SizeOfElements,int (__attribute__((__cdecl__)) *_PtFuncCompare)(void *,const void *,const void *),void *_Context);
# 741 "/usr/share/mingw-w64/include/stdlib.h" 2 3
# 1 "/usr/share/mingw-w64/include/malloc.h" 1 3
# 11 "/usr/share/mingw-w64/include/malloc.h" 3
#pragma pack(push,_CRT_PACKING)
# 46 "/usr/share/mingw-w64/include/malloc.h" 3
  typedef struct _heapinfo {
    int *_pentry;
    size_t _size;
    int _useflag;
  } _HEAPINFO;


  extern unsigned int _amblksiz;
# 65 "/usr/share/mingw-w64/include/malloc.h" 3
       
       
# 97 "/usr/share/mingw-w64/include/malloc.h" 3
       
       




void * __mingw_aligned_malloc (size_t _Size, size_t _Alignment);
void __mingw_aligned_free (void *_Memory);
void * __mingw_aligned_offset_realloc (void *_Memory, size_t _Size, size_t _Alignment, size_t _Offset);
void * __mingw_aligned_realloc (void *_Memory, size_t _Size, size_t _Offset);



  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _resetstkoflw (void);
  __attribute__ ((__dllimport__)) unsigned long __attribute__((__cdecl__)) _set_malloc_crt_max_wait(unsigned long _NewValue);

  __attribute__ ((__dllimport__)) void *__attribute__((__cdecl__)) _expand(void *_Memory,size_t _NewSize);
  __attribute__ ((__dllimport__)) size_t __attribute__((__cdecl__)) _msize(void *_Memory);






  __attribute__ ((__dllimport__)) size_t __attribute__((__cdecl__)) _get_sbh_threshold(void);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _set_sbh_threshold(size_t _NewValue);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _set_amblksiz(size_t _Value);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _get_amblksiz(size_t *_Value);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _heapadd(void *_Memory,size_t _Size);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _heapchk(void);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _heapmin(void);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _heapset(unsigned int _Fill);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _heapwalk(_HEAPINFO *_EntryInfo);
  __attribute__ ((__dllimport__)) size_t __attribute__((__cdecl__)) _heapused(size_t *_Used,size_t *_Commit);
  __attribute__ ((__dllimport__)) intptr_t __attribute__((__cdecl__)) _get_heap_handle(void);
# 144 "/usr/share/mingw-w64/include/malloc.h" 3
  static __inline void *_MarkAllocaS(void *_Ptr,unsigned int _Marker) {
    if(_Ptr) {
      *((unsigned int*)_Ptr) = _Marker;
      _Ptr = (char*)_Ptr + 16;
    }
    return _Ptr;
  }
# 163 "/usr/share/mingw-w64/include/malloc.h" 3
  static __inline void __attribute__((__cdecl__)) _freea(void *_Memory) {
    unsigned int _Marker;
    if(_Memory) {
      _Memory = (char*)_Memory - 16;
      _Marker = *(unsigned int *)_Memory;
      if(_Marker==0xDDDD) {
 free(_Memory);
      }





    }
  }
# 209 "/usr/share/mingw-w64/include/malloc.h" 3
#pragma pack(pop)
# 742 "/usr/share/mingw-w64/include/stdlib.h" 2 3
# 21 "util/Set.c" 2

# 1 "./node_build/dependencies/tree.h" 1
# 23 "util/Set.c" 2


# 24 "util/Set.c"
struct Entry {
    void* data;
    uint32_t hashCode;
    struct Allocator* alloc;
    struct Set_pvt* set;
    struct {
        struct Entry* rbe_left;
        struct Entry* rbe_right;
        struct Entry* rbe_parent;
        int rbe_color;
    } tree;
};


struct Block {
    int usedCount;
    int number;
    struct Block* next;
    struct Allocator* alloc;
    struct Entry entries[8];
};

struct Set_Iter
{
    void* val;
    struct Entry* entry;
};

struct Set_pvt
{
    int size;

    struct Allocator* alloc;
    struct Block* block;

    struct ActiveTree {
        struct Entry* rbh_root;
    } activeTree;

    struct FreeTree {
        struct Entry* rbh_root;
    } freeTree;

    Set_Compare_t compare;
    Set_HashCode_t hashCode;

    unsigned long Identity_verifier;
};

static int compare(const struct Entry* a, const struct Entry* b)
{
    if (a->hashCode != b->hashCode) { return a->hashCode - b->hashCode; }
    struct Set_pvt* set = (__extension__ ({ __typeof__((struct Set_pvt*) a->set) Identity_ptr = ((struct Set_pvt*) a->set); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x4ec76200a73e652cull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("Set.c"), (76), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x4ec76200a73e652cull))"); } } while (0); Identity_ptr; }));
    return set->compare(a->data, b->data);
}
__attribute__((unused)) static void ActiveTree_RB_INSERT_COLOR(struct ActiveTree *head, struct Entry *elm) { struct Entry *parent, *gparent, *tmp; while ((parent = (elm)->tree.rbe_parent) != 
# 79 "util/Set.c" 3 4
((void *)0) 
# 79 "util/Set.c"
&& (parent)->tree.rbe_color == 1) { gparent = (parent)->tree.rbe_parent; if (parent == (gparent)->tree.rbe_left) { tmp = (gparent)->tree.rbe_right; if (tmp && (tmp)->tree.rbe_color == 1) { (tmp)->tree.rbe_color = 0; do { (parent)->tree.rbe_color = 0; (gparent)->tree.rbe_color = 1; } while ( 0); elm = gparent; continue; } if ((parent)->tree.rbe_right == elm) { do { (tmp) = (parent)->tree.rbe_right; if (((parent)->tree.rbe_right = (tmp)->tree.rbe_left) != 
# 79 "util/Set.c" 3 4
((void *)0)
# 79 "util/Set.c"
) { ((tmp)->tree.rbe_left)->tree.rbe_parent = (parent); } do {} while (0); if (((tmp)->tree.rbe_parent = (parent)->tree.rbe_parent) != 
# 79 "util/Set.c" 3 4
((void *)0)
# 79 "util/Set.c"
) { if ((parent) == ((parent)->tree.rbe_parent)->tree.rbe_left) ((parent)->tree.rbe_parent)->tree.rbe_left = (tmp); else ((parent)->tree.rbe_parent)->tree.rbe_right = (tmp); } else (head)->rbh_root = (tmp); (tmp)->tree.rbe_left = (parent); (parent)->tree.rbe_parent = (tmp); do {} while (0); if (((tmp)->tree.rbe_parent)) do {} while (0); } while ( 0); tmp = parent; parent = elm; elm = tmp; } do { (parent)->tree.rbe_color = 0; (gparent)->tree.rbe_color = 1; } while ( 0); do { (tmp) = (gparent)->tree.rbe_left; if (((gparent)->tree.rbe_left = (tmp)->tree.rbe_right) != 
# 79 "util/Set.c" 3 4
((void *)0)
# 79 "util/Set.c"
) { ((tmp)->tree.rbe_right)->tree.rbe_parent = (gparent); } do {} while (0); if (((tmp)->tree.rbe_parent = (gparent)->tree.rbe_parent) != 
# 79 "util/Set.c" 3 4
((void *)0)
# 79 "util/Set.c"
) { if ((gparent) == ((gparent)->tree.rbe_parent)->tree.rbe_left) ((gparent)->tree.rbe_parent)->tree.rbe_left = (tmp); else ((gparent)->tree.rbe_parent)->tree.rbe_right = (tmp); } else (head)->rbh_root = (tmp); (tmp)->tree.rbe_right = (gparent); (gparent)->tree.rbe_parent = (tmp); do {} while (0); if (((tmp)->tree.rbe_parent)) do {} while (0); } while ( 0); } else { tmp = (gparent)->tree.rbe_left; if (tmp && (tmp)->tree.rbe_color == 1) { (tmp)->tree.rbe_color = 0; do { (parent)->tree.rbe_color = 0; (gparent)->tree.rbe_color = 1; } while ( 0); elm = gparent; continue; } if ((parent)->tree.rbe_left == elm) { do { (tmp) = (parent)->tree.rbe_left; if (((parent)->tree.rbe_left = (tmp)->tree.rbe_right) != 
# 79 "util/Set.c" 3 4
((void *)0)
# 79 "util/Set.c"
) { ((tmp)->tree.rbe_right)->tree.rbe_parent = (parent); } do {} while (0); if (((tmp)->tree.rbe_parent = (parent)->tree.rbe_parent) != 
# 79 "util/Set.c" 3 4
((void *)0)
# 79 "util/Set.c"
) { if ((parent) == ((parent)->tree.rbe_parent)->tree.rbe_left) ((parent)->tree.rbe_parent)->tree.rbe_left = (tmp); else ((parent)->tree.rbe_parent)->tree.rbe_right = (tmp); } else (head)->rbh_root = (tmp); (tmp)->tree.rbe_right = (parent); (parent)->tree.rbe_parent = (tmp); do {} while (0); if (((tmp)->tree.rbe_parent)) do {} while (0); } while ( 0); tmp = parent; parent = elm; elm = tmp; } do { (parent)->tree.rbe_color = 0; (gparent)->tree.rbe_color = 1; } while ( 0); do { (tmp) = (gparent)->tree.rbe_right; if (((gparent)->tree.rbe_right = (tmp)->tree.rbe_left) != 
# 79 "util/Set.c" 3 4
((void *)0)
# 79 "util/Set.c"
) { ((tmp)->tree.rbe_left)->tree.rbe_parent = (gparent); } do {} while (0); if (((tmp)->tree.rbe_parent = (gparent)->tree.rbe_parent) != 
# 79 "util/Set.c" 3 4
((void *)0)
# 79 "util/Set.c"
) { if ((gparent) == ((gparent)->tree.rbe_parent)->tree.rbe_left) ((gparent)->tree.rbe_parent)->tree.rbe_left = (tmp); else ((gparent)->tree.rbe_parent)->tree.rbe_right = (tmp); } else (head)->rbh_root = (tmp); (tmp)->tree.rbe_left = (gparent); (gparent)->tree.rbe_parent = (tmp); do {} while (0); if (((tmp)->tree.rbe_parent)) do {} while (0); } while ( 0); } } (head->rbh_root)->tree.rbe_color = 0; } __attribute__((unused)) static void ActiveTree_RB_REMOVE_COLOR(struct ActiveTree *head, struct Entry *parent, struct Entry *elm) { struct Entry *tmp; while ((elm == 
# 79 "util/Set.c" 3 4
((void *)0) 
# 79 "util/Set.c"
|| (elm)->tree.rbe_color == 0) && elm != (head)->rbh_root) { if ((parent)->tree.rbe_left == elm) { tmp = (parent)->tree.rbe_right; if ((tmp)->tree.rbe_color == 1) { do { (tmp)->tree.rbe_color = 0; (parent)->tree.rbe_color = 1; } while ( 0); do { (tmp) = (parent)->tree.rbe_right; if (((parent)->tree.rbe_right = (tmp)->tree.rbe_left) != 
# 79 "util/Set.c" 3 4
((void *)0)
# 79 "util/Set.c"
) { ((tmp)->tree.rbe_left)->tree.rbe_parent = (parent); } do {} while (0); if (((tmp)->tree.rbe_parent = (parent)->tree.rbe_parent) != 
# 79 "util/Set.c" 3 4
((void *)0)
# 79 "util/Set.c"
) { if ((parent) == ((parent)->tree.rbe_parent)->tree.rbe_left) ((parent)->tree.rbe_parent)->tree.rbe_left = (tmp); else ((parent)->tree.rbe_parent)->tree.rbe_right = (tmp); } else (head)->rbh_root = (tmp); (tmp)->tree.rbe_left = (parent); (parent)->tree.rbe_parent = (tmp); do {} while (0); if (((tmp)->tree.rbe_parent)) do {} while (0); } while ( 0); tmp = (parent)->tree.rbe_right; } if (((tmp)->tree.rbe_left == 
# 79 "util/Set.c" 3 4
((void *)0) 
# 79 "util/Set.c"
|| ((tmp)->tree.rbe_left)->tree.rbe_color == 0) && ((tmp)->tree.rbe_right == 
# 79 "util/Set.c" 3 4
((void *)0) 
# 79 "util/Set.c"
|| ((tmp)->tree.rbe_right)->tree.rbe_color == 0)) { (tmp)->tree.rbe_color = 1; elm = parent; parent = (elm)->tree.rbe_parent; } else { if ((tmp)->tree.rbe_right == 
# 79 "util/Set.c" 3 4
((void *)0) 
# 79 "util/Set.c"
|| ((tmp)->tree.rbe_right)->tree.rbe_color == 0) { struct Entry *oleft; if ((oleft = (tmp)->tree.rbe_left) != 
# 79 "util/Set.c" 3 4
((void *)0)
# 79 "util/Set.c"
) (oleft)->tree.rbe_color = 0; (tmp)->tree.rbe_color = 1; do { (oleft) = (tmp)->tree.rbe_left; if (((tmp)->tree.rbe_left = (oleft)->tree.rbe_right) != 
# 79 "util/Set.c" 3 4
((void *)0)
# 79 "util/Set.c"
) { ((oleft)->tree.rbe_right)->tree.rbe_parent = (tmp); } do {} while (0); if (((oleft)->tree.rbe_parent = (tmp)->tree.rbe_parent) != 
# 79 "util/Set.c" 3 4
((void *)0)
# 79 "util/Set.c"
) { if ((tmp) == ((tmp)->tree.rbe_parent)->tree.rbe_left) ((tmp)->tree.rbe_parent)->tree.rbe_left = (oleft); else ((tmp)->tree.rbe_parent)->tree.rbe_right = (oleft); } else (head)->rbh_root = (oleft); (oleft)->tree.rbe_right = (tmp); (tmp)->tree.rbe_parent = (oleft); do {} while (0); if (((oleft)->tree.rbe_parent)) do {} while (0); } while ( 0); tmp = (parent)->tree.rbe_right; } (tmp)->tree.rbe_color = (parent)->tree.rbe_color; (parent)->tree.rbe_color = 0; if ((tmp)->tree.rbe_right) ((tmp)->tree.rbe_right)->tree.rbe_color = 0; do { (tmp) = (parent)->tree.rbe_right; if (((parent)->tree.rbe_right = (tmp)->tree.rbe_left) != 
# 79 "util/Set.c" 3 4
((void *)0)
# 79 "util/Set.c"
) { ((tmp)->tree.rbe_left)->tree.rbe_parent = (parent); } do {} while (0); if (((tmp)->tree.rbe_parent = (parent)->tree.rbe_parent) != 
# 79 "util/Set.c" 3 4
((void *)0)
# 79 "util/Set.c"
) { if ((parent) == ((parent)->tree.rbe_parent)->tree.rbe_left) ((parent)->tree.rbe_parent)->tree.rbe_left = (tmp); else ((parent)->tree.rbe_parent)->tree.rbe_right = (tmp); } else (head)->rbh_root = (tmp); (tmp)->tree.rbe_left = (parent); (parent)->tree.rbe_parent = (tmp); do {} while (0); if (((tmp)->tree.rbe_parent)) do {} while (0); } while ( 0); elm = (head)->rbh_root; break; } } else { tmp = (parent)->tree.rbe_left; if ((tmp)->tree.rbe_color == 1) { do { (tmp)->tree.rbe_color = 0; (parent)->tree.rbe_color = 1; } while ( 0); do { (tmp) = (parent)->tree.rbe_left; if (((parent)->tree.rbe_left = (tmp)->tree.rbe_right) != 
# 79 "util/Set.c" 3 4
((void *)0)
# 79 "util/Set.c"
) { ((tmp)->tree.rbe_right)->tree.rbe_parent = (parent); } do {} while (0); if (((tmp)->tree.rbe_parent = (parent)->tree.rbe_parent) != 
# 79 "util/Set.c" 3 4
((void *)0)
# 79 "util/Set.c"
) { if ((parent) == ((parent)->tree.rbe_parent)->tree.rbe_left) ((parent)->tree.rbe_parent)->tree.rbe_left = (tmp); else ((parent)->tree.rbe_parent)->tree.rbe_right = (tmp); } else (head)->rbh_root = (tmp); (tmp)->tree.rbe_right = (parent); (parent)->tree.rbe_parent = (tmp); do {} while (0); if (((tmp)->tree.rbe_parent)) do {} while (0); } while ( 0); tmp = (parent)->tree.rbe_left; } if (((tmp)->tree.rbe_left == 
# 79 "util/Set.c" 3 4
((void *)0) 
# 79 "util/Set.c"
|| ((tmp)->tree.rbe_left)->tree.rbe_color == 0) && ((tmp)->tree.rbe_right == 
# 79 "util/Set.c" 3 4
((void *)0) 
# 79 "util/Set.c"
|| ((tmp)->tree.rbe_right)->tree.rbe_color == 0)) { (tmp)->tree.rbe_color = 1; elm = parent; parent = (elm)->tree.rbe_parent; } else { if ((tmp)->tree.rbe_left == 
# 79 "util/Set.c" 3 4
((void *)0) 
# 79 "util/Set.c"
|| ((tmp)->tree.rbe_left)->tree.rbe_color == 0) { struct Entry *oright; if ((oright = (tmp)->tree.rbe_right) != 
# 79 "util/Set.c" 3 4
((void *)0)
# 79 "util/Set.c"
) (oright)->tree.rbe_color = 0; (tmp)->tree.rbe_color = 1; do { (oright) = (tmp)->tree.rbe_right; if (((tmp)->tree.rbe_right = (oright)->tree.rbe_left) != 
# 79 "util/Set.c" 3 4
((void *)0)
# 79 "util/Set.c"
) { ((oright)->tree.rbe_left)->tree.rbe_parent = (tmp); } do {} while (0); if (((oright)->tree.rbe_parent = (tmp)->tree.rbe_parent) != 
# 79 "util/Set.c" 3 4
((void *)0)
# 79 "util/Set.c"
) { if ((tmp) == ((tmp)->tree.rbe_parent)->tree.rbe_left) ((tmp)->tree.rbe_parent)->tree.rbe_left = (oright); else ((tmp)->tree.rbe_parent)->tree.rbe_right = (oright); } else (head)->rbh_root = (oright); (oright)->tree.rbe_left = (tmp); (tmp)->tree.rbe_parent = (oright); do {} while (0); if (((oright)->tree.rbe_parent)) do {} while (0); } while ( 0); tmp = (parent)->tree.rbe_left; } (tmp)->tree.rbe_color = (parent)->tree.rbe_color; (parent)->tree.rbe_color = 0; if ((tmp)->tree.rbe_left) ((tmp)->tree.rbe_left)->tree.rbe_color = 0; do { (tmp) = (parent)->tree.rbe_left; if (((parent)->tree.rbe_left = (tmp)->tree.rbe_right) != 
# 79 "util/Set.c" 3 4
((void *)0)
# 79 "util/Set.c"
) { ((tmp)->tree.rbe_right)->tree.rbe_parent = (parent); } do {} while (0); if (((tmp)->tree.rbe_parent = (parent)->tree.rbe_parent) != 
# 79 "util/Set.c" 3 4
((void *)0)
# 79 "util/Set.c"
) { if ((parent) == ((parent)->tree.rbe_parent)->tree.rbe_left) ((parent)->tree.rbe_parent)->tree.rbe_left = (tmp); else ((parent)->tree.rbe_parent)->tree.rbe_right = (tmp); } else (head)->rbh_root = (tmp); (tmp)->tree.rbe_right = (parent); (parent)->tree.rbe_parent = (tmp); do {} while (0); if (((tmp)->tree.rbe_parent)) do {} while (0); } while ( 0); elm = (head)->rbh_root; break; } } } if (elm) (elm)->tree.rbe_color = 0; } __attribute__((unused)) static struct Entry * ActiveTree_RB_REMOVE(struct ActiveTree *head, struct Entry *elm) { struct Entry *child, *parent, *old = elm; int color; if ((elm)->tree.rbe_left == 
# 79 "util/Set.c" 3 4
((void *)0)
# 79 "util/Set.c"
) child = (elm)->tree.rbe_right; else if ((elm)->tree.rbe_right == 
# 79 "util/Set.c" 3 4
((void *)0)
# 79 "util/Set.c"
) child = (elm)->tree.rbe_left; else { struct Entry *left; elm = (elm)->tree.rbe_right; while ((left = (elm)->tree.rbe_left) != 
# 79 "util/Set.c" 3 4
((void *)0)
# 79 "util/Set.c"
) elm = left; child = (elm)->tree.rbe_right; parent = (elm)->tree.rbe_parent; color = (elm)->tree.rbe_color; if (child) (child)->tree.rbe_parent = parent; if (parent) { if ((parent)->tree.rbe_left == elm) (parent)->tree.rbe_left = child; else (parent)->tree.rbe_right = child; do {} while (0); } else (head)->rbh_root = child; if ((elm)->tree.rbe_parent == old) parent = elm; (elm)->tree = (old)->tree; if ((old)->tree.rbe_parent) { if (((old)->tree.rbe_parent)->tree.rbe_left == old) ((old)->tree.rbe_parent)->tree.rbe_left = elm; else ((old)->tree.rbe_parent)->tree.rbe_right = elm; do {} while (0); } else (head)->rbh_root = elm; ((old)->tree.rbe_left)->tree.rbe_parent = elm; if ((old)->tree.rbe_right) ((old)->tree.rbe_right)->tree.rbe_parent = elm; if (parent) { left = parent; do { do {} while (0); } while ((left = (left)->tree.rbe_parent) != 
# 79 "util/Set.c" 3 4
((void *)0)
# 79 "util/Set.c"
); } goto color; } parent = (elm)->tree.rbe_parent; color = (elm)->tree.rbe_color; if (child) (child)->tree.rbe_parent = parent; if (parent) { if ((parent)->tree.rbe_left == elm) (parent)->tree.rbe_left = child; else (parent)->tree.rbe_right = child; do {} while (0); } else (head)->rbh_root = child; color: if (color == 0) ActiveTree_RB_REMOVE_COLOR(head, parent, child); return (old); } __attribute__((unused)) static struct Entry * ActiveTree_RB_INSERT(struct ActiveTree *head, struct Entry *elm) { struct Entry *tmp; struct Entry *parent = 
# 79 "util/Set.c" 3 4
((void *)0)
# 79 "util/Set.c"
; int comp = 0; tmp = (head)->rbh_root; while (tmp) { parent = tmp; comp = (compare)(elm, parent); if (comp < 0) tmp = (tmp)->tree.rbe_left; else if (comp > 0) tmp = (tmp)->tree.rbe_right; else return (tmp); } do { (elm)->tree.rbe_parent = parent; (elm)->tree.rbe_left = (elm)->tree.rbe_right = 
# 79 "util/Set.c" 3 4
((void *)0)
# 79 "util/Set.c"
; (elm)->tree.rbe_color = 1; } while ( 0); if (parent != 
# 79 "util/Set.c" 3 4
((void *)0)
# 79 "util/Set.c"
) { if (comp < 0) (parent)->tree.rbe_left = elm; else (parent)->tree.rbe_right = elm; do {} while (0); } else (head)->rbh_root = elm; ActiveTree_RB_INSERT_COLOR(head, elm); return (
# 79 "util/Set.c" 3 4
((void *)0)
# 79 "util/Set.c"
); } __attribute__((unused)) static struct Entry * ActiveTree_RB_FIND(struct ActiveTree *head, struct Entry *elm) { struct Entry *tmp = (head)->rbh_root; int comp; while (tmp) { comp = compare(elm, tmp); if (comp < 0) tmp = (tmp)->tree.rbe_left; else if (comp > 0) tmp = (tmp)->tree.rbe_right; else return (tmp); } return (
# 79 "util/Set.c" 3 4
((void *)0)
# 79 "util/Set.c"
); } __attribute__((unused)) static struct Entry * ActiveTree_RB_NFIND(struct ActiveTree *head, struct Entry *elm) { struct Entry *tmp = (head)->rbh_root; struct Entry *res = 
# 79 "util/Set.c" 3 4
((void *)0)
# 79 "util/Set.c"
; int comp; while (tmp) { comp = compare(elm, tmp); if (comp < 0) { res = tmp; tmp = (tmp)->tree.rbe_left; } else if (comp > 0) tmp = (tmp)->tree.rbe_right; else return (tmp); } return (res); } __attribute__((unused)) static struct Entry * ActiveTree_RB_NEXT(struct Entry *elm) { if ((elm)->tree.rbe_right) { elm = (elm)->tree.rbe_right; while ((elm)->tree.rbe_left) elm = (elm)->tree.rbe_left; } else { if ((elm)->tree.rbe_parent && (elm == ((elm)->tree.rbe_parent)->tree.rbe_left)) elm = (elm)->tree.rbe_parent; else { while ((elm)->tree.rbe_parent && (elm == ((elm)->tree.rbe_parent)->tree.rbe_right)) elm = (elm)->tree.rbe_parent; elm = (elm)->tree.rbe_parent; } } return (elm); } __attribute__((unused)) static struct Entry * ActiveTree_RB_PREV(struct Entry *elm) { if ((elm)->tree.rbe_left) { elm = (elm)->tree.rbe_left; while ((elm)->tree.rbe_right) elm = (elm)->tree.rbe_right; } else { if ((elm)->tree.rbe_parent && (elm == ((elm)->tree.rbe_parent)->tree.rbe_right)) elm = (elm)->tree.rbe_parent; else { while ((elm)->tree.rbe_parent && (elm == ((elm)->tree.rbe_parent)->tree.rbe_left)) elm = (elm)->tree.rbe_parent; elm = (elm)->tree.rbe_parent; } } return (elm); } __attribute__((unused)) static struct Entry * ActiveTree_RB_MINMAX(struct ActiveTree *head, int val) { struct Entry *tmp = (head)->rbh_root; struct Entry *parent = 
# 79 "util/Set.c" 3 4
((void *)0)
# 79 "util/Set.c"
; while (tmp) { parent = tmp; if (val < 0) tmp = (tmp)->tree.rbe_left; else tmp = (tmp)->tree.rbe_right; } return (parent); }

static int freeCompare(const struct Entry* a, const struct Entry* b)
{
    return a->hashCode - b->hashCode;
}
__attribute__((unused)) static void FreeTree_RB_INSERT_COLOR(struct FreeTree *head, struct Entry *elm) { struct Entry *parent, *gparent, *tmp; while ((parent = (elm)->tree.rbe_parent) != 
# 85 "util/Set.c" 3 4
((void *)0) 
# 85 "util/Set.c"
&& (parent)->tree.rbe_color == 1) { gparent = (parent)->tree.rbe_parent; if (parent == (gparent)->tree.rbe_left) { tmp = (gparent)->tree.rbe_right; if (tmp && (tmp)->tree.rbe_color == 1) { (tmp)->tree.rbe_color = 0; do { (parent)->tree.rbe_color = 0; (gparent)->tree.rbe_color = 1; } while ( 0); elm = gparent; continue; } if ((parent)->tree.rbe_right == elm) { do { (tmp) = (parent)->tree.rbe_right; if (((parent)->tree.rbe_right = (tmp)->tree.rbe_left) != 
# 85 "util/Set.c" 3 4
((void *)0)
# 85 "util/Set.c"
) { ((tmp)->tree.rbe_left)->tree.rbe_parent = (parent); } do {} while (0); if (((tmp)->tree.rbe_parent = (parent)->tree.rbe_parent) != 
# 85 "util/Set.c" 3 4
((void *)0)
# 85 "util/Set.c"
) { if ((parent) == ((parent)->tree.rbe_parent)->tree.rbe_left) ((parent)->tree.rbe_parent)->tree.rbe_left = (tmp); else ((parent)->tree.rbe_parent)->tree.rbe_right = (tmp); } else (head)->rbh_root = (tmp); (tmp)->tree.rbe_left = (parent); (parent)->tree.rbe_parent = (tmp); do {} while (0); if (((tmp)->tree.rbe_parent)) do {} while (0); } while ( 0); tmp = parent; parent = elm; elm = tmp; } do { (parent)->tree.rbe_color = 0; (gparent)->tree.rbe_color = 1; } while ( 0); do { (tmp) = (gparent)->tree.rbe_left; if (((gparent)->tree.rbe_left = (tmp)->tree.rbe_right) != 
# 85 "util/Set.c" 3 4
((void *)0)
# 85 "util/Set.c"
) { ((tmp)->tree.rbe_right)->tree.rbe_parent = (gparent); } do {} while (0); if (((tmp)->tree.rbe_parent = (gparent)->tree.rbe_parent) != 
# 85 "util/Set.c" 3 4
((void *)0)
# 85 "util/Set.c"
) { if ((gparent) == ((gparent)->tree.rbe_parent)->tree.rbe_left) ((gparent)->tree.rbe_parent)->tree.rbe_left = (tmp); else ((gparent)->tree.rbe_parent)->tree.rbe_right = (tmp); } else (head)->rbh_root = (tmp); (tmp)->tree.rbe_right = (gparent); (gparent)->tree.rbe_parent = (tmp); do {} while (0); if (((tmp)->tree.rbe_parent)) do {} while (0); } while ( 0); } else { tmp = (gparent)->tree.rbe_left; if (tmp && (tmp)->tree.rbe_color == 1) { (tmp)->tree.rbe_color = 0; do { (parent)->tree.rbe_color = 0; (gparent)->tree.rbe_color = 1; } while ( 0); elm = gparent; continue; } if ((parent)->tree.rbe_left == elm) { do { (tmp) = (parent)->tree.rbe_left; if (((parent)->tree.rbe_left = (tmp)->tree.rbe_right) != 
# 85 "util/Set.c" 3 4
((void *)0)
# 85 "util/Set.c"
) { ((tmp)->tree.rbe_right)->tree.rbe_parent = (parent); } do {} while (0); if (((tmp)->tree.rbe_parent = (parent)->tree.rbe_parent) != 
# 85 "util/Set.c" 3 4
((void *)0)
# 85 "util/Set.c"
) { if ((parent) == ((parent)->tree.rbe_parent)->tree.rbe_left) ((parent)->tree.rbe_parent)->tree.rbe_left = (tmp); else ((parent)->tree.rbe_parent)->tree.rbe_right = (tmp); } else (head)->rbh_root = (tmp); (tmp)->tree.rbe_right = (parent); (parent)->tree.rbe_parent = (tmp); do {} while (0); if (((tmp)->tree.rbe_parent)) do {} while (0); } while ( 0); tmp = parent; parent = elm; elm = tmp; } do { (parent)->tree.rbe_color = 0; (gparent)->tree.rbe_color = 1; } while ( 0); do { (tmp) = (gparent)->tree.rbe_right; if (((gparent)->tree.rbe_right = (tmp)->tree.rbe_left) != 
# 85 "util/Set.c" 3 4
((void *)0)
# 85 "util/Set.c"
) { ((tmp)->tree.rbe_left)->tree.rbe_parent = (gparent); } do {} while (0); if (((tmp)->tree.rbe_parent = (gparent)->tree.rbe_parent) != 
# 85 "util/Set.c" 3 4
((void *)0)
# 85 "util/Set.c"
) { if ((gparent) == ((gparent)->tree.rbe_parent)->tree.rbe_left) ((gparent)->tree.rbe_parent)->tree.rbe_left = (tmp); else ((gparent)->tree.rbe_parent)->tree.rbe_right = (tmp); } else (head)->rbh_root = (tmp); (tmp)->tree.rbe_left = (gparent); (gparent)->tree.rbe_parent = (tmp); do {} while (0); if (((tmp)->tree.rbe_parent)) do {} while (0); } while ( 0); } } (head->rbh_root)->tree.rbe_color = 0; } __attribute__((unused)) static void FreeTree_RB_REMOVE_COLOR(struct FreeTree *head, struct Entry *parent, struct Entry *elm) { struct Entry *tmp; while ((elm == 
# 85 "util/Set.c" 3 4
((void *)0) 
# 85 "util/Set.c"
|| (elm)->tree.rbe_color == 0) && elm != (head)->rbh_root) { if ((parent)->tree.rbe_left == elm) { tmp = (parent)->tree.rbe_right; if ((tmp)->tree.rbe_color == 1) { do { (tmp)->tree.rbe_color = 0; (parent)->tree.rbe_color = 1; } while ( 0); do { (tmp) = (parent)->tree.rbe_right; if (((parent)->tree.rbe_right = (tmp)->tree.rbe_left) != 
# 85 "util/Set.c" 3 4
((void *)0)
# 85 "util/Set.c"
) { ((tmp)->tree.rbe_left)->tree.rbe_parent = (parent); } do {} while (0); if (((tmp)->tree.rbe_parent = (parent)->tree.rbe_parent) != 
# 85 "util/Set.c" 3 4
((void *)0)
# 85 "util/Set.c"
) { if ((parent) == ((parent)->tree.rbe_parent)->tree.rbe_left) ((parent)->tree.rbe_parent)->tree.rbe_left = (tmp); else ((parent)->tree.rbe_parent)->tree.rbe_right = (tmp); } else (head)->rbh_root = (tmp); (tmp)->tree.rbe_left = (parent); (parent)->tree.rbe_parent = (tmp); do {} while (0); if (((tmp)->tree.rbe_parent)) do {} while (0); } while ( 0); tmp = (parent)->tree.rbe_right; } if (((tmp)->tree.rbe_left == 
# 85 "util/Set.c" 3 4
((void *)0) 
# 85 "util/Set.c"
|| ((tmp)->tree.rbe_left)->tree.rbe_color == 0) && ((tmp)->tree.rbe_right == 
# 85 "util/Set.c" 3 4
((void *)0) 
# 85 "util/Set.c"
|| ((tmp)->tree.rbe_right)->tree.rbe_color == 0)) { (tmp)->tree.rbe_color = 1; elm = parent; parent = (elm)->tree.rbe_parent; } else { if ((tmp)->tree.rbe_right == 
# 85 "util/Set.c" 3 4
((void *)0) 
# 85 "util/Set.c"
|| ((tmp)->tree.rbe_right)->tree.rbe_color == 0) { struct Entry *oleft; if ((oleft = (tmp)->tree.rbe_left) != 
# 85 "util/Set.c" 3 4
((void *)0)
# 85 "util/Set.c"
) (oleft)->tree.rbe_color = 0; (tmp)->tree.rbe_color = 1; do { (oleft) = (tmp)->tree.rbe_left; if (((tmp)->tree.rbe_left = (oleft)->tree.rbe_right) != 
# 85 "util/Set.c" 3 4
((void *)0)
# 85 "util/Set.c"
) { ((oleft)->tree.rbe_right)->tree.rbe_parent = (tmp); } do {} while (0); if (((oleft)->tree.rbe_parent = (tmp)->tree.rbe_parent) != 
# 85 "util/Set.c" 3 4
((void *)0)
# 85 "util/Set.c"
) { if ((tmp) == ((tmp)->tree.rbe_parent)->tree.rbe_left) ((tmp)->tree.rbe_parent)->tree.rbe_left = (oleft); else ((tmp)->tree.rbe_parent)->tree.rbe_right = (oleft); } else (head)->rbh_root = (oleft); (oleft)->tree.rbe_right = (tmp); (tmp)->tree.rbe_parent = (oleft); do {} while (0); if (((oleft)->tree.rbe_parent)) do {} while (0); } while ( 0); tmp = (parent)->tree.rbe_right; } (tmp)->tree.rbe_color = (parent)->tree.rbe_color; (parent)->tree.rbe_color = 0; if ((tmp)->tree.rbe_right) ((tmp)->tree.rbe_right)->tree.rbe_color = 0; do { (tmp) = (parent)->tree.rbe_right; if (((parent)->tree.rbe_right = (tmp)->tree.rbe_left) != 
# 85 "util/Set.c" 3 4
((void *)0)
# 85 "util/Set.c"
) { ((tmp)->tree.rbe_left)->tree.rbe_parent = (parent); } do {} while (0); if (((tmp)->tree.rbe_parent = (parent)->tree.rbe_parent) != 
# 85 "util/Set.c" 3 4
((void *)0)
# 85 "util/Set.c"
) { if ((parent) == ((parent)->tree.rbe_parent)->tree.rbe_left) ((parent)->tree.rbe_parent)->tree.rbe_left = (tmp); else ((parent)->tree.rbe_parent)->tree.rbe_right = (tmp); } else (head)->rbh_root = (tmp); (tmp)->tree.rbe_left = (parent); (parent)->tree.rbe_parent = (tmp); do {} while (0); if (((tmp)->tree.rbe_parent)) do {} while (0); } while ( 0); elm = (head)->rbh_root; break; } } else { tmp = (parent)->tree.rbe_left; if ((tmp)->tree.rbe_color == 1) { do { (tmp)->tree.rbe_color = 0; (parent)->tree.rbe_color = 1; } while ( 0); do { (tmp) = (parent)->tree.rbe_left; if (((parent)->tree.rbe_left = (tmp)->tree.rbe_right) != 
# 85 "util/Set.c" 3 4
((void *)0)
# 85 "util/Set.c"
) { ((tmp)->tree.rbe_right)->tree.rbe_parent = (parent); } do {} while (0); if (((tmp)->tree.rbe_parent = (parent)->tree.rbe_parent) != 
# 85 "util/Set.c" 3 4
((void *)0)
# 85 "util/Set.c"
) { if ((parent) == ((parent)->tree.rbe_parent)->tree.rbe_left) ((parent)->tree.rbe_parent)->tree.rbe_left = (tmp); else ((parent)->tree.rbe_parent)->tree.rbe_right = (tmp); } else (head)->rbh_root = (tmp); (tmp)->tree.rbe_right = (parent); (parent)->tree.rbe_parent = (tmp); do {} while (0); if (((tmp)->tree.rbe_parent)) do {} while (0); } while ( 0); tmp = (parent)->tree.rbe_left; } if (((tmp)->tree.rbe_left == 
# 85 "util/Set.c" 3 4
((void *)0) 
# 85 "util/Set.c"
|| ((tmp)->tree.rbe_left)->tree.rbe_color == 0) && ((tmp)->tree.rbe_right == 
# 85 "util/Set.c" 3 4
((void *)0) 
# 85 "util/Set.c"
|| ((tmp)->tree.rbe_right)->tree.rbe_color == 0)) { (tmp)->tree.rbe_color = 1; elm = parent; parent = (elm)->tree.rbe_parent; } else { if ((tmp)->tree.rbe_left == 
# 85 "util/Set.c" 3 4
((void *)0) 
# 85 "util/Set.c"
|| ((tmp)->tree.rbe_left)->tree.rbe_color == 0) { struct Entry *oright; if ((oright = (tmp)->tree.rbe_right) != 
# 85 "util/Set.c" 3 4
((void *)0)
# 85 "util/Set.c"
) (oright)->tree.rbe_color = 0; (tmp)->tree.rbe_color = 1; do { (oright) = (tmp)->tree.rbe_right; if (((tmp)->tree.rbe_right = (oright)->tree.rbe_left) != 
# 85 "util/Set.c" 3 4
((void *)0)
# 85 "util/Set.c"
) { ((oright)->tree.rbe_left)->tree.rbe_parent = (tmp); } do {} while (0); if (((oright)->tree.rbe_parent = (tmp)->tree.rbe_parent) != 
# 85 "util/Set.c" 3 4
((void *)0)
# 85 "util/Set.c"
) { if ((tmp) == ((tmp)->tree.rbe_parent)->tree.rbe_left) ((tmp)->tree.rbe_parent)->tree.rbe_left = (oright); else ((tmp)->tree.rbe_parent)->tree.rbe_right = (oright); } else (head)->rbh_root = (oright); (oright)->tree.rbe_left = (tmp); (tmp)->tree.rbe_parent = (oright); do {} while (0); if (((oright)->tree.rbe_parent)) do {} while (0); } while ( 0); tmp = (parent)->tree.rbe_left; } (tmp)->tree.rbe_color = (parent)->tree.rbe_color; (parent)->tree.rbe_color = 0; if ((tmp)->tree.rbe_left) ((tmp)->tree.rbe_left)->tree.rbe_color = 0; do { (tmp) = (parent)->tree.rbe_left; if (((parent)->tree.rbe_left = (tmp)->tree.rbe_right) != 
# 85 "util/Set.c" 3 4
((void *)0)
# 85 "util/Set.c"
) { ((tmp)->tree.rbe_right)->tree.rbe_parent = (parent); } do {} while (0); if (((tmp)->tree.rbe_parent = (parent)->tree.rbe_parent) != 
# 85 "util/Set.c" 3 4
((void *)0)
# 85 "util/Set.c"
) { if ((parent) == ((parent)->tree.rbe_parent)->tree.rbe_left) ((parent)->tree.rbe_parent)->tree.rbe_left = (tmp); else ((parent)->tree.rbe_parent)->tree.rbe_right = (tmp); } else (head)->rbh_root = (tmp); (tmp)->tree.rbe_right = (parent); (parent)->tree.rbe_parent = (tmp); do {} while (0); if (((tmp)->tree.rbe_parent)) do {} while (0); } while ( 0); elm = (head)->rbh_root; break; } } } if (elm) (elm)->tree.rbe_color = 0; } __attribute__((unused)) static struct Entry * FreeTree_RB_REMOVE(struct FreeTree *head, struct Entry *elm) { struct Entry *child, *parent, *old = elm; int color; if ((elm)->tree.rbe_left == 
# 85 "util/Set.c" 3 4
((void *)0)
# 85 "util/Set.c"
) child = (elm)->tree.rbe_right; else if ((elm)->tree.rbe_right == 
# 85 "util/Set.c" 3 4
((void *)0)
# 85 "util/Set.c"
) child = (elm)->tree.rbe_left; else { struct Entry *left; elm = (elm)->tree.rbe_right; while ((left = (elm)->tree.rbe_left) != 
# 85 "util/Set.c" 3 4
((void *)0)
# 85 "util/Set.c"
) elm = left; child = (elm)->tree.rbe_right; parent = (elm)->tree.rbe_parent; color = (elm)->tree.rbe_color; if (child) (child)->tree.rbe_parent = parent; if (parent) { if ((parent)->tree.rbe_left == elm) (parent)->tree.rbe_left = child; else (parent)->tree.rbe_right = child; do {} while (0); } else (head)->rbh_root = child; if ((elm)->tree.rbe_parent == old) parent = elm; (elm)->tree = (old)->tree; if ((old)->tree.rbe_parent) { if (((old)->tree.rbe_parent)->tree.rbe_left == old) ((old)->tree.rbe_parent)->tree.rbe_left = elm; else ((old)->tree.rbe_parent)->tree.rbe_right = elm; do {} while (0); } else (head)->rbh_root = elm; ((old)->tree.rbe_left)->tree.rbe_parent = elm; if ((old)->tree.rbe_right) ((old)->tree.rbe_right)->tree.rbe_parent = elm; if (parent) { left = parent; do { do {} while (0); } while ((left = (left)->tree.rbe_parent) != 
# 85 "util/Set.c" 3 4
((void *)0)
# 85 "util/Set.c"
); } goto color; } parent = (elm)->tree.rbe_parent; color = (elm)->tree.rbe_color; if (child) (child)->tree.rbe_parent = parent; if (parent) { if ((parent)->tree.rbe_left == elm) (parent)->tree.rbe_left = child; else (parent)->tree.rbe_right = child; do {} while (0); } else (head)->rbh_root = child; color: if (color == 0) FreeTree_RB_REMOVE_COLOR(head, parent, child); return (old); } __attribute__((unused)) static struct Entry * FreeTree_RB_INSERT(struct FreeTree *head, struct Entry *elm) { struct Entry *tmp; struct Entry *parent = 
# 85 "util/Set.c" 3 4
((void *)0)
# 85 "util/Set.c"
; int comp = 0; tmp = (head)->rbh_root; while (tmp) { parent = tmp; comp = (freeCompare)(elm, parent); if (comp < 0) tmp = (tmp)->tree.rbe_left; else if (comp > 0) tmp = (tmp)->tree.rbe_right; else return (tmp); } do { (elm)->tree.rbe_parent = parent; (elm)->tree.rbe_left = (elm)->tree.rbe_right = 
# 85 "util/Set.c" 3 4
((void *)0)
# 85 "util/Set.c"
; (elm)->tree.rbe_color = 1; } while ( 0); if (parent != 
# 85 "util/Set.c" 3 4
((void *)0)
# 85 "util/Set.c"
) { if (comp < 0) (parent)->tree.rbe_left = elm; else (parent)->tree.rbe_right = elm; do {} while (0); } else (head)->rbh_root = elm; FreeTree_RB_INSERT_COLOR(head, elm); return (
# 85 "util/Set.c" 3 4
((void *)0)
# 85 "util/Set.c"
); } __attribute__((unused)) static struct Entry * FreeTree_RB_FIND(struct FreeTree *head, struct Entry *elm) { struct Entry *tmp = (head)->rbh_root; int comp; while (tmp) { comp = freeCompare(elm, tmp); if (comp < 0) tmp = (tmp)->tree.rbe_left; else if (comp > 0) tmp = (tmp)->tree.rbe_right; else return (tmp); } return (
# 85 "util/Set.c" 3 4
((void *)0)
# 85 "util/Set.c"
); } __attribute__((unused)) static struct Entry * FreeTree_RB_NFIND(struct FreeTree *head, struct Entry *elm) { struct Entry *tmp = (head)->rbh_root; struct Entry *res = 
# 85 "util/Set.c" 3 4
((void *)0)
# 85 "util/Set.c"
; int comp; while (tmp) { comp = freeCompare(elm, tmp); if (comp < 0) { res = tmp; tmp = (tmp)->tree.rbe_left; } else if (comp > 0) tmp = (tmp)->tree.rbe_right; else return (tmp); } return (res); } __attribute__((unused)) static struct Entry * FreeTree_RB_NEXT(struct Entry *elm) { if ((elm)->tree.rbe_right) { elm = (elm)->tree.rbe_right; while ((elm)->tree.rbe_left) elm = (elm)->tree.rbe_left; } else { if ((elm)->tree.rbe_parent && (elm == ((elm)->tree.rbe_parent)->tree.rbe_left)) elm = (elm)->tree.rbe_parent; else { while ((elm)->tree.rbe_parent && (elm == ((elm)->tree.rbe_parent)->tree.rbe_right)) elm = (elm)->tree.rbe_parent; elm = (elm)->tree.rbe_parent; } } return (elm); } __attribute__((unused)) static struct Entry * FreeTree_RB_PREV(struct Entry *elm) { if ((elm)->tree.rbe_left) { elm = (elm)->tree.rbe_left; while ((elm)->tree.rbe_right) elm = (elm)->tree.rbe_right; } else { if ((elm)->tree.rbe_parent && (elm == ((elm)->tree.rbe_parent)->tree.rbe_right)) elm = (elm)->tree.rbe_parent; else { while ((elm)->tree.rbe_parent && (elm == ((elm)->tree.rbe_parent)->tree.rbe_left)) elm = (elm)->tree.rbe_parent; elm = (elm)->tree.rbe_parent; } } return (elm); } __attribute__((unused)) static struct Entry * FreeTree_RB_MINMAX(struct FreeTree *head, int val) { struct Entry *tmp = (head)->rbh_root; struct Entry *parent = 
# 85 "util/Set.c" 3 4
((void *)0)
# 85 "util/Set.c"
; while (tmp) { parent = tmp; if (val < 0) tmp = (tmp)->tree.rbe_left; else tmp = (tmp)->tree.rbe_right; } return (parent); }

static struct Block* blockForEntry(const struct Set_pvt* set, const struct Entry* e)
{
    struct Block* b = set->block;
    uintptr_t ep = (uintptr_t) e;
    while (b) {
        if (ep >= (uintptr_t)&b->entries && ep < (uintptr_t)&b->entries[8]) { return b; }
        b = b->next;
    }
    Assert_failure("No matching block for entry");
}

static struct Entry* allocateBlock(struct Set_pvt* set)
{
    struct Allocator* alloc = Allocator__child((set->alloc),"Set.c",100);
    struct Block* newBlock = Allocator__calloc((alloc),(sizeof(struct Block)),(1),"Set.c",101);
    newBlock->alloc = alloc;
    newBlock->next = set->block;
    set->block = newBlock;
    uint32_t num = newBlock->number = (set->block ? set->block->number : -1) + 1;
    for (int i = 0; i < 8; i++) {
        newBlock->entries[i].set = set;
        newBlock->entries[i].hashCode = num;
        FreeTree_RB_INSERT(&set->freeTree, &newBlock->entries[i]);
    }
    return &newBlock->entries[0];
}

static struct Entry* newEntry(struct Set_pvt* set)
{
    struct Entry* e = FreeTree_RB_MINMAX(&set->freeTree, -1);
    if (!e) { e = allocateBlock(set); }
    FreeTree_RB_REMOVE(&set->freeTree, e);
    struct Block* b = blockForEntry(set, e);
    b->usedCount++;
    set->size++;
    return e;
}

static void freeBlock(struct Set_pvt* set, struct Block* b)
{
    for (int i = 0; i < 8; i++) {
        FreeTree_RB_REMOVE(&set->freeTree, &b->entries[i]);
    }
    int ok = 0;
    for (struct Block** bp = &set->block; *bp; bp = &(*bp)->next) {
        if (*bp == b) {
            *bp = b->next;
            ok = 1;
            break;
        }
    }
    do { if (!((ok && "INTERNAL: block was not linked into linked list"))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("Set.c"), (138), "(ok && \"INTERNAL: block was not linked into linked list\")"); } } while (0);
    Allocator__free((b->alloc),"Set.c",139);
}

static void freeEntry(struct Set_pvt* set, struct Entry* e)
{
    struct Block* b = blockForEntry(set, e);
    ActiveTree_RB_REMOVE(&set->activeTree, e);
    e->data = 
# 146 "util/Set.c" 3 4
             ((void *)0)
# 146 "util/Set.c"
                 ;
    e->hashCode = b->number;
    if (e->alloc) {
        Allocator__free((e->alloc),"Set.c",149);
        e->alloc = 
# 150 "util/Set.c" 3 4
                  ((void *)0)
# 150 "util/Set.c"
                      ;
    }
    FreeTree_RB_INSERT(&set->freeTree, e);
    b->usedCount--;
    set->size--;
    if (!b->usedCount) { freeBlock(set, b); }
}

static struct Entry* get(struct Set_pvt* set, void* val, uint32_t hashCode)
{
    struct Entry e = {
        .hashCode = hashCode,
        .data = val,
        .set = set
    };
    return ActiveTree_RB_FIND(&set->activeTree, &e);
}

int Set_addCopy(struct Set* _set, void* val, uint32_t size)
{
    struct Set_pvt* set = (__extension__ ({ __typeof__((struct Set_pvt*) _set) Identity_ptr = ((struct Set_pvt*) _set); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x4ec76200a73e652cull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("Set.c"), (170), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x4ec76200a73e652cull))"); } } while (0); Identity_ptr; }));
    uint32_t hashCode = set->hashCode(val);
    struct Entry* e = get(set, val, hashCode);
    if (!e) {
        struct Entry* e = newEntry(set);
        e->hashCode = hashCode;
        ActiveTree_RB_INSERT(&set->activeTree, e);
        struct Block* b = blockForEntry(set, e);
        e->alloc = Allocator__child((b->alloc),"Set.c",178);
        e->data = Allocator__malloc((e->alloc),(size),"Set.c",179);
        Bits__memcpy(e->data,val,size,"Set.c",180);
    }
    return set->size;
}

int Set_add(struct Set* _set, void* val)
{
    struct Set_pvt* set = (__extension__ ({ __typeof__((struct Set_pvt*) _set) Identity_ptr = ((struct Set_pvt*) _set); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x4ec76200a73e652cull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("Set.c"), (187), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x4ec76200a73e652cull))"); } } while (0); Identity_ptr; }));
    uint32_t hashCode = set->hashCode(val);
    struct Entry* e = get(set, val, hashCode);
    if (!e) {
        struct Entry* e = newEntry(set);
        e->data = val;
        e->hashCode = hashCode;
        ActiveTree_RB_INSERT(&set->activeTree, e);
    }
    return set->size;
}

void Set_iter(struct Set* _set, struct Set_Iter* iter)
{
    struct Set_pvt* set = (__extension__ ({ __typeof__((struct Set_pvt*) _set) Identity_ptr = ((struct Set_pvt*) _set); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x4ec76200a73e652cull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("Set.c"), (201), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x4ec76200a73e652cull))"); } } while (0); Identity_ptr; }));
    struct Entry* e = iter->entry = ActiveTree_RB_MINMAX(&set->activeTree, -1);
    if (e) {
        iter->val = e->data;
        iter->entry = ActiveTree_RB_NEXT(e);
    } else {
        iter->val = 
# 207 "util/Set.c" 3 4
                   ((void *)0)
# 207 "util/Set.c"
                       ;
    }
}

void Set_iterNext(struct Set_Iter* iter)
{
    struct Entry* e = iter->entry;
    if (e) {
        iter->val = e->data;
        iter->entry = ActiveTree_RB_NEXT(e);
    } else {
        iter->val = 
# 218 "util/Set.c" 3 4
                   ((void *)0)
# 218 "util/Set.c"
                       ;
    }
}

void* Set_remove(struct Set* _set, void* val)
{
    struct Set_pvt* set = (__extension__ ({ __typeof__((struct Set_pvt*) _set) Identity_ptr = ((struct Set_pvt*) _set); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x4ec76200a73e652cull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("Set.c"), (224), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x4ec76200a73e652cull))"); } } while (0); Identity_ptr; }));
    struct Entry* e = get(set, val, set->hashCode(val));
    void* out = 
# 226 "util/Set.c" 3 4
               ((void *)0)
# 226 "util/Set.c"
                   ;
    if (e) {
        out = e->data;
        freeEntry(set, e);
    }
    return out;
}

void* Set_get(struct Set* _set, void* val)
{
    struct Set_pvt* set = (__extension__ ({ __typeof__((struct Set_pvt*) _set) Identity_ptr = ((struct Set_pvt*) _set); do { if (!((Identity_ptr->Identity_verifier == ((unsigned long) 0x4ec76200a73e652cull)))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("Set.c"), (236), "(Identity_ptr->Identity_verifier == ((unsigned long) 0x4ec76200a73e652cull))"); } } while (0); Identity_ptr; }));
    struct Entry* e = get(set, val, set->hashCode(val));
    void* out = 
# 238 "util/Set.c" 3 4
               ((void *)0)
# 238 "util/Set.c"
                   ;
    if (e) { out = e->data; }
    return out;
}

struct Set* Set_new(struct Allocator* alloc, Set_HashCode_t hashCode, Set_Compare_t compare)
{
    struct Set_pvt* out = Allocator__calloc((alloc),(sizeof(struct Set_pvt)),(1),"Set.c",245);
    (out)->Identity_verifier = ((unsigned long) 0x4ec76200a73e652cull);
    out->alloc = alloc;
    out->compare = compare;
    out->hashCode = hashCode;
    return (struct Set*) out;
}
