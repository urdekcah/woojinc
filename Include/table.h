#ifndef __WOOJINC_TABLE_H__
#define __WOOJINC_TABLE_H__
#include "woojin.h"
#include "statement.h"
#include "list.h"
#include "map.h"

struct Table {
  w__ast__FnDecl *curfn;
  StringIntMap type_idxs;
  StringBoolMap fn_used;
  StringBoolMap const_used;
  StringBoolMap global_used;
  ArrayString imports;
  Array_w__TypeSymbolPtr type_symbols;
};

Table* w__table__Table_new();
void w__table__Table__free(Table* self);
void w__table__Table_register_builtin_type_sym(Table* t);
#endif