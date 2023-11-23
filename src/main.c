#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>
#include "woojin.h"
#include "mem.h"
#include "error.h"
#include "tokenizer.h"
#include "parse.h"
#include "map.h"
#include "table.h"
#include "type.h"
#include "init.h"

int main(int argc, char *argv[]) {
  _winit(argc, argv);
  _wcleanup();
  return 0;
}

void* memdup(const void* mem, size_t size) { 
  void* out = malloc(size);
  if(out != NULL) memcpy(out, mem, size);
  return out;
}