#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

char* string_plus(char* a, char* b) {
  size_t len = strlen(a) + strlen(b) + 1;
  char* result = calloc(len, sizeof(char));
  strcpy(result, a);
  strcat(result, b);
  result[len] = '\0';
  return result;
}