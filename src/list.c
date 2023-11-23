#include <stdio.h>
#include <stdlib.h>
#include "woojin.h"
#include "mem.h"
#include "error.h"
#include "list.h"
#include "type.h"

List __list_new() {
  List _r1 = __list_new_with_capacity(1);
  return _r1;
}

List* __list_mal() {
  List* _r1 = (List*)calloc(1, sizeof(List));
  if (_r1 == NULL) ErrExit(E_MEMALLOC);
  _r1->capacity = 1;
  _r1->size = 0;
  _r1->items = (void **)calloc(_r1->capacity, sizeof(void *));
  if (_r1->items == NULL) ErrExit(E_MEMALLOC);
  return _r1;
}

List __list_new_with_capacity(size_t _a1) {
  List _r1;
  memset(&_r1, 0, sizeof(List));
  _r1.capacity = _a1;
  _r1.size = 0;
  _r1.items = (void **)calloc(_r1.capacity, sizeof(void *));
  printf("size: %zu\n", _r1.size);
  if (_r1.items == NULL) ErrExit(E_MEMALLOC);
  return _r1;
}

void __list_realloc(List* _a1) {
  printf("%d\n",_a1->capacity);
  _a1->capacity *= 2;
  void** temp = (void **)realloc(_a1->items, sizeof(void *) * _a1->capacity);
  printf("temp: %d\n", temp == NULL);
  if (temp == NULL) ErrExit(E_MEMALLOC);
  _a1->items = temp;
}

void __list_init(List *_a1) {
  _a1->capacity = 1;
  _a1->size = 0;
  _a1->items = (void **)calloc(_a1->capacity, sizeof(void *));
  if (_a1->items == NULL) ErrExit(E_MEMALLOC);
}

void __list_append_list(List* _a1, void* _a2) {
  printf("size: %d\n", _a1->size);
  if (_a1->size >= _a1->capacity)  __list_realloc(_a1);
  printf("size now: %d\n", _a1->size);
  _a1->items[_a1->size++] = _a2;
}

void* __list_get_list_front(List* _a1) {
  if (_a1->size == 0) ErrExit(E_EMPTYARR);
  return _a1->items[0];
}

void* __list_get_list_back(List* _a1) {
  if (_a1->size == 0) ErrExit(E_EMPTYARR);
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
  if (_a1->size == 0) ErrExit(E_EMPTYARR);
  void* _r1 = _a1->items[0];
  for (size_t i = 0;i < _a1->size - 1;i++)
    _a1->items[i] = _a1->items[i + 1];
  _a1->size--;
  return _r1;
}

void* __list_pop_back(List* _a1) {
  if (_a1->size == 0) ErrExit(E_EMPTYARR);
  void* _r1 = _a1->items[_a1->size - 1];
  _a1->size--;
  return _r1;
}

void* __list_list__get(List* _a1, size_t _a2) {
  if (_a1->size == 0) ErrExit(E_EMPTYARR);
  if (_a2 > _a1->size) ErrExit(E_OUTOFBOUNDS);
  return _a1->items[_a2];
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
  FreeAll(_a1->items[_a2]);
  _a1->items[_a2] = _a3;
  return 1;
}

void __list_list_free(List* _a1) {
  if (_a1 == NULL) return;
  for (size_t i = 0;i < _a1->size;i++)
    FreeAll(_a1->items[i]);
  FreeAll(_a1->items);
}

bool __list_list_string_includes(List* _a1, char* _a2) {
  for (size_t i = 0; i < _a1->size; i++) 
    if (strcmp(_a1->items[i], _a2) == 0) return true;
  return false;
}