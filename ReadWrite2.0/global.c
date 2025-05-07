#pragma once
#include "global.h"

RTL_OSVERSIONINFOEXW VER_INFO = {0};



void MyAttack(ULONG64 newcr3,MY_ATTACH_OBJ* obj) {
	_disable();
	obj->cr4 = __readcr4();
	__writecr4(obj->cr4 & (~0x800080L));
	obj->cr0 = __readcr0();
	__writecr0(obj->cr0 & (~0x10000L));
	obj->cr3 = __readcr3();
	__writecr3(newcr3);
}
void MyDeatch(MY_ATTACH_OBJ * obj) {
	__writecr3(obj->cr3);
	__writecr0(obj->cr0);
	__writecr4(obj->cr4);
	obj->cr3 = 0;
	obj->cr0 = 0;
	obj->cr4 = 0;
	_enable();
}

SKSTATUS GlbGetBuildNumber(PULONG buildNumber) {
	if (VER_INFO.dwBuildNumber == NULL)
	{
		VER_INFO.dwOSVersionInfoSize = sizeof(RTL_OSVERSIONINFOEXW);
		NTSTATUS ret = RtlGetVersion(&VER_INFO);
		if (!NT_SUCCESS(ret))
		{
			*buildNumber = NULL;
			VER_INFO.dwBuildNumber = NULL;
			return SK_FAILED_GETVERSION;
		}
	}
	*buildNumber = VER_INFO.dwBuildNumber;
	return SK_SUCCESS;
}

/*内核Sleep*/
VOID CpMySleepSec(LONGLONG time) {
	LARGE_INTEGER li = { 0 };	//时长结构。
	li.QuadPart = -10000 * 1000 * time;	//时间单位 负数代表相对时间  正数代表绝对时间。 5000代表5秒。 
	KeDelayExecutionThread(KernelMode, FALSE, &li);
}
VOID CpMySleepMin(LONGLONG time) {
	LARGE_INTEGER li = { 0 };	//时长结构。
	li.QuadPart = -10000 * time;	//时间单位 负数代表相对时间  正数代表绝对时间。 5000代表5秒。 
	KeDelayExecutionThread(KernelMode, FALSE, &li);
}

//FORCEINLINE
BOOLEAN
RemoveEntryList32(
	_In_ PLIST_ENTRY32 Entry
)

{

	PLIST_ENTRY32 PrevEntry;
	PLIST_ENTRY32 NextEntry;

	NextEntry = Entry->Flink;
	PrevEntry = Entry->Blink;
	if (!NextEntry || !PrevEntry)
	{
		return FALSE;
	}
	//if ((NextEntry->Blink != Entry) || (PrevEntry->Flink != Entry)) {
	//	FatalListEntryError((PVOID)PrevEntry,
	//		(PVOID)Entry,
	//		(PVOID)NextEntry);
	//}

	PrevEntry->Flink = NextEntry;
	NextEntry->Blink = PrevEntry;
	return (BOOLEAN)(PrevEntry == NextEntry);
}

//FORCEINLINE
BOOLEAN
RemoveEntryList64(
	_In_ PLIST_ENTRY Entry
)

{

	PLIST_ENTRY PrevEntry;
	PLIST_ENTRY NextEntry;

	NextEntry = Entry->Flink;
	PrevEntry = Entry->Blink;
	if (!NextEntry || !PrevEntry)
	{
		return FALSE;
	}
	//if ((NextEntry->Blink != Entry) || (PrevEntry->Flink != Entry)) {
	//	FatalListEntryError((PVOID)PrevEntry,
	//		(PVOID)Entry,
	//		(PVOID)NextEntry);
	//}

	PrevEntry->Flink = NextEntry;
	NextEntry->Blink = PrevEntry;
	return (BOOLEAN)(PrevEntry == NextEntry);
}