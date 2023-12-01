#ifndef __WOOJINC_NODE_H__
#define __WOOJINC_NODE_H__
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "token.h"
#include "list.h"
#include "map.h"
#include "config.h"

typedef struct Program {
  // functions: List<Function*>
  List functions;
  int fnidx;
} Program;

typedef enum StatementType {
  StmtUnknown, StmtFunction, StmtVar, StmtReturn, StmtFor, StmtBreak, StmtContinue, StmtIf, StmtExpr
} StatementType;

typedef struct _Statement {
  union {
    struct Function* func;
    struct Variable* var;
    struct Return* ret;
    struct For* _for;
    struct Break* _break;
    struct Continue* _continue;
    struct If* _if;
    struct ExprStatement* expr;
  } data;
  StatementType type;
} Statement;

// params: List<char*>, block: List<Statement*>
typedef struct Function { char* name; List params; List block; } Function;
typedef struct Variable { char* name; struct Expression* value; PrimitiveCoreType type; bool mut; } Variable;
typedef struct Return { struct Expression* expression; } Return;
// block: List<Statement*>
typedef struct For { struct Variable* variable; struct Expression* condition; struct Expression* expression; List block; } For;
typedef struct Break Break;
typedef struct Continue Continue;
// conditions: List<Expression*>, blocks: List<List<Statement*>>, elseblock: List<Statement*>
typedef struct If { List* conditions; List* blocks; List* elseblock; } If;
typedef struct ExprStatement { struct Expression* expression; } ExprStatement;

typedef enum ExpressionType { 
  ExprUnknown, ExprAnd, ExprOr, ExprRelational, ExprArithmetic, ExprUnary, ExprCall,
  ExprGetElem, ExprSetElem, ExprGetVar, ExprSetVar, ExprBool, ExprNum, ExprString,
  ExprArray, ExprMap
} ExpressionType;

typedef struct Expression {
  union {
    struct Or* or;
    struct And* and;
    struct Relational* relational;
    struct Arithmetic* arithmetic;
    struct Unary* unary;
    struct Call* call;
    struct GetElement* getelem;
    struct SetElement* setelem;
    struct GetVariable* getvar;
    struct SetVariable* setvar;
    struct BooleanLiteral* boolean;
    struct NumberLiteral* number;
    struct StringLiteral* string;
    struct ArrayLiteral* array;
    struct MapLiteral* map;
  } data;
  enum ExpressionType type;
} Expression;

typedef struct Or { Expression* lhs; Expression* rhs; } Or;
typedef struct And { Expression* lhs; Expression* rhs; } And;
typedef struct Relational { Kind kind; Expression* lhs; Expression* rhs; } Relational;
typedef struct Arithmetic { Kind kind; Expression* lhs; Expression* rhs; } Arithmetic;
typedef struct Unary { Kind kind; Expression* sub; } Unary;
// arguments: List<Expression*>
typedef struct Call { Expression* sub; List arguments; } Call;
typedef struct GetElement { Expression* sub; Expression* index; } GetElement;
typedef struct SetElement { Expression* sub; Expression* index; Expression* value; } SetElement;
typedef struct GetVariable { char* name; } GetVariable;
typedef struct SetVariable { char* name; Expression* value; } SetVariable;
typedef struct BooleanLiteral { bool value; } BooleanLiteral;
typedef struct NumberLiteral {
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
    f128 f128;
  } value;
  PrimitiveCoreType type;
} NumberLiteral;
typedef struct StringLiteral { char* value; } StringLiteral;
// values: List<Expression*>
typedef struct ArrayLiteral { List values; } ArrayLiteral;
// values: Map<char*, Expression*>
typedef struct MapLiteral { Map values; } MapLiteral;
#endif