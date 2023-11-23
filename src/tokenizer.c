#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stddef.h>
#include "error.h"
#include "token.h"
#include "tokenizer.h"
#include "mem.h"
#include "list.h"

#define w__Tokenizer__mAppend_alltok(_m1,_m2) {Token*_m3=(Token*)calloc(1,sizeof(Token));if(_m3==NULL)ErrExit(E_MEMALLOC);*_m3=_m2;__list_append_list(&_m1->tokens,(void*)_m3);}

Tokenizer *w__Tokenizer__scan(char* _a1) {
  Tokenizer *_r1 = w__Tokenizer__new(_a1);
  while(*(_r1->current)!='\0') {
    switch(w__Tokenizer__get_CharType(*(_r1->current))) {
      case CharWhiteSpace: 
        if (*_r1->current == '\n') {
          _r1->lineno++;
          _r1->coloff = 0;
          _r1->current++;
        } else {
          _r1->coloff++;
          _r1->current++;
        }
        break;
      case CharNumberLiteral: w__Tokenizer__mAppend_alltok(_r1, w__Tokenizer__scan_Number(_r1)); break;
      case CharStringLiteral: w__Tokenizer__mAppend_alltok(_r1, w__Tokenizer__scan_String(_r1)); break;
      case CharIdentAndKeyword: w__Tokenizer__mAppend_alltok(_r1, w__Tokenizer__scan_IdentKeyword(_r1)); break;
      case CharOperatorAndPunctuator: w__Tokenizer__mAppend_alltok(_r1, w__Tokenizer__scan_Operator_Punctuator(_r1)); break;
      default: exit(1);
    }
  }
  Token _r2 = (Token){ .kind = TOKEN_EOT, .token = "eot", .line = _r1->lineno, .col = _r1->coloff,};
  w__Tokenizer__mAppend_alltok(_r1,_r2);
  return _r1;
}

Token w__Tokenizer__scan_Number(Tokenizer* _a1) {
  int _r1 = 10;
  char* _r2 = (char*)calloc(_r1,sizeof(char));
  if (_r2 == NULL) ErrExit(E_MEMALLOC);
  int _r3 = _a1->lineno, _r4 = _a1->coloff, _r5 = 0;
  while (w__Tokenizer__is_CharType(*(_a1->current), CharNumberLiteral)) {
    _a1->coloff++;_r2[_r5++] = *(_a1->current++);
    if (_r5 >= _r1) {
      _r1 *= 2;
      char* _r6 = (char*)realloc(_r2, sizeof(char) * _r1);
      if (_r6 == NULL) MULTI(FreeAll(_r2); ErrExit(E_MEMALLOC));
      _r2 = _r6;
    }
  }
  if (*_a1->current == '.') {
    _a1->coloff++;_r2[_r5++] = *(_a1->current++);
    if (_r5 >= _r1) {
      _r1 *= 2;
      char* _r6 = (char*)realloc(_r2, sizeof(char) * _r1);
      if (_r6 == NULL) MULTI(FreeAll(_r2); ErrExit(E_MEMALLOC));
      _r2 = _r6;
    }
    while (w__Tokenizer__is_CharType(*(_a1->current), CharNumberLiteral)) {
      _a1->coloff++;_r2[_r5++] = *(_a1->current++);
      if (_r5 >= _r1) {
        _r1 *= 2;
        char* _r6 = (char*)realloc(_r2, sizeof(char) * _r1);
        if (_r6 == NULL) MULTI(FreeAll(_r2);ErrExit(E_MEMALLOC));
        _r2 = _r6;
      }
    }
  }
  _r2[_r5] = '\0';
  return (Token){
    .kind = TOKEN_NUMBER_LITERAL,
    .line = _r3,
    .col = _r4,
    .end_line = _a1->lineno,
    .end_col = _a1->coloff-1,
    .token = _r2
  };
}

Token w__Tokenizer__scan_String(Tokenizer* _a1) {
  int _r1 = 10;
  char* _r2 = (char*)calloc(_r1,sizeof(char));
  if (_r2 == NULL) ErrExit(E_MEMALLOC);
  int _r3 = _a1->lineno, _r4 = _a1->coloff, _r5 = 0;
  _a1->coloff++;_a1->current++;
  while (w__Tokenizer__is_CharType(*(_a1->current), CharStringLiteral)) {
    _a1->coloff++;
    _r2[_r5++] = *(_a1->current++);
    if (_r5 >= _r1) {
      _r1 *= 2;
      char* _r6 = (char*)realloc(_r2, sizeof(char) * _r1);
      if (_r6 == NULL) MULTI(FreeAll(_r2);ErrExit(E_MEMALLOC));
      _r2 = _r6;
    }
  }
  if (*_a1->current != '\"') ErrExit(E_UNCLOSEDSTR);
  _a1->coloff++;_a1->current++;
  _r2[_r5] = '\0';
  return (Token){
    .kind = TOKEN_STRING_LITERAL,
    .line = _r3,
    .col = _r4,
    .end_line = _a1->lineno,
    .end_col = _a1->coloff-1,
    .token = _r2
  };
}

Token w__Tokenizer__scan_IdentKeyword(Tokenizer* _a1) {
  int _r1 = 10;
  char* _r2 = (char*)calloc(_r1,sizeof(char));
  if (_r2 == NULL) ErrExit(E_MEMALLOC);
  int _r3 = _a1->lineno, _r4 = _a1->coloff, _r5 = 0;
  while (w__Tokenizer__is_CharType(*(_a1->current), CharIdentAndKeyword)) {
    _a1->coloff++;_r2[_r5++] = *(_a1->current++);
    if (_r5 >= _r1) {
      _r1 *= 2;
      char* _r6 = (char*)realloc(_r2, sizeof(char) * _r1);
      if (_r6 == NULL) MULTI(FreeAll(_r2);ErrExit(E_MEMALLOC));
      _r2 = _r6;
    }
  }
  _r2[_r5] = '\0';
  TokenKind k = w__Token__parse_string__kind(_r2);
  if (k == TOKEN_UNKNOWN) k = TOKEN_IDENT;
  return (Token){
    .kind = k,
    .line = _r3,
    .col = _r4,
    .end_line = _a1->lineno,
    .end_col = _a1->coloff-1,
    .token = _r2
  };
}

Token w__Tokenizer__scan_Operator_Punctuator(Tokenizer* _a1) {
  int _r1 = 10;
  char* _r2 = (char*)calloc(_r1,sizeof(char));
  if (_r2 == NULL) ErrExit(E_MEMALLOC);
  int _r3 = _a1->lineno, _r4 = _a1->coloff, _r5 = 0;
  while (w__Tokenizer__is_CharType(*(_a1->current), CharOperatorAndPunctuator)) {
    _a1->coloff++;_r2[_r5++] = *(_a1->current++);
    if (_r5 >= _r1) {
      _r1 *= 2;
      char* _r6 = (char*)realloc(_r2, sizeof(char) * _r1);
      if (_r6 == NULL) MULTI(FreeAll(_r2);ErrExit(E_MEMALLOC));
      _r2 = _r6;
    }
  }
  while (_r5 > 0 && w__Token__parse_string__kind(_r2) == TOKEN_UNKNOWN) {
    _r2[--_r5] = '\0';
    _a1->coloff--;
    _a1->current--;
  }
  if (_r5 == 0) ErrExit(E_UNKNOWNCH);
  _r2[_r5] = '\0';
  return (Token){
    .kind = w__Token__parse_string__kind(_r2),
    .line = _r3,
    .col = _r4,
    .end_line = _a1->lineno,
    .end_col = _a1->coloff-1,
    .token = _r2
  };
}

CharType w__Tokenizer__get_CharType(char _a1) {
  if (' ' == _a1 || '\t' == _a1 || '\r' == _a1 || '\n' == _a1 || _a1 == 32 || _a1 == 10 || _a1 == 0) return CharWhiteSpace;
  if ('0' <= _a1 && _a1 <= '9') return CharNumberLiteral;
  if (_a1 == '\"') return CharStringLiteral;
  if (('a' <= _a1 && _a1 <= 'z') || ('A' <= _a1 && _a1 <= 'Z') || '_' == _a1) return CharIdentAndKeyword;
  if (((33 <= _a1 && _a1 <= 47) && _a1 != '\"') || (58 <= _a1 && _a1 <= 64) || (91 <= _a1 && _a1 <= 96) || (123 <= _a1 && _a1 <= 126)) return CharOperatorAndPunctuator;
  return CharUnknown;
}

bool w__Tokenizer__is_CharType(char _a1, CharType _a2) {
  switch(_a2) {
    case CharNumberLiteral: return ('0' <= _a1 && _a1 <= '9');
    case CharStringLiteral: return 32 <= _a1 && _a1 <= 126 && _a1 != '\"';
    case CharIdentAndKeyword: return ('0' <= _a1 && _a1 <= '9') || ('a' <= _a1 && _a1 <= 'z') || ('A' <= _a1 && _a1 <= 'Z');
    case CharOperatorAndPunctuator: return (33 <= _a1 && _a1 <= 47) || (58 <= _a1 && _a1 <= 64) || (91 <= _a1 && _a1 <= 96) || (123 <= _a1 && _a1 <= 126);
    default: return false;
  }
}

Tokenizer *w__Tokenizer__new(char* _a1) {
  Tokenizer *_r1 = (Tokenizer *)calloc(1,sizeof(Tokenizer));
  if (_r1 == NULL) ErrExit(E_MEMALLOC);
  _r1->current = _a1;
  _r1->lineno = 1;
  _r1->coloff = 0;
  _r1->content = _a1;
  __list_init(&_r1->tokens);
  return _r1;
}