#ifndef __WOOJINC_SIGNAL_H__
#define __WOOJINC_SIGNAL_H__
#include "type.h"

struct ReturnSignal;
struct BreakSignal;
struct ContinueSignal;
struct Signal;
struct wjvalue;
typedef struct ReturnSignal { struct wjvalue *result; } ReturnSignal;
typedef struct BreakSignal { struct wjvalue *result; } BreakSignal;
typedef struct ContinueSignal { struct wjvalue *result; } ContinueSignal;

typedef enum SignalType {
  RETURN_SIGNAL,
  BREAK_SIGNAL,
  CONTINUE_SIGNAL
} SignalType;

typedef struct Signal {
  SignalType type;
  union {
    ReturnSignal* returnSignal;
    BreakSignal* breakSignal;
    ContinueSignal* continueSignal;
  } _sig;
} Signal;
#endif