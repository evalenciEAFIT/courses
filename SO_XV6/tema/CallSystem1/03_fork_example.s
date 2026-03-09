.global _start

.section .data
msg_padre_1: .ascii "Soy el proceso PADRE. Mi PID es: "
len_padre_1 = . - msg_padre_1

msg_padre_2: .ascii ", y el PID de mi nuevo hijo es: "
len_padre_2 = . - msg_padre_2

msg_hijo_1: .ascii "Soy el proceso HIJO. Mi PID es: "
len_hijo_1 = . - msg_hijo_1

msg_hijo_2: .ascii ", y el PID de mi padre es: "
len_hijo_2 = . - msg_hijo_2

msg_error: .ascii "Error al ejecutar fork()!\n"
len_error = . - msg_error

newline: .ascii "\n"

.section .text

_start:
    /* Llamada a sys_fork (syscall 57 en Linux x86_64) */
    mov $57, %rax
    syscall

    /* %rax contiene PID hijo (>0) si somos el padre, 0 si somos el hijo, <0 por error */
    cmp $0, %rax
    jl  error_fork
    je  proceso_hijo

proceso_padre:
    /* Guardar PID del hijo en %r12 (registro callee-saved, no será modificado por print_num) */
    mov %rax, %r12

    /* Imprimir: ("Soy el proceso PADRE. Mi PID es: ") */
    mov $1, %rax
    mov $1, %rdi
    lea msg_padre_1(%rip), %rsi
    mov $len_padre_1, %rdx
    syscall

    /* Llamada a sys_getpid (syscall 39) */
    mov $39, %rax
    syscall
    
    /* Imprimir PID Padre (en %rax) a texto -> enviado a %rdi para la función */
    mov %rax, %rdi
    call print_num

    /* Imprimir: (", y el PID de mi nuevo hijo es: ") */
    mov $1, %rax
    mov $1, %rdi
    lea msg_padre_2(%rip), %rsi
    mov $len_padre_2, %rdx
    syscall

    /* Recuperar PID del Hijo guardado en %r12 e imprimirlo */
    mov %r12, %rdi
    call print_num

    /* Imprimir NUEVA LINEA */
    mov $1, %rax
    mov $1, %rdi
    lea newline(%rip), %rsi
    mov $1, %rdx
    syscall

    jmp salir

proceso_hijo:
    /* Imprimir: ("Soy el proceso HIJO. Mi PID es: ") */
    mov $1, %rax
    mov $1, %rdi
    lea msg_hijo_1(%rip), %rsi
    mov $len_hijo_1, %rdx
    syscall

    /* Llamada a sys_getpid (syscall 39) */
    mov $39, %rax
    syscall
    
    /* Imprimir PID Hijo */
    mov %rax, %rdi
    call print_num

    /* Imprimir: (", y el PID de mi padre es: ") */
    mov $1, %rax
    mov $1, %rdi
    lea msg_hijo_2(%rip), %rsi
    mov $len_hijo_2, %rdx
    syscall

    /* Llamada a sys_getppid (syscall 110 en x86_64, para obtener el Parent PID) */
    mov $110, %rax
    syscall

    /* Imprimir PID del padre original */
    mov %rax, %rdi
    call print_num

    /* Imprimir NUEVA LINEA */
    mov $1, %rax
    mov $1, %rdi
    lea newline(%rip), %rsi
    mov $1, %rdx
    syscall

    jmp salir

error_fork:
    /* Imprimir ERROR de FORK */
    mov $1, %rax
    mov $1, %rdi
    lea msg_error(%rip), %rsi
    mov $len_error, %rdx
    syscall

salir:
    /* Llamada a sys_exit (syscall 60) */
    mov $60, %rax
    mov $0, %rdi
    syscall


/* 
===================================================
 FUNCIÓN print_num:
 Convierte entero positivo (en %rdi) a cadena ASCII 
 y usa sys_write (syscall 1) para imprimirlo.
===================================================
*/
.type print_num, @function
print_num:
    push %rbp
    mov %rsp, %rbp
    sub $32, %rsp          /* Reservar 32 bytes en la pila para el numero ASCII */
    
    mov %rdi, %rax         /* %rax sera el dividendo */
    mov $10, %r8           /* %r8 divisor (base 10) */
    lea -1(%rbp), %rcx     /* %rcx apuntara al final del buffer en la pila */

convert_loop:
    xor %rdx, %rdx         /* Importante: %rdx debe ser 0 antes de div, divide %rdx:%rax / %r8 */
    div %r8                /* Resulutado: Cociente en %rax, Residuo en %rdx */
    add $'0', %dl          /* Convertir el residuo a ASCII sumando '0' (48) */
    mov %dl, (%rcx)        /* Guardar el caracter temporalmente en la pila */
    dec %rcx               /* Retroceder puntero */
    test %rax, %rax        /* ¿Cociente == 0? Si es 0 hemos terminado de extraer digitos */
    jnz convert_loop       /* Si no es 0, repetimos para el siguiente digito */

    inc %rcx               /* Volver un caracter hacia adelante (donde pusimos el ultimo valido) */

    /* Calcular largo string: rcx -> -1(%rbp) */
    mov %rbp, %rdx
    sub %rcx, %rdx         /* %rdx tendra la cantidad exacta de bytes escritos (longitud) */

    /* Preparar e invocar sys_write */
    mov %rcx, %rsi         /* Inicio de la cadena */
    mov $1, %rax           /* syscall: sys_write (1) */
    mov $1, %rdi           /* fd: stdout (1) */
    syscall                

    leave                  /* Limpiar marco de pila (vuelve %rsp a %rbp, saca %rbp) */
    ret
