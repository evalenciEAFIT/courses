
section .data
    mensaje db 'Hola, mundo!', 0xA  ; Mensaje con salto de línea
    longitud equ $ - mensaje        ; Calcula la longitud del mensaje

section .text
    global _start

_start:
    ; Llamada al sistema: write (escribir en la salida estándar)
    mov eax, 4         ; Número de la llamada al sistema: sys_write
    mov ebx, 1         ; File descriptor: 1 (salida estándar)
    mov ecx, mensaje   ; Dirección del mensaje
    mov edx, longitud  ; Longitud del mensaje
    int 0x80           ; Interrupción del kernel

    ; Llamada al sistema: exit (terminar el programa)
    mov eax, 1         ; Número de la llamada al sistema: sys_exit
    xor ebx, ebx       ; Código de salida: 0
    int 0x80           ; Interrupción del kernel
