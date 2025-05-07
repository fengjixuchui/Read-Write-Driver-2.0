#pragma once 
#include "symtool.h"

SYM_INFO symList[100] = { 0 };

SKSTATUS SymInitSym() {
	NTSTATUS ntRet = NULL;

	OBJECT_ATTRIBUTES objFile = { 0 };
	HANDLE hFile = NULL;
	IO_STATUS_BLOCK ioBlock = { 0 };
	UNICODE_STRING unFileName = { 0 };
	RtlInitUnicodeString(&unFileName,L"\\??\\C:\\ProgramData\\Silky\\sym.dat");
	InitializeObjectAttributes(&objFile, &unFileName, OBJ_CASE_INSENSITIVE, NULL, NULL);
	ntRet = ZwCreateFile(
		&hFile,
		GENERIC_READ,
		&objFile,
		&ioBlock,
		NULL,
		FILE_ATTRIBUTE_NORMAL,
		FILE_SHARE_READ,
		FILE_OPEN_IF, FILE_NON_DIRECTORY_FILE, NULL, NULL);

	if (!NT_SUCCESS(ntRet))
	{
		return SK_FAILED_FILETOOL_CREATEFILE;
	}
	PFILE_OBJECT pFile = NULL;

	ntRet = ObReferenceObjectByHandle(hFile,
		GENERIC_READ,
		*IoFileObjectType, KernelMode, &pFile, NULL);

	if (!NT_SUCCESS(ntRet))
	{
		ZwClose(hFile);
		return SK_FAILED_FILETOOL_BADFHANDLE;
	}
	LARGE_INTEGER fileSize = { 0 };
	ntRet = FsRtlGetFileSize(pFile,&fileSize);
	ObDereferenceObject(pFile);
	if (!NT_SUCCESS(ntRet))
	{
		ZwClose(hFile);
		return SK_FAILED_FILETOOL_CREATEFILE;
	}
	LARGE_INTEGER fileOffset = { 0 };
	ntRet = NtReadFile(hFile,NULL,NULL,NULL,&ioBlock, symList, fileSize.LowPart,&fileOffset,NULL);
	ZwClose(hFile);
	if (!NT_SUCCESS(ntRet))
	{
		return SK_FAILED_FILETOOL_CREATEFILE;
	}

	for (int i = 0; i < fileSize.QuadPart; i++)
	{
		*((char*)symList + i) ^= 0xCC;
		*((char*)symList + i) -= 0x70;
	}
	return SK_SUCCESS;
}



ULONG64 SymGetFuncOffOfMod(CHAR mod,CHAR func) {
	for (int i = 0; i < 100; i++)
	{
		if (symList[i].type == SYM_TYPE_FUNC
			&& symList[i].mod == mod
			&& symList[i].name1 == func)
		{
			return symList[i].off;
		}
	}
	return -1;
}
ULONG64 SymGetMemOffOfStruct(CHAR mod, CHAR structName,CHAR mem) {
	for (int i = 0; i < 100; i++)
	{
		if (symList[i].type == SYM_TYPE_STRUCT
			&& symList[i].mod == mod
			&& symList[i].name1 == structName
			&& symList[i].name2 == mem)
		{
			return symList[i].off;
		}
	}
	return -1;
}