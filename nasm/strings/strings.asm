bits 64

; https://gitlab.com/x86-psABIs/x86-64-ABI/-/jobs/artifacts/master/raw/x86-64-ABI/abi.pdf?job=build
; %rdi, %rsi, %rdx, %rcx, %r8 and %r9 are useed for passing arguments to functions

section .rodata
  lut           db      "0123456789ABCDEF"

section .text

    global __itoa
    global __strlen
    global __strcat
    global __strcmp
    global __atoi
    global __toupper
    global __tolower


; char * itoa( int value, char * str, int base );
; rdi - number
; rsi - string
; rdx - base
__itoa :

    push rbp       ; Save the base pointer
    mov rbp, rsp   ; Set up the new base pointer

    ; code
    cmp rdi, 0
    jge .positive
    ; neg rdi ; make it positive

    mov byte [rsi], '-'
    inc rsi

    .positive :
    mov rax, rdi  ; store number
    mov rdi, rsi  ; store string pointer
    mov r14, rdx  ; store base

    .loop_itoa :
        xor rdx, rdx  ; prepare rdx for div
        div r14 ; rax = rax / r14, rdx = rax % r14
        mov cl, byte [lut + rdx]
        mov byte [rdi], cl

        inc rdi
        cmp rax ,0
    jne .loop_itoa

    mov byte [rdi], 0x0 ; null

    dec rdi       ; move back to last digit
    mov r15, rdi  ; length of number part
    sub r15, rsi

    xor rdx, rdx
    mov rax, r15
    mov r8, 2
    div r8
    mov r8, rax   ; half of length

    cmp r8, 0
    je .reverse_end

    ; r15 - length of number part
    ; r8 - half of length of number part
    ; rsi - start
    ; rdi - end
    .reverse:
        mov al, byte [rdi]
        mov cl, byte [rsi]
        mov byte [rdi], cl
        mov byte [rsi], al
        inc rsi
        dec rdi
        dec r8
        cmp r8, 0
    jne .reverse

    .reverse_end:

    ; end of code
    mov rsp, rbp  ; Clean up the base pointer
    pop rbp  ; Restore the base pointer

    ret

; int strlen (const char*)
; rdi - string
__strlen :

    push rbp       ; Save the base pointer
    mov rbp, rsp   ; Set up the new base pointer

    ; code
    mov rax ,0
    cmp byte [rdi], 0x0
    je .end
    mov rsi, rdi ; rsi - start, rdi - end
    dec rdi
    .loop_strln:
        inc rdi
        cmp byte [rdi], 0x0
    jne .loop_strln

    mov rax, rdi ; calculate length
    sub rax, rsi
    dec rax

    .end:

    ; end of code
    mov rsp, rbp  ; Clean up the base pointer
    pop rbp  ; Restore the base pointer

    ret



; char * strcat ( char * destination, const char * source );
; rdi - dest string
; rsi - src string
__strcat :

    push rbp       ; Save the base pointer
    mov rbp, rsp   ; Set up the new base pointer

    ; code


    push rdi
    push rsi

    call __strlen

    pop rsi
    pop rdi
                  ; rax contains string length
    add rdi , rax ; move at the end of dest string
    cmp rax, 0
    jne .has_len
    dec rdi
    .has_len:

    dec rsi
    .loop_strln:
        inc rdi
        inc rsi
        mov al, byte [rsi]
        mov byte [rdi], al
        cmp byte [rdi] , 0x0
    jne .loop_strln


    ; end of code
    mov rsp, rbp  ; Clean up the base pointer
    pop rbp  ; Restore the base pointer

    ret


; char * strcat ( char * first, const char * second );
; rdi - dest string
; rsi - src string
__strcmp :

    push rbp       ; Save the base pointer
    mov rbp, rsp   ; Set up the new base pointer

    ; code
    dec rdi        ; prepare for loop
    dec rsi
    mov ax, 0xFFFF

    .loop_cmp:
        mul ah
        cmp ax, 0 ; check if end of string
        je .end_string
        inc rdi
        inc rsi
        mov ah, byte [rdi]
        mov al, byte [rsi]
        cmp ah , al
    je .loop_cmp
    .end_string:
    mov r15, 0 ;strings are equal
    cmp ah, al
    jl .end_le
    jg .end_gt
    jmp .end
    .end_le:
    mov r15,1
    jmp .end
    .end_gt:
    mov r15, -1

    .end:
    mov rax, r15
    ; end of code
    mov rsp, rbp  ; Clean up the base pointer
    pop rbp  ; Restore the base pointer

    ret


; int atoi ( char * str);
; rdi - input string
__atoi :

    push rbp       ; Save the base pointer
    mov rbp, rsp   ; Set up the new base pointer

    push rdi
    call __strlen
    pop rdi

    ; code
    mov rsi, rdi
    add rdi, rax   ; prepare for loop
    inc rdi
    xor r15, r15   ; prepare for result
    mov r8, 1      ; prepare for base 10
    mov r9, 10


    .loop_atoi:
        dec rdi
        mov al, byte [rdi]
        cmp al, 0       ; check if end of string
        je .end_string
        sub al, 0x30    ; ascii to int
        and rax, 0xFF
        mul r8
        add r15, rax
        mov rax, r8

        mul r9
        mov r8, rax
        cmp rdi, rsi

    jne .loop_atoi

    .end_string:
    mov rax, r15
    ; end of code
    mov rsp, rbp  ; Clean up the base pointer
    pop rbp  ; Restore the base pointer

    ret

; int toupper (int);
; rdi - input character
__toupper :

    push rbp       ; Save the base pointer
    mov rbp, rsp   ; Set up the new base pointer

    ; code
    ; 0x61(a), 0x7a(z)
    and rdi, 0xFF
    cmp rdi, 'a'
    jl .no_action
    cmp rdi, 'z'
    jg .no_action
    sub rdi, 0x20

    .no_action:
    mov rax, rdi

    ; end of code
    mov rsp, rbp  ; Clean up the base pointer
    pop rbp  ; Restore the base pointer

    ret

; int tolower (int);
; rdi - input character
__tolower :

    push rbp       ; Save the base pointer
    mov rbp, rsp   ; Set up the new base pointer

    ; code
    ; 0x41(A), 0x5a(Z)
    and rdi, 0xFF
    cmp rdi, 'A'
    jl .no_action
    cmp rdi, 'z'
    jg .no_action
    add rdi, 0x20

    .no_action:
    mov rax, rdi

    ; end of code
    mov rsp, rbp  ; Clean up the base pointer
    pop rbp  ; Restore the base pointer

    ret