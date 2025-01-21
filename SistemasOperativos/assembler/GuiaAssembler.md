# Manual del Lenguaje de Programación Ensamblador

Este documento sirve como una guía para aprender el lenguaje de programación ensamblador, incluyendo conceptos básicos, instrucciones, herramientas y ejemplos prácticos para Linux y Windows 11.

---

## ¿Qué es el Lenguaje Ensamblador?

El lenguaje ensamblador (Assembly o ASM) es un lenguaje de bajo nivel que permite escribir instrucciones que el procesador puede ejecutar directamente. Es específico para cada arquitectura de CPU y proporciona un control detallado sobre el hardware.

### Características Principales
- **Específico de la arquitectura**: Cada conjunto de instrucciones depende del procesador (x86, x86_64, ARM, etc.).
- **Bajo nivel**: Proporciona control directo sobre los registros, memoria y periféricos.
- **Eficiencia**: Los programas en ensamblador suelen ser rápidos y compactos.
- **Difícil de aprender**: Requiere conocer detalles de la arquitectura y el conjunto de instrucciones del procesador.

---

## Conceptos Fundamentales

### 1. **Registros**
Los registros son pequeñas áreas de almacenamiento dentro del procesador. Algunos registros comunes en arquitecturas x86 son:
- **EAX, EBX, ECX, EDX**: Registros de propósito general.
- **ESP**: Puntero de pila.
- **EBP**: Puntero base.
- **EIP**: Puntero de instrucción (indica la próxima instrucción a ejecutar).

### 2. **Segmentos**
El programa se organiza en secciones o segmentos:
- **.data**: Contiene datos estáticos y constantes.
- **.bss**: Para datos no inicializados.
- **.text**: Contiene el código ejecutable.

### 3. **Instrucciones**
Las instrucciones son comandos que indican al procesador qué hacer. Ejemplos:
- `mov`: Mueve datos entre registros o memoria.
- `add`: Suma valores.
- `sub`: Resta valores.
- `int`: Llama a una interrupción del sistema.

---

## Herramientas para Programar en Ensamblador

### En Linux
- **NASM (Netwide Assembler)**: Ensamblador popular y eficiente.
- **GDB**: Depurador para analizar programas.
- **LD**: Enlazador para crear ejecutables.

### En Windows
- **NASM**: Disponible para Windows.
- **GoLink**: Enlazador sencillo.
- **MinGW**: Herramientas GNU para Windows.
- **OllyDbg**: Depurador gráfico.

---

## Crear, Compilar y Ejecutar un Programa en Ensamblador

### 1. Crear el Archivo Fuente
Crea un archivo con extensión `.asm`. Aquí tienes un ejemplo básico para imprimir "Hola, mundo!":

```asm
section .data
    mensaje db 'Hola, mundo!', 0xA  ; Mensaje con salto de línea
    longitud equ $ - mensaje        ; Calcula la longitud del mensaje

section .text
    global _start

_start:
    mov eax, 4         ; Número de la llamada al sistema: sys_write
    mov ebx, 1         ; Salida estándar
    mov ecx, mensaje   ; Dirección del mensaje
    mov edx, longitud  ; Longitud del mensaje
    int 0x80           ; Interrupción del kernel

    mov eax, 1         ; sys_exit
    xor ebx, ebx       ; Código de salida: 0
    int 0x80
```

### 2. Operaciones Matemáticas
Un programa que realiza una suma de dos números:

```asm
section .data
    num1 db 10
    num2 db 20
    resultado db 0

section .text
    global _start

_start:
    mov al, [num1]   ; Cargar num1 en AL
    add al, [num2]   ; Sumar num2 a AL
    mov [resultado], al ; Guardar el resultado

    ; Terminar el programa
    mov eax, 1       ; sys_exit
    xor ebx, ebx
    int 0x80
```

### 3. Leer y Escribir Archivos
Un programa que lee un archivo y escribe su contenido en la salida estándar:

```asm
section .data
    filename db 'archivo.txt', 0  ; Nombre del archivo
    buffer db 256 dup(0)          ; Buffer para leer datos

section .bss
    fd resb 1                     ; Descriptor de archivo

section .text
    global _start

_start:
    ; Abrir el archivo
    mov eax, 5         ; sys_open
    mov ebx, filename  ; Nombre del archivo
    mov ecx, 0         ; Modo de solo lectura
    int 0x80           ; Interrupción
    mov [fd], eax      ; Guardar descriptor

    ; Leer el archivo
    mov eax, 3         ; sys_read
    mov ebx, [fd]      ; Descriptor de archivo
    mov ecx, buffer    ; Buffer de destino
    mov edx, 256       ; Tamaño máximo a leer
    int 0x80           ; Interrupción

    ; Escribir el contenido leído
    mov eax, 4         ; sys_write
    mov ebx, 1         ; Salida estándar
    mov ecx, buffer    ; Buffer de datos
    int 0x80           ; Interrupción

    ; Cerrar el archivo
    mov eax, 6         ; sys_close
    mov ebx, [fd]      ; Descriptor de archivo
    int 0x80           ; Interrupción

    ; Terminar el programa
    mov eax, 1         ; sys_exit
    xor ebx, ebx
    int 0x80
```

### 4. Compilar y Enlazar
#### En Linux
```bash
nasm -f elf32 ejemplo.asm -o ejemplo.o
ld -m elf_i386 ejemplo.o -o ejemplo
```

#### En Windows
```cmd
nasm -f win32 ejemplo.asm -o ejemplo.obj
golink /entry:_start /console ejemplo.obj
```

### 5. Ejecutar el Programa
- **En Linux**: `./ejemplo`
- **En Windows**: `ejemplo.exe`

---

## Temas Avanzados

### 1. **Manejo de la Pila (Stack)**
La pila es una estructura de datos utilizada para almacenar información temporal. Instrucciones comunes:
- `push`: Agrega un valor a la pila.
- `pop`: Extrae un valor de la pila.

### 2. **Interrupciones**
Las interrupciones permiten interactuar con el sistema operativo. Por ejemplo:
- `int 0x80`: Llama a las funciones del sistema en Linux.
- `int 0x21`: Utilizado en DOS para operaciones como imprimir texto.

### 3. **Macros**
Las macros son atajos para escribir código repetitivo. Ejemplo:

```asm
%macro imprimir 2
    mov eax, 4
    mov ebx, 1
    mov ecx, %1
    mov edx, %2
    int 0x80
%endmacro
```

---

## Depuración y Optimización

### Depuración
- **GDB (Linux)**: Inspecciona registros y memoria.
  ```bash
  gdb ./hola_mundo
  ```

- **OllyDbg (Windows)**: Depurador visual para analizar el flujo del programa.

### Optimización
- Escribe código eficiente usando menos instrucciones.
- Minimiza el uso de memoria y registros temporales.

---

## Recursos Adicionales

- [Documentación oficial de NASM](https://www.nasm.us/)
- [Guía de instrucciones x86](https://c9x.me/x86/)
- [Tutorial interactivo en Linux](https://asmtutor.com/)

---

¡Este manual te ayudará a comenzar con el lenguaje ensamblador y a escribir programas eficientes en Linux y Windows! Si necesitas ejemplos adicionales o profundizar en algún tema, no dudes en pedirlo. 😊
