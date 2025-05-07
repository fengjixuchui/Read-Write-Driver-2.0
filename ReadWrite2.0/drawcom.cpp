#pragma once
#include "draw.hpp"


SKSTATUS DrComInstallDraw() {
	return DrInstallDraw();
}
extern BOOLEAN alreadyInstall;
ULONG64 DrComDrawBorderBox(ULONG64 workId, ULONG64 x, ULONG64 y, ULONG64 w, ULONG64 h, ULONG64 size, ULONG64 colorRGB) {
	if (!alreadyInstall) return NULL;
	return DrAddToWork_BorderBox(workId,  x,  y,  w,  h,  size,  colorRGB);
}
ULONG64 DrComDrawTXT(ULONG64 workId, ULONG64 x, ULONG64 y, ULONG64 txtSize, PUCHAR txt, PUCHAR fontName,ULONG64 color) {
	if (!alreadyInstall) return NULL;
	return DrAddToWork_TXT( workId,  x,  y,  txtSize,  txt,  fontName, color);
}
ULONG64 DrComDrawFillRect(ULONG64 workId, ULONG64 x, ULONG64 y, ULONG64 w, ULONG64 h, ULONG64 colorRGB) {
	if (!alreadyInstall) return NULL;
	return DrAddToWork_FillRect( workId,  x,  y,  w,  h,  colorRGB);
}

SKSTATUS DrComEndDraw(ULONG64 workId) {
	if (!alreadyInstall) return NULL;
	return DrEndDraw(workId);
}

SKSTATUS DrComDrawBindWindow(ULONG64 handle) {
	return DrtBindWindow(handle);
}
