#pragma once
#include <ntifs.h>
#include "global.h"
#include "krtools.h"


typedef NTSTATUS(NTAPI* Win10_off_140401C70CallBack)(PUCHAR p1, PUCHAR p2, PUCHAR p3);

SKSTATUS ComInitWin10Commmunication();
