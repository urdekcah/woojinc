#include <stdio.h>
#include "woojin.h"
#include "scope.h"
#include "parse.h"
#include "mem.h"
#include "list.h"
#include "map.h"

void w__parser__Parser_open_scope(w__parser__Parser* p) {
  Scope* s = calloc(1, sizeof(Scope));
  if (s == NULL) ErrDetailExit(E_MEMALLOC);
  s->objects = __map__create_map();
  s->children = __list_mal();
  s->detached_fparen = 0;
  s->parent = p->scope;
  p->scope = s;
}

void w__parser__Parser_close_scope(w__parser__Parser* p) {
  __list_push_back(p->scope->parent->children, (void*)p->scope);
  p->scope = p->scope->parent;
}

void w__ast__Scope_register(Scope* s, ScopeObject* obj) {
  __map__insert(s->objects, obj->name, (void*)memdup(obj, sizeof(ScopeObject)));
}

void w__ast__Scope_free(Scope* _a1) {
  Free(&_a1->objects);
  for (int _t1 = 0; _t1 < _a1->children->size; ++_t1)
    Free(((Scope*)_a1->children->items[_t1]));
  Free(&_a1->children);
  Free(_a1);
}