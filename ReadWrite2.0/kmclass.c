#pragma once
#include "kmclass.h"



//SKSTATUS KmInstall() {
//    NTSTATUS status = NULL;
//    SKSTATUS ret = NULL;
//    //   \\Driver\\WMIxWDM
//    UNICODE_STRING ntName = { 0 };
//    RtlInitUnicodeString(&ntName, L"\\Driver\\WMIxWDM");
//    PDRIVER_OBJECT  driverObject = NULL;
//    status = ObReferenceObjectByName(&ntName, FILE_ALL_ACCESS, 0, 0, *IoDriverObjectType, KernelMode, NULL, &driverObject);
//    if (!NT_SUCCESS(status))
//    {
//        EcUpdateLastSysErrCode(status);
//        return SK_FAILED_KMCLASS_KM_NULLOBJ_WMIxWDM;
//    }
//    ObDereferenceObject(driverObject);
//    ret = KmSearchKdbServiceCallBack(driverObject);
//    if (ret != SK_SUCCESS)
//    {
//        return ret;
//    }
//
//    //    status = GetKmclassInfo(DeviceObject, MOUSE_DEVICE);
//    ret = KmSearchMouServiceCallBack(driverObject);
//    if (ret != SK_SUCCESS)
//    {
//        return ret;
//    }
//    return ret;
//}