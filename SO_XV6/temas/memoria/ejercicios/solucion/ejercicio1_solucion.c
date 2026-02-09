/*
 * ============================================================================
 * SOLUCIÓN EJERCICIO 1: Implementar un Allocator Simple
 * ============================================================================
 * 
 * Esta es una solución completa y comentada del ejercicio de allocator.
 * Implementa tres versiones con complejidad creciente:
 * 
 * VERSIÓN 1 (Básica): Usa metadata inline para guardar el tamaño
 * VERSIÓN 2 (Intermedia): Agrega validaciones y mejor manejo de errores
 * VERSIÓN 3 (Avanzada): Incluye estadísticas y debugging
 * 
 * Autor: Solución educativa para Sistemas Operativos
 * Compilación: gcc -Wall -Wextra -o ejercicio1_sol ejercicio1_solucion.c
 * Ejecución: ./ejercicio1_sol
 * Valgrind: valgrind --leak-check=full ./ejercicio1_sol
 * ============================================================================
 */

#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdint.h>
#include <errno.h>
#include <assert.h>

#define PAGE_SIZE 4096

/*
 * ============================================================================
 * CONFIGURACIÓN: Elige la versión a compilar
 * ============================================================================
 * 
 * Descomenta UNA de las siguientes líneas para elegir la versión:
 */

// #define VERSION_BASICA      // Versión simple con metadata inline
// #define VERSION_INTERMEDIA  // Versión con validaciones mejoradas
#define VERSION_AVANZADA    // Versión con estadísticas y debugging

/*
 * ============================================================================
 * ESTRUCTURAS Y TIPOS
 * ============================================================================
 */

/**
 * Estructura de metadata que se guarda al inicio de cada bloque
 * 
 * Layout en memoria:
 * 
 *   +------------------+------------------------+
 *   | block_metadata_t |   User Data Area       |
 *   +------------------+------------------------+
 *   ^                  ^
 *   |                  |
 *   real_ptr           ptr retornado al usuario
 * 
 * El usuario solo ve el área de datos, la metadata está oculta.
 */
typedef struct block_metadata {
    size_t total_size;      // Tamaño total del bloque (incluyendo metadata)
    uint32_t magic;         // Número mágico para validación (0xDEADBEEF)
    uint32_t padding;       // Padding para alineación a 8 bytes
} block_metadata_t;

// Número mágico para detectar corrupción de memoria
#define MAGIC_NUMBER 0xDEADBEEF

#ifdef VERSION_AVANZADA
/**
 * Estadísticas globales del allocator
 */
typedef struct allocator_stats {
    size_t total_allocated;     // Total de bytes asignados actualmente
    size_t total_freed;          // Total de bytes liberados
    size_t num_allocations;      // Número de asignaciones activas
    size_t num_frees;            // Número de liberaciones
    size_t peak_memory;          // Pico de memoria usada
    size_t wasted_bytes;         // Bytes desperdiciados por alineación
} allocator_stats_t;

static allocator_stats_t g_stats = {0};
#endif

/*
 * ============================================================================
 * FUNCIONES AUXILIARES
 * ============================================================================
 */

/**
 * Redondea un tamaño al múltiplo de PAGE_SIZE más cercano (hacia arriba)
 * 
 * Ejemplo:
 *   round_to_page_size(100)   -> 4096
 *   round_to_page_size(4096)  -> 4096
 *   round_to_page_size(5000)  -> 8192
 */
static inline size_t round_to_page_size(size_t size) {
    return ((size + PAGE_SIZE - 1) / PAGE_SIZE) * PAGE_SIZE;
}

/**
 * Verifica si un puntero tiene metadata válida
 */
static inline int is_valid_block(block_metadata_t* meta) {
    return meta != NULL && meta->magic == MAGIC_NUMBER;
}

#ifdef VERSION_AVANZADA
/**
 * Actualiza las estadísticas del allocator
 */
static void update_stats_alloc(size_t requested, size_t allocated) {
    g_stats.total_allocated += allocated;
    g_stats.num_allocations++;
    
    if (g_stats.total_allocated > g_stats.peak_memory) {
        g_stats.peak_memory = g_stats.total_allocated;
    }
    
    g_stats.wasted_bytes += (allocated - requested - sizeof(block_metadata_t));
}

static void update_stats_free(size_t size) {
    g_stats.total_freed += size;
    g_stats.num_frees++;
    g_stats.total_allocated -= size;
}
#endif

/*
 * ============================================================================
 * IMPLEMENTACIÓN: my_malloc()
 * ============================================================================
 */

/**
 * my_malloc - Asigna memoria usando mmap()
 * 
 * @size: Número de bytes solicitados por el usuario
 * 
 * Retorna: Puntero a la memoria asignada, o NULL si falla
 * 
 * Algoritmo:
 *   1. Validar entrada (size > 0)
 *   2. Calcular tamaño total (metadata + datos del usuario)
 *   3. Redondear al múltiplo de PAGE_SIZE
 *   4. Mapear memoria con mmap()
 *   5. Inicializar metadata
 *   6. Retornar puntero al área de datos (después de metadata)
 * 
 * Complejidad: O(1)
 * Llamadas al sistema: 1 (mmap)
 */
void* my_malloc(size_t size) {
    // Paso 1: Validar entrada
    if (size == 0) {
        #ifdef VERSION_AVANZADA
        fprintf(stderr, "[DEBUG] my_malloc(0) - retornando NULL\n");
        #endif
        return NULL;
    }
    
    // Paso 2: Calcular tamaño total (metadata + datos)
    size_t total_size = sizeof(block_metadata_t) + size;
    
    // Detectar overflow (importante para seguridad)
    if (total_size < size) {
        errno = ENOMEM;
        return NULL;
    }
    
    // Paso 3: Redondear al múltiplo de PAGE_SIZE
    size_t rounded_size = round_to_page_size(total_size);
    
    #ifdef VERSION_AVANZADA
    fprintf(stderr, "[DEBUG] my_malloc(%zu) - solicitando %zu bytes al kernel\n", 
            size, rounded_size);
    #endif
    
    // Paso 4: Mapear memoria con mmap()
    void* mem = mmap(
        NULL,                           // Dirección: kernel elige
        rounded_size,                   // Tamaño redondeado
        PROT_READ | PROT_WRITE,        // Permisos: lectura + escritura
        MAP_PRIVATE | MAP_ANONYMOUS,   // Privado + anónimo
        -1,                            // No file descriptor
        0                              // No offset
    );
    
    // Paso 5: Verificar error
    if (mem == MAP_FAILED) {
        perror("mmap failed in my_malloc");
        return NULL;
    }
    
    // Paso 6: Inicializar metadata
    block_metadata_t* meta = (block_metadata_t*)mem;
    meta->total_size = rounded_size;
    meta->magic = MAGIC_NUMBER;
    meta->padding = 0;
    
    #ifdef VERSION_AVANZADA
    update_stats_alloc(size, rounded_size);
    fprintf(stderr, "[DEBUG] my_malloc(%zu) = %p (total: %zu bytes)\n", 
            size, (char*)mem + sizeof(block_metadata_t), rounded_size);
    #endif
    
    // Paso 7: Retornar puntero al área de datos (después de metadata)
    return (char*)mem + sizeof(block_metadata_t);
}

/*
 * ============================================================================
 * IMPLEMENTACIÓN: my_free()
 * ============================================================================
 */

/**
 * my_free - Libera memoria asignada con my_malloc()
 * 
 * @ptr: Puntero retornado por my_malloc()
 * 
 * Algoritmo:
 *   1. Validar que ptr no sea NULL
 *   2. Retroceder al inicio del bloque (antes de metadata)
 *   3. Validar metadata (número mágico)
 *   4. Recuperar tamaño del bloque
 *   5. Liberar con munmap()
 * 
 * Complejidad: O(1)
 * Llamadas al sistema: 1 (munmap)
 */
void my_free(void* ptr) {
    // Paso 1: Validar entrada
    if (ptr == NULL) {
        #ifdef VERSION_AVANZADA
        fprintf(stderr, "[DEBUG] my_free(NULL) - ignorando\n");
        #endif
        return;
    }
    
    // Paso 2: Retroceder al inicio del bloque real
    void* real_ptr = (char*)ptr - sizeof(block_metadata_t);
    
    // Paso 3: Obtener metadata
    block_metadata_t* meta = (block_metadata_t*)real_ptr;
    
    // Paso 4: Validar metadata (detectar corrupción)
    #if defined(VERSION_INTERMEDIA) || defined(VERSION_AVANZADA)
    if (!is_valid_block(meta)) {
        fprintf(stderr, "ERROR: my_free() - bloque corrupto o inválido en %p\n", ptr);
        fprintf(stderr, "       Magic number: 0x%X (esperado: 0x%X)\n", 
                meta->magic, MAGIC_NUMBER);
        abort();  // Terminar programa (comportamiento de seguridad)
    }
    #endif
    
    // Paso 5: Recuperar tamaño
    size_t size = meta->total_size;
    
    #ifdef VERSION_AVANZADA
    fprintf(stderr, "[DEBUG] my_free(%p) - liberando %zu bytes\n", ptr, size);
    update_stats_free(size);
    #endif
    
    // Paso 6: Invalidar metadata (ayuda a detectar double-free)
    meta->magic = 0;
    
    // Paso 7: Liberar con munmap()
    if (munmap(real_ptr, size) == -1) {
        perror("munmap failed in my_free");
        // No podemos hacer mucho aquí, pero al menos reportamos el error
    }
}

/*
 * ============================================================================
 * IMPLEMENTACIÓN: my_calloc()
 * ============================================================================
 */

/**
 * my_calloc - Asigna memoria inicializada a cero
 * 
 * @nmemb: Número de elementos
 * @size: Tamaño de cada elemento
 * 
 * Retorna: Puntero a la memoria asignada e inicializada, o NULL si falla
 * 
 * Nota: mmap() ya retorna memoria en cero, pero hacemos memset() por
 *       compatibilidad con la semántica de calloc() estándar.
 */
void* my_calloc(size_t nmemb, size_t size) {
    // Paso 1: Validar entrada
    if (nmemb == 0 || size == 0) {
        return NULL;
    }
    
    // Paso 2: Detectar overflow en la multiplicación
    // Si nmemb * size desborda, retornamos NULL
    if (size > SIZE_MAX / nmemb) {
        errno = ENOMEM;
        #ifdef VERSION_AVANZADA
        fprintf(stderr, "[DEBUG] my_calloc(%zu, %zu) - overflow detectado\n", 
                nmemb, size);
        #endif
        return NULL;
    }
    
    // Paso 3: Calcular tamaño total
    size_t total_size = nmemb * size;
    
    // Paso 4: Asignar con my_malloc()
    void* ptr = my_malloc(total_size);
    if (ptr == NULL) {
        return NULL;
    }
    
    // Paso 5: Inicializar a cero
    // Nota: mmap() ya retorna memoria en cero, pero lo hacemos explícitamente
    memset(ptr, 0, total_size);
    
    #ifdef VERSION_AVANZADA
    fprintf(stderr, "[DEBUG] my_calloc(%zu, %zu) = %p\n", nmemb, size, ptr);
    #endif
    
    return ptr;
}

/*
 * ============================================================================
 * BONUS: my_realloc()
 * ============================================================================
 */

#if defined(VERSION_INTERMEDIA) || defined(VERSION_AVANZADA)
/**
 * my_realloc - Redimensiona un bloque de memoria
 * 
 * @ptr: Puntero al bloque actual (puede ser NULL)
 * @new_size: Nuevo tamaño deseado
 * 
 * Retorna: Puntero al bloque redimensionado, o NULL si falla
 * 
 * Comportamiento:
 *   - Si ptr == NULL, equivalente a my_malloc(new_size)
 *   - Si new_size == 0, equivalente a my_free(ptr)
 *   - Si new_size <= tamaño actual, retorna el mismo puntero
 *   - Si new_size > tamaño actual, asigna nuevo bloque y copia datos
 */
void* my_realloc(void* ptr, size_t new_size) {
    // Caso 1: ptr == NULL -> comportamiento de malloc
    if (ptr == NULL) {
        return my_malloc(new_size);
    }
    
    // Caso 2: new_size == 0 -> comportamiento de free
    if (new_size == 0) {
        my_free(ptr);
        return NULL;
    }
    
    // Obtener tamaño actual
    void* real_ptr = (char*)ptr - sizeof(block_metadata_t);
    block_metadata_t* meta = (block_metadata_t*)real_ptr;
    
    if (!is_valid_block(meta)) {
        fprintf(stderr, "ERROR: my_realloc() - bloque inválido\n");
        return NULL;
    }
    
    size_t current_size = meta->total_size - sizeof(block_metadata_t);
    
    // Caso 3: new_size <= tamaño actual -> retornar mismo puntero
    if (new_size <= current_size) {
        #ifdef VERSION_AVANZADA
        fprintf(stderr, "[DEBUG] my_realloc(%p, %zu) - reutilizando bloque\n", 
                ptr, new_size);
        #endif
        return ptr;
    }
    
    // Caso 4: new_size > tamaño actual -> asignar nuevo bloque
    void* new_ptr = my_malloc(new_size);
    if (new_ptr == NULL) {
        return NULL;  // Fallo, pero ptr original sigue válido
    }
    
    // Copiar datos del bloque antiguo al nuevo
    memcpy(new_ptr, ptr, current_size);
    
    // Liberar bloque antiguo
    my_free(ptr);
    
    #ifdef VERSION_AVANZADA
    fprintf(stderr, "[DEBUG] my_realloc(%p, %zu) = %p (copiado)\n", 
            ptr, new_size, new_ptr);
    #endif
    
    return new_ptr;
}
#endif

/*
 * ============================================================================
 * FUNCIONES DE ESTADÍSTICAS Y DEBUGGING
 * ============================================================================
 */

#ifdef VERSION_AVANZADA
/**
 * Imprime estadísticas del allocator
 */
void print_allocator_stats() {
    printf("\n╔════════════════════════════════════════════════════════════════╗\n");
    printf("║  ESTADÍSTICAS DEL ALLOCATOR                                    ║\n");
    printf("╚════════════════════════════════════════════════════════════════╝\n\n");
    
    printf("  Total asignado actualmente:  %zu bytes\n", g_stats.total_allocated);
    printf("  Total liberado:              %zu bytes\n", g_stats.total_freed);
    printf("  Número de asignaciones:      %zu\n", g_stats.num_allocations);
    printf("  Número de liberaciones:      %zu\n", g_stats.num_frees);
    printf("  Pico de memoria:             %zu bytes (%.2f KB)\n", 
           g_stats.peak_memory, g_stats.peak_memory / 1024.0);
    printf("  Bytes desperdiciados:        %zu bytes (fragmentación interna)\n", 
           g_stats.wasted_bytes);
    
    // Detectar memory leaks
    if (g_stats.num_allocations != g_stats.num_frees) {
        printf("\n  ⚠️  ADVERTENCIA: Posible memory leak detectado!\n");
        printf("      Asignaciones: %zu, Liberaciones: %zu\n", 
               g_stats.num_allocations, g_stats.num_frees);
    } else {
        printf("\n  ✅ Sin memory leaks detectados\n");
    }
}
#endif

/*
 * ============================================================================
 * PROGRAMA DE PRUEBA
 * ============================================================================
 */

void test_basic_allocation() {
    printf("\n=== Test 1: Asignación Básica ===\n");
    
    char* ptr1 = (char*)my_malloc(100);
    if (ptr1 == NULL) {
        printf("❌ my_malloc(100) falló\n");
        return;
    }
    printf("✅ my_malloc(100) = %p\n", (void*)ptr1);
    
    strcpy(ptr1, "Hola desde mi allocator!");
    printf("✅ Datos escritos: %s\n", ptr1);
    
    my_free(ptr1);
    printf("✅ my_free() completado\n");
}

void test_multiple_allocations() {
    printf("\n=== Test 2: Múltiples Asignaciones ===\n");
    
    void* ptrs[5];
    size_t sizes[] = {16, 256, 1024, 4096, 8192};
    
    for (int i = 0; i < 5; i++) {
        ptrs[i] = my_malloc(sizes[i]);
        if (ptrs[i] == NULL) {
            printf("❌ my_malloc(%zu) falló\n", sizes[i]);
            return;
        }
        printf("✅ Asignado %zu bytes en %p\n", sizes[i], ptrs[i]);
    }
    
    for (int i = 0; i < 5; i++) {
        my_free(ptrs[i]);
        printf("✅ Liberado bloque %d\n", i);
    }
}

void test_calloc() {
    printf("\n=== Test 3: calloc ===\n");
    
    int* arr = (int*)my_calloc(10, sizeof(int));
    if (arr == NULL) {
        printf("❌ my_calloc(10, sizeof(int)) falló\n");
        return;
    }
    
    int all_zero = 1;
    for (int i = 0; i < 10; i++) {
        if (arr[i] != 0) {
            all_zero = 0;
            break;
        }
    }
    
    if (all_zero) {
        printf("✅ Array inicializado a cero correctamente\n");
    } else {
        printf("❌ Array NO está en cero\n");
    }
    
    for (int i = 0; i < 10; i++) {
        arr[i] = i * 10;
    }
    printf("✅ Array modificado: [");
    for (int i = 0; i < 10; i++) {
        printf("%d%s", arr[i], i < 9 ? ", " : "");
    }
    printf("]\n");
    
    my_free(arr);
    printf("✅ my_free() completado\n");
}

void test_edge_cases() {
    printf("\n=== Test 4: Casos Límite ===\n");
    
    void* ptr = my_malloc(0);
    if (ptr == NULL) {
        printf("✅ my_malloc(0) retorna NULL correctamente\n");
    } else {
        printf("⚠️  my_malloc(0) retornó %p\n", ptr);
        my_free(ptr);
    }
    
    my_free(NULL);
    printf("✅ my_free(NULL) no causa crash\n");
    
    size_t huge_size = 1024UL * 1024 * 1024 * 10;
    ptr = my_malloc(huge_size);
    if (ptr == NULL) {
        printf("✅ my_malloc(10GB) falló apropiadamente\n");
    } else {
        printf("⚠️  my_malloc(10GB) tuvo éxito\n");
        my_free(ptr);
    }
}

#if defined(VERSION_INTERMEDIA) || defined(VERSION_AVANZADA)
void test_realloc() {
    printf("\n=== Test 5: realloc ===\n");
    
    char* str = (char*)my_malloc(10);
    strcpy(str, "Hola");
    printf("✅ String inicial: \"%s\" (10 bytes)\n", str);
    
    str = (char*)my_realloc(str, 50);
    strcat(str, " Mundo desde realloc!");
    printf("✅ String expandido: \"%s\" (50 bytes)\n", str);
    
    my_free(str);
    printf("✅ my_free() completado\n");
}
#endif

int main() {
    printf("╔════════════════════════════════════════════════════════════════╗\n");
    printf("║  SOLUCIÓN EJERCICIO 1: Allocator Simple                       ║\n");
    
    #ifdef VERSION_BASICA
    printf("║  Versión: BÁSICA                                               ║\n");
    #elif defined(VERSION_INTERMEDIA)
    printf("║  Versión: INTERMEDIA (con validaciones)                       ║\n");
    #elif defined(VERSION_AVANZADA)
    printf("║  Versión: AVANZADA (con estadísticas)                         ║\n");
    #endif
    
    printf("╚════════════════════════════════════════════════════════════════╝\n");
    
    test_basic_allocation();
    test_multiple_allocations();
    test_calloc();
    test_edge_cases();
    
    #if defined(VERSION_INTERMEDIA) || defined(VERSION_AVANZADA)
    test_realloc();
    #endif
    
    #ifdef VERSION_AVANZADA
    print_allocator_stats();
    #endif
    
    printf("\n╔════════════════════════════════════════════════════════════════╗\n");
    printf("║  ✅ Tests Completados                                         ║\n");
    printf("╚════════════════════════════════════════════════════════════════╝\n");
    
    return 0;
}
