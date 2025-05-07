#pragma once
#include "stdafx.h"
#include "Dia2Dump.h"
#include <urlmon.h>
#include <wininet.h>
#pragma comment(lib, "Wininet.lib")
#include "diacreate.h"
#pragma comment(lib, "diaguids.lib")
#pragma warning (disable : 4100)



const wchar_t *g_szFilename;
IDiaDataSource *g_pDiaDataSource;
IDiaSession *g_pDiaSession;
IDiaSymbol *g_pGlobalSymbol;
DWORD g_dwMachineType = CV_CFL_80386;


bool InitSymbolService(
    IDiaDataSource** ppSource,
    IDiaSession** ppSession,
    IDiaSymbol** ppGlobal)
{
    HRESULT hr = CoInitialize(NULL);

    if (FAILED(hr)) {
        return false;
    }

#ifdef _VERX64
    hr = NoRegCoCreate(L"msdia140_x64.dll",
        __uuidof(DiaSource),
        __uuidof(IDiaDataSource),
        (void**)ppSource);
#else
    hr = NoRegCoCreate(L"msdia140_x86.dll",
        __uuidof(DiaSource),
        __uuidof(IDiaDataSource),
        (void**)ppSource);
#endif // WIN64


    if (FAILED(hr)) {
        return false;
    }
    return true;
}




bool ExpLoadPdbFile(const wchar_t* szFilename) {
    if (!InitSymbolService(&g_pDiaDataSource, &g_pDiaSession, &g_pGlobalSymbol)) {
        return false;
    }

    HRESULT hr = g_pDiaDataSource->loadDataFromPdb(szFilename);
    if (FAILED(hr)) {
        return false;
    }

    hr = g_pDiaDataSource->openSession(&g_pDiaSession);
    if (FAILED(hr)) {
        return false;
    }

    hr = g_pDiaSession->get_globalScope(&g_pGlobalSymbol);

    if (hr != S_OK) {
        return false;
    }

    DWORD dwMachType = 0;
    if (g_pGlobalSymbol->get_machineType(&dwMachType) == S_OK) {
        switch (dwMachType) {
        case IMAGE_FILE_MACHINE_I386: g_dwMachineType = CV_CFL_80386; break;
        case IMAGE_FILE_MACHINE_IA64: g_dwMachineType = CV_CFL_IA64; break;
        case IMAGE_FILE_MACHINE_AMD64: g_dwMachineType = CV_CFL_AMD64; break;
        }
    }
    return true;
}

void ExpUnLoadPdbFile() {
    if (g_pGlobalSymbol) {
        g_pGlobalSymbol->Release();
        g_pGlobalSymbol = NULL;
    }

    if (g_pDiaSession) {
        g_pDiaSession->Release();
        g_pDiaSession = NULL;
    }
    CoUninitialize();
}


DWORD ExpGetRvaBySymbolName(const wchar_t* szFilename,int index) {
    IDiaEnumSymbols* pEnumSymbols;
    if (FAILED(g_pGlobalSymbol->findChildren(SymTagNull, szFilename, nsCaseInRegularExpression, &pEnumSymbols))) {
        return false;
    }
    
    IDiaSymbol* pCompiland;
    ULONG celt = 0;
    ULONG64 rva = 0;
    for (int i = 0; SUCCEEDED(pEnumSymbols->Next(1, &pCompiland, &celt)) && (celt == 1); i++)
    {
        BSTR bstrName;
        if (pCompiland->get_name(&bstrName) == S_OK) {
            DWORD off = 0;
            pCompiland->get_relativeVirtualAddress(&off);
            rva = off;
            printf("Function  %ls  RVA  %08x\r\n", bstrName, off);
            SysFreeString(bstrName);
            if (i == index)
            {
                break;
            }
        }
    }
    pEnumSymbols->Release();
    return rva;
}

DWORD ExpGetMemberOffsetOfStruct(const wchar_t* structName,const wchar_t* memName,int structIndex,int memIndex) {
    IDiaEnumSymbols* pEnumSymbols;
    if (FAILED(g_pGlobalSymbol->findChildren(SymTagUDT, structName, nsCaseInRegularExpression, &pEnumSymbols))) {
        return false;
    }

    IDiaSymbol* pCompiland;
    ULONG celt = 0;
    ULONG64 offset = -1;
    for (int i = 0; SUCCEEDED(pEnumSymbols->Next(1, &pCompiland, &celt)) && (celt == 1); i++)
    {
        BSTR bstrName;
        IDiaEnumSymbols* pEnumSymbols2;
        IDiaSymbol* pCompiland2;
        ULONG celt2 = 0;
        if (pCompiland->get_name(&bstrName) == S_OK)
        {
            printf("struct  %ls\r\n", bstrName);
            if (FAILED(pCompiland->findChildren(SymTagNull, memName, nsCaseInRegularExpression, &pEnumSymbols2)))
            {
                break;
            }
            BSTR bstrName2;
            for (int j = 0; SUCCEEDED(pEnumSymbols2->Next(1, &pCompiland2, &celt2)) && (celt2 == 1); j++)
            {
                if (pCompiland2->get_name(&bstrName2) == S_OK)
                {
                    LONG off = 0;
                    pCompiland2->get_offset(&off);
                    printf("Mem: off  +0x%04x\t%ls  \r\n", off, bstrName2);
                    SysFreeString(bstrName);
                    if (j == memIndex && i == structIndex)
                    {
                        offset = off;
                        break;
                    }
                }
            }
            pEnumSymbols2->Release();
            SysFreeString(bstrName);
        }
    }
    pEnumSymbols->Release();
    return offset;
}




void ExpGetSymbolNameByRva(ULONG64 rva,wchar_t** szFilename) {
    IDiaSymbol* pSymbol = NULL;
    LONG lDisplacement;
    g_pDiaSession->findSymbolByRVAEx(rva, SymTagNull, &pSymbol, &lDisplacement);
    pSymbol->get_name(szFilename);
}

bool ExpDownloadPdbTo(const char * filename,const char * pdbPath) {
    HANDLE hFile = CreateFileA(
        filename,
        GENERIC_READ, FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) return false;

    DWORD size = GetFileSize(hFile, NULL);
    if (size == INVALID_FILE_SIZE)
    {
        CloseHandle(hFile);
        return false;
    }

    char* buf = (char*)VirtualAlloc(NULL,size,MEM_COMMIT,PAGE_READWRITE);
    if (!buf)
    {
        CloseHandle(hFile);
        return false;
    }
    DWORD readRetBytes = 0;
    if (!ReadFile(hFile, buf, size, &readRetBytes, NULL))
    {
        CloseHandle(hFile);
        VirtualFree(buf, size, MEM_RELEASE);
        return false;
    }

    DWORD pdbIndex = 0;
    for (DWORD i = size-1; i >= 0; i--)
    {
        if (buf[i] == 0x2E && buf[i+1] == 0x70 && buf[i + 2] == 0x64 && buf[i + 3] == 0x62 && buf[i + 4] == 0)
        {
            pdbIndex = i;
            break;
        }
    }

    if (!pdbIndex)
    {
        CloseHandle(hFile);
        VirtualFree(buf, size, MEM_RELEASE);
        return false;
    }

    DWORD rsdsIndex = 0;
    for (int i = 1; i < 0x1000; i++)
    {
        if (buf[pdbIndex - i] == 0 && buf[pdbIndex - i - 1] == 0 && buf[pdbIndex - i - 2] == 0
            && buf[pdbIndex - i - 23] == 0x52 && buf[pdbIndex - i - 22] == 0x53 && buf[pdbIndex - i - 21] == 0x44 && buf[pdbIndex - i - 20] == 0x53)
        {
            rsdsIndex = pdbIndex - i - 23;
            break;
        }
    }

    if (!rsdsIndex)
    {
        CloseHandle(hFile);
        VirtualFree(buf, size, MEM_RELEASE);
        return false;
    }

    char url[0x1000] = { 0 };
    sprintf(url, "http://103.71.50.194:880/download/symbols/%s/%08X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%X/%s",
        buf + rsdsIndex + 24  //xxx.pdb
        , *(PULONG32)(buf + rsdsIndex + 4) //
        , (unsigned char)buf[rsdsIndex + 9]
        , (unsigned char)buf[rsdsIndex + 8]
        , (unsigned char)buf[rsdsIndex + 11]
        , (unsigned char)buf[rsdsIndex + 10]
        , (unsigned char)buf[rsdsIndex + 12]
        , (unsigned char)buf[rsdsIndex + 13]
        , (unsigned char)buf[rsdsIndex + 14]
        , (unsigned char)buf[rsdsIndex + 15]
        , (unsigned char)buf[rsdsIndex + 16]
        , (unsigned char)buf[rsdsIndex + 17]
        , (unsigned char)buf[rsdsIndex + 18]
        , (unsigned char)buf[rsdsIndex + 19]
        , (unsigned char)buf[rsdsIndex + 20]
        , buf + rsdsIndex + 24
    );
    DeleteUrlCacheEntryA(url);
    HRESULT dret = URLDownloadToFileA(NULL, url, pdbPath, 0, NULL);
    if (dret != S_OK)
    {
        CloseHandle(hFile);
        VirtualFree(buf, size, MEM_RELEASE);
        return false;
    }
    CloseHandle(hFile);
    VirtualFree(buf, size, MEM_RELEASE);
    return true;
}



int wmain(int argc, wchar_t* argv[]) {
    //ExpDownloadPdbTo("C:\\Users\\5555\\Downloads\\aaaaaa\\7\\dwmcore.dll", "C:\\Users\\5555\\Downloads\\aaaaaa\\7\\dwmcore.pdb");
    ExpLoadPdbFile(L"C:\\Users\\10613\\Downloads\\d2d1.pdb");
    DWORD off = 0;
    off = ExpGetRvaBySymbolName(L"*PresentDWM@?$D2DDeviceContextBase@UID2D1DeviceContext6*", 0);
  

    off = ExpGetRvaBySymbolName(L"*PresentDWM@?$D2DDeviceContextBase@UID2D1DeviceContext6*", 1);
    

    off = ExpGetRvaBySymbolName(L"*PresentMultiplaneOverlay@?$D2DDeviceContextBase@UID2D1DeviceContext6*", 0);
  
    off = ExpGetRvaBySymbolName(L"*PresentMultiplaneOverlay@?$D2DDeviceContextBase@UID2D1DeviceContext6*", 1);
   
    return TRUE;
   
    ExpUnLoadPdbFile();
    getchar();
    return 0;

}