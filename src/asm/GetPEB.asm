section .text
    global GetPEB

GetPEB:
    mov rax, qword gs:[0x60]
    ret
