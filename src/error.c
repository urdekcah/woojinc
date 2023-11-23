#include <stdio.h>
#include <stdarg.h>
#include <locale.h>
#include <errno.h>
#include <wchar.h>
#include <stdlib.h>
#include <string.h>
#if !defined(__linux__)
#include <Windows.h>
#endif
#include "woojin.h"
#include "error.h"
#include "mem.h"

void errExit(bool detail, const wchar_t* format, ...) {
  va_list args;
  va_start(args, format);
  if (detail) lastError();
  #if !defined(__linux__)
  fwprintf_s(stderr, L"\x1b[31m%s\x1b[0m: ", (wchar_t*)E_PREFIX);
  vfwprintf(stderr, format, args);
  #else
  wprintf(L"\x1b[31m%s\x1b[0m: ", (wchar_t*)E_PREFIX);
  vfwprintf(stderr, format, args);
  #endif
  va_end(args);
  exit(1);
}

void lastError() {
  #if !defined(__linux__)
  wchar_t* message = NULL;
  FormatMessageW(
    FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
    NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPWSTR)&message, 0, NULL
  );
  if (message) {
    fwprintf_s(stderr, L"\x1b[31m%s\x1b[0m: %s", (wchar_t*)E_PREFIX, message);
    LocalFree(message);
  }
  #else
  const wchar_t *message = (const wchar_t *)strerror(errno);
  if (message) wprintf(L"\x1b[31m%s\x1b[0m: %ls\n", E_PREFIX, message);
  #endif
}

CompilerMessage* w__error__CompilerMessage__new(char* message, char* detail, char* filename, TokenPos pos) {
  CompilerMessage* self = calloc(1,sizeof(CompilerMessage));
  if (self == NULL) ErrExit(E_MEMALLOC);
  self->message = message;
  self->detail = detail;
  self->filename = filename;
  self->pos = pos;
  return self;
}

int openSource(char fileName[]);
void printCurrentLine();
char* getLine(int n);

void w__errors__print_line(int n) {
  char* line = getLine(n);
  printf("%s\n", line);
  free(line);
}

wchar_t* FormatW(const wchar_t* format, ...) {
  va_list args;
  va_start(args, format);
  int len = _vscwprintf(format, args) + 1;
  wchar_t* buffer = (wchar_t*)calloc(len, sizeof(wchar_t));
  if (buffer == NULL) ErrExit(E_MEMALLOC);
  vswprintf_s(buffer, len, format, args);
  va_end(args);
  return buffer;
}

void w__errors__ErrLine(Error* err) {
  CompilerMessage msg = err->msg;
  char* line = getLine(msg.pos.line);
  for (int i = 0; i < msg.pos.col; i++) printf("%d", line[i]);
  printf("\033[1;31m");
  for (int i = 0; i <= msg.pos.len; i++) printf("%c", line[msg.pos.col + i]);
  printf("\033[0m");
  for (int i = msg.pos.col + msg.pos.len+1; i < strlen(line); i++) printf("%c", line[i]);
  printf("\n");
  for (int i = 0; i < msg.pos.col; i++) printf(" ");
  printf("\033[1;31m");
  printf("^");
  for (int i = 1; i < msg.pos.len+1; i++) printf("~");
  printf("\033[0m\n");
  free(line);
}

void printLineData(const CompilerMessage* msg) {
  printf("\033[0;37m%s:%d:%d:\033[0m ", msg->filename, msg->pos.line, msg->pos.col);
}

void printError(Error* err) {
  fwprintf_s(stderr, L"\033[0;37m%S:%d:%d:\033[0m.", err->msg.filename, err->msg.pos.line, err->msg.pos.col, err->msg.message);
  w__errors__ErrLine(err);
}

void w__errors__MessageLine(CompilerMessage msg, ComplierMessageKind kind) {
  char* line = getLine(msg.pos.line);
  for (int i = 0; i < msg.pos.col; i++) printf("%c", (isspace(line[i]) ?' ':line[i]));
  printf(MessageColorSwitch(kind, false));
  for (int i = 0; i <= msg.pos.len; i++) printf("%c", line[msg.pos.col + i]);
  printf("\033[0m");
  for (int i = msg.pos.col + msg.pos.len+1; i < strlen(line); i++) printf("%c", line[i]);
  printf("\n");
  for (int i = 0; i < msg.pos.col; i++) printf(" ");
  printf(MessageColorSwitch(kind, false));
  printf("^");
  for (int i = 1; i < msg.pos.len+1; i++) printf("~");
  printf("\033[0m\n");
  free(line);
}

void w__error__PrintCompilerMessage(CompilerMessage msg, ComplierMessageKind kind) {
  printLineData(&msg);
  fwprintf_s(stderr,L"%S%S:\033[0m %s\n", MessageColorSwitch(kind, false), ComplierMsgPrefix(kind), msg.detail);
  w__errors__MessageLine(msg, kind);
}

Error* w__error__Error__new(char* message, char* detail, char* filename, TokenPos pos) {
  Error* self = calloc(1,sizeof(Error));
  if (self == NULL) ErrExit(E_MEMALLOC);
  self->msg = *w__error__CompilerMessage__new(message, detail, filename, pos);
  return self;
}

Warning* w__error__Warning__new(char* message, char* detail, char* filename, TokenPos pos) {
  Warning* self = calloc(1,sizeof(Warning));
  if (self == NULL) ErrExit(E_MEMALLOC);
  self->msg = *w__error__CompilerMessage__new(message, detail, filename, pos);
  return self;
}

Notice* w__error__Notice__new(char* message, char* detail, char* filename, TokenPos pos) {
  Notice* self = calloc(1,sizeof(Notice));
  if (self == NULL) ErrExit(E_MEMALLOC);
  self->msg = *w__error__CompilerMessage__new(message, detail, filename, pos);
  return self;
}

void w__error__CompilerMessage__free(CompilerMessage* self) {
  if (self == NULL) return;
  Free(self->message);
  Free(self->detail);
  Free(self->filename);
  Free(self);
}

void w__error__Error__free(Error* self) { FreeComplierMessageMacro(self); }
void w__error__Warning__free(Warning* self) { FreeComplierMessageMacro(self); }
void w__error__Notice__free(Notice* self) { FreeComplierMessageMacro(self); }
