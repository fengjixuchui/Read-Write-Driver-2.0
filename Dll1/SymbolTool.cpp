#pragma once
#include "SymbolTool.h"

#pragma comment(lib, "DIA2Dump.lib")

SYM_INFO symList[100] = { 0 };
int nowIndex = 0;

BOOLEAN InitAllNtosFunc() {
	DWORD off = 0;

	if (GetOsBuildNumber() >= 8000)
	{
		off = ExpGetRvaBySymbolName(L"*NtConvertBetweenAuxiliaryCounterAndPerformanceCounter*", 0);
		if (!off)
		{
			return FALSE;
		}
		symList[nowIndex].type = SYM_TYPE_FUNC;
		symList[nowIndex].mod = SYM_MOD_NTOS;
		symList[nowIndex].name1 = SYM_FUNC_NTCBACAP;
		symList[nowIndex++].off = off;
	}

	off = ExpGetRvaBySymbolName(L"*NtSuspendThread*", 0);
	if (!off)
	{
		return FALSE;
	}
	symList[nowIndex].type = SYM_TYPE_FUNC;
	symList[nowIndex].mod = SYM_MOD_NTOS;
	symList[nowIndex].name1 = SYM_FUNC_NTSUSPENDTHREAD;
	symList[nowIndex++].off = off;

	off = ExpGetRvaBySymbolName(L"*NtResumeThread*", 0);
	if (!off)
	{
		return FALSE;
	}
	symList[nowIndex].type = SYM_TYPE_FUNC;
	symList[nowIndex].mod = SYM_MOD_NTOS;
	symList[nowIndex].name1 = SYM_FUNC_NTRESUMETHREAD;
	symList[nowIndex++].off = off;

	off = ExpGetRvaBySymbolName(L"*ExpLookupHandleTableEntry*", 0);
	if (!off)
	{
		return FALSE;
	}
	symList[nowIndex].type = SYM_TYPE_FUNC;
	symList[nowIndex].mod = SYM_MOD_NTOS;
	symList[nowIndex].name1 = SYM_FUNC_ExpLookupHandleTableEntry;
	symList[nowIndex++].off = off;

	off = ExpGetRvaBySymbolName(L"*PspCidTable*", 0);
	if (!off)
	{
		return FALSE;
	}
	symList[nowIndex].type = SYM_TYPE_FUNC;
	symList[nowIndex].mod = SYM_MOD_NTOS;
	symList[nowIndex].name1 = SYM_FUNC_PspCidTable;
	symList[nowIndex++].off = off;

	off = ExpGetRvaBySymbolName(L"*NtQueryVirtualMemory*", 0);
	if (!off)
	{
		return FALSE;
	}
	symList[nowIndex].type = SYM_TYPE_FUNC;
	symList[nowIndex].mod = SYM_MOD_NTOS;
	symList[nowIndex].name1 = SYM_FUNC_NtQueryVirtualMemory;
	symList[nowIndex++].off = off;

	off = ExpGetRvaBySymbolName(L"*NtProtectVirtualMemory*", 0);
	if (!off)
	{
		return FALSE;
	}
	symList[nowIndex].type = SYM_TYPE_FUNC;
	symList[nowIndex].mod = SYM_MOD_NTOS;
	symList[nowIndex].name1 = SYM_FUNC_VirtualProtect;
	symList[nowIndex++].off = off;


	off = ExpGetRvaBySymbolName(L"*MiLocateAddress*", 0);
	if (!off)
	{
		return FALSE;
	}
	symList[nowIndex].type = SYM_TYPE_FUNC;
	symList[nowIndex].mod = SYM_MOD_NTOS;
	symList[nowIndex].name1 = SYM_FUNC_MiLocateAddress;
	symList[nowIndex++].off = off;

	return TRUE;
}

BOOLEAN InitAllNtosStruct() {
	DWORD off = 0;

	off = ExpGetMemberOffsetOfStruct(L"*EPROCESS", L"*WoW64Process*", 0, 0);
	if (off == -1)
	{
		return FALSE;
	}
	symList[nowIndex].type = SYM_TYPE_STRUCT;
	symList[nowIndex].mod = SYM_MOD_NTOS;
	symList[nowIndex].name1 = SYM_STRUCT_EPROCESS;
	symList[nowIndex].name2 = SYM_STRUCTMEM_EPROCESS_WoW64Process;
	symList[nowIndex++].off = off;


	off = ExpGetMemberOffsetOfStruct(L"*EPROCESS", L"*UniqueProcessId*", 0, 0);
	if (off == -1)
	{
		return FALSE;
	}
	symList[nowIndex].type = SYM_TYPE_STRUCT;
	symList[nowIndex].mod = SYM_MOD_NTOS;
	symList[nowIndex].name1 = SYM_STRUCT_EPROCESS;
	symList[nowIndex].name2 = SYM_STRUCTMEM_EPROCESS_UniqueProcessId;
	symList[nowIndex++].off = off;


	off = ExpGetMemberOffsetOfStruct(L"*EPROCESS", L"*InheritedFromUniqueProcessId*", 0, 0);
	if (off == -1)
	{
		return FALSE;
	}
	symList[nowIndex].type = SYM_TYPE_STRUCT;
	symList[nowIndex].mod = SYM_MOD_NTOS;
	symList[nowIndex].name1 = SYM_STRUCT_EPROCESS;
	symList[nowIndex].name2 = SYM_STRUCTMEM_EPROCESS_InheritedFromUniqueProcessId;
	symList[nowIndex++].off = off;


	off = ExpGetMemberOffsetOfStruct(L"*EPROCESS", L"*ImageFileName*", 0, 0);
	if (off == -1)
	{
		return FALSE;
	}
	symList[nowIndex].type = SYM_TYPE_STRUCT;
	symList[nowIndex].mod = SYM_MOD_NTOS;
	symList[nowIndex].name1 = SYM_STRUCT_EPROCESS;
	symList[nowIndex].name2 = SYM_STRUCTMEM_EPROCESS_ImageFileName;
	symList[nowIndex++].off = off;
	

	off = ExpGetMemberOffsetOfStruct(L"*EPROCESS", L"*ImagePathHash*", 0, 0);
	if (off == -1)
	{
		return FALSE;
	}
	symList[nowIndex].type = SYM_TYPE_STRUCT;
	symList[nowIndex].mod = SYM_MOD_NTOS;
	symList[nowIndex].name1 = SYM_STRUCT_EPROCESS;
	symList[nowIndex].name2 = SYM_STRUCTMEM_EPROCESS_ImagePathHash;
	symList[nowIndex++].off = off;


	off = ExpGetMemberOffsetOfStruct(L"*EPROCESS", L"*SeAuditProcessCreationInfo*", 0, 0);
	if (off == -1)
	{
		return FALSE;
	}
	symList[nowIndex].type = SYM_TYPE_STRUCT;
	symList[nowIndex].mod = SYM_MOD_NTOS;
	symList[nowIndex].name1 = SYM_STRUCT_EPROCESS;
	symList[nowIndex].name2 = SYM_STRUCTMEM_EPROCESS_SeAuditProcessCreationInfo;
	symList[nowIndex++].off = off;


	off = ExpGetMemberOffsetOfStruct(L"*EPROCESS", L"*SectionBaseAddress*", 0, 0);
	if (off == -1)
	{
		return FALSE;
	}
	symList[nowIndex].type = SYM_TYPE_STRUCT;
	symList[nowIndex].mod = SYM_MOD_NTOS;
	symList[nowIndex].name1 = SYM_STRUCT_EPROCESS;
	symList[nowIndex].name2 = SYM_STRUCTMEM_EPROCESS_SectionBaseAddress;
	symList[nowIndex++].off = off;
	

	off = ExpGetMemberOffsetOfStruct(L"*EPROCESS", L"*SectionObject*", 0, 0);
	if (off == -1)
	{
		return FALSE;
	}
	symList[nowIndex].type = SYM_TYPE_STRUCT;
	symList[nowIndex].mod = SYM_MOD_NTOS;
	symList[nowIndex].name1 = SYM_STRUCT_EPROCESS;
	symList[nowIndex].name2 = SYM_STRUCTMEM_EPROCESS_SectionObject;
	symList[nowIndex++].off = off;


	off = ExpGetMemberOffsetOfStruct(L"*KTHREAD", L"*ApcState*", 0, 0);
	if (off == -1)
	{
		return FALSE;
	}
	symList[nowIndex].type = SYM_TYPE_STRUCT;
	symList[nowIndex].mod = SYM_MOD_NTOS;
	symList[nowIndex].name1 = SYM_STRUCT_KTHREAD;
	symList[nowIndex].name2 = SYM_STRUCTMEM_KTHREAD_ApcState;
	symList[nowIndex++].off = off;


	off = ExpGetMemberOffsetOfStruct(L"*KTHREAD", L"*Teb*", 0, 0);
	if (off == -1)
	{
		return FALSE;
	}
	symList[nowIndex].type = SYM_TYPE_STRUCT;
	symList[nowIndex].mod = SYM_MOD_NTOS;
	symList[nowIndex].name1 = SYM_STRUCT_KTHREAD;
	symList[nowIndex].name2 = SYM_STRUCTMEM_KTHREAD_Teb;
	symList[nowIndex++].off = off;


	off = ExpGetMemberOffsetOfStruct(L"*KTHREAD", L"*TrapFrame*", 0, 0);
	if (off == -1)
	{
		return FALSE;
	}
	symList[nowIndex].type = SYM_TYPE_STRUCT;
	symList[nowIndex].mod = SYM_MOD_NTOS;
	symList[nowIndex].name1 = SYM_STRUCT_KTHREAD;
	symList[nowIndex].name2 = SYM_STRUCTMEM_KTHREAD_TrapFrame;
	symList[nowIndex++].off = off;


	off = ExpGetMemberOffsetOfStruct(L"*KTHREAD", L"*InitialStack*", 0, 0);
	if (off == -1)
	{
		return FALSE;
	}
	symList[nowIndex].type = SYM_TYPE_STRUCT;
	symList[nowIndex].mod = SYM_MOD_NTOS;
	symList[nowIndex].name1 = SYM_STRUCT_KTHREAD;
	symList[nowIndex].name2 = SYM_STRUCTMEM_KTHREAD_InitialStack;
	symList[nowIndex++].off = off;


	off = ExpGetMemberOffsetOfStruct(L"*KAPC_STATE", L"*Process*", 0, 0);
	if (off == -1)
	{
		return FALSE;
	}
	symList[nowIndex].type = SYM_TYPE_STRUCT;
	symList[nowIndex].mod = SYM_MOD_NTOS;
	symList[nowIndex].name1 = SYM_STRUCT_KAPC_STATE;
	symList[nowIndex].name2 = SYM_STRUCTMEM_KAPC_STATE_Process;
	symList[nowIndex++].off = off;


	off = ExpGetMemberOffsetOfStruct(L"*KPRCB", L"*CurrentThread*", 0, 0);
	if (off == -1)
	{
		return FALSE;
	}
	symList[nowIndex].type = SYM_TYPE_STRUCT;
	symList[nowIndex].mod = SYM_MOD_NTOS;
	symList[nowIndex].name1 = SYM_STRUCT_KPRCB;
	symList[nowIndex].name2 = SYM_STRUCTMEM_KPRCB_CurrentThread;
	symList[nowIndex++].off = off;
	return TRUE;
}

BOOLEAN InitAllD2d1Func() {
	DWORD off = 0;
	off = ExpGetRvaBySymbolName(L"*PresentDWM@?$D2DDeviceContextBase@UID2D1DeviceContext6*", 0);
	if (!off)
	{
		return FALSE;
	}
	symList[nowIndex].type = SYM_TYPE_FUNC;
	symList[nowIndex].mod = SYM_MOD_D2D1;
	symList[nowIndex].name1 = SYM_FUNC_PresentDWM;
	symList[nowIndex++].off = off;
	
	off = ExpGetRvaBySymbolName(L"*PresentDWM@?$D2DDeviceContextBase@UID2D1DeviceContext6*", 1);
	if (!off)
	{
		return FALSE;
	}
	symList[nowIndex].type = SYM_TYPE_FUNC;
	symList[nowIndex].mod = SYM_MOD_D2D1;
	symList[nowIndex].name1 = SYM_FUNC_PresentDWM2;
	symList[nowIndex++].off = off;

	off = ExpGetRvaBySymbolName(L"*PresentMultiplaneOverlay@?$D2DDeviceContextBase@UID2D1DeviceContext6*", 0);
	if (!off)
	{
		return FALSE;
	}
	symList[nowIndex].type = SYM_TYPE_FUNC;
	symList[nowIndex].mod = SYM_MOD_D2D1;
	symList[nowIndex].name1 = SYM_FUNC_PresentMultiplaneOverlay;
	symList[nowIndex++].off = off;

	off = ExpGetRvaBySymbolName(L"*PresentMultiplaneOverlay@?$D2DDeviceContextBase@UID2D1DeviceContext6*", 1);
	if (!off)
	{
		return FALSE;
	}
	symList[nowIndex].type = SYM_TYPE_FUNC;
	symList[nowIndex].mod = SYM_MOD_D2D1;
	symList[nowIndex].name1 = SYM_FUNC_PresentMultiplaneOverlay2;
	symList[nowIndex++].off = off;
	return TRUE;
}

BOOLEAN InitAllDxgiFunc() {
	DWORD off = 0;
	off = ExpGetRvaBySymbolName(L"*Present@*IDXGISwapChainDWM*", 0);
	if (!off)
	{
		return FALSE;
	}
	symList[nowIndex].type = SYM_TYPE_FUNC;
	symList[nowIndex].mod = SYM_MOD_DXGI;
	symList[nowIndex].name1 = SYM_FUNC_Win7Present;
	symList[nowIndex++].off = off;
	return TRUE;
}

BOOLEAN SymInitDatFile() {
#ifdef _WIN32
	PVOID oldfs = 0;
	Wow64DisableWow64FsRedirection(&oldfs);
#endif // _WIN32

	CreateDirectoryA("C:\\ProgramData\\Silky", NULL);
	DeleteFileA("C:\\ProgramData\\Silky\\sym.dat");
	HANDLE hDat = CreateFileA("C:\\ProgramData\\Silky\\sym.dat", GENERIC_READ | GENERIC_WRITE,0,NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_HIDDEN,NULL);
	if (hDat == INVALID_HANDLE_VALUE) return FALSE;


	{
		if (!ExpDownloadPdbTo("C:\\Windows\\System32\\ntoskrnl.exe", "C:\\ProgramData\\Silky\\ntkrnlmp.pdb"))
		{
			return FALSE;
		}

		if (!ExpLoadPdbFile(L"C:\\ProgramData\\Silky\\ntkrnlmp.pdb"))
		{
			return FALSE;
		}
		if (!InitAllNtosFunc() || !InitAllNtosStruct())
		{
			CloseHandle(hDat);
			ExpUnLoadPdbFile();
			return FALSE;
		}
		ExpUnLoadPdbFile();
	}

	if(GetOsBuildNumber() >= 8000){
		if (!ExpDownloadPdbTo("C:\\Windows\\System32\\d2d1.dll", "C:\\ProgramData\\Silky\\d2d1.pdb"))
		{
			CloseHandle(hDat);
			return FALSE;
		}

		if (!ExpLoadPdbFile(L"C:\\ProgramData\\Silky\\d2d1.pdb"))
		{
			CloseHandle(hDat);
			return FALSE;
		}
		if (!InitAllD2d1Func())
		{
			CloseHandle(hDat);
			ExpUnLoadPdbFile();
			return FALSE;
		}
		ExpUnLoadPdbFile();
	}
	else
	{
		if (!ExpDownloadPdbTo("C:\\Windows\\System32\\dxgi.dll", "C:\\ProgramData\\Silky\\dxgi.pdb"))
		{
			CloseHandle(hDat);
			return FALSE;
		}

		if (!ExpLoadPdbFile(L"C:\\ProgramData\\Silky\\dxgi.pdb"))
		{
			CloseHandle(hDat);
			return FALSE;
		}
		if (!InitAllDxgiFunc())
		{
			CloseHandle(hDat);
			ExpUnLoadPdbFile();
			return FALSE;
		}
		ExpUnLoadPdbFile();
	}

	



	for (int i = 0; i < sizeof(SYM_INFO) * (nowIndex + 1); i++)
	{
		*((char*)symList + i) += 0x70;
		*((char*)symList + i) ^= 0xCC;
	}
	DWORD lpNumberOfBytesWritten = 0;
	BOOLEAN wRet = WriteFile(hDat, symList,sizeof(SYM_INFO)* (nowIndex+1),&lpNumberOfBytesWritten,NULL);
	CloseHandle(hDat);
#ifdef _WIN32
	Wow64RevertWow64FsRedirection(oldfs);
#endif // _WIN32
	return wRet;
}
