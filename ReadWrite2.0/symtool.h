#pragma once
#include "errorCode.h"


typedef struct _SYM_INFO {
	CHAR mod;
	CHAR type;
	CHAR name1;
	CHAR name2;
	ULONG32 off;
}SYM_INFO, * PSYM_INFO;


#define SYM_TYPE_FUNC								((CHAR)0x0)
#define SYM_TYPE_STRUCT								((CHAR)0x1)

#define SYM_MOD_NTOS								((CHAR)0x0)
#define SYM_MOD_D2D1								((CHAR)0x1)
#define SYM_MOD_DWMCORE								((CHAR)0x2)


#define SYM_FUNC_NTCBACAP	((CHAR)0)	//NtConvertBetweenAuxiliaryCounterAndPerformanceCounter
#define SYM_FUNC_NTSUSPENDTHREAD	((CHAR)1)
#define SYM_FUNC_NTRESUMETHREAD	((CHAR)2)
#define SYM_FUNC_ExpLookupHandleTableEntry ((CHAR)3)	//ExpLookupHandleTableEntry
#define SYM_FUNC_PspCidTable  ((CHAR)4)
#define SYM_FUNC_NtQueryVirtualMemory ((CHAR)5)
#define SYM_FUNC_VirtualProtect ((CHAR)6)
#define SYM_FUNC_MiLocateAddress ((CHAR)7)

#define SYM_FUNC_PresentDWM	((CHAR)0)
#define SYM_FUNC_PresentDWM2	((CHAR)1)
#define SYM_FUNC_PresentMultiplaneOverlay ((CHAR)2)
#define SYM_FUNC_PresentMultiplaneOverlay2 ((CHAR)3)
#define SYM_FUNC_Win7Present ((CHAR)4)
#define SYM_FUNC_Win7PresentWithD3D ((CHAR)5)

#define SYM_STRUCT_EPROCESS	((CHAR)0)
#define SYM_STRUCT_KTHREAD	((CHAR)1)
#define SYM_STRUCT_KAPC_STATE	((CHAR)2)
#define SYM_STRUCT_KPRCB	((CHAR)3)

#define SYM_STRUCTMEM_EPROCESS_WoW64Process	((CHAR)0)
#define SYM_STRUCTMEM_EPROCESS_UniqueProcessId	((CHAR)1)
#define SYM_STRUCTMEM_EPROCESS_InheritedFromUniqueProcessId	((CHAR)2)
#define SYM_STRUCTMEM_EPROCESS_ImageFileName	((CHAR)3)
#define SYM_STRUCTMEM_KTHREAD_ApcState	((CHAR)4)
#define SYM_STRUCTMEM_KTHREAD_Teb	((CHAR)5)
#define SYM_STRUCTMEM_KTHREAD_TrapFrame	((CHAR)6)
#define SYM_STRUCTMEM_KAPC_STATE_Process	((CHAR)7)
#define SYM_STRUCTMEM_KPRCB_CurrentThread	((CHAR)8)
#define SYM_STRUCTMEM_KTHREAD_InitialStack	((CHAR)9)
#define SYM_STRUCTMEM_EPROCESS_ImagePathHash	((CHAR)10)
#define SYM_STRUCTMEM_EPROCESS_SeAuditProcessCreationInfo	((CHAR)11)
#define SYM_STRUCTMEM_EPROCESS_SectionBaseAddress	((CHAR)12)
#define SYM_STRUCTMEM_EPROCESS_SectionObject	((CHAR)13)

SKSTATUS SymInitSym();
ULONG64 SymGetFuncOffOfMod(CHAR mod, CHAR func);
ULONG64 SymGetMemOffOfStruct(CHAR mod, CHAR structName, CHAR mem);