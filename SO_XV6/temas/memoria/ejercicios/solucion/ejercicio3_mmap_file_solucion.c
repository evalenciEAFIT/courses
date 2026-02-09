/*
 * ============================================================================
 * SOLUCIÓN EJERCICIO 3: Memory-Mapped Files
 * ============================================================================
 * 
 * Objetivo:
 *   Mapear archivos en memoria usando mmap() para procesamiento eficiente.
 *   Demostrar ventajas sobre read()/write() tradicionales.
 * 
 * Conceptos demostrados:
 *   - mmap() con archivos reales (no anónimos)
 *   - Procesamiento eficiente de archivos grandes
 *   - Modificación in-place de archivos
 *   - Comparación de rendimiento vs read/write
 * 
 * Compilación:
 *   gcc -Wall -Wextra -o ejercicio3_sol ejercicio3_mmap_file_solucion.c
 * 
 * Ejecución:
 *   ./ejercicio3_sol
 * ============================================================================
 */

#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <ctype.h>

#define TEST_FILE "test_data.txt"
#define LARGE_FILE "large_test.dat"
#define OUTPUT_FILE "output.txt"

/*
 * ============================================================================
 * FUNCIONES AUXILIARES
 * ============================================================================
 */

/**
 * Crea un archivo de prueba con contenido de texto
 */
void create_test_file(const char* filename, size_t size_kb) {
    FILE* f = fopen(filename, "w");
    if (!f) {
        perror("fopen");
        return;
    }
    
    const char* sample_text = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. ";
    size_t text_len = strlen(sample_text);
    size_t bytes_written = 0;
    size_t target = size_kb * 1024;
    
    while (bytes_written < target) {
        size_t to_write = (target - bytes_written) < text_len ? 
                          (target - bytes_written) : text_len;
        fwrite(sample_text, 1, to_write, f);
        bytes_written += to_write;
    }
    
    fclose(f);
    printf("✅ Archivo '%s' creado (%zu KB)\n", filename, size_kb);
}

/**
 * Obtiene el tamaño de un archivo
 */
off_t get_file_size(const char* filename) {
    struct stat st;
    if (stat(filename, &st) == -1) {
        perror("stat");
        return -1;
    }
    return st.st_size;
}

/**
 * Mide tiempo de ejecución en milisegundos
 */
double get_time_ms() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec * 1000.0 + ts.tv_nsec / 1000000.0;
}

/*
 * ============================================================================
 * DEMOSTRACIÓN 1: Lectura Básica con mmap()
 * ============================================================================
 */

void demo1_basic_read() {
    printf("\n╔════════════════════════════════════════════════════════════════╗\n");
    printf("║  Demo 1: Lectura Básica con mmap()                            ║\n");
    printf("╚════════════════════════════════════════════════════════════════╝\n\n");
    
    // Crear archivo de prueba
    create_test_file(TEST_FILE, 1);  // 1 KB
    
    // Abrir archivo
    int fd = open(TEST_FILE, O_RDONLY);
    if (fd == -1) {
        perror("open");
        return;
    }
    
    // Obtener tamaño
    off_t file_size = get_file_size(TEST_FILE);
    printf("Tamaño del archivo: %ld bytes\n\n", file_size);
    
    // Mapear archivo en memoria
    char* mapped = (char*)mmap(
        NULL,                   // Dirección: kernel elige
        file_size,              // Tamaño del archivo
        PROT_READ,              // Solo lectura
        MAP_PRIVATE,            // Privado (cambios no se escriben al archivo)
        fd,                     // File descriptor
        0                       // Offset: desde el inicio
    );
    
    if (mapped == MAP_FAILED) {
        perror("mmap");
        close(fd);
        return;
    }
    
    printf("✅ Archivo mapeado en memoria: %p\n\n", (void*)mapped);
    
    // Leer y mostrar primeros 100 caracteres
    printf("Primeros 100 caracteres:\n");
    printf("─────────────────────────────────────────────────────────────\n");
    for (int i = 0; i < 100 && i < file_size; i++) {
        putchar(mapped[i]);
    }
    printf("\n─────────────────────────────────────────────────────────────\n\n");
    
    // Contar palabras (ejemplo de procesamiento)
    int word_count = 0;
    int in_word = 0;
    for (off_t i = 0; i < file_size; i++) {
        if (isspace(mapped[i])) {
            in_word = 0;
        } else if (!in_word) {
            in_word = 1;
            word_count++;
        }
    }
    
    printf("Estadísticas:\n");
    printf("  - Bytes: %ld\n", file_size);
    printf("  - Palabras: %d\n", word_count);
    
    // Limpiar
    munmap(mapped, file_size);
    close(fd);
    unlink(TEST_FILE);
    
    printf("\n✅ Demo 1 completada\n");
}

/*
 * ============================================================================
 * DEMOSTRACIÓN 2: Modificación de Archivo con mmap()
 * ============================================================================
 */

void demo2_file_modification() {
    printf("\n╔════════════════════════════════════════════════════════════════╗\n");
    printf("║  Demo 2: Modificación de Archivo con mmap()                   ║\n");
    printf("╚════════════════════════════════════════════════════════════════╝\n\n");
    
    // Crear archivo de prueba
    const char* original_text = "hello world from mmap!";
    FILE* f = fopen(TEST_FILE, "w");
    fprintf(f, "%s", original_text);
    fclose(f);
    
    printf("Contenido original: \"%s\"\n\n", original_text);
    
    // Abrir para lectura/escritura
    int fd = open(TEST_FILE, O_RDWR);
    if (fd == -1) {
        perror("open");
        return;
    }
    
    off_t file_size = get_file_size(TEST_FILE);
    
    // Mapear con permisos de escritura
    char* mapped = (char*)mmap(
        NULL,
        file_size,
        PROT_READ | PROT_WRITE,  // Lectura + Escritura
        MAP_SHARED,              // SHARED: cambios se escriben al archivo
        fd,
        0
    );
    
    if (mapped == MAP_FAILED) {
        perror("mmap");
        close(fd);
        return;
    }
    
    printf("✅ Archivo mapeado con permisos de escritura\n\n");
    
    // Modificar contenido (convertir a mayúsculas)
    printf("Convirtiendo a mayúsculas...\n");
    for (off_t i = 0; i < file_size; i++) {
        mapped[i] = toupper(mapped[i]);
    }
    
    // Sincronizar cambios al disco
    if (msync(mapped, file_size, MS_SYNC) == -1) {
        perror("msync");
    } else {
        printf("✅ Cambios sincronizados al disco\n\n");
    }
    
    // Limpiar
    munmap(mapped, file_size);
    close(fd);
    
    // Verificar cambios leyendo el archivo
    f = fopen(TEST_FILE, "r");
    char buffer[256];
    fgets(buffer, sizeof(buffer), f);
    fclose(f);
    
    printf("Contenido modificado: \"%s\"\n", buffer);
    
    unlink(TEST_FILE);
    printf("\n✅ Demo 2 completada\n");
}

/*
 * ============================================================================
 * DEMOSTRACIÓN 3: Comparación de Rendimiento mmap() vs read()
 * ============================================================================
 */

void demo3_performance_comparison() {
    printf("\n╔════════════════════════════════════════════════════════════════╗\n");
    printf("║  Demo 3: Comparación de Rendimiento mmap() vs read()          ║\n");
    printf("╚════════════════════════════════════════════════════════════════╝\n\n");
    
    const size_t file_size_mb = 10;  // 10 MB
    create_test_file(LARGE_FILE, file_size_mb * 1024);
    
    off_t file_size = get_file_size(LARGE_FILE);
    
    // ═══════════════════════════════════════════════════════════════
    // Método 1: read() tradicional
    // ═══════════════════════════════════════════════════════════════
    
    printf("Método 1: read() tradicional\n");
    printf("─────────────────────────────────────────────────────────────\n");
    
    double start = get_time_ms();
    
    int fd = open(LARGE_FILE, O_RDONLY);
    if (fd == -1) {
        perror("open");
        return;
    }
    
    char* buffer = (char*)malloc(file_size);
    ssize_t bytes_read = read(fd, buffer, file_size);
    
    // Procesar (contar caracteres 'e')
    int count_read = 0;
    for (ssize_t i = 0; i < bytes_read; i++) {
        if (buffer[i] == 'e') count_read++;
    }
    
    free(buffer);
    close(fd);
    
    double time_read = get_time_ms() - start;
    
    printf("  - Bytes leídos: %ld\n", bytes_read);
    printf("  - Caracteres 'e' encontrados: %d\n", count_read);
    printf("  - Tiempo: %.2f ms\n\n", time_read);
    
    // ═══════════════════════════════════════════════════════════════
    // Método 2: mmap()
    // ═══════════════════════════════════════════════════════════════
    
    printf("Método 2: mmap()\n");
    printf("─────────────────────────────────────────────────────────────\n");
    
    start = get_time_ms();
    
    fd = open(LARGE_FILE, O_RDONLY);
    if (fd == -1) {
        perror("open");
        return;
    }
    
    char* mapped = (char*)mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (mapped == MAP_FAILED) {
        perror("mmap");
        close(fd);
        return;
    }
    
    // Procesar (contar caracteres 'e')
    int count_mmap = 0;
    for (off_t i = 0; i < file_size; i++) {
        if (mapped[i] == 'e') count_mmap++;
    }
    
    munmap(mapped, file_size);
    close(fd);
    
    double time_mmap = get_time_ms() - start;
    
    printf("  - Bytes procesados: %ld\n", file_size);
    printf("  - Caracteres 'e' encontrados: %d\n", count_mmap);
    printf("  - Tiempo: %.2f ms\n\n", time_mmap);
    
    // ═══════════════════════════════════════════════════════════════
    // Comparación
    // ═══════════════════════════════════════════════════════════════
    
    printf("Comparación:\n");
    printf("─────────────────────────────────────────────────────────────\n");
    printf("  - read():  %.2f ms\n", time_read);
    printf("  - mmap():  %.2f ms\n", time_mmap);
    
    if (time_mmap < time_read) {
        printf("  - ✅ mmap() es %.2fx más rápido\n", time_read / time_mmap);
    } else {
        printf("  - ⚠️  read() es %.2fx más rápido (inusual)\n", time_mmap / time_read);
    }
    
    unlink(LARGE_FILE);
    printf("\n✅ Demo 3 completada\n");
}

/*
 * ============================================================================
 * DEMOSTRACIÓN 4: Acceso Aleatorio Eficiente
 * ============================================================================
 */

typedef struct record {
    int id;
    char name[32];
    double value;
} record_t;

void demo4_random_access() {
    printf("\n╔════════════════════════════════════════════════════════════════╗\n");
    printf("║  Demo 4: Acceso Aleatorio Eficiente                           ║\n");
    printf("╚════════════════════════════════════════════════════════════════╝\n\n");
    
    const int num_records = 1000;
    const char* db_file = "records.dat";
    
    // Crear archivo de "base de datos"
    printf("Creando base de datos con %d registros...\n", num_records);
    
    FILE* f = fopen(db_file, "wb");
    for (int i = 0; i < num_records; i++) {
        record_t rec;
        rec.id = i;
        snprintf(rec.name, sizeof(rec.name), "Record_%d", i);
        rec.value = i * 3.14;
        fwrite(&rec, sizeof(record_t), 1, f);
    }
    fclose(f);
    
    off_t file_size = get_file_size(db_file);
    printf("✅ Base de datos creada: %ld bytes\n\n", file_size);
    
    // Mapear archivo
    int fd = open(db_file, O_RDWR);
    if (fd == -1) {
        perror("open");
        return;
    }
    
    record_t* records = (record_t*)mmap(
        NULL,
        file_size,
        PROT_READ | PROT_WRITE,
        MAP_SHARED,
        fd,
        0
    );
    
    if (records == MAP_FAILED) {
        perror("mmap");
        close(fd);
        return;
    }
    
    printf("✅ Base de datos mapeada en memoria\n\n");
    
    // Acceso aleatorio super rápido
    printf("Acceso aleatorio a registros:\n");
    printf("─────────────────────────────────────────────────────────────\n");
    
    int indices[] = {0, 100, 500, 999};
    for (int i = 0; i < 4; i++) {
        int idx = indices[i];
        printf("  Registro #%d:\n", idx);
        printf("    ID: %d\n", records[idx].id);
        printf("    Nombre: %s\n", records[idx].name);
        printf("    Valor: %.2f\n\n", records[idx].value);
    }
    
    // Modificación in-place
    printf("Modificando registro #500...\n");
    records[500].value = 999.99;
    strcpy(records[500].name, "MODIFIED");
    
    msync(records, file_size, MS_SYNC);
    printf("✅ Cambios sincronizados\n\n");
    
    // Verificar modificación
    printf("Verificando modificación:\n");
    printf("  Registro #500:\n");
    printf("    ID: %d\n", records[500].id);
    printf("    Nombre: %s\n", records[500].name);
    printf("    Valor: %.2f\n", records[500].value);
    
    // Limpiar
    munmap(records, file_size);
    close(fd);
    unlink(db_file);
    
    printf("\n✅ Demo 4 completada\n");
}

/*
 * ============================================================================
 * MAIN
 * ============================================================================
 */

int main() {
    printf("╔════════════════════════════════════════════════════════════════╗\n");
    printf("║  SOLUCIÓN EJERCICIO 3: Memory-Mapped Files                    ║\n");
    printf("╚════════════════════════════════════════════════════════════════╝\n");
    
    printf("\n📚 Este programa demuestra 4 usos de memory-mapped files:\n");
    printf("   1. Lectura básica de archivos\n");
    printf("   2. Modificación de archivos\n");
    printf("   3. Comparación de rendimiento vs read()\n");
    printf("   4. Acceso aleatorio eficiente (base de datos)\n");
    
    demo1_basic_read();
    demo2_file_modification();
    demo3_performance_comparison();
    demo4_random_access();
    
    printf("\n╔════════════════════════════════════════════════════════════════╗\n");
    printf("║  ✅ Todas las demostraciones completadas                      ║\n");
    printf("╚════════════════════════════════════════════════════════════════╝\n");
    
    printf("\n💡 Ventajas de mmap() para archivos:\n");
    printf("   • Acceso aleatorio muy eficiente\n");
    printf("   • Modificación in-place sin buffers intermedios\n");
    printf("   • El kernel maneja el caching automáticamente\n");
    printf("   • Ideal para archivos grandes y bases de datos\n");
    printf("   • Simplifica el código (no necesitas read/write loops)\n");
    
    printf("\n⚠️  Consideraciones:\n");
    printf("   • Usa MAP_SHARED para persistir cambios al archivo\n");
    printf("   • Usa MAP_PRIVATE para cambios temporales\n");
    printf("   • Llama msync() para forzar escritura a disco\n");
    printf("   • Ten cuidado con archivos que cambian de tamaño\n");
    
    return 0;
}
