bits 64

extern printf
extern __itoa
extern __strlen
extern __strcat
extern __strcmp
extern __atoi

section .rodata
  format1       db      "%s",0xA, 0x0
  format2       db      "%d",0xA, 0x0


section .data

  align 8
  sbuff       db      0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0

  tstring     db      "1234567890abcdef",0x0
  tstring1     db     "abcdef",0x0
  tstring2     db     "abcdeg",0x0
  tstring3     db     "1234",0x0

section .bss

section .text

    global  _start

_start:

  ; https://en.wikibooks.org/wiki/X86_Assembly/Interfacing_with_Linux
  ; arguments rdi, rsi, rdx, rcx, r8, r9
  ; https://wiki.osdev.org/System_V_ABI

    mov rdi, 123456789
    mov rsi, sbuff
    mov rdx, 10

    call __itoa

    mov rdi, format1
    mov rsi, sbuff
    call printf

    mov rdi, tstring
    call __strlen
    mov rsi , rax

    mov rdi , format2
    call printf

    mov byte [sbuff], 0x0
    mov rdi, sbuff
    mov rsi, tstring
    call __strcat

    mov rdi, sbuff
    mov rsi, tstring
    call __strcat

    mov rdi, format1
    mov rsi, sbuff
    call printf

    mov rdi, tstring1
    mov rsi, tstring2
    call __strcmp

    mov rdi, format2
    mov rsi, rax
    call printf

    mov rdi, tstring2
    mov rsi, tstring1
    call __strcmp

    mov rdi, format2
    mov rsi, rax
    call printf

    mov rdi, tstring2
    mov rsi, tstring2
    call __strcmp

    mov rdi, format2
    mov rsi, rax
    call printf


    mov rdi, tstring3
    call __atoi

    mov rdi, format2
    mov rsi, rax
    call printf



    mov     rbx, 0            ; exit -> first syscall argument: exit code
    mov     rax, 1            ; system call number (sys_exit)
    int     0x80              ; call kernel
