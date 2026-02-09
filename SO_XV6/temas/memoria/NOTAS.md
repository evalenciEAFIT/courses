# 📝 Notas de Clase - Memoria Virtual

## 🎯 Objetivos de Aprendizaje

Al completar este módulo, deberías poder:

1. ✅ Explicar qué es la memoria virtual y por qué es importante
2. ✅ Identificar los diferentes segmentos de memoria de un proceso
3. ✅ Comprender cómo funciona la traducción de direcciones virtuales a físicas
4. ✅ Usar mmap() para mapear memoria con permisos específicos
5. ✅ Entender Copy-on-Write y su rol en fork()
6. ✅ Explicar los mecanismos de protección de memoria

---

## 📖 Conceptos Teóricos

### 1. ¿Por qué Memoria Virtual?

**Problema sin memoria virtual:**
- Programas acceden directamente a memoria física
- Un programa puede sobrescribir memoria de otro
- Difícil ejecutar programas más grandes que la RAM
- No hay aislamiento ni protección

**Solución: Memoria Virtual**
- Cada proceso ve su propio espacio de direcciones
- El kernel + MMU traducen direcciones virtuales → físicas
- Protección automática entre procesos
- Permite swapping (usar disco como extensión de RAM)

### 2. Traducción de Direcciones

```
Programa usa dirección virtual: 0x7fff12345678
                ↓
        MMU consulta tabla de páginas
                ↓
        Encuentra marco físico correspondiente
                ↓
Acceso real a memoria física: 0x0abcd678
```

**Componentes:**
- **VPN (Virtual Page Number)**: Número de página virtual
- **Offset**: Desplazamiento dentro de la página
- **PPN (Physical Page Number)**: Número de marco físico

**Ejemplo con páginas de 4KB:**
```
Dirección virtual: 0x00001234
                   ├─────┬────┤
                   VPN   Offset
                   (0x1) (0x234)

Tabla de páginas: VPN 0x1 → PPN 0x5

Dirección física:  0x00005234
                   ├─────┬────┤
                   PPN   Offset
                   (0x5) (0x234)
```

### 3. Page Faults

Un **page fault** ocurre cuando:
- Se accede a una página no presente en RAM
- Se viola un permiso (escribir en página read-only)
- Se accede a dirección no mapeada

**Tipos de Page Faults:**

1. **Minor Page Fault**: Página está en RAM pero no en tabla de páginas
   - Solución: Actualizar tabla de páginas
   - Rápido (microsegundos)

2. **Major Page Fault**: Página está en disco (swap)
   - Solución: Cargar desde disco a RAM
   - Lento (milisegundos)

3. **Invalid Page Fault**: Acceso ilegal
   - Solución: Enviar SIGSEGV al proceso
   - Programa termina (si no hay handler)

### 4. Copy-on-Write (COW)

**Optimización en fork():**

```
Antes de fork():
Padre: [Página A] [Página B] [Página C]

Después de fork() (sin COW):
Padre: [Página A] [Página B] [Página C]
Hijo:  [Copia A] [Copia B] [Copia C]  ← Costoso!

Después de fork() (con COW):
Padre: [Página A] [Página B] [Página C]
         ↑          ↑          ↑
Hijo:  ──┴──────────┴──────────┴──────  ← Comparten!

Cuando hijo escribe en Página B:
Padre: [Página A] [Página B] [Página C]
Hijo:  [Página A] [Copia B'] [Página C] ← Solo se copia B
```

**Ventajas:**
- fork() es O(1) en tiempo (no depende del tamaño del proceso)
- Ahorra memoria (solo se copian páginas modificadas)
- Eficiente para patrón fork() + exec()

### 5. TLB (Translation Lookaside Buffer)

**Problema:** Consultar tabla de páginas en cada acceso es lento

**Solución:** Cache de traducciones recientes

```
Acceso a memoria virtual
        ↓
    ¿Está en TLB?
    ↙          ↘
  Sí (hit)    No (miss)
    ↓            ↓
  Rápido    Consultar tabla
  (1 ciclo)  (10-100 ciclos)
              ↓
          Actualizar TLB
```

**Características:**
- Muy pequeño (64-512 entradas)
- Muy rápido (hardware)
- Hit rate típico: 95-99%
- Se limpia en cambio de contexto

---

## 💻 Experimentos Prácticos

### Experimento 1: Observar ASLR

```bash
# Ejecuta varias veces y compara direcciones
./mem1 | grep "TEXT"
./mem1 | grep "TEXT"
./mem1 | grep "TEXT"

# Deshabilitar ASLR (requiere root)
sudo sysctl -w kernel.randomize_va_space=0
./mem1 | grep "TEXT"  # Direcciones fijas

# Rehabilitar ASLR
sudo sysctl -w kernel.randomize_va_space=2
```

### Experimento 2: Ver el Mapa de Memoria

```bash
# Ejecuta el programa en background
./mem1 &
PID=$!

# Ve el mapa completo
cat /proc/$PID/maps

# Busca segmentos específicos
cat /proc/$PID/maps | grep stack
cat /proc/$PID/maps | grep heap
cat /proc/$PID/maps | grep mem1  # Ejecutable

# Limpia
kill $PID
```

**Formato de /proc/PID/maps:**
```
dirección_inicio-dirección_fin permisos offset device inode pathname
55555555-55556000 r-xp 00000000 08:01 12345  /path/to/mem1
                  ↑
                  r=read, w=write, x=execute, p=private
```

### Experimento 3: Provocar Page Fault

```c
// Agrega esto a mem1.c para experimentar
void demo_page_fault() {
    // Mapear 1GB de memoria (no se asigna físicamente)
    size_t huge_size = 1024 * 1024 * 1024;  // 1GB
    char* huge_mem = mmap(NULL, huge_size,
                          PROT_READ | PROT_WRITE,
                          MAP_PRIVATE | MAP_ANONYMOUS,
                          -1, 0);
    
    printf("Mapeado 1GB en: %p\n", huge_mem);
    
    // Primer acceso: causa page fault (demand paging)
    huge_mem[0] = 'A';  // Solo se asigna 1 página física
    
    // Acceso a otra página: otro page fault
    huge_mem[PAGE_SIZE] = 'B';
    
    munmap(huge_mem, huge_size);
}
```

### Experimento 4: Medir Overhead de fork()

```c
#include <time.h>

void benchmark_fork() {
    struct timespec start, end;
    
    clock_gettime(CLOCK_MONOTONIC, &start);
    
    pid_t pid = fork();
    if (pid == 0) {
        exit(0);  // Hijo termina inmediatamente
    }
    
    clock_gettime(CLOCK_MONOTONIC, &end);
    wait(NULL);
    
    long ns = (end.tv_sec - start.tv_sec) * 1000000000L +
              (end.tv_nsec - start.tv_nsec);
    
    printf("fork() tomó: %ld nanosegundos\n", ns);
}
```

---

## 🔍 Comandos Útiles para Análisis

### Ver Uso de Memoria

```bash
# Memoria del sistema
free -h

# Memoria de un proceso
ps aux | grep mem1
top -p $(pgrep mem1)

# Detalle de memoria de un proceso
cat /proc/$(pgrep mem1)/status | grep -i mem
```

### Herramientas de Debugging

```bash
# Valgrind: detectar memory leaks
valgrind --leak-check=full ./mem1

# strace: ver syscalls
strace -e mmap,munmap,mprotect ./mem1

# pmap: mapa de memoria
pmap -x $(pgrep mem1)

# /proc/meminfo: info del sistema
cat /proc/meminfo
```

### Estadísticas de Páginas

```bash
# Ver page faults de un comando
/usr/bin/time -v ./mem1 2>&1 | grep fault

# Estadísticas del sistema
vmstat 1 5  # Actualiza cada 1 seg, 5 veces
```

---

## 📊 Tabla Comparativa

| Característica | malloc() | mmap() |
|---------------|----------|--------|
| **Tipo** | Función de biblioteca | Syscall |
| **Uso** | Memoria pequeña/mediana | Memoria grande, archivos |
| **Overhead** | Bajo | Medio (syscall) |
| **Granularidad** | Bytes | Páginas (4KB) |
| **Permisos** | R/W fijos | Configurables |
| **Compartir** | No | Sí (MAP_SHARED) |
| **Archivos** | No | Sí (memory-mapped files) |

---

## 🎓 Preguntas de Repaso

### Nivel Básico

1. ¿Qué es una dirección virtual?
2. ¿Cuáles son los segmentos principales de un proceso?
3. ¿Qué es un page fault?
4. ¿Qué significa ASLR?

### Nivel Intermedio

5. ¿Cómo funciona Copy-on-Write en fork()?
6. ¿Cuál es la diferencia entre malloc() y mmap()?
7. ¿Qué hace la MMU (Memory Management Unit)?
8. ¿Por qué el stack crece hacia abajo?

### Nivel Avanzado

9. ¿Cómo se implementa demand paging?
10. ¿Qué es el TLB y por qué es importante?
11. ¿Cómo funciona W^X (Write XOR Execute)?
12. ¿Qué son las huge pages y cuándo usarlas?

---

## 📚 Recursos Adicionales

### Lecturas Recomendadas

- **Operating Systems: Three Easy Pieces** (Capítulos 13-23)
  - Virtualization: Address Spaces, Segmentation, Paging
  
- **The Linux Programming Interface** (Capítulo 49)
  - Memory Mappings
  
- **Understanding the Linux Virtual Memory Manager**
  - Mel Gorman (libro completo online)

### Videos Educativos

- MIT 6.828: Operating System Engineering
- UC Berkeley CS162: Operating Systems

### Documentación Linux

```bash
man mmap
man mprotect
man fork
man proc  # /proc filesystem
```

---

## ✏️ Ejercicios Propuestos

### Ejercicio 1: Implementar un Allocator Simple
Crea tu propio `my_malloc()` usando mmap().

### Ejercicio 2: Memoria Compartida
Usa mmap() con MAP_SHARED para comunicar dos procesos.

### Ejercicio 3: Memory-Mapped File
Mapea un archivo grande en memoria y procésalo.

### Ejercicio 4: Profiling de Page Faults
Mide cuántos page faults genera tu programa.

### Ejercicio 5: Implementar COW Manual
Simula Copy-on-Write usando mprotect().

---

**Última actualización:** 2026-02-07  
**Autor:** Material educativo para Sistemas Operativos
