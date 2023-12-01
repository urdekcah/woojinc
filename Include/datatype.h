#ifndef __WOOJINC_DATATYPE_H__
#define __WOOJINC_DATATYPE_H__
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "node.h"
#include "type.h"
typedef struct FuncTable { char* str; BuiltinFunction fn; } FuncTable;
#define BUILTIN_SIZE 3
wjvalue* builtin_print(List* values);
wjvalue* bulitin_println(List* values);
wjvalue* type(List* values);

bool isSize(wjvalue value);
bool isNull(wjvalue value);
bool isTrue(wjvalue value);
bool isFalse(wjvalue value);
bool isBoolean(wjvalue value);
bool isNumber(wjvalue value);
wjvalue toNumber(wjvalue value);
size_t toSize(wjvalue value);
wjvalue getValueOfArray(wjvalue object, wjvalue index);
wjvalue setValueOfArray(wjvalue object, wjvalue index, wjvalue value);
struct Function* toFunction(wjvalue value);
BuiltinFunction toBuiltinFunction(wjvalue value);
#endif