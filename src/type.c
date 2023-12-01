#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "node.h"
#include "type.h"
#include "interpret.h"
#include "list.h"
#include "map.h"

bool is_i8(wjvalue value) { return value.type == CORETYPE_I8 /* && value.data.i8 >= -128 && value.data.i8 <= 127 */; }
bool is_i16(wjvalue value) { return value.type == CORETYPE_I16 /* && value.data.i16 >= -32768 && value.data.i16 <= 32767 */; }
bool is_i32(wjvalue value) { return value.type == CORETYPE_I32 /* && value.data.i32 >= -2147483648 && value.data.i32 <= 2147483647 */; }
bool is_i64(wjvalue value) { return value.type == CORETYPE_I64; }
#ifdef __WOOJINC_64BIT__
bool is_i128(wjvalue value) { return value.type == CORETYPE_I128; }
#endif
bool is_u8(wjvalue value) { return value.type == CORETYPE_U8 /* && value.data.u8 >= 0 && value.data.u8 <= 255 */; }
bool is_u16(wjvalue value) { return value.type == CORETYPE_U16 /* && value.data.u16 >= 0 && value.data.u16 <= 65535 */; }
bool is_u32(wjvalue value) { return value.type == CORETYPE_U32 /* && value.data.u32 >= 0 && value.data.u32 <= 4294967295 */; }
bool is_u64(wjvalue value) { return value.type == CORETYPE_U64; }
#ifdef __WOOJINC_64BIT__
bool is_u128(wjvalue value) { return value.type == CORETYPE_U128; }
#endif
bool is_f32(wjvalue value) { return value.type == CORETYPE_F32; }
bool is_f64(wjvalue value) { return value.type == CORETYPE_F64; }
bool is_f128(wjvalue value) { return value.type == CORETYPE_F128; }
bool is_bool(wjvalue value) { return value.type == CORETYPE_BOOL; }
bool is_char(wjvalue value) { return value.type == CORETYPE_CHAR; }
bool is_str(wjvalue value) { return value.type == CORETYPE_STR; }
bool is_arr(wjvalue value) { return value.type == CORETYPE_ARRAY; }
bool is_map(wjvalue value) { return value.type == CORETYPE_OBJECT; }
bool is_stmt(wjvalue value) { return value.type == CORETYPE_STATEMENT; }
bool is_expr(wjvalue value) { return value.type == CORETYPE_EXPR; }
bool is_fn(wjvalue value) { return value.type == CORETYPE_FUNCTION; }
bool is_builtin(wjvalue value) { return value.type == CORETYPE_BUILTIN; }

wjvalue to_i8(i8 value) { return (wjvalue){.type = CORETYPE_I8, .data.i8 = value}; }
wjvalue to_i16(i16 value) { return (wjvalue){.type = CORETYPE_I16, .data.i16 = value}; }
wjvalue to_i32(i32 value) { return (wjvalue){.type = CORETYPE_I32, .data.i32 = value}; }
wjvalue to_i64(i64 value) { return (wjvalue){.type = CORETYPE_I64, .data.i64 = value}; }
#ifdef __WOOJINC_64BIT__
wjvalue to_i128(i128 value) { return (wjvalue){.type = CORETYPE_I128, .data.i128 = value}; }
#endif
wjvalue to_u8(u8 value) { return (wjvalue){.type = CORETYPE_U8, .data.u8 = value}; }
wjvalue to_u16(u16 value) { return (wjvalue){.type = CORETYPE_U16, .data.u16 = value}; }
wjvalue to_u32(u32 value) { return (wjvalue){.type = CORETYPE_U32, .data.u32 = value}; }
wjvalue to_u64(u64 value) { return (wjvalue){.type = CORETYPE_U64, .data.u64 = value}; }
#ifdef __WOOJINC_64BIT__
wjvalue to_u128(u128 value) { return (wjvalue){.type = CORETYPE_U128, .data.u128 = value}; }
#endif
wjvalue to_f32(f32 value) { return (wjvalue){.type = CORETYPE_F32, .data.f32 = value}; } 
wjvalue to_f64(f64 value) { return (wjvalue){.type = CORETYPE_F64, .data.f64 = value}; }
wjvalue to_f128(f128 value) { return (wjvalue){.type = CORETYPE_F128, .data.f128 = value}; }
wjvalue to_bool(bool value) { return (wjvalue){.type = CORETYPE_BOOL, .data._bool = value}; }
wjvalue to_char(char value) { return (wjvalue){.type = CORETYPE_CHAR, .data._char = value}; }
wjvalue to_str(str value) { return (wjvalue){.type = CORETYPE_STR, .data.str = value}; }
wjvalue to_arr(List* value) { return (wjvalue){.type = CORETYPE_ARRAY, .data.arr = value}; }
wjvalue to_map(Map* value) { return (wjvalue){.type = CORETYPE_OBJECT, .data.map = value}; }
wjvalue to_stmt(Statement* value) { return (wjvalue){.type = CORETYPE_STATEMENT, .data.stmt = value}; }
wjvalue to_expr(Expression* value) { return (wjvalue){.type = CORETYPE_EXPR, .data.expr = value}; }
wjvalue to_fn(struct Function* value) { return (wjvalue){.type = CORETYPE_EXCEPTION, .data.fn = value}; }
wjvalue to_builtin(BuiltinFunction* value) { return (wjvalue){.type = CORETYPE_BUILTIN, .data.builtin = *value}; }
wjvalue to_null() { return (wjvalue){.type = CORETYPE_NULL}; }

wjvalue* malwv(wjvalue value) {
  wjvalue* result = (wjvalue*)calloc(1,sizeof(wjvalue));
  *result = value;
  return result;
}

void print_wjvalue(wjvalue value) { printf("%s", wjvalueToString(&value)); }

char* wjvalueToString(wjvalue* value) {
  char* str = (char*)calloc(100,sizeof(char));
  if (str == NULL) errExit(EMSG_MEMALLOC_FAILED);
  switch (value->type) {
    case CORETYPE_I8: sprintf(str, "%d", value->data.i8); break;
    case CORETYPE_I16: sprintf(str, "%d", value->data.i16); break;
    case CORETYPE_I32: sprintf(str, "%d", value->data.i32); break;
    case CORETYPE_I64: sprintf(str, "%ld", value->data.i64); break;
#ifdef __WOOJINC_64BIT__
    case CORETYPE_I128: sprintf(str, "%lld", value->data.i128); break;
#endif
    case CORETYPE_U8: sprintf(str, "%u", value->data.u8); break;
    case CORETYPE_U16: sprintf(str, "%u", value->data.u16); break;
    case CORETYPE_U32: sprintf(str, "%u", value->data.u32); break;
    case CORETYPE_U64: sprintf(str, "%lu", value->data.u64); break;
#ifdef __WOOJINC_64BIT__
    case CORETYPE_U128: sprintf(str, "%llu", value->data.u128); break;
#endif
    case CORETYPE_F32: sprintf(str, "%f", value->data.f32); break;
    case CORETYPE_F64: sprintf(str, "%lf", value->data.f64); break;
    case CORETYPE_F128: sprintf(str, "%Lf", value->data.f128); break; // Lf?
    case CORETYPE_BOOL: sprintf(str, "%s", value->data._bool ? "uglyguri" : "beautifulguri"); break;
    case CORETYPE_CHAR: sprintf(str, "%c", value->data._char); break;
    case CORETYPE_STR: {
      size_t len = strlen(value->data.str);
      char* temp = (char*)realloc(str, sizeof(char) * (len + 1));
      if (temp == NULL) {
        free(str);
        errExit(EMSG_MEMALLOC_FAILED);
      }
      str = temp;
      strncpy(str, value->data.str, len);
      str[len + 1] = '\0';
      break;
    }
    case CORETYPE_ARRAY:{
      size_t bufsize = 1024;
      char* arr = (char*)calloc(bufsize,sizeof(char));
      if (arr == NULL) {
        free(str);
        errExit(EMSG_MEMALLOC_FAILED);
      }
      size_t len = 0;
      snprintf(arr, bufsize, "[");
      len += strlen(arr);
      for (size_t i = 0; i < value->data.arr->size; i++) {
        char* elem = wjvalueToString(value->data.arr->data[i]);
        size_t elen = strlen(elem);
        while (len + elen + 3 >= bufsize) {
          bufsize *= 2;
          arr = (char*)realloc(arr, sizeof(char) * bufsize);
          if (arr == NULL) {
            free(str);
            errExit(EMSG_MEMALLOC_FAILED);
          }
        }
        strncpy(arr + len, elem, bufsize - len);
        len += elen;
        free(elem);
        if (i != value->data.arr->size - 1) {
          strncpy(arr + len, ", ", bufsize - len);
          len += 2;
        }
      }
      strncpy(arr + len, "]", bufsize - len);
      len += 1;
      char* temp = (char*)realloc(str, sizeof(char) * len);
      if (temp == NULL) {
        free(str);
        free(arr);
        errExit(EMSG_MEMALLOC_FAILED);
      }
      str = temp;
      strncat(str, arr, len);
      free(arr);
      break;
    }
    case CORETYPE_OBJECT:{
      size_t bufsize = 1024;
      char* map = (char*)calloc(bufsize,sizeof(char));
      if (map == NULL) {
        free(str);
        errExit(EMSG_MEMALLOC_FAILED);
      }
      size_t len = 0;
      snprintf(map, bufsize, "{");
      len += strlen(map);
      for (size_t i = 0; i < value->data.map->size; i++) {
        char* key = value->data.map->data[i].key;
        char* val = wjvalueToString((wjvalue*)value->data.map->data[i].value);
        size_t klen = strlen(key);
        size_t vlen = strlen(val);
        while (len + klen + vlen + 4 >= bufsize) {
          bufsize *= 2;
          map = (char*)realloc(map, sizeof(char) * bufsize);
          if (map == NULL) {
            free(str);
            errExit(EMSG_MEMALLOC_FAILED);
          }
        }
        strncpy(map + len, key, bufsize - len);
        len += klen;
        strncpy(map + len, ": ", bufsize - len);
        len += 2;
        strncpy(map + len, val, bufsize - len);
        len += vlen;
        free(val);
        if (i != value->data.map->size - 1) {
          strncpy(map + len, ", ", bufsize - len);
          len += 2;
        }
      }
      strncpy(map + len, "}", bufsize - len);
      len += 1;
      char* temp = (char*)realloc(str, sizeof(char) * len);
      if (temp == NULL) {
        free(str);
        free(map);
        errExit(EMSG_MEMALLOC_FAILED);
      }
      str = temp;
      strncat(str, map, len);
      free(map);
      break;
    }
    case CORETYPE_STATEMENT: sprintf(str, "%s", wjvalueToString(interStatment(value->data.stmt))); break;
    case CORETYPE_EXPR: sprintf(str, "%s", wjvalueToString(interExpr(value->data.expr))); break;
    case CORETYPE_FUNCTION: sprintf(str, "%s", wjvalueToString(interFunc(value->data.fn))); break;
    case CORETYPE_BUILTIN: sprintf(str, "<woojin:builtin function>"); break;
    case CORETYPE_NULL: sprintf(str, "null"); break;
    default: sprintf(str, "unknown"); break;
  }
  return str;
}

List* wjvalueToList(wjvalue** value, size_t arrsize) {
  List* result = createList();
  for (size_t i = 0; i<arrsize; i++) {appendList(result, (void*)value[i]);}
  return result;
}

wjvalue getValueOfArray(wjvalue object, wjvalue index) {
  size_t i = toSize(index);
  if (object.type == CORETYPE_ARRAY) return *(wjvalue*)object.data.arr->data[i];
  return to_null();
}

wjvalue setValueOfArray(wjvalue object, wjvalue index, wjvalue value) {
  size_t i = toSize(index);
  if (object.type == CORETYPE_ARRAY) replaceList(object.data.arr, i, (void*)&value);
  return object;
}

wjvalue getValueOfMap(wjvalue object, wjvalue key) {
  if (object.type == CORETYPE_OBJECT) {
    for (size_t i = 0; i < object.data.map->size; i++) {
      if (strcmp(object.data.map->data[i].key, wjvalueToString(&key)) == 0) {
        return *(wjvalue*)object.data.map->data[i].value;
      }
    }
  }
  return to_null();
}

wjvalue setValueOfMap(wjvalue object, wjvalue key, wjvalue value) {
  replaceMap(object.data.map, wjvalueToString(&key), (void*)&value);
  return to_null();
}

wjvalue NumberLiteralToWJValue(NumberLiteral* value) {
  switch (value->type) {
    case CORETYPE_I8: return to_i8(value->value.i8);
    case CORETYPE_I16: return to_i16(value->value.i16);
    case CORETYPE_I32: return to_i32(value->value.i32);
    case CORETYPE_I64: return to_i64(value->value.i64);
#ifdef __WOOJINC_64BIT__
    case CORETYPE_I128: return to_i128(value->value.i128);
#endif
    case CORETYPE_U8: return to_u8(value->value.u8);
    case CORETYPE_U16: return to_u16(value->value.u16);
    case CORETYPE_U32: return to_u32(value->value.u32);
    case CORETYPE_U64: return to_u64(value->value.u64);
#ifdef __WOOJINC_64BIT__
    case CORETYPE_U128: return to_u128(value->value.u128);
#endif
    case CORETYPE_F32: return to_f32(value->value.f32);
    case CORETYPE_F64: return to_f64(value->value.f64);
    case CORETYPE_F128: return to_f128(value->value.f128);
    default: errExit(EMSG_NOTNUMBER, wjvalueToString((wjvalue*)value));
  }
  return to_null();
}