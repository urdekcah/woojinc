#ifndef __WOOJINC_LIST_H__
#define __WOOJINC_LIST_H__
#include <stdio.h>
#include "woojin.h"
struct List {
  size_t capacity, size;
  void** items;
};

List __list_new();
List __list_new_with_capacity(size_t _a1);
void __list_realloc(List* _a1);
void __list_init(List* _a1);
void __list_append_list(List* _a1, void* _a2);
void* __list_get_list_front(List* _a1);
void* __list_get_list_back(List* _a1);
void* __list_push_front(List* _a1, void* _a2);
void* __list_push_back(List* _a1, void* _a2);
void* __list_pop_front(List* _a1);
void* __list_pop_back(List* _a1);
void* __list_emplace_front(List* _a1, void* _a2);
void* __list_emplace_back(List* _a1, void* _a2);
int   __list_replace(List* _a1, size_t _a2, void* _a3);
void  __list_list_free(List* _a1);
#endif