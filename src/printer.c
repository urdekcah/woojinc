#include <stdio.h>
#include "printer.h"
#include "node.h"
// #include "type.h"

void printTokenList(Token* tokenList) {
  Token *tok = tokenList;
  printf("Printing token list...\n");
  printf("%-12s %-12s %-6s %-6s %-6s %-6s\n", "KIND", "STRING", "LINENO", "COLOFF", "E_COLOFF", "E_LINENO");
  printf("%s\n", "---------------------------------------------------------");
  for (int i = 0; tok != NULL; tok++, i++) {
    if (tok->kind == UNKNOWN) continue;
    printf("%-12s %-12s %-6d %-6d %-6d %-6d\n", kindToString(tok->kind), tok->string, tok->lineno, tok->coloff, tok->e_coloff, tok->e_lineno);
  }
}