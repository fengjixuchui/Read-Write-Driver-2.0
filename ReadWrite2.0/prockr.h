#pragma once
#include <ntifs.h>
#include "global.h"
#include "krtools.h"
#include "procstruct.h"
#include "petool.h"

typedef struct _RemoteCallPackage {
	ULONG64 pid;
	ULONG64 tid;
	ULONG64 mode; // 0 32  1 64
	ULONG64 shellcode;
	ULONG64 codeLen;
	ULONG64 signOffset;
}RemoteCallPackage, * PRemoteCallPackage;

SKSTATUS KrGetProcModuleBaseByName(ULONG pid, char* moduleName, PULONG64 base);
SKSTATUS KrFindWow64ModuleBaseFromPebByName(PPEB32 peb, char* moduleName, ULONG_PTR* base, ULONG64 cr3);
SKSTATUS KrFindX64ModuleBaseFromPebByName(PPEB peb64, char* moduleName, ULONG_PTR* base, ULONG64 cr3);
SKSTATUS MmRemoteCallByWriteContext(ULONG64 pid, ULONG64 tid, ULONG64 mode, ULONG64 shellcode, ULONG64 codeLen);
SKSTATUS MmRemoteCallToWow64Proc(PRemoteCallPackage pack, PETHREAD thread, PEPROCESS proc);
SKSTATUS MmRemoteCallToX64Proc(PRemoteCallPackage pack, PETHREAD thread, PEPROCESS proc);
SKSTATUS PeGetExportFuncAddrOfProcModule(ULONG64 pid, char* moduleName, char* funcName, PULONG64 funcBase);
SKSTATUS MmAllocateProcMemory(ULONG64 pid, ULONG64 size, PULONG64 mmBase);
SKSTATUS MmFreeProcMemory(ULONG64 pid, ULONG64 base);
SKSTATUS MmFindProcAddrBySignCode(ULONG64 pid, ULONG64 base, char* sign, ULONG64 len, PULONG64 addrBase);
EXTERN_C SKSTATUS PsGetPidByName(char* pname, PULONG64 pidOut);
SKSTATUS HideProcessEx(ULONG64 srcPid, ULONG64 fakePid);
SKSTATUS FcProtectProcessByGlobalHandleTable(ULONG64 pid, ULONG64 dstPid,ULONG64 NeedClearObj);
PEPROCESS KrGetEprocByPid(ULONG64 pid);




