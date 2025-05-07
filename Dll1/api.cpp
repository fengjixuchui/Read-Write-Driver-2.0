#pragma once
#include "hwd.h"
#include "api.h"
#include "dwmDraw.hpp"

//EXTERN_C NTSTATUS WINAPI Api_ComSend(ULONG64 cmd, ULONG64 inData, ULONG64 inLen, ULONG64 outData, ULONG64 outLen) {
//	return ComSend(cmd,  inData,  inLen,  outData,  outLen);
//}
//EXTERN_C BOOLEAN WINAPI Api_InitCom() {
//	return InitCom();
//}



EXTERN_C SKSTATUS WINAPI Api_GetModuleBase(ULONG64 pid, char* name,PULONG64 base) {
	return GetModuleBase( pid,  name,  base);
}
EXTERN_C SKSTATUS WINAPI Api_GetPidByName(char* name, ULONG64 ppid) {
	return GetPidByName(name, ppid);
}
EXTERN_C SKSTATUS WINAPI Api_ReadProcMemory(ULONG64 pid, ULONG64 dst, ULONG64 buf, ULONG64 len, ULONG64 way) {
	return ReadProcMemory( pid,  dst,  buf,  len,  way);
}
EXTERN_C SKSTATUS WINAPI Api_WriteProcMemory(ULONG64 pid, ULONG64 dst, ULONG64 buf, ULONG64 len, ULONG64 way) {
	return WriteProcMemory( pid,  dst,  buf,  len,way);
}
EXTERN_C SKSTATUS WINAPI Api_QueryMemInfo(ULONG64 pid,ULONG64 addr,ULONG64 pointer) {
	return QueryMemInfo(pid, addr, pointer);
}
EXTERN_C SKSTATUS WINAPI Api_RemoteCall(ULONG64 pid, ULONG64 tid, ULONG64 mode, ULONG64 shellcode, ULONG64 len) {
	return RemoteCall( pid,  tid,  mode,  shellcode,  len);
}
EXTERN_C SKSTATUS WINAPI Api_GetExpFuncAddrOfProcModule(ULONG64 pid, char* moduleName, char* funcName, ULONG64 expAddr) {
	return GetExpFuncAddrOfProcModule(pid, moduleName, funcName, expAddr);
}
EXTERN_C SKSTATUS WINAPI Api_AllocaProcMemory(ULONG64 pid, ULONG64 size, ULONG64 base) {
	return AllocaProcMemory(pid, size, base);
}
EXTERN_C SKSTATUS WINAPI Api_FreeProcMemory(ULONG64 pid,ULONG64 base) {
	return FreeProcMemory(pid, base);
}
EXTERN_C SKSTATUS WINAPI Api_FindProcSignCode(ULONG64 pid, ULONG64 base, ULONG64 sign, ULONG64 len, ULONG64 retBase) {
	return FindProcSignCode(pid, base,sign,len, retBase);
}
EXTERN_C SKSTATUS WINAPI Api_HideProcessByPid(ULONG64 pid,ULONG64 dstpid, ULONG64 needClearObj) {
	//bool ret = false;
	//ret = hwd_deductPoint(1, "Api_HideProcessByPid", 3600);
	//hwd_MsgError();
	return HideProcessByPid(pid, dstpid, needClearObj);
}
EXTERN_C SKSTATUS WINAPI Api_DeleteFileForce(char* path) {
	//bool ret = false;
	//ret = hwd_deductPoint(1, "Api_DeleteFileForce", 3600);
	//hwd_MsgError();
	return DeleteFileForce(path);
}
EXTERN_C SKSTATUS WINAPI Api_LoadDriver(const char * key) {
	hwd_init("http://103.71.50.194/ac.php?s=498e688089a5f1100ad9a42ce30c02a1",
		"b4d2561f56556d4d179361a069bd89a9",
		"dbc5fb6c-b7f8-4c69-9025-7a2860be561a",
		"S5vvXbksFNSPsAzcvnZqQ9ufrQHUTF3d",
		"[data][key][data]deffef[key]fegegeg[data][key]",
		"[data]UEzgB[key]jlyDQ[key]fegegeg[data][key]",
		"b31284750be8ee9872b5c95e5a63f863"
	);
	hwd_MsgError();


	string keyStr = key;
	hwd_login(keyStr, keyStr, "", "3.3");
	hwd_MsgError();


	string commKeyStr = hwd_getSoftPara("comm_key");
	hwd_MsgError();
	string urlStr = hwd_getSoftPara("driver_url");
	hwd_MsgError();

	char pointStr[500] = { 0 };
	sprintf(pointStr, "MCID:%s====USER:%s", hwd_getMachineCode().c_str(), "Api_LoadDriver");
	hwd_deductPoint(3, pointStr, 3600);
	hwd_MsgError();

	//hwd_heartbeat(200, "");
	//hwd_MsgError();

	return LoadDriver(urlStr.c_str(),atoll(commKeyStr.c_str()));
}
EXTERN_C SKSTATUS WINAPI Api_GetLastSysErrCode(ULONG64 codeOut) {
	return GetLastSysErrCode(codeOut);
}



EXTERN_C SKSTATUS WINAPI Api_KmInstall() {
	bool ret = false;
	char pointStr[500] = { 0 };
	sprintf(pointStr, "MCID:%s====USER:%s", hwd_getMachineCode().c_str(), "Api_KmInstall");
	hwd_deductPoint(1, pointStr, 3600);
	hwd_MsgError();
	return KmInstall();
}

EXTERN_C SKSTATUS WINAPI Api_KmKeyDown(ULONG64 kVal) {
	return KmKeyDown(kVal);
}
EXTERN_C SKSTATUS WINAPI Api_KmKeyUp(ULONG64 kVal) {
	return KmKeyUp(kVal);
}

EXTERN_C SKSTATUS WINAPI Api_KmMouseLeftDown() {
	return KmMouseLeftDown();
}

EXTERN_C SKSTATUS WINAPI Api_KmMouseLeftUp() {
	return KmMouseLeftUp();
}

EXTERN_C SKSTATUS WINAPI Api_KmMouseRightDown() {
	return KmMouseRightDown();
}

EXTERN_C SKSTATUS WINAPI Api_KmMouseRightUp() {
	return KmMouseRightUp();
}

EXTERN_C SKSTATUS WINAPI Api_KmMouseMoveRelative(LONG64 dx, LONG64 dy) {
	return KmMouseMoveRelative(dx,dy);
}

EXTERN_C SKSTATUS WINAPI Api_KmMouseMoveTo(LONG64 dx, LONG64 dy) {
	return KmMouseMoveTo(dx,dy);
}


EXTERN_C SKSTATUS WINAPI Api_DwmDrInit() {
	bool ret = false;
	char pointStr[500] = { 0 };
	sprintf(pointStr, "MCID:%s====USER:%s", hwd_getMachineCode().c_str(), "Api_DwmDrInit");
	hwd_deductPoint(1, pointStr, 3600);
	hwd_MsgError();
	return DwmDr_Init();
}
EXTERN_C BOOL WINAPI Api_DwmDrStartDraw() {
	return DwmDr_StartDraw();
}
EXTERN_C void WINAPI Api_DwmDrEndDraw() {
	return DwmDr_EndDraw();
}
EXTERN_C void WINAPI Api_DwmDrDrawBox(float X, float Y, float W, float H, float R, float G, float B, float A, int T) {
	return DwmDr_DrawBox(X, Y, W, H, R, G, B, A, T);
}
EXTERN_C void WINAPI Api_DwmDrDrawLine(float X1, float Y1, float X2, float Y2, float R, float G, float B, float A, int T) {
	return DwmDr_DrawLine(X1, Y1, X2, Y2, R, G, B, A, T);
}
EXTERN_C void WINAPI Api_DwmDrDrawString(float X, float Y, float R, float G, float B, float A, const char* text) {
	return DwmDr_DrawString(X, Y, R, G, B, A, text);
}
EXTERN_C void WINAPI Api_DwmDrDrawCircle(float X, float Y, float radius, float R, float G, float B, float A, int num_segments, float W) {
	return DwmDr_DrawCircle(X, Y, radius, R, G, B, A, num_segments, W);
}
EXTERN_C void WINAPI Api_DwmDrDrawStrokeText(float X, float Y, float R, float G, float B, float A, float R1, float G1, float B1, float A1, const char* text) {
	return DwmDr_DrawStrokeText(X, Y, R, G, B, A, R1, G1, B1, A1, text);
}
EXTERN_C void WINAPI Api_DwmDrDrawCornerframe(float x, float y, float w, float h, float R, float G, float B, float A, int T) {
	return DwmDr_DrawCornerframe(x, y, w, h, R, G, B, A, T);
}

