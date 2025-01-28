section .data
    msg db "Hora actual: ", 0
    hora db "00:00:00", 0  ; Cadena para la hora en formato HH:MM:SS
    newline db 0xA, 0       ; Salto de línea
    timezone_offset dq -39600  ; Ajuste de zona horaria en segundos (UTC-11)

section .text
    global _start

_start:
    ; Obtener el tiempo actual en segundos desde el epoch
    mov rax, 201          ; syscall número para 'time'
    xor rdi, rdi          ; NULL, no necesitamos un puntero
    syscall
    ; El tiempo UNIX está ahora en rax

    ; Ajustar por zona horaria
    add rax, [timezone_offset]

    ; Guardar el tiempo ajustado en rdi
    mov rdi, rax

    ; Calcular las horas
    xor rdx, rdx
    mov rbx, 3600         ; Segundos en una hora
    div rbx               ; rax = horas, rdx = segundos restantes
    call byte_to_ascii
    mov [hora], al
    mov [hora + 1], ah
    mov byte [hora + 2], ':'

    ; Actualizar rdi con los segundos restantes
    mov rdi, rdx

    ; Calcular los minutos
    xor rdx, rdx
    mov rax, rdi
    mov rbx, 60           ; Segundos en un minuto
    div rbx               ; rax = minutos, rdx = segundos restantes
    call byte_to_ascii
    mov [hora + 3], al
    mov [hora + 4], ah
    mov byte [hora + 5], ':'

    ; Los segundos restantes ya están en rdx
    movzx rax, dl         ; Mover los segundos restantes a rax
    call byte_to_ascii
    mov [hora + 6], al
    mov [hora + 7], ah

    ; Mostrar el mensaje "Hora actual: "
    mov rax, 1           ; syscall número para 'write'
    mov rdi, 1           ; stdout
    mov rsi, msg         ; Dirección del mensaje
    mov rdx, 13          ; Tamaño del mensaje
    syscall

    ; Mostrar la hora
    mov rax, 1           ; syscall número para 'write'
    mov rsi, hora        ; Dirección de la cadena con la hora
    mov rdx, 8           ; Tamaño de la cadena
    syscall

    ; Mostrar un salto de línea
    mov rax, 1           ; syscall número para 'write'
    mov rsi, newline     ; Dirección del salto de línea
    mov rdx, 1           ; Tamaño
    syscall

    ; Salir del programa
    mov rax, 60          ; syscall número para 'exit'
    xor rdi, rdi         ; Código de salida 0
    syscall

; Convertir un valor de un byte a representación ASCII (dos dígitos)
byte_to_ascii:
    push rdx             ; Guardar rdx
    xor rdx, rdx         ; Limpiar rdx
    mov rbx, 10          ; Divisor para obtener decenas
    div rbx              ; División: rax = decenas, rdx = unidades
    add al, '0'          ; Convertir decenas a ASCII
    mov ah, dl           ; Guardar unidades en ah
    add ah, '0'          ; Convertir unidades a ASCII
    pop rdx              ; Restaurar rdx
    ret
