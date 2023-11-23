#ifndef __WOOJINC_PARSE_H__
#define __WOOJINC_PARSE_H__
#include "woojin.h"
#include "table.h"
#include "token.h"
#include "language.h"
#include "scope.h"
#include "complier.h"

struct w__parser__Parser {
  Table* table;
  List tokens;
  Token* current;
  int tidx;
  Scope* scope;
	ComplierOption* co;
  bool inside_vlib_file;
	bool inside_test_file;
	bool inside_if;
	bool inside_comptime_if;
	bool inside_if_expr;
	bool inside_if_cond;
	bool inside_ct_if_expr;
	bool inside_or_expr;
	bool inside_for;
	bool inside_fn;
	bool inside_fn_return;
	bool inside_call_args;
	bool inside_unsafe_fn;
	bool inside_str_interp;
	bool inside_array_lit;
	bool inside_in_array;
	bool inside_infix;
	bool inside_match;
	bool inside_select;
	bool inside_match_case;
	bool inside_match_body;
	bool inside_unsafe;
	bool inside_sum_type;
	bool inside_asm_template;
	bool inside_asm;
	bool inside_defer;
	bool inside_generic_params;
	bool inside_receiver_param;
	bool inside_struct_field_decl;
	bool inside_struct_attr_decl;
	bool inside_map_init;
	bool inside_orm;
	bool inside_chan_decl;
	bool inside_attr_decl;
  Array_w__error__Error errors;
  Array_w__error__Warning warnings;
  Array_w__error__Notice notices;
};

w__parser__Parser* w__parser__Parser__new(Table* table, List token, ComplierOption* co);
void w__parser__Parser__init(w__parser__Parser* self, Table* table, List tokens, ComplierOption* co);
#define ParserInitDefault(self) w__parser__Parser__init(self, w__table__Table_new(), __list_new(), w__complier__option_default());
void w__parser__Parser__InitDefault(w__parser__Parser* out);
Array_w__ast__Node w__parser__Parser__parse(Tokenizer* tokenizer, ComplierOption* co);
void w__parser__Parser__free(w__parser__Parser* self);
type_t w__parser__Parser__parseAnyType(w__parser__Parser* parser, Language lang);
Array_w__ast__Statement w__parser__Parser_parse_block(w__parser__Parser *_r1);
Array_w__ast__Statement w__parser__parse_block_no_scope(w__parser__Parser *p, bool top_level);

w__ast__TypeDecl* parseTypeDecl(w__parser__Parser* parser);
w__ast__FnDecl* parseFunction(w__parser__Parser* parser);
w__ast__Expr w__parser__Parser_expr(w__parser__Parser* p, int precedence);

void skipCurrent(w__parser__Parser* parser);
void skipCurrentKind(w__parser__Parser* parser, TokenKind kind);
Token w__parser__Parser__prev(w__parser__Parser* parser);
Token w__parser__Parser__prev_n(w__parser__Parser* parser, int n);
Token w__parser__Parser_peek(w__parser__Parser* parser);
Token w__parser__Parser_peek_n(w__parser__Parser* parser, int n);
bool skipCurrentIf(w__parser__Parser* parser, TokenKind kind);

#define PrevN(parser, n) w__parser__Parser__prev_n(parser, n)
#define Prev(parser) PrevN(parser, 1)
#define PeekN(parser, n) w__parser__Parser_peek_n(parser, n)
#define Peek(parser) PeekN(parser, 1)
#define parseAnyType(p) w__parser__Parser__parseAnyType(p, (Language)0)
#endif