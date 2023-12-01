#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "type.h"
#include "node.h"
#include "datatype.h"
#include "list.h"

wjvalue* builtin_print(List* values) {
  for (size_t i = 0; i < values->size; i++) {
    wjvalue* value = (wjvalue*)values->data[i];
    print_wjvalue(*value);
  }
  wjvalue* result = (wjvalue*)calloc(1,sizeof(wjvalue));
  *result = to_null();
  return result;
}

wjvalue* bulitin_println(List* values) {
  builtin_print(values);
  printf("\n");
  wjvalue* result = (wjvalue*)calloc(1,sizeof(wjvalue));
  *result = to_null();
  return result;
}

wjvalue* type(List* values) {
  wjvalue value = *(wjvalue*)values->data[0];
  wjvalue* result = (wjvalue*)calloc(1,sizeof(wjvalue));
  switch (value.type) {
    case CORETYPE_I8: *result = to_str("i8"); break;
    case CORETYPE_I16: *result = to_str("i16"); break;
    case CORETYPE_I32: *result = to_str("i32"); break;
    case CORETYPE_I64: *result = to_str("i64"); break;
#ifdef __WOOJINC_64BIT__
    case CORETYPE_I128: *result = to_str("i128"); break;
#endif
    case CORETYPE_U8: *result = to_str("u8"); break;
    case CORETYPE_U16: *result = to_str("u16"); break;
    case CORETYPE_U32: *result = to_str("u32"); break;
    case CORETYPE_U64: *result = to_str("u64"); break;
#ifdef __WOOJINC_64BIT__
    case CORETYPE_U128: *result = to_str("u128"); break;
#endif
    case CORETYPE_F32: *result = to_str("f32"); break;
    case CORETYPE_F64: *result = to_str("f64"); break;
    case CORETYPE_BOOL: *result = to_str("bool"); break;
    case CORETYPE_STR: *result = to_str("str"); break;
    case CORETYPE_ARRAY: *result = to_str("array"); break;
    case CORETYPE_OBJECT: *result = to_str("object"); break;
    case CORETYPE_FUNCTION: *result = to_str("function"); break;
    case CORETYPE_BUILTIN: *result = to_str("builtin_function"); break;
    case CORETYPE_NULL: *result = to_str("null"); break;
    default: *result = to_str("unknown"); break;
  }
  return result;
}

int getBuiltinFunctionTableSize() {
  return BUILTIN_SIZE;
}

FuncTable builtinFunctionTable[] = {
  {"print",   (wjvalue* (*)(List*))builtin_print},
  {"println", (wjvalue* (*)(List*))bulitin_println},
  {"type",    (wjvalue* (*)(List*))type},
};