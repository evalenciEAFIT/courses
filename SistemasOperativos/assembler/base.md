# Crear, Compilar y Ejecutar un Programa en Ensamblador en Linux

Este documento describe cómo crear, compilar y ejecutar un programa en ensamblador utilizando el ensamblador **NASM** (Netwide Assembler) en Linux.

---

## Prerrequisitos

1. **Instalar NASM**:
   Si no tienes NASM instalado, puedes instalarlo utilizando el gestor de paquetes de tu distribución. Por ejemplo:

   ```bash
   sudo apt update && sudo apt install nasm -y   # Para distribuciones basadas en Debian/Ubuntu
   sudo dnf install nasm -y                     # Para Fedora
   sudo pacman -S nasm                          # Para Arch Linux
   ```

2. **Instalar un enlazador**:
   La mayoría de las distribuciones incluyen **ld** (el enlazador GNU) de forma predeterminada. Si no lo tienes:

   ```bash
   sudo apt install binutils -y  # Para Debian/Ubuntu
   sudo dnf install binutils -y  # Para Fedora
   sudo pacman -S binutils       # Para Arch Linux
   ```

---

## Pasos para Crear, Compilar y Ejecutar

### 1. Crear el Archivo Fuente en Ensamblador

Crea un archivo con extensión `.asm`, por ejemplo `hola_mundo.asm`. Puedes usar un editor de texto como **nano**, **vim** o **gedit**:

```bash
nano hola_mundo.asm
```

Dentro del archivo, escribe el siguiente código ensamblador:

```asm
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
```

### 2. Compilar el Programa

Usa NASM para ensamblar el archivo fuente y generar un archivo objeto:

```bash
nasm -f elf32 hola_mundo.asm -o hola_mundo.o
```

### 3. Enlazar el Archivo Objeto

Usa **ld** para enlazar el archivo objeto y crear un ejecutable:

```bash
ld -m elf_i386 hola_mundo.o -o hola_mundo
```

### 4. Ejecutar el Programa

Ejecuta el programa resultante:

```bash
./hola_mundo
```

Deberías ver la salida:

```
Hola, mundo!
```

---

## Explicación del Código

1. **Sección `.data`**:
   - Aquí se definen los datos estáticos del programa, como cadenas de texto.
   - `mensaje`: La cadena "Hola, mundo!" con un salto de línea (`0xA`).
   - `longitud`: Calcula automáticamente la longitud de la cadena usando `$ - mensaje`.

2. **Sección `.text`**:
   - Contiene el código ejecutable del programa.
   - `_start`: Es el punto de entrada del programa.

3. **Llamadas al sistema**:
   - Usamos interrupciones del kernel (`int 0x80`) para interactuar con el sistema operativo.
   - `sys_write` escribe datos en la salida estándar.
   - `sys_exit` termina el programa.

---

## Depuración

Si encuentras errores al compilar o ejecutar, prueba lo siguiente:

1. **Verifica la sintaxis**:
   - Asegúrate de que no falten etiquetas, instrucciones o declaraciones de secciones.

2. **Habilitar el soporte de 32 bits**:
   - Si estás en un sistema de 64 bits, instala las bibliotecas de 32 bits necesarias:

     ```bash
     sudo apt install libc6-dev-i386  # Para Ubuntu/Debian
     ```

   - Compila y enlaza usando opciones para 32 bits.

3. **Depurar con gdb**:
   - Usa `gdb` para inspeccionar el programa:

     ```bash
     gdb ./hola_mundo
     ```

---

¡Ahora tienes los conocimientos básicos para crear y ejecutar programas en ensamblador en Linux! Si necesitas más ejemplos o tienes preguntas, no dudes en pedir ayuda. 😊
