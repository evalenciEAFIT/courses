/*
 * ============================================================================
 * SOLUCIÓN EJERCICIO 4: Profiling de Page Faults
 * ============================================================================
 * 
 * Objetivo:
 *   Medir y analizar page faults en diferentes escenarios de acceso a memoria.
 *   Entender el impacto del patrón de acceso en el rendimiento.
 * 
 * Conceptos demostrados:
 *   - Page faults (minor y major)
 *   - Acceso secuencial vs aleatorio
 *   - Locality of reference
 *   - Working set y thrashing
 *   - Medición con /proc/self/stat
 * 
 * Compilación:
 *   gcc -Wall -Wextra -o ejercicio4_sol ejercicio4_page_faults_solucion.c -lm
 * 
 * Ejecución:
 *   ./ejercicio4_sol
 * ============================================================================
 */

#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include <math.h>

#define PAGE_SIZE 4096
#define MB (1024 * 1024)

/*
 * ============================================================================
 * ESTRUCTURAS Y TIPOS
 * ============================================================================
 */

typedef struct page_fault_stats {
    long minor_faults;      // Page faults que no requieren I/O
    long major_faults;      // Page faults que requieren I/O (swap)
    long total_faults;
} page_fault_stats_t;

typedef struct performance_metrics {
    double time_ms;
    long page_faults;
    double faults_per_second;
} performance_metrics_t;

/*
 * ============================================================================
 * FUNCIONES AUXILIARES
 * ============================================================================
 */

/**
 * Lee estadísticas de page faults desde /proc/self/stat
 */
void get_page_fault_stats(page_fault_stats_t* stats) {
    FILE* f = fopen("/proc/self/stat", "r");
    if (!f) {
        perror("fopen /proc/self/stat");
        memset(stats, 0, sizeof(page_fault_stats_t));
        return;
    }
    
    // Formato de /proc/self/stat (ver man proc)
    // Campo 10: minor faults, Campo 12: major faults
    long dummy;
    char comm[256];
    char state;
    
    fscanf(f, "%ld %s %c %ld %ld %ld %ld %ld %lu %lu %lu %lu %lu",
           &dummy, comm, &state, &dummy, &dummy, &dummy, &dummy, &dummy,
           (unsigned long*)&dummy, (unsigned long*)&stats->minor_faults,
           (unsigned long*)&dummy, (unsigned long*)&stats->major_faults,
           (unsigned long*)&dummy);
    
    stats->total_faults = stats->minor_faults + stats->major_faults;
    
    fclose(f);
}

/**
 * Alternativa usando getrusage()
 */
void get_page_fault_stats_rusage(page_fault_stats_t* stats) {
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    
    stats->minor_faults = usage.ru_minflt;
    stats->major_faults = usage.ru_majflt;
    stats->total_faults = stats->minor_faults + stats->major_faults;
}

/**
 * Mide tiempo en milisegundos
 */
double get_time_ms() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec * 1000.0 + ts.tv_nsec / 1000000.0;
}

/**
 * Imprime estadísticas de page faults
 */
void print_stats(const char* label, page_fault_stats_t* before, 
                 page_fault_stats_t* after, double time_ms) {
    long minor_delta = after->minor_faults - before->minor_faults;
    long major_delta = after->major_faults - before->major_faults;
    long total_delta = minor_delta + major_delta;
    
    printf("  %s:\n", label);
    printf("    Minor faults: %ld\n", minor_delta);
    printf("    Major faults: %ld\n", major_delta);
    printf("    Total faults: %ld\n", total_delta);
    printf("    Tiempo: %.2f ms\n", time_ms);
    if (time_ms > 0) {
        printf("    Faults/segundo: %.0f\n", (total_delta / time_ms) * 1000);
    }
    printf("\n");
}

/*
 * ============================================================================
 * DEMOSTRACIÓN 1: Acceso Secuencial vs Aleatorio
 * ============================================================================
 */

void demo1_sequential_vs_random() {
    printf("\n╔════════════════════════════════════════════════════════════════╗\n");
    printf("║  Demo 1: Acceso Secuencial vs Aleatorio                       ║\n");
    printf("╚════════════════════════════════════════════════════════════════╝\n\n");
    
    const size_t size = 100 * MB;  // 100 MB
    const size_t num_pages = size / PAGE_SIZE;
    
    printf("Configuración:\n");
    printf("  - Tamaño de memoria: %zu MB\n", size / MB);
    printf("  - Número de páginas: %zu\n", num_pages);
    printf("  - Tamaño de página: %d bytes\n\n", PAGE_SIZE);
    
    // Mapear memoria
    char* mem = (char*)mmap(NULL, size, PROT_READ | PROT_WRITE,
                            MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (mem == MAP_FAILED) {
        perror("mmap");
        return;
    }
    
    page_fault_stats_t before, after;
    double start, time_seq, time_rand;
    
    // ═══════════════════════════════════════════════════════════════
    // Acceso SECUENCIAL
    // ═══════════════════════════════════════════════════════════════
    
    printf("Test 1: Acceso SECUENCIAL\n");
    printf("─────────────────────────────────────────────────────────────\n");
    
    get_page_fault_stats_rusage(&before);
    start = get_time_ms();
    
    // Tocar cada página secuencialmente
    for (size_t i = 0; i < num_pages; i++) {
        mem[i * PAGE_SIZE] = 1;  // Escribir en cada página
    }
    
    time_seq = get_time_ms() - start;
    get_page_fault_stats_rusage(&after);
    
    print_stats("Acceso secuencial", &before, &after, time_seq);
    
    // Liberar y volver a mapear para el siguiente test
    munmap(mem, size);
    mem = (char*)mmap(NULL, size, PROT_READ | PROT_WRITE,
                      MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    
    // ═══════════════════════════════════════════════════════════════
    // Acceso ALEATORIO
    // ═══════════════════════════════════════════════════════════════
    
    printf("Test 2: Acceso ALEATORIO\n");
    printf("─────────────────────────────────────────────────────────────\n");
    
    // Generar índices aleatorios
    size_t* random_indices = (size_t*)malloc(num_pages * sizeof(size_t));
    for (size_t i = 0; i < num_pages; i++) {
        random_indices[i] = i;
    }
    
    // Shuffle (Fisher-Yates)
    srand(time(NULL));
    for (size_t i = num_pages - 1; i > 0; i--) {
        size_t j = rand() % (i + 1);
        size_t temp = random_indices[i];
        random_indices[i] = random_indices[j];
        random_indices[j] = temp;
    }
    
    get_page_fault_stats_rusage(&before);
    start = get_time_ms();
    
    // Tocar páginas en orden aleatorio
    for (size_t i = 0; i < num_pages; i++) {
        mem[random_indices[i] * PAGE_SIZE] = 1;
    }
    
    time_rand = get_time_ms() - start;
    get_page_fault_stats_rusage(&after);
    
    print_stats("Acceso aleatorio", &before, &after, time_rand);
    
    // ═══════════════════════════════════════════════════════════════
    // Comparación
    // ═══════════════════════════════════════════════════════════════
    
    printf("Comparación:\n");
    printf("─────────────────────────────────────────────────────────────\n");
    printf("  Secuencial: %.2f ms\n", time_seq);
    printf("  Aleatorio:  %.2f ms\n", time_rand);
    printf("  Diferencia: %.2fx más lento\n\n", time_rand / time_seq);
    
    printf("💡 Explicación:\n");
    printf("   El acceso aleatorio es más lento debido a:\n");
    printf("   • Peor locality of reference\n");
    printf("   • Cache misses más frecuentes\n");
    printf("   • TLB (Translation Lookaside Buffer) misses\n");
    printf("   • Prefetching del hardware menos efectivo\n");
    
    free(random_indices);
    munmap(mem, size);
    
    printf("\n✅ Demo 1 completada\n");
}

/*
 * ============================================================================
 * DEMOSTRACIÓN 2: Impacto del Tamaño de Stride
 * ============================================================================
 */

void demo2_stride_impact() {
    printf("\n╔════════════════════════════════════════════════════════════════╗\n");
    printf("║  Demo 2: Impacto del Tamaño de Stride                         ║\n");
    printf("╚════════════════════════════════════════════════════════════════╝\n\n");
    
    const size_t size = 50 * MB;
    
    char* mem = (char*)mmap(NULL, size, PROT_READ | PROT_WRITE,
                            MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (mem == MAP_FAILED) {
        perror("mmap");
        return;
    }
    
    printf("Probando diferentes tamaños de stride:\n\n");
    
    size_t strides[] = {64, 256, 1024, PAGE_SIZE, PAGE_SIZE * 4};
    const char* stride_names[] = {"64B (cache line)", "256B", "1KB", 
                                   "4KB (página)", "16KB (4 páginas)"};
    
    for (int s = 0; s < 5; s++) {
        size_t stride = strides[s];
        
        // Liberar y remapear para cada test
        munmap(mem, size);
        mem = (char*)mmap(NULL, size, PROT_READ | PROT_WRITE,
                          MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        
        page_fault_stats_t before, after;
        get_page_fault_stats_rusage(&before);
        double start = get_time_ms();
        
        // Acceder con el stride especificado
        for (size_t i = 0; i < size; i += stride) {
            mem[i] = 1;
        }
        
        double time = get_time_ms() - start;
        get_page_fault_stats_rusage(&after);
        
        long faults = (after.minor_faults - before.minor_faults) +
                      (after.major_faults - before.major_faults);
        
        printf("  Stride %s:\n", stride_names[s]);
        printf("    Tiempo: %.2f ms\n", time);
        printf("    Page faults: %ld\n", faults);
        printf("    Accesos: %zu\n\n", size / stride);
    }
    
    printf("💡 Observación:\n");
    printf("   • Stride pequeño: Más accesos, pero mejor locality\n");
    printf("   • Stride = PAGE_SIZE: Óptimo para minimizar faults\n");
    printf("   • Stride grande: Menos accesos, pero más faults por acceso\n");
    
    munmap(mem, size);
    printf("\n✅ Demo 2 completada\n");
}

/*
 * ============================================================================
 * DEMOSTRACIÓN 3: Working Set y Thrashing
 * ============================================================================
 */

void demo3_working_set() {
    printf("\n╔════════════════════════════════════════════════════════════════╗\n");
    printf("║  Demo 3: Working Set y Thrashing                              ║\n");
    printf("╚════════════════════════════════════════════════════════════════╝\n\n");
    
    printf("Simulando diferentes tamaños de working set:\n\n");
    
    size_t sizes[] = {1 * MB, 10 * MB, 50 * MB, 100 * MB};
    const char* size_names[] = {"1 MB", "10 MB", "50 MB", "100 MB"};
    
    for (int s = 0; s < 4; s++) {
        size_t size = sizes[s];
        
        char* mem = (char*)mmap(NULL, size, PROT_READ | PROT_WRITE,
                                MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        if (mem == MAP_FAILED) {
            perror("mmap");
            continue;
        }
        
        page_fault_stats_t before, after;
        get_page_fault_stats_rusage(&before);
        double start = get_time_ms();
        
        // Acceder repetidamente al working set
        const int iterations = 10;
        for (int iter = 0; iter < iterations; iter++) {
            for (size_t i = 0; i < size; i += PAGE_SIZE) {
                mem[i] = (char)iter;
            }
        }
        
        double time = get_time_ms() - start;
        get_page_fault_stats_rusage(&after);
        
        long faults = (after.minor_faults - before.minor_faults) +
                      (after.major_faults - before.major_faults);
        long expected_faults = size / PAGE_SIZE;  // Primera pasada
        
        printf("  Working set %s:\n", size_names[s]);
        printf("    Tiempo total: %.2f ms\n", time);
        printf("    Page faults: %ld\n", faults);
        printf("    Faults esperados (1ª pasada): %ld\n", expected_faults);
        printf("    Faults por iteración: %.1f\n", (double)faults / iterations);
        
        if (faults > expected_faults * 1.5) {
            printf("    ⚠️  Posible thrashing detectado!\n");
        } else {
            printf("    ✅ Working set cabe en memoria\n");
        }
        printf("\n");
        
        munmap(mem, size);
    }
    
    printf("💡 Concepto de Working Set:\n");
    printf("   • Working set = conjunto de páginas activamente usadas\n");
    printf("   • Si working set > RAM disponible → thrashing\n");
    printf("   • Thrashing = page faults excesivos, rendimiento colapsa\n");
    
    printf("\n✅ Demo 3 completada\n");
}

/*
 * ============================================================================
 * DEMOSTRACIÓN 4: Copy-on-Write y Page Faults
 * ============================================================================
 */

void demo4_copy_on_write() {
    printf("\n╔════════════════════════════════════════════════════════════════╗\n");
    printf("║  Demo 4: Copy-on-Write y Page Faults                          ║\n");
    printf("╚════════════════════════════════════════════════════════════════╝\n\n");
    
    const size_t size = 10 * MB;
    
    // Mapear y llenar memoria
    char* mem = (char*)mmap(NULL, size, PROT_READ | PROT_WRITE,
                            MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (mem == MAP_FAILED) {
        perror("mmap");
        return;
    }
    
    printf("Llenando %zu MB de memoria...\n", size / MB);
    for (size_t i = 0; i < size; i += PAGE_SIZE) {
        mem[i] = 1;
    }
    
    page_fault_stats_t before, after;
    
    printf("\nCreando proceso hijo con fork()...\n");
    
    get_page_fault_stats_rusage(&before);
    
    pid_t pid = fork();
    
    if (pid == 0) {
        // PROCESO HIJO
        printf("[HIJO] Proceso iniciado\n");
        
        page_fault_stats_t child_before, child_after;
        get_page_fault_stats_rusage(&child_before);
        
        // Leer memoria (no causa COW)
        printf("[HIJO] Leyendo memoria (sin modificar)...\n");
        volatile char dummy = 0;
        for (size_t i = 0; i < size; i += PAGE_SIZE) {
            dummy += mem[i];
        }
        
        get_page_fault_stats_rusage(&child_after);
        long read_faults = (child_after.minor_faults - child_before.minor_faults);
        printf("[HIJO] Page faults al leer: %ld\n", read_faults);
        
        // Escribir memoria (causa COW)
        printf("[HIJO] Escribiendo memoria (activa COW)...\n");
        get_page_fault_stats_rusage(&child_before);
        
        for (size_t i = 0; i < size; i += PAGE_SIZE) {
            mem[i] = 2;  // Modificar → COW
        }
        
        get_page_fault_stats_rusage(&child_after);
        long write_faults = (child_after.minor_faults - child_before.minor_faults);
        printf("[HIJO] Page faults al escribir: %ld\n", write_faults);
        printf("[HIJO] Páginas copiadas por COW: ~%ld\n", write_faults);
        
        exit(0);
    } else {
        // PROCESO PADRE
        wait(NULL);
        
        get_page_fault_stats_rusage(&after);
        
        printf("\n[PADRE] Hijo terminado\n");
        printf("[PADRE] Page faults del padre durante fork: %ld\n",
               (after.minor_faults - before.minor_faults));
    }
    
    printf("\n💡 Copy-on-Write (COW):\n");
    printf("   • fork() no copia memoria inmediatamente\n");
    printf("   • Padre e hijo comparten páginas (read-only)\n");
    printf("   • Escritura causa page fault → copia la página\n");
    printf("   • Ahorra memoria y tiempo en fork()\n");
    
    munmap(mem, size);
    printf("\n✅ Demo 4 completada\n");
}

/*
 * ============================================================================
 * MAIN
 * ============================================================================
 */

int main() {
    printf("╔════════════════════════════════════════════════════════════════╗\n");
    printf("║  SOLUCIÓN EJERCICIO 4: Profiling de Page Faults               ║\n");
    printf("╚════════════════════════════════════════════════════════════════╝\n");
    
    printf("\n📚 Este programa analiza page faults en 4 escenarios:\n");
    printf("   1. Acceso secuencial vs aleatorio\n");
    printf("   2. Impacto del tamaño de stride\n");
    printf("   3. Working set y thrashing\n");
    printf("   4. Copy-on-Write y fork()\n");
    
    printf("\n⚙️  Configuración del sistema:\n");
    printf("   Tamaño de página: %d bytes\n", PAGE_SIZE);
    
    demo1_sequential_vs_random();
    demo2_stride_impact();
    demo3_working_set();
    demo4_copy_on_write();
    
    printf("\n╔════════════════════════════════════════════════════════════════╗\n");
    printf("║  ✅ Todas las demostraciones completadas                      ║\n");
    printf("╚════════════════════════════════════════════════════════════════╝\n");
    
    printf("\n💡 Conclusiones clave:\n");
    printf("   • Locality of reference es crucial para rendimiento\n");
    printf("   • Acceso secuencial >> acceso aleatorio\n");
    printf("   • Working set debe caber en RAM para evitar thrashing\n");
    printf("   • COW optimiza fork() significativamente\n");
    printf("   • Page faults son costosos, minimízalos\n");
    
    return 0;
}
