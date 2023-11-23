#include <stdio.h>
#include "woojin.h"
#include "expr.h"
#include "mem.h"

w__ast__Expr w__ast__EmptyExpr_toSumType_Expr(w__ast__EmptyExpr _a1) {
  w__ast__EmptyExpr* ptr = memdup(&_a1, sizeof(w__ast__EmptyExpr));
	return (w__ast__Expr){ ._w__ast__EmptyExpr = ptr, .kind = EXPR_EMPTY };
}