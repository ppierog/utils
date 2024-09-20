bits 64

extern printf
extern sprintf

section .rodata
  format        db      "%lld",0xA, 0x0
  printf_format db      "%lld %lld",0xA, 0x0

section .data

  align 8
  sbuff       db      0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0

section .bss

  ;struct timespec {
  ;    time_t   tv_sec;        /* seconds */
  ;    long     tv_nsec;       /* nanoseconds */
  ;};
  time1: resq 2   ; 2 qwords for seconds and nanoseconds
  time2: resq 2   ; 2 qwords for seconds and nanoseconds

section .text

    global  _start

_start:

  ; https://en.wikibooks.org/wiki/X86_Assembly/Interfacing_with_Linux
  ; arguments rdi, rsi, rdx, rcx, r8, r9
  ; https://wiki.osdev.org/System_V_ABI


  mov rax, 228              ; 228 is system call for sys_clock_gettime
  xor rdi, rdi              ; 0 for system clock (preferred over "mov rdi, 0")
  lea rsi, [time1]


  syscall                   ; [time1] contains number of seconds
                            ; [time1 + 8] contains number of nanoseconds


  mov  rbp, 100000000       ; use EBP as counter 
  mov  r15 , 0              ; use r15 as cumulative sum

  .loop:
      add r15, rbp          ; add in a loop
      dec  ebp
  jnz  .loop

  mov rax, 228 
  xor edi, edi
  lea rsi, [time2]
  syscall
  
  mov rdi, sbuff
  mov rsi, format
  mov rdx, r15

  call sprintf               ; sprintf(target_string,"%d",source_int) 
  
  mov rdi, sbuff
  call printf

  mov r10,  [time2+8]        ; calculate nanoseconds for between timestamps 
  mov r9,   [time1+8]
  cmp r10, r9

  jle .lequal
    mov r14, r10
    sub r14, r9 
  jmp .end_diff

  .lequal:
    mov r14, 0xFFFFFFFF
    sub r14, r9
    add r14, r10

  .end_diff:

  mov long [sbuff],0
  mov long [sbuff+8],0

  mov rdi, sbuff
  mov rsi, format
  mov rdx, r14

  call sprintf
  mov rdi, sbuff
  call printf


  ;mov rdi, printf_format
  ;mov rsi, r15
  ;mov rdx, r14
  ;call printf

  mov     rbx, 42            ; exit -> first syscall argument: exit code
  mov     rax, 1             ; system call number (sys_exit)
  int     0x80               ; call kernel
