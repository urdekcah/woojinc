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
#include "node.h"
#include "complier.h"

#define READSIZE 512

char* buffer = NULL;
FILE* file = NULL;
Tokenizer* tz = NULL;
Array_w__ast__Node res;

void Some(w__ast__Node helloworld) {
  
}

void _winit(int _ac, char* _av[]) {
  setlocale(LC_ALL, "");
  ComplierOption* co = w__complier__option_parse(_ac, _av);
  unsigned int size = 0, length = 0;
  const char *wjext = ".wjc";
  for (int i = 0;i < _ac;i++) {
    int a = strlen(_av[i]), b = strlen(wjext);
    if (a>=b&&strcmp(_av[i]+a-b,wjext)==0) {
      file = fopen(_av[i], "r");
      openSource(_av[i]);
      if (file == NULL) ErrDetailExit(E_OPENFILE, _av[i]);
      while (!feof(file)) {
        size += READSIZE;
        char* temp = (char *)realloc(buffer, sizeof(char) * size);
        if (temp == NULL) {
          if (buffer) FreeAll(buffer);
          ErrExit(E_MEMALLOC);
        }
        buffer = temp;
        length += fread((buffer + size) - READSIZE, 1, READSIZE, file);
      }
      if (fclose(file) != 0) {
        if (buffer) FreeAll(buffer);
        ErrDetailExit(E_CLOSEFILE, _av[2]);
      }
      if (!buffer) return 1;
      buffer[length] = '\0';
      break;
    }
  }
  if (buffer == NULL) ErrExit(E_NOWJCF);
  printf("code:\n%s\n", buffer);
  tz = w__Tokenizer__scan(buffer);
  printf("\ntokens:\n");
  for (size_t i = 0;i<tz->tokens.size;i++) {
    printf("%4d %s\n", ((Token*)tz->tokens.items[i])->kind, ((Token*)tz->tokens.items[i])->token);
  }
  _tinit();
  res = w__parser__Parser__parse(tz, co);
  printf("\nresult:\n");
  Some(*(w__ast__Node*)res.items[0]);
  for (size_t i = 0;i<res.size;i++) {
    printf("%s\n", ((w__ast__Node*)res.items[i])->_w__ast__Statement._w__ast__FnDecl.name);
  }
  closeSource();
}

void _wcleanup(void) {
  FreeAll(buffer);
  FreeAll(tz);
  Free(&res);
}