#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "map.h"
#include "error.h"

unsigned int hash(const char* key) {
  unsigned int hash = 0;
  while (*key) {
    hash = (hash * 31) + (*key);
    key++;
  }
  return hash;
}

Map* createMap() {
  Map* map = (Map*)malloc(sizeof(Map));
  if (map == NULL) errExit(EMSG_MEMALLOC_FAILED);
  initMap(map);
  return map;
}

void initMap(Map* map) {
  map->size = 0;
  map->capacity = 1;
  map->data = (MapKVPair*)calloc(map->capacity, sizeof(MapKVPair));
  if (map->data == NULL) errExit(EMSG_MEMALLOC_FAILED);
}

void resizeMap(Map* map, size_t new_cap) {
  map->data = (MapKVPair*)realloc(map->data, sizeof(MapKVPair) * new_cap);
  if (map->data == NULL) errExit(EMSG_MEMALLOC_FAILED);
  map->capacity = new_cap;
}

void pushMap(Map* map, const char* key, void* value) {
  if (map->size == map->capacity) resizeMap(map, map->capacity * 2);
  MapKVPair* entry = &map->data[map->size];
  entry->key = strdup(key);
  entry->value = value;
  map->size++;
}

void* getMap(Map* map, const char* key) {
  for (size_t i = 0; i < map->size; i++) {
    if (strcmp(map->data[i].key, key) == 0) 
      return map->data[i].value;
  }
  return NULL;
}

int replaceMap(Map* map, const char* key, void* newValue) {
  if (map == NULL || key == NULL || newValue == NULL) return 0;
  for (size_t i = 0; i < map->size; i++) {
    if (strcmp(map->data[i].key, key) == 0) {
      free(map->data[i].value);
      map->data[i].value = newValue;
      return 1;
    }
  }
  pushMap(map, key, newValue);
  return 1;
}

void freeMap(Map* map) {
  for (size_t i = 0; i < map->size; i++) 
    free(map->data[i].key);
  free(map->data);
}