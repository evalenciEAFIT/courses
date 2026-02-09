# 📘 Solución Ejercicio 1: Implementar un Allocator Simple

## 🎯 Objetivo

Implementar un sistema de asignación de memoria dinámica personalizado usando `mmap()` en lugar de `malloc()`, para comprender cómo funcionan los allocators por dentro.

---

## 📁 Archivos

- **`ejercicio1_solucion.c`** - Solución completa con 3 versiones
- **`Makefile`** - Sistema de compilación automatizado
- **`README.md`** - Este archivo (documentación)

---

## 🔧 Compilación y Ejecución

### Opción 1: Usando Makefile (Recomendado)

```bash
# Compilar versión básica
make basica

# Compilar versión intermedia
make intermedia

# Compilar versión avanzada (por defecto)
make avanzada

# Ejecutar
make run

# Ejecutar con valgrind
make valgrind

# Limpiar
make clean
```

### Opción 2: Compilación Manual

```bash
# Versión básica
gcc -Wall -Wextra -DVERSION_BASICA -o ejercicio1_sol ejercicio1_solucion.c

# Versión intermedia
gcc -Wall -Wextra -DVERSION_INTERMEDIA -o ejercicio1_sol ejercicio1_solucion.c

# Versión avanzada
gcc -Wall -Wextra -DVERSION_AVANZADA -o ejercicio1_sol ejercicio1_solucion.c

# Ejecutar
./ejercicio1_sol
```

---

## 📚 Explicación de la Solución

### 🏗️ Arquitectura General

La solución implementa un allocator basado en **metadata inline**, donde cada bloque de memoria tiene una estructura de control al inicio:

```
┌──────────────────┬────────────────────────────────┐
│  block_metadata  │      User Data Area            │
│  (16 bytes)      │      (tamaño solicitado)       │
└──────────────────┴────────────────────────────────┘
^                  ^
|                  |
real_ptr           ptr retornado al usuario
```

### 📦 Estructura de Metadata

```c
typedef struct block_metadata {
    size_t total_size;      // Tamaño total del bloque (incluyendo metadata)
    uint32_t magic;         // Número mágico para validación (0xDEADBEEF)
    uint32_t padding;       // Padding para alineación a 8 bytes
} block_metadata_t;
```

**Campos:**
- **`total_size`**: Tamaño total mapeado (redondeado a PAGE_SIZE)
- **`magic`**: Número mágico `0xDEADBEEF` para detectar corrupción
- **`padding`**: Asegura alineación a 8 bytes (importante para rendimiento)

---

## 🔍 Implementación Detallada

### 1️⃣ `my_malloc(size_t size)`

**Algoritmo:**

1. **Validar entrada**: Si `size == 0`, retornar `NULL`
2. **Calcular tamaño total**: `metadata + size`
3. **Detectar overflow**: Verificar que no haya desbordamiento
4. **Redondear a PAGE_SIZE**: Alinear a límites de página (4096 bytes)
5. **Mapear memoria**: Usar `mmap()` con `MAP_PRIVATE | MAP_ANONYMOUS`
6. **Inicializar metadata**: Guardar tamaño y número mágico
7. **Retornar puntero**: Apuntar después de la metadata

**Código clave:**

```c
size_t total_size = sizeof(block_metadata_t) + size;
size_t rounded_size = round_to_page_size(total_size);

void* mem = mmap(NULL, rounded_size, PROT_READ | PROT_WRITE,
                 MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

block_metadata_t* meta = (block_metadata_t*)mem;
meta->total_size = rounded_size;
meta->magic = MAGIC_NUMBER;

return (char*)mem + sizeof(block_metadata_t);
```

**Complejidad:** O(1)  
**Llamadas al sistema:** 1 (`mmap`)

---

### 2️⃣ `my_free(void* ptr)`

**Algoritmo:**

1. **Validar entrada**: Si `ptr == NULL`, retornar
2. **Retroceder a metadata**: `real_ptr = ptr - sizeof(metadata)`
3. **Validar metadata**: Verificar número mágico
4. **Recuperar tamaño**: Leer `meta->total_size`
5. **Invalidar metadata**: Poner `magic = 0` (detectar double-free)
6. **Liberar memoria**: Usar `munmap()`

**Código clave:**

```c
void* real_ptr = (char*)ptr - sizeof(block_metadata_t);
block_metadata_t* meta = (block_metadata_t*)real_ptr;

if (meta->magic != MAGIC_NUMBER) {
    fprintf(stderr, "ERROR: Bloque corrupto!\n");
    abort();
}

size_t size = meta->total_size;
meta->magic = 0;  // Invalidar

munmap(real_ptr, size);
```

**Complejidad:** O(1)  
**Llamadas al sistema:** 1 (`munmap`)

---

### 3️⃣ `my_calloc(size_t nmemb, size_t size)`

**Algoritmo:**

1. **Validar entrada**: `nmemb > 0` y `size > 0`
2. **Detectar overflow**: Verificar `nmemb * size`
3. **Calcular tamaño total**: `total = nmemb * size`
4. **Asignar con my_malloc()**
5. **Inicializar a cero**: `memset()` (aunque `mmap` ya retorna ceros)

**Detección de overflow:**

```c
if (size > SIZE_MAX / nmemb) {
    errno = ENOMEM;
    return NULL;  // Overflow detectado
}
```

---

### 4️⃣ `my_realloc(void* ptr, size_t new_size)` (BONUS)

**Casos especiales:**

- **`ptr == NULL`**: Equivalente a `my_malloc(new_size)`
- **`new_size == 0`**: Equivalente a `my_free(ptr)`
- **`new_size <= tamaño actual`**: Retornar mismo puntero (optimización)
- **`new_size > tamaño actual`**: Asignar nuevo bloque, copiar datos, liberar antiguo

**Código clave:**

```c
if (new_size <= current_size) {
    return ptr;  // Reutilizar bloque actual
}

void* new_ptr = my_malloc(new_size);
memcpy(new_ptr, ptr, current_size);
my_free(ptr);
return new_ptr;
```

---

## 🎨 Versiones Implementadas

### 🟢 Versión BÁSICA

**Características:**
- Implementación mínima funcional
- Metadata inline para guardar tamaño
- Sin validaciones avanzadas
- Ideal para entender el concepto básico

**Compilar:**
```bash
make basica
```

---

### 🟡 Versión INTERMEDIA

**Características:**
- ✅ Todo de la versión básica
- ✅ Validación de metadata (número mágico)
- ✅ Detección de bloques corruptos
- ✅ Implementación de `my_realloc()`
- ✅ Mejor manejo de errores

**Compilar:**
```bash
make intermedia
```

---

### 🔴 Versión AVANZADA (Recomendada)

**Características:**
- ✅ Todo de la versión intermedia
- ✅ **Estadísticas en tiempo real**:
  - Total de bytes asignados/liberados
  - Número de asignaciones/liberaciones
  - Pico de memoria usado
  - Fragmentación interna
- ✅ **Detección de memory leaks**
- ✅ **Debugging detallado** (mensajes de log)
- ✅ **Validaciones exhaustivas**

**Compilar:**
```bash
make avanzada
```

**Salida de ejemplo:**

```
╔════════════════════════════════════════════════════════════════╗
║  ESTADÍSTICAS DEL ALLOCATOR                                    ║
╚════════════════════════════════════════════════════════════════╝

  Total asignado actualmente:  0 bytes
  Total liberado:              53248 bytes
  Número de asignaciones:      7
  Número de liberaciones:      7
  Pico de memoria:             53248 bytes (52.00 KB)
  Bytes desperdiciados:        44064 bytes (fragmentación interna)

  ✅ Sin memory leaks detectados
```

---

## 🧪 Tests Incluidos

El programa incluye 5 baterías de tests:

### Test 1: Asignación Básica
- Asignar 100 bytes
- Escribir datos
- Liberar memoria

### Test 2: Múltiples Asignaciones
- Asignar 5 bloques de diferentes tamaños (16B, 256B, 1KB, 4KB, 8KB)
- Liberar todos en orden

### Test 3: calloc
- Asignar array de 10 enteros
- Verificar inicialización a cero
- Modificar y liberar

### Test 4: Casos Límite
- `my_malloc(0)` → debe retornar `NULL`
- `my_free(NULL)` → no debe crashear
- `my_malloc(10GB)` → debe fallar apropiadamente

### Test 5: realloc (Versiones Intermedia/Avanzada)
- Asignar 10 bytes
- Expandir a 50 bytes
- Verificar que datos se preservan

---

## 🔬 Verificación con Valgrind

Para verificar que no hay memory leaks:

```bash
make valgrind
```

**Salida esperada:**

```
==12345== HEAP SUMMARY:
==12345==     in use at exit: 0 bytes in 0 blocks
==12345==   total heap usage: 0 allocs, 0 frees, 0 bytes allocated
==12345== 
==12345== All heap blocks were freed -- no leaks are possible
```

**Nota:** Valgrind no reporta `mmap()` como "heap", pero sí detecta si no se llama a `munmap()`.

---

## 📊 Análisis de Fragmentación

### Fragmentación Interna

Ocurre porque redondeamos al múltiplo de `PAGE_SIZE` (4096 bytes).

**Ejemplo:**

```
Solicitud:  100 bytes
Metadata:   16 bytes
Total:      116 bytes
Redondeado: 4096 bytes
Desperdicio: 4096 - 116 = 3980 bytes (96.7% desperdiciado!)
```

**Solución (no implementada aquí):**
- Usar un **free list** para reutilizar bloques
- Implementar **buddy allocator** o **slab allocator**
- Usar `brk()` para bloques pequeños y `mmap()` solo para grandes

---

## 🎓 Conceptos Aprendidos

### 1. Gestión de Memoria a Bajo Nivel
- Cómo funciona `mmap()` y `munmap()`
- Diferencia entre memoria virtual y física
- Alineación a límites de página

### 2. Metadata Management
- Cómo guardar información de control
- Trade-off entre overhead y funcionalidad
- Validación de integridad (magic numbers)

### 3. Detección de Errores
- Overflow en multiplicaciones
- Bloques corruptos
- Double-free
- Memory leaks

### 4. Optimizaciones
- Reutilización de bloques en `realloc()`
- Alineación de memoria
- Minimización de syscalls

---

## 🚀 Desafíos Adicionales (No Implementados)

Si quieres llevar este proyecto más allá, intenta:

### 1. Free List
Mantener una lista de bloques liberados para reutilizarlos:

```c
typedef struct free_block {
    size_t size;
    struct free_block* next;
} free_block_t;
```

### 2. Coalescing
Fusionar bloques adyacentes liberados:

```
Antes:  [LIBRE 4KB] [LIBRE 4KB] [USADO 4KB]
Después: [LIBRE 8KB] [USADO 4KB]
```

### 3. Best-Fit / First-Fit
Implementar diferentes estrategias de búsqueda en el free list.

### 4. Thread Safety
Agregar mutexes para uso en programas multi-threaded:

```c
pthread_mutex_t alloc_mutex = PTHREAD_MUTEX_INITIALIZER;
```

### 5. Memory Pool
Pre-asignar grandes bloques y sub-dividirlos:

```c
void* pool = mmap(..., 1024 * PAGE_SIZE, ...);
```

---

## 📖 Referencias

### Documentación
- [mmap(2) - Linux man page](https://man7.org/linux/man-pages/man2/mmap.2.html)
- [munmap(2) - Linux man page](https://man7.org/linux/man-pages/man2/munmap.2.html)

### Lecturas Recomendadas
- **"The Linux Programming Interface"** - Michael Kerrisk (Capítulo 49)
- **"Understanding the Linux Kernel"** - Daniel P. Bovet
- **"Operating Systems: Three Easy Pieces"** - Remzi H. Arpaci-Dusseau

### Allocators Reales
- **glibc malloc**: [Código fuente](https://sourceware.org/git/?p=glibc.git;a=blob;f=malloc/malloc.c)
- **jemalloc**: [Documentación](http://jemalloc.net/)
- **tcmalloc**: [Google Performance Tools](https://github.com/google/tcmalloc)

---

## ✅ Checklist de Aprendizaje

Después de completar este ejercicio, deberías poder:

- [ ] Explicar cómo funciona `mmap()` y sus parámetros
- [ ] Implementar un allocator básico desde cero
- [ ] Entender el concepto de metadata inline
- [ ] Detectar y prevenir overflow en operaciones aritméticas
- [ ] Usar `valgrind` para detectar memory leaks
- [ ] Calcular fragmentación interna
- [ ] Explicar las ventajas y desventajas de diferentes estrategias de allocación

---

## 🤝 Contribuciones

Este es material educativo. Siéntete libre de:
- Mejorar la implementación
- Agregar más tests
- Implementar los desafíos adicionales
- Crear visualizaciones del estado del allocator

---

## 📄 Licencia

Material educativo de uso libre para fines académicos.

---

**¡Feliz aprendizaje sobre gestión de memoria! 🚀**
