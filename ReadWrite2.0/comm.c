#pragma once
#include "comm.h"






ULONG64 _PACKAGE_SIGN = 0x65083911;

SKSTATUS ComInitCommmunication() {
	ULONG buildNum = NULL;
	SKSTATUS ret = NULL;
	ret = GlbGetBuildNumber(&buildNum);
	if (ret != SK_SUCCESS)
	{
		return SK_FAILED_INITCOM_BADVERSION;
	}
	if (buildNum == 7601 || buildNum == 7600)
	{
		return ComInitWin7Commmunication();
	}
	else if (buildNum == 22000) {
		return ComInitWin11Commmunication();
	}
	else
	{
		return ComInitWin10Commmunication();
	}
}







SKSTATUS ComHandlePackage(PComPackage pack) {
	if (!MmIsAddressValid(pack) || pack->sign != _PACKAGE_SIGN)
	{
		return SK_BADPACKSIGN;
	}
	PMEMORY_BASIC_INFORMATION info = NULL;
	switch (pack->cmd)
	{
	case CMD_TEST:
		pack->data1 = SK_TESTCOM_SUCCESS;
		pack->data2 = SK_TESTCOM_SUCCESS;
		pack->data3 = SK_TESTCOM_SUCCESS;
		pack->data4 = SK_TESTCOM_SUCCESS;
		pack->data5 = SK_TESTCOM_SUCCESS;
		pack->data6 = SK_TESTCOM_SUCCESS;
		pack->data7 = SK_TESTCOM_SUCCESS;
		pack->data8 = SK_TESTCOM_SUCCESS;
		pack->errCode = SK_TESTCOM_SUCCESS;
		break;
	case CMD_GET_SYSERRCODE:
		*(SKSTATUS*)pack->data1 = EcGetLastSysErrCode();
		pack->errCode = SK_SUCCESS;
		break;
	case CMD_GET_MODULE_BASE:
		pack->errCode = KrGetProcModuleBaseByName(pack->data1,pack->data2,pack->data3);
		break;
	case CMD_GET_PID_BYNAME:
		pack->errCode = PsGetPidByName(pack->data1, pack->data2);
		break;
	case CMD_READ_VIRBYPASS:
		pack->errCode = MmReadProcessMemory_Force(pack->data1, pack->data2, pack->data3, pack->data4,pack->data5);
		break;
	case CMD_WRITE_VIRBYPASS:
		pack->errCode = MmWriteProcessMemory_Force(pack->data1, pack->data2, pack->data3, pack->data4, pack->data5);
		break;
	case CMD_MEM_QUERY:
		pack->errCode = MmQueryMemInfo(pack->data1,pack->data2, &info);
		if (!pack->data3 || !MmIsAddressValid(pack->data3) || !MmIsAddressValid(pack->data3+ sizeof(MEMORY_BASIC_INFORMATION)-1))
		{
			pack->errCode = SK_FAILED_MMTOOL_BADPARAM;
			break;
		}
		if (pack->errCode == SK_SUCCESS)
		{
			memset((PUCHAR)pack->data3, 0, 56);
			*(PULONG64)(pack->data3 + 8 * 0) = info->BaseAddress;
			*(PULONG64)(pack->data3 + 8 * 1) = info->AllocationBase;
			*(PULONG64)(pack->data3 + 8 * 2) = info->AllocationProtect;
			*(PULONG64)(pack->data3 + 8 * 3) = info->RegionSize;
			*(PULONG64)(pack->data3 + 8 * 4) = info->State;
			*(PULONG64)(pack->data3 + 8 * 5) = info->Protect;
			*(PULONG64)(pack->data3 + 8 * 6) = info->Type;
		}
		break;
	case CMD_REMOTE_CALL:
		pack->errCode = MmRemoteCallByWriteContext(pack->data1, pack->data2, pack->data3, pack->data4, pack->data5);
		break;
	case CMD_GET_PROC_ADDR:
		pack->errCode = PeGetExportFuncAddrOfProcModule(pack->data1, pack->data2, pack->data3, pack->data4);
		break;
	case CMD_ALLOC_MEM:
		pack->errCode = MmAllocateProcMemory(pack->data1, pack->data2, pack->data3);
		break;
	case CMD_FREE_MEM:
		pack->errCode = MmFreeProcMemory(pack->data1, pack->data2);
		break;
	case CMD_FIND_SIGN:
		pack->errCode = MmFindProcAddrBySignCode(pack->data1, pack->data2, pack->data3, pack->data4, pack->data5);
		break;
	case CMD_HIDE_PROC:
		pack->errCode = FcProtectProcessByGlobalHandleTable(pack->data1, pack->data2,pack->data3);
		break;
	case CMD_DEL_FILE:
		pack->errCode = FsDeleteFile(pack->data1);
		break;
	case CMD_KM_INSTALL:
		pack->errCode = KmInstall();
		break;
	case CMD_KM_KEY:
		pack->errCode = KmKeyDownUp(pack->data1);
		break;
	case CMD_KM_MOUSE:
		pack->errCode = KmMouseDownUp(pack->data1);
		break;


	case CMD_DR_INSTALL:
		pack->errCode = DrComInstallDraw();
		break;
	case CMD_DR_BINDWINDOWS:
		pack->errCode = DrComDrawBindWindow(pack->data1);
		break;
	case CMD_DR_DRAW_TXT:
		pack->errCode = DrComDrawTXT(pack->data1,pack->data2,pack->data3,pack->data4,pack->data5,pack->data6,pack->data7);
		break;
	case CMD_DR_DRAW_BORDERBOX:
		pack->errCode = DrComDrawBorderBox(pack->data1, pack->data2, pack->data3, pack->data4, pack->data5, pack->data6,pack->data7);
		break;
	case CMD_DR_DRAW_FILLRECT:
		pack->errCode = DrComDrawFillRect(pack->data1, pack->data2, pack->data3, pack->data4, pack->data5, pack->data6);
		break;
	case CMD_DR_END:
		pack->errCode = DrComEndDraw(pack->data1);
		break;





	case DWM_INIT_STEP0:
		pack->errCode = DwmHookStep0();
		break;
	case DWM_INIT_STEP1:
		pack->errCode = DwmHookStep1(pack->data1);
		break;
	case DWM_INIT_STEP2:
		pack->errCode = DwmHookStep2();
		break;







	default:
		pack->errCode = SK_FUNC_NO_USE;
		break;
	}


	return SK_SUCCESS;
}