#pragma once
#include "draw.hpp"



extern GetDC_t NtUserGetDC;
extern SelectBrush_t NtGdiSelectBrush;
extern PatBlt_t NtGdiPatBlt;
extern ReleaseDC_t NtUserReleaseDC;
extern CreateSolidBrush_t NtGdiCreateSolidBrush;
extern DeleteObjectApp_t NtGdiDeleteObjectApp;
extern ExtTextOutW_t NtGdiExtTextOutW;
extern HfontCreate_t NtGdiHfontCreate;
extern SelectFont_t NtGdiSelectFont;







HDC hdc = NULL;

HANDLE DrawGlobalHandle = (HANDLE)0x10010;

VOID __fastcall DrMyHookFunc(
	D3DKMT_SUBMITCOMMAND* data)
{
	hdc = NtUserGetDC(DrawGlobalHandle);
	//hdc = NtUserGetDC(_GLOBAL_HDCHANDLE);
	DrtDrawAllWork();
	//NtUserReleaseDC(hdc);
}



ULONG64 DrAddToWork_TXT(ULONG64 workId,ULONG64 x, ULONG64 y, ULONG64 txtSize, PUCHAR txt, PUCHAR fontName,ULONG64 color) {
	if (!workId) {
		PDRAW_WORK_ITEM item = (PDRAW_WORK_ITEM)ExAllocatePool(NonPagedPool, sizeof(DRAW_WORK_ITEM));
		if (!item) return NULL;
		KeQueryTickCount(&workId);
		workId = RtlRandomEx((PULONG)&workId);
		item->workId = workId;
		item->isDeleted = FALSE;
		KeInitializeSpinLock(&item->lock);
		item->workType = DRWORK_TXT;
		item->attr.x = x;
		item->attr.y = y;
		item->attr.rgbColor = color;
		item->attr.size = txtSize;
		item->attr.txt = (PUCHAR)ExAllocatePool(NonPagedPool, strlen((const char*)txt) + 10);
		RtlZeroMemory(item->attr.txt, strlen((const char*)txt) + 10);
		memcpy(item->attr.txt, txt, strlen((const char*)txt));
		item->attr.fontName = (PUCHAR)ExAllocatePool(NonPagedPool, strlen((const char*)fontName) + 10);
		RtlZeroMemory(item->attr.fontName, strlen((const char*)fontName) + 10);
		memcpy(item->attr.fontName, fontName, strlen((const char*)fontName));
		DrtAddToWorkList(item);
	}
	else
	{
		PDRAW_WORK_ITEM item = DrtFindWorkByWid(workId);
		if (item == NULL) return 0;
		
		PUCHAR txtTT = (PUCHAR)ExAllocatePool(NonPagedPool, strlen((const char*)txt) + 10);
		PUCHAR fontNameTT = (PUCHAR)ExAllocatePool(NonPagedPool, strlen((const char*)fontName) + 10);
		RtlZeroMemory(txtTT, strlen((const char*)txt) + 10);
		RtlZeroMemory(fontNameTT, strlen((const char*)fontName) + 10);
		memcpy(txtTT, txt, strlen((const char*)txt));
		memcpy(fontNameTT, fontName, strlen((const char*)fontName));




		KLOCK_QUEUE_HANDLE handle = { 0 };
		//KeAcquireInStackQueuedSpinLock(&item->lock, &handle);
		item->attr.x = x;
		item->attr.y = y;
		item->attr.size = txtSize;
		item->attr.rgbColor = color;
		if (strcmp((const char*)txtTT, (const char*)item->attr.txt) != 0)
		{
			ExFreePool(item->attr.txt);
			item->attr.txt = (PUCHAR)ExAllocatePool(NonPagedPool, strlen((const char*)txt) + 10);
			RtlZeroMemory(item->attr.txt, strlen((const char*)txtTT) + 10);
			memcpy(item->attr.txt, txtTT, strlen((const char*)txtTT));
			
		}
		if (strcmp((const char *)fontNameTT, (const char*)item->attr.fontName) != 0)
		{
			ExFreePool(item->attr.fontName);
			item->attr.fontName = (PUCHAR)ExAllocatePool(NonPagedPool, strlen((const char*)fontName) + 10);
			RtlZeroMemory(item->attr.fontName, strlen((const char*)fontNameTT) + 10);
			memcpy(item->attr.fontName, fontNameTT, strlen((const char*)fontNameTT));
		}
		ExFreePool(txtTT);
		ExFreePool(fontNameTT);
		//KeReleaseInStackQueuedSpinLock(&handle);
	}
	return workId;
}


ULONG64 DrAddToWork_BorderBox(ULONG64 workId, ULONG64 x, ULONG64 y , ULONG64 w, ULONG64 h, ULONG64 size,ULONG64 colorRGB) {
	if (!workId) {
		PDRAW_WORK_ITEM item = (PDRAW_WORK_ITEM)ExAllocatePool(NonPagedPool, sizeof(DRAW_WORK_ITEM));
		if (!item) return NULL;
		KeQueryTickCount(&workId);
		workId = RtlRandomEx((PULONG)&workId);
		item->workId = workId;
		item->isDeleted = FALSE;
		KeInitializeSpinLock(&item->lock);
		item->workType = DRWORK_BORDERBOX;
		item->attr.x = x;
		item->attr.y = y;
		item->attr.w = w;
		item->attr.h = h;
		item->attr.size = size;
		item->attr.rgbColor = colorRGB;
		DrtAddToWorkList(item);
	}
	else
	{
		PDRAW_WORK_ITEM item = DrtFindWorkByWid(workId);
		if (item == NULL) return 0;

		KLOCK_QUEUE_HANDLE handle = { 0 };
		//KeAcquireInStackQueuedSpinLock(&item->lock, &handle);
		item->attr.x = x;
		item->attr.y = y;
		item->attr.w = w;
		item->attr.h = h;
		item->attr.size = size;
		item->attr.rgbColor = colorRGB;
		//KeReleaseInStackQueuedSpinLock(&handle);
	}
	return workId;
}


ULONG64 DrAddToWork_FillRect(ULONG64 workId, ULONG64 x, ULONG64 y, ULONG64 w, ULONG64 h, ULONG64 colorRGB) {
	if (!workId) {
		PDRAW_WORK_ITEM item = (PDRAW_WORK_ITEM)ExAllocatePool(NonPagedPool, sizeof(DRAW_WORK_ITEM));
		if (!item) return NULL;
		KeQueryTickCount(&workId);
		workId = RtlRandomEx((PULONG)&workId);
		item->workId = workId;
		item->isDeleted = FALSE;
		KeInitializeSpinLock(&item->lock);
		item->workType = DRWORK_FILLRECT;
		item->attr.x = x;
		item->attr.y = y;
		item->attr.w = w;
		item->attr.h = h;
		item->attr.rgbColor = colorRGB;
		DrtAddToWorkList(item);
	}
	else
	{
		PDRAW_WORK_ITEM item = DrtFindWorkByWid(workId);
		if (item == NULL) return 0;

		KLOCK_QUEUE_HANDLE handle = { 0 };
		//KeAcquireInStackQueuedSpinLock(&item->lock, &handle);
		item->attr.x = x;
		item->attr.y = y;
		item->attr.w = w;
		item->attr.h = h;
		item->attr.rgbColor = colorRGB;
		//KeReleaseInStackQueuedSpinLock(&handle);
	}
	return workId;
}



SKSTATUS DrEndDraw(ULONG64 workId) {
	return DrtDeleteWork(workId);
}