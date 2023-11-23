#ifndef __WOOJINC_STATEMENT_H__
#define __WOOJINC_STATEMENT_H__
#include "woojin.h"
#include "expr.h"
#include "list.h"

enum w__ast__StatementKind {
  STATEMENT_ASSIGN,
  STATEMENT_FN_DECL,
  STATEMENT_BLOCK,
  STATEMENT_EXPR,
  STATEMENT_IMPORT,
  STATEMENT_RETURN,
  STATEMENT_FOR,
  STATEMENT_GLOBAL_DECL,
  STATEMENT_TYPE_DECL,
};

struct w__ast__Param {
  char* name;
  bool mut;
  type_t type;
};

struct w__ast__CallArg {
  bool mut;
  bool temporary;
  w__ast__Expr expr;
  type_t type;
};

struct w__ast__IfBranch {
  w__ast__Expr cond;
  Array_w__ast__Statement stmts;
};

struct w__ast__GlobalField {
  char* name;
  bool used;
  bool _volatile;
  w__ast__Expr *expr;
  type_t type;
};

struct w__ast__AssignStatement {
  w__ast__Expr *left;
  w__ast__Expr *right;
  type_t ltype;
  type_t rtype;
  bool _static;
  bool _volatile: true;
};

struct w__ast__FnDecl {
  char* name;
  bool noreturn: true;
  bool _main;
  bool used;
  bool method;
  bool noparam;
  bool nobody;
  bool builtin;
  bool hasret;
  Array_w__ast__Param* params;
  Array_w__ast__Statement stmts;
  type_t return_type;
  bool has_return;
};

struct w__ast__Block {
  Array_w__ast__Statement stmts;
};

struct w__ast__ExprStatement {
  w__ast__Expr expr;
};

struct w__ast__Import {
  char* name;
};

struct w__ast__Return {
  w__ast__Expr expr;
  type_t type;
};

struct w__ast__GlobalDecl {
  w__ast__GlobalField fields;
};

struct w__ast__ForStatement {
  w__ast__Statement* init;
  w__ast__Expr *cond;
  w__ast__Statement *inc;
  Array_w__ast__Statement stmts;
  bool h_init;
  bool h_cond;
  bool h_inc;
  bool inf;
};

struct w__ast__AliasTypeDecl {
  char* name;
  type_t type;
};

struct w__ast__TypeDecl {
  union {
    w__ast__AliasTypeDecl _w__ast__AliasTypeDecl;
  };
  char* name;
  bool pub: true;
};

struct w__ast__Statement {
  union {
    w__ast__AssignStatement _w__ast__AssignStatement;
    w__ast__FnDecl _w__ast__FnDecl;
    w__ast__Block _w__ast__Block;
    w__ast__ExprStatement _w__ast__ExprStatement;
    w__ast__Import _w__ast__Import;
    w__ast__Return _w__ast__Return;
    w__ast__ForStatement _w__ast__ForStatement;
    w__ast__GlobalDecl _w__ast__GlobalDecl;
    w__ast__TypeDecl _w__ast__TypeDecl;
  };
  w__ast__StatementKind kind;
};
#endif