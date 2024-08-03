#include "../include/AddrResolution.h"
#include "../include/syscalls.h"
#include "../include/FileIO.h"
#include "../include/macros.h"
#include "../include/ntdll.h"


typedef unsigned __int64 QWORD;

extern void *pNTDLL;
extern DWORD NtCreateFileSSN;
extern QWORD NtCreateFileSyscall;
extern DWORD NtReadFileSSN;
extern QWORD NtReadFileSyscall;
extern DWORD NtQueryInformationFileSSN;
extern QWORD NtQueryInformationFileSyscall;

int OpenFileY(PHANDLE FileHandle, ACCESS_MASK AccessValue, POBJECT_ATTRIBUTES ObjectAttributes, ULONG FileStateValue) {
    NTSTATUS        status      = 0;
    IO_STATUS_BLOCK IOstatus    = {0};

    if (NtCreateFileSSN == 0) {
        if (pNTDLL == NULL) {
            pNTDLL = GetDllAddress(NTDHASH);
            if (pNTDLL == NULL) {
                return 1;
            }
        }

        NtCreateFileSyscall = (QWORD)GetFuncAddress(pNTDLL, NTCREATEFILE); + 0x12;
        NtCreateFileSSN     = ((PBYTE)(NtCreateFileSyscall - 0xe))[0];
    }

    status = NtCreateFile(
        FileHandle,
        AccessValue,
        ObjectAttributes,
        &IOstatus,
        NULL,
        FILE_ATTRIBUTE_NORMAL,
        FILE_SHARE_READ,
        FileStateValue,
        FILE_SYNCHRONOUS_IO_NONALERT,
        NULL,
        0
    );
    if (status != 0x00000000) {
        return 1;
    }
    return 0;
}

int ReadFileY(HANDLE FileHandle, unsigned char *buffer, ULONG BufferSize) {
    NTSTATUS        status      = 0;
    IO_STATUS_BLOCK IOstatus    = {0};

    if (NtReadFileSSN == 0) {
        if (pNTDLL == NULL) {
            pNTDLL = GetDllAddress(NTDHASH);
            if (pNTDLL == NULL) {
                return 1;
            }
        }
        NtReadFileSyscall   = (QWORD)GetFuncAddress(pNTDLL, NTREADFILE) + 0x12;
        NtReadFileSSN       = ((PBYTE)(NtReadFileSyscall - 0xe))[0];
    }

    status = NtReadFile(
            FileHandle,
            NULL,
            NULL,
            NULL,
            &IOstatus,
            buffer,
            BufferSize,
            NULL,
            NULL
    );
    if (status != 0x00000000) {
        return 1;
    }
    return 0;
}

int GetFileSizeY(HANDLE FileHandle) {
    FILE_STANDARD_INFORMATION   standardInfo    = {0};
    IO_STATUS_BLOCK             IOstatus        = {0};
    NTSTATUS                    status          = 0;

    if (NtQueryInformationFileSSN == 0) {
        if (pNTDLL == NULL) {
            pNTDLL = GetDllAddress(NTDHASH);
            if (pNTDLL == NULL) {
                return 1;
            }
        }
        NtQueryInformationFileSyscall   = GetFuncAddress(pNTDLL, NTQINFOFILE) + 0x12;
        NtQueryInformationFileSSN       = ((PBYTE)(NtQueryInformationFileSyscall - 0xe))[0];
    }

    status = NtQueryInformationFile(
        FileHandle,
        &IOstatus,
        &standardInfo,
        sizeof(standardInfo),
        FileStandardInformation
    );

    if (status != 0x00000000) {
        return 1;
    }

    return (int)standardInfo.EndOfFile.QuadPart;
}