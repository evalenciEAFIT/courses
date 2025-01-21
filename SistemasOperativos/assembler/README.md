
# Crear, Compilar y Ejecutar un Programa en Ensamblador en Linux y Windows 11

Este documento describe cómo crear, compilar y ejecutar un programa en ensamblador utilizando el ensamblador **NASM** (Netwide Assembler) en Linux y Windows 11.

---

## Prerrequisitos

### Para Linux
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

### Para Windows 11
1. **Instalar NASM**:
   - Descarga el instalador desde el sitio oficial de NASM: [https://www.nasm.us/](https://www.nasm.us/).
   - Ejecuta el instalador y asegúrate de agregar NASM al **PATH** durante la instalación o manualmente.

2. **Instalar un enlazador**:
   - Puedes usar el enlazador **GoLink** o **MinGW** para enlazar los archivos objeto generados.
   - Descarga GoLink desde: [https://www.godevtool.com/](https://www.godevtool.com/).
   - Para MinGW, puedes instalarlo desde [MinGW-w64](https://www.mingw-w64.org/).

---

## Pasos para Crear, Compilar y Ejecutar

### 1. Crear el Archivo Fuente en Ensamblador

Crea un archivo con extensión `.asm`, por ejemplo `hola_mundo.asm`. Puedes usar un editor de texto como **Notepad**, **Visual Studio Code** o **nano** (en Linux).

En ambos sistemas, escribe el siguiente código ensamblador:

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

#### En Linux
Usa NASM para ensamblar el archivo fuente y generar un archivo objeto:

```bash
nasm -f elf32 hola_mundo.asm -o hola_mundo.o
```

#### En Windows 11
Usa NASM para ensamblar el archivo y generar un archivo objeto en formato **win32**:

```cmd
nasm -f win32 hola_mundo.asm -o hola_mundo.obj
```

### 3. Enlazar el Archivo Objeto

#### En Linux
Usa **ld** para enlazar el archivo objeto y crear un ejecutable:

```bash
ld -m elf_i386 hola_mundo.o -o hola_mundo
```

#### En Windows 11
Si usas GoLink, enlaza el archivo objeto con el siguiente comando:

```cmd
golink /entry:_start /console hola_mundo.obj
```

Si usas MinGW, puedes enlazarlo con:

```cmd
gcc -o hola_mundo.exe hola_mundo.obj -nostdlib
```

### 4. Ejecutar el Programa

#### En Linux
Ejecuta el programa resultante:

```bash
./hola_mundo
```

Deberías ver la salida:

```
Hola, mundo!
```

#### En Windows 11
Ejecuta el programa resultante desde la terminal:

```cmd
hola_mundo.exe
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
   - Usamos interrupciones del kernel (`int 0x80`) para interactuar con el sistema operativo (en Linux).
   - En Windows, el sistema utiliza otras convenciones, pero el ejemplo aquí es para propósito educativo.

---

## Depuración

### En Linux
1. **Verifica la sintaxis**:
   - Asegúrate de que no falten etiquetas, instrucciones o declaraciones de secciones.

2. **Habilitar el soporte de 32 bits**:
   - Si estás en un sistema de 64 bits, instala las bibliotecas de 32 bits necesarias:

     ```bash
     sudo apt install libc6-dev-i386  # Para Ubuntu/Debian
     ```

3. **Depurar con gdb**:
   - Usa `gdb` para inspeccionar el programa:

     ```bash
     gdb ./hola_mundo
     ```

### En Windows
1. **Errores comunes**:
   - Asegúrate de que NASM y GoLink/MinGW estén correctamente configurados en el PATH.

2. **Depurar con herramientas**:
   - Usa herramientas como **OllyDbg** o **gdb** en MinGW para depuración.

---

## Menú de Ejemplos
[GuiaAssembler.md](GuiaAssembler.md) guia corta con ejemplos para usar Asseble.  
1. **Uso de Pantalla: Reloj**
   - [ejemploPantalla.md](ejemploPantalla.md) para aprender a manipular texto en pantalla, con un rejo en dos colores.

2. **Mostrar Registros de Memoria: Explorar memoria**
   - [MostrarRegistrosMemoria.md](MostrarRegistrosMemoria.md) ejemplo de cómo leer y mostrar el contenido de registros en binario y hexadecimal.
