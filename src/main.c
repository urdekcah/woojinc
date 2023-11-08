#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>
#include "mem.h"
#include "error.h"

#define READSIZE 512

int main(int argc, char *argv[]) {
  setlocale(LC_ALL, "");
  unsigned int size = 0, length = 0;
  char* buffer = NULL;
  FILE* file = NULL;
  const char *wjext = ".wjc";
  for (int i = 0;i < argc;i++) {
    int a = strlen(argv[i]), b = strlen(wjext);
    if (a>=b&&strcmp(argv[i]+a-b,wjext)==0) {
      file = fopen(argv[i], "r");
      if (file == NULL) EEXITD(E_OPENFILE, argv[i]);
      while (!feof(file)) {
        size += READSIZE;
        char* temp = (char *)realloc(buffer, sizeof(char) * size);
        if (temp == NULL) {
          if (buffer) FREE(buffer);
          EEXIT(E_MEMALLOC);
        }
        buffer = temp;
        length += fread((buffer + size) - READSIZE, 1, READSIZE, file);
      }
      if (fclose(file) != 0) {
        if (buffer) FREE(buffer);
        EEXITD(E_CLOSEFILE, argv[2]);
      }
      if (!buffer) return 1;
      buffer[length] = '\0';
      break;
    }
  }
  if (buffer == NULL) EEXIT(E_NOWJCF);
  printf("code:\n%s\n", buffer);
  FREE(buffer);
  return 0;
}