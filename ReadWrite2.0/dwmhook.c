#pragma once
#include "dwmhook.h"

/* 记录dwm进程EPROCESS  创建守护dwm进程  */

PEPROCESS dwmProc = NULL;
BOOLEAN alreadyInstall = FALSE;
SKSTATUS DwmHookStep0() {
	if (alreadyInstall)
	{
		return SK_DWMDRAW_ALREADYINSTALL;
	}
	else {
		return SK_SUCCESS;
	}
}

SKSTATUS DwmHookStep1(ULONG64 pid) {
	ULONG64 dwmPid = NULL;
	SKSTATUS ret = NULL;
	NTSTATUS ntRet = NULL;

	ret = PsGetPidByName("dwm.exe", &dwmPid);
	if (ret != SK_SUCCESS) return ret;
	
	ntRet = PsLookupProcessByProcessId(dwmPid,&dwmProc);
	if (!dwmProc) return SK_DWMDRAW_FAILEDGETDWMEPROC;

	ULONG64 pidOff = SymGetMemOffOfStruct(SYM_MOD_NTOS,SYM_STRUCT_EPROCESS,SYM_STRUCTMEM_EPROCESS_UniqueProcessId);
	//ret = KrGetPidOffset(&pidOff);
	//if (ret != SK_SUCCESS) return ret;

	*(PULONG64)(pid) = *(PULONG64)((ULONG64)dwmProc + pidOff);

	return SK_SUCCESS;
}

SKSTATUS DwmHookStep2() {
	alreadyInstall = TRUE;
	return SK_SUCCESS;
}
