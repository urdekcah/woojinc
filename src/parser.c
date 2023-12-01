#include <stdio.h>
#include <stdlib.h>
#include "token.h"
#include "node.h"
#include "parser.h"
#include "error.h"
#include "list.h"
#include "map.h"

Program* program_new() {
  Program* program = (Program*)calloc(1, sizeof(Program));
  program->functions = *createList();
  program->fnidx = 0;
  return program;
}

// void program_init(Program* program) {
//   program->functions = *createList();
//   program->fnidx = 0;
// }

Program* parse(Token* tokens) {
  Program* result = program_new();
  struct Parser* parser = (struct Parser*)malloc(sizeof(struct Parser));
  if (parser == NULL) errExit(EMSG_MEMALLOC_FAILED);
  parser_init(parser, tokens);
  while(parser->current->kind != LAST_TOKEN) {
    switch(parser->current->kind) {
      case FUNCTION: appendList(&result->functions, (void*)parseFunc(parser)); break;
      default: errExit(EMSG_INVAL_STMT, parser->current->string, parser->current->lineno);break;
    }
  }
  free(parser);
  return result;
}

Function* parseFunc(struct Parser* parser) {
  Function* result = (struct Function*)calloc(1, sizeof(struct Function));
  skipCurrentKind(parser, FUNCTION);
  result->name = strdup(parser->current->string);
  skipCurrentKind(parser, IDENTIFIER);
  skipCurrentKind(parser, LEFTPAREN);
  result->params = *createList();
  while (parser->current->kind != RIGHTPAREN) {
    appendList(&result->params, (void*)strdup(parser->current->string));
    skipCurrentKind(parser, IDENTIFIER);
  }
  skipCurrentKind(parser, RIGHTPAREN);
  skipCurrentKind(parser, LEFTBRACE);
  result->block = *parseBlock(parser);
  skipCurrentKind(parser, RIGHTBRACE);
  return result;
}

List* parseBlock(struct Parser* parser) {
  List* result = createList();
  while (parser->current->kind != RIGHTBRACE) {
    switch (parser->current->kind) {
      case VARIABLE: appendList(result, (void*)parseVar(parser)); break;
      case FOR: appendList(result, (void*)parseFor(parser)); break;
      case IF: appendList(result, (void*)parseIf(parser)); break;
      case RETURN: appendList(result, (void*)parseReturn(parser)); break;
      case BREAK: appendList(result, (void*)parseBreak(parser)); break;
      case CONTINUE: appendList(result, (void*)parseContinue(parser)); break;
      case LAST_TOKEN: errExit(EMSG_UNEXPECTED_EOT, parser->current->lineno); break;
      default: appendList(result, (void*)parseExprStatement(parser)); break;
    }
  }
  return result;
}

Statement* parseVar(struct Parser* parser) {
  Variable* var = (Variable*)calloc(1,sizeof(Variable));
  Statement* result = (Statement*)calloc(1,sizeof(Statement));
  skipCurrentKind(parser, VARIABLE);
  var->mut = skipCurrentIf(parser, MUT);
  var->name = parser->current->string;
  skipCurrentKind(parser, IDENTIFIER);
  if (skipCurrentIf(parser, COLON)) {
    var->type = parseType(parser->current->string);
    skipCurrentKind(parser, WJTYPE);
  } else { var->type = CORETYPE_ANY; }
  skipCurrentKind(parser, ASSIGNMENT);
  var->value = parseExpr(parser);
  if (var->value == NULL) {
    errExit(EMSG_NOINIT_EXPR_VARDEC, var->name);
    free(var);
    free(result);
  }
  skipCurrentKind(parser, SEMI);
  result->type = StmtVar;
  result->data.var = var;
  return result;
}

Statement* parseFor(struct Parser* parser) {
  struct For* _for = (struct For*)calloc(1,sizeof(struct For));
  Statement* result = (Statement*)calloc(1,sizeof(Statement));
  skipCurrentKind(parser, FOR);
  _for->variable = (struct Variable*)malloc(sizeof(struct Variable));
  _for->variable->name = parser->current->string;
  skipCurrentKind(parser, IDENTIFIER);
  skipCurrentKind(parser, ASSIGNMENT);
  _for->variable->value = parseExpr(parser);
  if (_for->variable->value == NULL) errExit(EMSG_NOINIT_EXPR_LOOP);
  skipCurrentKind(parser, SEMI);
  _for->condition = parseExpr(parser);
  if (_for->condition == NULL) errExit(EMSG_NOCONDI_LOOP);
  skipCurrentKind(parser, SEMI);
  _for->expression = parseExpr(parser);
  if (_for->expression == NULL) errExit(EMSG_NOEXPR_LOOP);
  skipCurrentKind(parser, LEFTBRACE);
  _for->block = *parseBlock(parser);
  skipCurrentKind(parser, RIGHTBRACE);
  result->data._for = _for;
  result->type = StmtFor;
  return result;
}

Statement* parseIf(struct Parser* parser) {
  If* _if = (If*)calloc(1,sizeof(If));
  Statement* result = (Statement*)calloc(1,sizeof(Statement));
  _if->conditions = (List*)calloc(1,sizeof(List));
  _if->blocks = (List*)calloc(1,sizeof(List));
  _if->elseblock = (List*)calloc(1,sizeof(List));
  skipCurrentKind(parser, IF);
  do {
    Expression* condition = parseExpr(parser);
    if (condition == NULL) errExit(EMSG_NOCONDI_IF);
    appendList(_if->conditions, (void*)condition);
    skipCurrentKind(parser, LEFTBRACE);
    appendList(_if->blocks, (void*)parseBlock(parser));
    skipCurrentKind(parser,RIGHTBRACE);
  } while (skipCurrentIf(parser, ELIF));
  if (skipCurrentIf(parser, ELSE)) {
    skipCurrentKind(parser, LEFTBRACE);
    _if->elseblock = parseBlock(parser);
    skipCurrentKind(parser, RIGHTBRACE);
  }
  result->type = StmtIf;
  result->data._if = _if;
  return result;
}

Statement* parseReturn(struct Parser* parser) {
  Return* ret = (struct Return*)calloc(1,sizeof(struct Return));
  Statement* result = (Statement*)calloc(1,sizeof(Statement));
  skipCurrentKind(parser, RETURN);
  ret->expression = parseExpr(parser);
  if (ret->expression == NULL) errExit(EMSG_NOEXPR_RET);
  skipCurrentKind(parser, SEMI);
  result->data.ret = ret;
  result->type = StmtReturn;
  return result;
}

Statement* parseBreak(struct Parser* parser) {
  Break* _break = (struct Break*)malloc(0);
  Statement* result = (Statement*)calloc(1,sizeof(Statement));
  skipCurrentKind(parser, BREAK);
  skipCurrentKind(parser, SEMI);
  result->data._break = _break;
  result->type = StmtBreak;
  return result;
}

Statement* parseContinue(struct Parser* parser) {
  Continue* _continue = (struct Continue*)malloc(0);
  Statement* result = (Statement*)calloc(1,sizeof(Statement));
  skipCurrentKind(parser, CONTINUE);
  skipCurrentKind(parser, SEMI);
  result->data._continue = _continue;
  result->type = StmtContinue;
  return result;
}

Statement* parseExprStatement(struct Parser* parser) {
  struct ExprStatement* expr = (struct ExprStatement*)calloc(1,sizeof(struct ExprStatement));
  Statement* result = (Statement*)calloc(1,sizeof(Statement));
  expr->expression = parseExpr(parser);
  skipCurrentKind(parser, SEMI);
  if (expr->expression == NULL) errExit(EMSG_NOEXPR);
  result->data.expr = expr;
  result->type = StmtExpr;
  return result;
}

Expression* parseExpr(struct Parser* parser) { return parseAssignment(parser); }

Expression* parseAssignment(struct Parser* parser) {
  Expression* result = parseOr(parser);
  if (parser->current->kind != ASSIGNMENT) return result;
  skipCurrentKind(parser, ASSIGNMENT);
  if (result->type == ExprGetVar) {
    struct SetVariable* sv = (struct SetVariable*)calloc(1,sizeof(struct SetVariable));
    sv->name = (parser->current-2)->string;
    sv->value = parseAssignment(parser);
    result->type = ExprSetVar;
    result->data.setvar = sv;
    return result;
  }
  if (result->type == ExprGetElem) {
    struct SetElement* se = (struct SetElement*)calloc(1,sizeof(struct SetElement));
    se->value = parseAssignment(parser);
    result->type = ExprSetElem;
    result->data.setelem = se;
    return result;
  }
  errExit(EMSG_CANNOT_ASSIGN);
  return NULL;
}

Expression* parseOr(struct Parser* parser) {
  Expression* result = parseAnd(parser);
  while (skipCurrentIf(parser, LOGICALOR)) {
    Or* or = (Or*)calloc(1,sizeof(Or));
    or->lhs = result;
    or->rhs = parseAnd(parser);
    result = (Expression*)calloc(1,sizeof(Expression));
    result->type = ExprOr;
    result->data.or = or;
  }
  return result;
}

Expression* parseAnd(struct Parser* parser) {
  Expression* result = parseRelational(parser);
  while (skipCurrentIf(parser, LOGICALAND)) {
    And* and = (And*)calloc(1,sizeof(And));
    and->lhs = result;
    and->rhs = parseRelational(parser);
    result = (Expression*)calloc(1,sizeof(Expression));
    result->type = ExprAnd;
    result->data.and = and;
  }
  return result;
}

Expression* parseRelational(struct Parser* parser) {
  Kind operators[] = { EQUAL, NOTEQUAL, LESSTHEN, GREATERTHAN, LESSOREQUAL, GREATEROREQUAL };
  Expression* result = parseArithmetic1(parser);
  for (int i = 0; i < 6; i++) {
    if (parser->current->kind == operators[i]) {
      struct Relational* temp = (struct Relational*)malloc(sizeof(struct Relational));
      temp->kind = parser->current->kind;
      skipCurrent(parser);
      temp->lhs = result;
      temp->rhs = parseArithmetic1(parser);
      Expression* re = (Expression*)malloc(sizeof(Expression));
      re->type = ExprRelational;
      re->data.relational = temp;      
      result = re;
    }
  }
  return result;
}

Expression* parseArithmetic1(struct Parser* parser) {
  Kind operators[] = { ADD, SUBTRACT };
  Expression* result = parseArithmetic2(parser);
  for (int i = 0; i < 2; i++) {
    if (parser->current->kind == operators[i]) {
      struct Arithmetic* temp = (struct Arithmetic*)malloc(sizeof(struct Arithmetic));
      temp->kind = parser->current->kind;
      skipCurrent(parser);
      temp->lhs = result;
      temp->rhs = parseArithmetic2(parser);
      Expression* ae = (Expression*)calloc(1,sizeof(Expression));
      ae->type = ExprArithmetic;
      ae->data.arithmetic = temp;
      result = ae;
    }
  }
  return result;
}

Expression* parseArithmetic2(struct Parser* parser) {
  Kind operators[] = { MULTIPLY, DIVIDE, MODULO };
  Expression* result = parseUnary(parser);
  for (int i = 0; i < 3; i++) {
    if (parser->current->kind == operators[i]) {
      struct Arithmetic* temp = (struct Arithmetic*)calloc(1,sizeof(struct Arithmetic));
      temp->kind = parser->current->kind;
      skipCurrent(parser);
      temp->lhs = result;
      temp->rhs = parseUnary(parser);
      Expression* ae = (Expression*)calloc(1,sizeof(Expression));
      ae->type = ExprArithmetic;
      ae->data.arithmetic = temp;
      result = ae;
    }
  }
  return result;
}

Expression* parseUnary(struct Parser* parser) {
  Kind operators[] = { ADD, SUBTRACT };
  for (int i = 0; i < 2; i++) {
    if (parser->current->kind == operators[i]) {
      struct Unary* temp = (struct Unary*)calloc(1,sizeof(struct Unary));
      temp->kind = parser->current->kind;
      skipCurrent(parser);
      temp->sub = parseUnary(parser);
      Expression* unaryExpr = (Expression*)calloc(1,sizeof(Expression));
      unaryExpr->type = ExprUnary;
      unaryExpr->data.unary = temp;
      return unaryExpr;
    }
  }
  return parseOperand(parser);
}

Expression* parseOperand(struct Parser* parser) {
  Expression* result = NULL;
  switch (parser->current->kind) {
    case BOOLEAN: result = parseBoolLit(parser); break;
    case NUMBER: result = parseNumLit(parser); break;
    case STRING: result = parseStringLit(parser); break;
    case LEFTBRAKET: result = parseListLit(parser); break;
    case LEFTBRACE: result = parseMapLit(parser); break;
    case IDENTIFIER: result = parseIdentifier(parser); break;
    case LEFTPAREN: result = parseInnerExpr(parser); break;
    default: errExit(EMSG_WRONG_EXPR, parser->current->lineno,parser->current->coloff); break;
  }
  return parsePostfix(parser, result);
}

Expression* parseBoolLit(struct Parser* parser) {
  Expression* result = (Expression*)calloc(1,sizeof(Expression));
  BooleanLiteral* temp = (BooleanLiteral*)calloc(1,sizeof(BooleanLiteral));
  temp->value = (strcmp(parser->current->string, "uglyguri") == 0) ? true : (strcmp(parser->current->string, "beautifulguri") == 0)? false: berrExit(EMSG_INVAL_BOOL, parser->current->lineno,parser->current->coloff);
  result->data.boolean = temp;
  result->type = ExprBool;
  skipCurrent(parser);
  return result;
}

Expression* parseNumLit(struct Parser* parser) {
  Expression* result = (Expression*)calloc(1,sizeof(Expression));
  NumberLiteral* temp = (NumberLiteral*)calloc(1,sizeof(NumberLiteral));
  char* endptr;
  // parse int and float. if it is float, parse to float
  if (strchr(parser->current->string, '.') != NULL) {
    temp->value.f32 = strtof(parser->current->string, &endptr);
    result->type = temp->type = CORETYPE_F32;
  } else {
    temp->value.i32 = strtol(parser->current->string, &endptr, 10);
    temp->type = CORETYPE_I32;
  }
  if (*endptr != '\0') errExit(EMSG_INVAL_NUMLIT, parser->current->lineno,parser->current->coloff);
  result->data.number = temp;
  result->type = ExprNum;
  skipCurrent(parser);
  return result;
}

Expression* parseStringLit(struct Parser* parser) {
  Expression* result = (Expression*)calloc(1,sizeof(Expression));
  StringLiteral* temp = (StringLiteral*)calloc(1,sizeof(StringLiteral));
  temp->value = strdup(parser->current->string);
  result->data.string = temp;
  result->type = ExprString;
  skipCurrent(parser);
  return result;
}

Expression* parseListLit(struct Parser* parser) {
  Expression* result = (Expression*)calloc(1,sizeof(Expression));
  ArrayLiteral* temp = (ArrayLiteral*)calloc(1,sizeof(ArrayLiteral));
  temp->values = *createList();
  skipCurrentKind(parser, LEFTBRAKET);
  if (parser->current->kind != RIGHTBRAKET) {
    do {
      appendList(&temp->values, (void*)parseExpr(parser));
    } while (skipCurrentIf(parser, COMMA));
  }
  skipCurrentKind(parser, RIGHTBRAKET);
  result->data.array = temp;
  result->type = ExprArray;
  return result;
}

Expression* parseMapLit(struct Parser* parser) {
  Expression* result = (Expression*)calloc(1,sizeof(Expression));
  MapLiteral* temp = (MapLiteral*)calloc(1,sizeof(MapLiteral));
  temp->values = *createMap();
  skipCurrentKind(parser, LEFTBRACE);
  if (parser->current->kind != RIGHTBRACE) {
    do {
      char *name = parser->current->string;
      skipCurrentKind(parser, STRING);
      skipCurrentKind(parser, COLON);
      Expression* value = parseExpr(parser);
      pushMap(&temp->values, name, (void*)value);
    } while (skipCurrentIf(parser, COMMA));
  }
  skipCurrentKind(parser, RIGHTBRACE);
  result->data.map = temp;
  result->type = ExprMap;
  return result;
}

Expression* parseIdentifier(struct Parser* parser) {
  Expression* result = (Expression*)calloc(1,sizeof(Expression));
  GetVariable* temp = (GetVariable*)calloc(1,sizeof(GetVariable));
  temp->name = strdup(parser->current->string);
  result->data.getvar = temp;
  result->type = ExprGetVar;
  skipCurrentKind(parser, IDENTIFIER);
  return result;
}

Expression* parseInnerExpr(struct Parser* parser) {
  skipCurrentKind(parser, LEFTPAREN);
  Expression* result = parseExpr(parser);
  skipCurrentKind(parser, RIGHTPAREN);
  return result;
}

Expression* parsePostfix(struct Parser* parser, Expression* sub) {
  while (true) {
    switch (parser->current->kind) {
      case LEFTPAREN:  sub = parseCall(parser, sub);    break;
      case LEFTBRAKET: sub = parseElement(parser, sub); break;
      default: return sub;
    }
    // if (parser->current->kind != LEFTPAREN && parser->current->kind != LEFTBRAKET) break; 
  }
}

Expression* parseCall(struct Parser* parser, Expression* sub) {
  struct Call* temp = (struct Call*)calloc(1,sizeof(struct Call));
  temp->arguments = *createList();
  skipCurrentKind(parser, LEFTPAREN);
  temp->sub = sub;
  if (parser->current->kind != RIGHTPAREN) {
    do {
      appendList(&temp->arguments, (void*)parseExpr(parser));
    } while (skipCurrentIf(parser, COMMA));
  }
  skipCurrentKind(parser, RIGHTPAREN);
  Expression* result = (Expression*)calloc(1,sizeof(Expression));
  result->data.call = temp;
  result->type = ExprCall;
  return result;
}

Expression* parseElement(struct Parser* parser, Expression* sub) {
  struct GetElement* temp = (struct GetElement*)calloc(1,sizeof(struct GetElement));
  temp->sub = sub;
  skipCurrentKind(parser, LEFTBRAKET);
  temp->index = parseExpr(parser);
  skipCurrentKind(parser, RIGHTBRAKET);
  Expression* result = (Expression*)calloc(1,sizeof(Expression));
  result->data.getelem = temp;
  result->type = ExprGetElem;
  return result;
}

void skipCurrent(struct Parser* parser) { parser->current++; }

void skipCurrentKind(struct Parser* parser, Kind kind) {
  if (parser->current->kind != kind)
    errExit(EMSG_UNEXPECTED_TOKEN, kindToString(kind), kindToString(parser->current->kind), parser->current->lineno, parser->current->coloff);
  parser->current++;
}

bool skipCurrentIf(struct Parser* parser, Kind kind) {
  if (parser->current->kind == kind) {
    parser->current++;
    return true;
  }
  return false;
}

void parser_init(struct Parser* parser, Token* current) { parser->current = current; }