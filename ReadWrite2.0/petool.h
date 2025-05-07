#pragma once
#include <ntifs.h>
#include "global.h"

EXTERN_C SKSTATUS PeGetExportFuncAddr64(PUCHAR base, PUCHAR funcName, PULONG64 ExpAddr);
EXTERN_C SKSTATUS PeGetExportFuncAddr32(PUCHAR base, PUCHAR funcName, PULONG64 ExpAddr);
