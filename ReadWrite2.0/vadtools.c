#pragma once
#include "vadtools.h"


/*
win7   +18 start,  +20 end,  +28 VadFlags
win8   +18 start,  +1c end,  +28 VadFlags, +2C VadFlags1
win8.1~win11 +18 start, +1c end, +30 VadFlags,+34 VadFlags1
==================MMVAD_FLAGS===================
win7
VadFlags
	ULONGLONG CommitCharge:51;                                              //0x0
	ULONGLONG NoChange:1;                                                   //0x0
	ULONGLONG VadType:3;                                                    //0x0
	ULONGLONG MemCommit:1;                                                  //0x0
	ULONGLONG Protection:5;                                                 //0x0
	ULONGLONG Spare:2;                                                      //0x0
	ULONGLONG PrivateMemory:1;                                              //0x0
==========================================================
win8、8.1、8.1Up1、1703、1709
VadFlags
	ULONG VadType:3;                                                        //0x0
	ULONG Protection:5;                                                     //0x0
	ULONG PreferredNode:6;                                                  //0x0
	ULONG NoChange:1;                                                       //0x0
	ULONG PrivateMemory:1;                                                  //0x0
	ULONG Teb:1;                                                            //0x0
	ULONG PrivateFixup:1;                                                   //0x0
	ULONG Spare:13;                                                         //0x0
	ULONG DeleteInProgress:1;                                               //0x0
VadFlags1
	ULONG CommitCharge:31;                                                  //0x0
	ULONG MemCommit:1;                                                      //0x0
====================================================================
1803
VadFlags
	ULONG VadType:3;                                                        //0x0
	ULONG Protection:5;                                                     //0x0
	ULONG PreferredNode:6;                                                  //0x0
	ULONG PrivateMemory:1;                                                  //0x0
	ULONG PrivateFixup:1;                                                   //0x0
	ULONG Enclave:1;                                                        //0x0
	ULONG PageSize64K:1;                                                    //0x0
	ULONG RfgControlStack:1;                                                //0x0
	ULONG Spare:8;                                                          //0x0
	ULONG NoChange:1;                                                       //0x0
	ULONG ManySubsections:1;                                                //0x0
	ULONG DeleteInProgress:1;                                               //0x0
	ULONG LockContended:1;                                                  //0x0
	ULONG Lock:1;                                                           //0x0
VadFlags1
	ULONG CommitCharge:31;                                                  //0x0
	ULONG MemCommit:1;
================================================================
1809
VadFlags
	ULONG VadType:3;                                                        //0x0
	ULONG Protection:5;                                                     //0x0
	ULONG PreferredNode:6;                                                  //0x0
	ULONG PrivateMemory:1;                                                  //0x0
	ULONG PrivateFixup:1;                                                   //0x0
	ULONG Graphics:1;                                                       //0x0
	ULONG Enclave:1;                                                        //0x0
	ULONG PageSize64K:1;                                                    //0x0
	ULONG ShadowStack:1;                                                    //0x0
	ULONG Spare:6;                                                          //0x0
	ULONG HotPatchAllowed:1;                                                //0x0
	ULONG NoChange:1;                                                       //0x0
	ULONG ManySubsections:1;                                                //0x0
	ULONG DeleteInProgress:1;                                               //0x0
	ULONG LockContended:1;                                                  //0x0
	ULONG Lock:1;                                                           //0x0
VadFlags1
	ULONG CommitCharge:31;                                                  //0x0
	ULONG MemCommit:1;
================================================================
1903、1909、20H1、20H2、21H1、21H2、Win11预览版、Win11正式版
VadFlags
	ULONG Lock:1;                                                           //0x0
	ULONG LockContended:1;                                                  //0x0
	ULONG DeleteInProgress:1;                                               //0x0
	ULONG NoChange:1;                                                       //0x0
	ULONG VadType:3;                                                        //0x0
	ULONG Protection:5;                                                     //0x0
	ULONG PreferredNode:6;                                                  //0x0
	ULONG PageSize:2;                                                       //0x0
	ULONG PrivateMemory:1;                                                  //0x0
VadFlags1
	ULONG CommitCharge:31;                                                  //0x0
	ULONG MemCommit:1;

*/
typedef ULONG64(*gGetMiLocateAddress)(ULONG64 addr);
gGetMiLocateAddress pMiLocateAddress = NULL;




/*取当前进程地址VAD*/
PUCHAR VtGetAddrVadOfCutProc(ULONG64 addr) {
	if (addr < 0x1000 || addr > MmHighestUserAddress) return NULL;
	
	SKSTATUS ret = NULL;
	if (!pMiLocateAddress)
	{
		ret = KrGetMiLocateAddressAddr(&pMiLocateAddress);
		if (ret != SK_SUCCESS) { return NULL; }
	}
	ULONG64 vad = pMiLocateAddress(addr);
	return (PUCHAR)vad;
}

/*取Vad CommitCharge*/
ULONG64 VtGetAddrCommitChargeInVad(PUCHAR vad) {
	ULONG buildNum = NULL;
	SKSTATUS ret = NULL;
	ret = GlbGetBuildNumber(&buildNum);
	if (ret != SK_SUCCESS)return NULL;
	
	if (buildNum == 7600 || buildNum == 7601)
	{
		//win7
		ULONG64 flags = *(PULONG64)(vad + 0x28);
		return flags & 0x0007FFFFFFFFFFFFL;
	}
	else if (buildNum == 9200)
	{
		//win8
		ULONG32 flags = *(PULONG32)(vad + 0x2C);
		return flags & 0x7FFFFFFF;
	}
	else
	{
		//win8.1及以上
		ULONG32 flags = *(PULONG32)(vad + 0x34);
		return flags & 0x7FFFFFFF;
	}
}

/*取Vad VadType   0 私有内存  1 Map  2 MapExe  3 未知*/
ULONG64 VtGetAddrTypeInVad(PUCHAR vad) {
	ULONG buildNum = NULL;
	SKSTATUS ret = NULL;
	ret = GlbGetBuildNumber(&buildNum);
	if (ret != SK_SUCCESS)return NULL;

	UCHAR type = NULL;
	BOOLEAN isPrivate = FALSE;
	if (buildNum == 7600 || buildNum == 7601)
	{
		//win7
		ULONG64 flags = *(PULONG64)(vad + 0x28);
		type = (UCHAR)((flags & 0x0070000000000000L) >> 52);
		isPrivate = (flags & 0x8000000000000000L) == 0 ? FALSE : TRUE;
	}
	else if (buildNum == 9200)
	{
		//win8
		ULONG32 flags = *(PULONG32)(vad + 0x28);
		type = (UCHAR)(flags & 0x00000007);
		isPrivate = (flags & 0x00020000) == 0 ? FALSE : TRUE;
	}
	else if (buildNum < 17134)
	{
		//win8.1~win10 1709
		ULONG32 flags = *(PULONG32)(vad + 0x30);
		type = (UCHAR)(flags & 0x00000007);
		isPrivate = (flags & 0x00008000) == 0 ? FALSE : TRUE;
	}
	else if (buildNum < 18362)
	{
		//win10 1803~1809
		ULONG32 flags = *(PULONG32)(vad + 0x30);
		type = (UCHAR)(flags & 0x00000007);
		isPrivate = (flags & 0x00004000) == 0 ? FALSE : TRUE;
	}
	else
	{
		//win10 1903~win11
		ULONG32 flags = *(PULONG32)(vad + 0x30);
		type = (UCHAR)((flags & 0x00000070) >> 4);
		isPrivate = (flags & 0x00100000) == 0 ? FALSE : TRUE;
	}
	if (type == 0 && isPrivate)
	{
		return 0;
	}
	else if (type == 0 && !isPrivate)
	{
		return 1;
	}
	else if (type == 2)
	{
		return 2;
	}
	else
	{
		return 3;
	}

}

/*取Vad Start*/
ULONG64 VtGetAddrStartPageInVad(PUCHAR vad) {
	ULONG buildNum = NULL;
	SKSTATUS ret = NULL;
	ret = GlbGetBuildNumber(&buildNum);
	if (ret != SK_SUCCESS)return NULL;

	if (buildNum == 7600 || buildNum == 7601)
	{
		//win7
		ULONG64 startPage = *(PULONG64)(vad + 0x18);
		return startPage;
	}
	else
	{
		//win8.1及以上
		ULONG32 startPage = *(PULONG32)(vad + 0x18);
		return (ULONG64)startPage;
	}
}

