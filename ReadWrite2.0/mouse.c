#pragma once
#include "kmclass.h"

//SKSTATUS  KmSearchMouServiceCallBack(IN PDRIVER_OBJECT DriverObject)
//{
//    //�����õ���һ��ȫ�ֱ�������Щ����������ǹ���˼���  
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
//    //����Ĵ���������USB���̶˿���������������  
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
//    //��PS/2���̵���������  
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
//    //��������豸��û���ҵ�  
//    if (!Kbd8042DriverObject && !KbdhidDriverObject)
//    {
//        return SK_FAILED_KMCLASS_MOUSE_NULLDRIVEROBJ;
//    }
//    //���USB���̺�PS/2����ͬʱ���ڣ�ʹ��USB���
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
//        //���û�гɹ���ֱ�ӷ��ؼ���  
//        return SK_FAILED_KMCLASS_MOUSE_NULLMOUCLASSOBJ;
//    }
//    else
//    {
//        ObDereferenceObject(KbdDriverObject);
//    }
//    //����KbdDriverObject�µ��豸���� 
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
//        //�����������ҵ��Ķ˿��������豸��չ��ÿһ��ָ��  
//        for (i = 0; i < 4096; i++, DeviceExt++)
//        {
//            PVOID tmp;
//            if (!MmIsAddressValid(DeviceExt))
//            {
//                break;
//            }
//            //�ҵ������д�����ȫ�ֱ����У��������Ƿ��Ѿ������  
//            //����Ѿ�����˾Ͳ��ü������ˣ�����ֱ���˳�  
//            if (g_KoMCallBack.MouDeviceObject && g_KoMCallBack.MouseClassServiceCallback)
//            {
//                status = STATUS_SUCCESS;
//                break;
//            }
//            //�ڶ˿��������豸��չ��ҵ����������豸��������������豸��������  
//            tmp = *(PVOID*)DeviceExt;
//            if (tmp == pTargetDeviceObject)
//            {
//                g_KoMCallBack.MouDeviceObject = pTargetDeviceObject;
//                continue;
//            }
//
//            //������豸��չ���ҵ�һ����ַλ��KbdClass��������У��Ϳ�����Ϊ�����������Ҫ�ҵĻص�����  
//            if ((tmp > KbdDriverStart) && (tmp < (UCHAR*)KbdDriverStart + KbdDriverSize) &&
//                (MmIsAddressValid(tmp)))
//            {
//                //������ص�������¼����  
//                g_KoMCallBack.MouseClassServiceCallback = (MY_MOUSECALLBACK)tmp;
//                //g_KoMCallBack.MouSerCallAddr = (PVOID *)DeviceExt;
//                status = STATUS_SUCCESS;
//            }
//        }
//        if (status == STATUS_SUCCESS)
//        {
//            break;
//        }
//        //������һ���豸����������  
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