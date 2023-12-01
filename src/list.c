#include <stdio.h>
#include "error.h"
#include "list.h"

List* createList() {
  List* list = (List*)malloc(sizeof(List));
  if (list == NULL) errExit(EMSG_MEMALLOC_FAILED);
  initList(list);
  return list;
}

void initList(List* list) {
  list->capacity = 1;
  list->size = 0;
  list->data = (void**)calloc(1, sizeof(void*));
  if (list->data == NULL) errExit(EMSG_MEMALLOC_FAILED);
}

void appendList(List* list, void* item) {
  if (list->size >= list->capacity) {
    list->capacity *= 2;
    list->data = (void**)realloc(list->data, sizeof(void*) * list->capacity);
    if (list->data == NULL) errExit(EMSG_MEMALLOC_FAILED);
  }
  list->data[list->size++] = item;
}

void* pushFrontList(List* list, void* item) {
  if (list->size >= list->capacity) {
    list->capacity *= 2;
    list->data = (void**)realloc(list->data, sizeof(void*) * list->capacity);
    if (list->data == NULL) errExit(EMSG_MEMALLOC_FAILED);
  }
  for (size_t i = list->size; i > 0; i--)
    list->data[i] = list->data[i - 1];
  list->data[0] = item;
  list->size++;
  return item;
}

void* pushBackList(List* list, void* item) {
  if (list->size >= list->capacity) {
    list->capacity *= 2;
    list->data = (void**)realloc(list->data, sizeof(void*) * list->capacity);
    if (list->data == NULL) errExit(EMSG_MEMALLOC_FAILED);
  }
  list->data[list->size] = item;
  list->size++;
  return item;
}

void* frontList(List* list) {
  if (list->size == 0) errExit(EMSG_EMPTY_LIST);
  return list->data[0];
}

void* backList(List* list) {
  if (list->size == 0) errExit(EMSG_EMPTY_LIST);
  return list->data[list->size - 1];
}

void* popFrontList(List* list) {
  if (list->size == 0) errExit(EMSG_EMPTY_LIST);
  void* frontElement = list->data[0];
  for (size_t i = 1; i < list->size; i++)
    list->data[i - 1] = list->data[i];
  list->size--;
  return frontElement;
}

void* popBackList(List* list) {
  if (list->size == 0) errExit(EMSG_EMPTY_LIST);
  void* backElement = list->data[list->size - 1];
  list->size--;
  return backElement;
}

void* emplaceFrontList(List* list, void* item) {
  if (list->size == 0) {
    appendList(list, item);
  } else {
    if (list->size == list->capacity) {
      list->capacity *= 2;
      list->data = (void**)realloc(list->data, list->capacity * sizeof(void*));
      if (list->data == NULL) errExit(EMSG_MEMALLOC_FAILED);
    }
    for (size_t i = list->size; i > 0; i--) list->data[i] = list->data[i - 1];
    list->data[0] = item;
    list->size++;
  }
  return list->data[0];
}

void* emplaceBackList(List* list, void* item) {
  appendList(list, item);
  return list->data[list->size - 1];
}

int replaceList(List* list, size_t index, void* newValue) {
  if (list == NULL) return 0;
  if (index >= list->size) {
    appendList(list, newValue);
    return 1;
  }
  free(list->data[index]);
  list->data[index] = newValue;
  return 1;
}

void freeList(List* list) {
  free(list->data);
  list->size = 0;
  list->capacity = 0;
  list->data = NULL;
}