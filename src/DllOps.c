#include "../include/AddrResolution.h"
#include "../include/StringUtils.h"
#include "../include/syscalls.h"
#include "../include/macros.h"
#include "../include/DllOps.h"
#include "../include/ntdll.h"
#include "../include/decoding.h"
#include "../include/global.h"


extern void CALLBACK LdrLoadWorkCallback(PTP_CALLBACK_INSTANCE Instance, PVOID Context, PTP_WORK Work);

typedef struct _LDRLOADDLL_ARGS {
    UINT_PTR        pLdrLoadDll;
    PWSTR           DllPath;
    PWSTR           DllCharacteristics;
    PUNICODE_STRING DllName;
    PVOID           *DllHandle;
} LDRLOADDLL_ARGS;


extern GlobalsContainer global;


void *LoadDll(unsigned short *DllName) {
    void            *DllAddress     = NULL;
    UNICODE_STRING  DllInfo         = {0};
    LDRLOADDLL_ARGS LdrLoadDllArgs  = {0};
    LARGE_INTEGER   WaitTime        = {0};
    PTP_WORK        WorkReturn      = NULL;

    if (global.functions.TpAllocWorkFunc == 0) {
        global.functions.TpAllocWorkFunc    = GetFuncAddress(global.NtDll, (HashInfo){TPALLOCWORK, 11});
        global.functions.TpPostWorkFunc     = GetFuncAddress(global.NtDll, (HashInfo){TPPOSTWORK, 10});
        global.functions.TpReleaseWorkFunc  = GetFuncAddress(global.NtDll, (HashInfo){TPRELEASEWORK, 13});
    }

    DecodeStringW(DllName);
    DllInfo.Buffer          = DllName;
    DllInfo.MaximumLength   = (DllInfo.Length = (USHORT)(lenW(DllName) * sizeof(WCHAR))) + sizeof(UNICODE_NULL);

    LdrLoadDllArgs.pLdrLoadDll          = (UINT_PTR) GetFuncAddress(global.NtDll, (HashInfo){LDRLOADDLL, 10});
    LdrLoadDllArgs.DllPath              = NULL;
    LdrLoadDllArgs.DllCharacteristics   = NULL;
    LdrLoadDllArgs.DllName              = &DllInfo;
    LdrLoadDllArgs.DllHandle            = &DllAddress;

    global.functions.TpAllocWorkFunc(&WorkReturn, (PTP_WORK_CALLBACK)LdrLoadWorkCallback, &LdrLoadDllArgs, NULL);
    global.functions.TpPostWorkFunc(WorkReturn);
    global.functions.TpReleaseWorkFunc(WorkReturn);

    WaitTime.QuadPart = -1500 * 10000;
    if (global.NtWaitForSingleObject.SyscallInstruction == 0) {
        global.NtWaitForSingleObject.SyscallInstruction = GetFuncAddress(global.NtDll, (HashInfo){NTWAITFORSINGLEOBJECT, 21}) + 0x12;
        global.NtWaitForSingleObject.SSN = ((PBYTE)(global.NtWaitForSingleObject.SyscallInstruction - 0xe))[0];
    }
    NtWaitForSingleObject((HANDLE)-1, FALSE, &WaitTime);

    return DllAddress;
}