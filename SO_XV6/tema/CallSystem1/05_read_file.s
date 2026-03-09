.global _start

.section .data
nombre_archivo: .asciz "archivo_ejemplo_asm.txt"
msg_error: .ascii "Error al leer archivo (asegurate de correr primero 04).\n"
long_error = . - msg_error
msg_exito: .ascii "\n--- Fin del archivo guardado en el disco ---\n"
long_exito = . - msg_exito
msg_titulo: .ascii "Contenido leido por buffers de sys_read:\n\n"
long_titulo = . - msg_titulo

/* Reservamos en memoria la seccion BSS (Block Started by Symbol) que no tiene 
   contenido inicial, solo es un casillero vacío para el Buffer donde leeremos datos. */
.section .bss
buffer_lectura: .space 128  /* Espacio de 128 bytes reservado */

.section .text
_start:
    /* Imprimir titulo descriptivo */
    mov     $1, %rax
    mov     $1, %rdi
    lea     msg_titulo(%rip), %rsi
    mov     $long_titulo, %rdx
    syscall

    /* Llamada a sys_open (syscall 2) */
    mov     $2, %rax
    lea     nombre_archivo(%rip), %rdi  
    /* Flags: SOLO LECTURA = 0 */
    mov     $0, %rsi 
    /* Permisos (Argumento ignorado al solo leer) */
    mov     $0, %rdx
    syscall

    /* Verificamos si logramos abrir el archivo (ver rrax) */
    cmp     $0, %rax
    jl      error_lectura
    
    /* Guardando el descriptor de disco en r12 */
    mov     %rax, %r12

bucle_lectura:
    /* sys_read (syscall número 0 en x86_64) */
    mov     $0, %rax
    mov     %r12, %rdi          /* Pasamos el File Descriptor del archivo en r12 */
    lea     buffer_lectura(%rip), %rsi /* Aqui ponemos donde deben guardarse los datos */
    mov     $128, %rdx          /* Tamaño del Buffer: Pedimos maximo 128 bytes */
    syscall

    /* %rax devuelve cuántos bytes acabamos de leer. Si es 0, terminamos el archivo (EOF). */
    cmp     $0, %rax
    jle     cerrar_lectura      /* Si es 0 o negativo, cerramos el Fd para terminar. */

    /* Escribir inmediamente al monitor stdout con un sys_write. 
       Ya tenemos la longitud en rrax.. asi que lo movemos a rdx ! */
    mov     %rax, %rdx          /* rdx = cantidad bytes a imprimir */
    mov     $1, %rax            /* sys_write (1) */
    mov     $1, %rdi            /* A pantalla (1) */
    /* La direccion en memoria (buffer) YA la tenemos en rsi por suerte! */
    syscall

    /* Regresamos al bucle por si el archivo tenía mas de 128 bytes (y debe repetir lectura) */
    jmp     bucle_lectura

cerrar_lectura:
    /* Misión Cumplida, llamamos a sys_close (3) */
    mov     $3, %rax
    mov     %r12, %rdi
    syscall

    /* Mensaje final para pantalla  */
    mov     $1, %rax
    mov     $1, %rdi
    lea     msg_exito(%rip), %rsi
    mov     $long_exito, %rdx
    syscall
    jmp     salir_exito

error_lectura:
    mov     $1, %rax
    mov     $1, %rdi
    lea     msg_error(%rip), %rsi
    mov     $long_error, %rdx
    syscall

salir_exito:
    mov     $60, %rax
    mov     $0, %rdi
    syscall
