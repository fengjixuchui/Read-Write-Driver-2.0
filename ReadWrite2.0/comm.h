#pragma once
#include <ntifs.h>
#include "global.h"
#include "comm_win7.h"
#include "comm_win10.h"
#include "comm_win11.h"
#include "prockr.h"
#include "mmtool.h"
#include "filetool.h"
#include "kmclass.h"
#include "draw.hpp"
#include "comm_signcode.h"
#include "dwmhook.h"

typedef struct _ComPackage {
	ULONG64 sign;
	ULONG64 cmd;
	ULONG64 errCode;
	ULONG64 data1;
	ULONG64 data2;
	ULONG64 data3;
	ULONG64 data4;
	ULONG64 data5;
	ULONG64 data6;
	ULONG64 data7;
	ULONG64 data8;
}ComPackage, * PComPackage;

SKSTATUS ComInitCommmunication();
SKSTATUS ComUnloadCommmunication();
SKSTATUS ComHandlePackage(PComPackage pack);



typedef enum _CMD {
	CMD_TEST,
	CMD_GET_SYSERRCODE,
	CMD_GET_MODULE_BASE,
	CMD_READ_VIRBYPASS,
	CMD_WRITE_VIRBYPASS,
	CMD_REMOTE_CALL,
	CMD_GET_PROC_ADDR,
	CMD_ALLOC_MEM,
	CMD_FREE_MEM,
	CMD_FIND_SIGN,
	CMD_HIDE_PROC,
	CMD_DEL_FILE,
	CMD_MEM_QUERY,
	CMD_GET_PID_BYNAME,


	CMD_KM_INSTALL,
	CMD_KM_KEY,
	CMD_KM_MOUSE,


	CMD_DR_INSTALL,
	CMD_DR_BINDWINDOWS,
	CMD_DR_DRAW_TXT,
	CMD_DR_DRAW_BORDERBOX,
	CMD_DR_DRAW_FILLRECT,
	CMD_DR_END,



	DWM_INIT_STEP0,
	DWM_INIT_STEP1,
	DWM_INIT_STEP2


};