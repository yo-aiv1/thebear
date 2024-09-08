#include <windows.h>
#include <stdio.h>
#include "include/global.h"

/* extern void print(); */
SyscallInfo data = {0};
int main(void) {
    data.SSN = 10;
    data.SyscallInstruction = 12220;

/*     print(); */
printf("Offset of NtCreateFile: %zu\n", offsetof(GlobalsContainer, NtCreateFile.SSN));
printf("Offset of NtCreateFile: %zu\n", offsetof(GlobalsContainer, NtCreateFile.SyscallInstruction));
printf("Offset of NtReadFile: %zu\n", offsetof(GlobalsContainer, NtReadFile.SSN));
printf("Offset of NtReadFile: %zu\n", offsetof(GlobalsContainer, NtReadFile.SyscallInstruction));
printf("Offset of NtQueryInformationFile: %zu\n", offsetof(GlobalsContainer, NtQueryInformationFile.SSN));
printf("Offset of NtQueryInformationFile: %zu\n", offsetof(GlobalsContainer, NtQueryInformationFile.SyscallInstruction));
printf("Offset of NtWaitForSingleObject: %zu\n", offsetof(GlobalsContainer, NtWaitForSingleObject.SSN));
printf("Offset of NtWaitForSingleObject: %zu\n", offsetof(GlobalsContainer, NtWaitForSingleObject.SyscallInstruction));
printf("Offset of NtFreeVirtualMemory: %zu\n", offsetof(GlobalsContainer, NtFreeVirtualMemory.SSN));
printf("Offset of NtFreeVirtualMemory: %zu\n", offsetof(GlobalsContainer, NtFreeVirtualMemory.SyscallInstruction));
printf("Offset of NtQueryDirectoryFile: %zu\n", offsetof(GlobalsContainer, NtQueryDirectoryFile.SSN));
printf("Offset of NtQueryDirectoryFile: %zu\n", offsetof(GlobalsContainer, NtQueryDirectoryFile.SyscallInstruction));

}


/* extern PEB *GetPEB();
int main(void) {
    PPEB_LDR_DATA           pLdr            = GetPEB()->Ldr;
    PLDR_DATA_TABLE_ENTRY   EntryPtr        = {0};
    PLIST_ENTRY             ListEntryHead   = {0};

    ListEntryHead = pLdr->InLoadOrderModuleList.Flink;
    while (ListEntryHead != &pLdr->InLoadOrderModuleList) {
        EntryPtr = (PLDR_DATA_TABLE_ENTRY)ListEntryHead;
        wprintf(L"name %s, size %i\n", EntryPtr->BaseDllName.Buffer, EntryPtr->BaseDllName.Length);
        ListEntryHead = ListEntryHead->Flink;
    }
    return NULL;
} */


/* void encode(unsigned char *str) {
    int TempKey = 5;

    while (*str != 0x00) {
        if (TempKey < 0) {
            TempKey = TempKey * -1;
        } else if (TempKey == 0) {
            TempKey = 5;
        }

        unsigned char CurrentChar = *str;

        *str = *str + TempKey;
        TempKey = CurrentChar - TempKey;
        str++;
    }
}

void decode(unsigned char *str) {
    int TempKey = 5;

    while (*str != 0x00) {
        if (TempKey < 0) {
            TempKey = TempKey * -1;
        } else if (TempKey == 0) {
            TempKey = 5;
        }

        *str = *str - TempKey;
        TempKey = *str - TempKey;
        str++;
    }
}


int main(void) {
    unsigned char str[13] = "hello world!";
    encode((unsigned char *)&str);
    int i = 0;
    while (str[i] != 0x00) {
        printf("%i ", str[i]);
        i++;
    }
    printf("\n");
    i = 0;
    decode((unsigned char *)&str);
    while (str[i] != 0x00) {
        printf("%i ", str[i]);
        i++;
    }
    printf("\n");
} */