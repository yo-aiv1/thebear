#include "../include/AddrResolution.h"
#include "../include/syscalls.h"
#include "../include/FileIO.h"
#include "../include/macros.h"
#include "../include/ntdll.h"
#include "../include/global.h"

extern GlobalsContainer global;

int OpenFileY(PHANDLE FileHandle, ACCESS_MASK AccessValue, POBJECT_ATTRIBUTES ObjectAttributes, ULONG FileStateValue) {
    NTSTATUS        status      = 0;
    IO_STATUS_BLOCK IOstatus    = {0};

    if (global.NtCreateFile.SSN == 0) {
        global.NtCreateFile.SyscallInstruction = (unsigned long long)GetFuncAddress(global.NtDll, (HashInfo){NTCREATEFILE, 12}); + 0x12;
        global.NtCreateFile.SSN = ((PBYTE)(global.NtCreateFile.SyscallInstruction - 0xe))[0];
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

    if (global.NtReadFile.SSN == 0) {
        global.NtReadFile.SyscallInstruction   = (unsigned long long)GetFuncAddress(global.NtDll, (HashInfo){NTREADFILE, 10}) + 0x12;
        global.NtReadFile.SSN = ((PBYTE)(global.NtReadFile.SyscallInstruction - 0xe))[0];
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

    if (global.NtQueryInformationFile.SSN == 0) {
        global.NtQueryInformationFile.SyscallInstruction = (unsigned long long)GetFuncAddress(global.NtDll, (HashInfo){NTQUERYINFORMATIONFILE, 22}) + 0x12;
        global.NtQueryInformationFile.SSN = ((PBYTE)(global.NtQueryInformationFile.SyscallInstruction - 0xe))[0];
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