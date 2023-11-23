#ifndef __WOOJINC_ERROR_H__
#define __WOOJINC_ERROR_H__
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <locale.h>
#include <errno.h>
#include "woojin.h"
#include "token.h"
#include "color.h"

struct CompilerMessage { char* message; char* detail; char* filename; TokenPos pos; };
struct Error { CompilerMessage msg; };
struct Warning { CompilerMessage msg; };
struct Notice { CompilerMessage msg; };

CompilerMessage* w__error__CompilerMessage__new(char* message, char* detail, char* filename, TokenPos pos);
Error* w__error__Error__new(char* message, char* detail, char* filename, TokenPos pos);
Warning* w__error__Warning__new(char* message, char* detail, char* filename, TokenPos pos);
Notice* w__error__Notice__new(char* message, char* detail, char* filename, TokenPos pos);

enum ComplierMessageKind { CM_ERROR, CM_WARNING, CM_NOTICE };

void w__error__PrintCompilerMessage(CompilerMessage msg, ComplierMessageKind kind);
#define w__errors__PrintErrorMessage(e) w__error__PrintCompilerMessage(e->msg, CM_ERROR)
#define w__errors__PrintWarningMessage(e) w__error__PrintCompilerMessage(e->msg, CM_WARNING)
#define w__errors__PrintNoticeMessage(e) w__error__PrintCompilerMessage(e->msg, CM_NOTICE)

#define PrintComplierMessage(T) _Generic((T), \
  Error*: w__errors__PrintErrorMessage, \
  Warning*: w__errors__PrintWarningMessage, \
  Notice*: w__errors__PrintNoticeMessage, \
)(T)

#define ChooseOneBrightONot(C, O) (O==true?C:C##_BRIGHT)
#define PrintErrorConsoleColor(C) ChooseOneBrightONot(ANSI_FG_RED, C)
#define PrintWarningConsoleColor(C) ChooseOneBrightONot(ANSI_FG_YELLOW, C)
#define PrintNoticeConsoleColor(C) ChooseOneBrightONot(ANSI_FG_BLUE, C)
#define MessageColorSwitch(T,C) (T==CM_ERROR?PrintErrorConsoleColor(C):(T==CM_WARNING?PrintWarningConsoleColor(C):PrintNoticeConsoleColor(C))) 
#define ComplierMsgPrefix(T) (T==CM_ERROR?"error":(T==CM_WARNING?"warning":"note"))

#define ComplierMessageConsoleColor(T,C) _Generic((T), \
  Error*: PrintErrorConsoleColor(C), \
  Warning*: PrintWarningConsoleColor(C), \
  Notice*: PrintNoticeConsoleColor(C) \
)(T);

#define FreeComplierMessageMacro(x) {if(x == NULL)return;FreeAll(&x->msg);FreeAll(x);}
void w__error__CompilerMessage__free(CompilerMessage* self);
void w__error__Error__free(Error* self);
void w__error__Warning__free(Warning* self);
void w__error__Notice__free(Notice* self);

#define FreeComplierMessage(e) _Generic((e), \
  Error*: w__error__Error__free, \
  Warning*: w__error__Warning__free, \
  Notice*: w__error__Notice__free, \
  CompilerMessage*: w__error__CompilerMessage__free \
)(e)

void printError(Error* err);

void errExit(bool detail, const wchar_t* format, ...);
wchar_t* FormatW(const wchar_t* format, ...);
#define ErrExit(...) errExit(false, __VA_ARGS__)
#define ErrDetailExit(...) errExit(true, __VA_ARGS__)
void lastError();

#define D(e) (*e)
#define EK(co) (co.korean)
#define E(e, co) EK(co)?e##_KR:e
#define EP(e, p)  EK(D(p->co))?e##_KR:e

#define E_PREFIX L"ПОМИЛКА"
#define E_UNKNOWN_USAGE L"Використання: %S <command> <ім'я файлу>"
#define E_NOCMD L"Такої команди не знайдено: %S"
#define E_NOWJCF L".wjc файл не вказаний. Спробуйте ще раз."
#define E_OPENFILE L"Файл %S не можливо відкрити. Переконайтеся, що файл існує або перевірте правильність введеного імені файлу, і спробуйте ще раз."
#define E_CLOSEFILE L"Через цю помилку не вдалося закрити файл %S."
#define E_MEMALLOC L"Виділення пам'яті не вдалося."
#define E_UNCLOSEDSTR L"Схоже, рядок не був коректно закритий."
#define E_UNKNOWNCH L"Це невідомий символ. Перевірте, чи ви ввели правильний символ."
#define E_EMPTYARR L"Масив порожній."
#define E_OUTOFBOUNDS L"Виходить за межі масиву."
#define EMSG_UNEXPECTED_TOKEN L"Очікувався токен %S, але отримано %S (на рядку %d, зсув %d)"
#define E_ALREADY_IMPORTED L"Ви вже імпортували цей файл."
#define E_ALREADY_IMPORTED_MODULE L"Ви вже імпортували модуль %S."
#define EMSG_REDECLARED_VAR L"Змінна %S вже була оголошена (на рядку %d, зсув %d)"
#define EMSG_REDEFINED_VAR L"Змінна %S вже була оголошена (на рядку %d, зсув %d)"
#define EMESG_UNDECLARED_VAR L"Змінна %S не була оголошена (на рядку %d, зсув %d)"

#ifndef __WOOJINC_COMPILE_NOT_INCLUDE_KOREAN__
#define E_PREFIX_KR L"오류"
#define E_UNKNOWN_USAGE_KR L"사용법: %S <command> <파일 이름>"
#define E_NOCMD_KR L"그런 명령어를 찾을 수 없습니다: %S"
#define E_NOWJCF_KR L".wjc 파일이 지정되지 않았습니다. 다시 시도해보세요."
#define E_OPENFILE_KR L"%S 파일을 열 수 없습니다. 파일이 존재하는지 또는 입력한 파일 이름이 올바른지 확인하고 다시 시도해보세요."
#define E_CLOSEFILE_KR L"파일 %S을(를) 닫는 데 실패했습니다."
#define E_MEMALLOC_KR L"메모리 할당에 실패했습니다."
#define E_UNCLOSEDSTR_KR L"문자열이 제대로 닫히지 않았습니다."
#define E_UNKNOWNCH_KR L"이 문자는 알 수 없습니다. 올바른 문자를 입력했는지 확인하세요."
#define E_EMPTYARR_KR L"배열이 비어있습니다."
#define E_OUTOFBOUNDS_KR L"배열의 범위를 벗어났습니다."
#define EMSG_UNEXPECTED_TOKEN_KR L"토큰 %S이(가) 예상되었지만 %S이(가) 입력되었습니다 (줄 %d, 위치 %d)"
#define E_ALREADY_IMPORTED_KR L"이 파일은 이미 가져왔습니다."
#define E_ALREADY_IMPORTED_MODULE_KR L"모듈 %S은(는) 이미 가져왔습니다."
#define EMSG_REDECLARED_VAR_KR L"변수 %S은(는) 이미 선언되었습니다 (줄 %d, 위치 %d)"
#define EMSG_REDEFINED_VAR_KR L"변수 %S은(는) 이미 선언되었습니다 (줄 %d, 위치 %d)"
#define EMESG_UNDECLARED_VAR_KR L"변수 %S은(는) 선언되지 않았습니다 (줄 %d, 위치 %d)"
#endif

#endif