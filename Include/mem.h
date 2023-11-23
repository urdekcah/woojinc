#ifndef __WOOJINC_MEM_H__
#define __WOOJINC_MEM_H__
#include <stdio.h>
#include <stdlib.h>
#include "woojin.h"
#include "error.h"
#include "list.h"
#include "map.h"
#include "parse.h"
#include "table.h"
#include "token.h"
#include "scope.h"

#define Free(T) _Generic((T), \
  List*: __list_list_free, \
  Map*: __map__free_map, \
  w__parser__Parser*: w__parser__Parser__free, \
  Table*: w__table__Table__free, \
  Token*: w__Token__free, \
  CompilerMessage*: w__error__Error__free, \
  Error*: w__error__Warning__free, \
  Warning*: w__error__Notice__free, \
  Notice*: w__error__CompilerMessage__free, \
  Scope*: w__ast__Scope_free, \
  default: free \
)(T)

#define New(T, ...) _Generic((T), \
  w__parser__Parser*: w__parser__Parser__InitDefault, \
  List*: __list_init, \
  Map*: __map__map_init \
)(T, __VA_ARGS__)

#define FreeAll(...) \
  do { \
    void *ptrs[] = { __VA_ARGS__ }; \
    size_t num_ptrs = sizeof(ptrs) / sizeof(ptrs[0]); \
    for (size_t i = 0; i < num_ptrs; i++) \
      if (ptrs[i] != NULL) Free(ptrs[i]); \
  } while (0)

void* memdup(const void* mem, size_t size);
#endif