#ifndef __WOOJINC_LITERAL_H__
#define __WOOJINC_LITERAL_H__
#include <stdint.h>
#include <stdbool.h>

typedef enum {
  LITERAL_INTEGER,
  LITERAL_FLOAT,
  LITERAL_STRING,
  LITERAL_BOOL,
  LITERAL_IDENTIFIER,
} LiteralType;

typedef enum {
  INTEGER_I8,
  INTEGER_I16,
  INTEGER_I32,
  INTEGER_I64,
  INTEGER_U8,
  INTEGER_U16,
  INTEGER_U32,
  INTEGER_U64,
} NumberLiteralType;

typedef enum {
  FLOAT_F32,
  FLOAT_F64,
} FloatLiteralType;

typedef struct {
  union {
    struct {
      union {
        int8_t i8;
        int16_t i16;
        int32_t i32;
        int64_t i64;
        uint8_t u8;
        uint16_t u16;
        uint32_t u32;
        uint64_t u64;
      } value;
      NumberLiteralType type;
    } integer;
    struct {
      union {
        float f32;
        double f64;
      } value;
      FloatLiteralType type;
    } _float;
    struct {
      char *value;
      int length, capacity;
    } string;
    struct {
      bool value;
    } _bool;
    struct {
      char *value;
    } identifier;
  } value;
  LiteralType type;
} Literal;
#endif