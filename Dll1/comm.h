// dllmain.cpp : 定义 DLL 应用程序的入口点。
#pragma once
#include "stdio.h"
#include "stdlib.h"
#include "Windows.h"
#include "intrin.h"
#include <Urlmon.h>
#include <Wininet.h>
#include "errorCode.h"
#include "SymbolTool.h"


#pragma comment(lib,"Urlmon.lib")
#pragma comment(lib, "Wininet.lib")

typedef struct _IO_STATUS_BLOCK {
	union {
		ULONG Status;
		PVOID    Pointer;
	};
	ULONG_PTR Information;
} IO_STATUS_BLOCK, * PIO_STATUS_BLOCK;


typedef ULONG(WINAPI* NtQueryInformationFileProc)(
	__in HANDLE FileHandle,
	__out PIO_STATUS_BLOCK IoStatusBlock,
	__out_bcount(Length) PVOID FileInformation,
	__in ULONG Length,
	__in ULONG FileInformationClass);

typedef ULONG(WINAPI* pNtConvertBetweenAuxiliaryCounterAndPerformanceCounter)(
	char a1,
	PUCHAR a2,
	PUCHAR a3,
	PUCHAR a4);
typedef enum _CMD {
	CMD_TEST,
	CMD_GET_SYSERRCODE,
	CMD_GET_MODULE_BASE,
	CMD_READ_VIRBYPASS,
	CMD_WRITE_VIRBYPASS,
	CMD_REMOTE_CALL,
	CMD_GET_PROC_ADDR,
	CMD_ALLOC_MEM,
	CMD_FREE_MEM,
	CMD_FIND_SIGN,
	CMD_HIDE_PROC,
	CMD_DEL_FILE,
	CMD_MEM_QUERY,
	CMD_GET_PID_BYNAME,


	CMD_KM_INSTALL,
	CMD_KM_KEY,
	CMD_KM_MOUSE,


	CMD_DR_INSTALL,
	CMD_DR_BINDWINDOWS,
	CMD_DR_DRAW_TXT,
	CMD_DR_DRAW_BORDERBOX,
	CMD_DR_DRAW_FILLRECT,
	CMD_DR_END,

	DWM_INIT_STEP0,
	DWM_INIT_STEP1,
	DWM_INIT_STEP2

};

typedef struct _ComPackage {
	ULONG64 sign;
	ULONG64 cmd;
	ULONG64 errCode;
	ULONG64 data1;
	ULONG64 data2;
	ULONG64 data3;
	ULONG64 data4;
	ULONG64 data5;
	ULONG64 data6;
	ULONG64 data7;
	ULONG64 data8;
}ComPackage, * PComPackage;

SKSTATUS ComSend(ULONG64 cmd, ULONG64 data1, ULONG64 data2, ULONG64 data3,
	ULONG64 data4, ULONG64 data5, ULONG64 data6, ULONG64 data7, ULONG64 data8);

SKSTATUS InitCom();
SKSTATUS InitComWin7();
SKSTATUS InitComWin10();
VOID ComSendWin7(PComPackage pkg);
VOID ComSendWin10(PComPackage pkg);
SKSTATUS GetModuleBase(ULONG64 pid, char* name, PULONG64 base);
SKSTATUS GetPidByName(char* name, ULONG64 ppid);
#define _IS_WOW64 sizeof(char*) == 4

SKSTATUS ReadProcMemory(ULONG64 pid, ULONG64 dst, ULONG64 buf, ULONG64 len, ULONG64 way);
SKSTATUS WriteProcMemory(ULONG64 pid, ULONG64 dst, ULONG64 buf, ULONG64 len, ULONG64 way);
SKSTATUS QueryMemInfo(ULONG64 pid, ULONG64 addr, ULONG64 pointer);
SKSTATUS RemoteCall(ULONG64 pid, ULONG64 tid, ULONG64 mode, ULONG64 shellcode, ULONG64 len);
SKSTATUS GetExpFuncAddrOfProcModule(ULONG64 pid, char* moduleName, char* funcName, ULONG64 expAddr);
SKSTATUS TestCom();
SKSTATUS LoadDriver(const char * driverUrl,ULONG64 commKey);
SKSTATUS AllocaProcMemory(ULONG64 pid, ULONG64 size, ULONG64 base);
SKSTATUS FreeProcMemory(ULONG64 pid, ULONG64 base);
SKSTATUS FindProcSignCode(ULONG64 pid, ULONG64 base, ULONG64 sign, ULONG64 len, ULONG64 retBase);
SKSTATUS HideProcessByPid(ULONG64 pid, ULONG64 dstPid, ULONG64 needClearObj);
SKSTATUS DeleteFileForce(char* path);
SKSTATUS GetLastSysErrCode(ULONG64 codeOut);

USHORT GetOsBuildNumber();