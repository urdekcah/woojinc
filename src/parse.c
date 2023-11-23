#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "woojin.h"
#include "expr.h"
#include "statement.h"
#include "scope.h"
#include "ast.h"
#include "mem.h"
#include "error.h"
#include "token.h"
#include "tokenizer.h"
#include "parse.h"
#include "node.h"
#include "list.h"
#include "type.h"

char* getFileName();

extern type_t _const_error_type;

Array_w__ast__Node w__parser__Parser__parse(Tokenizer* tokenizer, CompilerOption* co) {
  List result = __list_new();
  w__parser__Parser* self = w__parser__Parser__new(w__table__Table_new(), tokenizer->tokens, co);
  while (self->current->kind != TOKEN_EOT) {
    switch (self->current->kind) {
      case TOKEN_KW_IMPORT: {
        skipCurrent(self);
        do {
          char* import = self->current->token;
          skipCurrentIf(self, TOKEN_IDENT);
          bool includes = __list_list_string_includes(&self->table->imports, import);
          if (!includes) __list_append_list(&self->table->imports, (void*)import);
          else __list_append_list(&self->errors, (void*)w__error__Error__new(L"Помилка імпорту", FormatW(E(E_ALREADY_IMPORTED_MODULE,D(co)), import), getFileName(), *w__TokenPos__from_Token(self->tokens.items[self->tidx-1])));
        } while (skipCurrentIf(self, TOKEN_COMMA));
        skipCurrentKind(self, TOKEN_SEMI);
        break;
      }
      case TOKEN_KW_TYPE: {
        w__ast__TypeDecl* type = parseTypeDecl(self);
        w__ast__Node node = (w__ast__Node){.kind = NODE_STATEMENT, ._w__ast__Statement = (w__ast__Statement){.kind = STATEMENT_TYPE_DECL, ._w__ast__TypeDecl = *type} };
        __list_append_list(&result, (void*)&node);
        break;
      }
      case TOKEN_KW_FN: {
        w__ast__FnDecl* fn = parseFunction(self);
        w__ast__Node node = (w__ast__Node){.kind = NODE_STATEMENT, ._w__ast__Statement = (w__ast__Statement){.kind = STATEMENT_FN_DECL, ._w__ast__FnDecl = *fn} };
        __list_append_list(&result, memdup(&node, sizeof(w__ast__Node)));
        break;
      }
      default:{
        self->current = (Token*)self->tokens.items[++self->tidx];
        // Warning* err = w__error__Warning__new(L"Невідомий токен", "Схоже, що ви ввели невідомий токен. Перевірте, чи ви ввели правильний токен.", getFileName(), *w__TokenPos__from_Token(self->current));
        // w__errors__PrintWarningMessage(err);
        break;
      }
    }
  }
  for (int i = 0; i < self->errors.size; i++) w__errors__PrintErrorMessage(((Error*)self->errors.items[i]));
  for (int i = 0; i < self->warnings.size; i++) w__errors__PrintWarningMessage(((Warning*)self->warnings.items[i]));
  for (int i = 0; i < self->notices.size; i++) w__errors__PrintNoticeMessage(((Notice*)self->notices.items[i]));
  return result;
}

w__ast__TypeDecl* parseTypeDecl(w__parser__Parser* parser) {
  w__ast__TypeDecl* result = (w__ast__TypeDecl*)calloc(1,sizeof(w__ast__TypeDecl));
  w__ast__AliasTypeDecl* alias = (w__ast__AliasTypeDecl*)calloc(1,sizeof(w__ast__AliasTypeDecl));
  result->_w__ast__AliasTypeDecl = *alias;
  skipCurrentKind(parser, TOKEN_KW_TYPE);
  bool pub = skipCurrentIf(parser, TOKEN_KW_PUB);
  result->name = parser->current->token;
  skipCurrentKind(parser, TOKEN_IDENT);
  skipCurrentKind(parser, TOKEN_EQ);
  Array_w__ast__Token tokens = __list_new();
  while (parser->current->kind != TOKEN_SEMI) {
    TokenKind kind = parser->current->kind;
    if (kind != TOKEN_IDENT && kind != TOKEN_LBRACE && kind != TOKEN_RBRACE && kind != TOKEN_LBRACKET && kind != TOKEN_RBRACKET) {
      __list_append_list(&parser->errors, (void*)w__error__Error__new(L"Невідомий токен", FormatW(EMSG_UNEXPECTED_TOKEN, w__Token__kind_into__string(TOKEN_SEMI), w__Token__kind_into__string(parser->current->kind), parser->current->line, parser->current->col), getFileName(), *w__TokenPos__from_Token(parser->current)));
      return NULL;
    }
    __list_append_list(&tokens, (void*)parser->current);
    skipCurrent(parser);
  }
  if (tokens.size < 1) {
    __list_append_list(&parser->errors, (void*)w__error__Error__new(L"Невідомий токен", FormatW(EMSG_UNEXPECTED_TOKEN, w__Token__kind_into__string(TOKEN_SEMI), w__Token__kind_into__string(parser->current->kind), parser->current->line, parser->current->col), getFileName(), *w__TokenPos__from_Token(parser->current)));
    return NULL;
  }
  char* name = "";
  for (int i = 0; i < tokens.size; i++) {
    char* r = (char*)calloc(strlen(name)+strlen(((Token*)tokens.items[i])->token)+1,sizeof(char));
    if (r == NULL) ErrExit(E_MEMALLOC);
    strcpy(r, name);
    strcat(r, ((Token*)tokens.items[i])->token);
    name = r;
  }
  printf("%s = name: %s\n", result->name, name);
}

w__ast__Param* parseParam(w__parser__Parser* parser) {
  w__ast__Param* result = (w__ast__Param*)calloc(1,sizeof(w__ast__Param));
  result->name = parser->current->token;
  skipCurrentKind(parser, TOKEN_IDENT);
  skipCurrentKind(parser, TOKEN_COLON);
  result->mut = skipCurrentIf(parser, TOKEN_KW_MUT);
  type_t t = parseAnyType(parser);
  result->type = t;
  return result;
}

// v__ast__Table_register_fn
// void w__ast__Table__register_fn(Table* t, w__ast__fn)

w__ast__Return w__parser__Parser_return_stmt(w__parser__Parser *p) {
  w__ast__Return* result = calloc(1,sizeof(w__ast__Return));
  result = (w__ast__Return*)memdup(&(w__ast__Return) { .expr = w__parser__Parser_expr(p, 0), .type = 0 }, sizeof(w__ast__Return));
  return *result;
}

Array_w__ast__Statement w__parser__Parser_parse_block(w__parser__Parser *_a1) {
  printf("w__parser__Parser_parse_block\n");
  w__parser__Parser_open_scope(_a1);
	Array_w__ast__Statement _r1 = w__parser__parse_block_no_scope(_a1, false);
	w__parser__Parser_close_scope(_a1);
  return _r1;
}

w__ast__Expr w__parser__Parser__parse_numlit(w__parser__Parser* p) {
  bool neg = skipCurrentIf(p, TOKEN_MINUS);
  char* lit = p->current->token;
  skipCurrentKind(p, TOKEN_NUMBER_LITERAL);
  char* flit = (neg)?string_plus("-", lit):lit;
  w__ast__Expr node = _const_w__ast__empty_expr;
  if (strstr(lit, ".") != NULL) {
    node = (w__ast__Expr){.kind = EXPR_FLOAT_LITERAL, ._w__ast__FloatLiteral = (w__ast__FloatLiteral){.val=flit, .typ = (FloatType)FNOTYET} };
  } else {
    node = (w__ast__Expr){.kind = EXPR_INTEGER_LITERAL, ._w__ast__IntegerLiteral = (w__ast__IntegerLiteral){.val=lit, .typ = (IntegerType)INOTYET} };
  }
  return node;
}

ScopeObject* w__ast__Scope_find(Scope* s, char* name) {
  for (Scope* sc = s; true; sc = sc->parent) {
    if (sc == NULL) break;
    if (__map__get(sc->objects, name) != NULL)
      return (ScopeObject*)__map__get(sc->objects, name);
  }
  return NULL;
}

w__ast__Var* w__ast__Scope_find_var(Scope* s, char* name) {
  ScopeObject* obj = w__ast__Scope_find(s, name);
  printf("obj: %p\n", obj);
  if (obj != NULL)
    if (obj->kind == ScopeVarObject) return &obj->_w__ast__Var;
  return NULL;
}

bool w__ast__Scope_known_var(Scope* s, char* name) {
  w__ast__Var* _r1 = w__ast__Scope_find_var(s, name);
  return (_r1 != NULL); 
}

w__ast__IfExpr w__parser__Parser_if_expr(w__parser__Parser* p) {
  w__ast__IfExpr* result = (w__ast__IfExpr*)calloc(1,sizeof(w__ast__IfExpr));
  Array_w__ast__IfBranch branches = __list_new();
  w__ast__Expr cond = _const_w__ast__empty_expr;
  hello:
    if (!((p->current->kind == TOKEN_KW_IF || p->current->kind == TOKEN_KW_ELSE))) goto world;
    if (p->current->kind == TOKEN_KW_ELSE) {
      skipCurrentKind(p, TOKEN_KW_ELSE);
      result->has_else = true;
      if (p->current->kind == TOKEN_LBRACE) {
        w__parser__Parser_open_scope(p);
        __list_push_back(&branches, (void*)&((w__ast__IfBranch){.cond = {0}, .stmts = w__parser__parse_block_no_scope(p, false) }));
        w__parser__Parser_close_scope(p);
      }
    }
    skipCurrentKind(p, TOKEN_KW_IF);
    p->inside_if = true;
    cond = w__parser__Parser_expr(p, 0);
    if (p->current->kind == TOKEN_LBRACE) {
      w__parser__Parser_open_scope(p);
      __list_push_back(&branches, (void*)&((w__ast__IfBranch){.cond = cond, .stmts = w__parser__parse_block_no_scope(p, false) }));
      w__parser__Parser_close_scope(p);
    }
    printf("p->current->kind: %d\n", p->current->kind);
    p->inside_if = false;
    goto hello;
  world:
  result->cond = (w__ast__Expr*)memdup(&cond, sizeof(w__ast__Expr));
  result->branches = branches;
  return *result;
}

Array_w__ast__CallArg* w__parser__Parser_call_args(w__parser__Parser* p) {
  Array_w__ast__CallArg args = __list_new();
  while(true) {
    if (!(p->current->kind != TOKEN_RPAREN)) break;
    if (p->current->kind == TOKEN_EOT) {
      __list_append_list(&p->errors, (void*)w__error__Error__new(L"Невідомий токен", FormatW(EP(EMSG_UNEXPECTED_TOKEN,p), w__Token__kind_into__string(TOKEN_RPAREN), w__Token__kind_into__string(p->current->kind), p->current->line, p->current->col), getFileName(), *w__TokenPos__from_Token(p->current)));
      return NULL;
    }
    bool mut = skipCurrentIf(p, TOKEN_KW_MUT);
    w__ast__Expr expr = w__parser__Parser_expr(p, 0);
    __list_append_list(&args, (void*)&((w__ast__CallArg){.expr = expr, .mut = mut}));
    if (p->current->kind != TOKEN_RPAREN) skipCurrentKind(p, TOKEN_COMMA);
  }
  return (Array_w__ast__CallArg*)memdup(&args, sizeof(Array_w__ast__CallArg));
}

w__ast__CallExpr* w__parser__Parser_call_expr(w__parser__Parser* p, /*v__ast__Language language,*/ char* mod) {
  skipCurrentKind(p, TOKEN_LPAREN);
  Array_w__ast__Expr* args = w__parser__Parser_call_args(p);
  if (args == NULL) return NULL;
  skipCurrentKind(p, TOKEN_RPAREN);
  return &(w__ast__CallExpr){
    .name = mod,
    .args = *args,
    .method = false,
    .field = false,
    .noreturn = false,
    .reciever_type = 0,
    .args = *args,
    .args_typeinfo = NULL,
  };
}

bool w__token__Kind__is_assign(TokenKind _a1) {
  TokenKind _r1[] = { TOKEN_EQ };
  for (int i = 0; i < 1; i++) 
    if (_a1 == _r1[i]) return true;
  return false;
}

bool w__parser__Parser_mark_var_as_used(w__parser__Parser* p, char* varname) {
  ScopeObject* obj = w__ast__Scope_find(p->scope, varname);
  if (obj == NULL) return false;
  if (obj->kind == ScopeVarObject) {
    obj->_w__ast__Var.used = true;
    return true;
  }
  return false;
}

w__ast__Expr parseAssignment(w__parser__Parser* p) {
  Token prev = Prev(p);
  if (prev.kind != TOKEN_IDENT) __list_append_list(&p->errors, (void*)w__error__Error__new(L"Невідомий токен", FormatW(EP(EMSG_UNEXPECTED_TOKEN,p), w__Token__kind_into__string(TOKEN_IDENT), w__Token__kind_into__string(prev.kind), prev.line, prev.col), getFileName(), *w__TokenPos__from_Token(&prev)));
  char* varname = prev.token;
  skipCurrentKind(p, TOKEN_EQ);
  w__ast__Expr node = _const_w__ast__empty_expr;
  node = w__parser__Parser_expr(p, 0);
  w__ast__Var* var = w__ast__Scope_find_var(p->scope, varname);
  printf("%s!!! %s\n", (var==NULL)?"NULL":"NOT NULL",varname);
  if (var == NULL) {
    __list_append_list(&p->errors, (void*)w__error__Error__new(L"Невідомий токен", FormatW(EP(EMESG_UNDECLARED_VAR,p), varname, prev.line, prev.col), getFileName(), *w__TokenPos__from_Token(&prev)));
  }
  skipCurrentKind(p, TOKEN_SEMI);
}

bool w__ast__is_IntegerType(type_t _a1) {
  type_t _r1[] = {
    w__new_type(_const_i8_type_idx),
    w__new_type(_const_i16_type_idx),
    w__new_type(_const_i32_type_idx),
    w__new_type(_const_i64_type_idx),
    w__new_type(_const_u8_type_idx),
    w__new_type(_const_u16_type_idx),
    w__new_type(_const_u32_type_idx),
    w__new_type(_const_u64_type_idx),
  };
  for (int i = 0; i < 8; i++) 
    if (_a1 == _r1[i]) return true;
  return false;
}

bool w__ast__is_FloatType(type_t _a1) {
  type_t _r1[] = {
    w__new_type(_const_f32_type_idx),
    w__new_type(_const_f64_type_idx),
  };
  for (int i = 0; i < 2; i++) 
    if (_a1 == _r1[i]) return true;
  return false;
}

IntegerType w__ast__toIntegerType(type_t _a1) {
  if (_a1 == w__new_type(_const_i8_type_idx)) return (IntegerType)I8;
  if (_a1 == w__new_type(_const_i16_type_idx)) return (IntegerType)I16;
  if (_a1 == w__new_type(_const_i32_type_idx)) return (IntegerType)I32;
  if (_a1 == w__new_type(_const_i64_type_idx)) return (IntegerType)I64;
  if (_a1 == w__new_type(_const_u8_type_idx)) return (IntegerType)U8;
  if (_a1 == w__new_type(_const_u16_type_idx)) return (IntegerType)U16;
  if (_a1 == w__new_type(_const_u32_type_idx)) return (IntegerType)U32;
  if (_a1 == w__new_type(_const_u64_type_idx)) return (IntegerType)U64;
  return (IntegerType)INOTYET;
}

FloatType w__ast__toFloatType(type_t _a1) {
  if (_a1 == w__new_type(_const_f32_type_idx)) return (FloatType)F32;
  if (_a1 == w__new_type(_const_f64_type_idx)) return (FloatType)F64;
  return (FloatType)FNOTYET;
}

w__ast__Expr parseVarDecl(w__parser__Parser* p) {
  w__ast__Expr node = _const_w__ast__empty_expr;
  skipCurrentKind(p, TOKEN_KW_LET);
  bool mut = skipCurrentIf(p, TOKEN_KW_MUT);
  char* name = p->current->token;
  skipCurrentKind(p, TOKEN_IDENT);
  skipCurrentKind(p, TOKEN_COLON);
  type_t type = parseAnyType(p);
  skipCurrentKind(p, TOKEN_EQ);
  w__ast__Expr expr = w__parser__Parser_expr(p, 0);
  skipCurrentKind(p, TOKEN_SEMI);
  w__ast__Var* var = w__ast__Scope_find_var(p->scope, name);
  printf("var: %p\n", var);
  if (var != NULL) {
    Token t = PrevN(p,6);
    __list_append_list(&p->errors, (void*)w__error__Error__new(L"Невідомий токен", FormatW(EP(EMSG_REDECLARED_VAR,p), PrevN(p,6).token, p->current->line, p->current->col), getFileName(), *w__TokenPos__from_Token(&t)));
    return node;
  }
  if (expr.kind == EXPR_INTEGER_LITERAL) {
    if (w__ast__is_IntegerType(type)) {
      IntegerType itype = w__ast__toIntegerType(type);
      IntegerType etype = w__ast__toIntegerType(expr._w__ast__IntegerLiteral.typ);
      if (etype != INOTYET && itype != etype)
        __list_append_list(&p->errors, (void*)w__error__Error__new(L"Невідомий токен", FormatW(EP(EMSG_UNEXPECTED_TOKEN,p), w__Token__kind_into__string(TOKEN_IDENT), w__Token__kind_into__string(p->current->kind), p->current->line, p->current->col), getFileName(), *w__TokenPos__from_Token(p->current)));
      expr._w__ast__IntegerLiteral.typ = itype;
    }
  } else if (expr.kind == EXPR_FLOAT_LITERAL) {
    if (w__ast__is_FloatType(type)) {
      FloatType itype = w__ast__toFloatType(type);
      FloatType etype = w__ast__toFloatType(expr._w__ast__FloatLiteral.typ);
      if (etype != FNOTYET && itype != etype)
        __list_append_list(&p->errors, (void*)w__error__Error__new(L"Невідомий токен", FormatW(EP(EMSG_UNEXPECTED_TOKEN,p), w__Token__kind_into__string(TOKEN_IDENT), w__Token__kind_into__string(p->current->kind), p->current->line, p->current->col), getFileName(), *w__TokenPos__from_Token(p->current)));
      expr._w__ast__FloatLiteral.typ = itype;
    }
  }
  w__ast__Var* _t1 = (w__ast__Var*)calloc(1,sizeof(w__ast__Var));
  _t1->name = name;
  _t1->type = type;
  _t1->expr = expr;
  _t1->mut = mut;
  _t1->used = false;
  _t1->changed = false;
  _t1->tmp = false;
  ScopeObject* _t2 = (ScopeObject*)calloc(1,sizeof(ScopeObject));
  _t2->kind = ScopeVarObject;
  _t2->_w__ast__Var = *_t1;
  _t2->name = strdup(name);
  w__ast__Scope_register(p->scope, _t2);
  return node;
}

w__ast__Expr w__parser__Parser_name_expr(w__parser__Parser* p) {
  TokenKind prev_tok_kind = Prev(p).kind;
  w__ast__Expr node = _const_w__ast__empty_expr;
  bool known_var = (w__token__Kind__is_assign(Peek(p).kind) ? (w__ast__Scope_known_var(p->scope, p->current->token)) : (w__parser__Parser_mark_var_as_used(p, p->current->token)));
  printf("known_var: %d\n", known_var);
  if (prev_tok_kind == TOKEN_IDENT && p->current->kind == TOKEN_EQ) {
    return parseAssignment(p);
  } else if (prev_tok_kind == TOKEN_IDENT && p->current->kind == TOKEN_LPAREN) {
    return (w__ast__Expr){
      ._w__ast__CallExpr=(*w__parser__Parser_call_expr(p, (Prev(p).token))),
      .kind = EXPR_CALL,
    };
  }
}

w__ast__Expr w__parser__Parser_check_expr(w__parser__Parser* p) {
  w__ast__Expr node = _const_w__ast__empty_expr;
  switch (p->current->kind) {
    case TOKEN_IDENT: {
      skipCurrent(p);
      w__ast__Expr _t1 = w__parser__Parser_name_expr(p);
      node = _t1;
      break;
    }
    case TOKEN_KW_LET: {
      node = parseVarDecl(p);
      break;
    }
    case TOKEN_MINUS: {
      if (Peek(p).kind == TOKEN_NUMBER_LITERAL) node = w__parser__Parser__parse_numlit(p);
      // else node = v__parser__Parser_prefix_expr(p);
      break;
    }
    case TOKEN_STRING_LITERAL: {
      node = (w__ast__Expr){.kind = EXPR_STRING_LITERAL, ._w__ast__StringLiteral = (w__ast__StringLiteral){.val=p->current->token}};
      skipCurrent(p);
      break;
    }
    case TOKEN_TRUE_LITERAL: case TOKEN_FALSE_LITERAL: {
      node = (w__ast__Expr){.kind = EXPR_BOOL_LITERAL, ._w__ast__BoolLiteral = (w__ast__BoolLiteral){.val=(p->current->kind == TOKEN_TRUE_LITERAL)}};
      skipCurrent(p);
      break;
    }
    case TOKEN_NUMBER_LITERAL: {
      w__ast__Expr _t1 = w__parser__Parser__parse_numlit(p);
      node = _t1;
      break;
    }
    case TOKEN_KW_IF: {
      node = (w__ast__Expr){ .kind = EXPR_IF, ._w__ast__IfExpr = w__parser__Parser_if_expr(p) };
      break;
    }
    default: {
      __list_append_list(&p->errors, (void*)w__error__Error__new(L"Невідомий токен", FormatW(EP(EMSG_UNEXPECTED_TOKEN,p), w__Token__kind_into__string(TOKEN_SEMI), w__Token__kind_into__string(p->current->kind), p->current->line, p->current->col), getFileName(), *w__TokenPos__from_Token(p->current)));
      break;
    }
  }
  return node;
}

w__ast__Expr w__parser__Parser_expr(w__parser__Parser* p, int precedence) {
  return w__parser__Parser_check_expr(p);
}

w__ast__Statement parseStatement(w__parser__Parser *p, bool top_level) {
  printf("parseStatement\n");
  printf("%d", p->current->kind);
  switch (p->current->kind) {
    case TOKEN_LBRACE: {
      Array_w__ast__Statement _r1 = w__parser__Parser_parse_block(p);
      w__ast__Statement _r2 = (w__ast__Statement){.kind = STATEMENT_BLOCK};
      _r2._w__ast__Block = (w__ast__Block){.stmts=_r1};
      return _r2;
    }
    default: {
      w__ast__Expr _t33 = w__parser__Parser_expr(p, 0);
      w__ast__Statement _r1 = (w__ast__Statement){.kind = STATEMENT_EXPR, ._w__ast__ExprStatement = (w__ast__ExprStatement){.expr=_t33}};
      return _r1;
    }
  }
}

Array_w__ast__Statement w__parser__parse_block_no_scope(w__parser__Parser *p, bool top_level) {
  Array_w__ast__Statement* result = __list_mal();
  skipCurrentKind(p, TOKEN_LBRACE);
  while (p->current->kind != TOKEN_RBRACE) {
    w__ast__Statement s = parseStatement(p, top_level);
    __list_append_list(result, (void*)&s);
    printf("%d\n", p->current->kind);
  }
  skipCurrentKind(p, TOKEN_RBRACE);
  return *result;
}

w__ast__FnDecl* parseFunction(w__parser__Parser* parser) {
  printf("parseFunction\n");
  w__ast__FnDecl* result = (w__ast__FnDecl*)calloc(1,sizeof(w__ast__FnDecl));
  w__ast__FnDecl* bef = parser->table->curfn;
  bool was_inside_fn = parser->inside_fn;
  parser->table->curfn = result;
  parser->inside_fn = true;
  skipCurrentKind(parser, TOKEN_KW_FN);
  result->name = parser->current->token;
  skipCurrentKind(parser, TOKEN_IDENT);
  skipCurrentKind(parser, TOKEN_LPAREN);
  result->params = __list_mal();
  result->noparam = skipCurrentIf(parser, TOKEN_RPAREN);
  if (result->noparam) goto l0;
  do {
    __list_append_list(result->params, (void*)parseParam(parser));
  } while (skipCurrentIf(parser, TOKEN_COMMA));
  skipCurrentKind(parser, TOKEN_RPAREN);
  l0:
  result->has_return = skipCurrentIf(parser, TOKEN_ARROW);
  if (result->has_return) result->return_type = parseAnyType(parser);
  result->noreturn = (!result->has_return);
  result->stmts = w__parser__parse_block_no_scope(parser, true);
  parser->table->curfn = bef;
  parser->inside_fn = was_inside_fn;
  return result;
}

w__ast__Expr parseOr(w__parser__Parser* p) {

}

void skipCurrent(w__parser__Parser* parser) { parser->current = (Token*)parser->tokens.items[++parser->tidx]; }

void skipCurrentKind(w__parser__Parser* parser, TokenKind kind) {
  if (parser->current->kind != kind) {
    __list_append_list(&parser->errors, (void*)w__error__Error__new(L"Невідомий токен", FormatW(EP(EMSG_UNEXPECTED_TOKEN,parser), w__Token__kind_into__string(kind), w__Token__kind_into__string(parser->current->kind), parser->current->line, parser->current->col), getFileName(), *w__TokenPos__from_Token(parser->current)));
    return;
  }
  parser->current = (Token*)parser->tokens.items[++parser->tidx]; 
}

Token w__parser__Parser__prev(w__parser__Parser* parser) { return w__parser__Parser__prev_n(parser, 1); }
Token w__parser__Parser__prev_n(w__parser__Parser* parser, int n) { return *(Token*)parser->tokens.items[parser->tidx-n]; }
Token w__parser__Parser_peek(w__parser__Parser* parser) { return w__parser__Parser_peek_n(parser, 1); }
Token w__parser__Parser_peek_n(w__parser__Parser* parser, int n) { return *(Token*)parser->tokens.items[parser->tidx+n]; }

bool skipCurrentIf(w__parser__Parser* parser, TokenKind kind) {
  if (parser->current->kind == kind) {
    parser->current = (Token*)parser->tokens.items[++parser->tidx]; 
    return true;
  }
  return false;
}
