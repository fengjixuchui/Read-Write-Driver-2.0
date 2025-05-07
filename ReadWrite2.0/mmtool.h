#pragma once
#include <ntifs.h>
#include "global.h"
#include "krtools.h"
#include "prockr.h"
#include "vadtools.h"
#include "mmtools_signcode.h"

//SKSTATUS MmReadProcessMemory_ApiCopy(ULONG64 pid, ULONG64 dst, PUCHAR buf, ULONG64 len);
//SKSTATUS MmReadProcessMemory_Attach(ULONG64 pid, ULONG64 dst, PUCHAR buf, ULONG64 len);
SKSTATUS MmReadProcessMemory_Force(ULONG64 pid, ULONG64 dst, PUCHAR buf, ULONG64 len,ULONG64 tryAgain);
//SKSTATUS MmWriteProcessMemory(ULONG64 pid, ULONG64 dst, PUCHAR buf, ULONG64 len);
EXTERN_C SKSTATUS MmWriteKernelMemory(ULONG64 dst, PUCHAR buf, ULONG64 len);
SKSTATUS MmWriteProcessMemory_Force(ULONG64 pid, ULONG64 dst, PUCHAR buf, ULONG64 len, ULONG64 tryAgain);
SKSTATUS MmQueryMemInfo(ULONG64 pid, ULONG64 addr, PMEMORY_BASIC_INFORMATION* info);
BOOLEAN MmIsAddrOfCurProcHasPhysicalPage(ULONG64 addr);
SKSTATUS MmGetQueryVirAddrFuncAddr(PULONG64 addr);
