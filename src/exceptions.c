#include "exception.h"
#include "error.h"

ExceTable exctable[] = {
  {"E01", WoojinExce_UnresolvedName, ERM_UNRESOL_NAME, NULL},
  {NULL}
};

void runtime_err(const wchar_t* format, ...) {
  va_list args;
  va_start(args, format);
  fwprintf_s(stderr, L"\x1b[31m%s\x1b[0m: ", (wchar_t*)ERM_PRIFIX);
  vfwprintf(stderr, format, args);
  va_end(args);
  exit(1);
}