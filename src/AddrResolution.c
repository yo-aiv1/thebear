#include "../include/ntdll.h"
#include "../include/AddrResolution.h"
#include "../include/hashing.h"


extern PEB *GetPEB();

void *GetDllAddress(unsigned long DllHash) {
    PPEB_LDR_DATA           pLdr            = GetPEB()->Ldr;
    PLDR_DATA_TABLE_ENTRY   EntryPtr        = {0};
    PLIST_ENTRY             ListEntryHead   = {0};

    ListEntryHead = pLdr->InLoadOrderModuleList.Flink;
    while (ListEntryHead != &pLdr->InLoadOrderModuleList) {
        EntryPtr = (PLDR_DATA_TABLE_ENTRY)ListEntryHead;
        if (HashStrW(EntryPtr->BaseDllName.Buffer) == DllHash) {
            return EntryPtr->DllBase;
        }
        
        ListEntryHead = ListEntryHead->Flink;
    }
    return NULL;
}

/* Credits go to Cracked5pider for the following function */
PIMAGE_NT_HEADERS LdrpImageHeader(void *Image) {
    PIMAGE_DOS_HEADER   DosHeader =     {0};
    PIMAGE_NT_HEADERS   NtHeader  =     {0};

    DosHeader = (void*)Image;

    if ( DosHeader->e_magic != IMAGE_DOS_SIGNATURE ) {
        return NULL;
    }

    NtHeader = (void*)( (unsigned long long)Image + DosHeader->e_lfanew );

    if ( NtHeader->Signature != IMAGE_NT_SIGNATURE ) {
        return NULL;
    }

    return NtHeader;
}

/* Credits go to Cracked5pider for the following function */
void *GetFuncAddress(void *DllAddress, unsigned long Function) {
    PIMAGE_NT_HEADERS       NtHeader    = {0};
    PIMAGE_EXPORT_DIRECTORY ExpDir      = {0};
    unsigned long long      ExpDirSize  = {0};
    void                    *Address    = {0};
    unsigned long           *AddrNames  = {0};
    unsigned long           *AddrFuncs  = {0};
    unsigned short          *AddrOrdns  = {0};
    char                    *FuncName   = {0};

    if ( ! DllAddress || ! Function ) {
        return NULL;
    }

    if ( ! ( NtHeader = LdrpImageHeader( DllAddress ) ) ) {
        return NULL;
    }

    ExpDir     = (void*)DllAddress + NtHeader->OptionalHeader.DataDirectory[ IMAGE_DIRECTORY_ENTRY_EXPORT ].VirtualAddress;
    ExpDirSize = NtHeader->OptionalHeader.DataDirectory[ IMAGE_DIRECTORY_ENTRY_EXPORT ].Size;
    AddrNames  = (void*)DllAddress + ExpDir->AddressOfNames;
    AddrFuncs  = (void*)DllAddress + ExpDir->AddressOfFunctions;
    AddrOrdns  = (void*)DllAddress + ExpDir->AddressOfNameOrdinals;

    for ( unsigned long i = 0; i < ExpDir->NumberOfNames; i++ ) {
        FuncName =  (void*)((unsigned long long)DllAddress + AddrNames[ i ]);

        if (HashStr(FuncName) == Function ) {
            Address =  (void*)((unsigned long long)DllAddress + AddrFuncs[ AddrOrdns[ i ] ]);
            return Address;
        }
    }

    return NULL;
}