#ifndef __WOOJINC_NODE_H__
#define __WOOJINC_NODE_H__
#include <stdbool.h>
#include "woojin.h"
#include "list.h"
#include "expr.h"
#include "scope.h"
#include "statement.h"

enum w__ast__NodeKind {
  NODE_CALL_ARG,
  NODE_IF_BRANCH,
  NODE_EXPR,
  NODE_STATEMENT,
  NODE_PARAM,
  NODE_SCOPE_OBJECT,
};

struct w__ast__Node {
  union {
    w__ast__CallArg _w__ast__CallArg;
    w__ast__IfBranch _w__ast__IfBranch;
    w__ast__Expr _w__ast__Expr;
    w__ast__Param _w__ast__Param;
    ScopeObject _w__ast__ScopeObject;
    struct w__ast__Statement _w__ast__Statement;
  };
  w__ast__NodeKind kind;
};
#endif