#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "woojin.h"
#include "mem.h"
#include "token.h"

static struct {char* str; TokenKind kind;} w__Token__Token_String__Table[] = {
  {"->",       TOKEN_ARROW},
  {"&",        TOKEN_AMP},
  {"+",        TOKEN_PLUS},
  {"-",        TOKEN_MINUS},
  {"*",        TOKEN_MUL},
  {"/",        TOKEN_DIV},
  {"&",        TOKEN_AND},
  {"|",        TOKEN_PIPE},
  {"&&",       TOKEN_ANDAND},
  {"||",       TOKEN_OR},
  {"!",        TOKEN_NOT},
  {"$",        TOKEN_DOLLAR},
  {":",        TOKEN_COLON},
  {";",        TOKEN_SEMI},
  {",",        TOKEN_COMMA},
  {"?",        TOKEN_QUESTION},
  {"=",        TOKEN_EQ},
  {"==",       TOKEN_EQEQ},
  {"!=",       TOKEN_NE},
  {">",        TOKEN_GT},
  {">=",       TOKEN_GE},
  {"<",        TOKEN_LT},
  {"<=",       TOKEN_LE},
  {"//",       TOKEN_COMMENT},
  {".",        TOKEN_DOT},
  {"..",       TOKEN_DOTDOT},
  {"(",        TOKEN_LPAREN},
  {")",        TOKEN_RPAREN},
  {"{",        TOKEN_LBRACE},
  {"}",        TOKEN_RBRACE},
  {"[",        TOKEN_LBRACKET},
  {"]",        TOKEN_RBRACKET},
  {"as",       TOKEN_KW_AS},
  {"let",      TOKEN_KW_LET},
  {"break",    TOKEN_KW_BREAK},
  {"const",    TOKEN_KW_CONST},
  {"continue", TOKEN_KW_CONTINUE},
  {"else",     TOKEN_KW_ELSE},
  {"enum",     TOKEN_KW_ENUM},
  {"false",    TOKEN_FALSE_LITERAL},
  {"true",     TOKEN_TRUE_LITERAL},
  {"for",      TOKEN_KW_FOR},
  {"fn",       TOKEN_KW_FN},
  {"global",   TOKEN_KW_GLOBAL},
  {"if",       TOKEN_KW_IF},
  {"import",   TOKEN_KW_IMPORT},
  {"in",       TOKEN_KW_IN},
  {"is",       TOKEN_KW_IS},
  {"mut",      TOKEN_KW_MUT},
  {"yee",      TOKEN_KW_YEE},
  {"type",     TOKEN_KW_TYPE},
  {"static",   TOKEN_KW_STATIC},
  {"pub",      TOKEN_KW_PUB},
  {NULL}
};

TokenKind w__Token__parse_string__kind(const char* _a1) {
  for(size_t i=0;w__Token__Token_String__Table[i].str!=NULL;i++) {
    if (strcmp(w__Token__Token_String__Table[i].str, _a1) == 0)
      return w__Token__Token_String__Table[i].kind;
  }
  return TOKEN_UNKNOWN;
}

char* w__Token__kind_into__string(TokenKind _a1) {
  for(size_t i=0;w__Token__Token_String__Table[i].str!=NULL;i++) {
    if (w__Token__Token_String__Table[i].kind == _a1)
      return w__Token__Token_String__Table[i].str;
  }
  return NULL;
}

void w__Token__free(Token* _a1) {
  Free((void*)_a1->token);
  free(_a1);
}

TokenPos* w__TokenPos__from_Token(Token* _a1) {
  TokenPos* self = calloc(1,sizeof(TokenPos));
  if (self == NULL) ErrExit(E_MEMALLOC);
  self->len = _a1->end_col - _a1->col;
  self->line = _a1->line;
  self->col = _a1->col;
  self->end_line = _a1->end_line;
  return self;
}