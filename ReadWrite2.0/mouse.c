#pragma once
#include "kmclass.h"

//SKSTATUS  KmSearchMouServiceCallBack(IN PDRIVER_OBJECT DriverObject)
//{
//    //定义用到的一组全局变量，这些变量大多数是顾名思义的  
//    NTSTATUS status = STATUS_SUCCESS;
//    SKSTATUS ret = NULL;;
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
//    RtlInitUnicodeString(&uniNtNameString, L"\\Driver\\mouhid");
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
//    //如果两个设备都没有找到  
//    if (!Kbd8042DriverObject && !KbdhidDriverObject)
//    {
//        return SK_FAILED_KMCLASS_MOUSE_NULLDRIVEROBJ;
//    }
//    //如果USB键盘和PS/2键盘同时存在，使用USB鼠标
//    if (KbdhidDriverObject)
//    {
//        UsingDriverObject = KbdhidDriverObject;
//    }
//    else
//    {
//        UsingDriverObject = Kbd8042DriverObject;
//    }
//    RtlInitUnicodeString(&uniNtNameString, L"\\Driver\\mouclass");
//    status = ObReferenceObjectByName(&uniNtNameString,
//        OBJ_CASE_INSENSITIVE, NULL,
//        0,
//        *IoDriverObjectType,
//        KernelMode,
//        NULL,
//        (PVOID*)&KbdDriverObject);
//    if (!NT_SUCCESS(status))
//    {
//        EcUpdateLastSysErrCode(status);
//        //如果没有成功，直接返回即可  
//        return SK_FAILED_KMCLASS_MOUSE_NULLMOUCLASSOBJ;
//    }
//    else
//    {
//        ObDereferenceObject(KbdDriverObject);
//    }
//    //遍历KbdDriverObject下的设备对象 
//    UsingDeviceObject = UsingDriverObject->DeviceObject;
//    while (UsingDeviceObject)
//    {
//        ret = SearchServiceFromMouExt(KbdDriverObject, UsingDeviceObject);
//        if (ret == SK_SUCCESS)
//        {
//            break;
//        }
//        UsingDeviceObject = UsingDeviceObject->NextDevice;
//    }
//    if (g_KoMCallBack.MouDeviceObject && g_KoMCallBack.MouseClassServiceCallback)
//    {
//        return SK_SUCCESS;
//    }
//    return ret;
//}
//
//
//SKSTATUS SearchServiceFromMouExt(PDRIVER_OBJECT MouDriverObject, PDEVICE_OBJECT pPortDev)
//{
//    PDEVICE_OBJECT pTargetDeviceObject = NULL;
//    UCHAR* DeviceExt;
//    int i = 0;
//    NTSTATUS status;
//    SKSTATUS ret = NULL;
//    PVOID KbdDriverStart;
//    ULONG KbdDriverSize = 0;
//    PDEVICE_OBJECT  pTmpDev;
//    UNICODE_STRING  kbdDriName;
//
//    KbdDriverStart = MouDriverObject->DriverStart;
//    KbdDriverSize = MouDriverObject->DriverSize;
//
//    status = STATUS_UNSUCCESSFUL;
//
//    RtlInitUnicodeString(&kbdDriName, L"\\Driver\\mouclass");
//    pTmpDev = pPortDev;
//    while (pTmpDev->AttachedDevice != NULL)
//    {
//        //KdPrint(("Att:  0x%x", pTmpDev->AttachedDevice));
//        //KdPrint(("Dri Name : %wZ", &pTmpDev->AttachedDevice->DriverObject->DriverName));
//        if (RtlCompareUnicodeString(&pTmpDev->AttachedDevice->DriverObject->DriverName,
//            &kbdDriName, TRUE) == 0)
//        {
//            //KdPrint(("Find Object Device: "));
//            break;
//        }
//        pTmpDev = pTmpDev->AttachedDevice;
//    }
//    if (pTmpDev->AttachedDevice == NULL)
//    {
//        return SK_FAILED_KMCLASS_MOUSE_NULLDEVICE;
//    }
//    pTargetDeviceObject = MouDriverObject->DeviceObject;
//    while (pTargetDeviceObject)
//    {
//        if (pTmpDev->AttachedDevice != pTargetDeviceObject)
//        {
//            pTargetDeviceObject = pTargetDeviceObject->NextDevice;
//            continue;
//        }
//        DeviceExt = (UCHAR*)pTmpDev->DeviceExtension;
//        g_KoMCallBack.MouDeviceObject = NULL;
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
//            if (g_KoMCallBack.MouDeviceObject && g_KoMCallBack.MouseClassServiceCallback)
//            {
//                status = STATUS_SUCCESS;
//                break;
//            }
//            //在端口驱动的设备扩展里，找到了类驱动设备对象，填好类驱动设备对象后继续  
//            tmp = *(PVOID*)DeviceExt;
//            if (tmp == pTargetDeviceObject)
//            {
//                g_KoMCallBack.MouDeviceObject = pTargetDeviceObject;
//                continue;
//            }
//
//            //如果在设备扩展中找到一个地址位于KbdClass这个驱动中，就可以认为，这就是我们要找的回调函数  
//            if ((tmp > KbdDriverStart) && (tmp < (UCHAR*)KbdDriverStart + KbdDriverSize) &&
//                (MmIsAddressValid(tmp)))
//            {
//                //将这个回调函数记录下来  
//                g_KoMCallBack.MouseClassServiceCallback = (MY_MOUSECALLBACK)tmp;
//                //g_KoMCallBack.MouSerCallAddr = (PVOID *)DeviceExt;
//                status = STATUS_SUCCESS;
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
//        return SK_FAILED_KMCLASS_MOUSE_NULLDEVICECALLBACK;
//    }
//    return SK_SUCCESS;
//}
//
//SKSTATUS KmMouseDownUp(PMOUSE_INPUT_DATA mid) {
//    ULONG InputDataConsumed;
//    if (!mid || !g_KoMCallBack.MouseClassServiceCallback)
//    {
//        return SK_FAILED_KMCLASS_KM_NOTINSTALL;
//    }
//    //MouseInputDataEnd = MouseInputDataStart + 1;
//    PMOUSE_INPUT_DATA KbdInputDataStart = (PMOUSE_INPUT_DATA)ExAllocatePool(NonPagedPool, sizeof(MOUSE_INPUT_DATA) * 2 + 100);
//    PMOUSE_INPUT_DATA KbdInputDataEnd = KbdInputDataStart + 1;
//    memset(KbdInputDataStart, 0, sizeof(MOUSE_INPUT_DATA) * 2 + 100);
//    memcpy(KbdInputDataStart, mid, sizeof(MOUSE_INPUT_DATA));
//    g_KoMCallBack.MouseClassServiceCallback(g_KoMCallBack.MouDeviceObject,
//        KbdInputDataStart,
//        KbdInputDataEnd,
//        &InputDataConsumed);
//    ExFreePool(KbdInputDataStart);
//    return SK_SUCCESS;
//}