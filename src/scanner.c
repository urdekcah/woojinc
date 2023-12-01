#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "error.h"
#include "token.h"
#include "scan.h"

Token* scanCode(char* code) {
  int capacity = 10;
  Token* result = calloc(capacity, sizeof(Token));
  tokstat* stat = newTokStat(code);
  while(*stat->current != '\0') {
    switch(getCharType(*(stat->current))) {
      case CharWhiteSpace:
        if (*stat->current == '\n') {
          stat->lineno++;
          stat->coloff = 0;
          stat->current++;
        } else {
          stat->coloff++;
          stat->current++;
        }
        break;
      case CharNumberLiteral: result[stat->tokidx++] = scanNumberLiteral(stat); break;
      case CharStringLiteral: result[stat->tokidx++] = scanStringLiteral(stat); break;
      case CharIdentAndKeyword: result[stat->tokidx++] = scanIdentAndKeyword(stat); break;
      case CharOperatorAndPunctuator: result[stat->tokidx++] = scanOperatorAndPunctuator(stat); break;
      default: exit(1);
    }
    if (stat->tokidx >= capacity) {
      capacity *= 2;
      Token* temp = (Token*)realloc(result, sizeof(Token) * capacity);
      if (temp == NULL) errExit(EMSG_MEMALLOC_FAILED);
      result = temp;
    }
  }
  result[stat->tokidx++] = (Token){ .kind = LAST_TOKEN, .string = "EndOfToken", .lineno = stat->lineno, .coloff = stat->coloff };
  free(stat);
  return result;
}

Token scanNumberLiteral(tokstat* stat) {
  int capacity = 10;
  char* buffer = (char*)malloc(sizeof(char) * capacity);
  if (buffer == NULL) errExit(EMSG_MEMALLOC_FAILED);
  int sline = stat->lineno, scol = stat->coloff;
  int idx = 0;
  while (isCharType(*stat->current, CharNumberLiteral)) {
    stat->coloff++;
    buffer[idx++] = *(stat->current++);
    if (idx >= capacity) {
      capacity *= 2;
      char* temp = (char*)realloc(buffer, sizeof(char) * capacity);
      if (temp == NULL) {
        free(buffer);
        errExit(EMSG_MEMALLOC_FAILED);
      }
      buffer = temp;
    }
  }
  if (*stat->current == '.') {
    stat->coloff++;
    buffer[idx++] = *(stat->current++);
    if (idx >= capacity) {
      capacity *= 2;
      char* temp = (char*)realloc(buffer, sizeof(char) * capacity);
      if (temp == NULL) {
        free(buffer);
        errExit(EMSG_MEMALLOC_FAILED);
      }
      buffer = temp;
    }
    while (isCharType(*stat->current, CharNumberLiteral)) {
      stat->coloff++;
      buffer[idx++] = *(stat->current++);
      if (idx >= capacity) {
        capacity *= 2;
        char* temp = (char*)realloc(buffer, sizeof(char) * capacity);
        if (temp == NULL) {
          free(buffer);
          errExit(EMSG_MEMALLOC_FAILED);
        }
        buffer = temp;
      }
    }
  }
  buffer[idx] = '\0';
  Token token;
  token.kind = NUMBER;
  token.lineno = sline;
  token.coloff = scol;
  token.e_lineno = stat->lineno;
  token.e_coloff = stat->coloff-1;
  token.string = buffer;
  return token;
}

Token scanStringLiteral(tokstat* stat) {
  int capacity = 10;
  char* buffer = (char*)malloc(sizeof(char) * capacity);
  if (buffer == NULL) errExit(EMSG_MEMALLOC_FAILED);
  int sline = stat->lineno, scol = stat->coloff;
  stat->coloff++;
  stat->current++;
  int idx = 0;
  while(isCharType(*stat->current, CharStringLiteral)) {
    stat->coloff++;
    buffer[idx++] = *(stat->current++);
    if (idx >= capacity) {
      capacity *= 2;
      char* temp = (char*)realloc(buffer, sizeof(char) * capacity);
      if (temp == NULL) {
        free(buffer);
        errExit(EMSG_MEMALLOC_FAILED);
      }
      buffer = temp;
    }
  }
  if (*stat->current != '\"') errExit(EMSG_CLOSE_STRING);
  stat->coloff++;
  stat->current++;
  buffer[idx] = '\0';
  Token token;
  token.kind = STRING;
  token.lineno = sline;
  token.coloff = scol;
  token.e_lineno = stat->lineno;
  token.e_coloff = stat->coloff-1;
  token.string = buffer;
  return token;
}

Token scanIdentAndKeyword(tokstat* stat) {
  int capacity = 10;
  char* buffer = (char*)calloc(capacity, sizeof(char));
  if (buffer == NULL) errExit(EMSG_MEMALLOC_FAILED);
  int sline = stat->lineno, scol = stat->coloff;
  int idx = 0;
  while(isCharType(*(stat->current), CharIdentAndKeyword)) {
    stat->coloff++;
    buffer[idx++] = *(stat->current++);
    if (idx >= capacity) {
      capacity *= 2;
      char* temp = (char*)realloc(buffer, sizeof(char) * capacity);
      if (temp == NULL) {
        free(buffer);
        errExit(EMSG_MEMALLOC_FAILED);
      }
      buffer = temp;
    }
  }
  buffer[idx] = '\0';
  PrimitiveCoreType type = parseType(buffer);
  Kind kind;
  if (type != CORETYPE_UNKNOWN) {
    kind = WJTYPE;
  } else {
    kind = parseKind(buffer);
    if (kind == UNKNOWN) kind = IDENTIFIER;
  }
  Token token;
  token.kind = kind;
  token.lineno = sline;
  token.coloff = scol;
  token.e_lineno = stat->lineno;
  token.e_coloff = stat->coloff-1;
  token.string = buffer;
  return token;
}

Token scanOperatorAndPunctuator(struct tokstat* stat) {
  int capacity = 10;
  char* string = (char*)calloc(capacity,sizeof(char));
  if (string == NULL) errExit(EMSG_MEMALLOC_FAILED);
  int sline = stat->lineno, scol = stat->coloff;
  int idx = 0;
  while (isCharType(*stat->current, CharOperatorAndPunctuator)) {
    stat->coloff++;
    string[idx++] = *(stat->current++);
    if (idx >= capacity) {
      capacity *= 2;
      char* temp = (char*)realloc(string, sizeof(char) * capacity);
      if (temp == NULL) {
        free(string);
        errExit(EMSG_MEMALLOC_FAILED);
      }
      string = temp;
    }
  }
  while (idx > 0 && parseKind(string) == UNKNOWN) {
    string[--idx] = '\0';
    stat->coloff--;
    stat->current--;
  }
  if (idx == 0) errExit(EMSG_INVALCH);
  string[idx] = '\0';
  Token token;
  token.kind = parseKind(string);
  token.lineno = sline;
  token.coloff = scol;
  token.e_lineno = stat->lineno;
  token.e_coloff = stat->coloff-1;
  token.string = string;
  return token;
}

CharType getCharType(char c) {
  if (' ' == c || '\t' == c || '\r' == c || '\n' == c || c == 32 || c == 10 || c == 0) return CharWhiteSpace;
  if ('0' <= c && c <= '9') return CharNumberLiteral;
  if (c == '\"') return CharStringLiteral;
  if (('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || '_' == c) return CharIdentAndKeyword;
  if (((33 <= c && c <= 47) && c != '\"') || (58 <= c && c <= 64) || (91 <= c && c <= 96) || (123 <= c && c <= 126)) return CharOperatorAndPunctuator;
  return CharUnknown;
}

bool isCharType(char c, CharType type) {
  switch (type) {
    case CharNumberLiteral: return ('0' <= c && c <= '9');
    case CharStringLiteral: return 32 <= c && c <= 126 && c != '\"';
    case CharIdentAndKeyword: return ('0' <= c && c <= '9') || ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
    case CharOperatorAndPunctuator: return (33 <= c && c <= 47) || (58 <= c && c <= 64) || (91 <= c && c <= 96) || (123 <= c && c <= 126);
    default: return false;
  }
}