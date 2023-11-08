#include <stdio.h>
#include <stdlib.h>
#include "woojin.h"
#include "error.h"
#include "mem.h"
#include "list.h"

List __list_new() {
  List _r1 = __list_new_with_capacity(1);
  return _r1;
}

List __list_new_with_capacity(size_t _a1) {
  List _r1;
  _r1.capacity = _a1;
  _r1.size = 0;
  _r1.items = (void **)calloc(_r1.capacity, sizeof(void *));
  if (_r1.items == NULL) EEXIT(E_MEMALLOC);
  return _r1;
}

void __list_realloc(List* _a1) {
  _a1->capacity *= 2;
  void** temp = (void **)realloc(_a1->items, sizeof(void *) * _a1->capacity);
  if (temp == NULL) EEXIT(E_MEMALLOC);
  _a1->items = temp;
}

void __list_init(List *_a1) {
  _a1->capacity = 1;
  _a1->size = 0;
  _a1->items = (void **)calloc(_a1->capacity, sizeof(void *));
  if (_a1->items == NULL) EEXIT(E_MEMALLOC);
}

void __list_append_list(List* _a1, void* _a2) {
  if (_a1->size >= _a1->capacity)  __list_realloc(_a1);
  _a1->items[_a1->size++] = _a2;
}

void* __list_get_list_front(List* _a1) {
  if (_a1->size == 0) EEXIT(E_EMPTYARR);
  return _a1->items[0];
}

void* __list_get_list_back(List* _a1) {
  if (_a1->size == 0) EEXIT(E_EMPTYARR);
  return _a1->items[_a1->size - 1];
}

void* __list_push_front(List* _a1, void* _a2) {
  if (_a1->size >= _a1->capacity) __list_realloc(_a1);
  for (size_t i = _a1->size;i > 0;i--)
    _a1->items[i] = _a1->items[i - 1];
  _a1->items[0] = _a2;
  _a1->size++;
  return _a2;
}

void* __list_push_back(List* _a1, void* _a2) {
  if (_a1->size >= _a1->capacity) __list_realloc(_a1);
  _a1->items[_a1->size++] = _a2;
  return _a2;
}

void* __list_pop_front(List* _a1) {
  if (_a1->size == 0) EEXIT(E_EMPTYARR);
  void* _r1 = _a1->items[0];
  for (size_t i = 0;i < _a1->size - 1;i++)
    _a1->items[i] = _a1->items[i + 1];
  _a1->size--;
  return _r1;
}

void* __list_pop_back(List* _a1) {
  if (_a1->size == 0) EEXIT(E_EMPTYARR);
  void* _r1 = _a1->items[_a1->size - 1];
  _a1->size--;
  return _r1;
}

void* __list_emplace_front(List* _a1, void* _a2) {
  if (_a1->size < 1) {
    __list_append_list(_a1, _a2);
  } else {
    if (_a1->size >= _a1->capacity) __list_realloc(_a1);
    for (size_t i = _a1->size;i > 0;i--)
      _a1->items[i] = _a1->items[i - 1];
    _a1->items[0] = _a2;
    _a1->size++;
  }
  return _a1->items[0];
}

void* __list_emplace_back(List* _a1, void* _a2) {
  __list_append_list(_a1, _a2);
  return _a1->items[_a1->size - 1];
}

int __list_replace(List* _a1, size_t _a2, void* _a3) {
  if (_a1 == NULL) return 0;
  if (_a2 >= _a1->size) {
    __list_append_list(_a1, _a3);
    return 1;
  }
  FREE(_a1->items[_a2]);
  _a1->items[_a2] = _a3;
  return 1;
}

void __list_list_free(List* _a1) {
  if (_a1 == NULL) return;
  for (size_t i = 0;i < _a1->size;i++)
    FREE(_a1->items[i]);
  FREE(_a1->items);
  _a1->capacity = 0;
  _a1->size = 0;
  _a1->items = NULL;
}