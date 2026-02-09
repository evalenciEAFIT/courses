# 🧠 Memoria Virtual en Linux - Guía Educativa

## 📚 Índice
1. [Introducción](#introducción)
2. [Conceptos Fundamentales](#conceptos-fundamentales)
3. [Estructura del Proyecto](#estructura-del-proyecto)
4. [Compilación y Ejecución](#compilación-y-ejecución)
5. [Demostraciones](#demostraciones)
6. [Ejercicios Prácticos](#ejercicios-prácticos)
7. [Referencias](#referencias)

---

## 🎯 Introducción

Este proyecto educativo demuestra los conceptos fundamentales de **memoria virtual** en sistemas operativos Linux mediante ejemplos prácticos en C.

### ¿Qué es la Memoria Virtual?

La **memoria virtual** es una técnica de gestión de memoria que proporciona a cada proceso la ilusión de tener un espacio de direcciones contiguo y privado, independientemente de la memoria física disponible.

### Ventajas de la Memoria Virtual

✅ **Aislamiento**: Cada proceso tiene su propio espacio de direcciones  
✅ **Protección**: Un proceso no puede acceder a la memoria de otro  
✅ **Abstracción**: Los programas no necesitan preocuparse por la memoria física  
✅ **Eficiencia**: Permite ejecutar programas más grandes que la RAM disponible  

---

## 🧩 Conceptos Fundamentales

### 1. Espacio de Direcciones Virtuales

Cada proceso en Linux tiene un espacio de direcciones virtuales dividido en segmentos:

```
Direcciones Altas
┌─────────────────────┐
│   Stack (Pila)      │ ← Variables locales, parámetros de funciones
├─────────────────────┤
│        ↓            │
│     (crece hacia    │
│      abajo)         │
│                     │
│        ↑            │
│     (crece hacia    │
│      arriba)        │
├─────────────────────┤
│   Heap (Montículo)  │ ← Memoria dinámica (malloc, new)
├─────────────────────┤
│   BSS               │ ← Variables globales no inicializadas
├─────────────────────┤
│   Data              │ ← Variables globales inicializadas
├─────────────────────┤
│   Text (Código)     │ ← Instrucciones del programa
└─────────────────────┘
Direcciones Bajas
```

### 2. Páginas de Memoria

La memoria se divide en **páginas** (normalmente 4096 bytes = 4KB):

- **Página Virtual**: Bloque de memoria en el espacio virtual
- **Marco de Página (Frame)**: Bloque de memoria física
- **Tabla de Páginas**: Mapea páginas virtuales a marcos físicos

### 3. Traducción de Direcciones

```
Dirección Virtual → MMU (Memory Management Unit) → Dirección Física
                    ↑
              Tabla de Páginas
```

### 4. Protección de Memoria

Cada página tiene permisos:
- **PROT_READ**: Lectura permitida
- **PROT_WRITE**: Escritura permitida
- **PROT_EXEC**: Ejecución permitida
- **PROT_NONE**: Sin acceso

---

## 📁 Estructura del Proyecto

```
memoria/
├── README.md           # Este archivo
├── Makefile           # Automatización de compilación
├── mem1.c             # Programa principal con demostraciones
└── ejercicios/        # Ejercicios prácticos (próximamente)
```

---

## 🔨 Compilación y Ejecución

### Opción 1: Usando Makefile (Recomendado)

```bash
# Compilar
make

# Ejecutar
make run

# Limpiar archivos compilados
make clean
```

### Opción 2: Compilación Manual

```bash
# Compilar
gcc -Wall -Wextra -o mem1 mem1.c

# Ejecutar
./mem1
```

### Requisitos

- **GCC** (GNU Compiler Collection)
- **Linux** (kernel 2.6 o superior)
- Permisos de usuario estándar (no requiere root)

---

## 🎓 Demostraciones

El programa `mem1.c` incluye las siguientes demostraciones:

### 1. 🗺️ Mapeo de Memoria del Proceso

**Concepto**: Visualiza dónde se ubican diferentes tipos de variables en el espacio de direcciones.

**Qué demuestra**:
- Segmento de texto (código)
- Segmento de datos (variables estáticas)
- Heap (memoria dinámica)
- Stack (variables locales)

**Salida esperada**:
```
Texto (dirección de main): 0x55555555xxxx
Datos (static_var):        0x55555555xxxx
Heap (heap_var):           0x55555555xxxx
Stack (stack_var):         0x7fffffffxxxx
```

### 2. 🔗 mmap - Mapeo de Memoria

**Concepto**: Reserva regiones de memoria virtual usando la llamada al sistema `mmap()`.

**Qué demuestra**:
- Cómo solicitar memoria al kernel
- Diferencia entre memoria mapeada y malloc
- Control fino sobre regiones de memoria

**Función clave**:
```c
void* mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);
```

### 3. 🔀 Separación de Espacios de Direcciones

**Concepto**: Cada proceso tiene su propio espacio de direcciones privado.

**Qué demuestra**:
- Proceso padre e hijo tienen espacios separados
- Misma dirección virtual puede contener valores diferentes
- Copy-on-Write (COW) implícito en fork()

**Experimento**:
1. Padre e hijo crean variables con el mismo valor inicial
2. Hijo modifica su variable
3. Variable del padre permanece sin cambios

### 4. 🛡️ Protección de Páginas

**Concepto**: El sistema operativo puede proteger páginas contra accesos no autorizados.

**Qué demuestra**:
- Memoria de solo lectura (PROT_READ)
- Segmentation fault al intentar escribir
- Mecanismos de protección del kernel

---

## 📝 Ejercicios Prácticos

### Ejercicio 1: Observar el Layout de Memoria

**Objetivo**: Familiarizarse con el espacio de direcciones.

**Tareas**:
1. Ejecuta el programa y observa las direcciones
2. Ejecuta varias veces y nota que las direcciones cambian (ASLR)
3. Identifica qué segmento tiene direcciones más altas/bajas

**Preguntas**:
- ¿Por qué las direcciones del stack son más altas?
- ¿Qué es ASLR y por qué las direcciones cambian?

### Ejercicio 2: Experimentar con mmap

**Objetivo**: Entender el mapeo de memoria.

**Tareas**:
1. Modifica el tamaño de la región mapeada
2. Intenta mapear memoria sin permisos de escritura
3. Observa qué pasa si no llamas a `munmap()`

### Ejercicio 3: Fork y Memoria

**Objetivo**: Comprender la separación de espacios.

**Tareas**:
1. Modifica el código para crear más variables compartidas
2. Observa las direcciones virtuales en padre e hijo
3. Investiga cuánta memoria consume realmente (usa `ps aux`)

### Ejercicio 4: Protección de Memoria

**Objetivo**: Experimentar con permisos de páginas.

**Tareas**:
1. Descomenta la línea que causa segfault
2. Usa `mprotect()` para cambiar permisos después de mapear
3. Intenta ejecutar código desde una página sin PROT_EXEC

---

## 🔍 Comandos Útiles para Explorar

### Ver el mapa de memoria de un proceso en ejecución

```bash
# En una terminal, ejecuta el programa con sleep
./mem1 &

# En otra terminal, ve el mapa de memoria
cat /proc/$(pgrep mem1)/maps
```

### Ver el tamaño de página del sistema

```bash
getconf PAGE_SIZE
```

### Monitorear uso de memoria

```bash
# Mientras el programa corre
ps aux | grep mem1
top -p $(pgrep mem1)
```

---

## 📖 Referencias

### Documentación Oficial

- [mmap(2) - Linux man page](https://man7.org/linux/man-pages/man2/mmap.2.html)
- [fork(2) - Linux man page](https://man7.org/linux/man-pages/man2/fork.2.html)
- [Virtual Memory in Linux](https://www.kernel.org/doc/html/latest/admin-guide/mm/index.html)

### Libros Recomendados

- **"Operating Systems: Three Easy Pieces"** - Remzi H. Arpaci-Dusseau
  - Capítulo: Address Spaces, Paging
- **"The Linux Programming Interface"** - Michael Kerrisk
  - Capítulo 49: Memory Mappings
- **"Understanding the Linux Kernel"** - Daniel P. Bovet

### Recursos Online

- [OSDev Wiki - Paging](https://wiki.osdev.org/Paging)
- [Linux Kernel Documentation](https://www.kernel.org/doc/)

---

## 💡 Conceptos Avanzados (Para Explorar Después)

- **Copy-on-Write (COW)**: Optimización en fork()
- **Demand Paging**: Cargar páginas solo cuando se necesitan
- **Page Faults**: Qué pasa cuando se accede a una página no presente
- **Swapping**: Mover páginas entre RAM y disco
- **TLB (Translation Lookaside Buffer)**: Cache de traducciones
- **Huge Pages**: Páginas más grandes para mejor rendimiento
- **Memory-Mapped Files**: Mapear archivos directamente en memoria

---

## 🤝 Contribuciones

Este es un proyecto educativo. Siéntete libre de:
- Agregar más ejemplos
- Mejorar las explicaciones
- Crear ejercicios adicionales
- Reportar errores o confusiones

---

## 📄 Licencia

Material educativo de uso libre para fines académicos.

