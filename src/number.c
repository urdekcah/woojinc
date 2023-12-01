#include <math.h>
#include "error.h"
#include "number.h"

wjvalue number_not(wjvalue value) {
  if (value.type == CORETYPE_I8) value.data.i8 = -value.data.i8;
  else if (value.type == CORETYPE_I16) value.data.i16 = -value.data.i16;
  else if (value.type == CORETYPE_I32) value.data.i32 = -value.data.i32;
  else if (value.type == CORETYPE_I64) value.data.i64 = -value.data.i64;
#ifdef __WOOJINC_64BIT__
  else if (value.type == CORETYPE_I128) value.data.i128 = -value.data.i128;
#endif
  else if (value.type == CORETYPE_U8) value.data.u8 = -value.data.u8;
  else if (value.type == CORETYPE_U16) value.data.u16 = -value.data.u16;
  else if (value.type == CORETYPE_U32) value.data.u32 = -value.data.u32;
  else if (value.type == CORETYPE_U64) value.data.u64 = -value.data.u64;
#ifdef __WOOJINC_64BIT__
  else if (value.type == CORETYPE_U128) value.data.u128 = -value.data.u128;
#endif
  else if (value.type == CORETYPE_F32) value.data.f32 = -value.data.f32;
  else if (value.type == CORETYPE_F64) value.data.f64 = -value.data.f64;
  else if (value.type == CORETYPE_F128) value.data.f128 = -value.data.f128;
  else errExit(EMSG_NOTNUMBER, wjvalueToString(&value));
  return value;
}

wjvalue number_add(wjvalue value1, wjvalue value2) {
  CMPNTYPE(value1, value2, EMSG_CANNOT_ADD)
  CMPNUM_NRET(value1, value2, +=); return value1;
}

wjvalue number_sub(wjvalue value1, wjvalue value2) { 
  CMPNTYPE(value1, value2, EMSG_CANNOT_SUB)
  CMPNUM_NRET(value1, value2, -=); return value1;
}

wjvalue number_mul(wjvalue value1, wjvalue value2) {
  CMPNTYPE(value1, value2, EMSG_CANNOT_MUL)
  CMPNUM_NRET(value1, value2, *=); return value1;
}

wjvalue number_div(wjvalue value1, wjvalue value2) {
  CMPNTYPE(value1, value2, EMSG_CANNOT_DIVIDE)
  CMPNUM_NRET(value1, value2, /=); return value1;
}

wjvalue number_mod(wjvalue value1, wjvalue value2) {
  CMPNTYPE(value1, value2, EMSG_CANNOT_MOD)
  if (value1.type == CORETYPE_I8) value1.data.i8 %= value2.data.i8;
  else if (value1.type == CORETYPE_I16) value1.data.i16 %= value2.data.i16;
  else if (value1.type == CORETYPE_I32) value1.data.i32 %= value2.data.i32;
  else if (value1.type == CORETYPE_I64) value1.data.i64 %= value2.data.i64;
#ifdef __WOOJINC_64BIT__
  else if (value1.type == CORETYPE_I128) value1.data.i128 %= value2.data.i128;
#endif
  else if (value1.type == CORETYPE_U8) value1.data.u8 %= value2.data.u8;
  else if (value1.type == CORETYPE_U16) value1.data.u16 %= value2.data.u16;
  else if (value1.type == CORETYPE_U32) value1.data.u32 %= value2.data.u32;
  else if (value1.type == CORETYPE_U64) value1.data.u64 %= value2.data.u64;
#ifdef __WOOJINC_64BIT__
  else if (value1.type == CORETYPE_U128) value1.data.u128 %= value2.data.u128;
#endif
  else if (value1.type == CORETYPE_F32) value1.data.f32 = fmod(value1.data.f32, value2.data.f32);
  else if (value1.type == CORETYPE_F64) value1.data.f64 = fmod(value1.data.f64, value2.data.f64);
  else if (value1.type == CORETYPE_F128) value1.data.f128 = fmod(value1.data.f128, value2.data.f128);
  else errExit(EMSG_NOTNUMBER, wjvalueToString(&value1));
  return value1;
}

wjvalue number_pow(wjvalue value1, wjvalue value2) {
  if (value1.type == CORETYPE_I8) value1.data.i8 = pow(value1.data.i8, value2.data.i8);
  else if (value1.type == CORETYPE_I16) value1.data.i16 = pow(value1.data.i16, value2.data.i16);
  else if (value1.type == CORETYPE_I32) value1.data.i32 = pow(value1.data.i32, value2.data.i32);
  else if (value1.type == CORETYPE_I64) value1.data.i64 = pow(value1.data.i64, value2.data.i64);
#ifdef __WOOJINC_64BIT__
  else if (value1.type == CORETYPE_I128) value1.data.i128 = pow(value1.data.i128, value2.data.i128);
#endif
  else if (value1.type == CORETYPE_U8) value1.data.u8 = pow(value1.data.u8, value2.data.u8);
  else if (value1.type == CORETYPE_U16) value1.data.u16 = pow(value1.data.u16, value2.data.u16);
  else if (value1.type == CORETYPE_U32) value1.data.u32 = pow(value1.data.u32, value2.data.u32);
  else if (value1.type == CORETYPE_U64) value1.data.u64 = pow(value1.data.u64, value2.data.u64);
#ifdef __WOOJINC_64BIT__
  else if (value1.type == CORETYPE_U128) value1.data.u128 = pow(value1.data.u128, value2.data.u128);
#endif
  else if (value1.type == CORETYPE_F32) value1.data.f32 = pow(value1.data.f32, value2.data.f32);
  else if (value1.type == CORETYPE_F64) value1.data.f64 = pow(value1.data.f64, value2.data.f64);
  else if (value1.type == CORETYPE_F128) value1.data.f128 = pow(value1.data.f128, value2.data.f128);
  else errExit(EMSG_NOTNUMBER, wjvalueToString(&value1));
  return value1;
}

bool number_is_zero(wjvalue* value) {
  CMPNUM((*value), 0, ==);
  errExit(EMSG_NOTNUMBER, wjvalueToString(value));
  return false;
}

bool number_cmp(wjvalue value1, wjvalue value2, Kind kind) {
  switch (kind) {
    case EQUAL: CMPNUMP(value1, value2, ==); break;
    case NOTEQUAL: CMPNUMP(value1, value2, !=); break;
    case LESSTHEN: CMPNUMP(value1, value2, <); break;
    case GREATERTHAN: CMPNUMP(value1, value2, >); break;
    case LESSOREQUAL: CMPNUMP(value1, value2, <=); break;
    case GREATEROREQUAL: CMPNUMP(value1, value2, >=); break;
    default: errExit(EMSG_NOTNUMBER, wjvalueToString(&value1));
  }
  return false;
}