#include <stdio.h>
#include <stdlib.h>
#include "woojin.h"
#include "type.h"
#include "mem.h"
#include "parse.h"
#include "token.h"
#include "error.h"
#include "scope.h"

w__parser__Parser* w__parser__Parser__new(Table* table, List tokens, CompilerOption* co) {
  w__parser__Parser* self = (w__parser__Parser*)malloc(sizeof(w__parser__Parser));
  if (self == NULL) ErrDetailExit(E_MEMALLOC);
  w__parser__Parser__init(self, table, tokens, co);
  return self;
}

void w__parser__Parser__init(w__parser__Parser* self, Table* table, List tokens, CompilerOption* co) {
  self->table = table;
  self->tokens = tokens;
  self->current = (Token*)tokens.items[0];
  self->tidx = 0;
  self->scope = calloc(1, sizeof(Scope));
  if (self->scope == NULL) ErrDetailExit(E_MEMALLOC);
  self->scope->objects = __map__create_map();
  self->scope->children = __list_mal();
  self->scope->detached_fparen = 0;
  self->scope->parent = NULL;
  self->co = co;
  __list_init(&self->errors);
  __list_init(&self->warnings);
  __list_init(&self->notices);
}

void w__parser__Parser__InitDefault(w__parser__Parser* out) { ParserInitDefault(out); }

void w__parser__Parser__free(w__parser__Parser* self) {
  Free(&self->table);
  Free(&self->tokens);
  Free(&self->errors);
  Free(&self->warnings);
  Free(&self->notices);
  Free(&self->scope);
  free(self);
}