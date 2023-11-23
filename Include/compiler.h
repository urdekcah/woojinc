#ifndef __WOOJINC_COMPILER_H__
#define __WOOJINC_COMPILER_H__
#include <stdbool.h>
#include "woojin.h"

struct CompilerOption {
  bool korean;
};

CompilerOption* w__complier__option_parse(int argc, char** argv);
CompilerOption* w__complier__option_default();
#endif