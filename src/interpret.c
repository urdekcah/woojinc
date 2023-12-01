#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "datatype.h"
#include "type.h"
#include "node.h"
#include "signals.h"
#include "list.h"
#include "map.h"
#include "interpret.h"
#include "number.h"

static Map *globalTable; // key: char* value: wjvalue*
static List *localTable; // VarDetail*
static Map *functionTable; // key: char* value: Function*
extern FuncTable builtinFunctionTable[];
extern ExceTable exctable[];

void initLocalMap(List *list) {
  initList(list);
  List *list2 = createList();
  appendList(list, list2);
  Map *map = createMap();
  appendList(list2, map);
}

int findFn(char *name) {
  for (size_t i = 0; i<functionTable->size; i++)
    if (strcmp(functionTable->data[i].key, name) == 0) return i;
  return -1;
}

void interpret(Program *program) {
  localTable = createList();
  globalTable = createMap();
  functionTable = createMap();
  initLocalMap(localTable);
  for (size_t i = 0; i<program->functions.size; i++) {
    Function *fn = program->functions.data[i];
    pushMap(functionTable, fn->name, fn);
  }
  int mainx = findFn("main");
  if (mainx == -1) errExit(EMSG_MAIN_FUNC_NOTFOUND);
  emplaceFrontList((List*)emplaceBackList(localTable, (void *)createList()), createMap());
  interFunc((Function*)functionTable->data[mainx].value);
  popBackList(localTable);
  for (size_t i = 0; i<localTable->size; i++) {
    List *list = (List *)localTable->data[i];
    for (size_t j = 0; j<list->size; j++) {
      Map *map = (Map *)list->data[j];
      for (size_t k = 0; k<map->size; k++) {
        VarDetail *detail = (VarDetail *)map->data[k].value;
        free(detail);
      }
      free(map->data);
      free(map);
    }
    free(list->data);
    free(list);
  }
}

wjvalue *interFunc(struct Function* func) {
  for (size_t i = 0;i<func->block.size; i++)
    interStatment((Statement*)func->block.data[i]);
  return malwv(to_null());
}

wjvalue *interVar(struct Variable* var) {
  Map *table = (Map*)frontList((List *)backList(localTable));
  wjvalue *value = interExpr(var->value);
  if ((var->type != CORETYPE_ANY) && (value->type != var->type)) errExit(EMSG_TYPE_MISMATCH, var->type, value->type);
  VarDetail* detail = (VarDetail*)calloc(1,sizeof(VarDetail));
  if (detail == NULL) errExit(EMSG_MEMALLOC_FAILED);
  detail->value = value;
  detail->type = (var->type != CORETYPE_ANY)? var->type : value->type;
  detail->mut = var->mut;
  pushMap(table, var->name, (void*)detail);
  return value;
}

wjvalue *interFor(struct For* _for) {
  // List* lastLocal = (List*)backList(localTable);
  emplaceFrontList((List*)backList(localTable), createMap());
  // List* new = createList();
  // pushFrontList(localTable, new);
  interVar(_for->variable);
  while (true) {
    wjvalue *result = interExpr(_for->condition);
    if (isTrue(*result) == false) break;
    for (size_t i = 0; i < _for->block.size; i++) {
      wjvalue* res = interStatment((Statement*)_for->block.data[i]);
      if (res->type == CORETYPE_EXCEPTION) {
        if (res->data._sig->type == CONTINUE_SIGNAL) { /* continue; 처리 */ }
        else if (res->data._sig->type == BREAK_SIGNAL) { break; }
      }
    }
    interExpr(_for->expression);
  }
  popFrontList((List*)backList(localTable));
  return malwv(to_null());
}

wjvalue* interBreak(struct Break* stmt) {
  wjvalue *result = (wjvalue*)malloc(sizeof(wjvalue));
  if (result == NULL) errExit(EMSG_MEMALLOC_FAILED);
  result->type = CORETYPE_EXCEPTION;
  result->data._sig = (Signal*)malloc(sizeof(Signal));
  if (result->data._sig == NULL) {
    free(result);
    errExit(EMSG_MEMALLOC_FAILED);
  }
  result->data._sig->type = BREAK_SIGNAL;
  result->data._sig->_sig.breakSignal = (BreakSignal*)malloc(sizeof(BreakSignal));
  if (result->data._sig->_sig.breakSignal == NULL) {
    free(result->data._sig);
    free(result);
    errExit(EMSG_MEMALLOC_FAILED);
  }
  free(stmt);
  return result;
}

wjvalue* interContinue(struct Continue* stmt) {
  wjvalue *result = (wjvalue*)malloc(sizeof(wjvalue));
  if (result == NULL) errExit(EMSG_MEMALLOC_FAILED);
  result->type = CORETYPE_EXCEPTION;
  result->data._sig = (Signal*)malloc(sizeof(Signal));
  if (result->data._sig == NULL) {
    free(result);
    errExit(EMSG_MEMALLOC_FAILED);
  }
  result->data._sig->type = CONTINUE_SIGNAL;
  result->data._sig->_sig.continueSignal = (ContinueSignal*)malloc(sizeof(ContinueSignal));
  if (result->data._sig->_sig.continueSignal == NULL) {
    free(result->data._sig);
    free(result);
    errExit(EMSG_MEMALLOC_FAILED);
  }
  free(stmt);
  return result;
}

wjvalue* interIf(struct If* _if) {
  for (size_t i = 0; i < _if->conditions->size; i++) {
    wjvalue *result = interExpr((Expression*)_if->conditions->data[i]);
    if (isFalse(*result)) continue;
    emplaceFrontList((List*)backList(localTable), createMap());
    for (size_t j = 0; j < _if->blocks->size; j++) {
      List* block = (List*)_if->blocks->data[j];
      for (size_t k = 0; k < block->size; k++) interStatment((Statement*)block->data[k]);
    }
    popFrontList((List*)backList(localTable));
    return malwv(to_null());
    // return result;
  }
  if (_if->elseblock->capacity <= 0) return malwv(to_null());
  emplaceFrontList((List*)backList(localTable), createMap());
  for (size_t i = 0; i < _if->elseblock->size; i++) interStatment((Statement*)_if->elseblock->data[i]);
  popFrontList((List*)backList(localTable));
  return malwv(to_null());
}

wjvalue *interExprStatement(struct ExprStatement* exprstmt) { return interExpr(exprstmt->expression); }
wjvalue interOr(struct Or* _or) { return isTrue(*interExpr(_or->lhs)) ? to_bool(true) : isTrue(*interExpr(_or->rhs))? to_bool(true) : to_bool(false); }
wjvalue interAnd(struct And* _and) { return isFalse(*interExpr(_and->lhs)) ? to_bool(false): isFalse(*interExpr(_and->rhs)) ? to_bool(false) : to_bool(true); }

wjvalue *interStatment(Statement* stmt) {
  wjvalue* result = NULL;
  switch (stmt->type) {
    case StmtUnknown: break;
    case StmtFunction: result = interFunc(stmt->data.func); break;
    case StmtVar: result = interVar(stmt->data.var); break;
    case StmtReturn: result = interReturn(stmt->data.ret); break;
    case StmtFor: result = interFor(stmt->data._for); break;
    case StmtBreak: result = interBreak(stmt->data._break); break;
    case StmtContinue: result = interContinue(stmt->data._continue); break;
    case StmtIf: result = interIf(stmt->data._if); break;
    case StmtExpr: result = interExprStatement(stmt->data.expr); break;
  }
  return result;
}

wjvalue* interReturn(struct Return* ret) {
  wjvalue* result = (wjvalue*)malloc(sizeof(wjvalue));
  if (result == NULL) errExit(EMSG_MEMALLOC_FAILED);
  result->type = CORETYPE_EXCEPTION;
  result->data._sig = (Signal*)malloc(sizeof(Signal));
  if (result->data._sig == NULL) {
    free(result);
    errExit(EMSG_MEMALLOC_FAILED);
  }
  result->data._sig->type = RETURN_SIGNAL;
  result->data._sig->_sig.returnSignal = (ReturnSignal*)malloc(sizeof(ReturnSignal));
  if (result->data._sig->_sig.returnSignal == NULL) {
    free(result->data._sig);
    free(result);
    errExit(EMSG_MEMALLOC_FAILED);
  }
  result->data._sig->_sig.returnSignal->result = interExpr(ret->expression);
  return result;
}

wjvalue *interExpr(Expression* expr) {
  wjvalue* result = (wjvalue*)malloc(sizeof(wjvalue));
  switch (expr->type) {
    case ExprUnknown: result=NULL; break;
    case ExprAnd: *result=interAnd(expr->data.and); break;
    case ExprOr: *result=interOr(expr->data.or); break;
    case ExprRelational: *result=interRelational(expr->data.relational); break;
    case ExprArithmetic: *result=interArithmetic(expr->data.arithmetic); break;
    case ExprUnary: *result=interUnary(expr->data.unary); break;
    case ExprCall: *result=interCall(expr->data.call); break;
    case ExprGetElem: *result=interGetElem(expr->data.getelem); break;
    case ExprSetElem: *result=interSetElem(expr->data.setelem); break;
    case ExprGetVar: *result=interGetVar(expr->data.getvar); break;
    case ExprSetVar: *result=interSetVar(expr->data.setvar); break;
    case ExprBool: *result=interBoolLit(expr->data.boolean); break;
    case ExprNum: *result=interNumLit(expr->data.number); break;
    case ExprString: *result=interStrLit(expr->data.string); break;
    case ExprArray: *result=interArrayLit(expr->data.array); break;
    case ExprMap: *result=interMapLiteral(expr->data.map); break;
  }
  return result;
}

wjvalue interRelational(struct Relational* rel) {
  wjvalue *lhs = interExpr(rel->lhs);
  wjvalue *rhs = interExpr(rel->rhs);
  if (lhs == NULL || rhs == NULL) errExit(EMSG_INVOPERA_RELATIONAL);
  if (rel->kind == EQUAL && is_bool(*lhs) && is_bool(*rhs)) return to_bool(lhs->data._bool == rhs->data._bool);
  if (rel->kind == EQUAL && isNumber(*lhs) && isNumber(*rhs)) return to_bool(lhs->data.f64 == rhs->data.f64);
  if (rel->kind == EQUAL && is_str(*lhs) && is_str(*rhs)) return to_bool(strcmp(lhs->data.str, rhs->data.str) == 0);
  if (rel->kind == NOTEQUAL && is_bool(*lhs) && is_bool(*rhs)) return to_bool(lhs->data._bool != rhs->data._bool);
  if (rel->kind == NOTEQUAL && isNumber(*lhs) && isNumber(*rhs)) return to_bool(number_cmp(*lhs, *rhs, NOTEQUAL));
  if (rel->kind == NOTEQUAL && is_str(*lhs) && is_str(*rhs)) return to_bool(strcmp(lhs->data.str, rhs->data.str) != 0);
  if (rel->kind == LESSTHEN && isNumber(*lhs) && isNumber(*rhs)) return to_bool(number_cmp(*lhs, *rhs, LESSTHEN));
  if (rel->kind == GREATERTHAN && isNumber(*lhs) && isNumber(*rhs)) return to_bool(number_cmp(*lhs, *rhs, GREATERTHAN));
  if (rel->kind == LESSOREQUAL && isNumber(*lhs) && isNumber(*rhs)) return to_bool(number_cmp(*lhs, *rhs, LESSOREQUAL));
  if (rel->kind == GREATEROREQUAL && isNumber(*lhs) && isNumber(*rhs)) return to_bool(number_cmp(*lhs, *rhs, GREATEROREQUAL));
  return to_bool(false);
}

wjvalue interArithmetic(struct Arithmetic* ar) {
  wjvalue *lhs = interExpr(ar->lhs);
  wjvalue *rhs = interExpr(ar->rhs);
  if (lhs == NULL || rhs == NULL) errExit(EMSG_INVOPERA_ARITHMETIC);
  if (ar->kind == ADD && isNumber(*lhs) && isNumber(*rhs)) return number_add(toNumber(*lhs), toNumber(*rhs));
  if (ar->kind == ADD && is_str(*lhs) && is_str(*rhs)) {
    char* str = (char*)calloc(1, sizeof(char) * (strlen(lhs->data.str) + strlen(rhs->data.str) + 1));
    if (str == NULL) errExit(EMSG_MEMALLOC_FAILED);
    strcpy(str, lhs->data.str);
    strcat(str, rhs->data.str);
    return to_str(str);
  }
  if (ar->kind == SUBTRACT && isNumber(*lhs) && isNumber(*rhs)) return number_sub(toNumber(*lhs), toNumber(*rhs));
  if (ar->kind == MULTIPLY && isNumber(*lhs) && isNumber(*rhs)) return number_mul(toNumber(*lhs), toNumber(*rhs));
  if (ar->kind == DIVIDE && isNumber(*lhs) && isNumber(*rhs)) {
    if (number_is_zero(rhs)) errExit(EMSG_CANNOT_DIVIDE_BYZ);
    return number_div(toNumber(*lhs), toNumber(*rhs));
  }
  if (ar->kind == MODULO && isNumber(*lhs) && isNumber(*rhs)) {
    if (number_is_zero(rhs)) errExit(EMSG_CANNOT_MODULO_BYZ);
    return number_mod(toNumber(*lhs), toNumber(*rhs));
  }
  return to_f64(0.0);
}

wjvalue interUnary(struct Unary* unary) {
  wjvalue *sub = interExpr(unary->sub);
  if (sub == NULL) errExit(EMSG_INVOPERA_UNARY);
  if (unary->kind == ADD) {
    if (isNumber(*sub)) {return toNumber(*sub);}
  } else if (unary->kind == SUBTRACT && isNumber(*sub)) { return number_not(toNumber(*sub)); }
  return to_f64(0.0);
}

wjvalue interCall(struct Call* call) {
  wjvalue* value = interExpr(call->sub);
  if (value == NULL) errExit(EMSG_INVOPERA);
  if (is_builtin(*value)) {
    wjvalue** args = (wjvalue**)calloc(call->arguments.size,sizeof(wjvalue*));
    if (args == NULL) errExit(EMSG_MEMALLOC_FAILED);
    for (size_t i = 0; i<call->arguments.size; i++) {
      wjvalue* arg = interExpr((Expression*)call->arguments.data[i]);
      args[i] = arg;
    }
    BuiltinFunction func = toBuiltinFunction(*value);
    if (func == NULL) errExit(EMSG_UNKNOWN_BUILTIN);
    return *func(wjvalueToList(args, call->arguments.size));
  }
  if (is_fn(*value) == false) return to_null();
  Map* params = createMap();
  for (size_t i = 0; i<call->arguments.size; i++) {
    char* name = toFunction(*value)->params.data[i];
    wjvalue* arg = interExpr((Expression*)call->arguments.data[i]);
    pushMap(params, name, arg);
  }
  emplaceFrontList((List*)backList(localTable), params);
  wjvalue* result = interFunc(toFunction(*value));
  if (result->type == CORETYPE_EXCEPTION) {
    if (result->data._sig->type == RETURN_SIGNAL) {
      wjvalue* value = result->data._sig->_sig.returnSignal->result;
      popBackList(localTable);
      return *value;
    }
  }
  popBackList(localTable);
  return to_null(); // return nullptr;
}

wjvalue interGetElem(struct GetElement* getelem) {
  if (getelem == NULL) errExit(EMSG_INVOPERA);
  wjvalue* object = interExpr(getelem->sub);
  wjvalue* index = interExpr(getelem->index);
  if (object == NULL || index == NULL) errExit(EMSG_INVOPERA);
  if (is_arr(*object)) return getValueOfArray(*object, *index);
  if (is_map(*object)) return getValueOfMap(*object, *index);
  return to_null();
}

wjvalue interSetElem(struct SetElement* setelem) {
  if (setelem == NULL) errExit(EMSG_INVOPERA);
  wjvalue* object = interExpr(setelem->sub);
  if (object == NULL) errExit(EMSG_INVOPERA);
  wjvalue* index = interExpr(setelem->index);
  wjvalue* value = interExpr(setelem->value);
  if (index == NULL || value == NULL) errExit(EMSG_INVOPERA);
  if (is_arr(*object) && isNumber(*index)) return setValueOfArray(*object, *index, *value);
  if (is_map(*object) && is_str(*index)) return setValueOfMap(*object, *index, *value);
  return to_null();
}

wjvalue interGetVar(struct GetVariable* get) {
  List* vars = (List *)backList(localTable);
  for (size_t i = 0; i<vars->size; i++) {
    Map* map = (Map *)vars->data[i];
    VarDetail* value = (VarDetail *)getMap(map, get->name);
    if (value != NULL) return *value->value;
  }
  VarDetail* value = (VarDetail *)getMap(globalTable, get->name);
  if (value != NULL) return *value->value;
  Function* fn = (Function *)getMap(functionTable, get->name);
  if (fn != NULL) return to_fn(fn);
  for (int i = 0; i<BUILTIN_SIZE; i++) 
    if (strcmp(builtinFunctionTable[i].str, get->name) == 0) return to_builtin(&builtinFunctionTable[i].fn);
  rt_err(WoojinExce_UnresolvedName, get->name);
  return to_null();
}

wjvalue interSetVar(struct SetVariable* set) {
  List* loc = (List *)backList(localTable);
  for (size_t i = 0; i<loc->size; i++) {
    Map* map = (Map *)loc->data[i];
    VarDetail* value = (VarDetail *)getMap(map, set->name);
    if (value != NULL) {
      wjvalue* result = interExpr(set->value);
      if (value->mut == false) errExit(EMSG_CANNOT_ASSIGN);
      if (value->type != result->type) errExit(EMSG_TYPE_MISMATCH, TYPE(value->type), TYPE(result->type));
      value->value = result;
      return *result;
    }
  }
  // replaceMap(globalTable, set->name, (void*)interExpr(set->value));
  // VarDetail* new = (VarDetail*)calloc(1, sizeof(VarDetail));
  // if (new == NULL) errExit(EMSG_MEMALLOC_FAILED);
  // new->value = interExpr(set->value);
  // new->type = new->value->type;
  // new->mut = false;
  // pushMap((Map*)backList(localTable), set->name, (void*)new);
  errExit(L"몰?루");
  return to_null();
}

wjvalue interBoolLit(struct BooleanLiteral* _bool) { return to_bool(_bool->value); }
wjvalue interNumLit(struct NumberLiteral* num) { return NumberLiteralToWJValue(num); }
wjvalue interStrLit(struct StringLiteral* str) { return to_str(str->value); }

wjvalue interArrayLit(struct ArrayLiteral* arr) {
  List* result = createList();
  for (size_t i = 0; i<arr->values.size; i++) {
    wjvalue* value = interExpr((Expression*)arr->values.data[i]);
    appendList(result, (void*)value);
  }
  return to_arr(result);
}

wjvalue interMapLiteral(struct MapLiteral* map) {
  Map* result = createMap();
  for (size_t i = 0; i<map->values.size; i++) 
    replaceMap(result, map->values.data[i].key, (void*)interExpr((Expression*)map->values.data[i].value));
  return to_map(result);
}