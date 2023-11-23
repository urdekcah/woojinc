#ifndef __WOOJINC_SCOPE_H__
#define __WOOJINC_SCOPE_H__
#include <stdbool.h>
#include "woojin.h"
#include "variable.h"
#include "parse.h"
#include "list.h"
#include "map.h"

enum ScopeObjectKind {
  ScopeVarObject,
};

struct ScopeObject {
  union {
    w__ast__Var _w__ast__Var;
  };
  char* name;
  ScopeObjectKind kind;
};

struct Scope {
  StringScopeObjectMap* objects;
  Scope* parent;
  bool detached_fparen;
  Array_Scope_ptr* children;
};

void w__parser__Parser_open_scope(w__parser__Parser* p);
void w__parser__Parser_close_scope(w__parser__Parser* p);
void w__ast__Scope_register(Scope* s, ScopeObject* obj);
void w__ast__Scope_free(Scope* _a1);
#endif