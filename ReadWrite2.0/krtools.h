#pragma once
#include <ntifs.h>
#include "global.h"
#include "mmtool.h"
#include "krtools_signcode.h"


EXTERN_C SKSTATUS KrGetKernelModuleBase(PUCHAR moduleName, PULONG64 pModuleSize, PULONG64 base);
EXTERN_C SKSTATUS PeGetSectionOffsetByName(PUCHAR ptr, PUCHAR name, PULONG size, PULONG offset);
EXTERN_C SKSTATUS MmFindAddrBySignCode(PUCHAR startAddr, PUCHAR sign, ULONG len, PULONG64 base);
EXTERN_C SKSTATUS KrCreateJmpSysThread(ULONG64 func);
EXTERN_C SKSTATUS KrGetPsSuspendThreadAddr(PULONG64 funcBase);
EXTERN_C SKSTATUS KrGetPsResumeThreadAddr(PULONG64 funcBase);
EXTERN_C SKSTATUS KrGetVirtualProtectFuncAddr(PULONG64 funcBase);
EXTERN_C SKSTATUS KrGetExpLookupHandleTableEntryAddr(PULONG64 funcBase);
EXTERN_C SKSTATUS KrGetGlobalHandleTablePointer(PULONG64 funcBase);
EXTERN_C SKSTATUS KrGetMiLocateAddressAddr(PULONG64 funcBase);