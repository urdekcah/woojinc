#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "woojin.h"
#include "map.h"
#include "error.h"
#include "mem.h"

uint32_t __map__hash__murmur3_32(const char *_a1, uint32_t _a2, uint32_t _a3) {
  uint32_t _r1 = 0xcc9e2d51;
  uint32_t _r2 = 0x1b873593;
  uint32_t _r3 = 15;
  uint32_t _r4 = 13;
  uint32_t _r5 = 5;
  uint32_t _r6 = 0xe6546b64;
  uint32_t _r7 = _a3;
  uint32_t _r8;
  uint8_t *_r9 = (uint8_t *)_a1;
  const uint32_t *_r10 = NULL;
  const uint8_t *_r11 = NULL;
  int _r12 = 0;
  int _r13 = _a2 / 4;
  _r10 = (const uint32_t *)(_r9+_r13*4);
  _r11 = (const uint8_t *)(_r9+_r13*4);
  for (_r12 = -_r13; _r12 != 0; ++_r12) {
    _r8 = _r10[_r12];
    _r8 *= _r1;
    _r8 = (_r8 << _r3) | (_r8 >> (32 - _r3));
    _r8 *= _r2;
    _r7 ^= _r8;
    _r7 = (_r7 << _r4) | (_r7 >> (32 - _r4));
    _r7 = _r7 * _r5 + _r6;
  }
  _r8 = 0;
  switch (_a2 & 3) {
    case 3:
      _r8 ^= (_r11[2] << 16);
      break;
    case 2:
      _r8 ^= (_r11[1] << 8);
      break;
    case 1:
      _r8 ^= _r11[0];
      _r8 *= _r1;
      _r8 = (_r8 << _r3) | (_r8 >> (32 - _r3));
      _r8 *= _r2;
      _r7 ^= _r8;
      break;
  }
  _r7 ^= _a2;
  _r7 ^= (_r7 >> 16);
  _r7 *= 0x85ebca6b;
  _r7 ^= (_r7 >> 13);
  _r7 *= 0xc2b2ae35;
  _r7 ^= (_r7 >> 16);
  return _r7;
}

uint32_t __map_key__hash(const char* _a1) {
  return __map__hash__murmur3_32(_a1, strlen(_a1), 0);
}

Map* __map__create_map() {
  Map* _r1 = (Map*)calloc(1,sizeof(Map));
  if (_r1 == NULL) EEXIT(E_MEMALLOC);
  __map__map_init(_r1);
  return _r1;
}

void __map__map_init(Map* _a1) {
  _a1->size = 0;
  _a1->capacity = 100;
  printf("%d\n", _a1->capacity);
  _a1->items = (MapItem*)calloc(_a1->capacity, sizeof(MapItem));
  if (_a1->items == NULL) EEXIT(E_MEMALLOC);
}

void __map__resize(Map* _a1, size_t _a2) {
  MapItem* _r1 = (MapItem*)realloc(_a1->items, sizeof(MapItem) * _a2);
  if (_r1 == NULL) EEXIT(E_MEMALLOC);
  _a1->items = _r1;
  _a1->capacity = _a2;
}

void* __map__get(Map* _a1, char* _a2) {
  uint32_t idx = __map__get_map_index(_a1, _a2);
  return _a1->items[idx].value;
}

int __map__replace(Map* _a1, char* _a2, void* _a3) {
  if (_a1 == NULL || _a2 == NULL || _a3 == NULL) return 0;
  uint32_t _r1 = __map__get_map_index(_a1, _a2);
  if(_a1->items[_r1].key != NULL) {
    FREE(_a1->items[_r1].key);
    FREE(_a1->items[_r1].value);
    _a1->items[_r1].key = strdup(_a2);
    _a1->items[_r1].value = _a3;
    return 1;
  }
  __map__insert(_a1, _a2, _a3);
  return 1;
}

uint32_t __map__get_map_index(const Map* _a1, const char* _a2) {
  uint32_t _r1 = __map_key__hash(_a2) % _a1->capacity;
  while(_a1->items[_r1].key != NULL && strcmp(_a1->items[_r1].key, _a2) != 0)
    _r1 = (_r1 + 1) % _a1->capacity;
  return _r1;
}

void __map__insert(Map* _a1, char* _a2, void* _a3) {
  if ((double)_a1->size >= (double)(_a1->capacity * 0.7))
    __map__resize(_a1, _a1->capacity * 2);
  uint32_t _r1 = __map__get_map_index(_a1, _a2);
  if (_a1->items[_r1].key != NULL) {
    FREE(_a1->items[_r1].value);
  } else {
    _a1->size++;
  }
  _a1->items[_r1].key = strdup(_a2);
  _a1->items[_r1].value = _a3;
}

void __map__free_map(Map* _a1) {
  for (size_t i = 0;i < _a1->capacity;i++) {
    if (_a1->items[i].key != NULL) {
      FREE(_a1->items[i].key);
      FREE(_a1->items[i].value);
    }
  }
  FREE(_a1->items);
  FREE(_a1);
}