#include "../include/MemoryUtils.h"
#include "../include/AddrResolution.h"
#include "../include/ntdll.h"
#include "../include/macros.h"
#include "../include/syscalls.h"

#include <windows.h>


typedef unsigned __int64 QWORD;

extern NTSTATUS    (NTAPI *pTpAllocWork)           (PTP_WORK, PTP_WORK_CALLBACK, PVOID, PTP_CALLBACK_ENVIRON);
extern void        (NTAPI *pTpPostWork)            (PTP_WORK);
extern void        (NTAPI *pTpReleaseWork)         (PTP_WORK);

extern void CALLBACK NtAllocWorkCallback(PTP_CALLBACK_INSTANCE Instance, PVOID Context, PTP_WORK Work);

typedef struct _NTALLOC_ARGS {
    UINT_PTR    pNtAllocateVirtualMemory;
    HANDLE      hProcess;
    PVOID       *address;
    PSIZE_T     size;
} NTALLOC_ARGS, *PNTALLOC_ARGS;

extern DWORD NtWaitForSingleObjectSSN;
extern QWORD NtWaitForSingleObjectSyscall;
extern DWORD NtFreeVirtualMemorySSN;
extern QWORD NtFreeVirtualMemorySyscall;
extern void *pNTDLL;

void *AllocMemory(SIZE_T size) {
    NTALLOC_ARGS    NtAllocArgs         = {0};
    PTP_WORK        WorkReturn          = {0};
    LARGE_INTEGER   WaitTime            = {0};
    void            *AllocatedMemoryPtr = {0};

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

    NtAllocArgs.pNtAllocateVirtualMemory    = (UINT_PTR)GetFuncAddress(pNTDLL, NTALLOCMEM);
    NtAllocArgs.hProcess                    = (HANDLE)-1;
    NtAllocArgs.address                     = &AllocatedMemoryPtr;
    NtAllocArgs.size                        = &size;

    pTpAllocWork(&WorkReturn, (PTP_WORK_CALLBACK)NtAllocWorkCallback, &NtAllocArgs, NULL);
    pTpPostWork(WorkReturn);
    pTpReleaseWork(WorkReturn);

    WaitTime.QuadPart = -1000 * 10000;
    if (NtWaitForSingleObjectSSN == 0 || NtWaitForSingleObjectSyscall == 0) {
        if (pNTDLL == NULL) {
            pNTDLL = GetDllAddress(NTDHASH);
        }

        NtWaitForSingleObjectSyscall    = GetFuncAddress(pNTDLL, NTWAITOBJ) + 0x12;
        NtWaitForSingleObjectSSN        = ((PBYTE)(NtWaitForSingleObjectSyscall - 0xe))[0];
    }
    NtWaitForSingleObject((HANDLE)-1, FALSE, &WaitTime);

    return AllocatedMemoryPtr;
}


int FreeMemory(void *buffer) {
    NTSTATUS    status  = 0;
    SIZE_T      size    = 0;

    if (NtFreeVirtualMemorySSN == 0 || NtFreeVirtualMemorySyscall == 0) {
        if (pNTDLL == NULL) {
            pNTDLL = GetDllAddress(NTDHASH);
        }
        NtFreeVirtualMemorySyscall  = GetFuncAddress(pNTDLL, NTFREEMEM) + 0x12;
        NtFreeVirtualMemorySSN     = ((PBYTE)(NtFreeVirtualMemorySyscall - 0xe))[0];
    }

    status = NtFreeVirtualMemory((HANDLE)-1, &buffer, &size, MEM_RELEASE);

    if (status != 0x00000000) {
        return 1;
    }

    return 0;
}

void SetMemory(void *ptr, int c, int size) {
    register unsigned char *src = (unsigned char*)ptr;
    int i;

    for (i = 0; i <= size - 4; i += 4) {
        *src = c;
        *(src + 1) = c;
        *(src + 2) = c;
        *(src + 3) = c;
        src += 4;
    }

    for (; i < size; i++) {
        *src = c;
        src++;
    }
}


void MovMemory(unsigned char *src, unsigned char *dest, int size) {
    int i;

    for (i = 0; i <= size - 4; i += 4) {
        *dest = *src;
        *(dest + 1) = *(src + 1);
        *(dest + 2) = *(src + 2);
        *(dest + 3) = *(src + 3);
        dest += 4;
        src += 4;
    }

    for (; i < size; i++) {
        *dest = *src;
        dest++;
        src++;
    }
}