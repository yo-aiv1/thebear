#ifndef GLOBALVARS
#define GLOBALVARS

#include <windows.h>

/*8*/
typedef struct _HashInfo {
    unsigned long hash;
    int size;
} HashInfo;

/*24*/
typedef struct _SyscallInfo {
    unsigned long SSN;
    unsigned long long SyscallInstruction;
} SyscallInfo;

typedef struct _GlobalsContainer {
    void *NtDll;
    void *NtAllocateVirtualMemoryAddress;
    SyscallInfo NtCreateFile;
    SyscallInfo NtReadFile;
    SyscallInfo NtQueryInformationFile;
    SyscallInfo NtWaitForSingleObject;
    SyscallInfo NtFreeVirtualMemory;
    SyscallInfo NtQueryDirectoryFile;
    struct {
        NTSTATUS (__stdcall *TpAllocWorkFunc) (PTP_WORK, PTP_WORK_CALLBACK, PVOID, PTP_CALLBACK_ENVIRON);
        void (__stdcall *TpPostWorkFunc) (PTP_WORK);
        void (__stdcall *TpReleaseWorkFunc) (PTP_WORK);
    } functions;
} GlobalsContainer;


#endif