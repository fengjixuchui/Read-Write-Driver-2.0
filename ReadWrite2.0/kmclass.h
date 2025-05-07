#pragma once
#include <ntifs.h>
#include "global.h"

typedef struct _MOUSE_INPUT_DATA {

    USHORT UnitId;

    USHORT Flags;

    union {
        ULONG Buttons;
        struct {
            USHORT  ButtonFlags;
            USHORT  ButtonData;
        };
    };

    ULONG RawButtons;

    LONG LastX;

    LONG LastY;

    ULONG ExtraInformation;

} MOUSE_INPUT_DATA, * PMOUSE_INPUT_DATA;
typedef struct _KEYBOARD_INPUT_DATA {

    USHORT UnitId;

    USHORT MakeCode;

    USHORT Flags;

    USHORT Reserved;

    ULONG ExtraInformation;

} KEYBOARD_INPUT_DATA, * PKEYBOARD_INPUT_DATA;
// 回调函数声明
typedef VOID(*MY_KEYBOARDCALLBACK) (PDEVICE_OBJECT  DeviceObject,
    PKEYBOARD_INPUT_DATA  InputDataStart,
    PKEYBOARD_INPUT_DATA  InputDataEnd,
    PULONG  InputDataConsumed);

typedef VOID(*MY_MOUSECALLBACK) (PDEVICE_OBJECT  DeviceObject,
    PMOUSE_INPUT_DATA  InputDataStart,
    PMOUSE_INPUT_DATA  InputDataEnd,
    PULONG  InputDataConsumed);

struct
{
    PDEVICE_OBJECT KdbDeviceObject;
    MY_KEYBOARDCALLBACK KeyboardClassServiceCallback;
    PDEVICE_OBJECT MouDeviceObject;
    MY_MOUSECALLBACK MouseClassServiceCallback;
}g_KoMCallBack;



SKSTATUS KmInstall();
SKSTATUS  KmSearchMouServiceCallBack(IN PDRIVER_OBJECT DriverObject);
SKSTATUS SearchServiceFromMouExt(PDRIVER_OBJECT MouDriverObject, PDEVICE_OBJECT pPortDev);
SKSTATUS SearchServiceFromKdbExt(PDRIVER_OBJECT KbdDriverObject, PDEVICE_OBJECT pPortDev);
SKSTATUS  KmSearchKdbServiceCallBack(IN PDRIVER_OBJECT DriverObject);
SKSTATUS KmKeyDownUp(PKEYBOARD_INPUT_DATA kid);
SKSTATUS KmMouseDownUp(PMOUSE_INPUT_DATA mid);


