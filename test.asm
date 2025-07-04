global _start
_start:
    mov rax, 60 ; Rax specifies which syscall to call '60' is for exit
    mov rdi, 69 ; Rdi is of limited size, refer syscall linux in google
    syscall