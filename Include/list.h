#ifndef __WOOJINC_LIST_H__
#define __WOOJINC_LIST_H__
#include <stdio.h>
#include <stddef.h>
typedef struct List {
  size_t capacity;
  size_t size;
  void** data;
} List;

List* createList();
void initList(List* list);
void appendList(List* list, void* item);
void* frontList(List* list);
void* backList(List* list);
void* pushFrontList(List* list, void* item);
void* pushBackList(List* list, void* item);
void* popFrontList(List* list);
void* popBackList(List* list);
void* emplaceFrontList(List* list, void* item);
void* emplaceBackList(List* list, void* item);
int replaceList(List* list, size_t index, void* newValue);
void freeList(List* list);
#endif