#pragma once
#include <ntifs.h>
#include "global.h"
#include "krtools.h"

EXTERN_C PETHREAD NtGetProcessMainThread(PEPROCESS Process);
NTSTATUS NTAPI NtGetNextThread(
	__in HANDLE ProcessHandle,
	__in HANDLE ThreadHandle,
	__in ACCESS_MASK DesiredAccess,
	__in ULONG HandleAttributes,
	__in ULONG Flags,
	__out PHANDLE NewThreadHandle
);
PPEB32 PsGetWow64ProcPeb32(PEPROCESS proc, PPEB32* peb);
SKSTATUS PsGetThreadApcProcess(PETHREAD thread, PEPROCESS* proc);
SKSTATUS PsGetThreadTeb64(PUCHAR thread, PTEB64* teb);
SKSTATUS PsGetThreadTrapFrame(PUCHAR thread, PKTRAP_FRAME* trap);