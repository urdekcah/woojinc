#include <stdio.h>
#include <stdlib.h>
#include "woojin.h"
#include "error.h"
#include "type.h"
#include "parse.h"
#include "init.h"
#include "error.h"

type_t _const_i8_type;
type_t _const_i16_type;
type_t _const_i32_type;
type_t _const_i64_type;
type_t _const_u8_type;
type_t _const_u16_type;
type_t _const_u32_type;
type_t _const_u64_type;
type_t _const_f32_type;
type_t _const_f64_type;
type_t _const_char_type;
type_t _const_bool_type;
type_t _const_none_type;
type_t _const_string_type;
type_t _const_array_type;
type_t _const_map_type;
type_t _const_any_type;
type_t _const_error_type;

type_t w__parser__Parser_parse_any_type(w__parser__Parser* parser) {
  char* name = parser->current->token;

}

type_t w__parser__Parser_parse_type(w__parser__Parser* p) {
  
}

type_t w__ast__Type_set_flag(type_t t, TypeFlag flag) { return (((int)(t)) | ((1 << ((int)(((int)(flag)) + 24))))); }

type_t w__ast__Type_clear_flags(type_t t, TypeFlag flags) {
  
}

type_t w__new_type(int idx) {
  if (idx < 1 || idx > 65535) {
    ErrExit(FormatW(L"Invalid type index: %d", idx));
  }
  return idx;
}

void _tinit(void) {
  _const_i8_type = w__new_type(_const_i8_type_idx);
  _const_i16_type = w__new_type(_const_i16_type_idx);
  _const_i32_type = w__new_type(_const_i32_type_idx);
  _const_i64_type = w__new_type(_const_i64_type_idx);
  _const_u8_type = w__new_type(_const_u8_type_idx);
  _const_u16_type = w__new_type(_const_u16_type_idx);
  _const_u32_type = w__new_type(_const_u32_type_idx);
  _const_u64_type = w__new_type(_const_u64_type_idx);
  _const_f32_type = w__new_type(_const_f32_type_idx);
  _const_f64_type = w__new_type(_const_f64_type_idx);
  _const_char_type = w__new_type(_const_char_type_idx);
  _const_bool_type = w__new_type(_const_bool_type_idx);
  _const_none_type = w__new_type(_const_none_type_idx);
  _const_string_type = w__new_type(_const_string_type_idx);
  _const_array_type = w__new_type(_const_array_type_idx);
  _const_map_type = w__new_type(_const_map_type_idx);
  _const_any_type = w__new_type(_const_any_type_idx);
  _const_error_type = w__new_type(_const_error_type_idx);
}

type_t w__parser__Parser__parseArrayType(w__parser__Parser* p, TokenKind expect) {
  skipCurrentKind(p, expect);
}

type_t w__parser__Parser__parseAnyType(w__parser__Parser* parser, Language lang) {
  char* name = parser->current->token;
  skipCurrentIf(parser, TOKEN_IDENT);
  type_t ret = ((type_t)(0));
  if (parser->current->kind == TOKEN_LBRACKET) return w__parser__Parser__parseArrayType(parser, parser->current->kind);
  if (strcmp(name, "i8") == 0) {
    ret = _const_i8_type;
  } else if (strcmp(name, "i16") == 0) {
    ret = _const_i16_type;
  } else if (strcmp(name, "i32") == 0) {
    ret = _const_i32_type;
  } else if (strcmp(name, "i64") == 0) {
    ret = _const_i64_type;
  } else if (strcmp(name, "u8") == 0) {
    ret = _const_u8_type;
  } else if (strcmp(name, "u16") == 0) {
    ret = _const_u16_type;
  } else if (strcmp(name, "u32") == 0) {
    ret = _const_u32_type;
  } else if (strcmp(name, "u64") == 0) {
    ret = _const_u64_type;
  } else if (strcmp(name, "f32") == 0) {
    ret = _const_f32_type;
  } else if (strcmp(name, "f64") == 0) {
    ret = _const_f64_type;
  } else if (strcmp(name, "char") == 0) {
    ret = _const_char_type;
  } else if (strcmp(name, "bool") == 0) {
    ret = _const_bool_type;
  } else if (strcmp(name, "String") == 0) {
    ret = _const_string_type;
  } else {
    ErrExit(FormatW(L"Unknown type: %S", name));
  }
  return ret;
}