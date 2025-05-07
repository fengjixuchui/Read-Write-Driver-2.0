#pragma once
#include "kmclass.h"

//NTSTATUS  KmSearchKdbServiceCallBack(IN PDRIVER_OBJECT DriverObject)
//{
//    //定义用到的一组全局变量，这些变量大多数是顾名思义的  
//    NTSTATUS status = STATUS_SUCCESS;
//    SKSTATUS ret = NULL;
//    UNICODE_STRING uniNtNameString;
//    PDEVICE_OBJECT pTargetDeviceObject = NULL;
//    PDRIVER_OBJECT KbdDriverObject = NULL;
//    PDRIVER_OBJECT KbdhidDriverObject = NULL;
//    PDRIVER_OBJECT Kbd8042DriverObject = NULL;
//    PDRIVER_OBJECT UsingDriverObject = NULL;
//    PDEVICE_OBJECT UsingDeviceObject = NULL;
//
//    PVOID UsingDeviceExt = NULL;
//
//    //这里的代码用来打开USB键盘端口驱动的驱动对象  
//    RtlInitUnicodeString(&uniNtNameString, L"\\Driver\\kbdhid");
//    status = ObReferenceObjectByName(&uniNtNameString,
//        OBJ_CASE_INSENSITIVE, NULL, 0,
//        *IoDriverObjectType,
//        KernelMode,
//        NULL,
//        (PVOID*)&KbdhidDriverObject);
//    if (NT_SUCCESS(status))
//    {
//        ObDereferenceObject(KbdhidDriverObject);
//    }
//    //打开PS/2键盘的驱动对象  
//    RtlInitUnicodeString(&uniNtNameString, L"\\Driver\\i8042prt");
//    status = ObReferenceObjectByName(&uniNtNameString,
//        OBJ_CASE_INSENSITIVE,
//        NULL, 0,
//        *IoDriverObjectType,
//        KernelMode,
//        NULL,
//        (PVOID*)&Kbd8042DriverObject);
//    if (NT_SUCCESS(status))
//    {
//        ObDereferenceObject(Kbd8042DriverObject);
//    }
//    //这段代码考虑有一个键盘起作用的情况。如果USB键盘和PS/2键盘同时存在，用PS/2键盘
//    //如果两个设备都没有找到  
//    if (!Kbd8042DriverObject && !KbdhidDriverObject)
//    {
//        return SK_FAILED_KMCLASS_KEY_NULLDRIVEROBJ;
//    }
//    //找到合适的驱动对象，不管是USB还是PS/2，反正一定要找到一个   
//    UsingDriverObject = Kbd8042DriverObject ? Kbd8042DriverObject : KbdhidDriverObject;
//
//    RtlInitUnicodeString(&uniNtNameString, L"\\Driver\\kbdclass");
//    status = ObReferenceObjectByName(&uniNtNameString,
//        OBJ_CASE_INSENSITIVE, NULL,
//        0,
//        *IoDriverObjectType,
//        KernelMode,
//        NULL,
//        (PVOID*)&KbdDriverObject);
//    if (!NT_SUCCESS(status))
//    {
//        //如果没有成功，直接返回即可  
//        EcUpdateLastSysErrCode(status);
//        return SK_FAILED_KMCLASS_KEY_NULLKDBCLASSOBJ;
//    }
//    else
//    {
//        ObDereferenceObject(KbdDriverObject);
//    }
//
//    //遍历KbdDriverObject下的设备对象 
//    UsingDeviceObject = UsingDriverObject->DeviceObject;
//    while (UsingDeviceObject)
//    {
//        ret = SearchServiceFromKdbExt(KbdDriverObject, UsingDeviceObject);
//        if (ret == SK_SUCCESS)
//        {
//            break;
//        }
//        UsingDeviceObject = UsingDeviceObject->NextDevice;
//    }
//
//    //如果成功找到了，就把这个函数替换成我们自己的回调函数  
//    if (g_KoMCallBack.KdbDeviceObject && g_KoMCallBack.KeyboardClassServiceCallback)
//    {
//        return SK_SUCCESS;
//    }
//    return ret;
//}
//
//SKSTATUS SearchServiceFromKdbExt(PDRIVER_OBJECT KbdDriverObject, PDEVICE_OBJECT pPortDev)
//{
//    PDEVICE_OBJECT pTargetDeviceObject = NULL;
//    UCHAR* DeviceExt;
//    int i = 0;
//    NTSTATUS status;
//    PVOID KbdDriverStart;
//    ULONG KbdDriverSize = 0;
//    PDEVICE_OBJECT  pTmpDev;
//    UNICODE_STRING  kbdDriName;
//
//    KbdDriverStart = KbdDriverObject->DriverStart;
//    KbdDriverSize = KbdDriverObject->DriverSize;
//
//    status = STATUS_UNSUCCESSFUL;
//
//    RtlInitUnicodeString(&kbdDriName, L"\\Driver\\kbdclass");
//    pTmpDev = pPortDev;
//    while (pTmpDev->AttachedDevice != NULL)
//    {
//        //KdPrint(("Att:  0x%x", pTmpDev->AttachedDevice));
//        //KdPrint(("Dri Name : %wZ", &pTmpDev->AttachedDevice->DriverObject->DriverName));
//        if (RtlCompareUnicodeString(&pTmpDev->AttachedDevice->DriverObject->DriverName,
//            &kbdDriName, TRUE) == 0)
//        {
//            break;
//        }
//        pTmpDev = pTmpDev->AttachedDevice;
//    }
//    if (pTmpDev->AttachedDevice == NULL)
//    {
//        return SK_FAILED_KMCLASS_KEY_NULLDEVICE;
//    }
//
//    pTargetDeviceObject = KbdDriverObject->DeviceObject;
//    while (pTargetDeviceObject)
//    {
//        if (pTmpDev->AttachedDevice != pTargetDeviceObject)
//        {
//            pTargetDeviceObject = pTargetDeviceObject->NextDevice;
//            continue;
//        }
//        DeviceExt = (UCHAR*)pTmpDev->DeviceExtension;
//        g_KoMCallBack.KdbDeviceObject = NULL;
//        //遍历我们先找到的端口驱动的设备扩展的每一个指针  
//        for (i = 0; i < 4096; i++, DeviceExt++)
//        {
//            PVOID tmp;
//            if (!MmIsAddressValid(DeviceExt))
//            {
//                break;
//            }
//            //找到后会填写到这个全局变量中，这里检查是否已经填好了  
//            //如果已经填好了就不用继续找了，可以直接退出  
//            if (g_KoMCallBack.KdbDeviceObject && g_KoMCallBack.KeyboardClassServiceCallback)
//            {
//                status = STATUS_SUCCESS;
//                break;
//            }
//            //在端口驱动的设备扩展里，找到了类驱动设备对象，填好类驱动设备对象后继续  
//            tmp = *(PVOID*)DeviceExt;
//            if (tmp == pTargetDeviceObject)
//            {
//                g_KoMCallBack.KdbDeviceObject = pTargetDeviceObject;
//                continue;
//            }
//
//            //如果在设备扩展中找到一个地址位于KbdClass这个驱动中，就可以认为，这就是我们要找的回调函数  
//            if ((tmp > KbdDriverStart) && (tmp < (UCHAR*)KbdDriverStart + KbdDriverSize) &&
//                (MmIsAddressValid(tmp)))
//            {
//                //将这个回调函数记录下来  
//                g_KoMCallBack.KeyboardClassServiceCallback = (MY_KEYBOARDCALLBACK)tmp;
//            }
//        }
//        if (status == STATUS_SUCCESS)
//        {
//            break;
//        }
//        //换成下一个设备，继续遍历  
//        pTargetDeviceObject = pTargetDeviceObject->NextDevice;
//    }
//    if (!NT_SUCCESS(status))
//    {
//        return SK_FAILED_KMCLASS_KEY_NULLDEVICECALLBACK;
//    }
//    return SK_SUCCESS;
//}
//
//
//SKSTATUS KmKeyDownUp(PKEYBOARD_INPUT_DATA kid) {
//    ULONG InputDataConsumed;
//    if (!kid || !g_KoMCallBack.KeyboardClassServiceCallback)
//    {
//        return SK_FAILED_KMCLASS_KM_NOTINSTALL;
//    }
//    PKEYBOARD_INPUT_DATA KbdInputDataStart = (PKEYBOARD_INPUT_DATA)ExAllocatePool(NonPagedPool, sizeof(KEYBOARD_INPUT_DATA) * 2 + 100);
//    PKEYBOARD_INPUT_DATA KbdInputDataEnd = KbdInputDataStart + 1;
//    memset(KbdInputDataStart, 0, sizeof(KEYBOARD_INPUT_DATA) * 2 + 100);
//    memcpy(KbdInputDataStart, kid, sizeof(KEYBOARD_INPUT_DATA));
//    g_KoMCallBack.KeyboardClassServiceCallback(g_KoMCallBack.KdbDeviceObject,
//        KbdInputDataStart,
//        KbdInputDataEnd,
//        &InputDataConsumed);
//    ExFreePool(KbdInputDataStart);
//    return SK_SUCCESS;
//}