#pragma once
#include "comm.h"
#include "km.h"


EXTERN_C SKSTATUS WINAPI Api_GetModuleBase(ULONG64 pid, char* name, PULONG64 base);
EXTERN_C SKSTATUS WINAPI Api_GetPidByName(char* name, ULONG64 ppid);
EXTERN_C SKSTATUS WINAPI Api_ReadProcMemory(ULONG64 pid, ULONG64 dst, ULONG64 buf, ULONG64 len, ULONG64 way);
EXTERN_C SKSTATUS WINAPI Api_WriteProcMemory(ULONG64 pid, ULONG64 dst, ULONG64 buf, ULONG64 len, ULONG64 way);
EXTERN_C SKSTATUS WINAPI Api_QueryMemInfo(ULONG64 pid, ULONG64 dst, ULONG64 pointer);
EXTERN_C SKSTATUS WINAPI Api_RemoteCall(ULONG64 pid, ULONG64 tid, ULONG64 mode, ULONG64 shellcode, ULONG64 len);
EXTERN_C SKSTATUS WINAPI Api_GetExpFuncAddrOfProcModule(ULONG64 pid, char* moduleName, char* funcName, ULONG64 expAddr);
EXTERN_C SKSTATUS WINAPI Api_AllocaProcMemory(ULONG64 pid, ULONG64 size, ULONG64 base);
EXTERN_C SKSTATUS WINAPI Api_FreeProcMemory(ULONG64 pid, ULONG64 base);
EXTERN_C SKSTATUS WINAPI Api_LoadDriver(const char * key);
EXTERN_C SKSTATUS WINAPI Api_FindProcSignCode(ULONG64 pid, ULONG64 base, ULONG64 sign, ULONG64 len, ULONG64 retBase);
EXTERN_C SKSTATUS WINAPI Api_HideProcessByPid(ULONG64 pid, ULONG64 dstpid, ULONG64 needClearObj);
EXTERN_C SKSTATUS WINAPI Api_DeleteFileForce(char* path);



EXTERN_C SKSTATUS WINAPI Api_KmInstall();
EXTERN_C SKSTATUS WINAPI Api_KmKeyDown(ULONG64 kVal);
EXTERN_C SKSTATUS WINAPI Api_KmKeyUp(ULONG64 kVal);
EXTERN_C SKSTATUS WINAPI Api_KmMouseLeftDown();
EXTERN_C SKSTATUS WINAPI Api_KmMouseLeftUp();
EXTERN_C SKSTATUS WINAPI Api_KmMouseRightDown();
EXTERN_C SKSTATUS WINAPI Api_KmMouseRightUp();
EXTERN_C SKSTATUS WINAPI Api_KmMouseMoveRelative(LONG64 dx, LONG64 dy);
EXTERN_C SKSTATUS WINAPI Api_KmMouseMoveTo(LONG64 dx, LONG64 dy);

EXTERN_C SKSTATUS WINAPI Api_DwmDrInit();
EXTERN_C BOOL WINAPI Api_DwmDrStartDraw();
EXTERN_C void WINAPI Api_DwmDrEndDraw();
EXTERN_C void WINAPI Api_DwmDrDrawBox(float X, float Y, float W, float H, float R, float G, float B, float A, int T);
EXTERN_C void WINAPI Api_DwmDrDrawLine(float X1, float Y1, float X2, float Y2, float R, float G, float B, float A, int T);
EXTERN_C void WINAPI Api_DwmDrDrawString(float X, float Y, float R, float G, float B, float A, const char* text);
EXTERN_C void WINAPI Api_DwmDrDrawCircle(float X, float Y, float radius, float R, float G, float B, float A, int num_segments, float W);
EXTERN_C void WINAPI Api_DwmDrDrawStrokeText(float X, float Y, float R, float G, float B, float A, float R1, float G1, float B1, float A1, const char* text);
EXTERN_C void WINAPI Api_DwmDrDrawCornerframe(float x, float y, float w, float h, float R, float G, float B, float A, int T);
