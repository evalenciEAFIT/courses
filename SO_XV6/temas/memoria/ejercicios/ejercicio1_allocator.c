/*
 * ============================================================================
 * EJERCICIO 1: Implementar un Allocator Simple
 * ============================================================================
 * 
 * Objetivo:
 *   Crear tu propio sistema de asignación de memoria dinámica usando mmap()
 *   en lugar de malloc(). Esto te ayudará a entender cómo funcionan los
 *   allocators por dentro.
 * 
 * Conceptos que practicarás:
 *   - Uso de mmap() y munmap()
 *   - Gestión de bloques de memoria
 *   - Alineación de memoria
 *   - Fragmentación
 * 
 * Tareas:
 *   1. Implementar my_malloc(size_t size)
 *      - Debe mapear memoria usando mmap()
 *      - Redondear el tamaño a múltiplos de PAGE_SIZE
 *      - Retornar puntero a la memoria asignada
 * 
 *   2. Implementar my_free(void* ptr)
 *      - Debe liberar memoria usando munmap()
 *      - Manejar casos de error (ptr NULL, etc.)
 * 
 *   3. Implementar my_calloc(size_t nmemb, size_t size)
 *      - Similar a malloc pero inicializa a cero
 *      - Detectar overflow en nmemb * size
 * 
 *   4. Crear un programa de prueba que:
 *      - Asigne varios bloques de diferentes tamaños
 *      - Libere algunos bloques
 *      - Verifique que no hay memory leaks
 * 
 * Bonus (Desafíos Adicionales):
 *   - Implementar my_realloc(void* ptr, size_t new_size)
 *   - Llevar registro de bloques asignados (metadata)
 *   - Implementar un free list para reutilizar memoria
 *   - Agregar estadísticas (bytes asignados, fragmentación, etc.)
 * 
 * Compilación:
 *   gcc -Wall -Wextra -o ejercicio1 ejercicio1_allocator.c
 * 
 * Ejecución:
 *   ./ejercicio1
 *   valgrind --leak-check=full ./ejercicio1
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

#define PAGE_SIZE 4096

/*
 * ============================================================================
 * PARTE 1: Implementa estas funciones
 * ============================================================================
 */

/**
 * my_malloc - Asigna memoria usando mmap()
 * 
 * @size: Número de bytes a asignar
 * 
 * Retorna: Puntero a la memoria asignada, o NULL si falla
 * 
 * Pistas:
 *   - Redondea size al múltiplo de PAGE_SIZE más cercano
 *   - Usa mmap() con MAP_PRIVATE | MAP_ANONYMOUS
 *   - Verifica que mmap() no retorne MAP_FAILED
 *   - Considera guardar metadata (tamaño del bloque) para my_free()
 */
void* my_malloc(size_t size) {
    // TODO: Implementa aquí
    
    // Paso 1: Validar entrada
    if (size == 0) {
        return NULL;
    }
    
    // Paso 2: Redondear al múltiplo de PAGE_SIZE
    // Pista: size_rounded = ((size + PAGE_SIZE - 1) / PAGE_SIZE) * PAGE_SIZE
    
    // Paso 3: Mapear memoria con mmap()
    // Pista: mmap(NULL, size_rounded, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0)
    
    // Paso 4: Verificar error
    
    // Paso 5: Retornar puntero
    
    return NULL;  // Reemplaza esto con tu implementación
}

/**
 * my_free - Libera memoria asignada con my_malloc()
 * 
 * @ptr: Puntero a la memoria a liberar
 * 
 * Pistas:
 *   - Verifica que ptr no sea NULL
 *   - Necesitas saber el tamaño del bloque (¿cómo lo guardaste?)
 *   - Usa munmap() para liberar
 */
void my_free(void* ptr) {
    // TODO: Implementa aquí
    
    // Paso 1: Validar entrada
    if (ptr == NULL) {
        return;
    }
    
    // Paso 2: Recuperar tamaño del bloque
    // (Necesitas haber guardado esto en my_malloc)
    
    // Paso 3: Liberar con munmap()
    // Pista: munmap(ptr, size)
}

/**
 * my_calloc - Asigna memoria inicializada a cero
 * 
 * @nmemb: Número de elementos
 * @size: Tamaño de cada elemento
 * 
 * Retorna: Puntero a la memoria asignada y inicializada, o NULL si falla
 * 
 * Pistas:
 *   - Detecta overflow: si nmemb * size desborda, retorna NULL
 *   - Usa my_malloc() para asignar
 *   - Inicializa a cero con memset()
 *   - Nota: mmap() ya retorna memoria en cero, pero es buena práctica
 */
void* my_calloc(size_t nmemb, size_t size) {
    // TODO: Implementa aquí
    
    // Paso 1: Detectar overflow
    // Pista: if (nmemb != 0 && size > SIZE_MAX / nmemb) return NULL;
    
    // Paso 2: Calcular tamaño total
    
    // Paso 3: Asignar con my_malloc()
    
    // Paso 4: Inicializar a cero (opcional, mmap ya lo hace)
    
    return NULL;  // Reemplaza esto con tu implementación
}

/*
 * ============================================================================
 * PARTE 2: Programa de Prueba
 * ============================================================================
 */

void test_basic_allocation() {
    printf("\n=== Test 1: Asignación Básica ===\n");
    
    // Asignar 100 bytes
    char* ptr1 = (char*)my_malloc(100);
    if (ptr1 == NULL) {
        printf("❌ my_malloc(100) falló\n");
        return;
    }
    printf("✅ my_malloc(100) = %p\n", (void*)ptr1);
    
    // Escribir datos
    strcpy(ptr1, "Hola desde mi allocator!");
    printf("✅ Datos escritos: %s\n", ptr1);
    
    // Liberar
    my_free(ptr1);
    printf("✅ my_free() completado\n");
}

void test_multiple_allocations() {
    printf("\n=== Test 2: Múltiples Asignaciones ===\n");
    
    void* ptrs[5];
    size_t sizes[] = {16, 256, 1024, 4096, 8192};
    
    // Asignar varios bloques
    for (int i = 0; i < 5; i++) {
        ptrs[i] = my_malloc(sizes[i]);
        if (ptrs[i] == NULL) {
            printf("❌ my_malloc(%zu) falló\n", sizes[i]);
            return;
        }
        printf("✅ Asignado %zu bytes en %p\n", sizes[i], ptrs[i]);
    }
    
    // Liberar todos
    for (int i = 0; i < 5; i++) {
        my_free(ptrs[i]);
        printf("✅ Liberado bloque %d\n", i);
    }
}

void test_calloc() {
    printf("\n=== Test 3: calloc ===\n");
    
    // Asignar array de 10 enteros
    int* arr = (int*)my_calloc(10, sizeof(int));
    if (arr == NULL) {
        printf("❌ my_calloc(10, sizeof(int)) falló\n");
        return;
    }
    
    // Verificar que está inicializado a cero
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
    
    // Usar el array
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
    
    // Asignar 0 bytes
    void* ptr = my_malloc(0);
    if (ptr == NULL) {
        printf("✅ my_malloc(0) retorna NULL correctamente\n");
    } else {
        printf("⚠️  my_malloc(0) retornó %p (comportamiento definido por implementación)\n", ptr);
        my_free(ptr);
    }
    
    // Liberar NULL
    my_free(NULL);
    printf("✅ my_free(NULL) no causa crash\n");
    
    // Asignar tamaño muy grande (puede fallar)
    size_t huge_size = 1024UL * 1024 * 1024 * 10;  // 10 GB
    ptr = my_malloc(huge_size);
    if (ptr == NULL) {
        printf("✅ my_malloc(10GB) falló apropiadamente\n");
    } else {
        printf("⚠️  my_malloc(10GB) tuvo éxito (sistema con mucha memoria)\n");
        my_free(ptr);
    }
}

/*
 * ============================================================================
 * MAIN
 * ============================================================================
 */

int main() {
    printf("╔════════════════════════════════════════════════════════════════╗\n");
    printf("║  EJERCICIO 1: Implementar un Allocator Simple                 ║\n");
    printf("╚════════════════════════════════════════════════════════════════╝\n");
    
    printf("\n⚠️  NOTA: Este ejercicio requiere que implementes las funciones\n");
    printf("   my_malloc(), my_free(), y my_calloc() antes de ejecutar.\n");
    
    // Ejecutar tests
    test_basic_allocation();
    test_multiple_allocations();
    test_calloc();
    test_edge_cases();
    
    printf("\n╔════════════════════════════════════════════════════════════════╗\n");
    printf("║  Tests Completados                                             ║\n");
    printf("╚════════════════════════════════════════════════════════════════╝\n");
    
    printf("\n💡 Próximos pasos:\n");
    printf("   1. Ejecuta con valgrind para verificar memory leaks\n");
    printf("   2. Intenta los desafíos bonus\n");
    printf("   3. Compara tu implementación con malloc() real\n");
    
    return 0;
}

/*
 * ============================================================================
 * PISTAS PARA LA IMPLEMENTACIÓN
 * ============================================================================
 * 
 * Estrategia Simple (sin metadata):
 *   - Problema: my_free() no sabe cuánto liberar
 *   - Solución temporal: Guardar el tamaño en una variable global o array
 * 
 * Estrategia con Metadata:
 *   - Reserva espacio extra al inicio del bloque para guardar el tamaño
 *   - Estructura:
 *     [size_t: tamaño] [datos del usuario...]
 *                      ^
 *                      retornas este puntero
 *   - En my_free(), retrocede sizeof(size_t) para leer el tamaño
 * 
 * Ejemplo de Metadata:
 * 
 *   void* my_malloc(size_t size) {
 *       size_t total = sizeof(size_t) + size;
 *       size_t rounded = ((total + PAGE_SIZE - 1) / PAGE_SIZE) * PAGE_SIZE;
 *       
 *       void* mem = mmap(..., rounded, ...);
 *       if (mem == MAP_FAILED) return NULL;
 *       
 *       *(size_t*)mem = rounded;  // Guardar tamaño
 *       return (char*)mem + sizeof(size_t);  // Retornar después de metadata
 *   }
 *   
 *   void my_free(void* ptr) {
 *       if (!ptr) return;
 *       
 *       void* real_ptr = (char*)ptr - sizeof(size_t);
 *       size_t size = *(size_t*)real_ptr;
 *       
 *       munmap(real_ptr, size);
 *   }
 * 
 * ============================================================================
 */
