#ifndef __WOOJINC_TOKEN_H__
#define __WOOJINC_TOKEN_H__
#include <stdio.h>
#include "config.h"

typedef struct TypeStrPair { char* str; PrimitiveCoreType coretype; } TypeStrPair;

typedef enum Kind {
  UNKNOWN, FIRST_TOKEN, LAST_TOKEN,
  // TrueLiteral, FalseLiteral,
  BOOLEAN,
  NUMBER, STRING, WJTYPE,
  IDENTIFIER, MUT,

  FUNCTION, RETURN, VARIABLE, FOR, BREAK, CONTINUE, IF, ELIF, ELSE,
  LOGICALAND, LOGICALOR, ASSIGNMENT, ADD, SUBTRACT, MULTIPLY, DIVIDE, MODULO,
  EQUAL, NOTEQUAL, LESSTHEN, GREATERTHAN, LESSOREQUAL, GREATEROREQUAL,

  COMMA, COLON, SEMI, LEFTPAREN, RIGHTPAREN, LEFTBRACE, RIGHTBRACE, LEFTBRAKET, RIGHTBRAKET
} Kind;

typedef struct KindStrPair { char* str; Kind kind; } KindStrPair;

typedef struct Token {
  Kind kind;
  int lineno, coloff, e_lineno, e_coloff;
  char* string;
} Token;

typedef struct tokstat {
  char* current;
  int tokidx, tokcap;
  int lineno, coloff;
  int state;
  int err;
  int done;
} tokstat;

Kind parseKind(char* string);
char* kindToString(Kind kind);
tokstat* newTokStat(char* string);
PrimitiveCoreType parseType(char* string);
PrimitiveCoreType wjtypeToCoreType(Kind kind);
char* typeToString(PrimitiveCoreType type);
#endif