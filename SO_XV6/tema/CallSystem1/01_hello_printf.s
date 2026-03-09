.global main
.extern printf

.section .data
formato:
    .asciz "Hola Mundo usando la funcion *printf* desde Ensamblador (al enlazarse con glibc)!\n"

.section .text
main:
    /* Prólogo de la función para convención C y alineación de la pila exigida para GCC/libc */
    push %rbp
    mov %rsp, %rbp

    /* Preparar los argumentos para printf (el primer argumento va en %rdi) */
    lea formato(%rip), %rdi  
    /* Para funciones con parámetros variables en x86_64, %al (%rax) indica número de argumentos flotantes */
    mov $0, %rax         
    
    /* Llamamos a la función estándar de C 'printf' que usará glibc */
    call printf          

    /* Preparar valor de retorno (return 0) de int main() */
    mov $0, %rax         
    
    /* Epílogo */
    leave
    ret
