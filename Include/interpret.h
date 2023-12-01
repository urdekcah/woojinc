#ifndef __WOOJINC_INTERPRET_H__
#define __WOOJINC_INTERPRET_H__
#include "datatype.h"
#include "type.h"
#include "node.h"
#include "signals.h"
#include "list.h"

typedef struct VarDetail { wjvalue* value; PrimitiveCoreType type; bool mut; } VarDetail;

void interpret(Program *program);
wjvalue *interFunc(struct Function* func);
wjvalue *interVar(struct Variable* var);
wjvalue *interFor(struct For* _for);
wjvalue* interBreak(struct Break* stmt);
wjvalue* interContinue(struct Continue* stmt);
wjvalue *interStatment(Statement* stmt);
wjvalue *interReturn(struct Return* ret);
wjvalue* interIf(struct If* _if);
wjvalue *interExprStatement(struct ExprStatement* exprstmt);
wjvalue *interExpr(Expression* expr);
wjvalue interOr(struct Or* _or);
wjvalue interAnd(struct And* _and);
wjvalue interRelational(struct Relational* rel);
wjvalue interArithmetic(struct Arithmetic* ar);
wjvalue interUnary(struct Unary* unary);
wjvalue interCall(struct Call* call);
wjvalue interGetElem(struct GetElement* getelem);
wjvalue interSetElem(struct SetElement* setelem);
wjvalue interGetVar(struct GetVariable* get);
wjvalue interSetVar(struct SetVariable* set);
wjvalue interBoolLit(struct BooleanLiteral* _bool);
wjvalue interNumLit(struct NumberLiteral* num);
wjvalue interStrLit(struct StringLiteral* str);
wjvalue interArrayLit(struct ArrayLiteral* arr);
wjvalue interMapLiteral(struct MapLiteral* map);
#endif