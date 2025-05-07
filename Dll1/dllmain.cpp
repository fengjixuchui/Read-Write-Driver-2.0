// dllmain.cpp : 定义 DLL 应用程序的入口点。
#pragma once
#include "hwd.h"
#include "comm.h"
#include "km.h"
#include "api.h"
#include "dwmDraw.hpp"
#include <CommCtrl.h>


int main(HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR     lpCmdLine,
    int       nShowCmd)
{
    ULONG32 ret = Api_LoadDriver("skK9vFntSMAnWVAsFxb0ciM3xV1lZPxOSsWhW7uYJQbwIzL2R9");
    printf("Api_LoadDriver: 0x%08X\r\n",ret);
    
    ret = Api_KmInstall();
    printf("Api_KmInstall: 0x%08X\r\n", ret);
    
    ret = Api_DwmDrInit();
    printf("Api_DwmDrInit: 0x%08X\r\n", ret);
    
    while (true)
    {
        Api_DwmDrStartDraw();
        Api_DwmDrDrawBox(100,100,100,200,0,255,0,1.0,4);
        Api_DwmDrEndDraw();
    }
    
    system("pause");
	return 0;
}


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

