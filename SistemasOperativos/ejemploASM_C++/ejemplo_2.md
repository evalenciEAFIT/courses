# Explicación de los Códigos en NASM y C++ para Mostrar la Hora en UTC Ajustada

Este documento explica cómo funcionan los códigos en NASM y C++ para obtener la hora ajustada a una zona horaria específica. Además, incluye instrucciones para instalar los compiladores necesarios en Linux (Fedora, Kali y Ubuntu) y cómo compilar y ejecutar los programas.

---

## 1. Código en NASM

### Descripción

El código en NASM utiliza llamadas al sistema (`syscall`) de Linux para obtener el tiempo actual en segundos desde el epoch (1 de enero de 1970). Posteriormente, ajusta el tiempo según la zona horaria deseada, y calcula las horas, minutos y segundos. Finalmente, imprime la hora ajustada en formato `HH:MM:SS`.

### Estructura del Código

1. **Obtención del Tiempo**:
   - Se usa la syscall `time` (número 201 en x86-64 Linux) para obtener el tiempo actual en segundos.

2. **Ajuste de la Zona Horaria**:
   - El tiempo se ajusta sumando un desplazamiento (`timezone_offset`) en segundos. Por ejemplo, para UTC-11:
     ```nasm
     timezone_offset dq -39600  ; -11 horas * 3600 segundos/hora
     ```

3. **Cálculos de Hora, Minutos y Segundos**:
   - Se divide el tiempo ajustado entre 3600 para obtener las horas.
   - Los minutos y segundos restantes se calculan con divisiones sucesivas.

4. **Conversión a ASCII**:
   - Los valores numéricos de las horas, minutos y segundos se convierten a cadenas ASCII usando la función `byte_to_ascii`.

5. **Salida**:
   - La hora ajustada se imprime usando la syscall `write` (número 1 en x86-64 Linux).

### Código

```asm
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
```

---

## 2. Código en C++

### Descripción

El código en C++ utiliza las bibliotecas estándar `ctime` y `iomanip` para obtener y formatear el tiempo. Al igual que el código en NASM, ajusta el tiempo según la zona horaria deseada y lo imprime en formato `HH:MM:SS`.

### Estructura del Código

1. **Obtención del Tiempo**:
   - Se usa `std::time(nullptr)` para obtener el tiempo actual en segundos desde el epoch.

2. **Ajuste de la Zona Horaria**:
   - Se suma un desplazamiento en segundos al tiempo actual:
     ```cpp
     int timezone_offset = -5 * 3600; // UTC-5 en segundos
     tiempoActual += timezone_offset;
     ```

3. **Conversión a Estructura `tm`**:
   - Se utiliza `std::gmtime` para convertir el tiempo ajustado en una estructura `tm` que contiene horas, minutos y segundos.

4. **Salida Formateada**:
   - Se utiliza `std::setw` y `std::setfill('0')` para imprimir los valores en formato `HH:MM:SS`.

### Código

```cpp
#include <iostream>
#include <ctime>
#include <iomanip>

int main() {
    // Obtener el tiempo actual en segundos desde el epoch
    std::time_t tiempoActual = std::time(nullptr);

    // Ajustar el tiempo a UTC-5
    int timezone_offset = -5 * 3600; // UTC-5 en segundos
    tiempoActual += timezone_offset;

    // Convertir el tiempo ajustado a estructura de tiempo local
    std::tm* horaLocal = std::gmtime(&tiempoActual);

    // Mostrar la hora en formato HH:MM:SS
    if (horaLocal != nullptr) {
        std::cout << "Hora actual: "
                  << std::setw(2) << std::setfill('0') << horaLocal->tm_hour << ":"
                  << std::setw(2) << std::setfill('0') << horaLocal->tm_min << ":"
                  << std::setw(2) << std::setfill('0') << horaLocal->tm_sec
                  << std::endl;
    } else {
        std::cerr << "Error al obtener la hora local." << std::endl;
        return 1;
    }

    return 0;
}
```

---

## 3. Instalación de Compiladores

### Fedora, Kali y Ubuntu

#### Instalación de NASM
NASM es el ensamblador necesario para compilar el código en NASM.

```bash
sudo apt update          # Para Ubuntu y Kali
sudo apt install nasm    # Para Ubuntu y Kali

sudo dnf install nasm    # Para Fedora
```

#### Instalación de GCC (Compilador para C++)
GCC es el compilador necesario para compilar el código en C++.

```bash
sudo apt update          # Para Ubuntu y Kali
sudo apt install g++     # Para Ubuntu y Kali

sudo dnf install gcc-c++ # Para Fedora
```

---

## 4. Cómo Compilar y Ejecutar los Programas

### Compilación y Ejecución del Código NASM

1. Guarda el código en un archivo llamado `hora.asm`.
2. Usa los siguientes comandos para compilar y ejecutar:

```bash
nasm -f elf64 hora.asm -o hora.o
ld hora.o -o hora
./hora
```

### Compilación y Ejecución del Código C++

1. Guarda el código en un archivo llamado `hora.cpp`.
2. Usa los siguientes comandos para compilar y ejecutar:

```bash
g++ -o horaCPP hora.cpp
./horaCPP
```

---

## 5. Ejemplo de Salida

### Salida Esperada del Código NASM y C++

Si la hora actual en UTC es `14:30:00`, y el desplazamiento es UTC-5, ambos programas imprimirán:

```
Hora actual: 09:30:00
```

---

## 6. Notas Adicionales

- Asegúrate de que tu sistema esté configurado correctamente para interpretar cadenas UTF-8, ya que algunos caracteres pueden no mostrarse correctamente en terminales mal configuradas.
- Usa los ejemplos como base para ajustar otras zonas horarias o formatos de salida.

---

¡Espero que este archivo te sea útil! 😊

