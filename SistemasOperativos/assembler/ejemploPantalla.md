# Guía para Crear un Programa en Ensamblador que Muestra un Reloj

Este documento explica cómo crear un programa en ensamblador que muestra un reloj en el centro de la pantalla, con la hora en color rojo y los minutos en color azul. El programa utiliza la arquitectura x86 y la interrupción del BIOS para la manipulación de texto en pantalla.

---

## Requisitos Previos

### Herramientas Necesarias
1. **NASM (Netwide Assembler)**
   - Instálalo según tu sistema operativo:
     - **Linux**: `sudo apt install nasm` (para distribuciones basadas en Debian/Ubuntu).
     - **Windows**: Descarga desde [https://www.nasm.us/](https://www.nasm.us/).

2. **Emulador o Máquina Virtual** (opcional):
   - Si estás en un sistema moderno, puedes usar **DOSBox** o una máquina virtual con DOS para ejecutar el programa.

3. **Editor de Texto**: Cualquier editor de texto como **nano**, **vim**, o **Notepad++**.

---

## Código del Programa
Crea un archivo llamado `reloj.asm` y copia el siguiente código:

```asm
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
```

---

## Explicación del Código

1. **Obtención de la Hora:**
   - Se utiliza la interrupción `0x1A` del BIOS para leer la hora actual.
   - Los valores de la hora y los minutos se almacenan en los registros `CH` y `CL`, respectivamente.

2. **Configuración de la Pantalla:**
   - Se utiliza la interrupción `0x10` para limpiar la pantalla y configurar el cursor.
   - Se calcula la posición central (fila 12, columna 35).

3. **Mostrar Hora y Minutos:**
   - Los valores de hora y minutos se imprimen con colores diferentes mediante la función `0x09` de la interrupción `0x10`.

---

## Compilación y Ejecución

### Compilar el Programa
Utiliza NASM para ensamblar el programa:

```bash
nasm -f bin reloj.asm -o reloj.com
```

### Ejecutar el Programa
1. Si estás en un sistema DOS:
   ```
   reloj.com
   ```
2. Si usas DOSBox:
   - Mueve el archivo `reloj.com` a una carpeta accesible.
   - Monta la carpeta en DOSBox:
     ```
     mount c /ruta/a/carpeta
     c:
     reloj.com
     ```

---

## Notas

- Este programa utiliza interrupciones del BIOS, que son compatibles principalmente con sistemas DOS o emuladores.
- En sistemas modernos, es posible que necesites ajustar o simular estas interrupciones.

¡Listo! Ahora tienes un programa funcional en ensamblador que muestra un reloj con colores en el centro de la pantalla. 😊
