extern NtCreateFileSSN
extern NtCreateFileSyscall
extern NtReadFileSSN
extern NtReadFileSyscall
extern NtQueryInformationFileSSN
extern NtQueryInformationFileSyscall
extern NtWaitForSingleObjectSSN
extern NtWaitForSingleObjectSyscall
extern NtFreeVirtualMemorySSN
extern NtFreeVirtualMemorySyscall
extern NtQueryDirectoryFileSSN
extern NtQueryDirectoryFileSyscall


section .text
    global NtCreateFile
    global NtQueryInformationFile
    global NtReadFile
    global NtWaitForSingleObject
    global NtFreeVirtualMemory
    global NtQueryDirectoryFile


NtCreateFile:
    mov r10, rcx
    mov eax, [rel NtCreateFileSSN]
    jmp qword [rel NtCreateFileSyscall]
    ret

NtReadFile:
    mov r10, rcx
    mov eax, [rel NtReadFileSSN]
    jmp qword [rel NtReadFileSyscall]
    ret

NtQueryInformationFile:
    mov r10, rcx
    mov eax, [rel NtQueryInformationFileSSN]
    jmp qword [rel NtQueryInformationFileSyscall]
    ret

NtWaitForSingleObject:
    mov r10, rcx
    mov eax, [rel NtWaitForSingleObjectSSN]
    jmp qword [rel NtWaitForSingleObjectSyscall]
    ret

NtFreeVirtualMemory:
    mov r10, rcx
    mov eax, [rel NtFreeVirtualMemorySSN]
    jmp qword [rel NtFreeVirtualMemorySyscall]
    ret

NtQueryDirectoryFile:
    mov r10, rcx
    mov eax, [rel NtQueryDirectoryFileSSN]
    jmp qword [rel NtQueryDirectoryFileSyscall]
    ret