#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "woojin.h"
#include "error.h"
#include "mem.h"

static char *filename;
static FILE *fpi;
static char *line;
static char **lines;
static int   lineIndex;
static int   lastLineIndex;

int openSource(char fileName[]) {
  if ((fpi = fopen(fileName, "r")) == NULL) ErrDetailExit(E_OPENFILE, fileName);
  filename = strdup(fileName);
  line = NULL;
  lineIndex = 0;
  lastLineIndex = -1;
  return 0;
}

char* getFileName() {
  return filename;
}

void printCurrentLine() {
  printf("%s!!", line);
}

char* getLine(int n) {
  int capacity = 256;
  char *gline = (char*)calloc(1, capacity);
  if (gline == NULL) { return NULL; }
  fseek(fpi, 0, SEEK_SET);
  for (int current_line = 1; current_line <= n; current_line++) {
    int i = 0;
    char ch;
    while ((ch = fgetc(fpi)) != EOF && ch != '\n') {
      if (i == capacity - 1) {
        capacity *= 2;
        char *temp = realloc(gline, capacity);
        if (temp == NULL) MULTI(Free(line); return NULL);
        gline = temp;
      }
      gline[i++] = ch;
    }
    gline[i] = '\0';
    if (ch == EOF) MULTI(Free(gline); return NULL);
    if (current_line == n) return gline;
  }
  MULTI(Free(gline); return NULL);
}

void closeSource() {
  printf("closeSource\n");
  fclose(fpi);
}