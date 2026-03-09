.global _start

.section .data
nombre_archivo: .asciz "archivo_ejemplo_asm.txt"
mensaje: .ascii "Mensaje escrito directamente desde Ensamblador sin librerias.\n"
long_mensaje = . - mensaje
msg_exito: .ascii "Archivo guardado exitosamente desde Ensamblador.\n"
long_exito = . - msg_exito
msg_error: .ascii "Error creando archivo.\n"
long_error = . - msg_error

.section .text
_start:
    /* Llamada a sys_open (syscall número 2 en x86_64) */
    mov     $2, %rax
    lea     nombre_archivo(%rip), %rdi /* Argumento 1: ruta/nombre de archivo (puntero ascci-z) */
    
    /* Argumento 2: Flags 
       O_WRONLY = 1 (en octal 01), O_CREAT = 64 (0100 octal), O_TRUNC = 512 (01000 octal)
       1 + 64 + 512 = 577 decimal (indicamos modo de apertura) 
    */
    mov     $577, %rsi 
    
    /* Argumento 3: Modo/Permisos. 0644 octal = 420 decimal (rw-r--r--) */
    mov     $420, %rdx
    syscall

    /* %rax devuelve el "File Descriptor". Si es menor a 0 significa un Error. */
    cmp     $0, %rax
    jl      error_archivo
    
    /* Guardamos nuestro maravilloso fd en %r12 (registro a salvo durante llamadas) */
    mov     %rax, %r12

    /* sys_write (syscall número 1): Escribimos el mensaje al disco usando el Fd de r12 */
    mov     $1, %rax
    mov     %r12, %rdi
    lea     mensaje(%rip), %rsi
    mov     $long_mensaje, %rdx
    syscall

    /* sys_close (syscall número 3): Cerramos gentilmente nuestro descriptor abierto (en r12) */
    mov     $3, %rax
    mov     %r12, %rdi
    syscall

    /* Imprimir a pantalla un mensaje de éxito */
    mov     $1, %rax
    mov     $1, %rdi
    lea     msg_exito(%rip), %rsi
    mov     $long_exito, %rdx
    syscall
    jmp     salir

error_archivo:
    /* Imprimir error en caso de fallo (ej. sin permisos o disco bloqueado) */
    mov     $1, %rax
    mov     $1, %rdi
    lea     msg_error(%rip), %rsi
    mov     $long_error, %rdx
    syscall

salir:
    /* sys_exit (syscall número 60): Cerramos nuestro proceso correctamente con código 0 */
    mov     $60, %rax
    mov     $0, %rdi
    syscall
