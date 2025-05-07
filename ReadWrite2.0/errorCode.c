#pragma once
#include "errorCode.h"

NTSTATUS LAST_SYS_ERRCODE = STATUS_SUCCESS;

NTSTATUS EcGetLastSysErrCode() {
	return LAST_SYS_ERRCODE;
}

VOID EcUpdateLastSysErrCode(NTSTATUS code) {
	LAST_SYS_ERRCODE = code;
}