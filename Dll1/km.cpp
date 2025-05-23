#pragma once
#include "comm.h"
#include "km.h"


SKSTATUS KmInstall() {
	return ComSend(CMD_KM_INSTALL, 0, 0, 0, 0,0,0,0,0);
}

SKSTATUS KmKeyDown(ULONG64 kVal) {
	KEYBOARD_INPUT_DATA  kid;
	memset(&kid, 0, sizeof(KEYBOARD_INPUT_DATA));
	kid.Flags = KEY_DOWN;
	kid.MakeCode = (USHORT)MapVirtualKey(kVal, 0);
	return ComSend(CMD_KM_KEY, (ULONG64)&kid, 0, 0, 0, 0, 0, 0, 0);
}

SKSTATUS KmKeyUp(ULONG64 kVal) {
	KEYBOARD_INPUT_DATA  kid;
	memset(&kid, 0, sizeof(KEYBOARD_INPUT_DATA));
	kid.Flags = KEY_UP;
	kid.MakeCode = (USHORT)MapVirtualKey(kVal, 0);
	return ComSend(CMD_KM_KEY, (ULONG64)&kid, 0, 0, 0, 0, 0, 0, 0);
}

SKSTATUS KmMouseLeftDown() {
	MOUSE_INPUT_DATA  mid;
	DWORD dwOutput;
	memset(&mid, 0, sizeof(MOUSE_INPUT_DATA));
	mid.ButtonFlags = MOUSE_LEFT_BUTTON_DOWN;
	return ComSend(CMD_KM_MOUSE, (ULONG64)&mid, 0, 0, 0, 0, 0, 0, 0);
}

SKSTATUS KmMouseLeftUp() {
	MOUSE_INPUT_DATA  mid;
	DWORD dwOutput;
	memset(&mid, 0, sizeof(MOUSE_INPUT_DATA));
	mid.ButtonFlags = MOUSE_LEFT_BUTTON_UP;
	return ComSend(CMD_KM_MOUSE, (ULONG64)&mid, 0, 0, 0, 0, 0, 0, 0);
}

SKSTATUS KmMouseRightDown() {
	MOUSE_INPUT_DATA  mid;
	DWORD dwOutput;
	memset(&mid, 0, sizeof(MOUSE_INPUT_DATA));
	mid.ButtonFlags = MOUSE_RIGHT_BUTTON_DOWN;
	return ComSend(CMD_KM_MOUSE, (ULONG64)&mid, 0, 0, 0, 0, 0, 0, 0);
}

SKSTATUS KmMouseRightUp() {
	MOUSE_INPUT_DATA  mid;
	DWORD dwOutput;
	memset(&mid, 0, sizeof(MOUSE_INPUT_DATA));
	mid.ButtonFlags = MOUSE_RIGHT_BUTTON_UP;
	return ComSend(CMD_KM_MOUSE, (ULONG64)&mid, 0, 0, 0, 0, 0, 0, 0);
}

SKSTATUS KmMouseMidDown() {
	MOUSE_INPUT_DATA  mid;
	DWORD dwOutput;
	memset(&mid, 0, sizeof(MOUSE_INPUT_DATA));
	mid.ButtonFlags = MOUSE_MIDDLE_BUTTON_DOWN;
	return ComSend(CMD_KM_MOUSE, (ULONG64)&mid, 0, 0, 0, 0, 0, 0, 0) ;
}

SKSTATUS KmMouseMidUp() {
	MOUSE_INPUT_DATA  mid;
	DWORD dwOutput;
	memset(&mid, 0, sizeof(MOUSE_INPUT_DATA));
	mid.ButtonFlags = MOUSE_MIDDLE_BUTTON_UP;
	return ComSend(CMD_KM_MOUSE, (ULONG64)&mid, 0, 0, 0, 0, 0, 0, 0);
}

SKSTATUS KmMouseMoveRelative(LONG64 dx,LONG64 dy) {
	MOUSE_INPUT_DATA  mid;
	memset(&mid, 0, sizeof(MOUSE_INPUT_DATA));
	mid.Flags = MOUSE_MOVE_RELATIVE;
	mid.LastX = dx;
	mid.LastY = dy;
	return ComSend(CMD_KM_MOUSE, (ULONG64)&mid, 0, 0, 0, 0, 0, 0, 0);
}

SKSTATUS KmMouseMoveTo(LONG64 dx, LONG64 dy) {
	MOUSE_INPUT_DATA  mid;
	DWORD dwOutput;
	memset(&mid, 0, sizeof(MOUSE_INPUT_DATA));
	mid.Flags = MOUSE_MOVE_ABSOLUTE;
	mid.LastX = dx * 0xffff / GetSystemMetrics(SM_CXSCREEN);
	mid.LastY = dy * 0xffff / GetSystemMetrics(SM_CYSCREEN);
	return ComSend(CMD_KM_MOUSE, (ULONG64)&mid, 0, 0, 0, 0, 0, 0, 0);
}