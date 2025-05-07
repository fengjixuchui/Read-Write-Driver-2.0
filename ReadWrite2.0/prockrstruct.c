#pragma once
#include "procstruct.h"


PEPROCESS gpmtP[0x200] = { 0 };
PETHREAD gpmtT[0x200] = { 0 };
int gpmtI = 0;
PETHREAD NtGetProcessMainThread(PEPROCESS Process)
{
	PETHREAD ethread = NULL;
	//start  优化处理，如果不做优化，超高频率下的取主线程，会导致机器内存爆满。
	if (gpmtP[gpmtI] == Process)
	{
		if (gpmtT[gpmtI] == 0) {
			gpmtP[gpmtI] = 0;
			gpmtI--;
		}
		else
		{
			return gpmtT[gpmtI];
		}
	}
	for (int i = 0; i < 0x200; i++)
	{
		if (Process == gpmtP[i]) {
			if (gpmtT[i] == 0)
			{
				gpmtP[i] = 0;
				break;
			}
			else
			{
				return gpmtT[i];
			}
		}
	}
	//end  优化处理，如果不做优化，超高频率下的取主线程，会导致机器内存爆满。

	KAPC_STATE kApcState = { 0 };

	KeStackAttachProcess(Process, &kApcState);

	HANDLE hThread = NULL;

	NTSTATUS status = NtGetNextThread(NtCurrentProcess(), NULL, THREAD_ALL_ACCESS,
		OBJ_KERNEL_HANDLE | OBJ_CASE_INSENSITIVE, 0, &hThread);
	if (NT_SUCCESS(status))
	{

		status = ObReferenceObjectByHandle(hThread, THREAD_ALL_ACCESS,
			*PsThreadType, KernelMode, &ethread, NULL);
		NtClose(hThread);

		if (!NT_SUCCESS(status))
		{
			ethread = NULL;
		}
	}
	//if (ethread)
	//{
	//	ObDereferenceObject(ethread);
	//}

	KeUnstackDetachProcess(&kApcState);
	//start  优化处理，如果不做优化，超高频率下的取主线程，会导致机器内存爆满。
	gpmtI++;
	gpmtP[gpmtI] = Process;
	gpmtT[gpmtI] = ethread;
	//end  优化处理，如果不做优化，超高频率下的取主线程，会导致机器内存爆满。
	return ethread;
}

NTSTATUS NTAPI NtGetNextThread(
	__in HANDLE ProcessHandle,
	__in HANDLE ThreadHandle,
	__in ACCESS_MASK DesiredAccess,
	__in ULONG HandleAttributes,
	__in ULONG Flags,
	__out PHANDLE NewThreadHandle
)
{

	typedef NTSTATUS(NTAPI* ZwGetNextThreadProc)(
		__in HANDLE ProcessHandle,
		__in HANDLE ThreadHandle,
		__in ACCESS_MASK DesiredAccess,
		__in ULONG HandleAttributes,
		__in ULONG Flags,
		__out PHANDLE NewThreadHandle
		);

	static ZwGetNextThreadProc ZwGetNextThreadFunc = NULL;
	if (!ZwGetNextThreadFunc)
	{
		UNICODE_STRING unName = { 0 };
		RtlInitUnicodeString(&unName, L"ZwGetNextThread");
		ZwGetNextThreadFunc = (ZwGetNextThreadProc)MmGetSystemRoutineAddress(&unName);
		if (!ZwGetNextThreadFunc)
		{
			UNICODE_STRING unName = { 0 };
			RtlInitUnicodeString(&unName, L"ZwGetNotificationResourceManager");
			PUCHAR ZwGetNotificationResourceManagerAddr = (PUCHAR)MmGetSystemRoutineAddress(&unName);
			ZwGetNotificationResourceManagerAddr -= 0x50;
			for (int i = 0; i < 0x30; i++)
			{
				if (ZwGetNotificationResourceManagerAddr[i] == 0x48
					&& ZwGetNotificationResourceManagerAddr[i + 1] == 0x8B
					&& ZwGetNotificationResourceManagerAddr[i + 2] == 0xC4)
				{
					ZwGetNextThreadFunc = ZwGetNotificationResourceManagerAddr + i;
					break;
				}
			}
		}
	}

	if (ZwGetNextThreadFunc)
	{
		return ZwGetNextThreadFunc(ProcessHandle, ThreadHandle, DesiredAccess,
			HandleAttributes, Flags, NewThreadHandle);
	}

	return STATUS_UNSUCCESSFUL;
}

PPEB32 PsGetWow64ProcPeb32(PEPROCESS proc, PPEB32* peb) {
	PPEB32 peb32 = NULL;
	ULONG buildNum = NULL;
	SKSTATUS ret = NULL;
	ret = GlbGetBuildNumber(&buildNum);
	if (ret != SK_SUCCESS)
	{
		return SK_FAILED_INITCOM_BADVERSION;
	}
	if (buildNum == 7601 || buildNum == 7600)
	{
		peb32 = (PPEB32) * ((PULONG64)((PUCHAR)proc + SymGetMemOffOfStruct(SYM_MOD_NTOS,SYM_STRUCT_EPROCESS,SYM_STRUCTMEM_EPROCESS_WoW64Process)));
	}
	else
	{
		ULONG64 tmp = *((((PULONG64)((PUCHAR)proc + SymGetMemOffOfStruct(SYM_MOD_NTOS, SYM_STRUCT_EPROCESS, SYM_STRUCTMEM_EPROCESS_WoW64Process)))));
		if (tmp)
		{
			peb32 = (PPEB32) * (PULONG64)(tmp);
		}
	}
	*peb = peb32;
	return SK_SUCCESS;
}

SKSTATUS PsGetThreadApcProcess(PETHREAD thread, PEPROCESS* proc) {
	SKSTATUS ret = NULL;
	if (!thread || !MmIsAddressValid(thread))
	{
		return SK_FAILED_RMCALL_INVALIDTID;
	}
	*proc = *(PULONG64)((PUCHAR)thread + SymGetMemOffOfStruct(SYM_MOD_NTOS,SYM_STRUCT_KTHREAD,SYM_STRUCTMEM_KTHREAD_ApcState) + 
		SymGetMemOffOfStruct(SYM_MOD_NTOS,SYM_STRUCT_KAPC_STATE,SYM_STRUCTMEM_KAPC_STATE_Process));
	return SK_SUCCESS;
}
SKSTATUS PsGetThreadTeb64(PUCHAR thread, PTEB64* teb) {
	SKSTATUS ret = NULL;
	if (!thread || !MmIsAddressValid(thread))
	{
		return SK_FAILED_RMCALL_INVALIDTID;
	}
	*teb = *(PULONG64)(thread + SymGetMemOffOfStruct(SYM_MOD_NTOS,SYM_STRUCT_KTHREAD,SYM_STRUCTMEM_KTHREAD_Teb));
	return SK_SUCCESS;
}
SKSTATUS PsGetThreadTrapFrame(PUCHAR thread, PKTRAP_FRAME* trap) {
	if (!thread || !MmIsAddressValid(thread))
	{
		return SK_FAILED_RMCALL_INVALIDTID;
	}

	*trap = *(PULONG64)(thread + SymGetMemOffOfStruct(SYM_MOD_NTOS, SYM_STRUCT_KTHREAD, SYM_STRUCTMEM_KTHREAD_InitialStack)) - sizeof(KTRAP_FRAME);
	return SK_SUCCESS;
}