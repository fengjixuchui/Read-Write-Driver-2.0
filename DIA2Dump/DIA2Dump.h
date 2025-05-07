#include "dia2.h"

extern const wchar_t *g_szFilename;
extern IDiaDataSource *g_pDiaDataSource;
extern IDiaSession *g_pDiaSession;
extern IDiaSymbol *g_pGlobalSymbol;
extern DWORD g_dwMachineType;

void PrintHelpOptions();
bool ParseArg(int , wchar_t *[]);

void Cleanup();
bool LoadDataFromPdb(const wchar_t *, IDiaDataSource **, IDiaSession **, IDiaSymbol **);

void DumpAllPdbInfo(IDiaSession *, IDiaSymbol *);
bool DumpAllMods(IDiaSymbol *);
bool DumpAllPublics(IDiaSymbol *);
bool DumpCompiland(IDiaSymbol *, const wchar_t *);
bool DumpAllSymbols(IDiaSymbol *);
bool DumpAllGlobals(IDiaSymbol *);
bool DumpAllTypes(IDiaSymbol *);
bool DumpAllUDTs(IDiaSymbol *);
bool DumpAllEnums(IDiaSymbol *);
bool DumpAllTypedefs(IDiaSymbol *);
bool DumpAllOEMs(IDiaSymbol *);
bool DumpAllFiles(IDiaSession *, IDiaSymbol *);
bool DumpAllLines(IDiaSession *, IDiaSymbol *);
bool DumpAllLines(IDiaSession *, DWORD, DWORD);
bool DumpAllSecContribs(IDiaSession *);
bool DumpAllDebugStreams(IDiaSession *);
bool DumpAllInjectedSources(IDiaSession *);
bool DumpInjectedSource(IDiaSession *, const wchar_t *);
bool DumpAllSourceFiles(IDiaSession *, IDiaSymbol *);
bool DumpAllFPO(IDiaSession *);
bool DumpFPO(IDiaSession *, DWORD);
bool DumpFPO(IDiaSession *, IDiaSymbol *, const wchar_t *);
bool DumpSymbolWithRVA(IDiaSession *, DWORD, const wchar_t *);
bool DumpSymbolsWithRegEx(IDiaSymbol *, const wchar_t *, const wchar_t *);
bool DumpSymbolWithChildren(IDiaSymbol *, const wchar_t *);
bool DumpLines(IDiaSession *, DWORD);
bool DumpLines(IDiaSession *, IDiaSymbol *, const wchar_t *);
bool DumpType(IDiaSymbol *, const wchar_t *);
bool DumpLinesForSourceFile(IDiaSession *, const wchar_t *, DWORD);
bool DumpPublicSymbolsSorted(IDiaSession *, DWORD, DWORD, bool);
bool DumpLabel(IDiaSession *, DWORD);
bool DumpAnnotations(IDiaSession *, DWORD);
bool DumpMapToSrc(IDiaSession *, DWORD);
bool DumpMapFromSrc(IDiaSession *, DWORD);

HRESULT GetTable(IDiaSession *, REFIID, void **);

///////////////////////////////////////////////////////////////////
// Functions defined in regs.cpp
const wchar_t *SzNameC7Reg(USHORT, DWORD);
const wchar_t *SzNameC7Reg(USHORT);




/*=====================================分割线：二次开发==================================*/
bool ExpLoadPdbFile(const wchar_t* szFilename);
void ExpUnLoadPdbFile();
bool InitSymbolService(
    IDiaDataSource** ppSource,
    IDiaSession** ppSession,
    IDiaSymbol** ppGlobal);
DWORD ExpGetRvaBySymbolName(const wchar_t* szFilename, int index);
void ExpGetSymbolNameByRva(ULONG64 rva, const wchar_t** szFilename);
bool ExpDownloadPdbTo(const char* filename, const char* pdbPath);
DWORD ExpGetMemberOffsetOfStruct(const wchar_t* structName, const wchar_t* memName, int structIndex, int memIndex);