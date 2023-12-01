#ifndef __WOOJINC_NUMBER_H__
#define __WOOJINC_NUMBER_H__
#include "type.h"
#include "datatype.h"

wjvalue number_not(wjvalue value);
wjvalue number_add(wjvalue value1, wjvalue value2);
wjvalue number_sub(wjvalue value1, wjvalue value2);
wjvalue number_mul(wjvalue value1, wjvalue value2);
wjvalue number_div(wjvalue value1, wjvalue value2);
wjvalue number_mod(wjvalue value1, wjvalue value2);
wjvalue number_pow(wjvalue value1, wjvalue value2);
bool number_cmp(wjvalue value1, wjvalue value2, Kind kind);
// wjvalue* number_inc(wjvalue* value);
// wjvalue* number_dec(wjvalue* value);

bool number_is_zero(wjvalue* value);

#ifdef __WOOJINC_64BIT__
#define CMPNUM64(a, b, op) \
  if (a.type == CORETYPE_I128) return a.data.i128 op b; \
  if (a.type == CORETYPE_U128) return a.data.u128 op b; \

#define RCMPNUMP64(a,b,op,ret)
  if (a.type == CORETYPE_I128) ret a.data.i128 op b.data.i128; \
  if (a.type == CORETYPE_U128) ret a.data.u128 op b.data.u128; \

#define CMPNUMP64(a,b,op) RCMPNUMP64(a,b,op,return)
#else
#define CMPNUM64(value1, value2, op)
#define RCMPNUMP64(a,b,op,ret)
#define CMPNUMP64(a,b,op) RCMPNUMP64(a,b,op,return)
#endif

#define CMPNTYPE(a,b,MSG) if (a.type != b.type) errExit(MSG, TYPE(a.type), TYPE(b.type));

#define CMPNUM(a, b, op) \
  CMPNUM64(a, b, op) \
  if (a.type == CORETYPE_I8) return a.data.i8 op b; \
  if (a.type == CORETYPE_I16) return a.data.i16 op b; \
  if (a.type == CORETYPE_I32) return a.data.i32 op b; \
  if (a.type == CORETYPE_I64) return a.data.i64 op b; \
  if (a.type == CORETYPE_U8) return a.data.u8 op b; \
  if (a.type == CORETYPE_U16) return a.data.u16 op b; \
  if (a.type == CORETYPE_U32) return a.data.u32 op b; \
  if (a.type == CORETYPE_U64) return a.data.u64 op b; \
  if (a.type == CORETYPE_F32) return a.data.f32 op b; \
  if (a.type == CORETYPE_F64) return a.data.f64 op b; \
  if (a.type == CORETYPE_F128) return a.data.f128 op b; \
  errExit(EMSG_NOTNUMBER, wjvalueToString(&a));

#define RCMPNUMP(a, b, op, ret) \
  RCMPNUMP64(a,b,op, ret) \
  if (a.type == CORETYPE_I8) ret a.data.i8 op b.data.i8; \
  if (a.type == CORETYPE_I16) ret a.data.i16 op b.data.i16; \
  if (a.type == CORETYPE_I32) ret a.data.i32 op b.data.i32; \
  if (a.type == CORETYPE_I64) ret a.data.i64 op b.data.i64; \
  if (a.type == CORETYPE_U8) ret a.data.u8 op b.data.u8; \
  if (a.type == CORETYPE_U16) ret a.data.u16 op b.data.u16; \
  if (a.type == CORETYPE_U32) ret a.data.u32 op b.data.u32; \
  if (a.type == CORETYPE_U64) ret a.data.u64 op b.data.u64; \
  if (a.type == CORETYPE_F32) ret a.data.f32 op b.data.f32; \
  if (a.type == CORETYPE_F64) ret a.data.f64 op b.data.f64; \

#define CMPNUMP(a, b, op) RCMPNUMP(a, b, op, return)
#define CMPNUM_NRET(a, b, op) RCMPNUMP(a, b, op, )

#endif