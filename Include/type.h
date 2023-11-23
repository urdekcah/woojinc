#ifndef __WOOJINC_TYPE_H__
#define __WOOJINC_TYPE_H__
#include <stdbool.h>
#include "woojin.h"
#include "parse.h"

#define _const_i8_type_idx (1)
#define _const_i16_type_idx (_const_i8_type_idx+1)
#define _const_i32_type_idx (_const_i16_type_idx+1)
#define _const_i64_type_idx (_const_i32_type_idx+1)
#define _const_u8_type_idx (_const_i64_type_idx+1)
#define _const_u16_type_idx (_const_u8_type_idx+1)
#define _const_u32_type_idx (_const_u16_type_idx+1)
#define _const_u64_type_idx (_const_u32_type_idx+1)
#define _const_f32_type_idx (_const_u64_type_idx+1)
#define _const_f64_type_idx (_const_f32_type_idx+1)
#define _const_char_type_idx (_const_f64_type_idx+1)
#define _const_bool_type_idx (_const_char_type_idx+1)
#define _const_none_type_idx (_const_bool_type_idx+1)
#define _const_string_type_idx (_const_none_type_idx+1)
#define _const_array_type_idx (_const_string_type_idx+1)
#define _const_map_type_idx (_const_array_type_idx+1)
#define _const_any_type_idx (_const_map_type_idx+1)
#define _const_error_type_idx (_const_any_type_idx+1)

enum TypeFlag {
  Option,
  Result,

};

type_t w__ast__Type_set_flag(type_t t, TypeFlag flag);

enum WoojinKind {
  KindPlaceholder,
  Kindi8,
  Kindi16,
  Kindi32,
  Kindi64,
  Kindu8,
  Kindu16,
  Kindu32,
  Kindu64,
  Kindf32,
  Kindf64,
  KindChar,
  KindBool,
  KindNone,
  KindString,
  KindArray,
  KindMap,
  KindAny,
  KindInterface,
};

struct Alias {
  type_t parent_type;
  bool import;
};

enum TypeInfoKind {
  TypeInfoAlias,
};

struct TypeInfo {
  union {
    Alias _alias;
  };
  TypeInfoKind kind;
};

struct TypeSymbol {
  int parent_idx;
  TypeInfo tinfo;
  char* name;
  WoojinKind kind;
  bool pub;
  int idx;
};

type_t w__parser__Parser_parse_any_type(w__parser__Parser* parser);
type_t w__new_type(int idx);
#endif