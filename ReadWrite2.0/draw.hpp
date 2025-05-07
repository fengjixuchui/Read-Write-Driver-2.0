#pragma once
#include <ntifs.h>
#include <WinDef.h>
#include <wingdi.h>
#include "global.h"
#include "krtools.h"
#include "petool.h"
#include "mmtool.h"


#define D3DDDI_MAX_BROADCAST_CONTEXT 64
#define D3DDDI_MAX_WRITTEN_PRIMARIES 16

typedef DWORD LFTYPE;
typedef ULONGLONG D3DGPU_VIRTUAL_ADDRESS;
typedef UINT D3DKMT_HANDLE;
typedef long long          int64_t;

typedef struct _D3DKMT_SUBMITCOMMANDFLAGS
{
	UINT    NullRendering : 1;  // 0x00000001
	UINT    PresentRedirected : 1;  // 0x00000002
	UINT    Reserved : 30;  // 0xFFFFFFFC
} D3DKMT_SUBMITCOMMANDFLAGS;
typedef struct _D3DKMT_SUBMITCOMMAND
{
	D3DGPU_VIRTUAL_ADDRESS      Commands;
	UINT                        CommandLength;
	D3DKMT_SUBMITCOMMANDFLAGS   Flags;
	ULONGLONG                   PresentHistoryToken;                            // in: Present history token for redirected present calls
	UINT                        BroadcastContextCount;
	D3DKMT_HANDLE               BroadcastContext[D3DDDI_MAX_BROADCAST_CONTEXT];
	VOID* pPrivateDriverData;
	UINT                        PrivateDriverDataSize;
	UINT                        NumPrimaries;
	D3DKMT_HANDLE               WrittenPrimaries[D3DDDI_MAX_WRITTEN_PRIMARIES];
	UINT                        NumHistoryBuffers;
	D3DKMT_HANDLE* HistoryBufferArray;
} D3DKMT_SUBMITCOMMAND;


//typedef HDC(*GetDC_t)(HWND hwnd);//verified
typedef HDC(*GetDC_t)(HANDLE hwnd);//verified

typedef BOOL(*PatBlt_t)(HDC hdcDest, INT x, INT y, INT cx, INT cy, DWORD dwRop);//verified

typedef HBRUSH(*SelectBrush_t)(HDC hdc, HBRUSH hbrush); //verified

typedef int (*ReleaseDC_t)(HDC hdc); //verified

typedef HBRUSH(*CreateSolidBrush_t)(COLORREF cr, HBRUSH hbr); //verified

typedef BOOL(*DeleteObjectApp_t)(HANDLE hobj); //verified

typedef BOOL(*ExtTextOutW_t)(IN HDC hDC, //verified
	IN INT 	XStart,
	IN INT 	YStart,
	IN UINT 	fuOptions,
	IN OPTIONAL LPRECT 	UnsafeRect,
	IN LPWSTR 	UnsafeString,
	IN INT 	Count,
	IN OPTIONAL LPINT 	UnsafeDx,
	IN DWORD 	dwCodePage
	);

typedef HFONT(*HfontCreate_t)(IN PENUMLOGFONTEXDVW pelfw, IN ULONG cjElfw, IN LFTYPE lft, IN FLONG fl, IN PVOID pvCliData); //verified

typedef HFONT(*SelectFont_t)(_In_ HDC 	hdc, //verified
	_In_ HFONT 	hfont
	);
typedef int64_t(*dxgk_submit_command_t)(D3DKMT_SUBMITCOMMAND* data);

typedef ULONG64(*gNtUserInvalidateRect)(HDC hdc, PRECT rect);

typedef ULONG64(*gGreSetBkMode)(HDC hdc, ULONG mode);

typedef ULONG64(*gGreSetTextColor)(HDC hdc, ULONG color);

typedef ULONG64(*gGreSetBkColor)(HDC hdc, ULONG color);

typedef BOOLEAN(*gNtUserSetWindowPos)(__int64 a1, HWND a2, int a3, int a4, int a5, int a6, unsigned int a7);

typedef ULONG64(*gValidateHwnd)(ULONG64 hwnd);

typedef ULONG64(*gNtUserSetWindowLongPtr)(HWND hwnd,LONG a2,ULONG64 a3,LONG a4);

//Ë«»º³å

typedef HDC(*gGreCreateCompatibleDC)(HDC hdc);

typedef HBITMAP(*gGreCreateCompatibleBitmapInternal)(HDC hdc,ULONG w,ULONG h_and_0xF0FFFFFF,ULONG num_0,ULONG64 num_0__,ULONG64 num_0_);

typedef HBITMAP(*ghbmSelectBitmap)(HDC hdc,HBITMAP bm,ULONG val_0,ULONG val_0_);

typedef ULONG64(*gNtGdiBitBltInternal)(HDC realHdc, ULONG val1_0, ULONG val2_0, ULONG w, ULONG h, HDC memHdc, ULONG val3_0, ULONG val4_0, ULONG rop_SRCCOPY, ULONG val_fuyi, ULONG zero);

typedef ULONG64(*gbDeleteDCInternal)(HDC hdc);

typedef ULONG64(*ggDeleteObject)(ULONG64 pp);

EXTERN_C SKSTATUS DrInstallDraw();
SKSTATUS DrGetAllWin32kModuleBase();
SKSTATUS DrGetDrawKrFuncAddr();

SKSTATUS DrBorderBox(HDC hDC, int x, int y, int w, int h, int thickness, HBRUSH hbr);
SKSTATUS DrFillRectInside(HDC hDC, int x, int y, int w, int h, HBRUSH hbr);
SKSTATUS DrFillRect(HDC hDC, CONST RECT* lprc, HBRUSH hbr);
HFONT CreateFontW(int nHeight,
	int nWidth,
	int nEscapement,
	int nOrientation,
	int nWeight,
	DWORD   fnItalic,
	DWORD   fdwUnderline,
	DWORD   fdwStrikeOut,
	DWORD   fdwCharSet,
	DWORD   fdwOutputPrecision,
	DWORD   fdwClipPrecision,
	DWORD   fdwQuality,
	DWORD   fdwPitchAndFamily,
	LPCWSTR lpszFace);

PVOID AllocateVirtualMemory(SIZE_T Size);

VOID FreeVirtualMemory(PVOID VirtualAddress, SIZE_T Size);
BOOL DrTextW(HDC hdc, INT x, INT y, UINT fuOptions, RECT* lprc, LPWSTR lpString, UINT cwc, INT* lpDx);
BOOL DrTextA(HDC hdc, INT x, INT y, UINT fuOptions, RECT* lprc, LPCSTR lpString, UINT cch, INT* lpDx);
HFONT
CreateFontIndirectW(
	CONST LOGFONTW* lplf
);
HFONT
CreateFontIndirectExW(const ENUMLOGFONTEXDVW* elfexd);

SKSTATUS DrSetDrawHook();
SKSTATUS DrFillRect(HDC hDC, CONST RECT* lprc, HBRUSH hbr);
EXTERN_C VOID __fastcall DrMyHookFunc(
	D3DKMT_SUBMITCOMMAND* data);

SKSTATUS DrSetDrawHookWin10_DxgkRender();
SKSTATUS DrSetDrawHookWin10_SubmitCommand();
SKSTATUS DrSetDrawHookWin7();
SKSTATUS DrSetDrawHookWin10_DxgkPresent();


EXTERN_C VOID SetHookGeneral();
EXTERN_C VOID SetHookGeneral_backup();
EXTERN_C VOID SetHookGeneral_renderwin7();
EXTERN_C VOID SetHookGeneral_present();


typedef enum _WORK_ITEM_TYPE {
	DRWORK_TXT,
	DRWORK_BORDERBOX,
	DRWORK_FILLRECT

}WORK_ITEM_TYPE;

typedef struct _DRAW_ITEM_ATTR {
	ULONG64 x;
	ULONG64 y;
	ULONG64 w;
	ULONG64 h;
	ULONG64 size;
	ULONG64 rgbColor;
	PUCHAR txt;
	PUCHAR fontName;
}DRAW_ITEM_ATTR, * PDRAW_ITEM_ATTR;
typedef struct _DRAW_WORK_ITEM {
	LIST_ENTRY list;
	ULONG64 workId;
	BOOLEAN isDeleted;
	WORK_ITEM_TYPE workType;
	KSPIN_LOCK lock;
	DRAW_ITEM_ATTR attr;
}DRAW_WORK_ITEM,*PDRAW_WORK_ITEM;

#define COLOR_RGBA(r,g,b,a) ((COLORREF)((((DWORD)(BYTE)(a))<<24)|RGB(r,g,b)))

#define LVS_EX_TRANSPARENTBKGND 0x00400000  // Background is painted by the parent via WM_PRINTCLIENT
#define LVS_EX_TRANSPARENTSHADOWTEXT 0x00800000

VOID DrtDrawAllWork();
VOID DrtInitWorkList();
VOID DrtAddToWorkList(PDRAW_WORK_ITEM work);
PDRAW_WORK_ITEM DrtFindWorkByWid(ULONG64 workId);
SKSTATUS DrtDeleteWork(ULONG64 workId);
SKSTATUS DrtBindWindow(ULONG64 handle);



EXTERN_C ULONG64 DrAddToWork_TXT(ULONG64 workId, ULONG64 x, ULONG64 y, ULONG64 txtSize, PUCHAR txt, PUCHAR fontName,ULONG64 color);
EXTERN_C ULONG64 DrAddToWork_BorderBox(ULONG64 workId, ULONG64 x, ULONG64 y, ULONG64 w, ULONG64 h, ULONG64 size, ULONG64 colorRGB);
EXTERN_C ULONG64 DrAddToWork_FillRect(ULONG64 workId, ULONG64 x, ULONG64 y, ULONG64 w, ULONG64 h, ULONG64 colorRGB);
EXTERN_C SKSTATUS DrEndDraw(ULONG64 workId);






EXTERN_C SKSTATUS DrComInstallDraw();
EXTERN_C ULONG64 DrComDrawBorderBox(ULONG64 workId, ULONG64 x, ULONG64 y, ULONG64 w, ULONG64 h, ULONG64 size, ULONG64 colorRGB);
EXTERN_C ULONG64 DrComDrawTXT(ULONG64 workId, ULONG64 x, ULONG64 y, ULONG64 txtSize, PUCHAR txt, PUCHAR fontName,ULONG64 color);
EXTERN_C ULONG64 DrComDrawFillRect(ULONG64 workId, ULONG64 x, ULONG64 y, ULONG64 w, ULONG64 h, ULONG64 colorRGB);
EXTERN_C SKSTATUS DrComEndDraw(ULONG64 workId);
EXTERN_C SKSTATUS DrComDrawBindWindow(ULONG64 handle);
