#pragma once
#include "draw.hpp"



EXTERN_C ULONG64 jmp_back_original_DxgkSubmitCommand_backup = NULL;

//dxgkrnl!DxgkSubmitCommand
//静态文件：57 48 83 ec ?? 48 8b ?? 48 ff 15 ?? ?? ?? ?? 0f 1f 44 00 00 48 8b ?? 48 ff 15 ?? ?? ?? ?? 0f 1f 44 00 00
//内存数据：4889??????574883EC??488B??4C8B15????????E8????????488B??4C8B15????????E8????????488B
//新 DxgkSubmitCommandInternal
ULONG64 SubmitCommandAddr = NULL;
SKSTATUS DrSetDrawHookWin10_SubmitCommand() {
	SKSTATUS ret = NULL;
	ULONG64 dxgkrnlBase = NULL;
	ULONG64 dxgkrnlSize = NULL;
	ULONG pageSize = NULL;
	ULONG64 pageBase = NULL;
	ULONG pageOffset = NULL;
	ret = KrGetKernelModuleBase((PUCHAR)"dxgkrnl.sys",&dxgkrnlSize,&dxgkrnlBase );
	if (ret != SK_SUCCESS) return ret;
	ret = PeGetSectionOffsetByName((PUCHAR)dxgkrnlBase,(PUCHAR)"PAGE",&pageSize,&pageOffset);
	if (ret != SK_SUCCESS) return ret;
	ret = MmFindAddrBySignCode((PUCHAR)(dxgkrnlBase + pageOffset),(PUCHAR)"48895C24??48897424??5741544155415641574881EC????????488B05????????4833C448898424??????????8B??48899424", pageSize*0x10,&SubmitCommandAddr);
	if (ret != SK_SUCCESS) return ret;

	UCHAR buf[12] = { 0x48,0xB8,0,0,0,0,0,0,0,0,0xFF,0xE0 };
	UCHAR backup[13] = { 0 };
	*(PULONG64)(&buf[2]) = (ULONG64)SetHookGeneral_backup;
	memcpy(&backup[0], (PUCHAR)(SubmitCommandAddr), 13);
	ret = MmWriteKernelMemory((ULONG64)SetHookGeneral_backup + 65, &backup[0], 13);
	if (ret != SK_SUCCESS) return ret;
	//memcpy((PUCHAR)SetHookGeneral + 47, &backup[0], 13);
	ret = MmWriteKernelMemory(SubmitCommandAddr, buf,12);
	jmp_back_original_DxgkSubmitCommand_backup = SubmitCommandAddr + 13;
	return ret;
}


EXTERN_C ULONG64 jmp_back_original_DxgkSubmitCommand = NULL;
ULONG64 renderAddr = NULL;
//DxgkRender  
SKSTATUS DrSetDrawHookWin10_DxgkRender() {
	SKSTATUS ret = NULL;
	ULONG64 temp = NULL;
	ULONG64 dxgkrnlBase = NULL;
	ULONG64 dxgkrnlSize = NULL;
	ULONG pageSize = NULL;
	ULONG64 pageBase = NULL;
	ULONG pageOffset = NULL;
	ULONG64 findTemp = NULL;
	ret = KrGetKernelModuleBase((PUCHAR)"dxgkrnl.sys", &dxgkrnlSize, &dxgkrnlBase);
	if (ret != SK_SUCCESS) return ret;
	ret = PeGetExportFuncAddr64((PUCHAR)dxgkrnlBase,(PUCHAR)"NtGdiDdDDIRender",&renderAddr);
	if (renderAddr)
	{
		findTemp = renderAddr;
	}
	else
	{
		findTemp = dxgkrnlBase + pageOffset;
	}
	ret = PeGetSectionOffsetByName((PUCHAR)dxgkrnlBase, (PUCHAR)"PAGE", &pageSize, &pageOffset);
	if (ret != SK_SUCCESS) return ret;
	ret = MmFindAddrBySignCode((PUCHAR)(findTemp), (PUCHAR)"41??41??41??4881EC????????488B05????????48????48898424????????48????48898C24", pageSize * 0x10, &renderAddr);
	if (ret != SK_SUCCESS) return ret;
	temp = renderAddr;

	UCHAR buf[12] = { 0x48,0xB8,0,0,0,0,0,0,0,0,0xFF,0xE0 };
	UCHAR backup[13] = { 0 };
	*(PULONG64)(&buf[2]) = (ULONG64)SetHookGeneral;
	memcpy(&backup[0], (PUCHAR)(temp), 13);
	ret = MmWriteKernelMemory((ULONG64)SetHookGeneral + 65, &backup[0], 13);
	if (ret != SK_SUCCESS) return ret;
	//memcpy((PUCHAR)SetHookGeneral + 47, &backup[0], 13);
	ret = MmWriteKernelMemory(temp, buf, 12);
	jmp_back_original_DxgkSubmitCommand = temp + 13;
	return ret;
}

EXTERN_C ULONG64 jmp_back_original_present = NULL;
ULONG64 PresentAddr = NULL;
//DxgkPresent
SKSTATUS DrSetDrawHookWin10_DxgkPresent() {
	SKSTATUS ret = NULL;
	ULONG64 dxgkrnlBase = NULL;
	ULONG64 dxgkrnlSize = NULL;
	ULONG pageSize = NULL;
	ULONG64 pageBase = NULL;
	ULONG pageOffset = NULL;
	ret = KrGetKernelModuleBase((PUCHAR)"dxgkrnl.sys", &dxgkrnlSize, &dxgkrnlBase);
	if (ret != SK_SUCCESS) return ret;
	ret = PeGetSectionOffsetByName((PUCHAR)dxgkrnlBase, (PUCHAR)".text", &pageSize, &pageOffset);
	if (ret != SK_SUCCESS) return ret;

	ret = MmFindAddrBySignCode((PUCHAR)(dxgkrnlBase + pageOffset), (PUCHAR)"48895C24??48897424??5741544155415641574881EC????????488B05????????4833C448898424????????4C8B??48898C24????????C78424", pageSize * 0x10, &PresentAddr);
	if (ret == SK_SUCCESS)
	{
		PresentAddr += 10;
	}
	else
	{
		//17134.407
		ret = MmFindAddrBySignCode((PUCHAR)(dxgkrnlBase + pageOffset), (PUCHAR)"48894C24??53565741544155415641574881EC????????448BF24C8BF9C78424", pageSize * 0x10, &PresentAddr);
		if (ret != SK_SUCCESS) return ret;
		PresentAddr += 7;
	}


	//ret = MmFindAddrBySignCode((PUCHAR)(dxgkrnlBase + pageOffset), (PUCHAR)"4883EC??488B05????????4885C00F84????????4883C4??C3", pageSize * 0x10, &PresentAddr);
	//if (ret != SK_SUCCESS) return ret;

	UCHAR buf[12] = { 0x48,0xB8,0,0,0,0,0,0,0,0,0xFF,0xE0 };
	UCHAR backup[16] = { 0 };
	*(PULONG64)(&buf[2]) = (ULONG64)SetHookGeneral_present;
	memcpy(&backup[0], (PUCHAR)(PresentAddr), 16);
	ret = MmWriteKernelMemory((ULONG64)SetHookGeneral_present + 65, &backup[0], 16);
	if (ret != SK_SUCCESS) return ret;
	ret = MmWriteKernelMemory(PresentAddr, buf, 12);
	jmp_back_original_present = PresentAddr + 16;
	return ret;
}




EXTERN_C ULONG64 jmp_back_DxgkRender_win7 = NULL;
//DxgkRender
SKSTATUS DrSetDrawHookWin7() {
	SKSTATUS ret = NULL;
	ULONG64 temp = NULL;
	ULONG64 dxgkrnlBase = NULL;
	ULONG64 dxgkrnlSize = NULL;
	ULONG64 renderAddr = NULL;
	ret = KrGetKernelModuleBase((PUCHAR)"dxgkrnl.sys", &dxgkrnlSize, &dxgkrnlBase);
	if (ret != SK_SUCCESS) return ret;
	ret = MmFindAddrBySignCode((PUCHAR)dxgkrnlBase,(PUCHAR)"48894C24??5356574154415641574881EC????????488BD9C74424??????????BE????????408435????????74??B2??B9",dxgkrnlSize,&renderAddr);
	if (ret != SK_SUCCESS) return ret;
	temp = renderAddr;

	UCHAR buf[12] = { 0x48,0xB8,0,0,0,0,0,0,0,0,0xFF,0xE0 };
	UCHAR backup[12] = { 0 };
	*(PULONG64)(&buf[2]) = (ULONG64)SetHookGeneral_renderwin7;
	memcpy(&backup[0], (PUCHAR)(temp), 12);
	ret = MmWriteKernelMemory((ULONG64)SetHookGeneral_renderwin7 + 65, &backup[0], 12);
	if (ret != SK_SUCCESS) return ret;
	//memcpy((PUCHAR)SetHookGeneral + 47, &backup[0], 13);
	ret = MmWriteKernelMemory(temp, buf, 12);
	jmp_back_DxgkRender_win7 = temp + 12;
	return ret;
}



