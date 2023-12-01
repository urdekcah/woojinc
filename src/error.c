
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdbool.h>
#include <locale.h>
#include "error.h"

void errExit(const wchar_t* format, ...) {
  va_list args;
  va_start(args, format);
  fwprintf_s(stderr, L"\x1b[31m%s\x1b[0m: ", (wchar_t*)EMSG_PRIFIX);
  vfwprintf(stderr, format, args);
  va_end(args);
  exit(1);
}

bool berrExit(const wchar_t* format, ...) {
  va_list args;
  va_start(args, format);
  errExit(format, args);
  va_end(args);
  return false;
}