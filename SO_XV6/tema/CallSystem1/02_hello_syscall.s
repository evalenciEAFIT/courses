.global _start

.section .data
mensaje:
    .ascii "Hola Mundo desde Ensamblador usando Syscall (write) directamente!\n"
    longitud = . - mensaje

.section .text
_start:
    /* Llamada al sistema sys_write (syscall número 1 en Linux x86_64) */
    mov     $1, %rax        /* %rax = 1 (sys_write) */
    mov     $1, %rdi        /* %rdi = 1 (File Descriptor: pantalla o stdout) */
    lea     mensaje(%rip), %rsi /* %rsi = Puntero de memoria a la variable 'mensaje' */
    mov     $longitud, %rdx /* %rdx = Longitud en bytes del mensaje a escribir */
    syscall                 /* Interrupción de software: Ejecuta la directiva en el Kernel */

    /* Llamada al sistema sys_exit (syscall número 60 en Linux x86_64) */
    mov     $60, %rax       /* %rax = 60 (sys_exit) para indicarle al OS que terminamos */
    mov     $0, %rdi        /* %rdi = 0 (Código de salida u exit code 0) */
    syscall                 /* El Kernel limpia y termina el proceso */
