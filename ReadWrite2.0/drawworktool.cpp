#pragma once
#include "draw.hpp"

extern HDC hdc;
extern SelectBrush_t NtGdiSelectBrush;
extern PatBlt_t NtGdiPatBlt;
extern ReleaseDC_t NtUserReleaseDC;
extern CreateSolidBrush_t NtGdiCreateSolidBrush;
extern DeleteObjectApp_t NtGdiDeleteObjectApp;
extern ExtTextOutW_t NtGdiExtTextOutW;
extern HfontCreate_t NtGdiHfontCreate;
extern SelectFont_t NtGdiSelectFont;
extern gGreSetBkMode GreSetBkMode;
extern gGreSetTextColor GreSetTextColor;
extern gValidateHwnd ValidateHwnd;
extern HANDLE DrawGlobalHandle;

DRAW_WORK_ITEM workList = { 0 };

VOID DrtInitWorkList() {
	SKSTATUS ret = NULL;
	InitializeListHead(&workList.list);
	KeInitializeSpinLock(&workList.lock);
	workList.workId = 1;
}

VOID DrtAddToWorkList(PDRAW_WORK_ITEM work) {
	KLOCK_QUEUE_HANDLE handle = {0};
	//KeAcquireInStackQueuedSpinLock(&workList.lock, &handle);
	InsertHeadList(&workList.list,&work->list);
	//KeReleaseInStackQueuedSpinLock(&handle);
}

PDRAW_WORK_ITEM DrtFindWorkByWid(ULONG64 workId) {
	KLOCK_QUEUE_HANDLE handle = { 0 };
	//KeAcquireInStackQueuedSpinLock(&workList.lock, &handle);
	PDRAW_WORK_ITEM cur = (PDRAW_WORK_ITEM)workList.list.Flink;
	//KeReleaseInStackQueuedSpinLock(&handle);
	while (cur->workId > 1)
	{
		if (cur->workId == workId)
		{
			return cur;
		}

		//KeAcquireInStackQueuedSpinLock(&workList.lock, &handle);
		cur = (PDRAW_WORK_ITEM)cur->list.Flink;
		//KeReleaseInStackQueuedSpinLock(&handle);
	}
	return NULL;
}

SKSTATUS DrtDeleteWork(ULONG64 workId) {
	KLOCK_QUEUE_HANDLE handle = { 0 };
	PDRAW_WORK_ITEM work = DrtFindWorkByWid(workId);
	//KeAcquireInStackQueuedSpinLock(&workList.lock, &handle);
	if (work == NULL) return SK_FAILED_DRAW_NULLWORK;
	if (work->workType == DRWORK_TXT)
	{
		
		ExFreePool(work->attr.txt);
		ExFreePool(work->attr.fontName);
	}
	
	RemoveEntryList(&work->list);
	ExFreePool(work);
	//KeReleaseInStackQueuedSpinLock(&handle);
	return SK_SUCCESS;
}


VOID DrtDrawTxt(PDRAW_WORK_ITEM work) {
	KLOCK_QUEUE_HANDLE handle = { 0 };
	//KeAcquireInStackQueuedSpinLock(&workList.lock, &handle);
	if (!MmIsAddressValid(work))
	{
		//KeReleaseInStackQueuedSpinLock(&handle);
		return;
	}
	
	
	ANSI_STRING ansi = { 0 };
	UNICODE_STRING uni = { 0 };
	RtlInitAnsiString(&ansi, (PCSZ)work->attr.fontName);
	RtlAnsiStringToUnicodeString(&uni,&ansi,TRUE);

	HFONT font = CreateFontW(work->attr.size, 0, 0, 0, FW_NORMAL, 0, 0, 0, CHINESEBIG5_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FF_DONTCARE,uni.Buffer);

	HFONT oldfont = NtGdiSelectFont(hdc, font);

	ANSI_STRING ansi2 = { 0 };
	UNICODE_STRING uni2 = { 0 };
	RtlInitAnsiString(&ansi2, (PCSZ)work->attr.txt);
	RtlAnsiStringToUnicodeString(&uni2, &ansi2, TRUE);

	
	GreSetTextColor(hdc, work->attr.rgbColor);
	DrTextW(hdc, work->attr.x, work->attr.y, 0, NULL, uni2.Buffer, uni2.Length/2, 0);
	NtGdiSelectFont(hdc, oldfont);
	NtGdiDeleteObjectApp(font);
	
	
	RtlFreeUnicodeString(&uni);
	RtlFreeUnicodeString(&uni2);
	
	//KeReleaseInStackQueuedSpinLock(&handle);
}
VOID DrtDrawBorderBox(PDRAW_WORK_ITEM work) {
	KLOCK_QUEUE_HANDLE handle = { 0 };
	//KeAcquireInStackQueuedSpinLock(&workList.lock, &handle);
	ULONG64 x = work->attr.x;
	ULONG64 y = work->attr.y;
	ULONG64 w = work->attr.w;
	ULONG64 h = work->attr.h;
	ULONG64 color = work->attr.rgbColor;
	ULONG64 size = work->attr.size;
	//KeReleaseInStackQueuedSpinLock(&handle);
	
	HBRUSH brush = NtGdiCreateSolidBrush(color, NULL);
	DrBorderBox(hdc, x, y, w, h, size, brush);
	NtGdiDeleteObjectApp(brush);
}
VOID DrtDrawFillRect(PDRAW_WORK_ITEM work) {
	KLOCK_QUEUE_HANDLE handle = { 0 };
	//KeAcquireInStackQueuedSpinLock(&workList.lock, &handle);
	ULONG64 x = work->attr.x;
	ULONG64 y = work->attr.y;
	ULONG64 w = work->attr.w;
	ULONG64 h = work->attr.h;
	ULONG64 color = work->attr.rgbColor;
	//KeReleaseInStackQueuedSpinLock(&handle);
	HBRUSH brush = NtGdiCreateSolidBrush(color, NULL);
	DrFillRectInside(hdc,x,y,w,h, brush);
	NtGdiDeleteObjectApp(brush);
}


extern gGreCreateCompatibleDC GreCreateCompatibleDC;
extern gGreCreateCompatibleBitmapInternal GreCreateCompatibleBitmapInternal;
extern ghbmSelectBitmap hbmSelectBitmap;
extern gNtGdiBitBltInternal NtGdiBitBltInternal;
extern gbDeleteDCInternal bDeleteDCInternal;
extern ggDeleteObject gDeleteObject;
extern gNtUserSetWindowLongPtr NtUserSetWindowLongPtr;

#define LVS_EX_TRANSPARENTBKGND 0x00400000  // Background is painted by the parent via WM_PRINTCLIENT
#define LVS_EX_TRANSPARENTSHADOWTEXT 0x00800000 
VOID DrtDrawAllWork() {
	KLOCK_QUEUE_HANDLE handle = { 0 };
	//KeAcquireInStackQueuedSpinLock(&workList.lock, &handle);
	PDRAW_WORK_ITEM cur = (PDRAW_WORK_ITEM)workList.list.Flink;
	//KeReleaseInStackQueuedSpinLock(&handle);
	//NtUserSetWindowLongPtr((HWND)_GLOBAL_HDCHANDLE,-20, LVS_EX_TRANSPARENTBKGND| LVS_EX_TRANSPARENTSHADOWTEXT,1);
	if (DrawGlobalHandle != (HANDLE)0x10010)
	{
		ULONG64 oldBkMode = GreSetBkMode(hdc, TRANSPARENT);
	}
	//HDC memHdc = GreCreateCompatibleDC(hdc);
	//HBITMAP hMemBmp = GreCreateCompatibleBitmapInternal(hdc,500,500,0,0,0);
	//hbmSelectBitmap(memHdc,hMemBmp,0,0);
	//HDC oldRealHdc = hdc;
	//hdc = memHdc;
	//GreSetBkMode(hdc, TRANSPARENT);
	
	while (cur->workId > 1)
	{
		switch (cur->workType)
		{
		case DRWORK_TXT:
			DrtDrawTxt(cur);
			break;
		case DRWORK_BORDERBOX:
			DrtDrawBorderBox(cur);
			break;
		case DRWORK_FILLRECT:
			DrtDrawFillRect(cur);
			break;
		default:
			break;
		}
	
		//KeAcquireInStackQueuedSpinLock(&workList.lock, &handle);
		cur = (PDRAW_WORK_ITEM)cur->list.Flink;
		//KeReleaseInStackQueuedSpinLock(&handle);
	}
	

	//NtGdiBitBltInternal(oldRealHdc,0,0,500,500,hdc,0,0, SRCCOPY,-1,0);
	//bDeleteDCInternal(memHdc);
	//gDeleteObject((ULONG64)hMemBmp);
	//hdc = oldRealHdc;
	//GreSetBkMode(hdc, oldBkMode);
}


SKSTATUS DrtBindWindow(ULONG64 handle) {
	DrawGlobalHandle = (HANDLE)handle;
	return SK_SUCCESS;
}


