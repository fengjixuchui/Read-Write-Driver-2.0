#pragma once
#include "comm_win7.h"
#include "comm.h"

AttributeInformationCallBack oldExpDisQueryAttributeInformation = 0;
AttributeInformationCallBack oldExpDisSetAttributeInformation = 0;

SKSTATUS ComInitWin7Commmunication() {
	NTSTATUS ret = NULL;
	UNICODE_STRING Str_ExRegisterAttributeInformationCallback = { 0 };
	RtlInitUnicodeString(&Str_ExRegisterAttributeInformationCallback, L"ExRegisterAttributeInformationCallback");
	PUCHAR pExRegisterAttributeInformationCallback = (PUCHAR)MmGetSystemRoutineAddress(&Str_ExRegisterAttributeInformationCallback);
	if (!pExRegisterAttributeInformationCallback)
	{
		return SK_FAILED_INITCOM_NULLFUNC_ExRegisterAttributeInformationCallback;
	}

	PULONG64 pExpDisQueryAttributeInformation = (PULONG64)(*(PULONG)(pExRegisterAttributeInformationCallback + 16) + pExRegisterAttributeInformationCallback + 16 + 4);
	oldExpDisQueryAttributeInformation = (AttributeInformationCallBack)pExpDisQueryAttributeInformation[0];
	oldExpDisSetAttributeInformation = (AttributeInformationCallBack)pExpDisQueryAttributeInformation[1];
	pExpDisQueryAttributeInformation[0] = 0;
	pExpDisQueryAttributeInformation[1] = 0;
	funcExRegisterAttributeInformationCallback ExRegisterAttributeInformationCallback = (funcExRegisterAttributeInformationCallback)pExRegisterAttributeInformationCallback;
	RegisterAttributeInformationCallback callBack = { 0 };
	callBack.ExpDisQueryAttributeInformation = CallBack_ExpDisQueryAttributeInformation;
	callBack.ExpDisSetAttributeInformation = CallBack_ExpDisSetAttributeInformation;
	ret = ExRegisterAttributeInformationCallback(&callBack);
	if (!NT_SUCCESS(ret))
	{
		return SK_FAILED_INITCOM_ADDCALLBACK;
	}
	return SK_SUCCESS;
}

NTSTATUS  CallBack_ExpDisQueryAttributeInformation(ULONG64 handle, ULONG64 param) {
	if (ComHandlePackage((PComPackage)param) && oldExpDisQueryAttributeInformation)
	{
		return oldExpDisQueryAttributeInformation(handle, param);
	}
	return STATUS_SUCCESS;
}
NTSTATUS  CallBack_ExpDisSetAttributeInformation(ULONG64 handle, ULONG64 param) {
	if (ComHandlePackage((PComPackage)param) && oldExpDisSetAttributeInformation)
	{
		return oldExpDisSetAttributeInformation(handle, param);
	}
	return STATUS_SUCCESS;
}
