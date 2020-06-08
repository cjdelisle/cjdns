# 1 "util/log/WriterLog.c"
# 1 "/home/jojapoppa/cjdns//"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "util/log/WriterLog.c"
# 16 "util/log/WriterLog.c"
# 1 "./util/log/WriterLog.h" 1
# 18 "./util/log/WriterLog.h"
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
struct Linker_x010590401649420023;
# 33 "./util/Assert.h"
__attribute__((__format__ (__printf__, 1, 2)))
__attribute__((__noreturn__))
void Assert_failure(const char* format, ...);
# 19 "./util/Identity.h" 2
# 1 "./util/Constant.h" 1
# 33 "./util/Constant.h"

# 20 "./util/Identity.h" 2



    
# 19 "./memory/Allocator.h" 2


struct Linker_x03505172136167778;





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
# 19 "./util/log/WriterLog.h" 2
# 1 "./io/Writer.h" 1
# 18 "./io/Writer.h"
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
# 19 "./io/Writer.h" 2





# 23 "./io/Writer.h"
struct Writer {
# 35 "./io/Writer.h"
    int (* const write)(struct Writer* w, const void* toWrite, unsigned long length);

    uint64_t bytesWritten;
};
# 47 "./io/Writer.h"
static inline int Writer_write8 (struct Writer* writer, uint8_t number) { uint8_t num = number; return writer->write(writer, &num, 8/8); }
static inline int Writer_write16 (struct Writer* writer, uint16_t number) { uint16_t num = number; return writer->write(writer, &num, 16/8); }
static inline int Writer_write32 (struct Writer* writer, uint32_t number) { uint32_t num = number; return writer->write(writer, &num, 32/8); }
static inline int Writer_write64 (struct Writer* writer, uint64_t number) { uint64_t num = number; return writer->write(writer, &num, 64/8); }
# 20 "./util/log/WriterLog.h" 2
# 1 "./util/log/Log.h" 1
# 20 "./util/log/Log.h"
struct Linker_x07646585080229249;

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
# 21 "./util/log/WriterLog.h" 2

struct Linker_x07802583125669451;

struct Log* WriterLog_new(struct Writer* w, struct Allocator* alloc);
# 17 "util/log/WriterLog.c" 2
# 1 "./util/log/Log_impl.h" 1
# 20 "./util/log/Log_impl.h"
# 1 "/usr/lib/gcc/x86_64-w64-mingw32/9.2-win32/include/stdarg.h" 1 3 4
# 1 "/usr/share/mingw-w64/include/stdarg.h" 1 3 4
# 140 "/usr/share/mingw-w64/include/stdarg.h" 3 4
# 1 "/usr/share/mingw-w64/include/_mingw_stdarg.h" 1 3 4
# 141 "/usr/share/mingw-w64/include/stdarg.h" 2 3 4
# 2 "/usr/lib/gcc/x86_64-w64-mingw32/9.2-win32/include/stdarg.h" 2 3 4
# 21 "./util/log/Log_impl.h" 2

typedef void (* Log_callback) (struct Log* log,
                               enum Log_Level logLevel,
                               const char* file,
                               int line,
                               const char* format,
                               va_list args);

struct Log
{
    Log_callback print;
};
# 18 "util/log/WriterLog.c" 2
# 1 "./util/CString.h" 1
# 21 "./util/CString.h"
struct Linker_x09285538105775457;

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
# 19 "util/log/WriterLog.c" 2


# 1 "/usr/lib/gcc/x86_64-w64-mingw32/9.2-win32/include/stdarg.h" 1 3 4
# 1 "/usr/share/mingw-w64/include/stdarg.h" 1 3 4
# 2 "/usr/lib/gcc/x86_64-w64-mingw32/9.2-win32/include/stdarg.h" 2 3 4
# 22 "util/log/WriterLog.c" 2

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
# 24 "util/log/WriterLog.c" 2
# 1 "/usr/share/mingw-w64/include/strings.h" 1 3
# 16 "/usr/share/mingw-w64/include/strings.h" 3
# 1 "/usr/share/mingw-w64/include/string.h" 1 3
# 45 "/usr/share/mingw-w64/include/string.h" 3
  __attribute__ ((__dllimport__)) void *__attribute__((__cdecl__)) _memccpy(void *_Dst,const void *_Src,int _Val,size_t _MaxCount);
  void *__attribute__((__cdecl__)) memchr(const void *_Buf ,int _Val,size_t _MaxCount);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _memicmp(const void *_Buf1,const void *_Buf2,size_t _Size);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _memicmp_l(const void *_Buf1,const void *_Buf2,size_t _Size,_locale_t _Locale);
  int __attribute__((__cdecl__)) memcmp(const void *_Buf1,const void *_Buf2,size_t _Size);
  void * __attribute__((__cdecl__)) memcpy(void * __restrict__ _Dst,const void * __restrict__ _Src,size_t _Size) ;
  __attribute__((dllimport)) errno_t __attribute__((__cdecl__)) memcpy_s (void *_dest,size_t _numberOfElements,const void *_src,size_t _count);
  void * __attribute__((__cdecl__)) mempcpy (void *_Dst, const void *_Src, size_t _Size);
  void * __attribute__((__cdecl__)) memset(void *_Dst,int _Val,size_t _Size);

  void * __attribute__((__cdecl__)) memccpy(void *_Dst,const void *_Src,int _Val,size_t _Size) ;
  int __attribute__((__cdecl__)) memicmp(const void *_Buf1,const void *_Buf2,size_t _Size) ;


  char * __attribute__((__cdecl__)) _strset(char *_Str,int _Val) ;
  char * __attribute__((__cdecl__)) _strset_l(char *_Str,int _Val,_locale_t _Locale) ;
  char * __attribute__((__cdecl__)) strcpy(char * __restrict__ _Dest,const char * __restrict__ _Source);
  char * __attribute__((__cdecl__)) strcat(char * __restrict__ _Dest,const char * __restrict__ _Source);
  int __attribute__((__cdecl__)) strcmp(const char *_Str1,const char *_Str2);
  size_t __attribute__((__cdecl__)) strlen(const char *_Str);
  size_t __attribute__((__cdecl__)) strnlen(const char *_Str,size_t _MaxCount);
  void *__attribute__((__cdecl__)) memmove(void *_Dst,const void *_Src,size_t _Size) ;
  __attribute__ ((__dllimport__)) char *__attribute__((__cdecl__)) _strdup(const char *_Src);
  char *__attribute__((__cdecl__)) strchr(const char *_Str,int _Val);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _stricmp(const char *_Str1,const char *_Str2);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _strcmpi(const char *_Str1,const char *_Str2);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _stricmp_l(const char *_Str1,const char *_Str2,_locale_t _Locale);
  int __attribute__((__cdecl__)) strcoll(const char *_Str1,const char *_Str2);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _strcoll_l(const char *_Str1,const char *_Str2,_locale_t _Locale);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _stricoll(const char *_Str1,const char *_Str2);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _stricoll_l(const char *_Str1,const char *_Str2,_locale_t _Locale);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _strncoll (const char *_Str1,const char *_Str2,size_t _MaxCount);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _strncoll_l(const char *_Str1,const char *_Str2,size_t _MaxCount,_locale_t _Locale);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _strnicoll (const char *_Str1,const char *_Str2,size_t _MaxCount);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _strnicoll_l(const char *_Str1,const char *_Str2,size_t _MaxCount,_locale_t _Locale);
  size_t __attribute__((__cdecl__)) strcspn(const char *_Str,const char *_Control);
  __attribute__ ((__dllimport__)) char *__attribute__((__cdecl__)) _strerror(const char *_ErrMsg) ;
  char *__attribute__((__cdecl__)) strerror(int) ;
  __attribute__ ((__dllimport__)) char *__attribute__((__cdecl__)) _strlwr(char *_String) ;
  char *strlwr_l(char *_String,_locale_t _Locale) ;
  char *__attribute__((__cdecl__)) strncat(char * __restrict__ _Dest,const char * __restrict__ _Source,size_t _Count) ;
  int __attribute__((__cdecl__)) strncmp(const char *_Str1,const char *_Str2,size_t _MaxCount);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _strnicmp(const char *_Str1,const char *_Str2,size_t _MaxCount);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _strnicmp_l(const char *_Str1,const char *_Str2,size_t _MaxCount,_locale_t _Locale);
  char *strncpy(char * __restrict__ _Dest,const char * __restrict__ _Source,size_t _Count) ;
  __attribute__ ((__dllimport__)) char *__attribute__((__cdecl__)) _strnset(char *_Str,int _Val,size_t _MaxCount) ;
  __attribute__ ((__dllimport__)) char *__attribute__((__cdecl__)) _strnset_l(char *str,int c,size_t count,_locale_t _Locale) ;
  char *__attribute__((__cdecl__)) strpbrk(const char *_Str,const char *_Control);
  char *__attribute__((__cdecl__)) strrchr(const char *_Str,int _Ch);
  __attribute__ ((__dllimport__)) char *__attribute__((__cdecl__)) _strrev(char *_Str);
  size_t __attribute__((__cdecl__)) strspn(const char *_Str,const char *_Control);
  char *__attribute__((__cdecl__)) strstr(const char *_Str,const char *_SubStr);
  char *__attribute__((__cdecl__)) strtok(char * __restrict__ _Str,const char * __restrict__ _Delim) ;
       

  char *strtok_r(char * __restrict__ _Str, const char * __restrict__ _Delim, char ** __restrict__ __last);
       
  __attribute__ ((__dllimport__)) char *__attribute__((__cdecl__)) _strupr(char *_String) ;
  __attribute__ ((__dllimport__)) char *_strupr_l(char *_String,_locale_t _Locale) ;
  size_t __attribute__((__cdecl__)) strxfrm(char * __restrict__ _Dst,const char * __restrict__ _Src,size_t _MaxCount);
  __attribute__ ((__dllimport__)) size_t __attribute__((__cdecl__)) _strxfrm_l(char * __restrict__ _Dst,const char * __restrict__ _Src,size_t _MaxCount,_locale_t _Locale);


  char *__attribute__((__cdecl__)) strdup(const char *_Src) ;
  int __attribute__((__cdecl__)) strcmpi(const char *_Str1,const char *_Str2) ;
  int __attribute__((__cdecl__)) stricmp(const char *_Str1,const char *_Str2) ;
  char *__attribute__((__cdecl__)) strlwr(char *_Str) ;
  int __attribute__((__cdecl__)) strnicmp(const char *_Str1,const char *_Str,size_t _MaxCount) ;
  int __attribute__((__cdecl__)) strncasecmp (const char *, const char *, size_t);
  int __attribute__((__cdecl__)) strcasecmp (const char *, const char *);







  char *__attribute__((__cdecl__)) strnset(char *_Str,int _Val,size_t _MaxCount) ;
  char *__attribute__((__cdecl__)) strrev(char *_Str) ;
  char *__attribute__((__cdecl__)) strset(char *_Str,int _Val) ;
  char *__attribute__((__cdecl__)) strupr(char *_Str) ;





  __attribute__ ((__dllimport__)) wchar_t *__attribute__((__cdecl__)) _wcsdup(const wchar_t *_Str);
  wchar_t *__attribute__((__cdecl__)) wcscat(wchar_t * __restrict__ _Dest,const wchar_t * __restrict__ _Source) ;
  wchar_t *__attribute__((__cdecl__)) wcschr(const wchar_t *_Str,wchar_t _Ch);
  int __attribute__((__cdecl__)) wcscmp(const wchar_t *_Str1,const wchar_t *_Str2);
  wchar_t *__attribute__((__cdecl__)) wcscpy(wchar_t * __restrict__ _Dest,const wchar_t * __restrict__ _Source) ;
  size_t __attribute__((__cdecl__)) wcscspn(const wchar_t *_Str,const wchar_t *_Control);
  size_t __attribute__((__cdecl__)) wcslen(const wchar_t *_Str);
  size_t __attribute__((__cdecl__)) wcsnlen(const wchar_t *_Src,size_t _MaxCount);
  wchar_t *wcsncat(wchar_t * __restrict__ _Dest,const wchar_t * __restrict__ _Source,size_t _Count) ;
  int __attribute__((__cdecl__)) wcsncmp(const wchar_t *_Str1,const wchar_t *_Str2,size_t _MaxCount);
  wchar_t *wcsncpy(wchar_t * __restrict__ _Dest,const wchar_t * __restrict__ _Source,size_t _Count) ;
  wchar_t *__attribute__((__cdecl__)) _wcsncpy_l(wchar_t * __restrict__ _Dest,const wchar_t * __restrict__ _Source,size_t _Count,_locale_t _Locale) ;
  wchar_t *__attribute__((__cdecl__)) wcspbrk(const wchar_t *_Str,const wchar_t *_Control);
  wchar_t *__attribute__((__cdecl__)) wcsrchr(const wchar_t *_Str,wchar_t _Ch);
  size_t __attribute__((__cdecl__)) wcsspn(const wchar_t *_Str,const wchar_t *_Control);
  wchar_t *__attribute__((__cdecl__)) wcsstr(const wchar_t *_Str,const wchar_t *_SubStr);
  wchar_t *__attribute__((__cdecl__)) wcstok(wchar_t * __restrict__ _Str,const wchar_t * __restrict__ _Delim) ;
  __attribute__ ((__dllimport__)) wchar_t *__attribute__((__cdecl__)) _wcserror(int _ErrNum) ;
  __attribute__ ((__dllimport__)) wchar_t *__attribute__((__cdecl__)) __wcserror(const wchar_t *_Str) ;
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wcsicmp(const wchar_t *_Str1,const wchar_t *_Str2);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wcsicmp_l(const wchar_t *_Str1,const wchar_t *_Str2,_locale_t _Locale);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wcsnicmp(const wchar_t *_Str1,const wchar_t *_Str2,size_t _MaxCount);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wcsnicmp_l(const wchar_t *_Str1,const wchar_t *_Str2,size_t _MaxCount,_locale_t _Locale);
  __attribute__ ((__dllimport__)) wchar_t *__attribute__((__cdecl__)) _wcsnset(wchar_t *_Str,wchar_t _Val,size_t _MaxCount) ;
  __attribute__ ((__dllimport__)) wchar_t *__attribute__((__cdecl__)) _wcsrev(wchar_t *_Str);
  __attribute__ ((__dllimport__)) wchar_t *__attribute__((__cdecl__)) _wcsset(wchar_t *_Str,wchar_t _Val) ;
  __attribute__ ((__dllimport__)) wchar_t *__attribute__((__cdecl__)) _wcslwr(wchar_t *_String) ;
  __attribute__ ((__dllimport__)) wchar_t *_wcslwr_l(wchar_t *_String,_locale_t _Locale) ;
  __attribute__ ((__dllimport__)) wchar_t *__attribute__((__cdecl__)) _wcsupr(wchar_t *_String) ;
  __attribute__ ((__dllimport__)) wchar_t *_wcsupr_l(wchar_t *_String,_locale_t _Locale) ;
  size_t __attribute__((__cdecl__)) wcsxfrm(wchar_t * __restrict__ _Dst,const wchar_t * __restrict__ _Src,size_t _MaxCount);
  __attribute__ ((__dllimport__)) size_t __attribute__((__cdecl__)) _wcsxfrm_l(wchar_t * __restrict__ _Dst,const wchar_t * __restrict__ _Src,size_t _MaxCount,_locale_t _Locale);
  int __attribute__((__cdecl__)) wcscoll(const wchar_t *_Str1,const wchar_t *_Str2);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wcscoll_l(const wchar_t *_Str1,const wchar_t *_Str2,_locale_t _Locale);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wcsicoll(const wchar_t *_Str1,const wchar_t *_Str2);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wcsicoll_l(const wchar_t *_Str1,const wchar_t *_Str2,_locale_t _Locale);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wcsncoll(const wchar_t *_Str1,const wchar_t *_Str2,size_t _MaxCount);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wcsncoll_l(const wchar_t *_Str1,const wchar_t *_Str2,size_t _MaxCount,_locale_t _Locale);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wcsnicoll(const wchar_t *_Str1,const wchar_t *_Str2,size_t _MaxCount);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wcsnicoll_l(const wchar_t *_Str1,const wchar_t *_Str2,size_t _MaxCount,_locale_t _Locale);


  wchar_t *__attribute__((__cdecl__)) wcsdup(const wchar_t *_Str) ;

  int __attribute__((__cdecl__)) wcsicmp(const wchar_t *_Str1,const wchar_t *_Str2) ;
  int __attribute__((__cdecl__)) wcsnicmp(const wchar_t *_Str1,const wchar_t *_Str2,size_t _MaxCount) ;
  wchar_t *__attribute__((__cdecl__)) wcsnset(wchar_t *_Str,wchar_t _Val,size_t _MaxCount) ;
  wchar_t *__attribute__((__cdecl__)) wcsrev(wchar_t *_Str) ;
  wchar_t *__attribute__((__cdecl__)) wcsset(wchar_t *_Str,wchar_t _Val) ;
  wchar_t *__attribute__((__cdecl__)) wcslwr(wchar_t *_Str) ;
  wchar_t *__attribute__((__cdecl__)) wcsupr(wchar_t *_Str) ;
  int __attribute__((__cdecl__)) wcsicoll(const wchar_t *_Str1,const wchar_t *_Str2) ;







# 1 "/usr/share/mingw-w64/include/sec_api/string_s.h" 1 3
# 9 "/usr/share/mingw-w64/include/sec_api/string_s.h" 3
# 1 "/usr/share/mingw-w64/include/string.h" 1 3
# 10 "/usr/share/mingw-w64/include/sec_api/string_s.h" 2 3
# 26 "/usr/share/mingw-w64/include/sec_api/string_s.h" 3
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _strset_s(char *_Dst,size_t _DstSize,int _Value);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _strerror_s(char *_Buf,size_t _SizeInBytes,const char *_ErrMsg);
  __attribute__((dllimport)) errno_t __attribute__((__cdecl__)) strerror_s(char *_Buf,size_t _SizeInBytes,int _ErrNum);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _strlwr_s(char *_Str,size_t _Size);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _strlwr_s_l(char *_Str,size_t _Size,_locale_t _Locale);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _strnset_s(char *_Str,size_t _Size,int _Val,size_t _MaxCount);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _strupr_s(char *_Str,size_t _Size);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _strupr_s_l(char *_Str,size_t _Size,_locale_t _Locale);

  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) strncat_s(char *_Dst,size_t _DstSizeInChars,const char *_Src,size_t _MaxCount);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _strncat_s_l(char *_Dst,size_t _DstSizeInChars,const char *_Src,size_t _MaxCount,_locale_t _Locale);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) strcpy_s(char *_Dst, rsize_t _SizeInBytes, const char *_Src);
 
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) strncpy_s(char *_Dst, size_t _DstSizeInChars, const char *_Src, size_t _MaxCount);
 
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _strncpy_s_l(char *_Dst, size_t _DstSizeInChars, const char *_Src, size_t _MaxCount, _locale_t _Locale);
  ;
  __attribute__ ((__dllimport__)) char *__attribute__((__cdecl__)) strtok_s(char *_Str,const char *_Delim,char **_Context);
  __attribute__ ((__dllimport__)) char *__attribute__((__cdecl__)) _strtok_s_l(char *_Str,const char *_Delim,char **_Context,_locale_t _Locale);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) strcat_s(char *_Dst, rsize_t _SizeInBytes, const char * _Src);
 

  __attribute__((dllimport)) errno_t __attribute__((__cdecl__)) memmove_s(void *_dest,size_t _numberOfElements,const void *_src,size_t _count);


  __attribute__ ((__dllimport__)) wchar_t *__attribute__((__cdecl__)) wcstok_s(wchar_t *_Str,const wchar_t *_Delim,wchar_t **_Context);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _wcserror_s(wchar_t *_Buf,size_t _SizeInWords,int _ErrNum);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) __wcserror_s(wchar_t *_Buffer,size_t _SizeInWords,const wchar_t *_ErrMsg);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _wcsnset_s(wchar_t *_Dst,size_t _DstSizeInWords,wchar_t _Val,size_t _MaxCount);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _wcsset_s(wchar_t *_Str,size_t _SizeInWords,wchar_t _Val);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _wcslwr_s(wchar_t *_Str,size_t _SizeInWords);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _wcslwr_s_l(wchar_t *_Str,size_t _SizeInWords,_locale_t _Locale);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _wcsupr_s(wchar_t *_Str,size_t _Size);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _wcsupr_s_l(wchar_t *_Str,size_t _Size,_locale_t _Locale);

  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) wcscpy_s(wchar_t *_Dst, rsize_t _SizeInWords, const wchar_t *_Src);
 
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) wcscat_s(wchar_t * _Dst, rsize_t _SizeInWords, const wchar_t *_Src);
 

  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) wcsncat_s(wchar_t *_Dst,size_t _DstSizeInChars,const wchar_t *_Src,size_t _MaxCount);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _wcsncat_s_l(wchar_t *_Dst,size_t _DstSizeInChars,const wchar_t *_Src,size_t _MaxCount,_locale_t _Locale);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) wcsncpy_s(wchar_t *_Dst, size_t _DstSizeInChars, const wchar_t *_Src, size_t _MaxCount);
  ;
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _wcsncpy_s_l(wchar_t *_Dst, size_t _DstSizeInChars, const wchar_t *_Src, size_t _MaxCount, _locale_t _Locale);
  ;
  __attribute__ ((__dllimport__)) wchar_t *__attribute__((__cdecl__)) _wcstok_s_l(wchar_t *_Str,const wchar_t *_Delim,wchar_t **_Context,_locale_t _Locale);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _wcsset_s_l(wchar_t *_Str,size_t _SizeInChars,unsigned int _Val,_locale_t _Locale);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _wcsnset_s_l(wchar_t *_Str,size_t _SizeInChars,unsigned int _Val, size_t _Count,_locale_t _Locale);

  extern __inline__ __attribute__((__always_inline__,__gnu_inline__)) size_t __attribute__((__cdecl__)) wcsnlen_s(const wchar_t * _src, size_t _count) {
    return _src ? wcsnlen(_src, _count) : 0;
  }
# 191 "/usr/share/mingw-w64/include/string.h" 2 3
# 17 "/usr/share/mingw-w64/include/strings.h" 2 3
# 25 "util/log/WriterLog.c" 2
# 1 "/usr/share/mingw-w64/include/time.h" 1 3
# 24 "/usr/share/mingw-w64/include/time.h" 3
#pragma pack(push,_CRT_PACKING)
# 60 "/usr/share/mingw-w64/include/time.h" 3
  typedef long clock_t;
# 97 "/usr/share/mingw-w64/include/time.h" 3
  struct tm {
    int tm_sec;
    int tm_min;
    int tm_hour;
    int tm_mday;
    int tm_mon;
    int tm_year;
    int tm_wday;
    int tm_yday;
    int tm_isdst;
  };
# 122 "/usr/share/mingw-w64/include/time.h" 3
  extern __attribute__ ((__dllimport__)) int _daylight;
  extern __attribute__ ((__dllimport__)) long _dstbias;
  extern __attribute__ ((__dllimport__)) long _timezone;
  extern __attribute__ ((__dllimport__)) char * _tzname[2];


  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _get_daylight(int *_Daylight);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _get_dstbias(long *_Daylight_savings_bias);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _get_timezone(long *_Timezone);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _get_tzname(size_t *_ReturnValue,char *_Buffer,size_t _SizeInBytes,int _Index);
  char *__attribute__((__cdecl__)) asctime(const struct tm *_Tm) ;
  __attribute__((dllimport)) errno_t __attribute__((__cdecl__)) asctime_s (char *_Buf,size_t _SizeInWords,const struct tm *_Tm);
  char *__attribute__((__cdecl__)) _ctime32(const __time32_t *_Time) ;
  __attribute__((dllimport)) errno_t __attribute__((__cdecl__)) _ctime32_s (char *_Buf,size_t _SizeInBytes,const __time32_t *_Time);
  clock_t __attribute__((__cdecl__)) clock(void);
  double __attribute__((__cdecl__)) _difftime32(__time32_t _Time1,__time32_t _Time2);
  struct tm *__attribute__((__cdecl__)) _gmtime32(const __time32_t *_Time) ;
  __attribute__((dllimport)) errno_t __attribute__((__cdecl__)) _gmtime32_s (struct tm *_Tm,const __time32_t *_Time);
  struct tm *__attribute__((__cdecl__)) _localtime32(const __time32_t *_Time) ;
  __attribute__((dllimport)) errno_t __attribute__((__cdecl__)) _localtime32_s (struct tm *_Tm,const __time32_t *_Time);
  size_t __attribute__((__cdecl__)) strftime(char * __restrict__ _Buf,size_t _SizeInBytes,const char * __restrict__ _Format,const struct tm * __restrict__ _Tm);
  __attribute__ ((__dllimport__)) size_t __attribute__((__cdecl__)) _strftime_l(char * __restrict__ _Buf,size_t _Max_size,const char * __restrict__ _Format,const struct tm * __restrict__ _Tm,_locale_t _Locale);
  __attribute__ ((__dllimport__)) char *__attribute__((__cdecl__)) _strdate(char *_Buffer) ;
  __attribute__((dllimport)) errno_t __attribute__((__cdecl__)) _strdate_s (char *_Buf,size_t _SizeInBytes);
  __attribute__ ((__dllimport__)) char *__attribute__((__cdecl__)) _strtime(char *_Buffer) ;
  __attribute__((dllimport)) errno_t __attribute__((__cdecl__)) _strtime_s (char *_Buf ,size_t _SizeInBytes);
  __time32_t __attribute__((__cdecl__)) _time32(__time32_t *_Time);
  __time32_t __attribute__((__cdecl__)) _mktime32(struct tm *_Tm);
  __time32_t __attribute__((__cdecl__)) _mkgmtime32(struct tm *_Tm);


  void __attribute__((__cdecl__)) tzset(void) ;



  __attribute__ ((__dllimport__))

  void __attribute__((__cdecl__)) _tzset(void);


  double __attribute__((__cdecl__)) _difftime64(__time64_t _Time1,__time64_t _Time2);
  __attribute__ ((__dllimport__)) char *__attribute__((__cdecl__)) _ctime64(const __time64_t *_Time) ;
  __attribute__((dllimport)) errno_t __attribute__((__cdecl__)) _ctime64_s (char *_Buf,size_t _SizeInBytes,const __time64_t *_Time);
  __attribute__ ((__dllimport__)) struct tm *__attribute__((__cdecl__)) _gmtime64(const __time64_t *_Time) ;
  __attribute__((dllimport)) errno_t __attribute__((__cdecl__)) _gmtime64_s (struct tm *_Tm,const __time64_t *_Time);
  __attribute__ ((__dllimport__)) struct tm *__attribute__((__cdecl__)) _localtime64(const __time64_t *_Time) ;
  __attribute__((dllimport)) errno_t __attribute__((__cdecl__)) _localtime64_s (struct tm *_Tm,const __time64_t *_Time);
  __attribute__ ((__dllimport__)) __time64_t __attribute__((__cdecl__)) _mktime64(struct tm *_Tm);
  __attribute__ ((__dllimport__)) __time64_t __attribute__((__cdecl__)) _mkgmtime64(struct tm *_Tm);
  __attribute__ ((__dllimport__)) __time64_t __attribute__((__cdecl__)) _time64(__time64_t *_Time);
  unsigned __attribute__((__cdecl__)) _getsystime(struct tm *_Tm);
  unsigned __attribute__((__cdecl__)) _setsystime(struct tm *_Tm,unsigned _MilliSec);


  __attribute__ ((__dllimport__)) wchar_t *__attribute__((__cdecl__)) _wasctime(const struct tm *_Tm);
  __attribute__((dllimport)) errno_t __attribute__((__cdecl__)) _wasctime_s (wchar_t *_Buf,size_t _SizeInWords,const struct tm *_Tm);
  wchar_t *__attribute__((__cdecl__)) _wctime32(const __time32_t *_Time) ;
  __attribute__((dllimport)) errno_t __attribute__((__cdecl__)) _wctime32_s (wchar_t *_Buf,size_t _SizeInWords,const __time32_t *_Time);
  size_t __attribute__((__cdecl__)) wcsftime(wchar_t * __restrict__ _Buf,size_t _SizeInWords,const wchar_t * __restrict__ _Format,const struct tm * __restrict__ _Tm);
  __attribute__ ((__dllimport__)) size_t __attribute__((__cdecl__)) _wcsftime_l(wchar_t * __restrict__ _Buf,size_t _SizeInWords,const wchar_t * __restrict__ _Format,const struct tm * __restrict__ _Tm,_locale_t _Locale);
  __attribute__ ((__dllimport__)) wchar_t *__attribute__((__cdecl__)) _wstrdate(wchar_t *_Buffer) ;
  __attribute__((dllimport)) errno_t __attribute__((__cdecl__)) _wstrdate_s (wchar_t *_Buf,size_t _SizeInWords);
  __attribute__ ((__dllimport__)) wchar_t *__attribute__((__cdecl__)) _wstrtime(wchar_t *_Buffer) ;
  __attribute__((dllimport)) errno_t __attribute__((__cdecl__)) _wstrtime_s (wchar_t *_Buf,size_t _SizeInWords);
  __attribute__ ((__dllimport__)) wchar_t *__attribute__((__cdecl__)) _wctime64(const __time64_t *_Time) ;
  __attribute__((dllimport)) errno_t __attribute__((__cdecl__)) _wctime64_s (wchar_t *_Buf,size_t _SizeInWords,const __time64_t *_Time);



  wchar_t *__attribute__((__cdecl__)) _wctime(const time_t *) ;
# 203 "/usr/share/mingw-w64/include/time.h" 3
  errno_t __attribute__((__cdecl__)) _wctime_s(wchar_t *, size_t, const time_t *);
# 218 "/usr/share/mingw-w64/include/time.h" 3
double __attribute__((__cdecl__)) difftime(time_t _Time1,time_t _Time2);
char *__attribute__((__cdecl__)) ctime(const time_t *_Time) ;
struct tm *__attribute__((__cdecl__)) gmtime(const time_t *_Time) ;
struct tm *__attribute__((__cdecl__)) localtime(const time_t *_Time) ;

time_t __attribute__((__cdecl__)) mktime(struct tm *_Tm);
time_t __attribute__((__cdecl__)) _mkgmtime(struct tm *_Tm);
time_t __attribute__((__cdecl__)) time(time_t *_Time);
# 261 "/usr/share/mingw-w64/include/time.h" 3
extern __inline__ __attribute__((__always_inline__,__gnu_inline__)) errno_t __attribute__((__cdecl__)) localtime_s(struct tm *_Tm,const time_t *_Time) { return _localtime64_s(_Tm,_Time); }
extern __inline__ __attribute__((__always_inline__,__gnu_inline__)) errno_t __attribute__((__cdecl__)) gmtime_s(struct tm *_Tm, const time_t *_Time) { return _gmtime64_s(_Tm, _Time); }
extern __inline__ __attribute__((__always_inline__,__gnu_inline__)) errno_t __attribute__((__cdecl__)) ctime_s(char *_Buf,size_t _SizeInBytes,const time_t *_Time) { return _ctime64_s(_Buf,_SizeInBytes,_Time); }
# 279 "/usr/share/mingw-w64/include/time.h" 3
  __attribute__ ((__dllimport__)) extern int daylight ;
  __attribute__ ((__dllimport__)) extern long timezone ;
  __attribute__ ((__dllimport__)) extern char *tzname[2] ;
  void __attribute__((__cdecl__)) tzset(void) ;


# 1 "/usr/share/mingw-w64/include/_timeval.h" 1 3
# 10 "/usr/share/mingw-w64/include/_timeval.h" 3
struct timeval
{
 long tv_sec;
 long tv_usec;
};
# 286 "/usr/share/mingw-w64/include/time.h" 2 3



struct timezone {
  int tz_minuteswest;
  int tz_dsttime;
};

  extern int __attribute__((__cdecl__)) mingw_gettimeofday (struct timeval *p, struct timezone *z);






#pragma pack(pop)
# 323 "/usr/share/mingw-w64/include/time.h" 3
# 1 "/usr/share/mingw-w64/include/sys/timeb.h" 1 3
# 15 "/usr/share/mingw-w64/include/sys/timeb.h" 3
#pragma pack(push,_CRT_PACKING)
# 53 "/usr/share/mingw-w64/include/sys/timeb.h" 3
  struct __timeb32 {
    __time32_t time;
    unsigned short millitm;
    short timezone;
    short dstflag;
  };


  struct timeb {
    time_t time;
    unsigned short millitm;
    short timezone;
    short dstflag;
  };


  struct __timeb64 {
    __time64_t time;
    unsigned short millitm;
    short timezone;
    short dstflag;
  };



  __attribute__ ((__dllimport__)) void __attribute__((__cdecl__)) _ftime64(struct __timeb64 *_Time);



  __attribute__ ((__dllimport__)) void __attribute__((__cdecl__)) _ftime(struct __timeb64 *);







struct timespec {
  time_t tv_sec;
  long tv_nsec;
};

struct itimerspec {
  struct timespec it_interval;
  struct timespec it_value;
};



  void __attribute__((__cdecl__)) ftime (struct timeb *);
# 122 "/usr/share/mingw-w64/include/sys/timeb.h" 3
#pragma pack(pop)

# 1 "/usr/share/mingw-w64/include/sec_api/sys/timeb_s.h" 1 3
# 10 "/usr/share/mingw-w64/include/sec_api/sys/timeb_s.h" 3
# 1 "/usr/share/mingw-w64/include/sys/timeb.h" 1 3
# 11 "/usr/share/mingw-w64/include/sec_api/sys/timeb_s.h" 2 3







  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _ftime_s(struct __timeb32 *_Time);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _ftime64_s(struct __timeb64 *_Time);
# 125 "/usr/share/mingw-w64/include/sys/timeb.h" 2 3
# 324 "/usr/share/mingw-w64/include/time.h" 2 3







# 1 "/usr/share/mingw-w64/include/pthread_time.h" 1 3
# 49 "/usr/share/mingw-w64/include/pthread_time.h" 3
typedef int clockid_t;
# 78 "/usr/share/mingw-w64/include/pthread_time.h" 3
       





int __attribute__((__cdecl__)) nanosleep(const struct timespec *request, struct timespec *remain);

int __attribute__((__cdecl__)) clock_nanosleep(clockid_t clock_id, int flags, const struct timespec *request, struct timespec *remain);
int __attribute__((__cdecl__)) clock_getres(clockid_t clock_id, struct timespec *res);
int __attribute__((__cdecl__)) clock_gettime(clockid_t clock_id, struct timespec *tp);
int __attribute__((__cdecl__)) clock_settime(clockid_t clock_id, const struct timespec *tp);

       
# 332 "/usr/share/mingw-w64/include/time.h" 2 3
# 26 "util/log/WriterLog.c" 2


# 27 "util/log/WriterLog.c"
struct WriterLog
{
    struct Log pub;
    struct Writer* writer;
};

static void print(struct Log* genericLog,
                  enum Log_Level logLevel,
                  const char* file,
                  int line,
                  const char* format,
                  va_list args)
{
    struct WriterLog* log = (struct WriterLog*) genericLog;
    char timeAndLevelBuff[64];
    time_t now;
    time(&now);
    snprintf(timeAndLevelBuff, 64, "%u %s ", (uint32_t) now, Log_nameForLevel(logLevel));
    ((log->writer)->write((log->writer), (timeAndLevelBuff), (CString_strlen(timeAndLevelBuff))));



    ((log->writer)->write((log->writer), (file), (CString_strlen(file))));


    char buff[1024];
    snprintf(buff, 1024, ":%u ", line);
    ((log->writer)->write((log->writer), (buff), (CString_strlen(buff))));

    vsnprintf(buff, 1024, format, args);

    size_t length = CString_strlen(buff);


    if (length < 1024 && buff[length - 1] != '\n') {
        buff[length++] = '\n';
    }

    ((log->writer)->write((log->writer), (buff), (length > 1024 ? 1024 : length)));

}

struct Log* WriterLog_new(struct Writer* w, struct Allocator* alloc)
{
    return Allocator__clone((alloc),((&(struct WriterLog) { .pub = { .print = print }, .writer = w })),sizeof(*((&(struct WriterLog) { .pub = { .print = print }, .writer = w }))),"WriterLog.c",71)




       ;
}
