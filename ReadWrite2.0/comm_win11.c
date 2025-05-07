#pragma once
#include "comm_win10.h"
#include "comm.h"

Win11_off_140401C70CallBack oldCallBack_Win11_off_140401C70 = NULL;


NTSTATUS  CallBack_Win11_off_140401C70(PUCHAR p1, PUCHAR p2, PUCHAR p3) {
	if (ComHandlePackage((PComPackage)p1) && oldCallBack_Win11_off_140401C70)
	{
		return oldCallBack_Win11_off_140401C70(p1, p2, p3);
	}
	return STATUS_SUCCESS;
}
//NtConvertBetweenAuxiliaryCounterAndPerformanceCounter
SKSTATUS ComInitWin11Commmunication() {
	ULONG kernelSize = 0;
	SKSTATUS ret = NULL;
	ULONG64 base = 0;
	ret = KrGetKernelModuleBase("ntoskrnl.exe", &kernelSize,&base);
	if (ret != SK_SUCCESS)
	{
		return ret;
	}
	
	ULONG64 funcOff = SymGetFuncOffOfMod(SYM_MOD_NTOS, SYM_FUNC_NTCBACAP);
	PUCHAR func = (PUCHAR)(base + funcOff);
	ret = MmFindAddrBySignCode(func, "75??488B05", 0x200 * 0x10, &func);
	if (ret != SK_SUCCESS)
	{
		return ret;
	}
	LARGE_INTEGER li = { 0 };
	li.QuadPart = func + 9;
	li.LowPart += *(PLONG)(func + 5);
	PULONG64 global = li.QuadPart;
	oldCallBack_Win11_off_140401C70 = (Win10_off_140401C70CallBack)global[0];
	global[0] = CallBack_Win11_off_140401C70;
	return SK_SUCCESS;
}
