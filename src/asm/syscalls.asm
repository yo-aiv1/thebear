extern global

section .text
    global NtCreateFile
    global NtReadFile
    global NtQueryInformationFile
    global NtWaitForSingleObject
    global NtFreeVirtualMemory
    global NtQueryDirectoryFile


NtCreateFile:
    mov r10, rcx
    mov eax, [rel global + 16]
    jmp qword [rel global + 24]
    ret

NtReadFile:
    mov r10, rcx
    mov eax, [rel global + 32]
    jmp qword [rel global + 40]
    ret

NtQueryInformationFile:
    mov r10, rcx
    mov eax, [rel global + 48]
    jmp qword [rel global + 56]
    ret

NtWaitForSingleObject:
    mov r10, rcx
    mov eax, [rel global + 64]
    jmp qword [rel global + 72]
    ret

NtFreeVirtualMemory:
    mov r10, rcx
    mov eax, [rel global + 80]
    jmp qword [rel global + 88]
    ret

NtQueryDirectoryFile:
    mov r10, rcx
    mov eax, [rel global + 96]
    jmp qword [rel global + 104]
    ret