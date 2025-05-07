#pragma once
#include <ntifs.h>
#include "global.h"


SKSTATUS ComInitWin7Commmunication();
NTSTATUS  CallBack_ExpDisQueryAttributeInformation(ULONG64 handle, ULONG64 param);
NTSTATUS  CallBack_ExpDisSetAttributeInformation(ULONG64 handle, ULONG64 param);


typedef NTSTATUS(NTAPI* AttributeInformationCallBack)(ULONG64 handle, ULONG64 param);
typedef struct _RegisterAttributeInformationCallback {
	AttributeInformationCallBack ExpDisQueryAttributeInformation;
	AttributeInformationCallBack ExpDisSetAttributeInformation;
}RegisterAttributeInformationCallback, * PRegisterAttributeInformationCallback;
typedef NTSTATUS(NTAPI* funcExRegisterAttributeInformationCallback)(PRegisterAttributeInformationCallback callBack);
