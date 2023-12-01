#ifndef __WOOJINC_TYPE_H__
#define __WOOJINC_TYPE_H__
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <ctype.h>
#include "signals.h"
#include "error.h"
#include "list.h"
#include "map.h"
#include "config.h"
#include "node.h"

// struct Array;
// struct Map;

// type to string
#define TYPE(x) \
  (x == CORETYPE_I8 ? "i8" : \
  (x == CORETYPE_I16 ? "i16" : \
  (x == CORETYPE_I32 ? "i32" : \
  (x == CORETYPE_I64 ? "i64" : \
  (x == CORETYPE_I128 ? "i128" : \
  (x == CORETYPE_U8 ? "u8" : \
  (x == CORETYPE_U16 ? "u16" : \
  (x == CORETYPE_U32 ? "u32" : \
  (x == CORETYPE_U64 ? "u64" : \
  (x == CORETYPE_U128 ? "u128" : \
  (x == CORETYPE_F32 ? "f32" : \
  (x == CORETYPE_F64 ? "f64" : \
  (x == CORETYPE_F128 ? "f128" : \
  (x == CORETYPE_BOOL ? "bool" : \
  (x == CORETYPE_CHAR ? "char" : \
  (x == CORETYPE_STR ? "str" : \
  (x == CORETYPE_ARRAY ? "array" : \
  (x == CORETYPE_OBJECT ? "object" : \
  (x == CORETYPE_FUNCTION ? "function" : \
  (x == CORETYPE_BUILTIN ? "builtin" : \
  (x == CORETYPE_EXCEPTION ? "exception" : \
  (x == CORETYPE_NULL ? "null" : \
  "unknown"))))))))))))))))))))))

typedef struct wjvalue {
  union {
    i8 i8;
    i16 i16;
    i32 i32;
    i64 i64;
#ifdef __WOOJINC_64BIT__
    i128 i128;
#endif
    u8 u8;
    u16 u16;
    u32 u32;
    u64 u64;
#ifdef __WOOJINC_64BIT__
    u128 u128;
#endif
    f32 f32;
    f64 f64;
    bool _bool;
    f128 f128;
    char _char;
    str str;
    List* arr;
    Map* map;
    Statement* stmt;
    Expression* expr;
    struct Function* fn;
    // BuiltinFunction* builtin;
    struct wjvalue* (*builtin)(List*);
    struct Signal* _sig;
  } data;
  PrimitiveCoreType type;
} wjvalue;

// struct Array { size_t size; size_t capacity; struct wjvalue* elements; };
// struct Map { size_t size; size_t capacity; struct wjvalue* keys; struct  wjvalue* values; };
struct Object { bool isMarked; union { List array; Map map; } data; };
typedef wjvalue* (*BuiltinFunction)(List*);
List* wjvalueToList(wjvalue** value, size_t arrsize);
wjvalue getValueOfArray(wjvalue object, wjvalue index);
wjvalue setValueOfArray(wjvalue object, wjvalue index, wjvalue value);
wjvalue getValueOfMap(wjvalue object, wjvalue key);
wjvalue setValueOfMap(wjvalue object, wjvalue key, wjvalue value);

bool is_i8(wjvalue value);
bool is_i16(wjvalue value);
bool is_i32(wjvalue value);
bool is_i64(wjvalue value);
bool is_i128(wjvalue value);
bool is_u8(wjvalue value);
bool is_u16(wjvalue value);
bool is_u32(wjvalue value);
bool is_u64(wjvalue value);
bool is_u128(wjvalue value);
bool is_f32(wjvalue value);
bool is_f64(wjvalue value);
bool is_f128(wjvalue value);
bool is_bool(wjvalue value);
bool is_char(wjvalue value);
bool is_str(wjvalue value);
bool is_arr(wjvalue value);
bool is_map(wjvalue value);
bool is_stmt(wjvalue value);
bool is_expr(wjvalue value);
bool is_fn(wjvalue value);
bool is_builtin(wjvalue value);
wjvalue to_i8(i8 value);
wjvalue to_i16(i16 value);
wjvalue to_i32(i32 value);
wjvalue to_i64(i64 value);
#ifdef __WOOJINC_64BIT__
wjvalue to_i128(i128 value);
#endif
wjvalue to_u8(u8 value);
wjvalue to_u16(u16 value);
wjvalue to_u32(u32 value);
wjvalue to_u64(u64 value);
#ifdef __WOOJINC_64BIT__
wjvalue to_u128(u128 value);
#endif
wjvalue to_f32(f32 value);
wjvalue to_f64(f64 value);
wjvalue to_f128(f128 value);
wjvalue to_bool(bool value);
wjvalue to_char(char value);
wjvalue to_str(str value);
wjvalue to_arr(List* value);
wjvalue to_map(Map* value);
wjvalue to_stmt(Statement* value);
wjvalue to_expr(Expression* value);
wjvalue to_fn(struct Function* value);
wjvalue to_builtin(BuiltinFunction* value);
wjvalue to_null();

void print_wjvalue(wjvalue value);
char* wjvalueToString(wjvalue* value);
wjvalue* malwv(wjvalue value);
wjvalue NumberLiteralToWJValue(NumberLiteral* value);
#endif