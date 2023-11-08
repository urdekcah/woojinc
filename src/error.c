#include <stdio.h>
#include <stdarg.h>
#include <locale.h>
#include <errno.h>
#include <wchar.h>
#include <stdlib.h>
#include <string.h>
#if !defined(__linux__)
#include <Windows.h>
#endif
#include "error.h"

void errExit(bool detail, const wchar_t* format, ...) {
  va_list args;
  va_start(args, format);
  if (detail) lastError();
  #if !defined(__linux__)
  fwprintf_s(stderr, L"\x1b[31m%s\x1b[0m: ", (wchar_t*)E_PREFIX);
  vfwprintf(stderr, format, args);
  #else
  wprintf(L"\x1b[31m%s\x1b[0m: ", (wchar_t*)E_PREFIX);
  vfwprintf(stderr, format, args);
  #endif
  va_end(args);
  exit(1);
}

void lastError() {
  #if !defined(__linux__)
  wchar_t* message = NULL;
  FormatMessageW(
    FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
    NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPWSTR)&message, 0, NULL
  );
  if (message) {
    fwprintf_s(stderr, L"\x1b[31m%s\x1b[0m: %s", (wchar_t*)E_PREFIX, message);
    LocalFree(message);
  }
  #else
  const wchar_t *message = (const wchar_t *)strerror(errno);
  if (message) wprintf(L"\x1b[31m%s\x1b[0m: %ls\n", E_PREFIX, message);
  #endif
}
