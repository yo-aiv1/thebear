section .text
    global LdrLoadWorkCallback

LdrLoadWorkCallback:
    mov rbx, rdx
    mov rax, [rbx]
    mov rcx, [rbx + 0x8]
    mov rdx, [rbx + 0x10]
    mov r8, [rbx + 0x18]
    mov r9, [rbx + 0x20]
    mov r10, [rbx + 0x20]
    mov [rsp+0x28], r10
    jmp rax
