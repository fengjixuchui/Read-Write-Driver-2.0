#pragma once
#include "filetool.h"



SKSTATUS FsDeleteFile(char* path)
{
	OBJECT_ATTRIBUTES objFile = { 0 };
	HANDLE hFile = NULL;
	IO_STATUS_BLOCK ioBlock = { 0 };
	UNICODE_STRING unFileName = { 0 };
	ANSI_STRING ansiPath = { 0 };
	RtlInitAnsiString(&ansiPath, path);
	RtlAnsiStringToUnicodeString(&unFileName, &ansiPath, TRUE);
	//RtlInitUnicodeString(&unFileName, path);
	InitializeObjectAttributes(&objFile, &unFileName, OBJ_CASE_INSENSITIVE, NULL, NULL);
	NTSTATUS status = ZwCreateFile(
		&hFile,
		GENERIC_READ,
		&objFile,
		&ioBlock,
		NULL,
		FILE_ATTRIBUTE_NORMAL,
		FILE_SHARE_READ,
		FILE_OPEN_IF, FILE_NON_DIRECTORY_FILE, NULL, NULL);

	if (!NT_SUCCESS(status))
	{
		RtlFreeUnicodeString(&unFileName);
		EcUpdateLastSysErrCode(status);
		return SK_FAILED_FILETOOL_CREATEFILE;
	}

	PFILE_OBJECT pFile = NULL;

	status = ObReferenceObjectByHandle(hFile,
		FILE_ALL_ACCESS,
		*IoFileObjectType, KernelMode, &pFile, NULL);

	if (!NT_SUCCESS(status))
	{
		RtlFreeUnicodeString(&unFileName);
		ZwClose(hFile);
		EcUpdateLastSysErrCode(status);
		return SK_FAILED_FILETOOL_BADFHANDLE;
	}

	pFile->DeleteAccess = TRUE;
	pFile->DeletePending = FALSE;

	pFile->SectionObjectPointer->DataSectionObject = NULL;
	pFile->SectionObjectPointer->ImageSectionObject = NULL;
	//pFile->SectionObjectPointer->SharedCacheMap = NULL;

	ObDereferenceObject(pFile);
	ZwClose(hFile);

	status = ZwDeleteFile(&objFile);
	RtlFreeUnicodeString(&unFileName);
	if (!NT_SUCCESS(status))
	{
		EcUpdateLastSysErrCode(status);
		return SK_FAILED_FILETOOL_DELETE;
	}
	
	return SK_SUCCESS;
}