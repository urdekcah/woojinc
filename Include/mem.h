#ifndef __WOOJINC_MEM_H__
#define __WOOJINC_MEM_H__
#include <stdlib.h>

#define FREE(...) \
  do { \
    void *ptrs[] = { __VA_ARGS__ }; \
    size_t num_ptrs = sizeof(ptrs) / sizeof(ptrs[0]); \
    for (size_t i = 0; i < num_ptrs; i++) \
      if (ptrs[i] != NULL) free(ptrs[i]); \
  } while (0)
#endif