#pragma once
#include "comm.h"



bool ExpLoadPdbFile(const wchar_t* szFilename);
void ExpUnLoadPdbFile();
DWORD ExpGetRvaBySymbolName(const wchar_t* szFilename, int index);
bool ExpDownloadPdbTo(const char* filename, const char* pdbPath);
DWORD ExpGetMemberOffsetOfStruct(const wchar_t* structName, const wchar_t* memName, int structIndex, int memIndex);

typedef struct _SYM_INFO {
	BYTE mod;
	BYTE type;
	BYTE name1;
	BYTE name2;
	DWORD off;
}SYM_INFO,*PSYM_INFO;


#define SYM_TYPE_FUNC								((BYTE)0x0)
#define SYM_TYPE_STRUCT								((BYTE)0x1)

#define SYM_MOD_NTOS								((BYTE)0x0)
#define SYM_MOD_D2D1								((BYTE)0x1)
#define SYM_MOD_DXGI								((BYTE)0x2)



#define SYM_FUNC_NTCBACAP	((BYTE)0)	//NtConvertBetweenAuxiliaryCounterAndPerformanceCounter
#define SYM_FUNC_NTSUSPENDTHREAD	((BYTE)1)
#define SYM_FUNC_NTRESUMETHREAD	((BYTE)2)
#define SYM_FUNC_ExpLookupHandleTableEntry ((BYTE)3)	//ExpLookupHandleTableEntry
#define SYM_FUNC_PspCidTable  ((BYTE)4)
#define SYM_FUNC_NtQueryVirtualMemory ((BYTE)5)
#define SYM_FUNC_VirtualProtect ((BYTE)6)
#define SYM_FUNC_MiLocateAddress ((BYTE)7)

#define SYM_FUNC_PresentDWM	((BYTE)0)
#define SYM_FUNC_PresentDWM2	((BYTE)1)
#define SYM_FUNC_PresentMultiplaneOverlay ((BYTE)2)
#define SYM_FUNC_PresentMultiplaneOverlay2 ((BYTE)3)
#define SYM_FUNC_Win7Present ((BYTE)4)

#define SYM_STRUCT_EPROCESS	((BYTE)0)
#define SYM_STRUCT_KTHREAD	((BYTE)1)
#define SYM_STRUCT_KAPC_STATE	((BYTE)2)
#define SYM_STRUCT_KPRCB	((BYTE)3)

#define SYM_STRUCTMEM_EPROCESS_WoW64Process	((BYTE)0)
#define SYM_STRUCTMEM_EPROCESS_UniqueProcessId	((BYTE)1)
#define SYM_STRUCTMEM_EPROCESS_InheritedFromUniqueProcessId	((BYTE)2)
#define SYM_STRUCTMEM_EPROCESS_ImageFileName	((BYTE)3)
#define SYM_STRUCTMEM_KTHREAD_ApcState	((BYTE)4)
#define SYM_STRUCTMEM_KTHREAD_Teb	((BYTE)5)
#define SYM_STRUCTMEM_KTHREAD_TrapFrame	((BYTE)6)
#define SYM_STRUCTMEM_KAPC_STATE_Process	((BYTE)7)
#define SYM_STRUCTMEM_KPRCB_CurrentThread	((BYTE)8)
#define SYM_STRUCTMEM_KTHREAD_InitialStack	((BYTE)9)
#define SYM_STRUCTMEM_EPROCESS_ImagePathHash	((BYTE)10)
#define SYM_STRUCTMEM_EPROCESS_SeAuditProcessCreationInfo	((BYTE)11)
#define SYM_STRUCTMEM_EPROCESS_SectionBaseAddress	((BYTE)12)
#define SYM_STRUCTMEM_EPROCESS_SectionObject	((BYTE)13)

BOOLEAN SymInitDatFile();
