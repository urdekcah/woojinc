#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <locale.h>
#include "error.h"
#include "node.h"
#include "scan.h"
#include "printer.h"
#include "parser.h"
#include "interpret.h"
#include "config.h"

#define READSIZE 512

int main(int argc, char* argv[]) {
  setlocale(LC_ALL, "");
  unsigned int size = 0, length = 0;
  char *buffer = NULL/*, *fname = NULL*/;
  FILE *file = NULL;
  if (argc < 3) errExit(EMSG_UNKNOWN_USAGE, argv[0]);
  file = fopen(argv[2], "r");
  if (file == NULL) errExit(EMSG_FILEOPEN_FAILED, argv[2]);
  while (!feof(file)) {
    size += READSIZE;
    buffer = (char *)realloc(buffer, sizeof(char) * size);
    length += fread((buffer + size) - READSIZE, 1, READSIZE, file);
  }
  if (fclose(file) != 0) {
    if (buffer) free(buffer);
    errExit(EMSG_FILECLOSE_FAILED, argv[2]);
  }
  if (!buffer) return 1;
  buffer[length] = '\0';
  // printf("%s\n", buffer);
  Token* tokenList = scanCode(buffer);
  if (strcmp(argv[1], "scan") == 0) {
    printTokenList(tokenList);
  } else if (strcmp(argv[1], "run") == 0) {
    Program* program = parse(tokenList);
    interpret(program);
    free(program);
  }
  else errExit(EMSG_UNKNOWN_COMMAND, argv[1]);
  free(buffer);
  free(tokenList);
  return 0;
}