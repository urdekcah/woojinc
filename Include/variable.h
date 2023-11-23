#ifndef __WOOJINC_VARIABLE_H__
#define __WOOJINC_VARIABLE_H__
#include <stdbool.h>
#include "woojin.h"
#include "expr.h"

struct w__ast__Var {
  char* name;
  w__ast__Expr expr;
  type_t type;
  bool mut, used, changed, tmp;
};
#endif