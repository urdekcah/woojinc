#ifndef __WOOJINC_CONFIG_H__
#define __WOOJINC_CONFIG_H__
#include <io.h>
#include <stdbool.h>

#ifdef _MSC_VER // Microsoft Visual C++ Compiler
#define MS_WINDOWS
#ifdef _WIN64
#define MS_WIN64
#endif
#ifdef _WIN32
#define MS_WIN32
#endif
#ifdef _WIN16
#define MS_WIN16
#endif
#ifdef MS_WIN64
#ifdef _M_IX86
#define COMPILER "[MSC 64 bit (Intel)]"
#elif defined(_M_ALPHA)
#define COMPILER "[MSC 64 bit (Alpha)]"
#else
#define COMPILER "[MSC 64 bit (Unknown)]"
#endif
#endif
#if defined(MS_WIN32) && !defined(MS_WIN64)
#ifdef _M_IX86
#define COMPILER "[MSC 32 bit (Intel)]"
#elif defined(_M_ALPHA)
#define COMPILER "[MSC 32 bit (Alpha)]"
#else
#define COMPILER "[MSC (Unknown)]"
#endif
#endif
#endif

#if defined(_MSC_VER) && _MSC_VER > 850
#ifndef MS_NO_COREDLL
#define MS_COREDLL
#ifndef USE_DL_EXPORT
#define USE_DL_IMPORT
#endif
#endif
#define PYTHONPATH ".\\DLLs;.\\lib;.\\lib\\plat-win;.\\lib\\lib-tk"
#define WORD_BIT 32
#pragma warning(disable:4113)
#include <stdio.h>
#ifndef NETSCAPE_PI
#define USE_SOCKET
#endif
#ifdef USE_DL_IMPORT
#define DL_IMPORT(RTYPE) __declspec(dllimport) RTYPE
#endif
#ifdef USE_DL_EXPORT
#define DL_IMPORT(RTYPE) __declspec(dllexport) RTYPE
#define DL_EXPORT(RTYPE) __declspec(dllexport) RTYPE
#endif
#endif

#if defined(__GNUC__) && defined(_WIN32)
#if (__GNUC__==2) && (__GNUC_MINOR__<=91)
#warning "Please use an up-to-date version of gcc! (>2.91 recommended)"
#endif
#define MS_WIN32
#define MS_WINDOWS
#ifndef MS_NO_COREDLL
#define MS_COREDLL	/* Python core is in a DLL */
#ifndef USE_DL_EXPORT
#define USE_DL_IMPORT
#endif /* !USE_DL_EXPORT */
#endif /* !MS_NO_COREDLL */

#define COMPILER "[gcc]"
#define PYTHONPATH ".\\DLLs;.\\lib;.\\lib\\plat-win;.\\lib\\lib-tk"
#define WORD_BIT 32
#include <stdio.h>
#define HAVE_CLOCK
#define HAVE_STRFTIME
#define HAVE_STRERROR
#define NT_THREADS
#define WITH_THREAD
#ifndef NETSCAPE_PI
#define USE_SOCKET
#endif
#ifdef USE_DL_IMPORT
#define DL_IMPORT(RTYPE) __declspec(dllimport) RTYPE
#endif
#ifdef USE_DL_EXPORT
#define DL_IMPORT(RTYPE) __declspec(dllexport) RTYPE
#define DL_EXPORT(RTYPE) __declspec(dllexport) RTYPE
#endif

#define HAVE_LONG_LONG 1
#define LONG_LONG long long 
#endif /* GNUC */


typedef signed char i8;
typedef short i16;
typedef int i32;
typedef long i64;
#ifdef __WOOJINC_64BIT__
  typedef __int128_t i128;
#endif
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long u64;
#ifdef __WOOJINC_64BIT__
  typedef __uint128_t u128;
#endif
typedef float f32;
typedef double f64;
typedef long double f128;
typedef bool _bool;
typedef char _char;
typedef char* str;

// typedef enum wjtype {
//   WJTYPE_i8,
//   WJTYPE_i16,
//   WJTYPE_i32,
//   WJTYPE_i64,
//   WJTYPE_i128,
//   WJTYPE_u8,
//   WJTYPE_u16,
//   WJTYPE_u32,
//   WJTYPE_u64,
//   WJTYPE_u128,
//   WJTYPE_f32,
//   WJTYPE_f64,
//   WJTYPE_bool,
//   WJTYPE_f128,
//   WJTYPE_char,
//   WJTYPE_str,
//   WJTYPE_arr,
//   WJTYPE_map,
//   WJTYPE_stmt,
//   WJTYPE_expr,
//   WJTYPE_fn,
//   WJTYPE_builtin,
//   WJTYPE_exception,
//   WJTYPE_null,
// } WJType;

typedef enum PrimitiveCoreType {
  CORETYPE_ANY, /* internal only */
  CORETYPE_UNKNOWN,
  CORETYPE_BOOL,
  CORETYPE_CHAR,
  CORETYPE_STR,
  CORETYPE_F32,
  CORETYPE_F64,
  CORETYPE_F128,
  CORETYPE_I8,
  CORETYPE_I16,
  CORETYPE_I32,
  CORETYPE_I64,
  CORETYPE_I128,
  CORETYPE_U8,
  CORETYPE_U16,
  CORETYPE_U32,
  CORETYPE_U64,
  CORETYPE_U128,
  CORETYPE_ARRAY,
  CORETYPE_OBJECT,
  CORETYPE_EXPR, /* internal only */
  CORETYPE_STATEMENT, /* internal only */
  CORETYPE_FUNCTION,
  CORETYPE_BUILTIN,
  CORETYPE_EXCEPTION,
  CORETYPE_NULL,
} PrimitiveCoreType;

#endif