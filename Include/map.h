#ifndef __WOOJINC_MAP_H__
#define __WOOJINC_MAP_H__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "error.h"
typedef struct MapKVPair {
  char* key;
  void* value;
} MapKVPair;

typedef struct Map {
  MapKVPair* data;
  size_t size;
  size_t capacity;
} Map;

unsigned int hash(const char* key);
Map* createMap();
void initMap(Map* map);
void resizeMap(Map* map, size_t new_cap);
void pushMap(Map* map, const char* key, void* value);
void* getMap(Map* map, const char* key);
int replaceMap(Map* map, const char* key, void* newValue);
void freeMap(Map* map);
#endif