# Benchmark: I/O con Librería Estándar vs. System Calls
**Sistemas Operativos — Comparativa de Rendimiento en E/S de Archivos**

---

## Objetivo

Comparar el tiempo de escritura de **1,000,000 bytes** en un archivo usando tres estrategias distintas:

| # | Estrategia | Archivo |
|---|-----------|---------|
| 1 | Librería C Estándar (`fputc`) | `lib_io.c` |
| 2 | System Call directa, 1 byte a la vez (`write`) | `syscall_io.c` |
| 3 | System Call con buffer manual (bloques de 4 KB) | `syscall_buffered.c` |

---

## Resultados Obtenidos

```
1. Librería C Estándar (fputc)   -> Tiempo: 0.003505 segundos
2. System Call puros (write 1b)  -> Tiempo: 1.145280 segundos
3. System Call (+Buffer 4KB)     -> Tiempo: 0.000723 segundos
```

### Comparativa visual

```
Sistema Call puro  [===========================================] 1.145280 s  (más lento)
Librería C         [=]                                          0.003505 s
System Call+Buffer []                                           0.000723 s  (más RÁPIDO)
```

- El **System Call puro** fue **~326× más lento** que la librería.
- El **System Call con buffer** fue **~5× más rápido** que la librería.
- El **System Call puro** fue **~1584× más lento** que la versión con buffer.

---

## Conceptos Clave

### ¿Qué es un System Call (Llamada al Sistema)?

Un **System Call** es el mecanismo por el cual un programa en **User Mode** (modo usuario) solicita un servicio al **Kernel** (núcleo del Sistema Operativo) que está en **Kernel Mode** (modo privilegiado).

```
┌─────────────────────────────────────┐
│           ESPACIO DE USUARIO        │
│   Tu programa C (User Mode)         │
│         write(fd, &c, 1);           │
└───────────────┬─────────────────────┘
                │  ↕ System Call (cambio de contexto)
┌───────────────▼─────────────────────┐
│           ESPACIO DEL KERNEL        │
│   Kernel del SO (Kernel Mode)       │
│   - Valida el proceso               │
│   - Accede al hardware (disco)      │
│   - Retorna el control al usuario   │
└─────────────────────────────────────┘
```

Cada **cambio de contexto** (User Mode → Kernel Mode → User Mode) tiene un costo de tiempo no despreciable.

---

## Análisis por Estrategia

### Estrategia 1 — Librería C Estándar (`fputc`)

**Archivo:** `lib_io.c`  
**Tiempo:** `0.003505 s`

```c
for (int i = 0; i < BYTES_TO_WRITE; i++) {
    fputc('A', file);
}
```

**¿Por qué es rápido si llama `fputc` 1,000,000 de veces?**

La función `fputc` pertenece a la librería estándar de C (`stdio.h`) y **no realiza un System Call por cada byte**. Internamente utiliza un **buffer de la librería** (generalmente de 8 KB) que acumula los datos. El System Call `write` al kernel solo se llama cuando:
- El buffer interno se llena.
- Se llama a `fflush()` explícitamente.
- Se llama a `fclose()`.

Esto resulta en aproximadamente **~125 System Calls** reales (1,000,000 / 8,192 ≈ 122) en lugar de 1,000,000.

```
fputc() x1,000,000 → Buffer interno (8KB)
                            │
                            ▼  (cuando se llena)
                      write() → Kernel
```

---

### Estrategia 2 — System Call Directa 1 byte a la vez (`write`)

**Archivo:** `syscall_io.c`  
**Tiempo:** `1.145280 s` 🐌

```c
char c = 'A';
for (int i = 0; i < BYTES_TO_WRITE; i++) {
    write(fd, &c, 1);  // ← System Call por CADA byte
}
```

**¿Por qué es tan lento?**

Este es el **peor caso posible** para I/O. Cada llamada a `write()` provoca:

1. Guardar el contexto del proceso (registros de CPU, Program Counter, etc.)
2. Cambiar al modo Kernel.
3. Ejecutar la lógica del kernel para escribir 1 solo byte.
4. Restaurar el contexto y regresar a modo usuario.

Esto sucede **1,000,000 de veces**. El overhead del cambio de contexto es tan alto que domina el tiempo total, mientras que el dato real escrito (1 byte) es insignificante.

> **Regla fundamental de SO:** *Minimizar las llamadas al sistema es crucial para el rendimiento.*

---

### Estrategia 3 — System Call con Buffer Manual (Bloques 4KB)

**Archivo:** `syscall_buffered.c`  
**Tiempo:** `0.000723 s` ⚡

```c
char buffer[BUFFER_SIZE]; // 4096 bytes
memset(buffer, 'A', BUFFER_SIZE);

int remaining = BYTES_TO_WRITE;
while (remaining > 0) {
    int to_write = remaining < BUFFER_SIZE ? remaining : BUFFER_SIZE;
    write(fd, buffer, to_write);  // ← System Call con bloques de 4KB
    remaining -= to_write;
}
```

**¿Por qué es el más rápido?**

En lugar de 1,000,000 System Calls, se realizan solo **~245 llamadas** (1,000,000 / 4,096 ≈ 245). Cada llamada transmite 4,096 bytes, maximizando el trabajo útil por cada costoso cambio de contexto.

Además, **4 KB coincide con el tamaño de una página de memoria** en Linux, lo que hace que la transferencia sea naturalmente eficiente para el kernel.

```
write(4096 bytes) × ~245 veces  →  Kernel  →  Disco
```

---

## ¿Por qué la Librería es más lenta que el Buffer manual?

Aunque ambos usan buffers, la librería estándar tiene un overhead adicional:
- Funciones de formato y validación interna.
- Manejo de estructuras `FILE *` (locks, estado interno, etc.).
- Escritura en dos pasos: primero al buffer de la librería, luego al kernel.

El buffer manual de 4KB llama directamente al kernel sin capas intermedias.

---

## Resumen de Conceptos del SO

| Concepto | Descripción |
|----------|-------------|
| **User Mode** | Modo de ejecución restringido para programas normales |
| **Kernel Mode** | Modo privilegiado del SO con acceso total al hardware |
| **System Call** | Mecanismo de solicitud de servicios al Kernel |
| **Context Switch** | Cambio entre User Mode y Kernel Mode (costo de tiempo) |
| **Buffer de Librería** | Memoria temporal en User Space para agrupar escrituras |
| **Buffer manual** | Técnica de agrupar datos antes de hacer un System Call |
| **Página de Memoria** | Unidad básica de memoria en Linux = 4 KB |

---

## Conclusión

> La **librería estándar de C** es conveniente y ya optimizada con buffers internos, siendo una buena opción por defecto.
>
> Los **System Calls directos** son la base de todo I/O en Linux, pero usarlos para operaciones pequeñas y repetitivas es extremadamente costoso.
>
> La **mejor performance** se logra agrupando datos en buffers del tamaño de una página (4 KB o múltiplos) antes de cada System Call, minimizando los cambios de contexto.

---

## Compilar y Ejecutar

```bash
chmod +x run.sh
./run.sh
```

O manualmente:

```bash
gcc -O2 lib_io.c        -o lib_io
gcc -O2 syscall_io.c    -o syscall_io
gcc -O2 syscall_buffered.c -o syscall_buffered

./lib_io
./syscall_io
./syscall_buffered
```

---

## Estructura del Proyecto

```
io/
├── lib_io.c             # Escritura con librería C estándar (fputc)
├── syscall_io.c         # Escritura con System Call puro (1 byte)
├── syscall_buffered.c   # Escritura con System Call + buffer manual
├── run.sh               # Script de compilación y ejecución
└── README.md            # Este archivo (documentación)
```
