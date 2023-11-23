#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "woojin.h"
#include "compiler.h"

CompilerOption* w__complier__option_parse(int argc, char** argv) {
  CompilerOption* option = malloc(sizeof(CompilerOption));
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "--korean") == 0) {
      option->korean = true;
    }
  }
  return option;
}

CompilerOption* w__complier__option_default() {
  CompilerOption* option = malloc(sizeof(CompilerOption));
  option->korean = false;
  return option;
}