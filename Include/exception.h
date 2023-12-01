#ifndef __WOOJINC_EXCEPTION_H__
#define __WOOJINC_EXCEPTION_H__
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdbool.h>
#include <locale.h>

typedef enum ExceptionType {
  WoojinExce_UnresolvedName,
} ExceptionType;

typedef struct {
  char* name;
  ExceptionType type;
  wchar_t *value;
  wchar_t *doc;
} ExceTable;

// void rt_err(const wchar_t* format, ...);
#define rt_err(a,b) runtime_err(exctable[a].value, b)

#ifdef __WOOJINC_ERROR_LANG_KO__
#define ERM_PRIFIX L"런타임 오류"
#define ERM_UNRESOL_NAME L"이 범위에서 `%S` 값을 찾을 수 없습니다"
#elif defined(__WOOJINC_ERROR_LANG_EN__)
#define ERM_PRIFIX L"Runtime Error"
#define ERM_UNRESOL_NAME L"cannot find value `%S` in this scope"
#elif defined(__WOOJINC_ERROR_LANG_UA__)
#define ERM_PRIFIX L"Помилка під час виконання"
#define ERM_UNRESOL_NAME L"не вдалося знайти значення `%S` в цьому області видимості"
#else
#define ERM_PRIFIX L"Ошибка времени выполнения"
#define ERM_UNRESOL_NAME L"не удалось найти значение `%S` в этой области видимости"
#endif
#endif