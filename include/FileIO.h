#ifndef FILE_IO
#define FILE_IO

#include "../include/ntdll.h"

int OpenFileY(
        PHANDLE FileHandle,
        ACCESS_MASK AccessValue,
        POBJECT_ATTRIBUTES ObjectAttributes,
        ULONG FileStateValue
    );

int ReadFileY(
        HANDLE FileHandle,
        unsigned char *buffer,
        ULONG BufferSize
    );

int GetFileSizeY(
        HANDLE FileHandle
    );

#endif