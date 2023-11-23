#include <stdio.h>
#include <stdlib.h>
#include "woojin.h"
#include "type.h"
#include "mem.h"
#include "parse.h"
#include "table.h"
#include "map.h"
#include "list.h"

Table* w__table__Table_new() {
  Table* self = (Table*)malloc(sizeof(Table));
  if (self == NULL) return NULL;
  self->const_used = *__map__create_map();
  self->fn_used = *__map__create_map();
  self->global_used = *__map__create_map();
  self->imports = __list_new();
  self->curfn = NULL;
  self->type_idxs = *__map__create_map();
  self->type_symbols = __list_new();
  w__table__Table_register_builtin_type_sym(self);
  return self;
}

void w__table__Table__free(Table* self) {
  Free(&self->const_used);
  Free(&self->fn_used);
  Free(&self->global_used);
  Free(&self->imports);
  Free(&self->type_idxs);
  Free(&self->type_symbols);
  Free(self);
}

#define ADDR(type, expr) (&((type[]){expr}[0]))

type_t w__ast__new_type(int idx) {
	if (idx < 1 || idx > 65535) {
    ErrExit(FormatW(L"Invalid type index: %d", idx));
	}
	return idx;
}

int w__ast__Table_rewrite_already_registered_symbol(Table* t, TypeSymbol typ, int existing_idx) {
  TypeSymbol * esym = (*(TypeSymbol**)__list_list__get(&t->type_symbols, existing_idx));
  if (esym->kind == KindPlaceholder) {
    __list_replace(&t->type_symbols, existing_idx, (void*)memdup(&(TypeSymbol){.parent_idx=(typ).parent_idx, .name=(typ).name, .tinfo=(typ).tinfo, .kind=(typ).kind, .pub=(typ).pub, .idx=existing_idx},sizeof(TypeSymbol)));
    return existing_idx;
  }
}

int w__ast__Table_register_sym(Table* t, TypeSymbol sym) {
  int idx = -2, eidx = (int)__map__get_map_index(ADDR(Map, t->type_idxs), (char*[]){sym.name});
  if (eidx!=NULL) return w__ast__Table_rewrite_already_registered_symbol(t, sym, eidx);
  idx = t->type_symbols.size;
  __list_push_back((List*)&t->type_symbols, (void*)memdup(&(TypeSymbol){.parent_idx = (sym).parent_idx, .tinfo = (sym).tinfo, .kind = (sym).kind, .name = (sym).name, .pub = (sym).pub, .idx = idx},sizeof(TypeSymbol)));
  ((TypeSymbol*)__list_list__get((List*)&t->type_symbols, idx))->idx = idx;
  printf("registering %s at %d\n", sym.name, idx);
  return idx;
}

TypeSymbol* w__ast__Table_sym(Table* t, type_t type) {
  int idx = (((uint16_t)(type)) & 0xffffU);
  if (idx > 0) {
    TypeSymbol* _t1 = (*(TypeSymbol**)__list_list__get(&t->type_symbols, idx));
    return _t1;
  }
  return NULL;
}

void w__table__Table_register_builtin_type_sym(Table* t) {
  w__ast__Table_register_sym(t, (TypeSymbol){.parent_idx = 0, .tinfo = {{0}}, .kind = KindPlaceholder, .name = "placeholder", .pub = false, .idx = 0});
  w__ast__Table_register_sym(t, (TypeSymbol){.parent_idx = 0, .tinfo = {{0}}, .kind = Kindi8, .name = "i8", .pub = true, .idx = 0});
  w__ast__Table_register_sym(t, (TypeSymbol){.parent_idx = 0, .tinfo = {{0}}, .kind = Kindi16, .name = "i16", .pub = true, .idx = 0});
  w__ast__Table_register_sym(t, (TypeSymbol){.parent_idx = 0, .tinfo = {{0}}, .kind = Kindi32, .name = "i32", .pub = true, .idx = 0});
  w__ast__Table_register_sym(t, (TypeSymbol){.parent_idx = 0, .tinfo = {{0}}, .kind = Kindi64, .name = "i64", .pub = true, .idx = 0});
  w__ast__Table_register_sym(t, (TypeSymbol){.parent_idx = 0, .tinfo = {{0}}, .kind = Kindu8, .name = "u8", .pub = true, .idx = 0});
  w__ast__Table_register_sym(t, (TypeSymbol){.parent_idx = 0, .tinfo = {{0}}, .kind = Kindu16, .name = "u16", .pub = true, .idx = 0});
  w__ast__Table_register_sym(t, (TypeSymbol){.parent_idx = 0, .tinfo = {{0}}, .kind = Kindu32, .name = "u32", .pub = true, .idx = 0});
  w__ast__Table_register_sym(t, (TypeSymbol){.parent_idx = 0, .tinfo = {{0}}, .kind = Kindu64, .name = "u64", .pub = true, .idx = 0});
  w__ast__Table_register_sym(t, (TypeSymbol){.parent_idx = 0, .tinfo = {{0}}, .kind = Kindf32, .name = "f32", .pub = true, .idx = 0});
  w__ast__Table_register_sym(t, (TypeSymbol){.parent_idx = 0, .tinfo = {{0}}, .kind = Kindf64, .name = "f64", .pub = true, .idx = 0});
  w__ast__Table_register_sym(t, (TypeSymbol){.parent_idx = 0, .tinfo = {{0}}, .kind = KindChar, .name = "char", .pub = true, .idx = 0});
  w__ast__Table_register_sym(t, (TypeSymbol){.parent_idx = 0, .tinfo = {{0}}, .kind = KindBool, .name = "bool", .pub = true, .idx = 0});
  w__ast__Table_register_sym(t, (TypeSymbol){.parent_idx = 0, .tinfo = {{0}}, .kind = KindNone, .name = "none", .pub = true, .idx = 0});
  w__ast__Table_register_sym(t, (TypeSymbol){.parent_idx = 0, .tinfo = {{0}}, .kind = KindString, .name = "string", .pub = true, .idx = 0});
  w__ast__Table_register_sym(t, (TypeSymbol){.parent_idx = 0, .tinfo = {{0}}, .kind = KindArray, .name = "array", .pub = true, .idx = 0});
  w__ast__Table_register_sym(t, (TypeSymbol){.parent_idx = 0, .tinfo = {{0}}, .kind = KindMap, .name = "map", .pub = true, .idx = 0});
  w__ast__Table_register_sym(t, (TypeSymbol){.parent_idx = 0, .tinfo = {{0}}, .kind = KindAny, .name = "any", .pub = true, .idx = 0});
  w__ast__Table_register_sym(t, (TypeSymbol){.parent_idx = 0, .tinfo = {{0}}, .kind = KindInterface, .name = "error", .pub = true, .idx = 0});
}