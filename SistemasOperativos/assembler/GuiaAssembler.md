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

### 2. Compilar y Enlazar
#### En Linux
```bash
nasm -f elf32 hola_mundo.asm -o hola_mundo.o
ld -m elf_i386 hola_mundo.o -o hola_mundo
```

#### En Windows
```cmd
nasm -f win32 hola_mundo.asm -o hola_mundo.obj
golink /entry:_start /console hola_mundo.obj
```

### 3. Ejecutar el Programa
- **En Linux**: `./hola_mundo`
- **En Windows**: `hola_mundo.exe`

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
