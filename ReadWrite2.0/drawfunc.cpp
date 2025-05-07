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

//矩形框
SKSTATUS DrBorderBox(HDC hDC, int x, int y, int w, int h, int thickness, HBRUSH hbr)
{
	SKSTATUS ret = NULL;
	HBRUSH prevhbr = NULL;
	ret |= DrFillRectInside(hDC, x, y, w, thickness, hbr); //Top horiz line
	ret |= DrFillRectInside(hDC, x, y, thickness, h, hbr); //Left vertical line
	ret |= DrFillRectInside(hDC, (x + w), y, thickness, h, hbr); //right vertical line
	ret |= DrFillRectInside(hDC, x, y + h, w + thickness, thickness, hbr); //bottom horiz line
	return ret;
}

//实心矩形2
SKSTATUS DrFillRectInside(HDC hDC, int x, int y, int w, int h, HBRUSH hbr)
{
	BOOLEAN Ret;
	HBRUSH prevhbr = NULL;

	RECT lprc = { x, y, x + w, y + h };
	prevhbr = NtGdiSelectBrush(hDC, hbr);
	Ret = NtGdiPatBlt(hDC, lprc.left, lprc.top, lprc.right - lprc.left,
		lprc.bottom - lprc.top, PATCOPY);

	/* Select old brush */
	if (prevhbr)
		NtGdiSelectBrush(hDC, prevhbr);

	return Ret?SK_SUCCESS: SK_FAILED_DRAW_DRAW;
}

//实心矩形
SKSTATUS DrFillRect(HDC hDC, CONST RECT* lprc, HBRUSH hbr)
{
	BOOL Ret;
	HBRUSH prevhbr = NULL;

	prevhbr = NtGdiSelectBrush(hDC, hbr);
	Ret = NtGdiPatBlt(hDC, lprc->left, lprc->top, lprc->right - lprc->left,
		lprc->bottom - lprc->top, PATCOPY);

	if (prevhbr)
		NtGdiSelectBrush(hDC, prevhbr);
	if (Ret)
	{
		return SK_SUCCESS;
	}
	return SK_FAILED_DRAW_DRAW;
}


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
	LPCWSTR lpszFace) {
	LOGFONTW logfont;
	logfont.lfHeight = nHeight;
	logfont.lfWidth = nWidth;
	logfont.lfEscapement = nEscapement;
	logfont.lfOrientation = nOrientation;
	logfont.lfWeight = nWeight;
	logfont.lfItalic = (BYTE)fnItalic;
	logfont.lfUnderline = (BYTE)fdwUnderline;
	logfont.lfStrikeOut = (BYTE)fdwStrikeOut;
	logfont.lfCharSet = (BYTE)fdwCharSet;
	logfont.lfOutPrecision = (BYTE)fdwOutputPrecision;
	logfont.lfClipPrecision = (BYTE)fdwClipPrecision;
	logfont.lfQuality = (BYTE)fdwQuality;
	logfont.lfPitchAndFamily = (BYTE)fdwPitchAndFamily;
	if (NULL != lpszFace)
	{
		int Size = sizeof(logfont.lfFaceName) / sizeof(WCHAR);
		wcsncpy((wchar_t*)logfont.lfFaceName, lpszFace, Size - 1);
		/* Be 101% sure to have '\0' at end of string */
		logfont.lfFaceName[Size - 1] = '\0';
	}
	else
	{
		logfont.lfFaceName[0] = L'\0';
	}
	return CreateFontIndirectW(&logfont);
}

HFONT
CreateFontIndirectW(
	CONST LOGFONTW* lplf
)
{
	if (lplf)
	{
		ENUMLOGFONTEXDVW Logfont;

		RtlCopyMemory(&Logfont.elfEnumLogfontEx.elfLogFont, lplf, sizeof(LOGFONTW));
		// Need something other than just cleaning memory here.
		// Guess? Use caller data to determine the rest.
		RtlZeroMemory(&Logfont.elfEnumLogfontEx.elfFullName,
			sizeof(Logfont.elfEnumLogfontEx.elfFullName));
		RtlZeroMemory(&Logfont.elfEnumLogfontEx.elfStyle,
			sizeof(Logfont.elfEnumLogfontEx.elfStyle));
		RtlZeroMemory(&Logfont.elfEnumLogfontEx.elfScript,
			sizeof(Logfont.elfEnumLogfontEx.elfScript));

		Logfont.elfDesignVector.dvNumAxes = 0; // No more than MM_MAX_NUMAXES

		RtlZeroMemory(&Logfont.elfDesignVector, sizeof(DESIGNVECTOR));

		return CreateFontIndirectExW(&Logfont);
	}
	else {
		return NULL;
	}
}

HFONT
CreateFontIndirectExW(const ENUMLOGFONTEXDVW* elfexd)
{
	/* Msdn: Note, this function ignores the elfDesignVector member in
			 ENUMLOGFONTEXDV.
	 */
	if (elfexd)
	{
		return NtGdiHfontCreate((PENUMLOGFONTEXDVW)elfexd, 0, 0, 0, NULL);
	}
	else {
		return NULL;
	}
}


PVOID AllocateVirtualMemory(SIZE_T Size)
{
	PVOID pMem = NULL;
	NTSTATUS statusAlloc = ZwAllocateVirtualMemory(NtCurrentProcess(), &pMem, 0, &Size, MEM_COMMIT, PAGE_READWRITE);
	return pMem;
}
VOID FreeVirtualMemory(PVOID VirtualAddress, SIZE_T Size)
{
	if (MmIsAddressValid(VirtualAddress))
	{
		NTSTATUS Status = ZwFreeVirtualMemory(NtCurrentProcess(), &VirtualAddress, &Size, MEM_RELEASE);
	}
}

BOOL DrTextW(HDC hdc, INT x, INT y, UINT fuOptions, RECT* lprc, LPWSTR lpString, UINT cwc, INT* lpDx)
{
	BOOL		nRet = FALSE;
	PVOID       local_lpString = NULL;
	RECT* local_lprc = NULL;
	INT* local_lpDx = NULL;

	if (lprc != NULL)
	{
		SIZE_T Len = sizeof(RECT);
		local_lprc = (RECT*)AllocateVirtualMemory(Len);
		if (local_lprc != NULL)
		{
			__try
			{
				RtlZeroMemory(local_lprc, Len);
				RtlCopyMemory(local_lprc, lprc, Len);
			}
			__except (1)
			{
				goto $EXIT;
			}
		}
		else
		{
			goto $EXIT;
		}
	}

	if (cwc != 0)
	{
		SIZE_T     AllocSize = sizeof(WCHAR) * cwc + 1;
		local_lpString = AllocateVirtualMemory(AllocSize);

		if (local_lpString != NULL)
		{
			__try
			{
				RtlZeroMemory(local_lpString, AllocSize);
				RtlCopyMemory(local_lpString, lpString, AllocSize);
			}
			__except (1)
			{
				goto $EXIT;
			}
		}
		else
		{
			goto $EXIT;
		}
	}

	if (local_lpDx != NULL)
	{
		SIZE_T     AllocSize = sizeof(INT);
		local_lpDx = (INT*)AllocateVirtualMemory(AllocSize);
		if (local_lpDx != NULL)
		{
			__try
			{
				RtlZeroMemory(local_lpString, AllocSize);
				*local_lpDx = *lpDx;
			}
			__except (1)
			{
				goto $EXIT;
			}
		}
		else
		{
		}
	}

	if (NtGdiExtTextOutW != NULL) {
		nRet = NtGdiExtTextOutW(hdc, x, y, fuOptions, local_lprc, (LPWSTR)local_lpString, cwc, local_lpDx, 0);
	}
	else {
	}
$EXIT:
	if (lprc != NULL)
	{
		FreeVirtualMemory(lprc, sizeof(RECT));
		lprc = NULL;
	}

	if (local_lpDx != NULL)
	{
		FreeVirtualMemory(local_lpDx, sizeof(INT));
		local_lpDx = NULL;
	}

	if (local_lpString != NULL)
	{
		FreeVirtualMemory(local_lpString, cwc);
		local_lpString = NULL;
	}

	return nRet;
}

BOOL DrTextA(HDC hdc, INT x, INT y, UINT fuOptions, RECT* lprc, LPCSTR lpString, UINT cch, INT* lpDx)
{
	ANSI_STRING StringA;
	UNICODE_STRING StringU;
	BOOL ret;
	RtlInitAnsiString(&StringA, (LPSTR)lpString);
	RtlAnsiStringToUnicodeString(&StringU, &StringA, TRUE);
	ret = DrTextW(hdc, x, y, fuOptions, lprc, StringU.Buffer, cch, lpDx);
	RtlFreeUnicodeString(&StringU);
	return ret;
}