#include <stdio.h>
#include <sys/stat.h> // LIBRERÍA CLAVE para metadatos
#include <stdlib.h>

/**
 * PROGRAMA 21: estadisticas_archivo.c (struct stat)
 * --------------------------------------------------------------------------
 * CONCEPTO ACADÉMICO:
 * El "Inodo" es una estructura en el disco que guarda TODO sobre un archivo,
 * MENOS su nombre y su contenido. stat() lee ese Inodo.
 * 
 * ¿Por qué usamos stat()?
 * - Para saber el tamaño sin abrir el archivo.
 * - Para ver cuántos bloques físicos de 512 bytes ocupa en realidad.
 * --------------------------------------------------------------------------
 */

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Uso: %s <archivo_a_analizar>\n", argv[0]);
        return EXIT_FAILURE;
    }

    struct stat info; // Estructura donde se guardarán los resultados

    // stat() toma el nombre del archivo y una referencia a nuestra estructura.
    if (stat(argv[1], &info) == -1) {
        perror("Error al obtener estadísticas");
        return EXIT_FAILURE;
    }

    printf("--- Análisis de METADATOS para: %s ---\n", argv[1]);
    
    // st_size: Tamaño lógico en bytes
    printf("> Tamaño lógico: %ld bytes\n", info.st_size);
    
    // st_blocks: Cantidad de bloques de disco de 512B asignados
    // Útil para detectar "archivos huecos" (sparse files).
    printf("> Bloques de disco: %ld blocs (512B c/u)\n", info.st_blocks);
    
    // st_ino: Número de Inodo (ID único del archivo en este disco)
    printf("> Número de Inodo: %ld\n", info.st_ino);

    return EXIT_SUCCESS;
}

/**
 * REFLEXIÓN ACADÉMICA: 
 * st_blocks * 512 suele ser mayor que st_size. Esto es porque el disco 
 * asigna bloques enteros. Si escribes 1 byte, el disco gasta un bloque entero (ej: 4KB).
 */
