/*
 * ============================================================================
 * MEMORIA VIRTUAL EN LINUX - PROGRAMA EDUCATIVO
 * ============================================================================
 * 
 * Propósito:
 *   Este programa demuestra los conceptos fundamentales de memoria virtual
 *   en sistemas operativos Linux mediante ejemplos prácticos.
 * 
 * Conceptos Cubiertos:
 *   1. Espacio de direcciones virtuales (segmentos de memoria)
 *   2. Mapeo de memoria con mmap()
 *   3. Separación de espacios de direcciones entre procesos
 *   4. Protección de páginas de memoria
 * 
 * Autor: Material educativo para Sistemas Operativos
 * Compilación: gcc -Wall -Wextra -o mem1 mem1.c
 * Ejecución: ./mem1
 * ============================================================================
 */

// Definir _DEFAULT_SOURCE para tener acceso a usleep()
#define _DEFAULT_SOURCE

#include <stdio.h>      // printf, perror
#include <stdlib.h>     // malloc, free, exit
#include <unistd.h>     // getpid, fork, usleep
#include <sys/mman.h>   // mmap, munmap, mprotect
#include <sys/wait.h>   // wait
#include <string.h>     // strcpy, memset
#include <errno.h>      // errno

/*
 * Constantes del Sistema
 * ----------------------
 * PAGE_SIZE: Tamaño típico de una página en sistemas x86/x64 (4KB)
 * En producción, usar sysconf(_SC_PAGESIZE) para obtener el valor real
 */
#define PAGE_SIZE 4096

// Declaración forward de main para poder obtener su dirección
int main(void);

/*
 * ============================================================================
 * FUNCIÓN AUXILIAR: Mostrar Información de Direcciones
 * ============================================================================
 * 
 * Propósito:
 *   Imprime una dirección de memoria virtual en formato hexadecimal.
 * 
 * Parámetros:
 *   - label: Etiqueta descriptiva (ej: "Stack variable")
 *   - addr:  Puntero a la dirección de memoria a mostrar
 * 
 * Nota Educativa:
 *   Las direcciones mostradas son VIRTUALES, no físicas. El kernel y la MMU
 *   (Memory Management Unit) se encargan de traducirlas a direcciones físicas
 *   de forma transparente para el programa.
 * 
 *   Formato típico en x86_64: 0x7fffffffxxxx (stack), 0x55555555xxxx (código)
 * ============================================================================
 */
void print_address_info(const char* label, void* addr) {
    printf("  %-35s %p\n", label, addr);
}

/*
 * ============================================================================
 * DEMOSTRACIÓN 2: Mapeo de Memoria con mmap()
 * ============================================================================
 * 
 * Concepto:
 *   mmap() es una llamada al sistema que permite mapear regiones de memoria
 *   virtual directamente, ofreciendo más control que malloc().
 * 
 * Diferencias entre mmap() y malloc():
 * 
 *   malloc():
 *     - Función de biblioteca (libc)
 *     - Gestiona el heap del proceso
 *     - Más simple de usar
 *     - Usa internamente brk() o mmap()
 * 
 *   mmap():
 *     - Llamada al sistema (syscall)
 *     - Mapea memoria directamente desde el kernel
 *     - Más control (permisos, flags, ubicación)
 *     - Puede mapear archivos o memoria anónima
 * 
 * Prototipo de mmap():
 *   void* mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);
 * 
 * Parámetros:
 *   - addr:   Dirección sugerida (NULL = kernel elige)
 *   - length: Tamaño de la región a mapear
 *   - prot:   Permisos (PROT_READ, PROT_WRITE, PROT_EXEC, PROT_NONE)
 *   - flags:  Tipo de mapeo (MAP_PRIVATE, MAP_SHARED, MAP_ANONYMOUS, etc.)
 *   - fd:     File descriptor (para mapear archivos, -1 para memoria anónima)
 *   - offset: Offset en el archivo (0 para memoria anónima)
 * 
 * Casos de Uso:
 *   - Memoria compartida entre procesos (MAP_SHARED)
 *   - Mapear archivos grandes en memoria (memory-mapped files)
 *   - Alocar grandes bloques de memoria
 *   - Control fino sobre permisos de páginas
 * ============================================================================
 */
void demo_2_mmap() {
    printf("\n");
    printf("╔════════════════════════════════════════════════════════════════════╗\n");
    printf("║  DEMOSTRACIÓN 2: Mapeo de Memoria con mmap()                      ║\n");
    printf("╚════════════════════════════════════════════════════════════════════╝\n");
    printf("\nConcepto: Reservar regiones de memoria virtual con control fino\n\n");
    
    // Tamaño de la región: 4 páginas (16 KB)
    size_t region_size = 4 * PAGE_SIZE;
    
    printf("Solicitando al kernel: %zu bytes (%zu páginas de %d bytes)\n", 
           region_size, region_size / PAGE_SIZE, PAGE_SIZE);
    
    /*
     * Llamada a mmap() con los siguientes parámetros:
     * 
     * NULL:              Dejamos que el kernel elija la dirección
     * region_size:       Tamaño de la región (4 páginas = 16KB)
     * PROT_READ|WRITE:   Permisos de lectura y escritura
     * MAP_PRIVATE:       Mapeo privado (no compartido con otros procesos)
     * MAP_ANONYMOUS:     Memoria anónima (no respaldada por archivo)
     * -1:                No hay file descriptor (memoria anónima)
     * 0:                 No hay offset (memoria anónima)
     */
    void* mapped_mem = mmap(
        NULL,                           // Dirección: NULL = kernel elige
        region_size,                    // Tamaño: 16 KB
        PROT_READ | PROT_WRITE,        // Permisos: lectura + escritura
        MAP_PRIVATE | MAP_ANONYMOUS,   // Flags: privado + anónimo
        -1,                            // FD: -1 para memoria anónima
        0                              // Offset: 0 para memoria anónima
    );
    
    // Verificar si mmap() tuvo éxito
    if (mapped_mem == MAP_FAILED) {
        perror("❌ mmap falló");
        return;
    }
    
    printf("\n✅ Memoria mapeada exitosamente:\n");
    printf("────────────────────────────────────────────────────────────────────\n");
    print_address_info("🔹 Dirección de inicio", mapped_mem);
    print_address_info("🔹 Dirección de fin", (char*)mapped_mem + region_size - 1);
    printf("🔹 Tamaño total: %zu bytes\n", region_size);
    
    // Usar la memoria mapeada
    printf("\n📝 Escribiendo en la memoria mapeada...\n");
    strcpy((char*)mapped_mem, "¡Hola desde memoria mapeada con mmap()!");
    printf("📖 Contenido almacenado: \"%s\"\n", (char*)mapped_mem);
    
    // Demostrar que podemos escribir en diferentes páginas
    char* page2 = (char*)mapped_mem + PAGE_SIZE;
    strcpy(page2, "Esto está en la segunda página");
    printf("📖 Página 2: \"%s\"\n", page2);
    
    printf("\n💡 Observaciones:\n");
    printf("   • mmap() reserva memoria directamente del kernel\n");
    printf("   • La memoria está alineada a límites de página\n");
    printf("   • Podemos especificar permisos exactos (lectura, escritura, ejecución)\n");
    printf("   • Útil para memoria compartida, archivos mapeados, y grandes bloques\n");
    
    /*
     * Liberar la memoria mapeada
     * 
     * munmap() devuelve la memoria al sistema operativo.
     * Es importante llamarlo para evitar memory leaks.
     */
    printf("\n🗑️  Liberando memoria con munmap()...\n");
    if (munmap(mapped_mem, region_size) == -1) {
        perror("munmap falló");
    }
}

/*
 * ============================================================================
 * DEMOSTRACIÓN 3: Separación de Espacios de Direcciones
 * ============================================================================
 * 
 * Concepto:
 *   Cada proceso tiene su propio espacio de direcciones PRIVADO. Aunque dos
 *   procesos puedan tener la misma dirección virtual, apuntan a diferentes
 *   ubicaciones en memoria física.
 * 
 * fork() y Copy-on-Write (COW):
 * 
 *   Cuando se llama a fork():
 *   1. Se crea un nuevo proceso (hijo) que es copia del padre
 *   2. Inicialmente, padre e hijo COMPARTEN las mismas páginas físicas
 *   3. Las páginas se marcan como "read-only" en ambos procesos
 *   4. Cuando cualquiera intenta ESCRIBIR, ocurre un "page fault"
 *   5. El kernel crea una COPIA de esa página (Copy-on-Write)
 *   6. Ahora cada proceso tiene su propia copia física
 * 
 *   Ventajas de COW:
 *     ✓ fork() es muy rápido (no copia toda la memoria inmediatamente)
 *     ✓ Ahorra memoria (solo se copian páginas modificadas)
 *     ✓ Eficiente para exec() después de fork()
 * 
 * Experimento:
 *   - Padre e hijo crean variables con el mismo valor inicial (42)
 *   - Observaremos que tienen la MISMA dirección virtual
 *   - Hijo modifica su variable a 100
 *   - Variable del padre permanece en 42 (espacios separados)
 * 
 * Esto demuestra:
 *   ✓ Aislamiento entre procesos
 *   ✓ Protección de memoria
 *   ✓ Independencia de espacios de direcciones
 * ============================================================================
 */
void demo_3_address_space_separation() {
    printf("\n");
    printf("╔════════════════════════════════════════════════════════════════════╗\n");
    printf("║  DEMOSTRACIÓN 3: Separación de Espacios de Direcciones            ║\n");
    printf("╚════════════════════════════════════════════════════════════════════╝\n");
    printf("\nConcepto: Cada proceso tiene su propio espacio de direcciones privado\n\n");
    
    printf("🔄 Creando proceso hijo con fork()...\n\n");
    
    /*
     * fork() crea un nuevo proceso duplicando el proceso actual.
     * 
     * Retorna:
     *   - 0 en el proceso hijo
     *   - PID del hijo en el proceso padre
     *   - -1 si hay error
     */
    pid_t pid = fork();
    
    if (pid == 0) {
        /*
         * ═══════════════════════════════════════════════════════════════
         * PROCESO HIJO
         * ═══════════════════════════════════════════════════════════════
         */
        printf("┌────────────────────────────────────────────────────────────────┐\n");
        printf("│ 👶 PROCESO HIJO                                                │\n");
        printf("└────────────────────────────────────────────────────────────────┘\n");
        printf("  PID: %d (Padre PID: %d)\n", getpid(), getppid());
        
        // Crear variable local en el hijo
        int child_var = 42;
        
        print_address_info("  Dirección de child_var", &child_var);
        printf("  Valor inicial: %d\n", child_var);
        
        // Pequeña pausa para que el output sea más claro
        usleep(100000);  // 100ms
        
        // Modificar la variable (esto dispara Copy-on-Write)
        printf("\n  ✏️  Modificando child_var de 42 a 100...\n");
        child_var = 100;
        printf("  ✅ Nuevo valor: %d\n", child_var);
        printf("  💡 Esta modificación NO afecta al padre (espacios separados)\n");
        
        printf("  🏁 Hijo terminando...\n\n");
        exit(0);
        
    } else if (pid > 0) {
        /*
         * ═══════════════════════════════════════════════════════════════
         * PROCESO PADRE
         * ═══════════════════════════════════════════════════════════════
         */
        printf("┌────────────────────────────────────────────────────────────────┐\n");
        printf("│ 👨 PROCESO PADRE                                               │\n");
        printf("└────────────────────────────────────────────────────────────────┘\n");
        printf("  PID: %d (Hijo PID: %d)\n", getpid(), pid);
        
        // Crear variable local en el padre (mismo valor inicial que el hijo)
        int parent_var = 42;
        
        print_address_info("  Dirección de parent_var", &parent_var);
        printf("  Valor inicial: %d\n", parent_var);
        
        printf("\n  ⏳ Esperando a que el hijo termine...\n\n");
        
        /*
         * wait() suspende la ejecución del padre hasta que el hijo termine.
         * Esto previene procesos "zombie" (hijos terminados sin recoger).
         */
        wait(NULL);
        
        printf("┌────────────────────────────────────────────────────────────────┐\n");
        printf("│ 👨 PADRE - Después de que el hijo terminó                     │\n");
        printf("└────────────────────────────────────────────────────────────────┘\n");
        printf("  Valor de parent_var: %d (sin cambios)\n", parent_var);
        
        printf("\n💡 Observaciones Clave:\n");
        printf("   • Padre e hijo tienen la MISMA dirección virtual\n");
        printf("   • Pero apuntan a DIFERENTES ubicaciones físicas (COW)\n");
        printf("   • Modificaciones del hijo NO afectan al padre\n");
        printf("   • Cada proceso está AISLADO y PROTEGIDO\n");
        
    } else {
        /*
         * Error en fork()
         */
        perror("❌ fork falló");
    }
}

/*
 * ============================================================================
 * DEMOSTRACIÓN 1: Mapeo de Memoria del Proceso
 * ============================================================================
 * 
 * Concepto:
 *   Cada proceso tiene un espacio de direcciones virtuales dividido en
 *   diferentes segmentos, cada uno con un propósito específico.
 * 
 * Segmentos de Memoria:
 * 
 *   ┌─────────────────────┐ ← Direcciones Altas (ej: 0x7fff...)
 *   │   STACK (Pila)      │   - Variables locales
 *   │                     │   - Parámetros de funciones
 *   │        ↓            │   - Crece hacia abajo
 *   ├─────────────────────┤
 *   │                     │
 *   │        ↑            │
 *   │   HEAP (Montículo)  │   - Memoria dinámica (malloc/free)
 *   │                     │   - Crece hacia arriba
 *   ├─────────────────────┤
 *   │   BSS               │   - Variables globales no inicializadas
 *   ├─────────────────────┤
 *   │   DATA              │   - Variables globales/estáticas inicializadas
 *   ├─────────────────────┤
 *   │   TEXT (Código)     │   - Instrucciones del programa (read-only)
 *   └─────────────────────┘ ← Direcciones Bajas (ej: 0x5555...)
 * 
 * Qué Observar:
 *   - Las direcciones del stack son más altas que las del heap
 *   - El código (text) tiene las direcciones más bajas
 *   - Cada ejecución puede tener direcciones diferentes (ASLR)
 * 
 * ASLR (Address Space Layout Randomization):
 *   Técnica de seguridad que aleatoriza las direcciones base de los segmentos
 *   para dificultar ataques de explotación de memoria.
 * ============================================================================
 */
void demo_1_memory_map() {
    printf("\n");
    printf("╔════════════════════════════════════════════════════════════════════╗\n");
    printf("║  DEMOSTRACIÓN 1: Mapeo de Memoria del Proceso                     ║\n");
    printf("╚════════════════════════════════════════════════════════════════════╝\n");
    printf("\nConcepto: Visualizar los diferentes segmentos del espacio de direcciones\n\n");
    
    /*
     * Creamos variables en diferentes segmentos de memoria para observar
     * cómo el sistema operativo organiza el espacio de direcciones.
     */
    
    // Variable ESTÁTICA (segmento DATA)
    // - Existe durante toda la vida del programa
    // - Se inicializa una sola vez
    // - Ubicada en el segmento de datos
    static int static_var = 10;
    
    // Variable LOCAL (segmento STACK)
    // - Existe solo durante la ejecución de esta función
    // - Se crea en la pila (stack frame)
    // - Se destruye al salir de la función
    int stack_var = 20;
    
    // Variable DINÁMICA (segmento HEAP)
    // - Memoria solicitada explícitamente con malloc()
    // - Persiste hasta que se llame free()
    // - El programador controla su ciclo de vida
    int* heap_var = malloc(sizeof(int));
    if (heap_var == NULL) {
        perror("malloc falló");
        return;
    }
    *heap_var = 30;
    
    printf("Direcciones de memoria (virtuales):\n");
    printf("────────────────────────────────────────────────────────────────────\n");
    
    // Obtener dirección de la función main (segmento TEXT)
    int (*main_ptr)(void) = &main;
    print_address_info("📄 TEXT (código - función main)", (void*)main_ptr);
    print_address_info("📊 DATA (variable estática)", &static_var);
    print_address_info("🎯 HEAP (memoria dinámica)", heap_var);
    print_address_info("📚 STACK (variable local)", &stack_var);
    
    printf("\n💡 Observaciones:\n");
    printf("   • Las direcciones del STACK son las más altas\n");
    printf("   • Las direcciones del TEXT son las más bajas\n");
    printf("   • HEAP crece hacia arriba, STACK crece hacia abajo\n");
    printf("   • Ejecuta el programa varias veces: las direcciones cambian (ASLR)\n");
    
    // Liberar memoria dinámica (buena práctica)
    free(heap_var);
}

/*
 * ============================================================================
 * DEMOSTRACIÓN 4: Protección de Páginas
 * ============================================================================
 * 
 * Concepto:
 *   El sistema operativo puede asignar permisos a cada página de memoria,
 *   controlando qué operaciones están permitidas (lectura, escritura, ejecución).
 * 
 * Permisos de Páginas (Protection Bits):
 * 
 *   PROT_NONE:   Sin acceso (cualquier acceso causa page fault)
 *   PROT_READ:   Lectura permitida
 *   PROT_WRITE:  Escritura permitida
 *   PROT_EXEC:   Ejecución permitida
 * 
 *   Se pueden combinar: PROT_READ | PROT_WRITE
 * 
 * Segmentation Fault (SIGSEGV):
 *   Cuando un proceso intenta acceder a memoria de forma no permitida:
 *     - Escribir en página de solo lectura
 *     - Leer/escribir en página sin acceso
 *     - Ejecutar código en página no ejecutable (NX bit)
 *     - Acceder a dirección no mapeada
 * 
 *   El kernel envía señal SIGSEGV → proceso termina (si no hay handler)
 * 
 * Aplicaciones de Seguridad:
 *   ✓ W^X (Write XOR Execute): páginas escribibles no son ejecutables
 *   ✓ DEP (Data Execution Prevention): previene ejecución en stack/heap
 *   ✓ Protección de código: segmento text es read-only
 *   ✓ Guard pages: detectar stack overflow
 * 
 * Experimento:
 *   - Mapearemos memoria con permiso SOLO de lectura
 *   - Intentaremos escribir en ella (comentado para evitar crash)
 *   - Demostraremos cómo cambiar permisos con mprotect()
 * ============================================================================
 */
void demo_4_page_protection() {
    printf("\n");
    printf("╔════════════════════════════════════════════════════════════════════╗\n");
    printf("║  DEMOSTRACIÓN 4: Protección de Páginas                            ║\n");
    printf("╚════════════════════════════════════════════════════════════════════╝\n");
    printf("\nConcepto: Control de permisos de acceso a páginas de memoria\n\n");
    
    printf("📋 Paso 1: Mapear memoria con permiso de SOLO LECTURA\n");
    printf("────────────────────────────────────────────────────────────────────\n");
    
    /*
     * Mapear una página con permiso PROT_READ (solo lectura)
     * No incluimos PROT_WRITE, por lo que escribir causará SIGSEGV
     */
    void* read_only_mem = mmap(
        NULL, 
        PAGE_SIZE,
        PROT_READ,                     // ⚠️ SOLO LECTURA
        MAP_PRIVATE | MAP_ANONYMOUS,
        -1, 
        0
    );
    
    if (read_only_mem == MAP_FAILED) {
        perror("❌ mmap (solo lectura) falló");
        return;
    }
    
    print_address_info("🔒 Memoria de solo lectura", read_only_mem);
    printf("🔒 Permisos: PROT_READ (lectura permitida, escritura PROHIBIDA)\n");
    
    /*
     * ⚠️ ADVERTENCIA: La siguiente línea causaría un Segmentation Fault
     * 
     * Si descomentamos esta línea, el programa intentará escribir en
     * memoria de solo lectura, lo que resulta en:
     * 
     *   1. CPU detecta violación de permisos
     *   2. MMU genera page fault
     *   3. Kernel envía señal SIGSEGV al proceso
     *   4. Proceso termina con "Segmentation fault (core dumped)"
     * 
     * Descomentar para ver el efecto (el programa terminará):
     */
    // strcpy((char*)read_only_mem, "¡Esto causará SIGSEGV!");
    
    printf("\n💡 Nota: Intentar escribir aquí causaría Segmentation Fault\n");
    printf("   (La línea está comentada para evitar que el programa termine)\n");
    
    /*
     * Demostrar mprotect(): cambiar permisos de páginas existentes
     */
    printf("\n📋 Paso 2: Cambiar permisos con mprotect()\n");
    printf("────────────────────────────────────────────────────────────────────\n");
    
    printf("🔓 Cambiando permisos a LECTURA + ESCRITURA...\n");
    
    if (mprotect(read_only_mem, PAGE_SIZE, PROT_READ | PROT_WRITE) == -1) {
        perror("mprotect falló");
        munmap(read_only_mem, PAGE_SIZE);
        return;
    }
    
    printf("✅ Permisos actualizados: PROT_READ | PROT_WRITE\n");
    
    // Ahora SÍ podemos escribir
    printf("\n📝 Escribiendo en la memoria (ahora es seguro)...\n");
    strcpy((char*)read_only_mem, "¡Ahora sí podemos escribir!");
    printf("📖 Contenido: \"%s\"\n", (char*)read_only_mem);
    
    printf("\n💡 Observaciones:\n");
    printf("   • Cada página tiene permisos independientes (R, W, X)\n");
    printf("   • Violaciones de permisos causan SIGSEGV (Segmentation Fault)\n");
    printf("   • mprotect() permite cambiar permisos dinámicamente\n");
    printf("   • Importante para seguridad: W^X (Write XOR Execute)\n");
    
    printf("\n🛡️  Aplicaciones de Seguridad:\n");
    printf("   • Código ejecutable: solo lectura + ejecución (no escritura)\n");
    printf("   • Stack/Heap: lectura + escritura (no ejecución) → DEP\n");
    printf("   • Guard pages: detectar desbordamientos de pila\n");
    printf("   • ASLR + NX bit: dificultar exploits de memoria\n");
    
    // Limpiar
    munmap(read_only_mem, PAGE_SIZE);
}

/*
 * ============================================================================
 * FUNCIÓN PRINCIPAL
 * ============================================================================
 */
int main() {
    // Banner de bienvenida
    printf("\n");
    printf("╔════════════════════════════════════════════════════════════════════╗\n");
    printf("║                                                                    ║\n");
    printf("║        🧠 MEMORIA VIRTUAL EN LINUX - PROGRAMA EDUCATIVO 🧠        ║\n");
    printf("║                                                                    ║\n");
    printf("║  Explora los conceptos fundamentales de gestión de memoria        ║\n");
    printf("║  en sistemas operativos modernos                                  ║\n");
    printf("║                                                                    ║\n");
    printf("╚════════════════════════════════════════════════════════════════════╝\n");
    
    // Información del sistema
    printf("\n📊 Información del Sistema:\n");
    printf("────────────────────────────────────────────────────────────────────\n");
    printf("  Tamaño de página:  %d bytes (%d KB)\n", PAGE_SIZE, PAGE_SIZE / 1024);
    printf("  PID del proceso:   %d\n", getpid());
    print_address_info("  Dirección de main", (void*)main);
    
    /*
     * Ejecutar demostraciones en orden pedagógico:
     * 1. Conceptos básicos → 4. Conceptos avanzados
     */
    
    // Demo 1: Entender el layout de memoria (más básico)
    demo_1_memory_map();
    
    // Demo 2: Mapeo de memoria con mmap (intermedio)
    demo_2_mmap();
    
    // Demo 3: Separación de espacios (intermedio-avanzado)
    demo_3_address_space_separation();
    
    // Demo 4: Protección de páginas (avanzado)
    demo_4_page_protection();
    
    // Resumen final
    printf("\n");
    printf("╔════════════════════════════════════════════════════════════════════╗\n");
    printf("║  ✅ DEMOSTRACIÓN COMPLETADA                                       ║\n");
    printf("╚════════════════════════════════════════════════════════════════════╝\n");
    
    printf("\n📚 Conceptos Clave Aprendidos:\n");
    printf("────────────────────────────────────────────────────────────────────\n");
    printf("  1️⃣  Espacio de Direcciones Virtuales\n");
    printf("      → Cada proceso tiene segmentos: TEXT, DATA, HEAP, STACK\n");
    printf("\n");
    printf("  2️⃣  Mapeo de Memoria (mmap)\n");
    printf("      → Control fino sobre regiones de memoria virtual\n");
    printf("\n");
    printf("  3️⃣  Separación de Procesos\n");
    printf("      → Cada proceso está aislado (fork + Copy-on-Write)\n");
    printf("\n");
    printf("  4️⃣  Protección de Páginas\n");
    printf("      → Permisos R/W/X previenen accesos no autorizados\n");
    
    printf("\n💡 Puntos Importantes:\n");
    printf("────────────────────────────────────────────────────────────────────\n");
    printf("  ✓ Las direcciones mostradas son VIRTUALES (no físicas)\n");
    printf("  ✓ La MMU traduce direcciones virtuales → físicas\n");
    printf("  ✓ Cada proceso tiene su propio espacio de direcciones\n");
    printf("  ✓ La protección de memoria es fundamental para seguridad\n");
    printf("  ✓ El kernel gestiona todo esto de forma transparente\n");
    
    printf("\n🔍 Próximos Pasos:\n");
    printf("────────────────────────────────────────────────────────────────────\n");
    printf("  • Ejecuta: cat /proc/%d/maps (ver mapa de memoria)\n", getpid());
    printf("  • Experimenta: modifica el código y observa los cambios\n");
    printf("  • Investiga: page faults, swapping, huge pages\n");
    printf("  • Lee: README.md para más información y ejercicios\n");
    
    printf("\n");
    return 0;
}