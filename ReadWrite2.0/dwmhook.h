#pragma once
#include "errorCode.h"
#include "global.h"
#include "prockr.h"
#include "procstruct.h"

SKSTATUS DwmHookStep0();
SKSTATUS DwmHookStep1(ULONG64 pid);
SKSTATUS DwmHookStep2();