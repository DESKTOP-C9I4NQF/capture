#include <stdio.h>
#include <string.h>


size_t shellcode(void* ptr, size_t size)
{
    __asm__ volatile (
        ".intel_syntax noprefix;"

        "shellcode_start:;"

        "xor eax, eax;"
        "mov al, "
        "syscall"
        


        "shellcode_end:;"
        "lea rax, [rip+shellcode_start];"
        "lea rdi, [rip+shellcode_end];"
        "sub rdi, rax;"
        "mov %1, rdi;"
        "lea rax, [rip+shellcode_start];"
        "mov %0, rax;"

        ".att_syntax;"
        : "=r" (ptr), "=r" (size));
    return 0;
}
int main(int argc, char *argv[])
{
    shellcode(NULL, 0);
    return 0;
}