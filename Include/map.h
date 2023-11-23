#ifndef __WOOJINC_MAP_H__
#define __WOOJINC_MAP_H__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "woojin.h"

struct MapItem {
  char* key;
  void* value;
};

struct Map {
  MapItem* items;
  size_t size, capacity;
};

uint32_t     __map__hash__murmur3_32(const char *key, uint32_t len, uint32_t seed);
int          __map__hash__rehash(Map* _a1);
uint32_t     __map_key__hash(const char* _a1);
// MapItem*     __map__create_item(char* _a1, void* _a2);
void         __map__map_init(Map* _a1);
// void         __map__map_init_with_capacity(Map* _a1, size_t _a2);
// void         __map__free_item(MapItem* _a1);
void         __map__resize(Map* _a1, size_t _a2);
void         __map__insert(Map* _a1, const char* _a2, void* _a3);
uint32_t     __map__get_map_index(const Map* _a1, const char* _a2);
void*        __map__get(Map* _a1, char* _a2);
int          __map__replace(Map* _a1, char* _a2, void* _a3);
Map*         __map__create_map();
void         __map__free_map(Map* _a1);
#endif