#ifndef __WOOJINC_TOKENIZER_H__
#define __WOOJINC_TOKENIZER_H__
#include <stdbool.h>
#include "woojin.h"
#include "token.h"
#include "list.h"

struct Tokenizer {
  char* content;
  char* current;
  int lineno, coloff;
  List tokens;
};

enum CharType {
  CharUnknown,
  CharTypeChar,
  CharWhiteSpace,
  CharNumberLiteral,
  CharStringLiteral,
  CharIdentAndKeyword,
  CharOperatorAndPunctuator,
};

Tokenizer *w__Tokenizer__new(char* _a1);
Tokenizer *w__Tokenizer__scan(char* _a1);
Token w__Tokenizer__scan_Number(Tokenizer* _a1);
Token w__Tokenizer__scan_String(Tokenizer* _a1);
Token w__Tokenizer__scan_IdentKeyword(Tokenizer* _a1);
Token w__Tokenizer__scan_Operator_Punctuator(Tokenizer* _a1);
CharType w__Tokenizer__get_CharType(char _a1);
bool w__Tokenizer__is_CharType(char _a1, CharType _a2);
#endif