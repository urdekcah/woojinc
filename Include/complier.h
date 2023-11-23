#ifndef __WOOJINC_COMPLIER_H__
#define __WOOJINC_COMPLIER_H__
#include <stdbool.h>
#include "woojin.h"

struct ComplierOption {
  bool korean;
};

ComplierOption* w__complier__option_parse(int argc, char** argv);
ComplierOption* w__complier__option_default();
#endif