#ifndef __WOOJINC_WOOJIN_H__
#define __WOOJINC_WOOJIN_H__
#define MULTI(...) do { __VA_ARGS__; } while (0);

typedef        int type_t;
typedef struct CompilerMessage CompilerMessage;
typedef struct Token Token;
typedef struct Tokenizer Tokenizer;
typedef enum   TokenKind TokenKind;
typedef enum   CharType CharType;
typedef struct List List;
typedef struct MapItem MapItem;
typedef struct Map Map;
#endif