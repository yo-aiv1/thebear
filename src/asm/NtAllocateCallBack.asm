section .text
    global NtAllocWorkCallback

NtAllocWorkCallback:
    mov rbx, rdx
    mov rax, [rbx]
    mov rcx, [rbx + 0x8]
    mov rdx, [rbx + 0x10]
    xor r8, r8
    mov r9, [rbx + 0x18]
    mov r10, 0x04
    mov [rsp+0x30], r10
    mov r10, 0x3000
    mov [rsp+0x28], r10
    jmp rax