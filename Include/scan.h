#ifndef __WOOJINC_SCAN_H__
#define __WOOJINC_SCAN_H__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "error.h"
#include "token.h"

typedef enum CharType {
  CharUnknown,
  CharTypeChar,
  CharWhiteSpace,
  CharNumberLiteral,
  CharStringLiteral,
  CharIdentAndKeyword,
  CharOperatorAndPunctuator,
} CharType;

Token scanNumberLiteral(tokstat* stat);
Token scanStringLiteral(tokstat* stat);
Token scanIdentAndKeyword(tokstat* stat);
Token scanOperatorAndPunctuator(tokstat* stat);
CharType getCharType(char c);
bool isCharType(char c, CharType type);
Token* scanCode(char* code);
#endif