#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "token.h"
#include "error.h"

#define STATE_INIT 0
#define STATE_NORMAL 1

static TypeStrPair stringToType[] = {
  {"bool", CORETYPE_BOOL},
  {"i8", CORETYPE_I8},
  {"i16", CORETYPE_I16},
  {"i32", CORETYPE_I32},
  {"i64", CORETYPE_I64},
  {"i128", CORETYPE_I128},
  {"u8", CORETYPE_U8},
  {"u16", CORETYPE_U16},
  {"u32", CORETYPE_U32},
  {"u64", CORETYPE_U64},
  {"u128", CORETYPE_U128},
  {"f32", CORETYPE_F32},
  {"f64", CORETYPE_F64},
  {"f128", CORETYPE_F128},
  {"char", CORETYPE_CHAR},
  {"str", CORETYPE_STR},
  {"array", CORETYPE_ARRAY},
  {"object", CORETYPE_OBJECT},
  {"unknown", CORETYPE_UNKNOWN},
};

static KindStrPair stringToKind[] = {
  {"#unknown",      UNKNOWN},
  {"<first-token>", FIRST_TOKEN},
  {"<last-token>",  LAST_TOKEN},
  {"mut",           MUT},
  {"#Type",         WJTYPE},
  {"#Boolean",      BOOLEAN},
  {"#Number",       NUMBER},
  {"#String",       STRING},
  {"#identifier",   IDENTIFIER},
  {"fn",            FUNCTION},
  {"ret",           RETURN},
  {"let",           VARIABLE},
  {"for",           FOR},
  {"break",         BREAK},
  {"continue",      CONTINUE},
  {"if",            IF},
  {"elif",          ELIF},
  {"else",          ELSE},
  {"&&",            LOGICALAND},
  {"||",            LOGICALOR},
  {"=",             ASSIGNMENT},
  {"+",             ADD},
  {"-",             SUBTRACT},
  {"*",             MULTIPLY},
  {"/",             DIVIDE},
  {"%%",             MODULO},
  {"==",            EQUAL},
  {"!=",            NOTEQUAL},
  {"<",             LESSTHEN},
  {">",             GREATERTHAN},
  {"<=",            LESSOREQUAL},
  {">=",            GREATEROREQUAL},
  {",",             COMMA},
  {":",             COLON},
  {";",             SEMI},
  {"(",             LEFTPAREN},
  {")",             RIGHTPAREN},
  {"{",             LEFTBRACE},
  {"}",             RIGHTBRACE},
  {"[",             LEFTBRAKET},
  {"]",             RIGHTBRAKET},
};

Kind parseKind(char* string) {
  for(size_t i=0;i<sizeof(stringToKind)/sizeof(KindStrPair);i++) {
    if (strcmp(stringToKind[i].str, string) == 0)
      return stringToKind[i].kind;
  }
  return UNKNOWN;
}

char* kindToString(Kind kind) {
  for(size_t i=0;i<sizeof(stringToKind)/sizeof(KindStrPair);i++) {
    if (stringToKind[i].kind == kind)
      return stringToKind[i].str;
  }
  return NULL;
}

PrimitiveCoreType parseType(char* string) {
  for(size_t i=0;i<sizeof(stringToType)/sizeof(TypeStrPair);i++) {
    if (strcmp(stringToType[i].str, string) == 0)
      return stringToType[i].coretype;
  }
  return CORETYPE_UNKNOWN;
}

char* typeToString(PrimitiveCoreType type) {
  for(size_t i=0;i<sizeof(stringToType)/sizeof(TypeStrPair);i++) {
    if (stringToType[i].coretype == type)
      return stringToType[i].str;
  }
  return NULL;
}

tokstat* newTokStat(char* string) {
  tokstat* ts = (struct tokstat*)malloc(sizeof(struct tokstat));
  if (ts == NULL) errExit(EMSG_MEMALLOC_FAILED);
  ts->current = string;
  ts->tokidx = 0;
  ts->tokcap = 1;
  ts->lineno = 1;
  ts->coloff = 0;
  ts->state = STATE_INIT;
  ts->err = 0;
  ts->done = E_OK;
  return ts;
}