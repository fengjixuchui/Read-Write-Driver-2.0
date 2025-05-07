#pragma once
#include "petool.h"


SKSTATUS PeGetExportFuncAddr64(PUCHAR base, PUCHAR funcName,PULONG64 ExpAddr) {
	if (!MmIsAddressValid(base) || !MmIsAddressValid(funcName) || !MmIsAddressValid(ExpAddr))
	{
		return SK_FAILED_PETOOL_BADPARAM;
	}
	PIMAGE_DOS_HEADER dos = (PIMAGE_DOS_HEADER)base;
	PIMAGE_NT_HEADERS64 nt = (PIMAGE_NT_HEADERS64)((ULONG64)base + dos->e_lfanew);
	ULONG64 ImageBase = nt->OptionalHeader.ImageBase;
	PIMAGE_DATA_DIRECTORY pExport = &(nt->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT]);
	PIMAGE_EXPORT_DIRECTORY pExDir = pExport->VirtualAddress + base;
	ULONG64 NumberOfFuncs = pExDir->NumberOfFunctions;
	ULONG64 NumberOfNames = pExDir->NumberOfNames;
	PULONG AddrOfFuncs = pExDir->AddressOfFunctions + base;
	PULONG AddrOfNames = pExDir->AddressOfNames + base;
	PUSHORT AddrOfNameOrd = pExDir->AddressOfNameOrdinals + base;
	for (ULONG64 i = 0; i < NumberOfNames-1; i++)
	{
		PUCHAR preName = AddrOfNames[i] + base;
		//if (!MmIsAddressValid(preName))
		//{
		//	continue;
		//	//return SK_FAILED_PETOOL_BADPRENAME;
		//}
		ANSI_STRING funcNameStr = { 0 };
		RtlInitAnsiString(&funcNameStr, funcName);
		ANSI_STRING preNameStr = { 0 };
		RtlInitAnsiString(&preNameStr, preName);
		if (!RtlCompareString(&funcNameStr, &preNameStr,TRUE)) {
			*ExpAddr = AddrOfFuncs[AddrOfNameOrd[i]] + base;
			return SK_SUCCESS;
		}
	}
	return SK_FAILED_PETOOL_FUNCNOTEXIST;
}
SKSTATUS PeGetExportFuncAddr32(PUCHAR base, PUCHAR funcName, PULONG64 ExpAddr) {
	if (!MmIsAddressValid(base) || !MmIsAddressValid(funcName) || !MmIsAddressValid(ExpAddr))
	{
		return SK_FAILED_PETOOL_BADPARAM;
	}
	PIMAGE_DOS_HEADER dos = (PIMAGE_DOS_HEADER)base;
	PIMAGE_NT_HEADERS32 nt = (PIMAGE_NT_HEADERS32)((ULONG64)base + dos->e_lfanew);
	ULONG64 ImageBase = nt->OptionalHeader.ImageBase;
	PIMAGE_DATA_DIRECTORY pExport = &(nt->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT]);
	PIMAGE_EXPORT_DIRECTORY pExDir = pExport->VirtualAddress + base;
	ULONG64 NumberOfFuncs = pExDir->NumberOfFunctions;
	ULONG64 NumberOfNames = pExDir->NumberOfNames;
	PULONG AddrOfFuncs = pExDir->AddressOfFunctions + base;
	PULONG AddrOfNames = pExDir->AddressOfNames + base;
	PUSHORT AddrOfNameOrd = pExDir->AddressOfNameOrdinals + base;

	for (ULONG64 i = 0; i < NumberOfNames-1; i++)
	{
		PUCHAR preName = AddrOfNames[i] + base;
		//if (!MmIsAddressValid(preName))
		//{
		//	return SK_FAILED_PETOOL_BADPRENAME;
		//}
		ANSI_STRING funcNameStr = { 0 };
		RtlInitAnsiString(&funcNameStr, funcName);
		ANSI_STRING preNameStr = { 0 };
		RtlInitAnsiString(&preNameStr, preName);
		if (!RtlCompareString(&funcNameStr, &preNameStr,TRUE)) {
			*ExpAddr =  AddrOfFuncs[AddrOfNameOrd[i]] + base;
			return SK_SUCCESS;
		}
	}
	return SK_FAILED_PETOOL_FUNCNOTEXIST;
}