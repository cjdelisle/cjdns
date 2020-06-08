# 1 "client/Configurator.c"
# 1 "/home/jojapoppa/cjdns//"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "client/Configurator.c"
# 15 "client/Configurator.c"
# 1 "/usr/share/mingw-w64/include/stdio.h" 1 3
# 9 "/usr/share/mingw-w64/include/stdio.h" 3
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
# 10 "/usr/share/mingw-w64/include/stdio.h" 2 3

# 1 "/usr/share/mingw-w64/include/_mingw_print_push.h" 1 3
# 12 "/usr/share/mingw-w64/include/stdio.h" 2 3

#pragma pack(push,_CRT_PACKING)
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
# 16 "client/Configurator.c" 2
# 1 "./client/AdminClient.h" 1
# 18 "./client/AdminClient.h"
# 1 "./interface/addressable/AddrIface.h" 1
# 18 "./interface/addressable/AddrIface.h"
# 1 "./interface/Iface.h" 1
# 18 "./interface/Iface.h"
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
struct Linker_x06343823424955259;
# 33 "./util/Assert.h"
__attribute__((__format__ (__printf__, 1, 2)))
__attribute__((__noreturn__))
void Assert_failure(const char* format, ...);
# 19 "./util/Identity.h" 2
# 1 "./util/Constant.h" 1
# 33 "./util/Constant.h"

# 20 "./util/Identity.h" 2



    
# 19 "./memory/Allocator.h" 2


struct Linker_x09032418583156696;





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




struct Linker_x048842528915204464;

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
struct Linker_x013422769456755557;


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

struct Linker_x08573878152475245;



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
# 19 "./interface/addressable/AddrIface.h" 2
# 1 "./util/platform/Sockaddr.h" 1
# 21 "./util/platform/Sockaddr.h"
struct Linker_x05943232617418759;



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
struct Linker_x05747970165260514;



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
# 19 "./client/AdminClient.h" 2
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
struct Linker_x017573968375197402;

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

struct Linker_x016754323892397127;

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
# 20 "./client/AdminClient.h" 2
# 1 "./benc/Dict.h" 1
# 21 "./benc/Dict.h"
struct Linker_x09285866069694861;

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
# 21 "./client/AdminClient.h" 2

# 1 "./util/log/Log.h" 1
# 20 "./util/log/Log.h"
struct Linker_x00017102049914914375;

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
# 23 "./client/AdminClient.h" 2
# 1 "./util/events/EventBase.h" 1
# 20 "./util/events/EventBase.h"
struct Linker_x07361365978847321;

struct EventBase
{
    int unused;
};

struct EventBase* EventBase_new(struct Allocator* alloc);

int EventBase_eventCount(struct EventBase* eventBase);

void EventBase_beginLoop(struct EventBase* eventBase);

void EventBase_endLoop(struct EventBase* eventBase);
# 24 "./client/AdminClient.h" 2


struct Linker_x09556782583437022;

enum AdminClient_Error
{

    AdminClient_Error_NONE = 0,


    AdminClient_Error_OVERLONG_RESPONSE,


    AdminClient_Error_ERROR_READING_FROM_SOCKET,


    AdminClient_Error_SOCKET_NOT_READY,


    AdminClient_Error_DESERIALIZATION_FAILED,


    AdminClient_Error_SERIALIZATION_FAILED,


    AdminClient_Error_TIMEOUT,


    AdminClient_Error_NO_COOKIE,
};







struct AdminClient_Result
{

    enum AdminClient_Error err;


    uint8_t padding[(sizeof(struct Sockaddr_storage))];







    uint8_t messageBytes[1023 + 1];


    Dict* responseDict;
};

struct AdminClient_Promise;

typedef void (* AdminClient_ResultHandler)(struct AdminClient_Promise* p,
                                           struct AdminClient_Result* res);

struct AdminClient_Promise
{
    AdminClient_ResultHandler callback;
    void* userData;
    struct Allocator* alloc;
};

struct AdminClient
{

    uint32_t millisecondsToWait;
};

char* AdminClient_errorString(enum AdminClient_Error err);

struct AdminClient_Promise* AdminClient_rpcCall(String* function,
                                                Dict* args,
                                                struct AdminClient* client,
                                                struct Allocator* requestAlloc);


struct AdminClient* AdminClient_new(struct AddrIface* ai,
                                    struct Sockaddr* connectToAddress,
                                    String* adminPassword,
                                    struct EventBase* eventBase,
                                    struct Log* logger,
                                    struct Allocator* alloc);
# 17 "client/Configurator.c" 2
# 1 "./client/Configurator.h" 1
# 25 "./client/Configurator.h"
struct Linker_x05157427569988127;



void Configurator_config(Dict* config,
                         struct Sockaddr* addr,
                         String* adminPassword,
                         struct EventBase* eventBase,
                         struct Log* logger,
                         struct Allocator* alloc);
# 18 "client/Configurator.c" 2


# 1 "./benc/Int.h" 1
# 21 "client/Configurator.c" 2
# 1 "./benc/List.h" 1
# 21 "./benc/List.h"
struct Linker_x02881954157000919;

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
# 22 "client/Configurator.c" 2

# 1 "./util/events/Event.h" 1
# 21 "./util/events/Event.h"
struct Linker_x02519530680478723;

struct Event
{
    int unused;
};

struct Event* Event_socketRead(void (* const callback)(void* callbackContext),
                               void* const callbackContext,
                               int s,
                               struct EventBase* base,
                               struct Allocator* alloc);
# 24 "client/Configurator.c" 2
# 1 "./util/events/UDPAddrIface.h" 1
# 25 "./util/events/UDPAddrIface.h"
struct Linker_x05077170661843835;
# 35 "./util/events/UDPAddrIface.h"
struct UDPAddrIface
{
    struct AddrIface generic;
};
# 47 "./util/events/UDPAddrIface.h"
__attribute__ ((warn_unused_result)) struct Er_Ret* UDPAddrIface_new(struct UDPAddrIface* *Er_returnValP, struct EventBase* eventBase, struct Sockaddr* addr, struct Allocator* alloc, struct Log* logger)


                                                          ;

int UDPAddrIface_setDSCP(struct UDPAddrIface* iface, uint8_t dscp);

int UDPAddrIface_setBroadcast(struct UDPAddrIface* iface, 
# 54 "./util/events/UDPAddrIface.h" 3 4
                                                         _Bool 
# 54 "./util/events/UDPAddrIface.h"
                                                              enable);

int UDPAddrIface_getFd(struct UDPAddrIface*);
# 25 "client/Configurator.c" 2




# 1 "./util/events/Timeout.h" 1
# 20 "./util/events/Timeout.h"
struct Linker_x021162163449348403;



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
# 30 "client/Configurator.c" 2

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
# 32 "client/Configurator.c" 2



# 34 "client/Configurator.c"
struct Context
{
    struct Log* logger;
    struct Allocator* alloc;
    struct AdminClient* client;

    struct Allocator* currentReqAlloc;
    struct AdminClient_Result* currentResult;

    struct EventBase* base;
};

static void rpcCallback(struct AdminClient_Promise* p, struct AdminClient_Result* res)
{
    struct Context* ctx = p->userData;
    Allocator__adopt((ctx->alloc),(p->alloc),"Configurator.c",49);
    ctx->currentResult = res;
    EventBase_endLoop(ctx->base);
}

static void die(struct AdminClient_Result* res, struct Context* ctx, struct Allocator* alloc)
{
    do { if (ctx->logger && Log_Level_KEYS >= Log_Level_DEBUG) { Log_print(ctx->logger, Log_Level_KEYS, "Configurator.c", 56, "message bytes = [%s]", res->messageBytes); } } while (0);

        do { if (ctx->logger && Log_Level_CRITICAL >= Log_Level_DEBUG) { Log_print(ctx->logger, Log_Level_CRITICAL, "Configurator.c", 58, "enable Log_LEVEL=KEYS to see message content."); } } while (0);


    Dict d = 
# 61 "client/Configurator.c" 3 4
            ((void *)0)
# 61 "client/Configurator.c"
                ;
    struct AdminClient_Promise* exitPromise =
        AdminClient_rpcCall((&(String) { .bytes = "Core_exit", .len = CString_strlen("Core_exit") }), &d, ctx->client, alloc);
    exitPromise->callback = rpcCallback;
    exitPromise->userData = ctx;

    EventBase_beginLoop(ctx->base);

    if (ctx->currentResult->err) {
        do { if (ctx->logger && Log_Level_CRITICAL >= Log_Level_DEBUG) { Log_print(ctx->logger, Log_Level_CRITICAL, "Configurator.c", 70, "Failed to stop the core."); } } while (0);
    }
    do { if (ctx->logger && Log_Level_CRITICAL >= Log_Level_DEBUG) { Log_print(ctx->logger, Log_Level_CRITICAL, "Configurator.c", 72, "Aborting."); } } while (0);
    exit(1);
}

static int rpcCall0(String* function,
                    Dict* args,
                    struct Context* ctx,
                    struct Allocator* alloc,
                    Dict** resultP,
                    
# 81 "client/Configurator.c" 3 4
                   _Bool 
# 81 "client/Configurator.c"
                        exitIfError)
{
    ctx->currentReqAlloc = Allocator__child((alloc),"Configurator.c",83);
    ctx->currentResult = 
# 84 "client/Configurator.c" 3 4
                        ((void *)0)
# 84 "client/Configurator.c"
                            ;
    struct AdminClient_Promise* promise = AdminClient_rpcCall(function, args, ctx->client, alloc);
    promise->callback = rpcCallback;
    promise->userData = ctx;

    EventBase_beginLoop(ctx->base);

    struct AdminClient_Result* res = ctx->currentResult;
    do { if (!((res))) { Assert_failure("Assertion failure [%s:%d] [%s]\n", ("Configurator.c"), (92), "(res)"); } } while (0);

    if (res->err) {
        do { if (ctx->logger && Log_Level_CRITICAL >= Log_Level_DEBUG) { Log_print(ctx->logger, Log_Level_CRITICAL, "Configurator.c", 95, "Failed to make function call [%s], error: [%s]", AdminClient_errorString(res->err), function->bytes); } } while (0)


                                      ;
        die(res, ctx, alloc);
    }
    String* error = Dict_getString(res->responseDict, (&(String) { .bytes = "error", .len = CString_strlen("error") }));
    int ret = 0;
    if (error && !String_equals(error, (&(String) { .bytes = "none", .len = CString_strlen("none") }))) {
        if (exitIfError) {
            do { if (ctx->logger && Log_Level_CRITICAL >= Log_Level_DEBUG) { Log_print(ctx->logger, Log_Level_CRITICAL, "Configurator.c", 105, "Got error [%s] calling [%s]", error->bytes, function->bytes); } } while (0)


                                         ;
            die(res, ctx, alloc);
        }
        do { if (ctx->logger && Log_Level_WARN >= Log_Level_DEBUG) { Log_print(ctx->logger, Log_Level_WARN, "Configurator.c", 111, "Got error [%s] calling [%s], ignoring.", error->bytes, function->bytes); } } while (0)
                                               ;
        ret = 1;
    }

    if (resultP) {
        *resultP = res->responseDict;
    } else {
        Allocator__free((ctx->currentReqAlloc),"Configurator.c",119);
    }
    ctx->currentReqAlloc = 
# 121 "client/Configurator.c" 3 4
                          ((void *)0)
# 121 "client/Configurator.c"
                              ;

    return ret;
}

static void rpcCall(String* function, Dict* args, struct Context* ctx, struct Allocator* alloc)
{
    rpcCall0(function, args, ctx, alloc, 
# 128 "client/Configurator.c" 3 4
                                        ((void *)0)
# 128 "client/Configurator.c"
                                            , 
# 128 "client/Configurator.c" 3 4
                                              1
# 128 "client/Configurator.c"
                                                  );
}

static void authorizedPasswords(List* list, struct Context* ctx)
{
    uint32_t count = List_size(list);
    for (uint32_t i = 0; i < count; i++) {
        Dict* d = List_getDict(list, i);
        do { if (ctx->logger && Log_Level_INFO >= Log_Level_DEBUG) { Log_print(ctx->logger, Log_Level_INFO, "Configurator.c", 136, "Checking authorized password %d.", i); } } while (0);
        if (!d) {
            do { if (ctx->logger && Log_Level_CRITICAL >= Log_Level_DEBUG) { Log_print(ctx->logger, Log_Level_CRITICAL, "Configurator.c", 138, "Not a dictionary type %d.", i); } } while (0);
            exit(-1);
        }
        String* passwd = Dict_getString(d, (&(String) { .bytes = "password", .len = CString_strlen("password") }));
        if (!passwd) {
            do { if (ctx->logger && Log_Level_CRITICAL >= Log_Level_DEBUG) { Log_print(ctx->logger, Log_Level_CRITICAL, "Configurator.c", 143, "Must specify a password %d.", i); } } while (0);
            exit(-1);
        }
    }

    for (uint32_t i = 0; i < count; i++) {
        struct Allocator* child = Allocator__child((ctx->alloc),"Configurator.c",149);
        Dict* d = List_getDict(list, i);
        String* passwd = Dict_getString(d, (&(String) { .bytes = "password", .len = CString_strlen("password") }));
        String* user = Dict_getString(d, (&(String) { .bytes = "user", .len = CString_strlen("user") }));
        String* displayName = user;
        if (!displayName) {
            displayName = String_printf(child, "password [%d]", i);
        }

        String* ipv6 = Dict_getString(d, (&(String) { .bytes = "ipv6", .len = CString_strlen("ipv6") }));
        do { if (ctx->logger && Log_Level_INFO >= Log_Level_DEBUG) { Log_print(ctx->logger, Log_Level_INFO, "Configurator.c", 159, "Adding authorized password #[%d] for user [%s].", i, displayName->bytes); } } while (0)
                                  ;
        Dict *args = Dict_new(child);
        uint32_t i = 1;
        Dict_putInt(args, String_new("authType", child), i, child);
        Dict_putString(args, String_new("password", child), passwd, child);
        if (user) {
            Dict_putString(args, String_new("user", child), user, child);
        }
        Dict_putString(args, String_new("displayName", child), displayName, child);
        if (ipv6) {
            do { if (ctx->logger && Log_Level_INFO >= Log_Level_DEBUG) { Log_print(ctx->logger, Log_Level_INFO, "Configurator.c", 170, "  This connection password restricted to [%s] only.", ipv6->bytes); } } while (0)
                                                                                   ;
            Dict_putString(args, String_new("ipv6", child), ipv6, child);
        }
        rpcCall((&(String) { .bytes = "AuthorizedPasswords_add", .len = CString_strlen("AuthorizedPasswords_add") }), args, ctx, child);
        Allocator__free((child),"Configurator.c",175);
    }
}

static void udpInterfaceSetBeacon(
    Dict* udp, int beacon, uint16_t beaconPort, int ifNum, struct Context* ctx)
{
    if (!beacon) { return; }
    if (beacon > 2 || beacon < 0) {
        do { if (ctx->logger && Log_Level_ERROR >= Log_Level_DEBUG) { Log_print(ctx->logger, Log_Level_ERROR, "Configurator.c", 184, "interfaces.UDPInterface.beacon may only be 0, 1,or 2"); } } while (0);
        return;
    }
    if (!beaconPort) {
        do { if (ctx->logger && Log_Level_ERROR >= Log_Level_DEBUG) { Log_print(ctx->logger, Log_Level_ERROR, "Configurator.c", 188, "interfaces.UDPInterface.beacon requires beaconPort"); } } while (0);
        return;
    }
    List* devices = Dict_getList(udp, (&(String) { .bytes = "beaconDevices", .len = CString_strlen("beaconDevices") }));
    if (!devices) {
        do { if (ctx->logger && Log_Level_ERROR >= Log_Level_DEBUG) { Log_print(ctx->logger, Log_Level_ERROR, "Configurator.c", 193, "interfaces.UDPInterface.beacon requires beaconDevices"); } } while (0);
        return;
    }


    do { if (ctx->logger && Log_Level_INFO >= Log_Level_DEBUG) { Log_print(ctx->logger, Log_Level_INFO, "Configurator.c", 198, "Setting beacon mode UDPInterface to [%d].", (int) beacon); } } while (0);

    Dict* d = Dict_new(ctx->alloc);
    Dict_putInt(d, String_new("state", ctx->alloc), beacon, ctx->alloc);
    Dict_putInt(d, String_new("interfaceNumber", ctx->alloc), ifNum, ctx->alloc);
    rpcCall((&(String) { .bytes = "UDPInterface_beacon", .len = CString_strlen("UDPInterface_beacon") }), d, ctx, ctx->alloc);

    d = Dict_new(ctx->alloc);
    Dict_putList(d, String_new("devices", ctx->alloc), devices, ctx->alloc);
    Dict_putInt(d, String_new("interfaceNumber", ctx->alloc), ifNum, ctx->alloc);
    rpcCall((&(String) { .bytes = "UDPInterface_setBroadcastDevices", .len = CString_strlen("UDPInterface_setBroadcastDevices") }), d, ctx, ctx->alloc);
}

static void udpInterface(Dict* config, struct Context* ctx)
{
    List* ifaces = Dict_getList(config, (&(String) { .bytes = "UDPInterface", .len = CString_strlen("UDPInterface") }));
    if (!ifaces) {
        ifaces = List_new(ctx->alloc);
        List_addDict(ifaces, Dict_getDict(config, (&(String) { .bytes = "UDPInterface", .len = CString_strlen("UDPInterface") })), ctx->alloc);
    }

    uint32_t count = List_size(ifaces);
    for (uint32_t i = 0; i < count; i++) {
        Dict *udp = List_getDict(ifaces, i);
        if (!udp) {
            continue;
        }

        String* bindStr = Dict_getString(udp, (&(String) { .bytes = "bind", .len = CString_strlen("bind") }));
        Dict* d = Dict_new(ctx->alloc);
        if (bindStr) {
            Dict_putString(d, String_new("bindAddress", ctx->alloc), bindStr, ctx->alloc);
        }
        int64_t* dscp = Dict_getInt(udp, (&(String) { .bytes = "dscp", .len = CString_strlen("dscp") }));
        if (dscp) {
            Dict_putInt(d, String_new("dscp", ctx->alloc), *dscp, ctx->alloc);
        }
        int64_t* beaconPort_p = Dict_getInt(udp, (&(String) { .bytes = "beaconPort", .len = CString_strlen("beaconPort") }));
        uint16_t beaconPort = (beaconPort_p) ? *beaconPort_p : 0;
        int64_t* beaconP = Dict_getInt(udp, (&(String) { .bytes = "beacon", .len = CString_strlen("beacon") }));
        int64_t beacon = (beaconP) ? *beaconP : 0;
        if (beacon && beaconPort) { Dict_putInt(d, String_new("beaconPort", ctx->alloc), beaconPort, ctx->alloc); }
        Dict* resp = 
# 240 "client/Configurator.c" 3 4
                    ((void *)0)
# 240 "client/Configurator.c"
                        ;
        rpcCall0((&(String) { .bytes = "UDPInterface_new", .len = CString_strlen("UDPInterface_new") }), d, ctx, ctx->alloc, &resp, 
# 241 "client/Configurator.c" 3 4
                                                                             1
# 241 "client/Configurator.c"
                                                                                 );
        int ifNum = *(Dict_getInt(resp, (&(String) { .bytes = "interfaceNumber", .len = CString_strlen("interfaceNumber") })));

        udpInterfaceSetBeacon(udp, beacon, beaconPort, ifNum, ctx);


        Dict* connectTo = Dict_getDict(udp, (&(String) { .bytes = "connectTo", .len = CString_strlen("connectTo") }));
        if (connectTo) {
            struct Dict_Entry* entry = *connectTo;
            struct Allocator* perCallAlloc = Allocator__child((ctx->alloc),"Configurator.c",250);
            while (entry != 
# 251 "client/Configurator.c" 3 4
                           ((void *)0)
# 251 "client/Configurator.c"
                               ) {
                String* key = (String*) entry->key;
                if (entry->val->type != Object_DICT) {
                    do { if (ctx->logger && Log_Level_CRITICAL >= Log_Level_DEBUG) { Log_print(ctx->logger, Log_Level_CRITICAL, "Configurator.c", 254, "interfaces.UDPInterface.connectTo: entry [%s] " "is not a dictionary type.", key->bytes); } } while (0)
                                                                                       ;
                    exit(-1);
                }
                Dict* value = entry->val->as.dictionary;
                do { if (ctx->logger && Log_Level_KEYS >= Log_Level_DEBUG) { Log_print(ctx->logger, Log_Level_KEYS, "Configurator.c", 259, "Attempting to connect to node [%s].", key->bytes); } } while (0);
                key = ((key) ? String_newBinary(key->bytes, key->len, perCallAlloc) : 
# 260 "client/Configurator.c" 3 4
                     ((void *)0)
# 260 "client/Configurator.c"
                     );
                char* lastColon = CString_strrchr(key->bytes, ':');

                if (lastColon) {
                    if (!Sockaddr_parse(key->bytes, 
# 264 "client/Configurator.c" 3 4
                                                   ((void *)0)
# 264 "client/Configurator.c"
                                                       )) {

                    } else {

                        do { if (ctx->logger && Log_Level_CRITICAL >= Log_Level_DEBUG) { Log_print(ctx->logger, Log_Level_CRITICAL, "Configurator.c", 268, "Couldn't add connection [%s], " "hostnames aren't supported.", key->bytes); } } while (0)
                                                                                              ;
                        exit(-1);
                    }
                } else {

                    do { if (ctx->logger && Log_Level_CRITICAL >= Log_Level_DEBUG) { Log_print(ctx->logger, Log_Level_CRITICAL, "Configurator.c", 274, "Connection [%s] must be $IP:$PORT, or " "[$IP]:$PORT for IPv6.", key->bytes); } } while (0)
                                                                                    ;
                    exit(-1);
                }
                Dict_putInt(value, String_new("interfaceNumber", perCallAlloc), ifNum, perCallAlloc);
                Dict_putString(value, String_new("address", perCallAlloc), key, perCallAlloc);
                rpcCall((&(String) { .bytes = "UDPInterface_beginConnection", .len = CString_strlen("UDPInterface_beginConnection") }), value, ctx, perCallAlloc);


                Dict* aed = Dict_new(perCallAlloc);
                *lastColon = '\0';
                Dict_putString(aed, String_new("route", perCallAlloc), String_new(key->bytes, perCallAlloc), perCallAlloc)
                                 ;
                *lastColon = ':';
                rpcCall((&(String) { .bytes = "RouteGen_addException", .len = CString_strlen("RouteGen_addException") }), aed, ctx, perCallAlloc);

                entry = entry->next;
            }
            Allocator__free((perCallAlloc),"Configurator.c",292);
        }
    }
}

static void tunInterface(Dict* ifaceConf, struct Allocator* tempAlloc, struct Context* ctx)
{
    String* ifaceType = Dict_getString(ifaceConf, (&(String) { .bytes = "type", .len = CString_strlen("type") }));
    if (!String_equals(ifaceType, (&(String) { .bytes = "TUNInterface", .len = CString_strlen("TUNInterface") }))) {
        return;
    }


    String* tunfd = Dict_getString(ifaceConf, (&(String) { .bytes = "tunfd", .len = CString_strlen("tunfd") }));
    String* device = Dict_getString(ifaceConf, (&(String) { .bytes = "tunDevice", .len = CString_strlen("tunDevice") }));

    Dict* args = Dict_new(tempAlloc);
    if (tunfd && device) {
        do { if (ctx->logger && Log_Level_WARN >= Log_Level_DEBUG) { Log_print(ctx->logger, Log_Level_WARN, "Configurator.c", 310, "tunfd is nolonger used, see Admin_importFd() and Core_initTunFd()"); } } while (0);
    } else {
        if (device) {
            Dict_putString(args, String_new("desiredTunName", tempAlloc), device, tempAlloc);
        }
        rpcCall0((&(String) { .bytes = "Core_initTunnel", .len = CString_strlen("Core_initTunnel") }), args, ctx, tempAlloc, 
# 315 "client/Configurator.c" 3 4
                                                                       ((void *)0)
# 315 "client/Configurator.c"
                                                                           , 
# 315 "client/Configurator.c" 3 4
                                                                             0
# 315 "client/Configurator.c"
                                                                                  );
    }
}

static void socketInterface(Dict* ifaceConf, struct Allocator* tempAlloc, struct Context* ctx)
{
    String* ifaceType = Dict_getString(ifaceConf, (&(String) { .bytes = "type", .len = CString_strlen("type") }));
    if (!String_equals(ifaceType, (&(String) { .bytes = "SocketInterface", .len = CString_strlen("SocketInterface") }))) {
        return;
    }


    String* socketFullPath = Dict_getString(ifaceConf, (&(String) { .bytes = "socketFullPath", .len = CString_strlen("socketFullPath") }));

    Dict* args = Dict_new(tempAlloc);
    if (!socketFullPath) {
        do { if (ctx->logger && Log_Level_CRITICAL >= Log_Level_DEBUG) { Log_print(ctx->logger, Log_Level_CRITICAL, "Configurator.c", 331, "In router.interface" " 'socketFullPath' is required if it's SocketInterface."); } } while (0)
                                                                                           ;
        exit(1);
    }

    if (Dict_getInt(ifaceConf, (&(String) { .bytes = "socketAttemptToCreate", .len = CString_strlen("socketAttemptToCreate") }))) {
        do { if (ctx->logger && Log_Level_WARN >= Log_Level_DEBUG) { Log_print(ctx->logger, Log_Level_WARN, "Configurator.c", 337, "SocketInterface \"socketAttemptToCreate\" nolonger has any " "effect, you must create the socket from the outside"); } } while (0)
                                                                  ;
    }

    Dict_putString(args, String_new("socketFullPath", tempAlloc), socketFullPath, tempAlloc);
    rpcCall0((&(String) { .bytes = "Core_initSocket", .len = CString_strlen("Core_initSocket") }), args, ctx, tempAlloc, 
# 342 "client/Configurator.c" 3 4
                                                                   ((void *)0)
# 342 "client/Configurator.c"
                                                                       , 
# 342 "client/Configurator.c" 3 4
                                                                         1
# 342 "client/Configurator.c"
                                                                             );
}

static void ipTunnel(Dict* ifaceConf, struct Allocator* tempAlloc, struct Context* ctx)
{
    List* incoming = Dict_getList(ifaceConf, (&(String) { .bytes = "allowedConnections", .len = CString_strlen("allowedConnections") }));
    if (incoming) {
        Dict* d;
        for (int i = 0; (d = List_getDict(incoming, i)) != 
# 350 "client/Configurator.c" 3 4
                                                          ((void *)0)
# 350 "client/Configurator.c"
                                                              ; i++) {
            String* key = Dict_getString(d, (&(String) { .bytes = "publicKey", .len = CString_strlen("publicKey") }));
            String* ip4 = Dict_getString(d, (&(String) { .bytes = "ip4Address", .len = CString_strlen("ip4Address") }));


            int64_t* ip4Prefix = Dict_getInt(d, (&(String) { .bytes = "ip4Prefix", .len = CString_strlen("ip4Prefix") }));
            String* ip6 = Dict_getString(d, (&(String) { .bytes = "ip6Address", .len = CString_strlen("ip6Address") }));
            int64_t* ip6Prefix = Dict_getInt(d, (&(String) { .bytes = "ip6Prefix", .len = CString_strlen("ip6Prefix") }));
            if (!key) {
                do { if (ctx->logger && Log_Level_CRITICAL >= Log_Level_DEBUG) { Log_print(ctx->logger, Log_Level_CRITICAL, "Configurator.c", 359, "In router.ipTunnel.allowedConnections[%d]" "'publicKey' required.", i); } } while (0)
                                                                     ;
                exit(1);
            }
            if (!ip4 && !ip6) {
                do { if (ctx->logger && Log_Level_CRITICAL >= Log_Level_DEBUG) { Log_print(ctx->logger, Log_Level_CRITICAL, "Configurator.c", 364, "In router.ipTunnel.allowedConnections[%d]" "either 'ip4Address' or 'ip6Address' required.", i); } } while (0)
                                                                                             ;
                exit(1);
            } else if (ip4Prefix && !ip4) {
                do { if (ctx->logger && Log_Level_CRITICAL >= Log_Level_DEBUG) { Log_print(ctx->logger, Log_Level_CRITICAL, "Configurator.c", 368, "In router.ipTunnel.allowedConnections[%d]" "'ip4Address' required with 'ip4Prefix'.", i); } } while (0)
                                                                                       ;
                exit(1);
            } else if (ip6Prefix && !ip6) {
                do { if (ctx->logger && Log_Level_CRITICAL >= Log_Level_DEBUG) { Log_print(ctx->logger, Log_Level_CRITICAL, "Configurator.c", 372, "In router.ipTunnel.allowedConnections[%d]" "'ip6Address' required with 'ip6Prefix'.", i); } } while (0)
                                                                                       ;
                exit(1);
            }
            do { if (ctx->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(ctx->logger, Log_Level_DEBUG, "Configurator.c", 376, "Allowing IpTunnel connections from [%s]", key->bytes); } } while (0);

            if (ip4) {
                do { if (ctx->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(ctx->logger, Log_Level_DEBUG, "Configurator.c", 379, "Issue IPv4 address %s", ip4->bytes); } } while (0);
                if (ip4Prefix) {
                    do { if (ctx->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(ctx->logger, Log_Level_DEBUG, "Configurator.c", 381, "Issue IPv4 netmask/prefix length /%d", (int) *ip4Prefix); } } while (0)
                                         ;
                } else {
                    do { if (ctx->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(ctx->logger, Log_Level_DEBUG, "Configurator.c", 384, "Use default netmask/prefix length /0"); } } while (0);
                }
            }

            if (ip6) {
                do { if (ctx->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(ctx->logger, Log_Level_DEBUG, "Configurator.c", 389, "Issue IPv6 address [%s]", ip6->bytes); } } while (0);
                if (ip6Prefix) {
                    do { if (ctx->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(ctx->logger, Log_Level_DEBUG, "Configurator.c", 391, "Issue IPv6 netmask/prefix length /%d", (int) *ip6Prefix); } } while (0)
                                         ;
                } else {
                    do { if (ctx->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(ctx->logger, Log_Level_DEBUG, "Configurator.c", 394, "Use default netmask/prefix length /0"); } } while (0);
                }
            }

            Dict_putString(d, String_new("publicKeyOfAuthorizedNode", tempAlloc), key, tempAlloc);
            rpcCall0((&(String) { .bytes = "IpTunnel_allowConnection", .len = CString_strlen("IpTunnel_allowConnection") }), d, ctx, tempAlloc, 
# 399 "client/Configurator.c" 3 4
                                                                                 ((void *)0)
# 399 "client/Configurator.c"
                                                                                     , 
# 399 "client/Configurator.c" 3 4
                                                                                       1
# 399 "client/Configurator.c"
                                                                                           );
        }
    }

    List* outgoing = Dict_getList(ifaceConf, (&(String) { .bytes = "outgoingConnections", .len = CString_strlen("outgoingConnections") }));
    if (outgoing) {
        String* s;
        for (int i = 0; (s = List_getString(outgoing, i)) != 
# 406 "client/Configurator.c" 3 4
                                                            ((void *)0)
# 406 "client/Configurator.c"
                                                                ; i++) {
            do { if (ctx->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(ctx->logger, Log_Level_DEBUG, "Configurator.c", 407, "Initiating IpTunnel connection to [%s]", s->bytes); } } while (0);
            Dict requestDict =
                (&(struct Dict_Entry) { .key = (&(String) { .bytes = "publicKeyOfNodeToConnectTo", .len = CString_strlen("publicKeyOfNodeToConnectTo") }), .val = (&(Object) { .type = Object_STRING, .as.string = s }), .next = 
# 409 "client/Configurator.c" 3 4
               ((void *)0) 
# 409 "client/Configurator.c"
               });
            rpcCall0((&(String) { .bytes = "IpTunnel_connectTo", .len = CString_strlen("IpTunnel_connectTo") }), &requestDict, ctx, tempAlloc, 
# 410 "client/Configurator.c" 3 4
                                                                                      ((void *)0)
# 410 "client/Configurator.c"
                                                                                          , 
# 410 "client/Configurator.c" 3 4
                                                                                            1
# 410 "client/Configurator.c"
                                                                                                );
        }
    }
}

static void supernodes(List* supernodes, struct Allocator* tempAlloc, struct Context* ctx)
{
    if (!supernodes) { return; }
    String* s;
    for (int i = 0; (s = List_getString(supernodes, i)) != 
# 419 "client/Configurator.c" 3 4
                                                          ((void *)0)
# 419 "client/Configurator.c"
                                                              ; i++) {
        do { if (ctx->logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(ctx->logger, Log_Level_DEBUG, "Configurator.c", 420, "Loading supernode connection to [%s]", s->bytes); } } while (0);
        Dict reqDict = (&(struct Dict_Entry) { .key = (&(String) { .bytes = "key", .len = CString_strlen("key") }), .val = (&(Object) { .type = Object_STRING, .as.string = s }), .next = 
# 421 "client/Configurator.c" 3 4
                      ((void *)0) 
# 421 "client/Configurator.c"
                      });
        rpcCall0((&(String) { .bytes = "SupernodeHunter_addSnode", .len = CString_strlen("SupernodeHunter_addSnode") }), &reqDict, ctx, tempAlloc, 
# 422 "client/Configurator.c" 3 4
                                                                                    ((void *)0)
# 422 "client/Configurator.c"
                                                                                        , 
# 422 "client/Configurator.c" 3 4
                                                                                          1
# 422 "client/Configurator.c"
                                                                                              );
    }
}

static void routerConfig(Dict* routerConf, struct Allocator* tempAlloc, struct Context* ctx)
{
    tunInterface(Dict_getDict(routerConf, (&(String) { .bytes = "interface", .len = CString_strlen("interface") })), tempAlloc, ctx);
    socketInterface(Dict_getDict(routerConf, (&(String) { .bytes = "interface", .len = CString_strlen("interface") })), tempAlloc, ctx);
    ipTunnel(Dict_getDict(routerConf, (&(String) { .bytes = "ipTunnel", .len = CString_strlen("ipTunnel") })), tempAlloc, ctx);
    supernodes(Dict_getList(routerConf, (&(String) { .bytes = "supernodes", .len = CString_strlen("supernodes") })), tempAlloc, ctx);
}

static void ethInterfaceSetBeacon(int ifNum, Dict* eth, struct Context* ctx)
{
    int64_t* beaconP = Dict_getInt(eth, (&(String) { .bytes = "beacon", .len = CString_strlen("beacon") }));
    if (beaconP) {
        int64_t beacon = *beaconP;
        if (beacon > 2 || beacon < 0) {
            do { if (ctx->logger && Log_Level_ERROR >= Log_Level_DEBUG) { Log_print(ctx->logger, Log_Level_ERROR, "Configurator.c", 440, "interfaces.ETHInterface.beacon may only be 0, 1,or 2"); } } while (0);
        } else {

            do { if (ctx->logger && Log_Level_INFO >= Log_Level_DEBUG) { Log_print(ctx->logger, Log_Level_INFO, "Configurator.c", 443, "Setting beacon mode on ETHInterface to [%d].", (int) beacon); } } while (0);
            Dict d = (&(struct Dict_Entry) { .key = (&(String) { .bytes = "interfaceNumber", .len = CString_strlen("interfaceNumber") }), .val = (&(Object) { .type = Object_INTEGER, .as.number = ifNum }), .next = (&(struct Dict_Entry) { .key = (&(String) { .bytes = "state", .len = CString_strlen("state") }), .val = (&(Object) { .type = Object_INTEGER, .as.number = beacon }), .next = 
# 444 "client/Configurator.c" 3 4
                    ((void *)0) 
# 444 "client/Configurator.c"
                    }) })
                                                                              ;
            rpcCall((&(String) { .bytes = "ETHInterface_beacon", .len = CString_strlen("ETHInterface_beacon") }), &d, ctx, ctx->alloc);
        }
    }
}

static void ethInterface(Dict* config, struct Context* ctx)
{
    List* ifaces = Dict_getList(config, (&(String) { .bytes = "ETHInterface", .len = CString_strlen("ETHInterface") }));
    if (!ifaces) {
        ifaces = List_new(ctx->alloc);
        List_addDict(ifaces, Dict_getDict(config, (&(String) { .bytes = "ETHInterface", .len = CString_strlen("ETHInterface") })), ctx->alloc);
    }

    uint32_t count = List_size(ifaces);

    for (uint32_t i = 0; i < count; i++) {
        Dict *eth = List_getDict(ifaces, i);
        if (!eth) { continue; }
        String* deviceStr = Dict_getString(eth, (&(String) { .bytes = "bind", .len = CString_strlen("bind") }));
        if (!deviceStr || !String_equals((&(String) { .bytes = "all", .len = CString_strlen("all") }), deviceStr)) { continue; }
        do { if (ctx->logger && Log_Level_INFO >= Log_Level_DEBUG) { Log_print(ctx->logger, Log_Level_INFO, "Configurator.c", 466, "Setting up all ETHInterfaces..."); } } while (0);
        Dict* res = 
# 467 "client/Configurator.c" 3 4
                   ((void *)0)
# 467 "client/Configurator.c"
                       ;
        Dict* d = Dict_new(ctx->alloc);
        if (rpcCall0((&(String) { .bytes = "ETHInterface_listDevices", .len = CString_strlen("ETHInterface_listDevices") }), d, ctx, ctx->alloc, &res, 
# 469 "client/Configurator.c" 3 4
                                                                                        0
# 469 "client/Configurator.c"
                                                                                             )) {
            do { if (ctx->logger && Log_Level_INFO >= Log_Level_DEBUG) { Log_print(ctx->logger, Log_Level_INFO, "Configurator.c", 470, "Getting device list failed"); } } while (0);
            break;
        }
        List* devs = Dict_getList(res, (&(String) { .bytes = "devices", .len = CString_strlen("devices") }));
        uint32_t devCount = List_size(devs);
        for (uint32_t j = 0; j < devCount; j++) {
            Dict* d = Dict_new(ctx->alloc);
            String* deviceName = List_getString(devs, j);

            if (String_equals((&(String) { .bytes = "lo", .len = CString_strlen("lo") }), deviceName)) { continue; }
            Dict_putString(d, String_new("bindDevice", ctx->alloc), deviceName, ctx->alloc);
            Dict* resp;
            do { if (ctx->logger && Log_Level_INFO >= Log_Level_DEBUG) { Log_print(ctx->logger, Log_Level_INFO, "Configurator.c", 482, "Creating new ETHInterface [%s]", deviceName->bytes); } } while (0);
            if (rpcCall0((&(String) { .bytes = "ETHInterface_new", .len = CString_strlen("ETHInterface_new") }), d, ctx, ctx->alloc, &resp, 
# 483 "client/Configurator.c" 3 4
                                                                                     0
# 483 "client/Configurator.c"
                                                                                          )) {
                do { if (ctx->logger && Log_Level_WARN >= Log_Level_DEBUG) { Log_print(ctx->logger, Log_Level_WARN, "Configurator.c", 484, "Failed to create ETHInterface."); } } while (0);
                continue;
            }
            int ifNum = *(Dict_getInt(resp, (&(String) { .bytes = "interfaceNumber", .len = CString_strlen("interfaceNumber") })));
            ethInterfaceSetBeacon(ifNum, eth, ctx);
        }
        return;
    }

    for (uint32_t i = 0; i < count; i++) {
        Dict *eth = List_getDict(ifaces, i);
        if (!eth) { continue; }

        String* deviceStr = Dict_getString(eth, (&(String) { .bytes = "bind", .len = CString_strlen("bind") }));
        do { if (ctx->logger && Log_Level_INFO >= Log_Level_DEBUG) { Log_print(ctx->logger, Log_Level_INFO, "Configurator.c", 498, "Setting up ETHInterface [%d].", i); } } while (0);
        Dict* d = Dict_new(ctx->alloc);
        if (deviceStr) {
            do { if (ctx->logger && Log_Level_INFO >= Log_Level_DEBUG) { Log_print(ctx->logger, Log_Level_INFO, "Configurator.c", 501, "Binding to device [%s].", deviceStr->bytes); } } while (0);
            Dict_putString(d, String_new("bindDevice", ctx->alloc), deviceStr, ctx->alloc);
        }
        Dict* resp = 
# 504 "client/Configurator.c" 3 4
                    ((void *)0)
# 504 "client/Configurator.c"
                        ;
        if (rpcCall0((&(String) { .bytes = "ETHInterface_new", .len = CString_strlen("ETHInterface_new") }), d, ctx, ctx->alloc, &resp, 
# 505 "client/Configurator.c" 3 4
                                                                                 0
# 505 "client/Configurator.c"
                                                                                      )) {
            do { if (ctx->logger && Log_Level_WARN >= Log_Level_DEBUG) { Log_print(ctx->logger, Log_Level_WARN, "Configurator.c", 506, "Failed to create ETHInterface."); } } while (0);
            continue;
        }
        int ifNum = *(Dict_getInt(resp, (&(String) { .bytes = "interfaceNumber", .len = CString_strlen("interfaceNumber") })));
        ethInterfaceSetBeacon(ifNum, eth, ctx);


        Dict* connectTo = Dict_getDict(eth, (&(String) { .bytes = "connectTo", .len = CString_strlen("connectTo") }));
        if (connectTo) {
            do { if (ctx->logger && Log_Level_INFO >= Log_Level_DEBUG) { Log_print(ctx->logger, Log_Level_INFO, "Configurator.c", 515, "ETHInterface should connect to a specific node."); } } while (0);
            struct Dict_Entry* entry = *connectTo;
            while (entry != 
# 517 "client/Configurator.c" 3 4
                           ((void *)0)
# 517 "client/Configurator.c"
                               ) {
                String* key = (String*) entry->key;
                if (entry->val->type != Object_DICT) {
                    do { if (ctx->logger && Log_Level_CRITICAL >= Log_Level_DEBUG) { Log_print(ctx->logger, Log_Level_CRITICAL, "Configurator.c", 520, "interfaces.ETHInterface.connectTo: entry [%s] " "is not a dictionary type.", key->bytes); } } while (0)
                                                                                       ;
                    exit(-1);
                }
                Dict* value = entry->val->as.dictionary;

                do { if (ctx->logger && Log_Level_KEYS >= Log_Level_DEBUG) { Log_print(ctx->logger, Log_Level_KEYS, "Configurator.c", 526, "Attempting to connect to node [%s].", key->bytes); } } while (0);

                struct Allocator* perCallAlloc = Allocator__child((ctx->alloc),"Configurator.c",528);


                Dict_putString(value, String_new("macAddress", perCallAlloc), key, perCallAlloc);
                Dict_putInt(value, String_new("interfaceNumber", perCallAlloc), ifNum, perCallAlloc);
                rpcCall((&(String) { .bytes = "ETHInterface_beginConnection", .len = CString_strlen("ETHInterface_beginConnection") }), value, ctx, perCallAlloc);
                Allocator__free((perCallAlloc),"Configurator.c",534);

                entry = entry->next;
            }
        }
    }
}

static void security(struct Allocator* tempAlloc, List* conf, struct Log* log, struct Context* ctx)
{
    int seccomp = 1;
    int nofiles = 0;
    int noforks = 1;
    int chroot = 1;
    int setupComplete = 1;
    int setuser = 1;
    if (0 || 1) {
        setuser = 0;
    }

    int uid = -1;
    int64_t* group = 
# 555 "client/Configurator.c" 3 4
                    ((void *)0)
# 555 "client/Configurator.c"
                        ;
    int keepNetAdmin = 1;

    do {
        Dict* d = Dict_new(tempAlloc);
        Dict_putString(d, String_new("user", tempAlloc), String_new("nobody", tempAlloc), tempAlloc);
        if (!0 && !1) {
            Dict* ret = 
# 562 "client/Configurator.c" 3 4
                       ((void *)0)
# 562 "client/Configurator.c"
                           ;
            int r = rpcCall0((&(String) { .bytes = "Security_getUser", .len = CString_strlen("Security_getUser") }), d, ctx, tempAlloc, &ret, 
# 563 "client/Configurator.c" 3 4
                                                                                       0
# 563 "client/Configurator.c"
                                                                                            );
            if (!r) {
                uid = *Dict_getInt(ret, (&(String) { .bytes = "uid", .len = CString_strlen("uid") }));
                group = Dict_getInt(ret, (&(String) { .bytes = "gid", .len = CString_strlen("gid") }));
            }
        }
    } while (0);

    for (int i = 0; conf && i < List_size(conf); i++) {
        Dict* elem = List_getDict(conf, i);
        String* s;
        if (elem && (s = Dict_getString(elem, (&(String) { .bytes = "setuser", .len = CString_strlen("setuser") })))) {
            if (setuser == 0) { continue; }
            Dict* d = Dict_new(tempAlloc);
            Dict_putString(d, String_new("user", tempAlloc), s, tempAlloc);
            Dict* ret = 
# 578 "client/Configurator.c" 3 4
                       ((void *)0)
# 578 "client/Configurator.c"
                           ;
            rpcCall0((&(String) { .bytes = "Security_getUser", .len = CString_strlen("Security_getUser") }), d, ctx, tempAlloc, &ret, 
# 579 "client/Configurator.c" 3 4
                                                                               1
# 579 "client/Configurator.c"
                                                                                   );
            uid = *Dict_getInt(ret, (&(String) { .bytes = "uid", .len = CString_strlen("uid") }));
            group = Dict_getInt(ret, (&(String) { .bytes = "gid", .len = CString_strlen("gid") }));
            int64_t* nka = Dict_getInt(elem, (&(String) { .bytes = "keepNetAdmin", .len = CString_strlen("keepNetAdmin") }));
            int64_t* exemptAngel = Dict_getInt(elem, (&(String) { .bytes = "exemptAngel", .len = CString_strlen("exemptAngel") }));
            keepNetAdmin = ((nka) ? *nka : ((exemptAngel) ? *exemptAngel : 0));
            continue;
        }
        if (elem && (s = Dict_getString(elem, (&(String) { .bytes = "chroot", .len = CString_strlen("chroot") })))) {
            do { if (log && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(log, Log_Level_DEBUG, "Configurator.c", 588, "Security_chroot(%s)", s->bytes); } } while (0);
            Dict* d = Dict_new(tempAlloc);
            Dict_putString(d, String_new("root", tempAlloc), s, tempAlloc);
            rpcCall0((&(String) { .bytes = "Security_chroot", .len = CString_strlen("Security_chroot") }), d, ctx, tempAlloc, 
# 591 "client/Configurator.c" 3 4
                                                                        ((void *)0)
# 591 "client/Configurator.c"
                                                                            , 
# 591 "client/Configurator.c" 3 4
                                                                              0
# 591 "client/Configurator.c"
                                                                                   );
            chroot = 0;
            continue;
        }
        uint64_t* x;
        if (elem && (x = Dict_getInt(elem, (&(String) { .bytes = "nofiles", .len = CString_strlen("nofiles") })))) {
            if (!*x) { continue; }
            nofiles = 1;
            continue;
        }
        if (elem && (x = Dict_getInt(elem, (&(String) { .bytes = "setuser", .len = CString_strlen("setuser") })))) {
            if (!*x) { setuser = 0; }
            continue;
        }
        if (elem && (x = Dict_getInt(elem, (&(String) { .bytes = "seccomp", .len = CString_strlen("seccomp") })))) {
            if (!*x) { seccomp = 0; }
            continue;
        }
        if (elem && (x = Dict_getInt(elem, (&(String) { .bytes = "noforks", .len = CString_strlen("noforks") })))) {
            if (!*x) { noforks = 0; }
            continue;
        }
        if (elem && (x = Dict_getInt(elem, (&(String) { .bytes = "chroot", .len = CString_strlen("chroot") })))) {
            if (!*x) { chroot = 0; }
            continue;
        }
        if (elem && (x = Dict_getInt(elem, (&(String) { .bytes = "setupComplete", .len = CString_strlen("setupComplete") })))) {
            if (!*x) { setupComplete = 0; }
            continue;
        }
        do { if (ctx->logger && Log_Level_INFO >= Log_Level_DEBUG) { Log_print(ctx->logger, Log_Level_INFO, "Configurator.c", 621, "Unrecognized entry in security at index [%d]", i); } } while (0);
    }

    if (!0 && !1) {
        if (uid == -1) {
            do { if (ctx->logger && Log_Level_CRITICAL >= Log_Level_DEBUG) { Log_print(ctx->logger, Log_Level_CRITICAL, "Configurator.c", 626, "User \"nobody\" doesn't exist and no alternative user is set"); } } while (0)
                                                                               ;
            die(ctx->currentResult, ctx, tempAlloc);
        }
    }

    if (chroot) {
        do { if (log && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(log, Log_Level_DEBUG, "Configurator.c", 633, "Security_chroot(/var/run)"); } } while (0);
        Dict* d = Dict_new(tempAlloc);
        Dict_putString(d, String_new("root", tempAlloc), String_new("/var/run/", tempAlloc), tempAlloc);
        rpcCall0((&(String) { .bytes = "Security_chroot", .len = CString_strlen("Security_chroot") }), d, ctx, tempAlloc, 
# 636 "client/Configurator.c" 3 4
                                                                    ((void *)0)
# 636 "client/Configurator.c"
                                                                        , 
# 636 "client/Configurator.c" 3 4
                                                                          0
# 636 "client/Configurator.c"
                                                                               );
    }


    if (noforks) {
        do { if (log && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(log, Log_Level_DEBUG, "Configurator.c", 641, "Security_noforks()"); } } while (0);
        Dict* d = Dict_new(tempAlloc);
        rpcCall((&(String) { .bytes = "Security_noforks", .len = CString_strlen("Security_noforks") }), d, ctx, tempAlloc);
    }
    if (setuser) {
        do { if (log && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(log, Log_Level_DEBUG, "Configurator.c", 646, "Security_setUser(uid:%d, keepNetAdmin:%d)", uid, keepNetAdmin); } } while (0);
        Dict* d = Dict_new(tempAlloc);
        Dict_putInt(d, String_new("uid", tempAlloc), uid, tempAlloc);
        if (group) {
            Dict_putInt(d, String_new("gid", tempAlloc), (int)*group, tempAlloc);
        }
        Dict_putInt(d, String_new("keepNetAdmin", tempAlloc), keepNetAdmin, tempAlloc);
        rpcCall0((&(String) { .bytes = "Security_setUser", .len = CString_strlen("Security_setUser") }), d, ctx, tempAlloc, 
# 653 "client/Configurator.c" 3 4
                                                                     ((void *)0)
# 653 "client/Configurator.c"
                                                                         , 
# 653 "client/Configurator.c" 3 4
                                                                           0
# 653 "client/Configurator.c"
                                                                                );
    }
    if (nofiles) {
        do { if (log && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(log, Log_Level_DEBUG, "Configurator.c", 656, "Security_nofiles()"); } } while (0);
        Dict* d = Dict_new(tempAlloc);
        rpcCall((&(String) { .bytes = "Security_nofiles", .len = CString_strlen("Security_nofiles") }), d, ctx, tempAlloc);
    }
    if (seccomp) {
        do { if (log && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(log, Log_Level_DEBUG, "Configurator.c", 661, "Security_seccomp()"); } } while (0);
        Dict* d = Dict_new(tempAlloc);
        rpcCall((&(String) { .bytes = "Security_seccomp", .len = CString_strlen("Security_seccomp") }), d, ctx, tempAlloc);
    }
    if (setupComplete) {
        do { if (log && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(log, Log_Level_DEBUG, "Configurator.c", 666, "Security_setupComplete()"); } } while (0);
        Dict* d = Dict_new(tempAlloc);
        rpcCall((&(String) { .bytes = "Security_setupComplete", .len = CString_strlen("Security_setupComplete") }), d, ctx, tempAlloc);
    }
}

static int tryPing(struct Allocator* tempAlloc, struct Context* ctx)
{
    Dict* resp = 
# 674 "client/Configurator.c" 3 4
                ((void *)0)
# 674 "client/Configurator.c"
                    ;
    Dict* d = Dict_new(tempAlloc);
    rpcCall0((&(String) { .bytes = "ping", .len = CString_strlen("ping") }), d, ctx, tempAlloc, &resp, 
# 676 "client/Configurator.c" 3 4
                                                            0
# 676 "client/Configurator.c"
                                                                 );
    if (!resp) { return -1; }
    String* q = Dict_getString(resp, (&(String) { .bytes = "q", .len = CString_strlen("q") }));
    if (String_equals(q, (&(String) { .bytes = "pong", .len = CString_strlen("pong") }))) {
        return 
# 680 "client/Configurator.c" 3 4
              1
# 680 "client/Configurator.c"
                  ;
    }
    return 
# 682 "client/Configurator.c" 3 4
          0
# 682 "client/Configurator.c"
               ;
}

static void awaken(void* vcontext)
{
    struct Context* ctx = vcontext;
    EventBase_endLoop(ctx->base);
}

static void sleep(int milliseconds, struct Context* ctx, struct Allocator* temp)
{
    Timeout__setTimeout((awaken), (ctx), (milliseconds), (ctx->base), (temp), "Configurator.c", 693);
    EventBase_beginLoop(ctx->base);
}

static void waitUntilPong(struct Context* ctx)
{
    for (int i = 0; i < 10; i++) {
        struct Allocator* temp = Allocator__child((ctx->alloc),"Configurator.c",700);
        if (tryPing(temp, ctx)) {
            Allocator__free((temp),"Configurator.c",702);
            return;
        }
        sleep(200, ctx, temp);
        Allocator__free((temp),"Configurator.c",706);
    }
    Assert_failure("Failed connecting to core (perhaps you have a firewall on loopback device?)");
}

void Configurator_config(Dict* config,
                         struct Sockaddr* sockAddr,
                         String* adminPassword,
                         struct EventBase* eventBase,
                         struct Log* logger,
                         struct Allocator* alloc)
{
    struct Allocator* tempAlloc = Allocator__child((alloc),"Configurator.c",718);
    struct UDPAddrIface* udp = (__extension__({             struct UDPAddrIface* Er_returnVal;             __builtin_memset(&Er_returnVal, 0, sizeof(Er_returnVal));             struct Er_Ret* Er_ret = UDPAddrIface_new(&Er_returnVal, eventBase,                               ((void *)0)                              , alloc, logger);                          if (Er_ret) {                 struct Er_Ret** Er_errOut = (struct Er_Ret**)0;                 if (Er_errOut) {                     *Er_errOut = Er_unwind("Configurator.c", 719, Er_ret);                 } else {                     Er__assertFail(Er_unwind("Configurator.c", 719, Er_ret));                 }             }                      Er_returnVal;         }));
    struct AdminClient* client =
        AdminClient_new(&udp->generic, sockAddr, adminPassword, eventBase, logger, tempAlloc);

    struct Context ctx = {
        .logger = logger,
        .alloc = tempAlloc,
        .client = client,
        .base = eventBase,
    };

    waitUntilPong(&ctx);

    List* authedPasswords = Dict_getList(config, (&(String) { .bytes = "authorizedPasswords", .len = CString_strlen("authorizedPasswords") }));
    if (authedPasswords) {
        authorizedPasswords(authedPasswords, &ctx);
    }

    Dict* ifaces = Dict_getDict(config, (&(String) { .bytes = "interfaces", .len = CString_strlen("interfaces") }));
    udpInterface(ifaces, &ctx);

    if (0) {
        ethInterface(ifaces, &ctx);
    }

    Dict* routerConf = Dict_getDict(config, (&(String) { .bytes = "router", .len = CString_strlen("router") }));
    routerConfig(routerConf, tempAlloc, &ctx);

    List* secList = Dict_getList(config, (&(String) { .bytes = "security", .len = CString_strlen("security") }));
    security(tempAlloc, secList, logger, &ctx);

    do { if (logger && Log_Level_DEBUG >= Log_Level_DEBUG) { Log_print(logger, Log_Level_DEBUG, "Configurator.c", 750, "Cjdns started in the background"); } } while (0);

    Allocator__free((tempAlloc),"Configurator.c",752);
}
