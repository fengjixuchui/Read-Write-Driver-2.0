/*

#pragma once
#include "mmtool.h"

SKSTATUS MmWriteKernelMemory(ULONG64 dst, PUCHAR buf, ULONG64 len) {
	if (!MmIsAddressValid((PVOID)dst) || !MmIsAddressValid((PVOID)(dst + len)))
	{
		return SK_FAILED_MMTOOL_BADPARAM;
	}
	PPHYSICAL_ADDRESS pAddr = NULL;
	PUCHAR newAddr = MmMapIoSpace(MmGetPhysicalAddress(dst), len, MmNonCached);
	if (!MmIsAddressValid((PVOID)newAddr) || !MmIsAddressValid((PVOID)(newAddr + len)))
	{
		return SK_FAILED_MMTOOL_BADPARAM;
	}
	memcpy(newAddr, buf, len);
	return SK_SUCCESS;
}


ULONG64 ipfPid = NULL;
ULONG64 ipfAddr[100] = { 0 };
VOID MmInPageFile() {
	if (!ipfPid)return;
	PEPROCESS proc = NULL;
	NTSTATUS ntRet = PsLookupProcessByProcessId(ipfPid, &proc);
	if (!NT_SUCCESS(ntRet) || PsGetProcessExitStatus(proc) != 0x103)
	{
		ipfPid = NULL;
		return;
	}
	ObDereferenceObject(proc);
	PEPROCESS cur = PsGetCurrentProcess();

	for (int i = 0; i < 100; i++)
	{
		if (ipfAddr[i]) {
			ULONG64 temp = 0;
			ULONG64 len = 0;

			ntRet = MmCopyVirtualMemory(proc, ipfAddr[i], cur, &temp, 1, KernelMode, &len);
			ipfAddr[i] = 0;
		}
	}
}

ULONG64 LAST_INADDR = NULL;
SKSTATUS MmReadProcessMemory_Force(ULONG64 pid, ULONG64 dst, PUCHAR buf, ULONG64 len,ULONG64 tryAgain) {
	if (buf >= MmHighestUserAddress || (buf + len - 1) > MmHighestUserAddress || buf < 0x1000 || !MmIsAddressValid(buf) || !MmIsAddressValid(buf + len - 1) || len > 0x1000)
	{
		return SK_FAILED_MMREAD_BADADDR;
	}
	PEPROCESS proc = NULL;
	NTSTATUS ntRet = NULL;
	SKSTATUS ret = NULL;


	ntRet = PsLookupProcessByProcessId(pid, &proc);
	if (!NT_SUCCESS(ntRet) || PsGetProcessExitStatus(proc) != 0x103)
	{
		EcUpdateLastSysErrCode(ntRet);
		return SK_FAILED_MMREAD_INVALIDPID;
	}
	ObDereferenceObject(proc);


	PUCHAR bufR0 = (PUCHAR)ExAllocatePool(NonPagedPool, len + 10);
	if (!bufR0)
	{
		return SK_FAILED_ALLOCATEPOOL;
	}
	RtlZeroMemory(bufR0, len + 10);
	memcpy(bufR0, buf, len);

	PETHREAD dstThread = NtGetProcessMainThread(proc);
	if (!dstThread)
	{
		ExFreePool(bufR0);
		return SK_FAILED_PROCKR_BADTHREAD;
	}

	PKTHREAD currentThread = KeGetCurrentThread();
	ULONG currentThreadOffset = NULL;
	ret = KrGetKpcrCurrentThreadOffset(&currentThreadOffset);
	if (ret != SK_SUCCESS)
	{
		ExFreePool(bufR0);
		return ret;
	}

	ULONG eprocOffset = KrGetEprocessOffset();
	ULONG64 currentProc = (ULONG64)PsGetCurrentProcess();
	BOOLEAN writeSuccess = FALSE;


	_disable();
	__writegsqword(currentThreadOffset, dstThread);
	*(PULONG64)((ULONG64)currentThread + eprocOffset) = proc;
	ULONG64 oldCr0 = __readcr0();
	__writecr0(oldCr0 & (~0x10000));
	ULONG64 oldCr3 = __readcr3();
	__writecr3(*(PULONG64)((PUCHAR)proc + 0x28));
	if (MmIsAddressValid(dst) && MmIsAddressValid(dst + len - 1))
	{
		writeSuccess = TRUE;
		memcpy(bufR0, dst, len);
	}
	__writecr3(oldCr3);
	__writecr0(oldCr0);
	*(PULONG64)((ULONG64)currentThread + eprocOffset) = currentProc;
	__writegsqword(currentThreadOffset, currentThread);
	_enable();



	if (!writeSuccess && tryAgain)
	{
		PMEMORY_BASIC_INFORMATION memInfoStart = NULL;
		PMEMORY_BASIC_INFORMATION memInfoEnd = NULL;
		ret = MmQueryMemInfo(pid, dst, &memInfoStart);
		ret |= MmQueryMemInfo(pid, dst + len - 1, &memInfoEnd);
		if (ret != SK_SUCCESS)
		{
			ExFreePool(bufR0);
			return ret;
		}
		if (memInfoStart->State == MEM_COMMIT)
		{
			if (memInfoEnd->State == MEM_COMMIT)
			{
				//memInfoStart->Protect & PAGE_GUARD == 0 && memInfoEnd->Protect & PAGE_GUARD == 0
				if (LAST_INADDR!= memInfoStart->BaseAddress)
				{
					*(PULONG64)((ULONG64)currentThread + eprocOffset) = proc;
					ULONG64 oldCr0 = __readcr0();
					__writecr0(oldCr0 & (~0x10000));

					ULONG64 copyTemp = 0;
					ret = MmCopyVirtualMemory(proc,dst, currentProc, bufR0,len,KernelMode,&copyTemp);

					writeSuccess = NT_SUCCESS(ret);

					__writecr0(oldCr0);
					*(PULONG64)((ULONG64)currentThread + eprocOffset) = currentProc;
				}
			}
		}
	}
	memcpy(buf, bufR0, len);
	ExFreePool(bufR0);
	if (writeSuccess)
	{
		return SK_SUCCESS;
	}
	return SK_FAILED_MMWRITE_BADDSTADDR;
}










//ÆÆÐéÄâ»¯ Ç¿Ð´ ÎÞºÛ
SKSTATUS MmWriteProcessMemory_Force(ULONG64 pid, ULONG64 dst, PUCHAR buf, ULONG64 len,ULONG64 tryAgain) {
	if (buf >= MmHighestUserAddress || (buf + len-1) > MmHighestUserAddress || buf < 0x1000 || !MmIsAddressValid(buf) || !MmIsAddressValid(buf + len - 1) || len > 0x1000)
	{
		return SK_FAILED_MMREAD_BADADDR;
	}
	PEPROCESS proc = NULL;
	NTSTATUS ntRet = NULL;
	SKSTATUS ret = NULL;


	ntRet = PsLookupProcessByProcessId(pid, &proc);
	if (!NT_SUCCESS(ntRet) || PsGetProcessExitStatus(proc) != 0x103)
	{
		EcUpdateLastSysErrCode(ntRet);
		return SK_FAILED_MMREAD_INVALIDPID;
	}
	ObDereferenceObject(proc);


	PUCHAR bufR0 = (PUCHAR)ExAllocatePool(NonPagedPool, len + 10);
	if (!bufR0)
	{
		return SK_FAILED_ALLOCATEPOOL;
	}
	RtlZeroMemory(bufR0, len + 10);
	memcpy(bufR0,buf,len);

	PETHREAD dstThread = NtGetProcessMainThread(proc);
	if (!dstThread)
	{
		ExFreePool(bufR0);
		return SK_FAILED_PROCKR_BADTHREAD;
	}

	PKTHREAD currentThread = KeGetCurrentThread();
	ULONG currentThreadOffset = NULL;
	ret = KrGetKpcrCurrentThreadOffset(&currentThreadOffset);
	if (ret != SK_SUCCESS)
	{
		ExFreePool(bufR0);
		return ret;
	}

	ULONG eprocOffset = KrGetEprocessOffset();
	ULONG64 currentProc = (ULONG64)PsGetCurrentProcess();
	BOOLEAN writeSuccess = FALSE;


	_disable();
	__writegsqword(currentThreadOffset, dstThread);
	*(PULONG64)((ULONG64)currentThread + eprocOffset) = proc;
	ULONG64 oldCr0 = __readcr0();
	__writecr0(oldCr0 & (~0x10000));
	ULONG64 oldCr3 = __readcr3();
	__writecr3(*(PULONG64)((PUCHAR)proc + 0x28));
	if (MmIsAddressValid(dst) && MmIsAddressValid(dst+len-1))
	{
		writeSuccess = TRUE;
		memcpy(dst, bufR0, len);
	}
	__writecr3(oldCr3);
	__writecr0(oldCr0);
	*(PULONG64)((ULONG64)currentThread + eprocOffset) = currentProc;
	__writegsqword(currentThreadOffset, currentThread);
	_enable();



	if (!writeSuccess && tryAgain)
	{
		PMEMORY_BASIC_INFORMATION memInfoStart = NULL;
		PMEMORY_BASIC_INFORMATION memInfoEnd = NULL;
		ret = MmQueryMemInfo(pid, dst, &memInfoStart);
		ret |= MmQueryMemInfo(pid, dst + len - 1, &memInfoEnd);
		if (ret != SK_SUCCESS)
		{
			ExFreePool(bufR0);
			return ret;
		}
		if (memInfoStart->State == MEM_COMMIT)
		{
			if (memInfoEnd->State == MEM_COMMIT)
			{
				if (LAST_INADDR != memInfoStart->BaseAddress)
				{
					*(PULONG64)((ULONG64)currentThread + eprocOffset) = proc;
					ULONG64 oldCr0 = __readcr0();
					__writecr0(oldCr0 & (~0x10000));

					ULONG64 copyTemp = 0;
					ret = MmCopyVirtualMemory(currentProc, bufR0, proc, dst, len, KernelMode, &copyTemp);

					writeSuccess = NT_SUCCESS(ret);

					__writecr0(oldCr0);
					*(PULONG64)((ULONG64)currentThread + eprocOffset) = currentProc;
				}
			}
		}
	}

	ExFreePool(bufR0);
	if (writeSuccess)
	{
		return SK_SUCCESS;
	}
	return SK_FAILED_MMWRITE_BADDSTADDR;
}












ULONG64 qmPid = NULL;
PMEMORY_BASIC_INFORMATION qmMemInfoArr[0x1000] = { 0 };
ULONG64 curNum = 0;

VOID MqClear() {
	for (ULONG64 i = 0; i < 0x1000; i++)
	{
		if (qmMemInfoArr[i]) ExFreePool(qmMemInfoArr[i]);
		qmMemInfoArr[i] = 0;
	}
	curNum = 0;
}
SKSTATUS MqAdd(PEPROCESS proc, ULONG64 addr, PMEMORY_BASIC_INFORMATION* info) {
	NTSTATUS stRet = NULL;
	PMEMORY_BASIC_INFORMATION pBaseInformation = NULL;
	pBaseInformation = (PMEMORY_BASIC_INFORMATION)ExAllocatePool(PagedPool, sizeof(MEMORY_BASIC_INFORMATION));
	KAPC_STATE apc = { 0 };
	KeStackAttachProcess(proc, &apc);
	SIZE_T retLen = 0;
	stRet = ZwQueryVirtualMemory(NtCurrentProcess(), addr, MemoryBasicInformation, pBaseInformation, sizeof(MEMORY_BASIC_INFORMATION), &retLen);
	KeUnstackDetachProcess(&apc);
	if (!NT_SUCCESS(stRet))
	{
		return SK_FAILED_MMQUERY_FAILED;
	}
	*info = pBaseInformation;
	curNum++;
	for (ULONG64 i = 0; i < 0x1000; i++)
	{
		if (!qmMemInfoArr[i]) {
			qmMemInfoArr[i] = pBaseInformation;
			return SK_SUCCESS;
		}
	}
}

SKSTATUS MmQueryMemInfo(ULONG64 pid,ULONG64 addr, PMEMORY_BASIC_INFORMATION * info) {
	if (addr >= MmHighestUserAddress  || addr < 0x1000)
	{
		return SK_FAILED_MMQUERY_BADPARAM;
	}
	SKSTATUS ret = NULL;
	NTSTATUS stRet = NULL;
	PEPROCESS proc = NULL;
	stRet = PsLookupProcessByProcessId(pid,&proc);
	if (!NT_SUCCESS(stRet)) {
		if (qmPid == pid)
		{
			qmPid = 0;
			MqClear();
		}
		return SK_FAILED_MMQUERY_BADPID;
	}
	if (PsGetProcessExitStatus(proc) != 0x103) {
		if (qmPid == pid)
		{
			qmPid = 0;
			MqClear();
		}
		return SK_FAILED_MMQUERY_PROCALREADYEXIT;
	}

	if (qmPid == pid)
	{
		ULONG64 num = 0;
		for (ULONG64 i = 0; i < 0x1000; i++)
		{
			PMEMORY_BASIC_INFORMATION cur = qmMemInfoArr[i];
			if (cur)
			{
				num++;
				if (num>curNum)
				{
					break;
				}
				ULONG64 begin = (ULONG64)cur->BaseAddress;
				ULONG64 end = (ULONG64)cur->BaseAddress + cur->RegionSize;
				if (addr >= begin && addr < end)
				{
					*info = cur;
					return SK_SUCCESS;
				}
			}
		}
	}
	else
	{
		qmPid = pid;
		MqClear();
	}
	return MqAdd(proc, addr, info);
}



*/