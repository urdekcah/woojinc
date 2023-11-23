#ifndef __WOOJINC_EXPR_H__
#define __WOOJINC_EXPR_H__
#include <stdbool.h>
#include <stdint.h>
#include "woojin.h"
#include "list.h"
#include "ast.h"

typedef uint8_t w__ast__EmptyExpr;

enum w__ast__OpCode {
  OPCODE_ADD,
  OPCODE_SUB,
  OPCODE_MUL,
  OPCODE_DIV,
  OPCODE_MOD,
  OPCODE_AND,
  OPCODE_OR,
  OPCODE_INC,
  OPCODE_DEC,
};

enum w__ast__ExprKind {
  EXPR_EMPTY,
  EXPR_OR,
  EXPR_AND,
  EXPR_RELATIONAL,
  EXPR_ARITHMETIC,
  EXPR_UNARY,
  EXPR_CALL,
  EXPR_IF,
  EXPR_IDENT,
  EXPR_PAR,
  EXPR_POSTFIX,
  EXPR_INTEGER_LITERAL,
  EXPR_FLOAT_LITERAL,
  EXPR_BOOL_LITERAL,
  EXPR_STRING_LITERAL,
};

struct w__ast__OrExpr {
  w__ast__Expr *left;
  w__ast__Expr *right;
};

struct w__ast__AndExpr {
  w__ast__Expr *left;
  w__ast__Expr *right;
};

struct w__ast__RelationalExpr {
  w__ast__Expr *left;
  w__ast__Expr *right;
  w__ast__OpCode op;
};

struct w__ast__ArithmeticExpr {
  w__ast__Expr *left;
  w__ast__Expr *right;
  w__ast__OpCode op;
};

struct w__ast__UnaryExpr {
  w__ast__Expr *expr;
  w__ast__OpCode op;
};

struct w__ast__PostfixExpr {
  w__ast__Expr *expr;
  w__ast__OpCode op;
};

struct w__ast__ParExpr {
  w__ast__Expr *expr;
};

struct w__ast__CallExpr {
  char* name;
  bool method;
  bool field;
  bool noreturn: true;
  type_t reciever_type;
  Array_w__ast__CallArg args;
  Array_w__type_t *args_typeinfo;
};

struct w__ast__IfExpr {
  w__ast__Expr* cond;
  bool has_else;
  Array_w__ast__IfBranch branches;
};

struct w__ast__IfGuardVar {
  char* name;
  bool mut;
};

struct w__ast__Ident {
  char* name;
  w__ast__IdentKind kind;
  w__ast__IdentInfo info;
  bool mut;
};

enum IntegerType {
  INOTYET,
  I8,
  I16,
  I32,
  I64,
  U8,
  U16,
  U32,
  U64
};

enum FloatType {
  FNOTYET,
  F32,
  F64
};

struct w__ast__IntegerLiteral {
  char* val;
  IntegerType typ;
};

struct w__ast__FloatLiteral {
  char* val;
  FloatType typ;
};

struct w__ast__BoolLiteral {
  bool val;
};

struct w__ast__StringLiteral {
  char* val;
  bool is_raw;
};

struct w__ast__Expr {
  union {
    w__ast__EmptyExpr _w__ast__EmptyExpr;
    w__ast__OrExpr _w__ast__OrExpr;
    w__ast__AndExpr _w__ast__AndExpr;
    w__ast__RelationalExpr _w__ast__RelationalExpr;
    w__ast__ArithmeticExpr _w__ast__ArithmeticExpr;
    w__ast__UnaryExpr _w__ast__UnaryExpr;
    w__ast__CallExpr _w__ast__CallExpr;
    w__ast__IfExpr _w__ast__IfExpr;
    w__ast__Ident _w__ast__Ident;
    w__ast__ParExpr _w__ast__ParExpr;
    w__ast__PostfixExpr _w__ast__PostfixExpr;
    w__ast__IntegerLiteral _w__ast__IntegerLiteral;
    w__ast__FloatLiteral _w__ast__FloatLiteral;
    w__ast__BoolLiteral _w__ast__BoolLiteral;
    w__ast__StringLiteral _w__ast__StringLiteral;
  };
  w__ast__ExprKind kind;
};

w__ast__Expr w__ast__EmptyExpr_toSumType_Expr(w__ast__EmptyExpr _a1);
#define _const_w__ast__empty_expr w__ast__EmptyExpr_toSumType_Expr((w__ast__EmptyExpr)(0))
#endif