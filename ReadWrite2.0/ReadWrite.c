#pragma once
#include <ntifs.h>
#include "comm.h"
#include "draw.hpp"
#include "symtool.h"


/*
 �κ����������õ�ʱ���Ƚϳ���������ڴ�Խ�󣬺�ʱԽ��
 vmxPid:���������ID vmware-vmx.exe��ID
 MajorVersion:�����ϵͳ�����汾
 MinorVersion:�����ϵͳ�Ĵΰ汾
 BuildNumber :�����ϵͳ���ڲ��汾��
 ����:WIN7 MajorVersion=6  MinorVersion=1 BuildNumber=7600 �� 7601
 ����:WIN10 MajorVersion=10  MinorVersion=0 BuildNumber=18326 ����1903
 -----------------------------------------
 Windows 11��21H2�� 22000
 Windows 10��21H1��	19043
 Windows 10��20H2��	19042
 Windows 10��2004)  19041
 Windows 10��1909��	18363
 Windows 10��1903��	18362
 Windows 10��1809��	17763
 Windows 10��1803��	17134
 Windows 10��1709��	16299
 Windows 10��1703��	15063
 Windows 10��1607��	14393   ��ͨ��
 Windows 10��1511��	10586   ��ͨ��
 Windows 10	(1507)	10240   ��ͨ��

 Windows 8.1������1��	MajorVersion = 6 MinorVersion = 3 BuildNumber = 9600
 Windows 8.1			MajorVersion = 6 MinorVersion = 3 BuildNumber = 9200
 Windows 8				MajorVersion = 6 MinorVersion = 2 BuildNumber = 9200
 */


VOID UnloadDriver(PDRIVER_OBJECT obj) {

}


NTSTATUS DriverEntry(PDRIVER_OBJECT obj, PUNICODE_STRING reg) {
	if (SymInitSym() == SK_SUCCESS)
	{
		SKSTATUS ret = ComInitCommmunication();
		//ULONG64 base = 0;
		//KrGetProcModuleBaseByName(1432,"Dbsadgview.exe",&base);
	}
	
	//obj->DriverUnload = UnloadDriver;
	return (NTSTATUS)STATUS_SUCCESS;
}