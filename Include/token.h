#ifndef __WOOJINC_TOKEN_H__
#define __WOOJINC_TOKEN_H__
#include "woojin.h"

enum TokenKind {
  TOKEN_UNKNOWN,
  TOKEN_IDENT,
  TOKEN_ARROW,
  TOKEN_AMP,
  TOKEN_PLUS,
  TOKEN_MINUS,
  TOKEN_MUL,
  TOKEN_DIV,
  TOKEN_AND,
  TOKEN_PIPE,
  TOKEN_ANDAND,
  TOKEN_OR,
  TOKEN_NOT,
  TOKEN_DOLLAR,
  TOKEN_COLON,
  TOKEN_SEMI,
  TOKEN_COMMA,
  TOKEN_QUESTION,
  TOKEN_EQ,
  TOKEN_EQEQ,
  TOKEN_NE,
  TOKEN_GT,
  TOKEN_GE,
  TOKEN_LT,
  TOKEN_LE,
  TOKEN_LPAREN,
  TOKEN_RPAREN,
  TOKEN_LBRACE,
  TOKEN_RBRACE,
  TOKEN_LBRACKET,
  TOKEN_RBRACKET,
  TOKEN_COMMENT,
  TOKEN_DOT,
  TOKEN_DOTDOT,
  TOKEN_KW_AS,
  TOKEN_KW_LET,
  TOKEN_KW_BREAK,
  TOKEN_KW_CONST,
  TOKEN_KW_CONTINUE,
  TOKEN_KW_ELSE,
  TOKEN_KW_ENUM,
  TOKEN_KW_FOR,
  TOKEN_KW_FN,
  TOKEN_KW_GLOBAL,
  TOKEN_KW_IF,
  TOKEN_KW_IMPORT,
  TOKEN_KW_IN,
  TOKEN_KW_IS,
  TOKEN_KW_MUT,
  TOKEN_KW_YEE,
  TOKEN_KW_TYPE,
  TOKEN_KW_STATIC,
  TOKEN_KW_PUB,
  TOKEN_NUMBER_LITERAL,
  TOKEN_STRING_LITERAL,
  TOKEN_TRUE_LITERAL,
  TOKEN_FALSE_LITERAL,
  TOKEN_EOT
};

struct TokenPos {
  int len;
  int line, col;
  int end_line;
};

struct Token {
  const char *token;
  int line, col;
  int end_line, end_col;
  TokenKind kind;
};

TokenKind w__Token__parse_string__kind(const char* _a1);
char*     w__Token__kind_into__string(TokenKind _a1);
void      w__Token__free(Token* _a1);
TokenPos* w__TokenPos__from_Token(Token* _a1);
#endif