#pragma once
#include "krtools.h"

/*取驱动模块基址与大小*/
SKSTATUS KrGetKernelModuleBase(PUCHAR moduleName, PULONG64 pModuleSize,PULONG64 base) {
	RTL_PROCESS_MODULES SysModules = { 0 };
	PRTL_PROCESS_MODULES pModules = &SysModules;
	ULONG64 SystemInformationLength = 0;

	if (!MmIsAddressValid(moduleName) || !MmIsAddressValid(pModuleSize) || !MmIsAddressValid(base))
	{
		return SK_FAILED_GETKERNELBASE_BADPARAM;
	}


	NTSTATUS status = ZwQuerySystemInformation(SystemModuleInformation, pModules, sizeof(RTL_PROCESS_MODULES), &SystemInformationLength);
	if (status == STATUS_INFO_LENGTH_MISMATCH) {
		pModules = ExAllocatePool(NonPagedPool, SystemInformationLength + sizeof(RTL_PROCESS_MODULES));
		RtlZeroMemory(pModules, SystemInformationLength + sizeof(RTL_PROCESS_MODULES));
		status = ZwQuerySystemInformation(SystemModuleInformation, pModules, SystemInformationLength + sizeof(RTL_PROCESS_MODULES), &SystemInformationLength);
		if (!NT_SUCCESS(status)) {
			ExFreePool(pModules);
			EcUpdateLastSysErrCode(status);
			return SK_FAILED_GETKERNELBASE_QUERYSYSINFO;
		}
	}

	if (!strcmp("ntoskrnl.exe", moduleName) || !strcmp("ntkrnlpa.exe.exe", moduleName)) {
		*pModuleSize = pModules->Modules[0].ImageSize;
		*base = pModules->Modules[0].ImageBase;
		if (SystemInformationLength) {
			ExFreePool(pModules);
		}
		return SK_SUCCESS;
	}

	for (ULONG i = 0; i < pModules->NumberOfModules; i++) {
		if (strstr(pModules->Modules[i].FullPathName, moduleName)) {
			*pModuleSize = pModules->Modules[i].ImageSize;
			*base = pModules->Modules[i].ImageBase;
			if (SystemInformationLength) {
				ExFreePool(pModules);
			}
			return SK_SUCCESS;
		}
	}
	if (SystemInformationLength) {
		ExFreePool(pModules);
	}
	return SK_FAILED_GETKERNELBASE_NOMODULE;
}


/*根据指定区段名取区段相对偏移*/
SKSTATUS PeGetSectionOffsetByName(PUCHAR ptr, PUCHAR name, PULONG size,PULONG offset) {
	if (!MmIsAddressValid(ptr) || !MmIsAddressValid(name) || !MmIsAddressValid(offset))
	{
		return SK_FAILED_GETSECTIONOFFSET_BADPARAM;
	}

	PIMAGE_DOS_HEADER dos = (PIMAGE_DOS_HEADER)ptr;
	PIMAGE_NT_HEADERS nt = (PIMAGE_NT_HEADERS)(dos->e_lfanew + (ULONG64)ptr);
	ULONG SectionNum = nt->FileHeader.NumberOfSections;
	PIMAGE_SECTION_HEADER SectionBase = IMAGE_FIRST_SECTION(nt);
	for (ULONG i = 0; i < SectionNum; i++) {
		if (!strcmp(SectionBase->Name, name))
		{
			*size = SectionBase->Misc.VirtualSize;
			*offset =  SectionBase->VirtualAddress;
			return SK_SUCCESS;
		}
		SectionBase++;
	}

	return SK_FAILED_GETSECTIONOFFSET_NOSECTION;
}

/*特征码搜寻  AC??AC 格式*/
SKSTATUS MmFindAddrBySignCode(PUCHAR startAddr, PUCHAR sign, ULONG len,PULONG64 base) {
	ULONG signArr[0x100] = { 0 };
	ULONG index = 0;
	ULONG signBytes = strlen(sign) / 2;
	for (index = 0; index < signBytes; index++)
	{
		ULONG signIndex = index * 2;
		char temp1 = sign[signIndex];
		char temp2 = sign[signIndex + 1];
		ULONG high = 0;
		ULONG low = 0;
		if (temp1 == '?' && temp2 == '?')
		{
			signArr[index] = 999;
			continue;
		}
		if (temp1 < '0' || temp1>'F' || temp2 < '0' || temp2>'F')
		{
			return SK_FAILED_SIGNSEARCH_BADSIGN;
		}
		if (temp1 >= '0' && temp1 <= '9')
		{
			high = temp1 - 48;
		}
		if (temp1 >= 'A' && temp1 <= 'F')
		{
			high = temp1 - 65 + 10;
		}
		if (temp2 >= '0' && temp2 <= '9')
		{
			low = temp2 - 48;
		}
		if (temp2 >= 'A' && temp2 <= 'F')
		{
			low = temp2 - 65 + 10;
		}
		signArr[index] = high * 16 + low;
	}
	PUCHAR currentPtr = startAddr;
	ULONG rightBytes = 0;
	while ((currentPtr - startAddr) <= (len - signBytes)) {
		for (ULONG i = 0; i < signBytes; i++)
		{
			if (signArr[i] == 999) {
				rightBytes++;
				continue;
			}
			if (!MmIsAddressValid(currentPtr + i))
			{
				currentPtr = (PUCHAR)((ULONG64)currentPtr & 0xFFFFFFFFFFFFF000L) + 0x1000;
				rightBytes = 0;
				break;
			}
			if (signArr[i] != (UCHAR) * (currentPtr + i))
			{
				//currentPtr = currentPtr + i + 1;
				currentPtr = currentPtr + 1;
				rightBytes = 0;
				break;
			}
			rightBytes++;
		}
		if (rightBytes == signBytes)
		{
			*base = currentPtr;
			return SK_SUCCESS;
		}
	}
	return SK_FAILED_SIGNSEARCH_NOTFIND;
}


ULONG64 cjstModuleSize = NULL;
ULONG64 cjstModuleBase = NULL;
ULONG64 jmpAddr = NULL;
SKSTATUS KrCreateJmpSysThread(ULONG64 func) {
	SKSTATUS ret = NULL;
	if (!cjstModuleBase)
	{
		KrGetKernelModuleBase("ntoskrnl.exe", &cjstModuleSize, &cjstModuleBase);
		if (ret != SK_SUCCESS) return ret;
	}
	if (!jmpAddr)
	{
		ret = MmFindAddrBySignCode(cjstModuleBase,"CCCCCCCC", cjstModuleSize,&jmpAddr);
		if (ret != SK_SUCCESS) return ret;
		
	}
	if (*(PUCHAR)(jmpAddr) == 0xCC)
	{
		UCHAR ttt[2] = { 0xFF,0xE1 };
		ret = MmWriteKernelMemory(jmpAddr, &ttt[0], 2);
		if (ret != SK_SUCCESS) return ret;
	}
	
	HANDLE tHandle = NULL;
	NTSTATUS tRet = PsCreateSystemThread(&tHandle, THREAD_ALL_ACCESS, NULL, NULL, NULL, jmpAddr, func);
	if (NT_SUCCESS(tRet)) {
		ZwClose(tHandle);//相当于CloseHandle
	}
	return (SKSTATUS)tRet;
}


SKSTATUS KrGetPsSuspendThreadAddr(PULONG64 funcBase) {
	SKSTATUS ret = NULL;
	ULONG64 size = 0;
	PUCHAR base = NULL;
	ret = KrGetKernelModuleBase("ntoskrnl.exe", &size, &base);
	if (ret != SK_SUCCESS)
	{
		return ret;
	}
	*funcBase = base + SymGetFuncOffOfMod(SYM_MOD_NTOS,SYM_FUNC_NTSUSPENDTHREAD);
	return SK_SUCCESS;
}
SKSTATUS KrGetPsResumeThreadAddr(PULONG64 funcBase) {
	SKSTATUS ret = NULL;
	ULONG size = 0;
	PUCHAR base = NULL;
	ret = KrGetKernelModuleBase("ntoskrnl.exe", &size, &base);
	if (ret != SK_SUCCESS)
	{
		return ret;
	}
	*funcBase = base + SymGetFuncOffOfMod(SYM_MOD_NTOS, SYM_FUNC_NTRESUMETHREAD);
	return SK_SUCCESS;
}
SKSTATUS KrGetVirtualProtectFuncAddr(PULONG64 funcBase) {
	SKSTATUS ret = NULL;
	ULONG size = 0;
	PUCHAR base = NULL;
	ret = KrGetKernelModuleBase("ntoskrnl.exe", &size, &base);
	if (ret != SK_SUCCESS)
	{
		return ret;
	}
	*funcBase = base + SymGetFuncOffOfMod(SYM_MOD_NTOS, SYM_FUNC_VirtualProtect);
	return SK_SUCCESS;
}
SKSTATUS KrGetExpLookupHandleTableEntryAddr(PULONG64 funcBase) {
	SKSTATUS ret = NULL;
	ULONG size = 0;
	PUCHAR base = NULL;
	ret = KrGetKernelModuleBase("ntoskrnl.exe", &size, &base);
	if (ret != SK_SUCCESS)
	{
		return ret;
	}
	PUCHAR addr = base + SymGetFuncOffOfMod(SYM_MOD_NTOS,SYM_FUNC_ExpLookupHandleTableEntry);
	*funcBase = addr;
	return SK_SUCCESS;
}
/*全局句柄表二级指针 需要再取一次值*/
SKSTATUS KrGetGlobalHandleTablePointer(PULONG64 funcBase) {
	SKSTATUS ret = NULL;
	ULONG size = 0;
	PUCHAR base = NULL;
	ret = KrGetKernelModuleBase("ntoskrnl.exe", &size, &base);
	if (ret != SK_SUCCESS)
	{
		return ret;
	}
	PUCHAR addr = base + SymGetFuncOffOfMod(SYM_MOD_NTOS, SYM_FUNC_PspCidTable);
	*funcBase = addr;
	return SK_SUCCESS;
}
SKSTATUS KrGetMiLocateAddressAddr(PULONG64 funcBase) {
	SKSTATUS ret = NULL;
	ULONG size = 0;
	PUCHAR base = NULL;
	ret = KrGetKernelModuleBase("ntoskrnl.exe", &size, &base);
	if (ret != SK_SUCCESS)
	{
		return ret;
	}
	PUCHAR addr = base + SymGetFuncOffOfMod(SYM_MOD_NTOS, SYM_FUNC_MiLocateAddress);
	*funcBase = addr;
	return SK_SUCCESS;
}