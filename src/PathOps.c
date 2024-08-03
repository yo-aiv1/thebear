#include "../include/ntdll.h"
#include "../include/PathOps.h"
#include "../include/hashing.h"
#include "../include/StringUtils.h"
#include "../include/macros.h"

#include <windows.h>

extern PEB *GetPEB();

unsigned short LOCALAPPDATA[160]  = {0};

int ParseEnv(unsigned short *buffer, unsigned long EnvHash, int size) {
    unsigned short       *FullEnvVar     = GetPEB()->ProcessParameters->Environment;
    unsigned short       EnvNameTemp[13] = {0};

    buffer += 4;
    while (*FullEnvVar) {
        for (int j = 0; j < size; j++) {
            if (*FullEnvVar == 0x003d) {
                EnvNameTemp[j] = 0x0000;
                if (HashStrW(EnvNameTemp) == EnvHash) {
                    *FullEnvVar++;
                    while (*FullEnvVar != 0x0000) {
                        *buffer = *FullEnvVar;
                        *FullEnvVar++;
                        *buffer++;
                    }
                    *buffer = 0x0000;
                    return 0;
                }
                break;
            }
            EnvNameTemp[j] = *FullEnvVar;
            FullEnvVar++;
        }
        FullEnvVar += lenW(FullEnvVar) + 1;
    }
    return 1;
}


int InitPath(unsigned short *FilePath, unsigned short *OutBuffer) {
    int idx = 0;

    if (LOCALAPPDATA[0] == 0x0000) {
        LOCALAPPDATA[0]     = '\\';
        LOCALAPPDATA[1]     = '?';
        LOCALAPPDATA[2]     = '?';
        LOCALAPPDATA[3]     = '\\';
        if (ParseEnv(LOCALAPPDATA, LOCALAPP, 13) == 1) {
            return 1;
        }
    }

    while (LOCALAPPDATA[idx] != '\0') {
        *OutBuffer = LOCALAPPDATA[idx];
        OutBuffer++;
        idx++;
    }


    while (*FilePath) {
        *OutBuffer = *FilePath;
        *OutBuffer++;
        *FilePath++;
    }
    *OutBuffer = 0x0000;
    return 0;
}


void InitPathObj(unsigned short *SourceString, POBJECT_ATTRIBUTES ObjectAttributes, PUNICODE_STRING PathInfo) {
    PathInfo->MaximumLength     = (PathInfo->Length = (USHORT)(lenW(SourceString) * sizeof(WCHAR))) + sizeof(UNICODE_NULL);
    PathInfo->Buffer            = (PWCH)SourceString;

    ObjectAttributes->Length                    = sizeof(OBJECT_ATTRIBUTES);
    ObjectAttributes->Attributes                = OBJ_CASE_INSENSITIVE;
    ObjectAttributes->ObjectName                = PathInfo;
    ObjectAttributes->RootDirectory             = NULL;
    ObjectAttributes->SecurityDescriptor        = NULL;
    ObjectAttributes->SecurityQualityOfService  = NULL;
}