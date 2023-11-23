#ifndef __WOOJINC_WOOJIN_H__
#define __WOOJINC_WOOJIN_H__
#define MULTI(...) do { __VA_ARGS__; } while (0);

typedef        int type_t;
typedef struct CompilerMessage CompilerMessage;
typedef enum   ComplierMessageKind ComplierMessageKind;
typedef struct Error Error;
typedef struct Warning Warning;
typedef struct Notice Notice;
typedef struct Token Token;
typedef struct TokenPos TokenPos;
typedef struct Tokenizer Tokenizer;
typedef struct w__ast__IdentVar w__ast__IdentVar;
typedef struct w__ast__IdentInfo w__ast__IdentInfo;
typedef enum   w__ast__IdentKind w__ast__IdentKind;
typedef struct w__ast__Var w__ast__Var;
typedef struct w__ast__CallArg w__ast__CallArg;
typedef struct w__ast__IfBranch w__ast__IfBranch;
typedef struct w__ast__Param w__ast__Param;
typedef struct w__ast__GlobalField w__ast__GlobalField;
typedef struct w__ast__Node w__ast__Node;
typedef enum   w__ast__NodeKind w__ast__NodeKind;
typedef enum   w__ast__OpCode w__ast__OpCode;
typedef struct w__ast__OrExpr w__ast__OrExpr;
typedef struct w__ast__AndExpr w__ast__AndExpr;
typedef struct w__ast__RelationalExpr w__ast__RelationalExpr;
typedef struct w__ast__ArithmeticExpr w__ast__ArithmeticExpr;
typedef struct w__ast__UnaryExpr w__ast__UnaryExpr;
typedef struct w__ast__CallExpr w__ast__CallExpr;
typedef struct w__ast__IfExpr w__ast__IfExpr;
typedef struct w__ast__Ident w__ast__Ident;
typedef struct w__ast__ParExpr w__ast__ParExpr;
typedef struct w__ast__PostfixExpr w__ast__PostfixExpr;
typedef struct w__ast__IntegerLiteral w__ast__IntegerLiteral;
typedef enum   IntegerType IntegerType;
typedef struct w__ast__FloatLiteral w__ast__FloatLiteral;
typedef enum   FloatType FloatType;
typedef struct w__ast__BoolLiteral w__ast__BoolLiteral;
typedef struct w__ast__StringLiteral w__ast__StringLiteral;
typedef struct w__ast__Expr w__ast__Expr;
typedef enum   w__ast__ExprKind w__ast__ExprKind;
typedef struct w__ast__AssignStatement w__ast__AssignStatement;
typedef struct w__ast__FnDecl w__ast__FnDecl;
typedef struct w__ast__Block w__ast__Block;
typedef struct w__ast__ExprStatement w__ast__ExprStatement;
typedef struct w__ast__Import w__ast__Import;
typedef struct w__ast__Return w__ast__Return;
typedef struct w__ast__ForStatement w__ast__ForStatement;
typedef struct w__ast__GlobalDecl w__ast__GlobalDecl;
typedef struct w__ast__AliasTypeDecl w__ast__AliasTypeDecl;
typedef struct w__ast__TypeDecl w__ast__TypeDecl;
typedef struct w__ast__Statement w__ast__Statement;
typedef enum   w__ast__StatementKind w__ast__StatementKind;
typedef struct w__parser__Parser w__parser__Parser;
typedef struct w__ast__IfGuardVar w__ast__IfGuardVar;
typedef struct ComplierOption ComplierOption;
typedef enum   TokenKind TokenKind;
typedef enum   CharType CharType;
typedef struct List List;
typedef struct MapItem MapItem;
typedef struct Map Map;
typedef struct Table Table;
typedef struct TypeInfo TypeInfo;
typedef enum   TypeInfoKind TypeInfoKind;
typedef enum   TypeFlag TypeFlag;
typedef enum   WoojinKind WoojinKind;
typedef struct Alias Alias;
typedef struct TypeSymbol TypeSymbol;
typedef enum   Language Language;
typedef struct Scope Scope;
typedef struct ScopeObject ScopeObject;
typedef enum   ScopeObjectKind ScopeObjectKind;

typedef        List Array_w__type_t;
typedef struct List Array_w__ast__Param;
typedef struct List Array_w__ast__CallArg;
typedef struct List Array_w__ast__Expr;
typedef struct List Array_w__ast__Statement;
typedef struct List Array_w__ast__Literal;
typedef struct List Array_w__ast__Node;
typedef struct List ArrayString;
typedef struct List Array_w__error__Error;
typedef struct List Array_w__error__Warning;
typedef struct List Array_w__error__Notice;
typedef struct List Array_w__TypeSymbolPtr;
typedef struct List Array_w__ast__Token;
typedef struct List Array_Scope_ptr;
typedef struct List Array_w__ast__IfBranch;

typedef struct Map StringBoolMap;
typedef struct Map StringIntMap;
typedef struct Map StringScopeObjectMap;

typedef        w__ast__Expr (*native_fn)(Array_w__ast__Expr* arguments);
#endif