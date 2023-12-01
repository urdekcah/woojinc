#ifndef __WOOJINC_PARSER_H__
#define __WOOJINC_PARSER_H__
#include "token.h"
#include "node.h"
struct Parser { Token* current; };
struct Parser* parser_new(Token* current);
void parser_init(struct Parser* parser, Token* current);
Program* parse(Token* tokens);
Function* parseFunc(struct Parser* parser);
// return: List<Statement*>
List* parseBlock(struct Parser* parser);
Statement* parseVar(struct Parser* parser);
Statement* parseFor(struct Parser* parser);
Statement* parseIf(struct Parser* parser);
Statement* parseReturn(struct Parser* parser);
Statement* parseBreak(struct Parser* parser);
Statement* parseContinue(struct Parser* parser);
Statement* parseExprStatement(struct Parser* parser);
Expression* parseExpr(struct Parser* parser);
Expression*parseAssignment(struct Parser* parser);
Expression* parseOr(struct Parser* parser);
Expression* parseAnd(struct Parser* parser);
Expression* parseRelational(struct Parser* parser);
Expression* parseArithmetic1(struct Parser* parser);
Expression* parseArithmetic2(struct Parser* parser);
Expression* parseUnary(struct Parser* parser);
Expression* parseOperand(struct Parser* parser);
Expression* parseBoolLit(struct Parser* parser);
Expression* parseNumLit(struct Parser* parser);
Expression* parseStringLit(struct Parser* parser);
Expression* parseListLit(struct Parser* parser);
Expression* parseMapLit(struct Parser* parser);
Expression* parseIdentifier(struct Parser* parser);
Expression* parseInnerExpr(struct Parser* parser);
Expression* parsePostfix(struct Parser* parser, Expression* sub);
Expression* parseCall(struct Parser* parser, Expression* sub);
Expression* parseElement(struct Parser* parser, Expression* sub);
void skipCurrent(struct Parser* parser);
void skipCurrentKind(struct Parser* parser, Kind kind);
bool skipCurrentIf(struct Parser* parser, Kind kind);
#endif