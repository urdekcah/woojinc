#ifndef __WOOJINC_AST_H__
#define __WOOJINC_AST_H__
#include <stdbool.h>
#include "woojin.h"

enum w__ast__IdentKind {
  IDENT_VAR,
};

struct w__ast__IdentVar {
  type_t type;
  bool mut;
  bool _static;
  bool _volatile: true;
  bool option;
};

struct w__ast__IdentInfo {
  union {
    w__ast__IdentVar _w__ast__IdentVar;
  };
  w__ast__IdentKind kind;
};
#endif