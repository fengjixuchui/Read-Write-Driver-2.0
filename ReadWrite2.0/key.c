#pragma once
#include "kmclass.h"

//NTSTATUS  KmSearchKdbServiceCallBack(IN PDRIVER_OBJECT DriverObject)
//{
//    //�����õ���һ��ȫ�ֱ�������Щ����������ǹ���˼���  
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
//    //����Ĵ���������USB���̶˿���������������  
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
//    //��δ��뿼����һ�����������õ���������USB���̺�PS/2����ͬʱ���ڣ���PS/2����
//    //��������豸��û���ҵ�  
//    if (!Kbd8042DriverObject && !KbdhidDriverObject)
//    {
//        return SK_FAILED_KMCLASS_KEY_NULLDRIVEROBJ;
//    }
//    //�ҵ����ʵ��������󣬲�����USB����PS/2������һ��Ҫ�ҵ�һ��   
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
//        //���û�гɹ���ֱ�ӷ��ؼ���  
//        EcUpdateLastSysErrCode(status);
//        return SK_FAILED_KMCLASS_KEY_NULLKDBCLASSOBJ;
//    }
//    else
//    {
//        ObDereferenceObject(KbdDriverObject);
//    }
//
//    //����KbdDriverObject�µ��豸���� 
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
//    //����ɹ��ҵ��ˣ��Ͱ���������滻�������Լ��Ļص�����  
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
//            if (g_KoMCallBack.KdbDeviceObject && g_KoMCallBack.KeyboardClassServiceCallback)
//            {
//                status = STATUS_SUCCESS;
//                break;
//            }
//            //�ڶ˿��������豸��չ��ҵ����������豸��������������豸��������  
//            tmp = *(PVOID*)DeviceExt;
//            if (tmp == pTargetDeviceObject)
//            {
//                g_KoMCallBack.KdbDeviceObject = pTargetDeviceObject;
//                continue;
//            }
//
//            //������豸��չ���ҵ�һ����ַλ��KbdClass��������У��Ϳ�����Ϊ�����������Ҫ�ҵĻص�����  
//            if ((tmp > KbdDriverStart) && (tmp < (UCHAR*)KbdDriverStart + KbdDriverSize) &&
//                (MmIsAddressValid(tmp)))
//            {
//                //������ص�������¼����  
//                g_KoMCallBack.KeyboardClassServiceCallback = (MY_KEYBOARDCALLBACK)tmp;
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