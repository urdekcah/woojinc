#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "type.h"
#include "datatype.h"

bool isSize(wjvalue value) { return false; }
bool isNull(wjvalue value) { return false; }

bool isTrue(wjvalue value) {
  if (value.type == CORETYPE_BOOL) return value.data._bool;
  return false;
}

bool isFalse(wjvalue value) {
  if (value.type == CORETYPE_BOOL) return value.data._bool == false;
  return false;
}

bool isBoolean(wjvalue value) { return is_bool(value); }

bool isNumber(wjvalue value) {
  if (value.type == CORETYPE_I8) return true;
  if (value.type == CORETYPE_I16) return true;
  if (value.type == CORETYPE_I32) return true;
  if (value.type == CORETYPE_I64) return true;
#ifdef __WOOJINC_64BIT__
  if (value.type == CORETYPE_I128) return true;
#endif
  if (value.type == CORETYPE_U8) return true;
  if (value.type == CORETYPE_U16) return true;
  if (value.type == CORETYPE_U32) return true;
  if (value.type == CORETYPE_U64) return true;
#ifdef __WOOJINC_64BIT__
  if (value.type == CORETYPE_U128) return true;
#endif
  if (value.type == CORETYPE_F32) return true;
  if (value.type == CORETYPE_F64) return true;
  if (value.type == CORETYPE_F128) return true;
  return false;
}

size_t toSize(wjvalue value) {
  if (value.type == CORETYPE_I8) return (size_t)value.data.i8;
  if (value.type == CORETYPE_I16) return (size_t)value.data.i16;
  if (value.type == CORETYPE_I32) return (size_t)value.data.i32;
  if (value.type == CORETYPE_I64) return (size_t)value.data.i64;
#ifdef __WOOJINC_64BIT__
  if (value.type == CORETYPE_I128) return (size_t)value.data.i128;
#endif
  if (value.type == CORETYPE_U8) return (size_t)value.data.u8;
  if (value.type == CORETYPE_U16) return (size_t)value.data.u16;
  if (value.type == CORETYPE_U32) return (size_t)value.data.u32;
  if (value.type == CORETYPE_U64) return (size_t)value.data.u64;
#ifdef __WOOJINC_64BIT__
  if (value.type == CORETYPE_U128) return (size_t)value.data.u128;
#endif
  if (value.type == CORETYPE_F32) return (size_t)value.data.f32;
  if (value.type == CORETYPE_F64) return (size_t)value.data.f64;
  if (value.type == CORETYPE_F128) return (size_t)value.data.f128;
  return 0;
}

wjvalue toNumber(wjvalue value) {
  if (
    value.type == CORETYPE_I8 ||
    value.type == CORETYPE_I16 ||
    value.type == CORETYPE_I32 ||
    value.type == CORETYPE_I64 ||
#ifdef __WOOJINC_64BIT__
    value.type == CORETYPE_I128 ||
#endif
    value.type == CORETYPE_U8 ||
    value.type == CORETYPE_U16 ||
    value.type == CORETYPE_U32 ||
    value.type == CORETYPE_U64 ||
#ifdef __WOOJINC_64BIT__
    value.type == CORETYPE_U128 ||
#endif
    value.type == CORETYPE_F32 ||
    value.type == CORETYPE_F64 ||
    value.type == CORETYPE_F128
  ) return value;
  printf("%d", value.type);
  errExit(EMSG_NOTNUMBER, wjvalueToString(&value));
  return value;
}

struct Function* toFunction(wjvalue value) {
  if (value.type == CORETYPE_FUNCTION) return value.data.fn;
  return NULL;
}

BuiltinFunction toBuiltinFunction(wjvalue value) {
  if (value.type == CORETYPE_BUILTIN) return value.data.builtin;
  return NULL;
}