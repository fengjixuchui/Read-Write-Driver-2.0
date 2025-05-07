#pragma once
#include "krtools_signcode.h"


//PUCHAR GetNtQueryVirtualMemorySignCode() {
//	SKSTATUS ret = NULL;
//	ULONG64 buildNum = NULL;
//	ret = GlbGetBuildNumber(&buildNum);
//	if (ret != SK_SUCCESS) return "null code";
//	if (buildNum == 7600 || buildNum == 7601) {
//		//PS
//		return "48895C24??48897424??48894C24??5741544155415641574881EC????????4D8B??458BF8488BFA4C8BF1";
//	}
//	else
//	{
//		//NTÖÐ¼ä
//		return "4883EC??488B4424??C74424??02??????48894424??488B4424??48894424??E8????????4883C4??C3";
//	}
//}
