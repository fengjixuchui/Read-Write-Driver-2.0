#pragma once
#include <ntifs.h>
#include "global.h"
#include "mmtool.h"


PUCHAR GetNtOrPsSuspendThreadSignCode();
PUCHAR GetNtOrPsResumeThreadSignCode();
PUCHAR GetExpLookupHandleTableEntrySignCode();
PUCHAR GetPspCidTableSignCode();
