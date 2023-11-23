#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "woojin.h"
#include "complier.h"

ComplierOption* w__complier__option_parse(int argc, char** argv) {
  ComplierOption* option = malloc(sizeof(ComplierOption));
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "--korean") == 0) {
      option->korean = true;
    }
  }
  return option;
}

ComplierOption* w__complier__option_default() {
  ComplierOption* option = malloc(sizeof(ComplierOption));
  option->korean = false;
  return option;
}