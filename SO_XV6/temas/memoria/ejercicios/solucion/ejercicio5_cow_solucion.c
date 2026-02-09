/*
 * ============================================================================
 * SOLUCIÓN EJERCICIO 5: Implementar Copy-on-Write Manual
 * ============================================================================
 * 
 * Objetivo:
 *   Simular el mecanismo de Copy-on-Write (COW) del kernel usando mprotect()
 *   y signal handlers. Este es el ejercicio más avanzado.
 * 
 * Conceptos demostrados:
 *   - mprotect() para cambiar permisos de páginas
 *   - Signal handlers (SIGSEGV)
 *   - Copy-on-Write manual
 *   - Page protection y fault handling
 *   - Metadata de páginas
 * 
 * Compilación:
 *   gcc -Wall -Wextra -o ejercicio5_sol ejercicio5_cow_solucion.c
 * 
 * Ejecución:
 *   ./ejercicio5_sol
 * ============================================================================
 */

#define _DEFAULT_SOURCE
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <signal.h>
#include <unistd.h>
#include <stdint.h>

#define PAGE_SIZE 4096
#define MAX_PAGES 1024

/*
 * ============================================================================
 * ESTRUCTURAS DE DATOS
 * ============================================================================
 */

/**
 * Metadata de una página COW
 */
typedef struct page_info {
    void* address;              // Dirección de la página
    void* original_data;        // Datos originales (compartidos)
    int is_cow;                 // 1 = COW activo, 0 = página privada
    int ref_count;              // Número de referencias
} page_info_t;

/**
 * Gestor global de páginas COW
 */
typedef struct cow_manager {
    page_info_t pages[MAX_PAGES];
    int num_pages;
    void* base_address;
    size_t total_size;
} cow_manager_t;

static cow_manager_t g_cow_manager = {0};

/*
 * ============================================================================
 * SIGNAL HANDLER PARA SIGSEGV
 * ============================================================================
 */

/**
 * Handler para SIGSEGV (Segmentation Fault)
 * 
 * Cuando se intenta escribir en una página COW protegida:
 * 1. Se recibe SIGSEGV
 * 2. Identificamos la página
 * 3. Creamos una copia privada
 * 4. Cambiamos permisos a READ|WRITE
 * 5. Retornamos (la instrucción se reintenta automáticamente)
 */
void sigsegv_handler(int sig, siginfo_t* si, void* unused) {
    (void)sig;
    (void)unused;
    
    void* fault_addr = si->si_addr;
    
    // Alinear a límite de página
    void* page_addr = (void*)((uintptr_t)fault_addr & ~(PAGE_SIZE - 1));
    
    printf("\n[SIGSEGV] Fault en dirección: %p (página: %p)\n", 
           fault_addr, page_addr);
    
    // Buscar la página en nuestro gestor
    for (int i = 0; i < g_cow_manager.num_pages; i++) {
        page_info_t* page = &g_cow_manager.pages[i];
        
        if (page->address == page_addr && page->is_cow) {
            printf("[COW] Página COW detectada, creando copia privada...\n");
            
            // Crear nueva página privada
            void* private_page = mmap(
                NULL,
                PAGE_SIZE,
                PROT_READ | PROT_WRITE,
                MAP_PRIVATE | MAP_ANONYMOUS,
                -1,
                0
            );
            
            if (private_page == MAP_FAILED) {
                fprintf(stderr, "Error: No se pudo crear página privada\n");
                exit(1);
            }
            
            // Copiar datos originales a la nueva página
            memcpy(private_page, page->original_data, PAGE_SIZE);
            
            // Cambiar permisos de la página original a READ|WRITE
            if (mprotect(page->address, PAGE_SIZE, PROT_READ | PROT_WRITE) == -1) {
                perror("mprotect");
                exit(1);
            }
            
            // Copiar datos de la página privada a la original
            memcpy(page->address, private_page, PAGE_SIZE);
            
            // Liberar página temporal
            munmap(private_page, PAGE_SIZE);
            
            // Actualizar metadata
            page->is_cow = 0;  // Ya no es COW
            page->ref_count--;
            
            printf("[COW] ✅ Copia completada, página ahora es privada\n");
            printf("[COW] Referencias restantes: %d\n\n", page->ref_count);
            
            return;  // Retornar para reintentar la instrucción
        }
    }
    
    // Si llegamos aquí, es un SIGSEGV real (no COW)
    fprintf(stderr, "ERROR: Segmentation fault real en %p\n", fault_addr);
    exit(1);
}

/**
 * Instala el signal handler para SIGSEGV
 */
void install_sigsegv_handler() {
    struct sigaction sa;
    
    sa.sa_flags = SA_SIGINFO;
    sigemptyset(&sa.sa_mask);
    sa.sa_sigaction = sigsegv_handler;
    
    if (sigaction(SIGSEGV, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }
    
    printf("✅ Signal handler instalado para SIGSEGV\n");
}

/*
 * ============================================================================
 * FUNCIONES DE GESTIÓN COW
 * ============================================================================
 */

/**
 * Inicializa el gestor COW con una región de memoria
 */
void* cow_init(size_t size) {
    // Redondear al múltiplo de PAGE_SIZE
    size_t rounded_size = ((size + PAGE_SIZE - 1) / PAGE_SIZE) * PAGE_SIZE;
    int num_pages = rounded_size / PAGE_SIZE;
    
    if (num_pages > MAX_PAGES) {
        fprintf(stderr, "Error: Demasiadas páginas (%d > %d)\n", 
                num_pages, MAX_PAGES);
        return NULL;
    }
    
    // Mapear memoria
    void* mem = mmap(
        NULL,
        rounded_size,
        PROT_READ | PROT_WRITE,
        MAP_PRIVATE | MAP_ANONYMOUS,
        -1,
        0
    );
    
    if (mem == MAP_FAILED) {
        perror("mmap");
        return NULL;
    }
    
    // Inicializar gestor
    g_cow_manager.base_address = mem;
    g_cow_manager.total_size = rounded_size;
    g_cow_manager.num_pages = num_pages;
    
    // Inicializar metadata de páginas
    for (int i = 0; i < num_pages; i++) {
        g_cow_manager.pages[i].address = (char*)mem + (i * PAGE_SIZE);
        g_cow_manager.pages[i].original_data = NULL;
        g_cow_manager.pages[i].is_cow = 0;
        g_cow_manager.pages[i].ref_count = 1;
    }
    
    printf("✅ Gestor COW inicializado\n");
    printf("   Dirección base: %p\n", mem);
    printf("   Tamaño: %zu bytes (%d páginas)\n", rounded_size, num_pages);
    
    return mem;
}

/**
 * Activa COW en todas las páginas
 */
void cow_enable(void* mem) {
    printf("\n🔒 Activando COW en todas las páginas...\n");
    
    for (int i = 0; i < g_cow_manager.num_pages; i++) {
        page_info_t* page = &g_cow_manager.pages[i];
        
        // Crear copia de los datos originales
        page->original_data = mmap(
            NULL,
            PAGE_SIZE,
            PROT_READ | PROT_WRITE,
            MAP_PRIVATE | MAP_ANONYMOUS,
            -1,
            0
        );
        
        if (page->original_data == MAP_FAILED) {
            perror("mmap original_data");
            return;
        }
        
        // Copiar datos actuales
        memcpy(page->original_data, page->address, PAGE_SIZE);
        
        // Proteger página (solo lectura)
        if (mprotect(page->address, PAGE_SIZE, PROT_READ) == -1) {
            perror("mprotect");
            return;
        }
        
        page->is_cow = 1;
        page->ref_count = 2;  // Original + esta referencia
    }
    
    printf("✅ COW activado en %d páginas\n", g_cow_manager.num_pages);
}

/**
 * Limpia el gestor COW
 */
void cow_cleanup() {
    printf("\n🧹 Limpiando gestor COW...\n");
    
    for (int i = 0; i < g_cow_manager.num_pages; i++) {
        page_info_t* page = &g_cow_manager.pages[i];
        
        if (page->original_data) {
            munmap(page->original_data, PAGE_SIZE);
        }
    }
    
    if (g_cow_manager.base_address) {
        munmap(g_cow_manager.base_address, g_cow_manager.total_size);
    }
    
    printf("✅ Limpieza completada\n");
}

/*
 * ============================================================================
 * DEMOSTRACIONES
 * ============================================================================
 */

/**
 * Demo 1: COW Básico
 */
void demo1_basic_cow() {
    printf("\n╔════════════════════════════════════════════════════════════════╗\n");
    printf("║  Demo 1: Copy-on-Write Básico                                 ║\n");
    printf("╚════════════════════════════════════════════════════════════════╝\n\n");
    
    const size_t size = 3 * PAGE_SIZE;
    
    // Inicializar
    char* mem = (char*)cow_init(size);
    if (!mem) return;
    
    // Llenar con datos
    printf("\n📝 Escribiendo datos iniciales...\n");
    strcpy(mem, "Página 0: Datos originales");
    strcpy(mem + PAGE_SIZE, "Página 1: Más datos");
    strcpy(mem + 2 * PAGE_SIZE, "Página 2: Últimos datos");
    
    printf("   Página 0: \"%s\"\n", mem);
    printf("   Página 1: \"%s\"\n", mem + PAGE_SIZE);
    printf("   Página 2: \"%s\"\n", mem + 2 * PAGE_SIZE);
    
    // Activar COW
    cow_enable(mem);
    
    // Intentar leer (no causa fault)
    printf("\n📖 Leyendo datos (no causa fault)...\n");
    printf("   Página 0: \"%s\"\n", mem);
    printf("   ✅ Lectura exitosa\n");
    
    // Intentar escribir (causa fault y COW)
    printf("\n✏️  Escribiendo en página 0 (causa COW)...\n");
    strcpy(mem, "Página 0: MODIFICADA!");
    printf("   ✅ Escritura exitosa\n");
    printf("   Nuevo contenido: \"%s\"\n", mem);
    
    // Escribir en página 1
    printf("\n✏️  Escribiendo en página 1 (causa COW)...\n");
    strcpy(mem + PAGE_SIZE, "Página 1: TAMBIÉN MODIFICADA!");
    printf("   ✅ Escritura exitosa\n");
    printf("   Nuevo contenido: \"%s\"\n", mem + PAGE_SIZE);
    
    // Página 2 sigue siendo COW
    printf("\n📊 Estado final:\n");
    printf("   Página 0: Privada (modificada)\n");
    printf("   Página 1: Privada (modificada)\n");
    printf("   Página 2: COW (no modificada)\n");
    
    cow_cleanup();
    printf("\n✅ Demo 1 completada\n");
}

/**
 * Demo 2: Múltiples Escrituras
 */
void demo2_multiple_writes() {
    printf("\n╔════════════════════════════════════════════════════════════════╗\n");
    printf("║  Demo 2: Múltiples Escrituras                                  ║\n");
    printf("╚════════════════════════════════════════════════════════════════╝\n\n");
    
    const size_t size = 5 * PAGE_SIZE;
    int* mem = (int*)cow_init(size);
    if (!mem) return;
    
    // Llenar con números
    printf("📝 Inicializando array de enteros...\n");
    int num_ints = size / sizeof(int);
    for (int i = 0; i < num_ints; i++) {
        mem[i] = i;
    }
    printf("   Array inicializado con %d enteros\n", num_ints);
    
    // Activar COW
    cow_enable(mem);
    
    // Escribir en diferentes páginas
    printf("\n✏️  Modificando valores en diferentes páginas...\n");
    
    int indices[] = {0, 256, 512, 768, 1000};  // Diferentes páginas
    for (int i = 0; i < 5; i++) {
        int idx = indices[i];
        printf("   Modificando mem[%d] (página %d)...\n", idx, idx / (PAGE_SIZE / sizeof(int)));
        mem[idx] = 9999;
        printf("   ✅ mem[%d] = %d\n", idx, mem[idx]);
    }
    
    printf("\n📊 Páginas privadas creadas: 5\n");
    
    cow_cleanup();
    printf("\n✅ Demo 2 completada\n");
}

/**
 * Demo 3: Estadísticas de COW
 */
void demo3_cow_statistics() {
    printf("\n╔════════════════════════════════════════════════════════════════╗\n");
    printf("║  Demo 3: Estadísticas de COW                                   ║\n");
    printf("╚════════════════════════════════════════════════════════════════╝\n\n");
    
    const size_t size = 10 * PAGE_SIZE;
    char* mem = (char*)cow_init(size);
    if (!mem) return;
    
    // Llenar memoria
    memset(mem, 'A', size);
    
    // Activar COW
    cow_enable(mem);
    
    printf("📊 Estado inicial:\n");
    printf("   Total de páginas: %d\n", g_cow_manager.num_pages);
    printf("   Páginas COW: %d\n", g_cow_manager.num_pages);
    printf("   Páginas privadas: 0\n\n");
    
    // Modificar algunas páginas
    printf("✏️  Modificando 3 de 10 páginas...\n");
    mem[0] = 'X';                    // Página 0
    mem[PAGE_SIZE * 3] = 'Y';        // Página 3
    mem[PAGE_SIZE * 7] = 'Z';        // Página 7
    
    // Contar páginas privadas
    int cow_pages = 0;
    int private_pages = 0;
    for (int i = 0; i < g_cow_manager.num_pages; i++) {
        if (g_cow_manager.pages[i].is_cow) {
            cow_pages++;
        } else {
            private_pages++;
        }
    }
    
    printf("\n📊 Estado final:\n");
    printf("   Total de páginas: %d\n", g_cow_manager.num_pages);
    printf("   Páginas COW: %d\n", cow_pages);
    printf("   Páginas privadas: %d\n", private_pages);
    printf("   Memoria ahorrada: %d páginas (%.1f KB)\n", 
           cow_pages, (cow_pages * PAGE_SIZE) / 1024.0);
    
    printf("\n💡 Beneficio de COW:\n");
    printf("   • Solo se copian las páginas modificadas\n");
    printf("   • Ahorro de memoria: %d%%\n", (cow_pages * 100) / g_cow_manager.num_pages);
    printf("   • Ahorro de tiempo en fork()\n");
    
    cow_cleanup();
    printf("\n✅ Demo 3 completada\n");
}

/*
 * ============================================================================
 * MAIN
 * ============================================================================
 */

int main() {
    printf("╔════════════════════════════════════════════════════════════════╗\n");
    printf("║  SOLUCIÓN EJERCICIO 5: Copy-on-Write Manual                   ║\n");
    printf("╚════════════════════════════════════════════════════════════════╝\n");
    
    printf("\n⚠️  ADVERTENCIA: Este programa usa signal handlers avanzados\n");
    printf("   Implementa COW manualmente usando mprotect() y SIGSEGV\n\n");
    
    // Instalar signal handler
    install_sigsegv_handler();
    
    printf("\n📚 Este programa demuestra 3 aspectos de COW:\n");
    printf("   1. Funcionamiento básico de COW\n");
    printf("   2. Múltiples escrituras en diferentes páginas\n");
    printf("   3. Estadísticas y ahorro de memoria\n");
    
    demo1_basic_cow();
    demo2_multiple_writes();
    demo3_cow_statistics();
    
    printf("\n╔════════════════════════════════════════════════════════════════╗\n");
    printf("║  ✅ Todas las demostraciones completadas                      ║\n");
    printf("╚════════════════════════════════════════════════════════════════╝\n");
    
    printf("\n💡 Conceptos clave aprendidos:\n");
    printf("   • COW retrasa la copia hasta que sea necesaria\n");
    printf("   • mprotect() cambia permisos de páginas\n");
    printf("   • SIGSEGV se usa para detectar escrituras\n");
    printf("   • Signal handlers permiten implementar COW en userspace\n");
    printf("   • El kernel usa COW en fork() para eficiencia\n");
    
    printf("\n🎓 Aplicaciones de COW:\n");
    printf("   • fork() en Linux\n");
    printf("   • Snapshots en sistemas de archivos (ZFS, Btrfs)\n");
    printf("   • Máquinas virtuales (KVM)\n");
    printf("   • Contenedores (Docker)\n");
    
    return 0;
}
