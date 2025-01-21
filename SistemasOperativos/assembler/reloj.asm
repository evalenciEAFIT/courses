
section .data
    color_rojo db 0x04  ; Color rojo para la hora
    color_azul db 0x01  ; Color azul para los minutos
    mensaje db 'HH:MM', 0 ; Plantilla para mostrar la hora y minutos

section .bss
    hora resb 2
    minutos resb 2

section .text
    global _start

_start:
    ; Obtener la hora actual del sistema
    mov ah, 0x02        ; Función para leer la hora del reloj BIOS
    int 0x1A            ; Interrupción del BIOS

    ; Separar hora y minutos
    mov [hora], ch      ; Almacenar la hora (CH contiene la hora)
    mov [minutos], cl   ; Almacenar los minutos (CL contiene los minutos)

    ; Configurar pantalla
    call limpiar_pantalla
    call centrar_y_mostrar

    ; Finalizar programa
    mov ah, 0x4C        ; Terminar programa
    int 0x21

limpiar_pantalla:
    mov ax, 0x0600      ; Función de BIOS para limpiar pantalla
    mov bh, 0x07        ; Color de fondo: blanco sobre negro
    mov cx, 0x0000      ; Coordenada inicial (esquina superior izquierda)
    mov dx, 0x184F      ; Coordenada final (esquina inferior derecha)
    int 0x10
    ret

centrar_y_mostrar:
    ; Calcular posición centrada
    mov dh, 12          ; Fila 12 (centro vertical en pantalla estándar de 25 filas)
    mov dl, 35          ; Columna 35 (centro aproximado horizontal)
    mov bh, 0x00        ; Página de video

    ; Mostrar hora en rojo
    mov ah, 0x02        ; Configurar cursor
    int 0x10            ; Mover cursor al centro
    mov ah, 0x09        ; Escribir caracteres con atributos
    mov al, [hora]      ; Mostrar la hora
    mov bl, [color_rojo]; Color rojo
    mov cx, 2           ; Mostrar dos caracteres
    int 0x10

    ; Mostrar minutos en azul
    add dl, 3           ; Desplazar cursor después de la hora
    mov ah, 0x02        ; Configurar cursor
    int 0x10            ; Mover cursor
    mov ah, 0x09        ; Escribir caracteres con atributos
    mov al, [minutos]   ; Mostrar los minutos
    mov bl, [color_azul]; Color azul
    mov cx, 2           ; Mostrar dos caracteres
    int 0x10
    ret
