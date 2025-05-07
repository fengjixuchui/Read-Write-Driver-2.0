#pragma once
#include <ntifs.h>
#include "global.h"
#include "krtools.h"
#include "prockr.h"


PUCHAR VtGetAddrVadOfCutProc(ULONG64 addr);
ULONG64 VtGetAddrCommitChargeInVad(PUCHAR vad);
ULONG64 VtGetAddrTypeInVad(PUCHAR vad);
ULONG64 VtGetAddrStartPageInVad(PUCHAR vad);
