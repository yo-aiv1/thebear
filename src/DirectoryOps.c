#include "../include/AddrResolution.h"
#include "../include/DirectoryOps.h"
#include "../include/syscalls.h"
#include "../include/PathOps.h"
#include "../include/macros.h"
#include "../include/ntdll.h"
#include "../include/global.h"

#include <windows.h>

extern GlobalsContainer global;

int OpenFolder(PHANDLE FileHandle, unsigned short *path) {
    UNICODE_STRING      PathUnicode     = {0};
    OBJECT_ATTRIBUTES   PathObj         = {0};
    IO_STATUS_BLOCK     IOstatus        = {0};
    NTSTATUS            status          = 0;

    if (global.NtCreateFile.SSN == 0) {
        global.NtCreateFile.SyscallInstruction = (unsigned long long)GetFuncAddress(global.NtDll, (HashInfo){NTCREATEFILE, 12}); + 0x12;
        global.NtCreateFile.SSN = ((PBYTE)(global.NtCreateFile.SyscallInstruction - 0xe))[0];
    }

    InitPathObj(path, &PathObj, &PathUnicode);

    status = NtCreateFile(
        FileHandle,
        FILE_LIST_DIRECTORY | SYNCHRONIZE,
        &PathObj,
        &IOstatus,
        NULL,
        FILE_ATTRIBUTE_DIRECTORY,
        FILE_SHARE_READ,
        FILE_OPEN,
        FILE_DIRECTORY_FILE | FILE_SYNCHRONOUS_IO_NONALERT,
        NULL,
        0
    );

    if (status != 0x00000000) {
        return 1;
    }
    return 0;
}

int ReadFolder(unsigned char *OutBuffer, unsigned short *path) {
    HANDLE              FolderHandle    = {0};
    IO_STATUS_BLOCK     IOstatus        = {0};

    if (OpenFolder(&FolderHandle, path) != 0) {
        return 1;
    }

    if (global.NtQueryDirectoryFile.SSN == 0) {
        global.NtQueryDirectoryFile.SyscallInstruction = (unsigned long long)GetFuncAddress(global.NtDll, (HashInfo){NTQUERYDIRECTORYFILE, 20}) + 0x12;
        global.NtQueryDirectoryFile.SSN = ((PBYTE)(global.NtQueryDirectoryFile.SyscallInstruction - 0xe))[0];
    }

    NTSTATUS status = NtQueryDirectoryFile(
        FolderHandle,
        NULL,
        NULL,
        NULL,
        &IOstatus,
        OutBuffer,
        4096,
        FileDirectoryInformation,
        FALSE,
        NULL, 
        FALSE
    );

    if (status != 0x00000000) {
        return 1;
    }

    return 0;
}