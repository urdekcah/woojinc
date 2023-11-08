#ifndef __WOOJINC_ERROR_H__
#define __WOOJINC_ERROR_H__
#include <stdio.h>
#include <stdbool.h>
#include <locale.h>
#include <errno.h>
#include "woojin.h"

struct CompilerMessage { char* message; char* detail; };
struct Error { CompilerMessage msg; };
struct Warning { CompilerMessage msg; };
struct Notice { CompilerMessage msg; };

void errExit(bool detail, const wchar_t* format, ...);
#define EEXIT(...) errExit(false, __VA_ARGS__)
#define EEXITD(...) errExit(true, __VA_ARGS__)
void lastError();

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
#endif